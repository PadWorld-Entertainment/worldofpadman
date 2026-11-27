/* ioQuake3-U Cafe OS entry point. Follow the ProcUI lifecycle to the letter:
 * any deviation gets the app silently killed by the OS (no crash, no log). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>

#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <coreinit/foreground.h>
#include <coreinit/title.h>
#include <coreinit/systeminfo.h>
#include <coreinit/exception.h>
#include <coreinit/context.h>
#include <proc_ui/procui.h>
#include <sysapp/launch.h>
#include <whb/log.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "../sys/sys_local.h"

#ifndef DEDICATED
#include "../input/wiiu_input.h"
#include "../audio/wiiu_snd.h"
#include "wiiu_account.h"
/* Exit-only teardown, never vid_restart -- must run before ProcUIShutdown() or it hangs. */
void GLimp_ShutdownFinal(void);
#endif

void  CON_Init(void);
void  CON_Print(const char *msg);
void  CON_Flush(void);
void  CON_Shutdown(void);
void  CON_SetForeground(qboolean fg);
qboolean CON_IsForeground(void);
void  WiiU_SetBasePath(const char *path);
#define WIIU_BASE_PATH "fs:/vol/external01/wop"
#define WIIU_VM_GAME   "0"
#define WIIU_LOG_SUFFIX "_wop"

/* Diagnostic toggle for the console-freeze-on-quit bisect; networking was exonerated, kept at 0. */
#define WIIU_DIAG_SKIP_NET 0

static volatile qboolean wiiu_running = qtrue;
static qboolean          wiiu_from_hbl = qfalse;
static int               wiiu_frame_count = 0;

/* HBL / Mii Maker title IDs. */
#define HBL_TITLE_ID           (0x0005000013374842ULL)
#define MII_MAKER_JPN_TITLE_ID (0x000500101004A000ULL)
#define MII_MAKER_USA_TITLE_ID (0x000500101004A100ULL)
#define MII_MAKER_EUR_TITLE_ID (0x000500101004A200ULL)

static void LOGCP(const char *s) { CON_Print(s); CON_Flush(); }

/* Crash handler: dump registers to crash.txt. */
static BOOL wiiu_exception_handler(OSContext *ctx)
{
	FILE *f = fopen(WIIU_BASE_PATH "/crash" WIIU_LOG_SUFFIX ".txt", "w");
	if (f) {
		fprintf(f, "CRASH at frame %d\n", wiiu_frame_count);
		fprintf(f, "SRR0(PC)=0x%08X  LR=0x%08X\n", (unsigned)ctx->srr0, (unsigned)ctx->lr);
		fprintf(f, "DAR=0x%08X  DSISR=0x%08X\n", (unsigned)ctx->dar, (unsigned)ctx->dsisr);
		for (int i = 0; i < 32; i += 4)
			fprintf(f, "r%-2d=0x%08X r%-2d=0x%08X r%-2d=0x%08X r%-2d=0x%08X\n",
			        i, (unsigned)ctx->gpr[i], i+1, (unsigned)ctx->gpr[i+1],
			        i+2, (unsigned)ctx->gpr[i+2], i+3, (unsigned)ctx->gpr[i+3]);
		fclose(f);
	}
	return FALSE; /* let the OS terminate */
}

static uint32_t wiiu_save_callback(void *ctx)
{
	(void)ctx;
	OSSavesDone_ReadyToRelease();
	return 0;
}

static uint32_t wiiu_home_denied(void *ctx)
{
	(void)ctx;
	wiiu_running = qfalse;
	return 0;
}

#ifndef DEDICATED
/* Priority 50 so audio brackets the graphics transition; fine to register before WiiU_Snd_Init(). */
static uint32_t wiiu_audio_release(void *ctx)
{
	(void)ctx;
	WiiU_Snd_ReleaseForeground();
	return 0;
}

static uint32_t wiiu_audio_acquire(void *ctx)
{
	(void)ctx;
	WiiU_Snd_AcquireForeground();
	return 0;
}

