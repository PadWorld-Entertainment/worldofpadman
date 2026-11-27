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

add_custom_command(
  OUTPUT "${CMAKE_BINARY_DIR}/code/renderer_vulkan/bintoc"
  COMMAND /usr/bin/cc -O2 "${CODE_DIR}/tools/bintoc.c" -o "${CMAKE_BINARY_DIR}/code/renderer_vulkan/bintoc"
  DEPENDS "${CODE_DIR}/tools/bintoc.c"
  COMMENT "Building host bintoc tool for Vulkan SPIR-V shaders"
)
add_custom_target(bintoc DEPENDS "${CMAKE_BINARY_DIR}/code/renderer_vulkan/bintoc")
