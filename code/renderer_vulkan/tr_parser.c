#include "ref_import.h"
#include "tr_local.h"

/*
============================================================================

						 PARSING

split those parsing functions from q_shared.c
I want the render part standalone, dont fuck up with game part.

============================================================================
*/

static char r_parsename[512];
static int r_lines;
static int r_tokenline;

void R_BeginParseSession(const char *name) {
	r_lines = 1;
	r_tokenline = 0;
	snprintf(r_parsename, sizeof(r_parsename), "%s", name);
}

int R_GetCurrentParseLine(void) {
	if (r_tokenline) {
		return r_tokenline;
	}

	return r_lines;
}

int R_Compress(char *data_p) {
	qboolean newline = qfalse;
	qboolean whitespace = qfalse;

	char *in = data_p;
	char *out = data_p;

	if (in) {
		int c;
		while ((c = *in) != 0) {
			// skip double slash comments
			if (c == '/' && in[1] == '/') {
				while (*in && *in != '\n') {
					in++;
				}
				// skip /* */ comments
			} else if (c == '/' && in[1] == '*') {
				while (*in && (*in != '*' || in[1] != '/'))
					in++;
				if (*in)
					in += 2;
				// record when we hit a newline
			} else if (c == '\n' || c == '\r') {
				newline = qtrue;
				in++;
				// record when we hit whitespace
			} else if ((c == ' ') || (c == '\t')) {
				whitespace = qtrue;
				in++;
				// an actual token
			} else {
				// if we have a pending newline, emit it (and it counts as whitespace)
				if (newline) {
					*out++ = '\n';
					newline = qfalse;
					whitespace = qfalse;
				}
				if (whitespace) {
					*out++ = ' ';
					whitespace = qfalse;
				}

				// copy quoted strings unmolested
				if (c == '"') {
					*out++ = c;
					in++;
					while (1) {
						c = *in;
						if (c && c != '"') {
							*out++ = c;
							in++;
						} else {
							break;
						}
					}
					if (c == '"') {
						*out++ = c;
						in++;
					}
				} else {
					*out = c;
					out++;
					in++;
				}
			}
		}

		*out = 0;
	}
	return out - data_p;
}
