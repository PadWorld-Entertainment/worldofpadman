#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include "win/dirent.h"
#else
#include <dirent.h>
#endif

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

#define WOP_MAPS 0
#define WOP_PADPAK 1
#define WOP_COMMUNITY 2

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
				*data_p = (const char *)data;
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

	*data_p = (const char *)data;
	return com_token;
}

static int SkipBracedSection(const char **program, int depth) {
	const char *token;

	do {
		token = COM_ParseExt(program);
		if (token[1] == 0) {
			if (token[0] == '{') {
				depth++;
			} else if (token[0] == '}') {
				depth--;
			}
		}
	} while (depth && *program);

	return depth;
}

static const char *FindShaderInShaderText(const char *shadername, const char *allShaders) {
	const char *token, *p = allShaders;
	if (!p) {
		return NULL;
	}

	while (1) {
		token = COM_ParseExt(&p);
		if (token[0] == 0) {
			break;
		}


		if (!strcmp(token, shadername)) {
			return p;
		} else {
			const int depth = SkipBracedSection(&p, 0);
			if (depth != 0) {
				printf("Error: missing brace in shader file: %i\n", depth);
				exit(1);
			}
		}
	}

	return NULL;
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

static int validateSound(const char *filename, const char *pk3dir, const char *sound, int mappacktype) {
	FILE *fp;
	char buf[1024];
	char basename[1024];
	int len;
	static const char *ext[] = {"wav", "ogg", "opus", NULL};
	const char *searchpaths[] = {"sound.pk3dir", NULL, NULL};
	if (mappacktype == WOP_PADPAK) {
		searchpaths[1] = "padpack.pk3dir";
	} else if (mappacktype == WOP_COMMUNITY) {
		searchpaths[1] = "mappack.pk3dir";
	}
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

static int validateEntityString(const char *filename, const char *pk3dir, const char *entitystring, int mappacktype) {
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
			if (validateSound(filename, pk3dir, token, mappacktype) != 0) {
				error = 1;
			}
		}
	}

	return error;
}

static int checkForShader(const char *allShaders, const char *shaderName, const char *bspfilename) {
	if (!strcmp(shaderName, "noshader")) {
		return 0;
	}
	const char *shader = FindShaderInShaderText(shaderName, allShaders);
	if (shader == NULL) {
		printf("%s: error in bsp: texture or shader '%s' not found\n", bspfilename, shaderName);
		return 1;
	}
	return 0;
}

static int validateShader(const char *allShaders, const char *shaderName, const char *bspfilename, const char *pk3dir,
						  int mappacktype) {
	FILE *fp;
	char buf[1024];
	char basename[1024];
	int len;
	static const char *ext[] = {"jpg", "png", "tga", NULL};
	const char *searchpaths[] = {"models.pk3dir", "textures.pk3dir", NULL, NULL};
	if (mappacktype == WOP_PADPAK) {
		searchpaths[2] = "padpack.pk3dir";
	} else if (mappacktype == WOP_COMMUNITY) {
		searchpaths[2] = "mappack.pk3dir";
	}

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

	// no extension in filename - append supported extensions and check for those
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

	return checkForShader(allShaders, shaderName, bspfilename);
}

static int filter(const struct dirent *entry) {
	return strstr(entry->d_name, ".shader") != NULL;
}

