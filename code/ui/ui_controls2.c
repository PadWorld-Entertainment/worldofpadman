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

CONTROLS MENU

=======================================================================
*/

#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define LOOK0 "menu/buttons/look0"
#define LOOK1 "menu/buttons/look1"
#define MOVEMENT0 "menu/buttons/move0"
#define MOVEMENT1 "menu/buttons/move1"
#define WEAPONS0 "menu/buttons/shoot0"
#define WEAPONS1 "menu/buttons/shoot1"
#define MISC0 "menu/buttons/misc0"
#define MISC1 "menu/buttons/misc1"

typedef struct {
	const char *command;
	const char *label;
	int id;
	int anim;
	int defaultbind1;
	int defaultbind2;
	int bind1;
	int bind2;
} bind_t;

typedef struct {
	const char *name;
	float defaultvalue;
	float value;
} configcvar_t;

#define SAVE_NOOP 0
#define SAVE_YES 1
#define SAVE_NO 2
#define SAVE_CANCEL 3

// control sections
#define C_MOVEMENT 0
#define C_LOOKING 1
#define C_WEAPONS 2
#define C_MISC 3
#define C_MAX 4

#define ID_MOVEMENT 100
#define ID_LOOKING 101
#define ID_WEAPONS 102
#define ID_MISC 103
#define ID_DEFAULTS 104
#define ID_BACK 105
#define ID_SAVEANDEXIT 106
#define ID_EXIT 107

// bindable actions

#define ID_SPEED 0
#define ID_FORWARD 1
#define ID_BACKPEDAL 2
#define ID_MOVELEFT 3
#define ID_MOVERIGHT 4
#define ID_MOVEUP 5
#define ID_MOVEDOWN 6
#define ID_LEFT 7
#define ID_RIGHT 8
#define ID_STRAFE 9
#define ID_GESTURE 10
#define ID_USEITEM 11
#define ID_DROPCART 12
#define ID_LOOKUP 13
#define ID_LOOKDOWN 14
#define ID_MOUSELOOK 15
#define ID_CENTERVIEW 16
#define ID_3RDPERSON 17
#define ID_ATTACK 18
#define ID_ZOOM 19
#define ID_WEAPNEXT 20
#define ID_WEAPPREV 21
#define ID_WEAPON1 22
#define ID_WEAPON2 23
#define ID_WEAPON3 24
#define ID_WEAPON4 25
#define ID_WEAPON5 26
#define ID_WEAPON6 27
#define ID_WEAPON7 28
#define ID_WEAPON8 29
#define ID_WEAPON9 30
#define ID_SHOWSCORES 31
#define ID_TOGGLEMENU 32
#define ID_MUSIC 33
#define ID_HELP_GAMETYPE 34
#define ID_HELP_ITEMS 35
#define ID_SERVERINFO 36
#define ID_CHAT 37
#define ID_CHAT2 38
#define ID_CHAT3 39
#define ID_CHAT4 40
#define ID_PUSH2TALK 41
#define ID_NEXTSONG 42
#define ID_VOTEYES 43
#define ID_VOTENO 44

// all others
#define ID_FREELOOK 46
#define ID_INVERTMOUSE 47
#define ID_ALWAYSRUN 48
#define ID_AUTOSWITCH 49
#define ID_MOUSESPEED 50
#define ID_JOYENABLE 51
#define ID_JOYTHRESHOLD 52
#define ID_SMOOTHMOUSE 53

#define ANIM_IDLE 0
#define ANIM_RUN 1
#define ANIM_WALK 2
#define ANIM_BACK 3
#define ANIM_JUMP 4
#define ANIM_CROUCH 5
#define ANIM_STEPLEFT 6
#define ANIM_STEPRIGHT 7
#define ANIM_TURNLEFT 8
#define ANIM_TURNRIGHT 9
#define ANIM_LOOKUP 10
#define ANIM_LOOKDOWN 11
#define ANIM_WEAPON1 12
#define ANIM_WEAPON2 13
#define ANIM_WEAPON3 14
#define ANIM_WEAPON4 15
#define ANIM_WEAPON5 16
#define ANIM_WEAPON6 17
#define ANIM_WEAPON7 18
#define ANIM_WEAPON8 19
#define ANIM_WEAPON9 20
#define ANIM_WEAPON10 21
#define ANIM_ATTACK 22
#define ANIM_GESTURE 23
#define ANIM_DIE 24
#define ANIM_CHAT 25

typedef struct {
	menuframework_s menu;

	menubitmap_s movement;
	menubitmap_s looking;
	menubitmap_s weapons;
	menubitmap_s misc;

	menuradiobutton_s alwaysrun;
	menuaction_s run;
	menuaction_s walkforward;
	menuaction_s backpedal;
	menuaction_s stepleft;
	menuaction_s stepright;
	menuaction_s moveup;
	menuaction_s movedown;
	menuaction_s turnleft;
	menuaction_s turnright;
	menuaction_s sidestep;
	menuaction_s gesture;
	menuaction_s useitem;
	menuaction_s dropCart;

	menuslider_s sensitivity;
	menuradiobutton_s smoothmouse;
	menuradiobutton_s invertmouse;
	menuaction_s lookup;
	menuaction_s lookdown;
	menuaction_s mouselook;
	menuradiobutton_s freelook;
	menuaction_s centerview;
	menuaction_s thirdPerson;
	menuradiobutton_s joyenable;
	menuslider_s joythreshold;

	menuaction_s attack;
	menuaction_s zoom;
	menuaction_s nextweapon;
	menuaction_s prevweapon;
	menuradiobutton_s autoswitch;
	menuaction_s chainsaw;
	menuaction_s machinegun;
	menuaction_s shotgun;
	menuaction_s grenadelauncher;
	menuaction_s rocketlauncher;
	menuaction_s lightning;
	menuaction_s railgun;
	menuaction_s plasma;
	menuaction_s bfg;

	menuaction_s showscores;
	menuaction_s togglemenu;
	menuaction_s music;
	menuaction_s helpGametype;
	menuaction_s helpItems;
	menuaction_s ServerInfo;
	menuaction_s chat;
	menuaction_s chat2;
	menuaction_s chat3;
	menuaction_s chat4;
	menuaction_s pushToTalk;
	menuaction_s nextSong;
	menuaction_s voteyes;
	menuaction_s voteno;

	playerInfo_t playerinfo;
	qboolean changesmade;

	int section;
	qboolean waitingforkey;
	char playerModel[MAX_QPATH];
	vec3_t playerViewangles;
	vec3_t playerMoveangles;
	int playerLegs;
	int playerTorso;
	int playerWeapon;
	qboolean playerChat;

	menubitmap_s back;
	menutext_s name;
} controls_t;

static controls_t s_controls;

static vec4_t controls_binding_color = {1.00f, 1.00f, 0.00f, 1.00f};

