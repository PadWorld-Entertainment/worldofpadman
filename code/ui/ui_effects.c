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
#define ID_AUTOEXPOSURE 12
#define ID_NORMMAPS 13
#define ID_SPECMAPS 14
#define ID_PARAMAPS 15
#define ID_DELUMAPS 16
#define ID_SUNSHADOWS 17
#define ID_SUNLIGHTMODE 18
#define ID_SHADOWFILTER 19
#define ID_SHADOWMAPSIZE 20
#define ID_SUNRAYS 21
#define ID_DLIGHTMODE 22

#define XPOSITION 180
#define YPOSITION 198

typedef struct {
	menuframework_s menu;

	menubitmap_s graphics;
	menubitmap_s display;
	menubitmap_s sound;
	menubitmap_s network;

	menuradiobutton_s hdr;
	menuradiobutton_s ssao;
	menuradiobutton_s autoExposure;
	menuradiobutton_s normMaps;
	menuradiobutton_s specMaps;
	menuradiobutton_s paraMaps;
	menuradiobutton_s deluMaps;
	menuradiobutton_s sunShadows;
	menulist_s sunlightMode;
	menulist_s shadowFilter;
	menulist_s shadowMapSize;
	menuradiobutton_s sunRays;
	menulist_s dlightMode;

	menubitmap_s apply;
	menubitmap_s back;

	int hdr_original;
	int ssao_original;
	int normMaps_original;
	int specMaps_original;
	int paraMaps_original;
	int deluMaps_original;
	int sunShadows_original;
	int sunRays_original;
	int sunlightMode_original;
	int shadowFilter_original;
	int shadowMapSize_original;
	int dlightMode_original;
} effectsOptionsInfo_t;

static effectsOptionsInfo_t effectsOptionsInfo;

/*
=================
UI_EffectsOptions_SetMenuItems
=================
*/
static void UI_EffectsOptions_SetMenuItems(void) {
	int sunlight;
	int sfilter;
	int smapsize;
	int dlight;

	effectsOptionsInfo.hdr_original = UI_GetCvarInt("r_hdr");
	effectsOptionsInfo.hdr.curvalue = effectsOptionsInfo.hdr_original;

	effectsOptionsInfo.ssao_original = UI_GetCvarInt("r_ssao");
	effectsOptionsInfo.ssao.curvalue = effectsOptionsInfo.ssao_original;

	effectsOptionsInfo.autoExposure.curvalue = UI_GetCvarInt("r_autoExposure");

	effectsOptionsInfo.normMaps_original = UI_GetCvarInt("r_normalMapping");
	effectsOptionsInfo.normMaps.curvalue = effectsOptionsInfo.normMaps_original;

	effectsOptionsInfo.specMaps_original = UI_GetCvarInt("r_specularMapping");
	effectsOptionsInfo.specMaps.curvalue = effectsOptionsInfo.specMaps_original;

	effectsOptionsInfo.paraMaps_original = UI_GetCvarInt("r_parallaxMapping");
	effectsOptionsInfo.paraMaps.curvalue = effectsOptionsInfo.paraMaps_original;

	effectsOptionsInfo.deluMaps_original = UI_GetCvarInt("r_deluxeMapping");
	effectsOptionsInfo.deluMaps.curvalue = effectsOptionsInfo.deluMaps_original;

	effectsOptionsInfo.sunShadows_original = UI_GetCvarInt("r_sunShadows");
	effectsOptionsInfo.sunShadows.curvalue = effectsOptionsInfo.sunShadows_original;

	sunlight = UI_GetCvarInt("r_sunlightMode");
	if (sunlight > 1) {
		effectsOptionsInfo.sunlightMode_original = 2;
	} else if (sunlight < 1) {
		effectsOptionsInfo.sunlightMode_original = 0;
	} else {
		effectsOptionsInfo.sunlightMode_original = 1;
	}
	effectsOptionsInfo.sunlightMode.curvalue = effectsOptionsInfo.sunlightMode_original;

	sfilter = UI_GetCvarInt("r_shadowFilter");
	if (sfilter > 1) {
		effectsOptionsInfo.shadowFilter_original = 2;
	} else if (sfilter < 1) {
		effectsOptionsInfo.shadowFilter_original = 0;
	} else {
		effectsOptionsInfo.shadowFilter_original = 1;
	}
	effectsOptionsInfo.shadowFilter.curvalue = effectsOptionsInfo.shadowFilter_original;

	smapsize = trap_Cvar_VariableValue("r_shadowMapSize");
	if (smapsize > 2048) {
		effectsOptionsInfo.shadowMapSize_original = 3;
	} else if (smapsize > 1024) {
		effectsOptionsInfo.shadowMapSize_original = 2;
	} else if (smapsize > 512) {
		effectsOptionsInfo.shadowMapSize_original = 1;
	} else {
		effectsOptionsInfo.shadowMapSize_original = 0;
	}
	effectsOptionsInfo.shadowMapSize.curvalue = effectsOptionsInfo.shadowMapSize_original;

	effectsOptionsInfo.sunRays_original = UI_GetCvarInt("r_drawSunRays");
	effectsOptionsInfo.sunRays.curvalue = effectsOptionsInfo.sunRays_original;

	dlight = UI_GetCvarInt("r_dlightMode");
	if (dlight > 1) {
		effectsOptionsInfo.dlightMode_original = 2;
	} else if (dlight < 1) {
		effectsOptionsInfo.dlightMode_original = 0;
	} else {
		effectsOptionsInfo.dlightMode_original = 1;
	}
	effectsOptionsInfo.dlightMode.curvalue = effectsOptionsInfo.dlightMode_original;
}

