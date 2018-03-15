// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "ui_local.h"

#define ART_FRAMEL			"menu/art/frame2_l"
#define ART_FRAMER			"menu/art/frame1_r"
#define ART_MODEL0			"menu/art/model_0"
#define ART_MODEL1			"menu/art/model_1"
#define ART_BACK0			"menu/art/back_0"
#define ART_BACK1			"menu/art/back_1"
#define ART_FX_BASE			"menu/art/fx_base"
#define ART_FX_BLUE			"menu/art/fx_blue"
#define ART_FX_CYAN			"menu/art/fx_cyan"
#define ART_FX_GREEN		"menu/art/fx_grn"
#define ART_FX_RED			"menu/art/fx_red"
#define ART_FX_TEAL			"menu/art/fx_teal"
#define ART_FX_WHITE		"menu/art/fx_white"
#define ART_FX_YELLOW		"menu/art/fx_yel"

#define BACK0		"menu/BtnBack0"
#define BACK1		"menu/BtnBack1"

#define ID_NAME			10
#define ID_HANDICAP		11
#define ID_EFFECTS		12
#define ID_BACK			13
//#define ID_MODEL		14

#define	ID_MODELSLEFT	15
#define ID_MODELSRIGHT	16
#define ID_SKINSUP		17
#define ID_SKINSDOWN	18
#define ID_MICON		19 //+4
#define ID_SICON		24 //+2
#define ID_PLAYERMODEL	27
#define ID_SPRAYCOLOR	28
#define ID_SLOGONEXT	29
#define ID_SLOGOPREV	30

#define MAX_NAMELENGTH	20

#define DISPLAYED_MODELS	4

typedef struct {
	menuframework_s		menu;

	menutext_s			banner;
	menubitmap_s		framel;
	menubitmap_s		framer;
	menubitmap_s		player;

	menufield_s			name;
	menulist_s			handicap;
	menulist_s			effects;

	menubitmap_s		back;
	menubitmap_s		model;

	menubitmap1024s_s	modelsleft;
	menubitmap1024s_s	modelsright;

	menubitmap1024s_s	skinup;
	menubitmap1024s_s	skindown;

	menubitmap1024s_s	model_icons[DISPLAYED_MODELS];
	menubitmap1024s_s	skin_icons[3];

	menubitmap1024s_s	spraylogos_next;
	menubitmap1024s_s	spraylogos_prev;

	menubitmap_s		spraycolor;

	int					firstmodel;
	int					firstskin;
	int					chosenskins[2];
	qboolean			modelhold;
	int					lastCursorX;
	int					slogo_num;
	int					nextGestureTime;

	menubitmap_s		item_null;

	qhandle_t			fxBasePic;
	qhandle_t			fxPic[7];
	playerInfo_t		playerinfo;
	int					current_fx;
	char				playerModel[MAX_QPATH];
} playersettings_t;

static playersettings_t	s_playersettings;

static int gamecodetoui[] = {4,2,3,0,5,1,6};
static int uitogamecode[] = {4,6,2,3,1,5,7};

#if 0
static const char *handicap_items[] = {
	"None",
	"95",
	"90",
	"85",
	"80",
	"75",
	"70",
	"65",
	"60",
	"55",
	"50",
	"45",
	"40",
	"35",
	"30",
	"25",
	"20",
	"15",
	"10",
	"5",
	0
};
#endif

#define MAX_UIMODELS	96 //32
#define MAX_SKINS	640 // padman has 18 skins ...
#define MAX_MODELSKINNAME_STR	64
typedef struct {
	char name[MAX_MODELSKINNAME_STR];
	qhandle_t icon;
} SkinData_t;
typedef struct
{
	qhandle_t	modelicons[MAX_UIMODELS];
	qhandle_t	modeliconsB[MAX_UIMODELS];
	int			nummodel;
	int			lastskinicon[MAX_UIMODELS];
	SkinData_t	modelskins[MAX_SKINS];
} ps_playericons_t;

ps_playericons_t ps_playericons;


/*
=================
PlayerSettings_DrawName
=================
*/
static void PlayerSettings_DrawName( void *self ) {
	menufield_s		*f;
	qboolean		focus;
	int				style;
	char			*txt;
	char			c;
	float			*color;
	int				n;
	int				basex, x, y;

	f = (menufield_s*)self;
	basex = f->generic.x;
	y = f->generic.y;
	focus = (f->generic.parent->cursor == f->generic.menuPosition);

	style = UI_LEFT|UI_SMALLFONT;
	color = text_color_normal;
	if( focus ) {
		style |= UI_PULSE;
		color = text_color_highlight;
	}

	basex += 10;
	y += PROP_HEIGHT;
	txt = f->field.buffer;
	color = g_color_table[ColorIndex(COLOR_WHITE)];
	x = basex;
	while ( (c = *txt) != 0 ) {
		if ( !focus && Q_IsColorString( txt ) ) {
			n = ColorIndex( *(txt+1) );
			if( n == 0 ) {
				n = 7;
			}
			color = g_color_table[n];
			txt += 2;
			continue;
		}
		UI_DrawChar( x, y, c, style, color );
		txt++;
		x += SMALLCHAR_WIDTH;
	}

	// draw cursor if we have focus
	if( focus ) {
		if ( trap_Key_GetOverstrikeMode() ) {
			c = 11;
		} else {
			c = 10;
		}

		style &= ~UI_PULSE;
		style |= UI_BLINK;

		UI_DrawChar( basex + f->field.cursor * SMALLCHAR_WIDTH, y, c, style, color_white );
	}

}

