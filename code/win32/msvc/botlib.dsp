# Microsoft Developer Studio Project File - Name="botlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=botlib - Win32 Debug TA
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "botlib.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "botlib.mak" CFG="botlib - Win32 Debug TA"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "botlib - Win32 Debug TA" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "botlib - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "botlib - Win32 vector" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "botlib - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "botlib - Win32 Release TA" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "botlib - Win32 Release_noOpenAL" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\botlib_debug_ta"
# PROP BASE Intermediate_Dir ".\build\botlib_debug_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\botlib_debug_ta"
# PROP Intermediate_Dir ".\build\botlib_debug_ta"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "BOTLIB" /D "DEBUG" /D "_MBCS" /FR /Fp"$(IntDir)\botlib.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "BOTLIB" /D "DEBUG" /D "_MBCS" /FR /Fp"$(IntDir)\botlib.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\botlib_release"
# PROP BASE Intermediate_Dir ".\build\botlib_release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\botlib_release"
# PROP Intermediate_Dir ".\build\botlib_release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "BOTLIB" /D "_MBCS" /Fp"$(IntDir)\botlib.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "BOTLIB" /D "_MBCS" /Fp"Release\botlib.pch" /YX /Fo"Release\\" /GF /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\botlib_vector0"
# PROP BASE Intermediate_Dir ".\build\botlib_vector0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\botlib_vector0"
# PROP Intermediate_Dir ".\build\botlib_vector0"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "BOTLIB" /D "_MBCS" /Fp"$(IntDir)\botlib.pch" /YX /Fo"$(IntDir)\\" /GF /vec /vec:stats /vec:pii /vec:o10 /vec:yes /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "BOTLIB" /D "_MBCS" /Fp"$(IntDir)\botlib.pch" /YX /Fo"$(IntDir)\\" /GF /vec /vec:stats /vec:pii /vec:o10 /vec:yes /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\botlib_debug"
# PROP BASE Intermediate_Dir ".\build\botlib_debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\botlib_debug"
# PROP Intermediate_Dir ".\build\botlib_debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "BOTLIB" /D "DEBUG" /D "_MBCS" /FR /Fp"$(IntDir)\botlib.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "BOTLIB" /D "DEBUG" /D "_MBCS" /FR /Fp"Debug\botlib.pch" /YX /Fo"Debug\\" /Fd"Debug\\" /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\botlib_release_ta"
# PROP BASE Intermediate_Dir ".\build\botlib_release_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\botlib_release_ta"
# PROP Intermediate_Dir ".\build\botlib_release_ta"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "BOTLIB" /D "_MBCS" /Fp"$(IntDir)\botlib.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "BOTLIB" /D "_MBCS" /Fp"$(IntDir)\botlib.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "botlib___Win32_Release_noOpenAL"
# PROP BASE Intermediate_Dir "botlib___Win32_Release_noOpenAL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "botlib___Win32_Release_noOpenAL"
# PROP Intermediate_Dir "botlib___Win32_Release_noOpenAL"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "BOTLIB" /D "_MBCS" /Fp"Release\botlib.pch" /YX /Fo"Release\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "BOTLIB" /D "_MBCS" /Fp"Release\botlib.pch" /YX /Fo"Release\\" /GF /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "botlib - Win32 Debug TA"
# Name "botlib - Win32 Release"
# Name "botlib - Win32 vector"
# Name "botlib - Win32 Debug"
# Name "botlib - Win32 Release TA"
# Name "botlib - Win32 Release_noOpenAL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\botlib\be_aas_bspq3.c
DEP_CPP_BE_AA=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_cluster.c
DEP_CPP_BE_AAS=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_debug.c
DEP_CPP_BE_AAS_=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_entity.c
DEP_CPP_BE_AAS_E=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_file.c
DEP_CPP_BE_AAS_F=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_main.c
DEP_CPP_BE_AAS_M=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_move.c
DEP_CPP_BE_AAS_MO=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_optimize.c
DEP_CPP_BE_AAS_O=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_reach.c
DEP_CPP_BE_AAS_R=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_route.c
DEP_CPP_BE_AAS_RO=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_crc.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_routealt.c
DEP_CPP_BE_AAS_ROU=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_sample.c
DEP_CPP_BE_AAS_S=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_ai_char.c
DEP_CPP_BE_AI=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_ai_char.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_ai_chat.c
DEP_CPP_BE_AI_=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_ai_chat.h"\
	"..\..\botlib\be_ea.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_ai_gen.c
