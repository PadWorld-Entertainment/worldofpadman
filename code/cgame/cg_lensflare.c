/*
####################### ####################### ####################### ####################### #######################
	lens flare system coded by #@(aka Raute)

	this file contains the lensflare system
####################### ####################### ####################### ####################### #######################
*/

#include "cg_local.h"

typedef enum {
	SF_NO = 0, // SF=SpecialFlare
	SF_BEAM,
	SF_LINE,
	SF_OB, // ob=overbrighten
	SF_SUBLF
} specials_t;

typedef struct flare_s {
	struct flare_s *next;

	char shadername[128]; // we don't load the shader @ script scan
	qhandle_t shader;	  // if we need the lf I will load the shader
	specials_t special;
	float specialVar;
	float pos;
	vec4_t color;
	//(//noch einbinden)draw only in special distances (0->start fade, 1->end fade and start fulldraw, 2->end fulldraw
	//and start fade, 3->end fade)
	float radius;	  //(//noch einbinden)0->base radius, 1->radius over pos*dir, 2->distance(1/x²)
	vec4_t turnstyle; // 0->base-angle, 1->center(1.0f=wirklich zur mitte), 2->bei x bewegung, 3->bei y bewegung
} flare_t;

typedef struct lensflare_s {
	char lfname[128]; // reicht das??
	float viewsize;
	qboolean shadersloaded;

	flare_t *firstflare;
} lensflare_t;

#define MAX_FLARES 128
#define MAX_LENSFLARES 32

static flare_t flaremem[MAX_FLARES]; // wieviel brauch ich da??????????
static flare_t *freeflares;
static lensflare_t lfmem[MAX_LENSFLARES]; // wieviel brauch ich da??????????
static int numlfs;

typedef struct InfosForDrawing_s {
	int lensflare; // index in lfmem
	vec3_t origin; // origin ... the screen pos will be called @ drawing
	vec3_t dir;	   // dir ... the screen pos will be
	float alpha;
} InfosForDrawing_t;

#define MAX_SCREENLFS 16

static InfosForDrawing_t IFD_Array[MAX_SCREENLFS];
static int IFDA_firstempty;

// cvar:"lensflarelist"
// vv INIT-stuff vv
/*
#######################
LF_Parser

... reads a lf-script and puts the infos in the lf/flare-structs
#######################
*/
typedef enum { PL_BASE, PL_LF, PL_FLARE } parserlvl_t;

typedef enum {
	NT_NORMAL = 0,

	NT_VIEWSIZE,

	NT_RADIUS,
	NT_POS,
	NT_SHADER,
	NT_COLOR_R,
	NT_COLOR_G,
	NT_COLOR_B,
	NT_COLOR_A,
	NT_SPECIAL,
	NT_SPECIALVAR,
	NT_TURNSTYLE,
	NT_TURNVAR1,
	NT_TURNVAR2,
	NT_TURNVAR3,
	NT_SETSUBLFPTR
} next_tmpstr_t;

static parserlvl_t parserlvl;
static next_tmpstr_t next_tmpstr;
static int tmpstrl;
#define MAX_TMPSTRING 128
static char tmpString[MAX_TMPSTRING];
static char nextscript[128];

