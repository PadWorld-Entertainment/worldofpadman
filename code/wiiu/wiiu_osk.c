/*
 * wiiu_osk.c -- custom on-screen keyboard for the Wii U port.
 *
 * Why custom: nn::swkbd links clean but hangs forever inside Create() on real
 * hardware (bisected 2026-07-10), and WiiUBrew still documents no known-good
 * homebrew call sequence for it. So the key grid is drawn here through the
 * engine's own 2D path (re.DrawStretchPic + cls.charsetShader), which is
 * hardware-validated, and the result is fed back as ordinary key/char events.
 *
 * The result-delivery half is ported from the PS3 port's code/input/ps3_osk.c:
 * per-character SE_CHAR, K_ENTER for auto-submit, and K_END + 80 backspaces to
 * wipe a menu field. Those semantics are load-bearing -- don't "simplify" them.
 *
 * Controller-agnostic: wiiu_input.c reduces every pad to one oskInput_t per
 * frame, so GamePad touch and Wii Remote IR both arrive as the same absolute
 * cursor and nothing here needs to know which pad is driving.
 */

#include <string.h>

#include "qcommon/q_shared.h"
#include "qcommon/qcommon.h"
/* client.h already pulls keys.h/keycodes.h; including them again by a different
 * path re-declares qkey_t. */
#include "client/client.h"
#include "../wiiu/wiiu_osk.h"

/* ----------------------------------------------------------------
 * Layout -- all geometry in Q3's 640x480 virtual space
 * ---------------------------------------------------------------- */
#define OSK_CHAR_ROWS   4
#define OSK_FUNC_ROW    OSK_CHAR_ROWS
#define OSK_ROWS        (OSK_CHAR_ROWS + 1)
#define OSK_COLS        12
#define OSK_FUNC_COUNT  6           /* each func key spans 2 columns */

#define OSK_KEY_W       40.0f
#define OSK_KEY_H       30.0f
#define OSK_GAP         4.0f
#define OSK_PAD         10.0f

#define OSK_GRID_W      (OSK_COLS * (OSK_KEY_W + OSK_GAP) - OSK_GAP)     /* 524 */
#define OSK_PANEL_W     (OSK_GRID_W + 2.0f * OSK_PAD)                    /* 544 */
#define OSK_PANEL_X     ((640.0f - OSK_PANEL_W) * 0.5f)                  /* 48 */

#define OSK_TITLE_H     16.0f
#define OSK_FIELD_H     24.0f
#define OSK_HEAD_H      (OSK_TITLE_H + 4.0f + OSK_FIELD_H + 8.0f)        /* 52 */
#define OSK_GRID_H      (OSK_ROWS * (OSK_KEY_H + OSK_GAP) - OSK_GAP)     /* 166 */
#define OSK_PANEL_H     (2.0f * OSK_PAD + OSK_HEAD_H + OSK_GRID_H)       /* 238 */
#define OSK_PANEL_Y     (480.0f - OSK_PANEL_H - 16.0f)                   /* 226 */

#define OSK_GRID_X      (OSK_PANEL_X + OSK_PAD)
#define OSK_GRID_Y      (OSK_PANEL_Y + OSK_PAD + OSK_HEAD_H)

#define OSK_TEXT_W      10.0f       /* field/label glyph cell */
#define OSK_TEXT_H      14.0f

#define OSK_MAX_TEXT    256

/* Stick-as-dpad auto-repeat */
#define OSK_STICK_ON    0.55f
#define OSK_STICK_OFF   0.35f       /* hysteresis so it doesn't chatter at the edge */
#define OSK_REPEAT_FIRST 300
#define OSK_REPEAT_NEXT  90

static const char *osk_rows_lower[OSK_CHAR_ROWS] = {
    "1234567890-=",
    "qwertyuiop[]",
    "asdfghjkl;'",
    "zxcvbnm,./\\",
};

static const char *osk_rows_upper[OSK_CHAR_ROWS] = {
    "!@#$%^&*()_+",
    "QWERTYUIOP{}",
    "ASDFGHJKL:\"",
    "ZXCVBNM<>?|",
};

enum {
    OSK_FUNC_SHIFT = 0,
    OSK_FUNC_SPACE,
    OSK_FUNC_BKSP,
    OSK_FUNC_CLEAR,
    OSK_FUNC_OK,
    OSK_FUNC_CANCEL,
};

