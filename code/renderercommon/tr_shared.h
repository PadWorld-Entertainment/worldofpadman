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
#ifndef TR_SHARED_H
#define TR_SHARED_H

typedef enum {
	ST_TGA,
	ST_JPEG,
	ST_PNG
} screenshotType_e;

typedef struct {
	int commandId;
	int x;
	int y;
	int width;
	int height;
	const char *fileName;
	screenshotType_e type;
} screenshotCommand_t;

void R_ScreenshotFilename(int lastNumber, char *fileName, int size, const char *ext);

#endif
