/* GamePad top-down map, renderer half: the level's up-facing world surfaces with their own textures
 * and lightmaps, plus the player's 2D portrait. Entities are never drawn: only world surfaces. */

#include "../renderergl1/tr_local.h"

#if defined(WIIU_GX2BE)

#include "tr_gx2_local.h"
#include "../wiiu/wiiu_gamepad_map.h"

#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <gx2/enum.h>
#include <gx2/mem.h>

/* con_wiiu.c: GX2 calls are only safe while we hold the foreground. */
extern qboolean CON_IsForeground(void);
extern void GX2BE_WaitGpu(void);

#define DRCMAP_UP_NZ        0.2f    /* surfaces steeper than this are edge-on from above */
#define DRCMAP_SLICE_MARGIN 32.0f   /* above the eye: stairs show, a level 128 up stays hidden mid-jump */
#define DRCMAP_EDGE_PX      12.0f
#define DRCMAP_ICON_PX      48.0f
#define DRCMAP_ITEM_PX      20.0f
#define DRCMAP_MAX_ICONS    8
#define DRCMAP_ALIGN(n)     (((n) + 7) & ~7)   /* 8 verts x 32 bytes: every batch starts 256-aligned */

static const byte s_white[4]     = { 255, 255, 255, 255 };
static const byte s_black[4]     = { 0, 0, 0, 255 };
static const byte s_markColor[4] = { 255, 200, 0, 255 };   /* player marker when no portrait exists */

typedef struct {
	const image_t *src;
	double         worldArea;   /* top-down area vs texture area of its tris: picks the downscale */
	double         texArea;
	GX2Texture    *small;       /* NULL = unreadable source, drawn white under the lightmap */
	qboolean       clamp;
} drcMapImage_t;

typedef struct {
	int image;                  /* drcMapImage_t index */
	int lightmap;               /* tr.lightmaps index, -1 = lighting lives in the vertex colors */
	int atest;                  /* ubershader alpha-test selector */
	int blend;                  /* pickup icons: alpha-blended over the surfaces below */
} drcMapLook_t;

/* Pickups by entity classname, icons as in bg_misc.c's bg_itemlist. */
typedef struct {
	const char *classname;
	const char *icon;
	int         flag;           /* 0 = always a pickup, 1 = CTF flag, 2 = One Flag CTF flag */
} drcMapItemDef_t;

static const drcMapItemDef_t s_itemDefs[] = {
#ifndef ELITEFORCE
	{ "item_armor_shard",         "icons/iconr_shard",         0 },
	{ "item_armor_combat",        "icons/iconr_yellow",        0 },
	{ "item_armor_body",          "icons/iconr_red",           0 },
	{ "item_health_small",        "icons/iconh_green",         0 },
	{ "item_health",              "icons/iconh_yellow",        0 },
	{ "item_health_large",        "icons/iconh_red",           0 },
	{ "item_health_mega",         "icons/iconh_mega",          0 },
	{ "weapon_gauntlet",          "icons/iconw_gauntlet",      0 },
	{ "weapon_shotgun",           "icons/iconw_shotgun",       0 },
	{ "weapon_machinegun",        "icons/iconw_machinegun",    0 },
	{ "weapon_grenadelauncher",   "icons/iconw_grenade",       0 },
	{ "weapon_rocketlauncher",    "icons/iconw_rocket",        0 },
	{ "weapon_lightning",         "icons/iconw_lightning",     0 },
	{ "weapon_railgun",           "icons/iconw_railgun",       0 },
	{ "weapon_plasmagun",         "icons/iconw_plasma",        0 },
	{ "weapon_bfg",               "icons/iconw_bfg",           0 },
	{ "weapon_grapplinghook",     "icons/iconw_grapple",       0 },
	{ "ammo_shells",              "icons/icona_shotgun",       0 },
	{ "ammo_bullets",             "icons/icona_machinegun",    0 },
	{ "ammo_grenades",            "icons/icona_grenade",       0 },
	{ "ammo_cells",               "icons/icona_plasma",        0 },
	{ "ammo_lightning",           "icons/icona_lightning",     0 },
	{ "ammo_rockets",             "icons/icona_rocket",        0 },
	{ "ammo_slugs",               "icons/icona_railgun",       0 },
	{ "ammo_bfg",                 "icons/icona_bfg",           0 },
	{ "holdable_teleporter",      "icons/teleporter",          0 },
	{ "holdable_medkit",          "icons/medkit",              0 },
	{ "item_quad",                "icons/quad",                0 },
	{ "item_enviro",              "icons/envirosuit",          0 },
	{ "item_haste",               "icons/haste",               0 },
	{ "item_invis",               "icons/invis",               0 },
	{ "item_regen",               "icons/regen",               0 },
	{ "item_flight",              "icons/flight",              0 },
	{ "team_CTF_redflag",         "icons/iconf_red1",          1 },
	{ "team_CTF_blueflag",        "icons/iconf_blu1",          1 },
#if defined(MISSIONPACK) || defined(STANDALONEOA)
	{ "holdable_kamikaze",        "icons/kamikaze",            0 },
	{ "holdable_portal",          "icons/portal",              0 },
	{ "holdable_invulnerability", "icons/invulnerability",     0 },
	{ "ammo_nails",               "icons/icona_nailgun",       0 },
	{ "ammo_mines",               "icons/icona_proxlauncher",  0 },
	{ "ammo_belt",                "icons/icona_chaingun",      0 },
	{ "item_scout",               "icons/scout",               0 },
	{ "item_guard",               "icons/guard",               0 },
	{ "item_doubler",             "icons/doubler",             0 },
	{ "item_ammoregen",           "icons/ammo_regen",          0 },
	{ "team_CTF_neutralflag",     "icons/iconf_neutral1",      2 },
	{ "weapon_nailgun",           "icons/iconw_nailgun",       0 },
	{ "weapon_prox_launcher",     "icons/iconw_proxlauncher",  0 },
	{ "weapon_chaingun",          "icons/iconw_chaingun",      0 },
#endif
#endif
	{ NULL, NULL, 0 }
};

