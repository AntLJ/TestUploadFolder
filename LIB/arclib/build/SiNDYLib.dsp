# Microsoft Developer Studio Project File - Name="SiNDYLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SiNDYLib - Win32 Afxdll Debug for Arc9
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "SiNDYLib.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "SiNDYLib.mak" CFG="SiNDYLib - Win32 Afxdll Debug for Arc9"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "SiNDYLib - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Unicode Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Unicode Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Afxdll Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Afxdll Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Runtime DLL Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Runtime DLL Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Runtime DLL Release for Arc9" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Runtime DLL Unicode Release for Arc9" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Runtime DLL Unicode Debug for Arc9" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Afxdll Release for Arc9" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Runtime DLL Debug for Arc9" ("Win32 (x86) Static Library" 用)
!MESSAGE "SiNDYLib - Win32 Afxdll Debug for Arc9" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SiNDYLib - Win32 Release"

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
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/sindyms.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Debug"

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
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/sindymsd.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "NDEBUG" /D "_UNICODE" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib/sindyms.lib"
# ADD LIB32 /nologo /out:"../lib/sindyums.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "_DEBUG" /D "_UNICODE" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib/sindymsd.lib"
# ADD LIB32 /nologo /out:"../lib/sindyumsd.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SiNDYLib___Win32_Afxdll_Release"
# PROP BASE Intermediate_Dir "SiNDYLib___Win32_Afxdll_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Afxdll_Release"
# PROP Intermediate_Dir "Afxdll_Release"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "include" /I "../../SiNDY-e/COMMON" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "NDEBUG" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_ATL_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib/sindyms.lib"
# ADD LIB32 /nologo /out:"../lib/sindya.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SiNDYLib___Win32_Afxdll_Debug"
# PROP BASE Intermediate_Dir "SiNDYLib___Win32_Afxdll_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Afxdll_Debug"
# PROP Intermediate_Dir "Afxdll_Debug"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /I "../../SiNDY-e/COMMON" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_ATL_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib/sindymsd.lib"
# ADD LIB32 /nologo /out:"../lib/sindyad.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SiNDYLib___Win32_Runtime_DLL_Debug"
# PROP BASE Intermediate_Dir "SiNDYLib___Win32_Runtime_DLL_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Runtime_DLL_Debug"
# PROP Intermediate_Dir "Runtime_DLL_Debug"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /I "../GeoLib/include" /I "../../SiNDY-e/COMMON" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib/sindymsd.lib"
# ADD LIB32 /nologo /out:"../lib/sindymdd.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SiNDYLib___Win32_Runtime_DLL_Release"
# PROP BASE Intermediate_Dir "SiNDYLib___Win32_Runtime_DLL_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Runtime_DLL_Release"
# PROP Intermediate_Dir "Runtime_DLL_Release"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "include" /I "../GeoLib/include" /I "../../SiNDY-e/COMMON" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib/sindyms.lib"
# ADD LIB32 /nologo /out:"../lib/sindymd.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Release for Arc9"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SiNDYLib___Win32_Runtime_DLL_Release_for_Arc9"
# PROP BASE Intermediate_Dir "SiNDYLib___Win32_Runtime_DLL_Release_for_Arc9"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseArc9"
# PROP Intermediate_Dir "ReleaseArc9"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_USE_ARC9" /D "_STLP_NEW_PLATFORM_SDK" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../lib/sindymd.lib"
# ADD LIB32 /nologo /out:"../lib/sindyms9.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Unicode Release for Arc9"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SiNDYLib___Win32_Runtime_DLL_Unicode_Release_for_Arc9"
# PROP BASE Intermediate_Dir "SiNDYLib___Win32_Runtime_DLL_Unicode_Release_for_Arc9"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUArc9"
# PROP Intermediate_Dir "ReleaseUArc9"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_USE_ARC9" /D "_STLP_NEW_PLATFORM_SDK" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_UNICODE" /D "_LIB" /D "_USE_ARC9" /D "_STLP_NEW_PLATFORM_SDK" /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../lib/sindymus9.lib"
# ADD LIB32 /nologo /out:"../lib/sindyums9.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Unicode Debug for Arc9"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SiNDYLib___Win32_Runtime_DLL_Unicode_Debug_for_Arc9"
# PROP BASE Intermediate_Dir "SiNDYLib___Win32_Runtime_DLL_Unicode_Debug_for_Arc9"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugUArc9"
# PROP Intermediate_Dir "DebugUArc9"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_UNICODE" /D "_LIB" /D "_USE_ARC9" /D "_STLP_NEW_PLATFORM_SDK" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../lib/sindymdd.lib"
# ADD LIB32 /nologo /out:"../lib/sindyumsd9.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Release for Arc9"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SiNDYLib___Win32_Afxdll_Release_for_Arc9"
# PROP BASE Intermediate_Dir "SiNDYLib___Win32_Afxdll_Release_for_Arc9"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AfxdllReleaseArc9"
# PROP Intermediate_Dir "AfxdllReleaseArc9"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../" /D "NDEBUG" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_ATL_DLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "NDEBUG" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_ATL_DLL" /D "_USE_ARC9" /D "_STLP_NEW_PLATFORM_SDK" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../lib/sindya.lib"
# ADD LIB32 /nologo /out:"../lib/sindya9.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Debug for Arc9"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SiNDYLib___Win32_Runtime_DLL_Debug_for_Arc9"
# PROP BASE Intermediate_Dir "SiNDYLib___Win32_Runtime_DLL_Debug_for_Arc9"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugArc9"
# PROP Intermediate_Dir "DebugArc9"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "_DEBUG" /D "_ATL_DLL" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_USE_ARC9" /D "_STLP_NEW_PLATFORM_SDK" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../lib/sindymdd.lib"
# ADD LIB32 /nologo /out:"../lib/sindymsd9.lib"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Debug for Arc9"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SiNDYLib___Win32_Afxdll_Debug_for_Arc9"
# PROP BASE Intermediate_Dir "SiNDYLib___Win32_Afxdll_Debug_for_Arc9"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Afxdll_Debug_for_Arc9"
# PROP Intermediate_Dir "Afxdll_Debug_for_Arc9"
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_ATL_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../GeoLib/include" /I "../../WinLib" /I "../../spirit-1.6" /I "../../" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_ATL_DLL" /D "_USE_ARC9" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../lib/sindyad.lib"
# ADD LIB32 /nologo /out:"../lib/sindyad.lib"

