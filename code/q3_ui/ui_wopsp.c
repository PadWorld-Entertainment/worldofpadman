

#include "ui_local.h"
#include "../game/wopg_spstoryfiles.h"
#include "../game/wopg_sphandling.h"

#define WSPM_START0		"menu/single/start0.tga"
#define WSPM_START1		"menu/single/start1.tga"

#define WSPM_CONTINUE0	"menu/single/continue0.tga"
#define WSPM_CONTINUE1	"menu/single/continue1.tga"

#define WSPM_EXIT0		"menu/single/exit0.tga"
#define WSPM_EXIT1		"menu/single/exit1.tga"

#define MAX_STORYELEMENTS	32
#define MAX_CURRENTSTORYSTR	128

#define DEFAULT_STORY "shortTest"

void WoPSPContinue_Init(void);

enum {
// main singleplayer menu
ID_STARTSTORY = 10,
ID_CONTINUESTORY,

// continue menu
ID_FORWARD,
ID_START,
ID_BACKWARD,
ID_EXIT // used in both
};

typedef struct menuStoryElement_s {
	wop_StoryElement_t se;

	struct menuStoryElement_s* nextWin;
	struct menuStoryElement_s* nextLoos;

	float	x, y; // draw pos

	qboolean alreadyWon;
} menuStoryElement_t;

typedef struct {
	menuframework_s	menu;

	menubitmap_s startStoryButton;
	menubitmap_s continueStoryButton;
	menubitmap_s exitButton;

	char currentStory[MAX_CURRENTSTORYSTR];
	menuStoryElement_t storyElements[MAX_STORYELEMENTS];
	int	lastAvailableSE; // ~next after last win ...
	int numMenuElements;
} wopSPmenu_t;

wopSPmenu_t wopSPmenu;

#if 0
static void DrawSEConnection(menuStoryElement_t* s, menuStoryElement_t* e, qboolean win) {
	float* color;

	if(win && s->nextWin != e)
		return;
	if(!win && s->nextLoos != e)
		return;

	color = win?colorGreen:colorRed;

	if(e->y<=s->y) {
		float dy = (e->y-s->y);

		UI_FillRect(s->x+(win?75:25),s->y+100,2,10,color);
		UI_FillRect(s->x+(win?75:27),s->y+110,(win?35+2:-35-2),2,color);

		UI_FillRect(s->x+(win?110:-10),s->y+110,2,dy>0?dy+120:dy-120,color);

		UI_FillRect(e->x+(win?75:27),e->y-10,(win?35+2:-35-2),2,color);
		UI_FillRect(e->x+(win?75:25),e->y-10,2,10,color);
	}
	else {
		DrawLine(s->x+(win?75:25),s->y+100,e->x+(win?75:25),e->y,2,color);
	}
}

static void DrawStoryElement(menuStoryElement_t* mse) {
	UI_DrawRect(mse->x,mse->y,100,100,colorYellow,1);
	UI_DrawString(mse->x+2,mse->y+2,mse->se.name,UI_SMALLFONT|UI_LEFT,colorWhite);
	UI_DrawString(mse->x+2,mse->y+20,mse->se.map,UI_SMALLFONT|UI_LEFT,colorCyan);
	if(mse->alreadyWon)
		UI_DrawString(mse->x+2,mse->y+40,"WON!",UI_SMALLFONT|UI_LEFT,colorGreen);

	if(mse->nextWin)
		DrawSEConnection(mse,mse->nextWin,qtrue);
	if(mse->nextLoos)
		DrawSEConnection(mse,mse->nextLoos,qfalse);
}
#endif

static void WoPSPMenu_Draw( void ) {

	Menu_Draw( &wopSPmenu.menu );
}

