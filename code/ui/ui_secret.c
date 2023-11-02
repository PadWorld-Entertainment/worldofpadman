/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of WorldOfPadman source code.

WorldOfPadman source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

WorldOfPadman source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with WorldOfPadman source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
/*
=======================================================================

SECRET MENU (ENTE'S ARTWORKS)

=======================================================================
*/
#include "ui_local.h"

#define BGPIC "menu/bg/connecting"
#define BARROWLT0 "menu/arrows/bigblu_lt0"
#define BARROWLT1 "menu/arrows/bigblu_lt1"
#define BARROWRT0 "menu/arrows/bigblu_rt0"
#define BARROWRT1 "menu/arrows/bigblu_rt1"
#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"

#define ID_BACK 10
#define ID_NEXT 11
#define ID_PREV 12

#define MAX_SECRETPAGES 12

const char *secretpics[] = {"menu/secret/001", "menu/secret/002", "menu/secret/003", "menu/secret/004",
							"menu/secret/005", "menu/secret/006", "menu/secret/007", "menu/secret/008",
							"menu/secret/009", "menu/secret/010", "menu/secret/011", "menu/secret/012",
							NULL};

typedef struct {
	menuframework_s menu;

	menubitmap_s secpic;
	menubitmap_s next;
	menubitmap_s prev;
	menubitmap_s back;

	int currentpage;

} secret_t;

static secret_t s_secret;

/*
===============
UI_Secret_MenuUpdate
===============
*/
static void UI_Secret_MenuUpdate(void) {

	s_secret.secpic.generic.name = secretpics[s_secret.currentpage];
	s_secret.secpic.shader = 0;

	s_secret.next.generic.flags |= QMF_GRAYED;
	s_secret.prev.generic.flags |= QMF_GRAYED;

	if (s_secret.currentpage < (MAX_SECRETPAGES - 1)) {
		s_secret.next.generic.flags &= ~QMF_GRAYED;
	}
		
	if (s_secret.currentpage > 0) {
		s_secret.prev.generic.flags &= ~QMF_GRAYED;
	}

}

/*
===============
UI_Secret_MenuEvent
===============
*/
static void UI_Secret_MenuEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_BACK:
		UI_StopMusic(); // -> durch refresh sollte automatisch wieder der normale starten
		UI_PopMenu();
		break;

	case ID_NEXT:
		if (s_secret.currentpage < (MAX_SECRETPAGES - 1)) {
			s_secret.currentpage++;
			UI_Secret_MenuUpdate();
		}
		break;

	case ID_PREV:
		if (s_secret.currentpage > 0) {
			s_secret.currentpage--;
			UI_Secret_MenuUpdate();
		}
		break;
	}
}

/*
=================
UI_Secret_MenuDraw
=================
*/
static void UI_Secret_MenuDraw(void) {

	UI_DrawNamedPic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BGPIC);
	UI_FillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, colorTBlack33);
	UI_DrawStringNS(SCREEN_WIDTH - 14 * SMALLCHAR_WIDTH, SCREEN_HEIGHT - SMALLCHAR_HEIGHT, 
					"(c) ENTE 2024", UI_LEFT, SMALLCHAR_HEIGHT, color_white);
	Menu_Draw(&s_secret.menu);
}

/*
=================
UI_Secret_MenuKey
=================
*/
static sfxHandle_t UI_Secret_MenuKey(int key) {

	if (key == K_PGDN || key == K_RIGHTARROW) {
		UI_Secret_MenuEvent(&s_secret.next, QM_ACTIVATED);
	}
	if (key == K_PGUP || key == K_LEFTARROW) {
		UI_Secret_MenuEvent(&s_secret.prev, QM_ACTIVATED);
	}
	if (key == K_MOUSE2 || key == K_ESCAPE) {
		UI_StopMusic(); // -> durch refresh sollte automatisch wieder der normale starten
	}

	return Menu_DefaultKey(&s_secret.menu, key);
}

/*
===============
UI_Secret_MenuInit
===============
*/
static void UI_Secret_MenuInit(void) {
	UI_Secret_Cache();

	memset(&s_secret, 0, sizeof(secret_t));

	UI_StartCreditMusic();

	s_secret.menu.key = UI_Secret_MenuKey;
	s_secret.menu.draw = UI_Secret_MenuDraw;
	s_secret.menu.wrapAround = qtrue;
	s_secret.menu.fullscreen = qtrue;

	s_secret.secpic.generic.type = MTYPE_BITMAP;
	s_secret.secpic.generic.name = secretpics[0];
	s_secret.secpic.generic.x = 108;
	s_secret.secpic.generic.y = 0;
	s_secret.secpic.width = 648;
	s_secret.secpic.height = SCREEN_HEIGHT;

	s_secret.prev.generic.type = MTYPE_BITMAP;
	s_secret.prev.generic.name = BARROWLT0;
	s_secret.prev.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_secret.prev.generic.x = 24;
	s_secret.prev.generic.y = 173;
	s_secret.prev.generic.id = ID_PREV;
	s_secret.prev.generic.callback = UI_Secret_MenuEvent;
	s_secret.prev.width = 60;
	s_secret.prev.height = 140;
	s_secret.prev.focuspic = BARROWLT1;
	s_secret.prev.focuspicinstead = qtrue;

	s_secret.next.generic.type = MTYPE_BITMAP;
	s_secret.next.generic.name = BARROWRT0;
	s_secret.next.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_secret.next.generic.x = 780;
	s_secret.next.generic.y = 173;
	s_secret.next.generic.id = ID_NEXT;
	s_secret.next.generic.callback = UI_Secret_MenuEvent;
	s_secret.next.width = 60;
	s_secret.next.height = 140;
	s_secret.next.focuspic = BARROWRT1;
	s_secret.next.focuspicinstead = qtrue;

	s_secret.back.generic.type = MTYPE_BITMAP;
	s_secret.back.generic.name = BACK0;
	s_secret.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_secret.back.generic.x = 8;
	s_secret.back.generic.y = 446;
	s_secret.back.generic.id = ID_BACK;
	s_secret.back.generic.callback = UI_Secret_MenuEvent;
	s_secret.back.width = 80;
	s_secret.back.height = 40;
	s_secret.back.focuspic = BACK1;
	s_secret.back.focuspicinstead = qtrue;

	Menu_AddItem(&s_secret.menu, &s_secret.secpic);
	Menu_AddItem(&s_secret.menu, &s_secret.next);
	Menu_AddItem(&s_secret.menu, &s_secret.prev);
	Menu_AddItem(&s_secret.menu, &s_secret.back);

	UI_Secret_MenuUpdate();

}

/*
=================
UI_Secret_Cache
=================
*/
void UI_Secret_Cache(void) {
	int i;

	trap_R_RegisterShaderNoMip(BGPIC);
	for (i = 0; i < MAX_SECRETPAGES; i++) {
		trap_R_RegisterShaderNoMip(secretpics[i]);
	}
	trap_R_RegisterShaderNoMip(BARROWLT0);
	trap_R_RegisterShaderNoMip(BARROWLT1);
	trap_R_RegisterShaderNoMip(BARROWRT0);
	trap_R_RegisterShaderNoMip(BARROWRT1);
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
}

/*
===============
UI_SecretMenu
===============
*/
void UI_SecretMenu(void) {
	UI_Secret_MenuInit();
	UI_PushMenu(&s_secret.menu);
}
