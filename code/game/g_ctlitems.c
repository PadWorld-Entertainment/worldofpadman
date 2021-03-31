
/*

  this c-file contains the code for the special holdable items
  that should be usable in CTL

  BAMBAM
  BOOMiES

*/

#include "g_local.h"


/*
####################### ####################### #######################

  utility

####################### ####################### #######################
*/

static void Touch_triggerChild( gentity_t *ent, gentity_t *other, trace_t *trace ) {
	if(other->client && ent->parent && ent->parent->touch) {
		ent->parent->touch(ent->parent,other,trace);
	}
}

static gentity_t* createTrigger(gentity_t* parent, float w, float h) {
	gentity_t* ent;

	ent = G_Spawn();
	ent->classname = "CTLitemTrigger";
	ent->s.eType = ET_GENERAL;
	ent->s.eFlags = 0;
	ent->parent = parent;

	ent->r.mins[0] = -(ent->r.maxs[0] = w/2);
	ent->r.mins[1] = -(ent->r.maxs[1] = w/2);
	ent->r.mins[2] = -(ent->r.maxs[2] = h/2);

	ent->r.svFlags = SVF_USE_CURRENT_ORIGIN; // is it really needed?
	VectorCopy(parent->s.pos.trBase, ent->r.currentOrigin);
	VectorCopy(ent->r.currentOrigin, ent->s.pos.trBase);

	ent->r.contents = CONTENTS_TRIGGER;//CONTENTS_BODY;

	ent->touch = Touch_triggerChild;

	trap_LinkEntity(ent);

	return ent;
}


// TODO: Extend and use for cartridges
qboolean IsItemSameTeam( gentity_t *item, gentity_t *player ) {
	if ( !item || !player || !player->client ) {
		return qfalse;
	}

	if ( ( item->s.eType == ET_BAMBAM ) || ( item->s.eType == ET_BOOMIES ) ) {
		if ( ( ( item->team[0] == 'r' ) && ( player->client->sess.sessionTeam == TEAM_RED ) ) ||
		     ( ( item->team[0] == 'b' ) && ( player->client->sess.sessionTeam == TEAM_BLUE ) ) ) {
			return qtrue;
		}
	}

	return qfalse;
}

// NOTE: It's "can NOT" damage!
qboolean CantDamageTeamitem( gentity_t *target, gentity_t *attacker ) {
	if ( !attacker->client ) {
		return qfalse;
	}

	if ( ( target->s.eType == ET_BAMBAM ) || ( target->s.eType == ET_BOOMIES ) ) {
		if ( IsItemSameTeam( target, attacker ) ) {
			return ( g_friendlyFire.integer ? qfalse : qtrue );
		}
	}

	return qfalse;
}

