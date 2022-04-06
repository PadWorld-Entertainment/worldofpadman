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
#define GRASHICS1 "menu/buttons/graphics1"
#define DISPLAY0 "menu/buttons/display0"
#define DISPLAY1 "menu/buttons/display1"
#define SOUND0 "menu/buttons/sound0"
#define SOUND1 "menu/buttons/sound1"
#define NETWORK0 "menu/buttons/network0"
#define NETWORK1 "menu/buttons/network1"
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
#define ID_SOUNDSYSTEM 14
#define ID_QUALITY 15
#define ID_DOPPLER 16

#define ID_VOIPMODE 17
#define ID_RECORDMODE 18
#define ID_VOICETHRESHOLD 19
#define ID_GAINWHILECAPTURE 20

#define XPOSITION 180
#define DEFAULT_SDL_SND_SPEED 44100
#define UISND_SDL 0
#define UISND_OPENAL 1

static const char *quality_items[] = {"Low", "Medium", "High", NULL};
static const char *recording_items[] = {"Push to Talk", "Automatic", 0};
static const char *soundSystem_items[] = {"SDL", "OpenAL", NULL};

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
	menulist_s soundSystem;
	menulist_s quality;
	menuradiobutton_s doppler;

	menuradiobutton_s voipmode;
	menulist_s voipRecordMode;
	menutext_s voipmode_grayed;
	menuslider_s voiceThresholdVAD;
	menuslider_s voiceGainDuringCapture;

	menubitmap_s apply;
	menubitmap_s back;

	int soundSystem_original;
	int quality_original;
} soundOptionsInfo_t;

static soundOptionsInfo_t soundOptionsInfo;

static void UI_SoundOptionsMenu_Update(void) {
	// openAL and a high rate are conditions for all voip settings
	qboolean disableVoip = 25000 > trap_Cvar_VariableValue("rate");
	// voip is a condition for voip volume
	qboolean voipOff = (0 == soundOptionsInfo.voipmode.curvalue);
	// automatic recording is a condition for voip threshold
	qboolean hideVADThreshold = (0 == soundOptionsInfo.voipRecordMode.curvalue);
	int y;

	if (disableVoip) {
		soundOptionsInfo.voipmode_grayed.generic.flags &= ~QMF_HIDDEN;
		soundOptionsInfo.voipmode.generic.flags |= QMF_GRAYED;
		soundOptionsInfo.voipRecordMode.generic.flags |= QMF_GRAYED;
		soundOptionsInfo.voiceThresholdVAD.generic.flags |= QMF_GRAYED;
		soundOptionsInfo.voiceGainDuringCapture.generic.flags |= QMF_GRAYED;
	} else {
		soundOptionsInfo.voipmode_grayed.generic.flags |= QMF_HIDDEN;
		soundOptionsInfo.voipmode.generic.flags &= ~QMF_GRAYED;

		if (voipOff) {
			soundOptionsInfo.voipRecordMode.generic.flags |= QMF_GRAYED;
			soundOptionsInfo.voiceThresholdVAD.generic.flags |= QMF_GRAYED;
			soundOptionsInfo.voiceGainDuringCapture.generic.flags |= QMF_GRAYED;
		} else {
			soundOptionsInfo.voipRecordMode.generic.flags &= ~QMF_GRAYED;
			soundOptionsInfo.voiceGainDuringCapture.generic.flags &= ~QMF_GRAYED;

			if (hideVADThreshold)
				soundOptionsInfo.voiceThresholdVAD.generic.flags |= QMF_GRAYED;
			else
				soundOptionsInfo.voiceThresholdVAD.generic.flags &= ~QMF_GRAYED;
		}
	}

	// move voip settings down to make space for the voipmode_grayed text
	// or move them back up
	y = soundOptionsInfo.voipmode_grayed.generic.y;
	if (disableVoip) {
		y += BIGCHAR_HEIGHT + 2;
	}
	soundOptionsInfo.voipmode.generic.y = y;
	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voipRecordMode.generic.y = y;
	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voiceThresholdVAD.generic.y = y;
	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voiceGainDuringCapture.generic.y = y;
}
/*
=================
UI_SoundOptionsMenu_Event
=================
*/
static void UI_SoundOptionsMenu_Event(void *ptr, int event) {
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
		trap_Cvar_SetValue("s_volume", soundOptionsInfo.sfxvolume.curvalue / 10);
		break;

	case ID_MUSICVOLUME:
		trap_Cvar_SetValue("s_musicvolume", soundOptionsInfo.musicvolume.curvalue / 10);
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

	case ID_GAINWHILECAPTURE:
		trap_Cvar_SetValue("cl_voipGainDuringCapture", soundOptionsInfo.voiceGainDuringCapture.curvalue / 10);
		break;

	case ID_VOIPMODE:
		trap_Cvar_SetValue("cl_voip", (float)soundOptionsInfo.voipmode.curvalue);
		break;

	case ID_RECORDMODE:
		trap_Cvar_SetValue("cl_voipUseVAD", (float)soundOptionsInfo.voipRecordMode.curvalue);
		break;

	case ID_VOICETHRESHOLD:
		trap_Cvar_SetValue("cl_voipVADThreshold", soundOptionsInfo.voiceThresholdVAD.curvalue / 10);
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

	UI_SoundOptionsMenu_Update();
}

