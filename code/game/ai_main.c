// Copyright (C) 1999-2000 Id Software, Inc.
//

/*****************************************************************************
 * name:		ai_main.c
 *****************************************************************************/


#include "g_local.h"
//#include "../qcommon/q_shared.h"
#include "../botlib/botlib.h"		//bot lib interface
#include "../botlib/be_aas.h"
#include "../botlib/be_ea.h"
#include "../botlib/be_ai_char.h"
#include "../botlib/be_ai_chat.h"
#include "../botlib/be_ai_gen.h"
#include "../botlib/be_ai_goal.h"
#include "../botlib/be_ai_move.h"
#include "../botlib/be_ai_weap.h"
//
#include "ai_main.h"
#include "ai_dmq3.h"
#include "ai_chat.h"
#include "ai_cmd.h"
#include "ai_dmnet.h"


//
#include "chars.h"
#include "inv.h"
#include "syn.h"

#define MAX_PATH		144


//bot states
bot_state_t	*botstates[MAX_CLIENTS];

ctf_waypoint_t waypoints[MAX_WAYPOINTS];
int numwaypoints;

bambamspot_t bambamspots[MAX_BAMBAMSPOTS];
int numbambamspots;

boomiespot_t boomiespots[MAX_BOOMIESPOTS];
int numboomiespots;

//number of bots
int numbots;
//floating point time
float floattime;
//time to do a regular update
float regularupdate_time;
//
int bot_interbreed;
int bot_interbreedmatchcount;
//
vmCvar_t bot_thinktime;
vmCvar_t bot_memorydump;
vmCvar_t bot_saveroutingcache;
vmCvar_t bot_pause;
vmCvar_t bot_testsolid;
vmCvar_t bot_testclusters;
vmCvar_t bot_developer;
vmCvar_t bot_interbreedchar;
vmCvar_t bot_interbreedbots;
vmCvar_t bot_interbreedcycle;
vmCvar_t bot_interbreedwrite;
// cyr
vmCvar_t bot_cachetest;
vmCvar_t bot_roamfactor;
vmCvar_t bot_shownextitem;
vmCvar_t bot_shownoitem;
vmCvar_t bot_showreachesfrom;
vmCvar_t bot_showreachesto;

void ExitLevel( void );

void ResetWaypoints(void){
	numwaypoints = 0;
	memset( waypoints, 0, sizeof(waypoints) );

	numbambamspots = 0;
	memset( bambamspots, 0, sizeof(bambamspots) );

	numboomiespots = 0;
	memset( boomiespots, 0, sizeof(boomiespots) );
}

//! does the bot have a valid waypoint
qboolean BotWpValid( bot_state_t* bs ){
	static const int validspan = 1000;	// invalidate wp if it was chosen more than 1 sec ago
	return( bs->wp && bs->wptime && level.time - bs->wptime < validspan );
}

ctf_waypoint_t* FindNearestWP( bot_state_t* bs){
	int i;
	int best = -1;
	int travelTime;
	int bestTravelTime = INT_MAX;

	if( !numwaypoints )
		return NULL;

	if( !trap_AAS_AreaReachability(bs->areanum) )
		return NULL;

	for(i=0; i<numwaypoints; i++){
		travelTime = trap_AAS_AreaTravelTimeToGoalArea( bs->areanum, bs->origin, waypoints[i].goal.areanum, bs->tfl );
		if(!travelTime) continue;

		if( travelTime < bestTravelTime ){
			bestTravelTime = travelTime;
			best = i;
		}
	}

	if( INT_MAX == bestTravelTime )
		return NULL;

	return &waypoints[best];
}

qboolean FindWp( bot_state_t* bs, qboolean direction_home ){
	int i; 
	int tt_bot = -1;
	int tt_wp = -1;
	ctf_waypoint_t* successor;
	ctf_waypoint_t* nearest = FindNearestWP(bs);
	int team = (direction_home) ? BotTeam(bs) : BotOppositeTeam(bs);
	team -= 1;	

	if( !nearest )
		return qfalse;
	// first assume nearest == best
	bs->wp = nearest;

	// see if we are between nearest and one of its successors
	for( i=0; i < nearest->links[team].num; i++){
		successor = nearest->links[team].link[i];
		tt_bot = trap_AAS_AreaTravelTimeToGoalArea( bs->areanum, bs->origin, successor->goal.areanum, bs->tfl );
		tt_wp = trap_AAS_AreaTravelTimeToGoalArea( nearest->goal.areanum, nearest->goal.origin, successor->goal.areanum, bs->tfl );
		if( tt_bot && tt_wp && tt_bot < tt_wp ){
			bs->wp = successor;
			break;
		}
	}

	if( bot_developer.integer & AIDBG_GAMETYPE ) 
	{
		Com_Printf("FindWp: nearest %s (%d), choose %s (%d)\n", nearest->name, tt_wp, bs->wp->name, tt_bot);
	}

	bs->wptime = level.time;
	return qtrue;
}

int GetWpID(ctf_waypoint_t* wp){
	if(!wp) return 0;
	return wp - waypoints;
}

//! take the bots waypoint (if he has none, find one) and randomly choose 
//! one of its leaving links and assign its waypoint to the bot. prefer good 
//! links over lateral ones
qboolean GetNextWp( bot_state_t* bs, qboolean direction_home ){
	int linkid;
	int team = (direction_home) ? BotTeam(bs) : BotOppositeTeam(bs);
	team -= 1;	

	if( team < 0 )	// team free
		return qfalse;

	// make sure the wp is valid
	if( !BotWpValid(bs) )
		if( !FindWp(bs, direction_home) )
			return qfalse;

	if( !bs->wp->links[team].num ){	// no good link? try a lateral one
		if( !bs->wp->links[WPLINKLATERAL].num ){
			return qfalse;
		}
		linkid = randomindex( bs->wp->links[WPLINKLATERAL].num );
		bs->wp = bs->wp->links[WPLINKLATERAL].link[ linkid ];
		return qtrue;
	}

	linkid = randomindex( bs->wp->links[ team ].num );
	bs->wp = bs->wp->links[ team ].link[linkid ];
	bs->wptime = level.time;
	return qtrue;
}

// true if the bots wp has outgoing links
qboolean BotWpHasSuccessor(bot_state_t* bs, qboolean direction_home){

	int team = (direction_home) ? BotTeam(bs) : BotOppositeTeam(bs);
	team -= 1;

	if( team<0 )
		return qfalse;
	if( !bs->wp )
		return qfalse;

	return ( bs->wp->links[team].num || bs->wp->links[WPLINKLATERAL].num );
}


/*
// wp traveltimes don't make much sense as they are. 
// also they can't be initialized at level start, because AAS init is not done at this point
void InitWpTravelTimes(void)
{
	int i;
	ctf_waypoint_t* wp;

	wp = &waypoints[0];
	for(i=0; i<numwaypoints; i++,wp++)
	{
		if( !trap_AAS_AreaReachability( wp->goal.areanum ) )
		{
			G_Printf("waypoint %s unreachable\n", wp->name);
			wp->travelTime[TEAM_RED] = wp->travelTime[TEAM_RED] = 99999;
			continue;
		}
		wp->travelTime[TEAM_RED] = trap_AAS_AreaTravelTimeToGoalArea( 
			wp->goal.areanum, wp->goal.origin, ctf_redflag.areanum, TFL_DEFAULT);

		if( !wp->travelTime[TEAM_RED] )
			G_Printf("^1 waypoint %s has no route to the red flag \n", wp->name);


		wp->travelTime[TEAM_BLUE] = trap_AAS_AreaTravelTimeToGoalArea( 
			wp->goal.areanum, wp->goal.origin, ctf_blueflag.areanum, TFL_DEFAULT);

		if( !wp->travelTime[TEAM_BLUE] )
			G_Printf("^1 waypoint %s has no route to the blue flag \n", wp->name);
	}
}
*/


char* ParseLink( char* buf, int type ){
	char wpfrom[128];
	char wpto[128];
	int i, from, to;
	//qboolean gotfrom, gotto;
	waypointlinks_t* links;
	char* token;

	token = COM_Parse( &buf );
	if( !*token ){
		G_Printf("missing link origin name \n");
		return NULL;
	}
	Q_strncpyz( wpfrom, token, sizeof( wpfrom ) );

	token = COM_Parse( &buf );
	if( !*token ){
		G_Printf("missing link target name \n");
		return NULL;
	}
	Q_strncpyz( wpto, token, sizeof( wpto ) );

	// find the according waypoints
	from = to = -1;
	
	for( i=0; i<numwaypoints; i++ ){
		// both found?
		if( from != -1 && to != -1 )
			break;

		if( from == -1 ){
			if( !Q_stricmp(waypoints[i].name, wpfrom ) ){
				from = i;
				continue;
			}
		}

		if( to == -1 ){
			if( !Q_stricmp(waypoints[i].name, wpto ) ){
				to = i;
			}
		}
	}

	if( from == -1 || to == -1 ){
		G_Printf("no match found for %s or %s \n", wpfrom, wpto );
		return NULL;
	}

	links = &waypoints[from].links[type];
	if( links->num >= MAX_LINKSPERTYPE ){
		G_Printf("waypoint %s can't have more links of type %d \n", wpfrom, type);
		return NULL;
	}

	links->link[ links->num++ ] = &waypoints[to];
	return buf;
}

