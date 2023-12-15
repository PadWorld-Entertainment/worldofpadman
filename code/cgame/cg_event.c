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
// cg_event.c -- handle entity events at snapshot or playerstate transitions

#include "cg_local.h"

//==========================================================================

/*
===================
CG_PlaceString

Also called by scoreboard drawing
===================
*/
const char *CG_PlaceString(int rank) {
	static char str[64];
	char *s, *t;

	if (rank & RANK_TIED_FLAG) {
		rank &= ~RANK_TIED_FLAG;
		t = "Tied for ";
	} else {
		t = "";
	}

	if (rank == 1) {
		s = S_COLOR_BLUE "1st" S_COLOR_WHITE; // draw in blue
	} else if (rank == 2) {
		s = S_COLOR_RED "2nd" S_COLOR_WHITE; // draw in red
	} else if (rank == 3) {
		s = S_COLOR_GREEN "3rd" S_COLOR_WHITE; // draw in green
	} else if (rank == 11) {
		s = "11th";
	} else if (rank == 12) {
		s = "12th";
	} else if (rank == 13) {
		s = "13th";
	} else if (rank % 10 == 1) {
		s = va("%ist", rank);
	} else if (rank % 10 == 2) {
		s = va("%ind", rank);
	} else if (rank % 10 == 3) {
		s = va("%ird", rank);
	} else {
		s = va("%ith", rank);
	}

	Com_sprintf(str, sizeof(str), "%s%s", t, s);
	return str;
}

