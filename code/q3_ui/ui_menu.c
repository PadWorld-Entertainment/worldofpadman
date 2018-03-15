// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

MAIN MENU

=======================================================================
*/


#include "ui_local.h"

#define ID_SINGLEPLAYER			10
#define ID_MULTIPLAYER			11
#define ID_SETUP				12
#define ID_DEMOS				13
#define ID_MODS					14
#define ID_EXIT					15
#define ID_CREDITS				16
#define ID_SECRET				17

#define MULTIPLAYER0	"menu/main/ButtonMultiplayer0"
#define MULTIPLAYER1	"menu/main/ButtonMultiplayer1"
#define SINGLEPLAYER0	"menu/main/ButtonSingleplayer0"
#define SINGLEPLAYER1	"menu/main/ButtonSingleplayer1"
#define SETUP0			"menu/main/ButtonSetup0"
#define SETUP1			"menu/main/ButtonSetup1"
#define DEMOS0			"menu/main/ButtonDemos0"
#define DEMOS1			"menu/main/ButtonDemos1"
#define MODS0			"menu/main/ButtonMods0"
#define MODS1			"menu/main/ButtonMods1"
#define EXIT0			"menu/main/ButtonExit0"
#define EXIT1			"menu/main/ButtonExit1"
#define CREDITS0		"menu/main/ButtonCredits0"
#define CREDITS1		"menu/main/ButtonCredits1"

#define MAX_MSGLENGHT	256

typedef struct {
	menuframework_s	menu;

	menubitmap_s	Singleplayer;
	menubitmap_s	Multiplayer;
	menubitmap_s	Setup;
	menubitmap_s	Demos;
	menubitmap_s	Mods;
	menubitmap_s	Exit;
	menubitmap_s	Credits;

	menubitmap_s	secret;

	char			msg[MAX_MSGLENGHT];
	int				msgtime;
}
mainmenu_t;

static mainmenu_t s_main;

typedef struct {
	menuframework_s menu;	
	char errorMessage[4096];
} errorMessage_t;

static errorMessage_t s_errorMessage;

#if 0
/*
=================
MainMenu_ExitAction
=================
*/
static void MainMenu_ExitAction( qboolean result ) {
	if( !result ) {
		return;
	}
	UI_PopMenu();
	UI_CreditMenu();
}
#endif

/*
=================
Main_MenuEvent
=================
*/
void Main_MenuEvent (void* ptr, int event) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_SINGLEPLAYER:
//(original)		UI_SPLevelMenu();
//#ifdef WOPSP_TEST
//		WoPSPMenu_Init();
/*
#else
		Q_strncpyz(s_main.msg,"SinglePlayer hasn't been implemented yet!",MAX_MSGLENGHT);
		s_main.msgtime=uis.realtime;
#endif
*/
		UI_StartServerMenu( qtrue );
		break;

	case ID_MULTIPLAYER:
		UI_ArenaServersMenu();
		break;

	case ID_SETUP:
		UI_SetupMenu();
		break;

	case ID_DEMOS:
		UI_DemosMenu();
		break;

/*(orignal)
	case ID_CINEMATICS:
		UI_CinematicsMenu();
		break;
*/

	case ID_MODS:
		UI_ModsMenu();
		break;

	case ID_CREDITS:
		UI_InitBigCredits();
		break;

	case ID_SECRET:
		UI_SecretMenu();
		break;

	case ID_EXIT:
		ExitMenu_Init();
		break;
	}
}


/*
===============
MainMenu_Cache
===============
*/
void MainMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip(MULTIPLAYER0);
	trap_R_RegisterShaderNoMip(MULTIPLAYER1);
	trap_R_RegisterShaderNoMip(SINGLEPLAYER0);
	trap_R_RegisterShaderNoMip(SINGLEPLAYER1);
	trap_R_RegisterShaderNoMip(SETUP0);
	trap_R_RegisterShaderNoMip(SETUP1);
	trap_R_RegisterShaderNoMip(DEMOS0);
	trap_R_RegisterShaderNoMip(DEMOS1);
	trap_R_RegisterShaderNoMip(MODS0);
	trap_R_RegisterShaderNoMip(MODS1);
	trap_R_RegisterShaderNoMip(EXIT0);
	trap_R_RegisterShaderNoMip(EXIT1);
	trap_R_RegisterShaderNoMip(CREDITS0);
	trap_R_RegisterShaderNoMip(CREDITS1);
}

sfxHandle_t ErrorMessage_Key(int key)
{
	trap_Cvar_Set( "com_errorMessage", "" );
	UI_MainMenu();
	return (menu_null_sound);
}

