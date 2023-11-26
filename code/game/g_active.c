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

*/

#include "g_local.h"

/*
===============
G_DamageFeedback

Called just before a snapshot is sent to the given player.
Totals up all damage and generates both the player_state_t
damage values to that client for pain blends and kicks, and
global pain sound events for all clients.
===============
*/
static void P_DamageFeedback(gentity_t *player) {
	gclient_t *client;
	float count;
	vec3_t angles;

	client = player->client;
	if (client->ps.pm_type == PM_DEAD) {
		return;
	}

	// total points of damage shot at the player this frame
	count = client->damage_blood + client->damage_armor;
	if (count == 0) {
		return; // didn't take any damage
	}

	if (count > 255) {
		count = 255;
	}

	// send the information to the client

	// world damage (falling, slime, etc) uses a special code
	// to make the blend blob centered instead of positional
	if (client->damage_fromWorld) {
		client->ps.damagePitch = 255;
		client->ps.damageYaw = 255;

		client->damage_fromWorld = qfalse;
	} else {
		vectoangles(client->damage_from, angles);
		client->ps.damagePitch = angles[PITCH] / 360.0 * 256;
		client->ps.damageYaw = angles[YAW] / 360.0 * 256;
	}

	// play an appropriate pain sound
	if ((level.time > player->pain_debounce_time) && !(player->flags & FL_GODMODE)) {
		player->pain_debounce_time = level.time + 700;
		G_AddEvent(player, EV_PAIN, player->health);
		client->ps.damageEvent++;
	}

	client->ps.damageCount = count;

	//
	// clear totals
	//
	client->damage_blood = 0;
	client->damage_armor = 0;
	client->damage_knockback = 0;
}

/*
=============
P_WorldEffects

Check for lava / slime contents and drowning
=============
*/
static void P_WorldEffects(gentity_t *ent) {
	qboolean envirosuit;
	waterLevel_t waterlevel;

	if (ent->client->noclip) {
		ent->client->airOutTime = level.time + 12000; // don't need air
		return;
	}

	waterlevel = ent->waterlevel;

	envirosuit = ent->client->ps.powerups[PW_PADPOWER] > level.time;

	//
	// check for drowning
	//
	if (waterlevel == WL_DIVING) {
		// envirosuit give air
		if (envirosuit) {
			ent->client->airOutTime = level.time + 10000;
		}

		// if out of air, start drowning
		if (ent->client->airOutTime < level.time) {
			// drown!
			ent->client->airOutTime += 1000;
			if (ent->health > 0) {
				// take more damage the longer underwater
				ent->damage += 2;
				if (ent->damage > 15)
					ent->damage = 15;

				// don't play a normal pain sound
				ent->pain_debounce_time = level.time + 200;

				G_Damage(ent, NULL, NULL, NULL, NULL, ent->damage, DAMAGE_NO_ARMOR, MOD_WATER);
			}
		}
	} else {
		ent->client->airOutTime = level.time + 12000;
		ent->damage = 2;
	}

	//
	// check for sizzle damage (move to pmove?)
	//
	if (waterlevel != WL_NOT && (ent->watertype & (CONTENTS_LAVA | CONTENTS_SLIME))) {
		if (ent->health > 0 && ent->pain_debounce_time <= level.time) {

			if (envirosuit) {
				G_AddEvent(ent, EV_POWERUP_PADPOWER, 0);
			} else {
				if (ent->watertype & CONTENTS_LAVA) {
					G_Damage(ent, NULL, NULL, NULL, NULL, 30 * waterlevel, 0, MOD_LAVA);
				}

				if (ent->watertype & CONTENTS_SLIME) {
					G_Damage(ent, NULL, NULL, NULL, NULL, 10 * waterlevel, 0, MOD_SLIME);
				}
			}
		}
	}
}

/*
===============
G_SetClientSound
===============
*/
static void G_SetClientSound(gentity_t *ent) {
	if (ent->waterlevel != WL_NOT && (ent->watertype & (CONTENTS_LAVA | CONTENTS_SLIME))) {
		ent->client->ps.loopSound = level.snd_fry;
	} else {
		ent->client->ps.loopSound = 0;
	}
}

//==============================================================

