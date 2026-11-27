/* Minimal .shader parser -- just enough for UI/menu materials, everything else
 * skipped without desyncing. Chasing self-referencing wrapper shaders through
 * this was its own special hell. Last-defined name wins (mod override). */

#include <string.h>
#include <stdlib.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "tr_gx2_local.h"

#include <gx2/enum.h>

/* Generous limits: baseq3's scripts/ set defines several thousand shaders,
 * most never touched by this parser's actual menu/UI target. */
#define MAX_GX2_SHADER_SCRIPTS   4096
#define MAX_SHADER_SCRIPT_FILES  512

static gx2ShaderScript_t s_scripts[MAX_GX2_SHADER_SCRIPTS];
static int               s_numScripts;
static qboolean          s_scriptsInited;

/* SkipRestOfLine skips unhandled directives (tcMod, cull, sort, ...). */

static int GX2Script_NameToBlend(const char *name)
{
	if (!Q_stricmp(name, "GL_ONE"))                return GX2_BLEND_MODE_ONE;
	if (!Q_stricmp(name, "GL_ZERO"))                return GX2_BLEND_MODE_ZERO;
	if (!Q_stricmp(name, "GL_DST_COLOR"))           return GX2_BLEND_MODE_DST_COLOR;
	if (!Q_stricmp(name, "GL_ONE_MINUS_DST_COLOR")) return GX2_BLEND_MODE_INV_DST_COLOR;
	if (!Q_stricmp(name, "GL_SRC_ALPHA"))           return GX2_BLEND_MODE_SRC_ALPHA;
	if (!Q_stricmp(name, "GL_ONE_MINUS_SRC_ALPHA")) return GX2_BLEND_MODE_INV_SRC_ALPHA;
	if (!Q_stricmp(name, "GL_DST_ALPHA"))           return GX2_BLEND_MODE_DST_ALPHA;
	if (!Q_stricmp(name, "GL_ONE_MINUS_DST_ALPHA")) return GX2_BLEND_MODE_INV_DST_ALPHA;
	if (!Q_stricmp(name, "GL_SRC_COLOR"))           return GX2_BLEND_MODE_SRC_COLOR;
	if (!Q_stricmp(name, "GL_ONE_MINUS_SRC_COLOR")) return GX2_BLEND_MODE_INV_SRC_COLOR;
	if (!Q_stricmp(name, "GL_SRC_ALPHA_SATURATE"))  return GX2_BLEND_MODE_SRC_ALPHA_SAT;
	return GX2_BLEND_MODE_ONE;
}

/* Consumes tokens through the stage's closing '}'. Returns qfalse for an
 * empty/unusable stage (no map, no const color). */
static qboolean GX2Script_ParseStage(gx2ShaderStage_t *stage, const char **text)
{
	const const char *token;
	qboolean sawMap = qfalse;

	memset(stage, 0, sizeof(*stage));
	/* imageHandle stays 0, resolved lazily on first use -- self-referencing
	 * wrapper shaders broke silently here once, never again. */
	/* no blendFunc directive = opaque, matching Q3's default state bits */
	stage->blendSrc = GX2_BLEND_MODE_ONE;
	stage->blendDst = GX2_BLEND_MODE_ZERO;

	for (;;) {
		token = COM_ParseExt(text, qtrue);
		if (!token[0]) {
			ri.Printf(PRINT_ALL, "^1GX2ShaderScript: EOF inside a stage\n");
			return qfalse;
		}
		if (!strcmp(token, "}"))
			break;

		if (!Q_stricmp(token, "map") || !Q_stricmp(token, "clampmap")) {
			token = COM_ParseExt(text, qfalse);
			if (!Q_stricmp(token, "$whiteimage") || !Q_stricmp(token, "$lightmap"))
				stage->isWhiteRef = qtrue;
			else
				Q_strncpyz(stage->imagePath, token, sizeof(stage->imagePath));
			sawMap = qtrue;
		} else if (!Q_stricmp(token, "animMap")) {
			COM_ParseExt(text, qfalse); /* frequency; animation not supported, first frame only */
			token = COM_ParseExt(text, qfalse);
			Q_strncpyz(stage->imagePath, token, sizeof(stage->imagePath));
			sawMap = qtrue;
			SkipRestOfLine(text); /* remaining frame filenames */
		} else if (!Q_stricmp(token, "blendFunc")) {
			token = COM_ParseExt(text, qfalse);
			if (!Q_stricmp(token, "add")) {
				stage->blendSrc = GX2_BLEND_MODE_ONE;
				stage->blendDst = GX2_BLEND_MODE_ONE;
			} else if (!Q_stricmp(token, "filter") || !Q_stricmp(token, "modulate")) {
				stage->blendSrc = GX2_BLEND_MODE_DST_COLOR;
				stage->blendDst = GX2_BLEND_MODE_ZERO;
			} else if (!Q_stricmp(token, "blend")) {
				stage->blendSrc = GX2_BLEND_MODE_SRC_ALPHA;
				stage->blendDst = GX2_BLEND_MODE_INV_SRC_ALPHA;
			} else {
				stage->blendSrc = GX2Script_NameToBlend(token);
				token = COM_ParseExt(text, qfalse);
				stage->blendDst = GX2Script_NameToBlend(token);
			}
		} else if (!Q_stricmp(token, "rgbGen")) {
			token = COM_ParseExt(text, qfalse);
			if (!Q_stricmp(token, "const")) {
				COM_ParseExt(text, qfalse); /* ( */
				stage->constColor[0] = (float)atof(COM_ParseExt(text, qfalse));
				stage->constColor[1] = (float)atof(COM_ParseExt(text, qfalse));
				stage->constColor[2] = (float)atof(COM_ParseExt(text, qfalse));
				stage->constColor[3] = 1.0f;
				stage->hasConstColor = qtrue;
				COM_ParseExt(text, qfalse); /* ) */
			} else {
				SkipRestOfLine(text);
			}
		} else if (!Q_stricmp(token, "tcGen")) {
			token = COM_ParseExt(text, qfalse);
			if (!Q_stricmp(token, "environment"))
				stage->isEnvironmentMap = qtrue;
		} else if (!Q_stricmp(token, "alphaFunc")) {
			/* maps to the ubershader's u_Mode.w discard selector */
			token = COM_ParseExt(text, qfalse);
			if (!Q_stricmp(token, "GT0"))        stage->atestMode = 1;
			else if (!Q_stricmp(token, "LT128")) stage->atestMode = 2;
			else if (!Q_stricmp(token, "GE128")) stage->atestMode = 3;
		} else {
			SkipRestOfLine(text); /* alphaGen, tcMod, depthFunc, detail, ... */
		}
	}

	return (qboolean)(sawMap || stage->hasConstColor);
}

