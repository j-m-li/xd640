# Microsoft Developer Studio Project File - Name="fltkdll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fltkdll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll.mak" CFG="fltkdll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltkdll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltkdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fltkdll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "fltkdll0"
# PROP BASE Intermediate_Dir "fltkdll0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../test"
# PROP Intermediate_Dir "fltkdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Os /Ob2 /I "." /I ".." /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 opengl32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdll.pdb" /machine:I386 /out:"fltkdll.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fltkdll1"
# PROP BASE Intermediate_Dir "fltkdll1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../test"
# PROP Intermediate_Dir "fltkdlld"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /GX /ZI /Od /I "." /I ".." /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdlld.pdb" /debug /machine:I386 /out:"fltkdlld.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "fltkdll - Win32 Release"
# Name "fltkdll - Win32 Debug"
# Begin Source File

SOURCE=..\src\filename_absolute.cxx
DEP_CPP_FILEN=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FILEN=\
	".\L\filename.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_expand.cxx
DEP_CPP_FILENA=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FILENA=\
	".\L\filename.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_ext.cxx
NODEP_CPP_FILENAM=\
	".\L\filename.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_isdir.cxx
DEP_CPP_FILENAME=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FILENAME=\
	".\L\filename.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_list.cxx
DEP_CPP_FILENAME_=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FILENAME_=\
	".\L\filename.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_match.cxx
NODEP_CPP_FILENAME_M=\
	".\L\filename.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_setext.cxx
DEP_CPP_FILENAME_S=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FILENAME_S=\
	".\L\filename.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl.cxx
DEP_CPP_FL_CX=\
	"..\fl\fl_tooltip.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_CX=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_abort.cxx
DEP_CPP_FL_AB=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_AB=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_add_idle.cxx
NODEP_CPP_FL_AD=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Adjuster.cxx
DEP_CPP_FL_ADJ=\
	"..\fl\fl_adjuster.h"\
	"..\src\fastarrow.h"\
	"..\src\mediumarrow.h"\
	"..\src\slowarrow.h"\
	
NODEP_CPP_FL_ADJ=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Bitmap.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_arc.cxx
DEP_CPP_FL_AR=\
	"..\FL\math.h"\
	
NODEP_CPP_FL_AR=\
	".\L\Enumerations.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_arci.cxx
DEP_CPP_FL_ARC=\
	"..\FL\math.h"\
	
NODEP_CPP_FL_ARC=\
	".\L\Enumerations.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_arg.cxx
DEP_CPP_FL_ARG=\
	"..\fl\fl_tooltip.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_ARG=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_ask.cxx
DEP_CPP_FL_AS=\
	"..\fl\fl_return_button.h"\
	"..\fl\fl_secret_input.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_AS=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\fl_ask.H"\
	".\l\Fl_Box.H"\
	".\l\Fl_Button.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Bitmap.cxx
DEP_CPP_FL_BI=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_BI=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Bitmap.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_BMP_Image.cxx
DEP_CPP_FL_BM=\
	"..\fl\fl_bmp_image.h"\
	".\config.h"\
	
NODEP_CPP_FL_BM=\
	".\L\Enumerations.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Box.cxx
NODEP_CPP_FL_BO=\
	".\L\Enumerations.H"\
	".\l\Fl_Box.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_boxtype.cxx
DEP_CPP_FL_BOX=\
	".\config.h"\
	
NODEP_CPP_FL_BOX=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser.cxx
DEP_CPP_FL_BR=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_BR=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Browser.H"\
	".\l\Fl_Browser_.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser_.cxx
NODEP_CPP_FL_BRO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Browser_.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser_load.cxx
NODEP_CPP_FL_BROW=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Browser.H"\
	".\l\Fl_Browser_.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Button.cxx
NODEP_CPP_FL_BU=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Button.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Chart.cxx
DEP_CPP_FL_CH=\
	"..\FL\math.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_CH=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Chart.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Check_Browser.cxx