qboolean IsBambamBoomieSpotClean( vec3_t spot, gentity_t *pEnt, char* pickupName )
{
	int i;
	int numBoxEnts;
	int boxEnts[10];
	vec3_t boxMins = {-128, -128, -30};
	vec3_t boxMaxs = {128, 128, 64};

	VectorAdd( spot, boxMins, boxMins );
	VectorAdd( spot, boxMaxs, boxMaxs );

	numBoxEnts = trap_EntitiesInBox( boxMins, boxMaxs, boxEnts, 10);

	for(i=0; i<numBoxEnts; i++)
	{
		gentity_t* otherEnt = &g_entities[boxEnts[i]];
		if(	otherEnt->s.eType == ET_BOOMIES )
		{
			float distSqr = DistanceSquared( otherEnt->s.pos.trBase, spot );
			if( distSqr < Square(64) )
			{
				trap_SendServerCommand( ( pEnt - g_entities ), "cp \"Too close to BOOMiE\"" );
				return qfalse;
			}
		}
		else if( otherEnt->s.eType == ET_ITEM && otherEnt->item->giType == IT_WEAPON )
		{
			float distSqr = DistanceSquared( otherEnt->s.pos.trBase, spot );
			if ( ( distSqr < Square( 64 ) ) && !( otherEnt->flags & FL_DROPPED_ITEM ) ) {
				trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Too close to weapon spawnpoint\"" ) );
				return qfalse;
			}
		}
		else if( otherEnt->s.eType == ET_STATION )
		{
			float distSqr = DistanceSquared( otherEnt->s.pos.trBase, spot );
			if( distSqr < Square(128) )
			{
				trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Too close to health station\"" ) );
				return qfalse;
			}
		}
		else if( otherEnt->s.eType == ET_ITEM && 
			( otherEnt->item->giTag == PW_REDFLAG || otherEnt->item->giTag == PW_BLUEFLAG ) )
		{
			float distSqr = DistanceSquared( otherEnt->s.pos.trBase, spot );
			if( distSqr < Square(256) )
			{
				trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Too close to lolly base\"" ) );
				return qfalse;
			}			
		}
		else if( otherEnt->s.eType == ET_PUSH_TRIGGER )
		{
			// triggers have no origin \o/
			vec3_t origin;
			float distSqr;

			VectorAdd( otherEnt->r.mins, otherEnt->r.maxs, origin);
			VectorScale(origin, 0.5, origin);
			distSqr = DistanceSquared( origin, spot  ); 
			if( distSqr < Square(128) )
			{
				trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Too close to jump pad\"" ) );
				return qfalse;
			}
		}
		else if( !Q_stricmp( otherEnt->classname, "target_push" ) ){
			float distSqr = DistanceSquared( otherEnt->s.pos.trBase, spot  ); 
			if( distSqr < Square(128) )
			{
				trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Too close to jump pad\"" ) );
				return qfalse;
			}
		}
		else if( !Q_stricmp( otherEnt->classname, "target_position" ) 
				|| !Q_stricmp( otherEnt->classname, "misc_teleporter_dest" ) )
		{
			float distSqr = DistanceSquared( otherEnt->s.pos.trBase, spot );
			if( distSqr < Square(128) )
			{
				trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Too close to teleporter exit\"" ) );
				return qfalse;
			}
		}
		else if ( !Q_stricmp( otherEnt->classname, "info_player_start" ) 
				|| !Q_stricmp( otherEnt->classname, "info_player_deathmatch" ) )
		{
			float distSqr = DistanceSquared( otherEnt->s.pos.trBase, spot );
			if( distSqr < Square(128) )
			{
				trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Too close to spawn point\"" ) );
				return qfalse;
			}
		}
		else if( !Q_stricmp( otherEnt->classname, "trigger_balloonzone" ) )
		{
			// triggers have no origin \o/
			vec3_t origin;
			float distSqr;

			VectorAdd( otherEnt->r.mins, otherEnt->r.maxs, origin);
			VectorScale(origin, 0.5, origin);
			distSqr = DistanceSquared( origin, spot );
			if( distSqr < Square(128) )
			{
				trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Too close to balloon\"" ) );
				return qfalse;
			}
		}
	}
	return qtrue;
}

/*
####################### ####################### #######################

  BAM BAM

  ent->timestamp	last time the bambam shoot a missile
  ent->team[0]		contains to which player-team this bambam belongs ('b' = blue, 'r' = red)

  ent->s.generic1	BamBam State
  ent->s.time		start of the State

####################### ####################### #######################
*/

#define BAMBAM_IDLE_THINKTIME	(20*1000) // idle -> zzz
static void bambam_touch( gentity_t *ent, gentity_t *other, trace_t *trace ) {

	if(!other->client) return;
	if(!ent->team) return;

	if ( !IsItemSameTeam( ent, other ) && 
	     ( ent->timestamp == 0 || ( level.time - ent->timestamp ) > 100 ) ) {
		trace_t tr;
		vec3_t start;
		vec3_t end;
		float	tmpLen;
		float	missileVelocity = SPEED_BAMBAM;

		VectorCopy(ent->s.pos.trBase,start);
		start[2] += 64.0;

//		BG_EvaluateTrajectory(&other->s.pos,level.time+50, end );
		VectorSubtract(other->s.pos.trBase,start,end); // 'end' temporary used for distance calculation
		tmpLen = VectorLength(end);

		if ( other->client->ps.powerups[PW_VISIONLESS] && ( tmpLen > RANGE_BAMBAM_VISIONLESS ) ) {
			// I'm the invisible man I'm the invisible man
			// Incredible how you can see right through me
			return;
		}

		VectorMA(other->s.pos.trBase, (tmpLen/missileVelocity), other->s.pos.trDelta, end);
		trap_Trace(&tr,start,NULL,NULL,end,ent - g_entities,MASK_SHOT);

		if(tr.fraction == 1.0f || tr.entityNum==(other-g_entities)) {
			gentity_t	*m;
			vec3_t	dir;

			// Wake up when inactive, don't shoot yet
			if ( ent->s.generic1 == BBS_IDLE ) {
				ent->s.generic1	= BBS_IDLE2SHOOTING;
				ent->nextthink	= ( level.time + FIRESTART_BAMBAM );
				return;
			}

			if ( ent->s.generic1 != BBS_SHOOTING ) {
				return;
			}
			else {
				ent->nextthink	= ( level.time + FIREDELAY_BAMBAM );
			}


			// ~hack~ to use only one VectorNormalize, it should work because the 'end' isn't that different to 'other->s.pos.trBase'
			VectorSubtract(end,start,dir);
//			if(VectorNormalize(dir)>0.0f) {
			if(tmpLen>0.0f) {
				VectorScale(dir,1/tmpLen,dir);
				m = fire_bambamMissile( ent, start, dir, missileVelocity );
//				m->damage /= 20; // reduce dmg for testing
				G_AddEvent( ent, EV_GENERAL_SOUND, G_SoundIndex("sounds/items/bambam/shoot") );

				ent->timestamp = level.time;
			}
		}
	}
}

