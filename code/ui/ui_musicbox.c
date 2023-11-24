/*
	World of PADMAN - Music player and menu
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

#define MUSICBOX "menu/bg/musicbox"
#define ALL0 "menu/buttons/mb_all0"
#define ALL1 "menu/buttons/mb_all1"
#define CLEAR0 "menu/buttons/mb_clear0"
#define CLEAR1 "menu/buttons/mb_clear1"
#define EXIT0 "menu/buttons/mb_exit0"
#define EXIT1 "menu/buttons/mb_exit1"
#define SWITCH0 "menu/buttons/mb_switch0"
#define SWITCH1 "menu/buttons/mb_switch1"

#define MAX_TRACKS 15
#define MAX_ALBUMS 10
#define MAX_TRACKNAME 64
#define MAX_ALBUMNAME 32

#define ID_TRACK1 10
#define ID_EXIT (ID_TRACK1 + MAX_TRACKS)
#define ID_SWITCH (ID_EXIT + 1)
#define ID_CLEARLIST (ID_SWITCH + 1)
#define ID_PLAYALL (ID_CLEARLIST + 1)

#define SWITCHDELAY 500
#define XPOSITION (SCREEN_WIDTH / 2)

typedef enum {
	MUSICSWITCH_OUT = -1,
	MUSICSWITCH_NONE = 0,
	MUSICSWITCH_IN = 1,
	MUSICSWITCH_QUIT = -2
} musicMenuSwitch_t;

typedef struct {
	menuframework_s menu;

	menutext_s tracks[MAX_TRACKS];
	menubitmap_s switchAlbum;
	menubitmap_s clearList;
	menubitmap_s playAll;
	menubitmap_s exit;

	qhandle_t background;
	musicMenuSwitch_t switchState;
	int switchTime;

	int currentAlbum;
} musicMenu_t;

static musicMenu_t musicMenu;

typedef struct {
	char title[MAX_TRACKNAME];
	char file[MAX_QPATH];
} trackInfo_t;

typedef struct {
	trackInfo_t tracks[MAX_TRACKS];
	qhandle_t background;
	char name[MAX_ALBUMNAME];
	int numTracks;
} albumInfo_t;

typedef struct playOrder_s {
	struct playOrder_s *next;
	int album;
	int track;
} playOrder_t;

static playOrder_t playOrderMemory[(MAX_TRACKS * MAX_ALBUMS)];
static playOrder_t *freeOrderItems;

typedef struct {
	albumInfo_t albums[MAX_ALBUMS];

	playOrder_t *playOrder;
	playOrder_t *lastplayOrderItem;
	int numAlbums;
	int songStarted;
} musicInfo_t;

static musicInfo_t musicInfo;

#define PLAYLIST_FILENAME_S "playlist.dat"

#define MUSIC_MESSAGEPREFIX_S "[WoP Music]: "

static void Playlist_AddTrack(int album, int track, qboolean init) {
	playOrder_t *playOrder, *playOrder2;

	// are there already songs in the playlist?
	if (NULL != musicInfo.playOrder) {
		// are we currently playing the song to be added?
		if ((track == musicInfo.playOrder->track) && (album == musicInfo.playOrder->album)) {
			// stop it
			musicInfo.songStarted = qfalse;
			trap_S_StopBackgroundTrack();

			// if it was the last track,
			if (musicInfo.playOrder == musicInfo.lastplayOrderItem) {
				musicInfo.lastplayOrderItem = NULL;
			}
			playOrder = musicInfo.playOrder;
			musicInfo.playOrder = playOrder->next;
			playOrder->next = freeOrderItems;
			freeOrderItems = playOrder;

			return;
		}
		// iterate through playlist and find track in album
		for (playOrder = (playOrder2 = musicInfo.playOrder)->next; playOrder != NULL; playOrder = playOrder->next) {
			if ((album == playOrder->album) && (track == playOrder->track)) {
				if (playOrder == musicInfo.lastplayOrderItem) {
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

	// no more free items, *should* never happen since playOrderMemory is large enough
	if (NULL == freeOrderItems) {
		Com_Printf(S_COLOR_RED MUSIC_MESSAGEPREFIX_S "selected too many tracks!\n");
		return;
	}

	playOrder = freeOrderItems;
	freeOrderItems = freeOrderItems->next;

	if (NULL == musicInfo.playOrder) {
		musicInfo.playOrder = musicInfo.lastplayOrderItem = playOrder;
	} else {
		musicInfo.lastplayOrderItem->next = playOrder;
		musicInfo.lastplayOrderItem = playOrder;
	}

	playOrder->next = NULL;
	playOrder->album = album;
	playOrder->track = track;

	// if we're not initializing, check whether to play that song
	if (!init) {
		UI_MusicBox_Check();
	}
}

static void Playlist_Clear(void) {
	playOrder_t *playOrder;

	if (NULL == freeOrderItems) {
		freeOrderItems = musicInfo.playOrder;
	} else {
		for (playOrder = freeOrderItems; playOrder->next != NULL; playOrder = playOrder->next) {
			// do nothing, just loop
		}

		playOrder->next = musicInfo.playOrder;
	}

	musicInfo.playOrder = musicInfo.lastplayOrderItem = NULL;

	musicInfo.songStarted = qfalse;
	trap_S_StopBackgroundTrack();
}

static void Playlist_AddAllTracks(qboolean init) {
	int a, t;

	Playlist_Clear();

	for (a = 0; a < musicInfo.numAlbums; ++a) {
		for (t = 0; t < musicInfo.albums[a].numTracks; ++t) {
			Playlist_AddTrack(a, t, init);
		}
	}
}

static int QDECL SortTracks(const void *a, const void *b) {
	const trackInfo_t *ta, *tb;

	ta = (const trackInfo_t *)a;
	tb = (const trackInfo_t *)b;

	return Q_stricmp(ta->file, tb->file);
}

// snd_restart causes shutdown without initialization
static qboolean musicInitialized = qfalse;

void UI_MusicBox_Init(void) {
	char albumDirList[1024];
	char trackDirList[1024];
	char *albumStr, *trackStr;
	int albumDirLen, numAlbumDirs, a;
	int trackDirLen, numTrackDirs, t;
	char albumName[32];
	int numTracks;

	musicInitialized = qtrue;
	memset(&musicInfo, 0, sizeof(musicInfo));

	memset(&playOrderMemory, 0, sizeof(playOrderMemory));
	freeOrderItems = playOrderMemory;
	for (a = 0; a < ((MAX_TRACKS * MAX_ALBUMS) - 1); ++a) {
		playOrderMemory[a].next = &playOrderMemory[(a + 1)];
	}

	numAlbumDirs = trap_FS_GetFileList("wopmusic", "/", albumDirList, sizeof(albumDirList));
	albumStr = albumDirList;
	if (numAlbumDirs > MAX_ALBUMS) {
		numAlbumDirs = MAX_ALBUMS;
	}
	for (a = 0; a < numAlbumDirs; ++a, albumStr += (albumDirLen + 1)) {
		if ('\0' == *albumStr) {
			albumDirLen = 0;
			continue;
		}

		albumDirLen = strlen(albumStr);

		if (!Q_stricmp(albumStr, ".") || !Q_stricmp(albumStr, "..")) {
			continue;
		}

		// '/' is missing if the directory isn't in a pk3
		if ('/' == albumStr[(albumDirLen - 1)]) {
			Q_strncpyz(albumName, albumStr, sizeof(albumName));
		} else {
			Com_sprintf(albumName, sizeof(albumName), "%s/", albumStr);
		}
		Q_strncpyz(musicInfo.albums[musicInfo.numAlbums].name, albumStr,
				   sizeof(musicInfo.albums[musicInfo.numAlbums].name));

		musicInfo.albums[musicInfo.numAlbums].background = trap_R_RegisterShaderNoMip(va("wopmusic/%sbg", albumName));

		// Load tracks for each supported format, currently .ogg only

		// it'd be best if songs were in a subdirectory (without any non-track-files like bg)
		// so we could omit the .foo extension and just load all files from that dir

		numTracks = 0;
		numTrackDirs = trap_FS_GetFileList(va("wopmusic/%s", albumName), ".ogg", trackDirList, sizeof(trackDirList));
		trackStr = trackDirList;
		// numTracks is the number of .wav that were found previously
		if (numTrackDirs > (MAX_TRACKS - numTracks)) {
			numTrackDirs = (MAX_TRACKS - numTracks);
		}

		for (t = 0; t < numTrackDirs; ++t, trackStr += (trackDirLen + 1)) {
			if ('\0' == *trackStr) {
				trackDirLen = 0;
				continue;
			}

			trackDirLen = strlen(trackStr);

			Com_sprintf(musicInfo.albums[musicInfo.numAlbums].tracks[numTracks].file,
						sizeof(musicInfo.albums[musicInfo.numAlbums].tracks[numTracks].file), "wopmusic/%s%s",
						albumName, trackStr);

			// "09_pad-anthem (credits).ogg"
			Q_strncpyz(musicInfo.albums[musicInfo.numAlbums].tracks[numTracks].title, (char *)(trackStr + 3),
					   sizeof(musicInfo.albums[musicInfo.numAlbums].tracks[numTracks].title));
			*strstr(musicInfo.albums[musicInfo.numAlbums].tracks[numTracks].title, ".ogg") = '\0';
			numTracks++;
		}

		// sort album by track names
		qsort(musicInfo.albums[musicInfo.numAlbums].tracks, numTracks, sizeof(trackInfo_t), SortTracks);

		if (numTracks > 0) {
			musicInfo.albums[musicInfo.numAlbums].numTracks = numTracks;
			musicInfo.numAlbums++;
		}
	}

	// no playlist in memory?
	if (NULL == musicInfo.playOrder) {
		fileHandle_t f;
		char playlist[4096];
		const char *trackName;
		const char *playlistPtr;
		int i;

		i = trap_FS_FOpenFile(PLAYLIST_FILENAME_S, &f, FS_READ);
		if (!f) {
			// if playlist could not be read from file, fallback to playing all tracks
			Com_Printf(MUSIC_MESSAGEPREFIX_S "Could not read playlist from disk, will play all available tracks.\n");
			Playlist_AddAllTracks(qtrue);
			return;
		}

		if (i >= sizeof(playlist)) {
			Com_Printf(S_COLOR_YELLOW MUSIC_MESSAGEPREFIX_S
					   "Playlist from disk too large for internal buffer (is %d, max %ld), playlist will get cut!\n",
					   i, (sizeof(playlist) - 1));
		}

		memset(playlist, 0, sizeof(playlist));
		trap_FS_Read(playlist, (sizeof(playlist) - 1), f);
		trap_FS_FCloseFile(f);

		playlistPtr = playlist;
		for (;;) {
			trackName = Com_ParseLine(&playlistPtr);
			if (*playlistPtr == '\0' && *trackName == '\0') {
				break;
			}
			if (*trackName == '\0') {
				continue;
			}
			for (a = 0; a < musicInfo.numAlbums; ++a) {
				for (t = 0; t < musicInfo.albums[a].numTracks; ++t) {
					if (Q_stricmp(musicInfo.albums[a].tracks[t].file, trackName) == 0) {
						Playlist_AddTrack(a, t, qtrue);
						break;
					}
				}
			}
		}
	}
}

void UI_MusicBox_Shutdown(void) {
	playOrder_t *playOrder;
	int i;
	fileHandle_t f;

	if (!musicInitialized) {
		return;
	}

	trap_FS_FOpenFile(PLAYLIST_FILENAME_S, &f, FS_WRITE);
	if (!f) {
		Com_Printf(S_COLOR_RED MUSIC_MESSAGEPREFIX_S "Could not save playlist to disk!\n");
		return;
	}

	for (playOrder = musicInfo.playOrder, i = 0; playOrder != NULL; playOrder = playOrder->next, i += 3) {
		const char *line = va("%s\n", musicInfo.albums[playOrder->album].tracks[playOrder->track].file);
		const int size = strlen(line);
		trap_FS_Write(line, size, f);
	}

	trap_FS_FCloseFile(f);
}

static void Music_PrintTrackInfo(const trackInfo_t *ti) {
	Com_Printf(MUSIC_MESSAGEPREFIX_S "Playing \"" S_COLOR_YELLOW "%s" S_COLOR_WHITE "\"\n", ti->title);
}

static void Music_PlayNextTrack(const trackInfo_t *ti) {
	Music_PrintTrackInfo(ti);

	trap_S_StartBackgroundTrack(ti->file, "<nextsongCMD>");
}

void UI_MusicBox_NextTrack(void) {
	playOrder_t *playOrder;

	if (NULL == musicInfo.playOrder) {
		return;
	}

	// more than one track? rotate!
	if (musicInfo.playOrder != musicInfo.lastplayOrderItem) {
		playOrder = musicInfo.playOrder;
		musicInfo.playOrder = playOrder->next;
		playOrder->next = NULL;
		musicInfo.lastplayOrderItem->next = playOrder;
		musicInfo.lastplayOrderItem = playOrder;
	}

	Music_PlayNextTrack(&(musicInfo.albums[musicInfo.playOrder->album].tracks[musicInfo.playOrder->track]));
}

void UI_MusicBox_TriggerRestart(void) {
	musicInfo.songStarted = qfalse;
}

void UI_MusicBox_Check(void) {
	if (uis.musicstate == MUSICSTATE_RUNNING) {
		return;
	}

	if (NULL == musicInfo.playOrder) {
		return;
	}

	if (!musicInfo.songStarted) {
		if (wop_AutoswitchSongByNextMap.integer) {
			UI_MusicBox_NextTrack();
		} else {
			Music_PlayNextTrack(&(musicInfo.albums[musicInfo.playOrder->album].tracks[musicInfo.playOrder->track]));
		}
		musicInfo.songStarted = qtrue;
	}
}

static void MusicMenu_Event(void *ptr, int notification) {
	int id;

	if (notification != QM_ACTIVATED) {
		return;
	}

	id = ((menucommon_s *)ptr)->id;

	switch (id) {
	case ID_EXIT:
		if (musicMenu.switchState != MUSICSWITCH_QUIT) {
			musicMenu.switchTime = uis.realtime;
		}
		musicMenu.switchState = MUSICSWITCH_QUIT;
		break;
	case ID_SWITCH:
		if ((musicInfo.numAlbums < 2) || (musicMenu.switchState != MUSICSWITCH_NONE)) {
			return;
		}
		musicMenu.switchState = MUSICSWITCH_OUT;
		musicMenu.switchTime = uis.realtime;
		break;
	case ID_CLEARLIST:
		Playlist_Clear();
		break;
	case ID_PLAYALL:
		Playlist_AddAllTracks(qfalse);
		break;

	default:
		if ((id >= ID_TRACK1) && (id < ID_EXIT)) {
			Playlist_AddTrack(musicMenu.currentAlbum, (id - ID_TRACK1), qfalse);
			break;
		}
	}
}

static void MusicMenu_Draw(void) {
	int t;
	float switchOffset = 0.0f;

	if (musicMenu.switchState != MUSICSWITCH_NONE) {
		// if MUSICSWITCH_OUT or MUSICSWITCH_QUIT
		if (musicMenu.switchState < 0) {
			switchOffset = (uis.realtime - musicMenu.switchTime);
		}
		// else MUSICSWITCH_IN
		else if (musicMenu.switchState > 0) {
			switchOffset = (SWITCHDELAY - (uis.realtime - musicMenu.switchTime));
		}

		switchOffset *= switchOffset;
		switchOffset *= 0.01f;

		for (t = 0; t < MAX_TRACKS; ++t) {
			musicMenu.tracks[t].generic.x = (XPOSITION - 132 + switchOffset);
		}
		musicMenu.switchAlbum.generic.x = (XPOSITION + 253 + switchOffset);
		musicMenu.clearList.generic.x = (XPOSITION + 275 + switchOffset);
		musicMenu.playAll.generic.x = (XPOSITION + 253 + switchOffset);
		musicMenu.exit.generic.x = (XPOSITION + 270 + switchOffset);

		if ((uis.realtime - musicMenu.switchTime) > SWITCHDELAY) {
			if (MUSICSWITCH_OUT == musicMenu.switchState) {
				trap_S_StartLocalSound(menuSwitchSound, CHAN_LOCAL_SOUND);
				musicMenu.switchState = MUSICSWITCH_IN;
				musicMenu.switchTime = uis.realtime;

				if ((musicMenu.currentAlbum + 1) >= musicInfo.numAlbums) {
					musicMenu.currentAlbum = 0;
				} else {
					musicMenu.currentAlbum++;
				}

				for (t = 0; t < MAX_TRACKS; ++t) {
					if (t >= musicInfo.albums[musicMenu.currentAlbum].numTracks) {
						musicMenu.tracks[t].generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
					} else {
						musicMenu.tracks[t].generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
					}
					musicMenu.tracks[t].generic.right =
						(musicMenu.tracks[t].generic.left +
						 strlen(musicInfo.albums[musicMenu.currentAlbum].tracks[t].title) * SMALLCHAR_WIDTH);
					musicMenu.tracks[t].string = musicInfo.albums[musicMenu.currentAlbum].tracks[t].title;
				}
			} else if (MUSICSWITCH_IN == musicMenu.switchState) {
				musicMenu.switchState = MUSICSWITCH_NONE;
			} else if (MUSICSWITCH_QUIT == musicMenu.switchState) {
				UI_PopMenu();
			}
		}
	} else {
		for (t = 0; t < MAX_TRACKS; ++t) {
			musicMenu.tracks[t].generic.x = XPOSITION - 132;
		}
		musicMenu.switchAlbum.generic.x = XPOSITION + 253;
		musicMenu.clearList.generic.x = XPOSITION + 275;
		musicMenu.playAll.generic.x = XPOSITION + 253;
		musicMenu.exit.generic.x = XPOSITION + 270;
	}

	if (musicInfo.albums[musicMenu.currentAlbum].background) {
		UI_DrawHandlePic((XPOSITION - 202 + switchOffset), 60, 374, 340, musicInfo.albums[musicMenu.currentAlbum].background);
	} else {
		UI_FillRect((XPOSITION - 170 + switchOffset), 60, 340, 340, colorBlack);
		UI_DrawStringNS((XPOSITION - 150 + switchOffset), 62, musicInfo.albums[musicMenu.currentAlbum].name, 0, 16.0f, colorWhite);
	}

	UI_DrawNamedPic((XPOSITION + 108 + switchOffset), 68, 500, 315, MUSICBOX);

	if (musicInfo.playOrder != NULL) {
		playOrder_t *playOrder;
		int i;

		for (playOrder = musicInfo.playOrder, i = 1; playOrder != NULL; playOrder = playOrder->next, i++) {
			if (playOrder->album == musicMenu.currentAlbum) {
				// FIXME: Magical constant 18
				UI_DrawString((XPOSITION - 132 + switchOffset), (82 + playOrder->track * 18), va("%2i.", i),
							  (UI_RIGHT | UI_SMALLFONT), colorWhite);
			}
		}
	}

	// standard menu drawing
	Menu_Draw(&musicMenu.menu);
}

static sfxHandle_t MusicMenu_Key(int key) {
	switch (key) {
	case K_ESCAPE:
	case K_MOUSE2:
	case 'm':
	case 'M':
		if (musicMenu.switchState != MUSICSWITCH_QUIT) {
			musicMenu.switchTime = uis.realtime;
		}
		musicMenu.switchState = MUSICSWITCH_QUIT;
		return 0;
	case 's':
	case 'S':
		if ((musicInfo.numAlbums > 1) && (MUSICSWITCH_NONE == musicMenu.switchState)) {
			musicMenu.switchState = MUSICSWITCH_OUT;
			musicMenu.switchTime = uis.realtime;
		}
		return 0;
	}

	return (Menu_DefaultKey(&musicMenu.menu, key));
}

void UI_MusicBox_Open(void) {
	int y, t;

	memset(&musicMenu, 0, sizeof(musicMenu));
	// because cl_paused is not set when calling via cmd
	uis.musicstate = MUSICSTATE_RUNNING_MUSIC_MENU;

	// this should not matter if MUSICSTATE_RUNNING_MUSIC_MENU is set
	trap_Cvar_Set("cl_paused", "1");

	UI_MusicBox_Cache();

	if (musicInfo.playOrder) {
		musicMenu.currentAlbum = musicInfo.playOrder->album;
	}
	musicMenu.menu.wrapAround = qtrue;
	musicMenu.menu.fullscreen = qfalse;
	musicMenu.menu.draw = MusicMenu_Draw;
	musicMenu.menu.key = MusicMenu_Key;

	y = 82;
	for (t = 0; t < MAX_TRACKS; ++t) {
		musicMenu.tracks[t].generic.type = MTYPE_TEXTS;
		musicMenu.tracks[t].fontHeight = 16.0f;
		if (t >= musicInfo.albums[musicMenu.currentAlbum].numTracks) {
			musicMenu.tracks[t].generic.flags = (QMF_HIDDEN | QMF_INACTIVE);
		} else {
			musicMenu.tracks[t].generic.flags = 0;
		}
		musicMenu.tracks[t].generic.x = XPOSITION - 132;
		musicMenu.tracks[t].generic.y = y;
		musicMenu.tracks[t].generic.id = (ID_TRACK1 + t);
		musicMenu.tracks[t].generic.callback = MusicMenu_Event;
		musicMenu.tracks[t].string = musicInfo.albums[musicMenu.currentAlbum].tracks[t].title;
		musicMenu.tracks[t].color = color_white;
		musicMenu.tracks[t].style = (UI_LEFT | UI_SMALLFONT);

		Menu_AddItem(&musicMenu.menu, &musicMenu.tracks[t]);

		// FIXME: Magical constant 18, also used somewhere above
		y += 18;
	}

	musicMenu.switchAlbum.generic.type = MTYPE_BITMAP;
	if (musicInfo.numAlbums < 2) {
		musicMenu.switchAlbum.generic.flags = (QMF_HIDDEN | QMF_INACTIVE);
	}

	musicMenu.switchAlbum.generic.type = MTYPE_BITMAP;
	musicMenu.switchAlbum.generic.name = SWITCH0;
	musicMenu.switchAlbum.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	musicMenu.switchAlbum.generic.x = XPOSITION + 253;
	musicMenu.switchAlbum.generic.y = 216;
	musicMenu.switchAlbum.generic.id = ID_SWITCH;
	musicMenu.switchAlbum.generic.callback = MusicMenu_Event;
	musicMenu.switchAlbum.width = 48;
	musicMenu.switchAlbum.height = 13;
	musicMenu.switchAlbum.focuspic = SWITCH1;
	musicMenu.switchAlbum.focuspicinstead = qtrue;

	musicMenu.clearList.generic.type = MTYPE_BITMAP;
	musicMenu.clearList.generic.name = CLEAR0;
	musicMenu.clearList.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	musicMenu.clearList.generic.x = XPOSITION + 275;
	musicMenu.clearList.generic.y = 231;
	musicMenu.clearList.generic.id = ID_CLEARLIST;
	musicMenu.clearList.generic.callback = MusicMenu_Event;
	musicMenu.clearList.width = 26;
	musicMenu.clearList.height = 13;
	musicMenu.clearList.focuspic = CLEAR1;
	musicMenu.clearList.focuspicinstead = qtrue;

	musicMenu.playAll.generic.type = MTYPE_BITMAP;
	musicMenu.playAll.generic.name = ALL0;
	musicMenu.playAll.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	musicMenu.playAll.generic.x = XPOSITION + 253;
	musicMenu.playAll.generic.y = 231;
	musicMenu.playAll.generic.id = ID_PLAYALL;
	musicMenu.playAll.generic.callback = MusicMenu_Event;
	musicMenu.playAll.width = 20;
	musicMenu.playAll.height = 13;
	musicMenu.playAll.focuspic = ALL1;
	musicMenu.playAll.focuspicinstead = qtrue;

	musicMenu.exit.generic.type = MTYPE_BITMAP;
	musicMenu.exit.generic.name = EXIT0;
	musicMenu.exit.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	musicMenu.exit.generic.x = XPOSITION + 270;
	musicMenu.exit.generic.y = 257;
	musicMenu.exit.generic.id = ID_EXIT;
	musicMenu.exit.generic.callback = MusicMenu_Event;
	musicMenu.exit.width = 36;
	musicMenu.exit.height = 36;
	musicMenu.exit.focuspic = EXIT1;
	musicMenu.exit.focuspicinstead = qtrue;

	Menu_AddItem(&musicMenu.menu, &musicMenu.switchAlbum);
	Menu_AddItem(&musicMenu.menu, &musicMenu.playAll);
	Menu_AddItem(&musicMenu.menu, &musicMenu.clearList);
	Menu_AddItem(&musicMenu.menu, &musicMenu.exit);

	UI_PushMenu(&musicMenu.menu);

	musicMenu.switchState = MUSICSWITCH_IN;
	musicMenu.switchTime = (uis.realtime + SWITCHDELAY);
}

/*
===============
UI_MusicBox_Cache
===============
*/
void UI_MusicBox_Cache(void) {
	trap_R_RegisterShaderNoMip(MUSICBOX);
	trap_R_RegisterShaderNoMip(ALL0);
	trap_R_RegisterShaderNoMip(ALL1);
	trap_R_RegisterShaderNoMip(CLEAR0);
	trap_R_RegisterShaderNoMip(CLEAR1);
	trap_R_RegisterShaderNoMip(EXIT0);
	trap_R_RegisterShaderNoMip(EXIT1);
	trap_R_RegisterShaderNoMip(SWITCH0);
	trap_R_RegisterShaderNoMip(SWITCH1);
}