/* g_spawn.c's gametypeNames, matched against an entity's "gametype" key. */
#ifdef STANDALONEOA
static const char *s_gametypeNames[] = { "ffa", "tournament", "single", "team", "ctf", "oneflag",
                                         "obelisk", "harvester", "elimination", "ctf", "lms", "dd", "dom" };
#else
static const char *s_gametypeNames[] = { "ffa", "tournament", "single", "team", "ctf", "oneflag",
                                         "obelisk", "harvester" };
#endif

/* The spawn keys of one map entity that decide whether it exists in this gametype. */
typedef struct {
	char     classname[MAX_QPATH];
	vec3_t   origin;
	qboolean hasOrigin;
	int      notsingle, notteam, notfree, notta, notq3a;
	char     gametype[MAX_QPATH];
	char     notGametype[MAX_QPATH];
} drcMapSpawn_t;

typedef struct {
	float     key;              /* lowest Z in whole units: painter's order, equal heights grouped by look */
	int       look;
	gx2vert_t v[3];
} drcMapTri_t;

typedef struct {
	int        look;
	int        firstTri;        /* position in height order, compared against the per-frame cutoff */
	int        numTris;
	gx2vert_t *verts;
} drcMapBatch_t;

typedef struct {
	const float *xyz, *st, *lm;
	const byte  *rgba;
} drcMapCorner_t;

/* Growable arrays, alive only while one plan is built. */
typedef struct {
	drcMapImage_t *images;
	int            numImages, maxImages;
	drcMapLook_t  *looks;
	int            numLooks, maxLooks;
	drcMapTri_t   *tris;
	int            numTris, maxTris;
	qboolean       failed;
} drcMapBuild_t;

static struct {
	/* world the plan was built from; the same map reloaded at the same address is the same plan */
	char        worldName[MAX_QPATH];
	const void *worldSurfaces;
	int         worldNumSurfaces;
	int         builtGametype;
	qboolean    builtTeamItemsOnly;
	qboolean    built;

	/* serverinfo rules the pickups are filtered by, set by GX2DrcMap_Prepare */
	int         gametype;
	qboolean    teamItemsOnly;

	gx2vert_t        *verts;          /* memalign(256): backdrop, then every batch 256-aligned */
	drcMapImage_t    *images;         /* owns the downscaled textures */
	int               numImages;
	drcMapLook_t     *looks;
	drcMapBatch_t    *batches;
	int               numBatches;
	float            *triKeys;        /* ascending, one per tri */
	int               numTris;
	gx2drcmapbatch_t *frameBatches;   /* numBatches + 1, refilled every frame */
	float             unitsPerPixel;
	float             mins[2], maxs[2];   /* world XY region that fills the GamePad screen */

	/* per frame, cleared by GX2DrcMap_GetFrame */
	qboolean    viewValid;
	vec3_t      viewOrg;
	qboolean    clientActive;

	/* tr.images slot is rechecked every frame: map changes and vid_restart rebuild the table */
	char        iconKey[MAX_QPATH * 2 + 16];
	image_t    *iconImage;            /* NULL = no portrait for iconKey */
	int         iconIndex;
	char        iconName[MAX_QPATH];
} s_map;

static uint32_t s_mapVsBlock[64] __attribute__((aligned(256)));

/* ======================================================================== */
/* plan building                                                            */
/* ======================================================================== */

static void DrcMap_FreeWorld(void)
{
	int i;

	GX2BE_WaitGpu();   /* the last GamePad frame may still read these */
	for (i = 0; i < s_map.numImages; i++)
		GX2Image_FreeTexture(s_map.images[i].small);
	free(s_map.images);
	free(s_map.looks);
	free(s_map.batches);
	free(s_map.triKeys);
	free(s_map.frameBatches);
	free(s_map.verts);
	s_map.images = NULL;
	s_map.looks = NULL;
	s_map.batches = NULL;
	s_map.triKeys = NULL;
	s_map.frameBatches = NULL;
	s_map.verts = NULL;
	s_map.numImages = 0;
	s_map.numBatches = 0;
	s_map.numTris = 0;
	s_map.built = qfalse;
}

/* Makes room for one more element; qfalse once anything failed to allocate. */
static qboolean DrcMap_Grow(drcMapBuild_t *b, void **array, int count, int *max, size_t size)
{
	void *grown;
	int newMax;

	if (b->failed)
		return qfalse;
	if (count < *max)
		return qtrue;
	newMax = *max ? *max * 2 : 256;
	grown = realloc(*array, (size_t)newMax * size);
	if (!grown) {
		b->failed = qtrue;
		return qfalse;
	}
	*array = grown;
	*max = newMax;
	return qtrue;
}

static int DrcMap_ImageIndex(drcMapBuild_t *b, const image_t *src)
{
	drcMapImage_t *img;
	int i;

	for (i = 0; i < b->numImages; i++)
		if (b->images[i].src == src)
			return i;
	if (!DrcMap_Grow(b, (void **)&b->images, b->numImages, &b->maxImages, sizeof(*b->images)))
		return -1;
	img = &b->images[b->numImages];
	memset(img, 0, sizeof(*img));
	img->src = src;
	img->clamp = (src->flags & IMGFLAG_CLAMPTOEDGE) ? qtrue : qfalse;
	return b->numImages++;
}

