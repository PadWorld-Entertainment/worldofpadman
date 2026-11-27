/* refdef_t -> view/projection matrix + RenderScene glue. Camera math ported
 * from renderergl1/tr_main.c, targeting a uniform block instead of glLoadMatrixf. */

#include <math.h>
#include <string.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "tr_gx2_local.h"

/* Q3 view space (+X, Z up) -> GL camera space (-Z, Y up). */
static const float s_flipMatrix[16] = {
	0, 0, -1, 0,
	-1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 0, 1
};

static void GX2Scene_MultMatrix(const float *a, const float *b, float *out)
{
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			out[i * 4 + j] =
				a[i * 4 + 0] * b[0 * 4 + j] +
				a[i * 4 + 1] * b[1 * 4 + j] +
				a[i * 4 + 2] * b[2 * 4 + j] +
				a[i * 4 + 3] * b[3 * 4 + j];
		}
	}
}

/* Entity list, filled per-frame by cgame. Only RT_MODEL gets queued -- sprites, beams,
 * etc. are skipped, nothing else drew yet anyway. */

#define GX2_MAX_SCENE_ENTITIES 256

static refEntity_t s_sceneEntities[GX2_MAX_SCENE_ENTITIES];
static int         s_numSceneEntities = 0;

void GX2Scene_ClearEntities(void)
{
	s_numSceneEntities = 0;
}

void GX2Scene_AddEntity(const refEntity_t *re)
{
	if (s_numSceneEntities >= GX2_MAX_SCENE_ENTITIES)
		return; /* silently dropped, no budget enforcement yet */
	s_sceneEntities[s_numSceneEntities++] = *re;
}

typedef struct {
	const float *mvp;
	qhandle_t    customShader;
} gx2EntEmitCtx_t;

static void GX2Scene_EmitEntitySurface(void *ctx0, qhandle_t texHandle,
                                       const gx2vert_t *verts, uint32_t numVerts,
                                       const uint32_t *indices, uint32_t numIndexes)
{
	gx2EntEmitCtx_t *ctx = (gx2EntEmitCtx_t *)ctx0;
	qhandle_t tex = ctx->customShader ? ctx->customShader : texHandle;

	GX2R_AddEntityDraw(ctx->mvp, tex, verts, numVerts, indices, numIndexes);
}

/* Builds object->world matrix from refEntity_t axis[]/origin, column-major. */
static void GX2Scene_BuildEntityModelMatrix(const refEntity_t *re, float *m)
{
	m[0] = re->axis[0][0]; m[1] = re->axis[0][1]; m[2] = re->axis[0][2]; m[3] = 0.0f;
	m[4] = re->axis[1][0]; m[5] = re->axis[1][1]; m[6] = re->axis[1][2]; m[7] = 0.0f;
	m[8] = re->axis[2][0]; m[9] = re->axis[2][1]; m[10] = re->axis[2][2]; m[11] = 0.0f;
	m[12] = re->origin[0]; m[13] = re->origin[1]; m[14] = re->origin[2]; m[15] = 1.0f;
}

/* Port of renderergl1's R_SetupEntityLighting minus dynamic lights: grid
 * sample, flat fallback if no grid, min-light clamp, world->local rotation. */
static void GX2Scene_SetupEntityLighting(const refdef_t *fd, const refEntity_t *re,
                                         gx2ModelDrawParams_t *params)
{
	vec3_t lightOrigin, worldDir;
	int i;

	if (re->renderfx & RF_LIGHTING_ORIGIN) { /* separate lighting origin */
		VectorCopy(re->lightingOrigin, lightOrigin);
	} else {
		VectorCopy(re->origin, lightOrigin);
	}

	if ((fd->rdflags & RDF_NOWORLDMODEL) ||
	    !GX2World_LightForPoint(lightOrigin, params->ambientLight,
	                            params->directedLight, worldDir)) {
		/* no grid (menu/-nolight maps): flat fallback */
		VectorSet(params->ambientLight, 150.0f, 150.0f, 150.0f);
		VectorSet(params->directedLight, 150.0f, 150.0f, 150.0f);
		VectorSet(worldDir, 0.45f, 0.3f, 0.9f);
		VectorNormalize(worldDir);
	}

	/* min-light so entities never go pitch black, then clamp to byte range */
	for (i = 0; i < 3; i++) {
		params->ambientLight[i] += 32.0f;
		if (params->ambientLight[i] > 255.0f)
			params->ambientLight[i] = 255.0f;
		if (params->directedLight[i] > 255.0f)
			params->directedLight[i] = 255.0f;
	}

	params->lightDir[0] = DotProduct(worldDir, re->axis[0]);
	params->lightDir[1] = DotProduct(worldDir, re->axis[1]);
	params->lightDir[2] = DotProduct(worldDir, re->axis[2]);
	params->lit = qtrue;
}

/* Builds view*flip*proj mvp, sets world/sky MVPs, queues lit entity draws.
 * No PVS/frustum culling yet -- brute force, and it shows. */
