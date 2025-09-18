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

/*****************************************************************************
 * name:		ai_cmd.c
 *
 * desc:		WoP bot AI
 *
 * $Archive: /MissionPack/code/game/ai_cmd.c $
 *
 *****************************************************************************/

#include "g_local.h"
#include "../botlib/botlib.h"
#include "../botlib/be_aas.h"
#include "../botlib/be_ea.h"
#include "../botlib/be_ai_char.h"
#include "../botlib/be_ai_chat.h"
#include "../botlib/be_ai_gen.h"
#include "../botlib/be_ai_goal.h"
#include "../botlib/be_ai_move.h"
#include "../botlib/be_ai_weap.h"
//
#include "ai_main.h"
#include "ai_dmq3.h"
#include "ai_chat.h"
#include "ai_cmd.h"
#include "ai_dmnet.h"
#include "ai_team.h"
//
#include "chars.h" //characteristics
#include "inv.h"   //indexes into the inventory
#include "syn.h"   //synonyms
#include "match.h" //string matching types and vars

int notleader[MAX_CLIENTS];

#ifdef DEBUG
/*
==================
BotPrintTeamGoal
==================
*/
static void BotPrintTeamGoal(bot_state_t *bs) {
	char netname[MAX_NETNAME];
	float t;

	ClientName(bs->client, netname, sizeof(netname));
	t = bs->teamgoal_time - FloatTime();
	switch (bs->ltgtype) {
	case LTG_TEAMHELP: {
		BotAI_Print(PRT_MESSAGE, "%s: I'm gonna help a team mate for %1.0f secs\n", netname, t);
		break;
	}
	case LTG_TEAMACCOMPANY: {
		BotAI_Print(PRT_MESSAGE, "%s: I'm gonna accompany a team mate for %1.0f secs\n", netname, t);
		break;
	}
	case LTG_GETFLAG: {
		BotAI_Print(PRT_MESSAGE, "%s: I'm gonna get the flag for %1.0f secs\n", netname, t);
		break;
	}
	case LTG_RUSHBASE: {
		BotAI_Print(PRT_MESSAGE, "%s: I'm gonna rush to the base for %1.0f secs\n", netname, t);
		break;
	}
#if 0
	case LTG_RETURNFLAG: {
		BotAI_Print(PRT_MESSAGE, "%s: I'm gonna try to return the flag for %1.0f secs\n", netname, t);
		break;
	}
#endif
	case LTG_DEFENDKEYAREA: {
		BotAI_Print(PRT_MESSAGE, "%s: I'm gonna defend a key area for %1.0f secs\n", netname, t);
		break;
	}
	case LTG_GETITEM: {
		BotAI_Print(PRT_MESSAGE, "%s: I'm gonna get an item for %1.0f secs\n", netname, t);
		break;
	}
#if 0
	case LTG_KILL: {
		BotAI_Print(PRT_MESSAGE, "%s: I'm gonna kill someone for %1.0f secs\n", netname, t);
		break;
	}
	case LTG_CAMP:
	case LTG_CAMPORDER: {
		BotAI_Print(PRT_MESSAGE, "%s: I'm gonna camp for %1.0f secs\n", netname, t);
		break;
	}
	case LTG_PATROL: {
		BotAI_Print(PRT_MESSAGE, "%s: I'm gonna patrol for %1.0f secs\n", netname, t);
		break;
	}
#endif
	default: {
#if 0
		if (bs->ctfroam_time > FloatTime()) {
			t = bs->ctfroam_time - FloatTime();
			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna roam for %1.0f secs\n", netname, t);
		} else
#endif
		{
			BotAI_Print(PRT_MESSAGE, "%s: I've got a regular goal\n", netname);
		}
	}
	}
}
#endif // DEBUG

