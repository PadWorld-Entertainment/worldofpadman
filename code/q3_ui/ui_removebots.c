// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

REMOVE BOTS MENU

=======================================================================
*/


#include "ui_local.h"


#define ART_ARROWUP0		"menu/mods/arrowup0.tga"
#define ART_ARROWUP1		"menu/mods/arrowup1.tga"
#define ART_ARROWDOWN0		"menu/mods/arrowdown0.tga"
#define ART_ARROWDOWN1		"menu/mods/arrowdown1.tga"

#define NUM_BOTS			6

#define ID_UP				10
#define ID_DOWN				11
#define ID_DELETE			12
#define ID_BACK				13
#define ID_REMOVEALL		14
#define ID_BOTNAME0			20
#define ID_BOTNAME1			21
#define ID_BOTNAME2			22
#define ID_BOTNAME3			23
#define ID_BOTNAME4			24
#define ID_BOTNAME5			25
#define ID_BOTNAME6			26

typedef struct {
	menuframework_s	menu;

	menutext_s		banner;
	menubitmap_s	background;

	menubitmap_s	arrows;
	menubitmap_s	up;
	menubitmap_s	down;

	menutext_s		bots[NUM_BOTS];
	menubitmap_s	bot_pics[NUM_BOTS];

	menutext_s			delete;
	menutext_s			removeAll;
	menutext_s			back;
	menubitmap1024s_s	arrowup;
	menubitmap1024s_s	arrowdown;

	int				numBots;
	int				baseBotNum;
	int				selectedBotNum;
	char			boticonnames[NUM_BOTS][MAX_QPATH];
	char			botnames[NUM_BOTS][32];

	int				botClientNums[MAX_BOTS];
} removeBotsMenuInfo_t;

static removeBotsMenuInfo_t	removeBotsMenuInfo;


/*
=================
UI_RemoveBotsMenu_SetBotNames
=================
*/
static void UI_RemoveBotsMenu_SetBotNames( void ) {
	int		n;
	char	info[MAX_INFO_STRING];

	for ( n = 0; (n < NUM_BOTS) && (removeBotsMenuInfo.baseBotNum + n < removeBotsMenuInfo.numBots); n++ ) {
		trap_GetConfigString( CS_PLAYERS + removeBotsMenuInfo.botClientNums[removeBotsMenuInfo.baseBotNum + n], info, MAX_INFO_STRING );
		Q_strncpyz( removeBotsMenuInfo.botnames[n], Info_ValueForKey( info, "n" ), sizeof(removeBotsMenuInfo.botnames[n]) );
		Q_CleanStr( removeBotsMenuInfo.botnames[n] );

		UI_ModelIcon(Info_ValueForKey( info, "model" ),removeBotsMenuInfo.boticonnames[n],MAX_QPATH);
		removeBotsMenuInfo.bot_pics[n].shader=0;
	}

}


/*
=================
UI_RemoveBotsMenu_DeleteEvent
=================
*/
static void UI_RemoveBotsMenu_DeleteEvent( void* ptr, int event ) {
	if (event != QM_ACTIVATED) {
		return;
	}

	if(((menucommon_s*)ptr)->id == ID_REMOVEALL)
	{
		int i;
		for(i=0;i<removeBotsMenuInfo.numBots;++i)
			trap_Cmd_ExecuteText( EXEC_APPEND, va("clientkick %i\n", removeBotsMenuInfo.botClientNums[i]) );
	}
	else
		trap_Cmd_ExecuteText( EXEC_APPEND, va("clientkick %i\n", removeBotsMenuInfo.botClientNums[removeBotsMenuInfo.baseBotNum + removeBotsMenuInfo.selectedBotNum]) );

	uis.musicbool = 2;
	trap_Cvar_Set("cl_paused","0");
}


