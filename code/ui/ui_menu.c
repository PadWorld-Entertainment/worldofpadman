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
#define ID_EXIT 15
#define ID_CREDITS 16
#define ID_SECRET 17

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
#define EXIT0 "menu/buttons/exit0"
#define EXIT1 "menu/buttons/exit1"
#define CREDITS0 "menu/main/ButtonCredits0"
#define CREDITS1 "menu/main/ButtonCredits1"

#define MAX_MSGLENGHT 256

typedef struct {
	menuframework_s menu;

	menubitmap_s Create;
	menubitmap_s Join;
	menubitmap_s Setup;
	menubitmap_s Demos;
	menubitmap_s Mods;
	menubitmap_s Exit;
	menubitmap_s Credits;
	menubitmap_s Secret;

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
void Main_MenuEvent(void *ptr, int event) {
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
		UI_InitBigCredits();
		break;

	case ID_SECRET:
		UI_SecretMenu();
		break;

	case ID_EXIT:
		ExitMenu_Init();
		break;
	}
}

/*
===============
MainMenu_Cache
===============
*/
void MainMenu_Cache(void) {
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
	trap_R_RegisterShaderNoMip(EXIT0);
	trap_R_RegisterShaderNoMip(EXIT1);
	trap_R_RegisterShaderNoMip(CREDITS0);
	trap_R_RegisterShaderNoMip(CREDITS1);
}

sfxHandle_t ErrorMessage_Key(int key) {
	trap_Cvar_Set("com_errorMessage", "");
	UI_MainMenu();
	return (menu_null_sound);
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
		UI_DrawString_AutoWrapped(320, 192, 600, 20, s_errorMessage.errorMessage,
								  UI_CENTER | UI_SMALLFONT | UI_DROPSHADOW, menu_text_color, qtrue);
	else
		Menu_Draw(&s_main.menu);

	if (CINhandle >= 0) {
		trap_CIN_RunCinematic(CINhandle);
		trap_CIN_DrawCinematic(CINhandle);
	}
}