char* ParseBambam(char* buf)
{
	char* token;
	team_t team;
	vec3_t pos;
	int i;
	bambamspot_t* spot;

	// format: bambam red|blue x y z 

	// red|blue
	token = COM_Parse( &buf );
	if( !*token ){
		G_Printf("parsing bambam %d: missing team \n", numbambamspots);
		return NULL;
	}

	if( token[0] == 'r' )
		team = TEAM_RED;
	else if( token[0] == 'b' )
		team = TEAM_BLUE;
	else
	{
		G_Printf("parsing bambam %d: unknown team \n", numbambamspots);
		return NULL;
	}

	for( i=0; i<3; i++ ){
		token = COM_Parse( &buf );
		if( !*token )
		{
			G_Printf("parsing bambam %d: missing coordinate %d \n", numbambamspots, i);
			return NULL;
		}
		pos[i] = atoi(token);		
	}
	
	// register
	if( numbambamspots >= MAX_BAMBAMSPOTS )
	{
		G_Printf("parsing bambam %d: too many bambamspots \n", numbambamspots);
		return NULL;
	}

	spot = &bambamspots[ numbambamspots++];
	spot->team = team;
	VectorCopy( pos, spot->goal.origin );
	VectorSet( spot->goal.mins, -8, -8, -8 );
	VectorSet( spot->goal.maxs, 8, 8, 8 );
	spot->goal.areanum = trap_AAS_BestReachableArea( spot->goal.origin, spot->goal.mins, spot->goal.maxs, spot->goal.origin );
	if( !spot->goal.areanum || !trap_AAS_AreaReachability(spot->goal.areanum) )
	{
		G_Printf("bambam %d is not in a valid area \n", numbambamspots );
		numbambamspots--; // remove it
	}

	return buf;
}

char* ParseBoomie(char* buf)
{
	char* token;
	team_t team;
	vec3_t pos;
	vec3_t angles;
	int i;
	boomiespot_t* spot;

	// format: boomie red|blue x y z pitch yaw

	// red|blue
	token = COM_Parse( &buf );
	if( !*token ){
		G_Printf("parsing boomie %d: missing team \n", numboomiespots);
		return NULL;
	}

	if( token[0] == 'r' )
		team = TEAM_RED;
	else if( token[0] == 'b' )
		team = TEAM_BLUE;
	else
	{
		G_Printf("parsing boomie %d: unknown team \n", numboomiespots);
		return NULL;
	}

	// x y z coords
	for( i=0; i<3; i++ ){
		token = COM_Parse( &buf );
		if( !*token )
		{
			G_Printf("parsing boomie %d: missing coordinate %d \n", numboomiespots, i);
			return NULL;
		}
		pos[i] = atoi(token);		
	}

	// pitch
	for( i=0; i<2; i++)
	{
		token = COM_Parse( &buf );
		if( !*token )
		{
			G_Printf("parsing boomie %d: missing angle \n", numboomiespots);
			return NULL;
		}
		angles[i] = atoi( token );
	}

	angles[2] = 0;

	// register boomie
	if( numboomiespots >= MAX_BOOMIESPOTS )
	{
		G_Printf("parsing boomie %d: too many boomiespots \n", numboomiespots);
		return NULL;
	}

	spot = &boomiespots[ numboomiespots++ ];
	spot->team = team;
	VectorCopy( angles, spot->angles );
	VectorCopy( pos, spot->goal.origin );
	VectorSet( spot->goal.mins, -8, -8, -8 );
	VectorSet( spot->goal.maxs, 8, 8, 8 );
	spot->goal.areanum = trap_AAS_BestReachableArea( spot->goal.origin, spot->goal.mins, spot->goal.maxs, spot->goal.origin );
	if( !spot->goal.areanum || !trap_AAS_AreaReachability(spot->goal.areanum) )
	{
		G_Printf("boomie %d is not in a valid area \n", numboomiespots );
		numboomiespots--; // remove it
	}

	return buf;
}

void ParseWaypointFile( char* buf ){
	char* token;
	ctf_waypoint_t* wp;
	int i;
	qboolean eof = qfalse;

	// reset waypoints
	ResetWaypoints();

	while(1){
		token = COM_Parse( &buf );
		if( !*token ){	// eof
			eof = qtrue;
			break;	
		}
		if( !Q_stricmp(token,"waypoint") ){

			if( numwaypoints >= MAX_WAYPOINTS ){
				G_Printf("waypoint limit exceeded \n");
				break;
			}

			wp = &waypoints[ numwaypoints++ ];
			// waypoint name
			token = COM_Parse( &buf );
			if( !*token ){
				G_Printf("missing waypoint name\n");
				break;
			}
			Q_strncpyz( wp->name, token, sizeof( wp->name ) );

			for( i=0; i<3; i++ ){
				token = COM_Parse( &buf );
				if( !*token ) break;
				wp->goal.origin[i] = atoi( token );
			}
			// exit if less then 3 tokens were read
			if(i<3){
				G_Printf("3 coordinates for waypoint %s needed, only %d present\n", wp->name, i);
				break;
			}
			// finish setting up the goal
			VectorSet( wp->goal.mins, -8, -8, -8 );
			VectorSet( wp->goal.maxs,  8,  8,  8 );
			wp->goal.areanum = trap_AAS_BestReachableArea( wp->goal.origin, wp->goal.mins, wp->goal.maxs, wp->goal.origin );
			if( !wp->goal.areanum || !trap_AAS_AreaReachability(wp->goal.areanum) )
				G_Printf("warning: waypoint %s is not in a valid area \n", wp->name );

		}
		else if( !Q_stricmp(token,"bambam") ){
			buf = ParseBambam( buf );
			if(!buf) break;
		}
		else if( !Q_stricmp(token,"boomie") ){
			buf = ParseBoomie( buf );
			if(!buf) break;
		}
		else if( !Q_stricmp(token,"linkred") ){
			buf = ParseLink( buf, WPLINKRED );
			if( !buf ) break;
		}
		else if( !Q_stricmp(token,"linkblue") ){
			buf = ParseLink( buf, WPLINKBLUE );
			if( !buf ) break;
		}
		else if( !Q_stricmp(token,"linklateral") ){
			buf = ParseLink( buf, WPLINKLATERAL );
			if( !buf ) break;
		}
		else{
			G_Printf("unknown token %s \n", token );
		}
	}

	if( !eof )
	{
		numwaypoints = 0;
		G_Printf("malformatted waypoint file\n");
	}
	else
	{
		G_Printf("parsed %d waypoints %d bambams %d boomies \n", numwaypoints, numbambamspots, numboomiespots);
	}
}

void ReadWaypointFile(void){
	int				len;
	fileHandle_t	f;
	char			buf[8192];
	char			mapname[MAX_QPATH];
	char			filename[MAX_QPATH];
	char			serverinfo[MAX_INFO_STRING];
	
	trap_GetServerinfo( serverinfo, sizeof(serverinfo) );
	Q_strncpyz( mapname, Info_ValueForKey( serverinfo, "mapname" ), sizeof(mapname) );
	Com_sprintf( filename, sizeof(filename), "maps/%s.wp", mapname );

	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( !f ) {
		trap_Printf( va( S_COLOR_RED "file not found: %s\n", filename ) );
		return;
	}
	if ( len >= 8192 ) {
		trap_Printf( va( S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, 8192 ) );
		trap_FS_FCloseFile( f );
		return;
	}

	trap_FS_Read( buf, len, f );
	buf[len] = 0;
	trap_FS_FCloseFile( f );

	ParseWaypointFile( buf );
}

void WaypointInit( ){
	ReadWaypointFile();	
	//InitWpTravelTimes();
}

void AI_AddBoomie(gentity_t* pEnt1, gentity_t* pEnt2)
{	
	int id = pEnt1 - g_entities;
	bot_state_t* bs = botstates[id];

	// if it wasn't built by a bot, we are done
	if( !bs || !bs->inuse )
		return;

	// sanity checks
	if( bs->ltgtype != LTG_PLANTBOOMIE )
		return;

	if( bs->teammate < 0 || bs->teammate >= numboomiespots )
		return;

	boomiespots[ bs->teammate ].occupied = pEnt2;
	bs->ltgtype = 0;
}

void AI_RemoveBoomie(gentity_t* pEnt)
{
	int i;
	for(i=0; i<numboomiespots; i++)
	{
		if( boomiespots[i].occupied == pEnt )
		{
			boomiespots[i].occupied = NULL;
			return;
		}
	}
}

void AI_AddBambam(gentity_t* pEnt1, gentity_t* pEnt2)
{
	int id = pEnt1 - g_entities;
	bot_state_t* bs = botstates[id];

	// if it wasn't built by a bot, we are done
	if( !bs || !bs->inuse )
		return;

	// sanity checks
	if( bs->ltgtype != LTG_PLANTBAMBAM )
		return;

	if( bs->teammate < 0 || bs->teammate >= numbambamspots )
		return;

	bambamspots[ bs->teammate ].occupied = pEnt2;
	bs->ltgtype = 0;
}

void AI_RemoveBambam(gentity_t* pEnt)
{
	int i;
	for(i=0; i<numbambamspots; i++)
	{
		if( bambamspots[i].occupied == pEnt )
		{
			bambamspots[i].occupied = NULL;
			return;
		}
	}
}

// append info to bot's CS
void BotAddInfo(bot_state_t* bs, char* value, int dbgFlags ){    
    if( bot_developer.integer & dbgFlags )
		StringDump_Push(bs->hudinfo, value);
}

