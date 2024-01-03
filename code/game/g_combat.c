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
// g_combat.c

#include "g_local.h"

/*
============
BerserkerCheck
============
*/
void BerserkerCheck(gentity_t *ent) {
	if (!ent->client)
		return;

	if (!(ent->client->ps.persistant[PERS_SNACKATTACK_COUNT] % 5)) {
		const gitem_t *it;
		gentity_t *it_ent;
		trace_t trace;

		ent->client->ps.stats[STAT_HEALTH] = ent->health = 200;
		ent->client->ps.stats[STAT_ARMOR] = 200;
		it = BG_FindItem("PUPPET MASTER");
		it_ent = G_Spawn();
		VectorCopy(ent->r.currentOrigin, it_ent->s.origin);
		it_ent->classname = it->classname;
		G_SpawnItem(it_ent, it);
		FinishSpawningItem(it_ent);
		memset(&trace, 0, sizeof(trace));
		Touch_Item(it_ent, ent, &trace);
		G_FreeEntity(it_ent);
	}
}

/*
============
ScorePlum
============
*/
static void ScorePlum(gentity_t *ent, const vec3_t origin, int score) {
	gentity_t *plum;

	plum = G_TempEntity(origin, EV_SCOREPLUM);
	if (G_IsKillerDuck(ent)) {
		// send scoring point event to every player - the killerduck
		// should not be able to hide anywhere...
		plum->r.svFlags |= SVF_BROADCAST;
	} else {
		// only send this temp entity to a single client
		plum->r.svFlags |= SVF_SINGLECLIENT;
		plum->r.singleClient = ent->s.number;
	}

	plum->s.otherEntityNum = ent->s.number;
	plum->s.time = score;
}

/*
============
AddScore

Adds score to both the client and his team
============
*/
void AddScore(gentity_t *ent, const vec3_t origin, int score, const char *reason) {
	if (!ent->client) {
		return;
	}
	// no scoring during pre-match warmup
	if (level.warmupTime) {
		return;
	}
	// show score plum
	if (score > 0) // Ente doesn't like the -1(-5) with his new pics
		ScorePlum(ent, origin, score);

	ent->client->ps.persistant[PERS_SCORE] += score;
	if (g_gametype.integer == GT_TEAM || g_gametype.integer == GT_FREEZETAG)
		level.teamScores[ent->client->ps.persistant[PERS_TEAM]] += score;

	CalculateRanks();

	G_LogPrintf("AddScore: %ld %d %s\n", (ent - g_entities), score, reason);
}

