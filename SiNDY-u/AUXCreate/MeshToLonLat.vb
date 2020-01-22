Option Strict Off
Option Explicit On
Module MeshToLonLat
	
	Const sMesh1XOffset As Double = 100#
	Const sMesh1YOffset As Double = 0#
    Const sMesh1X As Double = 1.0# ' 1度
    Const sMesh1Y As Double = 0.666666666666667 ' 2/3度
    Const sMesh2X As Double = 0.125 ' 1次メッシュ8分割
    Const sMesh2Y As Double = 0.0833333333333333 ' 1次メッシュ8分割
    Const sMesh3X As Double = 0.015625 ' 2次メッシュ8分割
    Const sMesh3Y As Double = 0.0104166666666667 ' 2次メッシュ8分割
	
	Const XYLimit As Integer = 1000000
	Function OutRangePos(ByRef cMeshCode As Integer, ByRef cToPnts As ESRI.ArcGIS.Geometry.IPointCollection) As Boolean
		Dim aa As Short
		Dim aCornerMesh(3) As Integer
		Dim i As Short
		
		Dim nX As Integer
		Dim nY As Integer
		Dim dLon As Double
		Dim dLat As Double
		
		On Error GoTo CheckError
		If MeshCodeToCorner(2, cMeshCode, MeshLevel(cMeshCode), aCornerMesh) = False Then
			Exit Function
		End If
		
		For i = 0 To UBound(aCornerMesh) Step 1
			If cToPnts.Point(i).X < 0 Then
				nX = XYLimit + cToPnts.Point(i).X
			Else
				nX = cToPnts.Point(i).X - XYLimit
			End If
			If cToPnts.Point(i).Y < 0 Then
				nY = XYLimit + cToPnts.Point(i).Y
			Else
				nY = cToPnts.Point(i).Y - XYLimit
			End If
			
			Call MeshToLL(aCornerMesh(i), nX, nY, dLon, dLat)
			cToPnts.Point(i).X = dLon : cToPnts.Point(i).Y = dLat
		Next 
		'UPGRADE_NOTE: Erase は System.Array.Clear にアップグレードされました。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"' をクリックしてください。
		System.Array.Clear(aCornerMesh, 0, aCornerMesh.Length)
		OutRangePos = True
		Exit Function
		