/*
=================
UI_RemoveBotsMenu_BotEvent
=================
*/
static void UI_RemoveBotsMenu_BotEvent( void* ptr, int event ) {
	if (event != QM_ACTIVATED) {
		return;
	}

	removeBotsMenuInfo.bots[removeBotsMenuInfo.selectedBotNum].color = colorBlack;
	removeBotsMenuInfo.bots[removeBotsMenuInfo.selectedBotNum].focuscolor = colorDkLilac;
	removeBotsMenuInfo.selectedBotNum = ((menucommon_s*)ptr)->id - ID_BOTNAME0;
	removeBotsMenuInfo.bots[removeBotsMenuInfo.selectedBotNum].color = colorDkOrange;
	removeBotsMenuInfo.bots[removeBotsMenuInfo.selectedBotNum].focuscolor = color_orange;
}


/*
=================
UI_RemoveAddBotsMenu_BackEvent
=================
*/
static void UI_RemoveBotsMenu_BackEvent( void* ptr, int event ) {
	if (event != QM_ACTIVATED) {
		return;
	}
	UI_PopMenu();
}


/*
=================
UI_RemoveBotsMenu_UpEvent
=================
*/
static void UI_RemoveBotsMenu_UpEvent( void* ptr, int event ) {
	if (event != QM_ACTIVATED) {
		return;
	}

	if( removeBotsMenuInfo.baseBotNum > 0 ) {
		removeBotsMenuInfo.baseBotNum--;
		UI_RemoveBotsMenu_SetBotNames();
	}
}


/*
=================
UI_RemoveBotsMenu_DownEvent
=================
*/
static void UI_RemoveBotsMenu_DownEvent( void* ptr, int event ) {
	if (event != QM_ACTIVATED) {
		return;
	}

	if( removeBotsMenuInfo.baseBotNum + NUM_BOTS < removeBotsMenuInfo.numBots ) {
		removeBotsMenuInfo.baseBotNum++;
		UI_RemoveBotsMenu_SetBotNames();
	}
}


/*
=================
UI_RemoveBotsMenu_GetBots
=================
*/
static void UI_RemoveBotsMenu_GetBots( void ) {
	int		numPlayers;
	int		isBot;
	int		n;
	char	info[MAX_INFO_STRING];

	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
	numPlayers = atoi( Info_ValueForKey( info, "sv_maxclients" ) );
	removeBotsMenuInfo.numBots = 0;

	for( n = 0; n < numPlayers; n++ ) {
		trap_GetConfigString( CS_PLAYERS + n, info, MAX_INFO_STRING );

		isBot = atoi( Info_ValueForKey( info, "skill" ) );
		if( !isBot ) {
			continue;
		}

		removeBotsMenuInfo.botClientNums[removeBotsMenuInfo.numBots] = n;
		removeBotsMenuInfo.numBots++;
	}
}


/*
=================
UI_RemoveBots_Cache
=================
*/
void UI_RemoveBots_Cache( void ) {
	trap_R_RegisterShaderNoMip(ART_ARROWUP0);
	trap_R_RegisterShaderNoMip(ART_ARROWUP1);
	trap_R_RegisterShaderNoMip(ART_ARROWDOWN0);
	trap_R_RegisterShaderNoMip(ART_ARROWDOWN1);
}

/*
#######################

  UI_RemoveBotsMenu_Draw

#######################
*/
static void UI_RemoveBotsMenu_Draw( void ) {
	UI_DrawIngameBG();
	UI_DrawProportionalString( 320, 110, "REMOVE BOTS",UI_CENTER|UI_SMALLFONT,color_black);

	Menu_Draw( &removeBotsMenuInfo.menu );
}

/*
#######################

  RemoveBotsMenu_Key

#######################
*/
static sfxHandle_t RemoveBotsMenu_Key( int key ) {
	switch ( key ) {
	case K_MWHEELUP:
	case K_PGUP:
		UI_RemoveBotsMenu_UpEvent( &removeBotsMenuInfo.arrowup, QM_ACTIVATED );
		break;
	case K_MWHEELDOWN:
	case K_PGDN:
		UI_RemoveBotsMenu_DownEvent( &removeBotsMenuInfo.arrowdown, QM_ACTIVATED );
		break;

	case 'r':
	case 'R':
		UI_RemoveBotsMenu_DeleteEvent( &removeBotsMenuInfo.delete, QM_ACTIVATED );
		break;
	}

	return Menu_DefaultKey( &removeBotsMenuInfo.menu, key );
}

