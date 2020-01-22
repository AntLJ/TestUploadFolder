Option Strict Off
Option Explicit On
Module CreateAUX
	' tiff�C���[�W��png�ɕϊ�
	Public Function ConvertImage(ByRef cSrcPath As String, ByRef cTargetPath As String) As Boolean
		On Error GoTo ErrorHandler
		
		ConvertImage = True
		'UPGRADE_WARNING: Dir �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
		If Dir(GetPath(cTargetPath), FileAttribute.Directory) = "" Then
			MkDir(GetPath(cTargetPath))
		End If
		'UPGRADE_WARNING: Dir �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
		If Dir(cTargetPath) <> vbNullString Then
			Kill(cTargetPath)
		End If
		If ConvertImage Then
			'CmdExec(My.Application.Info.DirectoryPath & "\i_view32.exe " & cSrcPath & " /convert=" & cTargetPath)
			System.Diagnostics.Process.Start(My.Application.Info.DirectoryPath & "\i_view32.exe", cSrcPath & " /convert=" & cTargetPath)
		End If
		
		Exit Function
		
ErrorHandler: 
		Const ErrNotWritable As Short = 70
		If Err.Number = ErrNotWritable Then
			ConvertImage = False
			Resume Next
		Else
			MsgBox("�\�����Ȃ��G���[ #" & VB6.Format(Err.Number) & " ���������܂����B" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "�G���[")
			Stop
		End If
	End Function
	' ������aux�t�@�C�����폜
	Public Function RefClean(ByRef cTargetPath As Object) As Boolean
		On Error GoTo CheckError
		
		RefClean = True
		' v9.1 �ȑO�̏ꍇ
		'UPGRADE_WARNING: �I�u�W�F�N�g cTargetPath �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
		'UPGRADE_WARNING: Dir �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
		If Dir(Left(cTargetPath, InStr(cTargetPath, ".") - 1) & ".aux") <> vbNullString Then
			'UPGRADE_WARNING: �I�u�W�F�N�g cTargetPath �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
			Kill(Left(cTargetPath, InStr(cTargetPath, ".") - 1) & ".aux")
		End If
		' v9.2�ȍ~�̏ꍇ
		If RefClean Then
			'UPGRADE_WARNING: �I�u�W�F�N�g cTargetPath �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
			'UPGRADE_WARNING: Dir �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
			If Dir(cTargetPath & ".aux.xml") <> vbNullString Then
				'UPGRADE_WARNING: �I�u�W�F�N�g cTargetPath �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
				Kill(cTargetPath & ".aux.xml")
			End If
		End If
		' ���ԍ�Ɨp�̃��[���h�t�@�C��
		If RefClean Then
			'UPGRADE_WARNING: �I�u�W�F�N�g cTargetPath �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
			'UPGRADE_WARNING: Dir �ɐV�������삪�w�肳��Ă��܂��B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' ���N���b�N���Ă��������B
			If Dir(Left(cTargetPath, InStr(cTargetPath, ".") - 1) & ".pgw") <> vbNullString Then
				'UPGRADE_WARNING: �I�u�W�F�N�g cTargetPath �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
				Kill(Left(cTargetPath, InStr(cTargetPath, ".") - 1) & ".pgw")
			End If
		End If
		'  ���Ԃ񂢂�Ȃ�
		'    If RefClean Then
		'       If Dir$(Left(cTargetPath, InStr(cTargetPath, ".") - 1) & ".pgwx") <> vbNullString Then
		'            Kill Left(cTargetPath, InStr(cTargetPath, ".") - 1) & ".pgwx"
		'        End If
		'    End If
		Exit Function