static void StrEndWork(void) {
	//	char	offsetstr[32];

	if (tmpstrl == 0) // tmpString[0]=='\0')
	{
		next_tmpstr = NT_NORMAL;
		return;
	}

	/*
		memset(&offsetstr,' ',sizeof(offsetstr));
		offsetstr[parserlvl*3]='\0';

		Com_Printf("%s\"%s\"\n",offsetstr,tmpString);
	*/
	switch (parserlvl) {
	case PL_BASE:
		//		Com_Printf("scriptname:\"%s\"\n",tmpString);
		strcpy(nextscript, tmpString);
		break;
	case PL_LF:
		switch (next_tmpstr) {
		case NT_NORMAL:
			if (!Q_stricmp(tmpString, "viewsize")) {
				next_tmpstr = NT_VIEWSIZE;
			}
			break;
		case NT_VIEWSIZE:
			lfmem[numlfs - 1].viewsize = atof(tmpString);

			next_tmpstr = NT_NORMAL;
			break;
			/* gibt's nicht mehr aber falls ich noch mal was vor die flares machen will, so als beispiel ;)
						if(!Q_stricmp(tmpString,"ob_shader"))
						{ next_tmpstr=NT_OB_SHADER; }
						else if(!Q_stricmp(tmpString,"ob_color"))
						{ next_tmpstr=NT_OB_COLOR_R; }
						else if(!Q_stricmp(tmpString,"ob_area"))
						{ next_tmpstr=NT_OB_AREA; }
						else if(!Q_stricmp(tmpString,"ob_size"))
						{ next_tmpstr=NT_OB_SIZE; }
						break;
					case NT_OB_SHADER:
						strcpy(lfmem[numlfs-1].ob_shadername,tmpString);

						next_tmpstr=NT_NORMAL;
						break;
					case NT_OB_COLOR_R:
						lfmem[numlfs-1].ob_color[0]=atof(tmpString);

						next_tmpstr=NT_OB_COLOR_G;
						break;
					case NT_OB_COLOR_G:
						lfmem[numlfs-1].ob_color[1]=atof(tmpString);

						next_tmpstr=NT_OB_COLOR_B;
						break;
					case NT_OB_COLOR_B:
						lfmem[numlfs-1].ob_color[2]=atof(tmpString);

						next_tmpstr=NT_OB_COLOR_A;
						break;
					case NT_OB_COLOR_A:
						lfmem[numlfs-1].ob_color[3]=atof(tmpString);

						next_tmpstr=NT_NORMAL;
						break;
					case NT_OB_AREA:
						lfmem[numlfs-1].ob_area=atof(tmpString);

						next_tmpstr=NT_NORMAL;
						break;
					case NT_OB_SIZE:
						lfmem[numlfs-1].ob_size=atof(tmpString);

						next_tmpstr=NT_NORMAL;
						break;
			*/
		default:
			break;
		}
		break;
	case PL_FLARE:
		switch (next_tmpstr) {
		case NT_NORMAL:
			if (!Q_stricmp(tmpString, "radius")) {
				next_tmpstr = NT_RADIUS;
			} else if (!Q_stricmp(tmpString, "pos")) {
				next_tmpstr = NT_POS;
			} else if (!Q_stricmp(tmpString, "shader")) {
				next_tmpstr = NT_SHADER;
			} else if (!Q_stricmp(tmpString, "color")) {
				next_tmpstr = NT_COLOR_R;
			} else if (!Q_stricmp(tmpString, "special")) {
				next_tmpstr = NT_SPECIAL;
			} else if (!Q_stricmp(tmpString, "turnstyle")) {
				next_tmpstr = NT_TURNSTYLE;
			}
			break;
		case NT_RADIUS:
			lfmem[numlfs - 1].firstflare->radius = atof(tmpString);

			next_tmpstr = NT_NORMAL;
			break;
		case NT_POS:
			lfmem[numlfs - 1].firstflare->pos = atof(tmpString);

			next_tmpstr = NT_NORMAL;
			break;
		case NT_SHADER:
			strcpy(lfmem[numlfs - 1].firstflare->shadername, tmpString);

			next_tmpstr = NT_NORMAL;
			break;
		case NT_COLOR_R:
			lfmem[numlfs - 1].firstflare->color[0] = atof(tmpString);

			next_tmpstr = NT_COLOR_G;
			break;
		case NT_COLOR_G:
			lfmem[numlfs - 1].firstflare->color[1] = atof(tmpString);

			next_tmpstr = NT_COLOR_B;
			break;
		case NT_COLOR_B:
			lfmem[numlfs - 1].firstflare->color[2] = atof(tmpString);

			next_tmpstr = NT_COLOR_A;
			break;
		case NT_COLOR_A:
			lfmem[numlfs - 1].firstflare->color[3] = atof(tmpString);

			next_tmpstr = NT_NORMAL;
			break;
		case NT_SPECIAL:
			if (!Q_stricmp(tmpString, "beam")) {
				lfmem[numlfs - 1].firstflare->special = SF_BEAM;

				next_tmpstr = NT_SPECIALVAR;
				break;
			} else if (!Q_stricmp(tmpString, "line")) {
				lfmem[numlfs - 1].firstflare->special = SF_LINE;

				next_tmpstr = NT_SPECIALVAR;
				break;
			} else if (!Q_stricmp(tmpString, "overbrighten")) {
				lfmem[numlfs - 1].firstflare->special = SF_OB;

				next_tmpstr = NT_SPECIALVAR;
				break;
			} else if (!Q_stricmp(tmpString, "sublf")) {
				lfmem[numlfs - 1].firstflare->special = SF_SUBLF;

				next_tmpstr = NT_SETSUBLFPTR;
				break;
			}

			next_tmpstr = NT_NORMAL;
			break;

		case NT_SPECIALVAR:
			lfmem[numlfs - 1].firstflare->specialVar = atof(tmpString);

			next_tmpstr = NT_NORMAL;
			break;

		case NT_TURNSTYLE:
			if (!Q_stricmp(tmpString, "center")) {
				lfmem[numlfs - 1].firstflare->turnstyle[1] = 1.0f;
				next_tmpstr = NT_NORMAL;
			} else {
				lfmem[numlfs - 1].firstflare->turnstyle[0] = atof(tmpString);
				next_tmpstr = NT_TURNVAR1;
			}
			break;
		case NT_TURNVAR1:
			lfmem[numlfs - 1].firstflare->turnstyle[1] = atof(tmpString);
			next_tmpstr = NT_TURNVAR2;
			break;
		case NT_TURNVAR2:
			lfmem[numlfs - 1].firstflare->turnstyle[2] = atof(tmpString);
			next_tmpstr = NT_TURNVAR3;
			break;
		case NT_TURNVAR3:
			lfmem[numlfs - 1].firstflare->turnstyle[3] = atof(tmpString);
			next_tmpstr = NT_NORMAL;
			break;
		case NT_SETSUBLFPTR: {
			int i;

			for (i = 0; i < MAX_LENSFLARES; i++) {
				if (!Q_stricmp(lfmem[i].lfname, tmpString)) {
					*((int *)(&lfmem[numlfs - 1].firstflare->specialVar)) = i;
					break;
				}
			}
			if (*((int *)(&lfmem[numlfs - 1].firstflare->specialVar)) == 0 && Q_stricmp(tmpString, "default"))
				Com_Printf("Can't find sublf(\"%s\") for %s\n", tmpString, lfmem[numlfs - 1].lfname);
			next_tmpstr = NT_NORMAL;
		} break;
		default:
			break;
		}
		break;
	default:
		Com_Printf("^1error in lensflare-script: wrong parserlvl (wrong '{' or '}')\n");
		break;
	}

	tmpstrl = 0;
	tmpString[0] = '\0';
}

