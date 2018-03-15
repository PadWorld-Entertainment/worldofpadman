
#include "wopg_sphandling.h"
#include "wopg_spstoryfiles.h"

#ifdef UI
#include "../ui/ui_local.h"

/*
static executeCmd(const char* cmd) {
	trap_Cmd_ExecuteText(EXEC_APPEND, cmd);
}
*/
#else
#include "g_local.h"

static wop_StoryElement_t game_currentSE;

/*
static executeCmd(const char* cmd) {
	trap_SendConsoleCommand(EXEC_APPEND, cmd);
}
*/
#endif

//num args: trap_Argc()

static const char* wopSP_Argv(int i) {
	static char str[MAX_TOKEN_CHARS];

	trap_Argv( i, str, sizeof( str ) );

	return str;
}

/*
	direct call by ui-init
	... used by wopSP_initGame
*/
void wopSP_init(void) {
	//NOTE: addCommand only exists in cg -.- (so can't add this cmds in game/ui)

	trap_Cvar_Register(NULL, WOPSP_STORY_CVAR	, "", CVAR_SERVERINFO|CVAR_ROM );
	trap_Cvar_Register(NULL, WOPSP_SELEMENT_CVAR, "", CVAR_SERVERINFO|CVAR_ROM );

	trap_Cvar_Register(NULL, WOPSP_SAVENAME_CVAR, "default", CVAR_ARCHIVE );

	trap_Cvar_Register(NULL, WOPSP_BOTSKILL		, "2", CVAR_ARCHIVE|CVAR_LATCH ); //like g_spSkill and the addbot-parameter 1-5
}

static void wopSP_setElementCvar(const wop_StoryElement_t* se) {
	if(se->name[0])
		trap_Cvar_Set(WOPSP_SELEMENT_CVAR, se->name);
	else
		trap_Cvar_Set(WOPSP_SELEMENT_CVAR, va("#%d",se->elementID));
}

/*
	do everything needed for starting a story(+element selection)
	... used by start/load cmd and some other functions in here
*/
static void wopSP_startStory(const char* story, const char* element) {
	if(!story || !story[0])
		return;

	if(!element)
		element = "";

	if(!wopSP_openStory(story)) {
		wop_StoryElement_t tmpE;
		qboolean foundEle;

//		Com_Printf("opened Story file\n");

		if(element[0])
			foundEle = (wopSP_findStoryElement(element, &tmpE) >= 0);
		else
			foundEle = (wopSP_loadNextStoryElement( &tmpE ) >= 0);

		if(foundEle) {
			trap_Cvar_Set(WOPSP_STORY_CVAR, story);
			wopSP_setElementCvar(&tmpE);
			wopSP_execStoryElement(&tmpE);
		}
		else
			Com_Printf("Couldn't find StoryElement in Storyfile(%s)\n",story);
		wopSP_closeFile();
	}
	else
		Com_Printf("Couldn't open Storyfile(%s)\n",story);
}

static void wopSP_cmd_startStory(void) {
	if(trap_Argc() < 2) {
		Com_Printf("  usage: %s [storyname] ([elementname])\n",wopSP_Argv(0));
	}
	else {

		if(trap_Argc() >2) {
			char element[MAX_TOKEN_CHARS];
			trap_Argv( 2, element, sizeof( element ) );

			wopSP_startStory(wopSP_Argv(1), element);
		}
		else {
			wopSP_startStory(wopSP_Argv(1), NULL);
		}
	}
}

