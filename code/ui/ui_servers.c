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
/*
=======================================================================

JOIN MENU (SERVER BROWSER)

=======================================================================
*/

#include "ui_local.h"

#define MAX_GLOBALSERVERS 128
#define MAX_PINGREQUESTS 32
#define MAX_ADDRESSLENGTH 64
#define MAX_HOSTNAMELENGTH 31
#define MAX_MAPNAMELENGTH 32
#define MAX_LISTBOXITEMS MAX_GLOBALSERVERS
#define MAX_LOCALSERVERS MAX_GLOBALSERVERS
#define MAX_STATUSLENGTH 64
#define MAX_LEAGUELENGTH 28
#define MAX_LISTBOXWIDTH 80

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define SPECIFY0 "menu/buttons/specify0"
#define SPECIFY1 "menu/buttons/specify1"
#define REFRESH0 "menu/buttons/refresh0"
#define REFRESH1 "menu/buttons/refresh1"
#define DELETE0 "menu/buttons/delete0"
#define DELETE1 "menu/buttons/delete1"
#define FIGHT0 "menu/buttons/fight0"
#define FIGHT1 "menu/buttons/fight1"
#define ARROWUP0 "menu/arrows/headyel_up0"
#define ARROWUP1 "menu/arrows/headyel_up1"
#define ARROWDN0 "menu/arrows/headyel_dn0"
#define ARROWDN1 "menu/arrows/headyel_dn1"
#define UNKNOWNMAP "menu/art/unknownmap"

#define ID_MASTER 10
#define ID_GAMETYPE 11
#define ID_SORTKEY 12
#define ID_SHOW_FULL 13
#define ID_SHOW_EMPTY 14

#define ID_LIST 15
#define ID_SCROLL_UP 16
#define ID_SCROLL_DOWN 17
#define ID_BACK 18
#define ID_REFRESH 19
#define ID_SPECIFY 20
#define ID_CREATE 21
#define ID_CONNECT 22
#define ID_REMOVE 23

#define ID_ONLY_HUMANS 24
#define ID_HIDE_PRIVATE 25

#define ID_ADDTOFAV 26
#define ID_ADDALLTOFAV 27

// FIXME: Remove multiple master servers?
#define UIAS_LOCAL 0
#define UIAS_FAVORITES 1
#define UIAS_GLOBAL_START 2
#define UIAS_GLOBAL_MAX MAX_MASTER_SERVERS + UIAS_GLOBAL_START - 1

#define GAMES_ALL 0
#define GAMES_FFA 1
#define GAMES_SYC 2
#define GAMES_LPS 3
#define GAMES_TDM 4
#define GAMES_CTL 5
#define GAMES_1LCTL 6
#define GAMES_TSYC 7
#define GAMES_BB 8
#define GAMES_FT 9

#define GLOBAL_ITEM_NAME "Internet"

static char global_names[MAX_MASTER_SERVERS][sizeof(GLOBAL_ITEM_NAME) + sizeof("1234567890") - 1] = {""};

static const char *master_items[UIAS_GLOBAL_MAX + 2] = {"Local", "Favorites"};

static const char *servertype_items[] = {"All",
										 GAMETYPE_NAME(GT_FFA),
										 GAMETYPE_NAME(GT_SPRAYFFA),
										 GAMETYPE_NAME(GT_LPS),
										 GAMETYPE_NAME(GT_TEAM),
										 GAMETYPE_NAME(GT_FREEZETAG),
										 GAMETYPE_NAME(GT_CTF),
										 GAMETYPE_NAME(GT_1FCTF),
										 GAMETYPE_NAME(GT_SPRAY),
										 GAMETYPE_NAME(GT_BALLOON),
										 NULL};

static const char *sortkey_items[] = {"Server Name",   "Map Name", "Open Player Slots", "Game Type", "Ping Time",
									  "Human Players", NULL};

static char *gamenames[] = {GAMETYPE_NAME_SHORT(GT_FFA),
							GAMETYPE_NAME_SHORT(GT_TOURNAMENT),
							GAMETYPE_NAME_SHORT(GT_SINGLE_PLAYER),
							GAMETYPE_NAME_SHORT(GT_SPRAYFFA),
							GAMETYPE_NAME_SHORT(GT_LPS),
							GAMETYPE_NAME_SHORT(GT_CATCH),
							GAMETYPE_NAME_SHORT(GT_TEAM),
							GAMETYPE_NAME_SHORT(GT_FREEZETAG),
							GAMETYPE_NAME_SHORT(GT_CTF),
							GAMETYPE_NAME_SHORT(GT_1FCTF),
							GAMETYPE_NAME_SHORT(GT_SPRAY),
							GAMETYPE_NAME_SHORT(GT_BALLOON),
							GAMETYPE_NAME_SHORT(GT_MAX_GAME_TYPE),
							NULL};

static char quake3worldMessage[] = "Visit www.worldofpadman.net - News, Community, Events, Files";

typedef struct {
	char adrstr[MAX_ADDRESSLENGTH];
	int start;
} pinglist_t;

typedef struct servernode_s {
	char adrstr[MAX_ADDRESSLENGTH];
	char hostname[MAX_HOSTNAMELENGTH + 3];
	char mapname[MAX_MAPNAMELENGTH];
	int numclients;
	int humanclients;
	qboolean needPass;
	int maxclients;
	int pingtime;
	int gametype;
	char gamename[16];
	qboolean mod;
	int minPing;
	int maxPing;
} servernode_t;

typedef struct {
	char buff[MAX_LISTBOXWIDTH + (6 * (ARRAY_LEN(S_COLOR_BLACK) - 1))]; // extra space for color codes
	servernode_t *servernode;
} table_t;

typedef struct {
	menuframework_s menu;

	menulist_s master;
	menulist_s gametype;
	menulist_s sortkey;
	menuradiobutton_s showfull;
	menuradiobutton_s showempty;

	menuradiobutton_s onlyhumans;
	menuradiobutton_s hideprivate;

	menulist_s list;
	menubitmap_s mappic;
	menubitmap_s arrowup;
	menubitmap_s arrowdown;
	menutext_s status;
	menutext_s statusbar;

	menutext_s addFav;
	menutext_s addAllFav;

	menubitmap_s back;
	menubitmap_s refresh;
	menubitmap_s specify;
	menubitmap_s remove;
	menubitmap_s fight;

	pinglist_t pinglist[MAX_PINGREQUESTS];
	table_t table[MAX_LISTBOXITEMS];
	char *items[MAX_LISTBOXITEMS];
	int numqueriedservers;
	int *numservers;
	servernode_t *serverlist;
	int currentping;
	qboolean refreshservers;
	int nextpingtime;
	int maxservers;
	int refreshtime;
	char favoriteaddresses[MAX_FAVORITESERVERS][MAX_ADDRESSLENGTH];
	int numfavoriteaddresses;
} arenaservers_t;

static arenaservers_t g_arenaservers;
static servernode_t g_globalserverlist[MAX_GLOBALSERVERS];
static int g_numglobalservers;
static servernode_t g_localserverlist[MAX_LOCALSERVERS];
static int g_numlocalservers;
static servernode_t g_favoriteserverlist[MAX_FAVORITESERVERS];
static int g_numfavoriteservers;
static int g_servertype;
static int g_gametype;
static int g_sortkey;
static int g_emptyservers;
static int g_fullservers;
static int g_onlyhumans;
static int g_hideprivate;

/*
=================
UI_ArenaServers_ServerCleanName
Removes all color sequences, removes special chars, consecutive spaces etc.
Makes sure there is a non-empty output string. See ClientCleanName().
=================
*/

