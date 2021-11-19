Christmas Theme
===============
	This is the updated 2009 version of the 2008 World of Padman X-mas modification for World
	of Padman 1.2. Most of the ingame items, powerups, healthstations etc. have been replaced
	by a Christmas-themed version. The function of all items remains the same.
	In this updated version every character also got one Christmas themed skin and some menus
	have been edited. Bot support for the "Freezetag" mode has been added.

Freezetag
=========

	Freezetag transforms Teamplay/TDM into... something not boring :P Players are not knocked
	out and respawned but literally frozen in place, waiting to be "thawed" by a teammate.
	To thaw a member of your team stand next to them until the "thaw-bar" fills up.
	The team that first manages to freeze the entire opposing team wins the round.

Music
=====
	Included in this package are 6 Songs by the Goyette Christmas Carolers
	(www.soundclick.com/goyettechristmascarolers) and 2 Songs by Jonathan Coulton
	(www.jonathancoulton.com). These artists took no part in the creation of this mod and their
	original work is included under the creative commons license.
	The licenses used are:
	- for "silent night" and "gabriels message" by the Goyette Christmas Carolers:
		Attribution-Noncommercial-No Derivative Works 3.0 Unported
		http://creativecommons.org/licenses/by-nc-nd/3.0/
	- for "jolly old saint nicholas", "angels from the realm of glory", "from heaven high" 
	and "a child is born in bethlehem" by the Goyette Christmas Carolers:
		Attribution-Noncommercial-Share Alike 3.0 Unported
		http://creativecommons.org/licenses/by-nc-sa/3.0/
	- for "Chiron Beta Prime" and "Podsafe Christmas Song" by Jonathan Coulton:
		Attribution-Noncommercial 3.0 Unported
		http://creativecommons.org/licenses/by-nc/3.0/



General Server Cvars:
=====================

g_noSecrets
-----------

	values: "1" or "0"

	This option controls if secrets such as trap doors and buttons, present in the wop1.x and padpack
	maps, can be use or not.

g_noPowerups

	values: "1" or "0"

	This option controls if powerups and holdables are spawned on the map.


Freezetag Server Cvars:
=======================

g_ft_numRounds
--------------

	values: positive integers

	Specifies how many rounds are played per map.

g_ft_playAllRounds
------------------

	values: "1" or "0"

	Specifies if all rounds set with g_ft_numRounds are played or if the match ends when one team
	can not possibly win in the remaining rounds left to play.

g_ft_lateJoinTime
-----------------

	values: positive integers

	example: "15"

	Specifies the number of seconds a player has time to join a team without penalty. After this time
	is expired joining a team will automatically freeze the player.

g_ft_useWeaponSet
-----------------

	values: pairs of string and integer, all tokens seperated by a ":"

	example: "punchy:-1:betty:15:pumper:8"

	This option will enable alternate freezetag gameplay when it is not set to either "" nor "0".
	When enabled it will remove all weapon spawns from the map, it will convert all ammo spawns into
	one kind of ammo which will universally charge any weapon and it will spawn every player with the
	weapon and ammo specified as its value.
	
g_ft_weaponSetAmmoRatio
-----------------------

	values: positive floating point numbers

	example: "0.5"

	This option specifies how much ammo is recharged from the universal ammo enabled with
	g_ft_useWeaponSet. The amount is based on the initual fill chosen.
	g_ft_useWeaponSet "pumper:30" and g_ft_weaponSetAmmoRatio "0.5" will result in a recharge
	of 15 for every universal ammo picked up.


Freezetag Client Cvars:
=======================

cg_ft_thawerIconX
cg_ft_thawerIconY

	values: positive integers in the range [0, 640] for ~X and [0, 480] for ~Y

	Specifies the location of the "thawing" icon displayed to the thawer, ie the player doing the
	thawing for a teammate.