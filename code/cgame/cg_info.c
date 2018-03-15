// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_info.c -- display information while data is being loading

#include "cg_local.h"

#define MAX_LOADING_PLAYER_ICONS	16
#define MAX_LOADING_ITEM_ICONS		26

static int			loadingPlayerIconCount;
static int			loadingItemIconCount;
static qhandle_t	loadingPlayerIcons[MAX_LOADING_PLAYER_ICONS];
static qhandle_t	loadingItemIcons[MAX_LOADING_ITEM_ICONS];


#if 0
/*
===================
CG_DrawLoadingIcons
===================
*/
static void CG_DrawLoadingIcons( void ) {
	int		n;
	int		x, y;

	for( n = 0; n < loadingPlayerIconCount; n++ ) {
		x = 16 + n * 78;
		y = 324-40;
		CG_DrawPic( x, y, 64, 64, loadingPlayerIcons[n] );
	}

	for( n = 0; n < loadingItemIconCount; n++ ) {
		y = 400-40;
		if( n >= 13 ) {
			y += 40;
		}
		x = 16 + n % 13 * 48;
		CG_DrawPic( x, y, 32, 32, loadingItemIcons[n] );
	}
}
#endif

/*
======================
CG_LoadingString

======================
*/
void CG_LoadingString( const char *s ) {
	Q_strncpyz( cg.infoScreenText, s, sizeof( cg.infoScreenText ) );

	trap_UpdateScreen();
}

/*
#######################
CG_ChangeLoadingProgress
#######################
*/
void CG_ChangeLoadingProgress( float f ) {
	cg.loadingprogress = f;

	trap_UpdateScreen();
}

/*
===================
CG_LoadingItem
===================
*/
void CG_LoadingItem( int itemNum ) {
	gitem_t		*item;

	item = &bg_itemlist[itemNum];
	
	if ( item->icon && loadingItemIconCount < MAX_LOADING_ITEM_ICONS ) {
		loadingItemIcons[loadingItemIconCount++] = trap_R_RegisterShaderNoMip( item->icon );
	}

	CG_LoadingString( item->pickup_name );
}

/*
===================
CG_LoadingClient
===================
*/
void CG_LoadingClient( int clientNum ) {
	const char		*info;
	char			*skin;
	char			personality[MAX_QPATH];
	char			model[MAX_QPATH];
	char			iconName[MAX_QPATH];

	info = CG_ConfigString( CS_PLAYERS + clientNum );

	if ( loadingPlayerIconCount < MAX_LOADING_PLAYER_ICONS ) {
		Q_strncpyz( model, Info_ValueForKey( info, "model" ), sizeof( model ) );
		skin = strrchr( model, '/' );
		if ( skin ) {
			*skin++ = '\0';
		} else {
			skin = DEFAULT_SKIN;
		}

		Com_sprintf( iconName, MAX_QPATH, "models/wop_players/%s/icon_%s.tga", model, skin );
		
		loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		if ( !loadingPlayerIcons[loadingPlayerIconCount] ) {
			Com_sprintf( iconName, MAX_QPATH, "models/wop_players/characters/%s/icon_%s.tga", model, skin );
			loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		}
		if ( !loadingPlayerIcons[loadingPlayerIconCount] ) {
			Com_sprintf( iconName, MAX_QPATH, "models/wop_players/%s/icon_%s.tga", DEFAULT_MODEL, DEFAULT_SKIN );
			loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		}
		if ( loadingPlayerIcons[loadingPlayerIconCount] ) {
			loadingPlayerIconCount++;
		}
	}

	Q_strncpyz( personality, Info_ValueForKey( info, "n" ), sizeof(personality) );
	Q_CleanStr( personality );

	CG_LoadingString( personality );
}

/*
====================
CG_DrawInformation

Draw all the status / pacifier stuff during level loading
====================
*/
void CG_DrawInformation( void ) {
	const char	*s;
	const char	*info;
	const char	*sysInfo;
//	int			y;
//	int			value;
	qhandle_t	levelshot;
	qhandle_t	helppage;
//	char		buf[1024];
	float		lx,ly,lw,lh;//l->loading
	// levelshot ideal coords (assuming 1024*768 resolution)
	int lsiX = 167;
	int lsiY = 125;
	int lsiW = 690;
	int lsiH = 517;
	float idealAspectRatio = 4.0/3;
	int lsCenterDistX;			// x - distance to center
	int lsX, lsY, lsW, lsH;		// actual levelshot coords

	info = CG_ConfigString( CS_SERVERINFO );
	sysInfo = CG_ConfigString( CS_SYSTEMINFO );

	trap_R_SetColor( NULL );

	CG_DrawPic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, trap_R_RegisterShaderNoMip("loadingscreen/connecting"));
	
	s = Info_ValueForKey( info, "mapname" );
	levelshot = trap_R_RegisterShaderNoMip( va( "levelshots/%s", s ) );
	if ( !levelshot ) {
		levelshot = trap_R_RegisterShaderNoMip( "levelshots/unknownmap" );
	}

	switch ( cgs.gametype ) {
		case GT_BALLOON:
			info = "menu/help/loadinghelp_bb.tga";
			break;
		case GT_CTF:
			info = "menu/help/loadinghelp_ctl.tga";
			break;
		case GT_FFA:
			info = "menu/help/loadinghelp_ffa.tga";
			break;
		case GT_LPS:
			info = "menu/help/loadinghelp_lps.tga";
			break;
		case GT_SPRAYFFA:
			info = "menu/help/loadinghelp_syc.tga";
			break;
		case GT_SPRAY:
			info = "menu/help/loadinghelp_teamsyc.tga";
			break;
		case GT_TEAM:
			info = "menu/help/loadinghelp_teamffa.tga";
			break;

		default:
			info = "menu/help/loadinghelp_ffa.tga";
			break;
	}

	helppage = trap_R_RegisterShaderNoMip( info );


	// now scale x and width to preserve the levelshot aspect ratio
	lsCenterDistX = (512 - lsiX) * idealAspectRatio / cgs.glconfig.windowAspect;
	lsX = 512 - lsCenterDistX;
	lsY = lsiY;
	lsW = (float)lsiW * (idealAspectRatio / cgs.glconfig.windowAspect);
	lsH = lsiH;

	CG_DrawPic1024( lsX, lsY, lsW, lsH, levelshot );
	CG_DrawPic1024( lsX, lsY, lsW, lsH, helppage );

