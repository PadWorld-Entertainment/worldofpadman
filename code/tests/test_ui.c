#include "test_shared.h"
#include "ui_local.h"

TESTS_GLOBALS();

static void testLineCount(void) {
	const char *s = "first\n"
					"second second second\n"
					"third third third third third wrapped\n"
					"fifth\n"
					"sixth";
	EXPECT_EQ_INT(6, UI_AutoWrappedString_LineCount(250, s, UI_SMALLFONT, qfalse));
	EXPECT_EQ_INT(5, UI_AutoWrappedString_LineCount(320, s, UI_SMALLFONT, qfalse));
	EXPECT_EQ_INT(0, UI_AutoWrappedString_LineCount(250, "", UI_SMALLFONT, qfalse));
	EXPECT_EQ_INT(2, UI_AutoWrappedString_LineCount(250, "\nFooBar", UI_SMALLFONT, qfalse));
	EXPECT_EQ_INT(1, UI_AutoWrappedString_LineCount(250, "FooBar", UI_SMALLFONT, qfalse));
	EXPECT_EQ_INT(2, UI_AutoWrappedString_LineCount(250, "FooBar\n", UI_SMALLFONT, qfalse));
	EXPECT_EQ_INT(3, UI_AutoWrappedString_LineCount(250, "FooBar\n\n", UI_SMALLFONT, qfalse));
	EXPECT_EQ_INT(3, UI_AutoWrappedString_LineCount(250, "FooBar\n\nBaz", UI_SMALLFONT, qtrue));
	EXPECT_EQ_INT(1, UI_AutoWrappedString_LineCount(4 * SMALLCHAR_WIDTH, "FooBar", UI_SMALLFONT, qfalse));
}

extern void QDECL dllEntry(intptr_t(QDECL *syscallptr)(intptr_t, ...));

static int keyCatcher;
static int drawCalls;
static float lastDrawY;
static uiClientState_t clientState;
static qboolean passwordServer;
static qboolean voteRunning;
static char lastCommand[MAX_STRING_CHARS];

static const char *testConfigString(int index) {
	if (clientState.connState < CA_CONNECTED)
		return "";
	switch (index) {
	case CS_SERVERINFO:
		return va("\\sv_hostname\\Test server\\mapname\\wop_diner\\sv_maxclients\\8\\g_gametype\\%d\\g_allowVote\\1",
				  Cvar_VariableIntegerValue("g_gametype"));
	case CS_PLAYERS:
		return "\\n\\Local player\\t\\1\\model\\padman";
	case CS_PLAYERS + 1:
		return "\\n\\Test Bot\\t\\1\\model\\padman\\skill\\3";
	case CS_PLAYERS + 2:
		return "\\n\\Remote player\\t\\2\\model\\padgirl";
	case CS_VOTE_TIME:
		return voteRunning ? "1" : "";
	case CS_VOTE_STRING:
		return "map_restart";
	case CS_VOTE_YES:
		return "1";
	case CS_VOTE_NO:
		return "0";
	default:
		return "";
	}
}