void GX2Scene_RenderScene(const refdef_t *fd)
{
	float viewerMatrix[16];
	float modelMatrix[16];
	float projMatrix[16];
	float mvp[16];
	vec3_t origin;
	float xmin, xmax, ymin, ymax, width, height;
	float zNear, zFar;

	if ((fd->rdflags & RDF_NOWORLDMODEL) || !GX2World_HasWorld())
		return;

	VectorCopy(fd->vieworg, origin);

	viewerMatrix[0] = fd->viewaxis[0][0];
	viewerMatrix[4] = fd->viewaxis[0][1];
	viewerMatrix[8] = fd->viewaxis[0][2];
	viewerMatrix[12] = -origin[0] * viewerMatrix[0] + -origin[1] * viewerMatrix[4] + -origin[2] * viewerMatrix[8];

	viewerMatrix[1] = fd->viewaxis[1][0];
	viewerMatrix[5] = fd->viewaxis[1][1];
	viewerMatrix[9] = fd->viewaxis[1][2];
	viewerMatrix[13] = -origin[0] * viewerMatrix[1] + -origin[1] * viewerMatrix[5] + -origin[2] * viewerMatrix[9];

	viewerMatrix[2] = fd->viewaxis[2][0];
	viewerMatrix[6] = fd->viewaxis[2][1];
	viewerMatrix[10] = fd->viewaxis[2][2];
	viewerMatrix[14] = -origin[0] * viewerMatrix[2] + -origin[1] * viewerMatrix[6] + -origin[2] * viewerMatrix[10];

	viewerMatrix[3] = 0;
	viewerMatrix[7] = 0;
	viewerMatrix[11] = 0;
	viewerMatrix[15] = 1;

	GX2Scene_MultMatrix(viewerMatrix, s_flipMatrix, modelMatrix);

	zNear = 4.0f;
	zFar = 8192.0f; /* fixed far plane, no world bounds pass yet */

	ymax = zNear * tanf(fd->fov_y * (float)M_PI / 360.0f);
	ymin = -ymax;
	xmax = zNear * tanf(fd->fov_x * (float)M_PI / 360.0f);
	xmin = -xmax;
	width = xmax - xmin;
	height = ymax - ymin;

	memset(projMatrix, 0, sizeof(projMatrix));
	projMatrix[0] = 2.0f * zNear / width;
	projMatrix[8] = (xmax + xmin) / width;
	projMatrix[5] = 2.0f * zNear / height;
	projMatrix[9] = (ymax + ymin) / height;
	projMatrix[10] = -(zFar + zNear) / (zFar - zNear);
	projMatrix[14] = -2.0f * zFar * zNear / (zFar - zNear);
	projMatrix[11] = -1.0f;

	/* MultMatrix(a,b,out) computes out=b*a, so model must be "a" for mvp=proj*model. Took forever. */
	GX2Scene_MultMatrix(modelMatrix, projMatrix, mvp);

	GX2R_SetWorldMVP(mvp);

	/* skybox: same view/proj but rotation-only, box stays locked to camera */
	if (GX2World_HasSky()) {
		float skyViewer[16], skyModel[16], skyMvp[16];

		memcpy(skyViewer, viewerMatrix, sizeof(skyViewer));
		skyViewer[12] = skyViewer[13] = skyViewer[14] = 0.0f;
		GX2Scene_MultMatrix(skyViewer, s_flipMatrix, skyModel);
		GX2Scene_MultMatrix(skyModel, projMatrix, skyMvp);
		GX2R_SetSkyMVP(skyMvp);
	}

	/* entities: modelMatrix here is actually view*flip; entityMvp = proj*view*entityModel */
	{
		int i;

		for (i = 0; i < s_numSceneEntities; i++) {
			const refEntity_t *re = &s_sceneEntities[i];
			float entityModel[16];
			float viewModel[16];
			float entityMvp[16];
			gx2EntEmitCtx_t ctx;
			gx2ModelDrawParams_t params;

			if (re->reType != RT_MODEL || !re->hModel)
				continue;

			/* Skip own body (RF_THIRD_PERSON), no mirror support -- missing this rendered
			 * a degenerate wedge at the near plane and cost an embarrassing debug session. */
			if (re->renderfx & RF_THIRD_PERSON)
				continue;

			GX2Scene_BuildEntityModelMatrix(re, entityModel);
			GX2Scene_MultMatrix(entityModel, modelMatrix, viewModel);
			GX2Scene_MultMatrix(viewModel, projMatrix, entityMvp);

			ctx.mvp = entityMvp;
			ctx.customShader = re->customShader;

			GX2Scene_SetupEntityLighting(fd, re, &params);
			params.customSkin = re->customSkin;
			params.skinNum = re->skinNum;

			GX2Model_Draw(re->hModel, re->frame, re->oldframe, re->backlerp,
			              &params, GX2Scene_EmitEntitySurface, &ctx);
		}
	}
}
