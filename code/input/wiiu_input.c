/* wiiu_input.c -- Wii U input backend: polls VPAD/KPAD, emits ioq3 events via Com_QueueEvent(). */

#include <string.h>
#include <math.h>

#include <vpad/input.h>
#include <padscore/kpad.h>
#include <padscore/wpad.h>

#include "qcommon/q_shared.h"
#include "qcommon/qcommon.h"
#include "client/keycodes.h"
#include "client/keys.h"
#include "../input/wiiu_input.h"
#include "../input/con_osk.h"

/* Avoid pulling in the full client.h header */
extern int Key_GetCatcher(void);

/* ---- Constants ---- */
#define STICK_DEADZONE      0.15f
/* Engine-wide axis range is ±32767, not ±127 -- learned that one the hard way. */
#define STICK_AXIS_SCALE    32767
#define MENU_CURSOR_SPEED   14.0f    /* pixels/frame at full deflection */
#define MAX_KPAD_CHANNELS   4

/* KPADStatus.pos range is undocumented in wut; assumed ~±1. These scales match vWii
 * defaults and need hardware retuning. */
#define POINTER_MENU_SCALE  600.0f  /* frame-to-frame delta -> menu cursor mouse counts */
#define POINTER_TURN_SCALE  190.0f  /* center-offset -> continuous turn-rate mouse counts */

/* Axis indices matching j_*_axis cvars set in wiiu_main.c cmdline */
#define AXIS_SIDE       0   /* left stick X -> strafe */
#define AXIS_FORWARD    1   /* left stick Y -> fwd/back */
#define AXIS_YAW        4   /* right stick X -> yaw */
#define AXIS_PITCH      3   /* right stick Y -> pitch */

/* ---- Button mapping tables: hardware bit -> ioq3 keycode, per context. ---- */
typedef struct {
    uint32_t    mask;
    int         key_game;
    int         key_menu;
} buttonMap_t;

static const buttonMap_t vpadButtons[] = {
    { VPAD_BUTTON_A,        K_PAD0_A,               K_ENTER },
    { VPAD_BUTTON_B,        K_PAD0_B,               K_ESCAPE },
    { VPAD_BUTTON_X,        K_PAD0_X,               0 },
    { VPAD_BUTTON_Y,        K_PAD0_Y,               K_CONSOLE },
    { VPAD_BUTTON_L,        K_PAD0_LEFTSHOULDER,    0 },
    { VPAD_BUTTON_R,        K_PAD0_RIGHTSHOULDER,    0 },
    { VPAD_BUTTON_ZL,       K_PAD0_LEFTTRIGGER,     0 },
    { VPAD_BUTTON_ZR,       K_PAD0_RIGHTTRIGGER,     0 },
    { VPAD_BUTTON_PLUS,     K_ESCAPE,               K_ESCAPE },
    { VPAD_BUTTON_MINUS,    K_PAD0_BACK,            0 },
    { VPAD_BUTTON_STICK_L,  K_PAD0_LEFTSTICK_CLICK, 0 },
    { VPAD_BUTTON_STICK_R,  K_PAD0_RIGHTSTICK_CLICK,0 },
    { VPAD_BUTTON_UP,       K_PAD0_DPAD_UP,         K_UPARROW },
    { VPAD_BUTTON_DOWN,     K_PAD0_DPAD_DOWN,       K_DOWNARROW },
    { VPAD_BUTTON_LEFT,     K_PAD0_DPAD_LEFT,       K_LEFTARROW },
    { VPAD_BUTTON_RIGHT,    K_PAD0_DPAD_RIGHT,      K_RIGHTARROW },
};
#define VPAD_BUTTON_COUNT (sizeof(vpadButtons) / sizeof(vpadButtons[0]))

/* Pro Controller layout, Classic Controller shares it minus stick clicks. */
static const buttonMap_t proButtons[] = {
    { WPAD_PRO_BUTTON_A,        K_PAD0_A,               K_ENTER },
    { WPAD_PRO_BUTTON_B,        K_PAD0_B,               K_ESCAPE },
    { WPAD_PRO_BUTTON_X,        K_PAD0_X,               0 },
    { WPAD_PRO_BUTTON_Y,        K_PAD0_Y,               K_CONSOLE },
    { WPAD_PRO_BUTTON_L,        K_PAD0_LEFTSHOULDER,    0 },
    { WPAD_PRO_BUTTON_R,        K_PAD0_RIGHTSHOULDER,    0 },
    { WPAD_PRO_BUTTON_ZL,       K_PAD0_LEFTTRIGGER,     0 },
    { WPAD_PRO_BUTTON_ZR,       K_PAD0_RIGHTTRIGGER,     0 },
    { WPAD_PRO_BUTTON_PLUS,     K_ESCAPE,               K_ESCAPE },
    { WPAD_PRO_BUTTON_MINUS,    K_PAD0_BACK,            0 },
    { WPAD_PRO_BUTTON_STICK_L,  K_PAD0_LEFTSTICK_CLICK, 0 },
    { WPAD_PRO_BUTTON_STICK_R,  K_PAD0_RIGHTSTICK_CLICK,0 },
    { WPAD_PRO_BUTTON_UP,       K_PAD0_DPAD_UP,         K_UPARROW },
    { WPAD_PRO_BUTTON_DOWN,     K_PAD0_DPAD_DOWN,       K_DOWNARROW },
    { WPAD_PRO_BUTTON_LEFT,     K_PAD0_DPAD_LEFT,       K_LEFTARROW },
    { WPAD_PRO_BUTTON_RIGHT,    K_PAD0_DPAD_RIGHT,      K_RIGHTARROW },
};
#define PRO_BUTTON_COUNT (sizeof(proButtons) / sizeof(proButtons[0]))

/* Bare Wiimote buttons, used when a Nunchuk is attached. B is the underside
 * trigger, so it fires instead of mirroring VPAD's B=crouch. */
static const buttonMap_t wiimoteButtons[] = {
    { WPAD_BUTTON_A,        K_PAD0_A,               K_ENTER },   /* jump */
    { WPAD_BUTTON_B,        K_PAD0_RIGHTTRIGGER,    0 },         /* fire (trigger) */
    { WPAD_BUTTON_1,        K_PAD0_LEFTTRIGGER,     0 },         /* zoom */
    { WPAD_BUTTON_2,        K_PAD0_LEFTSHOULDER,    0 },         /* walk */
    { WPAD_BUTTON_MINUS,    K_PAD0_BACK,            0 },         /* scores */
    { WPAD_BUTTON_PLUS,     K_ESCAPE,               K_ESCAPE },
    { WPAD_BUTTON_UP,       K_PAD0_DPAD_UP,         K_UPARROW },
    { WPAD_BUTTON_DOWN,     K_PAD0_DPAD_DOWN,       K_DOWNARROW },
    { WPAD_BUTTON_LEFT,     K_PAD0_DPAD_LEFT,       K_LEFTARROW },
    { WPAD_BUTTON_RIGHT,    K_PAD0_DPAD_RIGHT,      K_RIGHTARROW },
};
#define WIIMOTE_BUTTON_COUNT (sizeof(wiimoteButtons) / sizeof(wiimoteButtons[0]))

