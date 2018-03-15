// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

GAME OPTIONS MENU

=======================================================================
*/


#include "ui_local.h"

#define ART_BACK0				"menu/BtnBack0"
#define ART_BACK1				"menu/BtnBack1"

#define PREFERENCES_X_POS		534

#define ID_CROSSHAIR			127
#define ID_SIMPLEITEMS			128
#define ID_HIGHQUALITYSKY		129
#define ID_FLARES				130
#define ID_WALLMARKS			131
#define ID_DYNAMICLIGHTS		132
#define ID_IDENTIFYTARGET		133
#define ID_SYNCEVERYFRAME		134
#define ID_FORCEMODEL			135
#define ID_DRAWTEAMOVERLAY		136
#define ID_ALLOWDOWNLOAD		137
#define ID_BACK					138

#define	ID_FFAHUD				139
#define ID_CONNOTIFY			140
#define	ID_TIMELEFT				141

#define	ID_TIMER				142
#define	ID_FPS					143
#define	ID_UPS					144
#define	ID_REALTIME				145
#define ID_MORE					146

#define ID_GLOWMODEL			147

#define	NUM_CROSSHAIRS			10


typedef struct {
	menuframework_s		menu;

	menulist_s			crosshair;
	menuradiobutton_s	simpleitems;
	menuradiobutton_s	flares;
	menuradiobutton_s	wallmarks;
	menuradiobutton_s	dynamiclights;
	menuradiobutton_s	identifytarget;
	menuradiobutton_s	highqualitysky;
	menuradiobutton_s	synceveryframe;
	menuradiobutton_s	forcemodel;
	menuradiobutton_s	glowmodel;
	menuradiobutton_s	drawteamoverlay;

	menulist_s			ffahud;
	menulist_s			con_notifytime;
	menuradiobutton_s	timeleft;

	menuradiobutton_s	timer;
	menuradiobutton_s	fps;
	menuradiobutton_s	ups;
	menuradiobutton_s	realtime;

	menuradiobutton_s	allowdownload;
	menutext_s			more;
	menubitmap_s		back;

	qhandle_t			crosshairShader[NUM_CROSSHAIRS];
} preferences_t;

static preferences_t s_preferences;
static qboolean page;

static menucommon_s *page1_options[] =
{
	(menucommon_s *)&s_preferences.crosshair,
	(menucommon_s *)&s_preferences.drawteamoverlay,
	(menucommon_s *)&s_preferences.ffahud,
	(menucommon_s *)&s_preferences.con_notifytime,
	(menucommon_s *)&s_preferences.timer,
	(menucommon_s *)&s_preferences.timeleft,
	(menucommon_s *)&s_preferences.realtime,
	(menucommon_s *)&s_preferences.ups,
	(menucommon_s *)&s_preferences.fps,     
	NULL
};

static menucommon_s *page2_options[] =
{
	(menucommon_s *)&s_preferences.forcemodel,
	(menucommon_s *)&s_preferences.glowmodel,
	(menucommon_s *)&s_preferences.simpleitems,
	(menucommon_s *)&s_preferences.wallmarks,
	(menucommon_s *)&s_preferences.dynamiclights,
	(menucommon_s *)&s_preferences.identifytarget,
	(menucommon_s *)&s_preferences.highqualitysky,
	(menucommon_s *)&s_preferences.flares,
	(menucommon_s *)&s_preferences.synceveryframe,    
	NULL
};

static const char *ffahud_names[] =
{
	"Black",
	"Red",
	"Blue",
	"Green",
	"Chrome",
	"Whitemetal",
	"Rust",
	"Flower",
	"Wood",
	"Airforce",
	0
};

static const char *con_notifytime_strs[] =
{
	"short",
	"long",
	"short (icon)",
	"long (icon)",
	0
};

