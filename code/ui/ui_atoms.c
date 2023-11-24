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
/**********************************************************************
	UI_ATOMS.C

	User interface building blocks and support functions.
**********************************************************************/
#include "ui_local.h"

uiStatic_t uis;
static qboolean m_entersound; // after a frame, so caching won't disrupt the sound

// these are here so the functions in q_shared.c can link
#ifndef UI_HARD_LINKED

void QDECL Com_Error(int level, const char *error, ...) {
	va_list argptr;
	char text[1024];

	va_start(argptr, error);
	Q_vsnprintf(text, sizeof(text), error, argptr);
	va_end(argptr);

	trap_Error(text);
}

void QDECL Com_Printf(const char *msg, ...) {
	va_list argptr;
	char text[1024];

	va_start(argptr, msg);
	Q_vsnprintf(text, sizeof(text), msg, argptr);
	va_end(argptr);

	trap_Print(text);
}

#endif

/*
=================
UI_ClampCvar
=================
*/
float UI_ClampCvar(float min, float max, float value) {
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

/*
=================
UI_StartDemoLoop
=================
*/
void UI_StartDemoLoop(void) {
	trap_Cmd_ExecuteText(EXEC_APPEND, "d1\n");
}

void UI_StartCreditMusic(void) {
	uis.musicstate = MUSICSTATE_RUNNING;
	trap_S_StopBackgroundTrack();
	trap_S_StartBackgroundTrack("music/credits", "music/credits");
}

void UI_StartMusic(void) {
	uis.musicstate = MUSICSTATE_RUNNING;
	trap_S_StartBackgroundTrack("music/menuloop", "music/menuloop");
}

void UI_StopMusic(void) {
	if (uis.musicstate == MUSICSTATE_RUNNING) {
		trap_S_StopBackgroundTrack();
	}
	uis.musicstate = MUSICSTATE_STOPPED;
}

/*
=================
UI_PushMenu
=================
*/
void UI_PushMenu(menuframework_s *menu) {
	int i;
	menucommon_s *item;

	// avoid stacking menus invoked by hotkeys
	for (i = 0; i < uis.menusp; i++) {
		if (uis.stack[i] == menu) {
			uis.menusp = i;
			break;
		}
	}

	if (i == uis.menusp) {
		if (uis.menusp >= MAX_MENUDEPTH)
			trap_Error("UI_PushMenu: menu stack overflow");

		uis.stack[uis.menusp++] = menu;
	}

	uis.activemenu = menu;

	// default cursor position
	menu->cursor = 0;
	menu->cursor_prev = 0;

	m_entersound = qtrue;

	trap_Key_SetCatcher(KEYCATCH_UI);

	if (menu->noPushSelect) {
		menu->cursor = -1;
		//		Menu_CursorMoved( &s_main.menu );
	} else
		// force first available item to have focus
		for (i = 0; i < menu->nitems; i++) {
			item = (menucommon_s *)menu->items[i];
			if (!(item->flags & (QMF_GRAYED | QMF_MOUSEONLY | QMF_INACTIVE))) {
				menu->cursor_prev = -1;
				Menu_SetCursor(menu, i);
				break;
			}
		}

	uis.firstdraw = qtrue;
}

/*
=================
UI_PopMenu
=================
*/
void UI_PopMenu(void) {
	uis.menusp--;

	if (uis.menusp < 0)
		trap_Error("UI_PopMenu: menu stack underflow");

	if (uis.menusp) {
		uis.activemenu = uis.stack[uis.menusp - 1];
		uis.firstdraw = qtrue;
	} else {
		UI_ForceMenuOff();
	}
}

void UI_ForceMenuOff(void) {
	uis.menusp = 0;
	uis.activemenu = NULL;

	if (!UI_GetCvarInt("cl_paused"))
		UI_StopMusic();

	trap_Key_SetCatcher(trap_Key_GetCatcher() & ~KEYCATCH_UI);
	trap_Key_ClearStates();
	trap_Cvar_Set("cl_paused", "0");
}

/*
=================
UI_LerpColor
=================
*/
void UI_LerpColor(const vec4_t a, const vec4_t b, vec4_t c, float t) {
	int i;

	// lerp and clamp each component
	for (i = 0; i < 4; i++) {
		c[i] = a[i] + t * (b[i] - a[i]);
		if (c[i] < 0)
			c[i] = 0;
		else if (c[i] > 1.0)
			c[i] = 1.0;
	}
}

/*
=================
UI_DrawProportionalString2
=================
*/
static const struct FontData {
	int x;	   // x offset in font atlas
	int y;	   // y offset in font atlas
	int width; // width of the character
} propMap[128] = {
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},

	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},

	{0, 0, PROP_SPACE_WIDTH}, // SPACE
	{9, 122, 11},			  // !
	{155, 181, 13},			  // "
	{54, 122, 15},			  // #
	{81, 122, 16},			  // $
	{102, 122, 18},			  // %
	{153, 122, 19},			  // &
	{10, 93, 6},			  // '
	{207, 122, 13},			  // (
	{225, 122, 13},			  // )
	{177, 122, 11},			  // *
	{33, 152, 13},			  // +
	{85, 181, 8},			  // ,
	{32, 93, 14},			  // -
	{108, 181, 8},			  // .
	{128, 152, 18},			  // /

	{21, 64, 18},	// 0
	{40, 64, 15},	// 1
	{57, 64, 16},	// 2
	{78, 64, 15},	// 3
	{97, 64, 14},	// 4
	{116, 64, 14},	// 5
	{135, 64, 13},	// 6
	{153, 64, 16},	// 7
	{174, 64, 16},	// 8
	{194, 64, 15},	// 9
	{59, 181, 8},	// :
	{33, 181, 8},	// ;
	{203, 152, 12},	// <
	{56, 93, 15},	// =
	{225, 152, 12},	// >
	{179, 181, 16},	// ?

	{28, 122, 22}, // @
	{4, 4, 20},	   // A
	{27, 4, 15},   // B
	{48, 4, 18},   // C
	{68, 4, 16},   // D
	{89, 4, 15},   // E
	{106, 4, 15},  // F
	{122, 4, 17},  // G
	{142, 4, 17},  // H
	{163, 4, 12},  // I
	{176, 4, 17},  // J
	{196, 4, 18},  // K
	{215, 4, 15},  // L
	{232, 4, 22},  // M
	{6, 34, 18},   // N
	{27, 34, 18},  // O

	{48, 34, 15},	// P
	{67, 34, 19},	// Q
	{90, 34, 18},	// R
	{111, 34, 18},	// S
	{130, 34, 15},	// T
	{146, 34, 19},	// U
	{167, 34, 18},	// V
	{188, 34, 21},	// W
	{213, 34, 22},	// X
	{235, 34, 19},	// Y
	{4, 64, 16},	// Z
	{58, 152, 11},	// [
	{105, 152, 15}, // '\'
	{81, 152, 11},	// ]
	{128, 122, 18}, // ^
	{4, 152, 20},	// _

	{134, 181, 6}, // '
	{4, 4, 20},	   // A
	{27, 4, 15},   // B
	{48, 4, 18},   // C
	{68, 4, 16},   // D
	{89, 4, 15},   // E
	{106, 4, 15},  // F
	{122, 4, 17},  // G
	{142, 4, 17},  // H
	{163, 4, 12},  // I
	{176, 4, 17},  // J
	{196, 4, 18},  // K
	{215, 4, 15},  // L
	{232, 4, 22},  // M
	{6, 34, 18},   // N
	{27, 34, 18},  // O

	{48, 34, 15},	// P
	{67, 34, 19},	// Q
	{90, 34, 18},	// R
	{111, 34, 18},	// S
	{130, 34, 15},	// T
	{146, 34, 19},	// U
	{167, 34, 18},	// V
	{188, 34, 21},	// W
	{213, 34, 22},	// X
	{235, 34, 19},	// Y
	{4, 64, 16},	// Z
	{154, 152, 10}, // {
	{11, 181, 8},	// |
	{182, 152, 10}, // }
	{78, 93, 20},	// ~
	{0, 0, -1}		// DEL
};

