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
#include "g_local.h"
#include "inv.h"

/*

  Items are any object that a player can touch to gain some effect.

  Pickup will return the number of seconds until they should respawn.

  all items should pop when dropped in lava or slime

  Respawnable items don't actually go away when picked up, they are
  just made invisible and untouchable.  This allows them to ride
  movers and respawn appropriately.
*/

#define RESPAWN_ARMOR 25
#define RESPAWN_HEALTH 35
#define RESPAWN_AMMO 40
#define RESPAWN_HOLDABLE 60
#define RESPAWN_MEGAHEALTH 35 // 120
#define RESPAWN_POWERUP 120

#define RESPAWN_DROPPED_FLAG 30
#define RESPAWN_DROPPED_ITEM 30

//======================================================================

static int Pickup_Powerup(gentity_t *ent, gentity_t *other) {
	int quantity;
	int i;
	qboolean laugh;
	gclient_t *client;

	if (!other->client->ps.powerups[ent->item->giTag]) {
		// round timing to seconds to make multiple powerup timers
		// count in sync
		other->client->ps.powerups[ent->item->giTag] = level.time - (level.time % 1000);
	}

	if (ent->count) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	other->client->ps.powerups[ent->item->giTag] += quantity * 1000;

	// look for other opponents nearby to laugh at them
	laugh = qfalse;
	for (i = 0; i < level.maxclients; i++) {
		vec3_t delta;
		float len;
		vec3_t forward;
		trace_t tr;

		// if already laughed at someone, no need to check further
		if (laugh != qfalse) {
			break;
		}

		client = &level.clients[i];
		if (client == other->client) {
			continue;
		}
		if (client->pers.connected == CON_DISCONNECTED) {
			continue;
		}
		if (client->ps.stats[STAT_HEALTH] <= 0) {
			continue;
		}

		// if same team in team game, no laughter
		// cannot use OnSameTeam as it expects to g_entities, not clients
		if (g_gametype.integer >= GT_TEAM && other->client->sess.sessionTeam == client->sess.sessionTeam) {
			continue;
		}

		// if too far away, no laughter
		VectorSubtract(ent->s.pos.trBase, client->ps.origin, delta);
		len = VectorNormalize(delta);
		if (len > 192) {
			continue;
		}

		// if not facing, no laughter
		AngleVectors(client->ps.viewangles, forward, NULL, NULL);
		if (DotProduct(delta, forward) < 0.4) {
			continue;
		}

		// if not line of sight, no laughter
		trap_Trace(&tr, client->ps.origin, NULL, NULL, ent->s.pos.trBase, ENTITYNUM_NONE, CONTENTS_SOLID);
		if (tr.fraction != 1.0) {
			continue;
		}

		laugh = qtrue;
	}

	// laugh at nearby opponents
	if (laugh) {
		G_AddEvent(other, EV_HEHE2, 0);
	}

	// FIXME: Where's the best place for this?
	if (ent->item->giTag == PW_BERSERKER) {
		G_ForceClientWeapon(ent->client, WP_PUNCHY, qfalse);
	}

	return RESPAWN_POWERUP;
}

//======================================================================

