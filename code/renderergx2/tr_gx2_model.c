/* MD3 model loader (own parse, ported byte-swap/lerp/bounds math from
 * renderergl1). MD3 only, no MDR/IQM -- getting even this far took forever. */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "../qcommon/qfiles.h"
#include "tr_gx2_local.h"

#define GX2_MAX_MODELS 256

typedef struct {
	qboolean     inuse;
	md3Header_t *md3; /* owned buffer, byte-swapped in place */
} gx2model_t;

static gx2model_t s_models[GX2_MAX_MODELS];
static int        s_numModels = 0;

static gx2model_t *GX2Model_Get(qhandle_t handle)
{
	int idx = (int)handle - 1;
	if (idx < 0 || idx >= s_numModels || !s_models[idx].inuse)
		return NULL;
	return &s_models[idx];
}

/* .skin registry, port of renderergl1's RE_RegisterSkin. */

#define GX2_MAX_SKINS         256
#define GX2_MAX_SKIN_SURFACES 32

typedef struct {
	char      name[MAX_QPATH]; /* md3 surface name, lowercase */
	qhandle_t texHandle;
} gx2skinsurf_t;

typedef struct {
	char          name[MAX_QPATH];
	int           numSurfaces;
	gx2skinsurf_t surfaces[GX2_MAX_SKIN_SURFACES];
} gx2skin_t;

static gx2skin_t s_skins[GX2_MAX_SKINS];
static int       s_numSkins = 0;

/* Like COM_Parse but tokens also break on commas (.skin file format). */
static char *GX2Skin_CommaParse(char **data_p)
{
	int c = 0, len = 0;
	char *data = *data_p;
	static char com_token[MAX_TOKEN_CHARS];

	com_token[0] = 0;

	if (!data) {
		*data_p = NULL;
		return com_token;
	}

	for (;;) {
		while ((c = *data) <= ' ') { /* skip whitespace */
			if (!c)
				break;
			data++;
		}

		c = *data;

		if (c == '/' && data[1] == '/') { /* skip // comment */
			data += 2;
			while (*data && *data != '\n')
				data++;
		} else if (c == '/' && data[1] == '*') { /* skip block comment */
			data += 2;
			while (*data && (*data != '*' || data[1] != '/'))
				data++;
			if (*data)
				data += 2;
		} else {
			break;
		}
	}

	if (c == 0)
		return "";

	if (c == '\"') { /* quoted strings */
		data++;
		for (;;) {
			c = *data++;
			if (c == '\"' || !c) {
				com_token[len] = 0;
				*data_p = data;
				return com_token;
			}
			if (len < MAX_TOKEN_CHARS - 1)
				com_token[len++] = (char)c;
		}
	}

	do { /* regular word, breaking on comma */
		if (len < MAX_TOKEN_CHARS - 1)
			com_token[len++] = (char)c;
		data++;
		c = *data;
	} while (c > 32 && c != ',');

	com_token[len] = 0;
	*data_p = data;
	return com_token;
}