int UI_ProportionalStringWidth(const char *str) {
	const char *s;
	int ch;
	int charWidth;
	int width;

	s = str;
	width = 0;
	while (*s) {
		ch = *s & 127;
		charWidth = propMap[ch].width;
		if (charWidth != -1) {
			width += charWidth;
			width += PROP_GAP_WIDTH;
		}
		s++;
	}

	width -= PROP_GAP_WIDTH;
	return width;
}

void UI_DrawProportionalString2(int x, int y, const char *str, const vec4_t color, float sizeScale, qhandle_t charset) {
	const char *s;
	unsigned char ch;
	float ax;
	float ay;
	float aw = 0;
	float ah;
	float frow;
	float fcol;
	float fwidth;
	float fheight;

	// draw the colored text
	trap_R_SetColor(color);

	ax = x * uis.xscale + uis.xbias;
	ay = y * uis.yscale + uis.ybias;

	s = str;
	while (*s) {
		ch = *s & 127;
		if (ch == ' ') {
			aw = (float)PROP_SPACE_WIDTH * uis.xscale * sizeScale;
		} else if (propMap[ch].width != -1) {
			fcol = (float)propMap[ch].x / 256.0f;
			frow = (float)propMap[ch].y / 256.0f;
			fwidth = (float)propMap[ch].width / 256.0f;
			fheight = (float)PROP_HEIGHT / 256.0f;
			aw = (float)propMap[ch].width * uis.xscale * sizeScale;
			ah = (float)PROP_HEIGHT * uis.yscale * sizeScale;
			trap_R_DrawStretchPic(ax, ay, aw, ah, fcol, frow, fcol + fwidth, frow + fheight, charset);
		}

		ax += (aw + (float)PROP_GAP_WIDTH * uis.xscale * sizeScale);
		s++;
	}

	trap_R_SetColor(NULL);
}

/*
=================
UI_ProportionalSizeScale
=================
*/
float UI_ProportionalSizeScale(int style) {
	if (style & UI_SMALLFONT) {
		return PROP_SMALL_SIZE_SCALE;
	}

	return 1.0f;
}