static void WoPSPMenu_Event(void *ptr, int event)
{
	if(event != QM_ACTIVATED)
		return;

	switch( ((menucommon_s*)ptr)->id )
	{
	case ID_STARTSTORY:
		//TODO: add menu to select ~skill~
		//FIXME: show real intro video ...
		//TODO: clear old save game ... ask player?
//		trap_Cmd_ExecuteText( EXEC_APPEND, va("wopSP_startStory %s\n",wopSPmenu.currentStory));
		//trap_Cmd_ExecuteText( EXEC_APPEND, va("wop_mediaview \"video:idlogo.roq\" \"wopSP_startStory %s\n\"\n",wopSPmenu.currentStory));
		LaunchMediaViewMenu("video:idlogo.roq",va("wopSP_startStory %s\n",wopSPmenu.currentStory));
		break;
	case ID_CONTINUESTORY:
		WoPSPContinue_Init();
		break;
	case ID_EXIT:
		UI_PopMenu();
		break;
	}
}

void WoPSPMenu_LoadStory(const char* newStory) {
	int i;

	Q_strncpyz(wopSPmenu.currentStory,newStory,sizeof(wopSPmenu.currentStory));
	wopSPmenu.lastAvailableSE = 0;

	if(!wopSP_openStory(wopSPmenu.currentStory)) {
		wop_StoryElement_t tmpE;

		wopSPmenu.numMenuElements = 0;
		Com_Printf("opend test Storyfile\n");
		while( wopSP_loadNextStoryElement( &tmpE ) >= 0 ) {
			if(wopSPmenu.numMenuElements<MAX_STORYELEMENTS) {
				wopSPmenu.storyElements[wopSPmenu.numMenuElements].se = tmpE;
				++wopSPmenu.numMenuElements;
			}
			else {
				Com_Printf("[WoPSPmenu] to many storyElements(current limit %d)\n",MAX_STORYELEMENTS);
			}

			Com_Printf("\"%s\"(%d): map=%s\n",tmpE.name,tmpE.elementID,tmpE.map);
		}
		wopSP_closeFile();
		Com_Printf("closed test Storyfile\n");
	}

	if(wopSPmenu.numMenuElements>0) {
		char savename[MAX_CVAR_VALUE_STRING];
		char gsBuffer[1024];

		trap_Cvar_VariableStringBuffer(WOPSP_SAVENAME_CVAR, savename, sizeof(savename) );
		if( !strlen(savename) )
			Q_strncpyz(savename, "default", sizeof(savename));

		if(wopSP_loadGameState(wopSPmenu.currentStory,savename,gsBuffer,sizeof(gsBuffer))) {
			char* parsePtr = gsBuffer;
			char* token;
			while((token=COM_Parse(&parsePtr)) && token[0]!='\0') {
				int seNr = atoi(token);

				if(seNr>=0 && seNr<wopSPmenu.numMenuElements)
					wopSPmenu.storyElements[seNr].alreadyWon = qtrue;
			}
		}
	}

	for(i=0;i<wopSPmenu.numMenuElements;++i) {
		int j;

		for(j=0;j<wopSPmenu.numMenuElements;++j) {
			if(wopSP_compareWithNextString(wopSPmenu.storyElements[i].se.nextAfterWin,&wopSPmenu.storyElements[j].se))
				wopSPmenu.storyElements[i].nextWin = &wopSPmenu.storyElements[j];
			if(wopSP_compareWithNextString(wopSPmenu.storyElements[i].se.nextAfterLoos,&wopSPmenu.storyElements[j].se))
				wopSPmenu.storyElements[i].nextLoos = &wopSPmenu.storyElements[j];
		}

		if(!wopSPmenu.storyElements[i].nextWin && (i+1)<wopSPmenu.numMenuElements)
			wopSPmenu.storyElements[i].nextWin = &wopSPmenu.storyElements[i+1];
		if(!wopSPmenu.storyElements[i].nextLoos)
			wopSPmenu.storyElements[i].nextLoos = &wopSPmenu.storyElements[i];


		if(wopSPmenu.storyElements[i].alreadyWon
				&& wopSPmenu.storyElements[i].nextWin
				&& wopSPmenu.storyElements[i].nextWin->se.elementID > wopSPmenu.lastAvailableSE)
			wopSPmenu.lastAvailableSE = wopSPmenu.storyElements[i].nextWin->se.elementID;
	}

	for(i=0;i<wopSPmenu.numMenuElements;++i) {
		wopSPmenu.storyElements[i].x = 100;
		wopSPmenu.storyElements[i].y = 20+130*i;
	}
}

