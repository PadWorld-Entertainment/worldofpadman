// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define ARROWUP0 "menu/arrows/headyel_up0"
#define ARROWUP1 "menu/arrows/headyel_up1"
#define ARROWDN0 "menu/arrows/headyel_dn0"
#define ARROWDN1 "menu/arrows/headyel_dn1"

#define ID_BACK 10
#define ID_ADD 11
#define ID_SCROLL_UP 12
#define ID_SCROLL_DOWN 13

#define XPOSITION (SCREEN_WIDTH / 2)

typedef struct {
	menuframework_s menu;

	menubitmap_s arrowup;
	menubitmap_s arrowdown;
	menubitmap_s back;

	menutext_s add;
	char info[MAX_INFO_STRING];
	int numlines;

	int firstline;
	int numdrawn;
} serverinfo_t;

static serverinfo_t s_serverinfo;

/*
=================
Favorites_Add

Add current server to favorites
=================
*/
static void Favorites_Add(void) {
	char adrstr[128];
	char serverbuff[128];
	int i;
	int best;

	trap_Cvar_VariableStringBuffer("cl_currentServerAddress", serverbuff, sizeof(serverbuff));
	if (!serverbuff[0])
		return;

	best = 0;
	for (i = 0; i < MAX_FAVORITESERVERS; i++) {
		trap_Cvar_VariableStringBuffer(va("server%d", i + 1), adrstr, sizeof(adrstr));
		if (!Q_stricmp(serverbuff, adrstr)) {
			// already in list
			return;
		}

		if (adrstr[0] == '\0') // if there is a empty one -> take it ;)
		{
			best = i + 1;
			break;
		}

		// use first empty or non-numeric available slot
		if ((adrstr[0] < '0' || adrstr[0] > '9') && !best)
			best = i + 1;
	}

	if (best)
		trap_Cvar_Set(va("server%d", best), serverbuff);
}

/*
=================
ServerInfo_Event
=================
*/
static void ServerInfo_Event(void *ptr, int event) {
	switch (((menucommon_s *)ptr)->id) {
	case ID_ADD:
		if (event != QM_ACTIVATED)
			break;

		Favorites_Add();
		UI_PopMenu();
		break;

	case ID_BACK:
		if (event != QM_ACTIVATED)
			break;
		UI_PopMenu();
		break;

	case ID_SCROLL_UP:
		if (event != QM_ACTIVATED)
			break;

		if (s_serverinfo.firstline > 0)
			s_serverinfo.firstline--;
		break;

	case ID_SCROLL_DOWN:
		if (event != QM_ACTIVATED)
			break;

		if ((s_serverinfo.firstline + s_serverinfo.numdrawn) < s_serverinfo.numlines) {
			s_serverinfo.firstline++;
		}
		break;
	}
}

/*
=================
ServerInfo_MenuDraw
=================
*/
static void ServerInfo_MenuDraw(void) {
	const char *s;
	char key[MAX_INFO_KEY];
	char value[MAX_INFO_VALUE];
	int y, i = 0;
	int keylen, vallen, infonum = -1;

	UI_DrawIngameBG();
	UI_DrawProportionalString(XPOSITION, 110, "SERVER INFO", UI_CENTER | UI_SMALLFONT, color_black);

	y = 140;
	s = s_serverinfo.info;
	s_serverinfo.numdrawn = 0;
	while (s && i < s_serverinfo.numlines) {
		i++;
		Info_NextPair(&s, key, value);
		if (!key[0]) {
			break;
		}

		infonum++;
		if (s_serverinfo.firstline > infonum)
			continue;

		if (y > 260)
			break;

		Com_sprintf(key, MAX_INFO_KEY, "%s: ", key);
		keylen = Q_PrintStrlen(key);
		vallen = Q_PrintStrlen(value);
		if (keylen + vallen < 20) {
			UI_DrawString(XPOSITION - 90, y, key, UI_LEFT | UI_SMALLFONT, color_black);
			UI_DrawString(XPOSITION - 90 + keylen * 8, y, value, UI_LEFT | UI_SMALLFONT, color_blue);

			s_serverinfo.numdrawn++;
		} else {
			int j;

			// TODO: Also add linebreaks for long keys?
			UI_DrawString(XPOSITION - 90, y, key, UI_LEFT | UI_SMALLFONT, color_black);

			for (j = 0; j < vallen; j += 20) {
				y += SMALLCHAR_HEIGHT;
				if (y > 260)
					break;

				UI_DrawString(XPOSITION - 90, y, va("%20.20s", &value[j]), UI_LEFT | UI_SMALLFONT, color_blue);

				s_serverinfo.numdrawn++;
			}
		}

		y += SMALLCHAR_HEIGHT;
	}

	Menu_Draw(&s_serverinfo.menu);
}