static void bambam_die( gentity_t *ent, gentity_t *inflictor, gentity_t *attacker, int damage, int mod ) {
	vec3_t		dir;

	G_FreeEntity(ent->nextTrain);//NOTE: nextTain == triggerChildEnt
	ent->takedamage = qfalse;

	dir[0] = dir[1] = 0;
	dir[2] = 1;
	ent->s.modelindex = 0;
	ent->s.eType = ET_GENERAL;
	G_AddEvent( ent, EV_BAMBAM_EXPLOSION, DirToByte( dir ) );
	AI_RemoveBambam(ent);

	if( ent->team[0] == 'r' )
		level.numBambams[TEAM_RED]--;
	else if( ent->team[0] == 'b' )
		level.numBambams[TEAM_BLUE]--;

	ent->freeAfterEvent = qtrue;
}


static void bambam_think( gentity_t *ent ) {

	switch(ent->s.generic1) {
	case BBS_INACTIVE:
		ent->s.generic1 = BBS_BUILDING;
		ent->s.time		= level.time;
		ent->nextthink	= level.time + 2000;
		break;
	case BBS_BUILDING:
		ent->s.generic1 = BBS_IDLE;
		ent->s.time		= level.time;
		ent->nextthink	= level.time + BAMBAM_IDLE_THINKTIME;
		break;
	case BBS_IDLE:
		ent->s.generic1 = BBS_ZZZ;
		ent->s.time		= level.time;
		ent->nextthink	= level.time + 3760;
		break;
	case BBS_ZZZ:
		ent->s.generic1 = BBS_IDLE;
		ent->s.time		= level.time;
		ent->nextthink	= level.time + BAMBAM_IDLE_THINKTIME;
		break;
	case BBS_IDLE2SHOOTING:
		ent->s.generic1	= BBS_SHOOTING;
		ent->nextthink	= ( level.time + FIREDELAY_BAMBAM );
		break;
	case BBS_SHOOTING:
		ent->s.generic1	= BBS_SHOOTING2IDLE;
		ent->s.time		= level.time;
		ent->nextthink	= level.time + 1;
		break;
	case BBS_SHOOTING2IDLE:
		ent->s.generic1	= BBS_IDLE;
		ent->nextthink	= level.time + BAMBAM_IDLE_THINKTIME;
		break;
	}

	if(ent->s.generic1==BBS_ZZZ) {
		G_AddEvent( ent, EV_GENERAL_SOUND, G_SoundIndex("sounds/items/bambam/zzz") );
	}
}

static void bambam_pain( gentity_t *self, gentity_t *attacker, int damage ) {
	self->s.angles2[2] = (float)( (float)self->health / HEALTH_BAMBAM );
}

