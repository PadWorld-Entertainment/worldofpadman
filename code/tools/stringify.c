#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef _MSC_VER
#include <stdlib.h>
#else
#include <libgen.h>
#endif

int main(int argc, char **argv) {
	FILE *ifp;
	FILE *ofp;
	char buffer[1024];
	char *base;
#ifndef _MSC_VER
	char *ext;
#endif
	char *inFile, *outFile;

	if (argc < 3) {
		printf("usage: stringify <input> <output>\n");
		return 1;
	}

	inFile = argv[1];
	outFile = argv[2];

	ifp = fopen(inFile, "r");
	if (!ifp) {
		printf("failed to open %s\n", inFile);
		return 2;
	}

	ofp = fopen(outFile, "w");
	if (!ofp) {
		printf("failed to open %s for writing\n", outFile);
		return 3;
	}

#ifdef _MSC_VER
	_splitpath_s(inFile, NULL, 0, NULL, 0, buffer, sizeof(buffer), NULL, 0);
	base = buffer;
#else
	base = basename(inFile);
	// Strip extension
	ext = strrchr(base, '.');
	if (ext) {
		*ext = '\0';
	}
#endif

	fprintf(ofp, "const char *fallbackShader_%s =\n", base);

	while (fgets(buffer, sizeof(buffer), ifp)) {
		// Strip trailing whitespace from line
		char *end = buffer + strlen(buffer) - 1;
		while (end >= buffer && isspace(*end))
			end--;

		end[1] = '\0';

		// Write line enquoted, with a newline
		fprintf(ofp, "\"%s\\n\"\n", buffer);
	}

	fprintf(ofp, ";\n");

	fclose(ifp);
	fclose(ofp);

	return 0;
}
