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

/*********************************************************************************
	SPECIFY SERVER
*********************************************************************************/

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define FIGHT0 "menu/buttons/fight0"
#define FIGHT1 "menu/buttons/fight1"

#define ID_BACK 102
#define ID_FIGHT 103

#define SPECIFY_QUERY_TIMEOUT 3000

static char *specifyserver_artlist[] = {BACK0, BACK1, FIGHT0, FIGHT1, NULL};

typedef struct {
	menuframework_s menu;
	menutext_s domainheader;
	menufield_s domain;
	menutext_s portheader;
	menufield_s port;
	menubitmap_s fight;
	menubitmap_s back;

	qboolean querying;
	int queryStart;
	char pendingAddr[256];
	char connectstring[MAX_STRING_CHARS];
} specifyserver_t;

static specifyserver_t s_specifyserver;

static void UI_SpecifyServer_ClearPings(void) {
	int i;

	for (i = 0; i < MAX_PINGREQUESTS; i++) {
		trap_LAN_ClearPing(i);
	}
}

/*
=================
UI_SpecifyServer_CachedNeedPass

If this address is already in a LAN server list, return whether it needs a
password. Otherwise *found is qfalse.
=================
*/
static qboolean UI_SpecifyServer_CachedNeedPass(const char *addr, qboolean *found) {
	static const int sources[] = {AS_LOCAL, AS_GLOBAL, AS_FAVORITES};
	int s, i, count;
	char serveraddr[MAX_STRING_CHARS];
	char info[MAX_INFO_STRING];

	*found = qfalse;

	for (s = 0; s < ARRAY_LEN(sources); s++) {
		count = trap_LAN_GetServerCount(sources[s]);
		if (count < 0) {
			continue;
		}

		for (i = 0; i < count; i++) {
			trap_LAN_GetServerAddressString(sources[s], i, serveraddr, sizeof(serveraddr));
			if (!serveraddr[0] || Q_stricmp(serveraddr, addr)) {
				continue;
			}

			// Master list entries have no getinfo yet, so g_needpass is unknown.
			if (trap_LAN_GetServerPing(sources[s], i) <= 0) {
				return qfalse;
			}

			trap_LAN_GetServerInfo(sources[s], i, info, sizeof(info));
			*found = qtrue;
			return atoi(Info_ValueForKey(info, "g_needpass")) ? qtrue : qfalse;
		}
	}

	return qfalse;
}

static void UI_SpecifyServer_FinishConnect(qboolean needPass) {
	if (s_specifyserver.querying) {
		UI_SpecifyServer_ClearPings();
	}
	s_specifyserver.querying = qfalse;

	if (needPass) {
		UI_SpecifyPasswordMenu(s_specifyserver.connectstring, s_specifyserver.pendingAddr);
	} else {
		trap_Cmd_ExecuteText(EXEC_APPEND, s_specifyserver.connectstring);
	}
}

static void UI_SpecifyServer_CheckQuery(void) {
	int i, time;
	char adrstr[MAX_STRING_CHARS];
	char info[MAX_INFO_STRING];

	if (!s_specifyserver.querying) {
		return;
	}

	for (i = 0; i < MAX_PINGREQUESTS; i++) {
		trap_LAN_GetPing(i, adrstr, sizeof(adrstr), &time);
		if (!adrstr[0] || !time) {
			continue;
		}

		trap_LAN_GetPingInfo(i, info, sizeof(info));
		UI_SpecifyServer_FinishConnect(atoi(Info_ValueForKey(info, "g_needpass")) ? qtrue : qfalse);
		return;
	}

	if (uis.realtime - s_specifyserver.queryStart > SPECIFY_QUERY_TIMEOUT) {
		UI_SpecifyServer_FinishConnect(qfalse);
	}
}

