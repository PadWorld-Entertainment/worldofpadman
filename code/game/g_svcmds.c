// Copyright (C) 1999-2000 Id Software, Inc.
//

// this file holds commands that can be executed by the server console, but not remote clients

#include "g_local.h"
#include "wopg_sphandling.h"


/*
==============================================================================

PACKET FILTERING
 

You can add or remove addresses from the filter list with:

addip <ip>
removeip <ip>

The ip address is specified in dot format, and you can use '*' to match any value
so you can specify an entire class C network with "addip 192.246.40.*"

Removeip will only remove an address specified exactly the same way.  You cannot addip a subnet, then removeip a single host.

listip
Prints the current list of filters.

g_filterban <0 or 1>

If 1 (the default), then ip addresses matching the current list will be prohibited from entering the game.  This is the default setting.

If 0, then only addresses matching the list will be allowed.  This lets you easily set up a private game, or a game that only allows players from your local network.

TTimo NOTE: for persistence, bans are stored in g_banIPs cvar MAX_CVAR_VALUE_STRING
The size of the cvar string buffer is limiting the banning to around 20 masks
this could be improved by putting some g_banIPs2 g_banIps3 etc. maybe
still, you should rely on PB for banning instead

==============================================================================
*/

typedef struct ipFilter_s
{
	unsigned	mask;
	unsigned	compare;
} ipFilter_t;

#define	MAX_IPFILTERS	1024

static ipFilter_t	ipFilters[MAX_IPFILTERS];
static int			numIPFilters;

/*
=================
StringToFilter
=================
*/
static qboolean StringToFilter (char *s, ipFilter_t *f)
{
	char	num[128];
	int		i, j;
	byte	b[4];
	byte	m[4];
	
	for (i=0 ; i<4 ; i++)
	{
		b[i] = 0;
		m[i] = 0;
	}
	
	for (i=0 ; i<4 ; i++)
	{
		if (*s < '0' || *s > '9')
		{
			if (*s == '*') // 'match any'
			{
				// b[i] and m[i] to 0
				s++;
				if (!*s)
					break;
				s++;
				continue;
			}
			G_Printf( "Bad filter address: %s\n", s );
			return qfalse;
		}
		
		j = 0;
		while (*s >= '0' && *s <= '9')
		{
			num[j++] = *s++;
		}
		num[j] = 0;
		b[i] = atoi(num);
		m[i] = 255;

		if (!*s)
			break;
		s++;
	}
	
	f->mask = *(unsigned *)m;
	f->compare = *(unsigned *)b;
	
	return qtrue;
}

/*
=================
UpdateIPBans
=================
*/
static void UpdateIPBans (void)
{
	byte	b[4];
	byte	m[4];
	int		i,j;
	char	iplist_final[MAX_CVAR_VALUE_STRING];
	char	ip[64];

	*iplist_final = 0;
	for (i = 0 ; i < numIPFilters ; i++)
	{
		if (ipFilters[i].compare == 0xffffffff)
			continue;

		*(unsigned *)b = ipFilters[i].compare;
		*(unsigned *)m = ipFilters[i].mask;
		*ip = 0;
		for (j = 0 ; j < 4 ; j++)
		{
			if (m[j]!=255)
				Q_strcat(ip, sizeof(ip), "*");
			else
				Q_strcat(ip, sizeof(ip), va("%i", b[j]));
			Q_strcat(ip, sizeof(ip), (j<3) ? "." : " ");
		}		
		if (strlen(iplist_final)+strlen(ip) < MAX_CVAR_VALUE_STRING - 1)
		{
			Q_strcat( iplist_final, sizeof(iplist_final), ip);
		}
		else
		{
			Com_Printf("g_banIPs overflowed at MAX_CVAR_VALUE_STRING\n");
			break;
		}
	}

	trap_Cvar_Set( "g_banIPs", iplist_final );
}

/*
=================
G_FilterPacket
=================
*/
qboolean G_FilterPacket (char *from)
{
	int		i;
	unsigned	in;
	byte m[4];
	char *p;

	i = 0;
	p = from;
	while (*p && i < 4) {
		m[i] = 0;
		while (*p >= '0' && *p <= '9') {
			m[i] = m[i]*10 + (*p - '0');
			p++;
		}
		if (!*p || *p == ':')
			break;
		i++, p++;
	}
	
	in = *(unsigned *)m;

	for (i=0 ; i<numIPFilters ; i++)
		if ( (in & ipFilters[i].mask) == ipFilters[i].compare)
			return g_filterBan.integer != 0;

	return g_filterBan.integer == 0;
}

