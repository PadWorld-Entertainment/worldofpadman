/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// tr_init.c -- functions that are not called every frame

#include "tr_local.h"
#include "tr_dsa.h"
#include "../renderercommon/tr_shared.h"

glconfig_t glConfig;
glRefConfig_t glRefConfig;
glstate_t glState;

static void GfxInfo_f(void);
static void GfxMemInfo_f(void);

#ifdef USE_RENDERER_DLOPEN
cvar_t *com_altivec;
#endif

cvar_t *r_flareSize;
cvar_t *r_flareFade;
cvar_t *r_flareCoeff;

cvar_t *r_railWidth;
cvar_t *r_railCoreWidth;
cvar_t *r_railSegmentLength;

cvar_t *r_verbose;
cvar_t *r_ignore;

cvar_t *r_displayRefresh;

cvar_t *r_detailTextures;

cvar_t *r_znear;
cvar_t *r_zproj;
cvar_t *r_stereoSeparation;

cvar_t *r_skipBackEnd;

cvar_t *r_stereoEnabled;
cvar_t *r_anaglyphMode;

cvar_t *r_greyscale;

cvar_t *r_ignorehwgamma;
cvar_t *r_measureOverdraw;

cvar_t *r_inGameVideo;
cvar_t *r_fastsky;
cvar_t *r_drawSun;
cvar_t *r_dynamiclight;
cvar_t *r_dlightBacks;

cvar_t *r_lodbias;
cvar_t *r_lodscale;

cvar_t *r_norefresh;
cvar_t *r_drawentities;
cvar_t *r_drawworld;
cvar_t *r_speeds;
cvar_t *r_fullbright;
cvar_t *r_novis;
cvar_t *r_nocull;
cvar_t *r_facePlaneCull;
cvar_t *r_showcluster;
cvar_t *r_nocurves;

cvar_t *r_allowExtensions;

cvar_t *r_ext_compressed_textures;
cvar_t *r_ext_multitexture;
cvar_t *r_ext_compiled_vertex_array;
cvar_t *r_ext_texture_env_add;
cvar_t *r_ext_texture_filter_anisotropic;
cvar_t *r_ext_max_anisotropy;

cvar_t *r_ext_framebuffer_object;
cvar_t *r_ext_texture_float;
cvar_t *r_ext_framebuffer_multisample;
cvar_t *r_arb_seamless_cube_map;
cvar_t *r_arb_vertex_array_object;
cvar_t *r_ext_direct_state_access;

cvar_t *r_cameraExposure;

cvar_t *r_externalGLSL;

cvar_t *r_hdr;
cvar_t *r_floatLightmap;
cvar_t *r_postProcess;

cvar_t *r_toneMap;
cvar_t *r_forceToneMap;
cvar_t *r_forceToneMapMin;
cvar_t *r_forceToneMapAvg;
cvar_t *r_forceToneMapMax;

cvar_t *r_autoExposure;
cvar_t *r_forceAutoExposure;
cvar_t *r_forceAutoExposureMin;
cvar_t *r_forceAutoExposureMax;

cvar_t *r_depthPrepass;
cvar_t *r_ssao;

cvar_t *r_normalMapping;
cvar_t *r_specularMapping;
cvar_t *r_deluxeMapping;
cvar_t *r_parallaxMapping;
cvar_t *r_parallaxMapOffset;
cvar_t *r_parallaxMapShadows;
cvar_t *r_cubeMapping;
cvar_t *r_cubemapSize;
cvar_t *r_deluxeSpecular;
cvar_t *r_pbr;
cvar_t *r_baseNormalX;
cvar_t *r_baseNormalY;
cvar_t *r_baseParallax;
cvar_t *r_baseSpecular;
cvar_t *r_baseGloss;
cvar_t *r_glossType;
cvar_t *r_mergeLightmaps;
cvar_t *r_dlightMode;
cvar_t *r_pshadowDist;
cvar_t *r_imageUpsample;
cvar_t *r_imageUpsampleMaxSize;
cvar_t *r_imageUpsampleType;
cvar_t *r_genNormalMaps;
cvar_t *r_forceSun;
cvar_t *r_forceSunLightScale;
cvar_t *r_forceSunAmbientScale;
cvar_t *r_sunlightMode;
cvar_t *r_drawSunRays;
cvar_t *r_sunShadows;
cvar_t *r_shadowFilter;
cvar_t *r_shadowBlur;
cvar_t *r_shadowMapSize;
cvar_t *r_shadowCascadeZNear;
cvar_t *r_shadowCascadeZFar;
cvar_t *r_shadowCascadeZBias;
cvar_t *r_ignoreDstAlpha;

cvar_t *r_ignoreGLErrors;
cvar_t *r_logFile;

cvar_t *r_stencilbits;
cvar_t *r_depthbits;
cvar_t *r_colorbits;
cvar_t *r_texturebits;
cvar_t *r_ext_multisample;

cvar_t *r_drawBuffer;
cvar_t *r_lightmap;
cvar_t *r_vertexLight;
cvar_t *r_uiFullScreen;
cvar_t *r_shadows;
cvar_t *r_flares;
cvar_t *r_mode;
cvar_t *r_nobind;
cvar_t *r_singleShader;
cvar_t *r_roundImagesDown;
cvar_t *r_colorMipLevels;
cvar_t *r_picmip;
cvar_t *r_showtris;
cvar_t *r_showsky;
cvar_t *r_shownormals;
cvar_t *r_finish;
cvar_t *r_clear;
cvar_t *r_swapInterval;
cvar_t *r_textureMode;
cvar_t *r_offsetFactor;
cvar_t *r_offsetUnits;
cvar_t *r_gamma;
cvar_t *r_intensity;
cvar_t *r_lockpvs;
cvar_t *r_noportals;
cvar_t *r_portalOnly;

cvar_t *r_subdivisions;
cvar_t *r_lodCurveError;

cvar_t *r_fullscreen;
cvar_t *r_noborder;

cvar_t *r_customwidth;
cvar_t *r_customheight;
cvar_t *r_customPixelAspect;

cvar_t *r_overBrightBits;
cvar_t *r_mapOverBrightBits;

cvar_t *r_debugSurface;
cvar_t *r_simpleMipMaps;

cvar_t *r_showImages;

cvar_t *r_ambientScale;
cvar_t *r_directedScale;
cvar_t *r_debugLight;
cvar_t *r_debugSort;
cvar_t *r_printShaders;
cvar_t *r_saveFontData;

cvar_t *r_marksOnTriangleMeshes;

cvar_t *r_aviMotionJpegQuality;
cvar_t *r_screenshotJpegQuality;
cvar_t *r_screenshotFormat;

cvar_t *r_maxpolys;
int max_polys;
cvar_t *r_maxpolyverts;
int max_polyverts;

/*
** InitOpenGL
**
** This function is responsible for initializing a valid OpenGL subsystem.  This
** is done by calling GLimp_Init (which gives us a working OGL subsystem) then
** setting variables, checking GL constants, and reporting the gfx system config
** to the user.
*/
static void InitOpenGL(void) {
	//
	// initialize OS specific portions of the renderer
	//
	// GLimp_Init directly or indirectly references the following cvars:
	//		- r_fullscreen
	//		- r_mode
	//		- r_(color|depth|stencil)bits
	//		- r_ignorehwgamma
	//		- r_gamma
	//

	if (glConfig.vidWidth == 0) {
		GLint temp;

		GLimp_Init(qfalse);
		GLimp_InitExtraExtensions();

		glConfig.textureEnvAddAvailable = qtrue;

		// OpenGL driver constants
		qglGetIntegerv(GL_MAX_TEXTURE_SIZE, &temp);
		glConfig.maxTextureSize = temp;

		// stubbed or broken drivers may have reported 0...
		if (glConfig.maxTextureSize <= 0) {
			glConfig.maxTextureSize = 0;
		}

		qglGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &temp);
		glConfig.numTextureUnits = temp;

		// reserve 160 components for other uniforms
		qglGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &temp);
		glRefConfig.glslMaxAnimatedBones = Com_Clamp(0, IQM_MAX_JOINTS, (temp - 160) / 16);
		if (glRefConfig.glslMaxAnimatedBones < 12) {
			glRefConfig.glslMaxAnimatedBones = 0;
		}
	}

	// check for GLSL function textureCubeLod()
	if (r_cubeMapping->integer && !QGL_VERSION_ATLEAST(3, 0)) {
		ri.Printf(PRINT_WARNING, "WARNING: Disabled r_cubeMapping because it requires OpenGL 3.0\n");
		ri.Cvar_Set("r_cubeMapping", "0");
	}

	// set default state
	GL_SetDefaultState();
}

