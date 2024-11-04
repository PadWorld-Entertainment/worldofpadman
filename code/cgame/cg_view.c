/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
// cg_view.c -- setup all the parameters (position, angle, etc)
// for a 3D rendering
#include "cg_local.h"

/*
=============================================================================

  MODEL TESTING

The viewthing and gun positioning tools from Q2 have been integrated and
enhanced into a single model testing facility.

Model viewing can begin with either "testmodel <modelname>" or "testgun <modelname>".

The names must be the full pathname after the basedir, like
"models/weapons/v_launch/tris" or "players/male/tris"

Testmodel will create a fake entity 100 units in front of the current view
position, directly facing the viewer.  It will remain immobile, so you can
move around it to view it from different angles.

Testgun will cause the model to follow the player around and suppress the real
view weapon model.  The default frame 0 of most guns is completely off screen,
so you will probably have to cycle a couple frames to see it.

"nextframe", "prevframe", "nextskin", and "prevskin" commands will change the
frame or skin of the testmodel.  These are bound to F5, F6, F7, and F8 in
q3default.cfg.

If a gun is being tested, the "gun_x", "gun_y", and "gun_z" variables will let
you adjust the positioning.

Note that none of the model testing features update while the game is paused, so
it may be convenient to test with deathmatch set to 1 so that bringing down the
console doesn't pause the game.

=============================================================================
*/

/*
=================
CG_TestModel_f

Creates an entity in front of the current position, which
can then be moved around
=================
*/
void CG_TestModel_f(void) {
	vec3_t angles;

	cg.testGun = qfalse;
	memset(&cg.testModelEntity, 0, sizeof(cg.testModelEntity));
	if (trap_Argc() < 2) {
		return;
	}

	Q_strncpyz(cg.testModelName, CG_Argv(1), MAX_QPATH);
	cg.testModelEntity.hModel = trap_R_RegisterModel(cg.testModelName);

	if (trap_Argc() == 3) {
		cg.testModelEntity.backlerp = atof(CG_Argv(2));
		cg.testModelEntity.frame = 1;
		cg.testModelEntity.oldframe = 0;
	}
	if (!cg.testModelEntity.hModel) {
		CG_Printf("Can't register model\n");
		return;
	}

	VectorMA(cg.refdef.vieworg, 100, cg.refdef.viewaxis[0], cg.testModelEntity.origin);

	angles[PITCH] = 0;
	angles[YAW] = 180 + cg.refdefViewAngles[1];
	angles[ROLL] = 0;

	AnglesToAxis(angles, cg.testModelEntity.axis);
}

/*
=================
CG_TestGun_f

Replaces the current view weapon with the given model
=================
*/
void CG_TestGun_f(void) {
	CG_TestModel_f();
	cg.testGun = qtrue;
	cg.testModelEntity.renderfx = RF_MINLIGHT | RF_DEPTHHACK | RF_FIRST_PERSON;
}

void CG_TestModelNextFrame_f(void) {
	cg.testModelEntity.frame++;
	CG_Printf("frame %i\n", cg.testModelEntity.frame);
}

void CG_TestModelPrevFrame_f(void) {
	cg.testModelEntity.frame--;
	if (cg.testModelEntity.frame < 0) {
		cg.testModelEntity.frame = 0;
	}
	CG_Printf("frame %i\n", cg.testModelEntity.frame);
}

void CG_TestModelNextSkin_f(void) {
	cg.testModelEntity.skinNum++;
	CG_Printf("skin %i\n", cg.testModelEntity.skinNum);
}

void CG_TestModelPrevSkin_f(void) {
	cg.testModelEntity.skinNum--;
	if (cg.testModelEntity.skinNum < 0) {
		cg.testModelEntity.skinNum = 0;
	}
	CG_Printf("skin %i\n", cg.testModelEntity.skinNum);
}

static void CG_AddTestModel(void) {
	int i;

	// re-register the model, because the level may have changed
	cg.testModelEntity.hModel = trap_R_RegisterModel(cg.testModelName);
	if (!cg.testModelEntity.hModel) {
		CG_Printf("Can't register model\n");
		return;
	}

	// if testing a gun, set the origin relative to the view origin
	if (cg.testGun) {
		VectorCopy(cg.refdef.vieworg, cg.testModelEntity.origin);
		VectorCopy(cg.refdef.viewaxis[0], cg.testModelEntity.axis[0]);
		VectorCopy(cg.refdef.viewaxis[1], cg.testModelEntity.axis[1]);
		VectorCopy(cg.refdef.viewaxis[2], cg.testModelEntity.axis[2]);

		// allow the position to be adjusted
		for (i = 0; i < 3; i++) {
			cg.testModelEntity.origin[i] += cg.refdef.viewaxis[0][i] * cg_gun_x.value;
			cg.testModelEntity.origin[i] += cg.refdef.viewaxis[1][i] * cg_gun_y.value;
			cg.testModelEntity.origin[i] += cg.refdef.viewaxis[2][i] * cg_gun_z.value;
		}
	}

	trap_R_AddRefEntityToScene(&cg.testModelEntity);
}

//============================================================================

