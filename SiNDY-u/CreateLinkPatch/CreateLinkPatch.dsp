# Microsoft Developer Studio Project File - Name="CreateLinkPatch" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=CreateLinkPatch - Win32 Debug for Arc9
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "CreateLinkPatch.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "CreateLinkPatch.mak" CFG="CreateLinkPatch - Win32 Debug for Arc9"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "CreateLinkPatch - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "CreateLinkPatch - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE "CreateLinkPatch - Win32 Debug for Arc9" ("Win32 (x86) Console Application" 用)
!MESSAGE "CreateLinkPatch - Win32 Release for Arc9" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CreateLinkPatch - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 STRCODE.LIB wkh.lib crd_cnv.lib libzcl.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /libpath:"../../LIB/WKH/lib" /libpath:"../../LIB/ZCL/lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "CreateLinkPatch - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 STRCODE.LIB wkhd.lib crd_cnvd.lib libzcld.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../../LIB/WKH/lib" /libpath:"../../LIB/ZCL/lib"

!ELSEIF  "$(CFG)" == "CreateLinkPatch - Win32 Debug for Arc9"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CreateLinkPatch___Win32_Debug_for_Arc9"
# PROP BASE Intermediate_Dir "CreateLinkPatch___Win32_Debug_for_Arc9"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugArc9"
# PROP Intermediate_Dir "DebugArc9"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "D:\WinCVS\SiNDY-u\PROG\SINDY\SINDY2INROAD" /I "D:\WinCVS\SiNDY-u\LIBS\SINDYDBINFO" /I "D:\WinCVS\SiNDY-u\LIBS\SINDYMAP" /I "D:\WinCVS\SiNDY-u\LIBS\SINDYBASE" /I "D:\WinCVS\SiNDY-u\LIBS\GDB" /I "D:\WinCVS\SiNDY-u\zou\zin2" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_USE_ARC9" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 STRCODE.LIB wkhd.lib crd_cnvd.lib libzcld.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wkhd.lib crd_cnv_vc60-d.lib libzcld.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../../LIB/WKH/lib" /libpath:"../../LIB/ZCL/lib"

!ELSEIF  "$(CFG)" == "CreateLinkPatch - Win32 Release for Arc9"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CreateLinkPatch___Win32_Release_for_Arc9"
# PROP BASE Intermediate_Dir "CreateLinkPatch___Win32_Release_for_Arc9"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseArc9"
# PROP Intermediate_Dir "ReleaseArc9"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "D:\WinCVS\SiNDY-u\PROG\SINDY\SINDY2INROAD" /I "D:\WinCVS\SiNDY-u\LIBS\SINDYDBINFO" /I "D:\WinCVS\SiNDY-u\LIBS\SINDYMAP" /I "D:\WinCVS\SiNDY-u\LIBS\SINDYBASE" /I "D:\WinCVS\SiNDY-u\LIBS\GDB" /I "D:\WinCVS\SiNDY-u\zou\zin2" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_USE_ARC9" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 STRCODE.LIB wkh.lib crd_cnv.lib libzcl.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 wkh.lib crd_cnv_vc60.lib libzcl.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /libpath:"../../LIB/WKH/lib" /libpath:"../../LIB/ZCL/lib"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "CreateLinkPatch - Win32 Release"
# Name "CreateLinkPatch - Win32 Debug"
# Name "CreateLinkPatch - Win32 Debug for Arc9"
# Name "CreateLinkPatch - Win32 Release for Arc9"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Data Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AppropriateAttribute.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfirmedLink.cpp
# End Source File
# Begin Source File

SOURCE=.\EtcLinkList.cpp
# End Source File
# Begin Source File

SOURCE=.\HighwayNodeCollection.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\RoadLinkCollection.cpp
# End Source File
# End Group
# Begin Group "Utility Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CmdLineArg.cpp
# End Source File
# Begin Source File

SOURCE=.\DBAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\DBHandleBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DBInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CreateLinkPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\version.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Utility Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CmdLineArg.h
# End Source File
# Begin Source File

SOURCE=.\DBAccess.h
# End Source File
# Begin Source File

SOURCE=.\DBHandleBase.h
# End Source File
# Begin Source File

SOURCE=.\DBInfo.h
# End Source File
# Begin Source File

SOURCE=.\GlobalFunction.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# End Group
# Begin Group "Data Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AppropriateAttribute.h
# End Source File
# Begin Source File

SOURCE=.\ConfirmedLink.h
# End Source File
# Begin Source File

SOURCE=.\EtcLinkList.h
# End Source File
# Begin Source File

SOURCE=.\HighwayNodeCollection.h
# End Source File
# Begin Source File

SOURCE=.\LinkPatch.h
# End Source File
# Begin Source File

SOURCE=.\RoadLinkCollection.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CreateLinkPatch.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "External Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GDBError.h
# End Source File
# Begin Source File

SOURCE=.\INAttrDef.h
# End Source File
# Begin Source File

SOURCE=.\SINDY2INCODE.cpp
# End Source File
# Begin Source File

SOURCE=.\SINDY2INCODE.h
# End Source File
# Begin Source File

SOURCE=.\SindyAttrdef.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyAttrdef.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
