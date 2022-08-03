#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	int fileofs, filelen;
} lump_t;

#define LUMP_ENTITIES 0
#define LUMP_SHADERS 1
#define LUMP_PLANES 2
#define LUMP_NODES 3
#define LUMP_LEAFS 4
#define LUMP_LEAFSURFACES 5
#define LUMP_LEAFBRUSHES 6
#define LUMP_MODELS 7
#define LUMP_BRUSHES 8
#define LUMP_BRUSHSIDES 9
#define LUMP_DRAWVERTS 10
#define LUMP_DRAWINDEXES 11
#define LUMP_FOGS 12
#define LUMP_SURFACES 13
#define LUMP_LIGHTMAPS 14
#define LUMP_LIGHTGRID 15
#define LUMP_VISIBILITY 16
#define HEADER_LUMPS 17
#define BSP_VERSION 46

typedef struct {
	int ident;
	int version;

	lump_t lumps[HEADER_LUMPS];
} dheader_t;

typedef struct {
	char shader[64];
	int surfaceFlags;
	int contentFlags;
} dshader_t;

#define MAX_TOKEN_CHARS 1024

static int com_tokenline = 0;
static int com_lines = 0;
static char com_token[MAX_TOKEN_CHARS];

static const char *SkipWhitespace(const char *data) {
	char c;

	while ((c = *data) <= ' ') {
		if (c == '\n') {
			com_lines++;
		}
		if (!c) {
			return NULL;
		}
		data++;
	}

	return data;
}

static const char *COM_ParseExt(const char **data_p) {
	char c = 0;
	int len;
	const char *data;

	data = *data_p;
	len = 0;
	com_token[0] = 0;
	com_tokenline = 0;

	// make sure incoming data is valid
	if (!data) {
		*data_p = NULL;
		return com_token;
	}

	while (1) {
		// skip whitespace
		data = SkipWhitespace(data);
		if (!data) {
			*data_p = NULL;
			return com_token;
		}

		c = *data;
		if (c == '/' && data[1] == '/') {
			// skip double slash comments
			data += 2;
			while (*data && *data != '\n') {
				data++;
			}
		} else if (c == '/' && data[1] == '*') {
			// skip /* */ comments
			data += 2;
			while (*data && (*data != '*' || data[1] != '/')) {
				if (*data == '\n') {
					com_lines++;
				}
				data++;
			}
			if (*data) {
				data += 2;
			}
		} else {
			break;
		}
	}

	// token starts on this line
	com_tokenline = com_lines;

	// handle quoted strings
	if (c == '\"') {
		data++;
		while (1) {
			c = *data++;
			if (c == '\"' || !c) {
				com_token[len] = 0;
				*data_p = (char *)data;
				return com_token;
			}
			if (c == '\n') {
				com_lines++;
			}
			if (len < MAX_TOKEN_CHARS - 1) {
				com_token[len] = c;
				len++;
			}
		}
	}

	// parse a regular word
	do {
		if (len < MAX_TOKEN_CHARS - 1) {
			com_token[len] = c;
			len++;
		}
		data++;
		c = *data;
	} while (c > 32);

	com_token[len] = 0;

	*data_p = (char *)data;
	return com_token;
}

static void Q_strncpyz(char *dest, const char *src, int destsize) {
	strncpy(dest, src, destsize - 1);
	dest[destsize - 1] = 0;
}

static void sanitizeFilename(char *in) {
	int i;
	int l = (int)strlen(in);
	for (i = 0; i < l; ++i) {
		if (in[i] == '\\') {
			in[i] = '/';
		}
	}
}

static int validateSound(const char* filename, const char *pk3dir, const char *sound) {
	FILE *fp;
	char buf[1024];
	char basename[1024];
	int len;
	static const char *ext[] = {"wav", "ogg", "opus", NULL};
	static const char *searchpaths[] = {"sound.pk3dir", NULL};
	static const char *subdirs[] = {".", "../wop"};

	if (sound[0] == '*') {
		return 0;
	}

	Q_strncpyz(basename, sound, sizeof(basename));
	len = strlen(basename);
	if (len <= 3) {
		return 0;
	}
	// remove extension
	if (basename[len - 4] == '.') {
		basename[len - 4] = '\0';
	}

	sanitizeFilename(basename);

	// no extension in filename - append supported extensions and check for for those
	for (const char **e = ext; *e; ++e) {
		for (const char **searchpath = searchpaths; *searchpath; ++searchpath) {
			for (const char **sd = subdirs; *sd; ++sd) {
				snprintf(buf, sizeof(buf), "%s/%s/%s/%s.%s", pk3dir, *sd, *searchpath, basename, *e);
				if ((fp = fopen(buf, "r")) != NULL) {
					fclose(fp);
					return 0;
				}
			}
		}
	}

	printf("%s: error in bsp: sound file %s not found\n", filename, sound);
	return 1;
}