#define MAX_LFSCRIPTSIZE 8192 //-> like arenas-file loader
void LF_Parser(const char *scriptname) {
	fileHandle_t f;
	int i;
	qboolean inString;
	char buffer[MAX_LFSCRIPTSIZE];

	memset(&buffer, 0, sizeof(buffer));

	trap_FS_FOpenFile(va("scripts/%s.lensflare", scriptname), &f, FS_READ);

	trap_FS_Read(buffer, sizeof(buffer), f);

	trap_FS_FCloseFile(f);

	Com_Printf("parsing \"scripts/%s.lensflare\"\n", scriptname);

	next_tmpstr = NT_NORMAL;
	inString = qfalse;
	parserlvl = PL_BASE;
	tmpstrl = 0;
	tmpString[0] = '\0';
	for (i = 0; i < MAX_LFSCRIPTSIZE; i++) {
		if (buffer[i] == '\0')
			break;

		if (buffer[i] == '/') {
			if (buffer[i + 1] == '/') {
				if (inString) {
					StrEndWork();
					inString = qfalse;
				}
				for (; buffer[i] != '\n'; i++)
					;
				continue;
			} else if (buffer[i + 1] == '*') {
				if (inString) {
					StrEndWork();
					inString = qfalse;
				}
				for (; buffer[i] != '*' && buffer[i + 1] != '/'; i++)
					;
				continue;
			}
		}

		if (buffer[i] <= 0x20) {
			if (inString) {
				StrEndWork();
				inString = qfalse;
			}
			if (buffer[i] == '\0')
				break;
			else
				continue;
		}

		if (buffer[i] == '{') {
			flare_t *tmpflare;

			if (inString) {
				StrEndWork();
				inString = qfalse;
			}

			parserlvl++;

			switch (parserlvl) {
			case PL_LF:
				// alloc a new lf-struct and set a pointer ...
				if (numlfs + 1 >= MAX_LENSFLARES) {
					Com_Printf("^1can't load lensflare-script: no struct free (MAX=%i)\n", MAX_LENSFLARES);
					return;
				}

				strcpy(lfmem[numlfs].lfname, nextscript);
				lfmem[numlfs].viewsize = 1.0f;
				numlfs++;
				break;
			case PL_FLARE:
				// alloc a new flare-struct and set the firstflare-pointer ...
				if (!freeflares) {
					Com_Printf("^1can't finish load lensflare-script(\"%s\"): no flare-struct free (MAX=%i)\n",
							   lfmem[numlfs - 1].lfname, MAX_FLARES);
					return;
				}

				tmpflare = freeflares;
				freeflares = tmpflare->next;

				tmpflare->next = lfmem[numlfs - 1].firstflare;
				lfmem[numlfs - 1].firstflare = tmpflare;
				break;
			default:
				break;
			}
		} else if (buffer[i] == '}') {
			if (inString) {
				StrEndWork();
				inString = qfalse;
			}

			parserlvl--;
		} else {
			if (!inString && buffer[i] == '\"') {
				i++;
				for (; buffer[i] != '\"'; i++) {
					if (buffer[i] == '\0')
						break;

					if (tmpstrl < MAX_TMPSTRING - 2) {
						tmpString[tmpstrl] = buffer[i];
						tmpString[++tmpstrl] = '\0';
					} else
						Com_Printf("^2tmpString is full (this may happen if you use very long names, >%i chars) ... if "
								   "this leads to errors, short the names or/and send me a mail at raute_at@gmx.de\n",
								   (MAX_TMPSTRING - 2));
				}

				StrEndWork();
			} else {
				inString = qtrue;

				if (tmpstrl < MAX_TMPSTRING - 2) {
					tmpString[tmpstrl] = buffer[i];
					tmpString[++tmpstrl] = '\0';
				} else
					Com_Printf("^2tmpString is full (this may happen if you use very long names, >%i chars) ... if "
							   "this leads to errors, short the names or/and send me a mail at raute_at@gmx.de\n",
							   (MAX_TMPSTRING - 2));
			}
		}
	}
	Com_Printf("finished parsing \"scripts/%s.lensflare\"\n", scriptname);
}

/*
#######################
Load_LFfiles

this function will search lensflare-scripts in "lensflarelist" and call the parser
#######################
*/
#define MAX_LFFILES 40 // I hope this is enough
void Load_LFfiles(void) {
	int i, scriptsfound;
	char *scriptnamelist[MAX_LFFILES];
	char scriptnamestr[1024];
	char *tmpchrptr;

	// get the filelist (created in the ui ... ui_gameinfo.c)
	trap_Cvar_VariableStringBuffer("lensflarelist", scriptnamestr, 1024);

	// scan liststring for scriptnames
	if (scriptnamestr[0] != '\0') {
		scriptnamelist[0] = &scriptnamestr[0];
		scriptsfound = 1;
		for (i = 0; i < MAX_LFFILES; i++) {
			tmpchrptr = strchr(scriptnamelist[i], '\\');

			if (tmpchrptr == NULL)
				break;

			*tmpchrptr = '\0';
			scriptsfound++;
			scriptnamelist[i + 1] = (char *)(tmpchrptr + 1);
		}
	} else
		scriptsfound = 0;

	for (i = 0; i < scriptsfound; i++) {
		LF_Parser(scriptnamelist[i]);
	}
}