#if 0
/*
=================
PlayerSettings_DrawHandicap
=================
*/
static void PlayerSettings_DrawHandicap( void *self ) {
	menulist_s		*item;
	qboolean		focus;
	int				style;
	float			*color;

	item = (menulist_s *)self;
	focus = (item->generic.parent->cursor == item->generic.menuPosition);

	style = UI_LEFT|UI_SMALLFONT;
	color = text_color_normal;
	if( focus ) {
		style |= UI_PULSE;
		color = text_color_highlight;
	}

	UI_DrawProportionalString( item->generic.x, item->generic.y, "Handicap", style, color );
	UI_DrawProportionalString( item->generic.x + 64, item->generic.y + PROP_HEIGHT, handicap_items[item->curvalue], style, color );
}


/*
=================
PlayerSettings_DrawEffects
=================
*/
static void PlayerSettings_DrawEffects( void *self ) {
	menulist_s		*item;
	qboolean		focus;
	int				style;
	float			*color;

	item = (menulist_s *)self;
	focus = (item->generic.parent->cursor == item->generic.menuPosition);

	style = UI_LEFT|UI_SMALLFONT;
	color = text_color_normal;
	if( focus ) {
		style |= UI_PULSE;
		color = text_color_highlight;
	}

	UI_DrawProportionalString( item->generic.x, item->generic.y, "Effects", style, color );

	UI_DrawHandlePic( item->generic.x + 64, item->generic.y + PROP_HEIGHT + 8, 128, 8, s_playersettings.fxBasePic );
	UI_DrawHandlePic( item->generic.x + 64 + item->curvalue * 16 + 8, item->generic.y + PROP_HEIGHT + 6, 16, 12, s_playersettings.fxPic[item->curvalue] );
}
#endif

/*
=================
PlayerSettings_DrawPlayer
=================
*/
static void PlayerSettings_DrawPlayer( void *self ) {
	menubitmap_s	*b;
	vec3_t			viewangles;
	char			buf[MAX_QPATH];

	trap_Cvar_VariableStringBuffer( "model", buf, sizeof( buf ) );
	if ( strcmp( buf, s_playersettings.playerModel ) != 0 ) {
		UI_PlayerInfo_SetModel( &s_playersettings.playerinfo, buf );
		strcpy( s_playersettings.playerModel, buf );

		viewangles[YAW]   = 180+10;
		viewangles[PITCH] = 0;
		viewangles[ROLL]  = 0;
		UI_PlayerInfo_SetInfo( &s_playersettings.playerinfo, LEGS_IDLE, TORSO_STAND, viewangles, vec3_origin, WP_NIPPER, qfalse );
		s_playersettings.nextGestureTime = uis.realtime+2000;
	}

	b = (menubitmap_s*) self;

	//YEW=1=right/left
//	s_playersettings.playerinfo.viewAngles[YAW] =
//	s_playersettings.playerinfo.legs.yawAngle = 
//	s_playersettings.playerinfo.torso.yawAngle = uis.realtime;

	UI_DrawPlayer( b->generic.x, b->generic.y, b->width, b->height, &s_playersettings.playerinfo, uis.realtime/2 );

//	UI_DrawRect(b->generic.x, b->generic.y, b->width, b->height, colorGreen);
}


/*
=================
PlayerSettings_SaveChanges
=================
*/
static void PlayerSettings_SaveChanges( void ) {
	// name
	trap_Cvar_Set( "name", s_playersettings.name.field.buffer );

	// handicap
	// NOTE: We do not use this currently. Some players reported they always get handicap 5 when using the player setup menu
	//trap_Cvar_SetValue( "handicap", 100 - s_playersettings.handicap.curvalue * 5 );

	// effects color
	trap_Cvar_SetValue( "color1", uitogamecode[s_playersettings.effects.curvalue] );

	trap_Cvar_Set( "syc_logo", uis.spraylogoNames[s_playersettings.slogo_num] );
	trap_Cmd_ExecuteText( EXEC_APPEND, va( "selectlogo \"%s\"\n", uis.spraylogoNames[s_playersettings.slogo_num] ) );
}


/*
=================
PlayerSettings_MenuKey
=================
*/
static sfxHandle_t PlayerSettings_MenuKey( int key ) {
	int newweapon;

	if( key == K_MOUSE2 || key == K_ESCAPE ) {
		PlayerSettings_SaveChanges();
	}

	if(key == K_MWHEELDOWN || key == K_PGDN)
	{
		if(s_playersettings.playerinfo.weapon>=WP_IMPERIUS)
			newweapon=WP_PUNCHY;
		else
			newweapon=s_playersettings.playerinfo.weapon+1;

		UI_PlayerInfo_SetInfo( &s_playersettings.playerinfo, LEGS_IDLE, (newweapon==WP_PUNCHY?TORSO_STAND2:TORSO_STAND), s_playersettings.playerinfo.viewAngles, vec3_origin, newweapon, qfalse );
	}
	else if(key == K_MWHEELUP || key == K_PGUP)
	{
		if(s_playersettings.playerinfo.weapon<=WP_PUNCHY)
			newweapon=WP_IMPERIUS;
		else
			newweapon=s_playersettings.playerinfo.weapon-1;

		UI_PlayerInfo_SetInfo( &s_playersettings.playerinfo, LEGS_IDLE, (newweapon==WP_PUNCHY?TORSO_STAND2:TORSO_STAND), s_playersettings.playerinfo.viewAngles, vec3_origin, newweapon, qfalse );
	}

	return Menu_DefaultKey( &s_playersettings.menu, key );
}