sfxHandle_t Main_MenuKey(int key) {
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

	if (UI_GetCvarInt("ui_mainmenuCinTest")) {
		char buffer[32];
		trap_Cvar_VariableStringBuffer("ui_mainmenuCinTestFile", buffer, sizeof(buffer));
		if (!buffer[0])
			strcpy(buffer, "idlogo.roq");
		if (CINhandle >= 0)
			trap_CIN_StopCinematic(CINhandle);
		//		CINhandle = trap_CIN_PlayCinematic("idlogo.roq", 10, 10, 160, 120, /*int bits*/ CIN_loop);
		CINhandle = trap_CIN_PlayCinematic(buffer, 10, 10, 160, 120, /*int bits*/ CIN_loop);
		trap_S_StopBackgroundTrack();
	}

	memset(&s_main, 0, sizeof(mainmenu_t));
	memset(&s_errorMessage, 0, sizeof(errorMessage_t));

	// com_errorMessage would need that too
	MainMenu_Cache();

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
	s_main.menu.bgparts = BGP_MAINBG | BGP_MAINFRAME;
	s_main.menu.noPushSelect = qtrue;

	s_main.Create.generic.type = MTYPE_BITMAP;
	s_main.Create.generic.name = CREATE0;
	s_main.Create.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_main.Create.generic.x = 502;
	s_main.Create.generic.y = 140;
	s_main.Create.generic.id = ID_CREATE;
	s_main.Create.generic.callback = Main_MenuEvent;
	s_main.Create.width = 120;
	s_main.Create.height = 40;
	s_main.Create.focuspic = CREATE1;
	s_main.Create.focuspicinstead = qtrue;
	Menu_AddItem(&s_main.menu, &s_main.Create);

	s_main.Join.generic.type = MTYPE_BITMAP;
	s_main.Join.generic.name = JOIN0;
	s_main.Join.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_main.Join.generic.x = 518;
	s_main.Join.generic.y = 175;
	s_main.Join.generic.id = ID_JOIN;
	s_main.Join.generic.callback = Main_MenuEvent;
	s_main.Join.width = 80;
	s_main.Join.height = 40;
	s_main.Join.focuspic = JOIN1;
	s_main.Join.focuspicinstead = qtrue;
	Menu_AddItem(&s_main.menu, &s_main.Join);

	s_main.Setup.generic.type = MTYPE_BITMAP;
	s_main.Setup.generic.name = SETUP0;
	s_main.Setup.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_main.Setup.generic.x = 496;
	s_main.Setup.generic.y = 210;
	s_main.Setup.generic.id = ID_SETUP;
	s_main.Setup.generic.callback = Main_MenuEvent;
	s_main.Setup.width = 120;
	s_main.Setup.height = 40;
	s_main.Setup.focuspic = SETUP1;
	s_main.Setup.focuspicinstead = qtrue;
	Menu_AddItem(&s_main.menu, &s_main.Setup);

	s_main.Demos.generic.type = MTYPE_BITMAP;
	s_main.Demos.generic.name = DEMOS0;
	s_main.Demos.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_main.Demos.generic.x = 510;
	s_main.Demos.generic.y = 245;
	s_main.Demos.generic.id = ID_DEMOS;
	s_main.Demos.generic.callback = Main_MenuEvent;
	s_main.Demos.width = 120;
	s_main.Demos.height = 40;
	s_main.Demos.focuspic = DEMOS1;
	s_main.Demos.focuspicinstead = qtrue;
	Menu_AddItem(&s_main.menu, &s_main.Demos);

	s_main.Mods.generic.type = MTYPE_BITMAP;
	s_main.Mods.generic.name = MODS0;
	s_main.Mods.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_main.Mods.generic.x = 516;
	s_main.Mods.generic.y = 280;
	s_main.Mods.generic.id = ID_MODS;
	s_main.Mods.generic.callback = Main_MenuEvent;
	s_main.Mods.width = 80;
	s_main.Mods.height = 40;
	s_main.Mods.focuspic = MODS1;
	s_main.Mods.focuspicinstead = qtrue;
	Menu_AddItem(&s_main.menu, &s_main.Mods);

	s_main.Exit.generic.type = MTYPE_BITMAP;
	s_main.Exit.generic.name = EXIT0;
	s_main.Exit.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_main.Exit.generic.x = 530;
	s_main.Exit.generic.y = 315;
	s_main.Exit.generic.id = ID_EXIT;
	s_main.Exit.generic.callback = Main_MenuEvent;
	s_main.Exit.width = 80;
	s_main.Exit.height = 40;
	s_main.Exit.focuspic = EXIT1;
	s_main.Exit.focuspicinstead = qtrue;
	Menu_AddItem(&s_main.menu, &s_main.Exit);

	s_main.Credits.generic.type = MTYPE_BITMAP;
	s_main.Credits.generic.name = CREDITS0;
	s_main.Credits.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_main.Credits.generic.x = 505;
	s_main.Credits.generic.y = 420;
	s_main.Credits.generic.id = ID_CREDITS;
	s_main.Credits.generic.callback = Main_MenuEvent;
	s_main.Credits.width = 120;
	s_main.Credits.height = 40;
	s_main.Credits.focuspic = CREDITS1;
	s_main.Credits.focuspicinstead = qtrue;
	Menu_AddItem(&s_main.menu, &s_main.Credits);

	s_main.Secret.generic.type = MTYPE_BITMAP;
	s_main.Secret.generic.flags = QMF_MOUSEONLY | QMF_SILENT | QMF_HIDDEN;
	s_main.Secret.generic.x = 129;
	s_main.Secret.generic.y = 431;
	s_main.Secret.generic.id = ID_SECRET;
	s_main.Secret.generic.callback = Main_MenuEvent;
	s_main.Secret.width = 22;
	s_main.Secret.height = 22;
	Menu_AddItem(&s_main.menu, &s_main.Secret);

	trap_Key_SetCatcher(KEYCATCH_UI);
	uis.menusp = 0;
	UI_PushMenu(&s_main.menu);
}
