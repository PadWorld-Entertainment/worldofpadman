/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>


#include <libgen.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"

//=============================================================================

/*
================
Sys_Milliseconds
================
*/
/* base time in seconds, that's our origin
   timeval:tv_sec is an int: 
   assuming this wraps every 0x7fffffff - ~68 years since the Epoch (1970) - we're safe till 2038
   using unsigned long data type to work right with Sys_XTimeToSysTime */
unsigned long sys_timeBase = 0;
/* current time in ms, using sys_timeBase as origin
   NOTE: sys_timeBase*1000 + curtime -> ms since the Epoch
     0x7fffffff ms - ~24 days
   although timeval:tv_usec is an int, I'm not sure whether it is actually used as an unsigned int
     (which would affect the wrap period) */
int Sys_Milliseconds( void )
{
	struct timeval tp;
	int curtime;

	gettimeofday( &tp, NULL );
	
	if ( !sys_timeBase )
	{
		sys_timeBase = tp.tv_sec;
		return tp.tv_usec/1000;
	}

	curtime = (tp.tv_sec - sys_timeBase) * 1000 + tp.tv_usec / 1000;
	
	return curtime;
}


char *strlwr( char *s ) {
  if ( s==NULL ) { // bk001204 - paranoia
    assert(0);
    return s;
  }
  while (*s) {
    *s = tolower(*s);
    s++;
  }
  return s; // bk001204 - duh
}


/*
==================
Sys_RandomBytes
==================
*/
qboolean Sys_RandomBytes( byte *string, int len )
{
	FILE *fp;

	fp = fopen( "/dev/urandom", "r" );
	if( !fp )
		return qfalse;

	setvbuf( fp, NULL, _IONBF, 0 ); // don't buffer reads from /dev/urandom

	if ( fread( string, sizeof( byte ), len, fp ) != len ) {
		fclose( fp );
		return qfalse;
	}

	fclose( fp );
	return qtrue;
}


//============================================



#define MAX_FOUND_FILES 0x1000

/*
==================
Sys_ListFilteredFiles
==================
*/
static void Sys_ListFilteredFiles(const char *basedir, const char *subdirs, const char *filter, char **list, int *numfiles) {
	char search[MAX_OSPATH], newsubdirs[MAX_OSPATH];
	char filename[MAX_OSPATH];
	DIR *fdir;
	struct dirent *d;
	struct stat st;

	if (*numfiles >= MAX_FOUND_FILES - 1) {
		return;
	}

	if (basedir[0] == '\0') {
		return;
	}

	if (strlen(subdirs)) {
		Com_sprintf(search, sizeof(search), "%s/%s", basedir, subdirs);
	} else {
		Com_sprintf(search, sizeof(search), "%s", basedir);
	}

	if ((fdir = opendir(search)) == NULL) {
		return;
	}

	while ((d = readdir(fdir)) != NULL) {
		Com_sprintf(filename, sizeof(filename), "%s/%s", search, d->d_name);
		if (stat(filename, &st) == -1)
			continue;

		if (st.st_mode & S_IFDIR) {
			if (Q_stricmp(d->d_name, ".") && Q_stricmp(d->d_name, "..")) {
				if (strlen(subdirs)) {
					Com_sprintf(newsubdirs, sizeof(newsubdirs), "%s/%s", subdirs, d->d_name);
				} else {
					Com_sprintf(newsubdirs, sizeof(newsubdirs), "%s", d->d_name);
				}
				Sys_ListFilteredFiles(basedir, newsubdirs, filter, list, numfiles);
			}
		}
		if (*numfiles >= MAX_FOUND_FILES - 1) {
			break;
		}
		Com_sprintf(filename, sizeof(filename), "%s/%s", subdirs, d->d_name);
		if (!Com_FilterPath(filter, filename, qfalse))
			continue;
		list[*numfiles] = CopyString(filename);
		(*numfiles)++;
	}

	closedir(fdir);
}

