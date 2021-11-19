// weapon configuration
#include "inv.h"

//damage types
#define DAMAGETYPE_IMPACT			1		//damage on impact
#define DAMAGETYPE_RADIAL			2		//radial damage
#define DAMAGETYPE_VISIBLE			4		//damage to all entities visible to the projectile
#define DAMAGETYPE_IGNOREARMOR	8		//projectile goes right through armor


//===========================================================================
// punchy
//===========================================================================

projectileinfo //for Gauntlet
{
	name				"gauntletdamage"
	damage				50
	damagetype			DAMAGETYPE_IMPACT
}

weaponinfo //Gauntlet
{
	name				"Punchy"
	number				WEAPONINDEX_PUNCHY
	projectile			"gauntletdamage"
	numprojectiles		1
	speed				0
} //end weaponinfo

//===========================================================================
// nipper
//===========================================================================

projectileinfo
{
	name				"machinegunbullet"
	damage				15
	damagetype			DAMAGETYPE_IMPACT
}

weaponinfo
{
	name				"Nipper"
	number				WEAPONINDEX_NIPPER
	projectile			"machinegunbullet"
	numprojectiles		1
	speed				3200
} //end weaponinfo

//===========================================================================
// pumper
//===========================================================================

projectileinfo
{
	name				"shotgunbullet"
	damage				70
	radius				80
	damagetype			$evalint(DAMAGETYPE_IMPACT|DAMAGETYPE_RADIAL)
}

weaponinfo
{
	name				"Pumper"
	number				WEAPONINDEX_PUMPER
	projectile			"shotgunbullet"
	numprojectiles		1
	speed				0
} //end weaponinfo

//===========================================================================
// Balloony
//===========================================================================

projectileinfo
{
	name				"grenade"
	damage				120
	radius				250
	damagetype			$evalint(DAMAGETYPE_IMPACT|DAMAGETYPE_RADIAL)
}

weaponinfo
{
	name				"Balloony"
	number				WEAPONINDEX_BALLOONY
	projectile			"grenade"
	numprojectiles		1
	speed				900
} //end weaponinfo

//===========================================================================
// Betty
//===========================================================================
projectileinfo
{
	name				"bettydamage"
	damage				100
	radius				120
	damagetype			$evalint(DAMAGETYPE_IMPACT|DAMAGETYPE_RADIAL)
}

weaponinfo //Gauntlet
{
	name				"Betty"
	number				WEAPONINDEX_BETTY
	projectile			"bettydamage"
	numprojectiles		1
	speed				900
}

//===========================================================================
// Boaster
//===========================================================================

projectileinfo //for Lightning
{
	name				"lightning"
	damage				8
	damagetype			DAMAGETYPE_IMPACT
}

weaponinfo //Railgun
{
	name				"Boaster"
	number				WEAPONINDEX_BOASTER
	projectile			"lightning"
	numprojectiles		1
	speed				1250
} //end weaponinfo

//===========================================================================
// Splasher
//===========================================================================

projectileinfo //for Railgun
{
	name				"rail"
	damage				110
	damagetype			DAMAGETYPE_IMPACT
}

weaponinfo //Railgun
{
	name				"Splasher"
	number				WEAPONINDEX_SPLASHER
	projectile			"rail"
	numprojectiles		1
	speed				100000
} //end weaponinfo

//===========================================================================
// BubbleG
//===========================================================================

projectileinfo //for Plasma Gun
{
	name				"plasma"
	damage				25
	damagetype			DAMAGETYPE_IMPACT
}

weaponinfo //Plasma Gun
{
	name				"BubbleG"
	number				WEAPONINDEX_BUBBLEG
	projectile			"plasma"
	numprojectiles		1
	speed				1800
} //end weaponinfo

//===========================================================================
// Imperius
//===========================================================================

projectileinfo
{
	name				"bfgexplosion"
	damage				120
	radius				800
	damagetype			$evalint(DAMAGETYPE_IMPACT|DAMAGETYPE_RADIAL)
}

weaponinfo //BFG10K
{
	name				"Imperius"
	number				WEAPONINDEX_IMPERIUS
	projectile			"bfgexplosion"
	numprojectiles		1
	speed				1100
} //end weaponinfo

weaponinfo
{
	name "KMA97"
	number				WEAPONINDEX_KMA97
	projectile			"rail"
	numprojectiles		1
	speed				100000
}

