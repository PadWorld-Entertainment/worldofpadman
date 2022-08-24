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

/*****************************************************************************
 * name:		be_ai_char.c
 *
 * desc:		bot characters
 *
 * $Archive: /MissionPack/code/botlib/be_ai_char.c $
 *
 *****************************************************************************/

#include "../qcommon/q_shared.h"
#include "l_log.h"
#include "l_memory.h"
#include "l_utils.h"
#include "l_script.h"
#include "l_precomp.h"
#include "l_struct.h"
#include "l_libvar.h"
#include "aasfile.h"
#include "botlib.h"
#include "be_aas.h"
#include "be_aas_funcs.h"
#include "be_interface.h"
#include "be_ai_char.h"

#define MAX_CHARACTERISTICS 80

#define CT_INTEGER 1
#define CT_FLOAT 2
#define CT_STRING 3

#define DEFAULT_CHARACTER "bots/default_c.c"

// characteristic value
union cvalue {
	int integer;
	float _float;
	char *string;
};
// a characteristic
typedef struct bot_characteristic_s {
	char type;			// characteristic type
	union cvalue value; // characteristic value
} bot_characteristic_t;

// a bot character
typedef struct bot_character_s {
	char filename[MAX_QPATH];
	float skill;
	bot_characteristic_t c[1]; // variable sized
} bot_character_t;

bot_character_t *botcharacters[MAX_CLIENTS + 1];

//========================================================================

static bot_character_t *BotCharacterFromHandle(int handle) {
	if (handle <= 0 || handle > MAX_CLIENTS) {
		botimport.Print(PRT_FATAL, "character handle %d out of range\n", handle);
		return NULL;
	}
	if (!botcharacters[handle]) {
		botimport.Print(PRT_FATAL, "invalid character %d\n", handle);
		return NULL;
	}
	return botcharacters[handle];
}

static void BotDumpCharacter(bot_character_t *ch) {
	int i;

	Log_Write("%s\n", ch->filename);
	Log_Write("skill %.1f\n", ch->skill);
	Log_Write("{\n");
	for (i = 0; i < MAX_CHARACTERISTICS; i++) {
		switch (ch->c[i].type) {
		case CT_INTEGER:
			Log_Write(" %4d %d\n", i, ch->c[i].value.integer);
			break;
		case CT_FLOAT:
			Log_Write(" %4d %f\n", i, ch->c[i].value._float);
			break;
		case CT_STRING:
			Log_Write(" %4d %s\n", i, ch->c[i].value.string);
			break;
		}
	}
	Log_Write("}\n");
}
//========================================================================

static void BotFreeCharacterStrings(bot_character_t *ch) {
	int i;

	for (i = 0; i < MAX_CHARACTERISTICS; i++) {
		if (ch->c[i].type == CT_STRING) {
			FreeMemory(ch->c[i].value.string);
		}
	}
}
//========================================================================

static void BotFreeCharacter2(int handle) {
	if (handle <= 0 || handle > MAX_CLIENTS) {
		botimport.Print(PRT_FATAL, "character handle %d out of range\n", handle);
		return;
	}
	if (!botcharacters[handle]) {
		botimport.Print(PRT_FATAL, "invalid character %d\n", handle);
		return;
	}
	BotFreeCharacterStrings(botcharacters[handle]);
	FreeMemory(botcharacters[handle]);
	botcharacters[handle] = NULL;
}
//========================================================================

void BotFreeCharacter(int handle) {
	if (!LibVarGetValue("bot_reloadcharacters"))
		return;
	BotFreeCharacter2(handle);
}