/*
=============
CG_Obituary
=============
*/
static void CG_Obituary(entityState_t *ent) {
	int mod;
	int target, attacker;
	char *message;
	char *message2;
	const char *targetInfo;
	const char *attackerInfo;
	char targetName[32];
	char attackerName[32];
	gender_t gender;
	clientInfo_t *ci;

	target = ent->otherEntityNum;
	attacker = ent->otherEntityNum2;
	mod = ent->eventParm;

	if (target < 0 || target >= MAX_CLIENTS) {
		CG_Error("CG_Obituary: target out of range");
	}
	ci = &cgs.clientinfo[target];

	if (attacker < 0 || attacker >= MAX_CLIENTS) {
		attacker = ENTITYNUM_WORLD;
		attackerInfo = NULL;
	} else {
		attackerInfo = CG_ConfigString(CS_PLAYERS + attacker);
	}

	targetInfo = CG_ConfigString(CS_PLAYERS + target);
	if (!targetInfo) {
		return;
	}
	Q_strncpyz(targetName, Info_ValueForKey(targetInfo, "n"), sizeof(targetName) - 2);
	Q_strcat(targetName, sizeof(targetName), S_COLOR_WHITE);

	message2 = "";

	// check for single client messages
	if (attacker != ENTITYNUM_WORLD) {
		message = NULL;
	} else {
		switch (mod) {
		case MOD_SUICIDE:
			message = "suicides";
			break;
		case MOD_FALLING:
			message = "cratered";
			break;
		case MOD_CRUSH:
			message = "was squished";
			break;
		case MOD_WATER:
			message = "sank like a rock";
			break;
		case MOD_SLIME:
			message = "melted";
			break;
		case MOD_LAVA:
			message = "does a back flip into the lava";
			break;
		case MOD_TARGET_LASER:
			message = "saw the light";
			break;
		case MOD_TRIGGER_HURT:
			message = "was in the wrong place";
			break;
		case MOD_BAMBAM:
			// FIXME: Remove this once we got proper clients for bambam kills
			message = "tried to hug a BamBam";
			break;

		default:
			message = NULL;
			break;
		}
	}

	if (attacker == target) {
		gender = ci->gender;
		switch (mod) {
		case MOD_BALLOONY_SPLASH:
			if (gender == GENDER_MALE)
				message = "tripped on his own water bomb";
			else if (gender == GENDER_FEMALE)
				message = "tripped on her own water bomb";
			else if (gender == GENDER_NEUTER)
				message = "tripped on its own water bomb";
			else
				message = "tripped on their own water bomb";
			break;
		case MOD_BETTY_SPLASH:
			if (gender == GENDER_MALE)
				message = "blew himself up";
			else if (gender == GENDER_FEMALE)
				message = "blew herself up";
			else if (gender == GENDER_NEUTER)
				message = "blew itself up";
			else
				message = "blew themselves up";
			break;
		case MOD_BUBBLEG_SPLASH:
			if (gender == GENDER_MALE)
				message = "melted himself";
			else if (gender == GENDER_FEMALE)
				message = "melted herself";
			else if (gender == GENDER_NEUTER)
				message = "melted itself";
			else
				message = "melted themselves";
			break;
		case MOD_IMPERIUS_SPLASH:
			message = "should have used a smaller gun";
			break;
		default:
			message = "did the lemming thing";
			break;
		}
	}

	if (message) {
		CG_Printf("%s %s.\n", targetName, message);
		return;
	}

	// check for kill messages from the current clientNum
	if (attacker == cg.snap->ps.clientNum) {
		char *s;

		if (cgs.gametype < GT_TEAM) {
			if (cgs.gametype == GT_LPS) {
				const char *gender_strings[] = {"they have", "he has", "she has", "it has", NULL};
				CASSERT(ARRAY_LEN(gender_strings) == GENDER_MAX + 1);

				gender = ci->gender;
				if (gender >= GENDER_MAX || gender < GENDER_NONE)
					gender = GENDER_NONE;

				if (ent->generic1 == 0)
					s = va("You fragged %s,\n%s no lives left.", targetName, gender_strings[gender]);
				else if (ent->generic1 == 1)
					s = va("You fragged %s,\n%s 1 life left.", targetName, gender_strings[gender]);
				else
					s = va("You fragged %s,\n%s %i lives left.", targetName, gender_strings[gender], ent->generic1);
			} else
				s = va("You fragged %s\n%s place with %i", targetName,
					   CG_PlaceString(cg.snap->ps.persistant[PERS_RANK] + 1), cg.snap->ps.persistant[PERS_SCORE]);
		} else {
			s = va("You fragged %s", targetName);
		}
		CG_CenterPrint(s, SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH);

		// print the text message as well
	}

	// check for double client messages
	if (!attackerInfo) {
		attacker = ENTITYNUM_WORLD;
		strcpy(attackerName, "noname");
	} else {
		Q_strncpyz(attackerName, Info_ValueForKey(attackerInfo, "n"), sizeof(attackerName) - 2);
		Q_strcat(attackerName, sizeof(attackerName), S_COLOR_WHITE);
		// check for kill messages about the current clientNum
		if (target == cg.snap->ps.clientNum) {
			Q_strncpyz(cg.killerName, attackerName, sizeof(cg.killerName));
		}
	}

	if (attacker != ENTITYNUM_WORLD) {
		switch (mod) {
		case MOD_GRAPPLE:
			message = "was caught by";
			break;
		case MOD_PUNCHY:
			message = "was bitten to death by";
			message2 = "'s pet 'Punchy'";
			break;
		case MOD_NIPPER:
			message = "was nipped by";
			break;
		case MOD_PUMPER:
			message = "was electrocuted by";
			message2 = "'s Pumper";
			break;
		case MOD_BALLOONY:
			message = "was flattened by";
			message2 = "'s water bomb";
			break;
		case MOD_BALLOONY_SPLASH:
			message = "was drowned by";
			message2 = "'s water bomb";
			break;
		case MOD_BETTY:
			message = "was fried from";
			message2 = "'s Betty";
			break;
		case MOD_BETTY_SPLASH:
			message = "was almost fried from";
			message2 = "'s Betty";
			break;
		case MOD_BUBBLEG:
		case MOD_BUBBLEG_SPLASH:
			message = "was plastered by";
			message2 = "'s Bubble.G.";
			break;
		case MOD_SPLASHER:
			message = "was splashed by";
			message2 = "'s Splasher";
			break;
		case MOD_BOASTER:
			message = "was showered by";
			break;
		case MOD_IMPERIUS:
		case MOD_IMPERIUS_SPLASH:
			message = "couldn't survive the impact from";
			message2 = "'s Imperius";
			break;
		case MOD_KILLERDUCKS:
			message = "was hunted & bitten to death by";
			message2 = "'s Killerduck";
			break;
		case MOD_TELEFRAG:
			message = "tried to invade";
			message2 = "'s personal space";
			break;
		case MOD_BAMBAM:
			message = "was plastered by";
			message2 = "'s BamBam";
			break;
		case MOD_BOOMIES:
			message = "tried to look at";
			message2 = "'s Boomie too closely";
			break;
		case MOD_INJECTOR:
			message = "was pricked by";
			message2 = "'s Injector";
			break;
		case MOD_LAVA:
			message = "was given a hot bath by";
			break;
		case MOD_SLIME:
			message = "was given an acid bath by";
			break;
		case MOD_FALLING:
			// just in case
			message = "was given a small push by";
			break;
		case MOD_TRIGGER_HURT:
			message = "was helped on the way by";
			break;
		case MOD_CRUSH:
			message = "was crushed in";
			message2 = "'s trap";
			break;

		default:
			message = "was killed by";
			break;
		}

		if (message) {
			CG_Printf("%s %s %s%s\n", targetName, message, attackerName, message2);
			return;
		}
	}

	// we don't know what it was
	CG_Printf("%s died.\n", targetName);
}

/*
===============
CG_UseItem
===============
*/
static void CG_UseItem(centity_t *cent) {
	entityState_t *es = &cent->currentState;

	int itemNum = (es->event & ~EV_EVENT_BITS) - EV_USE_ITEM0;
	if (itemNum < 0 || itemNum > HI_NUM_HOLDABLE) {
		itemNum = 0;
	}

	// print a message if the local player
	if (es->number == cg.snap->ps.clientNum) {
		if (!itemNum) {
			CG_CenterPrint("No item to use", SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH);
		}
	}

	switch (itemNum) {
	default:
	case HI_NONE:
		trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.useNothingSound);
		break;
	case HI_MEDKIT: {
		const int clientNum = cent->currentState.clientNum;
		if (clientNum >= 0 && clientNum < MAX_CLIENTS) {
			clientInfo_t *ci = &cgs.clientinfo[clientNum];
			ci->medkitUsageTime = cg.time;
		}
		trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.medkitSound);
		break;
	}
	case HI_TELEPORTER:
	case HI_FLOATER:
	case HI_KILLERDUCKS:
		break;
	}
}

