# Microsoft Developer Studio Project File - Name="test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=test - Win32 Debug for Arc9
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "test.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "test.mak" CFG="test - Win32 Debug for Arc9"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "test - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "test - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE "test - Win32 FinalCheck" ("Win32 (x86) Console Application" 用)
!MESSAGE "test - Win32 Debug for Arc9" ("Win32 (x86) Console Application" 用)
!MESSAGE "test - Win32 Release for Arc9" ("Win32 (x86) Console Application" 用)
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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 cppunit.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
TargetPath=.\Release\test.exe
SOURCE="$(InputPath)"
PostBuild_Desc=ﾃｽﾄ中...
PostBuild_Cmds=$(TargetPath)
# End Special Build Tool

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 cppunitd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cppunitd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "test - Win32 Debug for Arc9"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "test___Win32_Debug_for_Arc9"
# PROP BASE Intermediate_Dir "test___Win32_Debug_for_Arc9"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugArc9"
# PROP Intermediate_Dir "DebugArc9"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../" /D "_USE_ARC9" /D "_STLP_NEW_PLATFORM_SDK" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 cppunitd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cppunitd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "test - Win32 Release for Arc9"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "test___Win32_Release_for_Arc9"
# PROP BASE Intermediate_Dir "test___Win32_Release_for_Arc9"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseArc9"
# PROP Intermediate_Dir "ReleaseArc9"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../../../" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../" /D "_USE_ARC9" /D "_STLP_NEW_PLATFORM_SDK" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 cppunit.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 cppunit.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# Begin Special Build Tool
TargetPath=.\ReleaseArc9\test.exe
SOURCE="$(InputPath)"
PostBuild_Desc=ﾃｽﾄ中...
PostBuild_Cmds=$(TargetPath)
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "test - Win32 Release"
# Name "test - Win32 Debug"
# Name "test - Win32 FinalCheck"
# Name "test - Win32 Debug for Arc9"
# Name "test - Win32 Release for Arc9"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DeleteIteratorTest.cpp

!IF  "$(CFG)" == "test - Win32 Release"

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

!ELSEIF  "$(CFG)" == "test - Win32 FinalCheck"

# SUBTRACT BASE CPP /WX
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "test - Win32 Debug for Arc9"

# SUBTRACT BASE CPP /WX
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "test - Win32 Release for Arc9"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FieldIndexTest.cpp

!IF  "$(CFG)" == "test - Win32 Release"

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

!ELSEIF  "$(CFG)" == "test - Win32 FinalCheck"

# SUBTRACT BASE CPP /WX
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "test - Win32 Debug for Arc9"

# SUBTRACT BASE CPP /WX
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "test - Win32 Release for Arc9"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FieldOIDTest.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldShapeTest.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldValueModifierTest.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldValueTest.cpp
# End Source File
# Begin Source File

SOURCE=.\GeometryTest.cpp
# End Source File
# Begin Source File

SOURCE=.\PGDBCreatorTest.cpp

!IF  "$(CFG)" == "test - Win32 Release"

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

!ELSEIF  "$(CFG)" == "test - Win32 FinalCheck"

# SUBTRACT BASE CPP /WX
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "test - Win32 Debug for Arc9"

# SUBTRACT BASE CPP /WX
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "test - Win32 Release for Arc9"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RandomAccessIteratorTest.cpp
# End Source File
# Begin Source File

SOURCE=.\SinglePassIteratorTest.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateAlgorithmTest.cpp

!IF  "$(CFG)" == "test - Win32 Release"

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

!ELSEIF  "$(CFG)" == "test - Win32 FinalCheck"

# SUBTRACT BASE CPP /WX
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "test - Win32 Debug for Arc9"

# SUBTRACT BASE CPP /WX
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "test - Win32 Release for Arc9"

!ENDIF 

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
# Begin Group "arctl"

# PROP Default_Filter ""
# Begin Group "field"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\field\dmodifier.h
# End Source File
# Begin Source File

SOURCE=..\..\field\index.h
# End Source File
# Begin Source File

SOURCE=..\..\field\modifier.h
# End Source File
# Begin Source File

SOURCE=..\..\field\oid.h
# End Source File
# Begin Source File

SOURCE=..\..\field\shape.h
# End Source File
# Begin Source File

SOURCE=..\..\field\value.h
# End Source File
# End Group
# Begin Group "iterator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\iterator\add_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\iterator\delete_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\iterator\iterator_cc.h
# End Source File
# Begin Source File

SOURCE=..\..\iterator\iterators.h
# End Source File
# Begin Source File

SOURCE=..\..\iterator\single_pass_iterators.h
# End Source File
# Begin Source File

SOURCE=..\..\iterator\zero_based_random_access_iterators.h
# End Source File
# End Group
# Begin Group "detail"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\detail\multi_inherit.h
# End Source File
# End Group
# Begin Group "specif"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\specif\field.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\algorithm.h
# End Source File
# Begin Source File

SOURCE=..\..\class.h
# End Source File
# Begin Source File

SOURCE=..\..\coinitializer.h
# End Source File
# Begin Source File

SOURCE=..\..\cursor.h
# End Source File
# Begin Source File

SOURCE=..\..\field.h
# End Source File
# Begin Source File

SOURCE=..\..\geometry.h
# End Source File
# Begin Source File

SOURCE=..\..\iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\object_massacrer.h
# End Source File
# Begin Source File

SOURCE=..\..\pgdb.h
# End Source File
# Begin Source File

SOURCE=..\..\topological.h
# End Source File
# End Group
# Begin Group "atl2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\atl2\algorithm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\atl2\iterator\element_wrapper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\atl2\exception.h
# End Source File
# Begin Source File

SOURCE=..\..\..\atl2\iterator\iterator_base.h
# End Source File
# Begin Source File

SOURCE=..\..\..\atl2\seal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\atl2\iterator\single_pass_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\atl2\type_traits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\atl2\iterator\zero_based_random_access_iterator.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Misc.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
