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
// cg_drawtools.c -- helper functions called by cg_draw, cg_scoreboard, cg_info, etc
#include "cg_local.h"

static screenPlacement_e cg_horizontalPlacement = PLACE_CENTER;
static screenPlacement_e cg_verticalPlacement = PLACE_CENTER;
static screenPlacement_e cg_lastHorizontalPlacement = PLACE_CENTER;
static screenPlacement_e cg_lastVerticalPlacement = PLACE_CENTER;

/*
================
CG_SetScreenPlacement
================
*/
void CG_SetScreenPlacement(screenPlacement_e hpos, screenPlacement_e vpos)
{
	cg_lastHorizontalPlacement = cg_horizontalPlacement;
	cg_lastVerticalPlacement = cg_verticalPlacement;

	cg_horizontalPlacement = hpos;
	cg_verticalPlacement = vpos;
}

/*
================
CG_PopScreenPlacement
================
*/
void CG_PopScreenPlacement(void)
{
	cg_horizontalPlacement = cg_lastHorizontalPlacement;
	cg_verticalPlacement = cg_lastVerticalPlacement;
}

/*
================
CG_GetScreenHorizontalPlacement
================
*/
screenPlacement_e CG_GetScreenHorizontalPlacement(void)
{
	return cg_horizontalPlacement;
}

/*
================
CG_GetScreenVerticalPlacement
================
*/
screenPlacement_e CG_GetScreenVerticalPlacement(void)
{
	return cg_verticalPlacement;
}


/*
================
CG_AdjustFrom640

Adjusted for resolution and screen aspect ratio

See CG_NativeResTo640()
================
*/
void CG_AdjustFrom640(float *x, float *y, float *w, float *h) {
	// scale for screen sizes (aspect correct)
	if (x) {
		*x *= cgs.screenXScale;
		if (cg_horizontalPlacement == PLACE_CENTER) {
			*x += cgs.screenXBias;
		} else if (cg_horizontalPlacement == PLACE_RIGHT) {
			*x += cgs.screenXBias*2;
		}
	}
	if (w) {
		*w *= cgs.screenXScale;
	}

	if (y) {
		*y *= cgs.screenYScale;
		if (cg_verticalPlacement == PLACE_CENTER) {
			*y += cgs.screenYBias;
		} else if (cg_verticalPlacement == PLACE_BOTTOM) {
			*y += cgs.screenYBias*2;
		}
	}
	if (h) {
		*h *= cgs.screenYScale;
	}
}

/*
================
CG_NativeResTo640

Adjust native window position to current widescreen placement

See CG_AdjustFrom640()
================
*/
void CG_NativeResTo640(float *x, float *y, float *w, float *h) {
	if (x) {
		if (cg_horizontalPlacement == PLACE_CENTER) {
			*x -= cgs.screenXBias;
		} else if (cg_horizontalPlacement == PLACE_RIGHT) {
			*x -= cgs.screenXBias*2;
		}
		*x /= cgs.screenXScale;
	}
	if (w) {
		*w /= cgs.screenXScale;
	}

	if (y) {
		if (cg_verticalPlacement == PLACE_CENTER) {
			*y -= cgs.screenYBias;
		} else if (cg_verticalPlacement == PLACE_BOTTOM) {
			*y -= cgs.screenYBias*2;
		}
		*y /= cgs.screenYScale;
	}
	if (h) {
		*h /= cgs.screenYScale;
	}
}

/*
================
CG_FillRect

Coordinates are 640*480 virtual values
=================
*/
void CG_FillRect(float x, float y, float width, float height, const float *color) {
	trap_R_SetColor(color);

	CG_AdjustFrom640(&x, &y, &width, &height);
	trap_R_DrawStretchPic(x, y, width, height, 0, 0, 0, 0, cgs.media.whiteShader);

	trap_R_SetColor(NULL);
}

/*
================
CG_DrawSides

Coords are virtual 640x480
================
*/
void CG_DrawSides(float x, float y, float w, float h, float size) {
	CG_AdjustFrom640(&x, &y, &w, &h);
	size *= cgs.screenXScale;
	trap_R_DrawStretchPic(x, y, size, h, 0, 0, 0, 0, cgs.media.whiteShader);
	trap_R_DrawStretchPic(x + w - size, y, size, h, 0, 0, 0, 0, cgs.media.whiteShader);
}

