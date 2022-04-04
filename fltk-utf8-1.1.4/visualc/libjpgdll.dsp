# Microsoft Developer Studio Project File - Name="libjpegdll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libjpegdll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libjpgdll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libjpgdll.mak" CFG="libjpegdll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libjpegdll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libjpegdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libjpegdll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libjpegdll0"
# PROP BASE Intermediate_Dir "libjpegdll0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../test"
# PROP Intermediate_Dir "libjpegdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Os /Ob2 /I "." /I ".." /D "FL_DLL" /D "JPEG_DLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /FR /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 opengl32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"libjpegdll.pdb" /machine:I386 /out:"libjpgdll.dll" /implib:"../lib/libjpgdll.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "libjpegdll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libjpegdll1"
# PROP BASE Intermediate_Dir "libjpegdll1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../test"
# PROP Intermediate_Dir "libjpegdlld"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /GX /ZI /Od /I "." /I ".." /D "FL_DLL" /D "JPEG_DLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"libjpegdlld.pdb" /debug /machine:I386 /out:"libjpgdlld.dll" /implib:"../lib/libjpgdlld.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "libjpegdll - Win32 Release"
# Name "libjpegdll - Win32 Debug"
# Begin Source File

SOURCE=..\jpeg\jcapimin.c
DEP_CPP_JCAPI=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jcapistd.c
DEP_CPP_JCAPIS=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jccoefct.c
DEP_CPP_JCCOE=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jccolor.c
DEP_CPP_JCCOL=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jcdctmgr.c
DEP_CPP_JCDCT=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jdct.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jchuff.c
DEP_CPP_JCHUF=\
	"..\jpeg\jchuff.h"\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jcinit.c
DEP_CPP_JCINI=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jcmainct.c
DEP_CPP_JCMAI=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jcmarker.c
DEP_CPP_JCMAR=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jcmaster.c
DEP_CPP_JCMAS=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jcomapi.c
DEP_CPP_JCOMA=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jcparam.c
DEP_CPP_JCPAR=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jcphuff.c
DEP_CPP_JCPHU=\
	"..\jpeg\jchuff.h"\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jcprepct.c
DEP_CPP_JCPRE=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jcsample.c
DEP_CPP_JCSAM=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jctrans.c
DEP_CPP_JCTRA=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdapimin.c
DEP_CPP_JDAPI=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdapistd.c
DEP_CPP_JDAPIS=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdatadst.c
DEP_CPP_JDATA=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdatasrc.c
DEP_CPP_JDATAS=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdcoefct.c
DEP_CPP_JDCOE=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdcolor.c
DEP_CPP_JDCOL=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jddctmgr.c
DEP_CPP_JDDCT=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jdct.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdhuff.c
DEP_CPP_JDHUF=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jdhuff.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdinput.c
DEP_CPP_JDINP=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdmainct.c
DEP_CPP_JDMAI=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdmarker.c
DEP_CPP_JDMAR=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdmaster.c
DEP_CPP_JDMAS=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdmerge.c
DEP_CPP_JDMER=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdphuff.c
DEP_CPP_JDPHU=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jdhuff.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdpostct.c
DEP_CPP_JDPOS=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdsample.c
DEP_CPP_JDSAM=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jdtrans.c
DEP_CPP_JDTRA=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jerror.c
DEP_CPP_JERRO=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	"..\jpeg\jversion.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jfdctflt.c
DEP_CPP_JFDCT=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jdct.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jfdctfst.c
DEP_CPP_JFDCTF=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jdct.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jfdctint.c
DEP_CPP_JFDCTI=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jdct.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jidctflt.c
DEP_CPP_JIDCT=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jdct.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jidctfst.c
DEP_CPP_JIDCTF=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jdct.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jidctint.c
DEP_CPP_JIDCTI=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jdct.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jidctred.c
DEP_CPP_JIDCTR=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jdct.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jmemmgr.c
DEP_CPP_JMEMM=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmemsys.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jmemnobs.c
DEP_CPP_JMEMN=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmemsys.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jquant1.c
DEP_CPP_JQUAN=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jquant2.c
DEP_CPP_JQUANT=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\jpeg\jutils.c
DEP_CPP_JUTIL=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jconfig.vc"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jinclude.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	
# End Source File
# End Target
# End Project