static void UI_ArenaServers_ServerCleanName(const char *in, char *out, int outSize) {
	int outpos = 0, colorlessLen = 0, spaces = 0, totalWhitespace = 0;

	// discard leading spaces
	for (; *in == ' '; in++) {
		// NOP
	}

	for (; (*in && outpos < (outSize - 1)); in++) {
		out[outpos] = *in;

		if (*in == ' ') {
			// don't allow too many consecutive spaces
			if (spaces > 2) {
				continue;
			}

			spaces++;
		} else if ((outpos > 0) && (out[outpos - 1] == Q_COLOR_ESCAPE)) {
			if (Q_IsColorString(&out[outpos - 1])) {
				colorlessLen--;
				outpos--;
				continue;
			} else {
				spaces = 0;
				colorlessLen++;
			}
		} else {
			spaces = 0;
			colorlessLen++;
		}

		if ((*in < ' ') || (*in > '}') || (*in == '`')) {
			colorlessLen--;
			continue;
		}

		if (*in == ' ') {
			totalWhitespace++;
		}

		outpos++;
	}

	out[outpos] = '\0';

	// don't allow empty names
	if ((*out == '\0') || (colorlessLen == 0) || (totalWhitespace >= colorlessLen)) {
		Q_strncpyz(out, "noname", outSize);
	}
}

/*
=================
UI_ArenaServers_MaxPing
=================
*/
static int UI_ArenaServers_MaxPing(void) {
	int maxPing;

	maxPing = (int)trap_Cvar_VariableValue("cl_maxPing");
	if (maxPing < 100) {
		maxPing = 100;
	}
	return maxPing;
}

/*
=================
UI_ArenaServers_Compare
=================
*/
static int QDECL UI_ArenaServers_Compare(const void *arg1, const void *arg2) {
	float f1;
	float f2;
	const servernode_t *t1;
	const servernode_t *t2;

	t1 = (const servernode_t *)arg1;
	t2 = (const servernode_t *)arg2;

	switch (g_sortkey) {
	case SORT_HOST:
		return Q_stricmp(t1->hostname, t2->hostname);

	case SORT_MAP:
		return Q_stricmp(t1->mapname, t2->mapname);

	case SORT_CLIENTS:
		f1 = t1->maxclients - t1->numclients;
		if (f1 < 0) {
			f1 = 0;
		}

		f2 = t2->maxclients - t2->numclients;
		if (f2 < 0) {
			f2 = 0;
		}

		if (f1 < f2) {
			return 1;
		}
		if (f1 == f2) {
			return 0;
		}
		return -1;

	case SORT_HUMANS:
		f1 = t1->humanclients;
		f2 = t2->humanclients;

		if (f1 < f2) {
			return 1;
		}
		if (f1 == f2) {
			return 0;
		}
		return -1;

	case SORT_GAME:
		if (t1->gametype < t2->gametype) {
			return -1;
		}
		if (t1->gametype == t2->gametype) {
			return 0;
		}
		return 1;

	case SORT_PING:
		if (t1->pingtime < t2->pingtime) {
			return -1;
		}
		if (t1->pingtime > t2->pingtime) {
			return 1;
		}
		return Q_stricmp(t1->hostname, t2->hostname);
	}

	return 0;
}

/*
=================
UI_ArenaServers_Fight
=================
*/
static void UI_ArenaServers_Fight(void) {
	servernode_t *servernode;

	servernode = g_arenaservers.table[g_arenaservers.list.curvalue].servernode;
	if (servernode) {
		if (servernode->needPass) {
			UI_SpecifyPasswordMenu(va("connect %s\n", servernode->adrstr), servernode->hostname);
		} else {
			trap_Cmd_ExecuteText(EXEC_APPEND, va("connect %s\n", servernode->adrstr));
		}
	}
}

/*
=================
UI_ArenaServers_UpdatePicture
=================
*/
static void UI_ArenaServers_UpdatePicture(void) {
	static char picname[64];
	servernode_t *servernodeptr;

	if (!g_arenaservers.list.numitems) {
		g_arenaservers.mappic.generic.name = NULL;
	} else {
		servernodeptr = g_arenaservers.table[g_arenaservers.list.curvalue].servernode;
		Com_sprintf(picname, sizeof(picname), "levelshots/%sB", servernodeptr->mapname);
		g_arenaservers.mappic.generic.name = picname;
	}

	// force shader update during draw
	g_arenaservers.mappic.shader = 0;
}

