# Microsoft Developer Studio Project File - Name="ui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ui - Win32 vector
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ui.mak" CFG="ui - Win32 vector"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ui - Win32 vector" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ui - Win32 Release TA" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ui - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ui - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ui - Win32 Debug TA" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ui - Win32 vector"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\ui_vector0"
# PROP BASE Intermediate_Dir ".\build\ui_vector0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\ui_vector0"
# PROP Intermediate_Dir ".\build\ui_vector0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /W3 /O2 /Ob1 /G6 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "UI_EXPORTS" /D "_MBCS" /GF /Gy /YX /Fp"$(IntDir)\ui.pch" /Fo"$(IntDir)\" /Fd"$(IntDir)\" /c /GX 
# ADD CPP /nologo /ML /W3 /O2 /Ob1 /G6 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "UI_EXPORTS" /D "_MBCS" /GF /Gy /YX /Fp"$(IntDir)\ui.pch" /Fo"$(IntDir)\" /Fd"$(IntDir)\" /c /GX 
# ADD BASE MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\ui___Win32_vector\ui.tlb" /win32 
# ADD MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\ui___Win32_vector\ui.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDEBUG" 
# ADD RSC /l 1033 /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\build\ui_vector0\uix86.dll" /incremental:no /def:".\ui.def" /pdb:".\build\ui_vector0\uix86.pdb" /pdbtype:sept /map:".\build\ui_vector0\uix86.map" /subsystem:windows /base:"0x40000000" /implib:"$(IntDir)\uix86.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\build\ui_vector0\uix86.dll" /incremental:no /def:".\ui.def" /pdb:".\build\ui_vector0\uix86.pdb" /pdbtype:sept /map:".\build\ui_vector0\uix86.map" /subsystem:windows /base:"0x40000000" /implib:"$(IntDir)\uix86.lib" /machine:ix86 

!ELSEIF  "$(CFG)" == "ui - Win32 Release TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\ui_release_ta"
# PROP BASE Intermediate_Dir ".\build\ui_release_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\ui_release_ta"
# PROP Intermediate_Dir ".\build\ui_release_ta"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /W3 /O2 /Ob1 /G6 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "UI_EXPORTS" /D "_MBCS" /GF /Gy /YX /Fp"$(IntDir)\ui.pch" /Fo"$(IntDir)\" /Fd"$(IntDir)\" /c /GX 
# ADD CPP /nologo /ML /W3 /O2 /Ob1 /G6 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "UI_EXPORTS" /D "_MBCS" /GF /Gy /YX /Fp"$(IntDir)\ui.pch" /Fo"$(IntDir)\" /Fd"$(IntDir)\" /c /GX 
# ADD BASE MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release_TA\ui.tlb" /win32 
# ADD MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release_TA\ui.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDEBUG" 
# ADD RSC /l 1033 /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\build\ui_release_ta\uix86.dll" /incremental:no /def:".\ui.def" /pdb:".\build\ui_release_ta\uix86.pdb" /pdbtype:sept /map:".\build\ui_release_ta\uix86.map" /subsystem:windows /base:"0x40000000" /implib:"$(IntDir)\uix86.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\build\ui_release_ta\uix86.dll" /incremental:no /def:".\ui.def" /pdb:".\build\ui_release_ta\uix86.pdb" /pdbtype:sept /map:".\build\ui_release_ta\uix86.map" /subsystem:windows /base:"0x40000000" /implib:"$(IntDir)\uix86.lib" /machine:ix86 

!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\ui_debug"
# PROP BASE Intermediate_Dir ".\build\ui_debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\ui_debug"
# PROP Intermediate_Dir ".\build\ui_debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /ZI /W3 /Od /G5 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "UI_EXPORTS" /D "_MBCS" /YX /Fp"$(IntDir)\ui.pch" /Fo"$(IntDir)\" /Fd"$(IntDir)\" /FR /GZ /c /GX 
# ADD CPP /nologo /MTd /ZI /W3 /Od /G5 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "UI_EXPORTS" /D "_MBCS" /YX /Fp"$(IntDir)\ui.pch" /Fo"$(IntDir)\" /Fd"$(IntDir)\" /FR /GZ /c /GX 
# ADD BASE MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug\ui.tlb" /win32 
# ADD MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug\ui.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_DEBUG" 
# ADD RSC /l 1033 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\build\ui_debug\uix86_new.dll" /incremental:no /debug /pdb:".\build\ui_debug\ui.pdb" /pdbtype:sept /map:".\build\ui_debug\uix86_new.map" /subsystem:windows /base:"0x40000000" /implib:"$(IntDir)\uix86_new.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\build\ui_debug\uix86_new.dll" /incremental:no /debug /pdb:".\build\ui_debug\ui.pdb" /pdbtype:sept /map:".\build\ui_debug\uix86_new.map" /subsystem:windows /base:"0x40000000" /implib:"$(IntDir)\uix86_new.lib" /machine:ix86 

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\ui_release"
# PROP BASE Intermediate_Dir ".\build\ui_release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\ui_release"
# PROP Intermediate_Dir ".\build\ui_release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /W3 /O2 /Ob1 /G6 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "UI_EXPORTS" /D "_MBCS" /GF /Gy /YX /Fp"$(IntDir)\ui.pch" /Fo"$(IntDir)\" /Fd"$(IntDir)\" /c /GX 
# ADD CPP /nologo /ML /W3 /O2 /Ob1 /G6 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "UI_EXPORTS" /D "_MBCS" /GF /Gy /YX /Fp"$(IntDir)\ui.pch" /Fo"$(IntDir)\" /Fd"$(IntDir)\" /c /GX 
# ADD BASE MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release\ui.tlb" /win32 
# ADD MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release\ui.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDEBUG" 
# ADD RSC /l 1033 /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\build\ui_release\uix86_new.dll" /incremental:no /pdb:".\build\ui_release\uix86_new.pdb" /pdbtype:sept /map:".\build\ui_release\uix86_new.map" /subsystem:windows /base:"0x40000000" /implib:"$(IntDir)\uix86_new.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\build\ui_release\uix86_new.dll" /incremental:no /pdb:".\build\ui_release\uix86_new.pdb" /pdbtype:sept /map:".\build\ui_release\uix86_new.map" /subsystem:windows /base:"0x40000000" /implib:"$(IntDir)\uix86_new.lib" /machine:ix86 