/*
==================
BotAI_Print
==================
*/
void QDECL BotAI_Print(int type, char *fmt, ...) {
	char str[2048];
	va_list ap;

	va_start(ap, fmt);
	Q_vsnprintf(str, sizeof(str), fmt, ap);
	va_end(ap);

	switch(type) {
		case PRT_MESSAGE: {
			G_Printf("%s", str);
			break;
		}
		case PRT_WARNING: {
			G_Printf( S_COLOR_YELLOW "Warning: %s", str );
			break;
		}
		case PRT_ERROR: {
			G_Printf( S_COLOR_RED "Error: %s", str );
			break;
		}
		case PRT_FATAL: {
			G_Printf( S_COLOR_RED "Fatal: %s", str );
			break;
		}
		case PRT_EXIT: {
			G_Error( S_COLOR_RED "Exit: %s", str );
			break;
		}
		default: {
			G_Printf( "unknown print type\n" );
			break;
		}
	}
}


/*
==================
BotAI_Trace
==================
*/
void BotAI_Trace(bsp_trace_t *bsptrace, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passent, int contentmask) {
	trace_t trace;

	trap_Trace(&trace, start, mins, maxs, end, passent, contentmask);
	//copy the trace information
	bsptrace->allsolid = trace.allsolid;
	bsptrace->startsolid = trace.startsolid;
	bsptrace->fraction = trace.fraction;
	VectorCopy(trace.endpos, bsptrace->endpos);
	bsptrace->plane.dist = trace.plane.dist;
	VectorCopy(trace.plane.normal, bsptrace->plane.normal);
	bsptrace->plane.signbits = trace.plane.signbits;
	bsptrace->plane.type = trace.plane.type;
	bsptrace->surface.value = trace.surfaceFlags;
	bsptrace->ent = trace.entityNum;
	bsptrace->exp_dist = 0;
	bsptrace->sidenum = 0;
	bsptrace->contents = 0;
}

/*
==================
BotAI_GetClientState
==================
*/
int BotAI_GetClientState( int clientNum, playerState_t *state ) {
	gentity_t	*ent;

	ent = &g_entities[clientNum];
	if ( !ent->inuse ) {
		return qfalse;
	}
	if ( !ent->client ) {
		return qfalse;
	}

	memcpy( state, &ent->client->ps, sizeof(playerState_t) );
	return qtrue;
}

/*
==================
BotAI_GetEntityState
==================
*/
int BotAI_GetEntityState( int entityNum, entityState_t *state ) {
	gentity_t	*ent;

	ent = &g_entities[entityNum];
	memset( state, 0, sizeof(entityState_t) );
	if (!ent->inuse) return qfalse;
	if (!ent->r.linked) return qfalse;
	if (ent->r.svFlags & SVF_NOCLIENT) return qfalse;
	memcpy( state, &ent->s, sizeof(entityState_t) );
	return qtrue;
}

/*
==================
BotAI_GetSnapshotEntity
==================
*/
int BotAI_GetSnapshotEntity( int clientNum, int sequence, entityState_t *state ) {
	int		entNum;

	entNum = trap_BotGetSnapshotEntity( clientNum, sequence );
	if ( entNum == -1 ) {
		memset(state, 0, sizeof(entityState_t));
		return -1;
	}

	BotAI_GetEntityState( entNum, state );

	return sequence + 1;
}

/*
==================
BotAI_BotInitialChat
==================
*/
void QDECL BotAI_BotInitialChat( bot_state_t *bs, char *type, ... ) {
	int		i, mcontext;
	va_list	ap;
	char	*p;
	char	*vars[MAX_MATCHVARIABLES];

	memset(vars, 0, sizeof(vars));
	va_start(ap, type);
	p = va_arg(ap, char *);
	for (i = 0; i < MAX_MATCHVARIABLES; i++) {
		if( !p ) {
			break;
		}
		vars[i] = p;
		p = va_arg(ap, char *);
	}
	va_end(ap);

	mcontext = CONTEXT_NORMAL|CONTEXT_NEARBYITEM|CONTEXT_NAMES;

	trap_BotInitialChat( bs->cs, type, mcontext, vars[0], vars[1], vars[2], vars[3], vars[4], vars[5], vars[6], vars[7] );
}

// routing cache per area stuff, not used atm
/*
#define AREA_CACHELAG1				32		// cyr, 3 bits = 8 grades cacheupdate delay
#define AREA_CACHELAG2				64
#define AREA_CACHELAG3				128

int CacheUpdateTime(int flags){
    int ret=0;
    if(flags & AREA_CACHELAG3)
        ret+=4;
    if(flags & AREA_CACHELAG2)
        ret+=2;
    if(flags & AREA_CACHELAG1)
        ret+=1;
    return (ret*100)/7;
}
*/
/*
==================
BotTestAAS
==================
*/
void BotTestAAS(vec3_t origin) {
	int areanum;
	aas_areainfo_t info;

	trap_Cvar_Update(&bot_testsolid);
	trap_Cvar_Update(&bot_testclusters);
	if (bot_testsolid.integer) {
		if (!trap_AAS_Initialized()) return;
		areanum = BotPointAreaNum(origin);
		if (areanum) BotAI_Print(PRT_MESSAGE, "\remtpy area");
		else BotAI_Print(PRT_MESSAGE, "\r^1SOLID area");
	}
	else if (bot_testclusters.integer) {
		if (!trap_AAS_Initialized()) return;
		areanum = BotPointAreaNum(origin);
		if (!areanum)
			BotAI_Print(PRT_MESSAGE, "\r^1Solid!                              ");
		else {
			trap_AAS_AreaInfo(areanum, &info);
			//BotAI_Print(PRT_MESSAGE, "\rarea %d, cluster %d    cachetime %d   ", areanum, info.cluster, CacheUpdateTime(info.flags) );
			trap_SendServerCommand( -1, va("cp \"area %d, cluster %d \n\"", areanum, info.cluster) ); //, CacheUpdateTime(info.flags)) );
		}
	}
}

void BotAddInfoLtg( bot_state_t *bs ){
    //G_Printf("ltg: %i\n",i);
    switch( bs->ltgtype ){
		case 0:{
			char buf[128];
			bot_goal_t* goal = NULL;

			//get the goal at the top of the stack
			if(trap_BotGetTopGoal(bs->gs, goal) ){
				trap_BotGoalName(goal->number, buf, sizeof(buf));
				BotAddInfo(bs, va(" ltg: item %s",buf), AIDBG_ALL );
			}
			break;
		}
		case LTG_TEAMACCOMPANY:
			BotAddInfo(bs, "ltg: accompany", AIDBG_ALL );	
			break;
        case LTG_RUSHBASE:
			BotAddInfo(bs, "ltg: going for spraywall", AIDBG_ALL );
			break;
		case LTG_CAPTUREFLAG:
			BotAddInfo(bs, "ltg: bring flag to base", AIDBG_ALL );
			break;
        case LTG_GETFLAG:
			BotAddInfo(bs, "ltg: get the enemy flag", AIDBG_ALL );
        	break;
        	
		case LTG_ATTACKENEMYBASE:
			if( gametype == GT_BALLOON )
				BotAddInfo(bs, va("ltg: attack ball %s", g_entities[ bs->teamgoal.entitynum ].message ), AIDBG_ALL );
			else
				BotAddInfo(bs, "ltg: leave sprayroom", AIDBG_ALL);
			break;
		
		case LTG_DEFENDKEYAREA:
			if( gametype == GT_BALLOON )
				BotAddInfo(bs, va("ltg: defend ball %s", g_entities[ bs->teamgoal.entitynum ].message ), AIDBG_ALL );	
			else if( gametype == GT_CTF )
				BotAddInfo(bs, "ltg: defending flag", AIDBG_ALL );
			break;
        case LTG_BALLCAMP:
			BotAddInfo(bs, "ltg: camp loon", AIDBG_ALL );	
			break;
		case LTG_FETCHCART:
			BotAddInfo(bs, "ltg: fetch cart", AIDBG_ALL );	
			break;
		case LTG_GIVECART:	
			BotAddInfo(bs, "ltg: give cart", AIDBG_ALL );	
			break;
        case LTG_GETITEM:
			BotAddInfo(bs, "ltg: collect item", AIDBG_ALL );	
			break;
		case LTG_GO_FOR_HEALTH:
			BotAddInfo(bs, "ltg: going for health", AIDBG_ALL );	
			break;
		case LTG_PLANTBAMBAM:
			BotAddInfo(bs, "ltg: plant bambam", AIDBG_ALL );
			break;
		case LTG_PLANTBOOMIE:
			BotAddInfo(bs, "ltg: plant boomie", AIDBG_ALL);
			break;
        case LTG_JOINMATE:{
			char matename[128];
			ClientName(bs->client, matename, 128);
			BotAddInfo(bs, va("ltg: join mate %s",matename), AIDBG_ALL );	
			break;
		case LTG_PICKUPFLAG:
			BotAddInfo(bs, "ltg: pickup", AIDBG_ALL);
			break;
		}
        default:
			BotAddInfo(bs, va("ltg: %d", bs->ltgtype), AIDBG_ALL );
	}
}

void BotAddInfoNode(bot_state_t* bs){
	char ainode[MAX_MESSAGE_SIZE];

		// ainode
	if(bs->ainode == AINode_Seek_NBG)
		Com_sprintf(ainode, sizeof(ainode),"seek_nbg");
	else if(bs->ainode == AINode_Seek_LTG)
		Com_sprintf(ainode, sizeof(ainode),"Seek_LTG");
	else if(bs->ainode == AINode_Battle_Fight)
		Com_sprintf(ainode, sizeof(ainode),"Battle_Fight");
	else if(bs->ainode == AINode_Battle_Chase)
		Com_sprintf(ainode, sizeof(ainode),"Battle_Chase");
	else if(bs->ainode == AINode_Battle_Retreat)
		Com_sprintf(ainode, sizeof(ainode),"Battle_Retreat");
	else if(bs->ainode == AINode_Battle_NBG)
		Com_sprintf(ainode, sizeof(ainode),"Battle_NBG");
	else
		Com_sprintf(ainode, sizeof(ainode),"unknown");

	BotAddInfo(bs, va("ainode %s", ainode), AIDBG_ALL);
}

