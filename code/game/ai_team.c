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

#include "g_local.h"
#include "../botlib/botlib.h"
#include "../botlib/be_aas.h"
#include "../botlib/be_ai_goal.h"
#include "../botlib/be_ai_move.h"
#include "ai_main.h"
#include "ai_cmd.h"
#include "ai_dmq3.h"
#include "ai_team.h"
#include "inv.h"

#define TEAM_PLAN_INTERVAL 3000
#define TEAM_LOLLY_RETRY_INTERVAL 250
#define TEAM_ORDER_TIME 5
#define TEAM_THAW_STALL_TIME 9000
#define TEAM_LOLLY_RUNNER_STALL_TIME 12000

extern bot_state_t *botstates[MAX_CLIENTS];

typedef struct {
	int leader;
	int team;
	int numAvailable;
	bot_state_t *available[MAX_CLIENTS];
} bot_team_plan_t;

qboolean BotTeamAIEnabled(int team) {
	return bot_teamAI.integer && (team == TEAM_RED || team == TEAM_BLUE) &&
		   (bot_teamAITeams.integer & (1 << (team - TEAM_RED)));
}

static qboolean BotIsTeamMember(int client, int team) {
	return g_entities[client].inuse && level.clients[client].pers.connected == CON_CONNECTED &&
		   level.clients[client].sess.sessionTeam == team;
}

static void BotAssignTeamOrder(bot_team_plan_t *plan, int client, int teammate, int type, const bot_goal_t *goal) {
	BotSetTeamOrder(plan->available[client], plan->leader, teammate, type, TEAM_ORDER_TIME, goal);
	plan->available[client] = NULL;
	plan->numAvailable--;
}

static int BotNearestAvailable(const bot_team_plan_t *plan, const bot_goal_t *goal) {
	int i, best = -1, bestTime = 0;

	if (!goal->areanum)
		return -1;
	for (i = 0; i < level.maxclients; i++) {
		int travel;
		if (!plan->available[i])
			continue;
		travel = BotClientTravelTimeToGoal(i, goal);
		if (travel && (!bestTime || travel < bestTime)) {
			best = i;
			bestTime = travel;
		}
	}
	return best;
}

static void BotTeamGroup(bot_team_plan_t *plan) {
	int i;

	// Reserve one bot as the point player for each pair.
	for (i = 0; i < level.maxclients && plan->numAvailable > 1; i++) {
		int follower;
		bot_goal_t goal;

		if (!plan->available[i] || !BotPlayerLocation(i, &goal))
			continue;
		plan->available[i] = NULL;
		plan->numAvailable--;
		follower = BotNearestAvailable(plan, &goal);
		if (follower >= 0)
			BotAssignTeamOrder(plan, follower, i, LTG_TEAMACCOMPANY, &goal);
		if (botstates[i]->orderSource == BOT_ORDER_TEAM)
			BotClearOrder(botstates[i]);
	}
}

static qboolean BotHasTeamRole(const bot_state_t *bs, int teammate, int type) {
	return bs->orderSource == BOT_ORDER_TEAM && bs->ltgtype == type &&
		   bs->teammate == teammate && BotHasActiveOrder(bs);
}

static int BotAssignTeamRole(bot_team_plan_t *plan, int teammate, int type, const bot_goal_t *goal) {
	int i;
	int client = -1;

	for (i = 0; i < level.maxclients; i++) {
		if (plan->available[i] && BotHasTeamRole(plan->available[i], teammate, type) &&
			BotClientTravelTimeToGoal(i, goal)) {
			client = i;
			break;
		}
	}
	if (client < 0)
		client = BotNearestAvailable(plan, goal);
	if (client >= 0)
		BotAssignTeamOrder(plan, client, teammate, type, goal);
	return client;
}

