/* Static BSP world-geometry loader: own .bsp parse into one combined vertex+index
 * buffer, plus shader/blend classification, skybox capture, lightgrid sampling.
 * Every field byte-swapped since Espresso isn't little-endian -- this file alone
 * took more debugging than the rest of the renderer combined. */

#include <string.h>
#include <malloc.h>
#include <math.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "../qcommon/qfiles.h"
#include "../qcommon/surfaceflags.h"
#include "tr_gx2_local.h"

#include <gx2/mem.h>
#include <gx2/enum.h>

typedef struct {
	qhandle_t   texHandle;
	uint32_t    indexOffset; /* into s_indices, in elements */
	uint32_t    numIndexes;
	GX2Texture *lmTexture;   /* NULL = no lightmap (vertex-lit only) */
	int8_t      blendSrc;    /* GX2_BLEND_MODE_*; -1 = opaque */
	int8_t      blendDst;
	uint8_t     atestMode;   /* ubershader u_Mode.w selector, 0 = none */
} gx2worldsurf_t;

static gx2vert_t      *s_verts;
static uint32_t        s_numVerts;
static uint32_t       *s_indices;
static uint32_t        s_numIndexes;
static gx2worldsurf_t *s_surfaces;
static int             s_numSurfaces;
static int             s_numOpaqueSurfaces;
static qboolean        s_hasWorld = qfalse;

/* One 256-byte-aligned PS block row per surface -- hw enforces 0x100 alignment, no negotiating. */
static uint32_t       *s_surfPsBlocks;

static GX2Texture     **s_lightmaps;
static int              s_numLightmaps;

/* ---- skybox ----------------------------------------------------------- */

static qboolean   s_hasSky = qfalse;
static qhandle_t  s_skyFaceTex[6];  /* indexed rt,bk,lf,ft,up,dn */
static gx2vert_t *s_skyVerts;       /* memalign(256), 24 verts (4 per box axis) */

/* ---- lightgrid --------------------------------------------------------- */

static byte  *s_lightGridData;      /* numGridPoints * 8 bytes, color-shifted */
static vec3_t s_lightGridOrigin;
static vec3_t s_lightGridSize;
static vec3_t s_lightGridInverseSize;
static int    s_lightGridBounds[3];

void GX2World_Clear(void)
{
	int i;

	if (s_verts)        free(s_verts);
	if (s_indices)      free(s_indices);
	if (s_surfaces)     free(s_surfaces);
	if (s_surfPsBlocks) free(s_surfPsBlocks);
	if (s_skyVerts)     free(s_skyVerts);
	if (s_lightGridData) free(s_lightGridData);
	for (i = 0; i < s_numLightmaps; i++)
		GX2Image_FreeTexture(s_lightmaps[i]);
	if (s_lightmaps) free(s_lightmaps);
	s_verts = NULL;
	s_indices = NULL;
	s_surfaces = NULL;
	s_surfPsBlocks = NULL;
	s_skyVerts = NULL;
	s_lightGridData = NULL;
	s_lightmaps = NULL;
	s_numVerts = 0;
	s_numIndexes = 0;
	s_numSurfaces = 0;
	s_numOpaqueSurfaces = 0;
	s_numLightmaps = 0;
	s_hasWorld = qfalse;
	s_hasSky = qfalse;
	memset(s_skyFaceTex, 0, sizeof(s_skyFaceTex));
}

qboolean GX2World_HasWorld(void)
{
	return s_hasWorld;
}

gx2vert_t *GX2World_GetVertexBuffer(void)
{
	return s_verts;
}

uint32_t GX2World_GetNumVerts(void)
{
	return s_numVerts;
}

int GX2World_GetNumSurfaces(void)
{
	return s_numSurfaces;
}

int GX2World_GetNumOpaqueSurfaces(void)
{
	return s_numOpaqueSurfaces;
}

void GX2World_GetSurface(int surfNum, gx2worldsurfinfo_t *out)
{
	const gx2worldsurf_t *s = &s_surfaces[surfNum];
	out->texHandle = s->texHandle;
	out->indices = &s_indices[s->indexOffset];
	out->numIndexes = s->numIndexes;
	out->lmTexture = s->lmTexture;
	out->psBlock = s_surfPsBlocks ? &s_surfPsBlocks[(size_t)surfNum * 64] : NULL;
	out->blendSrc = s->blendSrc;
	out->blendDst = s->blendDst;
}

qboolean GX2World_HasSky(void)
{
	return s_hasSky;
}

const gx2vert_t *GX2World_GetSkyVerts(void)
{
	return s_skyVerts;
}

qhandle_t GX2World_GetSkyFaceTexture(int face)
{
	/* box axis order matching renderergl1's sky_texorder */
	static const int texorder[6] = { 0, 2, 1, 3, 4, 5 };

	if (face < 0 || face > 5)
		return GX2IMAGE_WHITE;
	return s_skyFaceTex[texorder[face]] ? s_skyFaceTex[texorder[face]] : GX2IMAGE_WHITE;
}

/* Port of R_ColorShiftLightingBytes: shift <<2, normalize instead of saturating.
 * <<1 left everything a full stop too dark -- found that the hard way. */
static void GX2World_ColorShiftBytes(byte *rgb)
{
	int r = (int)rgb[0] << 2;
	int g = (int)rgb[1] << 2;
	int b = (int)rgb[2] << 2;

	if ((r | g | b) > 255) {
		int max = r > g ? r : g;
		max = max > b ? max : b;
		r = r * 255 / max;
		g = g * 255 / max;
		b = b * 255 / max;
	}
	rgb[0] = (byte)r;
	rgb[1] = (byte)g;
	rgb[2] = (byte)b;
}

/* Uploads every LIGHTMAP_WIDTH x LIGHTMAP_HEIGHT RGB8 page as an RGBA8
 * GX2Texture. Missing/failed upload just leaves s_numLightmaps at 0. */