static void UI_SpecifyServer_StartQuery(const char *addr) {
	qboolean found;
	qboolean needPass;

	Q_strncpyz(s_specifyserver.pendingAddr, addr, sizeof(s_specifyserver.pendingAddr));
	Com_sprintf(s_specifyserver.connectstring, sizeof(s_specifyserver.connectstring), "connect %s\n", addr);

	needPass = UI_SpecifyServer_CachedNeedPass(addr, &found);
	if (found) {
		UI_SpecifyServer_FinishConnect(needPass);
		return;
	}

	UI_SpecifyServer_ClearPings();
	trap_Cmd_ExecuteText(EXEC_NOW, va("ping %s\n", addr));
	s_specifyserver.querying = qtrue;
	s_specifyserver.queryStart = uis.realtime;
}

/*
=================
UI_SpecifyServer_Draw
=================
*/
static void UI_SpecifyServer_Draw(void) {
	static const vec4_t color_specifyserver = {0.8f, 0.85f, 1.0f, 1.0f};

	UI_DrawProportionalString(SCREEN_WIDTH * 0.5f, 188, "SPECIFY SERVER", UI_CENTER | UI_SMALLFONT, color_specifyserver);
	if (s_specifyserver.querying) {
		UI_DrawProportionalString(SCREEN_WIDTH * 0.5f, 320, "Checking server...", UI_CENTER | UI_SMALLFONT,
								  color_specifyserver);
	}
	Menu_Draw(&s_specifyserver.menu);
	UI_SpecifyServer_CheckQuery();
}

/*
=================
UI_SpecifyServer_UpdateMenuItems
=================
*/
static void UI_SpecifyServer_UpdateMenuItems(void) {
	if (!s_specifyserver.querying && (s_specifyserver.domain.field.buffer[0]) && (s_specifyserver.port.field.buffer[0])) {
		s_specifyserver.fight.generic.flags &= ~QMF_GRAYED;
	} else {
		s_specifyserver.fight.generic.flags |= QMF_GRAYED;
	}
}

/*
=================
UI_SpecifyServer_Event
=================
*/
static void UI_SpecifyServer_Event(void *ptr, int event) {
	char buff[256];

	switch (((menucommon_s *)ptr)->id) {
	case ID_FIGHT:
		if (event != QM_ACTIVATED || s_specifyserver.querying)
			break;

		if (s_specifyserver.domain.field.buffer[0]) {
			strcpy(buff, s_specifyserver.domain.field.buffer);
			if (s_specifyserver.port.field.buffer[0])
				Com_sprintf(buff + strlen(buff), 128, ":%s", s_specifyserver.port.field.buffer);

			UI_SpecifyServer_StartQuery(buff);
		}
		break;

	case ID_BACK:
		if (event != QM_ACTIVATED)
			break;

		if (s_specifyserver.querying) {
			s_specifyserver.querying = qfalse;
			UI_SpecifyServer_ClearPings();
		}
		UI_PopMenu();
		break;
	}
}

static void SpecifyServer_DrawField(void *self) {
	Menu_DrawField((menufield_s *)self);
	UI_SpecifyServer_UpdateMenuItems();
}

