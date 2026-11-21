chat "padlilly"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"PadLilly. Mind the Balloony, ", 1, ". It drifts.";
	}

	type "game_exit"
	{
		"The garden closes. Come back greener.";
	}

	type "level_start"
	{
		"High lobs. Let the balloon find you.";
	}

	type "level_end"
	{
		2, " grew fast this round.";
	}

	type "level_end_victory"
	{
		"Everything in season. Including me.";
	}

	type "level_end_lose"
	{
		"You picked the round, ", 2, ". I'll replant.";
	}

	type "kill_insult"
	{
		"Wrong bed to stand in, ", 0, ".";
	}

	type "kill_praise"
	{
		"Lovely footwork. The balloon was lovelier.";
	}

	type "kill_gauntlet"
	{
		"Even flowers have thorns. This one is Punchy.";
	}

	type "kill_rail"
	{
		"A hard splash for a soft map.";
	}

	type "death_insult"
	{
		"Bent, not picked.";
	}

	type "death_praise"
	{
		"Pretty shot, ", 0, ".";
	}

	type "death_gauntlet"
	{
		"The puppet has no green thumb.";
	}

	type "death_rail"
	{
		"Splashed from the hedge. I should have ducked.";
	}

	type "enemy_suicide"
	{
		0, " wilted all by themselves.";
	}

	type "hit_nokill"
	{
		"A near miss. The next bloom won't be.";
	}
}
