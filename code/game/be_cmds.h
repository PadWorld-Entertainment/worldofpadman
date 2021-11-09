/*
===========================================================================
Copyright (C) 2010, 2011, 2012 the-brain

This program is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://gnu.org/licenses/>.
===========================================================================
*/

#ifndef _BE_CMDS_H
#define _BE_CMDS_H

enum { CMD_CHEAT = 1, CMD_MESSAGE = 2, CMD_LIVING = 4, CMD_INTERMISSION = 8 };

typedef struct {
	char *cmdName;
	int cmdFlags;
	void (*cmdHandler)(gentity_t *ent);
} ccmd_t;

qboolean BE_ClCmd(gentity_t *ent, const char *cmd);

void BE_Cmd_Ignore_f(gentity_t *ent);
void BE_Cmd_TellSpectator_f(gentity_t *ent);
void BE_Cmd_QueryCvarResponse_f(gentity_t *ent);
void BE_Cmd_Players_f(gentity_t *ent);

#endif
