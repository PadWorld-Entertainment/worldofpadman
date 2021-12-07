/*
===========================================================================
Copyright (C) 2010, 2011, 2012 the-brain

This program is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://gnu.org/licenses/>.
===========================================================================
*/

#ifndef _BE_UTIL_H
#define _BE_UTIL_H

/* assert macros for game logic, will only be used when compiled as debug version */
#ifdef NDEBUG
#define G_assert(_e) ((void)0)
#define G_massert(_e, _m) ((void)0)
#else
/* standard assert macro */
#define G_assert(_e) ((_e) ? ((void)0) : G_Error("assert \"%s\" failed: file %s, line %i\n", #_e, __FILE__, __LINE__))
/* assert variant which prints a custom message */
#define G_massert(_e, _m)                                                                                              \
	((_e) ? ((void)0) : G_Error("assert \"%s\" failed: file %s, line %i\n", _m, __FILE__, __LINE__))
#endif

typedef enum {
	CCMD_CENTERPRINT = 1,
	CCMD_MESSAGEPRINT = 2,
	CCMD_PRINT = 4,

	CCMD_CP = CCMD_CENTERPRINT,
	CCMD_MP = CCMD_MESSAGEPRINT,
	CCMD_PRT = CCMD_PRINT
} clientCommand_t;

/* NOTE: Anything from 0 to MAX_CLIENTS-1 is a valid client id
		 (but not neccessarily a valid/connected client).
		 Below are special (return) values.
		 Gamecode and engine use -1 to indicate "every client".
*/
enum {
	CID_NONE = MAX_CLIENTS,
	CID_WORLD = -1,
	CID_ALL = CID_WORLD,
	CID_MULTIPLE = -2 /* NOTE: See comment for ClientnumFromString() */
};

/* NOTE: With current "MAX_CLIENTS 64" this'd fit into char instead of int,
		 but engine/gamecode use int everywhere.
*/
typedef signed int clientNum_t;

typedef struct {
	const char *name;
	const char *shortName;
	gametype_t type;
} mapGametypeString_t;
extern const mapGametypeString_t GAMETYPE_REMAP[GT_MAX_GAME_TYPE];

void SendClientCommand(clientNum_t clientNum, clientCommand_t cmd, const char *str);

#define SendCmd(cId, cmdType, cmd) SendClientCommand(cId, cmdType, cmd)

gametype_t StringToGametype(const char *str);
const char *GametypeToString(gametype_t gt);

char *TimeToString(int time, char *str, size_t size);

qboolean ValidClientID(int clientNum, qboolean allowWorld);

clientNum_t ClientnumFromString(const char *name);

qboolean fileExists(const char *path);

qboolean validPlayermodel(const char *model, const char *headModel);

team_t TeamFromString(const char *s);

void PrintMessage(const gentity_t *ent, const char *msg);

qboolean InList(const char *needle, const char *haystack);

void QDECL G_DPrintf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

void QDECL BE_Printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

void Q_DecolorStr(char *in, char *out, size_t outsize);
void Q_StripWhitespace(char *in, char *out, size_t outsize);
void Q_ExtraCleanStr(char *in, char *out, size_t outsize);

void ExecuteClientCommand(clientNum_t clientNum, const char *cmd);

qboolean validGUID(const char *guid);

char *Teamname(team_t team);
char *TeamnameShort(team_t team);
char *TeamColorStr(team_t team);

void FormatChatName(char *buff, size_t size, char *namesrc, int mode, char *location, team_t team);

void PlayGlobalSound(int soundIndex);

void ReadableSize(char *buf, size_t bufsize, int value);

qboolean G_ColoredOutput(void);

void EscapeChars(const char *str, char *buffer, size_t size);

/* NOTE: Implementation is in g_main.c */
char *NameForCvar(const vmCvar_t *vmCvar);

qboolean IsSpectator(gclient_t *client);

void G_DropClient(gentity_t *ent, const char *reason);

clientNum_t GetConnectedClientNum(int argv, char *buf, size_t bufsize);

#endif
