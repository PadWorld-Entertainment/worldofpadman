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
 * name:		ai_team.c
 *
 * desc:		WoP bot AI
 *
 * $Archive: /MissionPack/code/game/ai_team.c $
 *
 *****************************************************************************/

// team leader AI disabled until someone comes up with actual use for it,
// and a fix for multiple bots with the same name in one team

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
#include "ai_vcmd.h"

#include "match.h"

int BotGetTeammates(bot_state_t *bs, int *teammates, int maxteammates);

#if USE_TEAMAI

// cyr{
static int lastorderedgoal[MAX_CLIENTS]; // leader AI, avoid spamming humans with the same MSG
static int lastballoonstate[MAX_BALLOONS];
// cyr}

// ctf task preferences for a client
typedef struct bot_ctftaskpreference_s {
	char name[36];
	int preference;
} bot_ctftaskpreference_t;

static bot_ctftaskpreference_t ctftaskpreferences[MAX_CLIENTS];

/*
==================
BotValidTeamLeader
==================
*/
static int BotValidTeamLeader(bot_state_t *bs) {
	int leadclient;
	if (!strlen(bs->teamleader))
		return qfalse;
	leadclient = ClientFromName(bs->teamleader);
	if (leadclient == -1)
		return qfalse;
	// client in unserem team ?
	if (!BotSameTeam(bs, leadclient))
		return qfalse;
	// client bot ?
	// if this player is not a bot
	if (!(g_entities[leadclient].r.svFlags & SVF_BOT))
		return qfalse;
	return qtrue;
}

/*
==================
BotNumTeamMates
==================
*/
static int BotNumTeamMates(bot_state_t *bs) {
	int i, numplayers;
	char buf[MAX_INFO_STRING];

	numplayers = 0;
	for (i = 0; i < level.maxclients; i++) {
		trap_GetConfigstring(CS_PLAYERS + i, buf, sizeof(buf));
		// if no config string or no name
		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n")))
			continue;
		// skip spectators
		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR)
			continue;

		if (BotSameTeam(bs, i)) {
			numplayers++;
		}
	}
	return numplayers;
}

/*
==================
BotSortTeamMatesByBaseTravelTime
==================
*/
static int BotSortTeamMatesByBaseTravelTime(bot_state_t *bs, int *teammates, int maxteammates) {
	int i, j, k, numteammates, traveltime;
	char buf[MAX_INFO_STRING];
	int traveltimes[MAX_CLIENTS];
	bot_goal_t *goal = NULL;

	if (gametype == GT_CTF) {
		if (BotTeam(bs) == TEAM_RED)
			goal = &ctf_redflag;
		else
			goal = &ctf_blueflag;
	}
	numteammates = 0;
	for (i = 0; i < level.maxclients; i++) {
		trap_GetConfigstring(CS_PLAYERS + i, buf, sizeof(buf));
		// if no config string or no name
		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n")))
			continue;
		// skip spectators
		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR)
			continue;
		if (BotSameTeam(bs, i) && goal) {
			traveltime = BotClientTravelTimeToGoal(i, goal);
			for (j = 0; j < numteammates; j++) {
				if (traveltime < traveltimes[j]) {
					for (k = numteammates; k > j; k--) {
						traveltimes[k] = traveltimes[k - 1];
						teammates[k] = teammates[k - 1];
					}
					break;
				}
			}
			traveltimes[j] = traveltime;
			teammates[j] = i;
			numteammates++;
			if (numteammates >= maxteammates)
				break;
		}
	}
	return numteammates;
}

/*
==================
BotSetTeamMateTaskPreference
==================
*/
static void BotSetTeamMateTaskPreference(bot_state_t *bs, int teammate, int preference) {
	char teammatename[MAX_NETNAME];

	ctftaskpreferences[teammate].preference = preference;
	ClientName(teammate, teammatename, sizeof(teammatename));
	strcpy(ctftaskpreferences[teammate].name, teammatename);
}

/*
==================
BotGetTeamMateTaskPreference
==================
*/
static int BotGetTeamMateTaskPreference(bot_state_t *bs, int teammate) {
	char teammatename[MAX_NETNAME];

	if (!ctftaskpreferences[teammate].preference)
		return 0;
	ClientName(teammate, teammatename, sizeof(teammatename));
	if (Q_stricmp(teammatename, ctftaskpreferences[teammate].name))
		return 0;
	return ctftaskpreferences[teammate].preference;
}