/* Returns 0 for missing/unreadable/surface-less .skin (legal fallback, unlike model/shader handles). */
qhandle_t GX2Skin_Register(const char *name)
{
	gx2skin_t *skin;
	union {
		char *c;
		void *v;
	} text;
	char  *text_p, *token;
	char   surfName[MAX_QPATH];
	size_t nameLen;
	int    i;

	if (!name || !name[0] || strlen(name) >= MAX_QPATH) {
		ri.Printf(PRINT_ALL, "^1GX2Skin_Register: bad skin name\n");
		return 0;
	}

	for (i = 0; i < s_numSkins; i++) {
		if (!Q_stricmp(s_skins[i].name, name))
			return (qhandle_t)(i + 1);
	}

	if (s_numSkins >= GX2_MAX_SKINS) {
		ri.Printf(PRINT_ALL, "^1GX2Skin_Register: skin table full (%d), dropping %s\n",
		          GX2_MAX_SKINS, name);
		return 0;
	}

	skin = &s_skins[s_numSkins];
	memset(skin, 0, sizeof(*skin));
	Q_strncpyz(skin->name, name, sizeof(skin->name));

	/* Not a .skin file: single-shader skin, empty-name surface entry. */
	nameLen = strlen(name);
	if (nameLen < 5 || strcmp(name + nameLen - 5, ".skin")) {
		skin->numSurfaces = 1;
		skin->surfaces[0].name[0] = '\0';
		skin->surfaces[0].texHandle = GX2Image_Register(name);
		s_numSkins++;
		return (qhandle_t)s_numSkins;
	}

	if (ri.FS_ReadFile(name, &text.v) <= 0 || !text.c)
		return 0;

	text_p = text.c;
	while (text_p && *text_p) { /* get surface name */
		token = GX2Skin_CommaParse(&text_p);
		if (!token[0])
			break;
		Q_strncpyz(surfName, token, sizeof(surfName));
		Q_strlwr(surfName);

		if (*text_p == ',')
			text_p++;

		if (strstr(token, "tag_"))
			continue;

		token = GX2Skin_CommaParse(&text_p); /* parse the shader name */

		if (skin->numSurfaces >= GX2_MAX_SKIN_SURFACES) {
			ri.Printf(PRINT_ALL, "^3GX2Skin_Register: %s has too many surfaces\n", name);
			break;
		}
		Q_strncpyz(skin->surfaces[skin->numSurfaces].name, surfName,
		           sizeof(skin->surfaces[0].name));
		skin->surfaces[skin->numSurfaces].texHandle = GX2Image_Register(token);
		skin->numSurfaces++;
	}

	ri.FS_FreeFile(text.v);

	if (skin->numSurfaces == 0)
		return 0; /* must have at least 1 surface */

	s_numSkins++;
	return (qhandle_t)s_numSkins;
}

/* 0 = no entry for the surface; caller falls back to embedded shader list. */
static qhandle_t GX2Skin_TextureForSurface(qhandle_t hSkin, const char *surfName)
{
	const gx2skin_t *skin;
	int i;

	if (hSkin < 1 || hSkin > s_numSkins)
		return 0;
	skin = &s_skins[hSkin - 1];
	for (i = 0; i < skin->numSurfaces; i++) {
		if (!strcmp(skin->surfaces[i].name, surfName))
			return skin->surfaces[i].texHandle;
	}
	return 0;
}

/* In-place endian swap of every MD3 field (real work on this big-endian PPC),
 * plus shader-name -> texture handle resolve stashed into shaderIndex. */