static int BotAssignRescuer(bot_team_plan_t *plan, int target, const bot_goal_t *goal) {
	int i, stalled = -1, rescuer;

	for (i = 0; i < level.maxclients; i++) {
		bot_state_t *bs = botstates[i];
		int travel;

		if (!bs || !bs->inuse || !BotIsTeamMember(i, plan->team) || FT_PlayerIsFrozen(&g_entities[i]) ||
			g_entities[i].health <= 0 || bs->ltgtype != LTG_UNFREEZE || bs->teammate != target ||
			!BotHasActiveOrder(bs))
			continue;
		if (bs->orderSource == BOT_ORDER_PLAYER)
			return i;
		if (!plan->available[i])
			continue;
		travel = BotClientTravelTimeToGoal(i, goal);
		if (!travel)
			continue;
		if (!bs->orderTravelTime || travel + 25 < bs->orderTravelTime ||
			FT_InThawingRange(&g_entities[i], &g_entities[target])) {
			bs->orderTravelTime = travel;
			bs->orderProgressTime = level.time;
		}
		if (level.time - bs->orderProgressTime < TEAM_THAW_STALL_TIME) {
			BotAssignTeamOrder(plan, i, target, LTG_UNFREEZE, goal);
			return i;
		}
		stalled = i;
	}
	if (stalled >= 0)
		plan->available[stalled] = NULL;
	rescuer = BotNearestAvailable(plan, goal);
	if (stalled >= 0)
		plan->available[stalled] = botstates[stalled];
	if (rescuer < 0)
		rescuer = stalled;
	else if (stalled >= 0)
		BotClearOrder(botstates[stalled]);
	if (rescuer >= 0)
		BotAssignTeamOrder(plan, rescuer, target, LTG_UNFREEZE, goal);
	return rescuer;
}

static void BotTeamRescue(bot_team_plan_t *plan) {
	int i, numThreatened = 0;
	int threatened[MAX_CLIENTS];

	for (i = 0; i < level.maxclients && plan->numAvailable; i++) {
		int rescuer, enemy;
		bot_goal_t goal;

		if (!BotIsTeamMember(i, plan->team) || !FT_PlayerIsFrozen(&g_entities[i]) || !BotPlayerLocation(i, &goal))
			continue;
		rescuer = BotAssignRescuer(plan, i, &goal);
		if (rescuer < 0)
			continue;
		enemy = botstates[rescuer]->enemy;
		if (enemy >= 0 && enemy < level.maxclients && BotIsTeamMember(enemy, BotOppositeTeam(botstates[rescuer])) &&
			g_entities[enemy].health > 0 && !FT_PlayerIsFrozen(&g_entities[enemy]) &&
			DistanceSquared(g_entities[enemy].r.currentOrigin, goal.origin) <= Square(512))
			threatened[numThreatened++] = rescuer;
	}
	for (i = 0; i < numThreatened && plan->numAvailable; i++) {
		bot_goal_t goal;
		if (BotPlayerLocation(threatened[i], &goal))
			BotAssignTeamRole(plan, threatened[i], LTG_TEAMACCOMPANY, &goal);
	}
}

static int BotRouteTravelTime(const bot_goal_t *start, const bot_goal_t *end) {
	vec3_t origin;

	VectorCopy(start->origin, origin);
	return trap_AAS_AreaTravelTimeToGoalArea(start->areanum, origin, end->areanum, TFL_DEFAULT);
}

// Hold a reachable point ahead of the carrier. AAS travel times are in hundredths of a second.
static void BotAssignRouteGuard(bot_team_plan_t *plan, int carrier, int type, bot_goal_t *start, const bot_goal_t *base) {
	aas_predictroute_t route;
	bot_goal_t prediction = {0}, bestGoal;
	int direct, i, j, best = -1, bestCost = INT_MAX;

	if (!plan->numAvailable || !base->areanum)
		return;
	direct = BotRouteTravelTime(start, base);
	if (direct < 150)
		return;

	trap_AAS_PredictRoute(&route, start->areanum, start->origin, base->areanum, TFL_DEFAULT,
						  48, 300, 0, 0, 0, 0);
	if (route.stopevent != RSE_NOROUTE && route.endarea != base->areanum && trap_AAS_AreaReachability(route.endarea)) {
		prediction.areanum = route.endarea;
		VectorCopy(route.endpos, prediction.origin);
		VectorSet(prediction.mins, -8, -8, -8);
		VectorSet(prediction.maxs, 8, 8, 8);
	}

	for (i = 0; i <= numwaypoints; i++) {
		const bot_goal_t *point = i == numwaypoints ? &prediction : &waypoints[i].goal;
		int ahead, remaining;

		if (!point->areanum || point->areanum == base->areanum || point->areanum == start->areanum)
			continue;
		ahead = BotRouteTravelTime(start, point);
		remaining = BotRouteTravelTime(point, base);
		if (ahead < 100 || !remaining || remaining >= direct || ahead + remaining > direct + 150)
			continue;
		for (j = 0; j < level.maxclients; j++) {
			const bot_state_t *bs = plan->available[j];
			int travel, cost;

			if (!bs)
				continue;
			travel = BotClientTravelTimeToGoal(j, point);
			if (!travel || travel + 25 >= ahead)
				continue;
			cost = ahead + travel / 2;
			if (BotHasTeamRole(bs, carrier, type) && bs->teamgoal.areanum == point->areanum)
				cost -= 150;
			if (cost < bestCost) {
				best = j;
				bestCost = cost;
				bestGoal = *point;
			}
		}
	}
	if (best >= 0) {
		bestGoal.entitynum = carrier;
		BotAssignTeamOrder(plan, best, carrier, type, &bestGoal);
	}
}

