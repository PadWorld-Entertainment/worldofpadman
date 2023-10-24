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

static int connectTimeoutSecond = 5;
static int timeoutSecond = 5;

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN (1)
#include <windows.h>
#include <winhttp.h>
#elif USE_CURL

#ifdef USE_LOCAL_HEADERS
#include "../curl-7.54.0/include/curl/curl.h"
#else
#include <curl/curl.h>
#endif

#ifdef USE_CURL_DLOPEN
#include "../sys/sys_loadlib.h"
cvar_t *sv_cURLLib;
static void *cURLLib = NULL;

#ifdef WIN32
#define DEFAULT_CURL_LIB "libcurl-4.dll"
#define ALTERNATE_CURL_LIB "libcurl-3.dll"
#elif defined(__APPLE__)
#define DEFAULT_CURL_LIB "libcurl.dylib"
#else
#define DEFAULT_CURL_LIB "libcurl.so.4"
#define ALTERNATE_CURL_LIB "libcurl.so.3"
#endif

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
#endif /* USE_CURL */

#ifdef WIN32
static void printLastError(const char *ctx) {
	DWORD errnum = GetLastError();
	char buffer[512] = "";
	if (FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
						 GetModuleHandleA("winhttp.dll"), errnum, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer,
						 sizeof(buffer), NULL) == 0) {
		Com_Printf("%s: %d - Unknown error\n", ctx, errnum);
	} else {
		Com_Printf("%s: %d - %s\n", ctx, errnum, buffer);
	}
}

static qboolean s2ws(const char *str, wchar_t *wstr, int buf_size) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	if (buf_size < size_needed) {
		Com_Printf("Failed to convert %s to wchar\n", str);
		return qfalse;
	}
	MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, size_needed);
	return qtrue;
}
#elif USE_CURL
static size_t HTTP_WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	void (*writeCallback)(unsigned char *, int) = (void (*)(unsigned char *, int))userp;
	writeCallback((unsigned char *)contents, (int)(size * nmemb));
	return size * nmemb;
}
#endif

void HTTP_SetTimeouts(int connectTimeoutS, int readWriteTimeoutS) {
	connectTimeoutSecond = connectTimeoutS;
	timeoutSecond = readWriteTimeoutS;
}

