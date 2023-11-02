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

CREDITS (Short credits when leaving World of PADMAN)

=======================================================================
*/

#include "ui_local.h"

#define CREDITSPIC "menu/bg/credits"

typedef struct {
	menuframework_s menu;

} credits_t;

static credits_t s_credits;

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

	UI_DrawNamedPic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CREDITSPIC);
}

/*
===============
UI_CreditMenu
===============
*/
void UI_CreditMenu(void) {
	memset(&s_credits, 0, sizeof(s_credits));

	UI_Credit_Cache();

	s_credits.menu.draw = UI_CreditMenu_Draw;
	s_credits.menu.key = UI_CreditMenu_Key;
	s_credits.menu.fullscreen = qtrue;
	UI_PushMenu(&s_credits.menu);
}

void UI_Credit_Cache(void) {
	trap_R_RegisterShaderNoMip(CREDITSPIC);
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

static sfxHandle_t UI_CheckMenuExitKeys(int key) {
	if (key == K_MOUSE2 || key == K_ESCAPE) {
		UI_StopMusic(); // -> durch refresh sollte automatisch wieder der normale starten
	}
	return Menu_DefaultKey(uis.activemenu, key);
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
	s_bigcredits.bgpic.width = 864;
	s_bigcredits.bgpic.height = 486;
	Menu_AddItem(&s_bigcredits.menu, &s_bigcredits.bgpic);

	UI_PushMenu(&s_bigcredits.menu);
}
