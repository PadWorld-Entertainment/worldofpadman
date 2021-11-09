/*
===========================================================================
Copyright (C) 2010, 2011 Neil "haste" Toronto

This program is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://gnu.org/licenses/>.
===========================================================================
*/

#include "g_local.h"

/*
===========================
G_PredictPlayerClipVelocity

Slide on the impacting surface
===========================
*/

#define OVERCLIP 1.001f

static void G_PredictPlayerClipVelocity(vec3_t in, vec3_t normal, vec3_t out) {
	float backoff;

	/* Find the magnitude of the vector "in" along "normal" */
	backoff = DotProduct(in, normal);

	/* Tilt the plane a bit to avoid floating-point error issues */
	if (backoff < 0) {
		backoff *= OVERCLIP;
	} else {
		backoff /= OVERCLIP;
	}

	/* Slide along */
	VectorMA(in, -backoff, normal, out);
}

#undef OVERCLIP

/*
========================
G_PredictPlayerSlideMove

Advance the given entity frametime seconds, sliding as appropriate
========================
*/
#define MAX_CLIP_PLANES 5

static qboolean G_PredictPlayerSlideMove(gentity_t *ent, float frametime) {
	int bumpcount, numbumps;
	vec3_t dir;
	float d;
	int numplanes;
	vec3_t planes[MAX_CLIP_PLANES];
	vec3_t primal_velocity, velocity, origin;
	vec3_t clipVelocity;
	int i, j, k;
	trace_t trace;
	vec3_t end;
	float time_left;
	float into;
	vec3_t endVelocity;
	vec3_t endClipVelocity;

	numbumps = 4;

	VectorCopy(ent->s.pos.trDelta, primal_velocity);
	VectorCopy(primal_velocity, velocity);
	VectorCopy(ent->s.pos.trBase, origin);

	VectorCopy(velocity, endVelocity);

	time_left = frametime;

	numplanes = 0;

	for (bumpcount = 0; bumpcount < numbumps; bumpcount++) {

		/* Calculate position we are trying to move to */
		VectorMA(origin, time_left, velocity, end);

		/* See if we can make it there */
		trap_Trace(&trace, origin, ent->r.mins, ent->r.maxs, end, ent->s.number, ent->clipmask);

		if (trace.allsolid) {
			/* Entity is completely trapped in another solid */
			VectorClear(velocity);
			VectorCopy(origin, ent->s.pos.trBase);
			return qtrue;
		}

		if (trace.fraction > 0) {
			/* Actually covered some distance */
			VectorCopy(trace.endpos, origin);
		}

		if (1 == trace.fraction) {
			/* Moved the entire distance */
			break;
		}

		time_left -= (time_left * trace.fraction);

		if (numplanes >= MAX_CLIP_PLANES) {
			/* This shouldn't really happen */
			VectorClear(velocity);
			VectorCopy(origin, ent->s.pos.trBase);
			return qtrue;
		}

		/*
		   If this is the same plane we hit before, nudge velocity
		   out along it, which fixes some epsilon issues with
		   non-axial planes
		*/
		for (i = 0; i < numplanes; i++) {
			if (DotProduct(trace.plane.normal, planes[i]) > 0.99) {
				VectorAdd(trace.plane.normal, velocity, velocity);
				break;
			}
		}

		if (i < numplanes) {
			continue;
		}

		VectorCopy(trace.plane.normal, planes[numplanes]);
		numplanes++;

		/*
		   Modify velocity so it parallels all of the clip planes
		*/

		/* Find a plane that it enters */
		for (i = 0; i < numplanes; i++) {
			into = DotProduct(velocity, planes[i]);
			if (into >= 0.1) {
				/* Move doesn't interact with the plane */
				continue;
			}

			/* Slide along the plane */
			G_PredictPlayerClipVelocity(velocity, planes[i], clipVelocity);

			/* Slide along the plane */
			G_PredictPlayerClipVelocity(endVelocity, planes[i], endClipVelocity);

			/* See if there is a second plane that the new move enters */
			for (j = 0; j < numplanes; j++) {
				if (j == i) {
					continue;
				}

				if (DotProduct(clipVelocity, planes[j]) >= 0.1) {
					/* Move doesn't interact with the plane */
					continue;
				}

				/* Try clipping the move to the plane */
				G_PredictPlayerClipVelocity(clipVelocity, planes[j], clipVelocity);
				G_PredictPlayerClipVelocity(endClipVelocity, planes[j], endClipVelocity);

				/* See if it goes back into the first clip plane */
				if (DotProduct(clipVelocity, planes[i]) >= 0) {
					continue;
				}

				/* Slide the original velocity along the crease */
				CrossProduct(planes[i], planes[j], dir);
				VectorNormalize(dir);
				d = DotProduct(dir, velocity);
				VectorScale(dir, d, clipVelocity);

				CrossProduct(planes[i], planes[j], dir);
				VectorNormalize(dir);
				d = DotProduct(dir, endVelocity);
				VectorScale(dir, d, endClipVelocity);

				/* See if there is a third plane the the new move enters */
				for (k = 0; k < numplanes; k++) {
					if ((k == i) || (k == j)) {
						continue;
					}

					if (DotProduct(clipVelocity, planes[k]) >= 0.1) {
						/* Move doesn't interact with the plane */
						continue;
					}

					/* Stop dead at a tripple plane interaction */
					VectorClear(velocity);
					VectorCopy(origin, ent->s.pos.trBase);
					return qtrue;
				}
			}

			/* If we have fixed all interactions, try another move */
			VectorCopy(clipVelocity, velocity);
			VectorCopy(endClipVelocity, endVelocity);
			break;
		}
	}

	VectorCopy(endVelocity, velocity);
	VectorCopy(origin, ent->s.pos.trBase);

	return (bumpcount != 0);
}

