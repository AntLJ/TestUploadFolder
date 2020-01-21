# Microsoft Developer Studio Project File - Name="mpqlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=mpqlib - Win32 MultiThread FinalCheck
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "mpqlib.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "mpqlib.mak" CFG="mpqlib - Win32 MultiThread FinalCheck"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "mpqlib - Win32 MultiThread Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "mpqlib - Win32 MultiThread Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "mpqlib - Win32 MultiThread FinalCheck" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mpqlib - Win32 MultiThread Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MultiThread Debug"
# PROP BASE Intermediate_Dir "MultiThread Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MultiThread_Debug"
# PROP Intermediate_Dir "MultiThread_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../ZCL" /D "_LIB" /D "_DEBUG" /D "LITTLE_ENDIAN" /D "WIN32" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\mpqmd.lib"

!ELSEIF  "$(CFG)" == "mpqlib - Win32 MultiThread Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MultiThread Release"
# PROP BASE Intermediate_Dir "MultiThread Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MultiThread_Release"
# PROP Intermediate_Dir "MultiThread_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../ZCL" /D "NDEBUG" /D "LITTLE_ENDIAN" /D "WIN32" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\mpqm.lib"

!ELSEIF  "$(CFG)" == "mpqlib - Win32 MultiThread FinalCheck"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MultiThread FinalCheck"
# PROP BASE Intermediate_Dir "MultiThread FinalCheck"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MultiThread_FinalCheck"
# PROP Intermediate_Dir "MultiThread_FinalCheck"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../ZCL" /D "_DEBUG" /D "LITTLE_ENDIAN" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../ZCL" /D "_LIB" /D "_DEBUG" /D "LITTLE_ENDIAN" /D "WIN32" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\mpqmd.lib"
# ADD LIB32 /nologo /out:"lib\mpqmf.lib"

!ENDIF 

# Begin Target

# Name "mpqlib - Win32 MultiThread Debug"
# Name "mpqlib - Win32 MultiThread Release"
# Name "mpqlib - Win32 MultiThread FinalCheck"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MPQ.cpp
# End Source File
# Begin Source File

SOURCE=.\MPQCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\MPQEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MPQJoint.cpp
# End Source File
# Begin Source File

SOURCE=.\MPQPro.cpp
# End Source File
# Begin Source File

SOURCE=.\MPQVer1.cpp
# End Source File
# Begin Source File

SOURCE=.\MPQVer2.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MPQ.h
# End Source File
# Begin Source File

SOURCE=.\MPQCreator.h
# End Source File
# Begin Source File

SOURCE=.\MPQEdit.h
# End Source File
# Begin Source File

SOURCE=.\MPQFunctional.h
# End Source File
# Begin Source File

SOURCE=.\MPQIO.h
# End Source File
# Begin Source File

SOURCE=.\MPQJoint.h
# End Source File
# Begin Source File

SOURCE=.\MPQPro.h
# End Source File
# Begin Source File

SOURCE=.\MPQVer1.h
# End Source File
# Begin Source File

SOURCE=.\MPQVer2.h
# End Source File
# Begin Source File

SOURCE=.\MPQView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
