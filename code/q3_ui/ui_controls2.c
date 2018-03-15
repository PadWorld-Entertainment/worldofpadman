// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

CONTROLS MENU

=======================================================================
*/


#include "ui_local.h"

#define ART_BACK0			"menu/btnback0"
#define ART_BACK1			"menu/btnback1"
#define LOOK0				"menu/controls/look0"
#define LOOK1				"menu/controls/look1"
#define MOVEMENT0			"menu/controls/move0"
#define MOVEMENT1			"menu/controls/move1"
#define WEAPONS0			"menu/controls/shoot0"
#define WEAPONS1			"menu/controls/shoot1"
#define MISC0				"menu/controls/misc0"
#define MISC1				"menu/controls/misc1"


typedef struct {
	char	*command;
	char	*label;
	int		id;
	int		anim;
	int		defaultbind1;
	int		defaultbind2;
	int		bind1;
	int		bind2;
} bind_t;

typedef struct
{
	char*	name;
	float	defaultvalue;
	float	value;	
} configcvar_t;

#define SAVE_NOOP		0
#define SAVE_YES		1
#define SAVE_NO			2
#define SAVE_CANCEL		3

// control sections
#define C_MOVEMENT		0
#define C_LOOKING		1
#define C_WEAPONS		2
#define C_MISC			3
#define C_MAX			4

#define ID_MOVEMENT		100
#define ID_LOOKING		101
#define ID_WEAPONS		102
#define ID_MISC			103
#define ID_DEFAULTS		104
#define ID_BACK			105
#define ID_SAVEANDEXIT	106
#define ID_EXIT			107

// bindable actions
#define ID_SHOWSCORES		0
#define ID_USEITEM			1	
#define ID_SPEED			2	
#define ID_FORWARD			3	
#define ID_BACKPEDAL		4
#define ID_MOVELEFT			5
#define ID_MOVERIGHT		6
#define ID_MOVEUP			7	
#define ID_MOVEDOWN			8
#define ID_WEAPON1			9
#define ID_WEAPON2			10
#define ID_WEAPON3			11
#define ID_WEAPON4			12
#define ID_WEAPON5			13
#define ID_WEAPON6			14
#define ID_WEAPON7			15
#define ID_WEAPON8			16
#define ID_WEAPON9			17
#define ID_ATTACK			18
#define ID_WEAPPREV			19
#define ID_WEAPNEXT			20
#define ID_GESTURE			21
#define ID_CHAT				22
#define ID_CHAT2			23
/*#define ID_CHAT3			42
#define ID_CHAT4			42*/

#define ID_VOTEYES			24
#define ID_VOTENO			25

#define ID_MUSIC			26
#define ID_NEXTSONG			27
#define ID_DROPCART			28
#define ID_SERVERINFO		29
#define ID_3RDPERSON		30
#define ID_HELP_GAMETYPE	31
#define ID_HELP_ITEMS		32
#define ID_PUSH2TALK		33
#define ID_ZOOM				34


// all others
#define ID_INVERTMOUSE	43
#define ID_AUTOSWITCH	45
#define ID_MOUSESPEED	46
#define ID_SMOOTHMOUSE	49

#define ANIM_IDLE		0
#define ANIM_RUN		1
#define ANIM_WALK		2
#define ANIM_BACK		3
#define ANIM_JUMP		4
#define ANIM_CROUCH		5
#define ANIM_STEPLEFT	6
#define ANIM_STEPRIGHT	7
#define ANIM_TURNLEFT	8
#define ANIM_TURNRIGHT	9
#define ANIM_LOOKUP		10
#define ANIM_LOOKDOWN	11
#define ANIM_WEAPON1	12
#define ANIM_WEAPON2	13
#define ANIM_WEAPON3	14
#define ANIM_WEAPON4	15
#define ANIM_WEAPON5	16
#define ANIM_WEAPON6	17
#define ANIM_WEAPON7	18
#define ANIM_WEAPON8	19
#define ANIM_WEAPON9	20
#define ANIM_WEAPON10	21
#define ANIM_ATTACK		22
#define ANIM_GESTURE	23
#define ANIM_DIE		24
#define ANIM_CHAT		25

typedef struct
{
	menuframework_s		menu;

	menubitmap_s		movement;
	menubitmap_s		looking;
	menubitmap_s		weapons;
	menubitmap_s		misc;

	menuaction_s		walkforward;
	menuaction_s		backpedal;
	menuaction_s		stepleft;
	menuaction_s		stepright;
	menuaction_s		moveup;
	menuaction_s		movedown;
	menuaction_s		run;
	menuaction_s		machinegun;
	menuaction_s		chainsaw;
	menuaction_s		shotgun;
	menuaction_s		grenadelauncher;
	menuaction_s		rocketlauncher;
	menuaction_s		lightning;
	menuaction_s		railgun;
	menuaction_s		plasma;
	menuaction_s		bfg;
	menuaction_s		attack;
	menuaction_s		prevweapon;
	menuaction_s		nextweapon;
	menuaction_s		gesture;
	menuradiobutton_s	invertmouse;
	menuslider_s		sensitivity;
	menuradiobutton_s	smoothmouse;
	menuaction_s		zoom;
	menuradiobutton_s	autoswitch;
	menuaction_s		showscores;
	menuaction_s		useitem;
	playerInfo_t		playerinfo;
	qboolean			changesmade;
	menuaction_s		chat;
	menuaction_s		chat2;
	menuaction_s		chat3;
	menuaction_s		chat4;

	menuaction_s		music;
	menuaction_s		nextSong;
	menuaction_s		dropCart;
	menuaction_s		ServerInfo;
	menuaction_s		thirdPerson;
	menuaction_s		pushToTalk;

	menuaction_s		helpGametype;
	menuaction_s		helpItems;

	int					section;
	qboolean			waitingforkey;
	char				playerModel[64];
	vec3_t				playerViewangles;
	vec3_t				playerMoveangles;
	int					playerLegs;
	int					playerTorso;
	int					playerWeapon;
	qboolean			playerChat;

	menubitmap_s		back;
	menutext_s			name;
} controls_t; 	

static controls_t s_controls;

static vec4_t controls_binding_color  = {1.00f, 1.00f, 0.00f, 1.00f};

