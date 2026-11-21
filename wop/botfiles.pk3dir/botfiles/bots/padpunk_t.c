chat "padpunk"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"PadPunk. Rules are a suggestion, ", 1, ".";
		"If it's bolted down I already stole the Boaster.";
	}

	type "game_exit"
	{
		"This scene's dead. I'm out.";
	}

	type "level_start"
	{
		"No plan. Boaster's plan enough.";
	}

	type "level_end"
	{
		2, " played it straight and somehow won. Gross.";
	}

	type "level_end_victory"
	{
		"Anarchy, one. Everybody else, zero.";
	}

	type "level_end_lose"
	{
		"Whatever. ", 2, " can have the plastic trophy.";
	}

	type "kill_insult"
	{
		"Skill issue, ", 0, ".";
	}

	type "kill_praise"
	{
		"You almost started a mosh pit. Then you died.";
	}

	type "kill_gauntlet"
	{
		"Punchy in the pit. No stage diving.";
	}

	type "kill_rail"
	{
		"Splasher's for poseurs. Worked, though.";
	}

	type "death_insult"
	{
		"Cheap. I'll tag your respawn.";
	}

	type "death_praise"
	{
		"Okay, ", 0, ", that was nasty. Respect.";
	}

	type "death_gauntlet"
	{
		"A puppet started a fight. I respect the commitment.";
	}

	type "death_rail"
	{
		"Camping with a Splasher. How original.";
	}

	type "enemy_suicide"
	{
		0, " yeeted themselves. Punk, accidentally.";
	}

	type "hit_nokill"
	{
		"Missed. Try aiming with the anger.";
	}
}