/* Nunchuk Z/C, delivered in a separate hold/trigger/release bitfield from the Wiimote's own. */
static const buttonMap_t nunchukButtons[] = {
    { WPAD_BUTTON_Z,        K_PAD0_B,               0 },         /* crouch */
    { WPAD_BUTTON_C,        K_PAD0_RIGHTSHOULDER,   0 },         /* use item */
};
#define NUNCHUK_BUTTON_COUNT (sizeof(nunchukButtons) / sizeof(nunchukButtons[0]))

static const buttonMap_t classicButtons[] = {
    { WPAD_CLASSIC_BUTTON_A,    K_PAD0_A,               K_ENTER },
    { WPAD_CLASSIC_BUTTON_B,    K_PAD0_B,               K_ESCAPE },
    { WPAD_CLASSIC_BUTTON_X,    K_PAD0_X,               0 },
    { WPAD_CLASSIC_BUTTON_Y,    K_PAD0_Y,               K_CONSOLE },
    { WPAD_CLASSIC_BUTTON_L,    K_PAD0_LEFTSHOULDER,    0 },
    { WPAD_CLASSIC_BUTTON_R,    K_PAD0_RIGHTSHOULDER,    0 },
    { WPAD_CLASSIC_BUTTON_ZL,   K_PAD0_LEFTTRIGGER,     0 },
    { WPAD_CLASSIC_BUTTON_ZR,   K_PAD0_RIGHTTRIGGER,     0 },
    { WPAD_CLASSIC_BUTTON_PLUS, K_ESCAPE,               K_ESCAPE },
    { WPAD_CLASSIC_BUTTON_MINUS,K_PAD0_BACK,            0 },
    { WPAD_CLASSIC_BUTTON_UP,   K_PAD0_DPAD_UP,         K_UPARROW },
    { WPAD_CLASSIC_BUTTON_DOWN, K_PAD0_DPAD_DOWN,       K_DOWNARROW },
    { WPAD_CLASSIC_BUTTON_LEFT, K_PAD0_DPAD_LEFT,       K_LEFTARROW },
    { WPAD_CLASSIC_BUTTON_RIGHT,K_PAD0_DPAD_RIGHT,      K_RIGHTARROW },
};
#define CLASSIC_BUTTON_COUNT (sizeof(classicButtons) / sizeof(classicButtons[0]))

/* ---- State ---- */
static qboolean     input_initialized;
static qboolean     quit_pressed;

static uint8_t      kpad_prev_ext[MAX_KPAD_CHANNELS];

/* Previous stick axis values (avoid redundant events) */
static int          vpad_prev_axis[4];
static int          kpad_prev_axis[MAX_KPAD_CHANNELS][4];

/* IR pointer (Wiimote+Nunchuk aim). Menu mode uses frame-to-frame delta (cursor movement);
 * game mode uses center-offset instead (see PollKPAD) -- only the menu path needs "previous". */
static qboolean     pointer_prev_valid[MAX_KPAD_CHANNELS];
static float        pointer_prev_x[MAX_KPAD_CHANNELS];
static float        pointer_prev_y[MAX_KPAD_CHANNELS];

/* Layer-2 instant fine-aim offset, consumed by CL_FinishMove() in cl_input.c (mirrors the
 * vWii sibling's wii_ir_aim_x/y). Not per-channel: single local player, last valid pointer wins. */
float                wiiu_ir_aim_x = 0.0f;
float                wiiu_ir_aim_y = 0.0f;

/* Menu cursor sub-pixel accumulators */
static float        cursor_accum_x, cursor_accum_y;

/* Touch screen state */
static qboolean     touch_prev_valid;
static uint16_t     touch_prev_x, touch_prev_y;

/* Separate touch edge for the OSK -- it wants taps, not the look-delta model. */
static qboolean     osk_touch_prev;

/* Track which ioq3 keys are currently held (for release-all on mode switch) */
static qboolean     key_held[MAX_KEYS];
static qboolean     prev_in_menu;

/* Stick response tuning (registered in IN_Init) */
static cvar_t       *stick_move_deadzone;
static cvar_t       *stick_move_curve;
static cvar_t       *stick_look_deadzone;
static cvar_t       *stick_look_curve;

/* Touch response tuning (registered in IN_Init) */
static cvar_t       *touch_sensitivity;
static cvar_t       *touch_accel;

/* Wiimote IR pointer tuning (registered in IN_Init), ported from the vWii sibling's
 * wii_ir_deadzone/sensitivity/maxdelta/yawrange/pitchrange cvars. */
static cvar_t       *pointer_deadzone;
static cvar_t       *pointer_sensitivity;
static cvar_t       *pointer_maxdelta;
/* Not static: read from cl_input.c's CL_FinishMove() to scale the layer-2 fine-aim offset. */
cvar_t              *pointer_yawrange;
cvar_t              *pointer_pitchrange;

/* True while any Wii Remote has a Nunchuk attached; aim assist is blocked then. */
static qboolean     nunchuk_active;

/* Rumble (registered in IN_Init) */
static cvar_t       *rumble_enable;
static int           s_rumbleExpiryMs;
static qboolean      s_rumbleActive;

/* VPADControlMotor timing is undocumented -- send a long pattern, cut it early with
 * VPADStopMotor() instead. Gave up guessing the byte-to-ms ratio. */
static uint8_t s_rumblePattern[40];  /* filled with 0xFF in WiiU_Input_Init */

/* ---- Helpers ---- */

static qboolean InMenu(void)
{
    int catcher = Key_GetCatcher();
    return (catcher & (KEYCATCH_UI | KEYCATCH_CGAME | KEYCATCH_CONSOLE)) ? qtrue : qfalse;
}

static void InjectKey(int key, qboolean down)
{
    if (key <= 0 || key >= MAX_KEYS)
        return;
    if (key_held[key] == down)
        return;
    key_held[key] = down;
    Com_QueueEvent(0, SE_KEY, key, down, 0, NULL);
}

static void ReleaseAllKeys(void)
{
    int i;
    for (i = 0; i < MAX_KEYS; i++) {
        if (key_held[i]) {
            key_held[i] = qfalse;
            Com_QueueEvent(0, SE_KEY, i, qfalse, 0, NULL);
        }
    }
}

/* Deadzone + response curve, -1..1 -> ±32767 axis range. curve==1 is linear. */
static int FilterStick(float raw, float deadzone, float curve)
{
    if (deadzone < 0.0f) deadzone = 0.0f;
    if (deadzone > 0.9f) deadzone = 0.9f;
    if (curve < 0.25f) curve = 0.25f;
    if (curve > 4.0f) curve = 4.0f;

    if (raw > -deadzone && raw < deadzone)
        return 0;

    float sign = (raw < 0.0f) ? -1.0f : 1.0f;
    float mag = (fabsf(raw) - deadzone) / (1.0f - deadzone);
    if (mag > 1.0f) mag = 1.0f;
    if (curve != 1.0f) mag = powf(mag, curve);

    return (int)(sign * mag * STICK_AXIS_SCALE);
}

/* Scale a clamped touch delta by sensitivity + quadratic accel ramp (fast flicks move further). */
static int ScaleTouchDelta(int raw, float sensitivity, float accel)
{
    float mag = fabsf((float)raw) / 40.0f;   /* 0..1 against the clamp */
    float mult = sensitivity + accel * mag * mag;
    return (int)((float)raw * mult);
}

