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

/*****************************************************************************
 * name:		be_ai_goal.c
 *
 * desc:		goal AI
 *
 * $Archive: /MissionPack/code/botlib/be_ai_goal.c $
 *
 *****************************************************************************/

#include "../qcommon/q_shared.h"
#include "l_utils.h"
#include "l_libvar.h"
#include "l_memory.h"
#include "l_log.h"
#include "l_script.h"
#include "l_precomp.h"
#include "l_struct.h"
#include "aasfile.h"
#include "botlib.h"
#include "be_aas.h"
#include "be_aas_funcs.h"
#include "be_interface.h"
#include "be_ai_weight.h"
#include "be_ai_goal.h"
#include "be_ai_move.h"

//#define DEBUG_AI_GOAL
//#ifdef RANDOMIZE
#define UNDECIDEDFUZZY
//#endif //RANDOMIZE
//#define DROPPEDWEIGHT
// minimum avoid goal time
#define AVOID_MINIMUM_TIME 10
// default avoid goal time
#define AVOID_DEFAULT_TIME 30
// avoid dropped goal time
#define AVOID_DROPPED_TIME 10

#define TRAVELTIME_SCALE 0.01
// item flags
#define IFL_NOTFREE 1	// not in free for all
#define IFL_NOTTEAM 2	// not in team play
#define IFL_NOTSINGLE 4 // not in single player
#define IFL_NOTBOT 8	// bot should never go for this
#define IFL_ROAM 16		// bot roam goal

// location in the map "target_location"
typedef struct maplocation_s {
	vec3_t origin;
	int areanum;
	char name[MAX_EPAIRKEY];
	struct maplocation_s *next;
} maplocation_t;

// camp spots "info_camp"
typedef struct campspot_s {
	vec3_t origin;
	int areanum;
	char name[MAX_EPAIRKEY];
	float range;
	float weight;
	float wait;
	float random;
	struct campspot_s *next;
} campspot_t;

// FIXME: these are game specific
typedef enum {
	GT_FFA,			  // free for all
	GT_TOURNAMENT,	  // one on one tournament
	GT_SINGLE_PLAYER, // single player tournament

	//-- team games go after this --

	GT_TEAM, // team deathmatch
	GT_CTF,	 // capture the flag
	GT_MAX_GAME_TYPE
} gametype_t;

typedef struct levelitem_s {
	int number;		   // number of the level item
	int iteminfo;	   // index into the item info
	int flags;		   // item flags
	float weight;	   // fixed roam weight
	float respawntime; // experimental for another roam setting
	vec3_t origin;	   // origin of the item
	int goalareanum;   // area the item is in
	vec3_t goalorigin; // goal origin within the area
	int entitynum;	   // entity number
	float timeout;	   // item is removed after this time
	struct levelitem_s *prev, *next;
} levelitem_t;

typedef struct iteminfo_s {
	char classname[32];			 // classname of the item
	char name[MAX_STRINGFIELD];	 // name of the item
	char model[MAX_STRINGFIELD]; // model of the item
	int modelindex;				 // model index
	int type;					 // item type
	int index;					 // index in the inventory
	float respawntime;			 // respawn time
	vec3_t mins;				 // mins of the item
	vec3_t maxs;				 // maxs of the item
	int number;					 // number of the item info
} iteminfo_t;

#define ITEMINFO_OFS(x) (size_t)(&(((iteminfo_t *)0)->x))

static const fielddef_t iteminfo_fields[] = {{"name", ITEMINFO_OFS(name), FT_STRING, 0, 0.0f, 0.0f, NULL},
											 {"model", ITEMINFO_OFS(model), FT_STRING, 0, 0.0f, 0.0f, NULL},
											 {"modelindex", ITEMINFO_OFS(modelindex), FT_INT, 0, 0.0f, 0.0f, NULL},
											 {"type", ITEMINFO_OFS(type), FT_INT, 0, 0.0f, 0.0f, NULL},
											 {"index", ITEMINFO_OFS(index), FT_INT, 0, 0.0f, 0.0f, NULL},
											 {"respawntime", ITEMINFO_OFS(respawntime), FT_FLOAT, 0, 0.0f, 0.0f, NULL},
											 {"mins", ITEMINFO_OFS(mins), FT_FLOAT | FT_ARRAY, 3, 0.0f, 0.0f, NULL},
											 {"maxs", ITEMINFO_OFS(maxs), FT_FLOAT | FT_ARRAY, 3, 0.0f, 0.0f, NULL},
											 {NULL, 0, 0, 0, 0.0f, 0.0f, NULL}};

static const structdef_t iteminfo_struct = {sizeof(iteminfo_t), iteminfo_fields};

typedef struct itemconfig_s {
	int numiteminfo;
	iteminfo_t *iteminfo;
} itemconfig_t;

// goal state
typedef struct bot_goalstate_s {
	struct weightconfig_s *itemweightconfig; // weight config
	int *itemweightindex;					 // index from item to weight

	int client;				  // client using this goal state
	int lastreachabilityarea; // last area with reachabilities the bot was in

	bot_goal_t goalstack[MAX_GOALSTACK]; // goal stack
	int goalstacktop;					 // the top of the goal stack

	int avoidgoals[MAX_AVOIDGOALS];		  // goals to avoid
	float avoidgoaltimes[MAX_AVOIDGOALS]; // times to avoid the goals
} bot_goalstate_t;

bot_goalstate_t *botgoalstates[MAX_CLIENTS + 1]; // FIXME: init?
// item configuration
itemconfig_t *itemconfig = NULL;
// level items
levelitem_t *levelitemheap = NULL;
levelitem_t *freelevelitems = NULL;
levelitem_t *levelitems = NULL;
levelitem_t *spoton_li = NULL; // cyr
int numlevelitems = 0;
// map locations
maplocation_t *maplocations = NULL;
// camp spots
campspot_t *campspots = NULL;
// the game type
int g_gametype = 0;
// additional dropped item weight
libvar_t *droppedweight = NULL;

//========================================================================

//========================================================================
// cyr{

void PrintCurItemInfo() {
	if (spoton_li == NULL) {
		botimport.Print(PRT_MESSAGE, "no item\n");
		return;
	}

	if (!itemconfig)
		return;
	botimport.Print(PRT_MESSAGE, "itemtype %s \n", itemconfig->iteminfo[spoton_li->iteminfo].classname);
}

