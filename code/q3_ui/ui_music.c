/*
	World of Padman - Music player and menu
*/

/*
 * README
 * 
 * The current implementation of the music player is rather basic, but it works.
 * Here are some toughts on improvements;
 *   Drop the need for the <nextsongCMD> in engine by
 *     having some sort of config for each album like
 *
 *       name "Name of the ^6great^7 album with spaces, color and such"
 *       track "filename.ext" "Title of the track with ^3color etc." "length-in-seconds"
 * 
 *     This would allow to remove the hack since we can check whether enough time
 *     passed to play a new song based on the length of the current one.
 *     It also allows to specify a fancy name for the entire album and each track,
 *     as well as giving more freedom/safety with filenames. Track numbers could
 *     either be explicit (i.e. part of the "track"-line in the config) or implicit
 *     (i.e. n-th track line equals n-th track).
 *     One could add a fade effect between songs (by setting s_musicvolume and having
 *     a separate ui_musicvolume).
 *     As a sidenote this'd also allow to include "hidden" bonus tracks which
 *     are simply not listed in the album.cfg
 *   See note in init about subdirs/formats.
 *   On track change, display album/track/(length)
 */

#include "ui_local.h"

#define MUSIC_BG_ARM "wopmusic/bg_arm.tga"

#define MAX_TRACKS			15
#define MAX_ALBUMS			10
#define MAX_TRACKNAME		64
#define MAX_ALBUMNAME		32

#define ID_TRACK1				10
#define ID_EXIT					( ID_TRACK1 + MAX_TRACKS )
#define ID_SWITCH				( ID_EXIT + 1 )
#define ID_CLEARLIST			( ID_SWITCH + 1 )
#define ID_PLAYALL				( ID_CLEARLIST + 1 )

typedef enum {
	MUSICSWITCH_OUT		= -1,
	MUSICSWITCH_NONE	=  0,
	MUSICSWITCH_IN		=  1,
	MUSICSWITCH_QUIT	= -2
} musicMenuSwitch_t;

typedef struct {
	menuframework_s		menu;

	menutext_s			tracks[MAX_TRACKS];
	menubitmap1024s_s	exit;
	menubitmap1024s_s	switchAlbum;
	menubitmap1024s_s	clearList;
	menubitmap1024s_s	playAll;

	qhandle_t			background;
	musicMenuSwitch_t 	switchState;
	int					switchTime;

	int					currentAlbum;
} musicMenu_t;

static musicMenu_t	musicMenu;

typedef struct {
	char	title[MAX_TRACKNAME];
	char 	file[MAX_QPATH];
} trackInfo_t;

typedef struct {
	trackInfo_t		tracks[MAX_TRACKS];
	qhandle_t		background;
	char			name[MAX_ALBUMNAME];
	int				numTracks;
} albumInfo_t;

typedef struct playOrder_s {
	struct playOrder_s	*next;
	int					album;
	int					track;
} playOrder_t;

static playOrder_t playOrderMemory[( MAX_TRACKS * MAX_ALBUMS )];
static playOrder_t *freeOrderItems;

typedef struct {
	albumInfo_t		albums[MAX_ALBUMS];

	playOrder_t		*playOrder;
	playOrder_t		*lastplayOrderItem;
	int				numAlbums;
	int				songStarted;
} musicInfo_t;

static musicInfo_t	musicInfo;

#define PLAYLIST_FILENAME_S "playlist.dat"

#define MUSIC_MESSAGEPREFIX_S "[WoP Music]: "


