// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_effects.c -- these functions generate localentities, usually as a result
// of event processing

#include "cg_local.h"


/*
==================
CG_BubbleTrail

Bullets shot underwater
==================
*/
void CG_BubbleTrail( vec3_t start, vec3_t end, float spacing ) {
	vec3_t		move;
	vec3_t		vec;
	float		len;
	int			i;

	if ( cg_noProjectileTrail.integer ) {
		return;
	}

	VectorCopy (start, move);
	VectorSubtract (end, start, vec);
	len = VectorNormalize (vec);

	// advance a random amount first
	i = rand() % (int)spacing;
	VectorMA( move, i, vec, move );

	VectorScale (vec, spacing, vec);

	for ( ; i < len; i += spacing ) {
		localEntity_t	*le;
		refEntity_t		*re;

		le = CG_AllocLocalEntity();
		le->leFlags = LEF_PUFF_DONT_SCALE;
		le->leType = LE_MOVE_SCALE_FADE;
		le->startTime = cg.time;
		le->endTime = cg.time + 1000 + random() * 250;
		le->lifeRate = 1.0 / ( le->endTime - le->startTime );

		re = &le->refEntity;
		re->shaderTime = cg.time / 1000.0f;

		re->reType = RT_SPRITE;
		re->rotation = 0;
		re->radius = 3;
		re->customShader = cgs.media.waterBubbleShader;
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0xff;
		re->shaderRGBA[2] = 0xff;
		re->shaderRGBA[3] = 0xff;

		le->color[3] = 1.0;

		le->pos.trType = TR_LINEAR;
		le->pos.trTime = cg.time;
		VectorCopy( move, le->pos.trBase );
		le->pos.trDelta[0] = crandom()*5;
		le->pos.trDelta[1] = crandom()*5;
		le->pos.trDelta[2] = crandom()*5 + 6;

		VectorAdd (move, vec, move);
	}
}

/*
=====================
CG_SmokePuff

Adds a smoke puff or blood trail localEntity.
=====================
*/
localEntity_t *CG_SmokePuff( const vec3_t p, const vec3_t vel, 
				   float radius,
				   float r, float g, float b, float a,
				   float duration,
				   int startTime,
				   int fadeInTime,
				   int leFlags,
				   qhandle_t hShader ) {
	static int	seed = 0x92;
	localEntity_t	*le;
	refEntity_t		*re;
//	int fadeInTime = startTime + duration / 2;

	le = CG_AllocLocalEntity();
	le->leFlags = leFlags;
	le->radius = radius;

	re = &le->refEntity;
	re->rotation = Q_random( &seed ) * 360;
	re->radius = radius;
	re->shaderTime = startTime / 1000.0f;

	le->leType = LE_MOVE_SCALE_FADE;
	le->startTime = startTime;
	le->fadeInTime = fadeInTime;
	le->endTime = startTime + duration;
	if ( fadeInTime > startTime ) {
		le->lifeRate = 1.0 / ( le->endTime - le->fadeInTime );
	}
	else {
		le->lifeRate = 1.0 / ( le->endTime - le->startTime );
	}
	le->color[0] = r;
	le->color[1] = g; 
	le->color[2] = b;
	le->color[3] = a;


	le->pos.trType = TR_LINEAR;
	le->pos.trTime = startTime;
	VectorCopy( vel, le->pos.trDelta );
	VectorCopy( p, le->pos.trBase );

	VectorCopy( p, re->origin );
	re->customShader = hShader;

	// rage pro can't alpha fade, so use a different shader
/*	if ( cgs.glconfig.hardwareType == GLHW_RAGEPRO ) {
		re->customShader = cgs.media.smokePuffRageProShader;
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0xff;
		re->shaderRGBA[2] = 0xff;
		re->shaderRGBA[3] = 0xff;
	} else {
*/
		re->shaderRGBA[0] = le->color[0] * 0xff;
		re->shaderRGBA[1] = le->color[1] * 0xff;
		re->shaderRGBA[2] = le->color[2] * 0xff;
		re->shaderRGBA[3] = 0xff;
//	}

	re->reType = RT_SPRITE;
	re->radius = le->radius;

	return le;
}