/* Emit SE_MOUSE from a stick for menu cursor movement (squared curve + sub-pixel accum). */
static void InjectMenuCursor(float sx, float sy)
{
    if (sx > -STICK_DEADZONE && sx < STICK_DEADZONE) sx = 0.0f;
    if (sy > -STICK_DEADZONE && sy < STICK_DEADZONE) sy = 0.0f;

    /* Squared response for fine control at small deflections */
    float fx = sx * fabsf(sx) * MENU_CURSOR_SPEED;
    float fy = sy * fabsf(sy) * MENU_CURSOR_SPEED;

    cursor_accum_x += fx;
    cursor_accum_y += fy;

    int dx = (int)cursor_accum_x;
    int dy = (int)cursor_accum_y;

    cursor_accum_x -= (float)dx;
    cursor_accum_y -= (float)dy;

    /* Q3 mouse Y is down-positive, stick Y is up-positive -- negate. */
    if (dx || dy)
        Com_QueueEvent(0, SE_MOUSE, dx, -dy, 0, NULL);
}

/* Emit SE_JOYSTICK_AXIS with change detection */
static void InjectAxis(int axis, int value, int *prev)
{
    if (value != *prev) {
        *prev = value;
        Com_QueueEvent(0, SE_JOYSTICK_AXIS, axis, value, 0, NULL);
    }
}

/* Process a button map table against trigger/release bitmasks */
static void ProcessButtons(const buttonMap_t *map, int count,
                           uint32_t triggered, uint32_t released,
                           qboolean in_menu)
{
    int i;
    for (i = 0; i < count; i++) {
        int key = in_menu ? map[i].key_menu : map[i].key_game;
        if (!key) key = map[i].key_game;  /* fallback if menu key is 0 */
        if (in_menu && key == K_ENTER && (Key_GetCatcher() & KEYCATCH_UI) && !(Key_GetCatcher() & (KEYCATCH_CONSOLE | KEYCATCH_MESSAGE))) {
            key = K_MOUSE1;
        }

        if (triggered & map[i].mask)
            InjectKey(key, qtrue);
        if (released & map[i].mask)
            InjectKey(key, qfalse);
    }
}

/* Edge test for a two-button combo: fires once when either half is newly
 * pressed while the other is already held, so press order doesn't matter. */
static qboolean ComboEdge(uint32_t trigger, uint32_t hold, uint32_t a, uint32_t b)
{
    return ((( trigger & a) && (hold & b)) ||
            (( trigger & b) && (hold & a))) ? qtrue : qfalse;
}

/* Accept raises the OSK where text can land: console/chat, or a ui_ime_target field an
 * IME-aware ui.qvm names on focus. Stock ui.qvm never sets it, so menus keep A = Enter. */
static qboolean OSK_AcceptRaises(void)
{
    int catcher = Key_GetCatcher();

    if (catcher & (KEYCATCH_CONSOLE | KEYCATCH_MESSAGE))
        return qtrue;

    if (catcher & (KEYCATCH_UI | KEYCATCH_CGAME)) {
        const char *target = Cvar_VariableString("ui_ime_target");
        if (target && target[0] && Q_stricmp(target, "donothing") != 0)
            return qtrue;
    }

    return qfalse;
}

/* Set a default bind only if the key is currently unbound */
static void SetDefaultBind(int key, const char *binding)
{
    const char *current = Key_GetBinding(key);
    if (!current || !current[0])
        Key_SetBinding(key, binding);
}

/* ---- Rumble (GamePad motor): on/off only, so strength = pulse duration ---- */
/* A new pulse extends (not restarts) the expiry, so autofire doesn't stutter it. */
void WiiU_SetRumble(int durationMs)
{
    if (!input_initialized)
        return;
    if (!rumble_enable || !rumble_enable->integer)
        return;

    VPADControlMotor(VPAD_CHAN_0, s_rumblePattern, sizeof(s_rumblePattern));

    int expiry = Com_Milliseconds() + durationMs;
    if (!s_rumbleActive || expiry > s_rumbleExpiryMs)
        s_rumbleExpiryMs = expiry;
    s_rumbleActive = qtrue;
}

/* Stop the motor once the latest pulse has expired, called every frame. */
static void WiiU_Input_RumbleTick(void)
{
    if (!s_rumbleActive)
        return;
    if (Com_Milliseconds() < s_rumbleExpiryMs)
        return;

    VPADStopMotor(VPAD_CHAN_0);
    s_rumbleActive = qfalse;
}

/* ---- Aim assist ---- */
int WiiU_Input_NunchukActive(void)
{
    return nunchuk_active;
}

/* Probe every channel: KPAD keeps no state for a pad that disconnected. */
static void UpdateNunchukActive(void)
{
    int ch;
    nunchuk_active = qfalse;
    for (ch = 0; ch < MAX_KPAD_CHANNELS; ch++) {
        WPADExtensionType ext;
        if (WPADProbe((WPADChan)ch, &ext) == WPAD_ERROR_NONE &&
            (ext == WPAD_EXT_NUNCHUK || ext == WPAD_EXT_MPLUS_NUNCHUK)) {
            nunchuk_active = qtrue;
            return;
        }
    }
}

/* L+R toggles cl_aimAssist and restores the last non-zero mode. Masks both
 * shoulders and releases whichever one was already down, so walk/use don't stick. */
static void AimAssistCombo(uint32_t *trigger, uint32_t *release, uint32_t hold,
                           uint32_t l, uint32_t r)
{
    static int lastMode = 1;
    int mode;

    if (nunchuk_active || !ComboEdge(*trigger, hold, l, r))
        return;

    mode = (int)Cvar_VariableValue("cl_aimAssist");
    if (mode > 0) {
        lastMode = mode;
        Cvar_Set("cl_aimAssist", "0");
        Com_Printf("DISABLED AIM ASSIST\n");
    } else {
        Cvar_Set("cl_aimAssist", va("%d", lastMode));
        Com_Printf("ENABLED AIM ASSIST\n");
    }
    WiiU_SetRumble(150);

    InjectKey(K_PAD0_LEFTSHOULDER, qfalse);
    InjectKey(K_PAD0_RIGHTSHOULDER, qfalse);
    *trigger &= ~(l | r);
    *release &= ~(l | r);
}

/* ---- R3 tap/hold: a quick tap runs turn180, a longer hold acts as the bound key (+scores) ---- */
#define R3_TAP_MS 200

typedef struct {
    qboolean    pending;
    qboolean    cancelled;
    int         downMs;
} r3Tap_t;

static r3Tap_t vpad_r3_tap;
static r3Tap_t kpad_r3_tap[MAX_KPAD_CHANNELS];

/* In game only. Reads hold (not release) so a tap shorter than one sample still resolves. */
static void R3TapOrHold(r3Tap_t *t, uint32_t *trigger, uint32_t *release, uint32_t hold,
                        uint32_t r3, uint32_t l3)
{
    int now = Com_Milliseconds();

    if (*trigger & r3) {
        t->pending = qtrue;
        t->cancelled = qfalse;
        t->downMs = now;
    }
    if (t->pending) {
        /* L3 + R3 is the rumble toggle, never a turn. */
        if (hold & l3)
            t->cancelled = qtrue;
        if (!(hold & r3)) {
            if (!t->cancelled && now - t->downMs < R3_TAP_MS)
                Cbuf_ExecuteText(EXEC_APPEND, "turn180\n");
            InjectKey(K_PAD0_RIGHTSTICK_CLICK, qfalse);
            t->pending = qfalse;
        } else if (!t->cancelled && now - t->downMs >= R3_TAP_MS) {
            InjectKey(K_PAD0_RIGHTSTICK_CLICK, qtrue);
        }
    }
    *trigger &= ~r3;
    *release &= ~r3;
}

