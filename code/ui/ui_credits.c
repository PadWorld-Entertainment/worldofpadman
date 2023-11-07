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

CREDITS
... the Q3-like credits menu (shown before leaving the programm)

=======================================================================
*/

#include "ui_local.h"

typedef struct {
	menuframework_s menu;
} creditsmenu_t;

static creditsmenu_t s_credits;

/*
=================
UI_CreditMenu_Key
=================
*/
static sfxHandle_t UI_CreditMenu_Key(int key) {
	if (key & K_CHAR_FLAG) {
		return 0;
	}

	trap_Cmd_ExecuteText(EXEC_INSERT, "quit\n");
	return 0;
}

/*
===============
UI_CreditMenu_Draw
===============
*/
static void UI_CreditMenu_Draw(void) {

	UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, trap_R_RegisterShaderNoMip("menu/bg/credits"));
}

/*
===============
UI_CreditMenu
===============
*/
void UI_CreditMenu(void) {
	/* This UI_FillRect() hack will blank the borders if you're in widescreen,
	   so you get a completely black background instead of stripes from the
	   previous frame on each side of the credits.. */
	const float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	UI_FillRect(0 - uis.xbias, 0, (640.0f / uis.xscale) * 2.0f, 480.0f / uis.yscale, black);

	memset(&s_credits, 0, sizeof(s_credits));

	UI_Credit_Cache();

	s_credits.menu.draw = UI_CreditMenu_Draw;
	s_credits.menu.key = UI_CreditMenu_Key;
	s_credits.menu.fullscreen = qtrue;
	UI_PushMenu(&s_credits.menu);
}

void UI_Credit_Cache(void) {
	trap_R_RegisterShaderNoMip("menu/bg/credits");
}

static sfxHandle_t UI_CheckMenuExitKeys(int key) {
	if (key == K_MOUSE2 || key == K_ESCAPE) {
		UI_StopMusic(); // -> durch refresh sollte automatisch wieder der normale starten
	}
	return Menu_DefaultKey(uis.activemenu, key);
}

/*
####################### ####################### ####################### ####################### #######################

  the real wop-credits-menu

####################### ####################### ####################### ####################### #######################
*/

const char *creditbgs[] = {"menu/credits/cred001", "menu/credits/cred002", "menu/credits/cred003",
						   "menu/credits/cred004", "menu/credits/cred005", "menu/credits/cred006",
						   "menu/credits/cred007", "menu/credits/cred008", 0};

#define MAX_CREDITPAGES 8

typedef struct {
	menuframework_s menu;

	menubitmap_s bgpic;

	int currentpage;
} bigcreditsmenu_t;

static bigcreditsmenu_t s_bigcredits;

void UI_BigCredits_Cache(void) {
	int i;

	for (i = 0; i < MAX_CREDITPAGES; i++)
		trap_R_RegisterShaderNoMip(creditbgs[i]);
}

static void UI_BigCreditsAction(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	s_bigcredits.currentpage++;
	if (s_bigcredits.currentpage >= MAX_CREDITPAGES)
		s_bigcredits.currentpage = 0;

	s_bigcredits.bgpic.generic.name = creditbgs[s_bigcredits.currentpage];
	s_bigcredits.bgpic.shader = 0;

	trap_S_StartLocalSound(menuSwitchSound, CHAN_LOCAL_SOUND);
}

void UI_InitBigCredits(void) {
	memset(&s_bigcredits, 0, sizeof(s_bigcredits));
	UI_StartCreditMusic();

	UI_BigCredits_Cache();

	s_bigcredits.menu.fullscreen = qtrue;
	s_bigcredits.menu.key = UI_CheckMenuExitKeys;

	s_bigcredits.bgpic.generic.type = MTYPE_BITMAP;
	s_bigcredits.bgpic.generic.name = creditbgs[0];
	s_bigcredits.bgpic.generic.flags = QMF_LEFT_JUSTIFY | QMF_SILENT;
	s_bigcredits.bgpic.generic.callback = UI_BigCreditsAction;
	s_bigcredits.bgpic.generic.x = 0;
	s_bigcredits.bgpic.generic.y = 0;
	s_bigcredits.bgpic.width = 640;
	s_bigcredits.bgpic.height = 480;
	Menu_AddItem(&s_bigcredits.menu, &s_bigcredits.bgpic);

	UI_PushMenu(&s_bigcredits.menu);
}

