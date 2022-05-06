project(ui)
set(SRCS
	ui_local.h
	ui_public.h
	ui_shared.h
	ui_main.c
	ui_addbots.c
	ui_atoms.c
	ui_callvote.c
	ui_confirm.c
	ui_connect.c
	ui_controls.c
	ui_credits.c
	ui_demos.c
	ui_display.c
	ui_effects.c
	ui_gameinfo.c
	ui_help.c
	ui_ingame.c
	ui_menu.c
	ui_mfield.c
	ui_mods.c
	ui_music.c
	ui_network.c
	ui_password.c
	ui_players.c
	ui_playersettings.c
	ui_preferences.c
	ui_qmenu.c
	ui_removebots.c
	ui_serverinfo.c
	ui_servers.c
	ui_setup.c
	ui_sound.c
	ui_specifyserver.c
	ui_startserver.c
	ui_team.c
	ui_teamorders.c
	ui_video.c
	ui_voicechat.c
	../game/bg_misc.c
	../game/bg_lib.c
	../cgame/wopc_advanced2d.c
	../qcommon/q_math.c
	../qcommon/q_shared.c
)
set(LIB_SRCS ui_syscalls.c)
set(QVM_SRCS ${SRCS} ui_syscalls.asm)
list(APPEND SRCS ${LIB_SRCS})
wop_add_library(TARGET ${PROJECT_NAME} GAME SRCS ${SRCS})
target_compile_definitions(${PROJECT_NAME} PRIVATE UI)
add_qvm(${PROJECT_NAME} ${QVM_SRCS})