static void GX2World_LoadLightmaps(const byte *lmData, int lmLen)
{
	int count, i, y;
	byte rgba[LIGHTMAP_WIDTH * LIGHTMAP_HEIGHT * 4];

	count = lmLen / (LIGHTMAP_WIDTH * LIGHTMAP_HEIGHT * 3);
	if (count <= 0)
		return;

	s_lightmaps = (GX2Texture **)malloc(sizeof(GX2Texture *) * (size_t)count);
	if (!s_lightmaps) {
		ri.Printf(PRINT_ALL, "^1GX2World_LoadLightmaps: alloc failed\n");
		return;
	}

	for (i = 0; i < count; i++) {
		const byte *page = lmData + (size_t)i * LIGHTMAP_WIDTH * LIGHTMAP_HEIGHT * 3;

		for (y = 0; y < LIGHTMAP_HEIGHT; y++) {
			int x;
			for (x = 0; x < LIGHTMAP_WIDTH; x++) {
				const byte *src = page + (size_t)(y * LIGHTMAP_WIDTH + x) * 3;
				byte *dst = rgba + (size_t)(y * LIGHTMAP_WIDTH + x) * 4;
				dst[0] = src[0];
				dst[1] = src[1];
				dst[2] = src[2];
				dst[3] = 255;
				GX2World_ColorShiftBytes(dst);
			}
		}

		/* noMip: stock Q3 never mips lightmaps */
		s_lightmaps[s_numLightmaps] = GX2Image_Upload(rgba, LIGHTMAP_WIDTH, LIGHTMAP_HEIGHT, qtrue);
		if (!s_lightmaps[s_numLightmaps])
			ri.Printf(PRINT_ALL, "^1GX2World_LoadLightmaps: upload FAILED for page %d\n", i);
		s_numLightmaps++;
	}

	ri.Printf(PRINT_ALL, "GX2World_LoadLightmaps: %d lightmap page(s)\n", s_numLightmaps);
}

/* ---- lightgrid: port of R_LoadLightGrid + R_SetupEntityLightingGrid. ---- */

/* Reads worldspawn's optional "gridsize" key (default 64x64x128). */
static void GX2World_ParseGridSize(const byte *entData, int entLen)
{
	char *text, *p, *token;
	char keyname[MAX_TOKEN_CHARS];
	char value[MAX_TOKEN_CHARS];

	s_lightGridSize[0] = 64.0f;
	s_lightGridSize[1] = 64.0f;
	s_lightGridSize[2] = 128.0f;

	if (!entData || entLen <= 0)
		return;

	text = (char *)malloc((size_t)entLen + 1);
	if (!text)
		return;
	memcpy(text, entData, (size_t)entLen);
	text[entLen] = '\0';
	p = text;

	token = COM_ParseExt(&p, qtrue);
	if (token[0] == '{') {
		for (;;) {
			token = COM_ParseExt(&p, qtrue);
			if (!token[0] || token[0] == '}')
				break;
			Q_strncpyz(keyname, token, sizeof(keyname));

			token = COM_ParseExt(&p, qtrue);
			if (!token[0] || token[0] == '}')
				break;
			Q_strncpyz(value, token, sizeof(value));

			if (!Q_stricmp(keyname, "gridsize")) {
				sscanf(value, "%f %f %f", &s_lightGridSize[0],
				       &s_lightGridSize[1], &s_lightGridSize[2]);
			}
		}
	}

	free(text);
}

/* Grid data is bytes, no endian swap, only overbright shift. On mismatch
 * grid is disabled and entity lighting falls back to the flat default. */
static void GX2World_LoadLightGrid(const byte *gridData, int gridLen,
                                   const byte *modelData, int modelLen)
{
	const dmodel_t *world;
	vec3_t wMins, wMaxs, maxs;
	int i, numGridPoints;

	if (!gridData || gridLen <= 0)
		return;
	if (!modelData || modelLen < (int)sizeof(dmodel_t))
		return;

	world = (const dmodel_t *)modelData;
	for (i = 0; i < 3; i++) {
		wMins[i] = LittleFloat(world->mins[i]);
		wMaxs[i] = LittleFloat(world->maxs[i]);
		if (s_lightGridSize[i] < 1.0f)
			s_lightGridSize[i] = 1.0f;
		s_lightGridInverseSize[i] = 1.0f / s_lightGridSize[i];
	}

	for (i = 0; i < 3; i++) {
		s_lightGridOrigin[i] = s_lightGridSize[i] * ceilf(wMins[i] / s_lightGridSize[i]);
		maxs[i] = s_lightGridSize[i] * floorf(wMaxs[i] / s_lightGridSize[i]);
		s_lightGridBounds[i] = (int)((maxs[i] - s_lightGridOrigin[i]) / s_lightGridSize[i]) + 1;
		if (s_lightGridBounds[i] < 1) {
			ri.Printf(PRINT_ALL, "^3GX2World: degenerate light grid bounds, grid disabled\n");
			return;
		}
	}

	numGridPoints = s_lightGridBounds[0] * s_lightGridBounds[1] * s_lightGridBounds[2];
	if (gridLen != numGridPoints * 8) {
		ri.Printf(PRINT_ALL, "^3GX2World: light grid mismatch (%d != %d), grid disabled\n",
		          gridLen, numGridPoints * 8);
		return;
	}

	s_lightGridData = (byte *)malloc((size_t)gridLen);
	if (!s_lightGridData) {
		ri.Printf(PRINT_ALL, "^1GX2World: light grid alloc failed\n");
		return;
	}
	memcpy(s_lightGridData, gridData, (size_t)gridLen);

	for (i = 0; i < numGridPoints; i++) {
		GX2World_ColorShiftBytes(&s_lightGridData[i * 8]);     /* ambient  */
		GX2World_ColorShiftBytes(&s_lightGridData[i * 8 + 3]); /* directed */
	}

	ri.Printf(PRINT_ALL, "GX2World: light grid %dx%dx%d loaded\n",
	          s_lightGridBounds[0], s_lightGridBounds[1], s_lightGridBounds[2]);
}

