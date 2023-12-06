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

void UI_GraphicsOptions_MenuInit(void);

/*
=======================================================================

GRAPHICS OPTIONS MENU

=======================================================================
*/

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
#define ID_EFFECTS 105

#define ID_LIST 10

#define XPOSITION 220
#define YPOSITION 198

typedef struct {
	menuframework_s menu;

	menubitmap_s graphics;
	menubitmap_s display;
	menubitmap_s sound;
	menubitmap_s network;

	menulist_s list;
	menulist_s mode;
#ifdef USE_RENDERER_DLOPEN
	menulist_s renderer;
#endif
	menulist_s colordepth;
	menulist_s lighting;
	menulist_s mdetail;
	menulist_s cdetail;
	menulist_s tdetail;
	menulist_s tquality;
	menulist_s tfilter;
	menuradiobutton_s ct;
	menulist_s aa;
	menutext_s effects;

	menubitmap_s apply;
	menubitmap_s back;
} graphicsoptions_t;

typedef struct {
	int mode;
	int renderer;
	int colordepth;
	int lighting;
	int mdetail;
	int cdetail;
	int tdetail;
	int tquality;
	int tfilter;
	qboolean ct;
	int aa;
} InitialVideoOptions_s;

static InitialVideoOptions_s s_ivo;
static graphicsoptions_t s_graphicsoptions;

static InitialVideoOptions_s s_ivo_templates[] = {{3, 1, 0, 1, 3, 3, 3, 2, 5, qfalse, 3},	// Can it run WoP?
												  {3, 0, 0, 1, 3, 3, 3, 2, 3, qfalse, 2},	// Maximum
												  {2, 0, 0, 1, 2, 2, 2, 2, 2, qfalse, 1},	// Quality
												  {1, 0, 0, 1, 1, 1, 1, 0, 1, qfalse, 0},	// Performance
												  {0, 0, 0, 0, 0, 0, 0, 1, 0, qtrue, 0},	// Minimum
												  {3, 0, 0, 1, 2, 2, 2, 0, 1, qfalse, 0}};	// Custom

#define NUM_IVO_TEMPLATES (ARRAY_LEN(s_ivo_templates))

// x^y
// FIXME: Overflow!
static int power(int x, int y) {
	int i, result = 1;

	for (i = 0; i < y; i++) {
		result *= x;
	}

	return result;
}

static const char *builtinResolutions[] = {"640x480", "800x600", "1024x768", "1152x864", "1280x720", "1280x800",
										   "1280x960", "1280x1024", "1366x768", "1440x900", "1600x900", "1600x1200",
										   "1680x1050", "1920x1080", "1920x1200", "2048x1536", "2560x1600", NULL};

static const char *knownRatios[][2] = {{"1.25:1", "5:4"},	{"1.33:1", "4:3"}, {"1.50:1", "3:2"},  {"1.56:1", "14:9"},
									   {"1.60:1", "16:10"}, {"1.67:1", "5:3"}, {"1.78:1", "16:9"}, {NULL, NULL}};

#define MAX_RESOLUTIONS 32

static char resbuf[MAX_STRING_CHARS];
static char resolutionStrings[MAX_RESOLUTIONS][128];
static const char *detectedResolutions[MAX_RESOLUTIONS];

static const char **resolutions = builtinResolutions;
static qboolean resolutionsDetected = qfalse;

/*
=================
UI_GraphicsOptions_FindBuiltinResolution
=================
*/
static int UI_GraphicsOptions_FindBuiltinResolution(int mode) {
	int i;

	if (!resolutionsDetected)
		return mode;

	if (mode < 0)
		return -1;

	for (i = 0; builtinResolutions[i]; i++) {
		if (!Q_stricmp(builtinResolutions[i], detectedResolutions[mode]))
			return i;
	}

	return -1;
}

/*
=================
UI_GraphicsOptions_FindDetectedResolution
=================
*/
static int UI_GraphicsOptions_FindDetectedResolution(int mode) {
	int i;
	int len;

	if (!resolutionsDetected)
		return mode;

	if (mode < 0)
		return -1;

	len = strlen(builtinResolutions[mode]);
	for (i = 0; detectedResolutions[i]; i++) {
		if (!Q_stricmpn(builtinResolutions[mode], detectedResolutions[i], len))
			return i;
	}

	return -1;
}

/*
=================
UI_GraphicsOptions_GetAspectRatios
=================
*/
static void UI_GraphicsOptions_GetAspectRatios(void) {
	int i, r;

	// add aspect ratio to the resolution strings
	for (r = 0; resolutions[r]; r++) {
		int w, h;
		char *x;
		char str[8];

		// calculate resolution's aspect ratio
		x = strchr(resolutions[r], 'x') + 1;
		Q_strncpyz(str, resolutions[r], x - resolutions[r]);
		w = atoi(str);
		h = atoi(x);
		Com_sprintf(str, sizeof(str), "%.2f:1", (float)w / (float)h);

		for (i = 0; knownRatios[i][0]; i++) {
			if (!Q_stricmp(str, knownRatios[i][0])) {
				Q_strncpyz(str, knownRatios[i][1], sizeof(str));
				break;
			}
		}

		Com_sprintf(resolutionStrings[r], 128, "%s (%s)", resolutions[r], str);
		resolutions[r] = resolutionStrings[r];
	}
}

