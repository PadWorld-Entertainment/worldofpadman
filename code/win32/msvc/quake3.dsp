# Microsoft Developer Studio Project File - Name="quake3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=quake3 - Win32 Release TA
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "quake3.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "quake3.mak" CFG="quake3 - Win32 Release TA"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "quake3 - Win32 Release TA" (basierend auf  "Win32 (x86) Application")
!MESSAGE "quake3 - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE "quake3 - Win32 Release TA DEMO" (basierend auf  "Win32 (x86) Application")
!MESSAGE "quake3 - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "quake3 - Win32 Debug TA DEMO" (basierend auf  "Win32 (x86) Application")
!MESSAGE "quake3 - Win32 vector" (basierend auf  "Win32 (x86) Application")
!MESSAGE "quake3 - Win32 Debug TA" (basierend auf  "Win32 (x86) Application")
!MESSAGE "quake3 - Win32 Release_noOpenAL" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\quake3_release_ta"
# PROP BASE Intermediate_Dir ".\build\quake3_release_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\quake3_release_ta"
# PROP Intermediate_Dir ".\build\quake3_release_ta"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "_WIN32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "USE_OPENAL" /FR /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "_WIN32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "USE_OPENAL" /FR /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /machine:IX86 /out:".\build\quake3_release_ta\ioquake3.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /machine:IX86 /out:".\build\quake3_release_ta\ioquake3.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Post build processing..
PostBuild_Cmds=rem bash -c "perl ./unix/cons -- release-TA"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\quake3_debug"
# PROP BASE Intermediate_Dir ".\build\quake3_debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\quake3_debug"
# PROP Intermediate_Dir ".\build\quake3_debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "USE_OPENAL" /FR"$(IntDir)\\" /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_OPENAL" /D "USE_OPENAL_DLOPEN" /D "USE_LOCAL_HEADERS" /D "USE_CODEC_VORBIS" /D "USE_CODEC_MP3" /FR"Debug\\" /Fp"Debug\quake3.pch" /YX /Fo"Debug\\" /Fd"Debug\\" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /fo"..\winquake.res" /d "_DEBUG"
# ADD RSC /l 0x409 /fo"..\winquake.res" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:".\build\quake3_debug\ioquake3.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib ogg_static.lib vorbisfile_static.lib libmad.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:"C:\WorldOfPadman\wop_d.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Post build processing..
PostBuild_Cmds=rem bash -c "perl ./unix/cons -- debug"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\quake3_release_ta_demo"
# PROP BASE Intermediate_Dir ".\build\quake3_release_ta_demo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\quake3_release_ta_demo"
# PROP Intermediate_Dir ".\build\quake3_release_ta_demo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /Zd /O2 /Ob2 /D "_WIN32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "PRE_RELEASE_TADEMO" /D "PRE_RELEASE_DEMO" /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /Zd /O2 /Ob2 /D "_WIN32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "PRE_RELEASE_TADEMO" /D "PRE_RELEASE_DEMO" /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:".\build\quake3_release_ta_demo\ioquake3.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:".\build\quake3_release_ta_demo\ioquake3.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\quake3_release"
# PROP BASE Intermediate_Dir ".\build\quake3_release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\quake3_release"
# PROP Intermediate_Dir ".\build\quake3_release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "USE_OPENAL" /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_OPENAL" /D "USE_OPENAL_DLOPEN" /D "USE_LOCAL_HEADERS" /D "USE_CODEC_VORBIS" /D "USE_CODEC_MP3" /Fp"Release\quake3.pch" /YX /Fo"Release\\" /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:".\build\quake3_release\ioquake3.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib ogg_static.lib vorbisfile_static.lib libmad.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:"C:\WorldOfPadman\wop.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\quake3_debug_ta_demo"
# PROP BASE Intermediate_Dir ".\build\quake3_debug_ta_demo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\quake3_debug_ta_demo"
# PROP Intermediate_Dir ".\build\quake3_debug_ta_demo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "PRE_RELEASE_TADEMO" /D "PRE_RELEASE_DEMO" /FR"$(IntDir)\\" /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "PRE_RELEASE_TADEMO" /D "PRE_RELEASE_DEMO" /FR"$(IntDir)\\" /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /fo"..\winquake.res" /d "_DEBUG"
# ADD RSC /l 0x409 /fo"..\winquake.res" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:".\build\quake3_debug_ta_demo\ioquake3.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:".\build\quake3_debug_ta_demo\ioquake3.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\quake3_vector0"
# PROP BASE Intermediate_Dir ".\build\quake3_vector0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\quake3_vector0"
# PROP Intermediate_Dir ".\build\quake3_vector0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /Zi /O2 /Ob2 /D "_WIN32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GF /vec /vec:stats /vec:pii /vec:o10 /vec:yes /c
# ADD CPP /nologo /G6 /W4 /GX /Zi /O2 /Ob2 /D "_WIN32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GF /vec /vec:stats /vec:pii /vec:o10 /vec:yes /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:".\build\quake3_vector0\ioquake3.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:".\build\quake3_vector0\ioquake3.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\quake3_debug_ta"
# PROP BASE Intermediate_Dir ".\build\quake3_debug_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\quake3_debug_ta"
# PROP Intermediate_Dir ".\build\quake3_debug_ta"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "USE_OPENAL" /FR"$(IntDir)\\" /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "USE_OPENAL" /FR"$(IntDir)\\" /Fp"$(IntDir)\quake3.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /fo"..\winquake.res" /d "_DEBUG"
# ADD RSC /l 0x409 /fo"..\winquake.res" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:".\build\quake3_debug_ta\ioquake3.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:".\build\quake3_debug_ta\ioquake3.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Post build processing..
PostBuild_Cmds=rem bash -c "perl ./unix/cons -- debug-TA"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "quake3___Win32_Release_noOpenAL"
# PROP BASE Intermediate_Dir "quake3___Win32_Release_noOpenAL"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "quake3___Win32_Release_noOpenAL"
# PROP Intermediate_Dir "quake3___Win32_Release_noOpenAL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_OPENAL" /D "USE_LOCAL_HEADERS" /D "USE_CODEC_VORBIS" /Fp"Release\quake3.pch" /YX /Fo"Release\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_LOCAL_HEADERS" /D "USE_CODEC_VORBIS" /Fp"Release\quake3.pch" /YX /Fo"Release\\" /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib ogg_static.lib vorbisfile_static.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:"C:\World of Padman\wop_ioq3.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib wsock32.lib openal32.lib ogg_static.lib vorbisfile_static.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:IX86 /out:"C:\World of Padman\wop_ioq3_noOpenAL.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "quake3 - Win32 Release TA"
# Name "quake3 - Win32 Debug"
# Name "quake3 - Win32 Release TA DEMO"
# Name "quake3 - Win32 Release"
# Name "quake3 - Win32 Debug TA DEMO"
# Name "quake3 - Win32 vector"
# Name "quake3 - Win32 Debug TA"
# Name "quake3 - Win32 Release_noOpenAL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\client\cl_avi.c
DEP_CPP_CL_AV=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_local.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\client\cl_cgame.c
DEP_CPP_CL_CG=\
	"..\..\botlib\botlib.h"\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\cl_cin.c
