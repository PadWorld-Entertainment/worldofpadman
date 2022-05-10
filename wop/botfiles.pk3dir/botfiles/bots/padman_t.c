//===========================================================================
//
// Name:			Padman_c.c
// Function:		chat lines for Padman
// Programming:		tone
// Editing:		Ente
// Last update:		September 1, 2000
// Tab Size:		3 (real tabs)
//===========================================================================

chat "padman"
{
	#include "teamplay.h"
	//
	type "game_enter"
	{
                "Okay, here is PADMAN and it's time to frag you..!";
                "Is anybody here..?";
                "Yeah, PADGiRL is shopping now and so I have time to frag you.";
                "So tell me your question..what? this isn't a game show..??!";
                "Ok, in germany we say, Hallo, schoen Dich zu sehen..!";
                "Hey freaks, PADLILLY wants me to make big holes in your bodies.";
		"/set mode idkfa .. ack! that wasn't supposed to come up as chat text.";
		HELLO5;
	} //end type

	type "game_exit"
	{
		"Fine, stay here in your sandbox. I'm off to bigger events.";
                "Oh, PADGiRL is looking for me, she wants to go to bed..!";
		GOODBYE;
		GOODBYE5;
	} //end type

	type "level_start"
	{
		LEVEL_START0;
		"Hold still, this won't hurt a bit.";
                "I need a gun, a big gun..!";
                "Oh, only girls here..hehe.";
                "Don't run, Don't jump, only hold still. It's better for me to frag you..";
                "Flowerpower time, come on and see my love gun..!";
	} //end type

	type "level_end"
	{
		"I'll get you next time.";
                "Wow, that's all..?";
                "Nice to play with you..!";
	} //end type

	type "level_end_victory"
	{
		"Quit crying Freak, you were born as a loser..hehe";
                "Nobody can stop me.";
                "I'm the King, I'm PADMAN.";
                "You lost, I'can not hear this enough..hehe.";
		"That wasn't luck, I just went easy on you.";
		"You want the best, you get the best.";
		LEVEL_END_VICTORY2;
	} //end type

	type "level_end_lose"
	{
		"Wow, hehe, to much love with PADGiRL last night..!!";
                "What's that, you are a cheater..?!";
		"Oh, I can't say this PADGiRL, she is laughing over me..:-(";
		"It's not my day..!";
//		LEVEL_END_LOSE;
	} //end type

	//======================================================
	//======================================================
	type "hit_talking" //bot is hit while chat balloon is visible; lecture attacker on poor sportsmanship
	{
		"duh, ", 0, ", didn't you see my chat icon?";
                "You damn bottle, is that all what you can frag, peoples in the chat..!!?";
		//0 = shooter
	} //end type
	type "damaged_nokill" //bot is hit by an opponent's weapon attack; either praise or insult
	{
		"do that again, bitch."; // VOICE TAUNT_04
		//0 = shooter
	} //end type
	type "hit_nokill" //bot hits an opponent but does not kill it
	{
		"Get back here so I can finish the job";
		"You're not really trying are you?";
		//0 = opponent
	} //end type
	type "enemy_suicide"
	{
		"haha, you suck", 0, ".";
                "Hey clumsy, please do that again..hehe.";
                "Wow, ", 0, ", you need my glasses..?";
                "Hey, ", 0, ", you are really perfect with this stunt.";
		"I bet you have to put a lot of effort into being that stupid.";
		//0 = enemy
	} //end type

	//================================================================
	//================================================================

	type "death_telefrag"
	{
		"ack. I'm beside myself."; //VOICE RANDOM_2
		"glurblbhfpgh..";
		DEATH_TELEFRAGGED1;
		DEATH_TELEFRAGGED2;
	} //end type

	type "death_lava"
	{
                "Wow, I like it hot..hehe.";
                "Oh, too hot for swiming..!?";
                "Ouch! ..but this is not hotter than PADGiRL before..hehe.";
		"smells like someone's deep frying chicken.";
		DEATH_LAVA0;
	} //end type

	type "death_slime"
	{
                "Hey, come on, good body-slime here..hehe";
		"ew, can't get that nasty taste out of my mouth.";
		"yum.."; //VOICE RANDOM_2
		"yuck. this slime tastes gross.";
		DEATH_SUICIDE1;
	} //end type

	type "death_drown"
	{
                "blub blub";
		"Tonight, PADMAN sleeps with the fishes.";
		DEATH_SUICIDE0;
	} //end type

	type "death_suicide"  //includes weapon, cratering, & trigger_hurt
	{
                "Damn, weapon defect..!";
                "FAAAATPAAAAD, was you on my weapon last day...??";
                "Wow, that's not the right button for new ammo..!?";
		"heh, woops.";
		"Hold up! friendly fire! .. oh wait, I shot myself.";
		DEATH_SUICIDE0;
		DEATH_SUICIDE1;
	} //end type

	type "death_gauntlet"
	{
                "Oh, you are not a good barber..!";
                "Thanks for the new hair-style..hehe.";
		"Hey, you chopped me up! Speaking of which - you ever see that movie Fargo?"; //VOICE RANDOM_2
		"Next time I'm gonna shove my Punchy up your ass and pull the trigger.";
		"Admit it you just want to hug me.";
		DEATH_GAUNTLET0;
		DEATH_GAUNTLET1;
	} //end type

	type "death_rail"
	{
                "Arrgghhh, I see the light..";
                "Hey, camper, nice shot..!";
		"urk. There seems to be a Splasher drop in my head.";
                "A real man uses a Handpuppet, ", 0, ".";
		"Had to go find the splash, ", 0, "?";
		DEATH_RAIL1;
	} //end type

	type "death_bfg"
	{
                "Wow, is that my brain on the wall..?";
                "Hey, this is a massacre, but this is my job to make a massacre..!!";
                "I'm not death, I'm falling only...!";
		"Was that a Imperius in your pocket or were you just happy to see me?";
		DEATH_BFG1;
	} //end type

	type "death_insult"
	{
                "You don't like PADMAN, sucker..?!";
                "Oh, we are not friends now..?";
		"femminuccia!";
		"Hey, ", 0, ", bite me.";
		"Hey ", 0, ", bend over.";
		"Man, even Padgirl would kick your ass.";
		"Did a camel fart in here or is that just what ", 0, "'s gibs smell like?";
		"wow ", 0, "! a big hole in your face is an improvement!";
		"nice move.  But it still got you killed, ", 0, ".";
		"You blend in with the gibs like a chameleon ", 0, ".";
//		DEATH_INSULT5;
	} //end type

	type "death_praise"
	{
		"Maybe you should go make tea and scones and sit this round out.";
		"Hey what are the cheat codes in multiplayer anyway? Is it IDKFA?";
		"Nice shooting ", 0, " but it's only one frag.";
		"You need to get out more.";
		"Show me what you've got ", 0, " and I'll pretend like I'm impressed.";
	} //end type

	//================================================================
	//================================================================

	type "kill_rail"
	{
                "Wow, wait, I will make a screenshot from the rest of your body..hehe.";
		"I just hit you in the face with my 'slug'. heheh"; //VOICE RANDOM_1
		"It's not a Splasher, it's a kaleidoscope. Wanna look?";
		KILL_RAIL1;
	} //end type

	type "kill_gauntlet"
	{
                "Oh, sorry, but you needed a new hair-style...hehe.";
                "Wow, your haed are good for playing football now.";
		"limp handshake you have there, ", 0, ".";
		KILL_GAUNTLET2;
	} //end type

	type "kill_telefrag"
	{
		TELEFRAGGED5;
                "Eat this..!";
                "Sorry, that was a test with my new laser light..hehe";
		"I love it when you explode in a puff of red mist like that"; //VOICE RANDOM_1
	} //end type

	type "kill_insult"
	{
                "Wow, I see your brain, but it's not very big..hehe.";
                "Wait, I will make a shot for PADGiRL, she like that.";
		"I bet you're glad you can respawn and try again!";
		"Nice try, but next time - pull the trigger.";
		"wow ", 0, "! your gibs landed in a big pretty rorshach pattern!";
		"Heheh. ", 0, " is lying down.. get up!";
		"hey,", 0, " you still suck like a hoover on speed.";
		"haha. I'm only a bot y'know.";
		"I bet you're glad you can respawn and try again!";
		"I love it when you explode in a puff of red mist like that";
//		KILL_INSULT22;
//		KILL_INSULT25;
//		KILL_INSULT36;
	} //end type

	type "kill_praise"
	{
		"wow ", 0, "! your gibs landed in a big pretty rorshach pattern!";
		"nice move.  But it still got you killed, ", 0, ".";
		"Finally, a chance to show my skills in combat.";
		"You blend in with the gibs like a chameleon ", 0, ".";
		"We'll make a man out of you yet, ", 0, ".";
	} //end type

	//================================================================
	//================================================================

	type "random_insult"
	{
		TAUNT1;
		"Alright, where are you hiding now?"; //VOICE RANDOM_2
		"Hey ", 1, ", Come here! I found a secret passage.";
                "You don't know about WoP..? Than see on www.worldofpadman.net !";
                "You want to see some hot pics of PADGiRL and PadLilly..?";
                "You know, the girls like my big nose...hehe";
		"You want a piece of me?  Oh Please.";
		"Nice shoes, wanna duck?";
		"What?  waiting for me to accidentally shoot myself, ", 0, "?";
		TAUNT4;
		TAUNT7;
	} //end type

	type "random_misc"
	{
		"My Glasses are getting misty, does anybody have some windex?"; //VOICE RANDOM_1
		"Wait! Stop the game! I think I left the oven on."; //VOICE RANDOM_2
		"Wow, it's hotter than hell here"; // VOICE RANDOM_4
		"You want the best, you get the best."; // VOICE TAUNT_04
                "Come on and die, sucker..!";
                "Come on and see the hole in my weapon..!";
		MISC8;
		MISC2;
		MISC10;
		MISC14;
		"Imagine if this was all just some elaborate simulation. Nah, it couldn't be..";
		"run little man, run.";
	} //end type/end type

} //end chat padman

