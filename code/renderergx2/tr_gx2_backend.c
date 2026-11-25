/* Native GX2 backend for the stock renderergl1 frontend (-DWIIU_GX2BE). 256-byte-aligned
 * byteswapped uniform rows; clears must re-bind context state or hardware trashes it. */

#include "../renderergl1/tr_local.h"

#if defined(WIIU_GX2BE)

#include "tr_gx2_local.h"
#include "tr_gx2_backend.h"

#include <malloc.h>
#include <string.h>

#include <whb/gfx.h>
#include <whb/log.h>
#include <gx2/clear.h>
#include <gx2/context.h>
#include <gx2/display.h>
#include <gx2/draw.h>
#include <gx2/enum.h>
#include <gx2/mem.h>
#include <gx2/registers.h>
#include <gx2/sampler.h>
#include <gx2/shaders.h>
#include <gx2/surface.h>
#include <gx2/texture.h>
#include <gx2/event.h>
#include <gx2/swap.h>
#include <gx2/state.h>
#include <coreinit/memdefaultheap.h>
#include <coreinit/thread.h>
#include <proc_ui/procui.h>
#include <avm/tv.h>

/* con_wiiu.c's single source of truth for ProcUI foreground state. */
extern qboolean CON_IsForeground(void);

void QGL_Init(void);   /* qgl_gx2.c */

gx2be_state_t gx2beState;

static qboolean s_inited;         /* WHBGfx display up */
static qboolean s_pipelineReady;  /* ubershader compiled, draws live */
static qboolean s_frameOpen;      /* TV target bound, draws accepted */

/* Last submitted frame may still be on the GPU: lets the CPU build the next frame meanwhile. */
static qboolean s_gpuPending;
static OSTime   s_gpuFrameTs;

void GX2BE_WaitGpu(void)
{
	if (s_frameOpen) {
		GX2DrawDone();
		s_gpuPending = qfalse;
		return;
	}
	if (s_gpuPending) {
		GX2WaitTimeStamp(s_gpuFrameTs);
		s_gpuPending = qfalse;
	}
}

/* ProcUI priority order is undocumented: registered at both ends so one runs before WHB frees MEM1. */
static uint32_t GX2BE_ReleaseDrain(void *ctx)
{
	(void)ctx;
	GX2BE_WaitGpu();
	return 0;
}

/* ---- pipeline objects -------------------------------------------------- */

static WHBGfxShaderGroup s_group;
static uint32_t s_vsBlockLoc, s_psBlockLoc, s_gammaBlockLoc;
static uint32_t s_baseSamplerLoc, s_lmSamplerLoc;

/* Samplers indexed [wrap][mip]: wrap 0=clamp 1=repeat; mip 0=off 1=on. */
static GX2Sampler s_samplers[2][2];

static uint32_t s_gammaBlock[64] __attribute__((aligned(256)));

/* ---- texture table: texnum is frontend's image->texnum, 1-based (0 = none) ---- */

#define GX2BE_MAX_TEXTURES 2048   /* matches MAX_DRAWIMAGES */

typedef struct {
	GX2Texture *tex;      /* NULL = slot allocated but nothing uploaded */
	int         wrap;     /* GL_REPEAT / GL_CLAMP / GL_CLAMP_TO_EDGE */
	qboolean    mips;
} gx2betex_t;

static gx2betex_t s_textures[GX2BE_MAX_TEXTURES + 1];
static int s_nextTexnum = 1;

/* ---- per-draw state cache: skip redundant GX2Set* writes, invalidated on frame open. ---- */
#define GX2BE_TEXKEY_INVALID   (-2147483647)
#define GX2BE_TEXKEY_FALLBACK  (-2)

static qboolean   s_renderStateValid;
static unsigned long s_lastStateBitsApplied;
static qboolean   s_lastPolyOffsetOn;
static int        s_lastFaceCulling = -1;
static qboolean   s_lastIsMirror;
static int        s_lastTexKey[2] = { GX2BE_TEXKEY_INVALID, GX2BE_TEXKEY_INVALID };

/* Last row bound via GX2Set{Vertex,Pixel}UniformBlock, dedups like s_lastTexKey. */
static const uint32_t *s_lastVsRowBound;
static const uint32_t *s_lastPsRowBound;

static void GX2BE_InvalidateDrawCache(void)
{
	s_renderStateValid = qfalse;
	s_lastFaceCulling = -1;
	s_lastTexKey[0] = GX2BE_TEXKEY_INVALID;
	s_lastTexKey[1] = GX2BE_TEXKEY_INVALID;
	s_lastVsRowBound = NULL;
	s_lastPsRowBound = NULL;
}

static GX2Texture *s_whiteTex;    /* 8x8 white — unbound-sampler safety */

/* ---- per-frame staging rings, reset on frame open; overflow stalls with GX2DrawDone. ---- */

#define GX2BE_RING_VERTS    (256 * 1024)                 /* 8 MB of gx2vert_t */
#define GX2BE_RING_INDEXES  (768 * 1024)                 /* 3 MB of u32 */
#define GX2BE_MAX_DRAWS     8192                         /* uniform rows/frame */

static gx2vert_t *s_ringVerts;    /* memalign(256) */
static uint32_t  *s_ringIndexes;  /* memalign(256) */
static uint32_t   s_ringVertPos;
static uint32_t   s_ringIndexPos;

/* One 256-byte row per draw for VS(mvp)/PS(mode) blocks — hw GX2 UBO base rule. */
static uint32_t (*s_vsRows)[64];  /* memalign(256), GX2BE_MAX_DRAWS rows */
static uint32_t (*s_psRows)[64];
static uint32_t   s_drawIndex;
static qboolean   s_ringWarned;

/* ---- matrices ----------------------------------------------------------- */

static float    s_mvp[16];
static qboolean s_mvpDirty = qtrue;
static const uint32_t *s_curVsRow;   /* row holding current mvp, NULL = stale */
static const uint32_t *s_curPsRow;   /* row holding current mode, NULL = stale */
static float    s_lastMode[4] = { -1, -1, -1, -1 };

/* ---- immediate-mode emulation ------------------------------------------- */

#define GX2BE_IMM_MAX 2048
static gx2vert_t s_immVerts[GX2BE_IMM_MAX];
static int s_immCount, s_immExpected, s_immPrim;

/* ---- DRC mirror pass ----------------------------------------------------- */

static GX2Texture s_tvTex;                /* TV color buffer viewed as texture */
static qboolean   s_tvTexReady;
static gx2vert_t *s_drcQuad;              /* memalign(256), 4 verts, static */
static uint32_t s_drcVsBlock[64] __attribute__((aligned(256)));
static uint32_t s_drcPsBlock[64] __attribute__((aligned(256)));
/* GamePad map pixel modes: [0] flat color, [1+a] texture, [5+a] texture x lightmap (a = alpha test). */
static uint32_t s_drcMapPsBlocks[9][64] __attribute__((aligned(256)));

/* ---- TV connection detect: skip TV present when nothing's plugged in, DRC unaffected. ---- */
static qboolean s_tvConnected = qtrue;    /* optimistic until first poll */
static uint32_t s_tvCheckCounter;

#ifdef WIIU_DEBUG_LOG
/* HOME-cycle freeze diag: swap/flip bookkeeping + buffer pointers around release/acquire. */
#include <coreinit/time.h>
static int s_diagFrames;
static int s_diagCycle;

void GX2BE_DiagSwap(const char *tag)
{
	uint32_t swapCount = 0, flipCount = 0;
	OSTime lastFlip = 0, lastVsync = 0;
	GX2ColorBuffer *cb = WHBGfxGetTVColourBuffer();

	if (!s_inited)
		return;
	GX2GetSwapStatus(&swapCount, &flipCount, &lastFlip, &lastVsync);
	ri.Printf(PRINT_ALL, "DIAG[%d] %s: swap=%u flip=%u pend=%d tvImg=%p tvTexImg=%p fg=%d\n",
	          s_diagCycle, tag, swapCount, flipCount, (int)(swapCount - flipCount),
	          cb ? cb->surface.image : NULL, s_tvTex.surface.image, (int)CON_IsForeground());
}

void GX2BE_DiagAcquired(void)
{
	s_diagCycle++;
	s_diagFrames = 10;
	GX2BE_DiagSwap("acquired");
}
#endif

/* ======================================================================== */
/* small helpers                                                            */
/* ======================================================================== */

static void GX2BE_MatIdentity(float *m)
{
	memset(m, 0, 16 * sizeof(float));
	m[0] = m[5] = m[10] = m[15] = 1.0f;
}

