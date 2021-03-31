//
// bg_game.h -- Game relevant constants like weapon damage, fire rate, spread etc.
//              Some of these don not need to be in both game.
//

// Weapons

#define ADDTIME_WEAPONCHANGE_BEGIN	150
#define ADDTIME_WEAPONCHANGE_FINISH	200
#define ADDTIME_WEAPON_NOAMMO		200

// TODO: Addtime killerducks, boomies

#define MAXAMMO_WEAPON		200

#define DAMAGE_PUNCHY		50
#define ADDTIME_PUNCHY		400

#define DAMAGE_NIPPER			12
#define	SPEED_NIPPER			3200
#define DURATION_NIPPER			10000
#define ADDTIME_AUTOFIRE_NIPPER	250
#define ADDTIME_NIPPER			150

#define DAMAGE_PUMPER			70
#define SPLASHDMG_PUMPER		50
#define SPLASHRAD_PUMPER		80
#define RANGE_PUMPER			1024
#define KNOCKBACK_MOD_PUMPER	2.0
#define ADDTIME_PUMPER			1000

#define DAMAGE_BALLOONY			80
#define SPLASHDMG_BALLOONY		80
#define SPLASHRAD_BALLOONY		200
#define DURATION_BALLOONY		2500
#define KNOCKBACK_MOD_BALLOONY	2.0
#define ADDTIME_BALLOONY		800
// TODO: #define SPEED_BALLOONY?

#define DAMAGE_BETTY		100
#define SPLASHDMG_BETTY		100
#define SPLASHRAD_BETTY		120
#define SPEED_BETTY			1000
#define DURATION_BETTY		15000
#define ADDTIME_BETTY		800

#define DAMAGE_BOASTER		3
#define DAMAGE_DOT_BOASTER	3
#define SPEED_BOASTER		1250
#define DURATION_BOASTER	1500
#define ADDTIME_BOASTER		50
#define ADDTIME_DOT_BOASTER	200
#define BOASTER_DOT_MAXTIME	1000


#define DAMAGE_SPLASHER		100
// FIXME: Make this instant hit via trap_Trace
#define SPEED_SPLASHER		100000
#define DURATION_SPLASHER	10000
#define ADDTIME_SPLASHER	2000

#define DAMAGE_BUBBLEG		20
#define SPEED_BUBBLEG		1800
#define DURATION_BUBBLEG	10000
#define ADDTIME_BUBBLEG		100

#define DAMAGE_IMPERIUS			120
#define SPLASHRAD_IMPERIUS		800 // NOTE: Differs from usual damage with splashDamage, see G_RunExplosion
#define SPEED_IMPERIUS			900
#define DURATION_IMPERIUS		10000
#define KNOCKBACK_MAX_IMPERIUS	100
#define MAXAMMO_IMPERIUS		2
#define CHARGETIME_IMPERIUS		1800
#define OVERCHARGETIME_IMPERIUS	3000
#define ADDTIME_IMPERIUS		800

// NOTE: More KMA97 should be here aswell, but it is supposed to be used
//       in instagib only. Should also be instant hit like splasher.
#define ADDTIME_KMA97		1500 // FIXME: Should this equal splasher?

// Spraypistol?

#define DAMAGE_KILLERDUCKS_BITE		2
#define DAMAGE_KILLERDUCKS_IMPACT	20
#define SPLASHDMG_KILLERDUCKS		50
#define SPLASHRAD_KILLERDUCKS		128
#define DURATION_KILLERDUCKS		10000
// TODO: #define SPEED_KILLERDUCKS, ADDTIME_KILLERDUCKS?

// TODO: ADDTIME_ for Bambams and Boomies

#define DAMAGE_BAMBAM		20
#define SPEED_BAMBAM		1800
#define DURATION_BAMBAM		10000
#define SPREAD_BAMBAM		5
#define HEALTH_BAMBAM		300
#define RANGE_BAMBAM_WIDTH	2000
#define RANGE_BAMBAM_HEIGHT	1000
#define RANGE_BAMBAM_VISIONLESS	320.0f // rather magical constant
#define FIREDELAY_BAMBAM	1000
#define FIRESTART_BAMBAM	1000
#define MAX_TEAM_BAMBAMS	5

// Not a real weapon ..
#define SPLASHDMG_BOOMIE	200
#define SPLASHRAD_BOOMIE	250
#define HEALTH_BOOMIE		80
#define RANGE_BOOMIE_WIDTH	150
#define RANGE_BOOMIE_HEIGHT	150
#define MAX_TEAM_BOOMIES	30

#define MAX_INTERMISSIONTIME 25000

// Holdables
// TODO: Merge into existing vars above?
#define MAX_FLOATER			30000
#define MAX_KILLERDUCKS		5
#define MAX_BOOMIES			3
#define MAX_BAMBAMS			1

#define	DROP_DELAY_LOLLY	5000

