cmake_minimum_required(VERSION 3.24)
project(WorldOfPadman VERSION 1.7.1 LANGUAGES C ASM)

include(CheckSymbolExists)
include(CheckCCompilerFlag)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CODE_DIR ${ROOT_DIR}/code)
set(LIBS_DIR ${ROOT_DIR}/libs)

set(GAME_NAME wop)
set(GAME_BINARY_DIR ${GAME_NAME})
set(ENGINE_BINARY_DIR .)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(DEFAULT_BASEDIR "" "")
option(BUILD_CLIENT "" ON)
option(BUILD_SERVER "" ON)
option(BUILD_TESTS "" ON)
option(BUILD_RENDERER_OPENGL2 "" ON)
option(BUILD_RENDERER_VULKAN "" OFF)

option(USE_INTERNAL_LIBS "Use bundled libraries" ON)
option(USE_INTERNAL_SDL "" ON)
option(USE_RENDERER_DLOPEN "" ON)
option(USE_OPENAL_DLOPEN "" ON)
option(USE_CURL_DLOPEN "" ON)
option(USE_VOIP "" ON)
option(USE_MUMBLE "" ON)

if (USE_INTERNAL_LIBS)
	set(USE_INTERNAL_SDL ON)
else()
	set(USE_INTERNAL_SDL OFF)
endif()

if (MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
	# 4244 conversion from 'float' to 'int', possible loss of data
	# 4305 truncation from 'double' to 'float'
	# 4820 padding
	# 5045 spectre instruction
	# 4668 unknown macro definition
	# 4061 explicit switch case enum mention
	# 4242 possible loss of data (convert int to short)
	# 4464 relative include path
	# 4619 warning id is not available
	# 4245 return signed/unsigned conflict
	# 4100 unreferenced formal parameter
	# 4255 invalid function prototype - missing void
	# 4389 comparison signed/unsigned
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4267 /wd4244 /wd4305 /wd4820 /wd5045 /wd4668 /wd4061 /wd4242 /wd4464 /wd4619 /wd4245 /wd4100 /wd4255 /wd4389")
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

enable_testing()
set(CTEST_TEST_TIMEOUT 1800)
string(TIMESTAMP COPYRIGHT_YEAR "%Y")

if (APPLE)
	set(CPACK_GENERATOR "Bundle")
	set(CPACK_BUNDLE_PLIST ${ROOT_DIR}/misc/osx/application.plist.in)
	set(CPACK_BUNDLE_NAME ${PROJECT_NAME})
	set(CPACK_BUNDLE_ICON ${ROOT_DIR}/misc/wop.icns)
	set(CMAKE_XCODE_GENERATE_SCHEME "YES")

	find_program(XCRUN_EXECUTABLE NAMES xcrun)
	if (XCRUN_EXECUTABLE)
		# execute xrun to find the development team id
		execute_process(COMMAND ${XCRUN_EXECUTABLE} security find-identity -v -p codesigning OUTPUT_VARIABLE DEVELOPMENT_TEAMS)
		# extract the team id from the output that starts with "Developer ID Application: "
		string(REGEX MATCH "Developer ID Application: [^)]+\\(([^)]+)\\)" DEVELOPMENT_TEAM_ID ${DEVELOPMENT_TEAMS})
		set(DEVELOPMENT_TEAM_ID ${CMAKE_MATCH_1})
		if (DEVELOPMENT_TEAM_ID)
			message(STATUS "Development Team ID: ${DEVELOPMENT_TEAM_ID}")
		else()
			message(WARNING "No Development Team ID found")
		endif()
	endif()

	# With xcode versions >= 14 it is no longer possible to build unsigned binaries
	# - is for signing locally
	if (DEVELOPMENT_TEAM_ID STREQUAL "")
		if(XCODE_VERSION VERSION_LESS 14)
			set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "" CACHE INTERNAL "")
		else()
			set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-" CACHE INTERNAL "")
		endif()
	else()
		set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Mac Developer" CACHE INTERNAL "")
		set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID})
	endif()
	# --deep is for signing 3d party libraries
	set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "--deep -o linker-signed --timestamp" CACHE INTERNAL "")
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)
endif()

include(CPack)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(NDEBUG)
else()
	add_compile_definitions(DEBUG)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)
	add_compile_definitions(MAC_OS_X_VERSION_MIN_REQUIRED=1070)
endif()