static intptr_t QDECL testUISyscall(intptr_t command, ...) {
	va_list args;
	intptr_t result = 0;
	va_start(args, command);
	switch (command) {
	case UI_CVAR_REGISTER: {
		vmCvar_t *cvar = va_arg(args, vmCvar_t *);
		const char *name = va_arg(args, const char *);
		const char *value = va_arg(args, const char *);
		Cvar_Register(cvar, name, value, va_arg(args, int));
		break;
	}
	case UI_CVAR_UPDATE:
		Cvar_Update(va_arg(args, vmCvar_t *));
		break;
	case UI_CVAR_SET: {
		const char *name = va_arg(args, const char *);
		Cvar_Set(name, va_arg(args, const char *));
		break;
	}
	case UI_CVAR_SETVALUE: {
		const char *name = va_arg(args, const char *);
		floatint_t value;
		value.i = va_arg(args, int);
		Cvar_SetValue(name, value.f);
		break;
	}
	case UI_CVAR_VARIABLEVALUE: {
		floatint_t value;
		value.f = Cvar_VariableValue(va_arg(args, const char *));
		result = value.i;
		break;
	}
	case UI_CVAR_VARIABLESTRINGBUFFER: {
		const char *name = va_arg(args, const char *);
		char *buffer = va_arg(args, char *);
		Cvar_VariableStringBuffer(name, buffer, va_arg(args, int));
		break;
	}
	case UI_ARGC:
		result = Cmd_Argc();
		break;
	case UI_ARGV: {
		int index = va_arg(args, int);
		char *buffer = va_arg(args, char *);
		Q_strncpyz(buffer, Cmd_Argv(index), va_arg(args, int));
		break;
	}
	case UI_CMD_EXECUTETEXT:
		(void)va_arg(args, int);
		Q_strncpyz(lastCommand, va_arg(args, const char *), sizeof(lastCommand));
		break;
	case UI_GETCLIENTSTATE:
		*va_arg(args, uiClientState_t *) = clientState;
		break;
	case UI_GETCONFIGSTRING: {
		const char *value = testConfigString(va_arg(args, int));
		char *buffer = va_arg(args, char *);
		Q_strncpyz(buffer, value, va_arg(args, int));
		result = *value != '\0';
		break;
	}
	case UI_GETGLCONFIG: {
		glconfig_t *config = va_arg(args, glconfig_t *);
		memset(config, 0, sizeof(*config));
		config->vidWidth = SCREEN_WIDTH;
		config->vidHeight = SCREEN_HEIGHT;
		break;
	}
	case UI_LAN_GETSERVERCOUNT:
		result = passwordServer && va_arg(args, int) == AS_LOCAL;
		break;
	case UI_LAN_GETSERVERADDRESSSTRING:
	case UI_LAN_GETSERVERINFO: {
		char *buffer;
		(void)va_arg(args, int);
		(void)va_arg(args, int);
		buffer = va_arg(args, char *);
		Q_strncpyz(buffer, command == UI_LAN_GETSERVERINFO ? "\\g_needpass\\1" : "127.0.0.1:27960", va_arg(args, int));
		break;
	}
	case UI_LAN_GETSERVERPING:
		result = 20;
		break;
	case UI_FS_GETFILELIST: {
		const char *path = va_arg(args, const char *);
		const char *extension = va_arg(args, const char *);
		char *list = va_arg(args, char *);
		result = FS_GetFileList(path, extension, list, va_arg(args, int));
		break;
	}
	case UI_FS_FOPENFILE: {
		const char *path = va_arg(args, const char *);
		fileHandle_t *file = va_arg(args, fileHandle_t *);
		*file = 0;
		result = -1;
		if (va_arg(args, int) == FS_READ && strcmp(path, "playlist.dat") != 0)
			result = FS_FOpenFileByMode(path, file, FS_READ);
		break;
	}
	case UI_FS_READ: {
		void *buffer = va_arg(args, void *);
		int size = va_arg(args, int);
		result = FS_Read(buffer, size, va_arg(args, fileHandle_t));
		break;
	}
	case UI_FS_FCLOSEFILE:
		FS_FCloseFile(va_arg(args, fileHandle_t));
		break;
	case UI_R_REGISTERSHADERNOMIP:
		result = 1;
		break;
	case UI_R_DRAWSTRETCHPIC: {
		floatint_t y;
		(void)va_arg(args, int);
		y.i = va_arg(args, int);
		lastDrawY = y.f;
		drawCalls++;
		break;
	}
	case UI_KEY_GETCATCHER:
		result = keyCatcher;
		break;
	case UI_KEY_SETCATCHER:
		keyCatcher = va_arg(args, int);
		break;
	case UI_KEY_KEYNUMTOSTRINGBUF:
	case UI_KEY_GETBINDINGBUF:
		(void)va_arg(args, int);
		va_arg(args, char *)[0] = '\0';
		break;
	case UI_ERROR:
		fprintf(stderr, "%s\n", va_arg(args, const char *));
		break;
	}
	va_end(args);
	return result;
}