/*
=================
UI_GraphicsOptions_GetResolutions
=================
*/
static void UI_GraphicsOptions_GetResolutions(void) {
	Q_strncpyz(resbuf, UI_Cvar_VariableString("r_availableModes"), sizeof(resbuf));
	if (*resbuf) {
		char *s = resbuf;
		unsigned int i = 0;
		while (s && i < ARRAY_LEN(detectedResolutions) - 1) {
			detectedResolutions[i++] = s;
			s = strchr(s, ' ');
			if (s)
				*s++ = '\0';
		}
		detectedResolutions[i] = NULL;

		if (i > 0) {
			resolutions = detectedResolutions;
			resolutionsDetected = qtrue;
		}
	}

	UI_GraphicsOptions_GetAspectRatios();
}

/*
=================
UI_GraphicsOptions_GetInitialVideo
=================
*/
static void UI_GraphicsOptions_GetInitialVideo(void) {
	s_ivo.mode = s_graphicsoptions.mode.curvalue;
#ifdef USE_RENDERER_DLOPEN
	s_ivo.renderer = s_graphicsoptions.renderer.curvalue;
#endif
	s_ivo.colordepth = s_graphicsoptions.colordepth.curvalue;
	s_ivo.lighting = s_graphicsoptions.lighting.curvalue;
	s_ivo.mdetail = s_graphicsoptions.mdetail.curvalue;
	s_ivo.cdetail = s_graphicsoptions.cdetail.curvalue;
	s_ivo.tdetail = s_graphicsoptions.tdetail.curvalue;
	s_ivo.tquality = s_graphicsoptions.tquality.curvalue;
	s_ivo.tfilter = s_graphicsoptions.tfilter.curvalue;
	s_ivo.ct = s_graphicsoptions.ct.curvalue;
	s_ivo.aa = s_graphicsoptions.aa.curvalue;
}

/*
=================
UI_GraphicsOptions_CheckConfig
=================
*/
static void UI_GraphicsOptions_CheckConfig(void) {
	int i;

	for (i = 0; i < (NUM_IVO_TEMPLATES - 1); i++) {
		if (s_ivo_templates[i].mode != s_graphicsoptions.mode.curvalue)
			continue;
#ifdef USE_RENDERER_DLOPEN
		if (s_ivo_templates[i].renderer != s_graphicsoptions.renderer.curvalue)
			continue;
#endif
		if (s_ivo_templates[i].colordepth != s_graphicsoptions.colordepth.curvalue)
			continue;
		if (s_ivo_templates[i].lighting != s_graphicsoptions.lighting.curvalue)
			continue;
		if (s_ivo_templates[i].mdetail != s_graphicsoptions.mdetail.curvalue)
			continue;
		if (s_ivo_templates[i].cdetail != s_graphicsoptions.cdetail.curvalue)
			continue;
		if (s_ivo_templates[i].tdetail != s_graphicsoptions.tdetail.curvalue)
			continue;
//		if (s_ivo_templates[i].tquality != s_graphicsoptions.tquality.curvalue)
//			continue;
		if (s_ivo_templates[i].tfilter != s_graphicsoptions.tfilter.curvalue)
			continue;
		if (s_ivo_templates[i].ct != s_graphicsoptions.ct.curvalue)
			continue;
		if (s_ivo_templates[i].aa != s_graphicsoptions.aa.curvalue)
			continue;

		s_graphicsoptions.list.curvalue = i;
		return;
	}

	s_graphicsoptions.list.curvalue = (NUM_IVO_TEMPLATES - 1); // "custom"
}

