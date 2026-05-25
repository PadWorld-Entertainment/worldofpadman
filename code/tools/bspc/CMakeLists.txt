set(BSPC_SRCS
	aas_areamerging.c
	aas_cfg.c
	aas_create.c
	aas_edgemelting.c
	aas_facemerging.c
	aas_file.c
	aas_gsubdiv.c
	aas_map.c
	aas_prunenodes.c
	aas_store.c
	be_aas_bspc.c
	brushbsp.c
	bspc.c
	csg.c
	glfile.c
	l_bsp_ent.c
	l_bsp_q3.c
	l_cmd.c
	l_log.c
	l_math.c
	l_mem.c
	l_poly.c
	l_qfiles.c
	l_threads.c
	l_utils.c
	leakfile.c
	map.c
	map_q3.c
	nodraw.c
	portals.c
	rch_load.c
	textures.c
	tree.c
	yxml.c
)

set(BSPC_BOTLIB_SRCS
	${CODE_DIR}/botlib/be_aas_bspq3.c
	${CODE_DIR}/botlib/be_aas_cluster.c
	${CODE_DIR}/botlib/be_aas_move.c
	${CODE_DIR}/botlib/be_aas_optimize.c
	${CODE_DIR}/botlib/be_aas_reach.c
	${CODE_DIR}/botlib/be_aas_sample.c
	${CODE_DIR}/botlib/l_libvar.c
	${CODE_DIR}/botlib/l_precomp.c
	${CODE_DIR}/botlib/l_script.c
	${CODE_DIR}/botlib/l_struct.c
)

set(BSPC_QCOMMON_SRCS
	${CODE_DIR}/qcommon/cm_load.c
	${CODE_DIR}/qcommon/cm_patch.c
	${CODE_DIR}/qcommon/cm_test.c
	${CODE_DIR}/qcommon/cm_trace.c
	${CODE_DIR}/qcommon/md4.c
	${CODE_DIR}/qcommon/unzip.c
	${CODE_DIR}/qcommon/ioapi.c
)

wop_add_executable(TARGET bspc SRCS ${BSPC_SRCS} ${BSPC_BOTLIB_SRCS} ${BSPC_QCOMMON_SRCS})
target_compile_definitions(bspc PRIVATE BSPC)

target_include_directories(bspc PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CODE_DIR}/qcommon
	${CODE_DIR}
	${CODE_DIR}/botlib
)

if (NOT MSVC)
	target_compile_options(bspc PRIVATE -Wno-error=implicit-function-declaration)
endif()

find_package(Threads REQUIRED)
find_library(MATH_LIBRARY m)

target_link_libraries(bspc PRIVATE
	zlib
	Threads::Threads
	$<$<BOOL:${MATH_LIBRARY}>:${MATH_LIBRARY}>
)
