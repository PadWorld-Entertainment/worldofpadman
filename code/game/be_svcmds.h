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

#ifndef _BE_SVCMDS_H
#define _BE_SVCMDS_H

/* TODO: Maybe we should use structs similar to those in be_vote? */
typedef struct {
	const char *cmdName;
	void (*cmdHandler)(void);
} svcmd_t;

qboolean BE_ConCmd(const char *cmd);

#endif
