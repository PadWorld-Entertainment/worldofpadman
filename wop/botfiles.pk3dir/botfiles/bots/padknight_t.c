chat "padknight"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"PadKnight takes the field. Fight clean, ", 1, ".";
		"A knight closes the distance. Remember that.";
	}

	type "game_exit"
	{
		"The watch is over. Sheathe it.";
	}

	type "level_start"
	{
		"Punchy for the duel. BubbleG before you reach me.";
	}

	type "level_end"
	{
		2, " fought well enough to lead. Honor to them.";
	}

	type "level_end_victory"
	{
		"The field is held.";
	}

	type "level_end_lose"
	{
		"You have the day, ", 2, ". I have the next one.";
	}

	type "kill_insult"
	{
		"Yield was an option.";
	}

	type "kill_praise"
	{
		"Well fought, ", 0, ".";
	}

	type "kill_gauntlet"
	{
		"Punchy is the sword. You met it.";
		"Up close, the puppet decides.";
	}

	type "kill_rail"
	{
		"A knight can splash, too. I prefer not to.";
	}

	type "death_insult"
	{
		"A fair hit. I am not done.";
	}

	type "death_praise"
	{
		"Good strike, ", 0, ". I salute it.";
	}

	type "death_gauntlet"
	{
		"Beaten at my own distance. I'll earn it back.";
	}

	type "death_rail"
	{
		"Splashed from afar. Come closer next time.";
	}

	type "enemy_suicide"
	{
		0, " fell on their own blade. A poor oath.";
	}

	type "hit_nokill"
	{
		"Your reach is short. Mine is Punchy.";
	}
}