/*
==================
BotGetItemTeamGoal

FIXME: add stuff like "upper rocket launcher"
"the rl near the railgun", "lower grenade launcher" etc.
==================
*/
static int BotGetItemTeamGoal(char *goalname, bot_goal_t *goal) {
	int i;

	if (!strlen(goalname))
		return qfalse;
	i = -1;
	do {
		i = trap_BotGetLevelItemGoal(i, goalname, goal);
		if (i > 0) {
			// do NOT defend dropped items
			if (goal->flags & GFL_DROPPED)
				continue;
			return qtrue;
		}
	} while (i > 0);
	return qfalse;
}

/*
==================
BotGetMessageTeamGoal
==================
*/
static int BotGetMessageTeamGoal(bot_state_t *bs, char *goalname, bot_goal_t *goal) {
	if (BotGetItemTeamGoal(goalname, goal))
		return qtrue;
	return qfalse;
}

/*
==================
FindClientByName
==================
*/
static int FindClientByName(char *name) {
	int i;
	char buf[MAX_INFO_STRING];

	for (i = 0; i < level.maxclients; i++) {
		ClientName(i, buf, sizeof(buf));
		if (!Q_stricmp(buf, name))
			return i;
	}
	for (i = 0; i < level.maxclients; i++) {
		ClientName(i, buf, sizeof(buf));
		if (Q_stristr(buf, name))
			return i;
	}
	return -1;
}

#if 0
/*
==================
FindEnemyByName
==================
*/
static int FindEnemyByName(bot_state_t *bs, char *name) {
	int i;
	char buf[MAX_INFO_STRING];

	for (i = 0; i < level.maxclients; i++) {
		if (BotSameTeam(bs, i))
			continue;
		ClientName(i, buf, sizeof(buf));
		if (!Q_stricmp(buf, name))
			return i;
	}
	for (i = 0; i < level.maxclients; i++) {
		if (BotSameTeam(bs, i))
			continue;
		ClientName(i, buf, sizeof(buf));
		if (Q_stristr(buf, name))
			return i;
	}
	return -1;
}

/*
==================
NumPlayersOnSameTeam
==================
*/
static int NumPlayersOnSameTeam(bot_state_t *bs) {
	int i, num;
	char buf[MAX_INFO_STRING];

	num = 0;
	for (i = 0; i < level.maxclients; i++) {
		trap_GetConfigstring(CS_PLAYERS + i, buf, MAX_INFO_STRING);
		if (strlen(buf)) {
			if (BotSameTeam(bs, i + 1))
				num++;
		}
	}
	return num;
}
#endif

/*
==================
BotAddressedToBot
==================
*/
static int BotAddressedToBot(bot_state_t *bs, bot_match_t *match) {
	char addressedto[MAX_MESSAGE_SIZE];
	char netname[MAX_MESSAGE_SIZE];
	// char name[MAX_MESSAGE_SIZE];
	char botname[128];
	int client;
	// char buf[MAX_MESSAGE_SIZE];

	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
	client = ClientOnSameTeamFromName(bs, netname);
	if (client < 0)
		return qfalse;

	// if the message is addressed to someone
	if (match->subtype & ST_ADDRESSED) {
		// compare addressee with botname
		trap_BotMatchVariable(match, ADDRESSEE, addressedto, sizeof(addressedto));
		ClientName(bs->client, botname, sizeof(botname));

		// is that me?
		if (strlen(addressedto) && Q_stristr(botname, addressedto)) {
			return qtrue;
		}

		// no its not!
		if (bot_developer.integer & AIDBG_CHAT) {
			// Com_sprintf(buf, sizeof(buf), "not addressed to me but %s", addressedto);
			// trap_EA_Say(bs->client, buf);
		}

		return qfalse;
	} else {
		bot_match_t tellmatch;

		tellmatch.type = 0;
		// if this message wasn't directed solely to this bot
		if (!trap_BotFindMatch(match->string, &tellmatch, MTCONTEXT_REPLYCHAT) || tellmatch.type != MSG_CHATTELL) {
			// make sure not everyone reacts to this message
			if (random() > (float)1.0 / (NumPlayersOnSameTeam(bs) - 1))
				return qfalse;
		}
	}
	return qtrue;
}