void BotSetInfoConfigString(bot_state_t *bs) {

	// write ai-node
	BotAddInfoNode(bs);

	// cut this and "ordered" out... belongs to seek_ltg and btl_retreat only
	if( !bs->ltgtype ){
		bot_goal_t goal;
		char goalname[MAX_INFO_VALUE];
		char ltgstr[MAX_INFO_VALUE];
		trap_BotGetTopGoal(bs->gs, &goal);
		trap_BotGoalName(goal.number, goalname, sizeof(goalname));
		Com_sprintf(ltgstr, sizeof(ltgstr), "item %s", goalname);
		BotAddInfo(bs, va("ltg %s", ltgstr), AIDBG_ALL );
	}
	else
		BotAddInfoLtg( bs );

	// enemy
	if( bs->enemy < 0){
		// no enemy
	}
	else if( bs->enemy < MAX_CLIENTS){
		char nmyname[32];
		//aas_entityinfo_t entinfo;
		//vec3_t offset;

		ClientName(bs->enemy, nmyname, sizeof(nmyname) );
		BotAddInfo(bs, va("nmy: %s", nmyname), AIDBG_COMBAT );

		//BotEntityInfo(bs->enemy, &entinfo);
		//if(entinfo.valid)
		//{
		//	VectorSubtract(bs->origin, entinfo.origin, offset);
		//	BotAddInfo(bs, va("nmydist %d",(int)VectorLength(offset)), AIDBG_COMBAT );
		//}
	}
	else if( IsDuck( bs->enemy ))
		BotAddInfo(bs, "nmy: killerduck", AIDBG_COMBAT );
	else if( IsWall( bs->enemy ))
		BotAddInfo(bs, "nmy: spray wall", AIDBG_COMBAT );
	else if( IsBoomie( bs->enemy ))
		BotAddInfo(bs, "nmy: boomie", AIDBG_COMBAT );
	else if( IsBambam( bs->enemy ))
		BotAddInfo(bs, "nmy: bambam", AIDBG_COMBAT );
}

/*
==============
BotUpdateInfoConfigStrings
==============
*/
void BotUpdateInfoConfigStrings(void) {
	int i;
	char buf[MAX_INFO_STRING];

	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
		//
		if ( !botstates[i] || !botstates[i]->inuse )
			continue;
		//
		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
		//if no config string or no name
		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n")))
			continue;
		BotSetInfoConfigString(botstates[i]);
	}
}

/*
==============
BotInterbreedBots
==============
*/
void BotInterbreedBots(void) {
	float ranks[MAX_CLIENTS];
	int parent1, parent2, child;
	int i;

	// get rankings for all the bots
	for (i = 0; i < MAX_CLIENTS; i++) {
		if ( botstates[i] && botstates[i]->inuse ) {
			ranks[i] = botstates[i]->num_kills * 2 - botstates[i]->num_deaths;
		}
		else {
			ranks[i] = -1;
		}
	}

	if (trap_GeneticParentsAndChildSelection(MAX_CLIENTS, ranks, &parent1, &parent2, &child)) {
		trap_BotInterbreedGoalFuzzyLogic(botstates[parent1]->gs, botstates[parent2]->gs, botstates[child]->gs);
		trap_BotMutateGoalFuzzyLogic(botstates[child]->gs, 1);
	}
	// reset the kills and deaths
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (botstates[i] && botstates[i]->inuse) {
			botstates[i]->num_kills = 0;
			botstates[i]->num_deaths = 0;
		}
	}
}

/*
==============
BotWriteInterbreeded
==============
*/
void BotWriteInterbreeded(char *filename) {
	float rank, bestrank;
	int i, bestbot;

	bestrank = 0;
	bestbot = -1;
	// get the best bot
	for (i = 0; i < MAX_CLIENTS; i++) {
		if ( botstates[i] && botstates[i]->inuse ) {
			rank = botstates[i]->num_kills * 2 - botstates[i]->num_deaths;
		}
		else {
			rank = -1;
		}
		if (rank > bestrank) {
			bestrank = rank;
			bestbot = i;
		}
	}
	if (bestbot >= 0) {
		//write out the new goal fuzzy logic
		trap_BotSaveGoalFuzzyLogic(botstates[bestbot]->gs, filename);
	}
}

/*
==============
BotInterbreedEndMatch

add link back into ExitLevel?
==============
*/
void BotInterbreedEndMatch(void) {

	if (!bot_interbreed) return;
	bot_interbreedmatchcount++;
	if (bot_interbreedmatchcount >= bot_interbreedcycle.integer) {
		bot_interbreedmatchcount = 0;
		//
		trap_Cvar_Update(&bot_interbreedwrite);
		if (strlen(bot_interbreedwrite.string)) {
			BotWriteInterbreeded(bot_interbreedwrite.string);
			trap_Cvar_Set("bot_interbreedwrite", "");
		}
		BotInterbreedBots();
	}
}

/*
==============
BotInterbreeding
==============
*/
void BotInterbreeding(void) {
	int i;

	trap_Cvar_Update(&bot_interbreedchar);
	if (!strlen(bot_interbreedchar.string)) return;
	//make sure we are in tournament mode
	if (gametype != GT_TOURNAMENT) {
		trap_Cvar_Set("g_gametype", va("%d", GT_TOURNAMENT));
		ExitLevel();
		return;
	}
	//shutdown all the bots
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (botstates[i] && botstates[i]->inuse) {
			BotAIShutdownClient(botstates[i]->client, qfalse);
		}
	}
	//make sure all item weight configs are reloaded and Not shared
	trap_BotLibVarSet("bot_reloadcharacters", "1");
	//add a number of bots using the desired bot character
	for (i = 0; i < bot_interbreedbots.integer; i++) {
		trap_SendConsoleCommand( EXEC_INSERT, va("addbot %s 4 free %i %s%d\n",
						bot_interbreedchar.string, i * 50, bot_interbreedchar.string, i) );
	}
	//
	trap_Cvar_Set("bot_interbreedchar", "");
	bot_interbreed = qtrue;
}

/*
==============
BotEntityInfo
==============
*/
void BotEntityInfo(int entnum, aas_entityinfo_t *info) {
	if(entnum < 0 || entnum >= MAX_GENTITIES)
	{
		memset(info,0,sizeof(aas_entityinfo_t));
		if (bot_developer.integer)
			BotAI_Print(PRT_ERROR, "BotEntityInfo: entnum out of range: %d\n", entnum);
	}
	else
		trap_AAS_EntityInfo(entnum, info);
}

/*
==============
NumBots
==============
*/
int NumBots(void) {
	return numbots;
}

/*
==============
AngleDifference
==============
*/
float AngleDifference(float ang1, float ang2) {
	float diff;

	diff = ang1 - ang2;
	if (ang1 > ang2) {
		if (diff > 180.0) diff -= 360.0;
	}
	else {
		if (diff < -180.0) diff += 360.0;
	}
	return diff;
}

/*
==============
BotChangeViewAngle
==============
*/
float BotChangeViewAngle(float angle, float ideal_angle, float speed) {
	float move;

	angle = AngleMod(angle);
	ideal_angle = AngleMod(ideal_angle);
	if (angle == ideal_angle) return angle;
	move = ideal_angle - angle;
	if (ideal_angle > angle) {
		if (move > 180.0) move -= 360.0;
	}
	else {
		if (move < -180.0) move += 360.0;
	}
	if (move > 0) {
		if (move > speed) move = speed;
	}
	else {
		if (move < -speed) move = -speed;
	}
	return AngleMod(angle + move);
}

/*
==============
BotChangeViewAngles
==============
*/
void BotChangeViewAngles(bot_state_t *bs, float thinktime) {
	float diff, factor, maxchange, anglespeed, disired_speed;
	int i;

	if (bs->ideal_viewangles[PITCH] > 180) bs->ideal_viewangles[PITCH] -= 360;
	//
	if (bs->enemy >= 0) {
		factor = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_VIEW_FACTOR, 0.01f, 1);
		maxchange = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_VIEW_MAXCHANGE, 1, 1800);
	}
	else {
		factor = 0.05f;
		maxchange = 360;
	}
	if (maxchange < 240) maxchange = 240;
	maxchange *= thinktime;
	for (i = 0; i < 2; i++) {
		//
		if (bot_challenge.integer) {
			//smooth slowdown view model
			diff = abs(AngleDifference(bs->viewangles[i], bs->ideal_viewangles[i]));
			anglespeed = diff * factor;
			if (anglespeed > maxchange) anglespeed = maxchange;
			bs->viewangles[i] = BotChangeViewAngle(bs->viewangles[i],
											bs->ideal_viewangles[i], anglespeed);
		}
		else {
			//over reaction view model
			bs->viewangles[i] = AngleMod(bs->viewangles[i]);
			bs->ideal_viewangles[i] = AngleMod(bs->ideal_viewangles[i]);
			diff = AngleDifference(bs->viewangles[i], bs->ideal_viewangles[i]);
			disired_speed = diff * factor;
			bs->viewanglespeed[i] += (bs->viewanglespeed[i] - disired_speed);
			if (bs->viewanglespeed[i] > 180) bs->viewanglespeed[i] = maxchange;
			if (bs->viewanglespeed[i] < -180) bs->viewanglespeed[i] = -maxchange;
			anglespeed = bs->viewanglespeed[i];
			if (anglespeed > maxchange) anglespeed = maxchange;
			if (anglespeed < -maxchange) anglespeed = -maxchange;
			bs->viewangles[i] += anglespeed;
			bs->viewangles[i] = AngleMod(bs->viewangles[i]);
			//demping
			bs->viewanglespeed[i] *= 0.45 * (1 - factor);
		}
		//BotAI_Print(PRT_MESSAGE, "ideal_angles %f %f\n", bs->ideal_viewangles[0], bs->ideal_viewangles[1], bs->ideal_viewangles[2]);`
		//bs->viewangles[i] = bs->ideal_viewangles[i];
	}
	//bs->viewangles[PITCH] = 0;
	if (bs->viewangles[PITCH] > 180) bs->viewangles[PITCH] -= 360;
	//elementary action: view
	trap_EA_View(bs->client, bs->viewangles);
}

