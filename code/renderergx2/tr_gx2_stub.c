/* refexport_t implementation. Real: 2D pipeline, shaders, BSP world, MD3 entities.
 * Still fake: skins, polys/dlights, cinematics -- gaps kept non-NULL or cgame Com_Errors. */

#include <string.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "tr_gx2_local.h"

refimport_t ri;
glconfig_t  glConfig;

static qboolean gx2_display_inited = qfalse;


static void GX2_Shutdown(qboolean destroyWindow)
{
	(void)destroyWindow;
	if (gx2_display_inited) {
		/* keeps GX2/WHBGfx alive -- re-init hung in the abandoned ANGLE era, not risking it again */
		GX2R_Shutdown();
	}
}

/* Fills glconfig_t with sane non-zero values -- cgame divides by
 * vidWidth/vidHeight, zeros crash immediately. */
static void GX2_BeginRegistration(glconfig_t *config)
{
	memset(&glConfig, 0, sizeof(glConfig));

	Q_strncpyz(glConfig.renderer_string, "GX2 stub (Session 0 -- no rendering)",
	           sizeof(glConfig.renderer_string));
	Q_strncpyz(glConfig.vendor_string, "ioQuake3-U", sizeof(glConfig.vendor_string));
	Q_strncpyz(glConfig.version_string, "0.0-stub", sizeof(glConfig.version_string));
	glConfig.extensions_string[0] = '\0';

	glConfig.maxTextureSize = 2048;
	glConfig.numTextureUnits = 2;

	glConfig.colorBits = 32;
	glConfig.depthBits = 24;
	glConfig.stencilBits = 8;

	glConfig.driverType = GLDRV_ICD;
	glConfig.hardwareType = GLHW_GENERIC;

	glConfig.deviceSupportsGamma = qfalse;
	glConfig.textureCompression = TC_NONE;
	glConfig.textureEnvAddAvailable = qfalse;

	glConfig.vidWidth = 1280;
	glConfig.vidHeight = 720;
	glConfig.windowAspect = (float)glConfig.vidWidth / (float)glConfig.vidHeight;

	glConfig.displayFrequency = 60;
	glConfig.isFullscreen = qtrue;
	glConfig.stereoEnabled = qfalse;
	glConfig.smpActive = qfalse;

	/* display bring-up gated behind this flag, see tr_gx2_init.c */
#ifdef WIIU_GX2_DISPLAY
	if (!gx2_display_inited) {
		GX2R_Init();
		gx2_display_inited = qtrue;
	}
#endif

	*config = glConfig;
}

static qhandle_t GX2_RegisterModel(const char *name)
{
	return GX2Model_Register(name);
}

static qhandle_t GX2_RegisterSkin(const char *name)
{
	return GX2Skin_Register(name);
}

static qhandle_t GX2_RegisterShader(const char *name)
{
	return GX2Image_Register(name);
}

static qhandle_t GX2_RegisterShaderNoMip(const char *name)
{
	return GX2Image_RegisterNoMip(name);
}

static void GX2_LoadWorld(const char *name) { GX2World_Load(name); }
static void GX2_SetWorldVisData(const byte *vis) { (void)vis; }
static void GX2_EndRegistration(void) { }

static void GX2_ClearScene(void) { GX2Scene_ClearEntities(); }
static void GX2_AddRefEntityToScene(const refEntity_t *re) { GX2Scene_AddEntity(re); }
static void GX2_AddPolyToScene(qhandle_t hShader, int numVerts, const polyVert_t *verts, int num)
{
	(void)hShader; (void)numVerts; (void)verts; (void)num;
}

static int GX2_LightForPoint(vec3_t point, vec3_t ambientLight, vec3_t directedLight, vec3_t lightDir)
{
	if (GX2World_LightForPoint(point, ambientLight, directedLight, lightDir))
		return qtrue;
	VectorClear(ambientLight);
	VectorClear(directedLight);
	VectorClear(lightDir);
	return qfalse;
}

static void GX2_AddLightToScene(const vec3_t org, float intensity, float r, float g, float b)
{
	(void)org; (void)intensity; (void)r; (void)g; (void)b;
}

static void GX2_AddAdditiveLightToScene(const vec3_t org, float intensity, float r, float g, float b)
{
	(void)org; (void)intensity; (void)r; (void)g; (void)b;
}

static void GX2_RenderScene(const refdef_t *fd) { GX2Scene_RenderScene(fd); }

static void GX2_SetColor(const float *rgba)
{
	GX2R_SetColor(rgba);
}

static void GX2_DrawStretchPic(float x, float y, float w, float h,
	float s1, float t1, float s2, float t2, qhandle_t hShader)
{
	GX2R_StretchPic(x, y, w, h, s1, t1, s2, t2, hShader);
}

