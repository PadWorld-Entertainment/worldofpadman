/*
####################### ####################### ####################### ####################### #######################

  exit(+credits) menu

####################### ####################### ####################### ####################### #######################
*/

#include "ui_local.h"

#define ID_EXIT_YES			10
#define ID_EXIT_NO			11

#define EXIT_YES0		"menu/exit/yes0"
#define EXIT_YES1		"menu/exit/yes1"
#define EXIT_NO0		"menu/exit/no0"
#define EXIT_NO1		"menu/exit/no1"
#define EXIT_THEEXIT	"menu/exit/theend"

typedef struct
{
	menuframework_s	menu;

	menubitmap_s	Yes;
	menubitmap_s	No;

	qhandle_t		theexit;
	int				starttime;
}
exitmenu_t;

static exitmenu_t exitmenu;

/*
#######################
ExitMenu_Event
#######################
*/
void ExitMenu_Event (void* ptr, int event)
{
	if( event != QM_ACTIVATED )
		return;

	switch( ((menucommon_s*)ptr)->id )
	{
	case ID_EXIT_YES:
		UI_PopMenu();
		UI_PopMenu();
		UI_CreditMenu();
		break;

	case ID_EXIT_NO:
		UI_PopMenu();
		break;
	}
}

/*
#######################
ExitMenu_Cache
#######################
*/
void ExitMenu_Cache(void)
{
	trap_R_RegisterShaderNoMip(EXIT_YES0);
	trap_R_RegisterShaderNoMip(EXIT_YES1);
	trap_R_RegisterShaderNoMip(EXIT_NO0);
	trap_R_RegisterShaderNoMip(EXIT_NO1);
	exitmenu.theexit=trap_R_RegisterShaderNoMip(EXIT_THEEXIT);
}

/*
#######################
ExitMenu_Draw
#######################
*/
static void ExitMenu_Draw(void)
{
	float x,y,w,h,scale;

	scale=(float)(uis.realtime-exitmenu.starttime)*0.003f;//0.001f;
	if(scale>1.0f) scale=1.0f;
	else if(scale<0.0f) scale=0.0f;

	//mitte: 320/209
	w=310.0f*scale;
	h=110.0f*scale;
	x=320.0f-w*0.5f;
	y=209.0f-h*0.5f;

	UI_AdjustFrom640(&x,&y,&w,&h);

	trap_R_DrawStretchPic(x,y,w,h,0,0,1,1,exitmenu.theexit);

	Menu_Draw( &exitmenu.menu );
}

/*
#######################
ExitMenu_Init
#######################
*/
void ExitMenu_Init(void)
{
	memset( &exitmenu, 0 ,sizeof(exitmenu) );

	ExitMenu_Cache();
	exitmenu.starttime=uis.realtime+100;

	exitmenu.menu.fullscreen = qtrue;
	exitmenu.menu.wrapAround = qtrue;
	exitmenu.menu.bgparts = BGP_EXITBG;
	exitmenu.menu.draw = ExitMenu_Draw;

	exitmenu.Yes.generic.type		= MTYPE_BITMAP;
	exitmenu.Yes.generic.name		= EXIT_YES0;
	exitmenu.Yes.generic.flags		= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    exitmenu.Yes.generic.x			= 256;
	exitmenu.Yes.generic.y			= 378;
	exitmenu.Yes.generic.id			= ID_EXIT_YES;
	exitmenu.Yes.generic.callback	= ExitMenu_Event;
    exitmenu.Yes.width				= 65;
	exitmenu.Yes.height				= 40;
	exitmenu.Yes.focuspic			= EXIT_YES1;
	exitmenu.Yes.focuspicinstead	= qtrue;
	Menu_AddItem(&exitmenu.menu, &exitmenu.Yes);

	exitmenu.No.generic.type		= MTYPE_BITMAP;
	exitmenu.No.generic.name		= EXIT_NO0;
	exitmenu.No.generic.flags		= QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
    exitmenu.No.generic.x			= 336;
	exitmenu.No.generic.y			= 378;
	exitmenu.No.generic.id			= ID_EXIT_NO;
	exitmenu.No.generic.callback	= ExitMenu_Event;
    exitmenu.No.width				= 45;
	exitmenu.No.height				= 40;
	exitmenu.No.focuspic			= EXIT_NO1;
	exitmenu.No.focuspicinstead		= qtrue;
	Menu_AddItem(&exitmenu.menu, &exitmenu.No);

	UI_PushMenu(&exitmenu.menu);
}
