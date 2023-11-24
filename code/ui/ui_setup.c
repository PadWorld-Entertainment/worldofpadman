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

SETUP MENU

=======================================================================
*/

#include "ui_local.h"

#define ID_CUSTOMIZEPLAYER 10
#define ID_CUSTOMIZECONTROLS 11
#define ID_SYSTEMCONFIG 12
#define ID_GAME 13
#define ID_DEFAULTS 14
#define ID_BACK 15

#define PLAYER0 "menu/buttons/player0"
#define PLAYER1 "menu/buttons/player1"
#define CONTROLS0 "menu/buttons/controls0"
#define CONTROLS1 "menu/buttons/controls1"
#define SYSTEM0 "menu/buttons/system0"
#define SYSTEM1 "menu/buttons/system1"
#define OPTIONS0 "menu/buttons/options0"
#define OPTIONS1 "menu/buttons/options1"
#define DEFAULTS0 "menu/buttons/defaults0"
#define DEFAULTS1 "menu/buttons/defaults1"
#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"

typedef struct {
	menuframework_s menu;
	menubitmap_s player;
	menubitmap_s controls;
	menubitmap_s system;
	menubitmap_s options;
	menubitmap_s defaults;
	menubitmap_s back;
} setupMenuInfo_t;

static setupMenuInfo_t setupMenuInfo;

/*
####################### ####################### ####################### ####################### #######################

  new defaults-menu

####################### ####################### ####################### ####################### #######################
*/

#define ID_DEFAULT_YES 10
#define ID_DEFAULT_NO 11
#define ID_DEFAULT_BACK 12

#define YES0 "menu/buttons/yes0"
#define YES1 "menu/buttons/yes1"
#define NO0 "menu/buttons/no0"
#define NO1 "menu/buttons/no1"

typedef struct {
	menuframework_s menu;
	menubitmap_s yes;
	menubitmap_s no;
	menubitmap_s back;
} setupDefaultMenu_t;

static setupDefaultMenu_t setupDefaultMenu;

/*
#######################
SetupDefaultMenu_Event
#######################
*/
static void SetupDefaultMenu_Event(void *ptr, int event) {
	if (event != QM_ACTIVATED)
		return;

	switch (((menucommon_s *)ptr)->id) {
	case ID_DEFAULT_YES:
		trap_Cmd_ExecuteText(EXEC_APPEND, "exec default.cfg\n");
		trap_Cmd_ExecuteText(EXEC_APPEND, "cvar_restart\n");
		trap_Cmd_ExecuteText(EXEC_APPEND, "vid_restart\n");
		break;
	case ID_DEFAULT_NO:
	case ID_DEFAULT_BACK:
		UI_PopMenu();
		break;
	}
}

static void SetupDefaultMenu_Draw(void) {
	static const vec4_t color_setdefaults = {0.8f, 0.85f, 1.0f, 1.0f};	
	UI_DrawProportionalString(2 * (SCREEN_WIDTH / 3) + 5, 256, "SET TO DEFAULTS?", (UI_CENTER | UI_SMALLFONT), color_setdefaults);
	UI_DrawStringNS(2 * (SCREEN_WIDTH / 3) + 20, 380, "This will reset *^1ALL^3* options", UI_CENTER, 20, color_yellow);
	UI_DrawStringNS(2 * (SCREEN_WIDTH / 3), 380 + 22, "to their default values.", UI_CENTER, 20, color_yellow);
	Menu_Draw(&setupDefaultMenu.menu);
}

/*
#######################
SetupDefaultMenu_Key
#######################
*/
static sfxHandle_t SetupDefaultMenu_Key(int key) {
	switch (key) {

	case 'n':
	case 'N':
		SetupDefaultMenu_Event(&setupDefaultMenu.no, QM_ACTIVATED);
		break;

	case 'y':
	case 'Y':
		SetupDefaultMenu_Event(&setupDefaultMenu.yes, QM_ACTIVATED);
		break;
	}

	return Menu_DefaultKey(&setupDefaultMenu.menu, key);
}

