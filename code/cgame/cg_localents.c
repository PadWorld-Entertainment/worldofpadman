// Copyright (C) 1999-2000 Id Software, Inc.
//

// cg_localents.c -- every frame, generate renderer commands for locally
// processed entities, like smoke puffs, gibs, shells, etc.

#include "cg_local.h"

#define	MAX_LOCAL_ENTITIES	512
localEntity_t	cg_localEntities[MAX_LOCAL_ENTITIES];
localEntity_t	cg_activeLocalEntities;		// double linked list
localEntity_t	*cg_freeLocalEntities;		// single linked list

/*
===================
CG_InitLocalEntities

This is called at startup and for tournement restarts
===================
*/
void	CG_InitLocalEntities( void ) {
	int		i;

	memset( cg_localEntities, 0, sizeof( cg_localEntities ) );
	cg_activeLocalEntities.next = &cg_activeLocalEntities;
	cg_activeLocalEntities.prev = &cg_activeLocalEntities;
	cg_freeLocalEntities = cg_localEntities;
	for ( i = 0 ; i < MAX_LOCAL_ENTITIES - 1 ; i++ ) {
		cg_localEntities[i].next = &cg_localEntities[i+1];
	}
}


/*
==================
CG_FreeLocalEntity
==================
*/
void CG_FreeLocalEntity( localEntity_t *le ) {
	if ( !le->prev ) {
		CG_Error( "CG_FreeLocalEntity: not active" );
	}

	// remove from the doubly linked active list
	le->prev->next = le->next;
	le->next->prev = le->prev;

	// the free list is only singly linked
	le->next = cg_freeLocalEntities;
	cg_freeLocalEntities = le;
}

/*
===================
CG_AllocLocalEntity

Will allways succeed, even if it requires freeing an old active entity
===================
*/
localEntity_t	*CG_AllocLocalEntity( void ) {
	localEntity_t	*le;

	if ( !cg_freeLocalEntities ) {
		// no free entities, so free the one at the end of the chain
		// remove the oldest active entity
		CG_FreeLocalEntity( cg_activeLocalEntities.prev );
	}

	le = cg_freeLocalEntities;
	cg_freeLocalEntities = cg_freeLocalEntities->next;

	memset( le, 0, sizeof( *le ) );

	// link into the active list
	le->next = cg_activeLocalEntities.next;
	le->prev = &cg_activeLocalEntities;
	cg_activeLocalEntities.next->prev = le;
	cg_activeLocalEntities.next = le;
	return le;
}


/*
====================================================================================

FRAGMENT PROCESSING

A fragment localentity interacts with the environment in some way (hitting walls),
or generates more localentities along a trail.

====================================================================================
*/

/*
================
CG_BloodTrail

Leave expanding blood puffs behind gibs
================
*/
void CG_BloodTrail( localEntity_t *le ) {
	int		t;
	int		t2;
	int		step;
	vec3_t	newOrigin;
	localEntity_t	*blood;

	step = 150;
	t = step * ( (cg.time - cg.frametime + step ) / step );
	t2 = step * ( cg.time / step );

	for ( ; t <= t2; t += step ) {
		BG_EvaluateTrajectory( &le->pos, t, newOrigin );

		blood = CG_SmokePuff( newOrigin, vec3_origin, 
					  20,		// radius
					  1, 1, 1, 1,	// color
					  2000,		// trailTime
					  t,		// startTime
					  0,		// fadeInTime
					  0,		// flags
					  cgs.media.bloodTrailShader );
		// use the optimized version
		blood->leType = LE_FALL_SCALE_FADE;
		// drop a total of 40 units over its lifetime
		blood->pos.trDelta[2] = 40;
	}
}


/*
================
CG_FragmentBounceMark
================
*/
void CG_FragmentBounceMark( localEntity_t *le, trace_t *trace ) {
/*
	int			radius;

	if ( le->leMarkType == LEMT_BLOOD ) {

		radius = 16 + (rand()&31);
		CG_ImpactMark( cgs.media.bloodMarkShader, trace->endpos, trace->plane.normal, random()*360,
			1,1,1,1, qtrue, radius, qfalse );
	} else if ( le->leMarkType == LEMT_BURN ) {

		radius = 8 + (rand()&15);
		CG_ImpactMark( cgs.media.burnMarkShader, trace->endpos, trace->plane.normal, random()*360,
			1,1,1,1, qtrue, radius, qfalse );
	}


	// don't allow a fragment to make multiple marks, or they
	// pile up while settling
	le->leMarkType = LEMT_NONE;
*/
}

/*
================
CG_FragmentBounceSound
================
*/
void CG_FragmentBounceSound( localEntity_t *le, trace_t *trace ) {
	if ( le->leBounceSoundType == LEBS_BLOOD ) {
		// half the gibs will make splat sounds
		if ( rand() & 1 ) {
			int r = rand()&3;
			sfxHandle_t	s;

			if ( r == 0 ) {
				s = cgs.media.gibBounce1Sound;
			} else if ( r == 1 ) {
				s = cgs.media.gibBounce2Sound;
			} else {
				s = cgs.media.gibBounce3Sound;
			}
			trap_S_StartSound( trace->endpos, ENTITYNUM_WORLD, CHAN_AUTO, s );
		}
	} else if ( le->leBounceSoundType == LEBS_BRASS ) {

	}

	// don't allow a fragment to make multiple bounce sounds,
	// or it gets too noisy as they settle
	le->leBounceSoundType = LEBS_NONE;
}


/*
================
CG_ReflectVelocity
================
*/
void CG_ReflectVelocity( localEntity_t *le, trace_t *trace ) {
	vec3_t	velocity;
	float	dot;
	int		hitTime;

	// reflect the velocity on the trace plane
	hitTime = cg.time - cg.frametime + cg.frametime * trace->fraction;
	BG_EvaluateTrajectoryDelta( &le->pos, hitTime, velocity );
	dot = DotProduct( velocity, trace->plane.normal );
	VectorMA( velocity, -2*dot, trace->plane.normal, le->pos.trDelta );

	VectorScale( le->pos.trDelta, le->bounceFactor, le->pos.trDelta );

	VectorCopy( trace->endpos, le->pos.trBase );
	le->pos.trTime = cg.time;


	// check for stop, making sure that even on low FPS systems it doesn't bobble
	if ( trace->allsolid || 
		( trace->plane.normal[2] > 0 && 
		( le->pos.trDelta[2] < 40 || le->pos.trDelta[2] < -cg.frametime * le->pos.trDelta[2] ) ) ) {
		le->pos.trType = TR_STATIONARY;
	} else {

	}
}