/*
=================
TossClientItems

Toss the weapon and powerups for the killed player
=================
*/
void TossClientItems(gentity_t *self) {
	const gitem_t *item;
	int weapon;
	float angle;
	int i;
	gentity_t *drop;

	weapon = self->s.weapon;

	// make a special check to see if they are changing to a new
	// weapon that isn't the mg or gauntlet.  Without this, a client
	// can pick up a weapon, be killed, and not drop the weapon because
	// their weapon change hasn't completed yet and they are still holding the MG.
	if (weapon == WP_NIPPER || weapon == WP_GRAPPLING_HOOK) {
		if (self->client->ps.weaponstate == WEAPON_DROPPING) {
			weapon = self->client->pers.cmd.weapon;
		}
		if (!(self->client->ps.stats[STAT_WEAPONS] & (1 << weapon))) {
			weapon = WP_NONE;
		}
	}

	// Modifiers
	// prevent weapon drop in instagib
	if (g_modInstagib.integer) {
		weapon = WP_NONE;
	}

	// drop the weapon if not punchy, nipper of spraypistol
	if (weapon > WP_NIPPER && weapon != WP_GRAPPLING_HOOK && weapon != WP_SPRAYPISTOL &&
		self->client->ps.ammo[weapon]) {
		// find the item type for this weapon
		item = BG_FindItemForWeapon(weapon);

		// spawn the item
		Drop_Item(self, item, 0);
	}

	if (g_gametype.integer == GT_SPRAY) {
		// droping too many lead to a server crash ... only with using cheats ;)
		// max cartridges 8 ... so don't drop more than 8
		if (self->client->ps.ammo[WP_SPRAYPISTOL] > 8)
			self->client->ps.ammo[WP_SPRAYPISTOL] = 8;

		if (self->client->sess.sessionTeam == TEAM_RED) {
			for (; self->client->ps.ammo[WP_SPRAYPISTOL] > 0; self->client->ps.ammo[WP_SPRAYPISTOL]--)
				Drop_Item(self, BG_FindItem("red Cartridge"), random() * 360); // pickup name ändern !!!

			Drop_Item(self, BG_FindItem("blue Cartridge"), random() * 360)->nextthink =
				level.time + 120000; // change pickup name!!!
		} else if (self->client->sess.sessionTeam == TEAM_BLUE) {
			for (; self->client->ps.ammo[WP_SPRAYPISTOL] > 0; self->client->ps.ammo[WP_SPRAYPISTOL]--)
				Drop_Item(self, BG_FindItem("blue Cartridge"), random() * 360); // pickup name ändern !!!

			Drop_Item(self, BG_FindItem("red Cartridge"), random() * 360)->nextthink =
				level.time + 120000; // change pickup name!!!
		}
		// other->client->ps.generic1=other->client->ps.ammo[WP_SPRAYPISTOL];
		self->client->ps.generic1 = 0;
	} else if (g_gametype.integer == GT_SPRAYFFA) {
		qboolean first = qtrue;

		// dropping too many lead to a server crash ... only with using cheats ;)
		// max cartridges 8 ... so don't drop more than 8
		if (self->client->ps.ammo[WP_SPRAYPISTOL] > 8)
			self->client->ps.ammo[WP_SPRAYPISTOL] = 8;

		self->client->ps.ammo[WP_SPRAYPISTOL]++; // add the own cartridge ...

		for (; self->client->ps.ammo[WP_SPRAYPISTOL] > 0; self->client->ps.ammo[WP_SPRAYPISTOL]--) {
			gentity_t *tmpGE = Drop_Item(self, BG_FindItem("neutral Cartridge"), random() * 360); // change pickup name !!!

			tmpGE->nextthink = level.time + 120000;
			if (first) {
				tmpGE->s.otherEntityNum = self->s.number;
				first = qfalse;
			}
		}
		self->client->ps.generic1 = 0;
	}

	angle = 45;
	for (i = 1; i < PW_NUM_POWERUPS; i++) {
		if (i == PW_BERSERKER)
			continue; // berserker doesn't get dropped

		if (self->client->ps.powerups[i] > level.time) {
			item = BG_FindItemForPowerup(i);
			if (!item) {
				continue;
			}
			drop = Drop_Item(self, item, angle);
			// decide how many seconds it has left
			drop->count = (self->client->ps.powerups[i] - level.time) / 1000;
			if (drop->count < 1) {
				drop->count = 1;
			}
			angle += 45;
		}
	}

	G_DropHoldable(self, angle);
}

void G_DropHoldable(gentity_t *self, float angle) {
	if (bg_itemlist[self->client->ps.stats[STAT_HOLDABLE_ITEM]].giTag != HI_NONE) {
		const gitem_t *item = BG_FindItemForHoldable(bg_itemlist[self->client->ps.stats[STAT_HOLDABLE_ITEM]].giTag);
		gentity_t *drop = Drop_Item(self, item, angle);
		drop->count = self->client->ps.stats[STAT_HOLDABLEVAR];
	}
}

/*
==================
LookAtKiller
==================
*/
static void LookAtKiller(gentity_t *self, gentity_t *inflictor, gentity_t *attacker) {
	vec3_t dir;

	if (attacker && attacker != self) {
		VectorSubtract(attacker->s.pos.trBase, self->s.pos.trBase, dir);
	} else if (inflictor && inflictor != self) {
		VectorSubtract(inflictor->s.pos.trBase, self->s.pos.trBase, dir);
	} else {
		self->client->ps.stats[STAT_DEAD_YAW] = self->s.angles[YAW];
		return;
	}

	self->client->ps.stats[STAT_DEAD_YAW] = vectoyaw(dir);
}

/*
==================
body_die
==================
*/
void body_die(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath) {
	if (self->health > GIB_HEALTH) {
		return;
	}
	self->health = GIB_HEALTH + 1;
}

// these are just for logging, the client prints its own messages
// keep in sync with meansOfDeath_t !!!
static const char *modNames[] = {
	"MOD_UNKNOWN",		   "MOD_PUMPER",  "MOD_PUNCHY",		  "MOD_NIPPER",			 "MOD_BALLOONY",
	"MOD_BALLOONY_SPLASH", "MOD_BETTY",	  "MOD_BETTY_SPLASH", "MOD_BUBBLEG",		 "MOD_BUBBLEG_SPLASH",
	"MOD_SPLASHER",		   "MOD_BOASTER", "MOD_IMPERIUS",	  "MOD_IMPERIUS_SPLASH", "MOD_INJECTOR",

	"MOD_KILLERDUCKS",

	"MOD_WATER",		   "MOD_SLIME",	  "MOD_LAVA",		  "MOD_CRUSH",			 "MOD_TELEFRAG",
	"MOD_FALLING",		   "MOD_SUICIDE", "MOD_TARGET_LASER", "MOD_TRIGGER_HURT",	 "MOD_GRAPPLE",

	"MOD_BAMBAM",		   "MOD_BOOMIES"};

