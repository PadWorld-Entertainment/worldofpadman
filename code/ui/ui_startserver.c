/*
=======================================================================

CREATE MENU (START SERVER)

=======================================================================
*/

#include "ui_local.h"

#define FIGHT0 "menu/buttons/fight0"
#define FIGHT1 "menu/buttons/fight1"
#define ARROWLT0 "menu/arrows/headblu_lt0"
#define ARROWLT1 "menu/arrows/headblu_lt1"
#define ARROWRT0 "menu/arrows/headblu_rt0"
#define ARROWRT1 "menu/arrows/headblu_rt1"
#define SELECTBOTS0 "menu/buttons/selectbots0"
#define SELECTBOTS1 "menu/buttons/selectbots1"
#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"

#define UNKNOWNMAP "menu/art/unknownmap"

#define MAX_MAPROWS 2
#define MAX_MAPCOLS 3
#define MAX_MAPSPERPAGE 9
#define OPTIONS_XPOS 700
#define GRID_XPOS 66
#define GRID_YPOS 40

#define MAX_SERVERSTEXT 8192

#define MAX_SERVERMAPS 64
#define MAX_NAMELENGTH 32 // old: 16 ... wop has some longer names(than vq3)
#define MAX_LONGNAMELENGTH 32
#define MAX_MAPSINLOOP 32

#define ID_GAMETYPE 10
#define ID_PICTURES 11 // 12, 13, 14, 15, 16
#define ID_PREVPAGE 17
#define ID_NEXTPAGE 18
#define ID_STARTSERVERBACK 19

#define ID_STARTSERVER 21
#define ID_SELECTBOTS 32

#define MAX_SELECTBOTS MAX_CLIENTS
#define MAX_BOTNAME 32

typedef struct {
	char name[MAX_BOTNAME];
	int team;
} selectbotinfo_t;

typedef struct {
	menuframework_s menu;

	menulist_s gametype;
	menubitmap_s mappics[MAX_MAPSPERPAGE];
	//	menubitmap_s	mapbuttons[MAX_MAPSPERPAGE];
	menubitmap_s arrowleft;
	menubitmap_s arrowright;

	menulist_s dedicated;
	menufield_s timelimit;
	menufield_s fraglimit;
	menufield_s maxclients;
	menuradiobutton_s friendlyfire;
	menuradiobutton_s instagib;
	menufield_s hostname;
	menuradiobutton_s pure;
	menufield_s startlives;

	menubitmap_s selectbots;

	menubitmap_s back;
	menubitmap_s fight;

	menubitmap_s item_null;

	qboolean multiplayer;
	int currentmap;
	int nummaps;
	int page;
	int maxpages;
	char maplist[MAX_SERVERMAPS][MAX_NAMELENGTH];
	char longnamemaplist[MAX_SERVERMAPS][MAX_LONGNAMELENGTH];
	int mapGamebits[MAX_SERVERMAPS];
	selectbotinfo_t selectbotinfos[MAX_SELECTBOTS];
	int botskill;
	int maploop[MAX_MAPSINLOOP];
	qhandle_t mapNumbers[10];
} startserver_t;

static startserver_t s_startserver;

static const char *gametype_items[] = {GAMETYPE_NAME(GT_FFA),		GAMETYPE_NAME(GT_TEAM),
									   GAMETYPE_NAME(GT_SPRAYFFA),	GAMETYPE_NAME(GT_SPRAY),
									   GAMETYPE_NAME(GT_CTF),		GAMETYPE_NAME(GT_BALLOON),
									   GAMETYPE_NAME(GT_LPS),		GAMETYPE_NAME(GT_CATCH),
									   GAMETYPE_NAME(GT_FREEZETAG), NULL};

static int gametype_remap[] = {GT_FFA,	   GT_TEAM, GT_SPRAYFFA, GT_SPRAY,	  GT_CTF,
							   GT_BALLOON, GT_LPS,	GT_CATCH,	 GT_FREEZETAG};
// NOTE: Maps g_gametype to gametype_items. Make sure these are defined for all GT_MAX_GAME_TYPE-1 gametypes
//       and remap to something reasonable for gametypes that do not have an entry in gametype_items.
static int gametype_remap2[] = {0, 0, 0, 2, 6, 7, 1, 8, 4, 3, 5};
static const char *teamstrs[] = {"free", "Red", "Blue", "spectator", 0};

// static void UI_ServerOptionsMenu( qboolean multiplayer );

/*
=================
UI_StartServer_GametypeBits
=================
*/
static int UI_StartServer_GametypeBits(const char *string) {
	int bits;
	const char *p;
	const char *token;

	bits = 0;
	p = string;
	while (1) {
		token = COM_ParseExt(&p, qfalse);
		if (token[0] == 0) {
			break;
		}

		if (Q_stricmp(token, GAMETYPE_NAME_SHORT(GT_FFA)) == 0) {
			bits |= 1 << GT_FFA;
			continue;
		}

		if (Q_stricmp(token, "tourney") == 0) {
			bits |= 1 << GT_TOURNAMENT;
			continue;
		}

		if (Q_stricmp(token, "single") == 0) {
			bits |= 1 << GT_SINGLE_PLAYER;
			continue;
		}

		if (Q_stricmp(token, "team") == 0) {
			bits |= 1 << GT_TEAM;
			continue;
		}

		if (Q_stricmp(token, GAMETYPE_NAME_SHORT(GT_FREEZETAG)) == 0) {
			bits |= 1 << GT_FREEZETAG;
			continue;
		}

		if (Q_stricmp(token, GAMETYPE_NAME_SHORT(GT_CATCH)) == 0) {
			bits |= 1 << GT_CATCH;
			continue;
		}

		if (Q_stricmp(token, "ctf") == 0 || Q_stricmp(token, GAMETYPE_NAME_SHORT(GT_CTF)) == 0) {
			bits |= 1 << GT_CTF;
			continue;
		}

		if (Q_stricmp(token, "SyC_ffa") == 0) {
			bits |= 1 << GT_SPRAYFFA;
			continue;
		}

		if (Q_stricmp(token, "SyC_tp") == 0) {
			bits |= 1 << GT_SPRAY;
			continue;
		}

		if (Q_stricmp(token, GAMETYPE_NAME_SHORT(GT_LPS)) == 0) {
			bits |= 1 << GT_LPS;
			continue;
		}

		if (Q_stricmp(token, GAMETYPE_NAME_SHORT(GT_BALLOON)) == 0) {
			bits |= 1 << GT_BALLOON;
			continue;
		}
	}

	return bits;
}

/*
=================
UI_StartServer_Update
=================
*/
static void UI_StartServer_Update(void) {
	int i;
	int top;
	static char picname[MAX_MAPSPERPAGE][64];
	static char focuspicnames[MAX_MAPSPERPAGE][64];

	top = s_startserver.page * MAX_MAPSPERPAGE;

	for (i = 0; i < MAX_MAPSPERPAGE; i++) {
		if (top + i >= s_startserver.nummaps)
			break;

		// try to load special levelshot first, fallback to default
		Com_sprintf(picname[i], sizeof(picname[i]), "levelshots/%sA", s_startserver.maplist[top + i]);
		if (!trap_R_RegisterShaderNoMip(picname[i])) {
			Com_sprintf(picname[i], sizeof(picname[i]), "levelshots/%s", s_startserver.maplist[top + i]);
		}
		Com_sprintf(focuspicnames[i], sizeof(focuspicnames[i]), "levelshots/%sB", s_startserver.maplist[top + i]);

		s_startserver.mappics[i].generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		s_startserver.mappics[i].generic.name = picname[i];
		s_startserver.mappics[i].shader = 0;

		s_startserver.mappics[i].focuspic = focuspicnames[i];
		s_startserver.mappics[i].focusshader = 0;
	}

	for (; i < MAX_MAPSPERPAGE; i++) {
		s_startserver.mappics[i].generic.flags |= (QMF_INACTIVE | QMF_HIDDEN);
		s_startserver.mappics[i].generic.name = NULL;
		s_startserver.mappics[i].shader = 0;
	}

	if (s_startserver.maxpages > 1) {
		s_startserver.arrowleft.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		s_startserver.arrowright.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		s_startserver.arrowleft.generic.flags |= QMF_GRAYED;
		s_startserver.arrowright.generic.flags |= QMF_GRAYED;

		if (s_startserver.page > 0) {
			s_startserver.arrowleft.generic.flags &= ~QMF_GRAYED;
		}

		if (s_startserver.page < (s_startserver.maxpages - 1)) {
			s_startserver.arrowright.generic.flags &= ~QMF_GRAYED;
		}
	} else {
		s_startserver.arrowleft.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;
		s_startserver.arrowright.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;
	}

	if (s_startserver.maploop[0] != -1) {
		s_startserver.fight.generic.flags &= ~QMF_GRAYED;
	} else {
		s_startserver.fight.generic.flags |= QMF_GRAYED;
	}
}