static int Pickup_Holdable(gentity_t *ent, gentity_t *other) {
	int count = 0;
	qboolean becomeKillerDuck;

	if (ent->count > 0) {
		count = ent->count;
	} else {
		// apply defaults
		switch (ent->item->giTag) {
		case HI_FLOATER:
			count = MAX_FLOATER;
			break;
		case HI_KILLERDUCKS:
			count = MAX_KILLERDUCKS;
			break;
		case HI_BOOMIES:
			count = MAX_BOOMIES;
			break;
		case HI_BAMBAM:
			count = MAX_BAMBAMS;
			break;
		default:
			count = 0; // FIXME: Return here?
			break;
		}
	}

	// already has this holdable, add to
	// NOTE: Basically BG_CanItemBeGrabbed() already ensures this
	if (bg_itemlist[other->client->ps.stats[STAT_HOLDABLE_ITEM]].giTag == ent->item->giTag) {
		count += other->client->ps.stats[STAT_HOLDABLEVAR];
	}
	// apply upper limits
	switch (ent->item->giTag) {
	case HI_FLOATER:
		if (count > MAX_FLOATER) {
			count = MAX_FLOATER;
		}
		break;
	case HI_KILLERDUCKS:
		if (count > MAX_KILLERDUCKS) {
			count = MAX_KILLERDUCKS;
		}
		break;
	case HI_BOOMIES:
		if (count > MAX_BOOMIES) {
			count = MAX_BOOMIES;
		}
		break;
	case HI_BAMBAM:
		if (count > MAX_BAMBAMS) {
			count = MAX_BAMBAMS;
		}
		break;
	default:
		count = 0; // FIXME: Return here?
		break;
	}

	becomeKillerDuck = g_gametype.integer == GT_CATCH && ent->item->giTag == HI_KILLERDUCKS;
	// is you are about the become the killerduck, and you already have a holdable, drop it in this game mode
	if (becomeKillerDuck && other->client->ps.stats[STAT_HOLDABLEVAR] > 0 && other->client->ps.stats[STAT_HOLDABLE_ITEM] != 0 &&
		other->client->ps.stats[STAT_HOLDABLE_ITEM] != MODELINDEX_KILLERDUCKS) {
		G_DropHoldable(other, 0.0f);
	}

	// FIXME: Check for NULLs?
	other->client->ps.stats[STAT_HOLDABLE_ITEM] = (ent->item - bg_itemlist);
	if (becomeKillerDuck) {
		G_BecomeKillerDuck(ent, other);
		// don't respawn the item
		return -1;
	}
	other->client->ps.stats[STAT_HOLDABLEVAR] = count;

	return RESPAWN_HOLDABLE;
}

//======================================================================

void Add_Ammo(gentity_t *ent, int weapon, int count) {
	ent->client->ps.ammo[weapon] += count;

	if ((weapon == WP_IMPERIUS) && (ent->client->ps.ammo[weapon] > MAXAMMO_IMPERIUS)) {
		ent->client->ps.ammo[weapon] = MAXAMMO_IMPERIUS;
	} else if (ent->client->ps.ammo[weapon] > MAXAMMO_WEAPON) {
		ent->client->ps.ammo[weapon] = MAXAMMO_WEAPON;
	}
}

static int Pickup_Ammo(gentity_t *ent, gentity_t *other) {
	int quantity;

	if (ent->count) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	if (ent->item->giTag == WP_SPRAYPISTOL) {
		if ((!strcmp(ent->item->classname, "ammo_spray_b") || !strcmp(ent->item->classname, "ammo_spray_r")) &&
			ent->s.otherEntityNum == other->s.number) {
			return 0; // leave the item in the world ...
		}

		if (!strcmp(ent->item->classname, "ammo_spray_n") && ent->s.otherEntityNum == other->s.number) {
			if ((level.time - other->client->lastWarningMessageTime) > 5000) {
				trap_SendServerCommand(other->s.clientNum, "cp \"You can't grab your own cartridge!\n\"");
				other->client->lastWarningMessageTime = level.time;
			}
			return 0; // leave the item in the world ...
		}

		if (((!strcmp(ent->item->classname, "ammo_spray_b") && other->client->sess.sessionTeam == TEAM_BLUE) ||
			 (!strcmp(ent->item->classname, "ammo_spray_r") && other->client->sess.sessionTeam == TEAM_RED) ||
			 !strcmp(ent->item->classname, "ammo_spray_n")) &&
			other->client->ps.ammo[WP_SPRAYPISTOL] >= MAX_CARTRIDGES) {
			if ((level.time - other->client->lastWarningMessageTime) > 5000) {
				trap_SendServerCommand(other->s.clientNum, "cp \"You can't grab more than 8 cartridges!\n\"");
				other->client->lastWarningMessageTime = level.time;
			}
			return 0; // leave the item in the world ...
		}

		if ((!strcmp(ent->item->classname, "ammo_spray_b") && other->client->sess.sessionTeam == TEAM_RED) ||
			(!strcmp(ent->item->classname, "ammo_spray_r") && other->client->sess.sessionTeam == TEAM_BLUE)) {
			return RESPAWN_AMMO; // remove item from world ... but no Add_Ammo
		}
	}

	Add_Ammo(other, ent->item->giTag, quantity);

	other->client->ps.generic1 = other->client->ps.ammo[WP_SPRAYPISTOL];

	return RESPAWN_AMMO;
}

