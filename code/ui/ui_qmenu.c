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
	UI_QMENU.C

	Quake's menu framework system.
**********************************************************************/
#include "ui_local.h"

sfxHandle_t menuMoveSound;
sfxHandle_t menuSwitchSound;
sfxHandle_t menuClickSound;
sfxHandle_t menuBuzzSound;
sfxHandle_t menuNullSound;
sfxHandle_t weaponChangeSound;

static qhandle_t sliderBar;
static qhandle_t sliderButton_0;
static qhandle_t sliderButton_1;

const vec4_t menu_text_color = {1.0f, 1.0f, 1.0f, 1.0f};
const vec4_t menu_dim_color = {0.0f, 0.0f, 0.0f, 0.75f};
const vec4_t color_black = {0.00f, 0.00f, 0.00f, 1.00f};
const vec4_t color_white = {1.00f, 1.00f, 1.00f, 1.00f};
const vec4_t color_yellow = {1.00f, 1.00f, 0.00f, 1.00f};
const vec4_t color_blue = {0.00f, 0.00f, 1.00f, 1.00f};
const vec4_t color_lightOrange = {1.00f, 0.68f, 0.00f, 1.00f};
const vec4_t color_orange = {1.00f, 0.43f, 0.00f, 1.00f};
const vec4_t color_red = {1.00f, 0.00f, 0.00f, 1.00f};
const vec4_t color_dim = {0.00f, 0.00f, 0.00f, 0.25f};

// current color scheme
static const vec4_t pulse_color = {1.00f, 1.00f, 1.00f, 1.00f};
const vec4_t text_color_disabled = {0.50f, 0.50f, 0.50f, 1.00f}; // light gray
const vec4_t text_color_normal = {1.00f, 1.00f, 0.00f, 1.00f};
const vec4_t text_color_highlight = {1.00f, 0.33f, 0.00f, 1.00f}; //{0.00f, 1.00f, 0.00f, 1.00f};
const vec4_t listbar_color = {1.00f, 1.00f, 1.00f, 0.50f};		  //{0.00f, 1.00f, 0.00f, 0.30f};

const vec4_t text_color_ignormal = {0.00f, 0.00f, 0.00f, 1.00f};
const vec4_t text_color_ighighlight = {0.40f, 0.00f, 0.40f, 1.00f};
const vec4_t listbar_igcolor = {1.00f, 0.43f, 0.00f, 0.33f};

const vec4_t text_color_bluenormal = {0.40f, 0.40f, 1.00f, 1.00f};
const vec4_t text_color_bluehighlight = {0.30f, 0.30f, 1.00f, 1.00f};
const vec4_t listbar_bluecolor = {0.00f, 0.00f, 1.00f, 0.33f};

/*
=================
Text_Init
=================
*/
static void Text_Init(menutext_s *t) {
	t->generic.flags |= QMF_INACTIVE;
}

/*
=================
Text_Draw
=================
*/
static void Text_Draw(menutext_s *t) {
	int x;
	int y;
	char buff[512];
	vec4_t color;

	x = t->generic.x;
	y = t->generic.y;

	buff[0] = '\0';

	// possible label
	if (t->generic.name)
		strcpy(buff, t->generic.name);

	// possible value
	if (t->string)
		strcat(buff, t->string);

	if (t->generic.flags & QMF_GRAYED)
		Vector4Copy(text_color_disabled, color);
	else {
		Vector4Copy(t->color, color);
		if (t->generic.flags & QMF_PULSE) {
			color[3] = 0.6 + 0.4 * sin(uis.realtime / 150.f);
		}
	}

	UI_DrawString(x, y, buff, t->style, color);
}

/*
=================
TextS_Init
=================
*/
static void TextS_Init(menutext_s *t) {
	int x;
	int y;
	int w;
	int h;

	if (t->fontHeight == 0.0f)
		t->fontHeight = 16.0f;

	x = t->generic.x;
	y = t->generic.y;
	if (t->style & UI_NS_STR_HGW) // height = width
		w = Q_PrintStrlen(t->string) * t->fontHeight;
	else
		w = Q_PrintStrlen(t->string) * (t->fontHeight * 0.5f);
	h = t->fontHeight;

	if (t->generic.flags & QMF_RIGHT_JUSTIFY) {
		x -= w;
	} else if (t->generic.flags & QMF_CENTER_JUSTIFY) {
		x -= w / 2;
	}

	t->generic.left = x;
	t->generic.right = x + w;
	t->generic.top = y;
	t->generic.bottom = y + h;
}

/*
=================
TextS_Draw
=================
*/
static void TextS_Draw(menutext_s *t) {
	int x;
	int y;
	vec4_t color;
	int style;

	x = t->generic.x;
	y = t->generic.y;

	if (t->generic.flags & QMF_GRAYED)
		Vector4Copy(text_color_disabled, color);
	else if (t->focuscolor && Menu_ItemAtCursor(t->generic.parent) == t)
		Vector4Copy(t->focuscolor, color);
	else
		Vector4Copy(t->color, color);

	style = t->style;
	if (t->generic.flags & QMF_PULSEIFFOCUS) {
		if (Menu_ItemAtCursor(t->generic.parent) == t) {
			style |= UI_PULSE;
		} else {
			style |= UI_INVERSE;
		}
	}

	UI_DrawStringNS(x, y, t->string, style, t->fontHeight, color);
}

/*
=================
Bitmap_Init
=================
*/
static void Bitmap_Init(menubitmap_s *b) {
	int x;
	int y;
	int w;
	int h;

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h = b->height;
	if (w < 0) {
		w = -w;
	}
	if (h < 0) {
		h = -h;
	}

	if (b->generic.flags & QMF_RIGHT_JUSTIFY) {
		x = x - w;
	} else if (b->generic.flags & QMF_CENTER_JUSTIFY) {
		x = x - w / 2;
	}

	b->generic.left = x;
	b->generic.right = x + w;
	b->generic.top = y;
	b->generic.bottom = y + h;

	b->shader = 0;
	b->focusshader = 0;
}

/*
=================
Bitmap_Draw
=================
*/
static void Bitmap_Draw(menubitmap_s *b) {
	float x;
	float y;
	float w;
	float h;
	vec4_t tempcolor;
	vec4_t color;

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h = b->height;

	if (b->generic.flags & QMF_RIGHT_JUSTIFY) {
		x = x - w;
	} else if (b->generic.flags & QMF_CENTER_JUSTIFY) {
		x = x - w / 2;
	}

	// used to refresh shader
	if (b->generic.name && !b->shader) {
		b->shader = trap_R_RegisterShaderNoMip(b->generic.name);
		if (!b->shader && b->errorpic)
			b->shader = trap_R_RegisterShaderNoMip(b->errorpic);
	}

	if (b->focuspic && !b->focusshader)
		b->focusshader = trap_R_RegisterShaderNoMip(b->focuspic);

	if (b->generic.flags & QMF_GRAYED) {
		if (b->shader) {
			trap_R_SetColor(colorMdGrey);
			UI_DrawHandlePic(x, y, w, h, b->shader);
			trap_R_SetColor(NULL);
		}
	} else {
		if (b->shader) {
			// coded by QuarterPounder
			if (!b->focuspicinstead ||
				(!(b->generic.flags & QMF_HIGHLIGHT) &&
				 !((b->generic.flags & QMF_HIGHLIGHT_IF_FOCUS) && (Menu_ItemAtCursor(b->generic.parent) == b))))
				UI_DrawHandlePic(x, y, w, h, b->shader);
		}

		// bk001204 - parentheses
		if (((b->generic.flags & QMF_PULSE) || (b->generic.flags & QMF_PULSEIFFOCUS)) &&
			(Menu_ItemAtCursor(b->generic.parent) == b)) {
			if (b->focuscolor) {
				tempcolor[0] = b->focuscolor[0];
				tempcolor[1] = b->focuscolor[1];
				tempcolor[2] = b->focuscolor[2];
				tempcolor[3] = 0;
				Vector4Copy(tempcolor, color);
			} else
				Vector4Copy(pulse_color, color);
			color[3] = 0.5f + 0.5f * sin(uis.realtime / PULSE_DIVISOR); // int division, is this on purpose?

			trap_R_SetColor(color);
			UI_DrawHandlePic(x, y, w, h, b->focusshader);
			trap_R_SetColor(NULL);
		} else if ((b->generic.flags & QMF_HIGHLIGHT) ||
				   ((b->generic.flags & QMF_HIGHLIGHT_IF_FOCUS) && (Menu_ItemAtCursor(b->generic.parent) == b))) {
			if (b->focuscolor) {
				trap_R_SetColor(b->focuscolor);
				UI_DrawHandlePic(x, y, w, h, b->focusshader);
				trap_R_SetColor(NULL);
			} else
				UI_DrawHandlePic(x, y, w, h, b->focusshader);
		}
	}
}