CheckError: 
        MsgBox("予期しないエラー #" & VB6.Format(Err.Number) & " が発生しました。" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "エラー")
		Stop
	End Function
	
    ' 4角の隣接メッシュコードを取得
    ' cCode = 0 メッシュ4角
    '       = 1 2x2メッシュ(2次メッシュのみ対応)
    '       = 2 のりしろ付きメッシュ（都市地図メッシュのみ対応）
	Function MeshCodeToCorner(ByRef cCode As Short, ByRef cMeshCode As Integer, ByRef cMeshLevel As Short, ByRef cCornerMesh() As Integer) As Boolean
		Dim aMesh1X As Integer
		Dim aMesh1Y As Integer
		Dim aMesh2X As Integer
		Dim aMesh2Y As Integer
		Dim aMesh3X As Integer
		Dim aMesh3Y As Integer
		Dim ULMeshCode As Integer
		Dim LLMeshCode As Integer
		Dim LRMeshCode As Integer
		Dim URMeshCode As Integer
		
		Dim aMove1X As Integer
		Dim aMove1Y As Integer
		Dim aMove2X As Integer
		Dim aMove2Y As Integer
		Dim aMove3X As Integer
		Dim aMove3Y As Integer
		
        Dim aLimit As Short ' リミット
        Dim aIncr As Short ' 増分
		
		On Error GoTo CheckError
		MeshCodeToCorner = False
		
		Select Case cCode
			Case 0
				aLimit = 7
				aIncr = 1
			Case 1
				aLimit = 6
				aIncr = 2
			Case 2
				aLimit = 7
				aIncr = 1
			Case Else
		End Select
		
        ' 1次メッシュ
		If cMeshLevel = 1 Then
			aMesh1X = Int(cMeshCode / 1) Mod 100
			aMesh1Y = Int(cMeshCode / 100)
			
            '左上
			aMove1Y = aMesh1Y
			aMove1X = aMesh1X
			aMove1Y = aMesh1Y + 1
			ULMeshCode = aMove1Y * 100 + aMove1X * 1
			
            ' 左下
			LLMeshCode = cMeshCode
			
            ' 右下
			aMove1Y = aMesh1Y
			aMove1X = aMesh1X
			aMove1X = aMesh1X + 1
			LRMeshCode = aMove1Y * 100 + aMove1X * 1
			
            ' 右上
			aMove1Y = aMesh1Y
			aMove1X = aMesh1X
			aMove1Y = aMesh1Y + 1
			aMove1X = aMesh1X + 1
			URMeshCode = aMove1Y * 100 + aMove1X * 1
			
            ' 2次メッシュ
		ElseIf cMeshLevel = 2 Then 
			aMesh1X = Int(cMeshCode / 100) Mod 100
			aMesh1Y = Int(cMeshCode / 10000)
			aMesh2X = cMeshCode Mod 10
			aMesh2Y = Int(cMeshCode / 10) Mod 10
			
            ' 左上
			aMove1Y = aMesh1Y
			aMove1X = aMesh1X
			aMove2Y = aMesh2Y
			aMove2X = aMesh2X
			
			If aMesh2Y >= 0 And aMesh2Y < aLimit Then
				aMove2Y = aMesh2Y + aIncr
			ElseIf aMesh2Y = aLimit Then 
				aMove1Y = aMesh1Y + 1
				aMove2Y = 0
			Else
				Debug.Print(cMeshCode)
			End If
			ULMeshCode = aMove1Y * 10000 + aMove1X * 100 + aMove2Y * 10 + aMove2X * 1
			
            ' 左下
			LLMeshCode = cMeshCode
			
            ' 右下
			aMove1Y = aMesh1Y
			aMove1X = aMesh1X
			aMove2Y = aMesh2Y
			aMove2X = aMesh2X
			If aMesh2X >= 0 And aMesh2X < aLimit Then
				aMove2X = aMesh2X + aIncr
			ElseIf aMesh2X = aLimit Then 
				aMove1X = aMesh1X + 1
				aMove2X = 0
			Else
				Debug.Print(cMeshCode)
			End If
			LRMeshCode = aMove1Y * 10000 + aMove1X * 100 + aMove2Y * 10 + aMove2X * 1
			
            ' 右上
			aMove1Y = aMesh1Y
			aMove1X = aMesh1X
			aMove2Y = aMesh2Y
			aMove2X = aMesh2X
			If aMesh2Y >= 0 And aMesh2Y < aLimit Then
				aMove2Y = aMesh2Y + aIncr
			ElseIf aMesh2Y = aLimit Then 
				aMove1Y = aMesh1Y + 1
				aMove2Y = 0
			Else
				Debug.Print(cMeshCode)
			End If
			If aMesh2X >= 0 And aMesh2X < aLimit Then
				aMove2X = aMesh2X + aIncr
			ElseIf aMesh2X = aLimit Then 
				aMove1X = aMesh1X + 1
				aMove2X = 0
			Else
				Debug.Print(cMeshCode)
			End If
			URMeshCode = aMove1Y * 10000 + aMove1X * 100 + aMove2Y * 10 + aMove2X * 1
			
            ' 都市地図メッシュ
		ElseIf cMeshLevel = 3 Then 
			aMesh1X = Int(cMeshCode / 10000) Mod 100
			aMesh1Y = Int(cMeshCode / 1000000)
			aMesh2X = Int(cMeshCode / 100) Mod 10
			aMesh2Y = Int(cMeshCode / 1000) Mod 10
			aMesh3X = cMeshCode Mod 10
			aMesh3Y = Int(cMeshCode / 10) Mod 10
			
            ' 左上
			aMove1Y = aMesh1Y
			aMove1X = aMesh1X
			aMove2Y = aMesh2Y
			aMove2X = aMesh2X
			aMove3Y = aMesh3Y
			aMove3X = aMesh3X
			If aMesh3Y >= 0 And aMesh3Y < 7 Then
				aMove3Y = aMesh3Y + aIncr
			ElseIf aMesh2Y >= 0 And aMesh2Y < 7 Then 
				aMove2Y = aMesh2Y + 1
				aMove3Y = 0
			ElseIf aMesh2Y = 7 Then 
				aMove1Y = aMesh1Y + 1
				aMove2Y = 0
				aMove3Y = 0
			Else
				Debug.Print(cMeshCode)
			End If
			
			If cCode = 2 Then
				If aMesh3X > 0 And aMesh3X <= 7 Then
					aMove3X = aMesh3X - aIncr
				ElseIf aMesh2X > 0 And aMesh2X <= 7 Then 
					aMove2X = aMesh2X - 1
					aMove3X = 7
				ElseIf aMesh2X = 0 Then 
					aMove1X = aMesh1X - 1
					aMove2X = 7
					aMove3X = 7
				Else
					Debug.Print(cMeshCode)
				End If
			End If
			
			ULMeshCode = aMove1Y * 1000000 + aMove1X * 10000 + aMove2Y * 1000 + aMove2X * 100 + aMove3Y * 10 + aMove3X
			
            ' 左下
			LLMeshCode = cMeshCode
			
			If cCode = 2 Then
				aMove1Y = aMesh1Y
				aMove1X = aMesh1X
				aMove2Y = aMesh2Y
				aMove2X = aMesh2X
				aMove3Y = aMesh3Y
				aMove3X = aMesh3X
				
				If aMesh3Y > 0 And aMesh3Y <= 7 Then
					aMove3Y = aMesh3Y - aIncr
				ElseIf aMesh2Y > 0 And aMesh2Y <= 7 Then 
					aMove2Y = aMesh2Y - 1
					aMove3Y = 7
				ElseIf aMesh2Y = 0 Then 
					aMove1Y = aMesh1Y - 1
					aMove2Y = 7
					aMove3Y = 7
				Else
					Debug.Print(cMeshCode)
				End If
				
				If aMesh3X > 0 And aMesh3X <= 7 Then
					aMove3X = aMesh3X - aIncr
				ElseIf aMesh2X > 0 And aMesh2X <= 7 Then 
					aMove2X = aMesh2X - 1
					aMove3X = 7
				ElseIf aMesh2X = 0 Then 
					aMove1X = aMesh1X - 1
					aMove2X = 7
					aMove3X = 7
				Else
					Debug.Print(cMeshCode)
				End If
				LLMeshCode = aMove1Y * 1000000 + aMove1X * 10000 + aMove2Y * 1000 + aMove2X * 100 + aMove3Y * 10 + aMove3X
			End If
			
            ' 右下
			aMove1Y = aMesh1Y
			aMove1X = aMesh1X
			aMove2Y = aMesh2Y
			aMove2X = aMesh2X
			aMove3Y = aMesh3Y
			aMove3X = aMesh3X
			If aMesh3X >= 0 And aMesh3X < 7 Then
				aMove3X = aMesh3X + 1
			ElseIf aMesh2X >= 0 And aMesh2X < 7 Then 
				aMove2X = aMesh2X + 1
				aMove3X = 0
			ElseIf aMesh2X = 7 Then 
				aMove1X = aMesh1X + 1
				aMove2X = 0
				aMove3X = 0
			Else
				Debug.Print(cMeshCode)
			End If
			
			If cCode = 2 Then
				If aMesh3Y > 0 And aMesh3Y <= 7 Then
					aMove3Y = aMesh3Y - aIncr
				ElseIf aMesh2Y > 0 And aMesh2Y <= 7 Then 
					aMove2Y = aMesh2Y - 1
					aMove3Y = 7
				ElseIf aMesh2Y = 0 Then 
					aMove1Y = aMesh1Y - 1
					aMove2Y = 7
					aMove3Y = 7
				Else
					Debug.Print(cMeshCode)
				End If
			End If
			LRMeshCode = aMove1Y * 1000000 + aMove1X * 10000 + aMove2Y * 1000 + aMove2X * 100 + aMove3Y * 10 + aMove3X
			
            ' 右上
			aMove1Y = aMesh1Y
			aMove1X = aMesh1X
			aMove2Y = aMesh2Y
			aMove2X = aMesh2X
			aMove3Y = aMesh3Y
			aMove3X = aMesh3X
			If aMesh3Y >= 0 And aMesh3Y < 7 Then
				aMove3Y = aMesh3Y + 1
			ElseIf aMesh2Y >= 0 And aMesh2Y < 7 Then 
				aMove2Y = aMesh2Y + 1
				aMove3Y = 0
			ElseIf aMesh2Y = 7 Then 
				aMove1Y = aMesh1Y + 1
				aMove2Y = 0
				aMove3Y = 0
			Else
				Debug.Print(cMeshCode)
			End If
			If aMesh3X >= 0 And aMesh3X < 7 Then
				aMove3X = aMesh3X + 1
			ElseIf aMesh2X >= 0 And aMesh2X < 7 Then 
				aMove2X = aMesh2X + 1
				aMove3X = 0
			ElseIf aMesh2X = 7 Then 
				aMove1X = aMesh1X + 1
				aMove2X = 0
				aMove3X = 0
			Else
				Debug.Print(cMeshCode)
			End If
			URMeshCode = aMove1Y * 1000000 + aMove1X * 10000 + aMove2Y * 1000 + aMove2X * 100 + aMove3Y * 10 + aMove3X
		End If
		
		cCornerMesh(0) = ULMeshCode
		cCornerMesh(1) = LLMeshCode
		cCornerMesh(2) = LRMeshCode
		cCornerMesh(3) = URMeshCode
		
		MeshCodeToCorner = True
		Exit Function
