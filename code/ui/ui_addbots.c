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

ADD BOTS MENU

=======================================================================
*/

#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define ARROWUP0 "menu/arrows/arryel_up0b"
#define ARROWUP1 "menu/arrows/arryel_up1"
#define ARROWDN0 "menu/arrows/arryel_dn0b"
#define ARROWDN1 "menu/arrows/arryel_dn1"

#define NUM_BOTS 6

#define ID_BACK 10
#define ID_GO 11
#define ID_LIST 12
#define ID_SCROLL_UP 13
#define ID_SCROLL_DOWN 14
#define ID_SKILL 15
#define ID_TEAM 16
#define ID_BOTNAME0 20
#define ID_BOTNAME1 21
#define ID_BOTNAME2 22
#define ID_BOTNAME3 23
#define ID_BOTNAME4 24
#define ID_BOTNAME5 25
#define ID_BOTNAME6 26

typedef struct {
	menuframework_s menu;

	menutext_s bots[NUM_BOTS];
	menubitmap_s bot_pics[NUM_BOTS];

	menulist_s skill;
	menulist_s team;

	menutext_s go;
	menubitmap_s back;
	menubitmap_s arrowup;
	menubitmap_s arrowdown;

	int numBots;
	int delay;
	int baseBotNum;
	int selectedBotNum;
	int sortedBotNums[MAX_BOTS];

	char boticonnames[NUM_BOTS][MAX_QPATH];
	char botnames[NUM_BOTS][32];
} addBotsMenuInfo_t;

static addBotsMenuInfo_t addBotsMenuInfo;

/*
=================
UI_AddBotsMenu_FightEvent
=================
*/
static void UI_AddBotsMenu_FightEvent(void *ptr, int event) {
	const char *team;
	int skill;

	if (event != QM_ACTIVATED) {
		return;
	}

	team = addBotsMenuInfo.team.itemnames[addBotsMenuInfo.team.curvalue];
	skill = addBotsMenuInfo.skill.curvalue + 1;

	trap_Cmd_ExecuteText(EXEC_APPEND,
						 va("addbot %s %i %s %i\n", addBotsMenuInfo.botnames[addBotsMenuInfo.selectedBotNum], skill,
							team, addBotsMenuInfo.delay));

	addBotsMenuInfo.delay += 1500;
}

/*
=================
UI_AddBotsMenu_BotEvent
=================
*/
static void UI_AddBotsMenu_BotEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	addBotsMenuInfo.bots[addBotsMenuInfo.selectedBotNum].color = colorBlack;
	addBotsMenuInfo.bots[addBotsMenuInfo.selectedBotNum].focuscolor = colorDkLilac;
	addBotsMenuInfo.selectedBotNum = ((menucommon_s *)ptr)->id - ID_BOTNAME0;
	addBotsMenuInfo.bots[addBotsMenuInfo.selectedBotNum].color = colorDkOrange;
	addBotsMenuInfo.bots[addBotsMenuInfo.selectedBotNum].focuscolor = color_orange;
}

/*
=================
UI_AddBotsMenu_BackEvent
=================
*/
static void UI_AddBotsMenu_BackEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}
	UI_PopMenu();
}

/*
=================
UI_AddBotsMenu_SetBotNames
=================
*/
static void UI_AddBotsMenu_SetBotNames(void) {
	int n;
	const char *info;

	for (n = 0; n < NUM_BOTS; n++) {
		info = UI_GetBotInfoByNumber(addBotsMenuInfo.sortedBotNums[addBotsMenuInfo.baseBotNum + n]);
		Q_strncpyz(addBotsMenuInfo.botnames[n], Info_ValueForKey(info, "name"), sizeof(addBotsMenuInfo.botnames[n]));

		UI_ModelIcon(Info_ValueForKey(info, "model"), addBotsMenuInfo.boticonnames[n], MAX_QPATH);
		addBotsMenuInfo.bot_pics[n].shader = 0;
	}
}

