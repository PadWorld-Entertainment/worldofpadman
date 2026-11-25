/*
 * wiiu_snd.c -- AXVoice audio backend. Splits ioq3's interleaved stereo
 * ring buffer into two mono buffers fed to AXVoice L/R via DMA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <coreinit/cache.h>
#include <coreinit/time.h>
#include <sndcore2/core.h>
#include <sndcore2/voice.h>
#include <sndcore2/device.h>
#include <sndcore2/result.h>

#include "qcommon/q_shared.h"
#include "qcommon/qcommon.h"
#include "client/snd_local.h"
#include "../audio/wiiu_snd.h"

/* -----------------------------------------------------------------------
 * Configuration
 * ----------------------------------------------------------------------- */
#define SND_FREQ        48000
#define SND_CHANNELS    2
#define SND_SAMPLEBITS  16

/* Ring size in sample-pairs, ~85ms at 48kHz, must be power of two. */
#define SND_SAMPLES     32768
#define SND_INTERLEAVED (SND_SAMPLES * SND_CHANNELS)
#define SND_RING_BYTES  (SND_SAMPLES * (SND_SAMPLEBITS / 8))

/* Mixer paints at most one half-ring ahead of the read head. */
#define SND_SUBMISSION  2048

/* AXVoice priority -- mid-range, high enough to not be stolen. */
#define SND_VOICE_PRIO  20

/* -----------------------------------------------------------------------
 * State
 * ----------------------------------------------------------------------- */
static qboolean s_ax_init    = qfalse;
static qboolean s_snd_active = qfalse;

/* Interleaved ring buffer that ioq3's mixer writes into. */
static byte *s_dma_buf = NULL;

/* Per-channel mono buffers AXVoice reads via DMA, 64-byte aligned for dcache flush. */
static int16_t *s_buf_left  = NULL;
static int16_t *s_buf_right = NULL;

/* AXVoice handles -- one per channel. */
static AXVoice *s_voice_left  = NULL;
static AXVoice *s_voice_right = NULL;

/* Fired if AX force-steals our voice; just mark the pointer NULL. */
static void wiiu_voice_dropped(void *ctx)
{
    AXVoice **slot = (AXVoice **)ctx;
    *slot = NULL;
}

/* -----------------------------------------------------------------------
 * Internal helpers
 * ----------------------------------------------------------------------- */

/* Configure one AXVoice for mono LPCM16 looping playback. */
static qboolean wiiu_setup_voice(AXVoice *voice, const int16_t *buf,
                                 qboolean is_left)
{
    if (!voice)
        return qfalse;

    AXVoiceBegin(voice);

    /* Sample buffer and loop configuration. */
    AXVoiceOffsets offsets;
    memset(&offsets, 0, sizeof(offsets));
    offsets.dataType       = AX_VOICE_FORMAT_LPCM16;
    offsets.loopingEnabled = AX_VOICE_LOOP_ENABLED;
    offsets.loopOffset     = 0;
    offsets.endOffset      = SND_SAMPLES - 1;
    offsets.currentOffset  = 0;
    offsets.data           = buf;
    AXSetVoiceOffsets(voice, &offsets);

    /* Sample rate conversion: native 48 kHz to 48 kHz = ratio 1.0. */
    AXSetVoiceSrcType(voice, AX_VOICE_SRC_TYPE_NONE);
    AXSetVoiceSrcRatio(voice, 1.0f);

    /* Volume envelope at full volume. */
    AXVoiceVeData ve;
    ve.volume = 0x8000;
    ve.delta  = 0;
    AXSetVoiceVe(voice, &ve);

    /* Route to TV (6ch) and DRC (2ch), full volume on this voice's channel, silence elsewhere. */
    int target_ch = is_left ? 0 : 1;

    /* TV output -- full scale. */
    AXVoiceDeviceMixData tv_mix[6];
    memset(tv_mix, 0, sizeof(tv_mix));
    tv_mix[target_ch].bus[0].volume = 0x8000;
    for (int ch = 0; ch < 6; ch++)
        AXSetVoiceDeviceMix(voice, AX_DEVICE_TYPE_TV, ch, &tv_mix[ch]);

    /* DRC headroom vs TV full-scale -- wireless codec crunches otherwise. Still not fully fixed. */
    AXVoiceDeviceMixData drc_mix[2];
    memset(drc_mix, 0, sizeof(drc_mix));
    drc_mix[target_ch].bus[0].volume = 0x5000;
    for (int ch = 0; ch < 2; ch++)
        AXSetVoiceDeviceMix(voice, AX_DEVICE_TYPE_DRC, ch, &drc_mix[ch]);

    AXSetVoiceState(voice, AX_VOICE_STATE_PLAYING);

    AXVoiceEnd(voice);
    return qtrue;
}

