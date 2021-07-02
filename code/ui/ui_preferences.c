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

GAME OPTIONS MENU

=======================================================================
*/

#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define HUD0 "menu/buttons/hud0"
#define HUD1 "menu/buttons/hud1"
#define CHAT0 "menu/buttons/chat0"
#define CHAT1 "menu/buttons/chat1"
#define HELP0 "menu/buttons/help0"
#define HELP1 "menu/buttons/help1"

// options sections
#define O_HUD 0
#define O_CHAT 1
#define O_HELP 2
#define O_MAX 3

#define ID_BACK 10
#define ID_HUD 11
#define ID_CHAT 12
#define ID_HELP 13

#define ID_CROSSHAIR 20
#define ID_FFAHUD 21
#define ID_IDENTIFYTARGET 22
#define ID_DRAWTEAMOVERLAY 23
#define ID_TIMER 24
#define ID_TIMELEFT 25
#define ID_REALTIME 26
#define ID_UPS 27
#define ID_FPS 28

#define ID_FORCEMODEL 30
#define ID_GLOWMODEL 31
#define ID_GLOWCOLOR 32

#define ID_CONNOTIFY 40
#define ID_CHATHEIGHT 41
#define ID_DRAWCHATICON 42
#define ID_CHATBEEP 43
#define ID_BOTCHAT 44
#define ID_TEAMCHATSONLY 45

#define ID_WALLHACKTEAMMATES 50
#define ID_WALLHACKHSTATION 51
#define ID_WALLHACKSYCTELE 52
#define ID_WALLHACKBALLOONS 53
#define ID_WALLHACKLPS 54
#define ID_WALLHACKFREEZETAG 55

#define NUM_CROSSHAIRS 12

#define XPOSITION 534
#define YPOSITION 216

typedef struct {
	menuframework_s menu;

	menubitmap_s hud;
	menubitmap_s chat;
	menubitmap_s help;

	menulist_s crosshair;
	menulist_s ffahud;
	menuradiobutton_s identifytarget;
	menuradiobutton_s drawteamoverlay;
	menuradiobutton_s timer;
	menuradiobutton_s timeleft;
	menuradiobutton_s realtime;
	menuradiobutton_s ups;
	menuradiobutton_s fps;
	menuradiobutton_s forcemodel;
	menuradiobutton_s glowmodel;
	menulist_s glowcolor;

	menulist_s connotify;
	menulist_s chatheight;
	menuradiobutton_s drawchaticon;
	menuradiobutton_s chatbeep;
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
	(menucommon_s *)&s_preferences.ffahud,
	(menucommon_s *)&s_preferences.identifytarget,
	(menucommon_s *)&s_preferences.drawteamoverlay,
	(menucommon_s *)&s_preferences.timer,
	(menucommon_s *)&s_preferences.timeleft,
	(menucommon_s *)&s_preferences.realtime,
	(menucommon_s *)&s_preferences.ups,
	(menucommon_s *)&s_preferences.fps,
	(menucommon_s *)&s_preferences.forcemodel,
	(menucommon_s *)&s_preferences.glowmodel,
	(menucommon_s *)&s_preferences.glowcolor,
	NULL
};

static menucommon_s *g_chat_options[] = {
	(menucommon_s *)&s_preferences.connotify,
	(menucommon_s *)&s_preferences.chatheight,
	(menucommon_s *)&s_preferences.drawchaticon,
	(menucommon_s *)&s_preferences.chatbeep,
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
	g_chat_options,
	g_help_options,
};

static const char *ffahud_names[] = {"Black", "Red",	"Blue", "Green",	"Chrome", "Whitemetal",
									 "Rust",  "Flower", "Wood", "Airforce", NULL};

static const char *connotify_names[] = {"Short", "Default", "Long", "Maximum", NULL};

static const char *glowcolor_names[] = {S_COLOR_BLACK "Black",	   S_COLOR_RED "Red",	  S_COLOR_GREEN "Green",
										S_COLOR_YELLOW "Yellow",   S_COLOR_BLUE "Blue",	  S_COLOR_CYAN "Cyan",
										S_COLOR_MAGENTA "Magenta", S_COLOR_WHITE "White", NULL};

static const char *botchat_names[] = {"Off", "Default", "Often", NULL};

static const char *chatheight_names[] = {"Default", "Expanded", "Maximum", NULL};

