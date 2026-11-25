/*
 * ioQuake3-U — Wii U native (wut / GX2) port
 * code/renderergx2/tr_gx2_font.c -- RE_RegisterFont. No FreeType here, so it just
 * reads baseq3's pre-baked fontImage_*.dat and byte-swaps every field by hand.
 */

#include <string.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "tr_gx2_local.h"

#define MAX_GX2_FONTS 6
static int        s_numFonts;
static fontInfo_t s_fonts[MAX_GX2_FONTS];

static const byte *s_fdFile;
static int         s_fdOffset;

static int GX2Font_ReadInt(void)
{
	int i = (int)((uint32_t)s_fdFile[s_fdOffset] |
	              ((uint32_t)s_fdFile[s_fdOffset + 1] << 8) |
	              ((uint32_t)s_fdFile[s_fdOffset + 2] << 16) |
	              ((uint32_t)s_fdFile[s_fdOffset + 3] << 24));
	s_fdOffset += 4;
	return i;
}

/* .dat is little-endian, this machine is big-endian -- always byte-reverse. */
static float GX2Font_ReadFloat(void)
{
	union { byte b[4]; float f; } v;
	v.b[0] = s_fdFile[s_fdOffset + 3];
	v.b[1] = s_fdFile[s_fdOffset + 2];
	v.b[2] = s_fdFile[s_fdOffset + 1];
	v.b[3] = s_fdFile[s_fdOffset + 0];
	s_fdOffset += 4;
	return v.f;
}

void GX2_RegisterFont(const char *fontName, int pointSize, fontInfo_t *font)
{
	char  name[MAX_QPATH];
	void *faceData;
	int   i, len;

	memset(font, 0, sizeof(*font));

	if (!fontName || !fontName[0]) {
		ri.Printf(PRINT_ALL, "GX2_RegisterFont: called with empty name\n");
		return;
	}
	if (pointSize <= 0)
		pointSize = 12;

	Com_sprintf(name, sizeof(name), "fonts/fontImage_%i.dat", pointSize);

	for (i = 0; i < s_numFonts; i++) {
		if (!Q_stricmp(name, s_fonts[i].name)) {
			Com_Memcpy(font, &s_fonts[i], sizeof(fontInfo_t));
			return;
		}
	}

	if (s_numFonts >= MAX_GX2_FONTS) {
		ri.Printf(PRINT_ALL, "^1GX2_RegisterFont: MAX_GX2_FONTS hit\n");
		return;
	}

	len = ri.FS_ReadFile(name, NULL);
	if (len != (int)sizeof(fontInfo_t)) {
		ri.Printf(PRINT_ALL, "^1GX2_RegisterFont: '%s' missing or wrong size "
		          "(no FreeType on this build -- baseq3 must ship it)\n", name);
		return;
	}

	ri.FS_ReadFile(name, &faceData);
	s_fdFile = (const byte *)faceData;
	s_fdOffset = 0;

	for (i = 0; i < GLYPHS_PER_FONT; i++) {
		font->glyphs[i].height      = GX2Font_ReadInt();
		font->glyphs[i].top         = GX2Font_ReadInt();
		font->glyphs[i].bottom      = GX2Font_ReadInt();
		font->glyphs[i].pitch       = GX2Font_ReadInt();
		font->glyphs[i].xSkip       = GX2Font_ReadInt();
		font->glyphs[i].imageWidth  = GX2Font_ReadInt();
		font->glyphs[i].imageHeight = GX2Font_ReadInt();
		font->glyphs[i].s           = GX2Font_ReadFloat();
		font->glyphs[i].t           = GX2Font_ReadFloat();
		font->glyphs[i].s2          = GX2Font_ReadFloat();
		font->glyphs[i].t2          = GX2Font_ReadFloat();
		font->glyphs[i].glyph       = GX2Font_ReadInt();
		Q_strncpyz(font->glyphs[i].shaderName,
		           (const char *)&s_fdFile[s_fdOffset],
		           sizeof(font->glyphs[i].shaderName));
		s_fdOffset += sizeof(font->glyphs[i].shaderName);
	}
	font->glyphScale = GX2Font_ReadFloat();

	Q_strncpyz(font->name, name, sizeof(font->name));

	for (i = GLYPH_START; i <= GLYPH_END; i++)
		font->glyphs[i].glyph = GX2Image_RegisterNoMip(font->glyphs[i].shaderName);

	Com_Memcpy(&s_fonts[s_numFonts++], font, sizeof(fontInfo_t));
	ri.FS_FreeFile(faceData);
}