/*
================
CG_AddFragment
================
*/
void CG_AddFragment( localEntity_t *le ) {
	vec3_t	newOrigin;
	trace_t	trace;

	if ( le->pos.trType == TR_STATIONARY ) {
		// sink into the ground if near the removal time
		int		t;
		float	oldZ;
		
		t = le->endTime - cg.time;
		if ( t < SINK_TIME ) {
			// we must use an explicit lighting origin, otherwise the
			// lighting would be lost as soon as the origin went
			// into the ground
			VectorCopy( le->refEntity.origin, le->refEntity.lightingOrigin );
			le->refEntity.renderfx |= RF_LIGHTING_ORIGIN;
			oldZ = le->refEntity.origin[2];
			le->refEntity.origin[2] -= 16 * ( 1.0 - (float)t / SINK_TIME );
			trap_R_AddRefEntityToScene( &le->refEntity );
			le->refEntity.origin[2] = oldZ;
		} else {
			trap_R_AddRefEntityToScene( &le->refEntity );
		}

		return;
	}

	// calculate new position
	BG_EvaluateTrajectory( &le->pos, cg.time, newOrigin );

	// trace a line from previous position to new position
	CG_Trace( &trace, le->refEntity.origin, NULL, NULL, newOrigin, -1, CONTENTS_SOLID );
	if ( trace.fraction == 1.0 ) {
		// still in free fall
		VectorCopy( newOrigin, le->refEntity.origin );

		if ( le->leFlags & LEF_TUMBLE ) {
			vec3_t angles;

			BG_EvaluateTrajectory( &le->angles, cg.time, angles );
			AnglesToAxis( angles, le->refEntity.axis );
		}

		trap_R_AddRefEntityToScene( &le->refEntity );

		// add a blood trail
		if ( le->leBounceSoundType == LEBS_BLOOD ) {
			CG_BloodTrail( le );
		}

		return;
	}

	// if it is in a nodrop zone, remove it
	// this keeps gibs from waiting at the bottom of pits of death
	// and floating levels
	if ( trap_CM_PointContents( trace.endpos, 0 ) & CONTENTS_NODROP ) {
		CG_FreeLocalEntity( le );
		return;
	}

	// leave a mark
	CG_FragmentBounceMark( le, &trace );

	// do a bouncy sound
	CG_FragmentBounceSound( le, &trace );

	// reflect the velocity on the trace plane
	CG_ReflectVelocity( le, &trace );

	trap_R_AddRefEntityToScene( &le->refEntity );
}

/*
=====================================================================

TRIVIAL LOCAL ENTITIES

These only do simple scaling or modulation before passing to the renderer
=====================================================================
*/

/*
====================
CG_AddFadeRGB
====================
*/
void CG_AddFadeRGB( localEntity_t *le ) {
	refEntity_t *re;
	float c;

	re = &le->refEntity;

	c = ( le->endTime - cg.time ) * le->lifeRate;
	c *= 0xff;

	re->shaderRGBA[0] = le->color[0] * c;
	re->shaderRGBA[1] = le->color[1] * c;
	re->shaderRGBA[2] = le->color[2] * c;
	re->shaderRGBA[3] = le->color[3] * c;

	trap_R_AddRefEntityToScene( re );
}

/*
====================
CG_AddSprayTrailFade
====================
*/
void CG_AddSprayTrailFade( localEntity_t *le ) {
	refEntity_t *re;
	float c;

	re = &le->refEntity;

	c = ( le->endTime - cg.time ) * le->lifeRate;
	c *= 0xff;

	re->shaderRGBA[3] = le->color[3] * c;

	re->radius = le->radius*(cg.time-le->startTime)/le->angles.trDuration;//(le->endTime-le->startTime);

	BG_EvaluateTrajectory( &le->pos, cg.time, re->origin );

	trap_R_AddRefEntityToScene( re );
}

/*
==================
CG_AddMoveScaleFade
==================
*/
static void CG_AddMoveScaleFade( localEntity_t *le ) {
	refEntity_t	*re;
	float		c;
	vec3_t		delta;
	float		len;

	re = &le->refEntity;

	if ( le->fadeInTime > le->startTime && cg.time < le->fadeInTime ) {
		// fade / grow time
		c = 1.0 - (float) ( le->fadeInTime - cg.time ) / ( le->fadeInTime - le->startTime );
	}
	else {
		// fade / grow time
		c = ( le->endTime - cg.time ) * le->lifeRate;
	}

	if ( re->hModel ) {
		if ( (le->leFlags & LEF_AXIS_ALIGNED) && le->radius ) {
			// it has a variing scale
			AxisClear( re->axis );
			AxisScale( re->axis, re->radius + le->radius * (cg.time - le->startTime) / (le->endTime - le->startTime), re->axis );
		}
		if ( !(le->leFlags & LEF_AXIS_ALIGNED) ) {
			// model looks in flight direction
			vec3_t	delta, angles;
			
			BG_EvaluateTrajectoryDelta( &le->pos, cg.time, delta );
			vectoangles( delta, angles );
			AnglesToAxis( angles, re->axis );
			//if ( re->radius || le->radius ) AxisScale( re->axis, re->radius + le->radius * (cg.time - le->startTime) / (le->endTime - le->startTime), re->axis );
		}
	}

	// HERBY: Particles
	re->shaderRGBA[0] = 0xff * c * le->color[0];
	re->shaderRGBA[1] = 0xff * c * le->color[1];
	re->shaderRGBA[2] = 0xff * c * le->color[2];
	re->shaderRGBA[3] = 0xff * c * le->color[3];

	if ( !( le->leFlags & LEF_PUFF_DONT_SCALE ) ) {
		re->radius = le->radius * ( 1.0 - c ) + 8;
	}

	BG_EvaluateTrajectory( &le->pos, cg.time, re->origin );

	// check for collisions
	if ( le->leFlags & LEF_COLLISIONS ) {
		// do a trace sometimes
		if ( (cg.oldTime >> 6) != (cg.time >> 6) ) {
			trace_t		trace;

			CG_Trace( &trace, re->oldorigin, NULL, NULL, re->origin, -1, CONTENTS_SOLID );
			VectorCopy( re->origin, re->oldorigin );

			// hit something
			if ( trace.fraction < 1.0 ) {
				// stop
				VectorCopy( trace.endpos, le->pos.trBase );
				le->pos.trType = TR_STATIONARY;
				le->leFlags &= ~(LEF_COLLISIONS | LEF_TUMBLE);
			}
		}
	}

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len < le->radius ) {
		CG_FreeLocalEntity( le );
		return;
	}

	trap_R_AddRefEntityToScene( re );
}