void GetNextItemNumber(int *ent, int *goal) {
	// if not inside the chain, go to start
	if (spoton_li == NULL) {
		if (levelitems == NULL) { // no list? no number
			*ent = 0;
			return;
		} else
			spoton_li = levelitems;
	} else // move along the chain
		spoton_li = spoton_li->prev;

	// reached end of list?
	if (spoton_li == NULL) {
		*ent = 0;
		return;
	}

	*ent = spoton_li->entitynum;
	*goal = spoton_li->goalareanum;
	return;
}

// cyr}

bot_goalstate_t *BotGoalStateFromHandle(int handle) {
	if (handle <= 0 || handle > MAX_CLIENTS) {
		botimport.Print(PRT_FATAL, "goal state handle %d out of range\n", handle);
		return NULL;
	}
	if (!botgoalstates[handle]) {
		botimport.Print(PRT_FATAL, "invalid goal state %d\n", handle);
		return NULL;
	}
	return botgoalstates[handle];
}

void BotInterbreedGoalFuzzyLogic(int parent1, int parent2, int child) {
	bot_goalstate_t *p1, *p2, *c;

	p1 = BotGoalStateFromHandle(parent1);
	p2 = BotGoalStateFromHandle(parent2);
	c = BotGoalStateFromHandle(child);

	if (!p1 || !p2 || !c)
		return;

	InterbreedWeightConfigs(p1->itemweightconfig, p2->itemweightconfig, c->itemweightconfig);
}

void BotSaveGoalFuzzyLogic(int goalstate, const char *filename) {
	// bot_goalstate_t *gs;

	// gs = BotGoalStateFromHandle(goalstate);
	// if (!gs) return;
	// WriteWeightConfig(filename, gs->itemweightconfig);
}

void BotMutateGoalFuzzyLogic(int goalstate, float range) {
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return;
	EvolveWeightConfig(gs->itemweightconfig);
}

itemconfig_t *LoadItemConfig(const char *filename) {
	int max_iteminfo;
	token_t token;
	char path[MAX_QPATH];
	source_t *source;
	itemconfig_t *ic;
	iteminfo_t *ii;

	max_iteminfo = (int)LibVarValue("max_iteminfo", "256");
	if (max_iteminfo < 0) {
		botimport.Print(PRT_ERROR, "max_iteminfo = %d\n", max_iteminfo);
		max_iteminfo = 256;
		LibVarSet("max_iteminfo", "256");
	}

	Q_strncpyz(path, filename, sizeof(path));
	PC_SetBaseFolder(BOTFILESBASEFOLDER);
	source = LoadSourceFile(path);
	if (!source) {
		botimport.Print(PRT_ERROR, "counldn't load %s\n", path);
		return NULL;
	}
	// initialize item config
	ic = (itemconfig_t *)GetClearedHunkMemory(sizeof(itemconfig_t) + max_iteminfo * sizeof(iteminfo_t));
	ic->iteminfo = (iteminfo_t *)((char *)ic + sizeof(itemconfig_t));
	ic->numiteminfo = 0;
	// parse the item config file
	while (PC_ReadToken(source, &token)) {
		if (!strcmp(token.string, "iteminfo")) {
			if (ic->numiteminfo >= max_iteminfo) {
				SourceError(source, "more than %d item info defined", max_iteminfo);
				FreeMemory(ic);
				FreeSource(source);
				return NULL;
			}
			ii = &ic->iteminfo[ic->numiteminfo];
			Com_Memset(ii, 0, sizeof(iteminfo_t));
			if (!PC_ExpectTokenType(source, TT_STRING, 0, &token)) {
				FreeMemory(ic);
				FreeSource(source);
				return NULL;
			}
			StripDoubleQuotes(token.string);
			Q_strncpyz(ii->classname, token.string, sizeof(ii->classname));
			if (!ReadStructure(source, &iteminfo_struct, (char *)ii)) {
				FreeMemory(ic);
				FreeSource(source);
				return NULL;
			}
			ii->number = ic->numiteminfo;
			ic->numiteminfo++;
		} else {
			SourceError(source, "unknown definition %s", token.string);
			FreeMemory(ic);
			FreeSource(source);
			return NULL;
		}
	}
	FreeSource(source);

	if (!ic->numiteminfo)
		botimport.Print(PRT_WARNING, "no item info loaded\n");
	botimport.Print(PRT_MESSAGE, "loaded %s\n", path);
	return ic;
}
//===========================================================================
// index to find the weight function of an iteminfo

//===========================================================================
int *ItemWeightIndex(weightconfig_t *iwc, itemconfig_t *ic) {
	int *index, i;

	// initialize item weight index
	index = (int *)GetClearedMemory(sizeof(int) * ic->numiteminfo);

	for (i = 0; i < ic->numiteminfo; i++) {
		index[i] = FindFuzzyWeight(iwc, ic->iteminfo[i].classname);
		if (index[i] < 0) {
			Log_Write("item info %d \"%s\" has no fuzzy weight\r\n", i, ic->iteminfo[i].classname);
		}
	}
	return index;
}

void InitLevelItemHeap(void) {
	int i, max_levelitems;

	if (levelitemheap)
		FreeMemory(levelitemheap);

	max_levelitems = (int)LibVarValue("max_levelitems", "256");
	levelitemheap = (levelitem_t *)GetClearedMemory(max_levelitems * sizeof(levelitem_t));

	for (i = 0; i < max_levelitems - 1; i++) {
		levelitemheap[i].next = &levelitemheap[i + 1];
	}
	levelitemheap[max_levelitems - 1].next = NULL;

	freelevelitems = levelitemheap;
}

levelitem_t *AllocLevelItem(void) {
	levelitem_t *li;

	li = freelevelitems;
	if (!li) {
		botimport.Print(PRT_FATAL, "out of level items\n");
		return NULL;
	}

	freelevelitems = freelevelitems->next;
	Com_Memset(li, 0, sizeof(levelitem_t));
	return li;
}

void FreeLevelItem(levelitem_t *li) {
	li->next = freelevelitems;
	freelevelitems = li;
}

void AddLevelItemToList(levelitem_t *li) {
	if (levelitems)
		levelitems->prev = li;
	li->prev = NULL;
	li->next = levelitems;
	levelitems = li;
}

void RemoveLevelItemFromList(levelitem_t *li) {
	if (li->prev)
		li->prev->next = li->next;
	else
		levelitems = li->next;
	if (li->next)
		li->next->prev = li->prev;
}