/*
================
CG_WeaponHigher
================
*/
static qboolean CG_WeaponHigher(int currentWeapon, int newWeapon) {
	const char *currentScore = NULL;
	const char *newScore = NULL;
	char weapon[5];
	Com_sprintf(weapon, sizeof(weapon), "/%i/", currentWeapon);
	currentScore = strstr(cg_weaponOrder.string, weapon);
	Com_sprintf(weapon, sizeof(weapon), "/%i/", newWeapon);
	newScore = strstr(cg_weaponOrder.string, weapon);
	if (!newScore || !currentScore)
		return qfalse;
	if (newScore > currentScore)
		return qtrue;
	else
		return qfalse;
}

/*
================
CG_ItemPickup

A new item was picked up this frame
================
*/
static void CG_ItemPickup(int itemNum) {
	cg.itemPickup = itemNum;
	cg.itemPickupTime = cg.time;
	cg.itemPickupBlendTime = cg.time;
	// see if it should be the grabbed weapon
	if (bg_itemlist[itemNum].giType == IT_WEAPON) {
		// select it immediately

		if (cg_autoswitch.integer != 0) {
			if (cg.zoomed)
				CG_ZoomDown_f();
			// no Autoswitch with Berserker
			if (cg.snap->ps.powerups[PW_BERSERKER])
				return;
			// no Autoswitch from SprayPistol
			if (cg.weaponSelect == WP_SPRAYPISTOL)
				return;
		}

		// if always
		if (cg_autoswitch.integer == 1 && bg_itemlist[itemNum].giTag != WP_NIPPER) {
			cg.weaponSelectTime = cg.time;
			cg.weaponSelect = bg_itemlist[itemNum].giTag;
		}

		// if new
		if (cg_autoswitch.integer == 2 && 0 == (cg.snap->ps.stats[STAT_WEAPONS] & (1 << bg_itemlist[itemNum].giTag))) {
			cg.weaponSelectTime = cg.time;
			cg.weaponSelect = bg_itemlist[itemNum].giTag;
		}

		// if better
		if (cg_autoswitch.integer == 3 && CG_WeaponHigher(cg.weaponSelect, bg_itemlist[itemNum].giTag)) {
			cg.weaponSelectTime = cg.time;
			cg.weaponSelect = bg_itemlist[itemNum].giTag;
		}

		// if new and better
		if (cg_autoswitch.integer == 4 && 0 == (cg.snap->ps.stats[STAT_WEAPONS] & (1 << bg_itemlist[itemNum].giTag)) &&
			CG_WeaponHigher(cg.weaponSelect, bg_itemlist[itemNum].giTag)) {
			cg.weaponSelectTime = cg.time;
			cg.weaponSelect = bg_itemlist[itemNum].giTag;
		}
	}

	if (bg_itemlist[itemNum].giType == IT_POWERUP && bg_itemlist[itemNum].giTag == PW_BERSERKER) {
		if (cg.weaponSelect != WP_SPRAYPISTOL && cg.weaponSelect != WP_PUNCHY) {
			cg.weaponSelectTime = cg.time;
			cg.weaponSelect = WP_PUNCHY;
		}
	}
}

/*
================
CG_WaterLevel
Returns waterlevel for entity origin
================
*/
static int CG_WaterLevel(centity_t *cent) {
	vec3_t point;
	int contents, sample1, sample2, anim, waterlevel;
	int viewheight;

	anim = cent->currentState.legsAnim & ~ANIM_TOGGLEBIT;

	if (anim == LEGS_WALKCR || anim == LEGS_IDLECR) {
		viewheight = CROUCH_VIEWHEIGHT;
	} else {
		viewheight = DEFAULT_VIEWHEIGHT;
	}

	//
	// get waterlevel, accounting for ducking
	//
	waterlevel = WL_NOT;

	point[0] = cent->lerpOrigin[0];
	point[1] = cent->lerpOrigin[1];
	point[2] = cent->lerpOrigin[2] + MINS_Z + 1;
	contents = CG_PointContents(point, -1);

	if (contents & MASK_WATER) {
		sample2 = viewheight - MINS_Z;
		sample1 = sample2 / 2;
		waterlevel = WL_SPLASHING;
		point[2] = cent->lerpOrigin[2] + MINS_Z + sample1;
		contents = CG_PointContents(point, -1);

		if (contents & MASK_WATER) {
			waterlevel = WL_SWIMMING;
			point[2] = cent->lerpOrigin[2] + MINS_Z + sample2;
			contents = CG_PointContents(point, -1);

			if (contents & MASK_WATER) {
				waterlevel = WL_DIVING;
			}
		}
	}

	return waterlevel;
}