//======================================================================

static int Pickup_Weapon(gentity_t *ent, gentity_t *other) {
	int quantity;

	if (ent->count < 0) {
		quantity = 0; // None for you, sir!
	} else {
		if (ent->count) {
			quantity = ent->count;
		} else {
			quantity = ent->item->quantity;
		}

		// dropped items and teamplay weapons always have full ammo
		if (!(ent->flags & FL_DROPPED_ITEM) && g_gametype.integer != GT_TEAM) {
			// respawning rules
			// drop the quantity if the already have over the minimum
			if (other->client->ps.ammo[ent->item->giTag] < quantity) {
				quantity = quantity - other->client->ps.ammo[ent->item->giTag];
			} else {
				quantity = 1; // only add a single shot
			}
		}
	}

	// add the weapon
	other->client->ps.stats[STAT_WEAPONS] |= (1 << ent->item->giTag);

	Add_Ammo(other, ent->item->giTag, quantity);

	if (ent->item->giTag == WP_GRAPPLING_HOOK)
		other->client->ps.ammo[ent->item->giTag] = -1; // unlimited ammo

	// team deathmatch has slow weapon respawns
	if (g_gametype.integer == GT_TEAM || g_gametype.integer == GT_FREEZETAG) {
		return g_weaponTeamRespawn.integer;
	}

	return g_weaponRespawn.integer;
}

//======================================================================

static int Pickup_Health(gentity_t *ent, gentity_t *other) {
	int max;
	int quantity;

	// small and mega healths will go over the max
	if (ent->item->quantity != 5 && ent->item->quantity != 100) {
		max = other->client->ps.stats[STAT_MAX_HEALTH];
	} else {
		max = other->client->ps.stats[STAT_MAX_HEALTH] * 2;
	}

	if (ent->count) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	other->health += quantity;

	if (other->health > max) {
		other->health = max;
	}
	other->client->ps.stats[STAT_HEALTH] = other->health;

	if (ent->item->quantity == 100) { // mega health respawns slow
		return RESPAWN_MEGAHEALTH;
	}

	return RESPAWN_HEALTH;
}

//======================================================================

static int Pickup_Armor(gentity_t *ent, gentity_t *other) {
	other->client->ps.stats[STAT_ARMOR] += ent->item->quantity;
	if (other->client->ps.stats[STAT_ARMOR] > other->client->ps.stats[STAT_MAX_HEALTH] * 2) {
		other->client->ps.stats[STAT_ARMOR] = other->client->ps.stats[STAT_MAX_HEALTH] * 2;
	}

	return RESPAWN_ARMOR;
}

//======================================================================

