//===========================================================================
//
// Name:			piratepad_c.c
// Function:		chat lines for Visor
// Programmer:		Mr. Elusive
// Scripter:		Paul Jaquays
// Last update:		Oct 11, 1999
// Tab Size:		3 (real tabs)
//===========================================================================

chat "visor"
{
	#include "teamplay.h"
	//
	type "game_enter"
	{
		HELLO3;
		HELLO4;
		HELLO6;
		"Heh. Fresh Meat.";
		"Time to Rock 'n Roll.";
		"Oh yeah! ", 4, " is where ~Visor lays down the law!";
	} //end type

	type "game_exit"
	{
		GOODBYE;
		GOODBYE2;
		"You know I'll be back.";
		"I'm history. Later kids.";
	} //end type

	type "level_start"
	{
		LEVEL_START2;
		LEVEL_START3;
		"This place rocks!";
		4, " rocks!";
		"I'm ready to win.";
		"Who needs a lesson today?";
		"Alright. Make me look good.";
	} //end type

	type "level_end"
	{
		LEVEL_END2;
		"Perhaps luck won't favor you next time, eh?";
		"I won't make the same mistakes next time.";
		"Only winning matters here.";
	} //end type

	type "level_end_victory"
	{
		"I deserve this!";
		"This is what it's all about, kid!";
		"This is the reason you ", fighter, "s are here. To make me look good.";
		LEVEL_END_VICTORY0;
		LEVEL_END_VICTORY2;
	} //end type

	type "level_end_lose"
	{
		"Grrrr.";
		"Luck. Pure luck.";
		"Next time, your butts are mine.";
		"Next time, your butt is mine, ", 2, "!";
		LEVEL_END_LOSE1;
	} //end type

	//======================================================
	//======================================================
	type "hit_talking" //bot is hit while chat balloon is visible; lecture attacker on poor sportsmanship
	{
		"There's no hole deep enough for you to hide in, ", 0, ".";
		0, ", I'm gonna ram my gauntlet so far down your skinny throat, I'll untie your shoes.";
		"That was cheap, and you know it, ", 0, ".";
		"You saying I talk to much, ", 0, "?";
		//0 = shooter
	} //end type

	type "hit_nodeath" //bot is hit by an opponent's weapon attack; either praise or insult
	{
		"You actually hit me, ", 0, ". I'm impressed, kid.";
		"So kid, you going to make a habit of dinking ol' ~Visor?";
		"Another scar to remember you by, ", 0, ".";
		"I've stopped counting the scars.";
		"That hurt, kid. But not enough to matter.";
		//0 = shooter
	} //end type

	type "hit_nokill" //bot hits an opponent but does not kill it
	{
		"You survived? It's time to show you my latest move, ", 0, ".";
		"I win the simple way, by out-fragging the competition.";
		"I hope you heal fast kid, because there's more where that came from.";
		"That'll sting in the morning.";
		"That's it. Run for the health. I know where to find you.";
		//0 = opponent
	} //end type

	//================================================================
	//================================================================

	type "death_telefrag"
	{
		DEATH_TELEFRAGGED0;
		DEATH_TELEFRAGGED1;
		"There's only room for ~one of us behind the mask.";
	} //end type

	type "death_lava"
	{
		"Lousy stuff.";
		"This stuff makes me mad.";
		"Whose idea was this?";
		DEATH_SUICIDE2;
		DEATH_LAVA1;
	} //end type

	type "death_slime"
	{
		"I never get used to that stuff.";
		"Get that slime in your cybronics and they're never the same again.";
		DEATH_SUICIDE0;
	} //end type

	type "death_drown"
	{
		"I knew I should have packed a snorkel for this trip.";
		"Wish this ~visor was fitted with a scuba hook up.";
		DEATH_DROWN0;
	} //end type

	type "death_suicide"
	{
		"Don't laugh kid, you've probably done this a lot.";
		"I've gone and made a mess of myself again.";
		DEATH_SUICIDE0;
		DEATH_SUICIDE2;
	} //end type 

	type "death_gauntlet"
	{
		"Will you stop touching me?";
		"Do that again and you are toast.";
		DEATH_GAUNTLET0;
		DEATH_GAUNTLET1;
	} //end type

	type "death_rail"
	{
		"Heh. Nice shot kid.";
		"I hate campers. I really do.";
		"Did ~Xian show you how to do that?";
		DEATH_RAIL1;
	} //end type 

	type "death_bfg"
	{
		"Meteor Shower from Hell!";
		DEATH_BFG0;
		DEATH_BFG1;
	} //end type

	type "death_insult"
	{
		DEATH_INSULT5;
		"Got any ammo left, punk? You're gonna need it.";
		"Did you enjoy that?";
		"I'll bet you enjoyed that more than I did, ", 0, ".";
		KILL_INSULT14;
	} //end type

	type "death_praise"
	{
		D_PRAISE5;
		PRAISE5;
		"Oh, yeah. You had me good there kid.";
		"Not bad kid.";
		"Work on your follow through, kid.";
		"You copied that move from me, didn't you, ", 0, "?";

	} //end type

	//================================================================
	//================================================================

	type "kill_rail" 
	{
		"Yessssss!!";
		"Gotcha!";
		"You're it!";
		"~Railguns rule my universe!";
		KILL_RAIL0;
	} //end type

	type "kill_gauntlet"
	{
		"Thanks, ", 0, ". I love doing that!";
		"Humiliation is the name of the game, kid.";
		KILL_GAUNTLET0;
		KILL_GAUNTLET1;
	} //end type

	type "kill_telefrag"
	{
		"Stand aside, kid. Whoops. Too late.";
		"Nothing but memories left there.";
		TELEFRAGGED0;
		TELEFRAGGED2;	
	} //end type

	type "kill_insult"
	{
		KILL_INSULT7;
		KILL_INSULT10;
		KILL_INSULT16;
		"Pick up your toys and go home kid.";
		"I was told you were competition, ", 0, ". Somebody got that wrong.";
	} //end type

	type "kill_praise"
	{
		"Not bad, kid. Most don't last that long.";
		"Almost didn't get you there, kid.";
		PRAISE4;
	} //end type

	//================================================================
	//================================================================

	type "random_insult"
	{
		"You will learn to fear me.";
		"I will be your personal nightmare, kid.";
		TAUNT7;
		TAUNT8;
	} //end type

	type "random_misc"
	{
		"Victory will be mine!";
		"You can't win, ", 0, ". It's not in you to be a winner.";
		"This is all I know. No ~one can take it from me.";
		"They come by the millions, but I turn them all back.";
		"Nothing fancy here, just solid skill.";
		"I would be proud to be to your father, ", 0, ".";
		MISC8;
		MISC9;
		MISC11;
	} //end type

} //end chat visor