static void GX2Model_SwapAndResolve(md3Header_t *md3, const char *name)
{
	int            i, j;
	md3Frame_t    *frame;
	md3Tag_t      *tag;
	md3Surface_t  *surf;

	md3->ident        = LittleLong(md3->ident);
	md3->version      = LittleLong(md3->version);
	md3->numFrames    = LittleLong(md3->numFrames);
	md3->numTags      = LittleLong(md3->numTags);
	md3->numSurfaces  = LittleLong(md3->numSurfaces);
	md3->numSkins     = LittleLong(md3->numSkins);
	md3->ofsFrames    = LittleLong(md3->ofsFrames);
	md3->ofsTags      = LittleLong(md3->ofsTags);
	md3->ofsSurfaces  = LittleLong(md3->ofsSurfaces);
	md3->ofsEnd       = LittleLong(md3->ofsEnd);

	frame = (md3Frame_t *)((byte *)md3 + md3->ofsFrames);
	for (i = 0; i < md3->numFrames; i++, frame++) {
		frame->radius = LittleFloat(frame->radius);
		for (j = 0; j < 3; j++) {
			frame->bounds[0][j] = LittleFloat(frame->bounds[0][j]);
			frame->bounds[1][j] = LittleFloat(frame->bounds[1][j]);
			frame->localOrigin[j] = LittleFloat(frame->localOrigin[j]);
		}
	}

	tag = (md3Tag_t *)((byte *)md3 + md3->ofsTags);
	for (i = 0; i < md3->numTags * md3->numFrames; i++, tag++) {
		for (j = 0; j < 3; j++) {
			tag->origin[j]  = LittleFloat(tag->origin[j]);
			tag->axis[0][j] = LittleFloat(tag->axis[0][j]);
			tag->axis[1][j] = LittleFloat(tag->axis[1][j]);
			tag->axis[2][j] = LittleFloat(tag->axis[2][j]);
		}
	}

	surf = (md3Surface_t *)((byte *)md3 + md3->ofsSurfaces);
	for (i = 0; i < md3->numSurfaces; i++) {
		md3Shader_t    *shader;
		md3St_t        *st;
		md3XyzNormal_t *xyz;
		md3Triangle_t  *tri;
		int             numXyz;

		surf->ident         = LittleLong(surf->ident);
		surf->flags         = LittleLong(surf->flags);
		surf->numFrames     = LittleLong(surf->numFrames);
		surf->numShaders    = LittleLong(surf->numShaders);
		surf->numTriangles  = LittleLong(surf->numTriangles);
		surf->ofsTriangles  = LittleLong(surf->ofsTriangles);
		surf->numVerts      = LittleLong(surf->numVerts);
		surf->ofsShaders    = LittleLong(surf->ofsShaders);
		surf->ofsSt         = LittleLong(surf->ofsSt);
		surf->ofsXyzNormals = LittleLong(surf->ofsXyzNormals);
		surf->ofsEnd        = LittleLong(surf->ofsEnd);

		/* lowercase surface name and strip trailing "_1"/"_2" LOD suffix */
		Q_strlwr(surf->name);
		j = (int)strlen(surf->name);
		if (j > 2 && surf->name[j - 2] == '_')
			surf->name[j - 2] = 0;

		if (surf->numVerts > MD3_MAX_VERTS || surf->numTriangles > MD3_MAX_TRIANGLES) {
			ri.Printf(PRINT_ALL,
				"^1GX2Model_Register: %s surface %s exceeds MD3 limits, skipping model\n",
				name, surf->name[0] ? surf->name : "?");
			/* left as-is; GX2Model_Draw re-checks limits and no-ops instead of overflowing */
		}

		/* resolve every embedded shader entry -- skinNum indexes into this list */
		shader = (md3Shader_t *)((byte *)surf + surf->ofsShaders);
		for (j = 0; j < surf->numShaders; j++, shader++)
			shader->shaderIndex = (int)GX2Image_Register(shader->name);

		tri = (md3Triangle_t *)((byte *)surf + surf->ofsTriangles);
		for (j = 0; j < surf->numTriangles; j++, tri++) {
			tri->indexes[0] = LittleLong(tri->indexes[0]);
			tri->indexes[1] = LittleLong(tri->indexes[1]);
			tri->indexes[2] = LittleLong(tri->indexes[2]);
		}

		st = (md3St_t *)((byte *)surf + surf->ofsSt);
		for (j = 0; j < surf->numVerts; j++, st++) {
			st->st[0] = LittleFloat(st->st[0]);
			st->st[1] = LittleFloat(st->st[1]);
		}

		numXyz = surf->numVerts * surf->numFrames;
		xyz = (md3XyzNormal_t *)((byte *)surf + surf->ofsXyzNormals);
		for (j = 0; j < numXyz; j++, xyz++) {
			xyz->xyz[0] = LittleShort(xyz->xyz[0]);
			xyz->xyz[1] = LittleShort(xyz->xyz[1]);
			xyz->xyz[2] = LittleShort(xyz->xyz[2]);
			xyz->normal = LittleShort(xyz->normal);
		}

		surf = (md3Surface_t *)((byte *)surf + surf->ofsEnd);
	}
}