static const char *osk_func_labels[OSK_FUNC_COUNT] = {
    "SHIFT", "SPACE", "BKSP", "CLEAR", "OK", "CANCEL"
};

/* ----------------------------------------------------------------
 * State
 * ---------------------------------------------------------------- */
static qboolean osk_active;
static qboolean osk_shift;

static char     osk_text[OSK_MAX_TEXT];
static int      osk_len;
static int      osk_maxlen;
static char     osk_title[64];

/* Delivery mode, decided at open time from the key catcher (mirrors the PS3
 * port's console / chat / menu split). */
static qboolean osk_auto_submit;
static qboolean osk_prepend_slash;
static qboolean osk_field_clear;
static qboolean osk_run_as_say;     /* in-game, no catcher: run "say ..." directly */

/* ui_ime_target snapshotted at open time. Non-empty means the UI told us which
 * field is focused, so the result goes back through the ui_ime_* cvars instead
 * of being typed blind. Empty means no IME-aware UI is loaded. */
static char     osk_ime_field[64];

static int      osk_row, osk_col;

static int      osk_nav_dir_x, osk_nav_dir_y;
static int      osk_nav_next_ms;

/* Last absolute cursor (DRC touch / Wii Remote IR), kept for drawing. */
static qboolean osk_cursor_valid;
static float    osk_cursor_x, osk_cursor_y;

/* ----------------------------------------------------------------
 * Helpers
 * ---------------------------------------------------------------- */

static int OSK_RowLen(int row)
{
    if (row == OSK_FUNC_ROW)
        return OSK_FUNC_COUNT;
    return (int)strlen(osk_shift ? osk_rows_upper[row] : osk_rows_lower[row]);
}

static int OSK_CharAt(int row, int col)
{
    const char *r = osk_shift ? osk_rows_upper[row] : osk_rows_lower[row];
    if (col < 0 || col >= (int)strlen(r))
        return 0;
    return (unsigned char)r[col];
}

/* Char rows are centred when they hold fewer than OSK_COLS keys. */
static void OSK_KeyRect(int row, int col, float *x, float *y, float *w, float *h)
{
    *y = OSK_GRID_Y + (float)row * (OSK_KEY_H + OSK_GAP);
    *h = OSK_KEY_H;

    if (row == OSK_FUNC_ROW) {
        *w = 2.0f * OSK_KEY_W + OSK_GAP;
        *x = OSK_GRID_X + (float)col * 2.0f * (OSK_KEY_W + OSK_GAP);
    } else {
        int len = OSK_RowLen(row);
        float indent = (float)(OSK_COLS - len) * (OSK_KEY_W + OSK_GAP) * 0.5f;
        *w = OSK_KEY_W;
        *x = OSK_GRID_X + indent + (float)col * (OSK_KEY_W + OSK_GAP);
    }
}

static void OSK_ClampCursor(void)
{
    int len;

    if (osk_row < 0) osk_row = 0;
    if (osk_row >= OSK_ROWS) osk_row = OSK_ROWS - 1;

    len = OSK_RowLen(osk_row);
    if (osk_col < 0) osk_col = 0;
    if (osk_col >= len) osk_col = len - 1;
}

static void OSK_AppendChar(int ch)
{
    if (ch < 32 || ch > 126)
        return;
    if (osk_len >= osk_maxlen || osk_len >= OSK_MAX_TEXT - 1)
        return;
    osk_text[osk_len++] = (char)ch;
    osk_text[osk_len] = '\0';
}

static void OSK_Backspace(void)
{
    if (osk_len > 0)
        osk_text[--osk_len] = '\0';
}

/* ----------------------------------------------------------------
 * Result delivery -- ported from ps3_osk.c's osk_inject_result()
 * ---------------------------------------------------------------- */