void CG_DrawTopBottom(float x, float y, float w, float h, float size) {
	CG_AdjustFrom640(&x, &y, &w, &h);
	size *= cgs.screenYScale;
	trap_R_DrawStretchPic(x, y, w, size, 0, 0, 0, 0, cgs.media.whiteShader);
	trap_R_DrawStretchPic(x, y + h - size, w, size, 0, 0, 0, 0, cgs.media.whiteShader);
}
/*
================
UI_DrawRect

Coordinates are 640*480 virtual values
=================
*/
void CG_DrawRect(float x, float y, float width, float height, float size, const float *color) {
	trap_R_SetColor(color);

	CG_DrawTopBottom(x, y, width, height, size);
	CG_DrawSides(x, y + size, width, height - size * 2, size);

	trap_R_SetColor(NULL);
}

/*
================
CG_DrawPic

Coordinates are 640*480 virtual values
=================
*/
void CG_DrawPic(float x, float y, float width, float height, qhandle_t hShader) {
	CG_AdjustFrom640(&x, &y, &width, &height);
	trap_R_DrawStretchPic(x, y, width, height, 0, 0, 1, 1, hShader);
}

/*
===============
CG_DrawChar

Coordinates and size in 640*480 virtual screen size
===============
*/
static void CG_DrawChar(int x, int y, fontSize_t fontsize, int ch) {
	int row, col;
	float frow, fcol;
	float size;
	float ax, ay, aw, ah;

	ch &= 255;

	if (ch == ' ') {
		return;
	}

	// For some reason, newline maps to a char
	if (ch == '\n') {
		return;
	}

	ax = x;
	ay = y;
	aw = fontsize.w;
	ah = fontsize.h;
	CG_AdjustFrom640(&ax, &ay, &aw, &ah);

	row = ch >> 4;
	col = ch & 15;

	frow = row * 0.0625f;
	fcol = col * 0.0625f;
	size = 0.0625f;

	trap_R_DrawStretchPic(ax, ay, aw, ah, fcol, frow, fcol + size, frow + size, cgs.media.charsetShader);
}

/*
==================
CG_DrawStringExt

Draws a multi-colored string with a drop shadow, optionally forcing
to a fixed color.

Coordinates are at 640 by 480 virtual resolution
==================
*/
void CG_DrawStringExt(int x, int y, const char *string, const float *setColor, qboolean forceColor, qboolean shadow,
					  fontSize_t fontsize, int maxChars) {
	vec4_t color;
	const char *s;
	int xx;
	int cnt;

	if (maxChars <= 0)
		maxChars = 32767; // do them all!

	// draw the drop shadow
	if (shadow) {
		color[0] = color[1] = color[2] = 0;
		color[3] = setColor[3];
		trap_R_SetColor(color);
		s = string;
		xx = x;
		cnt = 0;
		while (*s && cnt < maxChars) {
			if (Q_IsColorString(s)) {
				s += 2;
				continue;
			}
			CG_DrawChar(xx + 1, y + 1, fontsize, *s);
			cnt++;
			xx += fontsize.w;
			s++;
		}
	}

	// draw the colored text
	s = string;
	xx = x;
	cnt = 0;
	trap_R_SetColor(setColor);
	while (*s && cnt < maxChars) {
		if (Q_IsColorString(s)) {
			if (!forceColor) {
				memcpy(color, g_color_table[ColorIndex(*(s + 1))], sizeof(color));
				color[3] = setColor[3];
				trap_R_SetColor(color);
			}
			s += 2;
			continue;
		}
		CG_DrawChar(xx, y, fontsize, *s);
		xx += fontsize.w;
		cnt++;
		s++;
	}
	trap_R_SetColor(NULL);
}

void CG_DrawBigString(int x, int y, const char *s, float alpha) {
	float color[4];

	color[0] = color[1] = color[2] = 1.0f;
	color[3] = alpha;
	CG_DrawStringExt(x, y, s, color, qfalse, qtrue, FONT_BIG, 0);
}

void CG_DrawBigStringColor(int x, int y, const char *s, const vec4_t color) {
	CG_DrawStringExt(x, y, s, color, qtrue, qtrue, FONT_BIG, 0);
}

