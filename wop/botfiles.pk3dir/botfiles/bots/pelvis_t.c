chat "pelvis"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"Pelvis in the building. The Boaster's already dancing.";
		"Make room, ", 1, ". Hips, then frags.";
	}

	type "game_exit"
	{
		"Show's over. Tip your Boaster.";
	}

	type "level_start"
	{
		"If you can see me, you're already in the lightning.";
	}

	type "level_end"
	{
		2, " took the spotlight. I take the next verse.";
	}

	type "level_end_victory"
	{
		"And that, folks, is a closer.";
	}

	type "level_end_lose"
	{
		"Tough crowd. ", 2, " gets the encore.";
	}

	type "kill_insult"
	{
		"You missed the beat, ", 0, ".";
	}

	type "kill_praise"
	{
		"Nice move. Wrong dance floor.";
	}

	type "kill_gauntlet"
	{
		"Punchy does percussion.";
	}

	type "kill_rail"
	{
		"One splash, right on the one-count.";
	}

	type "death_insult"
	{
		"I slipped. The band plays on.";
	}

	type "death_praise"
	{
		"You got the timing, ", 0, ". I'll steal it.";
	}

	type "death_gauntlet"
	{
		"A puppet stole the spotlight. Unheard of.";
	}

	type "death_rail"
	{
		"Splashed mid-spin. The pose was excellent.";
	}

	type "enemy_suicide"
	{
		0, " left the stage without a cue.";
	}

	type "hit_nokill"
	{
		"Off tempo. Again, with feeling.";
	}
}
