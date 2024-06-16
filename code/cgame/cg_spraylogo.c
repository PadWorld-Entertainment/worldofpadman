/*
####################### ####################### ####################### ####################### #######################
	spray logo system coded by #@(aka Raute)

	this file contains the spraylogo system(for spray your logo mode)
	... It's the 2nd version
####################### ####################### ####################### ####################### #######################
*/

#include "cg_local.h"

// maybe I should put this in the cg_local.h ... but's not necessary, because the typedefs and the saved-stuff is only
// used in here loading-stuff
#define MAX_LOADEDLOGOS 64

typedef struct {
	char name[MAX_SPRAYLOGO_NAME];		  // the name of the logo (filename ;) ... I hope this is enough
	qhandle_t logohandle; // the qhandle of the logo shader
} loadedlogo_t;

static loadedlogo_t loadedlogos_array[MAX_LOADEDLOGOS];
static int loadedlogos;

static sfxHandle_t menuClickSound;

// drawing-stuff
#define MAX_LOGO_POLYS 256 // like marks

typedef struct logoPoly_s {
	struct logoPoly_s *prevPoly, *nextPoly;
	int spraytime;
	qhandle_t logoShader; // there is a handle in the poly
	float color[4];		  // spraycolor for calc fadeout
	//	poly_t		poly;//hShader | numVerts | *verts ... we don't need another verts-ptr (we already have the array)
	int numVerts;
	polyVert_t verts[MAX_VERTS_ON_POLY]; // xyz | st[2] | modulate[4]

	// vv anti-z-fighting
	vec3_t center;
	float radius;
	int level;
} logoPoly_t;

static logoPoly_t *freeLogoPolys; // last freepoly ... we add and take from the end of the list
static logoPoly_t
	*drawLogoPolys; // first poly in the drawlist ... for drawing & freeing polys if we run out of free polys (it's
					// better to clean a old one without fadeout than spraying without drawing ;)
static logoPoly_t *lastdrawLogoPolys; // the last in drawlist ... for adding new polys
static logoPoly_t logoPolys[MAX_LOGO_POLYS];

// cvar:"logolist"
// vv INIT-stuff vv

static int PP2ID(logoPoly_t *p) {
	return (p) ? p - logoPolys : -1;
}

void CG_DumpPolyInfo(void) {
	int i;
	logoPoly_t *p;

	Com_Printf("\n\n spray logo poly dump, time: %.1f \n*********\npoly array\n\n", (float)(cg.time / 1000.f));

	for (i = 0; i < MAX_LOGO_POLYS; i++) {
		p = &logoPolys[i];
		Com_Printf("%4d time %5.1f shader %8d radius %6.1f level %3d\n", i, (float)(p->spraytime / 1000.f),
				   p->logoShader, p->radius, p->level);
	}

	Com_Printf("\n");
	Com_Printf("free head: %d\n", PP2ID(freeLogoPolys));
	Com_Printf("inuse head: %d\n", PP2ID(drawLogoPolys));
	Com_Printf("inuse tail: %d\n", PP2ID(lastdrawLogoPolys));

	Com_Printf("\n\n inuse list \n**********\n");
	p = drawLogoPolys;
	while (p) {
		Com_Printf("time %5.1f id %4d next %4d prev %4d\n", (float)(p->spraytime / 1000.f), PP2ID(p),
				   PP2ID(p->nextPoly), PP2ID(p->prevPoly));
		p = p->nextPoly;
	}

	Com_Printf("\n\n free list \n***********\n");
	p = freeLogoPolys;
	while (p) {
		Com_Printf("id%4d next %4d prev %4d \n", PP2ID(p), PP2ID(p->nextPoly), PP2ID(p->prevPoly));
		p = p->prevPoly;
	}
}

/*
#######################
Init_LogoPolyList

This will clean all logopolys and put them into the freelist
#######################
*/
static void Init_LogoPolyList(void) {
	int i;

	// clean =)
	memset(&logoPolys, 0, sizeof(logoPolys));

	drawLogoPolys = NULL;
	lastdrawLogoPolys = NULL;

	// put all in a list
	for (i = 0; i < MAX_LOGO_POLYS; i++) {
		logoPolys[i].prevPoly = i != 0 ? &logoPolys[i - 1] : NULL;
		logoPolys[i].nextPoly = i != (MAX_LOGO_POLYS - 1) ? &logoPolys[i + 1] : NULL;
	}

	freeLogoPolys = &logoPolys[i - 1];
}