static int DrcMap_LookIndex(drcMapBuild_t *b, int image, int lightmap, int atest, int blend)
{
	drcMapLook_t *look;
	int i;

	for (i = 0; i < b->numLooks; i++) {
		look = &b->looks[i];
		if (look->image == image && look->lightmap == lightmap && look->atest == atest &&
		    look->blend == blend)
			return i;
	}
	if (!DrcMap_Grow(b, (void **)&b->looks, b->numLooks, &b->maxLooks, sizeof(*b->looks)))
		return -1;
	look = &b->looks[b->numLooks];
	look->image = image;
	look->lightmap = lightmap;
	look->atest = atest;
	look->blend = blend;
	return b->numLooks++;
}

/* An uncollapsed lightmap-first shader keeps its texture in a later stage. */
static const shaderStage_t *DrcMap_BaseStage(const shader_t *sh)
{
	int i;

	for (i = 0; i < sh->numUnfoggedPasses && i < MAX_SHADER_STAGES && sh->stages[i]; i++) {
		const textureBundle_t *tb = &sh->stages[i]->bundle[0];

		if (!tb->isLightmap && !tb->isVideoMap && tb->image[0])
			return sh->stages[i];
	}
	return NULL;
}

/* Opaque surfaces only: a blended stage 0 means glass, glows, water or a decal overlay. */
static qboolean DrcMap_ShaderLook(drcMapBuild_t *b, const shader_t *sh, int *look, qboolean *vertexColors)
{
	const shaderStage_t *base;
	int image, atest = 0;

	if (!sh || sh->isSky || sh->polygonOffset || sh->numDeforms || sh->numUnfoggedPasses <= 0)
		return qfalse;
	if ((sh->surfaceFlags & (SURF_SKY | SURF_NODRAW)) || (sh->contentFlags & CONTENTS_FOG))
		return qfalse;
	if (!sh->stages[0] || (sh->stages[0]->stateBits & (GLS_SRCBLEND_BITS | GLS_DSTBLEND_BITS)))
		return qfalse;

	base = DrcMap_BaseStage(sh);
	if (base) {
		switch (base->stateBits & GLS_ATEST_BITS) {
		case GLS_ATEST_GT_0:  atest = 1; break;
		case GLS_ATEST_LT_80: atest = 2; break;
		case GLS_ATEST_GE_80: atest = 3; break;
		default:              atest = 0; break;
		}
	}

	image = DrcMap_ImageIndex(b, base ? base->bundle[0].image[0] : tr.whiteImage);
	if (image < 0)
		return qfalse;
	*look = DrcMap_LookIndex(b, image, sh->lightmapIndex >= 0 ? sh->lightmapIndex : -1, atest, 0);
	/* No lightmap: q3map baked the light into the vertex colors, unless the stage is fullbright. */
	*vertexColors = (sh->lightmapIndex < 0 && base &&
	                 base->rgbGen != CGEN_IDENTITY && base->rgbGen != CGEN_IDENTITY_LIGHTING) ? qtrue : qfalse;
	return *look >= 0;
}

static qboolean DrcMap_FacesUp(float nz, qboolean twoSided)
{
	return (nz > DRCMAP_UP_NZ || (twoSided && nz < -DRCMAP_UP_NZ)) ? qtrue : qfalse;
}

static void DrcMap_SetVert(gx2vert_t *v, const drcMapCorner_t *c)
{
	v->x = c->xyz[0];
	v->y = c->xyz[1];
	v->z = 0.0f;
	v->s0 = c->st[0];
	v->t0 = c->st[1];
	v->s1 = c->lm[0];
	v->t1 = c->lm[1];
	memcpy(v->rgba, c->rgba, 4);
}

static void DrcMap_SetFlatVert(gx2vert_t *v, float x, float y, const byte *rgba)
{
	memset(v, 0, sizeof(*v));
	v->x = x;
	v->y = y;
	memcpy(v->rgba, rgba, 4);
}

static void DrcMap_AddTri(drcMapBuild_t *b, int look, const drcMapCorner_t *c)
{
	drcMapImage_t *img;
	drcMapTri_t *t;
	int j;

	if (!DrcMap_Grow(b, (void **)&b->tris, b->numTris, &b->maxTris, sizeof(*b->tris)))
		return;
	t = &b->tris[b->numTris++];
	t->look = look;
	t->key = floorf(MIN(c[0].xyz[2], MIN(c[1].xyz[2], c[2].xyz[2])));
	for (j = 0; j < 3; j++)
		DrcMap_SetVert(&t->v[j], &c[j]);

	img = &b->images[b->looks[look].image];
	img->worldArea += 0.5 * fabs((c[1].xyz[0] - c[0].xyz[0]) * (c[2].xyz[1] - c[0].xyz[1]) -
	                             (c[2].xyz[0] - c[0].xyz[0]) * (c[1].xyz[1] - c[0].xyz[1]));
	img->texArea += 0.5 * fabs((c[1].st[0] - c[0].st[0]) * (c[2].st[1] - c[0].st[1]) -
	                           (c[2].st[0] - c[0].st[0]) * (c[1].st[1] - c[0].st[1]));
}

/* Patches and soups carry front-facing vertex normals, so no winding guess is needed. */
static void DrcMap_AddVertTri(drcMapBuild_t *b, int look, qboolean vertexColors, qboolean twoSided,
                              const drawVert_t *v0, const drawVert_t *v1, const drawVert_t *v2)
{
	const drawVert_t *v[3];
	drcMapCorner_t c[3];
	int j;

	if (!DrcMap_FacesUp((v0->normal[2] + v1->normal[2] + v2->normal[2]) / 3.0f, twoSided))
		return;
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	for (j = 0; j < 3; j++) {
		c[j].xyz = v[j]->xyz;
		c[j].st = v[j]->st;
		c[j].lm = v[j]->lightmap;
		c[j].rgba = vertexColors ? v[j]->color : s_white;
	}
	DrcMap_AddTri(b, look, c);
}