static void wopSP_cmd_loadStory(void) {
	if(trap_Argc() < 2) {
		Com_Printf("  usage: %s [storyname]\n",wopSP_Argv(0));
	}
	else {
		char savename[MAX_CVAR_VALUE_STRING];
		int seNr = -1;

		trap_Cvar_VariableStringBuffer(WOPSP_SAVENAME_CVAR, savename, sizeof(savename) );

		seNr = wopSP_loadLastWonSE(wopSP_Argv(1), savename);

		if(seNr>=0) {
			wop_StoryElement_t tmpE;

			if(!wopSP_openStory(wopSP_Argv(1))) {
				wopSP_findStoryElement(va("#%d",seNr), &tmpE);

				if(!wopSP_validSE(&tmpE)) {
					Com_Printf("Couldn't find the saved story element. (seNr: %d)\n",seNr);
					return;
				}

				if(tmpE.nextAfterWin[0]) {
					// using va to have a copy of the string ;P
					wopSP_findStoryElement(va("%s",tmpE.nextAfterWin), &tmpE);
				}
				else {
					if(0>wopSP_findStoryElement(va("#%d",tmpE.elementID+1),&tmpE)) {
						wopSP_initStoryElement(&tmpE);
						Com_Printf("Couldn't find a next story element, after the saft one, probably the story was already finished\n");
					}
				}

				wopSP_closeFile();
			}

			if(wopSP_validSE(&tmpE))
				wopSP_startStory(wopSP_Argv(1), va("#%d",tmpE.elementID));
		}
		else
			Com_Printf("Couldn't find a saved state for this story.\n");
	}
}

static void wopSP_cmd_stopStory(void) {
	trap_Cvar_Set(WOPSP_STORY_CVAR, "");
	trap_Cvar_Set(WOPSP_SELEMENT_CVAR, "");

#ifndef UI // only in game-code
	wopSP_initStoryElement(&game_currentSE);//NOTE: it's more a "clear" than a init
#endif
}

static void wopSP_cmd_introFinished(void);

typedef struct {
	const char* name;
	void (*func)(void);
} cmdInfo_t;

static const cmdInfo_t wopSP_cmdInfos[] = {
	{ "wopSP_startStory",wopSP_cmd_startStory },
	{ "wopSP_loadStory",wopSP_cmd_loadStory },
	{ "wopSP_stopStory",wopSP_cmd_stopStory },
	{ "wopSP_introFinished",wopSP_cmd_introFinished },
	{NULL}
};

/*
  should be called by:
  ConsoleCommand (g_svcmds.c)
  UI_ConsoleCommand (ui_atoms.c)
*/
qboolean wopSP_cmdCheck(const char* cmd) {
	const cmdInfo_t* ci;

	for(ci=wopSP_cmdInfos;ci->name;++ci) {
		if(!Q_stricmp(ci->name,cmd)) {
			ci->func();

			return qtrue;
		}
	}

	return qfalse;
}

#ifndef UI // only in game-code

static qboolean won_intermission = qfalse;

static qboolean wopSP_calcWon(void) {
	int			i;
	gentity_t		*player;
	int			playerClientNum;

	// find the real player
	player = NULL;
	for (i = 0; i < level.maxclients; i++ ) {
		player = &g_entities[i];
		if ( !player->inuse ) {
			continue;
		}
		if ( !( player->r.svFlags & SVF_BOT ) ) {
			break;
		}
	}
	if ( !player || i == level.maxclients ) {
		return qfalse;
	}
	playerClientNum = i;

	if ( g_gametype.integer >= GT_TEAM ) {
		if((player->client->sess.sessionTeam == TEAM_RED && level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE])
				|| (player->client->sess.sessionTeam == TEAM_BLUE && level.teamScores[TEAM_RED] < level.teamScores[TEAM_BLUE]))
			return qtrue;
		else
			return qfalse;
	}
	else {
		CalculateRanks();

		return (player->client->ps.persistant[PERS_RANK] == 0);
	}
}

