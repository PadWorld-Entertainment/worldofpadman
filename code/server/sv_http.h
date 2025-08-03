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

#ifndef SV_HTTP_H
#define SV_HTTP_H

extern int httpConnectTimeoutSecond;
extern int httpTimeoutSecond;

int HTTP_Init(void);
void HTTP_Close(void);
void HTTP_SetTimeouts(int connectTimeoutSeconds, int readWriteTimeoutSeconds);
int HTTP_ExecutePOST(const char *url, const char *headers, const char *body,
					 void (*writeCallback)(unsigned char *, int));
int HTTP_ExecuteGET(const char *url, const char *headers, void (*writeCallback)(unsigned char *, int));
int HTTP_Execute(const char *mode, const char *url, const char *headers, const char *body,
				 void (*writeCallback)(unsigned char *, int));

#endif
