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

#ifndef _BE_BOT_H
#define _BE_BOT_H

enum { BE_BOT_USEITEMS = 1, BE_BOT_IGNOREITEMS = 2 };

extern vmCvar_t be_botFlags;

qboolean BE_Bot_UseItems(void);
void BE_Bot_CheckEntity(bot_state_t *bs, entityState_t *state);
qboolean BE_Bot_IllegalGoal(bot_state_t *bs);

#endif /* _BE_BOT_H */
