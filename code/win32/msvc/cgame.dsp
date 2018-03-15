# Microsoft Developer Studio Project File - Name="cgame" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cgame - Win32 Release TA
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "cgame.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "cgame.mak" CFG="cgame - Win32 Release TA"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "cgame - Win32 Release TA" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cgame - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cgame - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cgame - Win32 Debug TA" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\cgame_release_ta"
# PROP BASE Intermediate_Dir ".\build\cgame_release_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\cgame_release_ta"
# PROP Intermediate_Dir ".\build\cgame_release_ta"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "MISSIONPACK" /Fp"$(IntDir)\cgame.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "MISSIONPACK" /Fp"$(IntDir)\cgame.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /base:"0x30000000" /subsystem:windows /dll /map /machine:IX86 /def:".\cgame.def" /out:".\build\cgame_release_ta\cgamex86.dll" /implib:"$(IntDir)\/cgamex86.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /base:"0x30000000" /subsystem:windows /dll /map /machine:IX86 /def:".\cgame.def" /out:".\build\cgame_release_ta\cgamex86.dll" /implib:"$(IntDir)\/cgamex86.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\cgame_debug"
# PROP BASE Intermediate_Dir ".\build\cgame_debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\cgame_debug"
# PROP Intermediate_Dir ".\build\cgame_debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /Fp"$(IntDir)\cgame.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /Fp"Debug\cgame.pch" /YX /Fo"Debug\\" /Fd"Debug\\" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /base:"0x30000000" /subsystem:windows /dll /incremental:no /map /debug /machine:IX86 /def:".\cgame.def" /out:".\build\cgame_debug\cgamex86.dll" /implib:"$(IntDir)\cgamex86.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /base:"0x30000000" /subsystem:windows /dll /incremental:no /map /debug /machine:IX86 /def:".\cgame.def" /out:".\build\cgame_debug\cgamex86.dll" /implib:"$(IntDir)\cgamex86.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\cgame_release"
# PROP BASE Intermediate_Dir ".\build\cgame_release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\cgame_release"
# PROP Intermediate_Dir ".\build\cgame_release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(IntDir)\cgame.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"Release\cgame.pch" /YX /Fo"Release\\" /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /base:"0x30000000" /subsystem:windows /dll /map /machine:IX86 /def:".\cgame.def" /out:".\build\cgame_release\cgamex86.dll" /implib:"$(IntDir)\cgamex86.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /base:"0x30000000" /subsystem:windows /dll /map /machine:IX86 /def:".\cgame.def" /out:".\build\cgame_release\cgamex86.dll" /implib:"$(IntDir)\cgamex86.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\cgame_debug_ta"
# PROP BASE Intermediate_Dir ".\build\cgame_debug_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\cgame_debug_ta"
# PROP Intermediate_Dir ".\build\cgame_debug_ta"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "MISSIONPACK" /FR /Fp"$(IntDir)\cgame.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "MISSIONPACK" /FR /Fp"$(IntDir)\cgame.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /base:"0x30000000" /subsystem:windows /dll /incremental:no /map /debug /machine:IX86 /def:".\cgame.def" /out:".\build\cgame_debug_ta\cgamex86.dll" /implib:"$(IntDir)\cgamex86.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /base:"0x30000000" /subsystem:windows /dll /incremental:no /map /debug /machine:IX86 /def:".\cgame.def" /out:".\build\cgame_debug_ta\cgamex86.dll" /implib:"$(IntDir)\cgamex86.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "cgame - Win32 Release TA"
# Name "cgame - Win32 Debug"
# Name "cgame - Win32 Release"
# Name "cgame - Win32 Debug TA"
# Begin Group "Source Files"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\..\game\bg_lib.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\game\bg_misc.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\bg_pmove.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\game\bg_slidemove.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_consolecmds.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_draw.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_drawtools.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_effects.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_ents.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_event.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_info.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_localents.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_main.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_marks.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_newDraw.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_players.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_playerstate.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_predict.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_scoreboard.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_servercmds.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_snapshot.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_syscalls.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_view.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_weapons.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\q_math.c
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\q_shared.c
# End Source File
# Begin Source File

SOURCE=..\..\ui\ui_shared.c

!IF  "$(CFG)" == "cgame - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\..\game\bg_public.h
# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_local.h
# End Source File
# Begin Source File

SOURCE=..\..\cgame\cg_public.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\surfaceflags.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\cgame\cgame.def

!IF  "$(CFG)" == "cgame - Win32 Release TA"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
