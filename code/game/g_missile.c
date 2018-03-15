// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "g_local.h"

#define	MISSILE_PRESTEP_TIME	50

/*
================
G_BounceMissile

================
*/
void G_BounceMissile( gentity_t *ent, trace_t *trace ) {
	vec3_t	velocity;
	float	dot;
	int		hitTime;

	// reflect the velocity on the trace plane
	hitTime = level.previousTime + ( level.time - level.previousTime ) * trace->fraction;
	BG_EvaluateTrajectoryDelta( &ent->s.pos, hitTime, velocity );
	dot = DotProduct( velocity, trace->plane.normal );
	VectorMA( velocity, -2*dot, trace->plane.normal, ent->s.pos.trDelta );
	SnapVector( ent->s.pos.trDelta );

	G_AddEvent( ent, EV_STICKY_BOUNCE, DirToByte( trace->plane.normal ) );

	ent->s.pos.trTime = level.time;

	if ( ent->s.eFlags & (EF_BOUNCE | EF_BOUNCE_HALF) ) {
		VectorScale( ent->s.pos.trDelta, 0.8, ent->s.pos.trDelta );
		// check for stop
		if ( trace->plane.normal[2] > 0.2 && VectorLength( ent->s.pos.trDelta ) < 40 ) {
			G_SetOrigin( ent, trace->endpos );
			ent->s.time = level.time / 4;	//https://bugzilla.icculus.org/show_bug.cgi?id=5053
			return;
		}
		if ( ent->s.eFlags & EF_BOUNCE && ent->s.eFlags & EF_BOUNCE_HALF ) {
			// sticky jump
			ent->s.pos.trTime += 300 * fabs( dot ) / VectorLength( velocity );
		}
	}

	VectorAdd( ent->r.currentOrigin, trace->plane.normal, ent->r.currentOrigin);
	SnapVector( ent->r.currentOrigin );
	VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
}


/*
================
G_ExplodeMissile

Explode a missile without an impact
================
*/
void G_ExplodeMissile( gentity_t *ent )
{
	vec3_t		dir;
	vec3_t		origin;

	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );
	SnapVector( origin );
	G_SetOrigin( ent, origin );

	// we don't have a valid direction, so just point straight up
	dir[0] = dir[1] = 0;
	dir[2] = 1;

	ent->s.eType = ET_GENERAL;
	G_AddEvent( ent, EV_MISSILE_MISS, DirToByte( dir ) );

	ent->timestamp = level.time;

	if ( ent->s.weapon != WP_IMPERIUS ) ent->freeAfterEvent = qtrue;

	// splash damage
	if ( ent->splashDamage ) {
		if( G_RadiusDamage( ent->r.currentOrigin, ent->parent, ent->splashDamage, ent->splashRadius, ent
			, ent->splashMethodOfDeath ) && ent->parent && ent->parent->client ) {
			ent->parent->client->accuracy_hits++;
		}
	}

	trap_LinkEntity( ent );
}