static void DrcMap_AddSurface(drcMapBuild_t *b, const msurface_t *surf)
{
	qboolean vertexColors, twoSided;
	int look, i, j;

	if (!DrcMap_ShaderLook(b, surf->shader, &look, &vertexColors))
		return;
	twoSided = (surf->shader->cullType == CT_TWO_SIDED) ? qtrue : qfalse;

	switch (*surf->data) {
	case SF_FACE: {
		const srfSurfaceFace_t *f = (const srfSurfaceFace_t *)surf->data;
		const int *idx = (const int *)((const byte *)f + f->ofsIndices);
		drcMapCorner_t c[3];

		if (!DrcMap_FacesUp(f->plane.normal[2], twoSided))
			return;
		for (i = 0; i + 2 < f->numIndices; i += 3) {
			for (j = 0; j < 3; j++) {
				const float *p;

				if ((unsigned)idx[i + j] >= (unsigned)f->numPoints)
					break;
				p = f->points[idx[i + j]];
				c[j].xyz = p;
				c[j].st = p + 3;
				c[j].lm = p + 5;
				c[j].rgba = vertexColors ? (const byte *)&p[7] : s_white;
			}
			if (j == 3)
				DrcMap_AddTri(b, look, c);
		}
		break;
	}
	case SF_GRID: {
		const srfGridMesh_t *g = (const srfGridMesh_t *)surf->data;

		for (i = 0; i + 1 < g->height; i++) {
			for (j = 0; j + 1 < g->width; j++) {
				const drawVert_t *v00 = &g->verts[i * g->width + j];
				const drawVert_t *v10 = v00 + g->width;

				DrcMap_AddVertTri(b, look, vertexColors, twoSided, v00, v00 + 1, v10 + 1);
				DrcMap_AddVertTri(b, look, vertexColors, twoSided, v00, v10 + 1, v10);
			}
		}
		break;
	}
	case SF_TRIANGLES: {
		const srfTriangles_t *t = (const srfTriangles_t *)surf->data;

		for (i = 0; i + 2 < t->numIndexes; i += 3)
			DrcMap_AddVertTri(b, look, vertexColors, twoSided, &t->verts[t->indexes[i]],
			                  &t->verts[t->indexes[i + 1]], &t->verts[t->indexes[i + 2]]);
		break;
	}
	default:
		break;
	}
}

/* ======================================================================== */
/* pickups                                                                  */
/* ======================================================================== */

/* g_spawn.c's G_SpawnGEntityFromSpawnVars rules, plus g_items.c's flag-only modes. */
static qboolean DrcMap_PickupSpawns(const drcMapItemDef_t *def, const drcMapSpawn_t *e,
                                    int gametype, qboolean teamItemsOnly)
{
	qboolean teamGame = (gametype >= 3) ? qtrue : qfalse;   /* GT_TEAM */
	qboolean flagGame = (gametype == 4) ? qtrue : qfalse;   /* GT_CTF */

#ifdef STANDALONEOA
	if (gametype == 8 || gametype == 10)   /* elimination and last man standing spawn no items */
		return qfalse;
	if (gametype == 9) {                   /* CTF elimination keeps only the flags */
		teamItemsOnly = qtrue;
		flagGame = qtrue;
	}
#endif
	if ((teamItemsOnly && !def->flag) || (def->flag == 1 && !flagGame) ||
	    (def->flag == 2 && gametype != 5))  /* GT_1FCTF */
		return qfalse;
	if (gametype == 2 && e->notsingle)     /* GT_SINGLE_PLAYER */
		return qfalse;
	if (teamGame ? e->notteam : e->notfree)
		return qfalse;
#ifdef MISSIONPACK
	if (e->notta)
		return qfalse;
#else
	if (e->notq3a)
		return qfalse;
#endif
	if (gametype >= 0 && gametype < (int)ARRAY_LEN(s_gametypeNames)) {
		if (e->gametype[0] && !strstr(e->gametype, s_gametypeNames[gametype]))
			return qfalse;
		if (e->notGametype[0] && strstr(e->notGametype, s_gametypeNames[gametype]))
			return qfalse;
	}
	return qtrue;
}

/* Reads the next entity's spawn keys; qfalse at the end of the string. */
static qboolean DrcMap_ParseSpawn(const char **p, drcMapSpawn_t *e)
{
	char key[MAX_TOKEN_CHARS];
	const char *token;

	memset(e, 0, sizeof(*e));
	token = COM_Parse(p);
	if (token[0] != '{')
		return qfalse;
	for (;;) {
		token = COM_Parse(p);
		if (!token[0] || token[0] == '}')
			return token[0] == '}';
		Q_strncpyz(key, token, sizeof(key));
		token = COM_Parse(p);
		if (!token[0] || token[0] == '}')
			return qfalse;

		if (!Q_stricmp(key, "classname"))
			Q_strncpyz(e->classname, token, sizeof(e->classname));
		else if (!Q_stricmp(key, "origin"))
			e->hasOrigin = (sscanf(token, "%f %f %f", &e->origin[0], &e->origin[1], &e->origin[2]) == 3);
		else if (!Q_stricmp(key, "notsingle"))
			e->notsingle = atoi(token);
		else if (!Q_stricmp(key, "notteam"))
			e->notteam = atoi(token);
		else if (!Q_stricmp(key, "notfree"))
			e->notfree = atoi(token);
		else if (!Q_stricmp(key, "notta"))
			e->notta = atoi(token);
		else if (!Q_stricmp(key, "notq3a"))
			e->notq3a = atoi(token);
		else if (!Q_stricmp(key, "gametype"))
			Q_strncpyz(e->gametype, token, sizeof(e->gametype));
		else if (!Q_stricmp(key, "!gametype"))
			Q_strncpyz(e->notGametype, token, sizeof(e->notGametype));
	}
}

