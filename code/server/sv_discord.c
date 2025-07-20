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

#include "sv_discord.h"
#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "sv_http.h"
#include <string.h>

#ifdef USE_LOCAL_HEADERS
#include "SDL.h"
#else
#include <SDL.h>
#endif

static SDL_atomic_t shouldQuit;
static cvar_t *discord_webhook_url;
static cvar_t *discord_webhook_content;

#define MAX_COMMANDS 32

static SDL_Thread *consumerThread;
static SDL_mutex *commandQueueMutex;
static SDL_cond *queueNotEmpty;
static SDL_cond *queueNotFull;

typedef struct command_s {
	char user[64];
	char message[1024];
} command_t;

static command_t commandQueue[MAX_COMMANDS];
static int commandCount = 0;
static int producerIndex = 0;
static int consumerIndex = 0;

qboolean DISCORD_WantMessages(discordMsg_t type) {
	const unsigned int mask = Cvar_VariableIntegerValue("discord_webhook_content");
	return mask & type;
}

int DISCORD_EnqueueMessage(const char *user, const char *message) {
	if (consumerThread == NULL) {
		Com_DPrintf("Discord: Not initialized\n");
		return -1;
	}
	SDL_LockMutex(commandQueueMutex);
	if (commandCount >= MAX_COMMANDS) {
		SDL_UnlockMutex(commandQueueMutex);
		Com_Printf("Don't enqueue message from user %s - queue is full\n", user);
		return -1;
	}
	if (strlen(message) >= sizeof(commandQueue[producerIndex].message)) {
		Com_DPrintf("Message is too long and will get cut for user %s", user);
	}
	Com_DPrintf("Enqueue message from user %s\n", user);
	Q_strncpyz(commandQueue[producerIndex].user, user, sizeof(commandQueue[producerIndex].user));
	Q_strncpyz(commandQueue[producerIndex].message, message, sizeof(commandQueue[producerIndex].message));
	producerIndex = (producerIndex + 1) % MAX_COMMANDS;
	++commandCount;
	SDL_CondSignal(queueNotEmpty);
	SDL_UnlockMutex(commandQueueMutex);
	return 0;
}

static int DISCORD_Threadfn(void *data) {
	command_t command;
	while (1) {
		SDL_LockMutex(commandQueueMutex);
		while (commandCount <= 0 && !SDL_AtomicGet(&shouldQuit)) {
			SDL_CondWait(queueNotEmpty, commandQueueMutex);
		}
		if (SDL_AtomicGet(&shouldQuit)) {
			SDL_UnlockMutex(commandQueueMutex);
			break;
		}
		command = commandQueue[consumerIndex];
		consumerIndex = (consumerIndex + 1) % MAX_COMMANDS;
		--commandCount;
		SDL_CondSignal(queueNotFull);
		SDL_UnlockMutex(commandQueueMutex);

		DISCORD_SendMessage(command.user, command.message);
	}
	return 0;
}

static void NULL_WriteCallback(unsigned char *buf, int size) {
}

static int DISCORD_SendWebHook(const char *headers, const char *body) {
	const int statusCode = HTTP_ExecutePOST(discord_webhook_url->string, headers, body, NULL_WriteCallback);
	return statusCode;
}

int DISCORD_Init(void) {
	// e.g. https://discord.com/api/webhooks/xxx/yyy
	discord_webhook_url = Cvar_Get("discord_webhook_url", "", CVAR_ARCHIVE);
	discord_webhook_content = Cvar_Get("discord_webhook_content", "1", CVAR_ARCHIVE);
	Cvar_SetDescription(discord_webhook_content, "A bitmask of the content to send to the discord webhook");

	commandQueueMutex = SDL_CreateMutex();
	queueNotEmpty = SDL_CreateCond();
	queueNotFull = SDL_CreateCond();

	consumerThread = SDL_CreateThread(DISCORD_Threadfn, "DiscordConsumerThread", NULL);

	if (discord_webhook_url->string[0] == '\0') {
		Com_Printf("discord webhook initialized - but no discord_webhook_url value is set\n");
	} else {
		Com_Printf("discord webhook initialized\n");
	}

	return 0;
}

void DISCORD_Close(void) {
	SDL_AtomicSet(&shouldQuit, 1);
	SDL_CondSignal(queueNotEmpty);
	SDL_WaitThread(consumerThread, NULL);

	SDL_DestroyMutex(commandQueueMutex);
	SDL_DestroyCond(queueNotEmpty);
	SDL_DestroyCond(queueNotFull);
}

#define USE_DISCORD_COLORS 0

int DISCORD_SendMessage(const char *user, const char *msg) {
	const char *headers = "Content-Type: application/json; charset=UTF-8";
	char payload[2048];
	char jsonBodyContent[2048] = "";
	char userContent[128] = "";
	int statusCode;
	int len;
	int length = 0;
#if USE_DISCORD_COLORS
	static int q3ToAnsi[] = {
		30, // COLOR_BLACK
		31, // COLOR_RED
		32, // COLOR_GREEN
		33, // COLOR_YELLOW
		34, // COLOR_BLUE
		36, // COLOR_CYAN
		35, // COLOR_MAGENTA
		0	// COLOR_WHITE
	};
#endif
	if (discord_webhook_url->string[0] == '\0') {
		return -1;
	}

	while (*msg) {
		if (length > sizeof(jsonBodyContent) - 2) {
			return -2;
		}
		if (*msg == '\n') {
#if USE_DISCORD_COLORS
			const char *convert = "\x1B[0m\\n";
			Q_strcat(jsonBodyContent, sizeof(jsonBodyContent), convert);
			length += strlen(convert);
#else
			jsonBodyContent[length++] = '\\';
			jsonBodyContent[length++] = 'n';
#endif
			msg++;
		} else if (Q_IsColorString(msg)) {
#if USE_DISCORD_COLORS
			const char *color = va("\x1B[%dm", q3ToAnsi[ColorIndex(*(msg + 1))]);
			Q_strcat(jsonBodyContent, sizeof(jsonBodyContent), color);
			length += strlen(color);
#endif
			msg += 2;
		} else {
			jsonBodyContent[length++] = *msg++;
		}
	}
	jsonBodyContent[length] = '\0';

	length = 0;
	while (*user) {
		if (length > sizeof(userContent) - 2) {
			return -2;
		}
		if (Q_IsColorString(user)) {
#if USE_DISCORD_COLORS
			const char *color = va("\x1B[%dm", q3ToAnsi[ColorIndex(*(msg + 1))]);
			Q_strcat(userContent, sizeof(userContent), color);
			length += strlen(color);
#endif
			user += 2;
		} else {
			userContent[length++] = *user++;
		}
	}
	userContent[length] = '\0';

	len = Com_sprintf(payload, sizeof(payload),
					  "{\"username\": \"%s\", \"allowed_mentions\": {\"parse\": []}, \"content\": \"%s\"}", userContent,
					  jsonBodyContent);
	if (len >= (int)sizeof(payload)) {
		return -2;
	}

	statusCode = DISCORD_SendWebHook(headers, payload);
	return statusCode;
}