DEP_CPP_CL_CI=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_local.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\cl_console.c
DEP_CPP_CL_CO=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\cl_input.c
DEP_CPP_CL_IN=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\cl_keys.c
DEP_CPP_CL_KE=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\cl_main.c
DEP_CPP_CL_MA=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\cl_net_chan.c
DEP_CPP_CL_NE=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\cl_parse.c
DEP_CPP_CL_PA=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\cl_scrn.c
DEP_CPP_CL_SC=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\cl_ui.c
DEP_CPP_CL_UI=\
	"..\..\botlib\botlib.h"\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cm_load.c
DEP_CPP_CM_LO=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_local.h"\
	"..\..\qcommon\cm_polylib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	
NODEP_CPP_CM_LO=\
	"..\..\bspc\l_qfiles.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cm_patch.c
DEP_CPP_CM_PA=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_local.h"\
	"..\..\qcommon\cm_patch.h"\
	"..\..\qcommon\cm_polylib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cm_polylib.c
DEP_CPP_CM_PO=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_local.h"\
	"..\..\qcommon\cm_polylib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cm_test.c
DEP_CPP_CM_TE=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_local.h"\
	"..\..\qcommon\cm_polylib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cm_trace.c
DEP_CPP_CM_TR=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_local.h"\
	"..\..\qcommon\cm_polylib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cmd.c
