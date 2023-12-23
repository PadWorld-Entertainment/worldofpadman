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
// cg_scoreboard -- draw the scoreboard on top of the game screen
#include "cg_local.h"

#define SCOREBOARD_X (0)

#define SB_HEADER 73
#define SB_TOP (100)

// Where the status bar starts, so we don't overwrite it
#define SB_STATUSBAR 310

#define SB_NORMAL_HEIGHT 32

#define SB_INTER_HEIGHT 16 // interleaved height

// -48 => to fill the gaps between the "teams"
#define SB_MAXCLIENTS_NORMAL ((SB_STATUSBAR - SB_TOP - 48) / SB_NORMAL_HEIGHT)
#define SB_MAXCLIENTS_INTER ((SB_STATUSBAR - SB_TOP - 48) / SB_INTER_HEIGHT - 1)

// Used when interleaved

#define SB_LEFT_BOTICON_X (SCOREBOARD_X + 0)
#define SB_LEFT_HEAD_X (SCOREBOARD_X + 32)
#define SB_RIGHT_BOTICON_X (SCOREBOARD_X + 64)
#define SB_RIGHT_HEAD_X (SCOREBOARD_X + 96)
// Normal
#define SB_BOTICON_X (SCOREBOARD_X + 32)
#define SB_HEAD_X (SCOREBOARD_X + 64)

#define SB_SCORELINE_X 112

#define SB_RATING_WIDTH (6 * BIGCHAR_WIDTH)					// width 6
#define SB_SCORE_X (SB_SCORELINE_X + BIGCHAR_WIDTH)			// width 6
#define SB_RATING_X (SB_SCORELINE_X + 6 * BIGCHAR_WIDTH)	// width 6
#define SB_PING_X (SB_SCORELINE_X + 12 * BIGCHAR_WIDTH + 8) // width 5
#define SB_TIME_X (SB_SCORELINE_X + 17 * BIGCHAR_WIDTH + 8) // width 5
#define SB_NAME_X (SB_SCORELINE_X + 22 * BIGCHAR_WIDTH)		// width 15

// The new and improved score board
//
// In cases where the number of clients is high, the score board heads are interleaved
// here's the layout

//
//	0   32   80  112  144   240  320  400   <-- pixel position
//  bot head bot head score ping time name
//
//  wins/losses are drawn on bot icon now

