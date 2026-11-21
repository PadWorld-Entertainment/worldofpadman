chat "padra"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"Pad-RA sees you, ", 1, ". The Splasher is already awake.";
		"Quiet. The eye does not announce itself twice.";
	}

	type "game_exit"
	{
		"The watch ends. Walk carefully.";
	}

	type "level_start"
	{
		"Stand in the open if you are tired of living.";
	}

	type "level_end"
	{
		2, " led. I was busy aiming.";
	}

	type "level_end_victory"
	{
		"As written. As shot.";
	}

	type "level_end_lose"
	{
		"A miss is a lesson. ", 2, " was the louder one today.";
	}

	type "kill_insult"
	{
		"You crossed the sight. That was the whole mistake.";
	}

	type "kill_praise"
	{
		"You moved well, ", 0, ". Once.";
	}

	type "kill_gauntlet"
	{
		"Even a priest keeps a fist for rude guests.";
	}

	type "kill_rail"
	{
		"The Splasher spoke. You listened.";
	}

	type "death_insult"
	{
		"A lucky stone. Not a plan.";
	}

	type "death_praise"
	{
		"Clean shot, ", 0, ". The eye remembers.";
	}

	type "death_gauntlet"
	{
		"Too close. My fault.";
	}

	type "death_rail"
	{
		"One splash. Efficient. I respect it.";
	}

	type "enemy_suicide"
	{
		0, " removed themselves. Convenient.";
	}

	type "hit_nokill"
	{
		"You had the angle. You wasted it.";
	}
}