/*
==============
ClientImpacts
==============
*/
static void ClientImpacts(gentity_t *ent, pmove_t *pm) {
	int i, j;
	trace_t trace;
	gentity_t *other;

	memset(&trace, 0, sizeof(trace));
	for (i = 0; i < pm->numtouch; i++) {
		for (j = 0; j < i; j++) {
			if (pm->touchents[j] == pm->touchents[i]) {
				break;
			}
		}
		if (j != i) {
			continue; // duplicated
		}
		other = &g_entities[pm->touchents[i]];

		if ((ent->r.svFlags & SVF_BOT) && (ent->touch)) {
			ent->touch(ent, other, &trace);
		}

		if (!other->touch) {
			continue;
		}

		other->touch(other, ent, &trace);
	}
}

/*
============
G_TouchTriggers

Find all trigger entities that ent's current position touches.
Spectators will only interact with teleporters.
============
*/
void G_TouchTriggers(gentity_t *ent) {
	int i, num;
	int touch[MAX_GENTITIES];
	gentity_t *hit;
	trace_t trace;
	vec3_t mins, maxs;
	static vec3_t range = {40, 40, 52};

	if (!ent->client) {
		return;
	}

	// dead clients don't activate triggers!
	if (ent->client->ps.stats[STAT_HEALTH] <= 0) {
		return;
	}

	VectorSubtract(ent->client->ps.origin, range, mins);
	VectorAdd(ent->client->ps.origin, range, maxs);

	// check if we lagged through a tele-trigger (for the tele-tubes of the syc-room)
	{
		vec3_t tmpv3;
		float v;
		trace_t tr;

		tmpv3[0] = ent->client->ps.origin[0] - ent->client->oldOrigin[0];
		tmpv3[1] = ent->client->ps.origin[1] - ent->client->oldOrigin[1];
		tmpv3[2] = ent->client->ps.origin[2] - ent->client->oldOrigin[2];
		v = VectorLengthSquared(tmpv3);
		// the touchbox checks 80 units ... so we'll need to check the rest ;)
		if (v >= 80.0f * 80.0f) {
			trap_Trace(&tr, ent->client->oldOrigin, ent->r.mins, ent->r.maxs, ent->client->ps.origin, ENTITYNUM_WORLD,
					   (int)0xFFFFFFFF);
			// ToDo?: checking other triggers?
			if (tr.fraction < 1.0f && g_entities[tr.entityNum].s.eType == ET_TELEPORT_TRIGGER) {
				memset(&trace, 0, sizeof(trace));
				if (g_entities[tr.entityNum].touch) {
					g_entities[tr.entityNum].touch(&g_entities[tr.entityNum], ent, &trace);
				}
				if ((ent->r.svFlags & SVF_BOT) && (ent->touch)) {
					ent->touch(ent, &g_entities[tr.entityNum], &trace);
				}

				// vv copy from bellow ... don't know if we realy need it ;P vv
				if (ent->client->ps.jumppad_frame != ent->client->ps.pmove_framecount) {
					ent->client->ps.jumppad_frame = 0;
					ent->client->ps.jumppad_ent = 0;
				}
				return;
			}
		}
	}

	num = trap_EntitiesInBox(mins, maxs, touch, MAX_GENTITIES);

	// can't use ent->absmin, because that has a one unit pad
	VectorAdd(ent->client->ps.origin, ent->r.mins, mins);
	VectorAdd(ent->client->ps.origin, ent->r.maxs, maxs);

	for (i = 0; i < num; i++) {
		hit = &g_entities[touch[i]];

		if (!hit->touch && !ent->touch) {
			continue;
		}
		if (!(hit->r.contents & CONTENTS_TRIGGER)) {
			continue;
		}

		// ignore most entities if a spectator
		if ((ent->client->sess.sessionTeam == TEAM_SPECTATOR) || LPSDeadSpec(ent->client)) {
			if (hit->s.eType != ET_TELEPORT_TRIGGER &&
				// this is ugly but adding a new ET_? type will
				// most likely cause network incompatibilities
				hit->touch != Touch_DoorTrigger) {
				continue;
			}
		}

		// use separate code for determining if an item is picked up
		// so you don't have to actually contact its bounding box
		if (hit->s.eType == ET_ITEM) {
			if (!BG_PlayerTouchesItem(&ent->client->ps, &hit->s, level.time)) {
				continue;
			}
		} else {
			if (!trap_EntityContact(mins, maxs, hit)) {
				continue;
			}
		}

		memset(&trace, 0, sizeof(trace));

		if (hit->touch) {
			hit->touch(hit, ent, &trace);
		}

		if ((ent->r.svFlags & SVF_BOT) && (ent->touch)) {
			ent->touch(ent, hit, &trace);
		}
	}

	// if we didn't touch a jump pad this pmove frame
	if (ent->client->ps.jumppad_frame != ent->client->ps.pmove_framecount) {
		ent->client->ps.jumppad_frame = 0;
		ent->client->ps.jumppad_ent = 0;
	}
}

