/*
===========================================================================
Copyright (C) 2007-2026 PadWorld Entertainment
===========================================================================
*/

#ifndef RCH_LOAD_H
#define RCH_LOAD_H

#include "qcommon/q_shared.h"

// Load a .rch reachability XML file corresponding to the given .aas filename.
// Returns qtrue if the file was found and parsed successfully.
qboolean RCH_LoadReachFile(const char *aasfilename);

#endif
