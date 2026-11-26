/*
 * PS5 Vulkan Template - the console: klog, the splash, the pad, sound, time, the exit.
 *
 * Copyright (C) 2026 Mihawk
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 *
 * Every console call here is one PS5_vkQuake, PS5_RetroArch or PS5_Vulkan's
 * smoke test runs with; no SDK header declares them.
 */
#include "platform.h"

#include <ps5platform/klog.h>

#include <pthread.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int sceSystemServiceHideSplashScreen(void);
int sceSystemServiceLoadExec(const char *path, const char *const *argv);
int sceUserServiceInitialize(const void *params);
int sceUserServiceGetInitialUser(int32_t *user_id);
/* The signed-in users, at most four; unused entries are -1. */
struct user_list {
   int32_t user_id[4];
};
int sceUserServiceGetLoginUserIdList(struct user_list *list);
int scePadInit(void);
int scePadOpen(int32_t user_id, int32_t port_type, int32_t index, const void *params);
int scePadGetHandle(int32_t user_id, int32_t port_type, int32_t index);
int scePadRead(int32_t handle, void *samples, int32_t capacity);
int scePadClose(int32_t handle);
int scePadSetVibrationMode(int32_t handle, int32_t mode);
int scePadSetVibration(int32_t handle, const void *vibration);
int scePadSetLightBar(int32_t handle, const void *color);
int scePadResetLightBar(int32_t handle);
int scePadSetMotionSensorState(int32_t handle, bool enable);
int scePadSetAngularVelocityDeadbandState(int32_t handle, bool enable);
int sceKernelLoadStartModule(const char *name, size_t argc, const void *argv, uint32_t flags,
                             const void *option, int *result);
const char *sceKernelGetFsSandboxRandomWord(void);
int sceMouseInit(void);
int sceMouseOpen(int32_t user_id, int32_t type, int32_t index, const void *params);
int sceMouseRead(int32_t handle, void *samples, int32_t capacity);
int sceKeyboardInit(void);
int sceKeyboardOpen(int32_t user_id, int32_t type, int32_t index, const void *params);
int sceKeyboardRead(int32_t handle, void *samples, int32_t capacity);
int sceAudioOutInit(void);
int sceAudioOutOpen(int32_t user, int32_t type, int32_t index, uint32_t grain, uint32_t rate,
                    uint32_t format);
int sceAudioOutOutput(int32_t port, const void *samples);
int sceAudioOutClose(int32_t port);
int sceKernelUsleep(uint32_t microseconds);

void
platform_init(const char *title_name)
{
   static char prefix[64];
   snprintf(prefix, sizeof(prefix), "[%s] ", title_name);
   const int captured = ps5_klog_capture_stderr(prefix);
   say("starts (standard error to klog: %s)", captured == 0 ? "yes" : "no");
   /* The shell's splash covers the title's frames until it is dismissed. */
   say("splash dismissed: %d", sceSystemServiceHideSplashScreen());
}

void
say(const char *format, ...)
{
   va_list args;
   va_start(args, format);
   vfprintf(stderr, format, args);
   va_end(args);
   fputc('\n', stderr);
   fflush(stderr);
}

