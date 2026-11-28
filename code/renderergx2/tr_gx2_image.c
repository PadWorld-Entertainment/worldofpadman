/*
 * ioQuake3-U — Wii U native (wut / GX2) port
 * code/renderergx2/tr_gx2_image.c -- name -> GX2Texture registry, no .shader parser.
 * Upload is linear RGBA8, row-copied via surface.pitch -- mip placement was a long
 * fight and lost; see GX2Image_Upload for the surrender terms.
 */

#include <string.h>

#include <coreinit/memdefaultheap.h>
#include <gx2/texture.h>
#include <gx2/surface.h>
#include <gx2/mem.h>
#include <gx2/enum.h>
#include <gx2/event.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "../renderercommon/tr_common.h"
#include "tr_gx2_local.h"

/* 1 = tiled GX2 surface via staging+GX2CopySurface blit; 0 = proven linear-only
 * path. REVERT to 0 on any hardware regression, don't iteratively debug tiled. */
#define GX2_TRY_TILED_STATIC_TEXTURES 0

#define MAX_GX2_IMAGES 4096
#define GX2_MAX_MIP_LEVELS 13 /* matches GX2Surface::mipLevelOffset[13] */

typedef struct {
	char                     name[MAX_QPATH];
	GX2Texture              *texture; /* NULL if unresolvable OR a script (see below) */
	const gx2ShaderScript_t *script;  /* non-NULL if `name` is a multi-stage .shader */
} gx2image_t;

static gx2image_t s_images[MAX_GX2_IMAGES];
static int        s_numImages;   /* handle = index + 1 */

typedef void (*imageLoader_t)(const char *, byte **, int *, int *);
static const struct {
	const char   *ext;
	imageLoader_t load;
} s_loaders[] = {
	{ "png", R_LoadPNG },
	{ "tga", R_LoadTGA },
	{ "jpg", R_LoadJPG },
	{ "jpeg", R_LoadJPG },
	{ "bmp", R_LoadBMP },
	{ "pcx", R_LoadPCX },
};
#define NUM_LOADERS ((int)ARRAY_LEN(s_loaders))

/* 1 + floor(log2(max(width,height))), capped to GX2_MAX_MIP_LEVELS. */
static int GX2Image_MipCount(int width, int height)
{
	int levels = 1;

	while ((width > 1 || height > 1) && levels < GX2_MAX_MIP_LEVELS) {
		width = (width > 1) ? (width >> 1) : 1;
		height = (height > 1) ? (height >> 1) : 1;
		levels++;
	}
	return levels;
}

/* 2x2 box filter, round to nearest; clamps second sample for odd (NPOT) dimensions. */
static void GX2Image_DownsampleHalf(const byte *in, int inW, int inH,
                                     byte *out, int outW, int outH)
{
	int x, y, c;

	for (y = 0; y < outH; y++) {
		int y0 = y * 2;
		int y1 = (y0 + 1 < inH) ? y0 + 1 : y0;
		for (x = 0; x < outW; x++) {
			int x0 = x * 2;
			int x1 = (x0 + 1 < inW) ? x0 + 1 : x0;
			const byte *p00 = in + ((size_t)y0 * inW + x0) * 4;
			const byte *p01 = in + ((size_t)y0 * inW + x1) * 4;
			const byte *p10 = in + ((size_t)y1 * inW + x0) * 4;
			const byte *p11 = in + ((size_t)y1 * inW + x1) * 4;
			byte *o = out + ((size_t)y * outW + x) * 4;

			for (c = 0; c < 4; c++)
				o[c] = (byte)((p00[c] + p01[c] + p10[c] + p11[c] + 2) >> 2);
		}
	}
}

