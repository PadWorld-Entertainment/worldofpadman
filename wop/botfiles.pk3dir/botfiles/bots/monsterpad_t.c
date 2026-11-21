chat "monsterpad"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"MonsterPad hungry.";
		"Come close, ", 1, ". Punchy is lonely.";
	}

	type "game_exit"
	{
		"Belly full. Going back under the map.";
	}

	type "level_start"
	{
		"Run and I still catch. Stand and it's faster.";
	}

	type "level_end"
	{
		2, " is loud. Monster is still hungry.";
	}

	type "level_end_victory"
	{
		"Monster win. Map scared.";
	}

	type "level_end_lose"
	{
		"Grr. ", 2, " got lucky teeth.";
	}

	type "kill_insult"
	{
		"Too easy. Chew louder next time.";
	}

	type "kill_praise"
	{
		"You bite back, ", 0, ". Still eaten.";
	}

	type "kill_gauntlet"
	{
		"Punchy is the favorite tooth.";
	}

	type "kill_rail"
	{
		"Splash in the eye. You fell over. Good.";
	}

	type "death_insult"
	{
		"Monster get back up. Always.";
	}

	type "death_praise"
	{
		"Ow. ", 0, " hits like a bigger monster.";
	}

	type "death_gauntlet"
	{
		"Puppet bite. Rude puppet.";
	}

	type "death_rail"
	{
		"Bright line. Monster no like.";
	}

	type "enemy_suicide"
	{
		0, " fell in a hole. Monster laughs.";
	}

	type "hit_nokill"
	{
		"That not a bite. This is a bite.";
	}
}