/*
=================
PlayerSettings_SetMenuItems
=================
*/
static void PlayerSettings_SetMenuItems( void ) {
	vec3_t	viewangles;
	int		c;
	int		h;

	// name
	Q_strncpyz( s_playersettings.name.field.buffer, UI_Cvar_VariableString("name"), sizeof(s_playersettings.name.field.buffer) );

	// effects color
	c = trap_Cvar_VariableValue( "color1" ) - 1;
	if( c < 0 || c > 6 ) {
		c = 6;
	}
	s_playersettings.effects.curvalue = gamecodetoui[c];

	// model/skin
	memset( &s_playersettings.playerinfo, 0, sizeof(playerInfo_t) );
	
	viewangles[YAW]   = 180 - 30;
	viewangles[PITCH] = 0;
	viewangles[ROLL]  = 0;

	UI_PlayerInfo_SetModel( &s_playersettings.playerinfo, UI_Cvar_VariableString( "model" ) );
	UI_PlayerInfo_SetInfo( &s_playersettings.playerinfo, LEGS_IDLE, TORSO_STAND, viewangles, vec3_origin, WP_NIPPER, qfalse );

	{
		int i;
		char modelname[32];
		int mnLen;
		Q_strncpyz(modelname,UI_Cvar_VariableString("model"),sizeof(modelname));

		for(i=0;i<sizeof(modelname);i++) if(modelname[i]=='/') {modelname[i]='\0'; break;}

		mnLen = strlen(modelname);

		for(i=0;i<ps_playericons.nummodel;++i) {
			const char* modelskinname = ps_playericons.modelskins[ps_playericons.lastskinicon[i]].name;
			if(!Q_stricmpn(modelname,modelskinname,mnLen)) {
				s_playersettings.firstskin = i==0?0:(ps_playericons.lastskinicon[i-1]+1);

				s_playersettings.chosenskins[0]=s_playersettings.firstskin;
				s_playersettings.chosenskins[1]=ps_playericons.lastskinicon[i];

				s_playersettings.firstmodel = (i+DISPLAYED_MODELS<ps_playericons.nummodel)?i:ps_playericons.nummodel-DISPLAYED_MODELS;
				break;
			}
		}
	}

	// handicap
	h = Com_Clamp( 5, 100, trap_Cvar_VariableValue("handicap") );
	s_playersettings.handicap.curvalue = 20 - h / 5;
}

static int GetSpecialSkinScore(const char* iconPath) {
	if(NULL != Q_stristr(iconPath,"/default"))
		return 3;
	else if(NULL != Q_stristr(iconPath,"/blue"))
		return 2;
	else if(NULL != Q_stristr(iconPath,"/red"))
		return 1;
	else
		return 0;
}
static int SkinComp(const void *vA, const void *vB) {
	const SkinData_t* a = (const SkinData_t*)vA;
	const SkinData_t* b = (const SkinData_t*)vB;

	const int a_special = GetSpecialSkinScore(a->name);
	const int b_special = GetSpecialSkinScore(b->name);

	if(a_special || b_special) {
		/// "higher score" should be "less" in compare (so that they get to the top by qsort) ... that's why it's "b-a" and not "a-b"
		return b_special - a_special;
	}
	else {
		return Q_stricmp(a->name,b->name);
	}
}

static void sortSkins(int first, int last) {
	qsort(&(ps_playericons.modelskins[first]),
		  last - first +1,
		  sizeof( SkinData_t ),
		  SkinComp);
}

#define MAX_MODEL_DIR_LIST 2048
#define MAX_MODELFOLDER_FILELIST 2048

static const char fixedModelList[] = "padman\0padgirl\0monster\0padlilly\0fatpad";
static const int numFixedModels = 5;

qboolean IsEntryInList(const char* modelName,const char* list, int numListEntries) {
	int i=0;
	const char* entryPtr = list;

	while(i<numListEntries) {
		const int entryLen = strlen(entryPtr);

		if(0==Q_stricmp(modelName,entryPtr)) {
			return qtrue;
		}

		entryPtr += entryLen +1;
		++i;
	}

	return qfalse;
}

qboolean IsAFixedModelListEntry(const char* modelName) {
	return IsEntryInList(modelName,fixedModelList,numFixedModels);
}

void TweakModelFolderList(char* list, int* numEntries) {
	char newList[MAX_MODEL_DIR_LIST];
	int newNumEntries =0;
	char* entryPtr =NULL;
	char* newEntryPtr =NULL;
	int i =0;

	memcpy(newList,fixedModelList,sizeof(fixedModelList));
	newNumEntries = numFixedModels;

	entryPtr = list;
	newEntryPtr = newList + sizeof(fixedModelList);
	while(i<(*numEntries)) {
		const int entryLen = strlen(entryPtr);

		///HACKAROUND(SL): pk3-filesystem gives directory-names with '/' suffix (os-filesystem doesn't)
		if(entryPtr[entryLen-1]=='/')
			entryPtr[entryLen-1] = '\0';

		///NOTE(SL): just recognized that there are duplicates if the same folder is in *.pk3 and os-filesystem -.-
		if(!IsAFixedModelListEntry(entryPtr) && !IsEntryInList(entryPtr,newList,newNumEntries)) {
			const int destSize = MAX_MODEL_DIR_LIST-(newEntryPtr-newList);
			if(destSize < (entryLen+1) ) {
				Com_Printf("ERROR: hit limit for modellist buffer ...");
				return;
			}

			Q_strncpyz(newEntryPtr,entryPtr,destSize);
			newEntryPtr += strlen(newEntryPtr) +1;
			++newNumEntries;
		}

		entryPtr += entryLen +1;
		++i;
	}

	memcpy(list,newList,MAX_MODEL_DIR_LIST);
	*numEntries = newNumEntries;
}

