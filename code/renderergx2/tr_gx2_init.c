#ifdef __WIIU__
#include <whb/log.h>
#include <coreinit/thread.h>
#endif
/*
 * ioQuake3-U — Wii U native (wut / GX2) port
 * code/renderergx2/tr_gx2_init.c -- GX2/WHBGfx display bring-up + the 2D quad pipeline.
 * Per-frame vertex ring records quads, then draws TV/DRC through the ubershader.
 * Uniform words are byte-swapped LE for raw GPU fetch; vertex attribs aren't (fetch
 * shader already does that swap) -- naturally, the two halves disagree.
 */

#include <string.h>
#include <malloc.h>
#include <stddef.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "tr_gx2_local.h"

#include <whb/gfx.h>
#include <gx2/clear.h>
#include <gx2/context.h>
#include <gx2/draw.h>
#include <gx2/enum.h>
#include <gx2/mem.h>
#include <gx2/registers.h>
#include <gx2/sampler.h>
#include <gx2/shaders.h>
#include <gx2/surface.h>
#include <gx2/texture.h>

static qboolean gx2r_inited = qfalse;        /* WHBGfx display up */
static qboolean gx2r_pipelineReady = qfalse; /* shaders compiled, 2D path live */

/* Stock Q3 r_fastsky: skip skybox draw, clear color shows through sky openings. */
static cvar_t *r_fastsky;
static cvar_t *r_gamma;

/* con_wiiu.c's single source of truth for ProcUI foreground state. */
extern qboolean CON_IsForeground(void);

/* ---- 2D quad ring ---------------------------------------------------- */

#define GX2_MAX_QUADS 4096

static gx2vert_t *s_verts;       /* memalign(256), GX2_MAX_QUADS * 4 */
static qhandle_t  s_quadTex[GX2_MAX_QUADS];
/* Per-quad blend override; -1 = frame default (src-alpha), else from a .shader stage. */
static int8_t     s_quadBlendSrc[GX2_MAX_QUADS];
static int8_t     s_quadBlendDst[GX2_MAX_QUADS];
static int        s_numQuads;
static int        s_ringOverflowWarned;

static float s_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

/* ---- pipeline objects ------------------------------------------------- */

static WHBGfxShaderGroup s_shaderGroup;
static GX2Sampler        s_sampler;     /* clamp -- 2D/HUD quads (st always 0..1) */
static GX2Sampler        s_samplerWrap; /* repeat -- world/entity base textures (BSP st tiles past 0..1) */

static uint32_t s_vsBlockLoc;    /* "VSBlock" binding from VS reflection */
static uint32_t s_psBlockLoc;    /* "PSBlock" binding from PS reflection */
static uint32_t s_gammaBlockLoc; /* "GammaBlock" binding from PS reflection */
static uint32_t s_baseSamplerLoc;/* "s_Base" location from PS reflection */
static uint32_t s_lmSamplerLoc;  /* "s_Lightmap" location */

/* 256-byte aligned because real hw enforces UBO base alignment where Cemu just
 * shrugged -- a 64-aligned block "worked" in Cemu and broke every draw on hardware. */
static uint32_t s_vsBlock[16] __attribute__((aligned(256)));  /* mat4 ortho   */
static uint32_t s_psBlock[8]  __attribute__((aligned(256)));  /* mode, aref   */

/* Set once per frame in GX2R_EndFrame; x = 1.0/r_gamma. */
static uint32_t s_gammaBlock[4] __attribute__((aligned(256)));

/* GX2R_SwapF32 lives in tr_gx2_local.h (shared with tr_gx2_world.c). */

/* MEM1-residency numbers for the eventual dedicated 3D render targets. */
static void GX2R_LogPlannedSurface(void)
{
	GX2Surface color;
	GX2Surface depth;

	memset(&color, 0, sizeof(color));
	color.dim = GX2_SURFACE_DIM_TEXTURE_2D;
	color.width = 1280;
	color.height = 720;
	color.depth = 1;
	color.mipLevels = 1;
	color.format = GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8;
	color.aa = GX2_AA_MODE1X;
	color.use = GX2_SURFACE_USE_TEXTURE_COLOR_BUFFER_TV;
	color.tileMode = GX2_TILE_MODE_DEFAULT;
	GX2CalcSurfaceSizeAndAlignment(&color);

	memset(&depth, 0, sizeof(depth));
	depth.dim = GX2_SURFACE_DIM_TEXTURE_2D;
	depth.width = 1280;
	depth.height = 720;
	depth.depth = 1;
	depth.mipLevels = 1;
	depth.format = GX2_SURFACE_FORMAT_FLOAT_D24_S8;
	depth.aa = GX2_AA_MODE1X;
	depth.use = GX2_SURFACE_USE_DEPTH_BUFFER;
	depth.tileMode = GX2_TILE_MODE_DEFAULT;
	GX2CalcSurfaceSizeAndAlignment(&depth);

	ri.Printf(PRINT_ALL,
		"GX2R: planned 1280x720 RGBA8 color target: size=%u align=%u\n",
		(unsigned)color.imageSize, (unsigned)color.alignment);
	ri.Printf(PRINT_ALL,
		"GX2R: planned 1280x720 D24S8 depth target: size=%u align=%u\n",
		(unsigned)depth.imageSize, (unsigned)depth.alignment);
}