/*
=================
CG_DrawClientScore

See also CG_ParseScores()
=================
*/
static void CG_DrawClientScore(int y, const score_t *score, const vec4_t color, float fade, int lineHeight) {
	int icony, heady, fontHeight;
	char string[1024];
	vec3_t headAngles;
	clientInfo_t *ci;
	int iconx, headx;
	int iconsize;

	if (score->client < 0 || score->client >= cgs.maxclients) {
		Com_Printf("Bad score->client: %i\n", score->client);
		return;
	}

	icony = y;
	heady = y;
	if (lineHeight > 16) {
		y += (lineHeight - 16) / 2;
		fontHeight = 16;
		iconsize = (int)((float)lineHeight * 0.9f);
		icony += (lineHeight - iconsize) / 2;
	} else {
		fontHeight = lineHeight;
		iconsize = lineHeight;
	}

	ci = &cgs.clientinfo[score->client];

	iconx = SB_BOTICON_X + (SB_RATING_WIDTH / 2);
	headx = SB_HEAD_X + (SB_RATING_WIDTH / 2);

	// freeze icon indicating frozen player in freezetag
	if (CG_FreezeTag() && ci->powerups & (1 << PW_FREEZE)) {
		CG_DrawPic(iconx, icony, lineHeight, lineHeight, cgs.media.freezeIconShader);
	// killerduck icon indicating player is killerduck carrier in ctkd
	} else if (ci->ctkdIsKillerduck) {
		CG_DrawDuckModel(iconx, icony, iconsize, iconsize);
	// cartridge icon indicating player carries cartridges in syc
	} else if (ci->numCartridges > 0) {
		CG_DrawCartModel(iconx, icony, iconsize, iconsize, ci->team);
	// red lolly icon indicating player has the red lolly in ctl
	} else if (ci->powerups & (1 << PW_REDFLAG)) {
		CG_DrawFlagModel(iconx, icony, iconsize, iconsize, TEAM_RED);
	// blue lolly icon indicating player has the blue lolly in ctl
	} else if (ci->powerups & (1 << PW_BLUEFLAG)) {
		CG_DrawFlagModel(iconx, icony, iconsize, iconsize, TEAM_BLUE);
	// neutral lolly icon indicating player has the neutral lolly in 1lctl
	} else if (ci->powerups & (1 << PW_NEUTRALFLAG)) {
		CG_DrawFlagModel(iconx, icony, iconsize, iconsize, TEAM_FREE);
	// draw the handicap or bot skill marker unless player has
	// lolly, cartridge, killer duck, or is frozen)
	} else {
		if (ci->botSkill > 0 && ci->botSkill <= 5) {
			CG_DrawPic(iconx, icony, iconsize, iconsize, cgs.media.botSkillShaders[ci->botSkill - 1]);
		} else if (ci->handicap < 100) {
			Com_sprintf(string, sizeof(string), "%i", ci->handicap);
			if (cgs.gametype == GT_TOURNAMENT)
				CG_DrawSmallStringColor(iconx, y - SMALLCHAR_HEIGHT / 2, string, color);
			else
				CG_DrawSmallStringColor(iconx, y, string, color);
		}
	}

	// draw the wins / losses
	if (cgs.gametype == GT_TOURNAMENT) {
		Com_sprintf(string, sizeof(string), "%i/%i", ci->wins, ci->losses);
		if (ci->handicap < 100 && !ci->botSkill) {
			CG_DrawSmallStringColor(iconx, y + SMALLCHAR_HEIGHT / 2, string, color);
		} else {
			CG_DrawStringExt(iconx, y, string, color, qfalse, qfalse, 8, fontHeight, 10);
		}
	}

	// draw the face
	VectorClear(headAngles);
	headAngles[YAW] = 180;
	CG_DrawHead(headx, heady, lineHeight, lineHeight, score->client, headAngles);

	// draw the score line
	if (score->ping == -1) {
		Com_sprintf(string, sizeof(string), " connecting   ");
	} else if (ci->team == TEAM_SPECTATOR) {
		Com_sprintf(string, sizeof(string), " SPECT %3i %4i", score->ping, score->time);
	} else {
		if (cgs.gametype == GT_LPS) {
			if (cgs.lpsflags & LPSF_PPOINTLIMIT) {
				Com_sprintf(string, sizeof(string), "%3i/%-2i %3i %4i", score->score, score->livesleft, score->ping,
							score->time);
			} else {
				Com_sprintf(string, sizeof(string), "%5i %4i %4i", score->livesleft, score->ping, score->time);
			}
		}
		Com_sprintf(string, sizeof(string), "%5i %4i %4i", score->score, score->ping, score->time);
	}

	// highlight your position
	if (score->client == cg.snap->ps.clientNum) {
		vec4_t hcolor;
		int rank;

		if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR || cgs.gametype >= GT_TEAM) {
			rank = -1;
		} else {
			rank = cg.snap->ps.persistant[PERS_RANK] & ~RANK_TIED_FLAG;
		}
		if (rank == 0) {
			hcolor[0] = 0;
			hcolor[1] = 0;
			hcolor[2] = 0.7f;
		} else if (rank == 1) {
			hcolor[0] = 0.7f;
			hcolor[1] = 0;
			hcolor[2] = 0;
		} else if (rank == 2) {
			hcolor[0] = 0.7f;
			hcolor[1] = 0.7f;
			hcolor[2] = 0;
		} else {
			hcolor[0] = 0.7f;
			hcolor[1] = 0.7f;
			hcolor[2] = 0.7f;
		}

		hcolor[3] = fade * 0.7f;

		CG_FillRect(SB_SCORELINE_X + BIGCHAR_WIDTH + (SB_RATING_WIDTH / 2), y, 400, fontHeight, hcolor);

		if ((cgs.gametype < GT_TEAM) && (cg.snap->ps.persistant[PERS_TEAM] == TEAM_FREE)) {
			// FIXME: These should be events caused by game
			if ((cg.snap->ps.pm_type == PM_INTERMISSION) && !cg.playedIntermissionMsg) {
				if ((cgs.gametype == GT_LPS) && (rank == 0)) {
					trap_S_StartLocalSound(cgs.media.winLPSSound[(int)(random() * 1.9999f)], CHAN_ANNOUNCER);
				} else if (((rank + 1) == cg.scoreTeamCount[TEAM_FREE]) && (cg.scoreTeamCount[TEAM_FREE] > 1)) {
					trap_S_StartLocalSound(cgs.media.loseFFASound, CHAN_ANNOUNCER);
				}

				cg.playedIntermissionMsg = qtrue;
			}
		}
	}

	CG_DrawStringExt(SB_SCORELINE_X + (SB_RATING_WIDTH / 2), y, string, color, qfalse, qfalse, 16, fontHeight, 0);
	CG_DrawStringExt(SB_NAME_X - (SB_RATING_WIDTH / 2), y, ci->name, color, qfalse, qfalse, 8, fontHeight, 20);

	// add the "ready" marker for intermission exiting
	if (cg.snap->ps.stats[STAT_CLIENTS_READY] & (1 << score->client)) {
		CG_DrawStringExt(iconx, y, "READY", color, qfalse, qfalse, 16, fontHeight, 0);
	}
}

