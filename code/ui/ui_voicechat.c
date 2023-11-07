#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define ARROWUP0 "menu/arrows/headyel_up0"
#define ARROWUP1 "menu/arrows/headyel_up1"
#define ARROWDN0 "menu/arrows/headyel_dn0"
#define ARROWDN1 "menu/arrows/headyel_dn1"

#define NUM_LISTEDCLIENTS 6 // tied to the ID_CLIENTXs
#define NAME_MAXLENGTH 32

#define ID_BACK 10
#define ID_SCROLL_UP 11
#define ID_SCROLL_DOWN 12
#define ID_CLIENT0 13
#define ID_CLIENT1 14
#define ID_CLIENT2 15
#define ID_CLIENT3 16
#define ID_CLIENT4 17
#define ID_CLIENT5 18

#define XPOSITION (SCREEN_WIDTH / 2)

typedef struct {
	menuframework_s menu;

	menutext_s displayClients[NUM_LISTEDCLIENTS];

	menuslider_s gain;
	menuradiobutton_s mute;
	menuradiobutton_s muteAll;
	menulist_s sendTarget;

	menubitmap_s back;
	menubitmap_s arrowup;
	menubitmap_s arrowdown;

	char displayClientNames[NUM_LISTEDCLIENTS][NAME_MAXLENGTH];
	int numDisplayed;	   // how many of the display slots do we use atm
	int baseClientNum;	   // list index of the first client we show
	int selectedClientNum; // list index of the selected client

	int numClients; // how many clients are in the list
	char clientNames[MAX_CLIENTS][NAME_MAXLENGTH];
	char clientIDs[MAX_CLIENTS];
} voiceChatMenuInfo_t;

static voiceChatMenuInfo_t voiceChatMenuInfo;

static const char *sendTarget_names[] = {"all", "none", "attacker", "crosshair", "spatial", NULL};

static void UI_VoiceChatMenu_SetClientNames(void) {
	int i, j;

	// reset entries
	for (i = 0; i < NUM_LISTEDCLIENTS; i++)
		voiceChatMenuInfo.displayClientNames[i][0] = '\0';

	voiceChatMenuInfo.numDisplayed = 0;

	if (!voiceChatMenuInfo.numClients)
		return;

	if (voiceChatMenuInfo.baseClientNum >= voiceChatMenuInfo.numClients)
		voiceChatMenuInfo.baseClientNum = voiceChatMenuInfo.numClients - 1;

	for (i = 0, j = voiceChatMenuInfo.baseClientNum; i < NUM_LISTEDCLIENTS; i++, j++) {
		if (j >= voiceChatMenuInfo.numClients)
			break;
		Q_strncpyz(voiceChatMenuInfo.displayClientNames[i], voiceChatMenuInfo.clientNames[j], NAME_MAXLENGTH);
		voiceChatMenuInfo.numDisplayed++;
	}
}

static void UI_VoiceChatMenu_UpdateSelection(int newNum) {
	int selectedClientID;
	// set unselected colors for former selected item
	voiceChatMenuInfo.displayClients[voiceChatMenuInfo.selectedClientNum].color = colorBlack;
	voiceChatMenuInfo.displayClients[voiceChatMenuInfo.selectedClientNum].focuscolor = colorDkLilac;

	voiceChatMenuInfo.selectedClientNum = newNum;
	selectedClientID =
		voiceChatMenuInfo.clientIDs[voiceChatMenuInfo.selectedClientNum + voiceChatMenuInfo.baseClientNum];
	// set selected colors for new selected item
	voiceChatMenuInfo.displayClients[voiceChatMenuInfo.selectedClientNum].color = colorDkOrange;
	voiceChatMenuInfo.displayClients[voiceChatMenuInfo.selectedClientNum].focuscolor = color_orange;
	// adjust mute and gain values

	voiceChatMenuInfo.mute.generic.flags &= ~QMF_GRAYED;
	voiceChatMenuInfo.mute.curvalue = (trap_GetVoiceMuteClient(selectedClientID) != 0);

	voiceChatMenuInfo.gain.generic.flags &= ~QMF_GRAYED;
	voiceChatMenuInfo.gain.curvalue = trap_GetVoiceGainClient(selectedClientID) * 5;
}

static void UI_VoiceChatMenu_UpEvent(void *ptr, int event) {
	int newSelection;
	if (event != QM_ACTIVATED) {
		return;
	}

	if (voiceChatMenuInfo.baseClientNum > 0) {
		voiceChatMenuInfo.baseClientNum--;
		UI_VoiceChatMenu_SetClientNames();

		newSelection = voiceChatMenuInfo.selectedClientNum + 1;
		if (newSelection >= NUM_LISTEDCLIENTS)
			newSelection = NUM_LISTEDCLIENTS - 1;
		UI_VoiceChatMenu_UpdateSelection(newSelection);
	}
}