/* out = a * b, column-major 4x4 (GL convention) */
static void GX2BE_MatMul(float *out, const float *a, const float *b)
{
	int c, r, k;
	float t[16];

	for (c = 0; c < 4; c++)
		for (r = 0; r < 4; r++) {
			float sum = 0.0f;
			for (k = 0; k < 4; k++)
				sum += a[k * 4 + r] * b[c * 4 + k];
			t[c * 4 + r] = sum;
		}
	memcpy(out, t, sizeof(t));
}

/* glOrtho(l,r,b,t,n,f) matrix, column-major */
static void GX2BE_MatOrtho(float *m, float l, float r, float b, float t,
                           float n, float f)
{
	memset(m, 0, 16 * sizeof(float));
	m[0]  = 2.0f / (r - l);
	m[5]  = 2.0f / (t - b);
	m[10] = -2.0f / (f - n);
	m[12] = -(r + l) / (r - l);
	m[13] = -(t + b) / (t - b);
	m[14] = -(f + n) / (f - n);
	m[15] = 1.0f;
}

static void GX2BE_MarkMvpDirty(void)
{
	s_mvpDirty = qtrue;
	s_curVsRow = NULL;
}

/* ======================================================================== */
/* frame open / close                                                       */
/* ======================================================================== */

/* GammaBlock stays NEUTRAL: frontend already bakes r_gamma at upload, applying it
 * again in-shader double/triple-gammas the image. Learned via a blindingly bright hw test. */
static void GX2BE_UpdateGammaBlock(void)
{
	s_gammaBlock[0] = GX2R_SwapF32(1.0f);
	s_gammaBlock[1] = 0;
	s_gammaBlock[2] = 0;
	s_gammaBlock[3] = 0;
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
	              s_gammaBlock, sizeof(s_gammaBlock));
}

static void GX2BE_EnsureFrame(void)
{
	if (s_frameOpen || !s_inited || !s_pipelineReady)
		return;
	if (!CON_IsForeground())
		return;      /* backgrounded: draws are dropped, no present */

#ifdef WIIU_DEBUG_LOG
	if (s_diagFrames > 0) {
		OSTime t0;
		GX2BE_DiagSwap("pre-BeginRender");
		t0 = OSGetTime();
		WHBGfxBeginRender();
		ri.Printf(PRINT_ALL, "DIAG[%d] BeginRender took %u us\n", s_diagCycle,
		          (unsigned)OSTicksToMicroseconds(OSGetTime() - t0));
	} else
#endif
	WHBGfxBeginRender();      /* waits previous flip */
	GX2BE_WaitGpu();          /* rings/uniform rows are reused below */
	WHBGfxBeginRenderTV();    /* binds TV context + targets */

	s_ringVertPos = 0;
	s_ringIndexPos = 0;
	s_drawIndex = 0;
	s_ringWarned = qfalse;
	s_curVsRow = NULL;
	s_curPsRow = NULL;
	s_lastMode[0] = -1.0f;
	GX2BE_InvalidateDrawCache();

	GX2BE_UpdateGammaBlock();

	/* Shader pipeline once per frame (nothing else ever binds another). */
	GX2SetFetchShader(&s_group.fetchShader);
	GX2SetVertexShader(s_group.vertexShader);
	GX2SetPixelShader(s_group.pixelShader);
	GX2SetShaderMode(GX2_SHADER_MODE_UNIFORM_BLOCK);
	GX2SetPixelUniformBlock(s_gammaBlockLoc, sizeof(s_gammaBlock), s_gammaBlock);

	s_frameOpen = qtrue;
}

/* Drain GPU so staged draws are consumed, then reuse rings from the top. Stalls; logged once. */
static void GX2BE_RingOverflow(void)
{
	if (!s_ringWarned) {
		s_ringWarned = qtrue;
		ri.Printf(PRINT_WARNING, "GX2BE: staging ring overflow — GX2DrawDone mid-frame (perf hit)\n");
	}
	GX2DrawDone();
	s_ringVertPos = 0;
	s_ringIndexPos = 0;
	s_drawIndex = 0;
	s_curVsRow = NULL;
	s_curPsRow = NULL;
}

/* ======================================================================== */
/* state application (per draw)                                             */
/* ======================================================================== */

static GX2BlendMode GX2BE_SrcBlend(unsigned long bits)
{
	switch (bits & GLS_SRCBLEND_BITS) {
	case GLS_SRCBLEND_ZERO:                return GX2_BLEND_MODE_ZERO;
	case GLS_SRCBLEND_ONE:                 return GX2_BLEND_MODE_ONE;
	case GLS_SRCBLEND_DST_COLOR:           return GX2_BLEND_MODE_DST_COLOR;
	case GLS_SRCBLEND_ONE_MINUS_DST_COLOR: return GX2_BLEND_MODE_INV_DST_COLOR;
	case GLS_SRCBLEND_SRC_ALPHA:           return GX2_BLEND_MODE_SRC_ALPHA;
	case GLS_SRCBLEND_ONE_MINUS_SRC_ALPHA: return GX2_BLEND_MODE_INV_SRC_ALPHA;
	case GLS_SRCBLEND_DST_ALPHA:           return GX2_BLEND_MODE_DST_ALPHA;
	case GLS_SRCBLEND_ONE_MINUS_DST_ALPHA: return GX2_BLEND_MODE_INV_DST_ALPHA;
	case GLS_SRCBLEND_ALPHA_SATURATE:      return GX2_BLEND_MODE_SRC_ALPHA_SAT;
	default:                               return GX2_BLEND_MODE_ONE;
	}
}

static GX2BlendMode GX2BE_DstBlend(unsigned long bits)
{
	switch (bits & GLS_DSTBLEND_BITS) {
	case GLS_DSTBLEND_ZERO:                return GX2_BLEND_MODE_ZERO;
	case GLS_DSTBLEND_ONE:                 return GX2_BLEND_MODE_ONE;
	case GLS_DSTBLEND_SRC_COLOR:           return GX2_BLEND_MODE_SRC_COLOR;
	case GLS_DSTBLEND_ONE_MINUS_SRC_COLOR: return GX2_BLEND_MODE_INV_SRC_COLOR;
	case GLS_DSTBLEND_SRC_ALPHA:           return GX2_BLEND_MODE_SRC_ALPHA;
	case GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA: return GX2_BLEND_MODE_INV_SRC_ALPHA;
	case GLS_DSTBLEND_DST_ALPHA:           return GX2_BLEND_MODE_DST_ALPHA;
	case GLS_DSTBLEND_ONE_MINUS_DST_ALPHA: return GX2_BLEND_MODE_INV_DST_ALPHA;
	default:                               return GX2_BLEND_MODE_ZERO;
	}
}

static void GX2BE_ApplyRenderState(void)
{
	unsigned long bits = gx2beState.stateBits;
	qboolean depthTest = !(bits & GLS_DEPTHTEST_DISABLE);
	qboolean depthWrite = (bits & GLS_DEPTHMASK_TRUE) != 0;
	GX2CompareFunction depthFunc =
		(bits & GLS_DEPTHFUNC_EQUAL) ? GX2_COMPARE_FUNC_EQUAL : GX2_COMPARE_FUNC_LEQUAL;
	GX2BlendMode src, dst;
	BOOL cullFront = FALSE, cullBack = FALSE;
	qboolean isMirror = backEnd.viewParms.isMirror;

	if (s_renderStateValid && bits == s_lastStateBitsApplied &&
	    gx2beState.polyOffsetOn == s_lastPolyOffsetOn &&
	    gx2beState.faceCulling == s_lastFaceCulling &&
	    isMirror == s_lastIsMirror)
		return;   /* identical to previous draw's state — nothing to write */

	if (bits & (GLS_SRCBLEND_BITS | GLS_DSTBLEND_BITS)) {
		src = GX2BE_SrcBlend(bits);
		dst = GX2BE_DstBlend(bits);
	} else {
		src = GX2_BLEND_MODE_ONE;   /* opaque */
		dst = GX2_BLEND_MODE_ZERO;
	}

	GX2SetColorControl(GX2_LOGIC_OP_COPY, 0x01, FALSE, TRUE);
	GX2SetBlendControl(GX2_RENDER_TARGET_0, src, dst, GX2_BLEND_COMBINE_MODE_ADD,
	                   TRUE, src, dst, GX2_BLEND_COMBINE_MODE_ADD);
	GX2SetDepthOnlyControl(depthTest, depthWrite, depthFunc);

	/* Matches renderergl1's GL_Cull: CT_FRONT_SIDED culls GL_FRONT, CT_BACK_SIDED
	 * culls GL_BACK, CT_TWO_SIDED culls neither; isMirror inverts front/back. */
	if (gx2beState.faceCulling != CT_TWO_SIDED) {
		qboolean cf = (gx2beState.faceCulling == CT_FRONT_SIDED);
		if (isMirror)
			cf = !cf;
		cullFront = cf ? TRUE : FALSE;
		cullBack = cf ? FALSE : TRUE;
	}

	GX2SetPolygonControl(GX2_FRONT_FACE_CCW, cullFront, cullBack, FALSE,
	                     GX2_POLYGON_MODE_TRIANGLE, GX2_POLYGON_MODE_TRIANGLE,
	                     gx2beState.polyOffsetOn ? TRUE : FALSE,
	                     gx2beState.polyOffsetOn ? TRUE : FALSE,
	                     FALSE);

	s_lastStateBitsApplied = bits;
	s_lastPolyOffsetOn = gx2beState.polyOffsetOn;
	s_lastFaceCulling = gx2beState.faceCulling;
	s_lastIsMirror = isMirror;
	s_renderStateValid = qtrue;
}

