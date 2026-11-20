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

static const char *BotOrderStatus(const bot_state_t *bs);

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
static qboolean BotAddressedToBot(bot_state_t *bs, bot_match_t *match) {
	char addressedto[MAX_MESSAGE_SIZE], botname[MAX_NETNAME];
	int length;

	if (!(match->subtype & ST_ADDRESSED))
		return qtrue;

	trap_BotMatchVariable(match, ADDRESSEE, addressedto, sizeof(addressedto));
	length = strlen(addressedto);
	if (length > 0 && addressedto[length - 1] == ',')
		addressedto[length - 1] = '\0';
	ClientName(bs->client, botname, sizeof(botname));
	return *addressedto && !Q_stricmp(botname, addressedto);
}

static qboolean BotOrderSenderValid(const bot_state_t *bs, int sender) {
	const int team = BotTeam(bs);
	return (team == TEAM_RED || team == TEAM_BLUE) && sender >= 0 && sender < level.maxclients &&
		   g_entities[sender].inuse && g_entities[sender].client &&
		   g_entities[sender].client->pers.connected == CON_CONNECTED && BotSameTeam(bs, sender);
}

static qboolean BotEnemyHasLolly(const bot_state_t *bs) {
	const int target = bs->teammate;
	const int powerup = gametype == GT_1FCTF ? PW_NEUTRALFLAG : BotTeam(bs) == TEAM_RED ? PW_REDFLAG : PW_BLUEFLAG;
	return target >= 0 && target < level.maxclients && g_entities[target].inuse &&
		   level.clients[target].pers.connected == CON_CONNECTED &&
		   level.clients[target].sess.sessionTeam == BotOppositeTeam(bs) && level.clients[target].ps.powerups[powerup];
}

void BotClearOrder(bot_state_t *bs) {
	bs->orderSource = BOT_ORDER_NONE;
	bs->ltgtype = 0;
	bs->teamgoal.areanum = 0;
	bs->teammessage_time = 0;
}

qboolean BotHasActiveOrder(const bot_state_t *bs) {
	return bs->orderSource != BOT_ORDER_NONE && bs->ltgtype && bs->teamgoal_time > FloatTime() &&
		BotOrderSenderValid(bs, bs->decisionmaker) &&
		(bs->orderSource != BOT_ORDER_TEAM ||
		 (BotTeamAIEnabled(BotTeam(bs)) && (g_entities[bs->decisionmaker].r.svFlags & SVF_BOT) &&
		  level.clients[bs->decisionmaker].sess.teamLeader)) &&
		((bs->ltgtype != LTG_TEAMACCOMPANY && bs->ltgtype != LTG_JOINMATE && bs->ltgtype != LTG_GIVECART) ||
		 BotOrderSenderValid(bs, bs->teammate)) &&
		(bs->ltgtype != LTG_LOLLYESCORT ||
		 (BotOrderSenderValid(bs, bs->teammate) && g_entities[bs->teammate].health > 0)) &&
		(bs->ltgtype != LTG_LOLLYRUNNER || Team_GetFlagStatus(BotOppositeTeam(bs)) != FLAG_TAKEN) &&
		(bs->orderSource != BOT_ORDER_TEAM || bs->ltgtype != LTG_PICKUPFLAG ||
		 Team_GetFlagStatus(BotTeam(bs)) == FLAG_DROPPED) &&
		(bs->ltgtype != LTG_UNFREEZE ||
		 (BotOrderSenderValid(bs, bs->teammate) && FT_PlayerIsFrozen(&g_entities[bs->teammate]))) &&
		((bs->ltgtype != LTG_RECOVERFLAG && bs->ltgtype != LTG_LOLLYAMBUSH) || BotEnemyHasLolly(bs)) &&
		(bs->ltgtype != LTG_LOLLYSCREEN ||
		 (BotOrderSenderValid(bs, bs->teammate) &&
		  level.clients[bs->teammate].ps.powerups[BotTeam(bs) == TEAM_RED ? PW_BLUEFLAG : PW_REDFLAG])) &&
		!BotCTFCarryingFlag(bs);
}