/*
=================
SpectatorThink
=================
*/
static void SpectatorThink(gentity_t *ent, usercmd_t *ucmd) {
	pmove_t pm;
	gclient_t *client;

	client = ent->client;

	if (client->sess.spectatorState != SPECTATOR_FOLLOW) {
		if (client->noclip) {
			client->ps.pm_type = PM_NOCLIP;
		} else {
			client->ps.pm_type = PM_SPECTATOR;
		}
		client->ps.speed = 400; // faster than normal

		// set up for pmove
		memset(&pm, 0, sizeof(pm));
		pm.ps = &client->ps;
		pm.cmd = *ucmd;
		pm.tracemask = MASK_PLAYERSOLID & ~CONTENTS_BODY; // spectators can fly through bodies
		pm.trace = trap_Trace;
		pm.pointcontents = trap_PointContents;
		pm.gametype = g_gametype.integer;

		// perform a pmove
		Pmove(&pm);
		// save results of pmove
		VectorCopy(client->ps.origin, ent->s.origin);

		G_TouchTriggers(ent);
		trap_UnlinkEntity(ent);
	}

	client->oldbuttons = client->buttons;
	client->buttons = ucmd->buttons;

	if (level.time - client->lastDeathTime < 10000)
		return; // verhindern das man bei lps zu schnell in einen follow landet

	// attack button cycles through spectators
	if ((client->buttons & BUTTON_ATTACK) && !(client->oldbuttons & BUTTON_ATTACK)) {
		Cmd_FollowCycle_f(ent, 1);
	}

	if ((client->buttons & BUTTON_USE_HOLDABLE) && !(client->oldbuttons & BUTTON_USE_HOLDABLE))
		StopFollowing(ent);
}

/*
=================
ClientInactivityTimer

Returns qfalse if the client is dropped
=================
*/
static qboolean ClientInactivityTimer(gclient_t *client) {
	if (!g_inactivity.integer) {
		// give everyone some time, so if the operator sets g_inactivity during
		// gameplay, everyone isn't kicked
		client->inactivityTime = level.time + 60 * 1000;
		client->inactivityWarning = qfalse;
	} else if (client->pers.cmd.forwardmove || client->pers.cmd.rightmove || client->pers.cmd.upmove ||
			   (client->pers.cmd.buttons & BUTTON_ATTACK)) {
		client->inactivityTime = level.time + g_inactivity.integer * 1000;
		client->inactivityWarning = qfalse;
	} else if (!client->pers.localClient) {
		if (level.time > client->inactivityTime) {
			trap_DropClient(client - level.clients, "Dropped due to inactivity");
			return qfalse;
		}
		if (level.time > client->inactivityTime - 10000 && !client->inactivityWarning) {
			client->inactivityWarning = qtrue;
			trap_SendServerCommand(client - level.clients, "cp \"Ten seconds until inactivity drop!\n\"");
		}
	}
	return qtrue;
}

/*
==================
ClientTimerActions

Actions that happen once a second
==================
*/
static void ClientTimerActions(gentity_t *ent, int msec) {
	gclient_t *client;

	client = ent->client;
	client->timeResidual += msec;

	while (client->timeResidual >= 1000) {
		client->timeResidual -= 1000;

		if (client->ps.powerups[PW_REVIVAL]) {
			if (ent->health < client->ps.stats[STAT_MAX_HEALTH]) {
				ent->health += 15;
				if (ent->health > client->ps.stats[STAT_MAX_HEALTH] * 1.1) {
					ent->health = client->ps.stats[STAT_MAX_HEALTH] * 1.1;
				}
				G_AddEvent(ent, EV_POWERUP_REVIVAL, 0);
			} else if (ent->health < client->ps.stats[STAT_MAX_HEALTH] * 2) {
				ent->health += 5;
				if (ent->health > client->ps.stats[STAT_MAX_HEALTH] * 2) {
					ent->health = client->ps.stats[STAT_MAX_HEALTH] * 2;
				}
				G_AddEvent(ent, EV_POWERUP_REVIVAL, 0);
			}
		} else {
			if (ent->health > client->ps.stats[STAT_MAX_HEALTH]) {
				ent->health--;
			}
		}

		// count down armor when over max
		if (client->ps.stats[STAT_ARMOR] > client->ps.stats[STAT_MAX_HEALTH]) {
			client->ps.stats[STAT_ARMOR]--;
		}
	}
}