/*
=================
CG_TeamScoreboard
=================
*/
// maxLines = max Lines for THIS team
static int CG_TeamScoreboard(int y, team_t team, float fade, int maxLines, int lineHeight) {
	int i;
	vec4_t color;
	int count;
	qboolean localSpecialDraw;
	int localScoreID = 0;

	color[0] = color[1] = color[2] = 0.0f;
	color[3] = fade;

	localSpecialDraw = qfalse;

	if ((cg.snap->ps.persistant[PERS_TEAM] == team) && (maxLines < cg.scoreTeamCount[team])) {
		count = 0;
		for (i = 0; i < cg.numScores; i++) {
			const score_t *score = &cg.scores[i];

			if (team == cgs.clientinfo[score->client].team) {
				count++;
			}

			if (score->client == cg.snap->ps.clientNum) {
				localSpecialDraw = (count > maxLines);
				localScoreID = i;
				break;
			}
		}
	}

	count = 0;
	for (i = 0; i < cg.numScores && count < maxLines; i++) {
		const score_t *score = &cg.scores[i];
		const clientInfo_t *ci = &cgs.clientinfo[score->client];

		if (team != ci->team) {
			continue;
		}

		if (localSpecialDraw && ((maxLines - count) <= 2)) {
			if ((maxLines - count) == 2) {
				CG_DrawStringExt(SB_SCORELINE_X, (y + lineHeight * count),
								 va("... %i Players skipped ...", (cg.scoreTeamCount[team] - maxLines + 1)), colorBlack,
								 qfalse, qfalse, 8, lineHeight, 40);
			} else if ((maxLines - count) == 1) {
				CG_DrawClientScore((y + lineHeight * count), &cg.scores[localScoreID], colorBlack, fade, lineHeight);
			}
		} else if ((maxLines < cg.scoreTeamCount[team]) && (!localSpecialDraw) && ((maxLines - count) == 1)) {
			CG_DrawStringExt(SB_SCORELINE_X, (y + lineHeight * count),
							 va("... %i Players skipped ...", (cg.scoreTeamCount[team] - maxLines + 1)), colorBlack,
							 qfalse, qfalse, 8, lineHeight, 40);
		} else {
			CG_DrawClientScore((y + lineHeight * count), score, color, fade, lineHeight);
		}

		count++;
	}

	return count;
}

