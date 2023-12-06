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
#ifndef __UI_LOCAL_H__
#define __UI_LOCAL_H__

#include "../qcommon/q_shared.h"
#include "../renderercommon/tr_types.h"
#include "../ui/ui_public.h"
// redefine to old API version
#undef UI_API_VERSION
#define UI_API_VERSION 4
#include "../client/keycodes.h"
#include "../game/bg_public.h"

// For only widescreen menu SCREEN_* must be seprate values for UI, CGame, and Client.
#undef SCREEN_WIDTH
#undef SCREEN_HEIGHT
#define SCREEN_WIDTH 864
#define SCREEN_HEIGHT 486

typedef void (*voidfunc_f)(void);

extern vmCvar_t ui_ffa_fraglimit;
extern vmCvar_t ui_ffa_timelimit;

extern vmCvar_t ui_tourney_fraglimit;
extern vmCvar_t ui_tourney_timelimit;

extern vmCvar_t ui_team_fraglimit;
extern vmCvar_t ui_team_timelimit;
extern vmCvar_t ui_team_friendly;

extern vmCvar_t ui_ctf_capturelimit;
extern vmCvar_t ui_ctf_timelimit;
extern vmCvar_t ui_ctf_friendly;

extern vmCvar_t ui_lps_startlives;

extern vmCvar_t ui_arenasFile;
extern vmCvar_t ui_botsFile;

extern vmCvar_t ui_browserMaster;
extern vmCvar_t ui_browserGameType;
extern vmCvar_t ui_browserSortKey;
extern vmCvar_t ui_browserShowFull;
extern vmCvar_t ui_browserShowEmpty;
extern vmCvar_t ui_browserOnlyHumans;
extern vmCvar_t ui_browserHidePrivate;

extern vmCvar_t ui_createGametype;
extern vmCvar_t ui_drawToolTip;

extern vmCvar_t ui_drawCrosshair;
extern vmCvar_t ui_drawCrosshairNames;
extern vmCvar_t ui_marks;

extern vmCvar_t ui_server1;
extern vmCvar_t ui_server2;
extern vmCvar_t ui_server3;
extern vmCvar_t ui_server4;
extern vmCvar_t ui_server5;
extern vmCvar_t ui_server6;
extern vmCvar_t ui_server7;
extern vmCvar_t ui_server8;
extern vmCvar_t ui_server9;
extern vmCvar_t ui_server10;
extern vmCvar_t ui_server11;
extern vmCvar_t ui_server12;
extern vmCvar_t ui_server13;
extern vmCvar_t ui_server14;
extern vmCvar_t ui_server15;
extern vmCvar_t ui_server16;

extern vmCvar_t spraycolor;
extern vmCvar_t syc_logo;
extern vmCvar_t s_wop_restarted;
extern vmCvar_t con_notifytime;
extern vmCvar_t wop_AutoswitchSongByNextMap;
extern vmCvar_t wop_AutoBindUnusedKeys;
extern vmCvar_t cg_drawChatIcon;
extern vmCvar_t cg_chatBeep;
extern vmCvar_t cg_glowModel;
#ifdef USE_RENDERER_DLOPEN
extern vmCvar_t cl_renderer;
#endif

//
// locals
//

#define UI_LOCALEFILE "locales/ui.txt"

//
// ui_qmenu.c
//

#define SLIDER_RANGE 10
#define MAX_EDIT_LINE 256

#define MAX_MENUDEPTH 8
#define MAX_MENUITEMS 96

#define MTYPE_NULL 0
#define MTYPE_SLIDER 1
#define MTYPE_ACTION 2
#define MTYPE_SPINCONTROL 3
#define MTYPE_FIELD 4
#define MTYPE_RADIOBUTTON 5
#define MTYPE_BITMAP 6
#define MTYPE_TEXT 7
#define MTYPE_SCROLLLIST 8

#define MTYPE_TEXTS 12		 // sizeable(S) text

