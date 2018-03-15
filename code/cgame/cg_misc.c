/*
	TODO: Write some sort of header, GPL etc.
*/

#include "cg_local.h"

/*
	Called in CG_UpdateCvars() to enforce limits an all sort of cvars.
	Mainly useful for cvars in the engine.
*/
void CG_LimitCvars( void ) {
	int i;

	// Haters gonna hate

	/* NOTE: Assuming nowadays (2011) most computers run fast engough for 100+ frames
	         and are connected via ADSL/Cable, we can safely enforce some limits which
	         will result in benefits for all players.
	*/

	/* TODO: There are still some settings left, such as cl_timenudge, cl_packetdup etc.
	         Maybe check for default com_maxfps of 85 and then set to 76?
	*/
	/* FIXME: Maybe print some info when vars are out of range?	*/

	if ( CG_GetCvarInt( "sv_running" ) ) {
		return;
	}

	trap_Cvar_Set( "snaps", "40" );

	if ( CG_GetCvarInt( "rate" ) < 25000 ) {
		trap_Cvar_Set( "rate", "25000" );
	}

	i = CG_GetCvarInt( "com_maxfps" );
	if ( i < 43 ) {
		i = 43;
		trap_Cvar_Set( "com_maxfps", "43" );
	}
	else if ( i > 125 ) {
		i = 125;
		trap_Cvar_Set( "com_maxfps", "125" );
	}

	if ( i % CG_GetCvarInt( "cl_maxpackets" ) ) {
		// TODO: Maybe use a more sane approach such as i/2+1 ?
		//       Beware of lower bound
		trap_Cvar_Set( "cl_maxpackets", va( "%d", i ) );
	}
}


/*
	Depending on cvar settings, returns a formatted timestamp
*/
#define	TIMESTAMP_TIMER		1
#define TIMESTAMP_REALTIME	2
char* CG_Timestamp( char *timestamp, unsigned int size ) {
	if ( cg_timestamps.integer == TIMESTAMP_TIMER ) {
		int	mins, seconds, tens;
		int msec;

		msec = ( cg.time - cgs.levelStartTime );

		seconds	=  ( msec / 1000 );
		mins	=  ( seconds / 60 );
		seconds	-= ( mins * 60 );
		tens	=  ( seconds / 10 );
		seconds -= ( tens * 10 );

		Com_sprintf( timestamp, size, S_COLOR_YELLOW"%i:%i%i "S_COLOR_WHITE, mins, tens, seconds );
	}
	else if ( cg_timestamps.integer == TIMESTAMP_REALTIME ) {
		qtime_t		qtime;

		trap_RealTime( &qtime );

		Com_sprintf( timestamp, size, S_COLOR_YELLOW"%i:%i%i "S_COLOR_WHITE,
		             qtime.tm_hour, ( ( qtime.tm_min / 10 ) % 10 ), ( qtime.tm_min % 10 ) );
	}
	else {
		Q_strncpyz( timestamp, "", size );
	}

	return timestamp;
}


/*
	Queries the value of a client cvar
*/
void CG_QueryCvar( void ) {
	char query[32];
	char value[MAX_STRING_CHARS];
	char buffer[MAX_STRING_CHARS];

	if ( trap_Argc() < 2 ) {
		return;
	}

	trap_Argv( 1, query, sizeof( query ) );

	trap_Cvar_VariableStringBuffer( query, value, sizeof( value ) );

	Com_sprintf( buffer, sizeof( buffer ), "qcr \"%s\" \"%s\"\n", query, value );

	trap_SendClientCommand( buffer );
}