static bind_t g_bindings[] = 
{
	{"+scores",			"show scores",		ID_SHOWSCORES,	ANIM_IDLE,		K_TAB,			-1,		-1, -1},
	{"+button2",		"use item",			ID_USEITEM,		ANIM_IDLE,		K_ENTER,		-1,		-1, -1},
	{"+speed", 			"run / walk",		ID_SPEED,		ANIM_RUN,		K_SHIFT,		-1,		-1,	-1},
	{"+forward", 		"walk forward",		ID_FORWARD,		ANIM_WALK,		K_UPARROW,		-1,		-1, -1},
	{"+back", 			"backpedal",		ID_BACKPEDAL,	ANIM_BACK,		K_DOWNARROW,	-1,		-1, -1},
	{"+moveleft", 		"step left",		ID_MOVELEFT,	ANIM_STEPLEFT,	',',			-1,		-1, -1},
	{"+moveright", 		"step right",		ID_MOVERIGHT,	ANIM_STEPRIGHT,	'.',			-1,		-1, -1},
	{"+moveup",			"up / jump",		ID_MOVEUP,		ANIM_JUMP,		K_SPACE,		-1,		-1, -1},
	{"+movedown",		"down / crouch",	ID_MOVEDOWN,	ANIM_CROUCH,	'c',			-1,		-1, -1},

	{"weapon 1",		"PUNCHY",			ID_WEAPON1,		ANIM_WEAPON1,	'1',			-1,		-1, -1},
	{"weapon 2",		"NiPPER",			ID_WEAPON2,		ANIM_WEAPON2,	'2',			-1,		-1, -1},
	{"weapon 3",		"PUMPER",			ID_WEAPON3,		ANIM_WEAPON3,	'3',			-1,		-1, -1},
	{"weapon 4",		"BALLOONY",			ID_WEAPON4,		ANIM_WEAPON4,	'4',			-1,		-1, -1},
	{"weapon 5",		"BETTY",			ID_WEAPON5,		ANIM_WEAPON5,	'5',			-1,		-1, -1},
	{"weapon 6",		"BOASTER",			ID_WEAPON6,		ANIM_WEAPON6,	'6',			-1,		-1, -1},
	{"weapon 7",		"SPLASHER",			ID_WEAPON7,		ANIM_WEAPON7,	'7',			-1,		-1, -1},
	{"weapon 8",		"BUBBLE.G.",		ID_WEAPON8,		ANIM_WEAPON8,	'8',			-1,		-1, -1},
	{"weapon 9",		"IMPERiUS",			ID_WEAPON9,		ANIM_WEAPON9,	'9',			-1,		-1, -1},

	{"+attack", 		"attack",			ID_ATTACK,		ANIM_ATTACK,	K_CTRL,			-1,		-1, -1},
	{"weapprev",		"prev weapon",		ID_WEAPPREV,	ANIM_IDLE,		'[',			-1,		-1, -1},
	{"weapnext", 		"next weapon",		ID_WEAPNEXT,	ANIM_IDLE,		']',			-1,		-1, -1},
	{"+button3", 		"gesture",			ID_GESTURE,		ANIM_GESTURE,	K_MOUSE3,		-1,		-1, -1},
	{"messagemode", 	"chat",				ID_CHAT,		ANIM_CHAT,		't',			-1,		-1, -1},
	{"messagemode2", 	"chat - team",		ID_CHAT2,		ANIM_CHAT,		-1,				-1,		-1, -1},
/*	{"messagemode3", 	"chat - target",	ID_CHAT3,		ANIM_CHAT,		-1,				-1,		-1, -1},
	{"messagemode4", 	"chat - attacker",	ID_CHAT4,		ANIM_CHAT,		-1,				-1,		-1, -1},*/

	{"vote yes", 		"vote yes",			ID_VOTEYES,		ANIM_IDLE,		'+',	K_KP_PLUS,		-1, -1},
	{"vote no", 		"vote no",			ID_VOTENO,		ANIM_IDLE,		'-',	K_KP_MINUS,		-1, -1},

	{"wop_music",		"music menu",	ID_MUSIC,		ANIM_IDLE,		'm',			-1,		-1, -1},
	{"wop_nextsong",	"skip to next song",ID_NEXTSONG,	ANIM_IDLE,		'n',			-1,		-1,	-1},
	{"dropCartridge",	"drop cartridge",ID_DROPCART,		ANIM_IDLE,		K_BACKSPACE,	-1,		-1, -1},
	{"toggle cg_drawServerInfos","server info",ID_SERVERINFO,ANIM_IDLE,'k',			-1,		-1, -1},
	{"toggle cg_thirdPerson","3rd person view",ID_3RDPERSON,ANIM_IDLE,	'u',			-1,		-1, -1},
	{ "ui_help gametype",	"help gametype",	ID_HELP_GAMETYPE,	ANIM_IDLE,	'g',	-1,	-1,	-1 },
	{ "ui_help item",		"help items",		ID_HELP_ITEMS,		ANIM_IDLE,	'h',	-1,	-1,	-1 },
	{"+voiprecord", "voice chat", ID_PUSH2TALK, ANIM_IDLE, 'r', -1, -1, -1},
	{"+zoom", "zoom view", ID_ZOOM, ANIM_IDLE, -1, -1, -1, -1 },

	{(char*)NULL,		(char*)NULL,		0,				0,				-1,				-1,		-1,	-1},
};

static configcvar_t g_configcvars[] =
{
	{"cl_run",			0,					0},
	{"m_pitch",			0,					0},
	{"cg_autoswitch",	0,					0},
	{"sensitivity",		0,					0},
	{"m_filter",		0,					0},
	{NULL,				0,					0}
};

static menucommon_s *g_movement_controls[] =
{  
	(menucommon_s *)&s_controls.run,            
	(menucommon_s *)&s_controls.walkforward,
	(menucommon_s *)&s_controls.backpedal,
	(menucommon_s *)&s_controls.stepleft,      
	(menucommon_s *)&s_controls.stepright,     
	(menucommon_s *)&s_controls.moveup,        
	(menucommon_s *)&s_controls.movedown,      
	(menucommon_s *)&s_controls.useitem, //
	(menucommon_s *)&s_controls.gesture, //
	NULL
};

static menucommon_s *g_weapons_controls[] = {
	(menucommon_s *)&s_controls.attack,           
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
	(menucommon_s *)&s_controls.zoom,
	NULL
};