/* Ortho: glConfig space (0..w right, 0..h down) -> NDC, column-major, LE-swapped. */
static void GX2R_BuildUniformBlocks(void)
{
	float w = (float)glConfig.vidWidth;
	float h = (float)glConfig.vidHeight;
	float m[16];
	int i;

	if (w <= 0.0f) w = 1280.0f;
	if (h <= 0.0f) h = 720.0f;

	memset(m, 0, sizeof(m));
	m[0]  = 2.0f / w;    /* col0.x */
	m[5]  = -2.0f / h;   /* col1.y (y down -> NDC y up) */
	m[10] = 1.0f;        /* col2.z (z passthrough; 2D uses z=0) */
	m[12] = -1.0f;       /* col3.x */
	m[13] = 1.0f;        /* col3.y */
	m[15] = 1.0f;        /* col3.w */

	for (i = 0; i < 16; i++)
		s_vsBlock[i] = GX2R_SwapF32(m[i]);

	/* u_Mode = (tex0On=1, tex1On=0, tex1Blend=0, atest=0); u_AlphaRef.x=0.5 */
	s_psBlock[0] = GX2R_SwapF32(1.0f);
	s_psBlock[1] = GX2R_SwapF32(0.0f);
	s_psBlock[2] = GX2R_SwapF32(0.0f);
	s_psBlock[3] = GX2R_SwapF32(0.0f);
	s_psBlock[4] = GX2R_SwapF32(0.5f);
	s_psBlock[5] = GX2R_SwapF32(0.0f);
	s_psBlock[6] = GX2R_SwapF32(0.0f);
	s_psBlock[7] = GX2R_SwapF32(0.0f);

	GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
	              s_vsBlock, sizeof(s_vsBlock));
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
	              s_psBlock, sizeof(s_psBlock));
}

/* No hw gamma ramp exists here, so r_gamma is a per-pixel pow(color, 1/gamma) instead. */
static void GX2R_UpdateGammaBlock(void)
{
	float gamma = (r_gamma && r_gamma->value > 0.0f) ? r_gamma->value : 1.0f;

	s_gammaBlock[0] = GX2R_SwapF32(1.0f / gamma);
	s_gammaBlock[1] = 0;
	s_gammaBlock[2] = 0;
	s_gammaBlock[3] = 0;

	GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
	              s_gammaBlock, sizeof(s_gammaBlock));
}

/* ---- 3D world draw: reuses the 2D ring's shader/layout, only the blocks + GX2 state differ. ---- */

static uint32_t s_worldVsBlock[16] __attribute__((aligned(256))); /* mat4 mvp */
static uint32_t s_skyVsBlock[16]   __attribute__((aligned(256))); /* rotation-only mvp */
static qboolean s_worldPending = qfalse;
static qboolean s_skyPending = qfalse;

void GX2R_SetWorldMVP(const float *mvp16)
{
	int i;

	for (i = 0; i < 16; i++)
		s_worldVsBlock[i] = GX2R_SwapF32(mvp16[i]);

	GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
	              s_worldVsBlock, sizeof(s_worldVsBlock));

	s_worldPending = qtrue;
}

void GX2R_SetSkyMVP(const float *mvp16)
{
	int i;

	for (i = 0; i < 16; i++)
		s_skyVsBlock[i] = GX2R_SwapF32(mvp16[i]);

	GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
	              s_skyVsBlock, sizeof(s_skyVsBlock));

	s_skyPending = qtrue;
}

/* Camera-locked skybox, drawn first with depth off; world draws over it except sky openings. CLAMP sampler avoids face-edge seams. */
static void GX2R_DrawSky(void)
{
	int face;

	if (!gx2r_pipelineReady || !s_skyPending || !GX2World_HasSky())
		return;
	if (!GX2World_GetSkyVerts())
		return;
	if (r_fastsky && r_fastsky->integer)
		return; /* stock Q3 behavior: clear color shows through instead */

	GX2SetFetchShader(&s_shaderGroup.fetchShader);
	GX2SetVertexShader(s_shaderGroup.vertexShader);
	GX2SetPixelShader(s_shaderGroup.pixelShader);
	GX2SetShaderMode(GX2_SHADER_MODE_UNIFORM_BLOCK);

	GX2SetVertexUniformBlock(s_vsBlockLoc, sizeof(s_skyVsBlock), s_skyVsBlock);
	GX2SetPixelUniformBlock(s_psBlockLoc, sizeof(s_psBlock), s_psBlock);
	GX2SetPixelUniformBlock(s_gammaBlockLoc, sizeof(s_gammaBlock), s_gammaBlock);

	GX2SetAttribBuffer(0, sizeof(gx2vert_t) * 24,
	                   sizeof(gx2vert_t), (void *)GX2World_GetSkyVerts());

	GX2SetColorControl(GX2_LOGIC_OP_COPY, 0x01, FALSE, TRUE);
	GX2SetBlendControl(GX2_RENDER_TARGET_0,
	                   GX2_BLEND_MODE_ONE, GX2_BLEND_MODE_ZERO, GX2_BLEND_COMBINE_MODE_ADD,
	                   TRUE, GX2_BLEND_MODE_ONE, GX2_BLEND_MODE_ZERO, GX2_BLEND_COMBINE_MODE_ADD);
	GX2SetDepthOnlyControl(FALSE, FALSE, GX2_COMPARE_FUNC_ALWAYS);
	GX2SetPolygonControl(GX2_FRONT_FACE_CCW, FALSE, FALSE, FALSE,
	                     GX2_POLYGON_MODE_TRIANGLE, GX2_POLYGON_MODE_TRIANGLE,
	                     FALSE, FALSE, FALSE);

	GX2SetPixelTexture(GX2Image_GetTexture(GX2IMAGE_WHITE), s_lmSamplerLoc);
	GX2SetPixelSampler(&s_sampler, s_lmSamplerLoc);

	for (face = 0; face < 6; face++) {
		GX2SetPixelTexture(GX2Image_GetTexture(GX2World_GetSkyFaceTexture(face)),
		                   s_baseSamplerLoc);
		GX2SetPixelSampler(&s_sampler, s_baseSamplerLoc); /* CLAMP mandatory */
		GX2DrawEx(GX2_PRIMITIVE_MODE_QUADS, 4, face * 4, 1);
	}
}

