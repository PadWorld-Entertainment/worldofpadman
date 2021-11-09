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

#include "g_local.h"
#include "../botlib/botlib.h"
#include "../botlib/be_aas.h"
#include "../botlib/be_ai_goal.h"
#include "../botlib/be_ai_move.h"
#include "ai_main.h"

/*
 *  Called from original bot logic to determine whether to use BamBams
 *  and Boomies.
 */
qboolean BE_Bot_UseItems(void) {
	return (be_botFlags.integer & BE_BOT_USEITEMS);
}

qboolean BE_Bot_IgnoreItems(void) {
	return (be_botFlags.integer & BE_BOT_IGNOREITEMS);
}

qboolean BE_Bot_IllegalGoal(bot_state_t *bs) {
	bot_goal_t goal;
	char buf[128];

	trap_BotGetTopGoal(bs->gs, &goal);
	trap_BotGoalName(goal.number, buf, sizeof(buf));

	if (BE_Bot_IgnoreItems()) {
		if (!strcmp(buf, "BAMBAM") || !strcmp(buf, "BOOMiES")) {
			/* FIXME: Avoid forever (-1) or just some time? */
			trap_BotSetAvoidGoalTime(bs->gs, goal.number, 42);
			trap_BotPopGoal(bs->gs);

			return qtrue;
		}
	}

	return qfalse;
}

void BE_Bot_CheckEntity(bot_state_t *bs, entityState_t *state) {
	if (BE_Bot_IgnoreItems() && (ET_ITEM == state->eType) && (state->modelindex2 != 0)) {
		gitem_t *item = &bg_itemlist[state->modelindex];
		int avoidTime = 0;

		switch (item->giTag) {
		case HI_BAMBAM:
			// fall through
		case HI_BOOMIES:
			avoidTime = 160;
			break;

		default:
			break;
		}

		if (avoidTime > 0) {
			trap_BotAddAvoidSpot(bs->ms, state->pos.trBase, avoidTime, AVOID_ALWAYS);
		}
	}
}
