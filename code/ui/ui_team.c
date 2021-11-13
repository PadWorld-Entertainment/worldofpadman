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
//
// ui_team.c
//

#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"

#define ID_BACK 10
#define ID_JOINRED 11
#define ID_JOINBLUE 12
#define ID_JOINGAME 13
#define ID_SPECTATE 14

typedef struct {
	menuframework_s menu;
	menutext_s joinred;
	menutext_s joinblue;
	menutext_s joingame;
	menutext_s spectate;
	menubitmap_s back;
} teammain_t;

static teammain_t s_teammain;

/*
===============
TeamMain_MenuEvent
===============
*/
static void TeamMain_MenuEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_JOINRED:
		trap_Cmd_ExecuteText(EXEC_APPEND, "cmd team red\n");
		UI_ForceMenuOff();
		break;

	case ID_JOINBLUE:
		trap_Cmd_ExecuteText(EXEC_APPEND, "cmd team blue\n");
		UI_ForceMenuOff();
		break;

	case ID_JOINGAME:
		trap_Cmd_ExecuteText(EXEC_APPEND, "cmd team free\n");
		UI_ForceMenuOff();
		break;

	case ID_SPECTATE:
		trap_Cmd_ExecuteText(EXEC_APPEND, "cmd team spectator\n");
		UI_ForceMenuOff();
		break;

	case ID_BACK:
		if (event != QM_ACTIVATED)
			break;
		UI_PopMenu();
		break;
	}
}

static sfxHandle_t TeamMain_Key(int key) {
	switch (key) {
	case 'f':
	case 'j':
	case 'a':
		TeamMain_MenuEvent(&s_teammain.joingame, QM_ACTIVATED);
		break;

	case 's':
		TeamMain_MenuEvent(&s_teammain.spectate, QM_ACTIVATED);
		break;

	case 'r':
		if (!(s_teammain.joinred.generic.flags & QMF_GRAYED)) {
			TeamMain_MenuEvent(&s_teammain.joinred, QM_ACTIVATED);
		}
		break;
	case 'b':
		if (!(s_teammain.joinblue.generic.flags & QMF_GRAYED)) {
			TeamMain_MenuEvent(&s_teammain.joinred, QM_ACTIVATED);
		}
		break;

	default:
		break;
	}

	return Menu_DefaultKey(&s_teammain.menu, key);
}

static void TeamMain_Draw(void) {
	UI_DrawIngameBG();
	UI_DrawProportionalString(320, 110, "TEAM", (UI_CENTER | UI_SMALLFONT), color_black);

	Menu_Draw(&s_teammain.menu);
}

