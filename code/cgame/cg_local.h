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
#include "../qcommon/q_shared.h"
#include "../renderercommon/tr_types.h"
#include "../game/bg_public.h"
#include "cg_public.h"
#include "../client/keycodes.h" // logo choose menu

// The entire cgame module is unloaded and reloaded on each level change,
// so there is NO persistant data between levels on the client side.
// If you absolutely need something stored, it can either be kept
// by the server in the server stored userinfos, or stashed in a cvar.

#define POWERUP_BLINKS 5

#define POWERUP_BLINK_TIME 1000
#define FADE_TIME 200
#define PULSE_TIME 200
#define DAMAGE_DEFLECT_TIME 100
#define DAMAGE_RETURN_TIME 400
#define DAMAGE_TIME 500
#define LAND_DEFLECT_TIME 150
#define LAND_RETURN_TIME 300
#define STEP_TIME 200
#define DUCK_TIME 100
#define PAIN_TWITCH_TIME 200
#define WEAPON_SELECT_TIME 1400
#define ITEM_SCALEUP_TIME 1000
#define ZOOM_TIME 150
#define ITEM_BLOB_TIME 200
#define MUZZLE_FLASH_TIME 20
#define SINK_TIME 1000 // time for fragments to sink into ground before going away
#define ATTACKER_HEAD_TIME 10000
#define REWARD_TIME 3000

#define TRAIL_FADETIME 800 // HERBY

#define PULSE_SCALE 1.5 // amount to scale up the icons when activating

#define MAX_STEP_CHANGE 32

#define MAX_VERTS_ON_POLY 10
#define MAX_MARK_POLYS 512

#define STAT_MINUS 10 // num frame for '-' stats digit

#define ICON_SIZE 48
#define TEXT_ICON_SPACE 4

#define TEAMCHAT_WIDTH 80
#define TEAMCHAT_HEIGHT 8

// very large characters
#define GIANT_WIDTH 32
#define GIANT_HEIGHT 48

#define NUM_CROSSHAIRS 12

#define TEAM_OVERLAY_MAXNAME_WIDTH 12
#define TEAM_OVERLAY_MAXLOCATION_WIDTH 16

#define DEFAULT_MODEL "padman"
#define DEFAULT_HEADMODEL "padman"
#define DEFAULT_TEAM_MODEL "padman"
#define DEFAULT_TEAM_HEADMODEL "padman"
#define DEFAULT_SKIN "default"
#define GLOW_SKIN "glow"

enum { FORCEMODEL_ENEMY = 1, FORCEMODEL_TEAM = 2 };

typedef enum {
	FOOTSTEP_NORMAL,
	FOOTSTEP_SPURS,
	FOOTSTEP_FLESH,
	FOOTSTEP_MECH,
	FOOTSTEP_ENERGY,
	FOOTSTEP_METAL,
	FOOTSTEP_SPLASH,
	FOOTSTEP_WADE,

	FOOTSTEP_CARPET,
	FOOTSTEP_LATTICE,
	FOOTSTEP_SAND,
	FOOTSTEP_SOFT,
	FOOTSTEP_WOOD,
	FOOTSTEP_SNOW,

	FOOTSTEP_TOTAL
} footstep_t;

typedef enum { IMPACTSOUND_DEFAULT, IMPACTSOUND_METAL, IMPACTSOUND_FLESH } impactSound_t;

//=================================================

// player entities need to track more information
// than any other type of entity.

// note that not every player entity is a client entity,
// because corpses after respawn are outside the normal
// client numbering range

// when changing animation, set animationTime to frameTime + lerping time
// The current lerp will finish out, then it will lerp to the new animation
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
	lerpFrame_t legs, torso, flag;
	int painTime;
	int painDirection; // flip from 0 to 1
	int lightningFiring;

	qboolean railgunFlash;

	// machinegun spinning
	float barrelAngle;
	int barrelTime;
	qboolean barrelSpinning;
} playerEntity_t;

//======================================================================

// local entities are created as a result of events or predicted actions,
// and live independently from all server transmitted entities

typedef enum {
	LE_MARK,
	LE_EXPLOSION,
	LE_SPRITE_EXPLOSION,
	LE_FRAGMENT,
	LE_MOVE_SCALE_FADE,
	LE_FALL_SCALE_FADE,
	LE_FADE_RGB,
	LE_SCALE_FADE,
	LE_SCOREPLUM,

	LE_SPRAYTRAIL,

	LE_TRAIL,
	LE_TRAIL_TRACED_RGB,
	LE_WATERBEAM,
	LE_PUMPERTRAIL,
	LE_IMPERIUSBOOM,
	LE_IMPERIUSRINGS,
	LE_TELEFFECT,
	LE_BOOMIESEXPLOSION,
} leType_t;

typedef enum {
	LEF_PUFF_DONT_SCALE = 0x0001, // do not scale size over time
	LEF_TUMBLE = 0x0002,		  // tumble over time, used for ejecting shells
	LEF_SOUND1 = 0x0004,		  // sound 1 for kamikaze
	LEF_SOUND2 = 0x0008			  // sound 2 for kamikaze

	,
	LEF_AXIS_ALIGNED = 0x0010,
	LEF_COLLISIONS = 0x0020,
	LEF_GRAVITY = 0x0040
} leFlag_t;

typedef enum { LEMT_NONE, LEMT_BURN, LEMT_BLOOD } leMarkType_t; // fragment local entities can leave marks on walls

typedef enum {
	LEBS_NONE,
	LEBS_BRASS
} leBounceSoundType_t; // fragment local entities can make sounds on impacts

typedef struct localEntity_s {
	struct localEntity_s *prev, *next;

	struct localEntity_s *older, *newer;
	int ownerNum;

	leType_t leType;
	int leFlags;

	int startTime;
	int endTime;
	int fadeInTime;

	float lifeRate; // 1.0 / (endTime - startTime)

	trajectory_t pos;
	trajectory_t angles;

	float bounceFactor; // 0.0 = no bounce, 1.0 = perfect

	vec4_t color;

	float radius;

	float light;
	vec3_t lightColor;

	leMarkType_t leMarkType; // mark to leave on fragment impact
	leBounceSoundType_t leBounceSoundType;

	refEntity_t refEntity;
} localEntity_t;

//=================================================

// centity_t have a direct correspondence with gentity_t in the game, but
// only the entityState_t is directly communicated to the cgame
typedef struct centity_s {
	entityState_t currentState; // from cg.frame
	entityState_t nextState;	// from cg.nextFrame, if available
	qboolean interpolate;		// true if next is valid to interpolate to
	qboolean currentValid;		// true if cg.frame holds this entity
	qboolean spawnProtection;	// set by EV_SPAWNPROTECT

	int muzzleFlashTime; // move to playerEntity?
	int previousEvent;
	int teleportFlag;

	int lastweaponframe;
	vec3_t speedyOrigin;
	int nextSpeedytime;
	int revivalEffectTime;
	localEntity_t *trailLE; // HERBY

	int trailTime; // so missile trails can handle dropped initial packets
	int dustTrailTime;
	int miscTime;

	int snapShotTime; // last time this entity was found in a snapshot

	playerEntity_t pe;

	int errorTime; // decay the error from this time
	vec3_t errorOrigin;
	vec3_t errorAngles;

	qboolean extrapolated; // false if origin / angles is an interpolation
	vec3_t rawOrigin;
	vec3_t rawAngles;

	vec3_t beamEnd;

	// exact interpolated position of entity on this frame
	vec3_t lerpOrigin;
	vec3_t lerpAngles;
} centity_t;

//======================================================================

// for trail drawing
extern refEntity_t trailEnt;

typedef struct {
	int client;
	int score;
	int ping;
	int time;
	int scoreFlags;
	int powerUps;
	int accuracy;
	int spraygodCount;
	int spraykillerCount;
	int padheroCount;
	int watchpadCount;
	int livesleft;
	int excellentCount;
	int snackattackCount;
	int captures;
	qboolean perfect;
	int team;
} score_t;

