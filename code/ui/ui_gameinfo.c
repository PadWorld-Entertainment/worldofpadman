/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
//
// gameinfo.c
//

#include "ui_local.h"

//
// arena and bot info
//

#define POOLSIZE 128 * 1024

static int ui_numBots;
static char *ui_botInfos[MAX_BOTS];

static int ui_numArenas;
static char *ui_arenaInfos[MAX_ARENAS];

static int ui_numSinglePlayerArenas;
static int ui_numSpecialSinglePlayerArenas;

static char memoryPool[POOLSIZE];
static int allocPoint, outOfMemory;

/*
===============
UI_Alloc
===============
*/
static void *UI_Alloc(int size) {
	char *p;

	if (allocPoint + size > POOLSIZE) {
		outOfMemory = qtrue;
		return NULL;
	}

	p = &memoryPool[allocPoint];

	allocPoint += (size + 31) & ~31;

	return p;
}

/*
===============
UI_InitMemory
===============
*/
static void UI_InitMemory(void) {
	allocPoint = 0;
	outOfMemory = qfalse;
}

/*
===============
UI_ParseInfos
===============
*/
static int UI_ParseInfos(const char *buf, int max, char *infos[]) {
	const char *token;
	int count;
	char key[MAX_TOKEN_CHARS];
	char info[MAX_INFO_STRING];

	count = 0;

	while (1) {
		token = COM_Parse(&buf);
		if (!token[0]) {
			break;
		}
		if (strcmp(token, "{")) {
			Com_Printf("Missing { in info file\n");
			break;
		}

		if (count == max) {
			Com_Printf("Max infos exceeded\n");
			break;
		}

		info[0] = '\0';
		while (1) {
			token = COM_ParseExt(&buf, qtrue);
			if (!token[0]) {
				Com_Printf("Unexpected end of info file\n");
				break;
			}
			if (!strcmp(token, "}")) {
				break;
			}
			Q_strncpyz(key, token, sizeof(key));

			token = COM_ParseExt(&buf, qfalse);
			if (!token[0]) {
				Info_SetValueForKey(info, key, "<NULL>");
			} else {
				Info_SetValueForKey(info, key, token);
			}
		}
		// NOTE: extra space for arena number
		infos[count] = UI_Alloc(strlen(info) + strlen(va("\\num\\%d", MAX_ARENAS)) + 1);
		if (infos[count]) {
			strcpy(infos[count], info);
			count++;
		}
	}
	return count;
}

/*
===============
UI_LoadArenasFromFile
===============
*/
static void UI_LoadArenasFromFile(char *filename) {
	int len;
	fileHandle_t f;
	char buf[MAX_ARENAS_TEXT];

	len = trap_FS_FOpenFile(filename, &f, FS_READ);
	if (!f) {
		trap_Print(va(S_COLOR_RED "file not found: %s\n", filename));
		return;
	}
	if (len >= MAX_ARENAS_TEXT) {
		trap_Print(va(S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_ARENAS_TEXT));
		trap_FS_FCloseFile(f);
		return;
	}

	trap_FS_Read(buf, len, f);
	buf[len] = 0;
	trap_FS_FCloseFile(f);

	ui_numArenas += UI_ParseInfos(buf, MAX_ARENAS - ui_numArenas, &ui_arenaInfos[ui_numArenas]);
}