/*
================
G_MissileImpact
================
*/
void G_MissileImpact( gentity_t *ent, trace_t *trace ) {
	gentity_t		*other;
	qboolean		hitClient = qfalse;
	other = &g_entities[trace->entityNum];

	// check for bounce
	if ( !other->takedamage &&
		( ent->s.eFlags & ( EF_BOUNCE | EF_BOUNCE_HALF ) ) ) {
		G_BounceMissile( ent, trace );
		return;
	}

	// impact damage
	if (other->takedamage) {
		// FIXME: wrong damage direction?
		if ( ent->damage ) {
			vec3_t	velocity;

			if( LogAccuracyHit( other, &g_entities[ent->r.ownerNum] ) ) {
				g_entities[ent->r.ownerNum].client->accuracy_hits++;
				hitClient = qtrue;
			}
			BG_EvaluateTrajectoryDelta( &ent->s.pos, level.time, velocity );
			if ( VectorLength( velocity ) == 0 ) {
				velocity[2] = 1;	// stepped on a grenade
			}
			G_Damage (other, ent, &g_entities[ent->r.ownerNum], velocity,
				ent->s.origin, ent->damage, 
				0, ent->methodOfDeath);
		}
	}

	if(ent->s.weapon==WP_SPLASHER)
	{
		if ( !other->takedamage ) {
			// complete miss
			ent->parent->client->accurateCount = 0;
		} else {
			// check for "impressive" reward sound
			ent->parent->client->accurateCount++;
			if ( ent->parent->client->accurateCount >= 2 ) {
				ent->parent->client->accurateCount--; //-= 2;
//				ent->parent->client->ps.persistant[PERS_IMPRESSIVE_COUNT]++;
//				// add the sprite over the player's head
//				SetAward( ent->parent->client, AWARD_IMPRESSIVE );
				G_AddEvent(ent->parent,EV_HEHE2,0);
			}
		}
	}

	if (!strcmp(ent->classname, "hook")) {
		gentity_t *nent;
		vec3_t v;

		nent = G_Spawn();
		if ( other->takedamage && other->client ) {

			G_AddEvent( nent, EV_MISSILE_HIT, DirToByte( trace->plane.normal ) );
			nent->s.otherEntityNum = other->s.number;

			ent->enemy = other;

			v[0] = other->r.currentOrigin[0] + (other->r.mins[0] + other->r.maxs[0]) * 0.5;
			v[1] = other->r.currentOrigin[1] + (other->r.mins[1] + other->r.maxs[1]) * 0.5;
			v[2] = other->r.currentOrigin[2] + (other->r.mins[2] + other->r.maxs[2]) * 0.5;

			SnapVectorTowards( v, ent->s.pos.trBase );	// save net bandwidth
		} else {
			VectorCopy(trace->endpos, v);
			G_AddEvent( nent, EV_MISSILE_MISS, DirToByte( trace->plane.normal ) );
			ent->enemy = NULL;
		}

		SnapVectorTowards( v, ent->s.pos.trBase );	// save net bandwidth

		nent->freeAfterEvent = qtrue;
		// change over to a normal entity right at the point of impact
		nent->s.eType = ET_GENERAL;
		ent->s.eType = ET_GRAPPLE;

		G_SetOrigin( ent, v );
		G_SetOrigin( nent, v );

		ent->think = Weapon_HookThink;
		ent->nextthink = level.time + FRAMETIME;

		ent->parent->client->ps.pm_flags |= PMF_GRAPPLE_PULL;
		VectorCopy( ent->r.currentOrigin, ent->parent->client->ps.grapplePoint);

		trap_LinkEntity( ent );
		trap_LinkEntity( nent );

		return;
	}

	// is it cheaper in bandwidth to just remove this ent and create a new
	// one, rather than changing the missile into the explosion?

	memcpy(&ent->s.apos,&ent->s.pos,sizeof(trajectory_t));

	if ( other->takedamage && other->client ) {
		G_AddEvent( ent, EV_MISSILE_HIT, DirToByte( trace->plane.normal ) );
		ent->s.otherEntityNum = other->s.number;
	} else if( trace->surfaceFlags & SURF_METALSTEPS ) {
		G_AddEvent( ent, EV_MISSILE_MISS_METAL, DirToByte( trace->plane.normal ) );
	} else {
		G_AddEvent( ent, EV_MISSILE_MISS, DirToByte( trace->plane.normal ) );
	}

	if ( ent->s.weapon != WP_IMPERIUS ) ent->freeAfterEvent = qtrue;

	// change over to a normal entity right at the point of impact
	ent->s.eType = ET_EXPLOSION;
	ent->timestamp = level.time;

	SnapVectorTowards( trace->endpos, ent->s.pos.trBase );	// save net bandwidth

	G_SetOrigin( ent, trace->endpos );

	// splash damage (doesn't apply to person directly hit)
	if ( ent->splashDamage ) {
		if( G_RadiusDamage( trace->endpos, ent->parent, ent->splashDamage, ent->splashRadius, 
			other, ent->splashMethodOfDeath ) ) {
			if( !hitClient ) {
				g_entities[ent->r.ownerNum].client->accuracy_hits++;
			}
		}
	}

	// Instagib Weaponjump
	if ( g_modInstagib.integer && g_modInstagib_WeaponJump.integer )
		Instagib_applyWeaponJumpKnockback( trace->endpos, ent->parent, ent->methodOfDeath );

	trap_LinkEntity( ent );
}

/*
#######################
move_killerducks
#######################
*/
const float duckAttackPos[5] = { 0.0f, 20.0f, -20.0f, 10.0f, -10.0f };
int duckFrame;
int duckNum;