/*
#######################
Init_LensFlareSys

The main-init for the lensflaresys ... should be called at every vid_restart
#######################
*/
void Init_LensFlareSys(void) {
	int i;
	char tmpstr[256];
	const char *info;

	info = CG_ConfigString(CS_SERVERINFO);

	memset(&flaremem, 0, sizeof(flaremem));
	memset(&lfmem, 0, sizeof(lfmem));

	for (i = 1; i < MAX_FLARES; i++) {
		flaremem[i].next = &flaremem[i - 1];
	}
	freeflares = &flaremem[MAX_FLARES - 1];

	Q_strncpyz(tmpstr, cg_skyLensflare.string, sizeof(tmpstr));
	if (tmpstr[0] != '\0') {
		sscanf(tmpstr, "%f %f %f", &cg.skylensflare_dir[0], &cg.skylensflare_dir[1], &cg.skylensflare_dir[2]);
		strcpy(cg.skylensflare, (char *)(strchr(tmpstr, '>') + 1));
	} else
		cg.skylensflare[0] = '\0';

	// default lf vv
	strcpy(lfmem[0].lfname, "default");
	lfmem[0].firstflare = freeflares;
	freeflares = lfmem[0].firstflare->next->next->next->next;
	lfmem[0].firstflare->next->next->next->next = NULL;
	lfmem[0].firstflare->radius = 5;
	lfmem[0].firstflare->color[0] = 1.0f;
	lfmem[0].firstflare->color[1] = 1.0f;
	lfmem[0].firstflare->color[2] = 0.0f;
	lfmem[0].firstflare->color[3] = 0.8f;
	lfmem[0].firstflare->shader = cgs.media.whiteShader;
	lfmem[0].firstflare->pos = 0.2f;
	lfmem[0].firstflare->next->radius = 10;
	lfmem[0].firstflare->next->color[0] = 0.0f;
	lfmem[0].firstflare->next->color[1] = 0.33f;
	lfmem[0].firstflare->next->color[2] = 1.0f;
	lfmem[0].firstflare->next->color[3] = 0.33f;
	lfmem[0].firstflare->next->shader = cgs.media.whiteShader;
	lfmem[0].firstflare->next->pos = -0.3f;
	lfmem[0].firstflare->next->next->radius = 4;
	lfmem[0].firstflare->next->next->color[0] = 1.0f;
	lfmem[0].firstflare->next->next->color[1] = 0.33f;
	lfmem[0].firstflare->next->next->color[2] = 0.0f;
	lfmem[0].firstflare->next->next->color[3] = 0.66f;
	lfmem[0].firstflare->next->next->shader = cgs.media.whiteShader;
	lfmem[0].firstflare->next->next->pos = 1.0f;
	lfmem[0].firstflare->next->next->next->radius = 15;
	lfmem[0].firstflare->next->next->next->color[0] = 0.75f;
	lfmem[0].firstflare->next->next->next->color[1] = 0.0f;
	lfmem[0].firstflare->next->next->next->color[2] = 0.0f;
	lfmem[0].firstflare->next->next->next->color[3] = 0.33f;
	lfmem[0].firstflare->next->next->next->shader = cgs.media.whiteShader;
	lfmem[0].firstflare->next->next->next->pos = 2.0f;
	lfmem[0].shadersloaded = qtrue;
	// default lf ^^
	numlfs = 1;

	Load_LFfiles();
}
// loading all lensflare-scripts (filelist by ui)
// VV ... VV //get a info from the server which flares will be used(I don't know how I will make this exactly ...
// especially if I use ent-flares) ... I think I will inform me of the fixed lens and load the ent-lens depending on the
// registlist VV ... VV //loading all shaders needed for the lensflares I think I will load the shaders at the first use
// (sprites should not take that much loading time) ...
//*oh* and don't forget to save the scripts in vars (maybe only the needed ones) ??? öh? what ;)
// make a list of all lensflares in this map (name, origin/dir, dirbool) ... this list is only for fixed lensflares ...
// maybe we only need to do this vor skylfs
// ^^ INIT-stuff ^^
// vv ACTIVE-stuff vv
/*
#######################
AddLFToDrawList

TODO: write some info ;)
#######################
*/
void AddLFToDrawList(const char *lfname, vec3_t origin, vec3_t dir) {
	int i;

	if (IFDA_firstempty == MAX_SCREENLFS)
		return; // noch ein meldung ausgeben?

	IFD_Array[IFDA_firstempty].dir[0] = dir[0];
	IFD_Array[IFDA_firstempty].dir[1] = dir[1];
	IFD_Array[IFDA_firstempty].dir[2] = dir[2];
	IFD_Array[IFDA_firstempty].origin[0] = origin[0];
	IFD_Array[IFDA_firstempty].origin[1] = origin[1];
	IFD_Array[IFDA_firstempty].origin[2] = origin[2];

	for (i = 0; i < MAX_LENSFLARES; i++) {
		if (!Q_stricmp(lfmem[i].lfname, lfname)) {
			IFD_Array[IFDA_firstempty].lensflare = i;
			break;
		}
	}

	IFDA_firstempty++;
}

