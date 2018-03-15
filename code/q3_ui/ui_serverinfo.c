// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "ui_local.h"

#define ARROWUP0	"menu/server/arrowup0"
#define ARROWUP1	"menu/server/arrowup1"
#define ARROWDOWN0	"menu/server/arrowdown0"
#define ARROWDOWN1	"menu/server/arrowdown1"

static char* serverinfo_artlist[] =
{
	ARROWUP0,
	ARROWUP1,
	ARROWDOWN0,
	ARROWDOWN1,

	NULL
};

#define ID_ADD	 100
#define ID_BACK	 101

#define ID_SCROLL_UP		102
#define ID_SCROLL_DOWN		103

typedef struct
{
	menuframework_s	menu;

	menubitmap1024s_s	arrowup;
	menubitmap1024s_s	arrowdown;
	menutext_s			back;

	menutext_s		add;
	char			info[MAX_INFO_STRING];
	int				numlines;

	int				firstline;
	int				numdrawn;
} serverinfo_t;

static serverinfo_t	s_serverinfo;


/*
=================
Favorites_Add

Add current server to favorites
=================
*/
void Favorites_Add( void )
{
	char	adrstr[128];
	char	serverbuff[128];
	int		i;
	int		best;

	trap_Cvar_VariableStringBuffer( "cl_currentServerAddress", serverbuff, sizeof(serverbuff) );
	if (!serverbuff[0])
		return;

	best = 0;
	for (i=0; i<MAX_FAVORITESERVERS; i++)
	{
		trap_Cvar_VariableStringBuffer( va("server%d",i+1), adrstr, sizeof(adrstr) );
		if (!Q_stricmp(serverbuff,adrstr))
		{
			// already in list
			return;
		}

		if(adrstr[0]=='\0') // if there is a empty one -> take it ;)
		{
			best = i+1;
			break;
		}
		
		// use first empty or non-numeric available slot
		if ((adrstr[0]  < '0' || adrstr[0] > '9' ) && !best)
			best = i+1;
	}

	if (best)
		trap_Cvar_Set( va("server%d",best), serverbuff);
}


/*
=================
ServerInfo_Event
=================
*/
static void ServerInfo_Event( void* ptr, int event )
{
	switch (((menucommon_s*)ptr)->id)
	{
		case ID_ADD:
			if (event != QM_ACTIVATED)
				break;
		
			Favorites_Add();
			UI_PopMenu();
			break;

		case ID_BACK:
			if (event != QM_ACTIVATED)
				break;

			UI_PopMenu();
			break;

		case ID_SCROLL_UP:
			if (event != QM_ACTIVATED)
				break;

			if(s_serverinfo.firstline>0) s_serverinfo.firstline--;
			break;

		case ID_SCROLL_DOWN:
			if (event != QM_ACTIVATED)
				break;

			if ( ( s_serverinfo.firstline + s_serverinfo.numdrawn ) < s_serverinfo.numlines ) {
				s_serverinfo.firstline++;
			}
			break;
	}
}

/*
=================
ServerInfo_MenuDraw
=================
*/
static void ServerInfo_MenuDraw( void )
{
	const char		*s;
	char			key[MAX_INFO_KEY];
	char			value[MAX_INFO_VALUE];
	int				y, i=0;
	int				keylen, vallen, infonum=-1;

	UI_DrawIngameBG();
	UI_DrawProportionalString( 320, 110, "SERVER INFO",UI_CENTER|UI_SMALLFONT,color_black);

	y = 140;//95;
	s = s_serverinfo.info;
	s_serverinfo.numdrawn = 0;
	while ( s && i < s_serverinfo.numlines ) {
		i++;
		Info_NextPair( &s, key, value );
		if ( !key[0] ) {
			break;
		}

		infonum++;
		if(s_serverinfo.firstline>infonum)
			continue;

		if(y>260) break;

		Com_sprintf(key,MAX_INFO_KEY,"%s: ",key);
		keylen=Q_PrintStrlen(key);
		vallen=Q_PrintStrlen(value);
		if(keylen+vallen<20)
		{
			UI_DrawString(230,y,key,UI_LEFT|UI_SMALLFONT,color_black);
			UI_DrawString(230+keylen*8,y,value,UI_LEFT|UI_SMALLFONT,color_blue);

			s_serverinfo.numdrawn++;
		}
		else
		{
			int i;

			// TODO: Also add linebreaks for long keys?
			UI_DrawString(230,y,key,UI_LEFT|UI_SMALLFONT,color_black);
			
			for(i=0;i<vallen;i+=20)
			{
				y += SMALLCHAR_HEIGHT;
				if(y>260) break;

				UI_DrawString(230,y,va("%20.20s",&value[i]),UI_LEFT|UI_SMALLFONT,color_blue);

				s_serverinfo.numdrawn++;
			}
		}

		y += SMALLCHAR_HEIGHT;
	}

	Menu_Draw( &s_serverinfo.menu );
}

