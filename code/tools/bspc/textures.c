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

#include "qbsp.h"

static const vec3_t baseaxis[18] = {
	{0, 0, 1},	{1, 0, 0}, {0, -1, 0}, // floor
	{0, 0, -1}, {1, 0, 0}, {0, -1, 0}, // ceiling
	{1, 0, 0},	{0, 1, 0}, {0, 0, -1}, // west wall
	{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}, // east wall
	{0, 1, 0},	{1, 0, 0}, {0, 0, -1}, // south wall
	{0, -1, 0}, {1, 0, 0}, {0, 0, -1}  // north wall
};

void TextureAxisFromPlane(plane_t *pln, vec3_t xv, vec3_t yv) {
	int bestaxis;
	vec_t dot, best;
	int i;

	best = 0;
	bestaxis = 0;

	for (i = 0; i < 6; i++) {
		dot = DotProduct(pln->normal, baseaxis[i * 3]);
		if (dot > best) {
			best = dot;
			bestaxis = i;
		}
	}

	VectorCopy(baseaxis[bestaxis * 3 + 1], xv);
	VectorCopy(baseaxis[bestaxis * 3 + 2], yv);
}
