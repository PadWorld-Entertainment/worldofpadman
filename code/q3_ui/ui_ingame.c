// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

INGAME MENU

=======================================================================
*/


#include "ui_local.h"

#define INGAME_MENU_VERTICAL_SPACING	18

#define ID_TEAM					10
#define ID_ADDBOTS				11
#define ID_REMOVEBOTS			12
#define ID_SETUP				13
#define ID_SERVERINFO			14
#define ID_LEAVEARENA			15
#define ID_RESTART				16
#define ID_QUIT					17
#define ID_RESUME				18
#define ID_VOICEINGAME			19
#define ID_RECHOOSELOGO			20
#define ID_WOPMUSIC				21
#define ID_NEXTMAP				22
#define ID_CALLVOTE				23


typedef struct {
	menuframework_s	menu;
	menutext_s		team;

	menutext_s		rechooselogo;
	menutext_s		nextmap;

	menutext_s		setup;
	menutext_s		server;
	menutext_s		leave;
	menutext_s		restart;
	menutext_s		addbots;
	menutext_s		removebots;
	menutext_s		voiceingame;
//	menutext_s		teamorders;
	menutext_s		callvote;
	menutext_s		quit;
	menutext_s		resume;
} ingamemenu_t;

static ingamemenu_t	s_ingame;


/*
=================
InGame_RestartAction
=================
*/
static void InGame_RestartAction( qboolean result ) {
	if( !result ) {
		return;
	}

	UI_PopMenu();
	trap_Cmd_ExecuteText( EXEC_APPEND, "map_restart 0\n" );
}


/*
=================
InGame_QuitAction
=================
*/
static void InGame_QuitAction( qboolean result ) {
	if( !result ) {
		return;
	}
	UI_PopMenu();
	UI_CreditMenu();
}


/*
=================
InGame_Event
=================
*/
void InGame_Event( void *ptr, int notification ) {
	if( notification != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_TEAM:
		UI_TeamMainMenu();
		break;

	case ID_SETUP:
		UI_SetupMenu();
		break;

	case ID_LEAVEARENA:
		trap_Cmd_ExecuteText( EXEC_APPEND, "disconnect\n" );
		break;

	case ID_RESTART:
		UI_ConfirmMenu( "RESTART MAP?", 0, InGame_RestartAction );
		break;

	case ID_QUIT:
		UI_ConfirmMenu( "EXIT GAME?",  0, InGame_QuitAction );
		break;

	case ID_SERVERINFO:
		UI_ServerInfoMenu();
		break;

	case ID_ADDBOTS:
		UI_AddBotsMenu();
		break;

	case ID_REMOVEBOTS:
		UI_RemoveBotsMenu();
		break;

	case ID_VOICEINGAME:
		UI_VoiceIngame();
		break;

/*	case ID_TEAMORDERS:
		UI_TeamOrdersMenu();
		break;
*/
	case ID_CALLVOTE:
		UI_CallVoteMenu();
		break;

	case ID_RESUME:
		UI_PopMenu();
		break;

	case ID_WOPMUSIC:
		trap_Cmd_ExecuteText(EXEC_APPEND,"wop_music\n");//call wop-music-menu (in ui O_o) ... get cl_paused 0 ;)
		UI_PopMenu();//pop the menu ;)
		break;

	case ID_NEXTMAP:
		trap_Cmd_ExecuteText( EXEC_APPEND, "vstr nextmap\n" );
		UI_PopMenu();
		break;
	case ID_RECHOOSELOGO:
		trap_Cmd_ExecuteText(EXEC_APPEND,"rechooselogo\n");//call logomenu (in cgame)
		UI_PopMenu();//pop the menu ;)
		break;
	}
}

/*
#######################
  InGame_Draw
#######################
*/
static void InGame_Draw( void ) {
	UI_DrawIngameBG();

	// standard menu drawing
	Menu_Draw( &s_ingame.menu );
}