void CG_DrawSmallString(int x, int y, const char *s, float alpha) {
	vec4_t color;

	color[0] = color[1] = color[2] = 1.0f;
	color[3] = alpha;
	CG_DrawStringExt(x, y, s, color, qfalse, qfalse, FONT_SMALL, 0);
}

void CG_DrawSmallStringColor(int x, int y, const char *s, const vec4_t color) {
	CG_DrawStringExt(x, y, s, color, qtrue, qfalse, FONT_SMALL, 0);
}

/*
=================
CG_DrawStrlen

Returns character count, skiping color escape codes
=================
*/
int CG_DrawStrlen(const char *str) {
	const char *s = str;
	int count = 0;

	while (*s) {
		if (Q_IsColorString(s)) {
			s += 2;
		} else {
			count++;
			s++;
		}
	}

	return count;
}

/*
=============
CG_TileClearBox

This repeats a 64*64 tile graphic to fill the screen around a sized down
refresh window.
=============
*/
static void CG_TileClearBox(int x, int y, int w, int h, qhandle_t hShader) {
	float s1, t1, s2, t2;

	s1 = x / 64.0f;
	t1 = y / 64.0f;
	s2 = (x + w) / 64.0f;
	t2 = (y + h) / 64.0f;
	trap_R_DrawStretchPic(x, y, w, h, s1, t1, s2, t2, hShader);
}

/*
==============
CG_TileClear

Clear around a sized down screen
==============
*/
void CG_TileClear(void) {
	int top, bottom, left, right;
	int w, h;

	w = cgs.glconfig.vidWidth;
	h = cgs.glconfig.vidHeight;

	if (cg.refdef.x == 0 && cg.refdef.y == 0 && cg.refdef.width == w && cg.refdef.height == h) {
		return; // full screen rendering
	}

	top = cg.refdef.y;
	bottom = top + cg.refdef.height - 1;
	left = cg.refdef.x;
	right = left + cg.refdef.width - 1;

	// clear above view screen
	CG_TileClearBox(0, 0, w, top, cgs.media.backTileShader);

	// clear below view screen
	CG_TileClearBox(0, bottom, w, h - bottom, cgs.media.backTileShader);

	// clear left of view screen
	CG_TileClearBox(0, top, left, bottom - top + 1, cgs.media.backTileShader);

	// clear right of view screen
	CG_TileClearBox(right, top, w - right, bottom - top + 1, cgs.media.backTileShader);
}

/*
================
CG_FadeColor
================
*/
float *CG_FadeColor(int startMsec, int totalMsec) {
	static vec4_t color;
	int t;

	if (startMsec == 0) {
		return NULL;
	}

	t = cg.time - startMsec;

	if (t >= totalMsec) {
		return NULL;
	}

	// fade out
	if (totalMsec - t < FADE_TIME) {
		color[3] = (totalMsec - t) * 1.0 / FADE_TIME;
	} else {
		color[3] = 1.0;
	}
	color[0] = color[1] = color[2] = 1;

	return color;
}

/*
=================
CG_GetColorForHealth
=================
*/
void CG_GetColorForHealth(int health, int armor, vec4_t hcolor) {
	int count;
	int max;

	// calculate the total points of damage that can
	// be sustained at the current health / armor level
	if (health <= 0) {
		VectorClear(hcolor); // black
		hcolor[3] = 1;
		return;
	}
	count = armor;
	max = health * ARMOR_PROTECTION / (1.0 - ARMOR_PROTECTION);
	if (max < count) {
		count = max;
	}
	health += count;

	// set the color based on health
	hcolor[0] = 1.0;
	hcolor[3] = 1.0;
	if (health >= 100) {
		hcolor[2] = 1.0;
	} else if (health < 66) {
		hcolor[2] = 0;
	} else {
		hcolor[2] = (health - 66) / 33.0;
	}

	if (health > 60) {
		hcolor[1] = 1.0;
	} else if (health < 30) {
		hcolor[1] = 0;
	} else {
		hcolor[1] = (health - 30) / 30.0;
	}
}

/*
=================
CG_ColorForHealth
=================
*/
void CG_ColorForHealth(vec4_t hcolor) {

	CG_GetColorForHealth(cg.snap->ps.stats[STAT_HEALTH], cg.snap->ps.stats[STAT_ARMOR], hcolor);
}

