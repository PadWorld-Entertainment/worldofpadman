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
#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define BARROWLT0 "menu/arrows/bigblu_lt0"
#define BARROWLT1 "menu/arrows/bigblu_lt1"
#define BARROWRT0 "menu/arrows/bigblu_rt0"
#define BARROWRT1 "menu/arrows/bigblu_rt1"
#define BARROWUP0 "menu/arrows/bigyel_up0"
#define BARROWUP1 "menu/arrows/bigyel_up1"
#define BARROWDN0 "menu/arrows/bigyel_dn0"
#define BARROWDN1 "menu/arrows/bigyel_dn1"
#define SARROWLT0 "menu/arrows/smallyel_lt0"
#define SARROWLT1 "menu/arrows/smallyel_lt1"
#define SARROWRT0 "menu/arrows/smallyel_rt0"
#define SARROWRT1 "menu/arrows/smallyel_rt1"

#define ID_NAME 10
#define ID_SEX 11
#define ID_HANDICAP 12
#define ID_SKINCOLOR 13
#define ID_EFFECTS 14
#define ID_BACK 15

#define ID_PREVMODEL 16
#define ID_NEXTMODEL 17
#define ID_PREVSKIN 18
#define ID_NEXTSKIN 19
#define ID_MICON 20 //+4
#define ID_SICON 25 //+2
#define ID_PLAYERMODEL 28
#define ID_SPRAYCOLOR 29
#define ID_NEXTLOGO 30
#define ID_PREVLOGO 31

#define MAX_NAMELENGTH 20

#define DISPLAYED_MODELS 4
#define SKINGRID_COLS 1
#define SKINGRID_ROWS 3
#define SKINSPERPAGE (SKINGRID_ROWS * SKINGRID_COLS)

#define XPOSITION 24
#define YPOSITION 168

typedef struct {
	menuframework_s menu;

	menutext_s banner;
	menubitmap_s framer;
	menubitmap_s player;

	menutext_s nameheader;
	menufield_s name;
	menulist_s sex;
	menulist_s handicap;
	menutext_s logoheader;
	menulist_s skincolor;
	menulist_s effects;

	menubitmap_s back;
	menubitmap_s model;

	menubitmap_s arrowleft;
	menubitmap_s arrowright;
	menubitmap_s arrowup;
	menubitmap_s arrowdown;

	menubitmap1024s_s model_icons[DISPLAYED_MODELS];
	menubitmap1024s_s skin_icons[3];

	menubitmap_s logoleft;
	menubitmap_s logoright;

	menubitmap_s spraycolor;

	int firstmodel;
	int firstskin;
	int chosenskins[2];
	qboolean modelhold;
	int lastCursorX;
	int slogo_num;
	int nextGestureTime;

	menubitmap_s item_null;

	playerInfo_t playerinfo;
	int current_fx;
	char playerModel[MAX_QPATH];
} playersettings_t;

static playersettings_t s_playersettings;

static int gamecodetoui[] = {4, 2, 3, 0, 5, 1, 6};
static int uitogamecode[] = {4, 6, 2, 3, 1, 5, 7};

static const char *sex_items[] = {"None", "Male", "Female", NULL};

static const char *handicap_items[] = {"None", "90", "80", "70", "60", "50", "40", "30", "20", "10", NULL};

static const char *skincolor_items[] = {S_COLOR_YELLOW "Default", S_COLOR_BLUE "Blue", S_COLOR_RED "Red", NULL};

#define MAX_UIMODELS 96 // 32
#define MAX_SKINS 640	// padman has 18 skins ...
#define MAX_MODELSKINNAME_STR 64
typedef struct {
	char name[MAX_MODELSKINNAME_STR];
	qhandle_t icon;
} SkinData_t;

typedef struct {
	qhandle_t modelicons[MAX_UIMODELS];
	qhandle_t modeliconsB[MAX_UIMODELS];
	int nummodel;
	int lastskinicon[MAX_UIMODELS];
	SkinData_t modelskins[MAX_SKINS];
} ps_playericons_t;

static ps_playericons_t ps_playericons;

/*
=================
PlayerSettings_DrawName
=================
*/
static void PlayerSettings_DrawName(void *self) {
	menufield_s *f;
	qboolean focus;
	int style;
	const char *txt;
	char c;
	const float *color;
	int n;
	int basex, x, y;

	f = (menufield_s *)self;
	basex = f->generic.x;
	y = f->generic.y;
	focus = (f->generic.parent->cursor == f->generic.menuPosition);

	style = UI_LEFT | UI_SMALLFONT;
	color = text_color_normal;
	if (focus) {
		style |= UI_PULSE;
		color = text_color_highlight;
	}

	basex += 8;
	y += PROP_HEIGHT;
	txt = f->field.buffer;
	color = g_color_table[ColorIndex(COLOR_WHITE)];
	x = basex;
	while ((c = *txt) != 0) {
		if (!focus && Q_IsColorString(txt)) {
			n = ColorIndex(*(txt + 1));
			if (n == 0) {
				n = 7;
			}
			color = g_color_table[n];
			txt += 2;
			continue;
		}
		UI_DrawChar(x, y, c, style, color);
		txt++;
		x += SMALLCHAR_WIDTH;
	}

	// draw cursor if we have focus
	if (focus) {
		if (trap_Key_GetOverstrikeMode()) {
			c = FONT_ASCII_FULLBLOCK;
		} else {
			c = FONT_ASCII_UNDERLINE;
		}

		style &= ~UI_PULSE;
		style |= UI_BLINK;

		UI_DrawChar(basex + f->field.cursor * SMALLCHAR_WIDTH, y, c, style, color_white);
	}
}