void wopSP_OnIntermission(void) {
	won_intermission = qfalse;

	if(wopSP_validSE(&game_currentSE)) {
		won_intermission = wopSP_calcWon();
		//TODO: check win/loos ... save ... trigger extro

		if(won_intermission) {
			char story[MAX_CVAR_VALUE_STRING];
			char savename[MAX_CVAR_VALUE_STRING];

			trap_Cvar_VariableStringBuffer(WOPSP_STORY_CVAR, story, sizeof(story) );
			trap_Cvar_VariableStringBuffer(WOPSP_SAVENAME_CVAR, savename, sizeof(savename) );

			if(!wopSP_WonSEalreadySaved(story, savename,game_currentSE.elementID))
				wopSP_addWonSEtoGameState(story, savename, game_currentSE.elementID);
		}

//TODO: 		wopSP_triggerExtro(&game_currentSE,won_intermission);
	}


	if(wop_storyMode.integer==WSM_STARTMAP) {
		trap_Cvar_Set("cl_run","1");
		level.clients[0].readyToExit = qtrue;
	}
}

qboolean wopSP_OnExitLevel(void) {
	if(wopSP_validSE(&game_currentSE)) {
		int i;
		char netname[36];
		gclient_t	*cl;
		char story[MAX_CVAR_VALUE_STRING];

		// kick all bots!
		for ( i=0 ; i< g_maxclients.integer ; ++i ) {
			cl = level.clients + i;
			if ( cl->pers.connected != CON_CONNECTED ) {
				continue;
			}
			if ( !(g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT) ) {
				continue;
			}
			strcpy(netname, cl->pers.netname);
			Q_CleanStr(netname);
			trap_SendConsoleCommand( EXEC_INSERT, va("kick %s\n", netname) );
		}

		trap_Cvar_VariableStringBuffer(WOPSP_STORY_CVAR, story, sizeof(story) );

		if(won_intermission) {
			if(!wopSP_openStory(story)) {
				if(game_currentSE.nextAfterWin[0]) {
					// using va to have a copy of the string ;P
					wopSP_findStoryElement(va("%s",game_currentSE.nextAfterWin), &game_currentSE);
				}
				else {
					if(0>wopSP_findStoryElement(va("#%d",game_currentSE.elementID+1),&game_currentSE))
						wopSP_initStoryElement(&game_currentSE);
				}

				wopSP_closeFile();
			}
			else // couldn't open story ... stop it
				wopSP_initStoryElement(&game_currentSE);
		}
		else {
			if(game_currentSE.nextAfterLoos[0] && !wopSP_openStory(story)) {
				wopSP_findStoryElement(va("%s",game_currentSE.nextAfterLoos), &game_currentSE);

				wopSP_closeFile();
			}
			//empty afterLoos => restart element
		}

		if(wopSP_validSE(&game_currentSE)) {
			wopSP_setElementCvar(&game_currentSE);
			wopSP_execStoryElement(&game_currentSE);
		}
		else {
			Com_Printf("Story finished!\nServer will be shutdown\n");
			wopSP_cmd_stopStory();
			trap_Cvar_Set( "sv_killserver", "1" ); //FIXME: find a better way to shutdown the server ... this doesn't work correctly
		}

		return qtrue;
	}

	return qfalse;
}

void wopSP_initGame(void) {
	char story[MAX_CVAR_VALUE_STRING];
	char element[MAX_CVAR_VALUE_STRING];

	trap_Cvar_Set("wop_storyMode","0");

	wopSP_initStoryElement(&game_currentSE);

	wopSP_init();

	trap_Cvar_VariableStringBuffer(WOPSP_STORY_CVAR, story, sizeof(story) );
	trap_Cvar_VariableStringBuffer(WOPSP_SELEMENT_CVAR, element, sizeof(element) );
	if(story[0] && element[0]) {
		if(!wopSP_openStory(story)) {
			wopSP_findStoryElement(element, &game_currentSE);

			wopSP_closeFile();
		}
	}

	//TODO: move it to another position (or change the name of the function)
	wopSP_firstFrame();
}

#define BOT_BEGIN_DELAY_BASE		2000
#define BOT_BEGIN_DELAY_INCREMENT	1500

static const char* team_strs[] = { "free", "red", "blue" };