static void BotDefaultCharacteristics(bot_character_t *ch, bot_character_t *defaultch) {
	int i;

	for (i = 0; i < MAX_CHARACTERISTICS; i++) {
		if (ch->c[i].type)
			continue;

		if (defaultch->c[i].type == CT_FLOAT) {
			ch->c[i].type = CT_FLOAT;
			ch->c[i].value._float = defaultch->c[i].value._float;
		} else if (defaultch->c[i].type == CT_INTEGER) {
			ch->c[i].type = CT_INTEGER;
			ch->c[i].value.integer = defaultch->c[i].value.integer;
		} else if (defaultch->c[i].type == CT_STRING) {
			ch->c[i].type = CT_STRING;
			ch->c[i].value.string = (char *)GetMemory(strlen(defaultch->c[i].value.string) + 1);
			strcpy(ch->c[i].value.string, defaultch->c[i].value.string);
		}
	}
}

static bot_character_t *BotLoadCharacterFromFile(const char *charfile, int skill) {
	int indent, index, foundcharacter;
	bot_character_t *ch;
	source_t *source;
	token_t token;

	foundcharacter = qfalse;
	// a bot character is parsed in two phases
	PC_SetBaseFolder(BOTFILESBASEFOLDER);
	source = LoadSourceFile(charfile);
	if (!source) {
		botimport.Print(PRT_ERROR, "couldn't load %s\n", charfile);
		return NULL;
	}
	ch = (bot_character_t *)GetClearedMemory(sizeof(bot_character_t) +
											 MAX_CHARACTERISTICS * sizeof(bot_characteristic_t));
	strcpy(ch->filename, charfile);
	while (PC_ReadToken(source, &token)) {
		if (!strcmp(token.string, "skill")) {
			if (!PC_ExpectTokenType(source, TT_NUMBER, 0, &token)) {
				FreeSource(source);
				BotFreeCharacterStrings(ch);
				FreeMemory(ch);
				return NULL;
			}
			if (!PC_ExpectTokenString(source, "{")) {
				FreeSource(source);
				BotFreeCharacterStrings(ch);
				FreeMemory(ch);
				return NULL;
			}
			// if it's the correct skill
			if (skill < 0 || token.intvalue == skill) {
				foundcharacter = qtrue;
				ch->skill = token.intvalue;
				while (PC_ExpectAnyToken(source, &token)) {
					if (!strcmp(token.string, "}"))
						break;
					if (token.type != TT_NUMBER || !(token.subtype & TT_INTEGER)) {
						SourceError(source, "expected integer index, found %s", token.string);
						FreeSource(source);
						BotFreeCharacterStrings(ch);
						FreeMemory(ch);
						return NULL;
					}
					index = token.intvalue;
					if (index < 0 || index >= MAX_CHARACTERISTICS) {
						SourceError(source, "characteristic index out of range [0, %d]", MAX_CHARACTERISTICS);
						FreeSource(source);
						BotFreeCharacterStrings(ch);
						FreeMemory(ch);
						return NULL;
					}
					if (ch->c[index].type) {
						SourceError(source, "characteristic %d already initialized", index);
						FreeSource(source);
						BotFreeCharacterStrings(ch);
						FreeMemory(ch);
						return NULL;
					}
					if (!PC_ExpectAnyToken(source, &token)) {
						FreeSource(source);
						BotFreeCharacterStrings(ch);
						FreeMemory(ch);
						return NULL;
					}
					if (token.type == TT_NUMBER) {
						if (token.subtype & TT_FLOAT) {
							ch->c[index].value._float = token.floatvalue;
							ch->c[index].type = CT_FLOAT;
						} else {
							ch->c[index].value.integer = token.intvalue;
							ch->c[index].type = CT_INTEGER;
						}
					} else if (token.type == TT_STRING) {
						StripDoubleQuotes(token.string);
						ch->c[index].value.string = GetMemory(strlen(token.string) + 1);
						strcpy(ch->c[index].value.string, token.string);
						ch->c[index].type = CT_STRING;
					} else {
						SourceError(source, "expected integer, float or string, found %s", token.string);
						FreeSource(source);
						BotFreeCharacterStrings(ch);
						FreeMemory(ch);
						return NULL;
					}
				}
				break;
			} else {
				indent = 1;
				while (indent) {
					if (!PC_ExpectAnyToken(source, &token)) {
						FreeSource(source);
						BotFreeCharacterStrings(ch);
						FreeMemory(ch);
						return NULL;
					}
					if (!strcmp(token.string, "{"))
						indent++;
					else if (!strcmp(token.string, "}"))
						indent--;
				}
			}
		} else {
			SourceError(source, "unknown definition %s", token.string);
			FreeSource(source);
			BotFreeCharacterStrings(ch);
			FreeMemory(ch);
			return NULL;
		}
	}
	FreeSource(source);

	if (!foundcharacter) {
		BotFreeCharacterStrings(ch);
		FreeMemory(ch);
		return NULL;
	}
	return ch;
}

