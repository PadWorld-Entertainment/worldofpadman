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
#include "../sys/sys_local.h"

void Sys_Print(const char *msg) {
}

void Sys_Error(const char *error, ...) {
	exit(1);
}

void Sys_SigHandler(int signal) {
	exit(signal);
}

const char *Sys_DefaultAppPath(void) {
	return "";
}

const char *Sys_DefaultInstallPath(void) {
	return ".";
}

void Sys_Quit(void) {
	exit(0);
}

void Sys_Init(void) {
}

char *Sys_ConsoleInput(void) {
	return NULL;
}

cpuFeatures_t Sys_GetProcessorFeatures(void) {
	return 0;
}

void Sys_InitPIDFile(const char *gamedir) {
}

void Sys_RemovePIDFile(const char *gamedir) {
}

void *Sys_LoadGameDll(const char *name, vmMainProc *entryPoint, intptr_t (*systemcalls)(intptr_t, ...)) {
	return NULL;
}

void Sys_UnloadDll(void *dllHandle) {
}
