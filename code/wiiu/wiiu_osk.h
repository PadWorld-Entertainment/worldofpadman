/*
 * wiiu_osk.h -- custom on-screen keyboard for the Wii U port.
 *
 * nn::swkbd (the system keyboard) hangs forever inside Create() on real
 * hardware and has no known-good homebrew call sequence, so this draws its own
 * key grid through the engine's 2D path and injects the result as SE_CHAR
 * events. The engine-integration half (inject/auto-submit/field-clear) is
 * ported from the PS3 port's code/input/ps3_osk.c, which solves the same
 * problem against a system keyboard that does work.
 */

#ifndef WIIU_OSK_H
#define WIIU_OSK_H

#include "qcommon/q_shared.h"

/*
 * Controller-neutral input for one frame. wiiu_input.c fills this from the
 * GamePad, Pro Controller, Classic Controller and Wii Remote alike -- the
 * keyboard itself knows nothing about VPAD/KPAD, so no pad is privileged and
 * several can drive it at once (edges are OR'd together).
 */
typedef struct {
    /* Edge-triggered, true for exactly the frame the button went down. */
    qboolean pressLeft;
    qboolean pressRight;
    qboolean pressUp;
    qboolean pressDown;
    qboolean pressAccept;   /* type the selected key */
    qboolean pressBack;     /* backspace */
    qboolean pressSpace;
    qboolean pressShift;
    qboolean pressOk;       /* commit and close */
    qboolean pressCancel;   /* close, discarding */

    /* Analog navigation, -1..1, Y up-positive. Auto-repeats. */
    float    navX;
    float    navY;

    /*
     * Absolute cursor in 640x480 virtual coords -- DRC touch or Wii Remote IR.
     * cursorValid means "pointing at the screen right now": the selection
     * follows it, and a pointer is drawn. cursorPress is the tap edge, which
     * selects and activates in one go (touch has no hover phase).
     */
    qboolean cursorValid;
    float    cursorX;
    float    cursorY;
    qboolean cursorPress;
} oskInput_t;

/* Open the keyboard, picking mode from the current key catcher, or close it
 * again if already open. Raised by a per-controller combo (L3+B on GamePad and
 * Pro, ZL+B on Classic, 1+2 on Wii Remote) or by the accept button while the
 * console / chat prompt holds the key catcher. */
void     WiiU_OSK_Toggle(void);

/* Close without committing (used on shutdown / foreground loss). */
void     WiiU_OSK_Cancel(void);

qboolean WiiU_OSK_IsActive(void);

/* Drive the keyboard for one frame. Only called while active; every pad's
 * normal mapping is suppressed for that frame. */
void     WiiU_OSK_Frame(const oskInput_t *in);

/* Draw the overlay. Called from SCR_DrawScreenField() after everything else. */
void     WiiU_OSK_Draw(void);

#endif /* WIIU_OSK_H */