/*
==================
GL_CheckErrors
==================
*/
void GL_CheckErrs(char *file, int line) {
	int err;
	char s[64];

	err = qglGetError();
	if (err == GL_NO_ERROR) {
		return;
	}
	if (r_ignoreGLErrors->integer) {
		return;
	}
	switch (err) {
	case GL_INVALID_ENUM:
		strcpy(s, "GL_INVALID_ENUM");
		break;
	case GL_INVALID_VALUE:
		strcpy(s, "GL_INVALID_VALUE");
		break;
	case GL_INVALID_OPERATION:
		strcpy(s, "GL_INVALID_OPERATION");
		break;
	case GL_STACK_OVERFLOW:
		strcpy(s, "GL_STACK_OVERFLOW");
		break;
	case GL_STACK_UNDERFLOW:
		strcpy(s, "GL_STACK_UNDERFLOW");
		break;
	case GL_OUT_OF_MEMORY:
		strcpy(s, "GL_OUT_OF_MEMORY");
		break;
	default:
		Com_sprintf(s, sizeof(s), "%i", err);
		break;
	}

	ri.Error(ERR_FATAL, "GL_CheckErrors: %s in %s at line %d", s, file, line);
}

/*
** R_GetModeInfo
*/
typedef struct vidmode_s {
	const char *description;
	int width, height;
	float pixelAspect; // pixel width / height
} vidmode_t;

vidmode_t r_vidModes[] = {{"Mode  0: 320x240", 320, 240, 1},	 {"Mode  1: 400x300", 400, 300, 1},
						  {"Mode  2: 512x384", 512, 384, 1},	 {"Mode  3: 640x480", 640, 480, 1},
						  {"Mode  4: 800x600", 800, 600, 1},	 {"Mode  5: 960x720", 960, 720, 1},
						  {"Mode  6: 1024x768", 1024, 768, 1},	 {"Mode  7: 1152x864", 1152, 864, 1},
						  {"Mode  8: 1280x1024", 1280, 1024, 1}, {"Mode  9: 1600x1200", 1600, 1200, 1},
						  {"Mode 10: 2048x1536", 2048, 1536, 1}, {"Mode 11: 856x480 (wide)", 856, 480, 1}};
static int s_numVidModes = ARRAY_LEN(r_vidModes);

qboolean R_GetModeInfo(int *width, int *height, float *windowAspect, int mode) {
	vidmode_t *vm;
	float pixelAspect;

	if (mode < -1) {
		return qfalse;
	}
	if (mode >= s_numVidModes) {
		return qfalse;
	}

	if (mode == -1) {
		*width = r_customwidth->integer;
		*height = r_customheight->integer;
		pixelAspect = r_customPixelAspect->value;
	} else {
		vm = &r_vidModes[mode];

		*width = vm->width;
		*height = vm->height;
		pixelAspect = vm->pixelAspect;
	}

	*windowAspect = (float)*width / (*height * pixelAspect);

	return qtrue;
}

/*
** R_ModeList_f
*/
static void R_ModeList_f(void) {
	int i;

	ri.Printf(PRINT_ALL, "\n");
	for (i = 0; i < s_numVidModes; i++) {
		ri.Printf(PRINT_ALL, "%s\n", r_vidModes[i].description);
	}
	ri.Printf(PRINT_ALL, "\n");
}

/*
==============================================================================

						SCREEN SHOTS

NOTE TTimo
some thoughts about the screenshots system:
screenshots get written in fs_homepath + fs_gamedir
vanilla q3 .. baseq3/screenshots/ *.tga
team arena .. missionpack/screenshots/ *.tga

two commands: "screenshot" and "screenshotJPEG"
we use statics to store a count and start writing the first screenshot/screenshot????.tga (.jpg) available
(with FS_FileExists / FS_FOpenFileWrite calls)
FIXME: the statics don't get a reinit between fs_game changes

==============================================================================
*/

/*
==================
RB_ReadPixels

Reads an image but takes care of alignment issues for reading RGB images.

Reads a minimum offset for where the RGB data starts in the image from
integer stored at pointer offset. When the function has returned the actual
offset was written back to address offset. This address will always have an
alignment of packAlign to ensure efficient copying.

Stores the length of padding after a line of pixels to address padlen

Return value must be freed with ri.Hunk_FreeTempMemory()
==================
*/

static byte *RB_ReadPixels(int x, int y, int width, int height, size_t *offset, int *padlen) {
	byte *buffer, *bufstart;
	int padwidth, linelen;
	GLint packAlign = 0;

	qglGetIntegerv(GL_PACK_ALIGNMENT, &packAlign);

	linelen = width * 3;
	padwidth = PAD(linelen, packAlign);

	// Allocate a few more bytes so that we can choose an alignment we like
	buffer = ri.Hunk_AllocateTempMemory(padwidth * height + *offset + packAlign - 1);

	bufstart = PADP((intptr_t)buffer + *offset, packAlign);

	qglReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, bufstart);

	*offset = bufstart - buffer;
	*padlen = padwidth - linelen;

	return buffer;
}

/*
==================
RB_TakeScreenshotTGA
==================
*/
static void RB_TakeScreenshotTGA(int x, int y, int width, int height, const char *fileName) {
	byte *buffer;
	size_t offset = 0, memcount;
	int padlen;

	buffer = RB_ReadPixels(x, y, width, height, &offset, &padlen);
	memcount = (width * 3 + padlen) * height;

	// gamma correction
	if (glConfig.deviceSupportsGamma)
		R_GammaCorrect(buffer + offset, memcount);

	RE_SaveTGA(fileName, width, height, buffer + offset, padlen);
	ri.Hunk_FreeTempMemory(buffer);
}

/*
==================
RB_TakeScreenshotJPEG
==================
*/
static void RB_TakeScreenshotJPEG(int x, int y, int width, int height, const char *fileName) {
	byte *buffer;
	size_t offset = 0, memcount;
	int padlen;

	buffer = RB_ReadPixels(x, y, width, height, &offset, &padlen);
	memcount = (width * 3 + padlen) * height;

	// gamma correction
	if (glConfig.deviceSupportsGamma)
		R_GammaCorrect(buffer + offset, memcount);

	RE_SaveJPG(fileName, r_screenshotJpegQuality->integer, width, height, buffer + offset, padlen);
	ri.Hunk_FreeTempMemory(buffer);
}

/*
==================
RB_TakeScreenshotPNG
==================
*/
static void RB_TakeScreenshotPNG(int x, int y, int width, int height, const char *fileName)
{
	byte *buffer;
	size_t offset = 0, memcount;
	int padlen;

	buffer = RB_ReadPixels(x, y, width, height, &offset, &padlen);
	memcount = (width * 3 + padlen) * height;

	// gamma correct
	if(glConfig.deviceSupportsGamma)
		R_GammaCorrect(buffer + offset, memcount);

	RE_SavePNG(fileName, width, height, buffer + offset, padlen);
	ri.Hunk_FreeTempMemory(buffer);
}

/*
==================
RB_TakeScreenshotCmd
==================
*/
const void *RB_TakeScreenshotCmd(const void *data) {
	const screenshotCommand_t *cmd;

	cmd = (const screenshotCommand_t *)data;

	// finish any 2D drawing if needed
	if (tess.numIndexes)
		RB_EndSurface();

	if (cmd->type == ST_TGA)
		RB_TakeScreenshotTGA(cmd->x, cmd->y, cmd->width, cmd->height, cmd->fileName);
	else if (cmd->type == ST_JPEG)
		RB_TakeScreenshotJPEG(cmd->x, cmd->y, cmd->width, cmd->height, cmd->fileName);
	else if (cmd->type == ST_PNG)
		RB_TakeScreenshotPNG(cmd->x, cmd->y, cmd->width, cmd->height, cmd->fileName);

	return (const void *)(cmd + 1);
}

/*
==================
R_TakeScreenshot
==================
*/
static void R_TakeScreenshot(int x, int y, int width, int height, char *name, screenshotType_e type) {
	static char fileName[MAX_OSPATH]; // bad things if two screenshots per frame?
	screenshotCommand_t *cmd;

	cmd = R_GetCommandBuffer(sizeof(*cmd));
	if (!cmd) {
		return;
	}
	cmd->commandId = RC_SCREENSHOT;

	cmd->x = x;
	cmd->y = y;
	cmd->width = width;
	cmd->height = height;
	Q_strncpyz(fileName, name, sizeof(fileName));
	cmd->fileName = fileName;
	cmd->type = type;
}

