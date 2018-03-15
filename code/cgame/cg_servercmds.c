// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_servercmds.c -- reliably sequenced text commands sent by the server
// these are processed at snapshot transition time, so there will definately
// be a valid snapshot this frame

#include "cg_local.h"

/*
=================
CG_ParseScores

=================
*/
static void CG_ParseScores( void ) {
	int		i, powerups;

	cg.numScores = atoi( CG_Argv( 1 ) );
	if ( cg.numScores > MAX_CLIENTS ) {
		cg.numScores = MAX_CLIENTS;
	}
//	cg.numFFAplayers = 0;

	memset(cg.scoreTeamCount,0,sizeof(cg.scoreTeamCount));

	cg.teamScores[0] = atoi( CG_Argv( 2 ) );
	cg.teamScores[1] = atoi( CG_Argv( 3 ) );

	memset( cg.scores, 0, sizeof( cg.scores ) );
	for ( i = 0 ; i < cg.numScores ; i++ ) {
		//
		//i*14 zu i*17 um geändert

		cg.scores[i].client = atoi( CG_Argv( i * 17 + 4 ) );
		cg.scores[i].score = atoi( CG_Argv( i * 17 + 5 ) );
		cg.scores[i].ping = atoi( CG_Argv( i * 17 + 6 ) );
		cg.scores[i].time = atoi( CG_Argv( i * 17 + 7 ) );
		cg.scores[i].scoreFlags = atoi( CG_Argv( i * 17 + 8 ) );
		powerups = atoi( CG_Argv( i * 17 + 9 ) );
		cg.scores[i].accuracy = atoi(CG_Argv(i * 17 + 10));
		cg.scores[i].impressiveCount = atoi(CG_Argv(i * 17 + 11));
		cg.scores[i].excellentCount = atoi(CG_Argv(i * 17 + 12));
		cg.scores[i].guantletCount = atoi(CG_Argv(i * 17 + 13));
		cg.scores[i].defendCount = atoi(CG_Argv(i * 17 + 14));
		cg.scores[i].assistCount = atoi(CG_Argv(i * 17 + 15));
		cg.scores[i].perfect = atoi(CG_Argv(i * 17 + 16));
		cg.scores[i].captures = atoi(CG_Argv(i * 17 + 17));
		cg.scores[i].spraygod = atoi(CG_Argv(i * 17 + 18)); //noch mal ändern ... oder stimmt das doch?
		cg.scores[i].spraykiller = atoi(CG_Argv(i * 17 + 19));
		cg.scores[i].livesleft = atoi(CG_Argv(i * 17 + 20));

		if ( cg.scores[i].client < 0 || cg.scores[i].client >= MAX_CLIENTS ) {
			cg.scores[i].client = 0;
		}
		cgs.clientinfo[ cg.scores[i].client ].score = cg.scores[i].score;
		cgs.clientinfo[ cg.scores[i].client ].powerups = powerups;

		cg.scores[i].team = cgs.clientinfo[cg.scores[i].client].team;
//		if(cgs.gametype<GT_TEAM && cg.scores[i].team==TEAM_FREE) cg.numFFAplayers++;
		if(cg.scores[i].team>=TEAM_FREE && cg.scores[i].team<=TEAM_SPECTATOR)
			cg.scoreTeamCount[cg.scores[i].team]++;
	}

}

