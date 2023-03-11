/*
===========================================================================
Copyright (C) 2009 Poul Sander (OpenArena)
Copyright (C) 2021 Padworld Entertainment

This file is part of the World of PADMAN source code.

World of PADMAN source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

World of PADMAN source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with World of PADMAN source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
#include "ui_local.h"

/*********************************************************************************
		SPECIFY PASSWORD
*********************************************************************************/

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define FIGHT0 "menu/buttons/fight0"
#define FIGHT1 "menu/buttons/fight1"

#define ID_BACK 102
#define ID_FIGHT 103

static const char *specifypassword_artlist[] = {BACK0, BACK1, FIGHT0, FIGHT1, NULL};

typedef struct {
	menuframework_s menu;
	menutext_s passwordheader;
	menufield_s password;
	menubitmap_s fight;
	menubitmap_s back;

	const char *connectstring;
	char servername[32];
} specifypassword_t;

static specifypassword_t s_specifypassword;

/*
=================
UI_SpecifyPassword_UpdateMenuItems
=================
*/
static void UI_SpecifyPassword_UpdateMenuItems(void) {
	if (s_specifypassword.password.field.buffer[0]) {
		s_specifypassword.fight.generic.flags &= ~QMF_GRAYED;
	} else {
		s_specifypassword.fight.generic.flags |= QMF_GRAYED;
	}
}

/*
=================
SpecifyPassword_Event
=================
*/
static void SpecifyPassword_Event(void *ptr, int event) {
	switch (((menucommon_s *)ptr)->id) {
	case ID_FIGHT:
		if (event != QM_ACTIVATED)
			break;

		if (s_specifypassword.password.field.buffer[0]) {
			trap_Cvar_Set("password", s_specifypassword.password.field.buffer);
			trap_Cmd_ExecuteText(EXEC_APPEND, s_specifypassword.connectstring);
		}
		break;

	case ID_BACK:
		if (event != QM_ACTIVATED)
			break;

		UI_PopMenu();
		break;
	}
}

/*
=================
SpecifyPassword_Cache
=================
*/
static void SpecifyPassword_Cache(void) {
	int i;

	// touch all our pics
	for (i = 0;; i++) {
		if (!specifypassword_artlist[i])
			break;
		trap_R_RegisterShaderNoMip(specifypassword_artlist[i]);
	}
}

static void SpecifyPassword_DrawField(void *self) {
	Menu_DrawField((menufield_s *)self);
	UI_SpecifyPassword_UpdateMenuItems();
}

/*
=================
SpecifyPassword_MenuInit
=================
*/
static void SpecifyPassword_MenuInit(void) {
	// zero set all our globals
	memset(&s_specifypassword, 0, sizeof(specifypassword_t));

	SpecifyPassword_Cache();

	s_specifypassword.menu.wrapAround = qtrue;
	s_specifypassword.menu.fullscreen = qtrue;
	s_specifypassword.menu.bgparts = BGP_SPECIFYPASS | BGP_MENUFX;

	s_specifypassword.passwordheader.generic.type = MTYPE_TEXT;
	s_specifypassword.passwordheader.generic.x = 342;
	s_specifypassword.passwordheader.generic.y = 218;
	s_specifypassword.passwordheader.string = "Password:";
	s_specifypassword.passwordheader.style = UI_SMALLFONT;
	s_specifypassword.passwordheader.color = menu_text_color;

	s_specifypassword.password.generic.type = MTYPE_FIELD;
	s_specifypassword.password.generic.name = "";
	s_specifypassword.password.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	s_specifypassword.password.generic.x = 342;
	s_specifypassword.password.generic.y = 234;
	s_specifypassword.password.field.widthInChars = 22;
	s_specifypassword.password.field.maxchars = 80;
	s_specifypassword.password.generic.ownerdraw = SpecifyPassword_DrawField;
	trap_Cvar_VariableStringBuffer("password", s_specifypassword.password.field.buffer, 80);

	s_specifypassword.back.generic.type = MTYPE_BITMAP;
	s_specifypassword.back.generic.name = BACK0;
	s_specifypassword.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_specifypassword.back.generic.x = 8;
	s_specifypassword.back.generic.y = 446;
	s_specifypassword.back.generic.id = ID_BACK;
	s_specifypassword.back.generic.callback = SpecifyPassword_Event;
	s_specifypassword.back.width = 80;
	s_specifypassword.back.height = 40;
	s_specifypassword.back.focuspic = BACK1;
	s_specifypassword.back.focuspicinstead = qtrue;

	s_specifypassword.fight.generic.type = MTYPE_BITMAP;
	s_specifypassword.fight.generic.name = FIGHT0;
	s_specifypassword.fight.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_specifypassword.fight.generic.callback = SpecifyPassword_Event;
	s_specifypassword.fight.generic.id = ID_FIGHT;
	s_specifypassword.fight.focuspic = FIGHT1;
	s_specifypassword.fight.generic.x = 776;
	s_specifypassword.fight.generic.y = 420;
	s_specifypassword.fight.width = 80;
	s_specifypassword.fight.height = 60;
	s_specifypassword.fight.focuspicinstead = qtrue;

	Menu_AddItem(&s_specifypassword.menu, &s_specifypassword.passwordheader);
	Menu_AddItem(&s_specifypassword.menu, &s_specifypassword.password);
	Menu_AddItem(&s_specifypassword.menu, &s_specifypassword.fight);
	Menu_AddItem(&s_specifypassword.menu, &s_specifypassword.back);
}

/*
=================
UI_SpecifyPasswordMenu
=================
*/
void UI_SpecifyPasswordMenu(const char *string, const char *name) {
	SpecifyPassword_MenuInit();
	s_specifypassword.connectstring = string;
	Q_strncpyz(s_specifypassword.servername, name, sizeof(s_specifypassword.servername));
	UI_PushMenu(&s_specifypassword.menu);
}