/*
=================
CG_CalcVrect

Sets the coordinates of the rendered window
=================
*/
static void CG_CalcVrect(void) {
	int size;

	// the intermission should always be full screen
	if (cg.snap->ps.pm_type == PM_INTERMISSION) {
		size = 100;
	} else {
		// bound normal viewsize
		if (cg_viewsize.integer < 30) {
			trap_Cvar_Set("cg_viewsize","30");
			size = 30;
		} else if (cg_viewsize.integer > 100) {
			trap_Cvar_Set("cg_viewsize","100");
			size = 100;
		} else {
			size = cg_viewsize.integer;
		}

	}
	cg.refdef.width = cgs.glconfig.vidWidth * size / 100;
	cg.refdef.width &= ~1;

	cg.refdef.height = cgs.glconfig.vidHeight * size / 100;
	cg.refdef.height &= ~1;

	cg.refdef.x = (cgs.glconfig.vidWidth - cg.refdef.width) / 2;
	cg.refdef.y = (cgs.glconfig.vidHeight - cg.refdef.height) / 2;
}

//==============================================================================

/*
===============
CG_OffsetThirdPersonView

===============
*/
#define FOCUS_DISTANCE 512
/// it seems a min distance of 20 looks ok for most models ... setting it higher could be cheat-prone (because it could
/// be abused to look through walls)
static const float MIN_3RD_DIST = 20.0f;
static void CG_OffsetThirdPersonView(void) {
	vec3_t forward, right, up;
	vec3_t view;
	vec3_t focusAngles;
	trace_t trace;
	static vec3_t mins = {-4, -4, -4};
	static vec3_t maxs = {4, 4, 4};
	vec3_t focusPoint;
	float focusDist;
	float forwardScale, sideScale;

	cg.refdef.vieworg[2] += cg.predictedPlayerState.viewheight;

	VectorCopy(cg.refdefViewAngles, focusAngles);

	// if dead, look at killer
	if (cg.predictedPlayerState.stats[STAT_HEALTH] <= 0 &&
		(cgs.gametype != GT_LPS || cg.snap->ps.stats[STAT_LIVESLEFT] >= 0)) {
		focusAngles[YAW] = cg.predictedPlayerState.stats[STAT_DEAD_YAW];
		cg.refdefViewAngles[YAW] = cg.predictedPlayerState.stats[STAT_DEAD_YAW];
	}

	if (focusAngles[PITCH] > 45) {
		focusAngles[PITCH] = 45; // don't go too far overhead
	}
	AngleVectors(focusAngles, forward, NULL, NULL);

	VectorMA(cg.refdef.vieworg, FOCUS_DISTANCE, forward, focusPoint);

	VectorCopy(cg.refdef.vieworg, view);

	view[2] += 8;

	cg.refdefViewAngles[PITCH] *= 0.5;

	AngleVectors(cg.refdefViewAngles, forward, right, up);

	forwardScale = cos(cg_thirdPersonAngle.value / 180 * M_PI);
	sideScale = sin(cg_thirdPersonAngle.value / 180 * M_PI);
	VectorMA(view, -cg_thirdPersonRange.value * forwardScale, forward, view);
	VectorMA(view, -cg_thirdPersonRange.value * sideScale, right, view);

	// trace a ray from the origin to the viewpoint to make sure the view isn't
	// in a solid block.  Use an 8 by 8 block to prevent the view from near clipping anything

	if (!cg_cameraMode.integer) {
		CG_Trace(&trace, cg.refdef.vieworg, mins, maxs, view, cg.predictedPlayerState.clientNum, MASK_SOLID);

		if (trace.fraction != 1.0) {
			VectorCopy(trace.endpos, view);
			view[2] += (1.0 - trace.fraction) * 32;
			// try another trace to this position, because a tunnel may have the ceiling
			// close enough that this is poking out

			CG_Trace(&trace, cg.refdef.vieworg, mins, maxs, view, cg.predictedPlayerState.clientNum, MASK_SOLID);
			VectorCopy(trace.endpos, view);

			if (DistanceSquared(cg.refdef.vieworg, view) < MIN_3RD_DIST * MIN_3RD_DIST) {
				VectorCopy(cg.refdef.vieworg, view);
				view[2] += 8;
				VectorMA(view, -MIN_3RD_DIST * forwardScale, forward, view);
				VectorMA(view, -MIN_3RD_DIST * sideScale, right, view);
			}
		}
	}

	VectorCopy(view, cg.refdef.vieworg);

	// select pitch to look at focus point from vieword
	VectorSubtract(focusPoint, cg.refdef.vieworg, focusPoint);
	focusDist = sqrt(focusPoint[0] * focusPoint[0] + focusPoint[1] * focusPoint[1]);
	if (focusDist < 1) {
		focusDist = 1; // should never happen
	}
	cg.refdefViewAngles[PITCH] = -180 / M_PI * atan2(focusPoint[2], focusDist);
	cg.refdefViewAngles[YAW] -= cg_thirdPersonAngle.value;
}

// this causes a compiler bug on mac MrC compiler
static void CG_StepOffset(void) {
	int timeDelta;

	// smooth out stair climbing
	timeDelta = cg.time - cg.stepTime;
	if (timeDelta < STEP_TIME) {
		cg.refdef.vieworg[2] -= cg.stepChange * (STEP_TIME - timeDelta) / STEP_TIME;
	}
}

