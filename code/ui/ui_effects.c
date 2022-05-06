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

EFFECTS OPTIONS MENU

=======================================================================
*/

#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define GRAPHICS0 "menu/buttons/graphics0"
#define GRAPHICS1 "menu/buttons/graphics1"
#define DISPLAY0 "menu/buttons/display0"
#define DISPLAY1 "menu/buttons/display1"
#define SOUND0 "menu/buttons/sound0"
#define SOUND1 "menu/buttons/sound1"
#define NETWORK0 "menu/buttons/netvoip0"
#define NETWORK1 "menu/buttons/netvoip1"
#define ACCEPT0 "menu/buttons/accept"
#define ACCEPT1 "menu/buttons/accept"

#define ID_GRAPHICS 100
#define ID_DISPLAY 101
#define ID_SOUND 102
#define ID_NETWORK 103
#define ID_BACK 104
#define ID_APPLY 105

#define ID_HDR 10
#define ID_SSAO 11
#define ID_SUNSHADOWS 12
#define ID_SUNLIGHTMODE 13

#define XPOSITION 180
#define YPOSITION 180 + 36

typedef struct {
	menuframework_s menu;

	menubitmap_s graphics;
	menubitmap_s display;
	menubitmap_s sound;
	menubitmap_s network;

	menuradiobutton_s hdr;
	menuradiobutton_s ssao;
	menuradiobutton_s sunShadows;
	menulist_s sunlightMode;

	menubitmap_s apply;
	menubitmap_s back;

	int hdr_original;
	int ssao_original;
	int sunShadows_original;
	int sunlightMode_original;
} effectsOptionsInfo_t;

static effectsOptionsInfo_t effectsOptionsInfo;

/*
=================
UI_EffectsOptions_SetMenuItems
=================
*/
static void UI_EffectsOptions_SetMenuItems(void) {

	effectsOptionsInfo.hdr_original = UI_GetCvarInt("r_hdr");
	effectsOptionsInfo.hdr.curvalue = effectsOptionsInfo.hdr_original;

	effectsOptionsInfo.ssao_original = UI_GetCvarInt("r_ssao");
	effectsOptionsInfo.ssao.curvalue = effectsOptionsInfo.ssao_original;

	effectsOptionsInfo.sunShadows_original = UI_GetCvarInt("r_sunShadows");
	effectsOptionsInfo.sunShadows.curvalue = effectsOptionsInfo.sunShadows_original;

	if (UI_GetCvarInt("r_sunlightMode") > 1) {
		effectsOptionsInfo.sunlightMode_original = 2;
	} else if (UI_GetCvarInt("r_sunlightMode") < 1) {
		effectsOptionsInfo.sunlightMode_original = 0;
	} else {
		effectsOptionsInfo.sunlightMode_original = 1;
	}
	effectsOptionsInfo.sunlightMode.curvalue = effectsOptionsInfo.sunlightMode_original;
}