static void OSK_InjectResult(void)
{
    int i;

    if (osk_field_clear) {
        /* Move to end, then backspace-clear. The field wants ch==8, NOT
         * K_BACKSPACE (127) -- that distinction cost the PS3 port a cycle. */
        Com_QueueEvent(0, SE_KEY, K_END, qtrue,  0, NULL);
        Com_QueueEvent(0, SE_KEY, K_END, qfalse, 0, NULL);
        for (i = 0; i < 80; i++)
            Com_QueueEvent(0, SE_CHAR, 8, 0, 0, NULL);
    }

    if (osk_prepend_slash)
        Com_QueueEvent(0, SE_CHAR, '/', 0, 0, NULL);

    for (i = 0; i < osk_len; i++) {
        int ch = (unsigned char)osk_text[i];
        if (ch >= 32 && ch <= 126)
            Com_QueueEvent(0, SE_CHAR, ch, 0, 0, NULL);
    }

    if (osk_auto_submit) {
        Com_QueueEvent(0, SE_KEY, K_ENTER, qtrue,  0, NULL);
        Com_QueueEvent(0, SE_KEY, K_ENTER, qfalse, 0, NULL);
    }
}

/* In-game with no key catcher there is no field to type into, so the text goes
 * straight to the command buffer as a chat line. Strip the characters that
 * would break out of the quoted argument. */
static void OSK_RunAsSay(void)
{
    char clean[OSK_MAX_TEXT];
    int i, out = 0;

    for (i = 0; i < osk_len; i++) {
        char c = osk_text[i];
        if (c == '"' || c == ';' || c == '\n' || c == '\r')
            continue;
        clean[out++] = c;
    }
    clean[out] = '\0';

    if (out > 0)
        Cbuf_ExecuteText(EXEC_APPEND, va("say \"%s\"\n", clean));
}

/* Hand the result to an IME-aware UI. Team Arena's g_editingField path never
 * fires here, so write the field cvars directly and let the QVM pick them up on
 * its next frame -- the same handshake ps3_osk.c uses. osk_text is already
 * filtered to printable ASCII by OSK_AppendChar. */
static void OSK_DeliverToImeField(void)
{
    Cvar_Set("ui_ime_text",  osk_text);
    Cvar_Set("ui_ime_field", osk_ime_field);
    Cvar_Set("ui_ime_done",  "1");
    osk_ime_field[0] = '\0';
}

static void OSK_Commit(void)
{
    if (osk_ime_field[0])
        OSK_DeliverToImeField();
    else if (osk_run_as_say)
        OSK_RunAsSay();
    else
        OSK_InjectResult();

    osk_active = qfalse;
}

/* ----------------------------------------------------------------
 * Key activation
 * ---------------------------------------------------------------- */
static void OSK_PressSelected(void)
{
    if (osk_row != OSK_FUNC_ROW) {
        OSK_AppendChar(OSK_CharAt(osk_row, osk_col));
        return;
    }

    switch (osk_col) {
    case OSK_FUNC_SHIFT:
        osk_shift = osk_shift ? qfalse : qtrue;
        break;
    case OSK_FUNC_SPACE:
        OSK_AppendChar(' ');
        break;
    case OSK_FUNC_BKSP:
        OSK_Backspace();
        break;
    case OSK_FUNC_CLEAR:
        osk_len = 0;
        osk_text[0] = '\0';
        break;
    case OSK_FUNC_OK:
        OSK_Commit();
        break;
    case OSK_FUNC_CANCEL:
        osk_active = qfalse;
        break;
    }
}

/* Moving between a 12-wide char row and the 6-wide function row keeps the
 * cursor roughly under the finger instead of snapping to column 0. */
static void OSK_MoveRow(int delta)
{
    int from = osk_row;
    int to = osk_row + delta;

    if (to < 0) to = OSK_ROWS - 1;
    if (to >= OSK_ROWS) to = 0;

    if (to == OSK_FUNC_ROW && from != OSK_FUNC_ROW)
        osk_col /= 2;
    else if (from == OSK_FUNC_ROW && to != OSK_FUNC_ROW)
        osk_col *= 2;

    osk_row = to;
    OSK_ClampCursor();
}

static void OSK_MoveCol(int delta)
{
    int len = OSK_RowLen(osk_row);

    osk_col += delta;
    if (osk_col < 0) osk_col = len - 1;
    if (osk_col >= len) osk_col = 0;
}

/* ----------------------------------------------------------------
 * Open / close
 * ---------------------------------------------------------------- */