/*
===============
Main_MenuDraw
TTimo: this function is common to the main menu and errorMessage menu
===============
*/
int CINhandle = -1;
static void Main_MenuDraw( void ) {
	int		msglen;
	vec4_t	tblack = {0.0f,0.0f,0.0f,0.66f};
	float	size;

	msglen=strlen(s_main.msg);

	// advanced2d-test ;)
//	DrawTurnableString(320,240,"blablubli",colorRed,50,uis.realtime/100.0f,TURNORIGIN_MIDDLECENTER);
//	DrawLine(320.0f*(1.0f+sin(uis.realtime*0.001f+30)),240.0f*(1.0f+sin(uis.realtime*0.001f+87)),320.0f*(1.0f+sin(uis.realtime*0.001f+4567)),240.0f*(1.0f+sin(uis.realtime*0.001f+649)),20,colorGreen);

	if(s_main.msgtime!=0 && uis.realtime-s_main.msgtime<=5000)
	{
		if(uis.realtime-s_main.msgtime>4750)
		{
			size = 16.0f*((float)(5000-(uis.realtime-s_main.msgtime))/250.0f);
		}
		else if(uis.realtime-s_main.msgtime>250)
		{
			size=16.0f;
		}
		else
		{
			size = 16.0f*((float)((uis.realtime-s_main.msgtime))/250.0f);
		}

		UI_FillRect(320.0f-(size*(msglen+4.0f)*0.25f),240.0f-size,(msglen+4.0f)*size*0.5f,2.0f*size,tblack);
		UI_DrawRect(320.0f-(size*(msglen+4.0f)*0.25f),240.0f-size,(msglen+4.0f)*size*0.5f,2.0f*size,colorBlack, 1);
		UI_DrawStringNS(320,240-size/2,s_main.msg,UI_CENTER,size,colorWhite);
	}

	if (strlen(s_errorMessage.errorMessage))
		UI_DrawString_AutoWrapped( 320, 192, 600, 20, s_errorMessage.errorMessage, UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, menu_text_color, qtrue );
	else
		Menu_Draw( &s_main.menu );

	if(CINhandle>=0) 
	{
		trap_CIN_RunCinematic(CINhandle);
		trap_CIN_DrawCinematic(CINhandle);
	}
}

sfxHandle_t Main_MenuKey(int key)
{
	if(key==K_ESCAPE || key==K_MOUSE2)
		return 0;

	return Menu_DefaultKey( &s_main.menu, key );
}

