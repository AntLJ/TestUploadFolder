# Microsoft Developer Studio Project File - Name="OCILib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=OCILib - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "OCILib.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "OCILib.mak" CFG="OCILib - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "OCILib - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "OCILib - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OCILib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "../oci/include" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_UNICODE" /D "_STLP_NEW_PLATFORM_SDK" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/OCILib.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\OCILibud.lib"

!ELSEIF  "$(CFG)" == "OCILib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Op /Oy /Ob2 /I "../oci/include" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_UNICODE" /D "_STLP_NEW_PLATFORM_SDK" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\OCILibu.lib"

!ENDIF 

# Begin Target

# Name "OCILib - Win32 Debug"
# Name "OCILib - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ocilibColumn.cpp
# End Source File
# Begin Source File

SOURCE=.\ocilibConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\ocilibError.cpp
# End Source File
# Begin Source File

SOURCE=.\ocilibGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\ocilibParameter.cpp
# End Source File
# Begin Source File

SOURCE=.\ocilibRecordset.cpp
# End Source File
# Begin Source File

SOURCE=.\ocilibStatement.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\oci.def
# End Source File
# Begin Source File

SOURCE=.\ocilibColumn.h
# End Source File
# Begin Source File

SOURCE=.\ocilibConnection.h
# End Source File
# Begin Source File

SOURCE=.\ocilibDate.h
# End Source File
# Begin Source File

SOURCE=.\ocilibError.h
# End Source File
# Begin Source File

SOURCE=.\ocilibGlobal.h
# End Source File
# Begin Source File

SOURCE=.\ocilibParameter.h
# End Source File
# Begin Source File

SOURCE=.\ocilibRecordset.h
# End Source File
# Begin Source File

SOURCE=.\ocilibStatement.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
