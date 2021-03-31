/*
=======================================================================

HELP MENU

=======================================================================
*/


#include "ui_local.h"

#define ID_PREV			10
#define ID_NEXT			11

#define HELPMENU_PATH	"menu/help/"

#define ART_PREV0	HELPMENU_PATH"arrowup0"
#define ART_PREV1	HELPMENU_PATH"arrowup1"
#define ART_NEXT0	HELPMENU_PATH"arrowdown0"
#define ART_NEXT1	HELPMENU_PATH"arrowdown1"
#define	HM_PN_W		45
#define HM_PN_H		27
#define HM_PN_X0	266
#define HM_PN_X1	329
#define HM_PN_Y		350

#define HMI_GAMETYPE_W	373
#define HMI_GAMETYPE_H	395
#define HMI_GAMETYPE_X	150
#define HMI_GAMETYPE_Y	0

#define HMI_ITEM_W	499
#define HMI_ITEM_H	433
#define HMI_ITEM_X	92
#define HMI_ITEM_Y	0

typedef struct {
	menuframework_s		menu;

	qhandle_t		img;
	float			width, height, x, y;
	
	menubitmap_s	prev;
	menubitmap_s	next;
} helpMenuInfo_t;

static helpMenuInfo_t	helpMenuInfo;


// TODO: Use short gametype names from bg
// Order needs to match gametype_t!
static const char *HELP_GAMETYPE[] = {
	"help_ffa",
//	"help_tourney",
//	"help_single", // ""
	"help_syc",
	"help_lps",
	"help_team_ffa",
	"help_ctl",
	"help_team_syc",
	"help_bb",
	"help_unknown" // must be the last entry!
};
static const unsigned int NUM_HELP_GAMETYPE = ( sizeof( HELP_GAMETYPE ) / sizeof( HELP_GAMETYPE[0] ) );
// Must contain an entry for each gametype_t
// Maps gametypes to array indices of HELP_GAMETYPE
static const int GAMETYPE_REMAP[] = {
	0, 	// GT_FFA
	7,	// GT_TOURNAMENT, no help
	7,	// GT_SINGLE_PLAYER, no help
	1,	// GT_SPRAY_FFA
	2,	// GT_LPS
	3,	// GT_TEAM
	4,	// GT_CTF
	5,	// GT_SPRAY
	6,	// GT_BALLOON
	7,	// GT_MAX_GAME_TYPE, invalid thus no help
};
	
static const char *HELP_ITEM[] = {
	"help_powerups",
	"help_weapons"
};
static const unsigned int NUM_HELP_ITEM = ( sizeof( HELP_ITEM ) / sizeof( HELP_ITEM[0] ) );


static int			helpMax, helpMin	= 0;
static int			helpIndex			= 0;
static const char   **helpList 			= NULL;


/*
===============
UI_HelpMenu_AdjustButtons
===============
*/
static void UI_HelpMenu_AdjustButtons( void ) {
	helpMenuInfo.prev.generic.flags &= ~QMF_GRAYED;
	helpMenuInfo.next.generic.flags &= ~QMF_GRAYED;

	if ( helpIndex <= 0 ) {
		helpMenuInfo.prev.generic.flags |= QMF_GRAYED;
	}
	if ( helpIndex >= helpMax ) {
		helpMenuInfo.next.generic.flags |= QMF_GRAYED;
	}

	helpMenuInfo.img = trap_R_RegisterShaderNoMip( va( HELPMENU_PATH"%s", helpList[helpIndex] ) );
}


/*
===============
UI_HelpMenu_Event
===============
*/
static void UI_HelpMenu_Event( void *ptr, int event ) {
	if ( event != QM_ACTIVATED ) {
		return;
	}

	switch ( ( (menucommon_s*)ptr )->id ) {
		case ID_PREV:
			helpIndex--;
			break;

		case ID_NEXT:
		default:
			helpIndex++;
			break;
	}

	if ( helpIndex < helpMin ) {
		helpIndex = helpMin;
	}
	else if ( helpIndex > helpMax ) {
		helpIndex = helpMax;
	}

	UI_HelpMenu_AdjustButtons();
}


/*
=================
UI_HelpMenu_Key
=================
*/
static sfxHandle_t UI_HelpMenu_Key( int key ) {
	switch( key ) {
		case K_KP_LEFTARROW:
		case K_LEFTARROW:
			UI_HelpMenu_Event( &helpMenuInfo.prev, QM_ACTIVATED );
			break;

		case K_KP_RIGHTARROW:
		case K_RIGHTARROW:
			UI_HelpMenu_Event( &helpMenuInfo.next, QM_ACTIVATED );
			break;

		// TODO: Close menu with second keypress. How to get original key (from "ui_help x" bind(s))?
		//       Key_GetKey seems to be in cgame only
		// FIXME: Hardcoded like in ui_music until there's a better approach
		case 'g':
		case 'h':
			UI_PopMenu();
			return 0;
		
		default:
			break;
	}

	return Menu_DefaultKey( &helpMenuInfo.menu, key );
}


/*
===============
UI_DrawHelpMenuImg
===============
*/
static void UI_DrawHelpMenuImg( void ) {
	//if ( helpMenuInfo.img ) {
		UI_DrawHandlePic( helpMenuInfo.x, helpMenuInfo.y, helpMenuInfo.width, helpMenuInfo.height, helpMenuInfo.img );
	//}
}