void BotFreeInfoEntities(void) {
	maplocation_t *ml, *nextml;
	campspot_t *cs, *nextcs;

	for (ml = maplocations; ml; ml = nextml) {
		nextml = ml->next;
		FreeMemory(ml);
	}
	maplocations = NULL;
	for (cs = campspots; cs; cs = nextcs) {
		nextcs = cs->next;
		FreeMemory(cs);
	}
	campspots = NULL;
}

void BotInitInfoEntities(void) {
	char classname[MAX_EPAIRKEY];
	maplocation_t *ml;
	campspot_t *cs;
	int ent, numlocations, numcampspots;

	BotFreeInfoEntities();

	numlocations = 0;
	numcampspots = 0;
	for (ent = AAS_NextBSPEntity(0); ent; ent = AAS_NextBSPEntity(ent)) {
		if (!AAS_ValueForBSPEpairKey(ent, "classname", classname, MAX_EPAIRKEY))
			continue;

		// map locations
		if (!strcmp(classname, "target_location")) {
			ml = (maplocation_t *)GetClearedMemory(sizeof(maplocation_t));
			AAS_VectorForBSPEpairKey(ent, "origin", ml->origin);
			AAS_ValueForBSPEpairKey(ent, "message", ml->name, sizeof(ml->name));
			ml->areanum = AAS_PointAreaNum(ml->origin);
			ml->next = maplocations;
			maplocations = ml;
			numlocations++;
		}
		// camp spots
		else if (!strcmp(classname, "info_camp")) {
			cs = (campspot_t *)GetClearedMemory(sizeof(campspot_t));
			AAS_VectorForBSPEpairKey(ent, "origin", cs->origin);
			// cs->origin[2] += 16;
			AAS_ValueForBSPEpairKey(ent, "message", cs->name, sizeof(cs->name));
			AAS_FloatForBSPEpairKey(ent, "range", &cs->range);
			AAS_FloatForBSPEpairKey(ent, "weight", &cs->weight);
			AAS_FloatForBSPEpairKey(ent, "wait", &cs->wait);
			AAS_FloatForBSPEpairKey(ent, "random", &cs->random);
			cs->areanum = AAS_PointAreaNum(cs->origin);
			if (!cs->areanum) {
				botimport.Print(PRT_MESSAGE, "camp spot at %1.1f %1.1f %1.1f in solid\n", cs->origin[0], cs->origin[1],
								cs->origin[2]);
				FreeMemory(cs);
				continue;
			}
			cs->next = campspots;
			campspots = cs;
			// AAS_DrawPermanentCross(cs->origin, 4, LINECOLOR_YELLOW);
			numcampspots++;
		}
	}
	if (botDeveloper) {
		botimport.Print(PRT_MESSAGE, "%d map locations\n", numlocations);
		botimport.Print(PRT_MESSAGE, "%d camp spots\n", numcampspots);
	}
}

void BotInitLevelItems(void) {
	int i, spawnflags, value;
	char classname[MAX_EPAIRKEY];
	vec3_t origin, end;
	int ent, goalareanum;
	itemconfig_t *ic;
	levelitem_t *li;
	bsp_trace_t trace;

	// initialize the map locations and camp spots
	BotInitInfoEntities();

	// initialize the level item heap
	InitLevelItemHeap();
	levelitems = NULL;
	numlevelitems = 0;

	ic = itemconfig;
	if (!ic)
		return;

	// if there's no AAS file loaded
	if (!AAS_Loaded())
		return;

	// validate the modelindexes of the item info
	for (i = 0; i < ic->numiteminfo; i++) {
		if (!ic->iteminfo[i].modelindex) {
			Log_Write("item %s has modelindex 0", ic->iteminfo[i].classname);
		}
	}

	for (ent = AAS_NextBSPEntity(0); ent; ent = AAS_NextBSPEntity(ent)) {
		if (!AAS_ValueForBSPEpairKey(ent, "classname", classname, MAX_EPAIRKEY))
			continue;

		spawnflags = 0;
		AAS_IntForBSPEpairKey(ent, "spawnflags", &spawnflags);

		for (i = 0; i < ic->numiteminfo; i++) {
			if (!strcmp(classname, ic->iteminfo[i].classname))
				break;
		}
		if (i >= ic->numiteminfo) {
			Log_Write("entity %s unknown item\r\n", classname);
			continue;
		}
		// get the origin of the item
		if (!AAS_VectorForBSPEpairKey(ent, "origin", origin)) {
			botimport.Print(PRT_ERROR, "item %s without origin\n", classname);
			continue;
		}

		goalareanum = 0;
		// if it is a floating item
		if (spawnflags & 1) {
			// if the item is not floating in water
			if (!(AAS_PointContents(origin) & CONTENTS_WATER)) {
				VectorCopy(origin, end);
				end[2] -= 32;
				trace = AAS_Trace(origin, ic->iteminfo[i].mins, ic->iteminfo[i].maxs, end, -1,
								  CONTENTS_SOLID | CONTENTS_PLAYERCLIP);
				// if the item not near the ground
				if (trace.fraction >= 1) {
					// if the item is not reachable from a jumppad
					goalareanum = AAS_BestReachableFromJumpPadArea(origin, ic->iteminfo[i].mins, ic->iteminfo[i].maxs);
					Log_Write("item %s reachable from jumppad area %d\r\n", ic->iteminfo[i].classname, goalareanum);
					// botimport.Print(PRT_MESSAGE, "item %s reachable from jumppad area %d\r\n",
					// ic->iteminfo[i].classname, goalareanum);
					if (!goalareanum)
						continue;
				}
			}
		}

		li = AllocLevelItem();
		if (!li)
			return;

		li->number = ++numlevelitems;
		li->timeout = 0;
		li->entitynum = 0;

		li->flags = 0;
		AAS_IntForBSPEpairKey(ent, "notfree", &value);
		if (value)
			li->flags |= IFL_NOTFREE;
		AAS_IntForBSPEpairKey(ent, "notteam", &value);
		if (value)
			li->flags |= IFL_NOTTEAM;
		AAS_IntForBSPEpairKey(ent, "notsingle", &value);
		if (value)
			li->flags |= IFL_NOTSINGLE;
		AAS_IntForBSPEpairKey(ent, "notbot", &value);
		if (value)
			li->flags |= IFL_NOTBOT;
		if (!strcmp(classname, "item_botroam")) {
			li->flags |= IFL_ROAM;
			AAS_FloatForBSPEpairKey(ent, "weight", &li->weight);
			AAS_FloatForBSPEpairKey(ent, "respawntime", &li->respawntime);
		}
		// if not a stationary item
		if (!(spawnflags & 1)) {
			if (!AAS_DropToFloor(origin, ic->iteminfo[i].mins, ic->iteminfo[i].maxs)) {
				botimport.Print(PRT_MESSAGE, "%s in solid at (%1.1f %1.1f %1.1f)\n", classname, origin[0], origin[1],
								origin[2]);
			}
		}
		// item info of the level item
		li->iteminfo = i;
		// origin of the item
		VectorCopy(origin, li->origin);

		if (goalareanum) {
			li->goalareanum = goalareanum;
			VectorCopy(origin, li->goalorigin);
		} else {
			// get the item goal area and goal origin
			li->goalareanum = AAS_BestReachableArea(origin, ic->iteminfo[i].mins, ic->iteminfo[i].maxs, li->goalorigin);
			if (!li->goalareanum) {
				botimport.Print(PRT_MESSAGE, "%s not reachable for bots at (%1.1f %1.1f %1.1f)\n", classname, origin[0],
								origin[1], origin[2]);
			}
		}

		AddLevelItemToList(li);
	}
	botimport.Print(PRT_MESSAGE, "found %d level items\n", numlevelitems);
}