static void Playlist_AddTrack( int album, int track, qboolean init ) {
	playOrder_t *playOrder, *playOrder2;

	// are there already songs in the playlist?
	if ( NULL != musicInfo.playOrder ) {
		// are we currently playing the song to be added?
		if ( ( track == musicInfo.playOrder->track ) && ( album == musicInfo.playOrder->album ) ) {
			// stop it
			musicInfo.songStarted = 0;
			trap_S_StopBackgroundTrack();

			// if it was the last track, 
			if ( musicInfo.playOrder == musicInfo.lastplayOrderItem ) {
				musicInfo.lastplayOrderItem = NULL;
			}
			playOrder = musicInfo.playOrder;
			musicInfo.playOrder = playOrder->next;
			playOrder->next = freeOrderItems;
			freeOrderItems = playOrder;

			return;
		}
		else {
			// iterate through playlist and find track in album
			for ( playOrder = ( playOrder2 = musicInfo.playOrder )->next; playOrder != NULL; playOrder = playOrder->next ) {
				if ( ( album == playOrder->album ) && ( track == playOrder->track ) ) {
					if ( playOrder == musicInfo.lastplayOrderItem ) {
						musicInfo.lastplayOrderItem = playOrder2;
					}
					playOrder2->next = playOrder->next;
					playOrder->next = freeOrderItems;
					freeOrderItems = playOrder;

					return;
				}

				playOrder2 = playOrder;
			}
		}
	}

	// no more free items, *should* never happen since playOrderMemory is large enough
	if ( NULL == freeOrderItems ) {
		Com_Printf( MUSIC_MESSAGEPREFIX_S "^1selected too many tracks!\n" );
		return;
	}

	playOrder = freeOrderItems;
	freeOrderItems = freeOrderItems->next;

	if ( NULL == musicInfo.playOrder ) {
		musicInfo.playOrder = musicInfo.lastplayOrderItem = playOrder;
	}
	else {
		musicInfo.lastplayOrderItem->next = playOrder;
		musicInfo.lastplayOrderItem = playOrder;
	}

	playOrder->next = NULL;
	playOrder->album = album;
	playOrder->track = track;

	// if we're not initializing, check whether to play that song
	if ( !init ) {
		Music_Check();
	}
}


static void Playlist_Clear( void ) {
	playOrder_t *playOrder;

	if ( NULL == freeOrderItems ) {
		freeOrderItems = musicInfo.playOrder;
	}
	else {
		for ( playOrder = freeOrderItems; playOrder->next !=NULL ; playOrder = playOrder->next ) {
			// do nothing, just loop
		}

		playOrder->next = musicInfo.playOrder;
	}

	musicInfo.playOrder = musicInfo.lastplayOrderItem = NULL;

	musicInfo.songStarted = 0;
	trap_S_StopBackgroundTrack();
}


static void Playlist_AddAllTracks( qboolean init ) {
	int a, t;

	Playlist_Clear();

	for ( a = 0; a < musicInfo.numAlbums; ++a ) {
		for ( t = 0; t < musicInfo.albums[a].numTracks; ++t ) {
			Playlist_AddTrack( a, t, init );
		}
	}
}


static int QDECL SortTracks( const void *a, const void *b ) {
	trackInfo_t *ta, *tb;

	ta = (trackInfo_t*)a;
	tb = (trackInfo_t*)b;

	return Q_stricmp( ta->file, tb->file );
}


// snd_restart causes shutdown without initialization
static qboolean musicInitialized = qfalse;


