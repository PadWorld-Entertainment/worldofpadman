// Copyright (C) 1999-2000 Id Software, Inc.
//

#include "g_local.h"


typedef struct teamgame_s {
	float			last_flag_capture;
	int				last_capture_team;
	flagStatus_t	redStatus;	// CTF
	flagStatus_t	blueStatus;	// CTF
	flagStatus_t	flagStatus;	// One Flag CTF
	int				redTakenTime;
	int				blueTakenTime;
	int				redObeliskAttackedTime;
	int				blueObeliskAttackedTime;
} teamgame_t;

teamgame_t teamgame;

gentity_t	*neutralObelisk;

void Team_SetFlagStatus( int team, flagStatus_t status );

void Team_InitGame( void ) {
	memset(&teamgame, 0, sizeof teamgame);

	switch( g_gametype.integer ) {
	case GT_CTF:
		teamgame.redStatus = -1; // Invalid to force update
		Team_SetFlagStatus( TEAM_RED, FLAG_ATBASE );
		teamgame.blueStatus = -1;
		Team_SetFlagStatus( TEAM_BLUE, FLAG_ATBASE );
		break;
	default:
		break;
	}
}

int OtherTeam(int team) {
	if (team==TEAM_RED)
		return TEAM_BLUE;
	else if (team==TEAM_BLUE)
		return TEAM_RED;
	return team;
}

const char *TeamName(int team)  {
	if (team==TEAM_RED)
		return "RED";
	else if (team==TEAM_BLUE)
		return "BLUE";
	else if (team==TEAM_SPECTATOR)
		return "SPECTATOR";
	return "FREE";
}

const char *TeamColorString(int team) {
	if (team==TEAM_RED)
		return S_COLOR_RED;
	else if (team==TEAM_BLUE)
		return S_COLOR_BLUE;
	else if (team==TEAM_SPECTATOR)
		return S_COLOR_YELLOW;
	return S_COLOR_WHITE;
}

// NULL for everyone
void QDECL PrintMsg( gentity_t *ent, const char *fmt, ... ) {
	char		msg[1024];
	va_list		argptr;
	char		*p;
	
	va_start (argptr,fmt);
	if (Q_vsnprintf (msg, sizeof(msg), fmt, argptr) >= sizeof(msg)) {
		G_Error ( "PrintMsg overrun" );
	}
	va_end (argptr);

	// double quotes are bad
	while ((p = strchr(msg, '"')) != NULL)
		*p = '\'';

	trap_SendServerCommand ( ( (ent == NULL) ? -1 : ent-g_entities ), va("print \"%s\"", msg ));
}

/*
==============
AddTeamScore

 used for gametype > GT_TEAM
 for gametype GT_TEAM the level.teamScores is updated in AddScore in g_combat.c
==============
*/
// TODO: Draw a ScorePlum() as well? Needs cgame fixes to draw plum regardless of "owner"
void AddTeamScore( vec3_t origin, int team, int score, char *reason ) {
	gentity_t	*te;

	te = G_TempEntity(origin, EV_GLOBAL_TEAM_SOUND );
	te->r.svFlags |= SVF_BROADCAST;

	if ( team == TEAM_RED ) {
		if ( level.teamScores[ TEAM_RED ] + score == level.teamScores[ TEAM_BLUE ] ) {
			//teams are tied sound
			te->s.eventParm = GTS_TEAMS_ARE_TIED;
		}
		else if ( level.teamScores[ TEAM_RED ] <= level.teamScores[ TEAM_BLUE ] &&
					level.teamScores[ TEAM_RED ] + score > level.teamScores[ TEAM_BLUE ]) {
			// red took the lead sound
			te->s.eventParm = GTS_REDTEAM_TOOK_LEAD;
		}
		else {
			// red scored sound
			te->s.eventParm = GTS_REDTEAM_SCORED;

			// Hackity! In BB teams score continously, which makes for annoying
			// sound spam. Thus disable sound.
			// Better solution would be to either only do this sound on captures/destroys
			// or continously at fixed time offsets.
			// Also note that there already is a "blue/red balloon" sound in cgame CG_UpdateBalloonStates
			if ( g_gametype.integer == GT_BALLOON ) {
				G_FreeEntity( te );
			}
		}
	}
	else {
		if ( level.teamScores[ TEAM_BLUE ] + score == level.teamScores[ TEAM_RED ] ) {
			//teams are tied sound
			te->s.eventParm = GTS_TEAMS_ARE_TIED;
		}
		else if ( level.teamScores[ TEAM_BLUE ] <= level.teamScores[ TEAM_RED ] &&
					level.teamScores[ TEAM_BLUE ] + score > level.teamScores[ TEAM_RED ]) {
			// blue took the lead sound
			te->s.eventParm = GTS_BLUETEAM_TOOK_LEAD;
		}
		else {
			// blue scored sound
			te->s.eventParm = GTS_BLUETEAM_SCORED;

			// See note above
			if ( g_gametype.integer == GT_BALLOON ) {
				G_FreeEntity( te );
			}
		}
	}
	level.teamScores[ team ] += score;

	CalculateRanks();

	G_LogPrintf( "AddTeamScore: %s %i %s\n", TeamName( team ), score, reason );
}