/*
#######################
SetupDefaultMenu_Init
#######################
*/
static void SetupDefaultMenu_Init(void) {
	UI_SetupDefaultMenu_Cache();

	memset(&setupDefaultMenu, 0, sizeof(setupDefaultMenu));
	setupDefaultMenu.menu.key = SetupDefaultMenu_Key;
	setupDefaultMenu.menu.draw = SetupDefaultMenu_Draw;
	setupDefaultMenu.menu.wrapAround = qtrue;
	setupDefaultMenu.menu.fullscreen = qtrue;
	setupDefaultMenu.menu.bgparts = BGP_DEFAULTS | BGP_MENUFX;

	setupDefaultMenu.yes.generic.type = MTYPE_BITMAP;
	setupDefaultMenu.yes.generic.name = YES0;
	setupDefaultMenu.yes.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	setupDefaultMenu.yes.generic.x = 496;
	setupDefaultMenu.yes.generic.y = 288;
	setupDefaultMenu.yes.generic.id = ID_DEFAULT_YES;
	setupDefaultMenu.yes.generic.callback = SetupDefaultMenu_Event;
	setupDefaultMenu.yes.width = 80;
	setupDefaultMenu.yes.height = 40;
	setupDefaultMenu.yes.focuspic = YES1;
	setupDefaultMenu.yes.focuspicinstead = qtrue;

	setupDefaultMenu.no.generic.type = MTYPE_BITMAP;
	setupDefaultMenu.no.generic.name = NO0;
	setupDefaultMenu.no.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	setupDefaultMenu.no.generic.x = 616;
	setupDefaultMenu.no.generic.y = 288;
	setupDefaultMenu.no.generic.id = ID_DEFAULT_NO;
	setupDefaultMenu.no.generic.callback = SetupDefaultMenu_Event;
	setupDefaultMenu.no.width = 40;
	setupDefaultMenu.no.height = 40;
	setupDefaultMenu.no.focuspic = NO1;
	setupDefaultMenu.no.focuspicinstead = qtrue;

	setupDefaultMenu.back.generic.type = MTYPE_BITMAP;
	setupDefaultMenu.back.generic.name = BACK0;
	setupDefaultMenu.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	setupDefaultMenu.back.generic.x = 8;
	setupDefaultMenu.back.generic.y = 446;
	setupDefaultMenu.back.generic.id = ID_DEFAULT_BACK;
	setupDefaultMenu.back.generic.callback = SetupDefaultMenu_Event;
	setupDefaultMenu.back.width = 80;
	setupDefaultMenu.back.height = 40;
	setupDefaultMenu.back.focuspic = BACK1;
	setupDefaultMenu.back.focuspicinstead = qtrue;

	Menu_AddItem(&setupDefaultMenu.menu, &setupDefaultMenu.yes);
	Menu_AddItem(&setupDefaultMenu.menu, &setupDefaultMenu.no);
	Menu_AddItem(&setupDefaultMenu.menu, &setupDefaultMenu.back);

	UI_PushMenu(&setupDefaultMenu.menu);
}

/*
#######################
UI_SetupDefaultMenu_Cache
#######################
*/
void UI_SetupDefaultMenu_Cache(void) {
	trap_R_RegisterShaderNoMip(YES0);
	trap_R_RegisterShaderNoMip(YES1);
	trap_R_RegisterShaderNoMip(NO0);
	trap_R_RegisterShaderNoMip(NO1);
}

/*
===============
UI_SetupMenu_Event
===============
*/
static void UI_SetupMenu_Event(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_CUSTOMIZEPLAYER:
		UI_PlayerSettingsMenu();
		break;

	case ID_CUSTOMIZECONTROLS:
		UI_ControlsMenu();
		break;

	case ID_SYSTEMCONFIG:
		UI_GraphicsOptionsMenu();
		break;

	case ID_GAME:
		UI_PreferencesMenu();
		break;

	case ID_DEFAULTS:
		SetupDefaultMenu_Init();
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}