qboolean BotOrderActive(bot_state_t *bs) {
	if (BotHasActiveOrder(bs))
		return qtrue;
	if (bs->orderSource != BOT_ORDER_NONE)
		BotClearOrder(bs);
	return qfalse;
}

static void BotSetOrder(bot_state_t *bs, int sender, int teammate, int type, float duration, const bot_goal_t *goal) {
	bs->ltgtype = type;
	bs->decisionmaker = sender;
	bs->teammate = teammate;
	bs->teamgoal_time = FloatTime() + duration;
	bs->teammatevisible_time = FloatTime();
	bs->teammessage_time = 0;
	bs->defendaway_time = 0;
	bs->attackaway_time = 0;
	bs->arrive_time = 0;
	if (goal)
		bs->teamgoal = *goal;
	else
		memset(&bs->teamgoal, 0, sizeof(bs->teamgoal));
}

static void BotSetPlayerOrder(bot_state_t *bs, int sender, int type, float duration, const bot_goal_t *goal) {
	bs->orderSource = BOT_ORDER_PLAYER;
	BotSetOrder(bs, sender, sender, type, duration, goal);
}

void BotSetTeamOrder(bot_state_t *bs, int leader, int teammate, int type, float duration, const bot_goal_t *goal) {
	if (bs->orderSource == BOT_ORDER_TEAM && bs->decisionmaker == leader && bs->ltgtype == type &&
		bs->teammate == teammate && bs->teamgoal.entitynum == goal->entitynum &&
		((type != LTG_LOLLYAMBUSH && type != LTG_LOLLYSCREEN) || bs->teamgoal.areanum == goal->areanum)) {
		bs->teamgoal_time = FloatTime() + duration;
		bs->teamgoal = *goal;
		return;
	}
	bs->orderSource = BOT_ORDER_TEAM;
	BotSetOrder(bs, leader, teammate, type, duration, goal);
	bs->orderTravelTime = 0;
	bs->orderProgressTime = level.time;
	bs->lollyPushTime = 0;
	bs->formation_dist = type == LTG_LOLLYESCORT ? 144 : 112;
	bs->arrive_time = 1;
	if (type == LTG_RECOVERFLAG || type == LTG_PICKUPFLAG)
		bs->nbg_time = 0;
	if (bot_developer.integer & AIDBG_GAMETYPE)
		G_LogPrintf("BotTeamOrder: %d %d %d %d %s\n", leader, bs->client, teammate, goal->entitynum, BotOrderStatus(bs));
}

static void BotReplyToOrder(bot_state_t *bs, int sender, const char *message) {
	if (sender == bs->client || bot_nochat.integer)
		return;
	// A broadcast order must not make a full team reply on the same server frame.
	bs->orderReplyTime = FloatTime() + 0.05f * bs->client;
	bs->orderReplyClient = sender;
	bs->orderReply = message;
}

void BotSendOrderReply(bot_state_t *bs) {
	if (!bs->orderReply || bs->orderReplyTime > FloatTime())
		return;
	if (!bot_nochat.integer && BotOrderSenderValid(bs, bs->orderReplyClient)) {
		BotAI_BotInitialChat(bs, "order_response", bs->orderReply, NULL);
		trap_BotEnterChat(bs->cs, bs->orderReplyClient, CHAT_TELL);
	}
	bs->orderReply = NULL;
}

