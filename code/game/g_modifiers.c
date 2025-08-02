/*
===========================================================================
Copyright (C) 2007 PaulR

This program is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

/*
	All(/most of) the Modifier specific stuff should go here
	So far it's all just instapad though, no idea if there will be more :X
*/

#include "g_local.h"

//#define dbg_print(x) trap_SendServerCommand( -1, va( "print \"" S_COLOR_YELLOW "%s\n\"", x ) )

/*
*****************************************

	InstaPad

*****************************************
*/

#define INSTAPAD_DAMAGE 5000
#define INSTAPAD_WJUMP_MAXRADIUS 50
#define INSTAPAD_WJUMP_KNOCKBACK 110

/**
Returns the damage value to be used in instapad play

target:		the target of the attack
inflictor:	the inflictor used for the attack (missile)
attacker:	the attacker
damage:		damage to be normally dealt by attack
dflags:		special DAMAGE_* flag
mod:		method of death used in the attack
*/
int InstaPad_calculateDamage(gentity_t *target, gentity_t *inflictor, gentity_t *attacker, int damage, int dflags,
							 int mod) {
	if (attacker == target)
		return damage;

	if (mod == MOD_TRIGGER_HURT || mod == MOD_WATER || mod == MOD_SLIME || mod == MOD_LAVA)
		return damage;

	if (dflags == DAMAGE_RADIUS && attacker->client)
		return 0; // no splash damage from players -- might change later ;)

	return INSTAPAD_DAMAGE;
}

/**
Returns qfalse if entity is not to be spawned for instapad play.

ent:	entity that wants to be spawned
*/
qboolean InstaPad_canSpawnEntity(const gentity_t *ent) {
	static const char *list[] = {// list of stuff we don't want
								 "holdable_", "weapon_", "ammo_", "station_health", NULL};
	const char *classname = ent->classname;
	// see if entity is in the list of stuff that we don't want
	int i = 0;

	while (NULL != list[i]) {
		const char *listEntry = list[i];
		if (NULL != strstr(classname, listEntry))
			return qfalse;

		i++;
	}

	// special case: we want SOME item_*s
	if (NULL != strstr(classname, "item_")) {
		if (NULL == strstr(classname, "botroam"))
			return qfalse;
	}

	return qtrue; // happy spawning
}

/**
Applys Weapon Jump Knockback to a player's velocity

origin:		point from where to calculate knockback direction (point of missile impact)
playerEnt:	entity to which to apply weapon jump knockback
mod:		method of death

(feels kinda weird, very un-rocket-jumpy.. >_<)
*/
void InstaPad_applyWeaponJumpKnockback(vec3_t origin, gentity_t *playerEnt, int mod) {
	int distance;
	vec3_t distanceVector;

	if (!playerEnt->client) // check for invalid client
		return;

	if (mod != MOD_INJECTOR)
		return; // for now, only KMA97

	VectorSubtract(origin, playerEnt->r.currentOrigin, distanceVector);
	distance = VectorLength(distanceVector);

	if (distance <= INSTAPAD_WJUMP_MAXRADIUS) {
		vec3_t direction, kvel;
		float mass, knockback;

		VectorSubtract(playerEnt->r.currentOrigin, origin, direction);
		direction[2] += 25;
		VectorNormalize(direction);

		mass = 200;
		knockback = INSTAPAD_WJUMP_KNOCKBACK;

		VectorScale(direction, g_knockback.value * knockback / mass, kvel);
		VectorAdd(playerEnt->client->ps.velocity, kvel, playerEnt->client->ps.velocity);
	}
}

/**
Returns WP_* index of the weapon that players are to be spawned
with during instapad play
*/
int InstaPad_getSpawnWeapon(void) {
	return WP_KMA97; // for now, it's always KMA :)
}