/*
=================
UI_ArenaServers_UpdateMenu
=================
*/
static void UI_ArenaServers_UpdateMenu(void) {
	int i;
	int j;
	int count;
	servernode_t *servernodeptr;
	table_t *tableptr;
	char *pingColor, *slotsColor, *mapColor, *modColor;

	if (g_arenaservers.numqueriedservers > 0) {
		// servers found
		if (g_arenaservers.refreshservers && (g_arenaservers.currentping <= g_arenaservers.numqueriedservers)) {
			// show progress
			Com_sprintf(g_arenaservers.status.string, MAX_STATUSLENGTH, "%d of %d Servers.", g_arenaservers.currentping,
						g_arenaservers.numqueriedservers);
			g_arenaservers.statusbar.string = "Press SPACE to stop";
			qsort(g_arenaservers.serverlist, *g_arenaservers.numservers, sizeof(servernode_t), UI_ArenaServers_Compare);
		} else {
			// all servers pinged - enable controls
			g_arenaservers.master.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.gametype.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.sortkey.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.showempty.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.onlyhumans.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.hideprivate.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.showfull.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.list.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.refresh.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.fight.generic.flags &= ~QMF_GRAYED;

			// update status bar
			if (g_servertype >= UIAS_GLOBAL_START && g_servertype <= UIAS_GLOBAL_MAX)
				g_arenaservers.statusbar.string = quake3worldMessage;
			else
				g_arenaservers.statusbar.string = "";

			Q_strncpyz(g_arenaservers.status.string, "Hit REFRESH to update", MAX_STATUSLENGTH);
		}
	} else {
		// no servers found
		if (g_arenaservers.refreshservers) {
			Q_strncpyz(g_arenaservers.status.string, "Scanning for servers.", MAX_STATUSLENGTH);
			g_arenaservers.statusbar.string = "Press SPACE to stop";

			// disable controls during refresh
			g_arenaservers.master.generic.flags |= QMF_GRAYED;
			g_arenaservers.gametype.generic.flags |= QMF_GRAYED;
			g_arenaservers.sortkey.generic.flags |= QMF_GRAYED;
			g_arenaservers.showempty.generic.flags |= QMF_GRAYED;
			g_arenaservers.onlyhumans.generic.flags |= QMF_GRAYED;
			g_arenaservers.hideprivate.generic.flags |= QMF_GRAYED;
			g_arenaservers.showfull.generic.flags |= QMF_GRAYED;
			g_arenaservers.list.generic.flags |= QMF_GRAYED;
			g_arenaservers.refresh.generic.flags |= QMF_GRAYED;
			g_arenaservers.fight.generic.flags |= QMF_GRAYED;
		} else {
			if (g_arenaservers.numqueriedservers < 0) {
				Q_strncpyz(g_arenaservers.status.string, "No response from master.", MAX_STATUSLENGTH);
			} else {
				Q_strncpyz(g_arenaservers.status.string, "No servers found.", MAX_STATUSLENGTH);
			}

			// update status bar
			if (g_servertype >= UIAS_GLOBAL_START && g_servertype <= UIAS_GLOBAL_MAX)
				g_arenaservers.statusbar.string = quake3worldMessage;
			else
				g_arenaservers.statusbar.string = "";

			// end of refresh - set control state
			g_arenaservers.master.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.gametype.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.sortkey.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.showempty.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.onlyhumans.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.hideprivate.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.showfull.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.list.generic.flags |= QMF_GRAYED;
			g_arenaservers.refresh.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.fight.generic.flags |= QMF_GRAYED;
		}

		// zero out list box
		g_arenaservers.list.numitems = 0;
		g_arenaservers.list.curvalue = 0;
		g_arenaservers.list.top = 0;

		// update picture
		UI_ArenaServers_UpdatePicture();
		return;
	}

	// build list box strings - apply culling filters
	servernodeptr = g_arenaservers.serverlist;
	count = *g_arenaservers.numservers;
	for (i = 0, j = 0; i < count; i++, servernodeptr++) {
		tableptr = &g_arenaservers.table[j];
		tableptr->servernode = servernodeptr;

		// can only cull valid results
		if (!g_emptyservers && !servernodeptr->numclients) {
			continue;
		}

		// If "Show only humans" and "Hide empty server" are enabled hide servers that only have bots
		if (!g_emptyservers && g_onlyhumans && !servernodeptr->humanclients) {
			continue;
		}

		if (!g_fullservers && (servernodeptr->numclients == servernodeptr->maxclients)) {
			continue;
		}

		switch (g_gametype) {
		case GAMES_ALL:
			break;

		case GAMES_FFA:
			if (servernodeptr->gametype != GT_FFA) {
				continue;
			}
			break;

		case GAMES_SYC:
			if (servernodeptr->gametype != GT_SPRAYFFA) {
				continue;
			}
			break;

		case GAMES_LPS:
			if (servernodeptr->gametype != GT_LPS) {
				continue;
			}
			break;

		case GAMES_TDM:
			if (servernodeptr->gametype != GT_TEAM) {
				continue;
			}
			break;

		case GAMES_CTL:
			if (servernodeptr->gametype != GT_CTF) {
				continue;
			}
			break;

		case GAMES_1LCTL:
			if (servernodeptr->gametype != GT_1FCTF) {
				continue;
			}
			break;

		case GAMES_TSYC:
			if (servernodeptr->gametype != GT_SPRAY) {
				continue;
			}
			break;

		case GAMES_BB:
			if (servernodeptr->gametype != GT_BALLOON) {
				continue;
			}
			break;

		case GAMES_FT:
			if (servernodeptr->gametype != GT_FREEZETAG) {
				continue;
			}
			break;
		}

		if (g_hideprivate && servernodeptr->needPass) {
			continue;
		}

		if (servernodeptr->pingtime < servernodeptr->minPing) {
			pingColor = S_COLOR_BLUE;
		} else if (servernodeptr->maxPing && servernodeptr->pingtime > servernodeptr->maxPing) {
			pingColor = S_COLOR_BLUE;
		} else if (servernodeptr->pingtime < 100) {
			pingColor = S_COLOR_GREEN;
		} else if (servernodeptr->pingtime < 200) {
			pingColor = S_COLOR_YELLOW;
		} else {
			pingColor = S_COLOR_RED;
		}

#define DEFAULT_COLOR_S S_COLOR_YELLOW
		if (servernodeptr->numclients >= servernodeptr->maxclients) {
			slotsColor = S_COLOR_RED;
		} else {
			slotsColor = DEFAULT_COLOR_S;
		}

		// this only partial exact, works at least for current mod
		if (-1 == trap_FS_FOpenFile(va("maps/%s.bsp", servernodeptr->mapname), NULL, FS_READ)) {
			mapColor = S_COLOR_RED;
		} else {
			mapColor = DEFAULT_COLOR_S;
		}

		if (servernodeptr->mod) {
			modColor = S_COLOR_CYAN;
		} else {
			modColor = DEFAULT_COLOR_S;
		}

		// TODO: Color if password'ed? Different colors depending on free slots?

		// NOTE: netname has been removed, since it's not really useful to the average player
		//       Also note that highlighting is partially broken due to static colors

		Com_sprintf(tableptr->buff, ARRAY_LEN(tableptr->buff),
					"%-22.22s%s %-18.18s" DEFAULT_COLOR_S " %s%2d+%2d/%2d" DEFAULT_COLOR_S " %s%-8.8s %s%-3d",
					servernodeptr->hostname, mapColor, servernodeptr->mapname, slotsColor, servernodeptr->humanclients,
					(servernodeptr->numclients - servernodeptr->humanclients), servernodeptr->maxclients, modColor,
					servernodeptr->gamename, pingColor, servernodeptr->pingtime);

		j++;
	}

	g_arenaservers.list.numitems = j;
	g_arenaservers.list.curvalue = 0;
	g_arenaservers.list.top = 0;

	// update picture
	UI_ArenaServers_UpdatePicture();
}

/*
=================
UI_ArenaServers_Remove
=================
*/
static void UI_ArenaServers_Remove(void) {
	int i;
	servernode_t *servernodeptr;
	table_t *tableptr;

	if (!g_arenaservers.list.numitems)
		return;

	// remove selected item from display list
	// items are in scattered order due to sort and cull
	// perform delete on list box contents, resync all lists

	tableptr = &g_arenaservers.table[g_arenaservers.list.curvalue];
	servernodeptr = tableptr->servernode;

	// find address in master list
	for (i = 0; i < g_arenaservers.numfavoriteaddresses; i++) {
		if (Q_stricmp(g_arenaservers.favoriteaddresses[i], servernodeptr->adrstr) == 0) {
			// delete address from master list
			if (i < (g_arenaservers.numfavoriteaddresses - 1)) {
				// shift items up
				memcpy(&g_arenaservers.favoriteaddresses[i], &g_arenaservers.favoriteaddresses[i + 1],
					   ((g_arenaservers.numfavoriteaddresses - i - 1) * MAX_ADDRESSLENGTH));
			}
			g_arenaservers.numfavoriteaddresses--;
			memset(&g_arenaservers.favoriteaddresses[g_arenaservers.numfavoriteaddresses], 0, MAX_ADDRESSLENGTH);
			break;
		}
	}

	// find address in server list
	for (i = 0; i < g_numfavoriteservers; i++) {
		if (&g_favoriteserverlist[i] == servernodeptr) {

			// delete address from server list
			if (i < (g_numfavoriteservers - 1)) {
				// shift items up
				memcpy(&g_favoriteserverlist[i], &g_favoriteserverlist[i + 1],
					   ((g_numfavoriteservers - i - 1) * sizeof(servernode_t)));
			}
			g_numfavoriteservers--;
			memset(&g_favoriteserverlist[g_numfavoriteservers], 0, sizeof(servernode_t));
			break;
		}
	}

	g_arenaservers.numqueriedservers = g_arenaservers.numfavoriteaddresses;
	g_arenaservers.currentping = g_arenaservers.numfavoriteaddresses;
}