#define QMF_BLINK ((unsigned int)0x00000001)
#define QMF_SMALLFONT ((unsigned int)0x00000002)
#define QMF_LEFT_JUSTIFY ((unsigned int)0x00000004)
#define QMF_CENTER_JUSTIFY ((unsigned int)0x00000008)
#define QMF_RIGHT_JUSTIFY ((unsigned int)0x00000010)
#define QMF_NUMBERSONLY ((unsigned int)0x00000020) // edit field is only numbers
#define QMF_HIGHLIGHT ((unsigned int)0x00000040)
#define QMF_HIGHLIGHT_IF_FOCUS ((unsigned int)0x00000080) // steady focus
#define QMF_PULSEIFFOCUS ((unsigned int)0x00000100)		  // pulse if focus
#define QMF_HASMOUSEFOCUS ((unsigned int)0x00000200)
#define QMF_NOONOFFTEXT ((unsigned int)0x00000400)
#define QMF_MOUSEONLY ((unsigned int)0x00000800)	 // only mouse input allowed
#define QMF_HIDDEN ((unsigned int)0x00001000)		 // skips drawing
#define QMF_GRAYED ((unsigned int)0x00002000)		 // grays and disables
#define QMF_INACTIVE ((unsigned int)0x00004000)		 // disables any input
#define QMF_NODEFAULTINIT ((unsigned int)0x00008000) // skip default initialization
#define QMF_OWNERDRAW ((unsigned int)0x00010000)
#define QMF_PULSE ((unsigned int)0x00020000)
#define QMF_LOWERCASE ((unsigned int)0x00040000) // edit field is all lower case
#define QMF_UPPERCASE ((unsigned int)0x00080000) // edit field is all upper case
#define QMF_SILENT ((unsigned int)0x00100000)

#define QMF_INGAMESTYLE 0x00200000
#define QMF_FORCEDROPDOWN                                                                                              \
	0x00400000 // forces dropdown at SpinControls width ownerdraw (normal ownerdraw SCs should work like they did in
			   // vq3)
#define QMF_BLUESTYLE 0x00800000

// callback notifications
#define QM_GOTFOCUS 1
#define QM_LOSTFOCUS 2
#define QM_ACTIVATED 3

#define BGP_MAINFX 0x000001
#define BGP_MAIN 0x000002
#define BGP_SETUP 0x000004
#define BGP_SYSTEM 0x000008
#define BGP_DEFAULTS 0x000010
#define BGP_EXITBG 0x000020
#define BGP_CONTROLS 0x000040
#define BGP_PLAYER 0x000080
#define BGP_SERVERS 0x000100
#define BGP_MODS 0x000200
#define BGP_DEMOS 0x000400
#define BGP_SPECIFY 0x000800
#define BGP_PREFERENCES 0x001000
#define BGP_STARTSERVER 0x002000
#define BGP_SELECTBOTS 0x004000
#define BGP_MENUFX 0x008000
#define BGP_LASTMENU 0x010000

typedef struct _tag_menuframework {
	int cursor;
	int cursor_prev;

	int nitems;
	void *items[MAX_MENUITEMS];

	void (*draw)(void);
	sfxHandle_t (*key)(int key);

	qboolean wrapAround;
	qboolean fullscreen;

	int bgparts;
	qboolean noPushSelect;

	qboolean noMouseCursor;
} menuframework_s;

typedef struct {
	int type;
	const char *name;
	const char *toolTip;
	int id;
	int x, y;
	int left;
	int top;
	int right;
	int bottom;
	menuframework_s *parent;
	int menuPosition;
	unsigned int flags;

	/**
	 * @param[in] event QM_GOTFOCUS, QM_LOSTFOCUS or QM_ACTIVATED
	 */
	void (*callback)(void *self, int event);
	void (*statusbar)(void *self);
	// total subclassing, owner draws everything if set
	void (*ownerdraw)(void *self);
} menucommon_s;

typedef struct {
	int cursor;
	int scroll;
	int widthInChars;
	char buffer[MAX_EDIT_LINE];
	int maxchars;
} mfield_t;

typedef struct {
	menucommon_s generic;
	mfield_t field;
} menufield_s;

typedef struct {
	menucommon_s generic;

	float minvalue;
	float maxvalue;
	float curvalue;

	float range;
} menuslider_s;