/*
=================
AddIP
=================
*/
static void AddIP( char *str )
{
	int		i;

	for (i = 0 ; i < numIPFilters ; i++)
		if (ipFilters[i].compare == 0xffffffff)
			break;		// free spot
	if (i == numIPFilters)
	{
		if (numIPFilters == MAX_IPFILTERS)
		{
			G_Printf ("IP filter list is full\n");
			return;
		}
		numIPFilters++;
	}
	
	if (!StringToFilter (str, &ipFilters[i]))
		ipFilters[i].compare = 0xffffffffu;

	UpdateIPBans();
}

/*
=================
G_ProcessIPBans
=================
*/
void G_ProcessIPBans(void) 
{
	char *s, *t;
	char		str[MAX_CVAR_VALUE_STRING];

	Q_strncpyz( str, g_banIPs.string, sizeof(str) );

	for (t = s = g_banIPs.string; *t; /* */ ) {
		s = strchr(s, ' ');
		if (!s)
			break;
		while (*s == ' ')
			*s++ = 0;
		if (*t)
			AddIP( t );
		t = s;
	}
}


/*
=================
Svcmd_AddIP_f
=================
*/
void Svcmd_AddIP_f (void)
{
	char		str[MAX_TOKEN_CHARS];

	if ( trap_Argc() < 2 ) {
		G_Printf("Usage:  addip <ip-mask>\n");
		return;
	}

	trap_Argv( 1, str, sizeof( str ) );

	AddIP( str );

}

/*
=================
Svcmd_RemoveIP_f
=================
*/
void Svcmd_RemoveIP_f (void)
{
	ipFilter_t	f;
	int			i;
	char		str[MAX_TOKEN_CHARS];

	if ( trap_Argc() < 2 ) {
		G_Printf("Usage:  sv removeip <ip-mask>\n");
		return;
	}

	trap_Argv( 1, str, sizeof( str ) );

	if (!StringToFilter (str, &f))
		return;

	for (i=0 ; i<numIPFilters ; i++) {
		if (ipFilters[i].mask == f.mask	&&
			ipFilters[i].compare == f.compare) {
			ipFilters[i].compare = 0xffffffffu;
			G_Printf ("Removed.\n");

			UpdateIPBans();
			return;
		}
	}

	G_Printf ( "Didn't find %s.\n", str );
}

/*
===================
Svcmd_EntityList_f
===================
*/
void	Svcmd_EntityList_f (void) {
	int			e;
	gentity_t		*check;

	check = g_entities+1;
	for (e = 1; e < level.num_entities ; e++, check++) {
		if ( !check->inuse ) {
			continue;
		}
		G_Printf("%3i:", e);
		switch ( check->s.eType ) {
		case ET_GENERAL:
			G_Printf("ET_GENERAL          ");
			break;
		case ET_PLAYER:
			G_Printf("ET_PLAYER           ");
			break;
		case ET_ITEM:
			G_Printf("ET_ITEM             ");
			break;
		case ET_MISSILE:
			G_Printf("ET_MISSILE          ");
			break;
		case ET_MOVER:
			G_Printf("ET_MOVER            ");
			break;
		case ET_BEAM:
			G_Printf("ET_BEAM             ");
			break;
		case ET_PORTAL:
			G_Printf("ET_PORTAL           ");
			break;
		case ET_SPEAKER:
			G_Printf("ET_SPEAKER          ");
			break;
		case ET_PUSH_TRIGGER:
			G_Printf("ET_PUSH_TRIGGER     ");
			break;
		case ET_TELEPORT_TRIGGER:
			G_Printf("ET_TELEPORT_TRIGGER ");
			break;
		case ET_INVISIBLE:
			G_Printf("ET_INVISIBLE        ");
			break;
		case ET_GRAPPLE:
			G_Printf("ET_GRAPPLE          ");
			break;
		default:
			G_Printf("%3i                 ", check->s.eType);
			break;
		}

		if ( check->classname ) {
			G_Printf("%s", check->classname);
		}
		G_Printf("\n");
	}
}

