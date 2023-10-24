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

#ifndef DISCORD_H
#define DISCORD_H

int DISCORD_Init(void);
void DISCORD_Close(void);

// send a message of a user via configured discord webhook - blocking
// return a negative number on error and otherwise the http status code
int DISCORD_SendMessage(const char *user, const char *msg);
// send a message of a user via configured discord webhook - non-blocking
// return 0 on success
// return -1 if the queue is full and the message wasn't send
int DISCORD_EnqueueMessage(const char *user, const char *msg);

#endif