void WoPSPMenu_Init() {

	memset(&wopSPmenu,0,sizeof(wopSPmenu_t));

	WoPSPMenu_LoadStory(DEFAULT_STORY); //TODO: move to continue menu (and rework a bit ;) )

	wopSPmenu.menu.bgparts			= BGP_SINGLEMENU;
	wopSPmenu.menu.fullscreen		= qtrue;
	wopSPmenu.menu.draw				= WoPSPMenu_Draw;
	wopSPmenu.menu.noPushSelect		= qtrue;

	wopSPmenu.continueStoryButton.generic.type		= MTYPE_BITMAP;
	wopSPmenu.continueStoryButton.generic.name		= WSPM_CONTINUE0;
	wopSPmenu.continueStoryButton.generic.flags		= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    wopSPmenu.continueStoryButton.generic.x			= 466;
	wopSPmenu.continueStoryButton.generic.y			= 348;
	wopSPmenu.continueStoryButton.generic.id		= ID_CONTINUESTORY;
	wopSPmenu.continueStoryButton.generic.callback	= WoPSPMenu_Event;
    wopSPmenu.continueStoryButton.width				= 133;
	wopSPmenu.continueStoryButton.height			= 42;
	wopSPmenu.continueStoryButton.focuspic			= WSPM_CONTINUE1;
	wopSPmenu.continueStoryButton.focuspicinstead	= qtrue;
	Menu_AddItem(&wopSPmenu.menu, &wopSPmenu.continueStoryButton);

	wopSPmenu.startStoryButton.generic.type		= MTYPE_BITMAP;
	wopSPmenu.startStoryButton.generic.name		= WSPM_START0;
	wopSPmenu.startStoryButton.generic.flags	= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    wopSPmenu.startStoryButton.generic.x		= 500;
	wopSPmenu.startStoryButton.generic.y		= 406;
	wopSPmenu.startStoryButton.generic.id		= ID_STARTSTORY;
	wopSPmenu.startStoryButton.generic.callback	= WoPSPMenu_Event;
    wopSPmenu.startStoryButton.width			= 111;
	wopSPmenu.startStoryButton.height			= 45;
	wopSPmenu.startStoryButton.focuspic			= WSPM_START1;
	wopSPmenu.startStoryButton.focuspicinstead	= qtrue;
	Menu_AddItem(&wopSPmenu.menu, &wopSPmenu.startStoryButton);

	wopSPmenu.exitButton.generic.type			= MTYPE_BITMAP;
	wopSPmenu.exitButton.generic.name			= WSPM_EXIT0;
	wopSPmenu.exitButton.generic.flags			= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    wopSPmenu.exitButton.generic.x				= 19;
	wopSPmenu.exitButton.generic.y				= 377;
	wopSPmenu.exitButton.generic.id				= ID_EXIT;
	wopSPmenu.exitButton.generic.callback		= WoPSPMenu_Event;
    wopSPmenu.exitButton.width					= 116;
	wopSPmenu.exitButton.height					= 81;
	wopSPmenu.exitButton.focuspic				= WSPM_EXIT1;
	wopSPmenu.exitButton.focuspicinstead		= qtrue;
	Menu_AddItem(&wopSPmenu.menu, &wopSPmenu.exitButton);

	//TODO: add something like BACK?

	UI_PushMenu ( &wopSPmenu.menu );
}