/*
#######################
PlayerSettings_BuildList
#######################
*/
static void PlayerSettings_BuildList( void )
{
	int		numdirs =0;
	int		numfiles =0;
	char	dirlist[MAX_MODEL_DIR_LIST];
	char	filelist[MAX_MODELFOLDER_FILELIST];
	char	skinname[64];
	char*	dirptr =NULL;
	char*	fileptr =NULL;
	int		i =0;
	int		j =0;
	int		dirlen =0;
	int		filelen =0;
	int		tmpskinnum =0;

	memset(&ps_playericons,0,sizeof(ps_playericons));

	// iterate directory of all player models
	numdirs = trap_FS_GetFileList("models/wop_players", "/", dirlist, MAX_MODEL_DIR_LIST );
	TweakModelFolderList(dirlist,&numdirs);

	dirptr  = dirlist;
	for (i=0; i<numdirs && ps_playericons.nummodel < MAX_UIMODELS; i++,dirptr+=dirlen+1) {

		dirlen = strlen(dirptr);
		
		if (dirlen && dirptr[dirlen-1]=='/') dirptr[dirlen-1]='\0';

		if (!strcmp(dirptr,".") || !strcmp(dirptr,"..") || *dirptr=='\0')
			continue;

		if (!( ps_playericons.modelicons[ps_playericons.nummodel]=trap_R_RegisterShaderNoMip(va("models/wop_players/%s/wop_menu",dirptr)) ))
			continue;
		ps_playericons.modeliconsB[ps_playericons.nummodel]=trap_R_RegisterShaderNoMip(va("models/wop_players/%s/wop_menuB",dirptr));
		
		// iterate all skin files in directory
		numfiles = trap_FS_GetFileList( va("models/wop_players/%s",dirptr), "tga", filelist, MAX_MODELFOLDER_FILELIST );
		fileptr  = filelist;
		for (j=0; j<numfiles && ps_playericons.lastskinicon[ps_playericons.nummodel] < MAX_SKINS;j++,fileptr+=filelen+1)
		{
			filelen = strlen(fileptr);

			COM_StripExtension(fileptr,skinname, sizeof(skinname) );

			// look for icon_????
			if (!Q_stricmpn(skinname,"icon_",5))
			{
				ps_playericons.modelskins[tmpskinnum].icon=trap_R_RegisterShaderNoMip(va("models/wop_players/%s/%s",dirptr,skinname));
				Com_sprintf(ps_playericons.modelskins[tmpskinnum].name,MAX_MODELSKINNAME_STR,"%s/%s",dirptr,(char*)(&skinname[5]));

				tmpskinnum++;
			}
		}
		ps_playericons.lastskinicon[ps_playericons.nummodel]=tmpskinnum-1;
		ps_playericons.nummodel++;
	}

	j=0;
	for(i=0;i<ps_playericons.nummodel;++i) {
		sortSkins(j,ps_playericons.lastskinicon[i]);
		j = ps_playericons.lastskinicon[i]+1;
	}
}

/*
#######################
PlayerSettings_Draw
#######################
*/
static void PlayerSettings_Draw( void )
{
	int i;

static char modelname[32];
	if(s_playersettings.nextGestureTime==0)
		s_playersettings.nextGestureTime=uis.realtime+5000;

	if(s_playersettings.nextGestureTime<uis.realtime)
	{
		UI_PlayerInfo_SetInfo( &s_playersettings.playerinfo, LEGS_IDLE, TORSO_GESTURE, s_playersettings.playerinfo.viewAngles, vec3_origin, s_playersettings.playerinfo.weapon, qfalse );

		s_playersettings.nextGestureTime=uis.realtime+15000;
	}

	if(s_playersettings.modelhold)
	{
		if(trap_Key_IsDown(K_MOUSE1))
		{
			s_playersettings.playerinfo.viewAngles[YAW] =
			s_playersettings.playerinfo.legs.yawAngle = 
			(s_playersettings.playerinfo.torso.yawAngle += uis.cursorx-s_playersettings.lastCursorX);

			s_playersettings.lastCursorX=uis.cursorx;
		}
		else
			s_playersettings.modelhold=qfalse;
	}

	for(i=0;i<DISPLAYED_MODELS;i++)
	{
		s_playersettings.model_icons[i].shader = ps_playericons.modelicons[i+s_playersettings.firstmodel];
		s_playersettings.model_icons[i].mouseovershader = ps_playericons.modeliconsB[i+s_playersettings.firstmodel];
	}

	for(i=0;i<3;i++) {
		const int currentSkinIndex = s_playersettings.firstskin + i;

		if(currentSkinIndex > s_playersettings.chosenskins[1]) {
			s_playersettings.skin_icons[i].generic.flags |= (QMF_HIDDEN|QMF_INACTIVE);
		}
		else {
			s_playersettings.skin_icons[i].shader = ps_playericons.modelskins[currentSkinIndex].icon;
			s_playersettings.skin_icons[i].generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
		}
	}


	i = trap_Cvar_VariableValue( "syc_color" );
	i = Com_Clamp( 0, ( NUM_SPRAYCOLORS - 1 ), i );

	UI_SetColor( spraycolors[i] );
	UI_DrawHandlePic1024( 76, 464, 96, 96, uis.spraylogoShaders[s_playersettings.slogo_num] );
	UI_SetColor( NULL );


	Q_strncpyz(modelname,UI_Cvar_VariableString("model"),sizeof(modelname));

	for(i=0;i<sizeof(modelname);i++) if(modelname[i]=='/') {modelname[i]='\0'; break;}

	if(!Q_stricmp(&modelname[i+1],"default"))
		UI_DrawProportionalString(320,440,modelname,UI_CENTER|UI_SMALLFONT,colorWhite);
	else
		UI_DrawProportionalString(320,440,&modelname[i+1],UI_CENTER|UI_SMALLFONT,colorWhite);

	Menu_Draw( &s_playersettings.menu );
}


