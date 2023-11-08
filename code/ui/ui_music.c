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

/*
=======================================================================

MUSIC PLAYER MENU

=======================================================================
*/

#define MUSICBG "menu/bg/music"
#define PLAY0 "menu/buttons/mp_play0"
#define PLAY1 "menu/buttons/mp_play1"
#define STOP0 "menu/buttons/mp_stop0"
#define STOP1 "menu/buttons/mp_stop1"
#define PREVSONG0 "menu/buttons/mp_prevs0"
#define PREVSONG1 "menu/buttons/mp_prevs1"
#define NEXTSONG0 "menu/buttons/mp_nexts0"
#define NEXTSONG1 "menu/buttons/mp_nexts1"
#define PREVALBUM0 "menu/buttons/mp_preva0"
#define PREVALBUM1 "menu/buttons/mp_preva1"
#define NEXTALBUM0 "menu/buttons/mp_nexta0"
#define NEXTALBUM1 "menu/buttons/mp_nexta1"
#define EXIT0 "menu/buttons/mp_exit0"
#define EXIT1 "menu/buttons/mp_exit1"

#define ID_PLAY 10
#define ID_STOP 11
#define ID_NEXTSONG 12
#define ID_PREVSONG 13
#define ID_NEXTALBUM 14
#define ID_PREVALBUM 15
#define ID_EXIT 16

#define MAX_TRACKS 15
#define MAX_ALBUMS 10
#define MAX_TRACKNAME 64
#define MAX_ALBUMNAME 32

typedef struct {
	menuframework_s menu;

	menubitmap_s musicbg;
	menubitmap_s play;
	menubitmap_s stop;
	menubitmap_s nextsong;
	menubitmap_s prevsong;
	menubitmap_s nextalbum;
	menubitmap_s prevalbum;
	menubitmap_s exit;

} music_t;

static music_t s_music;

/*
=================
UI_Music_UpdateMenuItems
=================
*/
static void UI_Music_UpdateMenuItems(void) {
/*	if (UI_GetCvarInt("r_fullscreen") == 0) {
		s_graphicsoptions.colordepth.generic.flags |= QMF_GRAYED;
	} else {
		s_graphicsoptions.colordepth.generic.flags &= ~QMF_GRAYED;
	}

	if (Q_stricmp(UI_Cvar_VariableString("cl_renderer"), "opengl2") ||
		(s_graphicsoptions.renderer.curvalue != 1)) {
		s_graphicsoptions.effects.generic.flags |= QMF_GRAYED;
	} else {
		s_graphicsoptions.effects.generic.flags &= ~QMF_GRAYED;
	}

	s_graphicsoptions.apply.generic.flags |= QMF_HIDDEN | QMF_INACTIVE;
	if (s_ivo.mode != s_graphicsoptions.mode.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (s_ivo.renderer != s_graphicsoptions.renderer.curvalue) {
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
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
	}*/

}

/*
=================
UI_Music_MenuEvent
=================
*/
static void UI_Music_MenuEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_PLAY:
		break;

	case ID_STOP:
		break;

	case ID_NEXTSONG:
		break;

	case ID_PREVSONG:
		break;

	case ID_NEXTALBUM:
		break;

	case ID_PREVALBUM:
		break;

	case ID_EXIT:
		UI_StartMusic();
		UI_PopMenu();
		break;
	}
}

/*
================
UI_Music_Key
================
*/
static sfxHandle_t UI_Music_Key(int key) {
	switch (key) {
	case K_ESCAPE:
		UI_Music_MenuEvent(&s_music.exit, QM_ACTIVATED);
		break;
	}

	return Menu_DefaultKey(&s_music.menu, key);
}

/*
================
UI_Music_InitAlbums
================
*/
void UI_Music_InitAlbums(void) {



}

/*
================
UI_Music_DrawAlbums
================
*/
static void UI_Music_DrawAlbums(void) {

	// if (musicInfo.albums[musicMenu.currentAlbum].background) {
	// 	UI_DrawHandlePic((202), 60, 374, 340, musicInfo.albums[musicMenu.currentAlbum].background);
	//}
}