void BotGoalName(int number, char *name, int size) {
	levelitem_t *li;

	if (!itemconfig)
		return;

	for (li = levelitems; li; li = li->next) {
		if (li->number == number) {
			Q_strncpyz(name, itemconfig->iteminfo[li->iteminfo].name, size);
			return;
		}
	}
	strcpy(name, "");
}

void BotResetAvoidGoals(int goalstate) {
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return;
	Com_Memset(gs->avoidgoals, 0, MAX_AVOIDGOALS * sizeof(int));
	Com_Memset(gs->avoidgoaltimes, 0, MAX_AVOIDGOALS * sizeof(float));
}

void BotDumpAvoidGoals(int goalstate) {
	int i;
	bot_goalstate_t *gs;
	char name[32];

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return;
	for (i = 0; i < MAX_AVOIDGOALS; i++) {
		if (gs->avoidgoaltimes[i] >= AAS_Time()) {
			BotGoalName(gs->avoidgoals[i], name, 32);
			Log_Write("avoid goal %s, number %d for %f seconds", name, gs->avoidgoals[i],
					  gs->avoidgoaltimes[i] - AAS_Time());
		}
	}
}

void BotAddToAvoidGoals(bot_goalstate_t *gs, int number, float avoidtime) {
	int i;

	for (i = 0; i < MAX_AVOIDGOALS; i++) {
		// if the avoid goal is already stored
		if (gs->avoidgoals[i] == number) {
			gs->avoidgoals[i] = number;
			gs->avoidgoaltimes[i] = AAS_Time() + avoidtime;
			return;
		}
	}

	for (i = 0; i < MAX_AVOIDGOALS; i++) {
		// if this avoid goal has expired
		if (gs->avoidgoaltimes[i] < AAS_Time()) {
			gs->avoidgoals[i] = number;
			gs->avoidgoaltimes[i] = AAS_Time() + avoidtime;
			return;
		}
	}
}

void BotRemoveFromAvoidGoals(int goalstate, int number) {
	int i;
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return;
	// don't use the goals the bot wants to avoid
	for (i = 0; i < MAX_AVOIDGOALS; i++) {
		if (gs->avoidgoals[i] == number && gs->avoidgoaltimes[i] >= AAS_Time()) {
			gs->avoidgoaltimes[i] = 0;
			return;
		}
	}
}

float BotAvoidGoalTime(int goalstate, int number) {
	int i;
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return 0;
	// don't use the goals the bot wants to avoid
	for (i = 0; i < MAX_AVOIDGOALS; i++) {
		if (gs->avoidgoals[i] == number && gs->avoidgoaltimes[i] >= AAS_Time()) {
			return gs->avoidgoaltimes[i] - AAS_Time();
		}
	}
	return 0;
}

void BotSetAvoidGoalTime(int goalstate, int number, float avoidtime) {
	bot_goalstate_t *gs;
	levelitem_t *li;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return;
	if (avoidtime < 0) {
		if (!itemconfig)
			return;

		for (li = levelitems; li; li = li->next) {
			if (li->number == number) {
				avoidtime = itemconfig->iteminfo[li->iteminfo].respawntime;
				if (!avoidtime)
					avoidtime = AVOID_DEFAULT_TIME;
				if (avoidtime < AVOID_MINIMUM_TIME)
					avoidtime = AVOID_MINIMUM_TIME;
				BotAddToAvoidGoals(gs, number, avoidtime);
				return;
			}
		}
		return;
	} else {
		BotAddToAvoidGoals(gs, number, avoidtime);
	}
}

int BotGetLevelItemGoal(int index, char *name, bot_goal_t *goal) {
	levelitem_t *li;

	if (!itemconfig)
		return -1;
	li = levelitems;
	if (index >= 0) {
		for (; li; li = li->next) {
			if (li->number == index) {
				li = li->next;
				break;
			}
		}
	}
	for (; li; li = li->next) {

		if (g_gametype == GT_SINGLE_PLAYER) {
			if (li->flags & IFL_NOTSINGLE)
				continue;
		} else if (g_gametype >= GT_TEAM) {
			if (li->flags & IFL_NOTTEAM)
				continue;
		} else {
			if (li->flags & IFL_NOTFREE)
				continue;
		}
		if (li->flags & IFL_NOTBOT)
			continue;

		if (!Q_stricmp(name, itemconfig->iteminfo[li->iteminfo].name)) {
			goal->areanum = li->goalareanum;
			VectorCopy(li->goalorigin, goal->origin);
			goal->entitynum = li->entitynum;
			VectorCopy(itemconfig->iteminfo[li->iteminfo].mins, goal->mins);
			VectorCopy(itemconfig->iteminfo[li->iteminfo].maxs, goal->maxs);
			goal->number = li->number;
			goal->flags = GFL_ITEM;
			if (li->timeout)
				goal->flags |= GFL_DROPPED;
			goal->iteminfo = li->iteminfo;
			// botimport.Print(PRT_MESSAGE, "found li %s\n", itemconfig->iteminfo[li->iteminfo].name);
			return li->number;
		}
	}
	return -1;
}

