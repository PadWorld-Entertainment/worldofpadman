#include "test_shared.h"
#include "../botlib/botlib.h"
#define AASINTERN
#include "../botlib/be_aas_file.h"
#undef AASINTERN
#include "../botlib/be_ai_chat.h"
#include "../game/match.h"
#include "../qcommon/cm_patch.h"

TESTS_GLOBALS();

static const botlib_export_t *botlib;
static char lastCommand[MAX_STRING_CHARS];
static int lastClient;

static void QDECL botPrint(int type, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
static void QDECL botPrint(int type, const char *fmt, ...) {
	va_list args;
	if (type < PRT_WARNING)
		return;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}

static void botCommand(int client, const char *command) {
	lastClient = client;
	Q_strncpyz(lastCommand, command, sizeof(lastCommand));
}

static void *botAlloc(size_t size) {
	return Z_Malloc(size);
}

static void *botHunkAlloc(size_t size) {
	return Hunk_Alloc(size, h_high);
}

static void testConsoleMessages(void) {
	bot_consolemessage_t message;
	const ai_export_t *ai = &botlib->ai;
	int handle, chat = ai->BotAllocChatState();
	ASSERT_EQ_INT(1, chat > 0);
	ai->BotQueueConsoleMessage(chat, CMS_NORMAL, "Padman entered the game");
	ai->BotQueueConsoleMessage(chat, CMS_CHAT, "Padman: hello");
	EXPECT_EQ_INT(2, ai->BotNumConsoleMessages(chat));
	handle = ai->BotNextConsoleMessage(chat, &message);
	ASSERT_EQ_INT(1, handle > 0);
	EXPECT_EQ_INT(CMS_NORMAL, message.type);
	EXPECT_EQ_STRING("Padman entered the game", message.message);
	ai->BotRemoveConsoleMessage(chat, handle);
	handle = ai->BotNextConsoleMessage(chat, &message);
	ASSERT_EQ_INT(1, handle > 0);
	EXPECT_EQ_INT(CMS_CHAT, message.type);
	EXPECT_EQ_STRING("Padman: hello", message.message);
	ai->BotRemoveConsoleMessage(chat, handle);
	EXPECT_EQ_INT(0, ai->BotNextConsoleMessage(chat, &message));
	ai->BotFreeChatState(chat);
}

static void testMatchVariables(void) {
	const ai_export_t *ai = &botlib->ai;
	bot_match_t match;
	char text[MAX_MESSAGE_SIZE], place[151], value[MAX_MESSAGE_SIZE];
	memset(place, 'a', sizeof(place) - 1);
	place[sizeof(place) - 1] = '\0';
	Com_sprintf(text, sizeof(text), EC "(Padman" EC ") (%s)" EC ": PadGirl collect Nipper\n", place);
	ASSERT_EQ_INT(qtrue, ai->BotFindMatch(text, &match, MTCONTEXT_INITIALTEAMCHAT));
	EXPECT_EQ_INT(MSG_GETITEM, match.type);
	EXPECT_EQ_INT(ST_ADDRESSED, match.subtype);
	ai->BotMatchVariable(&match, NETNAME, value, sizeof(value));
	EXPECT_EQ_STRING("Padman", value);
	ai->BotMatchVariable(&match, ADDRESSEE, value, sizeof(value));
	EXPECT_EQ_STRING("PadGirl", value);
	ai->BotMatchVariable(&match, ITEM, value, sizeof(value));
	EXPECT_EQ_STRING("Nipper", value);
	EXPECT_EQ_INT(qfalse, ai->BotFindMatch(text, &match, MTCONTEXT_MISC));
}

static void testInitialChat(void) {
	const ai_export_t *ai = &botlib->ai;
	int chat = ai->BotAllocChatState();
	char item[] = "Nipper";
	char longText[2 * MAX_MESSAGE_SIZE];
	ASSERT_EQ_INT(1, chat > 0);
	ASSERT_EQ_INT(BLERR_NOERROR, ai->BotLoadChatFile(chat, "bots/guys_t.c", "padman"));
	ai->BotSetChatName(chat, "Padman", 3);
	ai->BotInitialChat(chat, "getitem_start", 0, item, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	EXPECT_EQ_INT(1, ai->BotChatLength(chat) > 0);
	ai->BotEnterChat(chat, 2, CHAT_TELL);
	EXPECT_EQ_INT(3, lastClient);
	EXPECT_EQ_STRING("tell 2 Nipper roger that.", lastCommand);
	EXPECT_EQ_INT(0, ai->BotChatLength(chat));
	ai->BotInitialChat(chat, "getitem_start", 0, item, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ai->BotInitialChat(chat, "missing", 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	EXPECT_EQ_INT(0, ai->BotChatLength(chat));
	memset(longText, 'a', sizeof(longText) - 1);
	longText[sizeof(longText) - 1] = '\0';
	ai->BotInitialChat(chat, "order_response", 0, longText, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	EXPECT_EQ_INT(MAX_MESSAGE_SIZE - 1, ai->BotChatLength(chat));
	ai->BotFreeChatState(chat);
}

static void testTeamOrderMatches(void) {
	static const struct {
		const char *text;
		int type;
	} orders[] = {
		{"I am the leader", MSG_STARTTEAMLEADERSHIP}, {"I stop being the leader", MSG_STOPTEAMLEADERSHIP},
		{"follow me", MSG_FOLLOW}, {"defend the base", MSG_DEFEND}, {"get the lolly", MSG_GETLOLLY},
		{"camp here", MSG_CAMP}, {"roam", MSG_ROAM}, {"report", MSG_REPORT},
		{"give your cartridges to me", MSG_DROPCART},
		{"secure a balloon", MSG_GOFORBALLOON}, {"defend a balloon", MSG_DEFENDBALLOON},
		{"thaw a teammate", MSG_THAW}, {"thaw me", MSG_THAWME},
		{"what are you doing?", MSG_REPORT}, {"who is the team leader?", MSG_WHOISTEAMLAEDER},
		{"what is my command?", MSG_WHATISMYCOMMAND},
	};
	static const struct {
		const char *text;
		int type;
	} addressed[] = {
		{"defend the base", MSG_DEFEND}, {"get enemy lolly", MSG_GETLOLLY}, {"follow me", MSG_FOLLOW},
		{"camp here", MSG_CAMP}, {"roam", MSG_ROAM}, {"report", MSG_REPORT},
		{"give your cartridges to me.", MSG_DROPCART}, {"thaw me!", MSG_THAWME},
	};
	bot_match_t match;
	int i;
	for (i = 0; i < ARRAY_LEN(orders); i++) {
		const char *team = va(EC "(PadGirl" EC ") (Kitchen)" EC ": %s", orders[i].text);
		const char *tell = va(EC "[PadGirl" EC "]" EC ": %s", orders[i].text);
		ASSERT_EQ_INT(qtrue, botlib->ai.BotFindMatch(team, &match, MTCONTEXT_INITIALTEAMCHAT));
		EXPECT_EQ_INT(orders[i].type, match.type);
		ASSERT_EQ_INT(qtrue, botlib->ai.BotFindMatch(tell, &match, MTCONTEXT_INITIALTEAMCHAT));
		EXPECT_EQ_INT(orders[i].type, match.type);
	}
	EXPECT_EQ_INT(qfalse, botlib->ai.BotFindMatch("PadGirl: follow me", &match, MTCONTEXT_INITIALTEAMCHAT));
	for (i = 0; i < ARRAY_LEN(addressed); i++) {
		char addressee[MAX_MESSAGE_SIZE];
		const char *team = va(EC "(PadGirl" EC ")" EC ": Padman, %s", addressed[i].text);
		ASSERT_EQ_INT(qtrue, botlib->ai.BotFindMatch(team, &match, MTCONTEXT_INITIALTEAMCHAT));
		EXPECT_EQ_INT(addressed[i].type, match.type);
		EXPECT_EQ_INT(ST_ADDRESSED, match.subtype);
		botlib->ai.BotMatchVariable(&match, ADDRESSEE, addressee, sizeof(addressee));
		EXPECT_EQ_STRING("Padman,", addressee);
	}
}

static void testAASCurveSubdivision(void) {
	vec3_t points[9];
	patchCollide_t *coarse, *normal, *flat;
	int row, column;

	for (row = 0; row < 3; row++) {
		for (column = 0; column < 3; column++) {
			VectorSet(points[row * 3 + column], 64 * column, 64 * row, column == 1 ? 96 : 0);
		}
	}
	coarse = CM_GeneratePatchCollideWithError(3, 3, points, 64);
	normal = CM_GeneratePatchCollide(3, 3, points);
	EXPECT_EQ_INT(1, coarse->numFacets);
	EXPECT_EQ_INT(1, normal->numFacets > coarse->numFacets);

	for (row = 0; row < 3; row++)
		points[row * 3 + 1][2] = 0;
	flat = CM_GeneratePatchCollide(3, 3, points);
	EXPECT_EQ_INT(1, flat->numFacets);
}

static void testAirplaneNavigationFile(void) {
	void *bsp;
	const int length = FS_ReadFile("maps/wop_airplane.bsp", &bsp);
	int checksum;

	ASSERT_EQ_INT(1, length > 0);
	checksum = LittleLong(Com_BlockChecksum(bsp, length));
	FS_FreeFile(bsp);
	botlib->BotLibVarSet("sv_mapChecksum", va("%d", checksum));
	// Airplane has an unclustered area with a stale cluster-local number.
	EXPECT_EQ_INT(BLERR_NOERROR, AAS_LoadAASFile("maps/wop_airplane.aas"));
	botlib->BotLibVarSet("sv_mapChecksum", va("%d", checksum ^ 1));
	EXPECT_EQ_INT(BLERR_WRONGAASFILEVERSION, AAS_LoadAASFile("maps/wop_airplane.aas"));
}

int main(int argc, char *argv[]) {
	botlib_import_t imports = {0};
	TESTS_INIT();
	imports.Print = botPrint;
	imports.BotClientCommand = botCommand;
	imports.GetMemory = botAlloc;
	imports.FreeMemory = Z_Free;
	imports.HunkAlloc = botHunkAlloc;
	imports.FS_FOpenFile = FS_FOpenFileByMode;
	imports.FS_Read = FS_Read;
	imports.FS_FCloseFile = FS_FCloseFile;
	imports.FS_Seek = FS_Seek;
	botlib = GetBotLibAPI(BOTLIB_API_VERSION, &imports);
	if (!botlib)
		return 1;
	botlib->BotLibVarSet("maxclients", va("%d", MAX_CLIENTS));
	if (botlib->BotLibSetup() != BLERR_NOERROR)
		return 1;
	ADD_TEST(testConsoleMessages);
	ADD_TEST(testMatchVariables);
	ADD_TEST(testInitialChat);
	ADD_TEST(testTeamOrderMatches);
	ADD_TEST(testAASCurveSubdivision);
	ADD_TEST(testAirplaneNavigationFile);
	botlib->BotLibShutdown();
	Com_Shutdown();
	TESTS_SHUTDOWN();
}
