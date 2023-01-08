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

#ifndef CG_SYSCALLS_H
#define CG_SYSCALLS_H

#include "cg_local.h"

#ifdef Q3_VM
#error "Do not use in VM build"
#endif

void trap_CM_TransformedCapsuleTrace(trace_t *results, const vec3_t start, const vec3_t end, const vec3_t min,
									 const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin,
									 const vec3_t angles);
void trap_R_AddAdditiveLightToScene(const vec3_t org, float intensity, float r, float g, float b);
int trap_PC_AddGlobalDefine(const char *define);
int trap_PC_LoadSource(const char *filename);
int trap_PC_FreeSource(int handle);
int trap_PC_ReadToken(int handle, pc_token_t *pc_token);
int trap_PC_SourceFileAndLine(int handle, const char *filename, int *line);
qboolean trap_R_inPVS(const vec3_t p1, const vec3_t p2);

#endif
