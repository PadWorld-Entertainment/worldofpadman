#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>

#include <ps5platform/fp.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "../client/client.h"
#include "../sys/sys_local.h"
#include "platform.h"

void IN_Frame( void );
void catchReturnFromMain( int status );
static qboolean sys_exiting;

void Q_NORETURN Sys_Exit( int code )
{
	say( "ends: status %d", code );
	fflush( NULL );
	catchReturnFromMain( code );
	for ( ;; )
		usleep( 100000 );
}

void Q_NORETURN Sys_Quit( void )
{
	if ( !sys_exiting ) {
		sys_exiting = qtrue;
		NET_Shutdown();
		CL_Shutdown( "", qtrue, qtrue );
	}
	Sys_Exit( 0 );
}

void Sys_Init( void )
{
	Cvar_Set( "arch", OS_STRING " " ARCH_STRING );
	Cvar_Set( "username", "Padman" );
}

void Q_NORETURN QDECL Sys_Error( const char *format, ... )
{
	va_list argptr;
	char text[1024];

	va_start( argptr, format );
	Q_vsnprintf( text, sizeof( text ), format, argptr );
	va_end( argptr );

	say( "Sys_Error: %s", text );

	if ( !sys_exiting ) {
		sys_exiting = qtrue;
		NET_Shutdown();
		CL_Shutdown( text, qtrue, qtrue );
	}
	Sys_Exit( 1 );
}

void Sys_ErrorDialog( const char *error )
{
	say( "ERROR: %s", error );
}

void Sys_Print( const char *msg )
{
	static char line[1024];
	static int used;

	for ( ; *msg; msg++ ) {
		const char c = *msg;
		if ( Q_IsColorString( msg ) ) {
			msg++;
			continue;
		}
		if ( c == '\n' || used == (int)sizeof( line ) - 1 ) {
			line[used] = '\0';
			say( "%s", line );
			used = 0;
			if ( c == '\n' )
				continue;
		}
		if ( c >= ' ' || c == '\t' )
			line[used++] = c;
	}
}

void Sys_AnsiColorPrint( const char *msg ) { Sys_Print( msg ); }
void QDECL Sys_SetStatus( const char *format, ... ) {}
char *Sys_ConsoleInput( void ) { return NULL; }
char *Sys_GetClipboardData( void ) { return NULL; }
void Sys_SendKeyEvents( void ) {}

void Sys_Sleep( int msec )
{
	struct timespec req;
	if ( msec < 0 ) {
		NET_Sleep( 10 * 1000 );
		return;
	}
	if ( msec == 0 )
		return;
	req.tv_sec = msec / 1000;
	req.tv_nsec = ( msec % 1000 ) * 1000000;
	nanosleep( &req, NULL );
}

qboolean Sys_LowPhysicalMemory( void ) { return qfalse; }
void Sys_BeginProfiling( void ) {}
void Sys_PlatformInit( void ) {}
void Sys_PlatformExit( void ) {}
void Q_NORETURN Sys_SigHandler( int signal ) { (void)signal; Sys_Quit(); }
int Sys_PID( void ) { return (int)getpid(); }
qboolean Sys_PIDIsRunning( int pid ) { (void)pid; return qtrue; }
const char *Sys_BinaryPath( void ) { return "/app0"; }
const char *Sys_BinaryPathRelative( const char *relative ) { return va( "/app0/%s", relative ); }
const char *Sys_DefaultBasePath( void ) { return "/app0"; }
const char *Sys_DefaultAppPath( void ) { return "/app0"; }

unsigned int ps5q3_if_nametoindex( const char *name ) { (void)name; return 0; }