/*
==================
BotSortTeamMatesByTaskPreference
==================
*/
static int BotSortTeamMatesByTaskPreference(bot_state_t *bs, int *teammates, int numteammates) {
	int defenders[MAX_CLIENTS], numdefenders;
	int attackers[MAX_CLIENTS], numattackers;
	int roamers[MAX_CLIENTS], numroamers;
	int i, preference;

	numdefenders = numattackers = numroamers = 0;
	for (i = 0; i < numteammates; i++) {
		preference = BotGetTeamMateTaskPreference(bs, teammates[i]);
		if (preference & TEAMTP_DEFENDER) {
			defenders[numdefenders++] = teammates[i];
		} else if (preference & TEAMTP_ATTACKER) {
			attackers[numattackers++] = teammates[i];
		} else {
			roamers[numroamers++] = teammates[i];
		}
	}
	numteammates = 0;
	// defenders at the front of the list
	memcpy(&teammates[numteammates], defenders, numdefenders * sizeof(int));
	numteammates += numdefenders;
	// roamers in the middle
	memcpy(&teammates[numteammates], roamers, numroamers * sizeof(int));
	numteammates += numroamers;
	// attacker in the back of the list
	memcpy(&teammates[numteammates], attackers, numattackers * sizeof(int));
	numteammates += numattackers;

	return numteammates;
}

static void BotSayTeamOrder(bot_state_t *bs, int toclient) {
	char teamchat[MAX_MESSAGE_SIZE];
	char buf[MAX_MESSAGE_SIZE];
	char name[MAX_NETNAME];

	// if the bot is talking to itself
	if (bs->client == toclient) {
		// don't show the message just put it in the console message queue
		trap_BotGetChatMessage(bs->cs, buf, sizeof(buf));
		ClientName(bs->client, name, sizeof(name));
		Com_sprintf(teamchat, sizeof(teamchat), EC "(%s" EC ")" EC ": %s", name, buf);
		trap_BotQueueConsoleMessage(bs->cs, CMS_CHAT, teamchat);
	} else {
		trap_BotEnterChat(bs->cs, toclient, CHAT_TELL);
	}
}

static void BotInstructMate(bot_state_t *bs, int client, int goal) {
	char name[MAX_NETNAME];

	ClientName(client, name, sizeof(name));
	// G_Printf("ordering %s",name);	// cyr 20055
	if (g_entities[client].r.svFlags & SVF_BOT)
		BotAI_BotInitialChat(bs, "cmd_accompany", name, va("%d", goal), NULL);
	else {
		if (lastorderedgoal[client] == goal + 1)
			return; // dont bother humans with the same MSG
		lastorderedgoal[client] = goal + 1;

		if (goal >= 0)
			BotAI_BotInitialChat(bs, "cmd_accompany", name,
								 va("the %s", g_entities[balloongoal[goal].entitynum].message), NULL);
		else
			BotAI_BotInitialChat(bs, "cmd_accompany", name, va("nothing, just roam"), NULL);
	}
	BotSayTeamOrder(bs, client);
}