/* ---- VPAD (GamePad) polling ---- */
static void PollVPAD(qboolean in_menu)
{
    VPADStatus status;
    VPADReadError error = VPAD_READ_SUCCESS;

    int samples = VPADRead(VPAD_CHAN_0, &status, 1, &error);
    if (samples <= 0 || error != VPAD_READ_SUCCESS)
        return;

    /* Text entry (PS3/PS4 shape): L3+X console, A raises the OSK at a prompt, L3+B OSK for chat.
     * All release held keys first, or they stay down while the keyboard is up. */

    /* Console toggle: L3 + X. In-game nothing else reaches the console -- Y only
     * maps to K_CONSOLE in menu context. */
    if ((status.trigger & VPAD_BUTTON_X && status.hold & VPAD_BUTTON_STICK_L) ||
        (status.trigger & VPAD_BUTTON_STICK_L && status.hold & VPAD_BUTTON_X)) {
        ReleaseAllKeys();
        Cbuf_ExecuteText(EXEC_APPEND, "toggleconsole\n");
        status.trigger &= ~(VPAD_BUTTON_X | VPAD_BUTTON_STICK_L);
        status.release &= ~(VPAD_BUTTON_X | VPAD_BUTTON_STICK_L);
    }

    /* A opens the OSK wherever text can land -- console/chat prompt, or a
     * focused menu field. See OSK_AcceptRaises(). */
    if (OSK_AcceptRaises() && (status.trigger & VPAD_BUTTON_A)) {
        ReleaseAllKeys();
        Con_OSK_Toggle();
        status.trigger &= ~VPAD_BUTTON_A;
        status.release &= ~VPAD_BUTTON_A;
        if (Con_OSK_IsActive())
            return;
    }

    /* OSK toggle: L3 + B, fires on edge (same shape as the rumble combo below). */
    if ((status.trigger & VPAD_BUTTON_B && status.hold & VPAD_BUTTON_STICK_L) ||
        (status.trigger & VPAD_BUTTON_STICK_L && status.hold & VPAD_BUTTON_B)) {
        ReleaseAllKeys();
        Con_OSK_Toggle();
        status.trigger &= ~(VPAD_BUTTON_B | VPAD_BUTTON_STICK_L);
        status.release &= ~(VPAD_BUTTON_B | VPAD_BUTTON_STICK_L);
        if (Con_OSK_IsActive())
            return;
    }

    /* Quit combo: Plus + Minus held; mask them so ESCAPE/CONSOLE don't also fire. */
    if ((status.hold & VPAD_BUTTON_PLUS) &&
        (status.hold & VPAD_BUTTON_MINUS)) {
        quit_pressed = qtrue;
        status.trigger &= ~(VPAD_BUTTON_PLUS | VPAD_BUTTON_MINUS);
        status.release &= ~(VPAD_BUTTON_PLUS | VPAD_BUTTON_MINUS);
    }

    /* Rumble toggle: L3 + R3 on edge (L3 + X became the console). Both sticks bind
     * +scores, so the scoreboard blips for a frame -- harmless. */
    if ((status.trigger & VPAD_BUTTON_STICK_R && status.hold & VPAD_BUTTON_STICK_L) ||
        (status.trigger & VPAD_BUTTON_STICK_L && status.hold & VPAD_BUTTON_STICK_R)) {
        qboolean newState = rumble_enable->integer ? qfalse : qtrue;
        Cvar_Set("rumble_enable", newState ? "1" : "0");
        Com_Printf(newState ? "ENABLED RUMBLE\n" : "DISABLED RUMBLE\n");
        if (!newState) {
            VPADStopMotor(VPAD_CHAN_0);
            s_rumbleActive = qfalse;
        }
        /* Mask both stick clicks so +scores doesn't also fire. */
        status.trigger &= ~(VPAD_BUTTON_STICK_R | VPAD_BUTTON_STICK_L);
        status.release &= ~(VPAD_BUTTON_STICK_R | VPAD_BUTTON_STICK_L);
    }

    AimAssistCombo(&status.trigger, &status.release, status.hold,
                   VPAD_BUTTON_L, VPAD_BUTTON_R);

    if (!in_menu)
        R3TapOrHold(&vpad_r3_tap, &status.trigger, &status.release, status.hold,
                    VPAD_BUTTON_STICK_R, VPAD_BUTTON_STICK_L);

    /* Button edge detection using hardware trigger/release fields */
    ProcessButtons(vpadButtons, VPAD_BUTTON_COUNT,
                   status.trigger, status.release, in_menu);

    /* Analog sticks */
    float lx = status.leftStick.x;
    float ly = status.leftStick.y;
    float rx = status.rightStick.x;
    float ry = status.rightStick.y;

    if (in_menu) {
        /* Left stick drives menu cursor, right stick ignored */
        InjectMenuCursor(lx, ly);
    } else {
        /* Left stick strafe/forward, right stick look, both via SE_JOYSTICK_AXIS */
        InjectAxis(AXIS_SIDE,    FilterStick(lx,  stick_move_deadzone->value, stick_move_curve->value),  &vpad_prev_axis[0]);
        InjectAxis(AXIS_FORWARD, FilterStick(-ly, stick_move_deadzone->value, stick_move_curve->value),  &vpad_prev_axis[1]);
        InjectAxis(AXIS_YAW,    FilterStick(rx,  stick_look_deadzone->value, stick_look_curve->value),  &vpad_prev_axis[2]);
        InjectAxis(AXIS_PITCH,  FilterStick(-ry, stick_look_deadzone->value, stick_look_curve->value),  &vpad_prev_axis[3]);
    }

    /* Touch screen -> SE_MOUSE (delta model, clamped) */
    VPADTouchData tp;
    VPADGetTPCalibratedPoint(VPAD_CHAN_0, &tp, &status.tpNormal);

    if (tp.touched && tp.validity == VPAD_VALID) {
        if (touch_prev_valid) {
            int tdx = (int)tp.x - (int)touch_prev_x;
            int tdy = (int)tp.y - (int)touch_prev_y;

            /* Clamp to prevent jumps when finger re-lands */
            if (tdx > 40) tdx = 40;
            if (tdx < -40) tdx = -40;
            if (tdy > 40) tdy = 40;
            if (tdy < -40) tdy = -40;

            int mx = ScaleTouchDelta(tdx, touch_sensitivity->value, touch_accel->value);
            int my = ScaleTouchDelta(tdy, touch_sensitivity->value, touch_accel->value);

            if (mx || my)
                Com_QueueEvent(0, SE_MOUSE, mx, my, 0, NULL);
        }
        touch_prev_valid = qtrue;
        touch_prev_x = tp.x;
        touch_prev_y = tp.y;
    } else {
        touch_prev_valid = qfalse;
    }

}

