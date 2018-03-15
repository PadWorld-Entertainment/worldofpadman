// Copyright (C) 1999-2000 Id Software, Inc.
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
const char	*CG_PlaceString( int rank ) {
	static char	str[64];
	char	*s, *t;

	if ( rank & RANK_TIED_FLAG ) {
		rank &= ~RANK_TIED_FLAG;
		t = "Tied for ";
	} else {
		t = "";
	}

	if ( rank == 1 ) {
		s = S_COLOR_BLUE "1st" S_COLOR_WHITE;		// draw in blue
	} else if ( rank == 2 ) {
		s = S_COLOR_RED "2nd" S_COLOR_WHITE;		// draw in red
	} else if ( rank == 3 ) {
		s = S_COLOR_GREEN "3rd" S_COLOR_WHITE;		// draw in green
	} else if ( rank == 11 ) {
		s = "11th";
	} else if ( rank == 12 ) {
		s = "12th";
	} else if ( rank == 13 ) {
		s = "13th";
	} else if ( rank % 10 == 1 ) {
		s = va("%ist", rank);
	} else if ( rank % 10 == 2 ) {
		s = va("%ind", rank);
	} else if ( rank % 10 == 3 ) {
		s = va("%ird", rank);
	} else {
		s = va("%ith", rank);
	}

	Com_sprintf( str, sizeof( str ), "%s%s", t, s );
	return str;
}

/*
=============
CG_Obituary
=============
*/
static void CG_Obituary( entityState_t *ent ) {
	int			mod;
	int			target, attacker;
	char		*message;
	char		*message2;
	const char	*targetInfo;
	const char	*attackerInfo;
	char		targetName[32];
	char		attackerName[32];
	gender_t	gender;
	clientInfo_t	*ci;

	target = ent->otherEntityNum;
	attacker = ent->otherEntityNum2;
	mod = ent->eventParm;

	if ( target < 0 || target >= MAX_CLIENTS ) {
		CG_Error( "CG_Obituary: target out of range" );
	}
	ci = &cgs.clientinfo[target];

	if ( attacker < 0 || attacker >= MAX_CLIENTS ) {
		attacker = ENTITYNUM_WORLD;
		attackerInfo = NULL;
	} else {
		attackerInfo = CG_ConfigString( CS_PLAYERS + attacker );
	}

	targetInfo = CG_ConfigString( CS_PLAYERS + target );
	if ( !targetInfo ) {
		return;
	}
	Q_strncpyz( targetName, Info_ValueForKey( targetInfo, "n" ), sizeof(targetName) - 2);
	strcat( targetName, S_COLOR_WHITE );

	message2 = "";

	// check for single client messages
	if ( attacker != ENTITYNUM_WORLD ) {
		message = NULL;
	}
	else {
		switch( mod ) {
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
			if ( gender == GENDER_FEMALE )
				message = "tripped on her own Waterbomb";
			else if ( gender == GENDER_NEUTER )
				message = "tripped on its own Waterbomb";
			else
				message = "tripped on his own Waterbomb";
			break;
		case MOD_BETTY_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "blew herself up";
			else if ( gender == GENDER_NEUTER )
				message = "blew itself up";
			else
				message = "blew himself up";
			break;
		case MOD_BUBBLEG_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "melted herself";
			else if ( gender == GENDER_NEUTER )
				message = "melted itself";
			else
				message = "melted himself";
			break;
		case MOD_IMPERIUS_SPLASH:
			message = "should have used a smaller gun";
			break;
		default:/*
			if ( gender == GENDER_FEMALE )
				message = "killed herself";
			else if ( gender == GENDER_NEUTER )
				message = "killed itself";
			else
				message = "killed himself";
			break;*/
			message = "did the lemming thing";
			break;
		}
	}

	if (message) {
		CG_Printf( "%s %s.\n", targetName, message);
		return;
	}

	// check for kill messages from the current clientNum
	if ( attacker == cg.snap->ps.clientNum ) {
		char	*s;

		if ( cgs.gametype < GT_TEAM ) {
			if(cgs.gametype==GT_LPS)
			{
				//bg_public.h(115):typedef enum { GENDER_MALE, GENDER_FEMALE, GENDER_NEUTER } gender_t;
				const char *gender_strings[] = {"he", "she", "it", 0 };

				gender = ci->gender;
				if(gender>2 || gender<0) gender=2;

				if(ent->generic1==0)
					s = va("You fragged %s\n%s has no lives left.", targetName, gender_strings[gender]);
				else if(ent->generic1==1)
					s = va("You fragged %s\n%s has 1 live left.", targetName, gender_strings[gender]);
				else
					s = va("You fragged %s\n%s has %i lives left.", targetName, gender_strings[gender], 
						ent->generic1 );
			}
			else
				s = va("You fragged %s\n%s place with %i", targetName, 
					CG_PlaceString( cg.snap->ps.persistant[PERS_RANK] + 1 ),
					cg.snap->ps.persistant[PERS_SCORE] );
		} else {
			s = va("You fragged %s", targetName );
		}
		CG_CenterPrint( s, SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );

		// print the text message as well
	}

	// check for double client messages
	if ( !attackerInfo ) {
		attacker = ENTITYNUM_WORLD;
		strcpy( attackerName, "noname" );
	} else {
		Q_strncpyz( attackerName, Info_ValueForKey( attackerInfo, "n" ), sizeof(attackerName) - 2);
		strcat( attackerName, S_COLOR_WHITE );
		// check for kill messages about the current clientNum
		if ( target == cg.snap->ps.clientNum ) {
			Q_strncpyz( cg.killerName, attackerName, sizeof( cg.killerName ) );
		}
	}

	if ( attacker != ENTITYNUM_WORLD ) {
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
			message = "was pumped full of lead by";
			message2 = "'s Pumper";
			break;
		case MOD_BALLOONY:
			message = "was flattened by";
			message2 = "'s Water Bomb";
			break;
		case MOD_BALLOONY_SPLASH:
			message = "was drowned by";
			message2 = "'s Water Bomb";
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
			message2 = "'s splasher";
			break;
		case MOD_BOASTER:
			message = "was showered by";
			break;
		case MOD_IMPERIUS:
		case MOD_IMPERIUS_SPLASH:
			message = "couldnt survive the impact from";
			message2 = "'s Imperius";
			break;
		case MOD_KILLERDUCKS:
			message = "was hunted & bitten to death by";
			message2 = "'s KillerDuck";
			break;
		case MOD_TELEFRAG:
			message = "tried to invade";
			message2 = "'s personal space";
			break;
		case MOD_BAMBAM:
			message  = "was plastered by";
			message2 = "'s BamBam";
			break;
		case MOD_BOOMIES:
			message  = "tried to look at";
			message2 = "'s Boomies too closely";
			break;
		case MOD_INJECTOR:
			message  = "was pricked by";
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
			CG_Printf( "%s %s %s%s\n", 
				targetName, message, attackerName, message2);
			return;
		}
	}

	// we don't know what it was
	CG_Printf( "%s died.\n", targetName );
}

