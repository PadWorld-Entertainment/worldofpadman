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
//
#ifndef UI_SYSCALLS_H
#define UI_SYSCALLS_H

#include "ui_local.h"

void trap_R_RegisterFont(const char *fontName, int pointSize, fontInfo_t *font);
void trap_R_ModelBounds(clipHandle_t model, vec3_t mins, vec3_t maxs);
int trap_LAN_GetServerPing(int source, int n);
void trap_LAN_SaveCachedServers(void);
void trap_LAN_LoadCachedServers(void);
void trap_LAN_ResetPings(int n);
void trap_LAN_MarkServerVisible(int source, int n, qboolean visible);
int trap_LAN_ServerIsVisible(int source, int n);
qboolean trap_LAN_UpdateVisiblePings(int source);
int trap_LAN_AddServer(int source, const char *name, const char *addr);
void trap_LAN_RemoveServer(int source, const char *addr);
int trap_LAN_CompareServers(int source, int sortKey, int sortDir, int s1, int s2);
int trap_PC_AddGlobalDefine(const char *define);
int trap_PC_LoadSource(const char *filename);
int trap_PC_FreeSource(int handle);
int trap_PC_ReadToken(int handle, pc_token_t *pc_token);
int trap_PC_SourceFileAndLine(int handle, const char *filename, int *line);
void trap_R_RemapShader(const char *oldShader, const char *newShader, const char *timeOffset);

#endif
