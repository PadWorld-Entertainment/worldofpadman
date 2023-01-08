#include "tr_backend.h"
#include "tr_globals.h"
#include "tr_drawnormals.h"
#include "vk_pipelines.h"
#include "vk_shade_geometry.h"

/*
================
Draws vertex normals for debugging
================
*/
void RB_DrawNormals(shaderCommands_t *pTess, int numVertexes) {
	// VULKAN
	// int numVertexes = tess.numVertexes;
	int i = 0;
	vec4_t xyz[SHADER_MAX_VERTEXES];

	memcpy(xyz, pTess->xyz, numVertexes * sizeof(vec4_t));
	memset(pTess->svars.colors, tr.identityLightByte, SHADER_MAX_VERTEXES * sizeof(color4ub_t));

	while (i < numVertexes) {
		int count = numVertexes - i;
		int k;
		if (count >= SHADER_MAX_VERTEXES / 2 - 1)
			count = SHADER_MAX_VERTEXES / 2 - 1;

		for (k = 0; k < count; k++) {
			VectorCopy(xyz[i + k], pTess->xyz[2 * k]);
			VectorMA(xyz[i + k], 2, pTess->normal[i + k], pTess->xyz[2 * k + 1]);
		}
		pTess->numVertexes = 2 * count;
		pTess->numIndexes = 0;

		vk_UploadXYZI(pTess->xyz, pTess->numVertexes, NULL, 0);

		updateMVP(backEnd.viewParms.isPortal, backEnd.projection2D, getptr_modelview_matrix());
		vk_shade_geometry(g_stdPipelines.normals_debug_pipeline, VK_FALSE, DEPTH_RANGE_ZERO, VK_FALSE);

		i += count;
	}
}
