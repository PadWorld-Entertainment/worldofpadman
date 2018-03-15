# Microsoft Developer Studio Project File - Name="renderer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=renderer - Win32 Debug TA
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "renderer.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "renderer.mak" CFG="renderer - Win32 Debug TA"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "renderer - Win32 Debug TA" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "renderer - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "renderer - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "renderer - Win32 Release TA" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "renderer - Win32 vector" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "renderer - Win32 Release_noOpenAL" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\renderer_debug_ta"
# PROP BASE Intermediate_Dir ".\build\renderer_debug_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\renderer_debug_ta"
# PROP Intermediate_Dir ".\build\renderer_debug_ta"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /FR /Fp"$(IntDir)\renderer.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /FR /Fp"$(IntDir)\renderer.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\renderer_release"
# PROP BASE Intermediate_Dir ".\build\renderer_release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\renderer_release"
# PROP Intermediate_Dir ".\build\renderer_release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /Fp"$(IntDir)\renderer.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /Fp"Release\renderer.pch" /YX /Fo"Release\\" /GF /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\build\renderer_debug"
# PROP BASE Intermediate_Dir ".\build\renderer_debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\build\renderer_debug"
# PROP Intermediate_Dir ".\build\renderer_debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /FR /Fp"$(IntDir)\renderer.pch" /YX /Fo"$(IntDir)\\" /Fd"$(IntDir)\\" /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /FR /Fp"Debug\renderer.pch" /YX /Fo"Debug\\" /Fd"Debug\\" /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\renderer_release_ta"
# PROP BASE Intermediate_Dir ".\build\renderer_release_ta"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\renderer_release_ta"
# PROP Intermediate_Dir ".\build\renderer_release_ta"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /Fp"$(IntDir)\renderer.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /Fp"$(IntDir)\renderer.pch" /YX /Fo"$(IntDir)\\" /GF /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\build\renderer_vector0"
# PROP BASE Intermediate_Dir ".\build\renderer_vector0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\build\renderer_vector0"
# PROP Intermediate_Dir ".\build\renderer_vector0"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /Fp"$(IntDir)\renderer.pch" /YX /Fo"$(IntDir)\\" /GF /vec /vec:stats /vec:pii /vec:o10 /vec:yes /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /Fp"$(IntDir)\renderer.pch" /YX /Fo"$(IntDir)\\" /GF /vec /vec:stats /vec:pii /vec:o10 /vec:yes /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "renderer___Win32_Release_noOpenAL"
# PROP BASE Intermediate_Dir "renderer___Win32_Release_noOpenAL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "renderer___Win32_Release_noOpenAL"
# PROP Intermediate_Dir "renderer___Win32_Release_noOpenAL"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /Fp"Release\renderer.pch" /YX /Fo"Release\\" /GF /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /Fp"Release\renderer.pch" /YX /Fo"Release\\" /GF /c
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

# Name "renderer - Win32 Debug TA"
# Name "renderer - Win32 Release"
# Name "renderer - Win32 Debug"
# Name "renderer - Win32 Release TA"
# Name "renderer - Win32 vector"
# Name "renderer - Win32 Release_noOpenAL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\renderer\ref_trin.def
# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_animation.c
DEP_CPP_TR_AN=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_AN=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_backend.c
DEP_CPP_TR_BA=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_BA=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_bsp.c
DEP_CPP_TR_BS=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_BS=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_cmds.c
DEP_CPP_TR_CM=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_CM=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_curve.c
DEP_CPP_TR_CU=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_CU=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_flares.c
DEP_CPP_TR_FL=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_FL=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_font.c
DEP_CPP_TR_FO=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_FO=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_image.c
DEP_CPP_TR_IM=\
	"..\..\game\bg_lib.h"\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_IM=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_init.c
DEP_CPP_TR_IN=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_IN=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_light.c
DEP_CPP_TR_LI=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_LI=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_main.c
DEP_CPP_TR_MA=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_MA=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_marks.c
DEP_CPP_TR_MAR=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_MAR=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_mesh.c
DEP_CPP_TR_ME=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_ME=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_model.c
DEP_CPP_TR_MO=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_MO=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_noise.c
DEP_CPP_TR_NO=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_NO=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_scene.c
DEP_CPP_TR_SC=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_SC=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_shade.c
DEP_CPP_TR_SH=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_SH=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_shade_calc.c
DEP_CPP_TR_SHA=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_SHA=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_shader.c
DEP_CPP_TR_SHAD=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_SHAD=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_shadows.c
DEP_CPP_TR_SHADO=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_SHADO=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_sky.c
DEP_CPP_TR_SK=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_SK=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_surface.c
DEP_CPP_TR_SU=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_SU=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_world.c
DEP_CPP_TR_WO=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_TR_WO=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\win32\win_gamma.c
DEP_CPP_WIN_G=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\glw_win.h"\
	"..\win_local.h"\
	
NODEP_CPP_WIN_G=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\win32\win_glimp.c
DEP_CPP_WIN_GL=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\glw_win.h"\
	"..\resource.h"\
	"..\win_local.h"\
	
NODEP_CPP_WIN_GL=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\win32\win_qgl.c
DEP_CPP_WIN_Q=\
	"..\..\game\bg_lib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	"..\glw_win.h"\
	
NODEP_CPP_WIN_Q=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\qcommon\cm_public.h
# End Source File
# Begin Source File

SOURCE=..\..\win32\glw_win.h
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

SOURCE=..\..\renderer\qgl.h
# End Source File
# Begin Source File

SOURCE=..\..\qcommon\surfaceflags.h
# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_local.h
# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_public.h
# End Source File
# Begin Source File

SOURCE=..\..\renderer\tr_types.h
# End Source File
# Begin Source File