/* Acquire and configure both voices; returns qtrue if both ready. */
static qboolean wiiu_acquire_voices(void)
{
    /* Left channel voice. */
    s_voice_left = AXAcquireVoice(SND_VOICE_PRIO,
                                  wiiu_voice_dropped,
                                  &s_voice_left);
    if (!s_voice_left)
        return qfalse;

    /* Right channel voice. */
    s_voice_right = AXAcquireVoice(SND_VOICE_PRIO,
                                   wiiu_voice_dropped,
                                   &s_voice_right);
    if (!s_voice_right) {
        AXFreeVoice(s_voice_left);
        s_voice_left = NULL;
        return qfalse;
    }

    if (!wiiu_setup_voice(s_voice_left, s_buf_left, qtrue)) {
        AXFreeVoice(s_voice_left);
        AXFreeVoice(s_voice_right);
        s_voice_left = s_voice_right = NULL;
        return qfalse;
    }

    if (!wiiu_setup_voice(s_voice_right, s_buf_right, qfalse)) {
        AXSetVoiceState(s_voice_left, AX_VOICE_STATE_STOPPED);
        AXFreeVoice(s_voice_left);
        AXFreeVoice(s_voice_right);
        s_voice_left = s_voice_right = NULL;
        return qfalse;
    }

    return qtrue;
}

/* Release both voices if they exist. */
static void wiiu_release_voices(void)
{
    if (s_voice_left) {
        AXSetVoiceState(s_voice_left, AX_VOICE_STATE_STOPPED);
        AXFreeVoice(s_voice_left);
        s_voice_left = NULL;
    }
    if (s_voice_right) {
        AXSetVoiceState(s_voice_right, AX_VOICE_STATE_STOPPED);
        AXFreeVoice(s_voice_right);
        s_voice_right = NULL;
    }
}

/* Public API, called from wiiu_main.c */

static void snd_log(const char *msg)
{
#ifdef WIIU_DEBUG
    FILE *f = fopen("fs:/vol/external01/quake3/log.txt", "a");
    if (f) { fprintf(f, "%s\n", msg); fclose(f); }
#endif
    (void)msg;
}

void WiiU_Snd_Init(void)
{
    if (s_ax_init)
        return;

    snd_log("AXInitWithParams start");
    AXInitParams params;
    memset(&params, 0, sizeof(params));
    params.renderer = AX_INIT_RENDERER_48KHZ;
    params.pipeline = AX_INIT_PIPELINE_SINGLE;
    AXInitWithParams(&params);
    snd_log("AXInitWithParams done");

    s_ax_init = qtrue;
}

static void wiiu_snd_stop(void)
{
    if (s_snd_active) {
        wiiu_release_voices();
        s_snd_active = qfalse;
    }

    if (s_dma_buf) { free(s_dma_buf); s_dma_buf = NULL; }
    if (s_buf_left) { free(s_buf_left); s_buf_left = NULL; }
    if (s_buf_right) { free(s_buf_right); s_buf_right = NULL; }
}

void WiiU_Snd_Shutdown(void)
{
    wiiu_snd_stop();

    if (s_ax_init) {
        AXQuit();
        s_ax_init = qfalse;
    }
}

/* ProcUI foreground/background transitions (Home menu invoked/dismissed). */
void WiiU_Snd_ReleaseForeground(void)
{
    if (!s_snd_active)
        return;
    wiiu_release_voices();
}

void WiiU_Snd_AcquireForeground(void)
{
    if (!s_snd_active || !s_ax_init)
        return;

    /* Zero buffers to avoid playing stale data. */
    memset(s_buf_left, 0, SND_RING_BYTES);
    memset(s_buf_right, 0, SND_RING_BYTES);
    DCFlushRange(s_buf_left, SND_RING_BYTES);
    DCFlushRange(s_buf_right, SND_RING_BYTES);

    wiiu_acquire_voices();
}

static int s_snd_pause_depth = 0;

