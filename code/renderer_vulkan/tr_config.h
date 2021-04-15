#ifndef GL_CONFIGURE_H_
#define GL_CONFIGURE_H_

#include "tr_local.h"

void R_SetWinMode(int mode, unsigned int w, unsigned int h, unsigned int hz);
void R_glConfigInit(void);
void R_glConfigClear(void);
void R_GetWinResolution(int *w, int *h);
void R_GetWinResolutionF(float *w, float *h);

void R_GetGlConfig(glconfig_t *const pOut);
void R_InitDisplayResolution(void);

void vulkanInfo_f(void);
#endif
