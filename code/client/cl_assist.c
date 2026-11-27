#include "client.h"

#if defined(__WIIU__)
// turn180, ADS slowdown and aim assist, ported from the Xbox port.
extern int WiiU_Input_NunchukActive( void );

static cvar_t	*cl_turn180time;
static cvar_t	*cl_adsScale;
static cvar_t	*cl_aimAssist;
static cvar_t	*cl_aimAssistAngle;
static cvar_t	*cl_aimAssistStrength;
static float	turn180Remaining;

// The server opts in through serverinfo; stock servers without the key fall back to "not pure".
static qboolean CL_AssistAllowed( void ) {
	const char *info = cl.gameState.stringData + cl.gameState.stringOffsets[CS_SERVERINFO];
	const char *v = Info_ValueForKey( info, "sv_allowAssist" );

	return v[0] ? ( atoi( v ) != 0 ) : !cl_connectedToPureServer;
}

static void IN_Turn180_f( void ) {
	if ( !CL_AssistAllowed() ) {
		Com_Printf( "turn180 is disabled on this server.\n" );
		return;
	}
	if ( turn180Remaining > 0 ) {
		return;
	}
	if ( cl_turn180time->integer <= 0 ) {
		cl.viewangles[YAW] += 180;
		return;
	}
	turn180Remaining = 180;
}

static void CL_Turn180Step( void ) {
	float step;

	if ( turn180Remaining <= 0 ) {
		return;
	}
	if ( cl_turn180time->integer <= 0 ) {
		step = turn180Remaining;
	} else {
		step = 180.0f * cls.frametime / cl_turn180time->integer;
	}
	if ( step > turn180Remaining ) {
		step = turn180Remaining;
	}
	cl.viewangles[YAW] += step;
	turn180Remaining -= step;
}

#define ASSIST_RANGE		4096.0f
#define ASSIST_AIM_Z		8.0f		// aim a little above the origin (chest)
#define ASSIST_GAIN			8.0f		// strength 1.0 = 8 / second at cone centre
#define ASSIST_MAX_RATE		60.0f		// never turn faster than this (deg/sec)
#define ASSIST_STICK_CUT	0.7f		// full stick removes this much assist

static void CL_AimAssistOrigin( const entityState_t *es, vec3_t out ) {
	int dt;

	VectorCopy( es->pos.trBase, out );
	if ( es->pos.trType == TR_LINEAR || es->pos.trType == TR_LINEAR_STOP ) {
		dt = cl.serverTime - es->pos.trTime;
		if ( dt < 0 ) {
			dt = 0;
		}
		if ( dt > 250 ) {
			dt = 250;
		}
		if ( es->pos.trType == TR_LINEAR_STOP && dt > es->pos.trDuration ) {
			dt = es->pos.trDuration;
		}
		VectorMA( out, dt * 0.001f, es->pos.trDelta, out );
	}
}

