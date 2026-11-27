/*
 * code/wiiu/wiiu_account.h -- Mii nickname lookup (nn::act), C-linkage wrapper
 * so sys_main_wiiu.c (a plain .c file) can call into the C++-only nn::act API.
 */
#ifndef WIIU_ACCOUNT_H
#define WIIU_ACCOUNT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Writes default account's Mii nickname (ASCII, non-ASCII replaced with '_') into out. */
int WiiU_GetAccountName(char *out, size_t outSize);

/* Call once early in boot (before Com_Init) -- the nn::act hit must happen at this
 * proven-safe timing. WiiU_DefaultPlayerName() is then a pure read. */
void WiiU_InitDefaultPlayerName(void);

/* Cached Mii nickname, or "UnnamedPlayer" if unavailable. Never NULL. */
const char *WiiU_DefaultPlayerName(void);

#ifdef __cplusplus
}
#endif

#endif /* WIIU_ACCOUNT_H */