#undef MAX_CLIP_PLANES

/*
============================
G_PredictPlayerStepSlideMove

Advance the given entity frametime seconds, stepping and sliding as appropriate
============================
*/
#define STEPSIZE 18

static void G_PredictPlayerStepSlideMove(gentity_t *ent, float frametime) {
	vec3_t start_o, start_v, down_o, down_v;
	vec3_t down, up;
	trace_t trace;
	float stepSize;

	VectorCopy(ent->s.pos.trBase, start_o);
	VectorCopy(ent->s.pos.trDelta, start_v);

	if (!G_PredictPlayerSlideMove(ent, frametime)) {
		/* Not clipped, so forget stepping */
		return;
	}

	VectorCopy(ent->s.pos.trBase, down_o);
	VectorCopy(ent->s.pos.trDelta, down_v);

	VectorCopy(start_o, up);
	up[2] += STEPSIZE;

	/* Test the player position if they were a stepheight higher */
	trap_Trace(&trace, start_o, ent->r.mins, ent->r.maxs, up, ent->s.number, ent->clipmask);
	if (trace.allsolid) {
		/* Can't step up */
		return;
	}

	stepSize = trace.endpos[2] - start_o[2];

	/* Try slidemove from this position */
	VectorCopy(trace.endpos, ent->s.pos.trBase);
	VectorCopy(start_v, ent->s.pos.trDelta);

	G_PredictPlayerSlideMove(ent, frametime);

	/* Push down the final amount */
	VectorCopy(ent->s.pos.trBase, down);
	down[2] -= stepSize;
	trap_Trace(&trace, ent->s.pos.trBase, ent->r.mins, ent->r.maxs, down, ent->s.number, ent->clipmask);
	if (!trace.allsolid) {
		VectorCopy(trace.endpos, ent->s.pos.trBase);
	}
	if (trace.fraction < 1.0) {
		G_PredictPlayerClipVelocity(ent->s.pos.trDelta, trace.plane.normal, ent->s.pos.trDelta);
	}
}

#undef STEPSIZE

/*
===================
G_PredictPlayerMove

Advance the given entity frametime seconds, stepping and sliding as appropriate

This is the entry point to the server-side-only prediction code
===================
*/
void G_PredictPlayerMove(gentity_t *ent, float frametime) {
	G_PredictPlayerStepSlideMove(ent, frametime);
}