/*
=================
CG_ParseTeamInfo

=================
*/
#define TI_VALUES_PER_PLAYER 7
static void CG_ParseTeamInfo( void ) {
	int		i, index;
	int		client;

	numSortedTeamPlayers = atoi( CG_Argv( 1 ) );
	if ( ( numSortedTeamPlayers < 0 ) || ( numSortedTeamPlayers > TEAM_MAXOVERLAY ) ) {
		CG_Error( "CG_ParseTeamInfo: numSortedTeamPlayers out of range (%d)", numSortedTeamPlayers );
		return;
	}

	for ( i = 0 ; i < numSortedTeamPlayers ; i++ ) {
		index = ( i * TI_VALUES_PER_PLAYER );

		client = atoi( CG_Argv( index + 2 ) );
		if ( ( client < 0 ) || ( client >= MAX_CLIENTS ) ) {
		  CG_Error( "CG_ParseTeamInfo: bad client number: %d", client );
		  return;
		}

		sortedTeamPlayers[i] = client;

		cgs.clientinfo[ client ].location		= atoi( CG_Argv( index + 3 ) );
		cgs.clientinfo[ client ].health			= atoi( CG_Argv( index + 4 ) );
		cgs.clientinfo[ client ].armor			= atoi( CG_Argv( index + 5 ) );
		cgs.clientinfo[ client ].curWeapon		= atoi( CG_Argv( index + 6 ) );
		cgs.clientinfo[ client ].powerups		= atoi( CG_Argv( index + 7 ) );
		cgs.clientinfo[ client ].numCartridges	= atoi( CG_Argv( index + 8 ) );
	}
}


/*
================
CG_ParseServerinfo

This is called explicitly when the gamestate is first received,
and whenever the server updates any serverinfo flagged cvars
================
*/
void CG_ParseServerinfo( void ) {
	const char	*info;
	char	*mapname;

	info = CG_ConfigString( CS_SERVERINFO );
	cgs.gametype = atoi( Info_ValueForKey( info, "g_gametype" ) );
	trap_Cvar_Set("g_gametype", va("%i", cgs.gametype));
	cgs.dmflags = atoi( Info_ValueForKey( info, "dmflags" ) );
	cgs.teamflags = atoi( Info_ValueForKey( info, "teamflags" ) );
	cgs.fraglimit = 
	cgs.capturelimit = atoi( Info_ValueForKey( info, "pointlimit" ) );
	strcpy(cgs.shortmapname,Info_ValueForKey( info, "mapname" ));
	Q_strncpyz(cgs.servername,Info_ValueForKey( info, "sv_hostname" ),32);
	cgs.lpsflags = atoi( Info_ValueForKey( info, "g_LPS_flags" ) );
	cgs.lpsStartLives = atoi( Info_ValueForKey( info, "g_LPS_startlives" ) );
	cgs.timelimit = atoi( Info_ValueForKey( info, "timelimit" ) );
	cgs.maxclients = atoi( Info_ValueForKey( info, "sv_maxclients" ) );
	mapname = Info_ValueForKey( info, "mapname" );
	Com_sprintf( cgs.mapname, sizeof( cgs.mapname ), "maps/%s.bsp", mapname );
}

/*
==================
CG_ParseWarmup
==================
*/
static void CG_ParseWarmup( void ) {
	const char	*info;
	int			warmup;

	info = CG_ConfigString( CS_WARMUP );

	warmup = atoi( info );
	cg.warmupCount = -1;

	if ( warmup == 0 && cg.warmup ) {

	} else if ( warmup > 0 && cg.warmup <= 0 ) {
		{
//			trap_S_StartLocalSound( cgs.media.countPrepareSound, CHAN_ANNOUNCER );
		}
	}

	cg.warmup = warmup;
}

/*
================
CG_SetConfigValues

Called on load to set the initial values from configure strings
================
*/
void CG_SetConfigValues( void ) {
	const char *s;

	cgs.scores1 = atoi( CG_ConfigString( CS_SCORES1 ) );
	cgs.scores2 = atoi( CG_ConfigString( CS_SCORES2 ) );
	Q_strncpyz( cgs.balloonState, CG_ConfigString( CS_BALLOONS ), sizeof( cgs.balloonState ) );
	cgs.levelStartTime = atoi( CG_ConfigString( CS_LEVEL_START_TIME ) );
	if( cgs.gametype == GT_CTF ) {
		s = CG_ConfigString( CS_FLAGSTATUS );
		cgs.redflag = s[0] - '0';
		cgs.blueflag = s[1] - '0';
	}
	cg.warmup = atoi( CG_ConfigString( CS_WARMUP ) );
}