static int BotTeamIntercept(bot_team_plan_t *plan, int flagTeam, int otherTeam, const bot_goal_t *enemyBase) {
	const int carrier = GetTeamFlagCarrier(flagTeam);
	const int count = MIN(3, 1 + plan->numAvailable / 8);
	const int available = plan->numAvailable;
	int i;
	bot_goal_t goal;

	if (carrier < 0 || !BotIsTeamMember(carrier, otherTeam) || !BotPlayerLocation(carrier, &goal))
		return 0;
	for (i = 0; i < count; i++) {
		if (BotAssignTeamRole(plan, carrier, LTG_RECOVERFLAG, &goal) < 0)
			break;
		if (i == 0 && enemyBase && plan->numAvailable > 1)
			BotAssignRouteGuard(plan, carrier, LTG_LOLLYAMBUSH, &goal, enemyBase);
	}
	return available - plan->numAvailable;
}

static int BotChooseLollyRunner(const bot_team_plan_t *plan, const bot_goal_t *flag) {
	int i, best = -1, bestCost = INT_MAX;

	for (i = 0; i < level.maxclients; i++) {
		bot_state_t *bs = plan->available[i];
		qboolean current;
		int travel, health, cost;

		if (!bs)
			continue;
		travel = BotClientTravelTimeToGoal(i, flag);
		health = g_entities[i].health;
		current = bs->teamgoal.entitynum == flag->entitynum && BotHasTeamRole(bs, i, LTG_LOLLYRUNNER);
		if (current && health >= 35) {
			if (travel && (!bs->orderTravelTime || travel + 25 < bs->orderTravelTime)) {
				bs->orderTravelTime = travel;
				bs->orderProgressTime = level.time;
			}
			if (level.time - bs->orderProgressTime < TEAM_LOLLY_RUNNER_STALL_TIME)
				return i;
		}
		if (!travel)
			continue;
		// Prefer a healthy, armed runner over saving a few seconds of travel.
		cost = travel + 5 * MAX(0, 100 - health) +
			   2 * MAX(0, 50 - level.clients[i].ps.stats[STAT_ARMOR]) +
			   MAX(0, 50 - bs->inventory[INV_GOTWEAPON]) * 4;
		if (current)
			cost += 500;
		if (cost < bestCost) {
			best = i;
			bestCost = cost;
		}
	}
	return best;
}

static void BotTeamPushForLolly(bot_team_plan_t *plan, const bot_goal_t *flag) {
	const int groups = 1 + (plan->numAvailable >= 10);
	const int escorts = 1 + (plan->numAvailable >= 4);
	int runners[2], count = 0, i, j;

	// Reserve runners first so a second push keeps its runner across replans.
	for (i = 0; i < groups; i++) {
		const int runner = BotChooseLollyRunner(plan, flag);
		if (runner < 0)
			break;
		runners[count++] = runner;
		BotAssignTeamOrder(plan, runner, runner, LTG_LOLLYRUNNER, flag);
	}
	for (i = 0; i < count; i++) {
		bot_goal_t runnerGoal;

		if (!BotPlayerLocation(runners[i], &runnerGoal))
			continue;
		for (j = 0; j < escorts; j++) {
			if (BotAssignTeamRole(plan, runners[i], LTG_LOLLYESCORT, &runnerGoal) < 0)
				break;
		}
	}
}