static void CL_AimAssistUpdate( void ) {
	const playerState_t	*ps;
	const entityState_t	*es;
	trace_t		tr;
	vec3_t		eye, aim, dir, bestDir, fwd, viewAngles, tgtAngles;
	float		cone, minDot, bestDot, dot, dist, ang, weight, dt, t;
	float		stick, stickY, yawErr, pitchErr, stepYaw, stepPitch, total, maxStep, strength;
	int			i, mode;
	qboolean	teamplay, found;
	const char	*info;

	mode = cl_aimAssist->integer;
	if ( mode <= 0 ) {
		return;
	}
	// The Wii Remote IR pointer already aims precisely.
	if ( WiiU_Input_NunchukActive() ) {
		return;
	}
	if ( !CL_AssistAllowed() ) {
		return;
	}
	if ( cl.mouseDx[0] || cl.mouseDx[1] || cl.mouseDy[0] || cl.mouseDy[1] ) {
		return;
	}
	if ( clc.state != CA_ACTIVE || !cl.snap.valid || clc.demoplaying || cl_paused->integer ) {
		return;
	}
	ps = &cl.snap.ps;
	if ( ps->pm_type != PM_NORMAL || ps->stats[STAT_HEALTH] <= 0 || ( ps->pm_flags & PMF_FOLLOW ) ) {
		return;
	}
	if ( mode == 2 && cl.cgameSensitivity >= 0.999f ) {
		return;		// ADS-only mode and not zoomed
	}

	strength = cl_aimAssistStrength->value;
	cone = cl_aimAssistAngle->value;
	if ( strength <= 0.0f || cone <= 0.0f ) {
		return;
	}
	if ( strength > 1.0f ) {
		strength = 1.0f;
	}
	if ( cone > 45.0f ) {
		cone = 45.0f;
	}
	minDot = cos( DEG2RAD( cone ) );

	dt = cls.frametime * 0.001f;
	if ( dt <= 0.0f ) {
		return;
	}
	if ( dt > 0.05f ) {
		dt = 0.05f;
	}

	// eye position: snapshot origin, moved forward by the time since the snapshot
	t = ( cl.serverTime - cl.snap.serverTime ) * 0.001f;
	if ( t < 0.0f ) {
		t = 0.0f;
	}
	if ( t > 0.25f ) {
		t = 0.25f;
	}
	VectorMA( ps->origin, t, ps->velocity, eye );
	eye[2] += ps->viewheight;

	// true view = command angles + the server's delta angles
	for ( i = 0; i < 3; i++ ) {
		viewAngles[i] = cl.viewangles[i] + SHORT2ANGLE( ps->delta_angles[i] );
	}
	AngleVectors( viewAngles, fwd, NULL, NULL );

	info = cl.gameState.stringData + cl.gameState.stringOffsets[CS_SERVERINFO];
	teamplay = ( atoi( Info_ValueForKey( info, "g_gametype" ) ) >= GT_TEAM );

	found = qfalse;
	bestDot = minDot;
	for ( i = 0; i < cl.snap.numEntities; i++ ) {
		es = &cl.parseEntities[ ( cl.snap.parseEntitiesNum + i ) & ( MAX_PARSE_ENTITIES - 1 ) ];

		if ( es->eType != ET_PLAYER || es->number >= MAX_CLIENTS ||
			es->number == ps->clientNum || ( es->eFlags & EF_DEAD ) ) {
			continue;
		}
		if ( teamplay ) {
			info = cl.gameState.stringData + cl.gameState.stringOffsets[CS_PLAYERS + es->number];
			if ( atoi( Info_ValueForKey( info, "t" ) ) == ps->persistant[PERS_TEAM] ) {
				continue;	// teammate
			}
		}

		CL_AimAssistOrigin( es, aim );
		aim[2] += ASSIST_AIM_Z;

		VectorSubtract( aim, eye, dir );
		dist = VectorNormalize( dir );
		if ( dist < 32.0f || dist > ASSIST_RANGE ) {
			continue;
		}
		dot = DotProduct( dir, fwd );
		if ( dot < bestDot ) {
			continue;
		}

		// world line of sight (doors and other movers are ignored)
		CM_BoxTrace( &tr, eye, aim, vec3_origin, vec3_origin, 0, CONTENTS_SOLID, qfalse );
		if ( tr.fraction < 1.0f ) {
			continue;
		}

		bestDot = dot;
		VectorCopy( dir, bestDir );
		found = qtrue;
	}
	if ( !found ) {
		return;
	}

	// 1.0 at the centre of the cone, quadratic fade to 0 at its edge
	if ( bestDot > 1.0f ) {
		bestDot = 1.0f;
	}
	ang = acos( bestDot ) * ( 180.0f / M_PI ) / cone;
	weight = 1.0f - ang * ang;
	if ( weight <= 0.0f ) {
		return;
	}

	// back off while the player is pushing the right stick
	stick = fabs( (float)cl.joystickAxis[j_yaw_axis->integer] );
	stickY = fabs( (float)cl.joystickAxis[j_pitch_axis->integer] );
	if ( stickY > stick ) {
		stick = stickY;
	}
	stick /= 32767.0f;
	if ( stick > 1.0f ) {
		stick = 1.0f;
	}
	weight *= 1.0f - ASSIST_STICK_CUT * stick;

	t = strength * ASSIST_GAIN * weight * dt;
	if ( t > 1.0f ) {
		t = 1.0f;
	}

	vectoangles( bestDir, tgtAngles );
	yawErr = AngleSubtract( tgtAngles[YAW], viewAngles[YAW] );
	pitchErr = AngleSubtract( tgtAngles[PITCH], viewAngles[PITCH] );

	stepYaw = yawErr * t;
	stepPitch = pitchErr * t;

	maxStep = ASSIST_MAX_RATE * dt;
	total = sqrt( stepYaw * stepYaw + stepPitch * stepPitch );
	if ( total > maxStep ) {
		stepYaw *= maxStep / total;
		stepPitch *= maxStep / total;
	}

	cl.viewangles[YAW] += stepYaw;
	cl.viewangles[PITCH] += stepPitch;
}


void CL_AssistFrame(void) {
	CL_Turn180Step();
	CL_AimAssistUpdate();
}

void CL_AssistInit(void) {
	Cmd_AddCommand("turn180", IN_Turn180_f);
	cl_turn180time = Cvar_Get("cl_turn180time", "150", CVAR_ARCHIVE);
	cl_adsScale = Cvar_Get("cl_adsScale", "0.25", CVAR_ARCHIVE);
	cl_aimAssist = Cvar_Get("cl_aimAssist", "1", CVAR_ARCHIVE);
	cl_aimAssistAngle = Cvar_Get("cl_aimAssistAngle", "5", CVAR_ARCHIVE);
	cl_aimAssistStrength = Cvar_Get("cl_aimAssistStrength", "0.80", CVAR_ARCHIVE);
}

void CL_AssistShutdown(void) {
	Cmd_RemoveCommand("turn180");
}

float CL_AssistGetAdsScale(void) {
	return (cl_adsScale && cl_adsScale->value >= 0.0f) ? cl_adsScale->value : 1.0f;
}
#else
void CL_AssistFrame(void) {}
void CL_AssistInit(void) {}
void CL_AssistShutdown(void) {}
float CL_AssistGetAdsScale(void) { return 1.0f; }
#endif