static int BotFindCachedCharacter(const char *charfile, float skill) {
	int handle;

	for (handle = 1; handle <= MAX_CLIENTS; handle++) {
		if (!botcharacters[handle])
			continue;
		if (strcmp(botcharacters[handle]->filename, charfile) == 0 &&
			(skill < 0 || fabs(botcharacters[handle]->skill - skill) < 0.01)) {
			return handle;
		}
	}
	return 0;
}

static int BotLoadCachedCharacter(const char *charfile, float skill, int reload) {
	int handle, cachedhandle, intskill;
	bot_character_t *ch = NULL;
#ifdef DEBUG
	int starttime;

	starttime = Sys_MilliSeconds();
#endif // DEBUG

	// find a free spot for a character
	for (handle = 1; handle <= MAX_CLIENTS; handle++) {
		if (!botcharacters[handle])
			break;
	}
	if (handle > MAX_CLIENTS)
		return 0;
	// try to load a cached character with the given skill
	if (!reload) {
		cachedhandle = BotFindCachedCharacter(charfile, skill);
		if (cachedhandle) {
			botimport.Print(PRT_MESSAGE, "loaded cached skill %f from %s\n", skill, charfile);
			return cachedhandle;
		}
	}

	intskill = (int)(skill + 0.5);
	// try to load the character with the given skill
	ch = BotLoadCharacterFromFile(charfile, intskill);
	if (ch) {
		botcharacters[handle] = ch;

		botimport.Print(PRT_MESSAGE, "loaded skill %d from %s\n", intskill, charfile);
#ifdef DEBUG
		if (botDeveloper) {
			botimport.Print(PRT_MESSAGE, "skill %d loaded in %d msec from %s\n", intskill,
							Sys_MilliSeconds() - starttime, charfile);
		}
#endif // DEBUG
		return handle;
	}

	botimport.Print(PRT_WARNING, "couldn't find skill %d in %s\n", intskill, charfile);

	if (!reload) {
		// try to load a cached default character with the given skill
		cachedhandle = BotFindCachedCharacter(DEFAULT_CHARACTER, skill);
		if (cachedhandle) {
			botimport.Print(PRT_MESSAGE, "loaded cached default skill %d from %s\n", intskill, charfile);
			return cachedhandle;
		}
	}
	// try to load the default character with the given skill
	ch = BotLoadCharacterFromFile(DEFAULT_CHARACTER, intskill);
	if (ch) {
		botcharacters[handle] = ch;
		botimport.Print(PRT_MESSAGE, "loaded default skill %d from %s\n", intskill, charfile);
		return handle;
	}

	if (!reload) {
		// try to load a cached character with any skill
		cachedhandle = BotFindCachedCharacter(charfile, -1);
		if (cachedhandle) {
			botimport.Print(PRT_MESSAGE, "loaded cached skill %f from %s\n", botcharacters[cachedhandle]->skill,
							charfile);
			return cachedhandle;
		}
	}
	// try to load a character with any skill
	ch = BotLoadCharacterFromFile(charfile, -1);
	if (ch) {
		botcharacters[handle] = ch;
		botimport.Print(PRT_MESSAGE, "loaded skill %f from %s\n", ch->skill, charfile);
		return handle;
	}

	if (!reload) {
		// try to load a cached character with any skill
		cachedhandle = BotFindCachedCharacter(DEFAULT_CHARACTER, -1);
		if (cachedhandle) {
			botimport.Print(PRT_MESSAGE, "loaded cached default skill %f from %s\n", botcharacters[cachedhandle]->skill,
							charfile);
			return cachedhandle;
		}
	}
	// try to load a character with any skill
	ch = BotLoadCharacterFromFile(DEFAULT_CHARACTER, -1);
	if (ch) {
		botcharacters[handle] = ch;
		botimport.Print(PRT_MESSAGE, "loaded default skill %f from %s\n", ch->skill, charfile);
		return handle;
	}

	botimport.Print(PRT_WARNING, "couldn't load any skill from %s\n", charfile);
	// couldn't load any character
	return 0;
}