SOURCE=..\..\win32\win_local.h
# End Source File
# End Group
# Begin Group "jpeg"

# PROP Default_Filter ""
# Begin Group "Source Files No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\jpeg-6\jcapimin.c"
DEP_CPP_JCAPI=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jccoefct.c"
DEP_CPP_JCCOE=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jccolor.c"
DEP_CPP_JCCOL=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jcdctmgr.c"
DEP_CPP_JCDCT=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jdct.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jchuff.c"
DEP_CPP_JCHUF=\
	"..\..\jpeg-6\jchuff.h"\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jcinit.c"
DEP_CPP_JCINI=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jcmainct.c"
DEP_CPP_JCMAI=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jcmarker.c"
DEP_CPP_JCMAR=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jcmaster.c"
DEP_CPP_JCMAS=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jcomapi.c"
DEP_CPP_JCOMA=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jcparam.c"
DEP_CPP_JCPAR=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jcphuff.c"
DEP_CPP_JCPHU=\
	"..\..\jpeg-6\jchuff.h"\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jcprepct.c"
DEP_CPP_JCPRE=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jcsample.c"
DEP_CPP_JCSAM=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jctrans.c"
DEP_CPP_JCTRA=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdapimin.c"
DEP_CPP_JDAPI=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdapistd.c"
DEP_CPP_JDAPIS=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdatadst.c"
DEP_CPP_JDATA=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdatasrc.c"
DEP_CPP_JDATAS=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdcoefct.c"
DEP_CPP_JDCOE=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdcolor.c"
DEP_CPP_JDCOL=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jddctmgr.c"
DEP_CPP_JDDCT=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jdct.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdhuff.c"
DEP_CPP_JDHUF=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jdhuff.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdinput.c"
DEP_CPP_JDINP=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdmainct.c"
DEP_CPP_JDMAI=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdmarker.c"
DEP_CPP_JDMAR=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdmaster.c"
DEP_CPP_JDMAS=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdpostct.c"
DEP_CPP_JDPOS=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdsample.c"
DEP_CPP_JDSAM=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdtrans.c"
DEP_CPP_JDTRA=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jerror.c"
DEP_CPP_JERRO=\
	"..\..\game\bg_lib.h"\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	"..\..\jpeg-6\jversion.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_JERRO=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jfdctflt.c"
DEP_CPP_JFDCT=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jdct.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jidctflt.c"
DEP_CPP_JIDCT=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jdct.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jmemmgr.c"
DEP_CPP_JMEMM=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmemsys.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jmemnobs.c"
DEP_CPP_JMEMN=\
	"..\..\game\bg_lib.h"\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmemsys.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	"..\..\qcommon\cm_public.h"\
	"..\..\qcommon\q_platform.h"\
	"..\..\qcommon\q_shared.h"\
	"..\..\qcommon\qcommon.h"\
	"..\..\qcommon\qfiles.h"\
	"..\..\qcommon\surfaceflags.h"\
	"..\..\renderer\qgl.h"\
	"..\..\renderer\qgl_linked.h"\
	"..\..\renderer\tr_local.h"\
	"..\..\renderer\tr_public.h"\
	"..\..\renderer\tr_types.h"\
	
NODEP_CPP_JMEMN=\
	"..\..\renderer\macosx_local.h"\
	"..\..\renderer\macosx_qgl.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jutils.c"
DEP_CPP_JUTIL=\
	"..\..\jpeg-6\jconfig.h"\
	"..\..\jpeg-6\jerror.h"\
	"..\..\jpeg-6\jinclude.h"\
	"..\..\jpeg-6\jmorecfg.h"\
	"..\..\jpeg-6\jpegint.h"\
	"..\..\jpeg-6\jpeglib.h"\
	

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# ADD CPP /nologo /GX /Od /FR /GZ

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# ADD BASE CPP /nologo /GX /O2
# ADD CPP /nologo /GX /O2

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\jpeg-6\jchuff.h"
# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jconfig.h"
# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdct.h"
# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jdhuff.h"
# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jerror.h"
# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jinclude.h"
# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jmemsys.h"
# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jmorecfg.h"
# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jpegint.h"
# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jpeglib.h"
# End Source File
# Begin Source File

SOURCE="..\..\jpeg-6\jversion.h"
# End Source File
# End Group
# End Group
# Begin Group "FreeType2"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Include files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\ft2\ahangles.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahglobal.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahglyph.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahhint.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahloader.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahmodule.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahoptim.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahtypes.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\autohint.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\freetype.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftbbox.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftcalc.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftconfig.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftdebug.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftdriver.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\fterrors.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftextend.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftglyph.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftimage.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftlist.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftmemory.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftmm.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftmodule.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftnames.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftobjs.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftoption.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftoutln.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftrender.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftstream.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftsystem.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\fttypes.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\psnames.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\sfnt.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\t1errors.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\t1tables.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\t1types.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\t2errors.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\t2types.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttdriver.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\tterrors.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttgload.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttinterp.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttnameid.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttobjs.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttpload.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\tttables.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\tttags.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\tttypes.h

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\ft2\ahangles.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahglobal.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahglyph.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahhint.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahmodule.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ahoptim.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftcalc.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftdebug.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftextend.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftglyph.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftgrays.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftinit.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftlist.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftmm.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftnames.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftobjs.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftoutln.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftraster.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftrend1.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftsmooth.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftstream.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ftsystem.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\sfdriver.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\sfobjs.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttcmap.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttdriver.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttgload.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttinterp.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttload.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttobjs.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttpload.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttpost.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ft2\ttsbit.c

!IF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 vector"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "renderer - Win32 Release_noOpenAL"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
