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

#define WIN32_LEAN_AND_MEAN (1)
#include <windows.h>
#include <winhttp.h>

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

int HTTP_Execute(const char *mode, const char *url, const char *headers, const char *body,
				 void (*writeCallback)(unsigned char *, int)) {
	const wchar_t *method = strcmp(mode, "GET") == 0 ? L"GET" : L"POST";
	URL_COMPONENTS url_components;
	wchar_t scheme[32];
	wchar_t hostname[128];
	wchar_t url_path[4096];
	wchar_t urlw[4096];

	/* Set the connection timeout */
	DWORD dwResolveTimeout = httpTimeoutSecond * 1000;
	DWORD dwConnectTimeout = httpConnectTimeoutSecond * 1000;
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
}

int HTTP_Init(void) {
	return 0;
}

void HTTP_Close(void) {
}