/*
=================
UI_StartServer_MapEvent
=================
*/
static void UI_StartServer_MapEvent(void *ptr, int event) {
	int i, tmpi, currentmap;

	currentmap = (s_startserver.page * MAX_MAPSPERPAGE) + (((menucommon_s *)ptr)->id - ID_PICTURES);

	if (event == QM_GOTFOCUS) {
		s_startserver.currentmap = currentmap;
		return;
	}

	if (event != QM_ACTIVATED) {
		return;
	}

	for (i = 0; i < MAX_MAPSINLOOP; i++) {
		if (s_startserver.maploop[i] == currentmap) {
			s_startserver.maploop[i] = -1;
			for (; i < MAX_MAPSINLOOP; i++) {
				if (s_startserver.maploop[i + 1] == -1)
					break;

				tmpi = s_startserver.maploop[i + 1];
				s_startserver.maploop[i + 1] = s_startserver.maploop[i];
				s_startserver.maploop[i] = tmpi;
			}
			break;
		}

		if (s_startserver.maploop[i] == -1) {
			s_startserver.maploop[i] = currentmap;
			break;
		}
	}

	UI_StartServer_Update();
}

/*
=================
UI_StartServer_GametypeEvent
=================
*/
static void UI_StartServer_GametypeEvent(void *ptr, int event) {
	int i, j;
	int count;
	int gamebits;
	int matchbits;
	const char *info;
	char tmpmaplist[MAX_MAPSINLOOP][MAX_NAMELENGTH];

	if (event != QM_ACTIVATED) {
		return;
	}

	memset(tmpmaplist, 0, sizeof(tmpmaplist));
	for (i = 0; i < MAX_MAPSINLOOP; i++) {
		if (s_startserver.maploop[i] < 0)
			break;

		Q_strncpyz(tmpmaplist[i], s_startserver.maplist[s_startserver.maploop[i]], sizeof(tmpmaplist[i]));
	}

	trap_Cvar_Set("g_gametype", va("%i", gametype_remap[s_startserver.gametype.curvalue]));
	trap_Cvar_Set("ui_createGametype", va("%i", gametype_remap[s_startserver.gametype.curvalue]));

	if (gametype_remap[s_startserver.gametype.curvalue] >= GT_TEAM)
		s_startserver.friendlyfire.generic.flags &= ~QMF_GRAYED;
	else
		s_startserver.friendlyfire.generic.flags |= QMF_GRAYED;

	if (gametype_remap[s_startserver.gametype.curvalue] == GT_LPS)
		s_startserver.startlives.generic.flags &= ~QMF_GRAYED;
	else
		s_startserver.startlives.generic.flags |= QMF_GRAYED;

	count = UI_GetNumArenas();
	s_startserver.nummaps = 0;
	matchbits = 1 << gametype_remap[s_startserver.gametype.curvalue];

	if (gametype_remap[s_startserver.gametype.curvalue] == GT_FFA) {
		matchbits |= (1 << GT_SINGLE_PLAYER);
	}

	for (i = 0; i < count; i++) {
		info = UI_GetArenaInfoByNumber(i);

		gamebits = UI_StartServer_GametypeBits(Info_ValueForKey(info, "type"));
		if (!(gamebits & matchbits)) {
			continue;
		}

		Q_strncpyz(s_startserver.maplist[s_startserver.nummaps], Info_ValueForKey(info, "map"), MAX_NAMELENGTH);
		Q_strncpyz(s_startserver.longnamemaplist[s_startserver.nummaps], Info_ValueForKey(info, "longname"),
				   MAX_LONGNAMELENGTH);
		s_startserver.mapGamebits[s_startserver.nummaps] = gamebits;
		++s_startserver.nummaps;
		if (s_startserver.nummaps >= MAX_SERVERMAPS)
			break;
	}

	s_startserver.maxpages = (s_startserver.nummaps + MAX_MAPSPERPAGE - 1) / MAX_MAPSPERPAGE;
	s_startserver.page = 0;
	s_startserver.currentmap = -1;

	for (i = 0; i < MAX_MAPSINLOOP; i++)
		s_startserver.maploop[i] = -1;

	for (i = 0, j = 0; i < MAX_MAPSINLOOP; ++i) {
		int k;

		if (tmpmaplist[i][0] == '\0')
			break;

		for (k = 0; k < s_startserver.nummaps; ++k) {
			if (s_startserver.maplist[k][0] == '\0')
				continue;

			if (!Q_stricmp(tmpmaplist[i], s_startserver.maplist[k])) {
				if (j >= MAX_MAPSINLOOP)
					goto DOUBLEBRAKE;
				s_startserver.maploop[j++] = k;
				break;
			}
		}
	}
DOUBLEBRAKE:

	UI_StartServer_Update();
}