/*
===============
CG_OffsetFirstPersonView

===============
*/
static void CG_OffsetFirstPersonView(void) {
	float *origin;
	float *angles;
	float bob;
	float ratio;
	float delta;
	float speed;
	float f;
	vec3_t predictedVelocity;
	int timeDelta;

	if (cg.snap->ps.pm_type == PM_INTERMISSION) {
		return;
	}

	origin = cg.refdef.vieworg;
	angles = cg.refdefViewAngles;

	// if dead, fix the angle and don't add any kick
	if (cg.snap->ps.stats[STAT_HEALTH] <= 0 && (cgs.gametype != GT_LPS || cg.snap->ps.stats[STAT_LIVESLEFT] >= 0)) {
		angles[ROLL] = 40;
		angles[PITCH] = -15;
		angles[YAW] = cg.snap->ps.stats[STAT_DEAD_YAW];
		origin[2] += cg.predictedPlayerState.viewheight;
		return;
	}

	// add angles based on damage kick
	if (cg.damageTime) {
		ratio = cg.time - cg.damageTime;
		if (ratio < DAMAGE_DEFLECT_TIME) {
			ratio /= DAMAGE_DEFLECT_TIME;
			angles[PITCH] += ratio * cg.v_dmg_pitch;
			angles[ROLL] += ratio * cg.v_dmg_roll;
		} else {
			ratio = 1.0 - (ratio - DAMAGE_DEFLECT_TIME) / DAMAGE_RETURN_TIME;
			if (ratio > 0) {
				angles[PITCH] += ratio * cg.v_dmg_pitch;
				angles[ROLL] += ratio * cg.v_dmg_roll;
			}
		}
	}

	// add pitch based on fall kick
#if 0
	ratio = (cg.time - cg.landTime) / FALL_TIME;
	if (ratio < 0)
		ratio = 0;
	angles[PITCH] += ratio * cg.fall_value;
#endif

	// add angles based on velocity
	VectorCopy(cg.predictedPlayerState.velocity, predictedVelocity);

	delta = DotProduct(predictedVelocity, cg.refdef.viewaxis[0]);
	angles[PITCH] += delta * cg_runpitch.value;

	delta = DotProduct(predictedVelocity, cg.refdef.viewaxis[1]);
	angles[ROLL] -= delta * cg_runroll.value;

	// add angles based on bob

	// make sure the bob is visible even at low speeds
	speed = cg.xyspeed > 200 ? cg.xyspeed : 200;

	delta = cg.bobfracsin * cg_bobpitch.value * speed;
	if (cg.predictedPlayerState.pm_flags & PMF_DUCKED)
		delta *= 3; // crouching
	angles[PITCH] += delta;
	delta = cg.bobfracsin * cg_bobroll.value * speed;
	if (cg.predictedPlayerState.pm_flags & PMF_DUCKED)
		delta *= 3; // crouching accentuates roll
	if (cg.bobcycle & 1)
		delta = -delta;
	angles[ROLL] += delta;

	//===================================

	// add view height
	origin[2] += cg.predictedPlayerState.viewheight;

	// smooth out duck height changes
	timeDelta = cg.time - cg.duckTime;
	if (timeDelta < DUCK_TIME) {
		cg.refdef.vieworg[2] -= cg.duckChange * (DUCK_TIME - timeDelta) / DUCK_TIME;
	}

	// add bob height
	bob = cg.bobfracsin * cg.xyspeed * cg_bobup.value;
	if (bob > 6) {
		bob = 6;
	}

	origin[2] += bob;

	// add fall height
	delta = cg.time - cg.landTime;
	if (delta < LAND_DEFLECT_TIME) {
		f = delta / LAND_DEFLECT_TIME;
		cg.refdef.vieworg[2] += cg.landChange * f;
	} else if (delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME) {
		delta -= LAND_DEFLECT_TIME;
		f = 1.0 - (delta / LAND_RETURN_TIME);
		cg.refdef.vieworg[2] += cg.landChange * f;
	}

	// add step offset
	CG_StepOffset();

	// pivot the eye based on a neck length
#if 0
	{
#define NECK_LENGTH 8
		vec3_t forward, up;

		cg.refdef.vieworg[2] -= NECK_LENGTH;
		AngleVectors(cg.refdefViewAngles, forward, NULL, up);
		VectorMA(cg.refdef.vieworg, 3, forward, cg.refdef.vieworg);
		VectorMA(cg.refdef.vieworg, NECK_LENGTH, up, cg.refdef.vieworg);
	}
#endif
}

//======================================================================

void CG_ZoomDown_f(void) {

	if (cg.snap == NULL)
		return;

	if (cg.zoomedkeydown)
		return;

	if (cg.zoomed) {
		cg.zoomed = qfalse;
		if (cg.wasThirdPerson) {
			trap_Cvar_Set("cg_thirdPerson", "1");
			cg.wasThirdPerson = qfalse;
		}
		cg.zoomTime = cg.time;
		return;
	}

	if (cg.snap->ps.weapon != WP_SPLASHER && cg.snap->ps.weapon != WP_KMA97)
		return;

	if (cg.snap->ps.pm_flags & PMF_FOLLOW || (cgs.gametype == GT_LPS && cg.snap->ps.stats[STAT_LIVESLEFT] <= 0))
		return;

	if (cg.renderingThirdPerson) {
		trap_Cvar_Set("cg_thirdPerson", "0");
		cg.wasThirdPerson = qtrue;
	}

	//	trap_S_StartLocalSound(cgs.media.zoomsound, CHAN_LOCAL_SOUND);
	cg.zoomSoundStat = 1;

	cg.zoomedkeydown = qtrue;
	cg.zoomed = qtrue;
	cg.zoomTime = cg.time;
}

void CG_ZoomUp_f(void) {
	if (cg.snap == NULL)
		return;

	cg.zoomSoundStat = 0; // stop sound
	cg.zoomedkeydown = qfalse;
	if (cg.snap->ps.weapon == WP_KMA97) {
		cg.zoomed = qfalse;
		if (cg.wasThirdPerson) {
			trap_Cvar_Set("cg_thirdPerson", "1");
			cg.wasThirdPerson = qfalse;
		}
	}
}

/*
====================
CG_CalcFov

Fixed fov at intermissions, otherwise account for fov variable and zooms.
====================
*/
#define WAVE_AMPLITUDE 1
#define WAVE_FREQUENCY 0.4

