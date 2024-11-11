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

/*
===============
UI_Credit_Cache
===============
*/
void UI_Credit_Cache(void) {
	trap_R_RegisterShaderNoMip(CREDITSPIC);
}

/*
=======================================================================

WOP CREDITS (Slideshow credits for World of PADMAN)

=======================================================================
*/

#define ARROWLT0 "menu/arrows/arryel_lt0b"
#define ARROWLT1 "menu/arrows/arryel_lt1"
#define ARROWRT0 "menu/arrows/arryel_rt0b"
#define ARROWRT1 "menu/arrows/arryel_rt1"
#define EXIT0 "menu/buttons/exit0"
#define EXIT1 "menu/buttons/exit1"

#define ID_EXIT 10
#define ID_NEXT 11
#define ID_PREV 12

const char *creditspics[] = {"menu/credits/001", "menu/credits/002", "menu/credits/003",
							"menu/credits/004", "menu/credits/005", "menu/credits/006",
							"menu/credits/007", "menu/credits/008", NULL};

#define MAX_CREDITSPAGES STRARRAY_LEN(creditspics)

typedef struct {
	menuframework_s menu;

	menubitmap_s creditspic;
	menubitmap_s next;
	menubitmap_s prev;
	menubitmap_s exit;

	int currentpage;

} wopcredits_t;

static wopcredits_t s_wopcredits;

/*
===============
UI_WopCredits_MenuUpdate
===============
*/
static void UI_WopCredits_MenuUpdate(void) {

	s_wopcredits.creditspic.generic.name = creditspics[s_wopcredits.currentpage];
	s_wopcredits.creditspic.shader = 0;

	s_wopcredits.next.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;
	s_wopcredits.prev.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;

	if (s_wopcredits.currentpage < (MAX_CREDITSPAGES - 1)) {
		s_wopcredits.next.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
	}
		
	if (s_wopcredits.currentpage > 0) {
		s_wopcredits.prev.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
	}

}

/*
===============
UI_WopCredits_MenuEvent
===============
*/
static void UI_WopCredits_MenuEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_EXIT:
		UI_StopMusic(); // -> durch refresh sollte automatisch wieder der normale starten
		UI_PopMenu();
		break;

	case ID_NEXT:
		if (s_wopcredits.currentpage < (MAX_CREDITSPAGES - 1)) {
			s_wopcredits.currentpage++;
			UI_WopCredits_MenuUpdate();
		}
		break;

	case ID_PREV:
		if (s_wopcredits.currentpage > 0) {
			s_wopcredits.currentpage--;
			UI_WopCredits_MenuUpdate();
		}
		break;
	}
}

/*
===============
UI_WopCredits_MenuKey
===============
*/
static sfxHandle_t UI_WopCredits_MenuKey(int key) {

	if (key == K_PGDN || key == K_RIGHTARROW) {
		UI_WopCredits_MenuEvent(&s_wopcredits.next, QM_ACTIVATED);
	}
	if (key == K_PGUP || key == K_LEFTARROW) {
		UI_WopCredits_MenuEvent(&s_wopcredits.prev, QM_ACTIVATED);
	}
	if (key == K_MOUSE2 || key == K_ESCAPE) {
		UI_StopMusic(); // -> durch refresh sollte automatisch wieder der normale starten
	}

	return Menu_DefaultKey(&s_wopcredits.menu, key);
}

/*
===============
UI_WopCredits_MenuInit
===============
*/
static void UI_WopCredits_MenuInit(void) {
	memset(&s_wopcredits, 0, sizeof(s_wopcredits));

	UI_WopCredits_Cache();
	UI_StartCreditMusic();

	s_wopcredits.menu.key = UI_WopCredits_MenuKey;
	s_wopcredits.menu.wrapAround = qtrue;
	s_wopcredits.menu.fullscreen = qtrue;

	s_wopcredits.creditspic.generic.type = MTYPE_BITMAP;
	s_wopcredits.creditspic.generic.name = creditspics[0];
	s_wopcredits.creditspic.generic.x = 0;
	s_wopcredits.creditspic.generic.y = 0;
	s_wopcredits.creditspic.width = SCREEN_WIDTH;
	s_wopcredits.creditspic.height = SCREEN_HEIGHT;

	s_wopcredits.prev.generic.type = MTYPE_BITMAP;
	s_wopcredits.prev.generic.name = ARROWLT0;
	s_wopcredits.prev.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_wopcredits.prev.generic.x = 364;
	s_wopcredits.prev.generic.y = 446;
	s_wopcredits.prev.generic.id = ID_PREV;
	s_wopcredits.prev.generic.callback = UI_WopCredits_MenuEvent;
	s_wopcredits.prev.width = 60;
	s_wopcredits.prev.height = 26;
	s_wopcredits.prev.focuspic = ARROWLT1;
	s_wopcredits.prev.focuspicinstead = qtrue;

	s_wopcredits.next.generic.type = MTYPE_BITMAP;
	s_wopcredits.next.generic.name = ARROWRT0;
	s_wopcredits.next.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_wopcredits.next.generic.x = 440;
	s_wopcredits.next.generic.y = 446;
	s_wopcredits.next.generic.id = ID_NEXT;
	s_wopcredits.next.generic.callback = UI_WopCredits_MenuEvent;
	s_wopcredits.next.width = 60;
	s_wopcredits.next.height = 26;
	s_wopcredits.next.focuspic = ARROWRT1;
	s_wopcredits.next.focuspicinstead = qtrue;

	s_wopcredits.exit.generic.type = MTYPE_BITMAP;
	s_wopcredits.exit.generic.name = EXIT0;
	s_wopcredits.exit.generic.flags = QMF_PULSEIFFOCUS;
	s_wopcredits.exit.generic.x = 8;
	s_wopcredits.exit.generic.y = 446;
	s_wopcredits.exit.generic.id = ID_EXIT;
	s_wopcredits.exit.generic.callback = UI_WopCredits_MenuEvent;
	s_wopcredits.exit.width = 80;
	s_wopcredits.exit.height = 40;
	s_wopcredits.exit.focuspic = EXIT1;
	s_wopcredits.exit.focuspicinstead = qtrue;

	Menu_AddItem(&s_wopcredits.menu, &s_wopcredits.creditspic);
	Menu_AddItem(&s_wopcredits.menu, &s_wopcredits.next);
	Menu_AddItem(&s_wopcredits.menu, &s_wopcredits.prev);
	Menu_AddItem(&s_wopcredits.menu, &s_wopcredits.exit);

	UI_WopCredits_MenuUpdate();

}

/*
===============
UI_WopCredits_Cache
===============
*/
void UI_WopCredits_Cache(void) {
	int i;

	for (i = 0; i < MAX_CREDITSPAGES; i++) {
		trap_R_RegisterShaderNoMip(creditspics[i]);
	}
	trap_R_RegisterShaderNoMip(ARROWLT0);
	trap_R_RegisterShaderNoMip(ARROWLT1);
	trap_R_RegisterShaderNoMip(ARROWRT0);
	trap_R_RegisterShaderNoMip(ARROWRT1);
	trap_R_RegisterShaderNoMip(EXIT0);
	trap_R_RegisterShaderNoMip(EXIT1);
}

/*
===============
UI_WopCreditsMenu
===============
*/
void UI_WopCreditsMenu(void) {
	UI_WopCredits_MenuInit();
	UI_PushMenu(&s_wopcredits.menu);
}