/*
====================
ClientIntermissionThink
====================
*/
static void ClientIntermissionThink(gclient_t *client) {
	client->ps.eFlags &= ~EF_TALK;
	client->ps.eFlags &= ~EF_FIRING;

	// the level will exit when everyone wants to or after timeouts

	// swap and latch button actions
	client->oldbuttons = client->buttons;
	client->buttons = client->pers.cmd.buttons;
	if (client->buttons & (BUTTON_ATTACK | BUTTON_USE_HOLDABLE) & (client->oldbuttons ^ client->buttons)) {
		// this used to be an ^1 but once a player says ready, it should stick
		client->readyToExit = 1;
	}
}

/*
================
ClientEvents

Events will be passed on to the clients for presentation,
but any server game effects are handled here
================
*/
static void ClientEvents(gentity_t *ent, int oldEventSequence) {
	int i, j;
	int event;
	gclient_t *client;
	//	int		damage;
	//	vec3_t	dir;
	vec3_t origin, angles;
	//	qboolean	fired;
	gentity_t *drop;

	client = ent->client;

	if (oldEventSequence < client->ps.eventSequence - MAX_PS_EVENTS) {
		oldEventSequence = client->ps.eventSequence - MAX_PS_EVENTS;
	}
	for (i = oldEventSequence; i < client->ps.eventSequence; i++) {
		event = client->ps.events[i & (MAX_PS_EVENTS - 1)];

		switch (event) {
		case EV_FALL_MEDIUM:
		case EV_FALL_FAR: // no fall dmg in wop at all
			break;

			/*			if ( ent->s.eType != ET_PLAYER ) {
							break;		// not in the player model
						}
						if ( g_dmflags.integer & DF_NO_FALLING ) {
							break;
						}
						if ( event == EV_FALL_FAR ) {
							damage = 10;
						} else {
							damage = 5;
						}
						ent->pain_debounce_time = level.time + 200;	// no normal pain sound
						G_Damage (ent, NULL, NULL, NULL, NULL, damage, 0, MOD_FALLING);
						break;
			*/
		case EV_FIRE_WEAPON:
			G_FireWeapon(ent);
			break;

		case EV_IMPERIUS_EXPLODE:
			G_ImperiusExplode(ent);
			break;

		case EV_USE_ITEM1: { // teleporter
			// drop flags in CTF
			const gitem_t *item = NULL;
			j = 0;

			if (client->ps.powerups[PW_REDFLAG]) {
				item = BG_FindItemForPowerup(PW_REDFLAG);
				j = PW_REDFLAG;
			} else if (client->ps.powerups[PW_BLUEFLAG]) {
				item = BG_FindItemForPowerup(PW_BLUEFLAG);
				j = PW_BLUEFLAG;
			} else if (ent->client->ps.powerups[PW_NEUTRALFLAG]) {
				item = BG_FindItemForPowerup(PW_NEUTRALFLAG);
				j = PW_NEUTRALFLAG;
			}

			if (item) {
				drop = Drop_Item(ent, item, 0);
				// decide how many seconds it has left
				drop->count = (client->ps.powerups[j] - level.time) / 1000;
				if (drop->count < 1) {
					drop->count = 1;
				}

				client->ps.powerups[j] = 0;
			}

			SelectSpawnPoint(client->ps.origin, origin, angles, qfalse);
			TeleportPlayer(ent, origin, angles);
			break;
		}

		case EV_USE_ITEM2: // medkit
			ent->health = client->ps.stats[STAT_MAX_HEALTH] + 25;

			break;

		case EV_USE_ITEM3:																		  // HI_FLOATER
			if (client->ps.velocity[2] < 512.0f)											  //<256
				client->ps.velocity[2] += 900.0f * (1.0f / (float)(G_GetCvarInt("sv_fps"))); // noch zu testen

			break;

		case EV_USE_ITEM4: { // HI_KILLERDUCKS
			vec3_t forward, right, up, muzzle;

			AngleVectors(client->ps.viewangles, forward, right, up);

			CalcMuzzlePoint(ent, forward, right, up, muzzle);
			fire_killerducks(ent, muzzle, forward);
			break;
		}
		// HI_BAMBAM
		case EV_USE_ITEM5: {
			const gitem_t *item = BG_FindItemForHoldable(HI_BAMBAM);
			if (item) {
				if (client->ps.stats[STAT_FORBIDDENITEMS] & (1 << HI_BAMBAM)) {
					trap_SendServerCommand((ent - g_entities), va("cp \"%s not allowed here\"", item->pickup_name));
				} else {
					if (bambam_createByPlayer(ent, item->pickup_name)) {
						client->ps.stats[STAT_HOLDABLEVAR] = 0;
						client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
					}
				}
			} else {
				trap_SendServerCommand((ent - g_entities), va("cp \"Invalid item used: %d\"", event));
			}
			break;
		}

		// HI_BOOMIES
		case EV_USE_ITEM6: {
			const gitem_t *item = BG_FindItemForHoldable(HI_BOOMIES);
			if (item) {
				if (client->ps.stats[STAT_FORBIDDENITEMS] & (1 << HI_BOOMIES)) {
					trap_SendServerCommand((ent - g_entities), va("cp \"%s not allowed here\"", item->pickup_name));
				} else {
					if (boomies_createByPlayer(ent, item->pickup_name)) {
						client->ps.stats[STAT_HOLDABLEVAR]--;
						if (client->ps.stats[STAT_HOLDABLEVAR] <= 0) {
							client->ps.pm_flags |= PMF_USE_ITEM_HELD;
							client->ps.stats[STAT_HOLDABLEVAR] = 0;
							client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
						}
					}
				}
			} else {
				trap_SendServerCommand((ent - g_entities), va("cp \"Invalid item used: %d\"", event));
			}
			break;
		}
		default:
			break;
		}
	}
}