/*
==============
OnSameTeam
==============
*/
qboolean OnSameTeam( gentity_t *ent1, gentity_t *ent2 ) {
	if ( !ent1->client || !ent2->client ) {
		return qfalse;
	}

	if ( g_gametype.integer < GT_TEAM ) {
		return qfalse;
	}

	if ( ent1->client->sess.sessionTeam == ent2->client->sess.sessionTeam ) {
		return qtrue;
	}

	return qfalse;
}


static char ctfFlagStatusRemap[] = { '0', '1', '*', '*', '2' };
static char oneFlagStatusRemap[] = { '0', '1', '2', '3', '4' };

void Team_SetFlagStatus( int team, flagStatus_t status ) {
	qboolean modified = qfalse;

	switch( team ) {
	case TEAM_RED:	// CTF
		if( teamgame.redStatus != status ) {
			teamgame.redStatus = status;
			modified = qtrue;
		}
		break;

	case TEAM_BLUE:	// CTF
		if( teamgame.blueStatus != status ) {
			teamgame.blueStatus = status;
			modified = qtrue;
		}
		break;

	case TEAM_FREE:	// One Flag CTF
		if( teamgame.flagStatus != status ) {
			teamgame.flagStatus = status;
			modified = qtrue;
		}
		break;
	}

	if( modified ) {
		char st[4];

		if( g_gametype.integer == GT_CTF ) {
			st[0] = ctfFlagStatusRemap[teamgame.redStatus];
			st[1] = ctfFlagStatusRemap[teamgame.blueStatus];
			st[2] = 0;
		}
		else {		// GT_1FCTF
			st[0] = oneFlagStatusRemap[teamgame.flagStatus];
			st[1] = 0;
		}

		trap_SetConfigstring( CS_FLAGSTATUS, st );
	}
}

int Team_GetFlagStatus(int team){
	switch( team ) {
	case TEAM_RED:	// CTF
		return teamgame.redStatus;

	case TEAM_BLUE:
		return teamgame.blueStatus;
		
	default:
		return 0;
	}
}

void Team_CheckDroppedItem( gentity_t *dropped ) {
	if( dropped->item->giTag == PW_REDFLAG ) {
		Team_SetFlagStatus( TEAM_RED, FLAG_DROPPED );
	}
	else if( dropped->item->giTag == PW_BLUEFLAG ) {
		Team_SetFlagStatus( TEAM_BLUE, FLAG_DROPPED );
	}
}