CheckError: 
        MsgBox("予期しないエラー #" & VB6.Format(Err.Number) & " が発生しました。" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "エラー")
		Stop
	End Function
	
    ' メッシュコードから4隅の経緯度を取得
    ' cCode : ワイドメッシュフラグ（1 or 0）
	Function MeshCodeToLonLat(ByRef cCode As Short, ByRef cMeshCode As Integer, ByRef cToPnts As ESRI.ArcGIS.Geometry.IPointCollection) As Boolean
		Dim aCornerMesh(3) As Integer
		
		Dim dLon As Double
		Dim dLat As Double
		
		Dim i As Short
		Dim hoge As Short
		
		Dim ULpntTo As ESRI.ArcGIS.Geometry.IPoint : ULpntTo = New ESRI.ArcGIS.Geometry.Point
		Dim LLpntTo As ESRI.ArcGIS.Geometry.IPoint : LLpntTo = New ESRI.ArcGIS.Geometry.Point
		Dim LRpntTo As ESRI.ArcGIS.Geometry.IPoint : LRpntTo = New ESRI.ArcGIS.Geometry.Point
		Dim URpntTo As ESRI.ArcGIS.Geometry.IPoint : URpntTo = New ESRI.ArcGIS.Geometry.Point
		
		On Error GoTo CheckError
		MeshCodeToLonLat = False
		
		If MeshCodeToCorner(cCode, cMeshCode, MeshLevel(cMeshCode), aCornerMesh) = False Then
			Exit Function
		End If
		For i = 0 To UBound(aCornerMesh) Step 1
			hoge = MeshToLL(aCornerMesh(i), 0, 0, dLon, dLat)
			
			Select Case i
				Case 0
					ULpntTo.X = dLon
					ULpntTo.Y = dLat
				Case 1
					LLpntTo.X = dLon
					LLpntTo.Y = dLat
				Case 2
					LRpntTo.X = dLon
					LRpntTo.Y = dLat
				Case 3
					URpntTo.X = dLon
					URpntTo.Y = dLat
				Case Else
					Debug.Print(cMeshCode)
			End Select
		Next 
		
		cToPnts.AddPoint(ULpntTo)
		cToPnts.AddPoint(LLpntTo)
		cToPnts.AddPoint(LRpntTo)
		cToPnts.AddPoint(URpntTo)
		
		'UPGRADE_NOTE: Erase は System.Array.Clear にアップグレードされました。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"' をクリックしてください。
		System.Array.Clear(aCornerMesh, 0, aCornerMesh.Length)
		MeshCodeToLonLat = True
		Exit Function