/* RGBA8 pixels -> linear-tiled GX2Texture with a box-filtered mip chain. */
GX2Texture *GX2Image_Upload(const byte *pic, int width, int height, qboolean noMip, qboolean allowTiled)
{
	GX2Texture *texture;
	int numMips, level, y;
	byte *mipPics[GX2_MAX_MIP_LEVELS];
	int mipW[GX2_MAX_MIP_LEVELS], mipH[GX2_MAX_MIP_LEVELS];
	qboolean actuallyTiled = qfalse;
	(void)actuallyTiled; (void)allowTiled; /* only read when GX2_TRY_TILED_STATIC_TEXTURES */

	texture = MEMAllocFromDefaultHeap(sizeof(GX2Texture));
	if (!texture)
		return NULL;
	memset(texture, 0, sizeof(GX2Texture));

	/* Mipmapping DISABLED for everyone -- every stride approach tried corrupted texels on
	 * real hw. Gave up and forced single-level: aliasing beats sometimes-corrupt. */
	numMips = 1;
	(void)GX2Image_MipCount;

	mipPics[0] = NULL; /* level 0 is the caller's own buffer */
	mipW[0] = width;
	mipH[0] = height;
	for (level = 1; level < numMips; level++) {
		mipW[level] = (mipW[level - 1] > 1) ? (mipW[level - 1] >> 1) : 1;
		mipH[level] = (mipH[level - 1] > 1) ? (mipH[level - 1] >> 1) : 1;
		mipPics[level] = ri.Malloc(mipW[level] * mipH[level] * 4);
		GX2Image_DownsampleHalf(level == 1 ? pic : mipPics[level - 1],
		                         mipW[level - 1], mipH[level - 1],
		                         mipPics[level], mipW[level], mipH[level]);
	}

	texture->surface.width = width;
	texture->surface.height = height;
	texture->surface.depth = 1;
	texture->surface.mipLevels = numMips;
	texture->surface.format = GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8;
	texture->surface.aa = GX2_AA_MODE1X;
	texture->surface.use = GX2_SURFACE_USE_TEXTURE;
	texture->surface.dim = GX2_SURFACE_DIM_TEXTURE_2D;
#if GX2_TRY_TILED_STATIC_TEXTURES
	texture->surface.tileMode = allowTiled ? GX2_TILE_MODE_DEFAULT
	                                        : GX2_TILE_MODE_LINEAR_ALIGNED;
#else
	texture->surface.tileMode = GX2_TILE_MODE_LINEAR_ALIGNED;
#endif
	texture->surface.swizzle = 0;
	texture->viewFirstMip = 0;
	texture->viewNumMips = numMips;
	texture->viewFirstSlice = 0;
	texture->viewNumSlices = 1;
	texture->compMap = 0x00010203;
	GX2CalcSurfaceSizeAndAlignment(&texture->surface);

#if GX2_TRY_TILED_STATIC_TEXTURES
	/* If DEFAULT didn't resolve to tiled, this texture silently falls back to linear. */
	actuallyTiled = allowTiled &&
	                texture->surface.tileMode != GX2_TILE_MODE_LINEAR_ALIGNED &&
	                texture->surface.tileMode != GX2_TILE_MODE_LINEAR_SPECIAL &&
	                texture->surface.tileMode != GX2_TILE_MODE_DEFAULT;
	if (allowTiled)
		ri.Printf(PRINT_ALL, "^2GX2Image: %dx%d tileMode resolved to %d (tiled=%d)\n",
		          width, height, (int)texture->surface.tileMode, (int)actuallyTiled);
#endif

	if (texture->surface.imageSize == 0) {
		for (level = 1; level < numMips; level++)
			ri.Free(mipPics[level]);
		MEMFreeToDefaultHeap(texture);
		return NULL;
	}

	/* One combined allocation at GX2's own mipLevelOffset[] values -- a rebased
	 * two-allocation split tried earlier still corrupted texels on hw, this doesn't. */
	{
		uint32_t mipBase = (numMips > 1) ? texture->surface.mipLevelOffset[1] : 0;
		uint32_t mipEnd = (numMips > 1) ? mipBase + texture->surface.mipmapSize : 0;
		uint32_t bufSize = texture->surface.imageSize;
		byte *buf;

		if (mipEnd > bufSize)
			bufSize = mipEnd;

		buf = MEMAllocFromDefaultHeapEx(bufSize, texture->surface.alignment);
		if (!buf && numMips > 1) {
			/* Degrade to base-level-only rather than fail the whole texture. */
			ri.Printf(PRINT_ALL, "^1GX2Image: mip allocation FAILED (%dx%d), using base level only\n",
			          width, height);
			numMips = 1;
			texture->surface.mipLevels = 1;
			texture->viewNumMips = 1;
			buf = MEMAllocFromDefaultHeapEx(texture->surface.imageSize, texture->surface.alignment);
		}
		if (!buf) {
			for (level = 1; level < numMips; level++)
				ri.Free(mipPics[level]);
			MEMFreeToDefaultHeap(texture);
			return NULL;
		}
		texture->surface.image = buf;
		texture->surface.mipmaps = (numMips > 1) ? buf : NULL;
	}

#if GX2_TRY_TILED_STATIC_TEXTURES
	if (actuallyTiled) {
		GX2Surface stagingSurface;
		byte *stagingBuf;

		memset(&stagingSurface, 0, sizeof(stagingSurface));
		stagingSurface.dim       = GX2_SURFACE_DIM_TEXTURE_2D;
		stagingSurface.width     = width;
		stagingSurface.height    = height;
		stagingSurface.depth     = 1;
		stagingSurface.mipLevels = 1;
		stagingSurface.format    = GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8;
		stagingSurface.aa        = GX2_AA_MODE1X;
		stagingSurface.use       = GX2_SURFACE_USE_TEXTURE;
		stagingSurface.tileMode  = GX2_TILE_MODE_LINEAR_ALIGNED;
		stagingSurface.swizzle   = 0;
		GX2CalcSurfaceSizeAndAlignment(&stagingSurface);

		stagingBuf = MEMAllocFromDefaultHeapEx(stagingSurface.imageSize, stagingSurface.alignment);
		if (!stagingBuf) {
			/* Fail closed; don't reuse the already-allocated tiled buffer. */
			for (level = 1; level < numMips; level++)
				ri.Free(mipPics[level]);
			MEMFreeToDefaultHeap(texture->surface.image);
			MEMFreeToDefaultHeap(texture);
			return NULL;
		}
		stagingSurface.image = stagingBuf;

		for (y = 0; y < height; y++) {
			memcpy((byte *)stagingSurface.image + (size_t)y * stagingSurface.pitch * 4,
			       pic + (size_t)y * width * 4,
			       (size_t)width * 4);
		}
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU_TEXTURE,
		              stagingSurface.image, stagingSurface.imageSize);

		GX2CopySurface(&stagingSurface, 0, 0, &texture->surface, 0, 0);
		/* Async GPU command — staging buffer must survive until GPU consumes it.
		 * Not yet confirmed safe pre-first-frame; a boot hang here is the tell. */
		GX2DrawDone();

		MEMFreeToDefaultHeap(stagingBuf);
	} else
