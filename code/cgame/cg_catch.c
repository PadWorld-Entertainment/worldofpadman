/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of WorldOfPadman source code.

WorldOfPadman source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

WorldOfPadman source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with WorldOfPadman source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "cg_local.h"

qboolean CG_IsKillerDuck(const centity_t *cent) {
	return cent->currentState.eFlags & EF_KILLERDUCK;
}

void CG_DrawKillerduckIcon(const centity_t *cent) {
	vec3_t iconPos;
	float size, x, y;
	float squaredDist;

	if (!(cg_icons.integer & ICON_KILLERDUCK)) {
		return;
	}

	if (cgs.gametype != GT_CATCH) {
		return;
	}

	VectorCopy(cent->currentState.pos.trBase, iconPos);
	iconPos[2] += 50; // draw icon above visible killerduck origin
	squaredDist = CG_WorldToScreen(iconPos, &x, &y);
	if (!squaredDist) {
		return;
	}

	// don't draw the icon if the killerduck holdable is visible and close
	if (DistanceSquared(cg.refdef.vieworg, cent->currentState.pos.trBase) < Square(250.0f)) {
		trace_t trace;
		CG_Trace(&trace, cg.refdef.vieworg, NULL, NULL, cent->currentState.pos.trBase, cg.snap->ps.clientNum, MASK_OPAQUE);
		if (1.0 == trace.fraction) {
			return;
		}
	}

	size = Com_Clamp(0.5f, 1.0f, (1.0f / (sqrt(squaredDist) * 0.002f)));

	trap_R_SetColor(NULL);
	CG_DrawPic((x - 16.0f * size), (y - 16.0f * size), (32.0f * size), (32.0f * size), cgs.media.ctkdHintIconShader);
}