void WiiU_Snd_SetPaused(qboolean paused)
{
    if (!s_snd_active)
        return;
    if (paused) {
        s_snd_pause_depth++;
        if (s_snd_pause_depth == 1) {
            if (s_buf_left) {
                memset(s_buf_left, 0, SND_RING_BYTES);
                DCFlushRange(s_buf_left, SND_RING_BYTES);
            }
            if (s_buf_right) {
                memset(s_buf_right, 0, SND_RING_BYTES);
                DCFlushRange(s_buf_right, SND_RING_BYTES);
            }
            if (s_voice_left)
                AXSetVoiceState(s_voice_left, AX_VOICE_STATE_STOPPED);
            if (s_voice_right)
                AXSetVoiceState(s_voice_right, AX_VOICE_STATE_STOPPED);
        }
    } else {
        if (s_snd_pause_depth > 0)
            s_snd_pause_depth--;
        if (s_snd_pause_depth == 0) {
            if (s_voice_left)
                AXSetVoiceState(s_voice_left, AX_VOICE_STATE_PLAYING);
            if (s_voice_right)
                AXSetVoiceState(s_voice_right, AX_VOICE_STATE_PLAYING);
        }
    }
}

/* SNDDMA interface, called by snd_dma.c */

qboolean SNDDMA_Init(void)
{
    WiiU_Snd_Init();
    if (!s_ax_init)
        return qfalse;

    /* Interleaved DMA ring buffer for ioq3's mixer. */
    s_dma_buf = (byte *)memalign(64,
                    SND_INTERLEAVED * (SND_SAMPLEBITS / 8));
    if (!s_dma_buf)
        return qfalse;
    memset(s_dma_buf, 0, SND_INTERLEAVED * (SND_SAMPLEBITS / 8));

    /* Per-channel mono buffers for AXVoice DMA. */
    s_buf_left = (int16_t *)memalign(64, SND_RING_BYTES);
    s_buf_right = (int16_t *)memalign(64, SND_RING_BYTES);
    if (!s_buf_left || !s_buf_right) {
        free(s_dma_buf); s_dma_buf = NULL;
        free(s_buf_left); s_buf_left = NULL;
        free(s_buf_right); s_buf_right = NULL;
        return qfalse;
    }
    memset(s_buf_left, 0, SND_RING_BYTES);
    memset(s_buf_right, 0, SND_RING_BYTES);
    DCFlushRange(s_buf_left, SND_RING_BYTES);
    DCFlushRange(s_buf_right, SND_RING_BYTES);

    /* Describe the ring to ioq3's mixer. */
    dma.samplebits       = SND_SAMPLEBITS;
    dma.isfloat          = 0;
    dma.speed            = SND_FREQ;
    dma.channels         = SND_CHANNELS;
    dma.samples          = SND_INTERLEAVED;
    dma.fullsamples      = SND_SAMPLES;
    dma.submission_chunk = SND_SUBMISSION;
    dma.buffer           = s_dma_buf;

    /* Acquire and start both voices. */
    if (!wiiu_acquire_voices()) {
        free(s_dma_buf); s_dma_buf = NULL;
        free(s_buf_left); s_buf_left = NULL;
        free(s_buf_right); s_buf_right = NULL;
        return qfalse;
    }

    s_snd_active = qtrue;
    return qtrue;
}

/* Real hardware read position via AXGetVoiceOffsets, not elapsed-time guessing -- drift here
 * means audible clicks, learned that one live. */
int SNDDMA_GetDMAPos(void)
{
    if (!s_snd_active || !s_voice_left)
        return 0;

    AXVoiceOffsets offs;
    AXGetVoiceOffsets(s_voice_left, &offs);

    return (int)(offs.currentOffset * SND_CHANNELS);
}

void SNDDMA_BeginPainting(void)
{
    /* No-op: dma.buffer is always valid. */
}

/* Deinterleave mixed stereo into per-channel mono buffers, flush dcache for AX DMA. */
void SNDDMA_Submit(void)
{
    if (!s_snd_active)
        return;

    const int16_t *src = (const int16_t *)s_dma_buf;
    int16_t *left  = s_buf_left;
    int16_t *right = s_buf_right;

    for (int i = 0; i < SND_SAMPLES; i++) {
        left[i]  = src[i * 2];
        right[i] = src[i * 2 + 1];
    }

    DCFlushRange(s_buf_left, SND_RING_BYTES);
    DCFlushRange(s_buf_right, SND_RING_BYTES);
}

/* AX stays up until process exit (WiiU_Snd_Shutdown): runtime AXQuit+AXInit on vid_restart or
 * mod switch is the suspect for the console hang on the next HOME-menu return. */
void SNDDMA_Shutdown(void)
{
    wiiu_snd_stop();
}

void SNDDMA_Activate(void)
{
    /* Called when app gains/loses focus -- ProcUI handles this separately. */
}