DEP_CPP_BE_AI_G=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_ai_gen.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_ai_goal.c
DEP_CPP_BE_AI_GO=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_ai_goal.h"\
	"..\..\botlib\be_ai_move.h"\
	"..\..\botlib\be_ai_weight.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_ai_move.c
DEP_CPP_BE_AI_M=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_ai_goal.h"\
	"..\..\botlib\be_ai_move.h"\
	"..\..\botlib\be_ea.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_ai_weap.c
DEP_CPP_BE_AI_W=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_ai_weap.h"\
	"..\..\botlib\be_ai_weight.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_ai_weight.c
DEP_CPP_BE_AI_WE=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_ai_weight.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_ea.c
DEP_CPP_BE_EA=\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_interface.c
DEP_CPP_BE_IN=\
	"..\..\botlib\aasfile.h"\
	"..\..\botlib\be_aas.h"\
	"..\..\botlib\be_aas_bsp.h"\
	"..\..\botlib\be_aas_cluster.h"\
	"..\..\botlib\be_aas_debug.h"\
	"..\..\botlib\be_aas_def.h"\
	"..\..\botlib\be_aas_entity.h"\
	"..\..\botlib\be_aas_file.h"\
	"..\..\botlib\be_aas_funcs.h"\
	"..\..\botlib\be_aas_main.h"\
	"..\..\botlib\be_aas_move.h"\
	"..\..\botlib\be_aas_optimize.h"\
	"..\..\botlib\be_aas_reach.h"\
	"..\..\botlib\be_aas_route.h"\
	"..\..\botlib\be_aas_routealt.h"\
	"..\..\botlib\be_aas_sample.h"\
	"..\..\botlib\be_ai_char.h"\
	"..\..\botlib\be_ai_chat.h"\
	"..\..\botlib\be_ai_gen.h"\
	"..\..\botlib\be_ai_goal.h"\
	"..\..\botlib\be_ai_move.h"\
	"..\..\botlib\be_ai_weap.h"\
	"..\..\botlib\be_ai_weight.h"\
	"..\..\botlib\be_ea.h"\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_crc.c
DEP_CPP_L_CRC=\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_libvar.c
DEP_CPP_L_LIB=\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_log.c
DEP_CPP_L_LOG=\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_memory.c
DEP_CPP_L_MEM=\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_log.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_precomp.c
DEP_CPP_L_PRE=\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	
NODEP_CPP_L_PRE=\
	"..\..\botlib\qcc.h"\
	"..\..\bspc\l_log.h"\
	"..\..\bspc\l_mem.h"\
	"..\..\bspc\qbsp.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_script.c
DEP_CPP_L_SCR=\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_libvar.h"\
	"..\..\botlib\l_log.h"\
	"..\..\botlib\l_memory.h"\
	"..\..\botlib\l_script.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	
NODEP_CPP_L_SCR=\
	"..\..\botlib\qcc.h"\
	"..\..\bspc\l_log.h"\
	"..\..\bspc\l_mem.h"\
	"..\..\bspc\qbsp.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_struct.c
DEP_CPP_L_STR=\
	"..\..\botlib\be_interface.h"\
	"..\..\botlib\botlib.h"\
	"..\..\botlib\l_precomp.h"\
	"..\..\botlib\l_script.h"\
	"..\..\botlib\l_struct.h"\
	"..\..\botlib\l_utils.h"\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\surfaceflags.h"\
	
NODEP_CPP_L_STR=\
	"..\..\bspc\l_log.h"\
	"..\..\bspc\l_mem.h"\
	"..\..\bspc\qbsp.h"\
	

!IF  "$(CFG)" == "botlib - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "botlib - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "botlib - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\botlib\aasfile.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_bsp.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_cluster.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_debug.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_def.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_entity.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_file.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_funcs.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_main.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_move.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_optimize.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_reach.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_route.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_routealt.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_aas_sample.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_ai_weight.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\be_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\game\bg_public.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\cm_public.h
# End Source File
# Begin Source File

SOURCE=..\\..\game\g_public.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_crc.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_libvar.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_log.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_memory.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_precomp.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_script.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_struct.h
# End Source File
# Begin Source File

SOURCE=..\..\botlib\l_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\qcommon.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\qfiles.h
# End Source File
# Begin Source File

SOURCE=..\\..\server\server.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\surfaceflags.h
# End Source File
# End Group
# End Target
# End Project