/*
=================
UI_StartServer_MenuEvent
=================
*/
static void UI_StartServer_MenuEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_PREVPAGE:
		if (s_startserver.page > 0) {
			s_startserver.page--;
			UI_StartServer_Update();
		}
		break;

	case ID_NEXTPAGE:
		if (s_startserver.page < s_startserver.maxpages - 1) {
			s_startserver.page++;
			UI_StartServer_Update();
		}
		break;

	case ID_SELECTBOTS:
		UI_SelectBotsMenu();
		break;

	case ID_STARTSERVER:
		trap_Cvar_SetValue("g_gameType", gametype_remap[s_startserver.gametype.curvalue]);

		{
			int timelimit;
			int fraglimit;
			int lifelimit;
			int maxclients;
			int dedicated;
			int gametype;
			int friendlyfire;
			int instagib;
			int pure;
			int n;
			char buf[64];

			if (s_startserver.maploop[0] == -1) {
				Com_Printf("You must select a map to start a server!\n");
				break;
			}

			timelimit = atoi(s_startserver.timelimit.field.buffer);
			fraglimit = atoi(s_startserver.fraglimit.field.buffer);
			lifelimit = atoi(s_startserver.startlives.field.buffer);
			maxclients = atoi(s_startserver.maxclients.field.buffer);
			dedicated = s_startserver.dedicated.curvalue;
			friendlyfire = s_startserver.friendlyfire.curvalue;
			instagib = s_startserver.instagib.curvalue;
			pure = s_startserver.pure.curvalue;
			gametype = gametype_remap[s_startserver.gametype.curvalue];

			switch (gametype) {
			case GT_FFA:
			default:
				trap_Cvar_SetValue("ui_ffa_fraglimit", fraglimit);
				trap_Cvar_SetValue("ui_ffa_timelimit", timelimit);
				break;

			case GT_TOURNAMENT:
				trap_Cvar_SetValue("ui_tourney_fraglimit", fraglimit);
				trap_Cvar_SetValue("ui_tourney_timelimit", timelimit);
				break;

			case GT_TEAM:
				trap_Cvar_SetValue("ui_team_fraglimit", fraglimit);
				trap_Cvar_SetValue("ui_team_timelimit", timelimit);
				trap_Cvar_SetValue("ui_team_friendlt", friendlyfire);
				break;

			case GT_CTF:
				trap_Cvar_SetValue("ui_ctf_fraglimit", fraglimit);
				trap_Cvar_SetValue("ui_ctf_timelimit", timelimit);
				trap_Cvar_SetValue("ui_ctf_friendlt", friendlyfire);
				break;

			case GT_LPS:
				trap_Cvar_SetValue("ui_lps_startlives", lifelimit);
				break;
			}

			trap_Cvar_SetValue("sv_maxclients", Com_Clamp(1, MAX_CLIENTS, maxclients));
			trap_Cvar_SetValue("dedicated", Com_Clamp(0, 2, dedicated));
			trap_Cvar_SetValue("timelimit", Com_Clamp(0, timelimit, timelimit));
			trap_Cvar_SetValue("pointlimit", Com_Clamp(0, fraglimit, fraglimit));
			trap_Cvar_SetValue("g_LPS_startlives", Com_Clamp(1, lifelimit, lifelimit));
			trap_Cvar_SetValue("g_friendlyfire", friendlyfire);
			trap_Cvar_SetValue("g_instaPad", instagib);
			trap_Cvar_SetValue("sv_pure", pure);
			trap_Cvar_Set("sv_hostname", s_startserver.hostname.field.buffer);

			for (n = 0; n < MAX_MAPSINLOOP; n++) {
				char buffer[16];

				if (s_startserver.maploop[n] == -1)
					break;

				Com_sprintf(buffer, 16, "automaploop%i", n);

				trap_Cvar_Set(buffer, va("map %s; set nextmap vstr automaploop%i",
										 s_startserver.maplist[s_startserver.maploop[n]],
										 s_startserver.maploop[n + 1] == -1 ? 0 : n + 1));
			}

			// the wait commands will allow the dedicated to take effect
			trap_Cmd_ExecuteText(EXEC_APPEND, "wait ; wait ; vstr automaploop0\n");

			// add bots
			trap_Cmd_ExecuteText(EXEC_APPEND, "wait 3\n");
			for (n = 0; n < MAX_SELECTBOTS && s_startserver.selectbotinfos[n].name[0] != '\0'; n++) {
				Com_sprintf(buf, sizeof(buf), "addbot %s %i %s\n", s_startserver.selectbotinfos[n].name,
							s_startserver.botskill, teamstrs[s_startserver.selectbotinfos[n].team]);
				trap_Cmd_ExecuteText(EXEC_APPEND, buf);
			}
		}
		break;

	case ID_STARTSERVERBACK:
		UI_PopMenu();
		break;
	}
}

/*
===============
UI_StartServer_LevelshotDraw
===============
*/
static void UI_StartServer_LevelshotDraw(void *self) {
	menubitmap_s *b;
	int x;
	int y;
	int w;
	int h;
	int n;
	int i;

	b = (menubitmap_s *)self;

	if (!b->generic.name) {
		return;
	}

	if (b->generic.name && !b->shader) {
		b->shader = trap_R_RegisterShaderNoMip(b->generic.name);
		if (!b->shader && b->errorpic) {
			b->shader = trap_R_RegisterShaderNoMip(b->errorpic);
		}
	}

	if (b->focuspic && !b->focusshader) {
		b->focusshader = trap_R_RegisterShaderNoMip(b->focuspic);
	}

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h = b->height;
	if (Menu_ItemAtCursor(b->generic.parent) == b && b->focusshader) {
		UI_DrawHandlePic(x, y, w, h, b->focusshader);
	} else if (b->shader) {
		UI_DrawHandlePic(x, y, w, h, b->shader);
	}

	n = s_startserver.page * MAX_MAPSPERPAGE + b->generic.id - ID_PICTURES;

	for (i = 0; i < MAX_MAPSINLOOP; i++)
		if (s_startserver.maploop[i] == n) {
			if (i + 1 > 9) {
				UI_DrawHandlePic(x + 8, y + 6, 20, 35, s_startserver.mapNumbers[((i + 1) / 10) % 10]);
				UI_DrawHandlePic(x + 8 + 23, y + 6, 20, 35, s_startserver.mapNumbers[(i + 1) % 10]);
			} else
				UI_DrawHandlePic(x + 8, y + 6, 20, 35, s_startserver.mapNumbers[(i + 1) % 10]);

			break;
		}

	x = b->generic.x;
	y = b->generic.y + b->height;

	x += b->width / 2;
	y += 4;
	if (Menu_ItemAtCursor(b->generic.parent) == b) {
		// Draw mapname in UPPERCASE, maplist remains untouched so "map mapname" still works
		// TODO: Only do strupr once and save into list while initialising
		char mapUpper[MAX_NAMELENGTH];
		Q_strncpyz(mapUpper, s_startserver.maplist[n], sizeof(mapUpper));
		Q_strupr(mapUpper);

		UI_DrawStringNS(x, y, mapUpper, UI_CENTER, 14.0f, color_white);
	} else
		UI_DrawStringNS(x, y, s_startserver.longnamemaplist[n], UI_CENTER, 14.0f, color_white);
}

/*
=================
UI_StartServer_MenuDraw
=================
*/
static void UI_StartServer_MenuDraw(void) {
	// standard menu drawing
	Menu_Draw(&s_startserver.menu);
}

/*
=================
UI_StartServer_GameTypeDraw
=================
*/
static void UI_StartServer_GameTypeDraw(void *voidptr) {
	const float *color;
	int x, y;
	int style;
	qboolean focus;
	menulist_s *s = voidptr;

	x = s->generic.x;
	y = s->generic.y;

	style = UI_SMALLFONT;
	focus = (s->generic.parent->cursor == s->generic.menuPosition);

	color = color_black;

	if (focus)
		UI_FillRect(s->generic.left, s->generic.top, ((s->generic.right - s->generic.left + 1) * 15) / 16,
					s->generic.bottom - s->generic.top + 1, listbar_color);

	UI_DrawStringNS(x, y, s->itemnames[s->curvalue], style, 15.0f, color);
}

/*
=================
UI_StartServer_MenuInit
=================
*/

static const char *dedicated_list[] = {"No", "LAN", "Internet", NULL};

