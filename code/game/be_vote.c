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

#include "g_local.h"

/* Internal functions */
static qboolean VoteH_Map(gentity_t *ent, voteID_t id);
static qboolean VoteH_Kick(gentity_t *ent, voteID_t id);
static qboolean VoteH_Gametype(gentity_t *ent, voteID_t id);
static qboolean VoteH_Misc(gentity_t *ent, voteID_t id);

const voteEntry_t VOTES[] = {{{"nextmap", VOTEID_NEXTMAP}, VoteH_Map},
							 {{"map", VOTEID_MAP}, VoteH_Map},
							 {{"map_restart", VOTEID_MAP_RESTART}, VoteH_Map},
							 {{"kick", VOTEID_KICK}, VoteH_Kick},
							 {{"clientkick", VOTEID_CLIENTKICK}, VoteH_Kick},
							 {{"pointlimit", VOTEID_POINTLIMIT}, VoteH_Misc},
							 {{"timelimit", VOTEID_TIMELIMIT}, VoteH_Misc},
							 {{"setgametype", VOTEID_SETGAMETYPE}, VoteH_Gametype},
							 {{"g_gametype", VOTEID_G_GAMETYPE}, VoteH_Gametype},
							 {{"shuffleteams", VOTEID_SHUFFLETEAMS}, VoteH_Misc}};
const unsigned int NUM_VOTES = ARRAY_LEN(VOTES);

static const char *GetVoteStr(voteID_t id) {
	int i;

	for (i = 0; i < NUM_VOTES; i++) {
		if (id == VOTES[i].ident.id) {
			return VOTES[i].ident.str;
		}
	}

	return NULL;
}

static voteID_t GetVoteID(const char *str) {
	int i;

	G_assert(str != NULL);

	for (i = 0; i < NUM_VOTES; i++) {
		if (Q_stricmp(str, VOTES[i].ident.str) == 0) {
			return VOTES[i].ident.id;
		}
	}

	return VOTE_NONE;
}

static voteFunc_t GetVoteHandler(voteID_t id) {
	int i;

	for (i = 0; i < NUM_VOTES; i++) {
		if (id == VOTES[i].ident.id) {
			return VOTES[i].func;
		}
	}

	return (voteFunc_t)0;
}

static qboolean IsAllowedVote(voteID_t id, qboolean limit) {
	/* No G_assert() here! typeof( id ) is basically int */
	if ((id <= VOTEID_NONE) || (id >= VOTEID_NUM_VOTES)) {
		return qfalse;
	}

	if (limit) {
		const char *str = GetVoteStr(id);
		/* FIXME: Due to cvar.string MAX_CVAR_VALUE_STRING limit, we
				  can't have many votes. Use some sort of config, use bitmask?
				  At least add a qboolean allowed to VOTES.
				  Maybe don't use the game logic to register cvars, only engine functions.
		*/
		return InList(be_allowedVotes.string, str);
	}

	return qtrue;
}

/*
	Beryllium's replacement for Cmd_Vote_f() in g_cmds.c
*/
void BE_Cmd_Vote_f(gentity_t *ent) {
	char msg[8];

	G_assert(ent != NULL);

	if (!level.voteTime) {
		SendClientCommand((ent - g_entities), CCMD_PRT, S_COLOR_NEGATIVE "No vote in progress.\n");
		return;
	}
	if (ent->client->ps.eFlags & EF_VOTED) {
		/* TODO: Print vote? */
		SendClientCommand((ent - g_entities), CCMD_PRT, S_COLOR_NEGATIVE "Vote already cast.\n");
		return;
	}
	if (IsSpectator(ent->client)) {
		SendClientCommand((ent - g_entities), CCMD_PRT, S_COLOR_NEGATIVE "Not allowed to vote as spectator.\n");
		return;
	}

	if (trap_Argc() < 2) {
		SendClientCommand((ent - g_entities), CCMD_PRT, "Usage: vote <yes|no>\n");
		return;
	}

	SendClientCommand((ent - g_entities), CCMD_PRT, "Vote cast, thank you.\n");

	ent->client->ps.eFlags |= EF_VOTED;

	trap_Argv(1, msg, sizeof(msg));

	if ((msg[0] == 'y') || (msg[0] == 'Y') || (msg[0] == '1')) {
		level.voteYes++;
		trap_SetConfigstring(CS_VOTE_YES, va("%i", level.voteYes));

		/* TODO: Don't print to self? Need SendClientCommandExcept()
				 Use Loglevels?
		*/
		/* TODO: Privacy? Cvar? Log? */
		/*
		SendClientCommand( CID_ALL, CCMD_PRT, va( "%s"S_COLOR_DEFAULT" voted yes.\n", ent->client->pers.netname ) );
		*/

		ent->client->pers.voted = VOTE_YES;
	} else if ((msg[0] == 'n') || (msg[0] == 'N') || (msg[0] == '0')) {
		level.voteNo++;
		trap_SetConfigstring(CS_VOTE_NO, va("%i", level.voteNo));

		/* TODO: Privacy? Cvar? Log? */
		/*
		SendClientCommand( CID_ALL, CCMD_PRT, va( "%s"S_COLOR_DEFAULT" voted no.\n", ent->client->pers.netname ) );
		*/

		ent->client->pers.voted = VOTE_NO;
	}

	/* "a majority will be determined in CheckVote, which will also account
	   for players entering or leaving"
	*/
}

