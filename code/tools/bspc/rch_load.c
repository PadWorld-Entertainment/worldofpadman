/*
===========================================================================
Copyright (C) 2007-2026 PadWorld Entertainment

This file is part of World of PADMAN source code.

World of PADMAN source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

World of PADMAN source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with World of PADMAN source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// rch_load.c - Loads .rch (reachability) XML files to provide hand-crafted
// reachability links (e.g. spray teleporters) that the automatic BSP-to-AAS
// conversion cannot detect. Uses yxml parser (https://dev.yorhel.nl/yxml).
//
// The .rch XML format uses nested elements with text content:
//   <reachability>
//     <areanum>5</areanum>
//     <facenum>0</facenum>
//     <edgenum>0</edgenum>
//     <startX>100.0</startX>
//     <startY>200.0</startY>
//     <startZ>300.0</startZ>
//     <endX>400.0</endX>
//     <endY>500.0</endY>
//     <endZ>600.0</endZ>
//     <traveltype>18</traveltype>
//     <traveltime>100</traveltime>
//   </reachability>

#include "qcommon/q_shared.h"
#include "l_log.h"
#include "botlib/l_memory.h"
#include "botlib/aasfile.h"
#include "botlib/botlib.h"
#include "aas_store.h"
#include "rch_load.h"
#include "yxml.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define RCH_READ_BUF_SIZE 8192
#define RCH_YXML_BUF_SIZE 4096
#define RCH_CONTENT_BUF_SIZE 64

// traveltype for jumppad (TRAVEL_JUMPPAD in be_aas_reach.c)
#define TRAVEL_JUMPPAD 0x12

extern int AAS_PointAreaNum(vec3_t point);
extern int AAS_NextBSPEntity(int ent);
extern int AAS_ValueForBSPEpairKey(int ent, char *key, char *value, int size);

// Temporary linked-list node for parsed reachabilities
typedef struct rch_reach_s {
	aas_reachability_t reach;
	struct rch_reach_s *next;
} rch_reach_t;

// Parser state
typedef struct {
	// Content accumulation for current sub-element
	char content[RCH_CONTENT_BUF_SIZE];
	char *contentptr;
	// Current element name (sub-element of <reachability>)
	char curelem[RCH_CONTENT_BUF_SIZE];
	// Whether we're inside a <reachability> element
	qboolean in_reach;
	// Current reachability being built
	rch_reach_t *current;
	// Linked list of completed reachabilities
	rch_reach_t *head;
	int count;
	// Nesting depth
	int depth;
} rch_state_t;

static void RCH_SetFieldValue(rch_reach_t *r, const char *elem, const char *val) {
	if (!strcmp(elem, "areanum"))
		r->reach.areanum = atoi(val);
	else if (!strcmp(elem, "edgenum"))
		r->reach.edgenum = atoi(val);
	else if (!strcmp(elem, "facenum"))
		r->reach.facenum = atoi(val);
	else if (!strcmp(elem, "startX"))
		r->reach.start[0] = (float)atof(val);
	else if (!strcmp(elem, "startY"))
		r->reach.start[1] = (float)atof(val);
	else if (!strcmp(elem, "startZ"))
		r->reach.start[2] = (float)atof(val);
	else if (!strcmp(elem, "endX"))
		r->reach.end[0] = (float)atof(val);
	else if (!strcmp(elem, "endY"))
		r->reach.end[1] = (float)atof(val);
	else if (!strcmp(elem, "endZ"))
		r->reach.end[2] = (float)atof(val);
	else if (!strcmp(elem, "traveltime"))
		r->reach.traveltime = (unsigned short)atoi(val);
	else if (!strcmp(elem, "traveltype"))
		r->reach.traveltype = atoi(val);
}

static void RCH_FinalizeReach(rch_state_t *state) {
	rch_reach_t *r = state->current;
	if (!r)
		return;

	// Resolve areanum from start position (overrides XML areanum)
	r->reach.areanum = AAS_PointAreaNum(r->reach.start);

	// Debug print
	Log_Print("  reach: area=%d face=%d edge=%d start=(%.1f %.1f %.1f) end=(%.1f %.1f %.1f) type=%d time=%d\n",
		r->reach.areanum, r->reach.facenum, r->reach.edgenum,
		r->reach.start[0], r->reach.start[1], r->reach.start[2],
		r->reach.end[0], r->reach.end[1], r->reach.end[2],
		r->reach.traveltype, r->reach.traveltime);

	// Link into list
	r->next = state->head;
	state->head = r;
	state->count++;
	state->current = NULL;
}

static void RCH_StoreReachabilities(rch_state_t *state) {
	rch_reach_t *r, *next;
	int stored = 0;

	if (!state->count) {
		Log_Print("no reachabilities loaded from .rch file\n");
		return;
	}

	Log_Print("storing %d reachabilities from .rch file...\n", state->count);

	for (r = state->head; r; r = next) {
		next = r->next;

		int destarea = AAS_PointAreaNum(r->reach.end);
		if (destarea <= 0 || destarea >= aasworld.numareas) {
			Log_Print("WARNING: reach end point not in valid area, skipping\n");
			FreeMemory(r);
			continue;
		}

		if (aasworld.reachabilitysize >= AAS_MAX_REACHABILITYSIZE) {
			Log_Print("WARNING: AAS_MAX_REACHABILITYSIZE reached\n");
			FreeMemory(r);
			continue;
		}

		aas_reachability_t *dest = &aasworld.reachability[aasworld.reachabilitysize];
		*dest = r->reach;

		// Link into the destination area's reachability chain
		aasworld.areasettings[destarea].numreachableareas++;
		if (!aasworld.areasettings[destarea].firstreachablearea)
			aasworld.areasettings[destarea].firstreachablearea = aasworld.reachabilitysize;

		aasworld.reachabilitysize++;
		stored++;
		FreeMemory(r);
	}

	state->head = NULL;
	Log_Print("stored %d reachabilities from .rch file\n", stored);
}

qboolean RCH_LoadReachFile(const char *aasfilename) {
	char rchpath[MAX_PATH];
	FILE *fp;
	yxml_t x;
	char yxmlbuf[RCH_YXML_BUF_SIZE];
	char readbuf[RCH_READ_BUF_SIZE];
	rch_state_t state;
	size_t nread;
	int i;
	yxml_ret_t r;

	// Construct .rch path from .aas path
	Q_strncpyz(rchpath, aasfilename, sizeof(rchpath));
	// Replace .aas extension with .rch
	i = strlen(rchpath);
	while (i > 0 && rchpath[i - 1] != '.')
		i--;
	if (i > 0)
		rchpath[i - 1] = '\0';
	else
		rchpath[0] = '\0';
	strcat(rchpath, ".rch");

	fp = fopen(rchpath, "rb");
	if (!fp) {
		Log_Print("no .rch file found (%s)\n", rchpath);
		return qfalse;
	}

	Log_Print("loading reachability file %s...\n", rchpath);

	memset(&state, 0, sizeof(state));
	yxml_init(&x, yxmlbuf, sizeof(yxmlbuf));

	while ((nread = fread(readbuf, 1, sizeof(readbuf), fp)) > 0) {
		for (i = 0; i < (int)nread; i++) {
			r = yxml_parse(&x, readbuf[i]);
			if (r < 0) {
				Log_Print("ERROR: XML parse error in %s at line %u\n", rchpath, x.line);
				fclose(fp);
				return qfalse;
			}

			switch (r) {
			case YXML_ELEMSTART:
				state.depth++;
				if (!strcmp(x.elem, "reachability")) {
					state.in_reach = qtrue;
					state.current = (rch_reach_t *)GetMemory(sizeof(rch_reach_t));
					memset(state.current, 0, sizeof(rch_reach_t));
				} else if (state.in_reach && state.depth == 2) {
					// Sub-element of <reachability>
					Q_strncpyz(state.curelem, x.elem, sizeof(state.curelem));
					state.contentptr = state.content;
					state.content[0] = '\0';
				}
				break;

			case YXML_ELEMEND:
				if (state.in_reach && state.depth == 2 && state.current) {
					// Closing a sub-element: apply the accumulated content
					RCH_SetFieldValue(state.current, state.curelem, state.content);
					state.curelem[0] = '\0';
				} else if (state.in_reach && state.depth == 1) {
					// Closing </reachability>
					RCH_FinalizeReach(&state);
					state.in_reach = qfalse;
				}
				state.depth--;
				break;

			case YXML_CONTENT:
				// Accumulate text content for sub-elements
				if (state.in_reach && state.depth == 2 && state.contentptr) {
					char *s = x.data;
					while (*s && state.contentptr < state.content + sizeof(state.content) - 1)
						*(state.contentptr++) = *(s++);
					*state.contentptr = '\0';
				}
				break;

			default:
				break;
			}
		}
	}

	fclose(fp);

	if (yxml_eof(&x) < 0) {
		Log_Print("WARNING: .rch file %s has incomplete XML\n", rchpath);
	}

	RCH_StoreReachabilities(&state);
	Log_Print("reachability file loaded: %d entries\n", state.count);
	return qtrue;
}