static void BotTeamLollyOrders(bot_team_plan_t *plan) {
	const int otherTeam = plan->team == TEAM_RED ? TEAM_BLUE : TEAM_RED;
	const int flagTeam = gametype == GT_1FCTF ? TEAM_FREE : otherTeam;
	const bot_goal_t *base = plan->team == TEAM_RED ? &ctf_redflag : &ctf_blueflag;
	const bot_goal_t *enemyBase = otherTeam == TEAM_RED ? &ctf_redflag : &ctf_blueflag;
	const flagStatus_t status = Team_GetFlagStatus(flagTeam);
	int defenders = plan->numAvailable >= 2 ? (plan->numAvailable + 1) / 3 : 0;
	const int minDefenders = plan->numAvailable >= 4;
	qboolean escorting = qfalse;
	int carrier, client, i;
	bot_goal_t goal, flag = gametype == GT_1FCTF ? ctf_neutralflag : *enemyBase;

	if (gametype == GT_CTF && Team_GetFlagStatus(plan->team) == FLAG_DROPPED) {
		const int retrievers = 1 + (plan->numAvailable >= 4);
		i = 0;
		if (GetDroppedLollyGoal(plan->team, &goal)) {
			for (; i < retrievers; i++) {
				if (BotAssignTeamRole(plan, plan->leader, LTG_PICKUPFLAG, &goal) < 0)
					break;
			}
		}
		if (!i && plan->numAvailable)
			botstates[plan->leader]->teamPlanTime = level.time + TEAM_LOLLY_RETRY_INTERVAL;
		defenders = MAX(minDefenders, defenders - i);
	} else if (gametype == GT_CTF && Team_GetFlagStatus(plan->team) == FLAG_TAKEN) {
		const int interceptors = BotTeamIntercept(plan, plan->team, otherTeam, enemyBase);
		if (!interceptors && plan->numAvailable)
			botstates[plan->leader]->teamPlanTime = level.time + TEAM_LOLLY_RETRY_INTERVAL;
		defenders = MAX(minDefenders, defenders - interceptors);
	}

	if (status != FLAG_ATBASE && status != FLAG_DROPPED) {
		carrier = GetTeamFlagCarrier(flagTeam);
		if (carrier >= 0 && BotIsTeamMember(carrier, plan->team) && BotPlayerLocation(carrier, &goal)) {
			const int escortCount = 1 + (plan->numAvailable >= 8);
			const int escortType = gametype == GT_CTF ? LTG_LOLLYESCORT : LTG_TEAMACCOMPANY;
			escorting = qtrue;
			for (i = 0; i < escortCount; i++) {
				if (BotAssignTeamRole(plan, carrier, escortType, &goal) < 0)
					break;
				if (i == 0 && gametype == GT_CTF && plan->numAvailable > 1)
					BotAssignRouteGuard(plan, carrier, LTG_LOLLYSCREEN, &goal, base);
			}
		} else if (gametype == GT_1FCTF) {
			BotTeamIntercept(plan, TEAM_FREE, otherTeam, NULL);
		}
	}

	for (i = 0; i < defenders; i++) {
		client = BotAssignTeamRole(plan, plan->leader, LTG_DEFENDKEYAREA, base);
		if (client < 0)
			break;
	}

	if (!escorting && (status == FLAG_ATBASE || (status == FLAG_DROPPED && GetDroppedLollyGoal(flagTeam, &flag))) &&
		flag.areanum) {
		if (gametype == GT_CTF) {
			BotTeamPushForLolly(plan, &flag);
		} else {
			for (i = 0; i < level.maxclients; i++) {
				if (plan->available[i] && BotClientTravelTimeToGoal(i, &flag))
					BotAssignTeamOrder(plan, i, plan->leader, LTG_GETFLAG, &flag);
			}
		}
	}
	if (gametype == GT_CTF || escorting) {
		for (i = 0; i < level.maxclients; i++) {
			if (plan->available[i])
				BotAssignTeamOrder(plan, i, plan->leader, LTG_ROAMORDER, base);
		}
	}
}

static void BotTeamBalloonOrders(bot_team_plan_t *plan) {
	const int numBalloons = MIN(level.numBalloons, MAX_BALLOONS);
	int assigned[MAX_BALLOONS] = {0};
	qboolean owned[MAX_BALLOONS];
	int i, j;

	for (j = 0; j < numBalloons; j++)
		owned[j] = G_BalloonIsCaptured(g_entities[balloongoal[j].entitynum].count, plan->team, qtrue);

	for (i = 0; i < level.maxclients; i++) {
		bot_state_t *bs = plan->available[i];
		if (!bs || bs->ltgtype != LTG_BALLCAMP)
			continue;
		for (j = 0; j < numBalloons; j++) {
			if (!owned[j] && balloongoal[j].areanum && bs->teamgoal.entitynum == balloongoal[j].entitynum) {
				BotAssignTeamOrder(plan, i, plan->leader, LTG_BALLCAMP, &balloongoal[j]);
				assigned[j]++;
				break;
			}
		}
	}

	for (i = 0; i < level.maxclients; i++) {
		int best = -1, bestCost = 0;
		if (!plan->available[i])
			continue;
		for (j = 0; j < numBalloons; j++) {
			int travel, cost;
			if (!balloongoal[j].areanum)
				continue;
			travel = BotClientTravelTimeToGoal(i, &balloongoal[j]);
			if (!travel)
				continue;
			cost = travel + (owned[j] ? 150 : 0);
			if (best < 0 || assigned[j] < assigned[best] || (assigned[j] == assigned[best] && cost < bestCost)) {
				best = j;
				bestCost = cost;
			}
		}
		if (best >= 0) {
			BotAssignTeamOrder(plan, i, plan->leader, owned[best] ? LTG_DEFENDKEYAREA : LTG_ATTACKENEMYBASE,
							   &balloongoal[best]);
			assigned[best]++;
		}
	}
}

