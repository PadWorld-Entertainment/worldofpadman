/* GamePad top-down map, client half: the map replaces the TV mirror only during live play.
 * Off by default; enable with "set gamepad_map 1" (e.g. in autoexec.cfg). */

#include <gx2/display.h>
#include "qcommon/q_shared.h"
#include "qcommon/qcommon.h"
#include "client/client.h"
#include "../wiiu/wiiu_osk.h"
#include "../wiiu/wiiu_gamepad_map.h"

static cvar_t *gamepad_map;

/* Menus, console, chat and the OSK keep the mirror: the OSK is tapped through it. */
static const char *GamepadMap_BlockedBy(void)
{
	if (!gamepad_map->integer)
		return "cvar gamepad_map is 0";
	if (GX2GetSystemTVScanMode() == GX2_TV_SCAN_MODE_NONE)
		return "no TV connected";
	if (clc.state != CA_ACTIVE)
		return "not in a match";
	if (!cl.snap.valid)
		return "no valid snapshot";
	if (Key_GetCatcher() & (KEYCATCH_UI | KEYCATCH_CONSOLE | KEYCATCH_MESSAGE))
		return "menu, console or chat open";
	if (WiiU_OSK_IsActive())
		return "on-screen keyboard open";
	if (cl.snap.ps.pm_type == PM_INTERMISSION || cl.snap.ps.pm_type == PM_SPINTERMISSION)
		return "intermission";
	/* ps.clientNum is the followed player while spectating, matching the HUD face. */
	if (cl.snap.ps.clientNum < 0 || cl.snap.ps.clientNum >= MAX_CLIENTS)
		return "bad client number";
	return NULL;
}

void WiiU_GamepadMap_Frame(void)
{
	char model[MAX_QPATH], headModel[MAX_QPATH];
	const char *info, *blockedBy;
	int team;

	if (!gamepad_map)
		gamepad_map = Cvar_Get("gamepad_map", "0", 0);

	/* Built during the loading screen, so entering the match doesn't stall on it. */
	if (gamepad_map->integer && GX2GetSystemTVScanMode() != GX2_TV_SCAN_MODE_NONE &&
	    clc.state >= CA_LOADING && clc.state <= CA_ACTIVE) {
		const char *serverInfo = cl.gameState.stringData + cl.gameState.stringOffsets[CS_SERVERINFO];
		int gametype = atoi(Info_ValueForKey(serverInfo, "g_gametype"));
		qboolean teamItemsOnly = qfalse;

		GX2DrcMap_Prepare(gametype, teamItemsOnly);
	}

	blockedBy = GamepadMap_BlockedBy();
#ifdef WIIU_DEBUG_LOG
	{
		/* Logged on change only: CON_Print reopens log.txt for every line. */
		static const char *lastBlockedBy = "";

		if (blockedBy != lastBlockedBy) {
			lastBlockedBy = blockedBy;
			Com_Printf("GamePad map: client gate %s\n", blockedBy ? blockedBy : "open");
		}
	}
#endif
	if (blockedBy) {
		GX2DrcMap_SetClientFrame(qfalse, NULL, NULL, 0);
		return;
	}

	/* Info_ValueForKey hands back a shared static buffer, so copy each value out at once. */
	info = cl.gameState.stringData + cl.gameState.stringOffsets[CS_PLAYERS + cl.snap.ps.clientNum];
	Q_strncpyz(model, Info_ValueForKey(info, "model"), sizeof(model));
	Q_strncpyz(headModel, Info_ValueForKey(info, "hmodel"), sizeof(headModel));
	team = atoi(Info_ValueForKey(info, "t"));

	GX2DrcMap_SetClientFrame(qtrue, model, headModel, team);
}
