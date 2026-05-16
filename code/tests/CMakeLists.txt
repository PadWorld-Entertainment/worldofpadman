enable_testing()

project(woptest)

set(SRCS_BASE
	${CODE_DIR}/null/null_client.c
	${CODE_DIR}/null/null_input.c
	${CODE_DIR}/null/null_snddma.c
	${CODE_DIR}/null/null_server.c
	${CODE_DIR}/null/null_sys.c

	${CODE_DIR}/server/sv_http.c
	${CODE_DIR}/server/sv_discord.c

	${QCOMMON_COLLISION_SRCS}
	${CODE_DIR}/qcommon/cmd.c
	${CODE_DIR}/qcommon/common.c
	${CODE_DIR}/qcommon/cvar.c
	${CODE_DIR}/qcommon/files.c
	${CODE_DIR}/qcommon/huffman.c
	${CODE_DIR}/qcommon/ioapi.c
	${CODE_DIR}/qcommon/md4.c
	${CODE_DIR}/qcommon/msg.c
	${CODE_DIR}/qcommon/net_chan.c
	${CODE_DIR}/qcommon/net_ip.c
	${CODE_DIR}/qcommon/q_math.c
	${CODE_DIR}/qcommon/q_shared.c
	${CODE_DIR}/qcommon/unzip.c
	${QCOMMON_VM_SRCS}
	${CODE_DIR}/sys/con_log.c
)

if (WIN32)
	list(APPEND SRCS_BASE ${CODE_DIR}/sys/sys_win32.c)
	list(APPEND SRCS_BASE ${CODE_DIR}/sys/win_resource.rc)
else()
	if(APPLE)
		list(APPEND SRCS_BASE ${CODE_DIR}/sys/sys_osx.m)
	endif()
	list(APPEND SRCS_BASE ${CODE_DIR}/sys/sys_unix.c)
endif()

set(LIBS curl zlib SDL2::SDL2 SDL2::SDL2main ${CMAKE_DL_LIBS})
if (MSVC)
	list(APPEND LIBS ws2_32 winmm psapi)
elseif (APPLE)
	set(FRAMEWORKS Cocoa)
	foreach (_framework ${FRAMEWORKS})
		list(APPEND LIBS "-framework ${_framework}")
	endforeach()
	list(APPEND TEST_DEFINES _THREAD_SAFE=1)
else()
	set(CMAKE_REQUIRED_LIBRARIES m)
	check_symbol_exists("cosf" "math.h" HAVE_COSF)
	set(CMAKE_REQUIRED_LIBRARIES)
	list(APPEND LIBS m)
endif()

set(TEST_DEFINES TESTS)
if (USE_VOIP)
	list(APPEND TEST_DEFINES USE_VOIP)
endif()

set(SRCS_MAIN test_main.c ${SRCS_BASE})
wop_add_executable(TARGET ${PROJECT_NAME}-main SRCS ${SRCS_MAIN} NOINSTALL)
add_asm(${PROJECT_NAME}-main)
add_botlib(${PROJECT_NAME}-main)
target_link_libraries(${PROJECT_NAME}-main ${LIBS})
target_include_directories(${PROJECT_NAME}-main PRIVATE ${CODE_DIR}/qcommon)
target_compile_definitions(${PROJECT_NAME}-main PRIVATE ${TEST_DEFINES})
add_test(NAME ${PROJECT_NAME}-main COMMAND $<TARGET_FILE:${PROJECT_NAME}-main>)
set_tests_properties(${PROJECT_NAME}-main PROPERTIES WORKING_DIRECTORY ${ROOT_DIR})

