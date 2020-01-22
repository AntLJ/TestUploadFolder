Option Strict Off
Option Explicit On
Module GeoTransform
	
	Public Function GeoTransformation(ByRef cToPnts As ESRI.ArcGIS.Geometry.IPointCollection) As Boolean
		
		' Create a spatial reference factory
		Dim pSRF As ESRI.ArcGIS.Geometry.ISpatialReferenceFactory2
		pSRF = New ESRI.ArcGIS.Geometry.SpatialReferenceEnvironment
		
		' Create a GeocentricTranslation object for OSGB1936_to_WGS1984 using the factory.
		Dim pGT As ESRI.ArcGIS.Geometry.IGeocentricTranslation
		'UPGRADE_WARNING: オブジェクト pSRF.CreateGeoTransformation の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
		pGT = pSRF.CreateGeoTransformation(ESRI.ArcGIS.Geometry.esriSRGeoTransformation3Type.esriSRGeoTransformation_Tokyo_To_JGD_2000_1)
		'    Set pGT = pSRF.CreateGeoTransformation(esriSRGeoTransformation_Tokyo_To_WGS1984_4)
		
		' Debug Code
		' How to get the parameters from the Geotransformation object
		'    Dim dx As Double
		'    Dim dy As Double
		'    Dim dz As Double
		'    pGT.GetParameters dx, dy, dz
		'    Debug.Print dx, dy, dz
		
		' Debug Code
		' How to get the from/to SpatialReferences that are supported by this geotransformation.
		'    Dim pFromGCS As IGeographicCoordinateSystem
		'    Dim pToGCS As IGeographicCoordinateSystem
		'    pGT.GetSpatialReferences pFromGCS, pToGCS
		'    Debug.Print pFromGCS.Name, pToGCS.Name
		
		' Get a Transform2D by QI'ing from the points collection object
		Dim pTransform As ESRI.ArcGIS.Geometry.ITransform2D
		pTransform = cToPnts
		
		' Make the call - note the direction and the geotrans object are all that are needed
		'pTransform.Transform esriTransformForward, pGT
		'UPGRADE_WARNING: オブジェクト pGT の既定プロパティを解決できませんでした。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"' をクリックしてください。
		pTransform.Transform(ESRI.ArcGIS.Geometry.esriTransformDirection.esriTransformReverse, pGT)
		
		' Debug Code
		' Output the results
		'    Debug.Print "Results using Transform..."
		'    Dim i As Integer
		'    For i = 0 To cToPnts.PointCount - 1
		'      Debug.Print "LON " & cToPnts.Point(i).X & " LAT " & cToPnts.Point(i).Y
		'    Next i
		
		'UPGRADE_NOTE: オブジェクト pTransform をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pTransform = Nothing
		'UPGRADE_NOTE: オブジェクト pGT をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pGT = Nothing
		'UPGRADE_NOTE: オブジェクト pSRF をガベージ コレクトするまでこのオブジェクトを破棄することはできません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"' をクリックしてください。
		pSRF = Nothing
		
		GeoTransformation = True
		Exit Function
	End Function
End Module