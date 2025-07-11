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
#include "inv.h"

#define RESPAWN_DROPPED_KILLERDUCKS 5

// TODO: don't free them - but allow to re-use those entities to re-spawn the killerduck here
static void G_EnsureExactlyOne(const char *name) {
	gentity_t *item = G_Find(NULL, FOFS(classname), name);
	if (item == NULL) {
		Com_Printf("WARNING: No %s found\n", name);
		return;
	}

	do {
		item = G_Find(item, FOFS(classname), name);
		if (item == NULL) {
			break;
		}
		G_FreeEntity(item);
	} while (item);
}

/**
 * @brief Checks that enough holdable_killerducks are spawned
 *
 * Removes all holdable_killerducks if there are too many
 */
void G_CatchHandleHoldableKillerducks(void) {
	G_EnsureExactlyOne("holdable_killerducks");
}

/**
 * Executes the respawn of the killerducks after a given timeout at one of the spawn points
 */
static void G_RespawnKillerDucks(gentity_t *self) {
	gentity_t *ent, *spawnPoint = NULL;
	const gitem_t *item;

	trap_SendServerCommand(-1, va("cp \"The KillerDuck respawned.\n\""));
	trap_SendServerCommand(-1, va("print \"The KillerDuck respawned.\n\""));

	item = BG_FindItemForHoldable(HI_KILLERDUCKS);
	if (item == NULL) {
		Com_Error(ERR_DROP, "Failed to respawn the KillerDuck");
	}

	ent = G_Spawn();
	ent->classname = item->classname;
	ent->think = FinishSpawningItem;

	// randomly also pick the original spawn point (if available)
	if (rand() % 5 == 1) {
		spawnPoint = G_Find(NULL, FOFS(classname), "holdable_killerducks");
	}
	if (spawnPoint) {
		VectorCopy(spawnPoint->r.currentOrigin, ent->s.origin);
	} else if (SelectSpawnPoint(self->r.currentOrigin, ent->s.origin, ent->s.angles, qfalse) == NULL) {
		VectorCopy(self->r.currentOrigin, ent->s.origin);
	}
	VectorCopy(ent->s.origin, ent->s.pos.trBase);
	VectorCopy(ent->s.origin, ent->r.currentOrigin);

	G_SpawnItem(ent, item);
}

/**
 * This think function is set in @c LaunchItem and ensure to respawn the
 * killerduck after a few seconds
 */
static void G_DroppedKillerDucksThinkRespawn(gentity_t *self) {
	G_RespawnKillerDucks(self);
	G_FreeEntity(self);
}

static void G_DroppedKillerDucksThinkPickable(gentity_t *ent) {
	ent->nextthink = (level.time + RESPAWN_DROPPED_KILLERDUCKS * 1000);
	ent->think = G_DroppedKillerDucksThinkRespawn;
}

/**
 * The respawn of the killerduck happens on death in @c TossClientItems
 */
gentity_t *G_DropKillerDucks(gentity_t *ent) {
	const int clientNum = ent - g_entities;
	vec3_t velocity, angles;
	gclient_t *client = ent->client;
	gentity_t *killerDucks;

	if (!G_IsKillerDuck(ent)) {
		return NULL;
	}

	client->pers.lastKillerDuckPoints = 0;
	client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
	client->ps.stats[STAT_HOLDABLEVAR] = 0;
	G_RestoreClientLastWeapon(client, qtrue);
	client->ps.eFlags &= ~EF_KILLERDUCK;
	trap_SendServerCommand(-1, va("cp \"%s" S_COLOR_WHITE " lost the KillerDuck.\n\"", client->pers.netname));
	if (ent->client->ps.clientNum == clientNum) {
		trap_SendServerCommand(clientNum, va("cp \"%s" S_COLOR_WHITE, "You lost the KillerDuck.\n\""));
	}
	trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " lost the KillerDuck.\n\"", client->pers.netname));

	VectorCopy(ent->s.apos.trBase, angles);
	AngleVectors(angles, velocity, NULL, NULL);
	VectorScale(velocity, 600, velocity);
	velocity[2] += 200;

	killerDucks = LaunchItem(BG_FindItemForHoldable(HI_KILLERDUCKS), ent->s.pos.trBase, velocity);
	if (killerDucks == NULL) {
		Com_Error(ERR_DROP, "Failed to drop the KillerDuck");
	}

	// wait a bit before we send a warning for not being able to pick up
	// the killerducks that fast again - see PICKUP_KILLERDUCKS_NOT_YET_AGAIN
	// and the handling in Touch_Item()
	client->lastWarningMessageTime = level.time;
	// don't allow to re-collect for 5+1 seconds
	// see BG_CanItemBeGrabbed()
	killerDucks->s.otherEntityNum = ent->s.number;
	killerDucks->s.time = level.time + (RESPAWN_DROPPED_KILLERDUCKS + 1) * 1000;
	killerDucks->nextthink = (level.time + 1000);
	killerDucks->think = G_DroppedKillerDucksThinkPickable;
	return killerDucks;
}

/**
 * This is called for the killerduck carrier in (its client think function) to distribute the points
 */
void G_KillerDuckThink(gentity_t *ent) {
	const int carrySec = (level.time - ent->client->pers.lastKillerDuckPoints) / 1000;
	const int pointSecs = 3;
	if (carrySec >= pointSecs) {
		const int score = carrySec / pointSecs;
		ent->client->pers.lastKillerDuckPoints += score * pointSecs * 1000;
		AddScore(ent, ent->r.currentOrigin, score, "killerduck");
		G_AddEvent(ent, EV_GENERAL_SOUND, G_SoundIndex("sound/items/killerducks/bite"));
	}
}

/**
 * This is called if a player collects the killerduck
 */
void G_BecomeKillerDuck(gentity_t *item, gentity_t *ent) {
	const int clientNum = ent - g_entities;
	item->wait = -1;
	item->nextthink = 0;
	item->think = NULL;

	// we only carry one killer duck in this game mode
	ent->client->ps.stats[STAT_HOLDABLEVAR] = 1;
	ent->client->pers.lastKillerDuckPoints = level.time;
	G_ForceClientWeapon(ent->client, WP_KILLERDUCKS, qtrue);
	ent->client->ps.eFlags |= EF_KILLERDUCK;
	ent->client->ps.stats[STAT_HEALTH] = ent->health = 200;
	ent->client->ps.stats[STAT_ARMOR] = 200;
	trap_SendServerCommand(-1, va("cp \"%s" S_COLOR_WHITE " has caught the KillerDuck.\n\"", ent->client->pers.netname));
	if (ent->client->ps.clientNum == clientNum) {
		trap_SendServerCommand(clientNum, va("cp \"%s" S_COLOR_WHITE, "You've caught the KillerDuck.\n\""));
	}
	trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " has caught the KillerDuck.\n\"", ent->client->pers.netname));

	// cache sound
	G_SoundIndex("sound/items/killerducks/bite");
}

/**
 * Returns true if the given entity is the killerduck carrier
 */
qboolean G_IsKillerDuck(const gentity_t *ent) {
	if (ent->client == NULL) {
		return qfalse;
	}
	return BG_IsKillerDuck(&ent->client->ps);
}