/*
=================
UI_AddBotsMenu_UpEvent
=================
*/
static void UI_AddBotsMenu_UpEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	if (addBotsMenuInfo.baseBotNum > 0) {
		addBotsMenuInfo.baseBotNum--;
		UI_AddBotsMenu_SetBotNames();
	}
}

/*
=================
UI_AddBotsMenu_DownEvent
=================
*/
static void UI_AddBotsMenu_DownEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	if (addBotsMenuInfo.baseBotNum + NUM_BOTS < addBotsMenuInfo.numBots) {
		addBotsMenuInfo.baseBotNum++;
		UI_AddBotsMenu_SetBotNames();
	}
}

/*
=================
UI_AddBotsMenu_GetSortedBotNums
=================
*/
static void UI_AddBotsMenu_GetSortedBotNums(void) {
	int n;

	// initialize the array
	for (n = 0; n < addBotsMenuInfo.numBots; n++) {
		addBotsMenuInfo.sortedBotNums[n] = n;
	}

	qsort(addBotsMenuInfo.sortedBotNums, addBotsMenuInfo.numBots, sizeof(addBotsMenuInfo.sortedBotNums[0]),
		  BotListCompare);
}

/*
=================
UI_AddBotsMenu_Draw
=================
*/
static void UI_AddBotsMenu_Draw(void) {
	UI_DrawIngameBG();
	UI_DrawProportionalString(SCREEN_CENTER, 110, "ADD BOTS", UI_CENTER | UI_SMALLFONT, color_black);

	// standard menu drawing
	Menu_Draw(&addBotsMenuInfo.menu);
}

/*
#######################

  AddBotsMenu_Key

#######################
*/
static sfxHandle_t AddBotsMenu_Key(int key) {
	switch (key) {
	case K_MWHEELUP:
	case K_PGUP:
		UI_AddBotsMenu_UpEvent(&addBotsMenuInfo.arrowup, QM_ACTIVATED);
		break;
	case K_MWHEELDOWN:
	case K_PGDN:
		UI_AddBotsMenu_DownEvent(&addBotsMenuInfo.arrowdown, QM_ACTIVATED);
		break;

	case 'a':
	case 'A':
		UI_AddBotsMenu_FightEvent(&addBotsMenuInfo.go, QM_ACTIVATED);
		break;
	}

	return Menu_DefaultKey(&addBotsMenuInfo.menu, key);
}

/*
=================
UI_AddBotsMenu_Init
=================
*/
static const char *skillNames[] = {"Kindergarten", "Flower Power", "Mosquito Bite",
								   "Peacemaker", "Brutal Blue Noses", NULL};

static const char *teamNames1[] = {"Default", "Red", "Blue", NULL};

static const char *teamNames2[] = {"Red", "Blue", NULL};