static void UpdateGlowColorFlags(void) {
	if (s_preferences.glowcolor.generic.flags & QMF_HIDDEN)
		return;

	if (0 == s_preferences.glowmodel.curvalue) {
		s_preferences.glowcolor.generic.flags |= (QMF_GRAYED);
	} else {
		s_preferences.glowcolor.generic.flags &= ~(QMF_GRAYED);
	}
}

static void Preferences_SetMenuItems(void) {
	int notify;
	int chatheight;
	int cg_iconsCvarValue = (int)trap_Cvar_VariableValue("cg_icons");

	notify = UI_GetCvarInt("con_notifytime");
	if (notify < 0) {
		notify *= -1;
	}
	if (notify > 0 && notify <= 2)
		s_preferences.connotify.curvalue = 0;
	else if (notify > 2 && notify <= 4)
		s_preferences.connotify.curvalue = 1;
	else if (notify > 4 && notify <= 6)
		s_preferences.connotify.curvalue = 2;
	else if (notify > 6)
		s_preferences.connotify.curvalue = 3;

	s_preferences.drawchaticon.curvalue = trap_Cvar_VariableValue("cg_drawChatIcon") != 0;
	s_preferences.ffahud.curvalue = Com_Clamp(0, 9, trap_Cvar_VariableValue("cg_wopFFAhud"));
	s_preferences.timeleft.curvalue = Com_Clamp(0, 1, trap_Cvar_VariableValue("cg_drawTimeLeft"));
	s_preferences.chatbeep.curvalue = trap_Cvar_VariableValue("cg_chatBeep") != 0;
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

	chatheight = trap_Cvar_VariableValue("cg_chatHeight");
	if (chatheight > 6 && chatheight <= 8)
		s_preferences.chatheight.curvalue = 2;
	else if (chatheight > 4 && chatheight <= 6)
		s_preferences.chatheight.curvalue = 1;
	else
		s_preferences.chatheight.curvalue = 0;

	s_preferences.timer.curvalue = Com_Clamp(0, 1, trap_Cvar_VariableValue("cg_drawTimer"));
	s_preferences.fps.curvalue = Com_Clamp(0, 1, trap_Cvar_VariableValue("cg_drawFPS"));
	s_preferences.ups.curvalue = Com_Clamp(0, 1, trap_Cvar_VariableValue("cg_drawups"));
	s_preferences.realtime.curvalue = Com_Clamp(0, 1, trap_Cvar_VariableValue("cg_drawRealTime"));
	s_preferences.whLPS.curvalue = (ICON_ARROW & cg_iconsCvarValue);
	s_preferences.whBalloons.curvalue = (ICON_BALLOON & cg_iconsCvarValue);
	s_preferences.whTeamMates.curvalue = (ICON_TEAMMATE & cg_iconsCvarValue);
	s_preferences.whFreezeTag.curvalue = (ICON_FREEZETAG & cg_iconsCvarValue);
	s_preferences.whHStations.curvalue = (ICON_HEALTHSTATION & cg_iconsCvarValue);
	s_preferences.whSycTele.curvalue = (ICON_SPRAYROOM & cg_iconsCvarValue);

	s_preferences.crosshair.curvalue = (int)trap_Cvar_VariableValue("cg_drawCrosshair") % NUM_CROSSHAIRS;
	s_preferences.identifytarget.curvalue = trap_Cvar_VariableValue("cg_drawCrosshairNames") != 0;
	s_preferences.forcemodel.curvalue = (trap_Cvar_VariableValue("cg_forcemodel") != 0.0f);
	s_preferences.drawteamoverlay.curvalue = trap_Cvar_VariableValue("cg_drawTeamOverlay") != 0;
	s_preferences.glowcolor.curvalue = trap_Cvar_VariableValue("cg_glowModel"); // cg_glowModelTeam..
	s_preferences.glowmodel.curvalue = (s_preferences.glowcolor.curvalue != 0);
	UpdateGlowColorFlags();
}

