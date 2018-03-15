/*****************************************************************************
 * name:		ai_team.c
 *****************************************************************************/

// team leader AI disabled until someone comes up with actual use for it,
// and a fix for multiple bots with the same name in one team

#if 1
#define REMOVETHISWHENFILEISUSEDAGAIN
int muh;
#else

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


//cyr{
int lastorderedgoal[MAX_CLIENTS];	// leader AI, aviod spamming humans with the same MSG
int lastballoonstate[MAX_BALLOONS];
//cyr}



int BotValidTeamLeader(bot_state_t *bs) {
	int leadclient;
	if (!strlen(bs->teamleader)) return qfalse;
	leadclient = ClientFromName(bs->teamleader);
	if (leadclient == -1) return qfalse;
	// client in unserem team ?
	if(!BotSameTeam(bs, leadclient)) return qfalse;
	// client bot ?
	// if this player is not a bot
	if ( !(g_entities[leadclient].r.svFlags & SVF_BOT) ) return qfalse;
	return qtrue;
}

int BotNumTeamMates(bot_state_t *bs) {
	int i, numplayers;
	char buf[MAX_INFO_STRING];
	static int maxclients;

	if (!maxclients)
		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");

	numplayers = 0;
	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
		//if no config string or no name
		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
		//skip spectators
		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
		//
		if (BotSameTeam(bs, i)) {
			numplayers++;
		}
	}
	return numplayers;
}

int BotSortTeamMatesByBaseTravelTime(bot_state_t *bs, int *teammates, int maxteammates) {

	int i, j, k, numteammates, traveltime;
	char buf[MAX_INFO_STRING];
	static int maxclients;
	int traveltimes[MAX_CLIENTS];
	bot_goal_t *goal = NULL;

	if (!maxclients)
		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");

	numteammates = 0;
	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
		//if no config string or no name
		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
		//skip spectators
		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
		//
		if (BotSameTeam(bs, i)) {
			//
			traveltime = BotClientTravelTimeToGoal(i, goal);
			//
			for (j = 0; j < numteammates; j++) {
				if (traveltime < traveltimes[j]) {
					for (k = numteammates; k > j; k--) {
						traveltimes[k] = traveltimes[k-1];
						teammates[k] = teammates[k-1];
					}
					break;
				}
			}
			traveltimes[j] = traveltime;
			teammates[j] = i;
			numteammates++;
			if (numteammates >= maxteammates) break;
		}
	}
	return numteammates;
}

void BotSayTeamOrderAlways(bot_state_t *bs, int toclient) {
	char teamchat[MAX_MESSAGE_SIZE];
	char buf[MAX_MESSAGE_SIZE];
	char name[MAX_NETNAME];

	//if the bot is talking to itself
	if (bs->client == toclient) {
		//don't show the message just put it in the console message queue
		trap_BotGetChatMessage(bs->cs, buf, sizeof(buf));
		ClientName(bs->client, name, sizeof(name));
		Com_sprintf(teamchat, sizeof(teamchat), EC"(%s"EC")"EC": %s", name, buf);
		trap_BotQueueConsoleMessage(bs->cs, CMS_CHAT, teamchat);
	}
	else {
		trap_BotEnterChat(bs->cs, toclient, CHAT_TELL);
	}
}

int BotGetTeammates(bot_state_t *bs, int *teammates, int maxteammates) {

	int i, numteammates;
	char buf[MAX_INFO_STRING];
	static int maxclients;


	if (!maxclients)
		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");

	//G_Printf("mates: ");

	numteammates = 0;
	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
		//if no config string or no name
		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
		//skip spectators
		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
		//
		if (BotSameTeam(bs, i)) {
			//
			teammates[numteammates++] = i;
			if (numteammates >= maxteammates) break;
			//G_Printf("/%s ", Info_ValueForKey(buf, "n"));
		}
	}
	//G_Printf("\n %d mates \n", numteammates);
	return numteammates;
}


void BotInstructMate(bot_state_t* bs, int client ,int goal){
    char name[MAX_NETNAME];

	ClientName(client, name, sizeof(name));
	//G_Printf("ordering %s",name);	// cyr 20055
    if( g_entities[client].r.svFlags & SVF_BOT )
            BotAI_BotInitialChat(bs, "cmd_accompany", name, va("%d", goal), NULL);
    else{
		if(lastorderedgoal[client] == goal+1) return;	//dont bother humans with the same MSG
		lastorderedgoal[client] = goal+1;

        if( goal >= 0)
            BotAI_BotInitialChat(bs, "cmd_accompany", name,
            va("the %s",g_entities[ balloongoal[goal].entitynum ].message ), NULL);
        else
            BotAI_BotInitialChat(bs, "cmd_accompany", name, va("nothing, just roam"), NULL);
    }
    BotSayTeamOrder(bs, client);
}