/*
	tries to create a BamBam
	returns false if it fails
	... doesn't handle bambam-holdable stuff!
*/
static const vec3_t bambamMin = { -22.0, -22.0,   0.0 };
static const vec3_t bambamMax = {  22.0,  22.0,  38.0 };
qboolean bambam_createByPlayer( gentity_t *pEnt, char* pickupName ) {
	vec3_t forward;
	vec3_t start;
	vec3_t end;
	vec3_t tmpAngles;
	trace_t	tr;

	vec3_t boxMins = {-128, -128, -30};
	vec3_t boxMaxs = {128, 128, 64};
	int boxEnts[10];
	int numBoxEnts;
	int i;
	gentity_t* entBam;

	if( MAX_TEAM_BAMBAMS <= level.numBambams[ pEnt->client->sess.sessionTeam ] )
	{
		trap_SendServerCommand( pEnt - g_entities, va("cp \"Your team has reached its %s limit.\n\"", pickupName) );
		return qfalse;
	}

	VectorClear( tmpAngles );
	tmpAngles[1] = pEnt->s.apos.trBase[1];
	AngleVectors( tmpAngles, forward, NULL, NULL );

	VectorCopy( pEnt->s.pos.trBase, start );
	start[2] += pEnt->client->ps.viewheight;
	VectorMA( start, 64.0, forward, end );

	trap_Trace( &tr, start, bambamMin, bambamMax, end, ( pEnt - g_entities ), MASK_SHOT );
	if ( tr.allsolid || tr.startsolid ) {
		trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Can't build %s here\"", pickupName ) );
		return qfalse;
	}

	// Make a trace without bbox, since there are problems with bbox size/curves and wrong startsolid results
	trap_Trace( &tr, start, NULL, NULL, tr.endpos, ( pEnt - g_entities ), MASK_SHOT );
	if ( tr.fraction != 1.0 ) {
		trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Can't build %s here\"", pickupName ) );
		return qfalse;
	}

	VectorCopy( tr.endpos, start );
	VectorCopy( tr.endpos, end );
	end[2] -= 256;

	trap_Trace( &tr, start, bambamMin, bambamMax, end, ENTITYNUM_NONE, MASK_SHOT );

	if(  tr.allsolid || tr.startsolid || tr.fraction >= 1.f || tr.entityNum != ENTITYNUM_WORLD ){
		trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Can't build %s here\"", pickupName ) );
		return qfalse;
	}

	if( !IsBambamBoomieSpotClean( tr.endpos, pEnt, pickupName ) )
		return qfalse;

	// all good, build it
	entBam = G_Spawn();
	entBam->classname = "BamBam";
	entBam->s.eType = ET_BAMBAM;
	
	if ( pEnt->client->sess.sessionTeam == TEAM_BLUE ) {
		entBam->s.modelindex = G_ModelIndex( "models/weapons2/bambam/bambam_blue.md3" );
		entBam->team = "b";
		entBam->s.time2 = TEAM_BLUE;
	}
	else {
		entBam->s.modelindex = G_ModelIndex( "models/weapons2/bambam/bambam_red.md3" );
		entBam->team = "r";
		entBam->s.time2 = TEAM_RED;
	}

	entBam->s.angles[1] = pEnt->s.apos.trBase[1];
	entBam->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	G_SetOrigin( entBam, tr.endpos );

	VectorCopy( bambamMin, entBam->r.mins );
	VectorCopy( bambamMax, entBam->r.maxs );
	entBam->r.contents = CONTENTS_BODY;

	entBam->die = bambam_die;
	entBam->takedamage = qtrue;
	entBam->health = HEALTH_BAMBAM;

	entBam->parent = pEnt;

	// Needed for clientside bambam health bar, abuse some vars
	entBam->s.angles2[2] = (float)( (float)entBam->health / HEALTH_BAMBAM );
	entBam->pain = bambam_pain;

	entBam->think = bambam_think;
	entBam->nextthink = ( level.time + FRAMETIME );

	entBam->touch = bambam_touch;
	entBam->nextTrain = createTrigger( entBam, RANGE_BAMBAM_WIDTH, RANGE_BAMBAM_HEIGHT );

	trap_LinkEntity( entBam );

	G_AddEvent( entBam, EV_GENERAL_SOUND, G_SoundIndex( "sounds/items/bambam/start" ) );

	level.numBambams[ pEnt->client->sess.sessionTeam ]++;

	AI_AddBambam(pEnt, entBam);

	return qtrue;
}

/*
####################### ####################### #######################

  BOOMiES

  ent->s.time	start time (to do animation in the first 15/25 sec)

####################### ####################### #######################
*/
static void boomies_die( gentity_t *ent, gentity_t *inflictor, gentity_t *attacker, int damage, int mod ) {
	vec3_t dir;

	G_FreeEntity(ent->nextTrain);//NOTE: nextTain == triggerChildEnt
	ent->takedamage = qfalse;

	dir[0] = dir[1] = 0;
	dir[2] = 1;
	ent->s.modelindex = 0;
	ent->s.eType = ET_GENERAL;
	G_AddEvent( ent, EV_BOOMIES_EXPLOSION, DirToByte( dir ) );

	AI_RemoveBoomie(ent);

	ent->freeAfterEvent = qtrue;

	if( ent->team[0] == 'r' )
		level.numBoomies[TEAM_RED]--;
	else if( ent->team[0] == 'b' )
		level.numBoomies[TEAM_BLUE]--;

	G_RadiusDamage( ent->s.pos.trBase, ent->parent, SPLASHDMG_BOOMIE, SPLASHRAD_BOOMIE, NULL, MOD_BOOMIES );
}


