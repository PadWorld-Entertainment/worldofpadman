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

static intptr_t QDECL testUISyscall(intptr_t command, ...) {
	va_list args;
	intptr_t result = 0;
	va_start(args, command);
	switch (command) {
	case UI_CVAR_REGISTER:
		memset(va_arg(args, vmCvar_t *), 0, sizeof(vmCvar_t));
		break;
	case UI_CVAR_VARIABLESTRINGBUFFER: {
		const char *name = va_arg(args, const char *);
		char *buffer = va_arg(args, char *);
		Q_strncpyz(buffer, strcmp(name, "sv_maxclients") == 0 ? "8" : "", va_arg(args, int));
		break;
	}
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
		result = FS_FOpenFileByMode(path, file, va_arg(args, int));
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

static qboolean activateMenuItem(const char *name) {
	int i;
	menuframework_s *menu = uis.activemenu;
	for (i = 0; i < menu->nitems; i++) {
		menucommon_s *item = menu->items[i];
		const char *label = item->type == MTYPE_TEXTS ? ((menutext_s *)item)->string : item->name;
		if (label && strcmp(label, name) == 0 && !(item->flags & (QMF_HIDDEN | QMF_INACTIVE | QMF_GRAYED))) {
			Menu_SetCursorToItem(menu, item);
			UI_KeyEvent(K_ENTER, qtrue);
			return qtrue;
		}
	}
	return qfalse;
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

static void testSetupMenus(void) {
	static const char *const buttons[] = {
		"menu/buttons/player0", "menu/buttons/controls0", "menu/buttons/system0",
		"menu/buttons/options0", "menu/buttons/defaults0",
	};
	menuframework_s *setup;
	int i;

	UI_SetActiveMenu(UIMENU_MAIN);
	ASSERT_EQ_INT(qtrue, activateMenuItem("menu/buttons/setup0"));
	ASSERT_EQ_INT(2, uis.menusp);
	setup = uis.activemenu;
	for (i = 0; i < ARRAY_LEN(buttons); i++) {
		ASSERT_EQ_INT(qtrue, activateMenuItem(buttons[i]));
		ASSERT_EQ_INT(3, uis.menusp);
		drawCalls = 0;
		UI_Refresh(16 * (i + 1));
		EXPECT_GT_INT(1, drawCalls);
		UI_KeyEvent(K_ESCAPE, qtrue);
		ASSERT_EQ_INT(qtrue, uis.activemenu == setup);
	}
}

int main(int argc, char *argv[]) {
	TESTS_INIT();
	dllEntry(testUISyscall);
	uis.xscale = uis.yscale = 1;
	UI_InitGameinfo();

	ADD_TEST(testLineCount);
	ADD_TEST(testWrappedStringDraw);
	ADD_TEST(testCreateBotSelection);
	ADD_TEST(testSetupMenus);

	TESTS_SHUTDOWN();
}