/*
==================
Sys_ListFiles
==================
*/
char **Sys_ListFiles(const char *directory, const char *extension, const char *filter, int *numfiles, qboolean wantsubs) {
	struct dirent *d;
	DIR *fdir;
	qboolean dironly = wantsubs;
	char search[MAX_OSPATH];
	int nfiles;
	char **listCopy;
	char *list[MAX_FOUND_FILES];
	int i;
	struct stat st;

	int extLen;

	if (filter) {

		nfiles = 0;
		Sys_ListFilteredFiles(directory, "", filter, list, &nfiles);

		list[nfiles] = NULL;
		*numfiles = nfiles;

		if (!nfiles)
			return NULL;

		listCopy = Z_Malloc((nfiles + 1) * sizeof(*listCopy));
		for (i = 0; i < nfiles; i++) {
			listCopy[i] = list[i];
		}
		listCopy[i] = NULL;

		return listCopy;
	}

	if (directory[0] == '\0') {
		*numfiles = 0;
		return NULL;
	}

	if (!extension)
		extension = "";

	if (extension[0] == '/' && extension[1] == 0) {
		extension = "";
		dironly = qtrue;
	}

	extLen = strlen(extension);

	// search
	nfiles = 0;

	if ((fdir = opendir(directory)) == NULL) {
		*numfiles = 0;
		return NULL;
	}

	while ((d = readdir(fdir)) != NULL) {
		Com_sprintf(search, sizeof(search), "%s/%s", directory, d->d_name);
		if (stat(search, &st) == -1)
			continue;
		if ((dironly && !(st.st_mode & S_IFDIR)) || (!dironly && (st.st_mode & S_IFDIR)))
			continue;

		if (*extension) {
			if (strlen(d->d_name) < extLen || Q_stricmp(d->d_name + strlen(d->d_name) - extLen, extension)) {
				continue; // didn't match
			}
		}

		if (nfiles == MAX_FOUND_FILES - 1)
			break;
		list[nfiles] = CopyString(d->d_name);
		nfiles++;
	}

	list[nfiles] = NULL;

	closedir(fdir);

	// return a copy of the list
	*numfiles = nfiles;

	if (!nfiles) {
		return NULL;
	}

	listCopy = Z_Malloc((nfiles + 1) * sizeof(*listCopy));
	for (i = 0; i < nfiles; i++) {
		listCopy[i] = list[i];
	}
	listCopy[i] = NULL;

	return listCopy;
}

/*
==================
Sys_FreeFileList
==================
*/
void Sys_FreeFileList(char **list) {
	int i;

	if (!list) {
		return;
	}

	for (i = 0; list[i]; i++) {
		Z_Free(list[i]);
	}

	Z_Free(list);
}


/*
=============
Sys_GetFileStats
=============
*/
static qboolean ps5_unused_GetFileStats( const char *filename, int64_t *size, int64_t *mtime, int64_t *ctime ) {
	struct stat s;

	if ( stat( filename, &s ) == 0 ) {
		*size = (int64_t)s.st_size;
		*mtime = (int64_t)s.st_mtime;
		*ctime = (int64_t)s.st_ctime;
		return qtrue;
	} else {
		*size = 0;
		*mtime = *ctime = 0;
		return qfalse;
	}
}


/*
=================
Sys_Mkdir

PS5: everything the title creates stays reachable over FTP (0777)
=================
*/
qboolean Sys_Mkdir( const char *path )
{
	if ( mkdir( path, 0777 ) == 0 ) {
		chmod( path, 0777 );
		return qtrue;
	}
	return errno == EEXIST ? qtrue : qfalse;
}


/*
=================
Sys_FOpen

PS5: files the title writes are 0666, for the same reason
=================
*/
FILE *Sys_FOpen( const char *ospath, const char *mode )
{
	struct stat buf;
	FILE *f;

	// check if path exists and it is not a directory
	if ( stat( ospath, &buf ) == 0 && S_ISDIR( buf.st_mode ) )
		return NULL;

	f = fopen( ospath, mode );
	if ( f && ( strchr( mode, 'w' ) || strchr( mode, 'a' ) || strchr( mode, '+' ) ) )
		fchmod( fileno( f ), 0666 );
	return f;
}


qboolean Sys_ResetReadOnlyAttribute( const char *ospath )
{
	return qfalse;
}


