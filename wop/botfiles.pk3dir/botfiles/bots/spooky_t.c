chat "spooky"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"Spooky. Boo is a tactic, ", 1, ".";
	}

	type "game_exit"
	{
		"Lights on. I'm off.";
	}

	type "level_start"
	{
		"Balloons in the dark. Don't follow the string.";
	}

	type "level_end"
	{
		2, " wasn't scared. I'm offended.";
	}

	type "level_end_victory"
	{
		"The house wins. The house is haunted.";
	}

	type "level_end_lose"
	{
		2, " turned the lights on. Rude.";
	}

	type "kill_insult"
	{
		"Scared you into the balloon.";
	}

	type "kill_praise"
	{
		"You didn't scream, ", 0, ". I'm almost impressed.";
	}

	type "kill_gauntlet"
	{
		"Punchy says boo.";
	}

	type "kill_rail"
	{
		"A splash from the dark. Classic haunting.";
	}

	type "death_insult"
	{
		"You can't kill a jump scare. You can delay it.";
	}

	type "death_praise"
	{
		"Ooh, ", 0, " knows where the ghosts stand.";
	}

	type "death_gauntlet"
	{
		"Grabbed by a puppet. My own trick. Humiliating.";
	}

	type "death_rail"
	{
		"Lit up. So much for cover of dark.";
	}

	type "enemy_suicide"
	{
		0, " scared themselves off the ledge.";
	}

	type "hit_nokill"
	{
		"Missed the sheet, hit the air.";
	}
}