static void Preferences_SetMenuItems( void )
{
	int notify;

	notify=UI_GetCvarInt("con_notifytime");
	if(notify>0 && notify<=3)
		s_preferences.con_notifytime.curvalue = 0;
	else if(notify>3)
		s_preferences.con_notifytime.curvalue = 1;
	else if(notify<0 && notify>=-5)
		s_preferences.con_notifytime.curvalue = 2;
	else if(notify<-5)
		s_preferences.con_notifytime.curvalue = 3;

	s_preferences.ffahud.curvalue			= Com_Clamp( 0, 9, trap_Cvar_VariableValue( "cg_wopFFAhud" ) );
	s_preferences.timeleft.curvalue			= Com_Clamp( 0, 1, trap_Cvar_VariableValue("cg_drawTimeLeft") );

	s_preferences.timer.curvalue			= Com_Clamp( 0, 1, trap_Cvar_VariableValue("cg_drawTimer") );
	s_preferences.fps.curvalue				= Com_Clamp( 0, 1, trap_Cvar_VariableValue("cg_drawFPS") );
	s_preferences.ups.curvalue				= Com_Clamp( 0, 1, trap_Cvar_VariableValue("cg_drawups") );
	s_preferences.realtime.curvalue			= Com_Clamp( 0, 1, trap_Cvar_VariableValue("cg_drawRealTime") );

	s_preferences.crosshair.curvalue		= (int)trap_Cvar_VariableValue( "cg_drawCrosshair" ) % NUM_CROSSHAIRS;
	s_preferences.simpleitems.curvalue		= trap_Cvar_VariableValue( "cg_simpleItems" ) != 0;
	s_preferences.flares.curvalue			= trap_Cvar_VariableValue( "r_flares" ) != 0;
	s_preferences.wallmarks.curvalue		= trap_Cvar_VariableValue( "cg_marks" ) != 0;
	s_preferences.identifytarget.curvalue	= trap_Cvar_VariableValue( "cg_drawCrosshairNames" ) != 0;
	s_preferences.dynamiclights.curvalue	= trap_Cvar_VariableValue( "r_dynamiclight" ) != 0;
	s_preferences.highqualitysky.curvalue	= trap_Cvar_VariableValue ( "r_fastsky" ) == 0;
	s_preferences.synceveryframe.curvalue	= trap_Cvar_VariableValue( "r_finish" ) != 0;
	s_preferences.forcemodel.curvalue		= ( trap_Cvar_VariableValue( "cg_forcemodel" ) != 0.0f );
	s_preferences.glowmodel.curvalue		= trap_Cvar_VariableValue( "cg_glowModel" ) != 0; // cg_glowModelTeam..
	s_preferences.drawteamoverlay.curvalue	= trap_Cvar_VariableValue( "cg_drawTeamOverlay" ) != 0;
	s_preferences.allowdownload.curvalue	= trap_Cvar_VariableValue( "cl_allowDownload" ) != 0;
}

static void SwitchPage()
{
	menucommon_s	**hide_options;
	menucommon_s	**show_options;
	menucommon_s	*option;
	int j;

	page = !page;

	if(!page)
	{
		show_options = page1_options;
		hide_options = page2_options;		
	}
	else
	{
		show_options = page2_options;
		hide_options = page1_options;		
	}

	
	for( j = 0;  (option = hide_options[j]) ; j++ ) {
		option->flags |= (QMF_HIDDEN|QMF_INACTIVE);
	}

	for( j = 0;  (option = show_options[j]) ; j++ ) {
		option->flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}

}

