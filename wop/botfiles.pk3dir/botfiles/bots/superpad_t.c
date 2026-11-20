chat "superpad"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"SuperPad has arrived. Evil, consider yourself scheduled.";
		"Stand back, ", 1, ". Imperius needs a runway.";
	}

	type "game_exit"
	{
		"The city is safe enough. I'm taking a lap.";
	}

	type "level_start"
	{
		"This looks like a job for a very large gun.";
	}

	type "level_end"
	{
		"Citizens, ", 2, " gets the headline. I get the next one.";
	}

	type "level_end_victory"
	{
		"Another day saved. You're welcome.";
	}

	type "level_end_lose"
	{
		"Even heroes eat a loss. ", 2, ", enjoy it.";
	}

	type "kill_insult"
	{
		"Justice is loud and bright.";
	}

	type "kill_praise"
	{
		"You fought bravely, ", 0, ". The ending was still mine.";
	}

	type "kill_gauntlet"
	{
		"Sometimes the right tool is Punchy.";
	}

	type "kill_rail"
	{
		"A single splash, for truth and whatever.";
	}

	type "death_insult"
	{
		"I meant to do the dramatic kneel.";
	}

	type "death_praise"
	{
		"Worthy hit, ", 0, ". The cape will be avenged.";
	}

	type "death_gauntlet"
	{
		"A sidekick puppet. Humbling.";
	}

	type "death_rail"
	{
		"Sniped. My cape was too visible.";
	}

	type "enemy_suicide"
	{
		0, " surrendered to gravity.";
	}

	type "hit_nokill"
	{
		"Is that all the villainy you've got?";
	}
}