/*
=================
ServerInfo_MenuKey
=================
*/
static sfxHandle_t ServerInfo_MenuKey( int key )
{
	switch ( key ) {
	case K_MWHEELUP:
	case K_PGUP:
		ServerInfo_Event( &s_serverinfo.arrowup, QM_ACTIVATED );
		break;
	case K_MWHEELDOWN:
	case K_PGDN:
		ServerInfo_Event( &s_serverinfo.arrowdown, QM_ACTIVATED );
		break;
	}

	return ( Menu_DefaultKey( &s_serverinfo.menu, key ) );
}

/*
=================
ServerInfo_Cache
=================
*/
void ServerInfo_Cache( void )
{
	int	i;

	// touch all our pics
	for (i=0; ;i++)
	{
		if (!serverinfo_artlist[i])
			break;
		trap_R_RegisterShaderNoMip(serverinfo_artlist[i]);
	}
}

/*
=================
UI_ServerInfoMenu
=================
*/
void UI_ServerInfoMenu( void )
{
	const char		*s;
	char			key[MAX_INFO_KEY];
	char			value[MAX_INFO_VALUE];

	// zero set all our globals
	memset( &s_serverinfo, 0 ,sizeof(serverinfo_t) );

	ServerInfo_Cache();

	s_serverinfo.menu.draw       = ServerInfo_MenuDraw;
	s_serverinfo.menu.key        = ServerInfo_MenuKey;
	s_serverinfo.arrowup.generic.type		= MTYPE_BITMAP1024S;
	s_serverinfo.arrowup.x					= 630;
	s_serverinfo.arrowup.y					= 226;
	s_serverinfo.arrowup.w					= 29;
	s_serverinfo.arrowup.h					= 74;
	s_serverinfo.arrowup.shader				= trap_R_RegisterShaderNoMip(ARROWUP0);
	s_serverinfo.arrowup.mouseovershader	= trap_R_RegisterShaderNoMip(ARROWUP1);
	s_serverinfo.arrowup.generic.callback	= ServerInfo_Event;
	s_serverinfo.arrowup.generic.id			= ID_SCROLL_UP;
	Menu_AddItem( &s_serverinfo.menu, (void*) &s_serverinfo.arrowup );

	s_serverinfo.arrowdown.generic.type		= MTYPE_BITMAP1024S;
	s_serverinfo.arrowdown.x				= 630;
	s_serverinfo.arrowdown.y				= 436-74;
	s_serverinfo.arrowdown.w				= 29;//38
	s_serverinfo.arrowdown.h				= 74;//98
	s_serverinfo.arrowdown.shader			= trap_R_RegisterShaderNoMip(ARROWDOWN0);
	s_serverinfo.arrowdown.mouseovershader	= trap_R_RegisterShaderNoMip(ARROWDOWN1);
	s_serverinfo.arrowdown.generic.callback	= ServerInfo_Event;
	s_serverinfo.arrowdown.generic.id		= ID_SCROLL_DOWN;
	Menu_AddItem( &s_serverinfo.menu, (void*) &s_serverinfo.arrowdown );

	s_serverinfo.add.generic.type		= MTYPE_TEXTS;
	s_serverinfo.add.fontHeight			= 18.0f;
	s_serverinfo.add.generic.flags		= QMF_CENTER_JUSTIFY;//|QMF_PULSEIFFOCUS;
	s_serverinfo.add.generic.callback	= ServerInfo_Event;
	s_serverinfo.add.generic.id			= ID_ADD;
	s_serverinfo.add.generic.x			= 320;
	s_serverinfo.add.generic.y			= 290;
	s_serverinfo.add.string				= "ADD TO FAVORiTES";
	s_serverinfo.add.style				= UI_CENTER|UI_SMALLFONT;
	s_serverinfo.add.color				= colorDkBlue;
	s_serverinfo.add.focuscolor			= colorBlue;
	if( trap_Cvar_VariableValue( "sv_running" ) ) {
		s_serverinfo.add.generic.flags |= QMF_GRAYED;
	}

	s_serverinfo.back.generic.type		= MTYPE_TEXTS;
	s_serverinfo.back.fontHeight		= 16.0f;
//	s_serverinfo.back.generic.flags		= QMF_PULSEIFFOCUS;
	s_serverinfo.back.generic.callback	= ServerInfo_Event;
	s_serverinfo.back.generic.id		= ID_BACK;
	s_serverinfo.back.generic.x			= 245;
	s_serverinfo.back.generic.y			= 315;
	s_serverinfo.back.string			= "BACK";
	s_serverinfo.back.style				= UI_SMALLFONT;
	s_serverinfo.back.color				= color_black;
	s_serverinfo.back.focuscolor		= color_orange;

	trap_GetConfigString( CS_SERVERINFO, s_serverinfo.info, MAX_INFO_STRING );

	s_serverinfo.numlines = 0;
	s = s_serverinfo.info;
	while ( s ) {
		Info_NextPair( &s, key, value );
		if ( !key[0] ) {
			break;
		}
		s_serverinfo.numlines++;
	}


	Menu_AddItem( &s_serverinfo.menu, (void*) &s_serverinfo.add );
	Menu_AddItem( &s_serverinfo.menu, (void*) &s_serverinfo.back );

	UI_PushMenu( &s_serverinfo.menu );
}