static void GX2BE_ApplyUniforms(void)
{
	float mode[4];

	/* VS block: mvp */
	if (s_mvpDirty) {
		GX2BE_MatMul(s_mvp, gx2beState.projMtx, gx2beState.mvMtx);
		s_mvpDirty = qfalse;
	}
	if (!s_curVsRow) {
		uint32_t *row;
		int i;

		if (s_drawIndex >= GX2BE_MAX_DRAWS)
			GX2BE_RingOverflow();
		row = s_vsRows[s_drawIndex];
		for (i = 0; i < 16; i++)
			row[i] = GX2R_SwapF32(s_mvp[i]);
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
		              row, 64 * sizeof(uint32_t));
		s_curVsRow = row;
		s_drawIndex++;
	}
	if (s_curVsRow != s_lastVsRowBound) {
		GX2SetVertexUniformBlock(s_vsBlockLoc, 16 * sizeof(uint32_t), s_curVsRow);
		s_lastVsRowBound = s_curVsRow;
	}

	/* PS block: (tex0On, tex1On, tex1Add, atest) + alpharef */
	mode[0] = gx2beState.tex2DEnabled[0] ? 1.0f : 0.0f;
	mode[1] = gx2beState.tex2DEnabled[1] ? 1.0f : 0.0f;
	mode[2] = (gx2beState.texenv[1] == GL_ADD) ? 1.0f : 0.0f;
	switch (gx2beState.stateBits & GLS_ATEST_BITS) {
	case GLS_ATEST_GT_0:  mode[3] = 1.0f; break;
	case GLS_ATEST_LT_80: mode[3] = 2.0f; break;
	case GLS_ATEST_GE_80: mode[3] = 3.0f; break;
	default:              mode[3] = 0.0f; break;
	}

	if (!s_curPsRow || memcmp(mode, s_lastMode, sizeof(mode)) != 0) {
		uint32_t *row;

		if (s_drawIndex >= GX2BE_MAX_DRAWS)
			GX2BE_RingOverflow();
		row = s_psRows[s_drawIndex];
		row[0] = GX2R_SwapF32(mode[0]);
		row[1] = GX2R_SwapF32(mode[1]);
		row[2] = GX2R_SwapF32(mode[2]);
		row[3] = GX2R_SwapF32(mode[3]);
		row[4] = GX2R_SwapF32(0.5f);   /* u_AlphaRef.x */
		row[5] = 0;
		row[6] = 0;
		row[7] = 0;
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
		              row, 64 * sizeof(uint32_t));
		s_curPsRow = row;
		memcpy(s_lastMode, mode, sizeof(mode));
		s_drawIndex++;
	}
	if (s_curPsRow != s_lastPsRowBound) {
		GX2SetPixelUniformBlock(s_psBlockLoc, 8 * sizeof(uint32_t), s_curPsRow);
		s_lastPsRowBound = s_curPsRow;
	}
}

static void GX2BE_BindTextureUnit(int tmu, uint32_t samplerLoc)
{
	int texnum = gx2beState.boundtex[tmu];
	GX2Texture *tex = NULL;
	GX2Sampler *sampler;
	int wrapIdx = 1, mipIdx = 0;

	if (texnum > 0 && texnum <= GX2BE_MAX_TEXTURES && s_textures[texnum].tex) {
		tex = s_textures[texnum].tex;
		wrapIdx = (s_textures[texnum].wrap == GL_REPEAT) ? 1 : 0;
		mipIdx = s_textures[texnum].mips ? 1 : 0;
	}
	if (!tex)
		tex = s_whiteTex;

	sampler = &s_samplers[wrapIdx][mipIdx];
	GX2SetPixelTexture(tex, samplerLoc);
	GX2SetPixelSampler(sampler, samplerLoc);
}

/* ======================================================================== */
/* GX2BE_ state entry points (from the renderergl1 choke arms)              */
/* ======================================================================== */

void GX2BE_GL_State(unsigned long stateBits)
{
	gx2beState.stateBits = stateBits;
	s_curPsRow = NULL;   /* atest may have changed */
}

void GX2BE_GL_Cull(int cullType)
{
	gx2beState.faceCulling = cullType;
}

void GX2BE_GL_TexEnv(int unit, int env)
{
	if (unit >= 0 && unit < 2)
		gx2beState.texenv[unit] = env;
	s_curPsRow = NULL;
}

void GX2BE_BindTexnum(int tmu, int texnum)
{
	if (tmu >= 0 && tmu < 2)
		gx2beState.boundtex[tmu] = texnum;
		gx2beState.tex2DEnabled[tmu] = (texnum > 0);
}

void GX2BE_SetVertexPtr(const void *p, int stride)
{
	gx2beState.posPtr = p;
	gx2beState.posStride = stride;
}

void GX2BE_SetColorPtr(const void *p, int stride)
{
	gx2beState.clrPtr = p;
	gx2beState.clrStride = stride;
}

void GX2BE_SetTexCoordPtr(const void *p, int stride)
{
	gx2beState.texPtr[gx2beState.currenttmu] = p;
	gx2beState.texStride[gx2beState.currenttmu] = stride;
}

void GX2BE_SetTexture2DEnabled(int enabled)
{
	gx2beState.tex2DEnabled[gx2beState.currenttmu] = enabled;
	s_curPsRow = NULL;
}

void GX2BE_PolygonOffset(float factor, float units)
{
	/* GL polygon offset: o = factor*DZ + units*r; R700 uses the same scale semantics as GL. */
	GX2SetPolygonOffset(factor, units, factor, units, 0.0f);
}

void GX2BE_SetPolygonOffsetEnabled(int enabled)
{
	gx2beState.polyOffsetOn = enabled;
}

void GX2BE_SetViewport(int x, int y, int w, int h)
{
	gx2beState.vpX = x; gx2beState.vpY = y;
	gx2beState.vpW = w; gx2beState.vpH = h;

	GX2BE_EnsureFrame();
	if (!s_frameOpen)
		return;
	/* GL viewport is bottom-up; GX2 window coords are top-down. */
	GX2SetViewport((float)x, (float)(glConfig.vidHeight - (y + h)),
	               (float)w, (float)h,
	               gx2beState.depthNear, gx2beState.depthFar);
}

void GX2BE_SetScissor(int x, int y, int w, int h)
{
	int gx2y = glConfig.vidHeight - (y + h);

	GX2BE_EnsureFrame();
	if (!s_frameOpen)
		return;
	if (x < 0) { w += x; x = 0; }
	if (gx2y < 0) { h += gx2y; gx2y = 0; }
	if (w < 0) w = 0;
	if (h < 0) h = 0;
	GX2SetScissor((uint32_t)x, (uint32_t)gx2y, (uint32_t)w, (uint32_t)h);
}

void GX2BE_DepthRange(float n, float f)
{
	if (n == gx2beState.depthNear && f == gx2beState.depthFar)
		return;
	gx2beState.depthNear = n;
	gx2beState.depthFar = f;
	/* Depth range rides on the viewport. */
	GX2BE_SetViewport(gx2beState.vpX, gx2beState.vpY, gx2beState.vpW, gx2beState.vpH);
}

/* ---- matrices ---------------------------------------------------------- */

void GX2BE_LoadProjectionGL(const float *gl16)
{
	memcpy(gx2beState.projMtx, gl16, 16 * sizeof(float));
	GX2BE_MarkMvpDirty();
}

