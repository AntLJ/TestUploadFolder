# Microsoft Developer Studio Project File - Name="document" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** �ҏW���Ȃ��ł������� **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=document - Win32 Release
!MESSAGE ����͗L����Ҳ�̧�قł͂���܂���B ������ۼު�Ă�����ނ��邽�߂ɂ� NMAKE ���g�p���Ă��������B
!MESSAGE [Ҳ�̧�ق̴���߰�] ����ނ��g�p���Ď��s���Ă�������
!MESSAGE 
!MESSAGE NMAKE /f "document.mak".
!MESSAGE 
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "document.mak" CFG="document - Win32 Release"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "document - Win32 Release" ("Win32 (x86) Generic Project" �p)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe
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
# Begin Target

# Name "document - Win32 Release"
# Begin Group "Example Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\example.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Doxyfile
# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\Doxyfile

"dummy" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	doxygen 
	hhc.exe html\index.hhp 
	md ..\help 
	copy html\index.chm ..\help\SketchTool.chm 
	
# End Custom Build
# End Source File
# Begin Source File

SOURCE=.\intro.h
# End Source File
# Begin Source File

SOURCE=.\mainpage.h
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\specification.h
# End Source File
# Begin Source File

SOURCE=.\tool.h
# End Source File
# End Target
# End Project