/*
=================
PlayerSettings_DrawPlayer
=================
*/
static void PlayerSettings_DrawPlayer(void *self) {
	menubitmap_s *b;
	vec3_t viewangles;
	char buf[MAX_QPATH];

	trap_Cvar_VariableStringBuffer("model", buf, sizeof(buf));
	if (strcmp(buf, s_playersettings.playerModel) != 0) {
		UI_PlayerInfo_SetModel(&s_playersettings.playerinfo, buf);
		Q_strncpyz(s_playersettings.playerModel, buf, sizeof(s_playersettings.playerModel));

		viewangles[YAW] = 180 + 10;
		viewangles[PITCH] = 0;
		viewangles[ROLL] = 0;
		UI_PlayerInfo_SetInfo(&s_playersettings.playerinfo, LEGS_IDLE, TORSO_STAND, viewangles, vec3_origin, WP_NIPPER,
							  qfalse);
		s_playersettings.nextGestureTime = uis.realtime + 2000;
	}

	b = (menubitmap_s *)self;

	// YEW=1=right/left
	//	s_playersettings.playerinfo.viewAngles[YAW] =
	//	s_playersettings.playerinfo.legs.yawAngle =
	//	s_playersettings.playerinfo.torso.yawAngle = uis.realtime;

	UI_DrawPlayer(b->generic.x, b->generic.y, b->width, b->height, &s_playersettings.playerinfo, uis.realtime / 2);

	//	UI_DrawRect(b->generic.x, b->generic.y, b->width, b->height, colorGreen);
}

/*
=================
PlayerSettings_SaveChanges
=================
*/
static void PlayerSettings_SaveChanges(void) {
	// name
	trap_Cvar_Set("name", s_playersettings.name.field.buffer);

	// handicap
	trap_Cvar_SetValue("handicap", 100 - s_playersettings.handicap.curvalue * 10);

	// effects color
	trap_Cvar_SetValue("color1", uitogamecode[s_playersettings.effects.curvalue]);

	trap_Cvar_Set("syc_logo", uis.spraylogoNames[s_playersettings.slogo_num]);

	if (trap_Cvar_VariableValue("cl_paused")) {
		trap_Cmd_ExecuteText(EXEC_APPEND, va("selectlogo \"%s\"\n", uis.spraylogoNames[s_playersettings.slogo_num]));
	}
}

/*
=================
PlayerSettings_MenuKey
=================
*/
static sfxHandle_t PlayerSettings_MenuKey(int key) {
	int newweapon;

	if (key == K_MOUSE2 || key == K_ESCAPE) {
		PlayerSettings_SaveChanges();
	}

	if (key == K_MWHEELDOWN || key == K_PGDN) {
		if (s_playersettings.playerinfo.weapon >= WP_IMPERIUS)
			newweapon = WP_PUNCHY;
		else
			newweapon = s_playersettings.playerinfo.weapon + 1;

		UI_PlayerInfo_SetInfo(&s_playersettings.playerinfo, LEGS_IDLE,
							  (newweapon == WP_PUNCHY ? TORSO_STAND2 : TORSO_STAND),
							  s_playersettings.playerinfo.viewAngles, vec3_origin, newweapon, qfalse);
	} else if (key == K_MWHEELUP || key == K_PGUP) {
		if (s_playersettings.playerinfo.weapon <= WP_PUNCHY)
			newweapon = WP_IMPERIUS;
		else
			newweapon = s_playersettings.playerinfo.weapon - 1;

		UI_PlayerInfo_SetInfo(&s_playersettings.playerinfo, LEGS_IDLE,
							  (newweapon == WP_PUNCHY ? TORSO_STAND2 : TORSO_STAND),
							  s_playersettings.playerinfo.viewAngles, vec3_origin, newweapon, qfalse);
	}

	return Menu_DefaultKey(&s_playersettings.menu, key);
}

/*
=================
PlayerSettings_SetMenuItems
=================
*/
static void PlayerSettings_SetMenuItems(void) {
	vec3_t viewangles;
	int c;
	int h;

	// name
	Q_strncpyz(s_playersettings.name.field.buffer, UI_Cvar_VariableString("name"),
			   sizeof(s_playersettings.name.field.buffer));

	// effects color
	c = UI_GetCvarInt("color1") - 1;
	if (c < 0 || c > 6) {
		c = 6;
	}
	s_playersettings.effects.curvalue = gamecodetoui[c];

	// model/skin
	memset(&s_playersettings.playerinfo, 0, sizeof(playerInfo_t));

	viewangles[YAW] = 180 - 30;
	viewangles[PITCH] = 0;
	viewangles[ROLL] = 0;

	UI_PlayerInfo_SetModel(&s_playersettings.playerinfo, UI_Cvar_VariableString("model"));
	UI_PlayerInfo_SetInfo(&s_playersettings.playerinfo, LEGS_IDLE, TORSO_STAND, viewangles, vec3_origin, WP_NIPPER,
						  qfalse);

	{
		int i;
		char modelname[32];
		int mnLen;
		Q_strncpyz(modelname, UI_Cvar_VariableString("model"), sizeof(modelname));

		// Set skin color menu item
		if (Q_stristr(modelname, "_blue") || Q_stristr(modelname, "/blue")) {
			s_playersettings.skincolor.curvalue = 1;
		} else if (Q_stristr(modelname, "_red") || Q_stristr(modelname, "/red")) {
			s_playersettings.skincolor.curvalue = 2;
		} else {
			s_playersettings.skincolor.curvalue = 0;
		}

		for (i = 0; i < sizeof(modelname); i++)
			if (modelname[i] == '/') {
				modelname[i] = '\0';
				break;
			}

		mnLen = strlen(modelname);

		for (i = 0; i < ps_playericons.nummodel; ++i) {
			const char *modelskinname = ps_playericons.modelskins[ps_playericons.lastskinicon[i]].name;
			if (!Q_stricmpn(modelname, modelskinname, mnLen)) {
				s_playersettings.firstskin = i == 0 ? 0 : (ps_playericons.lastskinicon[i - 1] + 1);

				s_playersettings.chosenskins[0] = s_playersettings.firstskin;
				s_playersettings.chosenskins[1] = ps_playericons.lastskinicon[i];

				s_playersettings.firstmodel =
					(i + DISPLAYED_MODELS < ps_playericons.nummodel) ? i : ps_playericons.nummodel - DISPLAYED_MODELS;
				break;
			}
		}
	}

	// handicap
	h = Com_Clamp(10, 100, trap_Cvar_VariableValue("handicap"));
	s_playersettings.handicap.curvalue = 10 - h / 10;
}