typedef struct {
	menucommon_s generic;

	int oldvalue;
	int curvalue;
	int numitems;
	int top;

	const char **itemnames;

	int width;
	int height;
	int columns;
	int seperation;

	int dropdown_len;
	const float *dropdown_bg;
	const float *dropdown_text;
	const float *dropdown_mark;
	const float *dropdown_border;
} menulist_s;

typedef struct {
	menucommon_s generic;
} menuaction_s;

typedef struct {
	menucommon_s generic;
	int curvalue;
} menuradiobutton_s;

typedef struct {
	menucommon_s generic;
	const char *focuspic;
	const char *errorpic;
	qhandle_t shader;
	qhandle_t focusshader;
	int width;
	int height;
	const float *focuscolor;

	qboolean focuspicinstead; // by QuarterPounder
} menubitmap_s;

typedef struct {
	menucommon_s generic;
	char *string;
	int style;
	const float *color;

	float fontHeight; // for TextS ...
	const float *focuscolor;
} menutext_s;

extern void Menu_Cache(void);
extern void Menu_AddItem(menuframework_s *menu, void *item);
extern void Menu_InitItem(menucommon_s *item);
extern void Menu_Draw(menuframework_s *menu);
extern void Menu_DrawField(menufield_s *field);
extern void *Menu_ItemAtCursor(menuframework_s *m);
extern sfxHandle_t Menu_ActivateItem(menuframework_s *s, menucommon_s *item);
extern void Menu_SetCursor(menuframework_s *s, int cursor);
extern void Menu_SetCursorToItem(menuframework_s *m, void *ptr);
extern sfxHandle_t Menu_DefaultKey(menuframework_s *s, int key);
extern sfxHandle_t ScrollList_Key(menulist_s *l, int key);
extern void ParseMenuListItems(char *input, menulist_s *output, int maxItems, char separator, const char *selection, const char *empty);

extern sfxHandle_t menuMoveSound;
extern sfxHandle_t menuSwitchSound;
extern sfxHandle_t menuClickSound;
extern sfxHandle_t menuBuzzSound;
extern sfxHandle_t menuNullSound;
extern sfxHandle_t weaponChangeSound;
extern const vec4_t menu_text_color;
extern const vec4_t menu_grayed_color;
extern const vec4_t menu_dark_color;
extern const vec4_t menu_highlight_color;
extern const vec4_t menu_red_color;
extern const vec4_t menu_black_color;
extern const vec4_t menu_dim_color;
extern const vec4_t color_black;
extern const vec4_t color_white;
extern const vec4_t color_yellow;
extern const vec4_t color_blue;
extern const vec4_t color_lightOrange;
extern const vec4_t color_orange;
extern const vec4_t color_red;
extern const vec4_t color_dim;
extern const vec4_t name_color;
extern const vec4_t list_color;
extern const vec4_t listbar_color;
extern const vec4_t text_color_disabled;
extern const vec4_t text_color_normal;
extern const vec4_t text_color_highlight;

extern const vec4_t listbar_igcolor;
extern const vec4_t text_color_ignormal;
extern const vec4_t text_color_ighighlight;
extern const vec4_t text_color_bluenormal;
extern const vec4_t text_color_bluehighlight;
extern const vec4_t listbar_bluecolor;

//
// ui_mfield.c
//
extern void MField_Clear(mfield_t *edit);
extern void MField_Draw(mfield_t *edit, int x, int y, int style, const vec4_t color);
extern void MenuField_Init(menufield_s *m);
extern void MenuField_Draw(menufield_s *f);
extern sfxHandle_t MenuField_Key(menufield_s *m, int *key);

//
// ui_main.c
//
extern void UI_RegisterCvars(void);
extern void UI_UpdateCvars(void);
#define UI_GetCvarInt(cv) (int)trap_Cvar_VariableValue(cv)

//
// ui_menu.c
//
extern void UI_MainMenu_Cache(void);
extern void UI_MainMenu(void);

//
// ui_credits.c
//
extern void UI_Credit_Cache(void);
extern void UI_CreditMenu(void);
extern void UI_WopCredits_Cache(void);
extern void UI_WopCreditsMenu(void);

//
// ui_ingame.c
//
extern void UI_InGame_Cache(void);
extern void UI_InGameMenu(void);

//
// ui_callvote.c
//
extern void UI_CallVoteMenu(void);