/*
===================
CG_AddScaleFade

For rocket smokes that hang in place, fade out, and are
removed if the view passes through them.
There are often many of these, so it needs to be simple.
===================
*/
static void CG_AddScaleFade( localEntity_t *le ) {
	refEntity_t	*re;
	float		c;
	vec3_t		delta;
	float		len;

	re = &le->refEntity;

	// fade / grow time
	c = ( le->endTime - cg.time ) * le->lifeRate;

	re->shaderRGBA[3] = 0xff * c * le->color[3];
	re->radius = le->radius * ( 1.0 - c ) + 8;

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len < le->radius ) {
		CG_FreeLocalEntity( le );
		return;
	}

	trap_R_AddRefEntityToScene( re );
}


/*
=================
CG_AddFallScaleFade

This is just an optimized CG_AddMoveScaleFade
For blood mists that drift down, fade out, and are
removed if the view passes through them.
There are often 100+ of these, so it needs to be simple.
=================
*/
static void CG_AddFallScaleFade( localEntity_t *le ) {
	refEntity_t	*re;
	float		c;
	vec3_t		delta;
	float		len;

	re = &le->refEntity;

	// fade time
	c = ( le->endTime - cg.time ) * le->lifeRate;

	re->shaderRGBA[3] = 0xff * c * le->color[3];

	re->origin[2] = le->pos.trBase[2] - ( 1.0 - c ) * le->pos.trDelta[2];

	re->radius = le->radius * ( 1.0 - c ) + 16;

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len < le->radius ) {
		CG_FreeLocalEntity( le );
		return;
	}

	trap_R_AddRefEntityToScene( re );
}



/*
================
CG_AddExplosion
================
*/
static void CG_AddExplosion( localEntity_t *ex ) {
	refEntity_t	*ent;

	ent = &ex->refEntity;

	// add the entity
	if ( ent->customShader == cgs.media.nipperWaveShader )
	{
		vec3_t	axisCopy[3];
		AxisCopy( ent->axis, axisCopy );
		AxisScale( ent->axis, 0.05 + 0.2 * (float)( cg.time - ex->startTime ) / ( ex->endTime - ex->startTime ), ent->axis );
		trap_R_AddRefEntityToScene(ent);
		AxisCopy( axisCopy, ent->axis );
	}
	else
		trap_R_AddRefEntityToScene(ent);

	// add the dlight
	if ( ex->light ) {
		float		light;

		light = (float)( cg.time - ex->startTime ) / ( ex->endTime - ex->startTime );
		if ( light < 0.5 ) {
			light = 1.0;
		} else {
			light = 1.0 - ( light - 0.5 ) * 2;
		}
		light = ex->light * light;
		trap_R_AddLightToScene(ent->origin, light, ex->lightColor[0], ex->lightColor[1], ex->lightColor[2] );
	}
}

/*
================
CG_AddSpriteExplosion
================
*/
static void CG_AddSpriteExplosion( localEntity_t *le ) {
	refEntity_t	re;
	float c;

	re = le->refEntity;

	c = ( le->endTime - cg.time ) / ( float ) ( le->endTime - le->startTime );
	if ( c > 1 ) {
		c = 1.0;	// can happen during connection problems
	}

	re.shaderRGBA[0] = 0xff;
	re.shaderRGBA[1] = 0xff;
	re.shaderRGBA[2] = 0xff;
	re.shaderRGBA[3] = 0xff * c * 0.33;

	re.reType = RT_SPRITE;
	re.radius = 42 * ( 1.0 - c ) + 30;

	trap_R_AddRefEntityToScene( &re );

	// add the dlight
	if ( le->light ) {
		float		light;

		light = (float)( cg.time - le->startTime ) / ( le->endTime - le->startTime );
		if ( light < 0.5 ) {
			light = 1.0;
		} else {
			light = 1.0 - ( light - 0.5 ) * 2;
		}
		light = le->light * light;
		trap_R_AddLightToScene(re.origin, light, le->lightColor[0], le->lightColor[1], le->lightColor[2] );
	}
}

/*
====================
CG_AddTrail
====================
*/
#define TRAIL_TEXSCALE		0.005
// FADETIME declared in cg_local.h

