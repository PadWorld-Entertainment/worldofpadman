chat "padclassic"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"PadClassic. Rockets, Splasher, no speeches.";
		"Old rules, ", 1, ". The Betty still wins fights.";
	}

	type "game_exit"
	{
		"That's the round. Same time next map.";
	}

	type "level_start"
	{
		"Jump, shoot, don't stand in the rocket.";
	}

	type "level_end"
	{
		2, " played the new way. I played the one that works.";
	}

	type "level_end_victory"
	{
		"Classic for a reason.";
	}

	type "level_end_lose"
	{
		"You got me, ", 2, ". The loadout stays.";
	}

	type "kill_insult"
	{
		"Seen it. Dodged it. Fired second.";
	}

	type "kill_praise"
	{
		"Good strafe, ", 0, ". The rocket was better.";
	}

	type "kill_gauntlet"
	{
		"Punchy. Older than the argument.";
	}

	type "kill_rail"
	{
		"Splasher. Some problems are a straight line.";
	}

	type "death_insult"
	{
		"I'll take the next rocket.";
	}

	type "death_praise"
	{
		"Fair frag, ", 0, ".";
	}

	type "death_gauntlet"
	{
		"Walked into Punchy. That's on me.";
	}

	type "death_rail"
	{
		"Crosshair discipline. I remember when I had it.";
	}

	type "enemy_suicide"
	{
		0, " rocket-jumped into the floor. A classic, actually.";
	}

	type "hit_nokill"
	{
		"Whiff. Reload your habits.";
	}
}
