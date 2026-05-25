/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#if defined(WIN32) | defined(_WIN32)
#include <windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <glob.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "qbsp.h"

// file extensions with their type
typedef struct qfile_exttype_s {
	char *extension;
	int type;
} qfile_exttyp_t;

qfile_exttyp_t quakefiletypes[] = {{QFILEEXT_UNKNOWN, QFILETYPE_UNKNOWN},
								   {QFILEEXT_PK3, QFILETYPE_PK3},
								   {QFILEEXT_BSP, QFILETYPE_BSP},
								   {QFILEEXT_AAS, QFILETYPE_AAS},
								   {NULL, 0}};

static int QuakeFileExtensionType(char *extension) {
	int i;

	for (i = 0; quakefiletypes[i].extension; i++) {
		if (!stricmp(extension, quakefiletypes[i].extension)) {
			return quakefiletypes[i].type;
		}
	}
	return QFILETYPE_UNKNOWN;
}

int QuakeFileType(char *filename) {
	char ext[MAX_PATH] = ".";

	ExtractFileExtension(filename, ext + 1);
	return QuakeFileExtensionType(ext);
}

static char *StringContains(char *str1, char *str2, int casesensitive) {
	int len, i, j;

	len = strlen(str1) - strlen(str2);
	for (i = 0; i <= len; i++, str1++) {
		for (j = 0; str2[j]; j++) {
			if (casesensitive) {
				if (str1[j] != str2[j])
					break;
			} else {
				if (toupper(str1[j]) != toupper(str2[j]))
					break;
			}
		}
		if (!str2[j])
			return str1;
	}
	return NULL;
}

int FileFilter(char *filter, char *filename, int casesensitive) {
	char buf[1024];
	char *ptr;
	int i, found;

	while (*filter) {
		if (*filter == '*') {
			filter++;
			for (i = 0; *filter; i++) {
				if (*filter == '*' || *filter == '?')
					break;
				buf[i] = *filter;
				filter++;
			}
			buf[i] = '\0';
			if (strlen(buf)) {
				ptr = StringContains(filename, buf, casesensitive);
				if (!ptr)
					return qfalse;
				filename = ptr + strlen(buf);
			}
		} else if (*filter == '?') {
			filter++;
			filename++;
		} else if (*filter == '[' && *(filter + 1) == '[') {
			filter++;
		} else if (*filter == '[') {
			filter++;
			found = qfalse;
			while (*filter && !found) {
				if (*filter == ']' && *(filter + 1) != ']')
					break;
				if (*(filter + 1) == '-' && *(filter + 2) && (*(filter + 2) != ']' || *(filter + 3) == ']')) {
					if (casesensitive) {
						if (*filename >= *filter && *filename <= *(filter + 2))
							found = qtrue;
					} else {
						if (toupper(*filename) >= toupper(*filter) && toupper(*filename) <= toupper(*(filter + 2)))
							found = qtrue;
					}
					filter += 3;
				} else {
					if (casesensitive) {
						if (*filter == *filename)
							found = qtrue;
					} else {
						if (toupper(*filter) == toupper(*filename))
							found = qtrue;
					}
					filter++;
				}
			}
			if (!found)
				return qfalse;
			while (*filter) {
				if (*filter == ']' && *(filter + 1) != ']')
					break;
				filter++;
			}
			filter++;
			filename++;
		} else {
			if (casesensitive) {
				if (*filter != *filename)
					return qfalse;
			} else {
				if (toupper(*filter) != toupper(*filename))
					return qfalse;
			}
			filter++;
			filename++;
		}
	}
	return qtrue;
}