static void OSK_Open(void)
{
    int catcher = Key_GetCatcher();

    memset(osk_text, 0, sizeof(osk_text));
    osk_len   = 0;
    osk_shift = qfalse;
    osk_row   = 1;
    osk_col   = 0;
    osk_nav_dir_x = osk_nav_dir_y = 0;
    osk_nav_next_ms = 0;
    osk_cursor_valid = qfalse;

    osk_auto_submit   = qfalse;
    osk_prepend_slash = qfalse;
    osk_field_clear   = qfalse;
    osk_run_as_say    = qfalse;
    osk_ime_field[0]  = '\0';

    if (catcher & KEYCATCH_CONSOLE) {
        /* '/' so the console treats the line as a command, then Enter. */
        Q_strncpyz(osk_title, "Console Command", sizeof(osk_title));
        osk_maxlen = 128;
        osk_auto_submit   = qtrue;
        osk_prepend_slash = qtrue;
    } else if (catcher & KEYCATCH_MESSAGE) {
        Q_strncpyz(osk_title, "Chat", sizeof(osk_title));
        osk_maxlen = 128;
        osk_auto_submit = qtrue;
    } else if (catcher & (KEYCATCH_UI | KEYCATCH_CGAME)) {
        const char *target = Cvar_VariableString("ui_ime_target");

        osk_maxlen = 80;

        if (target && target[0] && Q_stricmp(target, "donothing") != 0) {
            /* An IME-aware ui.qvm named the focused field, so deliver exactly
             * to it via the ui_ime_* cvars -- no blind typing, no field wipe. */
            Q_strncpyz(osk_ime_field, target, sizeof(osk_ime_field));
            Com_sprintf(osk_title, sizeof(osk_title), "Enter: %s", target);
        } else {
            /* Stock ui.qvm, or nothing focused: fall back to typing blind into
             * whatever has focus, wiping it first. */
            Q_strncpyz(osk_title, "Enter Text", sizeof(osk_title));
            osk_field_clear = qtrue;
        }
    } else {
        Q_strncpyz(osk_title, "Say", sizeof(osk_title));
        osk_maxlen = 128;
        osk_run_as_say = qtrue;
    }

    osk_active = qtrue;
}

void WiiU_OSK_Toggle(void)
{
    if (osk_active)
        osk_active = qfalse;
    else
        OSK_Open();
}

void WiiU_OSK_Cancel(void)
{
    osk_active = qfalse;
}

qboolean WiiU_OSK_IsActive(void)
{
    return osk_active;
}

/* ----------------------------------------------------------------
 * Input
 * ---------------------------------------------------------------- */

/* Left stick behaves as a d-pad with auto-repeat; returns the step to apply
 * this frame for one axis. */
static int OSK_StickStep(float v, int *dir)
{
    if (*dir != 0) {
        /* Released (below the lower hysteresis threshold)? */
        if ((*dir > 0 && v < OSK_STICK_OFF) || (*dir < 0 && v > -OSK_STICK_OFF)) {
            *dir = 0;
            return 0;
        }
        return *dir;    /* held; repeat timing handled by caller */
    }

    if (v > OSK_STICK_ON)  { *dir =  1; return 1; }
    if (v < -OSK_STICK_ON) { *dir = -1; return -1; }
    return 0;
}

/* Move the selection to the key under an absolute cursor. Returns false when
 * the cursor is off the grid, which leaves the previous selection alone. */
static qboolean OSK_SelectAt(float cx, float cy)
{
    int row, col;

    for (row = 0; row < OSK_ROWS; row++) {
        int len = OSK_RowLen(row);
        for (col = 0; col < len; col++) {
            float kx, ky, kw, kh;
            OSK_KeyRect(row, col, &kx, &ky, &kw, &kh);
            if (cx >= kx && cx < kx + kw && cy >= ky && cy < ky + kh) {
                osk_row = row;
                osk_col = col;
                return qtrue;
            }
        }
    }
    return qfalse;
}