static int GetSpecialSkinScore(const char *iconPath) {
	if (NULL != Q_stristr(iconPath, "/default"))
		return 3;
	else if (NULL != Q_stristr(iconPath, "/blue"))
		return 2;
	else if (NULL != Q_stristr(iconPath, "/red"))
		return 1;
	return 0;
}

static int SkinComp(const void *vA, const void *vB) {
	const SkinData_t *a = (const SkinData_t *)vA;
	const SkinData_t *b = (const SkinData_t *)vB;

	const int a_special = GetSpecialSkinScore(a->name);
	const int b_special = GetSpecialSkinScore(b->name);

	if (a_special || b_special) {
		/// "higher score" should be "less" in compare (so that they get to the top by qsort) ... that's why it's "b-a"
		/// and not "a-b"
		return b_special - a_special;
	} else {
		return Q_stricmp(a->name, b->name);
	}
}

static void sortSkins(int first, int last) {
	qsort(&(ps_playericons.modelskins[first]), last - first + 1, sizeof(SkinData_t), SkinComp);
}

#define MAX_MODEL_DIR_LIST 2048
#define MAX_MODELFOLDER_FILELIST 2048

static const char fixedModelList[] =
	"padman\0padgirl\0monster\0piratepad\0padlilly\0fatpad\0beachpad\0paddybell\0padcho\0padking\0padpunk";
static const int numFixedModels = 11;

static qboolean IsEntryInList(const char *modelName, const char *list, int numListEntries) {
	int i = 0;
	const char *entryPtr = list;

	while (i < numListEntries) {
		const int entryLen = strlen(entryPtr);

		if (0 == Q_stricmp(modelName, entryPtr)) {
			return qtrue;
		}

		entryPtr += entryLen + 1;
		++i;
	}

	return qfalse;
}

static qboolean IsAFixedModelListEntry(const char *modelName) {
	return IsEntryInList(modelName, fixedModelList, numFixedModels);
}

static void TweakModelFolderList(char *list, int *numEntries) {
	char newList[MAX_MODEL_DIR_LIST];
	int newNumEntries = 0;
	char *entryPtr = NULL;
	char *newEntryPtr = NULL;
	int i = 0;

	memcpy(newList, fixedModelList, sizeof(fixedModelList));
	newNumEntries = numFixedModels;

	entryPtr = list;
	newEntryPtr = newList + sizeof(fixedModelList);
	while (i < (*numEntries)) {
		const int entryLen = strlen(entryPtr);

		/// HACKAROUND(SL): pk3-filesystem gives directory-names with '/' suffix (os-filesystem doesn't)
		if (entryPtr[entryLen - 1] == '/')
			entryPtr[entryLen - 1] = '\0';

		/// NOTE(SL): just recognized that there are duplicates if the same folder is in *.pk3 and os-filesystem -.-
		if (!IsAFixedModelListEntry(entryPtr) && !IsEntryInList(entryPtr, newList, newNumEntries)) {
			const int destSize = MAX_MODEL_DIR_LIST - (newEntryPtr - newList);
			if (destSize < (entryLen + 1)) {
				Com_Printf("ERROR: hit limit for modellist buffer ...");
				return;
			}

			Q_strncpyz(newEntryPtr, entryPtr, destSize);
			newEntryPtr += strlen(newEntryPtr) + 1;
			++newNumEntries;
		}

		entryPtr += entryLen + 1;
		++i;
	}

	memcpy(list, newList, MAX_MODEL_DIR_LIST);
	*numEntries = newNumEntries;
}