void BotTestSolid(vec3_t origin);

/*
==============
SendPendingPredictableEvents
==============
*/
static void SendPendingPredictableEvents(playerState_t *ps) {
	// if there are still events pending
	if (ps->entityEventSequence < ps->eventSequence) {
		gentity_t *t;
		int number;
		// create a temporary entity for this event which is sent to everyone
		// except the client who generated the event
		const int seq = ps->entityEventSequence & (MAX_PS_EVENTS - 1);
		const int event = ps->events[seq] | ((ps->entityEventSequence & 3) << 8);
		// set external event to zero before calling BG_PlayerStateToEntityState
		const int extEvent = ps->externalEvent;
		ps->externalEvent = 0;
		// create temporary entity for event
		t = G_TempEntity(ps->origin, event);
		number = t->s.number;
		BG_PlayerStateToEntityState(ps, &t->s, qtrue);
		t->s.number = number;
		t->s.eType = ET_EVENTS + event;
		t->s.eFlags |= EF_PLAYER_EVENT;
		t->s.otherEntityNum = ps->clientNum;
		// send to everyone except the client who generated the event
		t->r.svFlags |= SVF_NOTSINGLECLIENT;
		t->r.singleClient = ps->clientNum;
		// set back external event
		ps->externalEvent = extEvent;
	}
}

