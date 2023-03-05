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

#include "cg_local.h"

qboolean CG_FreezeTag(void) {
	return cgs.gametype == GT_FREEZETAG;
}

qboolean FT_LocalIsFrozen(void) {
	if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR) {
		if (!(cg.snap->ps.pm_flags & PMF_FOLLOW)) {
			return qfalse;
		}
	}
	return cg.snap->ps.powerups[PW_FREEZE];
}

qboolean FT_PlayerIsFrozen(const centity_t *cent) {
	return cent->currentState.powerups & (1 << PW_FREEZE);
}
