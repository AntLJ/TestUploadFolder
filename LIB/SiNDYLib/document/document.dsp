# Microsoft Developer Studio Project File - Name="document" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=document - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "document.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "document.mak" CFG="document - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "document - Win32 Release" ("Win32 (x86) Generic Project" 用)
!MESSAGE "document - Win32 Update" ("Win32 (x86) Generic Project" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "document - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "document - Win32 Update"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "document___Win32_Update"
# PROP BASE Intermediate_Dir "document___Win32_Update"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "document - Win32 Release"
# Name "document - Win32 Update"
# Begin Group "Example Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\example.cpp
# End Source File
# Begin Source File

SOURCE=.\example2.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Doxyfile

!IF  "$(CFG)" == "document - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\Doxyfile

"html/SiNDYRoadBlockToShape.chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	doxygen

# End Custom Build

!ELSEIF  "$(CFG)" == "document - Win32 Update"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\Doxyfile

"dummy" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	doxygen -u

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\intro.h
# End Source File
# Begin Source File

SOURCE=.\library.h
# End Source File
# Begin Source File

SOURCE=.\mainpage.h
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=..\include\SiNDYObjectIdRange.h
# End Source File
# End Target
# End Project