/*
#######################
Calculate_2DdirOf3D

A function for calculating the 2D-dir(center of the screen is 0/0) of a 3D-Dot/Dir ... base on my first lensflare
src(for padmod) returns alpha-float, if the lensflare can't be seen it is 0.0f point is the 3d origin of the dot dir is
the direktion of the lf-source refdef is the refdef for the drawn scene v2 is the 2D-vector from the screen-center to
the lf point
#######################
*/
static float Calculate_2DdirOf3D(vec3_t point, vec3_t dir, refdef_t *refdef, vec2_t v2, float *distanceSquared,
								 vec4_t xywh) {
	vec3_t vec;
	vec3_t axis[3];
	trace_t tr;

	// x,y of the center ... Width, Height ... all convertet to the 640x480-screen
	xywh[2] = 640.0f * (float)refdef->width / (float)cgs.glconfig.vidWidth;
	xywh[3] = 480.0f * (float)refdef->height / (float)cgs.glconfig.vidHeight;

	xywh[0] = 640.0f * (float)(refdef->x + refdef->width * 0.5f) / (float)cgs.glconfig.vidWidth;
	xywh[1] = 480.0f * (float)(refdef->y + refdef->height * 0.5f) / (float)cgs.glconfig.vidHeight;

	if (point[0] != 2300000.0f) // a small hack to mark only dirs (origins have to be between 65535 and -65535)
	{
		CG_Trace(&tr, refdef->vieworg, NULL, NULL, point, cg.snap->ps.clientNum, CONTENTS_SOLID);
		if (tr.fraction != 1.0f)
			return 0.0f;

		// make a vector from camera to dot
		vec[0] = point[0] - refdef->vieworg[0];
		vec[1] = point[1] - refdef->vieworg[1];
		vec[2] = point[2] - refdef->vieworg[2];

		*distanceSquared = VectorLengthSquared(vec);
	} else {
		vec3_t start;
		start[0] = refdef->vieworg[0];
		start[1] = refdef->vieworg[1];
		start[2] = refdef->vieworg[2];

		vec[0] = refdef->vieworg[0] + dir[0] * 200000.0f;
		vec[1] = refdef->vieworg[1] + dir[1] * 200000.0f;
		vec[2] = refdef->vieworg[2] + dir[2] * 200000.0f;

		do {
			CG_Trace(&tr, start, NULL, NULL, vec, cg.snap->ps.clientNum, CONTENTS_SOLID);
			if (tr.fraction == 1.0f)
				break;

			if (!tr.startsolid) {
				start[0] = tr.endpos[0] + dir[0] * 10.0f;
				start[1] = tr.endpos[1] + dir[1] * 10.0f;
				start[2] = tr.endpos[2] + dir[2] * 10.0f;
			} else {
				start[0] += dir[0] * 10.0f;
				start[1] += dir[1] * 10.0f;
				start[2] += dir[2] * 10.0f;
			}
		} while (tr.surfaceFlags & SURF_NODRAW || tr.contents & CONTENTS_TRANSLUCENT);

		if (!(tr.surfaceFlags & SURF_SKY))
			return 0.0f;

		vec[0] = dir[0];
		vec[1] = dir[1];
		vec[2] = dir[2];

		*distanceSquared = 14400000000.0f; // 120000²
	}

	// make a dotproduct to get a rough anglecheck ...
	if (((vec[0] * refdef->viewaxis[0][0] + vec[1] * refdef->viewaxis[0][1] + vec[2] * refdef->viewaxis[0][2]) <= 0))
		return 0.0f;

	// copy axis to get a short name ;)
	AxisCopy(refdef->viewaxis, axis);
	if (vec[0] != 0.0f) // the normal formula doesn't work with vec[0]==0 ...
		v2[0] = (((vec[0] * axis[0][2] - vec[2] * axis[0][0]) * (vec[1] * axis[2][0] - vec[0] * axis[2][1])) -
				 ((vec[0] * axis[0][1] - vec[1] * axis[0][0]) * (vec[2] * axis[2][0] - vec[0] * axis[2][2]))) /
				(((vec[0] * axis[1][1] - vec[1] * axis[1][0]) * (vec[2] * axis[2][0] - vec[0] * axis[2][2])) -
				 ((vec[0] * axis[1][2] - vec[2] * axis[1][0]) * (vec[1] * axis[2][0] - vec[0] * axis[2][1])));
	else // the formula is symmetric ... so I exchange [0] with [2] and [2] with [0]
		v2[0] = (((vec[2] * axis[0][0] - vec[0] * axis[0][2]) * (vec[1] * axis[2][2] - vec[2] * axis[2][1])) -
				 ((vec[2] * axis[0][1] - vec[1] * axis[0][2]) * (vec[0] * axis[2][2] - vec[2] * axis[2][0]))) /
				(((vec[2] * axis[1][1] - vec[1] * axis[1][2]) * (vec[0] * axis[2][2] - vec[2] * axis[2][0])) -
				 ((vec[2] * axis[1][0] - vec[0] * axis[1][2]) * (vec[1] * axis[2][2] - vec[2] * axis[2][1])));
	if (vec[1] * axis[2][0] - vec[0] * axis[2][1]) // don't diff with 0
		v2[1] = (vec[0] * axis[0][1] - vec[1] * axis[0][0] + v2[0] * (vec[0] * axis[1][1] - vec[1] * axis[1][0])) /
				(vec[1] * axis[2][0] - vec[0] * axis[2][1]);
	else // use a formular with an other "Definitionslücke"(definition-gap ?)
		v2[1] = (vec[0] * axis[0][2] - vec[2] * axis[0][0] + v2[0] * (vec[0] * axis[1][2] - vec[2] * axis[1][0])) /
				(vec[2] * axis[2][0] - vec[0] * axis[2][2]);

	v2[0] *= -1.0f; // turn around so we get a normal 2d system

	// wow the calc is very simple (and I don't need any fixed mult value =) ... but I must use Rad instead of Deg ;)
	// scale to render-size
#define DEG2RAD_FLOAT 0.017453292f // PI/180
	v2[0] *= ((float)(xywh[2]) * 0.5f) / tan(refdef->fov_x * 0.5f * DEG2RAD_FLOAT);
	v2[1] *= ((float)(xywh[3]) * 0.5f) / tan(refdef->fov_y * 0.5f * DEG2RAD_FLOAT);

	return 1.0f; // noch neuen code für den alpha-wert schreiben
}

