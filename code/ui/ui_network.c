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

NETWORK OPTIONS MENU

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

#define ID_GRAPHICS 100
#define ID_DISPLAY 101
#define ID_SOUND 102
#define ID_NETWORK 103
#define ID_BACK 104

#define ID_RATE 10
#define ID_ALLOWDOWNLOAD 11
#define ID_VOIPMODE 12
#define ID_VOIPVADMODE 13
#define ID_VOIPVADTHRESHOLD 14
#define ID_VOIPCAPTUREGAIN 15
#define ID_VOIPCAPTUREMULT 16
#define ID_VOIPSENDTARGET 17
#define ID_MUMBLESCALE 18


#define XPOSITION 180

static const char *rate_items[] = {"Modem", "ISDN", "LAN/Cable/xDSL", NULL};
static const char *voipMode_items[] = {"Off", "Built-in", "Mumble", NULL};
static const char *capture_items[] = {"Push to Talk", "Automatic", NULL};
static const char *sendTarget_items[] = {"all", "none", "attacker", "crosshair", "spatial", NULL};
static const char *sendTarget_custom[] = {"Custom Setting", NULL};

typedef struct {
	menuframework_s menu;

	menubitmap_s graphics;
	menubitmap_s display;
	menubitmap_s sound;
	menubitmap_s network;

	menulist_s rate;
	menuradiobutton_s allowDownload;
	menutext_s voipHint;
	menulist_s voipMode;
	menulist_s voipVADmode;
	menuslider_s voipVADthreshold;
	menuslider_s voipGainDuringCapture;
	menuslider_s voipCaptureMult;
	menulist_s voipSendTarget;
	menuslider_s mumbleScale;
	
	menubitmap_s back;

	qboolean voipCustomSendTarget;

} networkOptionsInfo_t;

static networkOptionsInfo_t networkOptionsInfo;

/*
=================
UI_NetworkOptions_SendTargetUpdate
=================
*/
static void UI_NetworkOptions_SendTargetUpdate(void) {
	int i;
	const char **s;
	char voipSendTarget[MAX_CVAR_VALUE_STRING];

	trap_Cvar_VariableStringBuffer("cl_voipSendTarget", voipSendTarget, sizeof(voipSendTarget));

	for (i = 0, s = sendTarget_items; *s != NULL; ++s, ++i) {
		if (!strcmp(*s, voipSendTarget)) {
			networkOptionsInfo.voipCustomSendTarget = qfalse;
			networkOptionsInfo.voipSendTarget.curvalue = i;
			networkOptionsInfo.voipSendTarget.generic.flags &= ~QMF_GRAYED;
			networkOptionsInfo.voipSendTarget.itemnames = sendTarget_items;
			networkOptionsInfo.voipSendTarget.generic.toolTip =
				"'All' to broadcast to everyone. "
				"'None' to send to no one. "
				"'Attacker' to send to the last person that hit you. "
				"'Crosshair' to send to the people currently in your crosshair. "
				"'Spatial' to talk to all people in hearing range. "
				"NOTE: See ingame VoIP chat menu for more options.";
			SpinControl_Init(&networkOptionsInfo.voipSendTarget);
			return;
		}
	}
	networkOptionsInfo.voipSendTarget.itemnames = sendTarget_custom;
	networkOptionsInfo.voipSendTarget.generic.flags |= QMF_GRAYED;
	networkOptionsInfo.voipCustomSendTarget = qtrue;
	SpinControl_Init(&networkOptionsInfo.voipSendTarget);
}