static void UI_VoiceChatMenu_DownEvent(void *ptr, int event) {
	int newSelection;
	if (event != QM_ACTIVATED) {
		return;
	}

	if (voiceChatMenuInfo.baseClientNum + NUM_LISTEDCLIENTS < voiceChatMenuInfo.numClients) {
		voiceChatMenuInfo.baseClientNum++;
		UI_VoiceChatMenu_SetClientNames();

		newSelection = voiceChatMenuInfo.selectedClientNum - 1;
		if (newSelection < 0)
			newSelection = 0;
		UI_VoiceChatMenu_UpdateSelection(newSelection);
	}
}

static sfxHandle_t UI_VoiceChatMenu_Key(int key) {
	switch (key) {
	case K_MWHEELUP:
	case K_PGUP:
		UI_VoiceChatMenu_UpEvent(&voiceChatMenuInfo.arrowup, QM_ACTIVATED);
		break;
	case K_MWHEELDOWN:
	case K_PGDN:
		UI_VoiceChatMenu_DownEvent(&voiceChatMenuInfo.arrowdown, QM_ACTIVATED);
		break;
	}

	return Menu_DefaultKey(&voiceChatMenuInfo.menu, key);
}

static void UI_VoiceChatMenu_Draw(void) {
	UI_DrawIngameBG();
	UI_DrawProportionalString(XPOSITION, 110, "VOICE CHAT", UI_CENTER | UI_SMALLFONT, color_black);

	// standard menu drawing
	Menu_Draw(&voiceChatMenuInfo.menu);
}

static void UI_VoiceChatMenu_SelectEvent(void *ptr, int event) {
	if (event == QM_ACTIVATED)
		UI_VoiceChatMenu_UpdateSelection(((menucommon_s *)ptr)->id - ID_CLIENT0);
}

static void UI_VoiceChatMenu_BackEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}
	UI_PopMenu();
}

static void UI_VoiceChatMenu_GainSliderEvent(void *ptr, int event) {
	const int arrayIdx = voiceChatMenuInfo.baseClientNum + voiceChatMenuInfo.selectedClientNum;
	const int clID = voiceChatMenuInfo.clientIDs[arrayIdx];
	const float gain = voiceChatMenuInfo.gain.curvalue / 5;
	trap_Cmd_ExecuteText(EXEC_APPEND, va("voip gain %d %f", clID, gain));
}

static void UI_VoiceChatMenu_MuteEvent(void *ptr, int event) {
	const int arrayIdx = voiceChatMenuInfo.baseClientNum + voiceChatMenuInfo.selectedClientNum;
	const int clID = voiceChatMenuInfo.clientIDs[arrayIdx];

	if (event != QM_ACTIVATED)
		return;

	if (voiceChatMenuInfo.mute.curvalue)
		trap_Cmd_ExecuteText(EXEC_APPEND, va("voip ignore %d", clID));
	else
		trap_Cmd_ExecuteText(EXEC_APPEND, va("voip unignore %d", clID));
}

static void UI_VoiceChatMenu_MuteAllEvent(void *ptr, int event) {
	if (event != QM_ACTIVATED)
		return;

	if (voiceChatMenuInfo.muteAll.curvalue)
		trap_Cmd_ExecuteText(EXEC_APPEND, "voip muteall");
	else
		trap_Cmd_ExecuteText(EXEC_APPEND, "voip unmuteall");
}

static void UI_VoiceChatMenu_sendTargetEvent(void *ptr, int event) {
	int val = voiceChatMenuInfo.sendTarget.curvalue;
	if (event != QM_ACTIVATED)
		return;

	if (val < 0 || val >= 2) // hardcoded array size :/
		return;

	trap_Cvar_Set("cl_voipSendTarget", sendTarget_names[voiceChatMenuInfo.sendTarget.curvalue]);
}

static void UI_VoiceChatMenu_InitClients(void) {
	int i;
	const char *name;
	int index;
	char info[MAX_INFO_STRING];
	char localClientName[32]; // used to identify the local client.. there has to be a better way

	trap_Cvar_VariableStringBuffer("name", localClientName, sizeof(localClientName));
	voiceChatMenuInfo.numClients = 0;

	for (i = 0; i < MAX_CLIENTS; ++i) {
		trap_GetConfigString(CS_PLAYERS + i, info, sizeof(info));

		name = Info_ValueForKey(info, "n");
		if (!strlen(name))
			continue;

		if (Q_stricmpn(name, localClientName, sizeof(localClientName)) == 0)
			continue;

		// it' a client, add it
		index = voiceChatMenuInfo.numClients++;
		Q_strncpyz(voiceChatMenuInfo.clientNames[index], name, NAME_MAXLENGTH);
		Q_CleanStr(voiceChatMenuInfo.clientNames[index]);
		voiceChatMenuInfo.clientIDs[index] = i;
	}
}