/*
====================
R_LevelShot

levelshots are specialized thumbnails for the menu system,
sampled down from full screen distorted images
====================
*/
static void R_LevelShot(screenshotType_e type, const char *ext) {
	char fileName[MAX_OSPATH];
	byte *source, *allsource;
	byte *resample;
	size_t offset = 0, memcount;
	int spadlen;
	int linelen;
	byte *src, *dst;
	int x, y;
	int r, g, b;
	float xScale, yScale;
	int xx, yy;
	int width, height;
	int arg;
	int lscount;

	// Allow custom resample width/height
	arg = atoi(ri.Cmd_Argv(2));
	if (arg > 0)
		width = height = arg;
	else
		width = height = 256;

	if (width > glConfig.vidWidth)
		width = glConfig.vidWidth;
	if (height > glConfig.vidHeight)
		height = glConfig.vidHeight;

	// scan for a free number
	for (lscount = 1; lscount <= MAX_LEVELSHOTS; lscount++) {
		Com_sprintf(fileName, sizeof(fileName), "levelshots/%sB%i%s", tr.world->baseName, lscount, ext);

		if (!ri.FS_FileExists(fileName)) {
			break; // file doesn't exist
		}
	}
	
	if (lscount > MAX_LEVELSHOTS) {
		ri.Printf(PRINT_ALL, "LevelShot: Couldn't create a file\n");
		return;
	}

	allsource = RB_ReadPixels(0, 0, glConfig.vidWidth, glConfig.vidHeight, &offset, &spadlen);
	source = allsource + offset;

	linelen = width * 3;
	memcount = linelen * height;

	resample = ri.Hunk_AllocateTempMemory(memcount);

	// resample from source
	xScale = glConfig.vidWidth / (float)(width * 4.0f);
	yScale = glConfig.vidHeight / (float)(height * 3.0f);
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			r = g = b = 0;
			for (yy = 0; yy < 3; yy++) {
				for (xx = 0; xx < 4; xx++) {
					src = source + (3 * glConfig.vidWidth + spadlen) * (int)((y * 3 + yy) * yScale) +
						  3 * (int)((x * 4 + xx) * xScale);
					r += src[0];
					g += src[1];
					b += src[2];
				}
			}
			dst = resample + 3 * (y * width + x);
			dst[0] = r / 12;
			dst[1] = g / 12;
			dst[2] = b / 12;
		}
	}

	// gamma correction
	if (glConfig.deviceSupportsGamma) {
		R_GammaCorrect(resample, memcount);
	}

	if (type == ST_TGA)
		RE_SaveTGA(fileName, width, height, resample, 0);
	else if (type == ST_JPEG)
		RE_SaveJPG(fileName, r_screenshotJpegQuality->integer, width, height, resample, 0);
	else if (type == ST_PNG)
		RE_SavePNG(fileName, width, height, resample, 0);

	ri.Hunk_FreeTempMemory(resample);
	ri.Hunk_FreeTempMemory(allsource);

	ri.Printf(PRINT_ALL, "Wrote %s\n", fileName);
}

static void R_ScreenShot(screenshotType_e type) {
	char checkname[MAX_OSPATH];
	static int lastNumber = -1;
	qboolean silent;

	static const char *exts[] = {
		".tga", ".jpg", ".png"
	};

	if (!strcmp(ri.Cmd_Argv(1), "levelshot")) {
		R_LevelShot(ST_JPEG, exts[ST_JPEG]);
		return;
	}

	if (!strcmp(ri.Cmd_Argv(1), "silent")) {
		silent = qtrue;
	} else {
		silent = qfalse;
	}

	if (ri.Cmd_Argc() == 2 && !silent) {
		// explicit filename
		Com_sprintf(checkname, MAX_OSPATH, "screenshots/%s%s", ri.Cmd_Argv(1), exts[type]);
	} else {
		// scan for a free filename

		// if we have saved a previous screenshot, don't scan
		// again, because recording demo avis can involve
		// thousands of shots
		if (lastNumber == -1) {
			lastNumber = 0;
		}
		// scan for a free number
		for (; lastNumber <= 9999; lastNumber++) {
			R_ScreenshotFilename(lastNumber, checkname, sizeof(checkname), exts[type]);

			if (!ri.FS_FileExists(checkname)) {
				break; // file doesn't exist
			}
		}

		if (lastNumber >= 9999) {
			ri.Printf(PRINT_ALL, "ScreenShot: Couldn't create a file\n");
			return;
		}

		lastNumber++;
	}

	R_TakeScreenshot(0, 0, glConfig.vidWidth, glConfig.vidHeight, checkname, type);

	if (!silent) {
		ri.Printf(PRINT_ALL, "Wrote %s\n", checkname);
	}
}

static void R_ScreenShot_f(void) {
	int type = r_screenshotFormat->integer;
	if (type > 1) {
		R_ScreenShot(ST_PNG);
	} else if (type == 1) {
		R_ScreenShot(ST_JPEG);
	} else {
		R_ScreenShot(ST_TGA);
	}
}

//============================================================================

/*
==================
R_ExportCubemaps
==================
*/
static void R_ExportCubemaps(void) {
	exportCubemapsCommand_t *cmd;

	cmd = R_GetCommandBuffer(sizeof(*cmd));
	if (!cmd) {
		return;
	}
	cmd->commandId = RC_EXPORT_CUBEMAPS;
}

/*
==================
R_ExportCubemaps_f
==================
*/
static void R_ExportCubemaps_f(void) {
	R_ExportCubemaps();
}

//============================================================================

/*
==================
RB_TakeVideoFrameCmd
==================
*/
const void *RB_TakeVideoFrameCmd(const void *data) {
	const videoFrameCommand_t *cmd;
	byte *cBuf;
	size_t memcount, linelen;
	int padwidth, avipadwidth, padlen, avipadlen;
	GLint packAlign = 0;

	// finish any 2D drawing if needed
	if (tess.numIndexes)
		RB_EndSurface();

	cmd = (const videoFrameCommand_t *)data;

	qglGetIntegerv(GL_PACK_ALIGNMENT, &packAlign);

	linelen = cmd->width * 3;

	// Alignment stuff for glReadPixels
	padwidth = PAD(linelen, packAlign);
	padlen = padwidth - linelen;
	// AVI line padding
	avipadwidth = PAD(linelen, AVI_LINE_PADDING);
	avipadlen = avipadwidth - linelen;

	cBuf = PADP(cmd->captureBuffer, packAlign);

	qglReadPixels(0, 0, cmd->width, cmd->height, GL_RGB, GL_UNSIGNED_BYTE, cBuf);

	memcount = padwidth * cmd->height;

	// gamma correction
	if (glConfig.deviceSupportsGamma)
		R_GammaCorrect(cBuf, memcount);

	if (cmd->motionJpeg) {
		memcount = RE_SaveJPGToBuffer(cmd->encodeBuffer, linelen * cmd->height, r_aviMotionJpegQuality->integer,
									  cmd->width, cmd->height, cBuf, padlen);
		ri.CL_WriteAVIVideoFrame(cmd->encodeBuffer, memcount);
	} else {
		byte *lineend, *memend;
		byte *srcptr, *destptr;

		srcptr = cBuf;
		destptr = cmd->encodeBuffer;
		memend = srcptr + memcount;

		// swap R and B and remove line paddings
		while (srcptr < memend) {
			lineend = srcptr + linelen;
			while (srcptr < lineend) {
				*destptr++ = srcptr[2];
				*destptr++ = srcptr[1];
				*destptr++ = srcptr[0];
				srcptr += 3;
			}

			Com_Memset(destptr, '\0', avipadlen);
			destptr += avipadlen;

			srcptr += padlen;
		}

		ri.CL_WriteAVIVideoFrame(cmd->encodeBuffer, avipadwidth * cmd->height);
	}

	return (const void *)(cmd + 1);
}

//============================================================================

/*
** GL_SetDefaultState
*/
void GL_SetDefaultState(void) {
	qglClearDepth(1.0f);

	qglCullFace(GL_FRONT);

	GL_BindNullTextures();

	if (glRefConfig.framebufferObject)
		GL_BindNullFramebuffers();

	GL_TextureMode(r_textureMode->string);

	// qglShadeModel( GL_SMOOTH );
	qglDepthFunc(GL_LEQUAL);

	//
	// make sure our GL state vector is set correctly
	//
	glState.glStateBits = GLS_DEPTHTEST_DISABLE | GLS_DEPTHMASK_TRUE;
	glState.storedGlState = 0;
	glState.faceCulling = CT_TWO_SIDED;
	glState.faceCullFront = qtrue;

	GL_BindNullProgram();

	if (glRefConfig.vertexArrayObject)
		qglBindVertexArray(0);

	qglBindBuffer(GL_ARRAY_BUFFER, 0);
	qglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glState.currentVao = NULL;
	glState.vertexAttribsEnabled = 0;

	qglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	qglDepthMask(GL_TRUE);
	qglDisable(GL_DEPTH_TEST);
	qglEnable(GL_SCISSOR_TEST);
	qglDisable(GL_CULL_FACE);
	qglDisable(GL_BLEND);

	if (glRefConfig.seamlessCubeMap)
		qglEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// GL_POLYGON_OFFSET_FILL will be glEnable()d when this is used
	qglPolygonOffset(r_offsetFactor->value, r_offsetUnits->value);

	qglClearColor(0.0f, 0.0f, 0.0f, 1.0f); // FIXME: get color of sky
}

