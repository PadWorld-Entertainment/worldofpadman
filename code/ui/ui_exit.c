/*
####################### ####################### ####################### ####################### #######################

  exit(+credits) menu

####################### ####################### ####################### ####################### #######################
*/

#include "ui_local.h"

#define ID_YES 10
#define ID_NO 11

#define YES0 "menu/buttons/yes0_ws"
#define YES1 "menu/buttons/yes1"
#define NO0 "menu/buttons/no0_ws"
#define NO1 "menu/buttons/no1"
#define THEEND "menu/art/theend"

typedef struct {
	menuframework_s menu;

	menubitmap_s Yes;
	menubitmap_s No;

	qhandle_t theend;
	int starttime;
} exitmenu_t;

static exitmenu_t exitmenu;

/*
#######################
ExitMenu_Event
#######################
*/
void ExitMenu_Event(void *ptr, int event) {
	if (event != QM_ACTIVATED)
		return;

	switch (((menucommon_s *)ptr)->id) {
	case ID_YES:
		UI_PopMenu();
		UI_PopMenu();
		UI_CreditMenu();
		break;

	case ID_NO:
		UI_PopMenu();
		break;
	}
}

/*
#######################
ExitMenu_Cache
#######################
*/
void ExitMenu_Cache(void) {
	trap_R_RegisterShaderNoMip(YES0);
	trap_R_RegisterShaderNoMip(YES1);
	trap_R_RegisterShaderNoMip(NO0);
	trap_R_RegisterShaderNoMip(NO1);
	exitmenu.theend = trap_R_RegisterShaderNoMip(THEEND);
}

/*
#######################
ExitMenu_Draw
#######################
*/
static void ExitMenu_Draw(void) {
	float x, y, w, h, scale;

	scale = (float)(uis.realtime - exitmenu.starttime) * 0.003f; // 0.001f;
	if (scale > 1.0f)
		scale = 1.0f;
	else if (scale < 0.0f)
		scale = 0.0f;

	// mitte: 320/209
	w = 310.0f * scale;
	h = 110.0f * scale;
	x = 320.0f - w * 0.5f;
	y = 209.0f - h * 0.5f;

	UI_AdjustFrom640(&x, &y, &w, &h);

	trap_R_DrawStretchPic(x, y, w, h, 0, 0, 1, 1, exitmenu.theend);

	Menu_Draw(&exitmenu.menu);
}

/*
#######################
ExitMenu_Init
#######################
*/
void ExitMenu_Init(void) {
	memset(&exitmenu, 0, sizeof(exitmenu));

	ExitMenu_Cache();
	exitmenu.starttime = uis.realtime + 100;

	exitmenu.menu.fullscreen = qtrue;
	exitmenu.menu.wrapAround = qtrue;
	exitmenu.menu.bgparts = BGP_EXITBG;
	exitmenu.menu.draw = ExitMenu_Draw;

	exitmenu.Yes.generic.type = MTYPE_BITMAP;
	exitmenu.Yes.generic.name = YES0;
	exitmenu.Yes.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	exitmenu.Yes.generic.x = 240;
	exitmenu.Yes.generic.y = 380;
	exitmenu.Yes.generic.id = ID_YES;
	exitmenu.Yes.generic.callback = ExitMenu_Event;
	exitmenu.Yes.width = 80;
	exitmenu.Yes.height = 40;
	exitmenu.Yes.focuspic = YES1;
	exitmenu.Yes.focuspicinstead = qtrue;
	Menu_AddItem(&exitmenu.menu, &exitmenu.Yes);

	exitmenu.No.generic.type = MTYPE_BITMAP;
	exitmenu.No.generic.name = NO0;
	exitmenu.No.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	exitmenu.No.generic.x = 340;
	exitmenu.No.generic.y = 380;
	exitmenu.No.generic.id = ID_NO;
	exitmenu.No.generic.callback = ExitMenu_Event;
	exitmenu.No.width = 40;
	exitmenu.No.height = 40;
	exitmenu.No.focuspic = NO1;
	exitmenu.No.focuspicinstead = qtrue;
	Menu_AddItem(&exitmenu.menu, &exitmenu.No);

	UI_PushMenu(&exitmenu.menu);
}