int BotGetMapLocationGoal(char *name, bot_goal_t *goal) {
	maplocation_t *ml;
	vec3_t mins = {-8, -8, -8}, maxs = {8, 8, 8};

	for (ml = maplocations; ml; ml = ml->next) {
		if (!Q_stricmp(ml->name, name)) {
			goal->areanum = ml->areanum;
			VectorCopy(ml->origin, goal->origin);
			goal->entitynum = 0;
			VectorCopy(mins, goal->mins);
			VectorCopy(maxs, goal->maxs);
			goal->number = 0;
			goal->flags = 0;
			goal->iteminfo = 0;
			return qtrue;
		}
	}
	return qfalse;
}

int BotGetNextCampSpotGoal(int num, bot_goal_t *goal) {
	int i;
	campspot_t *cs;
	vec3_t mins = {-8, -8, -8}, maxs = {8, 8, 8};

	if (num < 0)
		num = 0;
	i = num;
	for (cs = campspots; cs; cs = cs->next) {
		if (--i < 0) {
			goal->areanum = cs->areanum;
			VectorCopy(cs->origin, goal->origin);
			goal->entitynum = 0;
			VectorCopy(mins, goal->mins);
			VectorCopy(maxs, goal->maxs);
			goal->number = 0;
			goal->flags = 0;
			goal->iteminfo = 0;
			return num + 1;
		}
	}
	return 0;
}

void BotFindEntityForLevelItem(levelitem_t *li) {
	int ent, modelindex;
	itemconfig_t *ic;
	aas_entityinfo_t entinfo;
	vec3_t dir;

	ic = itemconfig;
	if (!itemconfig)
		return;
	for (ent = AAS_NextEntity(0); ent; ent = AAS_NextEntity(ent)) {
		// get the model index of the entity
		modelindex = AAS_EntityModelindex(ent);

		if (!modelindex)
			continue;
		// get info about the entity
		AAS_EntityInfo(ent, &entinfo);
		// if the entity is still moving
		if (entinfo.origin[0] != entinfo.lastvisorigin[0] || entinfo.origin[1] != entinfo.lastvisorigin[1] ||
			entinfo.origin[2] != entinfo.lastvisorigin[2])
			continue;

		if (ic->iteminfo[li->iteminfo].modelindex == modelindex) {
			// check if the entity is very close
			VectorSubtract(li->origin, entinfo.origin, dir);
			if (VectorLength(dir) < 30) {
				// found an entity for this level item
				li->entitynum = ent;
			}
		}
	}
}

// NOTE: enum entityType_t in bg_public.h
#define ET_ITEM 2

void BotUpdateEntityItems(void) {
	int ent, i, modelindex;
	vec3_t dir;
	levelitem_t *li, *nextli;
	aas_entityinfo_t entinfo;
	itemconfig_t *ic;

	// timeout current entity items if necessary
	for (li = levelitems; li; li = nextli) {
		nextli = li->next;
		// if it is an item that will time out
		if (li->timeout) {
			// timeout the item
			if (li->timeout < AAS_Time()) {
				RemoveLevelItemFromList(li);
				FreeLevelItem(li);
			}
		}
	}
	// find new entity items
	ic = itemconfig;
	if (!itemconfig)
		return;

	for (ent = AAS_NextEntity(0); ent; ent = AAS_NextEntity(ent)) {
		if (AAS_EntityType(ent) != ET_ITEM)
			continue;
		// get the model index of the entity
		modelindex = AAS_EntityModelindex(ent);

		if (!modelindex)
			continue;
		// get info about the entity
		AAS_EntityInfo(ent, &entinfo);
		// FIXME: don't do this
		// skip all floating items for now
		// if (entinfo.groundent != ENTITYNUM_WORLD) continue;
		// if the entity is still moving
		if (entinfo.origin[0] != entinfo.lastvisorigin[0] || entinfo.origin[1] != entinfo.lastvisorigin[1] ||
			entinfo.origin[2] != entinfo.lastvisorigin[2])
			continue;
		// check if the entity is already stored as a level item
		for (li = levelitems; li; li = li->next) {
			// if the level item is linked to an entity
			if (li->entitynum && li->entitynum == ent) {
				// the entity is re-used if the models are different
				if (ic->iteminfo[li->iteminfo].modelindex != modelindex) {
					// remove this level item
					RemoveLevelItemFromList(li);
					FreeLevelItem(li);
					li = NULL;
					break;
				} else {
					if (entinfo.origin[0] != li->origin[0] || entinfo.origin[1] != li->origin[1] ||
						entinfo.origin[2] != li->origin[2]) {
						VectorCopy(entinfo.origin, li->origin);
						// also update the goal area number
						li->goalareanum = AAS_BestReachableArea(li->origin, ic->iteminfo[li->iteminfo].mins,
																ic->iteminfo[li->iteminfo].maxs, li->goalorigin);
					}
					break;
				}
			}
		}
		if (li)
			continue;
		// try to link the entity to a level item
		for (li = levelitems; li; li = li->next) {
			// if this level item is already linked
			if (li->entitynum)
				continue;

			if (g_gametype == GT_SINGLE_PLAYER) {
				if (li->flags & IFL_NOTSINGLE)
					continue;
			} else if (g_gametype >= GT_TEAM) {
				if (li->flags & IFL_NOTTEAM)
					continue;
			} else {
				if (li->flags & IFL_NOTFREE)
					continue;
			}
			// if the model of the level item and the entity are the same
			if (ic->iteminfo[li->iteminfo].modelindex == modelindex) {
				// check if the entity is very close
				VectorSubtract(li->origin, entinfo.origin, dir);
				if (VectorLength(dir) < 30) {
					// found an entity for this level item
					li->entitynum = ent;
					// if the origin is different
					if (entinfo.origin[0] != li->origin[0] || entinfo.origin[1] != li->origin[1] ||
						entinfo.origin[2] != li->origin[2]) {
						// update the level item origin
						VectorCopy(entinfo.origin, li->origin);
						// also update the goal area number
						li->goalareanum = AAS_BestReachableArea(li->origin, ic->iteminfo[li->iteminfo].mins,
																ic->iteminfo[li->iteminfo].maxs, li->goalorigin);
					}
#ifdef DEBUG
					Log_Write("linked item %s to an entity", ic->iteminfo[li->iteminfo].classname);
#endif // DEBUG
					break;
				}
			}
		}
		if (li)
			continue;
		// check if the model is from a known item
		for (i = 0; i < ic->numiteminfo; i++) {
			if (ic->iteminfo[i].modelindex == modelindex) {
				break;
			}
		}
		// if the model is not from a known item
		if (i >= ic->numiteminfo)
			continue;
		// allocate a new level item
		li = AllocLevelItem();

		if (!li)
			continue;
		// entity number of the level item
		li->entitynum = ent;
		// number for the level item
		li->number = numlevelitems + ent;
		// set the item info index for the level item
		li->iteminfo = i;
		// origin of the item
		VectorCopy(entinfo.origin, li->origin);
		// get the item goal area and goal origin
		li->goalareanum = AAS_BestReachableArea(li->origin, ic->iteminfo[i].mins, ic->iteminfo[i].maxs, li->goalorigin);
		// never go for items dropped into jumppads
		if (AAS_AreaJumpPad(li->goalareanum)) {
			FreeLevelItem(li);
			continue;
		}
		// time this item out after 30 seconds
		// dropped items disappear after 30 seconds
		li->timeout = AAS_Time() + 30;
		// add the level item to the list
		AddLevelItemToList(li);
		// botimport.Print(PRT_MESSAGE, "found new level item %s\n", ic->iteminfo[i].classname);
	}
	/*
	for (li = levelitems; li; li = li->next) {
		if (!li->entitynum) {
			BotFindEntityForLevelItem(li);
		}
	}
	*/
}

