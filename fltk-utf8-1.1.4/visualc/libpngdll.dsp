# Microsoft Developer Studio Project File - Name="libpngdll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libpngdll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libpngdll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libpngdll.mak" CFG="libpngdll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libpngdll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libpngdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libpngdll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libpngdll0"
# PROP BASE Intermediate_Dir "libpngdll0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "libpngdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Os /Ob2 /I "." /I ".." /I "..\zlib" /D "FL_DLL" /D "PNG_BUILD_DLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 opengl32.lib zlibdll.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"libpngdll.pdb" /machine:I386 /out:"libpngdll.dll" /libpath:"../lib/" /libpath:".."
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "libpngdll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libpngdll1"
# PROP BASE Intermediate_Dir "libpngdll1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "libpngdlld"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /GX /ZI /Od /I "." /I ".." /I "..\zlib" /D "FL_DLL" /D "PNG_BUILD_DLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib zlibdlld.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"libpngdlld.pdb" /debug /machine:I386 /out:"libpngdlld.dll" /pdbtype:sept /libpath:"../lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "libpngdll - Win32 Release"
# Name "libpngdll - Win32 Debug"
# Begin Source File

SOURCE=..\png\png.c
DEP_CPP_PNG_C=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNG_C=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngerror.c
DEP_CPP_PNGER=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGER=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pnggccrd.c
DEP_CPP_PNGGC=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGGC=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngget.c
DEP_CPP_PNGGE=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGGE=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngmem.c
DEP_CPP_PNGME=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGME=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngpread.c
DEP_CPP_PNGPR=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGPR=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngread.c
DEP_CPP_PNGRE=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGRE=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngrio.c
DEP_CPP_PNGRI=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGRI=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngrtran.c
DEP_CPP_PNGRT=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGRT=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngrutil.c
DEP_CPP_PNGRU=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGRU=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngset.c
DEP_CPP_PNGSE=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGSE=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngtrans.c
DEP_CPP_PNGTR=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGTR=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngvcrd.c
DEP_CPP_PNGVC=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGVC=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngwio.c
DEP_CPP_PNGWI=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGWI=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngwrite.c
DEP_CPP_PNGWR=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGWR=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngwtran.c
DEP_CPP_PNGWT=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGWT=\
	"..\png\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\png\pngwutil.c
DEP_CPP_PNGWU=\
	"..\png\png.h"\
	"..\png\pngconf.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
NODEP_CPP_PNGWU=\
	"..\png\alloc.h"\
	
# End Source File
# End Target
# End Project
