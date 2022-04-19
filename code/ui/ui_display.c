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

DISPLAY OPTIONS MENU

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

#define ID_IGNOREHWG 14
#define ID_BRIGHTNESS 15
#define ID_SCREENSIZE 16
#define ID_VSYNC 17
#define ID_WINDOWMODE 18
#define ID_RESIZE 19
#define ID_MAXFPS 20
#define ID_ANAGLYPH 21
#define ID_GREYSCALE 22

#define XPOSITION 180
#define YPOSITION 180 + 36

typedef struct {
	menuframework_s menu;

	menubitmap_s graphics;
	menubitmap_s display;
	menubitmap_s sound;
	menubitmap_s network;

	menuradiobutton_s ignoreHWG;
	menuslider_s brightness;
	menuslider_s screensize;
	menuradiobutton_s vsync;
	menulist_s windowmode;	
	menuradiobutton_s resize;
	menuradiobutton_s maxfps;
	menulist_s anaglyph;
	menuslider_s greyscale;

	menubitmap_s apply;
	menubitmap_s back;

	int vsync_original;
	int windowmode_original;
	int resize_original;

} displayOptionsInfo_t;

static displayOptionsInfo_t displayOptionsInfo;

static const char *wm_names[] = {"Off (Fullscreen)", "On (Border)", "On (No Border)", NULL};

static const char *anaglyph_names[] = {"Off", "Red-Cyan", "Red-Blue", "Red-Green", "Green-Magenta", 
									   "Cyan-Red", "Blue-Red", "Green-Red", "Magenta-Green", NULL};

static void ApplyPressed(void *unused, int notification) {
	if (notification != QM_ACTIVATED)
		return;

	trap_Cvar_SetValue("r_ignorehwgamma", (float)displayOptionsInfo.ignoreHWG.curvalue);
	trap_Cvar_SetValue("r_swapInterval", displayOptionsInfo.vsync.curvalue);

	if (displayOptionsInfo.windowmode.curvalue == 2) {
		trap_Cvar_SetValue("r_fullscreen", 0);
		trap_Cvar_SetValue("r_noborder", 1);
	} else if (displayOptionsInfo.windowmode.curvalue == 1) {
		trap_Cvar_SetValue("r_fullscreen", 0);
		trap_Cvar_SetValue("r_noborder", 0);
	} else {
		trap_Cvar_SetValue("r_fullscreen", 1);
		trap_Cvar_SetValue("r_noborder", 0);
	}

	trap_Cvar_SetValue("r_allowResize", displayOptionsInfo.resize.curvalue);
	trap_Cvar_SetValue("r_greyscale", (displayOptionsInfo.greyscale.curvalue / 100.0f));

	// hide the button and do the vid restart
	displayOptionsInfo.apply.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
	trap_Cmd_ExecuteText(EXEC_APPEND, "vid_restart\n");
}

/*
=================
UI_DisplayOptionsMenu_Event
=================
*/
static void UI_DisplayOptionsMenu_Event(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_DISPLAY:
	case ID_IGNOREHWG:
	case ID_VSYNC:
	case ID_WINDOWMODE:
	case ID_RESIZE:
	case ID_GREYSCALE:
		break;

	case ID_GRAPHICS:
		UI_PopMenu();
		UI_GraphicsOptionsMenu();
		break;

	case ID_SOUND:
		UI_PopMenu();
		UI_SoundOptionsMenu();
		break;

	case ID_NETWORK:
		UI_PopMenu();
		UI_NetworkOptionsMenu();
		break;

	case ID_BRIGHTNESS:
		trap_Cvar_SetValue("r_gamma", displayOptionsInfo.brightness.curvalue / 10.0f);
		break;

	case ID_SCREENSIZE:
		trap_Cvar_SetValue("cg_viewsize", displayOptionsInfo.screensize.curvalue * 10);
		break;

	case ID_MAXFPS:
		switch (displayOptionsInfo.maxfps.curvalue) {
		case 0:
			trap_Cvar_SetValue("com_maxfpsUnfocused", 0);
			trap_Cvar_SetValue("com_maxfpsMinimized", 0);
			break;
		case 1:
			trap_Cvar_SetValue("com_maxfpsUnfocused", 30);
			trap_Cvar_SetValue("com_maxfpsMinimized", 5);
			break;
		}
		break;

	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_ANAGLYPH:
		trap_Cvar_SetValue("r_anaglyphMode", (float)displayOptionsInfo.anaglyph.curvalue);
		if (!displayOptionsInfo.anaglyph.curvalue) {
			displayOptionsInfo.greyscale.generic.flags |= QMF_GRAYED;
		} else {
			displayOptionsInfo.greyscale.generic.flags &= ~QMF_GRAYED;
		}
		break;
	}
}