void move_killerducks(gentity_t *ent)
{
	int tmptime;
	trace_t	tr;
	vec3_t	tmpv3, tmpv3_2;
	int		i, opfer;//rofl
	float	opferlenght;//""

//checkjumppads
	int		touch[MAX_GENTITIES];
	int		num;

	int ownerNum = ent->parent->s.number;// ent->r.ownerNum;

	tmpv3[0]=ent->r.currentOrigin[0]+ent->r.mins[0];
	tmpv3[1]=ent->r.currentOrigin[1]+ent->r.mins[1];
	tmpv3[2]=ent->r.currentOrigin[2]+ent->r.mins[2];
	tmpv3_2[0]=ent->r.currentOrigin[0]+ent->r.maxs[0];
	tmpv3_2[1]=ent->r.currentOrigin[1]+ent->r.maxs[1];
	tmpv3_2[2]=ent->r.currentOrigin[2]+ent->r.maxs[2];

	num = trap_EntitiesInBox( tmpv3, tmpv3_2, touch, MAX_GENTITIES );

	for(i=0 ; i<num ; i++ )
	{
		if ( !( g_entities[touch[i]].r.contents & CONTENTS_TRIGGER ) )
			continue;

		if(!Q_stricmp(g_entities[touch[i]].classname,"trigger_push"))
		{
//			Com_Printf("kd im jumppad\n");
			ent->s.pos.trDelta[0]=g_entities[touch[i]].s.origin2[0];
			ent->s.pos.trDelta[1]=g_entities[touch[i]].s.origin2[1];
			ent->s.pos.trDelta[2]=g_entities[touch[i]].s.origin2[2];
		}

		if(!Q_stricmp(g_entities[touch[i]].classname,"trigger_teleport") && !(g_entities[touch[i]].spawnflags & 0x2))
		{
			gentity_t *dest;

			dest = 	G_PickTarget( g_entities[touch[i]].target );
			if (!dest) {
				G_Printf ("Couldn't find teleporter destination\n");
				return;
			}

			ent->r.currentOrigin[0]=dest->s.origin[0];
			ent->r.currentOrigin[1]=dest->s.origin[1];
			ent->r.currentOrigin[2]=dest->s.origin[2]+20;
		}
	}
//CJP end

	opferlenght=(1024.0f*1024.0f);//262144.0f;//(512.0f)²
	opfer=-1;
	for(i=0;i<g_maxclients.integer;i++)
	{
		if(level.clients[i].pers.connected != CON_CONNECTED)
			continue;
		if(i==ownerNum)
			continue;
		if(level.clients[i].ps.stats[STAT_HEALTH]<=0)
			continue;
		if( ( level.clients[i].sess.sessionTeam == TEAM_SPECTATOR ) || LPSDeadSpec( &level.clients[i] ) ) {
			continue;
		}
		
		tmpv3[0]=level.clients[i].ps.origin[0]-ent->r.currentOrigin[0];
		tmpv3[1]=level.clients[i].ps.origin[1]-ent->r.currentOrigin[1];
		tmpv3[2]=(level.clients[i].ps.origin[2]-ent->r.currentOrigin[2])*2.0f;//die höhe soll stärker gewertet werden ...

		tmpv3[0]=tmpv3[0]*tmpv3[0]+tmpv3[1]*tmpv3[1]+tmpv3[2]*tmpv3[2];

		if(tmpv3[0]<opferlenght)
		{
			opfer=i;
			opferlenght=tmpv3[0];
		}
	}

	//check distance of the owner
	if(opfer==-1)
	{
		tmpv3[0]=level.clients[ownerNum].ps.origin[0]-ent->r.currentOrigin[0];
		tmpv3[1]=level.clients[ownerNum].ps.origin[1]-ent->r.currentOrigin[1];
		tmpv3[2]=(level.clients[ownerNum].ps.origin[2]-ent->r.currentOrigin[2])*2.0f;//die höhe soll stärker gewertet werden ...

		tmpv3[0]=tmpv3[0]*tmpv3[0]+tmpv3[1]*tmpv3[1]+tmpv3[2]*tmpv3[2];

		if(tmpv3[0]<opferlenght)
		{
			opfer=ownerNum;//r.ownerNum;
			opferlenght=tmpv3[0];
		}
	}

	if(opfer!=-1 && (level.time-(ent->nextthink - 10000))>500)// in die ersten 1/2 sek. sollen die opfer egal sein
	{
		float tmpf;

		tmpv3[0]=level.clients[opfer].ps.origin[0]-ent->r.currentOrigin[0];
		tmpv3[1]=level.clients[opfer].ps.origin[1]-ent->r.currentOrigin[1];
//			tmpv3[2]=level.clients[opfer].ps.origin[2]-ent->r.currentOrigin[2];
		tmpv3[2]=0.0f;

//:HERBY:ea
		// change direction to the victim slightly to avoid overlapping ducks
		// not a beatiful solution, but it looks better
		tmpv3_2[0] = tmpv3[1];
		tmpv3_2[1] = -tmpv3[0];
		tmpv3_2[2] = 0;
		VectorNormalize( tmpv3_2 );

		if ( duckFrame != level.framenum ) {
			duckNum = 0;
			duckFrame = level.framenum;
		} 
		else duckNum++;
		VectorMA( tmpv3, duckAttackPos[duckNum%5], tmpv3_2, tmpv3 );
//:HERBY:ee

		VectorNormalize(tmpv3);

		if((tmpf=ent->s.pos.trDelta[0]*ent->s.pos.trDelta[0]+ent->s.pos.trDelta[1]*ent->s.pos.trDelta[1])>160000.0f)
		{
			tmpf=1/sqrt(tmpf);//also wenn die wurzel aus >400 0 wird ist eh der weltuntergang nicht mehr weit O_o

			if(ent->s.pos.trDelta[0]*tmpv3[0]+ent->s.pos.trDelta[1]*tmpv3[1]<0.98)//cos<0.98 -> größer ~10° abweichung
			{
				ent->s.pos.trDelta[0]=tmpv3[0]*400.0f;
				ent->s.pos.trDelta[1]=tmpv3[1]*400.0f;
			}

		}
		else
		{
			ent->s.pos.trDelta[0]=tmpv3[0]*400.0f;
			ent->s.pos.trDelta[1]=tmpv3[1]*400.0f;
		}
	}

	ent->s.pos.trType = TR_LINEAR;

	tmptime=level.time-ent->timestamp;

	ent->s.pos.trBase[0]=ent->r.currentOrigin[0];
	ent->s.pos.trBase[1]=ent->r.currentOrigin[1];
	ent->s.pos.trBase[2]=ent->r.currentOrigin[2];
	ent->s.pos.trTime = level.time;

	tmpv3[0]=ent->r.currentOrigin[0];
	tmpv3[1]=ent->r.currentOrigin[1];
	tmpv3[2]=ent->r.currentOrigin[2]-2;

	trap_Trace(&tr,ent->r.currentOrigin,ent->r.mins,ent->r.maxs,tmpv3,ent->s.number,ent->clipmask);

	if(tr.fraction==1.0f)
		ent->s.pos.trDelta[2]-=0.8f*(float)tmptime;
	else if(ent->s.pos.trDelta[2]<0.0f)
//		ent->s.pos.trDelta[2]=0.0f;
	{
//		PM_ClipVelocity (ent->s.pos.trDelta, tr.plane.normal, ent->s.pos.trDelta, OVERCLIP );
		float	backoff;
		float	change;
		int		i;
		
		backoff = DotProduct (ent->s.pos.trDelta, tr.plane.normal);
		
		if ( backoff < 0 ) {
			backoff *= 1.001f;
		} else {
			backoff /= 1.001f;
		}

		for ( i=0 ; i<3 ; i++ ) {
			change = tr.plane.normal[i]*backoff;
			ent->s.pos.trDelta[i] = ent->s.pos.trDelta[i] - change;
		}

		VectorNormalize(ent->s.pos.trDelta);
		ent->s.pos.trDelta[0]*=400.0f;
		ent->s.pos.trDelta[1]*=400.0f;
		ent->s.pos.trDelta[2]*=400.0f;
	}

	tmpv3[0]=ent->s.pos.trBase[0]+ent->s.pos.trDelta[0]*(float)tmptime*0.001f;
	tmpv3[1]=ent->s.pos.trBase[1]+ent->s.pos.trDelta[1]*(float)tmptime*0.001f;
	tmpv3[2]=ent->s.pos.trBase[2]+ent->s.pos.trDelta[2]*(float)tmptime*0.001f-ent->s.pos.trDelta[2]*((float)tmptime*(float)tmptime*0.000001f);//0.001² ... hmm was sollte das //noch mal O_o

	trap_Trace(&tr,ent->s.pos.trBase,ent->r.mins,ent->r.maxs,tmpv3,ent->s.number,ent->clipmask);

//	if(ent->s.pos.trDelta[2]<1)//nicht die richtung ändern wenn wir uns nach oben bewegen (JUMP/JUMP_PAD)
//	{
		if(tr.fraction!=1.0f)
		{
//			if(((ent->s.pos.trDelta[0]*ent->s.pos.trDelta[0]+ent->s.pos.trDelta[1]*ent->s.pos.trDelta[1])<ent->s.pos.trDelta[2]*ent->s.pos.trDelta[2]) &&
//					tr.plane.normal[2]>0.8f)
//			{
//				ent->s.pos.trDelta[2]=0;
//			}
//			else 
			if(tr.contents & CONTENTS_SOLID)//tr.entityNum==ENTITYNUM_WORLD)//!=opfer)
			{
				vec3_t	oldvel;

//check jump
				trace_t	trj;

//				if(tr.plane.normal[2]<1.0f)
//				{
					tmpv3_2[0]=tmpv3[0];
					tmpv3_2[1]=tmpv3[1];
					tmpv3_2[2]=tmpv3[2]+64;
					trap_Trace(&trj,tmpv3_2,ent->r.mins,ent->r.maxs,tmpv3,ent->s.number,ent->clipmask);

					if(trj.entityNum==opfer) trj.startsolid=qtrue; //don't jump on top of the "opfer" ^^
//				}
//				else
//					trj.plane.normal[2]=0.0f;//uah O_o

				if(trj.startsolid==qfalse && trj.plane.normal[2]>0.8f)
				{
//					if(trj.fraction>0.5f)
//					{//uah nich schön ;)
						tr.endpos[0]=trj.endpos[0];
						tr.endpos[1]=trj.endpos[1];
						tr.endpos[2]=trj.endpos[2];
//					}
//					else
//					{
//						ent->s.pos.trDelta[2]+=400;
//					}
				}
				else
				{
//CJ end

					oldvel[0]=ent->s.pos.trDelta[0];
					oldvel[1]=ent->s.pos.trDelta[1];
					oldvel[2]=ent->s.pos.trDelta[2];

					//noch ändern
					CrossProduct(oldvel,tr.plane.normal,tmpv3);
					CrossProduct(tmpv3,tr.plane.normal,ent->s.pos.trDelta);
					VectorNormalize(ent->s.pos.trDelta);
	
					if(tr.plane.normal[2]>0.6f)
					{
						ent->s.pos.trDelta[0]*=-400.0f;
						ent->s.pos.trDelta[1]*=-400.0f;
						ent->s.pos.trDelta[2]*=-400.0f;
					}
					else
					{
						float oldspeed;
						float cosalpha;
	
						oldspeed=VectorNormalize(oldvel);
						VectorNormalize(tr.plane.normal);//noch überlegen ob das weg kann (ist das schon 1 lang?)
	
						cosalpha=oldvel[0]*tr.plane.normal[0]+oldvel[1]*tr.plane.normal[1]+oldvel[2]*tr.plane.normal[2];
	
						ent->s.pos.trDelta[0]=(tr.plane.normal[0]*-2*cosalpha+oldvel[0])*oldspeed;
						ent->s.pos.trDelta[1]=(tr.plane.normal[1]*-2*cosalpha+oldvel[1])*oldspeed;
						ent->s.pos.trDelta[2]=(tr.plane.normal[2]*-2*cosalpha+oldvel[2])*oldspeed;
					}
				}
			}
			//if hitting any non worldent ...//noch, hmm was ist mit den map-ents(türen usw.)
			else if(tr.contents & CONTENTS_BODY)
			{
//:HERBY:ea
				if ( tr.fraction > 0.1f )
					VectorScale( ent->s.pos.trDelta, tr.fraction, ent->s.pos.trDelta );
				else
					ent->s.pos.trType = TR_STATIONARY;
//:HERBY:ee
			}
		}
//	}

	if ( ( tr.entityNum == opfer ) && ( ent->s.time2 <= level.time ) ) {
		G_AddEvent( ent, EV_GENERAL_SOUND, G_SoundIndex( "sounds/weapons/killerducks/bite" ) );
		// TODO: Add dir
		G_Damage( &g_entities[opfer], NULL, ent->parent, NULL, NULL, DAMAGE_KILLERDUCKS_BITE , 0, ent->methodOfDeath );
		ent->s.time2 = ( level.time + 1000 );
	}

	ent->r.currentOrigin[0]=tr.endpos[0];
	ent->r.currentOrigin[1]=tr.endpos[1];
	ent->r.currentOrigin[2]=tr.endpos[2];

	ent->timestamp=level.time;

	trap_LinkEntity( ent );

	G_RunThink( ent );
	return;
}