CASSERT(ARRAY_LEN(modNames) == MOD_MEANSOFDEATH_MAX);

/*
==================
CheckAlmostLollyCapture
==================
*/
static void CheckAlmostLollyCapture(gentity_t *self, gentity_t *attacker) {
	gentity_t *ent;
	char *classname;

	// if this player was carrying a lolly (flag)
	if (self->client->ps.powerups[PW_REDFLAG] || self->client->ps.powerups[PW_BLUEFLAG] ||
		self->client->ps.powerups[PW_NEUTRALFLAG]) {
		// get the goal flag this player should have been going for
		if (self->client->sess.sessionTeam == TEAM_BLUE) {
			classname = "team_CTL_bluelolly";
		} else {
			classname = "team_CTL_redlolly";
		}
		ent = NULL;
		do {
			ent = G_Find(ent, FOFS(classname), classname);
		} while (ent && (ent->flags & FL_DROPPED_ITEM));
		// if we found the destination lolly and it's not picked up
		if (ent && !(ent->r.svFlags & SVF_NOCLIENT)) {
			float distSqr;
			// if the player was *very* close
			distSqr = DistanceSquared(self->client->ps.origin, ent->s.origin);
			if (distSqr < Square(200)) {
				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_ALMOSTCAPTURE;
				if (attacker->client) {
					attacker->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_ALMOSTCAPTURE;
				}
			}
		}
	}
}

/*
==================
CheckAlmostSprayAward
==================
*/
static void CheckAlmostSprayAward(gentity_t *self, gentity_t *attacker) {
	// If the player had 5 (CNT_CARTRIDGES) or more cartridges on him and could have received a spray award
	// (SprayKiller/SprayGod)
	if (IsSyc() && self->client->ps.generic1 >= CNT_CARTRIDGES) {
		gentity_t *ent = NULL;
		do {
			ent = G_Find(ent, FOFS(classname), "trigger_teleport");
		} while (ent && (ent->spawnflags & TELEPORT_ENTER_SPRAYROOM) == 0);
		// if we found the teleporter for the spray room
		if (ent) {
			// triggers have no origin \o/
			vec3_t origin;
			float distSqr;

			VectorAdd(ent->r.mins, ent->r.maxs, origin);
			VectorScale(origin, 0.5f, origin);
			// if the player was *very* close
			distSqr = DistanceSquared(self->client->ps.origin, origin);
			if (distSqr < Square(200)) {
				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_ALMOSTCAPTURE;
				if (attacker->client) {
					attacker->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_ALMOSTCAPTURE;
				}
			}
		}
	}
}

/*
===================
CheckAlmostBigBalloon

If self is dying we are checking whether they were close to the ballon trigger zone
and if they almost won the match because they were about to capture the last ballon
===================
*/
static void CheckAlmostBigBalloon(gentity_t *self, gentity_t *attacker) {
	if (g_gametype.integer == GT_BALLOON) {
		int notCapturedBalloon = -1;
		int captured = 0;
		int i;

		for (i = 0; i < level.numBalloons; ++i) {
			if (G_BalloonIsCaptured(i, self->client->sess.sessionTeam, qtrue)) {
				++captured;
			} else {
				notCapturedBalloon = i;
			}
		}

		// if only the last balloon is not captured yet
		if (captured >= level.numBalloons - 1) {
			gentity_t *balloonZone = NULL;
			for (;;) {
				balloonZone = G_Find(balloonZone, FOFS(classname), "trigger_balloonzone");
				if (balloonZone == NULL) {
					break;
				}
				if (balloonZone->count != notCapturedBalloon) {
					continue;
				}
				// if the player is at the balloon
				if (IsPlayerAtBalloon(self->client->ps.clientNum, balloonZone)) {
					self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_ALMOSTCAPTURE;
					if (attacker->client) {
						attacker->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_ALMOSTCAPTURE;
					}
					break;
				}
			};
		}
	}
}