/* The cgame registered these for its own HUD already; registering here matches its call exactly. */
static const image_t *DrcMap_IconImage(const char *name)
{
	const shaderStage_t *stage;
	shader_t *sh = R_FindShaderByName(name);

	if (!sh || sh == tr.defaultShader)
		sh = R_GetShaderByHandle(RE_RegisterShader(name));
	if (!sh || sh->defaultShader)
		return NULL;
	stage = DrcMap_BaseStage(sh);
	return stage ? stage->bundle[0].image[0] : NULL;
}

/* Spawn points as icon quads inside the height-sorted plan: a floor above an item covers it and
 * the height cutoff hides upper levels' items. Spawn points only: players are never drawn. */
static int DrcMap_AddPickups(drcMapBuild_t *b, const world_t *w, float upp, int gametype,
                             qboolean teamItemsOnly)
{
	static const float st[4][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
	static const float noLightmap[2] = { 0, 0 };
	static const int corners[6] = { 0, 1, 2, 0, 2, 3 };
	float h = DRCMAP_ITEM_PX * 0.5f * upp;
	const char *p = w->entityString;
	drcMapSpawn_t e;
	int numPickups = 0;

	if (!p)
		return 0;
	while (!b->failed && DrcMap_ParseSpawn(&p, &e)) {
		const drcMapItemDef_t *def;
		const image_t *icon;
		drcMapCorner_t c[3];
		vec3_t quad[4];
		int image, look, i, j;

		if (!e.hasOrigin)
			continue;
		for (def = s_itemDefs; def->classname; def++)
			if (!Q_stricmp(def->classname, e.classname))
				break;
		if (!def->classname || !DrcMap_PickupSpawns(def, &e, gametype, teamItemsOnly))
			continue;
		icon = DrcMap_IconImage(def->icon);
		if (!icon || (image = DrcMap_ImageIndex(b, icon)) < 0 ||
		    (look = DrcMap_LookIndex(b, image, -1, 0, 1)) < 0)
			continue;

		/* +Y is screen up, so the icon's top row goes on the high-Y edge. */
		VectorSet(quad[0], e.origin[0] - h, e.origin[1] + h, e.origin[2]);
		VectorSet(quad[1], e.origin[0] + h, e.origin[1] + h, e.origin[2]);
		VectorSet(quad[2], e.origin[0] + h, e.origin[1] - h, e.origin[2]);
		VectorSet(quad[3], e.origin[0] - h, e.origin[1] - h, e.origin[2]);
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 3; j++) {
				int k = corners[i * 3 + j];

				c[j].xyz = quad[k];
				c[j].st = st[k];
				c[j].lm = noLightmap;
				c[j].rgba = s_white;
			}
			DrcMap_AddTri(b, look, c);
		}
		numPickups++;
	}
	return numPickups;
}

static int DrcMap_CompareTris(const void *a, const void *b)
{
	const drcMapTri_t *ta = (const drcMapTri_t *)a, *tb = (const drcMapTri_t *)b;

	if (ta->key != tb->key)
		return (ta->key > tb->key) ? 1 : -1;
	return ta->look - tb->look;
}

/* Mipmaps are forced off on this port, so full-size textures turn to noise at map scale: box-filter
 * a copy down to about one texel per GamePad pixel. Sources are linear, so the CPU reads them. */
static GX2Texture *DrcMap_Downscale(const drcMapImage_t *img, float upp)
{
	GX2Texture *src = GX2BE_GetTexture((int)img->src->texnum);
	GX2Texture *tex;
	double upt;
	byte *out;
	int w, h, f = 1, outW, outH, x, y, bx, by, c;

	if (!src || !src->surface.image || src->surface.tileMode != GX2_TILE_MODE_LINEAR_ALIGNED ||
	    src->surface.format != GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8)
		return NULL;
	w = (int)src->surface.width;
	h = (int)src->surface.height;
	if (w <= 0 || h <= 0)
		return NULL;

	/* World units per texel as seen from above; 0.5 is Q3's default texture scale. */
	upt = (img->texArea > 1e-9 && img->worldArea > 1e-3) ?
	      sqrt(img->worldArea / (img->texArea * (double)w * (double)h)) : 0.5;
	while (f * 2 * upt <= upp && f * 2 <= MAX(w, h))
		f *= 2;
	outW = MAX(1, w / f);
	outH = MAX(1, h / f);

	out = malloc((size_t)outW * outH * 4);
	if (!out)
		return NULL;
	for (y = 0; y < outH; y++) {
		for (x = 0; x < outW; x++) {
			unsigned sum[4] = { 0, 0, 0, 0 }, n = 0;

			for (by = y * f; by < MIN(h, y * f + f); by++) {
				const byte *row = (const byte *)src->surface.image + (size_t)by * src->surface.pitch * 4;

				for (bx = x * f; bx < MIN(w, x * f + f); bx++) {
					for (c = 0; c < 4; c++)
						sum[c] += row[bx * 4 + c];
					n++;
				}
			}
			for (c = 0; c < 4; c++)
				out[((size_t)y * outW + x) * 4 + c] = (byte)((sum[c] + n / 2) / n);
		}
	}
	tex = GX2Image_Upload(out, outW, outH, qtrue, qfalse);
	free(out);
	return tex;
}

