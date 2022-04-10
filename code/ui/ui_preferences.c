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

PREFERENCES MENU

=======================================================================
*/

#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define HUD0 "menu/buttons/hud0"
#define HUD1 "menu/buttons/hud1"
#define GAME0 "menu/buttons/game0"
#define GAME1 "menu/buttons/game1"
#define CHAT0 "menu/buttons/chat0"
#define CHAT1 "menu/buttons/chat1"
#define HELP0 "menu/buttons/help0"
#define HELP1 "menu/buttons/help1"

// sections
#define O_HUD 0
#define O_GAME 1
#define O_CHAT 2
#define O_HELP 3
#define O_MAX 4

#define ID_BACK 100
#define ID_HUD 101
#define ID_GAME 102
#define ID_CHAT 103
#define ID_HELP 104

#define ID_CROSSHAIR 10
#define ID_INDICATEHEALTH 11
#define ID_IDENTIFYTARGET 12
#define ID_FFAHUDTHEME 13
#define ID_TEAMOVERLAY 14
#define ID_LAGOMETER 15
#define ID_VOIPMETER 16
#define ID_TIMER 17
#define ID_TIMELEFT 18
#define ID_REALTIME 19
#define ID_FPS 20
#define ID_UPS 21

#define ID_SIMPLEITEMS 30
#define ID_WALLMARKS 31
#define ID_DYNAMICLIGHTS 32
#define ID_FLARES 33
#define ID_HIGHQUALITYSKY 34
#define ID_LENSFLARE 35
#define ID_INGAMEVIDEO 36
#define ID_SYNCEVERYFRAME 37
#define ID_FORCEMODEL 38
#define ID_GLOWMODEL 39
#define ID_GLOWCOLOR 40

#define ID_CONNOTIFY 50
#define ID_CHATHEIGHT 51
#define ID_CHATICON 52
#define ID_CHATBEEP 53
#define ID_GESTURE 54
#define ID_BOTCHAT 55
#define ID_TEAMCHATSONLY 56

#define ID_ICONTEAMMATE 70
#define ID_ICONHSTATION 71
#define ID_ICONSYCTELE 72
#define ID_ICONBALLOON 73
#define ID_ICONLPSARROW 74
#define ID_ICONFREEZETAG 75

#define NUM_CROSSHAIRS 12

#define XPOSITION 534
#define YPOSITION 216

typedef struct {
	menuframework_s menu;

	menubitmap_s hud;
	menubitmap_s game;
	menubitmap_s chat;
	menubitmap_s help;

	menulist_s crosshair;
	menuradiobutton_s indicatehealth;
	menuradiobutton_s identifytarget;
	menulist_s ffahudtheme;
	menuradiobutton_s teamoverlay;
	menuradiobutton_s lagometer;
	menuradiobutton_s voipmeter;
	menuradiobutton_s timer;
	menuradiobutton_s timeleft;
	menuradiobutton_s realtime;
	menuradiobutton_s fps;
	menuradiobutton_s ups;

	menuradiobutton_s simpleitems;
	menuradiobutton_s wallmarks;
	menuradiobutton_s dynamiclights;
	menuradiobutton_s flares;
	menuradiobutton_s highqualitysky;
	menuradiobutton_s lensFlare;
	menuradiobutton_s ingamevideo;
	menuradiobutton_s synceveryframe;
	menuradiobutton_s forcemodel;
	menuradiobutton_s glowmodel;
	menulist_s glowcolor;

	menulist_s connotify;
	menulist_s chatheight;
	menuradiobutton_s chaticon;
	menuradiobutton_s chatbeep;
	menuradiobutton_s gesture;
	menulist_s botchat;
	menuradiobutton_s teamchatsonly;

	menutext_s whIcons;
	menuradiobutton_s whTeamMates;
	menuradiobutton_s whHStations;
	menuradiobutton_s whSycTele;
	menuradiobutton_s whBalloons;
	menuradiobutton_s whLPS;
	menuradiobutton_s whFreezeTag;

	menubitmap_s back;

	qhandle_t crosshairShader[NUM_CROSSHAIRS];
	int section;

} preferences_t;

static preferences_t s_preferences;

static menucommon_s *g_hud_options[] = {
	(menucommon_s *)&s_preferences.crosshair,
	(menucommon_s *)&s_preferences.indicatehealth,
	(menucommon_s *)&s_preferences.identifytarget,
	(menucommon_s *)&s_preferences.ffahudtheme,
	(menucommon_s *)&s_preferences.teamoverlay,
	(menucommon_s *)&s_preferences.lagometer,
	(menucommon_s *)&s_preferences.voipmeter,
	(menucommon_s *)&s_preferences.timer,
	(menucommon_s *)&s_preferences.timeleft,
	(menucommon_s *)&s_preferences.realtime,
	(menucommon_s *)&s_preferences.fps,
	(menucommon_s *)&s_preferences.ups,
	NULL
};

static menucommon_s *g_game_options[] = {
	(menucommon_s *)&s_preferences.simpleitems,
	(menucommon_s *)&s_preferences.wallmarks,
	(menucommon_s *)&s_preferences.dynamiclights,
	(menucommon_s *)&s_preferences.flares,
	(menucommon_s *)&s_preferences.highqualitysky,
	(menucommon_s *)&s_preferences.lensFlare,
	(menucommon_s *)&s_preferences.ingamevideo,
	(menucommon_s *)&s_preferences.synceveryframe,
	(menucommon_s *)&s_preferences.forcemodel,
	(menucommon_s *)&s_preferences.glowmodel,
	(menucommon_s *)&s_preferences.glowcolor,
	NULL
};

static menucommon_s *g_chat_options[] = {
	(menucommon_s *)&s_preferences.connotify,
	(menucommon_s *)&s_preferences.chatheight,
	(menucommon_s *)&s_preferences.chaticon,
	(menucommon_s *)&s_preferences.chatbeep,
	(menucommon_s *)&s_preferences.gesture,
	(menucommon_s *)&s_preferences.botchat,
	(menucommon_s *)&s_preferences.teamchatsonly,
	NULL
};

static menucommon_s *g_help_options[] = {
	(menucommon_s *)&s_preferences.whIcons,
	(menucommon_s *)&s_preferences.whTeamMates,
	(menucommon_s *)&s_preferences.whHStations,
	(menucommon_s *)&s_preferences.whSycTele,
	(menucommon_s *)&s_preferences.whBalloons,
	(menucommon_s *)&s_preferences.whLPS,
	(menucommon_s *)&s_preferences.whFreezeTag,
	NULL
};

static menucommon_s **g_options[] = {
	g_hud_options,
	g_game_options,
	g_chat_options,
	g_help_options,
};