/*
=====================
CG_ShaderStateChanged
=====================
*/
void CG_ShaderStateChanged(void) {
	char originalShader[MAX_QPATH];
	char newShader[MAX_QPATH];
	char timeOffset[16];
	const char *o;
	char *n,*t;

	o = CG_ConfigString( CS_SHADERSTATE );
	while (o && *o) {
		n = strstr(o, "=");
		if (n && *n) {
			strncpy(originalShader, o, n-o);
			originalShader[n-o] = 0;
			n++;
			t = strstr(n, ":");
			if (t && *t) {
				strncpy(newShader, n, t-n);
				newShader[t-n] = 0;
			} else {
				break;
			}
			t++;
			o = strstr(t, "@");
			if (o) {
				strncpy(timeOffset, t, o-t);
				timeOffset[o-t] = 0;
				o++;
				trap_R_RemapShader( originalShader, newShader, timeOffset );
			}
		} else {
			break;
		}
	}
}

/*
================
CG_UpdateBalloonStates
================
*/
void CG_UpdateBalloonStates( const char *str ) {

	if ( str[0] )
	{
		char team;
		int i;

		// test for ownage
		team = str[0];
		for ( i = 1; i < 8 && str[i]; i++ )
			if ( team != str[i] ) {
				team = '0';
				break;
			}
		if ( team == '1' || team == '2' ) 
		{
			// announce
			if ( team == '1' ) trap_S_StartLocalSound(cgs.media.Announcer_BigBalloonRed, CHAN_ANNOUNCER);
			else if ( team == '2' ) trap_S_StartLocalSound(cgs.media.Announcer_BigBalloonBlue, CHAN_ANNOUNCER);

			// copy the new string
			Q_strncpyz( cgs.balloonState, str, sizeof(cgs.balloonState) );
			return;
		}

		// announce normal stuff
		for ( i = 0; ( ( i < MAX_BALLOONS ) && str[i] && cgs.balloonState[i] ); i++ ) {
			if ( str[i] != cgs.balloonState[i] ) {
				switch ( str[i] ) {
					case '0':
						CG_AddBufferedSound( cgs.media.Announcer_BalloonDestroyed );
						break;
					case '1':
						CG_AddBufferedSound( cgs.media.Announcer_RedBalloon );
						break;
					case '2':
						CG_AddBufferedSound( cgs.media.Announcer_BlueBalloon );
						break;

					default:
						break;
				}
			}
		}
	}

	// copy the new string
	Q_strncpyz( cgs.balloonState, str, sizeof(cgs.balloonState) );
}