void MusicMenu_Init( void ) {
	char			albumDirList[1024];
	char			trackDirList[1024];
	char			*albumStr, *trackStr;
	int				albumDirLen, numAlbumDirs, a;
	int				trackDirLen, numTrackDirs, t;
	char			albumName[32];
	int				numTracks;

	musicInitialized = qtrue;
	memset( &musicInfo, 0, sizeof( musicInfo) );

	memset( &playOrderMemory, 0, sizeof( playOrderMemory ) );
	freeOrderItems = playOrderMemory;
	for ( a = 0; a < ( ( MAX_TRACKS * MAX_ALBUMS ) - 1 ); ++a ) {
		playOrderMemory[a].next = &playOrderMemory[( a + 1 )];
	}

	numAlbumDirs = trap_FS_GetFileList( "wopmusic", "/", albumDirList, sizeof( albumDirList ) );
	albumStr  = albumDirList;
	if ( numAlbumDirs > MAX_ALBUMS ) {
		numAlbumDirs = MAX_ALBUMS;
	}
	for ( a = 0; a < numAlbumDirs; ++a, albumStr += ( albumDirLen + 1 ) ) {
		if ( '\0' == *albumStr ) {
			albumDirLen = 0;
			continue;
		}

		albumDirLen = strlen( albumStr );

		if ( !Q_stricmp( albumStr, "." ) || !Q_stricmp( albumStr, ".." ) ) {
			continue;
		}

		// '/' is missing if the directory isn't in a pk3
		if ( '/' == albumStr[( albumDirLen - 1 )] ) {
			Q_strncpyz( albumName, albumStr, sizeof( albumName ) );
		}
		else {
			Com_sprintf( albumName, sizeof( albumName ), "%s/", albumStr );
		}
		Q_strncpyz( musicInfo.albums[musicInfo.numAlbums].name, albumStr, sizeof( musicInfo.albums[musicInfo.numAlbums].name ) );

		musicInfo.albums[musicInfo.numAlbums].background = trap_R_RegisterShaderNoMip( va( "wopmusic/%sbg", albumName ) );

		// Load tracks for each supported format, currently .ogg only

		// it'd be best if songs were in a subdirectory (without any non-track-files like bg)
		// so we could omit the .foo extension and just load all files from that dir

		numTracks = 0;
		numTrackDirs = trap_FS_GetFileList( va( "wopmusic/%s", albumName ), ".ogg", trackDirList, sizeof( trackDirList ) );
		trackStr = trackDirList;
		// numTracks is the number of .wav that were found previously
		if ( numTrackDirs > ( MAX_TRACKS - numTracks ) ) {
			numTrackDirs = ( MAX_TRACKS - numTracks );
		}

		for ( t = 0; t < numTrackDirs; ++t, trackStr += ( trackDirLen + 1 ) ) {
			if ( '\0' == *trackStr ) {
				trackDirLen = 0;
				continue;
			}

			trackDirLen = strlen( trackStr );

			Com_sprintf( musicInfo.albums[musicInfo.numAlbums].tracks[numTracks].file, sizeof( musicInfo.albums[musicInfo.numAlbums].tracks[numTracks].file ), "wopmusic/%s%s", albumName, trackStr );

			// "09_pad-anthem (credits).ogg"
			Q_strncpyz( musicInfo.albums[musicInfo.numAlbums].tracks[numTracks].title, (char *)( trackStr + 3 ), sizeof( musicInfo.albums[musicInfo.numAlbums].tracks[numTracks].title ) );
			*strstr( musicInfo.albums[musicInfo.numAlbums].tracks[numTracks].title, ".ogg" ) = '\0';
			numTracks++;
		}

		// sort album by track names
		qsort( musicInfo.albums[musicInfo.numAlbums].tracks, numTracks, sizeof( trackInfo_t ), SortTracks );

		if ( numTracks > 0 ) {
			musicInfo.albums[musicInfo.numAlbums].numTracks = numTracks;
			musicInfo.numAlbums++;
		}
	}


	// no playlist in memory?
	if ( NULL == musicInfo.playOrder ) {
		fileHandle_t	f;
		char			playlist[2048];
		char			trackName[MAX_QPATH];
		char			*c;
		int				i, t, a;

		i = trap_FS_FOpenFile( PLAYLIST_FILENAME_S, &f, FS_READ );
		if ( !f ) {
			// if playlist could not be read from file, fallback to playing all tracks
			Com_Printf( MUSIC_MESSAGEPREFIX_S "Could not read playlist from disk, will play all available tracks.\n" );
			Playlist_AddAllTracks( qtrue );
		}
		else if ( i >= sizeof( playlist ) ) {
			Com_Printf( MUSIC_MESSAGEPREFIX_S "^1Playlist from disk too large for internal buffer (is %d, max %ld), will be reset!\n", i, ( sizeof( playlist ) - 1 ) );
			// TODO: Empty playlist from memory will lateron be written to disk. Do Playlist_AddAllTracks( qtrue ) instead?
		}
		else {
			memset( playlist, 0, sizeof( playlist ) );
			trap_FS_Read( playlist, ( sizeof( playlist ) - 1 ), f );
			trap_FS_FCloseFile( f );

			i = 0;
			for ( c = playlist; *c != '\0'; ++c ) {
				if ( '\n' != *c ) {
					trackName[i++] = *c;
				}
				else {
					trackName[i] = '\0';
					for ( a = 0 ; a < musicInfo.numAlbums; ++a ) {
						for ( t = 0; t < musicInfo.albums[a].numTracks; ++t ) {
							if ( Q_stricmp( musicInfo.albums[a].tracks[t].file, trackName ) == 0 ) {
								Playlist_AddTrack( a, t, qtrue );
								break;
							}
						}
						i = 0;
					}
				}
			}
		}
	}

}