static const char *ffahudtheme_items[] = {"Black", "Red", "Blue", "Green", "Chrome", "Whitemetal",
									 "Rust", "Flower", "Wood", "Airforce", NULL};

static const char *connotify_items[] = {"Short", "Default", "Long", "Maximum", NULL};

static const char *glowcolor_items[] = {S_COLOR_BLACK "Black", S_COLOR_RED "Red", S_COLOR_GREEN "Green",
										S_COLOR_YELLOW "Yellow", S_COLOR_BLUE "Blue", S_COLOR_CYAN "Cyan",
										S_COLOR_MAGENTA "Magenta", S_COLOR_WHITE "White", NULL};

static const char *botchat_items[] = {"Off", "Default", "Often", NULL};

static const char *chatheight_items[] = {"Default", "Expanded", "Maximum", NULL};

static void UpdateGlowColorFlags(void) {
	if (s_preferences.glowcolor.generic.flags & QMF_HIDDEN)
		return;

	if (0 == s_preferences.glowmodel.curvalue) {
		s_preferences.glowcolor.generic.flags |= (QMF_GRAYED);
	} else {
		s_preferences.glowcolor.generic.flags &= ~(QMF_GRAYED);
	}
}

/*
=================
UI_Preferences_SetMenuItems
=================
*/
static void UI_Preferences_SetMenuItems(void) {
	int notify;
	int chatheight;
	int cg_iconsCvarValue;

	s_preferences.crosshair.curvalue = (int)trap_Cvar_VariableValue("cg_drawCrosshair") % NUM_CROSSHAIRS;
	s_preferences.indicatehealth.curvalue = trap_Cvar_VariableValue("cg_drawCrosshairHealth") != 0;
	s_preferences.identifytarget.curvalue = trap_Cvar_VariableValue("cg_drawCrosshairNames") != 0;
	s_preferences.ffahudtheme.curvalue = Com_Clamp(0, 9, trap_Cvar_VariableValue("cg_wopffahud"));
	s_preferences.teamoverlay.curvalue = trap_Cvar_VariableValue("cg_drawTeamOverlay") != 0;
	s_preferences.lagometer.curvalue = trap_Cvar_VariableValue("cg_lagometer") != 0;
	s_preferences.voipmeter.curvalue = trap_Cvar_VariableValue("cl_voipShowMeter") != 0;
	s_preferences.timer.curvalue = Com_Clamp(0, 1, trap_Cvar_VariableValue("cg_drawTimer"));
	s_preferences.timeleft.curvalue = Com_Clamp(0, 1, trap_Cvar_VariableValue("cg_drawTimeLeft"));
	s_preferences.realtime.curvalue = Com_Clamp(0, 1, trap_Cvar_VariableValue("cg_drawRealTime"));
	s_preferences.fps.curvalue = Com_Clamp(0, 1, trap_Cvar_VariableValue("cg_drawFPS"));
	s_preferences.ups.curvalue = Com_Clamp(0, 1, trap_Cvar_VariableValue("cg_drawups"));

	s_preferences.simpleitems.curvalue = trap_Cvar_VariableValue("cg_simpleItems") != 0;
	s_preferences.wallmarks.curvalue = trap_Cvar_VariableValue("cg_marks") != 0;
	s_preferences.dynamiclights.curvalue = trap_Cvar_VariableValue("r_dynamiclight") != 0;
	s_preferences.flares.curvalue = trap_Cvar_VariableValue("r_flares") != 0;
	s_preferences.highqualitysky.curvalue = trap_Cvar_VariableValue("r_fastsky") == 0;
	s_preferences.lensFlare.curvalue = trap_Cvar_VariableValue("cg_drawLensflare") != 0;
	s_preferences.ingamevideo.curvalue = trap_Cvar_VariableValue("r_inGameVideo") != 0;
	s_preferences.synceveryframe.curvalue = trap_Cvar_VariableValue("r_finish") != 0;
	s_preferences.forcemodel.curvalue = (trap_Cvar_VariableValue("cg_forcemodel") != 0);
	s_preferences.glowmodel.curvalue = (s_preferences.glowcolor.curvalue != 0);
	s_preferences.glowcolor.curvalue = trap_Cvar_VariableValue("cg_glowModel"); // cg_glowModelTeam..

	notify = UI_GetCvarInt("con_notifytime");
	if (notify < 0) {
		notify *= -1;
	}
	if (notify > 0 && notify <= 2) {
		s_preferences.connotify.curvalue = 0;
	} else if (notify > 2 && notify <= 4) {
		s_preferences.connotify.curvalue = 1;
	} else if (notify > 4 && notify <= 6) {
		s_preferences.connotify.curvalue = 2;
	} else if (notify > 6) {
		s_preferences.connotify.curvalue = 3;
	}

	chatheight = trap_Cvar_VariableValue("cg_chatHeight");
	if (chatheight > 6 && chatheight <= 8) {
		s_preferences.chatheight.curvalue = 2;
	} else if (chatheight > 4 && chatheight <= 6) {
		s_preferences.chatheight.curvalue = 1;
	} else {
		s_preferences.chatheight.curvalue = 0;
	}

	s_preferences.chaticon.curvalue = trap_Cvar_VariableValue("cg_drawChatIcon") != 0;
	s_preferences.chatbeep.curvalue = trap_Cvar_VariableValue("cg_chatBeep") != 0;
	s_preferences.gesture.curvalue = trap_Cvar_VariableValue("cg_noTaunt") == 0;
	s_preferences.teamchatsonly.curvalue = trap_Cvar_VariableValue("cg_teamChatsOnly") != 0;

	if (trap_Cvar_VariableValue("bot_nochat") != 1) {
		if (trap_Cvar_VariableValue("bot_fastchat") != 0) {
			s_preferences.botchat.curvalue = 2;
		} else {
			s_preferences.botchat.curvalue = 1;			
		}
	} else {
		s_preferences.botchat.curvalue = 0;
	}

	cg_iconsCvarValue = (int)trap_Cvar_VariableValue("cg_icons");
	s_preferences.whLPS.curvalue = (ICON_ARROW & cg_iconsCvarValue);
	s_preferences.whBalloons.curvalue = (ICON_BALLOON & cg_iconsCvarValue);
	s_preferences.whTeamMates.curvalue = (ICON_TEAMMATE & cg_iconsCvarValue);
	s_preferences.whFreezeTag.curvalue = (ICON_FREEZETAG & cg_iconsCvarValue);
	s_preferences.whHStations.curvalue = (ICON_HEALTHSTATION & cg_iconsCvarValue);
	s_preferences.whSycTele.curvalue = (ICON_SPRAYROOM & cg_iconsCvarValue);

	UpdateGlowColorFlags();
}