/*
==================
player_die
==================
*/
void player_die(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath) {
	gentity_t *ent;
	int anim;
	int contents;
	int killer;
	int i;
	const char *killerName, *obit;

	if (self->client->ps.pm_type == PM_DEAD) {
		return;
	}

	if (level.intermissiontime) {
		return;
	}

	// check for an almost flag/lolly capture
	CheckAlmostLollyCapture(self, attacker);
	// check for almost received a spray award (SprayKiller/SprayGod)
	CheckAlmostSprayAward(self, attacker);
	// check for an almost Big Balloon (capturing the last balloon)
	CheckAlmostBigBalloon(self, attacker);

	if (self->client && self->client->hook) {
		Weapon_HookFree(self->client->hook);
	}
	self->client->ps.pm_type = PM_DEAD;

	if (attacker) {
		killer = attacker->s.number;
		if (attacker->client) {
			killerName = attacker->client->pers.netname;
		} else {
			killerName = "<non-client>";
		}
	} else {
		killer = ENTITYNUM_WORLD;
		killerName = "<world>";
	}

	if (killer < 0 || killer >= MAX_CLIENTS) {
		killer = ENTITYNUM_WORLD;
		killerName = "<world>";
	}

	if (meansOfDeath < 0 || meansOfDeath >= ARRAY_LEN(modNames)) {
		obit = "<bad obituary>";
	} else {
		obit = modNames[meansOfDeath];
	}

	G_LogPrintf("Kill: %i %i %i: %s killed %s by %s\n", killer, self->s.number, meansOfDeath, killerName,
				self->client->pers.netname, obit);

	// broadcast the death event to everyone
	ent = G_TempEntity(self->r.currentOrigin, EV_OBITUARY);
	ent->s.eventParm = meansOfDeath;
	ent->s.otherEntityNum = self->s.number;
	ent->s.otherEntityNum2 = killer;
	ent->r.svFlags = SVF_BROADCAST; // send to everyone

	self->enemy = attacker;

	self->client->ps.persistant[PERS_KILLED]++;

	if (g_gametype.integer == GT_LPS) {
		if (!level.warmupTime) {
			self->client->sess.livesleft--;
			if (self->client->sess.livesleft < 0) {
				// FIXME: Properly limit lives to 0
				//        and don't check for "<=" everywhere
				self->client->sess.livesleft = 0;
			}
			// AddScore() would call this, but since LPS does not use it midway in the game, we need to do it on our own
			CalculateRanks();
			// Update scoreboard clientside. This is also used for the different lps icons clientside!
			SendScoreboardMessageToAllClients();
		}

		ent->s.generic1 = self->client->sess.livesleft; // add lives left info to tmpEntity (for attacker midscreen-msg)
		self->client->lastDeathTime = level.time;

		if (self->client->sess.livesleft <= 0) {
			trap_SendServerCommand(self->s.number, "cdi " XSTRING(CLIENT_DO_IT_LPS_LOST));
		}
	}

	if (attacker && attacker->client) {
		attacker->client->lastkilled_client = self->s.number;

		if (attacker == self || (OnSameTeam(self, attacker) || IsItemSameTeam(attacker, self))) {
			if (g_gametype.integer != GT_LPS)
				AddScore(attacker, self->r.currentOrigin, SCORE_TEAMKILL, SCORE_TEAMKILL_S);
		} else {
#if 0
			if (g_gametype.integer != GT_SPRAY && g_gametype.integer != GT_SPRAYFFA &&
				g_gametype.integer != GT_BALLOON && g_gametype.integer != GT_LPS)
				AddScore(attacker, self->r.currentOrigin, 1, "");
#endif

			// Scores for killing only in some non-teamplay gametypes
			if ((g_gametype.integer <= GT_TEAM) && (g_gametype.integer != GT_LPS) && (!IsSyc())) {
				AddScore(attacker, self->r.currentOrigin, SCORE_KILL, SCORE_KILL_S);
			}

			if (meansOfDeath == MOD_PUNCHY) {
				// play humiliation on player
				if (!attacker->client->ps.powerups[PW_BERSERKER]) {
					attacker->client->ps.persistant[PERS_SNACKATTACK_COUNT]++;
					BerserkerCheck(attacker);
				}

				// add the sprite over the player's head
				SetAward(attacker->client, AWARD_SNACKATTACK);

				// also play humiliation on target
				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_GAUNTLETREWARD;
			}

			// check for two kills in a short amount of time
			// if this is close enough to the last kill, give a reward sound
			if (level.time - attacker->client->lastKillTime < CARNAGE_REWARD_TIME) {
				// play excellent on player
				attacker->client->ps.persistant[PERS_EXCELLENT_COUNT]++;

				G_AddEvent(attacker, EV_HEHE1, 0);

				// add the sprite over the player's head
				SetAward(attacker->client, AWARD_EXCELLENT);
			}
			attacker->client->lastKillTime = level.time;
		}
	} else {
		if (g_gametype.integer != GT_LPS) {
			AddScore(self, self->r.currentOrigin, SCORE_SUICIDE, SCORE_SUICIDE_S);
		}
	}

	// Add team bonuses
	Team_FragBonuses(self, attacker);

	// if I committed suicide, the flag does not fall, it returns.
	if (meansOfDeath == MOD_SUICIDE) {
		if (self->client->ps.powerups[PW_NEUTRALFLAG]) { // only happens in One Flag CTF
			Team_ReturnFlag(TEAM_FREE);
			self->client->ps.powerups[PW_NEUTRALFLAG] = 0;
		} else if (self->client->ps.powerups[PW_REDFLAG]) { // only happens in standard CTF
			Team_ReturnFlag(TEAM_RED);
			self->client->ps.powerups[PW_REDFLAG] = 0;
		} else if (self->client->ps.powerups[PW_BLUEFLAG]) { // only happens in standard CTF
			Team_ReturnFlag(TEAM_BLUE);
			self->client->ps.powerups[PW_BLUEFLAG] = 0;
		}
	}

	// if client is in a nodrop area, don't drop anything (but return CTF flags!)
	contents = trap_PointContents(self->r.currentOrigin, -1);
	if (!(contents & CONTENTS_NODROP) && !level.cammode) {
		TossClientItems(self);
	} else {
		if (self->client->ps.powerups[PW_REDFLAG]) { // only happens in standard CTF
			Team_ReturnFlag(TEAM_RED);
		} else if (self->client->ps.powerups[PW_BLUEFLAG]) { // only happens in standard CTF
			Team_ReturnFlag(TEAM_BLUE);
		}
	}

	DeathmatchScoreboardMessage(self); // show scores
	// send updated scores to any clients that are following this one,
	// or they would get stale scoreboards
	for (i = 0; i < level.maxclients; i++) {
		gclient_t *client;

		client = &level.clients[i];
		if (client->pers.connected != CON_CONNECTED) {
			continue;
		}
		if (client->sess.sessionTeam != TEAM_SPECTATOR) {
			continue;
		}
		if (client->sess.spectatorClient == self->s.number) {
			DeathmatchScoreboardMessage(g_entities + i);
		}
	}

	self->takedamage = qfalse; // no gibbing in wop

	self->s.weapon = WP_NONE;
	self->s.powerups = 0;
	self->r.contents = CONTENTS_CORPSE;

	self->s.angles[0] = 0;
	self->s.angles[2] = 0;
	LookAtKiller(self, inflictor, attacker);

	VectorCopy(self->s.angles, self->client->ps.viewangles);

	self->s.loopSound = 0;

	self->r.maxs[2] = -8;

	// don't allow respawn until the death anim is done
	// g_forcerespawn may force spawning at some later time
	self->client->respawnTime = level.time + 1700;

	// remove powerups
	memset(self->client->ps.powerups, 0, sizeof(self->client->ps.powerups));

	{
		// normal death
		static int deathAnimationIndex;

		switch (deathAnimationIndex) {
		case 0:
			anim = BOTH_DEATH1;
			break;
		case 1:
			anim = BOTH_DEATH2;
			break;
		case 2:
		default:
			anim = BOTH_DEATH3;
			break;
		}

		// for the no-blood option, we need to prevent the health
		// from going to gib level
		if (self->health <= GIB_HEALTH) {
			self->health = GIB_HEALTH + 1;
		}

		self->client->ps.legsAnim = ((self->client->ps.legsAnim & ANIM_TOGGLEBIT) ^ ANIM_TOGGLEBIT) | anim;
		self->client->ps.torsoAnim = ((self->client->ps.torsoAnim & ANIM_TOGGLEBIT) ^ ANIM_TOGGLEBIT) | anim;

		G_AddEvent(self, EV_DEATH1 + deathAnimationIndex, killer);

		// the body can still be gibbed
		self->die = body_die;

		// globally cycle through the different death animations
		deathAnimationIndex = (deathAnimationIndex + 1) % 3;
	}

	trap_LinkEntity(self);
}

