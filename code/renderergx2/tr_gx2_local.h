/* ioQuake3-U GX2 renderer: state shared by the refexport_t stub, display/2D pipeline,
 * shader loader, texture registry and GamePad map. */

#ifndef TR_GX2_LOCAL_H
#define TR_GX2_LOCAL_H

#include "../qcommon/q_shared.h"
#include "../renderercommon/tr_types.h"
#include "../renderercommon/tr_public.h"

#include <gx2/texture.h>
#include <gx2/shaders.h>

extern refimport_t ri;
extern glconfig_t  glConfig;

/* Shared vertex layout for the 2D quad ring and 3D world geometry, one fetch shader group serves both. */
typedef struct {
	float   x, y, z;
	float   s0, t0;
	float   s1, t1;
	byte    rgba[4];
} gx2vert_t;                     /* 32 bytes, matches fetch layout */

/* Stringified GLSL sources (generated from shaders/ by Makefile.client). */
extern const char *shaderSrc_q3uber_vert;
extern const char *shaderSrc_q3uber_frag;

/* GPU fetches uniform words raw LE, so every 32-bit word must be byte-swapped on this BE target. */
static inline uint32_t GX2R_SwapF32(float f)
{
	union { float f; uint32_t u; } v;
	v.f = f;
	return __builtin_bswap32(v.u);
}

/* ---- tr_gx2_init.c: display bring-up + 2D quad pipeline ---- */

/* WHBGfxInit + pipeline setup, idempotent -- context stays alive for process lifetime, GX2 teardown/reinit has hung before. */
void GX2R_Init(void);

/* Keeps GX2 alive (see above); logging only. */
void GX2R_Shutdown(void);

/* Resets the per-frame 2D quad ring. Cheap; no GX2 calls. */
void GX2R_BeginFrame(void);

/* Renders recorded 2D quads to TV+DRC and presents; no-op while backgrounded, or revoked MEM1 crashes hardware. */
void GX2R_EndFrame(void);

/* 2D draw recording, called from refexport_t entries. Coords in glConfig space, y-down. */
void GX2R_SetColor(const float *rgba);   /* NULL = opaque white */
void GX2R_StretchPic(float x, float y, float w, float h,
                     float s1, float t1, float s2, float t2, qhandle_t hShader);

/* ---- tr_gx2_shader.c: offline-precompiled q3uber shader loader ---- */

/* Loads one shader stage from the checked-in GFD blob; `source` unused/vestigial. No runtime compiler involved. */
GX2VertexShader *GX2Shader_CompileVS(const char *name, const char *source);
GX2PixelShader  *GX2Shader_CompilePS(const char *name, const char *source);

/* ---- tr_gx2_image.c: name -> GX2Texture registry ---- */

/* Creates the shared 8x8 white texture (handle GX2IMAGE_WHITE). */
qboolean GX2Image_Init(void);

/* Name -> texture handle (1-based, 0 invalid). Checks .shader scripts first, falls
 * back to image loaders; unresolvable names silently become the white texture. */
qhandle_t GX2Image_Register(const char *name);
qhandle_t GX2Image_RegisterNoMip(const char *name); /* UI/HUD/fonts: no mip chain */

/* Handle -> texture; invalid/unknown/script-only handles return the white texture (safe fallback). */
GX2Texture *GX2Image_GetTexture(qhandle_t handle);

/* RGBA8 pixels -> linear-tiled GX2Texture; also used directly for BSP lightmap pages. */
GX2Texture *GX2Image_Upload(const byte *pic, int width, int height, qboolean noMip, qboolean allowTiled);

/* Frees a texture from GX2Image_Upload never handed to the name registry (lightmap pages only). */
void GX2Image_FreeTexture(GX2Texture *tex);

/* Handle -> parsed multi-stage script, or NULL for a plain/unresolved image handle. */
const struct gx2ShaderScript_s *GX2Image_GetScript(qhandle_t handle);

/* Picks the stage that best represents a multi-stage material for single-pass
 * rendering: first real diffuse stage, else first non-environment, else stage 0. */
const struct gx2ShaderStage_s *GX2Image_GetRepresentativeStage(const struct gx2ShaderScript_s *script);