/*
==============
ClientThink

This will be called once for each client frame, which will
usually be a couple times for each server frame on fast clients.

If "g_synchronousClients 1" is set, this will be called exactly
once for each server frame, which makes for smooth demo recording.
==============
*/
static void ClientThink_real(gentity_t *ent) {
	gclient_t *client;
	pmove_t pm;
	int oldEventSequence;
	int msec;
	usercmd_t *ucmd;

	client = ent->client;

	// don't think if the client is not yet connected (and thus not yet spawned in)
	if (client->pers.connected != CON_CONNECTED) {
		return;
	}
	// mark the time, so the connection sprite can be removed
	ucmd = &ent->client->pers.cmd;

	// sanity check the command time to prevent speedup cheating
	if (ucmd->serverTime > level.time + 200) {
		ucmd->serverTime = level.time + 200;
		//		Com_Printf("serverTime <<<<<\n" );
	}
	if (ucmd->serverTime < level.time - 1000) {
		ucmd->serverTime = level.time - 1000;
		//		Com_Printf("serverTime >>>>>\n" );
	}

	msec = ucmd->serverTime - client->ps.commandTime;
	// following others may result in bad times, but we still want
	// to check for follow toggles
	if (msec < 1 && client->sess.spectatorState != SPECTATOR_FOLLOW) {
		return;
	}
	if (msec > 200) {
		msec = 200;
	}

	if (pmove_msec.integer < 8) {
		trap_Cvar_Set("pmove_msec", "8");
		trap_Cvar_Update(&pmove_msec);
	} else if (pmove_msec.integer > 33) {
		trap_Cvar_Set("pmove_msec", "33");
		trap_Cvar_Update(&pmove_msec);
	}

	if (pmove_fixed.integer || client->pers.pmoveFixed) {
		ucmd->serverTime = ((ucmd->serverTime + pmove_msec.integer - 1) / pmove_msec.integer) * pmove_msec.integer;
		// if (ucmd->serverTime - client->ps.commandTime <= 0)
		//	return;
	}

	//
	// check for exiting intermission
	//
	if (level.intermissiontime) {
		ClientIntermissionThink(client);
		return;
	}

	if ((client->buttons & BUTTON_DROPCART) && !(client->oldbuttons & BUTTON_DROPCART)) {
		Cmd_DropTeamItem_f(ent);
	}

	// spectators don't do much
	if ((client->sess.sessionTeam == TEAM_SPECTATOR) || LPSDeadSpec(client)) {
		if (client->sess.spectatorState == SPECTATOR_SCOREBOARD) {
			return;
		}
		SpectatorThink(ent, ucmd);
		// with wop, we need that in the touch-trigger code ...
		VectorCopy(client->ps.origin, client->oldOrigin);
		return;
	}

	// check for inactivity timer, but never drop the local client of a non-dedicated server
	if (!ClientInactivityTimer(client)) {
		return;
	}

	// clear the rewards if time
	if (level.time > client->rewardTime) {
		client->ps.eFlags &= REMOVE_AWARDFLAGS;
	}

	// set hurt flag
	if (client->ps.stats[STAT_HEALTH] <= 30)
		client->ps.eFlags |= EF_HURT;
	else
		client->ps.eFlags &= ~EF_HURT;

	// always reset balloontime
	client->ps.stats[STAT_BALLOONTIME] = 0;

	if (client->noclip) {
		client->ps.pm_type = PM_NOCLIP;
	} else if (client->ps.stats[STAT_HEALTH] <= 0) {
		client->ps.pm_type = PM_DEAD;
	} else if (FT_ClientIsFrozen(client)) {
		client->ps.pm_type = PM_FROZEN;
		// don't allow to fire any weapon
		ucmd->buttons &= ~BUTTON_ATTACK;
	} else {
		client->ps.pm_type = PM_NORMAL;
	}

	client->ps.gravity = g_gravity.value;

	// set speed
	client->ps.speed = g_speed.value;

	if (InSprayroom(client)) {
		// no speedmodification in sprayroom
	} else if (client->ps.powerups[PW_SPEEDY])
		client->ps.speed *= 2.0f;
	else {
		if (client->ps.powerups[PW_BERSERKER])
			client->ps.speed *= 1.5f;
		else if (client->ps.weapon == WP_PUNCHY)
			client->ps.speed *= 1.3f;
	}

	if (G_IsKillerDuck(ent)) {
		client->ps.speed *= 1.3f;
	}

	// Let go of the hook if we aren't firing
	if (client->ps.weapon == WP_GRAPPLING_HOOK && client->hook && !(ucmd->buttons & BUTTON_ATTACK)) {
		Weapon_HookFree(client->hook);
	}

	// set up for pmove
	oldEventSequence = client->ps.eventSequence;

	memset(&pm, 0, sizeof(pm));

	// check for the hit-scan gauntlet, don't let the action
	// go through as an attack unless it actually hits something
	if ((client->ps.weapon == WP_PUNCHY) && !(ucmd->buttons & BUTTON_TALK) && (ucmd->buttons & BUTTON_ATTACK) &&
		(client->ps.weaponTime <= 0) && !InSprayroom(client)) {
		pm.gauntletHit = CheckGauntletAttack(ent);
	}

	if (ent->flags & FL_FORCE_GESTURE) {
		ent->flags &= ~FL_FORCE_GESTURE;
		ent->client->pers.cmd.buttons |= BUTTON_GESTURE;
	}

	pm.ps = &client->ps;
	pm.cmd = *ucmd;
	if (pm.ps->pm_type == PM_DEAD) {
		pm.tracemask = MASK_PLAYERSOLID & ~CONTENTS_BODY;
	} else if (ent->r.svFlags & SVF_BOT) {
		pm.tracemask = MASK_PLAYERSOLID | CONTENTS_BOTCLIP;
	} else {
		pm.tracemask = MASK_PLAYERSOLID;
	}
	pm.trace = trap_Trace;
	pm.pointcontents = trap_PointContents;
	pm.debugLevel = g_debugMove.integer;
	pm.noFootsteps = (g_dmflags.integer & DF_NO_FOOTSTEPS) > 0;

	pm.pmove_fixed = pmove_fixed.integer | client->pers.pmoveFixed;
	pm.pmove_msec = pmove_msec.integer;

	VectorCopy(client->ps.origin, client->oldOrigin);

	pm.gametype = g_gametype.integer;
	Pmove(&pm);

	// save results of pmove
	if (ent->client->ps.eventSequence != oldEventSequence) {
		ent->eventTime = level.time;
	}
	if (g_smoothClients.integer) {
		BG_PlayerStateToEntityStateExtraPolate(&ent->client->ps, &ent->s, ent->client->ps.commandTime, qtrue);
	} else {
		BG_PlayerStateToEntityState(&ent->client->ps, &ent->s, qtrue);
	}
	SendPendingPredictableEvents(&ent->client->ps);

	if (!(ent->client->ps.eFlags & EF_FIRING)) {
		client->fireHeld = qfalse; // for grapple
	}

	// use the snapped origin for linking so it matches client predicted versions
	VectorCopy(ent->s.pos.trBase, ent->r.currentOrigin);

	VectorCopy(pm.mins, ent->r.mins);
	VectorCopy(pm.maxs, ent->r.maxs);

	ent->waterlevel = pm.waterlevel;
	ent->watertype = pm.watertype;

	// execute client events
	ClientEvents(ent, oldEventSequence);

	// reset forbidden items after events have been handled
	// trigger touches below will set flag for next frame and ClientEvents()
	client->ps.stats[STAT_FORBIDDENITEMS] = 0;

	// link entity now, after any personal teleporters have been used
	trap_LinkEntity(ent);
	if (!ent->client->noclip) {
		G_TouchTriggers(ent);
	}

	// freezetag
	if (G_FreezeTag()) {
		if (FT_PlayerIsFrozen(ent)) {
			if (!FT_MatchInProgress())
				FT_ThawPlayer(ent, NULL);
		} else {
			gentity_t *frozenPlayer = FT_NearestFrozenPlayer(ent);
			if (frozenPlayer) {
				if (FT_InThawingRange(ent, frozenPlayer))
					FT_ProgressThawing(frozenPlayer, ent);
			}

			// not thawing for a while, let the client know
			if (level.time - ent->client->lastProgressTime > 1000)
				ent->client->ps.stats[STAT_CHILL] = 0;

			// freeze player that joined too late
			if (ent->client->pers.ftLateJoin) {
				FT_FreezePlayer(ent, NULL);
				ent->client->pers.ftLateJoin = qfalse;
			}
		}
	}

	if (G_IsKillerDuck(ent)) {
		G_KillerDuckThink(ent);
	}

	// NOTE: now copy the exact origin over otherwise clients can be snapped into solid
	VectorCopy(ent->client->ps.origin, ent->r.currentOrigin);

	// test for solid areas in the AAS file
	BotTestAAS(ent->r.currentOrigin);

	// touch other objects
	ClientImpacts(ent, &pm);

	// save results of triggers and client events
	if (ent->client->ps.eventSequence != oldEventSequence) {
		ent->eventTime = level.time;
	}

	// swap and latch button actions
	client->oldbuttons = client->buttons;
	client->buttons = ucmd->buttons;
	client->latched_buttons |= client->buttons & ~client->oldbuttons;

	// check for respawning
	if (client->ps.stats[STAT_HEALTH] <= 0) {
		// wait for the attack button to be pressed
		if (level.time > client->respawnTime) {
			// forcerespawn is to prevent users from waiting out powerups
			if (g_forcerespawn.integer > 0 && (level.time - client->respawnTime) > g_forcerespawn.integer * 1000) {
				G_Respawn(ent);
				return;
			}

			// pressing attack or use is the normal respawn method
			if (ucmd->buttons & (BUTTON_ATTACK | BUTTON_USE_HOLDABLE)) {
				G_Respawn(ent);
			}
		}
		return;
	}

	if ((pm.waterlevel <= WL_SPLASHING) && (pm.ps->groundEntityNum != ENTITYNUM_NONE) &&
		((client->lastSentFlyingTime + 500) > level.time)) {
		if (!(pm.ps->pm_flags & PMF_TIME_KNOCKBACK)) {
			client->lastSentFlying = -1;
		}
	}

	// perform once-a-second actions
	ClientTimerActions(ent, msec);
}