//==========================================================================

/*
===============
CG_UseItem
===============
*/
static void CG_UseItem( centity_t *cent ) {
	clientInfo_t *ci;
	int			itemNum, clientNum;
	//gitem_t		*item;
	entityState_t *es;

	es = &cent->currentState;
	
	itemNum = (es->event & ~EV_EVENT_BITS) - EV_USE_ITEM0;
	if ( itemNum < 0 || itemNum > HI_NUM_HOLDABLE ) {
		itemNum = 0;
	}

	// print a message if the local player
	if ( es->number == cg.snap->ps.clientNum ) {
		if ( !itemNum ) {
			CG_CenterPrint( "No item to use", SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
		}/* else {
			item = BG_FindItemForHoldable( itemNum );
			CG_CenterPrint( va("Use %s", item->pickup_name), SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
		}*/
	}

	switch ( itemNum ) {
	default:
	case HI_NONE:
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.useNothingSound );
		break;

	case HI_TELEPORTER:
		break;

	case HI_MEDKIT:
		clientNum = cent->currentState.clientNum;
		if ( clientNum >= 0 && clientNum < MAX_CLIENTS ) {
			ci = &cgs.clientinfo[ clientNum ];
			ci->medkitUsageTime = cg.time;
		}
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.medkitSound );
		break;

	case HI_FLOATER:
		break;

	case HI_KILLERDUCKS:
		break;

	}

}

/*
================
CG_ItemPickup

A new item was picked up this frame
================
*/
static void CG_ItemPickup( int itemNum ) {
	cg.itemPickup = itemNum;
	cg.itemPickupTime = cg.time;
	cg.itemPickupBlendTime = cg.time;
	// see if it should be the grabbed weapon
	if ( bg_itemlist[itemNum].giType == IT_WEAPON ) {
		// select it immediately
		if ( cg_autoswitch.integer && bg_itemlist[itemNum].giTag != WP_NIPPER ) {

			if(cg.zoomed) CG_ZoomDown_f();
			// no Autoswitch with Berserker
			if(cg.snap->ps.powerups[PW_BERSERKER])	return;
			// no Autoswitch from SprayPistol
			if(cg.weaponSelect==WP_SPRAYPISTOL) return;
			cg.weaponSelectTime = cg.time;
			cg.weaponSelect = bg_itemlist[itemNum].giTag;
		}
	}

	if(bg_itemlist[itemNum].giType == IT_POWERUP && bg_itemlist[itemNum].giTag == PW_BERSERKER)
	{
		if(cg.weaponSelect!=WP_SPRAYPISTOL && cg.weaponSelect!=WP_PUNCHY)
		{
			cg.weaponSelectTime = cg.time;
			cg.weaponSelect = WP_PUNCHY;
		}
	}
}