static int HTTP_Execute(const char *mode, const char *url, const char *headers, const char *body,
						void (*writeCallback)(unsigned char *, int)) {
#if WIN32
	const wchar_t *method = strcmp(mode, "GET") == 0 ? L"GET" : L"POST";
	URL_COMPONENTS url_components;
	wchar_t scheme[32];
	wchar_t hostname[128];
	wchar_t url_path[4096];
	wchar_t urlw[4096];

	/* Set the connection timeout */
	DWORD dwResolveTimeout = timeoutSecond * 1000;
	DWORD dwConnectTimeout = connectTimeoutSecond * 1000;
	DWORD dwSendTimeout = dwResolveTimeout;
	DWORD dwReceiveTimeout = dwResolveTimeout;
	HINTERNET hConnection;
	HINTERNET hSession;
	HINTERNET hRequest;
	wchar_t reqHeaders[1024];
	SIZE_T len;
	int maxRedirects = 3;
	qboolean requestState = qfalse;
	DWORD dwStatusCode = 0;
	DWORD dwSize = sizeof(dwStatusCode);
	DWORD bytesRead;
	BYTE buffer[4096];

	if (!s2ws(headers, reqHeaders, 1024)) {
		Com_Printf("Failed to convert headers\n");
		return -1;
	}
	len = wcslen(reqHeaders);
	if (!s2ws(url, urlw, 4096)) {
		Com_Printf("Failed to convert url\n");
		return -1;
	}

	/* Initialize WinHTTP and create a session */
	hSession = WinHttpOpen(NULL, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (hSession == NULL) {
		printLastError("Failed to create session for http download");
		return -1;
	}

	if (!WinHttpSetTimeouts(hSession, dwResolveTimeout, dwConnectTimeout, dwSendTimeout, dwReceiveTimeout)) {
		printLastError("Failed to set http timeouts");
		WinHttpCloseHandle(hSession);
		return -1;
	}

	/* Convert the URL to its components. */
	url_components.dwStructSize = sizeof(url_components);
	url_components.lpszScheme = scheme;
	url_components.dwSchemeLength = 32;
	url_components.lpszHostName = hostname;
	url_components.dwHostNameLength = 128;
	url_components.lpszUrlPath = url_path;
	url_components.dwUrlPathLength = 4096;
	url_components.nPort = INTERNET_DEFAULT_HTTP_PORT;
	if (!WinHttpCrackUrl(urlw, 0, 0, &url_components)) {
		printLastError("Failed to parse url");
		WinHttpCloseHandle(hSession);
		return -1;
	}

	/* Create the HTTP connection. */
	hConnection = WinHttpConnect(hSession, url_components.lpszHostName, url_components.nPort, 0);
	if (hConnection == NULL) {
		printLastError("Failed to connect");
		WinHttpCloseHandle(hSession);
		return -1;
	}

	hRequest = WinHttpOpenRequest(hConnection, method, url_components.lpszUrlPath, NULL, WINHTTP_NO_REFERER,
								  WINHTTP_DEFAULT_ACCEPT_TYPES,
								  url_components.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
	if (hRequest == NULL) {
		printLastError("Failed to create request");
		WinHttpCloseHandle(hSession);
		WinHttpCloseHandle(hConnection);
		return -1;
	}

	/* add request headers */
	if (!WinHttpAddRequestHeaders(hRequest, reqHeaders, (DWORD)len,
								  WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE)) {
		Com_Printf("Failed to add request headers to url: %s\n", url);
	}

	/* Send the request */
	if (strcmp(mode, "GET") == 0) {
		while (!requestState) {
			requestState =
				WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
			if (!requestState && GetLastError() == ERROR_WINHTTP_RESEND_REQUEST) {
				if (maxRedirects <= 0) {
					break;
				}
				--maxRedirects;
				continue;
			}
			if (!requestState) {
				Com_Printf("Failed to send request with error %d\n", GetLastError());
				break;
			}
		}
	} else {
		wchar_t wbody[4096];
		size_t bodySize;
		if (!s2ws(body, wbody, 4096)) {
			Com_Printf("Error while converting the POST body\n");
			return -1;
		}

		bodySize = wcslen(wbody);
		while (!requestState) {
			requestState =
				WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, (LPVOID)wbody, bodySize, bodySize, 0);
			if (!requestState && GetLastError() == ERROR_WINHTTP_RESEND_REQUEST) {
				if (maxRedirects <= 0) {
					break;
				}
				--maxRedirects;
				continue;
			}
			if (!requestState) {
				Com_Printf("Failed to send request with error %d\n", GetLastError());
				break;
			}
		}
	}
	WinHttpReceiveResponse(hRequest, NULL);
	WinHttpQueryDataAvailable(hRequest, NULL);

	WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
						&dwStatusCode, &dwSize, WINHTTP_NO_HEADER_INDEX);
	if (dwStatusCode != HTTP_STATUS_OK) {
		Com_Printf("Failed to download url: %s with status code: %d\n", url, dwStatusCode);
	}

	/* Read and save the response data */
	while (WinHttpReadData(hRequest, buffer, sizeof(buffer), &bytesRead)) {
		/* Write the 'bytesRead' bytes from the buffer */
		if (bytesRead == 0) {
			break;
		}
		writeCallback(buffer, bytesRead);
	}
	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hSession);
	WinHttpCloseHandle(hConnection);
	Com_DPrintf("Got status code %i for %s\n", (int)dwStatusCode, url);
	return dwStatusCode;
#elif USE_CURL
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
	qcurl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connectTimeoutSecond);
	qcurl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSecond);
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
#endif
	return -1;
}

int HTTP_ExecutePOST(const char *url, const char *headers, const char *body,
					 void (*writeCallback)(unsigned char *, int)) {
	const int statusCode = HTTP_Execute("POST", url, headers, body, writeCallback);
	return statusCode;
}

int HTTP_ExecuteGET(const char *url, const char *headers, void (*writeCallback)(unsigned char *, int)) {
	const int statusCode = HTTP_Execute("GET", url, headers, NULL, writeCallback);
	return statusCode;
}

int HTTP_Init(void) {
#ifdef WIN32
	return 0;
#elif USE_CURL
#ifdef USE_CURL_DLOPEN
	if (cURLLib)
		return 0;

	sv_cURLLib = Cvar_Get("sv_cURLLib", DEFAULT_CURL_LIB, CVAR_ARCHIVE | CVAR_PROTECTED);

	Com_Printf("Loading \"%s\"...\n", sv_cURLLib->string);
	if (!(cURLLib = Sys_LoadLibrary(sv_cURLLib->string))) {
#ifdef ALTERNATE_CURL_LIB
		/* On some linux distributions there is no libcurl.so.3, but only libcurl.so.4. That one works too. */
		if (!(cURLLib = Sys_LoadLibrary(ALTERNATE_CURL_LIB)))
#endif
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
#endif /* USE_CURL */
}

void HTTP_Close(void) {
#ifdef WIN32
#elif USE_CURL
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
#endif /* USE_CURL */
}