/*
=================
UI_NetworkOptions_SetMenuItems
=================
*/
static void UI_NetworkOptions_SetMenuItems(void) {
	int rate;

	rate = trap_Cvar_VariableValue("rate");
	if (rate <= 4000) {
		networkOptionsInfo.rate.curvalue = 0;
	} else if (rate <= 5000) {
		networkOptionsInfo.rate.curvalue = 1;
	} else {
		networkOptionsInfo.rate.curvalue = 2;
	}

	networkOptionsInfo.allowDownload.curvalue = (trap_Cvar_VariableValue("cl_allowDownload") != 0);

	if (!trap_Cvar_VariableValue("cl_voip") && trap_Cvar_VariableValue("cl_useMumble")) {
		networkOptionsInfo.voipMode.curvalue = 2;
	} else if (trap_Cvar_VariableValue("cl_voip") && !trap_Cvar_VariableValue("cl_useMumble")) {
		networkOptionsInfo.voipMode.curvalue = 1;
	} else {
		networkOptionsInfo.voipMode.curvalue = 0;
	}

	networkOptionsInfo.voipVADmode.curvalue = UI_GetCvarInt("cl_voipUseVAD");
	networkOptionsInfo.voipVADthreshold.curvalue = trap_Cvar_VariableValue("cl_voipVADThreshold") * 100;
	networkOptionsInfo.voipGainDuringCapture.curvalue = trap_Cvar_VariableValue("cl_voipGainDuringCapture") * 100;
	networkOptionsInfo.voipCaptureMult.curvalue = trap_Cvar_VariableValue("cl_voipCaptureMult");
	networkOptionsInfo.mumbleScale.curvalue = trap_Cvar_VariableValue("cl_mumbleScale") * 1000;

}

/*
=================
UI_NetworkOptions_UpdateMenuItems
=================
*/
static void UI_NetworkOptions_UpdateMenuItems(void) {

	// high rate enabled is condition for all voip settings
	if (trap_Cvar_VariableValue("rate") < 25000) {
		trap_Cvar_SetValue("cl_voip", 0);
		trap_Cvar_SetValue("cl_useMumble", 0);
		networkOptionsInfo.voipMode.curvalue = 0;
		networkOptionsInfo.voipHint.generic.flags &= ~QMF_HIDDEN;
		networkOptionsInfo.voipMode.generic.flags |= QMF_GRAYED;
		networkOptionsInfo.voipVADmode.generic.flags |= QMF_HIDDEN;
		networkOptionsInfo.voipVADthreshold.generic.flags |= QMF_HIDDEN;
		networkOptionsInfo.voipGainDuringCapture.generic.flags |= QMF_HIDDEN;
		networkOptionsInfo.voipCaptureMult.generic.flags |= QMF_HIDDEN;
		networkOptionsInfo.voipSendTarget.generic.flags |= QMF_HIDDEN;
		networkOptionsInfo.mumbleScale.generic.flags |= QMF_HIDDEN;
	} else {
		networkOptionsInfo.voipHint.generic.flags |= QMF_HIDDEN;
		networkOptionsInfo.voipMode.generic.flags &= ~QMF_GRAYED;

		// voipMode is set to Mumble
		if (!trap_Cvar_VariableValue("cl_voip") && trap_Cvar_VariableValue("cl_useMumble")) {
			networkOptionsInfo.voipVADmode.generic.flags |= QMF_HIDDEN;
			networkOptionsInfo.voipVADthreshold.generic.flags |= QMF_HIDDEN;
			networkOptionsInfo.voipGainDuringCapture.generic.flags |= QMF_HIDDEN;
			networkOptionsInfo.voipCaptureMult.generic.flags |= QMF_HIDDEN;
			networkOptionsInfo.voipSendTarget.generic.flags |= QMF_HIDDEN;
			networkOptionsInfo.mumbleScale.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);

		// voipMode is set to Built-in
		} else if (trap_Cvar_VariableValue("cl_voip") && !trap_Cvar_VariableValue("cl_useMumble")) {
			networkOptionsInfo.voipVADmode.generic.flags &= ~QMF_HIDDEN;
			networkOptionsInfo.voipVADthreshold.generic.flags &= ~QMF_HIDDEN;
			networkOptionsInfo.voipGainDuringCapture.generic.flags &= ~QMF_HIDDEN;
			networkOptionsInfo.voipCaptureMult.generic.flags &= ~QMF_HIDDEN;
			networkOptionsInfo.voipSendTarget.generic.flags &= ~QMF_HIDDEN;
			networkOptionsInfo.mumbleScale.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);

			// automatic recording enabled is a condition for voip vad threshold
			if (networkOptionsInfo.voipVADmode.curvalue == 0) {
				networkOptionsInfo.voipVADthreshold.generic.flags |= QMF_GRAYED;
			} else {
				networkOptionsInfo.voipVADthreshold.generic.flags &= ~QMF_GRAYED;
			}
			UI_NetworkOptions_SendTargetUpdate();
		} else {
			// voipMode is set to off
			networkOptionsInfo.voipVADmode.generic.flags |= QMF_HIDDEN;
			networkOptionsInfo.voipVADthreshold.generic.flags |= QMF_HIDDEN;
			networkOptionsInfo.voipGainDuringCapture.generic.flags |= QMF_HIDDEN;
			networkOptionsInfo.voipCaptureMult.generic.flags |= QMF_HIDDEN;
			networkOptionsInfo.voipSendTarget.generic.flags |= QMF_HIDDEN;
			networkOptionsInfo.mumbleScale.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
		}
	}
}