/*
================
CG_PainEvent

Also called by playerstate transition
================
*/
void CG_PainEvent( centity_t *cent, int health ) {
	char	*snd;

	// don't do more than two pain sounds a second
	if ( cg.time - cent->pe.painTime < 500 ) {
		return;
	}

	if ( health < 25 ) {
		snd = "*pain25_1";
	} else if ( health < 50 ) {
		snd = "*pain50_1";
	} else if ( health < 75 ) {
		snd = "*pain75_1";
	} else {
		snd = "*pain100_1";
	}
	trap_S_StartSound( NULL, cent->currentState.number, CHAN_VOICE, 
		CG_CustomSound( cent->currentState.number, snd ) );

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
#define	DEBUGNAME(x) if(cg_debugEvents.integer){CG_Printf(x"\n");}
void CG_EntityEvent( centity_t *cent, vec3_t position ) {
	entityState_t	*es;
	int				event;
	vec3_t			dir;
	const char		*s;
	int				clientNum;
	clientInfo_t	*ci;
	weaponInfo_t	*wi;//HERBY


	es = &cent->currentState;
	event = es->event & ~EV_EVENT_BITS;

	if ( cg_debugEvents.integer ) {
		CG_Printf( "ent:%3i  event:%3i ", es->number, event );
	}

	if ( !event ) {
		DEBUGNAME("ZEROEVENT");
		return;
	}

	clientNum = es->clientNum;
	if ( clientNum < 0 || clientNum >= MAX_CLIENTS ) {
		clientNum = 0;
	}
	ci = &cgs.clientinfo[ clientNum ];

	switch ( event ) {
	//
	// movement generated events
	//
	case EV_FOOTSTEP:
		DEBUGNAME("EV_FOOTSTEP");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ ci->footsteps ][rand()&3] );
		}
		break;
	case EV_FOOTSTEP_METAL:
		DEBUGNAME("EV_FOOTSTEP_METAL");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_METAL ][rand()&3] );
		}
		break;
	case EV_FOOTSTEP_CARPET:
		DEBUGNAME("EV_FOOTSTEP_CARPET");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_CARPET ][rand()&3] );
		}
		break;
	case EV_FOOTSTEP_LATTICE:
		DEBUGNAME("EV_FOOTSTEP_LATTICE");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_LATTICE ][rand()&3] );
		}
		break;
	case EV_FOOTSTEP_SAND:
		DEBUGNAME("EV_FOOTSTEP_SAND");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SAND ][rand()&3] );
		}
		break;
	case EV_FOOTSTEP_SOFT:
		DEBUGNAME("EV_FOOTSTEP_SOFT");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SOFT ][rand()&3] );
		}
		break;
	case EV_FOOTSTEP_WOOD:
		DEBUGNAME("EV_FOOTSTEP_WOOD");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_WOOD ][rand()&3] );
		}
		break;
	case EV_FOOTSTEP_SNOW:
		DEBUGNAME("EV_FOOTSTEP_SNOW");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SNOW ][rand()&3] );
		}
		break;
	case EV_FOOTSPLASH:
		DEBUGNAME("EV_FOOTSPLASH");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;
	case EV_FOOTWADE:
		DEBUGNAME("EV_FOOTWADE");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;
	case EV_SWIM:
		DEBUGNAME("EV_SWIM");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;


	case EV_FALL_SHORT:
		DEBUGNAME("EV_FALL_SHORT");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound );
		if ( clientNum == cg.predictedPlayerState.clientNum ) {
			// smooth landing z changes
			cg.landChange = -8;
			cg.landTime = cg.time;
		}
		break;
	case EV_FALL_MEDIUM:
		DEBUGNAME("EV_FALL_MEDIUM");
		// use normal pain sound
		trap_S_StartSound( NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*pain100_1" ) );
		if ( clientNum == cg.predictedPlayerState.clientNum ) {
			// smooth landing z changes
			cg.landChange = -16;
			cg.landTime = cg.time;
		}
		break;
	case EV_FALL_FAR:
		DEBUGNAME("EV_FALL_FAR");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound( es->number, "*fall1" ) );
		cent->pe.painTime = cg.time;	// don't play a pain sound right after this
		if ( clientNum == cg.predictedPlayerState.clientNum ) {
			// smooth landing z changes
			cg.landChange = -24;
			cg.landTime = cg.time;
		}
		break;

	case EV_STEP_4:
	case EV_STEP_8:
	case EV_STEP_12:
	case EV_STEP_16:		// smooth out step up transitions
		DEBUGNAME("EV_STEP");
	{
		float	oldStep;
		int		delta;
		int		step;

		if ( clientNum != cg.predictedPlayerState.clientNum ) {
			break;
		}
		// if we are interpolating, we don't need to smooth steps
		if ( cg.demoPlayback || (cg.snap->ps.pm_flags & PMF_FOLLOW) ||
			cg_nopredict.integer || cg_synchronousClients.integer ) {
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
		step = 4 * (event - EV_STEP_4 + 1 );
		cg.stepChange = oldStep + step;
		if ( cg.stepChange > MAX_STEP_CHANGE ) {
			cg.stepChange = MAX_STEP_CHANGE;
		}
		cg.stepTime = cg.time;
		break;
	}

	case EV_JUMP_PAD:
		DEBUGNAME("EV_JUMP_PAD");
//		CG_Printf( "EV_JUMP_PAD w/effect #%i\n", es->eventParm );
		{
			vec3_t			up = {0, 0, 1};


			CG_SmokePuff( cent->lerpOrigin, up, 
						  32, 
						  1, 1, 1, 0.33f,
						  1000, 
						  cg.time, 0,
						  LEF_PUFF_DONT_SCALE, 
						  cgs.media.smokePuffShader );
		}

		// boing sound at origin, jump sound on player
		trap_S_StartSound ( cent->lerpOrigin, -1, CHAN_VOICE, cgs.media.jumpPadSound );
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*jump1" ) );
		break;

	case EV_JUMP:
		DEBUGNAME("EV_JUMP");
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*jump1" ) );
		if(cg_entities[es->number].currentState.powerups & ( 1<< PW_JUMPER ))
			trap_S_StartSound (NULL, es->number, CHAN_ITEM, cgs.media.jumperSound);
		break;
	case EV_TAUNT:
		DEBUGNAME("EV_TAUNT");
		if(es->powerups & (1<<PW_SPEEDY) && VectorLengthSquared(es->pos.trDelta)>200.0f*200.0f)
			trap_S_StartSound (NULL, es->number, CHAN_VOICE, cgs.media.speedyTaunt );
		else
			trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*taunt" ) );
		break;
	case EV_HEHE1:
		DEBUGNAME("EV_HEHE1");
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*hehe1" ) );
		break;
	case EV_HEHE2:
		DEBUGNAME("EV_HEHE2");
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*hehe2" ) );
		break;
	case EV_WATER_TOUCH:
		DEBUGNAME("EV_WATER_TOUCH");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrInSound );
		break;
	case EV_WATER_LEAVE:
		DEBUGNAME("EV_WATER_LEAVE");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrOutSound );
		break;
	case EV_WATER_UNDER:
		DEBUGNAME("EV_WATER_UNDER");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrUnSound );
		ci->lastWaterClearTime = 0; // for wet-screen effect
		break;
	case EV_WATER_CLEAR:
		DEBUGNAME("EV_WATER_CLEAR");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound( es->number, "*gasp" ) );
		ci->lastWaterClearTime = cg.time; // for wet-screen effect
		break;

	case EV_ITEM_PICKUP:
		DEBUGNAME("EV_ITEM_PICKUP");
		{
			gitem_t	*item;
			int		index;

			index = es->eventParm;		// player predicted

			if ( index < 1 || index >= bg_numItems ) {
				break;
			}
			item = &bg_itemlist[ index ];

			// powerups and team items will have a separate global sound, this one
			// will be played at prediction time
			if ( item->giType == IT_POWERUP) {
				trap_S_StartSound (NULL, es->number, CHAN_AUTO,	cgs.media.n_healthSound );
			} else if(item->giType == IT_TEAM) {
				trap_S_StartSound (NULL, es->number, CHAN_AUTO,	cgs.media.stolenlollySound );
			} else if (item->giType == IT_PERSISTANT_POWERUP) {
			} else {
				if(item->giType==IT_WEAPON)
					trap_S_StartSound(NULL,es->number,CHAN_AUTO,cgs.media.pickupSound);
				if(item->giType!=IT_WEAPON || !(cg.snap->ps.stats[STAT_WEAPONS] & (1<<item->giTag)) )
				{
					trap_S_StartSound (NULL, es->number, CHAN_AUTO,	trap_S_RegisterSound( item->pickup_sound, qfalse ) );
					if(item->giType==IT_HOLDABLE)
						trap_S_StartSound(NULL,es->number,CHAN_AUTO,cgs.media.HIpickupSound);
					else if(!Q_stricmp(item->classname,"item_armor_padshield"))
						trap_S_StartSound(NULL,es->number,CHAN_AUTO,cgs.media.ARpickupSound);
				}
			}

			// show icon and name on status bar
			if ( es->number == cg.snap->ps.clientNum ) {
				CG_ItemPickup( index );
			}
		}
		break;

	case EV_GLOBAL_ITEM_PICKUP:
		DEBUGNAME("EV_GLOBAL_ITEM_PICKUP");
		{
			gitem_t	*item;
			int		index;

			index = es->eventParm;		// player predicted

			if ( index < 1 || index >= bg_numItems ) {
				break;
			}
			item = &bg_itemlist[ index ];
			// powerup pickups are global
			if( item->pickup_sound ) {
				trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, trap_S_RegisterSound( item->pickup_sound, qfalse ) );
			}

			// show icon and name on status bar
			if ( es->number == cg.snap->ps.clientNum ) {
				CG_ItemPickup( index );
			}
		}
		break;

	//
	// weapon events
	//
	case EV_NOAMMO:
		DEBUGNAME("EV_NOAMMO");
