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

SOUND OPTIONS MENU

=======================================================================
*/

#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define GRAPHICS0 "menu/buttons/graphics0"
#define GRAPHICS1 "menu/buttons/graphics1"
#define DISPLAY0 "menu/buttons/display0"
#define DISPLAY1 "menu/buttons/display1"
#define SOUND0 "menu/buttons/sound0"
#define SOUND1 "menu/buttons/sound1"
#define NETWORK0 "menu/buttons/netvoip0"
#define NETWORK1 "menu/buttons/netvoip1"
#define ACCEPT0 "menu/buttons/accept"
#define ACCEPT1 "menu/buttons/accept"

#define ID_GRAPHICS 100
#define ID_DISPLAY 101
#define ID_SOUND 102
#define ID_NETWORK 103
#define ID_BACK 104
#define ID_APPLY 105

#define ID_EFFECTSVOLUME 10
#define ID_MUSICVOLUME 11
#define ID_MUSICAUTOSWITCH 12
#define ID_AUTOMUTE 13
#define ID_DOPPLER 14
#define ID_SOUNDSYSTEM 15
#define ID_OUTPUTDEVICE 16
#define ID_INPUTDEVICE 17
#define ID_QUALITY 18
#define ID_ALPRECACHE 19
#define ID_ALSOURCES 20
#define ID_ALDOPPLERFACTOR 21
#define ID_ALDOPPLERSPEED 22

#define DEFAULT_SDL_SND_SPEED 44100
#define UISND_SDL 0
#define UISND_OPENAL 1

#define XPOSITION 180
#define YPOSITION 180 + 36

static const char *quality_items[] = {"Low", "Medium", "High", NULL};
static const char *soundSystem_items[] = {"SDL", "OpenAL", NULL};
static const char *alSources_items[] = {"Low (32)", "Medium (64)", "High (96)", "Maximum (128)", NULL};

typedef struct {
	menuframework_s menu;

	menubitmap_s graphics;
	menubitmap_s display;
	menubitmap_s sound;
	menubitmap_s network;

	menuslider_s sfxvolume;
	menuslider_s musicvolume;
	menuradiobutton_s automute;
	menuradiobutton_s musicautoswitch;
	menuradiobutton_s doppler;
	menulist_s soundSystem;
	menulist_s outputdevice;
	menulist_s inputdevice;
	menulist_s quality;
	menuradiobutton_s alprecache;
	menulist_s alsources;
	menuslider_s aldopplerfactor;
	menuslider_s aldopplerspeed;

	menubitmap_s apply;
	menubitmap_s back;

	int soundSystem_original;
	int quality_original;

} soundOptionsInfo_t;

static soundOptionsInfo_t soundOptionsInfo;