#endif
	{
		/* surface.pitch (pixels) usually exceeds width; copy row by row. */
		for (y = 0; y < height; y++) {
			memcpy((byte *)texture->surface.image + (size_t)y * texture->surface.pitch * 4,
			       pic + (size_t)y * width * 4,
			       (size_t)width * 4);
		}
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU_TEXTURE,
		              texture->surface.image, texture->surface.imageSize);
	}

	if (numMips > 1) {
		/* Tiny trailing mips can report a degenerate offset -- stop at the last sane one. */
		uint32_t mipBase = texture->surface.mipLevelOffset[1];
		int uploadMips = (mipBase > 0) ? 2 : 1;

		if (mipBase > 0) {
			uint32_t lastOffset = mipBase;
			for (level = 2; level < numMips; level++) {
				uint32_t offset = texture->surface.mipLevelOffset[level];
				if (offset <= lastOffset)
					break;
				lastOffset = offset;
				uploadMips = level + 1;
			}
		}

		for (level = 1; level < uploadMips; level++) {
			uint32_t rawOffset = texture->surface.mipLevelOffset[level];
			uint32_t rowBytes = (uint32_t)mipW[level] * 4;
			uint32_t bufEnd = mipBase + texture->surface.mipmapSize;
			int my;

			/* Real stride from next level's offset, only trustworthy inside the validated chain. */
			if (mipH[level] > 1 && level + 1 < uploadMips) {
				uint32_t nextOffset = texture->surface.mipLevelOffset[level + 1];
				uint32_t derived = (nextOffset - rawOffset) / (uint32_t)mipH[level];
				if (derived >= rowBytes)
					rowBytes = derived;
			}

			/* Bounds guard: overflow here corrupts unrelated heap textures — stop rather than write past the buffer. */
			if (rawOffset + rowBytes * (uint32_t)mipH[level] > bufEnd) {
				uploadMips = level;
				break;
			}

			for (my = 0; my < mipH[level]; my++) {
				memcpy((byte *)texture->surface.mipmaps + rawOffset
				         + (size_t)my * rowBytes,
				       mipPics[level] + (size_t)my * mipW[level] * 4,
				       (size_t)mipW[level] * 4);
			}
		}

		texture->surface.mipLevels = (uint32_t)uploadMips;
		texture->viewNumMips = (uint32_t)uploadMips;
		GX2Invalidate(GX2_INVALIDATE_MODE_CPU_TEXTURE,
		              (byte *)texture->surface.mipmaps + mipBase,
		              texture->surface.mipmapSize);
	}

	for (level = 1; level < numMips; level++)
		ri.Free(mipPics[level]);

	/* Bake GPU texture regs only after allocation, or the GPU faults on first sample. */
	GX2InitTextureRegs(texture);

	return texture;
}