//		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.noAmmoSound );
		if ( es->number == cg.snap->ps.clientNum ) {
			CG_OutOfAmmoChange();
		}
		break;
	case EV_CHANGE_WEAPON:
		DEBUGNAME("EV_CHANGE_WEAPON");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.selectSound );
		break;
	case EV_FIRE_WEAPON:
		DEBUGNAME("EV_FIRE_WEAPON");
		CG_FireWeapon( cent );
		break;
	case EV_IMPERIUS_EXPLODE:
		DEBUGNAME("EV_IMPERIUS_EXPLODE");
		ByteToDir( es->eventParm, dir );
		CG_MissileHitWall( es->weapon, 0, position, dir, cent->currentState.generic1, IMPACTSOUND_DEFAULT );
		break;
	case EV_USE_ITEM0:
		DEBUGNAME("EV_USE_ITEM0");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM1:
		DEBUGNAME("EV_USE_ITEM1");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM2:
		DEBUGNAME("EV_USE_ITEM2");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM3:
		DEBUGNAME("EV_USE_ITEM3");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM4:
		DEBUGNAME("EV_USE_ITEM4");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM5:
		DEBUGNAME("EV_USE_ITEM5");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM6:
		DEBUGNAME("EV_USE_ITEM6");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM7:
		DEBUGNAME("EV_USE_ITEM7");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM8:
		DEBUGNAME("EV_USE_ITEM8");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM9:
		DEBUGNAME("EV_USE_ITEM9");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM10:
		DEBUGNAME("EV_USE_ITEM10");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM11:
		DEBUGNAME("EV_USE_ITEM11");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM12:
		DEBUGNAME("EV_USE_ITEM12");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM13:
		DEBUGNAME("EV_USE_ITEM13");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM14:
		DEBUGNAME("EV_USE_ITEM14");
		CG_UseItem( cent );
		break;

	//=================================================================

	//
	// other events
	//
	case EV_PLAYER_TELEPORT_IN:
		DEBUGNAME("EV_PLAYER_TELEPORT_IN");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleInSound );
