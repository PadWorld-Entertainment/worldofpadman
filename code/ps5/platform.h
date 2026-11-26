/*
 * PS5 Vulkan Template - the console: klog, the splash, the pad, sound, time, the exit.
 *
 * Copyright (C) 2026 Mihawk
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Standard error into klog (each line prefixed with the title's name), and
 * the shell's splash dismissed. Call first. */
void platform_init(const char *title_name);

/* One line to klog, flushed. */
void say(const char *format, ...) __attribute__((format(printf, 1, 2)));

double now_seconds(void);

/* The pad, in the console's button numbering. */
enum {
   PAD_L3 = 0x000002,
   PAD_R3 = 0x000004,
   PAD_OPTIONS = 0x000008,
   PAD_UP = 0x000010,
   PAD_RIGHT = 0x000020,
   PAD_DOWN = 0x000040,
   PAD_LEFT = 0x000080,
   PAD_L1 = 0x000400,
   PAD_R1 = 0x000800,
   PAD_TRIANGLE = 0x001000,
   PAD_CIRCLE = 0x002000,
   PAD_CROSS = 0x004000,
   PAD_SQUARE = 0x008000,
   PAD_TOUCH_PAD = 0x100000,
};

struct pad {
   uint32_t held;    /* buttons down now */
   uint32_t pressed; /* buttons that went down since the last poll */
   float left_x, left_y, right_x, right_y; /* -1..1, a dead zone removed; y is down-positive */
   float l2, r2;                           /* 0..1 */
};

/* Up to four players, one per signed-in user: the console pairs each controller
 * with a user. Player 0 is the user who started the title and stays theirs;
 * pad_poll reads the signed-in users again about once a second, a new user's
 * controller takes the first free player, and a player is let go when their user
 * signs out. pad_poll, pad_readings, pad_vibrate and pad_light_bar are player
 * 0's; the pad_player calls reach the others. */
#define PAD_PLAYERS 4

/* Open player 0's pad (the first user's), and every other signed-in user's;
 * false (and no input for player 0) when there is none. */
bool pad_open(void);
/* Read every player's controller; *pad is player 0's. Call once a frame. */
void pad_poll(struct pad *pad);
/* The players whose controller reported itself connected, a bit each (bit 0 is
 * player 0), as of the last pad_poll. */
uint32_t pad_players(void);
/* A player's pad as the last pad_poll read it, `pressed` against the held
 * buttons *pad had; false, and a pad at rest, when that player has none. */
bool pad_player(int player, struct pad *pad);
/* The signed-in user whose controller is a player's; -1 when that player has none. */
int32_t pad_player_user(int player);

/* Set in a reading's buttons while the shell has the pad (the home screen, a
 * system dialog): that reading is not input for the title. */
#define PAD_INTERCEPTED 0x80000000u

/* One reading as the console took it, in its numbering. */
struct pad_reading {
   uint32_t buttons;
   uint8_t left_x, left_y, right_x, right_y; /* 0..255, 128 at rest; y is down-positive */
   uint8_t l2, r2;                           /* 0..255 */
   bool connected;
   uint64_t timestamp_us;
   float accel[3]; /* in g: +x right, +y out of the face, +z toward the player */
   float gyro[3];  /* rad/s about those axes, counter-clockwise positive */
   uint8_t touches; /* fingers on the touch pad, 0 to 2 (a touch, clicked or not) */
};

/* Every reading the last pad_poll took, oldest first (the console keeps up to
 * 64 between polls), for input models that must see a tap shorter than a
 * frame. Returns their number; *readings stays valid until the next poll. */
int pad_readings(const struct pad_reading **readings);
int pad_player_readings(int player, const struct pad_reading **readings);

/* Rumble: the large and the small motor, 0..1 each; 0, 0 stops it. */
void pad_vibrate(float large, float small);
void pad_player_vibrate(int player, float large, float small);
/* The light bar's colour. */
void pad_light_bar(uint8_t r, uint8_t g, uint8_t b);
void pad_player_light_bar(int player, uint8_t r, uint8_t g, uint8_t b);
/* The light bar back to the console's own colour for the player's user. */
void pad_player_reset_light_bar(int player);

/* A USB or Bluetooth mouse (libSceMouse), the first user's. */
enum {
   MOUSE_LEFT = 0x01,
   MOUSE_RIGHT = 0x02,
   MOUSE_MIDDLE = 0x04,
   MOUSE_BUTTON4 = 0x08,
   MOUSE_BUTTON5 = 0x10,
};

struct mouse {
   int dx, dy;       /* counts moved since the last poll; y is down-positive */
   int wheel;        /* notches turned since the last poll */
   uint32_t buttons; /* buttons down now */
   bool connected;
};

/* Load the console's mouse library and open the first user's mouse; false when
 * the console gives the title none. A mouse may be plugged in later. */
bool mouse_open(void);
/* What the mouse did since the last poll; false, and a mouse at rest, when
 * there is none or the shell has it. Call once a frame. */
bool mouse_poll(struct mouse *mouse);

/* A USB or Bluetooth keyboard (libSceKeyboard), the first user's. A key is its
 * USB HID usage (0x04 is A, 0x28 Enter, 0x52 the up arrow); the modifier keys
 * are a bit each, in the HID's own order. */
enum {
   KEYBOARD_LEFT_CTRL = 0x01,
   KEYBOARD_LEFT_SHIFT = 0x02,
   KEYBOARD_LEFT_ALT = 0x04,
   KEYBOARD_LEFT_GUI = 0x08,
   KEYBOARD_RIGHT_CTRL = 0x10,
   KEYBOARD_RIGHT_SHIFT = 0x20,
   KEYBOARD_RIGHT_ALT = 0x40,
   KEYBOARD_RIGHT_GUI = 0x80,
};

struct keyboard_event {
   uint8_t usage;
   bool down;
};

struct keyboard {
   bool connected;
   uint8_t modifiers; /* the modifier keys down now */
   int event_count;   /* the keys that went down or up since the last poll, in order */
   struct keyboard_event events[64];
};

/* Load the console's keyboard library and open the first user's keyboard; false
 * when the console gives the title none. A keyboard may be plugged in later. */
bool keyboard_open(void);
/* What the keyboard did since the last poll; false, and a keyboard at rest,
 * when there is none. Call once a frame. */
bool keyboard_poll(struct keyboard *keyboard);

/* Sound: one 48 kHz output of interleaved stereo 16-bit samples. A thread of
 * its own calls fill for 256 frames at a time and blocks while the console
 * plays them, so fill paces itself and must never wait on the render loop.
 * false when there is no output (the host reference build has none). */
typedef void (*audio_fill_fn)(int16_t *frames, int count, void *user);
bool audio_start(audio_fill_fn fill, void *user);
/* Stops the thread and closes the output; call it before what fill uses goes. */
void audio_stop(void);

#ifdef __cplusplus
}
#endif
