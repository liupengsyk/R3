# Microsoft Developer Studio Project File - Name="OfilmLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OfilmLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OfilmLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OfilmLib.mak" CFG="OfilmLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OfilmLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OfilmLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OfilmLib - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /fo"OfilmLib.res" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib"

!ELSEIF  "$(CFG)" == "OfilmLib - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /fo"OfilmLib.res" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "OfilmLib - Win32 Release"
# Name "OfilmLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DllDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DNPIndexDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\global.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5Checksum.cpp
# End Source File
# Begin Source File

SOURCE=.\MesLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\OfilmLib.cpp
# End Source File
# Begin Source File

SOURCE=.\OfilmLib.def
# End Source File
# Begin Source File

SOURCE=.\OfilmLib.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UnlockDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DllDialog.h
# End Source File
# Begin Source File

SOURCE=.\DNPIndexDlg.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\MD5Checksum.h
# End Source File
# Begin Source File

SOURCE=.\MD5ChecksumDefines.h
# End Source File
# Begin Source File

SOURCE=.\MesLogin.h
# End Source File
# Begin Source File

SOURCE=.\OfilmLib.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UnlockDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\114.bmp
# End Source File
# Begin Source File

SOURCE=.\res\116.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HAND.CUR
# End Source File
# Begin Source File

SOURCE=.\res\hand_v.cur
# End Source File
# Begin Source File

SOURCE=.\res\OfilmLib.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section OfilmLib : {39547AD4-831A-42AA-8D56-6283137C2675}
# 	1:9:IDD_LOGIN:108
# 	2:16:Resource Include:resource.h
# 	2:16:CLASS: CMesLogin:CMesLogin
# 	2:10:ENUM: enum:enum
# 	2:10:MesLogin.h:MesLogin.h
# 	2:12:MesLogin.cpp:MesLogin.cpp
# 	2:19:Application Include:OfilmLib.h
# 	2:9:IDD_LOGIN:IDD_LOGIN
# End Section
# Section OfilmLib : {1AF5EE6F-18EA-4174-AB2C-02B8B22A3156}
# 	1:19:IDD_DIALOG_DNPINDEX:102
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:19:IDD_DIALOG_DNPINDEX:IDD_DIALOG_DNPINDEX
# 	2:19:Application Include:OfilmLib.h
# 	2:13:DNPIndexDlg.h:DNPIndexDlg.h
# 	2:19:CLASS: CDNPIndexDlg:CDNPIndexDlg
# 	2:15:DNPIndexDlg.cpp:DNPIndexDlg.cpp
# End Section
# Section OfilmLib : {2917A889-BDFC-48CA-9336-BA21694BC4B9}
# 	1:17:IDD_UNLOCK_DIALOG:107
# 	2:16:Resource Include:resource.h
# 	2:17:CLASS: CUnlockDlg:CUnlockDlg
# 	2:17:IDD_UNLOCK_DIALOG:IDD_UNLOCK_DIALOG
# 	2:11:UnlockDlg.h:UnlockDlg.h
# 	2:10:ENUM: enum:enum
# 	2:13:UnlockDlg.cpp:UnlockDlg.cpp
# 	2:19:Application Include:OfilmLib.h
# End Section
