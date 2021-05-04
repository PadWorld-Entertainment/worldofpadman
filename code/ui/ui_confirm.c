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

#define ART_CONFIRM_FRAME "menu/art/cut_frame"
#define CONF_YES0 "menu/exit/yes0"
#define CONF_YES1 "menu/exit/yes1"
#define CONF_NO0 "menu/exit/no0"
#define CONF_NO1 "menu/exit/no1"

#define ID_CONFIRM_NO 10
#define ID_CONFIRM_YES 11

typedef struct {
	menuframework_s menu;

	menubitmap_s yes;
	menubitmap_s no;
	menutext_s ok;

	int slashX;
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

	if (((menucommon_s *)ptr)->id == ID_CONFIRM_NO) {
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
	UI_DrawStringNS(320, 204, s_confirm.question, s_confirm.style, 24, color_black);

	Menu_Draw(&s_confirm.menu);

	if (s_confirm.draw) {
		s_confirm.draw();
	}
}

/*
=================
ConfirmMenu_Cache
=================
*/
void ConfirmMenu_Cache(void) {
	trap_R_RegisterShaderNoMip(ART_CONFIRM_FRAME);
}

/*
=================
UI_ConfirmMenu_Stlye
=================
*/
static void UI_ConfirmMenu_Style(const char *question, int style, void (*draw)(void), void (*action)(qboolean result)) {
	uiClientState_t cstate;
	int n1, n2, n3;
	int l1, l2, l3;

	// zero set all our globals
	memset(&s_confirm, 0, sizeof(s_confirm));

	ConfirmMenu_Cache();

	n1 = UI_ProportionalStringWidth("YES/NO");
	n2 = UI_ProportionalStringWidth("YES") + PROP_GAP_WIDTH;
	n3 = UI_ProportionalStringWidth("/") + PROP_GAP_WIDTH;
	l1 = 320 - (n1 / 2);
	l2 = l1 + n2;
	l3 = l2 + n3;
	s_confirm.slashX = l2;

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
	s_confirm.yes.generic.name = CONF_YES0;
	s_confirm.yes.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_confirm.yes.generic.callback = ConfirmMenu_Event;
	s_confirm.yes.generic.id = ID_CONFIRM_YES;
	s_confirm.yes.generic.x = l1 + 15;
	s_confirm.yes.generic.y = 264;
	s_confirm.yes.width = 43;  // 65;
	s_confirm.yes.height = 30; // 40;
	s_confirm.yes.focuspic = CONF_YES1;
	s_confirm.yes.focuspicinstead = qtrue;

	s_confirm.no.generic.type = MTYPE_BITMAP;
	s_confirm.no.generic.name = CONF_NO0;
	s_confirm.no.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_confirm.no.generic.callback = ConfirmMenu_Event;
	s_confirm.no.generic.id = ID_CONFIRM_NO;
	s_confirm.no.generic.x = l3;
	s_confirm.no.generic.y = 264;
	s_confirm.no.width = 33;  // 45;
	s_confirm.no.height = 30; // 40;
	s_confirm.no.focuspic = CONF_NO1;
	s_confirm.no.focuspicinstead = qtrue;

	Menu_AddItem(&s_confirm.menu, &s_confirm.yes);
	Menu_AddItem(&s_confirm.menu, &s_confirm.no);

	UI_PushMenu(&s_confirm.menu);

	Menu_SetCursorToItem(&s_confirm.menu, &s_confirm.no);
}

/*
=================
UI_ConfirmMenu
=================
*/
void UI_ConfirmMenu(const char *question, void (*draw)(void), void (*action)(qboolean result)) {
	UI_ConfirmMenu_Style(question, UI_CENTER | UI_INVERSE, draw, action);
}