void CG_AddTrail( localEntity_t *le ) {
	vec3_t		endPos, startPos;
	vec3_t		endDir, startDir, beamDir;
	vec3_t		endDelta, startDelta;
	float		texScale;
	int			fadeFrac;
	polyVert_t	verts[4];

	// calculate positions and delta vectors
	BG_EvaluateTrajectory( &le->pos, le->fadeInTime, endPos );
	BG_EvaluateTrajectory( &le->pos, le->pos.trTime, startPos );
	startPos[2] -= 5;

	VectorSubtract( startPos, cg.refdef.vieworg, startDir );
	VectorSubtract( endPos, cg.refdef.vieworg, endDir );
	VectorSubtract( endPos, startPos, beamDir );

	texScale = VectorLength( beamDir ) * TRAIL_TEXSCALE;
	VectorNormalize( beamDir );

	CrossProduct( startDir, beamDir, startDelta );
	CrossProduct( endDir, beamDir, endDelta );

	VectorNormalize2( startDelta, startDir );
	VectorNormalize2( endDelta, endDir );

	// calc fade fraction
	fadeFrac = 255 - ((cg.time - le->fadeInTime) * 255 / TRAIL_FADETIME);
	if ( fadeFrac < 0 ) fadeFrac = 0;
	if ( fadeFrac > 255 ) fadeFrac = 255;

	// set vert properties
	verts[0].st[0] = 0;	
	verts[0].st[1] = 0;	
	verts[0].modulate[0] = verts[0].modulate[1] = verts[0].modulate[2] = 0;	
	verts[0].modulate[3] = 255;	

	verts[1].st[0] = 0;	
	verts[1].st[1] = 1;	
	verts[1].modulate[0] = verts[1].modulate[1] = verts[1].modulate[2] = 0;	
	verts[1].modulate[3] = 255;	

	verts[2].st[0] = texScale;	
	verts[2].st[1] = 1;	
	verts[2].modulate[0] = verts[2].modulate[1] = verts[2].modulate[2] = fadeFrac;	
	verts[2].modulate[3] = 255;	

	verts[3].st[0] = texScale;	
	verts[3].st[1] = 0;	
	verts[3].modulate[0] = verts[3].modulate[1] = verts[3].modulate[2] = fadeFrac;	
	verts[3].modulate[3] = 255;	

	// add first plane
	RotatePointAroundVector( startDelta, beamDir, startDir, 30 );
	RotatePointAroundVector( endDelta, beamDir, endDir, 30 );

	VectorMA( startPos,  le->refEntity.radius, startDelta, verts[0].xyz );
	VectorMA( startPos, -le->refEntity.radius, startDelta, verts[1].xyz );
	VectorMA( endPos, -le->refEntity.radius, endDelta, verts[2].xyz );
	VectorMA( endPos,  le->refEntity.radius, endDelta, verts[3].xyz );

	trap_R_AddPolyToScene( le->refEntity.customShader, 4, verts );

	// add second plane
	RotatePointAroundVector( startDelta, beamDir, startDir, -30 );
	RotatePointAroundVector( endDelta, beamDir, endDir, -30 );

	VectorMA( startPos,  le->refEntity.radius, startDelta, verts[0].xyz );
	VectorMA( startPos, -le->refEntity.radius, startDelta, verts[1].xyz );
	VectorMA( endPos, -le->refEntity.radius, endDelta, verts[2].xyz );
	VectorMA( endPos,  le->refEntity.radius, endDelta, verts[3].xyz );

	trap_R_AddPolyToScene( le->refEntity.customShader, 4, verts );
}

/*
====================
CG_AddTrailTracedRGB
====================
*/
void CG_AddTrailTracedRGB( localEntity_t *le ) {
	vec3_t		endPos, startPos;
	vec3_t		endDir, startDir, beamDir;
	vec3_t		endDelta, startDelta;
	float		texScale;
	int			fadeFrac;
	polyVert_t	verts[4];
	trace_t		trace;

	// calculate positions and delta vectors
	BG_EvaluateTrajectory( &le->pos, le->fadeInTime, endPos );
	BG_EvaluateTrajectory( &le->pos, le->pos.trTime, startPos );
	startPos[2] -= 5;

	/* trace to get a reasonable trail endpoint
	for missiles that travel rediculously fast like kma97
	( I guess that could be kinda costly though.. )*/
	CG_Trace( &trace, startPos, vec3_origin, vec3_origin, endPos, 0, MASK_SHOT );
	VectorCopy( trace.endpos, endPos );
	
	VectorSubtract( startPos, cg.refdef.vieworg, startDir );
	VectorSubtract( endPos, cg.refdef.vieworg, endDir );
	VectorSubtract( endPos, startPos, beamDir );

	texScale = VectorLength( beamDir ) * TRAIL_TEXSCALE;
	VectorNormalize( beamDir );

	CrossProduct( startDir, beamDir, startDelta );
	CrossProduct( endDir, beamDir, endDelta );

	VectorNormalize2( startDelta, startDir );
	VectorNormalize2( endDelta, endDir );

	// calc fade fraction
	fadeFrac = 255 - ((cg.time - le->fadeInTime) * 255 / TRAIL_FADETIME);
	if ( fadeFrac < 0 ) fadeFrac = 0;
	if ( fadeFrac > 255 ) fadeFrac = 255;

	// set vert properties
	verts[0].st[0] = 0;	
	verts[0].st[1] = 0;	
	verts[0].modulate[0] = 0;
	verts[0].modulate[1] = 0;
	verts[0].modulate[2] = 0;
	verts[0].modulate[3] = 255;	

	verts[1].st[0] = 0;	
	verts[1].st[1] = 1;	
	verts[1].modulate[0] = 0;
	verts[1].modulate[1] = 0;
	verts[1].modulate[2] = 0;
	verts[1].modulate[3] = 255;	

	verts[2].st[0] = texScale;	
	verts[2].st[1] = 1;	
	verts[2].modulate[0] = le->color[0] * fadeFrac;
	verts[2].modulate[1] = le->color[1] * fadeFrac;
	verts[2].modulate[2] = le->color[2] * fadeFrac;
	verts[2].modulate[3] = 255;	

	verts[3].st[0] = texScale;	
	verts[3].st[1] = 0;	
	verts[3].modulate[0] = le->color[0] * fadeFrac;
	verts[3].modulate[1] = le->color[1] * fadeFrac;
	verts[3].modulate[2] = le->color[2] * fadeFrac;
	verts[3].modulate[3] = 255;
	
	// add first plane
	RotatePointAroundVector( startDelta, beamDir, startDir, 30 );
	RotatePointAroundVector( endDelta, beamDir, endDir, 30 );

	VectorMA( startPos,  le->refEntity.radius, startDelta, verts[0].xyz );
	VectorMA( startPos, -le->refEntity.radius, startDelta, verts[1].xyz );
	VectorMA( endPos, -le->refEntity.radius, endDelta, verts[2].xyz );
	VectorMA( endPos,  le->refEntity.radius, endDelta, verts[3].xyz );

	trap_R_AddPolyToScene( le->refEntity.customShader, 4, verts );

	// add second plane
	RotatePointAroundVector( startDelta, beamDir, startDir, -30 );
	RotatePointAroundVector( endDelta, beamDir, endDir, -30 );

	VectorMA( startPos,  le->refEntity.radius, startDelta, verts[0].xyz );
	VectorMA( startPos, -le->refEntity.radius, startDelta, verts[1].xyz );
	VectorMA( endPos, -le->refEntity.radius, endDelta, verts[2].xyz );
	VectorMA( endPos,  le->refEntity.radius, endDelta, verts[3].xyz );

	trap_R_AddPolyToScene( le->refEntity.customShader, 4, verts );
}