/*
==================
CG_SpawnEffect

Player teleporting in or out
==================
*/
void CG_TeleOutEffect(vec3_t org, int team, centity_t *cent)
{
	clientInfo_t	*ci;
	refEntity_t*	legs = NULL;
	refEntity_t*	torso = NULL;
	refEntity_t*	head = NULL;
	int				clientNum;
	int				renderfx;
	localEntity_t	*le;
	int				i;

	clientNum = cent->currentState.clientNum;
	if ( clientNum < 0 || clientNum >= MAX_CLIENTS ) {
		CG_Error( "Bad clientNum on TeleOut-Ent");
	}
	ci = &cgs.clientinfo[ clientNum ];

	// it is possible to see corpses from disconnected players that may
	// not have valid clientinfo
//	if ( !ci->infoValid ) {
//		return;
//	}

	for(i=0;i<3;++i)
	{
		le = CG_AllocLocalEntity();
		le->leType = LE_TELEFFECT;
		le->startTime = cg.time;
		le->endTime = cg.time + 2000;
		le->lifeRate = 1.0 / (le->endTime - le->startTime);
	//	VectorCopy( org, le->refEntity.origin );
	//	VectorSet( le->angles.trBase, 0, 0, 0 ); //360*crandom(), 360*crandom(), 360*crandom() );
		le->angles.trBase[YAW] = cent->currentState.angles[YAW];
		if ( team == 1 ) le->refEntity.customShader = cgs.media.teleportEffectRedShader;
		else if ( team == 2 ) le->refEntity.customShader = cgs.media.teleportEffectBlueShader;
		else le->refEntity.customShader = cgs.media.teleportEffectGreenShader;
		switch(i)
		{
		case 0:
			head = &(le->refEntity);
			le->refEntity.frame = 0;
			break;
		case 1:
			torso = &(le->refEntity);
			le->refEntity.frame = ci->animations[TORSO_STAND].firstFrame;
			break;
		case 2:
			legs = &(le->refEntity);
			le->refEntity.frame = ci->animations[LEGS_IDLE].firstFrame;
			break;
		}
		le->refEntity.oldframe = le->refEntity.frame;
		AnglesToAxis( le->angles.trBase, le->refEntity.axis );
	}

//	VectorCopy(cent->currentState.angles,cent->lerpAngles);

	// get the player model information
	renderfx = 0;

//	memset( legs, 0, sizeof(refEntity_t) );
//	memset( torso, 0, sizeof(refEntity_t) );
//	memset( head, 0, sizeof(refEntity_t) );

	// get the rotation information
//	CG_PlayerAngles( cent, legs.axis, torso.axis, head.axis );
	
	// get the animation state (after rotation, to allow feet shuffle)
//	CG_PlayerAnimation( cent, &legs.oldframe, &legs.frame, &legs.backlerp,
//		 &torso.oldframe, &torso.frame, &torso.backlerp );

	renderfx |= RF_LIGHTING_ORIGIN;			// use the same origin for all

	//
	// add the legs
	//
	legs->hModel = ci->legsModel;
//	legs.customSkin = ci->legsSkin;

	VectorCopy( org, legs->origin );

	VectorCopy( org, legs->lightingOrigin );
	legs->renderfx = renderfx;
	VectorCopy (legs->origin, legs->oldorigin);	// don't positionally lerp at all


	// if the model failed, allow the default nullmodel to be displayed
	if (!legs->hModel) {
		return;
	}

	//
	// add the torso
	//
	torso->hModel = ci->torsoModel;
	if (!torso->hModel) {
		return;
	}

//	torso.customSkin = ci->torsoSkin;

	VectorCopy( org, torso->lightingOrigin );

	CG_PositionRotatedEntityOnTag( torso, legs, ci->legsModel, "tag_torso");

	torso->renderfx = renderfx;


	//
	// add the head
	//
	head->hModel = ci->headModel;
	if (!head->hModel) {
		return;
	}
//	head.customSkin = ci->headSkin;

	VectorCopy( org, head->lightingOrigin );

	CG_PositionRotatedEntityOnTag( head, torso, ci->torsoModel, "tag_head");

	head->renderfx = renderfx;
}

void CG_SpawnEffect( vec3_t org, int team ) {
	localEntity_t	*le;

	le = CG_AllocLocalEntity();
	le->leType = LE_TELEFFECT;
	le->startTime = cg.time;
	le->endTime = cg.time + 1000;
	le->lifeRate = 1.0 / (le->endTime - le->startTime);
	le->radius = rand();
	VectorCopy( org, le->refEntity.origin );
	VectorSet( le->angles.trBase, 0, 0, 0 ); //360*crandom(), 360*crandom(), 360*crandom() );
	//model zuweisung aus CG_AddTeleffect hier her verlegt ... um LE_TELEFFECT für den neuen effekt zu nutzen
	le->refEntity.hModel = cgs.media.teleportEffectModel;
	if ( team == 1 ) le->refEntity.customShader = cgs.media.teleportEffectRedShader;
	else if ( team == 2 ) le->refEntity.customShader = cgs.media.teleportEffectBlueShader;
	else le->refEntity.customShader = cgs.media.teleportEffectGreenShader;
}