static char *loadAllShaders(const char *pk3dir, int mappacktype) {
	const int maxShaderSize = 5 * 1024 * 1024;
	char *shaders = (char *)malloc(maxShaderSize);
	char buf[1024];
	int complete_filesize = 0;
	static const char *subdirs[] = {".", "../xmas"};
	const char *searchpaths[] = {"scripts.pk3dir", NULL, NULL};
	int shaderCount = 0;

	if (mappacktype == WOP_PADPAK) {
		searchpaths[1] = "padpack.pk3dir";
	} else if (mappacktype == WOP_COMMUNITY) {
		searchpaths[1] = "mappack.pk3dir";
	}

	shaders[0] = '\0';

	for (const char **searchpath = searchpaths; *searchpath; ++searchpath) {
		for (const char **sd = subdirs; *sd; ++sd) {
			snprintf(buf, sizeof(buf), "%s/%s/%s/scripts", pk3dir, *sd, *searchpath);
			struct dirent **files;
			int n = scandir(buf, &files, filter, alphasort);
			if (n <= 0) {
				printf("Failed to scan directory %s\n", buf);
				continue;
			}

			for (int i = 0; i < n; ++i) {
				FILE *fp;
				int file_size;
				char *buffer;

				if (files[i]->d_type != DT_REG) {
					continue;
				}
				snprintf(buf, sizeof(buf), "%s/%s/%s/scripts/%s", pk3dir, *sd, *searchpath, files[i]->d_name);
				fp = fopen(buf, "r");
				if (fp == NULL) {
					printf("Failed to open %s\n", buf);
					continue;
				}
				// printf("Load shader %s\n", buf);
				fseek(fp, 0, SEEK_END);
				file_size = ftell(fp);
				fseek(fp, 0, SEEK_SET);

				buffer = (char *)malloc(file_size + 2);
				fread(buffer, 1, file_size, fp);
				buffer[file_size] = '\n';
				buffer[file_size + 1] = '\0';
				fclose(fp);
				strncat(shaders, buffer, maxShaderSize - strlen(shaders) - 1);
				free(buffer);
				complete_filesize += file_size + 1;
				++shaderCount;
			}
			for (int i = 0; i < n; ++i) {
				free(files[i]);
			}
		}
	}
	if (complete_filesize > maxShaderSize) {
		printf("Fatal error: shader file size is %i bytes\n", complete_filesize);
		exit(1);
	}
	if (complete_filesize == 0) {
		printf("Fatal error: shader file size is 0 bytes\n");
		exit(1);
	}
	printf("Loaded %i shaders with %i bytes\n", shaderCount, complete_filesize);
	if (complete_filesize != strlen(shaders)) {
		printf("Fatal error: shader file size is %i bytes, but strlen(shaders) is %i\n", complete_filesize,
			   (int)strlen(shaders));
		exit(1);
	}
	return shaders;
}

static int validateBsp(const char *filename, const char *pk3dir, const void *buf, int mappacktype) {
	const dheader_t header = *(const dheader_t *)buf;
	printf("Validate bsp %s\n", filename);
	if (header.version != BSP_VERSION) {
		printf("%s: error invalid bsp version found: %i\n", filename, header.version);
		return 1;
	}

	char *allShaders = loadAllShaders(pk3dir, mappacktype);

	int errors = 0;
	{
		const lump_t *l = &header.lumps[LUMP_ENTITIES];
		char *entityString = (char *)malloc(l->filelen);
		Q_strncpyz(entityString, (const char *)((const unsigned char *)buf + l->fileofs), l->filelen);

		if (validateEntityString(filename, pk3dir, entityString, mappacktype) != 0) {
			++errors;
		}
		free(entityString);
	}
	{
		const lump_t *l = &header.lumps[LUMP_SHADERS];
		const dshader_t *shaders = (const dshader_t *)(const void *)((const unsigned char *)buf + l->fileofs);
		const int count = l->filelen / sizeof(*shaders);
		int i;

		if (l->filelen % sizeof(*shaders)) {
			printf("%s: error in bsp: shader lump size found\n", filename);
			++errors;
		} else {
			for (i = 0; i < count; ++i) {
				const char *shader = shaders[i].shader;
				if (validateShader(allShaders, shader, filename, pk3dir, mappacktype) != 0) {
					++errors;
				}
			}
		}
	}
	free(allShaders);
	return errors;
}

int main(int argc, char *argv[]) {
	const char *filename;
	const char *pk3dir;
	long bufsize;
	char *source;
	int mappacktype;
	FILE *fp;
	size_t newLen;
	int errorCount;

	if (argc != 4) {
		printf("Usage: bsptool <path-to-game-dir> <path-to.bsp> <mappacktype>\n");
		printf("mappacktype: 0 = wop, 1 = padpack, 2 = community\n");
		return 1;
	}

	pk3dir = argv[1];
	filename = argv[2];
	mappacktype = atoi(argv[3]);
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
	newLen = fread(source, 1, bufsize, fp);
	if (ferror(fp) != 0) {
		perror("Error: failed to read bsp file. ");
		return 1;
	}
	if (bufsize != newLen) {
		printf("Error: failed to read the complete bsp file.\n");
	}

	errorCount = validateBsp(filename, pk3dir, source, mappacktype);
	if (errorCount == 0) {
		free(source);
		return 0;
	}
	free(source);
	return 1;
}
