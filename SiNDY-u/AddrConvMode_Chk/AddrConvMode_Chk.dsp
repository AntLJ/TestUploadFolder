# Microsoft Developer Studio Project File - Name="AddrConvMode_Chk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=AddrConvMode_Chk - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "AddrConvMode_Chk.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "AddrConvMode_Chk.mak" CFG="AddrConvMode_Chk - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "AddrConvMode_Chk - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "AddrConvMode_Chk - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AddrConvMode_Chk - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Ot /Op /I "..\..\LIB\smp_prn_info" /I "..\..\LIB\crd_cnv" /I "..\..\LIB\feature_info" /I "..\..\LIB\prog_init_cls" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_ARC_9_1_" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib crd_cnv.lib smp_prn_info.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\LIB\smp_prn_info\lib" /libpath:"..\..\LIB\crd_cnv\lib"

!ELSEIF  "$(CFG)" == "AddrConvMode_Chk - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\LIB\smp_prn_info" /I "..\..\LIB\crd_cnv" /I "..\..\LIB\feature_info" /I "..\..\LIB\prog_init_cls" /D "_DEBUG" /D "_PERSONALL_" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_ARC_9_1_" /FR /YX /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib crd_cnvd.lib smp_prn_infod.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\LIB\smp_prn_info\lib" /libpath:"..\..\LIB\crd_cnv\lib"

!ENDIF 

# Begin Target

# Name "AddrConvMode_Chk - Win32 Release"
# Name "AddrConvMode_Chk - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddrConvMode_Chk.rc
# End Source File
# Begin Source File

SOURCE=.\AddrConvModeChk_m.cpp
# End Source File
# Begin Source File

SOURCE=..\..\LIB\feature_info\feature_info.cpp
# End Source File
# Begin Source File

SOURCE=.\main_rootin.cpp
# End Source File
# Begin Source File

SOURCE=.\msi_handle_ws.cpp
# End Source File
# Begin Source File

SOURCE=..\..\LIB\prog_init_cls\prog_init_cls.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\exec_time_handle.h
# End Source File
# Begin Source File

SOURCE=..\..\LIB\feature_info\feature_info.h
# End Source File
# Begin Source File

SOURCE=.\main_rootin.h
# End Source File
# Begin Source File

SOURCE=.\msi_handle_ws.h
# End Source File
# Begin Source File

SOURCE=..\..\LIB\prog_init_cls\prog_init_cls.h
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