DEP_CPP_FL_CHE=\
	"..\fl\fl_check_browser.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_CHE=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Browser_.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Check_Button.cxx
NODEP_CPP_FL_CHEC=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Button.H"\
	".\l\Fl_Check_Button.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Light_Button.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Choice.cxx
NODEP_CPP_FL_CHO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Choice.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Menu_.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Clock.cxx
NODEP_CPP_FL_CL=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Clock.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_color.cxx
DEP_CPP_FL_CO=\
	"..\src\fl_cmap.h"\
	"..\src\fl_color_mac.cxx"\
	"..\src\fl_color_win32.cxx"\
	"..\src\Fl_XColor.H"\
	".\config.h"\
	
NODEP_CPP_FL_CO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Color_Chooser.cxx
DEP_CPP_FL_COL=\
	"..\fl\fl_color_chooser.h"\
	"..\fl\fl_return_button.h"\
	"..\fl\fl_value_input.h"\
	"..\FL\math.h"\
	
NODEP_CPP_FL_COL=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Box.H"\
	".\l\Fl_Button.H"\
	".\l\Fl_Choice.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Menu_.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_compose.cxx
DEP_CPP_FL_COM=\
	"..\fl\fl_utf8.h"\
	
NODEP_CPP_FL_COM=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Counter.cxx
NODEP_CPP_FL_COU=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Counter.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_cursor.cxx
NODEP_CPP_FL_CU=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_curve.cxx
NODEP_CPP_FL_CUR=\
	".\L\Enumerations.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Dial.cxx
DEP_CPP_FL_DI=\
	"..\FL\math.h"\
	
NODEP_CPP_FL_DI=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Dial.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_diamond_box.cxx
NODEP_CPP_FL_DIA=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_display.cxx
DEP_CPP_FL_DIS=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_DIS=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_dnd.cxx
DEP_CPP_FL_DN=\
	"..\src\fl_dnd_mac.cxx"\
	"..\src\fl_dnd_win32.cxx"\
	"..\src\fl_dnd_x.cxx"\
	".\config.h"\
	
NODEP_CPP_FL_DN=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Double_Window.cxx
DEP_CPP_FL_DO=\
	".\config.h"\
	
NODEP_CPP_FL_DO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Double_Window.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw.cxx
DEP_CPP_FL_DR=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_DR=\
	".\L\Enumerations.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_image.cxx
DEP_CPP_FL_DRA=\
	"..\src\fl_draw_image_mac.cxx"\
	"..\src\fl_draw_image_win32.cxx"\
	"..\src\Fl_XColor.H"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_DRA=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_pixmap.cxx
DEP_CPP_FL_DRAW=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_DRAW=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_engraved_label.cxx
NODEP_CPP_FL_EN=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_File_Browser.cxx
DEP_CPP_FL_FI=\
	"..\FL\Fl_File_Browser.H"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_FI=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\l\Fl_Browser.H"\
	".\l\Fl_Browser_.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\L\Fl_File_Icon.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_File_Chooser.cxx
DEP_CPP_FL_FIL=\
	"..\FL\Fl_File_Browser.H"\
	"..\fl\fl_file_input.h"\
	"..\fl\fl_preferences.h"\
	"..\fl\fl_return_button.h"\
	"..\fl\fl_tile.h"\
	
NODEP_CPP_FL_FIL=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\l\fl_ask.H"\
	".\l\Fl_Bitmap.H"\
	".\l\Fl_Box.H"\
	".\l\Fl_Browser.H"\
	".\l\Fl_Browser_.H"\
	".\l\Fl_Button.H"\
	".\l\Fl_Check_Button.H"\
	".\l\Fl_Choice.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_File_Chooser.H"\
	".\L\Fl_File_Icon.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Light_Button.H"\
	".\l\Fl_Menu_.H"\
	".\l\Fl_Menu_Button.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_File_Chooser2.cxx