/*
=================
PlayerSettings_MenuEvent
=================
*/
static void PlayerSettings_MenuEvent( void* ptr, int event ) {
	int tmpid, i;

	if( event != QM_ACTIVATED ) {
		return;
	}

	tmpid=((menucommon_s*)ptr)->id;
	switch( tmpid ) {

	case ID_HANDICAP:
		//trap_Cvar_Set( "handicap", va( "%i", 100 - 25 * s_playersettings.handicap.curvalue ) );
		break;

/*	case ID_MODEL:
		PlayerSettings_SaveChanges();
		UI_PlayerModelMenu();
		break;
*/
	case ID_BACK:
		PlayerSettings_SaveChanges();
		UI_PopMenu();
		break;

	case ID_MODELSLEFT:
		if(s_playersettings.firstmodel>0)
			s_playersettings.firstmodel--;
		break;
	case ID_MODELSRIGHT:
		if(s_playersettings.firstmodel+DISPLAYED_MODELS<ps_playericons.nummodel)
			s_playersettings.firstmodel++;
		break;
	case ID_SKINSUP:
		if(s_playersettings.firstskin>s_playersettings.chosenskins[0])
			s_playersettings.firstskin--;
		break;
	case ID_SKINSDOWN:
		if(s_playersettings.firstskin+2<s_playersettings.chosenskins[1])
			s_playersettings.firstskin++;
		break;
	case ID_MICON:
	case ID_MICON+1:
	case ID_MICON+2:
	case ID_MICON+3:
	case ID_MICON+4:
		tmpid+=(s_playersettings.firstmodel-ID_MICON);
		if(tmpid>0)
			s_playersettings.chosenskins[0]=s_playersettings.firstskin=ps_playericons.lastskinicon[tmpid-1]+1;
		else
			s_playersettings.chosenskins[0]=s_playersettings.firstskin=0;
		s_playersettings.chosenskins[1]=ps_playericons.lastskinicon[tmpid];

		for(i=s_playersettings.chosenskins[0];i<=s_playersettings.chosenskins[1];i++)
		{
			if(strstr(ps_playericons.modelskins[i].name,"default")!=NULL)
			{
				char tmp[64],*chrptr;

				Q_strncpyz(tmp,ps_playericons.modelskins[i].name,64);
				if((chrptr=strchr(tmp,'/'))!=NULL) *chrptr='\0';
				trap_S_StartLocalSound(trap_S_RegisterSound(va("sounds/names/players/%s", Q_strlwr( tmp ) ),qfalse),CHAN_LOCAL_SOUND);

				trap_Cvar_Set("model",ps_playericons.modelskins[i].name);
				trap_Cvar_Set("headmodel",ps_playericons.modelskins[i].name);
				trap_Cvar_Set( "team_model", ps_playericons.modelskins[i].name );
				trap_Cvar_Set( "team_headmodel", ps_playericons.modelskins[i].name );
				break;
			}
		}
		break;
	case ID_SICON:
	case ID_SICON+1:
	case ID_SICON+2:
		trap_Cvar_Set("model",ps_playericons.modelskins[tmpid-ID_SICON+s_playersettings.firstskin].name);
		trap_Cvar_Set("headmodel",ps_playericons.modelskins[tmpid-ID_SICON+s_playersettings.firstskin].name);
		trap_Cvar_Set( "team_model", ps_playericons.modelskins[tmpid - ID_SICON + s_playersettings.firstskin].name );
		trap_Cvar_Set( "team_headmodel", ps_playericons.modelskins[tmpid - ID_SICON + s_playersettings.firstskin].name );
		break;
	case ID_PLAYERMODEL:
		if(trap_Key_IsDown(K_MOUSE1))
			s_playersettings.modelhold=qtrue;
			s_playersettings.lastCursorX=uis.cursorx;
		break;
	case ID_SPRAYCOLOR:
//		Com_Printf("Spraycolor=%i\n",(uis.cursorx-((menucommon_s*)ptr)->x)/14);
		trap_Cvar_Set( "syc_color", va("%i",(uis.cursorx-((menucommon_s*)ptr)->x)/14) );
		break;

	case ID_SLOGOPREV:
		if( --s_playersettings.slogo_num < 0 )
			s_playersettings.slogo_num = uis.spraylogosLoaded - 1;	// close the circle
		break;

	case ID_SLOGONEXT:
		if ( ++s_playersettings.slogo_num >= uis.spraylogosLoaded )
			s_playersettings.slogo_num = 0;	// close the circle
		break;
	}
}