!ELSEIF  "$(CFG)" == "ui - Win32 Debug TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\ui_debug_ta"
# PROP BASE Intermediate_Dir ".\build\ui_debug_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\ui_debug_ta"
# PROP Intermediate_Dir ".\build\ui_debug_ta"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /ZI /W3 /Od /G5 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "UI_EXPORTS" /D "MISSIONPACK" /D "_MBCS" /YX /Fp"$(IntDir)\ui.pch" /Fo"$(IntDir)\" /Fd"$(IntDir)\" /FR /GZ /c /GX 
# ADD CPP /nologo /MTd /ZI /W3 /Od /G5 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "UI_EXPORTS" /D "MISSIONPACK" /D "_MBCS" /YX /Fp"$(IntDir)\ui.pch" /Fo"$(IntDir)\" /Fd"$(IntDir)\" /FR /GZ /c /GX 
# ADD BASE MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug_TA\ui.tlb" /win32 
# ADD MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug_TA\ui.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_DEBUG" 
# ADD RSC /l 1033 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\build\ui_debug_ta\uix86.dll" /incremental:no /def:".\ui.def" /debug /pdb:".\build\ui_debug_ta\ui.pdb" /pdbtype:sept /map:".\build\ui_debug_ta\uix86.map" /subsystem:windows /base:"0x40000000" /implib:"$(IntDir)\uix86.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\build\ui_debug_ta\uix86.dll" /incremental:no /def:".\ui.def" /debug /pdb:".\build\ui_debug_ta\ui.pdb" /pdbtype:sept /map:".\build\ui_debug_ta\uix86.map" /subsystem:windows /base:"0x40000000" /implib:"$(IntDir)\uix86.lib" /machine:ix86 

!ENDIF

# Begin Target

# Name "ui - Win32 vector"
# Name "ui - Win32 Release TA"
# Name "ui - Win32 Debug"
# Name "ui - Win32 Release"
# Name "ui - Win32 Debug TA"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\game\bg_lib.c

!IF  "$(CFG)" == "ui - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\game\bg_misc.c

!IF  "$(CFG)" == "ui - Win32 vector"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Release TA"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Debug TA"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /D "MISSIONPACK" /FR /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\qcommon\q_math.c
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\q_shared.c
# End Source File
# Begin Source File

SOURCE=..\..\ui\ui.def

!IF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\ui\ui_atoms.c

!IF  "$(CFG)" == "ui - Win32 vector"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Release TA"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Debug TA"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /D "MISSIONPACK" /FR /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\ui\ui_gameinfo.c

!IF  "$(CFG)" == "ui - Win32 vector"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Release TA"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Debug TA"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /D "MISSIONPACK" /FR /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\ui\ui_main.c

!IF  "$(CFG)" == "ui - Win32 vector"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Release TA"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Debug TA"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /D "MISSIONPACK" /FR /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\ui\ui_players.c

!IF  "$(CFG)" == "ui - Win32 vector"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Release TA"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Debug TA"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /D "MISSIONPACK" /FR /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\ui\ui_shared.c

!IF  "$(CFG)" == "ui - Win32 vector"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Release TA"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Debug TA"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /D "MISSIONPACK" /FR /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\ui\ui_syscalls.c

!IF  "$(CFG)" == "ui - Win32 vector"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Release TA"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /FR /GZ /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "ui - Win32 Debug TA"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /D "MISSIONPACK" /FR /GZ /GX 
!ENDIF

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\game\bg_public.h

!IF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\ui\keycodes.h

!IF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\..\ui\menudef.h

!IF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\game\q_shared.h

!IF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\game\surfaceflags.h

!IF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\cgame\tr_types.h

!IF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\ui\ui_local.h

!IF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\ui\ui_public.h

!IF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\ui\ui_shared.h

!IF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