/*
=================
UI_ArenaServers_Insert
=================
*/
static void UI_ArenaServers_Insert(char *adrstr, char *info, int pingtime) {
	servernode_t *servernodeptr;
	const char *s;
	int i;

	if ((pingtime >= UI_ArenaServers_MaxPing()) && (g_servertype != UIAS_FAVORITES)) {
		// slow global or local servers do not get entered
		return;
	}

	if (*g_arenaservers.numservers >= g_arenaservers.maxservers) {
		// list full;
		servernodeptr = (g_arenaservers.serverlist + (*g_arenaservers.numservers) - 1);
	} else {
		// next slot
		servernodeptr = (g_arenaservers.serverlist + (*g_arenaservers.numservers));
		(*g_arenaservers.numservers)++;
	}

	Q_strncpyz(servernodeptr->adrstr, adrstr, MAX_ADDRESSLENGTH);

	UI_ArenaServers_ServerCleanName(Info_ValueForKey(info, "hostname"), servernodeptr->hostname, sizeof(servernodeptr->hostname));

	Q_strncpyz(servernodeptr->mapname, Info_ValueForKey(info, "mapname"), MAX_MAPNAMELENGTH);
	Q_CleanStr(servernodeptr->mapname);
	// Q_strupr( servernodeptr->mapname );

	servernodeptr->numclients = atoi(Info_ValueForKey(info, "clients"));
	servernodeptr->humanclients = atoi(Info_ValueForKey(info, "g_humanplayers"));
	servernodeptr->needPass = atoi(Info_ValueForKey(info, "g_needpass"));
	servernodeptr->maxclients = atoi(Info_ValueForKey(info, "sv_maxclients"));
	servernodeptr->pingtime = pingtime;
	servernodeptr->minPing = atoi(Info_ValueForKey(info, "minPing"));
	servernodeptr->maxPing = atoi(Info_ValueForKey(info, "maxPing"));

	s = Info_ValueForKey(info, "game");
	i = atoi(Info_ValueForKey(info, "gametype"));
	if (i < 0) {
		i = 0;
	} else if (i > (GT_MAX_GAME_TYPE - 1)) {
		i = GT_MAX_GAME_TYPE;
	}
	// if its a mod, show modname, otherwise show gametype name
	if (*s && (Q_stricmp(s, BASEGAME) != 0)) {
		servernodeptr->gametype = i;
		Q_strncpyz(servernodeptr->gamename, s, sizeof(servernodeptr->gamename));
		servernodeptr->mod = qtrue;
	} else {
		servernodeptr->gametype = i;
		Q_strncpyz(servernodeptr->gamename, gamenames[i], sizeof(servernodeptr->gamename));
		servernodeptr->mod = qfalse;
	}
}

/*
=================
UI_ArenaServers_InsertFavorites

Insert nonresponsive address book entries into display lists.
=================
*/
static void UI_ArenaServers_InsertFavorites(void) {
	int i;
	int j;
	char info[MAX_INFO_STRING];

	// resync existing results with new or deleted cvars
	info[0] = '\0';
	Info_SetValueForKey(info, "hostname", "No Response");
	for (i = 0; i < g_arenaservers.numfavoriteaddresses; i++) {
		// find favorite address in refresh list
		for (j = 0; j < g_numfavoriteservers; j++) {
			if (Q_stricmp(g_arenaservers.favoriteaddresses[i], g_favoriteserverlist[j].adrstr) == 0) {
				break;
			}
		}

		if (j >= g_numfavoriteservers) {
			// not in list, add it
			UI_ArenaServers_Insert(g_arenaservers.favoriteaddresses[i], info, UI_ArenaServers_MaxPing());
		}
	}
}

/*
=================
UI_ArenaServers_LoadFavorites

Load cvar address book entries into local lists.
=================
*/
static void UI_ArenaServers_LoadFavorites(void) {
	int i;
	int j;
	int numtempitems;
	char adrstr[MAX_ADDRESSLENGTH];
	servernode_t templist[MAX_FAVORITESERVERS];
	qboolean found;

	found = qfalse;

	// copy the old
	memcpy(templist, g_favoriteserverlist, (sizeof(servernode_t) * MAX_FAVORITESERVERS));
	numtempitems = g_numfavoriteservers;

	// clear the current for sync
	memset(g_favoriteserverlist, 0, sizeof(servernode_t) * MAX_FAVORITESERVERS);
	g_numfavoriteservers = 0;

	// resync existing results with new or deleted cvars
	for (i = 0; i < MAX_FAVORITESERVERS; i++) {
		trap_Cvar_VariableStringBuffer(va("server%d", i + 1), adrstr, MAX_ADDRESSLENGTH);
		if (!adrstr[0])
			continue;

		// quick sanity check to avoid slow domain name resolving
		// first character must be numeric
		if (adrstr[0] != '[' && (adrstr[0] < '0' || adrstr[0] > '9'))
			continue;

		// favorite server addresses must be maintained outside refresh list
		// this mimics local and global netadr's stored in client
		// these can be fetched to fill ping list
		Q_strncpyz(g_arenaservers.favoriteaddresses[g_numfavoriteservers], adrstr,
				   sizeof(g_arenaservers.favoriteaddresses[g_numfavoriteservers]));

		// find this server in the old list
		for (j = 0; j < numtempitems; j++)
			if (!Q_stricmp(templist[j].adrstr, adrstr))
				break;

		if (j < numtempitems) {
			// found server - add exisiting results
			memcpy(&g_favoriteserverlist[g_numfavoriteservers], &templist[j], sizeof(servernode_t));
			found = qtrue;
		} else {
			// add new server
			Q_strncpyz(g_favoriteserverlist[g_numfavoriteservers].adrstr, adrstr, MAX_ADDRESSLENGTH);
			g_favoriteserverlist[g_numfavoriteservers].pingtime = UI_ArenaServers_MaxPing();
		}

		g_numfavoriteservers++;
	}

	g_arenaservers.numfavoriteaddresses = g_numfavoriteservers;

	if (!found) {
		// no results were found, reset server list
		// list will be automatically refreshed when selected
		g_numfavoriteservers = 0;
	}
}

/*
=================
UI_ArenaServers_AddToFavorites

Add one server ip to favorites list
=================
*/
static void UI_ArenaServers_AddToFavorites(const char *adrstr) {
	char buff[128];
	int i;
	int best;

	if (!adrstr[0]) {
		return;
	}

	best = 0;
	for (i = 0; i < MAX_FAVORITESERVERS; i++) {
		trap_Cvar_VariableStringBuffer(va("server%d", (i + 1)), buff, sizeof(buff));
		if (Q_stricmp(adrstr, buff) == 0) {
			// already in list
			return;
		}

		// empty slot
		if (buff[0] == '\0') {
			best = (i + 1);
			break;
		}

		// use first empty or non-numeric available slot
		if (adrstr[0] != '[' && (buff[0] < '0' || buff[0] > '9') && !best)
			best = (i + 1);
	}

	if (best) {
		trap_Cvar_Set(va("server%d", best), adrstr);
		UI_ArenaServers_LoadFavorites();
	}
}

/*
=================
UI_ArenaServers_StopRefresh
=================
*/
static void UI_ArenaServers_StopRefresh(void) {
	if (!g_arenaservers.refreshservers) {
		// not currently refreshing
		return;
	}

	g_arenaservers.refreshservers = qfalse;

	if (g_servertype == UIAS_FAVORITES) {
		// nonresponsive favorites must be shown
		UI_ArenaServers_InsertFavorites();
	}

	// final tally
	if (g_arenaservers.numqueriedservers >= 0) {
		g_arenaservers.currentping = *g_arenaservers.numservers;
		g_arenaservers.numqueriedservers = *g_arenaservers.numservers;
	}

	// sort
	qsort(g_arenaservers.serverlist, *g_arenaservers.numservers, sizeof(servernode_t), UI_ArenaServers_Compare);

	UI_ArenaServers_UpdateMenu();
}