/*
================
CheckArmor
================
*/
static int CheckArmor(gentity_t *ent, int damage, int dflags) {
	gclient_t *client;
	int save;
	int count;

	if (!damage)
		return 0;

	client = ent->client;

	if (!client)
		return 0;

	if (dflags & DAMAGE_NO_ARMOR)
		return 0;

	// armor
	count = client->ps.stats[STAT_ARMOR];
	save = ceil(damage * ARMOR_PROTECTION);
	if (save >= count)
		save = count;

	if (!save)
		return 0;

	client->ps.stats[STAT_ARMOR] -= save;

	return save;
}

/*
============
G_Damage

targ		entity that is being damaged
inflictor	entity that is causing the damage
attacker	entity that caused the inflictor to damage targ
	example: targ=monster, inflictor=rocket, attacker=player

dir			direction of the attack for knockback
point		point at which the damage is being inflicted, used for headshots
damage		amount of damage being inflicted
knockback	force to be applied against targ as a result of the damage

inflictor, attacker, dir, and point can be NULL for environmental effects

dflags		these flags are used to control how G_Damage works
	DAMAGE_RADIUS			damage was indirect (from a nearby explosion)
	DAMAGE_NO_ARMOR			armor does not protect from this damage
	DAMAGE_NO_KNOCKBACK		do not affect velocity, just view angles
	DAMAGE_NO_PROTECTION	kills godmode, armor, everything
============
*/
void G_Damage(gentity_t *victim, gentity_t *inflictor, gentity_t *attacker, vec3_t dir, vec3_t point, int damage,
			  int dflags, int mod) {
	gclient_t *client;
	int take;
	int asave;
	int knockback;
	int max;

	if (!victim->takedamage) {
		return;
	}

	// the intermission has already been qualified for, so don't
	// allow any extra scoring
	if (level.intermissionQueued) {
		return;
	}

	client = victim->client;

	if ((!attacker || (attacker->s.eType != ET_PLAYER)) && (client && (client->lastSentFlying > -1)) &&
		((mod == MOD_FALLING /* just in case */) || (mod == MOD_LAVA) || (mod == MOD_SLIME) ||
		 (mod == MOD_TRIGGER_HURT) || (mod == MOD_SUICIDE))) {
		// FIXME: Magical constant
		if ((client->lastSentFlyingTime + 5000) < level.time) {
			// More than 5 seconds, not a kill!
			client->lastSentFlying = -1;
		} else {
			attacker = &g_entities[client->lastSentFlying];
		}
	}

	if (!inflictor) {
		inflictor = &g_entities[ENTITYNUM_WORLD];
	}
	if (!attacker) {
		attacker = &g_entities[ENTITYNUM_WORLD];
	}

	if (!victim->client && CantDamageTeamitem(victim, attacker)) {
		return;
	}

	// shootable doors / buttons don't actually have any health
	if (victim->s.eType == ET_MOVER) {
		if (victim->use && (victim->moverState == MOVER_POS1 || victim->moverState == ROTATOR_POS1)) {
			victim->use(victim, inflictor, attacker);
		}
		return;
	}
	// reduce damage by the attacker's handicap value
	// unless they are rocket jumping
	if (attacker->client && attacker != victim) {
		max = attacker->client->ps.stats[STAT_MAX_HEALTH];
		damage = damage * max / 100;
	}

	if (client) {
		if (client->noclip) {
			return;
		}
	}

	// freezetag
	if (G_FreezeTag()) {
		if (FT_PlayerIsFrozen(victim)) {
			if (mod == MOD_TRIGGER_HURT) {
				FT_RelocateToNearestSpawnPoint(victim);
				FT_FreezePlayer(victim, attacker);
			}
			return;
		} else if (FT_PlayerIsFrozen(attacker)) {
			if (mod == MOD_PUNCHY) {
				return;
			}
		} else if (mod == MOD_TELEFRAG) {
			FT_FreezePlayer(victim, attacker);
			FT_RelocateToNearestSpawnPoint(victim);
			return;
		}
	}

	if (!dir) {
		dflags |= DAMAGE_NO_KNOCKBACK;
	} else {
		VectorNormalize(dir);
	}

	// Modifiers / Instagib
	// Apply special instagib damage rules to target
	if (g_modInstagib.integer) {
		damage = Instagib_calculateDamage(victim, inflictor, attacker, damage, dflags, mod);
		dflags |= DAMAGE_NO_ARMOR;
	}

	knockback = damage;

	if ((mod == MOD_IMPERIUS_SPLASH) && (knockback > KNOCKBACK_MAX_IMPERIUS))
		knockback = KNOCKBACK_MAX_IMPERIUS;

	if ((mod == MOD_PUMPER) && (victim == attacker)) // PUMPER
		knockback *= KNOCKBACK_MOD_PUMPER;

	if (mod == MOD_BALLOONY_SPLASH) // BALLOONY
		knockback *= KNOCKBACK_MOD_BALLOONY;

	if (knockback > 200) {
		knockback = 200;
	}
	if (victim->flags & FL_NO_KNOCKBACK) {
		knockback = 0;
	}
	if (dflags & DAMAGE_NO_KNOCKBACK) {
		knockback = 0;
	}

	// figure momentum add, even if the damage won't be taken
	if (knockback && victim->client) {
		vec3_t kvel;
		float mass;

		mass = 200;

		VectorScale(dir, g_knockback.value * (float)knockback / mass, kvel);
		VectorAdd(victim->client->ps.velocity, kvel, victim->client->ps.velocity);

		// set the timer so that the other client can't cancel
		// out the movement immediately
		if (!victim->client->ps.pm_time) {
			int t;

			t = knockback * 2;
			if (t < 50) {
				t = 50;
			}
			if (t > 200) {
				t = 200;
			}
			victim->client->ps.pm_time = t;
			victim->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
		}

		// Remeber the last person to hurt the player
		if ((victim == attacker) || OnSameTeam(victim, attacker)) {
			victim->client->lastSentFlying = -1;
		} else {
			victim->client->lastSentFlying = attacker->s.number;
			victim->client->lastSentFlyingTime = level.time;
		}
	}

	// check for completely getting out of the damage
	if (!(dflags & DAMAGE_NO_PROTECTION)) {

		// if TF_NO_FRIENDLY_FIRE is set, don't do damage to the target
		// if the attacker was on the same team
		// NOTE: With Boomies we can have targ==attacker, should be considered friendlyfire instead of selfdamage
		if ((victim != attacker && (OnSameTeam(victim, attacker) || IsItemSameTeam(attacker, victim))) ||
			(victim == attacker && mod == MOD_BOOMIES)) {
			if (!g_friendlyFire.integer) {
				return;
			}
		}

		// check for godmode
		if (victim->flags & FL_GODMODE) {
			return;
		}
	}

	if (g_gametype.integer == GT_CATCH && !G_IsKillerDuck(victim)) {
		return;
	}

	// battlesuit protects from all radius damage (but takes knockback)
	// and protects 50% against all damage
	if (client && client->ps.powerups[PW_PADPOWER]) {
		G_AddEvent(victim, EV_POWERUP_PADPOWER, 0);
		if ((dflags & DAMAGE_RADIUS) || (mod == MOD_FALLING)) {
			return;
		}
		damage *= 0.5f;
	}

	// add to the attacker's hit counter (if the target isn't a general entity like a prox mine)
	if (attacker->client && victim != attacker && victim->health > 0 && client && victim->s.eType != ET_MISSILE &&
		victim->s.eType != ET_GENERAL) {
		if (OnSameTeam(victim, attacker)) {
			attacker->client->ps.persistant[PERS_HITS]--;
		} else {
			attacker->client->ps.persistant[PERS_HITS]++;
		}
		attacker->client->ps.persistant[PERS_ATTACKEE_ARMOR] = (victim->health << 8) | (client->ps.stats[STAT_ARMOR]);
	}

	// always give half damage if hurting self
	// calculated after knockback, so rocket jumping works
	if (victim == attacker) {
		damage *= 0.5f;
	}

	if (damage < 1) {
		damage = 1;
	}
	take = damage;

	// save some from armor
	asave = CheckArmor(victim, take, dflags);
	take -= asave;

	if (g_debugDamage.integer) {
		Com_Printf("%i: client:%i health:%i damage:%i armor:%i\n", level.time, victim->s.number, victim->health, take, asave);
	}

	if (g_logDamage.integer) {
		// "Damage: target-cid inflictor-cid attacker-cid damage meansofdeath"
		G_LogPrintf("Damage: %ld %ld %ld %d %d\n", (victim - g_entities), (inflictor - g_entities),
					(attacker - g_entities), take, mod);
	}

	// add to the damage inflicted on a player this frame
	// the total will be turned into screen blends and view angle kicks
	// at the end of the frame
	if (client) {
		if (attacker) {
			client->ps.persistant[PERS_ATTACKER] = attacker->s.number;
		} else if (client->lastSentFlying > -1) {
			client->ps.persistant[PERS_ATTACKER] = client->lastSentFlying;
		} else {
			client->ps.persistant[PERS_ATTACKER] = ENTITYNUM_WORLD;
		}
		client->damage_armor += asave;
		client->damage_blood += take;
		client->damage_knockback += knockback;
		if (dir) {
			VectorCopy(dir, client->damage_from);
			client->damage_fromWorld = qfalse;
		} else {
			VectorCopy(victim->r.currentOrigin, client->damage_from);
			client->damage_fromWorld = qtrue;
		}
	}

	// See if it's the player hurting the enemy flag/cartridge carrier
	if (g_gametype.integer == GT_CTF || g_gametype.integer == GT_SPRAY || g_gametype.integer == GT_SPRAY) {
		Team_CheckHurtCarrier(victim, attacker);
	}

	if (victim->client) {
		// set the last client who damaged the target
		victim->client->lasthurt_client = attacker->s.number;
		victim->client->lasthurt_mod = mod;
	}

	// do the damage
	if (take) {
		victim->health = victim->health - take;
		if (victim->client) {
			victim->client->ps.stats[STAT_HEALTH] = victim->health;
		}

		if (victim->health <= 0) {
			if (client)
				victim->flags |= FL_NO_KNOCKBACK;

			if (victim->health < -999)
				victim->health = -999;

			victim->enemy = attacker;

			// don't kill players in freezetag
			if (G_FreezeTag() && !FT_PlayerIsFrozen(victim)) {
				if (mod == MOD_TRIGGER_HURT) {
					ClientSpawn(victim); // relocate players to a spawnpoint
				}
				FT_FreezePlayer(victim, attacker);
			} else {
				victim->die(victim, inflictor, attacker, take, mod);
			}
			return;
		} else if (victim->pain) {
			victim->pain(victim, attacker, take);
		}
	}
}

