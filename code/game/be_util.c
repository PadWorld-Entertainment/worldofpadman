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

#include "g_local.h"

#define GAMETYPE_MAP(gt)                                                                                               \
	{ GAMETYPE_NAME(gt), GAMETYPE_NAME_SHORT(gt), gt }

const mapGametypeString_t GAMETYPE_REMAP[GT_MAX_GAME_TYPE] = {
	GAMETYPE_MAP(GT_FFA),	   GAMETYPE_MAP(GT_TOURNAMENT), GAMETYPE_MAP(GT_SINGLE_PLAYER),
	GAMETYPE_MAP(GT_SPRAYFFA), GAMETYPE_MAP(GT_LPS),		GAMETYPE_MAP(GT_TEAM),
	GAMETYPE_MAP(GT_CTF),	   GAMETYPE_MAP(GT_SPRAY),		GAMETYPE_MAP(GT_BALLOON)};

#undef GAMETYPE_MAP

/*
	Send a command to one or all clients. Basically a wrapper around trap_SendServerCommand()
	See CG_ServerCommand() in cg_servercmds.c
*/
void SendClientCommand(clientNum_t clientNum, clientCommand_t cmd, const char *str) {
	char buffer[MAX_STRING_CHARS];

	G_assert(str != NULL);

	if (!ValidClientID(clientNum, qtrue)) {
		G_Error("SendClientCommand: clientNum %i out of range\n", clientNum);
		return;
	}
	/* TODO: Check whether clientNum is connected */

	/* NOTE: Here's a special problem; You can not send newlines via rcon, not even
			 via server console, only via game code. Thus we need to replace those
			 and others ourself.
	*/
	/* FIXME: Only do conversion for print and centerprint */
	/* NOTE: This most likely won't work for stell, ssay(_team) and smp anyways,
			 since they suffer from the same bug; not even aware of newlines
	*/

	EscapeChars(str, buffer, sizeof(buffer));

	switch (cmd) {
	case CCMD_CENTERPRINT:
		trap_SendServerCommand(clientNum, va("cp \"%s\"", buffer));
		break;
	case CCMD_MESSAGEPRINT:
		trap_SendServerCommand(clientNum, va("mp \"%s\"", buffer));
		break;
	case CCMD_PRINT:
		/* NOTE: World of Padman is flawed with con_notifytime < 0
				 It'll always remove the last character in CG_Printf in cg_main.c
		*/
		trap_SendServerCommand(clientNum, va("print \"%s\"", buffer));
		break;

	default:
		G_Error("SendClientCommand: cmd %i out of clientCommand_t range!\n", cmd);
		return;
	}
}

/*
	Converts a string into a gametype.
	Might return invalid gametype if no match
*/
/* FIXME: Use partial match?
 */
gametype_t StringToGametype(const char *str) {
	int i;

	G_assert(str != NULL);

	for (i = 0; i < ARRAY_LEN(GAMETYPE_REMAP); i++) {
		if (Q_stricmp(GAMETYPE_REMAP[i].name, str) == 0) {
			return (GAMETYPE_REMAP[i].type);
		}
	}

	return GT_MAX_GAME_TYPE;
}

/*
	Converts a gametype into a string.
	Might return NULL if no match
*/
const char *GametypeToString(gametype_t gt) {
	int i;

	for (i = 0; i < ARRAY_LEN(GAMETYPE_REMAP); i++) {
		if (GAMETYPE_REMAP[i].type == gt) {
			return (GAMETYPE_REMAP[i].name);
		}
	}

	return NULL;
}

/*
	Converts a time into a short string.
	Assumes time is given in ms
*/
char *TimeToString(int time, char *str, size_t size) {
	int min, tens, sec;

	G_assert(str != NULL);
	G_assert(size > 0);

	sec = (time / 1000);

	min = (sec / 60);
	sec -= (min * 60);
	tens = (sec / 10);
	sec -= (tens * 10);

	Com_sprintf(str, size, "%i:%i%i", min, tens, sec);

	return str;
}

/*
	Returns whether given clientNum integer is in valid
	range from 0 to MAX_CLIENTS-1.
	Does not do any other sanity checks!
*/
qboolean ValidClientID(int clientNum, qboolean allowWorld) {
	if ((clientNum < 0) || (clientNum >= MAX_CLIENTS)) {
		if (allowWorld && (CID_WORLD == clientNum)) {
			return qtrue;
		}

		return qfalse;
	}

	return qtrue;
}

