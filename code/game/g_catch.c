/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of WorldOfPadman source code.

WorldOfPadman source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

WorldOfPadman source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with WorldOfPadman source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// code for the catch the killerduck game mode

#include "g_local.h"
#include "g_spawn.h"
#include "inv.h"

// TODO: cgame add killerduck to hand
// TODO: make sounds while carrying the duck

void G_DropKillerDucks(gentity_t *self) {
	const gitem_t *item;
	gentity_t *ent;

	self->client->pers.lastKillerDuckPoints = 0;
	self->client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
	self->client->ps.stats[STAT_HOLDABLEVAR] = 0;
	self->client->ps.eFlags &= ~EF_KILLERDUCK;
	trap_SendServerCommand(-1, va("cp \"%s" S_COLOR_WHITE " is not longer the KillerDuck.\n\"", self->client->pers.netname));

	ent = G_Spawn();
	ent->classname = "holdable_killerducks";
	if (SelectSpawnPoint(self->r.currentOrigin, ent->s.origin, ent->s.angles, qfalse) == NULL) {
		VectorCopy(self->r.currentOrigin, ent->s.origin);
	}
	VectorCopy(ent->s.origin, ent->s.pos.trBase);
	VectorCopy(ent->s.origin, ent->r.currentOrigin);

	// check item spawn functions
	for (item = bg_itemlist + 1; item->classname; item++) {
		if (!strcmp(item->classname, ent->classname)) {
			G_SpawnItem(ent, item);
			ent->nextthink = level.time + FRAMETIME * 20;
			return;
		}
	}
	Com_Error(ERR_DROP, "Failed to respawn killerducks");
}

void G_KillerDuckPoints(gentity_t *ent) {
	const int carrySec = (level.time - ent->client->pers.lastKillerDuckPoints) / 1000;
	const int pointSecs = 3;
	if (carrySec >= pointSecs) {
		const int score = carrySec / pointSecs;
		ent->client->pers.lastKillerDuckPoints += score * pointSecs * 1000;
		AddScore(ent, ent->r.currentOrigin, score, "killerduck");
	}
}

void G_GetKillerDuck(gentity_t *ent) {
	if (g_gametype.integer != GT_CATCH) {
		return;
	}
	// we only carry one killer duck in this game mode
	ent->client->ps.stats[STAT_HOLDABLEVAR] = 1;
	ent->client->pers.lastKillerDuckPoints = level.time;
	ent->client->ps.eFlags |= EF_KILLERDUCK;
	trap_SendServerCommand(-1, va("cp \"%s" S_COLOR_WHITE " is the KillerDuck.\n\"", ent->client->pers.netname));
}

qboolean G_IsKillerDuck(const gentity_t *ent) {
	return ent->client->ps.eFlags & EF_KILLERDUCK;
}