/*
================
Team_ForceGesture
================
*/
void Team_ForceGesture(int team) {
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

/*
================
Team_FragBonuses

Calculate the bonuses for flag defense, flag carrier defense, etc.
Note that bonuses are not cumulative.  You get one, they are in importance
order.
================
*/
void Team_FragBonuses(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker)
{
	int i;
	gentity_t *ent;
	int flag_pw, enemy_flag_pw;
	int otherteam;
	int tokens;
	gentity_t *flag, *carrier = NULL;
	char *c;
	vec3_t v1, v2;
	int team;

	// no bonus for fragging yourself or team mates
	if (!targ->client || !attacker->client || targ == attacker || OnSameTeam(targ, attacker))
		return;

	team = targ->client->sess.sessionTeam;
	otherteam = OtherTeam(targ->client->sess.sessionTeam);
	if (otherteam < 0)
		return; // whoever died isn't on a team

	// same team, if the flag at base, check to he has the enemy flag
	if (team == TEAM_RED) {
		flag_pw = PW_REDFLAG;
		enemy_flag_pw = PW_BLUEFLAG;
	} else {
		flag_pw = PW_BLUEFLAG;
		enemy_flag_pw = PW_REDFLAG;
	}

	// did the attacker frag the flag carrier?
	tokens = 0;
	if (targ->client->ps.powerups[enemy_flag_pw]) {
		attacker->client->pers.teamState.lastfraggedcarrier = level.time;
		AddScore( attacker, targ->r.currentOrigin, CTF_FRAG_CARRIER_BONUS, SCORE_BONUS_FRAG_CARRIER_S );
		attacker->client->pers.teamState.fragcarrier++;
		PrintMsg(NULL, "%s" S_COLOR_WHITE " fragged %s's flag carrier!\n",
			attacker->client->pers.netname, TeamName(team));

		// the target had the flag, clear the hurt carrier
		// field on the other team
		for (i = 0; i < g_maxclients.integer; i++) {
			ent = g_entities + i;
			if (ent->inuse && ent->client->sess.sessionTeam == otherteam)
				ent->client->pers.teamState.lasthurtcarrier = 0;
		}
		return;
	}

	// did the attacker frag a head carrier? other->client->ps.generic1
	if (tokens) {
		attacker->client->pers.teamState.lastfraggedcarrier = level.time;
		AddScore( attacker, targ->r.currentOrigin, CTF_FRAG_CARRIER_BONUS * tokens * tokens, SCORE_BONUS_FRAG_CARRIER_S );
		attacker->client->pers.teamState.fragcarrier++;
		PrintMsg(NULL, "%s" S_COLOR_WHITE " fragged %s's skull carrier!\n",
			attacker->client->pers.netname, TeamName(team));

		// the target had the flag, clear the hurt carrier
		// field on the other team
		for (i = 0; i < g_maxclients.integer; i++) {
			ent = g_entities + i;
			if (ent->inuse && ent->client->sess.sessionTeam == otherteam)
				ent->client->pers.teamState.lasthurtcarrier = 0;
		}
		return;
	}

	if (targ->client->pers.teamState.lasthurtcarrier &&
		level.time - targ->client->pers.teamState.lasthurtcarrier < CTF_CARRIER_DANGER_PROTECT_TIMEOUT &&
		!attacker->client->ps.powerups[flag_pw]) {
		// attacker is on the same team as the flag carrier and
		// fragged a guy who hurt our flag carrier
		AddScore( attacker, targ->r.currentOrigin, CTF_CARRIER_DANGER_PROTECT_BONUS, SCORE_BONUS_CARRIER_PROTECT_S );

		attacker->client->pers.teamState.carrierdefense++;
		targ->client->pers.teamState.lasthurtcarrier = 0;

		attacker->client->ps.persistant[PERS_DEFEND_COUNT]++;
		// add the sprite over the player's head
		SetAward( attacker->client, AWARD_DEFEND );

		return;
	}

	if (targ->client->pers.teamState.lasthurtcarrier &&
		level.time - targ->client->pers.teamState.lasthurtcarrier < CTF_CARRIER_DANGER_PROTECT_TIMEOUT) {
		// attacker is on the same team as the skull carrier and
		AddScore( attacker, targ->r.currentOrigin, CTF_CARRIER_DANGER_PROTECT_BONUS, SCORE_BONUS_CARRIER_PROTECT_S );

		attacker->client->pers.teamState.carrierdefense++;
		targ->client->pers.teamState.lasthurtcarrier = 0;

		attacker->client->ps.persistant[PERS_DEFEND_COUNT]++;
		// add the sprite over the player's head
		SetAward( attacker->client, AWARD_DEFEND );

		return;
	}

	// flag and flag carrier area defense bonuses

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
		return;
	}
	// find attacker's team's flag carrier
	for (i = 0; i < g_maxclients.integer; i++) {
		carrier = g_entities + i;
		if (carrier->inuse && carrier->client->ps.powerups[flag_pw])
			break;
		carrier = NULL;
	}
	flag = NULL;
	while ((flag = G_Find (flag, FOFS(classname), c)) != NULL) {
		if (!(flag->flags & FL_DROPPED_ITEM))
			break;
	}

	if (!flag)
		return; // can't find attacker's flag

	// ok we have the attackers flag and a pointer to the carrier

	// check to see if we are defending the base's flag
	VectorSubtract(targ->r.currentOrigin, flag->r.currentOrigin, v1);
	VectorSubtract(attacker->r.currentOrigin, flag->r.currentOrigin, v2);

	if ( ( ( VectorLength(v1) < CTF_TARGET_PROTECT_RADIUS &&
		trap_InPVS(flag->r.currentOrigin, targ->r.currentOrigin ) ) ||
		( VectorLength(v2) < CTF_TARGET_PROTECT_RADIUS &&
		trap_InPVS(flag->r.currentOrigin, attacker->r.currentOrigin ) ) ) &&
		attacker->client->sess.sessionTeam != targ->client->sess.sessionTeam) {

		// we defended the base flag
		AddScore( attacker, targ->r.currentOrigin, CTF_FLAG_DEFENSE_BONUS, SCORE_BONUS_DEFENSE_S );
		attacker->client->pers.teamState.basedefense++;

		attacker->client->ps.persistant[PERS_DEFEND_COUNT]++;
		// add the sprite over the player's head
		SetAward( attacker->client, AWARD_DEFEND );

		return;
	}

	if (carrier && carrier != attacker) {
		VectorSubtract(targ->r.currentOrigin, carrier->r.currentOrigin, v1);
		VectorSubtract(attacker->r.currentOrigin, carrier->r.currentOrigin, v1);

		if ( ( ( VectorLength(v1) < CTF_ATTACKER_PROTECT_RADIUS &&
			trap_InPVS(carrier->r.currentOrigin, targ->r.currentOrigin ) ) ||
			( VectorLength(v2) < CTF_ATTACKER_PROTECT_RADIUS &&
				trap_InPVS(carrier->r.currentOrigin, attacker->r.currentOrigin ) ) ) &&
			attacker->client->sess.sessionTeam != targ->client->sess.sessionTeam) {
			AddScore( attacker, targ->r.currentOrigin, CTF_CARRIER_PROTECT_BONUS, SCORE_BONUS_CARRIER_PROTECT_S );
			attacker->client->pers.teamState.carrierdefense++;

			attacker->client->ps.persistant[PERS_DEFEND_COUNT]++;
			// add the sprite over the player's head
			SetAward( attacker->client, AWARD_DEFEND );

			return;
		}
	}
}

