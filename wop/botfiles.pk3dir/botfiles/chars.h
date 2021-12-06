//========================================================
//========================================================
//name of character
#define CHARACTERISTIC_NAME							0	//string
//gender of the bot
#define CHARACTERISTIC_GENDER						1	//string ("male", "female", "it")
//attack skill
// >  0.0 && <  0.2 = don't move
// >  0.3 && <  1.0 = aim at enemy during retreat
// >  0.0 && <  0.4 = only move forward/backward
// >= 0.4 && <  1.0 = circle strafing
// >  0.7 && <  1.0 = random strafe direction change
#define CHARACTERISTIC_ATTACK_SKILL					2	//float [0, 1]
//weapon weight file
#define CHARACTERISTIC_WEAPONWEIGHTS				3	//string
//view angle difference to angle change factor
#define CHARACTERISTIC_VIEW_FACTOR					4	//float <0, 1]
//maximum view angle change
#define CHARACTERISTIC_VIEW_MAXCHANGE				5	//float [1, 360]
//reaction time in seconds--the lower the number the better
#define CHARACTERISTIC_REACTIONTIME					6	//float [0, 5]
//accuracy when aiming
#define CHARACTERISTIC_AIM_ACCURACY					7	//float [0, 1]
//weapon specific aim accuracy
#define CHARACTERISTIC_AIM_ACCURACY_NIPPER		8
#define CHARACTERISTIC_AIM_ACCURACY_PUMPER		9
#define CHARACTERISTIC_AIM_ACCURACY_BETTY		10
#define CHARACTERISTIC_AIM_ACCURACY_BALLOONY	11
#define CHARACTERISTIC_AIM_ACCURACY_BOASTER		12
#define CHARACTERISTIC_AIM_ACCURACY_BUBBLEG		13
#define CHARACTERISTIC_AIM_ACCURACY_SPLASHER	14
#define CHARACTERISTIC_AIM_ACCURACY_IMPERIUS	15
//skill when aiming
// >  0.0 && <  0.9 = aim is affected by enemy movement
// >  0.4 && <= 0.8 = enemy linear leading
// >  0.8 && <= 1.0 = enemy exact movement leading
// >  0.5 && <= 1.0 = prediction shots when enemy is not visible
// >  0.6 && <= 1.0 = splash damage by shooting nearby geometry
#define CHARACTERISTIC_AIM_SKILL					16	//float [0, 1]
//weapon specific aim skill
#define CHARACTERISTIC_AIM_SKILL_BETTY				17
#define CHARACTERISTIC_AIM_SKILL_BALLOONY			18
#define CHARACTERISTIC_AIM_SKILL_BUBBLEG			19
#define CHARACTERISTIC_AIM_SKILL_IMPERIUS			20
//========================================================
//chat
//========================================================
//file with chats
#define CHARACTERISTIC_CHAT_FILE					21	//string
//name of the character with respect to the name that appears when the bot chats
#define CHARACTERISTIC_CHAT_NAME					22	//string
//type speed in characters per minute, good secretaries type about 350 cpm
#define CHARACTERISTIC_CHAT_CPM						23	//integer [1, 4000]
//tendency to insult/praise
#define CHARACTERISTIC_CHAT_INSULT					24	//float [0, 1]
//tendency to chat about miscellaneous subject
#define CHARACTERISTIC_CHAT_MISC					25	//float [0, 1]
//tendency to chat at start or end of level
#define CHARACTERISTIC_CHAT_STARTENDLEVEL			26	//float [0, 1]
//tendency to chat entering or exiting the game
#define CHARACTERISTIC_CHAT_ENTEREXITGAME			27	//float [0, 1]
//tendency to chat when the character killed someone
#define CHARACTERISTIC_CHAT_KILL					28	//float [0, 1]
//tendency to chat when the character died
#define CHARACTERISTIC_CHAT_DEATH					29	//float [0, 1]
//tendency to chat when enemy commits suicide
#define CHARACTERISTIC_CHAT_ENEMYSUICIDE			30	//float [0, 1]
//tendency to chat when hit while chatting
#define CHARACTERISTIC_CHAT_HITTALKING				31	//float [0, 1]
//tendency to chat when bot was hit but didn't die
#define CHARACTERISTIC_CHAT_HITNODEATH				32	//float [0, 1]
//tendency to chat when bot hit the enemy but enemy didn't die
#define CHARACTERISTIC_CHAT_HITNOKILL				33	//float [0, 1]
//tendency to chat at random moments
#define CHARACTERISTIC_CHAT_RANDOM					34	//float [0, 1]
//tendency to reply to chats
#define CHARACTERISTIC_CHAT_REPLY					35	//float [0, 1]
//========================================================
//movement
//========================================================
//tendency to crouch
#define CHARACTERISTIC_CROUCHER						36	//float [0, 1]
//tendency to jump
#define CHARACTERISTIC_JUMPER						37	//float [0, 1]
//tendency to walk
#define CHARACTERISTIC_WALKER						48	//float [0, 1]
//tendency to jump using a weapon
#define CHARACTERISTIC_WEAPONJUMPING				38	//float [0, 1]
//tendency to use the grapple hook when available
#define CHARACTERISTIC_GRAPPLE_USER					39	//float [0, 1]
//========================================================
//goal
//========================================================
//item weight file
#define CHARACTERISTIC_ITEMWEIGHTS					40	//string
//the aggression of the bot
#define CHARACTERISTIC_AGGRESSION					41	//float [0, 1]
//the self preservation of the bot (rockets near walls etc.)
#define CHARACTERISTIC_SELFPRESERVATION				42	//float [0, 1]
//how likely the bot is to take revenge
#define CHARACTERISTIC_VENGEFULNESS					43	//float [0, 1]
//tendency to camp
#define CHARACTERISTIC_SPRAYLOGO					44	//string
//========================================================
//========================================================
//tendency to get easy frags (such as on chatting players)
#define CHARACTERISTIC_EASY_FRAGGER					45	//float [0, 1]
//how alert the bot is (view distance)
#define CHARACTERISTIC_ALERTNESS					46	//float [0, 1]
//how long the bot fires it's weapon (continuous or bursts)
#define CHARACTERISTIC_FIRETHROTTLE					47	//float [0, 1]


