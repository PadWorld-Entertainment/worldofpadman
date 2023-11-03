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
/*
=======================================================================

CREDITS (Short credits when leaving World of PADMAN)

=======================================================================
*/

#include "ui_local.h"

#define CREDITSPIC "menu/bg/credits"

typedef struct {
	menuframework_s menu;

} credits_t;

static credits_t s_credits;

/*
=================
UI_CreditMenu_Key
=================
*/
static sfxHandle_t UI_CreditMenu_Key(int key) {
	if (key & K_CHAR_FLAG) {
		return 0;
	}

	trap_Cmd_ExecuteText(EXEC_INSERT, "quit\n");
	return 0;
}

/*
===============
UI_CreditMenu_Draw
===============
*/
static void UI_CreditMenu_Draw(void) {

	UI_DrawNamedPic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CREDITSPIC);
}

/*
===============
UI_CreditMenu
===============
*/
void UI_CreditMenu(void) {
	memset(&s_credits, 0, sizeof(s_credits));

	UI_Credit_Cache();

	s_credits.menu.draw = UI_CreditMenu_Draw;
	s_credits.menu.key = UI_CreditMenu_Key;
	s_credits.menu.fullscreen = qtrue;
	UI_PushMenu(&s_credits.menu);
}

/*
===============
UI_Credit_Cache
===============
*/
void UI_Credit_Cache(void) {
	trap_R_RegisterShaderNoMip(CREDITSPIC);
}

/*
=======================================================================

WOP CREDITS (Scrolling credits for World of PADMAN)

=======================================================================
*/

#define WOPCREDITSBG "menu/wopcredits"
#define TRACKLENGTH 343 //length of the credits track in seconds

typedef struct {
	menuframework_s menu;

} wopcredits_t;

static wopcredits_t s_wopcredits;

int starttime;
float mvolume;

const vec4_t color_programming = {1.0f, 0.6f, 1.0f, 1.0f};
const vec4_t color_models = {1.0f, 0.6f, 0.0f, 1.0f};
const vec4_t color_skins = {0.8f, 1.0f, 0.0f, 1.0f};
const vec4_t color_artwork = {1.0f, 0.4f, 0.0f, 1.0f};
const vec4_t color_voiceart = {0.0f, 1.0f, 0.0f, 1.0f};
const vec4_t color_intro = {1.0f, 0.4f, 1.0f, 1.0f};
const vec4_t color_skyboxes = {0.6f, 0.8f, 1.0f, 1.0f};
const vec4_t color_soundtrack = {1.0f, 0.8f, 0.0f, 1.0f};
const vec4_t color_prnews = {0.8f, 1.0f, 0.0f, 1.0f};
const vec4_t color_thx = {1.0f, 0.2f, 0.0f, 1.0f};
const vec4_t color_mthx = {1.0f, 0.8f, 0.99f, 1.0f};
const vec4_t color_sthx = {0.6f, 0.6f, 1.0f, 1.0f};
const vec4_t color_vsthx = {0.6f, 0.8f, 0.2f, 1.0f};
const vec4_t color_memory = {0.0f, 0.64f, 0.99f, 1.0f};

typedef struct {
	char *string;
	int style;
	const vec4_t *color;
} cr_line;

