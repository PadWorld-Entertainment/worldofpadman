project(game)
set(SRCS
	g_main.c
	ai_chat.c
	ai_cmd.c
	ai_dmnet.c
	ai_dmq3.c
	ai_main.c
	ai_team.c
	ai_vcmd.c
	bg_misc.c
	bg_pmove.c
	bg_slidemove.c
	bg_lib.c
	g_active.c
	g_arenas.c
	g_bot.c
	g_client.c
	g_cmds.c
	g_combat.c
	g_ctlitems.c
	g_gameinfo.c
	g_items.c
	g_mem.c
	g_misc.c
	g_missile.c
	g_modifiers.c
	g_mover.c
	g_session.c
	g_spawn.c
	g_stations.c
	g_svcmds.c
	g_target.c
	g_team.c
	g_trigger.c
	g_utils.c
	g_weapon.c
	../qcommon/q_math.c
	../qcommon/q_shared.c
)
set(LIB_SRCS g_syscalls.c)
set(QVM_SRCS ${SRCS} g_syscalls.asm)
list(APPEND SRCS ${LIB_SRCS})
wop_add_library(TARGET ${PROJECT_NAME} GAME SRCS ${SRCS})
target_compile_definitions(${PROJECT_NAME} PRIVATE QAGAME)
add_qvm(${PROJECT_NAME} ${QVM_SRCS})
