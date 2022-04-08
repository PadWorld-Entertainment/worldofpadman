/*
===========================================================================
Copyright (C) 2021 Padworld Entertainment

This file is part of the World of Padman source code.

World of Padman source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

World of Padman source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with World of Padman source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "q_shared.h"
#include "qcommon.h"

void SV_Shutdown(const char *finalmsg) {
}

void SV_ShutdownGameProgs(void) {
}

void SV_Init(void) {
}

void SV_Frame(int msec) {
}

void SV_PacketEvent(netadr_t from, msg_t *msg) {
}

int SV_FrameMsec(void) {
	return 0;
}

qboolean SV_GameCommand(void) {
	return qfalse;
}

int SV_SendQueuedPackets(void) {
	return 0;
}

void BotDrawDebugPolygons(void (*drawPoly)(int color, int numPoints, float *points), int value) {
}