/* Brute force, no PVS/frustum culling. Opaque first, blended after entities so
 * translucents composite over everything -- fancy that would take another hw cycle. */
static void GX2R_DrawWorldSurfaces(qboolean blendedPass)
{
	int i, first, last;
	int boundBlendSrc = -2, boundBlendDst = -2;

	if (!gx2r_pipelineReady || !GX2World_HasWorld())
		return;

	if (blendedPass) {
		first = GX2World_GetNumOpaqueSurfaces();
		last = GX2World_GetNumSurfaces();
	} else {
		first = 0;
		last = GX2World_GetNumOpaqueSurfaces();
	}
	if (first >= last)
		return;

	GX2SetFetchShader(&s_shaderGroup.fetchShader);
	GX2SetVertexShader(s_shaderGroup.vertexShader);
	GX2SetPixelShader(s_shaderGroup.pixelShader);
	GX2SetShaderMode(GX2_SHADER_MODE_UNIFORM_BLOCK);

	GX2SetVertexUniformBlock(s_vsBlockLoc, sizeof(s_worldVsBlock), s_worldVsBlock);
	GX2SetPixelUniformBlock(s_gammaBlockLoc, sizeof(s_gammaBlock), s_gammaBlock);

	GX2SetAttribBuffer(0, sizeof(gx2vert_t) * GX2World_GetNumVerts(),
	                   sizeof(gx2vert_t), GX2World_GetVertexBuffer());

	/* No cull yet (front-face winding unverified); doubles as two-sided-shader support. */
	GX2SetColorControl(GX2_LOGIC_OP_COPY, 0x01, FALSE, TRUE);
	GX2SetPolygonControl(GX2_FRONT_FACE_CCW, FALSE, FALSE, FALSE,
	                     GX2_POLYGON_MODE_TRIANGLE, GX2_POLYGON_MODE_TRIANGLE,
	                     FALSE, FALSE, FALSE);

	if (blendedPass) {
		/* Depth-tested but not writing; no back-to-front sort (accepted for this scope). */
		GX2SetDepthOnlyControl(TRUE, FALSE, GX2_COMPARE_FUNC_LEQUAL);
	} else {
		GX2SetDepthOnlyControl(TRUE, TRUE, GX2_COMPARE_FUNC_LEQUAL);
		GX2SetBlendControl(GX2_RENDER_TARGET_0,
		                   GX2_BLEND_MODE_ONE, GX2_BLEND_MODE_ZERO, GX2_BLEND_COMBINE_MODE_ADD,
		                   TRUE, GX2_BLEND_MODE_ONE, GX2_BLEND_MODE_ZERO, GX2_BLEND_COMBINE_MODE_ADD);
	}

	/* s_Lightmap default white until a surface with a real lightmap overrides it. */
	GX2SetPixelTexture(GX2Image_GetTexture(GX2IMAGE_WHITE), s_lmSamplerLoc);
	GX2SetPixelSampler(&s_sampler, s_lmSamplerLoc);

	for (i = first; i < last; i++) {
		gx2worldsurfinfo_t info;

		GX2World_GetSurface(i, &info);
		if (!info.numIndexes || !info.psBlock)
			continue;

		if (blendedPass &&
		    (info.blendSrc != boundBlendSrc || info.blendDst != boundBlendDst)) {
			GX2SetBlendControl(GX2_RENDER_TARGET_0,
			                   (GX2BlendMode)info.blendSrc, (GX2BlendMode)info.blendDst,
			                   GX2_BLEND_COMBINE_MODE_ADD,
			                   TRUE,
			                   (GX2BlendMode)info.blendSrc, (GX2BlendMode)info.blendDst,
			                   GX2_BLEND_COMBINE_MODE_ADD);
			boundBlendSrc = info.blendSrc;
			boundBlendDst = info.blendDst;
		}

		GX2SetPixelUniformBlock(s_psBlockLoc, 32, (void *)info.psBlock);
		if (info.lmTexture) {
			GX2SetPixelTexture(info.lmTexture, s_lmSamplerLoc);
			GX2SetPixelSampler(&s_sampler, s_lmSamplerLoc); /* clamp, lightmap uv never tiles */
		}

		GX2SetPixelTexture(GX2Image_GetTexture(info.texHandle), s_baseSamplerLoc);
		GX2SetPixelSampler(&s_samplerWrap, s_baseSamplerLoc);
		GX2DrawIndexedEx(GX2_PRIMITIVE_MODE_TRIANGLES, info.numIndexes,
		                 GX2_INDEX_TYPE_U32, info.indices, 0, 1);
	}
}

/* ---- 3D entity draw: same state as world, one mvp+texture per surface, queued via
 * GX2R_AddEntityDraw and drawn after world / before 2D. ---- */

#define GX2_MAX_ENTITY_VERTS   65536
#define GX2_MAX_ENTITY_INDEXES 131072
#define GX2_MAX_ENTITY_DRAWS   1024

