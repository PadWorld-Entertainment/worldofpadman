chat "monsterhawk"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"Hawk's hunting. Don't stop moving, ", 1, ".";
	}

	type "game_exit"
	{
		"Trail's cold. I'm gone.";
	}

	type "level_start"
	{
		"Nipper up. I saw you already.";
	}

	type "level_end"
	{
		2, " was the loudest prey. Not the smartest.";
	}

	type "level_end_victory"
	{
		"Caught.";
	}

	type "level_end_lose"
	{
		"You slipped the dive, ", 2, ".";
	}

	type "kill_insult"
	{
		"Too slow to be breakfast.";
	}

	type "kill_praise"
	{
		"Good sprint, ", 0, ". I still stoop faster.";
	}

	type "kill_gauntlet"
	{
		"Talons today. Punchy tomorrow.";
	}

	type "kill_rail"
	{
		"One splash. I prefer the Nipper, but fine.";
	}

	type "death_insult"
	{
		"Clipped a wing. Still flying.";
	}

	type "death_praise"
	{
		"Sharp eyes, ", 0, ".";
	}

	type "death_gauntlet"
	{
		"Let you under me. Won't happen twice.";
	}

	type "death_rail"
	{
		"Splasher from cover. I'll find the perch.";
	}

	type "enemy_suicide"
	{
		0, " flew into the floor.";
	}

	type "hit_nokill"
	{
		"You revealed your spot. Thanks.";
	}
}
