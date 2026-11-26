#include <math.h>
#include "../client/client.h"
#include "platform.h"

static cvar_t *in_ps5LookSpeed;
static cvar_t *in_ps5MenuSpeed;
static qboolean in_open;
static double in_last;

enum {
	B_CROSS, B_CIRCLE, B_SQUARE, B_TRIANGLE,
	B_L1, B_R1, B_L2, B_R2, B_L3, B_R3,
	B_OPTIONS, B_TOUCH,
	B_UP, B_DOWN, B_LEFT, B_RIGHT,
	B_COUNT
};

static int in_key[B_COUNT];

static void IN_SendKey( int t, int slot, int key, qboolean down )
{
	if ( down ) {
		if ( in_key[slot] == key )
			return;
		if ( in_key[slot] )
			Com_QueueEvent( t, SE_KEY, in_key[slot], qfalse, 0, NULL );
		in_key[slot] = key;
		Com_QueueEvent( t, SE_KEY, key, qtrue, 0, NULL );
	} else if ( in_key[slot] ) {
		Com_QueueEvent( t, SE_KEY, in_key[slot], qfalse, 0, NULL );
		in_key[slot] = 0;
	}
}

static void IN_ReleaseAll( int t )
{
	int i;
	for ( i = 0; i < B_COUNT; i++ )
		IN_SendKey( t, i, 0, qfalse );
}

static float IN_ShapeStick( float v, float exponent )
{
	const float mag = fabsf( v );
	if ( mag <= 0.0f )
		return 0.0f;
	return copysignf( powf( mag, exponent ), v );
}

void IN_Init( void *windowData )
{
	(void)windowData;
	in_ps5LookSpeed = Cvar_Get( "in_ps5LookSpeed", "900", CVAR_ARCHIVE );
	in_ps5MenuSpeed = Cvar_Get( "in_ps5MenuSpeed", "1100", CVAR_ARCHIVE );
	Cvar_Set( "in_joystick", "1" );
	if ( !in_open ) {
		in_open = pad_open() ? qtrue : qfalse;
		Com_Printf( "PS5 pad: %s\n", in_open ? "open" : "none" );
	}
	in_last = now_seconds();
}