/* Stop in-flight rumble on RELEASE so a backgrounded app never leaves the GamePad buzzing. */
static uint32_t wiiu_input_release(void *ctx)
{
	(void)ctx;
	WiiU_Input_ReleaseForeground();
	return 0;
}

#ifdef WIIU_DEBUG_LOG
void GX2BE_DiagSwap(const char *tag);
void GX2BE_DiagAcquired(void);

/* Registered at 200 and 1 to bracket WHB's own RELEASE (priority 100). */
static uint32_t wiiu_diag_release(void *ctx)
{
	GX2BE_DiagSwap((const char *)ctx);
	return 0;
}
#endif
#endif

/* Ask for foreground handoff, then pump ProcUI until EXITING before shutdown -- skip this
 * and it's a console freeze and a power cycle, not a crash screen. Cost us that lesson once. */
static void WiiU_ExitHandshake(qboolean alreadyExiting)
{
	if (alreadyExiting) {
		LOGCP("[wop-u] exit handshake: already EXITING, no pump needed\n");
		return;
	}
	if (wiiu_from_hbl) {
		LOGCP("[wop-u] exit handshake: SYSRelaunchTitle\n");
		SYSRelaunchTitle(0, NULL);
		LOGCP("[wop-u] exit handshake: SYSRelaunchTitle returned\n");
	} else {
		LOGCP("[wop-u] exit handshake: SYSLaunchMenu\n");
		SYSLaunchMenu();
		LOGCP("[wop-u] exit handshake: SYSLaunchMenu returned\n");
	}
	{
		int lastSt = -1;
		for (;;) {
			ProcUIStatus st = ProcUIProcessMessages(TRUE);
			if ((int)st != lastSt) {
				char buf[64];
				snprintf(buf, sizeof(buf),
				         "[wop-u] exit handshake: status=%d\n", (int)st);
				LOGCP(buf);
				lastSt = (int)st;
			}
			if (st == PROCUI_STATUS_EXITING) {
				LOGCP("[wop-u] exit handshake: EXITING received\n");
				break;
			}
			if (st == PROCUI_STATUS_RELEASE_FOREGROUND) {
				CON_SetForeground(qfalse);
				LOGCP("[wop-u] exit handshake: calling ProcUIDrawDoneRelease\n");
				ProcUIDrawDoneRelease();
				LOGCP("[wop-u] exit handshake: ProcUIDrawDoneRelease returned\n");
			} else if (st != PROCUI_STATUS_IN_FOREGROUND) {
				OSSleepTicks(OSMillisecondsToTicks(16));
			}
		}
	}
}

/* Sys_* required by the engine. */

void Sys_Init(void)
{
	Cvar_Set("arch", OS_STRING "-" ARCH_STRING);
}

/* No Com_Shutdown() here -- Com_Quit_f already did it, calling it twice recurses via Com_Error. */
void Sys_Quit(void)
{
	wiiu_running = qfalse;
#if !WIIU_DIAG_SKIP_NET
	LOGCP("[wop-u] Sys_Quit: calling NET_Shutdown\n");
	NET_Shutdown();
	LOGCP("[wop-u] Sys_Quit: NET_Shutdown returned\n");
#endif
#ifndef DEDICATED
	/* Idempotent safety net -- anything left running here stalls exit()'s OS finalizers. */
	WiiU_Snd_Shutdown();
	WiiU_Input_Shutdown();
#endif
	/* Order matters: handshake, then ProcUIShutdown, then GX2. Swap the last two and it hangs. */
	WiiU_ExitHandshake(qfalse);
	LOGCP("[wop-u] Sys_Quit: calling ProcUIShutdown\n");
	ProcUIShutdown();
	LOGCP("[wop-u] Sys_Quit: ProcUIShutdown returned\n");
#ifndef DEDICATED
	GLimp_ShutdownFinal();
#endif
	CON_Shutdown();
	LOGCP("[wop-u] Sys_Quit: calling exit(0)\n");
	exit(0);
}

void Sys_Print(const char *msg) { CON_Print(msg); }