void MusicMenu_Shutdown( void ) {
	playOrder_t *playOrder;
	int			i;
	int			size;
	fileHandle_t f;

	if ( !musicInitialized ) {
		return;
	}

	trap_FS_FOpenFile( PLAYLIST_FILENAME_S, &f, FS_WRITE );
	if ( !f ) {
		Com_Printf( MUSIC_MESSAGEPREFIX_S "^1Could not save playlist to disk!\n" );
		return;
	}

	for ( playOrder = musicInfo.playOrder, i = 0; playOrder != NULL; playOrder = playOrder->next, i += 3 ) {
		size = ( strlen( musicInfo.albums[playOrder->album].tracks[playOrder->track].file ) + 1 ); // +1 => '\n'
		trap_FS_Write( va( "%s\n", musicInfo.albums[playOrder->album].tracks[playOrder->track].file ), size, f );
	}
	
	trap_FS_FCloseFile( f );
}


static void Music_PrintTrackInfo( const trackInfo_t *ti ) {
	Com_Printf( MUSIC_MESSAGEPREFIX_S "Playing \"^3%s^7\"\n", ti->title );
}


static void Music_PlayNextTrack( const trackInfo_t *ti ) {
	Music_PrintTrackInfo( ti );

	trap_S_StartBackgroundTrack( ti->file, "<nextsongCMD>" );
}


void Music_NextTrack( void ) {
	playOrder_t	*playOrder;

	if ( NULL == musicInfo.playOrder ) {
		return;
	}

	// more than one track? rotate!
	if( musicInfo.playOrder != musicInfo.lastplayOrderItem ) {
		playOrder = musicInfo.playOrder;
		musicInfo.playOrder = playOrder->next;
		playOrder->next = NULL;
		musicInfo.lastplayOrderItem->next = playOrder;
		musicInfo.lastplayOrderItem = playOrder;
	}

	Music_PlayNextTrack( &( musicInfo.albums[musicInfo.playOrder->album].tracks[musicInfo.playOrder->track] ) );
}


void Music_TriggerRestart( void ) {
	musicInfo.songStarted = qfalse;
}


void Music_Check( void ) {
	if ( qtrue == uis.musicbool ) {
		return;
	}

	if ( NULL == musicInfo.playOrder ) {
		return;
	}


	if ( qfalse == musicInfo.songStarted ) {
		if ( wop_AutoswitchSongByNextMap.integer ) {
			Music_NextTrack();
		}
		else {
			Music_PlayNextTrack( &( musicInfo.albums[musicInfo.playOrder->album].tracks[musicInfo.playOrder->track] ) );
		}
		musicInfo.songStarted = qtrue;
	}
}


