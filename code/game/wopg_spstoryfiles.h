/*
####################### ####################### #######################

	functions for handling SinglePlayerStory-files by #@

  * one file contains one story
  * the first StoryElement in file is the one which starts the story
  * a StoryElement stays for: one map, one intro(comic, video,...), win/loos extro(...), win/loos nextAction, options(gametype, limits, winning conditions,...)
  * "wop_spstoryfiles.*" should only contain the functions to handle the files(putting the data into a struct)

  ... includes also a simple savefile-format/system

####################### ####################### #######################
*/

#ifndef WOP_SPSTORYFILES_H
#define WOP_SPSTORYFILES_H

#include "../qcommon/q_shared.h"

#define WOPSP_INTROANDEXTRO_STR	128
#define WOPSP_MAPNAME_STR		32 // like ui_server*.c (old/vq3 16 ... but we have some longer names, like wop_kaistrashmap)
#define WOPSP_ELEMENTNAME_STR	64 // like MAX_QPATH
#define WOPSP_NEXTELEMENT_STR	128

#define WOPSP_BOTLIST_STR		512

typedef enum {
	WSEF_STARTMAP	= 0x0001, // no weapon, no hud, ...
	WSEF_ENDMAP		= 0x0002, // only punchy
} wop_seFlags_t;

typedef struct wop_StoryElement_s {
	char	name[WOPSP_ELEMENTNAME_STR];
	int		elementID; // Element Number in this file

	char	intro[WOPSP_INTROANDEXTRO_STR];

	char	map[WOPSP_MAPNAME_STR];
	int		gametype;
	int		timelimit;
	int		pointlimit;
	int		flags; // wop_seFlags_t
	//TODO: instaGib
	//TODO: LPS options
	//TODO: handle winning conditions
	//~force player into team X , for TeamPlayer-gts
	char	forceToTeam[8];
	//bots ... ("[botname] [botname] [botname] ..."? ... "[botname]@red [botname]@blue ..." )
	char	bots[WOPSP_BOTLIST_STR];

	char	extroWin[WOPSP_INTROANDEXTRO_STR];
	char	extroLoos[WOPSP_INTROANDEXTRO_STR];

	char	nextAfterWin[WOPSP_NEXTELEMENT_STR];
	char	nextAfterLoos[WOPSP_NEXTELEMENT_STR];
} wop_StoryElement_t;

void wopSP_initStoryElement(wop_StoryElement_t* e);
qboolean wopSP_validSE(const wop_StoryElement_t* e);

int wopSP_openStory(const char* storyname); // adds default path+suffix and use wopSP_openFile
int wopSP_openFile(const char* filename);
int wopSP_loadNextStoryElement( wop_StoryElement_t* newElement );
int wopSP_findStoryElement( const char* storyElementName, wop_StoryElement_t* newElement );
void wopSP_closeFile(void);

qboolean wopSP_compareWithNextString(const char* nextStr, const wop_StoryElement_t* element);
void wopSP_execStoryElement(const wop_StoryElement_t* newElement);


// Save/Load gamestates("savegame") functions
qboolean wopSP_loadGameState(const char* storyname, const char* savename, char* gsStr, int gsStrSize);
int wopSP_loadLastWonSE(const char* storyname, const char* savename);
qboolean wopSP_WonSEalreadySaved(const char* storyname, const char* savename,int WonSE);
void wopSP_addWonSEtoGameState(const char* storyname, const char* savename, int wonSE);

#endif

