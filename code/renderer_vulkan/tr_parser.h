#ifndef R_PARSER_H_
#define R_PARSER_H_

#include "../qcommon/q_shared.h"

const char *R_ParseExt(const char **data_p, qboolean allowLineBreaks);
int R_Compress(char *data_p);
int R_GetCurrentParseLine(void);
void R_BeginParseSession(const char *name);

#endif