static void BotTeamSprayOrders(bot_team_plan_t *plan) {
	int i, carrier = -1, mostCarts = 0;
	bot_goal_t goal;

	for (i = 0; i < level.maxclients; i++) {
		int carts;
		if (!BotIsTeamMember(i, plan->team) || g_entities[i].health <= 0 || ClientInSprayroom(i))
			continue;
		carts = level.clients[i].ps.ammo[WP_SPRAYPISTOL];
		if (carts > mostCarts && BotPlayerLocation(i, &goal)) {
			carrier = i;
			mostCarts = carts;
		}
	}
	if (carrier >= 0 && BotPlayerLocation(carrier, &goal)) {
		const int escort = BotNearestAvailable(plan, &goal);
		if (escort >= 0)
			BotAssignTeamOrder(plan, escort, carrier, LTG_TEAMACCOMPANY, &goal);
	}
}

void BotTeamAI(bot_state_t *bs) {
	bot_team_plan_t plan = {0};
	int i, flagCarrier;

	if (gametype < GT_TEAM || level.cammode)
		return;
	if (bs->teamPlanTime > level.time &&
		(gametype != GT_CTF || !level.clients[bs->client].sess.teamLeader ||
		 (bs->teamPlanRedLolly == Team_GetFlagStatus(TEAM_RED) &&
		  bs->teamPlanBlueLolly == Team_GetFlagStatus(TEAM_BLUE))))
		return;
	bs->teamPlanTime = level.time + TEAM_PLAN_INTERVAL;
	plan.team = BotTeam(bs);
	if (!BotTeamAIEnabled(plan.team))
		return;
	plan.leader = TeamLeader(plan.team);
	if (plan.leader < 0) {
		SetLeader(plan.team, bs->client);
		plan.leader = bs->client;
	}
	if (plan.leader != bs->client)
		return;

	if (gametype == GT_CTF) {
		bs->teamPlanRedLolly = Team_GetFlagStatus(TEAM_RED);
		bs->teamPlanBlueLolly = Team_GetFlagStatus(TEAM_BLUE);
	}
	flagCarrier = gametype == GT_CTF ? GetTeamFlagCarrier(BotOppositeTeam(bs)) :
				  gametype == GT_1FCTF ? GetTeamFlagCarrier(TEAM_FREE) : -1;
	for (i = 0; i < level.maxclients; i++) {
		bot_state_t *mate = botstates[i];
		if (!mate || !mate->inuse || !BotIsTeamMember(i, plan.team))
			continue;
		if (BotOrderActive(mate) && mate->orderSource == BOT_ORDER_PLAYER)
			continue;
		if (g_entities[i].health <= 0 || i == flagCarrier || (gametype == GT_FREEZETAG && FT_PlayerIsFrozen(&g_entities[i])) ||
			(gametype == GT_SPRAY && (level.clients[i].ps.ammo[WP_SPRAYPISTOL] || ClientInSprayroom(i) ||
									 mate->ltgtype == LTG_GIVECART || mate->ltgtype == LTG_FETCHCART))) {
			if (mate->orderSource == BOT_ORDER_TEAM)
				BotClearOrder(mate);
			continue;
		}
		plan.available[i] = mate;
		plan.numAvailable++;
	}

	switch (gametype) {
	case GT_FREEZETAG:
		BotTeamRescue(&plan);
		// fall through
	case GT_TEAM:
		BotTeamGroup(&plan);
		break;
	case GT_CTF:
	case GT_1FCTF:
		BotTeamLollyOrders(&plan);
		break;
	case GT_BALLOON:
		BotTeamBalloonOrders(&plan);
		break;
	case GT_SPRAY:
		BotTeamSprayOrders(&plan);
		break;
	}

	for (i = 0; i < level.maxclients; i++) {
		bot_state_t *mate = plan.available[i];
		if (mate && mate->orderSource == BOT_ORDER_TEAM)
			BotClearOrder(mate);
	}
}
