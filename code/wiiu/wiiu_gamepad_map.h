/* GamePad top-down map (cvar gamepad_map): the client half decides each frame whether the
 * map may replace the TV mirror, the renderer half (renderergx2/tr_gx2_drcmap.c) draws it. */

#ifndef WIIU_GAMEPAD_MAP_H
#define WIIU_GAMEPAD_MAP_H

#include "qcommon/q_shared.h"

/* Client half, once per frame from SCR_DrawScreenField after every other draw. */
void WiiU_GamepadMap_Frame(void);

/* Renderer half. model/headModel/team are the shown player's configstring values. */
void GX2DrcMap_SetClientFrame(qboolean active, const char *model, const char *headModel, int team);

/* Builds the top view for the loaded world if needed, pickups filtered like the server spawns them. */
void GX2DrcMap_Prepare(int gametype, qboolean teamItemsOnly);

#endif /* WIIU_GAMEPAD_MAP_H */
