chat "paddybell"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"PaddyBell! Oh, hello, ", 1, ". Please don't be scared of the guns.";
	}

	type "game_exit"
	{
		"That was fun! I'm late for... something with bells.";
	}

	type "level_start"
	{
		"I'll try to be nice. The weapons might not.";
	}

	type "level_end"
	{
		2, " did so well! I'm only a little jealous.";
	}

	type "level_end_victory"
	{
		"I won? Oh! I mean, yay!";
	}

	type "level_end_lose"
	{
		"You were great, ", 2, ". I'll practice the jumping.";
	}

	type "kill_insult"
	{
		"Sorry! You were in the way. Still sorry.";
	}

	type "kill_praise"
	{
		"You almost had me, ", 0, ". It was very close and very loud.";
	}

	type "kill_gauntlet"
	{
		"Punchy says hello. I said please first.";
	}

	type "kill_rail"
	{
		"Oh, the Splasher. I hope that wasn't too much.";
	}

	type "death_insult"
	{
		"Ow! Okay. Okay. I'm fine. I'm not fine.";
	}

	type "death_praise"
	{
		"Wow, ", 0, ", that was really good. I'm not mad.";
	}

	type "death_gauntlet"
	{
		"The puppet got me. I thought we were friends.";
	}

	type "death_rail"
	{
		"Splashed! My bow is ruined and so am I.";
	}

	type "enemy_suicide"
	{
		"Oh no, ", 0, ", are you okay? No. Right. Sorry.";
	}

	type "hit_nokill"
	{
		"Eep! That was close. Please miss more.";
	}
}