CheckError: 
		'UPGRADE_NOTE: Erase は System.Array.Clear にアップグレードされました。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"' をクリックしてください。
		System.Array.Clear(aCornerMesh, 0, aCornerMesh.Length)
		'UPGRADE_NOTE: オブジェクト ULpntTo をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		ULpntTo = Nothing
		'UPGRADE_NOTE: オブジェクト LLpntTo をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		LLpntTo = Nothing
		'UPGRADE_NOTE: オブジェクト LRpntTo をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		LRpntTo = Nothing
		'UPGRADE_NOTE: オブジェクト URpntTo をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		URpntTo = Nothing
        MsgBox("予期しないエラー #" & VB6.Format(Err.Number) & " が発生しました。" & Err.Description, MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, "エラー")
		Stop
	End Function
    ' メッシュコードからレベルの取得
	Function MeshLevel(ByRef cMeshCode As Integer) As Short
		If cMeshCode >= 1000 And cMeshCode <= 9999 Then
			MeshLevel = 1
		ElseIf cMeshCode >= 100000 And cMeshCode <= 999999 Then 
			MeshLevel = 2
		ElseIf cMeshCode >= 10000000 And cMeshCode <= 99999999 Then 
			MeshLevel = 3
		Else
			MeshLevel = 0
		End If
	End Function
	
	'Function MeshCodeToLonLat(cCode As Integer, cMeshCode As Long, cToPnts As IPointCollection) As Boolean
	'    Dim aMeshLevel As Integer
	'    Dim aMeshCode As Long
	'    Dim aMesh1X As Long
	'    Dim aMesh1Y As Long
	'    Dim aMesh2X As Long
	'    Dim aMesh2Y As Long
	'    Dim aMesh3X As Long
	'    Dim aMesh3Y As Long
	'    Dim aCornerMesh(3) As Long
	'
	'    Dim i As Integer
	'
	'    Dim ULpntTo As IPoint: Set ULpntTo = New Point
	'    Dim LLpntTo As IPoint: Set LLpntTo = New Point
	'    Dim LRpntTo As IPoint: Set LRpntTo = New Point
	'    Dim URpntTo As IPoint: Set URpntTo = New Point
	'
	'    On Error GoTo CheckError
	'    MeshCodeToLonLat = False
	'
	'    aMeshLevel = MeshLevel(cMeshCode)
	'    If MeshCodeToCorner(cCode, cMeshCode, aMeshLevel, aCornerMesh()) = False Then
	'        Exit Function
	'    End If
	'    For i = 0 To UBound(aCornerMesh()) Step 1
	'        If aMeshLevel = 1 Then
	'            aMesh1X = aCornerMesh(i) Mod 100
	'            aMesh1Y = Int(aCornerMesh(i) / 100)
	'            Select Case i
	'            Case 0
	'                ULpntTo.X = (sMesh1X * aMesh1X) + sMesh1XOffset
	'                ULpntTo.Y = (sMesh1Y * aMesh1Y) + sMesh1YOffset
	'            Case 1
	'                LLpntTo.X = (sMesh1X * aMesh1X) + sMesh1XOffset
	'                LLpntTo.Y = (sMesh1Y * aMesh1Y) + sMesh1YOffset
	'            Case 2
	'                LRpntTo.X = (sMesh1X * aMesh1X) + sMesh1XOffset
	'                LRpntTo.Y = (sMesh1Y * aMesh1Y) + sMesh1YOffset
	'            Case 3
	'                URpntTo.X = (sMesh1X * aMesh1X) + sMesh1XOffset
	'                URpntTo.Y = (sMesh1Y * aMesh1Y) + sMesh1YOffset
	'            Case Else
	'                Debug.Print cMeshCode
	'            End Select
	'        ElseIf aMeshLevel = 2 Then
	'            aMesh1X = Int(aCornerMesh(i) / 100) Mod 100
	'            aMesh1Y = Int(aCornerMesh(i) / 10000)
	'            aMesh2X = aCornerMesh(i) Mod 10
	'            aMesh2Y = Int(aCornerMesh(i) / 10) Mod 10
	'            Select Case i
	'            Case 0
	'                ULpntTo.X = (sMesh1X * aMesh1X) + (sMesh2X * aMesh2X) + sMesh1XOffset
	'                ULpntTo.Y = (sMesh1Y * aMesh1Y) + (sMesh2Y * aMesh2Y) + sMesh1YOffset
	'            Case 1
	'                LLpntTo.X = (sMesh1X * aMesh1X) + (sMesh2X * aMesh2X) + sMesh1XOffset
	'                LLpntTo.Y = (sMesh1Y * aMesh1Y) + (sMesh2Y * aMesh2Y) + sMesh1YOffset
	'            Case 2
	'                LRpntTo.X = (sMesh1X * aMesh1X) + (sMesh2X * aMesh2X) + sMesh1XOffset
	'                LRpntTo.Y = (sMesh1Y * aMesh1Y) + (sMesh2Y * aMesh2Y) + sMesh1YOffset
	'            Case 3
	'                URpntTo.X = (sMesh1X * aMesh1X) + (sMesh2X * aMesh2X) + sMesh1XOffset
	'                URpntTo.Y = (sMesh1Y * aMesh1Y) + (sMesh2Y * aMesh2Y) + sMesh1YOffset
	'            Case Else
	'                Debug.Print cMeshCode
	'            End Select
	'        ElseIf aMeshLevel = 3 Then
	'            aMesh1X = Int(aCornerMesh(i) / 10000) Mod 100
	'            aMesh1Y = Int(aCornerMesh(i) / 1000000)
	'            aMesh2X = Int(aCornerMesh(i) / 100) Mod 10
	'            aMesh2Y = Int(aCornerMesh(i) / 1000) Mod 10
	'            aMesh3X = aCornerMesh(i) Mod 10
	'            aMesh3Y = Int(aCornerMesh(i) / 10) Mod 10
	'
	'            Select Case i
	'            Case 0
	'                ULpntTo.X = (sMesh1X * aMesh1X) + (sMesh2X * aMesh2X) + (sMesh3X * aMesh3X) + sMesh1XOffset
	'                ULpntTo.Y = (sMesh1Y * aMesh1Y) + (sMesh2Y * aMesh2Y) + (sMesh3Y * aMesh3Y) + sMesh1YOffset
	'            Case 1
	'                LLpntTo.X = (sMesh1X * aMesh1X) + (sMesh2X * aMesh2X) + (sMesh3X * aMesh3X) + sMesh1XOffset
	'                LLpntTo.Y = (sMesh1Y * aMesh1Y) + (sMesh2Y * aMesh2Y) + (sMesh3Y * aMesh3Y) + sMesh1YOffset
	'            Case 2
	'                LRpntTo.X = (sMesh1X * aMesh1X) + (sMesh2X * aMesh2X) + (sMesh3X * aMesh3X) + sMesh1XOffset
	'                LRpntTo.Y = (sMesh1Y * aMesh1Y) + (sMesh2Y * aMesh2Y) + (sMesh3Y * aMesh3Y) + sMesh1YOffset
	'            Case 3
	'                URpntTo.X = (sMesh1X * aMesh1X) + (sMesh2X * aMesh2X) + (sMesh3X * aMesh3X) + sMesh1XOffset
	'                URpntTo.Y = (sMesh1Y * aMesh1Y) + (sMesh2Y * aMesh2Y) + (sMesh3Y * aMesh3Y) + sMesh1YOffset
	'            Case Else
	'                Debug.Print cMeshCode
	'            End Select
	'        Else
    '            Debug.Print "メッシュコード " & Str(cMeshCode) & " は不正です。"
	'            Exit Function
	'        End If
	'    Next
	'
	'    cToPnts.AddPoint ULpntTo
	'    cToPnts.AddPoint LLpntTo
	'    cToPnts.AddPoint LRpntTo
	'    cToPnts.AddPoint URpntTo
	'
	'    Erase aCornerMesh
	'    MeshCodeToLonLat = True
	'    Exit Function
	'CheckError:
	'    Erase aCornerMesh
	'    Set ULpntTo = Nothing
	'    Set LLpntTo = Nothing
	'    Set LRpntTo = Nothing
	'    Set URpntTo = Nothing
	'Invalid_string_refer_to_original_code
	'    Stop
	'End Function
End Module