/*
==============
BotInputToUserCommand
==============
*/
void BotInputToUserCommand(bot_input_t *bi, usercmd_t *ucmd, int delta_angles[3], int time) {
	vec3_t angles, forward, right;
	short temp;
		int j, forwardmv, rightmv, upmv; 

	//clear the whole structure
	memset(ucmd, 0, sizeof(usercmd_t));
	//
	//Com_Printf("dir = %f %f %f speed = %f\n", bi->dir[0], bi->dir[1], bi->dir[2], bi->speed);
	//the duration for the user command in milli seconds
	ucmd->serverTime = time;
	//
	if (bi->actionflags & ACTION_DELAYEDJUMP) {
		bi->actionflags |= ACTION_JUMP;
		bi->actionflags &= ~ACTION_DELAYEDJUMP;
	}
	//set the buttons
	if (bi->actionflags & ACTION_RESPAWN) ucmd->buttons = BUTTON_ATTACK;
	if (bi->actionflags & ACTION_ATTACK) ucmd->buttons |= BUTTON_ATTACK;
	if (bi->actionflags & ACTION_TALK) ucmd->buttons |= BUTTON_TALK;
	if (bi->actionflags & ACTION_GESTURE) ucmd->buttons |= BUTTON_GESTURE;
	if (bi->actionflags & ACTION_USE) ucmd->buttons |= BUTTON_USE_HOLDABLE;
	if (bi->actionflags & ACTION_WALK) ucmd->buttons |= BUTTON_WALKING;
	if (bi->actionflags & ACTION_AFFIRMATIVE) ucmd->buttons |= BUTTON_AFFIRMATIVE;
	if (bi->actionflags & ACTION_NEGATIVE) ucmd->buttons |= BUTTON_NEGATIVE;
	if (bi->actionflags & ACTION_GETFLAG) ucmd->buttons |= BUTTON_GETFLAG;
	if (bi->actionflags & ACTION_GUARDBASE) ucmd->buttons |= BUTTON_GUARDBASE;
	if (bi->actionflags & ACTION_PATROL) ucmd->buttons |= BUTTON_PATROL;
	if (bi->actionflags & ACTION_FOLLOWME) ucmd->buttons |= BUTTON_FOLLOWME;
	//
	ucmd->weapon = bi->weapon;
	//set the view angles
	//NOTE: the ucmd->angles are the angles WITHOUT the delta angles
	ucmd->angles[PITCH] = ANGLE2SHORT(bi->viewangles[PITCH]);
	ucmd->angles[YAW] = ANGLE2SHORT(bi->viewangles[YAW]);
	ucmd->angles[ROLL] = ANGLE2SHORT(bi->viewangles[ROLL]);
	//subtract the delta angles
	for (j = 0; j < 3; j++) {
		temp = ucmd->angles[j] - delta_angles[j];
		/*NOTE: disabled because temp should be mod first
		if ( j == PITCH ) {
			// don't let the player look up or down more than 90 degrees
			if ( temp > 16000 ) temp = 16000;
			else if ( temp < -16000 ) temp = -16000;
		}
		*/
		ucmd->angles[j] = temp;
	}
	//NOTE: movement is relative to the REAL view angles
	//get the horizontal forward and right vector
	//get the pitch in the range [-180, 180]
	if (bi->dir[2]) angles[PITCH] = bi->viewangles[PITCH];
	else angles[PITCH] = 0;
	angles[YAW] = bi->viewangles[YAW];
	angles[ROLL] = 0;
	AngleVectors(angles, forward, right, NULL);
	//bot input speed is in the range [0, 400]
	bi->speed = bi->speed * 127 / 400;

	//set the view independent movement
	forwardmv = DotProduct(forward, bi->dir) * bi->speed;
	rightmv = DotProduct(right, bi->dir) * bi->speed;
	upmv = abs(forward[2]) * bi->dir[2] * bi->speed;
	//normal keyboard movement
	if (bi->actionflags & ACTION_MOVEFORWARD) forwardmv += 127;
	if (bi->actionflags & ACTION_MOVEBACK) forwardmv -= 127;
	if (bi->actionflags & ACTION_MOVELEFT) rightmv -= 127;
	if (bi->actionflags & ACTION_MOVERIGHT) rightmv += 127;
	//jump/moveup
	if (bi->actionflags & ACTION_JUMP) upmv += 127;
	//crouch/movedown
	if (bi->actionflags & ACTION_CROUCH) upmv -= 127;
	
	ucmd->forwardmove = (forwardmv > 127) ? 127 : forwardmv;
	ucmd->rightmove = (rightmv > 127) ? 127 : rightmv;
	ucmd->upmove = (upmv > 127) ? 127 : upmv;
	//
	//Com_Printf("forward = %d right = %d up = %d\n", ucmd->forwardmove, ucmd->rightmove, ucmd->upmove);
	//Com_Printf("ucmd->serverTime = %d\n", ucmd->serverTime);
}

/*
==============
BotUpdateInput
==============
*/
void BotShowViewAngles(bot_state_t* bs){
	vec3_t forward, end;
	AngleVectors( bs->ideal_viewangles, forward, NULL, NULL );
	//VectorScale(forward, 500, forward);
	//VectorAdd(bs->origin, forward, end);
	VectorMA(bs->origin, 300, forward, end);
	DebugLineDouble(bs->origin, end, 4);
}

void BotUpdateInput(bot_state_t *bs, int time, int elapsed_time) {
	bot_input_t bi;
	int j;

	//add the delta angles to the bot's current view angles
	for (j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] + SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}
	//change the bot view angles
	BotChangeViewAngles(bs, (float) elapsed_time / 1000);
	//retrieve the bot input
	trap_EA_GetInput(bs->client, (float) time / 1000, &bi);
	//respawn hack
	if (bi.actionflags & ACTION_RESPAWN) {
		if (bs->lastucmd.buttons & BUTTON_ATTACK) bi.actionflags &= ~(ACTION_RESPAWN|ACTION_ATTACK);
	}
	//convert the bot input to a usercmd
	BotInputToUserCommand(&bi, &bs->lastucmd, bs->cur_ps.delta_angles, time);

	//subtract the delta angles
	for (j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] - SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}
}

/*
==============
BotAIRegularUpdate
==============
*/
void BotAIRegularUpdate(void) {
	if (regularupdate_time < FloatTime()) {
		trap_BotUpdateEntityItems();
		regularupdate_time = FloatTime() + 0.3;
	}
}

/*
==============
RemoveColorEscapeSequences
==============
*/
void RemoveColorEscapeSequences( char *text ) {
	int i, l;

	l = 0;
	for ( i = 0; text[i]; i++ ) {
		if (Q_IsColorString(&text[i])) {
			i++;
			continue;
		}
		if (text[i] > 0x7E)
			continue;
		text[l++] = text[i];
	}
	text[l] = '\0';
}