void IN_Frame( void )
{
	struct pad pad;
	double now, dt;
	int t;
	qboolean inMenu;
	static float remX, remY;
	static int lastSide, lastForward;

	if ( !in_open )
		return;

	now = now_seconds();
	dt = now - in_last;
	if ( dt < 0.0 )
		dt = 0.0;
	else if ( dt > 0.1 )
		dt = 0.1;
	in_last = now;
	t = Sys_Milliseconds();

	pad_poll( &pad );
	inMenu = ( ( Key_GetCatcher() & ( KEYCATCH_UI | KEYCATCH_CONSOLE | KEYCATCH_MESSAGE ) ) != 0 )
		|| ( clc.state != CA_ACTIVE );

	if ( inMenu ) {
		const float sx = fabsf( pad.left_x ) > fabsf( pad.right_x ) ? pad.left_x : pad.right_x;
		const float sy = fabsf( pad.left_y ) > fabsf( pad.right_y ) ? pad.left_y : pad.right_y;
		const float mx = IN_ShapeStick( sx, 1.5f ) * in_ps5MenuSpeed->value * (float)dt + remX;
		const float my = IN_ShapeStick( sy, 1.5f ) * in_ps5MenuSpeed->value * (float)dt + remY;
		const int dx = (int)mx;
		const int dy = (int)my;
		remX = mx - (float)dx;
		remY = my - (float)dy;
		if ( dx || dy )
			Com_QueueEvent( t, SE_MOUSE, dx, dy, 0, NULL );

		if ( lastSide != 0 ) {
			lastSide = 0;
			Com_QueueEvent( t, SE_JOYSTICK_AXIS, 0, 0, 0, NULL );
		}
		if ( lastForward != 0 ) {
			lastForward = 0;
			Com_QueueEvent( t, SE_JOYSTICK_AXIS, 1, 0, 0, NULL );
		}

		IN_SendKey( t, B_UP, K_UPARROW, ( pad.held & PAD_UP ) != 0 );
		IN_SendKey( t, B_DOWN, K_DOWNARROW, ( pad.held & PAD_DOWN ) != 0 );
		IN_SendKey( t, B_LEFT, K_LEFTARROW, ( pad.held & PAD_LEFT ) != 0 );
		IN_SendKey( t, B_RIGHT, K_RIGHTARROW, ( pad.held & PAD_RIGHT ) != 0 );
		IN_SendKey( t, B_CROSS, K_MOUSE1, ( pad.held & PAD_CROSS ) != 0 );
		IN_SendKey( t, B_R2, K_MOUSE1, pad.r2 > 0.35f );
		IN_SendKey( t, B_CIRCLE, K_ESCAPE, ( pad.held & PAD_CIRCLE ) != 0 );
		IN_SendKey( t, B_OPTIONS, K_ESCAPE, ( pad.held & PAD_OPTIONS ) != 0 );
		IN_SendKey( t, B_SQUARE, 0, qfalse );
		IN_SendKey( t, B_TRIANGLE, 0, qfalse );
		IN_SendKey( t, B_L1, 0, qfalse );
		IN_SendKey( t, B_R1, 0, qfalse );
		IN_SendKey( t, B_L2, 0, qfalse );
		IN_SendKey( t, B_L3, 0, qfalse );
		IN_SendKey( t, B_R3, 0, qfalse );
		IN_SendKey( t, B_TOUCH, 0, qfalse );
	} else {
		const float lx = IN_ShapeStick( pad.right_x, 1.8f ) * in_ps5LookSpeed->value * (float)dt + remX;
		const float ly = IN_ShapeStick( pad.right_y, 1.8f ) * in_ps5LookSpeed->value * (float)dt + remY;
		const int dx = (int)lx;
		const int dy = (int)ly;
		const int side = (int)( IN_ShapeStick( pad.left_x, 1.35f ) * 127.0f );
		const int forward = (int)( IN_ShapeStick( -pad.left_y, 1.35f ) * 127.0f );
		remX = lx - (float)dx;
		remY = ly - (float)dy;
		if ( dx || dy )
			Com_QueueEvent( t, SE_MOUSE, dx, dy, 0, NULL );
		if ( side != lastSide ) {
			lastSide = side;
			Com_QueueEvent( t, SE_JOYSTICK_AXIS, 0, side, 0, NULL );
		}
		if ( forward != lastForward ) {
			lastForward = forward;
			Com_QueueEvent( t, SE_JOYSTICK_AXIS, 1, forward, 0, NULL );
		}

		IN_SendKey( t, B_CROSS, K_SPACE, ( pad.held & PAD_CROSS ) != 0 );
		IN_SendKey( t, B_L3, K_SPACE, ( pad.held & PAD_L3 ) != 0 );
		IN_SendKey( t, B_CIRCLE, K_CTRL, ( pad.held & PAD_CIRCLE ) != 0 );
		IN_SendKey( t, B_R3, K_CTRL, ( pad.held & PAD_R3 ) != 0 );
		IN_SendKey( t, B_R2, K_MOUSE1, pad.r2 > 0.25f );
		IN_SendKey( t, B_SQUARE, K_MOUSE1, ( pad.held & PAD_SQUARE ) != 0 );
		IN_SendKey( t, B_L1, K_MWHEELDOWN, ( pad.held & PAD_L1 ) != 0 );
		IN_SendKey( t, B_R1, K_MWHEELUP, ( pad.held & PAD_R1 ) != 0 );
		IN_SendKey( t, B_TOUCH, K_TAB, ( pad.held & PAD_TOUCH_PAD ) != 0 );
		IN_SendKey( t, B_OPTIONS, K_ESCAPE, ( pad.held & PAD_OPTIONS ) != 0 );
		IN_SendKey( t, B_UP, K_UPARROW, ( pad.held & PAD_UP ) != 0 );
		IN_SendKey( t, B_DOWN, K_DOWNARROW, ( pad.held & PAD_DOWN ) != 0 );
		IN_SendKey( t, B_LEFT, K_LEFTARROW, ( pad.held & PAD_LEFT ) != 0 );
		IN_SendKey( t, B_RIGHT, K_RIGHTARROW, ( pad.held & PAD_RIGHT ) != 0 );
	}
}

void IN_Shutdown( void )
{
	IN_ReleaseAll( Sys_Milliseconds() );
}

void IN_Restart( void )
{
	IN_Shutdown();
	IN_Init( NULL );
}
