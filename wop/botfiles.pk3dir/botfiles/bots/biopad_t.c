chat "biopad"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"BioPad online. BubbleG is the whole experiment.";
		"Don't touch the glowing one, ", 1, ".";
	}

	type "game_exit"
	{
		"Sample collected. Lab time.";
	}

	type "level_start"
	{
		"Hypothesis: you dodge worse under plasma.";
	}

	type "level_end"
	{
		"Data says ", 2, " got lucky. I'll rerun it.";
	}

	type "level_end_victory"
	{
		"Result: significant. Also funny.";
	}

	type "level_end_lose"
	{
		"Failed trial. ", 2, " contaminated the sample.";
	}

	type "kill_insult"
	{
		"The control group sends its regrets.";
	}

	type "kill_praise"
	{
		"Fascinating reaction, ", 0, ". Fatal, but fascinating.";
	}

	type "kill_gauntlet"
	{
		"Manual application. Unscientific. Effective.";
	}

	type "kill_rail"
	{
		"A linear solution. I prefer bubbles, but fine.";
	}

	type "death_insult"
	{
		"Note to self: dodge.";
	}

	type "death_praise"
	{
		"Clean variables, ", 0, ". You win this trial.";
	}

	type "death_gauntlet"
	{
		"The puppet was not in the risk assessment.";
	}

	type "death_rail"
	{
		"Splashed. My goggles did nothing.";
	}

	type "enemy_suicide"
	{
		0, " became their own failed experiment.";
	}

	type "hit_nokill"
	{
		"Inconclusive. I'll increase the dose.";
	}
}
