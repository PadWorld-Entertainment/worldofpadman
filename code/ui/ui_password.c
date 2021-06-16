/*
===========================================================================
Copyright (C) 2009 Poul Sander (OpenArena)
Copyright (C) 2021 Padworld Entertainment

This file is part of the World of Padman source code.

World of Padman source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

World of Padman source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with World of Padman source code; if not, write to the Free Software
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

#define ID_SPECIFYPASSWORDBACK 102
#define ID_SPECIFYPASSWORDGO 103

static char *specifypassword_artlist[] = {BACK0, BACK1, FIGHT0, FIGHT1, NULL};

typedef struct {
	menuframework_s menu;
	menufield_s password;
	menubitmap1024s_s go;
	menubitmap_s back;

	const char *connectstring;
	char servername[32];
} specifypassword_t;

static specifypassword_t s_specifypassword;

/*
=================
SpecifyPassword_Event
=================
*/
static void SpecifyPassword_Event(void *ptr, int event) {
	switch (((menucommon_s *)ptr)->id) {
	case ID_SPECIFYPASSWORDGO:
		if (event != QM_ACTIVATED)
			break;

		if (s_specifypassword.password.field.buffer[0]) {
			trap_Cvar_Set("password", s_specifypassword.password.field.buffer);
			trap_Cmd_ExecuteText(EXEC_APPEND, s_specifypassword.connectstring);
		}
		break;

	case ID_SPECIFYPASSWORDBACK:
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

extern void SpecifyServer_DrawField(void *self);

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
	s_specifypassword.menu.bgparts = BGP_SPECIFYPASSBG | BGP_SIMPLEBG;

	s_specifypassword.password.generic.type = MTYPE_FIELD;
	s_specifypassword.password.generic.name = "";
	s_specifypassword.password.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	s_specifypassword.password.generic.x = 230;
	s_specifypassword.password.generic.y = 234;
	s_specifypassword.password.field.widthInChars = 22;
	s_specifypassword.password.field.maxchars = 80;
	s_specifypassword.password.generic.ownerdraw = SpecifyServer_DrawField;
	trap_Cvar_VariableStringBuffer("password", s_specifypassword.password.field.buffer, 80);

	s_specifypassword.back.generic.type = MTYPE_BITMAP;
	s_specifypassword.back.generic.name = BACK0;
	s_specifypassword.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_specifypassword.back.generic.x = 8;
	s_specifypassword.back.generic.y = 440;
	s_specifypassword.back.generic.id = ID_SPECIFYPASSWORDBACK;
	s_specifypassword.back.generic.callback = SpecifyPassword_Event;
	s_specifypassword.back.width = 80;
	s_specifypassword.back.height = 40;
	s_specifypassword.back.focuspic = BACK1;
	s_specifypassword.back.focuspicinstead = qtrue;

	s_specifypassword.go.generic.type = MTYPE_BITMAP1024S;
	s_specifypassword.go.x = 845;
	s_specifypassword.go.y = 658;
	s_specifypassword.go.w = 135;
	s_specifypassword.go.h = 97;
	s_specifypassword.go.shader = FIGHT0;
	s_specifypassword.go.mouseovershader = FIGHT1;
	s_specifypassword.go.generic.callback = SpecifyPassword_Event;
	s_specifypassword.go.generic.id = ID_SPECIFYPASSWORDGO;

	Menu_AddItem(&s_specifypassword.menu, &s_specifypassword.password);
	Menu_AddItem(&s_specifypassword.menu, &s_specifypassword.go);
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