/*
=================
UI_GraphicsOptions_UpdateMenuItems
=================
*/
static void UI_GraphicsOptions_UpdateMenuItems(void) {
	if (UI_GetCvarInt("r_fullscreen") == 0) {
		s_graphicsoptions.colordepth.generic.flags |= QMF_GRAYED;
	} else {
		s_graphicsoptions.colordepth.generic.flags &= ~QMF_GRAYED;
	}

#ifdef USE_RENDERER_DLOPEN
	if (Q_stricmp(UI_Cvar_VariableString("cl_renderer"), "opengl2") ||
		(s_graphicsoptions.renderer.curvalue != 1)) {
		s_graphicsoptions.effects.generic.flags |= QMF_GRAYED;
	} else {
		s_graphicsoptions.effects.generic.flags &= ~QMF_GRAYED;
	}
#endif

	s_graphicsoptions.apply.generic.flags |= QMF_HIDDEN | QMF_INACTIVE;
	if (s_ivo.mode != s_graphicsoptions.mode.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
#ifdef USE_RENDERER_DLOPEN
	} else if (s_ivo.renderer != s_graphicsoptions.renderer.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
#endif
	} else if (s_ivo.colordepth != s_graphicsoptions.colordepth.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (s_ivo.lighting != s_graphicsoptions.lighting.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (s_ivo.mdetail != s_graphicsoptions.mdetail.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (s_ivo.cdetail != s_graphicsoptions.cdetail.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (s_ivo.tdetail != s_graphicsoptions.tdetail.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (s_ivo.tquality != s_graphicsoptions.tquality.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (s_ivo.tfilter != s_graphicsoptions.tfilter.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (s_ivo.ct != s_graphicsoptions.ct.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (s_ivo.aa != s_graphicsoptions.aa.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	}

	UI_GraphicsOptions_CheckConfig();
}

/*
=================
UI_GraphicsOptions_ApplyChanges
=================
*/
static void UI_GraphicsOptions_ApplyChanges(void *unused, int notification) {
	if (notification != QM_ACTIVATED)
		return;

#ifdef USE_RENDERER_DLOPEN
	switch (s_graphicsoptions.renderer.curvalue) {
	case 0:
		trap_Cvar_Set("cl_renderer", "opengl1");
		break;
	case 1:
		trap_Cvar_Set("cl_renderer", "opengl2");
		break;
	case 2:
		trap_Cvar_Set("cl_renderer", "vulkan");
		break;
	}
#endif

	switch (s_graphicsoptions.tquality.curvalue) {
	case 0:
		trap_Cvar_SetValue("r_texturebits", 0);
		break;
	case 1:
		trap_Cvar_SetValue("r_texturebits", 16);
		break;
	case 2:
		trap_Cvar_SetValue("r_texturebits", 32);
		break;
	}

	trap_Cvar_SetValue("r_picmip", (float)(3 - s_graphicsoptions.tdetail.curvalue));

	switch (s_graphicsoptions.colordepth.curvalue) {
	case 0:
		trap_Cvar_SetValue("r_colorbits", 0);
		trap_Cvar_SetValue("r_depthbits", 0);
		trap_Cvar_SetValue("r_stencilbits", 0);
		break;
	case 1:
		trap_Cvar_SetValue("r_colorbits", 16);
		trap_Cvar_SetValue("r_depthbits", 16);
		trap_Cvar_SetValue("r_stencilbits", 0);
		break;
	case 2:
		trap_Cvar_SetValue("r_colorbits", 32);
		trap_Cvar_SetValue("r_depthbits", 24);
		break;
	}
	trap_Cvar_SetValue("r_vertexLight", !s_graphicsoptions.lighting.curvalue);

	if (s_graphicsoptions.mdetail.curvalue == 3) {
		trap_Cvar_SetValue("r_lodBias", -2);
	} else if (s_graphicsoptions.mdetail.curvalue == 2) {
		trap_Cvar_SetValue("r_lodBias", 0);
	} else if (s_graphicsoptions.mdetail.curvalue == 1) {
		trap_Cvar_SetValue("r_lodBias", 1);
	} else {
		trap_Cvar_SetValue("r_lodBias", 2);
	}

	if (s_graphicsoptions.cdetail.curvalue == 3) {
		trap_Cvar_SetValue("r_subdivisions", 2);
	} else if (s_graphicsoptions.cdetail.curvalue == 2) {
		trap_Cvar_SetValue("r_subdivisions", 4);
	} else if (s_graphicsoptions.cdetail.curvalue == 1) {
		trap_Cvar_SetValue("r_subdivisions", 12);
	} else {
		trap_Cvar_SetValue("r_subdivisions", 20);
	}

	if (s_graphicsoptions.tfilter.curvalue > 1) {
		trap_Cvar_Set("r_ext_texture_filter_anisotropic", "1");
		trap_Cvar_SetValue("r_ext_max_anisotropy", power(2, s_graphicsoptions.tfilter.curvalue - 1));
	} else {
		trap_Cvar_Set("r_ext_texture_filter_anisotropic", "0");
		trap_Cvar_Set("r_ext_max_anisotropy", "2");
		trap_Cvar_Set("r_textureMode", "GL_LINEAR_MIPMAP_NEAREST");
		if (s_graphicsoptions.tfilter.curvalue) {
			trap_Cvar_Set("r_textureMode", "GL_LINEAR_MIPMAP_LINEAR");
		}
	}

	trap_Cvar_SetValue("r_ext_compressed_textures", s_graphicsoptions.ct.curvalue);

	if (s_graphicsoptions.aa.curvalue > 0) {
		trap_Cvar_SetValue("r_ext_multisample", power(2, s_graphicsoptions.aa.curvalue));
		trap_Cvar_SetValue("r_ext_framebuffer_multisample", power(2, s_graphicsoptions.aa.curvalue));
	} else {
		trap_Cvar_Set("r_ext_multisample", "0");
		trap_Cvar_Set("r_ext_framebuffer_multisample", "0");
	}

	if (resolutionsDetected) {
		// search for builtin mode that matches the detected mode
		int mode;
		if (s_graphicsoptions.mode.curvalue == -1 ||
			s_graphicsoptions.mode.curvalue >= sizeof(detectedResolutions) / sizeof(detectedResolutions[0]))
			s_graphicsoptions.mode.curvalue = 0;

		mode = UI_GraphicsOptions_FindBuiltinResolution(s_graphicsoptions.mode.curvalue);
		if (mode == -1) {
			char w[16], h[16];
			Q_strncpyz(w, detectedResolutions[s_graphicsoptions.mode.curvalue], sizeof(w));
			*strchr(w, 'x') = 0;
			Q_strncpyz(h, strchr(detectedResolutions[s_graphicsoptions.mode.curvalue], 'x') + 1, sizeof(h));
			trap_Cvar_Set("r_customwidth", w);
			trap_Cvar_Set("r_customheight", h);
		}

		trap_Cvar_SetValue("r_mode", (float)mode);
	} else
		trap_Cvar_SetValue("r_mode", (float)s_graphicsoptions.mode.curvalue);

	trap_Cmd_ExecuteText(EXEC_APPEND, "vid_restart\n");
}

/*
=================
UI_GraphicsOptions_Event
=================
*/
static void UI_GraphicsOptions_Event(void *ptr, int event) {
	InitialVideoOptions_s *ivo;

	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_LIST:
		ivo = &s_ivo_templates[s_graphicsoptions.list.curvalue];
		s_graphicsoptions.mode.curvalue = ivo->mode;
#ifdef USE_RENDERER_DLOPEN
		s_graphicsoptions.renderer.curvalue = ivo->renderer;
#endif
		s_graphicsoptions.colordepth.curvalue = ivo->colordepth;
		s_graphicsoptions.lighting.curvalue = ivo->lighting;
		s_graphicsoptions.mdetail.curvalue = ivo->mdetail;
		s_graphicsoptions.cdetail.curvalue = ivo->cdetail;
		s_graphicsoptions.tdetail.curvalue = ivo->tdetail;
		s_graphicsoptions.tquality.curvalue = ivo->tquality;
		s_graphicsoptions.tfilter.curvalue = ivo->tfilter;
		s_graphicsoptions.ct.curvalue = ivo->ct;
		s_graphicsoptions.aa.curvalue = ivo->aa;
		break;

	case ID_GRAPHICS:
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

	case ID_EFFECTS:
		UI_PopMenu();
		UI_EffectsOptionsMenu();
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}

/*
================
UI_GraphicsOptions_MenuDraw
================
*/
static void UI_GraphicsOptions_MenuDraw(void) {
	// APSFIX - rework this
	UI_GraphicsOptions_UpdateMenuItems();
	Menu_Draw(&s_graphicsoptions.menu);
}

/*
=================
UI_GraphicsOptions_SetMenuItems
=================
*/
static void UI_GraphicsOptions_SetMenuItems(void) {

	s_graphicsoptions.mode.curvalue = UI_GraphicsOptions_FindDetectedResolution(UI_GetCvarInt("r_mode"));

	if (s_graphicsoptions.mode.curvalue < 0) {
		if (resolutionsDetected) {
			int i;
			char buf[MAX_STRING_CHARS];
			trap_Cvar_VariableStringBuffer("r_customwidth", buf, sizeof(buf) - 2);
			buf[strlen(buf) + 1] = 0;
			buf[strlen(buf)] = 'x';
			trap_Cvar_VariableStringBuffer("r_customheight", buf + strlen(buf), sizeof(buf) - strlen(buf));

			for (i = 0; detectedResolutions[i]; ++i) {
				if (!Q_stricmp(buf, detectedResolutions[i])) {
					s_graphicsoptions.mode.curvalue = i;
					break;
				}
			}
			if (s_graphicsoptions.mode.curvalue < 0)
				s_graphicsoptions.mode.curvalue = 0;
		} else {
			s_graphicsoptions.mode.curvalue = 2;
		}
	}

#ifdef USE_RENDERER_DLOPEN
	if (!Q_stricmp(UI_Cvar_VariableString("cl_renderer"), "vulkan")) {
		s_graphicsoptions.renderer.curvalue = 2;
	} else if (!Q_stricmp(UI_Cvar_VariableString("cl_renderer"), "opengl2")) {
		s_graphicsoptions.renderer.curvalue = 1;
	} else {
		s_graphicsoptions.renderer.curvalue = 0;
	}
#endif

	s_graphicsoptions.tdetail.curvalue = 3 - UI_GetCvarInt("r_picmip");
	if (s_graphicsoptions.tdetail.curvalue < 0) {
		s_graphicsoptions.tdetail.curvalue = 0;
	} else if (s_graphicsoptions.tdetail.curvalue > 3) {
		s_graphicsoptions.tdetail.curvalue = 3;
	}

	s_graphicsoptions.lighting.curvalue = trap_Cvar_VariableValue("r_vertexLight") == 0;

	switch (UI_GetCvarInt("r_texturebits")) {
	default:
	case 0:
		s_graphicsoptions.tquality.curvalue = 0;
		break;
	case 16:
		s_graphicsoptions.tquality.curvalue = 1;
		break;
	case 32:
		s_graphicsoptions.tquality.curvalue = 2;
		break;
	}

	if (trap_Cvar_VariableValue("r_ext_texture_filter_anisotropic")) {
		if (trap_Cvar_VariableValue("r_ext_max_anisotropy") > 2) {
			s_graphicsoptions.tfilter.curvalue = 1 + ceil(sqrt(trap_Cvar_VariableValue("r_ext_max_anisotropy")));
		} else {
			s_graphicsoptions.tfilter.curvalue = 1 + floor(sqrt(trap_Cvar_VariableValue("r_ext_max_anisotropy")));
		}
	} else {
		if (Q_stricmp(UI_Cvar_VariableString("r_textureMode"), "GL_LINEAR_MIPMAP_NEAREST")) {
			s_graphicsoptions.tfilter.curvalue = 1;
		} else {
			s_graphicsoptions.tfilter.curvalue = 0;
		}
	}

	if (trap_Cvar_VariableValue("r_lodBias") < 0) {
		s_graphicsoptions.mdetail.curvalue = 3;
	} else if (trap_Cvar_VariableValue("r_lodBias") < 1) {
		s_graphicsoptions.mdetail.curvalue = 2;
	} else if (trap_Cvar_VariableValue("r_lodBias") < 2) {
		s_graphicsoptions.mdetail.curvalue = 1;
	} else {
		s_graphicsoptions.mdetail.curvalue = 0;
	}

	if (trap_Cvar_VariableValue("r_subdivisions") < 4) {
		s_graphicsoptions.cdetail.curvalue = 3;
	} else if (trap_Cvar_VariableValue("r_subdivisions") < 12) {
		s_graphicsoptions.cdetail.curvalue = 2;
	} else if (trap_Cvar_VariableValue("r_subdivisions") < 20) {
		s_graphicsoptions.cdetail.curvalue = 1;
	} else {
		s_graphicsoptions.cdetail.curvalue = 0;
	}

	switch (UI_GetCvarInt("r_colorbits")) {
	default:
	case 0:
		s_graphicsoptions.colordepth.curvalue = 0;
		break;
	case 16:
		s_graphicsoptions.colordepth.curvalue = 1;
		break;
	case 32:
		s_graphicsoptions.colordepth.curvalue = 2;
		break;
	}

	if (UI_GetCvarInt("r_fullscreen") == 0) {
		s_graphicsoptions.colordepth.curvalue = 0;
	}

	s_graphicsoptions.ct.curvalue = UI_GetCvarInt("r_ext_compressed_textures");

#ifdef USE_RENDERER_DLOPEN
	if (!Q_stricmp(UI_Cvar_VariableString("cl_renderer"), "opengl2")) {
		if (trap_Cvar_VariableValue("r_ext_framebuffer_multisample") > 2) {
			s_graphicsoptions.aa.curvalue = ceil(sqrt(trap_Cvar_VariableValue("r_ext_framebuffer_multisample")));
		} else {
			s_graphicsoptions.aa.curvalue = floor(sqrt(trap_Cvar_VariableValue("r_ext_framebuffer_multisample")));
		}
	} else {
		if (trap_Cvar_VariableValue("r_ext_multisample") > 2) {
			s_graphicsoptions.aa.curvalue = ceil(sqrt(trap_Cvar_VariableValue("r_ext_multisample")));
		} else {
			s_graphicsoptions.aa.curvalue = floor(sqrt(trap_Cvar_VariableValue("r_ext_multisample")));
		}
	}
#else
	if (trap_Cvar_VariableValue("r_ext_framebuffer_multisample") > 2) {
		s_graphicsoptions.aa.curvalue = ceil(sqrt(trap_Cvar_VariableValue("r_ext_framebuffer_multisample")));
	} else {
		s_graphicsoptions.aa.curvalue = floor(sqrt(trap_Cvar_VariableValue("r_ext_framebuffer_multisample")));
	}
#endif
}

/*
================
UI_GraphicsOptions_MenuInit
================
*/
void UI_GraphicsOptions_MenuInit(void) {
	static const char *templates_names[] = {"Can it run WoP?", "Maximum", "Quality", "Performance", "Minimum", "Custom", NULL};
	static const char *renderer_names[] = {"OpenGL1", "OpenGL2", "Vulkan", NULL};
	static const char *colordepth_names[] = {"Desktop", "16 bit", "32 bit", NULL};
	static const char *lighting_names[] = {"Low (Vertex)", "High (Lightmap)", NULL};
	static const char *mdetail_names[] = {"Low", "Medium", "High", "Maximum", NULL};
	static const char *cdetail_names[] = {"Low", "Medium", "High", "Maximum", NULL};
	static const char *tdetail_names[] = {"Low", "Medium", "High", "Maximum", NULL};
	static const char *tquality_names[] = {"Default", "16 bit", "32 bit", NULL};
	static const char *tfilter_names[] = {"Bilinear", "Trilinear", "Anisotropic (2x)", "Anisotropic (4x)",
											"Anisotropic (8x)", "Anisotropic (16x)", NULL};
	static const char *aa_names[] = {"Off", "2x", "4x", "8x", NULL};

	int y;

	// zero set all our globals
	memset(&s_graphicsoptions, 0, sizeof(graphicsoptions_t));

	UI_GraphicsOptions_GetResolutions();

	UI_GraphicsOptions_Cache();

	s_graphicsoptions.menu.wrapAround = qtrue;
	s_graphicsoptions.menu.fullscreen = qtrue;
	s_graphicsoptions.menu.draw = UI_GraphicsOptions_MenuDraw;
	s_graphicsoptions.menu.bgparts = BGP_SYSTEM | BGP_MENUFX;

	s_graphicsoptions.graphics.generic.type = MTYPE_BITMAP;
	s_graphicsoptions.graphics.generic.name = GRAPHICS0;
	s_graphicsoptions.graphics.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT;
	s_graphicsoptions.graphics.generic.callback = UI_GraphicsOptions_Event;
	s_graphicsoptions.graphics.generic.id = ID_GRAPHICS;
	s_graphicsoptions.graphics.generic.x = XPOSITION - 144;
	s_graphicsoptions.graphics.generic.y = 43;
	s_graphicsoptions.graphics.width = 160;
	s_graphicsoptions.graphics.height = 40;
	s_graphicsoptions.graphics.focuspic = GRAPHICS1;
	s_graphicsoptions.graphics.focuspicinstead = qtrue;

	s_graphicsoptions.display.generic.type = MTYPE_BITMAP;
	s_graphicsoptions.display.generic.name = DISPLAY0;
	s_graphicsoptions.display.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_graphicsoptions.display.generic.callback = UI_GraphicsOptions_Event;
	s_graphicsoptions.display.generic.id = ID_DISPLAY;
	s_graphicsoptions.display.generic.x = XPOSITION + 9;
	s_graphicsoptions.display.generic.y = 36;
	s_graphicsoptions.display.width = 120;
	s_graphicsoptions.display.height = 40;
	s_graphicsoptions.display.focuspic = DISPLAY1;
	s_graphicsoptions.display.focuspicinstead = qtrue;

	s_graphicsoptions.sound.generic.type = MTYPE_BITMAP;
	s_graphicsoptions.sound.generic.name = SOUND0;
	s_graphicsoptions.sound.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_graphicsoptions.sound.generic.callback = UI_GraphicsOptions_Event;
	s_graphicsoptions.sound.generic.id = ID_SOUND;
	s_graphicsoptions.sound.generic.x = XPOSITION - 124;
	s_graphicsoptions.sound.generic.y = 85;
	s_graphicsoptions.sound.width = 120;
	s_graphicsoptions.sound.height = 40;
	s_graphicsoptions.sound.focuspic = SOUND1;
	s_graphicsoptions.sound.focuspicinstead = qtrue;

	s_graphicsoptions.network.generic.type = MTYPE_BITMAP;
	s_graphicsoptions.network.generic.name = NETWORK0;
	s_graphicsoptions.network.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_graphicsoptions.network.generic.callback = UI_GraphicsOptions_Event;
	s_graphicsoptions.network.generic.id = ID_NETWORK;
	s_graphicsoptions.network.generic.x = XPOSITION - 18;
	s_graphicsoptions.network.generic.y = 88;
	s_graphicsoptions.network.width = 160;
	s_graphicsoptions.network.height = 40;
	s_graphicsoptions.network.focuspic = NETWORK1;
	s_graphicsoptions.network.focuspicinstead = qtrue;

	y = YPOSITION;
	s_graphicsoptions.list.generic.type = MTYPE_SPINCONTROL;
	s_graphicsoptions.list.generic.name = "Graphics Settings:";
	s_graphicsoptions.list.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.list.generic.callback = UI_GraphicsOptions_Event;
	s_graphicsoptions.list.generic.id = ID_LIST;
	s_graphicsoptions.list.generic.x = XPOSITION;
	s_graphicsoptions.list.generic.y = y;
	s_graphicsoptions.list.itemnames = templates_names;
	s_graphicsoptions.list.generic.toolTip =
		"Select a suitable template from the list. Maximum will set all values to high or "
		"maximum except Texture Filter and Antialiasing. Minimum will set everything to low or "
		"off. Quality and Performance offer a good compromise in one direction or the other.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_mode"
	s_graphicsoptions.mode.generic.type = MTYPE_SPINCONTROL;
	s_graphicsoptions.mode.generic.name = "Video Mode:";
	s_graphicsoptions.mode.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.mode.itemnames = resolutions;
	s_graphicsoptions.mode.generic.x = XPOSITION;
	s_graphicsoptions.mode.generic.y = y;
	s_graphicsoptions.mode.generic.toolTip =
		"The game usually determines the correct resolution automatically upon the first "
		"start. However, you can also manually select a suitable resolution from the list "
		"of possible resolutions.";

#ifdef USE_RENDERER_DLOPEN
	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "cl_renderer"
	s_graphicsoptions.renderer.generic.type = MTYPE_SPINCONTROL;
	s_graphicsoptions.renderer.generic.name = "Renderer:";
	s_graphicsoptions.renderer.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.renderer.itemnames = renderer_names;
	s_graphicsoptions.renderer.generic.x = XPOSITION;
	s_graphicsoptions.renderer.generic.y = y;
	s_graphicsoptions.renderer.generic.toolTip =
		"Select a desired renderer. Default is OpenGL1. OpenGL2 offers more features and "
		"effects but can lead to a higher graphics card load. Vulkan is still experimental.";
#endif

	y += (BIGCHAR_HEIGHT + 2);
	// references "r_colorbits"
	s_graphicsoptions.colordepth.generic.type = MTYPE_SPINCONTROL;
	s_graphicsoptions.colordepth.generic.name = "Color Depth:";
	s_graphicsoptions.colordepth.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.colordepth.itemnames = colordepth_names;
	s_graphicsoptions.colordepth.generic.x = XPOSITION;
	s_graphicsoptions.colordepth.generic.y = y;
	s_graphicsoptions.colordepth.generic.toolTip =
		"Select a desired color depth, 16 or 32 bit. Default is desktop. The color depth is "
		"then determined based on the current desktop setting. Selecting a lower color depth "
		"value can save system resources. NOTE: This option is disabled in window mode.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_vertexLight"
	s_graphicsoptions.lighting.generic.type = MTYPE_SPINCONTROL;
	s_graphicsoptions.lighting.generic.name = "Lighting:";
	s_graphicsoptions.lighting.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.lighting.itemnames = lighting_names;
	s_graphicsoptions.lighting.generic.x = XPOSITION;
	s_graphicsoptions.lighting.generic.y = y;
	s_graphicsoptions.lighting.generic.toolTip =
		"Select a desired lighting method. Default is high (Lightmap). It enables advanced "
		"shaders and dynamic lighting effects in the game. Low (Vertex) instead offers a "
		"poor and flat lighting quality but can save system resources.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_lodBias"
	s_graphicsoptions.mdetail.generic.type = MTYPE_SPINCONTROL;
	s_graphicsoptions.mdetail.generic.name = "Models Detail:";
	s_graphicsoptions.mdetail.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.mdetail.itemnames = mdetail_names;
	s_graphicsoptions.mdetail.generic.x = XPOSITION;
	s_graphicsoptions.mdetail.generic.y = y;
	s_graphicsoptions.mdetail.generic.toolTip =
		"Select a desired models detail. Default is high. It enables the game to load high "
		"quality models on lower distances. Setting a lower level causes models with less "
		"polygons to be loaded but can save system resources. Setting maximum always results "
		"in high quality models regardless of the distance.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_subdivisions"
	s_graphicsoptions.cdetail.generic.type = MTYPE_SPINCONTROL;
	s_graphicsoptions.cdetail.generic.name = "Curves Detail:";
	s_graphicsoptions.cdetail.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.cdetail.itemnames = cdetail_names;
	s_graphicsoptions.cdetail.generic.x = XPOSITION;
	s_graphicsoptions.cdetail.generic.y = y;
	s_graphicsoptions.cdetail.generic.toolTip =
		"Select a desired curves detail. Default is high. It enables the game to smooth out "
		"curved patches on lower distances. Setting a lower level causes curved patches to "
		"look less smooth but can save system resources.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_picmip"
	s_graphicsoptions.tdetail.generic.type = MTYPE_SPINCONTROL;
	s_graphicsoptions.tdetail.generic.name = "Texture Detail:";
	s_graphicsoptions.tdetail.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.tdetail.itemnames = tdetail_names;
	s_graphicsoptions.tdetail.generic.x = XPOSITION;
	s_graphicsoptions.tdetail.generic.y = y;
	s_graphicsoptions.tdetail.generic.toolTip =
		"Select a desired texture detail. Default is high. It enables the game to adjust the "
		"texture quality. Setting a lower level causes textures to look flat but can save "
		"system resources.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_textureBits"
	s_graphicsoptions.tquality.generic.type = MTYPE_SPINCONTROL;
	s_graphicsoptions.tquality.generic.name = "Texture Quality:";
	s_graphicsoptions.tquality.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.tquality.itemnames = tquality_names;
	s_graphicsoptions.tquality.generic.x = XPOSITION;
	s_graphicsoptions.tquality.generic.y = y;
	s_graphicsoptions.tquality.generic.toolTip =
		"Select a desired texture quality, 16 or 32 bit. Default means the texture quality is "
		"then determined by the graphics driver. Selecting a lower texture quality can save "
		"system resources.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_textureMode", "r_ext_texture_filter_anisotropic", r_ext_max_anisotropy
	s_graphicsoptions.tfilter.generic.type = MTYPE_SPINCONTROL;
	s_graphicsoptions.tfilter.generic.name = "Texture Filter:";
	s_graphicsoptions.tfilter.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.tfilter.itemnames = tfilter_names;
	s_graphicsoptions.tfilter.generic.x = XPOSITION;
	s_graphicsoptions.tfilter.generic.y = y;
	s_graphicsoptions.tfilter.generic.toolTip =
		"Select a desired texture filtering method bilinear, trilinear or anisotropic. Default "
		"is trilinear. Selecting anisotropic above 4x can lead to a high graphics card load.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_ext_compressed_textures"
	s_graphicsoptions.ct.generic.type = MTYPE_RADIOBUTTON;
	s_graphicsoptions.ct.generic.name = "Compress Textures:";
	s_graphicsoptions.ct.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.ct.generic.x = XPOSITION;
	s_graphicsoptions.ct.generic.y = y;
	s_graphicsoptions.ct.generic.toolTip =
		"Enable to allow the graphics card to store texture data compressed if supported. "
		"Default is off. This can reduce graphics card memory usage on weak systems.";

	y += (BIGCHAR_HEIGHT + 2);
	// references/modifies "r_ext_max_multisampling" or "r_ext_framebuffer_multisample"
	s_graphicsoptions.aa.generic.type = MTYPE_SPINCONTROL;
	s_graphicsoptions.aa.generic.name = "Antialiasing:";
	s_graphicsoptions.aa.generic.flags = QMF_SMALLFONT;
	s_graphicsoptions.aa.itemnames = aa_names;
	s_graphicsoptions.aa.generic.x = XPOSITION;
	s_graphicsoptions.aa.generic.y = y;
	s_graphicsoptions.aa.generic.toolTip =
		"Enable to switch on multisample antialiasing (MSAA) at a desired level. Default is "
		"off. Selecting higher levels results in better render quality but can lead to a high "
		"graphics card load.";

	y += (BIGCHAR_HEIGHT + 2);
	s_graphicsoptions.effects.generic.type = MTYPE_TEXTS;
	s_graphicsoptions.effects.string = "Advanced Effects";
	s_graphicsoptions.effects.fontHeight = 20.0f;
	s_graphicsoptions.effects.generic.flags = QMF_CENTER_JUSTIFY;
	s_graphicsoptions.effects.generic.x = XPOSITION;
	s_graphicsoptions.effects.generic.y = y;
	s_graphicsoptions.effects.generic.id = ID_EFFECTS;
	s_graphicsoptions.effects.generic.callback = UI_GraphicsOptions_Event;
	s_graphicsoptions.effects.color = color_lightOrange;
	s_graphicsoptions.effects.focuscolor = color_orange;
	s_graphicsoptions.effects.style = UI_CENTER | UI_SMALLFONT;

	s_graphicsoptions.back.generic.type = MTYPE_BITMAP;
	s_graphicsoptions.back.generic.name = BACK0;
	s_graphicsoptions.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_graphicsoptions.back.generic.callback = UI_GraphicsOptions_Event;
	s_graphicsoptions.back.generic.id = ID_BACK;
	s_graphicsoptions.back.generic.x = 8;
	s_graphicsoptions.back.generic.y = 446;
	s_graphicsoptions.back.width = 80;
	s_graphicsoptions.back.height = 40;
	s_graphicsoptions.back.focuspic = BACK1;
	s_graphicsoptions.back.focuspicinstead = qtrue;

	s_graphicsoptions.apply.generic.type = MTYPE_BITMAP;
	s_graphicsoptions.apply.generic.name = ACCEPT0;
	s_graphicsoptions.apply.generic.flags = QMF_PULSEIFFOCUS | QMF_HIDDEN | QMF_INACTIVE;
	s_graphicsoptions.apply.generic.callback = UI_GraphicsOptions_ApplyChanges;
	s_graphicsoptions.apply.generic.x = 736;
	s_graphicsoptions.apply.generic.y = 446;
	s_graphicsoptions.apply.width = 120;
	s_graphicsoptions.apply.height = 40;
	s_graphicsoptions.apply.focuspic = ACCEPT1;

	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.graphics);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.display);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.sound);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.network);

	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.list);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.mode);
#ifdef USE_RENDERER_DLOPEN
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.renderer);
#endif
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.colordepth);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.lighting);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.mdetail);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.cdetail);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.tdetail);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.tquality);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.tfilter);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.ct);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.aa);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.effects);

	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.back);
	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.apply);

	UI_GraphicsOptions_SetMenuItems();
	UI_GraphicsOptions_GetInitialVideo();
}

/*
=================
UI_GraphicsOptions_Cache
=================
*/
void UI_GraphicsOptions_Cache(void) {
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
=================
UI_GraphicsOptionsMenu
=================
*/
void UI_GraphicsOptionsMenu(void) {
	UI_GraphicsOptions_MenuInit();
	UI_PushMenu(&s_graphicsoptions.menu);
	Menu_SetCursorToItem(&s_graphicsoptions.menu, &s_graphicsoptions.graphics);
}
