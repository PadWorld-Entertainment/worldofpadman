chat "padking"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"PadKing arrives. Kneel, or don't. The Splasher doesn't mind.";
		"The court is open, ", 1, ".";
	}

	type "game_exit"
	{
		"We are bored of this kingdom.";
	}

	type "level_start"
	{
		"A king does not chase. He is aimed at.";
	}

	type "level_end"
	{
		2, " may keep the applause. The crown stays.";
	}

	type "level_end_victory"
	{
		"As decreed.";
	}

	type "level_end_lose"
	{
		"A temporary regent, ", 2, ". Don't redecorate.";
	}

	type "kill_insult"
	{
		"The crown does not negotiate with ", 0, ".";
	}

	type "kill_praise"
	{
		"A worthy subject. Briefly.";
	}

	type "kill_gauntlet"
	{
		"Even royalty keeps Punchy for rude audiences.";
	}

	type "kill_rail"
	{
		"The Splasher is the royal seal.";
	}

	type "death_insult"
	{
		"Treason. We shall return.";
	}

	type "death_praise"
	{
		"Well struck, ", 0, ". The court noticed.";
	}

	type "death_gauntlet"
	{
		"A puppet laid hands on the king. Outrageous.";
	}

	type "death_rail"
	{
		"Splashed by a pretender. Noted.";
	}

	type "enemy_suicide"
	{
		0, " abdicated, messily.";
	}

	type "hit_nokill"
	{
		"You had an audience. You wasted it.";
	}
}