void BotDumpGoalStack(int goalstate) {
	int i;
	bot_goalstate_t *gs;
	char name[32];

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return;
	for (i = 1; i <= gs->goalstacktop; i++) {
		BotGoalName(gs->goalstack[i].number, name, 32);
		Log_Write("%d: %s", i, name);
	}
}

void BotPushGoal(int goalstate, bot_goal_t *goal) {
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return;
	if (gs->goalstacktop >= MAX_GOALSTACK - 1) {
		botimport.Print(PRT_ERROR, "goal heap overflow\n");
		BotDumpGoalStack(goalstate);
		return;
	}
	gs->goalstacktop++;
	Com_Memcpy(&gs->goalstack[gs->goalstacktop], goal, sizeof(bot_goal_t));
}

void BotPopGoal(int goalstate) {
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return;
	if (gs->goalstacktop > 0)
		gs->goalstacktop--;
}

void BotEmptyGoalStack(int goalstate) {
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return;
	gs->goalstacktop = 0;
}

int BotGetTopGoal(int goalstate, bot_goal_t *goal) {
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return qfalse;
	if (!gs->goalstacktop)
		return qfalse;
	Com_Memcpy(goal, &gs->goalstack[gs->goalstacktop], sizeof(bot_goal_t));
	return qtrue;
}

int BotGetSecondGoal(int goalstate, bot_goal_t *goal) {
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return qfalse;
	if (gs->goalstacktop <= 1)
		return qfalse;
	Com_Memcpy(goal, &gs->goalstack[gs->goalstacktop - 1], sizeof(bot_goal_t));
	return qtrue;
}

//===========================================================================
// pops a new long term goal on the goal stack in the goalstate
//===========================================================================
int BotChooseLTGItem(int goalstate, vec3_t origin, int *inventory, int travelflags) {
	int areanum, t, weightnum;
	float weight, bestweight, avoidtime;
	iteminfo_t *iteminfo;
	itemconfig_t *ic;
	levelitem_t *li, *bestitem;
	bot_goal_t goal;
	bot_goalstate_t *gs;

	// cyr
	int j = 0;
	char info[1024] = "0\\test\\";
	// char strtmp[1024];
	qboolean setcs = qfalse;
	float roamfactor = (float)LibVarGetValue("roamfactor") / 100;
	// vec3_t amins ={-8,-8,-8};
	// vec3_t amaxs ={8,8,8};

	// botimport.Print(PRT_MESSAGE, "roamfactor is %.1f \n", roamfactor);
	// AAS_ClearShownDebugLines();

	setcs = LibVarGetValue("showitemweights"); // cyr

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return qfalse;
	if (!gs->itemweightconfig)
		return qfalse;
	// get the area the bot is in
	areanum = BotReachabilityArea(origin, gs->client);
	// if the bot is in solid or if the area the bot is in has no reachability links
	if (!areanum || !AAS_AreaReachability(areanum)) {
		// use the last valid area the bot was in
		areanum = gs->lastreachabilityarea;
	}
	// remember the last area with reachabilities the bot was in
	gs->lastreachabilityarea = areanum;
	// if still in solid
	if (!areanum)
		return qfalse;
	// the item configuration
	ic = itemconfig;
	if (!itemconfig)
		return qfalse;
	// best weight and item so far
	bestweight = 0;
	bestitem = NULL;
	Com_Memset(&goal, 0, sizeof(bot_goal_t));
	// go through the items in the level
	for (li = levelitems; li; li = li->next) {
		if (g_gametype == GT_SINGLE_PLAYER) {
			if (li->flags & IFL_NOTSINGLE)
				continue;
		} else if (g_gametype >= GT_TEAM) {
			if (li->flags & IFL_NOTTEAM)
				continue;
		} else {
			if (li->flags & IFL_NOTFREE)
				continue;
		}
		if (li->flags & IFL_NOTBOT)
			continue;
		// if the item is not in a possible goal area
		if (!li->goalareanum)
			continue;
		// FIXME: is this a good thing? added this for items that never spawned into the game (f.i. CTF flags in
		// obelisk)
		if (!li->entitynum && !(li->flags & IFL_ROAM))
			continue;
		// get the fuzzy weight function for this item
		iteminfo = &ic->iteminfo[li->iteminfo];
		weightnum = gs->itemweightindex[iteminfo->number];
		if (weightnum < 0)
			continue;

#ifdef UNDECIDEDFUZZY
		weight = FuzzyWeightUndecided(inventory, gs->itemweightconfig, weightnum);
#else
		weight = FuzzyWeight(inventory, gs->itemweightconfig, weightnum);
#endif // UNDECIDEDFUZZY
#ifdef DROPPEDWEIGHT
		// HACK: to make dropped items more attractive
		if (li->timeout)
			weight += droppedweight->value;
#endif // DROPPEDWEIGHT

		// use weight scale for item_botroam
		if (li->flags & IFL_ROAM) {
			weight *= li->weight * roamfactor;
			// AAS_ShowBoundingBox(li->goalorigin, amins, amaxs);	// cyr
		}

		if (weight > 0) {
			// get the travel time towards the goal area
			t = AAS_AreaTravelTimeToGoalArea(areanum, origin, li->goalareanum, travelflags);
			// if the goal is reachable
			if (t > 0) {
				// if this item won't respawn before we get there
				avoidtime = BotAvoidGoalTime(goalstate, li->number);
				if (avoidtime - t * 0.009 > 0)
					continue;

				weight /= (float)t * TRAVELTIME_SCALE;

				if (weight > bestweight) {
					bestweight = weight;
					bestitem = li;
				}
			}
		}
		// cyr.. write to infostring
		if (setcs && weight > 10 && strlen(info) < 900)
			strcat(info, va("%d\\%s: %.1f\\", j++, iteminfo->name /*, fuzzweight*/, weight));
	}
	// if no goal item found
	if (!bestitem) {
		return qfalse;
	}
	if (setcs)
		botimport.SetBotInfoString(info); // cyr
	// botimport.Print(PRT_MESSAGE, "sending info string: %s\n\n", info);
	// create a bot goal for this item
	iteminfo = &ic->iteminfo[bestitem->iteminfo];
	VectorCopy(bestitem->goalorigin, goal.origin);
	VectorCopy(iteminfo->mins, goal.mins);
	VectorCopy(iteminfo->maxs, goal.maxs);
	goal.areanum = bestitem->goalareanum;
	goal.entitynum = bestitem->entitynum;
	goal.number = bestitem->number;
	goal.flags = GFL_ITEM;
	if (bestitem->timeout)
		goal.flags |= GFL_DROPPED;
	if (bestitem->flags & IFL_ROAM)
		goal.flags |= GFL_ROAM;
	goal.iteminfo = bestitem->iteminfo;
	// if it's a dropped item
	if (bestitem->timeout) {
		avoidtime = AVOID_DROPPED_TIME;
	} else if (bestitem->flags & IFL_ROAM && bestitem->respawntime) {
		avoidtime = bestitem->respawntime;
	} else {
		avoidtime = iteminfo->respawntime;
		if (!avoidtime)
			avoidtime = AVOID_DEFAULT_TIME;
		if (avoidtime < AVOID_MINIMUM_TIME)
			avoidtime = AVOID_MINIMUM_TIME;
	}
	// add the chosen goal to the goals to avoid for a while
	BotAddToAvoidGoals(gs, bestitem->number, avoidtime);
	// push the goal on the stack
	BotPushGoal(goalstate, &goal);

	return qtrue;
}

