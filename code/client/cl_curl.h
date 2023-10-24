/*
===========================================================================
Copyright (C) 2006 Tony J. White (tjw@tjw.org)

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

#ifndef __QCURL_H__
#define __QCURL_H__

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"

#ifdef USE_LOCAL_HEADERS
#include "../curl-7.54.0/include/curl/curl.h"
#else
#include <curl/curl.h>
#endif

#ifdef USE_CURL_DLOPEN
#ifdef WIN32
#define DEFAULT_CURL_LIB "libcurl-4.dll"
#define ALTERNATE_CURL_LIB "libcurl-3.dll"
#elif defined(__APPLE__)
#define DEFAULT_CURL_LIB "libcurl.dylib"
#else
#define DEFAULT_CURL_LIB "libcurl.so.4"
#define ALTERNATE_CURL_LIB "libcurl.so.3"
#endif

extern cvar_t *cl_cURLLib;
#endif

qboolean CL_cURL_Init(void);
void CL_cURL_Shutdown(void);
void CL_cURL_BeginDownload(const char *localName, const char *remoteURL);
void CL_cURL_PerformDownload(void);
void CL_cURL_Cleanup(void);
#endif // __QCURL_H__
