#include <math.h>
#include <string.h>
#include "../client/client.h"
#include "../input/con_osk.h"
#include "platform.h"

#define PAD_L2              0x0100u
#define PAD_R2              0x0200u
#define STICK_DEADZONE      0.15f
#define STICK_AXIS_SCALE    32767
extern void Con_ToggleConsole_f( void );
#define R3_TAP_MS           200

static char s_overlay_text[64];
static int  s_overlay_until_ms;
static vec4_t s_overlay_color;

static void PS5_ShowBanner( const char *msg, float r, float g, float b )
{
	Q_strncpyz( s_overlay_text, msg, sizeof( s_overlay_text ) );
	s_overlay_color[0] = r;
	s_overlay_color[1] = g;
	s_overlay_color[2] = b;
	s_overlay_color[3] = 1.0f;
	s_overlay_until_ms = Com_Milliseconds() + 2000;
}

void PS5_Input_DrawOverlay( void );
void PS5_Input_DrawOverlay( void )
{
	vec4_t bg = { 0.0f, 0.0f, 0.0f, 0.78f };
	float fx, fy, fw, fh, pad;
	fontSize_t fs;
	int len;
	if ( !s_overlay_text[0] || Com_Milliseconds() >= s_overlay_until_ms )
		return;
	len = (int)Q_PrintStrlen( s_overlay_text );
	fw = (float)( len * BIGCHAR_WIDTH + 24 );
	fh = (float)( BIGCHAR_HEIGHT + 12 );
	fx = ( SCREEN_WIDTH - fw ) * 0.5f;
	fy = 64.0f;
	SCR_FillRect( fx, fy, fw, fh, bg );
	pad = 12.0f;
	SCR_AdjustFrom640( &fx, &fy, &fw, &fh );
	SCR_AdjustFrom640( &pad, NULL, NULL, NULL );
	fs.w = (int)( ( fw - 2.0f * pad ) / (float)( len > 0 ? len : 1 ) );
	fs.h = (int)( fh * 0.68f );
	re.SetColor( NULL );
	SCR_DrawString( (int)( fx + pad ), (int)( fy + ( fh - fs.h ) * 0.5f ), s_overlay_text, qfalse, fs );
}


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
	{ PAD_TRIANGLE,  K_PAD0_Y,               0 },
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
static cvar_t *stick_menu_speed;

static qboolean in_open;
static qboolean prev_in_menu;
static uint32_t prev_held;
static int prev_axis[MAX_JOYSTICK_AXIS];
static float cursor_accum_x;
static float cursor_accum_y;
static qboolean key_held[MAX_KEYS];

static qboolean r3_pending;
static qboolean r3_cancelled;
static int r3_down_ms;

static int s_rumble_expiry_ms;
static qboolean s_rumble_active;

static void PS5_SetRumble( int duration_ms )
{
	int expiry;
	if ( !in_open )
		return;
	pad_vibrate( 0.6f, 0.6f );
	expiry = Com_Milliseconds() + duration_ms;
	if ( !s_rumble_active || expiry > s_rumble_expiry_ms )
		s_rumble_expiry_ms = expiry;
	s_rumble_active = qtrue;
}

static void PS5_RumbleTick( void )
{
	if ( !s_rumble_active )
		return;
	if ( Com_Milliseconds() < s_rumble_expiry_ms )
		return;
	pad_vibrate( 0.0f, 0.0f );
	s_rumble_active = qfalse;
}

static void SetDefaultBind( int key, const char *binding )
{
	const char *current = Key_GetBinding( key );
	if ( !current || !current[0] )
		Key_SetBinding( key, binding );
}

static void InjectKey( int key, qboolean down )
{
	if ( key <= 0 || key >= MAX_KEYS )
		return;
	if ( key_held[key] == down )
		return;
	key_held[key] = down;
	Com_QueueEvent( 0, SE_KEY, key, down, 0, NULL );
}