/*
=================
UI_DrawProportionalString
=================
*/
void UI_DrawProportionalString(int x, int y, const char *str, int style, const vec4_t color) {
	vec4_t drawcolor;
	int width;
	float sizeScale;

	sizeScale = UI_ProportionalSizeScale(style);

	switch (style & UI_FORMATMASK) {
	case UI_CENTER:
		width = UI_ProportionalStringWidth(str) * sizeScale;
		x -= width / 2;
		break;

	case UI_RIGHT:
		width = UI_ProportionalStringWidth(str) * sizeScale;
		x -= width;
		break;

	case UI_LEFT:
	default:
		break;
	}

	if (style & UI_DROPSHADOW) {
		drawcolor[0] = drawcolor[1] = drawcolor[2] = 0;
		drawcolor[3] = color[3];
		UI_DrawProportionalString2(x + 2, y + 2, str, drawcolor, sizeScale, uis.charsetProp);
	}

	if (style & UI_INVERSE) {
		drawcolor[0] = color[0] * 0.7f;
		drawcolor[1] = color[1] * 0.7f;
		drawcolor[2] = color[2] * 0.7f;
		drawcolor[3] = color[3];
		UI_DrawProportionalString2(x, y, str, drawcolor, sizeScale, uis.charsetProp);
		return;
	}

	if (style & UI_PULSE) {
		UI_DrawProportionalString2(x, y, str, color, sizeScale, uis.charsetProp);
		drawcolor[0] = color[0];
		drawcolor[1] = color[1];
		drawcolor[2] = color[2];
		drawcolor[3] = 0.75f + 0.25f * sin(uis.realtime / PULSE_DIVISOR);
		UI_DrawProportionalString2(x - 2 + (int)(random() * 5.99f), y - 2 + (int)(random() * 5.99f), str, drawcolor,
								   sizeScale, uis.charsetProp);

		return;
	}

	UI_DrawProportionalString2(x, y, str, color, sizeScale, uis.charsetProp);
}

static int CharWidthForStringStyle(int style) {
	if (style & UI_SMALLFONT)
		return SMALLCHAR_WIDTH;

	else if (style & UI_GIANTFONT)
		return GIANTCHAR_WIDTH;

	return BIGCHAR_WIDTH;
}

static int CharHeightForStringStyle(int style) {
	if (style & UI_SMALLFONT)
		return SMALLCHAR_HEIGHT;

	else if (style & UI_GIANTFONT)
		return GIANTCHAR_HEIGHT;

	return BIGCHAR_HEIGHT;
}

int UI_AutoWrappedString_LineCount(int xmax, const char *str, int style, qboolean proportional) {
	char *s1, *s2, *s3;
	char c_bcp;
	char buf[1024];
	float sizeScale;
	int lineCount = 0;
	int width;

	if (!str || str[0] == '\0')
		return 0;

	sizeScale = UI_ProportionalSizeScale(style);

	Q_strncpyz(buf, str, sizeof(buf));
	s1 = s2 = s3 = buf;

	while (1) {
		do {
			s3++;
		} while (*s3 != ' ' && *s3 != '\0' && *s3 != '\n');
		c_bcp = *s3;
		*s3 = '\0';

		if (proportional)
			width = UI_ProportionalStringWidth(s1) * sizeScale;
		else
			width = strlen(s1) * CharWidthForStringStyle(style);

		*s3 = c_bcp;
		if (width > xmax || c_bcp == '\n') {
			if (s1 == s2) {
				// fuck, don't have a clean cut, we'll overflow
				s2 = s3;
			}
			*s2 = '\0';
			lineCount++;
			if (c_bcp == '\0') {
				// that was the last word
				// we could start a new loop, but that wouldn't be much use
				// even if the word is too long, we would overflow it (see above)
				// so just print it now if needed
				s2++;
				if (*s2 != '\0') // if we are printing an overflowing line we have s2 == s3
					lineCount++;
				break;
			}
			s2++;
			s1 = s2;
			s3 = s2;
		} else {
			s2 = s3;
			if (c_bcp == '\0') // we reached the end
			{
				lineCount++;
				break;
			}
		}
	}
	return lineCount;
}

/*
=================
UI_DrawProportionalString_Wrapped
=================
*/
void UI_DrawString_AutoWrapped(int x, int y, int xmax, int ystep, const char *str, int style, const vec4_t color,
							   qboolean proportional) {
	int width;
	char *s1, *s2, *s3;
	char c_bcp;
	char buf[1024];
	float sizeScale;

	if (!str || str[0] == '\0')
		return;

	sizeScale = UI_ProportionalSizeScale(style);

	Q_strncpyz(buf, str, sizeof(buf));
	s1 = s2 = s3 = buf;

	while (1) {
		do {
			s3++;
		} while (*s3 != ' ' && *s3 != '\0' && *s3 != '\n');

		if (*s3 == '\n') {
			*s3 = '\0';

			if (proportional)
				UI_DrawProportionalString(x, y, s1, style, color);
			else
				UI_DrawString(x, y, s1, style, color);

			y += ystep;
			s3++;
			s1 = s2 = s3;
			continue;
		}

		c_bcp = *s3;
		*s3 = '\0';

		if (proportional)
			width = UI_ProportionalStringWidth(s1) * sizeScale;
		else
			width = strlen(s1) * CharWidthForStringStyle(style);

		*s3 = c_bcp;
		if (width > xmax) {
			if (s1 == s2) {
				// fuck, don't have a clean cut, we'll overflow
				s2 = s3;
			}
			*s2 = '\0';

			if (proportional)
				UI_DrawProportionalString(x, y, s1, style, color);
			else
				UI_DrawString(x, y, s1, style, color);

			y += ystep;
			if (c_bcp == '\0') {
				// that was the last word
				// we could start a new loop, but that wouldn't be much use
				// even if the word is too long, we would overflow it (see above)
				// so just print it now if needed
				s2++;
				if (*s2 != '\0') // if we are printing an overflowing line we have s2 == s3
				{
					if (proportional)
						UI_DrawProportionalString(x, y, s2, style, color);
					else
						UI_DrawString(x, y, s2, style, color);
				}
				break;
			}
			s2++;
			s1 = s2;
			s3 = s2;
		} else {
			s2 = s3;
			if (c_bcp == '\0') // we reached the end
			{
				if (proportional)
					UI_DrawProportionalString(x, y, s1, style, color);
				else
					UI_DrawString(x, y, s1, style, color);
				break;
			}
		}
	}
}

