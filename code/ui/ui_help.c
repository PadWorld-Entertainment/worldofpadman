/*
=======================================================================

HELP MENU

=======================================================================
*/

#include "ui_local.h"

#define ID_PREV 10
#define ID_NEXT 11

#define HELPMENU_PATH "menu/help/"

#define ARROWLT0 "menu/arrows/headyel_lt0"
#define ARROWLT1 "menu/arrows/headyel_lt1"
#define ARROWRT0 "menu/arrows/headyel_rt0"
#define ARROWRT1 "menu/arrows/headyel_rt1"

#define XPOSITION (SCREEN_WIDTH / 2)

#define HMI_GAMETYPE_W 400
#define HMI_GAMETYPE_H 400
#define HMI_GAMETYPE_X XPOSITION - 200
#define HMI_GAMETYPE_Y 0

#define HMI_ITEM_W 462
#define HMI_ITEM_H 400
#define HMI_ITEM_X XPOSITION - 201 - 19 // offset because picture is asymmetrical
#define HMI_ITEM_Y 0

float nwsOffset;

typedef struct {
	menuframework_s menu;

	qhandle_t img;
	float width, height, x, y;

	menubitmap_s prev;
	menubitmap_s next;
} helpMenuInfo_t;

static helpMenuInfo_t helpMenuInfo;

// TODO: Use short gametype names from bg
// Order needs to match gametype_t!
static const char *HELP_GAMETYPE[] = {
	"ffa",
	//	"tourney",
	//	"single",
	"syc", "lps", "ctkd", "ffa_team", "ft", "ctl", "syc_team", "bb",
	"unknown" // must be the last entry!
};

// Must contain an entry for each gametype_t
// Maps gametypes to array indices of HELP_GAMETYPE
static const int GAMETYPE_REMAP[] = {
	0, // GT_FFA
	9, // GT_TOURNAMENT, no help
	9, // GT_SINGLE_PLAYER, no help
	1, // GT_SPRAYFFA
	2, // GT_LPS
	3, // GT_CATCH
	4, // GT_TEAM
	5, // GT_FREEZETAG
	6, // GT_CTF
	7, // GT_SPRAY
	8, // GT_BALLOON
	9, // GT_MAX_GAME_TYPE, invalid thus no help
};

static const char *HELP_ITEM[] = {"powerups", "weapons"};
static const unsigned int NUM_HELP_ITEM = (sizeof(HELP_ITEM) / sizeof(HELP_ITEM[0]));

static int helpMax, helpMin = 0;
static int helpIndex = 0;
static const char **helpList = NULL;

/*
===============
UI_HelpMenu_AdjustButtons
===============
*/
static void UI_HelpMenu_AdjustButtons(void) {
	helpMenuInfo.prev.generic.flags &= ~QMF_GRAYED;
	helpMenuInfo.next.generic.flags &= ~QMF_GRAYED;

	if (helpIndex <= 0) {
		helpMenuInfo.prev.generic.flags |= QMF_GRAYED;
	}
	if (helpIndex >= helpMax) {
		helpMenuInfo.next.generic.flags |= QMF_GRAYED;
	}

	helpMenuInfo.img = trap_R_RegisterShaderNoMip(va(HELPMENU_PATH "%s", helpList[helpIndex]));
}

/*
===============
UI_HelpMenu_Event
===============
*/
static void UI_HelpMenu_Event(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_PREV:
		helpIndex--;
		break;

	case ID_NEXT:
	default:
		helpIndex++;
		break;
	}

	if (helpIndex < helpMin) {
		helpIndex = helpMin;
	} else if (helpIndex > helpMax) {
		helpIndex = helpMax;
	}

	UI_HelpMenu_AdjustButtons();
}

/*
=================
UI_HelpMenu_Key
=================
*/
static sfxHandle_t UI_HelpMenu_Key(int key) {
	switch (key) {
	case K_KP_LEFTARROW:
	case K_LEFTARROW:
		UI_HelpMenu_Event(&helpMenuInfo.prev, QM_ACTIVATED);
		break;

	case K_KP_RIGHTARROW:
	case K_RIGHTARROW:
		UI_HelpMenu_Event(&helpMenuInfo.next, QM_ACTIVATED);
		break;

	// TODO: Close menu with second keypress. How to get original key (from "ui_help x" bind(s))?
	//       Key_GetKey seems to be in cgame only
	// FIXME: Hardcoded like in ui_musicbox until there's a better approach
	case 'g':
	case 'h':
		UI_PopMenu();
		return 0;

	default:
		break;
	}

	return Menu_DefaultKey(&helpMenuInfo.menu, key);
}

/*
===============
UI_HelpMenu_Draw
===============
*/
static void UI_HelpMenu_Draw(void) {
	// for non-widescreen resolutions we need to calculate and subtract an offset to the height so that the menu always stays at the top of the screen
	nwsOffset = ((float)uis.glconfig.vidHeight * (float)SCREEN_WIDTH / (float)uis.glconfig.vidWidth) - (float)SCREEN_HEIGHT;
	if (nwsOffset < 0) {
		nwsOffset = 0.0f;
	}

	UI_DrawHandlePic(helpMenuInfo.x, helpMenuInfo.y - (nwsOffset * 0.5f), helpMenuInfo.width, helpMenuInfo.height, helpMenuInfo.img);

	// standard menu drawing
	Menu_Draw(&helpMenuInfo.menu);
}