/*
==================
ClientThink

A new command has arrived from the client
==================
*/
void ClientThink(int clientNum) {
	gentity_t *ent;

	ent = g_entities + clientNum;
	trap_GetUsercmd(clientNum, &ent->client->pers.cmd);

	// mark the time we got info, so we can display the
	// phone jack if they don't get any for a while
	ent->client->lastCmdTime = level.time;

	if (!(ent->r.svFlags & SVF_BOT) && !g_synchronousClients.integer) {
		ClientThink_real(ent);
	}
}

void G_RunClient(gentity_t *ent) {
	if (!(ent->r.svFlags & SVF_BOT) && !g_synchronousClients.integer) {
		return;
	}
	ent->client->pers.cmd.serverTime = level.time;
	ClientThink_real(ent);
}

/*
==================
SpectatorClientEndFrame

==================
*/
static void SpectatorClientEndFrame(gentity_t *ent) {
	gclient_t *cl;

	// if we are doing a chase cam or a remote view, grab the latest info
	if (ent->client->sess.spectatorState == SPECTATOR_FOLLOW) {
		int clientNum, flags;

		clientNum = ent->client->sess.spectatorClient;

		// team follow1 and team follow2 go to whatever clients are playing
		if (clientNum == -1) {
			clientNum = level.follow1;
		} else if (clientNum == -2) {
			clientNum = level.follow2;
		}
		if (clientNum >= 0) {
			cl = &level.clients[clientNum];
			if ((cl->pers.connected == CON_CONNECTED) &&
				((cl->sess.sessionTeam != TEAM_SPECTATOR) && !LPSDeadSpec(cl))) {
				flags =
					(cl->ps.eFlags & ~(EF_VOTED | EF_TEAMVOTED)) | (ent->client->ps.eFlags & (EF_VOTED | EF_TEAMVOTED));
				ent->client->ps = cl->ps;
				ent->client->ps.pm_flags |= PMF_FOLLOW;
				ent->client->ps.eFlags = flags;
				return;
			} else {
				// drop them to free spectators unless they are dedicated camera followers
				if (ent->client->sess.spectatorClient >= 0) {
					ent->client->sess.spectatorState = SPECTATOR_FREE;
					ClientBegin(ent->client - level.clients);
				}
			}
		}
	}

	if (ent->client->sess.spectatorState == SPECTATOR_SCOREBOARD) {
		ent->client->ps.pm_flags |= PMF_SCOREBOARD;
	} else {
		ent->client->ps.pm_flags &= ~PMF_SCOREBOARD;
	}
}

