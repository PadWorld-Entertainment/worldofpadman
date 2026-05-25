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
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// cmdlib.c

#include "l_cmd.h"
#include "l_log.h"
#include "l_mem.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>

#if defined(WIN32) || defined(_WIN32)
#include <direct.h>
#else
#include <unistd.h>
#endif

#define PATHSEPERATOR '/'

char com_token[1024];
qboolean com_eof;

qboolean archive;
char archivedir[1024];

/*
=================
Error

For abnormal program terminations in console apps
=================
*/
void Error(char *error, ...) {
	va_list argptr;
	char text[1024];

	va_start(argptr, error);
	vsprintf(text, error, argptr);
	va_end(argptr);
	printf("ERROR: %s\n", text);

	Log_Write("%s", text);
	Log_Close();

	exit(1);
}

void Warning(char *warning, ...) {
	va_list argptr;
	char text[1024];

	va_start(argptr, warning);
	vsprintf(text, warning, argptr);
	va_end(argptr);
	printf("WARNING: %s\n", text);

	Log_Write("%s", text);
}

// only printf if in verbose mode
qboolean verbose = qtrue;

void qprintf(char *format, ...) {
	va_list argptr;

	if (!verbose)
		return;

	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);
}

void QDECL Com_Error(int level, const char *error, ...) {
	va_list argptr;
	char text[1024];

	va_start(argptr, error);
	vsprintf(text, error, argptr);
	va_end(argptr);
	Error("%s", text);
	exit(1); // unreachable, but satisfies noreturn
}

void QDECL Com_Printf(const char *fmt, ...) {
	va_list argptr;
	char text[1024];

	va_start(argptr, fmt);
	vsprintf(text, fmt, argptr);
	va_end(argptr);
	Log_Print("%s", text);
}

char *copystring(char *s) {
	char *b;
	b = GetMemory(strlen(s) + 1);
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
}

void Q_mkdir(char *path) {
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

int Q_strcasecmp(const char *s1, const char *s2) {
	return Q_strncasecmp(s1, s2, 99999);
}

int Q_stricmp(const char *s1, const char *s2) {
	return Q_strncasecmp(s1, s2, 99999);
}

void Q_strncpyz(char *dest, const char *src, int destsize) {
	strncpy(dest, src, destsize - 1);
	dest[destsize - 1] = 0;
}

char *strupr(char *start) {
	char *in;
	in = start;
	while (*in) {
		*in = toupper(*in);
		in++;
	}
	return start;
}

char *strlower(char *start) {
	char *in;
	in = start;
	while (*in) {
		*in = tolower(*in);
		in++;
	}
	return start;
}

/*
=============================================================================

						MISC FUNCTIONS

=============================================================================
*/

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

FILE *SafeOpenWrite(char *filename) {
	FILE *f;

	f = fopen(filename, "wb");

	if (!f)
		Error("Error opening %s: %s", filename, strerror(errno));

	return f;
}

FILE *SafeOpenRead(char *filename) {
	FILE *f;

	f = fopen(filename, "rb");

	if (!f)
		Error("Error opening %s: %s", filename, strerror(errno));

	return f;
}

void SafeRead(FILE *f, void *buffer, int count) {
	if (fread(buffer, 1, count, f) != (size_t)count)
		Error("File read failure");
}

void SafeWrite(FILE *f, void *buffer, int count) {
	if (fwrite(buffer, 1, count, f) != (size_t)count)
		Error("File write failure");
}

/*
==============
FileExists
==============
*/
qboolean FileExists(char *filename) {
	FILE *f;

	f = fopen(filename, "r");
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
int LoadFile(char *filename, void **bufferptr, int offset, int length) {
	FILE *f;
	void *buffer;

	f = SafeOpenRead(filename);
	fseek(f, offset, SEEK_SET);
	if (!length)
		length = Q_filelength(f);
	buffer = GetMemory(length + 1);
	((char *)buffer)[length] = 0;
	SafeRead(f, buffer, length);
	fclose(f);

	*bufferptr = buffer;
	return length;
}

/*
==============
TryLoadFile

Allows failure
==============
*/
int TryLoadFile(char *filename, void **bufferptr) {
	FILE *f;
	int length;
	void *buffer;

	*bufferptr = NULL;

	f = fopen(filename, "rb");
	if (!f)
		return -1;
	length = Q_filelength(f);
	buffer = GetMemory(length + 1);
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
void SaveFile(char *filename, void *buffer, int count) {
	FILE *f;

	f = SafeOpenWrite(filename);
	SafeWrite(f, buffer, count);
	fclose(f);
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

void ExtractFileBase(char *path, char *dest) {
	char *src;

	src = path + strlen(path) - 1;

	//
	// back up until a \ or the start
	//
	while (src != path && *(src - 1) != '\\' && *(src - 1) != '/')
		src--;

	while (*src && *src != '.') {
		*dest++ = *src++;
	}
	*dest = 0;
}

void ExtractFileExtension(char *path, char *dest) {
	char *src;

	src = path + strlen(path) - 1;

	//
	// back up until a . or the start
	//
	while (src != path && *(src - 1) != '.')
		src--;
	if (src == path) {
		*dest = 0; // no extension
		return;
	}

	strcpy(dest, src);
}

/*
============
CreatePath
============
*/
void CreatePath(char *path) {
	char *ofs, c;

	if (path[1] == ':')
		path += 2;

	for (ofs = path + 1; *ofs; ofs++) {
		c = *ofs;
		if (c == '/' || c == '\\') { // create the directory
			*ofs = 0;
			Q_mkdir(path);
			*ofs = c;
		}
	}
}

void FS_FreeFile(void *buf) {
	FreeMemory(buf);
}

int FS_FOpenFileRead(const char *filename, FILE **file, qboolean uniqueFILE) {
	*file = fopen(filename, "rb");
	return (*file != NULL);
}

#ifdef _WIN32
int Q_vsnprintf(char *str, size_t size, const char *format, va_list ap) {
	int retval;

	retval = _vsnprintf(str, size, format, ap);

	if (retval < 0 || (size_t)retval == size) {
		str[size - 1] = '\0';
		return size;
	}

	return retval;
}
#endif
