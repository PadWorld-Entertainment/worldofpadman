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
#include "g_spawn.h"
#include "g_team.h"

typedef struct teamgame_s {
	float last_flag_capture;
	int last_capture_team;
	flagStatus_t redStatus;	 // CTF
	flagStatus_t blueStatus; // CTF
	flagStatus_t flagStatus; // One Flag CTF
	int redTakenTime;
	int blueTakenTime;
	int redObeliskAttackedTime;
	int blueObeliskAttackedTime;
} teamgame_t;

teamgame_t teamgame;

gentity_t *neutralObelisk;

static void Team_SetFlagStatus(int team, flagStatus_t status);

void Team_InitGame(void) {
	memset(&teamgame, 0, sizeof teamgame);

	switch (g_gametype.integer) {
	case GT_CTF:
		teamgame.redStatus = -1; // Invalid to force update
		Team_SetFlagStatus(TEAM_RED, FLAG_ATBASE);
		teamgame.blueStatus = -1;
		Team_SetFlagStatus(TEAM_BLUE, FLAG_ATBASE);
		break;
	case GT_1FCTF:
		teamgame.flagStatus = -1; // Invalid to force update
		Team_SetFlagStatus(TEAM_FREE, FLAG_ATBASE);
		break;
	default:
		break;
	}
}

int OtherTeam(int team) {
	if (team == TEAM_RED)
		return TEAM_BLUE;
	else if (team == TEAM_BLUE)
		return TEAM_RED;
	return team;
}

const char *TeamName(int team) {
	if (team == TEAM_RED)
		return "RED PADS";
	else if (team == TEAM_BLUE)
		return "BLUE NOSES";
	else if (team == TEAM_SPECTATOR)
		return "SPECTATOR";
	return "FREE";
}

const char *TeamColorString(int team) {
	if (team == TEAM_RED)
		return S_COLOR_RED;
	else if (team == TEAM_BLUE)
		return S_COLOR_BLUE;
	else if (team == TEAM_SPECTATOR)
		return S_COLOR_YELLOW;
	return S_COLOR_WHITE;
}

// NULL for everyone
void QDECL PrintMsg(const gentity_t *ent, const char *fmt, ...) {
	char msg[1024];
	va_list argptr;
	char *p;

	va_start(argptr, fmt);
	if (Q_vsnprintf(msg, sizeof(msg), fmt, argptr) >= sizeof(msg)) {
		G_Error("PrintMsg overrun");
	}
	va_end(argptr);

	// double quotes are bad
	while ((p = strchr(msg, '"')) != NULL)
		*p = '\'';

	trap_SendServerCommand(((ent == NULL) ? -1 : ent - g_entities), va("print \"%s\"", msg));
}

/*
==============
AddTeamScore

 used for gametype > GT_TEAM
 for gametype GT_TEAM the level.teamScores is updated in AddScore in g_combat.c
==============
*/
// TODO: Draw a ScorePlum() as well? Needs cgame fixes to draw plum regardless of "owner"
void AddTeamScore(const vec3_t origin, int team, int score, char *reason) {
	gentity_t *te;

	te = G_TempEntity(origin, EV_GLOBAL_TEAM_SOUND);
	te->r.svFlags |= SVF_BROADCAST;

	if (team == TEAM_RED) {
		if (level.teamScores[TEAM_RED] + score == level.teamScores[TEAM_BLUE]) {
			// teams are tied sound
			te->s.eventParm = GTS_TEAMS_ARE_TIED;
		} else if (level.teamScores[TEAM_RED] <= level.teamScores[TEAM_BLUE] &&
				   level.teamScores[TEAM_RED] + score > level.teamScores[TEAM_BLUE]) {
			// red took the lead sound
			te->s.eventParm = GTS_REDTEAM_TOOK_LEAD;
		} else {
			// red scored sound
			te->s.eventParm = GTS_REDTEAM_SCORED;

			// Hackity! In BB teams score continously, which makes for annoying
			// sound spam. Thus disable sound.
			// Better solution would be to either only do this sound on captures/destroys
			// or continously at fixed time offsets.
			// Also note that there already is a "blue/red balloon" sound in cgame CG_UpdateBalloonStates
			if (g_gametype.integer == GT_BALLOON) {
				G_FreeEntity(te);
			}
		}
	} else {
		if (level.teamScores[TEAM_BLUE] + score == level.teamScores[TEAM_RED]) {
			// teams are tied sound
			te->s.eventParm = GTS_TEAMS_ARE_TIED;
		} else if (level.teamScores[TEAM_BLUE] <= level.teamScores[TEAM_RED] &&
				   level.teamScores[TEAM_BLUE] + score > level.teamScores[TEAM_RED]) {
			// blue took the lead sound
			te->s.eventParm = GTS_BLUETEAM_TOOK_LEAD;
		} else {
			// blue scored sound
			te->s.eventParm = GTS_BLUETEAM_SCORED;

			// See note above
			if (g_gametype.integer == GT_BALLOON) {
				G_FreeEntity(te);
			}
		}
	}
	level.teamScores[team] += score;

	CalculateRanks();

	G_LogPrintf("AddTeamScore: %s %i %s\n", TeamName(team), score, reason);
}