typedef struct {
	qhandle_t texHandle;
	uint32_t  vertOffset;  /* base vertex (GX2DrawIndexedEx's baseVertex) */
	uint32_t  indexOffset; /* element offset into s_entIndices */
	uint32_t  numIndexes;
	/* -1/-1 = opaque default; skip a stage's real blendFunc and glow effects blot out geometry. */
	int8_t    blendSrc, blendDst;
} gx2entdraw_t;

static gx2vert_t *s_entVerts;    /* memalign(256), GX2_MAX_ENTITY_VERTS */
static uint32_t  *s_entIndices;  /* memalign(256), GX2_MAX_ENTITY_INDEXES */
static gx2entdraw_t s_entDraws[GX2_MAX_ENTITY_DRAWS];
static int        s_numEntDraws;
static uint32_t   s_entVertsUsed;
static uint32_t   s_entIndexesUsed;
static qboolean   s_entOverflowWarned;

/* One 256-byte-aligned row per queued draw, matching hw's UBO base alignment rule. */
static uint32_t s_entVsBlocks[GX2_MAX_ENTITY_DRAWS][64] __attribute__((aligned(256)));
static uint32_t s_entPsBlocks[GX2_MAX_ENTITY_DRAWS][64] __attribute__((aligned(256)));

qboolean GX2R_AddEntityDraw(const float *mvp16, qhandle_t texHandle,
                           const gx2vert_t *verts, uint32_t numVerts,
                           const uint32_t *indices, uint32_t numIndexes)
{
	gx2entdraw_t *d;
	int i;
	int entBlendSrc = -1, entBlendDst = -1; /* -1/-1 = opaque default */

	if (!gx2r_pipelineReady || !s_entVerts || !s_entIndices)
		return qfalse;

	if (s_numEntDraws >= GX2_MAX_ENTITY_DRAWS ||
	    s_entVertsUsed + numVerts > GX2_MAX_ENTITY_VERTS ||
	    s_entIndexesUsed + numIndexes > GX2_MAX_ENTITY_INDEXES) {
		if (!s_entOverflowWarned) {
			s_entOverflowWarned = qtrue;
			ri.Printf(PRINT_ALL, "^1GX2R: entity draw buffer overflow, dropping draws\n");
		}
		return qfalse;
	}

	memcpy(&s_entVerts[s_entVertsUsed], verts, sizeof(gx2vert_t) * numVerts);

	/* Bake vertex base into indices -- GX2DrawIndexedEx's "offset" as base-vertex is unconfirmed. */
	{
		uint32_t k;
		for (k = 0; k < numIndexes; k++)
			s_entIndices[s_entIndexesUsed + k] = indices[k] + s_entVertsUsed;
	}

	/* Skip env-map stages picking a texture -- blind stage[0] showed grayscale specular
	 * instead of real diffuse. Also apply rgbGen const tint; shared base textures need it. */
	{
		const gx2ShaderScript_t *script = GX2Image_GetScript(texHandle);
		const gx2ShaderStage_t *stage0 = GX2Image_GetRepresentativeStage(script);
		if (stage0) {
			texHandle = stage0->imageHandle ? stage0->imageHandle : GX2IMAGE_WHITE;
			/* Stage's own blendFunc (e.g. additive glow); ONE/ZERO default is safe when a script matched. */
			entBlendSrc = stage0->blendSrc;
			entBlendDst = stage0->blendDst;

			if (stage0->hasConstColor) {
				gx2vert_t *dst = &s_entVerts[s_entVertsUsed];
				byte tintR = (byte)(Com_Clamp(0.0f, 1.0f, stage0->constColor[0]) * 255.0f);
				byte tintG = (byte)(Com_Clamp(0.0f, 1.0f, stage0->constColor[1]) * 255.0f);
				byte tintB = (byte)(Com_Clamp(0.0f, 1.0f, stage0->constColor[2]) * 255.0f);
				uint32_t vi;

				for (vi = 0; vi < numVerts; vi++) {
					dst[vi].rgba[0] = (byte)(((int)dst[vi].rgba[0] * tintR) / 255);
					dst[vi].rgba[1] = (byte)(((int)dst[vi].rgba[1] * tintG) / 255);
					dst[vi].rgba[2] = (byte)(((int)dst[vi].rgba[2] * tintB) / 255);
				}
			}
		}
	}

	d = &s_entDraws[s_numEntDraws];
	d->texHandle = texHandle;
	d->vertOffset = s_entVertsUsed;
	d->indexOffset = s_entIndexesUsed;
	d->numIndexes = numIndexes;
	d->blendSrc = (int8_t)entBlendSrc;
	d->blendDst = (int8_t)entBlendDst;

	for (i = 0; i < 16; i++)
		s_entVsBlocks[s_numEntDraws][i] = GX2R_SwapF32(mvp16[i]);

	/* u_Mode = tex0On, no lightmap/alpha test on models yet -- matches the world's unlit pass. */
	s_entPsBlocks[s_numEntDraws][0] = GX2R_SwapF32(1.0f);
	s_entPsBlocks[s_numEntDraws][1] = GX2R_SwapF32(0.0f);
	s_entPsBlocks[s_numEntDraws][2] = GX2R_SwapF32(0.0f);
	s_entPsBlocks[s_numEntDraws][3] = GX2R_SwapF32(0.0f);
	s_entPsBlocks[s_numEntDraws][4] = GX2R_SwapF32(0.5f);
	s_entPsBlocks[s_numEntDraws][5] = 0;
	s_entPsBlocks[s_numEntDraws][6] = 0;
	s_entPsBlocks[s_numEntDraws][7] = 0;

	GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER,
	              &s_entVerts[d->vertOffset], sizeof(gx2vert_t) * numVerts);
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER,
	              &s_entIndices[d->indexOffset], sizeof(uint32_t) * numIndexes);
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
	              s_entVsBlocks[s_numEntDraws], 16 * sizeof(uint32_t));
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
	              s_entPsBlocks[s_numEntDraws], 8 * sizeof(uint32_t));

	s_entVertsUsed += numVerts;
	s_entIndexesUsed += numIndexes;
	s_numEntDraws++;
	return qtrue;
}