// bk001205 - code below duplicated in q3_ui/ui-atoms.c
// bk001205 - FIXME: does this belong in ui_shared.c?
// bk001205 - FIXME: HARD_LINKED flags not visible here
// bk001205 - q_shared defines not visible here
/*
=================
UI_DrawProportionalString2
=================
*/
static const struct FontData {
	int x; // x offset in font atlas
	int y; // y offset in font atlas
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

static int UI_ProportionalStringWidth(const char *str) {
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

static void UI_DrawProportionalString2(int x, int y, const char *str, const vec4_t color, float sizeScale,
									   qhandle_t charset) {
	float ax = x * cgs.screenXScale + cgs.screenXBias;
	const float ay = y * cgs.screenYScale;
	const char *s =  str;

	// draw the colored text
	trap_R_SetColor(color);

	while (*s) {
		unsigned char ch = *s & 127;
		float aw;
		if (ch == ' ') {
			aw = (float)PROP_SPACE_WIDTH * cgs.screenXScale * sizeScale;
		} else if (propMap[ch].width != -1) {
			const float fcol = (float)propMap[ch].x / 256.0f;
			const float frow = (float)propMap[ch].y / 256.0f;
			const float fwidth = (float)propMap[ch].width / 256.0f;
			const float fheight = (float)PROP_HEIGHT / 256.0f;
			const float ah = (float)PROP_HEIGHT * cgs.screenYScale * sizeScale;
			aw = (float)propMap[ch].width * cgs.screenXScale * sizeScale;
			trap_R_DrawStretchPic(ax, ay, aw, ah, fcol, frow, fcol + fwidth, frow + fheight, charset);
		} else {
			aw = 0;
		}

		ax += (aw + (float)PROP_GAP_WIDTH * cgs.screenXScale * sizeScale);
		s++;
	}

	trap_R_SetColor(NULL);
}

/*
=================
UI_ProportionalSizeScale
=================
*/
static float UI_ProportionalSizeScale(int style) {
	if (style & UI_SMALLFONT) {
		return 0.75f;
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
		UI_DrawProportionalString2(x + 2, y + 2, str, drawcolor, sizeScale, cgs.media.charsetProp);
	}

	if (style & UI_INVERSE) {
		drawcolor[0] = color[0] * 0.8f;
		drawcolor[1] = color[1] * 0.8f;
		drawcolor[2] = color[2] * 0.8f;
		drawcolor[3] = color[3];
		UI_DrawProportionalString2(x, y, str, drawcolor, sizeScale, cgs.media.charsetProp);
		return;
	}

	if (style & UI_PULSE) {
		UI_DrawProportionalString2(x, y, str, color, sizeScale, cgs.media.charsetProp);
		drawcolor[0] = color[0];
		drawcolor[1] = color[1];
		drawcolor[2] = color[2];
		drawcolor[3] = 0.75f + 0.25f * sin(cg.time / PULSE_DIVISOR);
		UI_DrawProportionalString2(x - 2 + (int)(random() * 5.99f), y - 2 + (int)(random() * 5.99f), str, drawcolor,
								   sizeScale, cgs.media.charsetProp);
		return;
	}

	UI_DrawProportionalString2(x, y, str, color, sizeScale, cgs.media.charsetProp);
}

// thanks  tremulous!

/*
================
CG_WorldToScreen
================
*/
qboolean CG_WorldToScreen(const vec3_t point, float *x, float *y) {
	const float px = tan(cg.refdef.fov_x * M_PI / 360.0f);
	const float py = tan(cg.refdef.fov_y * M_PI / 360.0f);
	float xc = cg.refdef.width * 0.5f;
	float yc = cg.refdef.height * 0.5f;
	qboolean front = qtrue;
	vec3_t trans;
	float z;

	VectorSubtract(point, cg.refdef.vieworg, trans);

	z = DotProduct(trans, cg.refdef.viewaxis[0]);
	if (z <= 0.001f) {
		front = qfalse;
	}

	if (x) {
		*x = cg.refdef.x + xc - DotProduct(trans, cg.refdef.viewaxis[1]) * xc / (z * px);
	}

	if (y) {
		*y = cg.refdef.y + yc - DotProduct(trans, cg.refdef.viewaxis[2]) * yc / (z * py);
	}

	CG_NativeResTo640(x, y, NULL, NULL);

	return front;
}