/*
==============
OnSameTeam
==============
*/
qboolean OnSameTeam(const gentity_t *ent1, const gentity_t *ent2) {
	if (!ent1->client || !ent2->client) {
		return qfalse;
	}

	if (g_gametype.integer < GT_TEAM) {
		return qfalse;
	}

	if (ent1->client->sess.sessionTeam == ent2->client->sess.sessionTeam) {
		return qtrue;
	}

	return qfalse;
}

static char ctfFlagStatusRemap[] = {'0', '1', '*', '*', '2'};
static char oneFlagStatusRemap[] = {'0', '1', '2', '3', '4'};

static void Team_SetFlagStatus(int team, flagStatus_t status) {
	qboolean modified = qfalse;

	switch (team) {
	case TEAM_RED: // CTF
		if (teamgame.redStatus != status) {
			teamgame.redStatus = status;
			modified = qtrue;
		}
		break;

	case TEAM_BLUE: // CTF
		if (teamgame.blueStatus != status) {
			teamgame.blueStatus = status;
			modified = qtrue;
		}
		break;

	case TEAM_FREE: // One Flag CTF
		if (teamgame.flagStatus != status) {
			teamgame.flagStatus = status;
			modified = qtrue;
		}
		break;
	}

	if (modified) {
		char st[4];

		if (g_gametype.integer == GT_CTF) {
			st[0] = ctfFlagStatusRemap[teamgame.redStatus];
			st[1] = ctfFlagStatusRemap[teamgame.blueStatus];
			st[2] = 0;
		} else { // GT_1FCTF
			st[0] = oneFlagStatusRemap[teamgame.flagStatus];
			st[1] = 0;
		}

		trap_SetConfigstring(CS_FLAGSTATUS, st);
	}
}

int Team_GetFlagStatus(int team) {
	switch (team) {
	case TEAM_RED: // CTF
		return teamgame.redStatus;

	case TEAM_BLUE:
		return teamgame.blueStatus;

	default:
		return 0;
	}
}

void Team_CheckDroppedItem(const gentity_t *dropped) {
	if (dropped->item->giTag == PW_REDFLAG) {
		Team_SetFlagStatus(TEAM_RED, FLAG_DROPPED);
	} else if (dropped->item->giTag == PW_BLUEFLAG) {
		Team_SetFlagStatus(TEAM_BLUE, FLAG_DROPPED);
	} else if (dropped->item->giTag == PW_NEUTRALFLAG) {
		Team_SetFlagStatus(TEAM_FREE, FLAG_DROPPED);
	}
}

#if 0
/*
================
Team_ForceGesture
================
*/
static void Team_ForceGesture(int team) {
	int i;
	gentity_t *ent;

	for (i = 0; i < MAX_CLIENTS; i++) {
		ent = &g_entities[i];
		if (!ent->inuse)
			continue;
		if (!ent->client)
			continue;
		if (ent->client->sess.sessionTeam != team)
			continue;
		//
		ent->flags |= FL_FORCE_GESTURE;
	}
}
#endif

void G_SetBalloonCaptured(int balloonIndex, team_t team, qboolean fullyCaptured) {
	if (team != TEAM_RED && team != TEAM_BLUE) {
		Com_Printf("Invalid team for capturing a balloon: %i\n", team);
		return;
	}
	if (balloonIndex < 0 || balloonIndex >= MAX_BALLOONS) {
		Com_Printf("Invalid balloon index given: %i\n", balloonIndex);
		return;
	}
	if (fullyCaptured) {
		level.balloonState[balloonIndex] = (char)('1' + team - 1);
	} else {
		level.balloonState[balloonIndex] = (char)('a' + team - 1);
	}
	// Com_Printf("balloon state: %s\n", level.balloonState);
}

qboolean G_BalloonIsCaptured(int balloonIndex, team_t team, qboolean fullyCaptured) {
	if (team != TEAM_RED && team != TEAM_BLUE) {
		Com_Printf("Invalid team for querying the balloon state: %i\n", team);
		return qfalse;
	}
	if (balloonIndex < 0 || balloonIndex >= MAX_BALLOONS) {
		Com_Printf("Invalid balloon index given: %i\n", balloonIndex);
		return qfalse;
	}
	if (fullyCaptured) {
		return level.balloonState[balloonIndex] == '1' + team - 1;
	}
	return level.balloonState[balloonIndex] == 'a' + team - 1;
}

/**
 * Checks to hand out a pad hero award to the attacker.
 *
 * @return @c true if the attacker fragged the victim close to a balloon zone that belongs to the attacker's team
 */