static int CG_CalcFov(void) {
	float x;
	float phase;
	float v;
	int contents;
	float fov_x, fov_y;
	float zoomFov;
	float f;
	int inwater;

	if (cg.predictedPlayerState.pm_type == PM_INTERMISSION) {
		// if in intermission, use a fixed value
		fov_x = 90;
	} else {
		// user selectable
		if (cgs.dmflags & DF_FIXED_FOV) {
			// dmflag to prevent wide fov for all clients
			fov_x = 90;
		} else {
			fov_x = cg_fov.value;
			if (fov_x < 1) {
				fov_x = 1;
			} else if (fov_x > 105) {
				fov_x = 105;
			}
		}

		// account for zooms
		zoomFov = cg_zoomFov.value;
		if (zoomFov < 1) {
			zoomFov = 1;
		} else if (zoomFov > 160) {
			zoomFov = 160;
		}

		zoomFov = 5;

		if (cg.zoomed) {
			if (cg.snap->ps.weapon == WP_SPLASHER) {
				if (cg.zoomedkeydown) {
					f = (cg.time - cg.zoomTime) / 2000.0f;
					cg.zoomfactor = f;
					if (cg.zoomfactor > 1.0f)
						cg.zoomfactor = 1.0f;
				}

				fov_x = fov_x + cg.zoomfactor * (zoomFov - fov_x);
			} else if (cg.snap->ps.weapon == WP_KMA97) {
				zoomFov = 20.0f;
				f = (cg.time - cg.zoomTime) / (float)ZOOM_TIME;
				if (f > 1.0f) {
					fov_x = zoomFov;
				} else {
					fov_x = fov_x + f * (zoomFov - fov_x);
				}
			}
		} else {
			f = (cg.time - cg.zoomTime) / (float)ZOOM_TIME;
			if (f <= 1.0f) {
				fov_x = zoomFov + f * (fov_x - zoomFov);
			}
		}
	}

	if (cg_fovAspectAdjust.integer) {
		// Based on LordHavoc's code for Darkplaces
		// http://www.quakeworld.nu/forum/topic/53/what-does-your-qw-look-like/page/30
		const float baseAspect = 0.75f; // 3/4
		const float aspect = (float)cg.refdef.width / (float)cg.refdef.height;
		const float desiredFov = fov_x;

		fov_x = atan2(tan(desiredFov * M_PI / 360.0f) * baseAspect * aspect, 1) * 360.0f / M_PI;
	}

	x = cg.refdef.width / tan(fov_x / 360 * M_PI);
	fov_y = atan2(cg.refdef.height, x);
	fov_y = fov_y * 360 / M_PI;

	// warp if underwater
	contents = CG_PointContents(cg.refdef.vieworg, -1);
	if (contents & (CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA)) {
		phase = cg.time / 1000.0 * WAVE_FREQUENCY * M_PI * 2;
		v = WAVE_AMPLITUDE * sin(phase);
		fov_x += v;
		fov_y -= v;
		inwater = qtrue;
	} else {
		inwater = qfalse;
	}

	if (cg.snap->ps.powerups[PW_SPEEDY]) {
		if (cg.xyspeed > 500.0f) {
			if (cg.speedyeffect < 1.5f)
				cg.speedyeffect += (float)cg.frametime * 0.001f;
		} else {
			if (cg.speedyeffect > 1.0f)
				cg.speedyeffect -= (float)cg.frametime * 0.001f;
			else if (cg.speedyeffect < 1.0f)
				cg.speedyeffect = 1.0f;
		}

		fov_x *= cg.speedyeffect;
		fov_y *= cg.speedyeffect;

		/* erster versuch:
				float tmpf;

				tmpf = cg.snap->ps.velocity[0]*cg.refdef.viewaxis[0][0]+
					cg.snap->ps.velocity[1]*cg.refdef.viewaxis[0][1]+
					cg.snap->ps.velocity[2]*cg.refdef.viewaxis[0][2];

				if(tmpf>640.0f)
				{
					fov_x*=1.5f;
					fov_y*=1.5f;
				}
				else if(tmpf>320.0f)
				{
					fov_x*=(tmpf+320.0f)/640.0f;
					fov_y*=(tmpf+320.0f)/640.0f;
				}
				else if(tmpf<-640.0f)
				{
					fov_x*=0.5f;
					fov_y*=0.5f;
				}
				else if(tmpf<-320.0f)
				{
					fov_x*=(960.0f+tmpf)/640.0f;
					fov_y*=(960.0f+tmpf)/640.0f;
				}
		*/
	} else
		cg.speedyeffect = 1.0f; // damit die waffenposition stimmt ;)

	// set it
	cg.refdef.fov_x = fov_x;
	cg.refdef.fov_y = fov_y;

	if (!cg.zoomed) {
		cg.zoomSensitivity = 1;
	} else {
		cg.zoomSensitivity = cg.refdef.fov_y / 75.0;
	}

	return inwater;
}