/*
=================
UI_DrawString2
=================
*/
static void UI_DrawString2(int x, int y, const char *str, const vec4_t color, int charw, int charh) {
	const char *s;
	char ch;
	int forceColor = qfalse; // APSFIXME;
	vec4_t tempcolor;
	float ax;
	float ay;
	float aw;
	float ah;
	float frow;
	float fcol;

	if (y < -charh)
		// offscreen
		return;

	// draw the colored text
	trap_R_SetColor(color);

	ax = x * uis.xscale + uis.xbias;
	ay = y * uis.yscale + uis.ybias;

	aw = charw * uis.xscale;
	ah = charh * uis.yscale;

	s = str;
	while (*s) {
		if (Q_IsColorString(s)) {
			if (!forceColor) {
				memcpy(tempcolor, g_color_table[ColorIndex(s[1])], sizeof(tempcolor));
				tempcolor[3] = color[3];
				trap_R_SetColor(tempcolor);
			}
			s += 2;
			continue;
		}

		ch = *s & 255;
		if (ch != ' ') {
			frow = (ch >> 4) * 0.0625f;
			fcol = (ch & 15) * 0.0625f;
			trap_R_DrawStretchPic(ax, ay, aw, ah, fcol, frow, fcol + 0.0625f, frow + 0.0625f, uis.charsetShader);
		}

		ax += aw;
		s++;
	}

	trap_R_SetColor(NULL);
}

/*
=================
UI_DrawString
=================
*/
void UI_DrawString(int x, int y, const char *str, int style, const vec4_t color) {
	int len;
	int charw;
	int charh;
	vec4_t newcolor;
	vec4_t lowlight;
	float *drawcolor;
	vec4_t dropcolor;

	if (!str) {
		return;
	}

	if ((style & UI_BLINK) && ((uis.realtime / BLINK_DIVISOR) & 1))
		return;

	charw = CharWidthForStringStyle(style);
	charh = CharHeightForStringStyle(style);

	if (style & UI_PULSE) {
		lowlight[0] = 0.8f * color[0];
		lowlight[1] = 0.8f * color[1];
		lowlight[2] = 0.8f * color[2];
		lowlight[3] = 0.8f * color[3];
		UI_LerpColor(color, lowlight, newcolor, (float)(0.5 + 0.5 * sin(uis.realtime / PULSE_DIVISOR)));
	} else {
		Vector4Copy(color, newcolor);
	}
	drawcolor = newcolor;

	switch (style & UI_FORMATMASK) {
	case UI_CENTER:
		// center justify at x
		len = strlen(str);
		x = x - len * charw / 2;
		break;

	case UI_RIGHT:
		// right justify at x
		len = strlen(str);
		x = x - len * charw;
		break;

	default:
		// left justify at x
		break;
	}

	if (style & UI_DROPSHADOW) {
		dropcolor[0] = dropcolor[1] = dropcolor[2] = 0;
		dropcolor[3] = drawcolor[3];
		UI_DrawString2(x + 2, y + 2, str, dropcolor, charw, charh);
	}

	UI_DrawString2(x, y, str, drawcolor, charw, charh);
}

/*
=================
UI_DrawStringNS
=================
*/
void UI_DrawStringNS(int x, int y, const char *str, int style, float fontsize, const vec4_t color) {
	int len;
	int charw;
	int charh;
	vec4_t newcolor;
	vec4_t lowlight;
	const float *drawcolor;
	vec4_t dropcolor;

	if (!str) {
		return;
	}

	if ((style & UI_BLINK) && ((uis.realtime / BLINK_DIVISOR) & 1))
		return;

	if (style & UI_NS_STR_HGW) // height = width
	{
		charw = charh = fontsize;
	} else // height/2 = width
	{
		charw = fontsize / 2;
		charh = fontsize;
	}

	if (style & UI_PULSE) {
		lowlight[0] = 0.8f * color[0];
		lowlight[1] = 0.8f * color[1];
		lowlight[2] = 0.8f * color[2];
		lowlight[3] = 0.8f * color[3];
		UI_LerpColor(color, lowlight, newcolor, 0.5 + 0.5 * sin(uis.realtime / PULSE_DIVISOR));
		drawcolor = newcolor;
	} else
		drawcolor = color;

	switch (style & UI_FORMATMASK) {
	case UI_CENTER:
		// center justify at x
		len = strlen(str);
		x = x - len * charw / 2;
		break;

	case UI_RIGHT:
		// right justify at x
		len = strlen(str);
		x = x - len * charw;
		break;

	default:
		// left justify at x
		break;
	}

	if (style & UI_DROPSHADOW) {
		dropcolor[0] = dropcolor[1] = dropcolor[2] = 0;
		dropcolor[3] = drawcolor[3];
		UI_DrawString2(x + 2, y + 2, str, dropcolor, charw, charh);
	}

	UI_DrawString2(x, y, str, drawcolor, charw, charh);
}

