// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

USER INTERFACE MAIN

=======================================================================
*/


#include "ui_local.h"


/*
================
vmMain

This is the only way control passes into the module.
This must be the very first function compiled into the .qvm file
================
*/
Q_EXPORT intptr_t vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11  ) {
	switch ( command ) {
	case UI_GETAPIVERSION:
		return UI_API_VERSION;

	case UI_INIT:
		UI_Init();
		return 0;

	case UI_SHUTDOWN:
		UI_Shutdown();
		return 0;

	case UI_KEY_EVENT:
		UI_KeyEvent( arg0, arg1 );
		return 0;

	case UI_MOUSE_EVENT:
		UI_MouseEvent( arg0, arg1 );
		return 0;

	case UI_REFRESH:
		UI_Refresh( arg0 );
		return 0;

	case UI_IS_FULLSCREEN:
		return UI_IsFullscreen();

	case UI_SET_ACTIVE_MENU:
		UI_SetActiveMenu( arg0 );
		return 0;

	case UI_CONSOLE_COMMAND:
		return UI_ConsoleCommand(arg0);

	case UI_DRAW_CONNECT_SCREEN:
		UI_DrawConnectScreen( arg0 );
		return 0;
	case UI_HASUNIQUECDKEY:				// mod authors need to observe this
//(original)		return qtrue;  // bk010117 - change this to qfalse for mods!
		return qfalse; //Raute: found this Sep. 4th 2005 O_o ... by accident -.-
	}

	return -1;
}


/*
================
cvars
================
*/

typedef struct {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
} cvarTable_t;

vmCvar_t	ui_ffa_fraglimit;
vmCvar_t	ui_ffa_timelimit;

vmCvar_t	ui_tourney_fraglimit;
vmCvar_t	ui_tourney_timelimit;

vmCvar_t	ui_team_fraglimit;
vmCvar_t	ui_team_timelimit;
vmCvar_t	ui_team_friendly;

vmCvar_t	ui_ctf_capturelimit;
vmCvar_t	ui_ctf_timelimit;
vmCvar_t	ui_ctf_friendly;

vmCvar_t	ui_arenasFile;
vmCvar_t	ui_botsFile;
vmCvar_t	ui_spScores1;
vmCvar_t	ui_spScores2;
vmCvar_t	ui_spScores3;
vmCvar_t	ui_spScores4;
vmCvar_t	ui_spScores5;
vmCvar_t	ui_spAwards;
vmCvar_t	ui_spVideos;
vmCvar_t	ui_spSkill;

vmCvar_t	ui_spSelection;

vmCvar_t	ui_browserMaster;
vmCvar_t	ui_browserGameType;
vmCvar_t	ui_browserSortKey;
vmCvar_t	ui_browserShowFull;
vmCvar_t	ui_browserShowEmpty;
vmCvar_t	ui_browserOnlyHumans;
vmCvar_t	ui_browserHidePrivate;

vmCvar_t	ui_createGametype;

vmCvar_t	ui_drawCrosshair;
vmCvar_t	ui_drawCrosshairNames;
vmCvar_t	ui_marks;

vmCvar_t	ui_server1;
vmCvar_t	ui_server2;
vmCvar_t	ui_server3;
vmCvar_t	ui_server4;
vmCvar_t	ui_server5;
vmCvar_t	ui_server6;
vmCvar_t	ui_server7;
vmCvar_t	ui_server8;
vmCvar_t	ui_server9;
vmCvar_t	ui_server10;
vmCvar_t	ui_server11;
vmCvar_t	ui_server12;
vmCvar_t	ui_server13;
vmCvar_t	ui_server14;
vmCvar_t	ui_server15;
vmCvar_t	ui_server16;

vmCvar_t	ui_cdkeychecked;
//spray anfang
//vmCvar_t	logolist;
//spray ende
//lens anfang
//vmCvar_t	lensflarelist;
//lens ende
vmCvar_t	spraycolor;
vmCvar_t	syc_logo;
vmCvar_t	s_wop_restarted;
vmCvar_t	con_notifytime;
vmCvar_t	wop_AutoswitchSongByNextMap;
vmCvar_t	wop_AutoBindUnusedKeys;
vmCvar_t	wop_specialSPLoadingScreen;

