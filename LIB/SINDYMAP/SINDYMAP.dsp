# Microsoft Developer Studio Project File - Name="SINDYMAP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SINDYMAP - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "SINDYMAP.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "SINDYMAP.mak" CFG="SINDYMAP - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "SINDYMAP - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "SINDYMAP - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SINDYMAP - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "..\GDB" /I "..\strcode\include" /I "..\WKH" /I "..\SINDYBASE" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\lib\sindymap.lib"

!ELSEIF  "$(CFG)" == "SINDYMAP - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\GDB" /I "..\strcode\include" /I "..\WKH" /I "..\SINDYBASE" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\lib\sindymapd.lib"

!ENDIF 

# Begin Target

# Name "SINDYMAP - Win32 Release"
# Name "SINDYMAP - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SindyAccessPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyAccessStore.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyAnnotation.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyBaseAdmin.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyBaseAnnotation.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyBaseMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyByway.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyCityAnnotation.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyCityMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyDirguide.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyDupliLink.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyFacilInfoPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyGrad.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyGuide.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyHighwayFacil.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyHighwayNode.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyHighwayPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyHighwayText.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyInterNavi.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyLane.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyMeshFeature.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyMiddleAnnotation.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyMiddleMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyParkingShape.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyRoadCodeList.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyRoadLink.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyRoadNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyRoadNode.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyRoute.cpp
# End Source File
# Begin Source File

SOURCE=.\SindySegmentAttr.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyTimeNoPassage.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyTimeOneway.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyTurnreg.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyUturn.cpp
# End Source File
# Begin Source File

SOURCE=.\SindyVics.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\SindyAccessPoint.h
# End Source File
# Begin Source File

SOURCE=.\SindyAccessStore.h
# End Source File
# Begin Source File

SOURCE=.\SindyAnnotation.h
# End Source File
# Begin Source File

SOURCE=.\SindyBaseAdmin.h
# End Source File
# Begin Source File

SOURCE=.\SindyBaseAnnotation.h
# End Source File
# Begin Source File

SOURCE=.\SindyBaseMesh.h
# End Source File
# Begin Source File

SOURCE=.\SindyByway.h
# End Source File
# Begin Source File

SOURCE=.\SindyCityAnnotation.h
# End Source File
# Begin Source File

SOURCE=.\SindyCityMesh.h
# End Source File
# Begin Source File

SOURCE=.\SindyDirguide.h
# End Source File
# Begin Source File

SOURCE=.\SindyDupliLink.h
# End Source File
# Begin Source File

SOURCE=.\SindyFacilInfoPoint.h
# End Source File
# Begin Source File

SOURCE=.\SindyGrad.h
# End Source File
# Begin Source File

SOURCE=.\SindyGuide.h
# End Source File
# Begin Source File

SOURCE=.\SindyHighwayFacil.h
# End Source File
# Begin Source File

SOURCE=.\SindyHighwayNode.h
# End Source File
# Begin Source File

SOURCE=.\SindyHighwayPoint.h
# End Source File
# Begin Source File

SOURCE=.\SindyHighwayText.h
# End Source File
# Begin Source File

SOURCE=.\SindyInterNavi.h
# End Source File
# Begin Source File

SOURCE=.\SindyLane.h
# End Source File
# Begin Source File

SOURCE=.\SindyMeshFeature.h
# End Source File
# Begin Source File

SOURCE=.\SindyMiddleAnnotation.h
# End Source File
# Begin Source File

SOURCE=.\SindyMiddleMesh.h
# End Source File
# Begin Source File

SOURCE=.\SindyParkingShape.h
# End Source File
# Begin Source File

SOURCE=.\SindyRoadCodeList.h
# End Source File
# Begin Source File

SOURCE=.\SindyRoadLink.h
# End Source File
# Begin Source File

SOURCE=.\SindyRoadNetwork.h
# End Source File
# Begin Source File

SOURCE=.\SindyRoadNode.h
# End Source File
# Begin Source File

SOURCE=.\SindyRoute.h
# End Source File
# Begin Source File

SOURCE=.\SindySegmentAttr.h
# End Source File
# Begin Source File

SOURCE=.\SindyTimeNoPassage.h
# End Source File
# Begin Source File

SOURCE=.\SindyTimeOneway.h
# End Source File
# Begin Source File

SOURCE=.\SindyTurnreg.h
# End Source File
# Begin Source File

SOURCE=.\SindyUturn.h
# End Source File
# Begin Source File

SOURCE=.\SindyVics.h
# End Source File
# End Group
# End Target
# End Project