#define GX2IMAGE_WHITE 1

/* ---- tr_gx2_world.c: BSP static-geometry loader, brute-force no culling. The 256-byte
 * PS block alignment rule cost real hardware-debugging pain to nail down. ---- */

void      GX2World_Load(const char *name);
void      GX2World_Clear(void);
qboolean  GX2World_HasWorld(void);

gx2vert_t *GX2World_GetVertexBuffer(void);
uint32_t   GX2World_GetNumVerts(void);

typedef struct {
	qhandle_t       texHandle;   /* resolved base texture */
	const uint32_t *indices;
	uint32_t        numIndexes;
	GX2Texture     *lmTexture;   /* NULL = no lightmap page */
	const uint32_t *psBlock;     /* 256-aligned PS uniform block, swapped+invalidated at load */
	int             blendSrc;    /* GX2_BLEND_MODE_*; -1 = opaque draw */
	int             blendDst;
} gx2worldsurfinfo_t;

int       GX2World_GetNumSurfaces(void);
int       GX2World_GetNumOpaqueSurfaces(void); /* [0..n) opaque, [n..total) blended */
void      GX2World_GetSurface(int surfNum, gx2worldsurfinfo_t *out);

/* Skybox from "skyparms" outerbox: 24 camera-locked gx2vert_t, face i draws verts [i*4, i*4+4). */
qboolean         GX2World_HasSky(void);
const gx2vert_t *GX2World_GetSkyVerts(void);
qhandle_t        GX2World_GetSkyFaceTexture(int face);

/* Light-grid trilinear sample; qfalse if the map has no usable light grid. */
qboolean  GX2World_LightForPoint(const vec3_t point, vec3_t ambientLight,
                                 vec3_t directedLight, vec3_t lightDir);

/* ---- tr_gx2_model.c: MD3-only model loader + .skin registry. Bad/missing files
 * never crash, they just draw nothing. ---- */

qhandle_t GX2Model_Register(const char *name);
qboolean  GX2Model_LerpTag(orientation_t *tag, qhandle_t handle, int startFrame, int endFrame,
                           float frac, const char *tagName);
void      GX2Model_Bounds(qhandle_t handle, vec3_t mins, vec3_t maxs);

/* .skin registry. Returns 0 for missing/bad .skin — the one handle where 0 means "not found". */
qhandle_t GX2Skin_Register(const char *name);

/* Called per surface by GX2Model_Draw with a lerped CPU vertex/index buffer in
 * scratch memory, valid only for the call's duration — copy out if you need it. */
typedef void (*gx2ModelSurfaceFn)(void *ctx, qhandle_t texHandle,
                                  const gx2vert_t *verts, uint32_t numVerts,
                                  const uint32_t *indices, uint32_t numIndexes);

/* Per-entity draw params (lighting+skin). NULL = flat white, no skin. When
 * `lit`, diffuse = ambient + max(0,N.L)*directed, lightDir in entity-local space. */
typedef struct {
	qboolean  lit;
	vec3_t    ambientLight;
	vec3_t    directedLight;
	vec3_t    lightDir;      /* entity-local, normalized */
	qhandle_t customSkin;    /* 0 = none */
	int       skinNum;       /* embedded md3Shader_t index selector */
} gx2ModelDrawParams_t;

/* qfalse if the handle is invalid/failed to load (nothing emitted). */
qboolean GX2Model_Draw(qhandle_t handle, int frame, int oldframe, float backlerp,
                       const gx2ModelDrawParams_t *params,
                       gx2ModelSurfaceFn emit, void *ctx);

/* ---- tr_gx2_scene.c: refdef_t -> view/projection, RenderScene glue ---- */

void GX2Scene_RenderScene(const refdef_t *fd);

/* Entity list for the scene being built; filled by GX2_ClearScene/AddRefEntityToScene, consumed once per RenderScene. */
void GX2Scene_ClearEntities(void);
void GX2Scene_AddEntity(const refEntity_t *re);

/* ---- tr_gx2_init.c: 3D world draw (called from GX2R_EndFrame) ---- */

