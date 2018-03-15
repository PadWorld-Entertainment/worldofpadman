// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

NETWORK OPTIONS MENU

=======================================================================
*/

#include "ui_local.h"


#define BACK0		"menu/BtnBack0"
#define BACK1		"menu/BtnBack1"
#define GRAPHICS0	"menu/system/graphics0"
#define GRASHICS1	"menu/system/graphics1"
#define DISPLAY0	"menu/system/display0"
#define DISPLAY1	"menu/system/display1"
#define SOUND0		"menu/system/sound0"
#define SOUND1		"menu/system/sound1"
#define NETWORK0	"menu/system/network0"
#define NETWORK1	"menu/system/network1"

#define ID_GRAPHICS			10
#define ID_DISPLAY			11
#define ID_SOUND			12
#define ID_NETWORK			13
#define ID_RATE			14
#define ID_ALLOWDOWNLOAD	15
#define ID_BACK			16


static const char *rate_items[] = {
	"Modem",
	"ISDN",
	"LAN/Cable/xDSL",
	0
};

typedef struct {
	menuframework_s	menu;

	menubitmap_s		graphics;
	menubitmap_s		display;
	menubitmap_s		sound;
	menubitmap_s		network;

	menulist_s		rate;
	menuradiobutton_s	allowdownload;

	menubitmap_s	back;
} networkOptionsInfo_t;

static networkOptionsInfo_t	networkOptionsInfo;


/*
=================
UI_NetworkOptionsMenu_Event
=================
*/
static void UI_NetworkOptionsMenu_Event( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_GRAPHICS:
		UI_PopMenu();
		UI_GraphicsOptionsMenu();
		break;

	case ID_DISPLAY:
		UI_PopMenu();
		UI_DisplayOptionsMenu();
		break;

	case ID_SOUND:
		UI_PopMenu();
		UI_SoundOptionsMenu();
		break;

	case ID_NETWORK:
		break;

	case ID_RATE:
		if( networkOptionsInfo.rate.curvalue == 0 ) {
			trap_Cvar_SetValue( "rate", 4000 );
		}
		else if( networkOptionsInfo.rate.curvalue == 1 ) {
			trap_Cvar_SetValue( "rate", 5000 );
		}
		else if( networkOptionsInfo.rate.curvalue == 2 ) {
			trap_Cvar_SetValue( "rate", 25000 );
		}
		break;

	case ID_ALLOWDOWNLOAD:
		trap_Cvar_SetValue( "cl_allowDownload", networkOptionsInfo.allowdownload.curvalue );
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}