void WiiU_OSK_Frame(const oskInput_t *in)
{
    int now = Com_Milliseconds();
    int stepX, stepY;

    if (!osk_active)
        return;

    /* Absolute cursor (Wii Remote IR) hovers: the selection tracks the pointer,
     * so the accept button then types whatever is under it. */
    osk_cursor_valid = in->cursorValid;
    if (in->cursorValid) {
        osk_cursor_x = in->cursorX;
        osk_cursor_y = in->cursorY;
        OSK_SelectAt(in->cursorX, in->cursorY);
    }

    /* Tap edge (DRC touch): select and activate in one go -- no hover phase. */
    if (in->cursorPress) {
        if (OSK_SelectAt(in->cursorX, in->cursorY)) {
            OSK_PressSelected();
            if (!osk_active)
                return;
        }
    }

    /* D-pad: one step per press. */
    if (in->pressLeft)  OSK_MoveCol(-1);
    if (in->pressRight) OSK_MoveCol(1);
    if (in->pressUp)    OSK_MoveRow(-1);
    if (in->pressDown)  OSK_MoveRow(1);

    /* Stick: same movement with auto-repeat. */
    stepX = OSK_StickStep(in->navX, &osk_nav_dir_x);
    stepY = OSK_StickStep(in->navY, &osk_nav_dir_y);

    if (stepX == 0 && stepY == 0) {
        osk_nav_next_ms = 0;
    } else if (osk_nav_next_ms == 0) {
        if (stepX) OSK_MoveCol(stepX);
        if (stepY) OSK_MoveRow(-stepY);     /* stick Y is up-positive, rows grow down */
        osk_nav_next_ms = now + OSK_REPEAT_FIRST;
    } else if (now >= osk_nav_next_ms) {
        if (stepX) OSK_MoveCol(stepX);
        if (stepY) OSK_MoveRow(-stepY);
        osk_nav_next_ms = now + OSK_REPEAT_NEXT;
    }

    /* Buttons. Shift has its own button as well as a grid key so it's reachable
     * without crossing the grid. */
    if (in->pressAccept) {
        OSK_PressSelected();
        if (!osk_active)
            return;             /* hit OK/CANCEL -- don't let pressOk commit twice */
    }
    if (in->pressBack)   OSK_Backspace();
    if (in->pressSpace)  OSK_AppendChar(' ');
    if (in->pressShift)  osk_shift = osk_shift ? qfalse : qtrue;
    if (in->pressOk)     OSK_Commit();
    if (in->pressCancel) osk_active = qfalse;

    if (!osk_active)
        return;

    OSK_ClampCursor();
}

/* ----------------------------------------------------------------
 * Drawing -- engine 2D path, same as SCR_DrawChar but sized per call
 * ---------------------------------------------------------------- */
static const float osk_color_panel[4]  = { 0.02f, 0.02f, 0.04f, 0.92f };
static const float osk_color_border[4] = { 0.45f, 0.45f, 0.50f, 0.95f };
static const float osk_color_field[4]  = { 0.10f, 0.10f, 0.14f, 0.95f };
static const float osk_color_key[4]    = { 0.22f, 0.22f, 0.28f, 0.95f };
static const float osk_color_sel[4]    = { 0.85f, 0.62f, 0.10f, 0.95f };
static const float osk_color_shift[4]  = { 0.30f, 0.55f, 0.85f, 0.95f };

static void OSK_DrawChar(float x, float y, float w, float h, int ch)
{
    float frow, fcol, vsize, hsize;
    int row, col;

    ch &= 255;
    if (ch == ' ')
        return;

    SCR_AdjustFrom640(&x, &y, &w, &h);

    row = ch >> 4;
    col = ch & 15;

    frow = row * 0.0625f;
    fcol = col * 0.0625f;
    vsize = 0.0625f;
#ifdef ELITEFORCE
    /* EF's "gfx/2d/charsgrid_med" is a 32x16 atlas, not vanilla's 16x16 grid. */
    hsize = 0.03125f;
#else
    hsize = 0.0625f;
#endif

    re.DrawStretchPic(x, y, w, h, fcol, frow, fcol + hsize, frow + vsize,
                      cls.charsetShader);
}

static void OSK_DrawString(float x, float y, float cw, float ch, const char *s)
{
    while (*s) {
        OSK_DrawChar(x, y, cw, ch, (unsigned char)*s);
        x += cw;
        s++;
    }
}

static void OSK_DrawStringCentered(float x, float y, float w, float cw, float ch,
                                   const char *s)
{
    float sw = (float)strlen(s) * cw;
    OSK_DrawString(x + (w - sw) * 0.5f, y, cw, ch, s);
}