/*
======================================================================

  >> slickent <<

======================================================================
*/

void think_slickent( gentity_t *ent ) {
	G_FreeEntity( ent );
}

void touch_slickent( gentity_t *self, gentity_t *other, trace_t *trace )
{
//	Com_Printf("touched slickent(self/cNum=%i|other/cNum=%i)\n",self->s.clientNum,other->s.clientNum);
	other->client->last_slickent_touch = level.time;
	other->client->ps.pm_flags |= PMF_TOUCHSLICKENT;
}

void launch_slickent(vec3_t origin, float size, vec3_t normal)
{
	gentity_t	*slickent;

	slickent = G_Spawn();
	VectorCopy(origin,slickent->r.currentOrigin);
//	slickent->r.currentAngles //ach brauchen wir doch nich
	slickent->r.maxs[0] = size*1.25f;
	slickent->r.maxs[1] = size*1.25f;
	slickent->r.maxs[2] = size*0.5f+4;
	slickent->r.mins[0] = -size*1.25f;
	slickent->r.mins[1] = -size*1.25f;
	slickent->r.mins[2] = -size*0.5f;
	slickent->classname = "slickent";//bei den missils steht da auch nur mist =)
	slickent->r.contents = CONTENTS_TRIGGER;//CONTENTS_SOLID;
	slickent->nextthink = level.time +10000;
	slickent->think = think_slickent;
	slickent->touch = touch_slickent;
	slickent->r.svFlags = SVF_USE_CURRENT_ORIGIN/*|SVF_CAPSULE*/;
	slickent->s.eType = ET_GENERAL;//ET_MOVER;
	slickent->s.weapon = 23;//... dann missbrauch ich eben die var *muhaha* ... extreeeeem-varmissbrauching :ugly:
	slickent->s.generic1 = (int)size;//missssssssssssbrauch =)
	slickent->s.time2 = 360*random();
	VectorCopy(origin,slickent->s.origin);
	VectorCopy(normal,slickent->s.angles);
	trap_LinkEntity(slickent);//update the states and activate the touch-event
}