static void UI_AddBotsMenu_Init(void) {
	int n;
	int y;
	int gametype;
	int count;
	char info[MAX_INFO_STRING];

	trap_GetConfigString(CS_SERVERINFO, info, MAX_INFO_STRING);
	gametype = atoi(Info_ValueForKey(info, "g_gametype"));

	memset(&addBotsMenuInfo, 0, sizeof(addBotsMenuInfo));
	addBotsMenuInfo.menu.draw = UI_AddBotsMenu_Draw;
	addBotsMenuInfo.menu.fullscreen = qfalse;
	addBotsMenuInfo.menu.wrapAround = qtrue;
	addBotsMenuInfo.delay = 1000;
	addBotsMenuInfo.menu.key = AddBotsMenu_Key;

	UI_AddBots_Cache();

	addBotsMenuInfo.numBots = UI_GetNumBots();
	count = addBotsMenuInfo.numBots < NUM_BOTS ? addBotsMenuInfo.numBots : NUM_BOTS;

	addBotsMenuInfo.arrowup.generic.type = MTYPE_BITMAP;
	addBotsMenuInfo.arrowup.generic.name = ARROWUP0;
	addBotsMenuInfo.arrowup.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	addBotsMenuInfo.arrowup.generic.callback = UI_AddBotsMenu_UpEvent;
	addBotsMenuInfo.arrowup.generic.id = ID_SCROLL_UP;
	addBotsMenuInfo.arrowup.generic.x = SCREEN_CENTER + 74;
	addBotsMenuInfo.arrowup.generic.y = 142;
	addBotsMenuInfo.arrowup.width = 22;
	addBotsMenuInfo.arrowup.height = 50;
	addBotsMenuInfo.arrowup.focuspic = ARROWUP1;
	addBotsMenuInfo.arrowup.focuspicinstead = qtrue;

	addBotsMenuInfo.arrowdown.generic.type = MTYPE_BITMAP;
	addBotsMenuInfo.arrowdown.generic.name = ARROWDN0;
	addBotsMenuInfo.arrowdown.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	addBotsMenuInfo.arrowdown.generic.callback = UI_AddBotsMenu_DownEvent;
	addBotsMenuInfo.arrowdown.generic.id = ID_SCROLL_DOWN;
	addBotsMenuInfo.arrowdown.generic.x = SCREEN_CENTER + 74;
	addBotsMenuInfo.arrowdown.generic.y = 208;
	addBotsMenuInfo.arrowdown.width = 22;
	addBotsMenuInfo.arrowdown.height = 50;
	addBotsMenuInfo.arrowdown.focuspic = ARROWDN1;
	addBotsMenuInfo.arrowdown.focuspicinstead = qtrue;

	for (n = 0, y = 140; n < count; n++, y += 20) {
		addBotsMenuInfo.bot_pics[n].generic.type = MTYPE_BITMAP;
		addBotsMenuInfo.bot_pics[n].generic.name = addBotsMenuInfo.boticonnames[n];
		addBotsMenuInfo.bot_pics[n].generic.flags = QMF_INACTIVE;
		addBotsMenuInfo.bot_pics[n].generic.x = SCREEN_CENTER - 90;
		addBotsMenuInfo.bot_pics[n].generic.y = y;
		addBotsMenuInfo.bot_pics[n].width = 20;
		addBotsMenuInfo.bot_pics[n].height = 20;

		addBotsMenuInfo.bots[n].generic.type = MTYPE_TEXTS;
		addBotsMenuInfo.bots[n].fontHeight = 20.0f;
		addBotsMenuInfo.bots[n].generic.flags = QMF_LEFT_JUSTIFY;
		addBotsMenuInfo.bots[n].generic.id = ID_BOTNAME0 + n;
		addBotsMenuInfo.bots[n].generic.x = SCREEN_CENTER - 70;
		addBotsMenuInfo.bots[n].generic.y = y;
		addBotsMenuInfo.bots[n].generic.callback = UI_AddBotsMenu_BotEvent;
		addBotsMenuInfo.bots[n].string = addBotsMenuInfo.botnames[n];
		addBotsMenuInfo.bots[n].color = colorBlack;
		addBotsMenuInfo.bots[n].focuscolor = colorDkLilac;
		addBotsMenuInfo.bots[n].style = UI_LEFT | UI_SMALLFONT;
	}

	y = 270;
	addBotsMenuInfo.skill.generic.type = MTYPE_SPINCONTROL;
	addBotsMenuInfo.skill.generic.flags = QMF_SMALLFONT | QMF_INGAMESTYLE;
	addBotsMenuInfo.skill.generic.x = SCREEN_CENTER - 40;
	addBotsMenuInfo.skill.generic.y = y;
	addBotsMenuInfo.skill.generic.name = "Skill:";
	addBotsMenuInfo.skill.generic.id = ID_SKILL;
	addBotsMenuInfo.skill.itemnames = skillNames;
	addBotsMenuInfo.skill.curvalue = Com_Clamp(0, 4, (int)trap_Cvar_VariableValue("g_spSkill") - 1);

	y += SMALLCHAR_HEIGHT;
	addBotsMenuInfo.team.generic.type = MTYPE_SPINCONTROL;
	addBotsMenuInfo.team.generic.flags = QMF_SMALLFONT | QMF_INGAMESTYLE;
	addBotsMenuInfo.team.generic.x = SCREEN_CENTER - 40;
	addBotsMenuInfo.team.generic.y = y;
	addBotsMenuInfo.team.generic.id = ID_TEAM;

	if (gametype >= GT_TEAM) {
		addBotsMenuInfo.team.itemnames = teamNames2;
		addBotsMenuInfo.team.generic.name = "Team: ";
	} else {
		addBotsMenuInfo.team.itemnames = teamNames1;
		addBotsMenuInfo.team.generic.name = "Color:";
	}

	addBotsMenuInfo.go.generic.type = MTYPE_TEXTS;
	addBotsMenuInfo.go.fontHeight = 24.0f;
	addBotsMenuInfo.go.generic.flags = QMF_CENTER_JUSTIFY;
	addBotsMenuInfo.go.generic.callback = UI_AddBotsMenu_FightEvent;
	addBotsMenuInfo.go.generic.id = ID_GO;
	addBotsMenuInfo.go.generic.x = SCREEN_CENTER;
	addBotsMenuInfo.go.generic.y = 315;
	addBotsMenuInfo.go.string = "ADD";
	addBotsMenuInfo.go.style = (UI_CENTER | UI_SMALLFONT);
	addBotsMenuInfo.go.color = color_black;
	addBotsMenuInfo.go.focuscolor = color_orange;

	addBotsMenuInfo.back.generic.type = MTYPE_BITMAP;
	addBotsMenuInfo.back.generic.name = BACK0;
	addBotsMenuInfo.back.generic.flags = QMF_PULSEIFFOCUS;
	addBotsMenuInfo.back.generic.x = SCREEN_CENTER - 95;
	addBotsMenuInfo.back.generic.y = 340;
	addBotsMenuInfo.back.generic.id = ID_BACK;
	addBotsMenuInfo.back.generic.callback = UI_AddBotsMenu_BackEvent;
	addBotsMenuInfo.back.width = 50;
	addBotsMenuInfo.back.height = 25;
	addBotsMenuInfo.back.focuspic = BACK1;
	addBotsMenuInfo.back.focuspicinstead = qtrue;

	addBotsMenuInfo.baseBotNum = 0;
	addBotsMenuInfo.selectedBotNum = 0;
	addBotsMenuInfo.bots[0].color = colorDkOrange;
	addBotsMenuInfo.bots[0].focuscolor = color_orange;

	UI_AddBotsMenu_GetSortedBotNums();
	UI_AddBotsMenu_SetBotNames();
	Menu_AddItem(&addBotsMenuInfo.menu, &addBotsMenuInfo.arrowup);
	Menu_AddItem(&addBotsMenuInfo.menu, &addBotsMenuInfo.arrowdown);
	for (n = 0; n < count; n++) {
		Menu_AddItem(&addBotsMenuInfo.menu, &addBotsMenuInfo.bots[n]);
		Menu_AddItem(&addBotsMenuInfo.menu, &addBotsMenuInfo.bot_pics[n]);
	}
	Menu_AddItem(&addBotsMenuInfo.menu, &addBotsMenuInfo.skill);
	Menu_AddItem(&addBotsMenuInfo.menu, &addBotsMenuInfo.team);
	Menu_AddItem(&addBotsMenuInfo.menu, &addBotsMenuInfo.go);
	Menu_AddItem(&addBotsMenuInfo.menu, &addBotsMenuInfo.back);
}

/*
=================
UI_AddBots_Cache
=================
*/
void UI_AddBots_Cache(void) {
	trap_R_RegisterShaderNoMip(ARROWUP0);
	trap_R_RegisterShaderNoMip(ARROWUP1);
	trap_R_RegisterShaderNoMip(ARROWDN0);
	trap_R_RegisterShaderNoMip(ARROWDN1);
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
}

/*
=================
UI_AddBotsMenu
=================
*/
void UI_AddBotsMenu(void) {
	UI_AddBotsMenu_Init();
	UI_PushMenu(&addBotsMenuInfo.menu);
}