/*
===============
CG_DamageBlendBlob

===============
*/
static void CG_DamageBlendBlob(void) {
#if 0
	int t;
	int maxTime;
	refEntity_t ent;

	if (!cg.damageValue) {
		return;
	}

	// if (cg.cameraMode) {
	//	return;
	// }

	// ragePro systems can't fade blends, so don't obscure the screen
	if (cgs.glconfig.hardwareType == GLHW_RAGEPRO) {
		return;
	}

	maxTime = DAMAGE_TIME;
	t = cg.time - cg.damageTime;
	if (t <= 0 || t >= maxTime) {
		return;
	}

	memset(&ent, 0, sizeof(ent));
	ent.reType = RT_SPRITE;
	ent.renderfx = RF_FIRST_PERSON;

	VectorMA(cg.refdef.vieworg, 8, cg.refdef.viewaxis[0], ent.origin);
	VectorMA(ent.origin, cg.damageX * -8, cg.refdef.viewaxis[1], ent.origin);
	VectorMA(ent.origin, cg.damageY * 8, cg.refdef.viewaxis[2], ent.origin);

	ent.radius = cg.damageValue * 3;
	ent.customShader = cgs.media.viewBloodShader;
	ent.shaderRGBA[0] = 255;
	ent.shaderRGBA[1] = 255;
	ent.shaderRGBA[2] = 255;
	ent.shaderRGBA[3] = 200 * (1.0 - ((float)t / maxTime));
	trap_R_AddRefEntityToScene(&ent);
#endif
}

/*
===============
CG_CalcViewValues

Sets cg.refdef view values
===============
*/
static int CG_CalcViewValues(void) {
	playerState_t *ps;

	memset(&cg.refdef, 0, sizeof(cg.refdef));

	// calculate size of 3D view
	CG_CalcVrect();

	ps = &cg.predictedPlayerState;
#if 0
	if (cg.cameraMode) {
		vec3_t origin, angles;
		if (trap_getCameraInfo(cg.time, &origin, &angles)) {
			VectorCopy(origin, cg.refdef.vieworg);
			angles[ROLL] = 0;
			VectorCopy(angles, cg.refdefViewAngles);
			AnglesToAxis(cg.refdefViewAngles, cg.refdef.viewaxis);
			return CG_CalcFov();
		} else {
			cg.cameraMode = qfalse;
		}
	}
#endif
	if (cg.Cam) {
		float fov = cg_fov.value;
		float x;

		VectorCopy(cg.CamPos, cg.refdef.vieworg);
		VectorCopy(cg.CamAngles, cg.refdefViewAngles);

		AnglesToAxis(cg.refdefViewAngles, cg.refdef.viewaxis);

		x = cg.refdef.width / tan(fov / 360 * M_PI);
		cg.refdef.fov_y = atan2(cg.refdef.height, x);
		cg.refdef.fov_y = cg.refdef.fov_y * 360 / M_PI;
		cg.refdef.fov_x = fov;

		return 0;
	}

	// intermission view
	if (ps->pm_type == PM_INTERMISSION) {
		VectorCopy(ps->origin, cg.refdef.vieworg);
		VectorCopy(ps->viewangles, cg.refdefViewAngles);
		AnglesToAxis(cg.refdefViewAngles, cg.refdef.viewaxis);
		return CG_CalcFov();
	}
	cg.playedIntermissionMsg = qfalse;

	cg.bobcycle = (ps->bobCycle & 128) >> 7;
	cg.bobfracsin = fabs(sin((ps->bobCycle & 127) / 127.0 * M_PI));
	cg.xyspeed = sqrt(ps->velocity[0] * ps->velocity[0] + ps->velocity[1] * ps->velocity[1]);

	VectorCopy(ps->origin, cg.refdef.vieworg);
	VectorCopy(ps->viewangles, cg.refdefViewAngles);

	if (cg_cameraOrbit.integer) {
		if (cg.time > cg.nextOrbitTime) {
			cg.nextOrbitTime = cg.time + cg_cameraOrbitDelay.integer;
			cg_thirdPersonAngle.value += cg_cameraOrbit.value;
		}
	}
	// add error decay
	if (cg_errorDecay.value > 0) {
		int t;
		float f;

		t = cg.time - cg.predictedErrorTime;
		f = (cg_errorDecay.value - t) / cg_errorDecay.value;
		if (f > 0 && f < 1) {
			VectorMA(cg.refdef.vieworg, f, cg.predictedError, cg.refdef.vieworg);
		} else {
			cg.predictedErrorTime = 0;
		}
	}

	if (cg.renderingThirdPerson) {
		// back away from character
		CG_OffsetThirdPersonView();
	} else {
		// offset for local bobbing and kicks
		CG_OffsetFirstPersonView();
	}

	// position eye relative to origin
	AnglesToAxis(cg.refdefViewAngles, cg.refdef.viewaxis);

	if (cg.hyperspace) {
		cg.refdef.rdflags |= RDF_NOWORLDMODEL | RDF_HYPERSPACE;
	}

	// field of view
	return CG_CalcFov();
}

/*
=====================
CG_PowerupTimerSounds
=====================
*/
static void CG_PowerupTimerSounds(void) {
	int i;
	int t;

	// powerup timers going away
	for (i = 0; i < MAX_POWERUPS; i++) {
		t = cg.snap->ps.powerups[i];
		if (t <= cg.time) {
			continue;
		}
		if (t - cg.time >= POWERUP_BLINKS * POWERUP_BLINK_TIME) {
			continue;
		}
		if ((t - cg.time) / POWERUP_BLINK_TIME != (t - cg.oldTime) / POWERUP_BLINK_TIME) {
			trap_S_StartSound(NULL, cg.snap->ps.clientNum, CHAN_ITEM, cgs.media.wearOffSound);
		}
	}
}

/*
=====================
CG_AddBufferedSound
=====================
*/
void CG_AddBufferedSound(sfxHandle_t sfx) {
	if (!sfx)
		return;
	cg.soundBuffer[cg.soundBufferIn] = sfx;
	cg.soundBufferIn = (cg.soundBufferIn + 1) % MAX_SOUNDBUFFER;
	if (cg.soundBufferIn == cg.soundBufferOut) {
		cg.soundBufferOut++;
	}
}