/*
=================
UI_RemoveBotsMenu_Init
=================
*/
static void UI_RemoveBotsMenu_Init( void ) {
	int		n;
	int		count;
	int		y;

	memset( &removeBotsMenuInfo, 0 ,sizeof(removeBotsMenuInfo) );
	removeBotsMenuInfo.menu.fullscreen = qfalse;
	removeBotsMenuInfo.menu.wrapAround = qtrue;
	removeBotsMenuInfo.menu.draw = UI_RemoveBotsMenu_Draw;
	removeBotsMenuInfo.menu.key = RemoveBotsMenu_Key;

	UI_RemoveBots_Cache();

	UI_RemoveBotsMenu_GetBots();
	UI_RemoveBotsMenu_SetBotNames();

	count = removeBotsMenuInfo.numBots < NUM_BOTS ? removeBotsMenuInfo.numBots : NUM_BOTS;

	removeBotsMenuInfo.arrowup.generic.type			= MTYPE_BITMAP1024S;
	removeBotsMenuInfo.arrowup.x					= 638;
	removeBotsMenuInfo.arrowup.y					= 236;
	removeBotsMenuInfo.arrowup.w					= 29;
	removeBotsMenuInfo.arrowup.h					= 74;
	removeBotsMenuInfo.arrowup.shader				= trap_R_RegisterShaderNoMip(ART_ARROWUP0);
	removeBotsMenuInfo.arrowup.mouseovershader		= trap_R_RegisterShaderNoMip(ART_ARROWUP1);
	removeBotsMenuInfo.arrowup.generic.callback		= UI_RemoveBotsMenu_UpEvent;
	removeBotsMenuInfo.arrowup.generic.id			= ID_UP;

	removeBotsMenuInfo.arrowdown.generic.type		= MTYPE_BITMAP1024S;
	removeBotsMenuInfo.arrowdown.x					= 638;
	removeBotsMenuInfo.arrowdown.y					= 406-74;
	removeBotsMenuInfo.arrowdown.w					= 29;//38
	removeBotsMenuInfo.arrowdown.h					= 74;//98
	removeBotsMenuInfo.arrowdown.shader				= trap_R_RegisterShaderNoMip(ART_ARROWDOWN0);
	removeBotsMenuInfo.arrowdown.mouseovershader	= trap_R_RegisterShaderNoMip(ART_ARROWDOWN1);
	removeBotsMenuInfo.arrowdown.generic.callback	= UI_RemoveBotsMenu_DownEvent;
	removeBotsMenuInfo.arrowdown.generic.id			= ID_DOWN;

	for( n = 0, y = 140; n < count; n++, y += 20 ) {
		removeBotsMenuInfo.bot_pics[n].generic.type		= MTYPE_BITMAP;
		removeBotsMenuInfo.bot_pics[n].generic.name		= removeBotsMenuInfo.boticonnames[n];
		removeBotsMenuInfo.bot_pics[n].generic.flags	= QMF_INACTIVE;
		removeBotsMenuInfo.bot_pics[n].generic.x		= 250-20;
		removeBotsMenuInfo.bot_pics[n].generic.y		= y;
		removeBotsMenuInfo.bot_pics[n].width  			= 20;
		removeBotsMenuInfo.bot_pics[n].height  			= 20;

		removeBotsMenuInfo.bots[n].generic.type			= MTYPE_TEXTS;
		removeBotsMenuInfo.bots[n].fontHeight			= 20.0f;
		removeBotsMenuInfo.bots[n].generic.flags		= QMF_LEFT_JUSTIFY;//|QMF_PULSEIFFOCUS;
		removeBotsMenuInfo.bots[n].generic.id			= ID_BOTNAME0 + n;
		removeBotsMenuInfo.bots[n].generic.x			= 250;
		removeBotsMenuInfo.bots[n].generic.y			= y;
		removeBotsMenuInfo.bots[n].generic.callback		= UI_RemoveBotsMenu_BotEvent;
		removeBotsMenuInfo.bots[n].string				= removeBotsMenuInfo.botnames[n];
		removeBotsMenuInfo.bots[n].color				= colorBlack;
		removeBotsMenuInfo.bots[n].focuscolor			= colorDkLilac;
		removeBotsMenuInfo.bots[n].style				= UI_LEFT|UI_SMALLFONT;
	}

	removeBotsMenuInfo.delete.generic.type		= MTYPE_TEXTS;
	removeBotsMenuInfo.delete.fontHeight		= 24.0f;
	removeBotsMenuInfo.delete.generic.flags		= QMF_RIGHT_JUSTIFY;//|QMF_PULSEIFFOCUS;
	removeBotsMenuInfo.delete.generic.callback	= UI_RemoveBotsMenu_DeleteEvent;
	removeBotsMenuInfo.delete.generic.id		= ID_DELETE;
	removeBotsMenuInfo.delete.generic.x			= 380;
	removeBotsMenuInfo.delete.generic.y			= 280;
	removeBotsMenuInfo.delete.string			= "REMOVE";
	removeBotsMenuInfo.delete.style				= UI_SMALLFONT|UI_RIGHT;
	removeBotsMenuInfo.delete.color				= color_black;
	removeBotsMenuInfo.delete.focuscolor		= color_orange;
	
	removeBotsMenuInfo.removeAll.generic.type	= MTYPE_TEXTS;
	removeBotsMenuInfo.removeAll.fontHeight		= 14.4f;
	removeBotsMenuInfo.removeAll.generic.flags	= QMF_RIGHT_JUSTIFY;//|QMF_PULSEIFFOCUS;
	removeBotsMenuInfo.removeAll.generic.callback= UI_RemoveBotsMenu_DeleteEvent;
	removeBotsMenuInfo.removeAll.generic.id		= ID_REMOVEALL;
	removeBotsMenuInfo.removeAll.generic.x		= 380;
	removeBotsMenuInfo.removeAll.generic.y		= 280+26;
	removeBotsMenuInfo.removeAll.string			= "REMOVE ALL";
	removeBotsMenuInfo.removeAll.style			= UI_SMALLFONT|UI_RIGHT;
	removeBotsMenuInfo.removeAll.color			= color_black;
	removeBotsMenuInfo.removeAll.focuscolor		= color_orange;

	removeBotsMenuInfo.back.generic.type		= MTYPE_TEXTS;
	removeBotsMenuInfo.back.fontHeight			= 16.0f;
//	removeBotsMenuInfo.back.generic.flags		= QMF_PULSEIFFOCUS;
	removeBotsMenuInfo.back.generic.callback	= UI_RemoveBotsMenu_BackEvent;
	removeBotsMenuInfo.back.generic.id			= ID_BACK;
	removeBotsMenuInfo.back.generic.x			= 245;
	removeBotsMenuInfo.back.generic.y			= 315;
	removeBotsMenuInfo.back.string				= "BACK";
	removeBotsMenuInfo.back.style				= UI_SMALLFONT;
	removeBotsMenuInfo.back.color				= color_black;
	removeBotsMenuInfo.back.focuscolor			= color_orange;
	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.arrowup );
	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.arrowdown );

	for( n = 0; n < count; n++ ) {
		Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.bots[n] );
		Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.bot_pics[n] );
	}
	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.delete );
	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.removeAll );
	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.back );

	removeBotsMenuInfo.baseBotNum = 0;
	removeBotsMenuInfo.selectedBotNum = 0;
	removeBotsMenuInfo.bots[0].color = colorDkOrange;
	removeBotsMenuInfo.bots[0].focuscolor = color_orange;
}


/*
=================
UI_RemoveBotsMenu
=================
*/
void UI_RemoveBotsMenu( void ) {
	UI_RemoveBotsMenu_Init();
	UI_PushMenu( &removeBotsMenuInfo.menu );
}
