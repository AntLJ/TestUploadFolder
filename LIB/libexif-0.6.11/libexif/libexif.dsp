# Microsoft Developer Studio Project File - Name="libexif" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libexif - Win32 MultiDebug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "libexif.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "libexif.mak" CFG="libexif - Win32 MultiDebug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "libexif - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "libexif - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "libexif - Win32 MultiDebug" ("Win32 (x86) Static Library" 用)
!MESSAGE "libexif - Win32 MultiRelease" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libexif - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "../" /I "./" /I "../libjpeg" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "__WATCOMC__" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\libexif.lib"

!ELSEIF  "$(CFG)" == "libexif - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../" /I "./" /I "../libjpeg" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "__WATCOMC__" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\libexifd.lib"

!ELSEIF  "$(CFG)" == "libexif - Win32 MultiDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MultiDebug"
# PROP BASE Intermediate_Dir "MultiDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MultiDebug"
# PROP Intermediate_Dir "MultiDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../" /I "./" /I "../libjpeg" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "__WATCOMC__" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../" /I "./" /I "../libjpeg" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "__WATCOMC__" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\libexifd.lib"
# ADD LIB32 /nologo /out:"..\lib\libexifmd.lib"

!ELSEIF  "$(CFG)" == "libexif - Win32 MultiRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MultiRelease"
# PROP BASE Intermediate_Dir "MultiRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MultiRelease"
# PROP Intermediate_Dir "MultiRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../" /I "./" /I "../libjpeg" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "__WATCOMC__" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../" /I "./" /I "../libjpeg" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "__WATCOMC__" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\libexif.lib"
# ADD LIB32 /nologo /out:"..\lib\libexifm.lib"

!ENDIF 

# Begin Target

# Name "libexif - Win32 Release"
# Name "libexif - Win32 Debug"
# Name "libexif - Win32 MultiDebug"
# Name "libexif - Win32 MultiRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=".\exif-byte-order.c"
# End Source File
# Begin Source File

SOURCE=".\exif-content.c"
# End Source File
# Begin Source File

SOURCE=".\exif-data.c"
# End Source File
# Begin Source File

SOURCE=".\exif-entry.c"
# End Source File
# Begin Source File

SOURCE=".\exif-format.c"
# End Source File
# Begin Source File

SOURCE=".\exif-ifd.c"
# End Source File
# Begin Source File

SOURCE=".\exif-loader.c"
# End Source File
# Begin Source File

SOURCE=".\exif-log.c"
# End Source File
# Begin Source File

SOURCE=".\exif-mem.c"
# End Source File
# Begin Source File

SOURCE=".\canon\exif-mnote-data-canon.c"
# End Source File
# Begin Source File

SOURCE=".\olympus\exif-mnote-data-olympus.c"
# End Source File
# Begin Source File

SOURCE=".\pentax\exif-mnote-data-pentax.c"
# End Source File
# Begin Source File

SOURCE=".\exif-mnote-data.c"
# End Source File
# Begin Source File

SOURCE=".\exif-tag.c"
# End Source File
# Begin Source File

SOURCE=".\exif-utils.c"
# End Source File
# Begin Source File

SOURCE="..\libjpeg\jpeg-data.c"
# End Source File
# Begin Source File

SOURCE="..\libjpeg\jpeg-marker.c"
# End Source File
# Begin Source File

SOURCE=".\canon\mnote-canon-entry.c"
# End Source File
# Begin Source File

SOURCE=".\canon\mnote-canon-tag.c"
# End Source File
# Begin Source File

SOURCE=".\olympus\mnote-olympus-entry.c"
# End Source File
# Begin Source File

SOURCE=".\olympus\mnote-olympus-tag.c"
# End Source File
# Begin Source File

SOURCE=".\pentax\mnote-pentax-entry.c"
# End Source File
# Begin Source File

SOURCE=".\pentax\mnote-pentax-tag.c"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_stdint.h
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=".\exif-byte-order.h"
# End Source File
# Begin Source File

SOURCE=".\exif-content.h"
# End Source File
# Begin Source File

SOURCE=".\exif-data.h"
# End Source File
# Begin Source File

SOURCE=".\exif-entry.h"
# End Source File
# Begin Source File

SOURCE=".\exif-format.h"
# End Source File
# Begin Source File

SOURCE=".\exif-ifd.h"
# End Source File
# Begin Source File

SOURCE=".\exif-loader.h"
# End Source File
# Begin Source File

SOURCE=".\exif-log.h"
# End Source File
# Begin Source File

SOURCE=".\exif-mem.h"
# End Source File
# Begin Source File

SOURCE=".\canon\exif-mnote-data-canon.h"
# End Source File
# Begin Source File

SOURCE=".\olympus\exif-mnote-data-olympus.h"
# End Source File
# Begin Source File

SOURCE=".\pentax\exif-mnote-data-pentax.h"
# End Source File
# Begin Source File

SOURCE=".\exif-mnote-data-priv.h"
# End Source File
# Begin Source File

SOURCE=".\exif-mnote-data.h"
# End Source File
# Begin Source File

SOURCE=".\exif-result.h"
# End Source File
# Begin Source File

SOURCE=".\exif-tag.h"
# End Source File
# Begin Source File

SOURCE=".\exif-utils.h"
# End Source File
# Begin Source File

SOURCE=.\i18n.h
# End Source File
# Begin Source File

SOURCE="..\libjpeg\jpeg-data.h"
# End Source File
# Begin Source File

SOURCE="..\libjpeg\jpeg-marker.h"
# End Source File
# Begin Source File

SOURCE=".\canon\mnote-canon-entry.h"
# End Source File
# Begin Source File

SOURCE=".\canon\mnote-canon-tag.h"
# End Source File
# Begin Source File

SOURCE=".\olympus\mnote-olympus-entry.h"
# End Source File
# Begin Source File

SOURCE=".\olympus\mnote-olympus-tag.h"
# End Source File
# Begin Source File

SOURCE=".\pentax\mnote-pentax-entry.h"
# End Source File
# Begin Source File

SOURCE=".\pentax\mnote-pentax-tag.h"
# End Source File
# End Group
# Begin Source File

SOURCE=".\修正履歴.txt"
# End Source File
# End Target
# End Project
