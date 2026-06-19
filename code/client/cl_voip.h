/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of WorldOfPadman source code.

WorldOfPadman source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

WorldOfPadman source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with WorldOfPadman source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// cl_voip.h -- VoIP jitter buffer and quality helpers

#ifndef CL_VOIP_H
#define CL_VOIP_H

#include "../qcommon/q_shared.h"

#ifdef USE_VOIP

// 20ms at 48k
#define VOIP_MAX_FRAME_SAMPLES (20 * 48)

// 3 frames is 60ms of audio, the max opus will encode at once
#define VOIP_MAX_PACKET_FRAMES 3
#define VOIP_MAX_PACKET_SAMPLES (VOIP_MAX_FRAME_SAMPLES * VOIP_MAX_PACKET_FRAMES)

// Maximum number of packets the jitter buffer can hold per sender
#define VOIP_JITTER_QUEUE 32

// Default jitter buffer delay in milliseconds
#define VOIP_JITTER_DEFAULT_MS 60

// Maximum decode buffer size (samples) - replaces the old hardcoded value
#define VOIP_DECODE_BUFFER_SAMPLES (VOIP_MAX_PACKET_SAMPLES * 4)

// Network stats sliding window size (number of packets to track)
#define VOIP_STATS_WINDOW 64

// How often to update the encoder with new network stats (ms)
#define VOIP_ADAPT_INTERVAL_MS 1000

// Minimum and maximum adaptive bitrate bounds
#define VOIP_MIN_BITRATE 8000
#define VOIP_MAX_BITRATE 96000

// Minimum and maximum adaptive jitter buffer delay (ms)
#define VOIP_MIN_JITTER_MS 20
#define VOIP_MAX_JITTER_MS 200

typedef struct voipJitterPacket_s {
	int sequence;
	int generation;
	int frames;
	int flags;
	int dataLen;
	byte data[4000];
	qboolean used;
} voipJitterPacket_t;

// Per-sender network statistics
typedef struct voipNetStats_s {
	// Packet loss tracking
	int packetsReceived;       // total packets received in current window
	int packetsExpected;       // total packets expected in current window
	int packetsLost;           // total packets lost (gaps in sequence)
	float lossRate;            // smoothed loss rate (0.0 - 1.0)

	// Jitter tracking (inter-arrival variance)
	int lastArrivalTime;       // cls.realtime when last packet arrived
	int arrivalDeltas[VOIP_STATS_WINDOW]; // recent inter-arrival times
	int deltaIndex;            // circular index into arrivalDeltas
	int deltaCount;            // number of valid entries
	float jitterMs;            // smoothed jitter estimate in ms

	// Sequence tracking for loss calculation
	int lastReceivedSeq;       // last sequence number received
	int windowStartSeq;        // sequence at start of current measurement window

	// Adaptive state
	int lastAdaptTime;         // last time we adapted encoder settings
	int adaptedBitrate;        // current adapted bitrate
	int adaptedLossPercent;    // current loss percent fed to encoder
	int adaptedJitterMs;       // current adaptive jitter buffer target
} voipNetStats_t;

typedef struct voipJitterBuffer_s {
	voipJitterPacket_t packets[VOIP_JITTER_QUEUE];
	int count;            // number of packets currently buffered
	int nextPlaySeq;      // next sequence number to play
	int lastInsertSeq;    // highest sequence number inserted
	int delayMs;          // current target delay in ms
	int lastPlayTime;     // last time we played a packet (cls.realtime)
	qboolean active;      // currently buffering/playing a stream
	int generation;       // current generation being played
} voipJitterBuffer_t;

// Per-sender VoIP state (replaces the old parallel arrays)
typedef struct voipSender_s {
	byte incomingGeneration;   // current generation from this sender
	int incomingSequence;      // next expected sequence number
	float gain;                // per-sender playback gain (0.0 - 2.0+)
	qboolean ignore;           // qtrue if this sender is muted
	int lastPacketTime;        // server time of last received packet
	voipJitterBuffer_t jitter; // jitter buffer for this sender
	voipNetStats_t netStats;   // network quality stats
} voipSender_t;

// Initialize a jitter buffer
void CL_VoipJitterInit(voipJitterBuffer_t *jb);

// Reset a jitter buffer (e.g., on generation change)
void CL_VoipJitterReset(voipJitterBuffer_t *jb);

// Insert a packet into the jitter buffer. Returns qtrue if accepted.
qboolean CL_VoipJitterInsert(voipJitterBuffer_t *jb, int sequence, int generation,
                             int frames, int flags, const byte *data, int dataLen);

// Retrieve the next packet to play. Returns qtrue if a packet is available.
// Caller must provide storage for the output fields.
qboolean CL_VoipJitterGet(voipJitterBuffer_t *jb, int *sequence, int *frames,
                          int *flags, byte *data, int *dataLen, int currentTime);

// Check if the jitter buffer has enough data to start playback
qboolean CL_VoipJitterReady(voipJitterBuffer_t *jb, int currentTime);

// Network stats functions
void CL_VoipNetStatsInit(voipNetStats_t *stats);
void CL_VoipNetStatsPacketReceived(voipNetStats_t *stats, int sequence, int frames, int currentTime);
void CL_VoipNetStatsReset(voipNetStats_t *stats);

// Adaptive encoder/jitter update (called periodically)
void CL_VoipAdaptToNetwork(void);

// Print VoIP network stats to console
void CL_VoipPrintStats(void);

// Toggle microphone loopback test mode
void CL_VoipToggleTest(void);

// Initialize the VoIP codec (encoder + decoders). Safe to call multiple times.
void CL_VoipCodecInit(void);

// Adaptive VAD check — returns qtrue if the current power level indicates speech.
qboolean CL_VoipVADCheck(float power);

// Clamp a 16-bit audio sample after gain multiplication (prevents clipping)
static ID_INLINE int16_t CL_VoipClampSample(float sample) {
	if (sample > 32767.0f)
		return 32767;
	if (sample < -32768.0f)
		return -32768;
	return (int16_t)sample;
}

extern cvar_t *cl_voipJitterDelay;
extern cvar_t *cl_voipAdaptive;

#endif // USE_VOIP
#endif // CL_VOIP_H
