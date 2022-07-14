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

#include "tr_shared.h"
#include "../qcommon/q_shared.h"

/*
==================
R_ScreenshotFilename
==================
*/
void R_ScreenshotFilename(int lastNumber, char *fileName, const char *ext) {
	int a, b, c, d;

	if (lastNumber < 0 || lastNumber > 9999) {
		Com_sprintf(fileName, MAX_OSPATH, "screenshots/shot9999.%s", ext);
		return;
	}

	a = lastNumber / 1000;
	lastNumber -= a * 1000;
	b = lastNumber / 100;
	lastNumber -= b * 100;
	c = lastNumber / 10;
	lastNumber -= c * 10;
	d = lastNumber;

	Com_sprintf(fileName, MAX_OSPATH, "screenshots/shot%i%i%i%i.%s", a, b, c, d, ext);
}
