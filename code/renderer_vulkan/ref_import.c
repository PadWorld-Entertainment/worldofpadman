#include "ref_import.h"
#include "../qcommon/q_shared.h"
#include "../renderercommon/tr_public.h"

extern refexport_t *R_Export(void);

refimport_t ri;

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

	ri = *rimp;

	if (apiVersion != REF_API_VERSION) {
		ri.Printf(PRINT_ALL, "Mismatched REF_API_VERSION: expected %i, got %i\n", REF_API_VERSION, apiVersion);
		return NULL;
	}

	return R_Export();
}

//
// common function replacements for modular renderer
//
#ifdef USE_RENDERER_DLOPEN

void QDECL Com_Printf(const char *msg, ...) {
	va_list argptr;
	char text[1024];

	va_start(argptr, msg);
	Q_vsnprintf(text, sizeof(text), msg, argptr);
	va_end(argptr);

	ri.Printf(PRINT_ALL, "%s", text);
}

void QDECL Com_Error(int level, const char *error, ...) {
	va_list argptr;
	char text[1024];

	va_start(argptr, error);
	Q_vsnprintf(text, sizeof(text), error, argptr);
	va_end(argptr);

	ri.Error(level, "%s", text);
}

#endif

char *R_SkipPath(char *pathname) {
	char *last = pathname;
	while (*pathname) {
		if (*pathname == '/')
			last = pathname + 1;
		pathname++;
	}
	return last;
}

void R_StripExtension(const char *in, char *out, int destsize) {
	const char *dot = strrchr(in, '.'), *slash;

	if (dot && (!(slash = strrchr(in, '/')) || slash < dot))
		destsize = (destsize < dot - in + 1 ? destsize : dot - in + 1);

	if (in == out && destsize > 1)
		out[destsize - 1] = '\0';
	else
		Q_strncpyz(out, in, destsize);
}