/* 1px-ish outline drawn as four fills; cheaper than a border shader. */
static void OSK_DrawOutline(float x, float y, float w, float h, const float *color)
{
    SCR_FillRect(x, y, w, 1.0f, color);
    SCR_FillRect(x, y + h - 1.0f, w, 1.0f, color);
    SCR_FillRect(x, y, 1.0f, h, color);
    SCR_FillRect(x + w - 1.0f, y, 1.0f, h, color);
}

void WiiU_OSK_Draw(void)
{
    float x, y, w, h;
    int row, col;
    const char *shown;
    int maxShown;

    if (!osk_active)
        return;

    /* Panel */
    SCR_FillRect(OSK_PANEL_X, OSK_PANEL_Y, OSK_PANEL_W, OSK_PANEL_H, osk_color_panel);
    OSK_DrawOutline(OSK_PANEL_X, OSK_PANEL_Y, OSK_PANEL_W, OSK_PANEL_H, osk_color_border);

    /* Title */
    OSK_DrawString(OSK_PANEL_X + OSK_PAD, OSK_PANEL_Y + OSK_PAD,
                   8.0f, OSK_TITLE_H - 2.0f, osk_title);

    /* Text field */
    x = OSK_PANEL_X + OSK_PAD;
    y = OSK_PANEL_Y + OSK_PAD + OSK_TITLE_H + 4.0f;
    w = OSK_GRID_W;
    h = OSK_FIELD_H;
    SCR_FillRect(x, y, w, h, osk_color_field);
    OSK_DrawOutline(x, y, w, h, osk_color_border);

    /* Scroll to keep the tail (and the caret) visible on long lines. */
    maxShown = (int)((w - 8.0f) / OSK_TEXT_W) - 1;
    if (maxShown < 1)
        maxShown = 1;
    shown = osk_text;
    if (osk_len > maxShown)
        shown = osk_text + (osk_len - maxShown);

    OSK_DrawString(x + 4.0f, y + (h - OSK_TEXT_H) * 0.5f, OSK_TEXT_W, OSK_TEXT_H, shown);
    OSK_DrawChar(x + 4.0f + (float)strlen(shown) * OSK_TEXT_W,
                 y + (h - OSK_TEXT_H) * 0.5f, OSK_TEXT_W, OSK_TEXT_H,
                 (cls.realtime >> 8) & 1 ? '_' : ' ');

    /* Keys */
    for (row = 0; row < OSK_ROWS; row++) {
        int len = OSK_RowLen(row);

        for (col = 0; col < len; col++) {
            qboolean selected = (row == osk_row && col == osk_col) ? qtrue : qfalse;
            const float *bg;

            OSK_KeyRect(row, col, &x, &y, &w, &h);

            if (selected)
                bg = osk_color_sel;
            else if (row == OSK_FUNC_ROW && col == OSK_FUNC_SHIFT && osk_shift)
                bg = osk_color_shift;
            else
                bg = osk_color_key;

            SCR_FillRect(x, y, w, h, bg);

            if (row == OSK_FUNC_ROW) {
                OSK_DrawStringCentered(x, y + (h - 12.0f) * 0.5f, w, 8.0f, 12.0f,
                                       osk_func_labels[col]);
            } else {
                char label[2];
                label[0] = (char)OSK_CharAt(row, col);
                label[1] = '\0';
                OSK_DrawStringCentered(x, y + (h - 16.0f) * 0.5f, w, 12.0f, 16.0f, label);
            }
        }
    }

    /* Legend */
    OSK_DrawString(OSK_PANEL_X + OSK_PAD, OSK_PANEL_Y + OSK_PANEL_H + 3.0f,
                   6.0f, 10.0f,
                   "A type   B bksp   space/shift   + accept   - cancel   point or tap");

    /* Wii Remote pointer. Touch has no hover state so it never sets this. */
    if (osk_cursor_valid) {
        SCR_FillRect(osk_cursor_x - 5.0f, osk_cursor_y - 1.0f, 10.0f, 2.0f, osk_color_sel);
        SCR_FillRect(osk_cursor_x - 1.0f, osk_cursor_y - 5.0f, 2.0f, 10.0f, osk_color_sel);
    }

    re.SetColor(NULL);
}
