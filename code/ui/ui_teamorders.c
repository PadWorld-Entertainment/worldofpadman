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

TEAM ORDERS MENU

=======================================================================
*/

#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"

#define ID_LIST_BOTS 10
#define ID_LIST_ORDERS 11

#define ORDER_HEIGHT 24
#define VISIBLE_ORDERS 8

static const struct {
	const char *label;
	const char *message;
	unsigned int modes;
} orders[] = {
	{"I am the leader", "I am the leader", 0},
	{"Follow me", "follow me", 0},
	{"Defend the base", "defend the base", (1 << GT_CTF) | (1 << GT_1FCTF)},
	{"Get the lolly", "get the lolly", (1 << GT_CTF) | (1 << GT_1FCTF)},
	{"Camp here", "camp here", 0},
	{"Roam", "roam", 0},
	{"Report", "report", 0},
	{"Give cartridges", "give your cartridges to me", 1 << GT_SPRAY},
	{"Secure a balloon", "secure a balloon", 1 << GT_BALLOON},
	{"Defend a balloon", "defend a balloon", 1 << GT_BALLOON},
	{"Thaw a teammate", "thaw a teammate", 1 << GT_FREEZETAG},
	{"I relinquish command", "I stop being the leader", 0},
};

typedef struct {
	menuframework_s menu;

	menulist_s list;

	menubitmap_s back;

	int gametype;
	int numBots;
	int selectedBot;
	const char *bots[MAX_CLIENTS];
	char botNames[MAX_CLIENTS][MAX_NAME_LENGTH];
	int botClients[MAX_CLIENTS];
	const char *orderLabels[ARRAY_LEN(orders)];
	const char *orderMessages[ARRAY_LEN(orders)];
} teamOrdersMenuInfo_t;

static teamOrdersMenuInfo_t teamOrdersMenuInfo;

/*
===============
UI_TeamOrdersMenu_BackEvent
===============
*/
static void UI_TeamOrdersMenu_BackEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}
	UI_PopMenu();
}

/*
===============
UI_TeamOrdersMenu_SetList
===============
*/
static void UI_TeamOrdersMenu_SetList(int id) {
	menulist_s *list = &teamOrdersMenuInfo.list;

	list->generic.id = id;
	list->curvalue = list->oldvalue = list->top = 0;
	if (id == ID_LIST_BOTS) {
		list->numitems = teamOrdersMenuInfo.numBots;
		list->itemnames = teamOrdersMenuInfo.bots;
	} else {
		int i;
		list->numitems = 0;
		list->itemnames = teamOrdersMenuInfo.orderLabels;
		for (i = 0; i < ARRAY_LEN(orders); i++) {
			if (orders[i].modes && !(orders[i].modes & (1 << teamOrdersMenuInfo.gametype)))
				continue;
			teamOrdersMenuInfo.orderLabels[list->numitems] = orders[i].label;
			teamOrdersMenuInfo.orderMessages[list->numitems++] = orders[i].message;
		}
	}

	list->generic.bottom = list->generic.top + MIN(list->numitems, VISIBLE_ORDERS) * ORDER_HEIGHT - 1;
}

/*
=================
UI_TeamOrdersMenu_Key
=================
*/
static sfxHandle_t UI_TeamOrdersMenu_Key(int key) {
	menulist_s *l;
	int x;
	int y;
	int index;

	l = (menulist_s *)Menu_ItemAtCursor(&teamOrdersMenuInfo.menu);
	if (l != &teamOrdersMenuInfo.list) {
		return Menu_DefaultKey(&teamOrdersMenuInfo.menu, key);
	}

	switch (key) {
	case K_MOUSE1:
		x = l->generic.left;
		y = l->generic.top;
		if (UI_CursorInRect(x, y, l->generic.right - x, l->generic.bottom - y)) {
			index = l->top + (uis.cursory - y) / ORDER_HEIGHT;
			l->oldvalue = l->curvalue;
			l->curvalue = index;

			if (l->generic.callback) {
				l->generic.callback(l, QM_ACTIVATED);
				return menuMoveSound;
			}
		}
		return menuNullSound;

	case K_MWHEELUP:
	case K_KP_UPARROW:
	case K_UPARROW:
		l->oldvalue = l->curvalue;

		if (l->curvalue == 0) {
			l->curvalue = l->numitems - 1;
		} else {
			l->curvalue--;
		}
		if (l->curvalue < l->top)
			l->top = l->curvalue;
		else if (l->curvalue >= l->top + VISIBLE_ORDERS)
			l->top = l->curvalue - VISIBLE_ORDERS + 1;
		return menuMoveSound;

	case K_MWHEELDOWN:
	case K_KP_DOWNARROW:
	case K_DOWNARROW:
		l->oldvalue = l->curvalue;

		if (l->curvalue == l->numitems - 1) {
			l->curvalue = 0;
		} else {
			l->curvalue++;
		}
		if (l->curvalue < l->top)
			l->top = l->curvalue;
		else if (l->curvalue >= l->top + VISIBLE_ORDERS)
			l->top = l->curvalue - VISIBLE_ORDERS + 1;
		return menuMoveSound;
	}

	return Menu_DefaultKey(&teamOrdersMenuInfo.menu, key);
}