static qboolean Team_BalloonDefendOrProtectBonus(const gentity_t *targ, gentity_t *attacker) {
	gentity_t *balloon = NULL;
	gentity_t *triggers[MAX_BALLOONS];
	int balloonCount = 0;

	if (attacker->client->sess.sessionTeam == targ->client->sess.sessionTeam) {
		return qfalse;
	}

	while ((balloon = G_Find(balloon, FOFS(classname), "trigger_balloonzone"))) {
		if (balloonCount >= MAX_BALLOONS) {
			break;
		}
		triggers[balloonCount++] = balloon;
	}

	while ((balloon = G_FindRadius(balloon, FOFS(classname), "target_balloon", targ->r.currentOrigin, BALLOON_TARGET_PROTECT_RADIUS))) {
		int i;
		for (i = 0; i < balloonCount; ++i) {
			// find the related trigger zone for the balloon
			if (triggers[i]->target_ent != balloon) {
				continue;
			}
			// if the balloon is already fully captured by your team, and you fragged the player from the other team
			// you get the award.
			if (G_BalloonIsCaptured(triggers[i]->count, attacker->client->sess.sessionTeam, qtrue)) {
				AddScore(attacker, attacker->r.currentOrigin, BB_DEFENSE_BONUS, SCORE_BONUS_DEFENSE_S);
				attacker->client->ps.persistant[PERS_PADHERO_COUNT]++;
				// add the sprite over the player's head
				SetAward(attacker->client, AWARD_PADHERO);
				return qtrue;
			}
		}
	}
	return qfalse;
}

static qboolean Team_FlagDefendOrProtectBonus(const gentity_t *targ, gentity_t *attacker, int flag_pw) {
	gentity_t *flag = NULL;
	gentity_t *carrier = NULL;
	const char *c;
	int i;

	// we have to find the flag and carrier entities
	// find the flag
	switch (attacker->client->sess.sessionTeam) {
	case TEAM_RED:
		c = "team_CTL_redlolly";
		break;
	case TEAM_BLUE:
		c = "team_CTL_bluelolly";
		break;
	default:
		return qfalse;
	}
	// find attacker's team's flag carrier
	for (i = 0; i < g_maxclients.integer; i++) {
		carrier = g_entities + i;
		if (carrier->inuse && carrier->client->ps.powerups[flag_pw]) {
			break;
		}
		carrier = NULL;
	}
	while ((flag = G_Find(flag, FOFS(classname), c)) != NULL) {
		if (!(flag->flags & FL_DROPPED_ITEM)) {
			break;
		}
	}

	if (!flag) {
		return qfalse; // can't find attacker's flag
	}

	// ok we have the attackers flag and a pointer to the carrier

	if (attacker->client->sess.sessionTeam != targ->client->sess.sessionTeam) {
		vec3_t v1, v2;
		// check to see if we are defending the base's flag
		VectorSubtract(targ->r.currentOrigin, flag->r.currentOrigin, v1);
		VectorSubtract(attacker->r.currentOrigin, flag->r.currentOrigin, v2);
		if (((VectorLength(v1) < CTF_TARGET_PROTECT_RADIUS &&
			  trap_InPVS(flag->r.currentOrigin, targ->r.currentOrigin)) ||
			 (VectorLength(v2) < CTF_TARGET_PROTECT_RADIUS &&
			  trap_InPVS(flag->r.currentOrigin, attacker->r.currentOrigin)))) {

			// we defended the base flag
			AddScore(attacker, targ->r.currentOrigin, CTF_FLAG_DEFENSE_BONUS, SCORE_BONUS_DEFENSE_S);
			attacker->client->pers.teamState.basedefense++;

			attacker->client->ps.persistant[PERS_PADHERO_COUNT]++;
			// add the sprite over the player's head
			SetAward(attacker->client, AWARD_PADHERO);

			return qtrue;
		}
	}

	if (carrier && carrier != attacker) {
		vec3_t v1, v2;
		VectorSubtract(targ->r.currentOrigin, carrier->r.currentOrigin, v1);
		VectorSubtract(attacker->r.currentOrigin, carrier->r.currentOrigin, v2);

		if (((VectorLength(v1) < CTF_ATTACKER_PROTECT_RADIUS &&
			  trap_InPVS(carrier->r.currentOrigin, targ->r.currentOrigin)) ||
			 (VectorLength(v2) < CTF_ATTACKER_PROTECT_RADIUS &&
			  trap_InPVS(carrier->r.currentOrigin, attacker->r.currentOrigin))) &&
			attacker->client->sess.sessionTeam != targ->client->sess.sessionTeam) {
			AddScore(attacker, targ->r.currentOrigin, CTF_CARRIER_PROTECT_BONUS, SCORE_BONUS_CARRIER_PROTECT_S);
			attacker->client->pers.teamState.carrierdefense++;

			attacker->client->ps.persistant[PERS_PADHERO_COUNT]++;
			// add the sprite over the player's head
			SetAward(attacker->client, AWARD_PADHERO);

			return qtrue;
		}
	}
	return qfalse;
}

static void G_ResetLastHurtCarrier(gentity_t *ent) {
	playerTeamState_t *teamState = &ent->client->pers.teamState;
	teamState->lasthurtcarrier = 0;
	teamState->lasthurtcarrierId = -1;
}

/**
 * @brief Checks if the attacker is eligible for getting a defense bonus for killing the victim
 */
