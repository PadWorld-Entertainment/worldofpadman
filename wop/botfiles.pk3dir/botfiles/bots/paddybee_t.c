chat "paddybee"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"PaddyBee, in and out. Nipper's already singing, ", 1, ".";
	}

	type "game_exit"
	{
		"Gotta buzz. Flowers don't spray themselves.";
	}

	type "level_start"
	{
		"Fast feet, short bursts. Try to keep up.";
	}

	type "level_end"
	{
		2, " was quick. I was quicker where it counted. Usually.";
	}

	type "level_end_victory"
	{
		"Zip. Done. Nectar.";
	}

	type "level_end_lose"
	{
		"Stung by ", 2, ". I'll be back before you blink.";
	}

	type "kill_insult"
	{
		"Too slow to swat a bee.";
	}

	type "kill_praise"
	{
		"Nice hop, ", 0, ". I hop faster.";
	}

	type "kill_gauntlet"
	{
		"A little sting from Punchy.";
	}

	type "kill_rail"
	{
		"One splash. I prefer a swarm, but okay.";
	}

	type "death_insult"
	{
		"Clipped a wing. Still got five.";
	}

	type "death_praise"
	{
		"Sharp, ", 0, ". I'll circle wider.";
	}

	type "death_gauntlet"
	{
		"Swatted. Rude.";
	}

	type "death_rail"
	{
		"A splash from across the garden. Show-off.";
	}

	type "enemy_suicide"
	{
		0, " flew into the glass.";
	}

	type "hit_nokill"
	{
		"Zig. You zagged. Too late.";
	}
}