/*
===============
RespawnItem
===============
*/
void RespawnItem(gentity_t *ent) {
	// select from teamed entities
	if (ent->team) {
		gentity_t *master;
		int count;
		int choice;

		if (!ent->teammaster) {
			G_Error("RespawnItem: bad teammaster");
		}

		if (ent->spawnflags & 2) // randomly select from the group
		{
			master = ent->teammaster;

			for (count = 0, ent = master; ent; ent = ent->teamchain, count++)
				;

			choice = rand() % count;

			for (count = 0, ent = master; count < choice; ent = ent->teamchain, count++)
				;
		} else // loop through the group
		{
			if (ent->teamchain)
				ent = ent->teamchain;
			else
				ent = ent->teammaster;
		}
	}

	if (ent->team) {
		if (!ent->teammaster)
			G_Error("RespawnItem: bad teammaster");
	}

	ent->r.contents = CONTENTS_TRIGGER;
	ent->s.eFlags &= ~EF_NODRAW;
	ent->r.svFlags &= ~SVF_NOCLIENT;
	trap_LinkEntity(ent);

	if (ent->item->giType == IT_POWERUP) {
		// play powerup spawn sound to all clients
		gentity_t *te;

		// if the powerup respawn sound should Not be global
		if (ent->speed) {
			te = G_TempEntity(ent->s.pos.trBase, EV_GENERAL_SOUND);
		} else {
			te = G_TempEntity(ent->s.pos.trBase, EV_GLOBAL_SOUND);
		}
		te->s.eventParm = G_SoundIndex("sound/items/powerup/respawn");
		te->r.svFlags |= SVF_BROADCAST;
	}

	// play the normal respawn sound only to nearby clients
	G_AddEvent(ent, EV_ITEM_RESPAWN, 0);

	ent->nextthink = 0;
}

/*
===============
Touch_Item
===============
*/
void Touch_Item(gentity_t *ent, gentity_t *other, trace_t *trace) {
	int respawn;
	qboolean predict;
	pickupresult_t result;

	if (!other->client)
		return;
	if (other->health < 1)
		return; // dead people can't pickup

	if (G_FreezeTag() && FT_PlayerIsFrozen(other))
		return; // frozen people don't pick stuff up

	// the same pickup rules are used for client side and server side
	result = BG_CanItemBeGrabbed(g_gametype.integer, &ent->s, &other->client->ps, level.time);
	if (result != PICKUP_OK) {
		if (result == PICKUP_KILLERDUCKS_NOT_YET_AGAIN) {
			if ((level.time - other->client->lastWarningMessageTime) > 2000) {
				trap_SendServerCommand(other->s.clientNum, "cp \"You can't pick up the KillerDuck for a while!\n\"");
				other->client->lastWarningMessageTime = level.time + 2000;
			}
		}
		return;
	}

	G_LogPrintf("Item: %i %s\n", other->s.number, ent->item->classname);

	predict = other->client->pers.predictItemPickup;

	// call the item-specific pickup function
	switch (ent->item->giType) {
	case IT_WEAPON:
		respawn = Pickup_Weapon(ent, other);
		//		predict = qfalse;
		break;
	case IT_AMMO:
		respawn = Pickup_Ammo(ent, other);
		//		predict = qfalse;
		break;
	case IT_ARMOR:
		respawn = Pickup_Armor(ent, other);
		break;
	case IT_HEALTH:
		respawn = Pickup_Health(ent, other);
		break;
	case IT_POWERUP:
		respawn = Pickup_Powerup(ent, other);
		//		predict = qfalse;
		break;
	case IT_TEAM:
		respawn = Pickup_Team(ent, other);
		break;
	case IT_HOLDABLE:
		respawn = Pickup_Holdable(ent, other);
		break;
	default:
		return;
	}

	if (!respawn) {
		return;
	}

	// play the normal pickup sound
	if (predict) {
		G_AddPredictableEvent(other, EV_ITEM_PICKUP, ent->s.modelindex);
	} else {
		G_AddEvent(other, EV_ITEM_PICKUP, ent->s.modelindex);
	}

	// powerup pickups are global broadcasts
	if (ent->item->giType == IT_POWERUP || ent->item->giType == IT_TEAM) {
		// if we want the global sound to play
		if (!ent->speed) {
			gentity_t *te;

			te = G_TempEntity(ent->s.pos.trBase, EV_GLOBAL_ITEM_PICKUP);
			te->s.eventParm = ent->s.modelindex;
			te->r.svFlags |= SVF_BROADCAST;
		} else {
			gentity_t *te;

			te = G_TempEntity(ent->s.pos.trBase, EV_GLOBAL_ITEM_PICKUP);
			te->s.eventParm = ent->s.modelindex;
			// only send this temp entity to a single client
			te->r.svFlags |= SVF_SINGLECLIENT;
			te->r.singleClient = other->s.number;
		}
	}

	// fire item targets
	G_UseTargets(ent, other);

	// wait of -1 will not respawn
	if (ent->wait == -1) {
		ent->r.svFlags |= SVF_NOCLIENT;
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		ent->unlinkAfterEvent = qtrue;
		return;
	}

	// non zero wait overrides respawn time
	if (ent->wait) {
		respawn = ent->wait;
	}

	// random can be used to vary the respawn time
	if (ent->random) {
		respawn += crandom() * ent->random;
		if (respawn < 1) {
			respawn = 1;
		}
	}

	// dropped items will not respawn
	if (ent->flags & FL_DROPPED_ITEM) {
		ent->freeAfterEvent = qtrue;
	}

	// picked up items still stay around, they just don't
	// draw anything.  This allows respawnable items
	// to be placed on movers.
	ent->r.svFlags |= SVF_NOCLIENT;
	ent->s.eFlags |= EF_NODRAW;
	ent->r.contents = 0;

	// ZOID
	// A negative respawn times means to never respawn this item (but don't
	// delete it).  This is used by items that are respawned by third party
	// events such as ctf flags
	if (respawn <= 0) {
		ent->nextthink = 0;
		ent->think = 0;
	} else {
		ent->nextthink = level.time + respawn * 1000;
		ent->think = RespawnItem;
	}
	trap_LinkEntity(ent);
}

