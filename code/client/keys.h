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
#include "keycodes.h"
#include "../qcommon/qcommon.h"
#include "../qcommon/q_shared.h"

typedef struct {
	qboolean down;
	int repeats; // if > 1, it is autorepeating
	char *binding;
} qkey_t;

extern qkey_t keys[MAX_KEYS];

void Field_Draw(field_t *edit, int x, int y, int width, qboolean showCursor, qboolean noColorEscape, fontSize_t fontsize);

#define COMMAND_HISTORY 32
extern field_t historyEditLines[COMMAND_HISTORY];

extern field_t g_consoleField;
extern field_t chatField;
extern int anykeydown;
extern qboolean chat_team;
extern int chat_playerNum;

void Key_WriteBindings(fileHandle_t f);
void Key_SetBinding(int keynum, const char *binding);
const char *Key_GetBinding(int keynum);
qboolean Key_IsDown(int keynum);
qboolean Key_GetOverstrikeMode(void);
void Key_SetOverstrikeMode(qboolean state);
void Key_ClearStates(void);
int Key_GetKey(const char *binding);
