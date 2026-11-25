/* ioQuake3-U Cafe OS layer: paths, timing, filesystem (modeled on the PS4 port's sys_ps4.c).
 * FS is plain POSIX over wut's newlib + SD devoptab. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <locale.h>

#include <coreinit/time.h>
#include <coreinit/thread.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "../sys/sys_local.h"

#define MAX_FOUND_FILES 0x1000

/* Base path (SD mount + game dir), set once at boot from sys_main_wiiu.c. */
static char wiiu_basePath[MAX_OSPATH] = "fs:/vol/external01/wop";

void WiiU_SetBasePath(const char *path)
{
	if (path && *path)
		Q_strncpyz(wiiu_basePath, path, sizeof(wiiu_basePath));
}

/* Must subtract before casting to int ms, or it overflows and Com_Frame spins forever. */
static OSTime sys_timeBase = 0;

void Sys_PlatformInit(void)
{
	setlocale(LC_ALL, "C"); /* locale-agnostic float parsing (j_pitch etc.) */
	sys_timeBase = OSGetSystemTime();
}

void Sys_PlatformExit(void)
{
}

int Sys_Milliseconds(void)
{
	OSTime now = OSGetSystemTime();
	if (!sys_timeBase)
		sys_timeBase = now;
	return (int)OSTicksToMilliseconds(now - sys_timeBase);
}

void Sys_Sleep(int msec)
{
	if (msec <= 0) return;
	OSSleepTicks(OSMillisecondsToTicks(msec));
}

const char *Sys_Cwd(void)
{
	static char cwd[MAX_OSPATH];
	Q_strncpyz(cwd, wiiu_basePath, sizeof(cwd));
	return cwd;
}

const char *Sys_Pwd(void)
{
	return Sys_Cwd();
}

const char *Sys_DefaultInstallPath(void)
{
	return wiiu_basePath;
}

void Sys_SetDefaultInstallPath(const char *path)
{
	WiiU_SetBasePath(path);
}

const char *Sys_DefaultAppPath(void)
{
	return wiiu_basePath;
}

char *Sys_DefaultHomePath(void)
{
	return wiiu_basePath;
}

char *Sys_DefaultHomeConfigPath(void) { return wiiu_basePath; }
char *Sys_DefaultHomeDataPath(void)   { return wiiu_basePath; }
char *Sys_DefaultHomeStatePath(void)  { return wiiu_basePath; }

char *Sys_SteamPath(void)          { static char p[] = ""; return p; }
char *Sys_GogPath(void)            { static char p[] = ""; return p; }
char *Sys_MicrosoftStorePath(void) { static char p[] = ""; return p; }
char *Sys_TempPath(void)           { return wiiu_basePath; }

const char *Sys_BinaryPath(void)
{
	return wiiu_basePath;
}

void Sys_SetBinaryPath(const char *path)
{
	(void)path;
}

qboolean Sys_Mkdir(const char *path)
{
	/* Virtual mount-point prefixes can't be mkdir'd/stat'd; fake success or FS_CreatePath aborts. */
	static const char *mount_prefixes[] = {
		"fs:", "fs:/vol", "fs:/vol/external01", "fs:/vol/external01/wop", "fs:/vol/external01/wop/wop", NULL
	};
	int i;
	for (i = 0; mount_prefixes[i]; i++) {
		if (!strcmp(path, mount_prefixes[i]))
			return qtrue;
	}

	if (!path || !path[0] || strncmp(path, "fs:/vol/", 8) != 0)
		return qfalse;

	struct stat st;
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return qtrue;

	if (mkdir(path, 0755) == 0)
		return qtrue;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return qtrue;
	return qfalse;
}

FILE *Sys_FOpen(const char *ospath, const char *mode)
{
	FILE *fp = fopen(ospath, mode);
	if (!fp || !ospath || !mode || mode[0] != 'r' || strchr(mode, '+'))
		return fp;

	const char *ext = strrchr(ospath, '.');
	if (ext && (!Q_stricmp(ext, ".pk3") || !Q_stricmp(ext, ".ogg"))) {
		if (fseek(fp, 0, SEEK_END) == 0) {
			long sz = ftell(fp);
			if (sz > 0 && sz <= 16 * 1024 * 1024) {
				FILE *mf = fmemopen(NULL, (size_t)sz, "rb+");
				if (mf) {
					void *tmp = malloc((size_t)sz);
					if (tmp) {
						rewind(fp);
						if (fread(tmp, 1, (size_t)sz, fp) == (size_t)sz && fwrite(tmp, 1, (size_t)sz, mf) == (size_t)sz) {
							free(tmp);
							fclose(fp);
							rewind(mf);
							return mf;
						}
						free(tmp);
					}
					fclose(mf);
				}
			}
			rewind(fp);
		}
	}
	return fp;
}

FILE *Sys_Mkfifo(const char *ospath)
{
	(void)ospath;
	return NULL;
}