static menucommon_s *g_misc_controls[] = {
	(menucommon_s *)&s_controls.showscores, 
	//(menucommon_s *)&s_controls.useitem,
	//(menucommon_s *)&s_controls.gesture,
	(menucommon_s *)&s_controls.chat,
	(menucommon_s *)&s_controls.chat2,
	(menucommon_s *)&s_controls.chat3,
	(menucommon_s *)&s_controls.chat4,
	(menucommon_s *)&s_controls.music,
	(menucommon_s *)&s_controls.nextSong,
	(menucommon_s *)&s_controls.dropCart,
	(menucommon_s *)&s_controls.ServerInfo,
	(menucommon_s *)&s_controls.thirdPerson,
	(menucommon_s *)&s_controls.helpGametype,
	(menucommon_s *)&s_controls.helpItems,
	(menucommon_s *)&s_controls.pushToTalk,
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
static void Controls_InitCvars( void )
{
	int				i;
	configcvar_t*	cvarptr;

	cvarptr = g_configcvars;
	for (i=0; ;i++,cvarptr++)
	{
		if (!cvarptr->name)
			break;

		// get current value
		cvarptr->value = trap_Cvar_VariableValue( cvarptr->name );

		// get default value
		trap_Cvar_Reset( cvarptr->name );
		cvarptr->defaultvalue = trap_Cvar_VariableValue( cvarptr->name );

		// restore current value
		trap_Cvar_SetValue( cvarptr->name, cvarptr->value );
	}
}

/*
=================
Controls_GetCvarDefault
=================
*/
static float Controls_GetCvarDefault( char* name )
{
	configcvar_t*	cvarptr;
	int				i;

	cvarptr = g_configcvars;
	for (i=0; ;i++,cvarptr++)
	{
		if (!cvarptr->name)
			return (0);

		if (!strcmp(cvarptr->name,name))
			break;
	}

	return (cvarptr->defaultvalue);
}

/*
=================
Controls_GetCvarValue
=================
*/
static float Controls_GetCvarValue( char* name )
{
	configcvar_t*	cvarptr;
	int				i;

	cvarptr = g_configcvars;
	for (i=0; ;i++,cvarptr++)
	{
		if (!cvarptr->name)
			return (0);

		if (!strcmp(cvarptr->name,name))
			break;
	}

	return (cvarptr->value);
}


/*
=================
Controls_UpdateModel
=================
*/
static void Controls_UpdateModel( int anim ) {
	VectorClear( s_controls.playerViewangles );
	VectorClear( s_controls.playerMoveangles );
	s_controls.playerViewangles[YAW] = 180 - 30;
	s_controls.playerMoveangles[YAW] = s_controls.playerViewangles[YAW];
	s_controls.playerLegs		     = LEGS_IDLE;
	s_controls.playerTorso			 = TORSO_STAND;
	s_controls.playerWeapon			 = -1;
	s_controls.playerChat			 = qfalse;

	switch( anim ) {
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

	UI_PlayerInfo_SetInfo( &s_controls.playerinfo, s_controls.playerLegs, s_controls.playerTorso, s_controls.playerViewangles, s_controls.playerMoveangles, s_controls.playerWeapon, s_controls.playerChat );
}


/*
=================
Controls_Update
=================
*/
static void Controls_Update( void ) {
	int		i;
	int		j;
	int		y;
	menucommon_s	**controls;
	menucommon_s	*control;

	// disable all controls in all groups
	for( i = 0; i < C_MAX; i++ ) {
		controls = g_controls[i];
		// bk001204 - parentheses
		for( j = 0;  (control = controls[j]) ; j++ ) {
			control->flags |= (QMF_HIDDEN|QMF_INACTIVE);
		}
	}

	controls = g_controls[s_controls.section];

	// enable controls in active group (and count number of items for vertical centering)
	// bk001204 - parentheses
	for( j = 0;  (control = controls[j]) ; j++ ) {
		control->flags &= ~(QMF_GRAYED|QMF_HIDDEN|QMF_INACTIVE);
	}

	// position controls
	y = 180;//160;
	// bk001204 - parentheses
	for( j = 0;	(control = controls[j]) ; j++, y += SMALLCHAR_HEIGHT ) {
		control->x      = 492;
		control->y      = y;
		control->left   = 500 - 19*SMALLCHAR_WIDTH;
		control->right  = 500 + 21*SMALLCHAR_WIDTH;
		control->top    = y;
		control->bottom = y + SMALLCHAR_HEIGHT;
	}

	if( s_controls.waitingforkey ) {
		// disable everybody
		for( i = 0; i < s_controls.menu.nitems; i++ ) {
			((menucommon_s*)(s_controls.menu.items[i]))->flags |= QMF_GRAYED;
		}

		// enable action item
		((menucommon_s*)(s_controls.menu.items[s_controls.menu.cursor]))->flags &= ~QMF_GRAYED;

		// don't gray out player's name
		s_controls.name.generic.flags &= ~QMF_GRAYED;

		return;
	}

	// enable everybody
	for( i = 0; i < s_controls.menu.nitems; i++ ) {
		((menucommon_s*)(s_controls.menu.items[i]))->flags &= ~QMF_GRAYED;
	}

	// makes sure flags are right on the group selection controls
	s_controls.looking.generic.flags  &= ~(QMF_GRAYED|QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
	s_controls.movement.generic.flags &= ~(QMF_GRAYED|QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
	s_controls.weapons.generic.flags  &= ~(QMF_GRAYED|QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
	s_controls.misc.generic.flags     &= ~(QMF_GRAYED|QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);

	s_controls.looking.generic.flags  |= QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.movement.generic.flags |= QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.weapons.generic.flags  |= QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.misc.generic.flags     |= QMF_HIGHLIGHT_IF_FOCUS;

	// set buttons
	switch( s_controls.section ) {
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
	if(		0 == UI_GetCvarInt("cl_voip") 
		||	0 == UI_GetCvarInt("s_useOpenAL")
		||	1 == UI_GetCvarInt("cl_voipUseVAD")
		)
	{
		s_controls.pushToTalk.generic.flags |= QMF_GRAYED;
	}
}


/*
=================
Controls_DrawKeyBinding
=================
*/
static void Controls_DrawKeyBinding( void *self )
{
	menuaction_s*	a;
	int				x;
	int				y;
	int				b1;
	int				b2;
	qboolean		c;
	char			name[32];
	char			name2[32];

	a = (menuaction_s*) self;

	x =	a->generic.x;
	y = a->generic.y;

	c = (Menu_ItemAtCursor( a->generic.parent ) == a);

	b1 = g_bindings[a->generic.id].bind1;
	if (b1 == -1)
		strcpy(name,"???");
	else
	{
		trap_Key_KeynumToStringBuf( b1, name, 32 );
		Q_strupr(name);

		b2 = g_bindings[a->generic.id].bind2;
		if (b2 != -1)
		{
			trap_Key_KeynumToStringBuf( b2, name2, 32 );
			Q_strupr(name2);

			strcat( name, " or " );
			strcat( name, name2 );
		}
	}

	if (c)
	{
		UI_FillRect( a->generic.left, a->generic.top, a->generic.right-a->generic.left+1, a->generic.bottom-a->generic.top+1, listbar_color ); 

		UI_DrawString( x - SMALLCHAR_WIDTH, y, g_bindings[a->generic.id].label, UI_RIGHT|UI_SMALLFONT, text_color_highlight );
		UI_DrawString( x + SMALLCHAR_WIDTH, y, name, UI_LEFT|UI_SMALLFONT|UI_PULSE, text_color_highlight );

		if (s_controls.waitingforkey)
		{
			UI_DrawChar( x, y, '=', UI_CENTER|UI_BLINK|UI_SMALLFONT, text_color_highlight);
			UI_DrawString(500, SCREEN_HEIGHT * 0.85, "Waiting for new key ... ESCAPE to cancel", UI_SMALLFONT|UI_CENTER|UI_PULSE, colorWhite );
		}
		else
		{
			UI_DrawChar( x, y, 13, UI_CENTER|UI_BLINK|UI_SMALLFONT, text_color_highlight);
			UI_DrawString(500, SCREEN_HEIGHT * 0.83, "Press ENTER or CLICK to change", UI_SMALLFONT|UI_CENTER, colorWhite );
			UI_DrawString(500, SCREEN_HEIGHT * 0.87, "Press BACKSPACE to clear", UI_SMALLFONT|UI_CENTER, colorWhite );
		}
	}
	else
	{
		if (a->generic.flags & QMF_GRAYED)
		{
			UI_DrawString( x - SMALLCHAR_WIDTH, y, g_bindings[a->generic.id].label, UI_RIGHT|UI_SMALLFONT, text_color_disabled );
			UI_DrawString( x + SMALLCHAR_WIDTH, y, name, UI_LEFT|UI_SMALLFONT, text_color_disabled );
		}
		else
		{
			UI_DrawString( x - SMALLCHAR_WIDTH, y, g_bindings[a->generic.id].label, UI_RIGHT|UI_SMALLFONT, controls_binding_color );
			UI_DrawString( x + SMALLCHAR_WIDTH, y, name, UI_LEFT|UI_SMALLFONT, controls_binding_color );
		}
	}
}

/*
=================
Controls_StatusBar
=================
*/
static void Controls_StatusBar( void *self )
{
	UI_DrawString(500, SCREEN_HEIGHT * 0.85, "Use Arrow Keys or CLICK to change", UI_SMALLFONT|UI_CENTER, colorWhite );
}


#if 0
/*
=================
Controls_DrawPlayer
=================
*/
static void Controls_DrawPlayer( void *self ) {
	menubitmap_s	*b;
	char			buf[MAX_QPATH];

	trap_Cvar_VariableStringBuffer( "model", buf, sizeof( buf ) );
	if ( strcmp( buf, s_controls.playerModel ) != 0 ) {
		UI_PlayerInfo_SetModel( &s_controls.playerinfo, buf );
		strcpy( s_controls.playerModel, buf );
		Controls_UpdateModel( ANIM_IDLE );
	}

	b = (menubitmap_s*) self;
	UI_DrawPlayer( b->generic.x, b->generic.y, b->width, b->height, &s_controls.playerinfo, uis.realtime/2 );
}
#endif

/*
=================
Controls_GetKeyAssignment
=================
*/
static void Controls_GetKeyAssignment (char *command, int *twokeys)
{
	int		count;
	int		j;
	char	b[256];

	twokeys[0] = twokeys[1] = -1;
	count = 0;

	for ( j = 0; j < 256; j++ )
	{
		trap_Key_GetBindingBuf( j, b, 256 );
		if ( *b == 0 ) {
			continue;
		}
		if ( !Q_stricmp( b, command ) ) {
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
static void Controls_GetConfig( void )
{
	int		i;
	int		twokeys[2];
	bind_t*	bindptr;

	// put the bindings into a local store
	bindptr = g_bindings;

	// iterate each command, get its numeric binding
	for (i=0; ;i++,bindptr++)
	{
		if (!bindptr->label)
			break;

		Controls_GetKeyAssignment(bindptr->command, twokeys);

		bindptr->bind1 = twokeys[0];
		bindptr->bind2 = twokeys[1];
	}

	s_controls.invertmouse.curvalue  = Controls_GetCvarValue( "m_pitch" ) < 0;
	s_controls.smoothmouse.curvalue  = UI_ClampCvar( 0, 1, Controls_GetCvarValue( "m_filter" ) );
	s_controls.autoswitch.curvalue   = UI_ClampCvar( 0, 1, Controls_GetCvarValue( "cg_autoswitch" ) );
	s_controls.sensitivity.curvalue  = UI_ClampCvar( 2, 30, Controls_GetCvarValue( "sensitivity" ) );
}

/*
=================
Controls_SetConfig
=================
*/
static void Controls_SetConfig( void )
{
	int		i;
	bind_t*	bindptr;

	// set the bindings from the local store
	bindptr = g_bindings;

	// iterate each command, get its numeric binding
	for (i=0; ;i++,bindptr++)
	{
		if (!bindptr->label)
			break;

		if (bindptr->bind1 != -1)
		{	
			trap_Key_SetBinding( bindptr->bind1, bindptr->command );

			if (bindptr->bind2 != -1)
				trap_Key_SetBinding( bindptr->bind2, bindptr->command );
		}
	}

	if ( s_controls.invertmouse.curvalue )
		trap_Cvar_SetValue( "m_pitch", -fabs( trap_Cvar_VariableValue( "m_pitch" ) ) );
	else
		trap_Cvar_SetValue( "m_pitch", fabs( trap_Cvar_VariableValue( "m_pitch" ) ) );

	trap_Cvar_SetValue( "m_filter", s_controls.smoothmouse.curvalue );
	trap_Cvar_SetValue( "cg_autoswitch", s_controls.autoswitch.curvalue );
	trap_Cvar_SetValue( "sensitivity", s_controls.sensitivity.curvalue );
	trap_Cmd_ExecuteText( EXEC_APPEND, "in_restart\n" );
}

void SetDefaultBinds_onUnusedKeys()
{
	bind_t*	bindptr;
	int i;
	char buff[8];

	if(!wop_AutoBindUnusedKeys.integer)
		return;

	bindptr = g_bindings;
	for (i=0; ;i++,bindptr++)
	{
		if (!bindptr->label)
			break;

		if(bindptr->defaultbind1==-1) continue;
		buff[0]=0;		trap_Key_GetBindingBuf(bindptr->defaultbind1,buff,8);
		if(buff[0]==0)	trap_Key_SetBinding(bindptr->defaultbind1,bindptr->command);

		if(bindptr->defaultbind2==-1) continue;
		buff[0]=0;		trap_Key_GetBindingBuf(bindptr->defaultbind2,buff,8);
		if(buff[0]==0)	trap_Key_SetBinding(bindptr->defaultbind2,bindptr->command);

	}
}

/*
=================
Controls_SetDefaults
=================
*/
static void Controls_SetDefaults( void )
{
	int	i;
	bind_t*	bindptr;

	// set the bindings from the local store
	bindptr = g_bindings;

	// iterate each command, set its default binding
	for (i=0; ;i++,bindptr++)
	{
		if (!bindptr->label)
			break;

		bindptr->bind1 = bindptr->defaultbind1;
		bindptr->bind2 = bindptr->defaultbind2;
	}

	s_controls.invertmouse.curvalue  = Controls_GetCvarDefault( "m_pitch" ) < 0;
	s_controls.smoothmouse.curvalue  = Controls_GetCvarDefault( "m_filter" );
	s_controls.autoswitch.curvalue   = Controls_GetCvarDefault( "cg_autoswitch" );
	s_controls.sensitivity.curvalue  = Controls_GetCvarDefault( "sensitivity" );
}

/*
=================
Controls_MenuKey
=================
*/
static sfxHandle_t Controls_MenuKey( int key )
{
	int			id;
	int			i;
	qboolean	found;
	bind_t*		bindptr;
	found = qfalse;

	if (!s_controls.waitingforkey)
	{
		switch (key)
		{
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
	}
	else
	{
		if (key & K_CHAR_FLAG)
			goto ignorekey;

		switch (key)
		{
			case K_ESCAPE:
				s_controls.waitingforkey = qfalse;
				Controls_Update();
				return menu_null_sound;
	
			case '`':
				goto ignorekey;
		}
	}

	s_controls.changesmade = qtrue;
	
	if (key != -1)
	{
		// remove from any other bind
		bindptr = g_bindings;
		for (i=0; ;i++,bindptr++)
		{
			if (!bindptr->label)	
				break;

			if (bindptr->bind2 == key)
				bindptr->bind2 = -1;

			if (bindptr->bind1 == key)
			{
				bindptr->bind1 = bindptr->bind2;	
				bindptr->bind2 = -1;
			}
		}
	}

	// assign key to local store
	id      = ((menucommon_s*)(s_controls.menu.items[s_controls.menu.cursor]))->id;
	bindptr = g_bindings;
	for (i=0; ;i++,bindptr++)
	{
		if (!bindptr->label)	
			break;
		
		if (bindptr->id == id)
		{
			found = qtrue;
			if (key == -1)
			{
				if( bindptr->bind1 != -1 ) {
					trap_Key_SetBinding( bindptr->bind1, "" );
					bindptr->bind1 = -1;
				}
				if( bindptr->bind2 != -1 ) {
					trap_Key_SetBinding( bindptr->bind2, "" );
					bindptr->bind2 = -1;
				}
			}
			else if (bindptr->bind1 == -1) {
				bindptr->bind1 = key;
			}
			else if (bindptr->bind1 != key && bindptr->bind2 == -1) {
				bindptr->bind2 = key;
			}
			else
			{
				trap_Key_SetBinding( bindptr->bind1, "" );
				trap_Key_SetBinding( bindptr->bind2, "" );
				bindptr->bind1 = key;
				bindptr->bind2 = -1;
			}						
			break;
		}
	}				
		
	s_controls.waitingforkey = qfalse;

	if (found)
	{	
		Controls_Update();
		return menu_null_sound;
	}

ignorekey:
	return Menu_DefaultKey( &s_controls.menu, key );
}

/*
=================
Controls_ResetDefaults_Action
=================
*/
static void Controls_ResetDefaults_Action( qboolean result ) {
	if( !result ) {
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
static void Controls_ResetDefaults_Draw( void ) {
	UI_DrawProportionalString( SCREEN_WIDTH/2, 356 + PROP_HEIGHT * 0, "WARNING: This will reset all", UI_CENTER|UI_SMALLFONT, color_yellow );
	UI_DrawProportionalString( SCREEN_WIDTH/2, 356 + PROP_HEIGHT * 1, "controls to their default values.", UI_CENTER|UI_SMALLFONT, color_yellow );
}

/*
=================
Controls_MenuEvent
=================
*/
static void Controls_MenuEvent( void* ptr, int event )
{
	switch (((menucommon_s*)ptr)->id)
	{
		case ID_MOVEMENT:
			if (event == QM_ACTIVATED)
			{
				s_controls.section = C_MOVEMENT; 
				Controls_Update();
			}
			break;

		case ID_LOOKING:
			if (event == QM_ACTIVATED)
			{
				s_controls.section = C_LOOKING; 
				Controls_Update();
			}
			break;

		case ID_WEAPONS:
			if (event == QM_ACTIVATED)
			{
				s_controls.section = C_WEAPONS; 
				Controls_Update();
			}
			break;

		case ID_MISC:
			if (event == QM_ACTIVATED)
			{
				s_controls.section = C_MISC; 
				Controls_Update();
			}
			break;

		case ID_DEFAULTS:
			if (event == QM_ACTIVATED)
			{
				UI_ConfirmMenu( "SET TO DEFAULTS?", Controls_ResetDefaults_Draw, Controls_ResetDefaults_Action );
			}
			break;

		case ID_BACK:
			if (event == QM_ACTIVATED)
			{
				if (s_controls.changesmade)
					Controls_SetConfig();
				UI_PopMenu();
			}
			break;

		case ID_SAVEANDEXIT:
			if (event == QM_ACTIVATED)
			{
				Controls_SetConfig();
				UI_PopMenu();
			}
			break;

		case ID_EXIT:
			if (event == QM_ACTIVATED)
			{
				UI_PopMenu();
			}
			break;

		case ID_MOUSESPEED:
		case ID_INVERTMOUSE:
		case ID_SMOOTHMOUSE:
		case ID_AUTOSWITCH:
			if (event == QM_ACTIVATED)
			{
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
static void Controls_ActionEvent( void* ptr, int event )
{
	if (event == QM_LOSTFOCUS)
	{
		Controls_UpdateModel( ANIM_IDLE );
	}
	else if (event == QM_GOTFOCUS)
	{
		Controls_UpdateModel( g_bindings[((menucommon_s*)ptr)->id].anim );
	}
	else if ((event == QM_ACTIVATED) && !s_controls.waitingforkey)
	{
		s_controls.waitingforkey = 1;
		Controls_Update();
	}
}

#if 0
/*
=================
Controls_InitModel
=================
*/
static void Controls_InitModel( void )
{
	memset( &s_controls.playerinfo, 0, sizeof(playerInfo_t) );

	Controls_UpdateModel( ANIM_IDLE );
}

/*
=================
Controls_InitWeapons
=================
*/
static void Controls_InitWeapons( void ) {
	gitem_t *	item;

	for ( item = bg_itemlist + 1 ; item->classname ; item++ ) {
		if ( item->giType != IT_WEAPON ) {
			continue;
		}
		trap_R_RegisterModel( item->world_model[0] );
	}
}
#endif

/*
=================
Controls_MenuInit
=================
*/
static void Controls_MenuInit( void )
{
	// zero set all our globals
	memset( &s_controls, 0 ,sizeof(controls_t) );

	Controls_Cache();

	s_controls.menu.key        = Controls_MenuKey;
	s_controls.menu.wrapAround = qtrue;
	s_controls.menu.fullscreen = qtrue;
	s_controls.menu.noPushSelect	= qtrue;
	s_controls.menu.bgparts		= BGP_CONTROLBG|BGP_SIMPLEBG;

	s_controls.looking.generic.type		= MTYPE_BITMAP;
	s_controls.looking.generic.name		= LOOK0;
	s_controls.looking.generic.flags	= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.looking.generic.x		= 412;
	s_controls.looking.generic.y		= 40;
	s_controls.looking.generic.id		= ID_LOOKING;
	s_controls.looking.generic.callback	= Controls_MenuEvent;
	s_controls.looking.width			= 75;
	s_controls.looking.height			= 40;
	s_controls.looking.focuspic			= LOOK1;
	s_controls.looking.focuspicinstead	= qtrue;

	// +/-... Rumrücken für Ente
	s_controls.movement.generic.type	= MTYPE_BITMAP;
	s_controls.movement.generic.name	= MOVEMENT0;
	s_controls.movement.generic.flags	= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.movement.generic.x		= 509-9;
	s_controls.movement.generic.y		= 50-5;
	s_controls.movement.generic.id		= ID_MOVEMENT;
	s_controls.movement.generic.callback= Controls_MenuEvent;
	s_controls.movement.width			= 80;
	s_controls.movement.height			= 40;
	s_controls.movement.focuspic		= MOVEMENT1;
	s_controls.movement.focuspicinstead	= qtrue;

	s_controls.weapons.generic.type		= MTYPE_BITMAP;
	s_controls.weapons.generic.name		= WEAPONS0;
	s_controls.weapons.generic.flags	= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.weapons.generic.x		= 480-2;
	s_controls.weapons.generic.y		= 105-15;
	s_controls.weapons.generic.id		= ID_WEAPONS;
	s_controls.weapons.generic.callback	= Controls_MenuEvent;
	s_controls.weapons.width			= 105;
	s_controls.weapons.height			= 40;
	s_controls.weapons.focuspic			= WEAPONS1;
	s_controls.weapons.focuspicinstead	= qtrue;

	s_controls.misc.generic.type		= MTYPE_BITMAP;
	s_controls.misc.generic.name		= MISC0;
	s_controls.misc.generic.flags		= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	s_controls.misc.generic.x			= 398+3;
	s_controls.misc.generic.y			= 88-9;
	s_controls.misc.generic.id			= ID_MISC;
	s_controls.misc.generic.callback	= Controls_MenuEvent;
	s_controls.misc.width				= 75;
	s_controls.misc.height				= 40;
	s_controls.misc.focuspic			= MISC1;
	s_controls.misc.focuspicinstead		= qtrue;


	s_controls.back.generic.type		= MTYPE_BITMAP;
	s_controls.back.generic.name		= ART_BACK0;
	s_controls.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_controls.back.generic.x			= 549;
	s_controls.back.generic.y			= 440;
	s_controls.back.generic.id			= ID_BACK;
	s_controls.back.generic.callback	= Controls_MenuEvent;
	s_controls.back.width				= 80;
	s_controls.back.height				= 40;
	s_controls.back.focuspic			= ART_BACK1;
	s_controls.back.focuspicinstead		= qtrue;

	s_controls.walkforward.generic.type	     = MTYPE_ACTION;
	s_controls.walkforward.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.walkforward.generic.callback  = Controls_ActionEvent;
	s_controls.walkforward.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.walkforward.generic.id 	     = ID_FORWARD;

	s_controls.backpedal.generic.type	   = MTYPE_ACTION;
	s_controls.backpedal.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.backpedal.generic.callback  = Controls_ActionEvent;
	s_controls.backpedal.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.backpedal.generic.id 	   = ID_BACKPEDAL;

	s_controls.stepleft.generic.type	  = MTYPE_ACTION;
	s_controls.stepleft.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.stepleft.generic.callback  = Controls_ActionEvent;
	s_controls.stepleft.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.stepleft.generic.id 		  = ID_MOVELEFT;

	s_controls.stepright.generic.type	   = MTYPE_ACTION;
	s_controls.stepright.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.stepright.generic.callback  = Controls_ActionEvent;
	s_controls.stepright.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.stepright.generic.id        = ID_MOVERIGHT;

	s_controls.moveup.generic.type	    = MTYPE_ACTION;
	s_controls.moveup.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.moveup.generic.callback  = Controls_ActionEvent;
	s_controls.moveup.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.moveup.generic.id        = ID_MOVEUP;

	s_controls.movedown.generic.type	  = MTYPE_ACTION;
	s_controls.movedown.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.movedown.generic.callback  = Controls_ActionEvent;
	s_controls.movedown.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.movedown.generic.id        = ID_MOVEDOWN;

	s_controls.run.generic.type	     = MTYPE_ACTION;
	s_controls.run.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.run.generic.callback  = Controls_ActionEvent;
	s_controls.run.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.run.generic.id        = ID_SPEED;

	s_controls.chainsaw.generic.type	  = MTYPE_ACTION;
	s_controls.chainsaw.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.chainsaw.generic.callback  = Controls_ActionEvent;
	s_controls.chainsaw.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.chainsaw.generic.id        = ID_WEAPON1;

	s_controls.machinegun.generic.type	    = MTYPE_ACTION;
	s_controls.machinegun.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.machinegun.generic.callback  = Controls_ActionEvent;
	s_controls.machinegun.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.machinegun.generic.id        = ID_WEAPON2;

	s_controls.shotgun.generic.type	     = MTYPE_ACTION;
	s_controls.shotgun.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.shotgun.generic.callback  = Controls_ActionEvent;
	s_controls.shotgun.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.shotgun.generic.id        = ID_WEAPON3;

	s_controls.grenadelauncher.generic.type	     = MTYPE_ACTION;
	s_controls.grenadelauncher.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.grenadelauncher.generic.callback  = Controls_ActionEvent;
	s_controls.grenadelauncher.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.grenadelauncher.generic.id        = ID_WEAPON4;

	s_controls.rocketlauncher.generic.type	    = MTYPE_ACTION;
	s_controls.rocketlauncher.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.rocketlauncher.generic.callback  = Controls_ActionEvent;
	s_controls.rocketlauncher.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.rocketlauncher.generic.id        = ID_WEAPON5;

	s_controls.lightning.generic.type	   = MTYPE_ACTION;
	s_controls.lightning.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.lightning.generic.callback  = Controls_ActionEvent;
	s_controls.lightning.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.lightning.generic.id        = ID_WEAPON6;

	s_controls.railgun.generic.type	     = MTYPE_ACTION;
	s_controls.railgun.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.railgun.generic.callback  = Controls_ActionEvent;
	s_controls.railgun.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.railgun.generic.id        = ID_WEAPON7;

	s_controls.plasma.generic.type	    = MTYPE_ACTION;
	s_controls.plasma.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.plasma.generic.callback  = Controls_ActionEvent;
	s_controls.plasma.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.plasma.generic.id        = ID_WEAPON8;

	s_controls.bfg.generic.type	     = MTYPE_ACTION;
	s_controls.bfg.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.bfg.generic.callback  = Controls_ActionEvent;
	s_controls.bfg.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.bfg.generic.id        = ID_WEAPON9;

	s_controls.attack.generic.type	    = MTYPE_ACTION;
	s_controls.attack.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.attack.generic.callback  = Controls_ActionEvent;
	s_controls.attack.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.attack.generic.id        = ID_ATTACK;

	s_controls.prevweapon.generic.type	    = MTYPE_ACTION;
	s_controls.prevweapon.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.prevweapon.generic.callback  = Controls_ActionEvent;
	s_controls.prevweapon.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.prevweapon.generic.id        = ID_WEAPPREV;

	s_controls.nextweapon.generic.type	    = MTYPE_ACTION;
	s_controls.nextweapon.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.nextweapon.generic.callback  = Controls_ActionEvent;
	s_controls.nextweapon.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.nextweapon.generic.id        = ID_WEAPNEXT;

	s_controls.useitem.generic.type	     = MTYPE_ACTION;
	s_controls.useitem.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.useitem.generic.callback  = Controls_ActionEvent;
	s_controls.useitem.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.useitem.generic.id        = ID_USEITEM;
	s_controls.useitem.generic.toolTip	 = "Press this key in game to throw killerducks / use floater / deploy bambam and place boomies (you must look at the ground to place a boomie).";

	s_controls.showscores.generic.type	    = MTYPE_ACTION;
	s_controls.showscores.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.showscores.generic.callback  = Controls_ActionEvent;
	s_controls.showscores.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.showscores.generic.id        = ID_SHOWSCORES;

	s_controls.invertmouse.generic.type      = MTYPE_RADIOBUTTON;
	s_controls.invertmouse.generic.flags	 = QMF_SMALLFONT;
	s_controls.invertmouse.generic.x	     = SCREEN_WIDTH/2;
	s_controls.invertmouse.generic.name	     = "invert mouse";
	s_controls.invertmouse.generic.id        = ID_INVERTMOUSE;
	s_controls.invertmouse.generic.callback  = Controls_MenuEvent;
	s_controls.invertmouse.generic.statusbar = Controls_StatusBar;
	s_controls.invertmouse.generic.toolTip	 = "Switch on to invert the directions of your mouse (not recommended).";

	s_controls.smoothmouse.generic.type      = MTYPE_RADIOBUTTON;
	s_controls.smoothmouse.generic.flags	 = QMF_SMALLFONT;
	s_controls.smoothmouse.generic.x	     = SCREEN_WIDTH/2;
	s_controls.smoothmouse.generic.name	     = "smooth mouse";
	s_controls.smoothmouse.generic.id        = ID_SMOOTHMOUSE;
	s_controls.smoothmouse.generic.callback  = Controls_MenuEvent;
	s_controls.smoothmouse.generic.statusbar = Controls_StatusBar;
	s_controls.smoothmouse.generic.toolTip	 = "Switch on to smooth out movement when using the mouse to look around.";

	s_controls.autoswitch.generic.type      = MTYPE_RADIOBUTTON;
	s_controls.autoswitch.generic.flags	    = QMF_SMALLFONT;
	s_controls.autoswitch.generic.x	        = SCREEN_WIDTH/2;
	s_controls.autoswitch.generic.name	    = "auto switch weapons";
	s_controls.autoswitch.generic.id        = ID_AUTOSWITCH;
	s_controls.autoswitch.generic.callback  = Controls_MenuEvent;
	s_controls.autoswitch.generic.statusbar = Controls_StatusBar;
	s_controls.autoswitch.generic.toolTip	= "If enabled, your character will automatically switch to the weapon that you run into to pick up.";

	s_controls.sensitivity.generic.type	     = MTYPE_SLIDER;
	s_controls.sensitivity.generic.x		 = SCREEN_WIDTH/2;
	s_controls.sensitivity.generic.flags	 = QMF_SMALLFONT;
	s_controls.sensitivity.generic.name	     = "mouse speed";
	s_controls.sensitivity.generic.id 	     = ID_MOUSESPEED;
	s_controls.sensitivity.generic.callback  = Controls_MenuEvent;
	s_controls.sensitivity.minvalue		     = 2;
	s_controls.sensitivity.maxvalue		     = 30;
	s_controls.sensitivity.generic.statusbar = Controls_StatusBar;

	s_controls.zoom.generic.type	  = MTYPE_ACTION;
	s_controls.zoom.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.zoom.generic.callback  = Controls_ActionEvent;
	s_controls.zoom.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.zoom.generic.id        = ID_ZOOM;
	s_controls.zoom.generic.toolTip	  = "Press this key in game to enable zoom mode for weapons that support it.";

	s_controls.gesture.generic.type	     = MTYPE_ACTION;
	s_controls.gesture.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.gesture.generic.callback  = Controls_ActionEvent;
	s_controls.gesture.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.gesture.generic.id        = ID_GESTURE;
	s_controls.gesture.generic.toolTip	 = "Press this key in game to taunt other players.";

	s_controls.chat.generic.type	  = MTYPE_ACTION;
	s_controls.chat.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.chat.generic.callback  = Controls_ActionEvent;
	s_controls.chat.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.chat.generic.id        = ID_CHAT;
	s_controls.chat.generic.toolTip		= "Press this key in game before typing a message to text chat to all players in the game.";

	s_controls.chat2.generic.type	   = MTYPE_ACTION;
	s_controls.chat2.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.chat2.generic.callback  = Controls_ActionEvent;
	s_controls.chat2.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.chat2.generic.id        = ID_CHAT2;

	s_controls.chat3.generic.type	   = MTYPE_ACTION;
	s_controls.chat3.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.chat3.generic.callback  = Controls_ActionEvent;
	s_controls.chat3.generic.ownerdraw = Controls_DrawKeyBinding;
//	s_controls.chat3.generic.id        = ID_CHAT3;
	s_controls.chat3.generic.id        = ID_VOTEYES;
	s_controls.chat3.generic.toolTip   = "Cast a yes vote to a vote in progress (votes appear top left of the screen).";

	s_controls.chat4.generic.type	   = MTYPE_ACTION;
	s_controls.chat4.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.chat4.generic.callback  = Controls_ActionEvent;
	s_controls.chat4.generic.ownerdraw = Controls_DrawKeyBinding;
//	s_controls.chat4.generic.id        = ID_CHAT4;
	s_controls.chat4.generic.id        = ID_VOTENO;
	s_controls.chat4.generic.toolTip   = "Cast a no vote to a vote in progress (votes appear top left of the screen).";

	s_controls.music.generic.type	   = MTYPE_ACTION;
	s_controls.music.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.music.generic.callback  = Controls_ActionEvent;
	s_controls.music.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.music.generic.id        = ID_MUSIC;

	s_controls.nextSong.generic.type	   = MTYPE_ACTION;
	s_controls.nextSong.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.nextSong.generic.callback  = Controls_ActionEvent;
	s_controls.nextSong.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.nextSong.generic.id        = ID_NEXTSONG;

	s_controls.dropCart.generic.type	  = MTYPE_ACTION;
	s_controls.dropCart.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.dropCart.generic.callback  = Controls_ActionEvent;
	s_controls.dropCart.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.dropCart.generic.id        = ID_DROPCART;

	s_controls.thirdPerson.generic.type	    = MTYPE_ACTION;
	s_controls.thirdPerson.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.thirdPerson.generic.callback  = Controls_ActionEvent;
	s_controls.thirdPerson.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.thirdPerson.generic.id        = ID_3RDPERSON;
	s_controls.thirdPerson.generic.toolTip	 = "Press this key in game to switch to third person view, press again to switch back to first person view.";

	s_controls.pushToTalk.generic.type		= MTYPE_ACTION;
	s_controls.pushToTalk.generic.flags		= QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.pushToTalk.generic.callback	= Controls_ActionEvent;
	s_controls.pushToTalk.generic.ownerdraw	= Controls_DrawKeyBinding;
	s_controls.pushToTalk.generic.id		= ID_PUSH2TALK;
	s_controls.pushToTalk.generic.toolTip	= "Press and hold this key to talk to other players through your microphone. This is not required if your microphone setting is set to open.";

	s_controls.ServerInfo.generic.type	    = MTYPE_ACTION;
	s_controls.ServerInfo.generic.flags     = QMF_LEFT_JUSTIFY|QMF_GRAYED|QMF_HIDDEN;
	s_controls.ServerInfo.generic.callback  = Controls_ActionEvent;
	s_controls.ServerInfo.generic.ownerdraw = Controls_DrawKeyBinding;
	s_controls.ServerInfo.generic.id        = ID_SERVERINFO;
	s_controls.ServerInfo.generic.toolTip	= "Press this key in game to display server settings information.";

	s_controls.helpGametype.generic.type		= MTYPE_ACTION;
	s_controls.helpGametype.generic.flags		= ( QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN );
	s_controls.helpGametype.generic.callback	= Controls_ActionEvent;
	s_controls.helpGametype.generic.ownerdraw	= Controls_DrawKeyBinding;
	s_controls.helpGametype.generic.id			= ID_HELP_GAMETYPE;
	s_controls.helpGametype.generic.toolTip		= "Press this key in game to see a basic description of the rules of the gametype set.";

	s_controls.helpItems.generic.type		= MTYPE_ACTION;
	s_controls.helpItems.generic.flags		= ( QMF_LEFT_JUSTIFY | QMF_GRAYED | QMF_HIDDEN );
	s_controls.helpItems.generic.callback	= Controls_ActionEvent;
	s_controls.helpItems.generic.ownerdraw	= Controls_DrawKeyBinding;
	s_controls.helpItems.generic.id			= ID_HELP_ITEMS;
	s_controls.helpItems.generic.toolTip	= "Press this key in game to see information on what the weapons / powerups / items and icons are.";

	Menu_AddItem( &s_controls.menu, &s_controls.looking );
	Menu_AddItem( &s_controls.menu, &s_controls.movement );
	Menu_AddItem( &s_controls.menu, &s_controls.weapons );
	Menu_AddItem( &s_controls.menu, &s_controls.misc );

	Menu_AddItem( &s_controls.menu, &s_controls.sensitivity );
	Menu_AddItem( &s_controls.menu, &s_controls.smoothmouse );
	Menu_AddItem( &s_controls.menu, &s_controls.invertmouse );
	Menu_AddItem( &s_controls.menu, &s_controls.zoom );
	
	Menu_AddItem( &s_controls.menu, &s_controls.run );
	Menu_AddItem( &s_controls.menu, &s_controls.walkforward );
	Menu_AddItem( &s_controls.menu, &s_controls.backpedal );
	Menu_AddItem( &s_controls.menu, &s_controls.stepleft );
	Menu_AddItem( &s_controls.menu, &s_controls.stepright );
	Menu_AddItem( &s_controls.menu, &s_controls.moveup );
	Menu_AddItem( &s_controls.menu, &s_controls.movedown );

	Menu_AddItem( &s_controls.menu, &s_controls.attack );
	Menu_AddItem( &s_controls.menu, &s_controls.nextweapon );
	Menu_AddItem( &s_controls.menu, &s_controls.prevweapon );
	Menu_AddItem( &s_controls.menu, &s_controls.autoswitch );
	Menu_AddItem( &s_controls.menu, &s_controls.chainsaw );
	Menu_AddItem( &s_controls.menu, &s_controls.machinegun );
	Menu_AddItem( &s_controls.menu, &s_controls.shotgun );
	Menu_AddItem( &s_controls.menu, &s_controls.grenadelauncher );
	Menu_AddItem( &s_controls.menu, &s_controls.rocketlauncher );
	Menu_AddItem( &s_controls.menu, &s_controls.lightning );
	Menu_AddItem( &s_controls.menu, &s_controls.railgun );
	Menu_AddItem( &s_controls.menu, &s_controls.plasma );
	Menu_AddItem( &s_controls.menu, &s_controls.bfg );

	Menu_AddItem( &s_controls.menu, &s_controls.showscores );
	Menu_AddItem( &s_controls.menu, &s_controls.useitem );
	Menu_AddItem( &s_controls.menu, &s_controls.gesture );
	Menu_AddItem( &s_controls.menu, &s_controls.chat );
	Menu_AddItem( &s_controls.menu, &s_controls.chat2 );
	Menu_AddItem( &s_controls.menu, &s_controls.chat3 );
	Menu_AddItem( &s_controls.menu, &s_controls.chat4 );
	Menu_AddItem( &s_controls.menu, &s_controls.music );
	Menu_AddItem( &s_controls.menu, &s_controls.nextSong );
	Menu_AddItem( &s_controls.menu, &s_controls.dropCart );
	Menu_AddItem( &s_controls.menu, &s_controls.ServerInfo );
	Menu_AddItem( &s_controls.menu, &s_controls.thirdPerson );
	Menu_AddItem( &s_controls.menu, &s_controls.helpGametype );
	Menu_AddItem( &s_controls.menu, &s_controls.helpItems );
	Menu_AddItem( &s_controls.menu, &s_controls.pushToTalk );

	Menu_AddItem( &s_controls.menu, &s_controls.back );

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
void Controls_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );

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
void UI_ControlsMenu( void ) {
	Controls_MenuInit();
	UI_PushMenu( &s_controls.menu );
}