/*
=================
Bitmap1024S_Init
=================
*/
static void Bitmap1024S_Init(menubitmap1024s_s *b) {
	int x;
	int y;
	int w;
	int h;

	x = b->x;
	y = b->y;
	w = b->w;
	h = b->h;
	if (w < 0) {
		w = -w;
	}
	if (h < 0) {
		h = -h;
	}

	b->generic.left = x * 0.625f;
	b->generic.right = (x + w) * 0.625f;
	b->generic.top = y * 0.625f;
	b->generic.bottom = (y + h) * 0.625f;
}

/*
=================
Bitmap1024S_Draw
=================
*/
static void Bitmap1024S_Draw(menubitmap1024s_s *b) {
	if (b->mouseovershader) {
		if (b->sw > 0 && b->shadowshader != 0)
			UI_DrawHandlePic1024(b->sx, b->sy, b->sw, b->sh, b->shadowshader);

		if (Menu_ItemAtCursor(b->generic.parent) == b)
			UI_DrawHandlePic1024(b->x, b->y, b->w, b->h, b->mouseovershader);
		else
			UI_DrawHandlePic1024(b->x, b->y, b->w, b->h, b->shader);
	} else {
		if (!(Menu_ItemAtCursor(b->generic.parent) == b) && b->sw > 0) {
			UI_DrawHandlePic1024(b->sx, b->sy, b->sw, b->sh, b->shadowshader);
		}

		UI_DrawHandlePic1024(b->x, b->y, b->w, b->h, b->shader);
	}
}

/*
=================
Action_Init
=================
*/
static void Action_Init(menuaction_s *a) {
	int len;

	// calculate bounds
	if (a->generic.name)
		len = strlen(a->generic.name);
	else
		len = 0;

	// left justify text
	a->generic.left = a->generic.x;
	a->generic.right = a->generic.x + len * BIGCHAR_WIDTH;
	a->generic.top = a->generic.y;
	a->generic.bottom = a->generic.y + BIGCHAR_HEIGHT;
}

/*
=================
Action_Draw
=================
*/
static void Action_Draw(menuaction_s *a) {
	int x, y;
	int style;
	const float *color;

	style = 0;
	color = menu_text_color;
	if (a->generic.flags & QMF_GRAYED) {
		color = text_color_disabled;
	} else if ((a->generic.flags & QMF_PULSEIFFOCUS) && (a->generic.parent->cursor == a->generic.menuPosition)) {
		if (a->generic.flags & QMF_BLUESTYLE)
			color = text_color_bluehighlight;
		else
			color = (a->generic.flags & QMF_INGAMESTYLE) ? text_color_ighighlight : text_color_highlight;
		/*style = UI_PULSE;*/
	} else if ((a->generic.flags & QMF_HIGHLIGHT_IF_FOCUS) && (a->generic.parent->cursor == a->generic.menuPosition)) {
		if (a->generic.flags & QMF_BLUESTYLE)
			color = text_color_bluehighlight;
		else
			color = (a->generic.flags & QMF_INGAMESTYLE) ? text_color_ighighlight : text_color_highlight;
	} else if (a->generic.flags & QMF_BLINK) {
		style = UI_BLINK;
		if (a->generic.flags & QMF_BLUESTYLE)
			color = text_color_bluehighlight;
		else
			color = (a->generic.flags & QMF_INGAMESTYLE) ? text_color_ighighlight : text_color_highlight;
	}

	x = a->generic.x;
	y = a->generic.y;

	UI_DrawString(x, y, a->generic.name, UI_LEFT | style, color);

	if (a->generic.parent->cursor == a->generic.menuPosition) {
		// draw cursor
		UI_DrawChar(x - BIGCHAR_WIDTH, y, FONT_ASCII_TRIANGLE, UI_LEFT | UI_BLINK, color);
	}
}

/*
=================
RadioButton_Init
=================
*/
static void RadioButton_Init(menuradiobutton_s *rb) {
	int len;

	// calculate bounds
	if (rb->generic.name)
		len = strlen(rb->generic.name);
	else
		len = 0;

	rb->generic.left = rb->generic.x - (len + 1) * SMALLCHAR_WIDTH;
	rb->generic.right = rb->generic.x + 6 * SMALLCHAR_WIDTH;
	rb->generic.top = rb->generic.y;
	rb->generic.bottom = rb->generic.y + SMALLCHAR_HEIGHT;
}

/*
=================
RadioButton_Key
=================
*/
static sfxHandle_t RadioButton_Key(menuradiobutton_s *rb, int key) {
	switch (key) {
	case K_MOUSE1:
		if (!(rb->generic.flags & QMF_HASMOUSEFOCUS))
			break;
		// fall through
	case K_JOY1:
	case K_JOY2:
	case K_JOY3:
	case K_JOY4:
	case K_ENTER:
	case K_KP_ENTER:
	case K_KP_LEFTARROW:
	case K_LEFTARROW:
	case K_KP_RIGHTARROW:
	case K_RIGHTARROW:
		rb->curvalue = !rb->curvalue;
		if (rb->generic.callback)
			rb->generic.callback(rb, QM_ACTIVATED);

		return (menuMoveSound);
	}

	// key not handled
	return 0;
}

