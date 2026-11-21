chat "ghostpirate"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"GhostPirate. You heard the chain. Too late, ", 1, ".";
	}

	type "game_exit"
	{
		"Dawn. I don't work days.";
	}

	type "level_start"
	{
		"The Splasher fires through more than walls.";
	}

	type "level_end"
	{
		2, " still has a pulse. How traditional.";
	}

	type "level_end_victory"
	{
		"The wreck is mine.";
	}

	type "level_end_lose"
	{
		2, " banished me for one round. I'll be in the walls.";
	}

	type "kill_insult"
	{
		"You looked right through me. Fatal.";
	}

	type "kill_praise"
	{
		"Brave, ", 0, ", for someone who can still bleed.";
	}

	type "kill_gauntlet"
	{
		"Cold hands. Colder Punchy.";
	}

	type "kill_rail"
	{
		"The splash you never saw coming.";
	}

	type "death_insult"
	{
		"I was dead before. This is a hobby.";
	}

	type "death_praise"
	{
		"You can touch a ghost, ", 0, ". Briefly.";
	}

	type "death_gauntlet"
	{
		"A living puppet. The irony is noted.";
	}

	type "death_rail"
	{
		"Splashed by the living. I'll return the courtesy.";
	}

	type "enemy_suicide"
	{
		0, " joined the crew early.";
	}

	type "hit_nokill"
	{
		"You shot the fog. Try again.";
	}
}
