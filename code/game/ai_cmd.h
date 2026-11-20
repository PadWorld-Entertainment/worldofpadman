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
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//

/*****************************************************************************
 * name:		ai_cmd.h
 *
 * desc:		Quake3 bot AI
 *
 * $Archive: /source/code/botai/ai_chat.c $
 *
 *****************************************************************************/

// Team chat and tells retain the server's sender slot in the console queue type.
#define CMS_TEAMCHAT_FROM 2

int BotMatchMessage(bot_state_t *bs, const char *message, int sender);
qboolean BotHasActiveOrder(const bot_state_t *bs);
qboolean BotOrderActive(bot_state_t *bs);
void BotClearOrder(bot_state_t *bs);
void BotSetTeamOrder(bot_state_t *bs, int leader, int teammate, int type, float duration, const bot_goal_t *goal);
qboolean BotPlayerLocation(int client, bot_goal_t *goal);
void BotSendOrderReply(bot_state_t *bs);
//float BotNearestVisibleItem(bot_state_t *bs, const char *itemname, bot_goal_t *goal);