/*
===============
UI_NetworkOptionsMenu_Init
===============
*/
static void UI_NetworkOptionsMenu_Init( void ) {
	int		y;
	int		rate;

	memset( &networkOptionsInfo, 0, sizeof(networkOptionsInfo) );

	UI_NetworkOptionsMenu_Cache();
	networkOptionsInfo.menu.wrapAround = qtrue;
	networkOptionsInfo.menu.fullscreen = qtrue;
	networkOptionsInfo.menu.bgparts		= BGP_SYSTEMBG|BGP_SIMPLEBG;

	networkOptionsInfo.graphics.generic.type		= MTYPE_BITMAP;
	networkOptionsInfo.graphics.generic.name		= GRAPHICS0;
	networkOptionsInfo.graphics.generic.flags		= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	networkOptionsInfo.graphics.generic.callback	= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.graphics.generic.id			= ID_GRAPHICS;
	networkOptionsInfo.graphics.generic.x			= 26;
	networkOptionsInfo.graphics.generic.y			= 37;
	networkOptionsInfo.graphics.width				= 130;
	networkOptionsInfo.graphics.height				= 40;
	networkOptionsInfo.graphics.focuspic			= GRASHICS1;
	networkOptionsInfo.graphics.focuspicinstead		= qtrue;

	networkOptionsInfo.display.generic.type			= MTYPE_BITMAP;
	networkOptionsInfo.display.generic.name			= DISPLAY0;
	networkOptionsInfo.display.generic.flags		= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	networkOptionsInfo.display.generic.callback		= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.display.generic.id			= ID_DISPLAY;
	networkOptionsInfo.display.generic.x			= 159;
	networkOptionsInfo.display.generic.y			= 30;
	networkOptionsInfo.display.width				= 122;
	networkOptionsInfo.display.height				= 40;
	networkOptionsInfo.display.focuspic				= DISPLAY1;
	networkOptionsInfo.display.focuspicinstead		= qtrue;

	networkOptionsInfo.sound.generic.type			= MTYPE_BITMAP;
	networkOptionsInfo.sound.generic.name			= SOUND0;
	networkOptionsInfo.sound.generic.flags			= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	networkOptionsInfo.sound.generic.callback		= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.sound.generic.id				= ID_SOUND;
	networkOptionsInfo.sound.generic.x				= 40;
	networkOptionsInfo.sound.generic.y				= 77;
	networkOptionsInfo.sound.width					= 130;
	networkOptionsInfo.sound.height					= 40;
	networkOptionsInfo.sound.focuspic				= SOUND1;
	networkOptionsInfo.sound.focuspicinstead		= qtrue;

	networkOptionsInfo.network.generic.type			= MTYPE_BITMAP;
	networkOptionsInfo.network.generic.name			= NETWORK0;
	networkOptionsInfo.network.generic.flags		= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	networkOptionsInfo.network.generic.callback		= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.network.generic.id			= ID_NETWORK;
	networkOptionsInfo.network.generic.x			= 170;
	networkOptionsInfo.network.generic.y			= 80;
	networkOptionsInfo.network.width				= 100;
	networkOptionsInfo.network.height				= 45;
	networkOptionsInfo.network.focuspic				= NETWORK1;
	networkOptionsInfo.network.focuspicinstead		= qtrue;

	y = 240 - 1 * (BIGCHAR_HEIGHT+2);
	networkOptionsInfo.rate.generic.type		= MTYPE_SPINCONTROL;
	networkOptionsInfo.rate.generic.name		= "Data Rate:";
	networkOptionsInfo.rate.generic.flags		= QMF_SMALLFONT;
	networkOptionsInfo.rate.generic.callback	= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.rate.generic.id			= ID_RATE;
	networkOptionsInfo.rate.generic.x			= 190;
	networkOptionsInfo.rate.generic.y			= y;
	networkOptionsInfo.rate.itemnames			= rate_items;

	y += ( BIGCHAR_HEIGHT + 2 );
	networkOptionsInfo.allowdownload.generic.type	= MTYPE_RADIOBUTTON;
	networkOptionsInfo.allowdownload.generic.name	= "Auto Download:";
	networkOptionsInfo.allowdownload.generic.flags	= QMF_SMALLFONT;
	networkOptionsInfo.allowdownload.generic.callback	= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.allowdownload.generic.id		= ID_ALLOWDOWNLOAD;
	networkOptionsInfo.allowdownload.generic.x		= 190;
	networkOptionsInfo.allowdownload.generic.y		= y;
	networkOptionsInfo.allowdownload.generic.toolTip = "Enable to automatically download custom maps / custom files needed to play on modified servers. NOTE: Custom game files may interfere with normal game files. Only connect to trusted servers with this enabled.";

	networkOptionsInfo.back.generic.type		= MTYPE_BITMAP;
	networkOptionsInfo.back.generic.name		= BACK0;
	networkOptionsInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	networkOptionsInfo.back.generic.callback	= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.back.generic.id			= ID_BACK;
	networkOptionsInfo.back.generic.x			= 9;
	networkOptionsInfo.back.generic.y			= 440;
	networkOptionsInfo.back.width  				= 80;
	networkOptionsInfo.back.height  		    = 40;
	networkOptionsInfo.back.focuspic			= BACK1;
	networkOptionsInfo.back.focuspicinstead		= qtrue;

	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.graphics );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.display );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.sound );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.network );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.rate );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.allowdownload );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.back );

	rate = trap_Cvar_VariableValue( "rate" );
	if( rate <= 4000 ) {
		networkOptionsInfo.rate.curvalue = 0;
	}
	else if( rate <= 5000 ) {
		networkOptionsInfo.rate.curvalue = 1;
	}
	else{
		networkOptionsInfo.rate.curvalue = 2;
	}

	networkOptionsInfo.allowdownload.curvalue = ( trap_Cvar_VariableValue( "cl_allowDownload" ) != 0 );
}


/*
===============
UI_NetworkOptionsMenu_Cache
===============
*/
void UI_NetworkOptionsMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
	trap_R_RegisterShaderNoMip(GRAPHICS0);
	trap_R_RegisterShaderNoMip(GRASHICS1);
	trap_R_RegisterShaderNoMip(DISPLAY0);
	trap_R_RegisterShaderNoMip(DISPLAY1);
	trap_R_RegisterShaderNoMip(SOUND0);
	trap_R_RegisterShaderNoMip(SOUND1);
	trap_R_RegisterShaderNoMip(NETWORK0);
	trap_R_RegisterShaderNoMip(NETWORK1);
}


/*
===============
UI_NetworkOptionsMenu
===============
*/
void UI_NetworkOptionsMenu( void ) {
	UI_NetworkOptionsMenu_Init();
	UI_PushMenu( &networkOptionsInfo.menu );
	Menu_SetCursorToItem( &networkOptionsInfo.menu, &networkOptionsInfo.network );
}
