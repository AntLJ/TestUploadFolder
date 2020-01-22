Option Strict Off
Option Explicit On
Imports VB = Microsoft.VisualBasic
Friend Class Form1
	Inherits System.Windows.Forms.Form
    Dim IniPath As String ' 初期化ファイル名
	Dim WMeshf As Short
	
	
	'UPGRADE_WARNING: イベント CBConvCheck.CheckStateChanged は、フォームが初期化されたときに発生します。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"' をクリックしてください。
	Private Sub CBConvCheck_CheckStateChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles CBConvCheck.CheckStateChanged
		If CBConvCheck.CheckState = 0 Then
			TIFFPath.Enabled = False
            _Cmd_0.Enabled = False
		Else
			TIFFPath.Enabled = True
            _Cmd_0.Enabled = True
		End If
	End Sub
	
	'UPGRADE_WARNING: イベント CBLog.CheckStateChanged は、フォームが初期化されたときに発生します。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"' をクリックしてください。
	Private Sub CBLog_CheckStateChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles CBLog.CheckStateChanged
		If CBLog.CheckState = 0 Then
			LogFile.Enabled = False
            _Cmd_5.Enabled = False 'ログの出力にある参照ボタン
		Else
			LogFile.Enabled = True
            _Cmd_5.Enabled = True
		End If
	End Sub
	
	Private Sub Form1_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
        Dim CmdPrm() As String ' コマンドライン引数
		
        ' 初期化パラメータファイルのチェック
		If GetCmdPrm(VB.Command(), CmdPrm) > 0 Then
			IniPath = CmdPrm(0)
			If InStr(IniPath, "\") = 0 Then
				IniPath = CurDir() & "\" & IniPath
			End If
		Else
			IniPath = My.Application.Info.DirectoryPath
			PlusEn(IniPath)
			IniPath = IniPath & "Profile.ini"
		End If
		
        ' 初期化パラメータファイルの読込
		GetPrm(IniPath)
		
        ' ラスター変換プログラム（i_view32.exe）の存在チェック
        If Not System.IO.File.Exists(My.Application.Info.DirectoryPath & "\i_view32.exe") Then
            If MsgBox("ラスター変換プログラム" & vbCrLf & vbCrLf & GetAppPath() & "\i_view32.exe" & vbCrLf & vbCrLf & "が見つからないため TIFF/PNG 変換機能は使用できません。" & vbCrLf & "続行しますか？", vbExclamation + vbYesNo, My.Application.Info.Title) = vbYes Then
                CBConvCheck.Enabled = False
                TIFFPath.Enabled = False
                _Cmd_0.Enabled = False
            Else
                Me.Close()
                Exit Sub
            End If
        End If

        ' ライセンス認証
        Dim pMgr As ArcGISVersionLib.IArcGISVersion
        pMgr = New ArcGISVersionLib.VersionManager
        If Not pMgr.LoadVersion(ArcGISVersionLib.esriProductCode.esriArcGISDesktop, "") Then
            MsgBox("ArcGISのライセンス認証に失敗しました")
            End
        End If
        Dim pInit As ESRI.ArcGIS.esriSystem.IAoInitialize
        pInit = New ESRI.ArcGIS.esriSystem.AoInitialize
        If pInit.IsProductCodeAvailable(ESRI.ArcGIS.esriSystem.esriLicenseProductCode.esriLicenseProductCodeStandard) Then
            pInit.Initialize(ESRI.ArcGIS.esriSystem.esriLicenseProductCode.esriLicenseProductCodeStandard)
        Else
            MsgBox("ArcGISのライセンス認証に失敗しました")
        End If
    End Sub
	
	Private Sub Cmd_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Cmd.Click
		Dim Index As Short = Cmd.GetIndex(eventSender)
        With ComDlgOpen
            Select Case Index
                Case 0
                    'UPGRADE_WARNING: Filter に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
                    .Filter = "TIFF イメージ (*.tif)|*.tif|すべてのファイル (*.*)|*.*"
                    .FilterIndex = 2
                    .InitialDirectory = TIFFPath.Text
                    .CheckPathExists = True
                    .CheckPathExists = True
                Case 1
                    'UPGRADE_WARNING: Filter に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
                    .Filter = "PNG イメージ (*.png)|*.png|JPEG イメージ (*.jpg)|*.jpg|すべてのファイル (*.*)|*.*"
                    .FilterIndex = 1
                    .InitialDirectory = PNGPath.Text
                    .CheckPathExists = True
                    .CheckPathExists = True
                Case 2
                    'UPGRADE_WARNING: Filter に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
                    .Filter = "CSV ファイル (*.csv)|*.csv|テキスト ファイル (*.txt)|*.txt|すべてのファイル (*.*)|*.*"
                    .FilterIndex = 1
                    .InitialDirectory = TKZFile.Text
                    'UPGRADE_WARNING: MSComDlg.CommonDialog プロパティ ComDlg.Flags は、新しい動作をもつ ComDlgOpen.CheckFileExists にアップグレードされました。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="DFCDE711-9694-47D7-9C50-45A99CD8E91E"' をクリックしてください。
                    .CheckFileExists = True
                    .CheckPathExists = True
                    .CheckPathExists = True
                Case 3
                    Select Case True
                        Case Option2(0).Checked
                            If CBConvCheck.CheckState = 1 Then
                                'UPGRADE_WARNING: Filter に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
                                .Filter = "TIFF イメージ (*.tif)|*.tif|すべてのファイル (*.*)|*.*"
                                .InitialDirectory = TIFFPath.Text
                            Else
                                'UPGRADE_WARNING: Filter に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
                                .Filter = "PNG イメージ (*.png)|*.png|JPEG イメージ (*.jpg)|*.jpg|すべてのファイル (*.*)|*.*"
                                .InitialDirectory = PNGPath.Text
                            End If

                            If Option1(2).Checked Or Option1(3).Checked Then
                                .FilterIndex = 2
                            Else
                                .FilterIndex = 1
                            End If
                        Case Option2(1).Checked
                            'UPGRADE_WARNING: Filter に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
                            .Filter = "テキスト ファイル (*.txt)|*.txt|すべてのファイル (*.*)|*.*"
                            .FilterIndex = 1
                            .InitialDirectory = vbNullString
                            'UPGRADE_WARNING: MSComDlg.CommonDialog プロパティ ComDlg.Flags は、新しい動作をもつ ComDlgOpen.CheckFileExists にアップグレードされました。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="DFCDE711-9694-47D7-9C50-45A99CD8E91E"' をクリックしてください。
                            .CheckFileExists = True
                            .CheckPathExists = True
                            .CheckPathExists = True
                    End Select
                    '    Case 4
                    'テキスト ファイル (*.txt)|*.txt|すべてのファイル (*.*)|*.*
                    '        .FilterIndex = 1
                    '        '.InitDir = vbNullString
                    '        .Flags = cdlOFNFileMustExist
                Case 5
                    'UPGRADE_WARNING: Filter に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
                    .Filter = "ログ ファイル (*.log)|*.log|テキスト ファイル (*.txt)|*.txt|すべてのファイル (*.*)|*.*"
                    .FilterIndex = 1
                    .InitialDirectory = LogFile.Text
                    .CheckPathExists = True
                    .CheckPathExists = True
            End Select

            'COMMONDIALOG
            .ShowDialog()

            If ExistFile(.FileName) = True Then
                Select Case Index
                    Case 0
                        TIFFPath.Text = GetPath(.FileName)
                    Case 1
                        PNGPath.Text = GetPath(.FileName)
                    Case 2
                        TKZFile.Text = .FileName
                    Case 3
                        Select Case True
                            Case Option2(0).Checked
                                OutPutPath(0).Text = GetFilename(.FileName)
                            Case Option2(1).Checked
                                OutPutPath(0).Text = .FileName
                        End Select
                        '        Case 4
                        '            OutPutPath(1).Text = .FileName
                    Case 5
                        LogFile.Text = .FileName
                End Select
            End If
        End With
	End Sub
	
	Private Sub Form1_FormClosing(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
		Dim Cancel As Boolean = eventArgs.Cancel
		Dim UnloadMode As System.Windows.Forms.CloseReason = eventArgs.CloseReason
        ' パラメータのチェック
		Dim CmpFlag As Boolean
		
		Dim Ret As Short
		Do 
			If TKZFile.Text <> GetProfilePrm(True, "INIT", "TKZFile", IniPath) Then
				CmpFlag = True
				Exit Do
			End If
			
			If TIFFPath.Text <> GetProfilePrm(True, "INIT", "TIFFPath", IniPath) Then
				CmpFlag = True
				Exit Do
			End If
			If PNGPath.Text <> GetProfilePrm(True, "INIT", "PNGPath", IniPath) Then
				CmpFlag = True
				Exit Do
			End If
			
			If LogFile.Text <> GetProfilePrm(True, "INIT", "LogFile", IniPath) Then
				CmpFlag = True
				Exit Do
			End If
			
			Select Case True
				Case Option1(0).Checked
					Ret = 0
				Case Option1(1).Checked
					Ret = 1
				Case Option1(2).Checked
					Ret = 2
				Case Option1(3).Checked
					Ret = 3
				Case Option1(4).Checked
					Ret = 4
			End Select
			If Ret <> CDbl(GetProfilePrm(False, "INIT", "Raster", IniPath)) Then
				CmpFlag = True
				Exit Do
			End If
			
			If CBConvCheck.CheckState <> CDbl(GetProfilePrm(False, "INIT", "ConvFlag", IniPath)) Then
				CmpFlag = True
				Exit Do
			End If
			
			Select Case True
				Case Option2(0).Checked
					Ret = 0
				Case Option2(1).Checked
					Ret = 1
			End Select
			If Ret <> CDbl(GetProfilePrm(False, "INIT", "Output", IniPath)) Then
				CmpFlag = True
				Exit Do
			End If
			Exit Do
		Loop 
		
		If CmpFlag Then
            Select Case MsgBox(IniPath & " は変更されています。保存しますか？", MsgBoxStyle.YesNoCancel + MsgBoxStyle.Exclamation, Me.Text)
                ' キャンセルされた場合
                Case 2
                    Cancel = True
                    ' はい
                Case 6
                    WritePrm(IniPath)
                    ' いいえ
                Case 7
                Case Else
            End Select
		End If
		
		eventArgs.Cancel = Cancel
	End Sub
	
	Public Sub mnuCreate_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuCreate.Click
		'UPGRADE_WARNING: CommonDialog 変数はアップグレードされませんでした 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="671167DC-EA81-475D-B690-7A40C7BF4A23"' をクリックしてください。
        With ComDlgSave
            'UPGRADE_WARNING: Filter に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
            .Filter = "設定ファイル (*.ini)|*.ini|すべてのファイル (*.*)|*.*"
            .FilterIndex = 1
            .InitialDirectory = GetPath(IniPath)
            'UPGRADE_WARNING: MSComDlg.CommonDialog プロパティ ComDlg.Flags は、新しい動作をもつ ComDlgSave.OverwritePrompt にアップグレードされました。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="DFCDE711-9694-47D7-9C50-45A99CD8E91E"' をクリックしてください。
            .OverwritePrompt = True

            .ShowDialog()

            'UPGRADE_WARNING: Dir に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
            If .FileName <> vbNullString And Dir(GetPath(.FileName), FileAttribute.Directory) <> vbNullString Then
                IniPath = .FileName
                WritePrm(IniPath)
            End If
        End With
	End Sub
	
	Public Sub mnuExit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuExit.Click
		Me.Close()
	End Sub
	
	Public Sub mnuOpen_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuOpen.Click
		'UPGRADE_WARNING: CommonDialog 変数はアップグレードされませんでした 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="671167DC-EA81-475D-B690-7A40C7BF4A23"' をクリックしてください。
        With ComDlgOpen
            'UPGRADE_WARNING: Filter に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
            .Filter = "設定ファイル (*.ini)|*.ini|すべてのファイル (*.*)|*.*"
            .FilterIndex = 1
            .InitialDirectory = My.Application.Info.DirectoryPath

            'COMMONDIALOG
            .ShowDialog()

            If ExistFile(.FileName) = True Then
                IniPath = .FileName
                GetPrm(IniPath)
            End If
        End With
		
	End Sub
	
	Public Sub mnuSave_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuSave.Click
		WritePrm(IniPath)
	End Sub
	
	Public Sub mnuVer_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuVer.Click
        MsgBox(My.Application.Info.Title & vbCrLf & vbCrLf & "Ver. " & My.Application.Info.Version.Major & "." & My.Application.Info.Version.Minor & "." & My.Application.Info.Version.Revision, , "バージョン情報")
	End Sub
	
	'UPGRADE_WARNING: イベント Option1.CheckedChanged は、フォームが初期化されたときに発生します。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"' をクリックしてください。
	Private Sub Option1_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Option1.CheckedChanged
		If eventSender.Checked Then
			Dim Index As Short = Option1.GetIndex(eventSender)
			Select Case Index
				Case 0
					TKZFile.Enabled = True
                    _Cmd_2.Enabled = True
                Case 1, 2, 3, 4
                    TKZFile.Enabled = False
                    _Cmd_2.Enabled = False
            End Select

            Select Case Index
                Case 2, 3
                    CBConvCheck.Enabled = False
                    TIFFPath.Enabled = False
                Case Else
                    CBConvCheck.Enabled = True
                    If CBConvCheck.CheckState = 1 Then
                        TIFFPath.Enabled = True
                    Else
                        TIFFPath.Enabled = False
                    End If
            End Select
        End If
    End Sub

    'UPGRADE_WARNING: イベント Option2.CheckedChanged は、フォームが初期化されたときに発生します。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"' をクリックしてください。
    Private Sub Option2_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Option2.CheckedChanged
        If eventSender.Checked Then
            Dim Index As Short = Option2.GetIndex(eventSender)
            Select Case Index
                Case 0
                    CBLog.Enabled = False
                    LogFile.Enabled = False
                    _Cmd_5.Enabled = False
                Case 1
                    CBLog.Enabled = True
                    If CBLog.CheckState = 1 Then
                        LogFile.Enabled = True
                        _Cmd_5.Enabled = True
                    Else
                        LogFile.Enabled = False
                        _Cmd_5.Enabled = False
                    End If
            End Select
        End If
    End Sub

    Private Sub CmdRun_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles CmdRun.Click
        Dim aTiffPath As String ' TIFF イメージのパス
        Dim aPngPath As String ' PNG  イメージのパス
        Dim aTKZFile As String ' 都計図CSVファイル名（フルパス）
        Dim aID() As String ' 図葉ID or メッシュ番号の配列

        Dim aMsg As String ' エラーメッセージ
        Dim aErrNum As Short ' エラー番号
        ' 0: エラー無し 1:座標系が不正 2:図葉IDが不正 3:メッシュコードが不正
        ' 4:TIFFファイルが無い 5:PNGがリフレッシュできない 6:PNGファイルが無い
        Dim aFileNum As Short ' ファイルアクセス番号
        Dim i As Integer ' リストのカウンター

        On Error GoTo CheckError

        aFileNum = -1
        CmdRun.Enabled = False
        CmdExit.Enabled = False

        ' 初期入力内容チェック

        ' TIFF イメージパス
        Dim aSrcPath As String
        If CBConvCheck.CheckState = 1 Then
            aTiffPath = TIFFPath.Text
            If ExistPath(aTiffPath) = False Then
                ErrDialog("エラー", "指定されたTIFFイメージパスがありません。")
                Exit Sub
            Else
                PlusEn(aTiffPath)
            End If
        End If
        ' PNG イメージパス
        aPngPath = PNGPath.Text
        Dim aTargetPath As String
        If ExistPath(aPngPath) = False Then
            ErrDialog("エラー", "指定されたPNGイメージパスがありません。")
            Exit Sub
        Else
            PlusEn(aPngPath)
        End If
        ' 都計図定義ファイル
        If Option1(0).Checked = True Then
            aTKZFile = TKZFile.Text
            If ExistFile(aTKZFile) = False Then
                ErrDialog("エラー", "指定された都計図定義ファイルがありません。")
                Exit Sub
            End If
        End If

        ' 処理方法によるチェック
        Select Case True
            Case Option2(0).Checked
                ReDim Preserve aID(0)
                aID(0) = GetPrefix(OutPutPath(0).Text)
            Case Option2(1).Checked
                If ExistFile(OutPutPath(0).Text) = False Then
                    ErrDialog("エラー", "指定されたファイルが見つかりません。")
                    Exit Sub
                ElseIf loadImgList(OutPutPath(0).Text, aID) = False Then
                    Exit Sub
                End If
                ' ログファイルの書込
                If CBLog.Enabled = True And CBLog.CheckState = 1 Then
                    aFileNum = openLogFile((LogFile.Text))
                End If
        End Select

        ' データの処理はここから
        Dim pWSF As ESRI.ArcGIS.Geodatabase.IWorkspaceFactory
        Dim mpts As ESRI.ArcGIS.Geometry.Multipoint
        Dim ToPnts As ESRI.ArcGIS.Geometry.IPointCollection
        Dim aPCS As Integer ' ESRIの19座標系のコード

        Dim aZone As Short ' 19座標系
        Dim aTKZLine() As String ' 都計図定義ファイル行データ
        Select Case True
            ' 都計図ラスターの場合
            Case Option1(0).Checked

                ' 都計図定義ファイルのロード
                If loadTKZFile(aTKZFile, aTKZLine) = False Then
                    CmdRun.Enabled = True
                    CmdExit.Enabled = True
                    Exit Sub
                End If
                ' メッシュラスターの場合
            Case Option1(1).Checked
            Case Option1(2).Checked
            Case Option1(3).Checked
            Case Option1(4).Checked
        End Select

        pWSF = New ESRI.ArcGIS.DataSourcesRaster.RasterWorkspaceFactory

        Dim localeInfo As String
        Dim aPrefix As String
        For i = 0 To UBound(aID)
            mpts = New ESRI.ArcGIS.Geometry.Multipoint
            ToPnts = mpts
            aErrNum = 0

            Select Case True
                Case Option1(0).Checked ' 都計図ラスターの場合
                    If searchTKZList(aTKZLine, aID(i), aZone, ToPnts) = True Then 'CSVから19座標系を取得
                        aPCS = SetPCSType(aZone) '19座標系のコードをセット
                        If aPCS = 0 Then
                            aErrNum = 1
                            aMsg = aID(i) & " : invalid coordinate system."
                            Debug.Print(aMsg)
                            If aFileNum >= 0 Then
                                PrintLine(aFileNum, aMsg)
                            ElseIf Option2(0).Checked Then
                                Exit For
                            End If
                            GoTo cont
                        End If
                    Else
                        aErrNum = 2
                        aMsg = aID(i) & " : TKZList ERROR."
                        Debug.Print(aMsg)
                        If aFileNum >= 0 Then
                            PrintLine(aFileNum, aMsg)
                        ElseIf Option2(0).Checked Then
                            Exit For
                        End If
                        GoTo cont
                    End If
                Case Option1(1).Checked ' メッシュラスターの場合
                    aPCS = 0
                    If MeshCodeToLonLat(WMeshf, Int(Val(aID(i))), ToPnts) = False Then
                        aErrNum = 3
                        aMsg = aID(i) & " : invalid MeshCode."
                        Debug.Print(aMsg)
                        If aFileNum >= 0 Then
                            PrintLine(aFileNum, aMsg)
                        ElseIf Option2(0).Checked Then
                            Exit For
                        End If
                        GoTo cont
                    End If
				Case Option1(2).Checked	' 航空写真（GS国土基本図図郭）の場合
					If IsNumeric(VB.Left(aID(i), 2)) = True Then
						aPCS = SetPCSType(Val(VB.Left(aID(i), 2)))
					Else
						aPCS = SetPCSType(Val(VB.Left(aID(i), 1)))
					End If
                Case Option1(3).Checked ' パスコの航空写真（メッシュ）の場合
                    aPCS = 0
                Case Option1(4).Checked ' 座標テキスト読込 Bug5122対応
                    ' リストの中身がファイル名かフルパス名かのチェック
                    If GetPrefix(aID(i)) <> aID(i) Then
                        localeInfo = GetPrefix(aID(i)) & ".txt"
                    Else
                        aPrefix = GetPrefix(aID(i))
                        If CBConvCheck.CheckState = 1 Then
                            localeInfo = aTiffPath & aPrefix & ".txt"
                        Else
                            localeInfo = aPngPath & aPrefix & ".txt"
                        End If
                    End If

                    If loadLocaleInfo(localeInfo, ToPnts) = False Then
                        aErrNum = 9
                        aMsg = aID(i) & " : invalid Locale Infomation file."
                        Debug.Print(aMsg)
                        If aFileNum >= 0 Then
                            PrintLine(aFileNum, aMsg)
                        ElseIf Option2(0).Checked Then
                            Exit For
                        End If
                        GoTo cont
                    End If
            End Select

            ' TargetPath の作成（PNG/JPEG のフルパス）
            aTargetPath = aPngPath
            Select Case True
                Case Option1(0).Checked
                    aTargetPath = aTargetPath & VB.Left(aID(i), 5)
                    PlusEn(aTargetPath)
                    aTargetPath = aTargetPath & aID(i)
                Case Option1(1).Checked
                    aTargetPath = aTargetPath & VB.Left(aID(i), 4)
                    PlusEn(aTargetPath)
                    aTargetPath = aTargetPath & aID(i)
                Case Option1(2).Checked
					' bug12236の対応により、3文字から2文字に変更
					aTargetPath = aTargetPath & VB.Left(aID(i), 2)
                    PlusEn(aTargetPath)
                    aTargetPath = aTargetPath & aID(i) & ".jpg"
                Case Option1(3).Checked
                    aTargetPath = aTargetPath & VB.Left(aID(i), 4)
                    PlusEn(aTargetPath)
                    aTargetPath = aTargetPath & aID(i) & ".jpg"
                Case Option1(4).Checked
                    If GetPrefix(aID(i)) <> aID(i) And GetFilename(aID(i)) <> vbNullString Then
                        aTargetPath = aID(i)
                    Else
                        aTargetPath = aTargetPath & aID(i)
                    End If
            End Select

            ' TIFF -> PNG 変換
            If CBConvCheck.Enabled = True Then
                If CBConvCheck.CheckState = 1 Then
                    aSrcPath = aTiffPath
                    Select Case True
                        Case Option1(0).Checked ' 都計図ラスター
                            aSrcPath = aSrcPath & VB.Left(aID(i), 5)
                            PlusEn(aSrcPath)
                            aSrcPath = aSrcPath & aID(i) & ".tif"
                        Case Option1(1).Checked ' メッシュ矩形ラスター
                            aSrcPath = aSrcPath & VB.Left(aID(i), 4)
                            PlusEn(aSrcPath)
                            aSrcPath = aSrcPath & aID(i) & ".tif"
                        Case Option1(4).Checked ' 座標テキスト読込
                            If GetPrefix(aID(i)) <> aID(i) And GetFilename(aID(i)) <> vbNullString Then
                                aSrcPath = aID(i)
                            Else
                                If GetSuffix(aID(i)) <> vbNullString Then
                                    aSrcPath = aSrcPath & aID(i)
                                Else
                                    PlusEn(aSrcPath)
                                    aSrcPath = aSrcPath & aID(i) & ".tif"
                                End If
                            End If
                    End Select

                    ' SrcPath のチェック
                    If ExistFile(aSrcPath) = False Then
                        aErrNum = 4
                        aMsg = aID(i) & " : TIFF image not found."
                        Debug.Print(aMsg)
                        If aFileNum >= 0 Then
                            PrintLine(aFileNum, aMsg)
                        ElseIf Option2(0).Checked Then
                            Exit For
                        End If
                        GoTo cont
                    End If
                    aTargetPath = GetPrefix(aTargetPath) & ".png"
                    If ConvertImage(aSrcPath, aTargetPath) = False Then
                        aErrNum = 5
                        If aFileNum >= 0 Then
                            PrintLine(aFileNum, aID(i) & " : PNG image not update.")
                        ElseIf Option2(0).Checked Then
                            Exit For
                        End If
                        GoTo cont
                    End If
                Else
                    If ExistFile(aTargetPath) Then

                    ElseIf ExistFile(aTargetPath & ".jpg") Then
                        aTargetPath = aTargetPath & ".jpg"
                    Else
                        aTargetPath = aTargetPath & ".png"
                    End If
                End If
            End If

            ' aTargetPath に PNG が作成された or PNG/JPGイメージが存在するのチェック
            If ExistFile(aTargetPath) = False Then
                aErrNum = 6
                aMsg = aID(i) & " : PNG/JPG image not found."
                Debug.Print(aMsg)
                If aFileNum >= 0 Then
                    PrintLine(aFileNum, aMsg)
                ElseIf Option2(0).Checked Then
                    Exit For
                End If
                GoTo cont
            End If

            ' 航空写真オルソの場合の処理
            If Option1(2).Checked Or Option1(3).Checked Then
                'UPGRADE_WARNING: Dir に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
                If Dir(VB.Left(aTargetPath, InStr(aTargetPath, ".") - 1) & ".jgw.org") = vbNullString Then
                    'UPGRADE_WARNING: Dir に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
                    If Dir(VB.Left(aTargetPath, InStr(aTargetPath, ".") - 1) & ".jgw") = vbNullString Then
                        aErrNum = 7
                        If aFileNum >= 0 Then
                            PrintLine(aFileNum, aID(i) & " : JGW file not found.")
                        ElseIf Option2(0).Checked Then
                            Exit For
                        End If
                        GoTo cont
                    Else
                        Rename(VB.Left(aTargetPath, InStr(aTargetPath, ".") - 1) & ".jgw", VB.Left(aTargetPath, InStr(aTargetPath, ".") - 1) & ".jgw.org")
                    End If
                End If
            End If

            ' .aux, .pgw or .jgw を削除
            If RefClean(aTargetPath) = False Then
                aErrNum = 5
                If aFileNum >= 0 Then
                    PrintLine(aFileNum, aID(i) & " : .aux file occupied.")
                ElseIf Option2(0).Checked Then
                    Exit For
                End If
                GoTo cont
            End If

            If Option1(2).Checked Or Option1(3).Checked Then
                Call ChangeRasterProperty(True, pWSF, aTargetPath, aPCS, ToPnts)
            Else
                Call ChangeRasterProperty(False, pWSF, aTargetPath, aPCS, ToPnts)
            End If
            If ExistFile(VB.Left(aTargetPath, InStr(aTargetPath, ".") - 1) & ".aux") Or ExistFile(aTargetPath & ".aux.xml") = False Then
                aErrNum = 8
                If aFileNum >= 0 Then
                    aMsg = aID(i) & " : image file ronly."
                    PrintLine(aFileNum, aMsg)
                ElseIf Option2(0).Checked Then
                    Exit For
                End If
                GoTo cont
            End If
            If aFileNum >= 0 Then
                aMsg = aID(i) & " : aux create successful."
                PrintLine(aFileNum, aMsg)
            End If

cont:

            '        Set pWSF = Nothing
            'UPGRADE_NOTE: オブジェクト mpts をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
            mpts = Nothing
            'UPGRADE_NOTE: オブジェクト ToPnts をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
            ToPnts = Nothing
        Next i

        ' オブジェクトのデストラクト処理
        'UPGRADE_NOTE: オブジェクト pWSF をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
        pWSF = Nothing
        'UPGRADE_NOTE: オブジェクト mpts をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
        mpts = Nothing
        'UPGRADE_NOTE: オブジェクト ToPnts をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
        ToPnts = Nothing

        If CBLog.Enabled = True And CBLog.CheckState = 1 Then
            closeLogFile(aFileNum)
        End If
        If aErrNum > 0 And Option2(0).Checked Then
            Select Case aErrNum
                Case 1
                    ErrDialog("エラー", "座標系が不正です。" & vbCrLf & vbCrLf & "auxファイルを作成できませんでした。")
                Case 2
                    ErrDialog("エラー", "図葉ID：" & aID(0) & "が不正です。" & vbCrLf & vbCrLf & "auxファイルを作成できませんでした。")
                Case 3
                    ErrDialog("エラー", "メッシュコード：" & aID(0) & "が不正です。" & vbCrLf & vbCrLf & "auxファイルを作成できませんでした。")
                Case 4
                    ErrDialog("エラー", "TIFF イメージ：" & aID(0) & "が見つかりません。" & vbCrLf & vbCrLf & "auxファイルを作成できませんでした。")
                Case 5
                    ErrDialog("エラー", "PNG イメージ：" & aID(0) & "を更新できません。" & vbCrLf & vbCrLf & "ラスターが使用中の可能性があります。")
                Case 6
                    ErrDialog("エラー", "PNG イメージ：" & aID(0) & "が見つかりません。" & vbCrLf & vbCrLf & "auxファイルを作成できませんでした。")
                Case 7
                    ErrDialog("エラー", "JGW ファイル：" & aID(0) & "が見つかりません。" & vbCrLf & vbCrLf & "auxファイルを作成できませんでした。")
                Case 8
                    ErrDialog("エラー", "AUX ファイル：" & aID(0) & "が作成できませんでした。" & vbCrLf & vbCrLf & "ラスターファイルが読取専用になっています。")
                Case 9
                    ErrDialog("エラー", "AUX ファイル：" & aID(0) & "が作成できませんでした。" & vbCrLf & vbCrLf & "位置情報ファイルが不正です。")
                Case Else
                    ErrDialog("エラー", "想定外のエラーです。" & vbCrLf & "作者に連絡してください。")
            End Select
        Else
            NormalMsg("AUX 出力", "おわったよ～ん。")
            CmdRun.Enabled = True
            CmdExit.Enabled = True
        End If

        Erase aTKZLine
        Erase aID

        Exit Sub

CheckError:
        MsgBox("予期しないエラー #" & VB6.Format(Err.Number) & " が発生しました。" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "エラー")
        Stop
        CmdRun.Enabled = True
        CmdExit.Enabled = True
    End Sub

    Private Sub CmdExit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles CmdExit.Click
        Me.Close()
    End Sub

    Private Sub ErrDialog(ByRef wTitle As String, ByRef wMsg As String)
        ErrMsg(wTitle, wMsg)
        CmdRun.Enabled = True
        CmdExit.Enabled = True
    End Sub
    Private Function GetPrm(ByRef wIniPath As String) As Boolean
        TKZFile.Text = GetProfilePrm(True, "INIT", "TKZFile", wIniPath)
        TIFFPath.Text = GetProfilePrm(True, "INIT", "TIFFPath", wIniPath)
        PNGPath.Text = GetProfilePrm(True, "INIT", "PNGPath", wIniPath)
        LogFile.Text = GetProfilePrm(True, "INIT", "LogFile", wIniPath)

        If CBool(GetProfilePrm(False, "INIT", "WideMesh", wIniPath)) Then
            WMeshf = 1
        Else
            WMeshf = 0
        End If

        Dim Ret As Short
        Ret = CShort(GetProfilePrm(False, "INIT", "Raster", wIniPath))
        Select Case Ret
            Case 0
                Option1(0).Checked = True
            Case 1
                Option1(1).Checked = True
            Case 2
                Option1(2).Checked = True
            Case 3
                Option1(3).Checked = True
            Case 4
                Option1(4).Checked = True
        End Select

        CBConvCheck.CheckState = CShort(GetProfilePrm(False, "INIT", "ConvFlag", wIniPath))

        Ret = CShort(GetProfilePrm(False, "INIT", "Output", wIniPath))
        If Ret = 0 Then
            Option2(0).Checked = True
        Else
            Option2(1).Checked = True
        End If


        ' 初期化パラメータによるフィールドのON/OFF
        Select Case True
            Case Option1(0).Checked
                TKZFile.Enabled = True
                _Cmd_2.Enabled = True
            Case Option1(1).Checked, Option1(2).Checked, Option1(3).Checked, Option1(4).Checked
                TKZFile.Enabled = False
                _Cmd_2.Enabled = False
        End Select

        Select Case True
            Case Option1(2).Checked, Option1(3).Checked
                CBConvCheck.Enabled = False
                TIFFPath.Enabled = False
            Case Else
                If CBConvCheck.CheckState = 1 Then
                    TIFFPath.Enabled = True
                    _Cmd_0.Enabled = True
                Else
                    TIFFPath.Enabled = False
                    _Cmd_0.Enabled = False
                End If
        End Select

        Select Case True
            Case Option2(0).Checked
                CBLog.Enabled = False
                LogFile.Enabled = False
                _Cmd_5.Enabled = False
            Case Option2(1).Checked
                LogFile.Enabled = True
                _Cmd_5.Enabled = True
		End Select
	End Function
	
	Private Function WritePrm(ByRef wIniPath As String) As Boolean
		Dim Ret As Boolean
		
		Select Case True
			Case Option1(0).Checked
				Ret = WriteProfilePrm("INIT", "Raster", "0", wIniPath)
			Case Option1(1).Checked
				Ret = WriteProfilePrm("INIT", "Raster", "1", wIniPath)
			Case Option1(2).Checked
				Ret = WriteProfilePrm("INIT", "Raster", "2", wIniPath)
			Case Option1(3).Checked
				Ret = WriteProfilePrm("INIT", "Raster", "3", wIniPath)
			Case Option1(4).Checked
				Ret = WriteProfilePrm("INIT", "Raster", "4", wIniPath)
		End Select
		
		Ret = WriteProfilePrm("INIT", "TKZFile", (TKZFile.Text), wIniPath)
		Ret = WriteProfilePrm("INIT", "ConvFlag", CStr(CBConvCheck.CheckState), wIniPath)
		Ret = WriteProfilePrm("INIT", "TIFFPath", (TIFFPath.Text), wIniPath)
		Ret = WriteProfilePrm("INIT", "PNGPath", (PNGPath.Text), wIniPath)
		
		Select Case True
			Case Option2(0).Checked
				Ret = WriteProfilePrm("INIT", "Output", "0", wIniPath)
			Case Option2(1).Checked
				Ret = WriteProfilePrm("INIT", "Output", "1", wIniPath)
		End Select
		
		Ret = WriteProfilePrm("INIT", "LogFile", (LogFile.Text), wIniPath)
		WritePrm = True
		Exit Function
	End Function
	
	Private Function openLogFile(ByRef cLogFile As String) As Short
		Dim aFile As Short
		
		On Error GoTo CheckError
		openLogFile = -1
		
		aFile = FreeFile
		
		'UPGRADE_WARNING: Dir に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
		If Dir(GetPath(cLogFile), FileAttribute.Directory) = vbNullString Then
			MkDir(GetPath(cLogFile))
		End If
		
		FileOpen(aFile, cLogFile, OpenMode.Output, , OpenShare.LockWrite)
		openLogFile = aFile
		'    Close #aFile
		Exit Function
		
CheckError: 
		FileError()
		openLogFile = -1
	End Function
	
	Private Function closeLogFile(ByRef cFile As Short) As Boolean
		
		On Error GoTo CheckError
		closeLogFile = False
		
		FileClose(cFile)
		
		closeLogFile = True
		Exit Function
		
CheckError: 
		FileError()
		closeLogFile = True
	End Function
	
	Private Function loadImgList(ByRef cProcList As String, ByRef cID() As String) As Boolean
		Dim Flag As Boolean
		
		Dim aFile As Short
		Dim i As Integer
		
		Dim aTxtLine() As String
		Dim aTmp() As String
		
		On Error GoTo CheckError
		loadImgList = True
		
		aFile = FreeFile
		i = 0
		
		FileOpen(aFile, cProcList, OpenMode.Input, , OpenShare.LockWrite)
		Flag = True
		Do While Not EOF(aFile)
			ReDim Preserve aTxtLine(i)
			aTxtLine(i) = LineInput(aFile)
			i = i + 1
		Loop 
		
		If i = 1 Then
			cID = Split(aTxtLine(0), Chr(10))
			If UBound(cID) <> 0 Then
				ReDim Preserve cID(UBound(cID) - 1)
			End If
		Else
			cID = VB6.CopyArray(aTxtLine)
		End If
		
		Erase aTxtLine
		FileClose(aFile)
		Flag = False
		
		Exit Function
CheckError: 
		FileError()
		If Flag = True Then FileClose(aFile)
		Erase aTxtLine
		loadImgList = False
	End Function
	
	Private Function searchTKZList(ByRef cTKZLine() As String, ByRef cID As String, ByRef cZone As Short, ByRef cToPnts As ESRI.ArcGIS.Geometry.IPointCollection) As Boolean
		Dim i As Integer
		Dim aPrm() As String
		Dim aFlag As Boolean
		
		On Error GoTo CheckError
		For i = 0 To UBound(cTKZLine)
			aPrm = Split(cTKZLine(i), ",")
			If Int(Val(aPrm(19))) = Int(Val(cID)) Then
				aFlag = True
				Exit For
			End If
		Next 
		
		Dim ULpntTo As ESRI.ArcGIS.Geometry.IPoint
		Dim LLpntTo As ESRI.ArcGIS.Geometry.IPoint
		Dim LRpntTo As ESRI.ArcGIS.Geometry.IPoint
		Dim URpntTo As ESRI.ArcGIS.Geometry.IPoint
		If aFlag = False Then
            'ErrMsg "エラー", "市区町村コード" & cID & "が見つかりませんでした。"
			searchTKZList = False
		Else
			cZone = Val(aPrm(10))
			
            '4隅ピクセル中心座標（地図座標系）
			ULpntTo = New ESRI.ArcGIS.Geometry.Point : LLpntTo = New ESRI.ArcGIS.Geometry.Point : LRpntTo = New ESRI.ArcGIS.Geometry.Point : URpntTo = New ESRI.ArcGIS.Geometry.Point
			ULpntTo.X = Val(aPrm(12)) * 1000 : ULpntTo.Y = Val(aPrm(11)) * 1000
			LLpntTo.X = Val(aPrm(14)) * 1000 : LLpntTo.Y = Val(aPrm(13)) * 1000
			URpntTo.X = Val(aPrm(16)) * 1000 : URpntTo.Y = Val(aPrm(15)) * 1000
			LRpntTo.X = Val(aPrm(18)) * 1000 : LRpntTo.Y = Val(aPrm(17)) * 1000
			
            'リンクベクトルの作成（「ToPnts」が地図座標）
			cToPnts.AddPoint(ULpntTo)
			cToPnts.AddPoint(LLpntTo)
			cToPnts.AddPoint(LRpntTo)
			cToPnts.AddPoint(URpntTo)
			
			searchTKZList = True
		End If
		Erase aPrm
		
		Exit Function
CheckError: 
        MsgBox("予期しないエラー #" & VB6.Format(Err.Number) & " が発生しました。" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "エラー")
		Erase aPrm
		Stop
	End Function


End Class