DEP_CPP_FL_FILE=\
	"..\FL\Fl_File_Browser.H"\
	"..\fl\fl_file_input.h"\
	"..\fl\fl_preferences.h"\
	"..\fl\fl_return_button.h"\
	"..\fl\fl_tile.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_FILE=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\l\fl_ask.H"\
	".\l\Fl_Box.H"\
	".\l\Fl_Browser.H"\
	".\l\Fl_Browser_.H"\
	".\l\Fl_Button.H"\
	".\l\Fl_Check_Button.H"\
	".\l\Fl_Choice.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_File_Chooser.H"\
	".\L\Fl_File_Icon.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Light_Button.H"\
	".\l\Fl_Menu_.H"\
	".\l\Fl_Menu_Button.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Scrollbar.H"\
	".\L\Fl_Shared_Image.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_file_dir.cxx
DEP_CPP_FL_FILE_=\
	"..\FL\Fl_File_Browser.H"\
	"..\fl\fl_file_input.h"\
	"..\fl\fl_preferences.h"\
	"..\fl\fl_return_button.h"\
	"..\fl\fl_tile.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_FILE_=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\l\fl_ask.H"\
	".\l\Fl_Box.H"\
	".\l\Fl_Browser.H"\
	".\l\Fl_Browser_.H"\
	".\l\Fl_Button.H"\
	".\l\Fl_Check_Button.H"\
	".\l\Fl_Choice.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_File_Chooser.H"\
	".\L\Fl_File_Icon.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Light_Button.H"\
	".\l\Fl_Menu_.H"\
	".\l\Fl_Menu_Button.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_File_Icon.cxx
DEP_CPP_FL_FILE_I=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_FILE_I=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\L\Fl_File_Icon.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_File_Icon2.cxx
DEP_CPP_FL_FILE_IC=\
	"..\FL\math.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_FILE_IC=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\L\Fl_File_Icon.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\L\Fl_Shared_Image.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_File_Input.cxx
DEP_CPP_FL_FILE_IN=\
	"..\fl\fl_file_input.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_FILE_IN=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_font.cxx
DEP_CPP_FL_FO=\
	"..\fl\fl_utf8.h"\
	"..\src\fl_font.h"\
	"..\src\fl_font_mac.cxx"\
	"..\src\fl_font_win32.cxx"\
	"..\src\fl_font_x.cxx"\
	"..\src\fl_font_xft.cxx"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_FO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Gdi.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_get_key.cxx
DEP_CPP_FL_GE=\
	"..\src\Fl_get_key_mac.cxx"\
	"..\src\fl_get_key_win32.cxx"\
	
NODEP_CPP_FL_GE=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_get_system_colors.cxx
DEP_CPP_FL_GET=\
	"..\fl\fl_tiled_image.h"\
	"..\FL\math.h"\
	"..\src\flstring.h"\
	"..\src\tile.xpm"\
	".\config.h"\
	
NODEP_CPP_FL_GET=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Pixmap.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_GIF_Image.cxx
DEP_CPP_FL_GI=\
	"..\fl\fl_gif_image.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_GI=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Pixmap.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Gl_Choice.cxx
DEP_CPP_FL_GL=\
	"..\FL\gl.h"\
	"..\src\Fl_Gl_Choice.H"\
	".\config.h"\
	
NODEP_CPP_FL_GL=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Gl_Overlay.cxx
DEP_CPP_FL_GL_=\
	"..\FL\gl.h"\
	"..\src\Fl_Gl_Choice.H"\
	".\config.h"\
	
NODEP_CPP_FL_GL_=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Gl_Window.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Gl_Window.cxx
DEP_CPP_FL_GL_W=\
	"..\FL\gl.h"\
	"..\src\Fl_Gl_Choice.H"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_GL_W=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Gl_Window.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_grab.cxx
DEP_CPP_FL_GR=\
	".\config.h"\
	