!ENDIF 

# Begin Target

# Name "SiNDYLib - Win32 Release"
# Name "SiNDYLib - Win32 Debug"
# Name "SiNDYLib - Win32 Unicode Release"
# Name "SiNDYLib - Win32 Unicode Debug"
# Name "SiNDYLib - Win32 Afxdll Release"
# Name "SiNDYLib - Win32 Afxdll Debug"
# Name "SiNDYLib - Win32 Runtime DLL Debug"
# Name "SiNDYLib - Win32 Runtime DLL Release"
# Name "SiNDYLib - Win32 Runtime DLL Release for Arc9"
# Name "SiNDYLib - Win32 Runtime DLL Unicode Release for Arc9"
# Name "SiNDYLib - Win32 Runtime DLL Unicode Debug for Arc9"
# Name "SiNDYLib - Win32 Afxdll Release for Arc9"
# Name "SiNDYLib - Win32 Runtime DLL Debug for Arc9"
# Name "SiNDYLib - Win32 Afxdll Debug for Arc9"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Connector Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SiNDYConnectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYConnectionFile.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYConnectionFileListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYConnectionInfoString.cpp
# End Source File
# End Group
# Begin Group "Logging Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SiNDYFreeStyleLogHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYLogFunctionExecutor.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYLogHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYLogTypeSelector.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYStdLogHeader.cpp
# End Source File
# End Group
# Begin Group "AnnoLib Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\AbleLayers.cpp
# End Source File
# Begin Source File

SOURCE=..\AnnotationClassTable.cpp
# End Source File
# Begin Source File

SOURCE=..\AttributeRistrict.cpp
# End Source File
# Begin Source File

SOURCE=..\BackGroundClassTable.cpp
# End Source File
# Begin Source File

SOURCE=..\CacheAttribute.cpp
# End Source File
# Begin Source File

SOURCE=..\ConnectUserTable.cpp
# End Source File
# Begin Source File

SOURCE=..\CreatableLayers.cpp
# End Source File
# Begin Source File

SOURCE=..\CreatableLayersTable.cpp
# End Source File
# Begin Source File

SOURCE=..\DeletableLayersTable.cpp
# End Source File
# Begin Source File

SOURCE=..\DispNaviRelationTable.cpp
# End Source File
# Begin Source File

SOURCE=..\DispOtherRelationTable.cpp
# End Source File
# Begin Source File

SOURCE=..\EditAttrLayersTable.cpp
# End Source File
# Begin Source File