/*
####################### ####################### #######################

  wopSPcontinue menu

####################### ####################### #######################
*/

#define WSPC_FORWARD0	"menu/single/continue/forward0.tga"
#define WSPC_FORWARD1	"menu/single/continue/forward1.tga"

#define WSPC_BACKWARD0	"menu/single/continue/backward0.tga"
#define WSPC_BACKWARD1	"menu/single/continue/backward1.tga"

#define WSPC_START0		"menu/single/continue/start0.tga"
#define WSPC_START1		"menu/single/continue/start1.tga"

#define WSPC_EXIT0		"menu/single/continue/exit0.tga"
#define WSPC_EXIT1		"menu/single/continue/exit1.tga"

typedef struct {
	menuframework_s	menu;

	menubitmap_s	forwardButton;
	menubitmap_s	backwardButton;
	menubitmap_s	startButton;
	menubitmap_s	exitButton;

	int				centerSE;

	qhandle_t		bg;
	qhandle_t		gateLeft;
	qhandle_t		gateRight;
	int				menuStartTime;
	qboolean		drawFirstFrame;
} wopSPcontinue_t;

wopSPcontinue_t wopSPcontinue;

static void DrawSPContinueLevelShot(int storyelement, int monitor) {
	qhandle_t	shader = 0;
	qboolean	won = qfalse;
	qboolean	available = qfalse;

	if(storyelement>=0 && storyelement<wopSPmenu.numMenuElements) {
		if(monitor==0 || monitor==2)
			shader = trap_R_RegisterShaderNoMip(va("levelshots/%sA",wopSPmenu.storyElements[storyelement].se.map));
		else if(monitor==1)
			shader = trap_R_RegisterShaderNoMip(va("levelshots/%s",wopSPmenu.storyElements[storyelement].se.map));

		won = wopSPmenu.storyElements[storyelement].alreadyWon;

		available = (storyelement <= wopSPmenu.lastAvailableSE);
	}

	if(shader) {
		switch(monitor) {
		case 0:
			UI_DrawHandlePic( 40,50,130,100,shader);
			if(won) UI_DrawStringNS(160,60,"WON!",UI_RIGHT,12,colorGreen);
			if(!available) UI_FillRect( 40,50,130,100, colorTBlack66);
			break;
		case 1:
			UI_DrawHandlePic(205,35,230,175,shader);
			if(won) UI_DrawStringNS(425,45,"WON!",UI_RIGHT,14,colorGreen);
			if(!available) UI_FillRect(205,35,230,175, colorTBlack66);
			break;
		case 2:
			UI_DrawHandlePic(470,50,130,100,shader);
			if(won) UI_DrawStringNS(590,60,"WON!",UI_RIGHT,12,colorGreen);
			if(!available) UI_FillRect(470,50,130,100, colorTBlack66);
			break;
		}
	}
}

// FIXME: In case this is really used, apply GAMETYPE_NAME() macros
static const char* WSPC_gametype_strs[] = {
	"FREE FOR ALL",			//GT_FFA
	"TOURNAMENT",			//GT_TOURNAMENT
	"SP???",				//GT_SINGLE_PLAYER
	"SPRAY YOUR COLOR FFA",	//GT_SPRAYFFA
	"LAST PAD STANDING",	//GT_LPS

	"TEAM DEATHMATCH",		//GT_TEAM
	"CAPTURE THE LOLLY",	//GT_CTF
	"SPRAY YOUR COLOR TEAMPLAY",//GT_SPRAY
	"BIG BALLOON"			//GT_BALLOON
};
static void WoPSPContinue_Draw( void ) {
	int gt =-1;

	if(wopSPcontinue.drawFirstFrame) { // draw the closed door, before loading the lvlshots
		UI_DrawHandlePic( 505-SCREEN_WIDTH-30, 0, SCREEN_WIDTH+50, SCREEN_HEIGHT, wopSPcontinue.gateLeft );
		UI_DrawHandlePic( 180, 0, SCREEN_WIDTH, SCREEN_HEIGHT, wopSPcontinue.gateRight );

		wopSPcontinue.drawFirstFrame = qfalse;
		return;
	}

	//TODO! monitor se's like they are in the save-chain
	DrawSPContinueLevelShot(wopSPcontinue.centerSE-1, 0);
	DrawSPContinueLevelShot(wopSPcontinue.centerSE, 1);
	DrawSPContinueLevelShot(wopSPcontinue.centerSE+1, 2);

	if(wopSPcontinue.centerSE>=0 && wopSPcontinue.centerSE<wopSPmenu.numMenuElements)
		gt = wopSPmenu.storyElements[wopSPcontinue.centerSE].se.gametype;
	if(gt>=0 && gt<GT_MAX_GAME_TYPE)
		UI_DrawStringNS(235,237,WSPC_gametype_strs[gt],UI_LEFT,18,colorGreen);

	UI_DrawHandlePic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, wopSPcontinue.bg );

