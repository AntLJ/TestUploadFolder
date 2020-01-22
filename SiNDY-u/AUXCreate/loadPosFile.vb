Option Strict Off
Option Explicit On
Module loadPosFile
	
    ' ���W���e�L�X�g�t�@�C���Ǎ� - Bug5122�Ή�
	Public Function loadLocaleInfo(ByRef cLocale As String, ByRef cToPnts As ESRI.ArcGIS.Geometry.IPointCollection) As Boolean
		Dim aFile As Short
		Dim i As Integer
		Dim Flag As Boolean
		Dim aGetLine As String
		Dim aLocInfo() As String
		
		On Error GoTo CheckError
		loadLocaleInfo = True
		
		aFile = FreeFile
		i = 0
		
		FileOpen(aFile, cLocale, OpenMode.Input, , OpenShare.LockWrite)
		Flag = True
		
		Do While Not EOF(aFile)
			aGetLine = LineInput(aFile)
			If Left(aGetLine, 1) <> "#" And aGetLine <> vbNullString Then
				aLocInfo = Split(aGetLine, vbTab)
				Exit Do
			End If
		Loop 
		FileClose(aFile)
		Flag = False
		
        If aLocInfo(2) = "���E" Then
            GeoTransformation(cToPnts)
        End If
		
        '4���s�N�Z�����S���W�i�n�}���W�n�j
		Dim ULpntTo As ESRI.ArcGIS.Geometry.IPoint : ULpntTo = New ESRI.ArcGIS.Geometry.Point
		Dim LLpntTo As ESRI.ArcGIS.Geometry.IPoint : LLpntTo = New ESRI.ArcGIS.Geometry.Point
		Dim LRpntTo As ESRI.ArcGIS.Geometry.IPoint : LRpntTo = New ESRI.ArcGIS.Geometry.Point
		Dim URpntTo As ESRI.ArcGIS.Geometry.IPoint : URpntTo = New ESRI.ArcGIS.Geometry.Point
		
        ' �p�����[�^�̃`�F�b�N
		If Val(aLocInfo(3)) <> 0 And Val(aLocInfo(4)) <> 0 And Val(aLocInfo(5)) <> 0 And Val(aLocInfo(6)) <> 0 And Val(aLocInfo(7)) <> 0 And Val(aLocInfo(8)) <> 0 And Val(aLocInfo(9)) <> 0 And Val(aLocInfo(10)) <> 0 Then
			ULpntTo.X = Val(aLocInfo(3)) : ULpntTo.Y = Val(aLocInfo(4))
			LLpntTo.X = Val(aLocInfo(7)) : LLpntTo.Y = Val(aLocInfo(8))
			LRpntTo.X = Val(aLocInfo(9)) : LRpntTo.Y = Val(aLocInfo(10))
			URpntTo.X = Val(aLocInfo(5)) : URpntTo.Y = Val(aLocInfo(6))
            ' ����A�E���A�E��
		ElseIf Val(aLocInfo(3)) <> 0 And Val(aLocInfo(4)) <> 0 And Val(aLocInfo(5)) <> 0 And Val(aLocInfo(6)) <> 0 And Val(aLocInfo(9)) <> 0 And Val(aLocInfo(10)) <> 0 Then 
			ULpntTo.X = Val(aLocInfo(3)) : ULpntTo.Y = Val(aLocInfo(4))
			LLpntTo.X = Val(aLocInfo(3)) : LLpntTo.Y = Val(aLocInfo(10))
			LRpntTo.X = Val(aLocInfo(9)) : LRpntTo.Y = Val(aLocInfo(10))
			URpntTo.X = Val(aLocInfo(5)) : URpntTo.Y = Val(aLocInfo(6))
            ' ����A�����A�E��
		ElseIf Val(aLocInfo(3)) <> 0 And Val(aLocInfo(4)) <> 0 And Val(aLocInfo(7)) <> 0 And Val(aLocInfo(8)) <> 0 And Val(aLocInfo(9)) <> 0 And Val(aLocInfo(10)) <> 0 Then 
			ULpntTo.X = Val(aLocInfo(3)) : ULpntTo.Y = Val(aLocInfo(4))
			LLpntTo.X = Val(aLocInfo(7)) : LLpntTo.Y = Val(aLocInfo(8))
			LRpntTo.X = Val(aLocInfo(9)) : LRpntTo.Y = Val(aLocInfo(10))
			URpntTo.X = Val(aLocInfo(9)) : URpntTo.Y = Val(aLocInfo(4))
            ' ����A�����A�E��
		ElseIf Val(aLocInfo(3)) <> 0 And Val(aLocInfo(4)) <> 0 And Val(aLocInfo(5)) <> 0 And Val(aLocInfo(6)) <> 0 And Val(aLocInfo(7)) <> 0 And Val(aLocInfo(8)) <> 0 Then 
			ULpntTo.X = Val(aLocInfo(3)) : ULpntTo.Y = Val(aLocInfo(4))
			LLpntTo.X = Val(aLocInfo(7)) : LLpntTo.Y = Val(aLocInfo(8))
			LRpntTo.X = Val(aLocInfo(5)) : LRpntTo.Y = Val(aLocInfo(8))
			URpntTo.X = Val(aLocInfo(5)) : URpntTo.Y = Val(aLocInfo(6))
            ' �����A�E���A�E��
		ElseIf Val(aLocInfo(5)) <> 0 And Val(aLocInfo(6)) <> 0 And Val(aLocInfo(7)) <> 0 And Val(aLocInfo(8)) <> 0 And Val(aLocInfo(9)) <> 0 And Val(aLocInfo(10)) <> 0 Then 
			ULpntTo.X = Val(aLocInfo(7)) : ULpntTo.Y = Val(aLocInfo(6))
			LLpntTo.X = Val(aLocInfo(7)) : LLpntTo.Y = Val(aLocInfo(8))
			LRpntTo.X = Val(aLocInfo(9)) : LRpntTo.Y = Val(aLocInfo(10))
			URpntTo.X = Val(aLocInfo(5)) : URpntTo.Y = Val(aLocInfo(6))
            ' ����A�E��
		ElseIf Val(aLocInfo(3)) <> 0 And Val(aLocInfo(4)) <> 0 And Val(aLocInfo(9)) <> 0 And Val(aLocInfo(10)) <> 0 Then 
			ULpntTo.X = Val(aLocInfo(3)) : ULpntTo.Y = Val(aLocInfo(4))
			LLpntTo.X = Val(aLocInfo(3)) : LLpntTo.Y = Val(aLocInfo(10))
			LRpntTo.X = Val(aLocInfo(9)) : LRpntTo.Y = Val(aLocInfo(10))
			URpntTo.X = Val(aLocInfo(9)) : URpntTo.Y = Val(aLocInfo(4))
            ' �����A�E��
		ElseIf Val(aLocInfo(5)) <> 0 And Val(aLocInfo(6)) <> 0 And Val(aLocInfo(7)) <> 0 And Val(aLocInfo(8)) <> 0 Then 
			ULpntTo.X = Val(aLocInfo(7)) : ULpntTo.Y = Val(aLocInfo(6))
			LLpntTo.X = Val(aLocInfo(7)) : LLpntTo.Y = Val(aLocInfo(8))
			LRpntTo.X = Val(aLocInfo(5)) : LRpntTo.Y = Val(aLocInfo(8))
			URpntTo.X = Val(aLocInfo(5)) : URpntTo.Y = Val(aLocInfo(6))
		Else
			loadLocaleInfo = False
			Erase aLocInfo
			Exit Function
		End If
		
        '�����N�x�N�g���̍쐬�i�uToPnts�v���n�}���W�j
		cToPnts.AddPoint(ULpntTo)
		cToPnts.AddPoint(LLpntTo)
		cToPnts.AddPoint(LRpntTo)
		cToPnts.AddPoint(URpntTo)
		
        If aLocInfo(2) = "���E" Then
            GeoTransformation(cToPnts)
        End If
		
		Erase aLocInfo
		Exit Function
		
