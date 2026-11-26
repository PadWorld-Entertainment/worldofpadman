#define PAD_L2 0x0100u
#define PAD_R2 0x0200u
#define PAD_L2 0x0100u
#define PAD_R2 0x0200u
#define PAD_L2 0x0100u
#define PAD_R2 0x0200u
#include <math.h>
#include <string.h>
#include "../client/client.h"
#include "platform.h"

#define STICK_DEADZONE      0.15f
#define STICK_AXIS_SCALE    32767
#define MENU_CURSOR_SPEED   14.0f

#define AXIS_SIDE           0
#define AXIS_FORWARD        1
#define AXIS_YAW            4
#define AXIS_PITCH          3

typedef struct {
	uint32_t mask;
	int key_game;
	int key_menu;
} ps5ButtonMap_t;

static const ps5ButtonMap_t ps5Buttons[] = {
	{ PAD_CROSS,     K_PAD0_A,               K_ENTER },
	{ PAD_CIRCLE,    K_PAD0_B,               K_ESCAPE },
	{ PAD_SQUARE,    K_PAD0_X,               0 },
	{ PAD_TRIANGLE,  K_PAD0_Y,               K_CONSOLE },
	{ PAD_L1,        K_PAD0_LEFTSHOULDER,    0 },
	{ PAD_R1,        K_PAD0_RIGHTSHOULDER,   0 },
	{ PAD_L2,        K_PAD0_LEFTTRIGGER,     0 },
	{ PAD_R2,        K_PAD0_RIGHTTRIGGER,    0 },
	{ PAD_OPTIONS,   K_ESCAPE,               K_ESCAPE },
	{ PAD_TOUCH_PAD, K_PAD0_BACK,            0 },
	{ PAD_L3,        K_PAD0_LEFTSTICK_CLICK, 0 },
	{ PAD_R3,        K_PAD0_RIGHTSTICK_CLICK,0 },
	{ PAD_UP,        K_PAD0_DPAD_UP,         K_UPARROW },
	{ PAD_DOWN,      K_PAD0_DPAD_DOWN,       K_DOWNARROW },
	{ PAD_LEFT,      K_PAD0_DPAD_LEFT,       K_LEFTARROW },
	{ PAD_RIGHT,     K_PAD0_DPAD_RIGHT,      K_RIGHTARROW },
};

#define PS5_BUTTON_COUNT ( (int)( sizeof( ps5Buttons ) / sizeof( ps5Buttons[0] ) ) )

static cvar_t *stick_move_deadzone;
static cvar_t *stick_move_curve;
static cvar_t *stick_look_deadzone;
static cvar_t *stick_look_curve;

static qboolean in_open;
static qboolean prev_in_menu;
static uint32_t prev_held;
static int prev_axis[MAX_JOYSTICK_AXIS];
static float cursor_accum_x;
static float cursor_accum_y;

static void SetDefaultBind( int key, const char *binding )
{
	const char *current = Key_GetBinding( key );
	if ( !current || !current[0] )
		Key_SetBinding( key, binding );
}

static int FilterStick( float raw, float deadzone, float curve )
{
	float sign, mag;
	if ( deadzone < 0.0f ) deadzone = 0.0f;
	if ( deadzone > 0.9f ) deadzone = 0.9f;
	if ( curve < 0.25f ) curve = 0.25f;
	if ( curve > 4.0f ) curve = 4.0f;

	if ( raw > -deadzone && raw < deadzone )
		return 0;

	sign = ( raw < 0.0f ) ? -1.0f : 1.0f;
	mag = ( fabsf( raw ) - deadzone ) / ( 1.0f - deadzone );
	if ( mag > 1.0f ) mag = 1.0f;
	if ( curve != 1.0f ) mag = powf( mag, curve );

	return (int)( sign * mag * STICK_AXIS_SCALE );
}

static void InjectMenuCursor( float sx, float sy )
{
	float fx, fy;
	int dx, dy;

	if ( sx > -STICK_DEADZONE && sx < STICK_DEADZONE ) sx = 0.0f;
	if ( sy > -STICK_DEADZONE && sy < STICK_DEADZONE ) sy = 0.0f;

	fx = sx * fabsf( sx ) * MENU_CURSOR_SPEED;
	fy = sy * fabsf( sy ) * MENU_CURSOR_SPEED;

	cursor_accum_x += fx;
	cursor_accum_y += fy;

	dx = (int)cursor_accum_x;
	dy = (int)cursor_accum_y;

	cursor_accum_x -= (float)dx;
	cursor_accum_y -= (float)dy;

	if ( dx || dy )
		Com_QueueEvent( 0, SE_MOUSE, dx, dy, 0, NULL );
}

static void InjectAxis( int axis, int value )
{
	if ( value != prev_axis[axis] ) {
		prev_axis[axis] = value;
		Com_QueueEvent( 0, SE_JOYSTICK_AXIS, axis, value, 0, NULL );
	}
}

static void ReleaseAll( void )
{
	int i;
	for ( i = 0; i < PS5_BUTTON_COUNT; i++ ) {
		if ( prev_held & ps5Buttons[i].mask ) {
			int k_game = ps5Buttons[i].key_game;
			int k_menu = ps5Buttons[i].key_menu ? ps5Buttons[i].key_menu : k_game;
			if ( k_menu == K_ENTER )
				Com_QueueEvent( 0, SE_KEY, K_MOUSE1, qfalse, 0, NULL );
			if ( k_menu )
				Com_QueueEvent( 0, SE_KEY, k_menu, qfalse, 0, NULL );
			if ( k_game && k_game != k_menu )
				Com_QueueEvent( 0, SE_KEY, k_game, qfalse, 0, NULL );
		}
	}
	prev_held = 0;
	for ( i = 0; i < MAX_JOYSTICK_AXIS; i++ )
		InjectAxis( i, 0 );
}