//test werte: 300,650,424,40
//von minibild: 282,675,460,48
	lx=282;
	ly=675;
	lw=460*cg.loadingprogress;
	lh=48;
	CG_AdjustFrom1024(&lx,&ly,&lw,&lh);
	trap_R_DrawStretchPic(lx,ly,lw,lh,0,0,cg.loadingprogress,1,trap_R_RegisterShaderNoMip("loadingscreen/ladebalken"));
	CG_DrawPic1024(282,675,460,48,trap_R_RegisterShaderNoMip("loadingscreen/ladefenster"));

	return;

	#if 0
	// draw the icons of things as they are loaded
	CG_DrawLoadingIcons();

	// the first 150 rows are reserved for the client connection
	// screen to write into
	if ( cg.infoScreenText[0] ) {
		UI_DrawProportionalString( 320, 128-32, va("Loading... %s", cg.infoScreenText),
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
	} else {
		UI_DrawProportionalString( 320, 128-32, "Awaiting snapshot...",
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
	}

	// draw info string information

	y = 180-32;

	// don't print server lines if playing a local game
	trap_Cvar_VariableStringBuffer( "sv_running", buf, sizeof( buf ) );
	if ( !atoi( buf ) ) {
		// server hostname
		Q_strncpyz(buf, Info_ValueForKey( info, "sv_hostname" ), 1024);
		Q_CleanStr(buf);
		UI_DrawProportionalString( 320, y, buf,
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
		y += PROP_HEIGHT;

		// pure server
		s = Info_ValueForKey( sysInfo, "sv_pure" );
		if ( s[0] == '1' ) {
			UI_DrawProportionalString( 320, y, "Pure Server",
				UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
			y += PROP_HEIGHT;
		}

		// server-specific message of the day
		s = CG_ConfigString( CS_MOTD );
		if ( s[0] ) {
			UI_DrawProportionalString( 320, y, s,
				UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
			y += PROP_HEIGHT;
		}

		// some extra space after hostname and motd
		y += 10;
	}

	// map-specific message (long map name)
	s = CG_ConfigString( CS_MESSAGE );
	if ( s[0] ) {
		UI_DrawProportionalString( 320, y, s,
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
		y += PROP_HEIGHT;
	}

	// cheats warning
	s = Info_ValueForKey( sysInfo, "sv_cheats" );
	if ( s[0] == '1' ) {
		UI_DrawProportionalString( 320, y, "CHEATS ARE ENABLED",
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
		y += PROP_HEIGHT;
	}

	// game type
	switch ( cgs.gametype ) {
	case GT_FFA:
		s = "Free For All";
		break;
	case GT_SINGLE_PLAYER:
		s = "Single Player";
		break;
	case GT_TOURNAMENT:
		s = "Tournament";
		break;
	case GT_TEAM:
		s = "Team Deathmatch";
		break;
	case GT_CTF:
		s = "Capture the Lolly";
		break;
	case GT_SPRAY:
		s = "TeamPlay Spraymode";
		break;
	case GT_SPRAYFFA:
		s = "FFA Spraymode";
		break;
	default:
		s = "Unknown Gametype";
		break;
	}
	UI_DrawProportionalString( 320, y, s,
		UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
	y += PROP_HEIGHT;
		
	value = atoi( Info_ValueForKey( info, "timelimit" ) );
	if ( value ) {
		UI_DrawProportionalString( 320, y, va( "timelimit %i", value ),
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
		y += PROP_HEIGHT;
	}

	if (cgs.gametype < GT_CTF ) {
		value = atoi( Info_ValueForKey( info, "pointlimit" ) );
			UI_DrawProportionalString( 320, y, va( "pointlimit %i", value ),
				UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
		if ( value ) {
			y += PROP_HEIGHT;
		}
	}

	if (cgs.gametype >= GT_CTF) {
		value = atoi( Info_ValueForKey( info, "pointlimit" ) );
		if ( value ) {
			UI_DrawProportionalString( 320, y, va( "pointlimit %i", value ),
				UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
			y += PROP_HEIGHT;
		}
	}
	#endif
}