/*
=====================
CG_GenerateParticles

Generates multiple generic particles
=====================
*/
void CG_GenerateParticles( qhandle_t model, qhandle_t shader, vec3_t pos, float randomPos, vec3_t speed, float randomDir, 
						  float randomSpeed, int numParticles, int owner, int time, int life, int randomLife, int size, 
						  int randomSize, int addSize, int randomAddSize, int flags, int renderfx ) {
	localEntity_t	*le;
	refEntity_t		*re;
	vec3_t	angles;
	vec3_t	axis[3];
	int		i;

	for ( i = 0; i < numParticles; i++ ) {
		// initialize new le
		le = CG_AllocLocalEntity();
		le->leType = LE_MOVE_SCALE_FADE;
		le->startTime = time;
		le->endTime = time + life + crandom()*randomLife;
		le->lifeRate = 1.0 / (le->endTime - le->startTime);
		le->radius = addSize + crandom()*randomAddSize;
		le->leFlags = flags; 
		re = &le->refEntity;
		re->shaderTime = time / 1000.0f;
		re->radius = size + crandom()*randomSize;
		re->renderfx = renderfx;
		le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;

		if ( model ) {
			re->hModel = model;
			re->customShader = shader;
			if ( flags & LEF_AXIS_ALIGNED ) {
				AxisClear( re->axis );
				if ( re->radius ) AxisScale( re->axis, re->radius, re->axis );
			}
		} else {
			re->reType = RT_SPRITE;
			re->customShader = shader;
		}

		// initialize movement
		if ( flags & LEF_GRAVITY ) le->pos.trType = TR_GRAVITY;
		else le->pos.trType = TR_LINEAR;
		if ( flags & LEF_COLLISIONS ) le->leFlags |= LEF_COLLISIONS;
		le->pos.trTime = time;
		VectorSet( le->pos.trBase, pos[0] + crandom()*randomPos, pos[1] + crandom()*randomPos, pos[2] + crandom()*randomPos );
		VectorCopy( le->pos.trBase, le->refEntity.oldorigin );

		vectoangles( speed, angles );
		angles[0] += crandom()*randomDir;
		angles[1] += crandom()*randomDir;
		angles[2] += crandom()*randomDir;
		AnglesToAxis( angles, axis );
		VectorNormalize( axis[0] );
		VectorScale( axis[0], VectorLength(speed) + crandom()*randomSpeed, le->pos.trDelta );
	}
}

/*
==================
CG_ScorePlum
==================
*/
void CG_ScorePlum( int client, vec3_t org, int score ) {
	localEntity_t	*le;
	refEntity_t		*re;
	vec3_t			angles;
	static vec3_t lastPos;

	// only visualize for the client that scored
	if (client != cg.predictedPlayerState.clientNum || cg_scorePlum.integer == 0) {
		return;
	}

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_SCOREPLUM;
	le->startTime = cg.time;
	le->endTime = cg.time + 4000;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	
	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;
	le->radius = score;
	
	VectorCopy( org, le->pos.trBase );
	if (org[2] >= lastPos[2] - 20 && org[2] <= lastPos[2] + 20) {
		le->pos.trBase[2] -= 20;
	}

	//CG_Printf( "Plum origin %i %i %i -- %i\n", (int)org[0], (int)org[1], (int)org[2], (int)Distance(org, lastPos));
	VectorCopy(org, lastPos);


	re = &le->refEntity;

	re->reType = RT_SPRITE;
	re->radius = 16;

	VectorClear(angles);
	AnglesToAxis( angles, re->axis );
}


/*
====================
CG_MakeExplosion
====================
*/
localEntity_t *CG_MakeExplosion( vec3_t origin, vec3_t dir, 
								qhandle_t hModel, qhandle_t shader,
								int msec, qboolean isSprite ) {
	float			ang;
	localEntity_t	*ex;
	int				offset;
	vec3_t			tmpVec, newOrigin;

	if ( msec <= 0 ) {
		CG_Error( "CG_MakeExplosion: msec = %i", msec );
	}

	// skew the time a bit so they aren't all in sync
	offset = rand() & 63;

	ex = CG_AllocLocalEntity();
	if ( isSprite ) {
		ex->leType = LE_SPRITE_EXPLOSION;

		// randomly rotate sprite orientation
		ex->refEntity.rotation = rand() % 360;
		VectorScale( dir, 16, tmpVec );
		VectorAdd( tmpVec, origin, newOrigin );
	} else {
		ex->leType = LE_EXPLOSION;
		VectorCopy( origin, newOrigin );

		// set axis with random rotate
		if ( !dir ) {
			AxisClear( ex->refEntity.axis );
		} else {
			ang = rand() % 360;
			VectorCopy( dir, ex->refEntity.axis[0] );
			RotateAroundDirection( ex->refEntity.axis, ang );
		}
	}

	ex->startTime = cg.time - offset;
	ex->endTime = ex->startTime + msec;

	// bias the time so all shader effects start correctly
	ex->refEntity.shaderTime = ex->startTime / 1000.0f;

	ex->refEntity.hModel = hModel;
	ex->refEntity.customShader = shader;

	// set origin
	VectorCopy( newOrigin, ex->refEntity.origin );
	VectorCopy( newOrigin, ex->refEntity.oldorigin );

	ex->color[0] = ex->color[1] = ex->color[2] = 1.0;

	return ex;
}