// each client has an associated clientInfo_t
// that contains media references necessary to present the
// client model and other color coded effects
// this is regenerated each time a client's configstring changes,
// usually as a result of a userinfo (name, model, etc) change
#define MAX_CUSTOM_SOUNDS 32

typedef struct {
	qboolean infoValid;

	char name[MAX_QPATH];
	team_t team;

	int botSkill; // 0 = not bot, 1-5 = bot

	vec4_t spraycolor;
	vec4_t rndspraycolor;
	int randomcolor;	// random color option

	char spraylogo[MAX_SPRAYLOGO_NAME];

	vec3_t realmuzzle; // for boaster-trail
	vec3_t curPos;	   // for LPS
	int lastPosSaveTime;
	int lastTeleInTime;
	int lastWaterClearTime;

	int score;	  // updated by score servercmds
	int location; // location index for team mode
	int health;	  // you only get this info about your teammates
	int armor;
	int curWeapon;
	int numCartridges;

	int handicap;
	int wins, losses; // in tourney mode

	qboolean teamLeader; // true when this is a team leader

	int powerups; // so can display quad/flag status

	int medkitUsageTime;
	int invulnerabilityStartTime;
	int invulnerabilityStopTime;

	int breathPuffTime;

	// when clientinfo is changed, the loading of models/skins/sounds
	// can be deferred until you are dead, to prevent hitches in
	// gameplay
	char modelName[MAX_QPATH];
	char skinName[MAX_QPATH];
	char headModelName[MAX_QPATH];
	char headSkinName[MAX_QPATH];
	//	char			redTeam[MAX_TEAMNAME]; //TODO: remove
	//	char			blueTeam[MAX_TEAMNAME];
	qboolean deferred;

	qboolean fixedlegs;	 // true if legs yaw is always the same as torso yaw
	qboolean fixedtorso; // true if torso never changes yaw

	vec3_t headOffset; // move head in icon views

	float headScale;

	footstep_t footsteps;
	gender_t gender; // from model

	qhandle_t legsModel;
	qhandle_t legsSkin;

	qhandle_t torsoModel;
	qhandle_t torsoSkin;

	qhandle_t headModel;
	qhandle_t headSkin;

	qhandle_t modelIcon;

	animation_t animations[MAX_TOTALANIMATIONS];

	sfxHandle_t sounds[MAX_CUSTOM_SOUNDS];

	qboolean glowModel;
	byte glowColor[4];

	qboolean ftIsFrozen;
	qboolean ctkdIsKillerduck;
} clientInfo_t;

// each WP_* weapon enum has an associated weaponInfo_t
// that contains media references necessary to present the
// weapon and its effects
typedef struct weaponInfo_s {
	qboolean registered;
	const gitem_t *item;

	qhandle_t handsModel; // the hands don't actually draw, they just position the weapon
	qhandle_t weaponModel;
	qhandle_t barrelModel;
	qhandle_t flashModel;

	vec3_t weaponMidpoint; // so it will rotate centered instead of by tag

	float flashDlight;
	vec3_t flashDlightColor;
	sfxHandle_t flashSound[4]; // fast firing weapons randomly choose

	qhandle_t weaponIcon;
	qhandle_t ammoIcon;

	qhandle_t ammoModel;

	qhandle_t missileModel;
	sfxHandle_t missileSound;
	void (*missileTrailFunc)(centity_t *, const struct weaponInfo_s *wi);
	float missileDlight;
	vec3_t missileDlightColor;
	int missileRenderfx;

	void (*ejectBrassFunc)(centity_t *);

	float trailRadius;
	float wiTrailTime;

	sfxHandle_t readySound;
	sfxHandle_t firingSound;
} weaponInfo_t;

// each IT_* item has an associated itemInfo_t
// that constains media references necessary to present the
// item and its effects
typedef struct {
	qboolean registered;
	qhandle_t models[MAX_ITEM_MODELS];
	qhandle_t icon;
} itemInfo_t;

typedef struct {
	int itemNum;
} powerupInfo_t;

#define MAX_CARTRIDGETRAIL 10

typedef struct {
	vec3_t positions[MAX_CARTRIDGETRAIL];
	int numpositions;
} cartridgetrail_t;

#define MAX_REWARDSTACK 10
#define MAX_SOUNDBUFFER 20

//======================================================================

// all cg.stepTime, cg.duckTime, cg.landTime, etc are set to cg.time when the action
// occurs, and they will have visible effects for #define STEP_TIME or whatever msec after

#define MAX_PREDICTED_EVENTS 16

#define MAX_CHATMESSAGES 8