/*
================
CG_ConfigStringModified

================
*/
static void CG_ConfigStringModified( void ) {
	const char	*str;
	int		num;

	num = atoi( CG_Argv( 1 ) );

	// get the gamestate from the client system, which will have the
	// new configstring already integrated
	trap_GetGameState( &cgs.gameState );

	// look up the individual string that was modified
	str = CG_ConfigString( num );

	// do something with it if necessary
	if ( num == CS_MUSIC ) {
		CG_StartMusic();
	} else if ( num == CS_SERVERINFO ) {
		CG_ParseServerinfo();
	} else if ( num == CS_WARMUP ) {
		CG_ParseWarmup();
	} else if ( num == CS_SCORES1 ) {
		cgs.scores1 = atoi( str );
	} else if ( num == CS_SCORES2 ) {
		cgs.scores2 = atoi( str );
	} else if ( num == CS_BALLOONS ) {
		CG_UpdateBalloonStates( str );
	} else if ( num == CS_LEVEL_START_TIME ) {
		cgs.levelStartTime = atoi( str );
	} else if ( num == CS_VOTE_TIME ) {
		cgs.voteTime = atoi( str );
		cgs.voteModified = qtrue;
	} else if ( num == CS_VOTE_YES ) {
		cgs.voteYes = atoi( str );
		cgs.voteModified = qtrue;
	} else if ( num == CS_VOTE_NO ) {
		cgs.voteNo = atoi( str );
		cgs.voteModified = qtrue;
	} else if ( num == CS_VOTE_STRING ) {
		Q_strncpyz( cgs.voteString, str, sizeof( cgs.voteString ) );
	} else if ( num >= CS_TEAMVOTE_TIME && num <= CS_TEAMVOTE_TIME + 1) {
		cgs.teamVoteTime[num-CS_TEAMVOTE_TIME] = atoi( str );
		cgs.teamVoteModified[num-CS_TEAMVOTE_TIME] = qtrue;
	} else if ( num >= CS_TEAMVOTE_YES && num <= CS_TEAMVOTE_YES + 1) {
		cgs.teamVoteYes[num-CS_TEAMVOTE_YES] = atoi( str );
		cgs.teamVoteModified[num-CS_TEAMVOTE_YES] = qtrue;
	} else if ( num >= CS_TEAMVOTE_NO && num <= CS_TEAMVOTE_NO + 1) {
		cgs.teamVoteNo[num-CS_TEAMVOTE_NO] = atoi( str );
		cgs.teamVoteModified[num-CS_TEAMVOTE_NO] = qtrue;
	} else if ( num >= CS_TEAMVOTE_STRING && num <= CS_TEAMVOTE_STRING + 1) {
		Q_strncpyz( cgs.teamVoteString[num-CS_TEAMVOTE_STRING], str, sizeof( cgs.teamVoteString ) );
	} else if ( num == CS_INTERMISSION ) {
		cg.intermissionStarted = atoi( str );
	} else if ( num >= CS_MODELS && num < CS_MODELS+MAX_MODELS ) {
		cgs.gameModels[ num-CS_MODELS ] = trap_R_RegisterModel( str );
	} else if ( num >= CS_SOUNDS && num < CS_SOUNDS+MAX_MODELS ) {
		if ( str[0] != '*' ) {	// player specific sounds don't register here
			cgs.gameSounds[ num-CS_SOUNDS] = trap_S_RegisterSound( str, qfalse );
		}
	} else if ( num >= CS_PLAYERS && num < CS_PLAYERS+MAX_CLIENTS ) {
		CG_NewClientInfo( num - CS_PLAYERS );
		CG_BuildSpectatorString();
	} else if ( num == CS_FLAGSTATUS ) {
		if( cgs.gametype == GT_CTF ) {
			// format is rb where its red/blue, 0 is at base, 1 is taken, 2 is dropped
			cgs.redflag = str[0] - '0';
			cgs.blueflag = str[1] - '0';
		}
	}
	else if ( num == CS_SHADERSTATE ) {
		CG_ShaderStateChanged();
	}
		
}


/*
=======================
CG_AddToTeamChat

=======================
*/
static void CG_AddToTeamChat( const char *str ) {
	int len;
	char *p, *ls;
	int lastcolor;
	int chatHeight;

	if (cg_teamChatHeight.integer < TEAMCHAT_HEIGHT) {
		chatHeight = cg_teamChatHeight.integer;
	} else {
		chatHeight = TEAMCHAT_HEIGHT;
	}

	if (chatHeight <= 0 || cg_teamChatTime.integer <= 0) {
		// team chat disabled, dump into normal chat
		cgs.teamChatPos = cgs.teamLastChatPos = 0;
		return;
	}

	len = 0;

	p = cgs.teamChatMsgs[cgs.teamChatPos % chatHeight];
	*p = 0;

	lastcolor = '7';

	ls = NULL;
	while (*str) {
		if (len > TEAMCHAT_WIDTH - 1) {
			if (ls) {
				str -= (p - ls);
				str++;
				p -= (p - ls);
			}
			*p = 0;

			cgs.teamChatMsgTimes[cgs.teamChatPos % chatHeight] = cg.time;

			cgs.teamChatPos++;
			p = cgs.teamChatMsgs[cgs.teamChatPos % chatHeight];
			*p = 0;
			*p++ = Q_COLOR_ESCAPE;
			*p++ = lastcolor;
			len = 0;
			ls = NULL;
		}

		if ( Q_IsColorString( str ) ) {
			*p++ = *str++;
			lastcolor = *str;
			*p++ = *str++;
			continue;
		}
		if (*str == ' ') {
			ls = p;
		}
		*p++ = *str++;
		len++;
	}
	*p = 0;

	cgs.teamChatMsgTimes[cgs.teamChatPos % chatHeight] = cg.time;
	cgs.teamChatPos++;

	if (cgs.teamChatPos - cgs.teamLastChatPos > chatHeight)
		cgs.teamLastChatPos = cgs.teamChatPos - chatHeight;
}