static bind_t g_bindings[] = {
	{"+speed", "Run/Walk", ID_SPEED, ANIM_RUN, K_SHIFT, -1, -1, -1},
	{"+forward", "Walk Forward", ID_FORWARD, ANIM_WALK, K_UPARROW, 'w', -1, -1},
	{"+back", "Backpedal", ID_BACKPEDAL, ANIM_BACK, K_DOWNARROW, 's', -1, -1},
	{"+moveleft", "Step Left", ID_MOVELEFT, ANIM_STEPLEFT, ',', 'a', -1, -1},
	{"+moveright", "Step Right", ID_MOVERIGHT, ANIM_STEPRIGHT, '.', 'd', -1, -1},
	{"+moveup", "Up/Jump", ID_MOVEUP, ANIM_JUMP, K_SPACE, -1, -1, -1},
	{"+movedown", "Down/Crouch", ID_MOVEDOWN, ANIM_CROUCH, 'c', -1, -1, -1},
	{"+left", "Turn Left", ID_LEFT, ANIM_TURNLEFT, K_LEFTARROW, -1, -1, -1},
	{"+right", "Turn Right", ID_RIGHT, ANIM_TURNRIGHT, K_RIGHTARROW, -1, -1, -1},
	{"+strafe", "Sidestep/Turn", ID_STRAFE, ANIM_IDLE, K_ALT, -1, -1, -1},
	{"+button3", "Gesture", ID_GESTURE, ANIM_GESTURE, K_MOUSE3, 'q', -1, -1},
	{"+button2", "Use Item", ID_USEITEM, ANIM_IDLE, K_ENTER, 'e', -1, -1},
	{"dropCartridge", "Drop Item", ID_DROPCART, ANIM_IDLE, K_BACKSPACE, 'f', -1, -1},

	{"+lookup", "Look Up", ID_LOOKUP, ANIM_LOOKUP, K_PGDN, -1, -1, -1},
	{"+lookdown", "Look Down", ID_LOOKDOWN, ANIM_LOOKDOWN, K_DEL, -1, -1, -1},
	{"+mlook", "Mouse Look", ID_MOUSELOOK, ANIM_IDLE, '/', -1, -1, -1},
	{"centerview", "Center View", ID_CENTERVIEW, ANIM_IDLE, K_END, -1, -1, -1},
	{"toggle cg_thirdPerson", "3rd Person View", ID_3RDPERSON, ANIM_IDLE, 'p', -1, -1, -1},

	{"+attack", "Attack", ID_ATTACK, ANIM_ATTACK, K_MOUSE1, K_CTRL, -1, -1},
	{"+zoom", "Scope", ID_ZOOM, ANIM_IDLE, K_MOUSE2, 'x', -1, -1},
	{"weapnext", "Next Weapon", ID_WEAPNEXT, ANIM_IDLE, K_MWHEELDOWN, ']', -1, -1},
	{"weapprev", "Previous Weapon", ID_WEAPPREV, ANIM_IDLE, K_MWHEELUP, '[', -1, -1},
	{"weapon 1", "PUNCHY", ID_WEAPON1, ANIM_WEAPON1, '1', -1, -1, -1},
	{"weapon 2", "NiPPER", ID_WEAPON2, ANIM_WEAPON2, '2', -1, -1, -1},
	{"weapon 3", "PUMPER", ID_WEAPON3, ANIM_WEAPON3, '3', -1, -1, -1},
	{"weapon 4", "BALLOONY", ID_WEAPON4, ANIM_WEAPON4, '4', -1, -1, -1},
	{"weapon 5", "BETTY", ID_WEAPON5, ANIM_WEAPON5, '5', -1, -1, -1},
	{"weapon 6", "BOASTER", ID_WEAPON6, ANIM_WEAPON6, '6', -1, -1, -1},
	{"weapon 7", "SPLASHER", ID_WEAPON7, ANIM_WEAPON7, '7', -1, -1, -1},
	{"weapon 8", "BUBBLE G.", ID_WEAPON8, ANIM_WEAPON8, '8', -1, -1, -1},
	{"weapon 9", "IMPERiUS", ID_WEAPON9, ANIM_WEAPON9, '9', -1, -1, -1},

	{"+scores", "Scoreboard", ID_SHOWSCORES, ANIM_IDLE, K_TAB, -1, -1, -1},
	{"togglemenu", "Ingame Menu", ID_TOGGLEMENU, ANIM_IDLE, K_ESCAPE, -1, -1, -1},
	{"wop_music", "Music Menu", ID_MUSIC, ANIM_IDLE, 'm', -1, -1, -1},
	{"ui_help gametype", "Help Gametype", ID_HELP_GAMETYPE, ANIM_IDLE, 'g', -1, -1, -1},
	{"ui_help item", "Help Weapons/Items", ID_HELP_ITEMS, ANIM_IDLE, 'h', -1, -1, -1},
	{"toggle cg_drawServerInfos", "Server Info", ID_SERVERINFO, ANIM_IDLE, 'k', -1, -1, -1},
	{"messagemode", "Chat All", ID_CHAT, ANIM_CHAT, 't', -1, -1, -1},
	{"messagemode2", "Chat Team", ID_CHAT2, ANIM_CHAT, 'y', 'z', -1, -1},
	{"messagemode3", "Chat Target", ID_CHAT3, ANIM_CHAT, 'u', -1, -1, -1},
	{"messagemode4", "Chat Attacker", ID_CHAT4, ANIM_CHAT, 'i', -1, -1, -1},
	{"+voiprecord", "Push to Talk", ID_PUSH2TALK, ANIM_IDLE, 'r', -1, -1, -1},
	{"wop_nextsong", "Skip to Next Song", ID_NEXTSONG, ANIM_IDLE, 'n', -1, -1, -1},
	{"vote yes", "Vote Yes", ID_VOTEYES, ANIM_IDLE, K_F1, K_KP_PLUS, -1, -1},
	{"vote no", "Vote No", ID_VOTENO, ANIM_IDLE, K_F2, K_KP_MINUS, -1, -1},

	{(char *)NULL, (char *)NULL, 0, 0, -1, -1, -1, -1},
};

static configcvar_t g_configcvars[] =
{
	{"cl_run", 0, 0},
	{"m_pitch",	0, 0},
	{"cg_autoswitch", 0, 0},
	{"sensitivity", 0, 0},
	{"in_joystick", 0, 0},
	{"joy_threshold", 0, 0},
	{"m_filter", 0,	0},
	{"cl_freelook",	0, 0},
	{NULL, 0, 0}
};

static menucommon_s *g_movement_controls[] = {
	(menucommon_s *)&s_controls.alwaysrun,
	(menucommon_s *)&s_controls.run,
	(menucommon_s *)&s_controls.walkforward,
	(menucommon_s *)&s_controls.backpedal,
	(menucommon_s *)&s_controls.stepleft,
	(menucommon_s *)&s_controls.stepright,
	(menucommon_s *)&s_controls.moveup,
	(menucommon_s *)&s_controls.movedown,
	(menucommon_s *)&s_controls.turnleft,
	(menucommon_s *)&s_controls.turnright,
	(menucommon_s *)&s_controls.sidestep,
	(menucommon_s *)&s_controls.gesture,
	(menucommon_s *)&s_controls.useitem,
	(menucommon_s *)&s_controls.dropCart,
	NULL
};

static menucommon_s *g_weapons_controls[] = {
	(menucommon_s *)&s_controls.attack,
	(menucommon_s *)&s_controls.zoom,
	(menucommon_s *)&s_controls.nextweapon,
	(menucommon_s *)&s_controls.prevweapon,
	(menucommon_s *)&s_controls.autoswitch,
	(menucommon_s *)&s_controls.chainsaw,
	(menucommon_s *)&s_controls.machinegun,
	(menucommon_s *)&s_controls.shotgun,
	(menucommon_s *)&s_controls.grenadelauncher,
	(menucommon_s *)&s_controls.rocketlauncher,
	(menucommon_s *)&s_controls.lightning,
	(menucommon_s *)&s_controls.railgun,
	(menucommon_s *)&s_controls.plasma,
	(menucommon_s *)&s_controls.bfg,
	NULL,
};

static menucommon_s *g_looking_controls[] = {
	(menucommon_s *)&s_controls.sensitivity,
	(menucommon_s *)&s_controls.smoothmouse,
	(menucommon_s *)&s_controls.invertmouse,
	(menucommon_s *)&s_controls.lookup,
	(menucommon_s *)&s_controls.lookdown,
	(menucommon_s *)&s_controls.mouselook,
	(menucommon_s *)&s_controls.freelook,
	(menucommon_s *)&s_controls.centerview,
	(menucommon_s *)&s_controls.thirdPerson,
	(menucommon_s *)&s_controls.joyenable,
	(menucommon_s *)&s_controls.joythreshold,
	NULL,
};

static menucommon_s *g_misc_controls[] = {
	(menucommon_s *)&s_controls.showscores,
	(menucommon_s *)&s_controls.togglemenu,
	(menucommon_s *)&s_controls.music,
	(menucommon_s *)&s_controls.helpGametype,
	(menucommon_s *)&s_controls.helpItems,
	(menucommon_s *)&s_controls.ServerInfo,
	(menucommon_s *)&s_controls.chat,
	(menucommon_s *)&s_controls.chat2,
	(menucommon_s *)&s_controls.chat3,
	(menucommon_s *)&s_controls.chat4,
	(menucommon_s *)&s_controls.pushToTalk,
	(menucommon_s *)&s_controls.nextSong,
	(menucommon_s *)&s_controls.voteyes,
	(menucommon_s *)&s_controls.voteno,
	NULL,
};

static menucommon_s **g_controls[] = {
	g_movement_controls,
	g_looking_controls,
	g_weapons_controls,
	g_misc_controls,
};

