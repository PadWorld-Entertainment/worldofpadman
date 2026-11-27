/*
 * wiiu_snd.h -- Wii U audio subsystem declarations
 */

#ifndef WIIU_SND_H
#define WIIU_SND_H

void WiiU_Snd_Init(void);
void WiiU_Snd_Shutdown(void);

/* ProcUI foreground/background transitions */
void WiiU_Snd_ReleaseForeground(void);
void WiiU_Snd_AcquireForeground(void);
void WiiU_Snd_SetPaused(qboolean paused);

#endif /* WIIU_SND_H */