/*	for(i=0;i<wopSPmenu.numMenuElements;++i) {
		DrawStoryElement(&wopSPmenu.storyElements[i]);
//		UI_DrawString(10,10+i*20,wopSPmenu.storyElements[i].se.name,UI_SMALLFONT|UI_LEFT,colorWhite);
//		UI_DrawString(300,10+i*20,wopSPmenu.storyElements[i].se.map,UI_SMALLFONT|UI_LEFT,colorCyan);
	}
*/
	Menu_Draw( &wopSPcontinue.menu );

	if(wopSPcontinue.menuStartTime == 0)
		wopSPcontinue.menuStartTime = uis.realtime;

	if(wopSPcontinue.menuStartTime + 2000 > uis.realtime) {
		float dx = (uis.realtime-wopSPcontinue.menuStartTime-500)*0.5f;

		if(dx<0.0f) dx=0.0f;

		//0 -> 505
		UI_DrawHandlePic( 505-SCREEN_WIDTH-30-dx, 0, SCREEN_WIDTH+50, SCREEN_HEIGHT, wopSPcontinue.gateLeft );
		UI_DrawHandlePic( 180+dx, 0, SCREEN_WIDTH, SCREEN_HEIGHT, wopSPcontinue.gateRight );

		return;
	}
}