static quakefile_t *FindQuakeFilesInZip(char *zipfile, char *filter) {
	unzFile uf;
	int err;
	unz_global_info gi;
	char filename_inzip[MAX_PATH];
	unz_file_info file_info;
	int i;
	quakefile_t *qfiles, *lastqf, *qf;

	uf = unzOpen(zipfile);
	err = unzGetGlobalInfo(uf, &gi);

	if (err != UNZ_OK)
		return NULL;

	unzGoToFirstFile(uf);

	qfiles = NULL;
	lastqf = NULL;
	for (i = 0; i < (int)gi.number_entry; i++) {
		err = unzGetCurrentFileInfo(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
		if (err != UNZ_OK)
			break;

		ConvertPath(filename_inzip);
		if (FileFilter(filter, filename_inzip, qfalse)) {
			qf = malloc(sizeof(quakefile_t));
			if (!qf)
				Error("out of memory");
			memset(qf, 0, sizeof(quakefile_t));
			strcpy(qf->pakfile, zipfile);
			strcpy(qf->filename, zipfile);
			strcpy(qf->origname, filename_inzip);
			qf->zipfile = qtrue;
			unzGetFilePos(uf, &qf->filepos);
			qf->offset = 0;
			qf->length = file_info.uncompressed_size;
			qf->type = QuakeFileType(filename_inzip);
			// add the file to the list
			qf->next = NULL;
			if (lastqf)
				lastqf->next = qf;
			else
				qfiles = qf;
			lastqf = qf;
		}
		unzGoToNextFile(uf);
	}

	unzClose(uf);

	return qfiles;
}


static quakefile_t *FindQuakeFilesWithPakFilter(char *pakfilter, char *filter) {
#if defined(WIN32) | defined(_WIN32)
	WIN32_FIND_DATA filedata;
	HWND handle;
	struct _stat statbuf;
	int done;
#else
	glob_t globbuf;
	struct stat statbuf;
	int j;
#endif
	quakefile_t *qfiles, *lastqf, *qf;
	char pakfile[MAX_PATH], filename[MAX_PATH], *str;

	qfiles = NULL;
	lastqf = NULL;
	if (pakfilter && strlen(pakfilter)) {
#if defined(WIN32) | defined(_WIN32)
		handle = FindFirstFile(pakfilter, &filedata);
		done = (handle == INVALID_HANDLE_VALUE);
		while (!done) {
			_splitpath(pakfilter, pakfile, NULL, NULL, NULL);
			_splitpath(pakfilter, NULL, &pakfile[strlen(pakfile)], NULL, NULL);
			AppendPathSeperator(pakfile, MAX_PATH);
			strcat(pakfile, filedata.cFileName);
			_stat(pakfile, &statbuf);
#else
		glob(pakfilter, 0, NULL, &globbuf);
		for (j = 0; j < globbuf.gl_pathc; j++) {
			strcpy(pakfile, globbuf.gl_pathv[j]);
			stat(pakfile, &statbuf);
#endif
			// if the file with .pk3 is a folder
			if (statbuf.st_mode & S_IFDIR) {
				strcpy(filename, pakfilter);
				AppendPathSeperator(filename, MAX_PATH);
				strcat(filename, filter);
				qf = FindQuakeFilesWithPakFilter(NULL, filename);
				if (lastqf)
					lastqf->next = qf;
				else
					qfiles = qf;
				lastqf = qf;
				while (lastqf->next)
					lastqf = lastqf->next;
			} else {
				qf = FindQuakeFilesInZip(pakfile, filter);
				if (qf) {
					if (lastqf)
						lastqf->next = qf;
					else
						qfiles = qf;
					lastqf = qf;
					while (lastqf->next)
						lastqf = lastqf->next;
				}
			}
			//
#if defined(WIN32) | defined(_WIN32)
			// find the next file
			done = !FindNextFile(handle, &filedata);
		}
#else
		}
		globfree(&globbuf);
#endif
	} else {
#if defined(WIN32) | defined(_WIN32)
		handle = FindFirstFile(filter, &filedata);
		done = (handle == INVALID_HANDLE_VALUE);
		while (!done) {
			_splitpath(filter, filename, NULL, NULL, NULL);
			_splitpath(filter, NULL, &filename[strlen(filename)], NULL, NULL);
			AppendPathSeperator(filename, MAX_PATH);
			strcat(filename, filedata.cFileName);
#else
		glob(filter, 0, NULL, &globbuf);
		for (j = 0; j < globbuf.gl_pathc; j++) {
			strcpy(filename, globbuf.gl_pathv[j]);
#endif
			//
			qf = malloc(sizeof(quakefile_t));
			if (!qf)
				Error("out of memory");
			memset(qf, 0, sizeof(quakefile_t));
			strcpy(qf->pakfile, "");
			strcpy(qf->filename, filename);
			strcpy(qf->origname, filename);
			qf->offset = 0;
			qf->length = 0;
			qf->type = QuakeFileType(filename);
			// add the file ot the list
			qf->next = NULL;
			if (lastqf)
				lastqf->next = qf;
			else
				qfiles = qf;
			lastqf = qf;
#if defined(WIN32) | defined(_WIN32)
			// find the next file
			done = !FindNextFile(handle, &filedata);
		}
#else
		}
		globfree(&globbuf);
#endif
	}
	return qfiles;
}

quakefile_t *FindQuakeFiles(char *filter) {
	char *str;
	char newfilter[MAX_PATH];
	char pakfilter[MAX_PATH];
	char filefilter[MAX_PATH];

	strcpy(newfilter, filter);
	ConvertPath(newfilter);
	strcpy(pakfilter, newfilter);

	str = StringContains(pakfilter, ".pk3", qfalse);

	// ensure .pk3 is followed by a path separator (not e.g. ".pk3dir")
	if (str) {
		char *end = str + strlen(".pk3");
		if (*end && *end != '/' && *end != '\\')
			str = NULL;
	}

	if (str) {
		str += strlen(".pk3");
		if (*str) {
			*str++ = '\0';
			while (*str == '\\' || *str == '/')
				str++;
			strcpy(filefilter, str);
			return FindQuakeFilesWithPakFilter(pakfilter, filefilter);
		}
	}
	return FindQuakeFilesWithPakFilter(NULL, newfilter);
}

int LoadQuakeFile(quakefile_t *qf, void **bufferptr) {
	FILE *fp;
	void *buffer;
	int length;
	unzFile zf;

	if (qf->zipfile) {
		// open the zip file
		zf = unzOpen(qf->pakfile);
		// seek to the saved file position
		unzGoToFilePos(zf, &qf->filepos);
		// open the Quake file in the zip file
		unzOpenCurrentFile(zf);
		// allocate memory for the buffer
		length = qf->length;
		buffer = GetMemory(length + 1);
		// read the Quake file from the zip file
		length = unzReadCurrentFile(zf, buffer, length);
		// close the Quake file in the zip file
		unzCloseCurrentFile(zf);
		// close the zip file
		unzClose(zf);

		*bufferptr = buffer;
		return length;
	} else {
		fp = SafeOpenRead(qf->filename);
		if (qf->offset)
			fseek(fp, qf->offset, SEEK_SET);
		length = qf->length;
		if (!length)
			length = Q_filelength(fp);
		buffer = GetMemory(length + 1);
		((char *)buffer)[length] = 0;
		SafeRead(fp, buffer, length);
		fclose(fp);

		*bufferptr = buffer;
		return length;
	}
}

int ReadQuakeFile(quakefile_t *qf, void *buffer, int offset, int length) {
	FILE *fp;
	int read;
	unzFile zf;
	char tmpbuf[1024];

	if (qf->zipfile) {
		// open the zip file
		zf = unzOpen(qf->pakfile);
		// seek to the saved file position
		unzGoToFilePos(zf, &qf->filepos);
		// open the Quake file in the zip file
		unzOpenCurrentFile(zf);
		//
		while (offset > 0) {
			read = offset;
			if (read > (int)sizeof(tmpbuf))
				read = sizeof(tmpbuf);
			unzReadCurrentFile(zf, tmpbuf, read);
			offset -= read;
		}
		// read the Quake file from the zip file
		length = unzReadCurrentFile(zf, buffer, length);
		// close the Quake file in the zip file
		unzCloseCurrentFile(zf);
		// close the zip file
		unzClose(zf);

		return length;
	} else {
		fp = SafeOpenRead(qf->filename);
		if (qf->offset)
			fseek(fp, qf->offset, SEEK_SET);
		if (offset)
			fseek(fp, offset, SEEK_CUR);
		SafeRead(fp, buffer, length);
		fclose(fp);

		return length;
	}
}