typedef struct {
	int clientFrame; // incremented each frame

	int clientNum;

	qboolean demoPlayback;
	qboolean levelShot; // taking a level menu screenshot
	int deferredPlayerLoading;
	qboolean loading;			  // don't defer players at initial startup
	qboolean intermissionStarted; // don't play voice rewards, because game will end shortly

	// there are only one or two snapshot_t that are relevant at a time
	int latestSnapshotNum;	// the number of snapshots the client system has received
	int latestSnapshotTime; // the time from latestSnapshotNum, so we don't need to read the snapshot yet

	snapshot_t *snap;	  // cg.snap->serverTime <= cg.time
	snapshot_t *nextSnap; // cg.nextSnap->serverTime > cg.time, or NULL
	snapshot_t activeSnapshots[2];

	float frameInterpolation; // (float)( cg.time - cg.frame->serverTime ) / (cg.nextFrame->serverTime -
							  // cg.frame->serverTime)

	qboolean thisFrameTeleport;
	qboolean nextFrameTeleport;

	int frametime; // cg.time - cg.oldTime

	int time; // this is the time value that the client
			  // is rendering at.
	int oldTime; // time at last frame, used for missile trails and prediction checking
	int millis; // current frame milliseconds

	int physicsTime; // either cg.snap->time or cg.nextSnap->time

	int timelimitWarnings; // 5 min, 1 min, overtime
	int fraglimitWarnings;
	int lifelimitWarnings;

	qboolean wantSelectLogo;
	int handleInputMillis;

	int first2dtime;
	char wopSky[128];
	vec3_t wopSky_Angles;
	vec3_t wopSky_TimeFactors;

	char skylensflare[128];
	vec3_t skylensflare_dir;

	// History of player chat and icons
	// Used (instead of code in cl_console) when con_notifytime <= 0
	char chattext[MAX_CHATMESSAGES][MAX_SAY_TEXT];
	qhandle_t chaticons[MAX_CHATMESSAGES];
	int chatmsgtime[MAX_CHATMESSAGES];
	int lastchatmsg;

	float speedyeffect;
	int sprayyourcolortime;

	// imperius
	int imp_lastwarningsound;
	int imp_startloading;

	qboolean mapRestart; // set on a map restart to set back the weapon

	qboolean renderingThirdPerson; // during deaths, chasecams, etc
	qboolean wasThirdPerson; // during weapon zoom when third person view was enabled

	// prediction state
	qboolean hyperspace; // true if prediction has hit a trigger_teleport
	playerState_t predictedPlayerState;
	centity_t predictedPlayerEntity;
	qboolean validPPS; // clear until the first call to CG_PredictPlayerState
	int predictedErrorTime;
	vec3_t predictedError;

	int eventSequence;
	int predictableEvents[MAX_PREDICTED_EVENTS];

	float stepChange; // for stair up smoothing
	int stepTime;

	float duckChange; // for duck viewheight smoothing
	int duckTime;

	float landChange; // for landing hard
	int landTime;

	// input state sent to server
	int weaponSelect;

	// auto rotating items
	vec3_t autoAngles;
	vec3_t autoAxis[3];
	vec3_t autoAnglesFast;
	vec3_t autoAxisFast[3];

	vec3_t autoAnglesPadPowerups;
	vec3_t autoAxisPadPowerups[3];

	// view rendering
	refdef_t refdef;
	vec3_t refdefViewAngles; // will be converted to refdef.viewaxis

	// zoom key
	float zoomfactor;
	qboolean zoomedkeydown;
	int zoomSoundStat;

	qboolean zoomed;
	int zoomTime;
	float zoomSensitivity;

	// information screen text during loading
	char infoScreenText[MAX_STRING_CHARS];

	float loadingprogress;

	//	int			numFFAplayers;
	qboolean playedIntermissionMsg;

	int scoreTeamCount[TEAM_NUM_TEAMS];

	// scoreboard
	int scoresRequestTime;
	int numScores;
	int selectedScore;
	int teamScores[2];
	score_t scores[MAX_CLIENTS];
	qboolean showScores;
	qboolean scoreBoardShowing;
	int scoreFadeTime;
	char killerName[MAX_NAME_LENGTH];
	char spectatorList[MAX_STRING_CHARS]; // list of names
	int spectatorLen;					  // length of list
	float spectatorWidth;				  // width in device units
	int spectatorTime;					  // next time to offset
	int spectatorPaintX;				  // current paint x
	int spectatorPaintX2;				  // current paint x
	int spectatorOffset;				  // current offset from start
	int spectatorPaintLen;				  // current offset from start

	// skull trails
	cartridgetrail_t cartridgeTrail[MAX_CLIENTS];

	// centerprinting
	int centerPrintTime;
	int centerPrintY;
	char centerPrint[1024];
	int centerPrintLines;

	// message printing
	int messagePrintTime;
	char messagePrint[128];

	// low ammo warning state
	int lowAmmoWarning; // 1 = low, 2 = empty

	// crosshair client ID
	int crosshairClientNum;
	int crosshairClientTime;

	// powerup active flashing
	int powerupActive;
	int powerupTime;

	// attacking player
	int attackerTime;

	// reward medals
	int rewardStack;
	int rewardTime;
	int rewardCount[MAX_REWARDSTACK];
	qhandle_t rewardShader[MAX_REWARDSTACK];
	qhandle_t rewardSound[MAX_REWARDSTACK];

	// sound buffer mainly for announcer sounds
	int soundBufferIn;
	int soundBufferOut;
	int soundTime;
	qhandle_t soundBuffer[MAX_SOUNDBUFFER];

	// warmup countdown
	int warmup;
	int warmupCount;

	//==========================

	int itemPickup;
	int itemPickupTime;
	int itemPickupBlendTime; // the pulse around the crosshair is timed separately

	int weaponSelectTime;
	int weaponAnimation;
	int weaponAnimationTime;

	// blend blobs
	float damageTime;
	float damageX, damageY, damageValue;

	// status bar head
	float headYaw;
	float headEndPitch;
	float headEndYaw;
	int headEndTime;
	float headStartPitch;
	float headStartYaw;
	int headStartTime;

	// view movement
	float v_dmg_time;
	float v_dmg_pitch;
	float v_dmg_roll;

	// temp working variables for player view
	float bobfracsin;
	int bobcycle;
	float xyspeed;
	int nextOrbitTime;

	// qboolean cameraMode;		// if rendering from a loaded camera

	// development tool
	refEntity_t testModelEntity;
	char testModelName[MAX_QPATH];
	qboolean testGun;

	int lastVoiceTime[MAX_CLIENTS];
	// cammod
	vec3_t CamPos;
	vec3_t CamAngles;
	qboolean Cam;
} cg_t;

