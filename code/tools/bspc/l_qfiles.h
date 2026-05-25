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

#include "qcommon/unzip.h"
#include "l_utils.h"

#define QFILETYPE_UNKNOWN 0x8000
#define QFILETYPE_PK3 0x0002
#define QFILETYPE_BSP 0x0004
#define QFILETYPE_AAS 0x4000

#define QFILEEXT_UNKNOWN ""
#define QFILEEXT_PK3 ".PK3"
#define QFILEEXT_BSP ".BSP"
#define QFILEEXT_AAS ".AAS"

typedef struct quakefile_s {
	char pakfile[MAX_PATH];
	char filename[MAX_PATH];
	char origname[MAX_PATH];
	int zipfile;
	int type;
	int offset;
	int length;
	unz_file_pos filepos;
	struct quakefile_s *next;
} quakefile_t;

// return the Quake file type for the given file
int QuakeFileType(char *filename);
// returns true if the filename complies to the filter
int FileFilter(char *filter, char *filename, int casesensitive);
// find Quake files using the given filter
quakefile_t *FindQuakeFiles(char *filter);
// load the given Quake file, returns the length of the file
int LoadQuakeFile(quakefile_t *qf, void **bufferptr);
// read part of a Quake file into the buffer
int ReadQuakeFile(quakefile_t *qf, void *buffer, int offset, int length);