/*
===============
UI_HelpMenu_SetTopic
===============
*/
static void UI_HelpMenu_SetTopic(void) {
	const char *arg;
	char info[MAX_INFO_STRING];
	int gametype;

	if (trap_Argc() > 1) {
		arg = UI_Argv(1);

		if (Q_stricmp(arg, "gametype") == 0) {
			helpList = HELP_GAMETYPE;

			trap_GetConfigString(CS_SERVERINFO, info, sizeof(info));
			gametype = atoi(Info_ValueForKey(info, "g_gametype"));
			if ((gametype < 0) || (gametype > GT_MAX_GAME_TYPE)) {
				gametype = GT_MAX_GAME_TYPE;
			}

			helpIndex = GAMETYPE_REMAP[gametype];
			helpMin = helpIndex;
			helpMax = helpIndex;

			helpMenuInfo.prev.generic.flags = QMF_HIDDEN;
			helpMenuInfo.next.generic.flags = QMF_HIDDEN;

			helpMenuInfo.width = HMI_GAMETYPE_W;
			helpMenuInfo.height = HMI_GAMETYPE_H;
			helpMenuInfo.x = HMI_GAMETYPE_X;
			helpMenuInfo.y = HMI_GAMETYPE_Y;
		} else {
			helpMin = 0;
			helpMax = (NUM_HELP_ITEM - 1);
			helpList = HELP_ITEM;
			helpIndex = helpMin;
			helpMenuInfo.width = HMI_ITEM_W;
			helpMenuInfo.height = HMI_ITEM_H;
			helpMenuInfo.x = HMI_ITEM_X;
			helpMenuInfo.y = HMI_ITEM_Y;
		}
	} else {
		helpMin = 0;
		helpMax = (NUM_HELP_ITEM - 1);
		helpList = HELP_ITEM;
		helpIndex = helpMax;
		helpMenuInfo.width = HMI_ITEM_W;
		helpMenuInfo.height = HMI_ITEM_H;
		helpMenuInfo.x = HMI_ITEM_X;
		helpMenuInfo.y = HMI_ITEM_Y;
	}

	UI_HelpMenu_AdjustButtons();
}

/*
===============
UI_HelpMenu_Init
===============
*/
static void UI_HelpMenu_Init(void) {
	UI_HelpMenu_Cache();

	memset(&helpMenuInfo, 0, sizeof(helpMenuInfo));
	helpMenuInfo.menu.fullscreen = qfalse;
	helpMenuInfo.menu.key = UI_HelpMenu_Key;
	helpMenuInfo.menu.draw = UI_HelpMenu_Draw;

	helpMenuInfo.prev.generic.type = MTYPE_BITMAP;
	helpMenuInfo.prev.generic.id = ID_PREV;
	helpMenuInfo.prev.generic.x = XPOSITION - 68;
	helpMenuInfo.prev.generic.y = 322 - (nwsOffset * 0.5f);
	helpMenuInfo.prev.width = 60;
	helpMenuInfo.prev.height = 26;
	helpMenuInfo.prev.generic.name = ARROWLT0;
	helpMenuInfo.prev.focuspic = ARROWLT1;
	helpMenuInfo.prev.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	helpMenuInfo.prev.generic.callback = UI_HelpMenu_Event;

	helpMenuInfo.next.generic.type = MTYPE_BITMAP;
	helpMenuInfo.next.generic.id = ID_NEXT;
	helpMenuInfo.next.generic.x = XPOSITION + 8;
	helpMenuInfo.next.generic.y = 322 - (nwsOffset * 0.5f);
	helpMenuInfo.next.width = 60;
	helpMenuInfo.next.height = 26;
	helpMenuInfo.next.generic.name = ARROWRT0;
	helpMenuInfo.next.focuspic = ARROWRT1;
	helpMenuInfo.next.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	helpMenuInfo.next.generic.callback = UI_HelpMenu_Event;

	Menu_AddItem(&helpMenuInfo.menu, &helpMenuInfo.prev);
	Menu_AddItem(&helpMenuInfo.menu, &helpMenuInfo.next);

	UI_HelpMenu_SetTopic();
}

/*
=================
UI_HelpMenu_Cache
=================
*/
void UI_HelpMenu_Cache(void) {
	trap_R_RegisterShaderNoMip(ARROWLT0);
	trap_R_RegisterShaderNoMip(ARROWLT1);
	trap_R_RegisterShaderNoMip(ARROWRT0);
	trap_R_RegisterShaderNoMip(ARROWRT1);
}

/*
===============
UI_HelpMenu
===============
*/
void UI_HelpMenu(void) {
	trap_Cvar_Set("cl_paused", "1"); // pause game and indirectly pause/stop background music

	UI_HelpMenu_Init();
	UI_PushMenu(&helpMenuInfo.menu);
}

/*
===============
UI_HelpMenu_f
===============
*/
void UI_HelpMenu_f(void) {
	// Just a wrapper for "ui_help" command
	UI_HelpMenu();
}