/* Trilinear 8-corner sample, port of R_SetupEntityLightingGrid. Out-of-bounds/in-wall
 * corners skipped, factor renormalized. Min-light/clamping is the caller's job. */
qboolean GX2World_LightForPoint(const vec3_t point, vec3_t ambientLight,
                                vec3_t directedLight, vec3_t lightDir)
{
	vec3_t rel, direction;
	int    pos[3], gridStep[3];
	float  frac[3], totalFactor;
	int    i, j;
	const byte *gridBase;

	if (!s_lightGridData)
		return qfalse;

	VectorSubtract(point, s_lightGridOrigin, rel);
	for (i = 0; i < 3; i++) {
		float v = rel[i] * s_lightGridInverseSize[i];
		pos[i] = (int)floorf(v);
		frac[i] = v - (float)pos[i];
		if (pos[i] < 0)
			pos[i] = 0;
		else if (pos[i] > s_lightGridBounds[i] - 1)
			pos[i] = s_lightGridBounds[i] - 1;
	}

	VectorClear(ambientLight);
	VectorClear(directedLight);
	VectorClear(direction);

	gridStep[0] = 8;
	gridStep[1] = 8 * s_lightGridBounds[0];
	gridStep[2] = 8 * s_lightGridBounds[0] * s_lightGridBounds[1];
	gridBase = s_lightGridData + pos[0] * gridStep[0]
	         + pos[1] * gridStep[1] + pos[2] * gridStep[2];

	totalFactor = 0.0f;
	for (i = 0; i < 8; i++) {
		float       factor = 1.0f;
		const byte *data = gridBase;
		float       lat, lng;
		vec3_t      normal;

		for (j = 0; j < 3; j++) {
			if (i & (1 << j)) {
				if (pos[j] + 1 > s_lightGridBounds[j] - 1)
					break; /* ignore values outside lightgrid */
				factor *= frac[j];
				data += gridStep[j];
			} else {
				factor *= (1.0f - frac[j]);
			}
		}
		if (j != 3)
			continue;
		if (!(data[0] + data[1] + data[2]))
			continue; /* ignore samples in walls */

		totalFactor += factor;
		ambientLight[0] += factor * data[0];
		ambientLight[1] += factor * data[1];
		ambientLight[2] += factor * data[2];
		directedLight[0] += factor * data[3];
		directedLight[1] += factor * data[4];
		directedLight[2] += factor * data[5];

		/* byte angles, full circle = 256; direct sinf/cosf, 8 samples per call */
		lat = (float)data[7] * (2.0f * (float)M_PI / 256.0f);
		lng = (float)data[6] * (2.0f * (float)M_PI / 256.0f);
		normal[0] = cosf(lat) * sinf(lng);
		normal[1] = sinf(lat) * sinf(lng);
		normal[2] = cosf(lng);
		VectorMA(direction, factor, normal, direction);
	}

	if (totalFactor > 0.0f && totalFactor < 0.99f) {
		totalFactor = 1.0f / totalFactor;
		VectorScale(ambientLight, totalFactor, ambientLight);
		VectorScale(directedLight, totalFactor, directedLight);
	}

	VectorScale(ambientLight, 0.6f, ambientLight);  /* r_ambientScale default */

	VectorNormalize2(direction, lightDir);
	return qtrue;
}

/* ---- skybox ------------------------------------------------------------ */

/* Port of renderergl1's MakeSkyVec: s,t in -1..1 on box axis -> world
 * direction just inside the far plane. Texcoords mapped to 0..1, t flipped. */
static void GX2World_MakeSkyVec(float s, float t, int axis, float outSt[2], vec3_t outXYZ)
{
	/* 1=s, 2=t, 3=boxSize */
	static const int st_to_vec[6][3] = {
		{ 3, -1, 2 },
		{ -3, 1, 2 },

		{ 1, 3, 2 },
		{ -1, -3, 2 },

		{ -2, -1, 3 },  /* 0 degrees yaw, look straight up */
		{ 2, -1, -3 }   /* look straight down */
	};

	const float boxSize = 8192.0f / 1.75f;
	vec3_t b;
	int    j;

	b[0] = s * boxSize;
	b[1] = t * boxSize;
	b[2] = boxSize;

	for (j = 0; j < 3; j++) {
		int k = st_to_vec[axis][j];
		if (k < 0)
			outXYZ[j] = -b[-k - 1];
		else
			outXYZ[j] = b[k - 1];
	}

	s = (s + 1.0f) * 0.5f;
	t = (t + 1.0f) * 0.5f;
	if (s < 0.0f) s = 0.0f;
	else if (s > 1.0f) s = 1.0f;
	if (t < 0.0f) t = 0.0f;
	else if (t > 1.0f) t = 1.0f;
	t = 1.0f - t;

	outSt[0] = s;
	outSt[1] = t;
}

/* Called once for the first SURF_SKY surface: resolves skyparms outerbox to
 * 6 face textures and builds a 24-vertex camera-locked box. */