/* Top-level loop: shaderName '{' (stage-block | skipped-directive)* '}',
 * repeated to EOF. Bails loudly on structural desync. */
static void GX2Script_ParseFile(const char *text, const char *filename)
{
	const char *token;
	gx2ShaderScript_t *script;

	for (;;) {
		token = COM_ParseExt(&text, qtrue);
		if (!token[0])
			return;

		if (s_numScripts >= MAX_GX2_SHADER_SCRIPTS) {
			ri.Printf(PRINT_ALL, "^1GX2ShaderScript: MAX_GX2_SHADER_SCRIPTS hit, "
			          "ignoring rest of '%s'\n", filename);
			return;
		}

		script = &s_scripts[s_numScripts];
		memset(script, 0, sizeof(*script));
		Q_strncpyz(script->name, token, sizeof(script->name));

		token = COM_ParseExt(&text, qtrue);
		if (strcmp(token, "{")) {
			ri.Printf(PRINT_ALL, "^1GX2ShaderScript: '%s' in %s: expected '{', "
			          "got '%s' -- stopping parse of this file\n",
			          script->name, filename, token);
			return;
		}

		for (;;) {
			token = COM_ParseExt(&text, qtrue);
			if (!token[0]) {
				ri.Printf(PRINT_ALL, "^1GX2ShaderScript: EOF inside shader '%s'\n",
				          script->name);
				return;
			}
			if (!strcmp(token, "}"))
				break;
			if (!strcmp(token, "{")) {
				gx2ShaderStage_t stage;
				if (GX2Script_ParseStage(&stage, &text) &&
				    script->numStages < GX2_MAX_SHADER_STAGES) {
					script->stages[script->numStages++] = stage;
				}
			} else if (!Q_stricmp(token, "skyparms")) {
				/* only the outerbox matters to the skybox path */
				token = COM_ParseExt(&text, qfalse);
				script->isSky = qtrue;
				if (token[0] && strcmp(token, "-"))
					Q_strncpyz(script->skyBox, token, sizeof(script->skyBox));
				SkipRestOfLine(&text); /* cloudheight + innerbox */
			} else {
				SkipRestOfLine(&text);
			}
		}

		/* sky shaders kept even with zero usable stages, so the outerbox is findable by name */
		if (script->numStages > 0 || script->isSky)
			s_numScripts++;
	}
}

void GX2ShaderScript_Init(void)
{
	char **files;
	int    numFiles, i;

	if (s_scriptsInited)
		return;
	s_scriptsInited = qtrue;

	files = ri.FS_ListFiles("scripts", ".shader", &numFiles);
	if (!files || !numFiles) {
		ri.Printf(PRINT_ALL, "GX2ShaderScript: no scripts/*.shader files found\n");
		return;
	}
	if (numFiles > MAX_SHADER_SCRIPT_FILES)
		numFiles = MAX_SHADER_SCRIPT_FILES;

	for (i = 0; i < numFiles; i++) {
		char  path[MAX_QPATH];
		void *buf = NULL;

		Com_sprintf(path, sizeof(path), "scripts/%s", files[i]);
		if (ri.FS_ReadFile(path, &buf) > 0 && buf) {
			GX2Script_ParseFile(buf, path);
			ri.FS_FreeFile(buf);
		}
	}
	ri.FS_FreeFileList(files);

	ri.Printf(PRINT_ALL, "GX2ShaderScript: parsed %d multi-stage shader(s) from %d file(s)\n",
	          s_numScripts, numFiles);
}

const gx2ShaderScript_t *GX2ShaderScript_Find(const char *name)
{
	char stripped[MAX_QPATH];
	int  i;

	COM_StripExtension(name, stripped, sizeof(stripped));

	/* last-defined wins (mod scripts/ override baseq3's) */
	for (i = s_numScripts - 1; i >= 0; i--) {
		if (!Q_stricmp(s_scripts[i].name, stripped) || !Q_stricmp(s_scripts[i].name, name))
			return &s_scripts[i];
	}
	return NULL;
}
