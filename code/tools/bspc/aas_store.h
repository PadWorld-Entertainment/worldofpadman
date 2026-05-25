/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#define AAS_MAX_BBOXES 5
#define AAS_MAX_VERTEXES 512000
#define AAS_MAX_PLANES 65536
#define AAS_MAX_EDGES 512000
#define AAS_MAX_EDGEINDEXSIZE 512000
#define AAS_MAX_FACES 512000
#define AAS_MAX_FACEINDEXSIZE 512000
#define AAS_MAX_AREAS 65536
#define AAS_MAX_AREASETTINGS 65536
#define AAS_MAX_REACHABILITYSIZE 65536
#define AAS_MAX_NODES 256000
#define AAS_MAX_PORTALS 65536
#define AAS_MAX_PORTALINDEXSIZE 65536
#define AAS_MAX_CLUSTERS 65536

#define BSPCINCLUDE
#include "botlib/be_aas.h"
#include "botlib/be_aas_def.h"

extern aas_t aasworld;

// stores the AAS file from the temporary AAS
void AAS_StoreFile(void);
// returns a number of the given plane
qboolean AAS_FindPlane(vec3_t normal, float dist, int *planenum);
// allocates the maximum AAS memory for storage
void AAS_AllocMaxAAS(void);
// frees the maximum AAS memory for storage
void AAS_FreeMaxAAS(void);
