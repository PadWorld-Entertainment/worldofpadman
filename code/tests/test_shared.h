#ifndef TEST_SHARED_H
#define TEST_SHARED_H

#include "q_shared.h"

#define TEST_STRINGIFY(arg) #arg

#define ADD_TEST(func)                                                                                                 \
	prevFailed = failed;                                                                                               \
	errorBuf[0] = '\0';                                                                                                \
	printf("Testing %-30s...", TEST_STRINGIFY(func));                                                                  \
	(func)();                                                                                                          \
	if (prevFailed == failed) {                                                                                        \
		printf(" [success]\n");                                                                                        \
	} else {                                                                                                           \
		printf("  [failed]\n");                                                                                        \
		printf("%s", errorBuf);                                                                                        \
	}                                                                                                                  \
	++tests

#define TESTS_GLOBALS()                                                                                                \
	static int failed = 0;                                                                                             \
	static int tests = 0;                                                                                              \
	static int prevFailed = 0;                                                                                         \
	static char errorBuf[4096] = "";                                                                                   \
	static int lastExpectedInt = 0;                                                                                    \
	static const char *lastExpectedString = NULL

#define TESTS_SHUTDOWN()                                                                                               \
	printf("\nfailed tests: %i out of %i\n", failed, tests);                                                           \
	if (failed != 0) {                                                                                                 \
		return 1;                                                                                                      \
	}                                                                                                                  \
	return 0

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

#endif
