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
//
// cg_info.c -- display information while data is being loading

#include "cg_local.h"

#define MAX_LOADING_PLAYER_ICONS 16
#define MAX_LOADING_ITEM_ICONS 26

static int loadingPlayerIconCount;
static int loadingItemIconCount;
static qhandle_t loadingPlayerIcons[MAX_LOADING_PLAYER_ICONS];
static qhandle_t loadingItemIcons[MAX_LOADING_ITEM_ICONS];

/*
======================
CG_LoadingString

======================
*/
void CG_LoadingString(const char *s) {
	Q_strncpyz(cg.infoScreenText, s, sizeof(cg.infoScreenText));

	trap_UpdateScreen();
}

/*
#######################
CG_ChangeLoadingProgress
#######################
*/
void CG_ChangeLoadingProgress(float f) {
	cg.loadingprogress = f;

	trap_UpdateScreen();
}

/*
===================
CG_LoadingItem
===================
*/
void CG_LoadingItem(int itemNum) {
	const gitem_t *item;

	item = &bg_itemlist[itemNum];

	if (item->icon && loadingItemIconCount < MAX_LOADING_ITEM_ICONS) {
		loadingItemIcons[loadingItemIconCount++] = trap_R_RegisterShaderNoMip(item->icon);
	}

	CG_LoadingString(item->pickup_name);
}

/*
===================
CG_LoadingClient
===================
*/
void CG_LoadingClient(int clientNum) {
	const char *info;
	char *skin;
	char personality[MAX_QPATH];
	char model[MAX_QPATH];
	char iconName[MAX_QPATH];

	info = CG_ConfigString(CS_PLAYERS + clientNum);

	if (loadingPlayerIconCount < MAX_LOADING_PLAYER_ICONS) {
		Q_strncpyz(model, Info_ValueForKey(info, "model"), sizeof(model));
		skin = strrchr(model, '/');
		if (skin) {
			*skin++ = '\0';
		} else {
			skin = DEFAULT_SKIN;
		}

		Com_sprintf(iconName, MAX_QPATH, "models/wop_players/%s/icon_%s", model, skin);

		loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip(iconName);
		if (!loadingPlayerIcons[loadingPlayerIconCount]) {
			Com_sprintf(iconName, MAX_QPATH, "models/wop_players/characters/%s/icon_%s", model, skin);
			loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip(iconName);
		}
		if (!loadingPlayerIcons[loadingPlayerIconCount]) {
			Com_sprintf(iconName, MAX_QPATH, "models/wop_players/%s/icon_%s", DEFAULT_MODEL, DEFAULT_SKIN);
			loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip(iconName);
		}
		if (loadingPlayerIcons[loadingPlayerIconCount]) {
			loadingPlayerIconCount++;
		}
	}

	Q_strncpyz(personality, Info_ValueForKey(info, "n"), sizeof(personality));
	Q_CleanStr(personality);

	CG_LoadingString(personality);
}

/*
====================
CG_DrawInformation

Draw all the status / pacifier stuff during level loading
====================
*/
void CG_DrawInformation(void) {
	const char *s;
	const char *info;
	qhandle_t levelshot;
	qhandle_t helppage;
	// levelshot ideal coords (assuming 640*480 resolution)
	int lsX = 104;
	int lsY = 78;
	int lsW = 432;
	int lsH = 324;
	// loading bar ideal coords (assuming 640*480 resolution)
	float lbX = 167;
	float lbY = 422;
	float lbW = 306;
	float lbH = 32;

	info = CG_ConfigString(CS_SERVERINFO);

	trap_R_SetColor(NULL);
	trap_R_DrawStretchPic(0, 0, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, 0, 0, 1, 1, trap_R_RegisterShaderNoMip("menu/bg/connecting"));

	s = Info_ValueForKey(info, "mapname");
	levelshot = trap_R_RegisterShaderNoMip(va("levelshots/%s", s));
	if (!levelshot) {
		levelshot = trap_R_RegisterShaderNoMip("levelshots/unknownmap");
	}

	switch (cgs.gametype) {
	case GT_FFA:
		info = "menu/info/ffa";
		break;
	case GT_SPRAYFFA:
		info = "menu/info/syc";
		break;
	case GT_LPS:
		info = "menu/info/lps";
		break;
	case GT_CATCH:
		info = "menu/info/ctkd";
		break;
	case GT_TEAM:
		info = "menu/info/ffa_team";
		break;
	case GT_FREEZETAG:
		info = "menu/info/ft";
		break;
	case GT_CTF:
		info = "menu/info/ctl";
		break;
	case GT_1FCTF:
		info = "menu/info/1lctl";
		break;
	case GT_SPRAY:
		info = "menu/info/syc_team";
		break;
	case GT_BALLOON:
		info = "menu/info/bb";
		break;
	default:
		info = "menu/info/ffa";
		break;
	}

	helppage = trap_R_RegisterShaderNoMip(info);

	CG_SetScreenPlacement(PLACE_CENTER, PLACE_CENTER);
	CG_DrawPic(lsX, lsY, lsW, lsH, levelshot);
	CG_DrawPic(lsX, lsY, lsW, lsH, helppage);

	CG_SetScreenPlacement(PLACE_CENTER, PLACE_BOTTOM);
	CG_AdjustFrom640(&lbX, &lbY, &lbW, &lbH);
	trap_R_DrawStretchPic(lbX, lbY, lbW * cg.loadingprogress, lbH, 0, 0, cg.loadingprogress, 1,
						  trap_R_RegisterShaderNoMip("menu/art/loadingbar"));
	CG_DrawPic(167, 422, 306, 32, trap_R_RegisterShaderNoMip("menu/art/loadingframe"));
}