/*
===============
UI_LoadArenas
===============
*/
static void UI_LoadArenas(void) {
	int numdirs;
	vmCvar_t arenasFile;
	char filename[128];
	char dirlist[4096];
	const char *dirptr;
	int i, n;
	int dirlen;
	const char *type;
	const char *tag;
	int singlePlayerNum, specialNum, otherNum;

	ui_numArenas = 0;

	trap_Cvar_Register(&arenasFile, "g_arenasFile", "", CVAR_INIT | CVAR_ROM);
	if (*arenasFile.string) {
		UI_LoadArenasFromFile(arenasFile.string);
	}

	// get all arenas from .arena files
	numdirs = trap_FS_GetFileList("scripts", ".arena", dirlist, sizeof(dirlist));
	dirptr = dirlist;
	for (i = 0; i < numdirs; i++, dirptr += dirlen + 1) {
		dirlen = strlen(dirptr);
		strcpy(filename, "scripts/");
		strcat(filename, dirptr);
		UI_LoadArenasFromFile(filename);
	}
	trap_Print(va("%i arenas parsed\n", ui_numArenas));
	if (outOfMemory)
		trap_Print(S_COLOR_YELLOW "WARNING: not enough memory in pool to load all arenas\n");

	// set initial numbers
	for (n = 0; n < ui_numArenas; n++) {
		Info_SetValueForKey(ui_arenaInfos[n], "num", va("%i", n));
	}

	// go through and count single players levels
	ui_numSinglePlayerArenas = 0;
	ui_numSpecialSinglePlayerArenas = 0;
	for (n = 0; n < ui_numArenas; n++) {
		// determine type
		type = Info_ValueForKey(ui_arenaInfos[n], "type");

		// if no type specified, it will be treated as "ffa"
		if (!*type) {
			continue;
		}

		if (strstr(type, "single")) {
			// check for special single player arenas (training, final)
			tag = Info_ValueForKey(ui_arenaInfos[n], "special");
			if (*tag) {
				ui_numSpecialSinglePlayerArenas++;
				continue;
			}

			ui_numSinglePlayerArenas++;
		}
	}

	n = ui_numSinglePlayerArenas % ARENAS_PER_TIER;
	if (n != 0) {
		ui_numSinglePlayerArenas -= n;
		trap_Print(va("%i arenas ignored to make count divisible by %i\n", n, ARENAS_PER_TIER));
	}

	// go through once more and assign number to the levels
	singlePlayerNum = 0;
	specialNum = singlePlayerNum + ui_numSinglePlayerArenas;
	otherNum = specialNum + ui_numSpecialSinglePlayerArenas;
	for (n = 0; n < ui_numArenas; n++) {
		// determine type
		type = Info_ValueForKey(ui_arenaInfos[n], "type");

		// if no type specified, it will be treated as "ffa"
		if (*type) {
			if (strstr(type, "single")) {
				// check for special single player arenas (training, final)
				tag = Info_ValueForKey(ui_arenaInfos[n], "special");
				if (*tag) {
					Info_SetValueForKey(ui_arenaInfos[n], "num", va("%i", specialNum++));
					continue;
				}

				Info_SetValueForKey(ui_arenaInfos[n], "num", va("%i", singlePlayerNum++));
				continue;
			}
		}

		Info_SetValueForKey(ui_arenaInfos[n], "num", va("%i", otherNum++));
	}
}

/*
===============
UI_GetArenaInfoByNumber
===============
*/
const char *UI_GetArenaInfoByNumber(int num) {
	int n;
	const char *value;

	if (num < 0 || num >= ui_numArenas) {
		trap_Print(va(S_COLOR_RED "Invalid arena number: %i\n", num));
		return NULL;
	}

	for (n = 0; n < ui_numArenas; n++) {
		value = Info_ValueForKey(ui_arenaInfos[n], "num");
		if (*value && atoi(value) == num) {
			return ui_arenaInfos[n];
		}
	}

	return NULL;
}

/*
===============
UI_LoadBotsFromFile
===============
*/
static void UI_LoadBotsFromFile(char *filename) {
	int len;
	fileHandle_t f;
	char buf[MAX_BOTS_TEXT];

	len = trap_FS_FOpenFile(filename, &f, FS_READ);
	if (!f) {
		trap_Print(va(S_COLOR_RED "file not found: %s\n", filename));
		return;
	}
	if (len >= MAX_BOTS_TEXT) {
		trap_Print(va(S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_BOTS_TEXT));
		trap_FS_FCloseFile(f);
		return;
	}

	trap_FS_Read(buf, len, f);
	buf[len] = 0;
	trap_FS_FCloseFile(f);

	ui_numBots += UI_ParseInfos(buf, MAX_BOTS - ui_numBots, &ui_botInfos[ui_numBots]);
	if (outOfMemory)
		trap_Print(S_COLOR_YELLOW "WARNING: not enough memory in pool to load all bots\n");
}

/*
===============
UI_LoadBots
===============
*/
static void UI_LoadBots(void) {
	vmCvar_t botsFile;
	int numdirs;
	char filename[128];
	char dirlist[1024];
	const char *dirptr;
	int i;
	int dirlen;

	ui_numBots = 0;

	trap_Cvar_Register(&botsFile, "g_botsFile", "", CVAR_INIT | CVAR_ROM);
	if (*botsFile.string) {
		UI_LoadBotsFromFile(botsFile.string);
	} else {
		//		UI_LoadBotsFromFile("scripts/bots.txt");
	}

	// get all bots from .bot files
	numdirs = trap_FS_GetFileList("scripts", ".bot", dirlist, 1024);
	dirptr = dirlist;
	for (i = 0; i < numdirs; i++, dirptr += dirlen + 1) {
		dirlen = strlen(dirptr);
		strcpy(filename, "scripts/");
		strcat(filename, dirptr);
		UI_LoadBotsFromFile(filename);
	}
	trap_Print(va("%i bots parsed\n", ui_numBots));
}