//======================================================================

/*
================
LaunchItem

Spawns an item and tosses it forward
================
*/
gentity_t *LaunchItem(const gitem_t *item, const vec3_t origin, const vec3_t velocity) {
	gentity_t *dropped;

	if (item == NULL) {
		return NULL;
	}

	dropped = G_Spawn();

	dropped->s.eType = ET_ITEM;
	dropped->s.modelindex = item - bg_itemlist; // store item number in modelindex
	dropped->s.modelindex2 = 1;					// This is non-zero is it's a dropped item

	dropped->classname = item->classname;
	dropped->item = item;
	VectorSet(dropped->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS);
	VectorSet(dropped->r.maxs, ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS);
	dropped->r.contents = CONTENTS_TRIGGER;

	dropped->touch = Touch_Item;

	G_SetOrigin(dropped, origin);
	dropped->s.pos.trType = TR_GRAVITY;
	dropped->s.pos.trTime = level.time;
	VectorCopy(velocity, dropped->s.pos.trDelta);

	dropped->s.eFlags |= EF_BOUNCE_HALF;
	// Special case for CTF flags
	if ((g_gametype.integer == GT_CTF || g_gametype.integer == GT_1FCTF) && (item->giType == IT_TEAM)) {
		dropped->think = Team_DroppedFlagThink;
		dropped->nextthink = (level.time + RESPAWN_DROPPED_FLAG * 1000);
		Team_CheckDroppedItem(dropped);
	} else {
		// auto-remove after timeout
		dropped->think = G_FreeEntity;
		dropped->nextthink = (level.time + RESPAWN_DROPPED_ITEM * 1000);
	}

	dropped->flags = FL_DROPPED_ITEM;

	trap_LinkEntity(dropped);

	return dropped;
}

/*
================
Drop_Item

Spawns an item and tosses it forward
================
*/
gentity_t *Drop_Item(gentity_t *ent, const gitem_t *item, float angle) {
	if (item->giType == IT_HOLDABLE && item->giTag == HI_KILLERDUCKS && g_gametype.integer == GT_CATCH) {
		return G_DropKillerDucks(ent);
	} else {
		vec3_t velocity;
		vec3_t angles;

		VectorCopy(ent->s.apos.trBase, angles);
		angles[YAW] += angle;
		angles[PITCH] = 0; // always forward

		AngleVectors(angles, velocity, NULL, NULL);
		VectorScale(velocity, 150, velocity);
		velocity[2] += 200 + crandom() * 50;

		// FIXME: Cartridges call LaunchItem() directly
		G_LogPrintf("DropItem: %ld %s\n", (ent - g_entities), item->classname);

		return LaunchItem(item, ent->s.pos.trBase, velocity);
	}
}

