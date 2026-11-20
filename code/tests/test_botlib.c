#include "test_shared.h"
#include "../botlib/botlib.h"
#define AASINTERN
#include "../botlib/be_aas_file.h"
#undef AASINTERN
#include "../qcommon/cm_patch.h"

TESTS_GLOBALS();

static const botlib_export_t *botlib;

static void QDECL botPrint(int type, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
static void QDECL botPrint(int type, const char *fmt, ...) {
	va_list args;
	if (type < PRT_WARNING)
		return;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}

static void *botAlloc(size_t size) {
	return Z_Malloc(size);
}

static void *botHunkAlloc(size_t size) {
	return Hunk_Alloc(size, h_high);
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
	ADD_TEST(testAASCurveSubdivision);
	ADD_TEST(testAirplaneNavigationFile);
	botlib->BotLibShutdown();
	Com_Shutdown();
	TESTS_SHUTDOWN();
}