/*
================
G_RunMissile
================
*/
void G_RunMissile( gentity_t *ent ) {
	vec3_t		origin;
	trace_t		tr;
	int			passent;

	if(ent->s.weapon==WP_KILLERDUCKS)
	{
		move_killerducks(ent);
		return;
	}

	// get current position
	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );

	// if this missile bounced off an invulnerability sphere
	if ( ent->target_ent ) {
		passent = ent->target_ent->s.number;
	}
	else {
		// ignore interactions with the missile owner
		passent = ent->r.ownerNum;
	}
	// trace a line from the previous position to the current position
	if ( level.time - ent->s.pos.trTime > 50 )
		trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin, passent, ent->clipmask );
	else
		trap_Trace( &tr, ent->r.currentOrigin, vec3_origin, vec3_origin, origin, passent, ent->clipmask );

	if ( ent->s.weapon == WP_BOASTER ) {
		if ( tr.fraction != 1.0f || tr.startsolid ) {
			gentity_t *other, *attacker;
			other = &g_entities[tr.entityNum];
			attacker = &g_entities[ent->r.ownerNum];

			// do damage
			if ( other->takedamage && ent->damage ) {
				vec3_t delta;
				BG_EvaluateTrajectoryDelta( &ent->s.pos, level.time, delta );
				G_Damage( other, ent, attacker, delta, tr.endpos, 
					ent->damage, 0, ent->methodOfDeath );

				if(other->client) {
					other->client->lastBoasterHitTime = level.time;
					other->client->nextBoasterDoTTick = level.time;
					other->client->lastBoasterHitAttacker = attacker;
				}
			}

			if(tr.entityNum == ENTITYNUM_WORLD && tr.plane.normal[2] > 0.8f) {
				origin[0] = tr.endpos[0];
				origin[1] = tr.endpos[1];
				origin[2] = tr.endpos[2]-64;
				trap_Trace( &tr, tr.endpos, vec3_origin, vec3_origin, origin, passent, ent->clipmask );
				tr.endpos[2] += 2;

				if(!(trap_PointContents(tr.endpos,ENTITYNUM_WORLD)&CONTENTS_BODY))// slick-ent is CONTENTS_TRIGGER, but trap_PointContents says CONTENTS_BODY (maybe it doesn't check r.contents, if the entity is just a min/max-box)
					launch_slickent(tr.endpos,32.0f+16.0f*random(),tr.plane.normal);
			}

			// free the entity
			G_FreeEntity( ent );
		}

		if(!g_transmitSVboastermissiles.integer)
			return; // <-- disables transmission of boaster-missiles (the visual-missiles will be generated on the clientside)
	}

	if ( tr.startsolid || tr.allsolid ) {
		// make sure the tr.entityNum is set to the entity we're stuck in
		// and don't care about big bounding boxes HERBY
		trap_Trace( &tr, ent->r.currentOrigin, vec3_origin, vec3_origin, ent->r.currentOrigin, passent, ent->clipmask );
		tr.fraction = 0;
	}
	else {
		VectorCopy( tr.endpos, ent->r.currentOrigin );
	}

	trap_LinkEntity( ent );

	if ( tr.fraction != 1 ) {
		// never explode or bounce on sky
		if ( tr.surfaceFlags & SURF_NOIMPACT ) {
			// If grapple, reset owner
			if (ent->parent && ent->parent->client && ent->parent->client->hook == ent) {
				ent->parent->client->hook = NULL;
			}
			G_FreeEntity( ent );
			return;
		}
		G_MissileImpact( ent, &tr );
		if ( ent->s.eType != ET_MISSILE ) {
			return;		// exploded
		}
	}
	// check think function after bouncing
	G_RunThink( ent );
}

