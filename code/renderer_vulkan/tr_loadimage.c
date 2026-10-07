#include "ref_import.h"
#include "tr_local.h"
#include "vk_image.h"

void R_LoadBMP(const char *name, byte **pic, int *width, int *height);
void R_LoadJPG(const char *name, byte **pic, int *width, int *height);
void R_LoadPCX(const char *name, byte **pic, int *width, int *height);
void R_LoadPNG(const char *name, byte **pic, int *width, int *height);
void R_LoadTGA(const char *name, byte **pic, int *width, int *height);

// Description:  Loads any of the supported image types into
// a cannonical 32 bit format.

typedef struct {
	char *ext;
	void (*ImageLoader)(const char *, unsigned char **, int *, int *);
} imageExtToLoaderMap_t;

// Note that the ordering indicates the order of preference used
// when there are multiple images of different formats available
static const imageExtToLoaderMap_t imageLoaders[6] = {{"png", R_LoadPNG},  {"tga", R_LoadTGA}, {"jpg", R_LoadJPG},
													  {"jpeg", R_LoadJPG}, {"pcx", R_LoadPCX}, {"bmp", R_LoadBMP}};

static const int numImageLoaders = 6;

void R_LoadImage(const char *name, unsigned char **pic, int *width, int *height) {
	qboolean orgNameFailed = qfalse;
	int orgLoader = -1;
	int i;
	char localName[MAX_QPATH];
	const char *ext;
	char *altName;

	*pic = NULL;
	*width = 0;
	*height = 0;

	Q_strncpyz(localName, name, sizeof(localName));
	ext = COM_GetExtension(localName);

	// If an explicit extension was given, try that loader first
	if (ext && *ext) {
		for (i = 0; i < numImageLoaders; i++) {
			if (!Q_stricmp(ext, imageLoaders[i].ext)) {
				imageLoaders[i].ImageLoader(localName, pic, width, height);
				if (*pic)
					return;
				orgNameFailed = qtrue;
				orgLoader = i;
				break;
			}
		}
		// Strip extension for fallback probing
		COM_StripExtension(name, localName, sizeof(localName));
	}

	// Try all supported formats (skipping the one that already failed)
	for (i = 0; i < numImageLoaders; i++) {
		if (i == orgLoader)
			continue;
		altName = va("%s.%s", localName, imageLoaders[i].ext);
		imageLoaders[i].ImageLoader(altName, pic, width, height);
		if (*pic) {
			if (orgNameFailed)
				ri.Printf(PRINT_DEVELOPER, "WARNING: %s not present, using %s instead\n", name, altName);
			return;
		}
	}
}