int Sys_FileTime(char *path)
{
	struct stat buf;
	if (stat(path, &buf) == -1)
		return -1;
	return buf.st_mtime;
}

/* Directory listing (same structure as sys_ps4.c / sys_unix.c). */

static void Sys_ListFilteredFiles(const char *basedir, char *subdirs,
	const char *filter, char **list, int *numfiles)
{
	char search[MAX_OSPATH], newsubdirs[MAX_OSPATH], filename[MAX_OSPATH];
	DIR *fdir;
	struct dirent *d;
	struct stat st;

	if (*numfiles >= MAX_FOUND_FILES - 1) return;
	if (basedir[0] == '\0') return;

	if (strlen(subdirs))
		Com_sprintf(search, sizeof(search), "%s/%s", basedir, subdirs);
	else
		Com_sprintf(search, sizeof(search), "%s", basedir);

	if ((fdir = opendir(search)) == NULL) return;

	while ((d = readdir(fdir)) != NULL) {
		Com_sprintf(filename, sizeof(filename), "%s/%s", search, d->d_name);
		if (stat(filename, &st) == -1) continue;

		if (S_ISDIR(st.st_mode)) {
			if (Q_stricmp(d->d_name, ".") && Q_stricmp(d->d_name, "..")) {
				if (strlen(subdirs))
					Com_sprintf(newsubdirs, sizeof(newsubdirs), "%s/%s", subdirs, d->d_name);
				else
					Com_sprintf(newsubdirs, sizeof(newsubdirs), "%s", d->d_name);
				Sys_ListFilteredFiles(basedir, newsubdirs, filter, list, numfiles);
			}
		}
		if (*numfiles >= MAX_FOUND_FILES - 1) break;

		Com_sprintf(filename, sizeof(filename), "%s/%s", subdirs, d->d_name);
		if (!Com_FilterPath(filter, filename, qfalse)) continue;

		list[*numfiles] = CopyString(filename);
		(*numfiles)++;
	}
	closedir(fdir);
}

char **Sys_ListFiles(const char *directory, const char *extension,
	const char *filter, int *numfiles, qboolean wantsubs)
{
	struct dirent *d;
	DIR *fdir;
	qboolean dironly = wantsubs;
	char search[MAX_OSPATH];
	int nfiles = 0;
	char **listCopy;
	char *list[MAX_FOUND_FILES];
	int extLen, i;

	if (filter) {
		Sys_ListFilteredFiles(directory, "", filter, list, &nfiles);
		*numfiles = nfiles;
		if (!nfiles) return NULL;
		listCopy = Z_Malloc((nfiles + 1) * sizeof(*listCopy));
		for (i = 0; i < nfiles; i++) listCopy[i] = list[i];
		listCopy[i] = NULL;
		return listCopy;
	}

	if (!extension) extension = "";
	extLen = strlen(extension);

	fdir = opendir(directory);
	if (!fdir) { *numfiles = 0; return NULL; }

	while ((d = readdir(fdir)) != NULL) {
		struct stat st;
		if (nfiles >= MAX_FOUND_FILES - 1) break;
		Com_sprintf(search, sizeof(search), "%s/%s", directory, d->d_name);
		if (stat(search, &st) == -1) continue;

		if (dironly) { if (!S_ISDIR(st.st_mode)) continue; }
		else         { if (S_ISDIR(st.st_mode)) continue; }

		if (*extension) {
			int nameLen = strlen(d->d_name);
			if (nameLen < extLen ||
			    Q_stricmp(d->d_name + nameLen - extLen, extension))
				continue;
		}
		list[nfiles++] = CopyString(d->d_name);
	}
	closedir(fdir);

	*numfiles = nfiles;
	if (!nfiles) return NULL;
	listCopy = Z_Malloc((nfiles + 1) * sizeof(*listCopy));
	for (i = 0; i < nfiles; i++) listCopy[i] = list[i];
	listCopy[i] = NULL;
	return listCopy;
}

void Sys_FreeFileList(char **list)
{
	int i;
	if (!list) return;
	for (i = 0; list[i]; i++) Z_Free(list[i]);
	Z_Free(list);
}

/* Misc. */

qboolean Sys_RandomBytes(byte *string, int len)
{
	uint64_t seed = (uint64_t)OSGetSystemTime();
	int i;
	for (i = 0; i < len; i++) {
		seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
		string[i] = (byte)(seed >> 33);
	}
	return qtrue;
}

char *Sys_GetCurrentUser(void) { return "WiiU Player"; }

qboolean Sys_LowPhysicalMemory(void) { return qfalse; }

/* No dlopen: qagame/cgame are pre-linked into the .rpx (Makefile.client, entry points renamed).
 * EF stays bytecode-only, its struct layout differs. */
#ifndef ELITEFORCE
extern void qagame_dllEntry( intptr_t (QDECL *syscallptr)( intptr_t arg, ... ) );
extern intptr_t qagame_vmMain( int command, int arg0, int arg1, int arg2, int arg3,
	int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 );