/* mvp16: column-major mat4, swapped to LE internally. Marks world pending for this frame; skip if RDF_NOWORLDMODEL. */
void GX2R_SetWorldMVP(const float *mvp16);

/* Rotation-only (camera-locked) mvp for the skybox, set only when the world has a sky. */
void GX2R_SetSkyMVP(const float *mvp16);

/* Queues one lerped entity surface for EndFrame; verts/indices copied internally,
 * safe to reuse scratch after. qfalse means buffers full -- drop this draw. */
qboolean GX2R_AddEntityDraw(const float *mvp16, qhandle_t texHandle,
                            const gx2vert_t *verts, uint32_t numVerts,
                            const uint32_t *indices, uint32_t numIndexes);

/* ---- tr_gx2_font.c: baseq3's pre-baked fontImage_*.dat loader ---- */

void GX2_RegisterFont(const char *fontName, int pointSize, fontInfo_t *font);

/* ---- tr_gx2_shaderscript.c: minimal .shader script parser ---- */

#define GX2_MAX_SHADER_STAGES 4

typedef struct gx2ShaderStage_s {
	/* Not resolved to imageHandle until actually registered — eager resolve
	 * during parse wasted the registry on textures nobody asked for. */
	char      imagePath[MAX_QPATH];
	qboolean  isWhiteRef;             /* $whiteimage / $lightmap, no path to load */
	qhandle_t imageHandle;            /* 0 = unresolved sentinel */
	int       blendSrc, blendDst;     /* GX2_BLEND_MODE_* */
	qboolean  hasConstColor;
	float     constColor[4];
	/* "tcGen environment" specular/reflection overlay, not the real diffuse texture. */
	qboolean  isEnvironmentMap;
	/* alphaFunc mapped to ubershader atest selector: 0 none, 1 GT0, 2 LT128, 3 GE128. */
	int       atestMode;
} gx2ShaderStage_t;

typedef struct gx2ShaderScript_s {
	char             name[MAX_QPATH];
	int              numStages;
	gx2ShaderStage_t stages[GX2_MAX_SHADER_STAGES];
	/* skyparms outerbox path ("-" = none); kept even with zero stages so world loader finds it by name. */
	qboolean         isSky;
	char             skyBox[MAX_QPATH];
} gx2ShaderScript_t;

/* Parses every scripts/ .shader file in the search path. Lazy/idempotent. */
void GX2ShaderScript_Init(void);

/* NULL if `name` isn't a known multi-stage shader — treat as a plain image. */
const gx2ShaderScript_t *GX2ShaderScript_Find(const char *name);

/* ---- tr_gx2_drcmap.c: GamePad top-down map, drawn by GLimp_EndFrame instead of the TV mirror ---- */

typedef struct {
	const gx2vert_t *verts;       /* 256-aligned run of triangles */
	uint32_t         numVerts;    /* only tris up to the player's height cutoff */
	GX2Texture      *baseTex;     /* NULL = flat vertex color (the backdrop) */
	GX2Texture      *lightmap;    /* NULL = lighting is in the vertex colors */
	int              clamp;       /* base sampler clamps instead of wrapping */
	int              atest;       /* ubershader alpha-test selector, 0..3 */
	int              blend;       /* alpha-blended (pickup icons) instead of opaque */
} gx2drcmapbatch_t;

typedef struct {
	const gx2drcmapbatch_t *batches;     /* [0] is the black backdrop, then low to high */
	int                     numBatches;
	const uint32_t         *vsBlock;     /* map ortho MVP, 256-aligned, swapped */
	gx2vert_t               icon[4];     /* portrait quad in map space */
	int                     iconTexnum;  /* 0 = no portrait found, draw a plain marker */
} gx2drcmapframe_t;

/* Consumes this frame's view and client state; qfalse = show the TV mirror. */
qboolean GX2DrcMap_GetFrame(gx2drcmapframe_t *out);

/* tr_gx2_backend.c: DRC color buffer size, and texnum -> uploaded texture (NULL if none). */
void        GX2BE_GetDrcSize(int *width, int *height);
GX2Texture *GX2BE_GetTexture(int texnum);

#endif /* TR_GX2_LOCAL_H */
