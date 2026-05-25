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
// cmdlib.h

#ifndef __CMDLIB__
#define __CMDLIB__

#ifndef __GNUC__
#ifndef __attribute__
#define __attribute__(x)
#endif
#endif

#ifdef _WIN32
#pragma warning(disable : 4244) // MIPS
#pragma warning(disable : 4136) // X86
#pragma warning(disable : 4051) // ALPHA

#pragma warning(disable : 4018) // signed/unsigned mismatch
#pragma warning(disable : 4305) // truncate from double to float

// Disable these: warning C4996: 'stricmp': The POSIX name for this item is deprecated. Instead, use the ISO C and C++
// conformant name: _stricmp. See online help for details.
#pragma warning(disable : 4996)

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>

#include "q_shared.h"

// the dec offsetof macro doesnt work very well...
#define myoffsetof(type, identifier) ((size_t)&((type *)0)->identifier)

// set these before calling CheckParm
extern int myargc;
extern char **myargv;

char *strupr(char *in);
char *strlower(char *in);
int Q_strncasecmp(const char *s1, const char *s2, int n);
int Q_strcasecmp(const char *s1, const char *s2);
void Q_strncpyz(char *dest, const char *src, int destsize);
void Q_getwd(char *out, size_t size);

int Q_filelength(FILE *f);
int FileTime(char *path);

void Q_mkdir(char *path);

extern char qdir[1024];
extern char gamedir[1024];
void SetQdirFromPath(char *path);
char *ExpandArg(char *path);  // from cmd line
char *ExpandPath(char *path); // from scripts
char *ExpandPathAndArchive(char *path);

double I_FloatTime(void);

void Error(char *error, ...) __attribute__((format(printf, 1, 2)));
void Warning(char *warning, ...) __attribute__((format(printf, 1, 2)));

int CheckParm(char *check);

FILE *SafeOpenWrite(char *filename);
FILE *SafeOpenRead(char *filename);
void SafeRead(FILE *f, void *buffer, int count);
void SafeWrite(FILE *f, void *buffer, int count);

int LoadFile(char *filename, void **bufferptr, int offset, int length);
int TryLoadFile(char *filename, void **bufferptr);
void SaveFile(char *filename, void *buffer, int count);
qboolean FileExists(char *filename);

void DefaultExtension(char *path, char *extension);
void DefaultPath(char *path, char *basepath);
void StripFilename(char *path);
void StripExtension(char *path);

void ExtractFilePath(char *path, char *dest);
void ExtractFileBase(char *path, char *dest);
void ExtractFileExtension(char *path, char *dest);

int ParseNum(char *str);

extern char com_token[1024];
extern qboolean com_eof;

char *copystring(char *s);

void CRC_Init(unsigned short *crcvalue);
void CRC_ProcessByte(unsigned short *crcvalue, byte data);
unsigned short CRC_Value(unsigned short crcvalue);

void CreatePath(char *path);
void QCopyFile(char *from, char *to);

extern qboolean archive;
extern char archivedir[1024];

extern qboolean verbose;
void qprintf(char *format, ...) __attribute__((format(printf, 1, 2)));

void ExpandWildcards(int *argc, char ***argv);

// for compression routines
typedef struct {
	byte *data;
	int count;
} cblock_t;

int ParseHex(char *hex);
void FS_FreeFile(void *buf);
int FS_ReadFileAndCache(const char *qpath, void **buffer);
int FS_FOpenFileRead(const char *filename, FILE **file, qboolean uniqueFILE);

#endif
