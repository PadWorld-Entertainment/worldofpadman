# Microsoft Developer Studio Project File - Name="game" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=game - Win32 Debug TA
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "game.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "game.mak" CFG="game - Win32 Debug TA"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "game - Win32 Debug TA" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "game - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "game - Win32 Release Alpha" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "game - Win32 Debug Alpha" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "game - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "game - Win32 Release TA" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "game - Win32 Debug TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\game_debug_ta"
# PROP BASE Intermediate_Dir ".\build\game_debug_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\game_debug_ta"
# PROP Intermediate_Dir ".\build\game_debug_ta"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "BUILDING_REF_GL" /D "DEBUG" /D "MISSIONPACK" /D "QAGAME" /FR /Fp"$(IntDir)\game.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "BUILDING_REF_GL" /D "DEBUG" /D "MISSIONPACK" /D "QAGAME" /FR /Fp"$(IntDir)\game.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /incremental:no /map /debug /machine:IX86 /def:".\game.def" /out:".\build\game_debug_ta\qagamex86.dll" /implib:"$(IntDir)\qagamex86.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /incremental:no /map /debug /machine:IX86 /def:".\game.def" /out:".\build\game_debug_ta\qagamex86.dll" /implib:"$(IntDir)\qagamex86.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\game_debug"
# PROP BASE Intermediate_Dir ".\build\game_debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\game_debug"
# PROP Intermediate_Dir ".\build\game_debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "BUILDING_REF_GL" /D "DEBUG" /D "GLOBALRANK" /FR /Fp"$(IntDir)\game.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "BUILDING_REF_GL" /D "DEBUG" /D "GLOBALRANK" /FR /Fp"Debug\game.pch" /YX /Fo"Debug\\" /Fd"Debug\\" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /incremental:no /map /debug /machine:IX86 /def:".\game.def" /out:".\build\game_debug\qagamex86.dll" /implib:"$(IntDir)\qagamex86.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /incremental:no /map /debug /machine:IX86 /def:".\game.def" /out:".\build\game_debug\qagamex86.dll" /implib:"$(IntDir)\qagamex86.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\game_release_alpha"
# PROP BASE Intermediate_Dir ".\build\game_release_alpha"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\game_release_alpha"
# PROP Intermediate_Dir ".\build\game_release_alpha"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "C_ONLY" /Fp"$(IntDir)\game.pch" /YX /Fo"$(IntDir)\\" /GF /QA21164 /c
# ADD CPP /nologo /W3 /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "C_ONLY" /Fp"$(IntDir)\game.pch" /YX /Fo"$(IntDir)\\" /GF /QA21164 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /machine:IX86 /def:".\game.def" /out:".\build\game_release_alpha\qagameaxp.dll" /implib:"$(IntDir)\qagameaxp.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /machine:IX86 /def:".\game.def" /out:".\build\game_release_alpha\qagameaxp.dll" /implib:"$(IntDir)\qagameaxp.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\game_debug_alpha"
# PROP BASE Intermediate_Dir ".\build\game_debug_alpha"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\game_debug_alpha"
# PROP Intermediate_Dir ".\build\game_debug_alpha"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "C_ONLY" /Fp"$(IntDir)\game.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /QA21164 /c
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "C_ONLY" /Fp"$(IntDir)\game.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /QA21164 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /incremental:no /debug /machine:IX86 /def:".\game.def" /out:".\build\game_debug_alpha\qagameaxp.dll" /implib:"$(IntDir)\qagameaxp.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /incremental:no /debug /machine:IX86 /def:".\game.def" /out:".\build\game_debug_alpha\qagameaxp.dll" /implib:"$(IntDir)\qagameaxp.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\game_release"
# PROP BASE Intermediate_Dir ".\build\game_release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\game_release"
# PROP Intermediate_Dir ".\build\game_release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "GLOBALRANK" /Fp"$(IntDir)\game.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "GLOBALRANK" /Fp"Release\game.pch" /YX /Fo"Release\\" /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /map /machine:IX86 /def:".\game.def" /out:".\build\game_release\qagamex86.dll" /implib:"$(IntDir)\qagamex86.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /map /machine:IX86 /def:".\game.def" /out:".\build\game_release\qagamex86.dll" /implib:"$(IntDir)\qagamex86.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\game_release_ta"
# PROP BASE Intermediate_Dir ".\build\game_release_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\game_release_ta"
# PROP Intermediate_Dir ".\build\game_release_ta"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "MISSIONPACK" /Fp"$(IntDir)\game.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "MISSIONPACK" /Fp"$(IntDir)\game.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /map /machine:IX86 /def:".\game.def" /out:".\build\game_release_ta\qagamex86.dll" /implib:"$(IntDir)\qagamex86.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /map /machine:IX86 /def:".\game.def" /out:".\build\game_release_ta\qagamex86.dll" /implib:"$(IntDir)\qagamex86.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "game - Win32 Debug TA"
# Name "game - Win32 Debug"
# Name "game - Win32 Release Alpha"
# Name "game - Win32 Debug Alpha"
# Name "game - Win32 Release"
# Name "game - Win32 Release TA"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\game\ai_chat.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\ai_cmd.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\ai_dmnet.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\ai_dmq3.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\ai_main.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\ai_team.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\ai_vcmd.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\bg_lib.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\bg_misc.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\bg_pmove.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\bg_slidemove.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_active.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_arenas.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_bot.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_client.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_cmds.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_combat.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_items.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_main.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_mem.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_misc.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_missile.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_mover.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_session.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_spawn.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_svcmds.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_syscalls.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_target.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_team.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_trigger.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_utils.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\g_weapon.c