/*
=================
Options_Update
=================
*/
static void Options_Update(void) {
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
	s_preferences.chat.generic.flags &= ~(QMF_GRAYED | QMF_HIGHLIGHT | QMF_HIGHLIGHT_IF_FOCUS);
	s_preferences.help.generic.flags &= ~(QMF_GRAYED | QMF_HIGHLIGHT | QMF_HIGHLIGHT_IF_FOCUS);

	s_preferences.hud.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.chat.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.help.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;

	// set buttons
	switch (s_preferences.section) {
	case O_HUD:
		s_preferences.hud.generic.flags |= QMF_HIGHLIGHT;
		break;

	case O_CHAT:
		s_preferences.chat.generic.flags |= QMF_HIGHLIGHT;
		break;

	case O_HELP:
		s_preferences.help.generic.flags |= QMF_HIGHLIGHT;
		break;
	}
}

/*
=================
Preferences_Event
=================
*/
static void Preferences_Event(void *ptr, int notification) {

	if (notification != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_HUD:
		s_preferences.section = O_HUD;
		Options_Update();
		break;

	case ID_CHAT:
		s_preferences.section = O_CHAT;
		Options_Update();
		break;

	case ID_HELP:
		s_preferences.section = O_HELP;
		Options_Update();
		break;

	case ID_CROSSHAIR:
		s_preferences.crosshair.curvalue++;
		if (s_preferences.crosshair.curvalue == NUM_CROSSHAIRS) {
			s_preferences.crosshair.curvalue = 0;
		}
		trap_Cvar_SetValue("cg_drawCrosshair", s_preferences.crosshair.curvalue);
		break;

	case ID_IDENTIFYTARGET:
		trap_Cvar_SetValue("cg_drawCrosshairNames", s_preferences.identifytarget.curvalue);
		break;

	case ID_FORCEMODEL:
		trap_Cvar_SetValue("cg_forcemodel", s_preferences.forcemodel.curvalue);
		break;

	case ID_CHATBEEP:
		if (s_preferences.chatbeep.curvalue == 0) {
			trap_Cvar_SetValue("cg_chatBeep", s_preferences.chatbeep.curvalue);
		} else {
			trap_Cvar_SetValue("cg_chatBeep", 7);
		}
		break;

	case ID_TEAMCHATSONLY:
		trap_Cvar_SetValue("cg_teamChatsOnly", s_preferences.teamchatsonly.curvalue);
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

	case ID_GLOWMODEL:
	case ID_GLOWCOLOR:
		if (s_preferences.glowmodel.curvalue == 0) {
			trap_Cvar_Set("cg_glowModel", "");
		} else {
			trap_Cvar_SetValue("cg_glowModel", s_preferences.glowcolor.curvalue); // default green enemies only
		}
		UpdateGlowColorFlags();
		break;

	case ID_DRAWTEAMOVERLAY:
		trap_Cvar_SetValue("cg_drawTeamOverlay", s_preferences.drawteamoverlay.curvalue);
		break;

	case ID_FFAHUD:
		trap_Cvar_SetValue("cg_wopFFAhud", s_preferences.ffahud.curvalue);
		break;

	case ID_TIMELEFT:
		trap_Cvar_SetValue("cg_drawTimeLeft", s_preferences.timeleft.curvalue);
		break;

	case ID_TIMER:
		trap_Cvar_SetValue("cg_drawTimer", s_preferences.timer.curvalue);
		break;

	case ID_FPS:
		trap_Cvar_SetValue("cg_drawFPS", s_preferences.fps.curvalue);
		break;

	case ID_UPS:
		trap_Cvar_SetValue("cg_drawUPS", s_preferences.ups.curvalue);
		break;

	case ID_REALTIME:
		trap_Cvar_SetValue("cg_drawRealTime", s_preferences.realtime.curvalue);
		break;

	case ID_WALLHACKLPS:
	case ID_WALLHACKTEAMMATES:
	case ID_WALLHACKBALLOONS:
	case ID_WALLHACKFREEZETAG:
	case ID_WALLHACKHSTATION:
	case ID_WALLHACKSYCTELE: {
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

	case ID_DRAWCHATICON:
		trap_Cvar_SetValue("cg_drawChatIcon", s_preferences.drawchaticon.curvalue);
		break;

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
		UI_DrawChar(x, y, 13, UI_CENTER | UI_BLINK | UI_SMALLFONT, color);
	}

	UI_DrawString(x - SMALLCHAR_WIDTH, y, s->generic.name, style | UI_RIGHT, color);
	if (!s->curvalue) {
		return;
	}
	UI_DrawHandlePic(x + SMALLCHAR_WIDTH, y - 4, 24, 24, s_preferences.crosshairShader[s->curvalue]);
}

/*
===============
Preferences_MenuInit
===============
*/
static void Preferences_MenuInit(void) {
	int y;

	memset(&s_preferences, 0, sizeof(preferences_t));

	Preferences_Cache();

	s_preferences.menu.wrapAround = qtrue;
	s_preferences.menu.fullscreen = qtrue;
	s_preferences.menu.bgparts = BGP_GAMEOPTIONS | BGP_SIMPLEBG;

	//menu
	s_preferences.hud.generic.type = MTYPE_BITMAP;
	s_preferences.hud.generic.name = HUD0;
	s_preferences.hud.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.hud.generic.callback = Preferences_Event;
	s_preferences.hud.generic.id = ID_HUD;
	s_preferences.hud.generic.x = 420;
	s_preferences.hud.generic.y = 40;
	s_preferences.hud.width = 80;
	s_preferences.hud.height = 40;
	s_preferences.hud.focuspic = HUD1;
	s_preferences.hud.focuspicinstead = qtrue;

	s_preferences.chat.generic.type = MTYPE_BITMAP;
	s_preferences.chat.generic.name = CHAT0;
	s_preferences.chat.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.chat.generic.callback = Preferences_Event;
	s_preferences.chat.generic.id = ID_CHAT;
	s_preferences.chat.generic.x = 520;
	s_preferences.chat.generic.y = 50;
	s_preferences.chat.width = 80;
	s_preferences.chat.height = 40;
	s_preferences.chat.focuspic = CHAT1;
	s_preferences.chat.focuspicinstead = qtrue;

	s_preferences.help.generic.type = MTYPE_BITMAP;
	s_preferences.help.generic.name = HELP0;
	s_preferences.help.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_preferences.help.generic.callback = Preferences_Event;
	s_preferences.help.generic.id = ID_HELP;
	s_preferences.help.generic.x = 450;
	s_preferences.help.generic.y = 85;
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
	s_preferences.crosshair.generic.callback = Preferences_Event;
	s_preferences.crosshair.generic.ownerdraw = Crosshair_Draw;
	s_preferences.crosshair.generic.id = ID_CROSSHAIR;
	s_preferences.crosshair.generic.top = y - 4;
	s_preferences.crosshair.generic.bottom = y + 20;
	s_preferences.crosshair.generic.left =
		XPOSITION - ((strlen(s_preferences.crosshair.generic.name) + 1) * SMALLCHAR_WIDTH);
	s_preferences.crosshair.generic.right = XPOSITION + 48;

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.ffahud.generic.type = MTYPE_SPINCONTROL;
	s_preferences.ffahud.generic.name = "FFA HUD Design:";
	s_preferences.ffahud.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.ffahud.generic.callback = Preferences_Event;
	s_preferences.ffahud.generic.id = ID_FFAHUD;
	s_preferences.ffahud.generic.x = XPOSITION;
	s_preferences.ffahud.generic.y = y;
	s_preferences.ffahud.itemnames = ffahud_names;

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.identifytarget.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.identifytarget.generic.name = "Identify Target:";
	s_preferences.identifytarget.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.identifytarget.generic.callback = Preferences_Event;
	s_preferences.identifytarget.generic.id = ID_IDENTIFYTARGET;
	s_preferences.identifytarget.generic.x = XPOSITION;
	s_preferences.identifytarget.generic.y = y;
	s_preferences.identifytarget.generic.toolTip =
		"Enable this to show the name of the player you actively have in your crosshair. Player name will disappear "
		"once out of your crosshair.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.drawteamoverlay.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.drawteamoverlay.generic.name = "Team Overlay:";
	s_preferences.drawteamoverlay.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.drawteamoverlay.generic.callback = Preferences_Event;
	s_preferences.drawteamoverlay.generic.id = ID_DRAWTEAMOVERLAY;
	s_preferences.drawteamoverlay.generic.x = XPOSITION;
	s_preferences.drawteamoverlay.generic.y = y;
	s_preferences.drawteamoverlay.generic.toolTip = "Enable this to see an overview "
													"of 4 of your team mates to the left on your HUD in a team game.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.timer.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.timer.generic.name = "Display Timer:";
	s_preferences.timer.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.timer.generic.callback = Preferences_Event;
	s_preferences.timer.generic.id = ID_TIMER;
	s_preferences.timer.generic.x = XPOSITION;
	s_preferences.timer.generic.y = y;

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.timeleft.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.timeleft.generic.name = "Display Time Left:";
	s_preferences.timeleft.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.timeleft.generic.callback = Preferences_Event;
	s_preferences.timeleft.generic.id = ID_TIMELEFT;
	s_preferences.timeleft.generic.x = XPOSITION;
	s_preferences.timeleft.generic.y = y;

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.realtime.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.realtime.generic.name = "Display Real Time:";
	s_preferences.realtime.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.realtime.generic.callback = Preferences_Event;
	s_preferences.realtime.generic.id = ID_REALTIME;
	s_preferences.realtime.generic.x = XPOSITION;
	s_preferences.realtime.generic.y = y;

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.ups.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.ups.generic.name = "Display Velocity:";
	s_preferences.ups.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.ups.generic.callback = Preferences_Event;
	s_preferences.ups.generic.id = ID_UPS;
	s_preferences.ups.generic.x = XPOSITION;
	s_preferences.ups.generic.y = y;

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.fps.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.fps.generic.name = "Display FPS:";
	s_preferences.fps.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.fps.generic.callback = Preferences_Event;
	s_preferences.fps.generic.id = ID_FPS;
	s_preferences.fps.generic.x = XPOSITION;
	s_preferences.fps.generic.y = y;

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.forcemodel.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.forcemodel.generic.name = "Force Player Model:";
	s_preferences.forcemodel.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.forcemodel.generic.callback = Preferences_Event;
	s_preferences.forcemodel.generic.id = ID_FORCEMODEL;
	s_preferences.forcemodel.generic.x = XPOSITION;
	s_preferences.forcemodel.generic.y = y;
	s_preferences.forcemodel.generic.toolTip = "Enable this to force seeing all opponents as your character in game.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.glowmodel.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.glowmodel.generic.name = "Glowing Player Model:";
	s_preferences.glowmodel.generic.flags = (QMF_SMALLFONT | QMF_HIDDEN);
	s_preferences.glowmodel.generic.callback = Preferences_Event;
	s_preferences.glowmodel.generic.id = ID_GLOWMODEL;
	s_preferences.glowmodel.generic.x = XPOSITION;
	s_preferences.glowmodel.generic.y = y;
	s_preferences.glowmodel.generic.toolTip = "Enable this to see glowing player models.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.glowcolor.generic.type = MTYPE_SPINCONTROL;
	s_preferences.glowcolor.generic.name = "Glow Color:";
	s_preferences.glowcolor.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.glowcolor.generic.callback = Preferences_Event;
	s_preferences.glowcolor.generic.id = ID_GLOWCOLOR;
	s_preferences.glowcolor.generic.x = XPOSITION;
	s_preferences.glowcolor.generic.y = y;
	s_preferences.glowcolor.itemnames = glowcolor_names;

	// chat options
	y = YPOSITION;
	s_preferences.connotify.generic.type = MTYPE_SPINCONTROL;
	s_preferences.connotify.generic.name = "Notification Time:";
	s_preferences.connotify.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.connotify.generic.callback = Preferences_Event;
	s_preferences.connotify.generic.id = ID_CONNOTIFY;
	s_preferences.connotify.generic.x = XPOSITION;
	s_preferences.connotify.generic.y = y;
	s_preferences.connotify.itemnames = connotify_names;
	s_preferences.connotify.generic.toolTip = "Select whether you prefer notifications to appear short (2s), default (4s), long (6s), or maximum (8s) at the top of the screen.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.chatheight.generic.type = MTYPE_SPINCONTROL;
	s_preferences.chatheight.generic.name = "Notification Space:";
	s_preferences.chatheight.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.chatheight.generic.callback = Preferences_Event;
	s_preferences.chatheight.generic.id = ID_CHATHEIGHT;
	s_preferences.chatheight.generic.x = XPOSITION;
	s_preferences.chatheight.generic.y = y;
	s_preferences.chatheight.itemnames = chatheight_names;
	s_preferences.chatheight.generic.toolTip = "Select whether you prefer notifications to appear default (up to 4 lines), expanded (up to 6 lines), or maximum (up to 8 lines) at the top of the screen.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.drawchaticon.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.drawchaticon.generic.name = "Draw Player Icon:";
	s_preferences.drawchaticon.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.drawchaticon.generic.callback = Preferences_Event;
	s_preferences.drawchaticon.generic.id = ID_DRAWCHATICON;
	s_preferences.drawchaticon.generic.x = XPOSITION;
	s_preferences.drawchaticon.generic.y = y;
	s_preferences.drawchaticon.generic.toolTip = "Disable this to remove the player icon at the beginning of a chat notification.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.chatbeep.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.chatbeep.generic.name = "Play Chat Beep:";
	s_preferences.chatbeep.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.chatbeep.generic.callback = Preferences_Event;
	s_preferences.chatbeep.generic.id = ID_CHATBEEP;
	s_preferences.chatbeep.generic.x = XPOSITION;
	s_preferences.chatbeep.generic.y = y;
	s_preferences.chatbeep.generic.toolTip = "Disable this to switch off the beep of all chat notifications.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.botchat.generic.type = MTYPE_SPINCONTROL;
	s_preferences.botchat.generic.name = "Chatting Bots:";
	s_preferences.botchat.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.botchat.generic.callback = Preferences_Event;
	s_preferences.botchat.generic.id = ID_BOTCHAT;
	s_preferences.botchat.generic.x = XPOSITION;
	s_preferences.botchat.generic.y = y;
	s_preferences.botchat.itemnames = botchat_names;
	s_preferences.botchat.generic.toolTip = "Set this to off to prevent the bots from chatting and silence them, or set it to often to make the bots more talkative.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.teamchatsonly.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.teamchatsonly.generic.name = "Team Chats Only:";
	s_preferences.teamchatsonly.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.teamchatsonly.generic.callback = Preferences_Event;
	s_preferences.teamchatsonly.generic.id = ID_TEAMCHATSONLY;
	s_preferences.teamchatsonly.generic.x = XPOSITION;
	s_preferences.teamchatsonly.generic.y = y;
	s_preferences.teamchatsonly.generic.toolTip = "Enable this to force only chat messages from your teammates to be displayed.";

	// help options
	y = YPOSITION;
	s_preferences.whIcons.generic.type = MTYPE_TEXT;
	s_preferences.whIcons.generic.flags = QMF_HIDDEN;
	s_preferences.whIcons.generic.x = XPOSITION - 20;
	s_preferences.whIcons.generic.y = y;
	s_preferences.whIcons.string = "Wallhack Icons";
	s_preferences.whIcons.style = (UI_CENTER | UI_SMALLFONT);
	s_preferences.whIcons.color = menu_text_color;

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.whTeamMates.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whTeamMates.generic.name = "Teammate:";
	s_preferences.whTeamMates.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whTeamMates.generic.callback = Preferences_Event;
	s_preferences.whTeamMates.generic.id = ID_WALLHACKTEAMMATES;
	s_preferences.whTeamMates.generic.x = XPOSITION;
	s_preferences.whTeamMates.generic.y = y;
	s_preferences.whTeamMates.generic.toolTip =
		"Show a PAD logo over your teammates heads, visible through walls, to help you find them.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.whHStations.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whHStations.generic.name = "Health Station:";
	s_preferences.whHStations.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whHStations.generic.callback = Preferences_Event;
	s_preferences.whHStations.generic.id = ID_WALLHACKHSTATION;
	s_preferences.whHStations.generic.x = XPOSITION;
	s_preferences.whHStations.generic.y = y;
	s_preferences.whHStations.generic.toolTip =
		"Show a heart icon over evey health station, visible through walls, to help you find them.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.whSycTele.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whSycTele.generic.name = "Sprayroom Teleporter:";
	s_preferences.whSycTele.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whSycTele.generic.callback = Preferences_Event;
	s_preferences.whSycTele.generic.id = ID_WALLHACKSYCTELE;
	s_preferences.whSycTele.generic.x = XPOSITION;
	s_preferences.whSycTele.generic.y = y;
	s_preferences.whSycTele.generic.toolTip =
		"Show an arrow icon over the sprayroom teleporter in Spray Your Color, visible through walls, to help you find it.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.whBalloons.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whBalloons.generic.name = "Balloon Box:";
	s_preferences.whBalloons.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whBalloons.generic.callback = Preferences_Event;
	s_preferences.whBalloons.generic.id = ID_WALLHACKBALLOONS;
	s_preferences.whBalloons.generic.x = XPOSITION;
	s_preferences.whBalloons.generic.y = y;
	s_preferences.whBalloons.generic.toolTip =
		"Show an balloon icon over balloon boxes in Big Ballon, visible through walls, to help you find them.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.whLPS.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whLPS.generic.name = "LPS Arrow:";
	s_preferences.whLPS.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whLPS.generic.callback = Preferences_Event;
	s_preferences.whLPS.generic.id = ID_WALLHACKLPS;
	s_preferences.whLPS.generic.x = XPOSITION;
	s_preferences.whLPS.generic.y = y;
	s_preferences.whLPS.generic.toolTip =
		"Show an arrow icon over every player in Last Pad Standing, visible through walls, to help you find them.";

	y += BIGCHAR_HEIGHT + 2;
	s_preferences.whFreezeTag.generic.type = MTYPE_RADIOBUTTON;
	s_preferences.whFreezeTag.generic.name = "Frozen Teammate:";
	s_preferences.whFreezeTag.generic.flags = QMF_SMALLFONT | QMF_HIDDEN;
	s_preferences.whFreezeTag.generic.callback = Preferences_Event;
	s_preferences.whFreezeTag.generic.id = ID_WALLHACKFREEZETAG;
	s_preferences.whFreezeTag.generic.x = XPOSITION;
	s_preferences.whFreezeTag.generic.y = y;
	s_preferences.whFreezeTag.generic.toolTip =
		"Show a freeze icon over frozen teammates in Freeze Tag, visible through walls, to help you find them.";

	s_preferences.back.generic.type = MTYPE_BITMAP;
	s_preferences.back.generic.name = BACK0;
	s_preferences.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_preferences.back.generic.x = 552;
	s_preferences.back.generic.y = 440;
	s_preferences.back.generic.id = ID_BACK;
	s_preferences.back.generic.callback = Preferences_Event;
	s_preferences.back.width = 80;
	s_preferences.back.height = 40;
	s_preferences.back.focuspic = BACK1;
	s_preferences.back.focuspicinstead = qtrue;

	// menu
	Menu_AddItem(&s_preferences.menu, &s_preferences.back);
	Menu_AddItem(&s_preferences.menu, &s_preferences.hud);
	Menu_AddItem(&s_preferences.menu, &s_preferences.chat);
	Menu_AddItem(&s_preferences.menu, &s_preferences.help);

	// hud options
	Menu_AddItem(&s_preferences.menu, &s_preferences.crosshair);
	Menu_AddItem(&s_preferences.menu, &s_preferences.ffahud);
	Menu_AddItem(&s_preferences.menu, &s_preferences.identifytarget);
	Menu_AddItem(&s_preferences.menu, &s_preferences.drawteamoverlay);
	Menu_AddItem(&s_preferences.menu, &s_preferences.timer);
	Menu_AddItem(&s_preferences.menu, &s_preferences.timeleft);
	Menu_AddItem(&s_preferences.menu, &s_preferences.realtime);
	Menu_AddItem(&s_preferences.menu, &s_preferences.ups);
	Menu_AddItem(&s_preferences.menu, &s_preferences.fps);
	Menu_AddItem(&s_preferences.menu, &s_preferences.forcemodel);
	Menu_AddItem(&s_preferences.menu, &s_preferences.glowmodel);
	Menu_AddItem(&s_preferences.menu, &s_preferences.glowcolor);

	// chat options
	Menu_AddItem(&s_preferences.menu, &s_preferences.connotify);
	Menu_AddItem(&s_preferences.menu, &s_preferences.chatheight);
	Menu_AddItem(&s_preferences.menu, &s_preferences.drawchaticon);
	Menu_AddItem(&s_preferences.menu, &s_preferences.chatbeep);
	Menu_AddItem(&s_preferences.menu, &s_preferences.botchat);
	Menu_AddItem(&s_preferences.menu, &s_preferences.teamchatsonly);

	// help options
	Menu_AddItem(&s_preferences.menu, &s_preferences.whIcons);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whTeamMates);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whHStations);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whSycTele);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whBalloons);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whLPS);
	Menu_AddItem(&s_preferences.menu, &s_preferences.whFreezeTag);

	Preferences_SetMenuItems();

	// initial default section
	s_preferences.section = O_HUD;

	Options_Update();
}

/*
===============
Preferences_Cache
===============
*/
void Preferences_Cache(void) {
	int n;

	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
	trap_R_RegisterShaderNoMip(HUD0);
	trap_R_RegisterShaderNoMip(HUD1);
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
	Preferences_MenuInit();
	UI_PushMenu(&s_preferences.menu);
}