void IN_Init( void *windowData )
{
	(void)windowData;

	stick_move_deadzone = Cvar_Get( "stick_move_deadzone", "0.15", CVAR_ARCHIVE );
	stick_move_curve    = Cvar_Get( "stick_move_curve",    "1.0",  CVAR_ARCHIVE );
	stick_look_deadzone = Cvar_Get( "stick_look_deadzone", "0.15", CVAR_ARCHIVE );
	stick_look_curve    = Cvar_Get( "stick_look_curve",    "1.0",  CVAR_ARCHIVE );

	Cvar_SetSafe( "in_joystick", "1" );
	Cvar_SetSafe( "in_joystickUseAnalog", "1" );
	Cvar_SetSafe( "j_pitch_axis", "3" );
	Cvar_SetSafe( "j_yaw_axis", "4" );
	Cvar_SetSafe( "j_forward_axis", "1" );
	Cvar_SetSafe( "j_side_axis", "0" );
	Cvar_SetSafe( "j_pitch", "0.002" );
	Cvar_SetSafe( "j_yaw", "-0.002" );
	Cvar_SetSafe( "j_forward", "-0.25" );
	Cvar_SetSafe( "j_side", "0.25" );

	SetDefaultBind( K_PAD0_A,                "+moveup" );
	SetDefaultBind( K_PAD0_B,                "+movedown" );
	SetDefaultBind( K_PAD0_X,                "weapnext" );
	SetDefaultBind( K_PAD0_Y,                "weapprev" );
	SetDefaultBind( K_PAD0_LEFTSHOULDER,     "+speed" );
	SetDefaultBind( K_PAD0_RIGHTSHOULDER,    "+button2" );
	SetDefaultBind( K_PAD0_LEFTTRIGGER,      "+zoom" );
	SetDefaultBind( K_PAD0_RIGHTTRIGGER,     "+attack" );
	SetDefaultBind( K_PAD0_LEFTSTICK_CLICK,  "+scores" );
	SetDefaultBind( K_PAD0_RIGHTSTICK_CLICK, "+scores" );
	SetDefaultBind( K_PAD0_BACK,             "+scores" );
	SetDefaultBind( K_PAD0_DPAD_UP,          "weapnext" );
	SetDefaultBind( K_PAD0_DPAD_DOWN,        "weapprev" );
	SetDefaultBind( K_PAD0_DPAD_LEFT,        "weapprev" );
	SetDefaultBind( K_PAD0_DPAD_RIGHT,       "weapnext" );

	memset( prev_axis, 0, sizeof( prev_axis ) );
	prev_held = 0;
	prev_in_menu = qfalse;
	cursor_accum_x = 0.0f;
	cursor_accum_y = 0.0f;

	if ( !in_open ) {
		in_open = pad_open() ? qtrue : qfalse;
		Com_Printf( "PS5 pad: %s\n", in_open ? "open" : "none" );
	}
}

void IN_Frame( void )
{
	struct pad pad;
	uint32_t held, triggered, released;
	int catcher, i;
	qboolean in_menu;

	if ( !in_open )
		return;

	pad_poll( &pad );
	held = pad.held;
	if ( pad.l2 > 0.35f )
		held |= PAD_L2;
	if ( pad.r2 > 0.25f )
		held |= PAD_R2;

	catcher = Key_GetCatcher();
	in_menu = ( ( catcher & ( KEYCATCH_UI | KEYCATCH_CONSOLE | KEYCATCH_MESSAGE ) ) != 0 )
		|| ( clc.state != CA_ACTIVE );

	if ( in_menu != prev_in_menu ) {
		ReleaseAll();
		cursor_accum_x = 0.0f;
		cursor_accum_y = 0.0f;
		prev_in_menu = in_menu;
	}

	triggered = held & ~prev_held;
	released = prev_held & ~held;
	prev_held = held;

	for ( i = 0; i < PS5_BUTTON_COUNT; i++ ) {
		int key = in_menu ? ps5Buttons[i].key_menu : ps5Buttons[i].key_game;
		if ( !key )
			key = ps5Buttons[i].key_game;
		if ( in_menu && key == K_ENTER && ( catcher & KEYCATCH_UI ) && !( catcher & ( KEYCATCH_CONSOLE | KEYCATCH_MESSAGE ) ) )
			key = K_MOUSE1;
		if ( !key )
			continue;

		if ( triggered & ps5Buttons[i].mask )
			Com_QueueEvent( 0, SE_KEY, key, qtrue, 0, NULL );
		if ( released & ps5Buttons[i].mask )
			Com_QueueEvent( 0, SE_KEY, key, qfalse, 0, NULL );
	}

	if ( in_menu ) {
		InjectMenuCursor( pad.left_x, pad.left_y );
	} else {
		InjectAxis( AXIS_SIDE,    FilterStick(  pad.left_x,  stick_move_deadzone->value, stick_move_curve->value ) );
		InjectAxis( AXIS_FORWARD, FilterStick(  pad.left_y,  stick_move_deadzone->value, stick_move_curve->value ) );
		InjectAxis( AXIS_YAW,     FilterStick(  pad.right_x, stick_look_deadzone->value, stick_look_curve->value ) );
		InjectAxis( AXIS_PITCH,   FilterStick(  pad.right_y, stick_look_deadzone->value, stick_look_curve->value ) );
	}
}

void IN_Shutdown( void )
{
	ReleaseAll();
}

void IN_Restart( void )
{
	IN_Shutdown();
	IN_Init( NULL );
}