static void GX2World_SetupSky(const char *shaderName)
{
	static const char *suf[6] = { "rt", "bk", "lf", "ft", "up", "dn" };
	/* corner (s,t) pairs in GX2 QUADS perimeter order */
	static const float corners[4][2] = {
		{ -1.0f, -1.0f }, { 1.0f, -1.0f }, { 1.0f, 1.0f }, { -1.0f, 1.0f }
	};
	const gx2ShaderScript_t *script;
	char  faceName[MAX_QPATH];
	int   axis, c;

	script = GX2ShaderScript_Find(shaderName);
	if (!script || !script->isSky || !script->skyBox[0])
		return;

	s_skyVerts = (gx2vert_t *)memalign(256, sizeof(gx2vert_t) * 24);
	if (!s_skyVerts)
		return;

	for (axis = 0; axis < 6; axis++) {
		Com_sprintf(faceName, sizeof(faceName), "%s_%s", script->skyBox, suf[axis]);
		s_skyFaceTex[axis] = GX2Image_Register(faceName);
	}

	for (axis = 0; axis < 6; axis++) {
		for (c = 0; c < 4; c++) {
			gx2vert_t *ov = &s_skyVerts[axis * 4 + c];
			vec3_t xyz;
			float st[2];

			GX2World_MakeSkyVec(corners[c][0], corners[c][1], axis, st, xyz);
			ov->x = xyz[0];
			ov->y = xyz[1];
			ov->z = xyz[2];
			ov->s0 = st[0];
			ov->t0 = st[1];
			ov->s1 = 0.0f;
			ov->t1 = 0.0f;
			ov->rgba[0] = ov->rgba[1] = ov->rgba[2] = ov->rgba[3] = 255;
		}
	}

	GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER,
	              s_skyVerts, sizeof(gx2vert_t) * 24);

	s_hasSky = qtrue;
	ri.Printf(PRINT_ALL, "GX2World: skybox '%s' (%s)\n", script->skyBox, shaderName);
}

/* Fixed-subdivision port of R_SubdividePatchToGrid, LOD/stitching dropped. The ~190 KB
 * control grid is a file-scope static -- wut thread stacks are tiny, this blew up before. */

#define GX2_MAX_PATCH_SIZE 32   /* max control-point dims in the map file */
#define GX2_MAX_GRID_SIZE  65   /* max dims after subdivision */
#define GX2_PATCH_SUBDIVISIONS 4.0f /* r_subdivisions default */

static drawVert_t s_patchCtrl[GX2_MAX_GRID_SIZE][GX2_MAX_GRID_SIZE];
static float      s_patchError[2][GX2_MAX_GRID_SIZE];

/* Post-subdivision grids stashed between pass A (counting) and pass B (emission). */
typedef struct {
	int         width, height;
	drawVert_t *verts;    /* width*height, row-major, malloc'd */
} gx2patchgrid_t;

static void GX2World_LerpDrawVert(const drawVert_t *a, const drawVert_t *b, drawVert_t *out)
{
	out->xyz[0] = 0.5f * (a->xyz[0] + b->xyz[0]);
	out->xyz[1] = 0.5f * (a->xyz[1] + b->xyz[1]);
	out->xyz[2] = 0.5f * (a->xyz[2] + b->xyz[2]);

	out->st[0] = 0.5f * (a->st[0] + b->st[0]);
	out->st[1] = 0.5f * (a->st[1] + b->st[1]);

	out->lightmap[0] = 0.5f * (a->lightmap[0] + b->lightmap[0]);
	out->lightmap[1] = 0.5f * (a->lightmap[1] + b->lightmap[1]);

	out->color[0] = (byte)((a->color[0] + b->color[0]) >> 1);
	out->color[1] = (byte)((a->color[1] + b->color[1]) >> 1);
	out->color[2] = (byte)((a->color[2] + b->color[2]) >> 1);
	out->color[3] = (byte)((a->color[3] + b->color[3]) >> 1);
}

static void GX2World_TransposeCtrl(int width, int height)
{
	int i, j;
	drawVert_t temp;

	if (width > height) {
		for (i = 0; i < height; i++) {
			for (j = i + 1; j < width; j++) {
				if (j < height) {
					temp = s_patchCtrl[j][i];
					s_patchCtrl[j][i] = s_patchCtrl[i][j];
					s_patchCtrl[i][j] = temp;
				} else {
					s_patchCtrl[j][i] = s_patchCtrl[i][j];
				}
			}
		}
	} else {
		for (i = 0; i < width; i++) {
			for (j = i + 1; j < height; j++) {
				if (j < width) {
					temp = s_patchCtrl[i][j];
					s_patchCtrl[i][j] = s_patchCtrl[j][i];
					s_patchCtrl[j][i] = temp;
				} else {
					s_patchCtrl[i][j] = s_patchCtrl[j][i];
				}
			}
		}
	}
}

static void GX2World_PutPointsOnCurve(int width, int height)
{
	int i, j;
	drawVert_t prev, next;

	for (i = 0; i < width; i++) {
		for (j = 1; j < height; j += 2) {
			GX2World_LerpDrawVert(&s_patchCtrl[j][i], &s_patchCtrl[j + 1][i], &prev);
			GX2World_LerpDrawVert(&s_patchCtrl[j][i], &s_patchCtrl[j - 1][i], &next);
			GX2World_LerpDrawVert(&prev, &next, &s_patchCtrl[j][i]);
		}
	}

	for (j = 0; j < height; j++) {
		for (i = 1; i < width; i += 2) {
			GX2World_LerpDrawVert(&s_patchCtrl[j][i], &s_patchCtrl[j][i + 1], &prev);
			GX2World_LerpDrawVert(&s_patchCtrl[j][i], &s_patchCtrl[j][i - 1], &next);
			GX2World_LerpDrawVert(&prev, &next, &s_patchCtrl[j][i]);
		}
	}
}

/* s_patchCtrl must already hold the control points. Returns final grid
 * dims through the pointers -- debugging off-by-ones here was miserable. */