static void testWrappedStringDraw(void) {
	const char *text = "1234 5678\n\nX";
	const int width = 4 * SMALLCHAR_WIDTH;
	drawCalls = 0;
	UI_DrawString_AutoWrapped(0, 0, width, 20, text, UI_SMALLFONT, color_white, qfalse);
	EXPECT_EQ_INT(4, UI_AutoWrappedString_LineCount(width, text, UI_SMALLFONT, qfalse));
	EXPECT_EQ_INT(9, drawCalls);
	EXPECT_EQ_INT(60, (int)lastDrawY);
}

static menucommon_s *findMenuItem(const char *name) {
	int i;
	menuframework_s *menu = uis.activemenu;
	for (i = 0; i < menu->nitems; i++) {
		menucommon_s *item = menu->items[i];
		const char *label = item->type == MTYPE_TEXTS ? ((menutext_s *)item)->string : item->name;
		if (label && strcmp(label, name) == 0)
			return item;
	}
	return NULL;
}

static qboolean activateMenuItem(const char *name) {
	menucommon_s *item = findMenuItem(name);
	if (!item || (item->flags & (QMF_HIDDEN | QMF_INACTIVE | QMF_GRAYED)))
		return qfalse;
	Menu_SetCursorToItem(uis.activemenu, item);
	UI_KeyEvent(K_ENTER, qtrue);
	return qtrue;
}

static int visibleBotSelectionLabels(void) {
	int i, count = 0;
	for (i = 0; i < uis.activemenu->nitems; i++) {
		const menucommon_s *item = uis.activemenu->items[i];
		if (item->type == MTYPE_TEXTS && item->callback && !(item->flags & QMF_HIDDEN)) {
			count++;
		}
	}
	return count;
}

static void testCreateBotSelection(void) {
	// The 65th shipped arena used to overwrite the first selected bot.
	EXPECT_GE_INT(65, UI_GetNumArenas());
	UI_MainMenu();
	ASSERT_EQ_INT(qtrue, activateMenuItem("menu/buttons/create0"));
	ASSERT_EQ_INT(qtrue, activateMenuItem("menu/buttons/selectbots0"));
	EXPECT_EQ_INT(0, visibleBotSelectionLabels());
	ASSERT_EQ_INT(qtrue, activateMenuItem("models/wop_players/padman/icon_default"));
	EXPECT_EQ_INT(2, visibleBotSelectionLabels());
	UI_KeyEvent(K_ESCAPE, qtrue);
	ASSERT_EQ_INT(qtrue, activateMenuItem("menu/buttons/selectbots0"));
	EXPECT_EQ_INT(2, visibleBotSelectionLabels());
	ASSERT_EQ_INT(qtrue, activateMenuItem("PADMAN"));
	EXPECT_EQ_INT(0, visibleBotSelectionLabels());
}

static qboolean drawMenuFrame(void) {
	drawCalls = 0;
	UI_Refresh(uis.realtime + 16);
	return drawCalls > 1;
}

static qboolean openMenu(const char *button) {
	menuframework_s *previous = uis.activemenu;
	if (activateMenuItem(button) && uis.activemenu && uis.activemenu != previous && drawMenuFrame())
		return qtrue;

	printf("\nCould not open or draw menu from '%s'\n", button);
	return qfalse;
}