extern char __qagame_data_start[], __qagame_data_end[];
extern char __qagame_bss_start[], __qagame_bss_end[];
#endif
#ifdef WIIU_NATIVE_CGAME
extern void cgame_dllEntry( intptr_t (QDECL *syscallptr)( intptr_t arg, ... ) );
extern intptr_t cgame_vmMain( int command, int arg0, int arg1, int arg2, int arg3,
	int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 );
extern char __cgame_data_start[], __cgame_data_end[];
extern char __cgame_bss_start[], __cgame_bss_end[];
#endif

typedef struct {
	const char *name;
	void (*dllEntry)( intptr_t (QDECL *syscallptr)( intptr_t arg, ... ) );
	vmMainProc vmMain;
	char *dataStart, *dataEnd, *bssStart, *bssEnd;
	char *pristine;
} wiiuNativeModule_t;

static wiiuNativeModule_t wiiu_native_modules[] = {
#ifndef ELITEFORCE
	{ "qagame", qagame_dllEntry, qagame_vmMain, __qagame_data_start, __qagame_data_end,
	  __qagame_bss_start, __qagame_bss_end, NULL },
#endif
#ifdef WIIU_NATIVE_CGAME
	{ "cgame", cgame_dllEntry, cgame_vmMain, __cgame_data_start, __cgame_data_end,
	  __cgame_bss_start, __cgame_bss_end, NULL },
#endif
	{ NULL }
};

void *Sys_LoadDll(const char *name, qboolean useSystemLib)
{
	(void)useSystemLib;
	Com_Printf("Sys_LoadDll(%s): not supported on Wii U (bytecode VMs only)\n", name);
	return NULL;
}

void Sys_UnloadDll(void *dllHandle) { (void)dllHandle; }

void *Sys_LoadGameDll(const char *name, vmMainProc *entryPoint,
	intptr_t (QDECL *systemcalls)(intptr_t, ...))
{
	wiiuNativeModule_t *m;

	for (m = wiiu_native_modules; m->name; m++) {
		size_t dataLen;

		if (Q_stricmp(name, m->name))
			continue;

		/* Restore .data and zero .bss on every load, like a fresh dlopen on PC:
		 * static state must not leak across map changes. */
		dataLen = (size_t)(m->dataEnd - m->dataStart);
		if (!m->pristine) {
			m->pristine = malloc(dataLen ? dataLen : 1);
			if (!m->pristine)
				Com_Error(ERR_FATAL, "Sys_LoadGameDll: out of memory for %s data copy", name);
			memcpy(m->pristine, m->dataStart, dataLen);
		} else {
			memcpy(m->dataStart, m->pristine, dataLen);
		}
		memset(m->bssStart, 0, (size_t)(m->bssEnd - m->bssStart));

		m->dllEntry(systemcalls);
		*entryPoint = m->vmMain;
		return (void *)1; /* sentinel: non-NULL, never dereferenced/unloaded */
	}

	(void)entryPoint; (void)systemcalls;
	Com_Printf("Sys_LoadGameDll(%s): not supported on Wii U (bytecode VM only)\n", name);
	return NULL;
}

qboolean Sys_DllExtension(const char *name)
{
	const char *p = strrchr(name, '.');
	return (p && !Q_stricmp(p, DLL_EXT)) ? qtrue : qfalse;
}

void Sys_SetEnv(const char *name, const char *value)
{
	if (value && *value) setenv(name, value, 1);
	else                  unsetenv(name);
}

char *Sys_GetClipboardData(void) { return NULL; }

dialogResult_t Sys_Dialog(dialogType_t type, const char *message, const char *title)
{
	(void)type;
	Com_Printf("[%s] %s\n", title ? title : "Dialog", message);
	return DR_OK;
}

cpuFeatures_t Sys_GetProcessorFeatures(void) { return (cpuFeatures_t)0; }

void Sys_ParseArgs(int argc, char **argv) { (void)argc; (void)argv; }

int Sys_PID(void) { return 1; }
qboolean Sys_PIDIsRunning(int pid) { (void)pid; return qfalse; }
void Sys_InitPIDFile(const char *gamedir) { (void)gamedir; }
void Sys_RemovePIDFile(const char *gamedir) { (void)gamedir; }

qboolean Sys_OpenFolderInPlatformFileManager(const char *path) { (void)path; return qfalse; }
qboolean Sys_OpenFolderInFileManager(const char *path, qboolean create) { (void)path; (void)create; return qfalse; }
qboolean Sys_SetMaxFileLimit(void) { return qtrue; }

void Sys_AnsiColorPrint(const char *msg) { fputs(msg, stdout); }

char *Sys_ConsoleInput(void) { return NULL; }

void Sys_In_Restart_f(void) { }

/* GL init lives in the Phase-3 glimp; no-ops for the dedicated build. */
void Sys_GLimpSafeInit(void) { }
void Sys_GLimpInit(void) { }
