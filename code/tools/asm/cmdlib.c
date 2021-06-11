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
// cmdlib.c

#include "cmdlib.h"
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
#include <direct.h>
#include <windows.h>
#elif defined(NeXT)
#include <libc.h>
#else
#include <unistd.h>
#endif

#define BASEDIRNAME "quake" // assumed to have a 2 or 3 following
#define PATHSEPERATOR '/'

// set these before calling CheckParm
int myargc;
char **myargv;

char com_token[1024];
qboolean com_eof;

qboolean archive;
char archivedir[1024];

/*
===================
ExpandWildcards

Mimic unix command line expansion
===================
*/
#define MAX_EX_ARGC 1024
int ex_argc;
char *ex_argv[MAX_EX_ARGC];
#ifdef _WIN32
#include "io.h"
void ExpandWildcards(int *argc, char ***argv) {
	struct _finddata_t fileinfo;
	intptr_t handle;
	int i;
	char filename[1024];
	char filebase[1024];
	char *path;

	ex_argc = 0;
	for (i = 0; i < *argc; i++) {
		path = (*argv)[i];
		if (path[0] == '-' || (!strstr(path, "*") && !strstr(path, "?"))) {
			ex_argv[ex_argc++] = path;
			continue;
		}

		handle = _findfirst(path, &fileinfo);
		if (handle == -1)
			return;

		ExtractFilePath(path, filebase);

		do {
			sprintf(filename, "%s%s", filebase, fileinfo.name);
			ex_argv[ex_argc++] = copystring(filename);
		} while (_findnext(handle, &fileinfo) != -1);

		_findclose(handle);
	}

	*argc = ex_argc;
	*argv = ex_argv;
}
#else
void ExpandWildcards(int *argc, char ***argv) {
}
#endif

#ifdef WIN_ERROR
#include <windows.h>
/*
=================
Error

For abnormal program terminations in windowed apps
=================
*/
void Error(const char *error, ...) {
	va_list argptr;
	char text[1024];
	char text2[1024];
	int err;

	err = GetLastError();

	va_start(argptr, error);
	vsprintf(text, error, argptr);
	va_end(argptr);

	sprintf(text2, "%s\nGetLastError() = %i", text, err);
	MessageBox(NULL, text2, "Error", 0 /* MB_OK */);

	exit(1);
}

#else

static void _printf(const char *format, ...);

/*
=================
Error

For abnormal program terminations in console apps
=================
*/
void Error(const char *error, ...) {
	va_list argptr;

	_printf("\n************ ERROR ************\n");

	va_start(argptr, error);
	vprintf(error, argptr);
	va_end(argptr);
	_printf("\r\n");

	exit(1);
}
#endif

// only printf if in verbose mode
qboolean verbose = qfalse;
void qprintf(const char *format, ...) {
	va_list argptr;

	if (!verbose)
		return;

	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);
}

#ifdef WIN32
HWND hwndOut = NULL;
qboolean lookedForServer = qfalse;
UINT wm_BroadcastCommand = -1;
#endif

void _printf(const char *format, ...) {
	va_list argptr;
	char text[4096];
#ifdef WIN32
	ATOM a;
#endif
	va_start(argptr, format);
	vsprintf(text, format, argptr);
	va_end(argptr);

	printf("%s", text);

#ifdef WIN32
	if (!lookedForServer) {
		lookedForServer = qtrue;
		hwndOut = FindWindow(NULL, "Q3Map Process Server");
		if (hwndOut) {
			wm_BroadcastCommand = RegisterWindowMessage("Q3MPS_BroadcastCommand");
		}
	}
	if (hwndOut) {
		a = GlobalAddAtom(text);
		PostMessage(hwndOut, wm_BroadcastCommand, 0, (LPARAM)a);
	}
#endif
}

/*

qdir will hold the path up to the quake directory, including the slash

  f:\quake\
  /raid/quake/

gamedir will hold qdir + the game directory (id1, id2, etc)

  */

char qdir[1024];
char gamedir[1024];

char *copystring(const char *s) {
	char *b;
	b = malloc(strlen(s) + 1);
	strcpy(b, s);
	return b;
}