/*
=================
ServerInfo_MenuKey
=================
*/
static sfxHandle_t ServerInfo_MenuKey(int key) {
	switch (key) {
	case K_MWHEELUP:
	case K_PGUP:
		ServerInfo_Event(&s_serverinfo.arrowup, QM_ACTIVATED);
		break;
	case K_MWHEELDOWN:
	case K_PGDN:
		ServerInfo_Event(&s_serverinfo.arrowdown, QM_ACTIVATED);
		break;
	}

	return (Menu_DefaultKey(&s_serverinfo.menu, key));
}

/*
=================
UI_ServerInfoMenu
=================
*/
void UI_ServerInfoMenu(void) {
	const char *s;
	char key[MAX_INFO_KEY];
	char value[MAX_INFO_VALUE];

	// zero set all our globals
	memset(&s_serverinfo, 0, sizeof(serverinfo_t));

	UI_ServerInfo_Cache();

 	s_serverinfo.menu.draw = ServerInfo_MenuDraw;
	s_serverinfo.menu.key = ServerInfo_MenuKey;

	s_serverinfo.arrowup.generic.type = MTYPE_BITMAP;
	s_serverinfo.arrowup.generic.name = ARROWUP0;
	s_serverinfo.arrowup.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_serverinfo.arrowup.generic.callback = ServerInfo_Event;
	s_serverinfo.arrowup.generic.id = ID_SCROLL_UP;
	s_serverinfo.arrowup.generic.x = XPOSITION + 105;
	s_serverinfo.arrowup.generic.y = 132;
	s_serverinfo.arrowup.width = 26;
	s_serverinfo.arrowup.height = 60;
	s_serverinfo.arrowup.focuspic = ARROWUP1;
	s_serverinfo.arrowup.focuspicinstead = qtrue;

	s_serverinfo.arrowdown.generic.type = MTYPE_BITMAP;
	s_serverinfo.arrowdown.generic.name = ARROWDN0;
	s_serverinfo.arrowdown.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_serverinfo.arrowdown.generic.callback = ServerInfo_Event;
	s_serverinfo.arrowdown.generic.id = ID_SCROLL_DOWN;
	s_serverinfo.arrowdown.generic.x = XPOSITION + 105;
	s_serverinfo.arrowdown.generic.y = 208;
	s_serverinfo.arrowdown.width = 26;
	s_serverinfo.arrowdown.height = 60;
	s_serverinfo.arrowdown.focuspic = ARROWDN1;
	s_serverinfo.arrowdown.focuspicinstead = qtrue;

	s_serverinfo.add.generic.type = MTYPE_TEXTS;
	s_serverinfo.add.fontHeight = 18.0f;
	s_serverinfo.add.generic.flags = QMF_CENTER_JUSTIFY;
	s_serverinfo.add.generic.callback = ServerInfo_Event;
	s_serverinfo.add.generic.id = ID_ADD;
	s_serverinfo.add.generic.x = XPOSITION;
	s_serverinfo.add.generic.y = 290;
	s_serverinfo.add.string = "ADD TO FAVORiTES";
	s_serverinfo.add.style = UI_CENTER | UI_SMALLFONT;
	s_serverinfo.add.color = colorDkBlue;
	s_serverinfo.add.focuscolor = colorBlue;
	if (trap_Cvar_VariableValue("sv_running")) {
		s_serverinfo.add.generic.flags |= QMF_GRAYED;
	}

	s_serverinfo.back.generic.type = MTYPE_BITMAP;
	s_serverinfo.back.generic.name = BACK0;
	s_serverinfo.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_serverinfo.back.generic.x = XPOSITION - 95;
	s_serverinfo.back.generic.y = 340;
	s_serverinfo.back.generic.id = ID_BACK;
	s_serverinfo.back.generic.callback = ServerInfo_Event;
	s_serverinfo.back.width = 50;
	s_serverinfo.back.height = 25;
	s_serverinfo.back.focuspic = BACK1;
	s_serverinfo.back.focuspicinstead = qtrue;

	trap_GetConfigString(CS_SERVERINFO, s_serverinfo.info, MAX_INFO_STRING);

	s_serverinfo.numlines = 0;
	s = s_serverinfo.info;
	while (s) {
		Info_NextPair(&s, key, value);
		if (!key[0]) {
			break;
		}
		s_serverinfo.numlines++;
	}

	Menu_AddItem(&s_serverinfo.menu, (void *)&s_serverinfo.arrowup);
	Menu_AddItem(&s_serverinfo.menu, (void *)&s_serverinfo.arrowdown);
	Menu_AddItem(&s_serverinfo.menu, (void *)&s_serverinfo.add);
	Menu_AddItem(&s_serverinfo.menu, (void *)&s_serverinfo.back);

	UI_PushMenu(&s_serverinfo.menu);
}

/*
=================
UI_ServerInfo_Cache
=================
*/
void UI_ServerInfo_Cache(void) {
	trap_R_RegisterShaderNoMip(ARROWUP0);
	trap_R_RegisterShaderNoMip(ARROWUP1);
	trap_R_RegisterShaderNoMip(ARROWDN0);
	trap_R_RegisterShaderNoMip(ARROWDN1);
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
}