/*
===============
CG_ColorByEntityType
by: hika
2006-12-28

Only used by CG_AddBoundingBoxEntity.
Don't question color choices, they're completely arbitrary ;)
===============
*/
static void CG_ColorByEntityType( int eType, byte colors[4] ) {
	switch ( eType ) {
		default:
			colors[0] = 220;
			colors[1] = 220;
			colors[2] = 100;
			colors[3] = 255;
			break;

		case ET_GENERAL:
			colors[0] = 255;
			colors[1] = 0;
			colors[2] = 0;
			colors[3] = 255;
			break;
		case ET_PLAYER:
			colors[0] = 192;
			colors[1] = 0;
			colors[2] = 0;
			colors[3] = 255;
			break;
		case ET_ITEM:
			colors[0] = 128;
			colors[1] = 0;
			colors[2] = 0;
			colors[3] = 255;
			break;
		case ET_MISSILE:
			colors[0] = 64;
			colors[1] = 0;
			colors[2] = 0;
			colors[3] = 255;
			break;
		case ET_MOVER:
			colors[0] = 0;
			colors[1] = 255;
			colors[2] = 0;
			colors[3] = 255;
			break;
		case ET_BEAM:
			colors[0] = 0;
			colors[1] = 192;
			colors[2] = 0;
			colors[3] = 255;
			break;
		case ET_PORTAL:
			colors[0] = 0;
			colors[1] = 128;
			colors[2] = 0;
			colors[3] = 255;
			break;
		case ET_SPEAKER:
			colors[0] = 0;
			colors[1] = 64;
			colors[2] = 0;
			colors[3] = 255;
			break;
		case ET_PUSH_TRIGGER:
			colors[0] = 0;
			colors[1] = 0;
			colors[2] = 255;
			colors[3] = 255;
			break;
		case ET_TELEPORT_TRIGGER:
			colors[0] = 0;
			colors[1] = 0;
			colors[2] = 192;
			colors[3] = 255;
			break;
		case ET_INVISIBLE:
			colors[0] = 0;
			colors[1] = 0;
			colors[2] = 128;
			colors[3] = 255;
			break;
		
		// ET_GRAPPLE
		// ET_TEAM
		// ET_EXPLOSION
		// ET_BALLOON
		// ET_BAMBAM
		// ET_BOOMIES
		}
}


/*
=================
TransposeMatrix
by: hika
2006-12-30

Imported from Q3 Engine ./code/qcommon/cm_trace.c
=================
*/
static void TransposeMatrix( /*const*/ vec3_t matrix[3], vec3_t transpose[3] ) {
	int i, j;
	for ( i = 0; i < 3; i++ ) {
		for ( j = 0; j < 3; j++ ) {
			transpose[i][j] = matrix[j][i];
		}
	}
}