// all of the model, shader, and sound references that are
// loaded at gamestate time are stored in cgMedia_t
// Other media that can be tied to clients, weapons, or items are
// stored in the clientInfo_t, itemInfo_t, weaponInfo_t, and powerupInfo_t
typedef struct {
	qhandle_t charsetShader;
	qhandle_t charsetProp;
	qhandle_t whiteShader;

	qhandle_t redFlagModel;
	qhandle_t blueFlagModel;
	qhandle_t neutralFlagModel;
	qhandle_t redFlagShader[3];
	qhandle_t blueFlagShader[3];
	qhandle_t neutralflagShader[3];

	qhandle_t armorModel;
	qhandle_t armorIcon;

	//	qhandle_t	teamStatusBar;

	qhandle_t deferShader;

	qhandle_t lightningShader;

	qhandle_t friendShader;

	qhandle_t balloonShader;
	qhandle_t connectionShader;

	qhandle_t selectShader;
	qhandle_t viewBloodShader;

	qhandle_t BloodScreenShader;
	qhandle_t BerserkerScreenShader;
	qhandle_t WetScreenShader;

	// freezetag
	qhandle_t FreezeScreenShader;
	qhandle_t iceblockModel;
	qhandle_t freezeIconShader;
	qhandle_t thawIcon;
	qhandle_t nipperMuzzleFTShader;
	qhandle_t pumperMuzzleFTShader;
	qhandle_t bettyMuzzleFTShader;
	qhandle_t bettySkinFTShader;
	qhandle_t bubblegMuzzleFTShader;
	qhandle_t bubblegSkinFTShader;

	// catch the killerduck
	qhandle_t ctkdHintIconShader;
	qhandle_t ctkdCarrierIconShader;

	qhandle_t tracerShader;
	qhandle_t crosshairShader[NUM_CROSSHAIRS];
	qhandle_t lagometerShader;
	qhandle_t backTileShader;
	qhandle_t noammoShader;

	qhandle_t smokePuffShader;
	qhandle_t plasmaBallShader;
	qhandle_t waterBubbleShader;
	qhandle_t bloodTrailShader;
	qhandle_t revivalParticleShader;

	qhandle_t numberShaders[11];

	qhandle_t shadowMarkShader;

	qhandle_t botSkillShaders[5];

	// surface damage mark shaders
	qhandle_t burnMarkShader;
	qhandle_t energyMarkShader;
	qhandle_t foamMarkShader;
	qhandle_t gumMarkShader;
	qhandle_t snowMarkShader;
	qhandle_t wakeMarkShader;
	qhandle_t waterMarkShader;

	// powerup shaders
	qhandle_t invisShader;

	qhandle_t lpsIcon;
	qhandle_t lpsIconLead;

	qhandle_t bbBoxIcon;
	qhandle_t healthstationIcon;
	qhandle_t sprayroomIcon;

	qhandle_t neutralSpraypistolskin;
	qhandle_t neutralSpraypistolicon;

	qhandle_t blueSpraypistolskin;
	qhandle_t blueSpraypistolicon;

	qhandle_t defaultspraylogo;

	int blueCartridgeEntNum, redCartridgeEntNum, neutralCartridgeEntNum;

	qhandle_t spraypuff;
	qhandle_t spraymark;
	qhandle_t arrowlt0;
	qhandle_t arrowlt1;
	qhandle_t arrowrt0;
	qhandle_t arrowrt1;
	qhandle_t cgwopmenu_cursor;
	qhandle_t chooselogo_bg;

	// hud
	qhandle_t hud_bl[10];
	qhandle_t hud_bc[10];
	qhandle_t hud_br[10];
	//:HERBY:ea
	qhandle_t hud_bk_balloon_red;
	qhandle_t hud_bk_balloon_blue;
	qhandle_t hud_balloon;
	qhandle_t hud_balloon_bar;
	//:HERBY:ee

	qhandle_t hud_CTL_bg_red;
	qhandle_t hud_CTL_bg_blue;

	qhandle_t hud_teammarker;
	qhandle_t hud_shieldbar;
	qhandle_t hud_energybar;
	qhandle_t hud_shieldbar2;
	qhandle_t hud_energybar2;
	qhandle_t hud_shieldglass;
	qhandle_t hud_energyglass;
	qhandle_t hud_dotfull;
	qhandle_t hud_dotempty;

	// zoom
	qhandle_t zoomhud; // splasher
	qhandle_t zoomruler;
	qhandle_t zoomcompass;
	sfxHandle_t zoomsound[9];
	qhandle_t zoomhud_kma; // kma
	qhandle_t zoomKMAaura;
	qhandle_t zoomKMAbluescreen;

	qhandle_t StationRingShader;
	qhandle_t PadPowerShader;
	qhandle_t PadPowerPunchySkin;
	qhandle_t BerserkerAura;
	qhandle_t deadfadeSkin;
	sfxHandle_t station_start;
	sfxHandle_t station_loop;
	sfxHandle_t station_end;

	sfxHandle_t station_empty;
	sfxHandle_t speedyTaunt;
	//	sfxHandle_t	speedyLoop;
	sfxHandle_t ghostPadSpawn;
	sfxHandle_t ghostPadSpook;

	sfxHandle_t pickupSound; // for weapons
	sfxHandle_t HIpickupSound;
	sfxHandle_t ARpickupSound;
	// sta
	qhandle_t HealthStation_Base;
	qhandle_t HealthStation_Cross;
	qhandle_t HealthStation_Ring;
	qhandle_t StationLoadingRings;
	// ste
	qhandle_t GhostPadModel;

	// HERBY vv
	qhandle_t star;
	qhandle_t fireBallShader;
	qhandle_t fireTrailShader;
	qhandle_t fireExplosionShader;
	qhandle_t waterTrailShader;
	qhandle_t waterBallShader;
	qhandle_t waterBeamShader;
	qhandle_t splasherTrailShader;
	qhandle_t bubbleGumShader;
	qhandle_t nipperBallShader;
	qhandle_t nipperWaveShader;
	qhandle_t pumperTrailShader;
	qhandle_t pumperFlashModel;
	qhandle_t pumperFlashShader;
	qhandle_t imperiusCoreShader;
	qhandle_t imperiusRingShader;
	qhandle_t imperiusBeamModel;

	qhandle_t bigDropModel;
	qhandle_t smallDropModel;
	qhandle_t boasterExplosionShader;
	qhandle_t waterExplosionShader;
	qhandle_t fireDropModel;
	qhandle_t fireDropShader;
	qhandle_t imperiusSphereModel;
	qhandle_t imperiusRingsModel;
	// HERBY ^^
	qhandle_t duckExplosionShader;
	sfxHandle_t duckExplosionSound;
	//:HERBY:ea
	qhandle_t duckWheelModel;
	qhandle_t duckHeadModel;
	//:HERBY:ee

	// BAMBAM
	qhandle_t bambamMissileRedShader;
	qhandle_t bambamMissileBlueShader;

	qhandle_t bambamImpactDrops;
	qhandle_t bambamImpactDropsRedShader;
	qhandle_t bambamImpactDropsBlueShader;

	qhandle_t bambamExplosionLeg;
	qhandle_t bambamExplosionTorso;

	sfxHandle_t bambamExplosionSound;
	sfxHandle_t boomiesExplosionSound;

	sfxHandle_t bambamMissileImpact;

	qhandle_t boomiesSphereModel;
	qhandle_t boomiesCoreShader;

	qhandle_t bambamHealthIcon;
	qhandle_t bambamHealthIconBG;

	// KMA
	qhandle_t kmaTrailShader;
	qhandle_t kmaBallShader;
	qhandle_t smallKmaDropModel;
	qhandle_t kmaMarkShader;

	// weapon effect models
	qhandle_t dishFlashModel;

	// special effects models
	qhandle_t teleportEffectModel;

	//	qhandle_t	teleportEffectShader;
	qhandle_t teleportEffectBlueShader;
	qhandle_t teleportEffectRedShader;
	qhandle_t teleportEffectGreenShader;
	qhandle_t teleEffectFPBlueShader;
	qhandle_t teleEffectFPRedShader;
	qhandle_t teleEffectFPGreenShader;

	// scoreboard headers
	qhandle_t scoreboardBG;
	qhandle_t scoreboardLives;
	qhandle_t scoreboardScoreLives;
	qhandle_t scoreboardName;
	qhandle_t scoreboardPing;
	qhandle_t scoreboardScore;
	qhandle_t scoreboardTime;
	qhandle_t scoreboardMedals;
	qhandle_t scoreboardAccuracy;

	qhandle_t voiceIcon;

	// medals shown during gameplay
	qhandle_t medalExcellent;
	qhandle_t medalSnackAttack;
	qhandle_t medalSprayGod;
	qhandle_t medalSprayKiller;
	qhandle_t medalPadHero;
	qhandle_t medalWatchPad;
	qhandle_t medalPadStar;
	qhandle_t medalPadStarRed;

	sfxHandle_t berserkerPunchyLoop;

	sfxHandle_t sfx_imperiusexp;

	sfxHandle_t imperiuswarning;
	sfxHandle_t imperiusloading[13];
	sfxHandle_t pumperexpSound;
	sfxHandle_t BalloonyExplosion;
	sfxHandle_t BettyExplosion;
	//:HERBY:ea
	sfxHandle_t balloonInflateSound;
	sfxHandle_t balloonBurstSound;
	//:HERBY:ee

	// sounds
	sfxHandle_t quadSound;
	sfxHandle_t selectSound;
	sfxHandle_t useNothingSound;
	sfxHandle_t wearOffSound;
	sfxHandle_t footsteps[FOOTSTEP_TOTAL][4];

	sfxHandle_t teleInSound;
	sfxHandle_t teleOutSound;
	sfxHandle_t noAmmoSound;
	sfxHandle_t respawnSound;
	sfxHandle_t talkSound;
	sfxHandle_t landSound;
	sfxHandle_t fallSound;
	sfxHandle_t jumpPadSound;

	sfxHandle_t dropItemSound;
	sfxHandle_t jumperSound;

	sfxHandle_t countSprayRoomSound[10];
	sfxHandle_t fiveMinutesToPlaySound;
	sfxHandle_t gameOverSound;
	sfxHandle_t oneMinuteToPlaySound;
	sfxHandle_t tenSecondsToLeaveSound;

	sfxHandle_t welcomeSound[3];
	sfxHandle_t byeSprayRoomSound[2];
	sfxHandle_t sprayYourColorSound;

	//:HERBY:ea
	sfxHandle_t redBalloonSound;
	sfxHandle_t blueBalloonSound;
	sfxHandle_t redBigBalloonSound;
	sfxHandle_t blueBigBalloonSound;
	sfxHandle_t balloonDestroyedSound;
	//:HERBY:ee

	sfxHandle_t wrongSprayWallSound[4];
	sfxHandle_t loseFFASound;
	sfxHandle_t loseLPSSound;
	sfxHandle_t winLPSSound[2];

	sfxHandle_t suddenDeathSound;

	sfxHandle_t threeFragsSound;
	sfxHandle_t twoFragsSound;
	sfxHandle_t oneFragSound;
	sfxHandle_t threeLogosSound;
	sfxHandle_t twoLogosSound;
	sfxHandle_t oneLogoSound;
	sfxHandle_t threeLivesSound;
	sfxHandle_t twoLivesSound;
	sfxHandle_t oneLifeSound;

	sfxHandle_t hitSound;
	sfxHandle_t hitShieldSound;
	sfxHandle_t hitNoShieldSound;
	sfxHandle_t hitTeamSound;
	sfxHandle_t excellentSound;
	sfxHandle_t snackattackSound;
	sfxHandle_t spraygodSound;
	sfxHandle_t spraykillerSound;
	sfxHandle_t padheroSound;
	sfxHandle_t watchpadSound;
	sfxHandle_t padstarSound;

	sfxHandle_t takenLeadSound;
	sfxHandle_t tiedLeadSound;
	sfxHandle_t lostLeadSound;

	sfxHandle_t voteNow;
	sfxHandle_t votePassed;
	sfxHandle_t voteFailed;

	sfxHandle_t watrInSound;
	sfxHandle_t watrOutSound;
	sfxHandle_t watrUnSound;

	sfxHandle_t flightSound;
	sfxHandle_t medkitSound;

	sfxHandle_t weaponHoverSound;

	// teamplay sounds
	//	sfxHandle_t captureAwardSound;
	sfxHandle_t redScoredSound;
	sfxHandle_t blueScoredSound;
	sfxHandle_t redLeadsSound[2];
	sfxHandle_t blueLeadsSound[2];

	sfxHandle_t teamsTiedSound;

	sfxHandle_t captureYourTeamSound;
	sfxHandle_t captureOpponentSound;
	sfxHandle_t returnYourTeamSound;
	sfxHandle_t returnOpponentSound;
	sfxHandle_t takenYourTeamSound;
	sfxHandle_t takenOpponentSound;

	// voc_ sounds:
	sfxHandle_t redLollyReturnedSound;
	sfxHandle_t blueLollyReturnedSound;
	sfxHandle_t neutralLollyReturnedSound;
	sfxHandle_t enemyTookYourLollySound;
	sfxHandle_t yourTeamTookEnemyLollySound;
	sfxHandle_t youHaveLollySound;
	sfxHandle_t enemyTookTheLollySound;
	sfxHandle_t yourTeamTookTheLollySound;

	sfxHandle_t stolenLollySound;
	sfxHandle_t almostCapture;

	// tournament sounds
	sfxHandle_t count3Sound;
	sfxHandle_t count2Sound;
	sfxHandle_t count1Sound;
	sfxHandle_t countFightSound;

	qhandle_t cursor;
	qhandle_t selectCursor;
	qhandle_t sizeCursor;

	sfxHandle_t regenSound;
	sfxHandle_t n_healthSound;
	sfxHandle_t hgrenb1aSound;
	sfxHandle_t hgrenb2aSound;
	sfxHandle_t wstbimplSound;
	sfxHandle_t wstbimpmSound;
	sfxHandle_t wstbimpdSound;
	sfxHandle_t wstbactvSound;

} cgMedia_t;