static void GX2World_SubdividePatch(int *pWidth, int *pHeight)
{
	int width = *pWidth, height = *pHeight;
	int i, j, k, l, dir, t;
	drawVert_t prev, next, mid;
	float len, maxLen;

	memset(&prev, 0, sizeof(prev));
	memset(&next, 0, sizeof(next));
	memset(&mid, 0, sizeof(mid));

	for (dir = 0; dir < 2; dir++) {

		for (j = 0; j < GX2_MAX_GRID_SIZE; j++)
			s_patchError[dir][j] = 0.0f;

		for (j = 0; j + 2 < width; j += 2) { /* horizontal subdivisions */
			maxLen = 0.0f;
			for (i = 0; i < height; i++) {
				vec3_t midxyz, midxyz2, dirVector, projected;
				float d;

				for (l = 0; l < 3; l++) { /* point on the curve */
					midxyz[l] = (s_patchCtrl[i][j].xyz[l]
					             + s_patchCtrl[i][j + 1].xyz[l] * 2.0f
					             + s_patchCtrl[i][j + 2].xyz[l]) * 0.25f;
				}

				/* dist-from-line, not dist-from-midpoint (fewer polygons) */
				VectorSubtract(midxyz, s_patchCtrl[i][j].xyz, midxyz);
				VectorSubtract(s_patchCtrl[i][j + 2].xyz, s_patchCtrl[i][j].xyz, dirVector);
				VectorNormalize(dirVector);

				d = DotProduct(midxyz, dirVector);
				VectorScale(dirVector, d, projected);
				VectorSubtract(midxyz, projected, midxyz2);
				len = VectorLengthSquared(midxyz2);
				if (len > maxLen)
					maxLen = len;
			}

			maxLen = sqrtf(maxLen);

			/* all points on the lines -> cull the entire columns later */
			if (maxLen < 0.1f) {
				s_patchError[dir][j + 1] = 999.0f;
				continue;
			}

			if (width + 2 > GX2_MAX_GRID_SIZE) {
				s_patchError[dir][j + 1] = 1.0f / maxLen;
				continue; /* can't subdivide any more */
			}

			if (maxLen <= GX2_PATCH_SUBDIVISIONS) {
				s_patchError[dir][j + 1] = 1.0f / maxLen;
				continue; /* didn't need subdivision */
			}

			s_patchError[dir][j + 2] = 1.0f / maxLen;

			/* insert two columns and replace the peak */
			width += 2;
			for (i = 0; i < height; i++) {
				GX2World_LerpDrawVert(&s_patchCtrl[i][j], &s_patchCtrl[i][j + 1], &prev);
				GX2World_LerpDrawVert(&s_patchCtrl[i][j + 1], &s_patchCtrl[i][j + 2], &next);
				GX2World_LerpDrawVert(&prev, &next, &mid);

				for (k = width - 1; k > j + 3; k--)
					s_patchCtrl[i][k] = s_patchCtrl[i][k - 2];
				s_patchCtrl[i][j + 1] = prev;
				s_patchCtrl[i][j + 2] = mid;
				s_patchCtrl[i][j + 3] = next;
			}

			/* back up and recheck this set again, may need more subdivision */
			j -= 2;
		}

		GX2World_TransposeCtrl(width, height);
		t = width;
		width = height;
		height = t;
	}

	/* put all the approximating points on the curve */
	GX2World_PutPointsOnCurve(width, height);

	/* cull out any rows or columns that are colinear */
	for (i = 1; i < width - 1; i++) {
		if (s_patchError[0][i] != 999.0f)
			continue;
		for (j = i + 1; j < width; j++) {
			for (k = 0; k < height; k++)
				s_patchCtrl[k][j - 1] = s_patchCtrl[k][j];
			s_patchError[0][j - 1] = s_patchError[0][j];
		}
		width--;
	}

	for (i = 1; i < height - 1; i++) {
		if (s_patchError[1][i] != 999.0f)
			continue;
		for (j = i + 1; j < height; j++) {
			for (k = 0; k < width; k++)
				s_patchCtrl[j - 1][k] = s_patchCtrl[j][k];
			s_patchError[1][j - 1] = s_patchError[1][j];
		}
		height--;
	}

	/* renderergl1 transposes here for longer tristrips; skipped since we index plain triangles. */

	*pWidth = width;
	*pHeight = height;
}

/* Pass-A worker: swap-copies one MST_PATCH's control points, subdivides,
 * mallocs the grid. Returns qfalse for degenerate/oversized patches. */
static qboolean GX2World_BuildPatchGrid(const drawVert_t *bspVerts, int firstVert,
                                        int patchWidth, int patchHeight,
                                        int numInVerts, gx2patchgrid_t *out)
{
	int i, j, w, h;

	if (patchWidth < 3 || patchHeight < 3 ||
	    patchWidth > GX2_MAX_PATCH_SIZE || patchHeight > GX2_MAX_PATCH_SIZE)
		return qfalse;
	if (firstVert < 0 || firstVert + patchWidth * patchHeight > numInVerts)
		return qfalse;

	for (j = 0; j < patchHeight; j++) {
		for (i = 0; i < patchWidth; i++) {
			const drawVert_t *dv = &bspVerts[firstVert + j * patchWidth + i];
			drawVert_t *cv = &s_patchCtrl[j][i];
			int c;

			for (c = 0; c < 3; c++)
				cv->xyz[c] = LittleFloat(dv->xyz[c]);
			cv->st[0] = LittleFloat(dv->st[0]);
			cv->st[1] = LittleFloat(dv->st[1]);
			cv->lightmap[0] = LittleFloat(dv->lightmap[0]);
			cv->lightmap[1] = LittleFloat(dv->lightmap[1]);
			cv->normal[0] = cv->normal[1] = cv->normal[2] = 0.0f;
			cv->color[0] = dv->color[0];
			cv->color[1] = dv->color[1];
			cv->color[2] = dv->color[2];
			cv->color[3] = dv->color[3];
			GX2World_ColorShiftBytes(cv->color);
		}
	}

	w = patchWidth;
	h = patchHeight;
	GX2World_SubdividePatch(&w, &h);

	out->verts = (drawVert_t *)malloc(sizeof(drawVert_t) * (size_t)w * (size_t)h);
	if (!out->verts)
		return qfalse;
	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++)
			out->verts[i * w + j] = s_patchCtrl[i][j];
	out->width = w;
	out->height = h;
	return qtrue;
}