/*
=================
UI_SoundOptions_SetMenuItems
=================
*/
static void UI_SoundOptions_SetMenuItems(void) {
	int speed;
	int sources;

	soundOptionsInfo.sfxvolume.curvalue = trap_Cvar_VariableValue("s_volume") * 100;
	soundOptionsInfo.musicvolume.curvalue = trap_Cvar_VariableValue("s_musicvolume") * 100;
	soundOptionsInfo.musicautoswitch.curvalue = (UI_GetCvarInt("wop_AutoswitchSongByNextMap") != 0);

	if (trap_Cvar_VariableValue("s_muteWhenUnfocused") || trap_Cvar_VariableValue("s_muteWhenMinimized")) {
		soundOptionsInfo.automute.curvalue = 1;
	} else {
		soundOptionsInfo.automute.curvalue = 0;
	}

	soundOptionsInfo.doppler.curvalue = (UI_GetCvarInt("s_doppler") != 0);

	if (trap_Cvar_VariableValue("s_useOpenAL"))
		soundOptionsInfo.soundSystem_original = UISND_OPENAL;
	else
		soundOptionsInfo.soundSystem_original = UISND_SDL;

	soundOptionsInfo.soundSystem.curvalue = soundOptionsInfo.soundSystem_original;

	speed = trap_Cvar_VariableValue("s_sdlSpeed");
	if (!speed) // Check for default
		speed = DEFAULT_SDL_SND_SPEED;

	if (speed <= 11025)
		soundOptionsInfo.quality_original = 0;
	else if (speed <= 22050)
		soundOptionsInfo.quality_original = 1;
	else // 44100
		soundOptionsInfo.quality_original = 2;

	soundOptionsInfo.quality.curvalue = soundOptionsInfo.quality_original;

	soundOptionsInfo.alprecache.curvalue = trap_Cvar_VariableValue("s_alPrecache");

	sources = trap_Cvar_VariableValue("s_alSources");
	if (sources <= 32) {
		soundOptionsInfo.alsources.curvalue = 0;
	} else if (sources <= 64) {
		soundOptionsInfo.alsources.curvalue = 1;
	} else if (sources <= 96) {
		soundOptionsInfo.alsources.curvalue = 2;
	} else { // 128
		soundOptionsInfo.alsources.curvalue = 3;
	}

	soundOptionsInfo.aldopplerfactor.curvalue = trap_Cvar_VariableValue("s_alDopplerFactor");
	soundOptionsInfo.aldopplerspeed.curvalue = trap_Cvar_VariableValue("s_alDopplerSpeed");

}

/*
=================
UI_SoundOptions_UpdateMenuItems
=================
*/
static void UI_SoundOptions_UpdateMenuItems(void) {

	// SDL enabled is a condition to show SDL and to hide AL options 
	if (soundOptionsInfo.soundSystem.curvalue == UISND_SDL) {
		soundOptionsInfo.quality.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
		soundOptionsInfo.alprecache.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
		soundOptionsInfo.alsources.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
		soundOptionsInfo.aldopplerfactor.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
		soundOptionsInfo.aldopplerspeed.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
	} else {
		soundOptionsInfo.quality.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
			soundOptionsInfo.alprecache.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
			soundOptionsInfo.alsources.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
			soundOptionsInfo.aldopplerfactor.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
			soundOptionsInfo.aldopplerspeed.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);

		// Doppler effect enabled is a condition to activate doppler effect options
		if (soundOptionsInfo.doppler.curvalue != 0) {
			soundOptionsInfo.aldopplerfactor.generic.flags &= ~QMF_GRAYED;
			soundOptionsInfo.aldopplerspeed.generic.flags &= ~QMF_GRAYED;
		} else {
			soundOptionsInfo.aldopplerfactor.generic.flags |= QMF_GRAYED;
			soundOptionsInfo.aldopplerspeed.generic.flags |= QMF_GRAYED;
		}
	}

	soundOptionsInfo.apply.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);

	if (soundOptionsInfo.soundSystem_original != soundOptionsInfo.soundSystem.curvalue) {
		soundOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	}
	if (soundOptionsInfo.quality_original != soundOptionsInfo.quality.curvalue) {
		soundOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	}

}