// The client game static (cgs) structure hold everything
// loaded or calculated from the gamestate.  It will NOT
// be cleared when a tournement restart is done, allowing
// all clients to begin playing instantly
typedef struct {
	gameState_t gameState; // gamestate from server
	glconfig_t glconfig;   // rendering configuration
	float screenXScale;	   // derived from glconfig
	float screenYScale;
	float screenXBias;
	float screenYBias;

	int serverCommandSequence; // reliable command stream counter
	int processedSnapshotNum;  // the number of snapshots cgame has requested

	qboolean localServer; // detected on startup by checking sv_running

	// parsed from serverinfo
	gametype_t gametype;
	int dmflags;
	int teamflags;
	int fraglimit;
	int capturelimit;
	int timelimit;
	int maxclients;
	char mapname[MAX_QPATH];
	char redTeam[MAX_QPATH];
	char blueTeam[MAX_QPATH];

	char shortmapname[MAX_QPATH];
	char servername[32];
	int lpsflags;
	int lpsStartLives;

	int voteTime;
	int voteYes;
	int voteNo;
	qboolean voteModified; // beep whenever changed
	char voteString[MAX_STRING_TOKENS];

	int teamVoteTime[2];
	int teamVoteYes[2];
	int teamVoteNo[2];
	qboolean teamVoteModified[2]; // beep whenever changed
	char teamVoteString[2][MAX_STRING_TOKENS];

	int levelStartTime;

	int scores1, scores2;  // from configstrings
	int redflag, blueflag; // flag status from configstrings
	int flagStatus;
	//:HERBY:ea
	char balloonState[MAX_BALLOONS + 1];
	//:HERBY:ee

	qboolean newHud;

	//
	// locally derived information from gamestate
	//
	qhandle_t gameModels[MAX_MODELS];
	sfxHandle_t gameSounds[MAX_SOUNDS];

	int numInlineModels;
	qhandle_t inlineDrawModel[MAX_MODELS];
	vec3_t inlineModelMidpoints[MAX_MODELS];

	clientInfo_t clientinfo[MAX_CLIENTS];

	// teamchat width is *3 because of embedded color codes
	char teamChatMsgs[TEAMCHAT_HEIGHT][TEAMCHAT_WIDTH * 3 + 1];
	int teamChatMsgTimes[TEAMCHAT_HEIGHT];
	int teamChatPos;
	int teamLastChatPos;

	int cursorX;
	int cursorY;
	qboolean eventHandling;
	qboolean mouseCaptured;
	qboolean sizingHud;
	void *capturedItem;
	qhandle_t activeCursor;
	int lastusedkey;

	// orders
	int currentOrder;
	qboolean orderPending;
	int orderTime;
	int currentVoiceClient;
	int acceptOrderTime;
	int acceptTask;
	int acceptLeader;
	char acceptVoice[MAX_NAME_LENGTH];

	// media
	cgMedia_t media;

} cgs_t;

//==============================================================================

extern cgs_t cgs;
extern cg_t cg;
extern centity_t cg_entities[MAX_GENTITIES];
extern weaponInfo_t cg_weapons[MAX_WEAPONS];
extern itemInfo_t cg_items[MAX_ITEMS];

extern vmCvar_t cg_centertime;
extern vmCvar_t cg_runpitch;
extern vmCvar_t cg_runroll;
extern vmCvar_t cg_bobup;
extern vmCvar_t cg_bobpitch;
extern vmCvar_t cg_bobroll;
extern vmCvar_t cg_swingSpeed;
extern vmCvar_t cg_shadows;
extern vmCvar_t cg_drawTimer;
extern vmCvar_t cg_drawFPS;
extern vmCvar_t cg_drawSnapshot;
extern vmCvar_t cg_draw3dIcons;
extern vmCvar_t cg_mapInfoTime;
extern vmCvar_t cg_showMapInfo;
extern vmCvar_t cg_drawAmmoWarning;
extern vmCvar_t cg_drawCrosshair;
extern vmCvar_t cg_drawTraceCrosshair;
extern vmCvar_t cg_drawCrosshairNames;
extern vmCvar_t cg_drawRewards;
extern vmCvar_t cg_drawTeamOverlay;
extern vmCvar_t cg_fovAspectAdjust;
extern vmCvar_t cg_teamOverlayUserinfo;
extern vmCvar_t cg_crosshairX;
extern vmCvar_t cg_crosshairY;
extern vmCvar_t cg_crosshairSize;
extern vmCvar_t cg_crosshairHealth;
extern vmCvar_t cg_drawStatus;
extern vmCvar_t cg_draw2D;
extern vmCvar_t cg_animSpeed;
extern vmCvar_t cg_debugAnim;
extern vmCvar_t cg_debugPosition;
extern vmCvar_t cg_debugEvents;
extern vmCvar_t cg_errorDecay;
extern vmCvar_t cg_nopredict;
extern vmCvar_t cg_noPlayerAnims;
extern vmCvar_t cg_showmiss;
extern vmCvar_t cg_footsteps;
extern vmCvar_t cg_addMarks;
extern vmCvar_t cg_gun_frame;
extern vmCvar_t cg_gun_x;
extern vmCvar_t cg_gun_y;
extern vmCvar_t cg_gun_z;
extern vmCvar_t cg_drawGun;
extern vmCvar_t cg_viewsize;
extern vmCvar_t cg_tracerChance;
extern vmCvar_t cg_tracerWidth;
extern vmCvar_t cg_tracerLength;
extern vmCvar_t cg_autoswitch;
extern vmCvar_t cg_weaponOrder;
extern vmCvar_t cg_ignore;
extern vmCvar_t cg_simpleItems;
extern vmCvar_t cg_fov;
extern vmCvar_t cg_zoomFov;
extern vmCvar_t cg_thirdPersonRange;
extern vmCvar_t cg_thirdPersonAngle;
extern vmCvar_t cg_thirdPerson;
extern vmCvar_t cg_lagometer;
extern vmCvar_t cg_drawAttacker;
extern vmCvar_t cg_synchronousClients;
extern vmCvar_t cg_chatBeep;
extern vmCvar_t cg_teamChatTime;
extern vmCvar_t cg_teamChatHeight;
extern vmCvar_t cg_stats;
extern vmCvar_t cg_forceModel;
extern vmCvar_t cg_buildScript;
extern vmCvar_t cg_paused;
extern vmCvar_t cg_predictItems;
extern vmCvar_t cg_deferPlayers;
extern vmCvar_t cg_drawFriend;
extern vmCvar_t cg_teamChatsOnly;
extern vmCvar_t cg_scorePlum;
extern vmCvar_t cg_smoothClients;
extern vmCvar_t pmove_fixed;
extern vmCvar_t pmove_msec;
extern vmCvar_t cg_cameraOrbit;
extern vmCvar_t cg_cameraOrbitDelay;
extern vmCvar_t cg_timescaleFadeEnd;
extern vmCvar_t cg_timescaleFadeSpeed;
extern vmCvar_t cg_timescale;
extern vmCvar_t cg_cameraMode;
extern vmCvar_t cg_smallFont;
extern vmCvar_t cg_bigFont;
extern vmCvar_t cg_noTaunt;
extern vmCvar_t cg_noProjectileTrail;
extern vmCvar_t cg_enableBreath;
extern vmCvar_t cg_enableDust;