static void CG_RestartUIMusic( void ) {
	// Restart q3_ui music player.
	// UI does not know about map_restart, which stops background tracks, i.e.
	// music
	trap_SendConsoleCommand( "wop_restartmusic\n" );
}

/*
===============
CG_MapRestart

The server has issued a map_restart, so the next snapshot
is completely new and should not be interpolated to.

A tournement restart will clear everything, but doesn't
require a reload of all the media
===============
*/
static void CG_MapRestart( void ) {
	if ( cg_showmiss.integer ) {
		CG_Printf( "CG_MapRestart\n" );
	}

	CG_InitLocalEntities();
	CG_InitMarkPolys();
	CG_ClearParticles ();

	Init_SprayLogoSys();

	Init_SpriteParticles();

	// make sure the "3 frags left" warnings play again
	cg.fraglimitWarnings = 0;

	cg.timelimitWarnings = 0;

	cg.intermissionStarted = qfalse;
	cg.levelShot = qfalse;

	cgs.voteTime = 0;

	cg.mapRestart = qtrue;

	CG_StartMusic();
	CG_RestartUIMusic();

	trap_S_ClearLoopingSounds(qtrue);

	// we really should clear more parts of cg here and stop sounds

	// play the "fight" sound if this is a restart without warmup
	if ( cg.warmup == 0 /* && cgs.gametype == GT_TOURNAMENT */) {
		trap_S_StartLocalSound( cgs.media.countFightSound, CHAN_ANNOUNCER );
	}
	trap_Cvar_Set("cg_thirdPerson", "0");
}

/*
=================
CG_RemoveChatEscapeChar
=================
*/
// TODO: Sync escapechar with g_cmds. and match.h!
#define EC		'\x19'
static void CG_RemoveChatEscapeChar( char *text ) {
	int i, l;

	l = 0;
	for ( i = 0; text[i]; i++ ) {
		if (text[i] == EC )
			continue;
		text[l++] = text[i];
	}
	text[l] = '\0';
}