/*
=================
UI_NetworkOptions_Event
=================
*/
static void UI_NetworkOptions_Event(void *ptr, int event) {
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
		UI_PopMenu();
		UI_SoundOptionsMenu();
		break;

	case ID_NETWORK:
		break;

	case ID_RATE:
		if (networkOptionsInfo.rate.curvalue == 0) {
			trap_Cvar_SetValue("rate", 4000);
		} else if (networkOptionsInfo.rate.curvalue == 1) {
			trap_Cvar_SetValue("rate", 5000);
		} else if (networkOptionsInfo.rate.curvalue == 2) {
			trap_Cvar_SetValue("rate", 25000);
		}
		break;

	case ID_ALLOWDOWNLOAD:
		trap_Cvar_SetValue("cl_allowDownload", networkOptionsInfo.allowDownload.curvalue);
		break;

	case ID_VOIPMODE:
		if (networkOptionsInfo.voipMode.curvalue == 0) {
			trap_Cvar_SetValue("cl_voip", 0);
			trap_Cvar_SetValue("cl_useMumble", 0);
		} else if (networkOptionsInfo.voipMode.curvalue == 1) {
			trap_Cvar_SetValue("cl_voip", 1);
			trap_Cvar_SetValue("cl_useMumble", 0);
		} else if (networkOptionsInfo.voipMode.curvalue == 2) {
			trap_Cvar_SetValue("cl_voip", 0);
			trap_Cvar_SetValue("cl_useMumble", 1);
		}
		break;

	case ID_VOIPVADMODE:
		trap_Cvar_SetValue("cl_voipUseVAD", (float)networkOptionsInfo.voipVADmode.curvalue);
		break;

	case ID_VOIPVADTHRESHOLD:
		trap_Cvar_SetValue("cl_voipVADThreshold", (float)((int)networkOptionsInfo.voipVADthreshold.curvalue) / 100);
		break;

	case ID_VOIPCAPTUREGAIN:
		trap_Cvar_SetValue("cl_voipGainDuringCapture", (float)((int)networkOptionsInfo.voipGainDuringCapture.curvalue) / 100);
		break;

	case ID_VOIPCAPTUREMULT:
		trap_Cvar_SetValue("cl_voipCaptureMult", (int)networkOptionsInfo.voipCaptureMult.curvalue);
		break;

	case ID_VOIPSENDTARGET:
		if (!networkOptionsInfo.voipCustomSendTarget) {
			trap_Cvar_Set("cl_voipSendTarget", sendTarget_items[networkOptionsInfo.voipSendTarget.curvalue]);
		}
		break;

	case ID_MUMBLESCALE:
		trap_Cvar_SetValue("cl_mumbleScale", (float)((int)networkOptionsInfo.mumbleScale.curvalue) / 1000);
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
	
}

/*
===============
UI_NetworkOptions_MenuDraw
===============
*/
static void UI_NetworkOptions_MenuDraw(void) {
	UI_NetworkOptions_UpdateMenuItems();
	Menu_Draw(&networkOptionsInfo.menu);
}