/*
	Returns a clientNum_t for a given string/name.
	If there are multiple matches, it won't return the first
	match but CID_MULTIPLE (sort of an error).
*/
/* TODO: Rewrite to better handle multiple matches OR
		 add ClientnumsFromString() (note the plural!).
   NOTE: There's always a tradeoff between accuracy and comfort
*/
clientNum_t ClientnumFromString(const char *name) {
	gclient_t *player;
	char cleanName[MAX_NETNAME], cleanQuery[MAX_NETNAME];
	int id = CID_NONE, matches = 0, i;

	G_assert(name != NULL);

	/* FIXME: Const correctness */
	ClientCleanName((char *)name, cleanQuery, sizeof(cleanQuery));

	/* FIXME: Return as soon as matches > 1? */

	for (i = 0; i < level.maxclients; i++) {
		player = &level.clients[i];

		if (CON_DISCONNECTED == player->pers.connected) {
			continue;
		}

		/* exact case-insensitive match */
		if (Q_stricmp(name, player->pers.netname) == 0) {
			id = i;
			matches++;
			continue;
		}

		/* match without colorcodes etc. */
		/* TODO: Maybe check for 'l' vs. '1' etc., as the font makes them look rather similar */
		/* FIXME: Does this violate the multiple matches approach? */
		ClientCleanName(player->pers.netname, cleanName, sizeof(cleanName));
		if (Q_stricmp(cleanQuery, cleanName) == 0) {
			id = i;
			matches++;
			continue;
		}

		/* TODO: Partial match with strstr (dangerous?) */
	}

	if (matches > 1) {
		return CID_MULTIPLE;
	} else {
		return id;
	}
}

/*
	Helper function to check whether a file exists serverside
*/
qboolean fileExists(const char *path) {
	G_assert(path != NULL);

	return (trap_FS_FOpenFile(path, NULL, FS_READ));
}

/*
	Helper function to fix models like "padman/doesntexist"
	which basically use a non existing skin.
*/

#define DEFAULT_PLAYERSKIN_S "default"
#define PLAYERMODEL_PATH_S "models/wop_players/"

qboolean validPlayermodel(const char *model, const char *headModel) {
	char base[MAX_QPATH];
	char *skin;

	G_assert(model != NULL);
	G_assert(headModel != NULL);

	Q_strncpyz(base, model, sizeof(base));
	skin = strchr(base, '/');
	if (skin) {
		*skin++ = '\0';
	} else {
		skin = DEFAULT_PLAYERSKIN_S;
	}

	if (!fileExists(va(PLAYERMODEL_PATH_S "%s/upper_%s.skin", base, skin))) {
		return qfalse;
	}

	Q_strncpyz(base, headModel, sizeof(base));
	skin = strchr(base, '/');
	if (skin) {
		*skin++ = '\0';
	} else {
		skin = DEFAULT_PLAYERSKIN_S;
	}

	if (!fileExists(va(PLAYERMODEL_PATH_S "%s/head_%s.skin", base, skin))) {
		return qfalse;
	}

	/* TODO: We could also check whether glowskins exist for this (custom-)model */

	return qtrue;
}

#undef DEFAULT_PLAYERSKIN_S
#undef PLAYERMODEL_PATH_S

/*
	Returns a team_t for the given string.
	SetTeam() has some implementation of this..
*/
team_t TeamFromString(const char *s) {
	team_t team;

	G_assert(s != NULL);

	/* TODO: Restore old verbose, case insensitive "spectator", "red", "blue", "free" logic? */

	switch (s[0]) {
	case 'r':
		team = TEAM_RED;
		break;
	case 'b':
		team = TEAM_BLUE;
		break;
	case 's':
		team = TEAM_SPECTATOR;
		break;
	case 'f':
		team = TEAM_FREE;
		break;

	default:
		team = TEAM_NUM_TEAMS;
		break;
	}

	return team;
}

/*
	If ent is given, will use CCMD_PRINT, otherwise G_Printf.
	This is usefull for functions that can be used by both clients
	and rcon.

	TODO: Use fmt and .. once va functions are buffer safe?
*/
void PrintMessage(const gentity_t *ent, const char *msg) {
	G_assert(msg != NULL);

	if (NULL == ent) {
		/* TODO: Strip colors with Q_CleanStr (violate const / buffer)? */
		G_Printf("%s", msg);
	} else {
		SendClientCommand((ent - g_entities), CCMD_PRINT, msg);
	}
}