NODEP_CPP_FL_GR=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Group.cxx
NODEP_CPP_FL_GRO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Help_Dialog.cxx
DEP_CPP_FL_HE=\
	"..\FL\Fl_Help_Dialog.H"\
	"..\FL\Fl_Help_View.H"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_HE=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\fl_ask.H"\
	".\l\Fl_Button.H"\
	".\l\Fl_Double_Window.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Scrollbar.H"\
	".\L\Fl_Shared_Image.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Help_View.cxx
DEP_CPP_FL_HEL=\
	"..\FL\Fl_Help_View.H"\
	"..\fl\fl_utf8.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_HEL=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Pixmap.H"\
	".\l\Fl_Scrollbar.H"\
	".\L\Fl_Shared_Image.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Image.cxx
DEP_CPP_FL_IM=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_IM=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_images_core.cxx
DEP_CPP_FL_IMA=\
	"..\fl\fl_bmp_image.h"\
	"..\fl\fl_gif_image.h"\
	"..\fl\fl_jpeg_image.h"\
	"..\fl\fl_png_image.h"\
	"..\fl\fl_pnm_image.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_IMA=\
	".\L\Enumerations.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Pixmap.H"\
	".\L\Fl_Shared_Image.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input.cxx
DEP_CPP_FL_IN=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_IN=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\fl_ask.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input_.cxx
DEP_CPP_FL_INP=\
	"..\fl\fl_utf8.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_INP=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\fl_ask.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_JPEG_Image.cxx
DEP_CPP_FL_JP=\
	"..\fl\fl_jpeg_image.h"\
	".\config.h"\
	
NODEP_CPP_FL_JP=\
	".\L\Enumerations.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_labeltype.cxx
NODEP_CPP_FL_LA=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Light_Button.cxx
NODEP_CPP_FL_LI=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Button.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Light_Button.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_line_style.cxx
DEP_CPP_FL_LIN=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_LIN=\
	".\L\Enumerations.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_lock.cxx
DEP_CPP_FL_LO=\
	".\config.h"\
	
NODEP_CPP_FL_LO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu.cxx
DEP_CPP_FL_ME=\
	"..\fl\fl_menu_window.h"\
	
NODEP_CPP_FL_ME=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Menu_.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Single_Window.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_.cxx
DEP_CPP_FL_MEN=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_MEN=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Menu_.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_add.cxx
DEP_CPP_FL_MENU=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_MENU=\
	".\L\Enumerations.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Menu_.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Bar.cxx
NODEP_CPP_FL_MENU_=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Menu_.H"\
	".\L\Fl_Menu_Bar.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Button.cxx
NODEP_CPP_FL_MENU_B=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Menu_.H"\
	".\l\Fl_Menu_Button.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_global.cxx
NODEP_CPP_FL_MENU_G=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Menu_.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Window.cxx
DEP_CPP_FL_MENU_W=\
	"..\fl\fl_menu_window.h"\
	".\config.h"\
	
NODEP_CPP_FL_MENU_W=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Single_Window.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Multi_Label.cxx
DEP_CPP_FL_MU=\
	"..\fl\fl_multi_label.h"\
	
NODEP_CPP_FL_MU=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_oval_box.cxx
NODEP_CPP_FL_OV=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay.cxx
NODEP_CPP_FL_OVE=\
	".\L\Enumerations.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay_visual.cxx
DEP_CPP_FL_OVER=\
	".\config.h"\
	
NODEP_CPP_FL_OVER=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Overlay_Window.cxx
DEP_CPP_FL_OVERL=\
	"..\fl\fl_overlay_window.h"\
	".\config.h"\
	
NODEP_CPP_FL_OVERL=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Double_Window.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_own_colormap.cxx
DEP_CPP_FL_OW=\
	".\config.h"\
	
NODEP_CPP_FL_OW=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pack.cxx
DEP_CPP_FL_PA=\
	"..\fl\fl_pack.h"\
	
