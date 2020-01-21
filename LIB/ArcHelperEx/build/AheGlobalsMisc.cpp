/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include "stdafx.h"
#include "AheGlobalsMisc.h"
#include "GlobalFunctions.h"

/////////////////////////////////////////////////////////////////////////////
//
// その他の関数
//
/////////////////////////////////////////////////////////////////////////////


IPointPtr AheGetPoint( IFeaturePtr ipFeature, LONG lIndex )
{
	IGeometryPtr ipGeom = NULL;
	IPointCollectionPtr ipCol = NULL;
	IPointPtr ipPoint = NULL;

	ipFeature->get_ShapeCopy( &ipGeom );
	AheGeometry2PointCollection( ipGeom, &ipCol );

	if( ipCol == NULL )
		ipPoint = ipGeom;
	else
		ipCol->get_Point( lIndex, &ipPoint );

	return ipPoint;
}

#if 0	// 使われていない
LONG AheGetPointCount( IFeaturePtr ipFeature )
{
	IGeometryPtr ipGeom = NULL;
	IPointCollectionPtr ipCol = NULL;
	LONG		lCount = 0;

	ipFeature->get_ShapeCopy( &ipGeom );
	AheGeometry2PointCollection( ipGeom, &ipCol );

	if( ipCol != NULL )
		ipCol->get_PointCount( &lCount );

	return lCount;
}
#endif

long AheFact(long lBase, long lCnt)
{
	if (lCnt == 0 )
		return 1;

	return lBase * AheFact(lBase, lCnt - 1);
}
BOOL AheIsVisibleScale( IMapPtr piMap, IGeoFeatureLayerPtr pGeoFeatureLayer )
{
	HRESULT hr;
	double dMaxScale = 0.0, dMinScale = 0.0, dCurrentScale = 0.0;

	if ( pGeoFeatureLayer == NULL ) return FALSE;

	// 現在のスケール取得
	piMap->get_MapScale( &dCurrentScale );

	hr = pGeoFeatureLayer->get_MaximumScale(&dMaxScale);		// 表示最小スケール
	hr = pGeoFeatureLayer->get_MinimumScale(&dMinScale);		// 表示最大スケール

	if ( (dMaxScale == 0.0) && (dMinScale == 0.0) )			// 未設定は表示
	{
		return TRUE;
	}
	else if ( (dMaxScale <= dCurrentScale) && (dCurrentScale <= dMinScale) )	// 範囲内なら表示
	{
		return TRUE;
	}
	else if ( (dMaxScale <= dCurrentScale) && (dMinScale == 0.0) )		// 最大0はOK
	{
		return TRUE;
	}
	else												// その他は非表示
	{
		return FALSE;
	}
}

_ICommandPtr AheGetCommandByProgramID( IDocumentPtr ipDoc, LPCTSTR lpcszProgramID )
{
	_ICommandPtr ipCommand;
	ICommandItemPtr ipCommandItem = AheGetCommandItemByProgramID( ipDoc, lpcszProgramID );
	if( ipCommandItem == NULL ) return NULL;

	ipCommandItem->get_Command( &ipCommand );

	return ipCommand;
}
ICommandItemPtr AheGetCommandItemByProgramID( IDocumentPtr ipDoc, LPCTSTR lpcszProgramID )
{
	if( ipDoc == NULL ) return NULL;

	ICommandBarsPtr 	ipCommandBars;
	ICommandItemPtr 	ipCommandItem;
	_ICommandPtr		ipCommand;
	IUIDPtr 			ipUID(CLSID_UID);
	VARIANT 			vName, vUID;

	ipDoc->get_CommandBars( &ipCommandBars );
	if( ipCommandBars == NULL ) return NULL;

	vName.vt = VT_BSTR;
	vName.bstrVal = T2BSTR( lpcszProgramID );
	ipUID->put_Value( vName );
	vUID.punkVal = ipUID;
	vUID.vt = VT_DISPATCH;

	ipCommandBars->Find( vUID, VARIANT_FALSE, VARIANT_TRUE, &ipCommandItem );
	if( ipCommandItem == NULL ) return NULL;

	return ipCommandItem;
}
IClonePtr AheGetClone( IUnknownPtr ipUnk )
{
	IClonePtr ipClone( ipUnk );
	IClonePtr ipCloneClone;

	if( ipClone == NULL ) return NULL;

	ipClone->Clone( &ipCloneClone );

	return ipCloneClone;
}