/*
====================
CG_CalcNormAxis
====================
*/
void CG_CalcNormAxis( const vec3_t dir, vec3_t axis[3] )
{
	vec3_t	adir;

	// forward vector is the dir
	VectorCopy( dir, axis[0] );

	// find up vector
	if ( dir[2] != 0.0f ) {
		VectorCopy( dir, adir );
		if ( adir[2] > 0 ) VectorScale( adir, -1, adir ); 
		axis[1][0] = adir[0]; 
		axis[1][1] = adir[1]; 
		axis[1][2] = -(adir[0]*adir[0] + adir[1]*adir[1]) / adir[2];
		VectorNormalize( axis[1] );
	} else {
		VectorSet( axis[1], 0, 0, 1 );
	}

	// find the normal
	CrossProduct( axis[1], axis[0], axis[2] );
}

/*
====================
CG_AddWaterBeam
====================
*/
#define BOASTER_WIDTH		4
#define BOASTER_FADEOUT		500
#define BOASTER_TRACETIME	100
#define BOASTER_IMPACTTIME	600
#define BOASTER_IMPACTUP	20
#define BOASTER_IMPACTSIDE	30
#define BOASTER_TEARDIST	128
// FADETIME declared in cg_local.h

void CG_AddWaterBeam( localEntity_t *le ) {
	vec3_t		endPos, startPos;
	vec3_t		endDir, startDir, beamDir;
	vec3_t		endDelta, startDelta;
	vec3_t		endAxis[3], startAxis[3];
	vec3_t		rot;
	int			fade, fadeEnd;
	int			i;
	float		phi;
	polyVert_t	verts[4];
	float		startwidth, endwidth;

	// changing back to sv-muzzelPoint, after some time (first cl-movement is done from weaponmodel-muzzelPoint)
	if(le->angles.trTime && cg.time-le->pos.trTime>150)	{
		vec3_t oldCurPos, newCurPos;
		trace_t tr;
		BG_EvaluateTrajectory( &le->pos, le->startTime, oldCurPos );

		VectorCopy(le->angles.trBase,le->pos.trBase);
		VectorCopy(le->angles.trDelta,le->pos.trDelta);
		le->angles.trTime=0;

		BG_EvaluateTrajectory( &le->pos, le->startTime, newCurPos );

		// check if there is a wall between current and new pos
		CG_Trace( &tr, oldCurPos, NULL, NULL, newCurPos, le->ownerNum, MASK_SHOT);
		if(tr.fraction!=1.0f || tr.startsolid) {
			if ( !(tr.surfaceFlags & SURF_NOIMPACT) )
				CG_MissileHitWall( WP_BOASTER, (tr.entityNum == ENTITYNUM_WORLD && tr.plane.normal[2] > 0.8), tr.endpos, tr.plane.normal, 0, IMPACTSOUND_DEFAULT );
			return;
		}

//		BG_EvaluateTrajectory( &le->pos, cg.time, startPos );
//		CG_GenerateParticles( cgs.media.fireDropModel, 0, startPos, 16, le->pos.trDelta, 120, 
//			250, 4, 0, cg.time, 700, 400, 0, 0, 0, 0, LEF_GRAVITY | LEF_COLLISIONS, 0 );
	}

	if( le->newer && le->newer->angles.trTime && cg.time-le->newer->pos.trTime>150)
	{
		VectorCopy(le->newer->angles.trBase,le->newer->pos.trBase);
		VectorCopy(le->newer->angles.trDelta,le->newer->pos.trDelta);
		le->newer->angles.trTime=0;
	}

	if( le->older && le->older->angles.trTime && cg.time-le->older->pos.trTime>150)
	{
		VectorCopy(le->older->angles.trBase,le->older->pos.trBase);
		VectorCopy(le->older->angles.trDelta,le->older->pos.trDelta);
		le->older->angles.trTime=0;
	}

	// check for last entity
	if ( !le->newer )
	{
		centity_t	*owner;
		vec3_t		forward;
		owner = &cg_entities[le->ownerNum];

		// get the current muzzle
		if ( !(owner->currentState.eFlags & EF_DEAD) )
		{
			CG_GetWaterMuzzle( le, owner, forward );
//			VectorCopy( le->color, startPos );
			VectorCopy(cgs.clientinfo[owner->currentState.number].realmuzzle, startPos);

			if ( DotProduct( forward, owner->beamEnd ) < 0.98 ) 
			{
				// get independent
				if ( le->older ) le->older->newer = le->older;
				le->older = le;
			}
		}
		else VectorCopy( le->pos.trBase, startPos );

		startwidth = 1.5f;
	}
	else
	{
		BG_EvaluateTrajectory( &le->newer->pos, cg.time, startPos );

		if(cg.time-le->newer->pos.trTime>50)
			startwidth=BOASTER_WIDTH;
		else
			startwidth=1.5f+2.5f*(float)(cg.time-le->newer->pos.trTime)*0.01f;// 1/100=0.01f
	}

	// calculate positions and delta vectors
	BG_EvaluateTrajectory( &le->pos, cg.time, endPos );
	VectorSubtract( endPos, startPos, beamDir );

	// tear beam apart, if needed
	if ( le == le->newer || (!le->newer && VectorLength( beamDir ) > BOASTER_TEARDIST) ) 
	{
//		if ( le->older && le != le->older ) le->older->newer = le->older;
		le->newer = le;
		fadeEnd = 0;

		// take velocity for direction of the beam
		BG_EvaluateTrajectoryDelta( &le->pos, cg.time, startDelta );
		VectorNormalize( startDelta );

		endwidth=startwidth;
		if(cg.time-le->pos.trTime>70)
			VectorMA( endPos, -BOASTER_TEARDIST, startDelta, startPos );

		VectorSubtract( endPos, startPos, beamDir );	
	}
	else
	{
		fadeEnd = 255;
		if(cg.time-le->pos.trTime>50)
			endwidth=BOASTER_WIDTH;
		else
			endwidth=1.5f+2.5f*(float)(cg.time-le->pos.trTime)*0.01f;// 1/100=0.01f
	}

	// check for wall hits
	if ( cg.time > le->startTime + BOASTER_TRACETIME )
	{
		trace_t	tr;
static vec3_t minBoaster = {-8.0f,-8.0f,-8.0f};
static vec3_t maxBoaster = {8.0f,8.0f,8.0f};

		BG_EvaluateTrajectory( &le->pos, le->startTime, endDelta );
		CG_Trace( &tr, endDelta, minBoaster, maxBoaster, endPos, le->ownerNum, MASK_SHOT);

		if ( tr.fraction < 1.0 )
		{
			// impact
			if ( le->newer && le != le->newer ) le->newer->older = le->newer;
			if ( le->older && le != le->older ) le->older->newer = le->older;

			if ( !(tr.surfaceFlags & SURF_NOIMPACT) )
				CG_MissileHitWall( WP_BOASTER, (tr.entityNum == ENTITYNUM_WORLD && tr.plane.normal[2] > 0.8), tr.endpos, tr.plane.normal, 0, IMPACTSOUND_DEFAULT );//clientNum wird hier missbraucht ;P

			CG_FreeLocalEntity( le );
			return;
		}

		le->startTime = cg.time;
	}
	
	// set static vert properties
	verts[0].st[0] = 0;	
	verts[0].st[1] = 0;	
	verts[0].modulate[3] = 255;	

	verts[1].st[0] = 0;	
	verts[1].st[1] = 1;	
	verts[1].modulate[3] = 255;	

	verts[2].st[0] = 1;	
	verts[2].st[1] = 1;	
	verts[2].modulate[3] = 255;	

	verts[3].st[0] = 1;	
	verts[3].st[1] = 0;	
	verts[3].modulate[3] = 255;	

	if ( cg.time > le->endTime - BOASTER_FADEOUT ) {
		fade = 255 * (float)(le->endTime - cg.time) / BOASTER_FADEOUT;
		fadeEnd *= (float)(le->endTime - cg.time) / BOASTER_FADEOUT;
	} else
		fade = 255;

	// use the beamDir for the startDir
	VectorNormalize( beamDir );
	VectorCopy( beamDir, startDir );
	VectorCopy( beamDir, endDir );

	// calc an endDir out of the older le
	if ( le->older && le->older != le ) {
		BG_EvaluateTrajectory( &le->older->pos, cg.time, endDelta );
		VectorSubtract( endDelta, endPos, endDir );
		VectorNormalize( endDir );
	}

	CG_CalcNormAxis( startDir, startAxis );
	CG_CalcNormAxis( endDir, endAxis );

	// set vert properties
	VectorSet( verts[0].modulate, fadeEnd, fadeEnd, fadeEnd );
	VectorSet( verts[1].modulate, fadeEnd, fadeEnd, fadeEnd );
	VectorSet( verts[2].modulate, fade, fade, fade );
	VectorSet( verts[3].modulate, fade, fade, fade );

	// add planes
	phi = 0;
	VectorSet( rot, 0, 0, 1 );
	VectorRotateTMatrix( rot, startAxis, startDelta );
	VectorRotateTMatrix( rot, endAxis, endDelta );

	for ( i = 0; i < 6; i++ )
	{
		VectorCopy( startDelta, startDir );
		VectorCopy( endDelta, endDir );

		phi += M_PI/3;
		VectorSet( rot, 0, sin(phi), cos(phi) );
		VectorRotateTMatrix( rot, startAxis, startDelta );
		VectorRotateTMatrix( rot, endAxis, endDelta );

		VectorMA( startPos, startwidth, startDir, verts[0].xyz );
		VectorMA( startPos, startwidth, startDelta, verts[1].xyz );
		VectorMA( endPos, endwidth, endDelta, verts[2].xyz );
		VectorMA( endPos, endwidth, endDir, verts[3].xyz );

		trap_R_AddPolyToScene( cgs.media.waterBeamShader, 4, verts );
	}

	// add planes
/*	for ( i = 0; i < 3; i++ )
	{
		phi = i*2*M_PI/3;
		VectorSet( rot, 0, sin(phi), cos(phi) );
		VectorRotateTMatrix( rot, startAxis, startDelta );
		VectorRotateTMatrix( rot, endAxis, endDelta );

		VectorMA( startPos,  BOASTER_WIDTH, startDelta, verts[0].xyz );
		VectorMA( startPos, -BOASTER_WIDTH, startDelta, verts[1].xyz );
		VectorMA( endPos, -BOASTER_WIDTH, endDelta, verts[2].xyz );
		VectorMA( endPos,  BOASTER_WIDTH, endDelta, verts[3].xyz );

		trap_R_AddPolyToScene( cgs.media.waterBeamShader, 4, verts );
	}*/
}