/*
============
CanDamage

Returns qtrue if the inflictor can directly damage the target.  Used for
explosions and melee attacks.
============
*/
static qboolean CanDamage(const gentity_t *targ, const vec3_t origin) {
	vec3_t dest;
	trace_t tr;
	vec3_t midpoint;

	// use the midpoint of the bounds instead of the origin, because
	// bmodels may have their origin is 0,0,0
	VectorAdd(targ->r.absmin, targ->r.absmax, midpoint);
	VectorScale(midpoint, 0.5, midpoint);

	VectorCopy(midpoint, dest);
	trap_Trace(&tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0 || tr.entityNum == targ->s.number)
		return qtrue;

	// this should probably check in the plane of projection,
	// rather than in world coordinate, and also include Z
	VectorCopy(midpoint, dest);
	dest[0] += 15.0f;
	dest[1] += 15.0f;
	trap_Trace(&tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy(midpoint, dest);
	dest[0] += 15.0f;
	dest[1] -= 15.0f;
	trap_Trace(&tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy(midpoint, dest);
	dest[0] -= 15.0f;
	dest[1] += 15.0f;
	trap_Trace(&tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy(midpoint, dest);
	dest[0] -= 15.0f;
	dest[1] -= 15.0f;
	trap_Trace(&tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	return qfalse;
}

/*
============
G_RadiusDamage
============
*/
qboolean G_RadiusDamage(vec3_t origin, gentity_t *attacker, float damage, float radius, gentity_t *ignore, int mod) {
	float points, dist;
	gentity_t *ent;
	int entityList[MAX_GENTITIES];
	int numListedEntities;
	vec3_t mins, maxs;
	vec3_t v;
	vec3_t dir;
	int i, e;
	qboolean hitClient = qfalse;

	if (radius < 1) {
		radius = 1;
	}

	for (i = 0; i < 3; i++) {
		mins[i] = origin[i] - radius;
		maxs[i] = origin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox(mins, maxs, entityList, MAX_GENTITIES);

	for (e = 0; e < numListedEntities; e++) {
		ent = &g_entities[entityList[e]];

		if (ent == ignore)
			continue;
		if (!ent->takedamage)
			continue;

		// find the distance from the edge of the bounding box
		for (i = 0; i < 3; i++) {
			if (origin[i] < ent->r.absmin[i]) {
				v[i] = ent->r.absmin[i] - origin[i];
			} else if (origin[i] > ent->r.absmax[i]) {
				v[i] = origin[i] - ent->r.absmax[i];
			} else {
				v[i] = 0;
			}
		}

		dist = VectorLength(v);
		if (dist >= radius) {
			continue;
		}

		points = damage * (1.0f - dist / radius);

		if (CanDamage(ent, origin)) {
			if (LogAccuracyHit(ent, attacker)) {
				hitClient = qtrue;
			}
			VectorSubtract(ent->r.currentOrigin, origin, dir);
			// push the center of mass higher than the origin so players
			// get knocked into the air more
			dir[2] += 24;
			G_Damage(ent, NULL, attacker, dir, origin, (int)points, DAMAGE_RADIUS, mod);
		}
	}

	return hitClient;
}
