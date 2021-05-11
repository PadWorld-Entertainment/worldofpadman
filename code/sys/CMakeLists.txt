set(SRCS
	con_log.c
	con_passive.c
	con_tty.c
	con_win32.c
	sys_loadlib.h
	sys_local.h
	sys_main.cpp
	sys_unix.c
	sys_win32.c
	win_resource.h
)
add_library(sys STATIC ${SRCS})