/*
================
R_PrintLongString

Workaround for ri.Printf's 1024 characters buffer limit.
================
*/
static void R_PrintLongString(const char *string) {
	char buffer[1024];
	const char *p;
	int size = strlen(string);

	p = string;
	while (size > 0) {
		Q_strncpyz(buffer, p, sizeof(buffer));
		ri.Printf(PRINT_ALL, "%s", buffer);
		p += 1023;
		size -= 1023;
	}
}

/*
================
GfxInfo_f
================
*/
static void GfxInfo_f(void) {
	const char *enablestrings[] = {"disabled", "enabled"};
	const char *fsstrings[] = {"windowed", "fullscreen"};

	ri.Printf(PRINT_ALL, "\nGL_VENDOR: %s\n", glConfig.vendor_string);
	ri.Printf(PRINT_ALL, "GL_RENDERER: %s\n", glConfig.renderer_string);
	ri.Printf(PRINT_ALL, "GL_VERSION: %s\n", glConfig.version_string);
	ri.Printf(PRINT_ALL, "GL_EXTENSIONS: ");
	// glConfig.extensions_string is a limited length so get the full list directly
	if (qglGetStringi) {
		GLint numExtensions = 0;
		int i;

		qglGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
		for (i = 0; i < numExtensions; i++) {
			ri.Printf(PRINT_ALL, "%s ", qglGetStringi(GL_EXTENSIONS, i));
		}
	} else {
		R_PrintLongString((const char *)qglGetString(GL_EXTENSIONS));
	}
	ri.Printf(PRINT_ALL, "\n");
	ri.Printf(PRINT_ALL, "GL_MAX_TEXTURE_SIZE: %d\n", glConfig.maxTextureSize);
	ri.Printf(PRINT_ALL, "GL_MAX_TEXTURE_IMAGE_UNITS: %d\n", glConfig.numTextureUnits);
	ri.Printf(PRINT_ALL, "\nPIXELFORMAT: color(%d-bits) Z(%d-bit) stencil(%d-bits)\n", glConfig.colorBits,
			  glConfig.depthBits, glConfig.stencilBits);
	ri.Printf(PRINT_ALL, "MODE: %d, %d x %d %s hz:", r_mode->integer, glConfig.vidWidth, glConfig.vidHeight,
			  fsstrings[r_fullscreen->integer == 1]);
	if (glConfig.displayFrequency) {
		ri.Printf(PRINT_ALL, "%d\n", glConfig.displayFrequency);
	} else {
		ri.Printf(PRINT_ALL, "N/A\n");
	}
	if (glConfig.deviceSupportsGamma) {
		ri.Printf(PRINT_ALL, "GAMMA: hardware w/ %d overbright bits\n", tr.overbrightBits);
	} else {
		ri.Printf(PRINT_ALL, "GAMMA: software w/ %d overbright bits\n", tr.overbrightBits);
	}

	ri.Printf(PRINT_ALL, "texturemode: %s\n", r_textureMode->string);
	ri.Printf(PRINT_ALL, "picmip: %d\n", r_picmip->integer);
	ri.Printf(PRINT_ALL, "texture bits: %d\n", r_texturebits->integer);
	ri.Printf(PRINT_ALL, "compiled vertex arrays: %s\n", enablestrings[qglLockArraysEXT != 0]);
	ri.Printf(PRINT_ALL, "texenv add: %s\n", enablestrings[glConfig.textureEnvAddAvailable != 0]);
	ri.Printf(PRINT_ALL, "compressed textures: %s\n", enablestrings[glConfig.textureCompression != TC_NONE]);
	if (r_vertexLight->integer || glConfig.hardwareType == GLHW_PERMEDIA2) {
		ri.Printf(PRINT_ALL, "HACK: using vertex lightmap approximation\n");
	}
	if (glConfig.hardwareType == GLHW_RAGEPRO) {
		ri.Printf(PRINT_ALL, "HACK: ragePro approximations\n");
	}
	if (glConfig.hardwareType == GLHW_RIVA128) {
		ri.Printf(PRINT_ALL, "HACK: riva128 approximations\n");
	}
	if (r_finish->integer) {
		ri.Printf(PRINT_ALL, "Forcing glFinish\n");
	}
}

/*
================
GfxMemInfo_f
================
*/
static void GfxMemInfo_f(void) {
	switch (glRefConfig.memInfo) {
	case MI_NONE: {
		ri.Printf(PRINT_ALL, "No extension found for GPU memory info.\n");
	} break;
	case MI_NVX: {
		int value;

		qglGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &value);
		ri.Printf(PRINT_ALL, "GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX: %ikb\n", value);

		qglGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &value);
		ri.Printf(PRINT_ALL, "GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX: %ikb\n", value);

		qglGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &value);
		ri.Printf(PRINT_ALL, "GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX: %ikb\n", value);

		qglGetIntegerv(GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX, &value);
		ri.Printf(PRINT_ALL, "GPU_MEMORY_INFO_EVICTION_COUNT_NVX: %i\n", value);

		qglGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &value);
		ri.Printf(PRINT_ALL, "GPU_MEMORY_INFO_EVICTED_MEMORY_NVX: %ikb\n", value);
	} break;
	case MI_ATI: {
		// GL_ATI_meminfo
		int value[4];

		qglGetIntegerv(GL_VBO_FREE_MEMORY_ATI, &value[0]);
		ri.Printf(PRINT_ALL, "VBO_FREE_MEMORY_ATI: %ikb total %ikb largest aux: %ikb total %ikb largest\n", value[0],
				  value[1], value[2], value[3]);

		qglGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, &value[0]);
		ri.Printf(PRINT_ALL, "TEXTURE_FREE_MEMORY_ATI: %ikb total %ikb largest aux: %ikb total %ikb largest\n",
				  value[0], value[1], value[2], value[3]);

		qglGetIntegerv(GL_RENDERBUFFER_FREE_MEMORY_ATI, &value[0]);
		ri.Printf(PRINT_ALL, "RENDERBUFFER_FREE_MEMORY_ATI: %ikb total %ikb largest aux: %ikb total %ikb largest\n",
				  value[0], value[1], value[2], value[3]);
	} break;
	}
}

