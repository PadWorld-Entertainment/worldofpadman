if(NOT PS5)
  return()
endif()
list(REMOVE_ITEM SRCS
  ../sdl/sdl_input.c
  ../sdl/sdl_snd.c
  ../sys/sys_unix.c
  ../sys/con_tty.c
  ../sys/con_log.c
  ../sys/sys_main.c
  ../qcommon/vm_x86.c
  ../qcommon/vm_aarch64.c
  ../qcommon/vm_armv7l.c
  ../qcommon/vm_powerpc.c
  ../server/sv_discord.c
  ../server/sv_http.c
)
list(APPEND SRCS
  cl_assist.c
  ../ps5/ps5_sys.c
  ../ps5/ps5_shared.c
  ../ps5/ps5_input.c
  ../wiiu/wiiu_osk.c
  ../ps5/ps5_snd.c
  ../ps5/platform.c
  ../renderercommon/puff.c
)
set_source_files_properties(${CODE_DIR}/qcommon/net_ip.c PROPERTIES COMPILE_DEFINITIONS
  "if_nametoindex=ps5q3_if_nametoindex;getnameinfo=ps5q3_getnameinfo;gai_strerror=ps5q3_gai_strerror;ioctl=ps5q3_ioctl;getaddrinfo=ps5q3_getaddrinfo;freeaddrinfo=ps5q3_freeaddrinfo")
set_source_files_properties(${CODE_DIR}/qcommon/common.c PROPERTIES COMPILE_DEFINITIONS
  "umask=ps5q3_umask")

function(ps5_add_native_module MOD DEFINE INCDIR)
  set(MOD_SRCS ${ARGN})
  add_library(wop_${MOD}_objs OBJECT ${MOD_SRCS})
  target_compile_definitions(wop_${MOD}_objs PRIVATE ${DEFINE})
  target_compile_options(wop_${MOD}_objs PRIVATE -fvisibility=hidden)
  target_include_directories(wop_${MOD}_objs PRIVATE ${INCDIR} ${CODE_DIR}/game ${CODE_DIR}/qcommon ${CODE_DIR}/ps5 ${CODE_DIR}/sys)
  set(OUT_OBJ "${CMAKE_CURRENT_BINARY_DIR}/${MOD}_native.o")
  set(PART_OBJ "${CMAKE_CURRENT_BINARY_DIR}/native_tmp/${MOD}_partial.o")
  set(REN_OBJ "${CMAKE_CURRENT_BINARY_DIR}/native_tmp/${MOD}_renamed.o")
  set(LD_SCRIPT "${CODE_DIR}/ps5/ps5_${MOD}.ld")
  add_custom_command(
    OUTPUT ${OUT_OBJ}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/native_tmp"
    COMMAND "${PS5_PAYLOAD_SDK}/bin/prospero-lld" -r -T "${LD_SCRIPT}" $<TARGET_OBJECTS:wop_${MOD}_objs> -o "${PART_OBJ}"
    COMMAND "${PS5_PAYLOAD_SDK}/bin/llvm-objcopy" --redefine-sym dllEntry=${MOD}_dllEntry --redefine-sym vmMain=${MOD}_vmMain "${PART_OBJ}" "${REN_OBJ}"
    COMMAND "${PS5_PAYLOAD_SDK}/bin/llvm-objcopy" -G ${MOD}_dllEntry -G ${MOD}_vmMain -G __${MOD}_data_start -G __${MOD}_data_end -G __${MOD}_bss_start -G __${MOD}_bss_end "${REN_OBJ}" "${OUT_OBJ}"
    DEPENDS wop_${MOD}_objs ${MOD_SRCS} "${LD_SCRIPT}"
    COMMAND_EXPAND_LISTS
    COMMENT "Building native PS5 ${MOD} module"
  )
  set(PS5_${MOD}_NATIVE_OBJ "${OUT_OBJ}" PARENT_SCOPE)
endfunction()

file(GLOB PS5_QAGAME_SRCS "${CODE_DIR}/game/*.c")
list(APPEND PS5_QAGAME_SRCS "${CODE_DIR}/qcommon/q_math.c" "${CODE_DIR}/qcommon/q_shared.c")
ps5_add_native_module(qagame QAGAME "${CODE_DIR}/game" ${PS5_QAGAME_SRCS})

file(GLOB PS5_CGAME_SRCS "${CODE_DIR}/cgame/*.c")
list(APPEND PS5_CGAME_SRCS
  "${CODE_DIR}/game/bg_misc.c"
  "${CODE_DIR}/game/bg_pmove.c"
  "${CODE_DIR}/game/bg_slidemove.c"
  "${CODE_DIR}/game/bg_lib.c"
  "${CODE_DIR}/qcommon/q_math.c"
  "${CODE_DIR}/qcommon/q_shared.c"
)
ps5_add_native_module(cgame CGAME "${CODE_DIR}/cgame" ${PS5_CGAME_SRCS})

file(GLOB PS5_UI_SRCS "${CODE_DIR}/ui/*.c")
list(APPEND PS5_UI_SRCS
  "${CODE_DIR}/game/bg_misc.c"
  "${CODE_DIR}/game/bg_lib.c"
  "${CODE_DIR}/qcommon/q_math.c"
  "${CODE_DIR}/qcommon/q_shared.c"
)
ps5_add_native_module(ui UI "${CODE_DIR}/ui" ${PS5_UI_SRCS})

list(APPEND SRCS ${PS5_qagame_NATIVE_OBJ} ${PS5_cgame_NATIVE_OBJ} ${PS5_ui_NATIVE_OBJ})

add_custom_command(
  OUTPUT "${CMAKE_BINARY_DIR}/code/renderer_vulkan/bintoc"
  COMMAND /usr/bin/cc -O2 "${CODE_DIR}/tools/bintoc.c" -o "${CMAKE_BINARY_DIR}/code/renderer_vulkan/bintoc"
  DEPENDS "${CODE_DIR}/tools/bintoc.c"
  COMMENT "Building host bintoc tool for Vulkan SPIR-V shaders"
)
add_custom_target(bintoc DEPENDS "${CMAKE_BINARY_DIR}/code/renderer_vulkan/bintoc")