/*
=================
UI_DrawChar
=================
*/
void UI_DrawChar(int x, int y, int ch, int style, const vec4_t color) {
	char buff[2];

	buff[0] = ch;
	buff[1] = '\0';

	UI_DrawString(x, y, buff, style, color);
}

qboolean UI_IsFullscreen(void) {
	if (uis.activemenu && (trap_Key_GetCatcher() & KEYCATCH_UI)) {
		return uis.activemenu->fullscreen;
	}

	return qfalse;
}

void UI_SetActiveMenu(uiMenuCommand_t menu) {
	// this should be the ONLY way the menu system is brought up
	// ensure minimum menu data is cached
	Menu_Cache();

	switch (menu) {
	case UIMENU_NONE:
		UI_ForceMenuOff();
		return;
	case UIMENU_MAIN:
		UI_MainMenu();
		return;
	case UIMENU_INGAME:
		trap_Cvar_Set("cl_paused", "1");
		UI_InGameMenu();
		return;

	// bk001204
	case UIMENU_TEAM:
	case UIMENU_POSTGAME:
	default:
#ifndef NDEBUG
		Com_Printf("UI_SetActiveMenu: bad enum %d\n", menu);
#endif
		break;
	}
}

/*
=================
UI_KeyEvent
=================
*/
void UI_KeyEvent(int key, int down) {
	sfxHandle_t s;

	if (!uis.activemenu) {
		return;
	}

	if (!down) {
		return;
	}

	if (uis.activemenu->key)
		s = uis.activemenu->key(key);
	else
		s = Menu_DefaultKey(uis.activemenu, key);

	if ((s > 0) && (s != menuNullSound))
		trap_S_StartLocalSound(s, CHAN_LOCAL_SOUND);
}

/*
=================
UI_MouseEvent
=================
*/
void UI_MouseEvent(int dx, int dy) {
	int i;
	int xbias;
	int ybias;

	menucommon_s *m;

	if (!uis.activemenu)
		return;

	// convert X Y bias to 640 coords
	xbias = uis.xbias / uis.xscale;
	ybias = uis.ybias / uis.yscale;

	// update mouse screen position
	uis.cursorx += dx;
	if (uis.cursorx < -xbias)
		uis.cursorx = -xbias;
	else if (uis.cursorx > SCREEN_WIDTH + xbias)
		uis.cursorx = SCREEN_WIDTH + xbias;

	uis.cursory += dy;
	if (uis.cursory < -ybias)
		uis.cursory = -ybias;
	else if (uis.cursory > SCREEN_HEIGHT + ybias)
		uis.cursory = SCREEN_HEIGHT + ybias;

	if (uis.dropdownlist)
		return;

	// dragable slider ;)
	if (uis.activemenu->cursor >= 0 && trap_Key_IsDown(K_MOUSE1) &&
		uis.activemenu->items[uis.activemenu->cursor] != NULL &&
		((menucommon_s *)(uis.activemenu->items[uis.activemenu->cursor]))->type == MTYPE_SLIDER) {
		menuslider_s *s = uis.activemenu->items[uis.activemenu->cursor];
		const int x = uis.cursorx - s->generic.x - 2 * SMALLCHAR_WIDTH;
		const float oldvalue = s->curvalue;
		s->curvalue = (x / (float)(SLIDER_RANGE * SMALLCHAR_WIDTH)) * (s->maxvalue - s->minvalue) + s->minvalue;
		if (s->curvalue < s->minvalue)
			s->curvalue = s->minvalue;
		else if (s->curvalue > s->maxvalue)
			s->curvalue = s->maxvalue;
		if (s->curvalue != oldvalue && s->generic.callback)
			s->generic.callback(s, QM_ACTIVATED);
	}

	// region test the active menu items
	//	for (i=0; i<uis.activemenu->nitems; i++)
	// draw order == selection order ...
	for (i = uis.activemenu->nitems - 1; i >= 0; --i) {
		m = (menucommon_s *)uis.activemenu->items[i];

		if (m->flags & (QMF_GRAYED | QMF_INACTIVE))
			continue;

		if ((uis.cursorx < m->left) || (uis.cursorx > m->right) || (uis.cursory < m->top) ||
			(uis.cursory > m->bottom)) {
			// cursor out of item bounds
			continue;
		}

		// set focus to item at cursor
		if (uis.activemenu->cursor != i) {
			Menu_SetCursor(uis.activemenu, i);
			if (uis.activemenu->cursor_prev >= 0 && uis.activemenu->cursor_prev < uis.activemenu->nitems)
				((menucommon_s *)(uis.activemenu->items[uis.activemenu->cursor_prev]))->flags &= ~QMF_HASMOUSEFOCUS;

			if (!(((menucommon_s *)(uis.activemenu->items[uis.activemenu->cursor]))->flags & QMF_SILENT)) {
				trap_S_StartLocalSound(menuMoveSound, CHAN_LOCAL_SOUND);
			}
		}

		((menucommon_s *)(uis.activemenu->items[uis.activemenu->cursor]))->flags |= QMF_HASMOUSEFOCUS;
		return;
	}

	if (uis.activemenu->cursor >= 0 && uis.activemenu->cursor < uis.activemenu->nitems) {
		// out of any region
		((menucommon_s *)(uis.activemenu->items[uis.activemenu->cursor]))->flags &= ~QMF_HASMOUSEFOCUS;
	}
}