/*
================
Team_CheckHurtCarrier

Check to see if attacker hurt the flag carrier.  Needed when handing out bonuses for assistance to flag
carrier defense.
================
*/
void Team_CheckHurtCarrier(gentity_t *targ, gentity_t *attacker)
{
	int flag_pw;

	if (!targ->client || !attacker->client)
		return;

	if (targ->client->sess.sessionTeam == TEAM_RED)
		flag_pw = PW_BLUEFLAG;
	else
		flag_pw = PW_REDFLAG;

	// flags
	if (targ->client->ps.powerups[flag_pw] &&
		targ->client->sess.sessionTeam != attacker->client->sess.sessionTeam)
		attacker->client->pers.teamState.lasthurtcarrier = level.time;

	// skulls
	if (targ->client->ps.generic1 &&
		targ->client->sess.sessionTeam != attacker->client->sess.sessionTeam)
		attacker->client->pers.teamState.lasthurtcarrier = level.time;
}


gentity_t *Team_ResetFlag( int team ) {
	char *c;
	gentity_t *ent, *rent = NULL;

	switch (team) {
	case TEAM_RED:
		c = "team_CTL_redlolly";
		break;
	case TEAM_BLUE:
		c = "team_CTL_bluelolly";
		break;
	case TEAM_FREE:
		c = "team_CTF_neutralflag";
		break;
	default:
		return NULL;
	}

	ent = NULL;
	while ((ent = G_Find (ent, FOFS(classname), c)) != NULL) {
		if (ent->flags & FL_DROPPED_ITEM)
			G_FreeEntity(ent);
		else {
			rent = ent;
			RespawnItem(ent);
		}
	}

	Team_SetFlagStatus( team, FLAG_ATBASE );

	return rent;
}

