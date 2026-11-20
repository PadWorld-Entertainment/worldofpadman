chat "stonepad"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"...StonePad.";
		"I walk. You run. We meet at the Pumper.";
	}

	type "game_exit"
	{
		"Heavy steps. Going home.";
	}

	type "level_start"
	{
		"Come closer. My shotgun is patient.";
	}

	type "level_end"
	{
		2, " was faster. I was still here.";
	}

	type "level_end_victory"
	{
		"Slow win. Still a win.";
	}

	type "level_end_lose"
	{
		"Hnh. ", 2, " can have the noise.";
	}

	type "kill_insult"
	{
		"Told you. Closer.";
	}

	type "kill_praise"
	{
		"You moved. Didn't help.";
	}

	type "kill_gauntlet"
	{
		"Didn't even rack the Pumper.";
	}

	type "kill_rail"
	{
		"Splash. You stopped moving. Mistake.";
	}

	type "death_insult"
	{
		"...I'll get up.";
	}

	type "death_praise"
	{
		"Hard hit, ", 0, ".";
	}

	type "death_gauntlet"
	{
		"Puppet. Hnh.";
	}

	type "death_rail"
	{
		"Long gun. Figures.";
	}

	type "enemy_suicide"
	{
		0, " fell over. Less work for me.";
	}

	type "hit_nokill"
	{
		"Tickles.";
	}
}