static void DisplayOptions_UpdateMenuItems(void) {
	displayOptionsInfo.apply.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);

	if (displayOptionsInfo.windowmode_original != displayOptionsInfo.windowmode.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (displayOptionsInfo.vsync_original != displayOptionsInfo.vsync.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (displayOptionsInfo.resize_original != displayOptionsInfo.resize.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (UI_GetCvarInt("r_ignorehwgamma") != displayOptionsInfo.ignoreHWG.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (UI_GetCvarInt("r_greyscale") != displayOptionsInfo.greyscale.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	}
}

static void DisplayOptions_MenuDraw(void) {
	DisplayOptions_UpdateMenuItems();

	Menu_Draw(&displayOptionsInfo.menu);
}

/*
===============
UI_DisplayOptionsMenu_Init
===============
*/
static void UI_DisplayOptionsMenu_Init(void) {
	int y;

	memset(&displayOptionsInfo, 0, sizeof(displayOptionsInfo));

	UI_DisplayOptionsMenu_Cache();
	displayOptionsInfo.menu.wrapAround = qtrue;
	displayOptionsInfo.menu.fullscreen = qtrue;
	displayOptionsInfo.menu.draw = DisplayOptions_MenuDraw;
	displayOptionsInfo.menu.bgparts = BGP_SYSTEM | BGP_MENUFX;

	displayOptionsInfo.graphics.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.graphics.generic.name = GRAPHICS0;
	displayOptionsInfo.graphics.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.graphics.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.graphics.generic.id = ID_GRAPHICS;
	displayOptionsInfo.graphics.generic.x = 16;
	displayOptionsInfo.graphics.generic.y = 37;
	displayOptionsInfo.graphics.width = 160;
	displayOptionsInfo.graphics.height = 40;
	displayOptionsInfo.graphics.focuspic = GRAPHICS1;
	displayOptionsInfo.graphics.focuspicinstead = qtrue;

	displayOptionsInfo.display.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.display.generic.name = DISPLAY0;
	displayOptionsInfo.display.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT;
	displayOptionsInfo.display.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.display.generic.id = ID_DISPLAY;
	displayOptionsInfo.display.generic.x = 169;
	displayOptionsInfo.display.generic.y = 30;
	displayOptionsInfo.display.width = 120;
	displayOptionsInfo.display.height = 40;
	displayOptionsInfo.display.focuspic = DISPLAY1;
	displayOptionsInfo.display.focuspicinstead = qtrue;

	displayOptionsInfo.sound.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.sound.generic.name = SOUND0;
	displayOptionsInfo.sound.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.sound.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.sound.generic.id = ID_SOUND;
	displayOptionsInfo.sound.generic.x = 36;
	displayOptionsInfo.sound.generic.y = 79;
	displayOptionsInfo.sound.width = 120;
	displayOptionsInfo.sound.height = 40;
	displayOptionsInfo.sound.focuspic = SOUND1;
	displayOptionsInfo.sound.focuspicinstead = qtrue;

	displayOptionsInfo.network.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.network.generic.name = NETWORK0;
	displayOptionsInfo.network.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.network.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.network.generic.id = ID_NETWORK;
	displayOptionsInfo.network.generic.x = 142;
	displayOptionsInfo.network.generic.y = 82;
	displayOptionsInfo.network.width = 160;
	displayOptionsInfo.network.height = 40;
	displayOptionsInfo.network.focuspic = NETWORK1;
	displayOptionsInfo.network.focuspicinstead = qtrue;

	y = YPOSITION;
	displayOptionsInfo.ignoreHWG.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.ignoreHWG.generic.name = "Ignore HW Gamma:";
	displayOptionsInfo.ignoreHWG.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.ignoreHWG.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.ignoreHWG.generic.id = ID_IGNOREHWG;
	displayOptionsInfo.ignoreHWG.generic.x = XPOSITION;
	displayOptionsInfo.ignoreHWG.generic.y = y;
	displayOptionsInfo.ignoreHWG.generic.toolTip =
		"Enable to control the brightness via your current graphics card and monitor options. "
		"Default is off. NOTE: This will disable the brightness slider and you will no longer "
		"be able to adjust the brightness ingame if necessary.";

	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.brightness.generic.type = MTYPE_SLIDER;
	displayOptionsInfo.brightness.generic.name = "Brightness:";
	displayOptionsInfo.brightness.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.brightness.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.brightness.generic.id = ID_BRIGHTNESS;
	displayOptionsInfo.brightness.generic.x = XPOSITION;
	displayOptionsInfo.brightness.generic.y = y;
	displayOptionsInfo.brightness.minvalue = 5;
	displayOptionsInfo.brightness.maxvalue = 20;
	if (!uis.glconfig.deviceSupportsGamma)
		displayOptionsInfo.brightness.generic.flags |= QMF_GRAYED;

	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.screensize.generic.type = MTYPE_SLIDER;
	displayOptionsInfo.screensize.generic.name = "Screen Size:";
	displayOptionsInfo.screensize.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.screensize.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.screensize.generic.id = ID_SCREENSIZE;
	displayOptionsInfo.screensize.generic.x = XPOSITION;
	displayOptionsInfo.screensize.generic.y = y;
	displayOptionsInfo.screensize.minvalue = 3;
    displayOptionsInfo.screensize.maxvalue = 10;

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.vsync.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.vsync.generic.name = "Vertical Sync:";
	displayOptionsInfo.vsync.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.vsync.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.vsync.generic.id = ID_VSYNC;
	displayOptionsInfo.vsync.generic.x = XPOSITION;
	displayOptionsInfo.vsync.generic.y = y;
	displayOptionsInfo.vsync.generic.toolTip =
		"Enable to force the game to follow the refresh rate of your monitor (VSync), "
		"which may reduce image tearing problems. Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.windowmode.generic.type = MTYPE_SPINCONTROL;
	displayOptionsInfo.windowmode.generic.name = "Window Mode:";
	displayOptionsInfo.windowmode.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.windowmode.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.windowmode.generic.id = ID_WINDOWMODE;
	displayOptionsInfo.windowmode.generic.x = XPOSITION;
	displayOptionsInfo.windowmode.generic.y = y;
	displayOptionsInfo.windowmode.itemnames = wm_names;
	displayOptionsInfo.windowmode.generic.toolTip =
		"Enable to play the game in a window. Change video mode in graphics options section "
		"to change the size of the window, when resizable window option is disabled. Choose "
		"no border to remove window decoration like borders and titlebar. Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.resize.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.resize.generic.name = "Resizable Window:";
	displayOptionsInfo.resize.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	displayOptionsInfo.resize.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.resize.generic.id = ID_RESIZE;
	displayOptionsInfo.resize.generic.x = XPOSITION;
	displayOptionsInfo.resize.generic.y = y;
	displayOptionsInfo.resize.generic.toolTip =
		"Enable to make the game window resizable in window mode. Default is off. NOTE: To reset "
		"the window size and game resolution select another video mode in graphics options section." ;

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.maxfps.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.maxfps.generic.name = "Limit Frame Rate:";
	displayOptionsInfo.maxfps.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	displayOptionsInfo.maxfps.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.maxfps.generic.id = ID_MAXFPS;
	displayOptionsInfo.maxfps.generic.x = XPOSITION;
	displayOptionsInfo.maxfps.generic.y = y;
	displayOptionsInfo.maxfps.generic.toolTip =
		"Enable to limit the frame rate when the game window loses focus (30 fps) or is minimized "
		"(5 fps). Default is off. NOTE: This may help to reduce your graphics card load while you "
		"are not playing the game.";

	y += 2 * (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.anaglyph.generic.type = MTYPE_SPINCONTROL;
	displayOptionsInfo.anaglyph.generic.name = "Anaglyph 3D Mode:";
	displayOptionsInfo.anaglyph.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.anaglyph.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.anaglyph.generic.id = ID_ANAGLYPH;
	displayOptionsInfo.anaglyph.generic.x = XPOSITION;
	displayOptionsInfo.anaglyph.generic.y = y;
	displayOptionsInfo.anaglyph.itemnames = anaglyph_names;
	displayOptionsInfo.anaglyph.generic.toolTip =
		"Enable to play the game in anaglyph 3D mode with the appriopriate glasses. Default is "
		"off. NOTE: Ensure the correct filter option you select matches that of your glasses.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.greyscale.generic.type = MTYPE_SLIDER;
	displayOptionsInfo.greyscale.generic.name = "Greyscale:";
	displayOptionsInfo.greyscale.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.greyscale.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.greyscale.generic.id = ID_GREYSCALE;
	displayOptionsInfo.greyscale.generic.x = XPOSITION;
	displayOptionsInfo.greyscale.generic.y = y;
	displayOptionsInfo.greyscale.minvalue = 0;
	displayOptionsInfo.greyscale.maxvalue = 100;

	displayOptionsInfo.apply.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.apply.generic.name = ACCEPT0;
	displayOptionsInfo.apply.generic.flags = QMF_PULSEIFFOCUS | QMF_HIDDEN | QMF_INACTIVE;
	displayOptionsInfo.apply.generic.callback = ApplyPressed;
	displayOptionsInfo.apply.generic.x = 512;
	displayOptionsInfo.apply.generic.y = 440;
	displayOptionsInfo.apply.width = 120;
	displayOptionsInfo.apply.height = 40;
	displayOptionsInfo.apply.focuspic = ACCEPT1;

	displayOptionsInfo.back.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.back.generic.name = BACK0;
	displayOptionsInfo.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	displayOptionsInfo.back.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.back.generic.id = ID_BACK;
	displayOptionsInfo.back.generic.x = 8;
	displayOptionsInfo.back.generic.y = 440;
	displayOptionsInfo.back.width = 80;
	displayOptionsInfo.back.height = 40;
	displayOptionsInfo.back.focuspic = BACK1;
	displayOptionsInfo.back.focuspicinstead = qtrue;

	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.graphics);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.display);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.sound);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.network);

	Menu_AddItem(&displayOptionsInfo.menu, &displayOptionsInfo.ignoreHWG);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.brightness);