static void testMainMenus(void) {
	static const char *const paths[][3] = {
		{"menu/buttons/create0", "menu/buttons/selectbots0"},
		{"menu/buttons/demos0"},
		{"menu/buttons/mods0"},
		{"menu/buttons/credits0"},
		{"menu/buttons/mania0"},
		{"menu/buttons/setup0", "menu/buttons/player0"},
		{"menu/buttons/setup0", "menu/buttons/controls0"},
		{"menu/buttons/setup0", "menu/buttons/options0"},
		{"menu/buttons/setup0", "menu/buttons/defaults0"},
		{"menu/buttons/setup0", "menu/buttons/system0"},
		{"menu/buttons/setup0", "menu/buttons/system0", "menu/buttons/display0"},
		{"menu/buttons/setup0", "menu/buttons/system0", "menu/buttons/effects0"},
		{"menu/buttons/setup0", "menu/buttons/system0", "menu/buttons/sound0"},
		{"menu/buttons/setup0", "menu/buttons/system0", "menu/buttons/netvoip0"},
	};
	int i, j;

	UI_SetActiveMenu(UIMENU_MAIN);
	ASSERT_EQ_INT(qtrue, drawMenuFrame());
	for (i = 0; i < ARRAY_LEN(paths); i++) {
		UI_SetActiveMenu(UIMENU_MAIN);
		for (j = 0; j < ARRAY_LEN(paths[i]) && paths[i][j]; j++) {
			if (!openMenu(paths[i][j]))
				break;
		}
		EXPECT_EQ_INT(qtrue, j == ARRAY_LEN(paths[i]) || !paths[i][j]);
		if (uis.menusp > 1) {
			menuframework_s *parent = uis.stack[uis.menusp - 2];
			UI_KeyEvent(K_ESCAPE, qtrue);
			EXPECT_EQ_INT(qtrue, uis.activemenu == parent);
		}
	}
}

static void testVideoModes(void) {
	static const struct {
		const char *modes;
		const char *firstLabel;
	} cases[] = {
		{"", "640x480 (4:3)"},
		{"", "640x480 (4:3)"},
		{"800x600 1280x720", "800x600 (4:3)"},
		{"", "640x480 (4:3)"},
	};
	int i;

	UI_SetActiveMenu(UIMENU_MAIN);
	ASSERT_EQ_INT(qtrue, openMenu("menu/buttons/setup0"));
	Cvar_Set("r_mode", "1");
	for (i = 0; i < ARRAY_LEN(cases); i++) {
		menulist_s *mode;
		Cvar_Set("r_availableModes", cases[i].modes);
		ASSERT_EQ_INT(qtrue, openMenu("menu/buttons/system0"));
		mode = (menulist_s *)findMenuItem("Video Mode:");
		ASSERT_EQ_INT(qtrue, mode != NULL);
		EXPECT_EQ_STRING(cases[i].firstLabel, mode->itemnames[0]);
		EXPECT_EQ_INT(cases[i].modes[0] ? 0 : 1, mode->curvalue);
		if (cases[i].modes[0]) {
			Menu_SetCursorToItem(uis.activemenu, mode);
			UI_KeyEvent(K_RIGHTARROW, qtrue);
			ASSERT_EQ_INT(qtrue, uis.dropdownlist == mode);
			EXPECT_EQ_INT(qtrue, drawMenuFrame());
			UI_MouseEvent(uis.dropdownxywh[0] + 1 - uis.cursorx,
						  uis.dropdownxywh[1] + SMALLCHAR_HEIGHT + 1 - uis.cursory);
			UI_KeyEvent(K_MOUSE1, qtrue);
			EXPECT_EQ_INT(qtrue, uis.dropdownlist == NULL);
			EXPECT_EQ_INT(1, mode->curvalue);
		}
		UI_KeyEvent(K_ESCAPE, qtrue);
	}
}

