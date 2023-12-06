/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of WorldOfPadman source code.

WorldOfPadman source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

WorldOfPadman source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with WorldOfPadman source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// advanced 2D functions by #@
// based on "first UI-only Version"

#include "cg_local.h"

#define DEG2RAD_FLOAT 0.017453292f // PI/180

static refdef_t refdef2D;
static qboolean initrefdef = qfalse;

/*
====================
CG_Initrefdef2D
====================
*/
static void CG_Initrefdef2D(void) {
	float x, y, w, h;

	memset(&refdef2D, 0, sizeof(refdef2D));
	refdef2D.rdflags = RDF_NOWORLDMODEL;
	// orig (0/0/0)
	refdef2D.vieworg[0] = SCREEN_WIDTH / 2.0f;
	refdef2D.vieworg[1] = SCREEN_HEIGHT / 2.0f;
	refdef2D.vieworg[2] = 1000.0f;
	refdef2D.viewaxis[0][0] = 0;
	refdef2D.viewaxis[0][1] = 0;
	refdef2D.viewaxis[0][2] = -1;
	refdef2D.viewaxis[1][0] = -1;
	refdef2D.viewaxis[1][1] = 0;
	refdef2D.viewaxis[1][2] = 0;
	refdef2D.viewaxis[2][0] = 0;
	refdef2D.viewaxis[2][1] = -1;
	refdef2D.viewaxis[2][2] = 0;
	x = 0;
	y = 0;
	w = SCREEN_WIDTH;
	h = SCREEN_HEIGHT;
	CG_AdjustFrom640(&x, &y, &w, &h);
	refdef2D.x = x;
	refdef2D.y = y;
	refdef2D.width = w;
	refdef2D.height = h;
	refdef2D.fov_x = RAD2DEG(2.0f * atan2(refdef2D.vieworg[0], refdef2D.vieworg[2]));
	refdef2D.fov_y = RAD2DEG(2.0f * atan2(refdef2D.vieworg[1], refdef2D.vieworg[2]));
	refdef2D.time = cg.time;

	initrefdef = qtrue;
}

/*
====================
CG_DrawPoly
====================
*/
static void CG_DrawPoly(const poly_t *poly) {
	if (!initrefdef)
		CG_Initrefdef2D();

	trap_R_ClearScene();

	trap_R_AddPolyToScene(poly->hShader, poly->numVerts, poly->verts);

	trap_R_RenderScene(&refdef2D);
}