/*
=================
Controls_InitCvars
=================
*/
static void Controls_InitCvars(void) {
	int i;
	configcvar_t *cvarptr;

	cvarptr = g_configcvars;
	for (i = 0;; i++, cvarptr++) {
		if (!cvarptr->name)
			break;

		// get current value
		cvarptr->value = trap_Cvar_VariableValue(cvarptr->name);

		// get default value
		trap_Cvar_Reset(cvarptr->name);
		cvarptr->defaultvalue = trap_Cvar_VariableValue(cvarptr->name);

		// restore current value
		trap_Cvar_SetValue(cvarptr->name, cvarptr->value);
	}
}

/*
=================
Controls_GetCvarDefault
=================
*/
static float Controls_GetCvarDefault(char *name) {
	configcvar_t *cvarptr;
	int i;

	cvarptr = g_configcvars;
	for (i = 0;; i++, cvarptr++) {
		if (!cvarptr->name)
			return (0);

		if (!strcmp(cvarptr->name, name))
			break;
	}

	return cvarptr->defaultvalue;
}

/*
=================
Controls_GetCvarValue
=================
*/
static float Controls_GetCvarValue(char *name) {
	configcvar_t *cvarptr;
	int i;

	cvarptr = g_configcvars;
	for (i = 0;; i++, cvarptr++) {
		if (!cvarptr->name)
			return (0);

		if (!strcmp(cvarptr->name, name))
			break;
	}

	return cvarptr->value;
}

/*
=================
Controls_UpdateModel
=================
*/
static void Controls_UpdateModel(int anim) {
	VectorClear(s_controls.playerViewangles);
	VectorClear(s_controls.playerMoveangles);
	s_controls.playerViewangles[YAW] = 180 - 30;
	s_controls.playerMoveangles[YAW] = s_controls.playerViewangles[YAW];
	s_controls.playerLegs = LEGS_IDLE;
	s_controls.playerTorso = TORSO_STAND;
	s_controls.playerWeapon = -1;
	s_controls.playerChat = qfalse;

	switch (anim) {
	case ANIM_RUN:
		s_controls.playerLegs = LEGS_RUN;
		break;

	case ANIM_WALK:
		s_controls.playerLegs = LEGS_WALK;
		break;

	case ANIM_BACK:
		s_controls.playerLegs = LEGS_BACK;
		break;

	case ANIM_JUMP:
		s_controls.playerLegs = LEGS_JUMP;
		break;

	case ANIM_CROUCH:
		s_controls.playerLegs = LEGS_IDLECR;
		break;

	case ANIM_TURNLEFT:
		s_controls.playerViewangles[YAW] += 90;
		break;

	case ANIM_TURNRIGHT:
		s_controls.playerViewangles[YAW] -= 90;
		break;

	case ANIM_STEPLEFT:
		s_controls.playerLegs = LEGS_WALK;
		s_controls.playerMoveangles[YAW] = s_controls.playerViewangles[YAW] + 90;
		break;

	case ANIM_STEPRIGHT:
		s_controls.playerLegs = LEGS_WALK;
		s_controls.playerMoveangles[YAW] = s_controls.playerViewangles[YAW] - 90;
		break;

	case ANIM_LOOKUP:
		s_controls.playerViewangles[PITCH] = -45;
		break;

	case ANIM_LOOKDOWN:
		s_controls.playerViewangles[PITCH] = 45;
		break;

	case ANIM_WEAPON1:
		s_controls.playerWeapon = WP_PUNCHY;
		break;

	case ANIM_WEAPON2:
		s_controls.playerWeapon = WP_NIPPER;
		break;

	case ANIM_WEAPON3:
		s_controls.playerWeapon = WP_PUMPER;
		break;

	case ANIM_WEAPON4:
		s_controls.playerWeapon = WP_BALLOONY;
		break;

	case ANIM_WEAPON5:
		s_controls.playerWeapon = WP_BETTY;
		break;

	case ANIM_WEAPON6:
		s_controls.playerWeapon = WP_BOASTER;
		break;

	case ANIM_WEAPON7:
		s_controls.playerWeapon = WP_SPLASHER;
		break;

	case ANIM_WEAPON8:
		s_controls.playerWeapon = WP_BUBBLEG;
		break;

	case ANIM_WEAPON9:
		s_controls.playerWeapon = WP_IMPERIUS;
		break;

	case ANIM_WEAPON10:
		s_controls.playerWeapon = WP_GRAPPLING_HOOK;
		break;

	case ANIM_ATTACK:
		s_controls.playerTorso = TORSO_ATTACK;
		break;

	case ANIM_GESTURE:
		s_controls.playerTorso = TORSO_GESTURE;
		break;

	case ANIM_DIE:
		s_controls.playerLegs = BOTH_DEATH1;
		s_controls.playerTorso = BOTH_DEATH1;
		s_controls.playerWeapon = WP_NONE;
		break;

	case ANIM_CHAT:
		s_controls.playerChat = qtrue;
		break;

	default:
		break;
	}

	UI_PlayerInfo_SetInfo(&s_controls.playerinfo, s_controls.playerLegs, s_controls.playerTorso,
						  s_controls.playerViewangles, s_controls.playerMoveangles, s_controls.playerWeapon,
						  s_controls.playerChat);
}

/*
=================
Controls_Update
=================
*/
static void Controls_Update(void) {
	int i;
	int j;
	int y;
	menucommon_s **controls;
	menucommon_s *control;

	// disable all controls in all groups
	for (i = 0; i < C_MAX; i++) {
		controls = g_controls[i];
		// bk001204 - parentheses
		for (j = 0; (control = controls[j]); j++) {
			control->flags |= (QMF_HIDDEN | QMF_INACTIVE);
		}
	}

	controls = g_controls[s_controls.section];

	// enable controls in active group (and count number of items for vertical centering)
	// bk001204 - parentheses
	for (j = 0; (control = controls[j]); j++) {
		control->flags &= ~(QMF_GRAYED | QMF_HIDDEN | QMF_INACTIVE);
	}

	// position controls
	y = 180; // 160;
	// bk001204 - parentheses
	for (j = 0; (control = controls[j]); j++, y += SMALLCHAR_HEIGHT) {
		control->x = 472;
		control->y = y;
		control->left = 472 - 19 * SMALLCHAR_WIDTH;
		control->right = 472 + 21 * SMALLCHAR_WIDTH;
		control->top = y;
		control->bottom = y + SMALLCHAR_HEIGHT;
	}

	if (s_controls.waitingforkey) {
		// disable everybody
		for (i = 0; i < s_controls.menu.nitems; i++) {
			((menucommon_s *)(s_controls.menu.items[i]))->flags |= QMF_GRAYED;
		}

		// enable action item
		((menucommon_s *)(s_controls.menu.items[s_controls.menu.cursor]))->flags &= ~QMF_GRAYED;

		// don't gray out player's name
		s_controls.name.generic.flags &= ~QMF_GRAYED;

		return;
	}

	// enable everybody
	for (i = 0; i < s_controls.menu.nitems; i++) {
		((menucommon_s *)(s_controls.menu.items[i]))->flags &= ~QMF_GRAYED;
	}

	// makes sure flags are right on the group selection controls
	s_controls.looking.generic.flags &= ~(QMF_GRAYED | QMF_HIGHLIGHT | QMF_HIGHLIGHT_IF_FOCUS);
	s_controls.movement.generic.flags &= ~(QMF_GRAYED | QMF_HIGHLIGHT | QMF_HIGHLIGHT_IF_FOCUS);
	s_controls.weapons.generic.flags &= ~(QMF_GRAYED | QMF_HIGHLIGHT | QMF_HIGHLIGHT_IF_FOCUS);
	s_controls.misc.generic.flags &= ~(QMF_GRAYED | QMF_HIGHLIGHT | QMF_HIGHLIGHT_IF_FOCUS);

	s_controls.looking.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.movement.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.weapons.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.misc.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;

	// set buttons
	switch (s_controls.section) {
	case C_MOVEMENT:
		s_controls.movement.generic.flags |= QMF_HIGHLIGHT;
		break;

	case C_LOOKING:
		s_controls.looking.generic.flags |= QMF_HIGHLIGHT;
		break;

	case C_WEAPONS:
		s_controls.weapons.generic.flags |= QMF_HIGHLIGHT;
		break;

	case C_MISC:
		s_controls.misc.generic.flags |= QMF_HIGHLIGHT;
		break;
	}

	// special case :/
	if (0 == UI_GetCvarInt("cl_voip") || 1 == UI_GetCvarInt("cl_voipUseVAD")) {
		s_controls.pushToTalk.generic.flags |= QMF_GRAYED;
	}
}