void UI_VoiceChatMenu_Cache(void);

static void UI_VoiceChatMenu_Init(void) {
	int n;
	int y;
	int i;
	char sendTargetValue[64];

	memset(&voiceChatMenuInfo, 0, sizeof(voiceChatMenuInfo));
	voiceChatMenuInfo.menu.draw = UI_VoiceChatMenu_Draw;
	voiceChatMenuInfo.menu.fullscreen = qfalse;
	voiceChatMenuInfo.menu.wrapAround = qtrue;
	voiceChatMenuInfo.menu.key = UI_VoiceChatMenu_Key;

	UI_VoiceChatMenu_Cache();
	UI_VoiceChatMenu_InitClients();
	UI_VoiceChatMenu_SetClientNames();

	voiceChatMenuInfo.arrowup.generic.type = MTYPE_BITMAP;
	voiceChatMenuInfo.arrowup.generic.name = ARROWUP0;
	voiceChatMenuInfo.arrowup.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	voiceChatMenuInfo.arrowup.generic.callback = UI_VoiceChatMenu_UpEvent;
	voiceChatMenuInfo.arrowup.generic.id = ID_SCROLL_UP;
	voiceChatMenuInfo.arrowup.generic.x = XPOSITION + 80;
	voiceChatMenuInfo.arrowup.generic.y = 142;
	voiceChatMenuInfo.arrowup.width = 22;
	voiceChatMenuInfo.arrowup.height = 50;
	voiceChatMenuInfo.arrowup.focuspic = ARROWUP1;
	voiceChatMenuInfo.arrowup.focuspicinstead = qtrue;

	voiceChatMenuInfo.arrowdown.generic.type = MTYPE_BITMAP;
	voiceChatMenuInfo.arrowdown.generic.name = ARROWDN0;
	voiceChatMenuInfo.arrowdown.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	voiceChatMenuInfo.arrowdown.generic.callback = UI_VoiceChatMenu_DownEvent;
	voiceChatMenuInfo.arrowdown.generic.id = ID_SCROLL_DOWN;
	voiceChatMenuInfo.arrowdown.generic.x = XPOSITION + 80;
	voiceChatMenuInfo.arrowdown.generic.y = 208;
	voiceChatMenuInfo.arrowdown.width = 22;
	voiceChatMenuInfo.arrowdown.height = 50;
	voiceChatMenuInfo.arrowdown.focuspic = ARROWDN1;
	voiceChatMenuInfo.arrowdown.focuspicinstead = qtrue;

	for (n = 0, y = 140; n < voiceChatMenuInfo.numDisplayed; n++, y += 20) {
		voiceChatMenuInfo.displayClients[n].generic.type = MTYPE_TEXTS;
		voiceChatMenuInfo.displayClients[n].fontHeight = 20.0f;
		voiceChatMenuInfo.displayClients[n].generic.flags = QMF_LEFT_JUSTIFY;
		voiceChatMenuInfo.displayClients[n].generic.id = ID_CLIENT0 + n;
		voiceChatMenuInfo.displayClients[n].generic.x = XPOSITION - 70;
		voiceChatMenuInfo.displayClients[n].generic.y = y;
		voiceChatMenuInfo.displayClients[n].generic.callback = UI_VoiceChatMenu_SelectEvent;
		voiceChatMenuInfo.displayClients[n].string = voiceChatMenuInfo.displayClientNames[n];
		voiceChatMenuInfo.displayClients[n].color = colorBlack;
		voiceChatMenuInfo.displayClients[n].focuscolor = colorDkLilac;
		voiceChatMenuInfo.displayClients[n].style = UI_LEFT | UI_SMALLFONT;
	}

	y = 266;
	voiceChatMenuInfo.gain.generic.type = MTYPE_SLIDER;
	voiceChatMenuInfo.gain.generic.name = "Volume";
	voiceChatMenuInfo.gain.generic.flags = QMF_BLUESTYLE | QMF_GRAYED;
	voiceChatMenuInfo.gain.generic.callback = UI_VoiceChatMenu_GainSliderEvent;
	voiceChatMenuInfo.gain.generic.x = XPOSITION - 5;
	voiceChatMenuInfo.gain.generic.y = y;
	voiceChatMenuInfo.gain.minvalue = 0;
	voiceChatMenuInfo.gain.maxvalue = 10;
	voiceChatMenuInfo.gain.curvalue = 5;

	y += SMALLCHAR_HEIGHT + 2;
	voiceChatMenuInfo.mute.generic.type = MTYPE_RADIOBUTTON;
	voiceChatMenuInfo.mute.generic.name = "Mute";
	voiceChatMenuInfo.mute.generic.flags = QMF_BLUESTYLE | QMF_SMALLFONT | QMF_GRAYED;
	voiceChatMenuInfo.mute.generic.callback = UI_VoiceChatMenu_MuteEvent;
	voiceChatMenuInfo.mute.generic.x = XPOSITION - 5;
	voiceChatMenuInfo.mute.generic.y = y;
	voiceChatMenuInfo.mute.curvalue = 0;

	y += SMALLCHAR_HEIGHT + 2;
	voiceChatMenuInfo.muteAll.generic.type = MTYPE_RADIOBUTTON;
	voiceChatMenuInfo.muteAll.generic.name = "Mute All";
	voiceChatMenuInfo.muteAll.generic.flags = QMF_BLUESTYLE | QMF_SMALLFONT;
	voiceChatMenuInfo.muteAll.generic.callback = UI_VoiceChatMenu_MuteAllEvent;
	voiceChatMenuInfo.muteAll.generic.x = XPOSITION - 5;
	voiceChatMenuInfo.muteAll.generic.y = y;
	voiceChatMenuInfo.muteAll.curvalue = (trap_GetVoiceMuteAll() != 0);

	y += SMALLCHAR_HEIGHT + 2;
	voiceChatMenuInfo.sendTarget.generic.type = MTYPE_SPINCONTROL;
	voiceChatMenuInfo.sendTarget.generic.name = "Talk To:";
	voiceChatMenuInfo.sendTarget.generic.flags = QMF_SMALLFONT;
	voiceChatMenuInfo.sendTarget.generic.callback = UI_VoiceChatMenu_sendTargetEvent;
	voiceChatMenuInfo.sendTarget.generic.x = XPOSITION - 5;
	voiceChatMenuInfo.sendTarget.generic.y = y;
	voiceChatMenuInfo.sendTarget.itemnames = sendTarget_names;

	trap_Cvar_VariableStringBuffer("cl_voipSendTarget", sendTargetValue, sizeof(sendTargetValue));
	i = 0;
	while (voiceChatMenuInfo.sendTarget.itemnames[i] != 0) {
		const char *item = voiceChatMenuInfo.sendTarget.itemnames[i];

		// sendtargetvalue can have extra data after the keyword
		if (!Q_stricmpn(sendTargetValue, item, strlen(item))) {
			voiceChatMenuInfo.sendTarget.curvalue = i;
			break;
		}
		i++;
	}

	voiceChatMenuInfo.back.generic.type = MTYPE_BITMAP;
	voiceChatMenuInfo.back.generic.name = BACK0;
	voiceChatMenuInfo.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	voiceChatMenuInfo.back.generic.x = XPOSITION - 95;
	voiceChatMenuInfo.back.generic.y = 340;
	voiceChatMenuInfo.back.generic.id = ID_BACK;
	voiceChatMenuInfo.back.generic.callback = UI_VoiceChatMenu_BackEvent;
	voiceChatMenuInfo.back.width = 50;
	voiceChatMenuInfo.back.height = 25;
	voiceChatMenuInfo.back.focuspic = BACK1;
	voiceChatMenuInfo.back.focuspicinstead = qtrue;

	Menu_AddItem(&voiceChatMenuInfo.menu, &voiceChatMenuInfo.arrowup);
	Menu_AddItem(&voiceChatMenuInfo.menu, &voiceChatMenuInfo.arrowdown);
	for (n = 0; n < voiceChatMenuInfo.numDisplayed; n++) {
		Menu_AddItem(&voiceChatMenuInfo.menu, &voiceChatMenuInfo.displayClients[n]);
		// Menu_AddItem( &voiceChatMenuInfo.menu, &voiceChatMenuInfo.bot_pics[n] );
	}
	Menu_AddItem(&voiceChatMenuInfo.menu, &voiceChatMenuInfo.gain);
	Menu_AddItem(&voiceChatMenuInfo.menu, &voiceChatMenuInfo.mute);
	Menu_AddItem(&voiceChatMenuInfo.menu, &voiceChatMenuInfo.muteAll);
	Menu_AddItem(&voiceChatMenuInfo.menu, &voiceChatMenuInfo.sendTarget);
	Menu_AddItem(&voiceChatMenuInfo.menu, &voiceChatMenuInfo.back);
}

void UI_VoiceChatMenu_Cache(void) {
	trap_R_RegisterShaderNoMip(ARROWUP0);
	trap_R_RegisterShaderNoMip(ARROWUP1);
	trap_R_RegisterShaderNoMip(ARROWDN0);
	trap_R_RegisterShaderNoMip(ARROWDN1);
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
}

void UI_VoiceIngame(void) {
	UI_VoiceChatMenu_Init();
	UI_PushMenu(&voiceChatMenuInfo.menu);
}