/*
================
I_FloatTime
================
*/
double I_FloatTime(void) {
	time_t t;

	time(&t);

	return t;
#if 0
// more precise, less portable
	struct timeval tp;
	struct timezone tzp;
	static int		secbase;

	gettimeofday(&tp, &tzp);

	if (!secbase)
	{
		secbase = tp.tv_sec;
		return tp.tv_usec/1000000.0;
	}

	return (tp.tv_sec - secbase) + tp.tv_usec/1000000.0;
#endif
}

void Q_getwd(char *out) {
	int i = 0;

#ifdef WIN32
	if (_getcwd(out, 256) == NULL)
		strcpy(out, "."); /* shrug */
	strcat(out, "\\");
#else
	if (getcwd(out, 256) == NULL)
		strcpy(out, "."); /* shrug */
	strcat(out, "/");
#endif

	while (out[i] != 0) {
		if (out[i] == '\\')
			out[i] = '/';
		i++;
	}
}

void Q_mkdir(const char *path) {
#ifdef WIN32
	if (_mkdir(path) != -1)
		return;
#else
	if (mkdir(path, 0777) != -1)
		return;
#endif
	if (errno != EEXIST)
		Error("mkdir %s: %s", path, strerror(errno));
}

/*
============
FileTime

returns -1 if not present
============
*/
int FileTime(const char *path) {
	struct stat buf;

	if (stat(path, &buf) == -1)
		return -1;

	return buf.st_mtime;
}

/*
==============
COM_Parse

Parse a token out of a string
==============
*/
const char *COM_Parse(const char *data) {
	int c;
	int len;

	len = 0;
	com_token[0] = 0;

	if (!data)
		return NULL;

// skip whitespace
skipwhite:
	while ((c = *data) <= ' ') {
		if (c == 0) {
			com_eof = qtrue;
			return NULL; // end of file;
		}
		data++;
	}

	// skip // comments
	if (c == '/' && data[1] == '/') {
		while (*data && *data != '\n')
			data++;
		goto skipwhite;
	}

	// handle quoted strings specially
	if (c == '\"') {
		data++;
		do {
			c = *data++;
			if (c == '\"') {
				com_token[len] = 0;
				return data;
			}
			com_token[len] = c;
			len++;
		} while (1);
	}

	// parse single characters
	if (c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ':') {
		com_token[len] = c;
		len++;
		com_token[len] = 0;
		return data + 1;
	}

	// parse a regular word
	do {
		com_token[len] = c;
		data++;
		len++;
		c = *data;
		if (c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ':')
			break;
	} while (c > 32);

	com_token[len] = 0;
	return data;
}

int Q_strncasecmp(const char *s1, const char *s2, int n) {
	int c1, c2;

	do {
		c1 = *s1++;
		c2 = *s2++;

		if (!n--)
			return 0; // strings are equal until end point

		if (c1 != c2) {
			if (c1 >= 'a' && c1 <= 'z')
				c1 -= ('a' - 'A');
			if (c2 >= 'a' && c2 <= 'z')
				c2 -= ('a' - 'A');
			if (c1 != c2)
				return -1; // strings not equal
		}
	} while (c1);

	return 0; // strings are equal
}

int Q_stricmp(const char *s1, const char *s2) {
	return Q_strncasecmp(s1, s2, 99999);
}

/*
=============================================================================

						MISC FUNCTIONS

=============================================================================
*/

/*
=================
CheckParm

Checks for the given parameter in the program's command line arguments
Returns the argument number (1 to argc-1) or 0 if not present
=================
*/
int CheckParm(const char *check) {
	int i;

	for (i = 1; i < myargc; i++) {
		if (!Q_stricmp(check, myargv[i]))
			return i;
	}

	return 0;
}

/*
================
Q_filelength
================
*/
int Q_filelength(FILE *f) {
	int pos;
	int end;

	pos = ftell(f);
	fseek(f, 0, SEEK_END);
	end = ftell(f);
	fseek(f, pos, SEEK_SET);

	return end;
}

#ifdef MAX_PATH
#undef MAX_PATH
#endif
#define MAX_PATH 4096
static FILE *myfopen(const char *filename, const char *mode) {
	char *p;
	char fn[MAX_PATH];

	fn[0] = '\0';
	strncat(fn, filename, sizeof(fn) - 1);

	for (p = fn; *p; ++p)
		if (*p == '\\')
			*p = '/';

	return fopen(fn, mode);
}