/*
=================
Controls_DrawKeyBinding
=================
*/
static void Controls_DrawKeyBinding(void *self) {
	const menuaction_s *a = (menuaction_s *)self;
	const int x = a->generic.x;
	const int y = a->generic.y;
	const int b1 = g_bindings[a->generic.id].bind1;
	char name[32];

	if (g_bindings[a->generic.id].id != a->generic.id) {
		trap_Error("Index and id is out of sync");
	}

	if (b1 == -1) {
		Q_strncpyz(name, "???", sizeof(name));
	} else {
		const int b2 = g_bindings[a->generic.id].bind2;
		char name2[32];

		trap_Key_KeynumToStringBuf(b1, name, sizeof(name));
		Q_strupr(name);

		if (b2 != -1) {
			trap_Key_KeynumToStringBuf(b2, name2, sizeof(name2));
			Q_strupr(name2);

			Q_strcat(name, sizeof(name), " or ");
			Q_strcat(name, sizeof(name), name2);
		}
	}

	if (Menu_ItemAtCursor(a->generic.parent) == a) {
		UI_FillRect(a->generic.left, a->generic.top, a->generic.right - a->generic.left + 1,
					a->generic.bottom - a->generic.top + 1, listbar_color);

		UI_DrawString(x - SMALLCHAR_WIDTH, y, g_bindings[a->generic.id].label, UI_RIGHT | UI_SMALLFONT,
					  text_color_highlight);
		UI_DrawString(x + SMALLCHAR_WIDTH, y, name, UI_LEFT | UI_SMALLFONT | UI_PULSE, text_color_highlight);

		if (s_controls.waitingforkey) {
			UI_DrawChar(x, y, '=', UI_CENTER | UI_BLINK | UI_SMALLFONT, text_color_highlight);
			UI_DrawString(450, SCREEN_HEIGHT * 0.86, "Waiting for new key ...", UI_SMALLFONT | UI_CENTER | UI_PULSE, colorWhite);
			UI_DrawString(450, SCREEN_HEIGHT * 0.89, "Press ESCAPE to cancel", UI_SMALLFONT | UI_CENTER | UI_PULSE, colorWhite);
		} else {
			UI_DrawChar(x, y, 13, UI_CENTER | UI_BLINK | UI_SMALLFONT, text_color_highlight);
			UI_DrawString(450, SCREEN_HEIGHT * 0.86, "Press ENTER or CLICK to change", UI_SMALLFONT | UI_CENTER, colorWhite);
			UI_DrawString(450, SCREEN_HEIGHT * 0.89, "Press BACKSPACE to clear", UI_SMALLFONT | UI_CENTER, colorWhite);
		}
	} else {
		if (a->generic.flags & QMF_GRAYED) {
			UI_DrawString(x - SMALLCHAR_WIDTH, y, g_bindings[a->generic.id].label, UI_RIGHT | UI_SMALLFONT,
						  text_color_disabled);
			UI_DrawString(x + SMALLCHAR_WIDTH, y, name, UI_LEFT | UI_SMALLFONT, text_color_disabled);
		} else {
			UI_DrawString(x - SMALLCHAR_WIDTH, y, g_bindings[a->generic.id].label, UI_RIGHT | UI_SMALLFONT,
						  controls_binding_color);
			UI_DrawString(x + SMALLCHAR_WIDTH, y, name, UI_LEFT | UI_SMALLFONT, controls_binding_color);
		}
	}
}

/*
=================
Controls_StatusBar
=================
*/
static void Controls_StatusBar(void *self) {
	UI_DrawString(450, SCREEN_HEIGHT * 0.86, "Use Arrow Keys or CLICK to change", UI_SMALLFONT | UI_CENTER, colorWhite);
}

/*
=================
Controls_GetKeyAssignment
=================
*/
static void Controls_GetKeyAssignment(const char *command, int *twokeys) {
	int count;
	int j;
	char b[256];

	twokeys[0] = twokeys[1] = -1;
	count = 0;

	for (j = 0; j < 256; j++) {
		trap_Key_GetBindingBuf(j, b, sizeof(b));
		if (*b == 0) {
			continue;
		}
		if (!Q_stricmp(b, command)) {
			twokeys[count] = j;
			count++;
			if (count == 2)
				break;
		}
	}
}

/*
=================
Controls_GetConfig
=================
*/
static void Controls_GetConfig(void) {
	int i;
	int twokeys[2];
	bind_t *bindptr;

	// put the bindings into a local store
	bindptr = g_bindings;

	// iterate each command, get its numeric binding
	for (i = 0;; i++, bindptr++) {
		if (!bindptr->label)
			break;

		Controls_GetKeyAssignment(bindptr->command, twokeys);

		bindptr->bind1 = twokeys[0];
		bindptr->bind2 = twokeys[1];
	}

	s_controls.invertmouse.curvalue = Controls_GetCvarValue("m_pitch") < 0;
	s_controls.smoothmouse.curvalue = UI_ClampCvar(0, 1, Controls_GetCvarValue("m_filter"));
	s_controls.alwaysrun.curvalue = UI_ClampCvar(0, 1, Controls_GetCvarValue("cl_run"));
	s_controls.autoswitch.curvalue = UI_ClampCvar(0, 1, Controls_GetCvarValue("cg_autoswitch"));
	s_controls.sensitivity.curvalue = UI_ClampCvar(2, 30, Controls_GetCvarValue("sensitivity"));
	s_controls.joyenable.curvalue = UI_ClampCvar(0, 1, Controls_GetCvarValue("in_joystick"));
	s_controls.joythreshold.curvalue = UI_ClampCvar(0.05f, 0.75f, Controls_GetCvarValue("joy_threshold"));
	s_controls.freelook.curvalue = UI_ClampCvar(0, 1, Controls_GetCvarValue("cl_freelook"));
}

/*
=================
Controls_SetConfig
=================
*/
static void Controls_SetConfig(void) {
	int i;
	bind_t *bindptr;

	// set the bindings from the local store
	bindptr = g_bindings;

	// iterate each command, get its numeric binding
	for (i = 0;; i++, bindptr++) {
		if (!bindptr->label)
			break;

		if (bindptr->bind1 != -1) {
			trap_Key_SetBinding(bindptr->bind1, bindptr->command);

			if (bindptr->bind2 != -1)
				trap_Key_SetBinding(bindptr->bind2, bindptr->command);
		}
	}

	if (s_controls.invertmouse.curvalue)
		trap_Cvar_SetValue("m_pitch", -fabs(trap_Cvar_VariableValue("m_pitch")));
	else
		trap_Cvar_SetValue("m_pitch", fabs(trap_Cvar_VariableValue("m_pitch")));

	trap_Cvar_SetValue("m_filter", s_controls.smoothmouse.curvalue);
	trap_Cvar_SetValue("cl_run", s_controls.alwaysrun.curvalue);
	trap_Cvar_SetValue("cg_autoswitch", s_controls.autoswitch.curvalue);
	trap_Cvar_SetValue("sensitivity", s_controls.sensitivity.curvalue);
	trap_Cvar_SetValue("in_joystick", s_controls.joyenable.curvalue);
	trap_Cvar_SetValue("joy_threshold", s_controls.joythreshold.curvalue);
	trap_Cvar_SetValue("cl_freelook", s_controls.freelook.curvalue);
	trap_Cmd_ExecuteText(EXEC_APPEND, "in_restart\n");
}

void SetDefaultBinds_onUnusedKeys(void) {
	bind_t *bindptr;
	int i;
	char buff[8];

	if (!wop_AutoBindUnusedKeys.integer)
		return;

	bindptr = g_bindings;
	for (i = 0;; i++, bindptr++) {
		if (!bindptr->label)
			break;

		if (bindptr->defaultbind1 == -1)
			continue;
		buff[0] = 0;
		trap_Key_GetBindingBuf(bindptr->defaultbind1, buff, 8);
		if (buff[0] == 0)
			trap_Key_SetBinding(bindptr->defaultbind1, bindptr->command);

		if (bindptr->defaultbind2 == -1)
			continue;
		buff[0] = 0;
		trap_Key_GetBindingBuf(bindptr->defaultbind2, buff, 8);
		if (buff[0] == 0)
			trap_Key_SetBinding(bindptr->defaultbind2, bindptr->command);
	}
}