static qboolean G_IsEligibleForCarrierDefense(const gentity_t *victim, const gentity_t *attacker) {
	const playerTeamState_t *teamState = &victim->client->pers.teamState;

	// the victim doesn't hurt the carrier
	if (teamState->lasthurtcarrier == 0) {
		return qfalse;
	}
	// the victim hurts the carrier, but the attacker is not eligible because this didn't happened recent enough
	if (level.time - teamState->lasthurtcarrier >= CTF_CARRIER_DANGER_PROTECT_TIMEOUT) {
		return qfalse;
	}

	if (g_gametype.integer == GT_CTF) {
		int flag_pw;
		// same team, if the flag at base, check to he has the enemy flag
		if (victim->client->sess.sessionTeam == TEAM_RED) {
			flag_pw = PW_REDFLAG;
		} else {
			flag_pw = PW_BLUEFLAG;
		}
		// if you are the carrier yourself, don't give bonus
		if (attacker->client->ps.powerups[flag_pw]) {
			return qfalse;
		}
	}
	// if the last attacked carrier was the attacker itself, don't give a bonus
	if (attacker->s.number == teamState->lasthurtcarrierId) {
		return qfalse;
	}
	return qtrue;
}

/*
================
Team_FragBonuses

Calculate the bonuses for flag defense, flag/cartridge carrier defense, etc.
Note that bonuses are not cumulative. You get one, they are in importance
order.
================
*/
void Team_FragBonuses(gentity_t *victim, gentity_t *attacker) {
	int i;
	int flag_pw, enemy_flag_pw;
	int otherteam;
	int victimTeam;

	// no bonus for non team game types
	if (g_gametype.integer < GT_TEAM) {
		return;
	}

	// no bonus for fragging yourself or team mates
	if (!victim->client || !attacker->client || victim == attacker || OnSameTeam(victim, attacker))
		return;

	victimTeam = victim->client->sess.sessionTeam;
	otherteam = OtherTeam(victim->client->sess.sessionTeam);
	if (otherteam < 0)
		return; // whoever died isn't on a team

	// same team, if the flag at base, check to he has the enemy flag
	if (victimTeam == TEAM_RED) {
		flag_pw = PW_REDFLAG;
		enemy_flag_pw = PW_BLUEFLAG;
	} else {
		flag_pw = PW_BLUEFLAG;
		enemy_flag_pw = PW_REDFLAG;
	}

	if (g_gametype.integer == GT_1FCTF) {
		flag_pw = PW_NEUTRALFLAG;
		enemy_flag_pw = PW_NEUTRALFLAG;
	} 

	// did the attacker frag the flag carrier?
	if (victim->client->ps.powerups[enemy_flag_pw]) {
		attacker->client->pers.teamState.lastfraggedcarrier = level.time;
		AddScore(attacker, victim->r.currentOrigin, CTF_FRAG_CARRIER_BONUS, SCORE_BONUS_FRAG_CARRIER_S);
		attacker->client->pers.teamState.fragcarrier++;
		PrintMsg(NULL, "%s" S_COLOR_WHITE " fragged %s' lolly carrier!\n", attacker->client->pers.netname,
				 TeamName(victimTeam));

		// the target had the flag, clear the hurt carrier
		// field on the other team
		for (i = 0; i < g_maxclients.integer; i++) {
			gentity_t *ent = g_entities + i;
			if (ent->inuse && ent->client->sess.sessionTeam == otherteam)
				G_ResetLastHurtCarrier(ent);
		}
		return;
	}

	// did the attacker frag a cartridge carrier?

	if (g_gametype.integer == GT_SPRAY) {
		int cartridges = victim->client->ps.generic1;
		if (cartridges) {
			attacker->client->pers.teamState.lastfraggedcarrier = level.time;
			AddScore(attacker, victim->r.currentOrigin, CTF_FRAG_CARRIER_BONUS * cartridges, SCORE_BONUS_FRAG_CARRIER_S);
			attacker->client->pers.teamState.fragcarrier++;
			PrintMsg(NULL, "%s" S_COLOR_WHITE " fragged %s' cartridge carrier!\n", attacker->client->pers.netname,
					 TeamName(victimTeam));

			// the target had the flag, clear the hurt carrier
			// field on the other team
			for (i = 0; i < g_maxclients.integer; i++) {
				gentity_t *ent = g_entities + i;
				if (ent->inuse && ent->client->sess.sessionTeam == otherteam)
					G_ResetLastHurtCarrier(ent);
			}
			return;
		}
	}

	// CTF and SyC
	if (G_IsEligibleForCarrierDefense(victim, attacker)) {
		// attacker is on the same team as the flag carrier and
		// fragged a guy who hurt our flag carrier
		AddScore(attacker, victim->r.currentOrigin, CTF_CARRIER_DANGER_PROTECT_BONUS, SCORE_BONUS_CARRIER_PROTECT_S);

		attacker->client->pers.teamState.carrierdefense++;
		G_ResetLastHurtCarrier(victim);

		attacker->client->ps.persistant[PERS_PADHERO_COUNT]++;
		// add the sprite over the player's head
		SetAward(attacker->client, AWARD_PADHERO);

		return;
	}

	// flag and flag carrier area defense bonuses
	if (g_gametype.integer == GT_CTF) {
		Team_FlagDefendOrProtectBonus(victim, attacker, flag_pw);
	} else if (g_gametype.integer == GT_BALLOON) {
		Team_BalloonDefendOrProtectBonus(victim, attacker);
	}
}

