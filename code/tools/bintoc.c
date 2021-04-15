#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
	FILE *ifp;
	FILE *ofp;
	const char *inFile;
	const char *outFile;
	char name[64];
	char *sep;

	if (argc != 3)
		return 0;

	inFile = argv[1];
	outFile = argv[2];
	sep = strrchr(outFile, '/');
	if (sep == NULL) {
		strncpy(name, outFile, sizeof(name));
	} else {
		strncpy(name, sep + 1, sizeof(name));
	}
	name[sizeof(name) - 1] = '\0';
	sep = strrchr(name, '.');
	if (sep != NULL) {
		*sep = '\0';
	}

	ifp = fopen(inFile, "r");
	if (!ifp)
		return 2;

	ofp = fopen(outFile, "w");
	if (!ofp)
		return 3;

	fprintf(ofp, "unsigned char %s_spv[] = {\n", name);
	unsigned long n = 0;

	while (!feof(ifp)) {
		unsigned char c;
		if (fread(&c, 1, 1, ifp) == 0)
			break;
		fprintf(ofp, "0x%.2X, ", (int)c);
		++n;
		if (n % 10 == 0)
			fprintf(ofp, "\n");
	}

	fprintf(ofp, "};\n");
	fprintf(ofp, "int %s_spv_size = %ld;\n", name, n);

	fclose(ifp);
	fclose(ofp);
	return 0;
}
