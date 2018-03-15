// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

SOUND OPTIONS MENU

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
#define ID_EFFECTSVOLUME	14
#define ID_MUSICVOLUME		15

#define ID_MUSICAUTOSWITCH	17
#define ID_BACK				18

#define ID_openAL			19
#define ID_VOICETHRESHOLD	20
#define ID_GAINWHILECAPTURE 21
#define ID_VOIPMODE			22
#define ID_RECORDMODE		23

#define POSITION_X			180

static const char* recording_modes[] =
{
	"Push to talk", "Automatic", 0
};

typedef struct {
	menuframework_s		menu;

	menubitmap_s		graphics;
	menubitmap_s		display;
	menubitmap_s		sound;
	menubitmap_s		network;
	menuradiobutton_s	openAL;

	menuslider_s		sfxvolume;
	menuslider_s		musicvolume;
	menuradiobutton_s	musicautoswitch;
	
	menuradiobutton_s	voipmode;
	menulist_s			voipRecordMode;
	menutext_s			voipmode_grayed;
	menutext_s			voip_restart;
	menuslider_s		voiceThresholdVAD;
	menuslider_s		voiceGainDuringCapture;

	menubitmap_s		back;
} soundOptionsInfo_t;

static soundOptionsInfo_t	soundOptionsInfo;


static void UI_SoundOptionsMenu_Update( void )
{
	// openAL and a high rate are conditions for all voip settings
	qboolean disableVoip = ( 0 == soundOptionsInfo.openAL.curvalue || 25000 > trap_Cvar_VariableValue( "rate" )  );
	// voip is a condition for voip volume
	qboolean voipOff = ( 0 == soundOptionsInfo.voipmode.curvalue );
	// automatic recording is a condition for voip threshold
	qboolean hideVADThreshold = ( 0 == soundOptionsInfo.voipRecordMode.curvalue );
	int y;

	if( disableVoip )
	{
		soundOptionsInfo.voipmode_grayed.generic.flags &= ~QMF_HIDDEN;
		soundOptionsInfo.voipmode.generic.flags |= QMF_GRAYED;
		soundOptionsInfo.voipRecordMode.generic.flags |= QMF_GRAYED;
		soundOptionsInfo.voiceThresholdVAD.generic.flags |= QMF_GRAYED;
		soundOptionsInfo.voiceGainDuringCapture.generic.flags |= QMF_GRAYED;
	}
	else
	{
		soundOptionsInfo.voipmode_grayed.generic.flags |= QMF_HIDDEN;
		soundOptionsInfo.voipmode.generic.flags &= ~QMF_GRAYED;		

		if( voipOff )
		{
			soundOptionsInfo.voipRecordMode.generic.flags |= QMF_GRAYED;
			soundOptionsInfo.voiceThresholdVAD.generic.flags |= QMF_GRAYED;
			soundOptionsInfo.voiceGainDuringCapture.generic.flags |= QMF_GRAYED;
		}
		else
		{
			soundOptionsInfo.voipRecordMode.generic.flags &= ~QMF_GRAYED;
			soundOptionsInfo.voiceGainDuringCapture.generic.flags &= ~QMF_GRAYED;

			if( hideVADThreshold )
				soundOptionsInfo.voiceThresholdVAD.generic.flags |= QMF_GRAYED;
			else
				soundOptionsInfo.voiceThresholdVAD.generic.flags &= ~QMF_GRAYED;
		}
	}

	// move voip settings down to make space for the voipmode_grayed text
	// or move them back up
	y = soundOptionsInfo.voipmode_grayed.generic.y;
	if( disableVoip )
	{
		y += BIGCHAR_HEIGHT + 2;
	}
	soundOptionsInfo.voipmode.generic.y =  y;
	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voipRecordMode.generic.y =  y;
	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voiceThresholdVAD.generic.y =  y;
	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voiceGainDuringCapture.generic.y = y;
}
/*
=================
UI_SoundOptionsMenu_Event
=================
*/
static void UI_SoundOptionsMenu_Event( void* ptr, int event ) {
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
		break;

	case ID_NETWORK:
		UI_PopMenu();
		UI_NetworkOptionsMenu();
		break;

	case ID_EFFECTSVOLUME:
		trap_Cvar_SetValue( "s_volume", soundOptionsInfo.sfxvolume.curvalue / 10 );
		break;

	case ID_MUSICVOLUME:
		trap_Cvar_SetValue( "s_musicvolume", soundOptionsInfo.musicvolume.curvalue / 10 );
		break;

	case ID_MUSICAUTOSWITCH:
		trap_Cvar_SetValue( "wop_AutoswitchSongByNextMap", soundOptionsInfo.musicautoswitch.curvalue );
		break;

	case ID_openAL:
//		soundOptionsInfo.openAL.curvalue = !soundOptionsInfo.openAL.curvalue;
		trap_Cvar_SetValue( "s_useOpenAL", soundOptionsInfo.openAL.curvalue );
		trap_Cmd_ExecuteText( EXEC_APPEND, "snd_restart\n" );

		break;

	case ID_GAINWHILECAPTURE:
		trap_Cvar_SetValue("cl_voipGainDuringCapture", soundOptionsInfo.voiceGainDuringCapture.curvalue / 10 );
		break;

	case ID_VOIPMODE:
		trap_Cvar_SetValue( "cl_voip", soundOptionsInfo.voipmode.curvalue );
		// voip activated? then tell the user to restart
		if( soundOptionsInfo.voipmode.curvalue )
			soundOptionsInfo.voip_restart.generic.flags &= ~QMF_HIDDEN;
		else
			soundOptionsInfo.voip_restart.generic.flags |= QMF_HIDDEN;

		break;

	case ID_RECORDMODE:
		trap_Cvar_SetValue("cl_voipUseVAD", soundOptionsInfo.voipRecordMode.curvalue );
		break;

	case ID_VOICETHRESHOLD:
		trap_Cvar_SetValue("cl_voipVADThreshold", soundOptionsInfo.voiceThresholdVAD.curvalue / 10 );
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}

	UI_SoundOptionsMenu_Update();
}

