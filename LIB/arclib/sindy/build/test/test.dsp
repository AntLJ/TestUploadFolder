# Microsoft Developer Studio Project File - Name="test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=test - Win32 FinalCheck
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "test.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "test.mak" CFG="test - Win32 FinalCheck"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "test - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "test - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE "test - Win32 FinalCheck" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "test___Win32_Release"
# PROP BASE Intermediate_Dir "test___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../" /I "../../../../../" /D "NDEBUG" /D "_USE_SINDYLIB_STATIC" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 cppunit.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# Begin Special Build Tool
TargetPath=.\Release\test.exe
SOURCE="$(InputPath)"
PostBuild_Desc=ﾃｽﾄ中...
PostBuild_Cmds=$(TargetPath)
# End Special Build Tool

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "test___Win32_Debug"
# PROP BASE Intermediate_Dir "test___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../" /I "../../../../../" /D "_DEBUG" /D "_USE_SINDYLIB_STATIC" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cppunitd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "test - Win32 FinalCheck"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "test___Win32_FinalCheck"
# PROP BASE Intermediate_Dir "test___Win32_FinalCheck"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FinalCheck"
# PROP Intermediate_Dir "FinalCheck"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../" /I "../../../../../" /D "_DEBUG" /D "_USE_SINDYLIB_STATIC" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../" /I "../../../../../" /D "_DEBUG" /D "_USE_SINDYLIB_STATIC" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 sindylibmd.lib cppunitd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../../../../lib"
# ADD LINK32 cppunitd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "test - Win32 Release"
# Name "test - Win32 Debug"
# Name "test - Win32 FinalCheck"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "impl"

# PROP Default_Filter ""
# End Group
# Begin Group "SiNDYLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\SiNDYCommonAttrCommunity.cpp
# End Source File
# End Group
# Begin Group "Test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BidirectionalLqRelationTest.cpp
# End Source File
# Begin Source File

SOURCE=.\BidirectionalLqRelationTestBase.h
# End Source File
# Begin Source File

SOURCE=.\DirectionalLqRelationWatcherTest.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgModifyStamperTest.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeStamperTest.cpp
# End Source File
# Begin Source File

SOURCE=.\TransformToNqTest.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\test.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "sindy"

# PROP Default_Filter ""
# Begin Group "algorithm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\algorithm\transform_to_nq.h
# End Source File
# End Group
# Begin Group "stamper"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\stamper\modify_date_stamper.h
# End Source File
# Begin Source File

SOURCE=..\..\stamper\modify_stamper.h
# End Source File
# Begin Source File

SOURCE=..\..\stamper\prog_modify_stamper.h
# End Source File
# Begin Source File

SOURCE=..\..\stamper\time_stamper.h
# End Source File
# Begin Source File

SOURCE=..\..\stamper\unary_modify_stamper.h
# End Source File
# End Group
# Begin Group "lq_relation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lq_relation\bidirectional.h
# End Source File
# Begin Source File

SOURCE=..\..\lq_relation\directional_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\lq_relation\directional_watcher.h
# End Source File
# Begin Source File

SOURCE=..\..\lq_relation\edit_watcher.h
# End Source File
# Begin Source File

SOURCE=..\..\lq_relation\interface.h
# End Source File
# Begin Source File

SOURCE=..\..\lq_relation\watcher.h
# End Source File
# End Group
# Begin Group "link_queue"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\link_queue\lq.h
# End Source File
# Begin Source File

SOURCE=..\..\link_queue\lq_list.h
# End Source File
# Begin Source File

SOURCE=..\..\link_queue\table.h
# End Source File
# End Group
# Begin Group "modifier"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\modifier\stamp_modifier.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\lq_relation.h
# End Source File
# Begin Source File

SOURCE=..\..\stamper.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