static void PrintValidVotes(gentity_t *ent) {
	int i, count = 0;
	char validVoteString[MAX_STRING_TOKENS] = {S_COLOR_ITALIC "Valid and allowed vote commands are: "};
	qboolean unpriv = qtrue;

	/* NOTE: No G_assert( ent ) here, since console might call this */

	if (!ent) {
		unpriv = qfalse;
	}

	for (i = 0; i < NUM_VOTES; i++) {
		/* FIXME: This is a horribly duplicated and nested, but somewhat clean implementation.. */
		if (!IsAllowedVote(VOTES[i].ident.id, unpriv)) {
			/* FIXME: Handle case when there are no allowed votes? Set g_allowVote 0? */
			continue;
		}

		/* This is not actually "bold", but needs to differ from the "italic" hint */
		Q_strcat(validVoteString, sizeof(validVoteString),
				 va("%s" S_COLOR_BOLD "%s", (count > 0 ? S_COLOR_ITALIC ", " : ""), VOTES[i].ident.str));
		count++;
	}
	Q_strcat(validVoteString, sizeof(validVoteString), "\n");

	PrintMessage(ent, validVoteString);
}

/*
	Beryllium's replacement for the original Cmd_CallVote_f() in g_cmds.c
*/
void BE_Cmd_CallVote_f(gentity_t *ent) {
	char arg1[MAX_STRING_TOKENS];
	char arg2[MAX_STRING_TOKENS];
	char *c;
	int i;
	voteID_t id;
	voteFunc_t handler;

	/* Console is always allowed to callvote */
	if (ent && !g_allowVote.integer) {
		SendClientCommand((ent - g_entities), CCMD_PRT, S_COLOR_NEGATIVE "Voting not allowed here.\n");
		return;
	}

	if (level.voteTime) {
		PrintMessage(ent, S_COLOR_NEGATIVE "A vote is already in progress.\n");
		return;
	}

	/* Following tests should only be applied when called by a player */
	if (ent) {
		if (ent->client->pers.voteCount >= be_maxVotes.integer) {
			SendClientCommand((ent - g_entities), CCMD_PRT,
							  S_COLOR_NEGATIVE "You have called the maximum number of votes.\n");
			return;
		}

		if (IsSpectator(ent->client)) {
			SendClientCommand((ent - g_entities), CCMD_PRT,
							  S_COLOR_NEGATIVE "Not allowed to call a vote as spectator.\n");
			return;
		}

		/* TODO: This *could* be checked for rcon callvote aswell */
		if (level.voteEnd && (level.time - level.voteEnd) <= (be_votePause.integer * 1000)) {

			SendClientCommand((ent - g_entities), CCMD_PRT,
							  S_COLOR_NEGATIVE "One can not call a vote so short after the previous one.\n");

			TimeToString(((be_votePause.integer * 1000) - (level.time - level.voteEnd)), arg1, sizeof(arg1));
			SendClientCommand((ent - g_entities), CCMD_PRT, va(S_COLOR_ITALIC "You need to wait %s.\n", arg1));
			return;
		}

		if (ent->client->pers.voteTime && (level.time - ent->client->pers.voteTime) <= (be_voteRate.integer * 1000)) {

			SendClientCommand((ent - g_entities), CCMD_PRT,
							  S_COLOR_NEGATIVE "You can not call a new vote so short after your previous one.\n");

			TimeToString(((be_voteRate.integer * 1000) - (level.time - ent->client->pers.voteTime)), arg1,
						 sizeof(arg1));
			SendClientCommand((ent - g_entities), CCMD_PRT, va(S_COLOR_ITALIC "You need to wait %s.\n", arg1));
			return;
		}
	}

	/* make sure it is a valid command to vote on */
	trap_Argv(1, arg1, sizeof(arg1));
	trap_Argv(2, arg2, sizeof(arg2));

	/* NOTE: http://bugzilla.icculus.org/show_bug.cgi?id=3593 */
	/* check for command separators in arg2 */
	for (c = arg2; *c; ++c) {
		switch (*c) {
		case '\n':
		case '\r':
		case ';':
			PrintMessage(ent, S_COLOR_NEGATIVE "Invalid vote string.\n");
			return;
			break;
		}
	}

	id = GetVoteID(arg1);

	if (!IsAllowedVote(id, (ent ? qtrue : qfalse))) {
		if (trap_Argc() > 1) {
			PrintMessage(ent, S_COLOR_NEGATIVE "Vote is not allowed or invalid.\n");
		}
		PrintValidVotes(ent);
		return;
	}

	/* if there is still a vote to be executed */
	if (level.voteExecuteTime) {
		level.voteExecuteTime = 0;
		/* TODO: Add a LogPrintf which states we are executing a vote now?
				 Votestring itself is already printed in callvote.
		*/
		trap_SendConsoleCommand(EXEC_APPEND, va("%s\n", level.voteString));
	}

	handler = GetVoteHandler(id);
	if (!handler) {
		G_Error("BE_Cmd_CallVote_f: GetVoteHandler() returned 0!\n");
		return;
	}

	if (handler(ent, id)) {
		/* FIXME: Votestring can not include \" since it is already escaped once.
				  voteDisplayString is also S_COLOR_ITALIC
		*/
		SendClientCommand(CID_ALL, CCMD_PRT,
						  va(S_COLOR_ITALIC "%s" S_COLOR_ITALIC " called a vote: %s" S_COLOR_ITALIC ".\n",
							 (ent ? ent->client->pers.netname : CHAT_SERVER_NAME), level.voteDisplayString));
		/* TODO: Create a seperate BE_Printf( type, level, text ).
				 Use voteString or voteDisplayString?
		*/
		if (ent) {
			G_LogPrintf("callvote %ld: '%s'\n", (ent - g_entities), level.voteString);
		} else {
			/* FIXME: #define CID_SERVER/use CID_WORLD? */
			G_LogPrintf("callvote %d: '%s'\n", -1, level.voteString);
		}

		/* start vote, the caller autoamtically votes yes */
		level.voteTime = level.time;
		level.voteNo = level.voteYes = 0;

		for (i = 0; i < level.maxclients; i++) {
			level.clients[i].ps.eFlags &= ~EF_VOTED;
			level.clients[i].pers.voted = VOTE_NONE;
		}

		if (ent) {
			level.voteYes = 1;
			ent->client->ps.eFlags |= EF_VOTED;
			ent->client->pers.voted = VOTE_YES;
			ent->client->pers.voteCount++;
			ent->client->pers.voteTime = level.time;
		}

		/* A littly hackity to get votes with variable time.
		   The client has a hardcoded duration of VOTE_TIME, which
		   is 30s. So we need to send a modified level.voteTime to the client
		   to get its time being displayed correctly, while still having
		   the correct voteTime in game.
		*/
		/* voteDuration is needed so changing the cvar doesn't fuck up current vote.
		   Furthermore, each vote *can* have an individual time that way, maybe one day
		   Callvote( displayString, string, duration ) instead of this beast.
		   Cvar is in seconds.
		*/
		level.voteDuration = (be_voteDuration.integer * 1000);
		i = (level.voteTime + (level.voteDuration - VOTE_TIME));

		trap_SetConfigstring(CS_VOTE_TIME, va("%i", i));
		trap_SetConfigstring(CS_VOTE_STRING, level.voteDisplayString);
		trap_SetConfigstring(CS_VOTE_YES, va("%i", level.voteYes));
		trap_SetConfigstring(CS_VOTE_NO, va("%i", level.voteNo));

		return;
	} else {
		/* Error inside vote handler. Error output is done in handler itself, so nothing to do */
		return;
	}
}

