// Copyright (C) 1999-2000 Id Software, Inc.
//

/*****************************************************************************
 * name:		ai_dmq3.h
 *
 * desc:		Quake3 bot AI
 *
 * $Archive: /source/code/botai/ai_chat.c $
 *
 *****************************************************************************/

//ctf flags
#define CTF_FLAG_NONE		0
#define CTF_FLAG_RED		1
#define CTF_FLAG_BLUE		2

//setup the deathmatch AI
void BotSetupDeathmatchAI(void);
//let the bot live within its deathmatch AI net
void BotDeathmatchAI(bot_state_t *bs, float thinktime);

//choose a weapon
void BotChooseWeapon(bot_state_t *bs);
//setup movement stuff
void BotSetupForMovement(bot_state_t *bs);
//update the inventory
void BotUpdateInventory(bot_state_t *bs);
//update the inventory during battle
void BotUpdateBattleInventory(bot_state_t *bs, int enemy);
//use holdable items during battle
void BotBattleUseItems(bot_state_t *bs);
//return true if the bot is dead
qboolean BotIsDead(bot_state_t *bs);
//returns true if the bot is in observer mode
qboolean BotIsObserver(bot_state_t *bs);
//returns true if the bot is in the intermission
qboolean BotIntermission(bot_state_t *bs);
//returns true if the bot is in lava or slime
qboolean BotInLavaOrSlime(bot_state_t *bs);
//returns true if the entity is dead
qboolean EntityIsDead(aas_entityinfo_t *entinfo);
//returns true if the entity is invisible
qboolean EntityIsInvisible(aas_entityinfo_t *entinfo);
//returns true if the entity is shooting
qboolean EntityIsShooting(aas_entityinfo_t *entinfo);
// set a user info key/value pair
void BotSetUserInfo(bot_state_t *bs, char *key, char *value);
//returns the name of the client
char *ClientName(int client, char *name, int size);
//returns an simplyfied client name
char *EasyClientName(int client, char *name, int size);
//returns the skin used by the client
char *ClientSkin(int client, char *skin, int size);
// selection of goals for teamplay
void BotTeamGoals(bot_state_t *bs, int retreat);
//returns the aggression of the bot in the range [0, 100]
float BotAggression(bot_state_t *bs);
//returns true if the bot wants to retreat
int BotWantsToRetreat(bot_state_t *bs);
//returns true if the bot wants to chase
int BotWantsToChase(bot_state_t *bs);
//returns true if the bot wants to help
int BotWantsToHelp(bot_state_t *bs);
//the bot will perform attack movements
bot_moveresult_t BotAttackMove(bot_state_t *bs, int tfl);
//returns true if the bot and the entity are in the same team
int BotSameTeam(bot_state_t *bs, int entnum);
//returns true if teamplay is on
int TeamPlayIsOn(void);
//get the number of visible teammates and enemies
void BotVisibleTeamMatesAndEnemies(bot_state_t *bs, int *teammates, int *enemies, float range);
//returns true if within the field of vision for the given angles
qboolean InFieldOfVision(vec3_t viewangles, float fov, vec3_t angles);
//returns true and sets the .enemy field when an enemy is found
int BotFindEnemy(bot_state_t *bs, int curenemy);
//returns a roam goal
void BotRoamGoal(bot_state_t *bs, vec3_t goal);
//returns entity visibility in the range [0, 1]
float BotEntityVisible(int viewer, vec3_t eye, vec3_t viewangles, float fov, int ent);
//the bot will aim at the current enemy
void BotAimAtEnemy(bot_state_t *bs);
//check if the bot should attack
void BotCheckAttack(bot_state_t *bs);
//AI when the bot is blocked
void BotAIBlocked(bot_state_t *bs, bot_moveresult_t *moveresult, int activate);
//AI to predict obstacles
int BotAIPredictObstacles(bot_state_t *bs, bot_goal_t *goal);
//enable or disable the areas the blocking entity is in
void BotEnableActivateGoalAreas(bot_activategoal_t *activategoal, int enable);
//pop an activate goal from the stack
int BotPopFromActivateGoalStack(bot_state_t *bs);
//clear the activate goal stack
void BotClearActivateGoalStack(bot_state_t *bs);
//returns the team the bot is in
int BotTeam(bot_state_t *bs);
//retuns the opposite team of the bot
int BotOppositeTeam(bot_state_t *bs);
//remember the last ordered task
void BotRememberLastOrderedTask(bot_state_t *bs);
//strstr but case insensitive
char *stristr(char *str, char *charset);
//returns the number of the client with the given name
int ClientFromName(char *name);
int ClientOnSameTeamFromName(bot_state_t *bs, char *name);
//
int BotPointAreaNum(vec3_t origin);
//
void BotMapScripts(bot_state_t *bs);
qboolean ClientInSprayroom(int clId);
qboolean BotCheckChargedImp(bot_state_t *bs);
qboolean BotNmyTurnedInvalid(bot_state_t* bs);
int BotGetNumClientCarts(bot_state_t* bs, int clientnum);
int BotSprayWallCheckAttack(bot_state_t *bs);
void BotSelectLogo(bot_state_t* bs);
qboolean IsDuck(int ent);
qboolean IsWall(int ent);
qboolean IsBoomie(int ent);
qboolean IsBambam(int ent);
qboolean IsMyBalloon(int team, bot_goal_t* goal);
qboolean BotCTFCarryingFlag(bot_state_t* bs);
void CheckMatrixForGoal(bot_goal_t* goal);
qboolean GetDroppedLollyGoal(int team, bot_goal_t* goal);
qboolean GetCTLFlagGoal(int team, bot_goal_t* goal);

#define MAX_HSTATIONS	8

extern int gametype;		//game type
extern int maxclients;		//maximum number of clients

extern vmCvar_t bot_grapple;
extern vmCvar_t bot_rocketjump;
extern vmCvar_t bot_fastchat;
extern vmCvar_t bot_nochat;
extern vmCvar_t bot_testrchat;
extern vmCvar_t bot_challenge;

extern bot_goal_t ctf_redflag;
extern bot_goal_t ctf_blueflag;
extern bot_goal_t balloongoal[MAX_BALLOONS];
extern bot_goal_t rwall;
extern bot_goal_t bwall;
extern bot_goal_t spraytele;
extern bot_goal_t outtele;
extern bot_goal_t hstations[MAX_HSTATIONS];
extern int num_hstations;
