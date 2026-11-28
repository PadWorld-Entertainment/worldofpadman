/*
 * Quake3e for the PlayStation 5 - libSceMouse's names, for the link.
 *
 * The payload SDK has no stub for the console's mouse library: this names the
 * imports ps5/src/platform.c makes. Host-link only: never packaged or executed.
 *
 * SPDX-License-Identifier: MIT
 */
#include <stdint.h>

int sceMouseInit(void)
{
   return -1;
}

int sceMouseOpen(int32_t user_id, int32_t type, int32_t index, const void *params)
{
   (void)user_id;
   (void)type;
   (void)index;
   (void)params;
   return -1;
}

int sceMouseRead(int32_t handle, void *samples, int32_t capacity)
{
   (void)handle;
   (void)samples;
   (void)capacity;
   return -1;
}

int sceMouseClose(int32_t handle)
{
   (void)handle;
   return -1;
}