/*
	Returns whether given string is in "/string/string2/" list
	Used in be_allowedVotes cvar
*/
qboolean InList(const char *haystack, const char *needle) {
	char pattern[MAX_STRING_TOKENS];

	G_assert(haystack != NULL);
	G_assert(needle != NULL);

	Com_sprintf(pattern, sizeof(pattern), "/%s/", needle);

	return (Q_stristr(haystack, pattern) != NULL);
}

/*
	Used to spit out some debug information, even in release builds.
*/
/* NOTE: This is an almost exact copy of G_Printf */
void QDECL G_DPrintf(const char *fmt, ...) {
	va_list argptr;
	char text[1024];

	G_assert(fmt != NULL);

	/* FIXME: Rather add a new g_debug/g_developer cvar and loglevels for it? */
	if (!trap_Cvar_VariableIntegerValue("developer")) {
		return;
	}

	va_start(argptr, fmt);
	Q_vsnprintf(text, sizeof(text), fmt, argptr);
	va_end(argptr);

	trap_Printf(text);
}

/*
	Prints text to server console, which will be optionally decolored.
*/
/* NOTE: This is an almost exact copy of G_Printf */
void QDECL BE_Printf(const char *fmt, ...) {
	va_list argptr;
	char buf[1024];

	G_assert(fmt != NULL);

	va_start(argptr, fmt);
	Q_vsnprintf(buf, sizeof(buf), fmt, argptr);
	va_end(argptr);

	if (!G_ColoredOutput()) {
		Q_DecolorStr(buf, buf, sizeof(buf));
	}

	trap_Printf(buf);
}

/*
	Removes all colortags "recursively".
*/
void Q_DecolorStr(char *in, char *out, size_t outsize) {
	int outpos = 0;

	G_assert(in != NULL);
	G_assert(out != NULL);
	G_assert(outsize > 0);

	for (; (*in && (outpos < (outsize - 1))); in++) {
		out[outpos] = *in;

		if ((outpos > 0) && (Q_COLOR_ESCAPE == out[outpos - 1])) {
			if (Q_IsColorString(&out[outpos - 1])) {
				outpos--;
				continue;
			}
		}

		outpos++;
	}

	out[outpos] = '\0';
}

/*
	Strips all whitespace from string, not only trailing, leading and double.
*/
void Q_StripWhitespace(char *in, char *out, size_t outsize) {
	int outpos = 0;

	G_assert(in != NULL);
	G_assert(out != NULL);
	G_assert(outsize > 0);

	for (; (*in && (outpos < (outsize - 1))); in++) {
		out[outpos] = *in;

		if (' ' == *in) {
			continue;
		}

		outpos++;
	}

	out[outpos] = '\0';
}

/*
	Strips colortags, whitespace and any strange characters and converts to lowercase.
*/
void Q_ExtraCleanStr(char *in, char *out, size_t outsize) {
	int outpos = 0;

	G_assert(in != NULL);
	G_assert(out != NULL);
	G_assert(outsize > 0);

	Q_DecolorStr(in, out, outsize);
	in = out;
	Q_StripWhitespace(in, out, outsize);

	for (; (*in && (outpos < (outsize - 1))); in++) {
		out[outpos] = *in;

		if ((*in < ' ') || (*in > '}') || (*in == '`')) {
			continue;
		}

		outpos++;
	}

	out[outpos] = '\0';

	Q_strlwr(out);
}

/*
	Inserts a client command as if the client sent it.
	trap_EA_Command() -> EA_Command() -> BotClientCommand() -> SV_ExecuteClientCommand()
*/
void ExecuteClientCommand(clientNum_t clientNum, const char *cmd) {
	G_assert(cmd != NULL);

	if (!ValidClientID(clientNum, qfalse)) {
		G_Error("ExecuteClientCommand: clientNum %i out of range\n", clientNum);
		return;
	}
	/* TODO: Check whether clientNum is connected */

	/* FIXME: Const correctness */
	trap_EA_Command(clientNum, (char *)cmd);
}