static void GX2_DrawStretchRaw(int x, int y, int w, int h, int cols, int rows,
	const byte *data, int client, qboolean dirty)
{
	(void)x; (void)y; (void)w; (void)h; (void)cols; (void)rows; (void)data; (void)client; (void)dirty;
}

static void GX2_UploadCinematic(int w, int h, int cols, int rows, const byte *data, int client, qboolean dirty)
{
	(void)w; (void)h; (void)cols; (void)rows; (void)data; (void)client; (void)dirty;
}

static void GX2_BeginFrame(stereoFrame_t stereoFrame)
{
	(void)stereoFrame;
	if (gx2_display_inited)
		GX2R_BeginFrame();
}

static void GX2_EndFrame(int *frontEndMsec, int *backEndMsec)
{
	if (frontEndMsec) *frontEndMsec = 0;
	if (backEndMsec)  *backEndMsec = 0;
	if (gx2_display_inited)
		GX2R_EndFrame();
}

static int GX2_MarkFragments(int numPoints, const vec3_t *points, const vec3_t projection,
	int maxPoints, vec3_t pointBuffer, int maxFragments, markFragment_t *fragmentBuffer)
{
	(void)numPoints; (void)points; (void)projection;
	(void)maxPoints; (void)pointBuffer; (void)maxFragments; (void)fragmentBuffer;
	return 0;
}

static int GX2_LerpTag(orientation_t *tag, qhandle_t model, int startFrame, int endFrame,
	float frac, const char *tagName)
{
	return GX2Model_LerpTag(tag, model, startFrame, endFrame, frac, tagName);
}

static void GX2_ModelBounds(qhandle_t model, vec3_t mins, vec3_t maxs)
{
	GX2Model_Bounds(model, mins, maxs);
}

/* implementation in tr_gx2_font.c */

static void GX2_RemapShader(const char *oldShader, const char *newShader, const char *offsetTime)
{
	(void)oldShader; (void)newShader; (void)offsetTime;
}

static qboolean GX2_GetEntityToken(char *buffer, int size)
{
	(void)buffer; (void)size;
	return qfalse;
}

static qboolean GX2_inPVS(const vec3_t p1, const vec3_t p2)
{
	(void)p1; (void)p2;
	return qtrue;
}

static void GX2_TakeVideoFrame(int h, int w, byte *captureBuffer, byte *encodeBuffer, qboolean motionJpeg)
{
	(void)h; (void)w; (void)captureBuffer; (void)encodeBuffer; (void)motionJpeg;
}

static refexport_t re;

refexport_t *GetRefAPI(int apiVersion, refimport_t *rimp)
{
	if (apiVersion != REF_API_VERSION) {
		return NULL;
	}

	ri = *rimp;
	memset(&re, 0, sizeof(re));

	re.Shutdown = GX2_Shutdown;
	re.BeginRegistration = GX2_BeginRegistration;
	re.RegisterModel = GX2_RegisterModel;
	re.RegisterSkin = GX2_RegisterSkin;
	re.RegisterShader = GX2_RegisterShader;
	re.RegisterShaderNoMip = GX2_RegisterShaderNoMip;
	re.LoadWorld = GX2_LoadWorld;
	re.SetWorldVisData = GX2_SetWorldVisData;
	re.EndRegistration = GX2_EndRegistration;
	re.ClearScene = GX2_ClearScene;
	re.AddRefEntityToScene = GX2_AddRefEntityToScene;
	re.AddPolyToScene = GX2_AddPolyToScene;
	re.LightForPoint = GX2_LightForPoint;
	re.AddLightToScene = GX2_AddLightToScene;
	re.AddAdditiveLightToScene = GX2_AddAdditiveLightToScene;
	re.RenderScene = GX2_RenderScene;
	re.SetColor = GX2_SetColor;
	re.DrawStretchPic = GX2_DrawStretchPic;
	re.DrawStretchRaw = GX2_DrawStretchRaw;
	re.UploadCinematic = GX2_UploadCinematic;
	re.BeginFrame = GX2_BeginFrame;
	re.EndFrame = GX2_EndFrame;
	re.MarkFragments = GX2_MarkFragments;
	re.LerpTag = GX2_LerpTag;
	re.ModelBounds = GX2_ModelBounds;
	re.RegisterFont = GX2_RegisterFont;
	re.RemapShader = GX2_RemapShader;
	re.GetEntityToken = GX2_GetEntityToken;
	re.inPVS = GX2_inPVS;
	re.TakeVideoFrame = GX2_TakeVideoFrame;

	return &re;
}