/*
================
Team_CheckHurtCarrier

Check to see if attacker hurt the flag/cartridge carrier.
Needed when handing out bonuses for assistance to flag/cartridge carrier defense.
================
*/
void Team_CheckHurtCarrier(const gentity_t *victim, gentity_t *attacker) {
	int flag_pw;
	playerTeamState_t *attackerTeamState;

	if (!victim->client || !attacker->client)
		return;

	if (victim->client->sess.sessionTeam == attacker->client->sess.sessionTeam)
		return;

	if (victim->client->sess.sessionTeam == TEAM_RED)
		flag_pw = PW_BLUEFLAG;
	else
		flag_pw = PW_REDFLAG;

	if (g_gametype.integer == GT_1FCTF) {
		flag_pw = PW_NEUTRALFLAG;
	}

	attackerTeamState = &attacker->client->pers.teamState;

	// flags CTF
	if (victim->client->ps.powerups[flag_pw]) {
		attackerTeamState->lasthurtcarrier = level.time;
		attackerTeamState->lasthurtcarrierId = victim->s.number;
	}

	// cartridges (5 or more cartridges only) SyC
	if (victim->client->ps.generic1 >= CNT_CARTRIDGES) {
		attackerTeamState->lasthurtcarrier = level.time;
		attackerTeamState->lasthurtcarrierId = victim->s.number;
	}
}

static const gentity_t *Team_ResetFlag(int team) {
	const char *c;
	gentity_t *ent;
	const gentity_t *rent = NULL;

	switch (team) {
	case TEAM_RED:
		c = "team_CTL_redlolly";
		break;
	case TEAM_BLUE:
		c = "team_CTL_bluelolly";
		break;
	case TEAM_FREE:
		c = "team_CTL_neutrallolly";
		break;
	default:
		return NULL;
	}

	ent = NULL;
	while ((ent = G_Find(ent, FOFS(classname), c)) != NULL) {
		if (ent->flags & FL_DROPPED_ITEM)
			G_FreeEntity(ent);
		else {
			rent = ent;
			RespawnItem(ent);
		}
	}

	Team_SetFlagStatus(team, FLAG_ATBASE);

	return rent;
}

static void Team_ResetFlags(void) {
	if (g_gametype.integer == GT_CTF) {
		Team_ResetFlag(TEAM_RED);
		Team_ResetFlag(TEAM_BLUE);
	} else if (g_gametype.integer == GT_1FCTF) {
		Team_ResetFlag(TEAM_FREE);
	}
}

static void Team_ReturnFlagSound(const gentity_t *ent, int team) {
	gentity_t *te;

	if (ent == NULL) {
		Com_Printf("Warning:  NULL passed to Team_ReturnFlagSound\n");
		return;
	}

	te = G_TempEntity(ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND);
	if (team == TEAM_BLUE) {
		te->s.eventParm = GTS_RED_RETURN;
	} else {
		te->s.eventParm = GTS_BLUE_RETURN;
	}
	te->r.svFlags |= SVF_BROADCAST;
}

static void Team_TakeFlagSound(const gentity_t *ent, int team) {
	gentity_t *te;

	if (ent == NULL) {
		Com_Printf("Warning:  NULL passed to Team_TakeFlagSound\n");
		return;
	}

	// only play sound when the flag was at the base
	// or not picked up the last 10 seconds
	switch (team) {
	case TEAM_RED:
		if (teamgame.blueStatus != FLAG_ATBASE) {
			if (teamgame.blueTakenTime > level.time - 10000)
				return;
		}
		teamgame.blueTakenTime = level.time;
		break;

	case TEAM_BLUE: // CTF
		if (teamgame.redStatus != FLAG_ATBASE) {
			if (teamgame.redTakenTime > level.time - 10000)
				return;
		}
		teamgame.redTakenTime = level.time;
		break;
	}

	te = G_TempEntity(ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND);
	if (team == TEAM_BLUE) {
		te->s.eventParm = GTS_RED_TAKEN;
	} else {
		te->s.eventParm = GTS_BLUE_TAKEN;
	}
	te->r.svFlags |= SVF_BROADCAST;
}

static void Team_CaptureFlagSound(const gentity_t *ent, int team) {
	gentity_t *te;

	if (ent == NULL) {
		Com_Printf("Warning:  NULL passed to Team_CaptureFlagSound\n");
		return;
	}

	te = G_TempEntity(ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND);
	if (team == TEAM_BLUE) {
		te->s.eventParm = GTS_BLUE_CAPTURE;
	} else {
		te->s.eventParm = GTS_RED_CAPTURE;
	}
	te->r.svFlags |= SVF_BROADCAST;
}

void Team_ReturnFlag(int team) {
	Team_ReturnFlagSound(Team_ResetFlag(team), team);
	if (team == TEAM_FREE) {
		PrintMsg(NULL, "The lolly has returned!\n");
	} else {
		PrintMsg(NULL, "The %s' lolly has returned!\n", TeamName(team));
	}
}

