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
#define ID_APPLY 105

#define ID_IGNOREHWG 14
#define ID_BRIGHTNESS 15
#define ID_SCREENSIZE 16
#define ID_VSYNC 17
#define ID_WINDOWMODE 18
#define ID_RESIZE 19
#define ID_MAXFPS 20
#define ID_SCREENSHOTFORMAT 21
#define ID_SSJPEGQUALITY 22
#define ID_ANAGLYPH 23
#define ID_SWAPCOLORS 24
#define ID_GREYSCALE 25

#define XPOSITION 220
#define YPOSITION 198

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
	menulist_s screenshotformat;
	menuslider_s ssjpegquality;
	menulist_s anaglyph;
	menuradiobutton_s swapcolors;
	menuslider_s greyscale;

	menubitmap_s apply;
	menubitmap_s back;

	int ignorehwg_original;
	int vsync_original;
	int windowmode_original;
	int resize_original;
	float greyscale_original;
} displayOptionsInfo_t;

static displayOptionsInfo_t displayOptionsInfo;

static const char *wm_names[] = {"Off (Fullscreen)", "On (Border)", "On (No Border)", NULL};
static const char *screenshot_formats[] = {"TGA Image", "JPG Image", "PNG Image", NULL};
static const char *anaglyph_names[] = {"Off", "Red-Cyan", "Red-Blue", "Red-Green", "Green-Magenta", NULL};

/*
=================
UI_DisplayOptions_SetMenuItems
=================
*/
static void UI_DisplayOptions_SetMenuItems(void) {
	int anaglyphMode;

	displayOptionsInfo.ignorehwg_original = UI_GetCvarInt("r_ignorehwgamma");
	displayOptionsInfo.ignoreHWG.curvalue = displayOptionsInfo.ignorehwg_original;

	displayOptionsInfo.brightness.curvalue = trap_Cvar_VariableValue("r_gamma") * 100.0f;
	displayOptionsInfo.screensize.curvalue = trap_Cvar_VariableValue("cg_viewsize");

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

	displayOptionsInfo.screenshotformat.curvalue = trap_Cvar_VariableValue("r_screenshotFormat");
	displayOptionsInfo.ssjpegquality.curvalue = trap_Cvar_VariableValue("r_screenshotJpegQuality");

	anaglyphMode = Com_Clamp(0, 8, trap_Cvar_VariableValue("r_anaglyphMode"));
	if (anaglyphMode > 4) {
		anaglyphMode -= 4;
		displayOptionsInfo.swapcolors.curvalue = 1;
	} else {
		displayOptionsInfo.swapcolors.curvalue = 0;
	}
	displayOptionsInfo.anaglyph.curvalue = anaglyphMode;

	displayOptionsInfo.greyscale_original = Com_Clamp(0, 100, (trap_Cvar_VariableValue("r_greyscale") * 100.0f));
	displayOptionsInfo.greyscale.curvalue = displayOptionsInfo.greyscale_original;
}