gclient_t	*ClientForString( const char *s ) {
	gclient_t	*cl;
	int			i;
	int			idnum;
	char		netname[1024];

	// numeric values are just slot numbers
	if ( s[0] >= '0' && s[0] <= '9' ) {
		idnum = atoi( s );
		if ( idnum < 0 || idnum >= level.maxclients ) {
			Com_Printf( "Bad client slot: %i\n", idnum );
			return NULL;
		}

		cl = &level.clients[idnum];
		if ( cl->pers.connected == CON_DISCONNECTED ) {
			G_Printf( "Client %i is not connected\n", idnum );
			return NULL;
		}
		return cl;
	}

	// check for a name match
	for ( i=0 ; i < level.maxclients ; i++ ) {
		cl = &level.clients[i];
		if ( cl->pers.connected == CON_DISCONNECTED ) {
			continue;
		}
		Q_strncpyz(netname, cl->pers.netname, 1024);
		Q_CleanStr(netname);
		if ( !Q_stricmp( netname, s ) ) {
			return cl;
		}
		else
			G_Printf("missmatch %s <-> %s \n", netname, s);
	}

	G_Printf( "User %s is not on the server\n", s );

	return NULL;
}


/*
===================
Svcmd_Say_f

ssay <text>
===================
*/
static void Svcmd_Say_f( void ) {
	if ( trap_Argc() < 2 ) {
		G_Printf( "usage: ssay <text>\n" );
		return;
	}

	G_Say( NULL, NULL, SAY_ALL, ConcatArgs( 1 ) );
}


/*
===================
Svcmd_Tell_f

stell <cid> <text>
===================
*/
static void Svcmd_Tell_f( void ) {
	char str[3];
	int cid;
	gentity_t* target;

	if ( trap_Argc() < 3 ) {
		G_Printf( "usage: stell <cid> <text>\n" );
		return;
	}

	trap_Argv( 1, str, sizeof( str ) );
	cid = atoi( str );

	if ( ( cid < 0 ) || ( cid >= MAX_CLIENTS ) ) {
		G_Printf( "Not a valid client number.\n" );
		return;
	}

	target = ( g_entities + cid );

	if ( target->client->pers.connected != CON_CONNECTED ) {
		G_Printf( "Client not connected.\n" );
		return;
	}

	G_Say( NULL, target, SAY_TELL, ConcatArgs( 2 ) );
}


typedef enum {
	CCMD_MP,
	CCMD_CP,
	CCMD_PRINT
} clientCommand_t;
/*
===================
Svcmd_ClientCommand_f

<cmd> <cid> <text>
===================
*/
static void Svcmd_ClientCommand_f( clientCommand_t cmd ) {
	char str[3], *cmdstr;
	int cid;

	if ( trap_Argc() < 3 ) {
		// FIXME: Use real command in error message
		G_Printf( "usage: $cmd <cid> <text>\n" );
		return;
	}

	switch ( cmd ) {
		case CCMD_MP:
			cmdstr = "mp";
			break;
		case CCMD_CP:
			cmdstr = "cp";
			break;
		case CCMD_PRINT:
			cmdstr = "print";
			break;

		default:
			cmdstr = NULL;
			G_Error( "Svcmd_ClientCommand_f: cmd out of clientCommand_t range!\n" );
	}

	trap_Argv( 1, str, sizeof( str ) );
	cid = atoi( str );

	if ( ( ( cid < 0 ) || ( cid >= MAX_CLIENTS ) ) && ( cid != -1 ) ) {
		G_Printf( "Not a valid client number.\n" );
		return;
	}

	if ( (cid != -1 ) && ( level.clients[cid].pers.connected != CON_CONNECTED ) ) {
		G_Printf( "Client not connected.\n" );
		return;
	}

	trap_SendServerCommand( cid, va( "%s \"%s\"" , cmdstr, ConcatArgs( 2 ) ) );
}


/*
===================
Svcmd_ForceTeam_f

forceteam <player> <team>
===================
*/
void	Svcmd_ForceTeam_f( void ) {
	gclient_t	*cl;
	char		str[MAX_TOKEN_CHARS];

	// find the player
	trap_Argv( 1, str, sizeof( str ) );
	cl = ClientForString( str );
	if ( !cl ) {
		return;
	}

	// set the team
	trap_Argv( 2, str, sizeof( str ) );
	SetTeam( &g_entities[cl - level.clients], str );
}