/*
#######################
Sort_Logos

This will sort the Logo list (using some Q-Sort code)
#######################
*/
static void Sort_Logos(loadedlogo_t arr[], int lidx, int ridx) {
	loadedlogo_t buffer;
	int e, k, mid;

	if (lidx >= ridx) // Array hat weniger als zwei Elemente
		return;

	mid = (lidx + ridx) / 2; // mittleres Element ist Trennwert.

	memcpy(&buffer, &arr[lidx], sizeof(loadedlogo_t));
	memcpy(&arr[lidx], &arr[mid], sizeof(loadedlogo_t));
	memcpy(&arr[mid], &buffer, sizeof(loadedlogo_t));

	e = lidx;

	for (k = lidx + 1; k <= ridx; k++)
		if (Q_stricmp(arr[k].name, arr[lidx].name) < 0) {
			e++;
			memcpy(&buffer, &arr[e], sizeof(loadedlogo_t));
			memcpy(&arr[e], &arr[k], sizeof(loadedlogo_t));
			memcpy(&arr[k], &buffer, sizeof(loadedlogo_t));
		}

	memcpy(&buffer, &arr[lidx], sizeof(loadedlogo_t));
	memcpy(&arr[lidx], &arr[e], sizeof(loadedlogo_t));
	memcpy(&arr[e], &buffer, sizeof(loadedlogo_t));

	Sort_Logos(arr, lidx, e - 1);
	Sort_Logos(arr, e + 1, ridx);
}

/*
#######################
Load_Logos

This function loads all logos which were found with the ui-function(ui_gameinfo.c)
#######################
*/
static void Load_Logos(void) {
	int i, logosfound;
	char *logonamelist[MAX_LOADEDLOGOS];
	char logonamestr[1024];
	char *tmpchrptr;

	// get the filelist (created in the ui ... ui_gameinfo.c)
	trap_Cvar_VariableStringBuffer("logolist", logonamestr, 1024);

	// scan liststring for logonames
	if (logonamestr[0] != '\0') {
		logonamelist[0] = &logonamestr[0];
		logosfound = 1;
		for (i = 0; i < MAX_LOADEDLOGOS; i++) {
			tmpchrptr = strchr(logonamelist[i], '\\');

			if (tmpchrptr == NULL)
				break;

			*tmpchrptr = '\0';
			logosfound++;
			logonamelist[i + 1] = (char *)(tmpchrptr + 1);
		}
	} else
		logosfound = 0;

	// register default logo (this will be displayed if we can't find the logo which the server wants to use ... this
	// should only happen if we are unpure)
	cgs.media.defaultspraylogo = trap_R_RegisterShader("spraylogos/01_wop");

	// set back the loadedcounter
	loadedlogos = 0;

	// clean the array ... isn't really necessary
	memset(&loadedlogos_array, 0, sizeof(loadedlogos_array));

	// register all found logos and save handle+name in the loadedarray
	if (logosfound > MAX_LOADEDLOGOS)
		logosfound = MAX_LOADEDLOGOS;
	for (i = 0; i < logosfound; i++) {
		Com_sprintf(loadedlogos_array[i].name, sizeof(loadedlogos_array[i].name), "%s", logonamelist[i]);
		loadedlogos_array[i].logohandle = trap_R_RegisterShader(va("spraylogos/%s", logonamelist[i]));
		// if we get a 0-handle we use the default logo ... this will happen if we want to use an unpure file on a pure
		// server
		if (!loadedlogos_array[i].logohandle)
			loadedlogos_array[i].logohandle = cgs.media.defaultspraylogo;
		loadedlogos++; // loaded should become found ;)
	}

	Sort_Logos(loadedlogos_array, 0, logosfound - 1);
}