static void testControlsTabs(void) {
	static const char *const tabs[] = {
		"menu/buttons/look0", "menu/buttons/move0", "menu/buttons/shoot0", "menu/buttons/chat0", "menu/buttons/misc0",
	};
	menuframework_s *controls;
	int i;

	UI_SetActiveMenu(UIMENU_MAIN);
	ASSERT_EQ_INT(qtrue, openMenu("menu/buttons/setup0"));
	ASSERT_EQ_INT(qtrue, openMenu("menu/buttons/controls0"));
	controls = uis.activemenu;
	for (i = 0; i < ARRAY_LEN(tabs); i++) {
		ASSERT_EQ_INT(qtrue, activateMenuItem(tabs[i]));
		ASSERT_EQ_INT(qtrue, uis.activemenu == controls);
		EXPECT_EQ_INT(qtrue, (findMenuItem(tabs[i])->flags & QMF_HIGHLIGHT) != 0);
		EXPECT_EQ_INT(qtrue, drawMenuFrame());
	}
}

static void testMainMenuDialogs(void) {
	menuframework_s *mainMenu;

	UI_SetActiveMenu(UIMENU_MAIN);
	mainMenu = uis.activemenu;

	// The secret button is intentionally hidden and mouse-only.
	UI_MouseEvent(325 - uis.cursorx, 336 - uis.cursory);
	UI_KeyEvent(K_MOUSE1, qtrue);
	ASSERT_EQ_INT(2, uis.menusp);
	EXPECT_EQ_INT(qtrue, drawMenuFrame());
	UI_KeyEvent(K_RIGHTARROW, qtrue);
	EXPECT_EQ_INT(qtrue, drawMenuFrame());
	UI_KeyEvent(K_ESCAPE, qtrue);
	EXPECT_EQ_INT(qtrue, uis.activemenu == mainMenu);

	ASSERT_EQ_INT(qtrue, openMenu("menu/buttons/exit0"));
	lastCommand[0] = '\0';
	UI_KeyEvent(K_ENTER, qtrue);
	EXPECT_EQ_STRING("quit\n", lastCommand);

	Cvar_Set("com_errorMessage", "Test connection error.\nPlease try again.");
	UI_SetActiveMenu(UIMENU_MAIN);
	EXPECT_EQ_INT(qtrue, uis.activemenu != mainMenu);
	EXPECT_EQ_INT(qtrue, drawMenuFrame());
	UI_KeyEvent(K_ENTER, qtrue);
	EXPECT_EQ_INT(qtrue, uis.activemenu == mainMenu);
	EXPECT_EQ_STRING("", Cvar_VariableString("com_errorMessage"));
}

static void typeText(const char *text) {
	for (; *text; text++)
		UI_KeyEvent(*text | K_CHAR_FLAG, qtrue);
}

static void testJoinMenus(void) {
	menuframework_s *mainMenu, *browser, *specify;
	qboolean passwordOpened;

	UI_SetActiveMenu(UIMENU_MAIN);
	mainMenu = uis.activemenu;
	ASSERT_EQ_INT(qtrue, openMenu("menu/buttons/join0"));
	browser = uis.activemenu;
	ASSERT_EQ_INT(qtrue, openMenu("menu/buttons/specify0"));
	specify = uis.activemenu;
	typeText("127.0.0.1");
	ASSERT_EQ_INT(qtrue, drawMenuFrame());

	passwordServer = qtrue;
	passwordOpened = openMenu("menu/buttons/fight0");
	passwordServer = qfalse;
	ASSERT_EQ_INT(qtrue, passwordOpened);
	EXPECT_EQ_INT(4, uis.menusp);
	typeText("pad-test");
	ASSERT_EQ_INT(qtrue, drawMenuFrame());
	lastCommand[0] = '\0';
	ASSERT_EQ_INT(qtrue, activateMenuItem("menu/buttons/fight0"));
	EXPECT_EQ_STRING("pad-test", Cvar_VariableString("password"));
	EXPECT_EQ_STRING("connect 127.0.0.1:27960\n", lastCommand);
	Cvar_Set("password", "");

	UI_KeyEvent(K_ESCAPE, qtrue);
	EXPECT_EQ_INT(qtrue, uis.activemenu == specify);
	UI_KeyEvent(K_ESCAPE, qtrue);
	EXPECT_EQ_INT(qtrue, uis.activemenu == browser);
	UI_KeyEvent(K_ESCAPE, qtrue);
	EXPECT_EQ_INT(qtrue, uis.activemenu == mainMenu);
}

