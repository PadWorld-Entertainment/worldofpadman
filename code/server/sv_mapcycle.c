#include "server.h"

#include <errno.h>

#define MAX_MAPCYCLE_FILE_SIZE 65536
#define MAX_MAPCYCLE_MAPS 256
#define MAX_MAPCYCLE_SETTINGS 2048
#define MAPCYCLE_NEXT "mapcycle_next"

typedef struct {
	char name[MAX_QPATH];
	char *value;
	char *original;
} mapCycleSetting_t;

typedef struct {
	char name[MAX_QPATH];
	int firstSetting;
	int numSettings;
} mapCycleMap_t;

typedef struct {
	char *originalNextmap;
	int current;
	int numMaps;
	int numSettings;
	mapCycleMap_t maps[MAX_MAPCYCLE_MAPS];
	mapCycleSetting_t settings[MAX_MAPCYCLE_SETTINGS];
} mapCycle_t;

static mapCycle_t *mapCycle;

static qboolean SV_MapCycleMapExists(const char *name) {
	char path[MAX_QPATH];

	Com_sprintf(path, sizeof(path), "maps/%s.bsp", name);
	return FS_ReadFile(path, NULL) >= 0;
}

static qboolean SV_ParseMapCycle(mapCycle_t *cycle, const char *filename, const char *data, const char *end) {
	const char *token;

	COM_BeginParseSession(filename);
	while (*(token = COM_Parse(&data))) {
		mapCycleMap_t *map;

		if (Q_stricmp(token, "map") || cycle->numMaps == MAX_MAPCYCLE_MAPS) {
			COM_ParseError("Expected a map block (maximum %d maps)", MAX_MAPCYCLE_MAPS);
			return qfalse;
		}

		token = COM_Parse(&data);
		if (!*token || strlen(token) > MAX_QPATH - sizeof("maps/.bsp") ||
			strspn(token, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-") != strlen(token)) {
			COM_ParseError("Invalid map name '%s'", token);
			return qfalse;
		}
		if (!SV_MapCycleMapExists(token)) {
			COM_ParseError("Can't find map '%s'", token);
			return qfalse;
		}

		map = &cycle->maps[cycle->numMaps++];
		Q_strncpyz(map->name, token, sizeof(map->name));
		map->firstSetting = cycle->numSettings;
		if (strcmp(COM_Parse(&data), "{")) {
			COM_ParseError("Expected '{' after map '%s'", map->name);
			return qfalse;
		}

		while (strcmp((token = COM_Parse(&data)), "}")) {
			mapCycleSetting_t *setting;
			qboolean quoted;
			int flags;

			if (!*token || strlen(token) >= MAX_QPATH ||
				strspn(token, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_.-") != strlen(token) ||
				cycle->numSettings == MAX_MAPCYCLE_SETTINGS) {
				COM_ParseError("Expected a cvar name or '}' in map '%s' (maximum %d settings)", map->name, MAX_MAPCYCLE_SETTINGS);
				return qfalse;
			}
			if (!Q_stricmp(token, "fraglimit") || !Q_stricmp(token, "capturelimit")) {
				token = "pointlimit";
			}
			flags = Cvar_Flags(token);
			if ((flags & (CVAR_ROM | CVAR_INIT)) || !Q_stricmp(token, "nextmap") ||
				!Q_stricmp(token, "nextmapBackUp")) {
				COM_ParseError("Can't set cvar '%s' in a map cycle", token);
				return qfalse;
			}

			setting = &cycle->settings[cycle->numSettings];
			Q_strncpyz(setting->name, token, sizeof(setting->name));
			token = COM_ParseExt(&data, qfalse);
			quoted = data && data[-1] == '"';
			if (!data || data > end || (!quoted && (!*token || !strcmp(token, "{") || !strcmp(token, "}"))) ||
				strlen(token) >= MAX_CVAR_VALUE_STRING || strpbrk(token, "\r\n")) {
				COM_ParseError("Expected a value for '%s' on the same line", setting->name);
				return qfalse;
			}
			if (!Q_stricmp(setting->name, "g_gametype") || !Q_stricmp(setting->name, "timelimit") ||
				!Q_stricmp(setting->name, "pointlimit")) {
				char *numberEnd;
				long number, maximum;
				qboolean gametype = !Q_stricmp(setting->name, "g_gametype");

				maximum = gametype ? GT_MAX_GAME_TYPE - 1 : INT_MAX;
				if (!Q_stricmp(setting->name, "timelimit")) {
					maximum = INT_MAX / 60000;
				}
				errno = 0;
				number = strtol(token, &numberEnd, 10);
				if (errno || numberEnd == token || *numberEnd || number < 0 || number > maximum ||
					(gametype && number == GT_SINGLE_PLAYER)) {
					COM_ParseError("Invalid value '%s' for '%s'", token, setting->name);
					return qfalse;
				}
			}
			setting->value = CopyString(token);
			cycle->numSettings++;
			map->numSettings++;
		}
	}
	if (data || !cycle->numMaps) {
		COM_ParseError("Expected a map block");
		return qfalse;
	}
	return qtrue;
}

static void SV_RestoreMapCycleSettings(void) {
	const mapCycleMap_t *map;
	int i;

	if (mapCycle->current < 0) {
		return;
	}
	map = &mapCycle->maps[mapCycle->current];
	for (i = map->firstSetting + map->numSettings - 1; i >= map->firstSetting; i--) {
		mapCycleSetting_t *setting = &mapCycle->settings[i];

		Cvar_SetLatched(setting->name, setting->original);
		if (setting->original) {
			Z_Free(setting->original);
			setting->original = NULL;
		}
	}
}

static void SV_FreeMapCycle(mapCycle_t *cycle) {
	int i;

	for (i = 0; i < cycle->numSettings; i++) {
		Z_Free(cycle->settings[i].value);
	}
	if (cycle->originalNextmap) {
		Z_Free(cycle->originalNextmap);
	}
	Z_Free(cycle);
}

void SV_StopMapCycle(void) {
	if (!mapCycle) {
		return;
	}
	SV_RestoreMapCycleSettings();
	if (!strcmp(Cvar_VariableString("nextmap"), MAPCYCLE_NEXT)) {
		const char *nextmap = mapCycle->originalNextmap;

		Cvar_Set("nextmap", *nextmap ? nextmap : "map_restart 0");
	}
	if (!strcmp(Cvar_VariableString("nextmapBackUp"), MAPCYCLE_NEXT)) {
		Cvar_Set("nextmapBackUp", "");
	}
	SV_FreeMapCycle(mapCycle);
	mapCycle = NULL;
}

static void SV_MapCycleNext_f(void) {
	mapCycleMap_t *map;
	int next, i;

	if (!mapCycle) {
		return;
	}
	next = (mapCycle->current + 1) % mapCycle->numMaps;
	map = &mapCycle->maps[next];
	if (!SV_MapCycleMapExists(map->name)) {
		Com_Printf("Can't find map '%s'\n", map->name);
		return;
	}

	SV_RestoreMapCycleSettings();
	mapCycle->current = next;
	for (i = map->firstSetting; i < map->firstSetting + map->numSettings; i++) {
		mapCycleSetting_t *setting = &mapCycle->settings[i];
		const char *original = Cvar_PendingString(setting->name);

		if (original) {
			setting->original = CopyString(original);
		} else {
			// Leave a reset value for cvars the game has yet to register.
			Cvar_SetLatched(setting->name, "");
		}
		Cvar_SetLatched(setting->name, setting->value);
	}
	Cmd_ExecuteString(va("map %s", map->name));
	if (mapCycle) {
		Cvar_Set("nextmap", MAPCYCLE_NEXT);
	}
}

static void SV_MapCycle_f(void) {
	const char *filename;
	char *data;
	fileHandle_t file;
	long length;
	mapCycle_t *cycle;
	qboolean parsed;

	if (Cmd_Argc() != 2) {
		Com_Printf("Usage: mapcycle <file> | mapcycle stop\n");
		return;
	}
	filename = Cmd_Argv(1);
	if (!Q_stricmp(filename, "stop")) {
		SV_StopMapCycle();
		return;
	}
	length = FS_FOpenFileRead(filename, &file, qtrue);
	if (!file) {
		Com_Printf("Can't open map cycle '%s'\n", filename);
		return;
	}
	if (length <= 0 || length > MAX_MAPCYCLE_FILE_SIZE) {
		FS_FCloseFile(file);
		Com_Printf("Invalid map cycle size for '%s' (maximum %d bytes)\n", filename, MAX_MAPCYCLE_FILE_SIZE);
		return;
	}

	// COM_ParseExt consumes the terminator of an unclosed quoted string.
	data = Z_Malloc(length + 2);
	parsed = FS_Read(data, length, file) == length && !memchr(data, '\0', length);
	FS_FCloseFile(file);
	if (!parsed) {
		Com_Printf("Can't read map cycle '%s'\n", filename);
		Z_Free(data);
		return;
	}

	cycle = Z_Malloc(sizeof(*cycle));
	parsed = SV_ParseMapCycle(cycle, filename, data, data + length);
	Z_Free(data);
	if (!parsed) {
		SV_FreeMapCycle(cycle);
		return;
	}
	SV_StopMapCycle();
	cycle->originalNextmap = CopyString(Cvar_VariableString("nextmap"));
	cycle->current = -1;
	mapCycle = cycle;
	SV_MapCycleNext_f();
}

void SV_AddMapCycleCommands(void) {
	Cmd_AddCommand("mapcycle", SV_MapCycle_f);
	Cmd_AddCommand(MAPCYCLE_NEXT, SV_MapCycleNext_f);
}