/*
#######################
PlayerSettings_BuildList
#######################
*/
static void PlayerSettings_BuildList(void) {
	int numdirs = 0;
	int numfiles = 0;
	char dirlist[MAX_MODEL_DIR_LIST];
	char filelist[MAX_MODELFOLDER_FILELIST];
	char skinname[64];
	char *dirptr = NULL;
	char *fileptr = NULL;
	int i = 0;
	int j = 0;
	int dirlen = 0;
	int filelen = 0;
	int tmpskinnum = 0;

	memset(&ps_playericons, 0, sizeof(ps_playericons));

	// iterate directory of all player models
	numdirs = trap_FS_GetFileList("models/wop_players", "/", dirlist, MAX_MODEL_DIR_LIST);
	TweakModelFolderList(dirlist, &numdirs);

	dirptr = dirlist;
	for (i = 0; i < numdirs && ps_playericons.nummodel < MAX_UIMODELS; i++, dirptr += dirlen + 1) {

		dirlen = strlen(dirptr);

		if (dirlen && dirptr[dirlen - 1] == '/')
			dirptr[dirlen - 1] = '\0';

		if (!strcmp(dirptr, ".") || !strcmp(dirptr, "..") || *dirptr == '\0')
			continue;

		if (!(ps_playericons.modelicons[ps_playericons.nummodel] =
				  trap_R_RegisterShaderNoMip(va("models/wop_players/%s/wop_menu", dirptr))))
			continue;
		ps_playericons.modeliconsB[ps_playericons.nummodel] =
			trap_R_RegisterShaderNoMip(va("models/wop_players/%s/wop_menuB", dirptr));

		// iterate all skin files in directory
		numfiles =
			trap_FS_GetFileList(va("models/wop_players/%s", dirptr), "tga;png", filelist, MAX_MODELFOLDER_FILELIST);
		fileptr = filelist;
		for (j = 0; j < numfiles && ps_playericons.lastskinicon[ps_playericons.nummodel] < MAX_SKINS;
			 j++, fileptr += filelen + 1) {
			filelen = strlen(fileptr);

			COM_StripExtension(fileptr, skinname, sizeof(skinname));

			// look for icon_???? and ignore all containing _blue or _red
			if (!Q_stricmpn(skinname, "icon_", 5) && !Q_stristr(skinname, "_blue") && !Q_stristr(skinname, "_red")) {
				ps_playericons.modelskins[tmpskinnum].icon =
					trap_R_RegisterShaderNoMip(va("models/wop_players/%s/%s", dirptr, skinname));
				Com_sprintf(ps_playericons.modelskins[tmpskinnum].name, MAX_MODELSKINNAME_STR, "%s/%s", dirptr,
							(char *)(&skinname[5]));

				tmpskinnum++;
			}
		}
		ps_playericons.lastskinicon[ps_playericons.nummodel] = tmpskinnum - 1;
		ps_playericons.nummodel++;
	}

	j = 0;
	for (i = 0; i < ps_playericons.nummodel; ++i) {
		sortSkins(j, ps_playericons.lastskinicon[i]);
		j = ps_playericons.lastskinicon[i] + 1;
	}
}

/*
#######################
PlayerSettings_Draw
#######################
*/
static void PlayerSettings_Draw(void) {
	int i;

	static char modelname[32];
	if (s_playersettings.nextGestureTime == 0)
		s_playersettings.nextGestureTime = uis.realtime + 5000;

	if (s_playersettings.nextGestureTime < uis.realtime) {
		UI_PlayerInfo_SetInfo(&s_playersettings.playerinfo, LEGS_IDLE, TORSO_GESTURE,
							  s_playersettings.playerinfo.viewAngles, vec3_origin, s_playersettings.playerinfo.weapon,
							  qfalse);

		s_playersettings.nextGestureTime = uis.realtime + 15000;
	}

	if (s_playersettings.modelhold) {
		if (trap_Key_IsDown(K_MOUSE1)) {
			s_playersettings.playerinfo.viewAngles[YAW] = s_playersettings.playerinfo.legs.yawAngle =
				(s_playersettings.playerinfo.torso.yawAngle += uis.cursorx - s_playersettings.lastCursorX);

			s_playersettings.lastCursorX = uis.cursorx;
		} else
			s_playersettings.modelhold = qfalse;
	}

	for (i = 0; i < DISPLAYED_MODELS; i++) {
		s_playersettings.model_icons[i].shader = ps_playericons.modelicons[i + s_playersettings.firstmodel];
		s_playersettings.model_icons[i].mouseovershader = ps_playericons.modeliconsB[i + s_playersettings.firstmodel];
	}

	for (i = 0; i < 3; i++) {
		const int currentSkinIndex = s_playersettings.firstskin + i;

		if (currentSkinIndex > s_playersettings.chosenskins[1]) {
			s_playersettings.skin_icons[i].generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
		} else {
			s_playersettings.skin_icons[i].shader = ps_playericons.modelskins[currentSkinIndex].icon;
			s_playersettings.skin_icons[i].generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
		}
	}

	i = trap_Cvar_VariableValue("syc_color");
	i = Com_Clamp(0, (NUM_SPRAYCOLORS - 1), i);

	UI_SetColor(spraycolors[i]);
	UI_DrawHandlePic1024(76, 470, 96, 96, uis.spraylogoShaders[s_playersettings.slogo_num]);
	UI_SetColor(NULL);

	Q_strncpyz(modelname, UI_Cvar_VariableString("model"), sizeof(modelname));

	for (i = 0; i < sizeof(modelname); i++)
		if (modelname[i] == '/') {
			modelname[i] = '\0';
			break;
		}

	if (!Q_stricmp(&modelname[i + 1], "default") || !Q_stricmp(&modelname[i + 1], "blue") ||
		!Q_stricmp(&modelname[i + 1], "red")) {
		UI_DrawProportionalString(320, 440, modelname, UI_CENTER | UI_SMALLFONT, colorWhite);
	} else if (Q_stristr(&modelname[i + 1], "_blue") || Q_stristr(&modelname[i + 1], "_red")) {
		char *chrptr = strstr(&modelname[i + 1], "_");
		if (chrptr != NULL) {
			*chrptr = '\0';
		}
		UI_DrawProportionalString(320, 440, &modelname[i + 1], UI_CENTER | UI_SMALLFONT, colorWhite);
	} else {
		UI_DrawProportionalString(320, 440, &modelname[i + 1], UI_CENTER | UI_SMALLFONT, colorWhite);
	}

	Menu_Draw(&s_playersettings.menu);
}

