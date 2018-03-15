echo off

set binpath=..\..\..\misc\bins_win32
set lcc=%binpath%\q3lcc
set asm=%binpath%\q3asm
set compile=%lcc% -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui

echo on

mkdir vm
cd vm

%compile% ../../game/bg_misc.c
@if errorlevel 1 goto quit
%compile% ../../game/bg_pmove.c
@if errorlevel 1 goto quit
%compile% ../../game/bg_slidemove.c
@if errorlevel 1 goto quit
%compile% ../../game/bg_lib.c
@if errorlevel 1 goto quit
%compile% ../../qcommon/q_math.c
@if errorlevel 1 goto quit
%compile% ../../qcommon/q_shared.c
@if errorlevel 1 goto quit
%compile% ../cg_consolecmds.c
@if errorlevel 1 goto quit
%compile% ../cg_draw.c
@if errorlevel 1 goto quit
%compile% ../cg_drawtools.c
@if errorlevel 1 goto quit
%compile% ../cg_effects.c
@if errorlevel 1 goto quit
%compile% ../cg_ents.c
@if errorlevel 1 goto quit
%compile% ../cg_event.c
@if errorlevel 1 goto quit
%compile% ../cg_info.c
@if errorlevel 1 goto quit
%compile% ../cg_localents.c
@if errorlevel 1 goto quit
%compile% ../cg_main.c
@if errorlevel 1 goto quit
%compile% ../cg_marks.c
@if errorlevel 1 goto quit
%compile% ../cg_misc.c
@if errorlevel 1 goto quit
%compile% ../cg_particles.c
@if errorlevel 1 goto quit
%compile% ../cg_players.c
@if errorlevel 1 goto quit
%compile% ../cg_playerstate.c
@if errorlevel 1 goto quit
%compile% ../cg_predict.c
@if errorlevel 1 goto quit
%compile% ../cg_scoreboard.c
@if errorlevel 1 goto quit
%compile% ../cg_servercmds.c
@if errorlevel 1 goto quit
%compile% ../cg_snapshot.c
@if errorlevel 1 goto quit
%compile% ../cg_view.c
@if errorlevel 1 goto quit
%compile% ../cg_weapons.c
@if errorlevel 1 goto quit


%compile% ../wopc_advanced2d.c
@if errorlevel 1 goto quit
%compile% ../cg_lensflare.c
@if errorlevel 1 goto quit
%compile% ../cg_rautelib.c
@if errorlevel 1 goto quit
%compile% ../cg_spraylogo.c
@if errorlevel 1 goto quit
%compile% ../cg_spriteparticles.c
@if errorlevel 1 goto quit
%compile% ../cg_cutscene2d.c
@if errorlevel 1 goto quit

%asm% -v -f ../cgame

:quit
echo off
cd ..

rem PATH=%oldPATH%

PAUSE
