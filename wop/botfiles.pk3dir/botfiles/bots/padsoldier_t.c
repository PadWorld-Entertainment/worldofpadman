chat "padsoldier"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"PadSoldier reporting. Nipper loaded.";
		"Hold your lane, ", 1, ".";
	}

	type "game_exit"
	{
		"Patrol over. Dismissed.";
	}

	type "level_start"
	{
		"Short bursts. Don't waste a cell.";
	}

	type "level_end"
	{
		"Mission complete enough. ", 2, " took the flag of bragging.";
	}

	type "level_end_victory"
	{
		"Area secured.";
	}

	type "level_end_lose"
	{
		"We regroup. ", 2, " had the better ground.";
	}

	type "kill_insult"
	{
		"Target down. Next.";
	}

	type "kill_praise"
	{
		"You almost broke contact, ", 0, ". Almost.";
	}

	type "kill_gauntlet"
	{
		"Close quarters. Punchy is standard issue.";
	}

	type "kill_rail"
	{
		"One splash, center mass.";
	}

	type "death_insult"
	{
		"Hit. Not finished.";
	}

	type "death_praise"
	{
		"Good burst, ", 0, ". I'll remember the angle.";
	}

	type "death_gauntlet"
	{
		"I let you inside arm's reach. Noted.";
	}

	type "death_rail"
	{
		"Splasher. I was in the open. My error.";
	}

	type "enemy_suicide"
	{
		0, " is a casualty of their own feet.";
	}

	type "hit_nokill"
	{
		"Suppressing fire only works if it suppresses.";
	}
}