/* ---- KPAD (Pro Controller / Classic Controller) polling ---- */
static void PollKPAD(qboolean in_menu)
{
    int ch;
    for (ch = 0; ch < MAX_KPAD_CHANNELS; ch++) {
        KPADStatus status;
        KPADError error = KPAD_ERROR_OK;

        uint32_t samples = KPADReadEx((KPADChan)ch, &status, 1, &error);
        if (samples == 0 || error != KPAD_ERROR_OK)
            continue;

        /* Release all keys if extension type changed (controller swap) */
        if (status.extensionType != kpad_prev_ext[ch]) {
            kpad_prev_ext[ch] = status.extensionType;
            memset(kpad_prev_axis[ch], 0, sizeof(kpad_prev_axis[ch]));
            pointer_prev_valid[ch] = qfalse;
            ReleaseAllKeys();
        }

        /* A raises the OSK wherever text can land -- same rule as the GamePad. */
        qboolean textPrompt = OSK_AcceptRaises();

        if (status.extensionType == WPAD_EXT_PRO_CONTROLLER) {
            /* Console: L3 + X. OSK: L3 + B. Identical to the GamePad. */
            if (ComboEdge(status.pro.trigger, status.pro.hold,
                          WPAD_PRO_BUTTON_STICK_L, WPAD_PRO_BUTTON_X)) {
                ReleaseAllKeys();
                Cbuf_ExecuteText(EXEC_APPEND, "toggleconsole\n");
                status.pro.trigger &= ~(WPAD_PRO_BUTTON_STICK_L | WPAD_PRO_BUTTON_X);
                status.pro.release &= ~(WPAD_PRO_BUTTON_STICK_L | WPAD_PRO_BUTTON_X);
            }
            if (ComboEdge(status.pro.trigger, status.pro.hold,
                          WPAD_PRO_BUTTON_STICK_L, WPAD_PRO_BUTTON_B)) {
                ReleaseAllKeys();
                Con_OSK_Toggle();
                if (Con_OSK_IsActive())
                    return;
                status.pro.trigger &= ~(WPAD_PRO_BUTTON_STICK_L | WPAD_PRO_BUTTON_B);
                status.pro.release &= ~(WPAD_PRO_BUTTON_STICK_L | WPAD_PRO_BUTTON_B);
            }
            if (textPrompt && (status.pro.trigger & WPAD_PRO_BUTTON_A)) {
                ReleaseAllKeys();
                Con_OSK_Toggle();
                if (Con_OSK_IsActive())
                    return;
                status.pro.trigger &= ~WPAD_PRO_BUTTON_A;
                status.pro.release &= ~WPAD_PRO_BUTTON_A;
            }

            AimAssistCombo(&status.pro.trigger, &status.pro.release, status.pro.hold,
                           WPAD_PRO_BUTTON_L, WPAD_PRO_BUTTON_R);

            if (!in_menu)
                R3TapOrHold(&kpad_r3_tap[ch], &status.pro.trigger, &status.pro.release,
                            status.pro.hold, WPAD_PRO_BUTTON_STICK_R, WPAD_PRO_BUTTON_STICK_L);

            ProcessButtons(proButtons, PRO_BUTTON_COUNT,
                           status.pro.trigger, status.pro.release, in_menu);

            /* Quit combo: Plus + Minus */
            if ((status.pro.hold & WPAD_PRO_BUTTON_PLUS) &&
                (status.pro.hold & WPAD_PRO_BUTTON_MINUS)) {
                quit_pressed = qtrue;
            }

            float lx = status.pro.leftStick.x;
            float ly = status.pro.leftStick.y;
            float rx = status.pro.rightStick.x;
            float ry = status.pro.rightStick.y;

            if (in_menu) {
                InjectMenuCursor(lx, ly);
            } else {
                InjectAxis(AXIS_SIDE,    FilterStick(lx,  stick_move_deadzone->value, stick_move_curve->value),  &kpad_prev_axis[ch][0]);
                InjectAxis(AXIS_FORWARD, FilterStick(-ly, stick_move_deadzone->value, stick_move_curve->value),  &kpad_prev_axis[ch][1]);
                InjectAxis(AXIS_YAW,    FilterStick(rx,  stick_look_deadzone->value, stick_look_curve->value),  &kpad_prev_axis[ch][2]);
                InjectAxis(AXIS_PITCH,  FilterStick(-ry, stick_look_deadzone->value, stick_look_curve->value),  &kpad_prev_axis[ch][3]);
            }

        } else if (status.extensionType == WPAD_EXT_CLASSIC) {
            /* No stick clicks on this pad, so the GamePad's L3 combos don't
             * exist here -- console is ZL + ZR, OSK is ZL + B. */
            if (ComboEdge(status.classic.trigger, status.classic.hold,
                          WPAD_CLASSIC_BUTTON_ZL, WPAD_CLASSIC_BUTTON_ZR)) {
                ReleaseAllKeys();
                Cbuf_ExecuteText(EXEC_APPEND, "toggleconsole\n");
                status.classic.trigger &= ~(WPAD_CLASSIC_BUTTON_ZL | WPAD_CLASSIC_BUTTON_ZR);
                status.classic.release &= ~(WPAD_CLASSIC_BUTTON_ZL | WPAD_CLASSIC_BUTTON_ZR);
            }
            if (ComboEdge(status.classic.trigger, status.classic.hold,
                          WPAD_CLASSIC_BUTTON_ZL, WPAD_CLASSIC_BUTTON_B)) {
                ReleaseAllKeys();
                Con_OSK_Toggle();
                if (Con_OSK_IsActive())
                    return;
                status.classic.trigger &= ~(WPAD_CLASSIC_BUTTON_ZL | WPAD_CLASSIC_BUTTON_B);
                status.classic.release &= ~(WPAD_CLASSIC_BUTTON_ZL | WPAD_CLASSIC_BUTTON_B);
            }
            if (textPrompt && (status.classic.trigger & WPAD_CLASSIC_BUTTON_A)) {
                ReleaseAllKeys();
                Con_OSK_Toggle();
                if (Con_OSK_IsActive())
                    return;
                status.classic.trigger &= ~WPAD_CLASSIC_BUTTON_A;
                status.classic.release &= ~WPAD_CLASSIC_BUTTON_A;
            }

            AimAssistCombo(&status.classic.trigger, &status.classic.release, status.classic.hold,
                           WPAD_CLASSIC_BUTTON_L, WPAD_CLASSIC_BUTTON_R);

            ProcessButtons(classicButtons, CLASSIC_BUTTON_COUNT,
                           status.classic.trigger, status.classic.release, in_menu);

            if ((status.classic.hold & WPAD_CLASSIC_BUTTON_PLUS) &&
                (status.classic.hold & WPAD_CLASSIC_BUTTON_MINUS)) {
                quit_pressed = qtrue;
            }

            float lx = status.classic.leftStick.x;
            float ly = status.classic.leftStick.y;
            float rx = status.classic.rightStick.x;
            float ry = status.classic.rightStick.y;

            if (in_menu) {
                InjectMenuCursor(lx, ly);
            } else {
                InjectAxis(AXIS_SIDE,    FilterStick(lx,  stick_move_deadzone->value, stick_move_curve->value),  &kpad_prev_axis[ch][0]);
                InjectAxis(AXIS_FORWARD, FilterStick(-ly, stick_move_deadzone->value, stick_move_curve->value),  &kpad_prev_axis[ch][1]);
                InjectAxis(AXIS_YAW,    FilterStick(rx,  stick_look_deadzone->value, stick_look_curve->value),  &kpad_prev_axis[ch][2]);
                InjectAxis(AXIS_PITCH,  FilterStick(-ry, stick_look_deadzone->value, stick_look_curve->value),  &kpad_prev_axis[ch][3]);
            }

        } else if (status.extensionType == WPAD_EXT_NUNCHUK) {
            /* K_CONSOLE isn't in wiimoteButtons, so console is 1 + Minus, OSK is 1 + 2.
             * Home + Minus stays the quit combo. */
            if (ComboEdge(status.trigger, status.hold,
                          WPAD_BUTTON_1, WPAD_BUTTON_MINUS)) {
                ReleaseAllKeys();
                Cbuf_ExecuteText(EXEC_APPEND, "toggleconsole\n");
                status.trigger &= ~(WPAD_BUTTON_1 | WPAD_BUTTON_MINUS);
                status.release &= ~(WPAD_BUTTON_1 | WPAD_BUTTON_MINUS);
            }
            if (ComboEdge(status.trigger, status.hold, WPAD_BUTTON_1, WPAD_BUTTON_2)) {
                ReleaseAllKeys();
                Con_OSK_Toggle();
                if (Con_OSK_IsActive())
                    return;
                status.trigger &= ~(WPAD_BUTTON_1 | WPAD_BUTTON_2);
                status.release &= ~(WPAD_BUTTON_1 | WPAD_BUTTON_2);
            }
            if (textPrompt && (status.trigger & WPAD_BUTTON_A)) {
                ReleaseAllKeys();
                Con_OSK_Toggle();
                if (Con_OSK_IsActive())
                    return;
                status.trigger &= ~WPAD_BUTTON_A;
                status.release &= ~WPAD_BUTTON_A;
            }

            /* Wiimote's own buttons (top-level hold/trigger/release) */
            ProcessButtons(wiimoteButtons, WIIMOTE_BUTTON_COUNT,
                           status.trigger, status.release, in_menu);
            /* Nunchuk Z/C live in a separate bitfield */
            ProcessButtons(nunchukButtons, NUNCHUK_BUTTON_COUNT,
                           status.nunchuk.trigger, status.nunchuk.release, in_menu);

            /* Quit combo: Home + Minus -- no stick-click available on this pad. */
            if ((status.hold & WPAD_BUTTON_HOME) && (status.hold & WPAD_BUTTON_MINUS)) {
                quit_pressed = qtrue;
            }

            /* Nunchuk stick drives movement; the IR pointer (below) drives the menu cursor instead. */
            if (!in_menu) {
                float nx = status.nunchuk.stick.x;
                float ny = status.nunchuk.stick.y;
                InjectAxis(AXIS_SIDE,    FilterStick(nx,  stick_move_deadzone->value, stick_move_curve->value),  &kpad_prev_axis[ch][0]);
                InjectAxis(AXIS_FORWARD, FilterStick(-ny, stick_move_deadzone->value, stick_move_curve->value),  &kpad_prev_axis[ch][1]);
            }

            /* IR pointer (needs a powered Sensor Bar). In-game: center-offset drives both a
             * continuous turn rate and an instant fine-aim offset (see CL_FinishMove). */
            if (status.posValid) {
                float px = status.pos.x;
                float py = status.pos.y;
                if (px >  1.0f) px =  1.0f;
                if (px < -1.0f) px = -1.0f;
                if (py >  1.0f) py =  1.0f;
                if (py < -1.0f) py = -1.0f;

                if (in_menu) {
                    wiiu_ir_aim_x = wiiu_ir_aim_y = 0.0f;
                    if (pointer_prev_valid[ch]) {
                        int mx = (int)((px - pointer_prev_x[ch]) * POINTER_MENU_SCALE);
                        int my = (int)((py - pointer_prev_y[ch]) * POINTER_MENU_SCALE);
                        if (mx || my)
                            Com_QueueEvent(0, SE_MOUSE, mx, my, 0, NULL);
                    }
                } else {
                    /* Layer 2: instant fine-aim offset, recomputed every frame (no accumulation) */
                    wiiu_ir_aim_x = px;
                    wiiu_ir_aim_y = py;

                    /* Layer 1: continuous turn rate from center-offset, like a stick held toward the edge */
                    float dx = px, dy = py;
                    if (dx > -pointer_deadzone->value && dx < pointer_deadzone->value) dx = 0.0f;
                    if (dy > -pointer_deadzone->value && dy < pointer_deadzone->value) dy = 0.0f;

                    if (dx != 0.0f || dy != 0.0f) {
                        dx *= pointer_sensitivity->value * POINTER_TURN_SCALE;
                        dy *= pointer_sensitivity->value * POINTER_TURN_SCALE;

                        float maxd = pointer_maxdelta->value;
                        if (dx >  maxd) dx =  maxd;
                        if (dx < -maxd) dx = -maxd;
                        if (dy >  maxd) dy =  maxd;
                        if (dy < -maxd) dy = -maxd;

                        int mx = (int)dx;
                        int my = (int)dy;
                        if (mx || my)
                            Com_QueueEvent(0, SE_MOUSE, mx, my, 0, NULL);
                    }
                }
                pointer_prev_valid[ch] = qtrue;
                pointer_prev_x[ch] = px;
                pointer_prev_y[ch] = py;
            } else {
                pointer_prev_valid[ch] = qfalse;
                wiiu_ir_aim_x = wiiu_ir_aim_y = 0.0f;
            }
        }
        /* Bare Wii Remote (no extension) has no analog input worth mapping for Q3, skip. */
    }
}