static void ReleaseAll( void )
{
	int i;
	for ( i = 0; i < MAX_KEYS; i++ ) {
		if ( key_held[i] ) {
			key_held[i] = qfalse;
			Com_QueueEvent( 0, SE_KEY, i, qfalse, 0, NULL );
		}
	}
	r3_pending = qfalse;
	r3_cancelled = qfalse;
	for ( i = 0; i < MAX_JOYSTICK_AXIS; i++ ) {
		if ( prev_axis[i] != 0 ) {
			prev_axis[i] = 0;
			Com_QueueEvent( 0, SE_JOYSTICK_AXIS, i, 0, 0, NULL );
		}
	}
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

static qboolean menu_cursor_active = qfalse;

static void InjectMenuCursor( float sx, float sy )
{
	float speed = ( stick_menu_speed && stick_menu_speed->value > 0.0f ) ? stick_menu_speed->value : 6.0f;
	float fx, fy;
	int dx, dy;

	if ( sx > -STICK_DEADZONE && sx < STICK_DEADZONE ) sx = 0.0f;
	if ( sy > -STICK_DEADZONE && sy < STICK_DEADZONE ) sy = 0.0f;

	fx = sx * fabsf( sx ) * speed;
	fy = sy * fabsf( sy ) * speed;

	cursor_accum_x += fx;
	cursor_accum_y += fy;

	dx = (int)cursor_accum_x;
	dy = (int)cursor_accum_y;

	cursor_accum_x -= (float)dx;
	cursor_accum_y -= (float)dy;

	if ( dx || dy ) {
		menu_cursor_active = qtrue;
		Com_QueueEvent( 0, SE_MOUSE, dx, dy, 0, NULL );
	}
}

static void InjectAxis( int axis, int value )
{
	if ( value != prev_axis[axis] ) {
		prev_axis[axis] = value;
		Com_QueueEvent( 0, SE_JOYSTICK_AXIS, axis, value, 0, NULL );
	}
}

static qboolean ComboEdge( uint32_t trigger, uint32_t hold, uint32_t a, uint32_t b )
{
	return ( ( ( trigger & a ) && ( hold & b ) ) ||
	         ( ( trigger & b ) && ( hold & a ) ) ) ? qtrue : qfalse;
}

static qboolean OSK_AcceptRaises( void )
{
	int catcher = Key_GetCatcher();
	if ( catcher & ( KEYCATCH_CONSOLE | KEYCATCH_MESSAGE ) )
		return qtrue;
	if ( catcher & ( KEYCATCH_UI | KEYCATCH_CGAME ) ) {
		const char *target = Cvar_VariableString( "ui_ime_target" );
		if ( target && target[0] && Q_stricmp( target, "donothing" ) != 0 )
			return qtrue;
	}
	return qfalse;
}

static void AimAssistCombo( uint32_t *trigger, uint32_t *release, uint32_t hold )
{
	static int lastMode = 1;
	int mode;

	if ( !ComboEdge( *trigger, hold, PAD_L1, PAD_R1 ) )
		return;

	mode = (int)Cvar_VariableValue( "cl_aimAssist" );
	if ( mode > 0 ) {
		lastMode = mode;
		Cvar_Set( "cl_aimAssist", "0" );
		Com_Printf( "^1DISABLED AIM ASSIST\n" );
		PS5_ShowBanner( "^1DISABLED AIM ASSIST", 1.0f, 0.25f, 0.2f );
		
		pad_light_bar( 255, 64, 0 );
	} else {
		Cvar_Set( "cl_aimAssist", va( "%d", lastMode ) );
		Com_Printf( "^2ENABLED AIM ASSIST\n" );
		PS5_ShowBanner( "^2ENABLED AIM ASSIST", 0.2f, 1.0f, 0.35f );
		
		pad_light_bar( 0, 255, 64 );
	}
	PS5_SetRumble( 150 );

	InjectKey( K_PAD0_LEFTSHOULDER, qfalse );
	InjectKey( K_PAD0_RIGHTSHOULDER, qfalse );
	*trigger &= ~( PAD_L1 | PAD_R1 );
	*release &= ~( PAD_L1 | PAD_R1 );
}

static void R3TapOrHold( uint32_t *trigger, uint32_t *release, uint32_t hold )
{
	int now = Com_Milliseconds();

	if ( *trigger & PAD_R3 ) {
		r3_pending = qtrue;
		r3_cancelled = qfalse;
		r3_down_ms = now;
	}
	if ( r3_pending ) {
		if ( hold & PAD_L3 )
			r3_cancelled = qtrue;
		if ( !( hold & PAD_R3 ) ) {
			if ( !r3_cancelled && now - r3_down_ms < R3_TAP_MS )
				Cbuf_ExecuteText( EXEC_APPEND, "turn180\n" );
			InjectKey( K_PAD0_RIGHTSTICK_CLICK, qfalse );
			r3_pending = qfalse;
		} else if ( !r3_cancelled && now - r3_down_ms >= R3_TAP_MS ) {
			InjectKey( K_PAD0_RIGHTSTICK_CLICK, qtrue );
		}
	}
	*trigger &= ~PAD_R3;
	*release &= ~PAD_R3;
}

void IN_Init( void *windowData )
{
	(void)windowData;

	stick_move_deadzone = Cvar_Get( "stick_move_deadzone", "0.15", CVAR_ARCHIVE );
	stick_move_curve    = Cvar_Get( "stick_move_curve",    "1.0",  CVAR_ARCHIVE );
	stick_look_deadzone = Cvar_Get( "stick_look_deadzone", "0.15", CVAR_ARCHIVE );
	stick_look_curve    = Cvar_Get( "stick_look_curve",    "1.0",  CVAR_ARCHIVE );
	stick_menu_speed    = Cvar_Get( "stick_menu_speed",    "6.0",  CVAR_ARCHIVE );

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
	memset( key_held, 0, sizeof( key_held ) );
	prev_held = 0;
	prev_in_menu = qfalse;
	cursor_accum_x = 0.0f;
	cursor_accum_y = 0.0f;
	r3_pending = qfalse;
	r3_cancelled = qfalse;
	s_rumble_active = qfalse;
	s_rumble_expiry_ms = 0;

	if ( !in_open ) {
		in_open = pad_open() ? qtrue : qfalse;
		Com_Printf( "PS5 pad: %s\n", in_open ? "open" : "none" );
	}
	if ( in_open ) {
		int mode = (int)Cvar_VariableValue( "cl_aimAssist" );
		if ( mode > 0 )
			pad_light_bar( 0, 255, 64 );
		else
			pad_light_bar( 255, 64, 0 );
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

	triggered = held & ~prev_held;
	released = prev_held & ~held;
	prev_held = held;

	if ( Con_OSK_IsActive() ) {
		oskInput_t osk;
		memset( &osk, 0, sizeof( osk ) );
		if ( triggered & PAD_LEFT )     osk.pressLeft   = qtrue;
		if ( triggered & PAD_RIGHT )    osk.pressRight  = qtrue;
		if ( triggered & PAD_UP )       osk.pressUp     = qtrue;
		if ( triggered & PAD_DOWN )     osk.pressDown   = qtrue;
		if ( triggered & PAD_CROSS )    osk.pressAccept = qtrue;
		if ( triggered & PAD_CIRCLE )   osk.pressBack   = qtrue;
		if ( triggered & PAD_SQUARE )   osk.pressSpace  = qtrue;
		if ( triggered & PAD_TRIANGLE ) osk.pressShift  = qtrue;
		if ( triggered & PAD_OPTIONS )  osk.pressOk     = qtrue;
		if ( triggered & PAD_TOUCH_PAD ) osk.pressCancel = qtrue;
		osk.navX = pad.left_x;
		osk.navY = -pad.left_y;
		Con_OSK_Frame( &osk );
		PS5_RumbleTick();
		return;
	}

	catcher = Key_GetCatcher();
	in_menu = ( ( catcher & ( KEYCATCH_UI | KEYCATCH_CONSOLE | KEYCATCH_MESSAGE ) ) != 0 )
		|| ( clc.state != CA_ACTIVE );

	if ( in_menu && ( triggered & ( PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT ) ) )
		menu_cursor_active = qfalse;
	if ( in_menu != prev_in_menu ) {
		ReleaseAll();
		cursor_accum_x = 0.0f;
		cursor_accum_y = 0.0f;
		menu_cursor_active = qfalse;
		prev_in_menu = in_menu;
	}

	{
		static qboolean prev_in_ui;
		qboolean in_ui = ( catcher & ( KEYCATCH_UI | KEYCATCH_CGAME ) ) ? qtrue : qfalse;
		if ( in_ui && !prev_in_ui )
			Cvar_Set( "ui_ime_target", "" );
		prev_in_ui = in_ui;
	}

	/* Console toggle: L3 + Triangle */
	if ( ComboEdge( triggered, held, PAD_L1, PAD_TRIANGLE ) || ComboEdge( triggered, held, PAD_L3, PAD_TRIANGLE ) ) {
		ReleaseAll();
		Con_ToggleConsole_f();
		triggered &= ~( PAD_L1 | PAD_L3 | PAD_TRIANGLE );
		released &= ~( PAD_L1 | PAD_L3 | PAD_TRIANGLE );
	}

	/* OSK toggle: L3 + Circle, or Cross when at console/chat/IME prompt */
	if ( ComboEdge( triggered, held, PAD_L1, PAD_CIRCLE ) || ComboEdge( triggered, held, PAD_L3, PAD_CIRCLE ) ||
	     ( OSK_AcceptRaises() && ( triggered & PAD_CROSS ) ) ) {
		ReleaseAll();
		Con_OSK_Toggle();
		PS5_RumbleTick();
		return;
	}

	/* Quit combo: Options + Touchpad */
	if ( ComboEdge( triggered, held, PAD_OPTIONS, PAD_TOUCH_PAD ) ) {
		ReleaseAll();
		Cbuf_ExecuteText( EXEC_APPEND, "quit\n" );
		triggered &= ~( PAD_OPTIONS | PAD_TOUCH_PAD );
		released &= ~( PAD_OPTIONS | PAD_TOUCH_PAD );
	}

	AimAssistCombo( &triggered, &released, held );

	if ( !in_menu )
		R3TapOrHold( &triggered, &released, held );

	for ( i = 0; i < PS5_BUTTON_COUNT; i++ ) {
		int key = in_menu ? ps5Buttons[i].key_menu : ps5Buttons[i].key_game;
		if ( !key )
			key = ps5Buttons[i].key_game;
		if ( in_menu && menu_cursor_active && key == K_ENTER && ( catcher & KEYCATCH_UI ) && !( catcher & ( KEYCATCH_CONSOLE | KEYCATCH_MESSAGE ) ) )
			key = K_MOUSE1;
		if ( !key )
			continue;

		if ( triggered & ps5Buttons[i].mask )
			InjectKey( key, qtrue );
		if ( released & ps5Buttons[i].mask )
			InjectKey( key, qfalse );
	}

	if ( in_menu ) {
		InjectMenuCursor( pad.left_x, pad.left_y );
	} else {
		InjectAxis( AXIS_SIDE,    FilterStick(  pad.left_x,  stick_move_deadzone->value, stick_move_curve->value ) );
		InjectAxis( AXIS_FORWARD, FilterStick(  pad.left_y,  stick_move_deadzone->value, stick_move_curve->value ) );
		InjectAxis( AXIS_YAW,     FilterStick(  pad.right_x, stick_look_deadzone->value, stick_look_curve->value ) );
		InjectAxis( AXIS_PITCH,   FilterStick(  pad.right_y, stick_look_deadzone->value, stick_look_curve->value ) );
	}

	PS5_RumbleTick();
}

void IN_Shutdown( void )
{
	if ( in_open ) {
		pad_vibrate( 0.0f, 0.0f );
		pad_player_reset_light_bar( 0 );
	}
	s_rumble_active = qfalse;
	ReleaseAll();
}

void IN_Restart( void )
{
	IN_Shutdown();
	IN_Init( NULL );
}