/*
	Returns whether the given string is a valid GUID for cl_guid
*/
qboolean validGUID(const char *guid) {
	int count = 0;
	qboolean valid = qtrue;

	G_assert(guid != NULL);

	/* "Its value is a 32 character string made up of [a-f] and [0-9] characters." */
	while (guid[count] != '\0' && valid) {
		if (((guid[count] < '0') || (guid[count] > '9')) && ((guid[count] < 'A') || (guid[count] > 'F'))) {
			valid = qfalse;
			break;
		}
		count++;
	}

	if (count != (GUIDSTRMAXLEN - 1)) {
		valid = qfalse;
	}

	return valid;
}

/*
	Returns a colored team name without " team" suffix
*/
char *Teamname(team_t team) {
	char *teamname;
	static char coloredTeamname[32];

	switch (team) {
	case TEAM_RED:
		teamname = "red";
		break;

	case TEAM_BLUE:
		teamname = "blue";
		break;

	case TEAM_FREE:
		teamname = "free";
		break;

	case TEAM_SPECTATOR:
		teamname = "spectators";
		break;

	default:
		teamname = "unknown";
		break;
	}

	Com_sprintf(coloredTeamname, sizeof(coloredTeamname), "%s%s", TeamColorStr(team), teamname);

	return coloredTeamname;
}

/*
 * Returns a short colored team name without " team" suffix.
 */
char *TeamnameShort(team_t team) {
	char *teamname;
	static char coloredTeamname[4];

	switch (team) {
	case TEAM_RED:
		teamname = "r";
		break;

	case TEAM_BLUE:
		teamname = "b";
		break;

	case TEAM_FREE:
		teamname = "f";
		break;

	case TEAM_SPECTATOR:
		teamname = "s";
		break;

	default:
		teamname = "?";
		break;
	}

	Com_sprintf(coloredTeamname, sizeof(coloredTeamname), "%s%s", TeamColorStr(team), teamname);

	return coloredTeamname;
}

/*
	Returns a color string for the team
*/
char *TeamColorStr(team_t team) {
	char *teamColor;

	switch (team) {
	case TEAM_RED:
		teamColor = S_COLOR_RED;
		break;

	case TEAM_BLUE:
		teamColor = S_COLOR_BLUE;
		break;

	case TEAM_FREE:
		teamColor = S_COLOR_GREEN;
		break;

	case TEAM_SPECTATOR:
		teamColor = S_COLOR_WHITE;
		break;

	default:
		teamColor = S_COLOR_YELLOW;
		break;
	}

	return teamColor;
}

/*
	Returns a colored name for use in G_Say()
*/
void FormatChatName(char *buff, size_t size, char *namesrc, int mode, char *location, team_t team) {
	char *teamColor;

	G_assert(buff != NULL);
	G_assert(size > 0);
	G_assert(namesrc != NULL);

	teamColor = TeamColorStr(team);

	/* NOTE: This does nothing special. It uses the same format as the vanilla chat, but colors the
			 final ": " depending on the team.
	*/
	switch (mode) {
	default: /* fall through */
	case SAY_ALL:
		Com_sprintf(buff, size, "%s" S_COLOR_DEFAULT "" EC "%s: ", namesrc, teamColor);
		break;

	case SAY_TEAM:
		if (location) {
			Com_sprintf(buff, size, EC "(%s" S_COLOR_DEFAULT EC ") (%s)" EC "%s: ", namesrc, location, teamColor);
		} else {
			Com_sprintf(buff, size, EC "(%s" S_COLOR_DEFAULT EC ")" EC "%s: ", namesrc, teamColor);
		}
		break;

	case SAY_TELL:
		if (location) {
			Com_sprintf(buff, size, EC "[%s" S_COLOR_DEFAULT EC "] (%s)" EC "%s: ", namesrc, location, teamColor);
		} else {
			Com_sprintf(buff, size, EC "[%s" S_COLOR_DEFAULT EC "]" EC "%s: ", namesrc, teamColor);
		}
		break;
	}
}

/*
	Plays the given sound globally on the server to all players.
*/
void PlayGlobalSound(int soundIndex) {
	gentity_t *te;

	G_assert(soundIndex != 0);

	te = G_TempEntity(level.intermission_origin, EV_GLOBAL_SOUND);
	te->s.eventParm = soundIndex;
	te->r.svFlags |= SVF_BROADCAST;
}