/*
=================
UI_TeamOrdersMenu_ListDraw
=================
*/
static void UI_TeamOrdersMenu_ListDraw(void *self) {
	const menulist_s *l;
	int x;
	int y;
	int i;
	const float *color;
	qboolean hasfocus;
	int style;

	l = (const menulist_s *)self;

	hasfocus = (l->generic.parent->cursor == l->generic.menuPosition);

	x = l->generic.x;
	y = l->generic.y;
	for (i = l->top; i < l->numitems && i < l->top + VISIBLE_ORDERS; i++) {
		style = UI_LEFT | UI_SMALLFONT | UI_CENTER;
		if (i == l->curvalue) {
			color = color_orange;
			if (hasfocus) {
				style |= UI_PULSE;
			}
		} else {
			color = color_black;
		}

		UI_DrawStringNS(x, y, l->itemnames[i], style, 18.0f, color);
		y += ORDER_HEIGHT;
	}
}

/*
===============
UI_TeamOrdersMenu_ListEvent
===============
*/
static void UI_TeamOrdersMenu_ListEvent(void *ptr, int event) {
	const menulist_s *list = ptr;
	const int selection = list->curvalue;
	int target;

	if (event != QM_ACTIVATED || selection < 0 || selection >= list->numitems)
		return;

	if (list->generic.id == ID_LIST_BOTS) {
		teamOrdersMenuInfo.selectedBot = selection;
		UI_TeamOrdersMenu_SetList(ID_LIST_ORDERS);
		return;
	}

	target = teamOrdersMenuInfo.botClients[teamOrdersMenuInfo.selectedBot];
	if (target < 0)
		trap_Cmd_ExecuteText(EXEC_APPEND, va("say_team \"%s\"\n", teamOrdersMenuInfo.orderMessages[selection]));
	else
		trap_Cmd_ExecuteText(EXEC_APPEND, va("tell %d \"%s\"\n", target, teamOrdersMenuInfo.orderMessages[selection]));
	UI_PopMenu();
}

static qboolean UI_TeamOrdersFindBots(teamOrdersMenuInfo_t *menu) {
	uiClientState_t cs;
	char info[MAX_INFO_STRING];
	int gametype, team, numPlayers, n;

	trap_GetClientState(&cs);
	if (cs.connState != CA_ACTIVE || cs.clientNum < 0 || cs.clientNum >= MAX_CLIENTS)
		return qfalse;

	trap_GetConfigString(CS_SERVERINFO, info, sizeof(info));
	gametype = atoi(Info_ValueForKey(info, "g_gametype"));
	if (gametype < GT_TEAM || gametype >= GT_MAX_GAME_TYPE)
		return qfalse;
	numPlayers = Com_Clamp(0, MAX_CLIENTS, atoi(Info_ValueForKey(info, "sv_maxclients")));

	trap_GetConfigString(CS_PLAYERS + cs.clientNum, info, sizeof(info));
	team = atoi(Info_ValueForKey(info, "t"));
	if (team != TEAM_RED && team != TEAM_BLUE)
		return qfalse;

	if (menu) {
		menu->gametype = gametype;
		menu->numBots = 1;
		menu->botClients[0] = -1;
		menu->bots[0] = "Everyone";
	}

	for (n = 0; n < numPlayers; n++) {
		const char *name;
		int i;
		if (n == cs.clientNum)
			continue;
		trap_GetConfigString(CS_PLAYERS + n, info, sizeof(info));
		if (atoi(Info_ValueForKey(info, "t")) != team || atoi(Info_ValueForKey(info, "skill")) <= 0)
			continue;
		name = Info_ValueForKey(info, "n");
		if (!*name)
			continue;
		if (!menu)
			return qtrue;
		i = menu->numBots++;
		menu->botClients[i] = n;
		Q_strncpyz(menu->botNames[i], name, sizeof(menu->botNames[i]));
		Q_CleanStr(menu->botNames[i]);
		menu->bots[i] = menu->botNames[i];
	}
	return menu && menu->numBots > 1;
}