/* Frees a texture never handed to the name registry (world lightmap pages). Registry textures live for process lifetime. */
void GX2Image_FreeTexture(GX2Texture *tex)
{
	if (!tex)
		return;
	/* image+mipmaps is one combined allocation; free once (avoid double-free). */
	if (tex->surface.image)
		MEMFreeToDefaultHeap(tex->surface.image);
	else if (tex->surface.mipmaps)
		MEMFreeToDefaultHeap(tex->surface.mipmaps);
	MEMFreeToDefaultHeap(tex);
}

qboolean GX2Image_Init(void)
{
	static byte white[8 * 8 * 4];
	GX2Texture *tex;

	if (s_numImages > 0)
		return qtrue;

	memset(white, 0xFF, sizeof(white));
	tex = GX2Image_Upload(white, 8, 8, qtrue, qtrue);
	if (!tex) {
		ri.Printf(PRINT_ALL, "^1GX2Image: FAILED to create white texture\n");
		return qfalse;
	}

	Q_strncpyz(s_images[0].name, "*white", sizeof(s_images[0].name));
	s_images[0].texture = tex;
	s_numImages = 1;   /* handle GX2IMAGE_WHITE == 1 */
	return qtrue;
}

/* Registers `name` as a raw image, never consulting the shader-script table -- doing
 * so recurses into same-named wrapper shaders and silently degrades to a white quad. */
static qhandle_t GX2Image_RegisterPlain(const char *name, qboolean noMip)
{
	char stripped[MAX_QPATH];
	char filename[MAX_QPATH];
	byte *pic = NULL;
	int width = 0, height = 0;
	int i;
	gx2image_t *img;

	if (!name || !name[0] || s_numImages == 0)
		return GX2IMAGE_WHITE;

	for (i = 0; i < s_numImages; i++) {
		if (!Q_stricmp(s_images[i].name, name))
			return (qhandle_t)(i + 1);
	}

	if (s_numImages >= MAX_GX2_IMAGES) {
		ri.Printf(PRINT_ALL, "^1GX2Image: MAX_GX2_IMAGES hit registering '%s'\n", name);
		return GX2IMAGE_WHITE;
	}

	img = &s_images[s_numImages];
	Q_strncpyz(img->name, name, sizeof(img->name));
	img->texture = NULL;
	img->script = NULL;

	const char *origExt = COM_GetExtension(name);
	int triedIdx = -1;
	if (origExt && origExt[0]) {
		for (i = 0; i < NUM_LOADERS; i++) {
			if (!Q_stricmp(origExt, s_loaders[i].ext)) {
				s_loaders[i].load(name, &pic, &width, &height);
				triedIdx = i;
				break;
			}
		}
	}
	if (!pic) {
		COM_StripExtension(name, stripped, sizeof(stripped));
		for (i = 0; i < NUM_LOADERS; i++) {
			if (i == triedIdx)
				continue;
			Com_sprintf(filename, sizeof(filename), "%s.%s", stripped, s_loaders[i].ext);
			s_loaders[i].load(filename, &pic, &width, &height);
			if (pic)
				break;
		}
	}

	if (pic) {
		img->texture = GX2Image_Upload(pic, width, height, noMip, qtrue);
		ri.Free(pic);
		if (!img->texture)
			ri.Printf(PRINT_ALL, "^1GX2Image: upload FAILED for '%s' (%dx%d)\n",
			          name, width, height);
	} else {
		/* Logged once per name; registry entry remembers the miss. */
		ri.Printf(PRINT_ALL, "GX2Image: no image for '%s' (script-only shader?), using white\n",
		          name);
	}

	s_numImages++;
	return (qhandle_t)s_numImages;
}