/*
===============
UI_SetupMenu_Init
===============
*/
static void UI_SetupMenu_Init(void) {
	UI_SetupMenu_Cache();

	memset(&setupMenuInfo, 0, sizeof(setupMenuInfo));
	setupMenuInfo.menu.wrapAround = qtrue;
	setupMenuInfo.menu.fullscreen = qtrue;

	setupMenuInfo.menu.noPushSelect = qtrue;
	setupMenuInfo.menu.bgparts = BGP_SETUP | BGP_MENUFX;

	setupMenuInfo.player.generic.type = MTYPE_BITMAP;
	setupMenuInfo.player.generic.name = PLAYER0;
	setupMenuInfo.player.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	setupMenuInfo.player.generic.x = 92;
	setupMenuInfo.player.generic.y = 46;
	setupMenuInfo.player.generic.id = ID_CUSTOMIZEPLAYER;
	setupMenuInfo.player.generic.callback = UI_SetupMenu_Event;
	setupMenuInfo.player.width = 120;
	setupMenuInfo.player.height = 40;
	setupMenuInfo.player.focuspic = PLAYER1;
	setupMenuInfo.player.focuspicinstead = qtrue;
	Menu_AddItem(&setupMenuInfo.menu, &setupMenuInfo.player);

	setupMenuInfo.controls.generic.type = MTYPE_BITMAP;
	setupMenuInfo.controls.generic.name = CONTROLS0;
	setupMenuInfo.controls.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	setupMenuInfo.controls.generic.x = 80;
	setupMenuInfo.controls.generic.y = 87;
	setupMenuInfo.controls.generic.id = ID_CUSTOMIZECONTROLS;
	setupMenuInfo.controls.generic.callback = UI_SetupMenu_Event;
	setupMenuInfo.controls.width = 160;
	setupMenuInfo.controls.height = 40;
	setupMenuInfo.controls.focuspic = CONTROLS1;
	setupMenuInfo.controls.focuspicinstead = qtrue;
	Menu_AddItem(&setupMenuInfo.menu, &setupMenuInfo.controls);

	setupMenuInfo.system.generic.type = MTYPE_BITMAP;
	setupMenuInfo.system.generic.name = SYSTEM0;
	setupMenuInfo.system.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	setupMenuInfo.system.generic.x = 109;
	setupMenuInfo.system.generic.y = 127;
	setupMenuInfo.system.generic.id = ID_SYSTEMCONFIG;
	setupMenuInfo.system.generic.callback = UI_SetupMenu_Event;
	setupMenuInfo.system.width = 120;
	setupMenuInfo.system.height = 40;
	setupMenuInfo.system.focuspic = SYSTEM1;
	setupMenuInfo.system.focuspicinstead = qtrue;
	Menu_AddItem(&setupMenuInfo.menu, &setupMenuInfo.system);

	setupMenuInfo.options.generic.type = MTYPE_BITMAP;
	setupMenuInfo.options.generic.name = OPTIONS0;
	setupMenuInfo.options.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	setupMenuInfo.options.generic.x = 91;
	setupMenuInfo.options.generic.y = 167;
	setupMenuInfo.options.generic.id = ID_GAME;
	setupMenuInfo.options.generic.callback = UI_SetupMenu_Event;
	setupMenuInfo.options.width = 120;
	setupMenuInfo.options.height = 40;
	setupMenuInfo.options.focuspic = OPTIONS1;
	setupMenuInfo.options.focuspicinstead = qtrue;
	Menu_AddItem(&setupMenuInfo.menu, &setupMenuInfo.options);

	if (!trap_Cvar_VariableValue("cl_paused")) {
		setupMenuInfo.defaults.generic.type = MTYPE_BITMAP;
		setupMenuInfo.defaults.generic.name = DEFAULTS0;
		setupMenuInfo.defaults.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
		setupMenuInfo.defaults.generic.x = 91;
		setupMenuInfo.defaults.generic.y = 207;
		setupMenuInfo.defaults.generic.id = ID_DEFAULTS;
		setupMenuInfo.defaults.generic.callback = UI_SetupMenu_Event;
		setupMenuInfo.defaults.width = 160;
		setupMenuInfo.defaults.height = 40;
		setupMenuInfo.defaults.focuspic = DEFAULTS1;
		setupMenuInfo.defaults.focuspicinstead = qtrue;
		Menu_AddItem(&setupMenuInfo.menu, &setupMenuInfo.defaults);
	}

	setupMenuInfo.back.generic.type = MTYPE_BITMAP;
	setupMenuInfo.back.generic.name = BACK0;
	setupMenuInfo.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	setupMenuInfo.back.generic.x = 8;
	setupMenuInfo.back.generic.y = 446;
	setupMenuInfo.back.generic.id = ID_BACK;
	setupMenuInfo.back.generic.callback = UI_SetupMenu_Event;
	setupMenuInfo.back.width = 80;
	setupMenuInfo.back.height = 40;
	setupMenuInfo.back.focuspic = BACK1;
	setupMenuInfo.back.focuspicinstead = qtrue;
	Menu_AddItem(&setupMenuInfo.menu, &setupMenuInfo.back);
}

/*
=================
UI_SetupMenu_Cache
=================
*/
void UI_SetupMenu_Cache(void) {
	trap_R_RegisterShaderNoMip(PLAYER0);
	trap_R_RegisterShaderNoMip(PLAYER1);
	trap_R_RegisterShaderNoMip(CONTROLS0);
	trap_R_RegisterShaderNoMip(CONTROLS1);
	trap_R_RegisterShaderNoMip(SYSTEM0);
	trap_R_RegisterShaderNoMip(SYSTEM1);
	trap_R_RegisterShaderNoMip(DEFAULTS0);
	trap_R_RegisterShaderNoMip(DEFAULTS1);
	trap_R_RegisterShaderNoMip(OPTIONS0);
	trap_R_RegisterShaderNoMip(OPTIONS1);
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
}

/*
===============
UI_SetupMenu
===============
*/
void UI_SetupMenu(void) {
	UI_SetupMenu_Init();
	UI_PushMenu(&setupMenuInfo.menu);
}