/*
===============
UI_EffectsOptions_UpdateMenuItems
===============
*/
static void UI_EffectsOptions_UpdateMenuItems(void) {

	effectsOptionsInfo.apply.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
	if (effectsOptionsInfo.hdr_original != effectsOptionsInfo.hdr.curvalue) {
		effectsOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (effectsOptionsInfo.ssao_original != effectsOptionsInfo.ssao.curvalue) {
		effectsOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (effectsOptionsInfo.sunShadows_original != effectsOptionsInfo.sunShadows.curvalue) {
		effectsOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (effectsOptionsInfo.sunlightMode_original != effectsOptionsInfo.sunlightMode.curvalue) {
		effectsOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	}
}

/*
=================
UI_EffectsOptions_Event
=================
*/
static void UI_EffectsOptions_Event(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_HDR:
	case ID_SSAO:
	case ID_SUNSHADOWS:
	case ID_SUNLIGHTMODE:
		break;

	case ID_GRAPHICS:
		UI_PopMenu();
		UI_GraphicsOptionsMenu();
		break;

	case ID_DISPLAY:
		UI_PopMenu();
		UI_DisplayOptionsMenu();
		break;

	case ID_SOUND:
		UI_PopMenu();
		UI_SoundOptionsMenu();
		break;

	case ID_NETWORK:
		UI_PopMenu();
		UI_NetworkOptionsMenu();
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	
	case ID_APPLY:
		if ((effectsOptionsInfo.hdr_original != effectsOptionsInfo.hdr.curvalue) ||
			(effectsOptionsInfo.ssao_original != effectsOptionsInfo.ssao.curvalue) ||
			(effectsOptionsInfo.sunShadows_original != effectsOptionsInfo.sunShadows.curvalue) ||
			(effectsOptionsInfo.sunlightMode_original != effectsOptionsInfo.sunlightMode.curvalue)) {

			trap_Cvar_SetValue("r_hdr", effectsOptionsInfo.hdr.curvalue);
			trap_Cvar_SetValue("r_ssao", effectsOptionsInfo.ssao.curvalue);
			trap_Cvar_SetValue("r_sunShadows", effectsOptionsInfo.sunShadows.curvalue);
			trap_Cvar_SetValue("r_sunlightMode", effectsOptionsInfo.sunlightMode.curvalue);

			if (effectsOptionsInfo.sunlightMode.curvalue > 1) {
				trap_Cvar_SetValue("r_sunlightMode", 2);
			} else if (effectsOptionsInfo.sunlightMode.curvalue < 1) {
				trap_Cvar_SetValue("r_sunlightMode", 0);
			} else {
				trap_Cvar_SetValue("r_sunlightMode", 1);
			}

			UI_ForceMenuOff();
			trap_Cmd_ExecuteText(EXEC_APPEND, "vid_restart\n");
		}
		break;
	}
}

/*
===============
UI_EffectsOptions_MenuDraw
===============
*/
static void UI_EffectsOptions_MenuDraw(void) {
	UI_EffectsOptions_UpdateMenuItems();
	Menu_Draw(&effectsOptionsInfo.menu);
}

/*
===============
UI_EffectsOptions_MenuInit
===============
*/
static void UI_EffectsOptions_MenuInit(void) {
	static const char *sunlightMode_items[] = {"Off", "Dynamic", "Hybrid", NULL};
	int y;

	memset(&effectsOptionsInfo, 0, sizeof(effectsOptionsInfo));

	UI_EffectsOptions_Cache();
	effectsOptionsInfo.menu.wrapAround = qtrue;
	effectsOptionsInfo.menu.fullscreen = qtrue;
	effectsOptionsInfo.menu.draw = UI_EffectsOptions_MenuDraw;
	effectsOptionsInfo.menu.bgparts = BGP_SYSTEM | BGP_MENUFX;

	effectsOptionsInfo.graphics.generic.type = MTYPE_BITMAP;
	effectsOptionsInfo.graphics.generic.name = GRAPHICS0;
	effectsOptionsInfo.graphics.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	effectsOptionsInfo.graphics.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.graphics.generic.id = ID_GRAPHICS;
	effectsOptionsInfo.graphics.generic.x = 16;
	effectsOptionsInfo.graphics.generic.y = 37;
	effectsOptionsInfo.graphics.width = 160;
	effectsOptionsInfo.graphics.height = 40;
	effectsOptionsInfo.graphics.focuspic = GRAPHICS1;
	effectsOptionsInfo.graphics.focuspicinstead = qtrue;

	effectsOptionsInfo.display.generic.type = MTYPE_BITMAP;
	effectsOptionsInfo.display.generic.name = DISPLAY0;
	effectsOptionsInfo.display.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT;
	effectsOptionsInfo.display.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.display.generic.id = ID_DISPLAY;
	effectsOptionsInfo.display.generic.x = 169;
	effectsOptionsInfo.display.generic.y = 30;
	effectsOptionsInfo.display.width = 120;
	effectsOptionsInfo.display.height = 40;
	effectsOptionsInfo.display.focuspic = DISPLAY1;
	effectsOptionsInfo.display.focuspicinstead = qtrue;

	effectsOptionsInfo.sound.generic.type = MTYPE_BITMAP;
	effectsOptionsInfo.sound.generic.name = SOUND0;
	effectsOptionsInfo.sound.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	effectsOptionsInfo.sound.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.sound.generic.id = ID_SOUND;
	effectsOptionsInfo.sound.generic.x = 36;
	effectsOptionsInfo.sound.generic.y = 79;
	effectsOptionsInfo.sound.width = 120;
	effectsOptionsInfo.sound.height = 40;
	effectsOptionsInfo.sound.focuspic = SOUND1;
	effectsOptionsInfo.sound.focuspicinstead = qtrue;

	effectsOptionsInfo.network.generic.type = MTYPE_BITMAP;
	effectsOptionsInfo.network.generic.name = NETWORK0;
	effectsOptionsInfo.network.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	effectsOptionsInfo.network.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.network.generic.id = ID_NETWORK;
	effectsOptionsInfo.network.generic.x = 142;
	effectsOptionsInfo.network.generic.y = 82;
	effectsOptionsInfo.network.width = 160;
	effectsOptionsInfo.network.height = 40;
	effectsOptionsInfo.network.focuspic = NETWORK1;
	effectsOptionsInfo.network.focuspicinstead = qtrue;

	y = YPOSITION;
	// references/modifies "r_hdr"
	effectsOptionsInfo.hdr.generic.type = MTYPE_RADIOBUTTON;
	effectsOptionsInfo.hdr.generic.name = "High Dynamic Range:";
	effectsOptionsInfo.hdr.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.hdr.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.hdr.generic.id = ID_HDR;
	effectsOptionsInfo.hdr.generic.x = XPOSITION;
	effectsOptionsInfo.hdr.generic.y = y;
	effectsOptionsInfo.hdr.generic.toolTip =
		"";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_ssao"
	effectsOptionsInfo.ssao.generic.type = MTYPE_RADIOBUTTON;
	effectsOptionsInfo.ssao.generic.name = "Ambient Occlusion:";
	effectsOptionsInfo.ssao.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.ssao.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.ssao.generic.id = ID_SSAO;
	effectsOptionsInfo.ssao.generic.x = XPOSITION;
	effectsOptionsInfo.ssao.generic.y = y;
	effectsOptionsInfo.ssao.generic.toolTip =
		"";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_sunShadows"
	effectsOptionsInfo.sunShadows.generic.type = MTYPE_RADIOBUTTON;
	effectsOptionsInfo.sunShadows.generic.name = "Sunlight & Shadows:";
	effectsOptionsInfo.sunShadows.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.sunShadows.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.sunShadows.generic.id = ID_SUNSHADOWS;
	effectsOptionsInfo.sunShadows.generic.x = XPOSITION;
	effectsOptionsInfo.sunShadows.generic.y = y;
	effectsOptionsInfo.sunShadows.generic.toolTip =
		"";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_sunlightMode"
	effectsOptionsInfo.sunlightMode.generic.type = MTYPE_SPINCONTROL;
	effectsOptionsInfo.sunlightMode.generic.name = "Sunlight Mode:";
	effectsOptionsInfo.sunlightMode.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.sunlightMode.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.sunlightMode.generic.id = ID_SUNLIGHTMODE;
	effectsOptionsInfo.sunlightMode.itemnames = sunlightMode_items;
	effectsOptionsInfo.sunlightMode.generic.x = XPOSITION;
	effectsOptionsInfo.sunlightMode.generic.y = y;
	effectsOptionsInfo.sunlightMode.generic.toolTip =
		"";

	effectsOptionsInfo.apply.generic.type = MTYPE_BITMAP;
	effectsOptionsInfo.apply.generic.name = ACCEPT0;
	effectsOptionsInfo.apply.generic.flags = QMF_PULSEIFFOCUS | QMF_HIDDEN | QMF_INACTIVE;
	effectsOptionsInfo.apply.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.apply.generic.id = ID_APPLY;
	effectsOptionsInfo.apply.generic.x = 512;
	effectsOptionsInfo.apply.generic.y = 440;
	effectsOptionsInfo.apply.width = 120;
	effectsOptionsInfo.apply.height = 40;
	effectsOptionsInfo.apply.focuspic = ACCEPT1;

	effectsOptionsInfo.back.generic.type = MTYPE_BITMAP;
	effectsOptionsInfo.back.generic.name = BACK0;
	effectsOptionsInfo.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	effectsOptionsInfo.back.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.back.generic.id = ID_BACK;
	effectsOptionsInfo.back.generic.x = 8;
	effectsOptionsInfo.back.generic.y = 440;
	effectsOptionsInfo.back.width = 80;
	effectsOptionsInfo.back.height = 40;
	effectsOptionsInfo.back.focuspic = BACK1;
	effectsOptionsInfo.back.focuspicinstead = qtrue;

	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.graphics);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.display);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.sound);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.network);

	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.hdr);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.ssao);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.sunShadows);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.sunlightMode);

	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.apply);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.back);

	UI_EffectsOptions_SetMenuItems();
}

/*
===============
UI_EffectsOptions_Cache
===============
*/
void UI_EffectsOptions_Cache(void) {
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
	trap_R_RegisterShaderNoMip(GRAPHICS0);
	trap_R_RegisterShaderNoMip(GRAPHICS1);
	trap_R_RegisterShaderNoMip(DISPLAY0);
	trap_R_RegisterShaderNoMip(DISPLAY1);
	trap_R_RegisterShaderNoMip(SOUND0);
	trap_R_RegisterShaderNoMip(SOUND1);
	trap_R_RegisterShaderNoMip(NETWORK0);
	trap_R_RegisterShaderNoMip(NETWORK1);
	trap_R_RegisterShaderNoMip(ACCEPT0);
	trap_R_RegisterShaderNoMip(ACCEPT1);
}

/*
===============
UI_EffectsOptionsMenu
===============
*/
void UI_EffectsOptionsMenu(void) {
	UI_EffectsOptions_MenuInit();
	UI_PushMenu(&effectsOptionsInfo.menu);
	Menu_SetCursorToItem(&effectsOptionsInfo.menu, &effectsOptionsInfo.graphics);
}