/*
================
CG_PainEvent

Also called by playerstate transition
================
*/
void CG_PainEvent(centity_t *cent, int health) {
	char *snd;

	// don't do more than two pain sounds a second
	if (cg.time - cent->pe.painTime < 500) {
		return;
	}

	if (health < 25) {
		snd = "*pain25_1";
	} else if (health < 50) {
		snd = "*pain50_1";
	} else if (health < 75) {
		snd = "*pain75_1";
	} else {
		snd = "*pain100_1";
	}
#if 0
	// play a gurp sound instead of a normal pain sound
	if (CG_WaterLevel(cent) == WL_DIVING) {
		if (rand() & 1) {
			trap_S_StartSound(NULL, cent->currentState.number, CHAN_VOICE, CG_CustomSound(cent->currentState.number, "sound/padplayer/gurp1"));
		} else {
			trap_S_StartSound(NULL, cent->currentState.number, CHAN_VOICE, CG_CustomSound(cent->currentState.number, "sound/padplayer/gurp2"));
		}
	} else
#endif
	{ trap_S_StartSound(NULL, cent->currentState.number, CHAN_VOICE, CG_CustomSound(cent->currentState.number, snd)); }

	// save pain time for programitic twitch animation
	cent->pe.painTime = cg.time;
	cent->pe.painDirection ^= 1;
}

