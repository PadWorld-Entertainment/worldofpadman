/*
===========================================================================
Copyright (C) 2010, 2011, 2012 the-brain

This program is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://gnu.org/licenses/>.
===========================================================================
*/

#ifndef _BE_VOTE_H
#define _BE_VOTE_H

typedef enum {
	VOTEID_NONE,
	VOTEID_NEXTMAP,
	VOTEID_MAP,
	VOTEID_MAP_RESTART,
	VOTEID_KICK,
	VOTEID_CLIENTKICK,
	VOTEID_TIMELIMIT,
	VOTEID_POINTLIMIT,
	VOTEID_G_GAMETYPE,
	VOTEID_SETGAMETYPE,
	VOTEID_SHUFFLETEAMS,
	VOTEID_NUM_VOTES
} voteID_t;

typedef qboolean (*voteFunc_t)(gentity_t *ent, voteID_t id);

/* NOTE: Should be unique */
typedef struct {
	char *str;
	voteID_t id;
} voteIdentifier_t;

typedef struct {
	voteIdentifier_t ident;
	voteFunc_t func;
} voteEntry_t;

void BE_Cmd_Vote_f(gentity_t *ent);
void BE_Cmd_CallVote_f(gentity_t *ent);

void BE_CheckVote(void);

#endif