static void UI_StartServer_MenuInit(void) {
	int i;
	int x;
	int y;

	// zero set all our globals
	memset(&s_startserver, 0, sizeof(startserver_t));

	for (i = 0; i < MAX_MAPSINLOOP; i++)
		s_startserver.maploop[i] = -1;

	UI_StartServer_Cache();

	s_startserver.menu.wrapAround = qtrue;
	s_startserver.menu.fullscreen = qtrue;
	s_startserver.menu.bgparts = BGP_STARTSERVER | BGP_MENUFX;
	s_startserver.menu.draw = UI_StartServer_MenuDraw;

	s_startserver.gametype.generic.type = MTYPE_SPINCONTROL;
	s_startserver.gametype.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT | QMF_FORCEDROPDOWN;
	s_startserver.gametype.generic.callback = UI_StartServer_GametypeEvent;
	s_startserver.gametype.generic.id = ID_GAMETYPE;
	s_startserver.gametype.generic.x = 500;
	s_startserver.gametype.generic.y = 430;
	s_startserver.gametype.itemnames = gametype_items;
	s_startserver.gametype.generic.ownerdraw = UI_StartServer_GameTypeDraw;
	s_startserver.gametype.curvalue = gametype_remap2[(int)Com_Clamp(
		0, (sizeof(gametype_remap2) / sizeof(gametype_remap[0]) - 1), trap_Cvar_VariableValue("ui_createGametype"))];

	for (i = 0; i < MAX_MAPSPERPAGE; i++) {
		x = (i % MAX_MAPCOLS) * (128 + 24) + GRID_XPOS;
		y = (i / MAX_MAPCOLS) * (96 + 23) + GRID_YPOS;

		s_startserver.mappics[i].generic.type = MTYPE_BITMAP;
		s_startserver.mappics[i].generic.flags = QMF_LEFT_JUSTIFY;
		s_startserver.mappics[i].generic.id = ID_PICTURES + i;
		s_startserver.mappics[i].generic.callback = UI_StartServer_MapEvent;
		s_startserver.mappics[i].generic.x = x;
		s_startserver.mappics[i].generic.y = y;
		s_startserver.mappics[i].width = 128;
		s_startserver.mappics[i].height = 96;
		s_startserver.mappics[i].errorpic = UNKNOWNMAP;
		s_startserver.mappics[i].generic.ownerdraw = UI_StartServer_LevelshotDraw;
	}

	s_startserver.arrowleft.generic.type = MTYPE_BITMAP;
	s_startserver.arrowleft.generic.name = ARROWLT0;
	s_startserver.arrowleft.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_startserver.arrowleft.generic.x = 214;
	s_startserver.arrowleft.generic.y = 396;
	s_startserver.arrowleft.generic.id = ID_PREVPAGE;
	s_startserver.arrowleft.generic.callback = UI_StartServer_MenuEvent;
	s_startserver.arrowleft.width = 60;
	s_startserver.arrowleft.height = 25;
	s_startserver.arrowleft.focuspic = ARROWLT1;
	s_startserver.arrowleft.focuspicinstead = qtrue;

	s_startserver.arrowright.generic.type = MTYPE_BITMAP;
	s_startserver.arrowright.generic.name = ARROWRT0;
	s_startserver.arrowright.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_startserver.arrowright.generic.x = 290;
	s_startserver.arrowright.generic.y = 396;
	s_startserver.arrowright.generic.id = ID_NEXTPAGE;
	s_startserver.arrowright.generic.callback = UI_StartServer_MenuEvent;
	s_startserver.arrowright.width = 60;
	s_startserver.arrowright.height = 25;
	s_startserver.arrowright.focuspic = ARROWRT1;
	s_startserver.arrowright.focuspicinstead = qtrue;

	s_startserver.back.generic.type = MTYPE_BITMAP;
	s_startserver.back.generic.name = BACK0;
	s_startserver.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_startserver.back.generic.callback = UI_StartServer_MenuEvent;
	s_startserver.back.generic.id = ID_STARTSERVERBACK;
	s_startserver.back.focuspic = BACK1;
	s_startserver.back.generic.x = 8;
	s_startserver.back.generic.y = 446;
	s_startserver.back.width = 80;
	s_startserver.back.height = 40;
	s_startserver.back.focuspicinstead = qtrue;

	y = 176;
	s_startserver.hostname.generic.type = MTYPE_FIELD;
	s_startserver.hostname.generic.name = "Host Name:";
	s_startserver.hostname.generic.flags = QMF_SMALLFONT;
	s_startserver.hostname.generic.x = OPTIONS_XPOS;
	s_startserver.hostname.generic.y = y;
	s_startserver.hostname.field.widthInChars = 12;
	s_startserver.hostname.field.maxchars = 64;

	y += BIGCHAR_HEIGHT + 2;
	s_startserver.friendlyfire.generic.type = MTYPE_RADIOBUTTON;
	s_startserver.friendlyfire.generic.flags = QMF_SMALLFONT | QMF_GRAYED;
	s_startserver.friendlyfire.generic.x = OPTIONS_XPOS;
	s_startserver.friendlyfire.generic.y = y;
	s_startserver.friendlyfire.generic.name = "Friendly Fire:";

	y += BIGCHAR_HEIGHT + 2;
	s_startserver.instagib.generic.type = MTYPE_RADIOBUTTON;
	s_startserver.instagib.generic.flags = QMF_SMALLFONT;
	s_startserver.instagib.generic.x = OPTIONS_XPOS;
	s_startserver.instagib.generic.y = y;
	s_startserver.instagib.generic.name = "InstaPad:";

	y += BIGCHAR_HEIGHT + 2;
	s_startserver.pure.generic.type = MTYPE_RADIOBUTTON;
	s_startserver.pure.generic.flags = QMF_SMALLFONT;
	s_startserver.pure.generic.x = OPTIONS_XPOS;
	s_startserver.pure.generic.y = y;
	s_startserver.pure.generic.name = "Pure Server:";
	s_startserver.pure.curvalue = Com_Clamp(0, 1, trap_Cvar_VariableValue("sv_pure"));

	y += BIGCHAR_HEIGHT + 2;
	s_startserver.dedicated.generic.type = MTYPE_SPINCONTROL;
	//	s_startserver.dedicated.generic.id			= ID_DEDICATED;
	s_startserver.dedicated.generic.flags = QMF_SMALLFONT;
	//	s_startserver.dedicated.generic.callback	= ServerOptions_Event;
	s_startserver.dedicated.generic.x = OPTIONS_XPOS;
	s_startserver.dedicated.generic.y = y;
	s_startserver.dedicated.generic.name = "Dedicated:";
	s_startserver.dedicated.itemnames = dedicated_list;

	y += BIGCHAR_HEIGHT + 2;
	s_startserver.fraglimit.generic.type = MTYPE_FIELD;
	s_startserver.fraglimit.generic.name = "Point Limit:";
	s_startserver.fraglimit.generic.flags = QMF_NUMBERSONLY | QMF_SMALLFONT;
	s_startserver.fraglimit.generic.x = OPTIONS_XPOS;
	s_startserver.fraglimit.generic.y = y;
	s_startserver.fraglimit.field.widthInChars = 3;
	s_startserver.fraglimit.field.maxchars = 3;

	y += BIGCHAR_HEIGHT + 2;
	s_startserver.timelimit.generic.type = MTYPE_FIELD;
	s_startserver.timelimit.generic.name = "Time Limit:";
	s_startserver.timelimit.generic.flags = QMF_NUMBERSONLY | QMF_SMALLFONT;
	s_startserver.timelimit.generic.x = OPTIONS_XPOS;
	s_startserver.timelimit.generic.y = y;
	s_startserver.timelimit.field.widthInChars = 3;
	s_startserver.timelimit.field.maxchars = 3;

	y += BIGCHAR_HEIGHT + 2;
	s_startserver.maxclients.generic.type = MTYPE_FIELD;
	s_startserver.maxclients.generic.name = "Player Slots:";
	s_startserver.maxclients.generic.flags = QMF_NUMBERSONLY | QMF_SMALLFONT;
	s_startserver.maxclients.generic.x = OPTIONS_XPOS;
	s_startserver.maxclients.generic.y = y;
	s_startserver.maxclients.field.widthInChars = 2;
	s_startserver.maxclients.field.maxchars = 2;

	y += BIGCHAR_HEIGHT + 2;
	s_startserver.startlives.generic.type = MTYPE_FIELD;
	s_startserver.startlives.generic.name = "Start Lives:";
	s_startserver.startlives.generic.flags = QMF_NUMBERSONLY | QMF_SMALLFONT;
	s_startserver.startlives.generic.x = OPTIONS_XPOS;
	s_startserver.startlives.generic.y = y;
	s_startserver.startlives.field.widthInChars = 3;
	s_startserver.startlives.field.maxchars = 3;

	y += BIGCHAR_HEIGHT + 4;
	s_startserver.selectbots.generic.type = MTYPE_BITMAP;
	s_startserver.selectbots.generic.name = SELECTBOTS0;
	s_startserver.selectbots.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_startserver.selectbots.generic.callback = UI_StartServer_MenuEvent;
	s_startserver.selectbots.generic.id = ID_SELECTBOTS;
	s_startserver.selectbots.focuspic = SELECTBOTS1;
	s_startserver.selectbots.generic.x = OPTIONS_XPOS - 70;
	s_startserver.selectbots.generic.y = y;
	s_startserver.selectbots.width = 140;
	s_startserver.selectbots.height = 24;
	s_startserver.selectbots.focuspicinstead = qtrue;

	s_startserver.fight.generic.type = MTYPE_BITMAP;
	s_startserver.fight.generic.name = FIGHT0;
	s_startserver.fight.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	s_startserver.fight.generic.callback = UI_StartServer_MenuEvent;
	s_startserver.fight.generic.id = ID_STARTSERVER;
	s_startserver.fight.focuspic = FIGHT1;
	s_startserver.fight.generic.x = 776;
	s_startserver.fight.generic.y = 420;
	s_startserver.fight.width = 80;
	s_startserver.fight.height = 60;
	s_startserver.fight.focuspicinstead = qtrue;

	s_startserver.item_null.generic.type = MTYPE_BITMAP;
	s_startserver.item_null.generic.flags = QMF_LEFT_JUSTIFY | QMF_MOUSEONLY | QMF_SILENT;
	s_startserver.item_null.generic.x = 0;
	s_startserver.item_null.generic.y = 0;
	s_startserver.item_null.width = 864;
	s_startserver.item_null.height = 486;

	Menu_AddItem(&s_startserver.menu, &s_startserver.item_null);
	Menu_AddItem(&s_startserver.menu, &s_startserver.gametype);
	s_startserver.gametype.generic.right -= SMALLCHAR_WIDTH;
	s_startserver.gametype.generic.left += SMALLCHAR_WIDTH;
	for (i = 0; i < MAX_MAPSPERPAGE; i++) {
		Menu_AddItem(&s_startserver.menu, &s_startserver.mappics[i]);
	}
	Menu_AddItem(&s_startserver.menu, &s_startserver.arrowleft);
	Menu_AddItem(&s_startserver.menu, &s_startserver.arrowright);

	Menu_AddItem(&s_startserver.menu, &s_startserver.hostname);
	Menu_AddItem(&s_startserver.menu, &s_startserver.friendlyfire);
	Menu_AddItem(&s_startserver.menu, &s_startserver.instagib);
	Menu_AddItem(&s_startserver.menu, &s_startserver.pure);
	Menu_AddItem(&s_startserver.menu, &s_startserver.dedicated);
	Menu_AddItem(&s_startserver.menu, &s_startserver.fraglimit);
	Menu_AddItem(&s_startserver.menu, &s_startserver.timelimit);
	Menu_AddItem(&s_startserver.menu, &s_startserver.maxclients);
	Menu_AddItem(&s_startserver.menu, &s_startserver.startlives);
	Menu_AddItem(&s_startserver.menu, &s_startserver.selectbots);

	Menu_AddItem(&s_startserver.menu, &s_startserver.fight);
	Menu_AddItem(&s_startserver.menu, &s_startserver.back);

	Q_strncpyz(s_startserver.hostname.field.buffer, UI_Cvar_VariableString("sv_hostname"),
			   sizeof(s_startserver.hostname.field.buffer));
	Q_strncpyz(s_startserver.timelimit.field.buffer, UI_Cvar_VariableString("timelimit"),
			   sizeof(s_startserver.timelimit.field.buffer));
	Q_strncpyz(s_startserver.fraglimit.field.buffer, UI_Cvar_VariableString("pointlimit"),
			   sizeof(s_startserver.fraglimit.field.buffer));
	Q_strncpyz(s_startserver.maxclients.field.buffer, UI_Cvar_VariableString("sv_maxclients"),
			   sizeof(s_startserver.maxclients.field.buffer));
	Q_strncpyz(s_startserver.startlives.field.buffer, UI_Cvar_VariableString("ui_lps_startlives"),
			   sizeof(s_startserver.startlives.field.buffer));

	UI_StartServer_GametypeEvent(NULL, QM_ACTIVATED);
}