/* One draw call per queued entity surface; mvp/PS blocks and texture change per draw. */
static void GX2R_DrawEntities(void)
{
	int i;

	if (!gx2r_pipelineReady || s_numEntDraws <= 0)
		return;

	GX2SetFetchShader(&s_shaderGroup.fetchShader);
	GX2SetVertexShader(s_shaderGroup.vertexShader);
	GX2SetPixelShader(s_shaderGroup.pixelShader);
	GX2SetShaderMode(GX2_SHADER_MODE_UNIFORM_BLOCK);

	GX2SetAttribBuffer(0, sizeof(gx2vert_t) * s_entVertsUsed,
	                   sizeof(gx2vert_t), s_entVerts);

	GX2SetPixelUniformBlock(s_gammaBlockLoc, sizeof(s_gammaBlock), s_gammaBlock);

	GX2SetColorControl(GX2_LOGIC_OP_COPY, 0x01, FALSE, TRUE);
	GX2SetDepthOnlyControl(TRUE, TRUE, GX2_COMPARE_FUNC_LEQUAL);
	GX2SetPolygonControl(GX2_FRONT_FACE_CCW, FALSE, FALSE, FALSE,
	                     GX2_POLYGON_MODE_TRIANGLE, GX2_POLYGON_MODE_TRIANGLE,
	                     FALSE, FALSE, FALSE);

	GX2SetPixelTexture(GX2Image_GetTexture(GX2IMAGE_WHITE), s_lmSamplerLoc);
	GX2SetPixelSampler(&s_sampler, s_lmSamplerLoc);

	for (i = 0; i < s_numEntDraws; i++) {
		gx2entdraw_t *d = &s_entDraws[i];

		if (!d->numIndexes)
			continue;

		/* Script blendFunc needs real blending + no depth write; -1/-1 keeps the opaque default. */
		if (d->blendSrc >= 0) {
			GX2SetDepthOnlyControl(TRUE, FALSE, GX2_COMPARE_FUNC_LEQUAL);
			GX2SetBlendControl(GX2_RENDER_TARGET_0,
			                   (GX2BlendMode)d->blendSrc, (GX2BlendMode)d->blendDst, GX2_BLEND_COMBINE_MODE_ADD,
			                   TRUE, (GX2BlendMode)d->blendSrc, (GX2BlendMode)d->blendDst, GX2_BLEND_COMBINE_MODE_ADD);
		} else {
			GX2SetDepthOnlyControl(TRUE, TRUE, GX2_COMPARE_FUNC_LEQUAL);
			GX2SetBlendControl(GX2_RENDER_TARGET_0,
			                   GX2_BLEND_MODE_ONE, GX2_BLEND_MODE_ZERO, GX2_BLEND_COMBINE_MODE_ADD,
			                   TRUE, GX2_BLEND_MODE_ONE, GX2_BLEND_MODE_ZERO, GX2_BLEND_COMBINE_MODE_ADD);
		}

		GX2SetVertexUniformBlock(s_vsBlockLoc, 16 * sizeof(uint32_t), s_entVsBlocks[i]);
		GX2SetPixelUniformBlock(s_psBlockLoc, 8 * sizeof(uint32_t), s_entPsBlocks[i]);

		GX2SetPixelTexture(GX2Image_GetTexture(d->texHandle), s_baseSamplerLoc);
		GX2SetPixelSampler(&s_samplerWrap, s_baseSamplerLoc);

		/* baseVertex arg is 0 -- indices already carry d->vertOffset baked in. */
		GX2DrawIndexedEx(GX2_PRIMITIVE_MODE_TRIANGLES, d->numIndexes,
		                 GX2_INDEX_TYPE_U32, &s_entIndices[d->indexOffset],
		                 0, 1);
	}
}

