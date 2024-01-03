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
// g_weapon.c
// perform the server side effects of a weapon firing

#include "g_local.h"

static float s_quadFactor;
static vec3_t forward, right, up;
static vec3_t muzzle;

/*
======================================================================

GAUNTLET

======================================================================
*/

static void Weapon_PunchyFire(gentity_t *ent) {
}

/*
===============
CheckGauntletAttack
===============
*/
qboolean CheckGauntletAttack(gentity_t *ent) {
	trace_t tr;
	vec3_t end;
	gentity_t *tent;
	gentity_t *traceEnt;
	int damage;

	// set aiming directions
	AngleVectors(ent->client->ps.viewangles, forward, right, up);

	CalcMuzzlePoint(ent, forward, right, up, muzzle);

	VectorMA(muzzle, 32, forward, end);

	trap_Trace(&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT);
	if (tr.surfaceFlags & SURF_NOIMPACT) {
		return qfalse;
	}

	if (ent->client->noclip) {
		return qfalse;
	}

	traceEnt = &g_entities[tr.entityNum];
	if (traceEnt->client && traceEnt->client->noclip) {
		return qfalse;
	}

	// send blood impact
	if (traceEnt->takedamage && traceEnt->client) {
		tent = G_TempEntity(tr.endpos, EV_MISSILE_HIT);
		tent->s.otherEntityNum = traceEnt->s.number;
		tent->s.eventParm = DirToByte(tr.plane.normal);
		tent->s.weapon = ent->s.weapon;
	}

	if (!traceEnt->takedamage) {
		return qfalse;
	}

	if (ent->client->ps.powerups[PW_PADPOWER]) {
		G_AddEvent(ent, EV_POWERUP_PADPOWER, 0);
		s_quadFactor = g_quadfactor.value;
	} else {
		s_quadFactor = 1;
	}
	if (ent->client->ps.powerups[PW_BERSERKER])
		s_quadFactor *= 10.0f; // one "berserker punchy" hit should kill

	damage = (DAMAGE_PUNCHY * s_quadFactor);
	G_Damage(traceEnt, ent, ent, forward, tr.endpos, damage, 0, MOD_PUNCHY);

	return qtrue;
}