/*
=================
UI_StartServer_Cache
=================
*/
void UI_StartServer_Cache(void) {
	int i;
	const char *info;
	char picname[MAX_QPATH];

	trap_R_RegisterShaderNoMip(FIGHT0);
	trap_R_RegisterShaderNoMip(FIGHT1);
	trap_R_RegisterShaderNoMip(ARROWLT0);
	trap_R_RegisterShaderNoMip(ARROWLT1);
	trap_R_RegisterShaderNoMip(ARROWRT0);
	trap_R_RegisterShaderNoMip(ARROWRT1);
	trap_R_RegisterShaderNoMip(SELECTBOTS0);
	trap_R_RegisterShaderNoMip(SELECTBOTS1);
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
	trap_R_RegisterShaderNoMip(UNKNOWNMAP);

	for (i = 0; i < 10; i++)
		s_startserver.mapNumbers[i] = trap_R_RegisterShaderNoMip(va("menu/numbers/%i", i));

	s_startserver.nummaps = UI_GetNumArenas();

	for (i = 0; i < s_startserver.nummaps; i++) {
		info = UI_GetArenaInfoByNumber(i);

		Q_strncpyz(s_startserver.maplist[i], Info_ValueForKey(info, "map"), sizeof(s_startserver.maplist[i]));
		s_startserver.mapGamebits[i] = UI_StartServer_GametypeBits(Info_ValueForKey(info, "type"));

		Com_sprintf(picname, sizeof(picname), "levelshots/%sA", s_startserver.maplist[i]);
		if (!trap_R_RegisterShaderNoMip(picname)) {
			Com_sprintf(picname, sizeof(picname), "levelshots/%s", s_startserver.maplist[i]);
			trap_R_RegisterShaderNoMip(picname);
		}
		Com_sprintf(picname, sizeof(picname), "levelshots/%sB", s_startserver.maplist[i]);
		trap_R_RegisterShaderNoMip(picname);
	}
	s_startserver.maxpages = ((s_startserver.nummaps + MAX_MAPSPERPAGE - 1) / MAX_MAPSPERPAGE);
}

/*
=================
UI_StartServerMenu
=================
*/
void UI_StartServerMenu(qboolean multiplayer) {
	UI_StartServer_MenuInit();
	s_startserver.multiplayer = multiplayer;
	UI_PushMenu(&s_startserver.menu);
}

/*
=======================================================================

SELECT BOTS MENU

=======================================================================
*/

#define ARROWUP0 "menu/arrows/headyel_up0"
#define ARROWUP1 "menu/arrows/headyel_up1"
#define ARROWDN0 "menu/arrows/headyel_dn0"
#define ARROWDN1 "menu/arrows/headyel_dn1"
#define ICONSHADOW "menu/art/iconshadow"
#define HEADERBOTS "menu/headers/bots"
#define HEADERCOLOR "menu/headers/color"

#define BOTGRID_COLS 5
#define BOTGRID_ROWS 4
#define MAX_BOTSPERPAGE (BOTGRID_ROWS * BOTGRID_COLS)

#define MAX_SELECTLISTBOTS 12

typedef struct {
	menuframework_s menu;

	menutext_s banner;

	menubitmap_s pics[MAX_BOTSPERPAGE];
	menubitmap_s arrowleft;
	menubitmap_s arrowright;

	menubitmap_s headerbots;
	menubitmap_s headercolor;

	menutext_s selectedbotnames[MAX_SELECTLISTBOTS];
	menutext_s selectedbotteams[MAX_SELECTLISTBOTS];

	menubitmap_s arrowup;
	menubitmap_s arrowdown;

	menulist_s BotSkill;
	menutext_s slotsleft;

	menubitmap_s back;

	int topbot; // firstbot in list ;)
	int numBots;
	int modelpage;
	int numpages;
	int selectedmodel;
	int sortedBotNums[MAX_BOTS];
	char boticons[MAX_BOTSPERPAGE][MAX_QPATH];
	char botnames[MAX_BOTSPERPAGE][16];
} botSelectInfo_t;

