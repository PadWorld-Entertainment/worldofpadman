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

MAIN MENU

=======================================================================
*/

#include "ui_local.h"

#define ID_CREATE 10
#define ID_JOIN 11
#define ID_SETUP 12
#define ID_DEMOS 13
#define ID_MODS 14
#define ID_CREDITS 15
#define ID_EXIT 16
#define ID_MANIA 17
#define ID_SECRET 18

#define CREATE0 "menu/buttons/create0"
#define CREATE1 "menu/buttons/create1"
#define JOIN0 "menu/buttons/join0"
#define JOIN1 "menu/buttons/join1"
#define SETUP0 "menu/buttons/setup0"
#define SETUP1 "menu/buttons/setup1"
#define DEMOS0 "menu/buttons/demos0"
#define DEMOS1 "menu/buttons/demos1"
#define MODS0 "menu/buttons/mods0"
#define MODS1 "menu/buttons/mods1"
#define CREDITS0 "menu/buttons/credits0"
#define CREDITS1 "menu/buttons/credits1"
#define EXIT0 "menu/buttons/exit0"
#define EXIT1 "menu/buttons/exit1"
#define MANIA0 "menu/buttons/mania0"
#define MANIA1 "menu/buttons/mania1"

#define MAX_MSGLENGHT 256

typedef struct {
	menuframework_s menu;

	menubitmap_s create;
	menubitmap_s join;
	menubitmap_s setup;
	menubitmap_s demos;
	menubitmap_s mods;
	menubitmap_s credits;
	menubitmap_s exit;
	menubitmap_s mania;
	menubitmap_s secret;

	char msg[MAX_MSGLENGHT];
	int msgtime;
} mainmenu_t;

static mainmenu_t s_main;

typedef struct {
	menuframework_s menu;
	char errorMessage[4096];
} errorMessage_t;

static errorMessage_t s_errorMessage;

/*
=================
Main_MenuEvent
=================
*/
static void Main_MenuEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_CREATE:
		UI_StartServerMenu(qtrue);
		break;

	case ID_JOIN:
		UI_ArenaServersMenu();
		break;

	case ID_SETUP:
		UI_SetupMenu();
		break;

	case ID_DEMOS:
		UI_DemosMenu();
		break;

	case ID_MODS:
		UI_ModsMenu();
		break;

	case ID_CREDITS:
		UI_WopCreditsMenu();
		break;

	case ID_MANIA:
		UI_ManiaMenu();
		break;

	case ID_SECRET:
		UI_SecretMenu();
		break;

	case ID_EXIT:
		UI_CreditMenu();
		break;
	}
}

/*
===============
UI_MainMenu_Cache
===============
*/
void UI_MainMenu_Cache(void) {
	trap_R_RegisterShaderNoMip(CREATE0);
	trap_R_RegisterShaderNoMip(CREATE1);
	trap_R_RegisterShaderNoMip(JOIN0);
	trap_R_RegisterShaderNoMip(JOIN1);
	trap_R_RegisterShaderNoMip(SETUP0);
	trap_R_RegisterShaderNoMip(SETUP1);
	trap_R_RegisterShaderNoMip(DEMOS0);
	trap_R_RegisterShaderNoMip(DEMOS1);
	trap_R_RegisterShaderNoMip(MODS0);
	trap_R_RegisterShaderNoMip(MODS1);
	trap_R_RegisterShaderNoMip(CREDITS0);
	trap_R_RegisterShaderNoMip(CREDITS1);
	trap_R_RegisterShaderNoMip(EXIT0);
	trap_R_RegisterShaderNoMip(EXIT1);
	trap_R_RegisterShaderNoMip(MANIA0);
	trap_R_RegisterShaderNoMip(MANIA1);
}

static sfxHandle_t ErrorMessage_Key(int key) {
	trap_Cvar_Set("com_errorMessage", "");
	UI_MainMenu();
	return (menuNullSound);
}