/*
=================
RadioButton_Draw
=================
*/
static void RadioButton_Draw(menuradiobutton_s *rb) {
	int x;
	int y;
	const float *color;
	int style;
	qboolean focus;

	x = rb->generic.x;
	y = rb->generic.y;

	focus = (rb->generic.parent->cursor == rb->generic.menuPosition);

	if (rb->generic.flags & QMF_GRAYED) {
		color = text_color_disabled;
		style = UI_LEFT | UI_SMALLFONT;
	} else if (focus) {
		if (rb->generic.flags & QMF_BLUESTYLE)
			color = text_color_bluehighlight;
		else
			color = (rb->generic.flags & QMF_INGAMESTYLE) ? text_color_ighighlight : text_color_highlight;
		style = UI_LEFT | UI_PULSE | UI_SMALLFONT;
	} else {
		if (rb->generic.flags & QMF_BLUESTYLE)
			color = text_color_bluenormal;
		else
			color = (rb->generic.flags & QMF_INGAMESTYLE) ? text_color_ignormal : text_color_normal;
		style = UI_LEFT | UI_SMALLFONT;
	}

	if (focus) {
		// draw cursor
		if (rb->generic.flags & QMF_BLUESTYLE)
			UI_FillRect(rb->generic.left, rb->generic.top, rb->generic.right - rb->generic.left + 1,
						rb->generic.bottom - rb->generic.top + 1, listbar_bluecolor);
		else if (rb->generic.flags & QMF_INGAMESTYLE)
			UI_FillRect(rb->generic.left, rb->generic.top, rb->generic.right - rb->generic.left + 1,
						rb->generic.bottom - rb->generic.top + 1, listbar_igcolor);
		else
			UI_FillRect(rb->generic.left, rb->generic.top, rb->generic.right - rb->generic.left + 1,
						rb->generic.bottom - rb->generic.top + 1, listbar_color);
		UI_DrawChar(x, y, FONT_ASCII_TRIANGLE, UI_CENTER | UI_BLINK | UI_SMALLFONT, color);
	}

	if (rb->generic.name)
		UI_DrawString(x - SMALLCHAR_WIDTH, y, rb->generic.name, UI_RIGHT | UI_SMALLFONT, color);

	if (!rb->curvalue) {
		UI_DrawHandlePic(x + SMALLCHAR_WIDTH, y + 2, 16, 16, uis.rb_off);
		UI_DrawString(x + SMALLCHAR_WIDTH + 16, y, "off", style, color);
	} else {
		UI_DrawHandlePic(x + SMALLCHAR_WIDTH, y + 2, 16, 16, uis.rb_on);
		UI_DrawString(x + SMALLCHAR_WIDTH + 16, y, "on", style, color);
	}
}

/*
=================
Slider_Init
=================
*/
static void Slider_Init(menuslider_s *s) {
	int len;

	// calculate bounds
	if (s->generic.name)
		len = strlen(s->generic.name);
	else
		len = 0;

	s->generic.left = s->generic.x - (len + 1) * SMALLCHAR_WIDTH;
	s->generic.right = s->generic.x + (SLIDER_RANGE + 2 + 1) * SMALLCHAR_WIDTH;
	s->generic.top = s->generic.y;
	s->generic.bottom = s->generic.y + SMALLCHAR_HEIGHT;
}

/*
=================
Slider_Key
=================
*/
static sfxHandle_t Slider_Key(menuslider_s *s, int key) {
	sfxHandle_t sound;

	switch (key) {
	case K_MOUSE1: { // FIXME? atm this code is duplicated in ui_atom.c (UI_MouseEvent) to have "dragable" sliders ...
					 // so extracting a method would be good
		const int x = uis.cursorx - s->generic.x - 2 * SMALLCHAR_WIDTH;
		const float oldvalue = s->curvalue;
		s->curvalue = (x / (float)(SLIDER_RANGE * SMALLCHAR_WIDTH)) * (s->maxvalue - s->minvalue) + s->minvalue;

		if (s->curvalue < s->minvalue)
			s->curvalue = s->minvalue;
		else if (s->curvalue > s->maxvalue)
			s->curvalue = s->maxvalue;
		if (s->curvalue != oldvalue)
			sound = menuMoveSound;
		else
			sound = 0;
	} break;

	case K_KP_LEFTARROW:
	case K_LEFTARROW:
		if (s->curvalue > s->minvalue) {
			s->curvalue--;
			sound = menuMoveSound;
		} else
			sound = menuBuzzSound;
		break;

	case K_KP_RIGHTARROW:
	case K_RIGHTARROW:
		if (s->curvalue < s->maxvalue) {
			s->curvalue++;
			sound = menuMoveSound;
		} else
			sound = menuBuzzSound;
		break;

	default:
		// key not handled
		sound = 0;
		break;
	}

	if (sound && s->generic.callback)
		s->generic.callback(s, QM_ACTIVATED);

	return (sound);
}

/*
=================
Slider_Draw
=================
*/
static void Slider_Draw(menuslider_s *s) {
	int x;
	int y;
	int style;
	const float *color;
	qboolean focus;
	int button;

	x = s->generic.x;
	y = s->generic.y;
	focus = (s->generic.parent->cursor == s->generic.menuPosition);

	if (s->generic.flags & QMF_GRAYED) {
		color = text_color_disabled;
		style = UI_SMALLFONT;
	} else if (focus) {
		if (s->generic.flags & QMF_BLUESTYLE)
			color = text_color_bluehighlight;
		else
			color = (s->generic.flags & QMF_INGAMESTYLE) ? text_color_ighighlight : text_color_highlight;
		style = UI_SMALLFONT | UI_PULSE;
	} else {
		if (s->generic.flags & QMF_BLUESTYLE)
			color = text_color_bluenormal;
		else
			color = (s->generic.flags & QMF_INGAMESTYLE) ? text_color_ignormal : text_color_normal;
		style = UI_SMALLFONT;
	}

	// draw label
	UI_DrawString(x - SMALLCHAR_WIDTH, y, s->generic.name, UI_RIGHT | (style & ~UI_PULSE), color);

	// draw slider
	UI_SetColor(color);
	UI_DrawHandlePic(x + SMALLCHAR_WIDTH, y + 4, 96, 8, sliderBar);
	UI_DrawStringNS(x + SMALLCHAR_WIDTH + 100, y + 2, va("%.1f", s->curvalue), UI_LEFT, 12, menu_text_color);
	UI_SetColor(NULL);

	// clamp thumb
	if (s->maxvalue > s->minvalue) {
		s->range = (s->curvalue - s->minvalue) / (float)(s->maxvalue - s->minvalue);
		if (s->range < 0) {
			s->range = 0;
		} else if (s->range > 1) {
			s->range = 1;
		}
	} else {
		s->range = 0;
	}

	// draw thumb
	if (style & UI_PULSE) {
		button = sliderButton_1;
	} else {
		button = sliderButton_0;
	}

	UI_DrawHandlePic((int)(x + 2 * SMALLCHAR_WIDTH + (SLIDER_RANGE - 1) * SMALLCHAR_WIDTH * s->range) - 2, y, 12, 16,
					 button);
}

/*
=================
SpinControl_Init
=================
*/
static void SpinControl_Init(menulist_s *s) {
	int len;
	int l;
	const char *str;

	if (s->generic.name)
		len = strlen(s->generic.name) * SMALLCHAR_WIDTH;
	else
		len = 0;

	s->generic.left = s->generic.x - SMALLCHAR_WIDTH - len;

	len = s->numitems = 0;
	while ((str = s->itemnames[s->numitems]) != 0) {
		l = strlen(str);
		if (l > len)
			len = l;

		s->numitems++;
	}

	s->generic.top = s->generic.y;
	s->generic.right = s->generic.x + (len + 1) * SMALLCHAR_WIDTH;
	s->generic.bottom = s->generic.y + SMALLCHAR_HEIGHT;

	s->dropdown_len = len * SMALLCHAR_WIDTH;

	if (s->generic.flags & QMF_BLUESTYLE) {
		if (!s->dropdown_bg)
			s->dropdown_bg = colorBlack;
		if (!s->dropdown_border)
			s->dropdown_border = colorBlue;
		if (!s->dropdown_mark)
			s->dropdown_mark = colorBlue;
		if (!s->dropdown_text)
			s->dropdown_text = colorWhite;
	} else if (s->generic.flags & QMF_INGAMESTYLE) {
		if (!s->dropdown_bg)
			s->dropdown_bg = colorWhite;
		if (!s->dropdown_border)
			s->dropdown_border = colorBlack;
		if (!s->dropdown_mark)
			s->dropdown_mark = color_orange;
		if (!s->dropdown_text)
			s->dropdown_text = colorBlack;
	} else {
		if (!s->dropdown_bg)
			s->dropdown_bg = color_orange;
		if (!s->dropdown_border)
			s->dropdown_border = colorYellow;
		if (!s->dropdown_mark)
			s->dropdown_mark = colorRed;
		if (!s->dropdown_text)
			s->dropdown_text = colorYellow;
	}
}

