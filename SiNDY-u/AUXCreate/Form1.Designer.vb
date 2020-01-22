<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> Partial Class Form1
#Region "Windows フォーム デザイナによって生成されたコード "
	<System.Diagnostics.DebuggerNonUserCode()> Public Sub New()
		MyBase.New()
		'この呼び出しは、Windows フォーム デザイナで必要です。
		InitializeComponent()
	End Sub
	'Form は、コンポーネント一覧に後処理を実行するために dispose をオーバーライドします。
	<System.Diagnostics.DebuggerNonUserCode()> Protected Overloads Overrides Sub Dispose(ByVal Disposing As Boolean)
		If Disposing Then
			If Not components Is Nothing Then
				components.Dispose()
			End If
		End If
		MyBase.Dispose(Disposing)
	End Sub
	'Windows フォーム デザイナで必要です。
	Private components As System.ComponentModel.IContainer
	Public ToolTip1 As System.Windows.Forms.ToolTip
	Public WithEvents mnuOpen As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuSave As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuCreate As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents separater As System.Windows.Forms.ToolStripSeparator
	Public WithEvents mnuExit As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuFile As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuVer As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuHelp As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents MainMenu1 As System.Windows.Forms.MenuStrip
	Public WithEvents CBLog As System.Windows.Forms.CheckBox
	Public WithEvents CBConvCheck As System.Windows.Forms.CheckBox
	Public WithEvents CmdExit As System.Windows.Forms.Button
	Public WithEvents CmdRun As System.Windows.Forms.Button
	Public WithEvents _Cmd_4 As System.Windows.Forms.Button
	Public WithEvents _Option2_0 As System.Windows.Forms.RadioButton
	Public WithEvents _Option2_1 As System.Windows.Forms.RadioButton
	Public WithEvents _OutPutPath_0 As System.Windows.Forms.TextBox
	Public WithEvents _Cmd_3 As System.Windows.Forms.Button
	Public WithEvents Frame5 As System.Windows.Forms.GroupBox
	Public WithEvents _Option1_4 As System.Windows.Forms.RadioButton
	Public WithEvents _Option1_3 As System.Windows.Forms.RadioButton
	Public WithEvents _Option1_2 As System.Windows.Forms.RadioButton
	Public WithEvents TKZFile As System.Windows.Forms.TextBox
	Public WithEvents _Cmd_2 As System.Windows.Forms.Button
	Public WithEvents _Option1_1 As System.Windows.Forms.RadioButton
	Public WithEvents _Option1_0 As System.Windows.Forms.RadioButton
	Public WithEvents Label2 As System.Windows.Forms.Label
	Public WithEvents Frame1 As System.Windows.Forms.GroupBox
	Public WithEvents TIFFPath As System.Windows.Forms.TextBox
	Public WithEvents _Cmd_0 As System.Windows.Forms.Button
	Public WithEvents Label1 As System.Windows.Forms.Label
	Public WithEvents Frame2 As System.Windows.Forms.GroupBox
	Public WithEvents PNGPath As System.Windows.Forms.TextBox
	Public WithEvents _Cmd_1 As System.Windows.Forms.Button
	Public WithEvents Frame3 As System.Windows.Forms.GroupBox
	Public ComDlgOpen As System.Windows.Forms.OpenFileDialog
	Public ComDlgSave As System.Windows.Forms.SaveFileDialog
	Public WithEvents _Cmd_5 As System.Windows.Forms.Button
	Public WithEvents LogFile As System.Windows.Forms.TextBox
	Public WithEvents Label3 As System.Windows.Forms.Label
	Public WithEvents Frame4 As System.Windows.Forms.GroupBox
	Public WithEvents Cmd As Microsoft.VisualBasic.Compatibility.VB6.ButtonArray
	Public WithEvents Option1 As Microsoft.VisualBasic.Compatibility.VB6.RadioButtonArray
	Public WithEvents Option2 As Microsoft.VisualBasic.Compatibility.VB6.RadioButtonArray
	Public WithEvents OutPutPath As Microsoft.VisualBasic.Compatibility.VB6.TextBoxArray
	'メモ: 以下のプロシージャは Windows フォーム デザイナで必要です。
	'Windows フォーム デザイナを使って変更できます。
	'コード エディタを使用して、変更しないでください。
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(Me.components)
		Me.MainMenu1 = New System.Windows.Forms.MenuStrip()
		Me.mnuFile = New System.Windows.Forms.ToolStripMenuItem()
		Me.mnuOpen = New System.Windows.Forms.ToolStripMenuItem()
		Me.mnuSave = New System.Windows.Forms.ToolStripMenuItem()
		Me.mnuCreate = New System.Windows.Forms.ToolStripMenuItem()
		Me.separater = New System.Windows.Forms.ToolStripSeparator()
		Me.mnuExit = New System.Windows.Forms.ToolStripMenuItem()
		Me.mnuHelp = New System.Windows.Forms.ToolStripMenuItem()
		Me.mnuVer = New System.Windows.Forms.ToolStripMenuItem()
		Me.CBLog = New System.Windows.Forms.CheckBox()
		Me.CBConvCheck = New System.Windows.Forms.CheckBox()
		Me.CmdExit = New System.Windows.Forms.Button()
		Me.CmdRun = New System.Windows.Forms.Button()
		Me.Frame5 = New System.Windows.Forms.GroupBox()
		Me._Cmd_4 = New System.Windows.Forms.Button()
		Me._Option2_0 = New System.Windows.Forms.RadioButton()
		Me._Option2_1 = New System.Windows.Forms.RadioButton()
		Me._OutPutPath_0 = New System.Windows.Forms.TextBox()
		Me._Cmd_3 = New System.Windows.Forms.Button()
		Me.Frame1 = New System.Windows.Forms.GroupBox()
		Me._Option1_4 = New System.Windows.Forms.RadioButton()
		Me._Option1_3 = New System.Windows.Forms.RadioButton()
		Me._Option1_2 = New System.Windows.Forms.RadioButton()
		Me.TKZFile = New System.Windows.Forms.TextBox()
		Me._Cmd_2 = New System.Windows.Forms.Button()
		Me._Option1_1 = New System.Windows.Forms.RadioButton()
		Me._Option1_0 = New System.Windows.Forms.RadioButton()
		Me.Label2 = New System.Windows.Forms.Label()
		Me.Frame2 = New System.Windows.Forms.GroupBox()
		Me.TIFFPath = New System.Windows.Forms.TextBox()
		Me._Cmd_0 = New System.Windows.Forms.Button()
		Me.Label1 = New System.Windows.Forms.Label()
		Me.Frame3 = New System.Windows.Forms.GroupBox()
		Me.PNGPath = New System.Windows.Forms.TextBox()
		Me._Cmd_1 = New System.Windows.Forms.Button()
		Me.ComDlgOpen = New System.Windows.Forms.OpenFileDialog()
		Me.ComDlgSave = New System.Windows.Forms.SaveFileDialog()
		Me.Frame4 = New System.Windows.Forms.GroupBox()
		Me._Cmd_5 = New System.Windows.Forms.Button()
		Me.LogFile = New System.Windows.Forms.TextBox()
		Me.Label3 = New System.Windows.Forms.Label()
		Me.Cmd = New Microsoft.VisualBasic.Compatibility.VB6.ButtonArray(Me.components)
		Me.Option1 = New Microsoft.VisualBasic.Compatibility.VB6.RadioButtonArray(Me.components)
		Me.Option2 = New Microsoft.VisualBasic.Compatibility.VB6.RadioButtonArray(Me.components)
		Me.OutPutPath = New Microsoft.VisualBasic.Compatibility.VB6.TextBoxArray(Me.components)
		Me.MainMenu1.SuspendLayout()
		Me.Frame5.SuspendLayout()
		Me.Frame1.SuspendLayout()
		Me.Frame2.SuspendLayout()
		Me.Frame3.SuspendLayout()
		Me.Frame4.SuspendLayout()
		CType(Me.Cmd, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.Option1, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.Option2, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.OutPutPath, System.ComponentModel.ISupportInitialize).BeginInit()
		Me.SuspendLayout()
		'
		'MainMenu1
		'
		Me.MainMenu1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.mnuFile, Me.mnuHelp})
		Me.MainMenu1.Location = New System.Drawing.Point(0, 0)
		Me.MainMenu1.Name = "MainMenu1"
		Me.MainMenu1.Size = New System.Drawing.Size(389, 26)
		Me.MainMenu1.TabIndex = 26
		'
		'mnuFile
		'
		Me.mnuFile.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.mnuOpen, Me.mnuSave, Me.mnuCreate, Me.separater, Me.mnuExit})
		Me.mnuFile.Name = "mnuFile"
		Me.mnuFile.Size = New System.Drawing.Size(85, 22)
		Me.mnuFile.Text = "ファイル(&F)"
		'
		'mnuOpen
		'
		Me.mnuOpen.Name = "mnuOpen"
		Me.mnuOpen.ShortcutKeys = CType((System.Windows.Forms.Keys.Control Or System.Windows.Forms.Keys.O), System.Windows.Forms.Keys)
		Me.mnuOpen.Size = New System.Drawing.Size(202, 22)
		Me.mnuOpen.Text = "開く(&O)"
		'
		'mnuSave
		'
		Me.mnuSave.Name = "mnuSave"
		Me.mnuSave.ShortcutKeys = CType((System.Windows.Forms.Keys.Control Or System.Windows.Forms.Keys.S), System.Windows.Forms.Keys)
		Me.mnuSave.Size = New System.Drawing.Size(202, 22)
		Me.mnuSave.Text = "上書き保存(&S)"
		'
		'mnuCreate
		'
		Me.mnuCreate.Name = "mnuCreate"
		Me.mnuCreate.Size = New System.Drawing.Size(202, 22)
		Me.mnuCreate.Text = "名前を付けて保存(&A)..."
		'
		'separater
		'
		Me.separater.Name = "separater"
		Me.separater.Size = New System.Drawing.Size(199, 6)
		'
		'mnuExit
		'
		Me.mnuExit.Name = "mnuExit"
		Me.mnuExit.ShortcutKeys = CType((System.Windows.Forms.Keys.Control Or System.Windows.Forms.Keys.X), System.Windows.Forms.Keys)
		Me.mnuExit.Size = New System.Drawing.Size(202, 22)
		Me.mnuExit.Text = "終了(&X)"
		'
		'mnuHelp
		'
		Me.mnuHelp.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.mnuVer})
		Me.mnuHelp.Name = "mnuHelp"
		Me.mnuHelp.Size = New System.Drawing.Size(75, 22)
		Me.mnuHelp.Text = "ヘルプ(&H)"
		'
		'mnuVer
		'
		Me.mnuVer.Name = "mnuVer"
		Me.mnuVer.Size = New System.Drawing.Size(178, 22)
		Me.mnuVer.Text = "バージョン情報(&A)"
		'
		'CBLog
		'
		Me.CBLog.AutoSize = True
		Me.CBLog.BackColor = System.Drawing.SystemColors.Control
		Me.CBLog.Checked = True
		Me.CBLog.CheckState = System.Windows.Forms.CheckState.Checked
		Me.CBLog.Cursor = System.Windows.Forms.Cursors.Default
		Me.CBLog.ForeColor = System.Drawing.SystemColors.ControlText
		Me.CBLog.Location = New System.Drawing.Point(24, 400)
		Me.CBLog.Name = "CBLog"
		Me.CBLog.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.CBLog.Size = New System.Drawing.Size(76, 16)
		Me.CBLog.TabIndex = 14
		Me.CBLog.Text = "ログの出力"
		Me.CBLog.UseVisualStyleBackColor = False
		'
		'CBConvCheck
		'
		Me.CBConvCheck.BackColor = System.Drawing.SystemColors.Control
		Me.CBConvCheck.Checked = True
		Me.CBConvCheck.CheckState = System.Windows.Forms.CheckState.Checked
		Me.CBConvCheck.Cursor = System.Windows.Forms.Cursors.Default
		Me.CBConvCheck.ForeColor = System.Drawing.SystemColors.ControlText
		Me.CBConvCheck.Location = New System.Drawing.Point(24, 173)
		Me.CBConvCheck.Name = "CBConvCheck"
		Me.CBConvCheck.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.CBConvCheck.Size = New System.Drawing.Size(105, 22)
		Me.CBConvCheck.TabIndex = 5
		Me.CBConvCheck.Text = "TIFF/PNG変換"
		Me.CBConvCheck.UseVisualStyleBackColor = False
		'
		'CmdExit
		'
		Me.CmdExit.BackColor = System.Drawing.SystemColors.Control
		Me.CmdExit.Cursor = System.Windows.Forms.Cursors.Default
		Me.CmdExit.ForeColor = System.Drawing.SystemColors.ControlText
		Me.CmdExit.Location = New System.Drawing.Point(232, 480)
		Me.CmdExit.Name = "CmdExit"
		Me.CmdExit.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.CmdExit.Size = New System.Drawing.Size(65, 25)
		Me.CmdExit.TabIndex = 18
		Me.CmdExit.Text = "終了"
		Me.CmdExit.UseVisualStyleBackColor = False
		'
		'CmdRun
		'
		Me.CmdRun.BackColor = System.Drawing.SystemColors.Control
		Me.CmdRun.Cursor = System.Windows.Forms.Cursors.Default
		Me.CmdRun.ForeColor = System.Drawing.SystemColors.ControlText
		Me.CmdRun.Location = New System.Drawing.Point(104, 480)
		Me.CmdRun.Name = "CmdRun"
		Me.CmdRun.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.CmdRun.Size = New System.Drawing.Size(57, 25)
		Me.CmdRun.TabIndex = 17
		Me.CmdRun.Text = "開始"
		Me.CmdRun.UseVisualStyleBackColor = False
		'
		'Frame5
		'
		Me.Frame5.BackColor = System.Drawing.SystemColors.Control
		Me.Frame5.Controls.Add(Me._Cmd_4)
		Me.Frame5.Controls.Add(Me._Option2_0)
		Me.Frame5.Controls.Add(Me._Option2_1)
		Me.Frame5.Controls.Add(Me._OutPutPath_0)
		Me.Frame5.Controls.Add(Me._Cmd_3)
		Me.Frame5.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Frame5.Location = New System.Drawing.Point(16, 312)
		Me.Frame5.Name = "Frame5"
		Me.Frame5.Padding = New System.Windows.Forms.Padding(0)
		Me.Frame5.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Frame5.Size = New System.Drawing.Size(361, 81)
		Me.Frame5.TabIndex = 19
		Me.Frame5.TabStop = False
		Me.Frame5.Text = "出力方法"
		'
		'_Cmd_4
		'
		Me._Cmd_4.BackColor = System.Drawing.SystemColors.Control
		Me._Cmd_4.Cursor = System.Windows.Forms.Cursors.Default
		Me._Cmd_4.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Cmd.SetIndex(Me._Cmd_4, CType(4, Short))
		Me._Cmd_4.Location = New System.Drawing.Point(280, 88)
		Me._Cmd_4.Name = "_Cmd_4"
		Me._Cmd_4.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Cmd_4.Size = New System.Drawing.Size(57, 25)
		Me._Cmd_4.TabIndex = 28
		Me._Cmd_4.Text = "ｻｲｾﾈ(&R)..."
		Me._Cmd_4.UseVisualStyleBackColor = False
		'
		'_Option2_0
		'
		Me._Option2_0.BackColor = System.Drawing.SystemColors.Control
		Me._Option2_0.Checked = True
		Me._Option2_0.Cursor = System.Windows.Forms.Cursors.Default
		Me._Option2_0.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Option2.SetIndex(Me._Option2_0, CType(0, Short))
		Me._Option2_0.Location = New System.Drawing.Point(24, 24)
		Me._Option2_0.Name = "_Option2_0"
		Me._Option2_0.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Option2_0.Size = New System.Drawing.Size(145, 17)
		Me._Option2_0.TabIndex = 10
		Me._Option2_0.TabStop = True
		Me._Option2_0.Text = "ラスターファイル指定"
		Me._Option2_0.UseVisualStyleBackColor = False
		'
		'_Option2_1
		'
		Me._Option2_1.BackColor = System.Drawing.SystemColors.Control
		Me._Option2_1.Cursor = System.Windows.Forms.Cursors.Default
		Me._Option2_1.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Option2.SetIndex(Me._Option2_1, CType(1, Short))
		Me._Option2_1.Location = New System.Drawing.Point(192, 24)
		Me._Option2_1.Name = "_Option2_1"
		Me._Option2_1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Option2_1.Size = New System.Drawing.Size(121, 17)
		Me._Option2_1.TabIndex = 13
		Me._Option2_1.TabStop = True
		Me._Option2_1.Text = "リストファイル指定"
		Me._Option2_1.UseVisualStyleBackColor = False
		'
		'_OutPutPath_0
		'
		Me._OutPutPath_0.AcceptsReturn = True
		Me._OutPutPath_0.BackColor = System.Drawing.SystemColors.Window
		Me._OutPutPath_0.Cursor = System.Windows.Forms.Cursors.IBeam
		Me._OutPutPath_0.ForeColor = System.Drawing.SystemColors.WindowText
		Me.OutPutPath.SetIndex(Me._OutPutPath_0, CType(0, Short))
		Me._OutPutPath_0.Location = New System.Drawing.Point(24, 48)
		Me._OutPutPath_0.MaxLength = 0
		Me._OutPutPath_0.Name = "_OutPutPath_0"
		Me._OutPutPath_0.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._OutPutPath_0.Size = New System.Drawing.Size(217, 19)
		Me._OutPutPath_0.TabIndex = 11
		'
		'_Cmd_3
		'
		Me._Cmd_3.AutoSize = True
		Me._Cmd_3.BackColor = System.Drawing.SystemColors.Control
		Me._Cmd_3.Cursor = System.Windows.Forms.Cursors.Default
		Me._Cmd_3.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Cmd.SetIndex(Me._Cmd_3, CType(3, Short))
		Me._Cmd_3.Location = New System.Drawing.Point(280, 44)
		Me._Cmd_3.Name = "_Cmd_3"
		Me._Cmd_3.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Cmd_3.Size = New System.Drawing.Size(61, 25)
		Me._Cmd_3.TabIndex = 12
		Me._Cmd_3.Text = "参照(&R)..."
		Me._Cmd_3.UseVisualStyleBackColor = False
		'
		'Frame1
		'
		Me.Frame1.BackColor = System.Drawing.SystemColors.Control
		Me.Frame1.Controls.Add(Me._Option1_4)
		Me.Frame1.Controls.Add(Me._Option1_3)
		Me.Frame1.Controls.Add(Me._Option1_2)
		Me.Frame1.Controls.Add(Me.TKZFile)
		Me.Frame1.Controls.Add(Me._Cmd_2)
		Me.Frame1.Controls.Add(Me._Option1_1)
		Me.Frame1.Controls.Add(Me._Option1_0)
		Me.Frame1.Controls.Add(Me.Label2)
		Me.Frame1.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Frame1.Location = New System.Drawing.Point(16, 32)
		Me.Frame1.Name = "Frame1"
		Me.Frame1.Padding = New System.Windows.Forms.Padding(0)
		Me.Frame1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Frame1.Size = New System.Drawing.Size(361, 137)
		Me.Frame1.TabIndex = 21
		Me.Frame1.TabStop = False
		Me.Frame1.Text = "ラスターの種類"
		'
		'_Option1_4
		'
		Me._Option1_4.BackColor = System.Drawing.SystemColors.Control
		Me._Option1_4.Cursor = System.Windows.Forms.Cursors.Default
		Me._Option1_4.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Option1.SetIndex(Me._Option1_4, CType(4, Short))
		Me._Option1_4.Location = New System.Drawing.Point(24, 56)
		Me._Option1_4.Name = "_Option1_4"
		Me._Option1_4.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Option1_4.Size = New System.Drawing.Size(137, 25)
		Me._Option1_4.TabIndex = 29
		Me._Option1_4.TabStop = True
		Me._Option1_4.Text = "座標テキスト読込"
		Me._Option1_4.UseVisualStyleBackColor = False
		'
		'_Option1_3
		'
		Me._Option1_3.BackColor = System.Drawing.SystemColors.Control
		Me._Option1_3.Cursor = System.Windows.Forms.Cursors.Default
		Me._Option1_3.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Option1.SetIndex(Me._Option1_3, CType(3, Short))
		Me._Option1_3.Location = New System.Drawing.Point(159, 32)
		Me._Option1_3.Name = "_Option1_3"
		Me._Option1_3.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Option1_3.Size = New System.Drawing.Size(169, 25)
		Me._Option1_3.TabIndex = 27
		Me._Option1_3.TabStop = True
		Me._Option1_3.Text = "航空写真（overlap有メッシュ）"
		Me._Option1_3.UseVisualStyleBackColor = False
		'
		'_Option1_2
		'
		Me._Option1_2.BackColor = System.Drawing.SystemColors.Control
		Me._Option1_2.Cursor = System.Windows.Forms.Cursors.Default
		Me._Option1_2.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Option1.SetIndex(Me._Option1_2, CType(2, Short))
		Me._Option1_2.Location = New System.Drawing.Point(159, 15)
		Me._Option1_2.Name = "_Option1_2"
		Me._Option1_2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Option1_2.Size = New System.Drawing.Size(199, 18)
		Me._Option1_2.TabIndex = 2
		Me._Option1_2.TabStop = True
		Me._Option1_2.Text = "航空写真（GS国土基本図図郭）"
		Me._Option1_2.UseVisualStyleBackColor = False
		'
		'TKZFile
		'
		Me.TKZFile.AcceptsReturn = True
		Me.TKZFile.BackColor = System.Drawing.SystemColors.Window
		Me.TKZFile.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.TKZFile.ForeColor = System.Drawing.SystemColors.WindowText
		Me.TKZFile.Location = New System.Drawing.Point(24, 107)
		Me.TKZFile.MaxLength = 0
		Me.TKZFile.Name = "TKZFile"
		Me.TKZFile.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.TKZFile.Size = New System.Drawing.Size(217, 19)
		Me.TKZFile.TabIndex = 3
		Me.TKZFile.Text = "\\terra\earth2\make_mesh\TKZ.txt.all"
		'
		'_Cmd_2
		'
		Me._Cmd_2.AutoSize = True
		Me._Cmd_2.BackColor = System.Drawing.SystemColors.Control
		Me._Cmd_2.Cursor = System.Windows.Forms.Cursors.Default
		Me._Cmd_2.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Cmd.SetIndex(Me._Cmd_2, CType(2, Short))
		Me._Cmd_2.Location = New System.Drawing.Point(280, 104)
		Me._Cmd_2.Name = "_Cmd_2"
		Me._Cmd_2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Cmd_2.Size = New System.Drawing.Size(61, 25)
		Me._Cmd_2.TabIndex = 4
		Me._Cmd_2.Text = "参照(&R)..."
		Me._Cmd_2.UseVisualStyleBackColor = False
		'
		'_Option1_1
		'
		Me._Option1_1.BackColor = System.Drawing.SystemColors.Control
		Me._Option1_1.Checked = True
		Me._Option1_1.Cursor = System.Windows.Forms.Cursors.Default
		Me._Option1_1.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Option1.SetIndex(Me._Option1_1, CType(1, Short))
		Me._Option1_1.Location = New System.Drawing.Point(24, 32)
		Me._Option1_1.Name = "_Option1_1"
		Me._Option1_1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Option1_1.Size = New System.Drawing.Size(145, 25)
		Me._Option1_1.TabIndex = 1
		Me._Option1_1.TabStop = True
		Me._Option1_1.Text = "メッシュ矩形ラスター"
		Me._Option1_1.UseVisualStyleBackColor = False
		'
		'_Option1_0
		'
		Me._Option1_0.BackColor = System.Drawing.SystemColors.Control
		Me._Option1_0.Cursor = System.Windows.Forms.Cursors.Default
		Me._Option1_0.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Option1.SetIndex(Me._Option1_0, CType(0, Short))
		Me._Option1_0.Location = New System.Drawing.Point(24, 16)
		Me._Option1_0.Name = "_Option1_0"
		Me._Option1_0.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Option1_0.Size = New System.Drawing.Size(105, 17)
		Me._Option1_0.TabIndex = 0
		Me._Option1_0.TabStop = True
		Me._Option1_0.Text = "都計図ラスター"
		Me._Option1_0.UseVisualStyleBackColor = False
		'
		'Label2
		'
		Me.Label2.BackColor = System.Drawing.SystemColors.Control
		Me.Label2.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label2.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Label2.Location = New System.Drawing.Point(24, 88)
		Me.Label2.Name = "Label2"
		Me.Label2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label2.Size = New System.Drawing.Size(217, 17)
		Me.Label2.TabIndex = 24
		Me.Label2.Text = "都計図四隅定義ファイル"
		'
		'Frame2
		'
		Me.Frame2.BackColor = System.Drawing.SystemColors.Control
		Me.Frame2.Controls.Add(Me.TIFFPath)
		Me.Frame2.Controls.Add(Me._Cmd_0)
		Me.Frame2.Controls.Add(Me.Label1)
		Me.Frame2.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Frame2.Location = New System.Drawing.Point(16, 176)
		Me.Frame2.Name = "Frame2"
		Me.Frame2.Padding = New System.Windows.Forms.Padding(0)
		Me.Frame2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Frame2.Size = New System.Drawing.Size(361, 73)
		Me.Frame2.TabIndex = 20
		Me.Frame2.TabStop = False
		'
		'TIFFPath
		'
		Me.TIFFPath.AcceptsReturn = True
		Me.TIFFPath.BackColor = System.Drawing.SystemColors.Window
		Me.TIFFPath.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.TIFFPath.ForeColor = System.Drawing.SystemColors.WindowText
		Me.TIFFPath.Location = New System.Drawing.Point(24, 44)
		Me.TIFFPath.MaxLength = 0
		Me.TIFFPath.Name = "TIFFPath"
		Me.TIFFPath.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.TIFFPath.Size = New System.Drawing.Size(217, 19)
		Me.TIFFPath.TabIndex = 6
		Me.TIFFPath.Text = "\\terra\earth2\raster_gou"
		'
		'_Cmd_0
		'
		Me._Cmd_0.AutoSize = True
		Me._Cmd_0.BackColor = System.Drawing.SystemColors.Control
		Me._Cmd_0.Cursor = System.Windows.Forms.Cursors.Default
		Me._Cmd_0.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Cmd.SetIndex(Me._Cmd_0, CType(0, Short))
		Me._Cmd_0.Location = New System.Drawing.Point(280, 40)
		Me._Cmd_0.Name = "_Cmd_0"
		Me._Cmd_0.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Cmd_0.Size = New System.Drawing.Size(61, 25)
		Me._Cmd_0.TabIndex = 7
		Me._Cmd_0.Text = "参照(&R)..."
		Me._Cmd_0.UseVisualStyleBackColor = False
		'
		'Label1
		'
		Me.Label1.BackColor = System.Drawing.SystemColors.Control
		Me.Label1.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label1.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Label1.Location = New System.Drawing.Point(24, 24)
		Me.Label1.Name = "Label1"
		Me.Label1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label1.Size = New System.Drawing.Size(217, 17)
		Me.Label1.TabIndex = 23
		Me.Label1.Text = "TIFFイメージディレクトリ"
		'
		'Frame3
		'
		Me.Frame3.BackColor = System.Drawing.SystemColors.Control
		Me.Frame3.Controls.Add(Me.PNGPath)
		Me.Frame3.Controls.Add(Me._Cmd_1)
		Me.Frame3.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Frame3.Location = New System.Drawing.Point(16, 256)
		Me.Frame3.Name = "Frame3"
		Me.Frame3.Padding = New System.Windows.Forms.Padding(0)
		Me.Frame3.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Frame3.Size = New System.Drawing.Size(361, 49)
		Me.Frame3.TabIndex = 22
		Me.Frame3.TabStop = False
		Me.Frame3.Text = "PNG/JPEGイメージディレクトリ"
		'
		'PNGPath
		'
		Me.PNGPath.AcceptsReturn = True
		Me.PNGPath.BackColor = System.Drawing.SystemColors.Window
		Me.PNGPath.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.PNGPath.ForeColor = System.Drawing.SystemColors.WindowText
		Me.PNGPath.Location = New System.Drawing.Point(24, 19)
		Me.PNGPath.MaxLength = 0
		Me.PNGPath.Name = "PNGPath"
		Me.PNGPath.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.PNGPath.Size = New System.Drawing.Size(217, 19)
		Me.PNGPath.TabIndex = 8
		Me.PNGPath.Text = "\\sindy04\data\raster\都計図メッシュ"
		'
		'_Cmd_1
		'
		Me._Cmd_1.AutoSize = True
		Me._Cmd_1.BackColor = System.Drawing.SystemColors.Control
		Me._Cmd_1.Cursor = System.Windows.Forms.Cursors.Default
		Me._Cmd_1.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Cmd.SetIndex(Me._Cmd_1, CType(1, Short))
		Me._Cmd_1.Location = New System.Drawing.Point(280, 16)
		Me._Cmd_1.Name = "_Cmd_1"
		Me._Cmd_1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Cmd_1.Size = New System.Drawing.Size(61, 25)
		Me._Cmd_1.TabIndex = 9
		Me._Cmd_1.Text = "参照(&R)..."
		Me._Cmd_1.UseVisualStyleBackColor = False
		'
		'Frame4
		'
		Me.Frame4.BackColor = System.Drawing.SystemColors.Control
		Me.Frame4.Controls.Add(Me._Cmd_5)
		Me.Frame4.Controls.Add(Me.LogFile)
		Me.Frame4.Controls.Add(Me.Label3)
		Me.Frame4.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Frame4.Location = New System.Drawing.Point(16, 400)
		Me.Frame4.Name = "Frame4"
		Me.Frame4.Padding = New System.Windows.Forms.Padding(0)
		Me.Frame4.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Frame4.Size = New System.Drawing.Size(361, 73)
		Me.Frame4.TabIndex = 25
		Me.Frame4.TabStop = False
		'
		'_Cmd_5
		'
		Me._Cmd_5.AutoSize = True
		Me._Cmd_5.BackColor = System.Drawing.SystemColors.Control
		Me._Cmd_5.Cursor = System.Windows.Forms.Cursors.Default
		Me._Cmd_5.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Cmd.SetIndex(Me._Cmd_5, CType(5, Short))
		Me._Cmd_5.Location = New System.Drawing.Point(280, 40)
		Me._Cmd_5.Name = "_Cmd_5"
		Me._Cmd_5.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._Cmd_5.Size = New System.Drawing.Size(61, 25)
		Me._Cmd_5.TabIndex = 16
		Me._Cmd_5.Text = "参照(&R)..."
		Me._Cmd_5.UseVisualStyleBackColor = False
		'
		'LogFile
		'
		Me.LogFile.AcceptsReturn = True
		Me.LogFile.BackColor = System.Drawing.SystemColors.Window
		Me.LogFile.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.LogFile.ForeColor = System.Drawing.SystemColors.WindowText
		Me.LogFile.Location = New System.Drawing.Point(24, 44)
		Me.LogFile.MaxLength = 0
		Me.LogFile.Name = "LogFile"
		Me.LogFile.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.LogFile.Size = New System.Drawing.Size(217, 19)
		Me.LogFile.TabIndex = 15
		Me.LogFile.Text = "d:\tmp\image\auxcreate.log"
		'
		'Label3
		'
		Me.Label3.BackColor = System.Drawing.SystemColors.Control
		Me.Label3.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label3.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Label3.Location = New System.Drawing.Point(24, 24)
		Me.Label3.Name = "Label3"
		Me.Label3.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label3.Size = New System.Drawing.Size(217, 17)
		Me.Label3.TabIndex = 26
		Me.Label3.Text = "ログ出力ファイル名"
		'
		'Cmd
		'
		'
		'Option1
		'
		'
		'Option2
		'
		'
		'Form1
		'
		Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.BackColor = System.Drawing.SystemColors.Control
		Me.ClientSize = New System.Drawing.Size(389, 513)
		Me.Controls.Add(Me.CBLog)
		Me.Controls.Add(Me.CBConvCheck)
		Me.Controls.Add(Me.CmdExit)
		Me.Controls.Add(Me.CmdRun)
		Me.Controls.Add(Me.Frame5)
		Me.Controls.Add(Me.Frame1)
		Me.Controls.Add(Me.Frame2)
		Me.Controls.Add(Me.Frame3)
		Me.Controls.Add(Me.Frame4)
		Me.Controls.Add(Me.MainMenu1)
		Me.Cursor = System.Windows.Forms.Cursors.Default
		Me.Location = New System.Drawing.Point(130, 104)
		Me.Name = "Form1"
		Me.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.StartPosition = System.Windows.Forms.FormStartPosition.Manual
		Me.Text = "ジオリファレンス作成ツール"
		Me.MainMenu1.ResumeLayout(False)
		Me.MainMenu1.PerformLayout()
		Me.Frame5.ResumeLayout(False)
		Me.Frame5.PerformLayout()
		Me.Frame1.ResumeLayout(False)
		Me.Frame1.PerformLayout()
		Me.Frame2.ResumeLayout(False)
		Me.Frame2.PerformLayout()
		Me.Frame3.ResumeLayout(False)
		Me.Frame3.PerformLayout()
		Me.Frame4.ResumeLayout(False)
		Me.Frame4.PerformLayout()
		CType(Me.Cmd, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.Option1, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.Option2, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.OutPutPath, System.ComponentModel.ISupportInitialize).EndInit()
		Me.ResumeLayout(False)
		Me.PerformLayout()

	End Sub
#End Region 
End Class