set(SRCS_UI
	test_ui.c

	${CODE_DIR}/ui/ui_main.c
	${CODE_DIR}/ui/ui_addbots.c
	${CODE_DIR}/ui/ui_atoms.c
	${CODE_DIR}/ui/ui_callvote.c
	${CODE_DIR}/ui/ui_confirm.c
	${CODE_DIR}/ui/ui_connect.c
	${CODE_DIR}/ui/ui_controls.c
	${CODE_DIR}/ui/ui_credits.c
	${CODE_DIR}/ui/ui_demos.c
	${CODE_DIR}/ui/ui_display.c
	${CODE_DIR}/ui/ui_effects.c
	${CODE_DIR}/ui/ui_gameinfo.c
	${CODE_DIR}/ui/ui_help.c
	${CODE_DIR}/ui/ui_ingame.c
	${CODE_DIR}/ui/ui_mania.c
	${CODE_DIR}/ui/ui_menu.c
	${CODE_DIR}/ui/ui_mfield.c
	${CODE_DIR}/ui/ui_mods.c
	${CODE_DIR}/ui/ui_musicbox.c
	${CODE_DIR}/ui/ui_network.c
	${CODE_DIR}/ui/ui_password.c
	${CODE_DIR}/ui/ui_players.c
	${CODE_DIR}/ui/ui_playersettings.c
	${CODE_DIR}/ui/ui_preferences.c
	${CODE_DIR}/ui/ui_qmenu.c
	${CODE_DIR}/ui/ui_removebots.c
	${CODE_DIR}/ui/ui_secret.c
	${CODE_DIR}/ui/ui_serverinfo.c
	${CODE_DIR}/ui/ui_servers.c
	${CODE_DIR}/ui/ui_setup.c
	${CODE_DIR}/ui/ui_sound.c
	${CODE_DIR}/ui/ui_specifyserver.c
	${CODE_DIR}/ui/ui_startserver.c
	${CODE_DIR}/ui/ui_team.c
	${CODE_DIR}/ui/ui_teamorders.c
	${CODE_DIR}/ui/ui_video.c
	${CODE_DIR}/ui/ui_voicechat.c
	${CODE_DIR}/ui/ui_syscalls.c

	${CODE_DIR}/game/bg_misc.c
	${CODE_DIR}/game/bg_lib.c

	${SRCS_BASE}
)
wop_add_executable(TARGET ${PROJECT_NAME}-ui SRCS ${SRCS_UI} NOINSTALL)
add_asm(${PROJECT_NAME}-ui)
target_link_libraries(${PROJECT_NAME}-ui ${LIBS})
target_include_directories(${PROJECT_NAME}-ui PRIVATE ${CODE_DIR}/qcommon)
target_include_directories(${PROJECT_NAME}-ui PRIVATE ${CODE_DIR}/ui)
target_compile_definitions(${PROJECT_NAME}-ui PRIVATE ${TEST_DEFINES})
target_compile_definitions(${PROJECT_NAME}-ui PRIVATE UI)
target_compile_definitions(${PROJECT_NAME}-ui PRIVATE UI_HARD_LINKED)
add_test(NAME ${PROJECT_NAME}-ui COMMAND $<TARGET_FILE:${PROJECT_NAME}-ui>)
set_tests_properties(${PROJECT_NAME}-ui PROPERTIES WORKING_DIRECTORY ${ROOT_DIR})

set(SRCS_VM test_vm.c ${SRCS_BASE})
wop_add_executable(TARGET ${PROJECT_NAME}-vm SRCS ${SRCS_VM} NOINSTALL)
add_asm(${PROJECT_NAME}-vm)
add_botlib(${PROJECT_NAME}-vm)
target_link_libraries(${PROJECT_NAME}-vm ${LIBS})
target_include_directories(${PROJECT_NAME}-vm PRIVATE ${CODE_DIR}/qcommon)
target_compile_definitions(${PROJECT_NAME}-vm PRIVATE ${TEST_DEFINES})

# Build the test QVM from C source using q3lcc + q3asm
set(TESTQVM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/testqvm/test_main.c)
set(TESTQVM_ASM_OUT ${CMAKE_CURRENT_BINARY_DIR}/test_main.asm)
set(TESTQVM_SYSCALLS ${CMAKE_CURRENT_SOURCE_DIR}/testqvm/test_syscalls.asm)
set(TESTQVM_QVM ${ROOT_DIR}/${ENGINE_BINARY_DIR}/testqvm.qvm)

add_custom_command(
	OUTPUT ${TESTQVM_ASM_OUT}
	COMMAND q3lcc -o "\"${TESTQVM_ASM_OUT}\"" "\"${TESTQVM_SRC}\""
	DEPENDS q3lcc ${TESTQVM_SRC}
	COMMENT "Compiling test QVM source"
)
add_custom_command(
	OUTPUT ${TESTQVM_QVM}
	COMMAND q3asm -o "${TESTQVM_QVM}" ${TESTQVM_SYSCALLS} ${TESTQVM_ASM_OUT}
	DEPENDS q3asm ${TESTQVM_ASM_OUT} ${TESTQVM_SYSCALLS}
	COMMENT "Assembling test QVM"
)
add_custom_target(testqvm_build DEPENDS ${TESTQVM_QVM})
add_dependencies(${PROJECT_NAME}-vm testqvm_build)

target_compile_definitions(${PROJECT_NAME}-vm PRIVATE TEST_QVM_PATH="${TESTQVM_QVM}")
add_test(NAME ${PROJECT_NAME}-vm COMMAND $<TARGET_FILE:${PROJECT_NAME}-vm>)
set_tests_properties(${PROJECT_NAME}-vm PROPERTIES WORKING_DIRECTORY ${ROOT_DIR})