/*
===============
Main_MenuDraw
TTimo: this function is common to the main menu and errorMessage menu
===============
*/
int CINhandle = -1;
static void Main_MenuDraw(void) {
	int msglen;
	vec4_t tblack = {0.0f, 0.0f, 0.0f, 0.66f};
	float size;

	msglen = strlen(s_main.msg);

	if (s_main.msgtime != 0 && uis.realtime - s_main.msgtime <= 5000) {
		if (uis.realtime - s_main.msgtime > 4750) {
			size = 16.0f * ((float)(5000 - (uis.realtime - s_main.msgtime)) / 250.0f);
		} else if (uis.realtime - s_main.msgtime > 250) {
			size = 16.0f;
		} else {
			size = 16.0f * ((float)((uis.realtime - s_main.msgtime)) / 250.0f);
		}

		UI_FillRect(320.0f - (size * (msglen + 4.0f) * 0.25f), 240.0f - size, (msglen + 4.0f) * size * 0.5f,
					2.0f * size, tblack);
		UI_DrawRect(320.0f - (size * (msglen + 4.0f) * 0.25f), 240.0f - size, (msglen + 4.0f) * size * 0.5f,
					2.0f * size, colorBlack, 1);
		UI_DrawStringNS(320, 240 - size / 2, s_main.msg, UI_CENTER, size, colorWhite);
	}

	if (strlen(s_errorMessage.errorMessage))
		UI_DrawString_AutoWrapped(SCREEN_WIDTH / 2, 192, 600, 20, s_errorMessage.errorMessage,
								  UI_CENTER | UI_SMALLFONT | UI_DROPSHADOW, menu_text_color, qtrue);
	else
		Menu_Draw(&s_main.menu);

	if (CINhandle >= 0) {
		trap_CIN_RunCinematic(CINhandle);
		trap_CIN_DrawCinematic(CINhandle);
	}
}

static sfxHandle_t Main_MenuKey(int key) {
	if (key == K_ESCAPE || key == K_MOUSE2)
		return 0;

	return Menu_DefaultKey(&s_main.menu, key);
}