static botSelectInfo_t botSelectInfo;

static const char *botSkill_list[] = {"Kindergarten", "Flower Power", "Mosquito Bite",
									  "Peacemaker", "Brutal Blue Noses", 0};

/*
=================
UI_SelectBots_BuildList
=================
*/
static void UI_SelectBots_BuildList(void) {
	int n;

	botSelectInfo.modelpage = 0;
	botSelectInfo.numBots = UI_GetNumBots();
	botSelectInfo.numpages = botSelectInfo.numBots / MAX_BOTSPERPAGE;
	if (botSelectInfo.numBots % MAX_BOTSPERPAGE) {
		botSelectInfo.numpages++;
	}

	// initialize the array
	for (n = 0; n < botSelectInfo.numBots; n++) {
		botSelectInfo.sortedBotNums[n] = n;
	}

	// now sort it
	qsort(botSelectInfo.sortedBotNums, botSelectInfo.numBots, sizeof(botSelectInfo.sortedBotNums[0]), BotListCompare);
}

/*
=================
UI_SelectBots_ServerPlayerIcon
=================
*/
static void UI_SelectBots_ServerPlayerIcon(const char *modelAndSkin, char *iconName, int iconNameMaxSize) {
	char *skin;
	char model[MAX_QPATH];

	Q_strncpyz(model, modelAndSkin, sizeof(model));
	skin = strrchr(model, '/');
	if (skin) {
		*skin++ = '\0';
	} else {
		skin = "default";
	}

	Com_sprintf(iconName, iconNameMaxSize, "models/wop_players/%s/icon_%s", model, skin);

	if (!trap_R_RegisterShaderNoMip(iconName) && Q_stricmp(skin, "default") != 0) {
		Com_sprintf(iconName, iconNameMaxSize, "models/wop_players/%s/icon_default", model);
	}
}

/*
=================
UI_SelectBots_UpdateGrid
=================
*/
static void UI_SelectBots_UpdateGrid(void) {
	const char *info;
	int i;
	int j;

	j = botSelectInfo.modelpage * MAX_BOTSPERPAGE;
	for (i = 0; i < (BOTGRID_ROWS * BOTGRID_COLS); i++, j++) {
		if (j < botSelectInfo.numBots) {
			info = UI_GetBotInfoByNumber(botSelectInfo.sortedBotNums[j]);
			UI_SelectBots_ServerPlayerIcon(Info_ValueForKey(info, "model"), botSelectInfo.boticons[i], MAX_QPATH);
			Q_strncpyz(botSelectInfo.botnames[i], Info_ValueForKey(info, "name"), 16);
			Q_CleanStr(botSelectInfo.botnames[i]);
			botSelectInfo.pics[i].generic.name = botSelectInfo.boticons[i];
		} else {
			// dead slot
			botSelectInfo.pics[i].generic.name = NULL;
			botSelectInfo.botnames[i][0] = 0;
		}
		botSelectInfo.pics[i].shader = 0;
	}

	// set selected model
	i = botSelectInfo.selectedmodel % MAX_BOTSPERPAGE;

	if (botSelectInfo.numpages > 1) {
		botSelectInfo.arrowleft.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		botSelectInfo.arrowright.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
		botSelectInfo.arrowleft.generic.flags |= QMF_GRAYED;
		botSelectInfo.arrowright.generic.flags |= QMF_GRAYED;

		if (botSelectInfo.modelpage > 0) {
			botSelectInfo.arrowleft.generic.flags &= ~QMF_GRAYED;
		}

		if (botSelectInfo.modelpage < (botSelectInfo.numpages - 1)) {
			botSelectInfo.arrowright.generic.flags &= ~QMF_GRAYED;
		}
	} else {
		botSelectInfo.arrowleft.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;
		botSelectInfo.arrowright.generic.flags |= QMF_INACTIVE | QMF_HIDDEN;
	}
}

/*
=================
UI_SelectBots_LeftEvent
=================
*/
static void UI_SelectBots_LeftEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}
	if (botSelectInfo.modelpage > 0) {
		botSelectInfo.modelpage--;
		botSelectInfo.selectedmodel = botSelectInfo.modelpage * MAX_BOTSPERPAGE;
		UI_SelectBots_UpdateGrid();
	}
}

/*
=================
UI_SelectBots_RightEvent
=================
*/
static void UI_SelectBots_RightEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}
	if (botSelectInfo.modelpage < botSelectInfo.numpages - 1) {
		botSelectInfo.modelpage++;
		botSelectInfo.selectedmodel = botSelectInfo.modelpage * MAX_BOTSPERPAGE;
		UI_SelectBots_UpdateGrid();
	}
}

/*
=================
UI_SelectBots_GetNumSelectedBots
=================
*/
static int UI_SelectBots_GetNumSelectedBots(void) {
	int i, numBots = 0;

	for (i = 0; i < MAX_SELECTBOTS; i++) {
		if (s_startserver.selectbotinfos[i].name[0] != '\0') {
			numBots++;
		}
	}
	return numBots;
}

/*
=================
UI_SelectBots_GetSlotsLeft
=================
*/
static int UI_SelectBots_GetSlotsLeft(void) {
	int maxclients = atoi(s_startserver.maxclients.field.buffer);

	if (s_startserver.dedicated.curvalue == 0) {
		// running locally
		return (maxclients - UI_SelectBots_GetNumSelectedBots() - 1);
	} else {
		return (maxclients - UI_SelectBots_GetNumSelectedBots());
	}
}

/*
=================
UI_SelectBots_UpdateList
=================
*/
static void UI_SelectBots_UpdateList(void) {
	int i, j;
	char *tmpptr;

	j = 0;
	for (i = 0; i < MAX_SELECTLISTBOTS; i++) {
		tmpptr = "";
		for (; j < MAX_SELECTBOTS; j++) {
			if (j < botSelectInfo.topbot)
				continue;

			if (s_startserver.selectbotinfos[j].name[0] != '\0') {
				tmpptr = s_startserver.selectbotinfos[j].name;
				j++; // damit es +1 ist wenn die äußere schleife looped
				break;
			}
		}

		if (*tmpptr != '\0') {
			botSelectInfo.selectedbotnames[i].string = tmpptr;
			botSelectInfo.selectedbotnames[i].generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
			if (gametype_remap[s_startserver.gametype.curvalue] >= GT_TEAM) {
				// this happens when switching gametype
				if (s_startserver.selectbotinfos[j - 1].team == TEAM_FREE) {
					s_startserver.selectbotinfos[j - 1].team = TEAM_RED;
				}

				botSelectInfo.selectedbotteams[i].string = (char *)teamstrs[s_startserver.selectbotinfos[j - 1].team];
				botSelectInfo.selectedbotteams[i].generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
				botSelectInfo.selectedbotteams[i].color =
					s_startserver.selectbotinfos[j - 1].team == TEAM_RED ? color_red : color_blue;
			} else {
				char *skinname;
				const float *skincolor; // FIXME: vec4_t

				switch (s_startserver.selectbotinfos[j - 1].team) {
				default:
				case TEAM_FREE:
					skinname = "Default";
					skincolor = color_white;
					break;
				case TEAM_RED:
					skinname = "Red";
					skincolor = color_red;
					break;
				case TEAM_BLUE:
					skinname = "Blue";
					skincolor = color_blue;
					break;
				}

				botSelectInfo.selectedbotteams[i].generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);

				botSelectInfo.selectedbotteams[i].string = skinname;
				botSelectInfo.selectedbotteams[i].color = skincolor;
			}
		} else {
			botSelectInfo.selectedbotnames[i].generic.flags |= (QMF_INACTIVE | QMF_HIDDEN);
			botSelectInfo.selectedbotteams[i].generic.flags |= (QMF_INACTIVE | QMF_HIDDEN);
		}
	}
	botSelectInfo.slotsleft.string = va("Open Slots: %2d", UI_SelectBots_GetSlotsLeft());
}