/*
=================
CG_EntityVectors
by: hika
2006-12-30

Get origin vector and bounding box vextex vectors from an entity.
If box_vertex is NULL, it will be ignored.
=> qfalse, if the entity has no bounds vertex.

The bounds index are in the following order.

    2+------+3     ^ z
    /|     /|      |  y
  1/ |   0/ |      |/
  +------+  |      +--> x
  | 6+---|--+7
  | /    | /
  |/     |/
 5+------+4

=================
*/
static qboolean CG_EntityVectors( const centity_t *cent, vec3_t origin, vec3_t box_vertex[8] ) {
	float *vmodel;
	vec3_t bmaxs, bmins;
	const entityState_t *ent;
	clipHandle_t cmodel;
	vec3_t angles, r, vertex, rotate_origin = { 0.0f, 0.0f, 0.0f }, median, ext_xyz;
	vec3_t matrix[3], transpose[3];
	int i, x, zd, zu;
	qboolean rotate = qfalse;
	qboolean hasBounds = qtrue;

	if ( !origin ) {
		// Output parameter origin has to be present
		return qfalse;
	}

	ent = &cent->currentState;

	if ( ent->solid == SOLID_BMODEL ) {
		vmodel = cgs.inlineModelMidpoints[ ent->modelindex ];
		VectorAdd( cent->lerpOrigin, vmodel, origin );

		if ( !box_vertex ) {
			// Has bounds
			return qtrue;
		}

		cmodel = cgs.inlineDrawModel[ ent->modelindex ];
		trap_R_ModelBounds( cmodel, bmins, bmaxs );

		for ( i = 0; i < 3; i++ ) {
			// Store a vector for one box side
			ext_xyz[i] = ( bmaxs[i] - bmins[i] );

			// And store a *positive* vector, from origin to one vertex corner.
			median[i] = ( bmaxs[i] - ( bmaxs[i] + bmins[i] ) * 0.5 );
		}

		VectorCopy( cent->lerpAngles, angles );
		// Rotate only solid models ?
		if  ( angles[0] || angles[1] || angles[2] ) {
			rotate = qtrue;
			VectorCopy( cent->lerpOrigin, rotate_origin );
		}
	}
	else if ( ent->solid ) {
		VectorCopy( cent->lerpOrigin, origin );

		if ( !box_vertex ) {
			// Has bounds
			return qtrue;
		}

		// encoded bbox
		x = ( ent->solid & 255 );
		zd = ( ( ent->solid >> 8 ) & 255 );
		zu = ( ( ( ent->solid >> 16 ) & 255 ) - 32 );

		median[0] = x;
		median[1] = x;
		median[2] = zu;

		ext_xyz[0] = ( 2 * x );
		ext_xyz[1] = ( 2 * x );
		ext_xyz[2] = ( zu + zd );
	}
	else {
		// Not a solid
		VectorCopy( cent->lerpOrigin, origin );

		if ( !box_vertex ) {
			// No bounds
			return qfalse;
		}

		// Default values
		median[0] = 3;
		median[1] = 3;
		median[2] = 3;

		ext_xyz[0] = 6;
		ext_xyz[1] = 6;
		ext_xyz[2] = 6;

		hasBounds = qfalse;
	}

	// Vertex bounds for the "horizontal" top box.
	VectorAdd( origin, median, box_vertex[3] );	// Top forward right

	VectorCopy( box_vertex[3], box_vertex[2] );	// Top forward left
	box_vertex[2][0] -= ext_xyz[0];

	VectorCopy( box_vertex[2], box_vertex[1] );	// Top backward left
	box_vertex[1][1] -= ext_xyz[1];

	VectorCopy( box_vertex[1], box_vertex[0] );	// Top backward right
	box_vertex[0][0] += ext_xyz[0];

	// Vertex bounds for the "horizontal" bottom box.
	for ( i = 0; i < 4; i++ ) {
		VectorCopy( box_vertex[i], box_vertex[i + 4] );
		box_vertex[i + 4][2] -= ext_xyz[2];
	}

	// Rotate all the bounds, if needed
	if (rotate) {
		// Get the entity axis's point of view.
		AnglesToAxis( angles, matrix );
		// Transpose matrix as it is the entity axis point of view, which rotate.
		// Consider that it is not each bounds that rotate, but the entity axis.
		// So, if the entity axis rotate in one direction, the bounds will be rotate in the opposite,
		// from the entity axis point of view.
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		TransposeMatrix( matrix, transpose );
		for ( i = 0; i < 8; i++ ) {
			// Rotate should be done from the rotation origin, not the entity origin
			VectorSubtract( box_vertex[i], rotate_origin, vertex );
			VectorRotate( vertex, transpose, r );

			// Apply the rotation result.
			VectorAdd( rotate_origin, r, box_vertex[i] );
		}
	}

	return hasBounds;
}