qboolean BotPlayerLocation(int sender, bot_goal_t *goal) {
	memset(goal, 0, sizeof(*goal));
	VectorCopy(g_entities[sender].client->ps.origin, goal->origin);
	goal->areanum = BotPointAreaNum(goal->origin);
	if (!goal->areanum || !trap_AAS_AreaReachability(goal->areanum))
		return qfalse;
	goal->entitynum = sender;
	VectorSet(goal->mins, -8, -8, -8);
	VectorSet(goal->maxs, 8, 8, 8);
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

static qboolean BotMatch_GoForBalloon(bot_state_t *bs, bot_match_t *match, int sender) {
	char balloon[MAX_MESSAGE_SIZE], *end;
	int i, index = -1, bestTime = 0;
	const int numBalloons = MIN(level.numBalloons, MAX_BALLOONS);
	const qboolean defend = match->type == MSG_DEFENDBALLOON;
	const bot_goal_t *goal;
	int type;

	if (gametype != GT_BALLOON)
		return qfalse;
	trap_BotMatchVariable(match, BALLOON, balloon, sizeof(balloon));
	if (*balloon) {
		long number = strtol(balloon, &end, 10);
		if (end == balloon || (*end && strcmp(end, ".")) || number < 0 || number >= numBalloons)
			return qfalse;
		index = number;
	} else {
		for (i = 0; i < numBalloons; i++) {
			const int id = g_entities[balloongoal[i].entitynum].count;
			int travelTime;
			if (!balloongoal[i].areanum || G_BalloonIsCaptured(id, BotTeam(bs), qtrue) != defend)
				continue;
			travelTime = BotClientTravelTimeToGoal(bs->client, &balloongoal[i]);
			if (travelTime && (!bestTime || travelTime < bestTime)) {
				index = i;
				bestTime = travelTime;
			}
		}
	}
	if (index < 0)
		return qfalse;
	goal = &balloongoal[index];
	if (!goal->areanum)
		return qfalse;
	type = G_BalloonIsCaptured(g_entities[goal->entitynum].count, BotTeam(bs), qtrue) ?
		LTG_DEFENDKEYAREA : LTG_ATTACKENEMYBASE;
	BotSetPlayerOrder(bs, sender, type, type == LTG_DEFENDKEYAREA ? TEAM_DEFENDKEYAREA_TIME : TEAM_ATTACKENEMYBASE_TIME, goal);
	return qtrue;
}

static qboolean BotMatch_Thaw(bot_state_t *bs, int sender, qboolean thawSender) {
	const int first = thawSender ? sender : 0;
	const int end = thawSender ? sender + 1 : level.maxclients;
	int i, teammate = -1, bestTime = 0;
	bot_goal_t goal, bestGoal;

	if (!G_FreezeTag() || FT_PlayerIsFrozen(&g_entities[bs->client]))
		return qfalse;
	for (i = first; i < end; i++) {
		int travelTime;
		if (!BotOrderSenderValid(bs, i) || !FT_PlayerIsFrozen(&g_entities[i]) || !BotPlayerLocation(i, &goal))
			continue;
		travelTime = BotClientTravelTimeToGoal(bs->client, &goal);
		if (travelTime && (!bestTime || travelTime < bestTime)) {
			bestTime = travelTime;
			teammate = i;
			bestGoal = goal;
		}
	}
	if (teammate < 0)
		return qfalse;
	BotSetPlayerOrder(bs, sender, LTG_UNFREEZE, TEAM_UNFREEZETIME, &bestGoal);
	bs->teammate = teammate;
	return qtrue;
}

static qboolean BotMatch_GetItem(bot_state_t *bs, bot_match_t *match, int sender) {
	char itemname[MAX_MESSAGE_SIZE];
	bot_goal_t goal;

	trap_BotMatchVariable(match, ITEM, itemname, sizeof(itemname));
	if (!BotGetItemTeamGoal(itemname, &goal)) {
		BotReplyToOrder(bs, sender, "I cannot find that item.");
		return qfalse;
	}
	BotSetPlayerOrder(bs, sender, LTG_GETITEM, TEAM_GETITEM_TIME, &goal);
#ifdef DEBUG
	BotPrintTeamGoal(bs);
#endif
	return qtrue;
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

static const char *BotOrderStatus(const bot_state_t *bs) {
	switch (bs->ltgtype) {
	case LTG_JOINMATE:
	case LTG_TEAMACCOMPANY:
		return "I am following a teammate.";
	case LTG_CAMPORDER:
		return "I am holding position.";
	case LTG_BALLCAMP:
		return "I am securing a balloon.";
	case LTG_DEFENDKEYAREA:
		return gametype == GT_BALLOON ? "I am defending a balloon." : "I am defending.";
	case LTG_GETFLAG:
		return "I am going for the lolly.";
	case LTG_PICKUPFLAG:
		return gametype == GT_CTF ? "I am retrieving our lolly." : "I am going for the lolly.";
	case LTG_RECOVERFLAG:
		return "I am chasing the enemy lolly carrier.";
	case LTG_LOLLYAMBUSH:
		return "I am cutting off the enemy lolly carrier.";
	case LTG_LOLLYSCREEN:
		return "I am clearing the return route.";
	case LTG_LOLLYRUNNER:
		return "I am leading a push for the lolly.";
	case LTG_LOLLYESCORT:
		return "I am covering the lolly runner.";
	case LTG_CAPTUREFLAG:
		return "I am carrying the lolly.";
	case LTG_GETITEM:
		return "I am collecting an item.";
	case LTG_GIVECART:
		return "I am bringing cartridges.";
	case LTG_UNFREEZE:
		return "I am thawing a teammate.";
	case LTG_ATTACKENEMYBASE:
		return "I am attacking.";
	default:
		return "I am roaming.";
	}
}

static const char *BotTeamAdvice(int team, int client) {
	switch (gametype) {
	case GT_CTF:
		if (Team_GetFlagStatus(team) != FLAG_ATBASE)
			return "Recover our lolly.";
		return "Get the enemy lolly.";
	case GT_1FCTF:
		return "Go for the neutral lolly.";
	case GT_BALLOON:
		return "Help secure a balloon.";
	case GT_SPRAY:
		return level.clients[client].ps.ammo[WP_SPRAYPISTOL] ? "Bring your cartridges to the spray room." :
													  "Collect spray cartridges.";
	case GT_FREEZETAG:
		if (FT_NearestFrozenPlayer(&g_entities[client]))
			return "Thaw a teammate.";
		// fall through
	default:
		return "Stick with a teammate.";
	}
}

static qboolean BotApplyOrder(bot_state_t *bs, bot_match_t *match, int sender) {
	const int team = BotTeam(bs);
	bot_goal_t goal;
	const bot_goal_t *base;

	switch (match->type) {
	case MSG_STARTTEAMLEADERSHIP:
		if (TeamLeader(team) != sender)
			SetLeader(team, sender);
		return qtrue;
	case MSG_STOPTEAMLEADERSHIP:
		if (TeamLeader(team) == sender) {
			g_entities[sender].client->sess.teamLeader = qfalse;
			ClientUserinfoChanged(sender);
		}
		return qtrue;
	case MSG_CATCHME:
	case MSG_FOLLOW:
		if (sender == bs->client || !BotPlayerLocation(sender, &goal))
			break;
		BotSetPlayerOrder(bs, sender, match->type == MSG_CATCHME ? LTG_JOINMATE : LTG_TEAMACCOMPANY,
						  TEAM_ACCOMPANY_TIME, &goal);
		bs->formation_dist = 112;
		return qtrue;
	case MSG_CAMP:
		if (!BotPlayerLocation(sender, &goal))
			break;
		BotSetPlayerOrder(bs, sender, LTG_CAMPORDER, TEAM_CAMP_TIME, &goal);
		return qtrue;
	case MSG_ROAM:
		BotSetPlayerOrder(bs, sender, LTG_ROAMORDER, TEAM_GETITEM_TIME, NULL);
		return qtrue;
	case MSG_DEFEND:
		if (gametype != GT_CTF && gametype != GT_1FCTF)
			break;
		base = team == TEAM_RED ? &ctf_redflag : &ctf_blueflag;
		if (!base->areanum)
			break;
		BotSetPlayerOrder(bs, sender, LTG_DEFENDKEYAREA, TEAM_DEFENDKEYAREA_TIME, base);
		return qtrue;
	case MSG_GETLOLLY: {
		const int flagTeam = gametype == GT_1FCTF ? TEAM_FREE : BotOppositeTeam(bs);
		if (gametype != GT_CTF && gametype != GT_1FCTF)
			break;
		base = gametype == GT_1FCTF ? &ctf_neutralflag : team == TEAM_RED ? &ctf_blueflag : &ctf_redflag;
		if (!base->areanum || BotCTFCarryingFlag(bs))
			break;
		goal = *base;
		if (Team_GetFlagStatus(flagTeam) == FLAG_DROPPED && !GetDroppedLollyGoal(flagTeam, &goal))
			break;
		BotSetPlayerOrder(bs, sender, LTG_GETFLAG, CTF_RUSHBASE_TIME, &goal);
		return qtrue;
	}
	case MSG_GOFORBALLOON:
	case MSG_DEFENDBALLOON:
		if (BotMatch_GoForBalloon(bs, match, sender))
			return qtrue;
		break;
	case MSG_THAW:
	case MSG_THAWME:
		if (BotMatch_Thaw(bs, sender, match->type == MSG_THAWME))
			return qtrue;
		break;
	case MSG_DROPCART:
		if (gametype != GT_SPRAY || sender == bs->client || !bs->cur_ps.ammo[WP_SPRAYPISTOL] ||
			!BotPlayerLocation(sender, &goal))
			break;
		BotSetPlayerOrder(bs, sender, LTG_GIVECART, SYC_CART_EXCHANGE_TIME, &goal);
		return qtrue;
	case MSG_GETITEM:
		return BotMatch_GetItem(bs, match, sender);
	default:
		return qfalse;
	}

	BotReplyToOrder(bs, sender, "I cannot do that right now.");
	return qfalse;
}

int BotMatchMessage(bot_state_t *bs, const char *message, int sender) {
	bot_match_t match;
	const int context = sender < 0 ? MTCONTEXT_MISC : MTCONTEXT_INITIALTEAMCHAT;

	if (!trap_BotFindMatch(message, &match, context)) {
		if (bot_developer.integer & AIDBG_CHAT)
			Com_Printf(S_COLOR_GREEN "no match for ^1%s\n", message);
		return qfalse;
	}

	if (sender < 0) {
		if (match.type == MSG_WRONGWALL)
			BotMatch_WrongWall(bs, &match);
		return qtrue;
	}

	if (!BotOrderSenderValid(bs, sender) || !BotAddressedToBot(bs, &match))
		return qfalse;

	if (match.type == MSG_REPORT) {
		BotOrderActive(bs);
		BotReplyToOrder(bs, sender, BotOrderStatus(bs));
	} else if (match.type == MSG_WHOISTEAMLAEDER || match.type == MSG_WHATISMYCOMMAND) {
		if (TeamLeader(BotTeam(bs)) == bs->client)
			BotReplyToOrder(bs, sender, match.type == MSG_WHOISTEAMLAEDER ? "I am the team leader." :
																			  BotTeamAdvice(BotTeam(bs), sender));
	} else if (BotCTFCarryingFlag(bs) && match.type != MSG_STARTTEAMLEADERSHIP && match.type != MSG_STOPTEAMLEADERSHIP) {
		BotReplyToOrder(bs, sender, "I am carrying the lolly.");
	} else if (BotApplyOrder(bs, &match, sender)) {
		BotReplyToOrder(bs, sender, "Roger.");
	}
	return qtrue;
}