void BotBalloonOrders(bot_state_t *bs) {
	int i, j;
	int index;
	int state;
	int capstate[MAX_BALLOONS];
	int nummates;
	int mates[MAX_CLIENTS];
	int numcap, numnmycap;
	float weight;	// 0 - attack, 100 - defend

	//G_Printf("^1 orders for %d",BotTeam(bs));	// 20055
	// get status of balloons
	numcap = numnmycap = 0;

	for(i=0; i < level.numBalloons; i++){
		index = g_entities[ balloongoal[i].entitynum ].count;
		state = level.balloonState[index];		// status of goal i
		if( BotTeam(bs) == TEAM_RED && state == '1' ||
			BotTeam(bs) == TEAM_BLUE && state == '2' ){
			// own balloon
		   	capstate[i]=0;
		   	numcap++;
		}
		else if(BotTeam(bs) == TEAM_RED && state == '2' ||
				BotTeam(bs) == TEAM_BLUE && state == '1'){
			// nmy balloon
		     capstate[i]=1;
		     numnmycap++;
		}
		else{
			// uncap balloon
			capstate[i]=2;
		}
	}

	//weight gets 0 if nmy controls and 1 if the bots team controls
	weight = ((numcap - numnmycap) + level.numBalloons ) / (2.0 * level.numBalloons) ;
	
	if(weight == 0) weight = 0.1f;
	if(weight == 1) weight = 0.9f;

	scorealert[BotTeam(bs)] = weight; //should also depend on scorediff and caplimit

	// istruct all, or only respawner
	if(! bs->orderclient){
		nummates = BotGetTeammates( bs, mates, sizeof(mates) );
		//G_Printf("teamorder\n");	// cyr 20055
	}
	else{
		char netname[MAX_NETNAME];
		mates[0] = bs->orderclient -1;
		nummates = 1;

		EasyClientName(bs->orderclient -1, netname, sizeof(netname));
		//G_Printf("individual order for %s \n", netname);	// cyr 20055

		bs->orderclient = 0;
	}


	for(i=0; i<nummates;i++){
		// find best goal
		int bestgoal = 0;
		float bestdist, tt, wtt, multiplier;
		bestdist = 9999.9f;

		for(j=0; j<level.numBalloons;j++){
			tt = BotClientTravelTimeToGoal( mates[i], &balloongoal[j] );
			// prefer balloons based on current balloon-difference
			if(!capstate[j]){	// our balloon
				multiplier = weight;
			}
			else{
				if(capstate[j] == 1)	// nmy balloon
					multiplier = 1.0 - weight;
				else // uncap balloon
					multiplier = (1.0 - weight) / 2;
			}
			wtt = tt * multiplier * multiplier;

			G_Printf("%f .. %d -> %f -> %f , %d (%f)\n", weight, i, tt, wtt, capstate[j], multiplier);
			if(wtt < bestdist){
				bestdist = wtt;
				bestgoal = j;
			}
			
		}
		BotInstructMate(bs, mates[i], bestgoal);
	}

}

int FindHumanTeamLeader(bot_state_t *bs) {
	int i;

	for (i = 0; i < MAX_CLIENTS; i++) {
		if ( g_entities[i].inuse ) {
			// if this player is not a bot
			if ( !(g_entities[i].r.svFlags & SVF_BOT) ) {
				// if this player is ok with being the leader
				if (!notleader[i]) {
					// if this player is on the same team
					if ( BotSameTeam(bs, i) ) {
						ClientName(i, bs->teamleader, sizeof(bs->teamleader));
						return qtrue;
					}
				}
			}
		}
	}
	return qfalse;

}

void BotTeamAI(bot_state_t *bs) {

	int numteammates;
	char netname[MAX_NETNAME];
	//cyr{
	int i;
	//cyr}

	//
	if ( gametype < GT_TEAM  )
		return;
	// make sure we've got a valid team leader
	if (!BotValidTeamLeader(bs)) {
		//
		if (qtrue){     // cyr (!FindHumanTeamLeader(bs)) {
			//
			if (!bs->askteamleader_time && !bs->becometeamleader_time) {
				if (bs->entergame_time + 10 > FloatTime()) {
					bs->askteamleader_time = FloatTime() + 5 + random() * 10;
				}
				else {
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
				strncpy(bs->teamleader, netname, sizeof(bs->teamleader));
				bs->teamleader[sizeof(bs->teamleader)] = '\0';
				bs->becometeamleader_time = 0;
			}
			return;
		}
	}
	bs->askteamleader_time = 0;
	bs->becometeamleader_time = 0;

	//return if this bot is NOT the team leader
	ClientName(bs->client, netname, sizeof(netname));
	if (Q_stricmp(netname, bs->teamleader) != 0) return;
	//
	numteammates = BotNumTeamMates(bs);
	//give orders
	switch(gametype) {
		case GT_TEAM:
		{
			if (bs->numteammates != numteammates || bs->forceorders) {
				bs->teamgiveorders_time = FloatTime();
				bs->numteammates = numteammates;
				bs->forceorders = qfalse;
			}
			//if it's time to give orders
			if (bs->teamgiveorders_time && bs->teamgiveorders_time < FloatTime() - 5) {
				BotTeamOrders(bs);
				//give orders again after 120 seconds
				bs->teamgiveorders_time = FloatTime() + 120;
			}
			break;
		}
//cyr{
		case GT_BALLOON:
		{
			// team changed ? someone needs orders ?
			if (bs->numteammates != numteammates || bs->forceorders) {
				bs->teamgiveorders_time = FloatTime();
				bs->numteammates = numteammates;
				bs->forceorders = qfalse;
			}
			// balloon status changed since last frame?
			for(i = 0; i < MAX_BALLOONS; i++){
				if(lastballoonstate[i] != level.balloonState[i]){
					lastballoonstate[i] = level.balloonState[i];
					bs->teamgiveorders_time = FloatTime();	// orders für "balloon attacked" überspringen wenns geht
				}
			}

			//if it's time to give orders
			if (bs->teamgiveorders_time && bs->teamgiveorders_time < FloatTime() - 2) {
                BotBalloonOrders(bs);
                //give orders again
                bs->teamgiveorders_time = FloatTime() + 5;
            }
			break;
		}
//cyr}
	}

}
#endif