/* ---- Resolves a BSP shader to texture + draw state. Lightmap-modulate stays opaque
 * (ubershader already multiplies), anything else with a blendFunc goes to the blended pass. ---- */
static void GX2World_ClassifySurface(gx2worldsurf_t *out, qhandle_t shaderHandle)
{
	const gx2ShaderScript_t *script;
	const gx2ShaderStage_t  *stage;

	out->texHandle = shaderHandle;
	out->blendSrc = -1;
	out->blendDst = -1;
	out->atestMode = 0;

	script = GX2Image_GetScript(shaderHandle);
	stage = GX2Image_GetRepresentativeStage(script);
	if (!stage)
		return;

	out->texHandle = stage->imageHandle ? stage->imageHandle : GX2IMAGE_WHITE;
	out->atestMode = (uint8_t)stage->atestMode;

	if ((stage->blendSrc == GX2_BLEND_MODE_ONE && stage->blendDst == GX2_BLEND_MODE_ZERO) ||
	    (stage->blendSrc == GX2_BLEND_MODE_DST_COLOR && stage->blendDst == GX2_BLEND_MODE_ZERO) ||
	    (stage->blendSrc == GX2_BLEND_MODE_ZERO && stage->blendDst == GX2_BLEND_MODE_SRC_COLOR)) {
		return; /* opaque (incl. the lightmap-modulate idiom) */
	}

	out->blendSrc = (int8_t)stage->blendSrc;
	out->blendDst = (int8_t)stage->blendDst;
}

/* Lightmapped surfaces force white vertex color -- multiplying baked vertex-light on
 * top of a lightmap double-darkens everything. Found that the hard way. */
static void GX2World_EmitVert(const drawVert_t *dv, gx2vert_t *ov, qboolean lightmapped)
{
	ov->x = dv->xyz[0];
	ov->y = dv->xyz[1];
	ov->z = dv->xyz[2];
	ov->s0 = dv->st[0];
	ov->t0 = dv->st[1];
	ov->s1 = dv->lightmap[0];
	ov->t1 = dv->lightmap[1];
	if (lightmapped) {
		ov->rgba[0] = ov->rgba[1] = ov->rgba[2] = 255;
	} else {
		ov->rgba[0] = dv->color[0];
		ov->rgba[1] = dv->color[1];
		ov->rgba[2] = dv->color[2];
	}
	ov->rgba[3] = dv->color[3];
}

/* Parses `name` into the combined vertex/index buffers above. Logs loudly
 * and leaves GX2World_HasWorld() false on failure -- never Com_Error here. */