/*
#######################
Init_SprayLogoSys

The main-init for the spraylogosys ... should be called at every vid_restart
#######################
*/
void Init_SprayLogoSys(void) {
	cgs.media.spraypuff = trap_R_RegisterShader("models/weaponsfx/spraypuff");
	cgs.media.spraymark = trap_R_RegisterShader("gfx/damage/spray_mrk");
	cgs.media.arrowlt0 = trap_R_RegisterShaderNoMip("menu/arrows/arrblu_lt0b");
	cgs.media.arrowlt1 = trap_R_RegisterShaderNoMip("menu/arrows/arrblu_lt1");
	cgs.media.arrowrt0 = trap_R_RegisterShaderNoMip("menu/arrows/arrblu_rt0b");
	cgs.media.arrowrt1 = trap_R_RegisterShaderNoMip("menu/arrows/arrblu_rt1");
	cgs.media.cgwopmenu_cursor = trap_R_RegisterShaderNoMip("menu/art/cursor");
	if (cgs.gametype == GT_SPRAY) {
		cgs.media.chooselogo_bg = trap_R_RegisterShaderNoMip("menu/bg/selectlogo_team");
	} else {
		cgs.media.chooselogo_bg = trap_R_RegisterShaderNoMip("menu/bg/selectlogo_ffa");
	}

	Load_Logos();
	Init_LogoPolyList();

	menuClickSound = trap_S_RegisterSound("sound/feedback/menu/click", qfalse);
}

/*
#######################
Free_LogoPoly
#######################
*/
static void Free_LogoPoly(logoPoly_t *lp) {
	if (!lp) {
		Com_Printf(S_COLOR_RED "ERROR: Free_LogoPoly get a NULL pointer\n");
		return;
	}

	// check if this was the "drawLogoPolys" poly
	if (lp == drawLogoPolys)
		drawLogoPolys = lp->nextPoly;
	if (lp == lastdrawLogoPolys)
		lastdrawLogoPolys = lp->prevPoly;

	// take this poly out of the drawlist
	if (lp->prevPoly)
		lp->prevPoly->nextPoly = lp->nextPoly;
	if (lp->nextPoly)
		lp->nextPoly->prevPoly = lp->prevPoly;

	// clean and put into freelist
	memset(lp, 0, sizeof(logoPoly_t));
	lp->prevPoly = freeLogoPolys;
	if (freeLogoPolys)
		freeLogoPolys->nextPoly = lp;
	freeLogoPolys = lp;
}

/*
#######################
Alloc_LogoPoly

this function can return NULL if there is an big error !!!!! ... this will lead to a disconnect
TODO: write some more info ;)
#######################
*/
static logoPoly_t *Alloc_LogoPoly(void) {
	logoPoly_t *lp;

	if (freeLogoPolys) {
		lp = freeLogoPolys;
	} else // there is no free poly so we must free one
	{
		if (!drawLogoPolys) // this should NEVER happen ... so make a big error and add a disconnect cmd
		{
			Com_Printf(S_COLOR_RED "***********************************************\n"
					   S_COLOR_RED "* ERROR: no LogoPolys in draw and free !!!!!! *\n"
					   S_COLOR_RED "***********************************************\n");
			trap_SendConsoleCommand("disconnect\n");
			return (logoPoly_t *)(NULL);
		}
		lp = drawLogoPolys; // we take the oldest
		Free_LogoPoly(lp);	// the func will set drawLogoPolys to the next poly
	}
	freeLogoPolys = lp->prevPoly; // take it from the list
	if (freeLogoPolys)
		freeLogoPolys->nextPoly = NULL;

	// put the poly to the end of the list (nextPoly was NULL and should be NULL =)
	lp->prevPoly = lastdrawLogoPolys;
	if (lastdrawLogoPolys)
		lastdrawLogoPolys->nextPoly = lp;
	lastdrawLogoPolys = lp;

	if (!drawLogoPolys) // if there is nothing in the drawlist we must set this as the first item ... maybe I should
						// check if lastdrawLogoPolys was also NULL (if it wasn't we have an big error)
		drawLogoPolys = lp;

	return lp;
}

// I took this from the mark code(the defs)
// I think this is too much (but I will keep it ...) ... why is it too much: the save list has only 256 elements
// (so with a complex surface there is only space for 2 logos)
#define MAX_LOGO_FRAGMENTS 128
#define MAX_LOGO_POINTS 384