static void BotBalloonOrders(bot_state_t *bs) {
	int i, j;
	int index;
	int state;
	int capstate[MAX_BALLOONS];
	int nummates;
	int mates[MAX_CLIENTS];
	int numcap, numnmycap;
	float weight; // 0 - attack, 100 - defend

	// G_Printf(S_COLOR_RED " orders for %d",BotTeam(bs));	// 20055
	// get status of balloons
	numcap = numnmycap = 0;

	for (i = 0; i < level.numBalloons; i++) {
		index = g_entities[balloongoal[i].entitynum].count;
		state = level.balloonState[index]; // status of goal i
		if (BotTeam(bs) == TEAM_RED && state == '1' || BotTeam(bs) == TEAM_BLUE && state == '2') {
			// own balloon
			capstate[i] = 0;
			numcap++;
		} else if (BotTeam(bs) == TEAM_RED && state == '2' || BotTeam(bs) == TEAM_BLUE && state == '1') {
			// nmy balloon
			capstate[i] = 1;
			numnmycap++;
		} else {
			// uncap balloon
			capstate[i] = 2;
		}
	}

	// weight gets 0 if nmy controls and 1 if the bots team controls
	weight = ((numcap - numnmycap) + level.numBalloons) / (2.0 * level.numBalloons);

	if (weight == 0)
		weight = 0.1f;
	if (weight == 1)
		weight = 0.9f;

	// scorealert[BotTeam(bs)] = weight; // should also depend on scorediff and caplimit

	// istruct all, or only respawner
	if (!bs->orderclient) {
		nummates = BotGetTeammates(bs, mates, sizeof(mates));
		// G_Printf("teamorder\n");	// cyr 20055
	} else {
		char netname[MAX_NETNAME];
		mates[0] = bs->orderclient - 1;
		nummates = 1;

		EasyClientName(bs->orderclient - 1, netname, sizeof(netname));
		// G_Printf("individual order for %s \n", netname);	// cyr 20055

		bs->orderclient = 0;
	}

	for (i = 0; i < nummates; i++) {
		// find best goal
		int bestgoal = 0;
		float bestdist, tt, wtt, multiplier;
		bestdist = 9999.9f;

		for (j = 0; j < level.numBalloons; j++) {
			tt = BotClientTravelTimeToGoal(mates[i], &balloongoal[j]);
			// prefer balloons based on current balloon-difference
			if (!capstate[j]) { // our balloon
				multiplier = weight;
			} else {
				if (capstate[j] == 1) // nmy balloon
					multiplier = 1.0f - weight;
				else // uncap balloon
					multiplier = (1.0f - weight) / 2.0f;
			}
			wtt = tt * multiplier * multiplier;

			G_Printf("%f .. %d -> %f -> %f , %d (%f)\n", weight, i, tt, wtt, capstate[j], multiplier);
			if (wtt < bestdist) {
				bestdist = wtt;
				bestgoal = j;
			}
		}
		BotInstructMate(bs, mates[i], bestgoal);
	}
}

static int FindHumanTeamLeader(bot_state_t *bs) {
	int i;

	for (i = 0; i < MAX_CLIENTS; i++) {
		if (g_entities[i].inuse) {
			// if this player is not a bot
			if (!(g_entities[i].r.svFlags & SVF_BOT)) {
				// if this player is ok with being the leader
				if (!notleader[i]) {
					// if this player is on the same team
					if (BotSameTeam(bs, i)) {
						ClientName(i, bs->teamleader, sizeof(bs->teamleader));
						return qtrue;
					}
				}
			}
		}
	}
	return qfalse;
}

/*
==================
BotCreateGroup
==================
*/
static void BotCreateGroup(bot_state_t *bs, int *teammates, int groupsize) {
	char name[MAX_NETNAME], leadername[MAX_NETNAME];
	int i;

	// the others in the group will follow the teammates[0]
	ClientName(teammates[0], leadername, sizeof(leadername));
	for (i = 1; i < groupsize; i++) {
		ClientName(teammates[i], name, sizeof(name));
		if (teammates[0] == bs->client) {
			BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
		} else {
			BotAI_BotInitialChat(bs, "cmd_accompany", name, leadername, NULL);
		}
		BotSayTeamOrder(bs, teammates[i]);
	}
}

/*
==================
BotTeamOrders

  FIXME: defend key areas?
==================
*/
static void BotTeamOrders(bot_state_t *bs) {
	int teammates[MAX_CLIENTS];
	int numteammates, i;
	char buf[MAX_INFO_STRING];

	numteammates = 0;
	for (i = 0; i < level.maxclients; i++) {
		trap_GetConfigstring(CS_PLAYERS + i, buf, sizeof(buf));
		// if no config string or no name
		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n")))
			continue;
		// skip spectators
		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR)
			continue;
		if (BotSameTeam(bs, i)) {
			teammates[numteammates] = i;
			numteammates++;
		}
	}
	switch (numteammates) {
	case 1:
		break;
	case 2: {
		// nothing special
		break;
	}
	case 3: {
		// have one follow another and one free roaming
		BotCreateGroup(bs, teammates, 2);
		break;
	}
	case 4: {
		BotCreateGroup(bs, teammates, 2);	  // a group of 2
		BotCreateGroup(bs, &teammates[2], 2); // a group of 2
		break;
	}
	case 5: {
		BotCreateGroup(bs, teammates, 2);	  // a group of 2
		BotCreateGroup(bs, &teammates[2], 3); // a group of 3
		break;
	}
	default: {
		if (numteammates <= 10) {
			for (i = 0; i < numteammates / 2; i++) {
				BotCreateGroup(bs, &teammates[i * 2], 2); // groups of 2
			}
		}
		break;
	}
	}
}

