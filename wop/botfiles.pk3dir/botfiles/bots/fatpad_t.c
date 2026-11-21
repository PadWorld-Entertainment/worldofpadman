chat "fatpad"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"FatPad is here. I walked. You're welcome.";
		"Leave the Pumper. I called it, ", 1, ".";
	}

	type "game_exit"
	{
		"That's enough running. There was too much running.";
	}

	type "level_start"
	{
		"I don't chase. You will come to the shotgun.";
	}

	type "level_end"
	{
		2, " did all that jumping for a number. Exhausting to watch.";
	}

	type "level_end_victory"
	{
		"Slow and wide. Still first.";
	}

	type "level_end_lose"
	{
		"Fine, ", 2, ". I'll sit on the next round.";
	}

	type "kill_insult"
	{
		"You ran all that way to meet the Pumper.";
	}

	type "kill_praise"
	{
		"Quick feet, ", 0, ". Short life.";
	}

	type "kill_gauntlet"
	{
		"Didn't even lift the gun. You're welcome.";
	}

	type "kill_rail"
	{
		"I aimed once. It counted.";
	}

	type "death_insult"
	{
		"Oof. Rude, and from a distance.";
	}

	type "death_praise"
	{
		"Okay, ", 0, ", that one moved me.";
	}

	type "death_gauntlet"
	{
		"A puppet got inside the belly. Embarrassing.";
	}

	type "death_rail"
	{
		"Splasher. I am a large target. Still rude.";
	}

	type "enemy_suicide"
	{
		0, " saved me the walk.";
	}

	type "hit_nokill"
	{
		"Was that a pebble?";
	}
}