void Team_ResetFlags( void ) {
	if( g_gametype.integer == GT_CTF ) {
		Team_ResetFlag( TEAM_RED );
		Team_ResetFlag( TEAM_BLUE );
	}
}

void Team_ReturnFlagSound( gentity_t *ent, int team ) {
	gentity_t	*te;

	if (ent == NULL) {
		G_Printf ("Warning:  NULL passed to Team_ReturnFlagSound\n");
		return;
	}

	te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND );
	if( team == TEAM_BLUE ) {
		te->s.eventParm = GTS_RED_RETURN;
	}
	else {
		te->s.eventParm = GTS_BLUE_RETURN;
	}
	te->r.svFlags |= SVF_BROADCAST;
}

void Team_TakeFlagSound( gentity_t *ent, int team ) {
	gentity_t	*te;

	if (ent == NULL) {
		G_Printf ("Warning:  NULL passed to Team_TakeFlagSound\n");
		return;
	}

	// only play sound when the flag was at the base
	// or not picked up the last 10 seconds
	switch(team) {
		case TEAM_RED:
			if( teamgame.blueStatus != FLAG_ATBASE ) {
				if (teamgame.blueTakenTime > level.time - 10000)
					return;
			}
			teamgame.blueTakenTime = level.time;
			break;

		case TEAM_BLUE:	// CTF
			if( teamgame.redStatus != FLAG_ATBASE ) {
				if (teamgame.redTakenTime > level.time - 10000)
					return;
			}
			teamgame.redTakenTime = level.time;
			break;
	}

	te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND );
	if( team == TEAM_BLUE ) {
		te->s.eventParm = GTS_RED_TAKEN;
	}
	else {
		te->s.eventParm = GTS_BLUE_TAKEN;
	}
	te->r.svFlags |= SVF_BROADCAST;
}

void Team_CaptureFlagSound( gentity_t *ent, int team ) {
	gentity_t	*te;

	if (ent == NULL) {
		G_Printf ("Warning:  NULL passed to Team_CaptureFlagSound\n");
		return;
	}

	te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND );
	if( team == TEAM_BLUE ) {
		te->s.eventParm = GTS_BLUE_CAPTURE;
	}
	else {
		te->s.eventParm = GTS_RED_CAPTURE;
	}
	te->r.svFlags |= SVF_BROADCAST;
}

void Team_ReturnFlag( int team ) {
	Team_ReturnFlagSound(Team_ResetFlag(team), team);
	if( team == TEAM_FREE ) {
		PrintMsg(NULL, "The lolly has returned!\n" );
	}
	else {
		PrintMsg(NULL, "The %s lolly has returned!\n", TeamName(team));
	}
}