/*
===============
UI_NetworkOptions_MenuInit
===============
*/
static void UI_NetworkOptions_MenuInit(void) {
	int y;

	memset(&networkOptionsInfo, 0, sizeof(networkOptionsInfo));

	UI_NetworkOptions_Cache();
	networkOptionsInfo.menu.wrapAround = qtrue;
	networkOptionsInfo.menu.fullscreen = qtrue;
	networkOptionsInfo.menu.draw = UI_NetworkOptions_MenuDraw;
	networkOptionsInfo.menu.bgparts = BGP_SYSTEM | BGP_MENUFX;

	networkOptionsInfo.graphics.generic.type = MTYPE_BITMAP;
	networkOptionsInfo.graphics.generic.name = GRAPHICS0;
	networkOptionsInfo.graphics.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	networkOptionsInfo.graphics.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.graphics.generic.id = ID_GRAPHICS;
	networkOptionsInfo.graphics.generic.x = 16;
	networkOptionsInfo.graphics.generic.y = 37;
	networkOptionsInfo.graphics.width = 160;
	networkOptionsInfo.graphics.height = 40;
	networkOptionsInfo.graphics.focuspic = GRAPHICS1;
	networkOptionsInfo.graphics.focuspicinstead = qtrue;

	networkOptionsInfo.display.generic.type = MTYPE_BITMAP;
	networkOptionsInfo.display.generic.name = DISPLAY0;
	networkOptionsInfo.display.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	networkOptionsInfo.display.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.display.generic.id = ID_DISPLAY;
	networkOptionsInfo.display.generic.x = 169;
	networkOptionsInfo.display.generic.y = 30;
	networkOptionsInfo.display.width = 120;
	networkOptionsInfo.display.height = 40;
	networkOptionsInfo.display.focuspic = DISPLAY1;
	networkOptionsInfo.display.focuspicinstead = qtrue;

	networkOptionsInfo.sound.generic.type = MTYPE_BITMAP;
	networkOptionsInfo.sound.generic.name = SOUND0;
	networkOptionsInfo.sound.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	networkOptionsInfo.sound.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.sound.generic.id = ID_SOUND;
	networkOptionsInfo.sound.generic.x = 36;
	networkOptionsInfo.sound.generic.y = 79;
	networkOptionsInfo.sound.width = 120;
	networkOptionsInfo.sound.height = 40;
	networkOptionsInfo.sound.focuspic = SOUND1;
	networkOptionsInfo.sound.focuspicinstead = qtrue;

	networkOptionsInfo.network.generic.type = MTYPE_BITMAP;
	networkOptionsInfo.network.generic.name = NETWORK0;
	networkOptionsInfo.network.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT;
	networkOptionsInfo.network.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.network.generic.id = ID_NETWORK;
	networkOptionsInfo.network.generic.x = 142;
	networkOptionsInfo.network.generic.y = 82;
	networkOptionsInfo.network.width = 160;
	networkOptionsInfo.network.height = 40;
	networkOptionsInfo.network.focuspic = NETWORK1;
	networkOptionsInfo.network.focuspicinstead = qtrue;

	y = 180 + 2 * (BIGCHAR_HEIGHT + 2);
	networkOptionsInfo.rate.generic.type = MTYPE_SPINCONTROL;
	networkOptionsInfo.rate.generic.name = "Data Rate:";
	networkOptionsInfo.rate.generic.flags = QMF_SMALLFONT;
	networkOptionsInfo.rate.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.rate.generic.id = ID_RATE;
	networkOptionsInfo.rate.generic.x = XPOSITION;
	networkOptionsInfo.rate.generic.y = y;
	networkOptionsInfo.rate.itemnames = rate_items;
	networkOptionsInfo.rate.generic.toolTip =
		"Please select your internet connection data rate, default is LAN/Cable/xDSL, the maximum. "
		"NOTE: This default setting is required for Voice over IP chat to work.";

	y += (BIGCHAR_HEIGHT + 2);
	networkOptionsInfo.allowDownload.generic.type = MTYPE_RADIOBUTTON;
	networkOptionsInfo.allowDownload.generic.name = "Auto Download:";
	networkOptionsInfo.allowDownload.generic.flags = QMF_SMALLFONT;
	networkOptionsInfo.allowDownload.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.allowDownload.generic.id = ID_ALLOWDOWNLOAD;
	networkOptionsInfo.allowDownload.generic.x = XPOSITION;
	networkOptionsInfo.allowDownload.generic.y = y;
	networkOptionsInfo.allowDownload.generic.toolTip =
		"Enable this to automatically download custom files needed to play on modified servers, default is off. "
		"NOTE: Custom files may interfere with original game files. Only connect to trusted servers with this enabled.";

	y += (BIGCHAR_HEIGHT + 2);
	networkOptionsInfo.voipHint.generic.type = MTYPE_TEXT;
	networkOptionsInfo.voipHint.generic.flags = QMF_PULSE;
	networkOptionsInfo.voipHint.generic.x = XPOSITION - 25;
	networkOptionsInfo.voipHint.generic.y = y;
	networkOptionsInfo.voipHint.string = "VoIP needs LAN/Cable/xDSL data rate!";
	networkOptionsInfo.voipHint.style = UI_CENTER | UI_SMALLFONT;
	networkOptionsInfo.voipHint.color = menu_text_color;

	y += (BIGCHAR_HEIGHT + 2);
	networkOptionsInfo.voipMode.generic.type = MTYPE_SPINCONTROL;
	networkOptionsInfo.voipMode.generic.name = "VoIP Chat:";
	networkOptionsInfo.voipMode.generic.flags = QMF_SMALLFONT;
	networkOptionsInfo.voipMode.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.voipMode.generic.id = ID_VOIPMODE;
	networkOptionsInfo.voipMode.generic.x = XPOSITION;
	networkOptionsInfo.voipMode.generic.y = y;
	networkOptionsInfo.voipMode.itemnames = voipMode_items;
	networkOptionsInfo.voipMode.generic.toolTip =
		"Set this to off to completely disable the VoIP chat. You will neither be able to transmit voice nor hear other "
		"people talking. The default setting is the built-in VoiP support. When you set to Mumble, you can link Mumble "
		"as an external program but you must start it before starting WoP.";

	y += (BIGCHAR_HEIGHT + 2);
	networkOptionsInfo.voipVADmode.generic.type = MTYPE_SPINCONTROL;
	networkOptionsInfo.voipVADmode.generic.name = "Capture Mode:";
	networkOptionsInfo.voipVADmode.generic.flags = QMF_SMALLFONT;
	networkOptionsInfo.voipVADmode.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.voipVADmode.generic.id = ID_VOIPVADMODE;
	networkOptionsInfo.voipVADmode.generic.x = XPOSITION;
	networkOptionsInfo.voipVADmode.generic.y = y;
	networkOptionsInfo.voipVADmode.itemnames = capture_items;
	networkOptionsInfo.voipVADmode.generic.toolTip =
		"Set this to push to talk, the default, to use a key to start the transmission manually. See the chat "
		"page of the controls menu for key binding. When set to automatic, audio is sent when the game thinks "
		"you are talking.";

	networkOptionsInfo.mumbleScale.generic.type = MTYPE_SLIDER;
	networkOptionsInfo.mumbleScale.generic.name = "Distance Scale:";
	networkOptionsInfo.mumbleScale.generic.flags = QMF_SMALLFONT;
	networkOptionsInfo.mumbleScale.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.mumbleScale.generic.id = ID_MUMBLESCALE;
	networkOptionsInfo.mumbleScale.generic.x = XPOSITION;
	networkOptionsInfo.mumbleScale.generic.y = y;
	networkOptionsInfo.mumbleScale.minvalue = 0;
	networkOptionsInfo.mumbleScale.maxvalue = 200;
	networkOptionsInfo.mumbleScale.generic.toolTip =
		"Use this to adjust the distance scale. The Mumble link support supplies it with your in-game position, "
		"but everything else is whatever features Mumble offers outside of the game.";

	y += (BIGCHAR_HEIGHT + 2);
	networkOptionsInfo.voipVADthreshold.generic.type = MTYPE_SLIDER;
	networkOptionsInfo.voipVADthreshold.generic.name = "Capture Threshold:";
	networkOptionsInfo.voipVADthreshold.generic.flags = QMF_SMALLFONT;
	networkOptionsInfo.voipVADthreshold.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.voipVADthreshold.generic.id = ID_VOIPVADTHRESHOLD;
	networkOptionsInfo.voipVADthreshold.generic.x = XPOSITION;
	networkOptionsInfo.voipVADthreshold.generic.y = y;
	networkOptionsInfo.voipVADthreshold.minvalue = 0;
	networkOptionsInfo.voipVADthreshold.maxvalue = 100;
	networkOptionsInfo.voipVADthreshold.generic.toolTip =
		"This signifies the volume of recorded audio that the game considers to be speech. You can use this "
		"to trim out background noise. You will have to experiment to find the setting that works best. The "
		"default is 25, with 0 being silence and 100 being pretty loud.";

	y += (BIGCHAR_HEIGHT + 2);
	networkOptionsInfo.voipGainDuringCapture.generic.type = MTYPE_SLIDER;
	networkOptionsInfo.voipGainDuringCapture.generic.name = "Speakers Volume:";
	networkOptionsInfo.voipGainDuringCapture.generic.flags = QMF_SMALLFONT;
	networkOptionsInfo.voipGainDuringCapture.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.voipGainDuringCapture.generic.id = ID_VOIPCAPTUREGAIN;
	networkOptionsInfo.voipGainDuringCapture.generic.x = XPOSITION;
	networkOptionsInfo.voipGainDuringCapture.generic.y = y;
	networkOptionsInfo.voipGainDuringCapture.minvalue = 0;
	networkOptionsInfo.voipGainDuringCapture.maxvalue = 100;
	networkOptionsInfo.voipGainDuringCapture.generic.toolTip =
		"This is the volume of audio coming out of your speakers while you are recording sound for transmission. "
		"Defaults to 20 percent of normal volume. This prevents audio feedback and echo. If you're using headphones, "
		"you don't need to turn this down.";

	y += (BIGCHAR_HEIGHT + 2);
	networkOptionsInfo.voipCaptureMult.generic.type = MTYPE_SLIDER;
	networkOptionsInfo.voipCaptureMult.generic.name = "Voice Amplifier:";
	networkOptionsInfo.voipCaptureMult.generic.flags = QMF_SMALLFONT;
	networkOptionsInfo.voipCaptureMult.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.voipCaptureMult.generic.id = ID_VOIPCAPTUREMULT;
	networkOptionsInfo.voipCaptureMult.generic.x = XPOSITION;
	networkOptionsInfo.voipCaptureMult.generic.y = y;
	networkOptionsInfo.voipCaptureMult.minvalue = 0;
	networkOptionsInfo.voipCaptureMult.maxvalue = 10;
	networkOptionsInfo.voipCaptureMult.generic.toolTip =
		"This multiplies recorded audio by the set value after denoising. Defaults to 2 to double the volume of "
		"your voice. This is to make you more audible if denoising eats away too much data. Set this to 1.0 to "
		"get no change, less to be quieter.";

	y += BIGCHAR_HEIGHT + 2;
	networkOptionsInfo.voipSendTarget.generic.type = MTYPE_SPINCONTROL;
	networkOptionsInfo.voipSendTarget.generic.name = "Send Target:";
	networkOptionsInfo.voipSendTarget.generic.flags = QMF_SMALLFONT;
	networkOptionsInfo.voipSendTarget.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.voipSendTarget.generic.id = ID_VOIPSENDTARGET;
	networkOptionsInfo.voipSendTarget.generic.x = XPOSITION;
	networkOptionsInfo.voipSendTarget.generic.y = y;
	networkOptionsInfo.voipCustomSendTarget = qtrue;
	
	networkOptionsInfo.back.generic.type = MTYPE_BITMAP;
	networkOptionsInfo.back.generic.name = BACK0;
	networkOptionsInfo.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	networkOptionsInfo.back.generic.callback = UI_NetworkOptions_Event;
	networkOptionsInfo.back.generic.id = ID_BACK;
	networkOptionsInfo.back.generic.x = 8;
	networkOptionsInfo.back.generic.y = 440;
	networkOptionsInfo.back.width = 80;
	networkOptionsInfo.back.height = 40;
	networkOptionsInfo.back.focuspic = BACK1;
	networkOptionsInfo.back.focuspicinstead = qtrue;

	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.graphics);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.display);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.sound);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.network);

	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.rate);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.allowDownload);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.voipHint);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.voipMode);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.voipVADmode);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.voipVADthreshold);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.voipGainDuringCapture);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.voipCaptureMult);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.voipSendTarget);
	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.mumbleScale);

	Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.back);

	UI_NetworkOptions_SetMenuItems();
}

/*
===============
UI_NetworkOptions_Cache
===============
*/
void UI_NetworkOptions_Cache(void) {
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
}

/*
===============
UI_NetworkOptionsMenu
===============
*/
void UI_NetworkOptionsMenu(void) {
	UI_NetworkOptions_MenuInit();
	UI_PushMenu(&networkOptionsInfo.menu);
	Menu_SetCursorToItem(&networkOptionsInfo.menu, &networkOptionsInfo.network);
}
