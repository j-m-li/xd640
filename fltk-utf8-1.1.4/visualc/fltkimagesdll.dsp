# Microsoft Developer Studio Project File - Name="fltkimagesdll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fltkimagesdll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltkimagesdll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltkimagesdll.mak" CFG="fltkimagesdll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltkimagesdll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltkimagesdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fltkimagesdll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "fltkimagesdll0"
# PROP BASE Intermediate_Dir "fltkimagesdll0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../test"
# PROP Intermediate_Dir "fltkimagesdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Os /Ob2 /I "." /I ".." /I "..\zlib" /I "..\libpng" /I "..\jpeg" /D "NDEBUG" /D "FL_DLL" /D "FL_IMAGES_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 opengl32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkimagesdll.pdb" /machine:I386 /out:"fltkutf8imagesdll.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltkimagesdll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fltkimagesdll1"
# PROP BASE Intermediate_Dir "fltkimagesdll1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../test"
# PROP Intermediate_Dir "fltkimagesdlld"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /GX /ZI /Od /I "." /I ".." /I "..\zlib" /I "..\libpng" /I "..\jpeg" /D "_DEBUG" /D "FL_DLL" /D "FL_IMAGES_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkimagesdlld.pdb" /debug /machine:I386 /out:"fltkutf8imagesdlld.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "fltkimagesdll - Win32 Release"
# Name "fltkimagesdll - Win32 Debug"
# Begin Source File

SOURCE=..\src\Fl_BMP_Image.cxx
DEP_CPP_FL_BM=\
	"..\fl\fl_bmp_image.h"\
	".\config.h"\
	{$(INCLUDE)}"fl\Enumerations.H"\
	{$(INCLUDE)}"FL\Fl_Device.H"\
	{$(INCLUDE)}"fl\Fl_Export.H"\
	{$(INCLUDE)}"FL\Fl_Fltk.H"\
	{$(INCLUDE)}"fl\Fl_Group.H"\
	{$(INCLUDE)}"fl\Fl_Image.H"\
	{$(INCLUDE)}"FL\fl_utf8.H"\
	{$(INCLUDE)}"fl\Fl_Widget.H"\
	{$(INCLUDE)}"fl\Fl_Window.H"\
	{$(INCLUDE)}"fl\mac.H"\
	{$(INCLUDE)}"fl\win32.H"\
	{$(INCLUDE)}"fl\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_File_Icon2.cxx
DEP_CPP_FL_FI=\
	"..\FL\filename.H"\
	"..\FL\Fl_File_Icon.H"\
	"..\FL\math.h"\
	"..\src\flstring.h"\
	".\config.h"\
	{$(INCLUDE)}"fl\Enumerations.H"\
	{$(INCLUDE)}"FL\Fl.H"\
	{$(INCLUDE)}"FL\Fl_Device.H"\
	{$(INCLUDE)}"FL\fl_draw.H"\
	{$(INCLUDE)}"fl\Fl_Export.H"\
	{$(INCLUDE)}"FL\Fl_Fltk.H"\
	{$(INCLUDE)}"fl\Fl_Group.H"\
	{$(INCLUDE)}"fl\Fl_Image.H"\
	{$(INCLUDE)}"FL\Fl_Shared_Image.H"\
	{$(INCLUDE)}"FL\fl_utf8.H"\
	{$(INCLUDE)}"fl\Fl_Widget.H"\
	{$(INCLUDE)}"fl\Fl_Window.H"\
	{$(INCLUDE)}"fl\mac.H"\
	{$(INCLUDE)}"fl\win32.H"\
	{$(INCLUDE)}"fl\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_GIF_Image.cxx
DEP_CPP_FL_GI=\
	"..\fl\fl_gif_image.h"\
	"..\src\flstring.h"\
	".\config.h"\
	{$(INCLUDE)}"fl\Enumerations.H"\
	{$(INCLUDE)}"FL\Fl.H"\
	{$(INCLUDE)}"FL\Fl_Device.H"\
	{$(INCLUDE)}"fl\Fl_Export.H"\
	{$(INCLUDE)}"FL\Fl_Fltk.H"\
	{$(INCLUDE)}"fl\Fl_Group.H"\
	{$(INCLUDE)}"fl\Fl_Image.H"\
	{$(INCLUDE)}"fl\Fl_Pixmap.H"\
	{$(INCLUDE)}"FL\fl_utf8.H"\
	{$(INCLUDE)}"fl\Fl_Widget.H"\
	{$(INCLUDE)}"fl\Fl_Window.H"\
	{$(INCLUDE)}"fl\mac.H"\
	{$(INCLUDE)}"fl\win32.H"\
	{$(INCLUDE)}"fl\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Help_Dialog.cxx