/*
=================
UI_ArenaServers_DoRefresh
=================
*/
static void UI_ArenaServers_DoRefresh(void) {
	int i;
	int j;
	int time;
	int maxPing;
	int asType;
	char adrstr[MAX_ADDRESSLENGTH];
	char info[MAX_INFO_STRING];

	switch (g_servertype) {
	case UIAS_LOCAL:
		asType = AS_LOCAL;
		break;
	case UIAS_FAVORITES:
		asType = AS_FAVORITES;
		break;
	default:
		asType = AS_GLOBAL;
		break;
	}

	if (uis.realtime < g_arenaservers.refreshtime) {
		if (g_servertype != UIAS_FAVORITES) {
			if (g_servertype == UIAS_LOCAL) {
				if (!trap_LAN_GetServerCount(asType))
					return;
			} else if (trap_LAN_GetServerCount(asType) < 0) {
				// still waiting for response
				return;
			}
		}
	}

	if (uis.realtime < g_arenaservers.nextpingtime) {
		// wait for time trigger
		return;
	}

	// trigger at 20Hz intervals
	g_arenaservers.nextpingtime = (uis.realtime + 20);

	// process ping results
	maxPing = UI_ArenaServers_MaxPing();
	for (i = 0; i < MAX_PINGREQUESTS; i++) {
		trap_LAN_GetPing(i, adrstr, MAX_ADDRESSLENGTH, &time);
		if (!adrstr[0]) {
			// ignore empty or pending pings
			continue;
		}

		// find ping result in our local list
		for (j = 0; j < MAX_PINGREQUESTS; j++)
			if (!Q_stricmp(adrstr, g_arenaservers.pinglist[j].adrstr))
				break;

		if (j < MAX_PINGREQUESTS) {
			// found it
			if (!time) {
				time = uis.realtime - g_arenaservers.pinglist[j].start;
				if (time < maxPing) {
					// still waiting
					continue;
				}
			}

			if (time > maxPing) {
				// stale it out
				info[0] = '\0';
				time = maxPing;
			} else {
				trap_LAN_GetPingInfo(i, info, MAX_INFO_STRING);
			}

			// insert ping results
			UI_ArenaServers_Insert(adrstr, info, time);

			// clear this query from internal list
			g_arenaservers.pinglist[j].adrstr[0] = '\0';
		}

		// clear this query from external list
		trap_LAN_ClearPing(i);
	}

	// get results of servers query
	// counts can increase as servers respond
	if (g_servertype == UIAS_FAVORITES) {
		g_arenaservers.numqueriedservers = g_arenaservers.numfavoriteaddresses;
	} else {
		g_arenaservers.numqueriedservers = trap_LAN_GetServerCount(asType);
	}

	//	if (g_arenaservers.numqueriedservers > g_arenaservers.maxservers)
	//		g_arenaservers.numqueriedservers = g_arenaservers.maxservers;

	// send ping requests in reasonable bursts
	// iterate ping through all found servers
	for (i = 0; i < MAX_PINGREQUESTS && g_arenaservers.currentping < g_arenaservers.numqueriedservers; i++) {
		if (trap_LAN_GetPingQueueCount() >= MAX_PINGREQUESTS) {
			// ping queue is full
			break;
		}

		// find empty slot
		for (j = 0; j < MAX_PINGREQUESTS; j++)
			if (!g_arenaservers.pinglist[j].adrstr[0])
				break;

		if (j >= MAX_PINGREQUESTS)
			// no empty slots available yet - wait for timeout
			break;

		// get an address to ping

		if (g_servertype == UIAS_FAVORITES) {
			Q_strncpyz(adrstr, g_arenaservers.favoriteaddresses[g_arenaservers.currentping], sizeof(adrstr));
		} else {
			trap_LAN_GetServerAddressString(asType, g_arenaservers.currentping, adrstr, MAX_ADDRESSLENGTH);
		}

		Q_strncpyz(g_arenaservers.pinglist[j].adrstr, adrstr, sizeof(g_arenaservers.pinglist[j].adrstr));
		g_arenaservers.pinglist[j].start = uis.realtime;

		trap_Cmd_ExecuteText(EXEC_NOW, va("ping %s\n", adrstr));

		// advance to next server
		g_arenaservers.currentping++;
	}

	if (!trap_LAN_GetPingQueueCount()) {
		// all pings completed
		UI_ArenaServers_StopRefresh();
		return;
	}

	// update the user interface with ping status
	UI_ArenaServers_UpdateMenu();
}

/*
=================
UI_ArenaServers_StartRefresh
=================
*/
static void UI_ArenaServers_StartRefresh(void) {
	int i;
	char myargs[32], protocol[32];

	memset(g_arenaservers.serverlist, 0, (g_arenaservers.maxservers * sizeof(table_t)));

	for (i = 0; i < MAX_PINGREQUESTS; i++) {
		g_arenaservers.pinglist[i].adrstr[0] = '\0';
		trap_LAN_ClearPing(i);
	}

	g_arenaservers.refreshservers = qtrue;
	g_arenaservers.currentping = 0;
	g_arenaservers.nextpingtime = 0;
	*g_arenaservers.numservers = 0;
	g_arenaservers.numqueriedservers = 0;

	// allow max 7 seconds for responses
	g_arenaservers.refreshtime = (uis.realtime + 7000);

	// place menu in zeroed state
	UI_ArenaServers_UpdateMenu();

	if (g_servertype == UIAS_LOCAL) {
		trap_Cmd_ExecuteText(EXEC_APPEND, "localservers\n");
		return;
	}

	if ((g_servertype >= UIAS_GLOBAL_START) && (g_servertype <= UIAS_GLOBAL_MAX)) {
		switch (g_arenaservers.gametype.curvalue) {
		default:
		case GAMES_ALL:
			myargs[0] = '\0';
			break;

			// FIXME: How are these _really_ handled by masterserver?

		case GAMES_FFA:
			Q_strncpyz(myargs, " ffa", sizeof(myargs));
			break;

		case GAMES_SYC:
			Q_strncpyz(myargs, " syc", sizeof(myargs));
			break;

		case GAMES_LPS:
			Q_strncpyz(myargs, " lps", sizeof(myargs));
			break;

		case GAMES_TDM:
			Q_strncpyz(myargs, " tdm", sizeof(myargs));
			break;

		case GAMES_CTL:
			Q_strncpyz(myargs, " ctl", sizeof(myargs));
			break;

		case GAMES_1LCTL:
			Q_strncpyz(myargs, " 1lctl", sizeof(myargs));
			break;

		case GAMES_TSYC:
			Q_strncpyz(myargs, " tsyc", sizeof(myargs));
			break;

		case GAMES_BB:
			Q_strncpyz(myargs, " bb", sizeof(myargs));
			break;

		case GAMES_FT:
			Q_strncpyz(myargs, " ft", sizeof(myargs));
			break;
		}

		if (g_emptyservers) {
			Q_strcat(myargs, sizeof(myargs), " empty");
		}

		if (g_fullservers) {
			Q_strcat(myargs, sizeof(myargs), " full");
		}

		protocol[0] = '\0';
		trap_Cvar_VariableStringBuffer("debug_protocol", protocol, sizeof(protocol));

		i = atoi(&master_items[g_servertype][ARRAY_LEN(GLOBAL_ITEM_NAME) - 1]);

		if (strlen(protocol)) {
			trap_Cmd_ExecuteText(EXEC_APPEND, va("globalservers %d %s%s\n", i - 1, protocol, myargs));
		} else {
			trap_Cmd_ExecuteText(
				EXEC_APPEND, va("globalservers %d %d%s\n", i - 1, (int)trap_Cvar_VariableValue("protocol"), myargs));
		}
	}
}

/*
=================
UI_ArenaServers_SaveChanges
=================
*/
static void UI_ArenaServers_SaveChanges(void) {
	int i;

	for (i = 0; i < g_arenaservers.numfavoriteaddresses; i++)
		trap_Cvar_Set(va("server%d", i + 1), g_arenaservers.favoriteaddresses[i]);

	for (; i < MAX_FAVORITESERVERS; i++)
		trap_Cvar_Set(va("server%d", i + 1), "");
}

/*
=================
UI_ArenaServers_Sort
=================
*/
static void UI_ArenaServers_Sort(int type) {
	if (g_sortkey == type) {
		return;
	}

	g_sortkey = type;
	qsort(g_arenaservers.serverlist, *g_arenaservers.numservers, sizeof(servernode_t), UI_ArenaServers_Compare);
}