/*
===============
R_Register
===============
*/
static void R_Register(void) {
#ifdef USE_RENDERER_DLOPEN
	com_altivec = ri.Cvar_Get("com_altivec", "1", CVAR_ARCHIVE);
#endif

	//
	// latched and archived variables
	//
	r_allowExtensions = ri.Cvar_Get("r_allowExtensions", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_allowExtensions, "Use all of the OpenGL extensions your card is capable of.");
	r_ext_compressed_textures = ri.Cvar_Get("r_ext_compressed_textures", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_ext_compressed_textures, "Enables texture compression.");
	r_ext_multitexture = ri.Cvar_Get("r_ext_multitexture", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_ext_multitexture, "Enables hardware multi-texturing (0: off, 1: on).");
	r_ext_compiled_vertex_array = ri.Cvar_Get("r_ext_compiled_vertex_array", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_ext_compiled_vertex_array, "Enables hardware-compiled vertex array rendering method.");
	r_ext_texture_env_add = ri.Cvar_Get("r_ext_texture_env_add", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_ext_texture_env_add,
						   "Enables additive blending in multitexturing. Requires \\r_ext_multitexture 1.");

	r_ext_framebuffer_object = ri.Cvar_Get("r_ext_framebuffer_object", "1", CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_texture_float = ri.Cvar_Get("r_ext_texture_float", "1", CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_framebuffer_multisample = ri.Cvar_Get("r_ext_framebuffer_multisample", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_ext_framebuffer_multisample, "For anti-aliasing geometry edges (FBO).");
	r_arb_seamless_cube_map = ri.Cvar_Get("r_arb_seamless_cube_map", "0", CVAR_ARCHIVE | CVAR_LATCH);
	r_arb_vertex_array_object = ri.Cvar_Get("r_arb_vertex_array_object", "1", CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_direct_state_access = ri.Cvar_Get("r_ext_direct_state_access", "1", CVAR_ARCHIVE | CVAR_LATCH);

	r_ext_texture_filter_anisotropic = ri.Cvar_Get("r_ext_texture_filter_anisotropic", "0", CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_max_anisotropy = ri.Cvar_Get("r_ext_max_anisotropy", "2", CVAR_ARCHIVE | CVAR_LATCH);

	r_picmip = ri.Cvar_Get("r_picmip", "1", CVAR_ARCHIVE | CVAR_LATCH);
	r_roundImagesDown = ri.Cvar_Get("r_roundImagesDown", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_roundImagesDown, "When images are scaled, round images down instead of up.");
	r_colorMipLevels = ri.Cvar_Get("r_colorMipLevels", "0", CVAR_LATCH);
	ri.Cvar_CheckRange(r_picmip, 0, 16, qtrue);
	ri.Cvar_SetDescription(
		r_colorMipLevels,
		"Debugging tool to artificially color different mipmap levels so that they are more apparent.");
	r_detailTextures = ri.Cvar_Get("r_detailtextures", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_detailTextures, "Enables usage of shader stages flagged as detail.");
	r_texturebits = ri.Cvar_Get("r_texturebits", "0", CVAR_ARCHIVE | CVAR_LATCH);
	r_colorbits = ri.Cvar_Get("r_colorbits", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_texturebits, "Number of texture bits per texture.");
	r_stencilbits = ri.Cvar_Get("r_stencilbits", "8", CVAR_ARCHIVE | CVAR_LATCH);
	r_depthbits = ri.Cvar_Get("r_depthbits", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_stencilbits, "Stencil buffer size, value decreases Z-buffer depth.");
	r_ext_multisample = ri.Cvar_Get("r_ext_multisample", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_CheckRange(r_ext_multisample, 0, 8, qtrue);
	r_overBrightBits = ri.Cvar_Get("r_overBrightBits", "0", CVAR_ARCHIVE | CVAR_LATCH);
	r_ignorehwgamma = ri.Cvar_Get("r_ignorehwgamma", "0", CVAR_ARCHIVE | CVAR_LATCH);
	r_mode = ri.Cvar_Get("r_mode", "-2", CVAR_ARCHIVE | CVAR_LATCH);
	r_fullscreen = ri.Cvar_Get("r_fullscreen", "1", CVAR_ARCHIVE);
	r_noborder = ri.Cvar_Get("r_noborder", "0", CVAR_ARCHIVE | CVAR_LATCH);
	r_customwidth = ri.Cvar_Get("r_customwidth", "1600", CVAR_ARCHIVE | CVAR_LATCH);
	r_customheight = ri.Cvar_Get("r_customheight", "1024", CVAR_ARCHIVE | CVAR_LATCH);
	r_customPixelAspect = ri.Cvar_Get("r_customPixelAspect", "1", CVAR_ARCHIVE | CVAR_LATCH);
	r_simpleMipMaps = ri.Cvar_Get("r_simpleMipMaps", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_simpleMipMaps, "Whether or not to use a simple mipmapping algorithm or a more correct "
											"one:\n 0: off (proper linear filter)\n 1: on (for slower machines)");
	r_vertexLight = ri.Cvar_Get("r_vertexLight", "0", CVAR_ARCHIVE | CVAR_LATCH);
	r_uiFullScreen = ri.Cvar_Get("r_uifullscreen", "0", 0);
	ri.Cvar_SetDescription(r_vertexLight, "Set to 1 to use vertex light instead of lightmaps, collapse all multi-stage "
										  "shaders into single-stage ones, might cause rendering artifacts.");
	r_subdivisions = ri.Cvar_Get("r_subdivisions", "4", CVAR_ARCHIVE | CVAR_LATCH);
	r_stereoEnabled = ri.Cvar_Get("r_stereoEnabled", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_subdivisions, "Distance to subdivide bezier curved surfaces. Higher values mean less "
										   "subdivision and less geometric complexity.");
	r_greyscale = ri.Cvar_Get("r_greyscale", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_CheckRange(r_greyscale, 0, 1, qfalse);
	ri.Cvar_SetDescription(r_greyscale, "Desaturates rendered frame.");

	r_externalGLSL = ri.Cvar_Get("r_externalGLSL", "0", CVAR_LATCH);

	r_hdr = ri.Cvar_Get("r_hdr", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_hdr, "Do scene rendering in a framebuffer with high dynamic range.");
	r_floatLightmap = ri.Cvar_Get("r_floatLightmap", "0", CVAR_ARCHIVE | CVAR_LATCH);
	r_postProcess = ri.Cvar_Get("r_postProcess", "1", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_postProcess, "Enable post-processing.");

	r_toneMap = ri.Cvar_Get("r_toneMap", "1", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_toneMap, "Enable tone mapping. Requires r_hdr and r_postProcess.");
	r_forceToneMap = ri.Cvar_Get("r_forceToneMap", "0", CVAR_CHEAT);
	r_forceToneMapMin = ri.Cvar_Get("r_forceToneMapMin", "-8.0", CVAR_CHEAT);
	r_forceToneMapAvg = ri.Cvar_Get("r_forceToneMapAvg", "-2.0", CVAR_CHEAT);
	r_forceToneMapMax = ri.Cvar_Get("r_forceToneMapMax", "0.0", CVAR_CHEAT);

	r_autoExposure = ri.Cvar_Get("r_autoExposure", "1", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_autoExposure,
						   "Do automatic exposure based on scene brightness. Hardcoded to -2 to 2 on maps that don't "
						   "specify otherwise. Requires r_hdr, r_postProcess, and r_toneMap.");
	r_forceAutoExposure = ri.Cvar_Get("r_forceAutoExposure", "0", CVAR_CHEAT);
	r_forceAutoExposureMin = ri.Cvar_Get("r_forceAutoExposureMin", "-2.0", CVAR_CHEAT);
	r_forceAutoExposureMax = ri.Cvar_Get("r_forceAutoExposureMax", "2.0", CVAR_CHEAT);

	r_cameraExposure = ri.Cvar_Get("r_cameraExposure", "1", CVAR_CHEAT);

	r_depthPrepass = ri.Cvar_Get("r_depthPrepass", "1", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_depthPrepass, "Do a depth-only pass before rendering. Speeds up rendering in cases where "
										   "advanced features are used. Required for r_sunShadows.");
	r_ssao = ri.Cvar_Get("r_ssao", "0", CVAR_LATCH | CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_ssao, "Enable screen-space ambient occlusion.");

	r_normalMapping = ri.Cvar_Get("r_normalMapping", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_normalMapping, "Enable normal maps for materials that support it.");
	r_specularMapping = ri.Cvar_Get("r_specularMapping", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_specularMapping, "Enable specular maps for materials that support it.");
	r_deluxeMapping = ri.Cvar_Get("r_deluxeMapping", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_deluxeMapping,
						   "Enable deluxe mapping (map is compiled with light directions). Even if the map doesn't "
						   "have deluxe mapping compiled in, an approximation based on the lightgrid will be used.");
	r_parallaxMapping = ri.Cvar_Get("r_parallaxMapping", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_parallaxMapping, "Enable parallax mapping for materials that support it.\n 0: No\n 1: Use "
											  "parallax occlusion mapping\n 2: Use relief mapping");
	r_parallaxMapOffset = ri.Cvar_Get("r_parallaxMapOffset", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_parallaxMapOffset, "Set the parallax height offset.");
	r_parallaxMapShadows = ri.Cvar_Get("r_parallaxMapShadows", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_parallaxMapShadows, "Enable self-shadowing on parallax map supported materials.");
	r_cubeMapping = ri.Cvar_Get("r_cubeMapping", "0", CVAR_ARCHIVE | CVAR_LATCH);
	r_cubemapSize = ri.Cvar_Get("r_cubemapSize", "128", CVAR_ARCHIVE | CVAR_LATCH);
	r_deluxeSpecular = ri.Cvar_Get("r_deluxeSpecular", "0.3", CVAR_ARCHIVE | CVAR_LATCH);
	r_pbr = ri.Cvar_Get("r_pbr", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_pbr, "Enable physically based rendering.");
	r_baseNormalX = ri.Cvar_Get("r_baseNormalX", "1.0", CVAR_ARCHIVE | CVAR_LATCH);
	r_baseNormalY = ri.Cvar_Get("r_baseNormalY", "1.0", CVAR_ARCHIVE | CVAR_LATCH);
	r_baseParallax = ri.Cvar_Get("r_baseParallax", "0.05", CVAR_ARCHIVE | CVAR_LATCH);
	r_baseSpecular = ri.Cvar_Get("r_baseSpecular", "0.04", CVAR_ARCHIVE | CVAR_LATCH);
	r_baseGloss = ri.Cvar_Get("r_baseGloss", "0.3", CVAR_ARCHIVE | CVAR_LATCH);
	r_glossType = ri.Cvar_Get("r_glossType", "1", CVAR_ARCHIVE | CVAR_LATCH);
	r_dlightMode = ri.Cvar_Get("r_dlightMode", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(
		r_dlightMode, "Dynamic light mode:\n 0: VQ3 'fake' dynamic lights\n 1: High-quality per-pixel dynamic lights, "
					  "slightly faster than VQ3's on modern hardware\n 2: Same as 1 but applies to all MD3 models too");
	r_pshadowDist = ri.Cvar_Get("r_pshadowDist", "128", CVAR_ARCHIVE);
	r_mergeLightmaps = ri.Cvar_Get("r_mergeLightmaps", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_mergeLightmaps, "Merge small lightmaps into 2 or fewer giant lightmaps.");
	r_imageUpsample = ri.Cvar_Get("r_imageUpsample", "0", CVAR_ARCHIVE | CVAR_LATCH);
	r_imageUpsampleMaxSize = ri.Cvar_Get("r_imageUpsampleMaxSize", "1024", CVAR_ARCHIVE | CVAR_LATCH);
	r_imageUpsampleType = ri.Cvar_Get("r_imageUpsampleType", "1", CVAR_ARCHIVE | CVAR_LATCH);
	r_genNormalMaps = ri.Cvar_Get("r_genNormalMaps", "0", CVAR_ARCHIVE | CVAR_LATCH);

	r_forceSun = ri.Cvar_Get("r_forceSun", "0", CVAR_CHEAT);
	r_forceSunLightScale = ri.Cvar_Get("r_forceSunLightScale", "1.0", CVAR_CHEAT);
	r_forceSunAmbientScale = ri.Cvar_Get("r_forceSunAmbientScale", "0.5", CVAR_CHEAT);
	r_drawSunRays = ri.Cvar_Get("r_drawSunRays", "1", CVAR_ARCHIVE | CVAR_LATCH);
	r_sunlightMode = ri.Cvar_Get("r_sunlightMode", "2", CVAR_ARCHIVE | CVAR_LATCH);

	r_sunShadows = ri.Cvar_Get("r_sunShadows", "1", CVAR_ARCHIVE | CVAR_LATCH);
	r_shadowFilter = ri.Cvar_Get("r_shadowFilter", "1", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_shadowFilter,
						   "Enable filtering shadows for a smoother look (0 - No. 1 - Some. 2 - Much).");
	r_shadowBlur = ri.Cvar_Get("r_shadowBlur", "0", CVAR_ARCHIVE | CVAR_LATCH);
	r_shadowMapSize = ri.Cvar_Get("r_shadowMapSize", "1024", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_shadowMapSize, "Size of each cascaded shadow map.");
	r_shadowCascadeZNear = ri.Cvar_Get("r_shadowCascadeZNear", "8", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_shadowCascadeZNear, "Near plane for shadow cascade frustums.");
	r_shadowCascadeZFar = ri.Cvar_Get("r_shadowCascadeZFar", "1024", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_shadowCascadeZFar, "Far plane for shadow cascade frustums.");
	r_shadowCascadeZBias = ri.Cvar_Get("r_shadowCascadeZBias", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(r_shadowCascadeZBias, "Z-bias for shadow cascade frustums.");
	r_ignoreDstAlpha = ri.Cvar_Get("r_ignoreDstAlpha", "1", CVAR_ARCHIVE | CVAR_LATCH);

	//
	// temporary latched variables that can only change over a restart
	//
	r_displayRefresh = ri.Cvar_Get("r_displayRefresh", "0", CVAR_LATCH);
	ri.Cvar_CheckRange(r_displayRefresh, 0, 200, qtrue);
	r_fullbright = ri.Cvar_Get("r_fullbright", "0", CVAR_LATCH | CVAR_CHEAT);
	ri.Cvar_SetDescription(r_fullbright, "Debugging tool to render the entire level without lighting.");
	r_mapOverBrightBits = ri.Cvar_Get("r_mapOverBrightBits", "2", CVAR_LATCH);
	ri.Cvar_SetDescription(r_mapOverBrightBits,
						   "Sets the number of overbright bits baked into all lightmaps and map data.");
	r_intensity = ri.Cvar_Get("r_intensity", "1", CVAR_LATCH);
	ri.Cvar_SetDescription(r_intensity, "Global texture lighting scale.");
	r_singleShader = ri.Cvar_Get("r_singleShader", "0", CVAR_CHEAT | CVAR_LATCH);
	ri.Cvar_SetDescription(r_singleShader, "Debugging tool that only uses the default shader for all rendering.");

	//
	// archived variables that can change at any time
	//
	r_lodCurveError = ri.Cvar_Get("r_lodCurveError", "250", CVAR_ARCHIVE | CVAR_CHEAT);
	ri.Cvar_CheckRange(r_lodCurveError, -1, 8192, qtrue);
	ri.Cvar_SetDescription(r_lodCurveError, "Level of detail error on curved surface grids.");
	r_lodbias = ri.Cvar_Get("r_lodbias", "0", CVAR_ARCHIVE);
	r_flares = ri.Cvar_Get("r_flares", "1", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_lodbias, "Sets the level of detail of in-game models:\n 0: High\n 1: Medium\n 2: Low");
	ri.Cvar_SetDescription(r_flares, "Enables corona effects on light sources.");
	r_znear = ri.Cvar_Get("r_znear", "4", CVAR_CHEAT);
	ri.Cvar_CheckRange(r_znear, 0.001f, 200, qfalse);
	ri.Cvar_SetDescription(r_znear, "Viewport distance from view origin (how close objects can be to the player before "
									"they're clipped out of the scene).");
	r_zproj = ri.Cvar_Get("r_zproj", "64", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_zproj, "Projected viewport frustum.");
	r_stereoSeparation = ri.Cvar_Get("r_stereoSeparation", "64", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(
		r_stereoSeparation,
		"Control eye separation. Resulting separation is \\r_zproj divided by this value in standard units.");
	r_ignoreGLErrors = ri.Cvar_Get("r_ignoreGLErrors", "1", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_ignoreGLErrors, "Ignore OpenGL errors.");
	r_fastsky = ri.Cvar_Get("r_fastsky", "0", CVAR_ARCHIVE);
	r_inGameVideo = ri.Cvar_Get("r_inGameVideo", "1", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_fastsky, "Draw flat colored skies.");
	r_drawSun = ri.Cvar_Get("r_drawSun", "0", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_drawSun, "Draw sun shader in skies.");
	r_dynamiclight = ri.Cvar_Get("r_dynamiclight", "1", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_dynamiclight, "Enables dynamic lighting.");
	r_dlightBacks = ri.Cvar_Get("r_dlightBacks", "1", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_dlightBacks, "Whether or not dynamic lights should light up back-face culled geometry.");
	r_finish = ri.Cvar_Get("r_finish", "0", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_finish, "Force a glFinish call after rendering a scene.");
	r_textureMode = ri.Cvar_Get("r_textureMode", "GL_LINEAR_MIPMAP_LINEAR", CVAR_ARCHIVE);
	r_swapInterval = ri.Cvar_Get("r_swapInterval", "0", CVAR_ARCHIVE | CVAR_LATCH);
	ri.Cvar_SetDescription(
		r_textureMode,
		"Texture interpolation mode:\n GL_NEAREST: Nearest neighbor interpolation and will therefore appear similar to "
		"Quake II except with the added colored lighting\n GL_LINEAR: Linear interpolation and will appear to blend in "
		"objects that are closer than the resolution that the textures are set as\n GL_NEAREST_MIPMAP_NEAREST: Nearest "
		"neighbor interpolation with mipmapping for bilinear hardware, mipmapping will blend objects that are farther "
		"away than the resolution that they are set as\n GL_LINEAR_MIPMAP_NEAREST: Linear interpolation with "
		"mipmapping for bilinear hardware\n GL_NEAREST_MIPMAP_LINEAR: Nearest neighbor interpolation with mipmapping "
		"for trilinear hardware\n GL_LINEAR_MIPMAP_LINEAR: Linear interpolation with mipmapping for trilinear "
		"hardware");
	r_gamma = ri.Cvar_Get("r_gamma", "1", CVAR_ARCHIVE);
	ri.Cvar_CheckRange(r_gamma, 0.5, 3, qfalse);
	ri.Cvar_SetDescription(r_gamma, "Gamma correction factor.");
	r_facePlaneCull = ri.Cvar_Get("r_facePlaneCull", "1", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_facePlaneCull, "Enables culling of planar surfaces with back side test.");
	r_railWidth = ri.Cvar_Get("r_railWidth", "16", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_railWidth, "Radius of railgun trails.");
	r_railCoreWidth = ri.Cvar_Get("r_railCoreWidth", "6", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_railCoreWidth,
						   "Size of railgun trail rings when enabled in game code (normally \\cg_oldRail 0).");
	r_railSegmentLength = ri.Cvar_Get("r_railSegmentLength", "32", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_railSegmentLength, "Length of segments in railgun trails.");

	r_ambientScale = ri.Cvar_Get("r_ambientScale", "0.6", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_ambientScale, "Light grid ambient light scaling on entity models.");
	r_directedScale = ri.Cvar_Get("r_directedScale", "1", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_directedScale, "Light grid direct light scaling on entity models.");

	r_anaglyphMode = ri.Cvar_Get("r_anaglyphMode", "0", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_anaglyphMode, "Enable rendering of anaglyph images. Valid options for 3D glasses types:\n "
										   "0: Disabled\n 1: Red-cyan\n 2: Red-blue\n 3: Red-green\n 4: Green-magenta");

	//
	// temporary variables that can change at any time
	//
	r_showImages = ri.Cvar_Get("r_showImages", "0", CVAR_TEMP);
	ri.Cvar_SetDescription(r_showImages, "Draw all images currently loaded into memory:\n 0: Disabled\n 1: Show images "
										 "set to uniform size\n 2: Show images with scaled relative to largest image");

	r_debugLight = ri.Cvar_Get("r_debuglight", "0", CVAR_TEMP);
	ri.Cvar_SetDescription(r_debugLight, "Debugging tool to print ambient and directed lighting information.");
	r_debugSort = ri.Cvar_Get("r_debugSort", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(
		r_debugSort, "Debugging tool to filter out shaders with depth sorting order values higher than the set value.");
	r_printShaders = ri.Cvar_Get("r_printShaders", "0", 0);
	ri.Cvar_SetDescription(r_printShaders, "Debugging tool to print on console of the number of shaders used.");
	r_saveFontData = ri.Cvar_Get("r_saveFontData", "0", 0);

	r_nocurves = ri.Cvar_Get("r_nocurves", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_nocurves, "Set to 1 to disable drawing world bezier curves. Set to 0 to enable.");
	r_drawworld = ri.Cvar_Get("r_drawworld", "1", CVAR_CHEAT);
	r_lightmap = ri.Cvar_Get("r_lightmap", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_drawworld, "Set to 0 to disable drawing the world. Set to 1 to enable.");
	ri.Cvar_SetDescription(r_lightmap, "Show only lightmaps on all world surfaces.");
	r_portalOnly = ri.Cvar_Get("r_portalOnly", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_portalOnly,
						   "Set to 1 to render only first mirror/portal view if it is present on the scene.");

	r_flareSize = ri.Cvar_Get("r_flareSize", "40", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_flareSize, "Radius of light flares. Requires \\r_flares 1.");
	r_flareFade = ri.Cvar_Get("r_flareFade", "7", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_flareFade, "Distance to fade out light flares. Requires \\r_flares 1.");
	r_flareCoeff = ri.Cvar_Get("r_flareCoeff", FLARE_STDCOEFF, CVAR_CHEAT);
	ri.Cvar_SetDescription(r_flareCoeff,
						   "Coefficient for the light flare intensity falloff function. Requires \\r_flares 1.");

	r_skipBackEnd = ri.Cvar_Get("r_skipBackEnd", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_skipBackEnd, "Skips loading rendering backend.");

	r_measureOverdraw = ri.Cvar_Get("r_measureOverdraw", "0", CVAR_CHEAT);
	r_lodscale = ri.Cvar_Get("r_lodscale", "5", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_lodscale, "Set scale for level of detail adjustment.");
	r_norefresh = ri.Cvar_Get("r_norefresh", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_norefresh, "Bypasses refreshing of the rendered scene.");
	r_drawentities = ri.Cvar_Get("r_drawentities", "1", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_drawentities, "Draw all world entities.");
	r_ignore = ri.Cvar_Get("r_ignore", "1", CVAR_CHEAT);
	r_nocull = ri.Cvar_Get("r_nocull", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_nocull, "Draw all culled objects.");
	r_novis = ri.Cvar_Get("r_novis", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_novis, "Disables usage of PVS.");
	r_showcluster = ri.Cvar_Get("r_showcluster", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_showcluster, "Shows current cluster index.");
	r_speeds = ri.Cvar_Get("r_speeds", "0", CVAR_CHEAT);
	r_verbose = ri.Cvar_Get("r_verbose", "0", CVAR_CHEAT);
	r_logFile = ri.Cvar_Get("r_logFile", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(
		r_speeds, "Prints out various debugging stats from PVS:\n 0: Disabled\n 1: Backend BSP\n 2: Frontend grid "
				  "culling\n 3: Current view cluster index\n 4: Dynamic lighting\n 5: zFar clipping\n 6: Flares");
	r_debugSurface = ri.Cvar_Get("r_debugSurface", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_debugSurface, "Backend visual debugging tool for bezier mesh surfaces.");
	r_nobind = ri.Cvar_Get("r_nobind", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_nobind, "Backend debugging tool: Disables texture binding.");
	r_showtris = ri.Cvar_Get("r_showtris", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_showtris, "Debugging tool: Wireframe rendering of polygon triangles in the world.");
	r_showsky = ri.Cvar_Get("r_showsky", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_showsky, "Forces sky in front of all surfaces.");
	r_shownormals = ri.Cvar_Get("r_shownormals", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_shownormals, "Debugging tool: Show wireframe surface normals.");
	r_clear = ri.Cvar_Get("r_clear", "0", CVAR_CHEAT);
	r_offsetFactor = ri.Cvar_Get("r_offsetfactor", "-1", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_clear, "Forces screen buffer clearing every frame, removing any hall of mirrors effect in "
									"void.\n Use \\r_clearColor to set color.");
	ri.Cvar_SetDescription(r_offsetFactor, "Offset factor for shaders with polygonOffset stages.");
	r_offsetUnits = ri.Cvar_Get("r_offsetunits", "-2", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_offsetUnits, "Offset units for shaders with polygonOffset stages.");
	r_drawBuffer = ri.Cvar_Get("r_drawBuffer", "GL_BACK", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_drawBuffer, "Sets which frame buffer to draw into.");
	r_lockpvs = ri.Cvar_Get("r_lockpvs", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(
		r_lockpvs, "Debugging tool: Locks to current potentially visible set. Useful for testing vis-culling in maps.");
	r_noportals = ri.Cvar_Get("r_noportals", "0", CVAR_CHEAT);
	ri.Cvar_SetDescription(r_noportals, "Disables in-game portals, valid values: 0: Portals enabled\n 1: Portals "
										"disabled\n 2: Portals and mirrors disabled");
	r_shadows = ri.Cvar_Get("cg_shadows", "1", 0);

	r_marksOnTriangleMeshes = ri.Cvar_Get("r_marksOnTriangleMeshes", "0", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_marksOnTriangleMeshes, "Enables impact marks on triangle mesh surfaces (ie: MD3 models.) "
													"Requires impact marks to be enabled in the game code.");

	r_aviMotionJpegQuality = ri.Cvar_Get("r_aviMotionJpegQuality", "100", CVAR_ARCHIVE);
	r_screenshotJpegQuality = ri.Cvar_Get("r_screenshotJpegQuality", "100", CVAR_ARCHIVE);
	ri.Cvar_SetDescription(r_aviMotionJpegQuality, "Controls quality of Jpeg video capture when \\cl_aviMotionJpeg 1.");
	ri.Cvar_SetDescription(r_screenshotJpegQuality, "Controls quality of Jpeg screenshots when using screenshotJpeg.");
	r_screenshotFormat = ri.Cvar_Get("r_screenshotFormat", "2", CVAR_ARCHIVE);

	r_maxpolys = ri.Cvar_Get("r_maxpolys", va("%d", MAX_POLYS), 0);
	ri.Cvar_SetDescription(r_maxpolys, "Maximum number of polygons to draw in a scene.");
	r_maxpolyverts = ri.Cvar_Get("r_maxpolyverts", va("%d", MAX_POLYVERTS), 0);
	ri.Cvar_SetDescription(r_maxpolyverts, "Maximum number of polygon vertices to draw in a scene.");

	// make sure all the commands added here are also
	// removed in R_Shutdown
	ri.Cmd_AddCommand("imagelist", R_ImageList_f);
	ri.Cmd_AddCommand("shaderlist", R_ShaderList_f);
	ri.Cmd_AddCommand("skinlist", R_SkinList_f);
	ri.Cmd_AddCommand("modellist", R_Modellist_f);
	ri.Cmd_AddCommand("modelist", R_ModeList_f);
	ri.Cmd_AddCommand("screenshot", R_ScreenShot_f);
	ri.Cmd_AddCommand("gfxinfo", GfxInfo_f);
	ri.Cmd_AddCommand("minimize", GLimp_Minimize);
	ri.Cmd_AddCommand("gfxmeminfo", GfxMemInfo_f);
	ri.Cmd_AddCommand("exportCubemaps", R_ExportCubemaps_f);
}

static void R_InitQueries(void) {
	if (!glRefConfig.occlusionQuery)
		return;

	if (r_drawSunRays->integer)
		qglGenQueries(ARRAY_LEN(tr.sunFlareQuery), tr.sunFlareQuery);
}

static void R_ShutDownQueries(void) {
	if (!glRefConfig.occlusionQuery)
		return;

	if (r_drawSunRays->integer)
		qglDeleteQueries(ARRAY_LEN(tr.sunFlareQuery), tr.sunFlareQuery);
}

/*
===============
R_Init
===============
*/
void R_Init(void) {
	int err;
	int i;
	byte *ptr;

	ri.Printf(PRINT_ALL, "----- R_Init -----\n");

	// clear all our internal state
	Com_Memset(&tr, 0, sizeof(tr));
	Com_Memset(&backEnd, 0, sizeof(backEnd));
	Com_Memset(&tess, 0, sizeof(tess));

	//	Swap_Init();

	if ((intptr_t)tess.xyz & 15) {
		ri.Printf(PRINT_WARNING, "tess.xyz not 16 byte aligned\n");
	}
	// Com_Memset( tess.constantColor255, 255, sizeof( tess.constantColor255 ) );

	//
	// init function tables
	//
	for (i = 0; i < FUNCTABLE_SIZE; i++) {
		tr.sinTable[i] = sin(DEG2RAD(i * 360.0f / ((float)(FUNCTABLE_SIZE - 1))));
		tr.squareTable[i] = (i < FUNCTABLE_SIZE / 2) ? 1.0f : -1.0f;
		tr.sawToothTable[i] = (float)i / FUNCTABLE_SIZE;
		tr.inverseSawToothTable[i] = 1.0f - tr.sawToothTable[i];

		if (i < FUNCTABLE_SIZE / 2) {
			if (i < FUNCTABLE_SIZE / 4) {
				tr.triangleTable[i] = (float)i / (FUNCTABLE_SIZE / 4);
			} else {
				tr.triangleTable[i] = 1.0f - tr.triangleTable[i - FUNCTABLE_SIZE / 4];
			}
		} else {
			tr.triangleTable[i] = -tr.triangleTable[i - FUNCTABLE_SIZE / 2];
		}
	}

	R_InitFogTable();

	R_NoiseInit();

	R_Register();

	max_polys = r_maxpolys->integer;
	if (max_polys < MAX_POLYS)
		max_polys = MAX_POLYS;

	max_polyverts = r_maxpolyverts->integer;
	if (max_polyverts < MAX_POLYVERTS)
		max_polyverts = MAX_POLYVERTS;

	ptr =
		ri.Hunk_Alloc(sizeof(*backEndData) + sizeof(srfPoly_t) * max_polys + sizeof(polyVert_t) * max_polyverts, h_low);
	backEndData = (backEndData_t *)ptr;
	backEndData->polys = (srfPoly_t *)((char *)ptr + sizeof(*backEndData));
	backEndData->polyVerts = (polyVert_t *)((char *)ptr + sizeof(*backEndData) + sizeof(srfPoly_t) * max_polys);
	R_InitNextFrame();

	InitOpenGL();

	R_InitImages();

	if (glRefConfig.framebufferObject)
		FBO_Init();

	GLSL_InitGPUShaders();

	R_InitVaos();

	R_InitShaders();

	R_InitSkins();

	R_ModelInit();

	R_InitFreeType();

	R_InitQueries();

	err = qglGetError();
	if (err != GL_NO_ERROR)
		ri.Printf(PRINT_ALL, "glGetError() = 0x%x\n", err);

	// print info
	GfxInfo_f();
	ri.Printf(PRINT_ALL, "----- finished R_Init -----\n");
}

/*
===============
RE_Shutdown
===============
*/
static void RE_Shutdown(qboolean destroyWindow) {

	ri.Printf(PRINT_ALL, "RE_Shutdown( %i )\n", destroyWindow);

	ri.Cmd_RemoveCommand("imagelist");
	ri.Cmd_RemoveCommand("shaderlist");
	ri.Cmd_RemoveCommand("skinlist");
	ri.Cmd_RemoveCommand("modellist");
	ri.Cmd_RemoveCommand("modelist");
	ri.Cmd_RemoveCommand("screenshot");
	ri.Cmd_RemoveCommand("gfxinfo");
	ri.Cmd_RemoveCommand("minimize");
	ri.Cmd_RemoveCommand("gfxmeminfo");
	ri.Cmd_RemoveCommand("exportCubemaps");

	if (tr.registered) {
		R_IssuePendingRenderCommands();
		R_ShutDownQueries();
		if (glRefConfig.framebufferObject)
			FBO_Shutdown();
		R_DeleteTextures();
		R_ShutdownVaos();
		GLSL_ShutdownGPUShaders();
	}

	R_DoneFreeType();

	// shut down platform specific OpenGL stuff
	if (destroyWindow) {
		GLimp_Shutdown();

		Com_Memset(&glConfig, 0, sizeof(glConfig));
		Com_Memset(&glRefConfig, 0, sizeof(glRefConfig));
		Com_Memset(&glState, 0, sizeof(glState));
	}

	tr.registered = qfalse;
}

/*
=============
RE_EndRegistration

Touch all images to make sure they are resident
=============
*/
static void RE_EndRegistration(void) {
	R_IssuePendingRenderCommands();
	if (!ri.Sys_LowPhysicalMemory()) {
		RB_ShowImages();
	}
}

/*
@@@@@@@@@@@@@@@@@@@@@
GetRefAPI

@@@@@@@@@@@@@@@@@@@@@
*/
#ifdef USE_RENDERER_DLOPEN
Q_EXPORT refexport_t *QDECL GetRefAPI(int apiVersion, refimport_t *rimp) {
#else
refexport_t *GetRefAPI(int apiVersion, refimport_t *rimp) {
#endif

	static refexport_t re;

	ri = *rimp;

	Com_Memset(&re, 0, sizeof(re));

	if (apiVersion != REF_API_VERSION) {
		ri.Printf(PRINT_ALL, "Mismatched REF_API_VERSION: expected %i, got %i\n", REF_API_VERSION, apiVersion);
		return NULL;
	}

	// the RE_ functions are Renderer Entry points

	re.Shutdown = RE_Shutdown;

	re.BeginRegistration = RE_BeginRegistration;
	re.RegisterModel = RE_RegisterModel;
	re.RegisterSkin = RE_RegisterSkin;
	re.RegisterShader = RE_RegisterShader;
	re.RegisterShaderNoMip = RE_RegisterShaderNoMip;
	re.LoadWorld = RE_LoadWorldMap;
	re.SetWorldVisData = RE_SetWorldVisData;
	re.EndRegistration = RE_EndRegistration;

	re.BeginFrame = RE_BeginFrame;
	re.EndFrame = RE_EndFrame;

	re.MarkFragments = R_MarkFragments;
	re.LerpTag = R_LerpTag;
	re.ModelBounds = R_ModelBounds;

	re.ClearScene = RE_ClearScene;
	re.AddRefEntityToScene = RE_AddRefEntityToScene;
	re.AddPolyToScene = RE_AddPolyToScene;
	re.LightForPoint = R_LightForPoint;
	re.AddLightToScene = RE_AddLightToScene;
	re.AddAdditiveLightToScene = RE_AddAdditiveLightToScene;
	re.RenderScene = RE_RenderScene;

	re.SetColor = RE_SetColor;
	re.DrawStretchPic = RE_StretchPic;
	re.DrawStretchRaw = RE_StretchRaw;
	re.UploadCinematic = RE_UploadCinematic;

	re.RegisterFont = RE_RegisterFont;
	re.RemapShader = R_RemapShader;
	re.GetEntityToken = R_GetEntityToken;
	re.inPVS = R_inPVS;

	re.TakeVideoFrame = RE_TakeVideoFrame;

	return &re;
}