/*
==============
BotAI
==============
*/
int BotAI(int client, float thinktime) {
	bot_state_t *bs;
	char buf[1024], *args;
	int j;
	int cdi_type;	// cyr

	trap_EA_ResetInput(client);
	//
	bs = botstates[client];
	if (!bs || !bs->inuse) {
		BotAI_Print(PRT_FATAL, "BotAI: client %d is not setup\n", client);
		return qfalse;
	}

	//retrieve the current client state
	BotAI_GetClientState( client, &bs->cur_ps );

	//retrieve any waiting server commands
	while( trap_BotGetServerCommand(client, buf, sizeof(buf)) ) {
		//have buf point to the command and args to the command arguments
		args = strchr( buf, ' ');
		if (!args) continue;
		*args++ = '\0';

		//remove color espace sequences from the arguments
		RemoveColorEscapeSequences( args );

		if (!Q_stricmp(buf, "cp "))
			{ /*CenterPrintf*/ }
		else if (!Q_stricmp(buf, "cs"))
			{ /*ConfigStringModified*/ }
		else if (!Q_stricmp(buf, "print")) {
			//remove first and last quote from the chat message
			memmove(args, args+1, strlen(args));
			args[strlen(args)-1] = '\0';
			trap_BotQueueConsoleMessage(bs->cs, CMS_NORMAL, args);
		}
		else if (!Q_stricmp(buf, "say"))
		{
			//remove first and last quote from the chat message
			memmove(args, args+5, strlen(args));
			args[strlen(args)-1] = '\0';
			trap_BotQueueConsoleMessage(bs->cs, CMS_CHAT, args);
		}
		else if (!Q_stricmp(buf, "scores"))
			{ /*FIXME: parse scores?*/ }
		else if (!Q_stricmp(buf, "clientLevelShot"))
			{ /*ignore*/ }

		else if(!Q_stricmp(buf,"srwc"))//srwc->sprayroomweaponchange ;)
		{
//			trap_SendConsoleCommand(va("weapon %s",CG_Argv(1)));
			bs->weaponnum = atoi(args);
			trap_EA_SelectWeapon(bs->client, bs->weaponnum);
		}

// cyr{
		else if(!Q_stricmp(buf,"cdi")){
			cdi_type = atoi(args);
			if(cdi_type == 3)
				BotSelectLogo(bs);
		}
// cyr}

	}
	//add the delta angles to the bot's current view angles
	for (j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] + SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}
	//increase the local time of the bot
	bs->ltime += thinktime;
	//
	bs->thinktime = thinktime;
	//origin of the bot
	VectorCopy(bs->cur_ps.origin, bs->origin);
	//eye coordinates of the bot
	VectorCopy(bs->cur_ps.origin, bs->eye);
	bs->eye[2] += bs->cur_ps.viewheight;
	//get the area the bot is in
	bs->areanum = BotPointAreaNum(bs->origin);

	// reset the bot's CS
	bs->hudinfo[0] = '\0';		//trap_SetConfigstring( CS_BOTINFO + bs->client, "");
	//the real AI
	BotDeathmatchAI(bs, thinktime);

	
	if(bot_developer.integer && bs->observed){
		BotSetInfoConfigString(bs);
		// copy config string
		trap_SetConfigstring( CS_BOTINFO, bs->hudinfo);
	}

	//set the weapon selection every AI frame
	trap_EA_SelectWeapon(bs->client, bs->weaponnum);
	//subtract the delta angles
	for (j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] - SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}
	//everything was ok
	return qtrue;
}

/*
==================
BotScheduleBotThink
==================
*/
void BotScheduleBotThink(void) {
	int i, botnum;

	botnum = 0;

	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if( !botstates[i] || !botstates[i]->inuse ) {
			continue;
		}
		//initialize the bot think residual time
		botstates[i]->botthink_residual = bot_thinktime.integer * botnum / numbots;
		botnum++;
	}
}

/*
void BotWriteSessionData(bot_state_t *bs) {
	const char	*s;
	const char	*var;

	s = va(
			"%i %i %i %i %i %i %i %i"
			" %f %f %f"
			" %f %f %f"
			" %f %f %f",
		bs->lastgoal_decisionmaker,
		bs->lastgoal_ltgtype,
		bs->lastgoal_teammate,
		bs->lastgoal_teamgoal.areanum,
		bs->lastgoal_teamgoal.entitynum,
		bs->lastgoal_teamgoal.flags,
		bs->lastgoal_teamgoal.iteminfo,
		bs->lastgoal_teamgoal.number,
		bs->lastgoal_teamgoal.origin[0],
		bs->lastgoal_teamgoal.origin[1],
		bs->lastgoal_teamgoal.origin[2],
		bs->lastgoal_teamgoal.mins[0],
		bs->lastgoal_teamgoal.mins[1],
		bs->lastgoal_teamgoal.mins[2],
		bs->lastgoal_teamgoal.maxs[0],
		bs->lastgoal_teamgoal.maxs[1],
		bs->lastgoal_teamgoal.maxs[2]
		);

	var = va( "botsession%i", bs->client );

	trap_Cvar_Set( var, s );
}


void BotReadSessionData(bot_state_t *bs) {
	char	s[MAX_STRING_CHARS];
	const char	*var;

	var = va( "botsession%i", bs->client );
	trap_Cvar_VariableStringBuffer( var, s, sizeof(s) );

	sscanf(s,
			"%i %i %i %i %i %i %i %i"
			" %f %f %f"
			" %f %f %f"
			" %f %f %f",
		&bs->lastgoal_decisionmaker,
		&bs->lastgoal_ltgtype,
		&bs->lastgoal_teammate,
		&bs->lastgoal_teamgoal.areanum,
		&bs->lastgoal_teamgoal.entitynum,
		&bs->lastgoal_teamgoal.flags,
		&bs->lastgoal_teamgoal.iteminfo,
		&bs->lastgoal_teamgoal.number,
		&bs->lastgoal_teamgoal.origin[0],
		&bs->lastgoal_teamgoal.origin[1],
		&bs->lastgoal_teamgoal.origin[2],
		&bs->lastgoal_teamgoal.mins[0],
		&bs->lastgoal_teamgoal.mins[1],
		&bs->lastgoal_teamgoal.mins[2],
		&bs->lastgoal_teamgoal.maxs[0],
		&bs->lastgoal_teamgoal.maxs[1],
		&bs->lastgoal_teamgoal.maxs[2]
		);
}
*/

/*
==============
BotAISetupClient
==============
*/
int BotAISetupClient(int client, struct bot_settings_s *settings, qboolean restart) {
	char filename[MAX_PATH], name[MAX_PATH], gender[MAX_PATH];
	bot_state_t *bs;
	int errnum;

	if (!botstates[client]) botstates[client] = G_Alloc(sizeof(bot_state_t));
	bs = botstates[client];

	if (bs && bs->inuse) {
		BotAI_Print(PRT_FATAL, "BotAISetupClient: client %d already setup\n", client);
		return qfalse;
	}

	if (!trap_AAS_Initialized()) {
		BotAI_Print(PRT_FATAL, "AAS not initialized\n");
		return qfalse;
	}

	//load the bot character
	bs->character = trap_BotLoadCharacter(settings->characterfile, settings->skill);
	if (!bs->character) {
		BotAI_Print(PRT_FATAL, "couldn't load skill %f from %s\n", settings->skill, settings->characterfile);
		return qfalse;
	}
	//copy the settings
	memcpy(&bs->settings, settings, sizeof(bot_settings_t));
	//allocate a goal state
	bs->gs = trap_BotAllocGoalState(client);
	//load the item weights
	trap_Characteristic_String(bs->character, CHARACTERISTIC_ITEMWEIGHTS, filename, MAX_PATH);
	errnum = trap_BotLoadItemWeights(bs->gs, filename);
	if (errnum != BLERR_NOERROR) {
		trap_BotFreeGoalState(bs->gs);
		return qfalse;
	}
	//allocate a weapon state
	bs->ws = trap_BotAllocWeaponState();
	//load the weapon weights
	trap_Characteristic_String(bs->character, CHARACTERISTIC_WEAPONWEIGHTS, filename, MAX_PATH);
	errnum = trap_BotLoadWeaponWeights(bs->ws, filename);
	if (errnum != BLERR_NOERROR) {
		trap_BotFreeGoalState(bs->gs);
		trap_BotFreeWeaponState(bs->ws);
		return qfalse;
	}
	//allocate a chat state
	bs->cs = trap_BotAllocChatState();
	//load the chat file
	trap_Characteristic_String(bs->character, CHARACTERISTIC_CHAT_FILE, filename, MAX_PATH);
	trap_Characteristic_String(bs->character, CHARACTERISTIC_CHAT_NAME, name, MAX_PATH);
	errnum = trap_BotLoadChatFile(bs->cs, filename, name);
	if (errnum != BLERR_NOERROR) {
		trap_BotFreeChatState(bs->cs);
		trap_BotFreeGoalState(bs->gs);
		trap_BotFreeWeaponState(bs->ws);
		return qfalse;
	}
	//get the gender characteristic
	trap_Characteristic_String(bs->character, CHARACTERISTIC_GENDER, gender, MAX_PATH);
	//set the chat gender
	if (*gender == 'f' || *gender == 'F') trap_BotSetChatGender(bs->cs, CHAT_GENDERFEMALE);
	else if (*gender == 'm' || *gender == 'M') trap_BotSetChatGender(bs->cs, CHAT_GENDERMALE);
	else trap_BotSetChatGender(bs->cs, CHAT_GENDERLESS);

	bs->inuse = qtrue;
	bs->client = client;
	bs->entitynum = client;
	bs->setupcount = 4;
	bs->entergame_time = FloatTime();
	bs->ms = trap_BotAllocMoveState();
	bs->walker = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_WALKER, 0, 1);
	numbots++;

	//if (trap_Cvar_VariableIntegerValue("bot_testichat")) {
	//	trap_BotLibVarSet("bot_testichat", "1");
	//	BotChatTest(bs);
	//}
	//NOTE: reschedule the bot thinking
	BotScheduleBotThink();
	//if interbreeding start with a mutation
	if (bot_interbreed) {
		trap_BotMutateGoalFuzzyLogic(bs->gs, 1);
	}
	// if we kept the bot client
	//if (restart) {
	//	BotReadSessionData(bs);
	//}

	//bot has been setup succesfully
	return qtrue;
}

/*
==============
BotAIShutdownClient
==============
*/
int BotAIShutdownClient(int client, qboolean restart) {
	bot_state_t *bs;

	bs = botstates[client];
	if (!bs || !bs->inuse) {
		//BotAI_Print(PRT_ERROR, "BotAIShutdownClient: client %d already shutdown\n", client);
		return qfalse;
	}

	//if (restart) {
	//	BotWriteSessionData(bs);
	//}

	if (BotChat_ExitGame(bs)) {
		trap_BotEnterChat(bs->cs, bs->client, CHAT_ALL);
	}

	trap_BotFreeMoveState(bs->ms);
	//free the goal state`
	trap_BotFreeGoalState(bs->gs);
	//free the chat file
	trap_BotFreeChatState(bs->cs);
	//free the weapon weights
	trap_BotFreeWeaponState(bs->ws);
	//free the bot character
	trap_BotFreeCharacter(bs->character);

	//clear activate goal stack
	BotClearActivateGoalStack(bs);
	//clear the bot state
	memset(bs, 0, sizeof(bot_state_t));
	//set the inuse flag to qfalse
	bs->inuse = qfalse;
	//there's one bot less
	numbots--;
	//everything went ok
	return qtrue;
}