static int BotLoadCharacterSkill(const char *charfile, float skill) {
	int ch, defaultch;

	defaultch = BotLoadCachedCharacter(DEFAULT_CHARACTER, skill, qfalse);
	ch = BotLoadCachedCharacter(charfile, skill, LibVarGetValue("bot_reloadcharacters"));

	if (defaultch && ch) {
		BotDefaultCharacteristics(botcharacters[ch], botcharacters[defaultch]);
	}

	return ch;
}

static int BotInterpolateCharacters(int handle1, int handle2, float desiredskill) {
	bot_character_t *ch1, *ch2, *out;
	int i, handle;
	float scale;

	ch1 = BotCharacterFromHandle(handle1);
	ch2 = BotCharacterFromHandle(handle2);
	if (!ch1 || !ch2)
		return 0;
	// find a free spot for a character
	for (handle = 1; handle <= MAX_CLIENTS; handle++) {
		if (!botcharacters[handle])
			break;
	}
	if (handle > MAX_CLIENTS)
		return 0;
	out = (bot_character_t *)GetClearedMemory(sizeof(bot_character_t) +
											  MAX_CHARACTERISTICS * sizeof(bot_characteristic_t));
	out->skill = desiredskill;
	strcpy(out->filename, ch1->filename);
	botcharacters[handle] = out;

	scale = (float)(desiredskill - ch1->skill) / (ch2->skill - ch1->skill);
	for (i = 0; i < MAX_CHARACTERISTICS; i++) {

		if (ch1->c[i].type == CT_FLOAT && ch2->c[i].type == CT_FLOAT) {
			out->c[i].type = CT_FLOAT;
			out->c[i].value._float = ch1->c[i].value._float + (ch2->c[i].value._float - ch1->c[i].value._float) * scale;
		} else if (ch1->c[i].type == CT_INTEGER) {
			out->c[i].type = CT_INTEGER;
			out->c[i].value.integer = ch1->c[i].value.integer;
		} else if (ch1->c[i].type == CT_STRING) {
			out->c[i].type = CT_STRING;
			out->c[i].value.string = (char *)GetMemory(strlen(ch1->c[i].value.string) + 1);
			strcpy(out->c[i].value.string, ch1->c[i].value.string);
		}
	}
	return handle;
}

int BotLoadCharacter(const char *charfile, float skill) {
	int firstskill, secondskill, handle;

	// make sure the skill is in the valid range
	if (skill < 1.0f)
		skill = 1.0f;
	else if (skill > 5.0f)
		skill = 5.0f;
	// skill 1, 4 and 5 should be available in the character files
	if (skill == 1.0f || skill == 4.0f || skill == 5.0f) {
		return BotLoadCharacterSkill(charfile, skill);
	}
	// check if there's a cached skill
	handle = BotFindCachedCharacter(charfile, skill);
	if (handle) {
		botimport.Print(PRT_MESSAGE, "loaded cached skill %f from %s\n", skill, charfile);
		return handle;
	}
	if (skill < 4.0f) {
		// load skill 1 and 4
		firstskill = BotLoadCharacterSkill(charfile, 1);
		if (!firstskill)
			return 0;
		secondskill = BotLoadCharacterSkill(charfile, 4);
		if (!secondskill)
			return firstskill;
	} else {
		// load skill 4 and 5
		firstskill = BotLoadCharacterSkill(charfile, 4);
		if (!firstskill)
			return 0;
		secondskill = BotLoadCharacterSkill(charfile, 5);
		if (!secondskill)
			return firstskill;
	}
	// interpolate between the two skills
	handle = BotInterpolateCharacters(firstskill, secondskill, skill);
	if (!handle)
		return 0;
	// write the character to the log file
	BotDumpCharacter(botcharacters[handle]);

	return handle;
}