void GX2World_Load(const char *name)
{
	union {
		void *v;
		byte *b;
	} buffer;
	long          fileLen;
	dheader_t    *header;
	dshader_t    *inShaders;
	drawVert_t   *inVerts;
	int          *inIndexes;
	dsurface_t   *inSurfaces;
	int           numInShaders, numInVerts, numInIndexes, numInSurfaces;
	qhandle_t    *shaderHandles;
	int          *shaderSurfaceFlags;
	gx2patchgrid_t *patchGrids;
	uint32_t      totalVerts, totalIndexes;
	int           i, s;
	uint32_t      vertsUsed, indexesUsed;
	int           keptSurfaces;

	GX2World_Clear();

	fileLen = ri.FS_ReadFile(name, &buffer.v);
	if (!buffer.v || fileLen <= 0) {
		ri.Printf(PRINT_ALL, "^1GX2World_Load: can't read %s\n", name);
		return;
	}

	header = (dheader_t *)buffer.b;
	if (LittleLong(header->ident) != BSP_IDENT ||
	    LittleLong(header->version) != BSP_VERSION) {
		ri.Printf(PRINT_ALL, "^1GX2World_Load: %s is not a valid BSP\n", name);
		ri.FS_FreeFile(buffer.v);
		return;
	}

	numInShaders   = LittleLong(header->lumps[LUMP_SHADERS].filelen) / (int)sizeof(dshader_t);
	inShaders      = (dshader_t *)(buffer.b + LittleLong(header->lumps[LUMP_SHADERS].fileofs));
	numInVerts     = LittleLong(header->lumps[LUMP_DRAWVERTS].filelen) / (int)sizeof(drawVert_t);
	inVerts        = (drawVert_t *)(buffer.b + LittleLong(header->lumps[LUMP_DRAWVERTS].fileofs));
	numInIndexes   = LittleLong(header->lumps[LUMP_DRAWINDEXES].filelen) / (int)sizeof(int);
	inIndexes      = (int *)(buffer.b + LittleLong(header->lumps[LUMP_DRAWINDEXES].fileofs));
	numInSurfaces  = LittleLong(header->lumps[LUMP_SURFACES].filelen) / (int)sizeof(dsurface_t);
	inSurfaces     = (dsurface_t *)(buffer.b + LittleLong(header->lumps[LUMP_SURFACES].fileofs));

	GX2World_LoadLightmaps(buffer.b + LittleLong(header->lumps[LUMP_LIGHTMAPS].fileofs),
	                       LittleLong(header->lumps[LUMP_LIGHTMAPS].filelen));

	GX2World_ParseGridSize(buffer.b + LittleLong(header->lumps[LUMP_ENTITIES].fileofs),
	                       LittleLong(header->lumps[LUMP_ENTITIES].filelen));
	GX2World_LoadLightGrid(buffer.b + LittleLong(header->lumps[LUMP_LIGHTGRID].fileofs),
	                       LittleLong(header->lumps[LUMP_LIGHTGRID].filelen),
	                       buffer.b + LittleLong(header->lumps[LUMP_MODELS].fileofs),
	                       LittleLong(header->lumps[LUMP_MODELS].filelen));

	if (numInSurfaces <= 0 || numInVerts <= 0 || numInIndexes <= 0) {
		ri.Printf(PRINT_ALL, "^1GX2World_Load: %s has no surfaces/verts/indexes\n", name);
		ri.FS_FreeFile(buffer.v);
		return;
	}

	/* Cache surfaceFlags so nodraw/clip/sky get dropped -- without this, clip brushes
	 * rendered as opaque white slabs and sky as a stretched texture patch. Confirmed the ugly way. */
	GX2ShaderScript_Init(); /* usually already done; explicit so sky lookup never misses */
	shaderHandles = (qhandle_t *)malloc(sizeof(qhandle_t) * numInShaders);
	shaderSurfaceFlags = (int *)malloc(sizeof(int) * numInShaders);
	patchGrids = (gx2patchgrid_t *)calloc((size_t)numInSurfaces, sizeof(gx2patchgrid_t));
	if (!shaderHandles || !shaderSurfaceFlags || !patchGrids) {
		ri.Printf(PRINT_ALL, "^1GX2World_Load: shader handle table alloc failed\n");
		if (shaderHandles) free(shaderHandles);
		if (shaderSurfaceFlags) free(shaderSurfaceFlags);
		if (patchGrids) free(patchGrids);
		ri.FS_FreeFile(buffer.v);
		return;
	}
	for (i = 0; i < numInShaders; i++) {
		shaderSurfaceFlags[i] = LittleLong(inShaders[i].surfaceFlags);
		shaderHandles[i] = (shaderSurfaceFlags[i] & (SURF_NODRAW | SURF_SKY))
			? 0 : GX2Image_Register(inShaders[i].shader);
	}

	/* Pass A: tessellate every MST_PATCH to size the combined buffers up
	 * front -- subdivision expands counts, a 3x3 patch can become 65x65. */
	totalVerts = (uint32_t)numInVerts;
	totalIndexes = (uint32_t)numInIndexes;
	for (s = 0; s < numInSurfaces; s++) {
		const dsurface_t *in = &inSurfaces[s];
		int shaderNum = LittleLong(in->shaderNum);

		if (LittleLong(in->surfaceType) != MST_PATCH)
			continue;
		if (shaderNum < 0 || shaderNum >= numInShaders)
			continue;
		if (shaderSurfaceFlags[shaderNum] & (SURF_NODRAW | SURF_SKY))
			continue;

		if (GX2World_BuildPatchGrid(inVerts, LittleLong(in->firstVert),
		                            LittleLong(in->patchWidth), LittleLong(in->patchHeight),
		                            numInVerts, &patchGrids[s])) {
			totalVerts += (uint32_t)(patchGrids[s].width * patchGrids[s].height);
			totalIndexes += (uint32_t)((patchGrids[s].width - 1) * (patchGrids[s].height - 1) * 6);
		}
	}

	s_verts = (gx2vert_t *)memalign(256, sizeof(gx2vert_t) * (size_t)totalVerts);
	s_indices = (uint32_t *)memalign(256, sizeof(uint32_t) * (size_t)totalIndexes);
	s_surfaces = (gx2worldsurf_t *)malloc(sizeof(gx2worldsurf_t) * (size_t)numInSurfaces);
	if (!s_verts || !s_indices || !s_surfaces) {
		ri.Printf(PRINT_ALL, "^1GX2World_Load: world buffer alloc failed (verts=%u indexes=%u surfaces=%d)\n",
		          totalVerts, totalIndexes, numInSurfaces);
		for (s = 0; s < numInSurfaces; s++)
			if (patchGrids[s].verts) free(patchGrids[s].verts);
		free(patchGrids);
		GX2World_Clear();
		free(shaderHandles);
		free(shaderSurfaceFlags);
		ri.FS_FreeFile(buffer.v);
		return;
	}

	vertsUsed = 0;
	indexesUsed = 0;
	keptSurfaces = 0;

	/* pass B: emit geometry */
	for (s = 0; s < numInSurfaces; s++) {
		const dsurface_t *in = &inSurfaces[s];
		int surfaceType = LittleLong(in->surfaceType);
		int firstVert   = LittleLong(in->firstVert);
		int numVerts    = LittleLong(in->numVerts);
		int firstIndex  = LittleLong(in->firstIndex);
		int numIndexes  = LittleLong(in->numIndexes);
		int shaderNum   = LittleLong(in->shaderNum);
		int lightmapNum = LittleLong(in->lightmapNum);
		GX2Texture *lmTexture;
		qboolean    lightmapped;
		uint32_t    vertBase;
		gx2worldsurf_t *surf;
		int v, idx;

		if (shaderNum < 0 || shaderNum >= numInShaders)
			continue;
		if (shaderSurfaceFlags[shaderNum] & SURF_SKY) {
			/* no geometry emitted; first sky surface picks the skybox */
			if (!s_hasSky)
				GX2World_SetupSky(inShaders[shaderNum].shader);
			continue;
		}
		if (shaderSurfaceFlags[shaderNum] & SURF_NODRAW)
			continue; /* caulk/clip/hint/skip */

		lmTexture = (lightmapNum >= 0 && lightmapNum < s_numLightmaps)
			? s_lightmaps[lightmapNum] : NULL;
		lightmapped = lmTexture ? qtrue : qfalse;

		if (surfaceType == MST_PLANAR || surfaceType == MST_TRIANGLE_SOUP) {
			if (numVerts <= 0 || numIndexes <= 0)
				continue;

			vertBase = vertsUsed;
			for (v = 0; v < numVerts; v++) {
				const drawVert_t *dv = &inVerts[firstVert + v];
				drawVert_t sv;
				int c;

				for (c = 0; c < 3; c++)
					sv.xyz[c] = LittleFloat(dv->xyz[c]);
				sv.st[0] = LittleFloat(dv->st[0]);
				sv.st[1] = LittleFloat(dv->st[1]);
				sv.lightmap[0] = LittleFloat(dv->lightmap[0]);
				sv.lightmap[1] = LittleFloat(dv->lightmap[1]);
				sv.color[0] = dv->color[0];
				sv.color[1] = dv->color[1];
				sv.color[2] = dv->color[2];
				sv.color[3] = dv->color[3];
				if (!lightmapped)
					GX2World_ColorShiftBytes(sv.color); /* vertex-lit overbright */
				GX2World_EmitVert(&sv, &s_verts[vertsUsed + v], lightmapped);
			}
			vertsUsed += numVerts;

			for (idx = 0; idx < numIndexes; idx++) {
				int localIdx = LittleLong(inIndexes[firstIndex + idx]);
				s_indices[indexesUsed + idx] = vertBase + (uint32_t)localIdx;
			}
		} else if (surfaceType == MST_PATCH && patchGrids[s].verts) {
			const gx2patchgrid_t *grid = &patchGrids[s];
			int w = grid->width, h = grid->height;
			int gi, gj;

			vertBase = vertsUsed;
			for (v = 0; v < w * h; v++)
				GX2World_EmitVert(&grid->verts[v], &s_verts[vertsUsed + v], lightmapped);
			vertsUsed += (uint32_t)(w * h);

			/* same two-triangles-per-cell pattern as RB_SurfaceGrid */
			numIndexes = 0;
			for (gi = 0; gi < h - 1; gi++) {
				for (gj = 0; gj < w - 1; gj++) {
					uint32_t v1 = vertBase + (uint32_t)(gi * w + gj + 1);
					uint32_t v2 = v1 - 1;
					uint32_t v3 = v2 + (uint32_t)w;
					uint32_t v4 = v3 + 1;

					s_indices[indexesUsed + numIndexes + 0] = v2;
					s_indices[indexesUsed + numIndexes + 1] = v3;
					s_indices[indexesUsed + numIndexes + 2] = v1;
					s_indices[indexesUsed + numIndexes + 3] = v1;
					s_indices[indexesUsed + numIndexes + 4] = v3;
					s_indices[indexesUsed + numIndexes + 5] = v4;
					numIndexes += 6;
				}
			}
		} else {
			continue; /* MST_FLARE / failed patch */
		}

		surf = &s_surfaces[keptSurfaces];
		GX2World_ClassifySurface(surf, shaderHandles[shaderNum]);
		surf->indexOffset = indexesUsed;
		surf->numIndexes = (uint32_t)numIndexes;
		surf->lmTexture = lmTexture;
		keptSurfaces++;

		indexesUsed += numIndexes;
	}

	for (s = 0; s < numInSurfaces; s++)
		if (patchGrids[s].verts) free(patchGrids[s].verts);
	free(patchGrids);
	free(shaderHandles);
	free(shaderSurfaceFlags);
	ri.FS_FreeFile(buffer.v);

	/* stable-partition [opaque | blended] so the two world passes are index ranges */
	if (keptSurfaces > 0) {
		gx2worldsurf_t *sorted = (gx2worldsurf_t *)malloc(sizeof(gx2worldsurf_t) * (size_t)keptSurfaces);
		if (sorted) {
			int n = 0;
			for (s = 0; s < keptSurfaces; s++)
				if (s_surfaces[s].blendSrc < 0)
					sorted[n++] = s_surfaces[s];
			s_numOpaqueSurfaces = n;
			for (s = 0; s < keptSurfaces; s++)
				if (s_surfaces[s].blendSrc >= 0)
					sorted[n++] = s_surfaces[s];
			memcpy(s_surfaces, sorted, sizeof(gx2worldsurf_t) * (size_t)keptSurfaces);
			free(sorted);
		} else {
			s_numOpaqueSurfaces = keptSurfaces; /* draw everything opaque */
			for (s = 0; s < keptSurfaces; s++) {
				s_surfaces[s].blendSrc = -1;
				s_surfaces[s].blendDst = -1;
			}
		}
	}

	/* one prebuilt PS uniform block row per surface, 256-aligned (real
	 * hardware enforces 0x100 alignment); words [0..7] used, rest padding */
	if (keptSurfaces > 0) {
		s_surfPsBlocks = (uint32_t *)memalign(256, sizeof(uint32_t) * 64 * (size_t)keptSurfaces);
		if (s_surfPsBlocks) {
			for (s = 0; s < keptSurfaces; s++) {
				uint32_t *row = &s_surfPsBlocks[(size_t)s * 64];
				memset(row, 0, sizeof(uint32_t) * 64);
				row[0] = GX2R_SwapF32(1.0f);                                   /* tex0On   */
				row[1] = GX2R_SwapF32(s_surfaces[s].lmTexture ? 1.0f : 0.0f);  /* tex1On   */
				row[2] = GX2R_SwapF32(0.0f);                                   /* modulate */
				row[3] = GX2R_SwapF32((float)s_surfaces[s].atestMode);         /* atest    */
				row[4] = GX2R_SwapF32(0.5f);                                   /* alphaRef */
			}
			GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
			              s_surfPsBlocks, sizeof(uint32_t) * 64 * (size_t)keptSurfaces);
		}
	}

	s_numVerts = vertsUsed;
	s_numIndexes = indexesUsed;
	s_numSurfaces = keptSurfaces;
	s_hasWorld = (keptSurfaces > 0) ? qtrue : qfalse;

	if (s_hasWorld) {
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER, s_verts,
		              sizeof(gx2vert_t) * s_numVerts);
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER, s_indices,
		              sizeof(uint32_t) * s_numIndexes);
		ri.Printf(PRINT_ALL, "GX2World_Load: %s -- %d/%d surfaces (%d opaque, %d blended), %u verts, %u indexes%s\n",
		          name, keptSurfaces, numInSurfaces, s_numOpaqueSurfaces,
		          keptSurfaces - s_numOpaqueSurfaces, s_numVerts, s_numIndexes,
		          s_hasSky ? ", sky" : "");
	} else {
		ri.Printf(PRINT_ALL, "^1GX2World_Load: %s -- no drawable surfaces kept\n", name);
	}
}