#if 0	// 使われていない
BOOL AheIsSameVariantObject( VARIANT va1, VARIANT va2 )
{
	CComVariant va( va1 );

	return ( va == va2 );
}
#endif

BOOL AheIsNullableField( IFieldPtr ipField)
{
	BOOL blResult = FALSE;

	VARIANT_BOOL vaIsNullable = VARIANT_FALSE;
	if( SUCCEEDED( ipField->get_IsNullable( &vaIsNullable)))
	{
		blResult = ( vaIsNullable == VARIANT_TRUE);
	}
	return blResult;
}

/**
 * @brief ジオメトリからポリライン取得
*/
BOOL AheGeometry2Polyline(IGeometryPtr ipGeometry, IPolyline** ppiPolyline)
{
	ISpatialReferencePtr	ipSpatialReference;
	IPolylinePtr			ipPolyline;

	ipGeometry->get_SpatialReference( &ipSpatialReference );
	if( SUCCEEDED( ipGeometry->QueryInterface( &ipPolyline) ) ) {
		ipPolyline->SimplifyNetwork();
		ipPolyline->putref_SpatialReference( ipSpatialReference );
		ipPolyline->SnapToSpatialReference();
		( *ppiPolyline) = ipPolyline;
		( *ppiPolyline)->AddRef();
		return TRUE;
	}
	return FALSE;
}

/**
  * @brief ジオメトリからポイントコレクション取得
*/
BOOL AheGeometry2PointCollection(IGeometryPtr ipGeometry, IPointCollection** ppiPointCollection)
{
	ISpatialReferencePtr	ipSpatialReference;
	IPointCollectionPtr		ipPointCollection;
	ICurvePtr				ipCurve;

	ipGeometry->get_SpatialReference( &ipSpatialReference );
	if( SUCCEEDED( ipGeometry->QueryInterface( &ipCurve) ) ) {
		IPolycurvePtr			ipPolycurve;

		ipCurve->putref_SpatialReference( ipSpatialReference );
		ipCurve->SnapToSpatialReference();
		if( SUCCEEDED( ipCurve->QueryInterface( &ipPolycurve) ) ) {
			ipPolycurve->putref_SpatialReference( ipSpatialReference );
			ipPolycurve->SnapToSpatialReference();

			if( SUCCEEDED( ipPolycurve->QueryInterface( &ipPointCollection ) ) ) {
				( *ppiPointCollection) = ipPointCollection;
				( *ppiPointCollection)->AddRef();
				return TRUE;
			}
		}
	}	
	return FALSE;
}

BOOL AheIsRightAndLeft(IPointPtr ipSPoint, IPointPtr ipMPoint, IPointPtr ipEPoint)
{
	IConstructCircularArcPtr ipConstructCircularArc( CLSID_CircularArc ); // 本来なら空間参照をセットすべきだが、時計回りかどうか調べるだけなのでまあいいか…
	ICircularArcPtr		ipCircularArc;
	VARIANT_BOOL		vValue;

	ipConstructCircularArc->ConstructThreePoints(ipSPoint, ipMPoint, ipEPoint, VARIANT_TRUE);
	ipCircularArc = ipConstructCircularArc;
	ipCircularArc->get_IsCounterClockwise( &vValue );
	if ( vValue == VARIANT_TRUE )
		return TRUE;
	else
		return FALSE;
}