/*
####################### ####################### ####################### ####################### #######################

  padmod-secret-menu

####################### ####################### ####################### ####################### #######################
*/

typedef struct {
	menuframework_s menu;

	menubitmap_s bgpic;

	char shadername[512];
	int currentpage;
	qboolean codeSecret;
} secretmenu_t;

static secretmenu_t s_secretmenu;

static void UI_SetSecretShaderStr(void) {
	int offset;
	strcpy(s_secretmenu.shadername, "menu/secret/sec");
	offset = strlen(s_secretmenu.shadername);

TRYAGAIN_SECSHADER:
	s_secretmenu.shadername[offset] = '0' + ((s_secretmenu.currentpage + 1) / 10) % 10;
	s_secretmenu.shadername[offset + 1] = '0' + (s_secretmenu.currentpage + 1) % 10;
	s_secretmenu.shadername[offset + 2] = '\0';

	if (trap_R_RegisterShaderNoMip(s_secretmenu.shadername))
		return;

	if (s_secretmenu.currentpage) {
		s_secretmenu.currentpage = 0;
		goto TRYAGAIN_SECSHADER;
	} else {
		s_secretmenu.codeSecret = qtrue;
		s_secretmenu.shadername[offset] = '.';
		s_secretmenu.shadername[offset + 1] = '.';
	}
}

static void UI_SecretAction(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	s_secretmenu.currentpage++;
	UI_SetSecretShaderStr();

	s_secretmenu.bgpic.shader = 0;

	trap_S_StartLocalSound(menuSwitchSound, CHAN_LOCAL_SOUND);
}

static void Secret_MenuDraw(void) {
	if (s_secretmenu.codeSecret) {
		vec4_t color1 = {0.0f, 0.8f, 0.0f, 0.4f};
		vec4_t color4 = {0.0f, 0.5f, 0.5f, 0.3f};
		// (float)-cast against double -> float/int warning
		UI_DrawStringNS(320 - 320.0f * (float)sin(uis.realtime / 600.0f + 2),
						240 - 240.0f * (float)sin(uis.realtime / 1200.0f - 1), "*cry*", UI_CENTER, 20, color1);
		UI_DrawStringNS(320 - 320.0f * (float)sin(uis.realtime / 800.0f + 1),
						240 - 240.0f * (float)sin(uis.realtime / 1000.0f + 3), "=(", UI_CENTER, 20, colorLtGrey);
		UI_DrawStringNS(320 - 320.0f * (float)sin(uis.realtime / 900.0f - 2),
						240 - 240.0f * (float)sin(uis.realtime / 800.0f - 2), "-.-", UI_CENTER, 20, colorDkGrey);
		UI_DrawStringNS(320 - 320.0f * (float)sin(uis.realtime / 700.0f + 3),
						240 - 240.0f * (float)sin(uis.realtime / 1400.0f - 4), "f*ck", UI_CENTER, 20, color4);

		UI_DrawStringNS(320, 240 - 20, "Cannot finde Secret-Textures", UI_CENTER, 20, colorWhite);
		UI_DrawStringNS(320, 240, s_secretmenu.shadername, UI_CENTER, 15, colorWhite);
	} else
		Menu_Draw(&s_secretmenu.menu);
}

void UI_SecretMenu(void) {
	memset(&s_secretmenu, 0, sizeof(s_secretmenu));

	UI_StartCreditMusic();

	UI_SetSecretShaderStr();

	s_secretmenu.menu.fullscreen = qtrue;
	s_secretmenu.menu.draw = Secret_MenuDraw;
	s_secretmenu.menu.key = UI_CheckMenuExitKeys;

	s_secretmenu.bgpic.generic.type = MTYPE_BITMAP;
	s_secretmenu.bgpic.generic.name = s_secretmenu.shadername;
	s_secretmenu.bgpic.generic.flags = QMF_LEFT_JUSTIFY | QMF_SILENT;
	s_secretmenu.bgpic.generic.callback = UI_SecretAction;
	s_secretmenu.bgpic.generic.x = 0;
	s_secretmenu.bgpic.generic.y = 0;
	s_secretmenu.bgpic.width = 640;
	s_secretmenu.bgpic.height = 480;
	Menu_AddItem(&s_secretmenu.menu, &s_secretmenu.bgpic);

	UI_PushMenu(&s_secretmenu.menu);
}