/* ---- OSK input gathering: every pad reduces to one oskInput_t, edges OR'd ---- */
/* Nothing else runs while the keyboard is up, so no button leaks into the game. */

/* Strongest deflection across all pads wins, so an idle stick on one pad can't
 * cancel out a real one on another. */
static void OSK_TakeNav(oskInput_t *in, float x, float y)
{
    if (fabsf(x) > fabsf(in->navX)) in->navX = x;
    if (fabsf(y) > fabsf(in->navY)) in->navY = y;
}

static void OSK_GatherVPAD(oskInput_t *in)
{
    VPADStatus status;
    VPADReadError error = VPAD_READ_SUCCESS;
    VPADTouchData tp;

    if (VPADRead(VPAD_CHAN_0, &status, 1, &error) <= 0 || error != VPAD_READ_SUCCESS)
        return;

    if (status.trigger & VPAD_BUTTON_LEFT)  in->pressLeft   = qtrue;
    if (status.trigger & VPAD_BUTTON_RIGHT) in->pressRight  = qtrue;
    if (status.trigger & VPAD_BUTTON_UP)    in->pressUp     = qtrue;
    if (status.trigger & VPAD_BUTTON_DOWN)  in->pressDown   = qtrue;
    if (status.trigger & VPAD_BUTTON_A)     in->pressAccept = qtrue;
    if (status.trigger & VPAD_BUTTON_B)     in->pressBack   = qtrue;
    if (status.trigger & VPAD_BUTTON_X)     in->pressSpace  = qtrue;
    if (status.trigger & VPAD_BUTTON_Y)     in->pressShift  = qtrue;
    if (status.trigger & VPAD_BUTTON_PLUS)  in->pressOk     = qtrue;
    if (status.trigger & VPAD_BUTTON_MINUS) in->pressCancel = qtrue;

    OSK_TakeNav(in, status.leftStick.x, status.leftStick.y);

    /* Touch taps in 854x480: only X needs rescaling to 640, since SCR_AdjustFrom640
     * is a pure stretch (its widescreen branch is #if 0). */
    VPADGetTPCalibratedPointEx(VPAD_CHAN_0, VPAD_TP_854X480, &tp, &status.tpNormal);

    if (tp.touched && tp.validity == VPAD_VALID) {
        /* Down edge only -- holding a finger down must not autofire the key. */
        if (!osk_touch_prev) {
            in->cursorPress = qtrue;
            in->cursorX = (float)tp.x * (640.0f / 854.0f);
            in->cursorY = (float)tp.y;
        }
        osk_touch_prev = qtrue;
    } else {
        osk_touch_prev = qfalse;
    }
}