/*
#######################
DrawLensflare

TODO: write some info ;)
#######################
*/
static void DrawLensflare(int lfid, vec2_t dir, float lfalpha, float distanceSquared, vec4_t xywh, qboolean sublf) {
	flare_t *tmpflare;
	vec2_t prozDir;
	float prozDirLenSquared;
	float invHalfWidth = 1 / (xywh[2] * 0.5f); // inv=inversed
	float invHalfHeight = 1 / (xywh[3] * 0.5f);
	float prozSize = xywh[3] / 480.0f;
	//	float	invprozSize=480.0f/xywh[3];

	float curRadius, curSpecialVar = 0;

	prozDir[0] = dir[0] * invHalfWidth;
	prozDir[1] = dir[1] * invHalfHeight;

	prozDirLenSquared = prozDir[0] * prozDir[0] + prozDir[1] * prozDir[1];

	// draw all flares of the lf
	for (tmpflare = lfmem[lfid].firstflare; tmpflare != NULL; tmpflare = tmpflare->next) {
		float tmpalpha, tmpalpha2;

		if (!lfmem[lfid].shadersloaded) {
			tmpflare->shader = trap_R_RegisterShaderNoMip(tmpflare->shadername);

			if (tmpflare->shader == 0)
				tmpflare->shader = cgs.media.whiteShader;
		}

		if (tmpflare->special == SF_OB && sublf)
			continue;

		if (!sublf) {
			if (tmpflare->special == SF_OB) {
				if ((tmpflare->specialVar * tmpflare->specialVar) < prozDirLenSquared)
					continue;
				else
					//				transparency=sqrt(tmpf)/tmpflare->specialVar;
					tmpalpha2 = lfalpha * (1.0f - prozDirLenSquared / (tmpflare->specialVar * tmpflare->specialVar));
			} else {
				if (1.0f < prozDirLenSquared / lfmem[lfid].viewsize)
					continue;
				else
					tmpalpha2 = lfalpha * (1.0f - prozDirLenSquared / lfmem[lfid].viewsize);
			}
		} else if (lfalpha < 0.0f)
			continue;
		else
			tmpalpha2 = lfalpha;

		tmpalpha = tmpflare->color[3];
		tmpflare->color[3] *= tmpalpha2;
		if (tmpflare->color[3] > 1.0f)
			tmpflare->color[3] = 1.0f;

		curRadius = tmpflare->radius * prozSize;
		if (tmpflare->special == SF_BEAM || tmpflare->special == SF_LINE)
			curSpecialVar = tmpflare->specialVar * prozSize;

		switch (tmpflare->special) {
		default:
			/*
						trap_R_SetColor(tmpflare->color);
						tmpflare->radius*=prozSize;
						trap_R_DrawStretchPic((320.0f+dir[0]*tmpflare->pos)-tmpflare->radius,(240.0f-dir[1]*tmpflare->pos)-tmpflare->radius,tmpflare->radius*2.0f,tmpflare->radius*2.0f,0,0,1,1,tmpflare->shader);
						tmpflare->radius*=invprozSize;
						trap_R_SetColor(NULL);
						break;
					case SF_TURNING:
			*/
			{
				float tmpf = 0;
				vec2_t tmpdir;

				if (tmpflare->turnstyle[1] != 0.0f) {
					tmpf = sqrt(dir[0] * dir[0] + dir[1] * dir[1]);

					if (tmpf) {
						tmpf = 1 / tmpf;
						tmpdir[0] = dir[0] * tmpf;
						tmpdir[1] = dir[1] * tmpf;

						//				tmpf=asin(tmpdir[0])*180.0f/M_PI;//q3 has no asin-table =(
						tmpf = 90.0f - (acos(tmpdir[0]) * 180.0f / M_PI);

						if (tmpdir[1] > 0)
							tmpf = 180.0f - tmpf;
					} else
						tmpf = 0.0f;

					tmpf *= tmpflare->turnstyle[1]; // noch mal überlegen ob das sinnmacht
				}

				tmpf += tmpflare->turnstyle[0] + (tmpflare->turnstyle[2] * prozDir[0] * tmpflare->pos) +
						(tmpflare->turnstyle[3] * prozDir[1] * tmpflare->pos);

				AdvancedDrawPicA((xywh[0] + dir[0] * tmpflare->pos), (xywh[1] - dir[1] * tmpflare->pos),
								 curRadius * 2.0f, curRadius * 2.0f, 0, 0, 1, 1, tmpflare->shader, tmpflare->color,
								 tmpf, 3); // TURNORIGIN_MIDDLECENTER=3
			}
			break;
		case SF_BEAM: {
			vec2_t qdir;
			vec2_t ldir;
			vec2_t rdir;

			qdir[0] = -dir[1] * curSpecialVar;
			qdir[1] = dir[0] * curSpecialVar;

			if (tmpflare->pos > 0 && curRadius > tmpflare->pos)
				curRadius = tmpflare->pos;
			else if (tmpflare->pos < 0 && curRadius > -tmpflare->pos)
				curRadius = -tmpflare->pos;

			ldir[0] = (dir[0] - qdir[0]) * curRadius;
			ldir[1] = (dir[1] - qdir[1]) * curRadius;

			rdir[0] = (dir[0] + qdir[0]) * curRadius;
			rdir[1] = (dir[1] + qdir[1]) * curRadius;

			qdir[0] *= tmpflare->pos;
			qdir[1] *= tmpflare->pos;

			Draw4VertsPic((xywh[0] + dir[0] * tmpflare->pos - qdir[0] + ldir[0]),
						  (xywh[1] - (dir[1] * tmpflare->pos - qdir[1] + ldir[1])),
						  (xywh[0] + dir[0] * tmpflare->pos - qdir[0] - ldir[0]),
						  (xywh[1] - (dir[1] * tmpflare->pos - qdir[1] - ldir[1])),
						  (xywh[0] + dir[0] * tmpflare->pos + qdir[0] - rdir[0]),
						  (xywh[1] - (dir[1] * tmpflare->pos + qdir[1] - rdir[1])),
						  (xywh[0] + dir[0] * tmpflare->pos + qdir[0] + rdir[0]),
						  (xywh[1] - (dir[1] * tmpflare->pos + qdir[1] + rdir[1])), tmpflare->shader, tmpflare->color);
		} break;
		case SF_LINE: {
			vec2_t qdir = {0.0f, 1.0f};
			vec2_t ndir = {1.0f, 0.0f};
			float tmpf = 0;

			if (tmpflare->turnstyle[1] != 0.0f) {
				/* old
									tmpf=sqrt(dir[0]*dir[0]+dir[1]*dir[1]);
									if(tmpf)
									{
										ndir[0]=dir[0]/tmpf;
										ndir[1]=dir[1]/tmpf;
									}
									else
									{
										ndir[0]=1;
										ndir[1]=0;
										//break;
									}
				*/
				tmpf = sqrt(dir[0] * dir[0] + dir[1] * dir[1]);

				if (tmpf) {
					ndir[0] = dir[0] / tmpf;
					ndir[1] = dir[1] / tmpf;

					tmpf = M_PI * 0.5f - acos(ndir[0]);

					if (ndir[1] > 0)
						tmpf = M_PI - tmpf;
				} else
					tmpf = 0.0f;

				tmpf *= tmpflare->turnstyle[1]; // noch mal überlegen ob das sinnmacht
			}

			tmpf += (tmpflare->turnstyle[0] + (tmpflare->turnstyle[2] * prozDir[0] * tmpflare->pos) +
					 (tmpflare->turnstyle[3] * prozDir[1] * tmpflare->pos)) /
					180.0f * M_PI;

			ndir[0] = sin(tmpf);
			ndir[1] = -cos(tmpf);

			qdir[0] = -ndir[1] * curSpecialVar;
			qdir[1] = ndir[0] * curSpecialVar;

			ndir[0] *= curRadius;
			ndir[1] *= curRadius;

			Draw4VertsPic((xywh[0] + dir[0] * tmpflare->pos - qdir[0] + ndir[0]),
						  (xywh[1] - (dir[1] * tmpflare->pos - qdir[1] + ndir[1])),
						  (xywh[0] + dir[0] * tmpflare->pos - qdir[0] - ndir[0]),
						  (xywh[1] - (dir[1] * tmpflare->pos - qdir[1] - ndir[1])),
						  (xywh[0] + dir[0] * tmpflare->pos + qdir[0] - ndir[0]),
						  (xywh[1] - (dir[1] * tmpflare->pos + qdir[1] - ndir[1])),
						  (xywh[0] + dir[0] * tmpflare->pos + qdir[0] + ndir[0]),
						  (xywh[1] - (dir[1] * tmpflare->pos + qdir[1] + ndir[1])), tmpflare->shader, tmpflare->color);
		} break;
		case SF_OB:
			// da fehlt das um skallieren -.-
			//			trap_R_SetColor(tmpflare->color);
			//			trap_R_DrawStretchPic(xywh[0]-(xywh[2]*0.5f*tmpflare->radius),xywh[1]-(xywh[3]*0.5f*tmpflare->radius),xywh[2]*tmpflare->radius,xywh[3]*tmpflare->radius,0,0,1,1,tmpflare->shader);
			//			trap_R_SetColor(NULL);

			AdvancedDrawPicA(xywh[0], xywh[1], 640.0f, 480.0f, 0, 0, 1, 1, tmpflare->shader, tmpflare->color, 0,
							 3); // TURNORIGIN_MIDDLECENTER=3
			break;
		case SF_SUBLF: {
			vec2_t subdir;
			vec4_t subxywh;
			float dirlen = 0, angle = 0;

			subxywh[0] = xywh[0] + dir[0] * tmpflare->pos;
			subxywh[1] = xywh[1] - dir[1] * tmpflare->pos;

			if (curRadius == 0.0f)
				curRadius = 1.0f;

			subxywh[2] = xywh[2]; //*curRadius;
			subxywh[3] = xywh[3]; //*curRadius;

			if (tmpflare->turnstyle[1] != 0.0f) {
				dirlen = sqrt(dir[0] * dir[0] + dir[1] * dir[1]);

				if (dirlen) {
					subdir[0] = dir[0] / dirlen;
					subdir[1] = dir[1] / dirlen;

					angle = M_PI * 0.5f - acos(subdir[0]);

					if (subdir[1] > 0)
						angle = M_PI - angle;
				} else
					angle = 0.0f;

				angle *= tmpflare->turnstyle[1]; // noch mal überlegen ob das sinnmacht
			}

			angle += (tmpflare->turnstyle[0] + (tmpflare->turnstyle[2] * prozDir[0] * tmpflare->pos) +
					  (tmpflare->turnstyle[3] * prozDir[1] * tmpflare->pos)) /
					 180.0f * M_PI;

			subdir[0] = sin(angle) * curRadius * dirlen * tmpflare->pos;
			subdir[1] = -cos(angle) * curRadius * dirlen * tmpflare->pos;

			if (*((int *)(&tmpflare->specialVar)) == lfid) {
				Com_Printf("ERROR: lensflare(%s) with the same lensflare as sublf!!!\n", lfmem[lfid].lfname);
				return;
			}

			DrawLensflare(*((int *)(&tmpflare->specialVar)), subdir, tmpflare->color[3], distanceSquared, subxywh,
						  qtrue);
		} break;
		}

		tmpflare->color[3] = tmpalpha;
	}

	lfmem[lfid].shadersloaded = qtrue;
}

