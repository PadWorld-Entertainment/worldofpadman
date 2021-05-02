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
#ifndef __UI_SHARED_H
#define __UI_SHARED_H

#include "../qcommon/q_shared.h"
#include "../renderercommon/tr_types.h"
#include "../client/keycodes.h"

#include "../../ui/menudef.h"

#define MAX_MENUNAME 32
#define MAX_ITEMTEXT 64
#define MAX_ITEMACTION 64
#define MAX_MENUDEFFILE 4096
#define MAX_MENUFILE 32768
#define MAX_MENUS 64
#define MAX_MENUITEMS 96
#define MAX_COLOR_RANGES 10
#define MAX_OPEN_MENUS 16

#define WINDOW_MOUSEOVER 0x00000001		// mouse is over it, non exclusive
#define WINDOW_HASFOCUS 0x00000002		// has cursor focus, exclusive
#define WINDOW_VISIBLE 0x00000004		// is visible
#define WINDOW_GREY 0x00000008			// is visible but grey ( non-active )
#define WINDOW_DECORATION 0x00000010	// for decoration only, no mouse, keyboard, etc..
#define WINDOW_FADINGOUT 0x00000020		// fading out, non-active
#define WINDOW_FADINGIN 0x00000040		// fading in
#define WINDOW_MOUSEOVERTEXT 0x00000080 // mouse is over it, non exclusive
#define WINDOW_INTRANSITION 0x00000100	// window is in transition
#define WINDOW_FORECOLORSET 0x00000200	// forecolor was explicitly set ( used to color alpha images or not )
#define WINDOW_HORIZONTAL 0x00000400	// for list boxes and sliders, vertical is default this is set of horizontal
#define WINDOW_LB_LEFTARROW 0x00000800	// mouse is over left/up arrow
#define WINDOW_LB_RIGHTARROW 0x00001000 // mouse is over right/down arrow
#define WINDOW_LB_THUMB 0x00002000		// mouse is over thumb
#define WINDOW_LB_PGUP 0x00004000		// mouse is over page up
#define WINDOW_LB_PGDN 0x00008000		// mouse is over page down
#define WINDOW_ORBITING 0x00010000		// item is in orbit
#define WINDOW_OOB_CLICK 0x00020000		// close on out of bounds click
#define WINDOW_WRAPPED 0x00040000		// manually wrap text
#define WINDOW_AUTOWRAPPED 0x00080000	// auto wrap text
#define WINDOW_FORCED 0x00100000		// forced open
#define WINDOW_POPUP 0x00200000			// popup
#define WINDOW_BACKCOLORSET 0x00400000	// backcolor was explicitly set
#define WINDOW_TIMEDVISIBLE 0x00800000	// visibility timing ( NOT implemented )

typedef struct {
	float x; // horiz position
	float y; // vert position
	float w; // width
	float h; // height;
} rectDef_t;

typedef rectDef_t Rectangle;

// FIXME: do something to separate text vs window stuff
typedef struct {
	Rectangle rect;			   // client coord rectangle
	Rectangle rectClient;	   // screen coord rectangle
	const char *name;		   //
	const char *group;		   // if it belongs to a group
	const char *cinematicName; // cinematic name
	int cinematic;			   // cinematic handle
	int style;				   //
	int border;				   //
	int ownerDraw;			   // ownerDraw style
	int ownerDrawFlags;		   // show flags for ownerdraw items
	float borderSize;		   //
	int flags;				   // visible, focus, mouseover, cursor
	Rectangle rectEffects;	   // for various effects
	Rectangle rectEffects2;	   // for various effects
	int offsetTime;			   // time based value for various effects
	int nextTime;			   // time next effect should cycle
	vec4_t foreColor;		   // text color
	vec4_t backColor;		   // border color
	vec4_t borderColor;		   // border color
	vec4_t outlineColor;	   // border color
	qhandle_t background;	   // background asset
} windowDef_t;

typedef windowDef_t Window;

int trap_PC_AddGlobalDefine(char *define);
int trap_PC_LoadSource(const char *filename);
int trap_PC_FreeSource(int handle);
int trap_PC_ReadToken(int handle, pc_token_t *pc_token);
int trap_PC_SourceFileAndLine(int handle, char *filename, int *line);

#endif