static void Svcmd_SetGameType_f(void)
{
	char* argStr = ConcatArgs(1);
	int gt = convertGTStringToGTNumber(argStr);

	if(gt!=-1)
	{
//		trap_Cvar_Set("g_gametype",va("%d",gt)); // doesn't use cvar-latch !?
		trap_SendConsoleCommand( EXEC_APPEND, va("g_gametype %d\n", gt ) );
//		Com_Printf("gametype = %d\n",gt);
	}
	else // ToDo?: msg also to the clients?
		Com_Printf("Error: Couldn't find a GameType with \"%s\"\n",argStr);
}

void Svcmd_StopCam( void ){
	level.cammode = qfalse;
	trap_SendConsoleCommand( EXEC_APPEND, "wopSP_introFinished\n" );
}

qboolean FileExists(char* fname){
	fileHandle_t	f;
	trap_FS_FOpenFile( fname, &f, FS_READ );
	if ( !f )
		return qfalse;
	else{
		trap_FS_FCloseFile( f );
		return qtrue;
	}
}

void Svcmd_StartCam( void ){
	char	map[MAX_QPATH];
	char	serverinfo[MAX_INFO_STRING];
	char	path[MAX_QPATH];
	
	trap_GetServerinfo( serverinfo, sizeof(serverinfo) );
	Q_strncpyz( map, Info_ValueForKey( serverinfo, "mapname" ), sizeof(map) );
	Com_sprintf( path, sizeof(path), "cutscenes\\%s\\scene.cfg", map);
	if( !FileExists(path) ){
		G_Printf("%s does not exist\n", path);
		return;
	}
	trap_SendConsoleCommand( EXEC_APPEND, va("exec \"%s\"\n",path) );
	level.cammode = qtrue;
}

void FreezePlayers( qboolean on ){
	int i;
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected != CON_CONNECTED )
			continue;
		if ( g_entities[i].r.svFlags & SVF_BOT )
			continue;
		g_entities[i].client->ps.pm_type = ( on ) ? PM_FREEZE : PM_NORMAL;
	}
}

void EditPlayerInventory(gentity_t *ent, int arg_offset);