/*
=================
CG_Bleed

This is the spurt of blood when a character gets hit
=================
*/
void CG_Bleed( vec3_t origin, int entityNum )
{
	//TODO? ... hier könnte man das mit den >>SCHMERZ<< sprites machen
	return;
/*
	localEntity_t	*ex;

	if ( !cg_blood.integer ) {
		return;
	}

	ex = CG_AllocLocalEntity();
	ex->leType = LE_EXPLOSION;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 500;
	
	VectorCopy ( origin, ex->refEntity.origin);
	ex->refEntity.reType = RT_SPRITE;
	ex->refEntity.rotation = rand() % 360;
	ex->refEntity.radius = 24;

	ex->refEntity.customShader = cgs.media.bloodExplosionShader;

	// don't show player's own blood in view
	if ( entityNum == cg.snap->ps.clientNum ) {
		ex->refEntity.renderfx |= RF_THIRD_PERSON;
	}
*/
}



/*
==================
CG_LaunchGib
==================
*/
void CG_LaunchGib( vec3_t origin, vec3_t velocity, qhandle_t hModel ) {
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FRAGMENT;
	le->startTime = cg.time;
	le->endTime = le->startTime + 5000 + random() * 3000;

	VectorCopy( origin, re->origin );
	AxisCopy( axisDefault, re->axis );
	re->hModel = hModel;

	le->pos.trType = TR_GRAVITY;
	VectorCopy( origin, le->pos.trBase );
	VectorCopy( velocity, le->pos.trDelta );
	le->pos.trTime = cg.time;

	le->bounceFactor = 0.6f;

	le->leBounceSoundType = LEBS_BLOOD;
	le->leMarkType = LEMT_BLOOD;
}

/*
===================
CG_GibPlayer

Generated a bunch of gibs launching out from the bodies location
===================
*/
#define	GIB_VELOCITY	250
#define	GIB_JUMP		250
void CG_GibPlayer( vec3_t playerOrigin )
{
	return;
/*
	vec3_t	origin, velocity;

	if ( !cg_blood.integer ) {
		return;
	}

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	if ( rand() & 1 ) {
		CG_LaunchGib( origin, velocity, cgs.media.gibSkull );
	} else {
		CG_LaunchGib( origin, velocity, cgs.media.gibBrain );
	}

	// allow gibs to be turned off for speed
	if ( !cg_gibs.integer ) {
		return;
	}

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, velocity, cgs.media.gibAbdomen );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, velocity, cgs.media.gibArm );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, velocity, cgs.media.gibChest );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, velocity, cgs.media.gibFist );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, velocity, cgs.media.gibFoot );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, velocity, cgs.media.gibForearm );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, velocity, cgs.media.gibIntestine );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, velocity, cgs.media.gibLeg );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, velocity, cgs.media.gibLeg );
*/
}

/*
==================
CG_LaunchGib
==================
*/
void CG_LaunchExplode( vec3_t origin, vec3_t velocity, qhandle_t hModel ) {
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FRAGMENT;
	le->startTime = cg.time;
	le->endTime = le->startTime + 10000 + random() * 6000;

	VectorCopy( origin, re->origin );
	AxisCopy( axisDefault, re->axis );
	re->hModel = hModel;

	le->pos.trType = TR_GRAVITY;
	VectorCopy( origin, le->pos.trBase );
	VectorCopy( velocity, le->pos.trDelta );
	le->pos.trTime = cg.time;

	le->bounceFactor = 0.1f;

	le->leBounceSoundType = LEBS_BRASS;
	le->leMarkType = LEMT_NONE;
}

#define	EXP_VELOCITY	100
#define	EXP_JUMP		150
/*
===================
CG_GibPlayer

Generated a bunch of gibs launching out from the bodies location
===================
*/
void CG_BigExplode( vec3_t playerOrigin ) {

	return;
	/*vec3_t	origin, velocity;

	if ( !cg_blood.integer ) {
		return;
	}

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY;
	velocity[1] = crandom()*EXP_VELOCITY;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY;
	velocity[1] = crandom()*EXP_VELOCITY;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*1.5;
	velocity[1] = crandom()*EXP_VELOCITY*1.5;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*2.0;
	velocity[1] = crandom()*EXP_VELOCITY*2.0;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*2.5;
	velocity[1] = crandom()*EXP_VELOCITY*2.5;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );
	*/
}