void Team_FreeEntity(const gentity_t *ent) {
	if (ent->item->giTag == PW_REDFLAG) {
		Team_ReturnFlag(TEAM_RED);
	} else if (ent->item->giTag == PW_BLUEFLAG) {
		Team_ReturnFlag(TEAM_BLUE);
	} else if (ent->item->giTag == PW_NEUTRALFLAG) {
		Team_ReturnFlag(TEAM_FREE);
	}
}

/*
==============
Team_DroppedFlagThink

Automatically set in Launch_Item if the item is one of the flags

Flags are unique in that if they are dropped, the base flag must be respawned when they time out
==============
*/
void Team_DroppedFlagThink(gentity_t *ent) {
	int team = TEAM_FREE;

	if (ent->item->giTag == PW_REDFLAG) {
		team = TEAM_RED;
	} else if (ent->item->giTag == PW_BLUEFLAG) {
		team = TEAM_BLUE;
	} else if (ent->item->giTag == PW_NEUTRALFLAG) {
		team = TEAM_FREE;
	}

	Team_ReturnFlagSound(Team_ResetFlag(team), team);
	// Reset Flag will delete this entity
}

/*
==============
Team_DroppedFlagThink
==============
*/
static int Team_TouchOurFlag(gentity_t *ent, gentity_t *other, int team) {
	int i;
	gclient_t *cl = other->client;
	int enemy_flag;

	if (g_gametype.integer == GT_1FCTF) {
		enemy_flag = PW_NEUTRALFLAG;
	} else {
		if (cl->sess.sessionTeam == TEAM_RED) {
			enemy_flag = PW_BLUEFLAG;
		} else {
			enemy_flag = PW_REDFLAG;
		}

		if (ent->flags & FL_DROPPED_ITEM) {
			// hey, it's not home.  return it by teleporting it back
			PrintMsg(NULL, "%s" S_COLOR_WHITE " returned the %s' lolly!\n", cl->pers.netname, TeamName(team));
			AddScore(other, ent->r.currentOrigin, CTF_RECOVERY_BONUS, SCORE_BONUS_RECOVERY_S);
			other->client->pers.teamState.flagrecovery++;
			other->client->pers.teamState.lastreturnedflag = level.time;
			// ResetFlag will remove this entity!  We must return zero
			Team_ReturnFlagSound(Team_ResetFlag(team), team);
			return 0;
		}
	}
	// the flag is at home base.  if the player has the enemy
	// flag, he's just won!
	if (!cl->ps.powerups[enemy_flag])
		return 0; // We don't have the flag
	if (g_gametype.integer == GT_1FCTF) {
		PrintMsg( NULL, "%s" S_COLOR_WHITE " captured the lolly!\n", cl->pers.netname);
	} else {
		PrintMsg(NULL, "%s" S_COLOR_WHITE " captured the %s' lolly!\n", cl->pers.netname, TeamName(OtherTeam(team)));
	}
	cl->ps.powerups[enemy_flag] = 0;

	teamgame.last_flag_capture = level.time;
	teamgame.last_capture_team = team;

	// Increase the team's score
	AddTeamScore(ent->s.pos.trBase, other->client->sess.sessionTeam, SCORE_CAPTURE, SCORE_BONUS_CAPTURE_S);
	//	Team_ForceGesture(other->client->sess.sessionTeam);

	other->client->pers.teamState.captures++;
	// add the sprite over the player's head
	SetAward(other->client, AWARD_CAP);

	other->client->ps.persistant[PERS_CAPTURES]++;

	// other gets another 10 frag bonus
	AddScore(other, ent->r.currentOrigin, CTF_CAPTURE_BONUS, SCORE_BONUS_CAPTURE_S);

	Team_CaptureFlagSound(ent, team);

	// Ok, let's do the player loop, hand out the bonuses
	for (i = 0; i < g_maxclients.integer; i++) {
		gentity_t *player = &g_entities[i];

		// also make sure we don't award assist bonuses to the flag carrier himself.
		if (!player->inuse || player == other)
			continue;

		if (player->client->sess.sessionTeam != cl->sess.sessionTeam) {
			player->client->pers.teamState.lasthurtcarrier = -5;
		} else if (player->client->sess.sessionTeam == cl->sess.sessionTeam) {
			// award extra points for capture assists
			if (player->client->pers.teamState.lastreturnedflag + CTF_RETURN_FLAG_ASSIST_TIMEOUT > level.time) {
				AddScore(player, ent->r.currentOrigin, CTF_RETURN_FLAG_ASSIST_BONUS, SCORE_BONUS_ASSIST_RETURN_S);
				other->client->pers.teamState.assists++;
				player->client->ps.persistant[PERS_PADACE_COUNT]++;
				// add the sprite over the player's head
				SetAward(player->client, AWARD_PADACE);
			}
			if (player->client->pers.teamState.lastfraggedcarrier + CTF_FRAG_CARRIER_ASSIST_TIMEOUT > level.time) {
				AddScore(player, ent->r.currentOrigin, CTF_FRAG_CARRIER_ASSIST_BONUS,
						 SCORE_BONUS_ASSIST_FRAG_CARRIER_S);
				other->client->pers.teamState.assists++;
				player->client->ps.persistant[PERS_PADACE_COUNT]++;
				// add the sprite over the player's head
				SetAward(player->client, AWARD_PADACE);
			}
		}
	}
	Team_ResetFlags();

	return 0; // Do not respawn this automatically
}