/*
===============
UI_GetBotInfoByNumber
===============
*/
char *UI_GetBotInfoByNumber(int num) {
	if (num < 0 || num >= ui_numBots) {
		trap_Print(va(S_COLOR_RED "Invalid bot number: %i\n", num));
		return NULL;
	}
	return ui_botInfos[num];
}

/*
===============
UI_GetNumArenas
===============
*/
int UI_GetNumArenas(void) {
	return ui_numArenas;
}

/*
===============
UI_GetNumBots
===============
*/
int UI_GetNumBots(void) {
	return ui_numBots;
}

static void UI_LoadLogoForMenu(const char *spraylogoName) {
	if (uis.spraylogosLoaded >= MAX_SPRAYLOGOS_LOADED) {
		return;
	}

	Q_strncpyz(uis.spraylogoNames[uis.spraylogosLoaded], spraylogoName,
			   sizeof(uis.spraylogoNames[uis.spraylogosLoaded]));
	uis.spraylogoShaders[uis.spraylogosLoaded] = trap_R_RegisterShaderNoMip(va(SPRAYLOGO_PATH "/%s", spraylogoName));
	uis.spraylogosLoaded++;
}

//
// Since cgame can't do trap_FS_GetFileList, we need to load the list of
// spraylogos here.
// While we do so, register them for the player setup menu aswell.
// We have to do this regardless of the actual gametype, since the cvar
// used for communication is not CVAR_ARCHIVE (which is intended) and the menu
// always shows the spraylogo selection.
//
static void UI_SearchSpraylogos(void) {
	int i;
	char fileList[1024];
	char *namePtr;
	int numFiles;
	int nameLen;
	char cvarBuff[1024];

	numFiles = trap_FS_GetFileList("spraylogos", NULL, fileList, sizeof(fileList));

	Com_Printf("Loading spraylogos:\n");

	cvarBuff[0] = '\0';
	namePtr = fileList;
	for (i = 0; i < numFiles; i++, namePtr += (nameLen + 1)) {
		char *ext = strchr(namePtr, '.');

		nameLen = strlen(namePtr);

		// ignore anything but jpg, png and tga
		if (!nameLen || !ext ||
			(Q_stricmp(".jpg", Q_strlwr(ext)) != 0 && Q_stricmp(".tga", Q_strlwr(ext)) != 0 &&
			 Q_stricmp(".png", Q_strlwr(ext)) != 0))
			continue;

		*ext = '\0';

		Com_Printf("%s ", namePtr);

		Q_strcat(cvarBuff, sizeof(cvarBuff), va("%s\\", namePtr));

		UI_LoadLogoForMenu(namePtr);
	}

	Com_Printf("\nLoaded %i spraylogos.\n", i);

	i = (strlen(cvarBuff) - 1);
	if (cvarBuff[i] == '\\') {
		cvarBuff[i] = '\0';
	}

	trap_Cvar_Set(SPRAYLOGO_LIST_CVAR, cvarBuff);
}

static void UI_SearchLensFlares(void) {
	char dirlist[1024];
	char tmpcvarstr[1024];
	const char *filestrptr;
	int dirlen, numdirs, i;
	char scriptname[32]; // reicht scho ;)

	Com_Printf("searching LensFlare-scripts:\n");
	numdirs = trap_FS_GetFileList("scripts", ".lensflare", dirlist, 1024);
	filestrptr = dirlist;
	tmpcvarstr[0] = '\0';
	for (i = 0; i < numdirs; i++, filestrptr += dirlen + 1) {
		dirlen = strlen(filestrptr);
		Q_strncpyz(scriptname, filestrptr, 32);
		*strstr(scriptname, ".lensflare") = '\0';
		Com_Printf("Script:%i Name:%s\n", i, scriptname);
		Q_strcat(tmpcvarstr, 1024, va("%s\\", scriptname));
	}
	Com_Printf("finished searching ... found %i scripts\n", numdirs);

	i = strlen(tmpcvarstr) - 1;
	if (tmpcvarstr[i] == '\\')
		tmpcvarstr[i] = '\0';

	trap_Cvar_Set("lensflarelist", tmpcvarstr);
}

/*
===============
UI_InitGameinfo
===============
*/
void UI_InitGameinfo(void) {
	UI_InitMemory();
	UI_LoadArenas();
	UI_LoadBots();

	UI_SearchSpraylogos();

	UI_SearchLensFlares();

	if ((trap_Cvar_VariableValue("fs_restrict")) ||
		(ui_numSpecialSinglePlayerArenas == 0 && ui_numSinglePlayerArenas == 4)) {
		uis.demoversion = qtrue;
	} else {
		uis.demoversion = qfalse;
	}
}
