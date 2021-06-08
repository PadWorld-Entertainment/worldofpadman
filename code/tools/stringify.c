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

	if (argc < 3)
		return 1;

	char *inFile = argv[1];
	char *outFile = argv[2];

	ifp = fopen(inFile, "r");
	if (!ifp)
		return 2;

	ofp = fopen(outFile, "w");
	if (!ofp)
		return 3;

#ifdef _MSC_VER
	_splitpath_s(inFile, NULL, 0, NULL, 0, buffer, sizeof(buffer), NULL, 0);
	base = buffer;
#else
	base = basename(inFile);
#endif
	// Strip extension
	*strrchr(base, '.') = '\0';

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
