// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

TEAM ORDERS MENU

=======================================================================
*/


#include "ui_local.h"

#define ART_FRAME		"menu/ingame/bg.tga"

#define ID_LIST_BOTS		10
#define ID_LIST_CTF_ORDERS	11
#define ID_LIST_TEAM_ORDERS	12

#define ORDER_HEIGHT 24

typedef struct {
	menuframework_s	menu;

	menulist_s		list;

	menutext_s		back;

	int				gametype;
	int				numBots;
	int				selectedBot;
	char			*bots[9];
	char			botNames[9][16];
} teamOrdersMenuInfo_t;

static teamOrdersMenuInfo_t	teamOrdersMenuInfo;

#define NUM_CTF_ORDERS		7
static const char *ctfOrders[] = {
	"I am the leader",
	"Defend the base",
	"Follow me",
	"Get enemy lolly",
	"Camp here",
	"Report",
	"I relinquish command",
	NULL
};
static const char *ctfMessages[] = {
	"I am the leader",
	"%s defend the base",
	"%s follow me",
	"%s get enemy lolly",
	"%s camp here",
	"%s report",
	"I stop being the leader",
	NULL
};

#define NUM_TEAM_ORDERS		7 //raute: 6 -> 7
static const char *teamOrders[] = {
	"I am the leader",
	"Follow me",
	"Roam",
	"Camp here",
	"Report",
	"Give cartridges",
	"I relinquish Command",
	NULL
};
static const char *teamMessages[] = {
	"I am the leader",
	"%s follow me",
	"%s roam",
	"%s camp here",
	"%s report",
	"%s give your cartridges to me",
	"I stop being the leader",
	NULL
};


/*
===============
UI_TeamOrdersMenu_BackEvent
===============
*/
static void UI_TeamOrdersMenu_BackEvent( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}
	UI_PopMenu();
}


/*
===============
UI_TeamOrdersMenu_SetList
===============
*/
static void UI_TeamOrdersMenu_SetList( int id ) {
	switch( id ) {
	default:
	case ID_LIST_BOTS:
		teamOrdersMenuInfo.list.generic.id = id;
		teamOrdersMenuInfo.list.numitems = teamOrdersMenuInfo.numBots;
		teamOrdersMenuInfo.list.itemnames = (const char **)teamOrdersMenuInfo.bots;
		 break;

	case ID_LIST_CTF_ORDERS:
		teamOrdersMenuInfo.list.generic.id = id;
		teamOrdersMenuInfo.list.numitems = NUM_CTF_ORDERS;
		teamOrdersMenuInfo.list.itemnames = ctfOrders;
		break;

	case ID_LIST_TEAM_ORDERS:
		teamOrdersMenuInfo.list.generic.id = id;
		teamOrdersMenuInfo.list.numitems = NUM_TEAM_ORDERS;
		teamOrdersMenuInfo.list.itemnames = teamOrders;
		break;
	}

	teamOrdersMenuInfo.list.generic.bottom = teamOrdersMenuInfo.list.generic.top + teamOrdersMenuInfo.list.numitems * ORDER_HEIGHT;
}


/*
=================
UI_TeamOrdersMenu_Key
=================
*/
sfxHandle_t UI_TeamOrdersMenu_Key( int key ) {
	menulist_s	*l;
	int	x;
	int	y;
	int	index;

	l = (menulist_s	*)Menu_ItemAtCursor( &teamOrdersMenuInfo.menu );
	if( l != &teamOrdersMenuInfo.list ) {
		return Menu_DefaultKey( &teamOrdersMenuInfo.menu, key );
	}

	switch( key ) {
		case K_MOUSE1:
			x = l->generic.left;
			y = l->generic.top;
			if( UI_CursorInRect( x, y, l->generic.right - x, l->generic.bottom - y ) ) {
				index = (uis.cursory - y) / ORDER_HEIGHT;
				l->oldvalue = l->curvalue;
				l->curvalue = index;

				if( l->generic.callback ) {
					l->generic.callback( l, QM_ACTIVATED );
					return menu_move_sound;
				}
			}
			return menu_null_sound;

		case K_KP_UPARROW:
		case K_UPARROW:
			l->oldvalue = l->curvalue;

			if( l->curvalue == 0 ) {
				l->curvalue = l->numitems - 1;
			}
			else {
				l->curvalue--;
			}
			return menu_move_sound;

		case K_KP_DOWNARROW:
		case K_DOWNARROW:
			l->oldvalue = l->curvalue;

			if( l->curvalue == l->numitems - 1 ) {
				l->curvalue = 0;;
			}
			else {
				l->curvalue++;
			}
			return menu_move_sound;
	}

	return Menu_DefaultKey( &teamOrdersMenuInfo.menu, key );
}


