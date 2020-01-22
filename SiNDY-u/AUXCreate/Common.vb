Option Strict Off
Option Explicit On
Module Common
	Public Const EPSG_WGS84 As Integer = 4326 ' WGS84のEPSGコード
	Public Const TOOL_REG As String = "Software\\INCREMENT P CORPORATION\\SiNDY-u\\AUXCreate" ' ツールのレジストリキー
	Public Const PARAM_REG As String = "ParamPath" ' 地域パラメータファイルのレジストリキー
	Public Const DEFAULT_PARAM_PATH As String = "./TKY2JGD.par"	' デフォルトの地域パラメータファイルのパス

	' エラーメッセージ出力
	Function ErrMsg(ByRef title As String, ByRef msg As String) As Integer
		ErrMsg = MsgBox(msg, MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, title)
	End Function
	' メッセージ出力
	Function NormalMsg(ByRef title As String, ByRef msg As String) As Integer
		NormalMsg = MsgBox(msg, MsgBoxStyle.OkOnly, title)
	End Function
	' ファイル名（拡張子無し）を取得
	Public Function GetPrefix(ByRef cFile As Object) As String
		Dim i As Short

		GetPrefix = vbNullString

		For i = Len(cFile) To 1 Step -1
			'UPGRADE_WARNING: オブジェクト cFile の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
			If Mid(cFile, i, 1) = "." Then
				'UPGRADE_WARNING: オブジェクト cFile の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
				GetPrefix = Mid(cFile, 1, i - 1)
				Exit Function
			Else

			End If
		Next
		'UPGRADE_WARNING: オブジェクト cFile の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
		GetPrefix = cFile
		Exit Function
	End Function
	' ファイル拡張子を取得
	Public Function GetSuffix(ByRef File As Object) As String
		Dim i As Short

		GetSuffix = vbNullString
		For i = Len(File) To 1 Step -1
			'UPGRADE_WARNING: オブジェクト File の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
			If Mid(File, i, 1) = "." Then
				'UPGRADE_WARNING: オブジェクト File の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
				GetSuffix = Mid(File, i, Len(File))
				Exit Function
			Else
			End If
		Next
	End Function
	' ファイルパスを取得
	Public Function GetPath(ByRef Path As Object) As String
		GetPath = System.IO.Path.GetDirectoryName(Path)
	End Function
	' ファイル名を取得
	Public Function GetFilename(ByRef Path As Object) As String
		GetFilename = System.IO.Path.GetFileName(Path)
	End Function
	' パスの存在確認
	Public Function ExistPath(ByRef wPath As String) As Boolean
		'UPGRADE_WARNING: Dir に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
		If wPath = vbNullString Or Dir(wPath, FileAttribute.Directory) = vbNullString Then
			ExistPath = False
		Else
			ExistPath = True
		End If
		Exit Function
	End Function
	' ファイルの存在確認
	Public Function ExistFile(ByRef wFile As String) As Boolean
		'UPGRADE_WARNING: Dir に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
		If wFile = vbNullString Or Dir(wFile) = vbNullString Then
			ExistFile = False
		Else
			ExistFile = True
		End If
		Exit Function
	End Function
	' パス名の最後の\記号をチェックして追加
	Public Function PlusEn(ByRef wPath As Object) As Boolean
		If Right(wPath, 1) = "\" Then
			PlusEn = True
		Else
			wPath = wPath & "\"
			PlusEn = False
		End If
		Exit Function
	End Function
	' コマンドライン引数の取得
	Public Function GetCmdPrm(ByVal wCmd As Object, ByRef wPrm() As String) As Short
		Dim wPos As Short
		Dim wStop As Short
		Dim wStart As Short
		Dim Line As String
		Dim i As Short

		i = 0
		GetCmdPrm = 0

		'UPGRADE_WARNING: オブジェクト wCmd の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
		Line = Trim(wCmd)

		wPos = InStr(Line, " ")
		wStart = InStr(Line, """")
		wStop = InStr(wPos + 1, Line, """")

		' 引数が1個の場合
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
			' カレント引数のセット
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

			' 次引数の検索
			wPos = InStr(Line, " ")
			wStart = InStr(Line, """")
			wStop = InStr(wPos + 1, Line, """")
			i = i + 1

			'  最後の引数のセット
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
	' INIファイルからの初期化パラメータの取得
	Public Function GetProfilePrm(ByRef bSw As Boolean, ByVal wSection As String, ByVal wKey As String, ByRef wIniPath As String) As String
		GetProfilePrm = vbNullString

		Dim StrBuf As New VB6.FixedLengthString(255) ' GetPrivateProfileStringの文字列取得エリア
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
			MsgBox("予期しないエラー #" & VB6.Format(Err.Number) & " が発生しました。" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "エラー")
			Stop
		End If
	End Sub
End Module