/* Never returns 0 (cgame Com_Errors on that) -- bad files get a slot with
 * md3==NULL, treated as "draw nothing" everywhere else in this file. */
qhandle_t GX2Model_Register(const char *name)
{
	void         *buf;
	long          len;
	md3Header_t  *hdr;
	int           size;
	gx2model_t   *m;
	int           idx;

	if (s_numModels >= GX2_MAX_MODELS) {
		ri.Printf(PRINT_ALL, "^1GX2Model_Register: model table full (%d), dropping %s\n",
		          GX2_MAX_MODELS, name);
		return 1;
	}
	idx = s_numModels++;
	m = &s_models[idx];
	memset(m, 0, sizeof(*m));
	m->inuse = qtrue;

	len = ri.FS_ReadFile(name, &buf);
	if (!buf || len <= 0) {
		ri.Printf(PRINT_ALL, "^1GX2Model_Register: can't read %s\n", name);
		return (qhandle_t)(idx + 1);
	}

	hdr = (md3Header_t *)buf;
	if (LittleLong(hdr->ident) != MD3_IDENT || LittleLong(hdr->version) != MD3_VERSION) {
		ri.Printf(PRINT_ALL, "^1GX2Model_Register: %s is not a valid MD3 (MDR/IQM not supported)\n", name);
		ri.FS_FreeFile(buf);
		return (qhandle_t)(idx + 1);
	}

	size = LittleLong(hdr->ofsEnd);
	if (size <= 0) {
		ri.Printf(PRINT_ALL, "^1GX2Model_Register: %s has bad ofsEnd\n", name);
		ri.FS_FreeFile(buf);
		return (qhandle_t)(idx + 1);
	}

	m->md3 = (md3Header_t *)malloc((size_t)size);
	if (!m->md3) {
		ri.Printf(PRINT_ALL, "^1GX2Model_Register: alloc failed for %s (%d bytes)\n", name, size);
		ri.FS_FreeFile(buf);
		return (qhandle_t)(idx + 1);
	}
	memcpy(m->md3, buf, (size_t)size);
	ri.FS_FreeFile(buf);

	GX2Model_SwapAndResolve(m->md3, name);

	if (m->md3->numFrames < 1) {
		ri.Printf(PRINT_ALL, "^1GX2Model_Register: %s has no frames\n", name);
		free(m->md3);
		m->md3 = NULL;
	}

	return (qhandle_t)(idx + 1);
}

static md3Tag_t *GX2Model_FindTag(md3Header_t *md3, int frame, const char *tagName)
{
	md3Tag_t *tag;
	int       i;

	if (frame < 0) frame = 0;
	if (frame >= md3->numFrames) frame = md3->numFrames - 1;

	tag = (md3Tag_t *)((byte *)md3 + md3->ofsTags) + (size_t)frame * md3->numTags;
	for (i = 0; i < md3->numTags; i++, tag++) {
		if (!strcmp(tag->name, tagName))
			return tag;
	}
	return NULL;
}

/* Ported from renderergl1's R_LerpTag (MD3-only branch). */
qboolean GX2Model_LerpTag(orientation_t *tag, qhandle_t handle, int startFrame, int endFrame,
                          float frac, const char *tagName)
{
	gx2model_t *m = GX2Model_Get(handle);
	md3Tag_t   *start, *end;
	float       frontLerp, backLerp;
	int         i;

	if (!m || !m->md3) {
		AxisClear(tag->axis);
		VectorClear(tag->origin);
		return qfalse;
	}

	start = GX2Model_FindTag(m->md3, startFrame, tagName);
	end   = GX2Model_FindTag(m->md3, endFrame, tagName);
	if (!start || !end) {
		AxisClear(tag->axis);
		VectorClear(tag->origin);
		return qfalse;
	}

	frontLerp = frac;
	backLerp = 1.0f - frac;
	for (i = 0; i < 3; i++) {
		tag->origin[i]  = start->origin[i]  * backLerp + end->origin[i]  * frontLerp;
		tag->axis[0][i] = start->axis[0][i] * backLerp + end->axis[0][i] * frontLerp;
		tag->axis[1][i] = start->axis[1][i] * backLerp + end->axis[1][i] * frontLerp;
		tag->axis[2][i] = start->axis[2][i] * backLerp + end->axis[2][i] * frontLerp;
	}
	VectorNormalize(tag->axis[0]);
	VectorNormalize(tag->axis[1]);
	VectorNormalize(tag->axis[2]);
	return qtrue;
}