double
now_seconds(void)
{
   struct timespec t;
   clock_gettime(CLOCK_MONOTONIC, &t);
   return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

/* The title ends by asking the shell to close it. exit() and returning from
 * _start both kill it, and the console then reports a crash over a run that
 * worked. The CRT calls this after main returns; the shell closes the title
 * asynchronously, so it must wait and never return. */
void
catchReturnFromMain(int status)
{
   say("exit status %d: asking the shell to close the title", status);
   fflush(NULL);
   (void)sceSystemServiceLoadExec("exit", NULL);
   for (;;)
      sceKernelUsleep(100000);
}

/* ----------------------------------------------------------------- the pad */

/* One sample as the console writes it: 120 bytes, the layout PS5_vkQuake and
 * ProsperoLight read. */
struct pad_sample {
   uint32_t buttons;
   uint8_t left_x, left_y, right_x, right_y, l2, r2;
   uint8_t reserved[2];
   float orientation[4];
   float acceleration[3];     /* in g: +x right, +y out of the face, +z toward the player */
   float angular_velocity[3]; /* rad/s about those axes, counter-clockwise positive */
   uint8_t touch[24];         /* the number of fingers first, then where each one is */
   int32_t connected;
   uint64_t timestamp_us;
   uint8_t extension[16];
   uint8_t connected_count;
   uint8_t remaining[15];
};
_Static_assert(sizeof(struct pad_sample) == 120, "the console's pad samples are 120 bytes");
_Static_assert(offsetof(struct pad_sample, acceleration) == 0x1c, "acceleration sits at 0x1c");
_Static_assert(offsetof(struct pad_sample, angular_velocity) == 0x28, "angular velocity sits at 0x28");
_Static_assert(offsetof(struct pad_sample, touch) == 0x34, "the touch pad sits at 0x34");
_Static_assert(offsetof(struct pad_sample, connected) == 0x4c, "connection state sits at 0x4c");
_Static_assert(offsetof(struct pad_sample, timestamp_us) == 0x50, "the timestamp sits at 0x50");

/* The two-motor rumble, as PS5_vkQuake and ProsperoLight drive it (the other
 * mode is the DualSense's haptics). */
#define PAD_VIBRATION_COMPATIBLE 2

/* A player: a signed-in user's controller, PS5_ProsperoEden's way. The console
 * pairs each controller with a user, and scePadOpen opens a user's. Player 0 is
 * the user who started the title and stays theirs; about once a second
 * pad_poll reads the signed-in users again, a new user's controller takes the
 * first free player, and a player is let go when their user signs out. */
struct pad_player_state {
   int32_t user;
   int32_t handle;
   bool connected;
   struct pad_sample last;
   struct pad_reading taken[64];
   int taken_count;
};

static struct pad_player_state pad_states[PAD_PLAYERS];
static bool pad_service;
static double pad_next_scan;
/* A user whose controller would not open, said once rather than every scan. */
static int32_t pad_unopened = -1;
#define PAD_SCAN_SECONDS 1.0

/* A player's state while they have a controller, else NULL (and always NULL
 * before pad_open has set the players up). */
static struct pad_player_state *
pad_state(int player)
{
   if (!pad_service || player < 0 || player >= PAD_PLAYERS || pad_states[player].handle < 0)
      return NULL;
   return &pad_states[player];
}

static void
pad_rest(struct pad_player_state *state)
{
   memset(state, 0, sizeof(*state));
   state->user = state->handle = -1;
   state->last.left_x = state->last.left_y = state->last.right_x = state->last.right_y = 128;
}

static bool
pad_player_open(int player, int32_t user, int attempts)
{
   struct pad_player_state *state = &pad_states[player];
   pad_rest(state);
   /* A title can start before the pad service has published the device. */
   int32_t handle = -1;
   for (int attempt = 0; attempt < attempts && handle < 0; attempt++) {
      handle = scePadOpen(user, 0, 0, NULL);
      if (handle < 0 && attempt + 1 < attempts)
         sceKernelUsleep(100000);
   }
   /* A handle this process already holds for the user is reused. */
   if (handle < 0)
      handle = scePadGetHandle(user, 0, 0);
   if (handle < 0) {
      if (user != pad_unopened)
         say("pad: scePadOpen failed for user %d (player %d)", (int)user, player);
      pad_unopened = user;
      return false;
   }
   state->user = user;
   state->handle = handle;
   say("pad: player %d opened for user %d, rumble mode %d", player, (int)user,
       scePadSetVibrationMode(handle, PAD_VIBRATION_COMPATIBLE));
   /* The motion sensors, with the console's own dead band off: small turns of
    * the pad must move a gyro aim, and the reader filters rest noise. */
   say("pad: player %d motion sensors %d, angular velocity dead band off %d", player,
       scePadSetMotionSensorState(handle, true), scePadSetAngularVelocityDeadbandState(handle, false));
   return true;
}

static void
pad_player_close(int player, const char *why)
{
   struct pad_player_state *state = &pad_states[player];
   if (state->handle < 0)
      return;
   say("pad: player %d let go (user %d %s), close %d", player, (int)state->user, why,
       scePadClose(state->handle));
   pad_rest(state);
}

/* The signed-in users again: player 0 stays; a player whose user signed out is
 * let go; a new user's controller takes the first free player. */
static void
pad_scan(void)
{
   struct user_list list = {{-1, -1, -1, -1}};
   if (sceUserServiceGetLoginUserIdList(&list) < 0)
      return;
   for (int player = 1; player < PAD_PLAYERS; player++) {
      if (pad_states[player].handle < 0)
         continue;
      bool signed_in = false;
      for (int i = 0; i < 4; i++)
         signed_in |= list.user_id[i] >= 0 && list.user_id[i] == pad_states[player].user;
      if (!signed_in)
         pad_player_close(player, "signed out");
   }
   for (int i = 0; i < 4; i++) {
      const int32_t user = list.user_id[i];
      bool known = user < 0;
      for (int player = 0; player < PAD_PLAYERS && !known; player++)
         known = pad_states[player].handle >= 0 && pad_states[player].user == user;
      for (int player = 0; player < PAD_PLAYERS && !known; player++) {
         if (pad_states[player].handle < 0) {
            (void)pad_player_open(player, user, 1);
            break;
         }
      }
   }
}

bool
pad_open(void)
{
   for (int player = 0; player < PAD_PLAYERS; player++)
      pad_rest(&pad_states[player]);
   (void)sceUserServiceInitialize(NULL);
   int32_t user = -1;
   if (sceUserServiceGetInitialUser(&user) < 0 || scePadInit() < 0) {
      say("pad: no user or no pad service");
      return false;
   }
   pad_service = true;
   const bool opened = pad_player_open(0, user, 10);
   pad_scan();
   pad_next_scan = now_seconds() + PAD_SCAN_SECONDS;
   return opened;
}

static float
stick(uint8_t value)
{
   const float v = ((float)value - 128.0f) / 127.0f;
   const float dead = 0.12f;
   if (v > -dead && v < dead)
      return 0.0f;
   return v > 0 ? (v - dead) / (1.0f - dead) : (v + dead) / (1.0f - dead);
}

static void
pad_fill(const struct pad_player_state *state, struct pad *pad)
{
   const uint32_t before = pad->held;
   pad->held = state->handle >= 0 ? state->last.buttons : 0;
   pad->pressed = pad->held & ~before;
   pad->left_x = stick(state->last.left_x);
   pad->left_y = stick(state->last.left_y);
   pad->right_x = stick(state->last.right_x);
   pad->right_y = stick(state->last.right_y);
   pad->l2 = state->last.l2 / 255.0f;
   pad->r2 = state->last.r2 / 255.0f;
}

void
pad_poll(struct pad *pad)
{
   if (pad_service && now_seconds() >= pad_next_scan) {
      pad_scan();
      pad_next_scan = now_seconds() + PAD_SCAN_SECONDS;
   }
   static struct pad_sample samples[64];
   for (int player = 0; player < PAD_PLAYERS; player++) {
      struct pad_player_state *state = pad_state(player);
      if (state == NULL)
         continue;
      state->taken_count = 0;
      const int count = scePadRead(state->handle, samples, 64);
      if (count < 0)
         state->connected = false;
      for (int i = 0; i < count && i < 64; i++) {
         const struct pad_sample *s = &samples[i];
         state->taken[state->taken_count++] = (struct pad_reading){
            s->buttons, s->left_x, s->left_y, s->right_x, s->right_y, s->l2, s->r2,
            s->connected != 0, s->timestamp_us,
            {s->acceleration[0], s->acceleration[1], s->acceleration[2]},
            {s->angular_velocity[0], s->angular_velocity[1], s->angular_velocity[2]},
            s->touch[0] < 2 ? s->touch[0] : 2};
         state->connected = s->connected != 0;
         if (s->connected && !(s->buttons & PAD_INTERCEPTED))
            state->last = *s;
      }
   }
   (void)pad_player(0, pad);
}

uint32_t
pad_players(void)
{
   uint32_t players = 0;
   for (int player = 0; player < PAD_PLAYERS; player++) {
      const struct pad_player_state *state = pad_state(player);
      if (state != NULL && state->connected)
         players |= 1u << player;
   }
   return players;
}

bool
pad_player(int player, struct pad *pad)
{
   const struct pad_player_state *state = pad_state(player);
   if (state == NULL) {
      struct pad_player_state rest;
      pad_rest(&rest);
      pad_fill(&rest, pad);
      return false;
   }
   pad_fill(state, pad);
   return true;
}

int32_t
pad_player_user(int player)
{
   const struct pad_player_state *state = pad_state(player);
   return state != NULL ? state->user : -1;
}

int
pad_player_readings(int player, const struct pad_reading **readings)
{
   const struct pad_player_state *state = pad_state(player);
   *readings = state != NULL ? state->taken : NULL;
   return state != NULL ? state->taken_count : 0;
}

int
pad_readings(const struct pad_reading **readings)
{
   return pad_player_readings(0, readings);
}

void
pad_player_vibrate(int player, float large, float small)
{
   const struct pad_player_state *state = pad_state(player);
   if (state == NULL)
      return;
   const float l = large < 0.0f ? 0.0f : large > 1.0f ? 1.0f : large;
   const float s = small < 0.0f ? 0.0f : small > 1.0f ? 1.0f : small;
   const uint8_t motors[2] = {(uint8_t)(l * 255.0f), (uint8_t)(s * 255.0f)};
   (void)scePadSetVibration(state->handle, motors);
}

void
pad_vibrate(float large, float small)
{
   pad_player_vibrate(0, large, small);
}

void
pad_player_light_bar(int player, uint8_t r, uint8_t g, uint8_t b)
{
   const struct pad_player_state *state = pad_state(player);
   if (state == NULL)
      return;
   const uint8_t color[4] = {r, g, b, 0};
   (void)scePadSetLightBar(state->handle, color);
}

void
pad_light_bar(uint8_t r, uint8_t g, uint8_t b)
{
   pad_player_light_bar(0, r, g, b);
}

void
pad_player_reset_light_bar(int player)
{
   const struct pad_player_state *state = pad_state(player);
   if (state == NULL)
      return;
   (void)scePadResetLightBar(state->handle);
}

/* --------------------------------------------------------------- the mouse */

#define MOUSE_INTERCEPTED 0x80000000u
#define MOUSE_READINGS 64

/* One reading as the console writes it: 40 bytes, the PlayStation 4's layout
 * (2026-10-11: movement and the first two buttons read back from klog, the
 * wheel seen changing weapons; the other buttons are as the PlayStation 4 has
 * them). */
struct mouse_sample {
   uint64_t timestamp_us;
   uint8_t connected;
   uint8_t padding[3];
   uint32_t buttons;
   int32_t x, y, wheel, tilt;
   uint8_t reserved[8];
};
_Static_assert(sizeof(struct mouse_sample) == 40, "the console's mouse readings are 40 bytes");

/* Load one of the console's libraries from the sandbox's own name for /system. */
static int
system_library(const char *name)
{
   const char *word = sceKernelGetFsSandboxRandomWord();
   char path[128];
   snprintf(path, sizeof(path), "/%s/common/lib/%s", word != NULL ? word : "system", name);
   const int module = sceKernelLoadStartModule(path, 0, NULL, 0, NULL, NULL);
   say("%s gave 0x%08x", path, (unsigned)module);
   return module;
}

static int32_t mouse_handle = -1;

bool
mouse_open(void)
{
   if (mouse_handle >= 0)
      return true;
   /* The title links against a stub of the library (ps5/stubs); the console
    * binds a name at its first call, from the modules loaded by then (an
    * import's address reads 0 before that, the on-screen keyboard's too, so it
    * tells nothing). The library's file is loaded from the sandbox's own name
    * for /system: sysmodule 0x00a8 loaded, and sceMouseInit then jumped to 0
    * (2026-10-11). */
   if (system_library("libSceMouse.sprx") < 0)
      return false;

   (void)sceUserServiceInitialize(NULL);
   int32_t user = -1;
   const int got_user = sceUserServiceGetInitialUser(&user);
   const int started = sceMouseInit();
   mouse_handle = sceMouseOpen(user, 0, 0, NULL);
   say("mouse: user %d (0x%08x), sceMouseInit gave 0x%08x, sceMouseOpen gave 0x%08x", (int)user,
       (unsigned)got_user, (unsigned)started, (unsigned)mouse_handle);
   return mouse_handle >= 0;
}

bool
mouse_poll(struct mouse *mouse)
{
   static struct mouse_sample samples[MOUSE_READINGS];
   static int last_count;
   static uint32_t held;
   static bool connected;
   bool taken = false;

   *mouse = (struct mouse){0};
   if (mouse_handle < 0)
      return false;
   const int count = sceMouseRead(mouse_handle, samples, MOUSE_READINGS);
   if ((count < 0 || last_count < 0) && count != last_count)
      say("mouse: sceMouseRead gave 0x%08x", (unsigned)count);
   last_count = count;
   for (int i = 0; i < count && i < MOUSE_READINGS; i++) {
      const struct mouse_sample s = samples[i];
      if ((s.connected != 0) != connected) {
         connected = s.connected != 0;
         say("mouse: %s", connected ? "connected" : "disconnected");
      }
      if (!connected || (s.buttons & MOUSE_INTERCEPTED))
         continue;
      mouse->dx += s.x;
      mouse->dy += s.y;
      mouse->wheel += s.wheel;
      held = s.buttons;
      taken = true;
   }
   if (!connected)
      held = 0;
   mouse->buttons = held;
   mouse->connected = connected;
   return taken;
}

/* ------------------------------------------------------------ the keyboard */

#define KEYBOARD_KEYS 16
#define KEYBOARD_READINGS 16

/* One reading as the console writes it: 64 bytes, read back from klog
 * (2026-10-11). The console takes a reading when a key changes, and each has
 * every key then down. The bytes named padding it leaves as they were. */
struct keyboard_sample {
   uint64_t timestamp_us;
   uint8_t unknown; /* 0 in every reading seen */
   uint8_t padding[7];
   uint8_t connected;
   uint8_t padding2[3];
   uint32_t count; /* the keys down, and 1 with none */
   uint32_t unknown2;
   uint32_t modifiers;
   uint16_t usages[KEYBOARD_KEYS];
};
_Static_assert(sizeof(struct keyboard_sample) == 64, "the console's keyboard readings are 64 bytes");

static int32_t keyboard_handle = -1;

bool
keyboard_open(void)
{
   if (keyboard_handle >= 0)
      return true;
   if (system_library("libSceKeyboard.sprx") < 0)
      return false;
   (void)sceUserServiceInitialize(NULL);
   int32_t user = -1;
   const int got_user = sceUserServiceGetInitialUser(&user);
   const int started = sceKeyboardInit();
   keyboard_handle = sceKeyboardOpen(user, 0, 0, NULL);
   say("keyboard: user %d (0x%08x), sceKeyboardInit gave 0x%08x, sceKeyboardOpen gave 0x%08x",
       (int)user, (unsigned)got_user, (unsigned)started, (unsigned)keyboard_handle);
   return keyboard_handle >= 0;
}

static bool
keyboard_has(const uint16_t *usages, uint16_t usage)
{
   for (int i = 0; i < KEYBOARD_KEYS; i++)
      if (usages[i] == usage)
         return true;
   return false;
}

static void
keyboard_changes(struct keyboard *keyboard, const uint16_t *from, const uint16_t *to, bool down)
{
   for (int i = 0; i < KEYBOARD_KEYS; i++) {
      /* 1 to 3 are the HID's errors (too many keys down), not keys */
      if (to[i] < 4 || to[i] > 0xff || keyboard_has(from, to[i]))
         continue;
      if (keyboard->event_count < (int)(sizeof(keyboard->events) / sizeof(keyboard->events[0])))
         keyboard->events[keyboard->event_count++] = (struct keyboard_event){(uint8_t)to[i], down};
   }
}

bool
keyboard_poll(struct keyboard *keyboard)
{
   static const uint16_t none[KEYBOARD_KEYS];
   static struct keyboard_sample samples[KEYBOARD_READINGS];
   static uint16_t held[KEYBOARD_KEYS];
   static uint8_t modifiers;
   static bool connected;

   *keyboard = (struct keyboard){0};
   if (keyboard_handle < 0)
      return false;
   const int count = sceKeyboardRead(keyboard_handle, samples, KEYBOARD_READINGS);
   for (int i = 0; i < count && i < KEYBOARD_READINGS; i++) {
      const struct keyboard_sample *s = &samples[i];
      if ((s->connected != 0) != connected) {
         connected = s->connected != 0;
         say("keyboard: %s", connected ? "connected" : "disconnected");
      }
      const uint16_t *now = connected ? s->usages : none;
      /* a reading with an error in it says nothing of the keys: keep the last */
      if (now[0] >= 1 && now[0] <= 3)
         continue;
      keyboard_changes(keyboard, now, held, false);
      keyboard_changes(keyboard, held, now, true);
      memcpy(held, now, sizeof(held));
      modifiers = connected ? (uint8_t)s->modifiers : 0;
   }
   keyboard->connected = connected;
   keyboard->modifiers = modifiers;
   return connected;
}

/* --------------------------------------------------------------- the sound */

#define AUDIO_GRAIN 256
#define AUDIO_ALREADY_INITIALISED ((int)0x8026000e)

static int32_t audio_port = -1;
static pthread_t audio_thread;
static atomic_bool audio_running;
static audio_fill_fn audio_fill;
static void *audio_user;

static void *
audio_main(void *unused)
{
   (void)unused;
   static int16_t grain[AUDIO_GRAIN * 2] __attribute__((aligned(64)));
   unsigned errors = 0;
   while (atomic_load(&audio_running)) {
      audio_fill(grain, AUDIO_GRAIN, audio_user);
      /* Blocks for one grain: this is what paces the thread. An output that
       * stops taking grains is said once, not left to stall the program. */
      if (sceAudioOutOutput(audio_port, grain) < 0) {
         if (errors++ == 0)
            say("audio: sceAudioOutOutput failed");
         sceKernelUsleep(5000);
      }
   }
   (void)sceAudioOutOutput(audio_port, NULL); /* drain the grain still queued */
   return NULL;
}

bool
audio_start(audio_fill_fn fill, void *user)
{
   if (audio_port >= 0 || fill == NULL)
      return false;
   const int init = sceAudioOutInit();
   if (init < 0 && init != AUDIO_ALREADY_INITIALISED) {
      say("audio: sceAudioOutInit 0x%08x", (unsigned)init);
      return false;
   }
   /* The system user's main port: 256 frames, 48 kHz, 16-bit stereo. */
   audio_port = sceAudioOutOpen(0xff, 0, 0, AUDIO_GRAIN, 48000, 1);
   if (audio_port < 0) {
      say("audio: sceAudioOutOpen 0x%08x", (unsigned)audio_port);
      audio_port = -1;
      return false;
   }
   audio_fill = fill;
   audio_user = user;
   atomic_store(&audio_running, true);
   if (pthread_create(&audio_thread, NULL, audio_main, NULL) != 0) {
      say("audio: no thread");
      atomic_store(&audio_running, false);
      (void)sceAudioOutClose(audio_port);
      audio_port = -1;
      return false;
   }
   say("audio: 48 kHz stereo on port %d", (int)audio_port);
   return true;
}

void
audio_stop(void)
{
   if (audio_port < 0)
      return;
   atomic_store(&audio_running, false);
   pthread_join(audio_thread, NULL);
   (void)sceAudioOutClose(audio_port);
   audio_port = -1;
}