/*
#######################
Add_LogoToDrawList

origin -> center of the logo
dir -> a normal of the logo (pointing away from the wall)
shader -> the shader-handle so we know what to draw ;)
radius -> this will be a fixed value (but I don't want to fix it here)
color -> the color of the logo

FIXME: Radius is not fixed at all!
#######################
*/
void Add_LogoToDrawList(const vec3_t origin, const vec3_t dir, qhandle_t shader, float radius, const vec4_t color) {
	vec3_t logoPoints[4]; // the 4 points of the unsplited logo
	vec3_t projection;	  // a vec for projecting the logo at the wall
	int numFragments;
	markFragment_t logoFragments[MAX_LOGO_FRAGMENTS];
	vec3_t logoFragmentPoints[MAX_LOGO_POINTS];
	vec3_t axis[3];
	int i, j;
	float texCoordScale;
	logoPoly_t *tmplp;
	int level;

	VectorNormalize2(dir, axis[0]);

	if (axis[0][2] == 1.0f) {
		axis[1][0] = 1.0f;
		axis[1][1] = 0.0f;
		axis[1][2] = 0.0f;

		axis[2][0] = 0.0f;
		axis[2][1] = -1.0f;
		axis[2][2] = 0.0f;
	} else if (axis[0][2] == -1.0f) {
		axis[1][0] = 1.0f;
		axis[1][1] = 0.0f;
		axis[1][2] = 0.0f;

		axis[2][0] = 0.0f;
		axis[2][1] = 1.0f;
		axis[2][2] = 0.0f;
	} else {
		axis[1][0] = axis[0][1];
		axis[1][1] = -axis[0][0];
		axis[1][2] = 0;
		VectorNormalize(axis[1]);

		axis[2][0] = axis[1][1] * axis[0][2] - axis[1][2] * axis[0][1];
		axis[2][1] = axis[1][2] * axis[0][0] - axis[1][0] * axis[0][2];
		axis[2][2] = axis[1][0] * axis[0][1] - axis[1][1] * axis[0][0];
		VectorNormalize(axis[2]);

		// turn 180 degree ...
		axis[1][0] *= -1.0f;
		axis[1][1] *= -1.0f;
		axis[1][2] *= -1.0f;

		axis[2][0] *= -1.0f;
		axis[2][1] *= -1.0f;
		axis[2][2] *= -1.0f;
	}

	// unwind the loop
	logoPoints[0][0] = origin[0] - radius * axis[1][0] - radius * axis[2][0];
	logoPoints[0][1] = origin[1] - radius * axis[1][1] - radius * axis[2][1];
	logoPoints[0][2] = origin[2] - radius * axis[1][2] - radius * axis[2][2];

	logoPoints[1][0] = origin[0] + radius * axis[1][0] - radius * axis[2][0];
	logoPoints[1][1] = origin[1] + radius * axis[1][1] - radius * axis[2][1];
	logoPoints[1][2] = origin[2] + radius * axis[1][2] - radius * axis[2][2];

	logoPoints[2][0] = origin[0] + radius * axis[1][0] + radius * axis[2][0];
	logoPoints[2][1] = origin[1] + radius * axis[1][1] + radius * axis[2][1];
	logoPoints[2][2] = origin[2] + radius * axis[1][2] + radius * axis[2][2];

	logoPoints[3][0] = origin[0] - radius * axis[1][0] + radius * axis[2][0];
	logoPoints[3][1] = origin[1] - radius * axis[1][1] + radius * axis[2][1];
	logoPoints[3][2] = origin[2] - radius * axis[1][2] + radius * axis[2][2];

	// in mark code the projection is only 20 units
	projection[0] = dir[0] * (-16);
	projection[1] = dir[1] * (-16);
	projection[2] = dir[2] * (-16);
	texCoordScale = 0.5f * 1.0f / radius;

	tmplp = drawLogoPolys;
	level = 1;
	while (tmplp) {
		if (tmplp->level >= level && ((tmplp->center[0] - origin[0]) * (tmplp->center[0] - origin[0]) +
									  (tmplp->center[1] - origin[1]) * (tmplp->center[1] - origin[1]) +
									  (tmplp->center[2] - origin[2]) * (tmplp->center[2] - origin[2]))
										 < (radius + tmplp->radius) * (radius + tmplp->radius) * 2) {
			level = tmplp->level + 1;
		}

		tmplp = tmplp->nextPoly;
	}

	numFragments = trap_CM_MarkFragments(4, (void *)logoPoints, projection, MAX_LOGO_POINTS, logoFragmentPoints[0],
										 MAX_LOGO_FRAGMENTS, logoFragments);

	for (i = 0; i < numFragments; i++) {
		markFragment_t *lf = &logoFragments[i];
		logoPoly_t *lp = Alloc_LogoPoly();
		if (!lp)
			return; // alloc had a big error ...

		// check the verts limit
		if (lf->numPoints > MAX_VERTS_ON_POLY) {
			lf->numPoints = MAX_VERTS_ON_POLY;
		}

		VectorCopy(origin, lp->center);
		lp->radius = radius;
		lp->level = level;

		for (j = 0; j < lf->numPoints; j++) {
			vec3_t delta;

			polyVert_t *v = &lp->verts[j];

			// calculate the texturecoordinates (took this code from mark func)
			// copy origin and move a bit away from the wall (because we can't use polygonoffset in the shader)
			v->xyz[0] = logoFragmentPoints[lf->firstPoint + j][0] + dir[0] * (0.1f + 0.01f * (float)(level));
			v->xyz[1] = logoFragmentPoints[lf->firstPoint + j][1] + dir[1] * (0.1f + 0.01f * (float)(level));
			v->xyz[2] = logoFragmentPoints[lf->firstPoint + j][2] + dir[2] * (0.1f + 0.01f * (float)(level));

			VectorSubtract(v->xyz, origin, delta);
			v->st[0] = 0.5f + DotProduct(delta, axis[1]) * texCoordScale;
			v->st[1] = 0.5f + DotProduct(delta, axis[2]) * texCoordScale;
			v->st[0] += level * 10.0f; // TODO ... level-info into the engine ^^

			v->modulate[0] = (byte)(color[0] * 255.0f);
			v->modulate[1] = (byte)(color[1] * 255.0f);
			v->modulate[2] = (byte)(color[2] * 255.0f);
			v->modulate[3] = (byte)(color[3] * 255.0f);
		}

		lp->spraytime = cg.time;
		lp->logoShader = shader;
		lp->numVerts = lf->numPoints;
		Vector4Copy(color, lp->color);
	}
}