/*
=================
UI_ArenaServers_SetType
=================
*/
static int UI_ArenaServers_SetType(int type) {
	g_servertype = type;

	if (type == UIAS_FAVORITES) {
		g_arenaservers.remove.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		g_arenaservers.addFav.generic.flags |= (QMF_INACTIVE | QMF_HIDDEN);
		g_arenaservers.addAllFav.generic.flags |= (QMF_INACTIVE | QMF_HIDDEN);
		g_arenaservers.serverlist = g_favoriteserverlist;
		g_arenaservers.numservers = &g_numfavoriteservers;
		g_arenaservers.maxservers = MAX_FAVORITESERVERS;
	} else if (type >= UIAS_GLOBAL_START && type <= UIAS_GLOBAL_MAX) {
		g_arenaservers.remove.generic.flags |= (QMF_INACTIVE | QMF_HIDDEN);
		g_arenaservers.addFav.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		g_arenaservers.addAllFav.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		g_arenaservers.serverlist = g_globalserverlist;
		g_arenaservers.numservers = &g_numglobalservers;
		g_arenaservers.maxservers = MAX_GLOBALSERVERS;
	} else {
		// includes case type == UIAS_LOCAL
		g_arenaservers.remove.generic.flags |= (QMF_INACTIVE | QMF_HIDDEN);
		g_arenaservers.addFav.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		g_arenaservers.addAllFav.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		g_arenaservers.serverlist = g_localserverlist;
		g_arenaservers.numservers = &g_numlocalservers;
		g_arenaservers.maxservers = MAX_LOCALSERVERS;
	}

	UI_ArenaServers_StartRefresh();

	return type;
}

/*
=================
UI_ArenaServers_Event
=================
*/
static void UI_ArenaServers_Event(void *ptr, int event) {
	int id;
	int i;

	id = ((menucommon_s *)ptr)->id;

	if (event != QM_ACTIVATED && id != ID_LIST) {
		return;
	}

	switch (id) {
	case ID_MASTER:
		g_arenaservers.master.curvalue = UI_ArenaServers_SetType(g_arenaservers.master.curvalue);
		trap_Cvar_SetValue("ui_browserMaster", g_arenaservers.master.curvalue);
		break;

	case ID_GAMETYPE:
		trap_Cvar_SetValue("ui_browserGameType", g_arenaservers.gametype.curvalue);
		g_gametype = g_arenaservers.gametype.curvalue;
		UI_ArenaServers_UpdateMenu();
		break;

	case ID_SORTKEY:
		trap_Cvar_SetValue("ui_browserSortKey", g_arenaservers.sortkey.curvalue);
		UI_ArenaServers_Sort(g_arenaservers.sortkey.curvalue);
		UI_ArenaServers_UpdateMenu();
		break;

	case ID_SHOW_FULL:
		trap_Cvar_SetValue("ui_browserShowFull", g_arenaservers.showfull.curvalue);
		g_fullservers = g_arenaservers.showfull.curvalue;
		UI_ArenaServers_UpdateMenu();
		break;

	case ID_SHOW_EMPTY:
		trap_Cvar_SetValue("ui_browserShowEmpty", g_arenaservers.showempty.curvalue);
		g_emptyservers = g_arenaservers.showempty.curvalue;
		UI_ArenaServers_UpdateMenu();
		break;

	case ID_ONLY_HUMANS:
		trap_Cvar_SetValue("ui_browserOnlyHumans", g_arenaservers.onlyhumans.curvalue);
		g_onlyhumans = g_arenaservers.onlyhumans.curvalue;
		UI_ArenaServers_UpdateMenu();
		break;

	case ID_HIDE_PRIVATE:
		trap_Cvar_SetValue("ui_browserHidePrivate", g_arenaservers.hideprivate.curvalue);
		g_hideprivate = g_arenaservers.hideprivate.curvalue;
		UI_ArenaServers_UpdateMenu();
		break;

	case ID_LIST:
		if (event == QM_GOTFOCUS) {
			UI_ArenaServers_UpdatePicture();
		}
		break;

	case ID_SCROLL_UP:
		ScrollList_Key(&g_arenaservers.list, K_UPARROW);
		break;

	case ID_SCROLL_DOWN:
		ScrollList_Key(&g_arenaservers.list, K_DOWNARROW);
		break;

	case ID_BACK:
		UI_ArenaServers_StopRefresh();
		UI_ArenaServers_SaveChanges();
		UI_PopMenu();
		break;

	case ID_REFRESH:
		UI_ArenaServers_StartRefresh();
		break;

	case ID_SPECIFY:
		UI_SpecifyServerMenu();
		break;

	case ID_CONNECT:
		UI_ArenaServers_Fight();
		break;

	case ID_REMOVE:
		UI_ArenaServers_Remove();
		UI_ArenaServers_UpdateMenu();
		break;

	case ID_ADDTOFAV:
		if (!g_arenaservers.list.numitems) {
			break;
		}
		UI_ArenaServers_AddToFavorites(g_arenaservers.table[g_arenaservers.list.curvalue].servernode->adrstr);
		break;

	case ID_ADDALLTOFAV:
		for (i = 0; i < g_arenaservers.list.numitems; i++) {
			UI_ArenaServers_AddToFavorites(g_arenaservers.table[i].servernode->adrstr);
		}
		break;
	}
}

/*
=================
UI_ArenaServers_MenuDraw
=================
*/
static void UI_ArenaServers_MenuDraw(void) {
	if (g_arenaservers.refreshservers)
		UI_ArenaServers_DoRefresh();

	Menu_Draw(&g_arenaservers.menu);
}

/*
=================
UI_ArenaServers_MenuKey
=================
*/
static sfxHandle_t UI_ArenaServers_MenuKey(int key) {
	if (key == K_SPACE && g_arenaservers.refreshservers) {
		UI_ArenaServers_StopRefresh();
		return menuMoveSound;
	}

	if ((key == K_DEL || key == K_KP_DEL) && (g_servertype == UIAS_FAVORITES) &&
		(Menu_ItemAtCursor(&g_arenaservers.menu) == &g_arenaservers.list)) {
		UI_ArenaServers_Remove();
		UI_ArenaServers_UpdateMenu();
		return menuMoveSound;
	}

	if (key == K_MOUSE2 || key == K_ESCAPE) {
		UI_ArenaServers_StopRefresh();
		UI_ArenaServers_SaveChanges();
	}

	if (key == K_MWHEELUP) {
		ScrollList_Key(&g_arenaservers.list, K_UPARROW);
	}
	if (key == K_MWHEELDOWN) {
		ScrollList_Key(&g_arenaservers.list, K_DOWNARROW);
	}

	if (key == K_ENTER || key == K_KP_ENTER) {
		UI_ArenaServers_Fight();
	}

	return Menu_DefaultKey(&g_arenaservers.menu, key);
}

/*
=================
UI_ArenaServers_ConstructMasterList
=================
*/
static int UI_ArenaServers_ConstructMasterList(void) {
	int index, listind = UIAS_GLOBAL_START;
	char masterstr[2], cvarname[sizeof("sv_master1234567890")];

	for (index = 0; index < MAX_MASTER_SERVERS; index++) {
		Com_sprintf(cvarname, sizeof(cvarname), "sv_master%d", index + 1);
		trap_Cvar_VariableStringBuffer(cvarname, masterstr, sizeof(masterstr));

		if (*masterstr) {
			master_items[listind] = global_names[index];
			listind++;
		}
	}

	master_items[listind] = NULL;
	return listind;
}