// draw medals
static void CG_DrawMedals(float x, float y, const score_t *score) {
	char buf[32];

	CG_DrawPic(x, y, 60, 24, cgs.media.scoreboardMedals);
	x += 64;
	CG_DrawPic(x, y, 24, 24, cgs.media.medalExcellent);
	Com_sprintf(buf, sizeof(buf), "%d", score->excellentCount);
	CG_DrawStringExt(x + 12 - SMALLCHAR_WIDTH * CG_DrawStrlen(buf) / 2, y + 12 - SMALLCHAR_HEIGHT / 2, buf, colorWhite, qfalse, qtrue, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0);
	x += 30;
	CG_DrawPic(x, y, 24, 24, cgs.media.medalSnackAttack);
	Com_sprintf(buf, sizeof(buf), "%d", score->snackattackCount);
	CG_DrawStringExt(x + 12 - SMALLCHAR_WIDTH * CG_DrawStrlen(buf) / 2, y + 12 - SMALLCHAR_HEIGHT / 2, buf, colorWhite, qfalse, qtrue, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0);
	if (cgs.gametype == GT_SPRAYFFA || cgs.gametype == GT_SPRAY) {
		x += 30;
		CG_DrawPic(x, y, 24, 24, cgs.media.medalSprayKiller);
		Com_sprintf(buf, sizeof(buf), "%d", score->spraykillerCount);
		CG_DrawStringExt(x + 12 - SMALLCHAR_WIDTH * CG_DrawStrlen(buf) / 2, y + 12 - SMALLCHAR_HEIGHT / 2, buf, colorWhite, qfalse, qtrue, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0);
		x += 32;
		CG_DrawPic(x, y, 24, 24, cgs.media.medalSprayGod);
		Com_sprintf(buf, sizeof(buf), "%d", score->spraygodCount);
		CG_DrawStringExt(x + 12 - SMALLCHAR_WIDTH * CG_DrawStrlen(buf) / 2, y + 12 - SMALLCHAR_HEIGHT / 2, buf, colorWhite, qfalse, qtrue, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0);
	}
	if (cgs.gametype == GT_CTF || cgs.gametype == GT_1FCTF || cgs.gametype == GT_BALLOON) {
		const clientInfo_t *ci = &cgs.clientinfo[score->client];
		x += 30;
		if (ci->team == TEAM_RED) {
			CG_DrawPic(x, y, 24, 24, cgs.media.medalPadStarRed);
		} else {
			CG_DrawPic(x, y, 24, 24, cgs.media.medalPadStar);
		}
		Com_sprintf(buf, sizeof(buf), "%d", score->captures);
		CG_DrawStringExt(x + 12 - SMALLCHAR_WIDTH * CG_DrawStrlen(buf) / 2, y + 12 - SMALLCHAR_HEIGHT / 2, buf, colorWhite, qfalse, qtrue, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0);
	}
	if (cgs.gametype == GT_CTF || cgs.gametype == GT_1FCTF || cgs.gametype == GT_BALLOON || cgs.gametype == GT_SPRAY) {
		x += 30;
		CG_DrawPic(x, y, 24, 24, cgs.media.medalPadHero);
		Com_sprintf(buf, sizeof(buf), "%d", score->padheroCount);
		CG_DrawStringExt(x + 12 - SMALLCHAR_WIDTH * CG_DrawStrlen(buf) / 2, y + 12 - SMALLCHAR_HEIGHT / 2, buf, colorWhite, qfalse, qtrue, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0);
		x += 30;
		CG_DrawPic(x, y, 24, 24, cgs.media.medalPadAce);
		Com_sprintf(buf, sizeof(buf), "%d", score->padaceCount);
		CG_DrawStringExt(x + 12 - SMALLCHAR_WIDTH * CG_DrawStrlen(buf) / 2, y + 12 - SMALLCHAR_HEIGHT / 2, buf, colorWhite, qfalse, qtrue, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0);
	}
}