int BotChooseNBGItem(int goalstate, vec3_t origin, int *inventory, int travelflags, bot_goal_t *ltg, float maxtime) {
	int areanum, t, weightnum, ltg_time;
	float weight, bestweight, avoidtime;
	iteminfo_t *iteminfo;
	itemconfig_t *ic;
	levelitem_t *li, *bestitem;
	bot_goal_t goal;
	bot_goalstate_t *gs;
	float roamfactor = (float)LibVarGetValue("roamfactor") / 100;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return qfalse;
	if (!gs->itemweightconfig)
		return qfalse;
	// get the area the bot is in
	areanum = BotReachabilityArea(origin, gs->client);
	// if the bot is in solid or if the area the bot is in has no reachability links
	if (!areanum || !AAS_AreaReachability(areanum)) {
		// use the last valid area the bot was in
		areanum = gs->lastreachabilityarea;
	}
	// remember the last area with reachabilities the bot was in
	gs->lastreachabilityarea = areanum;
	// if still in solid
	if (!areanum)
		return qfalse;

	if (ltg)
		ltg_time = AAS_AreaTravelTimeToGoalArea(areanum, origin, ltg->areanum, travelflags);
	else
		ltg_time = 99999;
	// the item configuration
	ic = itemconfig;
	if (!itemconfig)
		return qfalse;
	// best weight and item so far
	bestweight = 0;
	bestitem = NULL;
	Com_Memset(&goal, 0, sizeof(bot_goal_t));
	// go through the items in the level
	for (li = levelitems; li; li = li->next) {
		if (g_gametype == GT_SINGLE_PLAYER) {
			if (li->flags & IFL_NOTSINGLE)
				continue;
		} else if (g_gametype >= GT_TEAM) {
			if (li->flags & IFL_NOTTEAM)
				continue;
		} else {
			if (li->flags & IFL_NOTFREE)
				continue;
		}
		if (li->flags & IFL_NOTBOT)
			continue;
		// if the item is in a possible goal area
		if (!li->goalareanum)
			continue;
		// FIXME: is this a good thing? added this for items that never spawned into the game (f.i. CTF flags in
		// obelisk)
		if (!li->entitynum && !(li->flags & IFL_ROAM))
			continue;
		// get the fuzzy weight function for this item
		iteminfo = &ic->iteminfo[li->iteminfo];
		weightnum = gs->itemweightindex[iteminfo->number];
		if (weightnum < 0)
			continue;

#ifdef UNDECIDEDFUZZY
		weight = FuzzyWeightUndecided(inventory, gs->itemweightconfig, weightnum);
#else
		weight = FuzzyWeight(inventory, gs->itemweightconfig, weightnum);
#endif // UNDECIDEDFUZZY
#ifdef DROPPEDWEIGHT
		// HACK: to make dropped items more attractive
		if (li->timeout)
			weight += droppedweight->value;
#endif // DROPPEDWEIGHT
	   // use weight scale for item_botroam
		if (li->flags & IFL_ROAM)
			weight = roamfactor * li->weight * weight; // cyr

		if (weight > 0) {
			// get the travel time towards the goal area
			t = AAS_AreaTravelTimeToGoalArea(areanum, origin, li->goalareanum, travelflags);
			// if the goal is reachable
			if (t > 0 && t < maxtime) {
				// if this item won't respawn before we get there
				avoidtime = BotAvoidGoalTime(goalstate, li->number);
				if (avoidtime - t * 0.009 > 0)
					continue;

				weight /= (float)t * TRAVELTIME_SCALE;

				if (weight > bestweight) {
					t = 0;
					if (ltg && !li->timeout) {
						// get the travel time from the goal to the long term goal
						t = AAS_AreaTravelTimeToGoalArea(li->goalareanum, li->goalorigin, ltg->areanum, travelflags);
					}
					// if the travel back is possible and doesn't take too long
					if (t <= ltg_time) {
						bestweight = weight;
						bestitem = li;
					}
				}
			}
		}
	}
	// if no goal item found
	if (!bestitem)
		return qfalse;
	// create a bot goal for this item
	iteminfo = &ic->iteminfo[bestitem->iteminfo];
	VectorCopy(bestitem->goalorigin, goal.origin);
	VectorCopy(iteminfo->mins, goal.mins);
	VectorCopy(iteminfo->maxs, goal.maxs);
	goal.areanum = bestitem->goalareanum;
	goal.entitynum = bestitem->entitynum;
	goal.number = bestitem->number;
	goal.flags = GFL_ITEM;
	if (bestitem->timeout)
		goal.flags |= GFL_DROPPED;
	if (bestitem->flags & IFL_ROAM)
		goal.flags |= GFL_ROAM;
	goal.iteminfo = bestitem->iteminfo;
	// if it's a dropped item
	if (bestitem->timeout) {
		avoidtime = AVOID_DROPPED_TIME;
	} else if (bestitem->flags & IFL_ROAM && bestitem->respawntime) {
		avoidtime = bestitem->respawntime;
	} else {
		avoidtime = iteminfo->respawntime;
		if (!avoidtime)
			avoidtime = AVOID_DEFAULT_TIME;
		if (avoidtime < AVOID_MINIMUM_TIME)
			avoidtime = AVOID_MINIMUM_TIME;
	}
	// add the chosen goal to the goals to avoid for a while
	BotAddToAvoidGoals(gs, bestitem->number, avoidtime);
	// push the goal on the stack
	BotPushGoal(goalstate, &goal);

	return qtrue;
}