/*
=================
SpinControl_Key
=================
*/
static sfxHandle_t SpinControl_Key(menulist_s *s, int key) {
	sfxHandle_t sound;

	sound = 0;
	switch (key) {
	case K_KP_RIGHTARROW:
	case K_RIGHTARROW:
	case K_MOUSE1:
		if ((/*s->numitems<3 ||*/ s->generic.ownerdraw) && !(s->generic.flags & QMF_FORCEDROPDOWN)) {
			s->curvalue++;
			if (s->curvalue >= s->numitems)
				s->curvalue = 0;
			sound = menuClickSound;
		} else {
			uis.dropdownlist = s;
			if (s->generic.right > 640)
				uis.dropdownxywh[0] = 640 - s->dropdown_len;
			else
				uis.dropdownxywh[0] = s->generic.right - s->dropdown_len;
			uis.dropdownxywh[3] = s->numitems * SMALLCHAR_HEIGHT;
			uis.dropdownxywh[1] = s->generic.y + SMALLCHAR_HEIGHT * 0.5f - uis.dropdownxywh[3] * 0.5f;

			if (uis.dropdownxywh[1] + uis.dropdownxywh[3] > 480.0f)
				uis.dropdownxywh[1] = 480.0f - uis.dropdownxywh[3];
			else if (uis.dropdownxywh[1] < 0.0f)
				uis.dropdownxywh[1] = 0.0f;

			uis.dropdownxywh[2] = s->dropdown_len; // s->generic.right-s->generic.x-SMALLCHAR_WIDTH;

			return menuClickSound; // return damit kein callback aufgerufen wird
		}
		break;

	case K_KP_LEFTARROW:
	case K_LEFTARROW:
		s->curvalue--;
		if (s->curvalue < 0)
			s->curvalue = s->numitems - 1;
		sound = menuMoveSound;
		break;
	}

	if (sound && s->generic.callback)
		s->generic.callback(s, QM_ACTIVATED);

	return (sound);
}

/*
=================
SpinControl_Draw
=================
*/
static void SpinControl_Draw(const menulist_s *s) {
	const float *color;
	int x, y;
	int style;
	qboolean focus;

	x = s->generic.x;
	y = s->generic.y;

	style = UI_SMALLFONT;
	focus = (s->generic.parent->cursor == s->generic.menuPosition);

	if (s->generic.flags & QMF_GRAYED)
		color = text_color_disabled;
	else if (focus) {
		if (s->generic.flags & QMF_BLUESTYLE)
			color = text_color_bluehighlight;
		else
			color = (s->generic.flags & QMF_INGAMESTYLE) ? text_color_ighighlight : text_color_highlight;
		style |= UI_PULSE;
	} else if (s->generic.flags & QMF_BLINK) {
		if (s->generic.flags & QMF_BLUESTYLE)
			color = text_color_bluehighlight;
		else
			color = (s->generic.flags & QMF_INGAMESTYLE) ? text_color_ighighlight : text_color_highlight;
		style |= UI_BLINK;
	} else {
		if (s->generic.flags & QMF_BLUESTYLE)
			color = text_color_bluenormal;
		else
			color = (s->generic.flags & QMF_INGAMESTYLE) ? text_color_ignormal : text_color_normal;
	}

	if (focus) {
		// draw cursor
		if (s->generic.flags & QMF_BLUESTYLE)
			UI_FillRect(s->generic.left, s->generic.top, s->generic.right - s->generic.left + 1,
						s->generic.bottom - s->generic.top + 1, listbar_bluecolor);
		else if (s->generic.flags & QMF_INGAMESTYLE)
			UI_FillRect(s->generic.left, s->generic.top, s->generic.right - s->generic.left + 1,
						s->generic.bottom - s->generic.top + 1, listbar_igcolor);
		else
			UI_FillRect(s->generic.left, s->generic.top, s->generic.right - s->generic.left + 1,
						s->generic.bottom - s->generic.top + 1, listbar_color);
		UI_DrawChar(x, y, FONT_ASCII_TRIANGLE, UI_CENTER | UI_BLINK | UI_SMALLFONT, color);
	}

	UI_DrawString(x - SMALLCHAR_WIDTH, y, s->generic.name, UI_RIGHT | (style & ~UI_PULSE), color);
	UI_DrawString(x + SMALLCHAR_WIDTH, y, s->itemnames[s->curvalue], style | UI_LEFT, color);
}

/*
=================
ScrollList_Init
=================
*/
static void ScrollList_Init(menulist_s *l) {
	int w;

	l->oldvalue = 0;
	l->curvalue = 0;
	l->top = 0;

	if (!l->columns) {
		l->columns = 1;
		l->seperation = 0;
	} else if (!l->seperation) {
		l->seperation = 3;
	}

	w = ((l->width + l->seperation) * l->columns - l->seperation) * SMALLCHAR_WIDTH;

	l->generic.left = l->generic.x;
	l->generic.top = l->generic.y;
	l->generic.right = l->generic.x + w;
	l->generic.bottom = l->generic.y + l->height * SMALLCHAR_HEIGHT;

	if (l->generic.flags & QMF_CENTER_JUSTIFY) {
		l->generic.left -= w / 2;
		l->generic.right -= w / 2;
	}
}

void ParseMenuListItems(char *input, menulist_s *output, int maxItems, char separator, const char *selection, const char *empty) {
	size_t len = strlen(input);
	size_t i;
	const char *name = input;

	output->numitems = 0;
	for (i = 0; i < len; ++i) {
		if (input[i] == separator) {
			input[i] = '\0';
			if (selection && !strcmp(selection, name)) {
				output->curvalue = output->numitems;
			}
			output->itemnames[output->numitems++] = name;
			if (output->numitems >= maxItems) {
				break;
			}
			name = &input[i + 1];
		}
	}
	if (output->numitems == 0) {
		output->itemnames[0] = empty;
		output->numitems = 1;
	}
	Menu_InitItem((menucommon_s *)output);
}