static void MusicMenu_Event( void *ptr, int notification ) {
	int id;

	if( notification != QM_ACTIVATED ) {
		return;
	}

	id = ( (menucommon_s*)ptr )->id;

	switch ( id ) {
		case ID_EXIT:
			if ( musicMenu.switchState != MUSICSWITCH_QUIT ) {
				musicMenu.switchTime = uis.realtime;
			}
			musicMenu.switchState = MUSICSWITCH_QUIT;
			break;
		case ID_SWITCH:
			if ( ( musicInfo.numAlbums < 2 )  || ( musicMenu.switchState != MUSICSWITCH_NONE ) ) {
				return;
			}
			musicMenu.switchState = MUSICSWITCH_OUT;
			musicMenu.switchTime = uis.realtime;
			break;
		case ID_CLEARLIST:
			Playlist_Clear();
			break;
		case ID_PLAYALL:
			Playlist_AddAllTracks( qfalse );
			break;

		default:
			if ( ( id >= ID_TRACK1 ) && ( id < ID_EXIT ) ) {
			Playlist_AddTrack( musicMenu.currentAlbum, ( id - ID_TRACK1 ), qfalse );
			break;
		}
	}
}

#define FACTOR1024 ( 1024.0f / 640.0f )
static void MusicMenu_Draw( void ) {
	int		t;
	float	switchOffset = 0.0;

	if ( musicMenu.switchState != MUSICSWITCH_NONE ) {
		// if MUSICSWITCH_OUT or MUSICSWITCH_QUIT
		if ( musicMenu.switchState < 0 ) {
			switchOffset = ( uis.realtime - musicMenu.switchTime );
		}
		// else MUSICSWITCH_IN
		else if ( musicMenu.switchState > 0 ) {
			// FIXME: Magical constant 1000
			switchOffset = ( 1000 -( uis.realtime - musicMenu.switchTime ) );
		}

		switchOffset *= switchOffset;
		switchOffset *= 0.001;

		for ( t = 0 ; t < MAX_TRACKS; ++t) {
			musicMenu.tracks[t].generic.x	= ( 188 + switchOffset );
		}
		musicMenu.switchAlbum.x			= ( 925 + switchOffset * FACTOR1024 );
		musicMenu.clearList.x			= ( 958 + switchOffset * FACTOR1024 );
		musicMenu.playAll.x				= ( 925 + switchOffset * FACTOR1024 );
		musicMenu.exit.x				= ( 954 + switchOffset * FACTOR1024 );

		if ( ( uis.realtime - musicMenu.switchTime ) > 1000 ) {
			if ( MUSICSWITCH_OUT == musicMenu.switchState ) {
				trap_S_StartLocalSound( menu_switch_sound, CHAN_LOCAL_SOUND );
				musicMenu.switchState = MUSICSWITCH_IN;
				musicMenu.switchTime = uis.realtime;

				if ( ( musicMenu.currentAlbum + 1 ) >= musicInfo.numAlbums ) {
					musicMenu.currentAlbum = 0;
				}
				else {
					musicMenu.currentAlbum++;
				}

				for ( t = 0 ; t < MAX_TRACKS; ++t ) {
					if ( t >= musicInfo.albums[musicMenu.currentAlbum].numTracks ) {
						musicMenu.tracks[t].generic.flags	|= ( QMF_HIDDEN | QMF_INACTIVE );
					}
					else {
						musicMenu.tracks[t].generic.flags	&= ~( QMF_HIDDEN | QMF_INACTIVE );
					}
					musicMenu.tracks[t].generic.right	= ( musicMenu.tracks[t].generic.left + strlen( musicInfo.albums[musicMenu.currentAlbum].tracks[t].title ) * SMALLCHAR_WIDTH );
					musicMenu.tracks[t].string			= musicInfo.albums[musicMenu.currentAlbum].tracks[t].title;
				}
			}
			else if ( MUSICSWITCH_IN == musicMenu.switchState ) {
				musicMenu.switchState = MUSICSWITCH_NONE;
			}
			else if ( MUSICSWITCH_QUIT == musicMenu.switchState ) {
				UI_PopMenu();
			}
		}
	}
	else {
		for ( t = 0; t < MAX_TRACKS; ++t ) {
			musicMenu.tracks[t].generic.x	= 188;
		}
		musicMenu.switchAlbum.x		= 925;
		musicMenu.clearList.x		= 958;
		musicMenu.playAll.x			= 925;
		musicMenu.exit.x			= 954;
	}

	if ( musicInfo.albums[musicMenu.currentAlbum].background ) {
		UI_DrawHandlePic( ( 130 + switchOffset ), 61, 366, 338, musicInfo.albums[musicMenu.currentAlbum].background );
	}
	else {
		UI_FillRect( ( 160 + switchOffset ), 60, 335, 340, colorBlack );
		UI_DrawStringNS( ( 170 + switchOffset ), 62, musicInfo.albums[musicMenu.currentAlbum].name, 0, 16.0, colorWhite );
	}

	UI_DrawHandlePic( ( 433 + switchOffset ), 68, 500, 315, musicMenu.background );

	if ( musicInfo.playOrder != NULL ) {
		playOrder_t		*playOrder;
		int				i;

		for ( playOrder = musicInfo.playOrder, i = 1 ; playOrder != NULL; playOrder = playOrder->next, i++ ) {
			if ( playOrder->album == musicMenu.currentAlbum ) {
				// FIXME: Magical constant 18
				UI_DrawString( ( 188 + switchOffset ), ( 82 + playOrder->track * 18 ), va( "%2i.", i ), ( UI_RIGHT | UI_SMALLFONT ), colorWhite );
			}
		}
	}

	// standard menu drawing
	Menu_Draw( &musicMenu.menu );
}