int BotTouchingGoal(const vec3_t origin, const bot_goal_t *goal) {
	int i;
	vec3_t boxmins, boxmaxs;
	vec3_t absmins, absmaxs;
	const vec3_t safety_maxs = {0, 0, 0}; //{4, 4, 10};
	const vec3_t safety_mins = {0, 0, 0}; //{-4, -4, 0};

	AAS_PresenceTypeBoundingBox(PRESENCE_NORMAL, boxmins, boxmaxs);
	VectorSubtract(goal->mins, boxmaxs, absmins);
	VectorSubtract(goal->maxs, boxmins, absmaxs);
	VectorAdd(absmins, goal->origin, absmins);
	VectorAdd(absmaxs, goal->origin, absmaxs);
	// make the box a little smaller for safety
	VectorSubtract(absmaxs, safety_maxs, absmaxs);
	VectorSubtract(absmins, safety_mins, absmins);

	for (i = 0; i < 3; i++) {
		if (origin[i] < absmins[i] || origin[i] > absmaxs[i])
			return qfalse;
	}
	return qtrue;
}

int BotItemGoalInVisButNotVisible(int viewer, vec3_t eye, vec3_t viewangles, bot_goal_t *goal) {
	aas_entityinfo_t entinfo;
	bsp_trace_t trace;
	vec3_t middle;

	if (!(goal->flags & GFL_ITEM))
		return qfalse;

	VectorAdd(goal->mins, goal->mins, middle);
	VectorScale(middle, 0.5, middle);
	VectorAdd(goal->origin, middle, middle);

	trace = AAS_Trace(eye, NULL, NULL, middle, viewer, CONTENTS_SOLID);
	// if the goal middle point is visible
	if (trace.fraction >= 1) {
		// the goal entity number doesn't have to be valid
		// just assume it's valid
		if (goal->entitynum <= 0)
			return qfalse;

		// if the entity data isn't valid
		AAS_EntityInfo(goal->entitynum, &entinfo);
		// NOTE: for some wacko reason entities are sometimes
		// not updated
		// if (!entinfo.valid) return qtrue;
		if (entinfo.ltime < AAS_Time() - 0.5)
			return qtrue;
	}
	return qfalse;
}

void BotResetGoalState(int goalstate) {
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return;
	Com_Memset(gs->goalstack, 0, MAX_GOALSTACK * sizeof(bot_goal_t));
	gs->goalstacktop = 0;
	BotResetAvoidGoals(goalstate);
}

int BotLoadItemWeights(int goalstate, const char *filename) {
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return BLERR_CANNOTLOADITEMWEIGHTS;
	// load the weight configuration
	gs->itemweightconfig = ReadWeightConfig(filename);
	if (!gs->itemweightconfig) {
		botimport.Print(PRT_FATAL, "couldn't load weights\n");
		return BLERR_CANNOTLOADITEMWEIGHTS;
	}
	// if there's no item configuration
	if (!itemconfig)
		return BLERR_CANNOTLOADITEMWEIGHTS;
	// create the item weight index
	gs->itemweightindex = ItemWeightIndex(gs->itemweightconfig, itemconfig);
	// everything went ok
	return BLERR_NOERROR;
}

void BotFreeItemWeights(int goalstate) {
	bot_goalstate_t *gs;

	gs = BotGoalStateFromHandle(goalstate);
	if (!gs)
		return;
	if (gs->itemweightconfig)
		FreeWeightConfig(gs->itemweightconfig);
	if (gs->itemweightindex)
		FreeMemory(gs->itemweightindex);
}

int BotAllocGoalState(int client) {
	int i;

	for (i = 1; i <= MAX_CLIENTS; i++) {
		if (!botgoalstates[i]) {
			botgoalstates[i] = GetClearedMemory(sizeof(bot_goalstate_t));
			botgoalstates[i]->client = client;
			return i;
		}
	}
	return 0;
}

void BotFreeGoalState(int handle) {
	if (handle <= 0 || handle > MAX_CLIENTS) {
		botimport.Print(PRT_FATAL, "goal state handle %d out of range\n", handle);
		return;
	}
	if (!botgoalstates[handle]) {
		botimport.Print(PRT_FATAL, "invalid goal state handle %d\n", handle);
		return;
	}
	BotFreeItemWeights(handle);
	FreeMemory(botgoalstates[handle]);
	botgoalstates[handle] = NULL;
}

int BotSetupGoalAI(void) {
	const char *filename;

	// check if teamplay is on
	g_gametype = LibVarValue("g_gametype", "0");
	// item configuration file
	filename = LibVarString("itemconfig", "items.c");
	// load the item configuration
	itemconfig = LoadItemConfig(filename);
	if (!itemconfig) {
		botimport.Print(PRT_FATAL, "couldn't load item config\n");
		return BLERR_CANNOTLOADITEMCONFIG;
	}

	droppedweight = LibVar("droppedweight", "1000");
	// everything went ok
	return BLERR_NOERROR;
}

void BotShutdownGoalAI(void) {
	int i;

	if (itemconfig)
		FreeMemory(itemconfig);
	itemconfig = NULL;
	if (levelitemheap)
		FreeMemory(levelitemheap);
	levelitemheap = NULL;
	freelevelitems = NULL;
	levelitems = NULL;
	numlevelitems = 0;

	BotFreeInfoEntities();

	for (i = 1; i <= MAX_CLIENTS; i++) {
		if (botgoalstates[i]) {
			BotFreeGoalState(i);
		}
	}
}