NODEP_CPP_FL_PA=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pixmap.cxx
DEP_CPP_FL_PI=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_PI=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Pixmap.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_plastic.cxx
DEP_CPP_FL_PL=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_PL=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_PNG_Image.cxx
DEP_CPP_FL_PN=\
	"..\fl\fl_png_image.h"\
	".\config.h"\
	
NODEP_CPP_FL_PN=\
	".\L\Enumerations.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_PNM_Image.cxx
DEP_CPP_FL_PNM=\
	"..\fl\fl_pnm_image.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_PNM=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Positioner.cxx
NODEP_CPP_FL_PO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Positioner.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Preferences.cxx
DEP_CPP_FL_PR=\
	"..\fl\fl_preferences.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_PR=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Progress.cxx
DEP_CPP_FL_PRO=\
	"..\fl\fl_progress.h"\
	
NODEP_CPP_FL_PRO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Ps.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_rect.cxx
NODEP_CPP_FL_RE=\
	".\L\Enumerations.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Repeat_Button.cxx
DEP_CPP_FL_REP=\
	"..\fl\fl_repeat_button.h"\
	
NODEP_CPP_FL_REP=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Button.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Return_Button.cxx
DEP_CPP_FL_RET=\
	"..\fl\fl_return_button.h"\
	
NODEP_CPP_FL_RET=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Button.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Roller.cxx
DEP_CPP_FL_RO=\
	"..\fl\fl_roller.h"\
	
NODEP_CPP_FL_RO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_round_box.cxx
NODEP_CPP_FL_ROU=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Round_Button.cxx
NODEP_CPP_FL_ROUN=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Button.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Light_Button.H"\
	".\l\Fl_Round_Button.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_rounded_box.cxx
NODEP_CPP_FL_ROUND=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scroll.cxx
DEP_CPP_FL_SC=\
	"..\fl\fl_scroll.h"\
	
NODEP_CPP_FL_SC=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_scroll_area.cxx
NODEP_CPP_FL_SCR=\
	".\L\Enumerations.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scrollbar.cxx
NODEP_CPP_FL_SCRO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_set_font.cxx
DEP_CPP_FL_SE=\
	"..\src\fl_font.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_SE=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_set_fonts.cxx
DEP_CPP_FL_SET=\
	"..\src\fl_font.h"\
	"..\src\fl_set_fonts_mac.cxx"\
	"..\src\fl_set_fonts_win32.cxx"\
	"..\src\fl_set_fonts_x.cxx"\
	"..\src\fl_set_fonts_xft.cxx"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_SET=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_shadow_box.cxx
NODEP_CPP_FL_SH=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Shared_Image.cxx
DEP_CPP_FL_SHA=\
	"..\fl\fl_xbm_image.h"\
	"..\fl\fl_xpm_image.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_SHA=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Bitmap.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Pixmap.H"\
	".\L\Fl_Shared_Image.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_shortcut.cxx
DEP_CPP_FL_SHO=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_SHO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Button.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_show_colormap.cxx
DEP_CPP_FL_SHOW=\
	".\config.h"\
	
NODEP_CPP_FL_SHOW=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\fl_show_colormap.H"\
	".\l\Fl_Single_Window.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Single_Window.cxx
NODEP_CPP_FL_SI=\
	".\L\Enumerations.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Single_Window.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Slider.cxx
NODEP_CPP_FL_SL=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_symbols.cxx
DEP_CPP_FL_SY=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_SY=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tabs.cxx
DEP_CPP_FL_TA=\
	"..\fl\fl_tabs.h"\
	
NODEP_CPP_FL_TA=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Buffer.cxx
DEP_CPP_FL_TE=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_TE=\
	".\L\Fl_Export.H"\
	".\l\Fl_Text_Buffer.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Display.cxx
DEP_CPP_FL_TEX=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_TEX=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Text_Buffer.H"\
	".\l\Fl_Text_Display.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Editor.cxx
DEP_CPP_FL_TEXT=\
	"..\fl\fl_text_editor.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_TEXT=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Text_Buffer.H"\
	".\l\Fl_Text_Display.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tile.cxx