/* Compiles ubershaders, builds fetch shader, resolves bindings by name, allocates the vertex ring. */
static qboolean GX2R_InitPipeline(void)
{
	GX2VertexShader *vs;
	GX2PixelShader *ps;
	uint32_t i;

	vs = GX2Shader_CompileVS("q3uber_vert", shaderSrc_q3uber_vert);
	if (!vs)
		return qfalse;
	ps = GX2Shader_CompilePS("q3uber_frag", shaderSrc_q3uber_frag);
	if (!ps)
		return qfalse;

	memset(&s_shaderGroup, 0, sizeof(s_shaderGroup));
	s_shaderGroup.vertexShader = vs;
	s_shaderGroup.pixelShader = ps;
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, vs->program, vs->size);
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, ps->program, ps->size);

	if (!WHBGfxInitShaderAttribute(&s_shaderGroup, "in_Position", 0,
	                               offsetof(gx2vert_t, x), GX2_ATTRIB_FORMAT_FLOAT_32_32_32) ||
	    !WHBGfxInitShaderAttribute(&s_shaderGroup, "in_TexCoord0", 0,
	                               offsetof(gx2vert_t, s0), GX2_ATTRIB_FORMAT_FLOAT_32_32) ||
	    !WHBGfxInitShaderAttribute(&s_shaderGroup, "in_TexCoord1", 0,
	                               offsetof(gx2vert_t, s1), GX2_ATTRIB_FORMAT_FLOAT_32_32) ||
	    !WHBGfxInitShaderAttribute(&s_shaderGroup, "in_Color", 0,
	                               offsetof(gx2vert_t, rgba), GX2_ATTRIB_FORMAT_UNORM_8_8_8_8)) {
		ri.Printf(PRINT_ALL, "^1GX2R: WHBGfxInitShaderAttribute FAILED (attribute name mismatch?)\n");
		return qfalse;
	}
	if (!WHBGfxInitFetchShader(&s_shaderGroup)) {
		ri.Printf(PRINT_ALL, "^1GX2R: WHBGfxInitFetchShader FAILED\n");
		return qfalse;
	}

	/* Bindings resolved by name from reflection. */
	s_vsBlockLoc = 0;
	for (i = 0; i < vs->uniformBlockCount; i++)
		if (!strcmp(vs->uniformBlocks[i].name, "VSBlock"))
			s_vsBlockLoc = vs->uniformBlocks[i].offset;
	s_psBlockLoc = 0;
	s_gammaBlockLoc = 1;
	for (i = 0; i < ps->uniformBlockCount; i++) {
		if (!strcmp(ps->uniformBlocks[i].name, "PSBlock"))
			s_psBlockLoc = ps->uniformBlocks[i].offset;
		else if (!strcmp(ps->uniformBlocks[i].name, "GammaBlock"))
			s_gammaBlockLoc = ps->uniformBlocks[i].offset;
	}
	s_baseSamplerLoc = 0;
	s_lmSamplerLoc = 1;
	for (i = 0; i < ps->samplerVarCount; i++) {
		if (!strcmp(ps->samplerVars[i].name, "s_Base"))
			s_baseSamplerLoc = ps->samplerVars[i].location;
		else if (!strcmp(ps->samplerVars[i].name, "s_Lightmap"))
			s_lmSamplerLoc = ps->samplerVars[i].location;
	}
	ri.Printf(PRINT_ALL, "GX2R: bindings: VSBlock=%u PSBlock=%u GammaBlock=%u s_Base=%u s_Lightmap=%u\n",
	          (unsigned)s_vsBlockLoc, (unsigned)s_psBlockLoc, (unsigned)s_gammaBlockLoc,
	          (unsigned)s_baseSamplerLoc, (unsigned)s_lmSamplerLoc);

	s_verts = memalign(256, sizeof(gx2vert_t) * 4 * GX2_MAX_QUADS);
	if (!s_verts) {
		ri.Printf(PRINT_ALL, "^1GX2R: vertex ring allocation FAILED\n");
		return qfalse;
	}

	s_entVerts = memalign(256, sizeof(gx2vert_t) * GX2_MAX_ENTITY_VERTS);
	s_entIndices = memalign(256, sizeof(uint32_t) * GX2_MAX_ENTITY_INDEXES);
	if (!s_entVerts || !s_entIndices) {
		/* Non-fatal: entities just won't draw. */
		ri.Printf(PRINT_ALL, "^1GX2R: entity buffer allocation FAILED -- models will not draw\n");
	}

	GX2InitSampler(&s_sampler, GX2_TEX_CLAMP_MODE_CLAMP, GX2_TEX_XY_FILTER_MODE_LINEAR);
	/* World/model st coords routinely exceed 0..1 for tiling textures; clamping smeared the edge texel (the "bad textures" bug). */
	GX2InitSampler(&s_samplerWrap, GX2_TEX_CLAMP_MODE_WRAP, GX2_TEX_XY_FILTER_MODE_LINEAR);

	/* Trilinear LOD range, safe to share since per-texture mipLevels clamps the effective range. */
	GX2InitSamplerZMFilter(&s_sampler, GX2_TEX_Z_FILTER_MODE_POINT, GX2_TEX_MIP_FILTER_MODE_LINEAR);
	GX2InitSamplerLOD(&s_sampler, 0.0f, 13.0f, 0.0f);
	GX2InitSamplerZMFilter(&s_samplerWrap, GX2_TEX_Z_FILTER_MODE_POINT, GX2_TEX_MIP_FILTER_MODE_LINEAR);
	GX2InitSamplerLOD(&s_samplerWrap, 0.0f, 13.0f, 0.0f);

	/* Without near-plane clip, triangles straddling it rasterize as huge full-screen wedges (view weapon, close entities). */
	GX2SetRasterizerClipControl(TRUE, TRUE);

#ifdef __WIIU__
#endif
	ri.Printf(PRINT_ALL, "GX2R: samplers ready, calling GX2Image_Init...\n");
	if (!GX2Image_Init())
		return qfalse;
#ifdef __WIIU__
#endif
	ri.Printf(PRINT_ALL, "GX2R: GX2Image_Init done, calling GX2ShaderScript_Init...\n");
	GX2ShaderScript_Init();
#ifdef __WIIU__
#endif
	ri.Printf(PRINT_ALL, "GX2R: GX2ShaderScript_Init done, calling GX2R_BuildUniformBlocks...\n");

	GX2R_BuildUniformBlocks();
#ifdef __WIIU__
#endif
	ri.Printf(PRINT_ALL, "GX2R: GX2R_BuildUniformBlocks done\n");
	return qtrue;
}