/*
=================
UI_SoundOptions_Event
=================
*/
static void UI_SoundOptions_Event(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
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
		trap_Cvar_SetValue("s_volume", (float)((int)soundOptionsInfo.sfxvolume.curvalue) / 100);
		break;

	case ID_MUSICVOLUME:
		trap_Cvar_SetValue("s_musicvolume", (float)((int)soundOptionsInfo.musicvolume.curvalue) / 100);
		break;

	case ID_AUTOMUTE:
		switch (soundOptionsInfo.automute.curvalue) {
		case 0:
			trap_Cvar_SetValue("s_muteWhenUnfocused", 0);
			trap_Cvar_SetValue("s_muteWhenMinimized", 0);
			break;
		case 1:
			trap_Cvar_SetValue("s_muteWhenUnfocused", 1);
			trap_Cvar_SetValue("s_muteWhenMinimized", 1);
			break;
		}

	case ID_MUSICAUTOSWITCH:
		trap_Cvar_SetValue("wop_AutoswitchSongByNextMap", (float)soundOptionsInfo.musicautoswitch.curvalue);
		break;

	case ID_DOPPLER:
		trap_Cvar_SetValue("s_doppler", (float)soundOptionsInfo.doppler.curvalue);
		break;

	case ID_OUTPUTDEVICE:
		break;

	case ID_INPUTDEVICE:
		break;

	case ID_ALPRECACHE:
		trap_Cvar_SetValue("s_alPrecache", soundOptionsInfo.alprecache.curvalue);
		break;

	case ID_ALSOURCES:
		switch (soundOptionsInfo.alsources.curvalue) {
		case 0:
			trap_Cvar_SetValue("s_alSources", 32);
			break;
		case 1:
			trap_Cvar_SetValue("s_alSources", 64);
			break;
		case 2:
			trap_Cvar_SetValue("s_alSources", 96);
			break;
		case 3:
			trap_Cvar_SetValue("s_alSources", 128);
			break;
		}

	case ID_ALDOPPLERFACTOR:
		trap_Cvar_SetValue("s_alDopplerFactor", (int)soundOptionsInfo.aldopplerfactor.curvalue);
		break;

	case ID_ALDOPPLERSPEED:
		trap_Cvar_SetValue("s_alDopplerSpeed", (int)soundOptionsInfo.aldopplerspeed.curvalue);
		break;

	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_APPLY:
		// Check if something changed that requires the sound system to be restarted.
		if (soundOptionsInfo.quality_original != soundOptionsInfo.quality.curvalue ||
			soundOptionsInfo.soundSystem_original != soundOptionsInfo.soundSystem.curvalue) {
			int speed;

			switch (soundOptionsInfo.quality.curvalue) {
			default:
			case 0:
				speed = 11025;
				break;
			case 1:
				speed = 22050;
				break;
			case 2:
				speed = 44100;
				break;
			}

			if (speed == DEFAULT_SDL_SND_SPEED)
				speed = 0;

			trap_Cvar_SetValue("s_sdlSpeed", speed);
			soundOptionsInfo.quality_original = soundOptionsInfo.quality.curvalue;

			trap_Cvar_SetValue("s_useOpenAL", (soundOptionsInfo.soundSystem.curvalue == UISND_OPENAL));
			soundOptionsInfo.soundSystem_original = soundOptionsInfo.soundSystem.curvalue;

			UI_ForceMenuOff();
			trap_Cmd_ExecuteText(EXEC_APPEND, "snd_restart\n");
		}
		break;
	}

}

/*
===============
UI_SoundOptions_MenuDraw
===============
*/
static void UI_SoundOptions_MenuDraw(void) {
	UI_SoundOptions_UpdateMenuItems();
	Menu_Draw(&soundOptionsInfo.menu);
}