/*
==============
BotResetState

called when a bot enters the intermission or observer mode and
when the level is changed
==============
*/
void BotResetState(bot_state_t *bs) {
	int client, entitynum, inuse;
	int movestate, goalstate, chatstate, weaponstate;
	bot_settings_t settings;
	int character;
	playerState_t ps;							//current player state
	float entergame_time;

	//save some things that should not be reset here
	memcpy(&settings, &bs->settings, sizeof(bot_settings_t));
	memcpy(&ps, &bs->cur_ps, sizeof(playerState_t));
	inuse = bs->inuse;
	client = bs->client;
	entitynum = bs->entitynum;
	character = bs->character;
	movestate = bs->ms;
	goalstate = bs->gs;
	chatstate = bs->cs;
	weaponstate = bs->ws;
	entergame_time = bs->entergame_time;
	//reset the whole state
	memset(bs, 0, sizeof(bot_state_t));
	//copy back some state stuff that should not be reset
	bs->ms = movestate;
	bs->gs = goalstate;
	bs->cs = chatstate;
	bs->ws = weaponstate;
	memcpy(&bs->cur_ps, &ps, sizeof(playerState_t));
	memcpy(&bs->settings, &settings, sizeof(bot_settings_t));
	bs->inuse = inuse;
	bs->client = client;
	bs->entitynum = entitynum;
	bs->character = character;
	bs->entergame_time = entergame_time;
	//reset several states
	if (bs->ms) trap_BotResetMoveState(bs->ms);
	if (bs->gs) trap_BotResetGoalState(bs->gs);
	if (bs->ws) trap_BotResetWeaponState(bs->ws);
	if (bs->gs) trap_BotResetAvoidGoals(bs->gs);
	if (bs->ms) trap_BotResetAvoidReach(bs->ms);
}

/*
==============
BotAILoadMap
==============
*/
int BotAILoadMap( int restart ) {
	int			i;
	vmCvar_t	mapname;

	if (!restart) {
		trap_Cvar_Register( &mapname, "mapname", "", CVAR_SERVERINFO | CVAR_ROM );
		if ( trap_BotLibLoadMap( mapname.string ) ) {
			return qfalse;
		}
	}

	for (i = 0; i < MAX_CLIENTS; i++) {
		if (botstates[i] && botstates[i]->inuse) {
			BotResetState( botstates[i] );
			botstates[i]->setupcount = 4;
		}
	}

	BotSetupDeathmatchAI();

	return qtrue;
}

gentity_t* BotFindHumanPlayer(void){
int i;
	for(i=0; i<level.maxclients;i++){
		if(level.clients[i].pers.connected != CON_CONNECTED )
			continue;
		if( g_entities[i].r.svFlags & SVF_BOT )
			continue;
		return &g_entities[i];
	}
	return NULL;
}

int ScanForCrossHairPlayer( gentity_t* observer ) {
	trace_t		trace;
	vec3_t		start, forward, end;
	int			entnum =  observer - g_entities;

	VectorCopy( observer->s.pos.trBase, start );
	AngleVectors( observer->client->ps.viewangles, forward, NULL, NULL );

	VectorMA( start, 131072, forward, end );

	trap_Trace( &trace, start, NULL, NULL, end, entnum, CONTENTS_BODY );
	if ( trace.entityNum < MAX_CLIENTS ) 
		return trace.entityNum;
	return -1;
}

// maintains bot_state_t::observed for all bots
// find a human player, then find a bot the human player might be interested in
void BotAIObserve( void ){
	gentity_t* observer;
	int observedId;
	static int lastobserved = -1;
	int i;
	
	if(!bot_developer.integer){
		trap_SetConfigstring( CS_BOTINFO, '\0');
		return;
	}

	// determine bot to observe, if any
	observedId = -1;
	observer = BotFindHumanPlayer();
	
	if( !observer ){
		trap_SetConfigstring( CS_BOTINFO, '\0');
		return;
	}		

	// specing anyone?
	if (	observer->client->sess.sessionTeam == TEAM_SPECTATOR &&
			observer->client->sess.spectatorState == SPECTATOR_FOLLOW )
	{
		observedId = observer->client->sess.spectatorClient;
	}
	else{	// playing or freecam spectator, check the aim target
		observedId = ScanForCrossHairPlayer( observer );
		
		// none found? fall back to backup
		if(observedId == -1 && lastobserved != -1)
			observedId = lastobserved;
		// do backup
		lastobserved = observedId;
	}	

	// tell botlib	
	trap_BotLibVarSet( "debugclient", va("%d", observedId) );
	
	// update all bots
	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if( !botstates[i] || !botstates[i]->inuse )
			continue;
		botstates[i]->observed = (i == observedId) ? qtrue : qfalse;
	}
}

/*
==================
BotAIStartFrame
==================
*/
int BotAIStartFrame(int time) {
	int i;
	gentity_t	*ent;
	bot_entitystate_t state;
	int elapsed_time, thinktime;
	static int local_time;
	static int botlib_residual;
	static int lastbotthink_time;

	G_CheckBotSpawn();

	trap_Cvar_Update(&bot_developer);	// cyr
	if(bot_developer.integer == 2 ){	// cyr debug info hud
		//BotUpdateInfoConfigStrings();
		trap_BotLibVarSet("showitemweights","0");
		trap_BotLibVarSet("showweaponweights","0");
	}
	else if(bot_developer.integer == 3 ){
		trap_BotLibVarSet("showitemweights","1");
		trap_BotLibVarSet("showweaponweights","0");
		
	}
	else if(bot_developer.integer == 4 ){
		trap_BotLibVarSet("showweaponweights","1");	
		trap_BotLibVarSet("showitemweights","0");
	}

	trap_Cvar_Update(&bot_rocketjump);
	trap_Cvar_Update(&bot_grapple);
	trap_Cvar_Update(&bot_fastchat);
	trap_Cvar_Update(&bot_nochat);
	trap_Cvar_Update(&bot_testrchat);
	trap_Cvar_Update(&bot_thinktime);
	trap_Cvar_Update(&bot_memorydump);
	trap_Cvar_Update(&bot_saveroutingcache);
	trap_Cvar_Update(&bot_pause);
// cyr {
	trap_Cvar_Update(&bot_showreachesfrom);
	trap_Cvar_Update(&bot_showreachesto);
	trap_Cvar_Update(&bot_roamfactor);
	trap_Cvar_Update(&bot_shownextitem);
	trap_Cvar_Update(&bot_shownoitem);
	trap_Cvar_Update(&bot_cachetest);
	
	if(bot_cachetest.integer){
		trap_BotLibVarSet("cachetest","1");			
		trap_Cvar_Set("bot_cachetest", "0");
	}

		
	if(bot_shownextitem.integer){
		trap_BotLibVarSet("shownextitem","1");			
		trap_Cvar_Set("bot_shownextitem", "0");
	}

	if(bot_shownoitem.integer){
		trap_BotLibVarSet("shownoitem","1");
		trap_BotInitLevelItems();
		trap_Cvar_Set("bot_shownoitem", "0");
	}

	// copy values to botlib
	trap_BotLibVarSet("showreachesfrom", va("%d",bot_showreachesfrom.integer) );
	trap_BotLibVarSet("showreachesto",   va("%d",bot_showreachesto.integer)   );
	trap_BotLibVarSet("roamfactor",   va("%d",bot_roamfactor.integer)   );

	if(bot_developer.integer & AIDBG_ROUTES)
		trap_BotLibVarSet("showbotroutes", "1" );	// evt bot client id uebergeben und in movetogoal mit ms->client checken

// cyr }
	if (bot_pause.integer) {
		// execute bot user commands every frame
		for( i = 0; i < MAX_CLIENTS; i++ ) {
			if( !botstates[i] || !botstates[i]->inuse ) {
				continue;
			}
			if( g_entities[i].client->pers.connected != CON_CONNECTED ) {
				continue;
			}
			botstates[i]->lastucmd.forwardmove = 0;
			botstates[i]->lastucmd.rightmove = 0;
			botstates[i]->lastucmd.upmove = 0;
			botstates[i]->lastucmd.buttons = 0;
			botstates[i]->lastucmd.serverTime = time;
			trap_BotUserCommand(botstates[i]->client, &botstates[i]->lastucmd);
		}
		return qtrue;
	}

	if (bot_memorydump.integer) {
		trap_BotLibVarSet("memorydump", "1");
		trap_Cvar_Set("bot_memorydump", "0");
		G_Printf("bstates %lu one state %lu ratio %f\n", sizeof(botstates), sizeof(bot_state_t), (double) sizeof(botstates)/sizeof(bot_state_t));
	}
	if (bot_saveroutingcache.integer) {
		trap_BotLibVarSet("saveroutingcache", "1");
		trap_Cvar_Set("bot_saveroutingcache", "0");
	}

	//check if bot interbreeding is activated
	BotInterbreeding();
	//cap the bot think time
	if (bot_thinktime.integer > 200) {
		trap_Cvar_Set("bot_thinktime", "200");
	}
	//if the bot think time changed we should reschedule the bots
	if (bot_thinktime.integer != lastbotthink_time) {
		lastbotthink_time = bot_thinktime.integer;
		BotScheduleBotThink();
	}

	elapsed_time = time - local_time;
	local_time = time;

	botlib_residual += elapsed_time;

	if (elapsed_time > bot_thinktime.integer) thinktime = elapsed_time;
	else thinktime = bot_thinktime.integer;

	// update the bot library
	if ( botlib_residual >= thinktime ) {
		botlib_residual -= thinktime;

		trap_BotLibStartFrame((float) time / 1000);

		if (!trap_AAS_Initialized()) return qfalse;

		//update entities in the botlib
		for (i = 0; i < MAX_GENTITIES; i++) {
			ent = &g_entities[i];
			if (!ent->inuse) {
				trap_BotLibUpdateEntity(i, NULL);
				continue;
			}
			if (!ent->r.linked) {
				trap_BotLibUpdateEntity(i, NULL);
				continue;
			}
			if (ent->r.svFlags & SVF_NOCLIENT) {
				trap_BotLibUpdateEntity(i, NULL);
				continue;
			}
			// do not update missiles	// cyr, but do update ducks !
			if (ent->s.eType == ET_MISSILE && ent->s.weapon != WP_KILLERDUCKS) {
				trap_BotLibUpdateEntity(i, NULL);
				continue;
			}
			// do not update event only entities
			if (ent->s.eType > ET_EVENTS) {
				trap_BotLibUpdateEntity(i, NULL);
				continue;
			}
			//
			memset(&state, 0, sizeof(bot_entitystate_t));
			//
			VectorCopy(ent->r.currentOrigin, state.origin);
			if (i < MAX_CLIENTS) {
				VectorCopy(ent->s.apos.trBase, state.angles);
			} else {
				VectorCopy(ent->r.currentAngles, state.angles);
			}
			VectorCopy(ent->s.origin2, state.old_origin);
			VectorCopy(ent->r.mins, state.mins);
			VectorCopy(ent->r.maxs, state.maxs);
			state.type = ent->s.eType;
			state.flags = ent->s.eFlags;
			if (ent->r.bmodel) state.solid = SOLID_BSP;
			else state.solid = SOLID_BBOX;
			state.groundent = ent->s.groundEntityNum;
			state.modelindex = ent->s.modelindex;
			state.modelindex2 = ent->s.modelindex2;
			state.frame = ent->s.frame;
			state.event = ent->s.event;
			state.eventParm = ent->s.eventParm;
			state.powerups = ent->s.powerups;
			state.legsAnim = ent->s.legsAnim;
			state.torsoAnim = ent->s.torsoAnim;
			state.weapon = ent->s.weapon;
			//
			trap_BotLibUpdateEntity(i, &state);
		}

		BotAIRegularUpdate();
	}

	floattime = trap_AAS_Time();

	BotAIObserve();	// choose a bot to observe
	
	// execute scheduled bot AI
	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if( !botstates[i] || !botstates[i]->inuse ) {
			continue;
		}
	
		//
		botstates[i]->botthink_residual += elapsed_time;
		//
		if ( botstates[i]->botthink_residual >= thinktime ) {
			botstates[i]->botthink_residual -= thinktime;

			if (!trap_AAS_Initialized()) return qfalse;

			if (g_entities[i].client->pers.connected == CON_CONNECTED) {
				botstates[i]->frametime = time;		// cyr, clock frame duration
				BotAI(i, (float) thinktime / 1000);
			}
		}
	}

	//DeleteDebugLines();

	// execute bot user commands every frame
	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if( !botstates[i] || !botstates[i]->inuse ) {
			continue;
		}
		if( g_entities[i].client->pers.connected != CON_CONNECTED ) {
			continue;
		}

		//BotShowViewAngles(botstates[i]);

		BotUpdateInput(botstates[i], time, elapsed_time);
		trap_BotUserCommand(botstates[i]->client, &botstates[i]->lastucmd);
	}

	return qtrue;
}