SOURCE=..\EditHistoryTable.cpp
# End Source File
# Begin Source File

SOURCE=..\EditMeshTable.cpp
# End Source File
# Begin Source File

SOURCE=..\EnumSegment.cpp
# End Source File
# Begin Source File

SOURCE=..\EnumVertex.cpp
# End Source File
# Begin Source File

SOURCE=..\ErrorObject.cpp
# End Source File
# Begin Source File

SOURCE=..\FieldMap.cpp
# End Source File
# Begin Source File

SOURCE=..\Geometry.cpp
# End Source File
# Begin Source File

SOURCE=..\GeometryConvert.cpp
# End Source File
# Begin Source File

SOURCE=..\GeomtryCheck.cpp
# End Source File
# Begin Source File

SOURCE=..\GlobalArgument.cpp
# End Source File
# Begin Source File

SOURCE=..\GlobalFunctions.cpp
# End Source File
# Begin Source File

SOURCE=..\LayerNamesTable.cpp
# End Source File
# Begin Source File

SOURCE=..\ListLong.cpp
# End Source File
# Begin Source File

SOURCE=..\ListString.cpp
# End Source File
# Begin Source File

SOURCE=..\LockAreaTable.cpp
# End Source File
# Begin Source File

SOURCE=..\LockMeshTable.cpp
# End Source File
# Begin Source File

SOURCE=..\LogicRule.cpp
# End Source File
# Begin Source File

SOURCE=..\meshutil.cpp
# End Source File
# Begin Source File

SOURCE=..\ModifyCheck.cpp
# End Source File
# Begin Source File

SOURCE=..\MovableLayersTable.cpp
# End Source File
# Begin Source File

SOURCE=..\NameString.cpp
# End Source File
# Begin Source File

SOURCE=..\OperatorTable.cpp
# End Source File
# Begin Source File

SOURCE=..\Point.cpp
# End Source File
# Begin Source File

SOURCE=..\PointCollection.cpp
# End Source File
# Begin Source File

SOURCE=..\PointIterator.cpp
# End Source File
# Begin Source File

SOURCE=..\RelationalOpeartor.cpp
# End Source File
# Begin Source File

SOURCE=..\RoadAttributeTable.cpp
# End Source File
# Begin Source File

SOURCE=..\RoadDispClassTable.cpp
# End Source File
# Begin Source File

SOURCE=..\RoadNaviClassTable.cpp
# End Source File
# Begin Source File

SOURCE=..\Row.cpp
# End Source File
# Begin Source File

SOURCE=..\Rule.cpp
# End Source File
# Begin Source File

SOURCE=..\RuleCommunity.cpp
# End Source File
# Begin Source File

SOURCE=..\sde_ondemand.cpp
# End Source File
# Begin Source File

SOURCE=..\SegmentCollection.cpp
# End Source File
# Begin Source File

SOURCE=..\SharedDeleteLayersTable.cpp
# End Source File
# Begin Source File

SOURCE=..\SharedMoveLayersTable.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYeSettings.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYLogFunctions.cpp
# End Source File
# Begin Source File

SOURCE=..\Table.cpp
# End Source File
# Begin Source File

SOURCE=..\TableTypeUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\util.cpp
# End Source File
# Begin Source File

SOURCE=..\Where.cpp

!IF  "$(CFG)" == "SiNDYLib - Win32 Release"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Release"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Debug"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Debug"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Release"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Release for Arc9"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Unicode Release for Arc9"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Unicode Debug for Arc9"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Release for Arc9"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Debug for Arc9"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Debug for Arc9"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\WhereClauseGrammarChecker.cpp

!IF  "$(CFG)" == "SiNDYLib - Win32 Release"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Release"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Debug"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Debug"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Release"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Release for Arc9"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Unicode Release for Arc9"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Unicode Debug for Arc9"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Release for Arc9"

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Runtime DLL Debug for Arc9"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SiNDYLib - Win32 Afxdll Debug for Arc9"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\WorkPurposeTable.cpp
# End Source File
# Begin Source File

SOURCE=..\Workspace.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\DropSource.cpp
# End Source File
# Begin Source File

SOURCE=..\ErrorCode.cpp
# End Source File
# Begin Source File

SOURCE=..\FeatureClass.cpp
# End Source File
# Begin Source File

SOURCE=..\MapCommunity.cpp
# End Source File
# Begin Source File

SOURCE=..\RoadSeq.cpp
# End Source File
# Begin Source File