/*
=================
CG_ServerCommand

The string has been tokenized and can be retrieved with
Cmd_Argc() / Cmd_Argv()
=================
*/
static void CG_ServerCommand( void ) {
	const char	*cmd;
	char		text[MAX_SAY_TEXT];
	int i, id;

	cmd = CG_Argv(0);

	if ( !cmd[0] ) {
		// server claimed the command
		return;
	}

	if ( !strcmp( cmd, "cp" ) ) {
		CG_CenterPrint( CG_Argv(1), SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
		return;
	}

	if ( !strcmp( cmd, "mp" ) ) {
		CG_MessagePrint( CG_Argv(1) );
		return;
	}

	if ( !strcmp( cmd, "cs" ) ) {
		CG_ConfigStringModified();
		return;
	}

	if ( !strcmp( cmd, "print" ) ) {
		CG_Printf( "%s", CG_Argv(1) );
		return;
	}


	#define CHATBEEP_CHAT	1
	#define CHATBEEP_TCHAT	2
	#define CHATBEEP_TELL	4
	// say $mode $cid $text
	if ( !strcmp( cmd, "say" ) ) {
		int mode = atoi( CG_Argv( 1 ) );
		// Always print SAY_TELL
		if ( cg_teamChatsOnly.integer && ( mode == SAY_ALL ) ) {
			return;
		}

		switch ( mode ) {
			case SAY_ALL:
				if ( cg_chatBeep.integer & CHATBEEP_CHAT ) {
					trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
				}
				break;
			case SAY_TEAM:
				if ( cg_chatBeep.integer & CHATBEEP_TCHAT ) {
					trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
				}
				break;
			case SAY_TELL:
				if ( cg_chatBeep.integer & CHATBEEP_TELL ) {
					trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
				}
				break;

			default:
				CG_Printf( "Unknown say mode %i.\n", mode );
				return;
		}

		Q_strncpyz( text, CG_Argv( 3 ), sizeof( text ) );
		CG_RemoveChatEscapeChar( text );
		if ( mode == SAY_TEAM ) {
			CG_AddToTeamChat( text );
		}

		CG_Printf( "%s\n", text );

		// This is a little hack. CG_Printf() has now copied the message
		// into the cg.chat buffer and we set the icon afterwards
		// TODO: Check for valid clientid before doing anything
		id = atoi( CG_Argv( 2 ) );
		if ( ( id >= 0 ) && ( id < cgs.maxclients ) ) {
			cg.chaticons[cg.lastchatmsg] = cgs.clientinfo[id].modelIcon;
		}
		else if ( id == -1 ) {
			// might be console say
			cg.chaticons[cg.lastchatmsg] = cgs.media.deferShader;
		}

		return;
	}

	
	if ( !strcmp( cmd, "scores" ) ) {
		CG_ParseScores();
		return;
	}

	if ( !strcmp( cmd, "tinfo" ) ) {
		CG_ParseTeamInfo();
		return;
	}

	if ( !strcmp( cmd, "map_restart" ) ) {
		CG_MapRestart();
		return;
	}

	// loaddeferred can be both a servercmd and a consolecmd
	if ( !strcmp( cmd, "loaddefered" ) ) {	// FIXME: spelled wrong, but not changing for demo
		CG_LoadDeferredPlayers();
		return;
	}

	// clientLevelShot is sent before taking a special screenshot for
	// the menu system during development
	if ( !strcmp( cmd, "clientLevelShot" ) ) {
		cg.levelShot = qtrue;
		return;
	}

	// "sprayroom weapon change"
	if ( !strcmp( cmd, "srwc" ) ){
		id = atoi( CG_Argv( 1 ) );
		CG_WeaponSRWC( id );
		return;
	}

	// "client do it"
	if ( !strcmp( cmd, "cdi" ) ) {
		id = atoi( CG_Argv( 1 ) );

		// FIXME: Magical constants!
		switch ( id ) {
			// sprayroom goodbye
			case 0:
				i = (int)( random() * 1.9999f );
				trap_S_StartLocalSound( cgs.media.Announcer_SRfrag[i], CHAN_ANNOUNCER );
				break;

			// sprayed on wrong wall
			case 1:
				i = atoi( CG_Argv( 2 ) );
				trap_S_StartLocalSound( cgs.media.wrongLogo[i], CHAN_ANNOUNCER );
				break;

			// LPS lost
			case 2:
				trap_S_StartLocalSound( cgs.media.YouLooseSound, CHAN_ANNOUNCER );
				trap_S_StartLocalSound( cgs.media.YouLooseSound, CHAN_ANNOUNCER );
				break;

			// select syc-logo
			// TODO: Remove!
			case 3:
			{
				char buffer[64];

				trap_Cvar_VariableStringBuffer( "syc_logo", buffer, sizeof( buffer ) );

				if ( buffer[0] != '\0' ) {
					trap_SendClientCommand( va( "selectlogo \"%s\"\n", buffer ) );
				}
				else {
					trap_SendClientCommand("selectlogo " SPRAYLOGO_DEFAULT_NAME "\n");
				}

				cg.logoselected = qtrue;
				break;
			}

			default:
				break;
		}
		return;
	}

	if ( !strcmp( cmd, "qc" ) ) {
		CG_QueryCvar();
		return;
	}


	CG_Printf( "Unknown client game command: %s\n", cmd );
}


/*
====================
CG_ExecuteNewServerCommands

Execute all of the server commands that were received along
with this this snapshot.
====================
*/
void CG_ExecuteNewServerCommands( int latestSequence ) {
	while ( cgs.serverCommandSequence < latestSequence ) {
		if ( trap_GetServerCommand( ++cgs.serverCommandSequence ) ) {
			CG_ServerCommand();
		}
	}
}
