# Microsoft Developer Studio Project File - Name="libdb" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libdb - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libdb.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libdb.mak" CFG="libdb - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libdb - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libdb - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libdb - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /GX /Ot /Op /Ob2 /I "." /I ".." /I "../db" /I "../db/port/win32/include" /D "NDEBUG" /D "FL_STATIC" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "__DBINTERFACE_PRIVATE" /D "NO_POSIX_SIGNALS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\libdb.lib"

!ELSEIF  "$(CFG)" == "libdb - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /GX /Z7 /Od /I "." /I ".." /I "../db" /I "../db/port/win32/include" /D "_DEBUG" /D "FL_STATIC" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "__DBINTERFACE_PRIVATE" /D "NO_POSIX_SIGNALS" /D "DEBUG" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\libdbd.lib"

!ENDIF 

# Begin Target

# Name "libdb - Win32 Release"
# Name "libdb - Win32 Debug"
# Begin Source File

SOURCE=..\db\btree\bt_close.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_conv.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_debug.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_delete.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_get.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_open.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_overflow.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_page.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_put.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_search.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_seq.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_split.c
# End Source File
# Begin Source File

SOURCE=..\db\btree\bt_utils.c
# End Source File
# Begin Source File

SOURCE=..\db\db\db.c
# End Source File
# Begin Source File

SOURCE=..\db\hash\hash.c
# End Source File
# Begin Source File

SOURCE=..\db\hash\hash_bigkey.c
# End Source File
# Begin Source File

SOURCE=..\db\hash\hash_buf.c
# End Source File
# Begin Source File

SOURCE=..\db\hash\hash_func.c
# End Source File
# Begin Source File

SOURCE=..\db\hash\hash_log2.c
# End Source File
# Begin Source File

SOURCE=..\db\hash\hash_page.c
# End Source File
# Begin Source File

SOURCE=..\db\hash\hsearch.c
# End Source File
# Begin Source File

SOURCE=..\db\PORT\win32\lseek.c
# End Source File
# Begin Source File

SOURCE=..\db\mpool\mpool.c
# End Source File
# Begin Source File

SOURCE=..\db\recno\rec_close.c
# End Source File
# Begin Source File

SOURCE=..\db\recno\rec_delete.c
# End Source File
# Begin Source File

SOURCE=..\db\recno\rec_get.c
# End Source File
# Begin Source File

SOURCE=..\db\recno\rec_open.c
# End Source File
# Begin Source File

SOURCE=..\db\recno\rec_put.c
# End Source File
# Begin Source File

SOURCE=..\db\recno\rec_search.c
# End Source File
# Begin Source File

SOURCE=..\db\recno\rec_seq.c
# End Source File
# Begin Source File

SOURCE=..\db\recno\rec_utils.c
# End Source File
# End Target
# End Project