/*
================
G_RunExplosion
================
*/
void G_RunExplosion( gentity_t *ent ) {
	float frac;

	frac = (float)(level.time - ent->timestamp) / 700;

	if ( frac > 1.0 )
	{
		G_FreeEntity( ent );
		return;
	}

	if ( !ent->pain_debounce_time ) ent->pain_debounce_time = level.time;
	if ( level.time >= ent->pain_debounce_time )
	{
		// do damage
		ent->pain_debounce_time += 100;
		G_RadiusDamage( ent->r.currentOrigin, ent->parent, 400, 
			frac * ent->splashRadius, NULL, ent->splashMethodOfDeath );
	}
}


/*
=================
fire_nipper

=================
*/
gentity_t *fire_nipper (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "bolt_nipper";
	bolt->nextthink = ( level.time + DURATION_NIPPER );
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_NIPPER;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = DAMAGE_NIPPER;
	bolt->methodOfDeath = MOD_NIPPER;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, SPEED_NIPPER, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}	

//=============================================================================

/*
=================
fire_splasher

=================
*/
gentity_t *fire_splasher (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "bolt_splasher";
	bolt->nextthink = ( level.time + DURATION_SPLASHER );
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_SPLASHER;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = DAMAGE_SPLASHER;
	bolt->methodOfDeath = MOD_SPLASHER;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorCopy( start, bolt->s.origin2 );
	VectorScale( dir, SPEED_SPLASHER, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}	

/*
=================
fire_bubbleg

=================
*/
gentity_t *fire_bubbleg (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "bolt_bubbleg";
	bolt->nextthink = ( level.time + DURATION_BUBBLEG );
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_BUBBLEG;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = DAMAGE_BUBBLEG;
	bolt->methodOfDeath = MOD_BUBBLEG;
	bolt->clipmask = MASK_SHOT;
	bolt->s.generic1 = ((int)( 6 * random() ) + 1 ); // FIXME: Magical constant 6 for g_color_table in cg

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, SPEED_BUBBLEG, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}


/*
#######################

  fire_bambamMissile

  should only be used by the BamBam-entity, not directly by the player

#######################
*/

gentity_t *fire_bambamMissile(gentity_t *self, vec3_t start, vec3_t dir, float velocity) {
	gentity_t	*bolt;
	vec3_t angles, forward;

	bolt = G_Spawn();
	bolt->classname = "missile_bambam";
	bolt->nextthink = ( level.time + DURATION_BAMBAM );
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_BAMBAM_MISSILE;
	if(self->team[0] == 'b')
		bolt->s.generic1 = TEAM_BLUE;
	else
		bolt->s.generic1 = TEAM_RED;
	bolt->damage = DAMAGE_BAMBAM;
	bolt->methodOfDeath = MOD_BAMBAM;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time;
	VectorCopy( start, bolt->s.pos.trBase );

	bolt->parent = self; // BamBam missile parent is BamBam. Or should it be player?
	bolt->r.ownerNum = self->parent->s.number;

	// add random spread
	vectoangles(dir, angles);
	angles[PITCH] += ( SPREAD_BAMBAM *  crandom() );
	angles[YAW]   += ( SPREAD_BAMBAM *  crandom() );
	AngleVectors(angles, forward, NULL, NULL);
	VectorNormalize(forward);

	VectorScale( forward, velocity, bolt->s.pos.trDelta ); // velocity ~ 1800
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

	VectorCopy (start, bolt->r.currentOrigin);
	return bolt;
}

/*
=================
fire_boaster
=================
*/
gentity_t *fire_boaster (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "bolt_boaster";
	bolt->nextthink = ( level.time + DURATION_BOASTER );
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_BOASTER;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = DAMAGE_BOASTER;
	bolt->methodOfDeath = MOD_BOASTER;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;
	bolt->r.mins[0]=bolt->r.mins[1]=bolt->r.mins[2]=-8.0f;
	bolt->r.maxs[0]=bolt->r.maxs[1]=bolt->r.maxs[2]=8.0f;

	bolt->s.pos.trType = TR_GRAVITY;
	bolt->s.pos.trTime = level.time - 10;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	dir[2] += 0.15f;
	VectorScale( dir, SPEED_BOASTER, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	//... the transmission of this missiles is disabled in G_RunMissile!

	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}

//=============================================================================

/*
#######################
duck_die
#######################
*/
static void duck_die( gentity_t *ent, gentity_t *inflictor, gentity_t *attacker, int damage, int mod ) {
	ent->think = G_ExplodeMissile;
	ent->nextthink = level.time + 1;
}

/*
#######################
fire_duck
#######################
*/
gentity_t *fire_duck (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;
	trace_t		tr;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "missile_killerduck";
	bolt->nextthink = ( level.time + DURATION_KILLERDUCKS );
	bolt->think = G_ExplodeMissile;
	bolt->timestamp = level.time;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_KILLERDUCKS;
	bolt->r.ownerNum = bolt - g_entities;//self->s.number;
	bolt->parent = self;
	bolt->r.mins[0] = -10.0f;
	bolt->r.mins[1] = -10.0f;
	bolt->r.mins[2] = -10.0f;
	bolt->r.maxs[0] = 10.0f;
	bolt->r.maxs[1] = 10.0f;
	bolt->r.maxs[2] = 10.0f;
	bolt->r.contents = CONTENTS_CORPSE;//CONTENTS_BODY;
	bolt->clipmask = MASK_PLAYERSOLID;//MASK_SHOT
	if(g_KillerduckHealth.integer>0)
	{
		bolt->takedamage=qtrue;
		bolt->health = g_KillerduckHealth.integer;
	}
	else
	{
		bolt->takedamage=qfalse;
	}
	bolt->die = duck_die;
	//dmg-vars vielleicht noch missbrauchen ;)
	bolt->damage = DAMAGE_KILLERDUCKS_IMPACT;
	bolt->splashDamage = SPLASHDMG_KILLERDUCKS;
	bolt->splashRadius = SPLASHRAD_KILLERDUCKS;
	bolt->methodOfDeath = MOD_KILLERDUCKS;
	bolt->splashMethodOfDeath = MOD_KILLERDUCKS;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_GRAVITY;//TR_LINEAR;
	bolt->s.pos.trTime = level.time;// - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
//	VectorCopy( start, bolt->s.pos.trBase );

	//missbrauch ;)
	tr.endpos[0]=start[0]+dir[0]*32.0f;
	tr.endpos[1]=start[1]+dir[1]*32.0f;
	tr.endpos[2]=start[2]+dir[2]*32.0f;

	trap_Trace(&tr,start,bolt->r.mins,bolt->r.maxs,tr.endpos,self->s.number,MASK_SHOT);

	bolt->s.pos.trBase[0]=bolt->r.currentOrigin[0]=tr.endpos[0];
	bolt->s.pos.trBase[1]=bolt->r.currentOrigin[1]=tr.endpos[1];
	bolt->s.pos.trBase[2]=bolt->r.currentOrigin[2]=tr.endpos[2];


	VectorScale( dir, 400, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

//	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}	

//=============================================================================

/*
=================
fire_grenade
=================
*/
gentity_t *fire_grenade (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "missile_balloony";
	bolt->nextthink = ( level.time + DURATION_BALLOONY );
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_BALLOONY;
	bolt->s.eFlags = EF_BOUNCE_HALF | EF_BOUNCE;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = DAMAGE_BALLOONY;
	bolt->splashDamage = SPLASHDMG_BALLOONY;
	bolt->splashRadius = SPLASHRAD_BALLOONY;
	bolt->methodOfDeath = MOD_BALLOONY;
	bolt->splashMethodOfDeath = MOD_BALLOONY_SPLASH;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_GRAVITY;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, 850, bolt->s.pos.trDelta );//herby hatte 850
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

#define GRENADE_SPIN	500.0

	// HERBY
	bolt->s.apos.trType = TR_LINEAR;
	bolt->s.apos.trTime = level.time - MISSILE_PRESTEP_TIME;
	vectoangles( dir, bolt->s.apos.trBase );
	VectorSet( bolt->s.apos.trDelta, crandom()*GRENADE_SPIN, crandom()*GRENADE_SPIN, crandom()*GRENADE_SPIN );
	SnapVector( bolt->s.apos.trBase );
	SnapVector( bolt->s.apos.trDelta );

	VectorSet( bolt->r.mins, -BALLOONY_SIZE, -BALLOONY_SIZE, -BALLOONY_SIZE );
	VectorSet( bolt->r.maxs,  BALLOONY_SIZE,  BALLOONY_SIZE,  BALLOONY_SIZE );

	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}

//=============================================================================


/*
=================
fire_bfg
=================
*/
gentity_t *fire_bfg (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "missile_imperius";
	bolt->nextthink = (level.time + DURATION_IMPERIUS );
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_IMPERIUS;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = DAMAGE_IMPERIUS;
	bolt->splashRadius = SPLASHRAD_IMPERIUS;
	bolt->methodOfDeath = MOD_IMPERIUS;
	bolt->splashMethodOfDeath = MOD_IMPERIUS_SPLASH;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, SPEED_IMPERIUS, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}

/*
=================
explode_imperius
=================
*/
gentity_t *explode_imperius (gentity_t *self) {
	gentity_t	*bolt;
	vec3_t		origin, forward;

	bolt = G_Spawn();
	bolt->classname = "missile_imperius_explode";
	bolt->nextthink = level.time + 10000;
	bolt->think = G_ExplodeMissile;
	bolt->r.svFlags = 0;
	bolt->s.weapon = WP_IMPERIUS;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = DAMAGE_IMPERIUS;
	bolt->splashRadius = SPLASHRAD_IMPERIUS;
	bolt->methodOfDeath = MOD_IMPERIUS;
	bolt->splashMethodOfDeath = MOD_IMPERIUS_SPLASH;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;
	bolt->s.eType = ET_EXPLOSION;
	bolt->timestamp = level.time;

	AngleVectors (self->client->ps.viewangles, forward, NULL, NULL);
	VectorMA( self->s.pos.trBase, 14, forward, origin );
	SnapVector( origin );
	G_SetOrigin( bolt, origin );
	return bolt;
}

//=============================================================================


/*
=================
fire_rocket
=================
*/
gentity_t *fire_rocket (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "missile_betty";
	bolt->nextthink = ( level.time + DURATION_BETTY );
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_BETTY;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = DAMAGE_BETTY;
	bolt->splashDamage = SPLASHDMG_BETTY;
	bolt->splashRadius = SPLASHRAD_BETTY;
	bolt->methodOfDeath = MOD_BETTY;
	bolt->splashMethodOfDeath = MOD_BETTY_SPLASH;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, SPEED_BETTY, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}

/*
=================
fire_grapple
=================
*/
gentity_t *fire_grapple (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*hook;

	VectorNormalize (dir);

	hook = G_Spawn();
	hook->classname = "hook";
	hook->nextthink = level.time + 10000;
	hook->think = Weapon_HookFree;
	hook->s.eType = ET_MISSILE;
	hook->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	hook->s.weapon = WP_GRAPPLING_HOOK;
	hook->r.ownerNum = self->s.number;
	hook->methodOfDeath = MOD_GRAPPLE;
	hook->clipmask = MASK_SHOT;
	hook->parent = self;
	hook->target_ent = NULL;

	hook->s.pos.trType = TR_LINEAR;
	hook->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	hook->s.otherEntityNum = self->s.number; // use to match beam in client
	VectorCopy( start, hook->s.pos.trBase );
	VectorScale( dir, 800, hook->s.pos.trDelta );
	SnapVector( hook->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, hook->r.currentOrigin);

	self->client->hook = hook;

	return hook;
}


/*
=================
fire_kma

=================
*/
gentity_t *fire_kma ( gentity_t *self, vec3_t start, vec3_t dir ) {
	gentity_t	*bolt;

	VectorNormalize ( dir );

	bolt = G_Spawn();
	bolt->classname = "bolt_injector";
	bolt->nextthink = level.time + 10000;
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_KMA97;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = 110;
	bolt->methodOfDeath = MOD_INJECTOR;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorCopy( start, bolt->s.origin2 );
	VectorScale( dir, 100000, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

	VectorCopy ( start, bolt->r.currentOrigin );

	return bolt;
}