static void SoundOptions_UpdateMenuItems(void) {
	if (soundOptionsInfo.soundSystem.curvalue == UISND_SDL) {
		soundOptionsInfo.quality.generic.flags &= ~QMF_GRAYED;
	} else {
		soundOptionsInfo.quality.generic.flags |= QMF_GRAYED;
	}

	soundOptionsInfo.apply.generic.flags |= QMF_HIDDEN | QMF_INACTIVE;

	if (soundOptionsInfo.soundSystem_original != soundOptionsInfo.soundSystem.curvalue) {
		soundOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	}
	if (soundOptionsInfo.quality_original != soundOptionsInfo.quality.curvalue) {
		soundOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	}
}

static void SoundOptions_MenuDraw(void) {
	SoundOptions_UpdateMenuItems();

	Menu_Draw(&soundOptionsInfo.menu);
}

/*
===============
UI_SoundOptionsMenu_Init
===============
*/
static void UI_SoundOptionsMenu_Init(void) {
	int y;
	int speed;

	memset(&soundOptionsInfo, 0, sizeof(soundOptionsInfo));

	UI_SoundOptionsMenu_Cache();
	soundOptionsInfo.menu.wrapAround = qtrue;
	soundOptionsInfo.menu.fullscreen = qtrue;
	soundOptionsInfo.menu.draw = SoundOptions_MenuDraw;
	soundOptionsInfo.menu.bgparts = BGP_SYSTEM | BGP_MENUFX;

	soundOptionsInfo.graphics.generic.type = MTYPE_BITMAP;
	soundOptionsInfo.graphics.generic.name = GRAPHICS0;
	soundOptionsInfo.graphics.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	soundOptionsInfo.graphics.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.graphics.generic.id = ID_GRAPHICS;
	soundOptionsInfo.graphics.generic.x = 16;
	soundOptionsInfo.graphics.generic.y = 37;
	soundOptionsInfo.graphics.width = 160;
	soundOptionsInfo.graphics.height = 40;
	soundOptionsInfo.graphics.focuspic = GRASHICS1;
	soundOptionsInfo.graphics.focuspicinstead = qtrue;

	soundOptionsInfo.display.generic.type = MTYPE_BITMAP;
	soundOptionsInfo.display.generic.name = DISPLAY0;
	soundOptionsInfo.display.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	soundOptionsInfo.display.generic.callback = UI_SoundOptionsMenu_Event;
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
	soundOptionsInfo.sound.generic.callback = UI_SoundOptionsMenu_Event;
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
	soundOptionsInfo.network.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.network.generic.id = ID_NETWORK;
	soundOptionsInfo.network.generic.x = 142;
	soundOptionsInfo.network.generic.y = 82;
	soundOptionsInfo.network.width = 160;
	soundOptionsInfo.network.height = 40;
	soundOptionsInfo.network.focuspic = NETWORK1;
	soundOptionsInfo.network.focuspicinstead = qtrue;

	y = 180 + 2 * (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.sfxvolume.generic.type = MTYPE_SLIDER;
	soundOptionsInfo.sfxvolume.generic.name = "Effects Volume:";
	soundOptionsInfo.sfxvolume.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.sfxvolume.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.sfxvolume.generic.id = ID_EFFECTSVOLUME;
	soundOptionsInfo.sfxvolume.generic.x = XPOSITION;
	soundOptionsInfo.sfxvolume.generic.y = y;
	soundOptionsInfo.sfxvolume.minvalue = 0;
	soundOptionsInfo.sfxvolume.maxvalue = 10;

	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.musicvolume.generic.type = MTYPE_SLIDER;
	soundOptionsInfo.musicvolume.generic.name = "Music Volume:";
	soundOptionsInfo.musicvolume.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.musicvolume.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.musicvolume.generic.id = ID_MUSICVOLUME;
	soundOptionsInfo.musicvolume.generic.x = XPOSITION;
	soundOptionsInfo.musicvolume.generic.y = y;
	soundOptionsInfo.musicvolume.minvalue = 0;
	soundOptionsInfo.musicvolume.maxvalue = 10;

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.musicautoswitch.generic.type = MTYPE_RADIOBUTTON;
	soundOptionsInfo.musicautoswitch.generic.name = "Auto Switch Song:";
	soundOptionsInfo.musicautoswitch.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.musicautoswitch.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.musicautoswitch.generic.id = ID_MUSICAUTOSWITCH;
	soundOptionsInfo.musicautoswitch.generic.x = XPOSITION;
	soundOptionsInfo.musicautoswitch.generic.y = y;
	soundOptionsInfo.musicautoswitch.generic.toolTip = "Enable to automatically switch to the next song on map change, "
													   "if set to off current song will restart on map change.";

	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.automute.generic.type = MTYPE_RADIOBUTTON;
	soundOptionsInfo.automute.generic.name = "Auto Mute Sound:";
	soundOptionsInfo.automute.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	soundOptionsInfo.automute.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.automute.generic.id = ID_AUTOMUTE;
	soundOptionsInfo.automute.generic.x = XPOSITION;
	soundOptionsInfo.automute.generic.y = y;
	soundOptionsInfo.automute.generic.toolTip = "Enable this option to automatically mute the sound when the game window "
												"loses focus or is minimized.";

	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.soundSystem.generic.type = MTYPE_SPINCONTROL;
	soundOptionsInfo.soundSystem.generic.name = "Sound System:";
	soundOptionsInfo.soundSystem.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	soundOptionsInfo.soundSystem.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.soundSystem.generic.id = ID_SOUNDSYSTEM;
	soundOptionsInfo.soundSystem.generic.x = XPOSITION;
	soundOptionsInfo.soundSystem.generic.y = y;
	soundOptionsInfo.soundSystem.itemnames = soundSystem_items;

	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.quality.generic.type = MTYPE_SPINCONTROL;
	soundOptionsInfo.quality.generic.name = "SDL Sound Quality:";
	soundOptionsInfo.quality.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	soundOptionsInfo.quality.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.quality.generic.id = ID_QUALITY;
	soundOptionsInfo.quality.generic.x = XPOSITION;
	soundOptionsInfo.quality.generic.y = y;
	soundOptionsInfo.quality.itemnames = quality_items;

	y += (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.doppler.generic.type = MTYPE_RADIOBUTTON;
	soundOptionsInfo.doppler.generic.name = "Doppler Effect:";
	soundOptionsInfo.doppler.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.doppler.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.doppler.generic.id = ID_DOPPLER;
	soundOptionsInfo.doppler.generic.x = XPOSITION;
	soundOptionsInfo.doppler.generic.y = y;

	y += BIGCHAR_HEIGHT * 2 + 2;
	soundOptionsInfo.voipmode.generic.type = MTYPE_RADIOBUTTON;
	soundOptionsInfo.voipmode.generic.name = "Voice Chat:";
	soundOptionsInfo.voipmode.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.voipmode.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.voipmode.generic.id = ID_VOIPMODE;
	soundOptionsInfo.voipmode.generic.x = XPOSITION;
	soundOptionsInfo.voipmode.generic.y = y;

	soundOptionsInfo.voipmode_grayed.generic.type = MTYPE_TEXT;
	soundOptionsInfo.voipmode_grayed.generic.flags = QMF_PULSE;
	soundOptionsInfo.voipmode_grayed.generic.x = XPOSITION - 35;
	soundOptionsInfo.voipmode_grayed.generic.y = y;
	soundOptionsInfo.voipmode_grayed.string = "Needs LAN/Cable/xDSL Network!";
	soundOptionsInfo.voipmode_grayed.style = (UI_CENTER | UI_SMALLFONT);
	soundOptionsInfo.voipmode_grayed.color = menu_text_color;

	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voipRecordMode.generic.type = MTYPE_SPINCONTROL;
	soundOptionsInfo.voipRecordMode.generic.name = "Voice Recording:";
	soundOptionsInfo.voipRecordMode.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.voipRecordMode.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.voipRecordMode.generic.id = ID_RECORDMODE;
	soundOptionsInfo.voipRecordMode.generic.x = XPOSITION;
	soundOptionsInfo.voipRecordMode.generic.y = y;
	soundOptionsInfo.voipRecordMode.itemnames = recording_items;

	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voiceThresholdVAD.generic.type = MTYPE_SLIDER;
	soundOptionsInfo.voiceThresholdVAD.generic.name = "Voice Threshold:";
	soundOptionsInfo.voiceThresholdVAD.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.voiceThresholdVAD.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.voiceThresholdVAD.generic.id = ID_VOICETHRESHOLD;
	soundOptionsInfo.voiceThresholdVAD.generic.x = XPOSITION;
	soundOptionsInfo.voiceThresholdVAD.generic.y = y;
	soundOptionsInfo.voiceThresholdVAD.minvalue = 0;
	soundOptionsInfo.voiceThresholdVAD.maxvalue = 10;
	soundOptionsInfo.voiceThresholdVAD.generic.toolTip =
		"If microphone option is set to open use this to adjust the sensitivity of the open microphone, mid to high "
		"setting recommended if using this method.";

	y += BIGCHAR_HEIGHT + 2;
	soundOptionsInfo.voiceGainDuringCapture.generic.type = MTYPE_SLIDER;
	soundOptionsInfo.voiceGainDuringCapture.generic.name = "Capture Volume:";
	soundOptionsInfo.voiceGainDuringCapture.generic.flags = QMF_SMALLFONT;
	soundOptionsInfo.voiceGainDuringCapture.generic.callback = UI_SoundOptionsMenu_Event;
	soundOptionsInfo.voiceGainDuringCapture.generic.id = ID_GAINWHILECAPTURE;
	soundOptionsInfo.voiceGainDuringCapture.generic.x = XPOSITION;
	soundOptionsInfo.voiceGainDuringCapture.generic.y = y;
	soundOptionsInfo.voiceGainDuringCapture.minvalue = 0;
	soundOptionsInfo.voiceGainDuringCapture.maxvalue = 10;
	soundOptionsInfo.voiceGainDuringCapture.generic.toolTip =
		"This is the volume of audio coming out of your speakers while you are recording sound for transmission. This "
		"prevents audio feedback and echo. If you're using headphones, you don't need to turn this down.";

	soundOptionsInfo.back.generic.type = MTYPE_BITMAP;
	soundOptionsInfo.back.generic.name = BACK0;
	soundOptionsInfo.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	soundOptionsInfo.back.generic.callback = UI_SoundOptionsMenu_Event;
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
	soundOptionsInfo.apply.generic.callback = UI_SoundOptionsMenu_Event;
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
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.soundSystem);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.quality);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.doppler);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.voipmode);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.voipmode_grayed);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.voipRecordMode);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.voiceThresholdVAD);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.voiceGainDuringCapture);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.back);
	Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.apply);

	soundOptionsInfo.sfxvolume.curvalue = trap_Cvar_VariableValue("s_volume") * 10;
	soundOptionsInfo.musicvolume.curvalue = trap_Cvar_VariableValue("s_musicvolume") * 10;

	if (trap_Cvar_VariableValue("s_muteWhenUnfocused") == 1 || trap_Cvar_VariableValue("s_muteWhenMinimized") == 1) {
		soundOptionsInfo.automute.curvalue = 1;
	} else {
		soundOptionsInfo.automute.curvalue = 0;
	}

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

	soundOptionsInfo.musicautoswitch.curvalue = (UI_GetCvarInt("wop_AutoswitchSongByNextMap") != 0);
	soundOptionsInfo.doppler.curvalue = (UI_GetCvarInt("s_doppler") != 0);
	// soundOptionsInfo.voiceThresholdVAD.curvalue = trap_Cvar_VariableValue("cl_voipVADThreshold") * 10;
	soundOptionsInfo.voiceGainDuringCapture.curvalue = trap_Cvar_VariableValue("cl_voipGainDuringCapture") * 10;
	soundOptionsInfo.voipmode.curvalue = UI_GetCvarInt("cl_voip");
	soundOptionsInfo.voipRecordMode.curvalue = UI_GetCvarInt("cl_voipUseVAD");

	UI_SoundOptionsMenu_Update();
}

/*
===============
UI_SoundOptionsMenu_Cache
===============
*/
void UI_SoundOptionsMenu_Cache(void) {
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
	trap_R_RegisterShaderNoMip(ACCEPT0);
	trap_R_RegisterShaderNoMip(ACCEPT1);
}

/*
===============
UI_SoundOptionsMenu
===============
*/
void UI_SoundOptionsMenu(void) {
	UI_SoundOptionsMenu_Init();
	UI_PushMenu(&soundOptionsInfo.menu);
	Menu_SetCursorToItem(&soundOptionsInfo.menu, &soundOptionsInfo.sound);
}