int ps5q3_getnameinfo( const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen,
	char *serv, size_t servlen, int flags )
{
	const void *address;
	(void)salen; (void)flags;
	if ( serv && servlen ) serv[0] = '\0';
	if ( !host || !hostlen ) return 0;
	if ( sa->sa_family == AF_INET ) address = &( (const struct sockaddr_in *)sa )->sin_addr;
	else if ( sa->sa_family == AF_INET6 ) address = &( (const struct sockaddr_in6 *)sa )->sin6_addr;
	else return EAI_FAMILY;
	return inet_ntop( sa->sa_family, address, host, (socklen_t)hostlen ) ? 0 : EAI_FAIL;
}

#define PS5_SO_NBIO 0x1200
int ps5q3_ioctl( int fd, unsigned long request, ... )
{
	va_list args;
	int *value;
	int flags;
	if ( request != FIONBIO ) { errno = EINVAL; return -1; }
	va_start( args, request );
	value = va_arg( args, int * );
	va_end( args );
	flags = fcntl( fd, F_GETFL, 0 );
	if ( flags != -1 && fcntl( fd, F_SETFL, *value ? ( flags | O_NONBLOCK ) : ( flags & ~O_NONBLOCK ) ) != -1 )
		return 0;
	return setsockopt( fd, SOL_SOCKET, PS5_SO_NBIO, value, sizeof( *value ) );
}

const char *ps5q3_gai_strerror( int code )
{
	switch ( code ) {
	case EAI_NONAME: return "the name is not known";
	case EAI_FAMILY: return "no address of that family";
	case EAI_MEMORY: return "out of memory";
	case EAI_AGAIN: return "the console's resolver did not start";
	default: return "the name lookup failed";
	}
}

int sceNetInit( void );
int sceNetPoolCreate( const char *name, int size, int flags );
int sceNetPoolDestroy( int pool );
int sceNetResolverCreate( const char *name, int pool, int flags );
int sceNetResolverStartNtoa( int resolver, const char *hostname, struct in_addr *address, int timeout, int retry, int flags );
int sceNetResolverDestroy( int resolver );

static int ps5q3_resolve( const char *name, struct in_addr *address )
{
	static qboolean net_started;
	int pool, resolver, result;
	if ( !net_started ) { sceNetInit(); net_started = qtrue; }
	pool = sceNetPoolCreate( "wopresolver", 64 * 1024, 0 );
	if ( pool < 0 ) return EAI_AGAIN;
	resolver = sceNetResolverCreate( "wopresolver", pool, 0 );
	if ( resolver < 0 ) { sceNetPoolDestroy( pool ); return EAI_AGAIN; }
	result = sceNetResolverStartNtoa( resolver, name, address, 0, 0, 0 );
	sceNetResolverDestroy( resolver );
	sceNetPoolDestroy( pool );
	return result < 0 ? EAI_NONAME : 0;
}

int ps5q3_getaddrinfo( const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **result )
{
	const int family = hints ? hints->ai_family : AF_UNSPEC;
	struct in_addr v4;
	struct in6_addr v6;
	qboolean is6 = qfalse;
	struct addrinfo *info;
	size_t size;
	(void)service;
	if ( !result ) return EAI_FAIL;
	*result = NULL;
	if ( !node || !*node ) return EAI_NONAME;
	if ( inet_pton( AF_INET, node, &v4 ) == 1 ) {
		if ( family == AF_INET6 ) return EAI_FAMILY;
	} else if ( inet_pton( AF_INET6, node, &v6 ) == 1 ) {
		if ( family == AF_INET ) return EAI_FAMILY;
		is6 = qtrue;
	} else {
		int error;
		if ( family == AF_INET6 ) return EAI_NONAME;
		error = ps5q3_resolve( node, &v4 );
		if ( error ) return error;
	}
	size = sizeof( *info ) + ( is6 ? sizeof( struct sockaddr_in6 ) : sizeof( struct sockaddr_in ) );
	info = calloc( 1, size );
	if ( !info ) return EAI_MEMORY;
	info->ai_family = is6 ? AF_INET6 : AF_INET;
	info->ai_socktype = hints ? hints->ai_socktype : 0;
	info->ai_protocol = hints ? hints->ai_protocol : 0;
	info->ai_addr = (struct sockaddr *)( info + 1 );
	if ( is6 ) {
		struct sockaddr_in6 *a = (struct sockaddr_in6 *)info->ai_addr;
		a->sin6_len = sizeof( *a );
		a->sin6_family = AF_INET6;
		a->sin6_addr = v6;
		info->ai_addrlen = sizeof( *a );
	} else {
		struct sockaddr_in *a = (struct sockaddr_in *)info->ai_addr;
		a->sin_len = sizeof( *a );
		a->sin_family = AF_INET;
		a->sin_addr = v4;
		info->ai_addrlen = sizeof( *a );
	}
	*result = info;
	return 0;
}