//
// ui_musicbox.c
//
void UI_MusicBox_Init(void);
void UI_MusicBox_Shutdown(void);
void UI_MusicBox_Open(void);
void UI_MusicBox_Cache(void);
void UI_MusicBox_Check(void);
void UI_MusicBox_TriggerRestart(void);
void UI_MusicBox_NextTrack(void);

//
// ui_confirm.c
//
extern void UI_ConfirmMenu_Cache(void);
extern void UI_ConfirmMenu(const char *question, void (*draw)(void), void (*action)(qboolean result));

//
// ui_setup.c
//
extern void UI_SetupMenu_Cache(void);
extern void UI_SetupMenu(void);
extern void UI_SetupDefaultMenu_Cache(void);

//
// ui_team.c
//
extern void UI_TeamMainMenu(void);
extern void UI_TeamMain_Cache(void);

//
// ui_connect.c
//
extern void UI_DrawConnectScreen(qboolean overlay);

//
// ui_controls.c
//
extern void UI_ControlsMenu(void);
extern void UI_Controls_Cache(void);
extern void UI_SetDefaultBinds_onUnusedKeys(void);

//
// ui_demos.c
//
extern void UI_DemosMenu(void);
extern void UI_Demos_Cache(void);

//
// ui_mods.c
//
extern void UI_ModsMenu(void);
extern void UI_ModsMenu_Cache(void);

//
// ui_password.c
//
extern void UI_SpecifyPasswordMenu(const char *string, const char *name);

//
// ui_playersettings.c
//
extern void UI_PlayerSettingsMenu(void);
extern void UI_PlayerSettings_Cache(void);

//
// ui_preferences.c
//
extern void UI_PreferencesMenu(void);
extern void UI_Preferences_Cache(void);

//
// ui_secret.c
//
extern void UI_SecretMenu(void);
extern void UI_Secret_Cache(void);

//
// ui_specifyserver.c
//
extern void UI_SpecifyServerMenu(void);
extern void UI_SpecifyServer_Cache(void);

//
// ui_servers.c
//
#define MAX_FAVORITESERVERS 16

extern void UI_ArenaServersMenu(void);
extern void UI_ArenaServers_Cache(void);

//
// ui_startserver2.c
//
extern void UI_StartServerMenu(qboolean multiplayer);
extern void UI_StartServer_Cache(void);
extern void UI_SelectBotsMenu(void);
extern void UI_SelectBots_Cache(void);

//
// ui_serverinfo.c
//
extern void UI_ServerInfoMenu(void);
extern void UI_ServerInfo_Cache(void);

//
// ui_video.c
//
extern void UI_GraphicsOptionsMenu(void);
extern void UI_GraphicsOptions_Cache(void);

//
// ui_players.c
//

// FIXME ripped from cg_local.h
typedef struct {
	int oldFrame;
	int oldFrameTime; // time when ->oldFrame was exactly on

	int frame;
	int frameTime; // time when ->frame will be exactly on

	float backlerp;

	float yawAngle;
	qboolean yawing;
	float pitchAngle;
	qboolean pitching;

	int animationNumber; // may include ANIM_TOGGLEBIT
	animation_t *animation;
	int animationTime; // time when the first frame of the animation will be exact
} lerpFrame_t;

typedef struct {
	// model info
	qhandle_t legsModel;
	qhandle_t legsSkin;
	lerpFrame_t legs;

	qhandle_t torsoModel;
	qhandle_t torsoSkin;
	lerpFrame_t torso;

	qhandle_t headModel;
	qhandle_t headSkin;

	animation_t animations[MAX_ANIMATIONS];

	vec3_t modeloffset;
	float modelscale;

	qboolean fixedlegs;	 // true if legs yaw is always the same as torso yaw
	qboolean fixedtorso; // true if torso never changes yaw

	qhandle_t weaponModel;
	qhandle_t barrelModel;
	qhandle_t flashModel;
	vec3_t flashDlightColor;
	int muzzleFlashTime;

	// currently in use drawing parms
	vec3_t viewAngles;
	vec3_t moveAngles;
	weapon_t currentWeapon;
	int legsAnim;
	int torsoAnim;

	// animation vars
	weapon_t weapon;
	weapon_t lastWeapon;
	weapon_t pendingWeapon;
	int weaponTimer;
	int pendingLegsAnim;
	int torsoAnimationTimer;

	int pendingTorsoAnim;
	int legsAnimationTimer;

	qboolean chat;
	qboolean newModel;

	qboolean barrelSpinning;
	float barrelAngle;
	int barrelTime;

	int realWeapon;

	qboolean glowModel;
	byte glowColor[4];
} playerInfo_t;