/*
==================
BotCTFOrders
==================
*/
static void BotCTFOrders_BothFlagsNotAtBase(bot_state_t *bs) {
	int numteammates, defenders, attackers, i, other;
	int teammates[MAX_CLIENTS] = {0};
	char name[MAX_NETNAME], carriername[MAX_NETNAME];

	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
	// different orders based on the number of team mates
	switch (bs->numteammates) {
	case 1:
		break;
	case 2: {
		// tell the one not carrying the flag to attack the enemy base
		if (teammates[0] != bs->flagcarrier)
			other = teammates[0];
		else
			other = teammates[1];
		ClientName(other, name, sizeof(name));
		BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
		BotSayTeamOrder(bs, other);
		break;
	}
	case 3: {
		// tell the one closest to the base not carrying the flag to accompany the flag carrier
		if (teammates[0] != bs->flagcarrier)
			other = teammates[0];
		else
			other = teammates[1];
		ClientName(other, name, sizeof(name));
		if (bs->flagcarrier != -1) {
			ClientName(bs->flagcarrier, carriername, sizeof(carriername));
			if (bs->flagcarrier == bs->client) {
				BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
			} else {
				BotAI_BotInitialChat(bs, "cmd_accompany", name, carriername, NULL);
			}
		} else {
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
		}
		BotSayTeamOrder(bs, other);
		// tell the one furthest from the the base not carrying the flag to get the enemy flag
		if (teammates[2] != bs->flagcarrier)
			other = teammates[2];
		else
			other = teammates[1];
		ClientName(other, name, sizeof(name));
		BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
		BotSayTeamOrder(bs, other);
		break;
	}
	default: {
		defenders = (int)(float)numteammates * 0.4 + 0.5;
		if (defenders > 4)
			defenders = 4;
		attackers = (int)(float)numteammates * 0.5 + 0.5;
		if (attackers > 5)
			attackers = 5;
		if (bs->flagcarrier != -1) {
			ClientName(bs->flagcarrier, carriername, sizeof(carriername));
			for (i = 0; i < defenders; i++) {
				if (teammates[i] == bs->flagcarrier) {
					continue;
				}
				ClientName(teammates[i], name, sizeof(name));
				if (bs->flagcarrier == bs->client) {
					BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
				} else {
					BotAI_BotInitialChat(bs, "cmd_accompany", name, carriername, NULL);
				}
				BotSayTeamOrder(bs, teammates[i]);
			}
		} else {
			for (i = 0; i < defenders; i++) {
				if (teammates[i] == bs->flagcarrier) {
					continue;
				}
				ClientName(teammates[i], name, sizeof(name));
				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
				BotSayTeamOrder(bs, teammates[i]);
			}
		}
		for (i = 0; i < attackers; i++) {
			if (teammates[numteammates - i - 1] == bs->flagcarrier) {
				continue;
			}
			ClientName(teammates[numteammates - i - 1], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
		}
		break;
	}
	}
}

/*
==================
BotCTFOrders
==================
*/
static void BotCTFOrders_FlagNotAtBase(bot_state_t *bs) {
	int numteammates, defenders, attackers, i;
	int teammates[MAX_CLIENTS];
	char name[MAX_NETNAME];

	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
	// passive strategy
	if (!(bs->ctfstrategy & CTFS_AGRESSIVE)) {
		// different orders based on the number of team mates
		switch (bs->numteammates) {
		case 1:
			break;
		case 2: {
			// keep one near the base for when the flag is returned
			ClientName(teammates[0], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
			BotSayTeamOrder(bs, teammates[0]);
			ClientName(teammates[1], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[1]);
			break;
		}
		case 3: {
			// keep one near the base for when the flag is returned
			ClientName(teammates[0], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
			BotSayTeamOrder(bs, teammates[0]);
			// the other two get the flag
			ClientName(teammates[1], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[1]);
			ClientName(teammates[2], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[2]);
			break;
		}
		default: {
			// keep some people near the base for when the flag is returned
			defenders = (int)(float)numteammates * 0.3 + 0.5;
			if (defenders > 3)
				defenders = 3;
			attackers = (int)(float)numteammates * 0.6 + 0.5;
			if (attackers > 6)
				attackers = 6;
			for (i = 0; i < defenders; i++) {
				ClientName(teammates[i], name, sizeof(name));
				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
				BotSayTeamOrder(bs, teammates[i]);
			}
			for (i = 0; i < attackers; i++) {
				ClientName(teammates[numteammates - i - 1], name, sizeof(name));
				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
				BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
			}
			break;
		}
		}
	} else {
		// different orders based on the number of team mates
		switch (bs->numteammates) {
		case 1:
			break;
		case 2: {
			// both will go for the enemy flag
			ClientName(teammates[0], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[0]);
			ClientName(teammates[1], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[1]);
			break;
		}
		case 3: {
			// everyone go for the flag
			ClientName(teammates[0], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[0]);
			ClientName(teammates[1], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[1]);
			ClientName(teammates[2], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[2]);
			break;
		}
		default: {
			// keep some people near the base for when the flag is returned
			defenders = (int)(float)numteammates * 0.2 + 0.5;
			if (defenders > 2)
				defenders = 2;
			attackers = (int)(float)numteammates * 0.7 + 0.5;
			if (attackers > 7)
				attackers = 7;
			for (i = 0; i < defenders; i++) {
				ClientName(teammates[i], name, sizeof(name));
				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
				BotSayTeamOrder(bs, teammates[i]);
			}
			for (i = 0; i < attackers; i++) {
				ClientName(teammates[numteammates - i - 1], name, sizeof(name));
				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
				BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
			}
			break;
		}
		}
	}
}

/*
==================
BotCTFOrders
==================
*/
static void BotCTFOrders_EnemyFlagNotAtBase(bot_state_t *bs) {
	int numteammates, defenders, attackers, i, other;
	int teammates[MAX_CLIENTS];
	char name[MAX_NETNAME], carriername[MAX_NETNAME];

	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
	// different orders based on the number of team mates
	switch (numteammates) {
	case 1:
		break;
	case 2: {
		// tell the one not carrying the flag to defend the base
		if (teammates[0] == bs->flagcarrier)
			other = teammates[1];
		else
			other = teammates[0];
		ClientName(other, name, sizeof(name));
		BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
		BotSayTeamOrder(bs, other);
		break;
	}
	case 3: {
		// tell the one closest to the base not carrying the flag to defend the base
		if (teammates[0] != bs->flagcarrier)
			other = teammates[0];
		else
			other = teammates[1];
		ClientName(other, name, sizeof(name));
		BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
		BotSayTeamOrder(bs, other);
		// tell the other also to defend the base
		if (teammates[2] != bs->flagcarrier)
			other = teammates[2];
		else
			other = teammates[1];
		ClientName(other, name, sizeof(name));
		BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
		BotSayTeamOrder(bs, other);
		break;
	}
	default: {
		// 60% will defend the base
		defenders = (int)(float)numteammates * 0.6 + 0.5;
		if (defenders > 6)
			defenders = 6;
		// 30% accompanies the flag carrier
		attackers = (int)(float)numteammates * 0.3 + 0.5;
		if (attackers > 3)
			attackers = 3;
		for (i = 0; i < defenders; i++) {
			if (teammates[i] == bs->flagcarrier) {
				continue;
			}
			ClientName(teammates[i], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
			BotSayTeamOrder(bs, teammates[i]);
		}
		// if we have a flag carrier
		if (bs->flagcarrier != -1) {
			ClientName(bs->flagcarrier, carriername, sizeof(carriername));
			for (i = 0; i < attackers; i++) {
				if (teammates[numteammates - i - 1] == bs->flagcarrier) {
					continue;
				}
				ClientName(teammates[numteammates - i - 1], name, sizeof(name));
				if (bs->flagcarrier == bs->client) {
					BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
				} else {
					BotAI_BotInitialChat(bs, "cmd_accompany", name, carriername, NULL);
				}
				BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
			}
		} else {
			for (i = 0; i < attackers; i++) {
				if (teammates[numteammates - i - 1] == bs->flagcarrier) {
					continue;
				}
				ClientName(teammates[numteammates - i - 1], name, sizeof(name));
				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
				BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
			}
		}
		break;
	}
	}
}

/*
==================
BotCTFOrders
==================
*/
static void BotCTFOrders_BothFlagsAtBase(bot_state_t *bs) {
	int numteammates, defenders, attackers, i;
	int teammates[MAX_CLIENTS] = {0};
	char name[MAX_NETNAME];

	// sort team mates by travel time to base
	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
	// sort team mates by CTF preference
	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
	// passive strategy
	if (!(bs->ctfstrategy & CTFS_AGRESSIVE)) {
		// different orders based on the number of team mates
		switch (numteammates) {
		case 1:
			break;
		case 2: {
			// the one closest to the base will defend the base
			ClientName(teammates[0], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
			BotSayTeamOrder(bs, teammates[0]);
			// the other will get the flag
			ClientName(teammates[1], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[1]);
			break;
		}
		case 3: {
			// the one closest to the base will defend the base
			ClientName(teammates[0], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
			BotSayTeamOrder(bs, teammates[0]);
			// the second one closest to the base will defend the base
			ClientName(teammates[1], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
			BotSayTeamOrder(bs, teammates[1]);
			// the other will get the flag
			ClientName(teammates[2], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[2]);
			break;
		}
		default: {
			defenders = (int)(float)numteammates * 0.5 + 0.5;
			if (defenders > 5)
				defenders = 5;
			attackers = (int)(float)numteammates * 0.4 + 0.5;
			if (attackers > 4)
				attackers = 4;
			for (i = 0; i < defenders; i++) {
				ClientName(teammates[i], name, sizeof(name));
				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
				BotSayTeamOrder(bs, teammates[i]);
			}
			for (i = 0; i < attackers; i++) {
				ClientName(teammates[numteammates - i - 1], name, sizeof(name));
				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
				BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
			}
			break;
		}
		}
	} else {
		// different orders based on the number of team mates
		switch (numteammates) {
		case 1:
			break;
		case 2: {
			// the one closest to the base will defend the base
			ClientName(teammates[0], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
			BotSayTeamOrder(bs, teammates[0]);
			// the other will get the flag
			ClientName(teammates[1], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[1]);
			break;
		}
		case 3: {
			// the one closest to the base will defend the base
			ClientName(teammates[0], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
			BotSayTeamOrder(bs, teammates[0]);
			// the others should go for the enemy flag
			ClientName(teammates[1], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[1]);
			ClientName(teammates[2], name, sizeof(name));
			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
			BotSayTeamOrder(bs, teammates[2]);
			break;
		}
		default: {
			defenders = (int)(float)numteammates * 0.4 + 0.5;
			if (defenders > 4)
				defenders = 4;
			attackers = (int)(float)numteammates * 0.5 + 0.5;
			if (attackers > 5)
				attackers = 5;
			for (i = 0; i < defenders; i++) {
				ClientName(teammates[i], name, sizeof(name));
				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
				BotSayTeamOrder(bs, teammates[i]);
			}
			for (i = 0; i < attackers; i++) {
				ClientName(teammates[numteammates - i - 1], name, sizeof(name));
				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
				BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
			}
			break;
		}
		}
	}
}

/*
==================
BotCTFOrders
==================
*/
static void BotCTFOrders(bot_state_t *bs) {
	int flagstatus;

	if (BotTeam(bs) == TEAM_RED)
		flagstatus = bs->redflagstatus * 2 + bs->blueflagstatus;
	else
		flagstatus = bs->blueflagstatus * 2 + bs->redflagstatus;
	switch (flagstatus) {
	case 0:
		BotCTFOrders_BothFlagsAtBase(bs);
		break;
	case 1:
		BotCTFOrders_EnemyFlagNotAtBase(bs);
		break;
	case 2:
		BotCTFOrders_FlagNotAtBase(bs);
		break;
	case 3:
		BotCTFOrders_BothFlagsNotAtBase(bs);
		break;
	}
}

#endif

void BotTeamAI(bot_state_t *bs) {
#if USE_TEAMAI
	int numteammates;
	char netname[MAX_NETNAME];
	// cyr{
	int i;
	// cyr}

	if (gametype < GT_TEAM)
		return;
	// make sure we've got a valid team leader
	if (!BotValidTeamLeader(bs)) {
		if (!FindHumanTeamLeader(bs)) {
			if (!bs->askteamleader_time && !bs->becometeamleader_time) {
				if (bs->entergame_time + 10 > FloatTime()) {
					bs->askteamleader_time = FloatTime() + 5 + random() * 10;
				} else {
					bs->becometeamleader_time = FloatTime() + 5 + random() * 10;
				}
			}
			if (bs->askteamleader_time && bs->askteamleader_time < FloatTime()) {
				// if asked for a team leader and no response
				BotAI_BotInitialChat(bs, "whoisteamleader", NULL);
				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
				bs->askteamleader_time = 0;
				bs->becometeamleader_time = FloatTime() + 8 + random() * 10;
			}
			if (bs->becometeamleader_time && bs->becometeamleader_time < FloatTime()) {
				BotAI_BotInitialChat(bs, "iamteamleader", NULL);
				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
				ClientName(bs->client, netname, sizeof(netname));
				Q_strncpyz(bs->teamleader, netname, sizeof(bs->teamleader));
				bs->becometeamleader_time = 0;
			}
			return;
		}
	}
	bs->askteamleader_time = 0;
	bs->becometeamleader_time = 0;

	// return if this bot is NOT the team leader
	ClientName(bs->client, netname, sizeof(netname));
	if (Q_stricmp(netname, bs->teamleader) != 0)
		return;
	numteammates = BotNumTeamMates(bs);
	// give orders
	switch (gametype) {
	case GT_FREEZETAG:
	case GT_TEAM: {
		if (bs->numteammates != numteammates || bs->forceorders) {
			bs->teamgiveorders_time = FloatTime();
			bs->numteammates = numteammates;
			bs->forceorders = qfalse;
		}
		// if it's time to give orders
		if (bs->teamgiveorders_time && bs->teamgiveorders_time < FloatTime() - 5) {
			BotTeamOrders(bs);
			// give orders again after 120 seconds
			bs->teamgiveorders_time = FloatTime() + 120;
		}
		break;
	}
		// cyr{
	case GT_BALLOON: {
		// team changed ? someone needs orders ?
		if (bs->numteammates != numteammates || bs->forceorders) {
			bs->teamgiveorders_time = FloatTime();
			bs->numteammates = numteammates;
			bs->forceorders = qfalse;
		}
		// balloon status changed since last frame?
		for (i = 0; i < MAX_BALLOONS; i++) {
			if (lastballoonstate[i] != level.balloonState[i]) {
				lastballoonstate[i] = level.balloonState[i];
				bs->teamgiveorders_time = FloatTime(); // orders fuer "balloon attacked" ueberspringen wenns geht
			}
		}

		// if it's time to give orders
		if (bs->teamgiveorders_time && bs->teamgiveorders_time < FloatTime() - 2) {
			BotBalloonOrders(bs);
			// give orders again
			bs->teamgiveorders_time = FloatTime() + 5;
		}
		break;
	}
	case GT_CTF: {
		// if the number of team mates changed or the flag status changed
		// or someone wants to know what to do
		if (bs->numteammates != numteammates || bs->flagstatuschanged || bs->forceorders) {
			bs->teamgiveorders_time = FloatTime();
			bs->numteammates = numteammates;
			bs->flagstatuschanged = qfalse;
			bs->forceorders = qfalse;
		}
		// if there were no flag captures the last 3 minutes
		if (bs->lastflagcapture_time < FloatTime() - 240) {
			bs->lastflagcapture_time = FloatTime();
			// randomly change the CTF strategy
			if (random() < 0.4) {
				bs->ctfstrategy ^= CTFS_AGRESSIVE;
				bs->teamgiveorders_time = FloatTime();
			}
		}
		// if it's time to give orders
		if (bs->teamgiveorders_time && bs->teamgiveorders_time < FloatTime() - 3) {
			BotCTFOrders(bs);
			bs->teamgiveorders_time = 0;
		}
		break;
	}
		// cyr}
	}
#endif
}