DEP_CPP_CMD_C=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\common.c
DEP_CPP_COMMO=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cvar.c
DEP_CPP_CVAR_=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\files.c
DEP_CPP_FILES=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\qcommon\unzip.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\huffman.c
DEP_CPP_HUFFM=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\md4.c
DEP_CPP_MD4_C=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\md5.c
DEP_CPP_MD5_C=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\msg.c
DEP_CPP_MSG_C=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\net_chan.c
DEP_CPP_NET_C=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\.\qcommon\q_math.c
DEP_CPP_Q_MAT=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\.\qcommon\q_shared.c
DEP_CPP_Q_SHA=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\client\qal.c
DEP_CPP_QAL_C=\
	"..\..\AL\al.h"\
	"..\..\AL\alc.h"\
	"..\..\AL\alctypes.h"\
	"..\..\AL\altypes.h"\
	"..\..\client\qal.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	
NODEP_CPP_QAL_C=\
	"..\..\client\SDL.h"\
	"..\..\client\SDL_loadso.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\client\snd_adpcm.c
DEP_CPP_SND_A=\
	"..\..\client\snd_local.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\snd_codec.c
DEP_CPP_SND_C=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_codec.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\client\snd_codec_mp3.c
DEP_CPP_SND_CO=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_codec.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	{$(INCLUDE)}"mad.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\client\snd_codec_ogg.c
DEP_CPP_SND_COD=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_codec.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\client\snd_codec_wav.c
DEP_CPP_SND_CODE=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_codec.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\client\snd_dma.c
DEP_CPP_SND_D=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_codec.h"\
	"..\..\client\snd_local.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\snd_main.c
DEP_CPP_SND_M=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_codec.h"\
	"..\..\client\snd_local.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\client\snd_mem.c
DEP_CPP_SND_ME=\
	"..\..\client\snd_codec.h"\
	"..\..\client\snd_local.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\snd_mix.c
DEP_CPP_SND_MI=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_local.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\client\snd_openal.c
DEP_CPP_SND_O=\
	"..\..\AL\al.h"\
	"..\..\AL\alc.h"\
	"..\..\AL\alctypes.h"\
	"..\..\AL\altypes.h"\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\qal.h"\
	"..\..\client\snd_codec.h"\
	"..\..\client\snd_local.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\client\snd_wavelet.c
DEP_CPP_SND_W=\
	"..\..\client\snd_local.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\server\sv_bot.c
DEP_CPP_SV_BO=\
	"..\..\botlib\botlib.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\game\g_public.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\server\server.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\server\sv_ccmds.c
DEP_CPP_SV_CC=\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\game\g_public.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\server\server.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\server\sv_client.c
DEP_CPP_SV_CL=\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\game\g_public.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\server\server.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\server\sv_game.c
DEP_CPP_SV_GA=\
	"..\..\botlib\botlib.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\game\g_public.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\server\server.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\server\sv_init.c