!IF  "$(CFG)" == "game - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

# ADD CPP /nologo /GX /Od /GZ

!ELSEIF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\game.def

!IF  "$(CFG)" == "game - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

!ELSEIF  "$(CFG)" == "game - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "game - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "game - Win32 Release"

!ELSEIF  "$(CFG)" == "game - Win32 Release TA"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\q_math.c
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\q_shared.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\..\game\ai_chat.h
# End Source File
# Begin Source File

SOURCE=..\..\game\ai_cmd.h
# End Source File
# Begin Source File

SOURCE=..\..\game\ai_dmnet.h
# End Source File
# Begin Source File

SOURCE=..\..\game\ai_dmq3.h
# End Source File
# Begin Source File

SOURCE=..\..\game\ai_main.h
# End Source File
# Begin Source File

SOURCE=..\..\game\ai_team.h
# End Source File
# Begin Source File

SOURCE=..\..\game\ai_vcmd.h
# End Source File
# Begin Source File

SOURCE=..\..\game\be_aas.h
# End Source File
# Begin Source File

SOURCE=..\..\game\be_ai_char.h
# End Source File
# Begin Source File

SOURCE=..\..\game\be_ai_chat.h
# End Source File
# Begin Source File

SOURCE=..\..\game\be_ai_gen.h
# End Source File
# Begin Source File

SOURCE=..\..\game\be_ai_goal.h
# End Source File
# Begin Source File

SOURCE=..\..\game\be_ai_move.h
# End Source File
# Begin Source File

SOURCE=..\..\game\be_ai_weap.h
# End Source File
# Begin Source File

SOURCE=..\..\game\be_ea.h
# End Source File
# Begin Source File

SOURCE=..\..\game\bg_local.h
# End Source File
# Begin Source File

SOURCE=..\..\game\bg_public.h
# End Source File
# Begin Source File

SOURCE=..\..\game\botlib.h
# End Source File
# Begin Source File

SOURCE=..\..\game\chars.h
# End Source File
# Begin Source File

SOURCE=..\..\game\g_local.h
# End Source File
# Begin Source File

SOURCE=..\..\game\g_public.h
# End Source File
# Begin Source File

SOURCE=..\..\game\g_team.h
# End Source File
# Begin Source File

SOURCE=..\..\game\inv.h
# End Source File
# Begin Source File

SOURCE=..\..\game\match.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\surfaceflags.h
# End Source File
# Begin Source File

SOURCE=..\..\game\syn.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
