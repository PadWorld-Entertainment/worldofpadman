#include "tr_fog.h"
#include <math.h>
#include <stdlib.h>

#define FOG_TABLE_SIZE 256

static float FogTable[FOG_TABLE_SIZE];

void R_InitFogTable(void) {
	float exp = 0.5f;
	unsigned int i;

	for (i = 0; i < FOG_TABLE_SIZE; i++) {
		FogTable[i] = pow((float)i / (FOG_TABLE_SIZE - 1), exp);
	}
}

/*
================
Returns a 0.0 to 1.0 fog density value
This is called for each texel of the fog texture on startup
and for each vertex of transparent shaders in fog dynamically
================
*/
float R_FogFactor(float s, float t) {
	s -= 1.0f / 512.0f;
	if (s < 0.0f) {
		return 0.0f;
	}
	if (t < 1.0f / 32.0f) {
		return 0.0f;
	}
	if (t < 31.0f / 32.0f) {
		s *= (t - 1.0f / 32.0f) / (30.0f / 32.0f);
	}

	// we need to leave a lot of clamp range
	s *= 8.0f;

	if (s > 1.0f) {
		s = 1.0f;
	}

	return FogTable[(int)(s * (FOG_TABLE_SIZE - 1))];
}