//	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.screensize);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.vsync);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.windowmode);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.resize);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.maxfps);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.anaglyph);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.greyscale);

	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.apply);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.back);

	displayOptionsInfo.ignoreHWG.curvalue = UI_GetCvarInt("r_ignorehwgamma");
	displayOptionsInfo.brightness.curvalue = trap_Cvar_VariableValue("r_gamma") * 10;
	displayOptionsInfo.screensize.curvalue = trap_Cvar_VariableValue("cg_viewsize") / 10;

	displayOptionsInfo.vsync_original = trap_Cvar_VariableValue("r_swapInterval");
	displayOptionsInfo.vsync.curvalue = displayOptionsInfo.vsync_original;

	if (trap_Cvar_VariableValue("r_fullscreen") == 0) {
		if (trap_Cvar_VariableValue("r_noborder") == 1) {
			displayOptionsInfo.windowmode_original = 2;
		} else {
			displayOptionsInfo.windowmode_original = 1;
		}
	} else {
		displayOptionsInfo.windowmode_original = 0;
	}
	displayOptionsInfo.windowmode.curvalue = displayOptionsInfo.windowmode_original;
	
	displayOptionsInfo.resize_original = trap_Cvar_VariableValue("r_allowResize");
	displayOptionsInfo.resize.curvalue = displayOptionsInfo.resize_original;

	if ((trap_Cvar_VariableValue("com_maxfpsUnfocused") != 0 || trap_Cvar_VariableValue("com_maxfpsMinimized") != 0)) {
		displayOptionsInfo.maxfps.curvalue = 1;
	} else {
		displayOptionsInfo.maxfps.curvalue = 0;
	}

	displayOptionsInfo.anaglyph.curvalue =
		Com_Clamp(0, (ARRAY_LEN(anaglyph_names) - 1), trap_Cvar_VariableValue("r_anaglyphMode"));
	displayOptionsInfo.greyscale.curvalue = Com_Clamp(0, 100, (trap_Cvar_VariableValue("r_greyscale") * 100));

	if (!displayOptionsInfo.anaglyph.curvalue) {
		displayOptionsInfo.greyscale.generic.flags |= QMF_GRAYED;
	}
}

/*
===============
UI_DisplayOptionsMenu_Cache
===============
*/
void UI_DisplayOptionsMenu_Cache(void) {
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
}

/*
===============
UI_DisplayOptionsMenu
===============
*/
void UI_DisplayOptionsMenu(void) {
	UI_DisplayOptionsMenu_Init();
	UI_PushMenu(&displayOptionsInfo.menu);
	Menu_SetCursorToItem(&displayOptionsInfo.menu, &displayOptionsInfo.display);
}