/*
=================
UI_PlayerSettings_Update
=================
*/
static void UI_PlayerSettings_Update(void) {
	if (ps_playericons.nummodel > DISPLAYED_MODELS) {
		s_playersettings.arrowleft.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		s_playersettings.arrowright.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		s_playersettings.arrowleft.generic.flags |= QMF_GRAYED;
		s_playersettings.arrowright.generic.flags |= QMF_GRAYED;

		if (s_playersettings.firstmodel > 0) {
			s_playersettings.arrowleft.generic.flags &= ~QMF_GRAYED;
		}

		if (s_playersettings.firstmodel + DISPLAYED_MODELS < ps_playericons.nummodel) {
			s_playersettings.arrowright.generic.flags &= ~QMF_GRAYED;
		}
	} else {
		s_playersettings.arrowleft.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;
		s_playersettings.arrowright.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;
	}

	if (s_playersettings.chosenskins[0] < SKINSPERPAGE) {
		s_playersettings.arrowup.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		s_playersettings.arrowdown.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		s_playersettings.arrowup.generic.flags |= QMF_GRAYED;
		s_playersettings.arrowdown.generic.flags |= QMF_GRAYED;

		if (s_playersettings.firstskin > s_playersettings.chosenskins[0]) {
			s_playersettings.arrowup.generic.flags &= ~QMF_GRAYED;
		}

		if (s_playersettings.firstskin + (SKINSPERPAGE - 1) < s_playersettings.chosenskins[1]) {
			s_playersettings.arrowdown.generic.flags &= ~QMF_GRAYED;
		}
	} else {
		s_playersettings.arrowup.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;
		s_playersettings.arrowdown.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;
	}
}

/*
=================
UI_PlayerSettings_SetSkinColor
=================
*/
static void UI_PlayerSettings_SetSkinColor(void) {
	char modelname[64];

	Q_strncpyz(modelname, UI_Cvar_VariableString("model"), sizeof(modelname));
	if (Q_stristr(modelname, "/default") || Q_stristr(modelname, "/blue") || Q_stristr(modelname, "/red")) {
		char *chrptr = strstr(modelname, "/");
		if (chrptr != NULL) {
			*chrptr = '\0';
		}
		if (s_playersettings.skincolor.curvalue == 2) {
			Q_strcat(modelname, sizeof(modelname), "/red");
		} else if (s_playersettings.skincolor.curvalue == 1) {
			Q_strcat(modelname, sizeof(modelname), "/blue");
		} else {
			Q_strcat(modelname, sizeof(modelname), "/default");
		}
	} else {
		char *chrptr = strstr(modelname, "_");
		if (chrptr != NULL) {
			*chrptr = '\0';
		}
		if (s_playersettings.skincolor.curvalue == 2) {
			Q_strcat(modelname, sizeof(modelname), "_red");
		} else if (s_playersettings.skincolor.curvalue == 1) {
			Q_strcat(modelname, sizeof(modelname), "_blue");
		}
	}
	trap_Cvar_Set("model", modelname);
	trap_Cvar_Set("headmodel", modelname);
	trap_Cvar_Set("team_model", modelname);
	trap_Cvar_Set("team_headmodel", modelname);
}

/*
=================
PlayerSettings_MenuEvent
=================
*/
static void PlayerSettings_MenuEvent(void *ptr, int event) {
	int tmpid, i;

	if (event != QM_ACTIVATED) {
		return;
	}

	tmpid = ((menucommon_s *)ptr)->id;
	switch (tmpid) {

	case ID_HANDICAP:
		trap_Cvar_Set("handicap", va("%i", 100 - 25 * s_playersettings.handicap.curvalue));
		break;

	case ID_SKINCOLOR:
		UI_PlayerSettings_SetSkinColor();
		break;

	case ID_BACK:
		PlayerSettings_SaveChanges();
		UI_PopMenu();
		break;

	case ID_PREVMODEL:
		if (s_playersettings.firstmodel > 0) {
			s_playersettings.firstmodel--;
			UI_PlayerSettings_Update();
		}
		break;
	case ID_NEXTMODEL:
		if (s_playersettings.firstmodel + DISPLAYED_MODELS < ps_playericons.nummodel) {
			s_playersettings.firstmodel++;
			UI_PlayerSettings_Update();
		}
		break;
	case ID_PREVSKIN:
		if (s_playersettings.firstskin > s_playersettings.chosenskins[0]) {
			s_playersettings.firstskin = s_playersettings.firstskin - SKINSPERPAGE;
			UI_PlayerSettings_Update();
		}
		break;
	case ID_NEXTSKIN:
		if (s_playersettings.firstskin + 2 < s_playersettings.chosenskins[1]) {
			s_playersettings.firstskin = s_playersettings.firstskin + SKINSPERPAGE;
			UI_PlayerSettings_Update();
		}
		break;
	case ID_MICON:
	case ID_MICON + 1:
	case ID_MICON + 2:
	case ID_MICON + 3:
	case ID_MICON + 4:
		tmpid += (s_playersettings.firstmodel - ID_MICON);
		if (tmpid > 0)
			s_playersettings.chosenskins[0] = s_playersettings.firstskin = ps_playericons.lastskinicon[tmpid - 1] + 1;
		else
			s_playersettings.chosenskins[0] = s_playersettings.firstskin = 0;
		s_playersettings.chosenskins[1] = ps_playericons.lastskinicon[tmpid];

		for (i = s_playersettings.chosenskins[0]; i <= s_playersettings.chosenskins[1]; i++) {
			if (strstr(ps_playericons.modelskins[i].name, "default") != NULL) {
				char tmp[64], *chrptr;

				Q_strncpyz(tmp, ps_playericons.modelskins[i].name, 64);
				if ((chrptr = strchr(tmp, '/')) != NULL)
					*chrptr = '\0';
				trap_S_StartLocalSound(trap_S_RegisterSound(va("sound/feedback/players/%s", Q_strlwr(tmp)), qfalse),
									   CHAN_LOCAL_SOUND);

				trap_Cvar_Set("model", ps_playericons.modelskins[i].name);
				trap_Cvar_Set("headmodel", ps_playericons.modelskins[i].name);
				trap_Cvar_Set("team_model", ps_playericons.modelskins[i].name);
				trap_Cvar_Set("team_headmodel", ps_playericons.modelskins[i].name);
				break;
			}
		}
		UI_PlayerSettings_SetSkinColor();
		UI_PlayerSettings_Update();
		break;
	case ID_SICON:
	case ID_SICON + 1:
	case ID_SICON + 2:
		trap_Cvar_Set("model", ps_playericons.modelskins[tmpid - ID_SICON + s_playersettings.firstskin].name);
		trap_Cvar_Set("headmodel", ps_playericons.modelskins[tmpid - ID_SICON + s_playersettings.firstskin].name);
		trap_Cvar_Set("team_model", ps_playericons.modelskins[tmpid - ID_SICON + s_playersettings.firstskin].name);
		trap_Cvar_Set("team_headmodel", ps_playericons.modelskins[tmpid - ID_SICON + s_playersettings.firstskin].name);
		UI_PlayerSettings_SetSkinColor();
		break;
	case ID_PLAYERMODEL:
		if (trap_Key_IsDown(K_MOUSE1))
			s_playersettings.modelhold = qtrue;
		s_playersettings.lastCursorX = uis.cursorx;
		break;
	case ID_SPRAYCOLOR:
		//		Com_Printf("Spraycolor=%i\n",(uis.cursorx-((menucommon_s*)ptr)->x)/14);
		trap_Cvar_Set("syc_color", va("%i", (uis.cursorx - ((menucommon_s *)ptr)->x) / 14));
		break;

	case ID_PREVLOGO:
		if (--s_playersettings.slogo_num < 0)
			s_playersettings.slogo_num = uis.spraylogosLoaded - 1; // close the circle
		break;

	case ID_NEXTLOGO:
		if (++s_playersettings.slogo_num >= uis.spraylogosLoaded)
			s_playersettings.slogo_num = 0; // close the circle
		break;
	}
}