/*
=================
UI_SelectBots_BotEvent
=================
*/
static void UI_SelectBots_BotEvent(void *ptr, int event) {
	int i, j;

	if (event != QM_ACTIVATED) {
		return;
	}

	// set selected
	i = ((menucommon_s *)ptr)->id;
	botSelectInfo.selectedmodel = botSelectInfo.modelpage * MAX_BOTSPERPAGE + i;

	if (UI_SelectBots_GetSlotsLeft() <= 0) {
		return;
	}

	for (j = 0; j < MAX_SELECTBOTS; j++) {
		if (s_startserver.selectbotinfos[j].name[0] != '\0')
			continue;

		Q_strncpyz(s_startserver.selectbotinfos[j].name, botSelectInfo.botnames[i], MAX_BOTNAME);

		if (gametype_remap[s_startserver.gametype.curvalue] >= GT_TEAM) {
			s_startserver.selectbotinfos[j].team = TEAM_RED;
		} else {
			s_startserver.selectbotinfos[j].team = TEAM_FREE;
		}

		break;
	}
	UI_SelectBots_UpdateList();
}

/*
=================
UI_SelectBots_BackEvent
=================
*/
static void UI_SelectBots_BackEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}
	UI_PopMenu();
}

/*
=================
UI_SelectBots_Cache
=================
*/
void UI_SelectBots_Cache(void) {
	trap_R_RegisterShaderNoMip(HEADERBOTS);
	trap_R_RegisterShaderNoMip(HEADERCOLOR);
	trap_R_RegisterShaderNoMip(ARROWLT0);
	trap_R_RegisterShaderNoMip(ARROWLT1);
	trap_R_RegisterShaderNoMip(ARROWRT0);
	trap_R_RegisterShaderNoMip(ARROWRT1);
	trap_R_RegisterShaderNoMip(ARROWUP0);
	trap_R_RegisterShaderNoMip(ARROWUP1);
	trap_R_RegisterShaderNoMip(ARROWDN0);
	trap_R_RegisterShaderNoMip(ARROWDN1);
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
	trap_R_RegisterShaderNoMip(ICONSHADOW);
}

/*
=================
UI_SelectBots_SelectBotName
=================
*/
static void UI_SelectBots_SelectBotName(void *ptr, int event) {
	selectbotinfo_t tmpcpy;
	int i;

	if (event != QM_ACTIVATED)
		return;

	i = ((menucommon_s *)ptr)->id + botSelectInfo.topbot;
	Q_strncpyz(s_startserver.selectbotinfos[i].name, "", MAX_BOTNAME);
	for (; i < MAX_SELECTBOTS - 1; i++) {
		if (s_startserver.selectbotinfos[i + 1].name[0] == '\0')
			break;

		memcpy(&tmpcpy, &s_startserver.selectbotinfos[i], sizeof(selectbotinfo_t));
		memcpy(&s_startserver.selectbotinfos[i], &s_startserver.selectbotinfos[i + 1], sizeof(selectbotinfo_t));
		memcpy(&s_startserver.selectbotinfos[i + 1], &tmpcpy, sizeof(selectbotinfo_t));
	}
	UI_SelectBots_UpdateList();
}

/*
=================
UI_SelectBots_SelectBotTeam
=================
*/
static void UI_SelectBots_SelectBotTeam(void *ptr, int event) {
	int i;
	if (event != QM_ACTIVATED)
		return;

	i = ((menucommon_s *)ptr)->id + botSelectInfo.topbot;
	if (gametype_remap[s_startserver.gametype.curvalue] >= GT_TEAM) {
		s_startserver.selectbotinfos[i].team =
			(s_startserver.selectbotinfos[i].team == TEAM_RED ? TEAM_BLUE : TEAM_RED);
	} else {
		// NOTE: We're abusing the team field for the bot's skin
		int t = s_startserver.selectbotinfos[i].team;
		t++;
		if (t > TEAM_BLUE) {
			t = TEAM_FREE;
		}
		s_startserver.selectbotinfos[i].team = t;
	}

	UI_SelectBots_UpdateList();
}

/*
=================
UI_SelectBots_ListUp
=================
*/
static void UI_SelectBots_ListUp(void *ptr, int event) {
	if (event != QM_ACTIVATED)
		return;

	if (botSelectInfo.topbot > 0)
		botSelectInfo.topbot--;

	UI_SelectBots_UpdateList();
}

/*
=================
UI_SelectBots_ListDown
=================
*/
static void UI_SelectBots_ListDown(void *ptr, int event) {
	if (event != QM_ACTIVATED)
		return;

	if (botSelectInfo.topbot < MAX_SELECTBOTS - MAX_SELECTLISTBOTS)
		botSelectInfo.topbot++;

	UI_SelectBots_UpdateList();
}

/*
=================
UI_SelectBots_BotSkillChanged
=================
*/
static void UI_SelectBots_BotSkillChanged(void *ptr, int event) {
	if (event != QM_ACTIVATED)
		return;

	s_startserver.botskill = ((menulist_s *)ptr)->curvalue + 1;
}

/*
=================
UI_SelectBots_DrawBotIcon
=================
*/
static void UI_SelectBots_DrawBotIcon(void *self) {
	menubitmap_s *b = (menubitmap_s *)self;
	float x;
	float y;
	float w;
	float h;

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h = b->height;

	// used to refresh shader
	if (b->generic.name && !b->shader) {
		b->shader = trap_R_RegisterShaderNoMip(b->generic.name);
		if (!b->shader && b->errorpic)
			b->shader = trap_R_RegisterShaderNoMip(b->errorpic);
	}

	if (b->shader) // if there is no icon there should also be no shadow
	{
		if (!(Menu_ItemAtCursor(b->generic.parent) == b)) {
			UI_DrawNamedPic(x + 5, y + 5, w, h, ICONSHADOW);
		}
		UI_DrawHandlePic(x, y, w, h, b->shader);
	}
}