FILE *SafeOpenWrite(const char *filename) {
	FILE *f;

	f = myfopen(filename, "wb");

	if (!f)
		Error("Error opening %s: %s", filename, strerror(errno));

	return f;
}

FILE *SafeOpenRead(const char *filename) {
	FILE *f;

	f = myfopen(filename, "rb");

	if (!f)
		Error("Error opening %s: %s", filename, strerror(errno));

	return f;
}

void SafeRead(FILE *f, void *buffer, int count) {
	if (fread(buffer, 1, count, f) != (size_t)count)
		Error("File read failure");
}

void SafeWrite(FILE *f, const void *buffer, int count) {
	if (fwrite(buffer, 1, count, f) != (size_t)count)
		Error("File write failure");
}

/*
==============
FileExists
==============
*/
qboolean FileExists(const char *filename) {
	FILE *f;

	f = myfopen(filename, "r");
	if (!f)
		return qfalse;
	fclose(f);
	return qtrue;
}

/*
==============
LoadFile
==============
*/
int LoadFile(const char *filename, void **bufferptr) {
	FILE *f;
	int length;
	void *buffer;

	f = SafeOpenRead(filename);
	length = Q_filelength(f);
	buffer = malloc(length + 1);
	((char *)buffer)[length] = 0;
	SafeRead(f, buffer, length);
	fclose(f);

	*bufferptr = buffer;
	return length;
}

/*
==============
SaveFile
==============
*/
void SaveFile(const char *filename, const void *buffer, int count) {
	FILE *f;

	f = SafeOpenWrite(filename);
	SafeWrite(f, buffer, count);
	fclose(f);
}

void DefaultExtension(char *path, const char *extension) {
	char *src;
	//
	// if path doesn't have a .EXT, append extension
	// (extension should include the .)
	//
	src = path + strlen(path) - 1;

	while (*src != '/' && *src != '\\' && src != path) {
		if (*src == '.')
			return; // it has an extension
		src--;
	}

	strcat(path, extension);
}

void DefaultPath(char *path, const char *basepath) {
	char temp[128];

	if (path[0] == PATHSEPERATOR)
		return; // absolute path location
	strcpy(temp, path);
	strcpy(path, basepath);
	strcat(path, temp);
}

void StripFilename(char *path) {
	int length;

	length = strlen(path) - 1;
	while (length > 0 && path[length] != PATHSEPERATOR)
		length--;
	path[length] = 0;
}

void StripExtension(char *path) {
	int length;

	length = strlen(path) - 1;
	while (length > 0 && path[length] != '.') {
		length--;
		if (path[length] == '/')
			return; // no extension
	}
	if (length)
		path[length] = 0;
}

/*
====================
Extract file parts
====================
*/
// FIXME: should include the slash, otherwise
// backing to an empty path will be wrong when appending a slash
void ExtractFilePath(const char *path, char *dest) {
	const char *src;

	src = path + strlen(path) - 1;

	//
	// back up until a \ or the start
	//
	while (src != path && *(src - 1) != '\\' && *(src - 1) != '/')
		src--;

	memcpy(dest, path, src - path);
	dest[src - path] = 0;
}

void ExtractFileBase(const char *path, char *dest) {
	const char *src;

	src = path + strlen(path) - 1;

	//
	// back up until a \ or the start
	//
	while (src != path && *(src - 1) != PATHSEPERATOR)
		src--;

	while (*src && *src != '.') {
		*dest++ = *src++;
	}
	*dest = 0;
}

/*
============
CreatePath
============
*/
void CreatePath(const char *path) {
	const char *ofs;
	char c;
	char dir[1024];

#ifdef _WIN32
	int olddrive = -1;

	if (path[1] == ':') {
		olddrive = _getdrive();
		_chdrive(toupper(path[0]) - 'A' + 1);
	}
#endif

	if (path[1] == ':')
		path += 2;

	for (ofs = path + 1; *ofs; ofs++) {
		c = *ofs;
		if (c == '/' || c == '\\') { // create the directory
			memcpy(dir, path, ofs - path);
			dir[ofs - path] = 0;
			Q_mkdir(dir);
		}
	}

#ifdef _WIN32
	if (olddrive != -1) {
		_chdrive(olddrive);
	}
#endif
}