//		CG_SpawnEffect( position, 0 );
		ci->lastTeleInTime = cg.time;
		break;

	case EV_PLAYER_TELEPORT_OUT:
		DEBUGNAME("EV_PLAYER_TELEPORT_OUT");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleOutSound );
//		CG_SpawnEffect(  position, 0 );
		CG_TeleOutEffect(position, 0, cent);
		break;

	case EV_PLAYER_TELEPORT_RED_IN:
		DEBUGNAME("EV_PLAYER_TELEPORT_RED_IN");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleInSound );
//		CG_SpawnEffect( position, 1 );
		ci->lastTeleInTime = cg.time;
		break;

	case EV_PLAYER_TELEPORT_RED_OUT:
		DEBUGNAME("EV_PLAYER_TELEPORT_RED_OUT");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleOutSound );
//		CG_SpawnEffect(  position, 1 );
		CG_TeleOutEffect(position, 1, cent);
		break;

	case EV_PLAYER_TELEPORT_BLUE_IN:
		DEBUGNAME("EV_PLAYER_TELEPORT_BLUE_IN");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleInSound );
//		CG_SpawnEffect( position, 2 );
		ci->lastTeleInTime = cg.time;
		break;

	case EV_PLAYER_TELEPORT_BLUE_OUT:
		DEBUGNAME("EV_PLAYER_TELEPORT_BLUE_OUT");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleOutSound );
//		CG_SpawnEffect(  position, 2 );
		CG_TeleOutEffect(position, 2, cent);
		break;

	case EV_DROP_CARTRIDGE:
		DEBUGNAME("EV_DROP_CARTRIDGE");
		trap_S_StartSound(NULL,es->number,CHAN_AUTO, cgs.media.DropCartridgeSound);
		break;

	case EV_ITEM_POP:
		DEBUGNAME("EV_ITEM_POP");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.respawnSound );
		break;
	case EV_ITEM_RESPAWN:
		DEBUGNAME("EV_ITEM_RESPAWN");
		cent->miscTime = cg.time;	// scale up from this
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.respawnSound );
		break;

	case EV_GRENADE_BOUNCE:
		DEBUGNAME("EV_GRENADE_BOUNCE");
		if ( rand() & 1 ) {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.hgrenb1aSound );
		} else {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.hgrenb2aSound );
		}
		break;

	case EV_STICKY_BOUNCE:
		DEBUGNAME("EV_STICKY_BOUNCE");
		ByteToDir( es->eventParm, cent->beamEnd );
		cent->miscTime = cg.time;
//grenad_bounce vv  ... brauch ich das //noch ?
		if ( rand() & 1 ) {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.hgrenb1aSound );
		} else {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.hgrenb2aSound );
		}
