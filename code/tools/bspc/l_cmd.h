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

#include "q_shared.h"

char *strupr(char *in);
char *strlower(char *in);
int Q_strncasecmp(const char *s1, const char *s2, int n);
int Q_strcasecmp(const char *s1, const char *s2);
void Q_strncpyz(char *dest, const char *src, int destsize);

int Q_filelength(FILE *f);

void Q_mkdir(const char *path);

double I_FloatTime(void);

void Error(const char *error, ...) __attribute__((format(printf, 1, 2)));
void Warning(const char *warning, ...) __attribute__((format(printf, 1, 2)));

FILE *SafeOpenWrite(const char *filename);
FILE *SafeOpenRead(const char *filename);
void SafeRead(FILE *f, void *buffer, int count);
void SafeWrite(FILE *f, void *buffer, int count);

int LoadFile(const char *filename, void **bufferptr, int offset, int length);
int TryLoadFile(const char *filename, void **bufferptr);
void SaveFile(const char *filename, void *buffer, int count);
qboolean FileExists(const char *filename);

void StripExtension(char *path);

void ExtractFileBase(const char *path, char *dest);
void ExtractFileExtension(const char *path, char *dest);

extern char com_token[1024];
extern qboolean com_eof;

char *copystring(const char *s);

void CreatePath(const char *path);

extern qboolean archive;
extern char archivedir[1024];

extern qboolean verbose;
void qprintf(char *format, ...) __attribute__((format(printf, 1, 2)));

void FS_FreeFile(void *buf);
int FS_FOpenFileRead(const char *filename, FILE **file, qboolean uniqueFILE);

#endif