DEP_CPP_SV_IN=\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\game\g_public.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\server\server.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\server\sv_main.c
DEP_CPP_SV_MA=\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\game\g_public.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\server\server.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\server\sv_net_chan.c
DEP_CPP_SV_NE=\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\game\g_public.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\server\server.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\server\sv_snapshot.c
DEP_CPP_SV_SN=\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\game\g_public.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\server\server.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\server\sv_world.c
DEP_CPP_SV_WO=\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\game\g_public.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\server\server.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\unzip.c
DEP_CPP_UNZIP=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\qcommon\unzip.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\vm.c
DEP_CPP_VM_C62=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\qcommon\vm_local.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\vm_interpreted.c
DEP_CPP_VM_IN=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\qcommon\vm_local.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\vm_x86.c
DEP_CPP_VM_X8=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\qcommon\vm_local.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\win32\win_input.c
DEP_CPP_WIN_I=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	"..\win_local.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\win32\win_main.c
DEP_CPP_WIN_M=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	"..\resource.h"\
	"..\win_local.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\win32\win_net.c
DEP_CPP_WIN_N=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\win_local.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\win32\win_shared.c
DEP_CPP_WIN_S=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\win_local.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\win32\win_snd.c
DEP_CPP_WIN_SN=\
	"..\..\client\snd_local.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\win_local.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\win32\win_syscon.c
DEP_CPP_WIN_SY=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	"..\resource.h"\
	"..\win_local.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\win32\win_wndproc.c
DEP_CPP_WIN_W=\
	"..\..\cgame\cg_public.h"\
	"..\..\client\cl_curl.h"\
	"..\..\client\client.h"\
	"..\..\client\keycodes.h"\
	"..\..\client\keys.h"\
	"..\..\client\snd_public.h"\
	"..\..\game\bg_lib.h"\
	"..\..\game\bg_public.h"\
	"..\..\libcurl\curl\curl.h"\
	"..\..\libcurl\curl\curlver.h"\
	"..\..\libcurl\curl\easy.h"\
	"..\..\libcurl\curl\multi.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\..\ui\ui_public.h"\
	"..\win_local.h"\
	

!IF  "$(CFG)" == "quake3 - Win32 Release TA"

# ADD CPP /nologo /GX /O2 /FR

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release TA DEMO"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA DEMO"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "quake3 - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "quake3 - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\..\game\bg_public.h
# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_public.h
# End Source File
# Begin Source File

SOURCE=..\..\client\client.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cm_local.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cm_patch.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cm_polylib.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cm_public.h
# End Source File
# Begin Source File

SOURCE=..\..\game\g_public.h
# End Source File
# Begin Source File

SOURCE=..\..\ui\keycodes.h
# End Source File
# Begin Source File

SOURCE=..\..\client\keys.h
# End Source File
# Begin Source File

SOURCE=..\..\.\qcommon\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\..\client\qal.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\qcommon.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\qfiles.h
# End Source File
# Begin Source File

SOURCE=..\..\renderer\qgl.h
# End Source File
# Begin Source File

SOURCE=..\..\win32\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\server\server.h
# End Source File
# Begin Source File

SOURCE=..\..\client\snd_local.h
# End Source File
# Begin Source File

SOURCE=..\..\client\snd_public.h
# End Source File
# Begin Source File

SOURCE=..\..\.\qcommon\surfaceflags.h
# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_local.h
# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_public.h
# End Source File
# Begin Source File

SOURCE=..\..\cgame\tr_types.h
# End Source File
# Begin Source File

SOURCE=..\..\ui\ui_public.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\unzip.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\vm_local.h
# End Source File
# Begin Source File

SOURCE=..\..\win32\win_local.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\win32\qe3.ico
# End Source File
# Begin Source File

SOURCE=..\..\win32\winquake.rc
# ADD BASE RSC /l 0x407 /i "\editing\coding\WoP-Engine_wc\code\win32"
# ADD RSC /l 0x409 /i "\editing\coding\WoP-Engine_wc\code\win32" /i "win32"
# End Source File
# End Group
# End Target
# End Project
