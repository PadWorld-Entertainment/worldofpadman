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
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "sv_http.h"
#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include <string.h>

#ifdef USE_LOCAL_HEADERS
#include "../curl-7.54.0/include/curl/curl.h"
#else
#include <curl/curl.h>
#endif /* USE_LOCAL_HEADERS */

#ifdef USE_CURL_DLOPEN
#include "../sys/sys_loadlib.h"
cvar_t *sv_cURLLib;
static void *cURLLib = NULL;

#define DEFAULT_CURL_LIB "libcurl.so.4"
#define ALTERNATE_CURL_LIB "libcurl.so.3"

static CURL *(*qcurl_easy_init)(void);
static CURLcode (*qcurl_easy_setopt)(CURL *curl, CURLoption option, ...);
static CURLcode (*qcurl_easy_perform)(CURL *curl);
static void (*qcurl_easy_cleanup)(CURL *curl);
static CURLcode (*qcurl_easy_getinfo)(CURL *curl, CURLINFO info, ...);
static void (*qcurl_easy_reset)(CURL *curl);
static const char *(*qcurl_easy_strerror)(CURLcode);
static void (*qcurl_slist_free_all)(struct curl_slist *list);
static struct curl_slist *(*qcurl_slist_append)(struct curl_slist *list, const char *data);

/*
=================
GPA
=================
*/
static void *GPA(const char *str) {
	void *rv = Sys_LoadFunction(cURLLib, str);
	if (!rv) {
		Com_Printf("Can't load symbol %s\n", str);
		return NULL;
	}
	Com_DPrintf("Loaded symbol %s (0x%p)\n", str, rv);
	return rv;
}

#else /* USE_CURL_DLOPEN */
#define qcurl_easy_init curl_easy_init
#define qcurl_easy_setopt curl_easy_setopt
#define qcurl_easy_perform curl_easy_perform
#define qcurl_easy_cleanup curl_easy_cleanup
#define qcurl_easy_getinfo curl_easy_getinfo
#define qcurl_easy_reset curl_easy_reset
#define qcurl_easy_strerror curl_easy_strerror
#define qcurl_slist_free_all curl_slist_free_all
#define qcurl_slist_append curl_slist_append
#endif /* USE_CURL_DLOPEN */

static size_t HTTP_WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	void (*writeCallback)(unsigned char *, int) = (void (*)(unsigned char *, int))userp;
	writeCallback((unsigned char *)contents, (int)(size * nmemb));
	return size * nmemb;
}

int HTTP_Execute(const char *mode, const char *url, const char *headers, const char *body,
				 void (*writeCallback)(unsigned char *, int)) {
	struct curl_slist *headerList = NULL;
	CURL *curl = qcurl_easy_init();
	long statusCode = 0;
	char *hdrs;
	char *token;
	CURLcode res;

	if (curl == NULL) {
		Com_Printf("Failed to initialize curl\n");
		return -1;
	}

	hdrs = strdup(headers);
	/* TODO: not thread safe */
	token = strtok(hdrs, "\r\n");
	while (token != NULL) {
		headerList = qcurl_slist_append(headerList, token);
		token = strtok(NULL, "\r\n");
	}

	if (headerList) {
		qcurl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
	}
	if (body != NULL && body[0] != '\0') {
		qcurl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
	}
	qcurl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, mode);
	qcurl_easy_setopt(curl, CURLOPT_URL, url);
	qcurl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
	qcurl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
	qcurl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
	qcurl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HTTP_WriteCallback);
	qcurl_easy_setopt(curl, CURLOPT_WRITEDATA, writeCallback);
	qcurl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, httpConnectTimeoutSecond);
	qcurl_easy_setopt(curl, CURLOPT_TIMEOUT, httpTimeoutSecond);
	qcurl_easy_setopt(curl, CURLOPT_USERAGENT, GAMENAME_FOR_MASTER);
	res = qcurl_easy_perform(curl);
	if (res != CURLE_OK) {
		Com_Printf("Http request for '%s' failed with error %s\n", url, qcurl_easy_strerror(res));
		statusCode = -1;
	} else {
		res = qcurl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
		if (res != CURLE_OK) {
			Com_DPrintf("Failed to get status code for %s", url);
		} else {
			Com_DPrintf("Got status code %i for %s\n", (int)statusCode, url);
		}
	}
	qcurl_slist_free_all(headerList);
	qcurl_easy_cleanup(curl);
	free(hdrs);
	return (int)statusCode;
}

int HTTP_Init(void) {
#ifdef USE_CURL_DLOPEN
	if (cURLLib)
		return 0;

	sv_cURLLib = Cvar_Get("sv_cURLLib", DEFAULT_CURL_LIB, CVAR_ARCHIVE | CVAR_PROTECTED);

	Com_Printf("Loading \"%s\"...\n", sv_cURLLib->string);
	if (!(cURLLib = Sys_LoadLibrary(sv_cURLLib->string))) {
#ifdef ALTERNATE_CURL_LIB
		/* On some linux distributions there is no libcurl.so.3, but only libcurl.so.4. That one works too. */
		if (!(cURLLib = Sys_LoadLibrary(ALTERNATE_CURL_LIB)))
#endif /* ALTERNATE_CURL_LIB */
			return 1;
	}

	qcurl_easy_init = GPA("curl_easy_init");
	qcurl_easy_setopt = GPA("curl_easy_setopt");
	qcurl_easy_perform = GPA("curl_easy_perform");
	qcurl_easy_cleanup = GPA("curl_easy_cleanup");
	qcurl_easy_getinfo = GPA("curl_easy_getinfo");
	qcurl_easy_reset = GPA("curl_easy_reset");
	qcurl_easy_strerror = GPA("curl_easy_strerror");
	qcurl_slist_append = GPA("curl_slist_append");
	qcurl_slist_free_all = GPA("curl_slist_free_all");
#endif /* USE_CURL_DLOPEN */
	return 0;
}

void HTTP_Close(void) {
#ifdef USE_CURL_DLOPEN
	if (cURLLib) {
		Sys_UnloadLibrary(cURLLib);
		cURLLib = NULL;
	}
	qcurl_easy_init = NULL;
	qcurl_easy_setopt = NULL;
	qcurl_easy_perform = NULL;
	qcurl_easy_cleanup = NULL;
	qcurl_easy_getinfo = NULL;
	qcurl_easy_reset = NULL;
	qcurl_slist_append = NULL;
#endif /* USE_CURL_DLOPEN */
}