/*
=================
CG_AddBoundingBoxEntity
by: hika
2006-12-29

Same as CG_AddBoundingBox in cg_unlagged, except that it can be used for an
entity other than a player.
=================
*/
void CG_AddBoundingBoxEntity( const centity_t *cent ) {
	polyVert_t verts[4];
	int i;
	vec3_t box_vertex[8];
	qhandle_t bboxShader, bboxShader_nocull;
	const entityState_t *ent;
	vec3_t origin;

	ent = &cent->currentState;

	if ( !cg_drawBBox.integer ) {
		return;
	}

	// get the shader handles
	bboxShader = trap_R_RegisterShader( "bbox" );
	bboxShader_nocull = trap_R_RegisterShader( "bbox_nocull" );

	// if they don't exist, forget it
	if ( !bboxShader || !bboxShader_nocull ) {
		return;
	}

	CG_EntityVectors( cent, origin, box_vertex );

	// set the polygon's texture coordinates
	verts[0].st[0] = 0;
	verts[0].st[1] = 0;
	verts[1].st[0] = 0;
	verts[1].st[1] = 1;
	verts[2].st[0] = 1;
	verts[2].st[1] = 1;
	verts[3].st[0] = 1;
	verts[3].st[1] = 0;

	// Set the bounding box color, according to the entity type
	for ( i = 0; i < 4; i++ ) {
		CG_ColorByEntityType( ent->eType, verts[i].modulate );
	}

	// top
	VectorCopy( box_vertex[0], verts[0].xyz );
	VectorCopy( box_vertex[1], verts[1].xyz );
	VectorCopy( box_vertex[2], verts[2].xyz );
	VectorCopy( box_vertex[3], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader, 4, verts );

	// bottom
	VectorCopy( box_vertex[7], verts[0].xyz );
	VectorCopy( box_vertex[6], verts[1].xyz );
	VectorCopy( box_vertex[5], verts[2].xyz );
	VectorCopy( box_vertex[4], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader, 4, verts );

	// top side
	VectorCopy( box_vertex[3], verts[0].xyz );
	VectorCopy( box_vertex[2], verts[1].xyz );
	VectorCopy( box_vertex[6], verts[2].xyz );
	VectorCopy( box_vertex[7], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );

	// left side
	VectorCopy( box_vertex[2], verts[0].xyz );
	VectorCopy( box_vertex[1], verts[1].xyz );
	VectorCopy( box_vertex[5], verts[2].xyz );
	VectorCopy( box_vertex[6], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );

	// right side
	VectorCopy( box_vertex[0], verts[0].xyz );
	VectorCopy( box_vertex[3], verts[1].xyz );
	VectorCopy( box_vertex[7], verts[2].xyz );
	VectorCopy( box_vertex[4], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );

	// bottom side
	VectorCopy( box_vertex[1], verts[0].xyz );
	VectorCopy( box_vertex[0], verts[1].xyz );
	VectorCopy( box_vertex[4], verts[2].xyz );
	VectorCopy( box_vertex[5], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );
}


