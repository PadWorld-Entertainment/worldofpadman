if (APPLE)
	set(GLSL_LANG_VALIDATOR ${CODE_DIR}/../tools/darwin/glslangValidator)
elseif(UNIX)
	set(GLSL_LANG_VALIDATOR ${CODE_DIR}/../tools/linux/glslangValidator)
else()
	set(GLSL_LANG_VALIDATOR ${CODE_DIR}/../tools/windows/glslangValidator.exe)
endif()

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

function(add_qvm TARGET)
	set(srcfiles ${ARGV})
	string(TOUPPER ${TARGET} UPPERTARGET)
	set(compileflags "-D${UPPERTARGET}" "-DVMS")
	set(QVM_SRCS)
	foreach(srcfile ${srcfiles})
		get_filename_component(ext ${srcfile} EXT)
		if ("${ext}" STREQUAL ".asm")
			list(APPEND QVM_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile})
		elseif ("${ext}" STREQUAL ".c")
			get_filename_component(basename ${srcfile} NAME_WE)
			set(q3asm_outfile "${CMAKE_CURRENT_BINARY_DIR}/${basename}.asm")
			add_custom_command(
				OUTPUT ${q3asm_outfile}
				COMMAND q3lcc
				ARGS ${compileflags} -o "${q3asm_outfile}" "${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}"
				DEPENDS q3lcc ${QVM_DEPS}
			)
			set_source_files_properties(${q3asm_outfile} PROPERTIES GENERATED TRUE)
			list(APPEND QVM_SRCS ${q3asm_outfile})
		endif()
	endforeach()

	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.qvm
		COMMAND q3asm
		ARGS -o ${TARGET}.qvm ${QVM_SRCS}
		DEPENDS q3asm ${QVM_SRCS}
	)
	add_custom_target(${TARGET}.qvm DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.qvm)
	add_dependencies(${TARGET} ${TARGET}.qvm)
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
				list(APPEND ASM_SRCS ../asm/vm_x86_64.asm)
		endif()
		list(APPEND ASM_SRCS ../asm/snapvector.asm ../asm/ftola.asm)
		foreach(_file ${ASM_SRCS})
			set_property(SOURCE ${_file} APPEND PROPERTY COMPILE_DEFINITIONS idx64=1)
			set_property(SOURCE ${_file} APPEND PROPERTY LANGUAGE ASM_MASM)
		endforeach()
		target_sources(${TARGET} PRIVATE ${ASM_SRCS})
	else()
		target_sources(${TARGET} PRIVATE ../asm/snapvector.c ../asm/ftola.c)
	endif()
endfunction()

add_subdirectory(tools)
if (BUILD_CLIENT)
	add_subdirectory(client)
endif()
if (BUILD_SERVER)
	add_subdirectory(server)
endif()

add_subdirectory(renderergl1)
if (BUILD_RENDERER_OPENGL2)
	add_subdirectory(renderergl2)
endif()
if (BUILD_RENDERER_VULKAN)
	add_subdirectory(renderer_vulkan)
endif()

add_subdirectory(cgame)
add_subdirectory(game)
add_subdirectory(ui)