//grenad_bounce ^^
		break;

	case EV_SCOREPLUM:
		DEBUGNAME("EV_SCOREPLUM");
		CG_ScorePlum( cent->currentState.otherEntityNum, cent->lerpOrigin, cent->currentState.time );
		break;

	//
	// missile impacts
	//
	case EV_MISSILE_HIT:
		DEBUGNAME("EV_MISSILE_HIT");
		ByteToDir( es->eventParm, dir );
		//HERBY: Betty and Splasher Trail fix
		memcpy(&es->pos,&es->apos,sizeof(trajectory_t));
		wi = &cg_weapons[es->weapon];
		if ( wi->missileTrailFunc ) wi->missileTrailFunc( cent, wi );
		CG_MissileHitPlayer( es->weapon, position, dir, cent->currentState.generic1, es->otherEntityNum );
		break;

	case EV_MISSILE_MISS:
		DEBUGNAME("EV_MISSILE_MISS");
		ByteToDir( es->eventParm, dir );
		//HERBY: Betty and Splasher Trail fix
		memcpy(&es->pos,&es->apos,sizeof(trajectory_t));
		wi = &cg_weapons[es->weapon];
		if ( wi->missileTrailFunc ) wi->missileTrailFunc( cent, wi );
		CG_MissileHitWall( es->weapon, 0, position, dir, cent->currentState.generic1, IMPACTSOUND_DEFAULT );
		break;

	case EV_MISSILE_MISS_METAL:
		DEBUGNAME("EV_MISSILE_MISS_METAL");
		ByteToDir( es->eventParm, dir );
		//HERBY: Betty and Splasher Trail fix
		memcpy(&es->pos,&es->apos,sizeof(trajectory_t));
		wi = &cg_weapons[es->weapon];
		if ( wi->missileTrailFunc ) wi->missileTrailFunc( cent, wi );
		CG_MissileHitWall( es->weapon, 0, position, dir, cent->currentState.generic1, IMPACTSOUND_METAL );
		break;

	case EV_RAILTRAIL:
		DEBUGNAME("EV_RAILTRAIL");
		cent->currentState.weapon = WP_PUMPER; //damit herbys code funzt ;)
		
		if(es->clientNum == cg.snap->ps.clientNum && !cg.renderingThirdPerson)
		{
			if(cg_drawGun.integer == 2)
				VectorMA(es->origin2, 8, cg.refdef.viewaxis[1], es->origin2);
			else if(cg_drawGun.integer == 3)
				VectorMA(es->origin2, 4, cg.refdef.viewaxis[1], es->origin2);
		}

		CG_RailTrail(ci, es->origin2, es->pos.trBase);

		// if the end was on a nomark surface, don't make an explosion
		if ( es->eventParm != 255 ) {
			ByteToDir( es->eventParm, dir );
			CG_MissileHitWall( es->weapon, es->clientNum, position, dir, cent->currentState.generic1, IMPACTSOUND_DEFAULT );//HERBY
		}
		break;

	case EV_SHOTGUN:
		DEBUGNAME("EV_SHOTGUN");
		CG_ShotgunFire( es );
		break;

	case EV_GENERAL_SOUND:
		DEBUGNAME("EV_GENERAL_SOUND");
		if ( cgs.gameSounds[ es->eventParm ] ) {
			trap_S_StartSound (NULL, es->number, CHAN_VOICE, cgs.gameSounds[ es->eventParm ] );
		} else {
			s = CG_ConfigString( CS_SOUNDS + es->eventParm );
			trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, s ) );
		}
		break;

	case EV_SPRAYLOGO:
		DEBUGNAME( "EV_SPRAYLOGO" );

		// Hit a surface
		if ( es->eventParm != 255 ) {
			vec4_t		color;
			int			radius = ( 32 * es->angles[0] ); // Scale by distance
			qhandle_t	logohandle;

			ByteToDir( es->eventParm, dir );

			switch ( ci->team ) {
				case TEAM_RED:
					VectorSet( color, 1.0, 0.0, 0.0 );
					break;
				case TEAM_BLUE:
					VectorSet( color, 0.0, 0.0, 1.0 );
					break;
				default:
					VectorCopy( ci->color2, color );
					break;
			}
			color[3] = 1.0;

			// Hit a spraywall
			if ( es->generic1 == 0x23 ) {
				logohandle = FindLogoForSpraying( ci );
				Add_LogoToDrawList( es->pos.trBase, dir, logohandle, radius, ci );
			}
			else {
				CG_ImpactMark( cgs.media.spraymark, es->pos.trBase, dir, ( random() * 360 ),
				               color[0], color[1], color[2], color[3], qfalse, radius, qfalse );
			}
		}

		break;

	case EV_GLOBAL_SOUND:	// play from the player's head so it never diminishes
		DEBUGNAME("EV_GLOBAL_SOUND");
		if ( cgs.gameSounds[ es->eventParm ] ) {
			trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, cgs.gameSounds[ es->eventParm ] );
		} else {
			s = CG_ConfigString( CS_SOUNDS + es->eventParm );
			trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, CG_CustomSound( es->number, s ) );
		}
		break;

	case EV_GLOBAL_TEAM_SOUND:	// play from the player's head so it never diminishes
		{
			DEBUGNAME("EV_GLOBAL_TEAM_SOUND");
			switch( es->eventParm ) {
				case GTS_RED_CAPTURE: // CTF: red team captured the blue flag, 1FCTF: red team captured the neutral flag
					if ( cgs.clientinfo[cg.snap->ps.clientNum].team == TEAM_RED )
						CG_AddBufferedSound( cgs.media.captureYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.captureOpponentSound );
					break;
				case GTS_BLUE_CAPTURE: // CTF: blue team captured the red flag, 1FCTF: blue team captured the neutral flag
					if ( cgs.clientinfo[cg.snap->ps.clientNum].team == TEAM_BLUE )
						CG_AddBufferedSound( cgs.media.captureYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.captureOpponentSound );
					break;
				case GTS_RED_RETURN: // CTF: blue flag returned, 1FCTF: never used
					if ( cgs.clientinfo[cg.snap->ps.clientNum].team == TEAM_RED )
						CG_AddBufferedSound( cgs.media.returnYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.returnOpponentSound );
					// "voc_"-sound:
					CG_AddBufferedSound( cgs.media.blueFlagReturnedSound );
					break;
				case GTS_BLUE_RETURN: // CTF red flag returned, 1FCTF: neutral flag returned
					if ( cgs.clientinfo[cg.snap->ps.clientNum].team == TEAM_BLUE )
						CG_AddBufferedSound( cgs.media.returnYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.returnOpponentSound );
					// "voc_"-sound:
					CG_AddBufferedSound( cgs.media.redFlagReturnedSound );
					break;

				case GTS_RED_TAKEN: // CTF: red team took blue flag, 1FCTF: blue team took the neutral flag
					// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
					if (cg.snap->ps.powerups[PW_BLUEFLAG]) {
					}
					else {
						if (cgs.clientinfo[cg.snap->ps.clientNum].team == TEAM_BLUE) {
							CG_AddBufferedSound( cgs.media.takenOpponentSound );
							// "voc_"-sound:
						 	CG_AddBufferedSound( cgs.media.enemyTookYourFlagSound );
						}
						else if (cgs.clientinfo[cg.snap->ps.clientNum].team == TEAM_RED) {
							CG_AddBufferedSound( cgs.media.takenYourTeamSound );
							// "voc_"-sound:
 							CG_AddBufferedSound( cgs.media.yourTeamTookEnemyFlagSound );
						}
					}
					break;
				case GTS_BLUE_TAKEN: // CTF: blue team took the red flag, 1FCTF red team took the neutral flag
					// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
					if (cg.snap->ps.powerups[PW_REDFLAG]) {
					}
					else {
						if (cgs.clientinfo[cg.snap->ps.clientNum].team == TEAM_RED) {
							CG_AddBufferedSound( cgs.media.takenOpponentSound );
							// "voc_"-sound:
							CG_AddBufferedSound( cgs.media.enemyTookYourFlagSound );
						}
						else if (cgs.clientinfo[cg.snap->ps.clientNum].team == TEAM_BLUE) {
							CG_AddBufferedSound( cgs.media.takenYourTeamSound );
							// "voc_"-sound:
							CG_AddBufferedSound( cgs.media.yourTeamTookEnemyFlagSound );
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
					CG_AddBufferedSound(cgs.media.redLeadsSound[(int)(2.9999f*random())]);
					break;
				case GTS_BLUETEAM_TOOK_LEAD:
					CG_AddBufferedSound(cgs.media.blueLeadsSound[(int)(2.9999f*random())]);
					break;

				case GTS_TEAMS_ARE_TIED:
					CG_AddBufferedSound( cgs.media.teamsTiedSound );
					break;
				default:
					break;
			}
			break;
		}

	case EV_PAIN:
		// local player sounds are triggered in CG_CheckLocalSounds,
		// so ignore events on the player
		DEBUGNAME("EV_PAIN");
		if ( cent->currentState.number != cg.snap->ps.clientNum ) {
			CG_PainEvent( cent, es->eventParm );
		}
		break;

	case EV_DEATH1:
	case EV_DEATH2:
	case EV_DEATH3:
		DEBUGNAME("EV_DEATHx");
		trap_S_StartSound( NULL, es->number, CHAN_VOICE, 
				CG_CustomSound( es->number, va("*death%i", event - EV_DEATH1 + 1) ) );
		break;


	case EV_OBITUARY:
		DEBUGNAME("EV_OBITUARY");
		CG_Obituary( es );
		break;

	//
	// powerup events
	//
	case EV_POWERUP_REVIVAL:
		DEBUGNAME("EV_POWERUP_REVIVAL");
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_REVIVAL;
			cg.powerupTime = cg.time;
		}
		{
			static int lastrevivalsound = 0;
			if ( ( lastrevivalsound + 800 ) <= cg.time ) {
				trap_S_StartSound( NULL, es->number, CHAN_ITEM, cgs.media.regenSound );
				lastrevivalsound = cg.time;
			}
		}
		break;
	case EV_POWERUP_PADPOWER:
		DEBUGNAME("EV_POWERUP_PADPOWER");
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_PADPOWER;
			cg.powerupTime = cg.time;
		}
		trap_S_StartSound (NULL, es->number, CHAN_ITEM, cgs.media.quadSound );
		break;

	case EV_GIB_PLAYER:
		DEBUGNAME("EV_GIB_PLAYER");
		// don't play gib sound when using the kamikaze because it interferes
		// with the kamikaze sound, downside is that the gib sound will also
		// not be played when someone is gibbed while just carrying the kamikaze
