#include "q_shared.h"
#include "test_shared.h"
#include "../server/sv_discord.h"
#include "../server/sv_http.h"

TESTS_GLOBALS();

static char responseBuf[2048];

static void BufferWrite(unsigned char *buffer, int length) {
	const size_t l = strlen(responseBuf);
	if (l + length >= sizeof(responseBuf)) {
		return;
	}
	memcpy(&responseBuf[l], buffer, length);
}

static void testHTTPGET(void) {
	responseBuf[0] = '\0';
	HTTP_Init();
	HTTP_ExecuteGET("https://httpbin.org/get", "", BufferWrite);
	HTTP_Close();
	EXPECT_NE_STRING("", responseBuf);
}

static void testHTTPPOST(void) {
	responseBuf[0] = '\0';
	HTTP_Init();
	HTTP_ExecutePOST("https://httpbin.org/post", "Content-Type: application/json", "{}", BufferWrite);
	HTTP_Close();
	EXPECT_NE_STRING("", responseBuf);
}

static void testDiscord(void) {
	// when testing either export WOP_DISCORD_WEBHOOK_URL or change this value here with
	// a real webhook URL
	Cvar_Get("discord_webhook_url",
			 "https://discord.com/api/webhooks/xx/yy",
			 0);
	ASSERT_EQ_INT(0, DISCORD_Init());
	EXPECT_BETWEEN_INT(200, 299, DISCORD_SendMessage("testuser", "Test message"));
	DISCORD_Close();
}

static void testCommon(void) {
	EXPECT_EQ_STRING("file", COM_SkipPath("/path/to/file"));
	EXPECT_EQ_INT(0, Q_stricmp("foo", "FOo"));
}

int main(int argc, char *argv[]) {
	TESTS_INIT();

	ADD_TEST(testCommon);
	ADD_TEST(testHTTPPOST);
	ADD_TEST(testHTTPGET);
	ADD_DISABLED_TEST(testDiscord);

	Com_Shutdown();

	TESTS_SHUTDOWN();
}