/*
	Returns qtrue when the current vote should pass
*/
static qboolean G_VotePassed(void) {
	return (level.voteYes > (level.numVotingClients * be_votePass.value));
}

/*
	Returns qtrue when the current vote should fail
*/
static qboolean G_VoteFailed(void) {
	return (level.voteNo >= (level.numVotingClients * (1.0 - be_votePass.value)));
}

/*
	Counts the number of clients which can vote and sets level.numVotingClients
*/
static void G_CountVotingClients(void) {
	int i;

	level.numVotingClients = 0;
	for (i = 0; i < level.maxclients; i++) {
		if ((level.clients[i].pers.connected != CON_CONNECTED) || (g_entities[i].r.svFlags & SVF_BOT)) {
			continue;
		}

		/* Anyone currently in a team is allowed to vote */
		/* TODO: Decide about dead LPS specators and then use IsSpectator() */
		if (level.clients[i].sess.sessionTeam != TEAM_SPECTATOR) {
			level.numVotingClients++;
		}
		/* If spectator but voted already, also count him */
		else if (level.clients[i].pers.voted != VOTE_NONE) {
			level.numVotingClients++;
		}
	}
}

/*
	Beryllium's replacement for CheckVote() in g_main.c
	Determines whether a vote passed or failed and execute it
*/
#define VOTE_EXECUTEDELAY 3000
void BE_CheckVote(void) {
	if (level.voteExecuteTime && (level.voteExecuteTime < level.time)) {
		level.voteExecuteTime = 0;
		trap_SendConsoleCommand(EXEC_APPEND, va("%s\n", level.voteString));
	}

	if (!level.voteTime) {
		return;
	}

	/* FIXME: Properly count voters/votes. This is already done in CalculateRanks, but does not
			  account for players that callvote/vote, join spectators (but are still counted, since
			  they voted previously) at the next vote. So just recount here quick&dirty
	*/
	G_CountVotingClients();

	/* TODO: Print numbers of yay-nay-rest? */
	if ((level.time - level.voteTime) >= level.voteDuration) {
		SendClientCommand(CID_ALL, CCMD_PRT, S_COLOR_ITALIC "Vote failed, timeout.\n");
	} else {
		if (G_VotePassed()) {
			/* Set timeout, then execute and remove the vote at next call */
			SendClientCommand(CID_ALL, CCMD_PRT, S_COLOR_ITALIC "Vote passed.\n");
			level.voteExecuteTime = (level.time + VOTE_EXECUTEDELAY);
		} else if (G_VoteFailed()) {
			/* same behavior as a timeout */
			SendClientCommand(CID_ALL, CCMD_PRT, S_COLOR_ITALIC "Vote failed.\n");
		} else {
			/* still waiting for a majority */
			return;
		}
	}

	level.voteEnd = level.time;
	level.voteTime = 0;
	trap_SetConfigstring(CS_VOTE_TIME, "");
	trap_SetConfigstring(CS_VOTE_STRING, "");
}
#undef VOTE_EXECUTEDELAY