static const score_t *CG_GetScoreForClient(int clientNum) {
	int i;
	for (i = 0; i < cg.numScores; ++i) {
		if (cg.scores[i].client == clientNum) {
			return &cg.scores[i];
		}
	}
	return NULL;
}

/*
=================
CG_DrawOldScoreboard

Draw the normal in-game scoreboard
=================
*/
qboolean CG_DrawOldScoreboard(void) {
	int x, y, i;
	float fade;
	float *fadeColor;
	int maxClients;
	int lineHeight;
	int topBorderSize, bottomBorderSize;
	int scoreLineSpace, maxScoreLinesPerTeam[TEAM_NUM_TEAMS];
	const score_t *score;
	clientInfo_t *ci;
	char buf[32];

	// don't draw anything if the menu or console is up
	if (cg_paused.integer) {
		cg.deferredPlayerLoading = 0;
		return qfalse;
	}

	if (cgs.gametype == GT_SINGLE_PLAYER && cg.predictedPlayerState.pm_type == PM_INTERMISSION) {
		cg.deferredPlayerLoading = 0;
		return qfalse;
	}

	// don't draw scoreboard during death while warmup up
	if (cg.warmup && !cg.showScores) {
		return qfalse;
	}

	score = CG_GetScoreForClient(cg.clientNum);

	if (cg.showScores || cg.predictedPlayerState.pm_type == PM_DEAD ||
		cg.predictedPlayerState.pm_type == PM_INTERMISSION) {
		fade = 1.0f;
		fadeColor = colorWhite;
	} else {
		fadeColor = CG_FadeColor(cg.scoreFadeTime, FADE_TIME);

		if (!fadeColor) {
			// next time scoreboard comes up, don't print killer
			cg.deferredPlayerLoading = 0;
			cg.killerName[0] = 0;
			return qfalse;
		}
		fade = *fadeColor;
	}

	CG_DrawPic(40, 10, 560, 470, cgs.media.scoreboardBG);

	// fragged by ... line
	if (cg.killerName[0]) {
		char *s = va(S_COLOR_BLACK "Fragged by %s", cg.killerName);
		int w = (CG_DrawStrlen(s) * BIGCHAR_WIDTH);
		x = ((SCREEN_WIDTH - w) / 2);
		y = (SB_TOP - 60);

		CG_DrawStringExt(x, y, s, fadeColor, qfalse, qfalse, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0);
	}

	// current rank
	if (cgs.gametype < GT_TEAM) {
		if (cg.snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR) {
			char *s;
			int w;
			if (cgs.gametype == GT_LPS) {
				int ownLives = cg.snap->ps.stats[STAT_LIVESLEFT];
				if (ownLives < 0) {
					ownLives = 0;
				}

				if (cgs.lpsflags & LPSF_PPOINTLIMIT) {
					s = va("%s" S_COLOR_BLACK " place with %i Points & %i Lives",
						   CG_PlaceString(cg.snap->ps.persistant[PERS_RANK] + 1), cg.snap->ps.persistant[PERS_SCORE],
						   ownLives);
				} else {
					s = va("%s" S_COLOR_BLACK " place with %i Lives",
						   CG_PlaceString(cg.snap->ps.persistant[PERS_RANK] + 1), ownLives);
				}
			} else {
				s = va("%s" S_COLOR_BLACK " place with %i", CG_PlaceString(cg.snap->ps.persistant[PERS_RANK] + 1),
					   cg.snap->ps.persistant[PERS_SCORE]);
			}

			w = (CG_DrawStrlen(s) * BIGCHAR_WIDTH);
			x = ((SCREEN_WIDTH - w) / 2);
			y = (SB_TOP - 40);
			CG_DrawStringExt(x, y, s, fadeColor, qfalse, qfalse, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0);
		}
	} else {
		char *s;
		int w;
		// FIXME: Use some sort of team_t define/enum
		if (cg.teamScores[0] == cg.teamScores[1]) {
			if (CG_FreezeTag())
				s = va(S_COLOR_BLACK "Teams are tied at %i rounds", cg.teamScores[0]);
			else
				s = va(S_COLOR_BLACK "Teams are tied at %i", cg.teamScores[0]);
		} else if (cg.teamScores[0] >= cg.teamScores[1]) {
			if (CG_FreezeTag())
				s = va(S_COLOR_RED "Red" S_COLOR_BLACK " leads " S_COLOR_RED "%i" S_COLOR_BLACK " to " S_COLOR_BLUE
								   "%i" S_COLOR_BLACK " rounds",
					   cg.teamScores[0], cg.teamScores[1]);
			else
				s = va(S_COLOR_RED "Red" S_COLOR_BLACK " leads " S_COLOR_RED "%i" S_COLOR_BLACK " to " S_COLOR_BLUE
								   "%i",
					   cg.teamScores[0], cg.teamScores[1]);
		} else {
			if (CG_FreezeTag())
				s = va(S_COLOR_BLUE "Blue" S_COLOR_BLACK " leads " S_COLOR_BLUE "%i" S_COLOR_BLACK " to " S_COLOR_RED
									"%i" S_COLOR_BLACK " rounds",
					   cg.teamScores[1], cg.teamScores[0]);
			else
				s = va(S_COLOR_BLUE "Blue" S_COLOR_BLACK " leads " S_COLOR_BLUE "%i" S_COLOR_BLACK " to " S_COLOR_RED
									"%i",
					   cg.teamScores[1], cg.teamScores[0]);
		}

		w = (CG_DrawStrlen(s) * BIGCHAR_WIDTH);
		x = ((SCREEN_WIDTH - w) / 2);
		y = (SB_TOP - 40);
		CG_DrawStringExt(x, y, s, fadeColor, qfalse, qfalse, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0);
	}

	// scoreboard
	y = SB_HEADER;
	// better set x values manually since WoP uses individual header pictures, not a font set
	if (cgs.gametype == GT_LPS) {
		if (cgs.lpsflags & LPSF_PPOINTLIMIT) {
			CG_DrawPic(174, y, 80, 32, cgs.media.scoreboardScoreLives);
		} else {
			CG_DrawPic(180, y, 64, 32, cgs.media.scoreboardLives);
		}
	} else {
		CG_DrawPic(176, y, 64, 32, cgs.media.scoreboardScore);
	}

	CG_DrawPic(264, y, 64, 32, cgs.media.scoreboardPing);
	CG_DrawPic(342, y, 64, 32, cgs.media.scoreboardTime);
	CG_DrawPic(412, y, 64, 32, cgs.media.scoreboardName);

	y = SB_TOP;

	// If there are more than SB_MAXCLIENTS_NORMAL, use the interleaved scores
	topBorderSize = 4;
	bottomBorderSize = 4;

	if (cgs.gametype >= GT_TEAM) {
		scoreLineSpace = (SB_STATUSBAR - SB_TOP - (2 * topBorderSize + bottomBorderSize));
	} else {
		scoreLineSpace = (SB_STATUSBAR - SB_TOP);
	}

	if (cg.scoreTeamCount[TEAM_SPECTATOR] > 0) {
		scoreLineSpace -= 8;
	}

	if (cg.numScores == 0) {
		lineHeight = 50;
	} else {
		lineHeight = (scoreLineSpace / cg.numScores);
	}

	maxClients = cg.numScores;

	for (i = 0; i < TEAM_NUM_TEAMS; i++) {
		maxScoreLinesPerTeam[i] = cg.scoreTeamCount[i];
	}

	if (lineHeight > 40) {
		lineHeight = 40;
	} else if (lineHeight < 10) {
		int numTeams, averageTeamLines, linesLeft;
		qboolean teamDone[TEAM_NUM_TEAMS];

		linesLeft = (scoreLineSpace / 10);

		for (i = 0; i < TEAM_NUM_TEAMS; i++) {
			teamDone[i] = (cg.scoreTeamCount[i] <= 0);
		}

		numTeams = (!teamDone[TEAM_FREE] + !teamDone[TEAM_RED] + !teamDone[TEAM_BLUE] + !teamDone[TEAM_SPECTATOR]);

		while (numTeams) {
			qboolean anyTeamDone = qfalse;

			for (i = 0; i < TEAM_NUM_TEAMS; i++) {
				if (!numTeams) {
					break;
				}

				averageTeamLines = (linesLeft / numTeams);

				if ((!teamDone[i]) && (cg.scoreTeamCount[i] <= averageTeamLines)) {

					maxScoreLinesPerTeam[i] = cg.scoreTeamCount[i];
					teamDone[i] = qtrue;
					linesLeft -= cg.scoreTeamCount[i];
					anyTeamDone = qtrue;
				}

				numTeams =
					(!teamDone[TEAM_FREE] + !teamDone[TEAM_RED] + !teamDone[TEAM_BLUE] + !teamDone[TEAM_SPECTATOR]);
			}

			if (!anyTeamDone) {
				for (i = 0; i < TEAM_NUM_TEAMS; i++) {
					if (!teamDone[i]) {
						maxScoreLinesPerTeam[i] = averageTeamLines;
						teamDone[i] = qtrue;
					}
				}
				numTeams = 0;
			}
		}

		lineHeight = 10;
		maxClients = (scoreLineSpace / lineHeight);
	}

	if (cgs.gametype >= GT_TEAM) {
		int n1;
		//
		// teamplay scoreboard
		//

		y += topBorderSize;

		if (cg.teamScores[0] >= cg.teamScores[1]) {
			int n2;
			CG_DrawTeamBackground(70, y - topBorderSize, 506,
								  maxScoreLinesPerTeam[TEAM_RED] * lineHeight + topBorderSize + bottomBorderSize, 0.66f,
								  TEAM_RED);
			n1 = CG_TeamScoreboard(y, TEAM_RED, fade, maxScoreLinesPerTeam[TEAM_RED], lineHeight);
			y += (n1 * lineHeight) + bottomBorderSize + topBorderSize;
			maxClients -= n1;
			CG_DrawTeamBackground(70, y - topBorderSize, 506,
								  maxScoreLinesPerTeam[TEAM_BLUE] * lineHeight + topBorderSize + bottomBorderSize,
								  0.66f, TEAM_BLUE);
			n2 = CG_TeamScoreboard(y, TEAM_BLUE, fade, maxScoreLinesPerTeam[TEAM_BLUE], lineHeight);
			y += (n2 * lineHeight) + bottomBorderSize;
			maxClients -= n2;
		} else {
			int n2;
			CG_DrawTeamBackground(70, y - topBorderSize, 506,
								  maxScoreLinesPerTeam[TEAM_BLUE] * lineHeight + topBorderSize + bottomBorderSize,
								  0.66f, TEAM_BLUE);
			n1 = CG_TeamScoreboard(y, TEAM_BLUE, fade, maxScoreLinesPerTeam[TEAM_BLUE], lineHeight);
			y += (n1 * lineHeight) + bottomBorderSize + topBorderSize;
			maxClients -= n1;
			CG_DrawTeamBackground(70, y - topBorderSize, 506,
								  maxScoreLinesPerTeam[TEAM_RED] * lineHeight + topBorderSize + bottomBorderSize, 0.66f,
								  TEAM_RED);
			n2 = CG_TeamScoreboard(y, TEAM_RED, fade, maxScoreLinesPerTeam[TEAM_RED], lineHeight);
			y += (n2 * lineHeight) + bottomBorderSize;
			maxClients -= n2;
		}

		y += 8;
		n1 = CG_TeamScoreboard(y, TEAM_SPECTATOR, fade, maxScoreLinesPerTeam[TEAM_SPECTATOR], lineHeight);
		y += (n1 * lineHeight);
	} else {
		//
		// free for all scoreboard
		//
		int n1 = CG_TeamScoreboard(y, TEAM_FREE, fade, maxScoreLinesPerTeam[TEAM_FREE], lineHeight);
		int n2;
		y += (n1 * lineHeight) + 8;
		n2 = CG_TeamScoreboard(y, TEAM_SPECTATOR, fade, maxScoreLinesPerTeam[TEAM_SPECTATOR], lineHeight);
		y += (n2 * lineHeight);
	}

	CG_DrawMedals(64.0f, 313.0f, score);

	// draw accuracy rate
	CG_DrawPic(342.0f, 313.0f, 96.0f, 24.0f, cgs.media.scoreboardAccuracy);
	Com_sprintf(buf, sizeof(buf), "%i%%", score->accuracy);
	CG_DrawStringExt(440, 316, buf, colorWhite, qfalse, qtrue, BIGCHAR_WIDTH / 2 , BIGCHAR_HEIGHT, 0);

	// load any models that have been deferred
	if (++cg.deferredPlayerLoading > 10) {
		CG_LoadDeferredPlayers();
	}

	return qtrue;
}