//		CG_GibPlayer( cent->lerpOrigin );
		break;

	case EV_STOPLOOPINGSOUND:
		DEBUGNAME("EV_STOPLOOPINGSOUND");
		trap_S_StopLoopingSound( es->number );
		es->loopSound = 0;
		break;

	case EV_BAMBAM_EXPLOSION:
		DEBUGNAME("EV_BAMBAM_EXPLOSION");
		CG_BamBam_Explosion(position);
		break;
	case EV_BOOMIES_EXPLOSION:
		DEBUGNAME("EV_BOOMIES_EXPLOSION");
		CG_Boomies_Explosion(position);
		break;

	case EV_DEBUG_LINE:
		DEBUGNAME("EV_DEBUG_LINE");
		CG_Beam( cent );
		break;
	default:
		DEBUGNAME("UNKNOWN");
		CG_Error( "Unknown event: %i", event );
		break;
	}
}


/*
==============
CG_CheckEvents

==============
*/
void CG_CheckEvents( centity_t *cent ) {
	// check for event-only entities
	if ( cent->currentState.eType > ET_EVENTS ) {
		if ( cent->previousEvent ) {
			return;	// already fired
		}
		// if this is a player event set the entity number of the client entity number
		if ( cent->currentState.eFlags & EF_PLAYER_EVENT ) {
			cent->currentState.number = cent->currentState.otherEntityNum;
		}

		cent->previousEvent = 1;

		cent->currentState.event = cent->currentState.eType - ET_EVENTS;
	} else {
		// check for events riding with another entity
		if ( cent->currentState.event == cent->previousEvent ) {
			return;
		}
		cent->previousEvent = cent->currentState.event;
		if ( ( cent->currentState.event & ~EV_EVENT_BITS ) == 0 ) {
			return;
		}
	}

	// calculate the position at exactly the frame time
	BG_EvaluateTrajectory( &cent->currentState.pos, cg.snap->serverTime, cent->lerpOrigin );
	CG_SetEntitySoundPosition( cent );

	CG_EntityEvent( cent, cent->lerpOrigin );
}