/*
=================
Controls_SetDefaults
=================
*/
static void Controls_SetDefaults(void) {
	int i;
	bind_t *bindptr;

	// set the bindings from the local store
	bindptr = g_bindings;

	// iterate each command, set its default binding
	for (i = 0;; i++, bindptr++) {
		if (!bindptr->label)
			break;

		bindptr->bind1 = bindptr->defaultbind1;
		bindptr->bind2 = bindptr->defaultbind2;
	}

	s_controls.invertmouse.curvalue = Controls_GetCvarDefault("m_pitch") < 0;
	s_controls.alwaysrun.curvalue = Controls_GetCvarDefault("cl_run");
	s_controls.smoothmouse.curvalue = Controls_GetCvarDefault("m_filter");
	s_controls.autoswitch.curvalue = Controls_GetCvarDefault("cg_autoswitch");
	s_controls.sensitivity.curvalue = Controls_GetCvarDefault("sensitivity");
	s_controls.joyenable.curvalue = Controls_GetCvarDefault("in_joystick");
	s_controls.joythreshold.curvalue = Controls_GetCvarDefault("joy_threshold");
	s_controls.freelook.curvalue = Controls_GetCvarDefault("cl_freelook");
}

/*
=================
Controls_MenuKey
=================
*/
static sfxHandle_t Controls_MenuKey(int key) {
	int id;
	int i;
	qboolean found;
	bind_t *bindptr;
	found = qfalse;

	if (!s_controls.waitingforkey) {
		switch (key) {
		case K_BACKSPACE:
		case K_DEL:
		case K_KP_DEL:
			key = -1;
			break;

		case K_MOUSE2:
		case K_ESCAPE:
			if (s_controls.changesmade)
				Controls_SetConfig();
			goto ignorekey;

		default:
			goto ignorekey;
		}
	} else {
		if (key & K_CHAR_FLAG)
			goto ignorekey;

		switch (key) {
		case K_ESCAPE:
			s_controls.waitingforkey = qfalse;
			Controls_Update();
			return menu_null_sound;

		case '`':
			goto ignorekey;
		}
	}

	s_controls.changesmade = qtrue;

	if (key != -1) {
		// remove from any other bind
		bindptr = g_bindings;
		for (i = 0;; i++, bindptr++) {
			if (!bindptr->label)
				break;

			if (bindptr->bind2 == key)
				bindptr->bind2 = -1;

			if (bindptr->bind1 == key) {
				bindptr->bind1 = bindptr->bind2;
				bindptr->bind2 = -1;
			}
		}
	}

	// assign key to local store
	id = ((menucommon_s *)(s_controls.menu.items[s_controls.menu.cursor]))->id;
	bindptr = g_bindings;
	for (i = 0;; i++, bindptr++) {
		if (!bindptr->label)
			break;

		if (bindptr->id == id) {
			found = qtrue;
			if (key == -1) {
				if (bindptr->bind1 != -1) {
					trap_Key_SetBinding(bindptr->bind1, "");
					bindptr->bind1 = -1;
				}
				if (bindptr->bind2 != -1) {
					trap_Key_SetBinding(bindptr->bind2, "");
					bindptr->bind2 = -1;
				}
			} else if (bindptr->bind1 == -1) {
				bindptr->bind1 = key;
			} else if (bindptr->bind1 != key && bindptr->bind2 == -1) {
				bindptr->bind2 = key;
			} else {
				trap_Key_SetBinding(bindptr->bind1, "");
				trap_Key_SetBinding(bindptr->bind2, "");
				bindptr->bind1 = key;
				bindptr->bind2 = -1;
			}
			break;
		}
	}

	s_controls.waitingforkey = qfalse;

	if (found) {
		Controls_Update();
		return menu_null_sound;
	}

ignorekey:
	return Menu_DefaultKey(&s_controls.menu, key);
}

/*
=================
Controls_ResetDefaults_Action
=================
*/
static void Controls_ResetDefaults_Action(qboolean result) {
	if (!result) {
		return;
	}

	s_controls.changesmade = qtrue;
	Controls_SetDefaults();
	Controls_Update();
}

/*
=================
Controls_ResetDefaults_Draw
=================
*/
static void Controls_ResetDefaults_Draw(void) {
	UI_DrawProportionalString(SCREEN_WIDTH / 2, 356 + PROP_HEIGHT * 0, "WARNING: This will reset all",
							  UI_CENTER | UI_SMALLFONT, color_yellow);
	UI_DrawProportionalString(SCREEN_WIDTH / 2, 356 + PROP_HEIGHT * 1, "controls to their default values.",
							  UI_CENTER | UI_SMALLFONT, color_yellow);
}

/*
=================
Controls_MenuEvent
=================
*/
static void Controls_MenuEvent(void *ptr, int event) {
	switch (((menucommon_s *)ptr)->id) {
	case ID_MOVEMENT:
		if (event == QM_ACTIVATED) {
			s_controls.section = C_MOVEMENT;
			Controls_Update();
		}
		break;

	case ID_LOOKING:
		if (event == QM_ACTIVATED) {
			s_controls.section = C_LOOKING;
			Controls_Update();
		}
		break;

	case ID_WEAPONS:
		if (event == QM_ACTIVATED) {
			s_controls.section = C_WEAPONS;
			Controls_Update();
		}
		break;

	case ID_MISC:
		if (event == QM_ACTIVATED) {
			s_controls.section = C_MISC;
			Controls_Update();
		}
		break;

	case ID_DEFAULTS:
		if (event == QM_ACTIVATED) {
			UI_ConfirmMenu("SET TO DEFAULTS?", Controls_ResetDefaults_Draw, Controls_ResetDefaults_Action);
		}
		break;

	case ID_BACK:
		if (event == QM_ACTIVATED) {
			if (s_controls.changesmade)
				Controls_SetConfig();
			UI_PopMenu();
		}
		break;

	case ID_SAVEANDEXIT:
		if (event == QM_ACTIVATED) {
			Controls_SetConfig();
			UI_PopMenu();
		}
		break;

	case ID_EXIT:
		if (event == QM_ACTIVATED) {
			UI_PopMenu();
		}
		break;

	case ID_FREELOOK:
	case ID_MOUSESPEED:
	case ID_INVERTMOUSE:
	case ID_SMOOTHMOUSE:
	case ID_ALWAYSRUN:
	case ID_AUTOSWITCH:
	case ID_JOYENABLE:
	case ID_JOYTHRESHOLD:
		if (event == QM_ACTIVATED) {
			s_controls.changesmade = qtrue;
		}
		break;
	}
}

/*
=================
Controls_ActionEvent
=================
*/
static void Controls_ActionEvent(void *ptr, int event) {
	if (event == QM_LOSTFOCUS) {
		Controls_UpdateModel(ANIM_IDLE);
	} else if (event == QM_GOTFOCUS) {
		Controls_UpdateModel(g_bindings[((menucommon_s *)ptr)->id].anim);
	} else if ((event == QM_ACTIVATED) && !s_controls.waitingforkey) {
		s_controls.waitingforkey = 1;
		Controls_Update();
	}
}