/*
====================
CG_AddPumperTrail
====================
*/
#define PUMPER_WIDTH		6
#define PUMPER_DELTA_PHI	(M_PI/5)
#define PUMPER_DELTA_POS	16

void CG_AddPumperTrail( localEntity_t *le ) {
	vec3_t		axis[3];
	vec3_t		beamDir, rot;
	vec3_t		newDelta, lastDelta;
	vec3_t		newPos, lastPos;
	int			fade, modulate;
	float		phi, pos, dist;
	polyVert_t	verts[4];

	// set static vert properties
	fade = 255 * (float)(le->endTime - cg.time) / (float)(le->endTime - le->startTime);

	verts[0].st[0] = 0;	
	verts[0].st[1] = 0;	
	verts[0].modulate[3] = 255;	

	verts[1].st[0] = 0;	
	verts[1].st[1] = 1;	
	verts[1].modulate[3] = 255;	

	verts[2].st[0] = 1;	
	verts[2].st[1] = 1;	
	verts[2].modulate[3] = 255;	

	verts[3].st[0] = 1;	
	verts[3].st[1] = 0;	
	verts[3].modulate[3] = 255;	

	// calculate axis
	VectorSubtract( le->pos.trDelta, le->pos.trBase, beamDir );
	dist = VectorLength( beamDir );
	VectorNormalize( beamDir );
	CG_CalcNormAxis( beamDir, axis );
	VectorScale( beamDir, PUMPER_DELTA_POS, beamDir );

	// initialize
	VectorSet( rot, 0, 0, 1 );
	VectorRotateTMatrix( rot, axis, newDelta );
	VectorCopy( le->pos.trBase, newPos );
	phi = le->lifeRate;
	pos = 0;
	modulate = 0;
	VectorSet( verts[2].modulate, modulate, modulate, modulate );
	VectorSet( verts[3].modulate, modulate, modulate, modulate );

	// add planes
	do
	{
		// store old position and delta
		VectorCopy( newDelta, lastDelta );
		VectorCopy( newPos, lastPos );

		// set the modulate values
		VectorCopy( verts[2].modulate, verts[0].modulate );
		VectorCopy( verts[3].modulate, verts[1].modulate );

		modulate = pos > 0.2*dist ? fade : fade * pos / dist * 5;
		VectorSet( verts[2].modulate, modulate, modulate, modulate );
		VectorSet( verts[3].modulate, modulate, modulate, modulate );

		// get new delta
		phi += PUMPER_DELTA_PHI;
		VectorSet( rot, 0, sin(phi), cos(phi) );
		VectorRotateTMatrix( rot, axis, newDelta );

		// get new position
		if ( pos + PUMPER_DELTA_POS > dist )
			VectorScale( beamDir, (dist - pos) / PUMPER_DELTA_POS, beamDir );

		pos += PUMPER_DELTA_POS;
		VectorAdd( lastPos, beamDir, newPos );

		VectorMA( lastPos,  PUMPER_WIDTH, lastDelta, verts[0].xyz );
		VectorMA( lastPos, -PUMPER_WIDTH, lastDelta, verts[1].xyz );
		VectorMA( newPos,  -PUMPER_WIDTH, newDelta,  verts[2].xyz );
		VectorMA( newPos,   PUMPER_WIDTH, newDelta,  verts[3].xyz );

		trap_R_AddPolyToScene( cgs.media.pumperTrailShader, 4, verts );
	} while ( pos < dist );
}