/*
===============
UI_MainMenu

The main menu only comes up when not in a game,
so make sure that the attract loop server is down
and that local cinematics are killed
===============
*/
void UI_MainMenu(void) {
	trap_Cvar_Set("sv_killserver", "1");
	trap_Cvar_Set("mapname", ""); // note: I just recognized that this isn't reset on server shutdown ... but for some
								  // optimations in wopSP-code it is better to have this on a correct value

	memset(&s_main, 0, sizeof(mainmenu_t));
	memset(&s_errorMessage, 0, sizeof(errorMessage_t));

	// com_errorMessage would need that too
	UI_MainMenu_Cache();

	trap_Cvar_VariableStringBuffer("com_errorMessage", s_errorMessage.errorMessage,
								   sizeof(s_errorMessage.errorMessage));
	if (strlen(s_errorMessage.errorMessage)) {
		s_errorMessage.menu.draw = Main_MenuDraw;
		s_errorMessage.menu.key = ErrorMessage_Key;
		s_errorMessage.menu.fullscreen = qtrue;
		s_errorMessage.menu.wrapAround = qtrue;

		trap_Key_SetCatcher(KEYCATCH_UI);
		uis.menusp = 0;
		UI_PushMenu(&s_errorMessage.menu);

		return;
	}

	s_main.menu.draw = Main_MenuDraw;
	s_main.menu.key = Main_MenuKey;
	s_main.menu.fullscreen = qtrue;
	s_main.menu.wrapAround = qtrue;
	s_main.menu.bgparts = BGP_MAIN | BGP_MAINFX;
	s_main.menu.noPushSelect = qtrue;

	s_main.create.generic.type = MTYPE_BITMAP;
	s_main.create.generic.name = CREATE0;
	s_main.create.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_main.create.generic.x = 714;
	s_main.create.generic.y = 130;
	s_main.create.generic.id = ID_CREATE;
	s_main.create.generic.callback = Main_MenuEvent;
	s_main.create.width = 120;
	s_main.create.height = 40;
	s_main.create.focuspic = CREATE1;
	s_main.create.focuspicinstead = qtrue;

	s_main.join.generic.type = MTYPE_BITMAP;
	s_main.join.generic.name = JOIN0;
	s_main.join.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_main.join.generic.x = 734;
	s_main.join.generic.y = 166;
	s_main.join.generic.id = ID_JOIN;
	s_main.join.generic.callback = Main_MenuEvent;
	s_main.join.width = 80;
	s_main.join.height = 40;
	s_main.join.focuspic = JOIN1;
	s_main.join.focuspicinstead = qtrue;

	s_main.setup.generic.type = MTYPE_BITMAP;
	s_main.setup.generic.name = SETUP0;
	s_main.setup.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_main.setup.generic.x = 708;
	s_main.setup.generic.y = 202;
	s_main.setup.generic.id = ID_SETUP;
	s_main.setup.generic.callback = Main_MenuEvent;
	s_main.setup.width = 120;
	s_main.setup.height = 40;
	s_main.setup.focuspic = SETUP1;
	s_main.setup.focuspicinstead = qtrue;

	s_main.demos.generic.type = MTYPE_BITMAP;
	s_main.demos.generic.name = DEMOS0;
	s_main.demos.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_main.demos.generic.x = 720;
	s_main.demos.generic.y = 238;
	s_main.demos.generic.id = ID_DEMOS;
	s_main.demos.generic.callback = Main_MenuEvent;
	s_main.demos.width = 120;
	s_main.demos.height = 40;
	s_main.demos.focuspic = DEMOS1;
	s_main.demos.focuspicinstead = qtrue;

	s_main.mods.generic.type = MTYPE_BITMAP;
	s_main.mods.generic.name = MODS0;
	s_main.mods.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_main.mods.generic.x = 736;
	s_main.mods.generic.y = 274;
	s_main.mods.generic.id = ID_MODS;
	s_main.mods.generic.callback = Main_MenuEvent;
	s_main.mods.width = 80;
	s_main.mods.height = 40;
	s_main.mods.focuspic = MODS1;
	s_main.mods.focuspicinstead = qtrue;

	s_main.credits.generic.type = MTYPE_BITMAP;
	s_main.credits.generic.name = CREDITS0;
	s_main.credits.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_main.credits.generic.x = 712;
	s_main.credits.generic.y = 310;
	s_main.credits.generic.id = ID_CREDITS;
	s_main.credits.generic.callback = Main_MenuEvent;
	s_main.credits.width = 120;
	s_main.credits.height = 40;
	s_main.credits.focuspic = CREDITS1;
	s_main.credits.focuspicinstead = qtrue;

	s_main.exit.generic.type = MTYPE_BITMAP;
	s_main.exit.generic.name = EXIT0;
	s_main.exit.generic.flags = QMF_PULSEIFFOCUS;
	s_main.exit.generic.x = 738;
	s_main.exit.generic.y = 346;
	s_main.exit.generic.id = ID_EXIT;
	s_main.exit.generic.callback = Main_MenuEvent;
	s_main.exit.width = 80;
	s_main.exit.height = 40;
	s_main.exit.focuspic = EXIT1;
	s_main.exit.focuspicinstead = qtrue;

	s_main.mania.generic.type = MTYPE_BITMAP;
	s_main.mania.generic.name = MANIA0;
	s_main.mania.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_main.mania.generic.x = 0;
	s_main.mania.generic.y = 300;
	s_main.mania.generic.id = ID_MANIA;
	s_main.mania.generic.callback = Main_MenuEvent;
	s_main.mania.width = 200;
	s_main.mania.height = 186;
	s_main.mania.focuspic = MANIA1;
	s_main.mania.focuspicinstead = qtrue;

	s_main.secret.generic.type = MTYPE_BITMAP;
	s_main.secret.generic.flags = QMF_MOUSEONLY | QMF_SILENT | QMF_HIDDEN;
	s_main.secret.generic.x = 302;
	s_main.secret.generic.y = 316;
	s_main.secret.generic.id = ID_SECRET;
	s_main.secret.generic.callback = Main_MenuEvent;
	s_main.secret.width = 40;
	s_main.secret.height = 40;

	Menu_AddItem(&s_main.menu, &s_main.create);
	Menu_AddItem(&s_main.menu, &s_main.join);
	Menu_AddItem(&s_main.menu, &s_main.setup);
	Menu_AddItem(&s_main.menu, &s_main.demos);
	Menu_AddItem(&s_main.menu, &s_main.mods);
	Menu_AddItem(&s_main.menu, &s_main.credits);
	Menu_AddItem(&s_main.menu, &s_main.exit);
	Menu_AddItem(&s_main.menu, &s_main.mania);
	Menu_AddItem(&s_main.menu, &s_main.secret);

	trap_Key_SetCatcher(KEYCATCH_UI);
	uis.menusp = 0;
	UI_PushMenu(&s_main.menu);
}