const char *UI_Argv(int arg) {
	static char buffer[MAX_STRING_CHARS];

	trap_Argv(arg, buffer, sizeof(buffer));

	return buffer;
}

const char *UI_Cvar_VariableString(const char *var_name) {
	static char buffer[MAX_STRING_CHARS];

	trap_Cvar_VariableStringBuffer(var_name, buffer, sizeof(buffer));

	return buffer;
}

#if 0
/*
=================
UI_Cache
=================
*/
void UI_Cache_f(void) {
	UI_MainMenu_Cache();
	UI_InGame_Cache();
	UI_ConfirmMenu_Cache();
	UI_PlayerSettings_Cache();
	UI_Controls_Cache();
	UI_Demos_Cache();
	UI_Preferences_Cache();
	UI_ServerInfo_Cache();
	UI_SpecifyServer_Cache();
	UI_ArenaServers_Cache();
	UI_StartServer_Cache();

	UI_GraphicsOptions_Cache();
	UI_DisplayOptions_Cache();
	UI_EffectsOptions_Cache();
	UI_SoundOptions_Cache();
	UI_NetworkOptions_Cache();
	UI_TeamMain_Cache();
	UI_AddBots_Cache();
	UI_RemoveBots_Cache();
	UI_SetupMenu_Cache();

	UI_SetupDefaultMenu_Cache();
	UI_WopCredits_Cache();
	UI_Credit_Cache();

	UI_SelectBots_Cache();
	UI_ModsMenu_Cache();
}
#endif

/*
=================
UI_ConsoleCommand
=================
*/
qboolean UI_ConsoleCommand(int realTime) {
	const char *cmd;

	uis.frametime = realTime - uis.realtime;
	uis.realtime = realTime;

	cmd = UI_Argv(0);

	// ensure minimum menu data is available
	Menu_Cache();

	// this is still needed, but its ingame menu entry is disabled
	if (Q_stricmp(cmd, "ui_teamOrders") == 0) {
		UI_TeamOrdersMenu_f();
		return qtrue;
	}
	if (Q_stricmp(cmd, "wop_music") == 0) {
		UI_MusicBox_Open();
		return qtrue;
	}
	if (Q_stricmp(cmd, "wop_checkmusic") == 0) {
		UI_MusicBox_Check();
		return qtrue;
	}
	if (Q_stricmp(cmd, "wop_nextsong") == 0) {
		UI_MusicBox_NextTrack();
		return qtrue;
	}
	if (Q_stricmp(cmd, "wop_restartmusic") == 0) {
		UI_MusicBox_TriggerRestart();
		return qtrue;
	}

	if (Q_stricmp(cmd, "ui_help") == 0) {
		UI_HelpMenu_f();
		return qtrue;
	}
	return qfalse;
}

/*
=================
UI_Shutdown
=================
*/
void UI_Shutdown(void) {
	UI_StopMusic();
	UI_MusicBox_Shutdown();
}

/*
=================
UI_Init
=================
*/
void UI_Init(void) {
	UI_RegisterCvars();

	// not yet
	// WOP_LoadMenuText(UI_LOCALEFILE, qtrue);

	UI_SetDefaultBinds_onUnusedKeys();

	if (s_wop_restarted.integer != qtrue) {
		trap_Cvar_Set("s_wop_restarted", "1");
		// erstmal rausgenommen fuer WoP-Engine
		//		trap_Cmd_ExecuteText( EXEC_NOW, "snd_restart\n" );//noch testen ob das nicht zu radikal ist, auch da
		//return! ... old: EXEC_APPEND 		return;
	}

	UI_MusicBox_Init();

	UI_InitGameinfo();

	// cache redundant calulations
	trap_GetGlconfig(&uis.glconfig);

	// for 640x480 virtualized screen
	uis.xscale = uis.glconfig.vidWidth * (1.0f / (float)SCREEN_WIDTH);
	uis.yscale = uis.glconfig.vidHeight * (1.0f / (float)SCREEN_HEIGHT);

	if (uis.glconfig.vidWidth * (float)SCREEN_HEIGHT > uis.glconfig.vidHeight * (float)SCREEN_WIDTH) {
		// wide screen
		uis.xbias = 0.5f * (uis.glconfig.vidWidth - (uis.glconfig.vidHeight * ((float)SCREEN_WIDTH/(float)SCREEN_HEIGHT)));
		uis.xscale = uis.yscale;
		uis.ybias = 0;
	} else {
		// no wide screen
		uis.xbias = 0;

		// narrow screen
		uis.ybias = 0.5 * (uis.glconfig.vidHeight - (uis.glconfig.vidWidth * ((float)SCREEN_HEIGHT/(float)SCREEN_WIDTH)));
		uis.yscale = uis.xscale;
	}
/*
	if (uis.glconfig.vidWidth * 480 < uis.glconfig.vidHeight * 640) {
		uis.ybias = 0.5f * (uis.glconfig.vidHeight - (uis.glconfig.vidWidth * (480.0f / 640.0f)));
		uis.yscale = uis.xscale;
	} else
		uis.ybias = 0;

*/
	// initialize the menu system
	Menu_Cache();

	uis.activemenu = NULL;
	uis.menusp = 0;
}

