project(cgame)
set(SRCS
	cg_main.c
	cg_catch.c
	cg_consolecmds.c
	cg_cutscene2d.c
	cg_draw.c
	cg_drawtools.c
	cg_effects.c
	cg_ents.c
	cg_event.c
	cg_freezetag.c
	cg_info.c
	cg_lensflare.c
	cg_localents.c
	cg_marks.c
	cg_misc.c
	cg_players.c
	cg_playerstate.c
	cg_predict.c
	cg_scoreboard.c
	cg_servercmds.c
	cg_snapshot.c
	cg_spraylogo.c
	cg_spriteparticles.c
	cg_view.c
	cg_weapons.c
	wopc_advanced2d.c

	../game/bg_misc.c
	../game/bg_pmove.c
	../game/bg_slidemove.c
	../game/bg_lib.c

	../qcommon/q_math.c
	../qcommon/q_shared.c
)
set(LIB_SRCS cg_syscalls.c)
set(QVM_SRCS ${SRCS} cg_syscalls.asm)
list(APPEND SRCS ${LIB_SRCS})
wop_add_library(TARGET ${PROJECT_NAME} GAME SRCS ${SRCS})
target_compile_definitions(${PROJECT_NAME} PRIVATE CGAME)
add_qvm(${PROJECT_NAME} ${QVM_SRCS})
