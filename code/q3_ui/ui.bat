echo off

set binpath=..\..\..\misc\bins_win32
set lcc=%binpath%\q3lcc
set asm=%binpath%\q3asm
set compile=%lcc% -DQ3_VM -DUI -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui

echo on

mkdir vm
cd vm

%compile% ../ui_main.c
@if errorlevel 1 goto quit
%compile% ../ui_cdkey.c
@if errorlevel 1 goto quit
%compile% ../ui_ingame.c
@if errorlevel 1 goto quit
%compile% ../ui_serverinfo.c
@if errorlevel 1 goto quit
%compile% ../ui_confirm.c
@if errorlevel 1 goto quit
%compile% ../ui_setup.c
@if errorlevel 1 goto quit
%compile% ../../game/bg_misc.c
@if errorlevel 1 goto quit
%compile% ../../game/bg_lib.c
@if errorlevel 1 goto quit
%compile% ../../qcommon/q_math.c
@if errorlevel 1 goto quit
%compile% ../../qcommon/q_shared.c
@if errorlevel 1 goto quit
%compile% ../ui_gameinfo.c
@if errorlevel 1 goto quit
%compile% ../ui_atoms.c
@if errorlevel 1 goto quit
%compile% ../ui_connect.c
@if errorlevel 1 goto quit
%compile% ../ui_controls2.c
@if errorlevel 1 goto quit
%compile% ../ui_demo2.c
@if errorlevel 1 goto quit
%compile% ../ui_mfield.c
@if errorlevel 1 goto quit
%compile% ../ui_credits.c
@if errorlevel 1 goto quit
%compile% ../ui_menu.c
@if errorlevel 1 goto quit
%compile% ../ui_options.c
@if errorlevel 1 goto quit
%compile% ../ui_display.c
@if errorlevel 1 goto quit
%compile% ../ui_sound.c
@if errorlevel 1 goto quit
%compile% ../ui_network.c
@if errorlevel 1 goto quit
%compile% ../ui_playermodel.c
@if errorlevel 1 goto quit
%compile% ../ui_players.c
@if errorlevel 1 goto quit
%compile% ../ui_playersettings.c
@if errorlevel 1 goto quit
%compile% ../ui_preferences.c
@if errorlevel 1 goto quit
%compile% ../ui_qmenu.c
@if errorlevel 1 goto quit
%compile% ../ui_servers2.c
@if errorlevel 1 goto quit
%compile% ../ui_sparena.c
@if errorlevel 1 goto quit
%compile% ../ui_specifyserver.c
@if errorlevel 1 goto quit
%compile% ../ui_splevel.c
@if errorlevel 1 goto quit
%compile% ../ui_sppostgame.c
@if errorlevel 1 goto quit
%compile% ../ui_startserver2.c
@if errorlevel 1 goto quit
%compile% ../ui_team.c
@if errorlevel 1 goto quit
%compile% ../ui_video.c
@if errorlevel 1 goto quit
%compile% ../ui_cinematics.c
@if errorlevel 1 goto quit
%compile% ../ui_spskill.c
@if errorlevel 1 goto quit
%compile% ../ui_addbots.c
@if errorlevel 1 goto quit
%compile% ../ui_removebots.c
@if errorlevel 1 goto quit
rem %compile% ../ui_loadconfig.c
rem @if errorlevel 1 goto quit
rem %compile% ../ui_saveconfig.c
rem @if errorlevel 1 goto quit
%compile% ../ui_teamorders.c
@if errorlevel 1 goto quit
%compile% ../ui_mods.c
@if errorlevel 1 goto quit

%compile% ../ui_exit.c
@if errorlevel 1 goto quit

%compile% ../ui_music.c
@if errorlevel 1 goto quit

%compile% ../ui_callvote.c
@if errorlevel 1 goto quit

%compile% ../../cgame/wopc_advanced2d.c
@if errorlevel 1 goto quit

%compile% ../ui_wopsp.c
@if errorlevel 1 goto quit

%compile% ../../game/wopg_spstoryfiles.c
@if errorlevel 1 goto quit

%compile% ../../game/wopg_sphandling.c
@if errorlevel 1 goto quit

%compile% ../ui_mediaview.c
@if errorlevel 1 goto quit

%compile% ../ui_help.c
@if errorlevel 1 goto quit

%compile% ../ui_voicechat.c
@if errorlevel 1 goto quit


%asm% -v -f ../ui

:quit
echo off
cd ..

rem PATH=%oldPATH%

PAUSE
