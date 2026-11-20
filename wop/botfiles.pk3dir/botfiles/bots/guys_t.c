chat "padman"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"Welcome to my home, clumsy.";
		"You have entered the PadWorld, ", 1, ". Let's rock.";
		0, " is in the building.";
	}

	type "game_exit"
	{
		"Padman is out. Try not to miss me.";
		"Enough for today. The PadWorld can wait.";
	}

	type "level_start"
	{
		"Time for boom-boom-betty kisses.";
		"Punchy is warmed up. Are you?";
	}

	type "level_end"
	{
		"Can we try that again, ", 2, "? Warm up is over.";
	}

	type "level_end_victory"
	{
		"That was my house. You just visited.";
	}

	type "level_end_lose"
	{
		"Have fun storming the PadWorld.";
		"Good luck, ", 2, ". Next time Betty does the talking.";
	}

	type "kill_insult"
	{
		"Try harder. The PadWorld is not a waiting room.";
		"*yawn* Even the lollies put up more fight.";
	}

	type "kill_praise"
	{
		"Nice try. Padman still lives here.";
	}

	type "kill_gauntlet"
	{
		"Say hello to my good friend Punchy.";
		"My Punchy says you taste like chicken.";
	}

	type "kill_rail"
	{
		"SPLASH... hehe. The Splasher likes you.";
	}

	type "death_insult"
	{
		"I'll get you next time.";
		"Enjoy it. The balloons are already on their way back.";
	}

	type "death_praise"
	{
		"Not bad. Do it again and I might learn your name.";
	}

	type "death_gauntlet"
	{
		"Admit it: you just wanted to hug Punchy.";
	}

	type "death_rail"
	{
		"Right in the splash. Rude, and accurate.";
	}

	type "enemy_suicide"
	{
		"Hey, ", 0, ", that was my frag.";
	}

	type "hit_nokill"
	{
		"That was a warning shot. The next one is Betty.";
	}
}
