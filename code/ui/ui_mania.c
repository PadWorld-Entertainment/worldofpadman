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

PADMANIA MENU (FAN ARTWORKS etc.)

=======================================================================
*/
#include "ui_local.h"

#define ARROWLT0 "menu/arrows/arryel_lt0b"
#define ARROWLT1 "menu/arrows/arryel_lt1"
#define ARROWRT0 "menu/arrows/arryel_rt0b"
#define ARROWRT1 "menu/arrows/arryel_rt1"
#define EXIT0 "menu/buttons/exit0"
#define EXIT1 "menu/buttons/exit1"

#define ID_EXIT 10
#define ID_NEXT 11
#define ID_PREV 12

#define MAX_MANIAPAGES 10

const char *maniapics[] = {"menu/mania/001", "menu/mania/002", "menu/mania/003",
							"menu/mania/004", "menu/mania/005", "menu/mania/006",
							"menu/mania/007", "menu/mania/008", "menu/mania/009",
							"menu/mania/010", NULL};

typedef struct {
	menuframework_s menu;

	menubitmap_s maniapic;
	menubitmap_s next;
	menubitmap_s prev;
	menubitmap_s exit;

	int currentpage;

} mania_t;

static mania_t s_mania;

/*
===============
UI_Mania_MenuUpdate
===============
*/
static void UI_Mania_MenuUpdate(void) {

	s_mania.maniapic.generic.name = maniapics[s_mania.currentpage];
	s_mania.maniapic.shader = 0;

	s_mania.next.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;
	s_mania.prev.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;

	if (s_mania.currentpage < (MAX_MANIAPAGES - 1)) {
		s_mania.next.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
	}
		
	if (s_mania.currentpage > 0) {
		s_mania.prev.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
	}

}

/*
===============
UI_Mania_MenuEvent
===============
*/
static void UI_Mania_MenuEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_EXIT:
		UI_StopMusic(); // -> durch refresh sollte automatisch wieder der normale starten
		UI_PopMenu();
		break;

	case ID_NEXT:
		if (s_mania.currentpage < (MAX_MANIAPAGES - 1)) {
			s_mania.currentpage++;
			UI_Mania_MenuUpdate();
		}
		break;

	case ID_PREV:
		if (s_mania.currentpage > 0) {
			s_mania.currentpage--;
			UI_Mania_MenuUpdate();
		}
		break;
	}
}

/*
=================
UI_Mania_MenuKey
=================
*/
static sfxHandle_t UI_Mania_MenuKey(int key) {

	if (key == K_PGDN || key == K_RIGHTARROW) {
		UI_Mania_MenuEvent(&s_mania.next, QM_ACTIVATED);
	}
	if (key == K_PGUP || key == K_LEFTARROW) {
		UI_Mania_MenuEvent(&s_mania.prev, QM_ACTIVATED);
	}
	if (key == K_MOUSE2 || key == K_ESCAPE) {
		UI_StopMusic(); // -> durch refresh sollte automatisch wieder der normale starten
	}

	return Menu_DefaultKey(&s_mania.menu, key);
}

/*
===============
UI_Mania_MenuInit
===============
*/
static void UI_Mania_MenuInit(void) {
	UI_Mania_Cache();

	memset(&s_mania, 0, sizeof(mania_t));

	UI_StartCreditMusic();

	s_mania.menu.key = UI_Mania_MenuKey;
	s_mania.menu.wrapAround = qtrue;
	s_mania.menu.fullscreen = qtrue;

	s_mania.maniapic.generic.type = MTYPE_BITMAP;
	s_mania.maniapic.generic.name = maniapics[0];
	s_mania.maniapic.generic.x = 0;
	s_mania.maniapic.generic.y = 0;
	s_mania.maniapic.width = SCREEN_WIDTH;
	s_mania.maniapic.height = SCREEN_HEIGHT;

	s_mania.prev.generic.type = MTYPE_BITMAP;
	s_mania.prev.generic.name = ARROWLT0;
	s_mania.prev.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_mania.prev.generic.x = 364;
	s_mania.prev.generic.y = 446;
	s_mania.prev.generic.id = ID_PREV;
	s_mania.prev.generic.callback = UI_Mania_MenuEvent;
	s_mania.prev.width = 60;
	s_mania.prev.height = 26;
	s_mania.prev.focuspic = ARROWLT1;
	s_mania.prev.focuspicinstead = qtrue;

	s_mania.next.generic.type = MTYPE_BITMAP;
	s_mania.next.generic.name = ARROWRT0;
	s_mania.next.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_mania.next.generic.x = 440;
	s_mania.next.generic.y = 446;
	s_mania.next.generic.id = ID_NEXT;
	s_mania.next.generic.callback = UI_Mania_MenuEvent;
	s_mania.next.width = 60;
	s_mania.next.height = 26;
	s_mania.next.focuspic = ARROWRT1;
	s_mania.next.focuspicinstead = qtrue;

	s_mania.exit.generic.type = MTYPE_BITMAP;
	s_mania.exit.generic.name = EXIT0;
	s_mania.exit.generic.flags = QMF_PULSEIFFOCUS;
	s_mania.exit.generic.x = 8;
	s_mania.exit.generic.y = 446;
	s_mania.exit.generic.id = ID_EXIT;
	s_mania.exit.generic.callback = UI_Mania_MenuEvent;
	s_mania.exit.width = 80;
	s_mania.exit.height = 40;
	s_mania.exit.focuspic = EXIT1;
	s_mania.exit.focuspicinstead = qtrue;

	Menu_AddItem(&s_mania.menu, &s_mania.maniapic);
	Menu_AddItem(&s_mania.menu, &s_mania.next);
	Menu_AddItem(&s_mania.menu, &s_mania.prev);
	Menu_AddItem(&s_mania.menu, &s_mania.exit);

	UI_Mania_MenuUpdate();

}

/*
=================
UI_Mania_Cache
=================
*/
void UI_Mania_Cache(void) {
	int i;

	for (i = 0; i < MAX_MANIAPAGES; i++) {
		trap_R_RegisterShaderNoMip(maniapics[i]);
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
UI_ManiaMenu
===============
*/
void UI_ManiaMenu(void) {
	UI_Mania_MenuInit();
	UI_PushMenu(&s_mania.menu);
}