void ps5q3_freeaddrinfo( struct addrinfo *info ) { free( info ); }
mode_t ps5q3_umask( mode_t mask ) { (void)mask; return 0; }

int main( int argc, char **argv )
{
	static char cmdline[1024] =
		"+set fs_basepath /app0 +set fs_homepath /app0 +set com_basegame wop "
		"+set vm_game 1 +set vm_cgame 1 +set vm_ui 1 "
		"+set s_khz 48 +set r_fullscreen 1 +set com_introplayed 1 +set sv_pure 0";
	FILE *extra;
	(void)argc; (void)argv;

	platform_init( PS5_TITLE_NAME );
	ps5_fp_ieee();

	extra = fopen( "/app0/cmdline.txt", "r" );
	if ( extra ) {
		const size_t used = strlen( cmdline );
		cmdline[used] = ' ';
		if ( !fgets( cmdline + used + 1, (int)( sizeof( cmdline ) - used - 1 ), extra ) )
			cmdline[used] = '\0';
		cmdline[strcspn( cmdline, "\r\n" )] = '\0';
		fclose( extra );
	}
	say( "command line: %s", cmdline );

	Sys_PlatformInit();
	Com_Init( cmdline );
	NET_Init();

	while ( 1 ) {
		IN_Frame();
		Com_Frame();
	}
	return 0;
}

const char *Sys_DefaultInstallPath( void ) { return "/app0"; }
char *Sys_GogPath( void ) { static char empty[] = ""; return empty; }
qboolean Sys_DllExtension( const char *name ) { (void)name; return qfalse; }
void *QDECL Sys_LoadGameDll( const char *name, vmMainProc *entryPoint, intptr_t (QDECL *systemcalls)(intptr_t, ...) ) { (void)name; (void)entryPoint; (void)systemcalls; return NULL; }
void Sys_UnloadDll( void *dllHandle ) { (void)dllHandle; }
FILE *Sys_Mkfifo( const char *ospath ) { (void)ospath; return NULL; }
void Sys_InitPIDFile( const char *gamedir ) { (void)gamedir; }
void Sys_RemovePIDFile( const char *gamedir ) { (void)gamedir; }
cpuFeatures_t Sys_GetProcessorFeatures( void ) { return 0; }
void Sys_SetEnv( const char *name, const char *value ) { (void)name; (void)value; }
void Sys_GLimpSafeInit( void ) {}
void Sys_GLimpInit( void ) {}
void DISCORD_Init( void ) {}
void DISCORD_Close( void ) {}
void DISCORD_EnqueueMessage( const char *msg ) { (void)msg; }
void HTTP_Init( void ) {}
void HTTP_SetTimeouts( int c, int t ) { (void)c; (void)t; }
void HTTP_Close( void ) {}

__attribute__((visibility("hidden"))) struct hostent *ps5q3_gethostbyname( const char *name ) { (void)name; return NULL; }
__attribute__((visibility("hidden"))) ssize_t ps5q3_readlink( const char *path, char *buf, size_t bufsiz ) { (void)path; (void)buf; (void)bufsiz; errno = ENOSYS; return -1; }
__attribute__((visibility("hidden"))) int ps5q3_mkstemp( char *tmpl ) { (void)tmpl; errno = ENOSYS; return -1; }