void Svcmd_CamCmd( void ){
	char buf[MAX_TOKEN_CHARS];
	char cmd[MAX_TOKEN_CHARS];
	char name[MAX_TOKEN_CHARS];
	int i;
	gclient_t* cl;
	
	if( !level.cammode ){
		return;
	}

	if( trap_Argc() < 2 ){
		return;	
	}

	trap_Argv( 1, cmd, sizeof(cmd) );

	if ( !Q_stricmp (cmd, "print")  ) 
	{
		trap_Argv( 2, buf, sizeof(buf) );
		trap_SendServerCommand( -1, va("cp \"%s\n\"", buf ) );
	}
	else if( !Q_stricmp (cmd, "setclientpos") ) {
		vec3_t	newOrigin;

		if( trap_Argc() != 8 && trap_Argc() != 6 ){
			Com_Printf("usage: camcmd setclientpos name/id x y z (a b) \na = PITCH-angle, b = YAW-angle\n");
			return;
		}

		trap_Argv( 2, name, sizeof( name ) );

		cl = ClientForString( name ) ;
		if(!cl) 
			return;

		for( i=0; i<3; i++){
			trap_Argv( i+3, buf, sizeof( buf) );
			newOrigin[i] = atof( buf );
		}

		G_SetOrigin( &g_entities[cl->ps.clientNum], newOrigin );
		VectorCopy( newOrigin, cl->ps.origin );

		if(trap_Argc() == 8) {
			vec3_t	newAngles;

			memset(newAngles,0,sizeof(newAngles));
			trap_Argv( 6, buf, sizeof( buf ) );
			newAngles[PITCH] = atoi(buf);
			trap_Argv( 7, buf, sizeof( buf ) );
			newAngles[YAW] = atoi(buf);

			SetClientViewAngle( &g_entities[cl->ps.clientNum], newAngles );
		}
	}
	else if( !Q_stricmp (cmd, "setspawn")  )
	{
		if( trap_Argc() != 8 ){
			Com_Printf("usage: camcmd setspawn x y z a b c \n");
			return;
		}

		for( i=0; i<3; i++){
			trap_Argv( i+2, buf, sizeof(buf) );
			level.cam_spawnpos[i] = atof(buf);
		}
		for( i=0; i<3; i++){
			trap_Argv( i+5, buf, sizeof(buf) );
			level.cam_spawnangles[i] = atof(buf);
		}
	}
	else if( !Q_stricmp (cmd, "botmove")  )
	{
		vec3_t pos;

		if( trap_Argc() != 6 ){
			Com_Printf("usage: camcmd botmove name x y z \n");
			return;
		}

		trap_Argv( 2, name, sizeof( name ) );

		for( i=0; i<3; i++){
			trap_Argv( i+3, buf, sizeof( buf) );
			pos[i] = atof( buf );
		}

		cl = ClientForString( name ) ;
		if(!cl) 
			return;
		BotCamMoveTo( cl->ps.clientNum, pos );
	}
	else if( !Q_stricmp (cmd, "botviewangles")  )
	{
		vec3_t angles;

		if( trap_Argc() != 5 ){
			Com_Printf("usage: camcmd botviewangles name x y \n");
			return;
		}

		trap_Argv( 2, name, sizeof( name ) );
		cl = ClientForString( name ) ;
		if(!cl) 
			return;
		trap_Argv( 3, buf, sizeof( buf ) );
		angles[PITCH] = atoi(buf);
		trap_Argv( 4, buf, sizeof( buf ) );
		angles[YAW] = atoi(buf);

		BotCamViewangles( cl->ps.clientNum, angles );
	}
	else if( !Q_stricmp (cmd, "botviewtarget")  )
	{
		vec3_t target;

		if( trap_Argc() != 6 ){
			Com_Printf("usage: camcmd botviewtarget name x y z \n");
			return;
		}

		trap_Argv( 2, name, sizeof( name ) );
		cl = ClientForString( name ) ;
		if(!cl) 
			return;
		trap_Argv( 3, buf, sizeof( buf ) );
		target[0] = atoi(buf);
		trap_Argv( 4, buf, sizeof( buf ) );
		target[1] = atoi(buf);
		trap_Argv( 5, buf, sizeof( buf ) );
		target[2] = atoi(buf);

		BotCamViewTarget( cl->ps.clientNum, target );
	}
	else if( !Q_stricmp (cmd, "botviewentity")  )
	{
//		int target_bot;
		gclient_t* target_cl;
		if( trap_Argc() != 4 ){
			Com_Printf("usage: camcmd botviewentity name targetname \n");
			return;
		}

		trap_Argv( 2, name, sizeof( name ) );
		cl = ClientForString( name ) ;
		if(!cl) 
			return;

		trap_Argv( 3, buf, sizeof( buf ) );
		target_cl = ClientForString( buf );
		if(!target_cl)
			return;
		BotCamViewEntitiy( cl->ps.clientNum, target_cl->ps.clientNum );

	}
	else if( !Q_stricmp (cmd, "boteditinv")  )
	{
		if( trap_Argc() < 3) 
			return;
		trap_Argv( 2, name, sizeof(name) );
		cl = ClientForString( name );
		if( !cl ) return;
		EditPlayerInventory( &g_entities[cl->ps.clientNum], 3 );
	}
	else if( !Q_stricmp (cmd, "botchooseweap")  )
	{
		if( trap_Argc() != 4 ){
			Com_Printf("usage: camcmd botchooseweap name weapID \n");
			return;
		}
		trap_Argv( 2, name, sizeof( name ) );
		cl = ClientForString( name ) ;
		if(!cl) 
			return;
		trap_Argv( 3, buf, sizeof( buf ) );
		BotChooseWeap( cl->ps.clientNum, atoi(buf) );
	}
	else if( !Q_stricmp (cmd, "bottaunt")  )
	{
		if( trap_Argc() != 3 ){
			Com_Printf("usage: camcmd bottaunt name \n");
			return;
		}
		trap_Argv( 2, name, sizeof( name ) );
		cl = ClientForString( name ) ;
		if(!cl) 
			return;

		BotCamTaunt( cl->ps.clientNum );
	}
	else if( !Q_stricmp(cmd, "botfire") )
	{
		if( trap_Argc() != 3 ){
			Com_Printf("usage: camcmd botfire name \n");
			return;
		}
		trap_Argv( 2, name, sizeof( name ) );
		cl = ClientForString( name ) ;
		if(!cl) 
			return;

		BotCamFire( cl->ps.clientNum );		
	}
	else if( !Q_stricmp(cmd, "freeze") ){
		int on;

		
		if( trap_Argc() != 3 ){
			Com_Printf("usage: camcmd freeze [0|1] \n");
			return;
		}
		trap_Argv( 2, name, sizeof( name ) );
		
		on = atoi( name );
		FreezePlayers( on );
	}

}

