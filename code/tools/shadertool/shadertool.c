#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_TOKEN_CHARS 1024

static int com_tokenline = 0;
static int com_lines;
static char com_token[MAX_TOKEN_CHARS];

static const char *SkipWhitespace(const char *data) {
	char c;

	while ((c = *data) <= ' ') {
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

		// skip double slash comments
		if (c == '/' && data[1] == '/') {
			data += 2;
			while (*data && *data != '\n') {
				data++;
			}
		}
		// skip /* */ comments
		else if (c == '/' && data[1] == '*') {
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

static int validateTexture(const char *shaderfilename, const char *pk3dir, const char *filename) {
	FILE *fp;
	char buf[1024];
	char basename[1024];
	int len;
	static const char *ext[] = {"jpg", "png", "tga", NULL};
	static const char *searchpaths[] = {"gfx.pk3dir", "maps.pk3dir", "menu.pk3dir", "models.pk3dir", "textures.pk3dir", NULL};

	if (filename[0] == '$' || filename[0] == '*') {
		return 0;
	}

	Q_strncpyz(basename, filename, sizeof(basename));
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
			snprintf(buf, sizeof(buf), "%s/%s/%s.%s", pk3dir, *searchpath, basename, *e);
			if ((fp = fopen(buf, "r")) != NULL) {
				fclose(fp);
				return 0;
			}
		}
	}

	printf("%s: error %s not found\n", shaderfilename, filename);
	return 1;
}

static int validateShader(const char *filename, const char *pk3dir, const char *buf) {
	int error = 0;
	printf("Validate shader %s\n", filename);
	int depth = 0;
	for (;;) {
		char shaderName[64] = {0};

		const char *token = COM_ParseExt(&buf);
		if (!token[0]) {
			return 1;
		}

		Q_strncpyz(shaderName, token, sizeof(shaderName));

		token = COM_ParseExt(&buf);
		if (token[0] != '{' || token[1] != '\0') {
			printf("WARNING: missing opening brace\n");
			return 1;
		}

		depth = 1;
		for (;;) {
			token = COM_ParseExt(&buf);
			if (!token[0]) {
				break;
			}
			if (token[0] == '{') {
				depth++;
			} else if (token[0] == '}') {
				depth--;
			} else if (!strcmp(token, "map") || !strcmp(token, "qer_editorimage") ||
					   !strcmp(token, "q3map_lightimage") || !strcmp(token, "skyparms")) {
				token = COM_ParseExt(&buf);
				if (!token[0]) {
					return 1;
				}
				if (validateTexture(filename, pk3dir, token) != 0) {
					error = 1;
				}
			}
			// TODO animMap
			// TODO skyparms _ft _bk _rt _lf _up _dn

			if (depth == 0) {
				break;
			}
		}
	}
	if (depth != 0) {
		printf("%s: error unmatched brackets\n", filename);
		error = 1;
	}
	return error;
}

int main(int argc, char *argv[]) {
	const char *filename;
	const char *pk3dir;
	long bufsize;
	char *source;
	FILE *fp;

	if (argc != 3) {
		printf("Usage: shadertool <path-to-textures-pk3dir> <path-to.shader>\n");
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
		perror("Error: failed to read shader file. ");
		return 1;
	}

	source[newLen++] = '\0';

	if (validateShader(filename, pk3dir, source) == 0) {
		free(source);
		return 0;
	}
	free(source);
	return 1;
}