/*
===============
UI_EffectsOptions_UpdateMenuItems
===============
*/
static void UI_EffectsOptions_UpdateMenuItems(void) {

	if (!effectsOptionsInfo.hdr.curvalue) {
		effectsOptionsInfo.autoExposure.generic.flags |= QMF_GRAYED;
	} else {
		effectsOptionsInfo.autoExposure.generic.flags &= ~QMF_GRAYED;
	}

	effectsOptionsInfo.apply.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
	if ((effectsOptionsInfo.hdr_original != effectsOptionsInfo.hdr.curvalue) ||
		(effectsOptionsInfo.ssao_original != effectsOptionsInfo.ssao.curvalue) ||
		(effectsOptionsInfo.normMaps.curvalue != effectsOptionsInfo.normMaps_original) ||
		(effectsOptionsInfo.specMaps.curvalue != effectsOptionsInfo.specMaps_original) ||
		(effectsOptionsInfo.paraMaps.curvalue != effectsOptionsInfo.paraMaps_original) ||
		(effectsOptionsInfo.deluMaps.curvalue != effectsOptionsInfo.deluMaps_original) ||
		(effectsOptionsInfo.sunShadows_original != effectsOptionsInfo.sunShadows.curvalue) ||
		(effectsOptionsInfo.sunlightMode_original != effectsOptionsInfo.sunlightMode.curvalue) ||
		(effectsOptionsInfo.shadowFilter_original != effectsOptionsInfo.shadowFilter.curvalue) ||
		(effectsOptionsInfo.shadowMapSize_original != effectsOptionsInfo.shadowMapSize.curvalue) ||
		(effectsOptionsInfo.sunRays_original != effectsOptionsInfo.sunRays.curvalue) ||
		(effectsOptionsInfo.dlightMode.curvalue != effectsOptionsInfo.dlightMode_original)) {
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
	case ID_NORMMAPS:
	case ID_SPECMAPS:
	case ID_PARAMAPS:
	case ID_DELUMAPS:
	case ID_SUNSHADOWS:
	case ID_SUNRAYS:
	case ID_SUNLIGHTMODE:
	case ID_SHADOWFILTER:
	case ID_SHADOWMAPSIZE:
	case ID_DLIGHTMODE:
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

	case ID_AUTOEXPOSURE:
		trap_Cvar_SetValue("r_autoExposure", effectsOptionsInfo.autoExposure.curvalue);
		break;

	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_APPLY:
		if ((effectsOptionsInfo.hdr_original != effectsOptionsInfo.hdr.curvalue) ||
			(effectsOptionsInfo.ssao_original != effectsOptionsInfo.ssao.curvalue) ||
			(effectsOptionsInfo.normMaps.curvalue != effectsOptionsInfo.normMaps_original) ||
			(effectsOptionsInfo.specMaps.curvalue != effectsOptionsInfo.specMaps_original) ||
			(effectsOptionsInfo.paraMaps.curvalue != effectsOptionsInfo.paraMaps_original) ||
			(effectsOptionsInfo.deluMaps.curvalue != effectsOptionsInfo.deluMaps_original) ||
			(effectsOptionsInfo.sunShadows_original != effectsOptionsInfo.sunShadows.curvalue) ||
			(effectsOptionsInfo.sunlightMode_original != effectsOptionsInfo.sunlightMode.curvalue) ||
			(effectsOptionsInfo.shadowFilter_original != effectsOptionsInfo.shadowFilter.curvalue) ||
			(effectsOptionsInfo.shadowMapSize_original != effectsOptionsInfo.shadowMapSize.curvalue) ||
			(effectsOptionsInfo.sunRays_original != effectsOptionsInfo.sunRays.curvalue) ||
			(effectsOptionsInfo.dlightMode.curvalue != effectsOptionsInfo.dlightMode_original)) {

			trap_Cvar_SetValue("r_hdr", effectsOptionsInfo.hdr.curvalue);
			trap_Cvar_SetValue("r_ssao", effectsOptionsInfo.ssao.curvalue);
			trap_Cvar_SetValue("r_normalMapping", effectsOptionsInfo.normMaps.curvalue);
			trap_Cvar_SetValue("r_specularMapping", effectsOptionsInfo.specMaps.curvalue);
			trap_Cvar_SetValue("r_parallaxMapping", effectsOptionsInfo.paraMaps.curvalue);
			trap_Cvar_SetValue("r_deluxeMapping", effectsOptionsInfo.deluMaps.curvalue);

			trap_Cvar_SetValue("r_sunShadows", effectsOptionsInfo.sunShadows.curvalue);
			trap_Cvar_SetValue("r_sunlightMode", effectsOptionsInfo.sunlightMode.curvalue);

			if (effectsOptionsInfo.sunlightMode.curvalue > 1) {
				trap_Cvar_SetValue("r_sunlightMode", 2);
			} else if (effectsOptionsInfo.sunlightMode.curvalue < 1) {
				trap_Cvar_SetValue("r_sunlightMode", 0);
			} else {
				trap_Cvar_SetValue("r_sunlightMode", 1);
			}

			if (effectsOptionsInfo.shadowFilter.curvalue > 1) {
				trap_Cvar_SetValue("r_shadowFilter", 2);
			} else if (effectsOptionsInfo.shadowFilter.curvalue < 1) {
				trap_Cvar_SetValue("r_shadowFilter", 0);
			} else {
				trap_Cvar_SetValue("r_shadowFilter", 1);
			}

			if (effectsOptionsInfo.shadowMapSize.curvalue == 3) {
				trap_Cvar_SetValue("r_shadowMapSize", 4096);
			} else if (effectsOptionsInfo.shadowMapSize.curvalue == 2) {
				trap_Cvar_SetValue("r_shadowMapSize", 2048);
			} else if (effectsOptionsInfo.shadowMapSize.curvalue == 1) {
				trap_Cvar_SetValue("r_shadowMapSize", 1024);
			} else {
				trap_Cvar_SetValue("r_shadowMapSize", 512);
			}

			trap_Cvar_SetValue("r_drawSunRays", effectsOptionsInfo.sunRays.curvalue);

			if (effectsOptionsInfo.dlightMode.curvalue > 1) {
				trap_Cvar_SetValue("r_dlightMode", 2);
			} else if (effectsOptionsInfo.dlightMode.curvalue < 1) {
				trap_Cvar_SetValue("r_dlightMode", 0);
			} else {
				trap_Cvar_SetValue("r_dlightMode", 1);
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
	static const char *shadowFilter_items[] = {"Off", "Default", "Maximum", NULL};
	static const char *shadowMapSize_items[] = {"Low", "Medium", "High", "Maximum", NULL};
	static const char *dlightMode_items[] = {"Brightening", "Lightening", "Light & Shadow", NULL};
	int y;

	memset(&effectsOptionsInfo, 0, sizeof(effectsOptionsInfo));

	UI_EffectsOptions_Cache();
	effectsOptionsInfo.menu.wrapAround = qtrue;
	effectsOptionsInfo.menu.fullscreen = qtrue;
	effectsOptionsInfo.menu.draw = UI_EffectsOptions_MenuDraw;
	effectsOptionsInfo.menu.bgparts = BGP_SYSTEM | BGP_MENUFX;

	effectsOptionsInfo.graphics.generic.type = MTYPE_BITMAP;
	effectsOptionsInfo.graphics.generic.name = GRAPHICS0;
	effectsOptionsInfo.graphics.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT;
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
	effectsOptionsInfo.display.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
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
		"Disable to switch off scene rendering with high dynamic range. Default is on. "
		"NOTE: Needs to be activated for the automatic exposure effect.";

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
		"Enable screen-space ambient occlusion to achieve realistic shading. Default is off. "
		"NOTE: This effect currently eats framerate and has some visible artifacts.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_autoExposure"
	effectsOptionsInfo.autoExposure.generic.type = MTYPE_RADIOBUTTON;
	effectsOptionsInfo.autoExposure.generic.name = "Auto Exposure:";
	effectsOptionsInfo.autoExposure.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.autoExposure.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.autoExposure.generic.id = ID_AUTOEXPOSURE;
	effectsOptionsInfo.autoExposure.generic.x = XPOSITION;
	effectsOptionsInfo.autoExposure.generic.y = y;
	effectsOptionsInfo.autoExposure.generic.toolTip =
		"Disable to switch off automatic exposure based on scene brightness. This will stop "
		"adjusting the brightness depending on current light level. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_normalMapping"
	effectsOptionsInfo.normMaps.generic.type = MTYPE_RADIOBUTTON;
	effectsOptionsInfo.normMaps.generic.name = "Normal Mapping:";
	effectsOptionsInfo.normMaps.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.normMaps.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.normMaps.generic.id = ID_NORMMAPS;
	effectsOptionsInfo.normMaps.generic.x = XPOSITION;
	effectsOptionsInfo.normMaps.generic.y = y;
	effectsOptionsInfo.normMaps.generic.toolTip =
		"Disable to switch off normal mapping for textures that support it. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_specularMapping"
	effectsOptionsInfo.specMaps.generic.type = MTYPE_RADIOBUTTON;
	effectsOptionsInfo.specMaps.generic.name = "Specular Mapping:";
	effectsOptionsInfo.specMaps.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.specMaps.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.specMaps.generic.id = ID_SPECMAPS;
	effectsOptionsInfo.specMaps.generic.x = XPOSITION;
	effectsOptionsInfo.specMaps.generic.y = y;
	effectsOptionsInfo.specMaps.generic.toolTip =
		"Disable to switch off specular mapping for textures that support it. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_parallaxMapping"
	effectsOptionsInfo.paraMaps.generic.type = MTYPE_RADIOBUTTON;
	effectsOptionsInfo.paraMaps.generic.name = "Parallax Mapping:";
	effectsOptionsInfo.paraMaps.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.paraMaps.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.paraMaps.generic.id = ID_PARAMAPS;
	effectsOptionsInfo.paraMaps.generic.x = XPOSITION;
	effectsOptionsInfo.paraMaps.generic.y = y;
	effectsOptionsInfo.paraMaps.generic.toolTip =
		"Enable parallax mapping for textures that support it. Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_deluxeMapping"
	effectsOptionsInfo.deluMaps.generic.type = MTYPE_RADIOBUTTON;
	effectsOptionsInfo.deluMaps.generic.name = "Deluxe Mapping:";
	effectsOptionsInfo.deluMaps.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.deluMaps.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.deluMaps.generic.id = ID_DELUMAPS;
	effectsOptionsInfo.deluMaps.generic.x = XPOSITION;
	effectsOptionsInfo.deluMaps.generic.y = y;
	effectsOptionsInfo.deluMaps.generic.toolTip =
		"Disable to switch off deluxe mapping in maps. Default is on.";

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
		"Disable dynamic sunlight and cascaded shadow maps. Default is on. "
		"NOTE: The skybox shader of a map must support this for it to work.";

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
		"Select the method used to add sunlight to the scene. Off for none, "
		"dynamic (default) to multiply lit areas by light scale, and shadowed "
		"areas by ambient scale, or hybrid to add light respecting the baked "
		"lightmap.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_shadowFilter"
	effectsOptionsInfo.shadowFilter.generic.type = MTYPE_SPINCONTROL;
	effectsOptionsInfo.shadowFilter.generic.name = "Filter Shadows:";
	effectsOptionsInfo.shadowFilter.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.shadowFilter.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.shadowFilter.generic.id = ID_SHADOWFILTER;
	effectsOptionsInfo.shadowFilter.itemnames = shadowFilter_items;
	effectsOptionsInfo.shadowFilter.generic.x = XPOSITION;
	effectsOptionsInfo.shadowFilter.generic.y = y;
	effectsOptionsInfo.shadowFilter.generic.toolTip =
		"Set it to off to disable filtering shadows, or set to maximum to "
		"smooth shadows as much as possible.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_shadowMapSize"
	effectsOptionsInfo.shadowMapSize.generic.type = MTYPE_SPINCONTROL;
	effectsOptionsInfo.shadowMapSize.generic.name = "Shadows Quality:";
	effectsOptionsInfo.shadowMapSize.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.shadowMapSize.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.shadowMapSize.generic.id = ID_SHADOWMAPSIZE;
	effectsOptionsInfo.shadowMapSize.itemnames = shadowMapSize_items;
	effectsOptionsInfo.shadowMapSize.generic.x = XPOSITION;
	effectsOptionsInfo.shadowMapSize.generic.y = y;
	effectsOptionsInfo.shadowMapSize.generic.toolTip =
		"Select a desired shadow quality by defining the size of the shadow "
		"map: low (512), medium (1024), high (2048), and maximum (4096). "
		"Default is medium.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_drawSunRays"
	effectsOptionsInfo.sunRays.generic.type = MTYPE_RADIOBUTTON;
	effectsOptionsInfo.sunRays.generic.name = "Dynamic Sun Rays:";
	effectsOptionsInfo.sunRays.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.sunRays.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.sunRays.generic.id = ID_SUNRAYS;
	effectsOptionsInfo.sunRays.generic.x = XPOSITION;
	effectsOptionsInfo.sunRays.generic.y = y;
	effectsOptionsInfo.sunRays.generic.toolTip =
		"Enable to draw a dynamic sun rays effect. Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_dlightMode"
	effectsOptionsInfo.dlightMode.generic.type = MTYPE_SPINCONTROL;
	effectsOptionsInfo.dlightMode.generic.name = "Dynamic Light Mode:";
	effectsOptionsInfo.dlightMode.generic.flags = QMF_SMALLFONT;
	effectsOptionsInfo.dlightMode.generic.callback = UI_EffectsOptions_Event;
	effectsOptionsInfo.dlightMode.generic.id = ID_DLIGHTMODE;
	effectsOptionsInfo.dlightMode.itemnames = dlightMode_items;
	effectsOptionsInfo.dlightMode.generic.x = XPOSITION;
	effectsOptionsInfo.dlightMode.generic.y = y;
	effectsOptionsInfo.dlightMode.generic.toolTip =
		"Select whether dynamic lights should be brightening (default) the scene, "
		"lightening (without shadows), or lightening and cast shadows.";

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
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.autoExposure);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.normMaps);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.specMaps);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.paraMaps);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.deluMaps);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.sunShadows);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.sunlightMode);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.shadowFilter);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.shadowMapSize);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.dlightMode);
	Menu_AddItem(&effectsOptionsInfo.menu, (void *)&effectsOptionsInfo.sunRays);

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