static void Weapon_NipperFire(gentity_t *ent) {
	gentity_t *m;

	m = fire_nipper(ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;
}

static void Weapon_ImperiusFire(gentity_t *ent) {
	gentity_t *m;

	m = fire_bfg(ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

	//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

// new: g_combat.c(line ~830) 2x knockback with pumper
static void Weapon_PumperFire(gentity_t *ent) {
	vec3_t end;
	trace_t trace;
	gentity_t *tent;
	gentity_t *traceEnt;
	int damage;
	int passent;
	vec3_t minPumper = {-4.0f, -4.0f, -4.0f};
	vec3_t maxPumper = {4.0f, 4.0f, 4.0f};

	damage = (DAMAGE_PUMPER * s_quadFactor);

	VectorMA(muzzle, RANGE_PUMPER, forward, end);

	// trace only against the solids, so the railgun will go through people
	traceEnt = NULL;
	passent = ent->s.number;
	trap_Trace(&trace, muzzle, minPumper, maxPumper, end, passent, MASK_SHOT);
	if (trace.startsolid) {
		VectorCopy(muzzle, trace.endpos);
	} else if (trace.entityNum < ENTITYNUM_MAX_NORMAL) {
		traceEnt = &g_entities[trace.entityNum];
		if (traceEnt->takedamage) {
			if (LogAccuracyHit(traceEnt, ent)) {
				G_LogHit(ent);
			}
			damage *= 1.0f - (trace.fraction * 0.5f);
			G_Damage(traceEnt, ent, ent, forward, trace.endpos, damage, 0, MOD_PUMPER);
		}
	}
	// the final trace endpos will be the terminal point of the rail trail

	// snap the endpos to integers to save net bandwidth, but nudged towards the line
	SnapVectorTowards(trace.endpos, muzzle);

	// send railgun beam effect
	tent = G_TempEntity(trace.endpos, EV_RAILTRAIL);

	VectorCopy(muzzle, tent->s.origin2);
	// move origin a bit to come closer to the drawn gun muzzle
	VectorMA(tent->s.origin2, 4, right, tent->s.origin2);
	VectorMA(tent->s.origin2, -1, up, tent->s.origin2);

	// no explosion at end if SURF_NOIMPACT, but still make the trail
	if (trace.surfaceFlags & SURF_NOIMPACT) {
		// don't make the explosion at the end
		tent->s.eventParm = 255;
	} else {
		// make an explosion with radius damage
		tent->s.eventParm = DirToByte(trace.plane.normal);
		G_RadiusDamage(trace.endpos, ent, SPLASHDMG_PUMPER, SPLASHRAD_PUMPER, traceEnt, MOD_PUMPER);
	}
	tent->s.clientNum = ent->s.clientNum;
}

static void Weapon_BalloonyFire(gentity_t *ent) {
	gentity_t *m;

	// extra vertical velocity
	forward[2] += 0.2f;
	VectorNormalize(forward);

	m = fire_balloony(ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;
}

static void Weapon_BettyFire(gentity_t *ent) {
	gentity_t *m;

	vec3_t start;

	VectorMA(muzzle, 5, right, start);
	VectorMA(start, -5, up, start);
	m = fire_betty(ent, start, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;
}

static void Weapon_BuggleGFire(gentity_t *ent) {
	gentity_t *m;

	m = fire_bubbleg(ent, muzzle, forward); // HERBY: fire bubble gum
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;
}

/*
======================================================================

RAILGUN

======================================================================
*/

/*
=================
weapon_railgun_fire
=================
*/
static void Weapon_SplasherFire(gentity_t *ent) {
	gentity_t *m;

	m = fire_splasher(ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;
}

/*
======================================================================

GRAPPLING HOOK

======================================================================
*/

static void Weapon_GrapplingHookFire(gentity_t *ent) {
	if (!ent->client->fireHeld && !ent->client->hook)
		fire_grapple(ent, muzzle, forward);

	ent->client->fireHeld = qtrue;
}

void Weapon_HookFree(gentity_t *ent) {
	ent->parent->client->hook = NULL;
	ent->parent->client->ps.pm_flags &= ~PMF_GRAPPLE_PULL;
	G_FreeEntity(ent);
}

void Weapon_HookThink(gentity_t *ent) {
	if (ent->enemy) {
		vec3_t v, oldorigin;

		VectorCopy(ent->r.currentOrigin, oldorigin);
		v[0] = ent->enemy->r.currentOrigin[0] + (ent->enemy->r.mins[0] + ent->enemy->r.maxs[0]) * 0.5;
		v[1] = ent->enemy->r.currentOrigin[1] + (ent->enemy->r.mins[1] + ent->enemy->r.maxs[1]) * 0.5;
		v[2] = ent->enemy->r.currentOrigin[2] + (ent->enemy->r.mins[2] + ent->enemy->r.maxs[2]) * 0.5;
		SnapVectorTowards(v, oldorigin); // save net bandwidth

		G_SetOrigin(ent, v);
	}

	VectorCopy(ent->r.currentOrigin, ent->parent->client->ps.grapplePoint);
}

/*
======================================================================

BOASTER

======================================================================
*/

static void Weapon_BoasterFire(gentity_t *ent) {
	gentity_t *m;

	m = fire_boaster(ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;
}

/*
======================================================================

KMA / Kiss My Ass 97

======================================================================
*/
static void Weapon_KMAFire(gentity_t *ent) {
	gentity_t *m;

	m = fire_kma(ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;
	m->s.clientNum = ent->s.clientNum;
}

/*
======================================================================

SPRAY PISTOL

======================================================================
*/

static void check_sprayawards(gentity_t *ent) {
	ent->client->logocounter++;

	// FIXME: Less magical constants
	if (ent->client->logocounter == 5) {
		ent->client->ps.persistant[PERS_SPRAYAWARDS_COUNT] =
			((ent->client->ps.persistant[PERS_SPRAYAWARDS_COUNT] & 0xFF00) |
			 ((ent->client->ps.persistant[PERS_SPRAYAWARDS_COUNT] + 1) & 0xFF));
		// add the sprite over the player's head
		SetAward(ent->client, AWARD_SPRAYKILLER);

		PrintMsg(NULL, "%s" S_COLOR_MAGENTA " is a SprayKiller!\n", ent->client->pers.netname);

		AddScore(ent, ent->client->ps.origin, SCORE_BONUS_SPRAYKILLER, SCORE_BONUS_SPRAYKILLER_S);
		if (g_gametype.integer == GT_SPRAY) {
			AddTeamScore(ent->client->ps.origin, ent->client->sess.sessionTeam, SCORE_BONUS_SPRAYKILLER,
						 SCORE_BONUS_SPRAYKILLER_S);
		}
	} else if (ent->client->logocounter == 8) {
		ent->client->ps.persistant[PERS_SPRAYAWARDS_COUNT] += 0x100;

		// add the sprite over the player's head
		SetAward(ent->client, AWARD_SPRAYGOD);

		PrintMsg(NULL, "%s" S_COLOR_MAGENTA " is a SprayGod!\n", ent->client->pers.netname);

		AddScore(ent, ent->client->ps.origin, SCORE_BONUS_SPRAYGOD, SCORE_BONUS_SPRAYGOD_S);
		if (g_gametype.integer == GT_SPRAY) {
			AddTeamScore(ent->client->ps.origin, ent->client->sess.sessionTeam, SCORE_BONUS_SPRAYGOD,
						 SCORE_BONUS_SPRAYGOD_S);
		}

		ent->client->logocounter = 0;
	}
}

static void Weapon_SpraypistolFire(gentity_t *ent) {
	vec3_t end, tmpv3;
	trace_t tr;
	gentity_t *tent;

	// FIXME: Remove that double storing. Less magical constants, more documentation.
	ent->client->ps.generic1 = ent->client->ps.ammo[WP_SPRAYPISTOL];

	VectorMA(muzzle, 256, forward, end);
	trap_Trace(&tr, muzzle, NULL, NULL, end, ENTITYNUM_NONE, MASK_SHOT);

	if (g_gametype.integer == GT_SPRAY) {
		if (&g_entities[tr.entityNum] == level.rspraywall) {
			if (ent->client->sess.sessionTeam == TEAM_RED) {
				AddTeamScore(tr.endpos, TEAM_RED, SCORE_SPRAY, SCORE_SPRAY_S);
				AddScore(ent, tr.endpos, SCORE_SPRAY, SCORE_SPRAY_S);
				check_sprayawards(ent);
			} else {
				AddScore(ent, tr.endpos, SCORE_SPRAY_WRONGWALL, SCORE_SPRAY_WRONGWALL_S);
				// FIXME: Use an event rather than that crap
				trap_SendServerCommand(-1, va("cdi " XSTRING(CLIENT_DO_IT_SPRAYED_ON_WRONG_WALL) " %i", (int)(random() * 3.9999)));

				PrintMsg(NULL, "%s" S_COLOR_MAGENTA " (%s Team) sprayed on the WRONG WALL!!!\n",
						 ent->client->pers.netname, TeamName(ent->client->sess.sessionTeam));
			}
		} else if (&g_entities[tr.entityNum] == level.bspraywall) {
			if (ent->client->sess.sessionTeam == TEAM_BLUE) {
				AddTeamScore(tr.endpos, TEAM_BLUE, SCORE_SPRAY, SCORE_SPRAY_S);
				AddScore(ent, tr.endpos, SCORE_SPRAY, SCORE_SPRAY_S);
				check_sprayawards(ent);
			} else {
				AddScore(ent, tr.endpos, SCORE_SPRAY_WRONGWALL, SCORE_SPRAY_WRONGWALL_S);
				trap_SendServerCommand(-1, va("cdi " XSTRING(CLIENT_DO_IT_SPRAYED_ON_WRONG_WALL) " %i", (int)(random() * 3.9999)));

				PrintMsg(NULL, "%s" S_COLOR_MAGENTA " (%s Team) sprayed on the WRONG WALL!!!\n",
						 ent->client->pers.netname, TeamName(ent->client->sess.sessionTeam));
			}
		}
	}
	// Syc "ffa"
	else {
		// all spraywalls give the same points
		if ((&g_entities[tr.entityNum] == level.rspraywall) || (&g_entities[tr.entityNum] == level.bspraywall) ||
			(&g_entities[tr.entityNum] == level.nspraywall)) {
			AddScore(ent, tr.endpos, SCORE_SPRAY, SCORE_SPRAY_S);
			check_sprayawards(ent);
		}
	}

	// snap the endpos to integers to save net bandwidth, but nudged towards the line
	SnapVectorTowards(tr.endpos, muzzle);

	// send beam effect
	tent = G_TempEntity(tr.endpos, EV_SPRAYLOGO);

	tent->r.svFlags |= SVF_BROADCAST;

	VectorSubtract(tr.endpos, ent->s.pos.trBase, tmpv3);

	// angles[0] is the scaled distance from spraywall
	// and used to scale radius for the spraylogo clientside
	tent->s.angles[0] = (sqrt(VectorLengthSquared(tmpv3)) / 200);

	tent->s.clientNum = ent->s.clientNum;

	VectorCopy(muzzle, tent->s.origin2);
	// move origin a bit to come closer to the drawn gun muzzle
	VectorMA(tent->s.origin2, 4, right, tent->s.origin2);
	VectorMA(tent->s.origin2, -1, up, tent->s.origin2);

	// no explosion at end if SURF_NOIMPACT, but still make the trail
	if ((tr.surfaceFlags & SURF_NOIMPACT) || (tr.fraction == 1.0)) {
		tent->s.eventParm = 255; // don't make the explosion at the end
	} else {
		tent->s.eventParm = DirToByte(tr.plane.normal);
	}

	// Hit a spraywall
	if (tr.entityNum != ENTITYNUM_WORLD) {
		tent->s.generic1 = SPRAYROOM_CONSTANT;
	}
}

/*
===============
LogAccuracyHit
===============
*/
qboolean LogAccuracyHit(gentity_t *target, gentity_t *attacker) {
	if (!target->takedamage) {
		return qfalse;
	}

	if (target == attacker) {
		return qfalse;
	}

	if (!target->client) {
		return qfalse;
	}

	if (!attacker->client) {
		return qfalse;
	}

	if (target->client->ps.stats[STAT_HEALTH] <= 0) {
		return qfalse;
	}

	if (OnSameTeam(target, attacker)) {
		return qfalse;
	}

	return qtrue;
}

/*
===============
CalcMuzzlePoint

set muzzle location relative to pivoting eye
===============
*/
void CalcMuzzlePoint(gentity_t *ent, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint) {
	VectorCopy(ent->s.pos.trBase, muzzlePoint);
	muzzlePoint[2] += ent->client->ps.viewheight;
	VectorMA(muzzlePoint, 14, forward, muzzlePoint);
	// snap to integer coordinates for more efficient network bandwidth usage
	SnapVector(muzzlePoint);
}

/*
===============
CalcMuzzlePointOrigin

set muzzle location relative to pivoting eye
===============
*/
static void CalcMuzzlePointOrigin(gentity_t *ent, vec3_t origin, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint) {
	VectorCopy(ent->s.pos.trBase, muzzlePoint);
	muzzlePoint[2] += ent->client->ps.viewheight;
	VectorMA(muzzlePoint, 14, forward, muzzlePoint);
	// snap to integer coordinates for more efficient network bandwidth usage
	SnapVector(muzzlePoint);
}

/*
===============
FireWeapon
===============
*/
void G_FireWeapon(gentity_t *ent) {
	if (ent->client->ps.powerups[PW_PADPOWER]) {
		s_quadFactor = g_quadfactor.value;
	} else {
		s_quadFactor = 1;
	}

	// track shots taken for accuracy tracking.
	// Grapple is not a weapon. Punchy and Spraypistol is just not tracked
	if (ent->s.weapon != WP_GRAPPLING_HOOK && ent->s.weapon != WP_PUNCHY
		&& ent->s.weapon != WP_SPRAYPISTOL) {
		ent->client->accuracy_shots++;
	}

	// set aiming directions
	AngleVectors(ent->client->ps.viewangles, forward, right, up);

	CalcMuzzlePointOrigin(ent, ent->client->oldOrigin, forward, right, up, muzzle);

	// fire the specific weapon
	switch (ent->s.weapon) {
	case WP_PUNCHY:
		Weapon_PunchyFire(ent);
		break;
	case WP_BOASTER:
		Weapon_BoasterFire(ent);
		break;
	case WP_PUMPER:
		Weapon_PumperFire(ent);
		break;
	case WP_NIPPER:
		Weapon_NipperFire(ent);
		break;
	case WP_BALLOONY:
		Weapon_BalloonyFire(ent);
		break;
	case WP_BETTY:
		Weapon_BettyFire(ent);
		break;
	case WP_BUBBLEG:
		Weapon_BuggleGFire(ent);
		break;
	case WP_SPLASHER:
		Weapon_SplasherFire(ent);
		break;
	case WP_IMPERIUS:
		Weapon_ImperiusFire(ent);
		break;
	case WP_KMA97: // "Kiss My Ass 97"
		Weapon_KMAFire(ent);
		break;
	case WP_GRAPPLING_HOOK:
		Weapon_GrapplingHookFire(ent);
		break;
	case WP_SPRAYPISTOL:
		Weapon_SpraypistolFire(ent);
		break;
	default:
		// FIXME G_Error("Bad ent->s.weapon");
		break;
	}
}