/*
=================
UI_Preferences_Update
=================
*/
static void UI_Preferences_Update(void) {
	int i;
	int j;

	menucommon_s **options;
	menucommon_s *option;

	// disable all options in all groups
	for (i = 0; i < O_MAX; i++) {
		options = g_options[i];
		for (j = 0; (option = options[j]); j++) {
			option->flags |= (QMF_HIDDEN | QMF_INACTIVE);
		}
	}

	options = g_options[s_preferences.section];

	// enable options in active group
	for (j = 0; (option = options[j]); j++) {
		option->flags &= ~(QMF_GRAYED | QMF_HIDDEN | QMF_INACTIVE);
	}

	UpdateGlowColorFlags();

	// makes sure flags are right on the group selection controls
	s_preferences.hud.generic.flags &= ~(QMF_GRAYED | QMF_HIGHLIGHT | QMF_HIGHLIGHT_IF_FOCUS);
	s_preferences.game.generic.flags &= ~(QMF_GRAYED | QMF_HIGHLIGHT | QMF_HIGHLIGHT_IF_FOCUS);
	s_preferences.chat.generic.flags &= ~(QMF_GRAYED | QMF_HIGHLIGHT | QMF_HIGHLIGHT_IF_FOCUS);
	s_preferences.help.generic.flags &= ~(QMF_GRAYED | QMF_HIGHLIGHT | QMF_HIGHLIGHT_IF_FOCUS);

	s_preferences.hud.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.game.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.chat.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.help.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;

	// set buttons
	switch (s_preferences.section) {
	case O_HUD:
		s_preferences.hud.generic.flags |= QMF_HIGHLIGHT;
		break;

	case O_GAME:
		s_preferences.game.generic.flags |= QMF_HIGHLIGHT;
		break;

	case O_CHAT:
		s_preferences.chat.generic.flags |= QMF_HIGHLIGHT;
		break;

	case O_HELP:
		s_preferences.help.generic.flags |= QMF_HIGHLIGHT;
		break;
	}

	// special case
	if (UI_GetCvarInt("cl_voip") == 0) {
		s_preferences.voipmeter.generic.flags |= QMF_GRAYED;
	}
}

