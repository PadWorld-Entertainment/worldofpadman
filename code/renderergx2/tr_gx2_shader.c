/* Loads the q3uber shader pair from the checked-in GFD blob. No runtime
 * glslcompiler.rpl -- it hangs the console on unload, confirmed on hardware. */

#include <string.h>

#include <whb/gfx.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "tr_gx2_local.h"

extern const unsigned char q3uber_gsh[];
extern const unsigned int q3uber_gsh_len;

/* Reflection dump: names + offsets/locations, logged once per loaded shader. */
static void GX2Shader_DumpVSReflection(const char *name, const GX2VertexShader *vs)
{
	uint32_t i;
	ri.Printf(PRINT_ALL, "GX2Shader: VS '%s' reflection: mode=%d size=%u\n",
	          name, (int)vs->mode, (unsigned)vs->size);
	for (i = 0; i < vs->uniformBlockCount; i++)
		ri.Printf(PRINT_ALL, "  uniformBlock[%u] name=%s offset=%u size=%u\n", i,
		          vs->uniformBlocks[i].name, (unsigned)vs->uniformBlocks[i].offset,
		          (unsigned)vs->uniformBlocks[i].size);
	for (i = 0; i < vs->uniformVarCount; i++)
		ri.Printf(PRINT_ALL, "  uniformVar[%u] name=%s offset=%u block=%d\n", i,
		          vs->uniformVars[i].name, (unsigned)vs->uniformVars[i].offset,
		          (int)vs->uniformVars[i].block);
	for (i = 0; i < vs->attribVarCount; i++)
		ri.Printf(PRINT_ALL, "  attribVar[%u] name=%s location=%u\n", i,
		          vs->attribVars[i].name, (unsigned)vs->attribVars[i].location);
	for (i = 0; i < vs->samplerVarCount; i++)
		ri.Printf(PRINT_ALL, "  samplerVar[%u] name=%s location=%u\n", i,
		          vs->samplerVars[i].name, (unsigned)vs->samplerVars[i].location);
}

static void GX2Shader_DumpPSReflection(const char *name, const GX2PixelShader *ps)
{
	uint32_t i;
	ri.Printf(PRINT_ALL, "GX2Shader: PS '%s' reflection: mode=%d size=%u\n",
	          name, (int)ps->mode, (unsigned)ps->size);
	for (i = 0; i < ps->uniformBlockCount; i++)
		ri.Printf(PRINT_ALL, "  uniformBlock[%u] name=%s offset=%u size=%u\n", i,
		          ps->uniformBlocks[i].name, (unsigned)ps->uniformBlocks[i].offset,
		          (unsigned)ps->uniformBlocks[i].size);
	for (i = 0; i < ps->uniformVarCount; i++)
		ri.Printf(PRINT_ALL, "  uniformVar[%u] name=%s offset=%u block=%d\n", i,
		          ps->uniformVars[i].name, (unsigned)ps->uniformVars[i].offset,
		          (int)ps->uniformVars[i].block);
	for (i = 0; i < ps->samplerVarCount; i++)
		ri.Printf(PRINT_ALL, "  samplerVar[%u] name=%s location=%u\n", i,
		          ps->samplerVars[i].name, (unsigned)ps->samplerVars[i].location);
}

/* `source` unused; shader comes from the embedded GFD blob, not GLSL text. */
GX2VertexShader *GX2Shader_CompileVS(const char *name, const char *source)
{
	GX2VertexShader *vs;

	(void)source;

	vs = WHBGfxLoadGFDVertexShader(0, q3uber_gsh);
	if (!vs) {
		ri.Printf(PRINT_ALL, "^1GX2Shader: VERTEX shader '%s' FAILED to load from GFD blob\n", name);
		return NULL;
	}

	GX2Shader_DumpVSReflection(name, vs);
	return vs;
}

GX2PixelShader *GX2Shader_CompilePS(const char *name, const char *source)
{
	GX2PixelShader *ps;

	(void)source;

	ps = WHBGfxLoadGFDPixelShader(0, q3uber_gsh);
	if (!ps) {
		ri.Printf(PRINT_ALL, "^1GX2Shader: PIXEL shader '%s' FAILED to load from GFD blob\n", name);
		return NULL;
	}

	GX2Shader_DumpPSReflection(name, ps);
	return ps;
}