/*
	Handler for g_gametype and setgametype vote
*/
static qboolean VoteH_Gametype(gentity_t *ent, voteID_t id) {
	char arg2[MAX_STRING_TOKENS];
	int i;

	trap_Argv(2, arg2, sizeof(arg2));

	/* Special: A variant of g_gametype vote */
	if (VOTEID_SETGAMETYPE == id) {
		gametype_t gt;

		if (strlen(arg2) == 0) {
			PrintMessage(ent, S_COLOR_NEGATIVE "You must supply a gametype keyword.\n");
			return qfalse;
		}

		gt = StringToGametype(ConcatArgs(2));

		if (GT_MAX_GAME_TYPE == gt) {
			PrintMessage(ent, S_COLOR_NEGATIVE "Couldn't find a gametype with the keywords.\n");
			return qfalse;
		}

		/* Gametype found, now "convert" into g_gametype vote */
		id = VOTEID_G_GAMETYPE;
		Com_sprintf(arg2, sizeof(arg2), "%i", gt);
	}

	if (VOTEID_G_GAMETYPE != id) {
		G_Error("VoteH_Gametype: Invalid id %i!\n", id);
		return qfalse;
	}

	/* Now we have a g_gametype vote */
	if (strlen(arg2) == 0) {
		PrintMessage(ent, S_COLOR_NEGATIVE "You must supply a gametype number.\n");
		return qfalse;
	}

	if (!Q_isanumber(arg2)) {
		PrintMessage(ent, S_COLOR_NEGATIVE "You must supply a number.\n");
		return qfalse;
	}

	i = atoi(arg2);

	if ((GT_SINGLE_PLAYER == i) || (i < GT_FFA) || (i >= GT_MAX_GAME_TYPE)) {
		PrintMessage(ent, S_COLOR_NEGATIVE "Invalid gametype.\n");
		return qfalse;
	}

	if (g_gametype.integer == i) {
		PrintMessage(ent, S_COLOR_NEGATIVE "This is the current gametype.\n");
		return qfalse;
	}

	Com_sprintf(level.voteString, sizeof(level.voteString), "set g_gametype %d", i);
	Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString),
				S_COLOR_ITALIC "Gametype '%s'" S_COLOR_DEFAULT, GametypeToString(i));

	return qtrue;
}