/*
=================
InGame_MenuInit
=================
*/
void InGame_MenuInit( void ) {
	int		y;
//	uiClientState_t	cs;
	char	info[MAX_INFO_STRING];
//	int		team;
	int		allowVote;
	qboolean isLocalServer = ( trap_Cvar_VariableValue( "sv_running" ) );
	int gametype = trap_Cvar_VariableValue( "g_gametype" );
	qboolean isSinglePlayer = ( gametype == GT_SINGLE_PLAYER);
	qboolean isSyC = ( gametype == GT_SPRAYFFA || gametype == GT_SPRAY );

	trap_GetConfigString(CS_SERVERINFO, info, MAX_INFO_STRING);   
	allowVote = atoi( Info_ValueForKey( info,"g_allowVote" ) );

	memset( &s_ingame, 0 ,sizeof(ingamemenu_t) );

	InGame_Cache();

	s_ingame.menu.wrapAround = qtrue;
	s_ingame.menu.fullscreen = qfalse;
	s_ingame.menu.noPushSelect = qtrue;

	s_ingame.menu.draw		 = InGame_Draw;

	y = 110;
	s_ingame.team.generic.type			= MTYPE_TEXTS;
	s_ingame.team.fontHeight			= 20.0f;
	s_ingame.team.generic.flags			= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_ingame.team.generic.x				= 320;
	s_ingame.team.generic.y				= y;
	s_ingame.team.generic.id			= ID_TEAM;
	s_ingame.team.generic.callback		= InGame_Event; 
	s_ingame.team.string				= "START";
	s_ingame.team.color					= color_black;
	s_ingame.team.focuscolor			= color_orange;
	s_ingame.team.style					= UI_CENTER|UI_SMALLFONT;

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.addbots.generic.type			= MTYPE_TEXTS;
	s_ingame.addbots.fontHeight			= 20.0f;
	s_ingame.addbots.generic.flags		= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_ingame.addbots.generic.x			= 320;
	s_ingame.addbots.generic.y			= y;
	s_ingame.addbots.generic.id			= ID_ADDBOTS;
	s_ingame.addbots.generic.callback	= InGame_Event; 
	s_ingame.addbots.string				= "ADD BOTS";
	s_ingame.addbots.color				= color_black;
	s_ingame.addbots.focuscolor			= color_orange;
	s_ingame.addbots.style				= UI_CENTER|UI_SMALLFONT;
	if( !isLocalServer || !trap_Cvar_VariableValue( "bot_enable" ) || isSinglePlayer) {
		s_ingame.addbots.generic.flags |= QMF_GRAYED;
	}

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.removebots.generic.type			= MTYPE_TEXTS;
	s_ingame.removebots.fontHeight			= 20.0f;
	s_ingame.removebots.generic.flags		= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_ingame.removebots.generic.x			= 320;
	s_ingame.removebots.generic.y			= y;
	s_ingame.removebots.generic.id			= ID_REMOVEBOTS;
	s_ingame.removebots.generic.callback	= InGame_Event; 
	s_ingame.removebots.string				= "REMOVE BOTS";
	s_ingame.removebots.color				= color_black;
	s_ingame.removebots.focuscolor			= color_orange;
	s_ingame.removebots.style				= UI_CENTER|UI_SMALLFONT;
	if( !isLocalServer || !trap_Cvar_VariableValue( "bot_enable" ) || isSinglePlayer) {
		s_ingame.removebots.generic.flags |= QMF_GRAYED;
	}

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.voiceingame.generic.type		= MTYPE_TEXTS;
	s_ingame.voiceingame.fontHeight			= 20.0f;
	s_ingame.voiceingame.generic.flags		= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_ingame.voiceingame.generic.x			= 320;
	s_ingame.voiceingame.generic.y			= y;
	s_ingame.voiceingame.generic.id			= ID_VOICEINGAME;
	s_ingame.voiceingame.generic.callback	= InGame_Event; 
	s_ingame.voiceingame.string				= "VOICE CHAT";
	s_ingame.voiceingame.color				= color_black;
	s_ingame.voiceingame.focuscolor			= color_orange;
	s_ingame.voiceingame.style				= UI_CENTER|UI_SMALLFONT;
	// voip has to be supported by both server and client
	if( !trap_Cvar_VariableValue("sv_voip") || 
		!trap_Cvar_VariableValue("cl_voip") || 
		!trap_Cvar_VariableValue("s_useOpenAL") )
	{
		s_ingame.voiceingame.generic.flags |= QMF_GRAYED;
	}

/*	s_ingame.teamorders.generic.type			= MTYPE_TEXTS;
	s_ingame.teamorders.fontHeight			= 20.0f;
	s_ingame.teamorders.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.teamorders.generic.x			= 320;
	s_ingame.teamorders.generic.y			= y;
	s_ingame.teamorders.generic.id			= ID_TEAMORDERS;
	s_ingame.teamorders.generic.callback	= InGame_Event; 
	s_ingame.teamorders.string				= "TEAM ORDERS";
	s_ingame.teamorders.color				= color_black;
	s_ingame.teamorders.style				= UI_CENTER|UI_SMALLFONT;
	if( !(trap_Cvar_VariableValue( "g_gametype" ) >= GT_TEAM) ) {
		s_ingame.teamorders.generic.flags |= QMF_GRAYED;
	}
	else {
		trap_GetClientState( &cs );
		trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
		team = atoi( Info_ValueForKey( info, "t" ) );
		if( team == TEAM_SPECTATOR ) {
			s_ingame.teamorders.generic.flags |= QMF_GRAYED;
		}
	}
*/
	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.callvote.generic.type		= MTYPE_TEXTS;
	s_ingame.callvote.fontHeight		= 20.0f;
	s_ingame.callvote.generic.flags		= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	if(!allowVote)
		s_ingame.callvote.generic.flags		|= QMF_GRAYED;
	s_ingame.callvote.generic.x			= 320;
	s_ingame.callvote.generic.y			= y;
	s_ingame.callvote.generic.id		= ID_CALLVOTE;
	s_ingame.callvote.generic.callback	= InGame_Event; 
	s_ingame.callvote.string			= "CALL VOTE";
	s_ingame.callvote.color				= color_black;
	s_ingame.callvote.focuscolor			= color_orange;
	s_ingame.callvote.style				= UI_CENTER|UI_SMALLFONT;

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.setup.generic.type			= MTYPE_TEXTS;
	s_ingame.setup.fontHeight			= 20.0f;
	s_ingame.setup.generic.flags		= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_ingame.setup.generic.x			= 320;
	s_ingame.setup.generic.y			= y;
	s_ingame.setup.generic.id			= ID_SETUP;
	s_ingame.setup.generic.callback		= InGame_Event; 
	s_ingame.setup.string				= "SETUP";
	s_ingame.setup.color				= color_black;
	s_ingame.setup.focuscolor			= color_orange;
	s_ingame.setup.style				= UI_CENTER|UI_SMALLFONT;

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.nextmap.generic.type		= MTYPE_TEXTS;
	s_ingame.nextmap.fontHeight			= 20.0f;
	s_ingame.nextmap.generic.flags		= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_ingame.nextmap.generic.x			= 320;
	s_ingame.nextmap.generic.y			= y;
	s_ingame.nextmap.generic.id			= ID_NEXTMAP;
	s_ingame.nextmap.generic.callback	= InGame_Event; 
	s_ingame.nextmap.string				= "NEXT MAP";
	s_ingame.nextmap.color				= color_black;
	s_ingame.nextmap.focuscolor			= color_orange;
	s_ingame.nextmap.style				= UI_CENTER|UI_SMALLFONT;

	if( !isLocalServer ) {
		s_ingame.nextmap.generic.flags |= QMF_GRAYED;
	}

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.server.generic.type			= MTYPE_TEXTS;
	s_ingame.server.fontHeight			= 20.0f;
	s_ingame.server.generic.flags		= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_ingame.server.generic.x			= 320;
	s_ingame.server.generic.y			= y;
	s_ingame.server.generic.id			= ID_SERVERINFO;
	s_ingame.server.generic.callback	= InGame_Event; 
	s_ingame.server.string				= "SERVER INFO";
	s_ingame.server.color				= color_black;
	s_ingame.server.focuscolor			= color_orange;
	s_ingame.server.style				= UI_CENTER|UI_SMALLFONT;

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.restart.generic.type		= MTYPE_TEXTS;
	s_ingame.restart.fontHeight			= 20.0f;
	s_ingame.restart.generic.flags		= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_ingame.restart.generic.x			= 320;
	s_ingame.restart.generic.y			= y;
	s_ingame.restart.generic.id			= ID_RESTART;
	s_ingame.restart.generic.callback	= InGame_Event; 
	s_ingame.restart.string				= "RESTART MAP";
	s_ingame.restart.color				= color_black;
	s_ingame.restart.focuscolor			= color_orange;
	s_ingame.restart.style				= UI_CENTER|UI_SMALLFONT;
	if( !isLocalServer ) {
		s_ingame.restart.generic.flags |= QMF_GRAYED;
	}

	if( isSyC )
	{
		y += INGAME_MENU_VERTICAL_SPACING;
		s_ingame.rechooselogo.generic.type			= MTYPE_TEXTS;
		s_ingame.rechooselogo.fontHeight			= 20.0f;
		s_ingame.rechooselogo.generic.flags			= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
		s_ingame.rechooselogo.generic.x				= 320;
		s_ingame.rechooselogo.generic.y				= y;
		s_ingame.rechooselogo.generic.id			= ID_RECHOOSELOGO;
		s_ingame.rechooselogo.generic.callback		= InGame_Event; 
		s_ingame.rechooselogo.string				= "SELECT LOGO";
		s_ingame.rechooselogo.color					= color_black;
		s_ingame.rechooselogo.focuscolor			= color_orange;
		s_ingame.rechooselogo.style					= UI_CENTER|UI_SMALLFONT;
	}

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.resume.generic.type			= MTYPE_TEXTS;
	s_ingame.resume.fontHeight				= 20.0f;
	s_ingame.resume.generic.flags			= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_ingame.resume.generic.x				= 320;
	s_ingame.resume.generic.y				= y;
	s_ingame.resume.generic.id				= ID_RESUME;
	s_ingame.resume.generic.callback		= InGame_Event; 
	s_ingame.resume.string					= "RESUME GAME";
	s_ingame.resume.color					= color_black;
	s_ingame.resume.focuscolor			= color_orange;
	s_ingame.resume.style					= UI_CENTER|UI_SMALLFONT;

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.leave.generic.type			= MTYPE_TEXTS;
	s_ingame.leave.fontHeight			= 20.0f;
	s_ingame.leave.generic.flags		= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_ingame.leave.generic.x			= 320;
	s_ingame.leave.generic.y			= y;
	s_ingame.leave.generic.id			= ID_LEAVEARENA;
	s_ingame.leave.generic.callback		= InGame_Event; 
	s_ingame.leave.string				= "LEAVE ARENA";
	s_ingame.leave.color				= color_black;
	s_ingame.leave.focuscolor			= color_orange;
	s_ingame.leave.style				= UI_CENTER|UI_SMALLFONT;

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.quit.generic.type			= MTYPE_TEXTS;
	s_ingame.quit.fontHeight			= 20.0f;
	s_ingame.quit.generic.flags			= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_ingame.quit.generic.x				= 320;
	s_ingame.quit.generic.y				= y;
	s_ingame.quit.generic.id			= ID_QUIT;
	s_ingame.quit.generic.callback		= InGame_Event; 
	s_ingame.quit.string				= "EXIT GAME";
	s_ingame.quit.color					= color_black;
	s_ingame.quit.focuscolor			= color_orange;
	s_ingame.quit.style					= UI_CENTER|UI_SMALLFONT;

	Menu_AddItem( &s_ingame.menu, &s_ingame.team );
	Menu_AddItem( &s_ingame.menu, &s_ingame.addbots );
	Menu_AddItem( &s_ingame.menu, &s_ingame.removebots );
	Menu_AddItem( &s_ingame.menu, &s_ingame.voiceingame );
//	Menu_AddItem( &s_ingame.menu, &s_ingame.teamorders );
	Menu_AddItem( &s_ingame.menu, &s_ingame.callvote );
	Menu_AddItem( &s_ingame.menu, &s_ingame.setup );
	Menu_AddItem( &s_ingame.menu, &s_ingame.nextmap );
	Menu_AddItem( &s_ingame.menu, &s_ingame.server );
	Menu_AddItem( &s_ingame.menu, &s_ingame.restart );
	if(isSyC)
	{
		Menu_AddItem( &s_ingame.menu, &s_ingame.rechooselogo );
	}
	Menu_AddItem( &s_ingame.menu, &s_ingame.resume );
	Menu_AddItem( &s_ingame.menu, &s_ingame.leave );
	Menu_AddItem( &s_ingame.menu, &s_ingame.quit );
}


/*
=================
InGame_Cache
=================
*/
void InGame_Cache( void ) {
}


/*
=================
UI_InGameMenu
=================
*/
void UI_InGameMenu( void ) {
	// force as top level menu
	uis.menusp = 0;  

	// set menu cursor to a nice location
	uis.cursorx = 319;
	uis.cursory = 80;

	InGame_MenuInit();
	UI_PushMenu( &s_ingame.menu );
}