/*
=================
ScrollList_Key
=================
*/
sfxHandle_t ScrollList_Key(menulist_s *l, int key) {
	int x;
	int y;
	int w;
	int i;
	int j;
	int c;
	int cursorx;
	int cursory;
	int column;
	int index;

	switch (key) {
	case K_MOUSE1:
		if (l->generic.flags & QMF_HASMOUSEFOCUS) {
			// check scroll region
			x = l->generic.x;
			y = l->generic.y;
			w = ((l->width + l->seperation) * l->columns - l->seperation) * SMALLCHAR_WIDTH;
			if (l->generic.flags & QMF_CENTER_JUSTIFY) {
				x -= w / 2;
			}
			if (UI_CursorInRect(x, y, w, l->height * SMALLCHAR_HEIGHT)) {
				cursorx = (uis.cursorx - x) / SMALLCHAR_WIDTH;
				column = cursorx / (l->width + l->seperation);
				cursory = (uis.cursory - y) / SMALLCHAR_HEIGHT;
				index = column * l->height + cursory;
				if (l->top + index < l->numitems) {
					l->oldvalue = l->curvalue;
					l->curvalue = l->top + index;

					if (l->oldvalue != l->curvalue && l->generic.callback) {
						l->generic.callback(l, QM_GOTFOCUS);
						return (menuMoveSound);
					}
				}
			}

			// absorbed, silent sound effect
			return (menuNullSound);
		}
		break;

	case K_KP_HOME:
	case K_HOME:
		l->oldvalue = l->curvalue;
		l->curvalue = 0;
		l->top = 0;

		if (l->oldvalue != l->curvalue && l->generic.callback) {
			l->generic.callback(l, QM_GOTFOCUS);
			return (menuMoveSound);
		}
		return (menuBuzzSound);

	case K_KP_END:
	case K_END:
		l->oldvalue = l->curvalue;
		l->curvalue = l->numitems - 1;
		if (l->columns > 1) {
			c = (l->curvalue / l->height + 1) * l->height;
			l->top = c - (l->columns * l->height);
		} else {
			l->top = l->curvalue - (l->height - 1);
		}
		if (l->top < 0)
			l->top = 0;

		if (l->oldvalue != l->curvalue && l->generic.callback) {
			l->generic.callback(l, QM_GOTFOCUS);
			return (menuMoveSound);
		}
		return (menuBuzzSound);

	case K_PGUP:
	case K_KP_PGUP:
		if (l->columns > 1) {
			return menuNullSound;
		}

		if (l->curvalue > 0) {
			l->oldvalue = l->curvalue;
			l->curvalue -= l->height - 1;
			if (l->curvalue < 0)
				l->curvalue = 0;
			l->top = l->curvalue;
			if (l->top < 0)
				l->top = 0;

			if (l->generic.callback)
				l->generic.callback(l, QM_GOTFOCUS);

			return (menuMoveSound);
		}
		return (menuBuzzSound);

	case K_PGDN:
	case K_KP_PGDN:
		if (l->columns > 1) {
			return menuNullSound;
		}

		if (l->curvalue < l->numitems - 1) {
			l->oldvalue = l->curvalue;
			l->curvalue += l->height - 1;
			if (l->curvalue > l->numitems - 1)
				l->curvalue = l->numitems - 1;
			l->top = l->curvalue - (l->height - 1);
			if (l->top < 0)
				l->top = 0;

			if (l->generic.callback)
				l->generic.callback(l, QM_GOTFOCUS);

			return (menuMoveSound);
		}
		return (menuBuzzSound);

	case K_MWHEELUP:
		if (l->columns > 1) {
			return menuNullSound;
		}

		if (l->top > 0) {
			// if scrolling 3 lines would replace over half of the
			// displayed items, only scroll 1 item at a time.
			int scroll = l->height < 6 ? 1 : 3;
			l->top -= scroll;
			if (l->top < 0)
				l->top = 0;

			if (l->generic.callback)
				l->generic.callback(l, QM_GOTFOCUS);

			// make scrolling silent
			return (menuNullSound);
		}
		return (menuBuzzSound);

	case K_MWHEELDOWN:
		if (l->columns > 1) {
			return menuNullSound;
		}

		if (l->top < l->numitems - l->height) {
			// if scrolling 3 items would replace over half of the
			// displayed items, only scroll 1 item at a time.
			int scroll = l->height < 6 ? 1 : 3;
			l->top += scroll;
			if (l->top > l->numitems - l->height)
				l->top = l->numitems - l->height;

			if (l->generic.callback)
				l->generic.callback(l, QM_GOTFOCUS);

			// make scrolling silent
			return (menuNullSound);
		}
		return (menuBuzzSound);

	case K_KP_UPARROW:
	case K_UPARROW:
		if (l->curvalue == 0) {
			return menuBuzzSound;
		}

		l->oldvalue = l->curvalue;
		l->curvalue--;

		if (l->curvalue < l->top) {
			if (l->columns == 1) {
				l->top--;
			} else {
				l->top -= l->height;
			}
		}

		if (l->curvalue < l->top || l->curvalue > l->top + l->height - 1) {
			l->top = l->curvalue - (l->height - 1);
			if (l->top < 0)
				l->top = 0;
		}

		if (l->generic.callback) {
			l->generic.callback(l, QM_GOTFOCUS);
		}

		return (menuMoveSound);

	case K_KP_DOWNARROW:
	case K_DOWNARROW:
		if (l->curvalue == l->numitems - 1) {
			return menuBuzzSound;
		}

		l->oldvalue = l->curvalue;
		l->curvalue++;

		if (l->curvalue >= l->top + l->columns * l->height) {
			if (l->columns == 1) {
				l->top++;
			} else {
				l->top += l->height;
			}
		}

		if (l->curvalue < l->top || l->curvalue > l->top + l->height - 1)
			l->top = l->curvalue;

		if (l->generic.callback) {
			l->generic.callback(l, QM_GOTFOCUS);
		}

		return menuMoveSound;

	case K_KP_LEFTARROW:
	case K_LEFTARROW:
		if (l->columns == 1) {
			return menuNullSound;
		}

		if (l->curvalue < l->height) {
			return menuBuzzSound;
		}

		l->oldvalue = l->curvalue;
		l->curvalue -= l->height;

		if (l->curvalue < l->top) {
			l->top -= l->height;
		}

		if (l->generic.callback) {
			l->generic.callback(l, QM_GOTFOCUS);
		}

		return menuMoveSound;

	case K_KP_RIGHTARROW:
	case K_RIGHTARROW:
		if (l->columns == 1) {
			return menuNullSound;
		}

		c = l->curvalue + l->height;

		if (c >= l->numitems) {
			return menuBuzzSound;
		}

		l->oldvalue = l->curvalue;
		l->curvalue = c;

		if (l->curvalue > l->top + l->columns * l->height - 1) {
			l->top += l->height;
		}

		if (l->generic.callback) {
			l->generic.callback(l, QM_GOTFOCUS);
		}

		return menuMoveSound;
	}

	// cycle look for ascii key inside list items
	if (!Q_isprint(key))
		return (0);

	// force to lower for case insensitive compare
	if (Q_isupper(key)) {
		key -= 'A' - 'a';
	}

	// iterate list items
	for (i = 1; i <= l->numitems; i++) {
		j = (l->curvalue + i) % l->numitems;
		c = l->itemnames[j][0];
		if (Q_isupper(c)) {
			c -= 'A' - 'a';
		}

		if (c == key) {
			// set current item, mimic windows listbox scroll behavior
			if (j < l->top) {
				// behind top most item, set this as new top
				l->top = j;
			} else if (j > l->top + l->height - 1) {
				// past end of list box, do page down
				l->top = (j + 1) - l->height;
			}

			if (l->curvalue != j) {
				l->oldvalue = l->curvalue;
				l->curvalue = j;
				if (l->generic.callback)
					l->generic.callback(l, QM_GOTFOCUS);
				return (menuMoveSound);
			}

			return (menuBuzzSound);
		}
	}

	return (menuBuzzSound);
}