DEP_CPP_FL_TI=\
	"..\fl\fl_tile.h"\
	
NODEP_CPP_FL_TI=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tiled_Image.cxx
DEP_CPP_FL_TIL=\
	"..\fl\fl_tiled_image.h"\
	
NODEP_CPP_FL_TIL=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tooltip.cxx
DEP_CPP_FL_TO=\
	"..\fl\fl_menu_window.h"\
	"..\fl\fl_tooltip.h"\
	
NODEP_CPP_FL_TO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Single_Window.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_utf8.cxx
DEP_CPP_FL_UT=\
	"..\fl\fl_utf8.h"\
	"..\src\spacing.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Valuator.cxx
DEP_CPP_FL_VA=\
	"..\FL\math.h"\
	
NODEP_CPP_FL_VA=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Input.cxx
DEP_CPP_FL_VAL=\
	"..\fl\fl_value_input.h"\
	"..\FL\math.h"\
	
NODEP_CPP_FL_VAL=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Output.cxx
DEP_CPP_FL_VALU=\
	"..\fl\fl_value_output.h"\
	
NODEP_CPP_FL_VALU=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Slider.cxx
NODEP_CPP_FL_VALUE=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Slider.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Value_Slider.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_vertex.cxx
DEP_CPP_FL_VE=\
	"..\FL\math.h"\
	
NODEP_CPP_FL_VE=\
	".\L\Enumerations.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_visual.cxx
DEP_CPP_FL_VI=\
	".\config.h"\
	
NODEP_CPP_FL_VI=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Widget.cxx
DEP_CPP_FL_WI=\
	"..\fl\fl_tooltip.h"\
	
NODEP_CPP_FL_WI=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window.cxx
NODEP_CPP_FL_WIN=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_fullscreen.cxx
NODEP_CPP_FL_WIND=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_hotspot.cxx
NODEP_CPP_FL_WINDO=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_iconize.cxx
NODEP_CPP_FL_WINDOW=\
	".\L\Enumerations.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Wizard.cxx
DEP_CPP_FL_WIZ=\
	"..\fl\fl_wizard.h"\
	
NODEP_CPP_FL_WIZ=\
	".\L\Enumerations.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_x.cxx
DEP_CPP_FL_X_=\
	"..\FL\Fl_Sys_Menu_Bar.H"\
	"..\fl\fl_tooltip.h"\
	"..\fl\fl_utf8.h"\
	"..\src\aimm.h"\
	"..\src\Fl_mac.cxx"\
	"..\src\fl_win32.cxx"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_X_=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Menu_.H"\
	".\L\Fl_Menu_Bar.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_XBM_Image.cxx
DEP_CPP_FL_XB=\
	"..\fl\fl_xbm_image.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_XB=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\l\Fl_Bitmap.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_XPM_Image.cxx
DEP_CPP_FL_XP=\
	"..\fl\fl_xpm_image.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FL_XP=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Pixmap.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\flstring.c
DEP_CPP_FLSTR=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FLSTR=\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Flv_CStyle.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Flv_Data_Source.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Flv_List.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Flv_Style.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Flv_Table.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Flve_Check_Button.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Flve_Combo.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Flve_Input.cxx
# End Source File
# Begin Source File

SOURCE=..\src\forms_bitmap.cxx
DEP_CPP_FORMS=\
	"..\FL\Fl_File_Browser.H"\
	"..\fl\fl_file_input.h"\
	"..\fl\fl_preferences.h"\
	"..\fl\fl_return_button.h"\
	"..\fl\fl_tile.h"\
	"..\fl\forms.h"\
	
