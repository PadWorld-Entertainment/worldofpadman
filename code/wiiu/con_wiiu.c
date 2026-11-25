/*
 * ioQuake3-U — Wii U native (wut / GX2) port
 * code/wiiu/con_wiiu.c -- console output.
 *
 * fflush() is a no-op here, only fclose() commits to SD -- of course. DEDICATED
 * gets OSScreen, CLIENT gets UDP instead since GX2 already owns the scan buffers.
 */

#include <stdio.h>
#include <string.h>

#include <whb/log.h>
#include <whb/log_udp.h>
#include <coreinit/time.h>

#ifdef DEDICATED
#include <whb/log_console.h>
#endif

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"

#if defined(STANDALONETA)
#define WIIU_LOG_PATH "fs:/vol/external01/quake3/log_ta.txt"
#elif defined(STANDALONEOA)
#define WIIU_LOG_PATH "fs:/vol/external01/quake3/log_oa.txt"
#elif defined(CLASSIC)
#define WIIU_LOG_PATH "fs:/vol/external01/quake3/log_classic.txt"
#else
#define WIIU_LOG_PATH "fs:/vol/external01/quake3/log.txt"
#endif

#ifdef DEDICATED
static int    s_consoleReady = 0;
static OSTime s_lastDraw = 0;
#endif

/* Flipped by sys_main_wiiu.c on ACQUIRE/RELEASE, checked every frame -- yes, really. */
static volatile int s_inForeground = 1;

void CON_SetForeground(qboolean fg)
{
	s_inForeground = fg ? 1 : 0;
}

/* Foreground query used by tr_gx2_init.c before touching WHBGfx/GX2. */
qboolean CON_IsForeground(void)
{
	return s_inForeground ? qtrue : qfalse;
}

void CON_Init(void)
{
#ifdef WIIU_DEBUG_LOG
	/* Truncate log for a clean run. Release builds skip this -- no log.txt at all. */
	FILE *f = fopen(WIIU_LOG_PATH, "w");
	if (f) {
		fputs("=== ioQuake3-U boot log ===\n", f);
		fclose(f);
	}
#endif

#ifndef DEDICATED
	/* Client: no OSScreen (GX2 owns it); UDP log for remote boot watching. */
	WHBLogUdpInit();
#else
	if (WHBLogConsoleInit()) {
		s_consoleReady = 1;
		WHBLogConsoleSetColor(0xFF103000); /* dark-blue background, ARGB */
	}
#endif
}

void CON_Shutdown(void)
{
#ifdef DEDICATED
	if (s_consoleReady) {
		WHBLogConsoleFree();
		s_consoleReady = 0;
	}
#endif
}

void CON_Flush(void)
{
#ifdef DEDICATED
	if (s_consoleReady && s_inForeground) {
		WHBLogConsoleDraw();
		s_lastDraw = OSGetSystemTime();
	}
#endif
}

void CON_Print(const char *msg)
{
	if (!msg || !*msg)
		return;

#ifdef WIIU_DEBUG_LOG
	/* Per-line fopen/fprintf/fclose -- only durable SD path on Wii U. */
	{
		FILE *f = fopen(WIIU_LOG_PATH, "a");
		if (f) {
			fputs(msg, f);
			fclose(f);
		}
	}
#endif

	WHBLogPrint(msg);

#ifdef DEDICATED
	if (s_consoleReady && s_inForeground) {
		OSTime now = OSGetSystemTime();
		if (now - s_lastDraw > (OSTime)OSMillisecondsToTicks(50)) {
			WHBLogConsoleDraw();
			s_lastDraw = now;
		}
	}
#endif
}

char *CON_Input(void) { return NULL; }

unsigned int CON_LogSize(void) { return 0; }
unsigned int CON_LogWrite(const char *in) { (void)in; return 0; }
unsigned int CON_LogRead(char *out, unsigned int outSize) { (void)out; (void)outSize; return 0; }