/*
=================
ScrollList_Draw
=================
*/
static void ScrollList_Draw(const menulist_s *l) {
	int x;
	int u;
	int y;
	int i;
	int base;
	int column;
	const float *color;
	qboolean hasfocus;
	int style;
	int sLen;

	hasfocus = (l->generic.parent->cursor == l->generic.menuPosition);

	x = l->generic.x;
	for (column = 0; column < l->columns; column++) {
		y = l->generic.y;
		base = l->top + column * l->height;
		for (i = base; i < base + l->height; i++) {
			if (i >= l->numitems)
				break;

			if (i == l->curvalue) {
				u = x - 2;
				if (l->generic.flags & QMF_CENTER_JUSTIFY) {
					u -= (l->width * SMALLCHAR_WIDTH) / 2 + 1;
				}

				if (l->generic.flags & QMF_BLUESTYLE)
					UI_FillRect(u, y, l->width * SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT + 2, listbar_bluecolor);
				else if (l->generic.flags & QMF_INGAMESTYLE)
					UI_FillRect(u, y, l->width * SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT + 2, listbar_igcolor);
				else
					UI_FillRect(u, y, l->width * SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT + 2, listbar_color);

				if (l->generic.flags & QMF_BLUESTYLE)
					color = text_color_bluehighlight;
				else
					color = (l->generic.flags & QMF_INGAMESTYLE) ? text_color_ighighlight : text_color_highlight;

				if (hasfocus)
					style = UI_PULSE | UI_LEFT | UI_SMALLFONT;
				else
					style = UI_LEFT | UI_SMALLFONT;
			} else {
				if (l->generic.flags & QMF_BLUESTYLE)
					color = text_color_bluenormal;
				else
					color = (l->generic.flags & QMF_INGAMESTYLE) ? text_color_ignormal : text_color_normal;
				style = UI_LEFT | UI_SMALLFONT;
			}
			if (l->generic.flags & QMF_CENTER_JUSTIFY) {
				style |= UI_CENTER;
			}

			sLen = Q_PrintStrlen(l->itemnames[i]);
			if (sLen > l->width) {
				int j = l->width;
				char buff[128]; // max ~ 640/8 = 80

				if (j > 126)
					j = 126;

				if (i == l->curvalue) {
					Q_strncpyz(buff,
							   l->itemnames[i] +
								   (int)((sLen - j + 1) * (0.5f * sin(uis.realtime * 0.02f / (sLen - j)) + 0.5f)),
							   j + 1);
					UI_DrawString(x, y, buff, style, color);
				} else {
					// TODO: Beware of cutting a color code like ^2 in between or
					//       copying the color before inserted "..."
					Q_strncpyz(buff, l->itemnames[i], j);
					strcpy(buff + j - 4, "...");
					UI_DrawString(x, y, buff, style, color);
				}
			} else
				UI_DrawString(x, y, l->itemnames[i], style, color);

			y += SMALLCHAR_HEIGHT;
		}
		x += (l->width + l->seperation) * SMALLCHAR_WIDTH;
	}
}

void Menu_InitItem(menucommon_s *item) {
	switch (item->type) {
	case MTYPE_ACTION:
		Action_Init((menuaction_s *)item);
		break;

	case MTYPE_FIELD:
		MenuField_Init((menufield_s *)item);
		break;

	case MTYPE_SPINCONTROL:
		SpinControl_Init((menulist_s *)item);
		break;

	case MTYPE_RADIOBUTTON:
		RadioButton_Init((menuradiobutton_s *)item);
		break;

	case MTYPE_SLIDER:
		Slider_Init((menuslider_s *)item);
		break;

	case MTYPE_BITMAP:
		Bitmap_Init((menubitmap_s *)item);
		break;

	case MTYPE_TEXT:
		Text_Init((menutext_s *)item);
		break;

	case MTYPE_SCROLLLIST:
		ScrollList_Init((menulist_s *)item);
		break;

	case MTYPE_BITMAP1024S:
		Bitmap1024S_Init((menubitmap1024s_s *)item);
		break;
	case MTYPE_TEXTS:
		TextS_Init((menutext_s *)item);
		break;

	default:
		trap_Error(va("Menu_AddItem: unknown type %d", item->type));
	}
}

/*
=================
Menu_AddItem
=================
*/
void Menu_AddItem(menuframework_s *menu, void *item) {
	menucommon_s *itemptr;

	if (menu->nitems >= MAX_MENUITEMS)
		trap_Error("Menu_AddItem: excessive items");

	menu->items[menu->nitems] = item;
	((menucommon_s *)menu->items[menu->nitems])->parent = menu;
	((menucommon_s *)menu->items[menu->nitems])->menuPosition = menu->nitems;
	((menucommon_s *)menu->items[menu->nitems])->flags &= ~QMF_HASMOUSEFOCUS;

	// perform any item specific initializations
	itemptr = (menucommon_s *)item;
	if (!(itemptr->flags & QMF_NODEFAULTINIT)) {
		Menu_InitItem(itemptr);
	}

	menu->nitems++;
}

/*
=================
Menu_CursorMoved
=================
*/
static void Menu_CursorMoved(menuframework_s *m) {
	void (*callback)(void *self, int notification);

	if (m->cursor_prev == m->cursor)
		return;

	if (m->cursor_prev >= 0 && m->cursor_prev < m->nitems) {
		callback = ((menucommon_s *)(m->items[m->cursor_prev]))->callback;
		if (callback)
			callback(m->items[m->cursor_prev], QM_LOSTFOCUS);
	}

	if (m->cursor >= 0 && m->cursor < m->nitems) {
		callback = ((menucommon_s *)(m->items[m->cursor]))->callback;
		if (callback)
			callback(m->items[m->cursor], QM_GOTFOCUS);
	}
}

/*
=================
Menu_SetCursor
=================
*/
void Menu_SetCursor(menuframework_s *m, int cursor) {
	if (((menucommon_s *)(m->items[cursor]))->flags & (QMF_GRAYED | QMF_INACTIVE)) {
		// cursor can't go there
		return;
	}

	if (uis.dropdownlist)
		return;

	m->cursor_prev = m->cursor;
	m->cursor = cursor;

	Menu_CursorMoved(m);
}

/*
=================
Menu_SetCursorToItem
=================
*/
void Menu_SetCursorToItem(menuframework_s *m, void *ptr) {
	int i;

	for (i = 0; i < m->nitems; i++) {
		if (m->items[i] == ptr) {
			Menu_SetCursor(m, i);
			return;
		}
	}
}

/*
** Menu_AdjustCursor
**
** This function takes the given menu, the direction, and attempts
** to adjust the menu's cursor so that it's at the next available
** slot.
*/
static void Menu_AdjustCursor(menuframework_s *m, int dir) {
	menucommon_s *item = NULL;
	qboolean wrapped = qfalse;

wrap:
	while (m->cursor >= 0 && m->cursor < m->nitems) {
		item = (menucommon_s *)m->items[m->cursor];
		if ((item->flags & (QMF_GRAYED | QMF_MOUSEONLY | QMF_INACTIVE))) {
			m->cursor += dir;
		} else {
			break;
		}
	}

	if (dir == 1) {
		if (m->cursor >= m->nitems) {
			if (m->wrapAround) {
				if (wrapped) {
					m->cursor = m->cursor_prev;
					return;
				}
				m->cursor = 0;
				wrapped = qtrue;
				goto wrap;
			}
			m->cursor = m->cursor_prev;
		}
	} else {
		if (m->cursor < 0) {
			if (m->wrapAround) {
				if (wrapped) {
					m->cursor = m->cursor_prev;
					return;
				}
				m->cursor = m->nitems - 1;
				wrapped = qtrue;
				goto wrap;
			}
			m->cursor = m->cursor_prev;
		}
	}
}

static void UI_DrawToolTip(const menucommon_s *focusItem) {
	static const int textStyleFlags = UI_SMALLFONT;
	static const int maxTextWidth = 250;
	static const int boxMargin = 5;
	static const int lineHeight = 20;
	static const int outerMargin = 10;
	int numLines;
	int boxWidth, boxHeight;
	int totalTextWidth;
	int boxX, boxY, textX, textY;

	if (!focusItem)
		return;

	if (!focusItem->toolTip)
		return;

	// how many lines do we need?
	totalTextWidth = UI_ProportionalStringWidth(focusItem->toolTip) * UI_ProportionalSizeScale(textStyleFlags);
	if (totalTextWidth > maxTextWidth) {
		numLines = UI_AutoWrappedString_LineCount(maxTextWidth, focusItem->toolTip, textStyleFlags, qfalse);
		boxWidth = maxTextWidth + 2 * boxMargin;
	} else {
		numLines = 1;
		boxWidth = totalTextWidth + 2 * boxMargin;
	}

	boxHeight = numLines * lineHeight + 2 * boxMargin;

	boxX = focusItem->x;
	if (boxX > 640 - boxWidth) {
		boxX -= boxWidth;
		if (boxX < 0)
			boxX = 0;
	}

	boxY = focusItem->y - boxHeight - outerMargin;
	if (boxY < 0 && focusItem->y < 240) // box doesnt fit and item is in the top half
	{
		boxY = focusItem->y + 15 + outerMargin;
	}

	textX = boxX + boxMargin;
	textY = boxY + boxMargin;

	UI_FillRect(boxX, boxY, boxWidth, boxHeight, color_orange);
	UI_DrawString_AutoWrapped(textX, textY, maxTextWidth, 20, focusItem->toolTip, textStyleFlags, menu_text_color,
							  qfalse);
	UI_DrawRect(boxX, boxY, boxWidth, boxHeight, colorYellow, 2);

	// draw frame and tip
	// UI_DrawString(500, SCREEN_HEIGHT * 0.85, focusItem->toolTip, UI_SMALLFONT|UI_CENTER, colorWhite );
}