static int Team_TouchEnemyFlag(gentity_t *ent, gentity_t *other, int team) {
	gclient_t *cl = other->client;

	if (g_gametype.integer == GT_1FCTF) {
		PrintMsg (NULL, "%s" S_COLOR_WHITE " got the lolly!\n", other->client->pers.netname);

		cl->ps.powerups[PW_NEUTRALFLAG] = INT_MAX; // flags never expire

		if (team == TEAM_RED) {
			Team_SetFlagStatus(TEAM_FREE, FLAG_TAKEN_RED);
		} else {
			Team_SetFlagStatus(TEAM_FREE, FLAG_TAKEN_BLUE);
		}
	} else {
		PrintMsg(NULL, "%s" S_COLOR_WHITE " got the %s' lolly!\n", other->client->pers.netname, TeamName(team));

		if (team == TEAM_RED)
			cl->ps.powerups[PW_REDFLAG] = INT_MAX; // flags never expire
		else
			cl->ps.powerups[PW_BLUEFLAG] = INT_MAX; // flags never expire

		Team_SetFlagStatus(team, FLAG_TAKEN);
	}

	cl->pers.teamState.flagsince = level.time;
	Team_TakeFlagSound(ent, team);

	return -1; // Do not respawn this automatically, but do delete it if it was FL_DROPPED
}

int Pickup_Team(gentity_t *ent, gentity_t *other) {
	int team;
	gclient_t *cl = other->client;

	// figure out what team this flag is
	if (strcmp(ent->classname, "team_CTL_redlolly") == 0) {
		team = TEAM_RED;
	} else if (strcmp(ent->classname, "team_CTL_bluelolly") == 0) {
		team = TEAM_BLUE;
	} else if (strcmp(ent->classname, "team_CTL_neutrallolly") == 0) {
		team = TEAM_FREE;
	} else {
		PrintMsg(other, "Don't know what team the lolly is on.\n");
		return 0;
	}

	if (g_gametype.integer == GT_1FCTF) {
		if (team == TEAM_FREE) {
			return Team_TouchEnemyFlag(ent, other, cl->sess.sessionTeam);
		}
		if (team != cl->sess.sessionTeam) {
			return Team_TouchOurFlag(ent, other, cl->sess.sessionTeam);
		}
		return 0;
	}

	// GT_CTF
	if (team == cl->sess.sessionTeam) {
		return Team_TouchOurFlag(ent, other, team);
	}
	return Team_TouchEnemyFlag(ent, other, team);
}

/*
===========
Team_GetLocation

Report a location for the player. Uses placed nearby target_location entities
============
*/
static gentity_t *Team_GetLocation(const gentity_t *ent) {
	gentity_t *eloc, *best;
	float bestlen;
	vec3_t origin;

	best = NULL;
	bestlen = 3.0f * Square(8192.0f);

	VectorCopy(ent->r.currentOrigin, origin);

	for (eloc = level.locationHead; eloc; eloc = eloc->nextTrain) {
		const float len = DistanceSquared(eloc->r.currentOrigin, origin);
		if (len > bestlen) {
			continue;
		}

		if (!trap_InPVS(origin, eloc->r.currentOrigin)) {
			continue;
		}

		bestlen = len;
		best = eloc;
	}

	return best;
}

/*
===========
Team_GetLocation

Report a location for the player. Uses placed nearby target_location entities
============
*/
qboolean Team_GetLocationMsg(gentity_t *ent, char *loc, int loclen) {
	gentity_t *best = Team_GetLocation(ent);
	if (!best)
		return qfalse;

	if (best->count) {
		if (best->count < 0)
			best->count = 0;
		if (best->count > 7)
			best->count = 7;
		Com_sprintf(loc, loclen, "%c%c%s" S_COLOR_WHITE, Q_COLOR_ESCAPE, best->count + '0', best->message);
	} else
		Com_sprintf(loc, loclen, "%s", best->message);

	return qtrue;
}

/*---------------------------------------------------------------------------*/

/*
================
SelectRandomTeamSpawnPoint

go to a random point that doesn't telefrag
================
*/
#define MAX_TEAM_SPAWN_POINTS 32
static gentity_t *SelectRandomTeamSpawnPoint(int teamstate, team_t team) {
	gentity_t *spot;
	int count;
	int selection;
	gentity_t *spots[MAX_TEAM_SPAWN_POINTS];
	char *classname;

	if (teamstate == TEAM_BEGIN) {
		if (team == TEAM_RED)
			classname = "team_redplayer";
		else if (team == TEAM_BLUE)
			classname = "team_blueplayer";
		else
			return NULL;
	} else {
		if (team == TEAM_RED)
			classname = "team_redspawn";
		else if (team == TEAM_BLUE)
			classname = "team_bluespawn";
		else
			return NULL;
	}
	count = 0;

	spot = NULL;

	while ((spot = G_Find(spot, FOFS(classname), classname)) != NULL) {
		if (SpotWouldTelefrag(spot)) {
			continue;
		}
		spots[count] = spot;
		if (++count == MAX_TEAM_SPAWN_POINTS)
			break;
	}

	if (!count) { // no spots that won't telefrag
		return G_Find(NULL, FOFS(classname), classname);
	}

	selection = rand() % count;
	return spots[selection];
}