/*
=================
UI_TeamOrdersMenu_ListDraw
=================
*/
static void UI_TeamOrdersMenu_ListDraw( void *self ) {
	menulist_s	*l;
	int			x;
	int			y;
	int			i;
	float		*color;
	qboolean	hasfocus;
	int			style;

	l = (menulist_s *)self;

	hasfocus = (l->generic.parent->cursor == l->generic.menuPosition);

	x =	320;//l->generic.x;
	y =	l->generic.y;
	for( i = 0; i < l->numitems; i++ ) {
		style = UI_LEFT|UI_SMALLFONT|UI_CENTER;
		if( i == l->curvalue ) {
			color = color_orange;
			if( hasfocus ) {
				style |= UI_PULSE;
			}
		}
		else {
			color = color_black;
		}

		UI_DrawStringNS(x,y,l->itemnames[i],style,18.0f,color);
		y += ORDER_HEIGHT;
	}
}


/*
===============
UI_TeamOrdersMenu_ListEvent
===============
*/
static void UI_TeamOrdersMenu_ListEvent( void *ptr, int event ) {
	int		id;
	int		selection;
	char	message[256];

	if (event != QM_ACTIVATED)
		return;

	id = ((menulist_s *)ptr)->generic.id;
	selection = ((menulist_s *)ptr)->curvalue;

	if( id == ID_LIST_BOTS ) {
		teamOrdersMenuInfo.selectedBot = selection;
		if( teamOrdersMenuInfo.gametype == GT_CTF ) {
			UI_TeamOrdersMenu_SetList( ID_LIST_CTF_ORDERS );
		}
		else {
			UI_TeamOrdersMenu_SetList( ID_LIST_TEAM_ORDERS );
		}
		return;
	}

	if( id == ID_LIST_CTF_ORDERS ) {
		Com_sprintf( message, sizeof(message), ctfMessages[selection], teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.selectedBot] );
	}
	else {
		Com_sprintf( message, sizeof(message), teamMessages[selection], teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.selectedBot] );
	}

	trap_Cmd_ExecuteText( EXEC_APPEND, va( "say_team \"%s\"\n", message ) );
	UI_PopMenu();
}


/*
===============
UI_TeamOrdersMenu_BuildBotList
===============
*/
static void UI_TeamOrdersMenu_BuildBotList( void ) {
	uiClientState_t	cs;
	int		numPlayers;
	int		isBot;
	int		n;
	char	playerTeam;
	char	botTeam;
	char	info[MAX_INFO_STRING];

	for( n = 0; n < 9; n++ ) {
		teamOrdersMenuInfo.bots[n] = teamOrdersMenuInfo.botNames[n];
	}

	trap_GetClientState( &cs );

	Q_strncpyz( teamOrdersMenuInfo.botNames[0], "Everyone", 16 );
	teamOrdersMenuInfo.numBots = 1;

	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
	numPlayers = atoi( Info_ValueForKey( info, "sv_maxclients" ) );
	teamOrdersMenuInfo.gametype = atoi( Info_ValueForKey( info, "g_gametype" ) );

	trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
	playerTeam = *Info_ValueForKey( info, "t" );

	for( n = 0; n < numPlayers && teamOrdersMenuInfo.numBots < 9; n++ ) {
		trap_GetConfigString( CS_PLAYERS + n, info, MAX_INFO_STRING );

		if( n == cs.clientNum ) {
//			playerTeam = *Info_ValueForKey( info, "t" );
			continue;
		}

		isBot = atoi( Info_ValueForKey( info, "skill" ) );
		if( !isBot ) {
			continue;
		}

		botTeam = *Info_ValueForKey( info, "t" );
		if( botTeam != playerTeam ) {
			continue;
		}

		Q_strncpyz( teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.numBots], Info_ValueForKey( info, "n" ), 16 );
		Q_CleanStr( teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.numBots] );
		teamOrdersMenuInfo.numBots++;
	}
}

