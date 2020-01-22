Option Strict Off
Option Explicit On
Module CreateAUX
	' tiffイメージをpngに変換
	Public Function ConvertImage(ByRef cSrcPath As String, ByRef cTargetPath As String) As Boolean
		On Error GoTo ErrorHandler
		
		ConvertImage = True
		'UPGRADE_WARNING: Dir に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
		If Dir(GetPath(cTargetPath), FileAttribute.Directory) = "" Then
			MkDir(GetPath(cTargetPath))
		End If
		'UPGRADE_WARNING: Dir に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
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
			MsgBox("予期しないエラー #" & VB6.Format(Err.Number) & " が発生しました。" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "エラー")
			Stop
		End If
	End Function
	' 既存のauxファイルを削除
	Public Function RefClean(ByRef cTargetPath As Object) As Boolean
		On Error GoTo CheckError
		
		RefClean = True
		' v9.1 以前の場合
		'UPGRADE_WARNING: オブジェクト cTargetPath の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
		'UPGRADE_WARNING: Dir に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
		If Dir(Left(cTargetPath, InStr(cTargetPath, ".") - 1) & ".aux") <> vbNullString Then
			'UPGRADE_WARNING: オブジェクト cTargetPath の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
			Kill(Left(cTargetPath, InStr(cTargetPath, ".") - 1) & ".aux")
		End If
		' v9.2以降の場合
		If RefClean Then
			'UPGRADE_WARNING: オブジェクト cTargetPath の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
			'UPGRADE_WARNING: Dir に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
			If Dir(cTargetPath & ".aux.xml") <> vbNullString Then
				'UPGRADE_WARNING: オブジェクト cTargetPath の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
				Kill(cTargetPath & ".aux.xml")
			End If
		End If
		' 中間作業用のワールドファイル
		If RefClean Then
			'UPGRADE_WARNING: オブジェクト cTargetPath の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
			'UPGRADE_WARNING: Dir に新しい動作が指定されています。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"' をクリックしてください。
			If Dir(Left(cTargetPath, InStr(cTargetPath, ".") - 1) & ".pgw") <> vbNullString Then
				'UPGRADE_WARNING: オブジェクト cTargetPath の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
				Kill(Left(cTargetPath, InStr(cTargetPath, ".") - 1) & ".pgw")
			End If
		End If
		'  たぶんいらない
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
			MsgBox("予期しないエラー #" & VB6.Format(Err.Number) & " が発生しました。" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "エラー")
			Stop
		End If
	End Function

	''' <summary>
	''' ワールドファイルパラメータから実位置を計算
	''' </summary>
	''' <param name="IWidth">画像のX方向のピクセル数</param>
	''' <param name="IHeight">画像のY方向のピクセル数</param>
	''' <param name="cWPrm">ワールドファイルパラメータ</param>
	''' <param name="aPCS">AUXファイルに出力する座標系（平面直角座標系(日本測地系)）のEPSGコード</param>
	''' <param name="cToPnts">ワールドファイルパラメータから算出した実座標のポイント</param>
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
			' 地域パラメータファイルを読み込む
			' TODO:読み込み回数を最少にする
			cnv.JGD2000_RP_Init(SetParamPath(), bRet)

			' cWPrm(0)：1ピクセルのX方向の長さ
			' cWPrm(1)：行の回転パラメータ
			' cWPrm(2)：列の回転パラメータ
			' cWPrm(3)：1ピクセルのY方向の長さ
			' cWPrm(4)：左上ピクセルの中心位置のX座標(平面直角座標系(世界測地系))
			' cWPrm(5)：左上ピクセルの中心位置のY座標(平面直角座標系(世界測地系))

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
		'UPGRADE_NOTE: オブジェクト ULpntTo をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		ULpntTo = Nothing
		'UPGRADE_NOTE: オブジェクト LLpntTo をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		LLpntTo = Nothing
		'UPGRADE_NOTE: オブジェクト LRpntTo をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		LRpntTo = Nothing
		'UPGRADE_NOTE: オブジェクト URpntTo をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		URpntTo = Nothing

		Exit Sub