BOOL AheSnapToSpatialReference( IGeometryPtr ipGeom, ISpatialReferencePtr ipSpRef )
{
	if( ipGeom == NULL || ipSpRef == NULL )
	{
		ATLASSERT( ipGeom != NULL && ipSpRef != NULL );
		return FALSE;
	}

	if( SUCCEEDED( ipGeom->putref_SpatialReference( ipSpRef ) ) )
	{
		if( SUCCEEDED( ipGeom->SnapToSpatialReference() ) )
			return TRUE;
	}

	ATLASSERT( FALSE );
	return FALSE;
}

BOOL AheForceSimplify( IGeometryPtr ipGeom )
{
	if( ipGeom == NULL ) return FALSE;

	ISpatialReferencePtr ipSpRef;
	ipGeom->get_SpatialReference( &ipSpRef );
	if( ipSpRef == NULL )
		ATLASSERT( ipSpRef != NULL );
	
	ITopologicalOperatorPtr ipTopo( ipGeom );
	if( ipTopo == NULL )
	{
		ATLASSERT( ipTopo != NULL );
		AheTraceGeometry( ipGeom, _T("AheForceSimplify() : ") );
		AheTraceSpatialReference( ipSpRef, _T("AheForceSimplify() : ") );
		return FALSE;
	}

	// 強制的に Simplify をかけるためのおまじない
	ITopologicalOperator2Ptr ipTopo2( ipGeom );
	if( ipTopo2 == NULL )
	{
		//ATLASSERT( ipTopo2 != NULL );
		//AheTraceGeometry( ipGeom, _T("AheForceSimplify() : ") );
		//AheTraceSpatialReference( ipSpRef, _T("AheForceSimplify() : ") );
		//return FALSE;
	}
	else
	{
		if( FAILED( ipTopo2->put_IsKnownSimple( VARIANT_FALSE ) ) ) return FALSE;
	}

	esriGeometryType geomType;
	ipGeom->get_GeometryType( &geomType );
	HRESULT hr = 0;
	// 形状によってかける Simplify を区別
	switch( geomType )
	{
		case esriGeometryPolyline: 
			hr = ((IPolylinePtr)ipGeom)->SimplifyNetwork();       
			ipTopo->Simplify(); 
			break;
		case esriGeometryPolygon:
			try {
				hr = ipTopo2->Simplify();
			} 
			catch(...) {
				ATLASSERT( FALSE );
				hr = ((IPolygonPtr)ipGeom)->SimplifyPreserveFromTo();
			}
			break;
		default: 
			hr = ipTopo->Simplify();
			break;
	}

	if( FAILED( hr ) )
	{
		AheTraceErrorMessage( _T("AheForceSimplify" ) );
		ATLASSERT( hr == 0 );
	}
	return SUCCEEDED( hr );
}

BOOL AheSnapToSpatialReferenceAndForceSimplify( IGeometryPtr ipGeom, ISpatialReferencePtr ipSpRef )
{
	if( ipGeom == NULL || ipSpRef == NULL )
	{
		ATLASSERT( ipGeom != NULL && ipSpRef != NULL );
		return FALSE;
	}

	if( AheSnapToSpatialReference( ipGeom, ipSpRef ) == TRUE )
		return AheForceSimplify( ipGeom );

	return FALSE;
}

CString AheGetErrorMessage()
{
	IErrorInfoPtr pError;

	USES_CONVERSION;

	if( SUCCEEDED( ::GetErrorInfo( 0, &pError ) ) && pError != NULL )
	{
		CComBSTR      strDescription, strSource;
		pError->GetDescription( &strDescription );
		pError->GetSource( &strSource );

		CString strErr;
		strErr.Format(_T("%s(%s)"), OLE2T(strDescription), OLE2T(strSource) );
		return strErr;
	}
	return _T("");
}

void AheTraceErrorMessage( LPCTSTR lpcszMessage )
{
	ATLTRACE(_T("%s ERROR : %s\n"), lpcszMessage, AheGetErrorMessage() );
}