/*
	Handler for clientkick and kick vote
*/
static qboolean VoteH_Kick(gentity_t *ent, voteID_t id) {
	char arg2[MAX_STRING_TOKENS];
	int i;

	trap_Argv(2, arg2, sizeof(arg2));

	/* Special: A variant of clientkick vote */
	if (VOTEID_KICK == id) {
		int cid;

		if (strlen(arg2) == 0) {
			PrintMessage(ent, S_COLOR_NEGATIVE "You must supply a client name.\n");
			return qfalse;
		}

		cid = ClientnumFromString(arg2);

		if (CID_NONE == cid) {
			PrintMessage(ent, S_COLOR_NEGATIVE
						 "No player found with that name. Check for typos or use 'clientkick' instead.\n");
			return qfalse;
		} else if (CID_MULTIPLE == cid) {
			PrintMessage(ent, S_COLOR_NEGATIVE "Multiple players found with that name. Use 'clientkick' instead.\n");
			return qfalse;
		}

		if (level.clients[cid].pers.localClient) {
			/* NOTE: localClient is only set for clients connected to their own computer, not LAN clients in general */
			PrintMessage(ent, S_COLOR_NEGATIVE "You can not kick the host player.\n");
			return qfalse;
		}

		/* We've got one, convert into clientkick vote
		   TODO: Display old and current name in votestring?
		*/
		id = VOTEID_CLIENTKICK;
		Com_sprintf(arg2, sizeof(arg2), "%i", cid);
	}

	if (VOTEID_CLIENTKICK != id) {
		G_Error("VoteH_Kick: Invalid id %i!\n", id);
		return qfalse;
	}

	/* Now we have a clientkick vote */
	if (strlen(arg2) == 0) {
		PrintMessage(ent, S_COLOR_NEGATIVE "You must supply a client number.\n");
		return qfalse;
	}

	if (!Q_isanumber(arg2)) {
		PrintMessage(ent, S_COLOR_NEGATIVE "You must supply a number.\n");
		return qfalse;
	}

	i = atoi(arg2);

	if (!ValidClientID(i, qfalse)) {
		PrintMessage(ent, S_COLOR_NEGATIVE "Not a valid client number.\n");
		return qfalse;
	}

	if (CON_DISCONNECTED == level.clients[i].pers.connected) {
		PrintMessage(ent, S_COLOR_NEGATIVE "Client not connected.\n");
		return qfalse;
	}

	if (ent && ((ent - g_entities) == i)) {
		SendClientCommand((ent - g_entities), CCMD_PRINT, S_COLOR_NEGATIVE "You can not kick yourself.\n");
		return qfalse;
	}

	Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString),
				S_COLOR_ITALIC "Kick %i: '%s'" S_COLOR_DEFAULT, i, level.clients[i].pers.netname);

	/* Append additional argument, e.g. "reason" */
	if (trap_Argc() >= 4) {
		char reason[16];

		Q_strncpyz(reason, ConcatArgs(3), sizeof(reason));
		Q_strcat(level.voteDisplayString, sizeof(level.voteDisplayString),
				 va(S_COLOR_ITALIC ", %s" S_COLOR_DEFAULT, reason));

		/* NOTE: "dropclient" is a custom server command provided by beryllium */
		Com_sprintf(level.voteString, sizeof(level.voteString), "dropclient %i \"%s\"", i, reason);
	} else {
		Com_sprintf(level.voteString, sizeof(level.voteString), "dropclient %i", i);
	}

	return qtrue;
}