static void testConnectScreen(void) {
	static const connstate_t states[] = {CA_CONNECTING, CA_CHALLENGING, CA_CONNECTED, CA_LOADING, CA_PRIMED};
	int i;
	Q_strncpyz(clientState.servername, "Test server", sizeof(clientState.servername));
	Q_strncpyz(clientState.messageString, "Waiting for the server", sizeof(clientState.messageString));
	for (i = 0; i < ARRAY_LEN(states); i++) {
		clientState.connState = states[i];
		drawCalls = 0;
		UI_DrawConnectScreen(qfalse);
		EXPECT_GT_INT(1, drawCalls);
	}
	clientState.connState = CA_CONNECTED;
	Cvar_Set("cl_downloadName", "test.pk3");
	Cvar_Set("cl_downloadSize", "65536");
	Cvar_Set("cl_downloadCount", "16384");
	Cvar_SetValue("cl_downloadTime", uis.realtime);
	UI_Refresh(uis.realtime + 2000);
	drawCalls = 0;
	UI_DrawConnectScreen(qfalse);
	EXPECT_GT_INT(1, drawCalls);
	Cvar_Set("cl_downloadName", "");
	clientState.connState = CA_DISCONNECTED;
}

static void startInGameMenu(void) {
	clientState.connState = CA_ACTIVE;
	Cvar_Set("sv_running", "1");
	Cvar_Set("bot_enable", "1");
	Cvar_Set("name", "Local player");
	Cvar_Set("cl_voip", "1");
	Cvar_Set("sv_voip", "1");
	Cvar_SetValue("g_gametype", GT_TEAM);
	UI_SetActiveMenu(UIMENU_INGAME);
}

static void testInGameMenus(void) {
	static const char *const buttons[] = {
		"START", "ADD BOTS", "REMOVE BOTS", "VOICE CHAT", "CALL VOTE",
		"SETUP", "SERVER INFO", "RESTART MAP", "EXIT GAME",
	};
	menuframework_s *inGame;
	int i;

	startInGameMenu();
	inGame = uis.activemenu;
	ASSERT_EQ_INT(qtrue, drawMenuFrame());
	for (i = 0; i < ARRAY_LEN(buttons); i++) {
		ASSERT_EQ_INT(qtrue, openMenu(buttons[i]));
		EXPECT_EQ_INT(2, uis.menusp);
		UI_KeyEvent(K_ESCAPE, qtrue);
		EXPECT_EQ_INT(qtrue, uis.activemenu == inGame);
	}
	ASSERT_EQ_INT(qtrue, activateMenuItem("RESUME GAME"));
	EXPECT_EQ_INT(0, uis.menusp);
}