static void DrcMap_BuildWorld(const world_t *w)
{
	drcMapBuild_t b;
	vec3_t mins, maxs;
	float upp, cx, cy, halfW, halfH, m[16];
	int drcW, drcH, i, j, numVerts, start, numBatches, numPickups;
	gx2vert_t *v;

	start = ri.Milliseconds();
	memset(&b, 0, sizeof(b));
	for (i = 0; i < w->numsurfaces && !b.failed; i++)
		DrcMap_AddSurface(&b, &w->surfaces[i]);
	if (b.failed || !b.numTris) {
		ri.Printf(PRINT_ALL, "GamePad map: no top view for %s (%d tris%s)\n",
		          w->name, b.numTris, b.failed ? ", out of memory" : "");
		goto done;
	}

	/* Fitted to the level's surfaces only, before pickups are added. */
	ClearBounds(mins, maxs);
	for (i = 0; i < b.numTris; i++) {
		for (j = 0; j < 3; j++) {
			vec3_t p;

			VectorSet(p, b.tris[i].v[j].x, b.tris[i].v[j].y, 0.0f);
			AddPointToBounds(p, mins, maxs);
		}
	}

	/* Whole map fitted to the GamePad, north (+Y) up like the editor's top view. */
	GX2BE_GetDrcSize(&drcW, &drcH);
	upp = MAX((maxs[0] - mins[0]) / (drcW - 2.0f * DRCMAP_EDGE_PX),
	          (maxs[1] - mins[1]) / (drcH - 2.0f * DRCMAP_EDGE_PX));
	if (upp < 0.25f)
		upp = 0.25f;
	cx = (mins[0] + maxs[0]) * 0.5f;
	cy = (mins[1] + maxs[1]) * 0.5f;
	halfW = drcW * 0.5f * upp;
	halfH = drcH * 0.5f * upp;
	s_map.mins[0] = cx - halfW;
	s_map.maxs[0] = cx + halfW;
	s_map.mins[1] = cy - halfH;
	s_map.maxs[1] = cy + halfH;
	s_map.unitsPerPixel = upp;

	numPickups = DrcMap_AddPickups(&b, w, upp, s_map.gametype, s_map.teamItemsOnly);
	if (b.failed) {
		ri.Printf(PRINT_ALL, "GamePad map: out of memory adding pickups for %s\n", w->name);
		goto done;
	}

	/* Painter's order low to high: the highest surface drawn at a spot is the one seen from above. */
	qsort(b.tris, b.numTris, sizeof(*b.tris), DrcMap_CompareTris);
	numBatches = 0;
	for (i = 0; i < b.numTris; i++)
		if (!i || b.tris[i].look != b.tris[i - 1].look)
			numBatches++;

	numVerts = DRCMAP_ALIGN(6);
	for (i = 0; i < b.numTris; i = j) {
		for (j = i + 1; j < b.numTris && b.tris[j].look == b.tris[i].look; j++)
			;
		numVerts += DRCMAP_ALIGN((j - i) * 3);
	}

	s_map.verts = memalign(256, (size_t)numVerts * sizeof(gx2vert_t));
	s_map.batches = malloc((size_t)numBatches * sizeof(*s_map.batches));
	s_map.frameBatches = malloc((size_t)(numBatches + 1) * sizeof(*s_map.frameBatches));
	s_map.triKeys = malloc((size_t)b.numTris * sizeof(float));
	if (!s_map.verts || !s_map.batches || !s_map.frameBatches || !s_map.triKeys) {
		ri.Printf(PRINT_ALL, "GamePad map: out of memory for %d tris\n", b.numTris);
		DrcMap_FreeWorld();
		goto done;
	}
	memset(s_map.verts, 0, (size_t)numVerts * sizeof(gx2vert_t));

	/* Black backdrop over the whole screen, always the first draw. */
	v = s_map.verts;
	DrcMap_SetFlatVert(&v[0], s_map.mins[0], s_map.mins[1], s_black);
	DrcMap_SetFlatVert(&v[1], s_map.maxs[0], s_map.mins[1], s_black);
	DrcMap_SetFlatVert(&v[2], s_map.maxs[0], s_map.maxs[1], s_black);
	DrcMap_SetFlatVert(&v[3], s_map.mins[0], s_map.mins[1], s_black);
	DrcMap_SetFlatVert(&v[4], s_map.maxs[0], s_map.maxs[1], s_black);
	DrcMap_SetFlatVert(&v[5], s_map.mins[0], s_map.maxs[1], s_black);
	v += DRCMAP_ALIGN(6);

	/* One batch per run of equal looks in height order. */
	s_map.numBatches = 0;
	for (i = 0; i < b.numTris; i = j) {
		drcMapBatch_t *batch = &s_map.batches[s_map.numBatches++];

		for (j = i + 1; j < b.numTris && b.tris[j].look == b.tris[i].look; j++)
			;
		batch->look = b.tris[i].look;
		batch->firstTri = i;
		batch->numTris = j - i;
		batch->verts = v;
		for (; i < j; i++) {
			memcpy(v, b.tris[i].v, sizeof(b.tris[i].v));
			v += 3;
			s_map.triKeys[i] = b.tris[i].key;
		}
		v = batch->verts + DRCMAP_ALIGN(batch->numTris * 3);
	}
	s_map.numTris = b.numTris;
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER, s_map.verts,
	              (uint32_t)numVerts * sizeof(gx2vert_t));

	for (i = 0; i < b.numImages; i++)
		b.images[i].small = DrcMap_Downscale(&b.images[i], upp);
	s_map.images = b.images;
	s_map.numImages = b.numImages;
	s_map.looks = b.looks;
	b.images = NULL;
	b.looks = NULL;

	/* glOrtho(mins, maxs, z -1..1), column-major; flat z = 0 lands mid depth range. */
	memset(m, 0, sizeof(m));
	m[0]  = 2.0f / (s_map.maxs[0] - s_map.mins[0]);
	m[5]  = 2.0f / (s_map.maxs[1] - s_map.mins[1]);
	m[10] = -1.0f;
	m[12] = -(s_map.maxs[0] + s_map.mins[0]) / (s_map.maxs[0] - s_map.mins[0]);
	m[13] = -(s_map.maxs[1] + s_map.mins[1]) / (s_map.maxs[1] - s_map.mins[1]);
	m[15] = 1.0f;
	for (i = 0; i < 16; i++)
		s_mapVsBlock[i] = GX2R_SwapF32(m[i]);
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
	              s_mapVsBlock, sizeof(s_mapVsBlock));

	ri.Printf(PRINT_ALL, "GamePad map: %s, %d tris in %d batches, %d textures, %d pickups, "
	          "%.2f units/pixel, %d ms\n", w->name, s_map.numTris, s_map.numBatches, s_map.numImages,
	          numPickups, upp, ri.Milliseconds() - start);