/*
================
Use_Item

Respawn the item
================
*/
static void Use_Item(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	RespawnItem(ent);
}

//======================================================================

/*
================
FinishSpawningItem

Traces down to find where an item should rest, instead of letting them
free fall from their spawn points
================
*/
void FinishSpawningItem(gentity_t *ent) {
	trace_t tr;
	vec3_t dest;

	VectorSet(ent->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS);
	VectorSet(ent->r.maxs, ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS);

	ent->s.eType = ET_ITEM;
	ent->s.modelindex = ent->item - bg_itemlist; // store item number in modelindex
	ent->s.modelindex2 = 0;						 // zero indicates this isn't a dropped item
	ent->r.contents = CONTENTS_TRIGGER;
	ent->touch = Touch_Item;
	// using an item causes it to respawn
	ent->use = Use_Item;

	if (ent->spawnflags & 1) {
		// suspended
		G_SetOrigin(ent, ent->s.origin);
	} else {
		// drop to floor
		VectorSet(dest, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] - 4096);
		trap_Trace(&tr, ent->s.origin, ent->r.mins, ent->r.maxs, dest, ent->s.number, MASK_SOLID);
		if (tr.startsolid) {
			Com_Printf("FinishSpawningItem: %s startsolid at %s\n", ent->classname, vtos(ent->s.origin));
			G_FreeEntity(ent);
			return;
		}

		// allow to ride movers
		ent->s.groundEntityNum = tr.entityNum;

		G_SetOrigin(ent, tr.endpos);
	}

	// team slaves and targeted items aren't present at start
	if ((ent->flags & FL_TEAMSLAVE) || ent->targetname) {
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		return;
	}

	// powerups don't spawn in for a while
	if (ent->item->giType == IT_POWERUP) {
		float respawn;

		respawn = 45 + crandom() * 15;
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		ent->nextthink = level.time + respawn * 1000;
		ent->think = RespawnItem;
		return;
	}

	if (ent->item->giType == IT_HOLDABLE) {
		if ((ent->item->giTag == HI_BAMBAM) && (g_gametype.integer != GT_CTF) && (g_gametype.integer != GT_1FCTF)) {
			return;
		} else if ((ent->item->giTag == HI_BOOMIES) &&
				   ((g_gametype.integer != GT_CTF) && (g_gametype.integer != GT_1FCTF) && (g_gametype.integer != GT_BALLOON))) {
			return;
		}
	}

	trap_LinkEntity(ent);
}

static qboolean itemRegistered[MAX_ITEMS];
int g_itemCount[MAX_ITEMS];

/*
==================
G_CheckTeamItems
==================
*/
void G_CheckTeamItems(void) {
	// Set up team stuff
	Team_InitGame();

	if (g_gametype.integer == GT_CTF) {
		const gitem_t *item;

		// check for the two flags
		item = BG_FindItem("Red Lolly");
		if (!item || !itemRegistered[item - bg_itemlist]) {
			Com_Printf(S_COLOR_YELLOW "WARNING: No team_CTL_redlolly in map");
		}
		item = BG_FindItem("Blue Lolly");
		if (!item || !itemRegistered[item - bg_itemlist]) {
			Com_Printf(S_COLOR_YELLOW "WARNING: No team_CTL_bluelolly in map");
		}
	}

	if (g_gametype.integer == GT_1FCTF) {
		const gitem_t *item;

		// check for all three flags
		item = BG_FindItem("Red Lolly");
		if (!item || !itemRegistered[item - bg_itemlist]) {
			Com_Printf(S_COLOR_YELLOW "WARNING: No team_CTL_redlolly in map");
		}
		item = BG_FindItem("Blue Lolly");
		if (!item || !itemRegistered[item - bg_itemlist]) {
			Com_Printf(S_COLOR_YELLOW "WARNING: No team_CTL_bluelolly in map");
		}
		item = BG_FindItem("Neutral Lolly");
		if (!item || !itemRegistered[item - bg_itemlist]) {
			Com_Printf(S_COLOR_YELLOW "WARNING: No team_CTL_neutrallolly in map");
		}
	}
}