static sfxHandle_t MusicMenu_Key( int key ) {
	switch ( key ) {
		case K_ESCAPE:
		case K_MOUSE2:
		case 'm':
		case 'M':
			if ( musicMenu.switchState != MUSICSWITCH_QUIT ) {
				musicMenu.switchTime = uis.realtime;
			}
			musicMenu.switchState = MUSICSWITCH_QUIT;
			return 0;
			break;
		case 's':
		case 'S':
			if ( ( musicInfo.numAlbums > 1 ) && ( MUSICSWITCH_NONE == musicMenu.switchState ) ) {
				musicMenu.switchState = MUSICSWITCH_OUT;
				musicMenu.switchTime = uis.realtime;
			}
			return 0;
			break;
	}

	return ( Menu_DefaultKey( &musicMenu.menu, key ) );
}


void MusicMenu_Cache( void ) {
	musicMenu.background = trap_R_RegisterShaderNoMip( MUSIC_BG_ARM );
}


void MusicMenu_Open( void ) {
	int		y, t;

	memset( &musicMenu, 0 , sizeof( musicMenu ) );
	uis.musicbool = 2; //weil beim cmd aufruf cl_paused nicht gesetzt wird
					   //2->music kommt vom music-menu ... 1->music ist ui-bgmusic

	trap_Cvar_Set( "cl_paused", "1" ); //wenn musicbool auf 2 ist sollt das nicht's ausmachen

	MusicMenu_Cache();

	musicMenu.menu.wrapAround	= qtrue;
	musicMenu.menu.fullscreen	= qfalse;
	musicMenu.menu.draw			= MusicMenu_Draw;
	musicMenu.menu.key			= MusicMenu_Key;

	y = 82;
	for ( t = 0; t < MAX_TRACKS; ++t ) {
		musicMenu.tracks[t].generic.type	= MTYPE_TEXTS;
		musicMenu.tracks[t].fontHeight		= 16.0;
		if ( t >= musicInfo.albums[musicMenu.currentAlbum].numTracks ) {
			musicMenu.tracks[t].generic.flags = ( QMF_HIDDEN | QMF_INACTIVE );
		}
		else {
			musicMenu.tracks[t].generic.flags = 0;
		}
		musicMenu.tracks[t].generic.x			= 188;
		musicMenu.tracks[t].generic.y			= y;
		musicMenu.tracks[t].generic.id			= ( ID_TRACK1 + t );
		musicMenu.tracks[t].generic.callback	= MusicMenu_Event;
		musicMenu.tracks[t].string				= musicInfo.albums[musicMenu.currentAlbum].tracks[t].title;
		musicMenu.tracks[t].color				= color_white;
		musicMenu.tracks[t].style				= ( UI_LEFT | UI_SMALLFONT );

		Menu_AddItem( &musicMenu.menu, &musicMenu.tracks[t] );

		// FIXME: Magical constant 18, also used somewhere above
		y += 18;
	}

	musicMenu.switchAlbum.x		= 925;
	musicMenu.clearList.x		= 958;
	musicMenu.playAll.x			= 925;
	musicMenu.exit.x			= 954;

	musicMenu.switchAlbum.generic.type		= MTYPE_BITMAP1024S;
	if ( musicInfo.numAlbums < 2 ) {
		musicMenu.switchAlbum.generic.flags = ( QMF_HIDDEN | QMF_INACTIVE );
	}
	musicMenu.switchAlbum.y					= 348;
	musicMenu.switchAlbum.generic.id		= ID_SWITCH;
	musicMenu.switchAlbum.generic.callback	= MusicMenu_Event;
	musicMenu.switchAlbum.w					= 76;
	musicMenu.switchAlbum.h					= 19;
	musicMenu.switchAlbum.shader			= trap_R_RegisterShaderNoMip( "wopmusic/switch_button_off" );
	musicMenu.switchAlbum.mouseovershader	= trap_R_RegisterShaderNoMip( "wopmusic/switch_button_on" );
	Menu_AddItem( &musicMenu.menu, &musicMenu.switchAlbum );

	musicMenu.clearList.generic.type		= MTYPE_BITMAP1024S;
	musicMenu.clearList.y					= 369;
	musicMenu.clearList.generic.id			= ID_CLEARLIST;
	musicMenu.clearList.generic.callback	= MusicMenu_Event;
	musicMenu.clearList.w					= 43;
	musicMenu.clearList.h					= 19;
	musicMenu.clearList.shader				= trap_R_RegisterShaderNoMip( "wopmusic/clear_button_off");
	musicMenu.clearList.mouseovershader		= trap_R_RegisterShaderNoMip( "wopmusic/clear_button_on");
	Menu_AddItem( &musicMenu.menu, &musicMenu.clearList );

	musicMenu.playAll.generic.type			= MTYPE_BITMAP1024S;
	musicMenu.playAll.y						= 369;
	musicMenu.playAll.generic.id			= ID_PLAYALL;
	musicMenu.playAll.generic.callback		= MusicMenu_Event;
	musicMenu.playAll.w						= 31;
	musicMenu.playAll.h						= 19;
	musicMenu.playAll.shader				= trap_R_RegisterShaderNoMip( "wopmusic/all_button_off" );
	musicMenu.playAll.mouseovershader		= trap_R_RegisterShaderNoMip( "wopmusic/all_button_on" );
	Menu_AddItem( &musicMenu.menu, &musicMenu.playAll );

	musicMenu.exit.generic.type				= MTYPE_BITMAP1024S;
	musicMenu.exit.y						= 413;
	musicMenu.exit.generic.id				= ID_EXIT;
	musicMenu.exit.generic.callback			= MusicMenu_Event;
	musicMenu.exit.w						=
	musicMenu.exit.h						= 55;
	musicMenu.exit.shader					= trap_R_RegisterShaderNoMip( "wopmusic/exit_button_off" );
	musicMenu.exit.mouseovershader			= trap_R_RegisterShaderNoMip( "wopmusic/exit_button_on" );
	Menu_AddItem( &musicMenu.menu, &musicMenu.exit );

	UI_PushMenu( &musicMenu.menu );

	musicMenu.switchState = MUSICSWITCH_IN;
	musicMenu.switchTime = ( uis.realtime + 1000 );
}

