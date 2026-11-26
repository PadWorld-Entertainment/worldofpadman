#include "../client/client.h"
#include "platform.h"

static qboolean in_open;

void IN_Init( void *windowData )
{
	(void)windowData;
	if ( !in_open ) {
		in_open = pad_open() ? qtrue : qfalse;
		Com_Printf( "PS5 pad: %s\n", in_open ? "open" : "none" );
	}
}

void IN_Frame( void )
{
	static struct pad pad;
	if ( !in_open )
		return;
	pad_poll( &pad );
}

void IN_Shutdown( void )
{
}

void IN_Restart( void )
{
	IN_Shutdown();
	IN_Init( NULL );
}