static void testInGameActions(void) {
	startInGameMenu();
	ASSERT_EQ_INT(qtrue, openMenu("ADD BOTS"));
	ASSERT_EQ_INT(qtrue, activateMenuItem("ADD"));
	EXPECT_EQ_INT(0, strncmp("addbot ", lastCommand, 7));
	UI_KeyEvent(K_ESCAPE, qtrue);

	ASSERT_EQ_INT(qtrue, openMenu("REMOVE BOTS"));
	ASSERT_EQ_INT(qtrue, activateMenuItem("Test Bot"));
	ASSERT_EQ_INT(qtrue, activateMenuItem("REMOVE"));
	EXPECT_EQ_STRING("clientkick 1\n", lastCommand);
	startInGameMenu();

	ASSERT_EQ_INT(qtrue, openMenu("VOICE CHAT"));
	ASSERT_EQ_INT(qtrue, activateMenuItem("Remote player"));
	ASSERT_EQ_INT(qtrue, activateMenuItem("Mute"));
	EXPECT_EQ_STRING("voip ignore 2", lastCommand);
	UI_KeyEvent(K_ESCAPE, qtrue);

	voteRunning = qtrue;
	ASSERT_EQ_INT(qtrue, openMenu("CALL VOTE"));
	EXPECT_EQ_INT(qtrue, activateMenuItem("YES"));
	EXPECT_EQ_STRING("vote yes", lastCommand);
	voteRunning = qfalse;
	UI_KeyEvent(K_ESCAPE, qtrue);

	ASSERT_EQ_INT(qtrue, openMenu("RESTART MAP"));
	UI_KeyEvent('y', qtrue);
	EXPECT_EQ_STRING("map_restart 0\n", lastCommand);
	EXPECT_EQ_INT(0, uis.menusp);

	startInGameMenu();
	ASSERT_EQ_INT(qtrue, openMenu("START"));
	ASSERT_EQ_INT(qtrue, activateMenuItem("BLUE NOSES"));
	EXPECT_EQ_STRING("cmd team blue\n", lastCommand);
	EXPECT_EQ_INT(0, uis.menusp);

	// The actual logo picker belongs to cgame, while the pause menu dispatches its command.
	startInGameMenu();
	Cvar_SetValue("g_gametype", GT_SPRAY);
	UI_SetActiveMenu(UIMENU_INGAME);
	ASSERT_EQ_INT(qtrue, activateMenuItem("SELECT LOGO"));
	EXPECT_EQ_STRING("rechooselogo\n", lastCommand);
	EXPECT_EQ_INT(0, uis.menusp);
}

static void testConsoleMenus(void) {
	static const char *const commands[] = {"ui_help items", "ui_help gametype", "wop_music"};
	menuframework_s *inGame;
	int i;

	startInGameMenu();
	inGame = uis.activemenu;
	// Team Orders is compiled out. The legacy command must leave the current menu alone.
	Cmd_TokenizeString("ui_teamOrders");
	EXPECT_EQ_INT(qtrue, UI_ConsoleCommand(uis.realtime));
	EXPECT_EQ_INT(qtrue, uis.activemenu == inGame);
	for (i = 0; i < ARRAY_LEN(commands); i++) {
		const int time = uis.realtime;
		Cmd_TokenizeString(commands[i]);
		ASSERT_EQ_INT(qtrue, UI_ConsoleCommand(time));
		ASSERT_EQ_INT(2, uis.menusp);
		EXPECT_EQ_INT(qtrue, drawMenuFrame());
		UI_KeyEvent(K_ESCAPE, qtrue);
		// Music Box closes at the end of its animation.
		UI_Refresh(uis.realtime + 1000);
		EXPECT_EQ_INT(qtrue, uis.activemenu == inGame);
	}
	Cmd_TokenizeString("");
}

int main(int argc, char *argv[]) {
	TESTS_INIT();
	dllEntry(testUISyscall);
	Cvar_Set("com_errorMessage", "");
	Cvar_Set("sv_maxclients", "8");
	Cvar_Set("r_availableModes", "");
	UI_Init();

	ADD_TEST(testLineCount);
	ADD_TEST(testWrappedStringDraw);
	ADD_TEST(testCreateBotSelection);
	ADD_TEST(testMainMenus);
	ADD_TEST(testVideoModes);
	ADD_TEST(testControlsTabs);
	ADD_TEST(testMainMenuDialogs);
	ADD_TEST(testJoinMenus);
	ADD_TEST(testConnectScreen);
	ADD_TEST(testInGameMenus);
	ADD_TEST(testInGameActions);
	ADD_TEST(testConsoleMenus);

	TESTS_SHUTDOWN();
}
