Option Strict Off
Option Explicit On
Module Common
	Public Const EPSG_WGS84 As Integer = 4326 ' WGS84��EPSG�R�[�h
	Public Const TOOL_REG As String = "Software\\INCREMENT P CORPORATION\\SiNDY-u\\AUXCreate" ' �c�[���̃��W�X�g���L�[
	Public Const PARAM_REG As String = "ParamPath" ' �n��p�����[�^�t�@�C���̃��W�X�g���L�[
	Public Const DEFAULT_PARAM_PATH As String = "./TKY2JGD.par"	' �f�t�H���g�̒n��p�����[�^�t�@�C���̃p�X

	' �G���[���b�Z�[�W�o��
	Function ErrMsg(ByRef title As String, ByRef msg As String) As Integer
		ErrMsg = MsgBox(msg, MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, title)
	End Function
	' ���b�Z�[�W�o��
	Function NormalMsg(ByRef title As String, ByRef msg As String) As Integer
		NormalMsg = MsgBox(msg, MsgBoxStyle.OkOnly, title)
	End Function
	' �t�@�C�����i�g���q�����j���擾
	Public Function GetPrefix(ByRef cFile As Object) As String
		Dim i As Short

		GetPrefix = vbNullString

		For i = Len(cFile) To 1 Step -1
			'UPGRADE_WARNING: �I�u�W�F�N�g cFile �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
			If Mid(cFile, i, 1) = "." Then
				'UPGRADE_WARNING: �I�u�W�F�N�g cFile �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
				GetPrefix = Mid(cFile, 1, i - 1)
				Exit Function
			Else

			End If
		Next
		'UPGRADE_WARNING: �I�u�W�F�N�g cFile �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
		GetPrefix = cFile
		Exit Function
	End Function
	' �t�@�C���g���q���擾
	Public Function GetSuffix(ByRef File As Object) As String
		Dim i As Short

		GetSuffix = vbNullString
		For i = Len(File) To 1 Step -1
			'UPGRADE_WARNING: �I�u�W�F�N�g File �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
			If Mid(File, i, 1) = "." Then
				'UPGRADE_WARNING: �I�u�W�F�N�g File �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
				GetSuffix = Mid(File, i, Len(File))
				Exit Function
			Else
			End If
		Next
	End Function
	' �t�@�C���p�X���擾
	Public Function GetPath(ByRef Path As Object) As String
		GetPath = System.IO.Path.GetDirectoryName(Path)
	End Function
	' �t�@�C�������擾
	Public Function GetFilename(ByRef Path As Object) As String
		GetFilename = System.IO.Path.GetFileName(Path)
	End Function
	' �p�X�̑��݊m�F
	Public Function ExistPath(ByRef wPath As String) As Boolean
		'UPGRADE_WARNING: Dir �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
		If wPath = vbNullString Or Dir(wPath, FileAttribute.Directory) = vbNullString Then
			ExistPath = False
		Else
			ExistPath = True
		End If
		Exit Function
	End Function
	' �t�@�C���̑��݊m�F
	Public Function ExistFile(ByRef wFile As String) As Boolean
		'UPGRADE_WARNING: Dir �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
		If wFile = vbNullString Or Dir(wFile) = vbNullString Then
			ExistFile = False
		Else
			ExistFile = True
		End If
		Exit Function
	End Function
	' �p�X���̍Ō��\�L�����`�F�b�N���Ēǉ�
	Public Function PlusEn(ByRef wPath As Object) As Boolean
		If Right(wPath, 1) = "\" Then
			PlusEn = True
		Else
			wPath = wPath & "\"
			PlusEn = False
		End If
		Exit Function
	End Function
	' �R�}���h���C�������̎擾
	Public Function GetCmdPrm(ByVal wCmd As Object, ByRef wPrm() As String) As Short
		Dim wPos As Short
		Dim wStop As Short
		Dim wStart As Short
		Dim Line As String
		Dim i As Short

		i = 0
		GetCmdPrm = 0

		'UPGRADE_WARNING: �I�u�W�F�N�g wCmd �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
		Line = Trim(wCmd)

		wPos = InStr(Line, " ")
		wStart = InStr(Line, """")
		wStop = InStr(wPos + 1, Line, """")

		' ������1�̏ꍇ
		If (wPos <= 0 Or wStop = Len(Line)) And Line <> vbNullString Then
			If wPos <= wStart Then
				ReDim Preserve wPrm(0)
				wPrm(0) = Line
			Else
				ReDim Preserve wPrm(0)
				wPrm(0) = Mid(Line, wStart + 1, wStop - 2)
			End If
			GetCmdPrm = GetCmdPrm + 1
		End If

		Do Until wPos <= 0 Or wStop = Len(Line)
			' �J�����g�����̃Z�b�g
			If wPos <= wStart Then
				ReDim Preserve wPrm(i)
				wPrm(i) = Mid(Line, 1, wPos - 1)
				Line = Trim(Mid(Line, wPos))
			Else
				ReDim Preserve wPrm(i)
				wPrm(i) = Mid(Line, wStart + 1, wStop - 2)
				Line = Trim(Mid(Line, wStop + 1))
			End If
			GetCmdPrm = GetCmdPrm + 1

			' �������̌���
			wPos = InStr(Line, " ")
			wStart = InStr(Line, """")
			wStop = InStr(wPos + 1, Line, """")
			i = i + 1

			'  �Ō�̈����̃Z�b�g
			If wPos <= 0 Or wStop = Len(Line) Then
				If wPos <= wStart Then
					ReDim Preserve wPrm(i)
					wPrm(i) = Line
				Else
					ReDim Preserve wPrm(i)
					wPrm(i) = Mid(Line, wStart + 1, wStop - 2)
				End If
				GetCmdPrm = GetCmdPrm + 1
			End If
		Loop
	End Function
	' INI�t�@�C������̏������p�����[�^�̎擾
	Public Function GetProfilePrm(ByRef bSw As Boolean, ByVal wSection As String, ByVal wKey As String, ByRef wIniPath As String) As String
		GetProfilePrm = vbNullString

		Dim StrBuf As New VB6.FixedLengthString(255) ' GetPrivateProfileString�̕�����擾�G���A
		Dim Ret As Integer
		If bSw = True Then

			Ret = GetPrivateProfileString(wSection, wKey, vbNullString, StrBuf.Value, 100, wIniPath)
			GetProfilePrm = Trim(Left(StrBuf.Value, InStr(StrBuf.Value, vbNullChar) - 1))
		Else
			GetProfilePrm = CStr(GetPrivateProfileInt(wSection, wKey, 0, wIniPath))
		End If
	End Function

	Public Function WriteProfilePrm(ByVal wSection As String, ByVal wKey As String, ByRef wValue As String, ByRef wIniPath As String) As Short
		WriteProfilePrm = WritePrivateProfileString(wSection, wKey, wValue, wIniPath)
	End Function

	Public Sub FileError()
		Const ErrDriveNotExist As Short = 52
		Const ErrFileNotExist As Short = 53
		Const ErrPathNotExist As Short = 76
		If Err.Number = ErrDriveNotExist Then
			MsgBox(Err.Description, MsgBoxStyle.Exclamation)
			Stop
		ElseIf Err.Number = ErrFileNotExist Then
			MsgBox(Err.Description, MsgBoxStyle.Exclamation)
			Stop
		ElseIf Err.Number = ErrPathNotExist Then
			MsgBox(Err.Description, MsgBoxStyle.Exclamation)
			Stop
		Else
			MsgBox("�\�����Ȃ��G���[ #" & VB6.Format(Err.Number) & " ���������܂����B" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "�G���[")
			Stop
		End If
	End Sub
End Module