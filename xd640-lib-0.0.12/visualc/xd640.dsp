# Microsoft Developer Studio Project File - Name="xd640" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xd640 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xd640.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xd640.mak" CFG="xd640 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xd640 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "xd640 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xd640 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\xd640" /I ".." /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D PACKAGE=\"xd640\" /YX /FD /c
# ADD BASE RSC /l 0x100c /d "NDEBUG"
# ADD RSC /l 0x100c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\xd640.lib"

!ELSEIF  "$(CFG)" == "xd640 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\xd640" /I ".." /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D PACKAGE=\"xd640\" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x100c /d "_DEBUG"
# ADD RSC /l 0x100c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\xd640d.lib"

!ENDIF 

# Begin Target

# Name "xd640 - Win32 Release"
# Name "xd640 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\Xd6Base64.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6CfgParser.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6CheckButton.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6ConfigFile.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6DefaultFonts.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6FindDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6Gif.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlBlock.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlDisplay.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlDownload.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlFrame_Drawings.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlFrame_Edit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlFrame_Export.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlFrame_Handlers.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlLine.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlPrintWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlScroll.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlSegment.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlTagA.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlTagButton.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlTagForm.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlTagImg.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlTagInputText.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlTagLi.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlTagTable.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlTagTd.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlToRtf.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6HtmlView.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6IconWindowWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6SpellChoice.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6StdWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6Tabulator.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6TextParser.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6VirtualKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6XmlDtd.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6XmlParser.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6XmlStyle.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6XmlTree.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Xd6XmlUtils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\entities.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6Base64.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6CfgParser.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6CheckButton.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6ConfigFile.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6DefaultFonts.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6FindDialog.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6Gif.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlBlock.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlDisplay.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlDownload.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlFrame.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlLine.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlPrint.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlScroll.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlSegment.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlTagA.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlTagButton.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlTagForm.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlTagImg.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlTagInputText.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlTagLi.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlTagTable.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlTagTd.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlToRtf.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6HtmlView.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6IconWindow.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6SpellChoice.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6Std.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6System.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6Tabulator.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6TextParser.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6VirtualKeyboard.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6XmlDtd.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6XmlParser.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6XmlStyle.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6XmlTree.h
# End Source File
# Begin Source File

SOURCE=..\xd640\Xd6XmlUtils.h
# End Source File
# End Group
# End Target
# End Project