/*
#######################
AddLFsToScreen

TODO: write some info ;)
#######################
*/
void AddLFsToScreen(void) {
	int i;
	vec2_t v2;
	float lfalpha;

	//	strcpy(cg.skylensflare,"testskylf");

	if (!cg_drawLensflare.integer) {
		return;
	}

	if (cg.skylensflare[0] != '\0') {
		vec3_t tmpv3 = {2300000.0f, 0, 0};
		vec3_t tmpv32;

		VectorNormalize(cg.skylensflare_dir);

		if (cg.wopSky[0] != '\0') {
			/*
						//0->hoch/runter, 1->rechts/links, 2->v^/^v(kamera um linsen-axe drehen)
						if(cg.skylensflare_dir[0]==1.0f)
						{ tmpv32[0]=tmpv32[1]=tmpv32[2]=0.0f; }
						else if(cg.skylensflare_dir[1]==1.0f)
						{ tmpv32[0]=tmpv32[2]=0.0f;tmpv32[1]=-90.0f; }
						else if(cg.skylensflare_dir[2]==1.0f)
						{ tmpv32[1]=tmpv32[2]=0.0f;tmpv32[0]=-90.0f; }
						else if(cg.skylensflare_dir[0]==-1.0f)
						{ tmpv32[0]=tmpv32[2]=0.0f;tmpv32[1]=180.0f; }
						else if(cg.skylensflare_dir[1]==-1.0f)
						{ tmpv32[0]=tmpv32[2]=0.0f;tmpv32[1]=90.0f; }
						else if(cg.skylensflare_dir[2]==-1.0f)
						{ tmpv32[1]=tmpv32[2]=0.0f;tmpv32[0]=90.0f; }
						else
						{
							tmpv32[2]=0.0f;
							tmpv32[0]=-90.0f+180.0f/M_PI*acos(cg.skylensflare_dir[2]/sqrt(cg.skylensflare_dir[0]*cg.skylensflare_dir[0]+cg.skylensflare_dir[2]*cg.skylensflare_dir[2]));
							tmpv32[1]=90.0f-180.0f/M_PI*acos(cg.skylensflare_dir[0]/sqrt(cg.skylensflare_dir[0]*cg.skylensflare_dir[0]+cg.skylensflare_dir[1]*cg.skylensflare_dir[1]));
						}

						tmpv32[0]+=cg.wopSky_Angles[0]*sin(cg.time*0.0001f*cg.wopSky_TimeFactors[0]);
						tmpv32[1]+=cg.wopSky_Angles[1]*sin(cg.time*0.0001f*cg.wopSky_TimeFactors[1]);
						tmpv32[2]+=cg.wopSky_Angles[2]*sin(cg.time*0.0001f*cg.wopSky_TimeFactors[2]);

						AngleVectors(tmpv32,tmpv32,NULL,NULL);
			*/
			vec3_t wopSkyAxis[3];

			tmpv32[0] = -cg.wopSky_Angles[0] * sin(cg.time * 0.0001f * cg.wopSky_TimeFactors[0]);
			tmpv32[1] = -cg.wopSky_Angles[1] * sin(cg.time * 0.0001f * cg.wopSky_TimeFactors[1]);
			tmpv32[2] = -cg.wopSky_Angles[2] * sin(cg.time * 0.0001f * cg.wopSky_TimeFactors[2]);
			AnglesToAxis(tmpv32, wopSkyAxis);

			VectorRotate(cg.skylensflare_dir, wopSkyAxis, tmpv32);
		} else {
			tmpv32[0] = cg.skylensflare_dir[0];
			tmpv32[1] = cg.skylensflare_dir[1];
			tmpv32[2] = cg.skylensflare_dir[2];
		}

		AddLFToDrawList(cg.skylensflare, tmpv3, tmpv32);
	}

	// Draw (with 3DTo2D-calc)
	for (i = 0; i < IFDA_firstempty; i++) {
		float distanceSquared;
		vec4_t xywh;

		lfalpha = Calculate_2DdirOf3D(IFD_Array[i].origin, IFD_Array[i].dir, &cg.refdef, v2, &distanceSquared, xywh);
		if (lfalpha == 0.0f)
			continue;
		DrawLensflare(IFD_Array[i].lensflare, v2, lfalpha, distanceSquared, xywh,
					  qfalse); // normal 480 ... außer wen das bild verkleinert wird
	}

	// del list
	memset(IFD_Array, 0, sizeof(IFD_Array));
	IFDA_firstempty = 0;
}
// if I want to use dynamic lensflares (for ents), I will have to create a lensflare list
// check if we see a lens ...
// draw the seen lensflares
// ^^ ACTIVE-stuff ^^
// vv SHUTDOWN-stuff vv
// don't know if I need to shutdown something
// ^^ SHUTDOWN-stuff ^^