void Team_FreeEntity( gentity_t *ent ) {
	if( ent->item->giTag == PW_REDFLAG ) {
		Team_ReturnFlag( TEAM_RED );
	}
	else if( ent->item->giTag == PW_BLUEFLAG ) {
		Team_ReturnFlag( TEAM_BLUE );
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
	int		team = TEAM_FREE;

	if( ent->item->giTag == PW_REDFLAG ) {
		team = TEAM_RED;
	}
	else if( ent->item->giTag == PW_BLUEFLAG ) {
		team = TEAM_BLUE;
	}

	Team_ReturnFlagSound( Team_ResetFlag( team ), team );
	// Reset Flag will delete this entity
}


/*
==============
Team_DroppedFlagThink
==============
*/
int Team_TouchOurFlag( gentity_t *ent, gentity_t *other, int team ) {
	int			i;
	gentity_t	*player;
	gclient_t	*cl = other->client;
	int			enemy_flag;

	if (cl->sess.sessionTeam == TEAM_RED) {
		enemy_flag = PW_BLUEFLAG;
	} else {
		enemy_flag = PW_REDFLAG;
	}

	if ( ent->flags & FL_DROPPED_ITEM ) {
		// hey, it's not home.  return it by teleporting it back
		PrintMsg( NULL, "%s" S_COLOR_WHITE " returned the %s lolly!\n", cl->pers.netname, TeamName( team ) );

		AddScore( other, ent->r.currentOrigin, CTF_RECOVERY_BONUS, SCORE_BONUS_RECOVERY_S );

		other->client->pers.teamState.flagrecovery++;
		other->client->pers.teamState.lastreturnedflag = level.time;

		//ResetFlag will remove this entity!  We must return zero
		Team_ReturnFlagSound(Team_ResetFlag(team), team);
		return 0;
	}

	// the flag is at home base.  if the player has the enemy
	// flag, he's just won!
	if (!cl->ps.powerups[enemy_flag])
		return 0; // We don't have the flag
	PrintMsg( NULL, "%s" S_COLOR_WHITE " captured the %s lolly!\n", cl->pers.netname, TeamName(OtherTeam(team)));

	cl->ps.powerups[enemy_flag] = 0;

	teamgame.last_flag_capture = level.time;
	teamgame.last_capture_team = team;

	// Increase the team's score
	AddTeamScore( ent->s.pos.trBase, other->client->sess.sessionTeam, SCORE_CAPTURE, SCORE_BONUS_CAPTURE_S );
//	Team_ForceGesture(other->client->sess.sessionTeam);

	other->client->pers.teamState.captures++;
	// add the sprite over the player's head
	SetAward( other->client, AWARD_CAP );

	other->client->ps.persistant[PERS_CAPTURES]++;

	// other gets another 10 frag bonus
	AddScore( other, ent->r.currentOrigin, CTF_CAPTURE_BONUS, SCORE_BONUS_CAPTURE_S );

	Team_CaptureFlagSound( ent, team );

	// Ok, let's do the player loop, hand out the bonuses
	for (i = 0; i < g_maxclients.integer; i++) {
		player = &g_entities[i];

		// also make sure we don't award assist bonuses to the flag carrier himself.
		if (!player->inuse || player == other)
			continue;

		if (player->client->sess.sessionTeam !=
			cl->sess.sessionTeam) {
			player->client->pers.teamState.lasthurtcarrier = -5;
		} else if (player->client->sess.sessionTeam ==
			cl->sess.sessionTeam) {
			if (player != other)
				AddScore( player, ent->r.currentOrigin, CTF_TEAM_BONUS, SCORE_BONUS_CAPTURE_TEAM_S );
			// award extra points for capture assists
			if (player->client->pers.teamState.lastreturnedflag + 
				CTF_RETURN_FLAG_ASSIST_TIMEOUT > level.time) {
				AddScore( player, ent->r.currentOrigin, CTF_RETURN_FLAG_ASSIST_BONUS, SCORE_BONUS_ASSIST_RETURN_S );
				other->client->pers.teamState.assists++;

				player->client->ps.persistant[PERS_ASSIST_COUNT]++;
				// add the sprite over the player's head
				SetAward( player->client, AWARD_ASSIST );
			} else if (player->client->pers.teamState.lastfraggedcarrier + 
				CTF_FRAG_CARRIER_ASSIST_TIMEOUT > level.time) {
				AddScore( player, ent->r.currentOrigin, CTF_FRAG_CARRIER_ASSIST_BONUS, SCORE_BONUS_ASSIST_FRAG_CARRIER_S );
				other->client->pers.teamState.assists++;
				player->client->ps.persistant[PERS_ASSIST_COUNT]++;
				// add the sprite over the player's head
				SetAward( player->client, AWARD_ASSIST );
			}
		}
	}
	Team_ResetFlags();

	return 0; // Do not respawn this automatically
}

int Team_TouchEnemyFlag( gentity_t *ent, gentity_t *other, int team ) {
	gclient_t *cl = other->client;

		PrintMsg (NULL, "%s" S_COLOR_WHITE " got the %s lolly!\n",
			other->client->pers.netname, TeamName(team));

		if (team == TEAM_RED)
			cl->ps.powerups[PW_REDFLAG] = INT_MAX; // flags never expire
		else
			cl->ps.powerups[PW_BLUEFLAG] = INT_MAX; // flags never expire

		Team_SetFlagStatus( team, FLAG_TAKEN );

	AddScore( other, ent->r.currentOrigin, CTF_FLAG_BONUS, SCORE_BONUS_FLAG_S );

	cl->pers.teamState.flagsince = level.time;
	Team_TakeFlagSound( ent, team );

	return -1; // Do not respawn this automatically, but do delete it if it was FL_DROPPED
}

int Pickup_Team( gentity_t *ent, gentity_t *other ) {
	int team;
	gclient_t *cl = other->client;

	// figure out what team this flag is
	if( strcmp(ent->classname, "team_CTL_redlolly") == 0 ) {
		team = TEAM_RED;
	}
	else if( strcmp(ent->classname, "team_CTL_bluelolly") == 0 ) {
		team = TEAM_BLUE;
	}
	else {
		PrintMsg ( other, "Don't know what team the lolly is on.\n");
		return 0;
	}
	// GT_CTF
	if( team == cl->sess.sessionTeam) {
		return Team_TouchOurFlag( ent, other, team );
	}
	return Team_TouchEnemyFlag( ent, other, team );
}

/*
===========
Team_GetLocation

Report a location for the player. Uses placed nearby target_location entities
============
*/
gentity_t *Team_GetLocation(gentity_t *ent)
{
	gentity_t		*eloc, *best;
	float			bestlen, len;
	vec3_t			origin;

	best = NULL;
	bestlen = 3*8192.0*8192.0;

	VectorCopy( ent->r.currentOrigin, origin );

	for (eloc = level.locationHead; eloc; eloc = eloc->nextTrain) {
		len = ( origin[0] - eloc->r.currentOrigin[0] ) * ( origin[0] - eloc->r.currentOrigin[0] )
			+ ( origin[1] - eloc->r.currentOrigin[1] ) * ( origin[1] - eloc->r.currentOrigin[1] )
			+ ( origin[2] - eloc->r.currentOrigin[2] ) * ( origin[2] - eloc->r.currentOrigin[2] );

		if ( len > bestlen ) {
			continue;
		}

		if ( !trap_InPVS( origin, eloc->r.currentOrigin ) ) {
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
qboolean Team_GetLocationMsg(gentity_t *ent, char *loc, int loclen)
{
	gentity_t *best;

	best = Team_GetLocation( ent );
	
	if (!best)
		return qfalse;

	if (best->count) {
		if (best->count < 0)
			best->count = 0;
		if (best->count > 7)
			best->count = 7;
		Com_sprintf(loc, loclen, "%c%c%s" S_COLOR_WHITE, Q_COLOR_ESCAPE, best->count + '0', best->message );
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
#define	MAX_TEAM_SPAWN_POINTS	32
gentity_t *SelectRandomTeamSpawnPoint( int teamstate, team_t team ) {
	gentity_t	*spot;
	int			count;
	int			selection;
	gentity_t	*spots[MAX_TEAM_SPAWN_POINTS];
	char		*classname;

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

	while ((spot = G_Find (spot, FOFS(classname), classname)) != NULL) {
		if ( SpotWouldTelefrag( spot ) ) {
			continue;
		}
		spots[ count ] = spot;
		if (++count == MAX_TEAM_SPAWN_POINTS)
			break;
	}

	if ( !count ) {	// no spots that won't telefrag
		return G_Find( NULL, FOFS(classname), classname);
	}

	selection = rand() % count;
	return spots[ selection ];
}


/*
===========
SelectCTFSpawnPoint

============
*/
gentity_t *SelectCTFSpawnPoint ( team_t team, int teamstate, vec3_t origin, vec3_t angles, qboolean isbot ) {
	gentity_t	*spot;

	spot = SelectRandomTeamSpawnPoint ( teamstate, team );

	// TODO: don't look for info_player_deathmatch, exit with a meaningful msg instead
	if (!spot) {
		return SelectSpawnPoint( vec3_origin, origin, angles, isbot );
	}

	VectorCopy (spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy (spot->s.angles, angles);

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
static void TeamplayInfoMessage( const gentity_t *ent ) {
	char		entry[1024];
	char		string[8192];
	int			stringlength, entrylength;
	int			i;
	gentity_t	*player;
	int			count;
	int			h, a;

	if ( !ent->client->pers.teamInfo ) {
		return;
	}

	// figure out what client should be on the display
	// we are limited to 8, but we want to use the top eight players
	// but in client order (so they don't keep changing position on the overlay)

	
	// send the latest information on all clients
	string[0] = '\0';
	stringlength = 0;

	for ( i = 0, count = 0; ( ( i < level.maxclients ) && ( count < TEAM_MAXOVERLAY ) ); i++ ) {
		player = ( g_entities + i );
		if ( player->inuse && ( player != ent ) &&
		     ( player->client->sess.sessionTeam == ent->client->ps.persistant[PERS_TEAM] ) ) {

			h = player->client->ps.stats[STAT_HEALTH];
			if ( h < 0 ) {
				h = 0;
			}
			a = player->client->ps.stats[STAT_ARMOR];
			if ( a < 0 ) {
				a = 0;
			}

			Com_sprintf( entry, sizeof( entry ),
				" %i %i %i %i %i %i %i", 
				i, player->client->pers.teamState.location, h, a, 
				player->client->ps.weapon, player->s.powerups, player->client->ps.ammo[WP_SPRAYPISTOL]);

			entrylength = strlen( entry );
			if ( ( stringlength + entrylength ) >= sizeof( string ) ) {
				break;
			}

			strcpy( ( string + stringlength ), entry );
			stringlength += entrylength;
			count++;
		}
	}

	trap_SendServerCommand( ( ent - g_entities ), va( "tinfo %i %s", count, string ) );
}

void CheckTeamStatus(void) {
	int i;
	gentity_t *loc, *ent;

	if (level.time - level.lastTeamLocationTime > TEAM_LOCATION_UPDATE_TIME) {

		level.lastTeamLocationTime = level.time;

		for (i = 0; i < g_maxclients.integer; i++) {
			ent = g_entities + i;

			if ( ent->client->pers.connected != CON_CONNECTED ) {
				continue;
			}

			if (ent->inuse && (ent->client->sess.sessionTeam == TEAM_RED ||	ent->client->sess.sessionTeam == TEAM_BLUE)) {
				loc = Team_GetLocation( ent );
				if (loc)
					ent->client->pers.teamState.location = loc->health;
				else
					ent->client->pers.teamState.location = 0;
			}
		}

		for (i = 0; i < g_maxclients.integer; i++) {
			ent = g_entities + i;

			if ( ent->client->pers.connected != CON_CONNECTED ) {
				continue;
			}

			if (ent->inuse && (ent->client->ps.persistant[PERS_TEAM] == TEAM_RED ||	ent->client->ps.persistant[PERS_TEAM] == TEAM_BLUE)) {
				TeamplayInfoMessage( ent );
			}
		}
	}
}

/*-----------------------------------------------------------------*/

/*QUAKED team_CTF_redplayer (1 0 0) (-16 -16 -16) (16 16 32)
Only in CTF games.  Red players spawn here at game start.
*/
void SP_team_CTF_redplayer( gentity_t *ent ) {
}


/*QUAKED team_CTF_blueplayer (0 0 1) (-16 -16 -16) (16 16 32)
Only in CTF games.  Blue players spawn here at game start.
*/
void SP_team_CTF_blueplayer( gentity_t *ent ) {
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