static void OSK_GatherKPAD(oskInput_t *in)
{
    int ch;

    for (ch = 0; ch < MAX_KPAD_CHANNELS; ch++) {
        KPADStatus status;
        KPADError error = KPAD_ERROR_OK;

        if (KPADReadEx((KPADChan)ch, &status, 1, &error) == 0 || error != KPAD_ERROR_OK)
            continue;

        if (status.extensionType == WPAD_EXT_PRO_CONTROLLER) {
            uint32_t t = status.pro.trigger;
            if (t & WPAD_PRO_BUTTON_LEFT)  in->pressLeft   = qtrue;
            if (t & WPAD_PRO_BUTTON_RIGHT) in->pressRight  = qtrue;
            if (t & WPAD_PRO_BUTTON_UP)    in->pressUp     = qtrue;
            if (t & WPAD_PRO_BUTTON_DOWN)  in->pressDown   = qtrue;
            if (t & WPAD_PRO_BUTTON_A)     in->pressAccept = qtrue;
            if (t & WPAD_PRO_BUTTON_B)     in->pressBack   = qtrue;
            if (t & WPAD_PRO_BUTTON_X)     in->pressSpace  = qtrue;
            if (t & WPAD_PRO_BUTTON_Y)     in->pressShift  = qtrue;
            if (t & WPAD_PRO_BUTTON_PLUS)  in->pressOk     = qtrue;
            if (t & WPAD_PRO_BUTTON_MINUS) in->pressCancel = qtrue;
            OSK_TakeNav(in, status.pro.leftStick.x, status.pro.leftStick.y);

        } else if (status.extensionType == WPAD_EXT_CLASSIC) {
            uint32_t t = status.classic.trigger;
            if (t & WPAD_CLASSIC_BUTTON_LEFT)  in->pressLeft   = qtrue;
            if (t & WPAD_CLASSIC_BUTTON_RIGHT) in->pressRight  = qtrue;
            if (t & WPAD_CLASSIC_BUTTON_UP)    in->pressUp     = qtrue;
            if (t & WPAD_CLASSIC_BUTTON_DOWN)  in->pressDown   = qtrue;
            if (t & WPAD_CLASSIC_BUTTON_A)     in->pressAccept = qtrue;
            if (t & WPAD_CLASSIC_BUTTON_B)     in->pressBack   = qtrue;
            if (t & WPAD_CLASSIC_BUTTON_X)     in->pressSpace  = qtrue;
            if (t & WPAD_CLASSIC_BUTTON_Y)     in->pressShift  = qtrue;
            if (t & WPAD_CLASSIC_BUTTON_PLUS)  in->pressOk     = qtrue;
            if (t & WPAD_CLASSIC_BUTTON_MINUS) in->pressCancel = qtrue;
            OSK_TakeNav(in, status.classic.leftStick.x, status.classic.leftStick.y);

        } else if (status.extensionType == WPAD_EXT_NUNCHUK) {
            /* Held upright, so the d-pad reads naturally. B is the underside
             * trigger -- backspace here, since it's the easiest to reach. */
            uint32_t t = status.trigger;
            if (t & WPAD_BUTTON_LEFT)  in->pressLeft   = qtrue;
            if (t & WPAD_BUTTON_RIGHT) in->pressRight  = qtrue;
            if (t & WPAD_BUTTON_UP)    in->pressUp     = qtrue;
            if (t & WPAD_BUTTON_DOWN)  in->pressDown   = qtrue;
            if (t & WPAD_BUTTON_A)     in->pressAccept = qtrue;
            if (t & WPAD_BUTTON_B)     in->pressBack   = qtrue;
            if (t & WPAD_BUTTON_1)     in->pressSpace  = qtrue;
            if (t & WPAD_BUTTON_2)     in->pressShift  = qtrue;
            if (t & WPAD_BUTTON_PLUS)  in->pressOk     = qtrue;
            if (t & WPAD_BUTTON_MINUS) in->pressCancel = qtrue;
            OSK_TakeNav(in, status.nunchuk.stick.x, status.nunchuk.stick.y);

            /* IR pointer is the OSK cursor (hover + A). pos is -1..1 and already Y-down,
             * so it maps straight to 640x480. */
            if (status.posValid) {
                float px = status.pos.x;
                float py = status.pos.y;
                if (px >  1.0f) px =  1.0f;
                if (px < -1.0f) px = -1.0f;
                if (py >  1.0f) py =  1.0f;
                if (py < -1.0f) py = -1.0f;

                in->cursorValid = qtrue;
                in->cursorX = (px + 1.0f) * 0.5f * 640.0f;
                in->cursorY = (py + 1.0f) * 0.5f * 480.0f;
            }
        }
    }
}

/* ---- Public interface ---- */

void WiiU_Input_Init(void)
{
    if (input_initialized)
        return;

    VPADInit();
    KPADInit();
    WPADEnableURCC(TRUE);

    {
        int ch;
        for (ch = 0; ch < MAX_KPAD_CHANNELS; ch++)
            KPADEnableDPD((KPADChan)ch);   /* IR pointing; needs a USB-powered Sensor Bar to report valid data */
    }

    memset(key_held, 0, sizeof(key_held));
    memset(vpad_prev_axis, 0, sizeof(vpad_prev_axis));
    memset(kpad_prev_ext, 0xFF, sizeof(kpad_prev_ext));
    memset(kpad_prev_axis, 0, sizeof(kpad_prev_axis));
    memset(pointer_prev_valid, 0, sizeof(pointer_prev_valid));
    wiiu_ir_aim_x = wiiu_ir_aim_y = 0.0f;
    nunchuk_active = qfalse;

    memset(s_rumblePattern, 0xFF, sizeof(s_rumblePattern));
    s_rumbleActive = qfalse;
    s_rumbleExpiryMs = 0;

    cursor_accum_x = 0.0f;
    cursor_accum_y = 0.0f;
    touch_prev_valid = qfalse;
    quit_pressed = qfalse;
    prev_in_menu = qfalse;

    input_initialized = qtrue;
}

