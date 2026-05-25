add_library(curl INTERFACE)

# Require a system-installed curl >= 7.86.0 to avoid the bundled 7.54.0 headers,
# which carry dozens of known CVEs (CVE-2017-8816, CVE-2017-8817, CVE-2017-8818, etc.).
find_package(CURL 7.86.0 REQUIRED)
target_compile_definitions(curl INTERFACE USE_CURL)
set(LIBS ${CURL_LIBRARIES})
set(INCLUDE_DIRS ${CURL_INCLUDE_DIRS})

if (NOT USE_CURL_DLOPEN OR MINGW)
	if (LIBS)
		target_link_libraries(curl INTERFACE ${LIBS})
	endif()
else()
	target_compile_definitions(curl INTERFACE USE_CURL_DLOPEN)
endif()
target_include_directories(curl INTERFACE ${INCLUDE_DIRS})
