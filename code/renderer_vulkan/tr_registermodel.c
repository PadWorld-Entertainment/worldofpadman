#include "ref_import.h"
#include "tr_globals.h"
#include "tr_local.h"
#include "tr_model.h"
#include "render_export.h"

typedef struct {
	const char *ext;
	qhandle_t (*ModelLoader)(const char *, model_t *);
} modelExtToLoaderMap_t;

// Note that the ordering indicates the order of preference used
// when there are multiple models of different formats available
static const modelExtToLoaderMap_t modelLoaders[] = {
	{"md3", R_RegisterMD3}, {"mdr", R_RegisterMDR}, {"iqm", R_RegisterIQM}};

static const uint32_t numModelLoaders = ARRAY_LEN(modelLoaders);

/*
====================
Loads in a model for the given name

Zero will be returned if the model fails to load.
An entry will be retained for failed models as an
optimization to prevent disk rescanning if they are
asked for again.
====================
*/
qhandle_t RE_RegisterModel(const char *name) {
	qboolean orgNameFailed = qfalse;
	int orgLoader = -1;
	qhandle_t hModel = 0;
	model_t *mod;
	int i;
	char localName[MAX_QPATH];
	const char *ext;
	char altName[MAX_QPATH];

	if (!name || !name[0]) {
		ri.Printf(PRINT_ALL, "RE_RegisterModel: NULL name\n");
		return 0;
	}

	if (strlen(name) >= MAX_QPATH) {
		ri.Printf(PRINT_ALL, "Model name exceeds MAX_QPATH\n");
		return 0;
	}

	//
	// search the currently loaded models
	//
	for (hModel = 1; hModel < tr.numModels; hModel++) {
		if (0 == strcmp(tr.models[hModel]->name, name)) {
			if (tr.models[hModel]->type == MOD_BAD) {
				return 0;
			}
			return hModel;
		}
	}

	// allocate a new model_t
	mod = ri.Hunk_Alloc(sizeof(model_t), h_low);

	// only set the name after the model has been successfully loaded
	Q_strncpyz(mod->name, name, MAX_QPATH);
	mod->index = tr.numModels;
	mod->type = MOD_BAD;
	mod->numLods = 0;

	tr.models[tr.numModels] = mod;

	if (++tr.numModels > MAX_MOD_KNOWN) {
		ri.Printf(PRINT_WARNING, "RE_RegisterModel: MAX_MOD_KNOWN.\n");
	}

	//
	// load the files
	//
	Q_strncpyz(localName, name, sizeof(localName));
	ext = COM_GetExtension(localName);

	if (*ext) {
		// Look for the correct loader and use it
		for (i = 0; i < (int)numModelLoaders; i++) {
			if (!Q_stricmp(ext, modelLoaders[i].ext)) {
				hModel = modelLoaders[i].ModelLoader(localName, mod);
				break;
			}
		}

		// A loader was found
		if (i < (int)numModelLoaders) {
			if (!hModel) {
				// Loader failed, try again without the extension
				orgNameFailed = qtrue;
				orgLoader = i;
				COM_StripExtension(name, localName, sizeof(localName));
			} else {
				return hModel;
			}
		}
	}

	// Try and find a suitable match using all the model formats supported
	for (i = 0; i < (int)numModelLoaders; i++) {
		if (i == orgLoader)
			continue;

		Com_sprintf(altName, sizeof(altName), "%s.%s", localName, modelLoaders[i].ext);

		hModel = modelLoaders[i].ModelLoader(altName, mod);

		if (hModel) {
			if (orgNameFailed) {
				ri.Printf(PRINT_DEVELOPER, "WARNING: %s not present, using %s instead\n", name, altName);
			}
			break;
		}
	}

	return hModel;
}
