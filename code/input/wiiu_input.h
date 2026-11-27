/*
 * wiiu_input.h -- Wii U input subsystem declarations
 */

#ifndef WIIU_INPUT_H
#define WIIU_INPUT_H

void WiiU_Input_Init(void);
void WiiU_Input_Shutdown(void);
void WiiU_Input_Frame(void);
int  WiiU_Input_QuitPressed(void);

/* Stop rumble immediately (ProcUI RELEASE callback) so backgrounding never leaves it buzzing. */
void WiiU_Input_ReleaseForeground(void);

#endif /* WIIU_INPUT_H */