void GX2R_Init(void)
{
	if (gx2r_inited)
		return;

	r_fastsky = ri.Cvar_Get("r_fastsky", "0", CVAR_CHEAT);
	r_gamma = ri.Cvar_Get("r_gamma", "2", CVAR_ARCHIVE); /* TEMP: hw gamma test, revert to "1" */

	ri.Printf(PRINT_ALL, "GX2R_Init: WHBGfxInit...\n");
	if (!WHBGfxInit()) {
		ri.Printf(PRINT_ALL, "^1GX2R_Init: WHBGfxInit FAILED\n");
		return;
	}
	gx2r_inited = qtrue;

	GX2R_LogPlannedSurface();

	gx2r_pipelineReady = GX2R_InitPipeline();
	if (gx2r_pipelineReady)
		ri.Printf(PRINT_ALL, "GX2R_Init: 2D pipeline ready\n");
	else
		ri.Printf(PRINT_ALL, "^1GX2R_Init: 2D pipeline UNAVAILABLE -- clear-only display\n");

	ri.Printf(PRINT_ALL, "GX2R_Init: done\n");
}

void GX2R_Shutdown(void)
{
	/* Keep GX2/WHBGfx alive across vid_restart (teardown/reinit hangs on this platform). */
	if (gx2r_inited)
		ri.Printf(PRINT_ALL, "GX2R_Shutdown: keeping GX2 context alive (no-op)\n");
}

/* ---- 2D draw recording ------------------------------------------------ */

void GX2R_SetColor(const float *rgba)
{
	if (!rgba) {
		s_color[0] = s_color[1] = s_color[2] = s_color[3] = 1.0f;
	} else {
		s_color[0] = rgba[0];
		s_color[1] = rgba[1];
		s_color[2] = rgba[2];
		s_color[3] = rgba[3];
	}
}

/* One ring entry: quad with texture handle + optional blend/color override (script rgbGen const). */
static void GX2R_EmitQuad(float x, float y, float w, float h,
                          float s1, float t1, float s2, float t2,
                          qhandle_t hShader, int blendSrc, int blendDst,
                          const float *colorOverride)
{
	gx2vert_t *v;
	const float *color;
	byte rgba[4];
	int i;

	if (s_numQuads >= GX2_MAX_QUADS) {
		if (!s_ringOverflowWarned) {
			s_ringOverflowWarned = 1;
			ri.Printf(PRINT_ALL, "^1GX2R: quad ring overflow (%d), dropping draws\n",
			          GX2_MAX_QUADS);
		}
		return;
	}

	color = colorOverride ? colorOverride : s_color;
	rgba[0] = (byte)(Com_Clamp(0.0f, 1.0f, color[0]) * 255.0f);
	rgba[1] = (byte)(Com_Clamp(0.0f, 1.0f, color[1]) * 255.0f);
	rgba[2] = (byte)(Com_Clamp(0.0f, 1.0f, color[2]) * 255.0f);
	rgba[3] = (byte)(Com_Clamp(0.0f, 1.0f, color[3]) * 255.0f);

	v = &s_verts[s_numQuads * 4];

	v[0].x = x;     v[0].y = y;     v[0].s0 = s1; v[0].t0 = t1;
	v[1].x = x + w; v[1].y = y;     v[1].s0 = s2; v[1].t0 = t1;
	v[2].x = x + w; v[2].y = y + h; v[2].s0 = s2; v[2].t0 = t2;
	v[3].x = x;     v[3].y = y + h; v[3].s0 = s1; v[3].t0 = t2;

	for (i = 0; i < 4; i++) {
		v[i].z = 0.0f;
		v[i].s1 = 0.0f;
		v[i].t1 = 0.0f;
		v[i].rgba[0] = rgba[0];
		v[i].rgba[1] = rgba[1];
		v[i].rgba[2] = rgba[2];
		v[i].rgba[3] = rgba[3];
	}

	s_quadTex[s_numQuads] = hShader;
	s_quadBlendSrc[s_numQuads] = (int8_t)blendSrc;
	s_quadBlendDst[s_numQuads] = (int8_t)blendDst;
	s_numQuads++;
}

void GX2R_StretchPic(float x, float y, float w, float h,
                     float s1, float t1, float s2, float t2, qhandle_t hShader)
{
	const gx2ShaderScript_t *script;
	int st;

	if (!gx2r_pipelineReady)
		return;

	script = GX2Image_GetScript(hShader);
	if (script && script->numStages > 0) {
		for (st = 0; st < script->numStages; st++) {
			const gx2ShaderStage_t *stage = &script->stages[st];
			float rgbaOverride[4];
			const float *colorArg = NULL;

			if (stage->hasConstColor) {
				/* rgbGen const is RGB-only; keep caller's alpha or UI fade-highlights turn into solid blocks. */
				rgbaOverride[0] = stage->constColor[0];
				rgbaOverride[1] = stage->constColor[1];
				rgbaOverride[2] = stage->constColor[2];
				rgbaOverride[3] = s_color[3];
				colorArg = rgbaOverride;
			}
			GX2R_EmitQuad(x, y, w, h, s1, t1, s2, t2, stage->imageHandle,
			             stage->blendSrc, stage->blendDst, colorArg);
		}
		return;
	}

	GX2R_EmitQuad(x, y, w, h, s1, t1, s2, t2, hShader, -1, -1, NULL);
}

/* ---- frame ------------------------------------------------------------ */

void GX2R_BeginFrame(void)
{
	s_numQuads = 0;
	s_ringOverflowWarned = 0;
	s_worldPending = qfalse;
	s_skyPending = qfalse;
	s_numEntDraws = 0;
	s_entVertsUsed = 0;
	s_entIndexesUsed = 0;
	s_entOverflowWarned = qfalse;
}

