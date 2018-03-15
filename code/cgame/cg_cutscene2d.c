
#include "cg_local.h"


#define MAX_CUTSCENE2D_PICTURES 16
#define MAX_CUTSCENE2D_TRANSFORMATIONS 16

#define MAX_TEXT_PER_CSPICTURE	256

//NOTE: the comments "//(...?)" are just notes of ideas, that could be added in future ...
typedef struct {
	float			x, y;
	float			w, h;
	//(angle?)

	qhandle_t		shader;
	vec4_t			color, textcolor;

	char			text[MAX_TEXT_PER_CSPICTURE];
	float			text_x,text_y;
	float			text_cw, text_ch;

	qboolean		isShown;
} cutscene2dPicture_t;

typedef enum {
	C2DTT_UNUSED=0,
	C2DTT_MOVE, // v1/v2 = velocity x/y in pixel/ms
	C2DTT_RESIZE, // v1/v2 = resize-velocity x/y in pixel/ms ( += onto w/h )
	C2DTT_FADEALPHA, // v1/v2 = alpha change for pic/text
} c2dTransType_t;
typedef struct {
	c2dTransType_t	type;
	int				picture; // handle which point at the picture, that should be transformed

	float			v1, v2; // value1/2
	int				lastCalc; // time in ms (ref cg.time)

	int				timeout; // the time when this transformation will be stopped (<=0 wont be stoped)
} cutscene2dTransformation_t;

typedef struct {

	cutscene2dPicture_t			pics[MAX_CUTSCENE2D_PICTURES];
	cutscene2dTransformation_t	trans[MAX_CUTSCENE2D_TRANSFORMATIONS];
} cg_cutscene2d_t;

static cg_cutscene2d_t cg_cutscene2d;

static qboolean cutscene2d_Inited = qfalse;


static qboolean isPicHandleOK(int i) {
	return (i>=0 && i<MAX_CUTSCENE2D_PICTURES);
}

static cutscene2dPicture_t* csPicByHandle(int i) {
	if(isPicHandleOK(i))
		return &cg_cutscene2d.pics[i];
	else
		return 0;
}

static void Cutscene2d_setPicDefault(cutscene2dPicture_t* pic) {
	pic->text_cw = 8;
	pic->text_ch = 16;

	pic->color[0] = pic->textcolor[0] = 1.0f;
	pic->color[1] = pic->textcolor[1] = 1.0f;
	pic->color[2] = pic->textcolor[2] = 1.0f;
	pic->color[3] = pic->textcolor[3] = 1.0f;
}
static void Cutscene2d_ResetPic(int picID) {
	if(isPicHandleOK(picID)) {
		int i;

		memset(&cg_cutscene2d.pics[picID],0,sizeof(cutscene2dPicture_t));
		Cutscene2d_setPicDefault(&cg_cutscene2d.pics[picID]);

		for(i=0;i<MAX_CUTSCENE2D_TRANSFORMATIONS;++i) {
			cutscene2dTransformation_t* tran = &cg_cutscene2d.trans[i];

			if(tran->picture==picID) {
				tran->type = C2DTT_UNUSED;
			}
		}

	}
}

static void Cutscene2d_ResetAll(void) {
	int i;
	memset(&cg_cutscene2d,0,sizeof(cg_cutscene2d));
	for(i=0;i<MAX_CUTSCENE2D_PICTURES;++i) {
		Cutscene2d_setPicDefault(&cg_cutscene2d.pics[i]);
	}
}

void CG_Cutscene2d_Init(void) {

	if(cutscene2d_Inited)
		return;

	Cutscene2d_ResetAll();

	cutscene2d_Inited = qtrue;
}