add_compile_definitions(PRODUCT_VERSION="${CMAKE_PROJECT_VERSION}")

function(check_compiler_flag flag)
	string(REGEX REPLACE "[-=+]" "_" _flag ${flag})
	string(TOUPPER ${_flag} _flagfinal)
	check_c_compiler_flag("${flag}" COMPILER_SUPPORTS_${_flagfinal})
	if (COMPILER_SUPPORTS_${_flagfinal})
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}" PARENT_SCOPE)
	endif()
endfunction()

add_custom_target(pk3)

set(ZIP_7z "a")
set(ZIP_zip "-r9q")
find_program(ZIP NAMES zip 7z HINTS "$ENV{ProgramFiles}/7-Zip")
if (ZIP)
	get_filename_component(zip_binary ${ZIP} NAME_WE)
	set(ZIP_ARGS ${ZIP_${zip_binary}})
endif()

function(shader_validation GAME SHADER MAPPACKTYPE)
	set(PK3DIRS scripts.pk3dir padpack.pk3dir mappack.pk3dir)
	foreach(PK3DIR ${PK3DIRS})
		set(SHADERPATH "${CMAKE_CURRENT_SOURCE_DIR}/${PK3DIR}/scripts/${SHADER}")
		if (EXISTS ${SHADERPATH})
			set(targetname "shader-${GAME}-${SHADER}")
			get_filename_component(targetname ${targetname} NAME_WLE)
			get_filename_component(filename ${SHADERPATH} NAME)
			add_test(NAME ${targetname}
				COMMAND $<TARGET_FILE:shadertool> "${CMAKE_CURRENT_SOURCE_DIR}" "${SHADERPATH}" "${MAPPACKTYPE}"
				WORKING_DIRECTORY "$<TARGET_FILE_DIR:shadertool>"
			)
		endif()
	endforeach()
endfunction()

function(bsp_validation GAME BSP MAPPACKTYPE)
	set(PK3DIRS maps.pk3dir padpack.pk3dir mappack.pk3dir)
	foreach(PK3DIR ${PK3DIRS})
		set(BSPPATH "${CMAKE_CURRENT_SOURCE_DIR}/${PK3DIR}/maps/${BSP}")
		if (EXISTS ${BSPPATH})
			set(targetname "bsp-${GAME}-${BSP}")
			get_filename_component(targetname ${targetname} NAME_WLE)
			get_filename_component(filename ${BSPPATH} NAME)
			add_test(NAME ${targetname}
				COMMAND $<TARGET_FILE:bsptool> "${CMAKE_CURRENT_SOURCE_DIR}" "${BSPPATH}" "${MAPPACKTYPE}"
				WORKING_DIRECTORY "$<TARGET_FILE_DIR:bsptool>"
			)
		endif()
	endforeach()
endfunction()

function(compress_pk3 TARGET)
	add_custom_target(pk3-${TARGET})
	foreach (PK3DIR ${ARGN})
		set(_dir ${PK3DIR}.pk3dir)
		set(_file ${PK3DIR}-${CMAKE_PROJECT_VERSION}.pk3)
		add_custom_command(
			OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${_file}"
			COMMAND "${ZIP}" ${ZIP_ARGS} "${CMAKE_CURRENT_BINARY_DIR}/${_file}" .
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${_dir}"
			COMMENT "Create pk3 for ${PK3DIR}"
			VERBATIM
		)
		add_custom_target(pk3-${TARGET}-${PK3DIR}
			DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${_file}"
			COMMENT "Generate pk3 for ${PK3DIR}"
		)
		add_dependencies(pk3-${TARGET} pk3-${TARGET}-${PK3DIR})
	endforeach()
	add_dependencies(pk3 pk3-${TARGET})
endfunction()

check_compiler_flag(-Wformat=2)
check_compiler_flag(-Wno-format-zero-length)
check_compiler_flag(-Wformat-security)
check_compiler_flag(-Wno-format-nonliteral)
check_compiler_flag(-Wno-format-truncation)
check_compiler_flag(-Wstrict-aliasing=2)
check_compiler_flag(-Wmissing-format-attribute)
check_compiler_flag(-Wdisabled-optimization)
check_compiler_flag(-Werror-implicit-function-declaration)

add_subdirectory(libs)
add_subdirectory(code)
add_subdirectory(wop)
add_subdirectory(xmas)
add_subdirectory(misc)
