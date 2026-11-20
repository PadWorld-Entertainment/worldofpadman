chat "padgirl"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"Oh, hey, ", 1, ", don't leave. I'll be gentle. Maybe.";
		"In Germany we say 'Hallo, schoen Dich zu sehen'.";
	}

	type "game_exit"
	{
		"Oh, so late already. I have to put PADMAN to bed.";
		"That could have been more fun.";
	}

	type "level_start"
	{
		"Flower power time. Let's spread some love.";
		"Don't run. It's easier for me to splash you.";
	}

	type "level_end"
	{
		"It's been nice playing with you.";
	}

	type "level_end_victory"
	{
		"I'm the queen. Say it one more time.";
	}

	type "level_end_lose"
	{
		"OMG, I just lost to ", 2, ". What a shame.";
	}

	type "kill_insult"
	{
		"Wow, I can see your brain, and it is not very big.";
		"Nice try. Next time, pull the trigger.";
	}

	type "kill_praise"
	{
		"Nice move. It still got you killed, ", 0, ".";
	}

	type "kill_gauntlet"
	{
		"Sorry. You needed a new hairstyle.";
	}

	type "kill_rail"
	{
		"You might want a towel. The Splasher was thirsty.";
	}

	type "death_insult"
	{
		"What, we're not friends anymore?";
		"Hey, ", 0, ", bite me.";
	}

	type "death_praise"
	{
		"Nice shooting, ", 0, ". It is still only one frag.";
	}

	type "death_gauntlet"
	{
		"Thanks, ", 0, ". I needed a fresh haircut.";
	}

	type "death_rail"
	{
		"Argh. Splashed in the face. My mascara is ruined.";
	}

	type "enemy_suicide"
	{
		"Haha, ", 0, ", you need more practice. Or my glasses.";
	}

	type "hit_nokill"
	{
		"Get back here so I can finish the job.";
	}
}
