#ifndef TEST_SHARED_H
#define TEST_SHARED_H

#include "q_shared.h"
#include "qcommon.h"
#include "../sys/sys_local.h"

#define TEST_STRINGIFY(arg) #arg

#define ADD_TEST(func)                                                                                                 \
	prevFailed = failed;                                                                                               \
	errorBuf[0] = '\0';                                                                                                \
	printf("Testing  %-30s...", TEST_STRINGIFY(func));                                                                 \
	(func)();                                                                                                          \
	if (prevFailed == failed) {                                                                                        \
		printf("  [success]\n");                                                                                       \
	} else {                                                                                                           \
		printf("   [failed]\n");                                                                                       \
		printf("%s", errorBuf);                                                                                        \
	}                                                                                                                  \
	++tests

#define ADD_DISABLED_TEST(func)                                                                                        \
	prevFailed = failed;                                                                                               \
	errorBuf[0] = '\0';                                                                                                \
	if (!runDisabled) {                                                                                                \
		printf("Skipping %-30s...", TEST_STRINGIFY(func));                                                             \
		printf("  [skip]\n");                                                                                          \
	} else {                                                                                                           \
		printf("Testing  %-30s...", TEST_STRINGIFY(func));                                                             \
		(func)();                                                                                                      \
		if (prevFailed == failed) {                                                                                    \
			printf("  [success]\n");                                                                                   \
		} else {                                                                                                       \
			printf("   [failed]\n");                                                                                   \
			printf("%s", errorBuf);                                                                                    \
		}                                                                                                              \
	}                                                                                                                  \
	++tests

#define TESTS_GLOBALS()                                                                                                \
	static int failed = 0;                                                                                             \
	static int tests = 0;                                                                                              \
	static int prevFailed = 0;                                                                                         \
	static char errorBuf[4096] = "";                                                                                   \
	static int lastExpectedInt = 0;                                                                                    \
	static const char *lastExpectedString = NULL;                                                                      \
	static int runDisabled = 0;

#define TESTS_SHUTDOWN()                                                                                               \
	printf("\nfailed tests: %i out of %i\n", failed, tests);                                                           \
	if (failed != 0) {                                                                                                 \
		return 1;                                                                                                      \
	}                                                                                                                  \
	return 0

#define TESTS_INIT()                                                                                                   \
	if (argc > 1) {                                                                                                    \
		if (!strcmp(argv[1], "--also_run_disabled_tests")) {                                                           \
			runDisabled = 1;                                                                                           \
		} else if (!strcmp(argv[1], "--help")) {                                                                       \
			printf("--also_run_disabled_tests : also run disabled tests");                                             \
			return 0;                                                                                                  \
		}                                                                                                              \
	}                                                                                                                  \
	Sys_PlatformInit();                                                                                                \
	Sys_Milliseconds();                                                                                                \
	Com_Init("")

#define ASSERT_EQ_INT(exp, actual)                                                                                     \
	if (lastExpectedInt = (actual), (exp) != lastExpectedInt) {                                                        \
		snprintf(errorBuf + strlen(errorBuf), sizeof(errorBuf) - strlen(errorBuf),                                     \
				 " - " TEST_STRINGIFY(actual) ": expected %i, but got %i\n", exp, lastExpectedInt);                    \
		++failed;                                                                                                      \
		return;                                                                                                        \
	}

#define EXPECT_EQ_INT(exp, actual)                                                                                     \
	if (lastExpectedInt = (actual), (exp) != lastExpectedInt) {                                                        \
		snprintf(errorBuf + strlen(errorBuf), sizeof(errorBuf) - strlen(errorBuf),                                     \
				 " - " TEST_STRINGIFY(actual) ": expected %i, but got %i\n", exp, lastExpectedInt);                    \
		++failed;                                                                                                      \
	}

#define EXPECT_BETWEEN_INT(minv, maxv, actual)                                                                         \
	if (lastExpectedInt = (actual), lastExpectedInt < minv || lastExpectedInt > maxv) {                                \
		snprintf(errorBuf + strlen(errorBuf), sizeof(errorBuf) - strlen(errorBuf),                                     \
				 " - " TEST_STRINGIFY(actual) ": expected %i to in range of [%i:%i]\n", lastExpectedInt, minv, maxv);  \
		++failed;                                                                                                      \
	}

#define EXPECT_GT_INT(exp, actual)                                                                                     \
	if (lastExpectedInt = (actual), (exp) >= lastExpectedInt) {                                                        \
		snprintf(errorBuf + strlen(errorBuf), sizeof(errorBuf) - strlen(errorBuf),                                     \
				 " - " TEST_STRINGIFY(actual) ": expected to be greater than %i, but got %i\n", exp, lastExpectedInt); \
		++failed;                                                                                                      \
	}

#define EXPECT_GE_INT(exp, actual)                                                                                     \
	if (lastExpectedInt = (actual), (exp) > lastExpectedInt) {                                                         \
		snprintf(errorBuf + strlen(errorBuf), sizeof(errorBuf) - strlen(errorBuf),                                     \
				 " - " TEST_STRINGIFY(actual) ": expected to be greater or equal to %i, but got %i\n", exp,            \
				 lastExpectedInt);                                                                                     \
		++failed;                                                                                                      \
	}

#define EXPECT_LT_INT(exp, actual)                                                                                     \
	if (lastExpectedInt = (actual), (exp) <= lastExpectedInt) {                                                        \
		snprintf(errorBuf + strlen(errorBuf), sizeof(errorBuf) - strlen(errorBuf),                                     \
				 " - " TEST_STRINGIFY(actual) ": expected to be less than %i, but got %i\n", exp, lastExpectedInt);    \
		++failed;                                                                                                      \
	}

#define EXPECT_LE_INT(exp, actual)                                                                                     \
	if (lastExpectedInt = (actual), (exp) < lastExpectedInt) {                                                         \
		snprintf(errorBuf + strlen(errorBuf), sizeof(errorBuf) - strlen(errorBuf),                                     \
				 " - " TEST_STRINGIFY(actual) ": expected to be less or equal to %i, but got %i\n", exp,               \
				 lastExpectedInt);                                                                                     \
		++failed;                                                                                                      \
	}

#define ASSERT_EQ_STRING(exp, actual)                                                                                  \
	if (lastExpectedString = (actual), strcmp(exp, lastExpectedString) != 0) {                                         \
		snprintf(errorBuf + strlen(errorBuf), sizeof(errorBuf) - strlen(errorBuf),                                     \
				 " - " TEST_STRINGIFY(actual) ": expected '%s', but got '%s'\n", exp, lastExpectedString);             \
		++failed;                                                                                                      \
		return;                                                                                                        \
	}

#define EXPECT_EQ_STRING(exp, actual)                                                                                  \
	if (lastExpectedString = (actual), strcmp(exp, lastExpectedString) != 0) {                                         \
		snprintf(errorBuf + strlen(errorBuf), sizeof(errorBuf) - strlen(errorBuf),                                     \
				 " - " TEST_STRINGIFY(actual) ": expected '%s', but got '%s'\n", exp, lastExpectedString);             \
		++failed;                                                                                                      \
	}

#define EXPECT_NE_STRING(exp, actual)                                                                                  \
	if (lastExpectedString = (actual), strcmp(exp, lastExpectedString) == 0) {                                         \
		snprintf(errorBuf + strlen(errorBuf), sizeof(errorBuf) - strlen(errorBuf),                                     \
				 " - " TEST_STRINGIFY(actual) ": expected '%s', but got '%s'\n", exp, lastExpectedString);             \
		++failed;                                                                                                      \
	}

#endif