static void BotMatch_WrongWall(bot_state_t *bs, bot_match_t *match) {
	char netname[MAX_MESSAGE_SIZE];
	char buf[MAX_INFO_STRING];
	int client;

	if (gametype != GT_SPRAY)
		return;

	// talking about me ? (avoid clientfromname, its ambiguous)
	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
	trap_GetConfigstring(CS_PLAYERS + bs->client, buf, sizeof(buf));
	Q_CleanStr(buf);
	if (!Q_stricmp(Info_ValueForKey(buf, "n"), netname)) {
		// could be someone with same name, so make (more) sure
		if (ClientInSprayroom(bs->client)) {
			bs->which_wall = BotChooseCorrectWall(bs);
			bs->enemy = -1;
			// chat
			BotAI_BotInitialChat(bs, "wall_missed", NULL);
			trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
			return;
		}
	}
	// check if opposite team
	client = ClientFromName(netname);
	if (!BotSameTeam(bs, client)) {
		float rnd;
		// flame
		rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_INSULT, 0, 1);
		if (random() > rnd)
			return;
		BotAI_BotInitialChat(bs, "wall_insult", netname, NULL);
		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
	}
}

/*
==================
BotMatch_GoForBalloon
==================
*/
static void BotMatch_GoForBalloon(bot_state_t *bs, bot_match_t *match) {
#if 0
	int client;
	char balloonindex[MAX_MESSAGE_SIZE];
	char netname[MAX_MESSAGE_SIZE];

	int ballindex;
	int state;

	if (!TeamPlayIsOn())
		return;
	// if not addressed to this bot
	if (!BotAddressedToBot(bs, match))
		return;

	if (gametype != GT_BALLOON)
		return;

	// get the team mate name
	trap_BotMatchVariable(match, BALLOON, balloonindex, sizeof(balloonindex));

	ballindex = atoi(balloonindex);

	if (ballindex != -1) { // valid balloon index
		trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
		client = ClientFromName(netname);
		bs->decisionmaker = client;
		// bs->ordered = qtrue;
		bs->order_time = FloatTime();
		bs->teammessage_time = FloatTime() + 2 * random();

		state = level.balloonState[ballindex];
		// not our balloon?
		if (!G_BalloonIsCaptured(ballindex, BotTeam(bs), qtrue) {
			bs->ltgtype = LTG_ATTACKENEMYBASE;
			bs->teamgoal_time = FloatTime() + TEAM_BALLOONATTACK_TIME;

			if (bot_developer.integer & AIDBG_CHAT) {
				char botname[128];
				ClientName(bs->client, botname, sizeof(botname));
				Com_Printf("%s attacking %s \n", botname, g_entities[balloongoal[ballindex].entitynum].message);
			}
		} else {
			// DEFEND !!11
			// cyr_20055
			// reset defendaway_time only if the ltgtype changes.
			// so roaming defenders keep on roaming
			if (bs->ltgtype != LTG_DEFENDKEYAREA) {
				bs->ltgtype = LTG_DEFENDKEYAREA;
				bs->defendaway_time = 0;
			}
			bs->teamgoal_time = FloatTime() + TEAM_BALLOONDEFEND_TIME;

			if (bot_developer.integer & AIDBG_CHAT) {
				char botname[128];
				ClientName(bs->client, botname, sizeof(botname));
				Com_Printf("%s defending %s \n", botname, g_entities[balloongoal[ballindex].entitynum].message);
			}
		}
		memcpy(&bs->teamgoal, &balloongoal[ballindex], sizeof(bs->teamgoal));
		// remember last ordered task
		BotRememberLastOrderedTask(bs);
	} else
		bs->ltgtype = 0; // roam
#endif
}

static void BotMatch_CatchMe(bot_state_t *bs, bot_match_t *match) {
	char netname[MAX_MESSAGE_SIZE];
	int client;

	// if not addressed to this bot
	if (!TeamPlayIsOn() || !BotAddressedToBot(bs, match))
		return;

	// who wants me to to come?
	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
	client = ClientOnSameTeamFromName(bs, netname);

	bs->teammate = client;
	bs->teammatevisible_time = FloatTime(); // last time the team mate was assumed visible

	bs->decisionmaker = client;
	// bs->ordered = qtrue;
	// bs->order_time = FloatTime();

	// set the time to send a message to the team mates
	bs->teammessage_time = FloatTime() + 2 * random();
	// set the ltg type
	bs->ltgtype = LTG_JOINMATE;
	// set the team goal time
	bs->teamgoal_time = FloatTime() + 300; // 5 minutes
}

/*
==================
BotMatch_GetItem
==================
*/
static void BotMatch_GetItem(bot_state_t *bs, bot_match_t *match) {
	char itemname[MAX_MESSAGE_SIZE];
	char netname[MAX_MESSAGE_SIZE];
	int client;

	if (!TeamPlayIsOn())
		return;
	// if not addressed to this bot
	if (!BotAddressedToBot(bs, match))
		return;
	// get the match variable
	trap_BotMatchVariable(match, ITEM, itemname, sizeof(itemname));

	if (!BotGetMessageTeamGoal(bs, itemname, &bs->teamgoal)) {
		BotAI_BotInitialChat(bs, "cannotfind", itemname, NULL);
		trap_BotEnterChat(bs->cs, bs->client, CHAT_TEAM);
		return;
	}
	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
	client = ClientOnSameTeamFromName(bs, netname);

	bs->decisionmaker = client;
	// bs->ordered = qtrue;
	// bs->order_time = FloatTime();
	// set the time to send a message to the team mates
	bs->teammessage_time = FloatTime() + 1 * random();
	// set the ltg type
	bs->ltgtype = LTG_GETITEM;
	// set the team goal time
	bs->teamgoal_time = FloatTime() + TEAM_GETITEM_TIME;

#ifdef DEBUG
	BotPrintTeamGoal(bs);
#endif // DEBUG
}

#if 0
/*
==================
BotNearestVisibleItem
==================
*/
float BotNearestVisibleItem(bot_state_t *bs, const char *itemname, bot_goal_t *goal) {
	int i;
	char name[64];
	bot_goal_t tmpgoal;
	float dist, bestdist;
	vec3_t dir;
	bsp_trace_t trace;

	bestdist = 999999;
	i = -1;
	do {
		i = trap_BotGetLevelItemGoal(i, itemname, &tmpgoal);
		trap_BotGoalName(tmpgoal.number, name, sizeof(name));
		if (Q_stricmp(itemname, name) != 0)
			continue;
		VectorSubtract(tmpgoal.origin, bs->origin, dir);
		dist = VectorLength(dir);
		if (dist < bestdist) {
			// trace from start to end
			BotAI_Trace(&trace, bs->eye, NULL, NULL, tmpgoal.origin, bs->client, CONTENTS_SOLID | CONTENTS_PLAYERCLIP);
			if (trace.fraction >= 1.0) {
				bestdist = dist;
				memcpy(goal, &tmpgoal, sizeof(*goal));
			}
		}
	} while (i > 0);
	return bestdist;
}
#endif

static void BotMatch_EnterGame(bot_state_t *bs, bot_match_t *match) {
	int client;
	char netname[MAX_NETNAME];

	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
	client = FindClientByName(netname);
	if (client >= 0) {
		notleader[client] = qfalse;
	}
}

// cyr_drop{
static void BotMatch_DropCart(bot_state_t *bs, bot_match_t *match) {
	int client;
	char netname[MAX_MESSAGE_SIZE];
	aas_entityinfo_t entinfo;

	if (!TeamPlayIsOn())
		return;
	// if not addressed to this bot
	if (!BotAddressedToBot(bs, match))
		return;

	// get the netname
	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
	client = ClientFromName(netname);

	// if the bot doesn't know who to help (FindClientByName returned -1)
	if (client < 0) {
		BotAI_BotInitialChat(bs, "whois", netname, NULL);
		client = ClientFromName(netname);
		trap_BotEnterChat(bs->cs, client, CHAT_TELL);
		return;
	}
	// don't help or accompany yourself
	if (client == bs->client) {
		return;
	}

	bs->teamgoal.entitynum = -1;
	BotEntityInfo(client, &entinfo);
	// if info is valid (in PVS)
	if (entinfo.valid) {
		const int areanum = BotPointAreaNum(entinfo.origin);
		if (areanum) { // && trap_AAS_AreaReachability(areanum)) {
			bs->teamgoal.entitynum = client;
			bs->teamgoal.areanum = areanum;
			VectorCopy(entinfo.origin, bs->teamgoal.origin);
			VectorSet(bs->teamgoal.mins, -8, -8, -8);
			VectorSet(bs->teamgoal.maxs, 8, 8, 8);
		}
	}

	if (bs->teamgoal.entitynum < 0) {
		BotAI_BotInitialChat(bs, "whereareyou", netname, NULL);
		client = ClientFromName(netname);
		trap_BotEnterChat(bs->cs, client, CHAT_TEAM);
		return;
	}
	// the team mate
	bs->teammate = client;

	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));

	client = ClientFromName(netname);
	// the team mate who ordered
	bs->decisionmaker = client;
	// bs->ordered = qtrue;
	// bs->order_time = FloatTime();
	// last time the team mate was assumed visible
	bs->teammatevisible_time = FloatTime();
	// set the time to send a message to the team mates
	bs->teammessage_time = FloatTime() + 1 * random();
	// set the ltg type
	bs->ltgtype = LTG_GIVECART;
	// Com_Printf(S_COLOR_BLUE "giving cart! \n");	// cyr 20055
	bs->teamgoal_time = FloatTime() + SYC_CART_EXCHANGE_TIME;
}
// cyr_drop}

