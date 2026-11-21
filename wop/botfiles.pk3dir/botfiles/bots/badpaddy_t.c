chat "badpaddy"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"BadPaddy. The nice bell stayed home, ", 1, ".";
		"Smile. Betty hates a frown.";
	}

	type "game_exit"
	{
		"I'm done being the villain. For ten minutes.";
	}

	type "level_start"
	{
		"If it sparkles, it's a rocket. If it's you, it's a target.";
	}

	type "level_end"
	{
		2, " won. I'll be unbearable about the rematch.";
	}

	type "level_end_victory"
	{
		"Sugar on the outside. This on the inside.";
	}

	type "level_end_lose"
	{
		"Ugh, ", 2, ". Don't let it go to your head. It won't fit.";
	}

	type "kill_insult"
	{
		"Too sweet to live, ", 0, ".";
	}

	type "kill_praise"
	{
		"You had a mean streak. I had a bigger one.";
	}

	type "kill_gauntlet"
	{
		"Punchy doesn't do nice.";
	}

	type "kill_rail"
	{
		"Splashed. I even made it pretty.";
	}

	type "death_insult"
	{
		"This changes nothing. I hold grudges in bulk.";
	}

	type "death_praise"
	{
		"Mean shot, ", 0, ". I almost like you.";
	}

	type "death_gauntlet"
	{
		"A puppet? I'll pull your string later.";
	}

	type "death_rail"
	{
		"Sniped. Cowardice with good aim.";
	}

	type "enemy_suicide"
	{
		0, " quit before I could be cruel. Inconsiderate.";
	}

	type "hit_nokill"
	{
		"That was weak. I'm embarrassed for you.";
	}
}