/*
===============
UI_SoundOptions_MenuInit
===============
*/
static void UI_SoundOptions_MenuInit(void) {
	int y;

	memset(&soundOptionsInfo, 0, sizeof(soundOptionsInfo));

	UI_SoundOptions_Cache();
	soundOptionsInfo.menu.wrapAround = qtrue;
	soundOptionsInfo.menu.fullscreen = qtrue;
	soundOptionsInfo.menu.draw = UI_SoundOptions_MenuDraw;
	soundOptionsInfo.menu.bgparts = BGP_SYSTEM | BGP_MENUFX;

	soundOptionsInfo.graphics.generic.type = MTYPE_BITMAP;
	soundOptionsInfo.graphics.generic.name = GRAPHICS0;
	soundOptionsInfo.graphics.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	soundOptionsInfo.graphics.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.graphics.generic.id = ID_GRAPHICS;
	soundOptionsInfo.graphics.generic.x = 16;
	soundOptionsInfo.graphics.generic.y = 37;
	soundOptionsInfo.graphics.width = 160;
	soundOptionsInfo.graphics.height = 40;
	soundOptionsInfo.graphics.focuspic = GRAPHICS1;
	soundOptionsInfo.graphics.focuspicinstead = qtrue;

	soundOptionsInfo.display.generic.type = MTYPE_BITMAP;
	soundOptionsInfo.display.generic.name = DISPLAY0;
	soundOptionsInfo.display.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	soundOptionsInfo.display.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.display.generic.id = ID_DISPLAY;
	soundOptionsInfo.display.generic.x = 169;
	soundOptionsInfo.display.generic.y = 30;
	soundOptionsInfo.display.width = 120;
	soundOptionsInfo.display.height = 40;
	soundOptionsInfo.display.focuspic = DISPLAY1;
	soundOptionsInfo.display.focuspicinstead = qtrue;

	soundOptionsInfo.sound.generic.type = MTYPE_BITMAP;
	soundOptionsInfo.sound.generic.name = SOUND0;
	soundOptionsInfo.sound.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT;
	soundOptionsInfo.sound.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.sound.generic.id = ID_SOUND;
	soundOptionsInfo.sound.generic.x = 36;
	soundOptionsInfo.sound.generic.y = 79;
	soundOptionsInfo.sound.width = 120;
	soundOptionsInfo.sound.height = 40;
	soundOptionsInfo.sound.focuspic = SOUND1;
	soundOptionsInfo.sound.focuspicinstead = qtrue;

	soundOptionsInfo.network.generic.type = MTYPE_BITMAP;
	soundOptionsInfo.network.generic.name = NETWORK0;
	soundOptionsInfo.network.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	soundOptionsInfo.network.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.network.generic.id = ID_NETWORK;
	soundOptionsInfo.network.generic.x = 142;
	soundOptionsInfo.network.generic.y = 82;
	soundOptionsInfo.network.width = 160;
	soundOptionsInfo.network.height = 40;
	soundOptionsInfo.network.focuspic = NETWORK1;
	soundOptionsInfo.network.focuspicinstead = qtrue;

	y = YPOSITION;
	soundOptionsInfo.sfxvolume.generic.type = MTYPE_SLIDER;
	soundOptionsInfo.sfxvolume.generic.name = "Effects Volume:";
	soundOptionsInfo.sfxvolume.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.sfxvolume.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.sfxvolume.generic.id = ID_EFFECTSVOLUME;
	soundOptionsInfo.sfxvolume.generic.x = XPOSITION;
	soundOptionsInfo.sfxvolume.generic.y = y;
	soundOptionsInfo.sfxvolume.minvalue = 0;
	soundOptionsInfo.sfxvolume.maxvalue = 100;

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.musicvolume.generic.type = MTYPE_SLIDER;
	soundOptionsInfo.musicvolume.generic.name = "Music Volume:";
	soundOptionsInfo.musicvolume.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.musicvolume.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.musicvolume.generic.id = ID_MUSICVOLUME;
	soundOptionsInfo.musicvolume.generic.x = XPOSITION;
	soundOptionsInfo.musicvolume.generic.y = y;
	soundOptionsInfo.musicvolume.minvalue = 0;
	soundOptionsInfo.musicvolume.maxvalue = 100;

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.musicautoswitch.generic.type = MTYPE_RADIOBUTTON;
	soundOptionsInfo.musicautoswitch.generic.name = "Auto Switch Song:";
	soundOptionsInfo.musicautoswitch.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.musicautoswitch.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.musicautoswitch.generic.id = ID_MUSICAUTOSWITCH;
	soundOptionsInfo.musicautoswitch.generic.x = XPOSITION;
	soundOptionsInfo.musicautoswitch.generic.y = y;
	soundOptionsInfo.musicautoswitch.generic.toolTip = "Enable to automatically switch to the next song on map change, "
													   "if set to off current song will restart on map change.";

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.automute.generic.type = MTYPE_RADIOBUTTON;
	soundOptionsInfo.automute.generic.name = "Auto Mute Sound:";
	soundOptionsInfo.automute.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	soundOptionsInfo.automute.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.automute.generic.id = ID_AUTOMUTE;
	soundOptionsInfo.automute.generic.x = XPOSITION;
	soundOptionsInfo.automute.generic.y = y;
	soundOptionsInfo.automute.generic.toolTip =
		"Enable this option to automatically mute the sound when the game window "
		"loses focus or is minimized.";

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.doppler.generic.type = MTYPE_RADIOBUTTON;
	soundOptionsInfo.doppler.generic.name = "Doppler Effect:";
	soundOptionsInfo.doppler.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.doppler.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.doppler.generic.id = ID_DOPPLER;
	soundOptionsInfo.doppler.generic.x = XPOSITION;
	soundOptionsInfo.doppler.generic.y = y;

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.soundSystem.generic.type = MTYPE_SPINCONTROL;
	soundOptionsInfo.soundSystem.generic.name = "Sound System:";
	soundOptionsInfo.soundSystem.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	soundOptionsInfo.soundSystem.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.soundSystem.generic.id = ID_SOUNDSYSTEM;
	soundOptionsInfo.soundSystem.generic.x = XPOSITION;
	soundOptionsInfo.soundSystem.generic.y = y;
	soundOptionsInfo.soundSystem.itemnames = soundSystem_items;

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.outputdevice.generic.type = MTYPE_SPINCONTROL;
	soundOptionsInfo.outputdevice.generic.name = "Output Device:";
	soundOptionsInfo.outputdevice.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	soundOptionsInfo.outputdevice.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.outputdevice.generic.id = ID_OUTPUTDEVICE;
	soundOptionsInfo.outputdevice.generic.x = XPOSITION;
	soundOptionsInfo.outputdevice.generic.y = y;
//	soundOptionsInfo.outputdevice.itemnames = trap_Cvar_VariableValue("s_alAvailableDevices");		TODO! It's not that simple. ;)

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.inputdevice.generic.type = MTYPE_SPINCONTROL;
	soundOptionsInfo.inputdevice.generic.name = "Input Device:";
	soundOptionsInfo.inputdevice.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	soundOptionsInfo.inputdevice.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.inputdevice.generic.id = ID_INPUTDEVICE;
	soundOptionsInfo.inputdevice.generic.x = XPOSITION;
	soundOptionsInfo.inputdevice.generic.y = y;
//	soundOptionsInfo.inputdevice.itemnames = trap_Cvar_VariableValue("s_alAvailableInputDevices");	TODO! It's not that simple. ;)

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.quality.generic.type = MTYPE_SPINCONTROL;
	soundOptionsInfo.quality.generic.name = "Sound Quality:";
	soundOptionsInfo.quality.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	soundOptionsInfo.quality.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.quality.generic.id = ID_QUALITY;
	soundOptionsInfo.quality.generic.x = XPOSITION;
	soundOptionsInfo.quality.generic.y = y;
	soundOptionsInfo.quality.itemnames = quality_items;

	soundOptionsInfo.alprecache.generic.type = MTYPE_RADIOBUTTON;
	soundOptionsInfo.alprecache.generic.name = "Precache Sounds:";
	soundOptionsInfo.alprecache.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.alprecache.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.alprecache.generic.id = ID_ALPRECACHE;
	soundOptionsInfo.alprecache.generic.x = XPOSITION;
	soundOptionsInfo.alprecache.generic.y = y;

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.alsources.generic.type = MTYPE_SPINCONTROL;
	soundOptionsInfo.alsources.generic.name = "Sources Allocation:";
	soundOptionsInfo.alsources.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	soundOptionsInfo.alsources.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.alsources.generic.id = ID_ALSOURCES;
	soundOptionsInfo.alsources.generic.x = XPOSITION;
	soundOptionsInfo.alsources.generic.y = y;
	soundOptionsInfo.alsources.itemnames = alSources_items;

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.aldopplerfactor.generic.type = MTYPE_SLIDER;
	soundOptionsInfo.aldopplerfactor.generic.name = "Doppler Factor:";
	soundOptionsInfo.aldopplerfactor.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.aldopplerfactor.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.aldopplerfactor.generic.id = ID_ALDOPPLERFACTOR;
	soundOptionsInfo.aldopplerfactor.generic.x = XPOSITION;
	soundOptionsInfo.aldopplerfactor.generic.y = y;
	soundOptionsInfo.aldopplerfactor.minvalue = 0;
	soundOptionsInfo.aldopplerfactor.maxvalue = 5;

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.aldopplerspeed.generic.type = MTYPE_SLIDER;
	soundOptionsInfo.aldopplerspeed.generic.name = "Doppler Speed:";
	soundOptionsInfo.aldopplerspeed.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.aldopplerspeed.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.aldopplerspeed.generic.id = ID_ALDOPPLERSPEED;
	soundOptionsInfo.aldopplerspeed.generic.x = XPOSITION;
	soundOptionsInfo.aldopplerspeed.generic.y = y;
	soundOptionsInfo.aldopplerspeed.minvalue = 0;
	soundOptionsInfo.aldopplerspeed.maxvalue = 20000;

	soundOptionsInfo.back.generic.type = MTYPE_BITMAP;
	soundOptionsInfo.back.generic.name = BACK0;
	soundOptionsInfo.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	soundOptionsInfo.back.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.back.generic.id = ID_BACK;
	soundOptionsInfo.back.generic.x = 8;
	soundOptionsInfo.back.generic.y = 440;
	soundOptionsInfo.back.width = 80;
	soundOptionsInfo.back.height = 40;
	soundOptionsInfo.back.focuspic = BACK1;
	soundOptionsInfo.back.focuspicinstead = qtrue;

	soundOptionsInfo.apply.generic.type = MTYPE_BITMAP;
	soundOptionsInfo.apply.generic.name = ACCEPT0;
	soundOptionsInfo.apply.generic.flags = QMF_PULSEIFFOCUS | QMF_HIDDEN | QMF_INACTIVE;
	soundOptionsInfo.apply.generic.callback = UI_SoundOptions_Event;
	soundOptionsInfo.apply.generic.id = ID_APPLY;
	soundOptionsInfo.apply.generic.x = 512;
	soundOptionsInfo.apply.generic.y = 440;
	soundOptionsInfo.apply.width = 120;
	soundOptionsInfo.apply.height = 40;
	soundOptionsInfo.apply.focuspic = ACCEPT1;

	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.graphics);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.display);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.sound);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.network);

	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.sfxvolume);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.musicvolume);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.musicautoswitch);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.automute);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.doppler);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.soundSystem);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.outputdevice);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.inputdevice);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.quality);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.alprecache);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.alsources);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.aldopplerfactor);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.aldopplerspeed);

	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.back);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.apply);

	UI_SoundOptions_SetMenuItems();

}

/*
===============
UI_SoundOptions_Cache
===============
*/
void UI_SoundOptions_Cache(void) {
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
	trap_R_RegisterShaderNoMip(GRAPHICS0);
	trap_R_RegisterShaderNoMip(GRAPHICS1);
	trap_R_RegisterShaderNoMip(DISPLAY0);
	trap_R_RegisterShaderNoMip(DISPLAY1);
	trap_R_RegisterShaderNoMip(SOUND0);
	trap_R_RegisterShaderNoMip(SOUND1);
	trap_R_RegisterShaderNoMip(NETWORK0);
	trap_R_RegisterShaderNoMip(NETWORK1);
	trap_R_RegisterShaderNoMip(ACCEPT0);
	trap_R_RegisterShaderNoMip(ACCEPT1);
}

/*
===============
UI_SoundOptionsMenu
===============
*/
void UI_SoundOptionsMenu(void) {
	UI_SoundOptions_MenuInit();
	UI_PushMenu(&soundOptionsInfo.menu);
	Menu_SetCursorToItem(&soundOptionsInfo.menu, &soundOptionsInfo.sound);
}
