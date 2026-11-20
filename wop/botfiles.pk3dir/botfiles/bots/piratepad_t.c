chat "piratepad"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"Arr, PiratePad on deck. Balloons be the cannon.";
		"Welcome aboard, ", 1, ". The plank is optional.";
	}

	type "game_exit"
	{
		"I smell a bigger treasure. Shoving off.";
	}

	type "level_start"
	{
		"Grapple the high ground. Lob the rest.";
	}

	type "level_end"
	{
		2, " stole the chest. There'll be another tide.";
	}

	type "level_end_victory"
	{
		"The map is my harbor.";
	}

	type "level_end_lose"
	{
		"Scuttled by ", 2, ". I'll raid you proper next time.";
	}

	type "kill_insult"
	{
		"Overboard, ", 0, ".";
	}

	type "kill_praise"
	{
		"You sailed well. The balloon still landed.";
	}

	type "kill_gauntlet"
	{
		"Punchy is a boarding axe today.";
	}

	type "kill_rail"
	{
		"A splash across the bow. Down you go.";
	}

	type "death_insult"
	{
		"Barnacles. I'll swim back.";
	}

	type "death_praise"
	{
		"Fine broadside, ", 0, ".";
	}

	type "death_gauntlet"
	{
		"Boarded by a puppet. The crew will never hear of this.";
	}

	type "death_rail"
	{
		"Sniped from the crow's nest. Cowardly, accurate.";
	}

	type "enemy_suicide"
	{
		0, " walked the plank with no help from me.";
	}

	type "hit_nokill"
	{
		"A warning shot across your ears.";
	}
}