/*
================
UI_AdjustFrom640

Adjusted for resolution and screen aspect ratio
================
*/
void UI_AdjustFrom640(float *x, float *y, float *w, float *h) {
	// expect valid pointers
	*x = *x * uis.xscale + uis.xbias;
	*y = *y * uis.yscale + uis.ybias;
	*w *= uis.xscale;
	*h *= uis.yscale;
}

void UI_DrawNamedPic(float x, float y, float width, float height, const char *picname) {
	qhandle_t hShader;

	hShader = trap_R_RegisterShaderNoMip(picname);
	UI_AdjustFrom640(&x, &y, &width, &height);
	trap_R_DrawStretchPic(x, y, width, height, 0, 0, 1, 1, hShader);
}

void UI_DrawHandlePic(float x, float y, float w, float h, qhandle_t hShader) {
	float s0;
	float s1;
	float t0;
	float t1;

	if (w < 0) { // flip about vertical
		w = -w;
		s0 = 1;
		s1 = 0;
	} else {
		s0 = 0;
		s1 = 1;
	}

	if (h < 0) { // flip about horizontal
		h = -h;
		t0 = 1;
		t1 = 0;
	} else {
		t0 = 0;
		t1 = 1;
	}

	UI_AdjustFrom640(&x, &y, &w, &h);
	trap_R_DrawStretchPic(x, y, w, h, s0, t0, s1, t1, hShader);
}

/*
================
UI_FillRect

Coordinates are 640*480 virtual values
=================
*/
void UI_FillRect(float x, float y, float width, float height, const float *color) {
	trap_R_SetColor(color);

	UI_AdjustFrom640(&x, &y, &width, &height);
	trap_R_DrawStretchPic(x, y, width, height, 0, 0, 0, 0, uis.whiteShader);

	trap_R_SetColor(NULL);
}

/*
================
UI_DrawRect

Coordinates are 640*480 virtual values
=================
*/
void UI_DrawRect(float x, float y, float width, float height, const float *color, int thickness) {
	trap_R_SetColor(color);

	UI_AdjustFrom640(&x, &y, &width, &height);

	trap_R_DrawStretchPic(x, y, width, thickness, 0, 0, 0, 0, uis.whiteShader);
	trap_R_DrawStretchPic(x, y, thickness, height, 0, 0, 0, 0, uis.whiteShader);
	trap_R_DrawStretchPic(x, y + height - thickness, width, thickness, 0, 0, 0, 0, uis.whiteShader);
	trap_R_DrawStretchPic(x + width - thickness, y, thickness, height, 0, 0, 0, 0, uis.whiteShader);

	trap_R_SetColor(NULL);
}

void UI_SetColor(const float *rgba) {
	trap_R_SetColor(rgba);
}

void UI_UpdateScreen(void) {
	trap_UpdateScreen();
}

void UI_DrawIngameBG(void) {
	UI_DrawHandlePic(((SCREEN_WIDTH - 648) / 2), 0, (SCREEN_WIDTH - 216), SCREEN_HEIGHT, uis.ingamebg);
}

static void UI_DrawMenu(menuframework_s *menu) {
	if (menu) {
		if (menu->bgparts & BGP_LASTMENU) {
			if (uis.menusp - 2 > 0) {
				UI_DrawMenu(uis.stack[uis.menusp - 2]);
			}
		} else if (menu->fullscreen) {
			// clear edge if window is different aspect than UI
			// Clearing for 4:3 xbias is handled in SCR_DrawScreenField().
			if (uis.xbias || uis.ybias) {
				trap_R_SetColor(g_color_table[0]);
				trap_R_DrawStretchPic(0, 0, uis.glconfig.vidWidth, uis.glconfig.vidHeight, 0, 0, 0, 0, uis.whiteShader);
				trap_R_SetColor(NULL);
			}

			// draw the background
			if (menu->bgparts) {
				if (menu->bgparts & BGP_MAINFX)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.mainbgfx);
				if (menu->bgparts & BGP_MENUFX)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.menubgfx);
				if (menu->bgparts & BGP_MAIN)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.mainbg);
				if (menu->bgparts & BGP_SETUP)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.setupbg);
				if (menu->bgparts & BGP_SYSTEM)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.systembg);
				if (menu->bgparts & BGP_DEFAULTS)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.defaultsbg);
				if (menu->bgparts & BGP_CONTROLS)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.controlsbg);
				if (menu->bgparts & BGP_PLAYER)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.playerbg);
				if (menu->bgparts & BGP_SERVERS)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.serversbg);
				if (menu->bgparts & BGP_MODS)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.modsbg);
				if (menu->bgparts & BGP_DEMOS)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.demosbg);
				if (menu->bgparts & BGP_SPECIFY)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.specifybg);
				if (menu->bgparts & BGP_PREFERENCES)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.preferencesbg);
				if (menu->bgparts & BGP_STARTSERVER)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.startserverbg);
				if (menu->bgparts & BGP_SELECTBOTS)
					UI_DrawHandlePic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.selectbotsbg);
			} else {
				UI_FillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, colorBlack);
			}
		}

		if (menu->draw)
			menu->draw();
		else
			Menu_Draw(menu);

		if (uis.firstdraw) {
			UI_MouseEvent(0, 0);
			uis.firstdraw = qfalse;
		}
	}
}

