Option Strict Off
Option Explicit On
Imports VB = Microsoft.VisualBasic
Friend Class Form1
	Inherits System.Windows.Forms.Form
    Dim IniPath As String ' �������t�@�C����
	Dim WMeshf As Short
	
	
	'UPGRADE_WARNING: �C�x���g CBConvCheck.CheckStateChanged �́A�t�H�[�������������ꂽ�Ƃ��ɔ������܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"' ���N���b�N���Ă��������B
	Private Sub CBConvCheck_CheckStateChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles CBConvCheck.CheckStateChanged
		If CBConvCheck.CheckState = 0 Then
			TIFFPath.Enabled = False
            _Cmd_0.Enabled = False
		Else
			TIFFPath.Enabled = True
            _Cmd_0.Enabled = True
		End If
	End Sub
	
	'UPGRADE_WARNING: �C�x���g CBLog.CheckStateChanged �́A�t�H�[�������������ꂽ�Ƃ��ɔ������܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"' ���N���b�N���Ă��������B
	Private Sub CBLog_CheckStateChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles CBLog.CheckStateChanged
		If CBLog.CheckState = 0 Then
			LogFile.Enabled = False
            _Cmd_5.Enabled = False '���O�̏o�͂ɂ���Q�ƃ{�^��
		Else
			LogFile.Enabled = True
            _Cmd_5.Enabled = True
		End If
	End Sub
	
	Private Sub Form1_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
        Dim CmdPrm() As String ' �R�}���h���C������
		
        ' �������p�����[�^�t�@�C���̃`�F�b�N
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
		
        ' �������p�����[�^�t�@�C���̓Ǎ�
		GetPrm(IniPath)
		
        ' ���X�^�[�ϊ��v���O�����ii_view32.exe�j�̑��݃`�F�b�N
        If Not System.IO.File.Exists(My.Application.Info.DirectoryPath & "\i_view32.exe") Then
            If MsgBox("���X�^�[�ϊ��v���O����" & vbCrLf & vbCrLf & GetAppPath() & "\i_view32.exe" & vbCrLf & vbCrLf & "��������Ȃ����� TIFF/PNG �ϊ��@�\�͎g�p�ł��܂���B" & vbCrLf & "���s���܂����H", vbExclamation + vbYesNo, My.Application.Info.Title) = vbYes Then
                CBConvCheck.Enabled = False
                TIFFPath.Enabled = False
                _Cmd_0.Enabled = False
            Else
                Me.Close()
                Exit Sub
            End If
        End If

        ' ���C�Z���X�F��
        Dim pMgr As ArcGISVersionLib.IArcGISVersion
        pMgr = New ArcGISVersionLib.VersionManager
        If Not pMgr.LoadVersion(ArcGISVersionLib.esriProductCode.esriArcGISDesktop, "") Then
            MsgBox("ArcGIS�̃��C�Z���X�F�؂Ɏ��s���܂���")
            End
        End If
        Dim pInit As ESRI.ArcGIS.esriSystem.IAoInitialize
        pInit = New ESRI.ArcGIS.esriSystem.AoInitialize
        If pInit.IsProductCodeAvailable(ESRI.ArcGIS.esriSystem.esriLicenseProductCode.esriLicenseProductCodeStandard) Then
            pInit.Initialize(ESRI.ArcGIS.esriSystem.esriLicenseProductCode.esriLicenseProductCodeStandard)
        Else
            MsgBox("ArcGIS�̃��C�Z���X�F�؂Ɏ��s���܂���")
        End If
    End Sub
	
	Private Sub Cmd_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Cmd.Click
		Dim Index As Short = Cmd.GetIndex(eventSender)
        With ComDlgOpen
            Select Case Index
                Case 0
                    'UPGRADE_WARNING: Filter �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
                    .Filter = "TIFF �C���[�W (*.tif)|*.tif|���ׂẴt�@�C�� (*.*)|*.*"
                    .FilterIndex = 2
                    .InitialDirectory = TIFFPath.Text
                    .CheckPathExists = True
                    .CheckPathExists = True
                Case 1
                    'UPGRADE_WARNING: Filter �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
                    .Filter = "PNG �C���[�W (*.png)|*.png|JPEG �C���[�W (*.jpg)|*.jpg|���ׂẴt�@�C�� (*.*)|*.*"
                    .FilterIndex = 1
                    .InitialDirectory = PNGPath.Text
                    .CheckPathExists = True
                    .CheckPathExists = True
                Case 2
                    'UPGRADE_WARNING: Filter �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
                    .Filter = "CSV �t�@�C�� (*.csv)|*.csv|�e�L�X�g �t�@�C�� (*.txt)|*.txt|���ׂẴt�@�C�� (*.*)|*.*"
                    .FilterIndex = 1
                    .InitialDirectory = TKZFile.Text
                    'UPGRADE_WARNING: MSComDlg.CommonDialog �v���p�e�B ComDlg.Flags �́A�V������������� ComDlgOpen.CheckFileExists �ɃA�b�v�O���[�h����܂����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="DFCDE711-9694-47D7-9C50-45A99CD8E91E"' ���N���b�N���Ă��������B
                    .CheckFileExists = True
                    .CheckPathExists = True
                    .CheckPathExists = True
                Case 3
                    Select Case True
                        Case Option2(0).Checked
                            If CBConvCheck.CheckState = 1 Then
                                'UPGRADE_WARNING: Filter �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
                                .Filter = "TIFF �C���[�W (*.tif)|*.tif|���ׂẴt�@�C�� (*.*)|*.*"
                                .InitialDirectory = TIFFPath.Text
                            Else
                                'UPGRADE_WARNING: Filter �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
                                .Filter = "PNG �C���[�W (*.png)|*.png|JPEG �C���[�W (*.jpg)|*.jpg|���ׂẴt�@�C�� (*.*)|*.*"
                                .InitialDirectory = PNGPath.Text
                            End If

                            If Option1(2).Checked Or Option1(3).Checked Then
                                .FilterIndex = 2
                            Else
                                .FilterIndex = 1
                            End If
                        Case Option2(1).Checked
                            'UPGRADE_WARNING: Filter �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
                            .Filter = "�e�L�X�g �t�@�C�� (*.txt)|*.txt|���ׂẴt�@�C�� (*.*)|*.*"
                            .FilterIndex = 1
                            .InitialDirectory = vbNullString
                            'UPGRADE_WARNING: MSComDlg.CommonDialog �v���p�e�B ComDlg.Flags �́A�V������������� ComDlgOpen.CheckFileExists �ɃA�b�v�O���[�h����܂����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="DFCDE711-9694-47D7-9C50-45A99CD8E91E"' ���N���b�N���Ă��������B
                            .CheckFileExists = True
                            .CheckPathExists = True
                            .CheckPathExists = True
                    End Select
                    '    Case 4
                    '�e�L�X�g �t�@�C�� (*.txt)|*.txt|���ׂẴt�@�C�� (*.*)|*.*
                    '        .FilterIndex = 1
                    '        '.InitDir = vbNullString
                    '        .Flags = cdlOFNFileMustExist
                Case 5
                    'UPGRADE_WARNING: Filter �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
                    .Filter = "���O �t�@�C�� (*.log)|*.log|�e�L�X�g �t�@�C�� (*.txt)|*.txt|���ׂẴt�@�C�� (*.*)|*.*"
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
        ' �p�����[�^�̃`�F�b�N
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
            Select Case MsgBox(IniPath & " �͕ύX����Ă��܂��B�ۑ����܂����H", MsgBoxStyle.YesNoCancel + MsgBoxStyle.Exclamation, Me.Text)
                ' �L�����Z�����ꂽ�ꍇ
                Case 2
                    Cancel = True
                    ' �͂�
                Case 6
                    WritePrm(IniPath)
                    ' ������
                Case 7
                Case Else
            End Select
		End If
		
		eventArgs.Cancel = Cancel
	End Sub
	
	Public Sub mnuCreate_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuCreate.Click
		'UPGRADE_WARNING: CommonDialog �ϐ��̓A�b�v�O���[�h����܂���ł��� �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="671167DC-EA81-475D-B690-7A40C7BF4A23"' ���N���b�N���Ă��������B
        With ComDlgSave
            'UPGRADE_WARNING: Filter �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
            .Filter = "�ݒ�t�@�C�� (*.ini)|*.ini|���ׂẴt�@�C�� (*.*)|*.*"
            .FilterIndex = 1
            .InitialDirectory = GetPath(IniPath)
            'UPGRADE_WARNING: MSComDlg.CommonDialog �v���p�e�B ComDlg.Flags �́A�V������������� ComDlgSave.OverwritePrompt �ɃA�b�v�O���[�h����܂����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="DFCDE711-9694-47D7-9C50-45A99CD8E91E"' ���N���b�N���Ă��������B
            .OverwritePrompt = True

            .ShowDialog()

            'UPGRADE_WARNING: Dir �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
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
		'UPGRADE_WARNING: CommonDialog �ϐ��̓A�b�v�O���[�h����܂���ł��� �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="671167DC-EA81-475D-B690-7A40C7BF4A23"' ���N���b�N���Ă��������B
        With ComDlgOpen
            'UPGRADE_WARNING: Filter �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
            .Filter = "�ݒ�t�@�C�� (*.ini)|*.ini|���ׂẴt�@�C�� (*.*)|*.*"
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
        MsgBox(My.Application.Info.Title & vbCrLf & vbCrLf & "Ver. " & My.Application.Info.Version.Major & "." & My.Application.Info.Version.Minor & "." & My.Application.Info.Version.Revision, , "�o�[�W�������")
	End Sub
	
	'UPGRADE_WARNING: �C�x���g Option1.CheckedChanged �́A�t�H�[�������������ꂽ�Ƃ��ɔ������܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"' ���N���b�N���Ă��������B
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

    'UPGRADE_WARNING: �C�x���g Option2.CheckedChanged �́A�t�H�[�������������ꂽ�Ƃ��ɔ������܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"' ���N���b�N���Ă��������B
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
        Dim aTiffPath As String ' TIFF �C���[�W�̃p�X
        Dim aPngPath As String ' PNG  �C���[�W�̃p�X
        Dim aTKZFile As String ' �s�v�}CSV�t�@�C�����i�t���p�X�j
        Dim aID() As String ' �}�tID or ���b�V���ԍ��̔z��

        Dim aMsg As String ' �G���[���b�Z�[�W
        Dim aErrNum As Short ' �G���[�ԍ�
        ' 0: �G���[���� 1:���W�n���s�� 2:�}�tID���s�� 3:���b�V���R�[�h���s��
        ' 4:TIFF�t�@�C�������� 5:PNG�����t���b�V���ł��Ȃ� 6:PNG�t�@�C��������
        Dim aFileNum As Short ' �t�@�C���A�N�Z�X�ԍ�
        Dim i As Integer ' ���X�g�̃J�E���^�[

        On Error GoTo CheckError

        aFileNum = -1
        CmdRun.Enabled = False
        CmdExit.Enabled = False

        ' �������͓��e�`�F�b�N

        ' TIFF �C���[�W�p�X
        Dim aSrcPath As String
        If CBConvCheck.CheckState = 1 Then
            aTiffPath = TIFFPath.Text
            If ExistPath(aTiffPath) = False Then
                ErrDialog("�G���[", "�w�肳�ꂽTIFF�C���[�W�p�X������܂���B")
                Exit Sub
            Else
                PlusEn(aTiffPath)
            End If
        End If
        ' PNG �C���[�W�p�X
        aPngPath = PNGPath.Text
        Dim aTargetPath As String
        If ExistPath(aPngPath) = False Then
            ErrDialog("�G���[", "�w�肳�ꂽPNG�C���[�W�p�X������܂���B")
            Exit Sub
        Else
            PlusEn(aPngPath)
        End If
        ' �s�v�}��`�t�@�C��
        If Option1(0).Checked = True Then
            aTKZFile = TKZFile.Text
            If ExistFile(aTKZFile) = False Then
                ErrDialog("�G���[", "�w�肳�ꂽ�s�v�}��`�t�@�C��������܂���B")
                Exit Sub
            End If
        End If

        ' �������@�ɂ��`�F�b�N
        Select Case True
            Case Option2(0).Checked
                ReDim Preserve aID(0)
                aID(0) = GetPrefix(OutPutPath(0).Text)
            Case Option2(1).Checked
                If ExistFile(OutPutPath(0).Text) = False Then
                    ErrDialog("�G���[", "�w�肳�ꂽ�t�@�C����������܂���B")
                    Exit Sub
                ElseIf loadImgList(OutPutPath(0).Text, aID) = False Then
                    Exit Sub
                End If
                ' ���O�t�@�C���̏���
                If CBLog.Enabled = True And CBLog.CheckState = 1 Then
                    aFileNum = openLogFile((LogFile.Text))
                End If
        End Select

        ' �f�[�^�̏����͂�������
        Dim pWSF As ESRI.ArcGIS.Geodatabase.IWorkspaceFactory
        Dim mpts As ESRI.ArcGIS.Geometry.Multipoint
        Dim ToPnts As ESRI.ArcGIS.Geometry.IPointCollection
        Dim aPCS As Integer ' ESRI��19���W�n�̃R�[�h

        Dim aZone As Short ' 19���W�n
        Dim aTKZLine() As String ' �s�v�}��`�t�@�C���s�f�[�^
        Select Case True
            ' �s�v�}���X�^�[�̏ꍇ
            Case Option1(0).Checked

                ' �s�v�}��`�t�@�C���̃��[�h
                If loadTKZFile(aTKZFile, aTKZLine) = False Then
                    CmdRun.Enabled = True
                    CmdExit.Enabled = True
                    Exit Sub
                End If
                ' ���b�V�����X�^�[�̏ꍇ
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
                Case Option1(0).Checked ' �s�v�}���X�^�[�̏ꍇ
                    If searchTKZList(aTKZLine, aID(i), aZone, ToPnts) = True Then 'CSV����19���W�n���擾
                        aPCS = SetPCSType(aZone) '19���W�n�̃R�[�h���Z�b�g
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
                Case Option1(1).Checked ' ���b�V�����X�^�[�̏ꍇ
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
				Case Option1(2).Checked	' �q��ʐ^�iGS���y��{�}�}�s�j�̏ꍇ
					If IsNumeric(VB.Left(aID(i), 2)) = True Then
						aPCS = SetPCSType(Val(VB.Left(aID(i), 2)))
					Else
						aPCS = SetPCSType(Val(VB.Left(aID(i), 1)))
					End If
                Case Option1(3).Checked ' �p�X�R�̍q��ʐ^�i���b�V���j�̏ꍇ
                    aPCS = 0
                Case Option1(4).Checked ' ���W�e�L�X�g�Ǎ� Bug5122�Ή�
                    ' ���X�g�̒��g���t�@�C�������t���p�X�����̃`�F�b�N
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

            ' TargetPath �̍쐬�iPNG/JPEG �̃t���p�X�j
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
					' bug12236�̑Ή��ɂ��A3��������2�����ɕύX
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

            ' TIFF -> PNG �ϊ�
            If CBConvCheck.Enabled = True Then
                If CBConvCheck.CheckState = 1 Then
                    aSrcPath = aTiffPath
                    Select Case True
                        Case Option1(0).Checked ' �s�v�}���X�^�[
                            aSrcPath = aSrcPath & VB.Left(aID(i), 5)
                            PlusEn(aSrcPath)
                            aSrcPath = aSrcPath & aID(i) & ".tif"
                        Case Option1(1).Checked ' ���b�V����`���X�^�[
                            aSrcPath = aSrcPath & VB.Left(aID(i), 4)
                            PlusEn(aSrcPath)
                            aSrcPath = aSrcPath & aID(i) & ".tif"
                        Case Option1(4).Checked ' ���W�e�L�X�g�Ǎ�
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

                    ' SrcPath �̃`�F�b�N
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

            ' aTargetPath �� PNG ���쐬���ꂽ or PNG/JPG�C���[�W�����݂���̃`�F�b�N
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

            ' �q��ʐ^�I���\�̏ꍇ�̏���
            If Option1(2).Checked Or Option1(3).Checked Then
                'UPGRADE_WARNING: Dir �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
                If Dir(VB.Left(aTargetPath, InStr(aTargetPath, ".") - 1) & ".jgw.org") = vbNullString Then
                    'UPGRADE_WARNING: Dir �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
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

            ' .aux, .pgw or .jgw ���폜
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
            'UPGRADE_NOTE: �I�u�W�F�N�g mpts ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
            mpts = Nothing
            'UPGRADE_NOTE: �I�u�W�F�N�g ToPnts ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
            ToPnts = Nothing
        Next i

        ' �I�u�W�F�N�g�̃f�X�g���N�g����
        'UPGRADE_NOTE: �I�u�W�F�N�g pWSF ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
        pWSF = Nothing
        'UPGRADE_NOTE: �I�u�W�F�N�g mpts ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
        mpts = Nothing
        'UPGRADE_NOTE: �I�u�W�F�N�g ToPnts ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
        ToPnts = Nothing

        If CBLog.Enabled = True And CBLog.CheckState = 1 Then
            closeLogFile(aFileNum)
        End If
        If aErrNum > 0 And Option2(0).Checked Then
            Select Case aErrNum
                Case 1
                    ErrDialog("�G���[", "���W�n���s���ł��B" & vbCrLf & vbCrLf & "aux�t�@�C�����쐬�ł��܂���ł����B")
                Case 2
                    ErrDialog("�G���[", "�}�tID�F" & aID(0) & "���s���ł��B" & vbCrLf & vbCrLf & "aux�t�@�C�����쐬�ł��܂���ł����B")
                Case 3
                    ErrDialog("�G���[", "���b�V���R�[�h�F" & aID(0) & "���s���ł��B" & vbCrLf & vbCrLf & "aux�t�@�C�����쐬�ł��܂���ł����B")
                Case 4
                    ErrDialog("�G���[", "TIFF �C���[�W�F" & aID(0) & "��������܂���B" & vbCrLf & vbCrLf & "aux�t�@�C�����쐬�ł��܂���ł����B")
                Case 5
                    ErrDialog("�G���[", "PNG �C���[�W�F" & aID(0) & "���X�V�ł��܂���B" & vbCrLf & vbCrLf & "���X�^�[���g�p���̉\��������܂��B")
                Case 6
                    ErrDialog("�G���[", "PNG �C���[�W�F" & aID(0) & "��������܂���B" & vbCrLf & vbCrLf & "aux�t�@�C�����쐬�ł��܂���ł����B")
                Case 7
                    ErrDialog("�G���[", "JGW �t�@�C���F" & aID(0) & "��������܂���B" & vbCrLf & vbCrLf & "aux�t�@�C�����쐬�ł��܂���ł����B")
                Case 8
                    ErrDialog("�G���[", "AUX �t�@�C���F" & aID(0) & "���쐬�ł��܂���ł����B" & vbCrLf & vbCrLf & "���X�^�[�t�@�C�����ǎ��p�ɂȂ��Ă��܂��B")
                Case 9
                    ErrDialog("�G���[", "AUX �t�@�C���F" & aID(0) & "���쐬�ł��܂���ł����B" & vbCrLf & vbCrLf & "�ʒu���t�@�C�����s���ł��B")
                Case Else
                    ErrDialog("�G���[", "�z��O�̃G���[�ł��B" & vbCrLf & "��҂ɘA�����Ă��������B")
            End Select
        Else
            NormalMsg("AUX �o��", "���������`��B")
            CmdRun.Enabled = True
            CmdExit.Enabled = True
        End If

        Erase aTKZLine
        Erase aID

        Exit Sub

