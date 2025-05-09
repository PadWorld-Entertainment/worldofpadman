if (APPLE)
	set(GLSL_LANG_VALIDATOR ${CODE_DIR}/../tools/darwin/glslangValidator)
elseif(UNIX)
	set(GLSL_LANG_VALIDATOR ${CODE_DIR}/../tools/linux/glslangValidator)
else()
	set(GLSL_LANG_VALIDATOR ${CODE_DIR}/../tools/windows/glslangValidator.exe)
endif()

#check_compiler_flag(-Wall)
#check_compiler_flag(-Wextra)
#check_compiler_flag(-Wpedantic)
#check_compiler_flag(-Wshadow)
check_compiler_flag(-Wpointer-arith)
#check_compiler_flag(-Wcast-qual)
check_compiler_flag(-Wstrict-prototypes)
check_compiler_flag(-Wmissing-prototypes)
check_compiler_flag(-Wno-unused-parameter)
check_compiler_flag(-Wno-sign-compare)

add_custom_target(qvm)

set(QCOMMON_COLLISION_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/cm_load.c
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/cm_local.h
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/cm_patch.c
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/cm_patch.h
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/cm_polylib.c
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/cm_polylib.h
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/cm_public.h
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/cm_test.c
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/cm_trace.c
)
set(QCOMMON_VM_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/vm.c
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/vm_interpreted.c
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/vm_local.h
	${CMAKE_CURRENT_SOURCE_DIR}/qcommon/vm_x86.c
)

find_program(GDB_EXECUTABLE gdb)
find_program(LLDB_EXECUTABLE lldb)
if (GDB_EXECUTABLE)
	set(DEBUGGER ${GDB_EXECUTABLE} CACHE STRING "Which debugger should be used")
elseif (LLDB_EXECUTABLE)
	set(DEBUGGER ${LLDB_EXECUTABLE} CACHE STRING "Which debugger should be used")
else()
	set(DEBUGGER "unknown" CACHE STRING "Which debugger should be used")
	message(STATUS "No debugger (gdb or lldb) was found")
endif()
set_property(CACHE DEBUGGER PROPERTY STRINGS gdb lldb)

function(wop_add_debuggger TARGET)
	if (${DEBUGGER} MATCHES "gdb")
		add_custom_target(${TARGET}-debug)
		add_custom_command(TARGET ${TARGET}-debug
			COMMAND ${GDB_EXECUTABLE} -ex run --args $<TARGET_FILE:${TARGET}> +set vm_game 0 +set vm_cgame 0 +set vm_ui 0 +set sv_pure 0 +set ttycon 0 +set com_ansiColor 0 +set developer 1
			COMMENT "Starting debugger session for ${TARGET}"
			USES_TERMINAL
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			DEPENDS ${TARGET}
		)
	elseif (${DEBUGGER} MATCHES "lldb")
		add_custom_target(${TARGET}-debug)
		add_custom_command(TARGET ${TARGET}-debug
			COMMAND CG_CONTEXT_SHOW_BACKTRACE=1 ${LLDB_EXECUTABLE} -b -o run $<TARGET_FILE:${TARGET}> +set vm_game 0 +set vm_cgame 0 +set vm_ui 0 +set sv_pure 0 +set ttycon 0 +set com_ansiColor 0 +set developer 1
			COMMENT "Starting debugger session for ${TARGET}"
			USES_TERMINAL
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			DEPENDS ${TARGET}
		)
	endif()
endfunction()

function(wop_add_library)
	set(_OPTIONS_ARGS GAME)
	set(_ONE_VALUE_ARGS TARGET STATIC)
	set(_MULTI_VALUE_ARGS SRCS)

	cmake_parse_arguments(_LIB "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN} )

	set(ARCH "x86")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(ARCH "x86_64")
	endif()
	if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64|aarch64)$")
		set(ARCH "arm64")
	endif()
	if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ppc64le")
		set(ARCH "ppc64le")
	endif()

	set(OUTPUTNAME "${_LIB_TARGET}_${ARCH}")
	set(LIB_BINARY_DIR "${ENGINE_BINARY_DIR}")
	if (_LIB_GAME)
		set(LIB_BINARY_DIR "${GAME_BINARY_DIR}")
	endif()

	if (_LIB_STATIC)
		set(LINK_TYPE STATIC)
	else()
		set(LINK_TYPE SHARED)
	endif()

	add_library(${_LIB_TARGET} ${LINK_TYPE} ${_LIB_SRCS})
	set_target_properties(${_LIB_TARGET} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/${GAME_NAME})
	set_target_properties(${_LIB_TARGET} PROPERTIES PREFIX "")
	set_target_properties(${_LIB_TARGET} PROPERTIES OUTPUT_NAME "${OUTPUTNAME}")
	set_target_properties(${_LIB_TARGET} PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${LIB_BINARY_DIR}"
		LIBRARY_OUTPUT_DIRECTORY "${ROOT_DIR}/${LIB_BINARY_DIR}"
		RUNTIME_OUTPUT_DIRECTORY "${ROOT_DIR}/${LIB_BINARY_DIR}"
	)
	install(TARGETS ${_LIB_TARGET} DESTINATION "${LIB_BINARY_DIR}" COMPONENT wop)
	foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
		set_target_properties(${_LIB_TARGET} PROPERTIES
			ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${LIB_BINARY_DIR}"
			LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${ROOT_DIR}/${LIB_BINARY_DIR}"
			RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${ROOT_DIR}/${LIB_BINARY_DIR}"
		)
	endforeach()