static int validateEntityString(const char* filename, const char *pk3dir, const char *entitystring) {
	int error = 0;
	for (;;) {
		const char *token = COM_ParseExt(&entitystring);
		if (!token[0]) {
			break;
		}
		if (!strcmp(token, "noise")) {
			token = COM_ParseExt(&entitystring);
			if (!token[0]) {
				printf("%s: error unexpected end of entity string found\n", filename);
				return 1;
			}
			if (validateSound(filename, pk3dir, token) != 0) {
				error = 1;
			}
		}
	}

	return error;
}

static int validateTexture(const char *shaderName, const char *bspfilename, const char *pk3dir) {
	FILE *fp;
	char buf[1024];
	char basename[1024];
	int len;
	static const char *ext[] = {"jpg", "png", "tga", NULL};
	static const char *searchpaths[] = {"models.pk3dir", "textures.pk3dir", NULL};
	static const char *subdirs[] = {".", "../wop", "../xmas"};

	Q_strncpyz(basename, shaderName, sizeof(basename));
	len = strlen(basename);
	if (len <= 3) {
		return 0;
	}
	// remove extension
	if (basename[len - 4] == '.') {
		basename[len - 4] = '\0';
	}

	// no extension in filename - append supported extensions and check for for those
	for (const char **e = ext; *e; ++e) {
		for (const char **searchpath = searchpaths; *searchpath; ++searchpath) {
			for (const char **sd = subdirs; *sd; ++sd) {
				snprintf(buf, sizeof(buf), "%s/%s/%s/%s.%s", pk3dir, *sd, *searchpath, basename, *e);
				if ((fp = fopen(buf, "r")) != NULL) {
					fclose(fp);
					return 0;
				}
			}
		}
	}

	printf("%s: error in bsp: shader '%s' not found\n", bspfilename, shaderName);
	return 1;
}

static int validateBsp(const char *filename, const char *pk3dir, const void *buf) {
	const dheader_t header = *(dheader_t *)buf;
	printf("Validate bsp %s\n", filename);
	if (header.version != BSP_VERSION) {
		printf("%s: error invalid bsp version found: %i\n", filename, header.version);
		return 1;
	}

	int errors = 0;
	{
		const lump_t *l = &header.lumps[LUMP_ENTITIES];
		char *entityString = (char*)malloc(l->filelen);
		Q_strncpyz(entityString, (const char*)((unsigned char*)buf + l->fileofs), l->filelen);

		if (validateEntityString(filename, pk3dir, entityString) != 0) {
			++errors;
		}
		free(entityString);
	}
	{
		const lump_t *l = &header.lumps[LUMP_SHADERS];
		dshader_t *shaders = (dshader_t *)(void *)((unsigned char*)buf + l->fileofs);
		const int count = l->filelen / sizeof(*shaders);
		int i;

		if (l->filelen % sizeof(*shaders)) {
			printf("%s: error in bsp: shader lump size found\n", filename);
			++errors;
		} else {
			for (i = 0; i < count; ++i) {
				const char *shader = shaders[i].shader;
				if (validateTexture(shader, filename, pk3dir) == 0) {
					++errors;
				}
			}
		}
	}
	return errors;
}

int main(int argc, char *argv[]) {
	const char *filename;
	const char *pk3dir;
	long bufsize;
	char *source;
	FILE *fp;

	if (argc != 3) {
		printf("Usage: bsptool <path-to-game-dir> <path-to.bsp>\n");
		return 1;
	}

	pk3dir = argv[1];
	filename = argv[2];
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("Could not open file %s", filename);
		return 127;
	}

	if (fseek(fp, 0L, SEEK_END) != 0) {
		perror("Error: failed to seek the file stream. ");
		return 1;
	}

	bufsize = ftell(fp);
	if (bufsize == -1) {
		perror("Error: failed to get size of shader file. ");
		return 1;
	}

	if (fseek(fp, 0L, SEEK_SET) != 0) {
		perror("Error: failed to reset file stream. ");
		return 1;
	}

	source = (char *)malloc(bufsize + 1);
	size_t newLen = fread(source, 1, bufsize, fp);
	if (ferror(fp) != 0) {
		perror("Error: failed to read bsp file. ");
		return 1;
	}

	if (validateBsp(filename, pk3dir, source) == 0) {
		free(source);
		return 0;
	}
	free(source);
	return 1;
}