void UI_DrawPlayer(float x, float y, float w, float h, playerInfo_t *pi, int time);
void UI_PlayerInfo_SetModel(playerInfo_t *pi, const char *model);
void UI_PlayerInfo_SetInfo(playerInfo_t *pi, int legsAnim, int torsoAnim, const vec3_t viewAngles, const vec3_t moveAngles,
						   weapon_t weaponNum, qboolean chat);
qboolean UI_RegisterClientModelname(playerInfo_t *pi, const char *modelSkinName);

typedef enum {
	MUSICSTATE_STOPPED,			  // no music is running
	MUSICSTATE_RUNNING,			  // ui background music
	MUSICSTATE_RUNNING_MUSIC_MENU // music was started from the music menu
} music_state_t;

//
// ui_atoms.c
//
typedef struct {
	int frametime;
	int realtime;
	int cursorx;
	int cursory;
	int menusp;
	menuframework_s *activemenu;
	menuframework_s *stack[MAX_MENUDEPTH];
	glconfig_t glconfig;
	qboolean debug;
	qhandle_t whiteShader;
	qhandle_t menuBackShader;
	qhandle_t menuBackNoLogoShader;

	menulist_s *dropdownlist;
	vec4_t dropdownxywh;
	music_state_t musicstate;
	qhandle_t connectbg;

	qhandle_t menubgfx; // other menu bg fx shader
	qhandle_t mainbgfx; // main menu bg fx shader
	qhandle_t mainbg;
	qhandle_t setupbg;
	qhandle_t systembg;
	qhandle_t defaultsbg;
	qhandle_t controlsbg;
	qhandle_t playerbg;
	qhandle_t serversbg;
	qhandle_t modsbg;
	qhandle_t demosbg;
	qhandle_t specifybg;
	qhandle_t preferencesbg;
	qhandle_t startserverbg;
	qhandle_t selectbotsbg;
	qhandle_t ingamebg;

	char spraylogoNames[MAX_SPRAYLOGOS_LOADED][MAX_SPRAYLOGO_NAME];
	qhandle_t spraylogoShaders[MAX_SPRAYLOGOS_LOADED];
	int spraylogosLoaded;

	qhandle_t charsetShader;
	qhandle_t charsetProp;
	qhandle_t cursor;
	qhandle_t rb_on;
	qhandle_t rb_off;
	float xscale;
	float yscale;
	float xbias;
	float ybias;

	qboolean demoversion;
	qboolean firstdraw;
} uiStatic_t;

extern void UI_Init(void);
extern void UI_Shutdown(void);
extern void UI_KeyEvent(int key, int down);
extern void UI_MouseEvent(int dx, int dy);
extern void UI_Refresh(int realtime);
extern qboolean UI_ConsoleCommand(int realTime);
extern float UI_ClampCvar(float min, float max, float value);
extern void UI_DrawNamedPic(float x, float y, float width, float height, const char *picname);
extern void UI_DrawHandlePic(float x, float y, float w, float h, qhandle_t hShader);
extern void UI_FillRect(float x, float y, float width, float height, const float *color);
extern void UI_DrawRect(float x, float y, float width, float height, const float *color, int thickness);

extern void UI_StartMusic(void);
extern void UI_StartCreditMusic(void);
extern void UI_StopMusic(void);
extern void UI_ModelIcon(const char *modelAndSkin, char *iconName, int SizeOfIconName);
extern void UI_DrawStringNS(int x, int y, const char *str, int style, float fontsize, const vec4_t color);
extern void UI_DrawIngameBG(void);

