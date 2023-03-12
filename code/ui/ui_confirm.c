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

CONFIRMATION MENU

=======================================================================
*/

#include "ui_local.h"

#define YES0 "menu/buttons/yes0"
#define YES1 "menu/buttons/yes1"
#define NO0 "menu/buttons/no0"
#define NO1 "menu/buttons/no1"

#define ID_NO 10
#define ID_YES 11

#define XPOSITION (SCREEN_WIDTH / 2)

typedef struct {
	menuframework_s menu;

	menubitmap_s yes;
	menubitmap_s no;
	menutext_s ok;

	const char *question;
	void (*draw)(void);
	void (*action)(qboolean result);

	int style;
	const char **lines;
} confirmMenu_t;

static confirmMenu_t s_confirm;

/*
=================
ConfirmMenu_Event
=================
*/
static void ConfirmMenu_Event(void *ptr, int event) {
	qboolean result;

	if (event != QM_ACTIVATED) {
		return;
	}

	UI_PopMenu();

	if (((menucommon_s *)ptr)->id == ID_NO) {
		result = qfalse;
	} else {
		result = qtrue;
	}

	if (s_confirm.action) {
		s_confirm.action(result);
	}
}

/*
=================
ConfirmMenu_Key
=================
*/
static sfxHandle_t ConfirmMenu_Key(int key) {
	switch (key) {
	case K_KP_LEFTARROW:
	case K_LEFTARROW:
	case K_KP_RIGHTARROW:
	case K_RIGHTARROW:
		key = K_TAB;
		break;

	case 'n':
	case 'N':
		ConfirmMenu_Event(&s_confirm.no, QM_ACTIVATED);
		break;

	case 'y':
	case 'Y':
		ConfirmMenu_Event(&s_confirm.yes, QM_ACTIVATED);
		break;
	}

	return Menu_DefaultKey(&s_confirm.menu, key);
}

/*
=================
ConfirmMenu_Draw
=================
*/
static void ConfirmMenu_Draw(void) {
	UI_DrawIngameBG();
	UI_DrawStringNS(XPOSITION, 204, s_confirm.question, s_confirm.style, 24, color_black);

	Menu_Draw(&s_confirm.menu);

	if (s_confirm.draw) {
		s_confirm.draw();
	}
}

/*
=================
UI_ConfirmMenu_Stlye
=================
*/
static void UI_ConfirmMenu_Style(const char *question, int style, void (*draw)(void), void (*action)(qboolean result)) {
	uiClientState_t cstate;

	// zero set all our globals
	memset(&s_confirm, 0, sizeof(s_confirm));

	UI_ConfirmMenu_Cache();

	s_confirm.question = question;
	s_confirm.draw = draw;
	s_confirm.action = action;
	s_confirm.style = style;

	s_confirm.menu.draw = ConfirmMenu_Draw;
	s_confirm.menu.key = ConfirmMenu_Key;
	s_confirm.menu.wrapAround = qtrue;

	trap_GetClientState(&cstate);
	if (cstate.connState >= CA_CONNECTED) {
		s_confirm.menu.fullscreen = qfalse;
	} else {
		s_confirm.menu.fullscreen = qtrue;
	}

	s_confirm.yes.generic.type = MTYPE_BITMAP;
	s_confirm.yes.generic.name = YES0;
	s_confirm.yes.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_confirm.yes.generic.callback = ConfirmMenu_Event;
	s_confirm.yes.generic.id = ID_YES;
	s_confirm.yes.generic.x = XPOSITION - 60;
	s_confirm.yes.generic.y = 260;
	s_confirm.yes.width = 60;
	s_confirm.yes.height = 30;
	s_confirm.yes.focuspic = YES1;
	s_confirm.yes.focuspicinstead = qtrue;

	s_confirm.no.generic.type = MTYPE_BITMAP;
	s_confirm.no.generic.name = NO0;
	s_confirm.no.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_confirm.no.generic.callback = ConfirmMenu_Event;
	s_confirm.no.generic.id = ID_NO;
	s_confirm.no.generic.x = XPOSITION + 15;
	s_confirm.no.generic.y = 260;
	s_confirm.no.width = 30;
	s_confirm.no.height = 30;
	s_confirm.no.focuspic = NO1;
	s_confirm.no.focuspicinstead = qtrue;

	Menu_AddItem(&s_confirm.menu, &s_confirm.yes);
	Menu_AddItem(&s_confirm.menu, &s_confirm.no);

	UI_PushMenu(&s_confirm.menu);

	Menu_SetCursorToItem(&s_confirm.menu, &s_confirm.no);
}

/*
=================
UI_ConfirmMenu_Cache
=================
*/
void UI_ConfirmMenu_Cache(void) {
	trap_R_RegisterShaderNoMip(YES0);
	trap_R_RegisterShaderNoMip(YES1);
	trap_R_RegisterShaderNoMip(NO0);
	trap_R_RegisterShaderNoMip(NO1);
}

/*
=================
UI_ConfirmMenu
=================
*/
void UI_ConfirmMenu(const char *question, void (*draw)(void), void (*action)(qboolean result)) {
	UI_ConfirmMenu_Style(question, UI_CENTER | UI_INVERSE, draw, action);
}
