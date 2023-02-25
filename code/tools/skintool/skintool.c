#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_TOKEN_CHARS 1024

static void Q_strncpyz(char *dest, const char *src, int destsize) {
	strncpy(dest, src, destsize - 1);
	dest[destsize - 1] = 0;
}

static int validateTexture(const char *shaderName, const char *shaderfilename, const char *pk3dir,
						   const char *filename) {
	FILE *fp;
	char buf[1024];
	char basename[1024];
	int len;
	static const char *ext[] = {"jpg", "png", "tga", NULL};
	static const char *searchpaths[] = {"gfx.pk3dir",	 "maps.pk3dir",		"menu.pk3dir",
										"models.pk3dir", "textures.pk3dir", NULL};
	static const char *skyparms[] = {"", "_ft", "_bk", "_rt", "_lf", "_up", "_dn"};
	static const char *subdirs[] = {".", "../wop"};

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
			for (const char **s = skyparms; *s; ++s) {
				for (const char **sd = subdirs; *sd; ++sd) {
					snprintf(buf, sizeof(buf), "%s/%s/%s/%s%s.%s", pk3dir, *sd, *searchpath, basename, *s, *e);
					if ((fp = fopen(buf, "r")) != NULL) {
						fclose(fp);
						return 0;
					}
				}
			}
		}
	}

	printf("%s:%i: error in skin %s: %s not found\n", shaderfilename, 0, shaderName, filename);
	return 1;
}

/*
==================
CommaParse

This is unfortunate, but the skin files aren't
compatible with our normal parsing rules.
==================
*/
static char *CommaParse(const char **data_p) {
	int c, len;
	const char *data;
	static char com_token[MAX_TOKEN_CHARS];

	data = *data_p;
	len = 0;
	com_token[0] = 0;

	// make sure incoming data is valid
	if (!data) {
		*data_p = NULL;
		return com_token;
	}

	while (1) {
		// skip whitespace
		while ((c = *data) <= ' ') {
			if (!c) {
				break;
			}
			data++;
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
				data++;
			}
			if (*data) {
				data += 2;
			}
		} else {
			break;
		}
	}

	if (c == 0) {
		return "";
	}

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
	} while (c > 32 && c != ',');

	com_token[len] = 0;

	*data_p = (char *)data;
	return com_token;
}

static char *Q_strlwr(char *s1) {
	char *s;

	s = s1;
	while (*s) {
		*s = tolower(*s);
		s++;
	}
	return s1;
}

static int validateSkin(const char *filename, const char *pk3dir, const char *buf) {
	int error = 0;
	const char *text_p;
	const char *token;
	char surfName[64];

	printf("Validate skin %s\n", filename);

	text_p = buf;
	while (text_p && *text_p) {
		// get surface name
		token = CommaParse(&text_p);
		Q_strncpyz(surfName, token, sizeof(surfName));

		if (!token[0]) {
			break;
		}
		// lowercase the surface name so skin compares are faster
		Q_strlwr(surfName);

		if (*text_p == ',') {
			text_p++;
		}

		if (strstr(token, "tag_")) {
			continue;
		}

		// parse the skin name
		token = CommaParse(&text_p);
		if (validateTexture(surfName, filename, pk3dir, token) != 0) {
			error = 1;
		}
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
		printf("Usage: skintool <path-to-textures-pk3dir> <path-to.skin>\n");
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
		perror("Error: failed to get size of skin file. ");
		return 1;
	}

	if (fseek(fp, 0L, SEEK_SET) != 0) {
		perror("Error: failed to reset file stream. ");
		return 1;
	}

	source = (char *)malloc(bufsize + 1);
	size_t newLen = fread(source, 1, bufsize, fp);
	if (ferror(fp) != 0) {
		perror("Error: failed to read skin file. ");
		return 1;
	}

	source[newLen++] = '\0';

	if (validateSkin(filename, pk3dir, source) == 0) {
		free(source);
		return 0;
	}
	free(source);
	return 1;
}