static void Preferences_Event( void* ptr, int notification ) {
	if( notification != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_CROSSHAIR:
		s_preferences.crosshair.curvalue++;
		if( s_preferences.crosshair.curvalue == NUM_CROSSHAIRS ) {
			s_preferences.crosshair.curvalue = 0;
		}
		trap_Cvar_SetValue( "cg_drawCrosshair", s_preferences.crosshair.curvalue );
		break;

	case ID_SIMPLEITEMS:
		trap_Cvar_SetValue( "cg_simpleItems", s_preferences.simpleitems.curvalue );
		break;

	case ID_HIGHQUALITYSKY:
		trap_Cvar_SetValue( "r_fastsky", !s_preferences.highqualitysky.curvalue );
		break;

	case ID_FLARES:
		trap_Cvar_SetValue( "r_flares", s_preferences.flares.curvalue );
		break;

	case ID_WALLMARKS:
		trap_Cvar_SetValue( "cg_marks", s_preferences.wallmarks.curvalue );
		break;

	case ID_DYNAMICLIGHTS:
		trap_Cvar_SetValue( "r_dynamiclight", s_preferences.dynamiclights.curvalue );
		break;		

	case ID_IDENTIFYTARGET:
		trap_Cvar_SetValue( "cg_drawCrosshairNames", s_preferences.identifytarget.curvalue );
		break;

	case ID_SYNCEVERYFRAME:
		trap_Cvar_SetValue( "r_finish", s_preferences.synceveryframe.curvalue );
		break;

	case ID_FORCEMODEL:
		trap_Cvar_SetValue( "cg_forcemodel", s_preferences.forcemodel.curvalue );
		break;

	case ID_GLOWMODEL:
		if ( s_preferences.glowmodel.curvalue == 0 ) {
			trap_Cvar_Set( "cg_glowModel", "" );
		}
		else {
			trap_Cvar_SetValue( "cg_glowModel", 2 ); // default green enemies only
		}
		break;

	case ID_DRAWTEAMOVERLAY:
		trap_Cvar_SetValue( "cg_drawTeamOverlay", s_preferences.drawteamoverlay.curvalue );
		break;

	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_FFAHUD:
		trap_Cvar_SetValue( "cg_wopFFAhud", s_preferences.ffahud.curvalue );
		break;

	case ID_TIMELEFT:
		trap_Cvar_SetValue( "cg_drawTimeLeft", s_preferences.timeleft.curvalue );
		break;

	case ID_TIMER:
		trap_Cvar_SetValue( "cg_drawTimer", s_preferences.timer.curvalue );
		break;
	case ID_FPS:
		trap_Cvar_SetValue( "cg_drawFPS", s_preferences.fps.curvalue );
		break;
	case ID_UPS:
		trap_Cvar_SetValue( "cg_drawUPS", s_preferences.ups.curvalue );
		break;
	case ID_REALTIME:
		trap_Cvar_SetValue( "cg_drawRealTime", s_preferences.realtime.curvalue );
		break;

	case ID_CONNOTIFY:
		switch(s_preferences.con_notifytime.curvalue)
		{
		case 0:
			trap_Cvar_Set( "con_notifytime", "3" );
			break;
		case 1:
			trap_Cvar_Set( "con_notifytime", "8" );
			break;
		case 2:
			trap_Cvar_Set( "con_notifytime", "-5" );
			break;
		case 3:
			trap_Cvar_Set( "con_notifytime", "-10" );
			break;
		}
		break;
	case ID_MORE:
		SwitchPage();
		break;
	}
}


/*
=================
Crosshair_Draw
=================
*/
static void Crosshair_Draw( void *self ) {
	menulist_s	*s;
	float		*color;
	int			x, y;
	int			style;
	qboolean	focus;

	s = (menulist_s *)self;
	x = s->generic.x;
	y =	s->generic.y;

	style = UI_SMALLFONT;
	focus = (s->generic.parent->cursor == s->generic.menuPosition);

	if ( s->generic.flags & QMF_GRAYED )
		color = text_color_disabled;
	else if ( focus )
	{
		color = text_color_highlight;
		style |= UI_PULSE;
	}
	else if ( s->generic.flags & QMF_BLINK )
	{
		color = text_color_highlight;
		style |= UI_BLINK;
	}
	else
		color = text_color_normal;

	if ( focus )
	{
		// draw cursor
		UI_FillRect( s->generic.left, s->generic.top, s->generic.right-s->generic.left+1, s->generic.bottom-s->generic.top+1, listbar_color ); 
		UI_DrawChar( x, y, 13, UI_CENTER|UI_BLINK|UI_SMALLFONT, color);
	}

	UI_DrawString( x - SMALLCHAR_WIDTH, y, s->generic.name, style|UI_RIGHT, color );
	if( !s->curvalue ) {
		return;
	}
	UI_DrawHandlePic( x + SMALLCHAR_WIDTH, y - 4, 24, 24, s_preferences.crosshairShader[s->curvalue] );
}