void WaypointInit(void);
/*
=================
ConsoleCommand

=================
*/
qboolean	ConsoleCommand( void ) {
	char	cmd[MAX_TOKEN_CHARS];

	trap_Argv( 0, cmd, sizeof( cmd ) );

	if ( Q_stricmp (cmd, "entitylist") == 0 ) {
		Svcmd_EntityList_f();
		return qtrue;
	}

	if ( Q_stricmp (cmd, "forceteam") == 0 ) {
		Svcmd_ForceTeam_f();
		return qtrue;
	}

	if (Q_stricmp (cmd, "game_memory") == 0) {
		Svcmd_GameMem_f();
		return qtrue;
	}

	if (Q_stricmp (cmd, "addbot") == 0) {
		Svcmd_AddBot_f();
		return qtrue;
	}

	if (Q_stricmp (cmd, "botlist") == 0) {
		Svcmd_BotList_f();
		return qtrue;
	}

	if (Q_stricmp (cmd, "abort_podium") == 0) {
		Svcmd_AbortPodium_f();
		return qtrue;
	}

	if (Q_stricmp (cmd, "addip") == 0) {
		Svcmd_AddIP_f();
		return qtrue;
	}

	if (Q_stricmp (cmd, "removeip") == 0) {
		Svcmd_RemoveIP_f();
		return qtrue;
	}

	if (Q_stricmp (cmd, "listip") == 0) {
		trap_SendConsoleCommand( EXEC_NOW, "g_banIPs\n" );
		return qtrue;
	}

	if(Q_stricmp (cmd, "setGametype") == 0)
	{
		Svcmd_SetGameType_f();
		return qtrue;
	}
	if(Q_stricmp (cmd, "startcam") == 0){
        Svcmd_StartCam();
		return qtrue;
	}
	if(Q_stricmp (cmd, "stopcam") == 0){
        Svcmd_StopCam();
		return qtrue;
	}
	if(Q_stricmp (cmd, "camcmd") == 0){
		Svcmd_CamCmd();
		return qtrue;
	}
	if( !Q_stricmp( cmd, "initwp") ){
		WaypointInit();
		return qtrue;
	}

	if ( Q_stricmp( cmd, "blibset") == 0 ) {
		char key[MAX_TOKEN_CHARS];
		char value[MAX_TOKEN_CHARS];

		trap_Argv(1, key, sizeof(key) );
		trap_Argv(2, value, sizeof(value) );
		if(!strlen(key))
		{
			G_Printf("missing key\n");
			return qtrue;
		}

		if( !strlen(value) )	// use "1" as default
			strcpy( value, "1" );				
		
		trap_BotLibVarSet( key, value );
		return qtrue;
	}

	if(wopSP_cmdCheck(cmd))
		return qtrue;

	if (g_dedicated.integer) {
		if ( Q_stricmp( cmd, "ssay" ) == 0 ) {
			Svcmd_Say_f();
			return qtrue;
		}
		if ( Q_stricmp( cmd, "stell" ) == 0 ) {
			Svcmd_Tell_f();
			return qtrue;
		}

		if ( Q_stricmp( cmd, "scp" ) == 0 ) {
			Svcmd_ClientCommand_f( CCMD_CP );
			return qtrue;
		}
		if ( Q_stricmp( cmd, "smp" ) == 0 ) {
			Svcmd_ClientCommand_f( CCMD_MP );
			return qtrue;
		}
		if ( Q_stricmp( cmd, "sprint" ) == 0 ) {
			Svcmd_ClientCommand_f( CCMD_PRINT );
			return qtrue;
		}

		// everything else will also be printed to clients
		trap_SendServerCommand( -1, va("print \"server: %s\n\"", ConcatArgs(0) ) );
		return qtrue;
	}

	return qfalse;
}