/*
=================
PlayerSettings_MenuInit
=================
*/
static void PlayerSettings_MenuInit( void ) {
	int i;

	memset(&s_playersettings,0,sizeof(playersettings_t));

	PlayerSettings_Cache();

	s_playersettings.menu.key        = PlayerSettings_MenuKey;
	s_playersettings.menu.wrapAround = qtrue;
	s_playersettings.menu.fullscreen = qtrue;
	s_playersettings.chosenskins[1]	= ps_playericons.lastskinicon[0];
	s_playersettings.menu.draw		= PlayerSettings_Draw;
	s_playersettings.menu.bgparts	= BGP_PLAYERBG;

	s_playersettings.item_null.generic.type		= MTYPE_BITMAP;
	s_playersettings.item_null.generic.flags	= QMF_LEFT_JUSTIFY|QMF_MOUSEONLY|QMF_SILENT;
	s_playersettings.item_null.generic.x		= 0;
	s_playersettings.item_null.generic.y		= 0;
	s_playersettings.item_null.width			= 640;
	s_playersettings.item_null.height			= 480;
	Menu_AddItem( &s_playersettings.menu, &s_playersettings.item_null );

	s_playersettings.player.generic.type		= MTYPE_BITMAP;
	s_playersettings.player.generic.flags		= QMF_MOUSEONLY|QMF_SILENT;
	s_playersettings.player.generic.ownerdraw	= PlayerSettings_DrawPlayer;
	s_playersettings.player.generic.x			= 160;
	s_playersettings.player.generic.y			= 15;
	s_playersettings.player.width				= 320;
	s_playersettings.player.height				= 560;
	s_playersettings.player.generic.callback	= PlayerSettings_MenuEvent;
	s_playersettings.player.generic.id			= ID_PLAYERMODEL;
	Menu_AddItem( &s_playersettings.menu, &s_playersettings.player );

	s_playersettings.modelsleft.generic.type	= MTYPE_BITMAP1024S;
	s_playersettings.modelsleft.x				= 34;
	s_playersettings.modelsleft.y				= 33;
	s_playersettings.modelsleft.w				= 55;
	s_playersettings.modelsleft.h				= 117;
	s_playersettings.modelsleft.sx				= 38;
	s_playersettings.modelsleft.sy				= 38;
	s_playersettings.modelsleft.sw				= 56;
	s_playersettings.modelsleft.sh				= 117;
	s_playersettings.modelsleft.shader			= trap_R_RegisterShaderNoMip("menu/player/left.tga");
	s_playersettings.modelsleft.mouseovershader	= trap_R_RegisterShaderNoMip("menu/player/left_mOver.tga");
	s_playersettings.modelsleft.generic.callback= PlayerSettings_MenuEvent;
	s_playersettings.modelsleft.generic.id		= ID_MODELSLEFT;

	Menu_AddItem(&s_playersettings.menu, &s_playersettings.modelsleft);

	s_playersettings.modelsright.generic.type	= MTYPE_BITMAP1024S;
	s_playersettings.modelsright.x				= 937;
	s_playersettings.modelsright.y				= 32;
	s_playersettings.modelsright.w				= 55;
	s_playersettings.modelsright.h				= 117;
	s_playersettings.modelsright.sx				= 933;
	s_playersettings.modelsright.sy				= 35;
	s_playersettings.modelsright.sw				= 58;
	s_playersettings.modelsright.sh				= 120;
	s_playersettings.modelsright.shader			= trap_R_RegisterShaderNoMip("menu/player/right.tga");
	s_playersettings.modelsright.mouseovershader= trap_R_RegisterShaderNoMip("menu/player/right_mOver.tga");
	s_playersettings.modelsright.generic.callback= PlayerSettings_MenuEvent;
	s_playersettings.modelsright.generic.id		= ID_MODELSRIGHT;

	Menu_AddItem(&s_playersettings.menu, &s_playersettings.modelsright);

	s_playersettings.skinup.generic.type	= MTYPE_BITMAP1024S;
	s_playersettings.skinup.x				= 873;
	s_playersettings.skinup.y				= 193;
	s_playersettings.skinup.w				= 117;
	s_playersettings.skinup.h				= 37;
	s_playersettings.skinup.sx				= 875;
	s_playersettings.skinup.sy				= 199;
	s_playersettings.skinup.sw				= 119;
	s_playersettings.skinup.sh				= 39;
	s_playersettings.skinup.shader			= trap_R_RegisterShaderNoMip("menu/player/up.tga");
	s_playersettings.skinup.mouseovershader	= trap_R_RegisterShaderNoMip("menu/player/up_mOver.tga");
	s_playersettings.skinup.generic.callback= PlayerSettings_MenuEvent;
	s_playersettings.skinup.generic.id		= ID_SKINSUP;

	Menu_AddItem(&s_playersettings.menu, &s_playersettings.skinup);

	s_playersettings.skindown.generic.type	= MTYPE_BITMAP1024S;
	s_playersettings.skindown.x				= 871;
	s_playersettings.skindown.y				= 687;
	s_playersettings.skindown.w				= 117;
	s_playersettings.skindown.h				= 40;
	s_playersettings.skindown.sx			= 876;
	s_playersettings.skindown.sy			= 692;
	s_playersettings.skindown.sw			= 117;
	s_playersettings.skindown.sh			= 40;
	s_playersettings.skindown.shader		= trap_R_RegisterShaderNoMip("menu/player/down.tga");
	s_playersettings.skindown.mouseovershader= trap_R_RegisterShaderNoMip("menu/player/down_mOver.tga");
	s_playersettings.skindown.generic.callback= PlayerSettings_MenuEvent;
	s_playersettings.skindown.generic.id	= ID_SKINSDOWN;

	Menu_AddItem(&s_playersettings.menu, &s_playersettings.skindown);

	for(i=0;i<DISPLAYED_MODELS;++i)
	{
		s_playersettings.model_icons[i].generic.type	= MTYPE_BITMAP1024S;
		s_playersettings.model_icons[i].x				= 160+i*(160+20);
		s_playersettings.model_icons[i].y				= 16;
		s_playersettings.model_icons[i].w				= 
		s_playersettings.model_icons[i].h				= 160;
		s_playersettings.model_icons[i].shader			= 0;
		s_playersettings.model_icons[i].mouseovershader	= 0;
		s_playersettings.model_icons[i].generic.callback= PlayerSettings_MenuEvent;
		s_playersettings.model_icons[i].generic.id		= ID_MICON+i;
//		s_playersettings.model_icons[i].generic.flags	= QMF_SILENT;

		Menu_AddItem(&s_playersettings.menu, &s_playersettings.model_icons[i]);
	}

	s_playersettings.skin_icons[0].generic.type		= MTYPE_BITMAP1024S;
	s_playersettings.skin_icons[0].x				= 867;
	s_playersettings.skin_icons[0].y				= 253;
	s_playersettings.skin_icons[0].w				= 
	s_playersettings.skin_icons[0].h				= 128;
	s_playersettings.skin_icons[0].sx				= 866;
	s_playersettings.skin_icons[0].sy				= 253;
	s_playersettings.skin_icons[0].sw				= 
	s_playersettings.skin_icons[0].sh				= 138;
	s_playersettings.skin_icons[0].shader			= 0;
	s_playersettings.skin_icons[0].shadowshader		= trap_R_RegisterShaderNoMip("menu/player/sicon_shadow.tga");
	s_playersettings.skin_icons[0].generic.callback	= PlayerSettings_MenuEvent;
	s_playersettings.skin_icons[0].generic.id		= ID_SICON;

	s_playersettings.skin_icons[1].generic.type		= MTYPE_BITMAP1024S;
	s_playersettings.skin_icons[1].x				= 867;
	s_playersettings.skin_icons[1].y				= 396;
	s_playersettings.skin_icons[1].w				= 
	s_playersettings.skin_icons[1].h				= 128;
	s_playersettings.skin_icons[1].sx				= 866;
	s_playersettings.skin_icons[1].sy				= 395;
	s_playersettings.skin_icons[1].sw				= 
	s_playersettings.skin_icons[1].sh				= 138;
	s_playersettings.skin_icons[1].shader			= 0;
	s_playersettings.skin_icons[1].shadowshader		= trap_R_RegisterShaderNoMip("menu/player/sicon_shadow.tga");
	s_playersettings.skin_icons[1].generic.callback	= PlayerSettings_MenuEvent;
	s_playersettings.skin_icons[1].generic.id		= ID_SICON+1;

	s_playersettings.skin_icons[2].generic.type		= MTYPE_BITMAP1024S;
	s_playersettings.skin_icons[2].x				= 867;
	s_playersettings.skin_icons[2].y				= 540;
	s_playersettings.skin_icons[2].w				= 
	s_playersettings.skin_icons[2].h				= 128;
	s_playersettings.skin_icons[2].sx				= 866;
	s_playersettings.skin_icons[2].sy				= 539;
	s_playersettings.skin_icons[2].sw				= 
	s_playersettings.skin_icons[2].sh				= 138;
	s_playersettings.skin_icons[2].shader			= 0;
	s_playersettings.skin_icons[2].shadowshader		= trap_R_RegisterShaderNoMip("menu/player/sicon_shadow.tga");
	s_playersettings.skin_icons[2].generic.callback	= PlayerSettings_MenuEvent;
	s_playersettings.skin_icons[2].generic.id		= ID_SICON+2;

	Menu_AddItem(&s_playersettings.menu, &s_playersettings.skin_icons[0]);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.skin_icons[1]);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.skin_icons[2]);


	{
		char	spraylogoName[MAX_SPRAYLOGO_NAME];
		int		i;

		trap_Cvar_VariableStringBuffer( "syc_logo", spraylogoName, sizeof( spraylogoName ) );

		if ( spraylogoName[0] == '\0' ) {
			Q_strncpyz( spraylogoName, SPRAYLOGO_DEFAULT_NAME, sizeof( spraylogoName ) );
		}

		for( i = 0; i < uis.spraylogosLoaded; i++ ) {
			if ( Q_stricmp( uis.spraylogoNames[i], spraylogoName ) == 0 ) {
				s_playersettings.slogo_num = i;
				break;
			}
		}
	}

	s_playersettings.spraylogos_prev.generic.type	= MTYPE_BITMAP1024S;
	s_playersettings.spraylogos_prev.x				= 76;
	s_playersettings.spraylogos_prev.y				= 570;
	s_playersettings.spraylogos_prev.w				= 40;
	s_playersettings.spraylogos_prev.h				= 20;
	s_playersettings.spraylogos_prev.shader			= trap_R_RegisterShaderNoMip("menu/smallarrow_left.tga");
	s_playersettings.spraylogos_prev.mouseovershader= trap_R_RegisterShaderNoMip("menu/smallarrow_leftdown.tga");
	s_playersettings.spraylogos_prev.generic.callback= PlayerSettings_MenuEvent;
	s_playersettings.spraylogos_prev.generic.id		= ID_SLOGOPREV;

	s_playersettings.spraylogos_next.generic.type	= MTYPE_BITMAP1024S;
	s_playersettings.spraylogos_next.x				= 130;
	s_playersettings.spraylogos_next.y				= 570;
	s_playersettings.spraylogos_next.w				= 40;
	s_playersettings.spraylogos_next.h				= 20;
	s_playersettings.spraylogos_next.shader			= trap_R_RegisterShaderNoMip("menu/smallarrow_right.tga");
	s_playersettings.spraylogos_next.mouseovershader= trap_R_RegisterShaderNoMip("menu/smallarrow_rightdown.tga");
	s_playersettings.spraylogos_next.generic.callback= PlayerSettings_MenuEvent;
	s_playersettings.spraylogos_next.generic.id		= ID_SLOGONEXT;

	Menu_AddItem(&s_playersettings.menu, &s_playersettings.spraylogos_prev);
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.spraylogos_next);

	s_playersettings.back.generic.type			= MTYPE_BITMAP;
	s_playersettings.back.generic.name			= BACK0;
	s_playersettings.back.generic.flags			= QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_playersettings.back.generic.x				= 8;
	s_playersettings.back.generic.y				= 440;
	s_playersettings.back.generic.id			= ID_BACK;
	s_playersettings.back.generic.callback		= PlayerSettings_MenuEvent;
    s_playersettings.back.width					= 80;
	s_playersettings.back.height				= 40;
	s_playersettings.back.focuspic				= BACK1;
	s_playersettings.back.focuspicinstead		= qtrue;

	s_playersettings.name.generic.type			= MTYPE_FIELD;
	s_playersettings.name.generic.flags			= QMF_NODEFAULTINIT;
	s_playersettings.name.generic.ownerdraw		= PlayerSettings_DrawName;
	s_playersettings.name.field.widthInChars	= MAX_NAMELENGTH;
	s_playersettings.name.field.maxchars		= MAX_NAMELENGTH;
	s_playersettings.name.generic.x				= 20;
	s_playersettings.name.generic.y				= 160;
	s_playersettings.name.generic.left			= 20 - 8;
	s_playersettings.name.generic.top			= 160 - 8;
	s_playersettings.name.generic.right			= 20 + 130;
	s_playersettings.name.generic.bottom		= 160 + 2 * PROP_HEIGHT;

	s_playersettings.spraycolor.generic.type	= MTYPE_BITMAP;
	s_playersettings.spraycolor.generic.flags	= QMF_LEFT_JUSTIFY;
	s_playersettings.spraycolor.generic.x		= 36;
	s_playersettings.spraycolor.generic.y		= 379;
	s_playersettings.spraycolor.width			= 82;
	s_playersettings.spraycolor.height			= 18;
	s_playersettings.spraycolor.generic.id		= ID_SPRAYCOLOR;
	s_playersettings.spraycolor.generic.callback= PlayerSettings_MenuEvent;
	Menu_AddItem(&s_playersettings.menu, &s_playersettings.spraycolor);

	Menu_AddItem( &s_playersettings.menu, &s_playersettings.name );
	Menu_AddItem( &s_playersettings.menu, &s_playersettings.back );

	PlayerSettings_SetMenuItems();
}