static cr_line credits[] = {

	{"World of PADMAN", UI_CENTER|UI_GIANTFONT|UI_DROPSHADOW, &color_white},
	{"20th Anniversary", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW|UI_PULSE , &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Lead & Game Design", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_yellow},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Andreas (ENTE) Endres", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Programming", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_programming},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Stefan (#@) Langer", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Uwe (Cyrri) Koch", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Martin (mgerhardy) Gerhardy", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Niklas (brain) Link", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Thilo (Thilo) Schulz", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Paul (paulR) Rabe", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Kai (Kai-Li) Bergmann", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Herby", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Level Design", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_red},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Andreas Endres", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Joerg (Glowstar) Holsten", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Simon (Harmonieman) Furkert", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Kai Bergmann", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Martin (Cyben) Lange", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Andreas (MopAn) Spitzer", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Michael (oregano4) Grzesik", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Models & Animation", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_models},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Sebastian (doomdragon) Henke", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Kevin (SLoB) Jaques", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Manuel (Gogitason) Reyes", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Paul (paulR) Rabe", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"James (PencilWhipped) Johnson", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Tim (tpe) Evison", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Tony (Tone) Bellott", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Felix (FEzzz) Stellmacher", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Player Skins", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_skins},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Andreas Endres", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Sebastian Henke", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Manuel Reyes", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Kevin Jaques", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Tim Evison", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"James Johnson", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Camilla (milla) Koutsos", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Mario (Shocker) Perez Leal", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"2D Artwork", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_artwork},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Andreas Endres", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Andreas Spitzer", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"MightyPete", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Voice Art", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_voiceart},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Angie Lee Stahl", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Eileen Heath", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Andreas Endres", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Simon Furkert", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Sebastian Henke", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Intro & Trailers", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_intro},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Sebastian Henke", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Heiko (Mel O'Dee) Klueh", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Raptaker", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Sound Assortment", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &text_color_highlight},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Andreas Endres", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Kai Bergmann", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Skyboxes", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_skyboxes},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"MightyPete", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Soundtrack", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_soundtrack},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"dieselkopf", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"neurological", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Green Sun", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"PR & News", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_prnews},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Andreas Endres", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Kai Bergmann", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Simon Furkert", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Beta Tester", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_yellow},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"3aTmE!", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Adri{HUN}", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Hectic", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Ben_the_PadKing", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Iltis", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Imagine", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Sauerbraten", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Thanks To", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_thx},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"GrimReaper", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"GreenThumB", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"MrFloppi", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"jube", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Redlemons", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"chaOs", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Landi", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"QuarterPounder", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Maverick", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Sunrabbit", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Gekitsu", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"DemonPrincess", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"GomJabbar", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Wakey", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Paster", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Smiley", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"meimeiriver", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Fiesling", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Psychic_Pad", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Leo2701", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Shooeyy", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"More Thanks To", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_mthx},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Gerrit Neuendorf", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Scott Reismanis", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Ragman", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Magnacus", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Levelord", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Jeremy Williams", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Budi", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Pappy-R", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"CaliGirl", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Special Thanks To", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_sthx},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"id Software", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"ioQuake3", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"ModDB & IndieDB", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Mod.io", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Spawnpoint", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"GitHub", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Giga TV", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"MTV", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"In Memory Of", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_memory},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Pierre (Peyo) Culliford", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Creator Of The Smurfs", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Very Special Thanks To", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW, &color_vsthx},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"The fantastic", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"[PAD]Community", UI_CENTER|UI_BIGFONT|UI_DROPSHADOW|UI_PULSE, &color_white},
	{"You Rock!", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"And Our Families And", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Friends, Whom We Often", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Spend Little Time With", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{"Your WoP Team", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"PadWorld Entertainment", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"(2024)", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"And Now,", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Has Anybody Seen", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"Our Real Life?", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},
	{"", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, &color_white},

	{NULL}
};

/*
=================
UI_WopCredits_Draw
=================
*/
static void UI_WopCredits_Draw(void) {
int x = 270, y, n, ysize = 0, fadetime = 2.0f;
vec4_t fadecolour = {0.0f, 0.0f, 0.0f, 0.0f};

	if (!ysize) {
		for(n = 0; n <= sizeof(credits) - 1; n++) {
			if(credits[n].style & UI_SMALLFONT) {
				ysize += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
			} else if (credits[n].style & UI_BIGFONT) {
				ysize += PROP_HEIGHT;
			} else if (credits[n].style & UI_GIANTFONT) {
				ysize += PROP_HEIGHT * (1 / PROP_SMALL_SIZE_SCALE);
			}
		}
	}

	UI_DrawNamedPic(216, 0, 648, SCREEN_HEIGHT, WOPCREDITSBG); //pictures have 4:3 resolution referenced

	y = SCREEN_HEIGHT - (float)((ysize - 5 * SCREEN_HEIGHT)/TRACKLENGTH) * (float)(uis.realtime - starttime) / 10000;
	for (n = 0; n <= sizeof(credits) - 1; n++) {
		if(credits[n].string == NULL) {
			if (y < -16) {
				UI_StopMusic();
				trap_Cmd_ExecuteText(EXEC_APPEND, va("s_musicvolume %f\n", mvolume));
				UI_PopMenu();
				break;
			}
			break;
		}

		if (strlen(credits[n].string) == 1) {
			continue;
		}

		if (y > -(PROP_HEIGHT * (1 / PROP_SMALL_SIZE_SCALE))) {
			UI_DrawProportionalString(x, y, credits[n].string, credits[n].style, *credits[n].color);
		}

		if(credits[n].style & UI_SMALLFONT) {
			y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
		} else if (credits[n].style & UI_BIGFONT) {
			y += PROP_HEIGHT;
		} else if (credits[n].style & UI_GIANTFONT) {
			y += PROP_HEIGHT * (1 / PROP_SMALL_SIZE_SCALE);
		}

		if (y > SCREEN_HEIGHT) {
			break;
		}
	}
}

/*
===============
UI_WopCredits_MenuKey
===============
*/
static sfxHandle_t UI_WopCredits_MenuKey(int key) {
	if (key == K_MOUSE2 || key == K_ESCAPE) {
		UI_StopMusic(); // -> durch refresh sollte automatisch wieder der normale starten
		trap_Cmd_ExecuteText(EXEC_APPEND, va("s_musicvolume %f\n", mvolume));
	}
	return Menu_DefaultKey(uis.activemenu, key);
}

/*
===============
UI_WopCredits_MenuInit
===============
*/
void UI_WopCredits_MenuInit(void) {
	memset(&s_wopcredits, 0, sizeof(s_wopcredits));

	UI_WopCredits_Cache();

	starttime = uis.realtime;
	mvolume = trap_Cvar_VariableValue( "s_musicvolume" );
	if (mvolume < 0.5) {
		trap_Cmd_ExecuteText(EXEC_APPEND, "s_musicvolume 0.5\n");
	}

	UI_StartCreditMusic();

	s_wopcredits.menu.fullscreen = qtrue;
	s_wopcredits.menu.key = UI_WopCredits_MenuKey;
	s_wopcredits.menu.draw = UI_WopCredits_Draw;
	s_wopcredits.menu.bgparts = BGP_MENUFX;

}

/*
===============
UI_WopCredits_Cache
===============
*/
void UI_WopCredits_Cache(void) {
	trap_R_RegisterShaderNoMip(WOPCREDITSBG);
}

/*
===============
UI_WopCreditsMenu
===============
*/
void UI_WopCreditsMenu(void) {
	UI_WopCredits_MenuInit();
	UI_PushMenu(&s_wopcredits.menu);
}