/*
=================
UI_SelectBots_MenuInit
=================
*/
static void UI_SelectBots_MenuInit(void) {
	int i, j, k;
	int x, y;

	memset(&botSelectInfo, 0, sizeof(botSelectInfo));
	botSelectInfo.menu.wrapAround = qtrue;
	botSelectInfo.menu.fullscreen = qtrue;
	botSelectInfo.menu.bgparts = BGP_SELECTBOTS | BGP_MENUFX;

	UI_SelectBots_Cache();

	y = GRID_YPOS;
	for (i = 0, k = 0; i < BOTGRID_ROWS; i++) {
		x = GRID_XPOS;
		for (j = 0; j < BOTGRID_COLS; j++, k++) {
			botSelectInfo.pics[k].generic.type = MTYPE_BITMAP;
			botSelectInfo.pics[k].generic.flags = QMF_LEFT_JUSTIFY;
			botSelectInfo.pics[k].generic.x = x;
			botSelectInfo.pics[k].generic.y = y;
			botSelectInfo.pics[k].generic.name = botSelectInfo.boticons[k];
			botSelectInfo.pics[k].width = 64;
			botSelectInfo.pics[k].height = 64;
			botSelectInfo.pics[k].generic.callback = UI_SelectBots_BotEvent;
			botSelectInfo.pics[k].generic.id = k;
			botSelectInfo.pics[k].generic.ownerdraw = UI_SelectBots_DrawBotIcon;
			x += (64 + 28);
		}
		y += (64 + 26);
	}

	botSelectInfo.arrowleft.generic.type = MTYPE_BITMAP;
	botSelectInfo.arrowleft.generic.name = ARROWLT0;
	botSelectInfo.arrowleft.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	botSelectInfo.arrowleft.generic.x = 214;
	botSelectInfo.arrowleft.generic.y = 396;
	botSelectInfo.arrowleft.generic.callback = UI_SelectBots_LeftEvent;
	botSelectInfo.arrowleft.width = 60;
	botSelectInfo.arrowleft.height = 25;
	botSelectInfo.arrowleft.focuspic = ARROWLT1;
	botSelectInfo.arrowleft.focuspicinstead = qtrue;

	botSelectInfo.arrowright.generic.type = MTYPE_BITMAP;
	botSelectInfo.arrowright.generic.name = ARROWRT0;
	botSelectInfo.arrowright.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	botSelectInfo.arrowright.generic.x = 290;
	botSelectInfo.arrowright.generic.y = 396;
	botSelectInfo.arrowright.generic.callback = UI_SelectBots_RightEvent;
	botSelectInfo.arrowright.width = 60;
	botSelectInfo.arrowright.height = 25;
	botSelectInfo.arrowright.focuspic = ARROWRT1;
	botSelectInfo.arrowright.focuspicinstead = qtrue;

	botSelectInfo.headerbots.generic.type = MTYPE_BITMAP;
	botSelectInfo.headerbots.generic.name = HEADERBOTS;
	botSelectInfo.headerbots.generic.x = 596;
	botSelectInfo.headerbots.generic.y = 190;
	botSelectInfo.headerbots.width = 70;
	botSelectInfo.headerbots.height = 35;

	botSelectInfo.headercolor.generic.type = MTYPE_BITMAP;
	botSelectInfo.headercolor.generic.name = HEADERCOLOR;
	botSelectInfo.headercolor.generic.x = 706;
	botSelectInfo.headercolor.generic.y = 190;
	botSelectInfo.headercolor.width = 105;
	botSelectInfo.headercolor.height = 35;

	y = (414 - MAX_SELECTLISTBOTS * 16);

	for (i = 0; i < MAX_SELECTLISTBOTS; i++) {
		botSelectInfo.selectedbotnames[i].generic.type = MTYPE_TEXTS;
		botSelectInfo.selectedbotnames[i].fontHeight = 16.0f;
		botSelectInfo.selectedbotnames[i].generic.flags = QMF_SMALLFONT;
		botSelectInfo.selectedbotnames[i].generic.x = 600;
		botSelectInfo.selectedbotnames[i].generic.y = y + i * 16;
		botSelectInfo.selectedbotnames[i].string = "              "; // 14
		botSelectInfo.selectedbotnames[i].color = color_white;
		botSelectInfo.selectedbotnames[i].focuscolor = color_orange;
		botSelectInfo.selectedbotnames[i].generic.id = i;
		botSelectInfo.selectedbotnames[i].generic.callback = UI_SelectBots_SelectBotName;

		botSelectInfo.selectedbotteams[i].generic.type = MTYPE_TEXTS;
		botSelectInfo.selectedbotteams[i].fontHeight = 16.0f;
		botSelectInfo.selectedbotteams[i].generic.flags = QMF_SMALLFONT;
		botSelectInfo.selectedbotteams[i].generic.x = 720;
		botSelectInfo.selectedbotteams[i].generic.y = y + i * 16;
		botSelectInfo.selectedbotteams[i].string = "        "; // 8
		botSelectInfo.selectedbotteams[i].color = color_blue;
		botSelectInfo.selectedbotteams[i].focuscolor = color_white;
		botSelectInfo.selectedbotteams[i].generic.id = i;
		botSelectInfo.selectedbotteams[i].generic.callback = UI_SelectBots_SelectBotTeam;
	}

	botSelectInfo.arrowup.generic.type = MTYPE_BITMAP;
	botSelectInfo.arrowup.generic.name = ARROWUP0;
	botSelectInfo.arrowup.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	botSelectInfo.arrowup.generic.x = 794;
	botSelectInfo.arrowup.generic.y = 250;
	botSelectInfo.arrowup.generic.callback = UI_SelectBots_ListUp;
	botSelectInfo.arrowup.width = 25;
	botSelectInfo.arrowup.height = 60;
	botSelectInfo.arrowup.focuspic = ARROWUP1;
	botSelectInfo.arrowup.focuspicinstead = qtrue;

	botSelectInfo.arrowdown.generic.type = MTYPE_BITMAP;
	botSelectInfo.arrowdown.generic.name = ARROWDN0;
	botSelectInfo.arrowdown.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	botSelectInfo.arrowdown.generic.x = 794;
	botSelectInfo.arrowdown.generic.y = 326;
	botSelectInfo.arrowdown.generic.callback = UI_SelectBots_ListDown;
	botSelectInfo.arrowdown.width = 25;
	botSelectInfo.arrowdown.height = 60;
	botSelectInfo.arrowdown.focuspic = ARROWDN1;
	botSelectInfo.arrowdown.focuspicinstead = qtrue;

	botSelectInfo.BotSkill.generic.type = MTYPE_SPINCONTROL;
	botSelectInfo.BotSkill.generic.flags = QMF_SMALLFONT;
	botSelectInfo.BotSkill.generic.name = "Skill:";
	botSelectInfo.BotSkill.generic.x = 655;
	botSelectInfo.BotSkill.generic.y = 420;
	botSelectInfo.BotSkill.itemnames = botSkill_list;
	botSelectInfo.BotSkill.curvalue = 1;
	botSelectInfo.BotSkill.generic.callback = UI_SelectBots_BotSkillChanged;
	s_startserver.botskill = 2;

	botSelectInfo.slotsleft.generic.type = MTYPE_TEXTS;
	botSelectInfo.slotsleft.generic.flags = QMF_SMALLFONT;
	botSelectInfo.slotsleft.string = va("Open Slots: %2d", UI_SelectBots_GetSlotsLeft());
	botSelectInfo.slotsleft.generic.x = 600;
	botSelectInfo.slotsleft.generic.y = 440;
	botSelectInfo.slotsleft.color = color_yellow;

	botSelectInfo.back.generic.type = MTYPE_BITMAP;
	botSelectInfo.back.generic.name = BACK0;
	botSelectInfo.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	botSelectInfo.back.generic.callback = UI_SelectBots_BackEvent;
	botSelectInfo.back.generic.x = 8;
	botSelectInfo.back.generic.y = 446;
	botSelectInfo.back.width = 80;
	botSelectInfo.back.height = 40;
	botSelectInfo.back.focuspic = BACK1;

	for (i = 0; i < MAX_BOTSPERPAGE; i++) {
		Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.pics[i]);
	}

	Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.arrowleft);
	Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.arrowright);

	for (i = 0; i < MAX_SELECTLISTBOTS; i++) {
		Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.selectedbotnames[i]);
		Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.selectedbotteams[i]);
	}
	Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.headerbots);
	Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.headercolor);
	Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.arrowup);
	Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.arrowdown);
	Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.BotSkill);
	Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.slotsleft);
	Menu_AddItem(&botSelectInfo.menu, &botSelectInfo.back);

	UI_SelectBots_BuildList();
	UI_SelectBots_UpdateList(); // selectbot-list
	botSelectInfo.modelpage = botSelectInfo.selectedmodel / MAX_BOTSPERPAGE;
	UI_SelectBots_UpdateGrid();
}

/*
=================
UI_SelectBotsMenu
=================
*/
void UI_SelectBotsMenu(void) {
	UI_SelectBots_MenuInit();
	UI_PushMenu(&botSelectInfo.menu);
}