/*
=================
UI_ArenaServers_MenuInit
=================
*/
static void UI_ArenaServers_MenuInit(void) {
	int i;
	int y;
	int numMasters;
	static char statusbuffer[MAX_STATUSLENGTH];

	// zero set all our globals
	memset(&g_arenaservers, 0, sizeof(arenaservers_t));

	UI_ArenaServers_Cache();

	g_arenaservers.menu.fullscreen = qtrue;
	g_arenaservers.menu.wrapAround = qtrue;
	g_arenaservers.menu.draw = UI_ArenaServers_MenuDraw;
	g_arenaservers.menu.key = UI_ArenaServers_MenuKey;
	g_arenaservers.menu.bgparts = (BGP_SERVERS | BGP_MENUFX);

	numMasters = UI_ArenaServers_ConstructMasterList();

	y = (80 - SMALLCHAR_HEIGHT);
	g_arenaservers.master.generic.type = MTYPE_SPINCONTROL;
	g_arenaservers.master.generic.name = "Servers:";
	g_arenaservers.master.generic.flags = (QMF_PULSEIFFOCUS | QMF_SMALLFONT);
	g_arenaservers.master.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.master.generic.id = ID_MASTER;
	g_arenaservers.master.generic.x = 432;
	g_arenaservers.master.generic.y = y;
	g_arenaservers.master.itemnames = master_items;

	y += SMALLCHAR_HEIGHT;
	g_arenaservers.gametype.generic.type = MTYPE_SPINCONTROL;
	g_arenaservers.gametype.generic.name = "Game Type:";
	g_arenaservers.gametype.generic.flags = (QMF_PULSEIFFOCUS | QMF_SMALLFONT);
	g_arenaservers.gametype.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.gametype.generic.id = ID_GAMETYPE;
	g_arenaservers.gametype.generic.x = 432;
	g_arenaservers.gametype.generic.y = y;
	g_arenaservers.gametype.itemnames = servertype_items;

	y += SMALLCHAR_HEIGHT;
	g_arenaservers.sortkey.generic.type = MTYPE_SPINCONTROL;
	g_arenaservers.sortkey.generic.name = "Sort By:";
	g_arenaservers.sortkey.generic.flags = (QMF_PULSEIFFOCUS | QMF_SMALLFONT);
	g_arenaservers.sortkey.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.sortkey.generic.id = ID_SORTKEY;
	g_arenaservers.sortkey.generic.x = 432;
	g_arenaservers.sortkey.generic.y = y;
	g_arenaservers.sortkey.itemnames = sortkey_items;

	y += SMALLCHAR_HEIGHT;
	g_arenaservers.showfull.generic.type = MTYPE_RADIOBUTTON;
	g_arenaservers.showfull.generic.name = "Show Full:";
	g_arenaservers.showfull.generic.flags = (QMF_PULSEIFFOCUS | QMF_SMALLFONT);
	g_arenaservers.showfull.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.showfull.generic.id = ID_SHOW_FULL;
	g_arenaservers.showfull.generic.x = 432;
	g_arenaservers.showfull.generic.y = y;

	y += SMALLCHAR_HEIGHT;
	g_arenaservers.showempty.generic.type = MTYPE_RADIOBUTTON;
	g_arenaservers.showempty.generic.name = "Show Empty:";
	g_arenaservers.showempty.generic.flags = (QMF_PULSEIFFOCUS | QMF_SMALLFONT);
	g_arenaservers.showempty.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.showempty.generic.id = ID_SHOW_EMPTY;
	g_arenaservers.showempty.generic.x = 432;
	g_arenaservers.showempty.generic.y = y;

	y += SMALLCHAR_HEIGHT;
	g_arenaservers.onlyhumans.generic.type = MTYPE_RADIOBUTTON;
	g_arenaservers.onlyhumans.generic.name = "Humans Only:";
	g_arenaservers.onlyhumans.generic.flags = (QMF_PULSEIFFOCUS | QMF_SMALLFONT);
	g_arenaservers.onlyhumans.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.onlyhumans.generic.id = ID_ONLY_HUMANS;
	g_arenaservers.onlyhumans.generic.x = 432;
	g_arenaservers.onlyhumans.generic.y = y;

	y += SMALLCHAR_HEIGHT;
	g_arenaservers.hideprivate.generic.type = MTYPE_RADIOBUTTON;
	g_arenaservers.hideprivate.generic.name = "Hide Private:";
	g_arenaservers.hideprivate.generic.flags = (QMF_PULSEIFFOCUS | QMF_SMALLFONT);
	g_arenaservers.hideprivate.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.hideprivate.generic.id = ID_HIDE_PRIVATE;
	g_arenaservers.hideprivate.generic.x = 432;
	g_arenaservers.hideprivate.generic.y = y;

	g_arenaservers.list.generic.type = MTYPE_SCROLLLIST;
	g_arenaservers.list.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	g_arenaservers.list.generic.id = ID_LIST;
	g_arenaservers.list.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.list.generic.x = 100;
	g_arenaservers.list.generic.y = 204;
	g_arenaservers.list.width = MAX_LISTBOXWIDTH;
	g_arenaservers.list.height = 11;
	g_arenaservers.list.itemnames = (const char **)g_arenaservers.items;
	for (i = 0; i < MAX_LISTBOXITEMS; i++) {
		g_arenaservers.items[i] = g_arenaservers.table[i].buff;
	}

	g_arenaservers.mappic.generic.type = MTYPE_BITMAP;
	g_arenaservers.mappic.generic.flags = (QMF_LEFT_JUSTIFY | QMF_INACTIVE);
	g_arenaservers.mappic.generic.x = 100;
	g_arenaservers.mappic.generic.y = 66;
	g_arenaservers.mappic.width = 128;
	g_arenaservers.mappic.height = 96;
	g_arenaservers.mappic.errorpic = UNKNOWNMAP;

	g_arenaservers.arrowup.generic.type = MTYPE_BITMAP;
	g_arenaservers.arrowup.generic.name = ARROWUP0;
	g_arenaservers.arrowup.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	g_arenaservers.arrowup.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.arrowup.generic.id = ID_SCROLL_UP;
	g_arenaservers.arrowup.generic.x = 744;
	g_arenaservers.arrowup.generic.y = 224;
	g_arenaservers.arrowup.width = 25;
	g_arenaservers.arrowup.height = 60;
	g_arenaservers.arrowup.focuspic = ARROWUP1;
	g_arenaservers.arrowup.focuspicinstead = qtrue;

	g_arenaservers.arrowdown.generic.type = MTYPE_BITMAP;
	g_arenaservers.arrowdown.generic.name = ARROWDN0;
	g_arenaservers.arrowdown.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	g_arenaservers.arrowdown.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.arrowdown.generic.id = ID_SCROLL_DOWN;
	g_arenaservers.arrowdown.generic.x = 744;
	g_arenaservers.arrowdown.generic.y = 300;
	g_arenaservers.arrowdown.width = 25;
	g_arenaservers.arrowdown.height = 60;
	g_arenaservers.arrowdown.focuspic = ARROWDN1;
	g_arenaservers.arrowdown.focuspicinstead = qtrue;

	y = 390;
	g_arenaservers.status.generic.type = MTYPE_TEXT;
	g_arenaservers.status.generic.x = 432;
	g_arenaservers.status.generic.y = y;
	g_arenaservers.status.string = statusbuffer;
	g_arenaservers.status.style = (UI_CENTER | UI_SMALLFONT);
	g_arenaservers.status.color = menu_text_color;

	y += SMALLCHAR_HEIGHT;
	g_arenaservers.statusbar.generic.type = MTYPE_TEXT;
	g_arenaservers.statusbar.generic.x = 432;
	g_arenaservers.statusbar.generic.y = y;
	g_arenaservers.statusbar.string = "";
	g_arenaservers.statusbar.style = UI_CENTER | UI_SMALLFONT;
	g_arenaservers.statusbar.color = text_color_normal;

	g_arenaservers.addFav.generic.type = MTYPE_TEXTS;
	g_arenaservers.addFav.fontHeight = 16.0f;
	g_arenaservers.addFav.generic.flags = QMF_PULSEIFFOCUS;
	g_arenaservers.addFav.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.addFav.generic.id = ID_ADDTOFAV;
	g_arenaservers.addFav.generic.x = 630;
	g_arenaservers.addFav.generic.y = 48;
	g_arenaservers.addFav.string = "Add to Favorites";
	g_arenaservers.addFav.style = UI_SMALLFONT;
	g_arenaservers.addFav.color = color_orange;
	g_arenaservers.addFav.focuscolor = color_red;

	g_arenaservers.addAllFav.generic.type = MTYPE_TEXTS;
	g_arenaservers.addAllFav.fontHeight = 16.0f;
	g_arenaservers.addAllFav.generic.flags = QMF_PULSEIFFOCUS;
	g_arenaservers.addAllFav.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.addAllFav.generic.id = ID_ADDALLTOFAV;
	g_arenaservers.addAllFav.generic.x = 630;
	g_arenaservers.addAllFav.generic.y = 64;
	g_arenaservers.addAllFav.string = "Add All to Favorites";
	g_arenaservers.addAllFav.style = UI_SMALLFONT;
	g_arenaservers.addAllFav.color = color_orange;
	g_arenaservers.addAllFav.focuscolor = color_red;

	g_arenaservers.back.generic.type = MTYPE_BITMAP;
	g_arenaservers.back.generic.name = BACK0;
	g_arenaservers.back.generic.flags = (QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS);
	g_arenaservers.back.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.back.generic.id = ID_BACK;
	g_arenaservers.back.generic.x = 8;
	g_arenaservers.back.generic.y = 446;
	g_arenaservers.back.width = 80;
	g_arenaservers.back.height = 40;
	g_arenaservers.back.focuspic = BACK1;

	g_arenaservers.specify.generic.type = MTYPE_BITMAP;
	g_arenaservers.specify.generic.name = SPECIFY0;
	g_arenaservers.specify.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	g_arenaservers.specify.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.specify.generic.id = ID_SPECIFY;
	g_arenaservers.specify.generic.x = 222;
	g_arenaservers.specify.generic.y = 426;
	g_arenaservers.specify.width = 120;
	g_arenaservers.specify.height = 40;
	g_arenaservers.specify.focuspic = SPECIFY1;
	g_arenaservers.specify.focuspicinstead = qtrue;

	g_arenaservers.refresh.generic.type = MTYPE_BITMAP;
	g_arenaservers.refresh.generic.name = REFRESH0;
	g_arenaservers.refresh.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	g_arenaservers.refresh.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.refresh.generic.id = ID_REFRESH;
	g_arenaservers.refresh.generic.x = 372;
	g_arenaservers.refresh.generic.y = 426;
	g_arenaservers.refresh.width = 120;
	g_arenaservers.refresh.height = 40;
	g_arenaservers.refresh.focuspic = REFRESH1;
	g_arenaservers.refresh.focuspicinstead = qtrue;

	g_arenaservers.remove.generic.type = MTYPE_BITMAP;
	g_arenaservers.remove.generic.name = DELETE0;
	g_arenaservers.remove.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	g_arenaservers.remove.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.remove.generic.id = ID_REMOVE;
	g_arenaservers.remove.generic.x = 522;
	g_arenaservers.remove.generic.y = 426;
	g_arenaservers.remove.width = 120;
	g_arenaservers.remove.height = 40;
	g_arenaservers.remove.focuspic = DELETE1;
	g_arenaservers.remove.focuspicinstead = qtrue;

	g_arenaservers.fight.generic.type = MTYPE_BITMAP;
	g_arenaservers.fight.generic.name = FIGHT0;
	g_arenaservers.fight.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	g_arenaservers.fight.generic.callback = UI_ArenaServers_Event;
	g_arenaservers.fight.generic.id = ID_CONNECT;
	g_arenaservers.fight.focuspic = FIGHT1;
	g_arenaservers.fight.generic.x = 776;
	g_arenaservers.fight.generic.y = 420;
	g_arenaservers.fight.width = 80;
	g_arenaservers.fight.height = 60;
	g_arenaservers.fight.focuspicinstead = qtrue;

	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.master);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.gametype);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.sortkey);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.showfull);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.showempty);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.onlyhumans);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.hideprivate);

	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.mappic);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.list);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.status);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.statusbar);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.arrowup);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.arrowdown);

	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.addFav);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.addAllFav);

	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.specify);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.refresh);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.remove);
	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.fight);

	Menu_AddItem(&g_arenaservers.menu, (void *)&g_arenaservers.back);

	UI_ArenaServers_LoadFavorites();

	// FIXME: Magical constants everywhere

	// FIXME: servertype_items does not match gametype_t!
	g_gametype = Com_Clamp(0, (sizeof(servertype_items) / sizeof(servertype_items[0]) - 2), ui_browserGameType.integer);
	g_arenaservers.gametype.curvalue = g_gametype;

	g_sortkey = Com_Clamp(0, 5, ui_browserSortKey.integer);
	g_arenaservers.sortkey.curvalue = g_sortkey;

	g_fullservers = Com_Clamp(0, 1, ui_browserShowFull.integer);
	g_arenaservers.showfull.curvalue = g_fullservers;

	g_emptyservers = Com_Clamp(0, 1, ui_browserShowEmpty.integer);
	g_arenaservers.showempty.curvalue = g_emptyservers;

	g_arenaservers.onlyhumans.curvalue = Com_Clamp(0, 1, ui_browserOnlyHumans.integer);
	g_onlyhumans = ui_browserOnlyHumans.integer;

	g_hideprivate = Com_Clamp(0, 1, ui_browserHidePrivate.integer);
	g_arenaservers.hideprivate.curvalue = g_hideprivate;

	g_servertype = ui_browserMaster.integer;
	if (g_servertype < 0 || g_servertype >= numMasters)
		g_servertype = 0;

	// force to initial state and refresh
	g_arenaservers.master.curvalue = g_servertype = UI_ArenaServers_SetType(g_servertype);

	trap_Cvar_Register(NULL, "debug_protocol", "", 0);
}

/*
=================
UI_ArenaServers_Cache
=================
*/
void UI_ArenaServers_Cache(void) {
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
	trap_R_RegisterShaderNoMip(SPECIFY0);
	trap_R_RegisterShaderNoMip(SPECIFY1);
	trap_R_RegisterShaderNoMip(REFRESH0);
	trap_R_RegisterShaderNoMip(REFRESH1);
	trap_R_RegisterShaderNoMip(DELETE0);
	trap_R_RegisterShaderNoMip(DELETE1);
	trap_R_RegisterShaderNoMip(FIGHT0);
	trap_R_RegisterShaderNoMip(FIGHT1);
	trap_R_RegisterShaderNoMip(UNKNOWNMAP);

	if (!*global_names[0]) {
		int index;

		for (index = 0; index < MAX_MASTER_SERVERS; index++)
			Com_sprintf(global_names[index], sizeof(global_names[index]), "%s %d", GLOBAL_ITEM_NAME, index + 1);
	}
}

/*
=================
UI_ArenaServersMenu
=================
*/
void UI_ArenaServersMenu(void) {
	UI_ArenaServers_MenuInit();
	UI_PushMenu(&g_arenaservers.menu);
}