void CG_Cutscene2d_Draw(void) {
	int i;

	CG_Cutscene2d_Init(); // make sure that we are inited

	for(i=0;i<MAX_CUTSCENE2D_PICTURES;++i) {
		cutscene2dPicture_t* pic = &cg_cutscene2d.pics[i];

		if(pic->isShown) {

			trap_R_SetColor(pic->color);
				CG_DrawPic(pic->x,pic->y,pic->w,pic->h,pic->shader);
			trap_R_SetColor(NULL);

			if(pic->text[0]) {
				char* start, *end;
				int line=0;

				start = pic->text;
				do {
					end = strstr(start,"\\n");
					if(end) end[0] = '\0';

					CG_DrawStringExt(pic->x+pic->text_x,pic->y+pic->text_y +line*pic->text_ch,
									start,pic->textcolor, qfalse, qfalse, pic->text_cw, pic->text_ch, 0 );

					if(end) {
						end[0] = '\\';
						start = end+2;
					}
					++line;
				} while(end);
			}
		}
	}
}

void CG_Cutscene2d_UpdateTrans(void) {
	int i;

	CG_Cutscene2d_Init(); // make sure that we are inited

	for(i=0;i<MAX_CUTSCENE2D_TRANSFORMATIONS;++i) {
		cutscene2dTransformation_t* tran = &cg_cutscene2d.trans[i];

		if(tran->type!=C2DTT_UNUSED) {
			int dT;
			cutscene2dPicture_t* pic;

			if(!isPicHandleOK(tran->picture)) {
				tran->type = C2DTT_UNUSED;
				continue;
			}
			pic = &cg_cutscene2d.pics[tran->picture];

			if(tran->timeout<cg.time) // some restTime from the last frame
				dT = tran->timeout-tran->lastCalc;
			else
				dT = (cg.time-tran->lastCalc);
			if(dT<=0)
				continue;

			switch(tran->type) {
			case C2DTT_MOVE:
				pic->x += tran->v1 * dT;
				pic->y += tran->v2 * dT;
				break;
			case C2DTT_RESIZE:
				pic->w += tran->v1 * dT;
				pic->h += tran->v2 * dT;
				break;
			case C2DTT_FADEALPHA:
				pic->color[3] += tran->v1 * dT;
				if(pic->color[3]>1.0f) pic->color[3]=1.0f;
				if(pic->color[3]<0.0f) pic->color[3]=0.0f;
				pic->textcolor[3] += tran->v2 * dT;
				if(pic->textcolor[3]>1.0f) pic->textcolor[3]=1.0f;
				if(pic->textcolor[3]<0.0f) pic->textcolor[3]=0.0f;
				if(pic->color[3]==0 && pic->textcolor[3]==0)
					pic->isShown = qfalse;
				break;
			default:
				break;
			}

			if(tran->timeout>0 && tran->timeout<cg.time) {
				tran->type = C2DTT_UNUSED;
			}

			tran->lastCalc = cg.time;
		}
	}
}

static cutscene2dTransformation_t* getFreeTransformation(void) {
	int i;

	for(i=0;i<MAX_CUTSCENE2D_TRANSFORMATIONS;++i) {
		if(cg_cutscene2d.trans[i].type == C2DTT_UNUSED)
			return &cg_cutscene2d.trans[i];
	}

	return 0;
}

