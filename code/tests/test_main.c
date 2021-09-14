#include "test_shared.h"

TESTS_GLOBALS();

static void testCommon() {
	EXPECT_EQ_STRING("file", COM_SkipPath("/path/to/file"));
	EXPECT_EQ_INT(0, Q_stricmp("foo", "FOo"));
}

int main(int argc, char *argv[]) {
	ADD_TEST(testCommon);

	TESTS_SHUTDOWN();
}