/*
===========
SelectCTFSpawnPoint

============
*/
gentity_t *SelectCTFSpawnPoint(team_t team, int teamstate, vec3_t origin, vec3_t angles, qboolean isbot) {
	gentity_t *spot;

	spot = SelectRandomTeamSpawnPoint(teamstate, team);

	// TODO: don't look for info_player_deathmatch, exit with a meaningful msg instead
	if (!spot) {
		return SelectSpawnPoint(vec3_origin, origin, angles, isbot);
	}

	VectorCopy(spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy(spot->s.angles, angles);

	return spot;
}

/*---------------------------------------------------------------------------*/

/*
==================
TeamplayLocationsMessage

Format:
	clientNum location health armor weapon powerups cartridges

==================
*/
static void TeamplayInfoMessage(const gentity_t *ent) {
	char entry[1024];
	char string[8192];
	int stringlength, entrylength;
	int i;
	gentity_t *player;
	int count;
	int h, a;

	if (!ent->client->pers.teamInfo) {
		return;
	}

	// figure out what client should be on the display
	// we are limited to 8, but we want to use the top eight players
	// but in client order (so they don't keep changing position on the overlay)

	// send the latest information on all clients
	string[0] = '\0';
	stringlength = 0;

	for (i = 0, count = 0; ((i < level.maxclients) && (count < TEAM_MAXOVERLAY)); i++) {
		player = (g_entities + i);
		if (player->inuse && (player != ent) &&
			(player->client->sess.sessionTeam == ent->client->ps.persistant[PERS_TEAM])) {

			h = player->client->ps.stats[STAT_HEALTH];
			if (h < 0) {
				h = 0;
			}
			a = player->client->ps.stats[STAT_ARMOR];
			if (a < 0) {
				a = 0;
			}

			Com_sprintf(entry, sizeof(entry), " %i %i %i %i %i %i %i", i, player->client->pers.teamState.location, h, a,
						player->client->ps.weapon, player->s.powerups, player->client->ps.ammo[WP_SPRAYPISTOL]);

			entrylength = strlen(entry);
			if ((stringlength + entrylength) >= sizeof(string)) {
				break;
			}

			strcpy((string + stringlength), entry);
			stringlength += entrylength;
			count++;
		}
	}

	trap_SendServerCommand((ent - g_entities), va("tinfo %i %s", count, string));
}

void CheckTeamStatus(void) {
	int i;
	const gentity_t *loc, *ent;

	if (level.time - level.lastTeamLocationTime > TEAM_LOCATION_UPDATE_TIME) {

		level.lastTeamLocationTime = level.time;

		for (i = 0; i < g_maxclients.integer; i++) {
			ent = g_entities + i;

			if (ent->client->pers.connected != CON_CONNECTED) {
				continue;
			}

			if (ent->inuse &&
				(ent->client->sess.sessionTeam == TEAM_RED || ent->client->sess.sessionTeam == TEAM_BLUE)) {
				loc = Team_GetLocation(ent);
				if (loc)
					ent->client->pers.teamState.location = loc->health;
				else
					ent->client->pers.teamState.location = 0;
			}
		}

		for (i = 0; i < g_maxclients.integer; i++) {
			ent = g_entities + i;

			if (ent->client->pers.connected != CON_CONNECTED) {
				continue;
			}

			if (ent->inuse && (ent->client->ps.persistant[PERS_TEAM] == TEAM_RED ||
							   ent->client->ps.persistant[PERS_TEAM] == TEAM_BLUE)) {
				TeamplayInfoMessage(ent);
			}
		}
	}
}

/*-----------------------------------------------------------------*/

/*QUAKED team_CTF_redplayer (1 0 0) (-16 -16 -16) (16 16 32)
Only in CTF games.  Red players spawn here at game start.
*/
void SP_team_CTF_redplayer(gentity_t *ent) {
}

/*QUAKED team_CTF_blueplayer (0 0 1) (-16 -16 -16) (16 16 32)
Only in CTF games.  Blue players spawn here at game start.
*/
void SP_team_CTF_blueplayer(gentity_t *ent) {
}

/*QUAKED team_CTF_redspawn (1 0 0) (-16 -16 -24) (16 16 32)
potential spawning position for red team in CTF games.
Targets will be fired when someone spawns in on them.
*/
void SP_team_CTF_redspawn(gentity_t *ent) {
}

/*QUAKED team_CTF_bluespawn (0 0 1) (-16 -16 -24) (16 16 32)
potential spawning position for blue team in CTF games.
Targets will be fired when someone spawns in on them.
*/
void SP_team_CTF_bluespawn(gentity_t *ent) {
}