// bk001129 - made static to avoid aliasing.
static cvarTable_t		cvarTable[] = {
	{ &ui_ffa_fraglimit, "ui_ffa_fraglimit", "20", CVAR_ARCHIVE },
	{ &ui_ffa_timelimit, "ui_ffa_timelimit", "0", CVAR_ARCHIVE },

	{ &ui_tourney_fraglimit, "ui_tourney_fraglimit", "0", CVAR_ARCHIVE },
	{ &ui_tourney_timelimit, "ui_tourney_timelimit", "15", CVAR_ARCHIVE },

	{ &ui_team_fraglimit, "ui_team_fraglimit", "0", CVAR_ARCHIVE },
	{ &ui_team_timelimit, "ui_team_timelimit", "20", CVAR_ARCHIVE },
	{ &ui_team_friendly, "ui_team_friendly",  "1", CVAR_ARCHIVE },

	{ &ui_ctf_capturelimit, "ui_ctf_capturelimit", "8", CVAR_ARCHIVE },
	{ &ui_ctf_timelimit, "ui_ctf_timelimit", "30", CVAR_ARCHIVE },
	{ &ui_ctf_friendly, "ui_ctf_friendly",  "0", CVAR_ARCHIVE },

	{ &ui_arenasFile, "g_arenasFile", "", CVAR_INIT|CVAR_ROM },
	{ &ui_botsFile, "g_botsFile", "", CVAR_INIT|CVAR_ROM },
	{ &ui_spScores1, "g_spScores1", "", CVAR_ARCHIVE },
	{ &ui_spScores2, "g_spScores2", "", CVAR_ARCHIVE },
	{ &ui_spScores3, "g_spScores3", "", CVAR_ARCHIVE },
	{ &ui_spScores4, "g_spScores4", "", CVAR_ARCHIVE },
	{ &ui_spScores5, "g_spScores5", "", CVAR_ARCHIVE },
	{ &ui_spAwards, "g_spAwards", "", CVAR_ARCHIVE },
	{ &ui_spVideos, "g_spVideos", "", CVAR_ARCHIVE },
	{ &ui_spSkill, "g_spSkill", "2", CVAR_ARCHIVE | CVAR_LATCH },

	{ &ui_spSelection, "ui_spSelection", "", CVAR_ROM },

	{ &ui_browserMaster, "ui_browserMaster", "2", CVAR_ARCHIVE },
	{ &ui_browserGameType, "ui_browserGameType", "0", CVAR_ARCHIVE },
	{ &ui_browserSortKey, "ui_browserSortKey", "5", CVAR_ARCHIVE },
	{ &ui_browserShowFull, "ui_browserShowFull", "0", CVAR_ARCHIVE },
	{ &ui_browserShowEmpty, "ui_browserShowEmpty", "1", CVAR_ARCHIVE },
	{ &ui_browserOnlyHumans, "ui_browserOnlyHumans", "1", CVAR_ARCHIVE },
	{ &ui_browserHidePrivate, "ui_browserHidePrivate", "0", CVAR_ARCHIVE },

	{ &ui_createGametype, "ui_createGametype", "3", 0 },

	{ &ui_drawCrosshair, "cg_drawCrosshair", "1", CVAR_ARCHIVE },
	{ &ui_drawCrosshairNames, "cg_drawCrosshairNames", "1", CVAR_ARCHIVE },
	{ &ui_marks, "cg_marks", "1", CVAR_ARCHIVE },

	{ &ui_server1, "server1", "", CVAR_ARCHIVE },
	{ &ui_server2, "server2", "", CVAR_ARCHIVE },
	{ &ui_server3, "server3", "", CVAR_ARCHIVE },
	{ &ui_server4, "server4", "", CVAR_ARCHIVE },
	{ &ui_server5, "server5", "", CVAR_ARCHIVE },
	{ &ui_server6, "server6", "", CVAR_ARCHIVE },
	{ &ui_server7, "server7", "", CVAR_ARCHIVE },
	{ &ui_server8, "server8", "", CVAR_ARCHIVE },
	{ &ui_server9, "server9", "", CVAR_ARCHIVE },
	{ &ui_server10, "server10", "", CVAR_ARCHIVE },
	{ &ui_server11, "server11", "", CVAR_ARCHIVE },
	{ &ui_server12, "server12", "", CVAR_ARCHIVE },
	{ &ui_server13, "server13", "", CVAR_ARCHIVE },
	{ &ui_server14, "server14", "", CVAR_ARCHIVE },
	{ &ui_server15, "server15", "", CVAR_ARCHIVE },
	{ &ui_server16, "server16", "", CVAR_ARCHIVE },

	{ &ui_cdkeychecked, "ui_cdkeychecked", "0", CVAR_ROM }
//spray anfang
//	,{ NULL, "logolist", "", CVAR_INIT|CVAR_ROM } // ohne verwaltungs-cvar -> kein value limit
//spray ende
//lens anfang
//	,{ NULL, "lensflarelist", "", CVAR_INIT|CVAR_ROM }
//lens ende
	,{ &spraycolor, "syc_color", "0", CVAR_ARCHIVE|CVAR_USERINFO }
	,{ &syc_logo, "syc_logo", "", CVAR_ARCHIVE }
	,{ &s_wop_restarted, "s_wop_restarted", "0", CVAR_ROM|CVAR_TEMP }//CVAR_NORESTART
	,{ &con_notifytime, "con_notifytime", "4", CVAR_ARCHIVE }
	,{ &wop_AutoswitchSongByNextMap, "wop_AutoswitchSongByNextMap", "0", CVAR_ARCHIVE }
	,{ &wop_AutoBindUnusedKeys, "wop_AutoBindUnusedKeys", "1", CVAR_ARCHIVE }
	,{ &wop_specialSPLoadingScreen, "wop_specialSPLoadingScreen", "1", CVAR_TEMP }
};

static int cvarTableSize = ARRAY_LEN( cvarTable );


/*
=================
UI_RegisterCvars
=================
*/
void UI_RegisterCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
		trap_Cvar_Register( cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags );
	}

	// cvars die mehr als 256 Zeich aufnehmen sollen (256-limit ist nur im gamecode)
//spray anfang
	trap_Cvar_Register( NULL, "logolist", "", CVAR_INIT|CVAR_ROM );
//spray ende
//lens anfang
	trap_Cvar_Register( NULL, "lensflarelist", "", CVAR_INIT|CVAR_ROM );
//lens ende
}

/*
=================
UI_UpdateCvars
=================
*/
void UI_UpdateCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
		trap_Cvar_Update( cv->vmCvar );
	}
}