void Sys_Error(const char *error, ...)
{
	va_list argptr;
	char string[1024];
	FILE *f;

	va_start(argptr, error);
	Q_vsnprintf(string, sizeof(string), error, argptr);
	va_end(argptr);

	CON_Print("Sys_Error: ");
	CON_Print(string);
	CON_Print("\n");
	CON_Flush();

	f = fopen(WIIU_BASE_PATH "/error" WIIU_LOG_SUFFIX ".txt", "w");
	if (f) { fprintf(f, "%s\n", string); fclose(f); }

	/* Safe to call directly -- Sys_Quit() doesn't touch Com_Shutdown(). */
	Sys_Quit();
}

void Sys_ErrorDialog(const char *error)
{
	CON_Print("ERROR: ");
	CON_Print(error);
	CON_Print("\n");
	CON_Flush();
}

void Sys_SigHandler(int signal)
{
	(void)signal;
	wiiu_running = qfalse;
	Sys_Quit();
}

/* Entry point. */

int main(int argc, char **argv)
{
	char commandLine[2048] = { 0 };
	qboolean inited = qfalse;
	qboolean os_exiting = qfalse;
	int i;

	/* Core 1 gets 2MB L2 vs 512KB on 0/2 -- asymmetric cores, so the main thread pins here. */
	OSSetThreadAffinity(OSGetCurrentThread(), OS_THREAD_ATTRIB_AFFINITY_CPU1);

	/* HBL detection must precede ProcUIInitEx, or a foreground-release kills us before the loop. */
	{
		uint64_t tid = OSGetTitleID();
		if (tid == HBL_TITLE_ID || tid == MII_MAKER_JPN_TITLE_ID ||
		    tid == MII_MAKER_USA_TITLE_ID || tid == MII_MAKER_EUR_TITLE_ID) {
			OSEnableHomeButtonMenu(FALSE);
			wiiu_from_hbl = qtrue;
		}
	}

	OSSetExceptionCallbackEx(OS_EXCEPTION_MODE_GLOBAL_ALL_CORES,
	                         OS_EXCEPTION_TYPE_DSI, wiiu_exception_handler);
	OSSetExceptionCallbackEx(OS_EXCEPTION_MODE_GLOBAL_ALL_CORES,
	                         OS_EXCEPTION_TYPE_ISI, wiiu_exception_handler);
	OSSetExceptionCallbackEx(OS_EXCEPTION_MODE_GLOBAL_ALL_CORES,
	                         OS_EXCEPTION_TYPE_PROGRAM, wiiu_exception_handler);

	/* Only ProcUIInitEx + callback registration are safe before the loop. */
	ProcUIInitEx(wiiu_save_callback, NULL);
	if (wiiu_from_hbl)
		ProcUIRegisterCallback(PROCUI_CALLBACK_HOME_BUTTON_DENIED,
		                       wiiu_home_denied, NULL, 100);
#ifndef DEDICATED
	ProcUIRegisterCallback(PROCUI_CALLBACK_RELEASE, wiiu_audio_release, NULL, 50);
	ProcUIRegisterCallback(PROCUI_CALLBACK_ACQUIRE, wiiu_audio_acquire, NULL, 50);
	ProcUIRegisterCallback(PROCUI_CALLBACK_RELEASE, wiiu_input_release, NULL, 50);
#ifdef WIIU_DEBUG_LOG
	ProcUIRegisterCallback(PROCUI_CALLBACK_RELEASE, wiiu_diag_release, "release-cb-prio200", 200);
	ProcUIRegisterCallback(PROCUI_CALLBACK_RELEASE, wiiu_diag_release, "release-cb-prio1", 1);
#endif
#endif

	/* CON_Init/FS setup/Com_Init happen inside this loop, on the first IN_FOREGROUND tick. */
	while (wiiu_running) {
		ProcUIStatus st = ProcUIProcessMessages(TRUE);

		if (st == PROCUI_STATUS_EXITING) {
			os_exiting = qtrue;
			break;
		}
		if (st == PROCUI_STATUS_RELEASE_FOREGROUND) {
#if defined(WIIU_DEBUG_LOG) && !defined(DEDICATED)
			if (inited)
				GX2BE_DiagSwap("RELEASE_FOREGROUND status");
#endif
			CON_SetForeground(qfalse);
			ProcUIDrawDoneRelease();
			continue;
		}
		if (st != PROCUI_STATUS_IN_FOREGROUND) {
			OSSleepTicks(OSMillisecondsToTicks(16));
			continue;
		}
#if defined(WIIU_DEBUG_LOG) && !defined(DEDICATED)
		if (inited && !CON_IsForeground()) {
			CON_SetForeground(qtrue);
			GX2BE_DiagAcquired();
		}
#endif
		CON_SetForeground(qtrue);

		if (!inited) {
			inited = qtrue;

			/* First safe point to touch SD/OSScreen/do real work. */
			CON_Init();
			LOGCP("[wop-u] main(): ProcUI up, entering init\n");

			WiiU_SetBasePath(WIIU_BASE_PATH);
			chdir(WIIU_BASE_PATH);
			LOGCP("[wop-u] CP1: base = " WIIU_BASE_PATH "\n");

#ifndef DEDICATED
#ifndef DEDICATED
			/* Verify WoP paks exist before Com_Init bothers trying. */
			{
				DIR *dir;
				struct dirent *entry;
				char path[256];
				int found_pk3 = 0;
				snprintf(path, sizeof(path), "%s/wop", WIIU_BASE_PATH);
				dir = opendir(path);
				if (dir) {
					while ((entry = readdir(dir)) != NULL) {
						size_t len = strlen(entry->d_name);
						if (len >= 4 && strcmp(entry->d_name + len - 4, ".pk3") == 0) {
							found_pk3 = 1;
							break;
						}
					}
					closedir(dir);
				}
				if (!found_pk3) {
					LOGCP("[wop-u] FATAL: No .pk3 files found in " WIIU_BASE_PATH "/wop\n");
					OSSleepTicks(OSMillisecondsToTicks(3000));
					break;
				}
			}
#endif

			/* Create qkey if missing. */
			{
				FILE *kf = fopen(WIIU_BASE_PATH "/qkey", "rb");
				if (kf) {
					fclose(kf);
				} else {
					kf = fopen(WIIU_BASE_PATH "/qkey", "wb");
					if (kf) {
						unsigned char buf[2048];
						int b;
						for (b = 0; b < 2048; b++) buf[b] = (unsigned char)(b & 0xFF);
						fwrite(buf, 1, 2048, kf);
						fclose(kf);
					}
				}
			}

			/* Input before Com_Init so IN_Init (inside CL_InitRef) has a live backend. */
			WiiU_Input_Init();
			LOGCP("[wop-u] CP2: input ready\n");

			/* Audio before Com_Init or SNDDMA_Init silently disables sound. Silently. */
			WiiU_Snd_Init();
			LOGCP("[wop-u] CP3: audio ready\n");
#endif

			for (i = 1; i < argc; i++) {
				if (commandLine[0]) Q_strcat(commandLine, sizeof(commandLine), " ");
				Q_strcat(commandLine, sizeof(commandLine), argv[i]);
			}

			if (commandLine[0]) Q_strcat(commandLine, sizeof(commandLine), " ");

#ifdef DEDICATED
			Q_strcat(commandLine, sizeof(commandLine),
			         "+set fs_basepath " WIIU_BASE_PATH
			         " +set fs_homepath " WIIU_BASE_PATH
			         " +set com_basegame " BASEGAME
			         " +set com_hunkMegs 128 +set com_zoneMegs 64"
			         " +set sv_pure 0 +set com_standalone 0"
			         " +set com_logfile 0 +set vm_game 0");
#else
			Q_strcat(commandLine, sizeof(commandLine),
			         "+set fs_basepath " WIIU_BASE_PATH
			         " +set fs_homepath " WIIU_BASE_PATH
			         " +set com_basegame " BASEGAME
			         " +set com_hunkMegs 256 +set com_zoneMegs 64"
			         " +set com_soundMegs 16 +set s_khz 48"
			         " +set r_mode -1 +set r_customwidth 1280 +set r_customheight 720"
			         " +set com_maxfps 60"
			         " +set sv_pure 0 +set com_standalone 0 +set com_logfile 0"
			         " +set vm_game " WIIU_VM_GAME
#ifdef WIIU_NATIVE_CGAME
			         " +set vm_cgame 0 +set vm_ui 0"
#endif
			         " +set in_joystick 1 +set in_joystickUseAnalog 1"
			         " +set j_pitch_axis 3 +set j_yaw_axis 4"
			         " +set j_forward_axis 1 +set j_side_axis 0"
			         /* Kept tiny -- cl_input.c's __WIIU__ block also multiplies by cl_sensitivity. */
			         " +set j_pitch 0.002 +set j_yaw -0.002"
			         " +set j_forward -0.25 +set j_side 0.25"
			         " +set net_enabled 1");

			/* Cache Mii nickname here (proven-safe nn::act timing) as CL_Init's
			 * "name" cvar default, so the UI's Defaults button doesn't wipe it. */
			WiiU_InitDefaultPlayerName();
#endif

			LOGCP("[wop-u] CP4: calling Com_Init (loads paks)...\n");
			WHBLogPrintf("[TIME] before Com_Init: %d ms\n", Sys_Milliseconds());
			Sys_PlatformInit();
			Com_Init(commandLine);
			WHBLogPrintf("[TIME] after Com_Init: %d ms\n", Sys_Milliseconds());
#if !WIIU_DIAG_SKIP_NET
			NET_Init();
			WHBLogPrintf("[TIME] after NET_Init: %d ms\n", Sys_Milliseconds());
#else
			LOGCP("[wop-u] DIAG: skipping NET_Init entirely\n");
#endif

#ifdef DEDICATED
			/* Prove FS mounted + paks readable by reading a pak0-only file. */
			{
				fileHandle_t fh = 0;
				long len = FS_FOpenFileRead("default.cfg", &fh, qfalse);
				if (len > 0) {
					Com_Printf("\n==== PHASE 2 OK ====\n");
					Com_Printf("Read default.cfg from paks (%ld bytes).\n", len);
					Com_Printf("Filesystem + paks working on Wii U!\n");
					Com_Printf("====================\n\n");
					FS_FCloseFile(fh);
				} else {
					Com_Printf("\n==== PHASE 2: PAKS NOT FOUND ====\n");
					Com_Printf("Put Q3 data at " WIIU_BASE_PATH "/baseq3/pak0.pk3\n\n");
				}
				CON_Flush();
			}
#endif
			continue;
		}

#ifndef DEDICATED
		WiiU_Input_Frame();

		if (wiiu_frame_count > 60 && WiiU_Input_QuitPressed()) {
			LOGCP("[wop-u] Quit combo detected\n");
			break;
		}
#endif

		Com_Frame();
		wiiu_frame_count++;
	}

	LOGCP("[wop-u] shutting down\n");
#if !WIIU_DIAG_SKIP_NET
	/* Shut down network + somemopt donation thread BEFORE Com_Shutdown/AXQuit (matches Sys_Quit). */
	LOGCP("[wop-u] main: calling NET_Shutdown\n");
	NET_Shutdown();
	LOGCP("[wop-u] main: NET_Shutdown returned\n");
#endif
	LOGCP("[wop-u] main: calling Com_Shutdown\n");
	Com_Shutdown();
	LOGCP("[wop-u] main: Com_Shutdown returned\n");
#ifndef DEDICATED
	WiiU_Snd_Shutdown();
	WiiU_Input_Shutdown();
#endif
	/* Same order as Sys_Quit: handshake to EXITING, ProcUIShutdown, then GX2 teardown. */
	WiiU_ExitHandshake(os_exiting);
	LOGCP("[wop-u] main: calling ProcUIShutdown\n");
	ProcUIShutdown();
	LOGCP("[wop-u] main: ProcUIShutdown returned\n");
#ifndef DEDICATED
	GLimp_ShutdownFinal();
#endif
	CON_Shutdown();
	LOGCP("[wop-u] main: returning 0\n");
	return 0;
}
