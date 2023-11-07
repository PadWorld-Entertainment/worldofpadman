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
#include "ui_local.h"

#define ARROWUP0 "menu/arrows/headyel_up0"
#define ARROWUP1 "menu/arrows/headyel_up1"
#define ARROWDN0 "menu/arrows/headyel_dn0"
#define ARROWDN1 "menu/arrows/headyel_dn1"
#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define GO0 "menu/buttons/play_blu0"
#define GO1 "menu/buttons/play_blu1"

#define MAX_MODS 64
#define NAMEBUFSIZE (MAX_MODS * 48)
#define GAMEBUFSIZE (MAX_MODS * 16)

#define ID_LIST 10
#define ID_SCROLL_UP 11
#define ID_SCROLL_DOWN 12
#define ID_BACK 13
#define ID_GO 14

typedef struct {
	menuframework_s menu;

	menulist_s list;

	menubitmap_s arrowup;
	menubitmap_s arrowdown;
	menubitmap_s go;
	menubitmap_s back;

	char description[NAMEBUFSIZE];
	char fs_game[GAMEBUFSIZE];

	char *descriptionPtr;
	char *fs_gamePtr;

	char *descriptionList[MAX_MODS];
	char *fs_gameList[MAX_MODS];
} mods_t;

static mods_t s_mods;

/*
===============
UI_Mods_MenuEvent
===============
*/
static void UI_Mods_MenuEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_GO:
		trap_Cvar_Set("fs_game", s_mods.fs_gameList[s_mods.list.curvalue]);
		trap_Cvar_Set("s_wop_restarted", "0"); // damit bei padmod->anderer mod->padmod kein falscher sound bleibt
		trap_Cmd_ExecuteText(EXEC_APPEND, "vid_restart;");
		UI_PopMenu();
		break;

	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_SCROLL_UP:
		ScrollList_Key(&s_mods.list, K_UPARROW);
		break;

	case ID_SCROLL_DOWN:
		ScrollList_Key(&s_mods.list, K_DOWNARROW);
		break;
	}
}

/*
=================
UI_Mods_MenuKey
=================
*/
static sfxHandle_t UI_Mods_MenuKey(int key) {

	if (key == K_MWHEELUP) {
		ScrollList_Key(&s_mods.list, K_UPARROW);
	}
	if (key == K_MWHEELDOWN) {
		ScrollList_Key(&s_mods.list, K_DOWNARROW);
	}

	if (key == K_ENTER || key == K_KP_ENTER) {
		trap_Cvar_Set("fs_game", s_mods.fs_gameList[s_mods.list.curvalue]);
		trap_Cvar_Set("s_wop_restarted", "0"); // damit bei padmod->anderer mod->padmod kein falscher sound bleibt
		trap_Cmd_ExecuteText(EXEC_APPEND, "vid_restart;");
		UI_PopMenu();
	}

	return Menu_DefaultKey(&s_mods.menu, key);
}

/*
===============
UI_Mods_ParseInfos
===============
*/
static void UI_Mods_ParseInfos(char *modDir, char *modDesc) {
	s_mods.fs_gameList[s_mods.list.numitems] = s_mods.fs_gamePtr;
	Q_strncpyz(s_mods.fs_gamePtr, modDir, 16);

	s_mods.descriptionList[s_mods.list.numitems] = s_mods.descriptionPtr;
	Q_strncpyz(s_mods.descriptionPtr, modDesc, 48);

	s_mods.list.itemnames[s_mods.list.numitems] = s_mods.descriptionPtr;
	s_mods.descriptionPtr += strlen(s_mods.descriptionPtr) + 1;
	s_mods.fs_gamePtr += strlen(s_mods.fs_gamePtr) + 1;
	s_mods.list.numitems++;
}

/*
===============
UI_Mods_LoadMods
===============
*/
static void UI_Mods_LoadMods(void) {
	int numdirs;
	char dirlist[2048];
	char *dirptr;
	char *descptr;
	int i;
	int dirlen;

	s_mods.list.itemnames = (const char **)s_mods.descriptionList;
	s_mods.descriptionPtr = s_mods.description;
	s_mods.fs_gamePtr = s_mods.fs_game;

	// always start off with WoP
	s_mods.list.numitems = 1;
	s_mods.list.itemnames[0] = s_mods.descriptionList[0] = "World of PADMAN";
	s_mods.fs_gameList[0] = "";

	numdirs = trap_FS_GetFileList("$modlist", "", dirlist, sizeof(dirlist));
	dirptr = dirlist;
	for (i = 0; i < numdirs; i++) {
		dirlen = strlen(dirptr) + 1;
		descptr = dirptr + dirlen;
		UI_Mods_ParseInfos(dirptr, descptr);
		dirptr += dirlen + strlen(descptr) + 1;
	}

	trap_Print(va("%i mods parsed\n", s_mods.list.numitems));
	if (s_mods.list.numitems > MAX_MODS) {
		s_mods.list.numitems = MAX_MODS;
	}
}