qboolean UI_TeamOrdersAvailable(void) {
	return UI_TeamOrdersFindBots(NULL);
}

/*
#######################
  TeamOrderMenu_Draw
#######################
*/
static void TeamOrderMenu_Draw(void) {
	UI_DrawIngameBG();
	UI_DrawProportionalString(SCREEN_CENTER, 110, "TEAM ORDERS", UI_CENTER | UI_SMALLFONT, color_black);

	// standard menu drawing
	Menu_Draw(&teamOrdersMenuInfo.menu);
}

/*
===============
UI_TeamOrdersMenu_Init
===============
*/
static qboolean UI_TeamOrdersMenu_Init(void) {
	UI_TeamOrdersMenu_Cache();

	memset(&teamOrdersMenuInfo, 0, sizeof(teamOrdersMenuInfo));
	teamOrdersMenuInfo.menu.fullscreen = qfalse;
	teamOrdersMenuInfo.menu.key = UI_TeamOrdersMenu_Key;

	if (!UI_TeamOrdersFindBots(&teamOrdersMenuInfo))
		return qfalse;

	teamOrdersMenuInfo.menu.draw = TeamOrderMenu_Draw;

	teamOrdersMenuInfo.list.generic.type = MTYPE_SCROLLLIST;
	teamOrdersMenuInfo.list.generic.flags = QMF_PULSEIFFOCUS;
	teamOrdersMenuInfo.list.generic.ownerdraw = UI_TeamOrdersMenu_ListDraw;
	teamOrdersMenuInfo.list.generic.callback = UI_TeamOrdersMenu_ListEvent;
	teamOrdersMenuInfo.list.generic.x = SCREEN_CENTER;
	teamOrdersMenuInfo.list.generic.y = 140;

	teamOrdersMenuInfo.back.generic.type = MTYPE_BITMAP;
	teamOrdersMenuInfo.back.generic.name = BACK0;
	teamOrdersMenuInfo.back.generic.flags = QMF_PULSEIFFOCUS;
	teamOrdersMenuInfo.back.generic.x = SCREEN_CENTER - 95;
	teamOrdersMenuInfo.back.generic.y = 340;
	teamOrdersMenuInfo.back.generic.callback = UI_TeamOrdersMenu_BackEvent;
	teamOrdersMenuInfo.back.width = 50;
	teamOrdersMenuInfo.back.height = 25;
	teamOrdersMenuInfo.back.focuspic = BACK1;
	teamOrdersMenuInfo.back.focuspicinstead = qtrue;

	Menu_AddItem(&teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.list);
	Menu_AddItem(&teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.back);

	teamOrdersMenuInfo.list.generic.left = SCREEN_CENTER - 100;
	teamOrdersMenuInfo.list.generic.top = teamOrdersMenuInfo.list.generic.y;
	teamOrdersMenuInfo.list.generic.right = SCREEN_CENTER + 100;
	UI_TeamOrdersMenu_SetList(ID_LIST_BOTS);
	return qtrue;
}

/*
=================
UI_TeamOrdersMenu_Cache
=================
*/
void UI_TeamOrdersMenu_Cache(void) {
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
}

/*
===============
UI_TeamOrdersMenu
===============
*/
void UI_TeamOrdersMenu(void) {
	if (UI_TeamOrdersMenu_Init()) {
		trap_Cvar_Set("cl_paused", "1");
		UI_PushMenu(&teamOrdersMenuInfo.menu);
	}
}

void UI_TeamOrdersMenu_f(void) {
	UI_TeamOrdersMenu();
}