/* Ported from renderergl1's R_ModelBounds MOD_MESH case (frame-0 bounds). */
void GX2Model_Bounds(qhandle_t handle, vec3_t mins, vec3_t maxs)
{
	gx2model_t *m = GX2Model_Get(handle);
	md3Frame_t *frame;

	if (!m || !m->md3) {
		VectorClear(mins);
		VectorClear(maxs);
		return;
	}

	frame = (md3Frame_t *)((byte *)m->md3 + m->md3->ofsFrames);
	VectorCopy(frame->bounds[0], mins);
	VectorCopy(frame->bounds[1], maxs);
}

/* Byte-angle sin/cos tables for packed MD3 lat/lng normal decode, built once. */
static float    s_byteSin[256];
static float    s_byteCos[256];
static qboolean s_byteTablesBuilt = qfalse;

static void GX2Model_BuildByteTables(void)
{
	int i;

	for (i = 0; i < 256; i++) {
		float a = (float)i * (2.0f * (float)M_PI / 256.0f);
		s_byteSin[i] = sinf(a);
		s_byteCos[i] = cosf(a);
	}
	s_byteTablesBuilt = qtrue;
}

/* Decode packed md3XyzNormal_t.normal into a unit vector. */
static void GX2Model_DecodeNormal(short packed, vec3_t out)
{
	int lat = (packed >> 8) & 0xff;
	int lng = packed & 0xff;

	out[0] = s_byteCos[lat] * s_byteSin[lng];
	out[1] = s_byteSin[lat] * s_byteSin[lng];
	out[2] = s_byteCos[lng];
}

/* Builds a lerped vertex/index scratch buffer per surface and hands it to
 * `emit`. params may be NULL (flat white); took a while to get lerp+lighting right. */