//================================================================================

/*
================
CG_CenterGiantLine
================
*/
static void CG_CenterGiantLine(float y, const char *string) {
	const float x = 0.5f * (SCREEN_WIDTH - GIANT_WIDTH * CG_DrawStrlen(string));
	CG_DrawStringExt(x, y, string, colorWhite, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0);
}

/*
=================
CG_DrawTourneyScoreboard

Draw the oversize scoreboard for tournements
=================
*/
void CG_DrawOldTourneyScoreboard(void) {
	const char *s;
	int min, tens, ones;
	clientInfo_t *ci;
	int y;
	int i;

	// request more scores regularly
	if (cg.scoresRequestTime + 2000 < cg.time) {
		cg.scoresRequestTime = cg.time;
		trap_SendClientCommand("score");
	}

	// draw the dialog background
	CG_FillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, colorBlack);

	// print the message of the day
	s = CG_ConfigString(CS_MOTD);
	if (!s[0]) {
		s = "Scoreboard";
	}

	// print optional title
	CG_CenterGiantLine(8, s);

	// print server time
	ones = cg.time / 1000;
	min = ones / 60;
	ones %= 60;
	tens = ones / 10;
	ones %= 10;
	s = va("%i:%i%i", min, tens, ones);

	CG_CenterGiantLine(64, s);

	// print the two scores

	y = 160;
	if (cgs.gametype >= GT_TEAM) {
		//
		// teamplay scoreboard
		//
		CG_DrawStringExt(8, y, "Red Pads", colorWhite, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0);
		s = va("%i", cg.teamScores[0]);
		CG_DrawStringExt(SCREEN_WIDTH - 8 - GIANT_WIDTH * strlen(s), y, s, colorWhite, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0);

		y += 64;

		CG_DrawStringExt(8, y, "Blue Noses", colorWhite, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0);
		s = va("%i", cg.teamScores[1]);
		CG_DrawStringExt(SCREEN_WIDTH - 8 - GIANT_WIDTH * strlen(s), y, s, colorWhite, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0);
	} else {
		//
		// free for all scoreboard
		//
		for (i = 0; i < MAX_CLIENTS; i++) {
			ci = &cgs.clientinfo[i];
			if (!ci->infoValid) {
				continue;
			}
			if (ci->team != TEAM_FREE) {
				continue;
			}

			CG_DrawStringExt(8, y, ci->name, colorWhite, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0);
			s = va("%i", ci->score);
			CG_DrawStringExt(SCREEN_WIDTH - 8 - GIANT_WIDTH * strlen(s), y, s, colorWhite, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0);
			y += 64;
		}
	}
}