CheckError: 
		Debug.Print(Err.Number)
		Const ErrNotWritable As Short = 70
		If Err.Number = ErrNotWritable Then
			RefClean = False
			Resume Next
		Else
			MsgBox("�\�����Ȃ��G���[ #" & VB6.Format(Err.Number) & " ���������܂����B" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "�G���[")
			Stop
		End If
	End Function

	''' <summary>
	''' ���[���h�t�@�C���p�����[�^������ʒu���v�Z
	''' </summary>
	''' <param name="IWidth">�摜��X�����̃s�N�Z����</param>
	''' <param name="IHeight">�摜��Y�����̃s�N�Z����</param>
	''' <param name="cWPrm">���[���h�t�@�C���p�����[�^</param>
	''' <param name="aPCS">AUX�t�@�C���ɏo�͂�����W�n�i���ʒ��p���W�n(���{���n�n)�j��EPSG�R�[�h</param>
	''' <param name="cToPnts">���[���h�t�@�C���p�����[�^����Z�o���������W�̃|�C���g</param>
	''' <remarks></remarks>
	Public Sub WorldToPos(ByRef IWidth As Integer, ByRef IHeight As Integer, ByRef cWPrm() As Double, ByRef aPCS As Integer, ByRef cToPnts As ESRI.ArcGIS.Geometry.IPointCollection)
		Dim ULpntTo As ESRI.ArcGIS.Geometry.IPoint : ULpntTo = New ESRI.ArcGIS.Geometry.Point
		Dim LLpntTo As ESRI.ArcGIS.Geometry.IPoint : LLpntTo = New ESRI.ArcGIS.Geometry.Point
		Dim LRpntTo As ESRI.ArcGIS.Geometry.IPoint : LRpntTo = New ESRI.ArcGIS.Geometry.Point
		Dim URpntTo As ESRI.ArcGIS.Geometry.IPoint : URpntTo = New ESRI.ArcGIS.Geometry.Point

		On Error GoTo ErrorHandler

		If aPCS = 0 Then
			ULpntTo.X = cWPrm(4) - cWPrm(0) / 2
			LLpntTo.X = ULpntTo.X
			ULpntTo.Y = cWPrm(5) + cWPrm(3) / 2
			URpntTo.Y = ULpntTo.Y
			URpntTo.X = cWPrm(0) * IWidth + ULpntTo.X
			LRpntTo.X = URpntTo.X
			LLpntTo.Y = cWPrm(3) * IHeight + ULpntTo.Y
			LRpntTo.Y = LLpntTo.Y
		Else
			Dim after_x As Double = 0
			Dim after_y As Double = 0

			Dim bRet As Boolean = False
			Dim cnv As New crdcnvLibLib.CrdConvert
			' �n��p�����[�^�t�@�C����ǂݍ���
			' TODO:�ǂݍ��݉񐔂��ŏ��ɂ���
			cnv.JGD2000_RP_Init(SetParamPath(), bRet)

			' cWPrm(0)�F1�s�N�Z����X�����̒���
			' cWPrm(1)�F�s�̉�]�p�����[�^
			' cWPrm(2)�F��̉�]�p�����[�^
			' cWPrm(3)�F1�s�N�Z����Y�����̒���
			' cWPrm(4)�F����s�N�Z���̒��S�ʒu��X���W(���ʒ��p���W�n(���E���n�n))
			' cWPrm(5)�F����s�N�Z���̒��S�ʒu��Y���W(���ʒ��p���W�n(���E���n�n))

			cnvXY19(cnv, aPCS, cWPrm(4) - cWPrm(0) / 2, cWPrm(5) + cWPrm(3) / 2, after_x, after_y)
			ULpntTo.X = after_x
			ULpntTo.Y = after_y

			cnvXY19(cnv, aPCS, cWPrm(4) - cWPrm(0) / 2, cWPrm(3) * IHeight + cWPrm(5) + cWPrm(3) / 2, after_x, after_y)
			LLpntTo.X = after_x
			LLpntTo.Y = after_y

			cnvXY19(cnv, aPCS, cWPrm(0) * IWidth + cWPrm(4) - cWPrm(0) / 2, cWPrm(5) + cWPrm(3) / 2, after_x, after_y)
			URpntTo.X = after_x
			URpntTo.Y = after_y

			cnvXY19(cnv, aPCS, cWPrm(0) * IWidth + cWPrm(4) - cWPrm(0) / 2, cWPrm(3) * IHeight + cWPrm(5) + cWPrm(3) / 2, after_x, after_y)
			LRpntTo.X = after_x
			LRpntTo.Y = after_y
		End If

		cToPnts.AddPoint(ULpntTo)
		cToPnts.AddPoint(LLpntTo)
		cToPnts.AddPoint(LRpntTo)
		cToPnts.AddPoint(URpntTo)


		'   clean
		'UPGRADE_NOTE: �I�u�W�F�N�g ULpntTo ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		ULpntTo = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g LLpntTo ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		LLpntTo = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g LRpntTo ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		LRpntTo = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g URpntTo ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		URpntTo = Nothing

		Exit Sub

ErrorHandler:
	End Sub

	''' <summary>
	''' ���ʒ��p���W�n�i���E���n�n�j->���ʒ��p���W�n�i���{���n�n�j�ւ̍��W�ϊ�
	''' </summary>
	''' <param name="cnv">���W�ϊ��p��crdcnvLib</param>
	''' <param name="aPCS">AUX�t�@�C���ɏo�͂�����W�n�i���ʒ��p���W�n(���{���n�n)�j��EPSG�R�[�h</param>
	''' <param name="before_x">�ϊ��O��X���W�i���ʒ��p���W�n�i���E���n�n�j�j</param>
	''' <param name="before_y">�ϊ��O��Y���W�i���ʒ��p���W�n�i���E���n�n�j�j</param>
	''' <param name="after_y">�ϊ����X���W�i���ʒ��p���W�n�i���{���n�n�j�j</param>
	''' <param name="after_x">�ϊ����Y���W�i���ʒ��p���W�n�i���{���n�n�j�j</param>
	''' <remarks>
	''' ���ۂ̕ϊ��t���[�F���ʒ��p���W�n�i���E���n�n�j->�o�ܓx�i���E���n�n�j--[�n��p�����[�^]-->�o�ܓx�i���{���n�n�j->���ʒ��p���W�n�i���{���n�n�j
	''' </remarks>
	Public Sub cnvXY19(ByVal cnv As crdcnvLibLib.CrdConvert, ByRef aPCS As Integer, ByVal before_x As Double, ByVal before_y As Double, ByRef after_y As Double, ByRef after_x As Double)
		' TODO:after_x��after_y���t�ɂ��Ă����Ȃ��Ƃ��܂������Ȃ��icrd_cnv��LLtoXY19()�̂����H�j

		' ���ʒ��p���W�n(���E���n�n) -> �o�ܓx(���E���n�n)
		Dim srcSpRef As ESRI.ArcGIS.Geometry.ISpatialReference
		Dim dstSpRef As ESRI.ArcGIS.Geometry.ISpatialReference
		Dim spRefFct As ESRI.ArcGIS.Geometry.ISpatialReferenceFactory2 : spRefFct = New ESRI.ArcGIS.Geometry.SpatialReferenceEnvironment
		srcSpRef = spRefFct.CreateSpatialReference(SetBeforePCSType(aPCS))
		Dim point As ESRI.ArcGIS.Geometry.IPoint : point = New ESRI.ArcGIS.Geometry.Point
		point.SpatialReference = srcSpRef
		point.PutCoords(before_x, before_y)

		dstSpRef = spRefFct.CreateSpatialReference(EPSG_WGS84)
		Dim before_lon As Double = 0
		Dim before_lat As Double = 0
		point.Project(dstSpRef)
		point.QueryCoords(before_lon, before_lat)

		' �o�ܓx(���E���n�n) --[�n��p�����[�^]--> �o�ܓx(���{���n�n)
		Dim after_lon As Double = 0
		Dim after_lat As Double = 0
		Dim bRet As Boolean = False
		cnv.JGD2000toTOKYO_RP(before_lon, before_lat, after_lon, after_lat, True, bRet)

		' �o�ܓx(���{���n�n) -> ���ʒ��p���W�n(���{���n�n)
		cnv.LLtoXY19(SetAxis(aPCS), after_lon, after_lat, after_x, after_y)
		point = Nothing

	End Sub

	''' <summary>
	''' �n��p�����[�^�t�@�C���̃p�X��ݒ肷��
	''' </summary>
	''' <returns>�n��p�����[�^�t�@�C���̃p�X</returns>
	''' <remarks>���W�X�g���L�[��������Ȃ��ꍇ��󕶎����w�肳��Ă���ꍇ�͊���̃t�@�C���p�X��Ԃ�</remarks>
	Public Function SetParamPath() As String
		Dim filepath As String
		Dim regkey As Microsoft.Win32.RegistryKey = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(TOOL_REG)
		If regkey IsNot Nothing Then
			filepath = DirectCast(regkey.GetValue(PARAM_REG, DEFAULT_PARAM_PATH), String)
		End If

		' ������Ȃ��ꍇ�̓J�����g�f�B���N�g���̃p�����[�^�t�@�C����ݒ�
		If String.IsNullOrEmpty(filepath) Then
			filepath = DEFAULT_PARAM_PATH
		End If
		SetParamPath = filepath
	End Function

	' AUX�t�@�C���̍쐬
	Public Sub ChangeRasterProperty(ByRef cSw As Boolean, ByRef pWSF As ESRI.ArcGIS.Geodatabase.IWorkspaceFactory, ByRef cTargetPath As String, ByRef PCS As Integer, ByRef ToPnts As ESRI.ArcGIS.Geometry.IPointCollection)
		' sPath: path for input and output
		' sInName: name of input raster
		' PCS: code for Projected Coordinate System, like 54008 is Sinusodial
		On Error GoTo ErrorHandler

		' Create RasterWorkSpaceFactory
		'Dim pWSF As IWorkspaceFactory
		'Set pWSF = New RasterWorkspaceFactory

		' Get RasterWorkspace
		Dim pRWS As ESRI.ArcGIS.DataSourcesRaster.IRasterWorkspace
		' Dim pRDS As IRasterDataset
		'Invalid_string_refer_to_original_code
		Dim pRDS As ESRI.ArcGIS.Geodatabase.IRasterDataset2

		If pWSF.IsWorkspace(GetPath(cTargetPath)) Then
			pRWS = pWSF.OpenFromFile(GetPath(cTargetPath), 0)
			System.Windows.Forms.Application.DoEvents()
			pRDS = pRWS.OpenRasterDataset(GetFilename(cTargetPath))	'
			System.Windows.Forms.Application.DoEvents()
		Else
			ErrMsg("�G���[", "���[�N�X�y�[�X�̍쐬�Ɏ��s���܂����B")
			Exit Sub
		End If

		' ���X�^�[�f�[�^�̃I�[�v��
		If pRDS Is Nothing Then
			ErrMsg("�G���[", "���X�^�f�[�^���擾�ł��܂���ł����B")
			Exit Sub
		End If

		' ���X�^�[�f�[�^�̎擾
		Dim pRas As ESRI.ArcGIS.Geodatabase.IRaster
		' Set pRas = pRDS.CreateDefaultRaster
		' 9.1 Bug ���
		pRas = pRDS.CreateFullRaster

		' 04/09/10 �ǉ�
		Dim pBand As ESRI.ArcGIS.DataSourcesRaster.IRasterBandCollection
		pBand = pRas

		Dim pRasBand As ESRI.ArcGIS.DataSourcesRaster.IRasterBand
		pRasBand = pBand.Item(0)
		' �����܂�

		Dim pRasGeo As ESRI.ArcGIS.DataSourcesRaster.IRasterGeometryProc
		pRasGeo = New ESRI.ArcGIS.DataSourcesRaster.RasterGeometryProc
		Dim pRasProp As ESRI.ArcGIS.DataSourcesRaster.IRasterProps
		pRasProp = pRasBand
		' Set pRasProp = pRas
		Dim pExt As ESRI.ArcGIS.Geometry.IEnvelope
		pExt = pRasProp.Extent

		'4���s�N�Z�����S���W�i�s�N�Z�����W�j
		Dim ULpntFrom As ESRI.ArcGIS.Geometry.IPoint : ULpntFrom = New ESRI.ArcGIS.Geometry.Point
		Dim LLpntFrom As ESRI.ArcGIS.Geometry.IPoint : LLpntFrom = New ESRI.ArcGIS.Geometry.Point
		Dim LRpntFrom As ESRI.ArcGIS.Geometry.IPoint : LRpntFrom = New ESRI.ArcGIS.Geometry.Point
		Dim URpntFrom As ESRI.ArcGIS.Geometry.IPoint : URpntFrom = New ESRI.ArcGIS.Geometry.Point

		' �q��ʐ^�̏ꍇ
		Dim aWPrm() As Double
		If cSw Then
			Call loadWorldPrm(cTargetPath, ".jgw.org", aWPrm)
			Call WorldToPos((pRasProp.Width), (pRasProp.Height), aWPrm, PCS, ToPnts)
			If PCS = 0 Then
				Call OutRangePos(Val(GetPrefix(GetFilename(cTargetPath))), ToPnts)
			End If
			Erase aWPrm
		End If

		'    ULpntFrom.X = pExt.UpperLeft.X + 0.5:  ULpntFrom.Y = pExt.UpperLeft.Y - 0.5
		'    LLpntFrom.X = pExt.LowerLeft.X + 0.5:  LLpntFrom.Y = pExt.LowerLeft.Y + 0.5
		'    LRpntFrom.X = pExt.LowerRight.X - 0.5: LRpntFrom.Y = pExt.LowerRight.Y + 0.5
		'    URpntFrom.X = pExt.UpperRight.X - 0.5: URpntFrom.Y = pExt.UpperRight.Y - 0.5

		ULpntFrom.X = pExt.UpperLeft.X : ULpntFrom.Y = pExt.UpperLeft.Y
		LLpntFrom.X = pExt.LowerLeft.X : LLpntFrom.Y = pExt.LowerLeft.Y
		LRpntFrom.X = pExt.LowerRight.X : LRpntFrom.Y = pExt.LowerRight.Y
		URpntFrom.X = pExt.UpperRight.X : URpntFrom.Y = pExt.UpperRight.Y

		' �����N�x�N�g���̍쐬�i�uFromPnts�v���s�N�Z�����W�|�C���g�A�uToPnts�v���n�}���W�j
		Dim mpts1 As ESRI.ArcGIS.Geometry.Multipoint
		mpts1 = New ESRI.ArcGIS.Geometry.Multipoint
		'Dim mpts2 As Multipoint
		'Set mpts2 = New Multipoint

		Dim FromPnts As ESRI.ArcGIS.Geometry.IPointCollection
		FromPnts = mpts1
		'Dim ToPnts As IPointCollection
		'Set ToPnts = mpts2

		FromPnts.AddPoint(ULpntFrom)
		FromPnts.AddPoint(LLpntFrom)
		FromPnts.AddPoint(LRpntFrom)
		FromPnts.AddPoint(URpntFrom)

		'ToPnts.AddPoint ULpntTo
		'ToPnts.AddPoint LLpntTo
		'ToPnts.AddPoint LRpntTo
		'ToPnts.AddPoint URpntTo

		pRasGeo.Warp(FromPnts, ToPnts, ESRI.ArcGIS.DataSourcesRaster.esriGeoTransTypeEnum.esriGeoTransPolyOrder1, pRas)
		pRasGeo.Register(pRas)

		' Define Spatial Reference
		Dim pSRS As ESRI.ArcGIS.Geometry.ISpatialReferenceFactory2
		pSRS = New ESRI.ArcGIS.Geometry.SpatialReferenceEnvironment

		Dim pPrj As ESRI.ArcGIS.Geometry.ISpatialReference
		'Set pPrj = New ProjectedCoordinateSystem

		If PCS > 0 Then
			'UPGRADE_WARNING: �I�u�W�F�N�g pSRS.CreateProjectedCoordinateSystem �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
			pPrj = pSRS.CreateProjectedCoordinateSystem(PCS)
			'    ElseIf PCS < 0 Then
			''        Set pPrj = pSRS.CreateGeographicCoordinateSystem(esriSRGeoCS_WGS1984)
			'        Set pPrj = pSRS.CreateGeographicCoordinateSystem(esriSRGeoCS_IERSTerrestrialReferenceFrame1994)
			'    ' PCS = 0
		Else
			'UPGRADE_WARNING: �I�u�W�F�N�g pSRS.CreateGeographicCoordinateSystem �̊���v���p�e�B�������ł��܂���ł����B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' ���N���b�N���Ă��������B
			pPrj = pSRS.CreateGeographicCoordinateSystem(ESRI.ArcGIS.Geometry.esriSRGeoCSType.esriSRGeoCS_Tokyo)
		End If

		Dim pGeoEdit As ESRI.ArcGIS.Geodatabase.IGeoDatasetSchemaEdit
		pGeoEdit = pRDS

		If pGeoEdit Is Nothing Then
			MsgBox("pGeoEdit���擾�ł��Ă��܂���B")
		Else
			pGeoEdit.AlterSpatialReference(pPrj)
		End If

		'Clean up
		'Set pWSF = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g pRWS ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		pRWS = Nothing

		'UPGRADE_NOTE: �I�u�W�F�N�g pRDS ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		pRDS = Nothing

		'UPGRADE_NOTE: �I�u�W�F�N�g pRas ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		pRas = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g pRasGeo ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		pRasGeo = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g pRasProp ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		pRasProp = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g pExt ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		pExt = Nothing

		'UPGRADE_NOTE: �I�u�W�F�N�g pSRS ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		pSRS = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g pPrj ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		pPrj = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g pGeoEdit ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		pGeoEdit = Nothing

		'UPGRADE_NOTE: �I�u�W�F�N�g mpts1 ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		mpts1 = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g FromPnts ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		FromPnts = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g ULpntFrom ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		ULpntFrom = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g LLpntFrom ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		LLpntFrom = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g LRpntFrom ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		LRpntFrom = Nothing
		'UPGRADE_NOTE: �I�u�W�F�N�g URpntFrom ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		URpntFrom = Nothing

		Exit Sub

ErrorHandler:
		'UPGRADE_NOTE: �I�u�W�F�N�g pRDS ���K�x�[�W �R���N�g����܂ł��̃I�u�W�F�N�g��j�����邱�Ƃ͂ł��܂���B �ڍׂɂ��ẮA'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' ���N���b�N���Ă��������B
		pRDS = Nothing
		ErrMsg("error", GetFilename(cTargetPath))
	End Sub

	''' <summary>
	''' �n�ԍ����畽�ʒ��p���W�n(���{���n�n)��EPSG�R�[�h���擾����
	''' </summary>
	''' <param name="cZone">���ʒ��p���W�n�̌n�ԍ�</param>
	''' <returns>�Ή�����n�ԍ��̕��ʒ��p���W�n�i���{���n�n�j��EPSG�R�[�h</returns>
	''' <remarks></remarks>
	Public Function SetPCSType(ByRef cZone As Short) As Integer
		Dim aPCS As Integer
		Select Case cZone
			Case 1
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan1
			Case 2
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan2
			Case 3
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan3
			Case 4
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan4
			Case 5
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan5
			Case 6
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan6
			Case 7
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan7
			Case 8
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan8
			Case 9
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan9
			Case 10
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan10
			Case 11
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan11
			Case 12
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan12
			Case 13
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan13
			Case 14
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan14
			Case 15
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan15
			Case 16
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan16
			Case 17
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan17
			Case 18
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan18
			Case 19
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan19
			Case Else
				aPCS = 0
		End Select
		SetPCSType = aPCS
	End Function

	''' <summary>
	''' ���ʒ��p���W�n(���{���n�n)��EPSG�R�[�h���畽�ʒ��p���W�n(���E���n�n)��EPSG�R�[�h���擾����
	''' </summary>
	''' <param name="afterPCS">���ʒ��p���W�n�i���{���n�n�j��EPSG�R�[�h</param>
	''' <returns>���͂ɑΉ����镽�ʒ��p���W�n�i���E���n�n�j��EPSG�R�[�h</returns>
	''' <remarks></remarks>
	Public Function SetBeforePCSType(ByRef afterPCS As Integer) As Integer
		Dim aPCS As Integer
		Select Case afterPCS
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan1
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_1
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan2
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_2
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan3
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_3
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan4
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_4
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan5
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_5
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan6
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_6
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan7
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_7
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan8
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_8
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan9
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_9
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan10
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_10
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan11
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_11
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan12
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_12
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan13
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_13
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan14
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_14
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan15
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_15
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan16
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_16
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan17
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_17
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan18
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_18
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan19
				aPCS = ESRI.ArcGIS.Geometry.esriSRProjCS4Type.esriSRProjCS_JGD2000_Japan_Zone_19
			Case Else
				aPCS = 0
		End Select
		SetBeforePCSType = aPCS
	End Function

	''' <summary>
	''' ���ʒ��p���W�n(���{���n�n)��EPSG�R�[�h����n�ԍ����擾����
	''' </summary>
	''' <param name="aPCS">���ʒ��p���W�n�i���{���n�n�j��EPSG�R�[�h</param>
	''' <returns>���͂ɑΉ�����n�ԍ�</returns>
	''' <remarks></remarks>
	Public Function SetAxis(ByRef aPCS As Integer) As Integer
		Dim axis As Integer
		Select Case aPCS
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan1
				axis = 1
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan2
				axis = 2
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan3
				axis = 3
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan4
				axis = 4
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan5
				axis = 5
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan6
				axis = 6
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan7
				axis = 7
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan8
				axis = 8
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan9
				axis = 9
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan10
				axis = 10
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan11
				axis = 11
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan12
				axis = 12
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan13
				axis = 13
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan14
				axis = 14
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan15
				axis = 15
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan16
				axis = 16
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan17
				axis = 17
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan18
				axis = 18
			Case ESRI.ArcGIS.Geometry.esriSRProjCSType.esriSRProjCS_TokyoJapan19
				axis = 19
			Case Else
				axis = 0
		End Select
		SetAxis = axis
	End Function
End Module