/*
==============
ClientEndFrame

Called at the end of each server frame for each connected client
A fast client will have multiple ClientThink for each ClientEdFrame,
while a slow client may have multiple ClientEndFrame between ClientThink.
==============
*/
void ClientEndFrame(gentity_t *ent) {
	int i;

	if ((ent->client->sess.sessionTeam == TEAM_SPECTATOR) || LPSDeadSpec(ent->client)) {
		SpectatorClientEndFrame(ent);
		return;
	}

	// turn off any expired powerups
	for (i = 0; i < MAX_POWERUPS; i++) {
		if (ent->client->ps.powerups[i] < level.time) {
			ent->client->ps.powerups[i] = 0;
		}
	}

	//
	// If the end of unit layout is displayed, don't give
	// the player any normal movement attributes
	//
	if (level.intermissiontime) {
		return;
	}

	// burn from lava, etc
	P_WorldEffects(ent);

	// apply all the damage taken this frame
	P_DamageFeedback(ent);

	// add the EF_CONNECTION flag if we haven't gotten commands recently
	if (level.time - ent->client->lastCmdTime > 1000) {
		ent->client->ps.eFlags |= EF_CONNECTION;
	} else {
		ent->client->ps.eFlags &= ~EF_CONNECTION;
	}

	ent->client->ps.stats[STAT_HEALTH] = ent->health; // FIXME: get rid of ent->health...

	G_SetClientSound(ent);

	// set the latest infor
	if (g_smoothClients.integer) {
		BG_PlayerStateToEntityStateExtraPolate(&ent->client->ps, &ent->s, ent->client->ps.commandTime, qtrue);
	} else {
		BG_PlayerStateToEntityState(&ent->client->ps, &ent->s, qtrue);
	}
	SendPendingPredictableEvents(&ent->client->ps);

	// set the bit for the reachability area the client is currently in
	//	i = trap_AAS_PointReachabilityAreaIndex( ent->client->ps.origin );
	//	ent->client->areabits[i >> 3] |= 1 << (i & 7);
}