/* Oblique near-plane clip (Lengyel): rebuild projection near plane as eyePlane, no user clip planes needed. */
void GX2BE_LoadProjectionObliqueGL(const float *gl16, const float *eyePlane)
{
	float m[16];
	float qx, qy, qz, qw, dot, s;

	memcpy(m, gl16, 16 * sizeof(float));

	qx = ((eyePlane[0] < 0.0f ? -1.0f : 1.0f) + m[8]) / m[0];
	qy = ((eyePlane[1] < 0.0f ? -1.0f : 1.0f) + m[9]) / m[5];
	qz = -1.0f;
	qw = (1.0f + m[10]) / m[14];

	dot = eyePlane[0] * qx + eyePlane[1] * qy + eyePlane[2] * qz + eyePlane[3] * qw;
	if (dot == 0.0f)
		dot = 0.00001f;
	s = 2.0f / dot;

	m[2]  = eyePlane[0] * s;
	m[6]  = eyePlane[1] * s;
	m[10] = eyePlane[2] * s + 1.0f;
	m[14] = eyePlane[3] * s;

	memcpy(gx2beState.projMtx, m, sizeof(m));
	GX2BE_MarkMvpDirty();
}

void GX2BE_LoadModelviewGL(const float *gl16)
{
	memcpy(gx2beState.mvMtx, gl16, 16 * sizeof(float));
	GX2BE_MarkMvpDirty();
}

void GX2BE_LoadModelviewTranslatedGL(const float *gl16, const vec3_t origin)
{
	float *m = gx2beState.mvMtx;

	memcpy(m, gl16, 16 * sizeof(float));
	/* Column-major post-translate: m' = m * T(origin). */
	m[12] = gl16[0] * origin[0] + gl16[4] * origin[1] + gl16[8]  * origin[2] + gl16[12];
	m[13] = gl16[1] * origin[0] + gl16[5] * origin[1] + gl16[9]  * origin[2] + gl16[13];
	m[14] = gl16[2] * origin[0] + gl16[6] * origin[1] + gl16[10] * origin[2] + gl16[14];
	GX2BE_MarkMvpDirty();
}

void GX2BE_LoadOrthoGL(float left, float right, float bottom, float top,
                       float znear, float zfar)
{
	GX2BE_MatOrtho(gx2beState.projMtx, left, right, bottom, top, znear, zfar);
	GX2BE_MarkMvpDirty();
}

void GX2BE_LoadOrtho2D(int vidWidth, int vidHeight)
{
	/* glOrtho(0, w, h, 0, 0, 1) — y down. */
	GX2BE_MatOrtho(gx2beState.projMtx, 0.0f, (float)vidWidth,
	               (float)vidHeight, 0.0f, 0.0f, 1.0f);
	GX2BE_MatIdentity(gx2beState.mvMtx);
	GX2BE_MarkMvpDirty();
}

void GX2BE_LoadIdentityModelview(void)
{
	GX2BE_MatIdentity(gx2beState.mvMtx);
	GX2BE_MarkMvpDirty();
}

/* ---- clear -------------------------------------------------------------- */

void GX2BE_Clear(qboolean clearColor, qboolean clearDepth, float r, float g, float b)
{
	GX2ColorBuffer *cb;
	GX2DepthBuffer *db;

	GX2BE_EnsureFrame();
	if (!s_frameOpen)
		return;

	cb = WHBGfxGetTVColourBuffer();
	db = WHBGfxGetTVDepthBuffer();

	if (clearColor && clearDepth)
		GX2ClearBuffersEx(cb, db, r, g, b, 1.0f, 1.0f, 0,
		                  GX2_CLEAR_FLAGS_DEPTH | GX2_CLEAR_FLAGS_STENCIL);
	else if (clearColor)
		GX2ClearColor(cb, r, g, b, 1.0f);
	else if (clearDepth)
		GX2ClearDepthStencilEx(db, 1.0f, 0,
		                       GX2_CLEAR_FLAGS_DEPTH | GX2_CLEAR_FLAGS_STENCIL);

	/* Raw clears trash context state on real hw -- re-bind and re-issue the pipeline
	 * or a stale mode poisons the whole view. Cemu never once complained about this. */
	GX2SetContextState(WHBGfxGetTVContextState());
	GX2SetFetchShader(&s_group.fetchShader);
	GX2SetVertexShader(s_group.vertexShader);
	GX2SetPixelShader(s_group.pixelShader);
	GX2SetShaderMode(GX2_SHADER_MODE_UNIFORM_BLOCK);
	GX2SetPixelUniformBlock(s_gammaBlockLoc, sizeof(s_gammaBlock), s_gammaBlock);
	GX2BE_InvalidateDrawCache();
}

void GX2BE_Finish(void)
{
	GX2BE_WaitGpu();
}

/* ======================================================================== */
/* draw paths                                                               */
/* ======================================================================== */

static gx2vert_t *GX2BE_AllocVerts(uint32_t count)
{
	/* 64-byte-align every segment base -- a 32-byte start rendered fine in Cemu, garbled on hw. */
	s_ringVertPos = (s_ringVertPos + 1u) & ~1u;
	if (s_ringVertPos + count > GX2BE_RING_VERTS)
		GX2BE_RingOverflow();
	if (count > GX2BE_RING_VERTS)
		return NULL;
	return &s_ringVerts[s_ringVertPos];
}

static void GX2BE_EmitDraw(const gx2vert_t *stagedVerts, uint32_t numVerts,
                           const uint32_t *stagedIndexes, uint32_t numIndexes,
                           GX2PrimitiveMode prim)
{
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER,
	              (void *)stagedVerts, numVerts * sizeof(gx2vert_t));

	GX2BE_ApplyRenderState();
	GX2BE_ApplyUniforms();

	GX2SetAttribBuffer(0, numVerts * sizeof(gx2vert_t),
	                   sizeof(gx2vert_t), (void *)stagedVerts);

	{
		int key0 = gx2beState.boundtex[0];
		int key1 = gx2beState.tex2DEnabled[1] ? gx2beState.boundtex[1]
		                                      : GX2BE_TEXKEY_FALLBACK;

		if (key0 != s_lastTexKey[0]) {
			GX2BE_BindTextureUnit(0, s_baseSamplerLoc);
			s_lastTexKey[0] = key0;
		}
		if (key1 != s_lastTexKey[1]) {
			if (gx2beState.tex2DEnabled[1]) {
				GX2BE_BindTextureUnit(1, s_lmSamplerLoc);
			} else {
				/* keep the lightmap sampler defined even when the shader path is off */
				GX2SetPixelTexture(s_whiteTex, s_lmSamplerLoc);
				GX2SetPixelSampler(&s_samplers[0][0], s_lmSamplerLoc);
			}
			s_lastTexKey[1] = key1;
		}
	}

	if (stagedIndexes) {
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER,
		              (void *)stagedIndexes, numIndexes * sizeof(uint32_t));
		GX2DrawIndexedEx(prim, numIndexes, GX2_INDEX_TYPE_U32,
		                 (void *)stagedIndexes, 0, 1);
	} else {
		GX2DrawEx(prim, numVerts, 0, 1);
	}
}

