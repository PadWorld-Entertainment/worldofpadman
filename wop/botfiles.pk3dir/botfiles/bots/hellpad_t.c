chat "hellpad"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"HellPad. Bring a fire extinguisher, ", 1, ".";
		"If you're here, you're already late.";
	}

	type "game_exit"
	{
		"I'm bored of winning. Door's that way.";
	}

	type "level_start"
	{
		"Betty's hot. So am I. You won't be.";
	}

	type "level_end"
	{
		2, " thinks this was close. Cute.";
	}

	type "level_end_victory"
	{
		"Burn the scoreboard. I already know.";
	}

	type "level_end_lose"
	{
		"Tch. ", 2, " gets one. I get the rematch.";
	}

	type "kill_insult"
	{
		"Down. Stay a minute.";
		"That all you brought?";
	}

	type "kill_praise"
	{
		"You made me use the good rocket, ", 0, ".";
	}

	type "kill_gauntlet"
	{
		"Punchy likes them scared.";
	}

	type "kill_rail"
	{
		"One splash. Don't say I never did anything precise.";
	}

	type "death_insult"
	{
		"I'll be right back. You won't like it.";
	}

	type "death_praise"
	{
		"Heh. ", 0, " has teeth.";
	}

	type "death_gauntlet"
	{
		"A puppet. I'll remember that.";
	}

	type "death_rail"
	{
		"Sniped from a hole. Coward's firework.";
	}

	type "enemy_suicide"
	{
		0, " did my job for me. Lazy, but I'll take it.";
	}

	type "hit_nokill"
	{
		"You flinched. I don't.";
	}
}
