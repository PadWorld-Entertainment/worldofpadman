echo off

set binpath=..\..\..\misc\bins_win32
set lcc=%binpath%\q3lcc
set asm=%binpath%\q3asm
set compile=%lcc% -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui

echo on

mkdir vm
cd vm

%compile% ../g_main.c
@if errorlevel 1 goto quit

%compile%  ../g_syscalls.c
@if errorlevel 1 goto quit

%compile%  ../bg_misc.c
@if errorlevel 1 goto quit
%compile%  ../bg_lib.c
@if errorlevel 1 goto quit
%compile%  ../bg_pmove.c
@if errorlevel 1 goto quit
%compile%  ../bg_slidemove.c
@if errorlevel 1 goto quit
%compile%  ../../qcommon/q_math.c
@if errorlevel 1 goto quit
%compile%  ../../qcommon/q_shared.c
@if errorlevel 1 goto quit

%compile% ../ai_dmnet.c
@if errorlevel 1 goto quit
%compile% ../ai_dmq3.c
@if errorlevel 1 goto quit
%compile% ../ai_main.c
@if errorlevel 1 goto quit
%compile% ../ai_chat.c
@if errorlevel 1 goto quit
%compile% ../ai_cmd.c
@if errorlevel 1 goto quit
%compile% ../ai_team.c
@if errorlevel 1 goto quit

%compile% ../g_active.c
@if errorlevel 1 goto quit
%compile%  ../g_arenas.c
@if errorlevel 1 goto quit
%compile%  ../g_bot.c
@if errorlevel 1 goto quit
%compile%  ../g_client.c
@if errorlevel 1 goto quit
%compile%  ../g_cmds.c
@if errorlevel 1 goto quit
%compile%  ../g_combat.c
@if errorlevel 1 goto quit
%compile%  ../g_gameinfo.c
@if errorlevel 1 goto quit
%compile%  ../g_items.c
@if errorlevel 1 goto quit
%compile%  ../g_mem.c
@if errorlevel 1 goto quit
%compile%  ../g_misc.c
@if errorlevel 1 goto quit
%compile%  ../g_missile.c
@if errorlevel 1 goto quit
%compile%  ../g_mover.c
@if errorlevel 1 goto quit
%compile%  ../g_session.c
@if errorlevel 1 goto quit
%compile%  ../g_spawn.c
@if errorlevel 1 goto quit
%compile% ../g_svcmds.c
@if errorlevel 1 goto quit
%compile%  ../g_target.c
@if errorlevel 1 goto quit
%compile%  ../g_team.c
@if errorlevel 1 goto quit
%compile%  ../g_trigger.c
@if errorlevel 1 goto quit
%compile%  ../g_utils.c
@if errorlevel 1 goto quit
%compile%  ../g_weapon.c
@if errorlevel 1 goto quit
%compile%  ../ai_vcmd.c
rem @if errorlevel 1 goto quit
%compile%  ../g_stations.c
@if errorlevel 1 goto quit
%compile%  ../g_modifiers.c
@if errorlevel 1 goto quit

%compile%  ../wopg_spstoryfiles.c
@if errorlevel 1 goto quit

%compile%  ../wopg_sphandling.c
@if errorlevel 1 goto quit

%compile%  ../g_ctlitems.c
@if errorlevel 1 goto quit

%asm% -v -f ../game

:quit
echo off
cd ..

rem PATH=%oldPATH%

PAUSE