/*
===============
UI_DisplayOptions_UpdateMenuItems
===============
*/
static void UI_DisplayOptions_UpdateMenuItems(void) {

	if (!uis.glconfig.deviceSupportsGamma) {
		displayOptionsInfo.brightness.generic.flags |= QMF_GRAYED;
	} else {
		displayOptionsInfo.brightness.generic.flags &= ~QMF_GRAYED;
	}

	if (displayOptionsInfo.screenshotformat.curvalue != 1) {
		displayOptionsInfo.ssjpegquality.generic.flags |= QMF_GRAYED;
	} else {
		displayOptionsInfo.ssjpegquality.generic.flags &= ~QMF_GRAYED;
	}

	if (!displayOptionsInfo.anaglyph.curvalue) {
		displayOptionsInfo.greyscale.generic.flags |= QMF_GRAYED;
		displayOptionsInfo.swapcolors.generic.flags |= QMF_GRAYED;
	} else {
		displayOptionsInfo.greyscale.generic.flags &= ~QMF_GRAYED;
		displayOptionsInfo.swapcolors.generic.flags &= ~QMF_GRAYED;
	}

	displayOptionsInfo.apply.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
	if (displayOptionsInfo.ignorehwg_original != displayOptionsInfo.ignoreHWG.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (displayOptionsInfo.windowmode_original != displayOptionsInfo.windowmode.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (displayOptionsInfo.vsync_original != displayOptionsInfo.vsync.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (displayOptionsInfo.resize_original != displayOptionsInfo.resize.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (displayOptionsInfo.greyscale_original != displayOptionsInfo.greyscale.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	}
}

/*
=================
UI_DisplayOptions_Event
=================
*/
static void UI_DisplayOptions_Event(void *ptr, int event) {
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
		trap_Cvar_SetValue("r_gamma", (float)((int)displayOptionsInfo.brightness.curvalue / 100));
		break;

	case ID_SCREENSIZE:
		trap_Cvar_SetValue("cg_viewsize", (int)displayOptionsInfo.screensize.curvalue);
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

	case ID_SCREENSHOTFORMAT:
		switch (displayOptionsInfo.screenshotformat.curvalue) {
		case 0:	//TGA
			trap_Cvar_SetValue("r_screenshotFormat", 0);
			break;
		case 1:	//JPG
			trap_Cvar_SetValue("r_screenshotFormat", 1);
			break;
		case 2:	//PNG
			trap_Cvar_SetValue("r_screenshotFormat", 2);
			break;
		}
		break;

	case ID_SSJPEGQUALITY:
		trap_Cvar_SetValue("r_screenshotJpegQuality", (int)displayOptionsInfo.ssjpegquality.curvalue);
		break;

	case ID_ANAGLYPH:
	case ID_SWAPCOLORS:
		if ((displayOptionsInfo.anaglyph.curvalue != 0) && (displayOptionsInfo.swapcolors.curvalue != 0)) {
			trap_Cvar_SetValue("r_anaglyphMode", displayOptionsInfo.anaglyph.curvalue + 4);
		} else {
			trap_Cvar_SetValue("r_anaglyphMode", displayOptionsInfo.anaglyph.curvalue);
		}
		break;

	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_APPLY:
		if (displayOptionsInfo.ignorehwg_original != displayOptionsInfo.ignoreHWG.curvalue ||
			displayOptionsInfo.windowmode_original != displayOptionsInfo.windowmode.curvalue ||
			displayOptionsInfo.vsync_original != displayOptionsInfo.vsync.curvalue ||
			displayOptionsInfo.resize_original != displayOptionsInfo.resize.curvalue ||
			displayOptionsInfo.greyscale_original != displayOptionsInfo.greyscale.curvalue) {

			trap_Cvar_SetValue("r_ignorehwgamma", displayOptionsInfo.ignoreHWG.curvalue);
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
			trap_Cvar_SetValue("r_greyscale", (float)((int)displayOptionsInfo.greyscale.curvalue / 100));

			UI_ForceMenuOff();
			trap_Cmd_ExecuteText(EXEC_APPEND, "vid_restart\n");
		}
		break;
	}
}

/*
===============
UI_DisplayOptions_MenuDraw
===============
*/
static void UI_DisplayOptions_MenuDraw(void) {
	UI_DisplayOptions_UpdateMenuItems();
	Menu_Draw(&displayOptionsInfo.menu);
}

/*
===============
UI_DisplayOptions_MenuInit
===============
*/
static void UI_DisplayOptions_MenuInit(void) {
	int y;

	memset(&displayOptionsInfo, 0, sizeof(displayOptionsInfo));

	UI_DisplayOptions_Cache();
	displayOptionsInfo.menu.wrapAround = qtrue;
	displayOptionsInfo.menu.fullscreen = qtrue;
	displayOptionsInfo.menu.draw = UI_DisplayOptions_MenuDraw;
	displayOptionsInfo.menu.bgparts = BGP_SYSTEM | BGP_MENUFX;

	displayOptionsInfo.graphics.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.graphics.generic.name = GRAPHICS0;
	displayOptionsInfo.graphics.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.graphics.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.graphics.generic.id = ID_GRAPHICS;
	displayOptionsInfo.graphics.generic.x = XPOSITION - 144;
	displayOptionsInfo.graphics.generic.y = 43;
	displayOptionsInfo.graphics.width = 160;
	displayOptionsInfo.graphics.height = 40;
	displayOptionsInfo.graphics.focuspic = GRAPHICS1;
	displayOptionsInfo.graphics.focuspicinstead = qtrue;

	displayOptionsInfo.display.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.display.generic.name = DISPLAY0;
	displayOptionsInfo.display.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT;
	displayOptionsInfo.display.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.display.generic.id = ID_DISPLAY;
	displayOptionsInfo.display.generic.x = XPOSITION + 9;
	displayOptionsInfo.display.generic.y = 36;
	displayOptionsInfo.display.width = 120;
	displayOptionsInfo.display.height = 40;
	displayOptionsInfo.display.focuspic = DISPLAY1;
	displayOptionsInfo.display.focuspicinstead = qtrue;

	displayOptionsInfo.sound.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.sound.generic.name = SOUND0;
	displayOptionsInfo.sound.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.sound.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.sound.generic.id = ID_SOUND;
	displayOptionsInfo.sound.generic.x = XPOSITION - 124;
	displayOptionsInfo.sound.generic.y = 85;
	displayOptionsInfo.sound.width = 120;
	displayOptionsInfo.sound.height = 40;
	displayOptionsInfo.sound.focuspic = SOUND1;
	displayOptionsInfo.sound.focuspicinstead = qtrue;

	displayOptionsInfo.network.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.network.generic.name = NETWORK0;
	displayOptionsInfo.network.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.network.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.network.generic.id = ID_NETWORK;
	displayOptionsInfo.network.generic.x = XPOSITION - 18;
	displayOptionsInfo.network.generic.y = 88;
	displayOptionsInfo.network.width = 160;
	displayOptionsInfo.network.height = 40;
	displayOptionsInfo.network.focuspic = NETWORK1;
	displayOptionsInfo.network.focuspicinstead = qtrue;

	y = YPOSITION;
	displayOptionsInfo.ignoreHWG.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.ignoreHWG.generic.name = "Ignore HW Gamma:";
	displayOptionsInfo.ignoreHWG.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.ignoreHWG.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.ignoreHWG.generic.id = ID_IGNOREHWG;
	displayOptionsInfo.ignoreHWG.generic.x = XPOSITION;
	displayOptionsInfo.ignoreHWG.generic.y = y;
	displayOptionsInfo.ignoreHWG.generic.toolTip =
		"Enable to control the brightness via your current graphics card and monitor options. "
		"Default is off. NOTE: This will disable the brightness slider and you will no longer "
		"be able to adjust the brightness ingame if necessary.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.brightness.generic.type = MTYPE_SLIDER;
	displayOptionsInfo.brightness.generic.name = "Brightness:";
	displayOptionsInfo.brightness.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.brightness.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.brightness.generic.id = ID_BRIGHTNESS;
	displayOptionsInfo.brightness.generic.x = XPOSITION;
	displayOptionsInfo.brightness.generic.y = y;
	displayOptionsInfo.brightness.minvalue = 50;
	displayOptionsInfo.brightness.maxvalue = 200;
	displayOptionsInfo.brightness.generic.toolTip =
		"Use this to adjust the brightness of the game to your needs. Default is 100. "
		"NOTE: With ignore hardware gamma enabled the brightness slider is disabled.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.screensize.generic.type = MTYPE_SLIDER;
	displayOptionsInfo.screensize.generic.name = "Screen Size:";
	displayOptionsInfo.screensize.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.screensize.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.screensize.generic.id = ID_SCREENSIZE;
	displayOptionsInfo.screensize.generic.x = XPOSITION;
	displayOptionsInfo.screensize.generic.y = y;
	displayOptionsInfo.screensize.minvalue = 30;
    displayOptionsInfo.screensize.maxvalue = 100;
	displayOptionsInfo.screensize.generic.toolTip =
		"Use this to adjust the size of the 3D world screen within the game window. "
		"This would allow to write part of HUD infos outside that part of the screen."
		"Default is 100. NOTE: A smaller screen size can save system resources.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.vsync.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.vsync.generic.name = "Vertical Sync:";
	displayOptionsInfo.vsync.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.vsync.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.vsync.generic.id = ID_VSYNC;
	displayOptionsInfo.vsync.generic.x = XPOSITION;
	displayOptionsInfo.vsync.generic.y = y;
	displayOptionsInfo.vsync.generic.toolTip =
		"Enable to force the game to follow the refresh rate of your monitor (VSync), "
		"which may reduce image tearing problems. Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.windowmode.generic.type = MTYPE_SPINCONTROL;
	displayOptionsInfo.windowmode.generic.name = "Window Mode:";
	displayOptionsInfo.windowmode.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.windowmode.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.windowmode.generic.id = ID_WINDOWMODE;
	displayOptionsInfo.windowmode.generic.x = XPOSITION;
	displayOptionsInfo.windowmode.generic.y = y;
	displayOptionsInfo.windowmode.itemnames = wm_names;
	displayOptionsInfo.windowmode.generic.toolTip =
		"Enable to play the game in a window. Change video mode in graphics options section "
		"to change the size of the window, when resizable window option is disabled. Select "
		"no border to remove window decoration like borders and titlebar. Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.resize.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.resize.generic.name = "Resizable Window:";
	displayOptionsInfo.resize.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.resize.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.resize.generic.id = ID_RESIZE;
	displayOptionsInfo.resize.generic.x = XPOSITION;
	displayOptionsInfo.resize.generic.y = y;
	displayOptionsInfo.resize.generic.toolTip =
		"Enable to make the game window resizable in window mode. Default is off. NOTE: To reset "
		"the window size and game resolution select another video mode in graphics options section.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.maxfps.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.maxfps.generic.name = "Limit Frame Rate:";
	displayOptionsInfo.maxfps.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.maxfps.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.maxfps.generic.id = ID_MAXFPS;
	displayOptionsInfo.maxfps.generic.x = XPOSITION;
	displayOptionsInfo.maxfps.generic.y = y;
	displayOptionsInfo.maxfps.generic.toolTip =
		"Enable to limit the frame rate when the game window loses focus (30 fps) or is minimized "
		"(5 fps). Default is off. NOTE: This may help to reduce your graphics card load while you "
		"are not playing the game.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.screenshotformat.generic.type = MTYPE_SPINCONTROL;
	displayOptionsInfo.screenshotformat.generic.name = "Save Screenshots As:";
	displayOptionsInfo.screenshotformat.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.screenshotformat.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.screenshotformat.generic.id = ID_SCREENSHOTFORMAT;
	displayOptionsInfo.screenshotformat.generic.x = XPOSITION;
	displayOptionsInfo.screenshotformat.generic.y = y;
	displayOptionsInfo.screenshotformat.itemnames = screenshot_formats;
	displayOptionsInfo.screenshotformat.generic.toolTip =
		"Select a desired screenshot image format. Default is PNG as best compromise between image "
		"quality and file size. TGA as a lossless format offers best quality but results in large "
		"files. JPG may result in smaller files due to compression if image quality is reduced.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.ssjpegquality.generic.type = MTYPE_SLIDER;
	displayOptionsInfo.ssjpegquality.generic.name = "JPEG Image Quality:";
	displayOptionsInfo.ssjpegquality.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.ssjpegquality.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.ssjpegquality.generic.id = ID_SSJPEGQUALITY;
	displayOptionsInfo.ssjpegquality.generic.x = XPOSITION;
	displayOptionsInfo.ssjpegquality.generic.y = y;
	displayOptionsInfo.ssjpegquality.minvalue = 60;
    displayOptionsInfo.ssjpegquality.maxvalue = 100;
	displayOptionsInfo.ssjpegquality.generic.toolTip =
		"Use this to adjust the quality of the screenshot saved as a JPG image between 60 to 100 "
		"percent. Default is 100. NOTE: A lower JPG image quality results in smaller files and "
		"thus can save disk space.";

	y += 2 * (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.anaglyph.generic.type = MTYPE_SPINCONTROL;
	displayOptionsInfo.anaglyph.generic.name = "Anaglyph 3D Mode:";
	displayOptionsInfo.anaglyph.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.anaglyph.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.anaglyph.generic.id = ID_ANAGLYPH;
	displayOptionsInfo.anaglyph.generic.x = XPOSITION;
	displayOptionsInfo.anaglyph.generic.y = y;
	displayOptionsInfo.anaglyph.itemnames = anaglyph_names;
	displayOptionsInfo.anaglyph.generic.toolTip =
		"Enable to play the game in anaglyph 3D mode with the appriopriate glasses. Default is "
		"off. NOTE: Ensure the correct filter option you select matches that of your glasses.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.swapcolors.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.swapcolors.generic.name = "Swap Colors:";
	displayOptionsInfo.swapcolors.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.swapcolors.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.swapcolors.generic.id = ID_SWAPCOLORS;
	displayOptionsInfo.swapcolors.generic.x = XPOSITION;
	displayOptionsInfo.swapcolors.generic.y = y;
	displayOptionsInfo.swapcolors.generic.toolTip =
		"Enable to swap the anaglyph 3D mode colors for left and right eye.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.greyscale.generic.type = MTYPE_SLIDER;
	displayOptionsInfo.greyscale.generic.name = "Greyscale:";
	displayOptionsInfo.greyscale.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.greyscale.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.greyscale.generic.id = ID_GREYSCALE;
	displayOptionsInfo.greyscale.generic.x = XPOSITION;
	displayOptionsInfo.greyscale.generic.y = y;
	displayOptionsInfo.greyscale.minvalue = 0;
	displayOptionsInfo.greyscale.maxvalue = 100;
	displayOptionsInfo.greyscale.generic.toolTip =
		"Use this to desaturate the colors in the game. A value of 100 replaces all colors "
		"with shades of gray. Default is 0. NOTE: Reducing the colors in the game can improve "
		"the operation of the anaglyph 3D modes.";

	displayOptionsInfo.apply.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.apply.generic.name = ACCEPT0;
	displayOptionsInfo.apply.generic.flags = QMF_PULSEIFFOCUS | QMF_HIDDEN | QMF_INACTIVE;
	displayOptionsInfo.apply.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.apply.generic.id = ID_APPLY;
	displayOptionsInfo.apply.generic.x = 736;
	displayOptionsInfo.apply.generic.y = 446;
	displayOptionsInfo.apply.width = 120;
	displayOptionsInfo.apply.height = 40;
	displayOptionsInfo.apply.focuspic = ACCEPT1;

	displayOptionsInfo.back.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.back.generic.name = BACK0;
	displayOptionsInfo.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	displayOptionsInfo.back.generic.callback = UI_DisplayOptions_Event;
	displayOptionsInfo.back.generic.id = ID_BACK;
	displayOptionsInfo.back.generic.x = 8;
	displayOptionsInfo.back.generic.y = 446;
	displayOptionsInfo.back.width = 80;
	displayOptionsInfo.back.height = 40;
	displayOptionsInfo.back.focuspic = BACK1;
	displayOptionsInfo.back.focuspicinstead = qtrue;

	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.graphics);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.display);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.sound);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.network);

	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.ignoreHWG);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.brightness);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.screensize);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.vsync);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.windowmode);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.resize);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.maxfps);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.screenshotformat);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.ssjpegquality);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.anaglyph);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.swapcolors);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.greyscale);

	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.apply);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.back);

	UI_DisplayOptions_SetMenuItems();
}

/*
===============
UI_DisplayOptions_Cache
===============
*/
void UI_DisplayOptions_Cache(void) {
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
UI_DisplayOptionsMenu
===============
*/
void UI_DisplayOptionsMenu(void) {
	UI_DisplayOptions_MenuInit();
	UI_PushMenu(&displayOptionsInfo.menu);
	Menu_SetCursorToItem(&displayOptionsInfo.menu, &displayOptionsInfo.display);
}
