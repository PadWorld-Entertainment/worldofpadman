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
#ifndef __MATHLIB__
#define __MATHLIB__

// mathlib.h

#include <math.h>

#define SIDE_FRONT 0
#define SIDE_ON 2
#define SIDE_BACK 1
#define SIDE_CROSS -2

#ifndef Q_PI
#define Q_PI 3.14159265358979323846
#endif

extern const vec3_t vec3_origin;

#define EQUAL_EPSILON 0.001

vec_t Q_rint(vec_t in);

vec_t ColorNormalize(vec3_t in, vec3_t out);

void R_ConcatRotations(float in1[3][3], float in2[3][3], float out[3][3]);
void RotatePoint(vec3_t point, vec3_t matrix[3]);
void CreateRotationMatrix(const vec3_t angles, vec3_t matrix[3]);

#endif
