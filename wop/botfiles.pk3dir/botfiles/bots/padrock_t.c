chat "padrock"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"PADROCK! If it moves, Betty gets it.";
		"Turn it up, ", 1, ". I can't hear the explosions.";
	}

	type "game_exit"
	{
		"I'm out. The amps were too quiet anyway.";
	}

	type "level_start"
	{
		"Rockets first. Questions never.";
	}

	type "level_end"
	{
		"Encore? ", 2, " thinks they headline.";
	}

	type "level_end_victory"
	{
		"That was the show. You were the fireworks.";
	}

	type "level_end_lose"
	{
		"Boo. ", 2, " stole the last chorus.";
	}

	type "kill_insult"
	{
		"Too slow for this riff, ", 0, ".";
	}

	type "kill_praise"
	{
		"Ha! You almost crowd-surfed out of that one.";
	}

	type "kill_gauntlet"
	{
		"Punchy does backup vocals.";
	}

	type "kill_rail"
	{
		"Splash solo. Short, loud, done.";
	}

	type "death_insult"
	{
		"Feedback. I'll be back louder.";
	}

	type "death_praise"
	{
		"Nasty hit, ", 0, ". Play it again.";
	}

	type "death_gauntlet"
	{
		"A puppet? At my show? Bold.";
	}

	type "death_rail"
	{
		"Right through the speakers. Cheap shot, great timing.";
	}

	type "enemy_suicide"
	{
		0, " stage-dived with no stage.";
	}

	type "hit_nokill"
	{
		"You missed the drop. I won't.";
	}
}