// extended G_SpawnBots(g_bots.c) by a teamoption ("botname@team")
static void wopSP_SpawnBots( char *botList, int baseDelay ) {
	char		*bot;
	char		*p;
	float		skill;
	int			delay;
	char		bots[WOPSP_BOTLIST_STR];
	int			team;

	skill = trap_Cvar_VariableValue( WOPSP_BOTSKILL );
	if( skill < 1 ) {
		trap_Cvar_Set( WOPSP_BOTSKILL, "1" );
		skill = 1;
	}
	else if ( skill > 5 ) {
		trap_Cvar_Set( WOPSP_BOTSKILL, "5" );
		skill = 5;
	}

	Q_strncpyz( bots, botList, sizeof(bots) );
	p = &bots[0];
	delay = baseDelay;
	while( *p ) {
		team = 0;

		//skip spaces
		while( *p && *p == ' ' ) {
			p++;
		}
		if( !p ) {
			break;
		}

		// mark start of bot name
		bot = p;

		// skip until space of null
		while( *p && *p != ' ' ) {
			p++;
		}
		if( *p ) {
			*p++ = 0;
		}

		{
			char* teamPtr;

			teamPtr = strchr(bot,'@');

			if(teamPtr) {
				if(teamPtr[1]==team_strs[1][0]) team = 1;
				if(teamPtr[1]==team_strs[2][0]) team = 2;

				*teamPtr = '\0';
			}
		}

		// we must add the bot this way, calling G_AddBot directly at this stage
		// does "Bad Things"
		trap_SendConsoleCommand( EXEC_INSERT, va("addbot %s %f %s %i\n", bot, skill,team_strs[team], delay) );

		delay += BOT_BEGIN_DELAY_INCREMENT;
	}
}

static qboolean notIntroStarted=qtrue;

void wopSP_firstFrame(void ) {
	//note: this is to early ... this will be called before r/ui/cgame is inited
	notIntroStarted=qtrue;

	if(!wopSP_validSE(&game_currentSE))
		return;

	if(game_currentSE.flags & WSEF_STARTMAP) {
		trap_Cvar_Set("wop_storyMode",va("%d",WSM_STARTMAP));
		trap_Cvar_Set("cl_run","0");
	}
	else if(game_currentSE.flags &  WSEF_ENDMAP) {
		trap_Cvar_Set("wop_storyMode",va("%d",WSM_ENDMAP));
	}
	else {
		trap_Cvar_Set("wop_storyMode",va("%d",WSM_NORMAL));
	}
}

void wopSP_client0Begins(void) {

	if(!wopSP_validSE(&game_currentSE))
		return;


	if(notIntroStarted) {
		if(game_currentSE.intro[0]) {
			//TODO: if intro is cutscene -> do something else
			//TODO: build in the calling+handling of a "introFinished"-cmd
			if(!Q_stricmp(game_currentSE.intro,"cutscene"))
				trap_SendConsoleCommand( EXEC_APPEND, "startcam\n");
			else
				trap_SendConsoleCommand( EXEC_APPEND, va("wop_mediaview \"%s\" \"wopSP_introFinished\"\n",game_currentSE.intro));

		}
		else
			wopSP_SpawnBots(game_currentSE.bots, BOT_BEGIN_DELAY_BASE);

		notIntroStarted=qfalse;
	}
}

static void wopSP_cmd_introFinished(void) {
	if(!wopSP_validSE(&game_currentSE))
		return;


	wopSP_SpawnBots(game_currentSE.bots, BOT_BEGIN_DELAY_BASE);
}

qboolean wopSP_hasForceTeam(void) {
	return (wopSP_validSE(&game_currentSE) && game_currentSE.forceToTeam[0]);
}
int wopSP_forceTeam(void) {
	return game_currentSE.forceToTeam[0]=='b'?TEAM_BLUE:TEAM_RED;
}

#else
static void wopSP_cmd_introFinished(void) {}
#endif // ! UI