static int CheckCharacteristicIndex(int character, int index) {
	bot_character_t *ch;

	ch = BotCharacterFromHandle(character);
	if (!ch)
		return qfalse;
	if (index < 0 || index >= MAX_CHARACTERISTICS) {
		botimport.Print(PRT_ERROR, "characteristic %d does not exist\n", index);
		return qfalse;
	}
	if (!ch->c[index].type) {
		botimport.Print(PRT_ERROR, "characteristic %d is not initialized\n", index);
		return qfalse;
	}
	return qtrue;
}

float Characteristic_Float(int character, int index) {
	bot_character_t *ch;

	ch = BotCharacterFromHandle(character);
	if (!ch)
		return 0;
	// check if the index is in range
	if (!CheckCharacteristicIndex(character, index))
		return 0;
	// an integer will be converted to a float
	if (ch->c[index].type == CT_INTEGER) {
		return (float)ch->c[index].value.integer;
	}
	// floats are just returned
	else if (ch->c[index].type == CT_FLOAT) {
		return ch->c[index].value._float;
	}
	// cannot convert a string pointer to a float
	else {
		botimport.Print(PRT_ERROR, "characteristic %d is not a float\n", index);
		return 0;
	}
	//	return 0;
}

float Characteristic_BFloat(int character, int index, float min, float max) {
	float value;
	bot_character_t *ch;

	ch = BotCharacterFromHandle(character);
	if (!ch)
		return 0;
	if (min > max) {
		botimport.Print(PRT_ERROR, "cannot bound characteristic %d between %f and %f\n", index, min, max);
		return 0;
	}
	value = Characteristic_Float(character, index);
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

int Characteristic_Integer(int character, int index) {
	bot_character_t *ch;

	ch = BotCharacterFromHandle(character);
	if (!ch)
		return 0;
	// check if the index is in range
	if (!CheckCharacteristicIndex(character, index))
		return 0;
	// an integer will just be returned
	if (ch->c[index].type == CT_INTEGER) {
		return ch->c[index].value.integer;
	}
	// floats are casted to integers
	else if (ch->c[index].type == CT_FLOAT) {
		return (int)ch->c[index].value._float;
	} else {
		botimport.Print(PRT_ERROR, "characteristic %d is not an integer\n", index);
		return 0;
	}
	//	return 0;
}

int Characteristic_BInteger(int character, int index, int min, int max) {
	int value;
	bot_character_t *ch;

	ch = BotCharacterFromHandle(character);
	if (!ch)
		return 0;
	if (min > max) {
		botimport.Print(PRT_ERROR, "cannot bound characteristic %d between %d and %d\n", index, min, max);
		return 0;
	}
	value = Characteristic_Integer(character, index);
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

void Characteristic_String(int character, int index, char *buf, int size) {
	bot_character_t *ch;

	ch = BotCharacterFromHandle(character);
	if (!ch)
		return;
	// check if the index is in range
	if (!CheckCharacteristicIndex(character, index))
		return;
	// an integer will be converted to a float
	if (ch->c[index].type == CT_STRING) {
		Q_strncpyz(buf, ch->c[index].value.string, size);
	} else {
		botimport.Print(PRT_ERROR, "characteristic %d is not a string\n", index);
	}
}

void BotShutdownCharacters(void) {
	int handle;

	for (handle = 1; handle <= MAX_CLIENTS; handle++) {
		if (botcharacters[handle]) {
			BotFreeCharacter2(handle);
		}
	}
}