/*
==============
BotInitLibrary
==============
*/
int BotInitLibrary(void) {
	char buf[144];

	//set the maxclients and maxentities library variables before calling BotSetupLibrary
	trap_Cvar_VariableStringBuffer("sv_maxclients", buf, sizeof(buf));
	if (!strlen(buf)) strcpy(buf, "8");
	trap_BotLibVarSet("maxclients", buf);
	Com_sprintf(buf, sizeof(buf), "%d", MAX_GENTITIES);
	trap_BotLibVarSet("maxentities", buf);
	//bsp checksum
	trap_Cvar_VariableStringBuffer("sv_mapChecksum", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("sv_mapChecksum", buf);
	//maximum number of aas links
	trap_Cvar_VariableStringBuffer("max_aaslinks", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("max_aaslinks", buf);
	//maximum number of items in a level
	trap_Cvar_VariableStringBuffer("max_levelitems", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("max_levelitems", buf);
	//game type
	trap_Cvar_VariableStringBuffer("g_gametype", buf, sizeof(buf));
	if (!strlen(buf)) strcpy(buf, "0");
	trap_BotLibVarSet("g_gametype", buf);
	//bot developer mode and log file
	trap_BotLibVarSet("bot_developer", bot_developer.string);
	trap_Cvar_VariableStringBuffer("logfile", buf, sizeof(buf));
	trap_BotLibVarSet("log", buf);
	//no chatting
	trap_Cvar_VariableStringBuffer("bot_nochat", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("nochat", "0");
	//visualize jump pads
	trap_Cvar_VariableStringBuffer("bot_visualizejumppads", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("bot_visualizejumppads", buf);
	//forced clustering calculations
	trap_Cvar_VariableStringBuffer("bot_forceclustering", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("forceclustering", buf);
	//forced reachability calculations
	trap_Cvar_VariableStringBuffer("bot_forcereachability", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("forcereachability", buf);
	//force writing of AAS to file
	trap_Cvar_VariableStringBuffer("bot_forcewrite", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("forcewrite", buf);
	//no AAS optimization
	trap_Cvar_VariableStringBuffer("bot_aasoptimize", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("aasoptimize", buf);
	//
	trap_Cvar_VariableStringBuffer("bot_saveroutingcache", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("saveroutingcache", buf);
	//reload instead of cache bot character files
	trap_Cvar_VariableStringBuffer("bot_reloadcharacters", buf, sizeof(buf));
	if (!strlen(buf)) strcpy(buf, "0");
	trap_BotLibVarSet("bot_reloadcharacters", buf);
	//base directory
	trap_Cvar_VariableStringBuffer("fs_basepath", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("basedir", buf);
	//game directory
	trap_Cvar_VariableStringBuffer("fs_game", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("gamedir", buf);
	//cd directory
	trap_Cvar_VariableStringBuffer("fs_cdpath", buf, sizeof(buf));
	if (strlen(buf)) trap_BotLibVarSet("cddir", buf);
	//setup the bot library
	return trap_BotLibSetup();
}

/*
==============
BotAISetup
==============
*/
int BotAISetup( int restart ) {
	int			errnum;

	trap_Cvar_Register(&bot_thinktime, "bot_thinktime", "100", CVAR_CHEAT);
	trap_Cvar_Register(&bot_memorydump, "bot_memorydump", "0", CVAR_CHEAT);
	trap_Cvar_Register(&bot_saveroutingcache, "bot_saveroutingcache", "0", CVAR_CHEAT);
	trap_Cvar_Register(&bot_pause, "bot_pause", "0", CVAR_CHEAT);
	trap_Cvar_Register(&bot_testsolid, "bot_testsolid", "0", CVAR_CHEAT);
	trap_Cvar_Register(&bot_testclusters, "bot_testclusters", "0", CVAR_CHEAT);
	trap_Cvar_Register(&bot_developer, "bot_developer", "0", CVAR_CHEAT);
	trap_Cvar_Register(&bot_interbreedchar, "bot_interbreedchar", "", 0);
	trap_Cvar_Register(&bot_interbreedbots, "bot_interbreedbots", "10", 0);
	trap_Cvar_Register(&bot_interbreedcycle, "bot_interbreedcycle", "20", 0);
	trap_Cvar_Register(&bot_interbreedwrite, "bot_interbreedwrite", "", 0);
	// cyr
	trap_Cvar_Register(&bot_cachetest, "bot_cachetest", "0", 0);	
	trap_Cvar_Register(&bot_roamfactor, "bot_roamfactor", "1", 0);
	trap_Cvar_Register(&bot_shownextitem, "bot_shownextitem", "0", 0);
	trap_Cvar_Register(&bot_shownoitem, "bot_shownoitem", "0", 0);
	trap_Cvar_Register(&bot_showreachesfrom, "bot_showreachesfrom", "0", 0);
	trap_Cvar_Register(&bot_showreachesto, "bot_showreachesto", "0", 0);

	//if the game is restarted for a tournament
	if (restart) {
		return qtrue;
	}

	//initialize the bot states
	memset( botstates, 0, sizeof(botstates) );

	ResetWaypoints();

	errnum = BotInitLibrary();
	if (errnum != BLERR_NOERROR) return qfalse;
	return qtrue;
}

/*
==============
BotAIShutdown
==============
*/
int BotAIShutdown( int restart ) {

	int i;

	//if the game is restarted for a tournament
	if ( restart ) {
		//shutdown all the bots in the botlib
		for (i = 0; i < MAX_CLIENTS; i++) {
			if (botstates[i] && botstates[i]->inuse) {
				BotAIShutdownClient(botstates[i]->client, restart);
			}
		}
		//don't shutdown the bot library
	}
	else {
		trap_BotLibShutdown();
	}
	return qtrue;
}