ErrorHandler:
	End Sub

	''' <summary>
	''' 平面直角座標系（世界測地系）->平面直角座標系（日本測地系）への座標変換
	''' </summary>
	''' <param name="cnv">座標変換用のcrdcnvLib</param>
	''' <param name="aPCS">AUXファイルに出力する座標系（平面直角座標系(日本測地系)）のEPSGコード</param>
	''' <param name="before_x">変換前のX座標（平面直角座標系（世界測地系））</param>
	''' <param name="before_y">変換前のY座標（平面直角座標系（世界測地系））</param>
	''' <param name="after_y">変換後のX座標（平面直角座標系（日本測地系））</param>
	''' <param name="after_x">変換後のY座標（平面直角座標系（日本測地系））</param>
	''' <remarks>
	''' 実際の変換フロー：平面直角座標系（世界測地系）->経緯度（世界測地系）--[地域パラメータ]-->経緯度（日本測地系）->平面直角座標系（日本測地系）
	''' </remarks>
	Public Sub cnvXY19(ByVal cnv As crdcnvLibLib.CrdConvert, ByRef aPCS As Integer, ByVal before_x As Double, ByVal before_y As Double, ByRef after_y As Double, ByRef after_x As Double)
		' TODO:after_xとafter_yを逆にしておかないとうまくいかない（crd_cnvのLLtoXY19()のせい？）

		' 平面直角座標系(世界測地系) -> 経緯度(世界測地系)
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

		' 経緯度(世界測地系) --[地域パラメータ]--> 経緯度(日本測地系)
		Dim after_lon As Double = 0
		Dim after_lat As Double = 0
		Dim bRet As Boolean = False
		cnv.JGD2000toTOKYO_RP(before_lon, before_lat, after_lon, after_lat, True, bRet)

		' 経緯度(日本測地系) -> 平面直角座標系(日本測地系)
		cnv.LLtoXY19(SetAxis(aPCS), after_lon, after_lat, after_x, after_y)
		point = Nothing

	End Sub

	''' <summary>
	''' 地域パラメータファイルのパスを設定する
	''' </summary>
	''' <returns>地域パラメータファイルのパス</returns>
	''' <remarks>レジストリキーが見つからない場合や空文字が指定されている場合は既定のファイルパスを返す</remarks>
	Public Function SetParamPath() As String
		Dim filepath As String
		Dim regkey As Microsoft.Win32.RegistryKey = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(TOOL_REG)
		If regkey IsNot Nothing Then
			filepath = DirectCast(regkey.GetValue(PARAM_REG, DEFAULT_PARAM_PATH), String)
		End If

		' 見つからない場合はカレントディレクトリのパラメータファイルを設定
		If String.IsNullOrEmpty(filepath) Then
			filepath = DEFAULT_PARAM_PATH
		End If
		SetParamPath = filepath
	End Function

	' AUXファイルの作成
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
			ErrMsg("エラー", "ワークスペースの作成に失敗しました。")
			Exit Sub
		End If

		' ラスターデータのオープン
		If pRDS Is Nothing Then
			ErrMsg("エラー", "ラスタデータが取得できませんでした。")
			Exit Sub
		End If

		' ラスターデータの取得
		Dim pRas As ESRI.ArcGIS.Geodatabase.IRaster
		' Set pRas = pRDS.CreateDefaultRaster
		' 9.1 Bug 回避
		pRas = pRDS.CreateFullRaster

		' 04/09/10 追加
		Dim pBand As ESRI.ArcGIS.DataSourcesRaster.IRasterBandCollection
		pBand = pRas

		Dim pRasBand As ESRI.ArcGIS.DataSourcesRaster.IRasterBand
		pRasBand = pBand.Item(0)
		' ここまで

		Dim pRasGeo As ESRI.ArcGIS.DataSourcesRaster.IRasterGeometryProc
		pRasGeo = New ESRI.ArcGIS.DataSourcesRaster.RasterGeometryProc
		Dim pRasProp As ESRI.ArcGIS.DataSourcesRaster.IRasterProps
		pRasProp = pRasBand
		' Set pRasProp = pRas
		Dim pExt As ESRI.ArcGIS.Geometry.IEnvelope
		pExt = pRasProp.Extent

		'4隅ピクセル中心座標（ピクセル座標）
		Dim ULpntFrom As ESRI.ArcGIS.Geometry.IPoint : ULpntFrom = New ESRI.ArcGIS.Geometry.Point
		Dim LLpntFrom As ESRI.ArcGIS.Geometry.IPoint : LLpntFrom = New ESRI.ArcGIS.Geometry.Point
		Dim LRpntFrom As ESRI.ArcGIS.Geometry.IPoint : LRpntFrom = New ESRI.ArcGIS.Geometry.Point
		Dim URpntFrom As ESRI.ArcGIS.Geometry.IPoint : URpntFrom = New ESRI.ArcGIS.Geometry.Point

		' 航空写真の場合
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

		' リンクベクトルの作成（「FromPnts」がピクセル座標ポイント、「ToPnts」が地図座標）
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
			'UPGRADE_WARNING: オブジェクト pSRS.CreateProjectedCoordinateSystem の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
			pPrj = pSRS.CreateProjectedCoordinateSystem(PCS)
			'    ElseIf PCS < 0 Then
			''        Set pPrj = pSRS.CreateGeographicCoordinateSystem(esriSRGeoCS_WGS1984)
			'        Set pPrj = pSRS.CreateGeographicCoordinateSystem(esriSRGeoCS_IERSTerrestrialReferenceFrame1994)
			'    ' PCS = 0
		Else
			'UPGRADE_WARNING: オブジェクト pSRS.CreateGeographicCoordinateSystem の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
			pPrj = pSRS.CreateGeographicCoordinateSystem(ESRI.ArcGIS.Geometry.esriSRGeoCSType.esriSRGeoCS_Tokyo)
		End If

		Dim pGeoEdit As ESRI.ArcGIS.Geodatabase.IGeoDatasetSchemaEdit
		pGeoEdit = pRDS

		If pGeoEdit Is Nothing Then
			MsgBox("pGeoEditが取得できていません。")
		Else
			pGeoEdit.AlterSpatialReference(pPrj)
		End If

		'Clean up
		'Set pWSF = Nothing
		'UPGRADE_NOTE: オブジェクト pRWS をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pRWS = Nothing

		'UPGRADE_NOTE: オブジェクト pRDS をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pRDS = Nothing

		'UPGRADE_NOTE: オブジェクト pRas をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pRas = Nothing
		'UPGRADE_NOTE: オブジェクト pRasGeo をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pRasGeo = Nothing
		'UPGRADE_NOTE: オブジェクト pRasProp をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pRasProp = Nothing
		'UPGRADE_NOTE: オブジェクト pExt をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pExt = Nothing

		'UPGRADE_NOTE: オブジェクト pSRS をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pSRS = Nothing
		'UPGRADE_NOTE: オブジェクト pPrj をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pPrj = Nothing
		'UPGRADE_NOTE: オブジェクト pGeoEdit をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pGeoEdit = Nothing

		'UPGRADE_NOTE: オブジェクト mpts1 をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		mpts1 = Nothing
		'UPGRADE_NOTE: オブジェクト FromPnts をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		FromPnts = Nothing
		'UPGRADE_NOTE: オブジェクト ULpntFrom をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		ULpntFrom = Nothing
		'UPGRADE_NOTE: オブジェクト LLpntFrom をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		LLpntFrom = Nothing
		'UPGRADE_NOTE: オブジェクト LRpntFrom をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		LRpntFrom = Nothing
		'UPGRADE_NOTE: オブジェクト URpntFrom をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		URpntFrom = Nothing

		Exit Sub

ErrorHandler:
		'UPGRADE_NOTE: オブジェクト pRDS をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pRDS = Nothing
		ErrMsg("error", GetFilename(cTargetPath))
	End Sub

	''' <summary>
	''' 系番号から平面直角座標系(日本測地系)のEPSGコードを取得する
	''' </summary>
	''' <param name="cZone">平面直角座標系の系番号</param>
	''' <returns>対応する系番号の平面直角座標系（日本測地系）のEPSGコード</returns>
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
	''' 平面直角座標系(日本測地系)のEPSGコードから平面直角座標系(世界測地系)のEPSGコードを取得する
	''' </summary>
	''' <param name="afterPCS">平面直角座標系（日本測地系）のEPSGコード</param>
	''' <returns>入力に対応する平面直角座標系（世界測地系）のEPSGコード</returns>
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
	''' 平面直角座標系(日本測地系)のEPSGコードから系番号を取得する
	''' </summary>
	''' <param name="aPCS">平面直角座標系（日本測地系）のEPSGコード</param>
	''' <returns>入力に対応する系番号</returns>
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