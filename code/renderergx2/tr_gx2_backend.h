/* ioQuake3-U GX2 backend for the stock renderergl1 frontend (WIIU_GX2BE only):
 * fixed-function GL state maps to ubershader uniforms + GX2 render state. */
#ifndef TR_GX2_BACKEND_H
#define TR_GX2_BACKEND_H

#if defined(WIIU_GX2BE)

/* Shadow state mirroring glState_t: matrices, client array ptrs, texenv/atest for ubershader uniform. */
typedef struct {
	int      currenttmu;      /* 0 or 1 */
	int      boundtex[2];     /* texnum per TMU; -1 = none */
	int      texenv[2];       /* pending GL_MODULATE/REPLACE/ADD per TMU */
	int      numActiveTMUs;   /* 1 or 2 */
	int      faceCulling;     /* last CT_* value; -1 = force-set next call */
	unsigned long stateBits;  /* last GLS_* bits applied */
	float    depthNear;       /* depth range [0,1] */
	float    depthFar;
	int      vpX, vpY, vpW, vpH; /* viewport, GL bottom-up coords */
	int      polyOffsetOn;

	/* Client vertex arrays; DrawTess reads these, qgl wrappers set them. */
	const void *posPtr;
	const void *clrPtr;
	const void *texPtr[2];
	int      posStride;
	int      clrStride;
	int      texStride[2];
	int      tex2DEnabled[2];

	/* Last-loaded matrices, GL column-major float[16]. */
	float    projMtx[16];
	float    mvMtx[16];
} gx2be_state_t;

extern gx2be_state_t gx2beState;

/* Init / frame */
void GX2BE_SetDefaultState(void);
void GX2BE_Finish(void);            /* qglFinish: GX2DrawDone */
/* Frames are submitted without GX2DrawDone; call before the CPU writes or frees GPU-read memory. */
void GX2BE_WaitGpu(void);

/* GLimp_* also live in tr_gx2_backend.c. */

/* State */
void GX2BE_GL_State(unsigned long stateBits);
void GX2BE_GL_Cull(int cullType);
void GX2BE_GL_TexEnv(int unit, int env);
void GX2BE_BindTexnum(int tmu, int texnum);

/* Matrices / viewport. Inputs are Q3's column-major GL float[16]. */
void GX2BE_LoadOrtho2D(int vidWidth, int vidHeight);
void GX2BE_LoadOrthoGL(float left, float right, float bottom, float top,
                       float znear, float zfar);   /* flares */
void GX2BE_LoadIdentityModelview(void);
void GX2BE_SetViewport(int x, int y, int w, int h);  /* GL bottom-up coords */
void GX2BE_SetScissor(int x, int y, int w, int h);
void GX2BE_DepthRange(float n, float f);
void GX2BE_PolygonOffset(float factor, float units);
void GX2BE_SetPolygonOffsetEnabled(int enabled);
void GX2BE_LoadProjectionGL(const float *gl16);
void GX2BE_LoadProjectionObliqueGL(const float *gl16, const float *eyePlane);
void GX2BE_LoadModelviewGL(const float *gl16);
void GX2BE_LoadModelviewTranslatedGL(const float *gl16, const vec3_t origin);

/* Clear via GX2ClearBuffersEx + mandatory context re-bind -- skip the re-bind and
 * hardware kills the context. Cost a whole investigation to find. */
void GX2BE_Clear(qboolean clearColor, qboolean clearDepth, float r, float g, float b);

/* Immediate-mode emulation for sky/cinematic/beam/axis paths; numVerts must match exactly. */
void GX2BE_ImmediateBegin(int primGL, int numVerts);
void GX2BE_ImmediateTexVertex(const float *st, const float *xyz,
                              byte r, byte g, byte b, byte a);
void GX2BE_ImmediateEnd(void);

/* Client array pointers, called from qgl wrappers / choke arms. */
void GX2BE_SetVertexPtr(const void *p, int stride);
void GX2BE_SetColorPtr(const void *p, int stride);
void GX2BE_SetTexCoordPtr(const void *p, int stride); /* currenttmu */
void GX2BE_SetTexture2DEnabled(int enabled);          /* currenttmu */

/* Stages tess client arrays into per-frame ring, applies GX2 state + uniforms, GX2DrawIndexedEx. */
void GX2BE_DrawTess(int numIndexes, const unsigned int *indexes);

/* PeekDepth backs RB_TestFlare (stub: always 1.0, no real depth readback yet).
 * ReadPixelsRGB backs screenshots, reads TV color buffer. */
float GX2BE_PeekDepth(int x, int y);
void  GX2BE_ReadPixelsRGB(int x, int y, int w, int h, int padlen, byte *dst);

/* Texture ops. Upload32 takes final scaled/picmipped RGBA8; mip chain generated internally (data not mutated). */
void GX2BE_CreateTexnum(unsigned int *texnum);
void GX2BE_DeleteTexnum(int texnum);
void GX2BE_DeleteAllTexnums(void);
void GX2BE_Upload32(unsigned *data, int width, int height,
                    int glInternalFormat, int texnum, int wrapClampMode,
                    qboolean mipmap, qboolean allowTiled);
void GX2BE_TexSubImage2D(int texnum, int fullW, int fullH, const byte *data);

/* GamePad map (tr_gx2_drcmap.c): RE_RenderScene reports each world view's origin. */
void GX2DrcMap_SetView(const vec3_t vieworg);

#endif /* WIIU_GX2BE */

#endif /* TR_GX2_BACKEND_H */