/*
	Prints the given size in bytes in a more human readable form using suffixes
*/
#define KIBIBYTE (1 << 10)
#define MEBIBYTE (KIBIBYTE << 10)
#define GIBIBYTE (MEBIBYTE << 10)
void ReadableSize(char *buf, size_t bufsize, int value) {
	G_assert(buf);

	if (value > (GIBIBYTE)) {
		Com_sprintf(buf, bufsize, "%d", value / GIBIBYTE);
		Com_sprintf((buf + strlen(buf)), (bufsize - strlen(buf)), ".%02d GiB", (value % GIBIBYTE) * 100 / GIBIBYTE);
	} else if (value > (MEBIBYTE)) {
		Com_sprintf(buf, bufsize, "%d", value / MEBIBYTE);
		Com_sprintf((buf + strlen(buf)), (bufsize - strlen(buf)), ".%02d MiB", (value % MEBIBYTE) * 100 / MEBIBYTE);
	} else if (value > KIBIBYTE) {
		Com_sprintf(buf, bufsize, "%d KiB", (value / KIBIBYTE));
	} else {
		Com_sprintf(buf, bufsize, "%d byte%s", value, (1 == value ? "" : "s"));
	}
}

/*
	Returns whether console supports color tags
*/
qboolean G_ColoredOutput(void) {
	/* tty has color tag conversion enabled */
	if (trap_Cvar_VariableIntegerValue("com_ansiColor")) {
		return qtrue;
	}

	/* If running a listen server, ingame console will be used */
	if (trap_Cvar_VariableIntegerValue("dedicated") == 0) {
		return qtrue;
	}

	/* TODO: rcon output, if there is a way to check capabilities of remote end */

	return qfalse;
}

/*
	Copies str into buffer and replaces "\\n" with a real "\n", "\\" with "\" and
	"\'" with "\"".
*/
void EscapeChars(const char *str, char *buffer, size_t size) {
	int i, j;

	Q_strncpyz(buffer, str, size);
	for (i = j = 0; i < strlen(buffer); i++, j++) {
		if ('\0' == buffer[i]) {
			break;
		}

		if ('\\' == buffer[i]) {
			if ('\\' == buffer[i + 1]) {
				buffer[j] = '\\';
				i++;
			} else if ('n' == buffer[i + 1]) {
				buffer[j] = '\n';
				i++;
			} else if ('\'' == buffer[i + 1]) {
				buffer[j] = '"';
				i++;
			} else {
				buffer[j] = buffer[i];
			}
		} else {
			buffer[j] = buffer[i];
		}
	}
	buffer[j] = '\0';
}

/*
	Returns qtrue if client is a spectator or dead waiting player in LPS
*/
qboolean IsSpectator(gclient_t *client) {
	G_assert(client != NULL);

	return ((TEAM_SPECTATOR == client->sess.sessionTeam) || LPSDeadSpec(client));
}

/*
 * Drops a client with an optional reason
 */
void G_DropClient(gentity_t *ent, const char *reason) {
	char buff[128];

	G_assert(ent != NULL);

	if ((NULL == reason) || (strlen(reason) < 1)) {
		Q_strncpyz(buff, "was kicked.", sizeof(buff));
	} else {
		Com_sprintf(buff, sizeof(buff), "was kicked: %s.", reason);
	}

	trap_DropClient((ent - g_entities), buff);
}

/*
 * Tries to obtain the client id of a connected client.
 */
clientNum_t GetConnectedClientNum(int argv, char *buf, size_t bufsize) {
	char clientStr[3];
	int clientNum;

	trap_Argv(argv, clientStr, sizeof(clientStr));

	if (!Q_isanumber(clientStr)) {
		Q_strncpyz(buf, S_COLOR_NEGATIVE "You must supply a client number, not a name.", bufsize);
		return CID_NONE;
	}

	clientNum = atoi(clientStr);
	if (!ValidClientID(clientNum, qfalse)) {
		Q_strncpyz(buf, S_COLOR_NEGATIVE "Not a valid client number.", bufsize);
		return CID_NONE;
	}

	if (CON_DISCONNECTED == level.clients[clientNum].pers.connected) {
		Q_strncpyz(buf, S_COLOR_NEGATIVE "Client not connected.", bufsize);
		return CID_NONE;
	}

	return clientNum;
}