static void WoPSPContinue_Event(void *ptr, int event)
{
	if(event != QM_ACTIVATED)
		return;

	switch( ((menucommon_s*)ptr)->id )
	{
	case ID_FORWARD:
//		if(wopSPcontinue.centerSE<wopSPmenu.numMenuElements-1)
		if((wopSPcontinue.centerSE+1) <= wopSPmenu.lastAvailableSE)
			++wopSPcontinue.centerSE;
		break;
	case ID_START:
//old:		trap_Cmd_ExecuteText( EXEC_APPEND, va("wopSP_loadStory %s\n",wopSPmenu.currentStory));
		trap_Cmd_ExecuteText( EXEC_APPEND, va("wopSP_startStory %s #%d\n",wopSPmenu.currentStory,wopSPcontinue.centerSE));
		break;
	case ID_BACKWARD:
		if(wopSPcontinue.centerSE>0)
			--wopSPcontinue.centerSE;
		break;
	case ID_EXIT:
		UI_PopMenu();
		break;
	}
}
void WoPSPContinue_Init(void) {

	memset(&wopSPcontinue,0,sizeof(wopSPcontinue_t));

	wopSPcontinue.centerSE = wopSPmenu.lastAvailableSE;

	wopSPcontinue.bg					= trap_R_RegisterShaderNoMip("menu/single/continue/background");
	wopSPcontinue.gateLeft				= trap_R_RegisterShaderNoMip("menu/single/continue/gate_l.tga");
	wopSPcontinue.gateRight				= trap_R_RegisterShaderNoMip("menu/single/continue/gate_r.tga");
	wopSPcontinue.menu.fullscreen		= qtrue;
	wopSPcontinue.menu.draw				= WoPSPContinue_Draw;
	wopSPcontinue.menu.noPushSelect		= qtrue;

	wopSPcontinue.backwardButton.generic.type	= MTYPE_BITMAP;
	wopSPcontinue.backwardButton.generic.name	= WSPC_BACKWARD0;
	wopSPcontinue.backwardButton.generic.flags	= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    wopSPcontinue.backwardButton.generic.x		= 254;
	wopSPcontinue.backwardButton.generic.y		= 323;
	wopSPcontinue.backwardButton.generic.id		= ID_BACKWARD;
	wopSPcontinue.backwardButton.generic.callback= WoPSPContinue_Event;
    wopSPcontinue.backwardButton.width			= 42;
	wopSPcontinue.backwardButton.height			= 35;
	wopSPcontinue.backwardButton.focuspic		= WSPC_BACKWARD1;
	wopSPcontinue.backwardButton.focuspicinstead= qtrue;
	Menu_AddItem(&wopSPcontinue.menu, &wopSPcontinue.backwardButton);

	wopSPcontinue.startButton.generic.type		= MTYPE_BITMAP;
	wopSPcontinue.startButton.generic.name		= WSPC_START0;
	wopSPcontinue.startButton.generic.flags		= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    wopSPcontinue.startButton.generic.x			= 309;
	wopSPcontinue.startButton.generic.y			= 323;
	wopSPcontinue.startButton.generic.id		= ID_START;
	wopSPcontinue.startButton.generic.callback	= WoPSPContinue_Event;
    wopSPcontinue.startButton.width				= 29;
	wopSPcontinue.startButton.height			= 35;
	wopSPcontinue.startButton.focuspic			= WSPC_START1;
	wopSPcontinue.startButton.focuspicinstead	= qtrue;
	Menu_AddItem(&wopSPcontinue.menu, &wopSPcontinue.startButton);

	wopSPcontinue.forwardButton.generic.type	= MTYPE_BITMAP;
	wopSPcontinue.forwardButton.generic.name	= WSPC_FORWARD0;
	wopSPcontinue.forwardButton.generic.flags	= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    wopSPcontinue.forwardButton.generic.x		= 351;
	wopSPcontinue.forwardButton.generic.y		= 323;
	wopSPcontinue.forwardButton.generic.id		= ID_FORWARD;
	wopSPcontinue.forwardButton.generic.callback= WoPSPContinue_Event;
    wopSPcontinue.forwardButton.width			= 42;
	wopSPcontinue.forwardButton.height			= 35;
	wopSPcontinue.forwardButton.focuspic		= WSPC_FORWARD1;
	wopSPcontinue.forwardButton.focuspicinstead	= qtrue;
	Menu_AddItem(&wopSPcontinue.menu, &wopSPcontinue.forwardButton);

	wopSPcontinue.exitButton.generic.type		= MTYPE_BITMAP;
	wopSPcontinue.exitButton.generic.name		= WSPC_EXIT0;
	wopSPcontinue.exitButton.generic.flags		= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    wopSPcontinue.exitButton.generic.x			= 563;
	wopSPcontinue.exitButton.generic.y			= 186;
	wopSPcontinue.exitButton.generic.id			= ID_EXIT;
	wopSPcontinue.exitButton.generic.callback	= WoPSPContinue_Event;
    wopSPcontinue.exitButton.width				= 30;
	wopSPcontinue.exitButton.height				= 30;
	wopSPcontinue.exitButton.focuspic			= WSPC_EXIT1;
	wopSPcontinue.exitButton.focuspicinstead	= qtrue;
	Menu_AddItem(&wopSPcontinue.menu, &wopSPcontinue.exitButton);

	wopSPcontinue.drawFirstFrame = qtrue;
	UI_PushMenu ( &wopSPcontinue.menu );

}