/*
=====================
CG_PlayBufferedSounds
=====================
*/
static void CG_PlayBufferedSounds(void) {
	if (cg.soundTime < cg.time) {
		if (cg.soundBufferOut != cg.soundBufferIn && cg.soundBuffer[cg.soundBufferOut]) {
			trap_S_StartLocalSound(cg.soundBuffer[cg.soundBufferOut], CHAN_ANNOUNCER);
			cg.soundBuffer[cg.soundBufferOut] = 0;
			cg.soundBufferOut = (cg.soundBufferOut + 1) % MAX_SOUNDBUFFER;
			cg.soundTime = cg.time + 750;
		}
	}
}

//=========================================================================

/*
=================
CG_DrawActiveFrame

Generates and draws a game scene and status information at the given time.
=================
*/
void CG_DrawActiveFrame(int serverTime, stereoFrame_t stereoView, qboolean demoPlayback) {
	int inwater;
	int lastSnapClientNum = -1;

	cg.time = serverTime;
	cg.millis = trap_Milliseconds();
	cg.demoPlayback = demoPlayback;

	// update cvars
	CG_UpdateCvars();

	// if we are only updating the screen as a loading
	// pacifier, don't even try to read snapshots
	if (cg.loadingprogress < 1.0f) {
		CG_DrawInformation();
		return;
	}

	if (!(cg.clientFrame & 0xff))
		// TODO: maybe we should not execute this every frame
		trap_SendConsoleCommand("wop_checkmusic\n");

	// any looped sounds will be respecified as entities
	// are added to the render list
	trap_S_ClearLoopingSounds(qfalse);

	// clear all the render lists
	trap_R_ClearScene();

	// save old playerstate client
	if (cg.snap)
		lastSnapClientNum = cg.snap->ps.clientNum;

	// set up cg.snap and possibly cg.nextSnap
	CG_ProcessSnapshots();

	// if we haven't received any snapshots yet, all
	// we can draw is the information screen
	if (!cg.snap || (cg.snap->snapFlags & SNAPFLAG_NOT_ACTIVE)) {
		CG_DrawInformation();
		return;
	}

	// spec switched view, adjust the glowskins
	if (lastSnapClientNum != cg.snap->ps.clientNum)
		CG_ForceModelChange();

	// let the client system know what our weapon and zoom settings are
	trap_SetUserCmdValue(cg.weaponSelect, cg.zoomSensitivity);

	// this counter will be bumped for every valid scene we generate
	cg.clientFrame++;

	if (cg.clientFrame == 1) // or maybe even a later frame
	{
		qtime_t qtime;

		trap_RealTime(&qtime);
		// random ist im ersten frame kein zufall
		trap_S_StartLocalSound(cgs.media.welcomeSound[(int)(qtime.tm_sec) % 3], CHAN_ANNOUNCER);
	}

	// update cg.predictedPlayerState
	CG_PredictPlayerState();

	// decide on third person view
	cg.renderingThirdPerson =
		cg_thirdPerson.integer ||
		((cg.snap->ps.stats[STAT_HEALTH] <= 0) && (cgs.gametype != GT_LPS || (cg.snap->ps.stats[STAT_LIVESLEFT] >= 0)));

	// build cg.refdef
	inwater = CG_CalcViewValues();

	if (cg.wopSky[0] != '\0') {
		poly_t poly;
		polyVert_t verts[4];
		vec3_t skyangles, skyf, skyu, skyr;

		memset(&poly, 0, sizeof(poly));
		memset(&verts, 0, sizeof(verts));
		poly.verts = verts;
		poly.numVerts = 4;

		verts[0].modulate[0] = verts[1].modulate[0] = verts[2].modulate[0] = verts[3].modulate[0] = 255;
		verts[0].modulate[1] = verts[1].modulate[1] = verts[2].modulate[1] = verts[3].modulate[1] = 255;
		verts[0].modulate[2] = verts[1].modulate[2] = verts[2].modulate[2] = verts[3].modulate[2] = 255;
		verts[0].modulate[3] = verts[1].modulate[3] = verts[2].modulate[3] = verts[3].modulate[3] = 255;

		verts[0].st[0] = 0;
		verts[0].st[1] = 0;

		verts[1].st[0] = 1;
		verts[1].st[1] = 0;

		verts[2].st[0] = 1;
		verts[2].st[1] = 1;

		verts[3].st[0] = 0;
		verts[3].st[1] = 1;

		skyangles[0] = cg.wopSky_Angles[0] * sin(cg.time * 0.0001f * cg.wopSky_TimeFactors[0]);
		skyangles[1] = cg.wopSky_Angles[1] * sin(cg.time * 0.0001f * cg.wopSky_TimeFactors[1]);
		skyangles[2] = cg.wopSky_Angles[2] * sin(cg.time * 0.0001f * cg.wopSky_TimeFactors[2]);
		AngleVectors(skyangles, skyf, skyr, skyu);

#define SKYABSTAND 256.0f
		verts[0].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyf[0] + SKYABSTAND * (-skyr[0] + skyu[0]);
		verts[0].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyf[1] + SKYABSTAND * (-skyr[1] + skyu[1]);
		verts[0].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyf[2] + SKYABSTAND * (-skyr[2] + skyu[2]);

		verts[1].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyf[0] + SKYABSTAND * (+skyr[0] + skyu[0]);
		verts[1].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyf[1] + SKYABSTAND * (+skyr[1] + skyu[1]);
		verts[1].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyf[2] + SKYABSTAND * (+skyr[2] + skyu[2]);

		verts[2].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyf[0] + SKYABSTAND * (+skyr[0] - skyu[0]);
		verts[2].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyf[1] + SKYABSTAND * (+skyr[1] - skyu[1]);
		verts[2].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyf[2] + SKYABSTAND * (+skyr[2] - skyu[2]);

		verts[3].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyf[0] + SKYABSTAND * (-skyr[0] - skyu[0]);
		verts[3].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyf[1] + SKYABSTAND * (-skyr[1] - skyu[1]);
		verts[3].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyf[2] + SKYABSTAND * (-skyr[2] - skyu[2]);

		poly.hShader = trap_R_RegisterShader(va("%s_ft", cg.wopSky));
		trap_R_AddPolyToScene(poly.hShader, poly.numVerts, poly.verts);

		verts[0].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyf[0] + SKYABSTAND * (+skyr[0] + skyu[0]);
		verts[0].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyf[1] + SKYABSTAND * (+skyr[1] + skyu[1]);
		verts[0].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyf[2] + SKYABSTAND * (+skyr[2] + skyu[2]);

		verts[1].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyf[0] + SKYABSTAND * (-skyr[0] + skyu[0]);
		verts[1].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyf[1] + SKYABSTAND * (-skyr[1] + skyu[1]);
		verts[1].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyf[2] + SKYABSTAND * (-skyr[2] + skyu[2]);

		verts[2].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyf[0] + SKYABSTAND * (-skyr[0] - skyu[0]);
		verts[2].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyf[1] + SKYABSTAND * (-skyr[1] - skyu[1]);
		verts[2].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyf[2] + SKYABSTAND * (-skyr[2] - skyu[2]);

		verts[3].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyf[0] + SKYABSTAND * (+skyr[0] - skyu[0]);
		verts[3].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyf[1] + SKYABSTAND * (+skyr[1] - skyu[1]);
		verts[3].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyf[2] + SKYABSTAND * (+skyr[2] - skyu[2]);

		poly.hShader = trap_R_RegisterShader(va("%s_bk", cg.wopSky));
		trap_R_AddPolyToScene(poly.hShader, poly.numVerts, poly.verts);

		verts[0].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyr[0] + SKYABSTAND * (-skyf[0] + skyu[0]);
		verts[0].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyr[1] + SKYABSTAND * (-skyf[1] + skyu[1]);
		verts[0].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyr[2] + SKYABSTAND * (-skyf[2] + skyu[2]);

		verts[1].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyr[0] + SKYABSTAND * (+skyf[0] + skyu[0]);
		verts[1].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyr[1] + SKYABSTAND * (+skyf[1] + skyu[1]);
		verts[1].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyr[2] + SKYABSTAND * (+skyf[2] + skyu[2]);

		verts[2].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyr[0] + SKYABSTAND * (+skyf[0] - skyu[0]);
		verts[2].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyr[1] + SKYABSTAND * (+skyf[1] - skyu[1]);
		verts[2].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyr[2] + SKYABSTAND * (+skyf[2] - skyu[2]);

		verts[3].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyr[0] + SKYABSTAND * (-skyf[0] - skyu[0]);
		verts[3].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyr[1] + SKYABSTAND * (-skyf[1] - skyu[1]);
		verts[3].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyr[2] + SKYABSTAND * (-skyf[2] - skyu[2]);

		poly.hShader = trap_R_RegisterShader(va("%s_rt", cg.wopSky));
		trap_R_AddPolyToScene(poly.hShader, poly.numVerts, poly.verts);

		verts[0].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyr[0] + SKYABSTAND * (+skyf[0] + skyu[0]);
		verts[0].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyr[1] + SKYABSTAND * (+skyf[1] + skyu[1]);
		verts[0].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyr[2] + SKYABSTAND * (+skyf[2] + skyu[2]);

		verts[1].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyr[0] + SKYABSTAND * (-skyf[0] + skyu[0]);
		verts[1].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyr[1] + SKYABSTAND * (-skyf[1] + skyu[1]);
		verts[1].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyr[2] + SKYABSTAND * (-skyf[2] + skyu[2]);

		verts[2].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyr[0] + SKYABSTAND * (-skyf[0] - skyu[0]);
		verts[2].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyr[1] + SKYABSTAND * (-skyf[1] - skyu[1]);
		verts[2].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyr[2] + SKYABSTAND * (-skyf[2] - skyu[2]);

		verts[3].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyr[0] + SKYABSTAND * (+skyf[0] - skyu[0]);
		verts[3].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyr[1] + SKYABSTAND * (+skyf[1] - skyu[1]);
		verts[3].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyr[2] + SKYABSTAND * (+skyf[2] - skyu[2]);

		poly.hShader = trap_R_RegisterShader(va("%s_lf", cg.wopSky));
		trap_R_AddPolyToScene(poly.hShader, poly.numVerts, poly.verts);

		// hr - vr - vl - hl ?
		verts[0].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyu[0] + SKYABSTAND * (-skyf[0] + skyr[0]);
		verts[0].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyu[1] + SKYABSTAND * (-skyf[1] + skyr[1]);
		verts[0].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyu[2] + SKYABSTAND * (-skyf[2] + skyr[2]);

		verts[1].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyu[0] + SKYABSTAND * (+skyf[0] + skyr[0]);
		verts[1].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyu[1] + SKYABSTAND * (+skyf[1] + skyr[1]);
		verts[1].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyu[2] + SKYABSTAND * (+skyf[2] + skyr[2]);

		verts[2].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyu[0] + SKYABSTAND * (+skyf[0] - skyr[0]);
		verts[2].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyu[1] + SKYABSTAND * (+skyf[1] - skyr[1]);
		verts[2].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyu[2] + SKYABSTAND * (+skyf[2] - skyr[2]);

		verts[3].xyz[0] = cg.refdef.vieworg[0] + (SKYABSTAND * 0.995f) * skyu[0] + SKYABSTAND * (-skyf[0] - skyr[0]);
		verts[3].xyz[1] = cg.refdef.vieworg[1] + (SKYABSTAND * 0.995f) * skyu[1] + SKYABSTAND * (-skyf[1] - skyr[1]);
		verts[3].xyz[2] = cg.refdef.vieworg[2] + (SKYABSTAND * 0.995f) * skyu[2] + SKYABSTAND * (-skyf[2] - skyr[2]);

		poly.hShader = trap_R_RegisterShader(va("%s_up", cg.wopSky));
		trap_R_AddPolyToScene(poly.hShader, poly.numVerts, poly.verts);

		verts[0].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyu[0] + SKYABSTAND * (-skyf[0] - skyr[0]);
		verts[0].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyu[1] + SKYABSTAND * (-skyf[1] - skyr[1]);
		verts[0].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyu[2] + SKYABSTAND * (-skyf[2] - skyr[2]);

		verts[1].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyu[0] + SKYABSTAND * (+skyf[0] - skyr[0]);
		verts[1].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyu[1] + SKYABSTAND * (+skyf[1] - skyr[1]);
		verts[1].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyu[2] + SKYABSTAND * (+skyf[2] - skyr[2]);

		verts[2].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyu[0] + SKYABSTAND * (+skyf[0] + skyr[0]);
		verts[2].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyu[1] + SKYABSTAND * (+skyf[1] + skyr[1]);
		verts[2].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyu[2] + SKYABSTAND * (+skyf[2] + skyr[2]);

		verts[3].xyz[0] = cg.refdef.vieworg[0] - (SKYABSTAND * 0.995f) * skyu[0] + SKYABSTAND * (-skyf[0] + skyr[0]);
		verts[3].xyz[1] = cg.refdef.vieworg[1] - (SKYABSTAND * 0.995f) * skyu[1] + SKYABSTAND * (-skyf[1] + skyr[1]);
		verts[3].xyz[2] = cg.refdef.vieworg[2] - (SKYABSTAND * 0.995f) * skyu[2] + SKYABSTAND * (-skyf[2] + skyr[2]);

		poly.hShader = trap_R_RegisterShader(va("%s_dn", cg.wopSky));
		trap_R_AddPolyToScene(poly.hShader, poly.numVerts, poly.verts);

		cg.refdef.rdflags |= RDF_NOWORLDMODEL;
		trap_R_RenderScene(&cg.refdef);
		cg.refdef.rdflags &= ~RDF_NOWORLDMODEL;
	}

	// first person blend blobs, done after AnglesToAxis
	if (!cg.renderingThirdPerson) {
		CG_DamageBlendBlob();
	}

	// build the render lists
	if (!cg.hyperspace) {
		CG_AddPacketEntities(); // after calcViewValues, so predicted player state is correct
		CG_AddMarks();
		CG_AddLocalEntities();
		AddLogosToScene();
		Main_SpriteParticles();
	}
	CG_AddViewWeapon(&cg.predictedPlayerState);

	// add buffered sounds
	CG_PlayBufferedSounds();

	if (cg.snap->ps.ammo[WP_SPRAYPISTOL] >= MAX_CARTRIDGES && cg.sprayyourcolortime < cg.time) {
		trap_S_StartLocalSound(cgs.media.sprayYourColorSound, cg.snap->ps.clientNum);
		cg.sprayyourcolortime = cg.time + 20000;
	}

	// finish up the rest of the refdef
	if (cg.testModelEntity.hModel) {
		CG_AddTestModel();
	}
	cg.refdef.time = cg.time;
	memcpy(cg.refdef.areamask, cg.snap->areamask, sizeof(cg.refdef.areamask));

	// warning sounds when powerup is wearing off
	CG_PowerupTimerSounds();

	// update audio positions
	trap_S_Respatialize(cg.snap->ps.clientNum, cg.refdef.vieworg, cg.refdef.viewaxis, inwater);

	// make sure the lagometerSample and frame timing isn't done twice when in stereo
	if (stereoView != STEREO_RIGHT) {
		cg.frametime = cg.time - cg.oldTime;
		if (cg.frametime < 0) {
			cg.frametime = 0;
		}
		cg.oldTime = cg.time;
		CG_AddLagometerFrameInfo();
	}
	if (cg_timescale.value != cg_timescaleFadeEnd.value) {
		if (cg_timescale.value < cg_timescaleFadeEnd.value) {
			cg_timescale.value += cg_timescaleFadeSpeed.value * ((float)cg.frametime) / 1000;
			if (cg_timescale.value > cg_timescaleFadeEnd.value)
				cg_timescale.value = cg_timescaleFadeEnd.value;
		} else {
			cg_timescale.value -= cg_timescaleFadeSpeed.value * ((float)cg.frametime) / 1000;
			if (cg_timescale.value < cg_timescaleFadeEnd.value)
				cg_timescale.value = cg_timescaleFadeEnd.value;
		}
		if (cg_timescaleFadeSpeed.value) {
			trap_Cvar_Set("timescale", va("%f", cg_timescale.value));
		}
	}

	// actually issue the rendering calls
	CG_DrawActive(stereoView);

	if (cg_stats.integer) {
		CG_Printf("cg.clientFrame:%i\n", cg.clientFrame);
	}

	CG_UpdateVoipTeamIDs();
	if (cg_drawVoiceNames.integer) {
		trap_GetVoipTimes(cg.lastVoiceTime);
	}
}