/*
=================
PlayerSettings_Cache
=================
*/
void PlayerSettings_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_FRAMEL );
	trap_R_RegisterShaderNoMip( ART_FRAMER );
	trap_R_RegisterShaderNoMip( ART_MODEL0 );
	trap_R_RegisterShaderNoMip( ART_MODEL1 );
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );

	s_playersettings.fxBasePic = trap_R_RegisterShaderNoMip( ART_FX_BASE );
	s_playersettings.fxPic[0] = trap_R_RegisterShaderNoMip( ART_FX_RED );
	s_playersettings.fxPic[1] = trap_R_RegisterShaderNoMip( ART_FX_YELLOW );
	s_playersettings.fxPic[2] = trap_R_RegisterShaderNoMip( ART_FX_GREEN );
	s_playersettings.fxPic[3] = trap_R_RegisterShaderNoMip( ART_FX_TEAL );
	s_playersettings.fxPic[4] = trap_R_RegisterShaderNoMip( ART_FX_BLUE );
	s_playersettings.fxPic[5] = trap_R_RegisterShaderNoMip( ART_FX_CYAN );
	s_playersettings.fxPic[6] = trap_R_RegisterShaderNoMip( ART_FX_WHITE );

	trap_R_RegisterShaderNoMip( BACK0 );
	trap_R_RegisterShaderNoMip( BACK1 );

	PlayerSettings_BuildList();
}


/*
=================
UI_PlayerSettingsMenu
=================
*/
void UI_PlayerSettingsMenu( void ) {
	PlayerSettings_MenuInit();
	UI_PushMenu( &s_playersettings.menu );
}