/*
=================
PlayerSettings_MenuInit
=================
*/
static void PlayerSettings_MenuInit(void) {
	int i;
	int y;

	memset(&s_playersettings, 0, sizeof(playersettings_t));

	UI_PlayerSettings_Cache();

	s_playersettings.menu.key = PlayerSettings_MenuKey;
	s_playersettings.menu.wrapAround = qtrue;
	s_playersettings.menu.fullscreen = qtrue;
	s_playersettings.chosenskins[1] = ps_playericons.lastskinicon[0];
	s_playersettings.menu.draw = PlayerSettings_Draw;
	s_playersettings.menu.bgparts = BGP_PLAYER;

	s_playersettings.item_null.generic.type = MTYPE_BITMAP;
	s_playersettings.item_null.generic.flags = QMF_LEFT_JUSTIFY | QMF_MOUSEONLY | QMF_SILENT;
	s_playersettings.item_null.generic.x = 0;
	s_playersettings.item_null.generic.y = 0;
	s_playersettings.item_null.width = 640;
	s_playersettings.item_null.height = 480;
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.item_null);

	s_playersettings.player.generic.type = MTYPE_BITMAP;
	s_playersettings.player.generic.flags = QMF_MOUSEONLY | QMF_SILENT;
	s_playersettings.player.generic.ownerdraw = PlayerSettings_DrawPlayer;
	s_playersettings.player.generic.x = 160;
	s_playersettings.player.generic.y = 15;
	s_playersettings.player.width = 320;
	s_playersettings.player.height = 560;
	s_playersettings.player.generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.player.generic.id = ID_PLAYERMODEL;
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.player);

	s_playersettings.arrowleft.generic.type = MTYPE_BITMAP;
	s_playersettings.arrowleft.generic.name = BARROWLT0;
	s_playersettings.arrowleft.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_playersettings.arrowleft.generic.x = 20;
	s_playersettings.arrowleft.generic.y = 20;
	s_playersettings.arrowleft.generic.id = ID_PREVMODEL;
	s_playersettings.arrowleft.generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.arrowleft.width = 30;
	s_playersettings.arrowleft.height = 70;
	s_playersettings.arrowleft.focuspic = BARROWLT1;
	s_playersettings.arrowleft.focuspicinstead = qtrue;

	s_playersettings.arrowright.generic.type = MTYPE_BITMAP;
	s_playersettings.arrowright.generic.name = BARROWRT0;
	s_playersettings.arrowright.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_playersettings.arrowright.generic.x = 588;
	s_playersettings.arrowright.generic.y = 20;
	s_playersettings.arrowright.generic.id = ID_NEXTMODEL;
	s_playersettings.arrowright.generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.arrowright.width = 30;
	s_playersettings.arrowright.height = 70;
	s_playersettings.arrowright.focuspic = BARROWRT1;
	s_playersettings.arrowright.focuspicinstead = qtrue;

	s_playersettings.arrowup.generic.type = MTYPE_BITMAP;
	s_playersettings.arrowup.generic.name = BARROWUP0;
	s_playersettings.arrowup.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_playersettings.arrowup.generic.x = 546;
	s_playersettings.arrowup.generic.y = 118;
	s_playersettings.arrowup.generic.id = ID_PREVSKIN;
	s_playersettings.arrowup.generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.arrowup.width = 70;
	s_playersettings.arrowup.height = 30;
	s_playersettings.arrowup.focuspic = BARROWUP1;
	s_playersettings.arrowup.focuspicinstead = qtrue;

	s_playersettings.arrowdown.generic.type = MTYPE_BITMAP;
	s_playersettings.arrowdown.generic.name = BARROWDN0;
	s_playersettings.arrowdown.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_playersettings.arrowdown.generic.x = 546;
	s_playersettings.arrowdown.generic.y = 426;
	s_playersettings.arrowdown.generic.id = ID_NEXTSKIN;
	s_playersettings.arrowdown.generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.arrowdown.width = 70;
	s_playersettings.arrowdown.height = 30;
	s_playersettings.arrowdown.focuspic = BARROWDN1;
	s_playersettings.arrowdown.focuspicinstead = qtrue;

	for (i = 0; i < DISPLAYED_MODELS; ++i) {
		s_playersettings.model_icons[i].generic.type = MTYPE_BITMAP1024S;
		s_playersettings.model_icons[i].x = 160 + i * (160 + 20);
		s_playersettings.model_icons[i].y = 16;
		s_playersettings.model_icons[i].w = s_playersettings.model_icons[i].h = 160;
		s_playersettings.model_icons[i].shader = 0;
		s_playersettings.model_icons[i].mouseovershader = 0;
		s_playersettings.model_icons[i].generic.callback = PlayerSettings_MenuEvent;
		s_playersettings.model_icons[i].generic.id = ID_MICON + i;
		//		s_playersettings.model_icons[i].generic.flags	= QMF_SILENT;
	}

	s_playersettings.skin_icons[0].generic.type = MTYPE_BITMAP1024S;
	s_playersettings.skin_icons[0].x = 867;
	s_playersettings.skin_icons[0].y = 253;
	s_playersettings.skin_icons[0].w = s_playersettings.skin_icons[0].h = 128;
	s_playersettings.skin_icons[0].sx = 866;
	s_playersettings.skin_icons[0].sy = 253;
	s_playersettings.skin_icons[0].sw = s_playersettings.skin_icons[0].sh = 138;
	s_playersettings.skin_icons[0].shader = 0;
	s_playersettings.skin_icons[0].shadowshader = trap_R_RegisterShaderNoMip("menu/art/sicon_shadow");
	s_playersettings.skin_icons[0].generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.skin_icons[0].generic.id = ID_SICON;

	s_playersettings.skin_icons[1].generic.type = MTYPE_BITMAP1024S;
	s_playersettings.skin_icons[1].x = 867;
	s_playersettings.skin_icons[1].y = 396;
	s_playersettings.skin_icons[1].w = s_playersettings.skin_icons[1].h = 128;
	s_playersettings.skin_icons[1].sx = 866;
	s_playersettings.skin_icons[1].sy = 395;
	s_playersettings.skin_icons[1].sw = s_playersettings.skin_icons[1].sh = 138;
	s_playersettings.skin_icons[1].shader = 0;
	s_playersettings.skin_icons[1].shadowshader = trap_R_RegisterShaderNoMip("menu/art/sicon_shadow");
	s_playersettings.skin_icons[1].generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.skin_icons[1].generic.id = ID_SICON + 1;

	s_playersettings.skin_icons[2].generic.type = MTYPE_BITMAP1024S;
	s_playersettings.skin_icons[2].x = 867;
	s_playersettings.skin_icons[2].y = 540;
	s_playersettings.skin_icons[2].w = s_playersettings.skin_icons[2].h = 128;
	s_playersettings.skin_icons[2].sx = 866;
	s_playersettings.skin_icons[2].sy = 539;
	s_playersettings.skin_icons[2].sw = s_playersettings.skin_icons[2].sh = 138;
	s_playersettings.skin_icons[2].shader = 0;
	s_playersettings.skin_icons[2].shadowshader = trap_R_RegisterShaderNoMip("menu/art/sicon_shadow");
	s_playersettings.skin_icons[2].generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.skin_icons[2].generic.id = ID_SICON + 2;

	{
		char spraylogoName[MAX_SPRAYLOGO_NAME];

		trap_Cvar_VariableStringBuffer("syc_logo", spraylogoName, sizeof(spraylogoName));

		if (spraylogoName[0] == '\0') {
			Q_strncpyz(spraylogoName, SPRAYLOGO_DEFAULT_NAME, sizeof(spraylogoName));
		}

		for (i = 0; i < uis.spraylogosLoaded; i++) {
			if (Q_stricmp(uis.spraylogoNames[i], spraylogoName) == 0) {
				s_playersettings.slogo_num = i;
				break;
			}
		}
	}

	s_playersettings.logoleft.generic.type = MTYPE_BITMAP;
	s_playersettings.logoleft.generic.name = SARROWLT0;
	s_playersettings.logoleft.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_playersettings.logoleft.generic.x = 48;
	s_playersettings.logoleft.generic.y = 357;
	s_playersettings.logoleft.generic.id = ID_PREVLOGO;
	s_playersettings.logoleft.generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.logoleft.width = 26;
	s_playersettings.logoleft.height = 13;
	s_playersettings.logoleft.focuspic = SARROWLT1;
	s_playersettings.logoleft.focuspicinstead = qtrue;

	s_playersettings.logoright.generic.type = MTYPE_BITMAP;
	s_playersettings.logoright.generic.name = SARROWRT0;
	s_playersettings.logoright.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_playersettings.logoright.generic.x = 81;
	s_playersettings.logoright.generic.y = 357;
	s_playersettings.logoright.generic.id = ID_NEXTLOGO;
	s_playersettings.logoright.generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.logoright.width = 26;
	s_playersettings.logoright.height = 13;
	s_playersettings.logoright.focuspic = SARROWRT1;
	s_playersettings.logoright.focuspicinstead = qtrue;

	s_playersettings.back.generic.type = MTYPE_BITMAP;
	s_playersettings.back.generic.name = BACK0;
	s_playersettings.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_playersettings.back.generic.x = 8;
	s_playersettings.back.generic.y = 440;
	s_playersettings.back.generic.id = ID_BACK;
	s_playersettings.back.generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.back.width = 80;
	s_playersettings.back.height = 40;
	s_playersettings.back.focuspic = BACK1;
	s_playersettings.back.focuspicinstead = qtrue;

	y = YPOSITION;
	s_playersettings.nameheader.generic.type = MTYPE_TEXT;
	s_playersettings.nameheader.generic.x = XPOSITION;
	s_playersettings.nameheader.generic.y = y;
	s_playersettings.nameheader.string = "Name:";
	s_playersettings.nameheader.style = UI_LEFT | UI_SMALLFONT;
	s_playersettings.nameheader.color = color_yellow;

	s_playersettings.name.generic.type = MTYPE_FIELD;
	s_playersettings.name.generic.flags = QMF_NODEFAULTINIT | QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	s_playersettings.name.generic.ownerdraw = PlayerSettings_DrawName;
	s_playersettings.name.field.widthInChars = MAX_NAMELENGTH;
	s_playersettings.name.field.maxchars = MAX_NAMELENGTH;
	s_playersettings.name.generic.x = XPOSITION;
	s_playersettings.name.generic.y = y - 10;
	s_playersettings.name.generic.left = XPOSITION - 12;
	s_playersettings.name.generic.top = y;
	s_playersettings.name.generic.right = XPOSITION + 128;
	s_playersettings.name.generic.bottom = y + 2 * (BIGCHAR_HEIGHT);

	y += 2 * (BIGCHAR_HEIGHT + 2);
	s_playersettings.sex.generic.type = MTYPE_SPINCONTROL;
	s_playersettings.sex.generic.name = "Sex:";
	s_playersettings.sex.generic.flags = QMF_SMALLFONT;
	s_playersettings.sex.generic.id = ID_HANDICAP;
	s_playersettings.sex.generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.sex.generic.x = XPOSITION + 56;
	s_playersettings.sex.generic.y = y;
	s_playersettings.sex.itemnames = sex_items;

	y += BIGCHAR_HEIGHT + 2;
	s_playersettings.handicap.generic.type = MTYPE_SPINCONTROL;
	s_playersettings.handicap.generic.name = "Cap:";
	s_playersettings.handicap.generic.flags = QMF_SMALLFONT;
	s_playersettings.handicap.generic.id = ID_HANDICAP;
	s_playersettings.handicap.generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.handicap.generic.x = XPOSITION + 56;
	s_playersettings.handicap.generic.y = y;
	s_playersettings.handicap.itemnames = handicap_items;

	y += BIGCHAR_HEIGHT + 2;
	s_playersettings.skincolor.generic.type = MTYPE_SPINCONTROL;
	s_playersettings.skincolor.generic.name = "Color:";
	s_playersettings.skincolor.generic.flags = QMF_SMALLFONT;
	s_playersettings.skincolor.generic.id = ID_SKINCOLOR;
	s_playersettings.skincolor.generic.callback = PlayerSettings_MenuEvent;
	s_playersettings.skincolor.generic.x = XPOSITION + 56;
	s_playersettings.skincolor.generic.y = y;
	s_playersettings.skincolor.itemnames = skincolor_items;

	y += 2 * (BIGCHAR_HEIGHT + 2);
	s_playersettings.logoheader.generic.type = MTYPE_TEXT;
	s_playersettings.logoheader.generic.x = XPOSITION;
	s_playersettings.logoheader.generic.y = y;
	s_playersettings.logoheader.string = "Spraylogo:";
	s_playersettings.logoheader.style = (UI_LEFT | UI_SMALLFONT);
	s_playersettings.logoheader.color = color_yellow;

	s_playersettings.spraycolor.generic.type = MTYPE_BITMAP;
	s_playersettings.spraycolor.generic.flags = QMF_LEFT_JUSTIFY;
	s_playersettings.spraycolor.generic.x = 36;
	s_playersettings.spraycolor.generic.y = 379;
	s_playersettings.spraycolor.width = 82;
	s_playersettings.spraycolor.height = 18;
	s_playersettings.spraycolor.generic.id = ID_SPRAYCOLOR;
	s_playersettings.spraycolor.generic.callback = PlayerSettings_MenuEvent;

	Menu_AddItem(&s_playersettings.menu, &s_playersettings.arrowleft);
	for (i = 0; i < DISPLAYED_MODELS; ++i) {
		Menu_AddItem(&s_playersettings.menu, &s_playersettings.model_icons[i]);
	}
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.arrowright);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.arrowup);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.skin_icons[0]);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.skin_icons[1]);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.skin_icons[2]);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.arrowdown);

	Menu_AddItem(&s_playersettings.menu, &s_playersettings.nameheader);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.name);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.sex);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.handicap);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.skincolor);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.logoheader);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.logoleft);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.logoright);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.spraycolor);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.back);

	PlayerSettings_SetMenuItems();
	UI_PlayerSettings_Update();
}

/*
=================
UI_PlayerSettings_Cache
=================
*/
void UI_PlayerSettings_Cache(void) {
	trap_R_RegisterShaderNoMip(BARROWLT0);
	trap_R_RegisterShaderNoMip(BARROWLT1);
	trap_R_RegisterShaderNoMip(BARROWRT0);
	trap_R_RegisterShaderNoMip(BARROWRT1);
	trap_R_RegisterShaderNoMip(BARROWUP0);
	trap_R_RegisterShaderNoMip(BARROWUP1);
	trap_R_RegisterShaderNoMip(BARROWDN0);
	trap_R_RegisterShaderNoMip(BARROWDN1);
	trap_R_RegisterShaderNoMip(SARROWLT0);
	trap_R_RegisterShaderNoMip(SARROWLT1);
	trap_R_RegisterShaderNoMip(SARROWRT0);
	trap_R_RegisterShaderNoMip(SARROWRT1);
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);

	PlayerSettings_BuildList();
}

/*
=================
UI_PlayerSettingsMenu
=================
*/
void UI_PlayerSettingsMenu(void) {
	PlayerSettings_MenuInit();
	UI_PushMenu(&s_playersettings.menu);
}