/*
=================
UI_Preferences_Event
=================
*/
static void UI_Preferences_Event(void *ptr, int notification) {

	if (notification != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_HUD:
		s_preferences.section = O_HUD;
		UI_Preferences_Update();
		break;

	case ID_GAME:
		s_preferences.section = O_GAME;
		UI_Preferences_Update();
		break;

	case ID_CHAT:
		s_preferences.section = O_CHAT;
		UI_Preferences_Update();
		break;

	case ID_HELP:
		s_preferences.section = O_HELP;
		UI_Preferences_Update();
		break;

	case ID_CROSSHAIR:
		s_preferences.crosshair.curvalue++;
		if (s_preferences.crosshair.curvalue == NUM_CROSSHAIRS) {
			s_preferences.crosshair.curvalue = 0;
		}
		trap_Cvar_SetValue("cg_drawCrosshair", s_preferences.crosshair.curvalue);
		break;

	case ID_INDICATEHEALTH:
		trap_Cvar_SetValue("cg_drawCrosshairHealth", s_preferences.indicatehealth.curvalue);
		break;

	case ID_IDENTIFYTARGET:
		trap_Cvar_SetValue("cg_drawCrosshairNames", s_preferences.identifytarget.curvalue);
		break;

	case ID_FFAHUDTHEME:
		trap_Cvar_SetValue("cg_wopffahud", s_preferences.ffahudtheme.curvalue);
		break;

	case ID_TEAMOVERLAY:
		trap_Cvar_SetValue("cg_drawTeamOverlay", s_preferences.teamoverlay.curvalue);
		break;

	case ID_LAGOMETER:
		trap_Cvar_SetValue("cg_lagometer", s_preferences.lagometer.curvalue);
		break;

	case ID_VOIPMETER:
		trap_Cvar_SetValue("cl_voipShowMeter", s_preferences.voipmeter.curvalue);
		break;

	case ID_TIMER:
		trap_Cvar_SetValue("cg_drawTimer", s_preferences.timer.curvalue);
		break;

	case ID_TIMELEFT:
		trap_Cvar_SetValue("cg_drawTimeLeft", s_preferences.timeleft.curvalue);
		break;

	case ID_REALTIME:
		trap_Cvar_SetValue("cg_drawRealTime", s_preferences.realtime.curvalue);
		break;

	case ID_FPS:
		trap_Cvar_SetValue("cg_drawFPS", s_preferences.fps.curvalue);
		break;

	case ID_UPS:
		trap_Cvar_SetValue("cg_drawUPS", s_preferences.ups.curvalue);
		break;

	case ID_SIMPLEITEMS:
		trap_Cvar_SetValue("cg_simpleItems", s_preferences.simpleitems.curvalue);
		break;

	case ID_WALLMARKS:
		trap_Cvar_SetValue("cg_marks", s_preferences.wallmarks.curvalue);
		break;

	case ID_DYNAMICLIGHTS:
		trap_Cvar_SetValue("r_dynamiclight", s_preferences.dynamiclights.curvalue);
		break;

	case ID_FLARES:
		trap_Cvar_SetValue("r_flares", s_preferences.flares.curvalue);
		break;

	case ID_HIGHQUALITYSKY:
		trap_Cvar_SetValue("r_fastsky", !s_preferences.highqualitysky.curvalue);
		break;

	case ID_LENSFLARE:
		trap_Cvar_Set("cg_drawLensflare", va("%d", s_preferences.lensFlare.curvalue));
		break;

	case ID_INGAMEVIDEO:
		trap_Cvar_SetValue("r_inGameVideo", s_preferences.ingamevideo.curvalue);
		break;

	case ID_SYNCEVERYFRAME:
		trap_Cvar_SetValue("r_finish", s_preferences.synceveryframe.curvalue);
		break;

	case ID_FORCEMODEL:
		trap_Cvar_SetValue("cg_forcemodel", s_preferences.forcemodel.curvalue);
		break;

	case ID_GLOWMODEL:
	case ID_GLOWCOLOR:
		if (s_preferences.glowmodel.curvalue == 0) {
			trap_Cvar_Set("cg_glowModel", "");
		} else {
			trap_Cvar_SetValue("cg_glowModel", s_preferences.glowcolor.curvalue); // default green enemies only
		}
		UpdateGlowColorFlags();
		break;

	case ID_CONNOTIFY:
		switch (s_preferences.connotify.curvalue) {
		case 0:
			trap_Cvar_SetValue("con_notifytime", -2);
			break;
		case 1:
			trap_Cvar_SetValue("con_notifytime", -4);
			break;
		case 2:
			trap_Cvar_SetValue("con_notifytime", -6);
			break;
		case 3:
			trap_Cvar_SetValue("con_notifytime", -8);
			break;
		}
		break;

	case ID_CHATHEIGHT:
		switch (s_preferences.chatheight.curvalue) {
		case 0:
			trap_Cvar_SetValue("cg_chatHeight", 4);
			break;
		case 1:
			trap_Cvar_SetValue("cg_chatHeight", 6);
			break;
		case 2:
			trap_Cvar_SetValue("cg_chatHeight", 8);
			break;
		}
		break;

	case ID_CHATICON:
		trap_Cvar_SetValue("cg_drawChatIcon", s_preferences.chaticon.curvalue);
		break;

	case ID_CHATBEEP:
		if (s_preferences.chatbeep.curvalue == 0) {
			trap_Cvar_SetValue("cg_chatBeep", s_preferences.chatbeep.curvalue);
		} else {
			trap_Cvar_SetValue("cg_chatBeep", 7);
		}
		break;

	case ID_GESTURE:
		trap_Cvar_SetValue("cg_noTaunt", !s_preferences.gesture.curvalue);
		break;

	case ID_TEAMCHATSONLY:
		trap_Cvar_SetValue("cg_teamChatsOnly", s_preferences.teamchatsonly.curvalue);
		break;

	case ID_BOTCHAT:
		if (s_preferences.botchat.curvalue != 0) {
			trap_Cvar_SetValue("bot_nochat", 0);
			if (s_preferences.botchat.curvalue == 2) {
				trap_Cvar_SetValue("bot_fastchat", 1);
			} else {
				trap_Cvar_SetValue("bot_fastchat", 0);
			}
		} else {
			trap_Cvar_SetValue("bot_nochat", 1);
		}
		break;

	case ID_ICONLPSARROW:
	case ID_ICONTEAMMATE:
	case ID_ICONBALLOON:
	case ID_ICONFREEZETAG:
	case ID_ICONHSTATION:
	case ID_ICONSYCTELE: {
		// this is only necessary if cg_icons contains flags that we don't cover here
		int icons = (UI_GetCvarInt("cg_icons") & ~ICON_ALL);

		if (s_preferences.whLPS.curvalue) {
			icons |= ICON_ARROW;
		}
		if (s_preferences.whBalloons.curvalue) {
			icons |= ICON_BALLOON;
		}
		if (s_preferences.whTeamMates.curvalue) {
			icons |= ICON_TEAMMATE;
		}
		if (s_preferences.whFreezeTag.curvalue) {
			icons |= ICON_FREEZETAG;
		}
		if (s_preferences.whHStations.curvalue) {
			icons |= ICON_HEALTHSTATION;
		}
		if (s_preferences.whSycTele.curvalue) {
			icons |= ICON_SPRAYROOM;
		}

		trap_Cvar_SetValue("cg_icons", icons);
		break;
	}

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}

/*
=================
Crosshair_Draw
=================
*/
static void Crosshair_Draw(void *self) {
	menulist_s *s;
	const float *color;
	int x, y;
	int style;
	qboolean focus;

	s = (menulist_s *)self;
	x = s->generic.x;
	y = s->generic.y;

	style = UI_SMALLFONT;
	focus = (s->generic.parent->cursor == s->generic.menuPosition);

	if (s->generic.flags & QMF_GRAYED)
		color = text_color_disabled;
	else if (focus) {
		color = text_color_highlight;
		style |= UI_PULSE;
	} else if (s->generic.flags & QMF_BLINK) {
		color = text_color_highlight;
		style |= UI_BLINK;
	} else
		color = text_color_normal;

	if (focus) {
		// draw cursor
		UI_FillRect(s->generic.left, s->generic.top, s->generic.right - s->generic.left + 1,
					s->generic.bottom - s->generic.top + 1, listbar_color);
		UI_DrawChar(x, y, FONT_ASCII_TRIANGLE, UI_CENTER | UI_BLINK | UI_SMALLFONT, color);
	}

	UI_DrawString(x - SMALLCHAR_WIDTH, y, s->generic.name, style | UI_RIGHT, color);
	if (!s->curvalue) {
		return;
	}
	UI_DrawHandlePic(x + SMALLCHAR_WIDTH, y - 4, 24, 24, s_preferences.crosshairShader[s->curvalue]);
}

/*
===============
UI_Preferences_MenuInit
===============
*/
static void UI_Preferences_MenuInit(void) {
	int y;

	memset(&s_preferences, 0, sizeof(preferences_t));

	UI_Preferences_Cache();

	s_preferences.menu.wrapAround = qtrue;
	s_preferences.menu.fullscreen = qtrue;
	s_preferences.menu.bgparts = BGP_PREFERENCES | BGP_MENUFX;

	//menu
	s_preferences.hud.generic.type = MTYPE_BITMAP;
	s_preferences.hud.generic.name = HUD0;
	s_preferences.hud.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.hud.generic.callback = UI_Preferences_Event;
	s_preferences.hud.generic.id = ID_HUD;
	s_preferences.hud.generic.x = 428;
	s_preferences.hud.generic.y = 30;
	s_preferences.hud.width = 80;
	s_preferences.hud.height = 40;
	s_preferences.hud.focuspic = HUD1;
	s_preferences.hud.focuspicinstead = qtrue;

	s_preferences.game.generic.type = MTYPE_BITMAP;
	s_preferences.game.generic.name = GAME0;
	s_preferences.game.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.game.generic.callback = UI_Preferences_Event;
	s_preferences.game.generic.id = ID_GAME;
	s_preferences.game.generic.x = 514;
	s_preferences.game.generic.y = 40;
	s_preferences.game.width = 80;
	s_preferences.game.height = 40;
	s_preferences.game.focuspic = GAME1;
	s_preferences.game.focuspicinstead = qtrue;

	s_preferences.chat.generic.type = MTYPE_BITMAP;
	s_preferences.chat.generic.name = CHAT0;
	s_preferences.chat.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.chat.generic.callback = UI_Preferences_Event;
	s_preferences.chat.generic.id = ID_CHAT;
	s_preferences.chat.generic.x = 402;
	s_preferences.chat.generic.y = 72;
	s_preferences.chat.width = 80;
	s_preferences.chat.height = 40;
	s_preferences.chat.focuspic = CHAT1;
	s_preferences.chat.focuspicinstead = qtrue;

	s_preferences.help.generic.type = MTYPE_BITMAP;
	s_preferences.help.generic.name = HELP0;
	s_preferences.help.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.help.generic.callback = UI_Preferences_Event;
	s_preferences.help.generic.id = ID_HELP;
	s_preferences.help.generic.x = 490;
	s_preferences.help.generic.y = 80;
	s_preferences.help.width = 80;
	s_preferences.help.height = 40;
	s_preferences.help.focuspic = HELP1;
	s_preferences.help.focuspicinstead = qtrue;

	y = YPOSITION;
	// hud options
	s_preferences.crosshair.generic.type = MTYPE_TEXT;
	s_preferences.crosshair.generic.flags = QMF_SMALLFONT | QMF_NODEFAULTINIT | QMF_OWNERDRAW;
	s_preferences.crosshair.generic.x = XPOSITION;
	s_preferences.crosshair.generic.y = y;
	s_preferences.crosshair.generic.name = "Crosshair:";
	s_preferences.crosshair.generic.callback = UI_Preferences_Event;
	s_preferences.crosshair.generic.ownerdraw = Crosshair_Draw;
	s_preferences.crosshair.generic.id = ID_CROSSHAIR;
	s_preferences.crosshair.generic.top = y - 4;
	s_preferences.crosshair.generic.bottom = y + 20;
	s_preferences.crosshair.generic.left =
		XPOSITION - ((strlen(s_preferences.crosshair.generic.name) + 1) * SMALLCHAR_WIDTH);
	s_preferences.crosshair.generic.right = XPOSITION + 48;
	s_preferences.crosshair.generic.toolTip =
		"Select your favorite crosshair design by clicking on the image.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.indicatehealth.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.indicatehealth.generic.name = "Indicate Health:";
	s_preferences.indicatehealth.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.indicatehealth.generic.callback = UI_Preferences_Event;
	s_preferences.indicatehealth.generic.id = ID_INDICATEHEALTH;
	s_preferences.indicatehealth.generic.x = XPOSITION;
	s_preferences.indicatehealth.generic.y = y;
	s_preferences.indicatehealth.generic.toolTip =
		"Enable to indicate your health status by the color change of the crosshair. As health points "
		"decrease, the crosshair turn yellow and then red. Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.identifytarget.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.identifytarget.generic.name = "Identify Target:";
	s_preferences.identifytarget.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.identifytarget.generic.callback = UI_Preferences_Event;
	s_preferences.identifytarget.generic.id = ID_IDENTIFYTARGET;
	s_preferences.identifytarget.generic.x = XPOSITION;
	s_preferences.identifytarget.generic.y = y;
	s_preferences.identifytarget.generic.toolTip =
		"Enable to display the name of the player you have actively in your crosshair. The name will "
		"disappear once out of your crosshair. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.ffahudtheme.generic.type = MTYPE_SPINCONTROL;
	s_preferences.ffahudtheme.generic.name = "HUD Color/Theme:";
	s_preferences.ffahudtheme.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.ffahudtheme.generic.callback = UI_Preferences_Event;
	s_preferences.ffahudtheme.generic.id = ID_FFAHUDTHEME;
	s_preferences.ffahudtheme.generic.x = XPOSITION;
	s_preferences.ffahudtheme.generic.y = y;
	s_preferences.ffahudtheme.itemnames = ffahudtheme_items;
	s_preferences.ffahudtheme.generic.toolTip =
		"Choose your favorite HUD color or theme for non-team-based gametypes. Default is black. "
		"NOTE: In team gametypes, the color or theme is always set to red or blue depending on which "
		"team you join. ";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.teamoverlay.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.teamoverlay.generic.name = "Team Overlay:";
	s_preferences.teamoverlay.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.teamoverlay.generic.callback = UI_Preferences_Event;
	s_preferences.teamoverlay.generic.id = ID_TEAMOVERLAY;
	s_preferences.teamoverlay.generic.x = XPOSITION;
	s_preferences.teamoverlay.generic.y = y;
	s_preferences.teamoverlay.generic.toolTip =
		"Disable to hide the overlay with additional information about up to four teammates on the "
		"left side of the screen. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.lagometer.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.lagometer.generic.name = "Lagometer:";
	s_preferences.lagometer.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.lagometer.generic.callback = UI_Preferences_Event;
	s_preferences.lagometer.generic.id = ID_LAGOMETER;
	s_preferences.lagometer.generic.x = XPOSITION;
	s_preferences.lagometer.generic.y = y;
	s_preferences.lagometer.generic.toolTip =
		"Enable to display a lagometer to indicate the current quality (ping) of your server connection "
		"in the bottom right of the screen. It is not shown when playing locally. Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.voipmeter.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.voipmeter.generic.name = "VoIP Volume Meter:";
	s_preferences.voipmeter.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.voipmeter.generic.callback = UI_Preferences_Event;
	s_preferences.voipmeter.generic.id = ID_VOIPMETER;
	s_preferences.voipmeter.generic.x = XPOSITION;
	s_preferences.voipmeter.generic.y = y;
	s_preferences.voipmeter.generic.toolTip =
		"Disable to not display the VoIP chat volume meter during voice recording via the microphone, which "
		"indicates your noise level. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.timer.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.timer.generic.name = "Timer:";
	s_preferences.timer.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.timer.generic.callback = UI_Preferences_Event;
	s_preferences.timer.generic.id = ID_TIMER;
	s_preferences.timer.generic.x = XPOSITION;
	s_preferences.timer.generic.y = y;
	s_preferences.timer.generic.toolTip =
		"Enable to display a timer with the current game time in the upper right corner of the screen. "
		"Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.timeleft.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.timeleft.generic.name = "Time Left:";
	s_preferences.timeleft.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.timeleft.generic.callback = UI_Preferences_Event;
	s_preferences.timeleft.generic.id = ID_TIMELEFT;
	s_preferences.timeleft.generic.x = XPOSITION;
	s_preferences.timeleft.generic.y = y;
	s_preferences.timeleft.generic.toolTip =
		"Enable to display the current remaining game time in the upper right corner of the screen. "
		"Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.realtime.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.realtime.generic.name = "Real Time:";
	s_preferences.realtime.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.realtime.generic.callback = UI_Preferences_Event;
	s_preferences.realtime.generic.id = ID_REALTIME;
	s_preferences.realtime.generic.x = XPOSITION;
	s_preferences.realtime.generic.y = y;
	s_preferences.realtime.generic.toolTip =
		"Enable to display the current time of your system  in the upper right corner of the screen. "
		"Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.fps.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.fps.generic.name = "Frame Rate:";
	s_preferences.fps.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.fps.generic.callback = UI_Preferences_Event;
	s_preferences.fps.generic.id = ID_FPS;
	s_preferences.fps.generic.x = XPOSITION;
	s_preferences.fps.generic.y = y;
	s_preferences.fps.generic.toolTip =
		"Enable this option to display the system's current frame rate in frames per second (fps) in "
		"the upper right corner of the screen. Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.ups.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.ups.generic.name = "Velocity:";
	s_preferences.ups.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.ups.generic.callback = UI_Preferences_Event;
	s_preferences.ups.generic.id = ID_UPS;
	s_preferences.ups.generic.x = XPOSITION;
	s_preferences.ups.generic.y = y;
	s_preferences.ups.generic.toolTip =
		"Enable to display your current speed in game units per second (ups) in the upper right "
		"corner of the screen. Default is off.";

	// game options
	y = YPOSITION;
	s_preferences.simpleitems.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.simpleitems.generic.name = "Simple Items:";
	s_preferences.simpleitems.generic.flags = QMF_SMALLFONT;
	s_preferences.simpleitems.generic.callback = UI_Preferences_Event;
	s_preferences.simpleitems.generic.id = ID_SIMPLEITEMS;
	s_preferences.simpleitems.generic.x = XPOSITION;
	s_preferences.simpleitems.generic.y = y;
	s_preferences.simpleitems.generic.toolTip =
		"Enable to replace 3D models for all items (weapons, ammo bottles, powerups, etc.) you can pick "
		"up ingame with simple 2D icons. Weapons and ammo bottles can be distinguished because the ammo "
		"icons have a border. This should save system resources. Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.wallmarks.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.wallmarks.generic.name = "Marks on Walls:";
	s_preferences.wallmarks.generic.flags = QMF_SMALLFONT;
	s_preferences.wallmarks.generic.callback = UI_Preferences_Event;
	s_preferences.wallmarks.generic.id = ID_WALLMARKS;
	s_preferences.wallmarks.generic.x = XPOSITION;
	s_preferences.wallmarks.generic.y = y;
	s_preferences.wallmarks.generic.toolTip =
		"Disable to not display surface damage effects from weapon impacts in the game. This should save "
		"system resources. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.dynamiclights.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.dynamiclights.generic.name = "Dynamic Lights:";
	s_preferences.dynamiclights.generic.flags = QMF_SMALLFONT;
	s_preferences.dynamiclights.generic.callback = UI_Preferences_Event;
	s_preferences.dynamiclights.generic.id = ID_DYNAMICLIGHTS;
	s_preferences.dynamiclights.generic.x = XPOSITION;
	s_preferences.dynamiclights.generic.y = y;
	s_preferences.dynamiclights.generic.toolTip =
		"Disable to switch off the dynamic lighting in the game. This should save system resources. "
		"Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.flares.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.flares.generic.name = "Dynamic Flares:";
	s_preferences.flares.generic.flags = QMF_SMALLFONT;
	s_preferences.flares.generic.callback = UI_Preferences_Event;
	s_preferences.flares.generic.id = ID_FLARES;
	s_preferences.flares.generic.x = XPOSITION;
	s_preferences.flares.generic.y = y;
	s_preferences.flares.generic.toolTip =
		"Disable to switch off glowing lens flare effects that are added to light sources in the game. "
		"Those dynamic flares are nice to look at, but may distract some players. This also should save "
		"system resources. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.highqualitysky.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.highqualitysky.generic.name = "High Quality Sky:";
	s_preferences.highqualitysky.generic.flags = QMF_SMALLFONT;
	s_preferences.highqualitysky.generic.callback = UI_Preferences_Event;
	s_preferences.highqualitysky.generic.id = ID_HIGHQUALITYSKY;
	s_preferences.highqualitysky.generic.x = XPOSITION;
	s_preferences.highqualitysky.generic.y = y;
	s_preferences.highqualitysky.generic.toolTip =
		"Disable to remove the skybox textures in the game and replace them with a a plain monochrome "
		"surface. This should save system resources. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.lensFlare.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.lensFlare.generic.name = "Sky Lens Flare:";
	s_preferences.lensFlare.generic.flags = QMF_SMALLFONT;
	s_preferences.lensFlare.generic.callback = UI_Preferences_Event;
	s_preferences.lensFlare.generic.id = ID_LENSFLARE;
	s_preferences.lensFlare.generic.x = XPOSITION;
	s_preferences.lensFlare.generic.y = y;
	s_preferences.lensFlare.generic.toolTip =
		"Disable to remove the sky lens flare effect from the sky box in maps that support it. "
		"This should save system resources. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.ingamevideo.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.ingamevideo.generic.name = "Ingame Videos:";
	s_preferences.ingamevideo.generic.flags = QMF_SMALLFONT;
	s_preferences.ingamevideo.generic.callback = UI_Preferences_Event;
	s_preferences.ingamevideo.generic.id = ID_FLARES;
	s_preferences.ingamevideo.generic.x = XPOSITION;
	s_preferences.ingamevideo.generic.y = y;
	s_preferences.ingamevideo.generic.toolTip =
		"Disable to prevent the game engine from drawing video shaders in maps as used in PadCrash. "
		"This should save system resources. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.synceveryframe.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.synceveryframe.generic.name = "Sync Every Frame:";
	s_preferences.synceveryframe.generic.flags = QMF_SMALLFONT;
	s_preferences.synceveryframe.generic.callback = UI_Preferences_Event;
	s_preferences.synceveryframe.generic.id = ID_SYNCEVERYFRAME;
	s_preferences.synceveryframe.generic.x = XPOSITION;
	s_preferences.synceveryframe.generic.y = y;
	s_preferences.synceveryframe.generic.toolTip =
		"Enable to synchronize the game with hardware rendering. It can cause a moderate framerate loss, "
		"but may fix mouse or keyboard input lag issues. Default is off. NOTE: This option is not to be "
		"mistaken for vertical synchronization (display option) or synchronous clients (for recording demos).";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.forcemodel.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.forcemodel.generic.name = "Force Player Model:";
	s_preferences.forcemodel.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.forcemodel.generic.callback = UI_Preferences_Event;
	s_preferences.forcemodel.generic.id = ID_FORCEMODEL;
	s_preferences.forcemodel.generic.x = XPOSITION;
	s_preferences.forcemodel.generic.y = y;
	s_preferences.forcemodel.generic.toolTip = 
		"Enable to force all opponents to be displayed in the game with your selected character model. "
		"In team-based gametypes, this then only affects the opposing team. This also should save system "
		"resources. Default is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.glowmodel.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.glowmodel.generic.name = "Glowing Player Skins:";
	s_preferences.glowmodel.generic.flags = (QMF_SMALLFONT | QMF_HIDDEN);
	s_preferences.glowmodel.generic.callback = UI_Preferences_Event;
	s_preferences.glowmodel.generic.id = ID_GLOWMODEL;
	s_preferences.glowmodel.generic.x = XPOSITION;
	s_preferences.glowmodel.generic.y = y;
	s_preferences.glowmodel.generic.toolTip = 
		"Enable to force all players to be displayed with glowing player skins. This always refers to the "
		"default player skin and not a skin color you have selected in the player settings menu. Default "
		"is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.glowcolor.generic.type = MTYPE_SPINCONTROL;
	s_preferences.glowcolor.generic.name = "Glow Color:";
	s_preferences.glowcolor.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.glowcolor.generic.callback = UI_Preferences_Event;
	s_preferences.glowcolor.generic.id = ID_GLOWCOLOR;
	s_preferences.glowcolor.generic.x = XPOSITION;
	s_preferences.glowcolor.generic.y = y;
	s_preferences.glowcolor.itemnames = glowcolor_items;
	s_preferences.glowcolor.generic.toolTip = 
		"Select a desired glow skin color. NOTE: In team gametypes, the color is always set to red "
		"or blue depending on which team you join.";

	// chat options
	y = YPOSITION;
	s_preferences.connotify.generic.type = MTYPE_SPINCONTROL;
	s_preferences.connotify.generic.name = "Notification Time:";
	s_preferences.connotify.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.connotify.generic.callback = UI_Preferences_Event;
	s_preferences.connotify.generic.id = ID_CONNOTIFY;
	s_preferences.connotify.generic.x = XPOSITION;
	s_preferences.connotify.generic.y = y;
	s_preferences.connotify.itemnames = connotify_items;
	s_preferences.connotify.generic.toolTip = 
		"Select whether you prefer notifications to appear short (2s), default (4s), long (6s), "
		"or maximum (8s) at the top of the screen.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.chatheight.generic.type = MTYPE_SPINCONTROL;
	s_preferences.chatheight.generic.name = "Notification Space:";
	s_preferences.chatheight.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.chatheight.generic.callback = UI_Preferences_Event;
	s_preferences.chatheight.generic.id = ID_CHATHEIGHT;
	s_preferences.chatheight.generic.x = XPOSITION;
	s_preferences.chatheight.generic.y = y;
	s_preferences.chatheight.itemnames = chatheight_items;
	s_preferences.chatheight.generic.toolTip = 
		"Select whether you prefer notifications to appear default (up to 4 lines), expanded "
		"(up to 6 lines), or maximum (up to 8 lines) at the top of the screen.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.chaticon.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.chaticon.generic.name = "Player Icon:";
	s_preferences.chaticon.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.chaticon.generic.callback = UI_Preferences_Event;
	s_preferences.chaticon.generic.id = ID_CHATICON;
	s_preferences.chaticon.generic.x = XPOSITION;
	s_preferences.chaticon.generic.y = y;
	s_preferences.chaticon.generic.toolTip = 
		"Disable to hide the player icon at the beginning of a chat notification. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.chatbeep.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.chatbeep.generic.name = "Chat Beep:";
	s_preferences.chatbeep.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.chatbeep.generic.callback = UI_Preferences_Event;
	s_preferences.chatbeep.generic.id = ID_CHATBEEP;
	s_preferences.chatbeep.generic.x = XPOSITION;
	s_preferences.chatbeep.generic.y = y;
	s_preferences.chatbeep.generic.toolTip = 
		"Disable to switch off the beep of all chat notifications. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.gesture.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.gesture.generic.name = "Gesture:";
	s_preferences.gesture.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.gesture.generic.callback = UI_Preferences_Event;
	s_preferences.gesture.generic.id = ID_GESTURE;
	s_preferences.gesture.generic.x = XPOSITION;
	s_preferences.gesture.generic.y = y;
	s_preferences.gesture.generic.toolTip = 
		"Disable this to not display gestures of other players. Default is on.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.botchat.generic.type = MTYPE_SPINCONTROL;
	s_preferences.botchat.generic.name = "Chatting Bots:";
	s_preferences.botchat.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.botchat.generic.callback = UI_Preferences_Event;
	s_preferences.botchat.generic.id = ID_BOTCHAT;
	s_preferences.botchat.generic.x = XPOSITION;
	s_preferences.botchat.generic.y = y;
	s_preferences.botchat.itemnames = botchat_items;
	s_preferences.botchat.generic.toolTip = 
		"Set to off to prevent the bots from chatting and silence them, or set it to "
		"often to make the bots more talkative.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.teamchatsonly.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.teamchatsonly.generic.name = "Team Chats Only:";
	s_preferences.teamchatsonly.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.teamchatsonly.generic.callback = UI_Preferences_Event;
	s_preferences.teamchatsonly.generic.id = ID_TEAMCHATSONLY;
	s_preferences.teamchatsonly.generic.x = XPOSITION;
	s_preferences.teamchatsonly.generic.y = y;
	s_preferences.teamchatsonly.generic.toolTip = 
		"Enable to force only chat messages from your teammates to be displayed. Deault is off.";

	// help options
	y = YPOSITION;
	s_preferences.whIcons.generic.type = MTYPE_TEXT;
	s_preferences.whIcons.generic.flags = QMF_HIDDEN;
	s_preferences.whIcons.generic.x = XPOSITION - 20;
	s_preferences.whIcons.generic.y = y;
	s_preferences.whIcons.string = "Wallhack Icons";
	s_preferences.whIcons.style = (UI_CENTER | UI_SMALLFONT);
	s_preferences.whIcons.color = menu_text_color;

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.whTeamMates.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whTeamMates.generic.name = "Teammate:";
	s_preferences.whTeamMates.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whTeamMates.generic.callback = UI_Preferences_Event;
	s_preferences.whTeamMates.generic.id = ID_ICONTEAMMATE;
	s_preferences.whTeamMates.generic.x = XPOSITION;
	s_preferences.whTeamMates.generic.y = y;
	s_preferences.whTeamMates.generic.toolTip =
		"Enable to display a PAD logo over your teammates heads, visible through walls, "
		"to help you find them. Deault is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.whHStations.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whHStations.generic.name = "Health Station:";
	s_preferences.whHStations.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whHStations.generic.callback = UI_Preferences_Event;
	s_preferences.whHStations.generic.id = ID_ICONHSTATION;
	s_preferences.whHStations.generic.x = XPOSITION;
	s_preferences.whHStations.generic.y = y;
	s_preferences.whHStations.generic.toolTip =
		"Enable to display a heart icon over every health station, visible through walls, "
		"to help you find them. Deault is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.whSycTele.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whSycTele.generic.name = "Sprayroom Teleporter:";
	s_preferences.whSycTele.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whSycTele.generic.callback = UI_Preferences_Event;
	s_preferences.whSycTele.generic.id = ID_ICONSYCTELE;
	s_preferences.whSycTele.generic.x = XPOSITION;
	s_preferences.whSycTele.generic.y = y;
	s_preferences.whSycTele.generic.toolTip =
		"Enable to display an arrow icon over the sprayroom teleporter in Spray Your Color, "
		"visible through walls, to help you find it. Deault is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.whBalloons.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whBalloons.generic.name = "Balloon Box:";
	s_preferences.whBalloons.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whBalloons.generic.callback = UI_Preferences_Event;
	s_preferences.whBalloons.generic.id = ID_ICONBALLOON;
	s_preferences.whBalloons.generic.x = XPOSITION;
	s_preferences.whBalloons.generic.y = y;
	s_preferences.whBalloons.generic.toolTip =
		"Enable to display a balloon icon over balloon boxes in Big Balloon, "
		"visible through walls, to help you find them. Deault is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.whLPS.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whLPS.generic.name = "LPS Arrow:";
	s_preferences.whLPS.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whLPS.generic.callback = UI_Preferences_Event;
	s_preferences.whLPS.generic.id = ID_ICONLPSARROW;
	s_preferences.whLPS.generic.x = XPOSITION;
	s_preferences.whLPS.generic.y = y;
	s_preferences.whLPS.generic.toolTip =
		"Enable this to display an arrow icon over every player in Last Pad Standing, "
		"visible through walls, to help you find them. Deault is off.";

	y += (BIGCHAR_HEIGHT + 2);
	s_preferences.whFreezeTag.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whFreezeTag.generic.name = "Frozen Teammate:";
	s_preferences.whFreezeTag.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whFreezeTag.generic.callback = UI_Preferences_Event;
	s_preferences.whFreezeTag.generic.id = ID_ICONFREEZETAG;
	s_preferences.whFreezeTag.generic.x = XPOSITION;
	s_preferences.whFreezeTag.generic.y = y;
	s_preferences.whFreezeTag.generic.toolTip =
		"Enable this to display a freeze icon over frozen teammates in Freeze Tag, "
		"visible through walls, to help you find them. Deault is off.";

	s_preferences.back.generic.type = MTYPE_BITMAP;
	s_preferences.back.generic.name = BACK0;
	s_preferences.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_preferences.back.generic.x = 552;
	s_preferences.back.generic.y = 440;
	s_preferences.back.generic.id = ID_BACK;
	s_preferences.back.generic.callback = UI_Preferences_Event;
	s_preferences.back.width = 80;
	s_preferences.back.height = 40;
	s_preferences.back.focuspic = BACK1;
	s_preferences.back.focuspicinstead = qtrue;

	Menu_AddItem(&s_preferences.menu, &s_preferences.hud);
	Menu_AddItem(&s_preferences.menu, &s_preferences.game);
	Menu_AddItem(&s_preferences.menu, &s_preferences.chat);
	Menu_AddItem(&s_preferences.menu, &s_preferences.help);
	Menu_AddItem(&s_preferences.menu, &s_preferences.back);

	Menu_AddItem(&s_preferences.menu, &s_preferences.crosshair);
	Menu_AddItem(&s_preferences.menu, &s_preferences.indicatehealth);
	Menu_AddItem(&s_preferences.menu, &s_preferences.identifytarget);
	Menu_AddItem(&s_preferences.menu, &s_preferences.ffahudtheme);
	Menu_AddItem(&s_preferences.menu, &s_preferences.teamoverlay);
	Menu_AddItem(&s_preferences.menu, &s_preferences.lagometer);
	Menu_AddItem(&s_preferences.menu, &s_preferences.voipmeter);
	Menu_AddItem(&s_preferences.menu, &s_preferences.timer);
	Menu_AddItem(&s_preferences.menu, &s_preferences.timeleft);
	Menu_AddItem(&s_preferences.menu, &s_preferences.realtime);
	Menu_AddItem(&s_preferences.menu, &s_preferences.fps);
	Menu_AddItem(&s_preferences.menu, &s_preferences.ups);

	Menu_AddItem(&s_preferences.menu, &s_preferences.simpleitems);
	Menu_AddItem(&s_preferences.menu, &s_preferences.wallmarks);
	Menu_AddItem(&s_preferences.menu, &s_preferences.dynamiclights);
	Menu_AddItem(&s_preferences.menu, &s_preferences.flares);
	Menu_AddItem(&s_preferences.menu, &s_preferences.highqualitysky);
	Menu_AddItem(&s_preferences.menu, &s_preferences.lensFlare);
	Menu_AddItem(&s_preferences.menu, &s_preferences.ingamevideo);
	Menu_AddItem(&s_preferences.menu, &s_preferences.synceveryframe);
	Menu_AddItem(&s_preferences.menu, &s_preferences.forcemodel);
	Menu_AddItem(&s_preferences.menu, &s_preferences.glowmodel);
	Menu_AddItem(&s_preferences.menu, &s_preferences.glowcolor);

	Menu_AddItem(&s_preferences.menu, &s_preferences.connotify);
	Menu_AddItem(&s_preferences.menu, &s_preferences.chatheight);
	Menu_AddItem(&s_preferences.menu, &s_preferences.chaticon);
	Menu_AddItem(&s_preferences.menu, &s_preferences.chatbeep);
	Menu_AddItem(&s_preferences.menu, &s_preferences.gesture);
	Menu_AddItem(&s_preferences.menu, &s_preferences.botchat);
	Menu_AddItem(&s_preferences.menu, &s_preferences.teamchatsonly);

	Menu_AddItem(&s_preferences.menu, &s_preferences.whIcons);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whTeamMates);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whHStations);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whSycTele);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whBalloons);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whLPS);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whFreezeTag);

	UI_Preferences_SetMenuItems();

	// initial default section
	s_preferences.section = O_HUD;

	UI_Preferences_Update();
}

/*
===============
UI_Preferences_Cache
===============
*/
void UI_Preferences_Cache(void) {
	int n;

	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
	trap_R_RegisterShaderNoMip(HUD0);
	trap_R_RegisterShaderNoMip(HUD1);
	trap_R_RegisterShaderNoMip(GAME0);
	trap_R_RegisterShaderNoMip(GAME1);
	trap_R_RegisterShaderNoMip(CHAT0);
	trap_R_RegisterShaderNoMip(CHAT1);
	trap_R_RegisterShaderNoMip(HELP0);
	trap_R_RegisterShaderNoMip(HELP1);

	for (n = 0; n < NUM_CROSSHAIRS; n++) {
		s_preferences.crosshairShader[n] = trap_R_RegisterShaderNoMip(va("gfx/2d/crosshair%c", 'a' + n));
	}
}

/*
===============
UI_PreferencesMenu
===============
*/
void UI_PreferencesMenu(void) {
	UI_Preferences_MenuInit();
	UI_PushMenu(&s_preferences.menu);
}