void GX2BE_DrawTess(int numIndexes, const unsigned int *indexes)
{
	uint32_t numVerts;
	gx2vert_t *dst;
	uint32_t *dstIdx;
	const byte *pos, *clr, *st0, *st1;
	uint32_t v;

	GX2BE_EnsureFrame();
	if (!s_frameOpen || numIndexes <= 0)
		return;

	numVerts = (uint32_t)tess.numVertexes;
	if (!numVerts || !gx2beState.posPtr)
		return;

	dst = GX2BE_AllocVerts(numVerts);
	if (!dst)
		return;
	/* 64-byte-align index segment bases too, same fetch-alignment caution. */
	s_ringIndexPos = (s_ringIndexPos + 15u) & ~15u;
	if (s_ringIndexPos + (uint32_t)numIndexes > GX2BE_RING_INDEXES)
		GX2BE_RingOverflow();
	dstIdx = &s_ringIndexes[s_ringIndexPos];

	pos = (const byte *)gx2beState.posPtr;
	clr = (const byte *)gx2beState.clrPtr;
	st0 = (const byte *)gx2beState.texPtr[0];
	st1 = (const byte *)gx2beState.texPtr[1];

	for (v = 0; v < numVerts; v++) {
		const float *p = (const float *)(pos + (size_t)v * gx2beState.posStride);

		dst[v].x = p[0];
		dst[v].y = p[1];
		dst[v].z = p[2];
		if (st0) {
			const float *t = (const float *)(st0 + (size_t)v * gx2beState.texStride[0]);
			dst[v].s0 = t[0];
			dst[v].t0 = t[1];
		} else {
			dst[v].s0 = dst[v].t0 = 0.0f;
		}
		if (st1) {
			const float *t = (const float *)(st1 + (size_t)v * gx2beState.texStride[1]);
			dst[v].s1 = t[0];
			dst[v].t1 = t[1];
		} else {
			dst[v].s1 = dst[v].t1 = 0.0f;
		}
		if (clr) {
			const byte *c = clr + (size_t)v * gx2beState.clrStride;
			dst[v].rgba[0] = c[0];
			dst[v].rgba[1] = c[1];
			dst[v].rgba[2] = c[2];
			dst[v].rgba[3] = c[3];
		} else {
			dst[v].rgba[0] = dst[v].rgba[1] = dst[v].rgba[2] = dst[v].rgba[3] = 255;
		}
	}

	memcpy(dstIdx, indexes, (size_t)numIndexes * sizeof(uint32_t));

	s_ringVertPos += numVerts;
	s_ringIndexPos += (uint32_t)numIndexes;

	GX2BE_EmitDraw(dst, numVerts, dstIdx, (uint32_t)numIndexes,
	               GX2_PRIMITIVE_MODE_TRIANGLES);
}

/* ---- immediate mode ------------------------------------------------------ */

void GX2BE_ImmediateBegin(int primGL, int numVerts)
{
	s_immCount = 0;
	s_immExpected = numVerts;
	s_immPrim = primGL;
}

void GX2BE_ImmediateTexVertex(const float *st, const float *xyz,
                              byte r, byte g, byte b, byte a)
{
	gx2vert_t *v;

	if (s_immCount >= GX2BE_IMM_MAX || s_immCount >= s_immExpected)
		return;
	v = &s_immVerts[s_immCount++];
	v->x = xyz[0];
	v->y = xyz[1];
	v->z = xyz[2];
	v->s0 = st[0];
	v->t0 = st[1];
	v->s1 = v->t1 = 0.0f;
	v->rgba[0] = r; v->rgba[1] = g; v->rgba[2] = b; v->rgba[3] = a;
}

void GX2BE_ImmediateEnd(void)
{
	GX2PrimitiveMode prim;
	gx2vert_t *dst;

	GX2BE_EnsureFrame();
	if (!s_frameOpen || !s_immCount)
		return;

	switch (s_immPrim) {
	case GL_TRIANGLE_STRIP: prim = GX2_PRIMITIVE_MODE_TRIANGLE_STRIP; break;
	case GL_QUADS:          prim = GX2_PRIMITIVE_MODE_QUADS; break;
	case GL_LINES:          prim = GX2_PRIMITIVE_MODE_LINES; break;
	case GL_TRIANGLES:      prim = GX2_PRIMITIVE_MODE_TRIANGLES; break;
	default:                s_immCount = 0; return;
	}

	dst = GX2BE_AllocVerts((uint32_t)s_immCount);
	if (!dst) {
		s_immCount = 0;
		return;
	}
	memcpy(dst, s_immVerts, (size_t)s_immCount * sizeof(gx2vert_t));
	s_ringVertPos += (uint32_t)s_immCount;

	GX2BE_EmitDraw(dst, (uint32_t)s_immCount, NULL, 0, prim);
	s_immCount = 0;
}

/* ======================================================================== */
/* readbacks (stubs this session)                                           */
/* ======================================================================== */

float GX2BE_PeekDepth(int x, int y)
{
	(void)x; (void)y;
	/* No depth readback yet: report "far plane", flare reads as occluded. */
	return 1.0f;
}

void GX2BE_ReadPixelsRGB(int x, int y, int w, int h, int padlen, byte *dst)
{
	static qboolean warned;

	(void)x; (void)y;
	if (!warned) {
		warned = qtrue;
		ri.Printf(PRINT_WARNING, "GX2BE_ReadPixelsRGB: not implemented — screenshots are black\n");
	}
	memset(dst, 0, (size_t)(w * 3 + padlen) * (size_t)h);
}

/* ======================================================================== */
/* textures                                                                 */
/* ======================================================================== */

void GX2BE_CreateTexnum(unsigned int *texnum)
{
	if (s_nextTexnum > GX2BE_MAX_TEXTURES) {
		ri.Error(ERR_DROP, "GX2BE_CreateTexnum: out of texture slots");
	}
	*texnum = (unsigned int)s_nextTexnum++;
}

void GX2BE_DeleteTexnum(int texnum)
{
	if (texnum <= 0 || texnum > GX2BE_MAX_TEXTURES)
		return;
	if (s_textures[texnum].tex) {
		GX2BE_WaitGpu();
		GX2Image_FreeTexture(s_textures[texnum].tex);
		s_textures[texnum].tex = NULL;
	}
	/* Bind cache keys on texnum; force rebind even if slot's reused next draw. */
	s_lastTexKey[0] = GX2BE_TEXKEY_INVALID;
	s_lastTexKey[1] = GX2BE_TEXKEY_INVALID;
}

void GX2BE_DeleteAllTexnums(void)
{
	int i;

	/* Drain first — GPU may still be sampling these this frame. */
	GX2BE_WaitGpu();
	for (i = 1; i <= GX2BE_MAX_TEXTURES; i++)
		GX2BE_DeleteTexnum(i);
	s_nextTexnum = 1;
}

void GX2BE_Upload32(unsigned *data, int width, int height,
                    int glInternalFormat, int texnum, int wrapClampMode,
                    qboolean mipmap, qboolean allowTiled)
{
	GX2Texture *tex;

	(void)glInternalFormat;   /* everything is RGBA8 on this path */

	if (texnum <= 0 || texnum > GX2BE_MAX_TEXTURES)
		return;

	if (s_textures[texnum].tex) {
		/* Re-upload into an existing slot (vid_restart, cinematic resize). */
		GX2BE_WaitGpu();
		GX2Image_FreeTexture(s_textures[texnum].tex);
		s_textures[texnum].tex = NULL;
		/* New tex may live at a different pointer — force rebind. */
		s_lastTexKey[0] = GX2BE_TEXKEY_INVALID;
		s_lastTexKey[1] = GX2BE_TEXKEY_INVALID;
	}

	tex = GX2Image_Upload((const byte *)data, width, height, !mipmap, allowTiled);
	if (!tex) {
		ri.Printf(PRINT_WARNING, "GX2BE_Upload32: upload failed (%dx%d)\n", width, height);
		return;
	}
	s_textures[texnum].tex = tex;
	s_textures[texnum].wrap = wrapClampMode;
	s_textures[texnum].mips = mipmap;
}

void GX2BE_TexSubImage2D(int texnum, int fullW, int fullH, const byte *data)
{
	GX2Texture *tex;
	int y;

	if (texnum <= 0 || texnum > GX2BE_MAX_TEXTURES || !s_textures[texnum].tex)
		return;
	tex = s_textures[texnum].tex;
	if ((uint32_t)fullW > tex->surface.width || (uint32_t)fullH > tex->surface.height)
		return;

	GX2BE_WaitGpu();   /* cinematic frames overwrite a texture the last frame may still sample */
	for (y = 0; y < fullH; y++) {
		memcpy((byte *)tex->surface.image + (size_t)y * tex->surface.pitch * 4,
		       data + (size_t)y * fullW * 4,
		       (size_t)fullW * 4);
	}
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU_TEXTURE,
	              tex->surface.image, tex->surface.imageSize);
}

/* ======================================================================== */
/* default state                                                            */
/* ======================================================================== */

void GX2BE_SetDefaultState(void)
{
	gx2beState.currenttmu = 0;
	gx2beState.boundtex[0] = -1;
	gx2beState.boundtex[1] = -1;
	gx2beState.texenv[0] = GL_MODULATE;
	gx2beState.texenv[1] = GL_MODULATE;
	gx2beState.tex2DEnabled[0] = 1;
	gx2beState.tex2DEnabled[1] = 0;
	gx2beState.numActiveTMUs = 1;
	gx2beState.faceCulling = -1;
	gx2beState.stateBits = GLS_DEPTHTEST_DISABLE | GLS_DEPTHMASK_TRUE;
	gx2beState.depthNear = 0.0f;
	gx2beState.depthFar = 1.0f;
	gx2beState.polyOffsetOn = 0;
	gx2beState.posPtr = NULL;
	gx2beState.clrPtr = NULL;
	gx2beState.texPtr[0] = NULL;
	gx2beState.texPtr[1] = NULL;
	GX2BE_MatIdentity(gx2beState.projMtx);
	GX2BE_MatIdentity(gx2beState.mvMtx);
	GX2BE_MarkMvpDirty();
	s_curPsRow = NULL;

	gx2beState.vpX = 0;
	gx2beState.vpY = 0;
	gx2beState.vpW = glConfig.vidWidth ? glConfig.vidWidth : 1280;
	gx2beState.vpH = glConfig.vidHeight ? glConfig.vidHeight : 720;
}

