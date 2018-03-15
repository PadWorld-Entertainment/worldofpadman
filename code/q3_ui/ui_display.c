// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

DISPLAY OPTIONS MENU

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
#define ACCEPT0		"menu/system/accept"
#define ACCEPT1		"menu/system/accept"

#define ID_GRAPHICS			10
#define ID_DISPLAY			11
#define ID_SOUND			12
#define ID_NETWORK			13
#define ID_BRIGHTNESS		14
#define	ID_ANAGLYPH			17
#define ID_GREYSCALE		18
#define ID_BACK			15
#define ID_IGNOREHWG		16

typedef struct {
	menuframework_s	menu;

	menubitmap_s	graphics;
	menubitmap_s	display;
	menubitmap_s	sound;
	menubitmap_s	network;

	menuradiobutton_s	ignoreHWG;

	menuslider_s	brightness;

	menulist_s		anaglyph;

	menuslider_s	greyscale;

	menubitmap_s	apply;
	menubitmap_s	back;
} displayOptionsInfo_t;

static displayOptionsInfo_t	displayOptionsInfo;

static const char *anaglyph_names[] = {
	"off",
	"red-cyan",
	"red-blue",
	"red-green",
	"cyan-red",
	"blue-red",
	"green-red",
	NULL
};



static void ApplyPressed( void *unused, int notification )
{
	if (notification != QM_ACTIVATED)
		return;

	// hide the button and do the vid restart
	displayOptionsInfo.apply.generic.flags |= QMF_HIDDEN|QMF_INACTIVE;
	trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart\n" );
}
/*
=================
UI_DisplayOptionsMenu_Event
=================
*/
static void UI_DisplayOptionsMenu_Event( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_GRAPHICS:
		UI_PopMenu();
		UI_GraphicsOptionsMenu();
		break;

	case ID_DISPLAY:
		break;

	case ID_SOUND:
		UI_PopMenu();
		UI_SoundOptionsMenu();
		break;

	case ID_NETWORK:
		UI_PopMenu();
		UI_NetworkOptionsMenu();
		break;

	case ID_BRIGHTNESS:
		trap_Cvar_SetValue( "r_gamma", displayOptionsInfo.brightness.curvalue / 10.0f );
		break;
	
	case ID_IGNOREHWG:
		trap_Cvar_SetValue( "r_ignorehwgamma", displayOptionsInfo.ignoreHWG.curvalue );
		trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart\n" );
		break;

	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_ANAGLYPH:
		trap_Cvar_SetValue( "r_anaglyphMode", displayOptionsInfo.anaglyph.curvalue );
		if ( !displayOptionsInfo.anaglyph.curvalue ) {
			displayOptionsInfo.greyscale.generic.flags |= QMF_GRAYED;
		}
		else {
			displayOptionsInfo.greyscale.generic.flags &= ~QMF_GRAYED;
		}
		break;

	case ID_GREYSCALE:
		trap_Cvar_SetValue( "r_greyscale", ( displayOptionsInfo.greyscale.curvalue / 100.0f ) );
		displayOptionsInfo.apply.generic.flags &= ~( QMF_HIDDEN | QMF_INACTIVE );
		break;
	}
}