/* Pipeline + 2D state bind, then the recorded quads. Runs once per target. */
static void GX2R_ApplyBlend(int src, int dst)
{
	if (src < 0) {
		/* Sentinel from a plain (non-.shader) pic: original fixed src-alpha blend. */
		src = GX2_BLEND_MODE_SRC_ALPHA;
		dst = GX2_BLEND_MODE_INV_SRC_ALPHA;
	}
	GX2SetBlendControl(GX2_RENDER_TARGET_0, src, dst, GX2_BLEND_COMBINE_MODE_ADD,
	                   TRUE, src, dst, GX2_BLEND_COMBINE_MODE_ADD);
}

static void GX2R_DrawQuadList(void)
{
	GX2Texture *tex, *boundTex;
	int q, runStart;
	int blendSrc, blendDst, boundBlendSrc, boundBlendDst;

	GX2SetFetchShader(&s_shaderGroup.fetchShader);
	GX2SetVertexShader(s_shaderGroup.vertexShader);
	GX2SetPixelShader(s_shaderGroup.pixelShader);
	/* Without this the GPU reads no block data on real hw -- Cemu forgives it, hw doesn't. */
	GX2SetShaderMode(GX2_SHADER_MODE_UNIFORM_BLOCK);

	GX2SetVertexUniformBlock(s_vsBlockLoc, sizeof(s_vsBlock), s_vsBlock);
	GX2SetPixelUniformBlock(s_psBlockLoc, sizeof(s_psBlock), s_psBlock);
	GX2SetPixelUniformBlock(s_gammaBlockLoc, sizeof(s_gammaBlock), s_gammaBlock);

	GX2SetAttribBuffer(0, sizeof(gx2vert_t) * 4 * s_numQuads,
	                   sizeof(gx2vert_t), s_verts);

	/* Q3 2D state: no depth, no cull; blend set per-run below. */
	GX2SetColorControl(GX2_LOGIC_OP_COPY, 0x01, FALSE, TRUE);
	GX2SetDepthOnlyControl(FALSE, FALSE, GX2_COMPARE_FUNC_ALWAYS);
	GX2SetPolygonControl(GX2_FRONT_FACE_CCW, FALSE, FALSE, FALSE,
	                     GX2_POLYGON_MODE_TRIANGLE, GX2_POLYGON_MODE_TRIANGLE,
	                     FALSE, FALSE, FALSE);

	/* Keep s_Lightmap bound (white); unbound samplers are undefined if the GPU speculates the disabled path. */
	GX2SetPixelTexture(GX2Image_GetTexture(GX2IMAGE_WHITE), s_lmSamplerLoc);
	GX2SetPixelSampler(&s_sampler, s_lmSamplerLoc);

	boundTex = NULL;
	boundBlendSrc = boundBlendDst = -2; /* neither -1 nor a real GX2_BLEND_MODE_* value */
	runStart = 0;
	for (q = 0; q <= s_numQuads; q++) {
		tex = (q < s_numQuads) ? GX2Image_GetTexture(s_quadTex[q]) : NULL;
		blendSrc = (q < s_numQuads) ? s_quadBlendSrc[q] : 0;
		blendDst = (q < s_numQuads) ? s_quadBlendDst[q] : 0;

		if (tex != boundTex || blendSrc != boundBlendSrc ||
		    blendDst != boundBlendDst || q == s_numQuads) {
			if (q > runStart && boundTex) {
				GX2DrawEx(GX2_PRIMITIVE_MODE_QUADS,
				          (q - runStart) * 4, runStart * 4, 1);
			}
			if (tex) {
				GX2SetPixelTexture(tex, s_baseSamplerLoc);
				GX2SetPixelSampler(&s_sampler, s_baseSamplerLoc);
				GX2R_ApplyBlend(blendSrc, blendDst);
			}
			boundTex = tex;
			boundBlendSrc = blendSrc;
			boundBlendDst = blendDst;
			runStart = q;
		}
	}
}

void GX2R_EndFrame(void)
{
	if (!gx2r_inited || !CON_IsForeground())
		return;

	if (gx2r_pipelineReady && s_numQuads > 0) {
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER,
		              s_verts, sizeof(gx2vert_t) * 4 * s_numQuads);
	}

	if (gx2r_pipelineReady)
		GX2R_UpdateGammaBlock();

	WHBGfxBeginRender();

	{
		qboolean need3D = s_worldPending || (s_numEntDraws > 0);

		/* No manual depth clear -- WHBGfxClearColor already does it and re-binds context
		 * state. A raw clear after that trashed state on real hw (Cemu drew fine, of course). */
		WHBGfxBeginRenderTV();
		WHBGfxClearColor(0.10f, 0.10f, 0.15f, 1.0f);
		if (need3D) {
			GX2R_DrawSky();
			GX2R_DrawWorldSurfaces(qfalse);
			GX2R_DrawEntities();
			GX2R_DrawWorldSurfaces(qtrue);
		}
		if (gx2r_pipelineReady && s_numQuads > 0)
			GX2R_DrawQuadList();
		WHBGfxFinishRenderTV();

		WHBGfxBeginRenderDRC();
		WHBGfxClearColor(0.10f, 0.10f, 0.15f, 1.0f);
		if (need3D) {
			GX2R_DrawSky();
			GX2R_DrawWorldSurfaces(qfalse);
			GX2R_DrawEntities();
			GX2R_DrawWorldSurfaces(qtrue);
		}
		if (gx2r_pipelineReady && s_numQuads > 0)
			GX2R_DrawQuadList();
		WHBGfxFinishRenderDRC();
	}

	WHBGfxFinishRender();
}
