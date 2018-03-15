// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "ui_local.h"

/*********************************************************************************
	SPECIFY SERVER
*********************************************************************************/

#define SPECIFYSERVER_BACK0		"menu/BtnBack0"
#define SPECIFYSERVER_BACK1		"menu/BtnBack1"
#define SPECIFYSERVER_FIGHT0	"menu/specify/fight0"
#define SPECIFYSERVER_FIGHT1	"menu/specify/fight1"

#define ID_SPECIFYSERVERBACK	102
#define ID_SPECIFYSERVERGO		103

static char* specifyserver_artlist[] =
{
	SPECIFYSERVER_BACK0,	
	SPECIFYSERVER_BACK1,	
	SPECIFYSERVER_FIGHT0,
	SPECIFYSERVER_FIGHT1,
	NULL
};

typedef struct
{
	menuframework_s	menu;
	menufield_s		domain;
	menufield_s		port;
	menubitmap1024s_s	go;
	menubitmap_s	back;
} specifyserver_t;

static specifyserver_t	s_specifyserver;

/*
=================
SpecifyServer_Event
=================
*/
static void SpecifyServer_Event( void* ptr, int event )
{
	char	buff[256];

	switch (((menucommon_s*)ptr)->id)
	{
		case ID_SPECIFYSERVERGO:
			if (event != QM_ACTIVATED)
				break;

			if (s_specifyserver.domain.field.buffer[0])
			{
				strcpy(buff,s_specifyserver.domain.field.buffer);
				if (s_specifyserver.port.field.buffer[0])
					Com_sprintf( buff+strlen(buff), 128, ":%s", s_specifyserver.port.field.buffer );

				trap_Cmd_ExecuteText( EXEC_APPEND, va( "connect %s\n", buff ) );
			}
			break;

		case ID_SPECIFYSERVERBACK:
			if (event != QM_ACTIVATED)
				break;

			UI_PopMenu();
			break;
	}
}

static void SpecifyServer_DrawField( void *self ) {
	menufield_s		*f;
	qboolean		focus;
	int				style;
	char			*txt;
	char			c;
	float			*color;
	int				n;
	int				basex, x, y;
//	vec4_t			highlight_color = {0.00f, 0.00f, 0.00f, 1.00f};
	float*			highlight_color = text_color_highlight;
	vec4_t			bg_color = {1.00f, 1.00f, 1.00f, 0.50f};
//	float*			bg_color = listbar_color;

	f = (menufield_s*)self;
	y = f->generic.y;
	basex = f->generic.x+(f->field.widthInChars-Q_PrintStrlen(f->field.buffer))/2*SMALLCHAR_WIDTH;
	focus = (f->generic.parent->cursor == f->generic.menuPosition);

	style = UI_LEFT|UI_SMALLFONT;
	color = text_color_normal;

	txt = f->field.buffer;
//	color = g_color_table[ColorIndex(COLOR_WHITE)];
	if(focus)
		color = highlight_color;
	else
		color = text_color_normal;

	// draw cursor if we have focus
	if( focus ) {
		if ( trap_Key_GetOverstrikeMode() ) {
			c = 11;
		} else {
			c = 10;
		}

		UI_FillRect(f->generic.x,f->generic.y,f->field.widthInChars*SMALLCHAR_WIDTH,SMALLCHAR_HEIGHT,bg_color);

		style |= UI_BLINK;
		UI_DrawChar( basex + f->field.cursor * SMALLCHAR_WIDTH, y, c, style, highlight_color );
		style &= ~UI_BLINK;
	}

	if( focus ) {
		style |= UI_PULSE;
		color = highlight_color;
	}

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
}

/*
=================
SpecifyServer_MenuInit
=================
*/
void SpecifyServer_MenuInit( void )
{
	// zero set all our globals
	memset( &s_specifyserver, 0 ,sizeof(specifyserver_t) );

	SpecifyServer_Cache();

	s_specifyserver.menu.wrapAround = qtrue;
	s_specifyserver.menu.fullscreen = qtrue;
	s_specifyserver.menu.bgparts	= BGP_SPECIFYBG|BGP_SIMPLEBG;


	s_specifyserver.domain.generic.type       = MTYPE_FIELD;
	s_specifyserver.domain.generic.name       = "";
	s_specifyserver.domain.generic.flags      = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_specifyserver.domain.generic.x	      = 230;//215;
	s_specifyserver.domain.generic.y	      = 230;
	s_specifyserver.domain.field.widthInChars = 22;//28;
	s_specifyserver.domain.field.maxchars     = 80;
	s_specifyserver.domain.generic.ownerdraw  = SpecifyServer_DrawField;

	s_specifyserver.port.generic.type       = MTYPE_FIELD;
	s_specifyserver.port.generic.name	    = "";
	s_specifyserver.port.generic.flags	    = QMF_PULSEIFFOCUS|QMF_SMALLFONT|QMF_NUMBERSONLY;
	s_specifyserver.port.generic.x	        = 230;//215;
	s_specifyserver.port.generic.y	        = 275;
	s_specifyserver.port.field.widthInChars = 22;//28;
	s_specifyserver.port.field.maxchars     = 5;
	s_specifyserver.port.generic.ownerdraw  = SpecifyServer_DrawField;

	s_specifyserver.back.generic.type		= MTYPE_BITMAP;
	s_specifyserver.back.generic.name		= SPECIFYSERVER_BACK0;
	s_specifyserver.back.generic.flags		= QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_specifyserver.back.generic.x			= 8;
	s_specifyserver.back.generic.y			= 440;
	s_specifyserver.back.generic.id			= ID_SPECIFYSERVERBACK;
	s_specifyserver.back.generic.callback	= SpecifyServer_Event;
    s_specifyserver.back.width				= 80;
	s_specifyserver.back.height				= 40;
	s_specifyserver.back.focuspic			= SPECIFYSERVER_BACK1;
	s_specifyserver.back.focuspicinstead	= qtrue;

	s_specifyserver.go.generic.type		= MTYPE_BITMAP1024S;
	s_specifyserver.go.x				= 845;//820;
	s_specifyserver.go.y				= 658;//620;
	s_specifyserver.go.w				= 135;//187;
	s_specifyserver.go.h				= 97;//134;
	s_specifyserver.go.shader			= trap_R_RegisterShaderNoMip(SPECIFYSERVER_FIGHT0);
	s_specifyserver.go.mouseovershader	= trap_R_RegisterShaderNoMip(SPECIFYSERVER_FIGHT1);
	s_specifyserver.go.generic.callback	= SpecifyServer_Event;
	s_specifyserver.go.generic.id		= ID_SPECIFYSERVERGO;

	Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.domain );
	Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.port );
	Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.go );
	Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.back );

	Com_sprintf( s_specifyserver.port.field.buffer, 6, "%i", 27960 );
}

/*
=================
SpecifyServer_Cache
=================
*/
void SpecifyServer_Cache( void )
{
	int	i;

	// touch all our pics
	for (i=0; ;i++)
	{
		if (!specifyserver_artlist[i])
			break;
		trap_R_RegisterShaderNoMip(specifyserver_artlist[i]);
	}
}

/*
=================
UI_SpecifyServerMenu
=================
*/
void UI_SpecifyServerMenu( void )
{
	SpecifyServer_MenuInit();
	UI_PushMenu( &s_specifyserver.menu );
}