/*
===============
UI_HelpMenu_Draw
===============
*/
static void UI_HelpMenu_Draw( void ) {
	UI_DrawHelpMenuImg();

	// standard menu drawing
	Menu_Draw( &helpMenuInfo.menu );
}


/*
===============
UI_HelpMenu_SetTopic
===============
*/
static void UI_HelpMenu_SetTopic( void ) {
	char	*arg;
	char	info[MAX_INFO_STRING];
	int		gametype;

	if ( trap_Argc() > 1 ) {
		arg = UI_Argv( 1 );

		if ( Q_stricmp( arg, "gametype" ) == 0 ) {
			helpList	= HELP_GAMETYPE;

			trap_GetConfigString( CS_SERVERINFO, info, sizeof( info ) );
			gametype = atoi( Info_ValueForKey( info, "g_gametype" ) );
			if ( ( gametype < 0 ) || ( gametype > GT_MAX_GAME_TYPE ) ) {
				gametype = GT_MAX_GAME_TYPE;
			}
			
			helpIndex	= GAMETYPE_REMAP[gametype];
			helpMin		= helpIndex;
			helpMax		= helpIndex;

			helpMenuInfo.prev.generic.flags = QMF_HIDDEN;
			helpMenuInfo.next.generic.flags = QMF_HIDDEN;

			helpMenuInfo.width	= HMI_GAMETYPE_W;
			helpMenuInfo.height	= HMI_GAMETYPE_H;
			helpMenuInfo.x		= HMI_GAMETYPE_X;
			helpMenuInfo.y		= HMI_GAMETYPE_Y;
		}
		else {
			helpMin		= 0;
			helpMax		= ( NUM_HELP_ITEM - 1 );
			helpList	= HELP_ITEM;
			helpIndex	= helpMin;
			helpMenuInfo.width	= HMI_ITEM_W;
			helpMenuInfo.height	= HMI_ITEM_H;
			helpMenuInfo.x		= HMI_ITEM_X;
			helpMenuInfo.y		= HMI_ITEM_Y;
		}
	}
	else {
		helpMin		= 0;
		helpMax		= ( NUM_HELP_ITEM - 1 );
		helpList	= HELP_ITEM;
		helpIndex	= helpMax;
		helpMenuInfo.width	= HMI_ITEM_W;
		helpMenuInfo.height	= HMI_ITEM_H;
		helpMenuInfo.x		= HMI_ITEM_X;
		helpMenuInfo.y		= HMI_ITEM_Y;
	}

	UI_HelpMenu_AdjustButtons();
}


/*
===============
UI_HelpMenu_Init
===============
*/
static void UI_HelpMenu_Init( void ) {
	UI_HelpMenu_Cache();

	memset( &helpMenuInfo, 0, sizeof( helpMenuInfo ) );
	helpMenuInfo.menu.fullscreen	= qfalse;
	helpMenuInfo.menu.key			= UI_HelpMenu_Key;
	helpMenuInfo.menu.draw			= UI_HelpMenu_Draw;


	helpMenuInfo.prev.generic.type		= MTYPE_BITMAP;
	helpMenuInfo.prev.generic.id		= ID_PREV;
	helpMenuInfo.prev.generic.x			= HM_PN_X0;
	helpMenuInfo.prev.generic.y			= HM_PN_Y;
	helpMenuInfo.prev.width				= HM_PN_W;
	helpMenuInfo.prev.height			= HM_PN_H;
	helpMenuInfo.prev.generic.name		= ART_PREV0;
	helpMenuInfo.prev.focuspic			= ART_PREV1;
	helpMenuInfo.prev.generic.flags		= QMF_HIGHLIGHT_IF_FOCUS;
	helpMenuInfo.prev.generic.callback	= UI_HelpMenu_Event;

	helpMenuInfo.next.generic.type		= MTYPE_BITMAP;
	helpMenuInfo.next.generic.id		= ID_NEXT;
	helpMenuInfo.next.generic.x			= HM_PN_X1;
	helpMenuInfo.next.generic.y			= HM_PN_Y;
	helpMenuInfo.next.width				= HM_PN_W;
	helpMenuInfo.next.height			= HM_PN_H;
	helpMenuInfo.next.generic.name		= ART_NEXT0;
	helpMenuInfo.next.focuspic			= ART_NEXT1;
	helpMenuInfo.next.generic.flags		= QMF_HIGHLIGHT_IF_FOCUS;
	helpMenuInfo.next.generic.callback	= UI_HelpMenu_Event;


	Menu_AddItem( &helpMenuInfo.menu, &helpMenuInfo.prev );
	Menu_AddItem( &helpMenuInfo.menu, &helpMenuInfo.next );


	UI_HelpMenu_SetTopic();
}


/*
=================
UI_HelpMenu_Cache
=================
*/
void UI_HelpMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_PREV0 );
	trap_R_RegisterShaderNoMip( ART_PREV1 );

	trap_R_RegisterShaderNoMip( ART_NEXT0 );
	trap_R_RegisterShaderNoMip( ART_NEXT1 );
}


/*
===============
UI_HelpMenu
===============
*/
void UI_HelpMenu( void ) {
	trap_Cvar_Set( "cl_paused", "1" ); // pause game and indirectly pause/stop background music

	UI_HelpMenu_Init();
	UI_PushMenu( &helpMenuInfo.menu );
}


/*
===============
UI_HelpMenu_f
===============
*/
void UI_HelpMenu_f( void ) {
	// Just a wrapper for "ui_help" command
	UI_HelpMenu();
}