CheckError:
        MsgBox("�\�����Ȃ��G���[ #" & VB6.Format(Err.Number) & " ���������܂����B" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "�G���[")
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


        ' �������p�����[�^�ɂ��t�B�[���h��ON/OFF
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
		
		'UPGRADE_WARNING: Dir �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
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
            'ErrMsg "�G���[", "�s�撬���R�[�h" & cID & "��������܂���ł����B"
			searchTKZList = False
		Else
			cZone = Val(aPrm(10))
			
            '4���s�N�Z�����S���W�i�n�}���W�n�j
			ULpntTo = New ESRI.ArcGIS.Geometry.Point : LLpntTo = New ESRI.ArcGIS.Geometry.Point : LRpntTo = New ESRI.ArcGIS.Geometry.Point : URpntTo = New ESRI.ArcGIS.Geometry.Point
			ULpntTo.X = Val(aPrm(12)) * 1000 : ULpntTo.Y = Val(aPrm(11)) * 1000
			LLpntTo.X = Val(aPrm(14)) * 1000 : LLpntTo.Y = Val(aPrm(13)) * 1000
			URpntTo.X = Val(aPrm(16)) * 1000 : URpntTo.Y = Val(aPrm(15)) * 1000
			LRpntTo.X = Val(aPrm(18)) * 1000 : LRpntTo.Y = Val(aPrm(17)) * 1000
			
            '�����N�x�N�g���̍쐬�i�uToPnts�v���n�}���W�j
			cToPnts.AddPoint(ULpntTo)
			cToPnts.AddPoint(LLpntTo)
			cToPnts.AddPoint(LRpntTo)
			cToPnts.AddPoint(URpntTo)
			
			searchTKZList = True
		End If
		Erase aPrm
		
		Exit Function
CheckError: 
        MsgBox("�\�����Ȃ��G���[ #" & VB6.Format(Err.Number) & " ���������܂����B" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "�G���[")
		Erase aPrm
		Stop
	End Function


End Class