/*
====================
CG_AdvancedDrawPicA
====================
*/
void CG_AdvancedDrawPicA(float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader,
						 vec4_t color, float angle, int turnorigin) {
	poly_t poly;
	polyVert_t verts[4];
	vec2_t vec_w, vec_h;

	memset(&poly, 0, sizeof(poly));
	memset(&verts, 0, sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 4;
	poly.hShader = hShader;

	verts[0].modulate[0] = verts[1].modulate[0] = verts[2].modulate[0] = verts[3].modulate[0] = 255 * color[0];

	verts[0].modulate[1] = verts[1].modulate[1] = verts[2].modulate[1] = verts[3].modulate[1] = 255 * color[1];

	verts[0].modulate[2] = verts[1].modulate[2] = verts[2].modulate[2] = verts[3].modulate[2] = 255 * color[2];

	verts[0].modulate[3] = verts[1].modulate[3] = verts[2].modulate[3] = verts[3].modulate[3] = 255 * color[3];

	angle *= DEG2RAD_FLOAT;

	vec_w[0] = cos(angle);
	vec_w[1] = -sin(angle);
	vec_h[0] = sin(angle);
	vec_h[1] = cos(angle);

	switch (turnorigin) {
	default:
	case TURNORIGIN_UPPERLEFT:
		verts[0].xyz[0] = x;
		verts[0].xyz[1] = y;

		verts[1].xyz[0] = x + vec_w[0] * w;
		verts[1].xyz[1] = y + vec_w[1] * w;

		verts[2].xyz[0] = x + vec_w[0] * w + vec_h[0] * h;
		verts[2].xyz[1] = y + vec_w[1] * w + vec_h[1] * h;

		verts[3].xyz[0] = x + vec_h[0] * h;
		verts[3].xyz[1] = y + vec_h[1] * h;
		break;
	case TURNORIGIN_MIDDLELEFT:
		verts[0].xyz[0] = x - vec_h[0] * h * 0.5f;
		verts[0].xyz[1] = y - vec_h[1] * h * 0.5f;

		verts[1].xyz[0] = x + vec_w[0] * w - vec_h[0] * h * 0.5f;
		verts[1].xyz[1] = y + vec_w[1] * w - vec_h[1] * h * 0.5f;

		verts[2].xyz[0] = x + vec_w[0] * w + vec_h[0] * h * 0.5f;
		verts[2].xyz[1] = y + vec_w[1] * w + vec_h[1] * h * 0.5f;

		verts[3].xyz[0] = x + vec_h[0] * h * 0.5f;
		verts[3].xyz[1] = y + vec_h[1] * h * 0.5f;
		break;
	case TURNORIGIN_MIDDLECENTER:
		verts[0].xyz[0] = x - vec_w[0] * w * 0.5f - vec_h[0] * h * 0.5f;
		verts[0].xyz[1] = y - vec_w[1] * w * 0.5f - vec_h[1] * h * 0.5f;

		verts[1].xyz[0] = x + vec_w[0] * w * 0.5f - vec_h[0] * h * 0.5f;
		verts[1].xyz[1] = y + vec_w[1] * w * 0.5f - vec_h[1] * h * 0.5f;

		verts[2].xyz[0] = x + vec_w[0] * w * 0.5f + vec_h[0] * h * 0.5f;
		verts[2].xyz[1] = y + vec_w[1] * w * 0.5f + vec_h[1] * h * 0.5f;

		verts[3].xyz[0] = x - vec_w[0] * w * 0.5f + vec_h[0] * h * 0.5f;
		verts[3].xyz[1] = y - vec_w[1] * w * 0.5f + vec_h[1] * h * 0.5f;
		break;
	}

	verts[0].st[0] = s1;
	verts[0].st[1] = t1;

	verts[1].st[0] = s2;
	verts[1].st[1] = t1;

	verts[2].st[0] = s2;
	verts[2].st[1] = t2;

	verts[3].st[0] = s1;
	verts[3].st[1] = t2;

	CG_DrawPoly(&poly);
}

/*
====================
CG_DrawLine
====================
*/
void CG_DrawLine(float x1, float y1, float x2, float y2, float size, vec4_t color) {
	poly_t poly;
	polyVert_t verts[4];
	vec2_t vec_w;
	float length_1;

	memset(&poly, 0, sizeof(poly));
	memset(&verts, 0, sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 4;
	poly.hShader = cgs.media.whiteShader;

	verts[0].modulate[0] = verts[1].modulate[0] = verts[2].modulate[0] = verts[3].modulate[0] = 255 * color[0];

	verts[0].modulate[1] = verts[1].modulate[1] = verts[2].modulate[1] = verts[3].modulate[1] = 255 * color[1];

	verts[0].modulate[2] = verts[1].modulate[2] = verts[2].modulate[2] = verts[3].modulate[2] = 255 * color[2];

	verts[0].modulate[3] = verts[1].modulate[3] = verts[2].modulate[3] = verts[3].modulate[3] = 255 * color[3];

	vec_w[0] = y1 - y2;
	vec_w[1] = x2 - x1;

	length_1 = 1.0f / sqrt(vec_w[0] * vec_w[0] + vec_w[1] * vec_w[1]);

	vec_w[0] *= length_1;
	vec_w[1] *= length_1;

	verts[0].xyz[0] = x1 - vec_w[0] * size * 0.5f;
	verts[0].xyz[1] = y1 - vec_w[1] * size * 0.5f;

	verts[1].xyz[0] = x2 - vec_w[0] * size * 0.5f;
	verts[1].xyz[1] = y2 - vec_w[1] * size * 0.5f;

	verts[2].xyz[0] = x2 + vec_w[0] * size * 0.5f;
	verts[2].xyz[1] = y2 + vec_w[1] * size * 0.5f;

	verts[3].xyz[0] = x1 + vec_w[0] * size * 0.5f;
	verts[3].xyz[1] = y1 + vec_w[1] * size * 0.5f;

	verts[0].st[0] = 0;
	verts[0].st[1] = 0;

	verts[1].st[0] = 1;
	verts[1].st[1] = 0;

	verts[2].st[0] = 1;
	verts[2].st[1] = 1;

	verts[3].st[0] = 0;
	verts[3].st[1] = 1;

	CG_DrawPoly(&poly);
}

/*
====================
CG_DrawPic2Color
====================
*/
void CG_DrawPic2Color(float x, float y, float w, float h, float s1, float t1, float s2, float t2, vec4_t color1,
					  vec4_t color2, qhandle_t shader) {
	poly_t poly;
	polyVert_t verts[4];

	memset(&poly, 0, sizeof(poly));
	memset(&verts, 0, sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 4;
	poly.hShader = shader;

	verts[0].modulate[0] = verts[3].modulate[0] = 255 * color1[0];
	verts[1].modulate[0] = verts[2].modulate[0] = 255 * color2[0];

	verts[0].modulate[1] = verts[3].modulate[1] = 255 * color1[1];
	verts[1].modulate[1] = verts[2].modulate[1] = 255 * color2[1];

	verts[0].modulate[2] = verts[3].modulate[2] = 255 * color1[2];
	verts[1].modulate[2] = verts[2].modulate[2] = 255 * color2[2];

	verts[0].modulate[3] = verts[3].modulate[3] = 255 * color1[3];
	verts[1].modulate[3] = verts[2].modulate[3] = 255 * color2[3];

	verts[0].xyz[0] = x;
	verts[0].xyz[1] = y;

	verts[1].xyz[0] = x + w;
	verts[1].xyz[1] = y;

	verts[2].xyz[0] = x + w;
	verts[2].xyz[1] = y + h;

	verts[3].xyz[0] = x;
	verts[3].xyz[1] = y + h;

	verts[0].st[0] = s1;
	verts[0].st[1] = t1;

	verts[1].st[0] = s2;
	verts[1].st[1] = t1;

	verts[2].st[0] = s2;
	verts[2].st[1] = t2;

	verts[3].st[0] = s1;
	verts[3].st[1] = t2;

	CG_DrawPoly(&poly);
}

/*
====================
CG_Draw4VertsPic
====================
*/
void CG_Draw4VertsPic(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, qhandle_t shader,
					  vec4_t color) {
	poly_t poly;
	polyVert_t verts[4];

	memset(&poly, 0, sizeof(poly));
	memset(&verts, 0, sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 4;
	poly.hShader = shader;

	//	Com_Printf("x1=%3.2f, y1=%3.2f, x2=%3.2f, y2=%3.2f\nx3=%3.2f, y3=%3.2f, x4=%3.2f, y4=%3.2f\n", x1, y1, x2, y2,
	// x3, y3, x4, y4);

	verts[0].modulate[0] = verts[1].modulate[0] = verts[2].modulate[0] = verts[3].modulate[0] = 255 * color[0];

	verts[0].modulate[1] = verts[1].modulate[1] = verts[2].modulate[1] = verts[3].modulate[1] = 255 * color[1];

	verts[0].modulate[2] = verts[1].modulate[2] = verts[2].modulate[2] = verts[3].modulate[2] = 255 * color[2];

	verts[0].modulate[3] = verts[1].modulate[3] = verts[2].modulate[3] = verts[3].modulate[3] = 255 * color[3];

	verts[0].xyz[0] = x1;
	verts[0].xyz[1] = y1;

	verts[1].xyz[0] = x2;
	verts[1].xyz[1] = y2;

	verts[2].xyz[0] = x3;
	verts[2].xyz[1] = y3;

	verts[3].xyz[0] = x4;
	verts[3].xyz[1] = y4;

	verts[0].st[0] = 0;
	verts[0].st[1] = 0;

	verts[1].st[0] = 1;
	verts[1].st[1] = 0;

	verts[2].st[0] = 1;
	verts[2].st[1] = 1;

	verts[3].st[0] = 1;
	verts[3].st[1] = 0;

	CG_DrawPoly(&poly);
}

/*
====================
CG_AddCharToScene
====================
*/
static void CG_AddCharToScene(float x, float y, int ch, vec4_t color, vec2_t vec_w, vec2_t vec_h) {
	poly_t poly;
	polyVert_t verts[4];
	//	vec2_t		vec_w,vec_h;
	int row, col;

	if (ch == ' ')
		return;

	memset(&poly, 0, sizeof(poly));
	memset(&verts, 0, sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 4;
	poly.hShader = cgs.media.charsetShader;

	verts[0].modulate[0] = verts[1].modulate[0] = verts[2].modulate[0] = verts[3].modulate[0] = 255 * color[0];

	verts[0].modulate[1] = verts[1].modulate[1] = verts[2].modulate[1] = verts[3].modulate[1] = 255 * color[1];

	verts[0].modulate[2] = verts[1].modulate[2] = verts[2].modulate[2] = verts[3].modulate[2] = 255 * color[2];

	verts[0].modulate[3] = verts[1].modulate[3] = verts[2].modulate[3] = verts[3].modulate[3] = 255 * color[3];

	verts[0].xyz[0] = x;
	verts[0].xyz[1] = y;

	verts[1].xyz[0] = x + vec_w[0];
	verts[1].xyz[1] = y + vec_w[1];

	verts[2].xyz[0] = x + vec_w[0] + vec_h[0];
	verts[2].xyz[1] = y + vec_w[1] + vec_h[1];

	verts[3].xyz[0] = x + vec_h[0];
	verts[3].xyz[1] = y + vec_h[1];

	// see CG_DrawChar
	row = ch >> 4;
	col = ch & 15;

	verts[0].st[0] = col * 0.0625f;
	verts[0].st[1] = row * 0.0625f;

	verts[1].st[0] = (col + 1) * 0.0625f;
	verts[1].st[1] = row * 0.0625f;

	verts[2].st[0] = (col + 1) * 0.0625f;
	verts[2].st[1] = (row + 1) * 0.0625f;

	verts[3].st[0] = col * 0.0625f;
	verts[3].st[1] = (row + 1) * 0.0625f;

	trap_R_AddPolyToScene(poly.hShader, poly.numVerts, poly.verts);
}

static int CG_ColorAndPosinc(const char *str, int *spos, float *tmpColor, qboolean forceColor) {
	int posInc = 1;
	int i = *spos;

	if (Q_IsColorString(str + i)) {
		if (!forceColor) {
			memcpy(tmpColor, g_color_table[ColorIndex(str[i + 1])], sizeof(vec3_t));
		}
		++(*spos);
		posInc = 0;
	}

	return posInc;
}

/*
====================
CG_DrawTurnableString
====================
*/
void CG_DrawTurnableString(float x, float y, const char *s, vec4_t color, float charHeight, float angle,
						   int turnorigin) {
	vec2_t vec_w, vec_h;
	int i, j, sLen;
	vec4_t tmpColor;
	qboolean forceColor = qfalse;

	memcpy(tmpColor, color, sizeof(vec4_t));

	if (!initrefdef)
		CG_Initrefdef2D();

	sLen = strlen(s);

	trap_R_ClearScene();

	angle *= DEG2RAD_FLOAT;

	vec_w[0] = cos(angle) * charHeight * 0.5f;
	vec_w[1] = -sin(angle) * charHeight * 0.5f;
	vec_h[0] = sin(angle) * charHeight;
	vec_h[1] = cos(angle) * charHeight;

	j = 0;
	switch (turnorigin) {
	default:
	case TURNORIGIN_UPPERLEFT:
		for (i = 0; i < sLen; i++) {
			if (CG_ColorAndPosinc(s, &i, tmpColor, forceColor)) {
				CG_AddCharToScene(x + j * vec_w[0], y + j * vec_w[1], s[i], tmpColor, vec_w, vec_h);
				++j;
			}
		}
		break;
	case TURNORIGIN_MIDDLELEFT:
		for (i = 0; i < sLen; i++) {
			if (CG_ColorAndPosinc(s, &i, tmpColor, forceColor)) {
				CG_AddCharToScene(x + j * vec_w[0] - vec_h[0] * 0.5f, y + j * vec_w[1] - vec_h[1] * 0.5f, s[i],
								  tmpColor, vec_w, vec_h);
				++j;
			}
		}
		break;
	case TURNORIGIN_MIDDLECENTER:
		for (i = 0; i < sLen; i++) {
			if (CG_ColorAndPosinc(s, &i, tmpColor, forceColor)) {
				CG_AddCharToScene(x + ((float)j - (float)sLen * 0.5f) * vec_w[0] - vec_h[0] * 0.5f,
								  y + ((float)j - (float)sLen * 0.5f) * vec_w[1] - vec_h[1] * 0.5f, s[i], tmpColor,
								  vec_w, vec_h);
				++j;
			}
		}
		break;
	}

	trap_R_RenderScene(&refdef2D);
}

#define CSIZE 0.0625f
static void CG_DrawCharWithCutFrame(float x, float y, char ch, float w, float h, float fl, float ft, float fr,
									float fb) {
	float lcut, tcut, rcut, bcut;
	int row, col;
	float s1, t1, s2, t2;

	if (x > fr || y > fb || x + w < fl || y + h < ft)
		return; // not in the frame

	lcut = (fl > x) ? ((fl - x) / w) : 0.0f;
	rcut = (fr < x + w) ? (((x + w) - fr) / w) : 0.0f;
	tcut = (ft > y) ? ((ft - y) / h) : 0.0f;
	bcut = (fb < y + h) ? (((y + h) - fb) / h) : 0.0f;

	// see CG_DrawChar
	row = ch >> 4;
	col = ch & 15;
	s1 = (col * CSIZE);
	t1 = (row * CSIZE);
	s2 = ((col + 1) * CSIZE);
	t2 = ((row + 1) * CSIZE);

	s1 += (lcut * CSIZE);
	t1 += (tcut * CSIZE);
	s2 -= (rcut * CSIZE);
	t2 -= (bcut * CSIZE);

	CG_AdjustFrom640(&x, &y, &w, &h);
	trap_R_DrawStretchPic(x + lcut * w, y + tcut * h, w * (1.0f - lcut - rcut), h * (1.0f - tcut - bcut), s1, t1, s2,
						  t2, cgs.media.charsetShader);
}

/**
 * @param x leftedge on 640x480
 * @param y topedge on 640x480
 * @param str String that should be drawn
 * @param cW width of one letter
 * @param cH height of one letter
 * @param fl frame leftedge
 * @param ft frame topedge
 * @param fr frame rightedge
 * @param fb frame bottomedge
 */
void CG_DrawStringWithCutFrame(float x, float y, const char *str, vec4_t color, float cW, float cH, float fl, float ft,
							   float fr, float fb) {
	qboolean forceColor = qfalse;
	int i, sLen;
	float j = 0.0f;
	vec4_t tmpColor;

	if (str == NULL)
		return;

	sLen = strlen(str);

	trap_R_SetColor(color);
	j = 0;
	for (i = 0; i < sLen; ++i) {
		if (Q_IsColorString(str + i)) {
			if (!forceColor) {
				memcpy(tmpColor, g_color_table[ColorIndex(str[i + 1])], sizeof(tmpColor));
				tmpColor[3] = color[3];
				trap_R_SetColor(tmpColor);
			}
			++i;
			continue;
		}

		CG_DrawCharWithCutFrame(x + j, y, str[i], cW, cH, fl, ft, fr, fb);
		j += cW;
	}
	trap_R_SetColor(NULL);
}