/*
=================
CG_AddBoundingBox

Draws a bounding box around a player.  Called from CG_Player.
=================
*/
void CG_AddBoundingBox( const centity_t *cent ) {
	polyVert_t verts[4];
	clientInfo_t *ci;
	int i;
	vec3_t mins = { -15, -15, -24 };
	vec3_t maxs = { 15, 15, 32 };
	float extx, exty, extz;
	vec3_t corners[8];
	qhandle_t bboxShader, bboxShader_nocull;

	if ( !cg_drawBBox.integer ) {
		return;
	}

	// don't draw it if it's us in first-person
	if ( cent->currentState.number == cg.predictedPlayerState.clientNum &&
			!cg.renderingThirdPerson ) {
		return;
	}

	// don't draw it for dead players
	if ( cent->currentState.eFlags & EF_DEAD ) {
		return;
	}

	// get the shader handles
	bboxShader = trap_R_RegisterShader( "bbox" );
	bboxShader_nocull = trap_R_RegisterShader( "bbox_nocull" );

	// if they don't exist, forget it
	if ( !bboxShader || !bboxShader_nocull ) {
		return;
	}

	// get the player's client info
	ci = &cgs.clientinfo[cent->currentState.clientNum];

	// if it's us
	if ( cent->currentState.number == cg.predictedPlayerState.clientNum ) {
		// use the view height
		maxs[2] = cg.predictedPlayerState.viewheight + 6;
	}
	else {
		int x, zd, zu;

		// otherwise grab the encoded bounding box
		x = ( cent->currentState.solid & 255 );
		zd = ( ( cent->currentState.solid >> 8 ) & 255 );
		zu = ( ( ( cent->currentState.solid >> 16 ) & 255 ) - 32 );

		mins[0] = mins[1] = -x ;
		maxs[0] = maxs[1] = x;
		mins[2] = -zd;
		maxs[2] = zu;
	}

	// get the extents (size)
	extx = ( maxs[0] - mins[0] );
	exty = ( maxs[1] - mins[1] );
	extz = ( maxs[2] - mins[2] );

	
	// set the polygon's texture coordinates
	verts[0].st[0] = 0;
	verts[0].st[1] = 0;
	verts[1].st[0] = 0;
	verts[1].st[1] = 1;
	verts[2].st[0] = 1;
	verts[2].st[1] = 1;
	verts[3].st[0] = 1;
	verts[3].st[1] = 0;

	// set the polygon's vertex colors
	if ( ci->team == TEAM_RED ) {
		for ( i = 0; i < 4; i++ ) {
			verts[i].modulate[0] = 160;
			verts[i].modulate[1] = 0;
			verts[i].modulate[2] = 0;
			verts[i].modulate[3] = 255;
		}
	}
	else if ( ci->team == TEAM_BLUE ) {
		for ( i = 0; i < 4; i++ ) {
			verts[i].modulate[0] = 0;
			verts[i].modulate[1] = 0;
			verts[i].modulate[2] = 192;
			verts[i].modulate[3] = 255;
		}
	}
	else {
		for ( i = 0; i < 4; i++ ) {
			verts[i].modulate[0] = 0;
			verts[i].modulate[1] = 128;
			verts[i].modulate[2] = 0;
			verts[i].modulate[3] = 255;
		}
	}

	VectorAdd( cent->lerpOrigin, maxs, corners[3] );

	VectorCopy( corners[3], corners[2] );
	corners[2][0] -= extx;

	VectorCopy( corners[2], corners[1] );
	corners[1][1] -= exty;

	VectorCopy( corners[1], corners[0] );
	corners[0][0] += extx;

	for ( i = 0; i < 4; i++ ) {
		VectorCopy( corners[i], corners[i + 4] );
		corners[i + 4][2] -= extz;
	}

	// top
	VectorCopy( corners[0], verts[0].xyz );
	VectorCopy( corners[1], verts[1].xyz );
	VectorCopy( corners[2], verts[2].xyz );
	VectorCopy( corners[3], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader, 4, verts );

	// bottom
	VectorCopy( corners[7], verts[0].xyz );
	VectorCopy( corners[6], verts[1].xyz );
	VectorCopy( corners[5], verts[2].xyz );
	VectorCopy( corners[4], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader, 4, verts );

	// top side
	VectorCopy( corners[3], verts[0].xyz );
	VectorCopy( corners[2], verts[1].xyz );
	VectorCopy( corners[6], verts[2].xyz );
	VectorCopy( corners[7], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );

	// left side
	VectorCopy( corners[2], verts[0].xyz );
	VectorCopy( corners[1], verts[1].xyz );
	VectorCopy( corners[5], verts[2].xyz );
	VectorCopy( corners[6], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );

	// right side
	VectorCopy( corners[0], verts[0].xyz );
	VectorCopy( corners[3], verts[1].xyz );
	VectorCopy( corners[7], verts[2].xyz );
	VectorCopy( corners[4], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );

	// bottom side
	VectorCopy( corners[1], verts[0].xyz );
	VectorCopy( corners[0], verts[1].xyz );
	VectorCopy( corners[4], verts[2].xyz );
	VectorCopy( corners[5], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );
}