qboolean GX2Model_Draw(qhandle_t handle, int frame, int oldframe, float backlerp,
                       const gx2ModelDrawParams_t *params,
                       gx2ModelSurfaceFn emit, void *ctx)
{
	static gx2vert_t  scratchVerts[MD3_MAX_VERTS];
	static uint32_t   scratchIndices[MD3_MAX_TRIANGLES * 3];

	gx2model_t   *m = GX2Model_Get(handle);
	md3Header_t  *md3;
	md3Surface_t *surf;
	int           s;
	float         frontLerp, backW;
	qboolean      lit;

	if (!m || !m->md3)
		return qfalse;
	md3 = m->md3;

	lit = (params && params->lit) ? qtrue : qfalse;
	if (lit && !s_byteTablesBuilt)
		GX2Model_BuildByteTables();

	if (frame < 0) frame = 0;
	if (frame >= md3->numFrames) frame = md3->numFrames - 1;
	if (oldframe < 0) oldframe = 0;
	if (oldframe >= md3->numFrames) oldframe = md3->numFrames - 1;

	frontLerp = 1.0f - backlerp;
	backW = backlerp;

	surf = (md3Surface_t *)((byte *)md3 + md3->ofsSurfaces);
	for (s = 0; s < md3->numSurfaces; s++) {
		md3Shader_t    *shaders = (md3Shader_t *)((byte *)surf + surf->ofsShaders);
		md3St_t        *st      = (md3St_t *)((byte *)surf + surf->ofsSt);
		md3XyzNormal_t *xyzBase = (md3XyzNormal_t *)((byte *)surf + surf->ofsXyzNormals);
		md3Triangle_t  *tris    = (md3Triangle_t *)((byte *)surf + surf->ofsTriangles);
		md3XyzNormal_t *xyzNew, *xyzOld;
		qhandle_t       texHandle;
		int             v, t;

		if (surf->numVerts > MD3_MAX_VERTS || surf->numTriangles > MD3_MAX_TRIANGLES) {
			surf = (md3Surface_t *)((byte *)surf + surf->ofsEnd);
			continue; /* already logged at registration time */
		}

		xyzNew = xyzBase + (size_t)frame * surf->numVerts;
		xyzOld = xyzBase + (size_t)oldframe * surf->numVerts;

		/* texture precedence: customSkin match, else skinNum-selected embedded shader */
		texHandle = 0;
		if (params && params->customSkin)
			texHandle = GX2Skin_TextureForSurface(params->customSkin, surf->name);
		if (!texHandle && surf->numShaders > 0) {
			int sel = params ? params->skinNum : 0;
			if (sel < 0)
				sel = 0;
			texHandle = (qhandle_t)shaders[sel % surf->numShaders].shaderIndex;
		}
		if (!texHandle)
			texHandle = GX2IMAGE_WHITE;

		for (v = 0; v < surf->numVerts; v++) {
			gx2vert_t *ov = &scratchVerts[v];
			ov->x = (xyzNew[v].xyz[0] * frontLerp + xyzOld[v].xyz[0] * backW) * (float)MD3_XYZ_SCALE;
			ov->y = (xyzNew[v].xyz[1] * frontLerp + xyzOld[v].xyz[1] * backW) * (float)MD3_XYZ_SCALE;
			ov->z = (xyzNew[v].xyz[2] * frontLerp + xyzOld[v].xyz[2] * backW) * (float)MD3_XYZ_SCALE;
			ov->s0 = st[v].st[0];
			ov->t0 = st[v].st[1];
			ov->s1 = 0.0f;
			ov->t1 = 0.0f;

			if (lit) {
				vec3_t normal;
				float  incoming;
				int    ch;

				GX2Model_DecodeNormal(xyzNew[v].normal, normal);
				if (backW != 0.0f && xyzOld != xyzNew) {
					vec3_t oldNormal;
					float  lenSq;

					GX2Model_DecodeNormal(xyzOld[v].normal, oldNormal);
					normal[0] = normal[0] * frontLerp + oldNormal[0] * backW;
					normal[1] = normal[1] * frontLerp + oldNormal[1] * backW;
					normal[2] = normal[2] * frontLerp + oldNormal[2] * backW;
					lenSq = DotProduct(normal, normal);
					if (lenSq > 0.0f) {
						float inv = 1.0f / sqrtf(lenSq);
						VectorScale(normal, inv, normal);
					}
				}

				/* back-facing gets ambient only, front-facing adds directed light */
				incoming = DotProduct(normal, params->lightDir);
				for (ch = 0; ch < 3; ch++) {
					float c = params->ambientLight[ch];
					if (incoming > 0.0f)
						c += incoming * params->directedLight[ch];
					if (c > 255.0f)
						c = 255.0f;
					ov->rgba[ch] = (byte)c;
				}
				ov->rgba[3] = 255;
			} else {
				ov->rgba[0] = ov->rgba[1] = ov->rgba[2] = ov->rgba[3] = 255;
			}
		}

		for (t = 0; t < surf->numTriangles; t++) {
			scratchIndices[t * 3 + 0] = (uint32_t)tris[t].indexes[0];
			scratchIndices[t * 3 + 1] = (uint32_t)tris[t].indexes[1];
			scratchIndices[t * 3 + 2] = (uint32_t)tris[t].indexes[2];
		}

		emit(ctx, texHandle, scratchVerts, (uint32_t)surf->numVerts,
		     scratchIndices, (uint32_t)surf->numTriangles * 3);

		surf = (md3Surface_t *)((byte *)surf + surf->ofsEnd);
	}

	return qtrue;
}
