# Microsoft Developer Studio Project File - Name="RNS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=RNS - Win32 Single Thread FinalCheck
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "RNS.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "RNS.mak" CFG="RNS - Win32 Single Thread FinalCheck"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "RNS - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "RNS - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "RNS - Win32 Single Thread Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "RNS - Win32 Single Thread Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "RNS - Win32 FinalCheck" ("Win32 (x86) Static Library" 用)
!MESSAGE "RNS - Win32 Single Thread FinalCheck" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RNS - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\rnsm.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir  ..\include\ 	copy   *.h   ..\include\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "RNS - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\rnsmd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy   *.h   ..\include\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "RNS - Win32 Single Thread Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RNS___Win32_Single_Thread_Debug0"
# PROP BASE Intermediate_Dir "RNS___Win32_Single_Thread_Debug0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RNS___Win32_Single_Thread_Debug0"
# PROP Intermediate_Dir "RNS___Win32_Single_Thread_Debug0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\rnsmd.lib"
# ADD LIB32 /nologo /out:"..\lib\rnsd.lib"

!ELSEIF  "$(CFG)" == "RNS - Win32 Single Thread Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RNS___Win32_Single_Thread_Release0"
# PROP BASE Intermediate_Dir "RNS___Win32_Single_Thread_Release0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "RNS___Win32_Single_Thread_Release0"
# PROP Intermediate_Dir "RNS___Win32_Single_Thread_Release0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\rnsm.lib"
# ADD LIB32 /nologo /out:"..\lib\rns.lib"

!ELSEIF  "$(CFG)" == "RNS - Win32 FinalCheck"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RNS___Win32_FinalCheck"
# PROP BASE Intermediate_Dir "RNS___Win32_FinalCheck"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RNS___Win32_FinalCheck"
# PROP Intermediate_Dir "RNS___Win32_FinalCheck"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\rnsmd.lib"
# ADD LIB32 /nologo /out:"..\lib\rnsmfd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy *.h ..\include
# End Special Build Tool

!ELSEIF  "$(CFG)" == "RNS - Win32 Single Thread FinalCheck"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RNS___Win32_Single_Thread_FinalCheck0"
# PROP BASE Intermediate_Dir "RNS___Win32_Single_Thread_FinalCheck0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RNS___Win32_Single_Thread_FinalCheck0"
# PROP Intermediate_Dir "RNS___Win32_Single_Thread_FinalCheck0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\rnsd.lib"
# ADD LIB32 /nologo /out:"..\lib\rnsfd.lib"

!ENDIF 

# Begin Target

# Name "RNS - Win32 Release"
# Name "RNS - Win32 Debug"
# Name "RNS - Win32 Single Thread Debug"
# Name "RNS - Win32 Single Thread Release"
# Name "RNS - Win32 FinalCheck"
# Name "RNS - Win32 Single Thread FinalCheck"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\RNSDelimitStream.cpp
# End Source File
# Begin Source File

SOURCE=.\RNSDirectory.cpp
# End Source File
# Begin Source File

SOURCE=.\RNSException.cpp
# End Source File
# Begin Source File

SOURCE=.\RNSGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\RNSID.cpp
# End Source File
# Begin Source File

SOURCE=.\RNSPlant.cpp
# End Source File
# Begin Source File

SOURCE=.\RNSString.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\RNS.h
# End Source File
# Begin Source File

SOURCE=.\RNSDelimitStream.h
# End Source File
# Begin Source File

SOURCE=.\RNSDirectory.h
# End Source File
# Begin Source File

SOURCE=.\RNSException.h
# End Source File
# Begin Source File

SOURCE=.\RNSGroup.h
# End Source File
# Begin Source File

SOURCE=.\RNSID.h
# End Source File
# Begin Source File

SOURCE=.\RNSObject.h
# End Source File
# Begin Source File

SOURCE=.\RNSPlant.h
# End Source File
# Begin Source File

SOURCE=.\RNSString.h
# End Source File
# End Group
# End Target
# End Project