extern vmCvar_t cg_correctedWeaponPos;
extern vmCvar_t cg_drawRealTime;
extern vmCvar_t cg_printDir;
extern vmCvar_t cg_wopFFAhud;
extern vmCvar_t cg_drawups;
extern vmCvar_t cg_drawMessages;
extern vmCvar_t cg_drawServerInfos;
extern vmCvar_t cg_drawTimeLeft;
extern vmCvar_t cg_bigScoreType;

extern vmCvar_t cg_cineHideHud;
extern vmCvar_t cg_cineDrawLetterBox;

extern vmCvar_t cg_glowModel;
extern vmCvar_t cg_glowModelTeam;
extern vmCvar_t cg_advHitSound;

extern vmCvar_t cg_warmupReady;
extern vmCvar_t cg_curWarmupReady;

extern vmCvar_t cg_sky;
extern vmCvar_t cg_skyLensflare;

extern vmCvar_t cg_timestamps;

extern vmCvar_t cg_drawLensflare;
extern vmCvar_t cg_drawVoiceNames;

extern vmCvar_t cg_drawBBox;

extern vmCvar_t cg_ambient;

extern vmCvar_t cg_chatHeight;
extern vmCvar_t cg_drawChatIcon;

extern vmCvar_t cg_icons;

//
// cg_main.c
//
const char *CG_ConfigString(int index);
const char *CG_Argv(int arg);

void QDECL CG_Printf(const char *msg, ...) __attribute__((format(printf, 1, 2)));
void QDECL CG_Error(const char *msg, ...) __attribute__((noreturn, format(printf, 1, 2)));

void CG_StartMusic(void);

void CG_UpdateCvars(void);

int CG_CrosshairPlayer(void);
int CG_LastAttacker(void);
void CG_LoadMenus(const char *menuFile);
void CG_KeyEvent(int key, qboolean down);
void CG_MouseEvent(int x, int y);
void CG_EventHandling(int type);
void CG_RankRunFrame(void);
void CG_SetScoreSelection(void *menu);
score_t *CG_GetSelectedScore(void);
void CG_BuildSpectatorString(void);
int CG_GetCvarInt(const char *cvar);
float CG_GetCvarFloat(const char *cvar);
void CG_ForceModelChange(void);

#define IsSyc() ((cgs.gametype == GT_SPRAY) || (cgs.gametype == GT_SPRAYFFA))

//
// cg_view.c
//
void CG_TestModel_f(void);
void CG_TestGun_f(void);
void CG_TestModelNextFrame_f(void);
void CG_TestModelPrevFrame_f(void);
void CG_TestModelNextSkin_f(void);
void CG_TestModelPrevSkin_f(void);
void CG_ZoomDown_f(void);
void CG_ZoomUp_f(void);
void CG_AddBufferedSound(sfxHandle_t sfx);

void CG_DrawActiveFrame(int serverTime, stereoFrame_t stereoView, qboolean demoPlayback);

//
// cg_drawtools.c
//
// ugly workaround for having it in cg_local.h and ui_shared.h
#ifndef HAVE_SCREEN_PLACEMENT
#define HAVE_SCREEN_PLACEMENT
typedef enum {
	PLACE_CENTER,

	// horizontal only
	PLACE_LEFT,
	PLACE_RIGHT,

	// vertical only
	PLACE_TOP,
	PLACE_BOTTOM
} screenPlacement_e;
#endif

void CG_SetScreenPlacement(screenPlacement_e hpos, screenPlacement_e vpos);
void CG_PopScreenPlacement(void);
screenPlacement_e CG_GetScreenHorizontalPlacement(void);
screenPlacement_e CG_GetScreenVerticalPlacement(void);
void CG_AdjustFrom640(float *x, float *y, float *w, float *h);
void CG_NativeResTo640(float *x, float *y, float *w, float *h);
void CG_FillRect(float x, float y, float width, float height, const float *color);
void CG_DrawPic(float x, float y, float width, float height, qhandle_t hShader);

void CG_DrawStringExt(int x, int y, const char *string, const float *setColor, qboolean forceColor, qboolean shadow,
					  fontSize_t fontsize, int maxChars);
void CG_DrawBigString(int x, int y, const char *s, float alpha);
void CG_DrawBigStringColor(int x, int y, const char *s, const vec4_t color);
void CG_DrawSmallString(int x, int y, const char *s, float alpha);
void CG_DrawSmallStringColor(int x, int y, const char *s, const vec4_t color);

int CG_DrawStrlen(const char *str);

float *CG_FadeColor(int startMsec, int totalMsec);
void CG_TileClear(void);
void CG_ColorForHealth(vec4_t hcolor);
void CG_GetColorForHealth(int health, int armor, vec4_t hcolor);

void UI_DrawProportionalString(int x, int y, const char *str, int style, const vec4_t color);
void CG_DrawRect(float x, float y, float width, float height, float size, const float *color);
void CG_DrawSides(float x, float y, float w, float h, float size);
void CG_DrawTopBottom(float x, float y, float w, float h, float size);

qboolean CG_WorldToScreen(const vec3_t point, float *x, float *y);

//
// cg_draw.c
//
extern int sortedTeamPlayers[TEAM_MAXOVERLAY];
extern int numSortedTeamPlayers;

void CG_MessagePrint(const char *str);
void CG_AddLagometerFrameInfo(void);
void CG_AddLagometerSnapshotInfo(snapshot_t *snap);
void CG_CenterPrint(const char *str, int y);
void CG_DrawHead(float x, float y, float w, float h, int clientNum, vec3_t headAngles);
void CG_DrawActive(stereoFrame_t stereoView);
void CG_DrawFlagModel(float x, float y, float w, float h, int team);
void CG_DrawCartModel(float x, float y, float w, float h, int team);
void CG_DrawDuckModel(float x, float y, float w, float h);
void CG_DrawTeamBackground(int x, int y, int w, int h, float alpha, int team);
void CG_SetPrintString(int type, const char *p);

//
// cg_player.c
//
void CG_Player(centity_t *cent);
void CG_ResetPlayerEntity(centity_t *cent);
void CG_AddRefEntityWithPowerups(refEntity_t *ent, entityState_t *state, int team);
void CG_NewClientInfo(int clientNum);
sfxHandle_t CG_CustomSound(int clientNum, const char *soundName);
void CG_UpdateVoipTeamIDs(void);

//
// cg_predict.c
//
void CG_BuildSolidList(void);
int CG_PointContents(const vec3_t point, int passEntityNum);
void CG_Trace(trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end,
			  int skipNumber, int mask);
void CG_PredictPlayerState(void);
void CG_LoadDeferredPlayers(void);