NODEP_CPP_FORMS=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\l\fl_ask.H"\
	".\l\Fl_Bitmap.H"\
	".\l\Fl_Box.H"\
	".\l\Fl_Browser.H"\
	".\l\Fl_Browser_.H"\
	".\l\Fl_Button.H"\
	".\l\Fl_Chart.H"\
	".\l\Fl_Check_Button.H"\
	".\l\Fl_Choice.H"\
	".\l\Fl_Clock.H"\
	".\l\Fl_Counter.H"\
	".\l\Fl_Dial.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_File_Chooser.H"\
	".\L\Fl_File_Icon.H"\
	".\l\Fl_FormsBitmap.H"\
	".\l\Fl_FormsPixmap.H"\
	".\l\Fl_Free.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Light_Button.H"\
	".\l\Fl_Menu_.H"\
	".\l\Fl_Menu_Button.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Pixmap.H"\
	".\l\Fl_Positioner.H"\
	".\l\Fl_Round_Button.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\fl_show_colormap.H"\
	".\l\Fl_Slider.H"\
	".\L\Fl_Timer.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Value_Slider.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\forms_compatability.cxx
DEP_CPP_FORMS_=\
	"..\FL\Fl_File_Browser.H"\
	"..\fl\fl_file_input.h"\
	"..\fl\fl_preferences.h"\
	"..\fl\fl_repeat_button.h"\
	"..\fl\fl_return_button.h"\
	"..\fl\fl_tile.h"\
	"..\fl\forms.h"\
	
NODEP_CPP_FORMS_=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\l\fl_ask.H"\
	".\l\Fl_Bitmap.H"\
	".\l\Fl_Box.H"\
	".\l\Fl_Browser.H"\
	".\l\Fl_Browser_.H"\
	".\l\Fl_Button.H"\
	".\l\Fl_Chart.H"\
	".\l\Fl_Check_Button.H"\
	".\l\Fl_Choice.H"\
	".\l\Fl_Clock.H"\
	".\l\Fl_Counter.H"\
	".\l\Fl_Dial.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_File_Chooser.H"\
	".\L\Fl_File_Icon.H"\
	".\l\Fl_FormsBitmap.H"\
	".\l\Fl_FormsPixmap.H"\
	".\l\Fl_Free.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Light_Button.H"\
	".\l\Fl_Menu_.H"\
	".\l\Fl_Menu_Button.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Pixmap.H"\
	".\l\Fl_Positioner.H"\
	".\l\Fl_Round_Button.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\fl_show_colormap.H"\
	".\l\Fl_Slider.H"\
	".\L\Fl_Timer.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Value_Slider.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\forms_free.cxx
NODEP_CPP_FORMS_F=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Free.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\forms_fselect.cxx
DEP_CPP_FORMS_FS=\
	"..\FL\Fl_File_Browser.H"\
	"..\fl\fl_file_input.h"\
	"..\fl\fl_preferences.h"\
	"..\fl\fl_return_button.h"\
	"..\fl\fl_tile.h"\
	"..\fl\forms.h"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_FORMS_FS=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\l\fl_ask.H"\
	".\l\Fl_Bitmap.H"\
	".\l\Fl_Box.H"\
	".\l\Fl_Browser.H"\
	".\l\Fl_Browser_.H"\
	".\l\Fl_Button.H"\
	".\l\Fl_Chart.H"\
	".\l\Fl_Check_Button.H"\
	".\l\Fl_Choice.H"\
	".\l\Fl_Clock.H"\
	".\l\Fl_Counter.H"\
	".\l\Fl_Dial.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_File_Chooser.H"\
	".\L\Fl_File_Icon.H"\
	".\l\Fl_FormsBitmap.H"\
	".\l\Fl_FormsPixmap.H"\
	".\l\Fl_Free.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Light_Button.H"\
	".\l\Fl_Menu_.H"\
	".\l\Fl_Menu_Button.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Pixmap.H"\
	".\l\Fl_Positioner.H"\
	".\l\Fl_Round_Button.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\fl_show_colormap.H"\
	".\l\Fl_Slider.H"\
	".\L\Fl_Timer.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Value_Slider.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\forms_pixmap.cxx
