chat "beachpad"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"BeachPad. Shoes off, balloons out.";
		"Easy, ", 1, ". The tide does the work.";
	}

	type "game_exit"
	{
		"Tide's going out. So am I.";
	}

	type "level_start"
	{
		"Float a balloon. I'll be on the floater.";
	}

	type "level_end"
	{
		2, " surfed the scoreboard. Nice wave.";
	}

	type "level_end_victory"
	{
		"I win. The ocean remains undefeated.";
	}

	type "level_end_lose"
	{
		"Wiped out by ", 2, ". The water's still warm.";
	}

	type "kill_insult"
	{
		"Should've stayed on the towel, ", 0, ".";
	}

	type "kill_praise"
	{
		"Nice cutback. The balloon still landed.";
	}

	type "kill_gauntlet"
	{
		"Punchy says the water's fine.";
	}

	type "kill_rail"
	{
		"A splash. Literally. Sorry. Not sorry.";
	}

	type "death_insult"
	{
		"Salt in the wound. I'll rinse off and come back.";
	}

	type "death_praise"
	{
		"Clean ride, ", 0, ".";
	}

	type "death_gauntlet"
	{
		"Boarded by a puppet. The lifeguard saw nothing.";
	}

	type "death_rail"
	{
		"Sniped on the beach. So much for the view.";
	}

	type "enemy_suicide"
	{
		0, " wiped out alone. I didn't even paddle.";
	}

	type "hit_nokill"
	{
		"Was that a splash or a wave?";
	}
}
