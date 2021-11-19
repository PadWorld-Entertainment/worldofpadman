// item configuration
#include "inv.h"

#define ITEM_NONE					0
#define ITEM_AMMO					1
#define ITEM_WEAPON					2
#define ITEM_HEALTH					3
#define ITEM_ARMOR					4
#define ITEM_POWERUP				5
#define ITEM_KEY					6
#define ITEM_FLAG					7
#define ITEM_ROAM					8

//===================================
// ARMOR
//===================================

iteminfo "item_armor_shard"
{
	name					"PAD SHARD"
	model					"models/powerups/armor/pad_shard.md3"
	modelindex				MODELINDEX_ARMORSHARD
	type					ITEM_ARMOR
	index					INVENTORY_ARMOR
	respawntime				25
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "item_armor_padshield"
{
	name					"PAD SHiELD"
	model					"models/powerups/armor/pad_shield.md3"
	modelindex				MODELINDEX_PADSHIELD
	type					ITEM_ARMOR
	index					INVENTORY_ARMOR
	respawntime				25
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo


//===================================
// WEAPONS
//===================================
//*
iteminfo "weapon_punchy"
{
	name					"PUNCHY"
	model					"models/weapons2/punchy/punchy.md3"
	modelindex				MODELINDEX_PUNCHY
	type					ITEM_WEAPON
	index					INVENTORY_PUNCHY
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo*/

iteminfo "weapon_pumper"
{
	name					"PUMPER"
	model					"models/weapons2/pumper/pumper.md3"
	modelindex				MODELINDEX_PUMPER
	type					ITEM_WEAPON
	index					INVENTORY_PUMPER
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "weapon_nipper"
{
	name					"NiPPER"
	model					"models/weapons2/nipper/nipper.md3"
	modelindex				MODELINDEX_NIPPER
	type					ITEM_WEAPON
	index					INVENTORY_NIPPER
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "weapon_balloony"
{
	name					"BALLOONY"
	model					"models/weapons2/balloony/balloony.md3"
	modelindex				MODELINDEX_BALLOONY
	type					ITEM_WEAPON
	index					INVENTORY_BALLOONY
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "weapon_betty"
{
	name					"BETTY"
	model					"models/weapons2/betty/betty.md3"
	modelindex				MODELINDEX_BETTY
	type					ITEM_WEAPON
	index					INVENTORY_BETTY
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "weapon_boaster"
{
	name					"BOASTER"
	model					"models/weapons2/boaster/boaster.md3"
	modelindex				MODELINDEX_BOASTER
	type					ITEM_WEAPON
	index					INVENTORY_BOASTER
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "weapon_splasher"
{
	name					"SPLASHER"
	model					"models/weapons2/splasher/splasher.md3"
	modelindex				MODELINDEX_SPLASHER
	type					ITEM_WEAPON
	index					INVENTORY_SPLASHERAMMO
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "weapon_bubbleg"
{
	name					"BUBBLE.G."
	model					"models/weapons2/bubbleg/bubbleg.md3"
	modelindex				MODELINDEX_BUBBLEG
	type					ITEM_WEAPON
	index					INVENTORY_BUBBLEG
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "weapon_imperius"
{
	name					"IMPERiUS"
	model					"models/weapons2/imperius/imperius.md3"
	modelindex				MODELINDEX_IMPERIUS
	type					ITEM_WEAPON
	index					INVENTORY_IMPERIUS
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "weapon_spraypistol"
{
	name					"Spray Pistol"
	model					"models/weapons2/spraypistol/spraypistol.md3"
	modelindex				MODELINDEX_SPRAYPISTOL
	type					ITEM_WEAPON
	index					INVENTORY_SPRAYPISTOL
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

//===================================
// AMMO
//===================================

iteminfo "ammo_pumper"
{
	name					"PUMPER AMMO"
	model					"models/powerups/ammo/pumper_am.md3"
	modelindex				MODELINDEX_PUMPERAMMO
	type					ITEM_AMMO
	index					INVENTORY_PUMPERAMMO
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "ammo_nipper"
{
	name					"NiPPER AMMO"
	model					"models/powerups/ammo/nipper_am.md3"
	modelindex				MODELINDEX_NIPPERAMMO
	type					ITEM_AMMO
	index					INVENTORY_NIPPERAMMO
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "ammo_balloony"
{
	name					"BALLOONY AMMO"
	model					"models/powerups/ammo/balloony_am.md3"
	modelindex				MODELINDEX_BALLOONYAMMO
	type					ITEM_AMMO
	index					INVENTORY_BALLOONYAMMO
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "ammo_bubbleg"
{
	name					"BUBBLE.G. AMMO"
	model					"models/powerups/ammo/bubbleg_am.md3"
	modelindex				MODELINDEX_BUBBLEGAMMO
	type					ITEM_AMMO
	index					INVENTORY_BUBBLEGAMMO
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "ammo_boaster"
{
	name					"BOASTER AMMO"
	model					"models/powerups/ammo/boaster_am.md3"
	modelindex				MODELINDEX_BOASTERAMMO
	type					ITEM_AMMO
	index					INVENTORY_BOASTERAMMO
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "ammo_betty"
{
	name					"BETTY AMMO"
	model					"models/powerups/ammo/betty_am.md3"
	modelindex				MODELINDEX_BETTYAMMO
	type					ITEM_AMMO
	index					INVENTORY_BETTYAMMO
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "ammo_splasher"
{
	name					"SPLASHER AMMO"
	model					"models/powerups/ammo/splasher_am.md3"
	modelindex				MODELINDEX_SPLASHERAMMO
	type					ITEM_AMMO
	index					INVENTORY_SPLASHERAMMO
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "ammo_imperius"
{
	name					"IMPERiUS AMMO"
	model					"models/powerups/ammo/imperius_am.md3"
	modelindex				MODELINDEX_IMPERIUSAMMO
	type					ITEM_AMMO
	index					INVENTORY_IMPERIUSAMMO
	respawntime				30
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "ammo_spray_r"
{
	name					"red Cartridge"
	model					"models/weapons2/spraypistol/cart_red.md3"
	modelindex				MODELINDEX_RCARTRIDGE
	type					ITEM_AMMO
	index					INVENTORY_RCARTRIDGE
	respawntime				0
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "ammo_spray_b"
{
	name					"blue Cartridge"
	model					"models/weapons2/spraypistol/cart_blue.md3"
	modelindex				MODELINDEX_BCARTRIDGE
	type					ITEM_AMMO
	index					INVENTORY_BCARTRIDGE
	respawntime				0
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "ammo_spray_n"
{
	name					"neutral Cartridge"
	model					"models/weapons2/spraypistol/cart_neutral.md3"
	modelindex				MODELINDEX_NCARTRIDGE
	type					ITEM_AMMO
	index					INVENTORY_NCARTRIDGE
	respawntime				0
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

//===================================
// POWERUPS
//===================================

// eigena cyr{

iteminfo "holdable_floater"
{
	name					"FLOATER"
	model					"models/powerups/instant/floater.md3"
	modelindex				MODELINDEX_FLOATER
	type					ITEM_POWERUP
	index					INVENTORY_FLOATER
	respawntime				120
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo


iteminfo "holdable_killerducks"
{
	name					"KiLLERDUCKS"
	model					"models/weapons2/killerducks/pad_killerduck_pu.md3"
	modelindex				MODELINDEX_KILLERDUCKS
	type					ITEM_POWERUP
	index					INVENTORY_KILLERDUCKS
	respawntime				120
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "holdable_bambam"
{
	name					"BAMBAM"
	model					"models/powerups/instant/bambam.md3"
	modelindex				MODELINDEX_BAMBAM
	type					ITEM_POWERUP
	index					INVENTORY_BAMBAM
	respawntime				120
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "holdable_boomies"
{
	name					"BOOMiES"
	model					"models/powerups/instant/boomies.md3"
	modelindex				MODELINDEX_BOOMIE
	type					ITEM_POWERUP
	index					INVENTORY_BOOMIE
	respawntime				120
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "item_padpower"
{
	name					"PADPOWER"
	model					"models/powerups/instant/padpower.md3"
	modelindex				MODELINDEX_PADPOWER
	type					ITEM_POWERUP
	index					INVENTORY_PADPOWER
	respawntime				120
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "item_climber"
{
	name					"CLiMBER"
	model					"models/powerups/instant/climber.md3"
	modelindex				MODELINDEX_CLIMBER
	type					ITEM_POWERUP
	index					INVENTORY_CLIMBER
	respawntime				120
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "item_speedy"
{
	name					"SPEEDY"
	model					"models/powerups/instant/speedy.md3"
	modelindex				MODELINDEX_SPEEDY
	type					ITEM_POWERUP
	index					INVENTORY_SPEEDY
	respawntime				60
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "item_jumper"
{
	name					"JUMPER"
	model					"models/powerups/instant/jumper.md3"
	modelindex				MODELINDEX_JUMPER
	type					ITEM_POWERUP
	index					INVENTORY_JUMPER
	respawntime				120
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "item_visionless"
{
	name					"ViSiONLESS"
	model					"models/powerups/instant/visionless.md3"
	modelindex				MODELINDEX_VISIONLESS
	type					ITEM_POWERUP
	index					INVENTORY_VISIONLESS
	respawntime				120
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

iteminfo "item_revival"
{
	name					"REViVAL"
	model					"models/powerups/instant/revival.md3"
	modelindex				MODELINDEX_REVIVAL
	type					ITEM_POWERUP
	index					INVENTORY_REVIVAL
	respawntime				120
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo
// eigene cyr}

//===================================
// bot roam item
//===================================

iteminfo "item_botroam"
{
	name					"Bot Roam Goal"
	model					""
	modelindex				0
	type					ITEM_ROAM
	index					0
	mins					{-15,-15,-15}
	maxs					{15,15,15}
}

//===================================
// CTF flags
//===================================

iteminfo "team_CTL_redlolly"
{
	name					"red Lolly"
	model					"models/ctl/lollipop_red.md3"
	modelindex				MODELINDEX_REDFLAG
	type					ITEM_FLAG
	index					INVENTORY_REDFLAG
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo


iteminfo "team_CTL_bluelolly"
{
	name					"blue Lolly"
	model					"models/ctl/lollipop_blue.md3"
	modelindex				MODELINDEX_BLUEFLAG
	type					ITEM_FLAG
	index					INVENTORY_BLUEFLAG
	mins					{-15,-15,-15}
	maxs					{15,15,15}
} //end iteminfo