#define LOGOFADEOUT_DONOTHING 90000 // 180000
#define LOGOFADEOUT_FINISH 120000	// 240000

/*
#######################
AddLogosToScene
#######################
*/
void AddLogosToScene(void) {
	logoPoly_t *lp, *tmplp;
	float fadeout;
	int i;

	lp = drawLogoPolys;
	// tmplp=NULL;

	while (lp) // if lp is a NULL-pointer all drawlist-items are done
	{
		// fadeout
		if (cg.time < lp->spraytime + LOGOFADEOUT_DONOTHING)
			fadeout = 1.0f;
		else if (cg.time < lp->spraytime + LOGOFADEOUT_FINISH)
			fadeout = 1.0f - (float)((cg.time - lp->spraytime) - LOGOFADEOUT_DONOTHING) /
								 (float)(LOGOFADEOUT_FINISH - LOGOFADEOUT_DONOTHING);
		else {
			tmplp = lp;
			lp = lp->nextPoly;
			Free_LogoPoly(tmplp);
			continue;
		}

		// maybe I will fade color as well
		for (i = 0; i < lp->numVerts; i++) {
			lp->verts[i].modulate[0] = (byte)(lp->color[0] * fadeout * 255.0f);
			lp->verts[i].modulate[1] = (byte)(lp->color[1] * fadeout * 255.0f);
			lp->verts[i].modulate[2] = (byte)(lp->color[2] * fadeout * 255.0f);
			lp->verts[i].modulate[3] = (byte)(lp->color[3] * fadeout * 255.0f);
		}

		// add to scene ;)
		trap_R_AddPolyToScene(lp->logoShader, lp->numVerts, lp->verts);

		// tmplp=lp;
		lp = lp->nextPoly;
	}
	// if (tmplp != lastdrawLogoPolys) { Com_Printf(S_COLOR_RED "last drawn Logo wasn't lastdrawLogoPolys-ptr\n"); }
}