/*
===============
UI_DisplayOptionsMenu_Init
===============
*/
static void UI_DisplayOptionsMenu_Init( void ) {
	int		y;

	memset( &displayOptionsInfo, 0, sizeof(displayOptionsInfo) );

	UI_DisplayOptionsMenu_Cache();
	displayOptionsInfo.menu.wrapAround = qtrue;
	displayOptionsInfo.menu.fullscreen = qtrue;

	displayOptionsInfo.menu.bgparts		= BGP_SYSTEMBG|BGP_SIMPLEBG;

	displayOptionsInfo.graphics.generic.type	= MTYPE_BITMAP;
	displayOptionsInfo.graphics.generic.name	= GRAPHICS0;
	displayOptionsInfo.graphics.generic.flags	= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.graphics.generic.callback= UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.graphics.generic.id		= ID_GRAPHICS;
	displayOptionsInfo.graphics.generic.x		= 26;
	displayOptionsInfo.graphics.generic.y		= 37;
	displayOptionsInfo.graphics.width			= 130;
	displayOptionsInfo.graphics.height			= 40;
	displayOptionsInfo.graphics.focuspic		= GRASHICS1;
	displayOptionsInfo.graphics.focuspicinstead	= qtrue;

	displayOptionsInfo.display.generic.type		= MTYPE_BITMAP;
	displayOptionsInfo.display.generic.name		= DISPLAY0;
	displayOptionsInfo.display.generic.flags	= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.display.generic.callback	= UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.display.generic.id		= ID_DISPLAY;
	displayOptionsInfo.display.generic.x		= 159;
	displayOptionsInfo.display.generic.y		= 30;
	displayOptionsInfo.display.width			= 122;
	displayOptionsInfo.display.height			= 40;
	displayOptionsInfo.display.focuspic			= DISPLAY1;
	displayOptionsInfo.display.focuspicinstead	= qtrue;

	displayOptionsInfo.sound.generic.type		= MTYPE_BITMAP;
	displayOptionsInfo.sound.generic.name		= SOUND0;
	displayOptionsInfo.sound.generic.flags		= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.sound.generic.callback	= UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.sound.generic.id			= ID_SOUND;
	displayOptionsInfo.sound.generic.x			= 40;
	displayOptionsInfo.sound.generic.y			= 77;
	displayOptionsInfo.sound.width				= 130;
	displayOptionsInfo.sound.height				= 40;
	displayOptionsInfo.sound.focuspic			= SOUND1;
	displayOptionsInfo.sound.focuspicinstead		= qtrue;

	displayOptionsInfo.network.generic.type		= MTYPE_BITMAP;
	displayOptionsInfo.network.generic.name		= NETWORK0;
	displayOptionsInfo.network.generic.flags	= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.network.generic.callback	= UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.network.generic.id		= ID_NETWORK;
	displayOptionsInfo.network.generic.x		= 170;
	displayOptionsInfo.network.generic.y		= 80;
	displayOptionsInfo.network.width			= 100;
	displayOptionsInfo.network.height			= 45;
	displayOptionsInfo.network.focuspic			= NETWORK1;
	displayOptionsInfo.network.focuspicinstead	= qtrue;

	y = 230;
	displayOptionsInfo.ignoreHWG.generic.type		= MTYPE_RADIOBUTTON;
	displayOptionsInfo.ignoreHWG.generic.name		= "Ignore HW-Gamma:";
	displayOptionsInfo.ignoreHWG.generic.flags		= QMF_SMALLFONT;
	displayOptionsInfo.ignoreHWG.generic.callback	= UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.ignoreHWG.generic.id			= ID_IGNOREHWG;
	displayOptionsInfo.ignoreHWG.generic.x			= 175;
	displayOptionsInfo.ignoreHWG.generic.y			= y;
	displayOptionsInfo.ignoreHWG.curvalue			= UI_GetCvarInt("r_ignorehwgamma");
	displayOptionsInfo.ignoreHWG.generic.toolTip	= "If enabled you won't be able to adjust the brightness in game and will be locked and controlled by your current graphics card and monitor options. It is recommended to leave it off so you can adjust the brightness via the slider if necessary.";

	y += BIGCHAR_HEIGHT+2;
	displayOptionsInfo.brightness.generic.type		= MTYPE_SLIDER;
	displayOptionsInfo.brightness.generic.name		= "Brightness:";
	displayOptionsInfo.brightness.generic.flags		= QMF_SMALLFONT;
	displayOptionsInfo.brightness.generic.callback	= UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.brightness.generic.id		= ID_BRIGHTNESS;
	displayOptionsInfo.brightness.generic.x			= 175;
	displayOptionsInfo.brightness.generic.y			= y;
	displayOptionsInfo.brightness.minvalue			= 5;
	displayOptionsInfo.brightness.maxvalue			= 20;
	if( !uis.glconfig.deviceSupportsGamma )
		displayOptionsInfo.brightness.generic.flags |= QMF_GRAYED;

	y += ( 2 * BIGCHAR_HEIGHT + 2 );
	displayOptionsInfo.anaglyph.generic.type		= MTYPE_SPINCONTROL;
	displayOptionsInfo.anaglyph.generic.name		= "Stereoscopic 3D:";
	displayOptionsInfo.anaglyph.generic.flags		= QMF_SMALLFONT;
	displayOptionsInfo.anaglyph.generic.callback	= UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.anaglyph.generic.id			= ID_ANAGLYPH;
	displayOptionsInfo.anaglyph.generic.x			= 175;
	displayOptionsInfo.anaglyph.generic.y			= y;
	displayOptionsInfo.anaglyph.itemnames			= anaglyph_names;
	displayOptionsInfo.anaglyph.generic.toolTip		= "Switch on to play in 3D with the appriopriate glasses. Ensure the correct filter option you select matches that of your glasses or the effect won't work at all.";

	y += ( BIGCHAR_HEIGHT + 2 );
	displayOptionsInfo.greyscale.generic.type		= MTYPE_SLIDER;
	displayOptionsInfo.greyscale.generic.name		= "Greyscale:";
	displayOptionsInfo.greyscale.generic.flags		= QMF_SMALLFONT;
	displayOptionsInfo.greyscale.generic.callback	= UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.greyscale.generic.id			= ID_GREYSCALE;
	displayOptionsInfo.greyscale.generic.x			= 175;
	displayOptionsInfo.greyscale.generic.y			= y;
	displayOptionsInfo.greyscale.minvalue			= 0;
	displayOptionsInfo.greyscale.maxvalue			= 100;

	displayOptionsInfo.apply.generic.type     = MTYPE_BITMAP;
	displayOptionsInfo.apply.generic.name     = ACCEPT0;
	displayOptionsInfo.apply.generic.flags    = QMF_PULSEIFFOCUS|QMF_HIDDEN|QMF_INACTIVE;
	displayOptionsInfo.apply.generic.callback = ApplyPressed;
	displayOptionsInfo.apply.generic.x        = 516;
	displayOptionsInfo.apply.generic.y        = 405;
	displayOptionsInfo.apply.width				= 102;
	displayOptionsInfo.apply.height  			= 61;
	displayOptionsInfo.apply.focuspic			= ACCEPT1;


	displayOptionsInfo.back.generic.type	= MTYPE_BITMAP;
	displayOptionsInfo.back.generic.name    = BACK0;
	displayOptionsInfo.back.generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	displayOptionsInfo.back.generic.callback= UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.back.generic.id	    = ID_BACK;
	displayOptionsInfo.back.generic.x		= 9;
	displayOptionsInfo.back.generic.y		= 440;
	displayOptionsInfo.back.width  		    = 80;
	displayOptionsInfo.back.height  	    = 40;
	displayOptionsInfo.back.focuspic        = BACK1;
	displayOptionsInfo.back.focuspicinstead	= qtrue;

	Menu_AddItem( &displayOptionsInfo.menu, ( void * ) &displayOptionsInfo.graphics );
	Menu_AddItem( &displayOptionsInfo.menu, ( void * ) &displayOptionsInfo.display );
	Menu_AddItem( &displayOptionsInfo.menu, ( void * ) &displayOptionsInfo.sound );
	Menu_AddItem( &displayOptionsInfo.menu, ( void * ) &displayOptionsInfo.network );
	Menu_AddItem( &displayOptionsInfo.menu, &displayOptionsInfo.ignoreHWG );
	Menu_AddItem( &displayOptionsInfo.menu, ( void * ) &displayOptionsInfo.brightness );
	Menu_AddItem( &displayOptionsInfo.menu, ( void * ) &displayOptionsInfo.anaglyph );
	Menu_AddItem( &displayOptionsInfo.menu, ( void * ) &displayOptionsInfo.greyscale );
	Menu_AddItem( &displayOptionsInfo.menu, ( void * ) &displayOptionsInfo.apply );
	Menu_AddItem( &displayOptionsInfo.menu, ( void * ) &displayOptionsInfo.back );

	displayOptionsInfo.brightness.curvalue  = trap_Cvar_VariableValue("r_gamma") * 10;
	displayOptionsInfo.anaglyph.curvalue	= Com_Clamp( 0, ( ARRAY_LEN( anaglyph_names ) - 1 ), trap_Cvar_VariableValue( "r_anaglyphMode" ) );
	displayOptionsInfo.greyscale.curvalue  = Com_Clamp( 0, 100, ( trap_Cvar_VariableValue( "r_greyscale" ) * 100 ) );

	if ( !displayOptionsInfo.anaglyph.curvalue ) {
		displayOptionsInfo.greyscale.generic.flags |= QMF_GRAYED;
	}
}


/*
===============
UI_DisplayOptionsMenu_Cache
===============
*/
void UI_DisplayOptionsMenu_Cache( void ) {
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
UI_DisplayOptionsMenu
===============
*/
void UI_DisplayOptionsMenu( void ) {
	UI_DisplayOptionsMenu_Init();
	UI_PushMenu( &displayOptionsInfo.menu );
	Menu_SetCursorToItem( &displayOptionsInfo.menu, &displayOptionsInfo.display );
}