SOURCE=..\RoadSeqDataObject.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYCommonAttrCommunity.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYConnectPropertyListDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYConnectPropertySet.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYDraw.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYHashSearcher.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYLib.rc
# ADD BASE RSC /l 0x411 /i "\home\furu2\work\src-svn\LIB\SiNDYLib" /i "\svn\LIB\SiNDYLib" /i "\works\LIB\SiNDYLib" /i "\svn\repos\trunk\LIB\SiNDYLib" /i "\Home\furu2\work\svn\LIB\SiNDYLib" /i "\Home\furu2\work\svn\SiNDY-e\BUILD\src\LIB\SiNDYLib"
# ADD RSC /l 0x411 /i "\home\furu2\work\src-svn\LIB\SiNDYLib" /i "\svn\LIB\SiNDYLib" /i "\works\LIB\SiNDYLib" /i "\svn\repos\trunk\LIB\SiNDYLib" /i "\Home\furu2\work\svn\LIB\SiNDYLib" /i "\Home\furu2\work\svn\SiNDY-e\BUILD\src\LIB\SiNDYLib" /i "include"
# End Source File
# Begin Source File

SOURCE=..\SiNDYLinkTracer.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYMapTableFinder.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYObjectIdRange.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYPlant.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYRoadAttrCommunity.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYTableFinder.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYTableName.cpp
# End Source File
# Begin Source File

SOURCE=..\SiNDYTablesBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "algorithm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\algorithm\find.h
# End Source File
# Begin Source File

SOURCE=..\include\algorithm\for_each.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYAlgorithm.h
# End Source File
# End Group
# Begin Group "functional"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\function\equal_to.h
# End Source File
# Begin Source File

SOURCE=..\include\function\less.h
# End Source File
# Begin Source File

SOURCE=..\include\function\modified.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYFunctional.h
# End Source File
# End Group
# Begin Group "detail"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\detail\_GetValueFunc.h
# End Source File
# Begin Source File

SOURCE=..\include\detail\def_reference_to.h
# End Source File
# Begin Source File

SOURCE=..\include\detail\iterator.h
# End Source File
# Begin Source File

SOURCE=..\include\detail\undef_reference_to.h
# End Source File
# End Group
# Begin Group "Connector Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\SiNDYConnectionDlg.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYConnectionDlgI.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYConnectionFile.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYConnectionFileListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYConnectionInfoString.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYConnectionInfoStringI.h
# End Source File
# End Group
# Begin Group "Logging Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\SiNDYFreeStyleLogHeader.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYLogFunctionExecutor.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYLogHeader.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYLogHeaderImpl.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYLogTypeSelector.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYStdLogHeader.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\ComWrapper.h
# End Source File
# Begin Source File

SOURCE=..\include\DropSource.h
# End Source File
# Begin Source File

SOURCE=..\include\ErrorCode.h
# End Source File
# Begin Source File

SOURCE=..\include\exportimport.h
# End Source File
# Begin Source File

SOURCE=..\include\FacilId.h
# End Source File
# Begin Source File

SOURCE=..\include\FacilIdDataObject.h
# End Source File
# Begin Source File

SOURCE=..\include\FeatureClass.h
# End Source File
# Begin Source File

SOURCE=..\include\FeatureTableType.h
# End Source File
# Begin Source File

SOURCE=..\include\Resource.h
# End Source File
# Begin Source File

SOURCE=..\include\RoadSeq.h
# End Source File
# Begin Source File

SOURCE=..\include\RoadSeqDataObject.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYAttrCommunity.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYCommonAttrCommunity.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYConnectPropertyDlg.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYConnectPropertyListDlg.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYConnectPropertySet.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYConnectPropertySetList.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYDatasetName.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYDraw.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYField.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYFieldIndex.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYFieldIndexMacro.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYFieldValue.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYFieldValueDomainName.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYFieldValueString.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYGeometry.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYHashSearcher.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYImplicitCast.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYInClauseStream.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYInheritMacro.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYLinkTracer.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYMapTableFinder.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYNodeIdAttractor.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYObjectId.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYObjectIdRange.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYPlant.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYRoadAttrCommunity.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYRowSetSearcher.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYShapeFieldName.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYSpatialLinkTracer.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYTableFinder.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYTableName.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYTablesBuffer.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYTraits.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYValueGetter.h
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYValueSetSearcher.h
# End Source File
# Begin Source File

SOURCE=..\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\include\TableType.h
# End Source File
# Begin Source File

SOURCE=..\include\VertexAttractor.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\sindylib.rh
# End Source File
# End Target
# End Project