/*
===============
UI_MainMenu

The main menu only comes up when not in a game,
so make sure that the attract loop server is down
and that local cinematics are killed
===============
*/
void UI_MainMenu( void )
{
	trap_Cvar_Set( "sv_killserver", "1" );
//	trap_Cmd_ExecuteText(EXEC_APPEND, "wopSP_stopStory\n");
	trap_Cvar_Set("mapname",""); //note: I just recognized that this isn't reset on server shutdown ... but for some optimations in wopSP-code it is better to have this on a correct value

	if(UI_GetCvarInt("ui_mainmenuCinTest")) {
		char buffer[32];
		trap_Cvar_VariableStringBuffer("ui_mainmenuCinTestFile",buffer,sizeof(buffer));
		if(!buffer[0]) strcpy(buffer,"idlogo.roq");
		if(CINhandle>=0) trap_CIN_StopCinematic(CINhandle);
//		CINhandle = trap_CIN_PlayCinematic("idlogo.roq", 10, 10, 160, 120, /*int bits*/ CIN_loop);
		CINhandle = trap_CIN_PlayCinematic(buffer, 10, 10, 160, 120, /*int bits*/ CIN_loop);
		trap_S_StopBackgroundTrack();
	}

	memset( &s_main, 0 ,sizeof(mainmenu_t) );
	memset( &s_errorMessage, 0 ,sizeof(errorMessage_t) );

	// com_errorMessage would need that too
	MainMenu_Cache();
	
	trap_Cvar_VariableStringBuffer( "com_errorMessage", s_errorMessage.errorMessage, sizeof(s_errorMessage.errorMessage) );
	if (strlen(s_errorMessage.errorMessage))
	{	
		s_errorMessage.menu.draw = Main_MenuDraw;
		s_errorMessage.menu.key = ErrorMessage_Key;
		s_errorMessage.menu.fullscreen = qtrue;
		s_errorMessage.menu.wrapAround = qtrue;

		trap_Key_SetCatcher( KEYCATCH_UI );
		uis.menusp = 0;
		UI_PushMenu ( &s_errorMessage.menu );
		
		return;
	}

	s_main.menu.draw			= Main_MenuDraw;
	s_main.menu.key				= Main_MenuKey;
	s_main.menu.fullscreen		= qtrue;
	s_main.menu.wrapAround		= qtrue;
	s_main.menu.bgparts			= BGP_MAINBG|BGP_MAINFRAME;
	s_main.menu.noPushSelect	= qtrue;

	s_main.Singleplayer.generic.type	= MTYPE_BITMAP;
	s_main.Singleplayer.generic.name	= SINGLEPLAYER0;
	s_main.Singleplayer.generic.flags	= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    s_main.Singleplayer.generic.x		= 497;
	s_main.Singleplayer.generic.y		= 140;
	s_main.Singleplayer.generic.id		= ID_SINGLEPLAYER;
	s_main.Singleplayer.generic.callback= Main_MenuEvent;
    s_main.Singleplayer.width			= 120;
	s_main.Singleplayer.height			= 40;
	s_main.Singleplayer.focuspic		= SINGLEPLAYER1;
	s_main.Singleplayer.focuspicinstead	= qtrue;
	Menu_AddItem(&s_main.menu,	&s_main.Singleplayer);

	s_main.Multiplayer.generic.type		= MTYPE_BITMAP;
	s_main.Multiplayer.generic.name		= MULTIPLAYER0;
	s_main.Multiplayer.generic.flags	= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    s_main.Multiplayer.generic.x		= 508;
	s_main.Multiplayer.generic.y		= 175;
	s_main.Multiplayer.generic.id		= ID_MULTIPLAYER;
	s_main.Multiplayer.generic.callback	= Main_MenuEvent;
    s_main.Multiplayer.width			= 120;
	s_main.Multiplayer.height			= 40;
	s_main.Multiplayer.focuspic			= MULTIPLAYER1;
	s_main.Multiplayer.focuspicinstead	= qtrue;
	Menu_AddItem(&s_main.menu,	&s_main.Multiplayer);

	s_main.Setup.generic.type			= MTYPE_BITMAP;
	s_main.Setup.generic.name			= SETUP0;
	s_main.Setup.generic.flags			= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    s_main.Setup.generic.x				= 496;
	s_main.Setup.generic.y				= 210;
	s_main.Setup.generic.id				= ID_SETUP;
	s_main.Setup.generic.callback		= Main_MenuEvent;
    s_main.Setup.width					= 120;
	s_main.Setup.height					= 40;
	s_main.Setup.focuspic				= SETUP1;
	s_main.Setup.focuspicinstead		= qtrue;
	Menu_AddItem(&s_main.menu,	&s_main.Setup);

	s_main.Demos.generic.type			= MTYPE_BITMAP;
	s_main.Demos.generic.name			= DEMOS0;
	s_main.Demos.generic.flags			= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    s_main.Demos.generic.x				= 510;
	s_main.Demos.generic.y				= 245;
	s_main.Demos.generic.id				= ID_DEMOS;
	s_main.Demos.generic.callback		= Main_MenuEvent;
    s_main.Demos.width					= 120;
	s_main.Demos.height					= 40;
	s_main.Demos.focuspic				= DEMOS1;
	s_main.Demos.focuspicinstead		= qtrue;
	Menu_AddItem(&s_main.menu,	&s_main.Demos);

	s_main.Mods.generic.type			= MTYPE_BITMAP;
	s_main.Mods.generic.name			= MODS0;
	s_main.Mods.generic.flags			= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    s_main.Mods.generic.x				= 516;
	s_main.Mods.generic.y				= 280;
	s_main.Mods.generic.id				= ID_MODS;
	s_main.Mods.generic.callback		= Main_MenuEvent;
    s_main.Mods.width					= 80;
	s_main.Mods.height					= 40;
	s_main.Mods.focuspic				= MODS1;
	s_main.Mods.focuspicinstead			= qtrue;
	Menu_AddItem(&s_main.menu,	&s_main.Mods);
	
	s_main.Exit.generic.type			= MTYPE_BITMAP;
	s_main.Exit.generic.name			= EXIT0;
	s_main.Exit.generic.flags			= QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_main.Exit.generic.x				= 530;
	s_main.Exit.generic.y				= 315;
	s_main.Exit.generic.id				= ID_EXIT;
	s_main.Exit.generic.callback		= Main_MenuEvent;
    s_main.Exit.width					= 80;
	s_main.Exit.height					= 40;
	s_main.Exit.focuspic				= EXIT1;
	s_main.Exit.focuspicinstead			= qtrue;
	Menu_AddItem(&s_main.menu,	&s_main.Exit);

	s_main.Credits.generic.type			= MTYPE_BITMAP;
	s_main.Credits.generic.name			= CREDITS0;
	s_main.Credits.generic.flags		= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    s_main.Credits.generic.x			= 505;
	s_main.Credits.generic.y			= 420;
	s_main.Credits.generic.id			= ID_CREDITS;
	s_main.Credits.generic.callback		= Main_MenuEvent;
    s_main.Credits.width				= 120;
	s_main.Credits.height				= 40;
	s_main.Credits.focuspic				= CREDITS1;
	s_main.Credits.focuspicinstead		= qtrue;
	Menu_AddItem(&s_main.menu,	&s_main.Credits);

	
	s_main.secret.generic.type			= MTYPE_BITMAP;
	s_main.secret.generic.flags			= QMF_MOUSEONLY|QMF_SILENT|QMF_HIDDEN;
    s_main.secret.generic.x				= 129;
	s_main.secret.generic.y				= 431;
	s_main.secret.generic.id			= ID_SECRET;
	s_main.secret.generic.callback		= Main_MenuEvent;
    s_main.secret.width					= 22;
	s_main.secret.height				= 22;
	Menu_AddItem(&s_main.menu,	&s_main.secret);

	trap_Key_SetCatcher( KEYCATCH_UI );
	uis.menusp = 0;
	UI_PushMenu ( &s_main.menu );
}