void CG_AddBoomiesExplosion( localEntity_t *le ) {
	refEntity_t *re;
	float	frac;

	// setup and fade
	re = &le->refEntity;
	frac = (le->endTime - cg.time) * le->lifeRate;

	// resize
	AnglesToAxis( le->angles.trBase, re->axis );
	AxisScale( re->axis, 2 * (1.05 - frac), re->axis ); //imperius: 10 * (1.05 - frac)

	// render the core
	re->reType = RT_SPRITE;
	re->customShader = cgs.media.boomiesCoreShader;
	re->radius = 20 + 200 * sin( M_PI * frac );
	re->shaderRGBA[0] = re->shaderRGBA[1] = re->shaderRGBA[2] = (frac > 0.25) ? 255 : 255*4 * frac;
	trap_R_AddRefEntityToScene( re );

/*	// render the rings
	re->reType = RT_MODEL;
	re->customShader = 0;
	re->hModel = cgs.media.imperiusRingsModel;
	re->shaderRGBA[0] = re->shaderRGBA[1] = re->shaderRGBA[2] = 255 * frac;
	trap_R_AddRefEntityToScene( re );
*/
	// render the sphere
	if ( frac > 0.3 )
	{
		frac = (frac - 0.3) * 3;
		if ( frac > 1.0 ) frac = 1.0;
		re->reType = RT_MODEL;
		re->customShader = 0;
		re->hModel = cgs.media.boomiesSphereModel;
		re->shaderRGBA[0] = re->shaderRGBA[1] = re->shaderRGBA[2] = 255 * frac;
		trap_R_AddRefEntityToScene( re );
	}
}

/*
====================
CG_AddImperiusBoom
====================
*/
void CG_AddImperiusBoom( localEntity_t *le ) {
	refEntity_t *re;
	float	frac;

	// setup and fade
	re = &le->refEntity;
	frac = (le->endTime - cg.time) * le->lifeRate;

	// resize
	AnglesToAxis( le->angles.trBase, re->axis );
	AxisScale( re->axis, 10 * (1.05 - frac), re->axis );

	// render the core
	re->reType = RT_SPRITE;
	re->customShader = cgs.media.imperiusCoreShader;
	re->radius = 20 + 200 * sin( M_PI * frac );
	re->shaderRGBA[0] = re->shaderRGBA[1] = re->shaderRGBA[2] = (frac > 0.25) ? 255 : 255*4 * frac;
	trap_R_AddRefEntityToScene( re );

	// render the rings
	re->reType = RT_MODEL;
	re->customShader = 0;
	re->hModel = cgs.media.imperiusRingsModel;
	re->shaderRGBA[0] = re->shaderRGBA[1] = re->shaderRGBA[2] = 255 * frac;
	trap_R_AddRefEntityToScene( re );

	// render the sphere
	if ( frac > 0.3 )
	{
		frac = (frac - 0.3) * 3;
		if ( frac > 1.0 ) frac = 1.0;
		re->hModel = cgs.media.imperiusSphereModel;
		re->shaderRGBA[0] = re->shaderRGBA[1] = re->shaderRGBA[2] = 255 * frac;
		trap_R_AddRefEntityToScene( re );
	}
}