void Menu_DrawField(menufield_s *f) {
	qboolean focus;
	int style;
	char *txt;
	char cursorChar;
	const float *color;
	int n;
	int basex, x, y;
	const float *highlight_color = text_color_highlight;
	const vec4_t bg_color = {1.00f, 1.00f, 1.00f, 0.50f};

	y = f->generic.y;
	basex = f->generic.x + (f->field.widthInChars - Q_PrintStrlen(f->field.buffer)) / 2 * SMALLCHAR_WIDTH;
	focus = (f->generic.parent->cursor == f->generic.menuPosition);

	style = UI_LEFT | UI_SMALLFONT;

	txt = f->field.buffer;
	if (focus)
		color = highlight_color;
	else
		color = text_color_normal;

	// draw cursor if we have focus
	if (focus) {
		if (trap_Key_GetOverstrikeMode()) {
			cursorChar = FONT_ASCII_FULLBLOCK;
		} else {
			cursorChar = FONT_ASCII_UNDERLINE;
		}

		UI_FillRect(f->generic.x, f->generic.y, f->field.widthInChars * SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, bg_color);

		style |= UI_BLINK;
		UI_DrawChar(basex + f->field.cursor * SMALLCHAR_WIDTH, y, cursorChar, style, highlight_color);
		style &= ~UI_BLINK;
	}

	if (focus) {
		style |= UI_PULSE;
		color = highlight_color;
	}

	x = basex;
	while ((cursorChar = *txt) != 0) {
		if (!focus && Q_IsColorString(txt)) {
			n = ColorIndex(*(txt + 1));
			if (n == 0) {
				n = 7;
			}
			color = g_color_table[n];
			txt += 2;
			continue;
		}
		UI_DrawChar(x, y, cursorChar, style, color);
		txt++;
		x += SMALLCHAR_WIDTH;
	}
}

/*
=================
Menu_Draw
=================
*/
void Menu_Draw(menuframework_s *menu) {
	int i;
	menucommon_s *itemptr;

	// draw menu
	for (i = 0; i < menu->nitems; i++) {
		itemptr = (menucommon_s *)menu->items[i];

		if (itemptr->flags & QMF_HIDDEN)
			continue;

		if (itemptr->ownerdraw) {
			// total subclassing, owner draws everything
			itemptr->ownerdraw(itemptr);
		} else {
			switch (itemptr->type) {
			case MTYPE_RADIOBUTTON:
				RadioButton_Draw((menuradiobutton_s *)itemptr);
				break;

			case MTYPE_FIELD:
				MenuField_Draw((menufield_s *)itemptr);
				break;

			case MTYPE_SLIDER:
				Slider_Draw((menuslider_s *)itemptr);
				break;

			case MTYPE_SPINCONTROL:
				SpinControl_Draw((menulist_s *)itemptr);
				break;

			case MTYPE_ACTION:
				Action_Draw((menuaction_s *)itemptr);
				break;

			case MTYPE_BITMAP:
				Bitmap_Draw((menubitmap_s *)itemptr);
				break;

			case MTYPE_TEXT:
				Text_Draw((menutext_s *)itemptr);
				break;

			case MTYPE_SCROLLLIST:
				ScrollList_Draw((menulist_s *)itemptr);
				break;

			case MTYPE_BITMAP1024S:
				Bitmap1024S_Draw((menubitmap1024s_s *)itemptr);
				break;
			case MTYPE_TEXTS:
				TextS_Draw((menutext_s *)itemptr);
				break;

			default:
				trap_Error(va("Menu_Draw: unknown type %d", itemptr->type));
			}
		}
#ifndef NDEBUG
		if (uis.debug) {
			int x;
			int y;
			int w;
			int h;

			if (!(itemptr->flags & QMF_INACTIVE)) {
				x = itemptr->left;
				y = itemptr->top;
				w = itemptr->right - itemptr->left + 1;
				h = itemptr->bottom - itemptr->top + 1;

				if (itemptr->flags & QMF_HASMOUSEFOCUS) {
					UI_DrawRect(x, y, w, h, colorYellow, 1);
				} else {
					UI_DrawRect(x, y, w, h, colorWhite, 1);
				}
			}
		}
#endif
	}

	itemptr = Menu_ItemAtCursor(menu);
	if (itemptr) {
		if (itemptr->statusbar)
			itemptr->statusbar((void *)itemptr);
		if (trap_Cvar_VariableValue("ui_drawToolTip") != 0) {
			UI_DrawToolTip(itemptr);
		}
	}

	if (uis.dropdownlist) {
		vec4_t dropdownBG;
		int mouseover = (uis.cursory - uis.dropdownxywh[1]) / SMALLCHAR_HEIGHT;
		mouseover = Com_Clamp(0, uis.dropdownlist->numitems, mouseover);

		dropdownBG[0] = uis.dropdownxywh[0] - 5;
		dropdownBG[1] = uis.dropdownxywh[1] - 5;
		dropdownBG[2] = uis.dropdownxywh[2] + 10;
		dropdownBG[3] = uis.dropdownxywh[3] + 10;

		// UI_FillRect(uis.dropdownxywh[0]+8,uis.dropdownxywh[1]+8,uis.dropdownxywh[2],uis.dropdownxywh[3],tblack);
		UI_FillRect(dropdownBG[0], dropdownBG[1], dropdownBG[2], dropdownBG[3], uis.dropdownlist->dropdown_bg);

		for (i = 0; i < uis.dropdownlist->numitems; i++) {
			if (i == mouseover)
				UI_FillRect(uis.dropdownxywh[0], uis.dropdownxywh[1] + i * SMALLCHAR_HEIGHT, uis.dropdownxywh[2],
							SMALLCHAR_HEIGHT, uis.dropdownlist->dropdown_mark);
			UI_DrawStringNS(uis.dropdownxywh[0], uis.dropdownxywh[1] + i * SMALLCHAR_HEIGHT,
							uis.dropdownlist->itemnames[i], UI_LEFT | UI_SMALLFONT, 16,
							uis.dropdownlist->dropdown_text);
		}

		UI_DrawRect(dropdownBG[0], dropdownBG[1], dropdownBG[2], dropdownBG[3], uis.dropdownlist->dropdown_border, 2);
	}
}

/*
=================
Menu_ItemAtCursor
=================
*/
void *Menu_ItemAtCursor(menuframework_s *m) {
	if (m->cursor < 0 || m->cursor >= m->nitems)
		return NULL;

	return m->items[m->cursor];
}

/*
=================
Menu_ActivateItem
=================
*/
sfxHandle_t Menu_ActivateItem(menuframework_s *s, menucommon_s *item) {
	if (item->callback) {
		item->callback(item, QM_ACTIVATED);
		if (!(item->flags & QMF_SILENT)) {
			return menuClickSound;
		}
	}

	return 0;
}