/*
===============
UI_Mods_MenuInit
===============
*/
static void UI_Mods_MenuInit(void) {
	UI_ModsMenu_Cache();

	memset(&s_mods, 0, sizeof(mods_t));
	s_mods.menu.key = UI_Mods_MenuKey;
	s_mods.menu.wrapAround = qtrue;
	s_mods.menu.fullscreen = qtrue;
	s_mods.menu.bgparts = BGP_MODS | BGP_MENUFX;

	s_mods.arrowup.generic.type = MTYPE_BITMAP;
	s_mods.arrowup.generic.name = ARROWUP0;
	s_mods.arrowup.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_mods.arrowup.generic.callback = UI_Mods_MenuEvent;
	s_mods.arrowup.generic.id = ID_SCROLL_UP;
	s_mods.arrowup.generic.x = 62;
	s_mods.arrowup.generic.y = 140;
	s_mods.arrowup.width = 26;
	s_mods.arrowup.height = 60;
	s_mods.arrowup.focuspic = ARROWUP1;
	s_mods.arrowup.focuspicinstead = qtrue;

	s_mods.arrowdown.generic.type = MTYPE_BITMAP;
	s_mods.arrowdown.generic.name = ARROWDN0;
	s_mods.arrowdown.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_mods.arrowdown.generic.callback = UI_Mods_MenuEvent;
	s_mods.arrowdown.generic.id = ID_SCROLL_DOWN;
	s_mods.arrowdown.generic.x = 62;
	s_mods.arrowdown.generic.y = 256;
	s_mods.arrowdown.width = 26;
	s_mods.arrowdown.height = 60;
	s_mods.arrowdown.focuspic = ARROWDN1;
	s_mods.arrowdown.focuspicinstead = qtrue;

	s_mods.go.generic.type = MTYPE_BITMAP;
	s_mods.go.generic.name = GO0;
	s_mods.go.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_mods.go.generic.callback = UI_Mods_MenuEvent;
	s_mods.go.generic.id = ID_GO;
	s_mods.go.generic.x = 55;
	s_mods.go.generic.y = 208;
	s_mods.go.width = 40;
	s_mods.go.height = 40;
	s_mods.go.focuspic = GO1;
	s_mods.go.focuspicinstead = qtrue;

	s_mods.back.generic.type = MTYPE_BITMAP;
	s_mods.back.generic.name = BACK0;
	s_mods.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_mods.back.generic.x = 8;
	s_mods.back.generic.y = 446;
	s_mods.back.generic.id = ID_BACK;
	s_mods.back.generic.callback = UI_Mods_MenuEvent;
	s_mods.back.width = 80;
	s_mods.back.height = 40;
	s_mods.back.focuspic = BACK1;
	s_mods.back.focuspicinstead = qtrue;

	s_mods.list.generic.type = MTYPE_SCROLLLIST;
	s_mods.list.generic.flags = QMF_PULSEIFFOCUS | QMF_CENTER_JUSTIFY;
	s_mods.list.generic.callback = UI_Mods_MenuEvent;
	s_mods.list.generic.id = ID_LIST;
	s_mods.list.generic.x = 252;
	s_mods.list.generic.y = 60;
	s_mods.list.width = 37;
	s_mods.list.height = 21;

	UI_Mods_LoadMods();

	Menu_AddItem(&s_mods.menu, &s_mods.list);
	Menu_AddItem(&s_mods.menu, &s_mods.arrowup);
	Menu_AddItem(&s_mods.menu, &s_mods.arrowdown);
	Menu_AddItem(&s_mods.menu, &s_mods.go);
	Menu_AddItem(&s_mods.menu, &s_mods.back);
}

/*
=================
UI_Mods_Cache
=================
*/
void UI_ModsMenu_Cache(void) {
	trap_R_RegisterShaderNoMip(ARROWUP0);
	trap_R_RegisterShaderNoMip(ARROWUP1);
	trap_R_RegisterShaderNoMip(ARROWDN0);
	trap_R_RegisterShaderNoMip(ARROWDN1);
	trap_R_RegisterShaderNoMip(GO0);
	trap_R_RegisterShaderNoMip(GO1);
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
}

/*
===============
UI_ModsMenu
===============
*/
void UI_ModsMenu(void) {
	UI_Mods_MenuInit();
	UI_PushMenu(&s_mods.menu);
}