/*
=================
UI_SpecifyServer_MenuInit
=================
*/
static void UI_SpecifyServer_MenuInit(void) {
	// zero set all our globals
	memset(&s_specifyserver, 0, sizeof(s_specifyserver));

	UI_SpecifyServer_Cache();

	s_specifyserver.menu.draw = UI_SpecifyServer_Draw;
	s_specifyserver.menu.wrapAround = qtrue;
	s_specifyserver.menu.fullscreen = qtrue;
	s_specifyserver.menu.bgparts = BGP_SPECIFY | BGP_MENUFX;

	s_specifyserver.domainheader.generic.type = MTYPE_TEXT;
	s_specifyserver.domainheader.generic.x = 342;
	s_specifyserver.domainheader.generic.y = 214;
	s_specifyserver.domainheader.string = "Address:";
	s_specifyserver.domainheader.style = UI_SMALLFONT;
	s_specifyserver.domainheader.color = menu_text_color;

	s_specifyserver.domain.generic.type = MTYPE_FIELD;
	s_specifyserver.domain.generic.name = "";
	s_specifyserver.domain.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	s_specifyserver.domain.generic.x = 342;
	s_specifyserver.domain.generic.y = 230;
	s_specifyserver.domain.field.widthInChars = 22;
	s_specifyserver.domain.field.maxchars = 80;
	s_specifyserver.domain.generic.ownerdraw = SpecifyServer_DrawField;

	s_specifyserver.portheader.generic.type = MTYPE_TEXT;
	s_specifyserver.portheader.generic.x = 342;
	s_specifyserver.portheader.generic.y = 259;
	s_specifyserver.portheader.string = "Port:";
	s_specifyserver.portheader.style = UI_SMALLFONT;
	s_specifyserver.portheader.color = menu_text_color;

	s_specifyserver.port.generic.type = MTYPE_FIELD;
	s_specifyserver.port.generic.name = "";
	s_specifyserver.port.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT | QMF_NUMBERSONLY;
	s_specifyserver.port.generic.x = 342;
	s_specifyserver.port.generic.y = 275;
	s_specifyserver.port.field.widthInChars = 22;
	s_specifyserver.port.field.maxchars = 5;
	s_specifyserver.port.generic.ownerdraw = SpecifyServer_DrawField;

	s_specifyserver.back.generic.type = MTYPE_BITMAP;
	s_specifyserver.back.generic.name = BACK0;
	s_specifyserver.back.generic.flags = QMF_PULSEIFFOCUS;
	s_specifyserver.back.generic.x = 8;
	s_specifyserver.back.generic.y = 446;
	s_specifyserver.back.generic.id = ID_BACK;
	s_specifyserver.back.generic.callback = UI_SpecifyServer_Event;
	s_specifyserver.back.width = 80;
	s_specifyserver.back.height = 40;
	s_specifyserver.back.focuspic = BACK1;
	s_specifyserver.back.focuspicinstead = qtrue;

	s_specifyserver.fight.generic.type = MTYPE_BITMAP;
	s_specifyserver.fight.generic.name = FIGHT0;
	s_specifyserver.fight.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	s_specifyserver.fight.generic.callback = UI_SpecifyServer_Event;
	s_specifyserver.fight.generic.id = ID_FIGHT;
	s_specifyserver.fight.focuspic = FIGHT1;
	s_specifyserver.fight.generic.x = 776;
	s_specifyserver.fight.generic.y = 432;
	s_specifyserver.fight.width = 80;
	s_specifyserver.fight.height = 40;
	s_specifyserver.fight.focuspicinstead = qtrue;

	Menu_AddItem(&s_specifyserver.menu, &s_specifyserver.domainheader);
	Menu_AddItem(&s_specifyserver.menu, &s_specifyserver.domain);
	Menu_AddItem(&s_specifyserver.menu, &s_specifyserver.portheader);
	Menu_AddItem(&s_specifyserver.menu, &s_specifyserver.port);
	Menu_AddItem(&s_specifyserver.menu, &s_specifyserver.fight);
	Menu_AddItem(&s_specifyserver.menu, &s_specifyserver.back);

	Com_sprintf(s_specifyserver.port.field.buffer, 6, "%i", 27960);
}

/*
=================
UI_SpecifyServer_Cache
=================
*/
void UI_SpecifyServer_Cache(void) {
	int i;

	// touch all our pics
	for (i = 0;; i++) {
		if (!specifyserver_artlist[i])
			break;
		trap_R_RegisterShaderNoMip(specifyserver_artlist[i]);
	}
}

/*
=================
UI_SpecifyServerMenu
=================
*/
void UI_SpecifyServerMenu(void) {
	UI_SpecifyServer_MenuInit();
	UI_PushMenu(&s_specifyserver.menu);
}
