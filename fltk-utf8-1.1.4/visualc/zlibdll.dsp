# Microsoft Developer Studio Project File - Name="zlibdll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=zlibdll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zlibdll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zlibdll.mak" CFG="zlibdll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zlibdll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zlibdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zlibdll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "zlibdll0"
# PROP BASE Intermediate_Dir "zlibdll0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../test"
# PROP Intermediate_Dir "zlibdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Os /Ob2 /I "." /I ".." /D "NDEBUG" /D "FL_DLL" /D "ZLIB_DLL" /D "Z_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 opengl32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:yes /pdb:"zlibdll.pdb" /map /machine:I386 /out:"zlibdll.dll" /implib:"../lib/zlibdll.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "zlibdll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "zlibdll1"
# PROP BASE Intermediate_Dir "zlibdll1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../test"
# PROP Intermediate_Dir "zlibdlld"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /GX /ZI /Od /I "." /I ".." /D "_DEBUG" /D "FL_DLL" /D "ZLIB_DLL" /D "Z_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"zlibdlld.pdb" /debug /machine:I386 /out:"zlibdlld.dll" /implib:"../lib/zlibdlld.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "zlibdll - Win32 Release"
# Name "zlibdll - Win32 Debug"
# Begin Source File

SOURCE=..\zlib\adler32.c
DEP_CPP_ADLER=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\compress.c
DEP_CPP_COMPR=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\crc32.c
DEP_CPP_CRC32=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\deflate.c
DEP_CPP_DEFLA=\
	"..\zlib\deflate.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	"..\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\gzio.c
DEP_CPP_GZIO_=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	"..\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\infblock.c
DEP_CPP_INFBL=\
	"..\zlib\infblock.h"\
	"..\zlib\infcodes.h"\
	"..\zlib\inftrees.h"\
	"..\zlib\infutil.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	"..\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\infcodes.c
DEP_CPP_INFCO=\
	"..\zlib\infblock.h"\
	"..\zlib\infcodes.h"\
	"..\zlib\inffast.h"\
	"..\zlib\inftrees.h"\
	"..\zlib\infutil.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	"..\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\inffast.c
DEP_CPP_INFFA=\
	"..\zlib\infblock.h"\
	"..\zlib\infcodes.h"\
	"..\zlib\inffast.h"\
	"..\zlib\inftrees.h"\
	"..\zlib\infutil.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	"..\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\inflate.c
DEP_CPP_INFLA=\
	"..\zlib\infblock.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	"..\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\inftrees.c
DEP_CPP_INFTR=\
	"..\zlib\inffixed.h"\
	"..\zlib\inftrees.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	"..\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\infutil.c
DEP_CPP_INFUT=\
	"..\zlib\infblock.h"\
	"..\zlib\infcodes.h"\
	"..\zlib\inftrees.h"\
	"..\zlib\infutil.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	"..\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\maketree.c
DEP_CPP_MAKET=\
	"..\zlib\inftrees.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	"..\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\trees.c
DEP_CPP_TREES=\
	"..\zlib\deflate.h"\
	"..\zlib\trees.h"\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	"..\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\uncompr.c
DEP_CPP_UNCOM=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\zlib\zutil.c
DEP_CPP_ZUTIL=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	"..\zlib\zutil.h"\
	
# End Source File
# End Target
# End Project