/* ======================================================================== */
/* pipeline / display bring-up + present (GLimp_*)                          */
/* ======================================================================== */

static qboolean GX2BE_InitPipeline(void)
{
	GX2VertexShader *vs;
	GX2PixelShader *ps;
	uint32_t i;
	int w, m;

	vs = GX2Shader_CompileVS("q3uber_vert", shaderSrc_q3uber_vert);
	if (!vs)
		return qfalse;
	ps = GX2Shader_CompilePS("q3uber_frag", shaderSrc_q3uber_frag);
	if (!ps)
		return qfalse;

	memset(&s_group, 0, sizeof(s_group));
	s_group.vertexShader = vs;
	s_group.pixelShader = ps;
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, vs->program, vs->size);
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, ps->program, ps->size);

	if (!WHBGfxInitShaderAttribute(&s_group, "in_Position", 0,
	                               offsetof(gx2vert_t, x), GX2_ATTRIB_FORMAT_FLOAT_32_32_32) ||
	    !WHBGfxInitShaderAttribute(&s_group, "in_TexCoord0", 0,
	                               offsetof(gx2vert_t, s0), GX2_ATTRIB_FORMAT_FLOAT_32_32) ||
	    !WHBGfxInitShaderAttribute(&s_group, "in_TexCoord1", 0,
	                               offsetof(gx2vert_t, s1), GX2_ATTRIB_FORMAT_FLOAT_32_32) ||
	    !WHBGfxInitShaderAttribute(&s_group, "in_Color", 0,
	                               offsetof(gx2vert_t, rgba), GX2_ATTRIB_FORMAT_UNORM_8_8_8_8)) {
		ri.Printf(PRINT_ALL, "^1GX2BE: WHBGfxInitShaderAttribute FAILED\n");
		return qfalse;
	}
	if (!WHBGfxInitFetchShader(&s_group)) {
		ri.Printf(PRINT_ALL, "^1GX2BE: WHBGfxInitFetchShader FAILED\n");
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
	ri.Printf(PRINT_ALL, "GX2BE: bindings: VSBlock=%u PSBlock=%u GammaBlock=%u s_Base=%u s_Lightmap=%u\n",
	          (unsigned)s_vsBlockLoc, (unsigned)s_psBlockLoc, (unsigned)s_gammaBlockLoc,
	          (unsigned)s_baseSamplerLoc, (unsigned)s_lmSamplerLoc);

	/* Samplers: [wrap 0=clamp 1=repeat][mip 0=off 1=on] */
	for (w = 0; w < 2; w++)
		for (m = 0; m < 2; m++) {
			GX2InitSampler(&s_samplers[w][m],
			               w ? GX2_TEX_CLAMP_MODE_WRAP : GX2_TEX_CLAMP_MODE_CLAMP,
			               GX2_TEX_XY_FILTER_MODE_LINEAR);
			if (m) {
				GX2InitSamplerZMFilter(&s_samplers[w][m],
				                       GX2_TEX_Z_FILTER_MODE_POINT,
				                       GX2_TEX_MIP_FILTER_MODE_LINEAR);
				GX2InitSamplerLOD(&s_samplers[w][m], 0.0f, 13.0f, 0.0f);
			}
		}

	/* Without this, near-plane-straddling triangles rasterize as full-screen wedges (view weapon). */
	GX2SetRasterizerClipControl(TRUE, TRUE);

	/* Staging rings */
	s_ringVerts = memalign(256, sizeof(gx2vert_t) * GX2BE_RING_VERTS);
	s_ringIndexes = memalign(256, sizeof(uint32_t) * GX2BE_RING_INDEXES);
	s_vsRows = memalign(256, sizeof(uint32_t) * 64 * GX2BE_MAX_DRAWS);
	s_psRows = memalign(256, sizeof(uint32_t) * 64 * GX2BE_MAX_DRAWS);
	s_drcQuad = memalign(256, sizeof(gx2vert_t) * 4);
	if (!s_ringVerts || !s_ringIndexes || !s_vsRows || !s_psRows || !s_drcQuad) {
		ri.Printf(PRINT_ALL, "^1GX2BE: staging ring allocation FAILED\n");
		return qfalse;
	}

	/* 8x8 opaque white — fallback for unbound texture slots. */
	{
		static byte white[8 * 8 * 4];
		memset(white, 0xFF, sizeof(white));
		s_whiteTex = GX2Image_Upload(white, 8, 8, qtrue, qtrue);
		if (!s_whiteTex) {
			ri.Printf(PRINT_ALL, "^1GX2BE: white texture upload FAILED\n");
			return qfalse;
		}
	}

	/* Static DRC mirror quad: unit square, ortho baked into its own VS block. */
	{
		float m16[16];
		gx2vert_t *v = s_drcQuad;

		memset(v, 0, sizeof(gx2vert_t) * 4);
		v[0].x = 0; v[0].y = 0; v[0].s0 = 0; v[0].t0 = 0;
		v[1].x = 1; v[1].y = 0; v[1].s0 = 1; v[1].t0 = 0;
		v[2].x = 1; v[2].y = 1; v[2].s0 = 1; v[2].t0 = 1;
		v[3].x = 0; v[3].y = 1; v[3].s0 = 0; v[3].t0 = 1;
		for (i = 0; i < 4; i++) {
			v[i].rgba[0] = v[i].rgba[1] = v[i].rgba[2] = v[i].rgba[3] = 255;
		}
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER,
		              s_drcQuad, sizeof(gx2vert_t) * 4);

		GX2BE_MatOrtho(m16, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		for (i = 0; i < 16; i++)
			s_drcVsBlock[i] = GX2R_SwapF32(m16[i]);
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
		              s_drcVsBlock, sizeof(s_drcVsBlock));

		s_drcPsBlock[0] = GX2R_SwapF32(1.0f);   /* tex0 on */
		s_drcPsBlock[1] = 0;
		s_drcPsBlock[2] = 0;
		s_drcPsBlock[3] = 0;
		s_drcPsBlock[4] = GX2R_SwapF32(0.5f);
		s_drcPsBlock[5] = s_drcPsBlock[6] = s_drcPsBlock[7] = 0;
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
		              s_drcPsBlock, sizeof(s_drcPsBlock));

		memset(s_drcMapPsBlocks, 0, sizeof(s_drcMapPsBlocks));
		for (m = 0; m < 9; m++) {
			s_drcMapPsBlocks[m][0] = GX2R_SwapF32(m ? 1.0f : 0.0f);          /* tex0On */
			s_drcMapPsBlocks[m][1] = GX2R_SwapF32(m >= 5 ? 1.0f : 0.0f);     /* tex1On, modulate */
			s_drcMapPsBlocks[m][3] = GX2R_SwapF32(m ? (float)((m - 1) % 4) : 0.0f);
			s_drcMapPsBlocks[m][4] = GX2R_SwapF32(0.5f);
		}
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK,
		              s_drcMapPsBlocks, sizeof(s_drcMapPsBlocks));
	}

	/* TV color buffer viewed as a texture for the DRC mirror pass. */
	{
		GX2ColorBuffer *cb = WHBGfxGetTVColourBuffer();

		memset(&s_tvTex, 0, sizeof(s_tvTex));
		s_tvTex.surface = cb->surface;
		s_tvTex.viewFirstMip = 0;
		s_tvTex.viewNumMips = 1;
		s_tvTex.viewFirstSlice = 0;
		s_tvTex.viewNumSlices = 1;
		s_tvTex.compMap = 0x00010203;
		GX2InitTextureRegs(&s_tvTex);
		s_tvTexReady = qtrue;
	}

	return qtrue;
}