static void boomies_touch( gentity_t *ent, gentity_t *other, trace_t *trace ) {
	if ( !other->client ) {
		return;
	}

	if ( !IsItemSameTeam( ent, other ) ) {
		// EXPLODE! goes the wiesel
		boomies_die( ent, other, other, 999, MOD_SUICIDE );
	}
}

/*
	create a Boomy(? or is it Boomie °°)
	trace forward .... if hitting a wall, place it there ... else drop to ground
	... doesn't handle boomies-holdable stuff!
*/
qboolean boomies_createByPlayer( gentity_t *pEnt, char* pickupName ) {
	vec3_t forward;
	vec3_t start;
	vec3_t end;
	trace_t	tr;

	if( MAX_TEAM_BOOMIES <= level.numBoomies[ pEnt->client->sess.sessionTeam ] )
	{
		trap_SendServerCommand( pEnt - g_entities, va("cp \"Your team has reached its %s limit.\n\"", pickupName) );	// should be item->pickup_name
		return qfalse;
	}

	VectorCopy(pEnt->s.pos.trBase,start);
	start[2] += pEnt->client->ps.viewheight;

	AngleVectors (pEnt->s.apos.trBase, forward, NULL, NULL);

	//VectorMA(v, s, b, o) ... (o)[0]=(v)[0]+(b)[0]*(s)
	VectorMA(start, 64.0f, forward, end);

	trap_Trace(&tr,start,NULL,NULL,end,(pEnt - g_entities),MASK_SHOT);
	if ( tr.allsolid || tr.startsolid ) 
	{
		trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Can't build %s here\"", pickupName ) );	// should be item->pickup_name
		return qfalse;
	}

	if( !IsBambamBoomieSpotClean( tr.endpos, pEnt, pickupName ) )
		return qfalse;

	if(tr.fraction<1.0f && tr.entityNum==ENTITYNUM_WORLD) {
		gentity_t* ent;

		ent = G_Spawn();
		ent->classname = "Boomy";
		ent->s.eType = ET_BOOMIES;
		ent->s.eFlags = 0;

		if(pEnt->client->sess.sessionTeam == TEAM_BLUE) {
			ent->s.modelindex = G_ModelIndex("models/weapons2/boomies/boomies_blue.md3");
			ent->team = "b";
		}
		else {
			ent->s.modelindex = G_ModelIndex("models/weapons2/boomies/boomies_red.md3");
			ent->team = "r";
		}

		ent->r.svFlags = SVF_USE_CURRENT_ORIGIN; // is it really needed?
		G_SetOrigin( ent, tr.endpos );

		vectoangles(tr.plane.normal,ent->s.angles);

		VectorSet( ent->r.mins, -16, -16, -16 );
		VectorSet( ent->r.maxs,  16,  16,  16 );

		ent->r.contents = CONTENTS_CORPSE;

		ent->die = boomies_die;
		ent->takedamage = qtrue;
		ent->health = HEALTH_BOOMIE;

		ent->s.time = level.time;

		VectorCopy(ent->r.currentOrigin, ent->s.pos.trBase);

		ent->touch = boomies_touch;
		ent->nextTrain = createTrigger( ent, RANGE_BOOMIE_WIDTH,  RANGE_BOOMIE_HEIGHT );

		//ent->r.ownerNum = pEnt->s.number;	// owner can't kill the boomie with non-splashdmg weapons if this is set
		ent->parent = pEnt;

		trap_LinkEntity(ent);

		G_AddEvent( ent, EV_GENERAL_SOUND, G_SoundIndex("sounds/items/boomies/start") );

		level.numBoomies[ pEnt->client->sess.sessionTeam ]++;

		AI_AddBoomie(pEnt, ent );

		return qtrue;
	}

	trap_SendServerCommand( ( pEnt - g_entities ), va( "cp \"Can't build %s here\"", pickupName ) );	// should be item->pickup_name

	return qfalse;
}