/*
=================
Menu_DefaultKey
=================
*/
sfxHandle_t Menu_DefaultKey(menuframework_s *m, int key) {
	sfxHandle_t sound = 0;
	menucommon_s *item;
	int cursor_prev;

	if (uis.dropdownlist && key != K_F12) {
		if (key != K_MOUSE1) {
			uis.dropdownlist = NULL;
			return 0;
		}

		if (uis.cursorx > uis.dropdownxywh[0] && uis.cursorx < uis.dropdownxywh[0] + uis.dropdownxywh[2] &&
			uis.cursory > uis.dropdownxywh[1] && uis.cursory < uis.dropdownxywh[1] + uis.dropdownxywh[3]) {
			uis.dropdownlist->curvalue = (uis.cursory - uis.dropdownxywh[1]) / SMALLCHAR_HEIGHT; // uis.dropdownxywh[3];
			if (uis.dropdownlist->curvalue < 0 || uis.dropdownlist->curvalue >= uis.dropdownlist->numitems) {
				Com_Printf("ERROR: wrong number @ listdropdown???\n");
				uis.dropdownlist->curvalue = 0;
			}
			if (uis.dropdownlist->generic.callback)
				uis.dropdownlist->generic.callback(uis.dropdownlist, QM_ACTIVATED);
			uis.dropdownlist = NULL;
			return 0;
		} else {
			uis.dropdownlist = NULL;
			return 0;
		}
	}

	// menu system keys
	switch (key) {
	case K_MOUSE2:
	case K_ESCAPE:
		UI_PopMenu();
		return menuNullSound;
	}

	if (!m || !m->nitems)
		return 0;

	// route key stimulus to widget
	item = Menu_ItemAtCursor(m);
	if (item && !(item->flags & (QMF_GRAYED | QMF_INACTIVE))) {
		switch (item->type) {
		case MTYPE_SPINCONTROL:
			sound = SpinControl_Key((menulist_s *)item, key);
			break;

		case MTYPE_RADIOBUTTON:
			sound = RadioButton_Key((menuradiobutton_s *)item, key);
			break;

		case MTYPE_SLIDER:
			sound = Slider_Key((menuslider_s *)item, key);
			break;

		case MTYPE_SCROLLLIST:
			sound = ScrollList_Key((menulist_s *)item, key);
			break;

		case MTYPE_FIELD:
			sound = MenuField_Key((menufield_s *)item, &key);
			break;
		}

		if (sound) {
			// key was handled
			return sound;
		}
	}

	// default handling
	switch (key) {
#ifndef NDEBUG
	case K_F11:
		if (UI_GetCvarInt("developer"))
			uis.debug ^= 1;
		break;

	case K_F12:
		trap_Cmd_ExecuteText(EXEC_APPEND, "screenshot\n");
		break;
#endif
	case K_KP_UPARROW:
	case K_UPARROW:
		cursor_prev = m->cursor;
		m->cursor_prev = m->cursor;
		m->cursor--;
		Menu_AdjustCursor(m, -1);
		if (cursor_prev != m->cursor) {
			Menu_CursorMoved(m);
			sound = menuMoveSound;
		}
		break;

	case K_TAB:
	case K_KP_DOWNARROW:
	case K_DOWNARROW:
		cursor_prev = m->cursor;
		m->cursor_prev = m->cursor;
		m->cursor++;
		Menu_AdjustCursor(m, 1);
		if (cursor_prev != m->cursor) {
			Menu_CursorMoved(m);
			sound = menuMoveSound;
		}
		break;

	case K_MOUSE1:
	case K_MOUSE3:
		if (item)
			if ((item->flags & QMF_HASMOUSEFOCUS) && !(item->flags & (QMF_GRAYED | QMF_INACTIVE)))
				return (Menu_ActivateItem(m, item));
		break;

	case K_JOY1:
	case K_JOY2:
	case K_JOY3:
	case K_JOY4:
	case K_AUX1:
	case K_AUX2:
	case K_AUX3:
	case K_AUX4:
	case K_AUX5:
	case K_AUX6:
	case K_AUX7:
	case K_AUX8:
	case K_AUX9:
	case K_AUX10:
	case K_AUX11:
	case K_AUX12:
	case K_AUX13:
	case K_AUX14:
	case K_AUX15:
	case K_AUX16:
	case K_KP_ENTER:
	case K_ENTER:
		if (item)
			if (!(item->flags & (QMF_MOUSEONLY | QMF_GRAYED | QMF_INACTIVE)))
				return (Menu_ActivateItem(m, item));
		break;
	}

	return sound;
}

/*
=================
Menu_Cache
=================
*/
void Menu_Cache(void) {
	uis.charsetShader = trap_R_RegisterShaderNoMip("fontascii");
	uis.charsetProp = trap_R_RegisterShaderNoMip("gfx/2d/fontmenu");
	uis.cursor = trap_R_RegisterShaderNoMip("menu/art/3_cursor2");
	uis.rb_on = trap_R_RegisterShaderNoMip("menu/art/switch_on");
	uis.rb_off = trap_R_RegisterShaderNoMip("menu/art/switch_off");

	uis.whiteShader = trap_R_RegisterShaderNoMip("white");
	uis.connectbg = trap_R_RegisterShaderNoMip("menu/bg/connecting");

	uis.menubgfx = trap_R_RegisterShaderNoMip("menu/bg/menubgfx");
	uis.mainbgfx = trap_R_RegisterShaderNoMip("menu/bg/mainbgfx");
	uis.mainbg = trap_R_RegisterShaderNoMip("menu/bg/main");
	uis.setupbg = trap_R_RegisterShaderNoMip("menu/bg/setup");
	uis.systembg = trap_R_RegisterShaderNoMip("menu/bg/system");
	uis.defaultsbg = trap_R_RegisterShaderNoMip("menu/bg/defaults");
	uis.controlsbg = trap_R_RegisterShaderNoMip("menu/bg/controls");
	uis.playerbg = trap_R_RegisterShaderNoMip("menu/bg/player");
	uis.serversbg = trap_R_RegisterShaderNoMip("menu/bg/servers");
	uis.modsbg = trap_R_RegisterShaderNoMip("menu/bg/mods");
	uis.demosbg = trap_R_RegisterShaderNoMip("menu/bg/demos");
	uis.specifybg = trap_R_RegisterShaderNoMip("menu/bg/specify");
	uis.specifypassbg = trap_R_RegisterShaderNoMip("menu/bg/password");
	uis.preferencesbg = trap_R_RegisterShaderNoMip("menu/bg/preferences");
	uis.startserverbg = trap_R_RegisterShaderNoMip("menu/bg/startserver");
	uis.selectbotsbg = trap_R_RegisterShaderNoMip("menu/bg/selectbots");
	uis.ingamebg = trap_R_RegisterShaderNoMip("menu/bg/ingame");

	menuSwitchSound = trap_S_RegisterSound("sound/feedback/menu/switch", qfalse);
	menuClickSound = trap_S_RegisterSound("sound/feedback/menu/click", qfalse);
	menuMoveSound = trap_S_RegisterSound("sound/feedback/menu/move", qfalse);
	menuBuzzSound = trap_S_RegisterSound("sound/feedback/menu/buzz", qfalse);
	weaponChangeSound = trap_S_RegisterSound("sound/weapons/change", qfalse);

	// need a nonzero sound, make an empty sound for this
	menuNullSound = -1;

	sliderBar = trap_R_RegisterShaderNoMip("menu/art/slider2");
	sliderButton_0 = trap_R_RegisterShaderNoMip("menu/art/sliderbutt_0");
	sliderButton_1 = trap_R_RegisterShaderNoMip("menu/art/sliderbutt_1");
}