/* Must match the real TV buffer size -- can be 1080p, hardcoding 720p broke y-flip on hw. */
static void GX2BE_SetVidSizeFromBuffer(void)
{
	GX2ColorBuffer *cb = WHBGfxGetTVColourBuffer();

	if (cb && cb->surface.width && cb->surface.height) {
		glConfig.vidWidth = (int)cb->surface.width;
		glConfig.vidHeight = (int)cb->surface.height;
	} else {
		glConfig.vidWidth = 1280;
		glConfig.vidHeight = 720;
	}
	glConfig.windowAspect = (float)glConfig.vidWidth / (float)glConfig.vidHeight;
	ri.Printf(PRINT_ALL, "GX2BE: TV color buffer %dx%d\n",
	          glConfig.vidWidth, glConfig.vidHeight);
}

void GX2BE_GetDrcSize(int *width, int *height)
{
	GX2ColorBuffer *cb = s_inited ? WHBGfxGetDRCColourBuffer() : NULL;

	*width = (cb && cb->surface.width) ? (int)cb->surface.width : 854;
	*height = (cb && cb->surface.height) ? (int)cb->surface.height : 480;
}

GX2Texture *GX2BE_GetTexture(int texnum)
{
	if (texnum <= 0 || texnum > GX2BE_MAX_TEXTURES)
		return NULL;
	return s_textures[texnum].tex;
}

void GLimp_Init(qboolean fixedFunction)
{
	(void)fixedFunction;

	QGL_Init();

	glConfig.colorBits = 32;
	glConfig.depthBits = 24;
	glConfig.stencilBits = 8;
	glConfig.displayFrequency = 60;
	glConfig.isFullscreen = qtrue;
	glConfig.stereoEnabled = qfalse;
	glConfig.deviceSupportsGamma = qfalse;   /* shader-side r_gamma instead */
	glConfig.textureCompression = TC_NONE;
	glConfig.textureEnvAddAvailable = qtrue;
	glConfig.numTextureUnits = 2;
	glConfig.driverType = GLDRV_ICD;
	glConfig.hardwareType = GLHW_GENERIC;
	Q_strncpyz(glConfig.renderer_string, "GX2 (Latte) / renderergl1 backend",
	           sizeof(glConfig.renderer_string));
	Q_strncpyz(glConfig.vendor_string, "Nintendo/AMD", sizeof(glConfig.vendor_string));
	Q_strncpyz(glConfig.version_string, "GX2", sizeof(glConfig.version_string));
	Q_strncpyz(glConfig.extensions_string, "", sizeof(glConfig.extensions_string));

	if (s_inited) {
		/* vid_restart: GX2/WHBGfx teardown+reinit hangs on this platform, so context stays alive for process lifetime.
		 * r_swapInterval is CVAR_LATCH -- vid_restart is exactly when a changed value should take effect. */
		GX2SetSwapInterval((uint32_t)r_swapInterval->integer);
		GX2BE_SetVidSizeFromBuffer();
		ri.Printf(PRINT_ALL, "GLimp_Init: GX2 context kept alive (vid_restart), swap interval = %d\n",
		          r_swapInterval->integer);
		ri.IN_Init(NULL);
		return;
	}

	ri.Printf(PRINT_ALL, "GLimp_Init: WHBGfxInit...\n");
	if (!WHBGfxInit()) {
		ri.Error(ERR_FATAL, "GLimp_Init: WHBGfxInit FAILED");
		return;
	}
	s_inited = qtrue;
	ProcUIRegisterCallback(PROCUI_CALLBACK_RELEASE, GX2BE_ReleaseDrain, NULL, 1000);
	ProcUIRegisterCallback(PROCUI_CALLBACK_RELEASE, GX2BE_ReleaseDrain, NULL, 1);

	/* 0 = tears, 1 = vblank locked. Untested on real hw -- no known homebrew title
	 * runs interval 0 on this display controller. Archived cvar, so it's fixable without a rebuild. */
	GX2SetSwapInterval((uint32_t)r_swapInterval->integer);
	ri.Printf(PRINT_ALL, "GLimp_Init: swap interval = %d (%s)\n",
	          r_swapInterval->integer,
	          r_swapInterval->integer ? "vsync locked" : "vsync OFF -- tearing expected");

	GX2BE_SetVidSizeFromBuffer();

	s_pipelineReady = GX2BE_InitPipeline();
	if (s_pipelineReady)
		ri.Printf(PRINT_ALL, "GLimp_Init: GX2 backend pipeline ready\n");
	else
		ri.Printf(PRINT_ALL, "^1GLimp_Init: GX2 backend pipeline UNAVAILABLE\n");


#ifdef __WIIU__
	/* Checkpoint after pipeline ready, before IN_Init */
	{
	}
#endif
	/* ri.IN_Init assigns default K_PAD0_* binds; without it VPAD/KPAD buttons
	 * reach the engine unbound (only Plus->ESCAPE and raw axes work). */
	ri.IN_Init(NULL);
}

void GLimp_Shutdown(void)
{
	/* Keep GX2/WHBGfx alive across vid_restart (see GLimp_Init). */
	if (s_inited)
		ri.Printf(PRINT_ALL, "GLimp_Shutdown: keeping GX2 context alive (no-op)\n");
}

/* Exit-only teardown, never vid_restart -- skip this and the console just hangs on close. */
void GLimp_ShutdownFinal(void)
{
	if (!s_inited)
		return;

	ri.Printf(PRINT_ALL, "GLimp_ShutdownFinal: disabling TV/DRC, WHBGfxShutdown...\n");

	/* If foreground's already gone, raw GX2 calls touch revoked state -- straight to shutdown. */
	if (CON_IsForeground()) {
		GX2DrawDone();
		s_gpuPending = qfalse;
		GX2SetTVEnable(FALSE);
		GX2SetDRCEnable(FALSE);
	}
	WHBGfxShutdown();
	ri.Printf(PRINT_ALL, "GLimp_ShutdownFinal: WHBGfxShutdown returned\n");

	s_inited = qfalse;
	ri.Printf(PRINT_ALL, "GLimp_ShutdownFinal: done\n");
}

/* Throttled to every 30 calls: AVMIsAVOutReady() round-trips through IOSU. Logs only on state change. */
static void GX2BE_UpdateTVConnected(void)
{
	qboolean nowConnected;

	if ((s_tvCheckCounter++ % 30) != 0)
		return;

	nowConnected = AVMIsAVOutReady() ? qtrue : qfalse;
	if (nowConnected != s_tvConnected) {
		s_tvConnected = nowConnected;
		ri.Printf(PRINT_ALL, "GX2BE: TV %s\n",
		          s_tvConnected ? "connected -- resuming TV present" :
		                          "disconnected -- skipping TV present");
	}
}