DEP_CPP_FORMS_P=\
	"..\FL\Fl_File_Browser.H"\
	"..\fl\fl_file_input.h"\
	"..\fl\fl_preferences.h"\
	"..\fl\fl_return_button.h"\
	"..\fl\fl_tile.h"\
	"..\fl\forms.h"\
	
NODEP_CPP_FORMS_P=\
	".\L\Enumerations.H"\
	".\L\filename.H"\
	".\l\Fl.H"\
	".\l\fl_ask.H"\
	".\l\Fl_Bitmap.H"\
	".\l\Fl_Box.H"\
	".\l\Fl_Browser.H"\
	".\l\Fl_Browser_.H"\
	".\l\Fl_Button.H"\
	".\l\Fl_Chart.H"\
	".\l\Fl_Check_Button.H"\
	".\l\Fl_Choice.H"\
	".\l\Fl_Clock.H"\
	".\l\Fl_Counter.H"\
	".\l\Fl_Dial.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_File_Chooser.H"\
	".\L\Fl_File_Icon.H"\
	".\l\Fl_FormsBitmap.H"\
	".\l\Fl_FormsPixmap.H"\
	".\l\Fl_Free.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\l\Fl_Input.H"\
	".\l\Fl_Input_.H"\
	".\l\Fl_Light_Button.H"\
	".\l\Fl_Menu_.H"\
	".\l\Fl_Menu_Button.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Pixmap.H"\
	".\l\Fl_Positioner.H"\
	".\l\Fl_Round_Button.H"\
	".\l\Fl_Scrollbar.H"\
	".\l\fl_show_colormap.H"\
	".\l\Fl_Slider.H"\
	".\L\Fl_Timer.H"\
	".\l\Fl_Valuator.H"\
	".\l\Fl_Value_Slider.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\forms_timer.cxx
NODEP_CPP_FORMS_T=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\L\Fl_Timer.H"\
	".\l\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\gl_draw.cxx
DEP_CPP_GL_DR=\
	"..\fl\fl_utf8.h"\
	"..\FL\gl.h"\
	"..\src\fl_font.h"\
	"..\src\Fl_Gl_Choice.H"\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_GL_DR=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\gl_start.cxx
DEP_CPP_GL_ST=\
	"..\FL\gl.h"\
	"..\src\Fl_Gl_Choice.H"\
	".\config.h"\
	
NODEP_CPP_GL_ST=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\fl_draw.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\glut_compatability.cxx
DEP_CPP_GLUT_=\
	"..\FL\gl.h"\
	"..\fl\glut.h"\
	".\config.h"\
	
NODEP_CPP_GLUT_=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Gl_Window.H"\
	".\l\Fl_Group.H"\
	".\L\Fl_Image.H"\
	".\L\Fl_Menu_Item.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	".\L\mac.H"\
	".\L\win32.H"\
	".\L\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\glut_font.cxx
DEP_CPP_GLUT_F=\
	"..\FL\gl.h"\
	"..\fl\glut.h"\
	".\config.h"\
	
NODEP_CPP_GLUT_F=\
	".\L\Enumerations.H"\
	".\l\Fl.H"\
	".\L\Fl_Export.H"\
	".\l\Fl_Gl_Window.H"\
	".\l\Fl_Group.H"\
	".\l\Fl_Widget.H"\
	".\l\Fl_Window.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\numericsort.c
DEP_CPP_NUMER=\
	".\config.h"\
	
NODEP_CPP_NUMER=\
	".\L\filename.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\scandir.c
DEP_CPP_SCAND=\
	"..\src\flstring.h"\
	"..\src\scandir_win32.c"\
	".\config.h"\
	
NODEP_CPP_SCAND=\
	".\L\filename.H"\
	".\L\Fl_Export.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\vsnprintf.c
DEP_CPP_VSNPR=\
	"..\src\flstring.h"\
	".\config.h"\
	
NODEP_CPP_VSNPR=\
	".\L\Fl_Export.H"\
	
# End Source File
# End Target
# End Project