/*
==============
CG_EntityEvent

An entity has an event value
also called by CG_CheckPlayerstateEvents
==============
*/
void CG_EntityEvent(centity_t *cent, vec3_t position) {
	entityState_t *es;
	int event;
	vec3_t dir;
	const char *s;
	int clientNum;
	clientInfo_t *ci;
	const weaponInfo_t *wi; // HERBY

	es = &cent->currentState;
	event = es->event & ~EV_EVENT_BITS;

	if (cg_debugEvents.integer) {
		CG_Printf("ent:%3i  event:%3i ", es->number, event);
	}

	if (!event) {
		if (cg_debugEvents.integer) {
			CG_Printf("ZEROEVENT\n");
		}
		return;
	}

	clientNum = es->clientNum;
	if (clientNum < 0 || clientNum >= MAX_CLIENTS) {
		clientNum = 0;
	}
	ci = &cgs.clientinfo[clientNum];
	if (cg_debugEvents.integer && event >= 0 && event < EV_EVENT_MAX) {
		CG_Printf("%s\n", entity_event_names[event]);
	}

	switch (event) {
	//
	// movement generated events
	//
	case EV_FOOTSTEP:
		if (cg_footsteps.integer) {
			trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[ci->footsteps][rand() & 3]);
		}
		break;
	case EV_FOOTSTEP_METAL:
		if (cg_footsteps.integer) {
			trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_METAL][rand() & 3]);
		}
		break;
	case EV_FOOTSTEP_CARPET:
		if (cg_footsteps.integer) {
			trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_CARPET][rand() & 3]);
		}
		break;
	case EV_FOOTSTEP_LATTICE:
		if (cg_footsteps.integer) {
			trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_LATTICE][rand() & 3]);
		}
		break;
	case EV_FOOTSTEP_SAND:
		if (cg_footsteps.integer) {
			trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_SAND][rand() & 3]);
		}
		break;
	case EV_FOOTSTEP_SOFT:
		if (cg_footsteps.integer) {
			trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_SOFT][rand() & 3]);
		}
		break;
	case EV_FOOTSTEP_WOOD:
		if (cg_footsteps.integer) {
			trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_WOOD][rand() & 3]);
		}
		break;
	case EV_FOOTSTEP_SNOW:
		if (cg_footsteps.integer) {
			trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_SNOW][rand() & 3]);
		}
		break;
	case EV_FOOTSPLASH:
		if (cg_footsteps.integer) {
			trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_SPLASH][rand() & 3]);
		}
		break;
	case EV_FOOTWADE:
	case EV_SWIM:
		if (cg_footsteps.integer) {
			trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_WADE][rand() & 3]);
		}
		break;

	case EV_FALL_SHORT:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.landSound);
		if (clientNum == cg.predictedPlayerState.clientNum) {
			// smooth landing z changes
			cg.landChange = -8;
			cg.landTime = cg.time;
		}
		break;
	case EV_FALL_MEDIUM:
		// use normal pain sound
		trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*pain100_1"));
		if (clientNum == cg.predictedPlayerState.clientNum) {
			// smooth landing z changes
			cg.landChange = -16;
			cg.landTime = cg.time;
		}
		break;
	case EV_FALL_FAR:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "*fall1"));
		cent->pe.painTime = cg.time; // don't play a pain sound right after this
		if (clientNum == cg.predictedPlayerState.clientNum) {
			// smooth landing z changes
			cg.landChange = -24;
			cg.landTime = cg.time;
		}
		break;

	case EV_STEP_4:
	case EV_STEP_8:
	case EV_STEP_12:
	case EV_STEP_16: // smooth out step up transitions
		{
			float oldStep;
			int delta;
			int step;

			if (clientNum != cg.predictedPlayerState.clientNum) {
				break;
			}
			// if we are interpolating, we don't need to smooth steps
			if (cg.demoPlayback || (cg.snap->ps.pm_flags & PMF_FOLLOW) || cg_nopredict.integer ||
				cg_synchronousClients.integer) {
				break;
			}
			// check for stepping up before a previous step is completed
			delta = cg.time - cg.stepTime;
			if (delta < STEP_TIME) {
				oldStep = cg.stepChange * (STEP_TIME - delta) / STEP_TIME;
			} else {
				oldStep = 0;
			}

			// add this amount
			step = 4 * (event - EV_STEP_4 + 1);
			cg.stepChange = oldStep + step;
			if (cg.stepChange > MAX_STEP_CHANGE) {
				cg.stepChange = MAX_STEP_CHANGE;
			}
			cg.stepTime = cg.time;
			break;
		}

	case EV_JUMP_PAD:
		{
			vec3_t up = {0.0f, 0.0f, 1.0f};

			CG_SmokePuff(cent->lerpOrigin, up, 32, 1, 1, 1, 0.33f, 1000, cg.time, 0, LEF_PUFF_DONT_SCALE,
						 cgs.media.smokePuffShader);
		}

		// boing sound at origin, jump sound on player
		trap_S_StartSound(cent->lerpOrigin, -1, CHAN_VOICE, cgs.media.jumpPadSound);
		trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*jump1"));
		break;

	case EV_JUMP:
		trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*jump1"));
		if (cg_entities[es->number].currentState.powerups & (1 << PW_JUMPER))
			trap_S_StartSound(NULL, es->number, CHAN_ITEM, cgs.media.jumperSound);
		break;
	case EV_TAUNT:
		if (es->powerups & (1 << PW_SPEEDY) && VectorLengthSquared(es->pos.trDelta) > 200.0f * 200.0f)
			trap_S_StartSound(NULL, es->number, CHAN_VOICE, cgs.media.speedyTaunt);
		else
			trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*taunt"));
		break;
	case EV_HEHE1:
		trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*hehe1"));
		break;
	case EV_HEHE2:
		trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*hehe2"));
		break;
	case EV_WATER_TOUCH:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.watrInSound);
		break;
	case EV_WATER_LEAVE:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.watrOutSound);
		break;
	case EV_WATER_UNDER:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.watrUnSound);
		ci->lastWaterClearTime = 0; // for wet-screen effect
		break;
	case EV_WATER_CLEAR:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "*gasp"));
		ci->lastWaterClearTime = cg.time; // for wet-screen effect
		break;

	case EV_ITEM_PICKUP:
		{
			const gitem_t *item;
			int index;

			index = es->eventParm; // player predicted

			if (index < 1 || index >= bg_numItems) {
				break;
			}
			item = &bg_itemlist[index];

			// powerups and team items will have a separate global sound, this one
			// will be played at prediction time
			if (item->giType == IT_POWERUP) {
				trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.n_healthSound);
			} else if (item->giType == IT_TEAM) {
				trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.stolenLollySound);
			} else if (item->giType != IT_PERSISTANT_POWERUP) {
				if (item->giType == IT_WEAPON)
					trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.pickupSound);
				if (item->giType != IT_WEAPON || !(cg.snap->ps.stats[STAT_WEAPONS] & (1 << item->giTag))) {
					trap_S_StartSound(NULL, es->number, CHAN_AUTO, trap_S_RegisterSound(item->pickup_sound, qfalse));
					if (item->giType == IT_HOLDABLE)
						trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.HIpickupSound);
					else if (!Q_stricmp(item->classname, "item_armor_padshield"))
						trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.ARpickupSound);
				}
			}

			// show icon and name on status bar
			if (es->number == cg.snap->ps.clientNum) {
				CG_ItemPickup(index);
			}
		}
		break;

	case EV_GLOBAL_ITEM_PICKUP:
		{
			const gitem_t *item;
			int index;

			index = es->eventParm; // player predicted

			if (index < 1 || index >= bg_numItems) {
				break;
			}
			item = &bg_itemlist[index];
			// powerup pickups are global
			if (item->pickup_sound) {
				trap_S_StartSound(NULL, cg.snap->ps.clientNum, CHAN_AUTO,
								  trap_S_RegisterSound(item->pickup_sound, qfalse));
			}

			// show icon and name on status bar
			if (es->number == cg.snap->ps.clientNum) {
				CG_ItemPickup(index);
			}
		}
		break;

	//
	// weapon events
	//
	case EV_NOAMMO:
		// trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.noAmmoSound);
		if (es->number == cg.snap->ps.clientNum) {
			CG_OutOfAmmoChange();
		}
		break;
	case EV_CHANGE_WEAPON:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.selectSound);
		break;
	case EV_FIRE_WEAPON:
		CG_FireWeapon(cent);
		break;
	case EV_IMPERIUS_EXPLODE:
		ByteToDir(es->eventParm, dir);
		CG_MissileHitWall(es->weapon, 0, position, dir, cent->currentState.generic1, IMPACTSOUND_DEFAULT);
		break;
	case EV_USE_ITEM0:
	case EV_USE_ITEM1:
	case EV_USE_ITEM2:
	case EV_USE_ITEM3:
	case EV_USE_ITEM4:
	case EV_USE_ITEM5:
	case EV_USE_ITEM6:
	case EV_USE_ITEM7:
	case EV_USE_ITEM8:
	case EV_USE_ITEM9:
	case EV_USE_ITEM10:
	case EV_USE_ITEM11:
	case EV_USE_ITEM12:
	case EV_USE_ITEM13:
	case EV_USE_ITEM14:
		CG_UseItem(cent);
		break;

	//
	// other events
	//
	case EV_PLAYER_TELEPORT_IN:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.teleInSound);
		ci->lastTeleInTime = cg.time;
		break;

	case EV_PLAYER_TELEPORT_OUT:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.teleOutSound);
		CG_TeleOutEffect(position, 0, cent);
		break;

	case EV_PLAYER_TELEPORT_RED_IN:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.teleInSound);
		ci->lastTeleInTime = cg.time;
		break;

	case EV_PLAYER_TELEPORT_RED_OUT:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.teleOutSound);
		CG_TeleOutEffect(position, 1, cent);
		break;

	case EV_PLAYER_TELEPORT_BLUE_IN:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.teleInSound);
		ci->lastTeleInTime = cg.time;
		break;

	case EV_PLAYER_TELEPORT_BLUE_OUT:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.teleOutSound);
		CG_TeleOutEffect(position, 2, cent);
		break;

	case EV_DROP_CARTRIDGE:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.dropItemSound);
		break;

	case EV_ITEM_POP:
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.respawnSound);
		break;
	case EV_ITEM_RESPAWN:
		cent->miscTime = cg.time; // scale up from this
		trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.respawnSound);
		break;

	case EV_GRENADE_BOUNCE:
		if (rand() & 1) {
			trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.hgrenb1aSound);
		} else {
			trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.hgrenb2aSound);
		}
		break;

	case EV_STICKY_BOUNCE:
		ByteToDir(es->eventParm, cent->beamEnd);
		cent->miscTime = cg.time;
		// grenad_bounce vv  ... brauch ich das //noch ?
		if (rand() & 1) {
			trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.hgrenb1aSound);
		} else {
			trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.hgrenb2aSound);
		}
		// grenad_bounce ^^
		break;

	case EV_SCOREPLUM:
		CG_ScorePlum(cent->currentState.otherEntityNum, cent->lerpOrigin, cent->currentState.time);
		break;

	//
	// missile impacts
	//
	case EV_MISSILE_HIT:
		ByteToDir(es->eventParm, dir);
		// HERBY: Betty and Splasher Trail fix
		memcpy(&es->pos, &es->apos, sizeof(trajectory_t));
		wi = &cg_weapons[es->weapon];
		if (wi->missileTrailFunc)
			wi->missileTrailFunc(cent, wi);
		CG_MissileHitPlayer(es->weapon, position, dir, cent->currentState.generic1, es->otherEntityNum);
		break;

	case EV_MISSILE_MISS:
		ByteToDir(es->eventParm, dir);
		// HERBY: Betty and Splasher Trail fix
		memcpy(&es->pos, &es->apos, sizeof(trajectory_t));
		wi = &cg_weapons[es->weapon];
		if (wi->missileTrailFunc)
			wi->missileTrailFunc(cent, wi);
		CG_MissileHitWall(es->weapon, 0, position, dir, cent->currentState.generic1, IMPACTSOUND_DEFAULT);
		break;

	case EV_MISSILE_MISS_METAL:
		ByteToDir(es->eventParm, dir);
		// HERBY: Betty and Splasher Trail fix
		memcpy(&es->pos, &es->apos, sizeof(trajectory_t));
		wi = &cg_weapons[es->weapon];
		if (wi->missileTrailFunc)
			wi->missileTrailFunc(cent, wi);
		CG_MissileHitWall(es->weapon, 0, position, dir, cent->currentState.generic1, IMPACTSOUND_METAL);
		break;

	case EV_RAILTRAIL:
		cent->currentState.weapon = WP_PUMPER; // damit herbys code funzt ;)

		if (es->clientNum == cg.snap->ps.clientNum && !cg.renderingThirdPerson) {
			if (cg_drawGun.integer == 2)
				VectorMA(es->origin2, 8, cg.refdef.viewaxis[1], es->origin2);
			else if (cg_drawGun.integer == 3)
				VectorMA(es->origin2, 4, cg.refdef.viewaxis[1], es->origin2);
		}

		CG_RailTrail(ci, es->origin2, es->pos.trBase);

		// if the end was on a nomark surface, don't make an explosion
		if (es->eventParm != 255) {
			ByteToDir(es->eventParm, dir);
			CG_MissileHitWall(es->weapon, es->clientNum, position, dir, cent->currentState.generic1,
							  IMPACTSOUND_DEFAULT); // HERBY
		}
		break;

	case EV_SHOTGUN:
		CG_ShotgunFire(es);
		break;

	case EV_GENERAL_SOUND:
		if (cgs.gameSounds[es->eventParm]) {
			trap_S_StartSound(NULL, es->number, CHAN_VOICE, cgs.gameSounds[es->eventParm]);
		} else {
			s = CG_ConfigString(CS_SOUNDS + es->eventParm);
			trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, s));
		}
		break;

	case EV_SPRAYLOGO:
		// Hit a surface
		if (es->eventParm != 255) {
			vec4_t color;
			int radius = (int)(32.0f * es->angles[0]); // Scale by distance
			qhandle_t logohandle;

			ByteToDir(es->eventParm, dir);

			switch (ci->team) {
			case TEAM_RED:
				VectorSet(color, 1.0, 0.0, 0.0);
				break;
			case TEAM_BLUE:
				VectorSet(color, 0.0, 0.0, 1.0);
				break;
			default:
				VectorCopy(ci->color2, color);
				break;
			}
			color[3] = 1.0f;

			// Hit a spraywall
			if (es->generic1 == SPRAYROOM_CONSTANT) {
				logohandle = FindLogoForSpraying(ci);
				Add_LogoToDrawList(es->pos.trBase, dir, logohandle, radius, ci);
			} else {
				CG_ImpactMark(cgs.media.spraymark, es->pos.trBase, dir, (random() * 360), color[0], color[1], color[2],
							  color[3], qfalse, radius, qfalse);
			}
		}

		break;

	case EV_GLOBAL_SOUND: // play from the player's head so it never diminishes
		if (cgs.gameSounds[es->eventParm]) {
			trap_S_StartSound(NULL, cg.snap->ps.clientNum, CHAN_AUTO, cgs.gameSounds[es->eventParm]);
		} else {
			s = CG_ConfigString(CS_SOUNDS + es->eventParm);
			trap_S_StartSound(NULL, cg.snap->ps.clientNum, CHAN_AUTO, CG_CustomSound(es->number, s));
		}
		break;

	case EV_GLOBAL_TEAM_SOUND: // play from the player's head so it never diminishes
	{
		switch (es->eventParm) {
		case GTS_RED_CAPTURE: // CTF: red team captured the blue flag, 1FCTF: red team captured the neutral flag
			if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_RED)
				CG_AddBufferedSound(cgs.media.captureYourTeamSound);
			else
				CG_AddBufferedSound(cgs.media.captureOpponentSound);
			break;
		case GTS_BLUE_CAPTURE: // CTF: blue team captured the red flag, 1FCTF: blue team captured the neutral flag
			if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_BLUE)
				CG_AddBufferedSound(cgs.media.captureYourTeamSound);
			else
				CG_AddBufferedSound(cgs.media.captureOpponentSound);
			break;
		case GTS_RED_RETURN: // CTF: blue flag returned, 1FCTF: never used
			if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_RED)
				CG_AddBufferedSound(cgs.media.returnYourTeamSound);
			else
				CG_AddBufferedSound(cgs.media.returnOpponentSound);
			// "voc_"-sound:
			CG_AddBufferedSound(cgs.media.blueLollyReturnedSound);
			break;
		case GTS_BLUE_RETURN: // CTF red flag returned, 1FCTF: neutral flag returned
			if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_BLUE)
				CG_AddBufferedSound(cgs.media.returnYourTeamSound);
			else
				CG_AddBufferedSound(cgs.media.returnOpponentSound);
			// "voc_"-sound:
			CG_AddBufferedSound(cgs.media.redLollyReturnedSound);
			break;

		case GTS_RED_TAKEN: // CTF: red team took blue flag, 1FCTF: blue team took the neutral flag
			// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
			if (cg.snap->ps.powerups[PW_BLUEFLAG] || cg.snap->ps.powerups[PW_NEUTRALFLAG]) {
			} else {
				if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_BLUE) {
					CG_AddBufferedSound(cgs.media.takenOpponentSound);
					// "voc_"-sound:
					if (cgs.gametype == GT_1FCTF) {
						CG_AddBufferedSound(cgs.media.yourTeamTookTheLollySound);
					} else {
						CG_AddBufferedSound(cgs.media.enemyTookYourLollySound);
					}
				} else if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_RED) {
					CG_AddBufferedSound(cgs.media.takenYourTeamSound);
					// "voc_"-sound:
					if (cgs.gametype == GT_1FCTF) {
						CG_AddBufferedSound(cgs.media.enemyTookTheLollySound);
					} else {
						CG_AddBufferedSound(cgs.media.yourTeamTookEnemyLollySound);
					}
				}
			}
			break;
		case GTS_BLUE_TAKEN: // CTF: blue team took the red flag, 1FCTF red team took the neutral flag
			// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
			if (cg.snap->ps.powerups[PW_REDFLAG] || cg.snap->ps.powerups[PW_NEUTRALFLAG]) {
			} else {
				if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_RED) {
					CG_AddBufferedSound(cgs.media.takenOpponentSound);
					// "voc_"-sound:
					if (cgs.gametype == GT_1FCTF) {
						CG_AddBufferedSound(cgs.media.yourTeamTookTheLollySound);
					} else {
						CG_AddBufferedSound(cgs.media.enemyTookYourLollySound);
					}
				} else if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_BLUE) {
					CG_AddBufferedSound(cgs.media.takenYourTeamSound);
					// "voc_"-sound:
					if (cgs.gametype == GT_1FCTF) {
						CG_AddBufferedSound(cgs.media.enemyTookTheLollySound);
					} else {
						CG_AddBufferedSound(cgs.media.yourTeamTookEnemyLollySound);
					}
				}
			}
			break;

		case GTS_REDTEAM_SCORED:
			CG_AddBufferedSound(cgs.media.redScoredSound);
			break;
		case GTS_BLUETEAM_SCORED:
			CG_AddBufferedSound(cgs.media.blueScoredSound);
			break;

		case GTS_REDTEAM_TOOK_LEAD:
			CG_AddBufferedSound(cgs.media.redLeadsSound[(int)(1.9999f * random())]);
			break;
		case GTS_BLUETEAM_TOOK_LEAD:
			CG_AddBufferedSound(cgs.media.blueLeadsSound[(int)(1.9999f * random())]);
			break;

		case GTS_TEAMS_ARE_TIED:
			CG_AddBufferedSound(cgs.media.teamsTiedSound);
			break;
		default:
			break;
		}
		break;
	}

	case EV_PAIN:
		// local player sounds are triggered in CG_CheckLocalSounds,
		// so ignore events on the player
		if (cent->currentState.number != cg.snap->ps.clientNum) {
			CG_PainEvent(cent, es->eventParm);
		}
		break;

	case EV_DEATH1:
	case EV_DEATH2:
	case EV_DEATH3:
		if (CG_WaterLevel(cent) == WL_DIVING) {
			trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*drown"));
		} else {
			trap_S_StartSound(NULL, es->number, CHAN_VOICE,
							  CG_CustomSound(es->number, va("*death%i", event - EV_DEATH1 + 1)));
		}
		break;

	case EV_OBITUARY:
		CG_Obituary(es);
		break;

	//
	// powerup events
	//
	case EV_POWERUP_REVIVAL:
		if (es->number == cg.snap->ps.clientNum) {
			cg.powerupActive = PW_REVIVAL;
			cg.powerupTime = cg.time;
		}
		{
			static int lastrevivalsound = 0;
			if ((lastrevivalsound + 800) <= cg.time) {
				trap_S_StartSound(NULL, es->number, CHAN_ITEM, cgs.media.regenSound);
				lastrevivalsound = cg.time;
			}
		}
		break;
	case EV_POWERUP_PADPOWER:
		if (es->number == cg.snap->ps.clientNum) {
			cg.powerupActive = PW_PADPOWER;
			cg.powerupTime = cg.time;
		}
		trap_S_StartSound(NULL, es->number, CHAN_ITEM, cgs.media.quadSound);
		break;

	case EV_GIB_PLAYER:
		// don't play gib sound when using the kamikaze because it interferes
		// with the kamikaze sound, downside is that the gib sound will also
		// not be played when someone is gibbed while just carrying the kamikaze
		//		CG_GibPlayer( cent->lerpOrigin );
		break;

	case EV_STOPLOOPINGSOUND:
		trap_S_StopLoopingSound(es->number);
		es->loopSound = 0;
		break;

	case EV_BAMBAM_EXPLOSION:
		CG_BamBam_Explosion(position);
		break;
	case EV_BOOMIES_EXPLOSION:
		CG_Boomies_Explosion(position);
		break;

	case EV_DEBUG_LINE:
		CG_Beam(cent);
		break;
	default:
		CG_Error("Unknown event: %i", event);
		break;
	}
}

/*
==============
CG_CheckEvents

==============
*/
void CG_CheckEvents(centity_t *cent) {
	// check for event-only entities
	if (cent->currentState.eType > ET_EVENTS) {
		if (cent->previousEvent) {
			return; // already fired
		}
		// if this is a player event set the entity number of the client entity number
		if (cent->currentState.eFlags & EF_PLAYER_EVENT) {
			cent->currentState.number = cent->currentState.otherEntityNum;
		}

		cent->previousEvent = 1;

		cent->currentState.event = cent->currentState.eType - ET_EVENTS;
	} else {
		// check for events riding with another entity
		if (cent->currentState.event == cent->previousEvent) {
			return;
		}
		cent->previousEvent = cent->currentState.event;
		if ((cent->currentState.event & ~EV_EVENT_BITS) == 0) {
			return;
		}
	}

	// calculate the position at exactly the frame time
	BG_EvaluateTrajectory(&cent->currentState.pos, cg.snap->serverTime, cent->lerpOrigin);
	CG_SetEntitySoundPosition(cent);

	CG_EntityEvent(cent, cent->lerpOrigin);
}