/*
==============
ClearRegisteredItems
==============
*/
void ClearRegisteredItems(void) {
	memset(itemRegistered, 0, sizeof(itemRegistered));
	memset(g_itemCount, 0, sizeof(g_itemCount));

	// players always start with the base weapon
	RegisterItem(BG_FindItemForWeapon(WP_NIPPER));
	RegisterItem(BG_FindItemForWeapon(WP_PUNCHY));

	RegisterItem(BG_FindItemForWeapon(WP_KILLERDUCKS));

	if (IsSyc()) {
		RegisterItem(BG_FindItemForWeapon(WP_SPRAYPISTOL));
	}
}

/*
===============
RegisterItem

The item will be added to the precache list
===============
*/
void RegisterItem(const gitem_t *item) {
	if (!item) {
		G_Error("RegisterItem: NULL");
	}
	itemRegistered[item - bg_itemlist] = qtrue;
}

/*
===============
SaveRegisteredItems

Write the needed items to a config string
so the client will know which ones to precache
===============
*/
void SaveRegisteredItems(void) {
	char string[MAX_ITEMS + 1];
	int i;
	int count;

	count = 0;
	for (i = 0; i < bg_numItems; i++) {
		if (itemRegistered[i]) {
			count++;
			string[i] = '1';
		} else {
			string[i] = '0';
		}
	}
	string[bg_numItems] = 0;

	Com_Printf("%i items registered\n", count);
	trap_SetConfigstring(CS_ITEMS, string);
}

/*
============
G_ItemDisabled
============
*/
static int G_ItemDisabled(const gitem_t *item) {
	char name[128];

	// never disable the killerduck in catch the killerduck mode
	if (item->giTag == HI_KILLERDUCKS && g_gametype.integer == GT_CATCH) {
		return 0;
	}

	Com_sprintf(name, sizeof(name), "disable_%s", item->classname);
	return trap_Cvar_VariableIntegerValue(name);
}

/*
============
G_SpawnItem

Sets the clipping size and plants the object on the floor.

Items can't be immediately dropped to floor, because they might
be on an entity that hasn't spawned yet.
============
*/
qboolean G_SpawnItem(gentity_t *ent, const gitem_t *item) {
	G_SpawnFloat("random", "0", &ent->random);
	G_SpawnFloat("wait", "0", &ent->wait);

	RegisterItem(item);
	if (G_ItemDisabled(item))
		return qfalse;

	ent->item = item;
	// some movers spawn on the second frame, so delay item
	// spawns until the third frame so they can ride trains
	ent->nextthink = level.time + FRAMETIME * 2;
	ent->think = FinishSpawningItem;

	ent->physicsBounce = 0.5f; // items are bouncy

	if (item->giType == IT_POWERUP) {
		G_SoundIndex("sound/items/powerup/respawn");
		G_SpawnFloat("noglobalsound", "0", &ent->speed);
	}
	return qtrue;
}