BOOL AheHasSameSpatialReference( IGeometryPtr ipGeom1, IGeometryPtr ipGeom2 )
{
	if( ipGeom1 == NULL || ipGeom2 == NULL ) return FALSE;

	ISpatialReferencePtr ipSpRef1, ipSpRef2;
	VARIANT_BOOL vbIsPrecisionEqual = VARIANT_FALSE;

	ipGeom1->get_SpatialReference( &ipSpRef1 );
	ipGeom2->get_SpatialReference( &ipSpRef2 );

	if( ipSpRef1 != NULL && ipSpRef2 != NULL )
	{
		if( SUCCEEDED( ipSpRef1->IsPrecisionEqual( ipSpRef2, &vbIsPrecisionEqual ) ) )
			return ( vbIsPrecisionEqual == VARIANT_TRUE ? TRUE : FALSE );
		else
			AheTraceErrorMessage( _T("AheHasSameSpatialReference") );
	}
/*	else
		_ASSERTE( ipSpRef1 != NULL && ipSpRef2 != NULL ); ここは出さないほうがよい*/

	return FALSE;
}

CString AheGetWindowText( HWND hWnd )
{
	if( hWnd == NULL ) return _T("");

	INT nLen = ::GetWindowTextLength( hWnd ) + 1;
	LPTSTR lpszText = (LPTSTR)_malloca( nLen * sizeof(TCHAR) );
	if( lpszText )
		::GetWindowText( hWnd, lpszText, nLen );
	else
		_ASSERTE( lpszText );

	CString strRet( lpszText );
	_freea( lpszText );

	return strRet;
}

CString AheGetNow( LPSYSTEMTIME lpSystemTime/* = NULL*/ )
{
	SYSTEMTIME stSystemTime;
	if( NULL != lpSystemTime)
	{
		CopyMemory( &stSystemTime, lpSystemTime, sizeof( SYSTEMTIME));
	}
	else
	{
		::GetLocalTime( &stSystemTime);
	}
	CString cStrModifyDate;
	cStrModifyDate.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"), stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

	return cStrModifyDate;
}

CString AheVariant2Str( const CComVariant &va )
{
	USES_CONVERSION;

	CComVariant vaStr(_T(""));

	switch ( va.vt ) {
		case VT_EMPTY:
			vaStr = EMPTY_VALUE;
			break;
		case VT_NULL:
			vaStr = NULL_VALUE;
			break;
		case VT_BOOL:
			if( va.boolVal == VARIANT_TRUE )
				vaStr = TRUE_VALUE;
			else if( va.boolVal == VARIANT_FALSE )
				vaStr = FALSE_VALUE;
			break;
			case VT_UNKNOWN:
				vaStr = GEOMETRY_VALUE;
			break;
		default:
			vaStr.ChangeType( VT_BSTR, &va );
			break;
	}
	
	return CString( vaStr.bstrVal );
}

BOOL AheGetKeywordValue( CString& strInputWord, CString strInputValue, LPCTSTR szKeyWord, CString& nRetValue, INT nFlg )
{
	LPTSTR 		lpszTmp;

	// szKeyWord=******以外があるか
	if ( strInputWord.SpanIncluding(szKeyWord) != strInputWord )
		return FALSE;
	// 0以下 or NULL チェック
	if ( _tcstod( (LPCTSTR)strInputValue, &lpszTmp ) < 0 || strInputValue == _T("") )
		return FALSE;
	// 数字以外が入っているか
	if ( nFlg == 0 ) {
		strInputWord = strInputValue.SpanIncluding(_T("0123456789"));
	} else {
		strInputWord = strInputValue.SpanIncluding(_T("EWNS0123456789."));
	}
	if ( strInputWord != strInputValue )
		return FALSE;

	nRetValue = (LPCTSTR)strInputValue;

	return TRUE;

}