//
// cg_events.c
//
void CG_CheckEvents(centity_t *cent);
const char *CG_PlaceString(int rank);
void CG_EntityEvent(centity_t *cent, vec3_t position);
void CG_PainEvent(centity_t *cent, int health);

//
// cg_ents.c
//
void CG_SetEntitySoundPosition(centity_t *cent);
void CG_AddPacketEntities(void);
void CG_Beam(centity_t *cent);
void CG_AdjustPositionForMover(const vec3_t in, int moverNum, int fromTime, int toTime, vec3_t out, vec3_t angles_in, vec3_t angles_out);

void CG_PositionEntityOnTag(refEntity_t *entity, const refEntity_t *parent, qhandle_t parentModel, const char *tagName);
void CG_PositionRotatedEntityOnTag(refEntity_t *entity, const refEntity_t *parent, qhandle_t parentModel,
								   char *tagName);

void CG_BamBam_Explosion(vec3_t origin);
void CG_Boomies_Explosion(vec3_t origin);

//
// cg_weapons.c
//
void CG_ChangeWeapon(int weapon);
void CG_NextWeapon_f(void);
void CG_PrevWeapon_f(void);
void CG_Weapon_f(void);
void CG_WeaponForceSwitch(int weapNum, qboolean checkInventory);

void CG_RegisterItemVisuals(int itemNum);

void CG_FireWeapon(centity_t *cent);
void CG_MissileHitWall(int weapon, int clientNum, vec3_t origin, vec3_t dir, int colorCode,
					   impactSound_t soundType); // HERBY
void CG_MissileHitPlayer(int weapon, vec3_t origin, vec3_t dir, int colorCode, int entityNum);
void CG_ShotgunFire(entityState_t *es);

void CG_GetWaterMuzzle(localEntity_t *le, centity_t *cent, vec3_t fw); // HERBY
void CG_RailTrail(clientInfo_t *ci, vec3_t start, vec3_t end);
void CG_GrappleTrail(centity_t *ent, const weaponInfo_t *wi);
void CG_AddViewWeapon(const playerState_t *ps);
void CG_AddPlayerWeapon(refEntity_t *parent, const playerState_t *ps, centity_t *cent, int team);
void CG_DrawWeaponSelect(void);

void CG_OutOfAmmoChange(void); // should this be in pmove?

//
// cg_marks.c
//
void CG_InitMarkPolys(void);
void CG_AddMarks(void);
void CG_ImpactMark(qhandle_t markShader, const vec3_t origin, const vec3_t dir, float orientation, const vec4_t color, qboolean alphaFade, float radius, qboolean temporary);

//
// cg_localents.c
//
void CG_InitLocalEntities(void);
localEntity_t *CG_AllocLocalEntity(void);
void CG_AddLocalEntities(void);

//
// cg_effects.c
//
localEntity_t *CG_SmokePuff(const vec3_t p, const vec3_t vel, float radius, float r, float g, float b, float a,
							float duration, int startTime, int fadeInTime, int leFlags, qhandle_t hShader);
void CG_GenerateParticles(qhandle_t model, qhandle_t shader, const vec3_t pos, float randomPos, const vec3_t speed, float randomDir,
						  float randomSpeed, int numParticles, int owner, int time, int life, int randomLife, int size,
						  int randomSize, int addSize, int randomAddSize, int flags, int renderfx);
void CG_BubbleTrail(vec3_t start, vec3_t end, float spacing);
void CG_SpawnEffect(vec3_t org, int team);
void CG_TeleOutEffect(vec3_t org, int team, centity_t *cent);
void CG_ScorePlum(int client, vec3_t org, int score);

localEntity_t *CG_MakeExplosion(vec3_t origin, vec3_t dir, qhandle_t hModel, qhandle_t shader, int msec,
								qboolean isSprite);

//
// cg_snapshot.c
//
void CG_ProcessSnapshots(void);

//
// cg_info.c
//
void CG_LoadingString(const char *s);
void CG_ChangeLoadingProgress(float f);
void CG_LoadingItem(int itemNum);
void CG_LoadingClient(int clientNum);
void CG_DrawInformation(void);

//
// cg_scoreboard.c
//
qboolean CG_DrawOldScoreboard(void);
void CG_DrawOldTourneyScoreboard(void);

//
// cg_consolecmds.c
//
qboolean CG_ConsoleCommand(void);
void CG_InitConsoleCommands(void);

//
// cg_servercmds.c
//
void CG_ExecuteNewServerCommands(int latestSequence);
void CG_ParseServerinfo(void);
void CG_SetConfigValues(void);
void CG_ShaderStateChanged(void);

//
// cg_playerstate.c
//
void CG_Respawn(void);
void CG_TransitionPlayerState(const playerState_t *ps, playerState_t *ops);
void CG_ToggleSpawnProtection(int entNum, qboolean state);

//
// cg_spraylogo.c
//
void Init_SprayLogoSys(void);
void AddLogosToScene(void);
void Add_LogoToDrawList(const vec3_t origin, const vec3_t dir, qhandle_t shader, float radius, const vec4_t color);
qhandle_t FindLogoForSpraying(const clientInfo_t *ci);
void ActiveChooseLogoMenu(void);
void CG_DumpPolyInfo(void);

//
// cg_lensflare.c
//
void CG_InitLensflareSystem(void);
void CG_AddLFsToScreen(void);

//
// cg_spriteparticles.c
//
void Init_SpriteParticles(void);
void LaunchSpeedyPuffTrail(vec3_t origin);
void LaunchRevivalParticle(vec3_t origin, const int lifetime);
void Main_SpriteParticles(void);

//
// cg_advanced2d.c
//
#define TURNORIGIN_UPPERLEFT 1
#define TURNORIGIN_MIDDLELEFT 2
#define TURNORIGIN_MIDDLECENTER 3

void CG_DrawPicAngle(float x, float y, float w, float h, qhandle_t hShader, vec4_t color, float angleDegree);

void CG_DrawLine(float x1, float y1, float x2, float y2, float size, vec4_t color);

void CG_DrawPic2Color(float x, float y, float w, float h, float s1, float t1, float s2, float t2, vec4_t color1,
				   vec4_t color2, qhandle_t shader);

void CG_Draw4VertsPic(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, qhandle_t shader,
				   vec4_t color);

void CG_DrawTurnableString(float x, float y, const char *s, vec4_t color, float charHeight, float angle, int turnorigin);

void CG_DrawStringWithCutFrame(float x, float y, const char *str, vec4_t color, fontSize_t fontsize, float fl, float ft,
							float fr, float fb);

//
// cg_freezetag.c
//
qboolean CG_FreezeTag(void);
qboolean FT_LocalIsFrozen(void);
qboolean FT_PlayerIsFrozen(const centity_t *cent);

//
// cg_cutscene2d.c
//
qboolean CG_Cutscene2d_CheckCmd(const char *cmd);
void CG_Cutscene2d_UpdateTrans(void);
void CG_Cutscene2d_Draw(void);

//===============================================

//
// system traps
// These functions are how the cgame communicates with the main game system
//

// print message on the local console
void trap_Print(const char *fmt);

// abort the game
void trap_Error(const char *fmt) Q_NORETURN;

// milliseconds should only be used for performance tuning, never
// for anything game related.  Get time from the CG_DrawActiveFrame parameter
int trap_Milliseconds(void);

// console variable interaction
void trap_Cvar_Register(vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags);
void trap_Cvar_Update(vmCvar_t *vmCvar);
void trap_Cvar_Set(const char *var_name, const char *value);
void trap_Cvar_VariableStringBuffer(const char *var_name, char *buffer, int bufsize);

// ServerCommand and ConsoleCommand parameter access
int trap_Argc(void);
void trap_Argv(int n, char *buffer, int bufferLength);
void trap_Args(char *buffer, int bufferLength);