done:
	free(b.images);
	free(b.looks);
	free(b.tris);
}

static qboolean DrcMap_EnsureWorld(void)
{
	const world_t *w = tr.world;

	if (!w)
		return qfalse;
	if (s_map.built && s_map.worldSurfaces == (const void *)w->surfaces &&
	    s_map.worldNumSurfaces == w->numsurfaces && !strcmp(s_map.worldName, w->name) &&
	    s_map.builtGametype == s_map.gametype && s_map.builtTeamItemsOnly == s_map.teamItemsOnly)
		return s_map.verts != NULL;
	if (!CON_IsForeground())
		return qfalse;

	DrcMap_FreeWorld();
	DrcMap_BuildWorld(w);
	/* Marked built even on failure so an unusable map isn't rescanned every frame. */
	s_map.built = qtrue;
	s_map.worldSurfaces = w->surfaces;
	s_map.worldNumSurfaces = w->numsurfaces;
	s_map.builtGametype = s_map.gametype;
	s_map.builtTeamItemsOnly = s_map.teamItemsOnly;
	Q_strncpyz(s_map.worldName, w->name, sizeof(s_map.worldName));
	return s_map.verts != NULL;
}

/* ======================================================================== */
/* portrait                                                                 */
/* ======================================================================== */

static void DrcMap_SplitModel(const char *in, char *name, char *skin)
{
	char *slash;

	Q_strncpyz(name, in, MAX_QPATH);
	slash = strchr(name, '/');
	if (slash) {
		*slash = 0;
		Q_strncpyz(skin, slash + 1, MAX_QPATH);
	} else {
		skin[0] = 0;
	}
	if (!skin[0])
		Q_strncpyz(skin, "default", MAX_QPATH);
}

static int DrcMap_PushPath(char (*paths)[MAX_QPATH], int n, const char *path)
{
	int i;

	if (n >= DRCMAP_MAX_ICONS)
		return n;
	for (i = 0; i < n; i++)
		if (!Q_stricmp(paths[i], path))
			return n;
	Q_strncpyz(paths[n], path, MAX_QPATH);
	return n + 1;
}

/* Same files cg_players.c registers for the scoreboard; the head model is the HUD face. */
static int DrcMap_IconPaths(const char *model, const char *headModel, int team,
                            char (*paths)[MAX_QPATH])
{
#ifdef ELITEFORCE
	const char *base = "models/players2/";
#else
	const char *base = "models/players/";
#endif
	char name[MAX_QPATH], skin[MAX_QPATH], head[MAX_QPATH], headSkin[MAX_QPATH];
	const char *teamSkin = (team == 1) ? "red" : (team == 2) ? "blue" : NULL; /* TEAM_RED/BLUE */
	const char *headDir, *folders[2];
	int n = 0, i;

	DrcMap_SplitModel(model, name, skin);
	DrcMap_SplitModel(headModel[0] ? headModel : model, head, headSkin);

	/* A '*' head lives under heads/ (Team Arena), else heads/ is only a fallback. */
	headDir = head;
	folders[0] = "";
	folders[1] = "heads/";
	if (head[0] == '*') {
		headDir = head + 1;
		folders[0] = "heads/";
		folders[1] = "";
	}

	if (headDir[0]) {
		for (i = 0; i < 2; i++) {
			n = DrcMap_PushPath(paths, n, va("%s%s%s/icon_%s.tga", base, folders[i], headDir,
			                                 teamSkin ? teamSkin : headSkin));
			n = DrcMap_PushPath(paths, n, va("%s%s%s/%s/icon_%s.tga", base, folders[i], headDir,
			                                 headSkin, teamSkin ? teamSkin : "default"));
		}
	}
	if (name[0]) {
		n = DrcMap_PushPath(paths, n, va("%s%s/icon_%s.tga", base, name, teamSkin ? teamSkin : skin));
		n = DrcMap_PushPath(paths, n, va("%s%s/icon_default.tga", base, name));
	}
	if (headDir[0])
		n = DrcMap_PushPath(paths, n, va("%s%s/icon_default.tga", base, headDir));
	return n;
}

static qboolean DrcMap_IconValid(void)
{
	return s_map.iconImage && s_map.iconIndex < tr.numImages &&
	       tr.images[s_map.iconIndex] == s_map.iconImage &&
	       !Q_stricmp(s_map.iconImage->imgName, s_map.iconName);
}

static void DrcMap_UseIcon(int index)
{
	s_map.iconIndex = index;
	s_map.iconImage = tr.images[index];
	Q_strncpyz(s_map.iconName, s_map.iconImage->imgName, sizeof(s_map.iconName));
	ri.Printf(PRINT_ALL, "GamePad map: portrait %s\n", s_map.iconName);
}