/*
	Handler for map, nextmap and map_restart vote.
*/
static qboolean VoteH_Map(gentity_t *ent, voteID_t id) {
	char arg2[MAX_STRING_TOKENS];

	trap_Argv(2, arg2, sizeof(arg2));

	if (VOTEID_MAP == id) {
		char s[128];

		if (strlen(arg2) == 0) {
			PrintMessage(ent, S_COLOR_NEGATIVE "You must supply a map name.\n");
			return qfalse;
		}

		/* Does map exist at all? */
		if (!fileExists(va("maps/%s.bsp", arg2))) {
			PrintMessage(ent, S_COLOR_NEGATIVE "Map not found.\n");
			return qfalse;
		}

		trap_Cvar_VariableStringBuffer("mapname", s, sizeof(s));
		if (Q_stricmp(arg2, s) == 0) {
			PrintMessage(ent, S_COLOR_NEGATIVE "This is the current map. Use 'map_restart' if you want to restart.\n");
			return qfalse;
		}

		/* Backup and re-apply current nextmap */
		/* FIXME: This is copied from original code. Is it neccessary? "/map x" does not affect nextmap cvar */
		trap_Cvar_VariableStringBuffer("nextmap", s, sizeof(s));
		if (*s) {
			Com_sprintf(level.voteString, sizeof(level.voteString), "map %s; set nextmap \"%s\"", arg2, s);
		} else {
			Com_sprintf(level.voteString, sizeof(level.voteString), "map %s", arg2);
		}
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), S_COLOR_ITALIC "Map %s" S_COLOR_DEFAULT,
					arg2);

		return qtrue;
	} else if (VOTEID_NEXTMAP == id) {
		char s[128];

		trap_Cvar_VariableStringBuffer("nextmap", s, sizeof(s));
		if (!*s) {
			/* This is unlikely to happen, since nextmap will usually be "map_restart 0" */
			PrintMessage(ent, S_COLOR_NEGATIVE "nextmap is not set on the server.\n");
			return qfalse;
		}

		Q_strncpyz(level.voteString, "vstr nextmap", sizeof(level.voteDisplayString));
		Q_strncpyz(level.voteDisplayString, S_COLOR_ITALIC "Next map" S_COLOR_DEFAULT, sizeof(level.voteDisplayString));

		return qtrue;
	} else if (VOTEID_MAP_RESTART == id) {
		Q_strncpyz(level.voteString, "map_restart", sizeof(level.voteDisplayString));
		Q_strncpyz(level.voteDisplayString, S_COLOR_ITALIC "Restart map" S_COLOR_DEFAULT,
				   sizeof(level.voteDisplayString));

		return qtrue;
	} else {
		G_Error("VoteH_Map: Unknown id %i!\n", id);
		return qfalse;
	}
}

/*
	Handler for any other vote
*/
static qboolean VoteH_Misc(gentity_t *ent, voteID_t id) {
	char arg2[MAX_STRING_TOKENS];
	int i;
	char variable[64]; /* Should be enough to hold common cvar/vote names */

	Q_strncpyz(variable, GetVoteStr(id), sizeof(variable));
	trap_Argv(2, arg2, sizeof(arg2));

	if ((VOTEID_POINTLIMIT == id) || (VOTEID_TIMELIMIT == id)) {

		if (strlen(arg2) == 0) {
			PrintMessage(ent, va(S_COLOR_NEGATIVE "You must supply a %s.\n", variable));
			return qfalse;
		}

		if (!Q_isanumber(arg2)) {
			PrintMessage(ent, S_COLOR_NEGATIVE "You must supply a number.\n");
			return qfalse;
		}

		i = atoi(arg2);

		if (i < 0) {
			PrintMessage(ent, va(S_COLOR_NEGATIVE "Invalid %s.\n", variable));
			return qfalse;
		}

		if (trap_Cvar_VariableIntegerValue(variable) == i) {
			PrintMessage(ent, va(S_COLOR_NEGATIVE "This is the current %s.\n", variable));
			return qfalse;
		}

		Com_sprintf(level.voteString, sizeof(level.voteString), "%s %i", variable, i);
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), S_COLOR_ITALIC "%s" S_COLOR_DEFAULT,
					level.voteString);

		return qtrue;
	}
	/* NOTE: New server command implemented in beryllium's gamecode */
	else if (VOTEID_SHUFFLETEAMS == id) {
		if (g_gametype.integer < GT_TEAM) {
			PrintMessage(ent, S_COLOR_NEGATIVE "Not in a team gametype.\n");
			return qfalse;
		}

		Q_strncpyz(level.voteString, "shuffleteams", sizeof(level.voteDisplayString));
		Q_strncpyz(level.voteDisplayString, S_COLOR_ITALIC "Shuffle teams" S_COLOR_DEFAULT,
				   sizeof(level.voteDisplayString));

		return qtrue;
	} else {
		G_Error("VoteH_Misc: Unknown id %i\n", id);
		return qfalse;
	}
}