// PS5: the title's folder; there is no working directory to ask for
const char *Sys_Cwd( void )
{
	return "/app0";
}


// PS5: no home; an empty home path makes the engine write under fs_basepath
char *Sys_DefaultHomePath( void )
{
	static char empty[] = "";
	return empty;
}


char *Sys_SteamPath( void )
{
	static char empty[] = "";
	return empty;
}


void Sys_ShowConsole( int visLevel, qboolean quitOnClose )
{
}


// PS5: native qagame/cgame/ui are pre-linked into eboot.bin with localized symbols
extern void qagame_dllEntry( intptr_t (QDECL *syscallptr)( intptr_t arg, ... ) );
extern intptr_t qagame_vmMain( int command, int arg0, int arg1, int arg2, int arg3,
	int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 );
extern char __qagame_data_start[], __qagame_data_end[];
extern char __qagame_bss_start[], __qagame_bss_end[];

extern void cgame_dllEntry( intptr_t (QDECL *syscallptr)( intptr_t arg, ... ) );
extern intptr_t cgame_vmMain( int command, int arg0, int arg1, int arg2, int arg3,
	int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 );
extern char __cgame_data_start[], __cgame_data_end[];
extern char __cgame_bss_start[], __cgame_bss_end[];

extern void ui_dllEntry( intptr_t (QDECL *syscallptr)( intptr_t arg, ... ) );
extern intptr_t ui_vmMain( int command, int arg0, int arg1, int arg2, int arg3,
	int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 );
extern char __ui_data_start[], __ui_data_end[];
extern char __ui_bss_start[], __ui_bss_end[];

typedef struct {
	const char *name;
	void (*dllEntry)( intptr_t (QDECL *syscallptr)( intptr_t arg, ... ) );
	vmMainProc vmMain;
	char *dataStart, *dataEnd, *bssStart, *bssEnd;
	char *pristine;
} ps5NativeModule_t;

static ps5NativeModule_t ps5_native_modules[] = {
	{ "qagame", qagame_dllEntry, qagame_vmMain, __qagame_data_start, __qagame_data_end,
	  __qagame_bss_start, __qagame_bss_end, NULL },
	{ "cgame", cgame_dllEntry, cgame_vmMain, __cgame_data_start, __cgame_data_end,
	  __cgame_bss_start, __cgame_bss_end, NULL },
	{ "ui", ui_dllEntry, ui_vmMain, __ui_data_start, __ui_data_end,
	  __ui_bss_start, __ui_bss_end, NULL },
	{ NULL }
};

void *Sys_LoadLibrary( const char *name )
{
	(void)name;
	return NULL;
}

void Sys_UnloadLibrary( void *handle )
{
	(void)handle;
}

void *Sys_LoadFunction( void *handle, const char *name )
{
	(void)handle; (void)name;
	return NULL;
}

int Sys_LoadFunctionErrors( void )
{
	return 1;
}

void *Sys_LoadGameDll( const char *name, vmMainProc *entryPoint,
	intptr_t (QDECL *systemcalls)( intptr_t, ... ) )
{
	ps5NativeModule_t *m;

	for ( m = ps5_native_modules; m->name; m++ ) {
		size_t dataLen;

		if ( Q_stricmp( name, m->name ) )
			continue;

		dataLen = (size_t)( m->dataEnd - m->dataStart );
		if ( !m->pristine ) {
			m->pristine = malloc( dataLen ? dataLen : 1 );
			if ( !m->pristine )
				Com_Error( ERR_FATAL, "Sys_LoadGameDll: out of memory for %s data copy", name );
			memcpy( m->pristine, m->dataStart, dataLen );
		} else {
			memcpy( m->dataStart, m->pristine, dataLen );
		}
		memset( m->bssStart, 0, (size_t)( m->bssEnd - m->bssStart ) );

		m->dllEntry( systemcalls );
		*entryPoint = m->vmMain;
		Com_Printf( "Sys_LoadGameDll(%s): loaded native module\n", name );
		return (void *)1;
	}

	(void)entryPoint; (void)systemcalls;
	Com_Printf( "Sys_LoadGameDll(%s): unknown module\n", name );
	return NULL;
}