/*
==================
BotMatchMessage
==================
*/
int BotMatchMessage(bot_state_t *bs, const char *message) {
	bot_match_t match;

	match.type = 0;
	// if it is an unknown message
	if (!trap_BotFindMatch(message, &match, MTCONTEXT_MISC | MTCONTEXT_INITIALTEAMCHAT)) {
		if (bot_developer.integer & AIDBG_CHAT) {
			Com_Printf(S_COLOR_GREEN "no match for ^1%s\n", message);
		}
		return qfalse;
	}

	if (bot_developer.integer & AIDBG_CHAT) {
		Com_Printf(S_COLOR_MAGENTA "match %d for " S_COLOR_RED "%s\n", match.type, message);
	}

	// react to the found message
	switch (match.type) {
	case MSG_WRONGWALL: {
		BotMatch_WrongWall(bs, &match);
		break;
	}
	case MSG_GOFORBALLOON: {				//someone calling for company
		BotMatch_GoForBalloon(bs, &match);
		break;
	}
	case MSG_DROPCART: {
		BotMatch_DropCart(bs, &match);
		break;
	}
	case MSG_GETITEM: {
		BotMatch_GetItem(bs, &match);
		break;
	}
	case MSG_ENTERGAME: { // someone entered the game
		BotMatch_EnterGame(bs, &match);
		break;
	}
	case MSG_CATCHME: {
		BotMatch_CatchMe(bs, &match);
		break;
	}
	default: {
		if (bot_developer.integer)
			BotAI_Print(PRT_MESSAGE, "unknown match type %d\n", match.type);
		break;
	}
	}
	return qtrue;
}