/*
=================
UI_Refresh
=================
*/
void UI_Refresh(int realtime) {
	uis.frametime = realtime - uis.realtime;
	uis.realtime = realtime;

	if (!(trap_Key_GetCatcher() & KEYCATCH_UI)) {
		return;
	}

	UI_UpdateCvars();

	if (!trap_Cvar_VariableValue("cl_paused")) {
		if (uis.musicstate == MUSICSTATE_STOPPED) {
			UI_StartMusic();
		}
	} else {
		UI_MusicBox_Check();
	}

	UI_DrawMenu(uis.activemenu);

	if (!uis.activemenu || !uis.activemenu->noMouseCursor) {
		// draw cursor
		UI_SetColor(NULL);
		UI_DrawHandlePic(uis.cursorx - 16, uis.cursory - 16, 32, 32, uis.cursor);
	}

	//	{ vec4_t	twhite = { 1,1,1,0.5f}; UI_DrawString(300,0,GAME_VERSION,UI_SMALLFONT,twhite); }

#ifndef NDEBUG
	if (uis.debug) {
		// cursor coordinates
		int y;
		char tmpstr[16];
		int len;

		y = trap_Key_GetCatcher() & KEYCATCH_CONSOLE ? 240 : 1;
		Com_sprintf(tmpstr, 16, "(%i,%i)", uis.cursorx, uis.cursory);
		len = strlen(tmpstr);
		UI_FillRect(1, y, 8 * len + 2, 18, colorWhite);
		UI_DrawRect(1, y, 8 * len + 2, 18, colorBlack, 1);
		UI_DrawString(2, y + 1, tmpstr, UI_LEFT | UI_SMALLFONT, colorBlack);
	}
#endif

	// delay playing the enter sound until after the
	// menu has been drawn, to avoid delay while
	// caching images
	if (m_entersound) {
		m_entersound = qfalse;
	}
}

void UI_DrawTextBox(int x, int y, int width, int lines) {
	UI_FillRect(x + BIGCHAR_WIDTH / 2, y + BIGCHAR_HEIGHT / 2, (width + 1) * BIGCHAR_WIDTH,
				(lines + 1) * BIGCHAR_HEIGHT, colorBlack);
	UI_DrawRect(x + BIGCHAR_WIDTH / 2, y + BIGCHAR_HEIGHT / 2, (width + 1) * BIGCHAR_WIDTH,
				(lines + 1) * BIGCHAR_HEIGHT, colorWhite, 1);
}

qboolean UI_CursorInRect(int x, int y, int width, int height) {
	if (uis.cursorx < x || uis.cursory < y || uis.cursorx > x + width || uis.cursory > y + height)
		return qfalse;

	return qtrue;
}

/*
#######################
UI_ModelIcon
based on UI_SelectBots_ServerPlayerIcon(ui_startserver.c)
#######################
*/
void UI_ModelIcon(const char *modelAndSkin, char *iconName, int SizeOfIconName) {
	char *skin;
	char model[MAX_QPATH];

	Q_strncpyz(model, modelAndSkin, sizeof(model));
	skin = strrchr(model, '/');
	if (skin) {
		*skin++ = '\0';
	} else {
		skin = "default";
	}

	Com_sprintf(iconName, SizeOfIconName, "models/wop_players/%s/icon_%s", model, skin);

	if (!trap_R_RegisterShaderNoMip(iconName) && Q_stricmp(skin, "default") != 0) {
		Com_sprintf(iconName, SizeOfIconName, "models/wop_players/%s/icon_default", model);
	}
}

/*
#######################
BotListCompare
used for qsort in startserver2- and addbots-menu
#######################
*/
// Ente requested to sort some main characters to the top ...
static int GetBotListSpecialScore(const char *name) {
	if (!Q_stricmp(name, "padman"))
		return 11;
	else if (!Q_stricmp(name, "padgirl"))
		return 10;
	else if (!Q_stricmp(name, "monsterpad"))
		return 9;
	else if (!Q_stricmp(name, "piratepad"))
		return 8;
	else if (!Q_stricmp(name, "padlilly"))
		return 7;
	else if (!Q_stricmp(name, "fatpad"))
		return 6;
	else if (!Q_stricmp(name, "beachpad"))
		return 5;
	else if (!Q_stricmp(name, "paddybell"))
		return 4;
	else if (!Q_stricmp(name, "padcho"))
		return 3;
	else if (!Q_stricmp(name, "padking"))
		return 2;
	else if (!Q_stricmp(name, "padpunk"))
		return 1;
	else
		return 0;
}
int QDECL BotListCompare(const void *arg1, const void *arg2) {
	int num1, num2;
	const char *info1, *info2;
	const char *name1, *name2;

	num1 = *(const int *)arg1;
	num2 = *(const int *)arg2;

	info1 = UI_GetBotInfoByNumber(num1);
	info2 = UI_GetBotInfoByNumber(num2);

	name1 = Info_ValueForKey(info1, "name");
	name2 = Info_ValueForKey(info2, "name");

	{
		const int score1 = GetBotListSpecialScore(name1);
		const int score2 = GetBotListSpecialScore(name2);
		if (score1 || score2) {
			return score2 - score1;
		}
	}

	return Q_stricmp(name1, name2);
}