CheckError: 
		FileError()
		If Flag = True Then FileClose(aFile)
		Erase aLocInfo
		loadLocaleInfo = False
	End Function
	
    ' ���[���h�t�@�C���̓ǂݍ���
	Public Function loadWorldPrm(ByRef cTargetPath As String, ByRef cSuffix As String, ByRef cWPrm() As Double) As Boolean
		Dim aFile As Short
		Dim i As Integer
		Dim aPrm As String
		
		Dim Flag As Boolean
		
		On Error GoTo CheckError
		
		loadWorldPrm = True
		aFile = FreeFile
		i = 0
		
		FileOpen(aFile, Left(cTargetPath, InStr(cTargetPath, ".") - 1) & cSuffix, OpenMode.Input, , OpenShare.LockWrite)
		Flag = True
		
		Do While Not EOF(aFile)
			aPrm = LineInput(aFile)
			If aPrm <> vbNullString Then
				ReDim Preserve cWPrm(i)
				cWPrm(i) = Val(aPrm)
				i = i + 1
			End If
		Loop 
		
		FileClose(aFile)
		Flag = False
		
		Exit Function
		
CheckError: 
		FileError()
		If Flag = True Then FileClose(aFile)
		loadWorldPrm = False
	End Function
	
    ' �s�s�v��}�l����`�t�@�C���̓ǂݍ���
	Public Function loadTKZFile(ByRef cTKZFile As String, ByRef cTKZLine() As String) As Boolean
		Dim aFile As Short
		Dim i As Integer
		Dim Flag As Boolean
		Dim aTxtLine() As String
		
		On Error GoTo CheckError
		loadTKZFile = True
		
		aFile = FreeFile
		i = 0
		
		FileOpen(aFile, cTKZFile, OpenMode.Input, , OpenShare.LockWrite)
		Flag = True
		
		Do While Not EOF(aFile)
			ReDim Preserve aTxtLine(i)
			aTxtLine(i) = LineInput(aFile)
			i = i + 1
		Loop 
		
		cTKZLine = VB6.CopyArray(aTxtLine)
		
		Erase aTxtLine
		FileClose(aFile)
		Flag = False
		
		Exit Function
		
CheckError: 
		FileError()
		If Flag = True Then FileClose(aFile)
		Erase aTxtLine
		loadTKZFile = False
	End Function
End Module