endfunction()

function(wop_add_executable)
	set(_OPTIONS_ARGS WINDOWED NOINSTALL)
	set(_ONE_VALUE_ARGS TARGET)
	set(_MULTI_VALUE_ARGS SRCS)

	cmake_parse_arguments(_EXE "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN} )

	if (_EXE_WINDOWED)
		if (WIN32)
			add_executable(${_EXE_TARGET} WIN32 ${_EXE_SRCS})
			if (MSVC)
				set_target_properties(${_EXE_TARGET} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
			endif()
		elseif(APPLE)
			add_executable(${_EXE_TARGET} MACOSX_BUNDLE ${_EXE_SRCS})
		else()
			add_executable(${_EXE_TARGET} ${_EXE_SRCS})
		endif()
	else()
		add_executable(${_EXE_TARGET} ${_EXE_SRCS})
		if (WIN32)
			if (MSVC)
				set_target_properties(${_EXE_TARGET} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
			endif()
		endif()
	endif()

	set(ARCH "x86")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(ARCH "x86_64")
	endif()
	if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64|aarch64)$")
		set(ARCH "arm64")
	endif()
	if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ppc64le")
		set(ARCH "ppc64le")
	endif()

	set(OUTPUTNAME "${_EXE_TARGET}.${ARCH}")
	set_target_properties(${_EXE_TARGET} PROPERTIES OUTPUT_NAME "${OUTPUTNAME}")
	set_target_properties(${_EXE_TARGET} PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ENGINE_BINARY_DIR}"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ENGINE_BINARY_DIR}"
		RUNTIME_OUTPUT_DIRECTORY "${ROOT_DIR}/${ENGINE_BINARY_DIR}"
	)
	foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
		set_target_properties(${_EXE_TARGET} PROPERTIES
			ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${ENGINE_BINARY_DIR}"
			LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${ENGINE_BINARY_DIR}"
			RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${ROOT_DIR}/${ENGINE_BINARY_DIR}"
		)
	endforeach()

	if (EXISTS ${ROOT_DIR}/misc/linux/${_EXE_TARGET}.desktop.in)
		configure_file(${ROOT_DIR}/misc/linux/${_EXE_TARGET}.desktop.in ${_EXE_TARGET}.desktop @ONLY)
	endif()
	if (EXISTS ${ROOT_DIR}/misc/linux/${_EXE_TARGET}.service.in)
		configure_file(${ROOT_DIR}/misc/linux/${_EXE_TARGET}.service.in ${_EXE_TARGET}.service @ONLY)
	endif()

	if (_EXE_NOINSTALL)
		set(INSTALL_DATA False)
		set(CPACK_${_EXE_TARGET}_COMPONENT_INSTALL OFF)
	else()
		set(INSTALL_DATA True)
		set(CPACK_${_EXE_TARGET}_COMPONENT_INSTALL ON)
	endif()

	set(CFGS
		default.cfg
		rotation.cfg
		server-allgametypes.cfg
		server-bigballoon.cfg
		server-capturethelolly.cfg
		server-catchthekillerduck.cfg
		server-freezetag.cfg
		server-lastpadstanding.cfg
		server-onelollycapture.cfg
		server-settings.cfg
		server-sprayyourcolor.cfg
	)
	if (INSTALL_DATA)
		if (APPLE)
			set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${ROOT_DIR}/misc/osx/application.plist.in)
			set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE ON)
			set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER "${CMAKE_PROJECT_NAME}")
			set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
			set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
			set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}")
			set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_COPYRIGHT "Copyright 2004 - ${COPYRIGHT_YEAR} PadWorld Entertainment")
			#set_target_properties(${_EXE_TARGET} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "YES")
			#set_target_properties(${_EXE_TARGET} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
			#set_target_properties(${_EXE_TARGET} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ${CODESIGNIDENTITY})
			#set_target_properties(${_EXE_TARGET} PROPERTIES XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID})
			#set_target_properties(${_EXE_TARGET} PROPERTIES XCODE_ATTRIBUTE_PROVISIONING_PROFILE_SPECIFIER ${PROVISIONING_PROFILE_NAME})
			add_custom_command(TARGET ${_EXE_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -DTARGET_BUNDLE_DIR=$<TARGET_BUNDLE_DIR:${_EXE_TARGET}> -P "${ROOT_DIR}/cmake/applebundle.cmake" VERBATIM)
			if (${CMAKE_GENERATOR} STREQUAL "Xcode")
				set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_EXECUTABLE_NAME \${EXECUTABLE_NAME})
				set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_PRODUCT_NAME \${PRODUCT_NAME})
				set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME \${PRODUCT_NAME})
			else()
				set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_PRODUCT_NAME "${CMAKE_PROJECT_NAME}")
				set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "${CMAKE_PROJECT_NAME}")
			endif()
			set_target_properties(${_EXE_TARGET} PROPERTIES MACOSX_BUNDLE_ICON_FILE wop.icns)
			set_source_files_properties(${ROOT_DIR}/misc/wop.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
			foreach (_c ${CFGS})
				set_source_files_properties(${ROOT_DIR}/wop/${_c} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/wop")
				target_sources(${_EXE_TARGET} PRIVATE ${ROOT_DIR}/wop/${_c})
			endforeach()
			target_sources(${_EXE_TARGET} PRIVATE ${ROOT_DIR}/misc/wop.icns)

			install(CODE "include(BundleUtilities)
				fixup_bundle(\"$<TARGET_BUNDLE_DIR:${_EXE_TARGET}>\" \"\" \"\")" COMPONENT wop)
		endif()
		install(TARGETS ${_EXE_TARGET} DESTINATION "${ENGINE_BINARY_DIR}" COMPONENT wop)
	endif()

	if (INSTALL_DATA)
		cpack_add_component(${_EXE_TARGET} DISPLAY_NAME "${_EXE_TARGET}")
	endif()

	add_custom_target(${_EXE_TARGET}-run
		COMMAND $<TARGET_FILE:${_EXE_TARGET}> +set sv_pure 0
		USES_TERMINAL
		DEPENDS ${_EXE_TARGET}
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	)
	wop_add_debuggger(${_EXE_TARGET})
endfunction()

function(add_qvm)
	set(_OPTIONS_ARGS)
	set(_ONE_VALUE_ARGS TARGET)
	set(_MULTI_VALUE_ARGS SRCS DEFINES)

	cmake_parse_arguments(_QVM "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN} )

	string(TOUPPER ${_QVM_TARGET} UPPERTARGET)
	set(compileflags "-D${UPPERTARGET}" "-DVMS")
	foreach (_d ${_QVM_DEFINES})
		list(APPEND compileflags "-D${_d}")
	endforeach()
	set(QVM_SRCS)
	foreach(srcfile ${_QVM_SRCS})
		get_filename_component(ext ${srcfile} EXT)
		if ("${ext}" STREQUAL ".asm")
			list(APPEND QVM_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile})
		elseif ("${ext}" STREQUAL ".c")
			get_filename_component(basename ${srcfile} NAME_WE)
			set(q3asm_outfile "${CMAKE_CURRENT_BINARY_DIR}/${basename}.asm")
			add_custom_command(
				OUTPUT ${q3asm_outfile}
				COMMAND q3lcc
				ARGS ${compileflags} -o "\"${q3asm_outfile}\"" "\"${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}\""
				DEPENDS q3lcc ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}
			)
			set_source_files_properties(${q3asm_outfile} PROPERTIES GENERATED TRUE)
			list(APPEND QVM_SRCS ${q3asm_outfile})
		endif()
	endforeach()
	add_dependencies(qvm qvm_${_QVM_TARGET})

	set(QVM_PATH ${GAME_NAME}/vm/${_QVM_TARGET}.qvm)
	add_custom_command(
		OUTPUT "${ROOT_DIR}/${ENGINE_BINARY_DIR}/${QVM_PATH}"
		COMMAND q3asm
		ARGS -o "${ROOT_DIR}/${ENGINE_BINARY_DIR}/${QVM_PATH}" ${QVM_SRCS}
		DEPENDS q3asm ${QVM_SRCS}
	)
	add_custom_target(qvm_${_QVM_TARGET} DEPENDS "${ROOT_DIR}/${ENGINE_BINARY_DIR}/${QVM_PATH}")
	add_dependencies(${_QVM_TARGET} qvm_${_QVM_TARGET})
	set_source_files_properties(${QVM_PATH} PROPERTIES GENERATED TRUE)
	set_source_files_properties(${QVM_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/${QVM_PATH})
	install(FILES "${ROOT_DIR}/${ENGINE_BINARY_DIR}/${QVM_PATH}" DESTINATION "${GAME_NAME}" COMPONENT wop)
endfunction()

function(add_botlib TARGET)
	set(BOTLIB_SRCS
		${CODE_DIR}/botlib/be_aas_bspq3.c
		${CODE_DIR}/botlib/be_aas_cluster.c
		${CODE_DIR}/botlib/be_aas_debug.c
		${CODE_DIR}/botlib/be_aas_entity.c
		${CODE_DIR}/botlib/be_aas_file.c
		${CODE_DIR}/botlib/be_aas_main.c
		${CODE_DIR}/botlib/be_aas_move.c
		${CODE_DIR}/botlib/be_aas_optimize.c
		${CODE_DIR}/botlib/be_aas_reach.c
		${CODE_DIR}/botlib/be_aas_route.c
		${CODE_DIR}/botlib/be_aas_routealt.c
		${CODE_DIR}/botlib/be_aas_sample.c
		${CODE_DIR}/botlib/be_ai_char.c
		${CODE_DIR}/botlib/be_ai_chat.c
		${CODE_DIR}/botlib/be_ai_gen.c
		${CODE_DIR}/botlib/be_ai_goal.c
		${CODE_DIR}/botlib/be_ai_move.c
		${CODE_DIR}/botlib/be_ai_weap.c
		${CODE_DIR}/botlib/be_ai_weight.c
		${CODE_DIR}/botlib/be_ea.c
		${CODE_DIR}/botlib/be_interface.c
		${CODE_DIR}/botlib/l_crc.c
		${CODE_DIR}/botlib/l_libvar.c
		${CODE_DIR}/botlib/l_log.c
		${CODE_DIR}/botlib/l_memory.c
		${CODE_DIR}/botlib/l_precomp.c
		${CODE_DIR}/botlib/l_script.c
		${CODE_DIR}/botlib/l_struct.c
	)
	target_sources(${TARGET} PRIVATE ${BOTLIB_SRCS})
	#target_include_directories(${TARGET} PRIVATE ${CODE_DIR}/botlib)
	foreach(_file ${BOTLIB_SRCS})
		set_property(SOURCE ${_file} APPEND PROPERTY COMPILE_DEFINITIONS BOTLIB)
		set_property(SOURCE ${_file} APPEND PROPERTY INCLUDE_DIRECTORIES ${CODE_DIR}/botlib)
	endforeach()
endfunction()

function(add_asm TARGET)
	if (MSVC)
		enable_language(ASM_MASM)
		set(ASM_SRCS)
		if (CMAKE_SIZEOF_VOID_P EQUAL 8)
			list(APPEND ASM_SRCS ${CODE_DIR}/asm/vm_x86_64.asm)
		endif()
		list(APPEND ASM_SRCS ${CODE_DIR}/asm/snapvector.asm ${CODE_DIR}/asm/ftola.asm)
		foreach(_file ${ASM_SRCS})
			if (CMAKE_SIZEOF_VOID_P EQUAL 8)
				set_property(SOURCE ${_file} APPEND PROPERTY COMPILE_DEFINITIONS idx64=1)
			endif()
			set_property(SOURCE ${_file} APPEND PROPERTY LANGUAGE ASM_MASM)
			set_source_files_properties(${_file} PROPERTIES COMPILE_FLAGS "/safeseh")
		endforeach()
		target_sources(${TARGET} PRIVATE ${ASM_SRCS})
	elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm" OR "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64")
	elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ppc64" OR "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ppc64le")
	else()
		target_sources(${TARGET} PRIVATE ${CODE_DIR}/asm/snapvector.c ${CODE_DIR}/asm/ftola.c)
		set_source_files_properties(${CODE_DIR}/asm/snapvector.c PROPERTIES COMPILE_FLAGS -march=k8)
		set_source_files_properties(${CODE_DIR}/asm/ftola.c PROPERTIES COMPILE_FLAGS -march=k8)
	endif()
endfunction()

if (DEFAULT_BASEDIR)
	add_compile_definitions(DEFAULT_BASEDIR="${DEFAULT_BASEDIR}")
endif()

add_subdirectory(tools)

set(RENDERER_LIST renderer_opengl1)
add_subdirectory(renderergl1)
if (BUILD_RENDERER_OPENGL2)
	add_subdirectory(renderergl2)
	list(APPEND RENDERER_LIST renderer_opengl2)
endif()
if (BUILD_RENDERER_VULKAN)
	add_subdirectory(renderer_vulkan)
	list(APPEND RENDERER_LIST renderer_vulkan)
endif()

add_subdirectory(cgame)
add_subdirectory(game)
add_subdirectory(ui)

if (BUILD_CLIENT)
	add_subdirectory(client)
endif()
if (BUILD_SERVER)
	add_subdirectory(server)
endif()
if (BUILD_TESTS AND UNIX)
	add_subdirectory(tests)
endif()