static qboolean CursorInBox(int x, int y, int w, int h) {
	if (cgs.cursorX >= x && cgs.cursorX <= x + w && cgs.cursorY >= y && cgs.cursorY <= y + h)
		return qtrue;
	return qfalse;
}

#define POSY_FIRSTLINE 100
#define POSY_SECONDLINE 200 // 260

#define LOGOSIZE 64							   // old: 128
#define GAP 32								   // old:16
#define XLL (SCREEN_CENTER - GAP * 3 / 2 - 2 * LOGOSIZE) // x left logo XD

#define ARROWLTX (SCREEN_CENTER - 58)
#define ARROWRTX (SCREEN_CENTER + 8)
#define ARROWY (POSY_SECONDLINE + LOGOSIZE + 40)

#define COLORSIZE 16
#define CGAP 4

static int activepage = 0;

static const char *SkipLogoNumber(const char *logoName) {
	const char *p = logoName;
	while (*p != '\0') {
		const char c = *p;
		if (c == '_')
			return p + 1;
		++p;
	}
	return logoName;
}

/*
#######################
ActiveChooseLogoMenu

Why here and not in UI ??? ;(
#######################
*/
void ActiveChooseLogoMenu(void) {
	vec4_t tblack33 = {0.0f, 0.0f, 0.0f, 0.33f};
	int i, j, catcher, CursorAtLogo, numPages;
	char tmpstr[256];
	static int lastklicktime = 0;
	static int lastpagetime = 0;

	char colorchar[16] = "0";
	float *spraycolor;
	int mouseOverColor = -1;

	trap_Cvar_Set("cl_paused", "1");

	if (cgs.gametype == GT_SPRAY) {
		if (cgs.clientinfo[cg.clientNum].team == TEAM_RED)
			spraycolor = colorRed;
		else
			spraycolor = colorBlue;
	} else {
		trap_Cvar_VariableStringBuffer("spraycolor", colorchar, sizeof(colorchar));

		if (colorchar[0] > COLOR_WHITE || colorchar[0] < COLOR_BLACK)
			colorchar[0] = COLOR_BLACK;

		spraycolor = g_color_table[ColorIndex(colorchar[0])];
	}

	catcher = trap_Key_GetCatcher();
	if (!(catcher & KEYCATCH_CGAME))
		trap_Key_SetCatcher(catcher | KEYCATCH_CGAME);

	numPages = (int)ceil((float)loadedlogos * 0.125f); // 1/8->0.125

	CG_SetScreenPlacement(PLACE_CENTER, PLACE_CENTER);

	if (cgs.media.chooselogo_bg == 0) {
		CG_FillRect(XLL - GAP, 60, 640 - 2 * (XLL - GAP), 320, tblack33);
		CG_DrawRect(XLL - GAP, 60, 640 - 2 * (XLL - GAP), 320, 1, colorBlack);
	} else {
		if (cgs.gametype == GT_SPRAY) {
			CG_DrawPic(XLL - GAP - 10, 60 - 10, 640 - 2 * (XLL - GAP) + 20, 320 - 15, cgs.media.chooselogo_bg);
		} else {
			CG_DrawPic(XLL - GAP - 10, 60 - 10, 640 - 2 * (XLL - GAP) + 20, 320 + 20, cgs.media.chooselogo_bg);
		}
	}

	CG_DrawStringExt(XLL, POSY_FIRSTLINE - 26, "Please select a spray logo...", colorWhite, qtrue, qtrue, FONT_SMALL, 64);

	CursorAtLogo = -1;
	for (i = 0, j = activepage * 8; i < 4; i++, j++) {
		char logoName[MAX_SPRAYLOGO_NAME];
		// first line
		if (j >= loadedlogos)
			break;
		trap_R_SetColor(spraycolor);
		CG_DrawPic(XLL + i * (LOGOSIZE + GAP), POSY_FIRSTLINE, LOGOSIZE, LOGOSIZE, loadedlogos_array[j].logohandle);
		trap_R_SetColor(NULL);
		Q_strncpyz(logoName, SkipLogoNumber(loadedlogos_array[j].name), sizeof(logoName));
		logoName[0] = toupper(logoName[0]);
		CG_DrawStringExt(XLL + i * (LOGOSIZE + GAP) + 32 - CG_DrawStrlen(logoName) * 4,
						 POSY_FIRSTLINE + LOGOSIZE + 10, logoName, colorWhite, qtrue, qtrue, FONT_SMALL,
						 32);
		if (CursorInBox(XLL + i * (LOGOSIZE + GAP), POSY_FIRSTLINE, LOGOSIZE, LOGOSIZE + 26))
			CursorAtLogo = j;

		// second line
		if (j + 4 >= loadedlogos)
			continue;
		trap_R_SetColor(spraycolor);
		CG_DrawPic(XLL + i * (LOGOSIZE + GAP), POSY_SECONDLINE, LOGOSIZE, LOGOSIZE,
				   loadedlogos_array[(j + 4)].logohandle);
		trap_R_SetColor(NULL);
		Q_strncpyz(logoName, SkipLogoNumber(loadedlogos_array[(j + 4)].name), sizeof(logoName));
		logoName[0] = toupper(logoName[0]);
		CG_DrawStringExt(XLL + i * (LOGOSIZE + GAP) + 32 - CG_DrawStrlen(logoName) * 4,
						 POSY_SECONDLINE + LOGOSIZE + 10, logoName, colorWhite, qtrue, qtrue, FONT_SMALL, 32);
		if (CursorInBox(XLL + i * (LOGOSIZE + GAP), POSY_SECONDLINE, LOGOSIZE, LOGOSIZE + 26))
			CursorAtLogo = (j + 4);
	}

	if (CursorAtLogo != -1)
		CG_DrawRect(XLL + (CursorAtLogo % 4) * (LOGOSIZE + GAP),
					((CursorAtLogo % 8) / 4) ? POSY_SECONDLINE : POSY_FIRSTLINE, LOGOSIZE, LOGOSIZE, 1, colorLtGrey);

	// draw left arrow
	if (activepage <= 0) {
		trap_R_SetColor(colorLtGrey);
		CG_DrawPic(ARROWLTX, ARROWY, 50, 22, cgs.media.arrowlt0);
		trap_R_SetColor(NULL);
	} else {
		if (cgs.cursorX > ARROWLTX && cgs.cursorX < (ARROWLTX + 50) && cgs.cursorY > ARROWY && cgs.cursorY < (ARROWY + 22)) {
			CG_DrawPic(ARROWLTX, ARROWY, 50, 22, cgs.media.arrowlt1);
		} else {
			CG_DrawPic(ARROWLTX, ARROWY, 50, 22, cgs.media.arrowlt0);
		}
	}

	// draw right arrow
	if (activepage >= (numPages - 1)) {
		trap_R_SetColor(colorLtGrey);
		CG_DrawPic(ARROWRTX, ARROWY, 50, 22, cgs.media.arrowrt0);
		trap_R_SetColor(NULL);
	} else {
		if (cgs.cursorX > ARROWRTX && cgs.cursorX < (ARROWRTX + 50) && cgs.cursorY > ARROWY && cgs.cursorY < (ARROWY + 22)) {
			CG_DrawPic(ARROWRTX, ARROWY, 50, 22, cgs.media.arrowrt1);
		} else {
			CG_DrawPic(ARROWRTX, ARROWY, 50, 22, cgs.media.arrowrt0);
		}
	}

	if (cgs.gametype == GT_SPRAYFFA) {
		int x = XLL;
		int y = ARROWY + 56;

		CG_DrawStringExt(x, y + COLORSIZE / 2 - 8, "...or change the color:", colorWhite, qtrue, qtrue, FONT_SMALL, -1);

		x += 200;
		for (i = 0; i < NUM_COLORS; ++i) {
			CG_FillRect(x + (COLORSIZE + CGAP) * i, y, COLORSIZE, COLORSIZE, g_color_table[i]);
			if (CursorInBox(x + (COLORSIZE + CGAP) * i, y, COLORSIZE, COLORSIZE)) {
				CG_FillRect(x + (COLORSIZE + CGAP) * i, y, COLORSIZE, COLORSIZE, colorTBlack33);
				mouseOverColor = i;
			}
		}
	}

	CG_DrawPic(cgs.cursorX - 16, cgs.cursorY - 16, 32, 32, cgs.media.cgwopmenu_cursor);

	if (cgs.lastusedkey == K_MOUSE1 && lastklicktime + 500 < cg.millis) {
		if (CursorAtLogo != -1) {
			trap_S_StartLocalSound(menuClickSound, CHAN_LOCAL_SOUND);

			trap_Cvar_Set("syc_logo", loadedlogos_array[CursorAtLogo].name);
			trap_SendClientCommand(va("selectlogo \"%s\"\n", loadedlogos_array[CursorAtLogo].name));
			trap_Key_SetCatcher(catcher & ~KEYCATCH_CGAME);
			cg.wantSelectLogo = qfalse;
		} else if (mouseOverColor != -1) {
			trap_S_StartLocalSound(menuClickSound, CHAN_LOCAL_SOUND);

			trap_Cvar_Set("spraycolor", va("%d", mouseOverColor));
		} else {
			// left arrow
			if (cgs.cursorX > ARROWLTX && cgs.cursorX < (ARROWLTX + 50) && cgs.cursorY > ARROWY && cgs.cursorY < (ARROWY + 22)) {
				if (activepage > 0) {
					trap_S_StartLocalSound(menuClickSound, CHAN_LOCAL_SOUND);

					--activepage;
				}
			}
			// right arrow
			else if (cgs.cursorX > ARROWRTX && cgs.cursorX < (ARROWRTX + 50) && cgs.cursorY > ARROWY && cgs.cursorY < (ARROWY + 22)) {
				if (activepage < (numPages - 1)) {
					trap_S_StartLocalSound(menuClickSound, CHAN_LOCAL_SOUND);

					++activepage;
				}
			}
		}
		lastklicktime = cg.millis;
	}

	if (cgs.lastusedkey == K_ESCAPE) // don't set a logo, so we have a empty str
	{
		trap_Key_SetCatcher(catcher & ~KEYCATCH_CGAME);
		cg.wantSelectLogo = qfalse;
	} else if ((cgs.lastusedkey == K_PGUP || cgs.lastusedkey == K_MWHEELUP) && lastpagetime + 500 < cg.millis) {
		if (activepage > 0)
			activepage--;
		lastpagetime = cg.millis;
	} else if ((cgs.lastusedkey == K_PGDN || cgs.lastusedkey == K_MWHEELDOWN) && lastpagetime + 500 < cg.millis) {
		if (activepage < (numPages - 1))
			activepage++;
		lastpagetime = cg.millis;
	} else if (cgs.lastusedkey >= '1' && cgs.lastusedkey <= '8') // a num key ... cgame don't use K_CHAR_FLAG
	{
		i = (cgs.lastusedkey - '1') + activepage * 8;
		if (i < loadedlogos) {
			trap_Cvar_Set("syc_logo", loadedlogos_array[i].name);
			trap_SendClientCommand(va("selectlogo \"%s\"\n", loadedlogos_array[i].name));
			trap_Key_SetCatcher(catcher & ~KEYCATCH_CGAME);
			cg.wantSelectLogo = qfalse;
		}
	} else if (cgs.lastusedkey == K_F12) {
		trap_SendConsoleCommand("screenshot\n");
	}

	if (!cg.wantSelectLogo) {
		trap_Cvar_Set("cl_paused", "0");
	}

	cgs.lastusedkey = 0; // clean
}

qhandle_t FindLogoForSpraying(const clientInfo_t *ci) {
	int i;
	char tmpstr[MAX_SPRAYLOGO_NAME];

	Q_strncpyz(tmpstr, ci->spraylogo, sizeof(tmpstr));

	if (!*tmpstr)
		return cgs.media.defaultspraylogo;

	for (i = 0; i < loadedlogos; i++) {
		if (!strcmp(loadedlogos_array[i].name, tmpstr))
			return loadedlogos_array[i].logohandle;
	}

	return cgs.media.defaultspraylogo;
}