extern void UI_UpdateScreen(void);
extern void UI_SetColor(const float *rgba);
extern void UI_LerpColor(const vec4_t a, const vec4_t b, vec4_t c, float t);
extern float UI_ProportionalSizeScale(int style);
extern void UI_DrawProportionalString(int x, int y, const char *str, int style, const vec4_t color);
extern void UI_DrawString_AutoWrapped(int x, int ystart, int xmax, int ystep, const char *str, int style, const vec4_t color,
									  qboolean proportinal);
extern int UI_AutoWrappedString_LineCount(int width, const char *str, int style, qboolean proportional);

extern void UI_DrawProportionalString2(int x, int y, const char *str, const vec4_t color, float sizeScale, qhandle_t charset);

extern int UI_ProportionalStringWidth(const char *str);
extern void UI_DrawString(int x, int y, const char *str, int style, const vec4_t color);
extern void UI_DrawChar(int x, int y, int ch, int style, const vec4_t color);
extern qboolean UI_CursorInRect(int x, int y, int width, int height);
extern void UI_AdjustFrom640(float *x, float *y, float *w, float *h);
extern void UI_DrawTextBox(int x, int y, int width, int lines);
extern qboolean UI_IsFullscreen(void);
extern void UI_SetActiveMenu(uiMenuCommand_t menu);
extern void UI_PushMenu(menuframework_s *menu);
extern void UI_PopMenu(void);
extern void UI_ForceMenuOff(void);
extern const char *UI_Argv(int arg);
extern const char *UI_Cvar_VariableString(const char *var_name);
extern void UI_Refresh(int time);
extern void UI_StartDemoLoop(void);
extern uiStatic_t uis;

extern int QDECL BotListCompare(const void *arg1, const void *arg2);

//
// ui_syscalls.c
//
void trap_Print(const char *string);
void trap_Error(const char *string) Q_NORETURN;
int trap_Milliseconds(void);
void trap_Cvar_Register(vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags);
void trap_Cvar_Update(vmCvar_t *vmCvar);
void trap_Cvar_Set(const char *var_name, const char *value);
float trap_Cvar_VariableValue(const char *var_name);
void trap_Cvar_VariableStringBuffer(const char *var_name, char *buffer, int bufsize);
void trap_Cvar_SetValue(const char *var_name, float value);
void trap_Cvar_Reset(const char *name);
void trap_Cvar_Create(const char *var_name, const char *var_value, int flags);
void trap_Cvar_InfoStringBuffer(int bit, char *buffer, int bufsize);
int trap_Argc(void);
void trap_Argv(int n, char *buffer, int bufferLength);
void trap_Cmd_ExecuteText(int exec_when, const char *text); // don't use EXEC_NOW!
int trap_FS_FOpenFile(const char *qpath, fileHandle_t *f, fsMode_t mode);
void trap_FS_Read(void *buffer, int len, fileHandle_t f);
void trap_FS_Write(const void *buffer, int len, fileHandle_t f);
void trap_FS_FCloseFile(fileHandle_t f);
int trap_FS_GetFileList(const char *path, const char *extension, char *listbuf, int bufsize);
int trap_FS_Seek(fileHandle_t f, long offset, int origin); // fsOrigin_t
qhandle_t trap_R_RegisterModel(const char *name);
qhandle_t trap_R_RegisterSkin(const char *name);
qhandle_t trap_R_RegisterShaderNoMip(const char *name);
void trap_R_ClearScene(void);
void trap_R_AddRefEntityToScene(const refEntity_t *re);
void trap_R_AddPolyToScene(qhandle_t hShader, int numVerts, const polyVert_t *verts);
void trap_R_AddLightToScene(const vec3_t org, float intensity, float r, float g, float b);
void trap_R_RenderScene(const refdef_t *fd);
void trap_R_SetColor(const float *rgba);
void trap_R_DrawStretchPic(float x, float y, float w, float h, float s1, float t1, float s2, float t2,
						   qhandle_t hShader);
void trap_UpdateScreen(void);
int trap_CM_LerpTag(orientation_t *tag, clipHandle_t mod, int startFrame, int endFrame, float frac,
					const char *tagName);