DEP_CPP_FL_HE=\
	"..\FL\fl_ask.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Double_Window.H"\
	"..\FL\Fl_Help_Dialog.H"\
	"..\FL\Fl_Help_View.H"\
	"..\src\flstring.h"\
	".\config.h"\
	{$(INCLUDE)}"fl\Enumerations.H"\
	{$(INCLUDE)}"FL\Fl.H"\
	{$(INCLUDE)}"FL\Fl_Device.H"\
	{$(INCLUDE)}"FL\fl_draw.H"\
	{$(INCLUDE)}"fl\Fl_Export.H"\
	{$(INCLUDE)}"FL\Fl_Fltk.H"\
	{$(INCLUDE)}"fl\Fl_Group.H"\
	{$(INCLUDE)}"fl\Fl_Image.H"\
	{$(INCLUDE)}"FL\Fl_Scrollbar.H"\
	{$(INCLUDE)}"FL\Fl_Shared_Image.H"\
	{$(INCLUDE)}"FL\Fl_Slider.H"\
	{$(INCLUDE)}"FL\fl_utf8.H"\
	{$(INCLUDE)}"FL\Fl_Valuator.H"\
	{$(INCLUDE)}"fl\Fl_Widget.H"\
	{$(INCLUDE)}"fl\Fl_Window.H"\
	{$(INCLUDE)}"fl\mac.H"\
	{$(INCLUDE)}"fl\win32.H"\
	{$(INCLUDE)}"fl\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_images_core.cxx
DEP_CPP_FL_IM=\
	"..\fl\fl_bmp_image.h"\
	"..\fl\fl_gif_image.h"\
	"..\fl\fl_jpeg_image.h"\
	"..\fl\fl_png_image.h"\
	"..\fl\fl_pnm_image.h"\
	"..\src\flstring.h"\
	".\config.h"\
	{$(INCLUDE)}"fl\Enumerations.H"\
	{$(INCLUDE)}"FL\Fl_Device.H"\
	{$(INCLUDE)}"fl\Fl_Export.H"\
	{$(INCLUDE)}"FL\Fl_Fltk.H"\
	{$(INCLUDE)}"fl\Fl_Group.H"\
	{$(INCLUDE)}"fl\Fl_Image.H"\
	{$(INCLUDE)}"fl\Fl_Pixmap.H"\
	{$(INCLUDE)}"FL\Fl_Shared_Image.H"\
	{$(INCLUDE)}"fl\Fl_Widget.H"\
	{$(INCLUDE)}"fl\Fl_Window.H"\
	{$(INCLUDE)}"fl\mac.H"\
	{$(INCLUDE)}"fl\win32.H"\
	{$(INCLUDE)}"fl\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_JPEG_Image.cxx
DEP_CPP_FL_JP=\
	"..\fl\fl_jpeg_image.h"\
	".\config.h"\
	{$(INCLUDE)}"fl\Enumerations.H"\
	{$(INCLUDE)}"FL\Fl_Device.H"\
	{$(INCLUDE)}"fl\Fl_Export.H"\
	{$(INCLUDE)}"FL\Fl_Fltk.H"\
	{$(INCLUDE)}"fl\Fl_Group.H"\
	{$(INCLUDE)}"fl\Fl_Image.H"\
	{$(INCLUDE)}"FL\fl_utf8.H"\
	{$(INCLUDE)}"fl\Fl_Widget.H"\
	{$(INCLUDE)}"fl\Fl_Window.H"\
	{$(INCLUDE)}"fl\mac.H"\
	{$(INCLUDE)}"fl\win32.H"\
	{$(INCLUDE)}"fl\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_PNG_Image.cxx
DEP_CPP_FL_PN=\
	"..\fl\fl_png_image.h"\
	"..\libpng\png.h"\
	".\config.h"\
	{$(INCLUDE)}"fl\Enumerations.H"\
	{$(INCLUDE)}"FL\Fl_Device.H"\
	{$(INCLUDE)}"fl\Fl_Export.H"\
	{$(INCLUDE)}"FL\Fl_Fltk.H"\
	{$(INCLUDE)}"fl\Fl_Group.H"\
	{$(INCLUDE)}"fl\Fl_Image.H"\
	{$(INCLUDE)}"FL\fl_utf8.H"\
	{$(INCLUDE)}"fl\Fl_Widget.H"\
	{$(INCLUDE)}"fl\Fl_Window.H"\
	{$(INCLUDE)}"fl\mac.H"\
	{$(INCLUDE)}"fl\win32.H"\
	{$(INCLUDE)}"fl\x.H"\
	{$(INCLUDE)}"libpng\pngconf.h"\
	
NODEP_CPP_FL_PN=\
	"..\libpng\alloc.h"\
	"..\libpng\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_PNM_Image.cxx
DEP_CPP_FL_PNM=\
	"..\fl\fl_pnm_image.h"\
	"..\src\flstring.h"\
	".\config.h"\
	{$(INCLUDE)}"fl\Enumerations.H"\
	{$(INCLUDE)}"FL\Fl.H"\
	{$(INCLUDE)}"FL\Fl_Device.H"\
	{$(INCLUDE)}"fl\Fl_Export.H"\
	{$(INCLUDE)}"FL\Fl_Fltk.H"\
	{$(INCLUDE)}"fl\Fl_Group.H"\
	{$(INCLUDE)}"fl\Fl_Image.H"\
	{$(INCLUDE)}"FL\fl_utf8.H"\
	{$(INCLUDE)}"fl\Fl_Widget.H"\
	{$(INCLUDE)}"fl\Fl_Window.H"\
	{$(INCLUDE)}"fl\mac.H"\
	{$(INCLUDE)}"fl\win32.H"\
	{$(INCLUDE)}"fl\x.H"\
	
# End Source File
# End Target
# End Project