/*
====================
CG_AddImperiusRings
====================
*/
void CG_AddImperiusRings( localEntity_t *le ) {
	refEntity_t *re;
	vec3_t	axis[3];
	float	c, s;

	re = &le->refEntity;

	c = ( le->endTime - cg.time ) * le->lifeRate;
	c *= 0xff;
	re->shaderRGBA[0] = le->color[0] * c;
	re->shaderRGBA[1] = le->color[1] * c;
	re->shaderRGBA[2] = le->color[2] * c;
	re->shaderRGBA[3] = le->color[3] * c;

	s = 0.1 + ( cg.time - le->startTime ) * le->lifeRate * 0.3;
	AxisCopy( re->axis, axis );
	AxisScale( axis, s, re->axis );

	trap_R_AddRefEntityToScene( re );

	AxisCopy( axis, re->axis );
}

/*
====================
CG_AddTeleffect
====================
*/
void CG_AddTeleffect( localEntity_t *le ) {
	refEntity_t *re;
	float		frac;

	// setup and fade
	re = &le->refEntity;
	frac = (le->endTime - cg.time) * le->lifeRate;

	// resize
	AnglesToAxis( le->angles.trBase, re->axis );
	AxisScale( re->axis, (1.3f - frac*0.3f), re->axis );

	// render the rings
	re->reType = RT_MODEL;
	re->shaderRGBA[0] = re->shaderRGBA[1] = re->shaderRGBA[2] = 255 * frac;
	trap_R_AddRefEntityToScene( re );
}

/*
===================
CG_AddScorePlum
===================
*/
#define NUMBER_SIZE		8

void CG_AddScorePlum( localEntity_t *le ) {
	refEntity_t	*re;
	vec3_t		origin, delta, dir, vec, up = {0, 0, 1};
	float		c, len;
	int			i, score, digits[10], numdigits, negative;

	re = &le->refEntity;

	c = ( le->endTime - cg.time ) * le->lifeRate;

	score = le->radius;
	if (score < 0) {
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0x11;
		re->shaderRGBA[2] = 0x11;
	}
	else {
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0xff;
		re->shaderRGBA[2] = 0xff;
		if (score >= 50) {
			re->shaderRGBA[1] = 0;
		} else if (score >= 20) {
			re->shaderRGBA[0] = re->shaderRGBA[1] = 0;
		} else if (score >= 10) {
			re->shaderRGBA[2] = 0;
		} else if (score >= 2) {
			re->shaderRGBA[0] = re->shaderRGBA[2] = 0;
		}

	}
	if (c < 0.25)
		re->shaderRGBA[3] = 0xff * 4 * c;
	else
		re->shaderRGBA[3] = 0xff;

	re->radius = NUMBER_SIZE / 2;

	VectorCopy(le->pos.trBase, origin);
	origin[2] += 110 - c * 100;

	VectorSubtract(cg.refdef.vieworg, origin, dir);
	CrossProduct(dir, up, vec);
	VectorNormalize(vec);

	VectorMA(origin, -10 + 20 * sin(c * 2 * M_PI), vec, origin);

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	VectorSubtract( origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len < 20 ) {
		CG_FreeLocalEntity( le );
		return;
	}

	negative = qfalse;
	if (score < 0) {
		negative = qtrue;
		score = -score;
	}

	for (numdigits = 0; !(numdigits && !score); numdigits++) {
		digits[numdigits] = score % 10;
		score = score / 10;
	}

	if (negative) {
		digits[numdigits] = 10;
		numdigits++;
	}

	for (i = 0; i < numdigits; i++) {
		VectorMA(origin, (float) (((float) numdigits / 2) - i) * NUMBER_SIZE, vec, re->origin);
		re->customShader = cgs.media.numberShaders[digits[numdigits-1-i]];
		trap_R_AddRefEntityToScene( re );
	}
}




//==============================================================================

/*
===================
CG_AddLocalEntities

===================
*/
void CG_AddLocalEntities( void ) {
	localEntity_t	*le, *next;

	// walk the list backwards, so any new local entities generated
	// (trails, marks, etc) will be present this frame
	le = cg_activeLocalEntities.prev;
	for ( ; le != &cg_activeLocalEntities ; le = next ) {
		// grab next now, so if the local entity is freed we
		// still have it
		next = le->prev;

		if ( cg.time >= le->endTime ) {
			CG_FreeLocalEntity( le );
			continue;
		}
		switch ( le->leType ) {
		default:
			CG_Error( "Bad leType: %i", le->leType );
			break;

		case LE_MARK:
			break;

		case LE_SPRITE_EXPLOSION:
			CG_AddSpriteExplosion( le );
			break;

		case LE_EXPLOSION:
			CG_AddExplosion( le );
			break;

		case LE_FRAGMENT:			// gibs and brass
			CG_AddFragment( le );
			break;

		case LE_MOVE_SCALE_FADE:		// water bubbles
			CG_AddMoveScaleFade( le );
			break;

		case LE_FADE_RGB:				// teleporters, railtrails
			CG_AddFadeRGB( le );
			break;

		case LE_SPRAYTRAIL:
			CG_AddSprayTrailFade( le );
			break;

		case LE_TRAIL:
			CG_AddTrail( le );
			break;

		case LE_TRAIL_TRACED_RGB:
			CG_AddTrailTracedRGB( le );
			break;

		case LE_WATERBEAM:
			CG_AddWaterBeam( le );
			break;

		case LE_PUMPERTRAIL:
			CG_AddPumperTrail( le );
			break;

		case LE_IMPERIUSBOOM:
			CG_AddImperiusBoom( le );
			break;

		case LE_IMPERIUSRINGS:
			CG_AddImperiusRings( le );
			break;

		case LE_TELEFFECT:
			CG_AddTeleffect( le );
			break;

		case LE_FALL_SCALE_FADE: // gib blood trails
			CG_AddFallScaleFade( le );
			break;

		case LE_SCALE_FADE:		// rocket trails
			CG_AddScaleFade( le );
			break;

		case LE_SCOREPLUM:
			CG_AddScorePlum( le );
			break;

		case LE_BOOMIESEXPLOSION:
			CG_AddBoomiesExplosion( le );
			break;
		}
	}
}