/*
================
UI_Music_MenuInit
================
*/
void UI_Music_MenuInit(void) {
	memset(&s_music, 0, sizeof(music_t));

	// zero set all our globals
	UI_Music_Cache();
	UI_Music_InitAlbums();

	s_music.menu.wrapAround = qtrue;
	s_music.menu.fullscreen = qtrue;
	s_music.menu.key = UI_Music_Key;
	
	UI_Music_DrawAlbums();

	s_music.musicbg.generic.type = MTYPE_BITMAP;
	s_music.musicbg.generic.name = MUSICBG;
	s_music.musicbg.generic.x = 0;
	s_music.musicbg.generic.y = 0;
	s_music.musicbg.width = SCREEN_WIDTH;
	s_music.musicbg.height = SCREEN_HEIGHT;

	s_music.play.generic.type = MTYPE_BITMAP;
	s_music.play.generic.name = PLAY0;
	s_music.play.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_music.play.generic.callback = UI_Music_MenuEvent;
	s_music.play.generic.id = ID_PLAY;
	s_music.play.generic.x = 433;
	s_music.play.generic.y = 323;
	s_music.play.width = 44;
	s_music.play.height = 44;
	s_music.play.focuspic = PLAY1;
	s_music.play.focuspicinstead = qtrue;

	s_music.stop.generic.type = MTYPE_BITMAP;
	s_music.stop.generic.name = STOP0;
	s_music.stop.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_music.stop.generic.callback = UI_Music_MenuEvent;
	s_music.stop.generic.id = ID_STOP;
	s_music.stop.generic.x = 387;
	s_music.stop.generic.y = 323;
	s_music.stop.width = 44;
	s_music.stop.height = 44;
	s_music.stop.focuspic = STOP1;
	s_music.stop.focuspicinstead = qtrue;

	s_music.nextsong.generic.type = MTYPE_BITMAP;
	s_music.nextsong.generic.name = NEXTSONG0;
	s_music.nextsong.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_music.nextsong.generic.callback = UI_Music_MenuEvent;
	s_music.nextsong.generic.id = ID_NEXTSONG;
	s_music.nextsong.generic.x = 479;
	s_music.nextsong.generic.y = 323;
	s_music.nextsong.width = 50;
	s_music.nextsong.height = 44;
	s_music.nextsong.focuspic = NEXTSONG1;
	s_music.nextsong.focuspicinstead = qtrue;

	s_music.prevsong.generic.type = MTYPE_BITMAP;
	s_music.prevsong.generic.name = PREVSONG0;
	s_music.prevsong.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_music.prevsong.generic.callback = UI_Music_MenuEvent;
	s_music.prevsong.generic.id = ID_PREVSONG;
	s_music.prevsong.generic.x = 335;
	s_music.prevsong.generic.y = 323;
	s_music.prevsong.width = 50;
	s_music.prevsong.height = 44;
	s_music.prevsong.focuspic = PREVSONG1;
	s_music.prevsong.focuspicinstead = qtrue;

	s_music.nextalbum.generic.type = MTYPE_BITMAP;
	s_music.nextalbum.generic.name = NEXTALBUM0;
	s_music.nextalbum.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_music.nextalbum.generic.callback = UI_Music_MenuEvent;
	s_music.nextalbum.generic.id = ID_NEXTALBUM;
	s_music.nextalbum.generic.x = 531;
	s_music.nextalbum.generic.y = 323;
	s_music.nextalbum.width = 50;
	s_music.nextalbum.height = 44;
	s_music.nextalbum.focuspic = NEXTALBUM1;
	s_music.nextalbum.focuspicinstead = qtrue;

	s_music.prevalbum.generic.type = MTYPE_BITMAP;
	s_music.prevalbum.generic.name = PREVALBUM0;
	s_music.prevalbum.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_music.prevalbum.generic.callback = UI_Music_MenuEvent;
	s_music.prevalbum.generic.id = ID_PREVALBUM;
	s_music.prevalbum.generic.x = 283;
	s_music.prevalbum.generic.y = 323;
	s_music.prevalbum.width = 50;
	s_music.prevalbum.height = 44;
	s_music.prevalbum.focuspic = PREVALBUM1;
	s_music.prevalbum.focuspicinstead = qtrue;

	s_music.exit.generic.type = MTYPE_BITMAP;
	s_music.exit.generic.name = EXIT0;
	s_music.exit.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_music.exit.generic.callback = UI_Music_MenuEvent;
	s_music.exit.generic.id = ID_EXIT;
	s_music.exit.generic.x = 750;
	s_music.exit.generic.y = 192;
	s_music.exit.width = 28;
	s_music.exit.height = 28;
	s_music.exit.focuspic = EXIT1;
	s_music.exit.focuspicinstead = qtrue;

	Menu_AddItem(&s_music.menu, &s_music.musicbg);
	Menu_AddItem(&s_music.menu, &s_music.play);
	Menu_AddItem(&s_music.menu, &s_music.stop);
	Menu_AddItem(&s_music.menu, &s_music.nextsong);
	Menu_AddItem(&s_music.menu, &s_music.prevsong);
	Menu_AddItem(&s_music.menu, &s_music.nextalbum);
	Menu_AddItem(&s_music.menu, &s_music.prevalbum);
	Menu_AddItem(&s_music.menu, &s_music.exit);
}

/*
=================
UI_Music_Cache
=================
*/
void UI_Music_Cache(void) {
	trap_R_RegisterShaderNoMip(MUSICBG);
	trap_R_RegisterShaderNoMip(PLAY0);
	trap_R_RegisterShaderNoMip(PLAY1);
	trap_R_RegisterShaderNoMip(STOP0);
	trap_R_RegisterShaderNoMip(STOP1);
	trap_R_RegisterShaderNoMip(NEXTSONG0);
	trap_R_RegisterShaderNoMip(NEXTSONG1);
	trap_R_RegisterShaderNoMip(PREVSONG0);
	trap_R_RegisterShaderNoMip(PREVSONG1);
	trap_R_RegisterShaderNoMip(NEXTALBUM0);
	trap_R_RegisterShaderNoMip(NEXTALBUM1);
	trap_R_RegisterShaderNoMip(PREVALBUM0);
	trap_R_RegisterShaderNoMip(PREVALBUM1);
	trap_R_RegisterShaderNoMip(EXIT0);
	trap_R_RegisterShaderNoMip(EXIT1);
}

/*
=================
UI_MusicMenu
=================
*/
void UI_MusicMenu(void) {
	trap_S_StopBackgroundTrack();
	UI_Music_MenuInit();
	UI_PushMenu(&s_music.menu);
}