/*
=================
Controls_MenuInit
=================
*/
static void Controls_MenuInit(void) {
	// zero set all our globals
	memset(&s_controls, 0, sizeof(s_controls));

	Controls_Cache();

	s_controls.menu.key = Controls_MenuKey;
	s_controls.menu.wrapAround = qtrue;
	s_controls.menu.fullscreen = qtrue;
	s_controls.menu.noPushSelect = qtrue;
	s_controls.menu.bgparts = BGP_CONTROLBG | BGP_SIMPLEBG;

	s_controls.looking.generic.type = MTYPE_BITMAP;
	s_controls.looking.generic.name = LOOK0;
	s_controls.looking.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.looking.generic.x = 412;
	s_controls.looking.generic.y = 40;
	s_controls.looking.generic.id = ID_LOOKING;
	s_controls.looking.generic.callback = Controls_MenuEvent;
	s_controls.looking.width = 80;
	s_controls.looking.height = 40;
	s_controls.looking.focuspic = LOOK1;
	s_controls.looking.focuspicinstead = qtrue;

	s_controls.movement.generic.type = MTYPE_BITMAP;
	s_controls.movement.generic.name = MOVEMENT0;
	s_controls.movement.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.movement.generic.x = 500;
	s_controls.movement.generic.y = 45;
	s_controls.movement.generic.id = ID_MOVEMENT;
	s_controls.movement.generic.callback = Controls_MenuEvent;
	s_controls.movement.width = 80;
	s_controls.movement.height = 40;
	s_controls.movement.focuspic = MOVEMENT1;
	s_controls.movement.focuspicinstead = qtrue;

	s_controls.weapons.generic.type = MTYPE_BITMAP;
	s_controls.weapons.generic.name = WEAPONS0;
	s_controls.weapons.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.weapons.generic.x = 390;
	s_controls.weapons.generic.y = 84;
	s_controls.weapons.generic.id = ID_WEAPONS;
	s_controls.weapons.generic.callback = Controls_MenuEvent;
	s_controls.weapons.width = 120;
	s_controls.weapons.height = 40;
	s_controls.weapons.focuspic = WEAPONS1;
	s_controls.weapons.focuspicinstead = qtrue;

	s_controls.misc.generic.type = MTYPE_BITMAP;
	s_controls.misc.generic.name = MISC0;
	s_controls.misc.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.misc.generic.x = 508;
	s_controls.misc.generic.y = 90;
	s_controls.misc.generic.id = ID_MISC;
	s_controls.misc.generic.callback = Controls_MenuEvent;
	s_controls.misc.width = 80;
	s_controls.misc.height = 40;
	s_controls.misc.focuspic = MISC1;
	s_controls.misc.focuspicinstead = qtrue;

	s_controls.back.generic.type = MTYPE_BITMAP;
	s_controls.back.generic.name = BACK0;
	s_controls.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_controls.back.generic.x = 552;
	s_controls.back.generic.y = 440;
	s_controls.back.generic.id = ID_BACK;
	s_controls.back.generic.callback = Controls_MenuEvent;
	s_controls.back.width = 80;
	s_controls.back.height = 40;
	s_controls.back.focuspic = BACK1;
	s_controls.back.focuspicinstead = qtrue;

	s_controls.sensitivity.generic.type = MTYPE_SLIDER;
	s_controls.sensitivity.generic.x = SCREEN_WIDTH / 2;
	s_controls.sensitivity.generic.flags = QMF_SMALLFONT;
	s_controls.sensitivity.generic.name = "Mouse Speed";
	s_controls.sensitivity.generic.id = ID_MOUSESPEED;
	s_controls.sensitivity.generic.callback = Controls_MenuEvent;
	s_controls.sensitivity.minvalue = 2;
	s_controls.sensitivity.maxvalue = 30;
	s_controls.sensitivity.generic.statusbar = Controls_StatusBar;

	s_controls.smoothmouse.generic.type = MTYPE_RADIOBUTTON;
	s_controls.smoothmouse.generic.flags = QMF_SMALLFONT;
	s_controls.smoothmouse.generic.x = SCREEN_WIDTH / 2;
	s_controls.smoothmouse.generic.name = "Smooth Mouse";
	s_controls.smoothmouse.generic.id = ID_SMOOTHMOUSE;
	s_controls.smoothmouse.generic.callback = Controls_MenuEvent;
	s_controls.smoothmouse.generic.statusbar = Controls_StatusBar;
	s_controls.smoothmouse.generic.toolTip = "Switch on to smooth out movement when using the mouse to look around.";

	s_controls.invertmouse.generic.type = MTYPE_RADIOBUTTON;
	s_controls.invertmouse.generic.flags = QMF_SMALLFONT;
	s_controls.invertmouse.generic.x = SCREEN_WIDTH / 2;
	s_controls.invertmouse.generic.name = "Invert Mouse";
	s_controls.invertmouse.generic.id = ID_INVERTMOUSE;
	s_controls.invertmouse.generic.callback = Controls_MenuEvent;
	s_controls.invertmouse.generic.statusbar = Controls_StatusBar;
	s_controls.invertmouse.generic.toolTip = "Switch on to invert the directions of your mouse (not recommended).";

	s_controls.lookup.generic.type = MTYPE_ACTION;
	s_controls.lookup.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.lookup.generic.callback = Controls_ActionEvent;
	s_controls.lookup.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.lookup.generic.id = ID_LOOKUP;

	s_controls.lookdown.generic.type = MTYPE_ACTION;
	s_controls.lookdown.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.lookdown.generic.callback = Controls_ActionEvent;
	s_controls.lookdown.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.lookdown.generic.id = ID_LOOKDOWN;

	s_controls.mouselook.generic.type = MTYPE_ACTION;
	s_controls.mouselook.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS | QMF_GRAYED | QMF_HIDDEN;
	s_controls.mouselook.generic.callback = Controls_ActionEvent;
	s_controls.mouselook.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.mouselook.generic.id = ID_MOUSELOOK;

	s_controls.freelook.generic.type = MTYPE_RADIOBUTTON;
	s_controls.freelook.generic.flags = QMF_SMALLFONT;
	s_controls.freelook.generic.x = SCREEN_WIDTH / 2;
	s_controls.freelook.generic.name = "Free Look";
	s_controls.freelook.generic.id = ID_FREELOOK;
	s_controls.freelook.generic.callback = Controls_MenuEvent;
	s_controls.freelook.generic.statusbar = Controls_StatusBar;

	s_controls.centerview.generic.type = MTYPE_ACTION;
	s_controls.centerview.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.centerview.generic.callback = Controls_ActionEvent;
	s_controls.centerview.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.centerview.generic.id = ID_CENTERVIEW;

	s_controls.thirdPerson.generic.type = MTYPE_ACTION;
	s_controls.thirdPerson.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.thirdPerson.generic.callback = Controls_ActionEvent;
	s_controls.thirdPerson.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.thirdPerson.generic.id = ID_3RDPERSON;
	s_controls.thirdPerson.generic.toolTip =
		"Press this key in game to switch to third person view, press again to switch back to first person view.";

	s_controls.joyenable.generic.type = MTYPE_RADIOBUTTON;
	s_controls.joyenable.generic.flags = QMF_SMALLFONT;
	s_controls.joyenable.generic.x = SCREEN_WIDTH / 2;
	s_controls.joyenable.generic.name = "Joystick";
	s_controls.joyenable.generic.id = ID_JOYENABLE;
	s_controls.joyenable.generic.callback = Controls_MenuEvent;
	s_controls.joyenable.generic.statusbar = Controls_StatusBar;

	s_controls.joythreshold.generic.type = MTYPE_SLIDER;
	s_controls.joythreshold.generic.x = SCREEN_WIDTH / 2;
	s_controls.joythreshold.generic.flags = QMF_SMALLFONT;
	s_controls.joythreshold.generic.name = "Joystick Threshold";
	s_controls.joythreshold.generic.id = ID_JOYTHRESHOLD;
	s_controls.joythreshold.generic.callback = Controls_MenuEvent;
	s_controls.joythreshold.minvalue = 0.05f;
	s_controls.joythreshold.maxvalue = 0.75f;
	s_controls.joythreshold.generic.statusbar = Controls_StatusBar;

	s_controls.alwaysrun.generic.type = MTYPE_RADIOBUTTON;
	s_controls.alwaysrun.generic.flags = QMF_SMALLFONT;
	s_controls.alwaysrun.generic.x = SCREEN_WIDTH / 2;
	s_controls.alwaysrun.generic.name = "Always Run";
	s_controls.alwaysrun.generic.id = ID_ALWAYSRUN;
	s_controls.alwaysrun.generic.callback = Controls_MenuEvent;
	s_controls.alwaysrun.generic.statusbar = Controls_StatusBar;

	s_controls.run.generic.type = MTYPE_ACTION;
	s_controls.run.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.run.generic.callback = Controls_ActionEvent;
	s_controls.run.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.run.generic.id = ID_SPEED;

	s_controls.walkforward.generic.type = MTYPE_ACTION;
	s_controls.walkforward.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.walkforward.generic.callback = Controls_ActionEvent;
	s_controls.walkforward.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.walkforward.generic.id = ID_FORWARD;

	s_controls.backpedal.generic.type = MTYPE_ACTION;
	s_controls.backpedal.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.backpedal.generic.callback = Controls_ActionEvent;
	s_controls.backpedal.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.backpedal.generic.id = ID_BACKPEDAL;

	s_controls.stepleft.generic.type = MTYPE_ACTION;
	s_controls.stepleft.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.stepleft.generic.callback = Controls_ActionEvent;
	s_controls.stepleft.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.stepleft.generic.id = ID_MOVELEFT;

	s_controls.stepright.generic.type = MTYPE_ACTION;
	s_controls.stepright.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.stepright.generic.callback = Controls_ActionEvent;
	s_controls.stepright.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.stepright.generic.id = ID_MOVERIGHT;

	s_controls.moveup.generic.type = MTYPE_ACTION;
	s_controls.moveup.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.moveup.generic.callback = Controls_ActionEvent;
	s_controls.moveup.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.moveup.generic.id = ID_MOVEUP;

	s_controls.movedown.generic.type = MTYPE_ACTION;
	s_controls.movedown.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.movedown.generic.callback = Controls_ActionEvent;
	s_controls.movedown.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.movedown.generic.id = ID_MOVEDOWN;

	s_controls.turnleft.generic.type = MTYPE_ACTION;
	s_controls.turnleft.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.turnleft.generic.callback = Controls_ActionEvent;
	s_controls.turnleft.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.turnleft.generic.id = ID_LEFT;

	s_controls.turnright.generic.type = MTYPE_ACTION;
	s_controls.turnright.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.turnright.generic.callback = Controls_ActionEvent;
	s_controls.turnright.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.turnright.generic.id = ID_RIGHT;

	s_controls.sidestep.generic.type = MTYPE_ACTION;
	s_controls.sidestep.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.sidestep.generic.callback = Controls_ActionEvent;
	s_controls.sidestep.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.sidestep.generic.id = ID_STRAFE;

	s_controls.useitem.generic.type = MTYPE_ACTION;
	s_controls.useitem.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.useitem.generic.callback = Controls_ActionEvent;
	s_controls.useitem.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.useitem.generic.id = ID_USEITEM;
	s_controls.useitem.generic.toolTip = "Press this key in game to throw killerducks / use floater / deploy bambam "
										 "and place boomies (you must look at the ground to place a boomie).";

	s_controls.gesture.generic.type = MTYPE_ACTION;
	s_controls.gesture.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.gesture.generic.callback = Controls_ActionEvent;
	s_controls.gesture.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.gesture.generic.id = ID_GESTURE;
	s_controls.gesture.generic.toolTip = "Press this key in game to taunt other players.";

	s_controls.dropCart.generic.type = MTYPE_ACTION;
	s_controls.dropCart.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.dropCart.generic.callback = Controls_ActionEvent;
	s_controls.dropCart.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.dropCart.generic.id = ID_DROPCART;

	s_controls.attack.generic.type = MTYPE_ACTION;
	s_controls.attack.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.attack.generic.callback = Controls_ActionEvent;
	s_controls.attack.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.attack.generic.id = ID_ATTACK;

	s_controls.zoom.generic.type = MTYPE_ACTION;
	s_controls.zoom.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.zoom.generic.callback = Controls_ActionEvent;
	s_controls.zoom.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.zoom.generic.id = ID_ZOOM;
	s_controls.zoom.generic.toolTip = "Press this key in game to enable zoom mode for weapons that support it.";

	s_controls.nextweapon.generic.type = MTYPE_ACTION;
	s_controls.nextweapon.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.nextweapon.generic.callback = Controls_ActionEvent;
	s_controls.nextweapon.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.nextweapon.generic.id = ID_WEAPNEXT;

	s_controls.prevweapon.generic.type = MTYPE_ACTION;
	s_controls.prevweapon.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.prevweapon.generic.callback = Controls_ActionEvent;
	s_controls.prevweapon.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.prevweapon.generic.id = ID_WEAPPREV;

	s_controls.autoswitch.generic.type = MTYPE_RADIOBUTTON;
	s_controls.autoswitch.generic.flags = QMF_SMALLFONT;
	s_controls.autoswitch.generic.x = SCREEN_WIDTH / 2;
	s_controls.autoswitch.generic.name = "Autoswitch Weapons";
	s_controls.autoswitch.generic.id = ID_AUTOSWITCH;
	s_controls.autoswitch.generic.callback = Controls_MenuEvent;
	s_controls.autoswitch.generic.statusbar = Controls_StatusBar;
	s_controls.autoswitch.generic.toolTip =
		"If enabled, your character will automatically switch to the weapon that you run into to pick up.";

	s_controls.chainsaw.generic.type = MTYPE_ACTION;
	s_controls.chainsaw.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.chainsaw.generic.callback = Controls_ActionEvent;
	s_controls.chainsaw.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.chainsaw.generic.id = ID_WEAPON1;

	s_controls.machinegun.generic.type = MTYPE_ACTION;
	s_controls.machinegun.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.machinegun.generic.callback = Controls_ActionEvent;
	s_controls.machinegun.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.machinegun.generic.id = ID_WEAPON2;

	s_controls.shotgun.generic.type = MTYPE_ACTION;
	s_controls.shotgun.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.shotgun.generic.callback = Controls_ActionEvent;
	s_controls.shotgun.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.shotgun.generic.id = ID_WEAPON3;

	s_controls.grenadelauncher.generic.type = MTYPE_ACTION;
	s_controls.grenadelauncher.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.grenadelauncher.generic.callback = Controls_ActionEvent;
	s_controls.grenadelauncher.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.grenadelauncher.generic.id = ID_WEAPON4;

	s_controls.rocketlauncher.generic.type = MTYPE_ACTION;
	s_controls.rocketlauncher.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.rocketlauncher.generic.callback = Controls_ActionEvent;
	s_controls.rocketlauncher.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.rocketlauncher.generic.id = ID_WEAPON5;

	s_controls.lightning.generic.type = MTYPE_ACTION;
	s_controls.lightning.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.lightning.generic.callback = Controls_ActionEvent;
	s_controls.lightning.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.lightning.generic.id = ID_WEAPON6;

	s_controls.railgun.generic.type = MTYPE_ACTION;
	s_controls.railgun.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.railgun.generic.callback = Controls_ActionEvent;
	s_controls.railgun.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.railgun.generic.id = ID_WEAPON7;

	s_controls.plasma.generic.type = MTYPE_ACTION;
	s_controls.plasma.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.plasma.generic.callback = Controls_ActionEvent;
	s_controls.plasma.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.plasma.generic.id = ID_WEAPON8;

	s_controls.bfg.generic.type = MTYPE_ACTION;
	s_controls.bfg.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.bfg.generic.callback = Controls_ActionEvent;
	s_controls.bfg.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.bfg.generic.id = ID_WEAPON9;

	s_controls.showscores.generic.type = MTYPE_ACTION;
	s_controls.showscores.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.showscores.generic.callback = Controls_ActionEvent;
	s_controls.showscores.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.showscores.generic.id = ID_SHOWSCORES;

	s_controls.togglemenu.generic.type = MTYPE_ACTION;
	s_controls.togglemenu.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS | QMF_GRAYED | QMF_HIDDEN;
	s_controls.togglemenu.generic.callback = Controls_ActionEvent;
	s_controls.togglemenu.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.togglemenu.generic.id = ID_TOGGLEMENU;

	s_controls.music.generic.type = MTYPE_ACTION;
	s_controls.music.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.music.generic.callback = Controls_ActionEvent;
	s_controls.music.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.music.generic.id = ID_MUSIC;

	s_controls.helpGametype.generic.type = MTYPE_ACTION;
	s_controls.helpGametype.generic.flags = (QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN);
	s_controls.helpGametype.generic.callback = Controls_ActionEvent;
	s_controls.helpGametype.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.helpGametype.generic.id = ID_HELP_GAMETYPE;
	s_controls.helpGametype.generic.toolTip =
		"Press this key in game to see a basic description of the rules of the gametype set.";

	s_controls.helpItems.generic.type = MTYPE_ACTION;
	s_controls.helpItems.generic.flags = (QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN);
	s_controls.helpItems.generic.callback = Controls_ActionEvent;
	s_controls.helpItems.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.helpItems.generic.id = ID_HELP_ITEMS;
	s_controls.helpItems.generic.toolTip =
		"Press this key in game to see information on what the weapons / powerups / items and icons are.";

	s_controls.ServerInfo.generic.type = MTYPE_ACTION;
	s_controls.ServerInfo.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.ServerInfo.generic.callback = Controls_ActionEvent;
	s_controls.ServerInfo.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.ServerInfo.generic.id = ID_SERVERINFO;
	s_controls.ServerInfo.generic.toolTip = "Press this key in game to display server settings information.";

	s_controls.chat.generic.type = MTYPE_ACTION;
	s_controls.chat.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.chat.generic.callback = Controls_ActionEvent;
	s_controls.chat.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.chat.generic.id = ID_CHAT;
	s_controls.chat.generic.toolTip =
		"Press this key in game before typing a message to text chat to all players in the game.";

	s_controls.chat2.generic.type = MTYPE_ACTION;
	s_controls.chat2.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.chat2.generic.callback = Controls_ActionEvent;
	s_controls.chat2.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.chat2.generic.id = ID_CHAT2;

	s_controls.chat3.generic.type = MTYPE_ACTION;
	s_controls.chat3.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.chat3.generic.callback = Controls_ActionEvent;
	s_controls.chat3.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.chat3.generic.id = ID_CHAT3;

	s_controls.chat4.generic.type = MTYPE_ACTION;
	s_controls.chat4.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.chat4.generic.callback = Controls_ActionEvent;
	s_controls.chat4.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.chat4.generic.id = ID_CHAT4;

	s_controls.nextSong.generic.type = MTYPE_ACTION;
	s_controls.nextSong.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.nextSong.generic.callback = Controls_ActionEvent;
	s_controls.nextSong.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.nextSong.generic.id = ID_NEXTSONG;

	s_controls.pushToTalk.generic.type = MTYPE_ACTION;
	s_controls.pushToTalk.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.pushToTalk.generic.callback = Controls_ActionEvent;
	s_controls.pushToTalk.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.pushToTalk.generic.id = ID_PUSH2TALK;
	s_controls.pushToTalk.generic.toolTip = "Press and hold this key to talk to other players through your microphone. "
											"This is not required if your microphone setting is set to open.";

	s_controls.voteyes.generic.type = MTYPE_ACTION;
	s_controls.voteyes.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.voteyes.generic.callback = Controls_ActionEvent;
	s_controls.voteyes.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.voteyes.generic.id = ID_VOTEYES;
	s_controls.voteyes.generic.toolTip = "Cast a yes vote to a vote in progress (votes appear top left of the screen).";

	s_controls.voteno.generic.type = MTYPE_ACTION;
	s_controls.voteno.generic.flags = QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN;
	s_controls.voteno.generic.callback = Controls_ActionEvent;
	s_controls.voteno.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.voteno.generic.id = ID_VOTENO;
	s_controls.voteno.generic.toolTip = "Cast a no vote to a vote in progress (votes appear top left of the screen).";

	Menu_AddItem(&s_controls.menu, &s_controls.looking);
	Menu_AddItem(&s_controls.menu, &s_controls.movement);
	Menu_AddItem(&s_controls.menu, &s_controls.weapons);
	Menu_AddItem(&s_controls.menu, &s_controls.misc);

	Menu_AddItem(&s_controls.menu, &s_controls.sensitivity);
	Menu_AddItem(&s_controls.menu, &s_controls.smoothmouse);
	Menu_AddItem(&s_controls.menu, &s_controls.invertmouse);
	Menu_AddItem(&s_controls.menu, &s_controls.lookup);
	Menu_AddItem(&s_controls.menu, &s_controls.lookdown);
	Menu_AddItem(&s_controls.menu, &s_controls.mouselook);
	Menu_AddItem(&s_controls.menu, &s_controls.freelook);
	Menu_AddItem(&s_controls.menu, &s_controls.centerview);
	Menu_AddItem(&s_controls.menu, &s_controls.thirdPerson);
	Menu_AddItem(&s_controls.menu, &s_controls.joyenable);
	Menu_AddItem(&s_controls.menu, &s_controls.joythreshold);

	Menu_AddItem(&s_controls.menu, &s_controls.alwaysrun);
	Menu_AddItem(&s_controls.menu, &s_controls.run);
	Menu_AddItem(&s_controls.menu, &s_controls.walkforward);
	Menu_AddItem(&s_controls.menu, &s_controls.backpedal);
	Menu_AddItem(&s_controls.menu, &s_controls.stepleft);
	Menu_AddItem(&s_controls.menu, &s_controls.stepright);
	Menu_AddItem(&s_controls.menu, &s_controls.moveup);
	Menu_AddItem(&s_controls.menu, &s_controls.movedown);
	Menu_AddItem(&s_controls.menu, &s_controls.turnleft);
	Menu_AddItem(&s_controls.menu, &s_controls.turnright);
	Menu_AddItem(&s_controls.menu, &s_controls.sidestep);
	Menu_AddItem(&s_controls.menu, &s_controls.gesture);
	Menu_AddItem(&s_controls.menu, &s_controls.useitem);
	Menu_AddItem(&s_controls.menu, &s_controls.dropCart);

	Menu_AddItem(&s_controls.menu, &s_controls.attack);
	Menu_AddItem(&s_controls.menu, &s_controls.zoom);
	Menu_AddItem(&s_controls.menu, &s_controls.nextweapon);
	Menu_AddItem(&s_controls.menu, &s_controls.prevweapon);
	Menu_AddItem(&s_controls.menu, &s_controls.autoswitch);
	Menu_AddItem(&s_controls.menu, &s_controls.chainsaw);
	Menu_AddItem(&s_controls.menu, &s_controls.machinegun);
	Menu_AddItem(&s_controls.menu, &s_controls.shotgun);
	Menu_AddItem(&s_controls.menu, &s_controls.grenadelauncher);
	Menu_AddItem(&s_controls.menu, &s_controls.rocketlauncher);
	Menu_AddItem(&s_controls.menu, &s_controls.lightning);
	Menu_AddItem(&s_controls.menu, &s_controls.railgun);
	Menu_AddItem(&s_controls.menu, &s_controls.plasma);
	Menu_AddItem(&s_controls.menu, &s_controls.bfg);

	Menu_AddItem(&s_controls.menu, &s_controls.showscores);
	Menu_AddItem(&s_controls.menu, &s_controls.togglemenu);
	Menu_AddItem(&s_controls.menu, &s_controls.music);
	Menu_AddItem(&s_controls.menu, &s_controls.helpGametype);
	Menu_AddItem(&s_controls.menu, &s_controls.helpItems);
	Menu_AddItem(&s_controls.menu, &s_controls.ServerInfo);
	Menu_AddItem(&s_controls.menu, &s_controls.chat);
	Menu_AddItem(&s_controls.menu, &s_controls.chat2);
	Menu_AddItem(&s_controls.menu, &s_controls.chat3);
	Menu_AddItem(&s_controls.menu, &s_controls.chat4);
	Menu_AddItem(&s_controls.menu, &s_controls.pushToTalk);
	Menu_AddItem(&s_controls.menu, &s_controls.nextSong);
	Menu_AddItem(&s_controls.menu, &s_controls.voteyes);
	Menu_AddItem(&s_controls.menu, &s_controls.voteno);

	Menu_AddItem(&s_controls.menu, &s_controls.back);

	// initialize the configurable cvars
	Controls_InitCvars();

	// initialize the current config
	Controls_GetConfig();

	// intialize the weapons
	//	Controls_InitWeapons ();

	// initial default section
	s_controls.section = C_LOOKING;

	// update the ui
	Controls_Update();
}

/*
=================
Controls_Cache
=================
*/
void Controls_Cache(void) {
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);

	trap_R_RegisterShaderNoMip(LOOK0);
	trap_R_RegisterShaderNoMip(LOOK1);
	trap_R_RegisterShaderNoMip(MOVEMENT0);
	trap_R_RegisterShaderNoMip(MOVEMENT1);
	trap_R_RegisterShaderNoMip(WEAPONS0);
	trap_R_RegisterShaderNoMip(WEAPONS1);
	trap_R_RegisterShaderNoMip(MISC0);
	trap_R_RegisterShaderNoMip(MISC1);
}

/*
=================
UI_ControlsMenu
=================
*/
void UI_ControlsMenu(void) {
	Controls_MenuInit();
	UI_PushMenu(&s_controls.menu);
}
