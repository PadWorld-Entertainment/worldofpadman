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

/* TODO: Write some doku about this!

		 Maybe create some sort of interface to register storage handlers and let them
		 save their data into infostrings?
		 Why must string handling be so awful in C :(
*/

/*
	Called on clients' first connect
*/
void BE_InitClientStorageData(gclient_t *client) {
	clientStorage_t *stor;

	G_assert(client);

	stor = &client->storage;

	Com_Memset(&stor->ignoreList, 0, sizeof(stor->ignoreList));
	stor->firstTime = qtrue;
	stor->sawGreeting = qfalse;

	BE_WriteClientStorageData(client);
}

/*
	Called on game shutdown
*/

#define STORAGE_CVARNAME "storage"

void BE_WriteClientStorageData(const gclient_t *client) {
	char var[32];
	char buff[MAX_STRING_CHARS] = {""};
	int clientNum;
	int i;
	const clientStorage_t *stor;

	G_assert(client);

	clientNum = (client - level.clients);
	Com_sprintf(var, sizeof(var), STORAGE_CVARNAME "%d", clientNum);
	stor = &client->storage;

	for (i = 0; i < MAX_CLIENTS; i++) {
		Q_strcat(buff, sizeof(buff), va("%d ", stor->ignoreList[i]));
	}
	Q_strcat(buff, sizeof(buff), va("%d ", stor->firstTime));
	Q_strcat(buff, sizeof(buff), va("%d ", stor->sawGreeting));

	trap_Cvar_Set(var, buff);
}

/*
	Called on clients' reconnect
*/
void BE_ReadClientStorageData(gclient_t *client) {
	char buff[MAX_STRING_CHARS];
	int clientNum;
	char *var;
	char *endp;
	int i;
	clientStorage_t *stor;

	G_assert(client);

	clientNum = (client - level.clients);
	var = va(STORAGE_CVARNAME "%d", clientNum);
	stor = &client->storage;

	trap_Cvar_VariableStringBuffer(var, buff, sizeof(buff));

	endp = buff;
	for (i = 0; i < MAX_CLIENTS; i++) {
		stor->ignoreList[i] = strtol(endp, &endp, 10);
	}
	stor->firstTime = strtol(endp, &endp, 10);
	stor->sawGreeting = strtol(endp, &endp, 10);
}

#undef STORAGE_CVARNAME

/*
	Called on game startup
*/
void BE_InitWorldStorage(void) {
	return;
}

/*
	Called on game shutdown
*/
void BE_WriteStorageData(void) {
	int i;

	for (i = 0; i < level.maxclients; i++) {
		if (CON_CONNECTED == level.clients[i].pers.connected) {
			BE_WriteClientStorageData(&level.clients[i]);
		}
	}
}
