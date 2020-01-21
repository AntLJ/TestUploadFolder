# Microsoft Developer Studio Project File - Name="drmalib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=drmalib - Win32 Multi Thread FinalCheck
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "drmalib.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "drmalib.mak" CFG="drmalib - Win32 Multi Thread FinalCheck"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "drmalib - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "drmalib - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "drmalib - Win32 FinalCheck" ("Win32 (x86) Static Library" 用)
!MESSAGE "drmalib - Win32 Multi Thread Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "drmalib - Win32 Multi Thread DLL Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "drmalib - Win32 Multi Thread Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "drmalib - Win32 Multi Thread DLL Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "drmalib - Win32 Multi Thread DLL FinalCheck" ("Win32 (x86) Static Library" 用)
!MESSAGE "drmalib - Win32 Multi Thread FinalCheck" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "drmalib - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\drma.lib"

!ELSEIF  "$(CFG)" == "drmalib - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\drmad.lib"

!ELSEIF  "$(CFG)" == "drmalib - Win32 FinalCheck"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "drmalib___Win32_FinalCheck0"
# PROP BASE Intermediate_Dir "drmalib___Win32_FinalCheck0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FinalCheck"
# PROP Intermediate_Dir "FinalCheck"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "./include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\drmad.lib"
# ADD LIB32 /nologo /out:"lib\drmafd.lib"

!ELSEIF  "$(CFG)" == "drmalib - Win32 Multi Thread Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "drmalib___Win32_Multi_Thread_Debug"
# PROP BASE Intermediate_Dir "drmalib___Win32_Multi_Thread_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Multi_Thread_Debug"
# PROP Intermediate_Dir "Multi_Thread_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\drmad.lib"
# ADD LIB32 /nologo /out:"lib\drmamsd.lib"

!ELSEIF  "$(CFG)" == "drmalib - Win32 Multi Thread DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "drmalib___Win32_Multi_Thread_DLL_Debug"
# PROP BASE Intermediate_Dir "drmalib___Win32_Multi_Thread_DLL_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Multi_Thread_DLL_Debug"
# PROP Intermediate_Dir "Multi_Thread_DLL_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\drmad.lib"
# ADD LIB32 /nologo /out:"lib\drmamdd.lib"

!ELSEIF  "$(CFG)" == "drmalib - Win32 Multi Thread Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "drmalib___Win32_Multi_Thread_Release"
# PROP BASE Intermediate_Dir "drmalib___Win32_Multi_Thread_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Multi_Thread_Release"
# PROP Intermediate_Dir "Multi_Thread_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\drma.lib"
# ADD LIB32 /nologo /out:"lib\drmams.lib"

!ELSEIF  "$(CFG)" == "drmalib - Win32 Multi Thread DLL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "drmalib___Win32_Multi_Thread_DLL_Release"
# PROP BASE Intermediate_Dir "drmalib___Win32_Multi_Thread_DLL_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Multi_Thread_DLL_Release"
# PROP Intermediate_Dir "Multi_Thread_DLL_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\drma.lib"
# ADD LIB32 /nologo /out:"lib\drmamd.lib"

!ELSEIF  "$(CFG)" == "drmalib - Win32 Multi Thread DLL FinalCheck"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "drmalib___Win32_Multi_Thread_DLL_FinalCheck"
# PROP BASE Intermediate_Dir "drmalib___Win32_Multi_Thread_DLL_FinalCheck"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Multi_Thread_DLL_FinalCheck"
# PROP Intermediate_Dir "Multi_Thread_DLL_FinalCheck"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\drmamdd.lib"
# ADD LIB32 /nologo /out:"lib\drmamdfd.lib"

!ELSEIF  "$(CFG)" == "drmalib - Win32 Multi Thread FinalCheck"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "drmalib___Win32_Multi_Thread_FinalCheck"
# PROP BASE Intermediate_Dir "drmalib___Win32_Multi_Thread_FinalCheck"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Multi_Thread_FinalCheck"
# PROP Intermediate_Dir "Multi_Thread_FinalCheck"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\drmamsfd.lib"
# ADD LIB32 /nologo /out:"lib\drmamsfd.lib"

!ENDIF 

# Begin Target

# Name "drmalib - Win32 Release"
# Name "drmalib - Win32 Debug"
# Name "drmalib - Win32 FinalCheck"
# Name "drmalib - Win32 Multi Thread Debug"
# Name "drmalib - Win32 Multi Thread DLL Debug"
# Name "drmalib - Win32 Multi Thread Release"
# Name "drmalib - Win32 Multi Thread DLL Release"
# Name "drmalib - Win32 Multi Thread DLL FinalCheck"
# Name "drmalib - Win32 Multi Thread FinalCheck"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DRMAAdmin1.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMAAdmin2.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMAAdminLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMAAllRoadLink.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMAAllRoadLinkAttr.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMAAllRoadNode.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMABackGround.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMABaseRoadLink.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMABaseRoadLinkAttr.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMABaseRoadNode.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMABeaconLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMACodeCommunity.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMADevPlant.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMAFacilLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMAFacilShape.cpp
# End Source File
# Begin Source File

SOURCE=.\drmalib.rc
# End Source File
# Begin Source File

SOURCE=.\DRMALoad2.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMALoadProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMAPlaceNameLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMARailwayLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMARecord.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMARoadLinkRelation.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMASite.cpp
# End Source File
# Begin Source File

SOURCE=.\DRMAValues.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\DRMAAdmin1.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAAdmin2.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAAdminLocation.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAAllRoadLink.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAAllRoadLink2.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAAllRoadLinkAttr.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAAllRoadLinkAttr2.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAAllRoadNode.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAAllRoadNode2.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMABackGround.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMABaseRoadLink.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMABaseRoadLink2.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMABaseRoadLinkAttr.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMABaseRoadLinkAttr2.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMABaseRoadNode.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMABaseRoadNode2.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMABeaconLocation.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMACodeCommunity.h
# End Source File
# Begin Source File

SOURCE=.\DRMADevPlant.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAFacilLocation.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAFacilShape.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMALayer.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMALayer2.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMALayerLoad.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMALoad.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMALoad2.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMALoadProtocol.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAOriginalValues.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAPlaceNameLocation.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMARailwayLocation.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMARecord.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMARoadLinkRelation.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMASite.h
# End Source File
# Begin Source File

SOURCE=.\include\DRMAValues.h
# End Source File
# End Group
# End Target
# End Project