/* Name -> handle. Never returns 0 (cgame errors on that); unresolved names get white. */
static qhandle_t GX2Image_RegisterEx(const char *name, qboolean noMip)
{
	int i;
	gx2image_t *img;
	const gx2ShaderScript_t *script;

	if (!name || !name[0] || s_numImages == 0)
		return GX2IMAGE_WHITE;

	for (i = 0; i < s_numImages; i++) {
		if (!Q_stricmp(s_images[i].name, name))
			return (qhandle_t)(i + 1);
	}

	/* Multi-stage .shader takes priority over a same-named loose texture;
	 * only this outer name is checked, never a stage's own path. */
	script = GX2ShaderScript_Find(name);
	if (!script)
		return GX2Image_RegisterPlain(name, noMip);

	if (s_numImages >= MAX_GX2_IMAGES) {
		ri.Printf(PRINT_ALL, "^1GX2Image: MAX_GX2_IMAGES hit registering '%s'\n", name);
		return GX2IMAGE_WHITE;
	}

	img = &s_images[s_numImages];
	Q_strncpyz(img->name, name, sizeof(img->name));
	img->texture = NULL;
	img->script = NULL;

	/* Resolve each stage's image on first use, never at parse time -- the dedup above
	 * guarantees this runs once per name, so mutating the shared static table is safe. */
	{
		gx2ShaderScript_t *mutScript = (gx2ShaderScript_t *)script;
		qhandle_t handle = (qhandle_t)(s_numImages + 1);
		int s;

		s_numImages++;
		for (s = 0; s < mutScript->numStages; s++) {
			gx2ShaderStage_t *stage = &mutScript->stages[s];
			if (stage->imageHandle != 0)
				continue;
			if (stage->isWhiteRef || !stage->imagePath[0])
				stage->imageHandle = GX2IMAGE_WHITE;
			else
				stage->imageHandle = GX2Image_RegisterPlain(stage->imagePath, noMip);
		}
		img->script = mutScript;
		return handle;
	}
}

qhandle_t GX2Image_Register(const char *name)
{
	return GX2Image_RegisterEx(name, qfalse);
}

/* RegisterShaderNoMip path: UI/HUD/font images. First registration wins the mip decision. */
qhandle_t GX2Image_RegisterNoMip(const char *name)
{
	return GX2Image_RegisterEx(name, qtrue);
}

const gx2ShaderScript_t *GX2Image_GetScript(qhandle_t handle)
{
	if (handle >= 1 && handle <= s_numImages)
		return s_images[handle - 1].script;
	return NULL;
}

GX2Texture *GX2Image_GetTexture(qhandle_t handle)
{
	if (handle >= 1 && handle <= s_numImages && s_images[handle - 1].texture)
		return s_images[handle - 1].texture;
	return s_images[0].texture;   /* white */
}

/* Picks the stage that best represents a multi-stage material's diffuse look
 * for the single-pass ubershader, skipping env-map/white/lightmap stages. */
const gx2ShaderStage_t *GX2Image_GetRepresentativeStage(const gx2ShaderScript_t *script)
{
	int s;

	if (!script || script->numStages <= 0)
		return NULL;

	for (s = 0; s < script->numStages; s++) {
		const gx2ShaderStage_t *stage = &script->stages[s];
		if (!stage->isEnvironmentMap && !stage->isWhiteRef && stage->imagePath[0])
			return stage;
	}
	for (s = 0; s < script->numStages; s++) {
		if (!script->stages[s].isEnvironmentMap)
			return &script->stages[s];
	}
	return &script->stages[0];
}