/*
#######################
  TeamOrderMenu_Draw
#######################
*/
static void TeamOrderMenu_Draw( void ) {
	UI_DrawIngameBG();
	UI_DrawProportionalString( 320, 110, "TEAM ORDERS",UI_CENTER|UI_SMALLFONT,color_black);

	// standard menu drawing
	Menu_Draw( &teamOrdersMenuInfo.menu );
}

/*
===============
UI_TeamOrdersMenu_Init
===============
*/
static void UI_TeamOrdersMenu_Init( void ) {
	UI_TeamOrdersMenu_Cache();

	memset( &teamOrdersMenuInfo, 0, sizeof(teamOrdersMenuInfo) );
	teamOrdersMenuInfo.menu.fullscreen = qfalse;
	teamOrdersMenuInfo.menu.key = UI_TeamOrdersMenu_Key;

	UI_TeamOrdersMenu_BuildBotList();

	teamOrdersMenuInfo.menu.draw = TeamOrderMenu_Draw;

	teamOrdersMenuInfo.list.generic.type		= MTYPE_SCROLLLIST;
	teamOrdersMenuInfo.list.generic.flags		= QMF_PULSEIFFOCUS;
	teamOrdersMenuInfo.list.generic.ownerdraw	= UI_TeamOrdersMenu_ListDraw;
	teamOrdersMenuInfo.list.generic.callback	= UI_TeamOrdersMenu_ListEvent;
	teamOrdersMenuInfo.list.generic.x			= 320-64;
	teamOrdersMenuInfo.list.generic.y			= 140;//120;

	teamOrdersMenuInfo.back.generic.type		= MTYPE_TEXTS;
	teamOrdersMenuInfo.back.fontHeight			= 16.0f;
	teamOrdersMenuInfo.back.generic.flags		= QMF_PULSEIFFOCUS;
	teamOrdersMenuInfo.back.generic.callback	= UI_TeamOrdersMenu_BackEvent;
	teamOrdersMenuInfo.back.generic.x			= 245;
	teamOrdersMenuInfo.back.generic.y			= 315;
	teamOrdersMenuInfo.back.string				= "BACK";
	teamOrdersMenuInfo.back.style				= UI_SMALLFONT;
	teamOrdersMenuInfo.back.color				= color_black;

	Menu_AddItem( &teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.list );
	Menu_AddItem( &teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.back );

	teamOrdersMenuInfo.list.generic.left = 220;
	teamOrdersMenuInfo.list.generic.top = teamOrdersMenuInfo.list.generic.y;
	teamOrdersMenuInfo.list.generic.right = 420;
	UI_TeamOrdersMenu_SetList( ID_LIST_BOTS );
}


/*
=================
UI_TeamOrdersMenu_Cache
=================
*/
void UI_TeamOrdersMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_FRAME );
}


/*
===============
UI_TeamOrdersMenu
===============
*/
void UI_TeamOrdersMenu( void ) {
	UI_TeamOrdersMenu_Init();
	UI_PushMenu( &teamOrdersMenuInfo.menu );
}


/*
===============
UI_TeamOrdersMenu_f
===============
*/
void UI_TeamOrdersMenu_f( void ) {
	uiClientState_t	cs;
	char	info[MAX_INFO_STRING];
	int		team;

	// make sure it's a team game
	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
	teamOrdersMenuInfo.gametype = atoi( Info_ValueForKey( info, "g_gametype" ) );
	if( teamOrdersMenuInfo.gametype < GT_TEAM ) {
		return;
	}

	// not available to spectators
	trap_GetClientState( &cs );
	trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
	team = atoi( Info_ValueForKey( info, "t" ) );
	if( team == TEAM_SPECTATOR ) {
		return;
	}

	UI_TeamOrdersMenu();
}