/*
===============
TeamMain_MenuInit
===============
*/
void TeamMain_MenuInit(void) {
	int y;
	int gametype;
	char info[MAX_INFO_STRING];

	memset(&s_teammain, 0, sizeof(s_teammain));

	TeamMain_Cache();

	s_teammain.menu.wrapAround = qtrue;
	s_teammain.menu.fullscreen = qfalse;
	s_teammain.menu.draw = TeamMain_Draw;
	s_teammain.menu.key = TeamMain_Key;

	y = 180;
	s_teammain.joinred.generic.type = MTYPE_TEXTS;
	s_teammain.joinred.fontHeight = 20.0f;
	s_teammain.joinred.generic.flags = QMF_CENTER_JUSTIFY;
	s_teammain.joinred.generic.id = ID_JOINRED;
	s_teammain.joinred.generic.callback = TeamMain_MenuEvent;
	s_teammain.joinred.generic.x = 320;
	s_teammain.joinred.generic.y = y;
	s_teammain.joinred.string = "RED PADS";
	s_teammain.joinred.style = (UI_CENTER | UI_SMALLFONT);
	s_teammain.joinred.color = colorDkRed;
	s_teammain.joinred.focuscolor = colorRed;

	y += 20;
	s_teammain.joinblue.generic.type = MTYPE_TEXTS;
	s_teammain.joinblue.fontHeight = 20.0f;
	s_teammain.joinblue.generic.flags = QMF_CENTER_JUSTIFY;
	s_teammain.joinblue.generic.id = ID_JOINBLUE;
	s_teammain.joinblue.generic.callback = TeamMain_MenuEvent;
	s_teammain.joinblue.generic.x = 320;
	s_teammain.joinblue.generic.y = y;
	s_teammain.joinblue.string = "BLUE NOSES";
	s_teammain.joinblue.style = (UI_CENTER | UI_SMALLFONT);
	s_teammain.joinblue.color = colorDkBlue;
	s_teammain.joinblue.focuscolor = colorBlue;

	y += 30;
	s_teammain.joingame.generic.type = MTYPE_TEXTS;
	s_teammain.joingame.fontHeight = 20.0f;
	s_teammain.joingame.generic.flags = QMF_CENTER_JUSTIFY;
	s_teammain.joingame.generic.id = ID_JOINGAME;
	s_teammain.joingame.generic.callback = TeamMain_MenuEvent;
	s_teammain.joingame.generic.x = 320;
	s_teammain.joingame.generic.y = y;
	s_teammain.joingame.string = "JOIN GAME";
	s_teammain.joingame.style = (UI_CENTER | UI_SMALLFONT);
	s_teammain.joingame.color = colorDkOrange;
	s_teammain.joingame.focuscolor = colorYellow;

	y += 40;
	s_teammain.spectate.generic.type = MTYPE_TEXTS;
	s_teammain.spectate.fontHeight = 20.0f;
	s_teammain.spectate.generic.flags = QMF_CENTER_JUSTIFY;
	s_teammain.spectate.generic.id = ID_SPECTATE;
	s_teammain.spectate.generic.callback = TeamMain_MenuEvent;
	s_teammain.spectate.generic.x = 320;
	s_teammain.spectate.generic.y = y;
	s_teammain.spectate.string = "SPECTATE";
	s_teammain.spectate.style = (UI_CENTER | UI_SMALLFONT);
	s_teammain.spectate.color = colorBlack;
	s_teammain.spectate.focuscolor = colorDkLilac;

	s_teammain.back.generic.type = MTYPE_BITMAP;
	s_teammain.back.generic.name = BACK0;
	s_teammain.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_teammain.back.generic.x = 225;
	s_teammain.back.generic.y = 340;
	s_teammain.back.generic.id = ID_BACK;
	s_teammain.back.generic.callback = TeamMain_MenuEvent;
	s_teammain.back.width = 50;
	s_teammain.back.height = 25;
	s_teammain.back.focuspic = BACK1;
	s_teammain.back.focuspicinstead = qtrue;

	trap_GetConfigString(CS_SERVERINFO, info, sizeof(info));
	gametype = atoi(Info_ValueForKey(info, "g_gametype"));

	// set initial states
	if (gametype < GT_TEAM) {
		s_teammain.joinred.generic.flags |= QMF_GRAYED;
		s_teammain.joinblue.generic.flags |= QMF_GRAYED;
	} else {
		// s_teammain.joingame.generic.flags |= QMF_GRAYED;
		// "team f" works in team gametypes, game will auto select
		s_teammain.joingame.string = "AUTO TEAM";
	}

	Menu_AddItem(&s_teammain.menu, (void *)&s_teammain.joinred);
	Menu_AddItem(&s_teammain.menu, (void *)&s_teammain.joinblue);
	Menu_AddItem(&s_teammain.menu, (void *)&s_teammain.joingame);
	Menu_AddItem(&s_teammain.menu, (void *)&s_teammain.spectate);
	Menu_AddItem(&s_teammain.menu, (void *)&s_teammain.back);
}

/*
===============
TeamMain_Cache
===============
*/
void TeamMain_Cache(void) {
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
}

/*
===============
UI_TeamMainMenu
===============
*/
void UI_TeamMainMenu(void) {
	TeamMain_MenuInit();
	UI_PushMenu(&s_teammain.menu);
}