static void DrcMap_UpdateIcon(const char *model, const char *headModel, int team)
{
	char key[sizeof(s_map.iconKey)];
	char paths[DRCMAP_MAX_ICONS][MAX_QPATH];
	int numPaths, i, j;

	Com_sprintf(key, sizeof(key), "%s|%s|%d", model, headModel, team);
	if (!strcmp(key, s_map.iconKey) && (!s_map.iconImage || DrcMap_IconValid()))
		return;   /* unchanged; a remembered miss stays a miss for this key */

	Q_strncpyz(s_map.iconKey, key, sizeof(s_map.iconKey));
	s_map.iconImage = NULL;
	numPaths = DrcMap_IconPaths(model, headModel, team, paths);

	/* cgame already loaded the scoreboard icon: find it in memory before any SD lookup. */
	for (i = 0; i < numPaths; i++) {
		for (j = 0; j < tr.numImages; j++) {
			if (!Q_stricmp(tr.images[j]->imgName, paths[i])) {
				DrcMap_UseIcon(j);
				return;
			}
		}
	}

	/* Slow path (several IOSU file probes): runs once per key change, never per frame. */
	for (i = 0; i < numPaths; i++) {
		image_t *img = R_FindImageFile(paths[i], IMGTYPE_COLORALPHA, IMGFLAG_CLAMPTOEDGE);

		if (!img)
			continue;
		for (j = 0; j < tr.numImages; j++) {
			if (tr.images[j] == img) {
				DrcMap_UseIcon(j);
				return;
			}
		}
	}
	ri.Printf(PRINT_ALL, "GamePad map: no portrait for \"%s\", using a plain marker\n", key);
}

/* ======================================================================== */
/* per-frame entry points                                                   */
/* ======================================================================== */

void GX2DrcMap_SetView(const vec3_t vieworg)
{
	/* The first world scene of a frame is the player's view; HUD models never have a world. */
	if (s_map.viewValid)
		return;
	VectorCopy(vieworg, s_map.viewOrg);
	s_map.viewValid = qtrue;
}

void GX2DrcMap_Prepare(int gametype, qboolean teamItemsOnly)
{
	s_map.gametype = gametype;
	s_map.teamItemsOnly = teamItemsOnly;
	if (tr.registered)
		DrcMap_EnsureWorld();
}

void GX2DrcMap_SetClientFrame(qboolean active, const char *model, const char *headModel, int team)
{
	s_map.clientActive = qfalse;
	if (!active || !tr.registered || !DrcMap_EnsureWorld())
		return;
	DrcMap_UpdateIcon(model ? model : "", headModel ? headModel : "", team);
	s_map.clientActive = qtrue;
}

qboolean GX2DrcMap_GetFrame(gx2drcmapframe_t *out)
{
	qboolean ready = s_map.clientActive && s_map.viewValid && s_map.verts;
	const byte *rgba = s_markColor;
	gx2drcmapbatch_t *fb;
	GX2Texture *white;
	float limit, h, cx, cy;
	int lo, hi, i, n;

#ifdef WIIU_DEBUG_LOG
	{
		static int lastState = -1;
		int state = (s_map.clientActive ? 1 : 0) | (s_map.viewValid ? 2 : 0) | (s_map.verts ? 4 : 0);

		if (state != lastState) {
			lastState = state;
			ri.Printf(PRINT_ALL, "GamePad map: frame client=%d view=%d plan=%d\n",
			          state & 1, (state >> 1) & 1, (state >> 2) & 1);
		}
	}
#endif

	s_map.clientActive = qfalse;
	s_map.viewValid = qfalse;
	if (!ready)
		return qfalse;

	/* Surfaces at or below the player only, so an upper level never covers the one you are on. */
	limit = s_map.viewOrg[2] + DRCMAP_SLICE_MARGIN;
	lo = 0;
	hi = s_map.numTris;
	while (lo < hi) {
		int mid = (lo + hi) >> 1;

		if (s_map.triKeys[mid] <= limit)
			lo = mid + 1;
		else
			hi = mid;
	}

	fb = s_map.frameBatches;
	memset(&fb[0], 0, sizeof(fb[0]));
	fb[0].verts = s_map.verts;
	fb[0].numVerts = 6;
	n = 1;

	/* Lightmaps are looked up per frame: their texnums change whenever the renderer restarts. */
	white = GX2BE_GetTexture((int)tr.whiteImage->texnum);
	for (i = 0; i < s_map.numBatches && s_map.batches[i].firstTri < lo; i++) {
		const drcMapBatch_t *batch = &s_map.batches[i];
		const drcMapLook_t *look = &s_map.looks[batch->look];
		const drcMapImage_t *img = &s_map.images[look->image];

		fb[n].verts = batch->verts;
		fb[n].numVerts = (uint32_t)MIN(batch->numTris, lo - batch->firstTri) * 3;
		fb[n].baseTex = img->small ? img->small : white;
		fb[n].lightmap = NULL;
		if (look->lightmap >= 0 && look->lightmap < tr.numLightmaps && tr.lightmaps[look->lightmap])
			fb[n].lightmap = GX2BE_GetTexture((int)tr.lightmaps[look->lightmap]->texnum);
		fb[n].clamp = img->clamp;
		fb[n].atest = look->atest;
		fb[n].blend = look->blend;
		n++;
	}

	out->batches = fb;
	out->numBatches = n;
	out->vsBlock = s_mapVsBlock;
	out->iconTexnum = DrcMap_IconValid() ? (int)s_map.iconImage->texnum : 0;
	if (out->iconTexnum)
		rgba = s_white;

	/* Clamped so the portrait stays on screen even when noclipping off the map. */
	h = DRCMAP_ICON_PX * 0.5f * s_map.unitsPerPixel;
	cx = Com_Clamp(s_map.mins[0] + h, s_map.maxs[0] - h, s_map.viewOrg[0]);
	cy = Com_Clamp(s_map.mins[1] + h, s_map.maxs[1] - h, s_map.viewOrg[1]);

	DrcMap_SetFlatVert(&out->icon[0], cx - h, cy + h, rgba);
	DrcMap_SetFlatVert(&out->icon[1], cx + h, cy + h, rgba);
	DrcMap_SetFlatVert(&out->icon[2], cx + h, cy - h, rgba);
	DrcMap_SetFlatVert(&out->icon[3], cx - h, cy - h, rgba);
	out->icon[1].s0 = 1.0f;
	out->icon[2].s0 = 1.0f;
	out->icon[2].t0 = 1.0f;
	out->icon[3].t0 = 1.0f;
	return qtrue;
}

#endif /* WIIU_GX2BE */
