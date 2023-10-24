#include "test_shared.h"
#include "ui_local.h"

TESTS_GLOBALS();

static void testLineCount(void) {
	const char *s = "first\n"
					"second second second\n"
					"third third third third third wrapped\n"
					"fifth\n"
					"sixth";
	EXPECT_EQ_INT(5, UI_AutoWrappedString_LineCount(250, s, UI_SMALLFONT, qfalse));
	EXPECT_EQ_INT(1, UI_AutoWrappedString_LineCount(250, "FooBar", UI_SMALLFONT, qfalse));
	EXPECT_EQ_INT(2, UI_AutoWrappedString_LineCount(250, "FooBar\n", UI_SMALLFONT, qfalse));
	EXPECT_EQ_INT(3, UI_AutoWrappedString_LineCount(250, "FooBar\n\n", UI_SMALLFONT, qfalse));
}

int main(int argc, char *argv[]) {
	TESTS_INIT();

	ADD_TEST(testLineCount);

	TESTS_SHUTDOWN();
}
