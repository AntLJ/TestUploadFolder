# Microsoft Developer Studio Project File - Name="SINDYBASE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SINDYBASE - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "SINDYBASE.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "SINDYBASE.mak" CFG="SINDYBASE - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "SINDYBASE - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "SINDYBASE - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SINDYBASE - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "..\WKH" /I "..\GDB" /I "../strcode/include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_STLP_NEW_PLATFORM_SDK" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\lib\sindybase.lib"

!ELSEIF  "$(CFG)" == "SINDYBASE - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\WKH" /I "..\GDB" /I "..\strcode\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_STLP_NEW_PLATFORM_SDK" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\lib\sindybased.lib"

!ENDIF 

# Begin Target

# Name "SINDYBASE - Win32 Release"
# Name "SINDYBASE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SindyAttrdef.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyDataset.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyDBConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyFeatureClass.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyInfTable.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyLineClass.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyLinkQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyLinkTable.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyLqTable.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyPointClass.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyPolygonClass.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyTable.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyTools.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\SindyAttrdef.h
# End Source File
# Begin Source File

SOURCE=.\SindyDataset.h
# End Source File
# Begin Source File

SOURCE=.\SindyDBConnection.h
# End Source File
# Begin Source File

SOURCE=.\SindyFeatureClass.h
# End Source File
# Begin Source File

SOURCE=.\SindyInfTable.h
# End Source File
# Begin Source File

SOURCE=.\SindyLineClass.h
# End Source File
# Begin Source File

SOURCE=.\SindyLinkQueue.h
# End Source File
# Begin Source File

SOURCE=.\SindyLinkTable.h
# End Source File
# Begin Source File

SOURCE=.\SindyLqTable.h
# End Source File
# Begin Source File

SOURCE=.\SindyPointClass.h
# End Source File
# Begin Source File

SOURCE=.\SindyPolygonClass.h
# End Source File
# Begin Source File

SOURCE=.\SindyTable.h
# End Source File
# Begin Source File

SOURCE=.\SindyTools.h
# End Source File
# End Group
# End Target
# End Project