qboolean CG_Cutscene2d_CheckCmd(const char	*cmd) {
	cutscene2dPicture_t* pic;
	int argc;

	CG_Cutscene2d_Init(); // make sure that we are inited

	argc = trap_Argc();

	//(hide all?)

	if(!Q_stricmp(cmd,"cs2d_reset")) {
		if(argc==2) {
			if(!Q_stricmp(CG_Argv(1),"all"))
				Cutscene2d_ResetAll();
			else {
				int picID = atoi(CG_Argv(1));
				if(isPicHandleOK(picID)) {
					Cutscene2d_ResetPic(picID);
				}
			}
		}
		else {
			Com_Printf("usage: %s [all|picID]\n",cmd);
		}


		return qtrue;
	}

	else if(!Q_stricmp(cmd,"cs2d_createPic")) {
		if(argc==7) {
			pic = csPicByHandle(atoi(CG_Argv(1)));
			if(pic) {
				pic->x = atof(CG_Argv(2));
				pic->y = atof(CG_Argv(3));
				pic->shader = trap_R_RegisterShaderNoMip(CG_Argv(4));
				pic->w = atof(CG_Argv(5));
				pic->h = atof(CG_Argv(6));
				pic->isShown = qtrue;
			}
		}
		else {
			Com_Printf("usage: %s [picID] [x] [y] [shaderName] [w] [h]\n",cmd);
		}

		return qtrue;
	}

	else if(!Q_stricmpn(cmd,"cs2d_setPic",11)) {
		if(!Q_stricmp(cmd,"cs2d_setPicShader")) {
			if(argc==3) {
				pic = csPicByHandle(atoi(CG_Argv(1)));
				if(pic) {
					pic->shader = trap_R_RegisterShaderNoMip(CG_Argv(2));
				}
			}
			else {
				Com_Printf("usage: %s [picID] [shaderName]\n",cmd);
			}

			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_setPicShown")) {
			if(argc==3) {
				pic = csPicByHandle(atoi(CG_Argv(1)));
				if(pic) {
					pic->isShown = !!atoi(CG_Argv(2));
				}
			}
			else {
				Com_Printf("usage: %s [picID] [bool]\n",cmd);
			}

			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_setPicPosition")) {
			if(argc==4) {
				pic = csPicByHandle(atoi(CG_Argv(1)));
				if(pic) {
					pic->x = atof(CG_Argv(2));
					pic->y = atof(CG_Argv(3));
				}
			}
			else {
				Com_Printf("usage: %s [picID] [x] [y]\n",cmd);
			}

			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_setPicSize")) {
			if(argc==4) {
				pic = csPicByHandle(atoi(CG_Argv(1)));
				if(pic) {
					pic->w = atof(CG_Argv(2));
					pic->h = atof(CG_Argv(3));
				}
			}
			else {
				Com_Printf("usage: %s [picID] [w] [h]\n",cmd);
			}

			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_setPicColor")) {
			if(argc>=5) {
				pic = csPicByHandle(atoi(CG_Argv(1)));
				if(pic) {
					pic->color[0] = atof(CG_Argv(2));
					pic->color[1] = atof(CG_Argv(3));
					pic->color[2] = atof(CG_Argv(4));
					if(argc>=6)
						pic->color[3] = atof(CG_Argv(5));
				}
			}
			else {
				Com_Printf("usage: %s [picID] [r] [g] [b] ([a])\n",cmd);
			}
			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_setPicText")) {
			if(argc==3) {
				pic = csPicByHandle(atoi(CG_Argv(1)));
				if(pic) {
					Q_strncpyz(pic->text,CG_Argv(2),sizeof(pic->text));
				}
			}
			else {
				Com_Printf("usage: %s [picID] [text(max. %d chars atm)]\n",cmd,MAX_TEXT_PER_CSPICTURE-1);
			}

			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_setPicTextOffset")) {
			if(argc==4) {
				pic = csPicByHandle(atoi(CG_Argv(1)));
				if(pic) {
					pic->text_x = atof(CG_Argv(2));
					pic->text_y = atof(CG_Argv(3));
				}
			}
			else {
				Com_Printf("usage: %s [picID] [x] [y]\n",cmd);
			}

			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_setPicTextCharSize")) {
			if(argc==4) {
				pic = csPicByHandle(atoi(CG_Argv(1)));
				if(pic) {
					pic->text_cw = atof(CG_Argv(2));
					pic->text_ch = atof(CG_Argv(3));
				}
			}
			else {
				Com_Printf("usage: %s [picID] [w] [w]\n",cmd);
			}

			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_setPicTextColor")) {
			if(argc>=5) {
				pic = csPicByHandle(atoi(CG_Argv(1)));
				if(pic) {
					pic->textcolor[0] = atof(CG_Argv(2));
					pic->textcolor[1] = atof(CG_Argv(3));
					pic->textcolor[2] = atof(CG_Argv(4));
					if(argc>=6)
						pic->textcolor[3] = atof(CG_Argv(5));
				}
			}
			else {
				Com_Printf("usage: %s [picID] [r] [g] [b] ([a])\n",cmd);
			}
			return qtrue;
		}
	}

	// Transformations
	else if(!Q_stricmpn(cmd,"cs2d_trans",10)) {
		cutscene2dTransformation_t* tran = getFreeTransformation();

		if(!tran) {
			Com_Printf("[CutScene2D] Error: no free transformation-memory for %s\n",cmd);
			return qtrue;
		}
		tran->lastCalc = cg.time;

		if(!Q_stricmp(cmd,"cs2d_transMoveTo")) {
			if(argc==5) {
				tran->picture = atoi(CG_Argv(1));
				pic = csPicByHandle(tran->picture);
				if(pic) {
					int duration = atoi(CG_Argv(4));
					tran->type = C2DTT_MOVE;
					tran->v1 = (atof(CG_Argv(2))-pic->x)/(float)duration;
					tran->v2 = (atof(CG_Argv(3))-pic->y)/(float)duration;
					tran->timeout = cg.time+duration;
				}
			}
			else {
				Com_Printf("usage: %s [picID] [x] [y] [duration]\n",cmd);
			}

			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_transResizeRel")) {
			if(argc==5) {
				tran->picture = atoi(CG_Argv(1));
				pic = csPicByHandle(tran->picture);
				if(pic) {
					int duration = atoi(CG_Argv(4));
					tran->type = C2DTT_RESIZE;
					tran->v1 = ((atof(CG_Argv(2))*pic->w)-pic->w)/(float)duration;
					tran->v2 = ((atof(CG_Argv(3))*pic->h)-pic->h)/(float)duration;
					tran->timeout = cg.time+duration;
				}
			}
			else {
				Com_Printf("usage: %s [picID] [w_scale] [h_scale] [duration]\n",cmd);
			}

			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_transResize")) {
			if(argc==5) {
				tran->picture = atoi(CG_Argv(1));
				pic = csPicByHandle(tran->picture);
				if(pic) {
					int duration = atoi(CG_Argv(4));
					tran->type = C2DTT_RESIZE;
					tran->v1 = atof(CG_Argv(2))/(float)duration;
					tran->v2 = atof(CG_Argv(3))/(float)duration;
					tran->timeout = cg.time+duration;
				}
			}
			else {
				Com_Printf("usage: %s [picID] [dW] [dH] [duration]\n",cmd);
			}

			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_transFadeOut")) {
			if(argc==3) {
				tran->picture = atoi(CG_Argv(1));
				pic = csPicByHandle(tran->picture);
				if(pic) {
					int duration = atoi(CG_Argv(2));
					tran->type = C2DTT_FADEALPHA;
					tran->v1 = (-pic->color[3])/(float)duration;
					tran->v2 = (-pic->textcolor[3])/(float)duration;
					tran->timeout = cg.time+duration;
				}
			}
			else {
				Com_Printf("usage: %s [picID] [duration]\n",cmd);
			}
			return qtrue;
		}
		else if(!Q_stricmp(cmd,"cs2d_transFadeIn")) {
			if(argc==3) {
				tran->picture = atoi(CG_Argv(1));
				pic = csPicByHandle(tran->picture);
				if(pic) {
					int duration = atoi(CG_Argv(2));
					tran->type = C2DTT_FADEALPHA;
					pic->color[3] = 0.0f;
					pic->textcolor[3] = 0.0f;
					tran->v1 = (1.0f)/(float)duration;
					tran->v2 = (1.0f)/(float)duration;
					tran->timeout = cg.time+duration;

					pic->isShown = qtrue;
				}
			}
			else {
				Com_Printf("usage: %s [picID] [duration]\n",cmd);
			}
			return qtrue;
		}

	}

	return qfalse;
}