void trap_S_StartLocalSound(sfxHandle_t sfx, int channelNum);
sfxHandle_t trap_S_RegisterSound(const char *sample, qboolean compressed);
void trap_Key_KeynumToStringBuf(int keynum, char *buf, int buflen);
void trap_Key_GetBindingBuf(int keynum, char *buf, int buflen);
void trap_Key_SetBinding(int keynum, const char *binding);
qboolean trap_Key_IsDown(int keynum);
qboolean trap_Key_GetOverstrikeMode(void);
void trap_Key_SetOverstrikeMode(qboolean state);
void trap_Key_ClearStates(void);
int trap_Key_GetCatcher(void);
void trap_Key_SetCatcher(int catcher);
void trap_GetClipboardData(char *buf, int bufsize);
void trap_GetClientState(uiClientState_t *state);
void trap_GetGlconfig(glconfig_t *glconfig);
int trap_GetConfigString(int index, char *buff, int buffsize);
int trap_LAN_GetServerCount(int source);
void trap_LAN_GetServerAddressString(int source, int n, char *buf, int buflen);
void trap_LAN_GetServerInfo(int source, int n, char *buf, int buflen);
int trap_LAN_GetPingQueueCount(void);
int trap_LAN_ServerStatus(const char *serverAddress, char *serverStatus, int maxLen);
void trap_LAN_ClearPing(int n);
void trap_LAN_GetPing(int n, char *buf, int buflen, int *pingtime);
void trap_LAN_GetPingInfo(int n, char *buf, int buflen);
int trap_MemoryRemaining(void);
void trap_GetCDKey(char *buf, int buflen);
void trap_SetCDKey(char *buf);

void trap_S_StopBackgroundTrack(void);
void trap_S_StartBackgroundTrack(const char *intro, const char *loop);
int trap_RealTime(qtime_t *qtime);

int trap_CIN_PlayCinematic(const char *arg0, int xpos, int ypos, int width, int height, int bits);
e_status trap_CIN_RunCinematic(int handle);
void trap_CIN_DrawCinematic(int handle);
e_status trap_CIN_StopCinematic(int handle);
void trap_CIN_SetExtents(int handle, int x, int y, int w, int h);

qboolean trap_VerifyCDKey(const char *key, const char *chksum); // bk001208 - RC4

void trap_SetPbClStatus(int status);

int trap_GetVoiceMuteClient(int client);
int trap_GetVoiceMuteAll(void);
float trap_GetVoiceGainClient(int client);

//
// ui_addbots.c
//
void UI_AddBots_Cache(void);
void UI_AddBotsMenu(void);

//
// ui_removebots.c
//
void UI_RemoveBots_Cache(void);
void UI_RemoveBotsMenu(void);

void UI_VoiceIngame(void);

//
// ui_teamorders.c
//
extern void UI_TeamOrdersMenu(void);
extern void UI_TeamOrdersMenu_f(void);
extern void UI_TeamOrdersMenu_Cache(void);

//
// ui_loadconfig.c
//
void UI_LoadConfig_Cache(void);
void UI_LoadConfigMenu(void);

//
// ui_saveconfig.c
//
void UI_SaveConfigMenu_Cache(void);
void UI_SaveConfigMenu(void);

//
// ui_display.c
//
void UI_DisplayOptions_Cache(void);
void UI_DisplayOptionsMenu(void);

//
// ui_effects.c
//
void UI_EffectsOptions_Cache(void);
void UI_EffectsOptionsMenu(void);

//
// ui_sound.c
//
void UI_SoundOptions_Cache(void);
void UI_SoundOptionsMenu(void);

//
// ui_network.c
//
void UI_NetworkOptions_Cache(void);
void UI_NetworkOptionsMenu(void);

//
// ui_gameinfo.c
//
typedef enum {
	AWARD_ACCURACY,
	AWARD_EXCELLENT,
	AWARD_SNACKATTACK,
	AWARD_FRAGS,
	AWARD_PERFECT
} awardType_t;

const char *UI_GetArenaInfoByNumber(int num);
int UI_GetNumArenas(void);

char *UI_GetBotInfoByNumber(int num);
int UI_GetNumBots(void);

void UI_InitGameinfo(void);

//
// ui_help.c
//
extern void UI_HelpMenu(void);
extern void UI_HelpMenu_f(void);
extern void UI_HelpMenu_Cache(void);

#endif