// filesystem access
// returns length of file
int trap_FS_FOpenFile(const char *qpath, fileHandle_t *f, fsMode_t mode);
void trap_FS_Read(void *buffer, int len, fileHandle_t f);
void trap_FS_Write(const void *buffer, int len, fileHandle_t f);
void trap_FS_FCloseFile(fileHandle_t f);
int trap_FS_Seek(fileHandle_t f, long offset, int origin); // fsOrigin_t

// add commands to the local console as if they were typed in
// for map changing, etc.  The command is not executed immediately,
// but will be executed in order the next time console commands
// are processed
void trap_SendConsoleCommand(const char *text);

// register a command name so the console can perform command completion.
// FIXME: replace this with a normal console command "defineCommand"?
void trap_AddCommand(const char *cmdName);
void trap_RemoveCommand(const char *cmdName);

// send a string to the server over the network
void trap_SendClientCommand(const char *s);

// force a screen update, only used during gamestate load
void trap_UpdateScreen(void);

// model collision
void trap_CM_LoadMap(const char *mapname);
int trap_CM_NumInlineModels(void);
clipHandle_t trap_CM_InlineModel(int index); // 0 = world, 1+ = bmodels
clipHandle_t trap_CM_TempBoxModel(const vec3_t mins, const vec3_t maxs);
clipHandle_t trap_CM_TempCapsuleModel(const vec3_t mins, const vec3_t maxs);
int trap_CM_PointContents(const vec3_t p, clipHandle_t model);
int trap_CM_TransformedPointContents(const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles);
void trap_CM_BoxTrace(trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs,
					  clipHandle_t model, int brushmask);
void trap_CM_CapsuleTrace(trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs,
					clipHandle_t model, int brushmask);
void trap_CM_TransformedBoxTrace(trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins,
								 const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin,
								 const vec3_t angles);

// Returns the projection of a polygon onto the solid brushes in the world
int trap_CM_MarkFragments(int numPoints, const vec3_t *points, const vec3_t projection, int maxPoints,
						  vec3_t pointBuffer, int maxFragments, markFragment_t *fragmentBuffer);

// normal sounds will have their volume dynamically changed as their entity
// moves and the listener moves
void trap_S_StartSound(vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx);
void trap_S_StopLoopingSound(int entnum);

// a local sound is always played full volume
void trap_S_StartLocalSound(sfxHandle_t sfx, int channelNum);
void trap_S_ClearLoopingSounds(qboolean killall);
void trap_S_AddLoopingSound(int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx);
void trap_S_AddRealLoopingSound(int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx);
void trap_S_UpdateEntityPosition(int entityNum, const vec3_t origin);

// respatialize recalculates the volumes of sound as they should be heard by the
// given entityNum and position
void trap_S_Respatialize(int entityNum, const vec3_t origin, vec3_t axis[3], int inwater);
sfxHandle_t trap_S_RegisterSound(const char *sample, qboolean compressed); // returns buzz if not found
void trap_S_StartBackgroundTrack(const char *intro, const char *loop);	   // empty name stops music
void trap_S_StopBackgroundTrack(void);

void trap_R_LoadWorldMap(const char *mapname);

// all media should be registered during level startup to prevent
// hitches during gameplay
qhandle_t trap_R_RegisterModel(const char *name);		// returns rgb axis if not found
qhandle_t trap_R_RegisterSkin(const char *name);		// returns all white if not found
qhandle_t trap_R_RegisterShader(const char *name);		// returns all white if not found
qhandle_t trap_R_RegisterShaderNoMip(const char *name); // returns all white if not found

// a scene is built up by calls to R_ClearScene and the various R_Add functions.
// Nothing is drawn until R_RenderScene is called.
void trap_R_ClearScene(void);
void trap_R_AddRefEntityToScene(const refEntity_t *re);

// polys are intended for simple wall marks, not really for doing
// significant construction
void trap_R_AddPolyToScene(qhandle_t hShader, int numVerts, const polyVert_t *verts);
void trap_R_AddPolysToScene(qhandle_t hShader, int numVerts, const polyVert_t *verts, int numPolys);
void trap_R_AddLightToScene(const vec3_t org, float intensity, float r, float g, float b);
int trap_R_LightForPoint(vec3_t point, vec3_t ambientLight, vec3_t directedLight, vec3_t lightDir);
void trap_R_RenderScene(const refdef_t *fd);
void trap_R_SetColor(const float *rgba); // NULL = 1,1,1,1
void trap_R_DrawStretchPic(float x, float y, float w, float h, float s1, float t1, float s2, float t2,
						   qhandle_t hShader);
void trap_R_ModelBounds(clipHandle_t model, vec3_t mins, vec3_t maxs);
int trap_R_LerpTag(orientation_t *tag, clipHandle_t mod, int startFrame, int endFrame, float frac, const char *tagName);
void trap_R_RemapShader(const char *oldShader, const char *newShader, const char *timeOffset);

// The glconfig_t will not change during the life of a cgame.
// If it needs to change, the entire cgame will be restarted, because
// all the qhandle_t are then invalid.
void trap_GetGlconfig(glconfig_t *glconfig);

// the gamestate should be grabbed at startup, and whenever a
// configstring changes
void trap_GetGameState(gameState_t *gamestate);

// cgame will poll each frame to see if a newer snapshot has arrived
// that it is interested in.  The time is returned separately so that
// snapshot latency can be calculated.
void trap_GetCurrentSnapshotNumber(int *snapshotNumber, int *serverTime);

// a snapshot get can fail if the snapshot (or the entties it holds) is so
// old that it has fallen out of the client system queue
qboolean trap_GetSnapshot(int snapshotNumber, snapshot_t *snapshot);

// retrieve a text command from the server stream
// the current snapshot will hold the number of the most recent command
// qfalse can be returned if the client system handled the command
// argc() / argv() can be used to examine the parameters of the command
qboolean trap_GetServerCommand(int serverCommandNumber);

// returns the most recent command number that can be passed to GetUserCmd
// this will always be at least one higher than the number in the current
// snapshot, and it may be quite a few higher if it is a fast computer on
// a lagged connection
int trap_GetCurrentCmdNumber(void);

qboolean trap_GetUserCmd(int cmdNumber, usercmd_t *ucmd);

// used for the weapon select and zoom
void trap_SetUserCmdValue(int stateValue, float sensitivityScale);

// aids for VM testing
void testPrintInt(char *string, int i);
void testPrintFloat(char *string, float f);

int trap_MemoryRemaining(void);
void trap_R_RegisterFont(const char *fontName, int pointSize, fontInfo_t *font);
qboolean trap_Key_IsDown(int keynum);
int trap_Key_GetCatcher(void);
void trap_Key_SetCatcher(int catcher);
int trap_Key_GetKey(const char *binding);

typedef enum {
	SYSTEM_PRINT,
	CHAT_PRINT,
	TEAMCHAT_PRINT
} q3print_t; // bk001201 - warning: useless keyword or type name in empty declaration

int trap_CIN_PlayCinematic(const char *arg0, int xpos, int ypos, int width, int height, int bits);
e_status trap_CIN_StopCinematic(int handle);
e_status trap_CIN_RunCinematic(int handle);
void trap_CIN_DrawCinematic(int handle);
void trap_CIN_SetExtents(int handle, int x, int y, int w, int h);

void trap_SnapVector(float *v);

qboolean trap_loadCamera(const char *name);
void trap_startCamera(int time);
qboolean trap_getCameraInfo(int time, vec3_t *origin, vec3_t *angles);

qboolean trap_GetEntityToken(char *buffer, int bufferSize);
void trap_GetVoipTimes(int *times);

int trap_RealTime(qtime_t *qtime);

//
// cg_catch.c
//
qboolean CG_IsKillerDuck(const centity_t *cent);
void CG_DrawKillerduckIcon(const centity_t *cent);

//
// cg_misc.c
//
void CG_LimitCvars(void);
char *CG_Timestamp(char *timestamp, unsigned int size);
void CG_QueryCvar(void);
void CG_AddBoundingBoxEntity(const centity_t *cent);
void CG_AddBoundingBox(const centity_t *cent);