/*
================
G_BounceItem
================
*/
static void G_BounceItem(gentity_t *ent, trace_t *trace) {
	vec3_t velocity;
	float dot;
	int hitTime;

	// reflect the velocity on the trace plane
	hitTime = level.previousTime + (level.time - level.previousTime) * trace->fraction;
	BG_EvaluateTrajectoryDelta(&ent->s.pos, hitTime, velocity);
	dot = DotProduct(velocity, trace->plane.normal);
	VectorMA(velocity, -2 * dot, trace->plane.normal, ent->s.pos.trDelta);

	// cut the velocity to keep from bouncing forever
	VectorScale(ent->s.pos.trDelta, ent->physicsBounce, ent->s.pos.trDelta);

	// check for stop
	if (trace->plane.normal[2] > 0 && ent->s.pos.trDelta[2] < 40) {
		trace->endpos[2] += 1.0; // make sure it is off ground
		SnapVector(trace->endpos);
		G_SetOrigin(ent, trace->endpos);
		ent->s.groundEntityNum = trace->entityNum;
		return;
	}

	VectorAdd(ent->r.currentOrigin, trace->plane.normal, ent->r.currentOrigin);
	VectorCopy(ent->r.currentOrigin, ent->s.pos.trBase);
	ent->s.pos.trTime = level.time;
}

/*
================
G_RunItem
================
*/
void G_RunItem(gentity_t *ent) {
	vec3_t origin;
	trace_t tr;
	int contents;
	int mask;

	// if its groundentity has been set to none, it may have been pushed off an edge
	if (ent->s.groundEntityNum == ENTITYNUM_NONE) {
		if (ent->s.pos.trType != TR_GRAVITY) {
			ent->s.pos.trType = TR_GRAVITY;
			ent->s.pos.trTime = level.time;
		}
	}

	if (ent->s.pos.trType == TR_STATIONARY) {
		// check think function
		G_RunThink(ent);
		return;
	}

	// get current position
	BG_EvaluateTrajectory(&ent->s.pos, level.time, origin);

	// trace a line from the previous position to the current position
	if (ent->clipmask) {
		mask = ent->clipmask;
	} else {
		mask = MASK_PLAYERSOLID & ~CONTENTS_BODY; // MASK_SOLID;
	}
	trap_Trace(&tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin, ent->r.ownerNum, mask);

	VectorCopy(tr.endpos, ent->r.currentOrigin);

	if (tr.startsolid) {
		tr.fraction = 0;
	}

	trap_LinkEntity(ent); // FIXME: avoid this for stationary?

	// check think function
	G_RunThink(ent);

	if (tr.fraction == 1) {
		return;
	}

	// if it is in a nodrop volume, remove it
	contents = trap_PointContents(ent->r.currentOrigin, -1);
	if (contents & CONTENTS_NODROP) {
		if (g_gametype.integer == GT_CATCH && ent->item->giTag == HI_KILLERDUCKS) {
			G_RespawnKillerDucks(ent);
		}
		if (ent->item && ent->item->giType == IT_TEAM) {
			Team_FreeEntity(ent);
		} else {
			G_FreeEntity(ent);
		}
		return;
	}

	G_BounceItem(ent, &tr);
}

void G_ForceClientWeapon(gclient_t *client, int weapon, qboolean addToInventory) {
	const int clientNum = client - level.clients;
	if (client->ps.weapon != weapon) {
		client->last_nonforced_weapon = client->ps.weapon;
	}
	// we don't want to check the client side of the inventory because the snapshot is sent to late
	trap_SendServerCommand(clientNum, va("fwc %i %i", weapon, addToInventory ? 0 : 1));
	if (addToInventory) {
		client->ps.stats[STAT_WEAPONS] |= (1 << weapon);
	}
	client->pers.cmd.weapon = weapon;
	client->ps.weapon = weapon;
}

void G_RestoreClientLastWeapon(gclient_t *client, qboolean removeFromInventory) {
	const int clientNum = client - level.clients;
	const int weapon = client->last_nonforced_weapon;
	trap_SendServerCommand(clientNum, va("fwc %i 1", weapon));
	if (removeFromInventory) {
		client->ps.stats[STAT_WEAPONS] &= ~(1 << client->ps.weapon);
	}
	client->pers.cmd.weapon = weapon;
	client->ps.weapon = weapon;
	client->last_nonforced_weapon = 0;
}