void WiiU_Input_Shutdown(void)
{
    if (!input_initialized)
        return;
    /* Stop any in-flight rumble before VPADShutdown, or the motor pattern gets stuck with no way to cancel it. */
    VPADStopMotor(VPAD_CHAN_0);
    s_rumbleActive = qfalse;
    /* Leaving KPAD/VPAD alive at exit stalls the OS finalizers -- cost a whole freeze investigation. */
    KPADShutdown();
    VPADShutdown();
    input_initialized = qfalse;
}

void WiiU_Input_Frame(void)
{
    if (!input_initialized)
        return;

    qboolean in_menu = InMenu();

    /* Release all keys on mode transition to prevent stuck keys */
    if (in_menu != prev_in_menu) {
        ReleaseAllKeys();
        cursor_accum_x = 0.0f;
        cursor_accum_y = 0.0f;
        touch_prev_valid = qfalse;
        memset(pointer_prev_valid, 0, sizeof(pointer_prev_valid));
        wiiu_ir_aim_x = wiiu_ir_aim_y = 0.0f;
        memset(vpad_prev_axis, 0, sizeof(vpad_prev_axis));
        memset(kpad_prev_axis, 0, sizeof(kpad_prev_axis));
        memset(&vpad_r3_tap, 0, sizeof(vpad_r3_tap));
        memset(kpad_r3_tap, 0, sizeof(kpad_r3_tap));
        prev_in_menu = in_menu;
    }

    /* Clear ui_ime_target as the UI opens so a stale field name can't misroute the OSK (from PS3).
     * Tracks the UI catcher only, not InMenu(), which also counts the console. */
    {
        static qboolean prev_in_ui;
        qboolean in_ui = (Key_GetCatcher() &
                          (KEYCATCH_UI | KEYCATCH_CGAME)) ? qtrue : qfalse;
        if (in_ui && !prev_in_ui)
            Cvar_Set("ui_ime_target", "");
        prev_in_ui = in_ui;
    }

    /* The OSK takes the whole frame's input across every pad -- no normal
     * mapping runs, so nothing leaks into the game or the text field. */
    if (Con_OSK_IsActive()) {
        oskInput_t osk;
        memset(&osk, 0, sizeof(osk));
        OSK_GatherVPAD(&osk);
        OSK_GatherKPAD(&osk);
        Con_OSK_Frame(&osk);

        /* Stale deltas would fire as a huge jump on the frame it closes. */
        touch_prev_valid = qfalse;
        memset(pointer_prev_valid, 0, sizeof(pointer_prev_valid));
        WiiU_Input_RumbleTick();
        return;
    }

    UpdateNunchukActive();
    PollVPAD(in_menu);
    PollKPAD(in_menu);
    WiiU_Input_RumbleTick();
}

int WiiU_Input_QuitPressed(void)
{
    return quit_pressed;
}

void WiiU_Input_ReleaseForeground(void)
{
    if (!input_initialized)
        return;
    VPADStopMotor(VPAD_CHAN_0);
    s_rumbleActive = qfalse;
    /* Drop the OSK rather than resume it half-typed after the HOME overlay. */
    Con_OSK_Cancel();
}

/* ---- ioq3 input interface (called by engine via IN_* function pointers) ---- */

void IN_Init(void *windowData)
{
    (void)windowData;

    /* Default gameplay binds (only if unbound), mirrors the vWii port's DRC layout. */
    SetDefaultBind(K_PAD0_A,                "+moveup");       /* jump */
    SetDefaultBind(K_PAD0_B,                "+movedown");     /* crouch */
    SetDefaultBind(K_PAD0_X,                "weapnext");
    SetDefaultBind(K_PAD0_Y,                "weapprev");
    SetDefaultBind(K_PAD0_LEFTSHOULDER,     "+speed");        /* walk */
    SetDefaultBind(K_PAD0_RIGHTSHOULDER,    "+button2");      /* use item */
    SetDefaultBind(K_PAD0_LEFTTRIGGER,      "+zoom");
    SetDefaultBind(K_PAD0_RIGHTTRIGGER,     "+attack");       /* fire */
#ifdef ELITEFORCE
    /* EF's cgame reads "+info" for the scoreboard, not Q3's "+scores". */
    SetDefaultBind(K_PAD0_LEFTSTICK_CLICK,  "+info");
    SetDefaultBind(K_PAD0_RIGHTSTICK_CLICK, "+info");
    SetDefaultBind(K_PAD0_BACK,             "+info");         /* Minus */
#else
    SetDefaultBind(K_PAD0_LEFTSTICK_CLICK,  "+scores");
    SetDefaultBind(K_PAD0_RIGHTSTICK_CLICK, "+scores");
    SetDefaultBind(K_PAD0_BACK,             "+scores");       /* Minus */
#endif
    SetDefaultBind(K_PAD0_DPAD_UP,          "weapnext");
    SetDefaultBind(K_PAD0_DPAD_DOWN,        "weapprev");
    SetDefaultBind(K_PAD0_DPAD_LEFT,        "weapprev");
    SetDefaultBind(K_PAD0_DPAD_RIGHT,       "weapnext");

    /* Joystick axis cvars set on the command line in wiiu_main.c. */
    Cvar_SetSafe("in_joystick", "1");
    Cvar_SetSafe("in_joystickUseAnalog", "1");

    /* Stick deadzone/curve, applied in FilterStick(). Defaults = old hardcoded linear behavior. */
    stick_move_deadzone = Cvar_Get("stick_move_deadzone", "0.15", CVAR_ARCHIVE);
    stick_move_curve    = Cvar_Get("stick_move_curve",    "1.0",  CVAR_ARCHIVE);
    stick_look_deadzone = Cvar_Get("stick_look_deadzone", "0.15", CVAR_ARCHIVE);
    stick_look_curve    = Cvar_Get("stick_look_curve",    "1.0",  CVAR_ARCHIVE);

    /* Touch response tuning, scaled in ScaleTouchDelta(). touch_accel 0.0 = flat multiply. */
    touch_sensitivity   = Cvar_Get("touch_sensitivity",   "1.5",  CVAR_ARCHIVE);
    touch_accel         = Cvar_Get("touch_accel",         "0.0",  CVAR_ARCHIVE);

    /* IR pointer aim tuning; mirrors vWii defaults, untested against a real Sensor Bar. */
    pointer_deadzone    = Cvar_Get("wiimote_pointer_deadzone",    "0.12", CVAR_ARCHIVE);
    pointer_sensitivity = Cvar_Get("wiimote_pointer_sensitivity", "0.30", CVAR_ARCHIVE);
    pointer_maxdelta    = Cvar_Get("wiimote_pointer_maxdelta",    "50",   CVAR_ARCHIVE);
    pointer_yawrange    = Cvar_Get("wiimote_pointer_yawrange",    "50",   CVAR_ARCHIVE);
    pointer_pitchrange  = Cvar_Get("wiimote_pointer_pitchrange",  "30",   CVAR_ARCHIVE);

    /* Rumble on weapon fire/damage (see snd_dma.c). Toggle with L3+R3. */
    rumble_enable       = Cvar_Get("rumble_enable",       "1",    CVAR_ARCHIVE);
}

void IN_Shutdown(void)
{
    /* Nothing to do; WiiU_Input_Shutdown handles hardware */
}

void IN_Frame(void)
{
    /* Polling done in WiiU_Input_Frame(), called before Com_Frame(). */
}

void IN_Restart(void)
{
    IN_Shutdown();
    IN_Init(NULL);
}