/* GamePad map: textured batches in painter's order, then a per-frame portrait quad. */
static void GX2BE_DrawDrcMap(const gx2drcmapframe_t *f)
{
	GX2Texture *iconTex = NULL;
	gx2vert_t *icon;
	int i, blend = -1;   /* -1 forces the first batch to set the blend state */

	GX2SetFetchShader(&s_group.fetchShader);
	GX2SetVertexShader(s_group.vertexShader);
	GX2SetPixelShader(s_group.pixelShader);
	GX2SetShaderMode(GX2_SHADER_MODE_UNIFORM_BLOCK);

	GX2SetVertexUniformBlock(s_vsBlockLoc, 16 * sizeof(uint32_t), f->vsBlock);
	GX2SetPixelUniformBlock(s_gammaBlockLoc, sizeof(s_gammaBlock), s_gammaBlock);

	GX2SetColorControl(GX2_LOGIC_OP_COPY, 0x01, FALSE, TRUE);
	GX2SetDepthOnlyControl(FALSE, FALSE, GX2_COMPARE_FUNC_ALWAYS);
	GX2SetPolygonControl(GX2_FRONT_FACE_CCW, FALSE, FALSE, FALSE,
	                     GX2_POLYGON_MODE_TRIANGLE, GX2_POLYGON_MODE_TRIANGLE,
	                     FALSE, FALSE, FALSE);
	for (i = 0; i < f->numBatches; i++) {
		const gx2drcmapbatch_t *b = &f->batches[i];
		int mode = b->baseTex ? (b->lightmap ? 5 : 1) + (b->atest & 3) : 0;

		if (!b->numVerts)
			continue;
		if (b->blend != blend) {
			blend = b->blend;
			if (blend)
				GX2SetBlendControl(GX2_RENDER_TARGET_0,
				                   GX2_BLEND_MODE_SRC_ALPHA, GX2_BLEND_MODE_INV_SRC_ALPHA, GX2_BLEND_COMBINE_MODE_ADD,
				                   TRUE, GX2_BLEND_MODE_SRC_ALPHA, GX2_BLEND_MODE_INV_SRC_ALPHA, GX2_BLEND_COMBINE_MODE_ADD);
			else
				GX2SetBlendControl(GX2_RENDER_TARGET_0,
				                   GX2_BLEND_MODE_ONE, GX2_BLEND_MODE_ZERO, GX2_BLEND_COMBINE_MODE_ADD,
				                   TRUE, GX2_BLEND_MODE_ONE, GX2_BLEND_MODE_ZERO, GX2_BLEND_COMBINE_MODE_ADD);
		}
		GX2SetPixelUniformBlock(s_psBlockLoc, 8 * sizeof(uint32_t), s_drcMapPsBlocks[mode]);
		GX2SetPixelTexture(b->baseTex ? b->baseTex : s_whiteTex, s_baseSamplerLoc);
		GX2SetPixelSampler(&s_samplers[b->clamp ? 0 : 1][0], s_baseSamplerLoc);
		GX2SetPixelTexture(b->lightmap ? b->lightmap : s_whiteTex, s_lmSamplerLoc);
		GX2SetPixelSampler(&s_samplers[0][0], s_lmSamplerLoc);
		GX2SetAttribBuffer(0, b->numVerts * sizeof(gx2vert_t), sizeof(gx2vert_t), (void *)b->verts);
		GX2DrawEx(GX2_PRIMITIVE_MODE_TRIANGLES, b->numVerts, 0, 1);
	}

	icon = GX2BE_AllocVerts(4);
	if (!icon)
		return;
	memcpy(icon, f->icon, sizeof(f->icon));
	s_ringVertPos += 4;
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER, icon, sizeof(f->icon));

	if (f->iconTexnum > 0 && f->iconTexnum <= GX2BE_MAX_TEXTURES)
		iconTex = s_textures[f->iconTexnum].tex;

	GX2SetBlendControl(GX2_RENDER_TARGET_0,
	                   GX2_BLEND_MODE_SRC_ALPHA, GX2_BLEND_MODE_INV_SRC_ALPHA, GX2_BLEND_COMBINE_MODE_ADD,
	                   TRUE, GX2_BLEND_MODE_SRC_ALPHA, GX2_BLEND_MODE_INV_SRC_ALPHA, GX2_BLEND_COMBINE_MODE_ADD);
	GX2SetPixelUniformBlock(s_psBlockLoc, 8 * sizeof(uint32_t), s_drcPsBlock);
	GX2SetPixelTexture(iconTex ? iconTex : s_whiteTex, s_baseSamplerLoc);
	GX2SetPixelSampler(&s_samplers[0][0], s_baseSamplerLoc);
	GX2SetAttribBuffer(0, sizeof(f->icon), sizeof(gx2vert_t), icon);
	GX2DrawEx(GX2_PRIMITIVE_MODE_QUADS, 4, 0, 1);
}

/* WHBGfxFinishRender minus its GX2DrawDone: that wait serialized CPU and GPU every frame,
 * so any frame over 16.7 ms fell to 30 FPS under vsync. GX2BE_WaitGpu guards the reuse. */
static void GX2BE_SubmitFrame(void)
{
	GX2SwapScanBuffers();
	GX2Flush();
	s_gpuFrameTs = GX2GetLastSubmittedTimeStamp();
	s_gpuPending = qtrue;
	GX2SetTVEnable(TRUE);
	GX2SetDRCEnable(TRUE);
}

void GLimp_EndFrame(void)
{
	gx2drcmapframe_t mapFrame;
	/* Consumed every frame, drawn or not, so a stale view never carries into the next one. */
	qboolean drawMap = GX2DrcMap_GetFrame(&mapFrame);

	if (!s_inited)
		return;

	if (!s_frameOpen)
		return;   /* nothing drawn (backgrounded or pipeline down) */

	GX2BE_UpdateTVConnected();
	if (s_tvConnected)
		WHBGfxFinishRenderTV();

#ifdef WIIU_DEBUG_LOG
	{
		static int lastDrc = -1;
		int drc = (drawMap ? 1 : 0) | (s_tvConnected ? 2 : 0) | (s_tvTexReady ? 4 : 0);

		if (drc != lastDrc) {
			lastDrc = drc;
			ri.Printf(PRINT_ALL, "GamePad map: DRC %s (map=%d tv=%d tvTex=%d)\n",
			          drc == 7 ? "draws the map" : "mirrors the TV", drc & 1, (drc >> 1) & 1, (drc >> 2) & 1);
		}
	}
#endif

	/* DRC = the GamePad map, else one bilinear downscale of the finished TV frame (not per-draw
	 * scaling). The map needs a TV showing the game; GamePad-only play keeps the mirror. */
	if (s_tvTexReady && drawMap && s_tvConnected) {
		WHBGfxBeginRenderDRC();
		GX2BE_DrawDrcMap(&mapFrame);
		WHBGfxFinishRenderDRC();
	} else if (s_tvTexReady) {
		GX2ColorBuffer *cb = WHBGfxGetTVColourBuffer();

		WHBGfxBeginRenderDRC();

		GX2Invalidate(GX2_INVALIDATE_MODE_COLOR_BUFFER,
		              cb->surface.image, cb->surface.imageSize);

		GX2SetFetchShader(&s_group.fetchShader);
		GX2SetVertexShader(s_group.vertexShader);
		GX2SetPixelShader(s_group.pixelShader);
		GX2SetShaderMode(GX2_SHADER_MODE_UNIFORM_BLOCK);

		GX2SetVertexUniformBlock(s_vsBlockLoc, 16 * sizeof(uint32_t), s_drcVsBlock);
		GX2SetPixelUniformBlock(s_psBlockLoc, 8 * sizeof(uint32_t), s_drcPsBlock);
		GX2SetPixelUniformBlock(s_gammaBlockLoc, sizeof(s_gammaBlock), s_gammaBlock);

		GX2SetAttribBuffer(0, sizeof(gx2vert_t) * 4, sizeof(gx2vert_t), s_drcQuad);

		GX2SetColorControl(GX2_LOGIC_OP_COPY, 0x01, FALSE, TRUE);
		GX2SetBlendControl(GX2_RENDER_TARGET_0,
		                   GX2_BLEND_MODE_ONE, GX2_BLEND_MODE_ZERO, GX2_BLEND_COMBINE_MODE_ADD,
		                   TRUE, GX2_BLEND_MODE_ONE, GX2_BLEND_MODE_ZERO, GX2_BLEND_COMBINE_MODE_ADD);
		GX2SetDepthOnlyControl(FALSE, FALSE, GX2_COMPARE_FUNC_ALWAYS);
		GX2SetPolygonControl(GX2_FRONT_FACE_CCW, FALSE, FALSE, FALSE,
		                     GX2_POLYGON_MODE_TRIANGLE, GX2_POLYGON_MODE_TRIANGLE,
		                     FALSE, FALSE, FALSE);

		GX2SetPixelTexture(&s_tvTex, s_baseSamplerLoc);
		GX2SetPixelSampler(&s_samplers[0][0], s_baseSamplerLoc);
		GX2SetPixelTexture(s_whiteTex, s_lmSamplerLoc);
		GX2SetPixelSampler(&s_samplers[0][0], s_lmSamplerLoc);

		GX2DrawEx(GX2_PRIMITIVE_MODE_QUADS, 4, 0, 1);

		WHBGfxFinishRenderDRC();
	}

#ifdef WIIU_DEBUG_LOG
	if (s_diagFrames > 0) {
		OSTime t0 = OSGetTime();
		ri.Printf(PRINT_ALL, "DIAG[%d] FinishRender enter (frame %d left)\n", s_diagCycle, s_diagFrames);
		GX2BE_SubmitFrame();
		ri.Printf(PRINT_ALL, "DIAG[%d] FinishRender took %u us\n", s_diagCycle,
		          (unsigned)OSTicksToMicroseconds(OSGetTime() - t0));
		GX2BE_DiagSwap("post-FinishRender");
		s_diagFrames--;
	} else
#endif
	GX2BE_SubmitFrame();
	s_frameOpen = qfalse;
}

void GLimp_LogComment(const char *comment)
{
	(void)comment;
}

void GLimp_Minimize(void)
{
}

void GLimp_SetGamma(unsigned char red[256], unsigned char green[256], unsigned char blue[256])
{
	(void)red; (void)green; (void)blue;
	/* deviceSupportsGamma is qfalse — never called; r_gamma is shader-side. */
}

#endif /* WIIU_GX2BE */