/*
===============
UI_SoundOptionsMenu_Init
===============
*/
static void UI_SoundOptionsMenu_Init( void ) {
	int				y;

	memset( &soundOptionsInfo, 0, sizeof(soundOptionsInfo) );

	UI_SoundOptionsMenu_Cache();
	soundOptionsInfo.menu.wrapAround = qtrue;
	soundOptionsInfo.menu.fullscreen = qtrue;
	soundOptionsInfo.menu.bgparts	= BGP_SYSTEMBG|BGP_SIMPLEBG;

	soundOptionsInfo.graphics.generic.type		= MTYPE_BITMAP;
	soundOptionsInfo.graphics.generic.name		= GRAPHICS0;
	soundOptionsInfo.graphics.generic.flags		= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	soundOptionsInfo.graphics.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.graphics.generic.id		= ID_GRAPHICS;
	soundOptionsInfo.graphics.generic.x			= 26;
	soundOptionsInfo.graphics.generic.y			= 37;
	soundOptionsInfo.graphics.width				= 130;
	soundOptionsInfo.graphics.height			= 40;
	soundOptionsInfo.graphics.focuspic			= GRASHICS1;
	soundOptionsInfo.graphics.focuspicinstead	= qtrue;

	soundOptionsInfo.display.generic.type		= MTYPE_BITMAP;
	soundOptionsInfo.display.generic.name		= DISPLAY0;
	soundOptionsInfo.display.generic.flags		= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	soundOptionsInfo.display.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.display.generic.id			= ID_DISPLAY;
	soundOptionsInfo.display.generic.x			= 159;
	soundOptionsInfo.display.generic.y			= 30;
	soundOptionsInfo.display.width				= 122;
	soundOptionsInfo.display.height				= 40;
	soundOptionsInfo.display.focuspic			= DISPLAY1;
	soundOptionsInfo.display.focuspicinstead	= qtrue;

	soundOptionsInfo.sound.generic.type			= MTYPE_BITMAP;
	soundOptionsInfo.sound.generic.name			= SOUND0;
	soundOptionsInfo.sound.generic.flags		= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	soundOptionsInfo.sound.generic.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.sound.generic.id			= ID_SOUND;
	soundOptionsInfo.sound.generic.x			= 40;
	soundOptionsInfo.sound.generic.y			= 77;
	soundOptionsInfo.sound.width				= 130;
	soundOptionsInfo.sound.height				= 40;
	soundOptionsInfo.sound.focuspic				= SOUND1;
	soundOptionsInfo.sound.focuspicinstead		= qtrue;

	soundOptionsInfo.network.generic.type		= MTYPE_BITMAP;
	soundOptionsInfo.network.generic.name		= NETWORK0;
	soundOptionsInfo.network.generic.flags		= QMF_LEFT_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	soundOptionsInfo.network.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.network.generic.id			= ID_NETWORK;
	soundOptionsInfo.network.generic.x			= 170;
	soundOptionsInfo.network.generic.y			= 80;
	soundOptionsInfo.network.width				= 100;
	soundOptionsInfo.network.height				= 45;
	soundOptionsInfo.network.focuspic			= NETWORK1;
	soundOptionsInfo.network.focuspicinstead	= qtrue;

	y = 240 - 1.5 * (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.sfxvolume.generic.type		= MTYPE_SLIDER;
	soundOptionsInfo.sfxvolume.generic.name		= "Effects Volume:";
	soundOptionsInfo.sfxvolume.generic.flags	= QMF_SMALLFONT;
	soundOptionsInfo.sfxvolume.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.sfxvolume.generic.id		= ID_EFFECTSVOLUME;
	soundOptionsInfo.sfxvolume.generic.x		= POSITION_X;
	soundOptionsInfo.sfxvolume.generic.y		= y;
	soundOptionsInfo.sfxvolume.minvalue			= 0;
	soundOptionsInfo.sfxvolume.maxvalue			= 10;

	y += BIGCHAR_HEIGHT+2;
	soundOptionsInfo.musicvolume.generic.type		= MTYPE_SLIDER;
	soundOptionsInfo.musicvolume.generic.name		= "Music Volume:";
	soundOptionsInfo.musicvolume.generic.flags		= QMF_SMALLFONT;
	soundOptionsInfo.musicvolume.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.musicvolume.generic.id			= ID_MUSICVOLUME;
	soundOptionsInfo.musicvolume.generic.x			= POSITION_X;
	soundOptionsInfo.musicvolume.generic.y			= y;
	soundOptionsInfo.musicvolume.minvalue			= 0;
	soundOptionsInfo.musicvolume.maxvalue			= 10;

	y += BIGCHAR_HEIGHT*2+2;
	soundOptionsInfo.openAL.generic.type			= MTYPE_RADIOBUTTON;
	soundOptionsInfo.openAL.generic.name			= "Use OpenAL:";
	soundOptionsInfo.openAL.generic.flags			= QMF_SMALLFONT;
	soundOptionsInfo.openAL.generic.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.openAL.generic.id				= ID_openAL;
	soundOptionsInfo.openAL.generic.x				= POSITION_X;
	soundOptionsInfo.openAL.generic.y				= y;
	soundOptionsInfo.openAL.curvalue				= UI_GetCvarInt("s_useOpenAL");

	y += ( BIGCHAR_HEIGHT + 2 );
	soundOptionsInfo.musicautoswitch.generic.type	= MTYPE_RADIOBUTTON;
	soundOptionsInfo.musicautoswitch.generic.name	= "Auto switch Song";
	soundOptionsInfo.musicautoswitch.generic.flags	= QMF_SMALLFONT;
	soundOptionsInfo.musicautoswitch.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.musicautoswitch.generic.id		= ID_MUSICAUTOSWITCH;
	soundOptionsInfo.musicautoswitch.generic.x		= POSITION_X;
	soundOptionsInfo.musicautoswitch.generic.y		= y;
	soundOptionsInfo.musicautoswitch.generic.toolTip = "Enable to automatically switch to the next song on map change, if set to off current song will restart on map change.";

	y += BIGCHAR_HEIGHT*2 + 2;
	soundOptionsInfo.voipmode.generic.type			= MTYPE_RADIOBUTTON;
	soundOptionsInfo.voipmode.generic.name			= "Voice Chat:";
	soundOptionsInfo.voipmode.generic.flags			= QMF_SMALLFONT;
	soundOptionsInfo.voipmode.generic.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.voipmode.generic.id				= ID_VOIPMODE;
	soundOptionsInfo.voipmode.generic.x				= POSITION_X;
	soundOptionsInfo.voipmode.generic.y				= y;

	soundOptionsInfo.voipmode_grayed.generic.type		= MTYPE_TEXT;
	soundOptionsInfo.voipmode_grayed.generic.flags		= QMF_PULSE;
	soundOptionsInfo.voipmode_grayed.generic.x			= POSITION_X - 25;
	soundOptionsInfo.voipmode_grayed.generic.y			= y;
	soundOptionsInfo.voipmode_grayed.string			= "needs openAL & fast network:";
	soundOptionsInfo.voipmode_grayed.style				= (UI_CENTER | UI_SMALLFONT);
	soundOptionsInfo.voipmode_grayed.color				= menu_text_color;

	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voipRecordMode.generic.type		= MTYPE_SPINCONTROL;
	soundOptionsInfo.voipRecordMode.generic.name		= "Voice Recording:";
	soundOptionsInfo.voipRecordMode.generic.flags		= QMF_SMALLFONT;
	soundOptionsInfo.voipRecordMode.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.voipRecordMode.generic.id			= ID_RECORDMODE;
	soundOptionsInfo.voipRecordMode.generic.x			= POSITION_X;
	soundOptionsInfo.voipRecordMode.generic.y			= y;
	soundOptionsInfo.voipRecordMode.itemnames			= recording_modes;
	
	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voiceThresholdVAD.generic.type = MTYPE_SLIDER;
	soundOptionsInfo.voiceThresholdVAD.generic.name = "Voice Threshold:";
	soundOptionsInfo.voiceThresholdVAD.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.voiceThresholdVAD.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.voiceThresholdVAD.generic.id  = ID_VOICETHRESHOLD;
	soundOptionsInfo.voiceThresholdVAD.generic.x = POSITION_X;
	soundOptionsInfo.voiceThresholdVAD.generic.y = y;
	soundOptionsInfo.voiceThresholdVAD.minvalue = 0;
	soundOptionsInfo.voiceThresholdVAD.maxvalue = 10;
	soundOptionsInfo.voiceThresholdVAD.generic.toolTip = "If microphone option is set to open use this to adjust the sensitivity of the open microphone, mid to high setting recommended if using this method.";
	

	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voiceGainDuringCapture.generic.type = MTYPE_SLIDER;
	soundOptionsInfo.voiceGainDuringCapture.generic.name = "Capture Volume:";
	soundOptionsInfo.voiceGainDuringCapture.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.voiceGainDuringCapture.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.voiceGainDuringCapture.generic.id  = ID_GAINWHILECAPTURE;
	soundOptionsInfo.voiceGainDuringCapture.generic.x = POSITION_X;
	soundOptionsInfo.voiceGainDuringCapture.generic.y = y;
	soundOptionsInfo.voiceGainDuringCapture.minvalue = 0;
	soundOptionsInfo.voiceGainDuringCapture.maxvalue = 10;
	soundOptionsInfo.voiceGainDuringCapture.generic.toolTip = "This is the volume of audio coming out of your speakers while you are recording sound for transmission. This prevents audio feedback and echo. If you're using headphones, you don't need to turn this down.";

	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voip_restart.generic.type		= MTYPE_TEXT;
	soundOptionsInfo.voip_restart.generic.flags		= QMF_HIDDEN|QMF_PULSE;
	soundOptionsInfo.voip_restart.generic.x			= POSITION_X;
	soundOptionsInfo.voip_restart.generic.y			= y;
	soundOptionsInfo.voip_restart.string			= "Restart the game to enable Voice Chat!";
	soundOptionsInfo.voip_restart.style				= (UI_CENTER | UI_SMALLFONT);
	soundOptionsInfo.voip_restart.color				= menu_text_color;

	soundOptionsInfo.back.generic.type		= MTYPE_BITMAP;
	soundOptionsInfo.back.generic.name		= BACK0;
	soundOptionsInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.back.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.back.generic.id	    = ID_BACK;
	soundOptionsInfo.back.generic.x			= 9;
	soundOptionsInfo.back.generic.y			= 440;
	soundOptionsInfo.back.width  		    = 80;
	soundOptionsInfo.back.height  		    = 40;
	soundOptionsInfo.back.focuspic			= BACK1;
	soundOptionsInfo.back.focuspicinstead	= qtrue;

	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.graphics );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.display );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.sound );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.network );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.sfxvolume );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.musicvolume );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.musicautoswitch );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.openAL );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.voipmode);	
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.voipmode_grayed);
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.voipRecordMode);
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.voiceGainDuringCapture);
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.voiceThresholdVAD);
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.voip_restart);
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.back );

	soundOptionsInfo.sfxvolume.curvalue = trap_Cvar_VariableValue( "s_volume" ) * 10;
	soundOptionsInfo.musicvolume.curvalue = trap_Cvar_VariableValue( "s_musicvolume" ) * 10;
	soundOptionsInfo.musicautoswitch.curvalue = ( trap_Cvar_VariableValue( "wop_AutoswitchSongByNextMap" ) != 0 );
	//soundOptionsInfo.voiceThresholdVAD.curvalue = trap_Cvar_VariableValue("cl_voipVADThreshold") * 10;
	soundOptionsInfo.voiceGainDuringCapture.curvalue = trap_Cvar_VariableValue("cl_voipGainDuringCapture") * 10;
	soundOptionsInfo.voipmode.curvalue				= UI_GetCvarInt("cl_voip");
	soundOptionsInfo.voipRecordMode.curvalue = UI_GetCvarInt("cl_voipUseVAD");
	
	UI_SoundOptionsMenu_Update();
}


/*
===============
UI_SoundOptionsMenu_Cache
===============
*/
void UI_SoundOptionsMenu_Cache( void ) {
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
UI_SoundOptionsMenu
===============
*/
void UI_SoundOptionsMenu( void ) {
	UI_SoundOptionsMenu_Init();
	UI_PushMenu( &soundOptionsInfo.menu );
	Menu_SetCursorToItem( &soundOptionsInfo.menu, &soundOptionsInfo.sound );
}