static void Preferences_MenuInit( void ) {
	int				y;
	int				yp1, yp2;

	memset( &s_preferences, 0 ,sizeof(preferences_t) );

	Preferences_Cache();

	s_preferences.menu.wrapAround = qtrue;
	s_preferences.menu.fullscreen = qtrue;
	s_preferences.menu.bgparts		= BGP_GAMEOPTIONS|BGP_SIMPLEBG;

	y = 156;
	// page 1
	s_preferences.crosshair.generic.type		= MTYPE_TEXT;
	s_preferences.crosshair.generic.flags		= QMF_SMALLFONT|QMF_NODEFAULTINIT|QMF_OWNERDRAW;
	s_preferences.crosshair.generic.x			= PREFERENCES_X_POS;
	s_preferences.crosshair.generic.y			= y;
	s_preferences.crosshair.generic.name		= "Crosshair:";
	s_preferences.crosshair.generic.callback	= Preferences_Event;
	s_preferences.crosshair.generic.ownerdraw	= Crosshair_Draw;
	s_preferences.crosshair.generic.id			= ID_CROSSHAIR;
	s_preferences.crosshair.generic.top			= y - 4;
	s_preferences.crosshair.generic.bottom		= y + 20;
	s_preferences.crosshair.generic.left		= PREFERENCES_X_POS - ( ( strlen(s_preferences.crosshair.generic.name) + 1 ) * SMALLCHAR_WIDTH );
	s_preferences.crosshair.generic.right		= PREFERENCES_X_POS + 48;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.drawteamoverlay.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.drawteamoverlay.generic.name	   = "Draw Team Overlay:";
	s_preferences.drawteamoverlay.generic.flags	   = QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.drawteamoverlay.generic.callback = Preferences_Event;
	s_preferences.drawteamoverlay.generic.id       = ID_DRAWTEAMOVERLAY;
	s_preferences.drawteamoverlay.generic.x	       = PREFERENCES_X_POS;
	s_preferences.drawteamoverlay.generic.y	       = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.ffahud.generic.type		= MTYPE_SPINCONTROL;
	s_preferences.ffahud.generic.name		= "FFA Hud:";
	s_preferences.ffahud.generic.flags		= QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.ffahud.generic.callback	= Preferences_Event;
	s_preferences.ffahud.generic.id			= ID_FFAHUD;
	s_preferences.ffahud.generic.x			= PREFERENCES_X_POS;
	s_preferences.ffahud.generic.y			= y;
	s_preferences.ffahud.itemnames			= ffahud_names;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.con_notifytime.generic.type		= MTYPE_SPINCONTROL;
	s_preferences.con_notifytime.generic.name		= "Chat:";
	s_preferences.con_notifytime.generic.flags		= QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.con_notifytime.generic.callback	= Preferences_Event;
	s_preferences.con_notifytime.generic.id			= ID_CONNOTIFY;
	s_preferences.con_notifytime.generic.x			= PREFERENCES_X_POS;
	s_preferences.con_notifytime.generic.y			= y;
	s_preferences.con_notifytime.itemnames			= con_notifytime_strs;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.timer.generic.type		= MTYPE_RADIOBUTTON;
	s_preferences.timer.generic.name		= "Display Timer:";
	s_preferences.timer.generic.flags		= QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.timer.generic.callback	= Preferences_Event;
	s_preferences.timer.generic.id			= ID_TIMER;
	s_preferences.timer.generic.x			= PREFERENCES_X_POS;
	s_preferences.timer.generic.y			= y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.timeleft.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.timeleft.generic.name	    = "Display Time Left:";
	s_preferences.timeleft.generic.flags	= QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.timeleft.generic.callback = Preferences_Event;
	s_preferences.timeleft.generic.id       = ID_TIMELEFT;
	s_preferences.timeleft.generic.x	    = PREFERENCES_X_POS;
	s_preferences.timeleft.generic.y	    = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.realtime.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.realtime.generic.name	    = "Display Real Time:";
	s_preferences.realtime.generic.flags	= QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.realtime.generic.callback = Preferences_Event;
	s_preferences.realtime.generic.id       = ID_REALTIME;
	s_preferences.realtime.generic.x	    = PREFERENCES_X_POS;
	s_preferences.realtime.generic.y	    = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.ups.generic.type			= MTYPE_RADIOBUTTON;
	s_preferences.ups.generic.name			= "Display Velocity:";
	s_preferences.ups.generic.flags			= QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.ups.generic.callback		= Preferences_Event;
	s_preferences.ups.generic.id			= ID_UPS;
	s_preferences.ups.generic.x				= PREFERENCES_X_POS;
	s_preferences.ups.generic.y				= y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.fps.generic.type			= MTYPE_RADIOBUTTON;
	s_preferences.fps.generic.name			= "Display FPS:";
	s_preferences.fps.generic.flags			= QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.fps.generic.callback		= Preferences_Event;
	s_preferences.fps.generic.id			= ID_FPS;
	s_preferences.fps.generic.x				= PREFERENCES_X_POS;
	s_preferences.fps.generic.y				= y;

	yp1 = y + BIGCHAR_HEIGHT + 2;

	// page 2
	y = 156;
	s_preferences.forcemodel.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.forcemodel.generic.name	  = "Force Player Model:";
	s_preferences.forcemodel.generic.flags	  = QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.forcemodel.generic.callback = Preferences_Event;
	s_preferences.forcemodel.generic.id       = ID_FORCEMODEL;
	s_preferences.forcemodel.generic.x	      = PREFERENCES_X_POS;
	s_preferences.forcemodel.generic.y	      = y;

	y += ( BIGCHAR_HEIGHT + 2 );
	s_preferences.glowmodel.generic.type		= MTYPE_RADIOBUTTON;
	s_preferences.glowmodel.generic.name		= "Glowing Player Model:";
	s_preferences.glowmodel.generic.flags		= ( QMF_SMALLFONT | QMF_HIDDEN );
	s_preferences.glowmodel.generic.callback	= Preferences_Event;
	s_preferences.glowmodel.generic.id			= ID_GLOWMODEL;
	s_preferences.glowmodel.generic.x			= PREFERENCES_X_POS;
	s_preferences.glowmodel.generic.y			= y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.simpleitems.generic.type        = MTYPE_RADIOBUTTON;
	s_preferences.simpleitems.generic.name	      = "Simple Items:";
	s_preferences.simpleitems.generic.flags	      = QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.simpleitems.generic.callback    = Preferences_Event;
	s_preferences.simpleitems.generic.id          = ID_SIMPLEITEMS;
	s_preferences.simpleitems.generic.x	          = PREFERENCES_X_POS;
	s_preferences.simpleitems.generic.y	          = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.wallmarks.generic.type          = MTYPE_RADIOBUTTON;
	s_preferences.wallmarks.generic.name	      = "Marks on Walls:";
	s_preferences.wallmarks.generic.flags	      = QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.wallmarks.generic.callback      = Preferences_Event;
	s_preferences.wallmarks.generic.id            = ID_WALLMARKS;
	s_preferences.wallmarks.generic.x	          = PREFERENCES_X_POS;
	s_preferences.wallmarks.generic.y	          = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.dynamiclights.generic.type      = MTYPE_RADIOBUTTON;
	s_preferences.dynamiclights.generic.name	  = "Dynamic Lights:";
	s_preferences.dynamiclights.generic.flags     = QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.dynamiclights.generic.callback  = Preferences_Event;
	s_preferences.dynamiclights.generic.id        = ID_DYNAMICLIGHTS;
	s_preferences.dynamiclights.generic.x	      = PREFERENCES_X_POS;
	s_preferences.dynamiclights.generic.y	      = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.identifytarget.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.identifytarget.generic.name	  = "Identify Target:";
	s_preferences.identifytarget.generic.flags    = QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.identifytarget.generic.callback = Preferences_Event;
	s_preferences.identifytarget.generic.id       = ID_IDENTIFYTARGET;
	s_preferences.identifytarget.generic.x	      = PREFERENCES_X_POS;
	s_preferences.identifytarget.generic.y	      = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.highqualitysky.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.highqualitysky.generic.name	  = "High Quality Sky:";
	s_preferences.highqualitysky.generic.flags	  = QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.highqualitysky.generic.callback = Preferences_Event;
	s_preferences.highqualitysky.generic.id       = ID_HIGHQUALITYSKY;
	s_preferences.highqualitysky.generic.x	      = PREFERENCES_X_POS;
	s_preferences.highqualitysky.generic.y	      = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.flares.generic.type		= MTYPE_RADIOBUTTON;
	s_preferences.flares.generic.name		= "Dynamic Flares:";
	s_preferences.flares.generic.flags		= QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.flares.generic.callback	= Preferences_Event;
	s_preferences.flares.generic.id			= ID_FLARES;
	s_preferences.flares.generic.x			= PREFERENCES_X_POS;
	s_preferences.flares.generic.y			= y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.synceveryframe.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.synceveryframe.generic.name	  = "Sync Every Frame:";
	s_preferences.synceveryframe.generic.flags	  = QMF_SMALLFONT|QMF_HIDDEN;
	s_preferences.synceveryframe.generic.callback = Preferences_Event;
	s_preferences.synceveryframe.generic.id       = ID_SYNCEVERYFRAME;
	s_preferences.synceveryframe.generic.x	      = PREFERENCES_X_POS;
	s_preferences.synceveryframe.generic.y	      = y;

	yp2 = y + BIGCHAR_HEIGHT + 2;

	// place the "more" button
	if(yp1 > yp2)
		y = yp1 + BIGCHAR_HEIGHT ;
	else
		y = yp2 + BIGCHAR_HEIGHT;

	s_preferences.more.generic.type			= MTYPE_TEXTS;
	s_preferences.more.fontHeight			= 20.0f;
	s_preferences.more.generic.flags		= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_preferences.more.generic.x			= PREFERENCES_X_POS;
	s_preferences.more.generic.y			= y;
	s_preferences.more.generic.id			= ID_MORE;
	s_preferences.more.generic.callback		= Preferences_Event; 
	s_preferences.more.string				= "More";
	s_preferences.more.color				= color_lightOrange;
	s_preferences.more.focuscolor			= color_orange;
	s_preferences.more.style				= UI_CENTER|UI_SMALLFONT;

	s_preferences.back.generic.type		= MTYPE_BITMAP;
	s_preferences.back.generic.name		= ART_BACK0;
	s_preferences.back.generic.flags	= QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_preferences.back.generic.x		= 552;//8;
	s_preferences.back.generic.y		= 440;
	s_preferences.back.generic.id		= ID_BACK;
	s_preferences.back.generic.callback	= Preferences_Event;
    s_preferences.back.width			= 80;
	s_preferences.back.height			= 40;
	s_preferences.back.focuspic			= ART_BACK1;
	s_preferences.back.focuspicinstead	= qtrue;

	/*
	(menucommon_s *)&s_preferences.crosshair,
	(menucommon_s *)&s_preferences.drawteamoverlay,
	(menucommon_s *)&s_preferences.ffahud,
	(menucommon_s *)&s_preferences.con_notifytime,
	(menucommon_s *)&s_preferences.timer,
	(menucommon_s *)&s_preferences.timeleft,
	(menucommon_s *)&s_preferences.realtime,
	(menucommon_s *)&s_preferences.ups,
	(menucommon_s *)&s_preferences.fps,   
	*/

	// page 1
	Menu_AddItem( &s_preferences.menu, &s_preferences.crosshair );
	Menu_AddItem( &s_preferences.menu, &s_preferences.drawteamoverlay );
	Menu_AddItem( &s_preferences.menu, &s_preferences.ffahud );
	Menu_AddItem( &s_preferences.menu, &s_preferences.con_notifytime );
	Menu_AddItem( &s_preferences.menu, &s_preferences.timer );
	Menu_AddItem( &s_preferences.menu, &s_preferences.timeleft );
	Menu_AddItem( &s_preferences.menu, &s_preferences.realtime );
	Menu_AddItem( &s_preferences.menu, &s_preferences.ups );
	Menu_AddItem( &s_preferences.menu, &s_preferences.fps );
	// page 2
	Menu_AddItem( &s_preferences.menu, &s_preferences.simpleitems );
	Menu_AddItem( &s_preferences.menu, &s_preferences.wallmarks );
	Menu_AddItem( &s_preferences.menu, &s_preferences.dynamiclights );
	Menu_AddItem( &s_preferences.menu, &s_preferences.identifytarget );
	Menu_AddItem( &s_preferences.menu, &s_preferences.highqualitysky );
	Menu_AddItem( &s_preferences.menu, &s_preferences.synceveryframe );
	Menu_AddItem( &s_preferences.menu, &s_preferences.flares );
	Menu_AddItem( &s_preferences.menu, &s_preferences.forcemodel );
	Menu_AddItem( &s_preferences.menu, &s_preferences.glowmodel );
	// misc
	Menu_AddItem( &s_preferences.menu, &s_preferences.more );
	Menu_AddItem( &s_preferences.menu, &s_preferences.back );

	Preferences_SetMenuItems();
	// show items of the first page
	page = 1;
	SwitchPage();
}


/*
===============
Preferences_Cache
===============
*/
void Preferences_Cache( void ) {
	int		n;

	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	for( n = 0; n < NUM_CROSSHAIRS; n++ ) {
		s_preferences.crosshairShader[n] = trap_R_RegisterShaderNoMip( va("gfx/2d/crosshair%c", 'a' + n ) );
	}
}


/*
===============
UI_PreferencesMenu
===============
*/
void UI_PreferencesMenu( void ) {
	Preferences_MenuInit();
	UI_PushMenu( &s_preferences.menu );
}
