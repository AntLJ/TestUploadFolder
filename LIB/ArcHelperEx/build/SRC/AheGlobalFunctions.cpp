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

/**
 * @file GlobalFunctions.cpp
 * @brief ArcObjectsグローバルヘルパー関数定義
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "GlobalFunctions.h"
#include <math.h>
#include <set>
#include <boost/assign/list_of.hpp>
#include "TDC/useful_headers/str_util.h"
#include "TDC/useful_headers/registry_util.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クエリフィルタ設定用関数
IQueryFilterPtr AheInitQueryFilter( IQueryFilter* ipFilter/*			= NULL*/,
								 LPCTSTR lpcszSubFields/*				= NULL*/,
								 LPCTSTR lpcszWhereClause/*				= NULL*/,... )
{
	IQueryFilterPtr ipFilterTmp;

	if( ! ipFilter )
		ipFilterTmp.CreateInstance( CLSID_QueryFilter );
	else
		ipFilterTmp = ipFilter;

	if( ipFilterTmp )
	{
		ipFilterTmp->put_SubFields( _bstr_t(lpcszSubFields) );
		if( lpcszWhereClause )
		{
			va_list args;
			va_start( args, lpcszWhereClause );
			CString strWhere;
			strWhere.FormatV( lpcszWhereClause, args );
			ipFilterTmp->put_WhereClause( CComBSTR((LPCTSTR)strWhere) );
			va_end(args);
		}
	}
	else
		_ASSERTE( ipFilterTmp );

	return ipFilterTmp;
}

// 空間クエリフィルタ設定用関数
ISpatialFilterPtr AheInitSpatialFilter( ISpatialFilter* ipFilter/*		= NULL*/,
									IGeometry* pGeom/*					= NULL*/,
									LPCTSTR lpcszGeomField/*			= _T("Shape")*/,
									esriSearchOrder order/*				= esriSearchOrderSpatial*/,
									esriSpatialRelEnum SpatialRel/*		= esriSpatialRelIntersects*/,
									LPCTSTR lpcszSpatialRelDescription/*= NULL*/ )
{
	ISpatialFilterPtr ipFilterTmp;

	if( ! ipFilter )
		ipFilterTmp.CreateInstance( CLSID_SpatialFilter );
	else
		ipFilterTmp = ipFilter;

	if( ipFilterTmp )
	{
		ipFilterTmp->putref_Geometry( pGeom );
		ipFilterTmp->put_GeometryField( _bstr_t(lpcszGeomField) );
		ipFilterTmp->put_SearchOrder( order );
		ipFilterTmp->put_SpatialRel( SpatialRel );
		ipFilterTmp->put_SpatialRelDescription( _bstr_t(lpcszSpatialRelDescription) );
	}
	else
		_ASSERTE( ipFilterTmp );

	return ipFilterTmp;
}

long AheGetOID( _IRow* ipRow )
{
	long lOID = -1;
	if( ipRow != NULL )
	{
		VARIANT_BOOL vaHasOID = VARIANT_FALSE;
		ipRow->get_HasOID( &vaHasOID );
		if( vaHasOID )
			ipRow->get_OID( &lOID );
	}
	return lOID;
}

// フィーチャから形状を取得する
IGeometryPtr AheGetShape( IFeature* ipFeature )
{
	if( ! ipFeature )
		return NULL;

	IGeometryPtr ipRet; // 戻り値
	ipFeature->get_Shape( &ipRet );

	return ipRet;
}

// フィーチャ形状コピーを取得する
IGeometryPtr AheGetShapeCopy( IFeature* ipFeature )
{
	if( ! ipFeature )
		return NULL;

	IGeometryPtr ipRet; // 戻り値
	ipFeature->get_ShapeCopy( &ipRet );

	return ipRet;
}

// COMオブジェクトのコピーを作成する
IClonePtr AheCreateClone( IUnknown* ipUnk )
{
	// ICloneインタフェースをサポートしていないのを区別するため、
	// NULLが来たらすぐ返す
	if( !ipUnk )
		return NULL;

	IClonePtr ipClone( ipUnk ), ipRet;
	if( ! ipClone )
	{
		_ASSERTE( ipClone ); // ICloneインターフェースをサポートしていない
		return NULL;
	}

	if( FAILED( ipClone->Clone( &ipRet ) ) )
		_ASSERTE(false); // クローン作成失敗

	return ipRet;
}

// プロパティセットを比較する
bool AheIsSamePropertySet( IPropertySet* ipProp1, IPropertySet* ipProp2 )
{
	bool bRet = false;	// 返り値
	if( ipProp1 && ipProp2 )
	{
		VARIANT vaValue;
		CString strUser1, strServer1, strVersion1;
		CString strUser2, strServer2, strVersion2;

		VariantInit( &vaValue );
		ipProp1->GetProperty( CComBSTR(L"USER"), &vaValue ); if( VT_BSTR == vaValue.vt ) strUser1 = vaValue.bstrVal; VariantClear(&vaValue);
		ipProp2->GetProperty( CComBSTR(L"USER"), &vaValue ); if( VT_BSTR == vaValue.vt ) strUser2 = vaValue.bstrVal; VariantClear(&vaValue);
		ipProp1->GetProperty( CComBSTR(L"SERVER"), &vaValue ); if( VT_BSTR == vaValue.vt ) strServer1 = vaValue.bstrVal; VariantClear(&vaValue);
		ipProp2->GetProperty( CComBSTR(L"SERVER"), &vaValue ); if( VT_BSTR == vaValue.vt ) strServer2 = vaValue.bstrVal; VariantClear(&vaValue);
		ipProp1->GetProperty( CComBSTR(L"VERSION"), &vaValue ); if( VT_BSTR == vaValue.vt ) strVersion1 = vaValue.bstrVal; VariantClear(&vaValue);
		ipProp2->GetProperty( CComBSTR(L"VERSION"), &vaValue ); if( VT_BSTR == vaValue.vt ) strVersion2 = vaValue.bstrVal; VariantClear(&vaValue);
		if( strUser1 == strUser2 && strServer1 == strServer2 && strVersion1 == strVersion2 )
			bRet = true;
	}

	return bRet;
}

// セパレータ付き文字列をリストに格納する
void AheSepStr2listString( LPCTSTR lpcszTargetStr, LPCTSTR lpcszSep, std::list<CString>& listStr )
{
	CString strTarget( lpcszTargetStr );

	int iPos = -1;

	while( 1 )
	{
		iPos = strTarget.Find( lpcszSep );
		if( iPos > 0 )
			listStr.push_back( strTarget.Left( iPos ) );
		else if( iPos < 0 )
			break;

		strTarget = strTarget.Right( strTarget.GetLength() - ( iPos + 1 ) - ( lstrlen( lpcszSep ) - 1 ) );
	}

	if( ! strTarget.IsEmpty() )
		listStr.push_back( strTarget );
}

// セパレータ付き文字列をマップに格納する
void AheSepStr2mapString( LPCTSTR lpcszTargetStr, LPCTSTR lpcszSep1, LPCTSTR lpcszSep2, std::map<CString,CString>& mapStr )
{
	std::list<CString> listStr;
	AheSepStr2listString( lpcszTargetStr, lpcszSep1, listStr );
	for( std::list<CString>::const_iterator it = listStr.begin(); it != listStr.end(); ++it )
	{
		std::list<CString> listStr2;
		AheSepStr2listString( *it, lpcszSep2, listStr2 );
		if( listStr2.size() == 2 )
		{
			std::list<CString>::const_iterator it21 = listStr2.begin();
			std::list<CString>::const_iterator it22 = it21; ++it22;
			mapStr[*it21] = *it22;
		}
	}
}

// 空間参照の最小単位距離を返す
double AheGetXYDomain1UnitLength( ISpatialReference* ipSpRef )
{
	double dRet = -1.0;	// 返り値

	if( ! ipSpRef )
	{
		_ASSERTE(false);
		return dRet;
	}

	double dX = 0.0f, dXMin = 0.0f, dXMax = 0.0f, dYMin = 0.0f, dYMax = 0.0f;
	if( SUCCEEDED( ipSpRef->GetDomain( &dXMin, &dXMax, &dYMin, &dYMax ) ) )
	{
		dX = dXMax - dXMin;
		if( dX != 0.0 )
			dRet = (double)( (double)dX / pow( 2.0, AheIsHighPrecision(ipSpRef) ? 52.0 : 31.0 ) );
		else
			_ASSERTE( dX != 0.0 );
	}
	else
		_ASSERTE(false);

	return dRet;
}

// PGDB編集モードかどうかを調べる
bool AheIsPGDBEditMode()
{
	static BOOL bPGDBEditMode = -1;
	if( -1 == bPGDBEditMode )
	{
		CRegKey key;
		DWORD dwPGDBEditMode = 0;
		if( ERROR_SUCCESS == key.Open( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
		{
			if( ERROR_SUCCESS == key.QueryDWORDValue( _T("PGDBEditMode") ,dwPGDBEditMode ) )
				bPGDBEditMode = ( 1 == dwPGDBEditMode ) ? TRUE : FALSE;
		}
		ATLTRACE(_T("%s : HKEY_CURRENT_USER\\Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON\\PGDBEditMode=%d\n"), __FUNCSIG__, dwPGDBEditMode );
		if( -1 == bPGDBEditMode )
			bPGDBEditMode = FALSE;
	}
	return (bPGDBEditMode) ? true : false;
}

// 重複許可レイヤかどうか
bool AheIsOverlapableLayer( const CString& layername )
{
	using namespace uh;
	// レジストリから重複可能レイヤ名を取得
	static std::vector<CString> layers = str_util::split( reg_util::read( _T("OverlapLayers"), REGPATH_SINDY_E_COMMON ), _T(",") );
	for( std::vector<CString>::const_iterator it = layers.begin(); it != layers.end(); ++it )
	{
		if( 0 == layername.CompareNoCase(*it) )
		{
			return true;
		}
	}
	return false;
}

// SiNDY-cのテーブルに固有のフィールドか
bool AheIsSindycField( const CString& strField )
{
	using namespace boost::assign;
	using namespace sindy::schema::category::sindyc_table;

	static std::set<CString> c_fields = list_of( CString(kOrgOBJID) )
		                                       ( CString(kOrgOperator) )
		                                       ( CString(kOrgModifyDate) )
											   ( CString(kUpdateC) )
		                                       ( CString(kCheckC) );

	return ( c_fields.end() != c_fields.find(strField) );
};

#ifdef _DEBUG
void _AheTracePoint( IPoint* ipPoint, long lPart, long lIndex, LPCTSTR lpcszHeader = NULL )
{
	if( ! ipPoint )
	{
		_ASSERTE( ipPoint );
		return ;
	}

	double dx = 0, dy = 0;
	ipPoint->QueryCoords( &dx, &dy );

	char szXPos[_CVTBUFSIZE + 1] = "", szYPos[_CVTBUFSIZE + 1] = "";
	_gcvt_s( szXPos, _CVTBUFSIZE, dx, 100 );
	_gcvt_s( szYPos, _CVTBUFSIZE, dy, 100 );

#ifdef _UNICODE
	if( lpcszHeader )
		ATLTRACE(L"%s [%d][%d]：%S(%08X %08X) %S(%08X %08X)\n", lpcszHeader, lPart, lIndex, szXPos, dx,  szYPos, dy );
	else
		ATLTRACE(L"[%d][%d]：%S(%08X %08X) %S(%08X %08X)\n", lPart, lIndex, szXPos, dx,  szYPos, dy );
#else
	if( lpcszHeader )
		ATLTRACE("%s [%d][%d]：%s(%08X %08X) %s(%08X %08X)\n", lpcszHeader, lPart, lIndex, szXPos, dx,  szYPos, dy );
	else
		ATLTRACE("[%d][%d]：%s(%08X %08X) %s(%08X %08X)\n", lPart, lIndex, szXPos, dx,  szYPos, dy );
#endif // ifdef _UNICODE

	ISpatialReferencePtr ipSpRef;
	ipPoint->get_SpatialReference( &ipSpRef );
	if( ! ipSpRef )
	{
		ATLTRACE(_T("%s[%d][%d] : 空間参照がNULLです\n"), lpcszHeader, lPart, lIndex );
		//_ASSERT_EXPR( ipSpRef, _T("空間参照がNULLです") );
	}
}
#endif // ifdef _DEBUG

//
// 点列をトレースする
#ifdef _DEBUG
void _AheTracePartGeometry( IGeometry* ipGeom, esriGeometryType emType, long lPartNumber, LPCTSTR lpcszHeader = NULL )
{
	try {
		switch( emType )
		{
			case esriGeometryPoint:
				_AheTracePoint( (IPointPtr)ipGeom, lPartNumber, 0, lpcszHeader );
				break;
			case esriGeometryLine:
			{
				ILinePtr ipLine( ipGeom );
				IPointPtr ipPoint;
				for( int i = 0; i < 2; ++i )
				{
					if( 0 == i )
						ipLine->get_FromPoint( &ipPoint );
					else
						ipLine->get_ToPoint( &ipPoint );

					_AheTracePoint( ipPoint, lPartNumber, i, lpcszHeader );
				}
				break;
			}
			default:
			{
				IPointCollectionPtr ipPointCol( ipGeom );

				if( ipPointCol != NULL )
				{
					LONG lCount = -1;
					ipPointCol->get_PointCount( &lCount );

					for( LONG j = 0; j < lCount; j++ )
					{
						IPointPtr ipPoint;
						ipPointCol->get_Point( j, &ipPoint );
						_AheTracePoint( ipPoint, lPartNumber, j, lpcszHeader );
					}
				}
				break;
			}
		}
	}
	catch(...)
	{
		ATLTRACE(_T("%s: 例外をキャッチしました\n"), __FILE__LINE__ );
	}
}

CString _esriGeometryType2String( esriGeometryType emType )
{
	CString strRet;
	switch( emType )
	{
		case esriGeometryNull:			strRet = _T("esriGeometryNull"); break;
		case esriGeometryPoint:			strRet = _T("esriGeometryPoint"); break;
		case esriGeometryMultipoint:	strRet = _T("esriGeometryMultipoint"); break;
		case esriGeometryLine:			strRet = _T("esriGeometryLine"); break;
		case esriGeometryCircularArc:	strRet = _T("esriGeometryCircularArc"); break;
		case esriGeometryEllipticArc:	strRet = _T("esriGeometryEllipticArc"); break;
		case esriGeometryBezier3Curve:	strRet = _T("esriGeometryBezier3Curve"); break;
		case esriGeometryPath:			strRet = _T("esriGeometryPath"); break;
		case esriGeometryPolyline:		strRet = _T("esriGeometryPolyline"); break;
		case esriGeometryRing:			strRet = _T("esriGeometryRing"); break;
		case esriGeometryPolygon:		strRet = _T("esriGeometryPolygon"); break;
		case esriGeometryEnvelope:		strRet = _T("esriGeometryEnvelope"); break;
		case esriGeometryAny:			strRet = _T("esriGeometryAny"); break;
		case esriGeometryBag:			strRet = _T("esriGeometryBag"); break;
		case esriGeometryMultiPatch:	strRet = _T("esriGeometryMultiPatch"); break;
		case esriGeometryTriangleStrip:	strRet = _T("esriGeometryTriangleStrip"); break;
		case esriGeometryTriangleFan:	strRet = _T("esriGeometryTriangleFan"); break;
		case esriGeometryRay:			strRet = _T("esriGeometryRay"); break;
		case esriGeometrySphere:		strRet = _T("esriGeometrySphere"); break;
		case esriGeometryTriangles:		strRet = _T("esriGeometryTriangles"); break;
		default:						strRet = _T("esriGeometry????"); break;
	}
	return strRet;
}

void AheTraceGeometry( IGeometry* ipGeom, LPCTSTR lpcszHeader/* = NULL*/ )
{
	if( ipGeom == NULL ) 
	{
		ATLTRACE(_T("%s ジオメトリが存在しません\n"), lpcszHeader );
		return;
	}

	USES_CONVERSION;
	try {
		// 空間参照

		// タイプ
		esriGeometryType type;
		ipGeom->get_GeometryType( &type );
		ATLTRACE(_T("%s ジオメトリタイプ：%d(%s)\n"), lpcszHeader, type, _esriGeometryType2String(type) );

		// 形状
		ITopologicalOperatorPtr ipTopo( ipGeom );
		if( ipTopo == NULL )
			ATLTRACE(_T("%s ITopologicalOperator に変換できません\n"), lpcszHeader );
		else {
			VARIANT_BOOL vbIsKnownSimple = VARIANT_FALSE, vbIsSimple = VARIANT_FALSE;
			bool bThrowErr = false;	// 例外キャッチフラグ
			try {
				ipTopo->get_IsKnownSimple( &vbIsKnownSimple );
				ipTopo->get_IsSimple( &vbIsSimple );	// 落ちることがある
			} catch(...) {
				bThrowErr = true;
			}
			if( ! bThrowErr )
				ATLTRACE(_T("%s KnownSimple=%s Simple=%s\n"), lpcszHeader, ( vbIsKnownSimple ) ? _T("TRUE") : _T("FALSE"), ( vbIsSimple ) ? _T("TRUE") : _T("FALSE") );
			else
				ATLTRACE(_T("%s KnownSimple=%s Simple=%s\n"), lpcszHeader, ( vbIsKnownSimple ) ? _T("TRUE") : _T("FALSE"), ( vbIsSimple ) ? _T("NOCHECK") : _T("NOCHECK") );
		}

		switch( type )
		{
			case esriGeometryPoint: 
			{
				IPointPtr ipPoint( ipGeom );
				DOUBLE dx, dy;

				if( ipPoint == NULL ) return;
				ipPoint->QueryCoords( &dx, &dy );
				char szXPos[128];
				_gcvt( dx, 100, szXPos );
				char szYPos[128];
				_gcvt( dy, 100, szYPos );

				ATLTRACE(_T("%s X=%s(%08X %08X) Y=%s(%08X %08X)\n"), lpcszHeader, A2T(szXPos), dx, A2T(szYPos), dy );
			}
			break;
			case esriGeometryEnvelope:
			{
				DOUBLE dMinX = 0.0f, dMinY = 0.0f, dMaxX = 0.0f, dMaxY = 0.0f;
				IEnvelopePtr ipEnv( ipGeom );
				if( ipEnv == NULL ) return;
				ipEnv->QueryCoords( &dMinX, &dMinY, &dMaxX, &dMaxY );
				char szMinX[128], szMinY[128], szMaxX[128], szMaxY[128];
				_gcvt( dMinX, 100, szMinX );
				_gcvt( dMinY, 100, szMinY );
				_gcvt( dMaxX, 100, szMaxX );
				_gcvt( dMaxY, 100, szMaxY );

				ATLTRACE(_T("%s MinX=%s(%08X %08X) MinY=%s(%08X %08X) MaxX=%s(%08X %08X) MaxY=%s(%08X %08X)\n"), lpcszHeader, A2T(szMinX), dMinX, A2T(szMinY), dMinY, A2T(szMaxX), dMaxX, A2T(szMaxY), dMaxY );
			}
			break;
			default:
			{
				// 本来ならパートのタイプかどうか確認して振り分けるべきだが、面倒なので
				// IGeometryCollectionインターフェースがあるかどうかで分ける
				LONG lGeometryCount = -1;
				IGeometryCollectionPtr ipGeomCol( ipGeom );
				if( ipGeomCol != NULL )
				{
					ipGeomCol->get_GeometryCount( &lGeometryCount );
					for( LONG i = 0; i < lGeometryCount; i++ )
					{
						IGeometryPtr ipColGeom;
						ipGeomCol->get_Geometry( i, &ipColGeom );

						esriGeometryType emPartType;
						ipColGeom->get_GeometryType( &emPartType );
						_AheTracePartGeometry( ipColGeom, emPartType, i, lpcszHeader );
					}
				}
				else
					_AheTracePartGeometry( ipGeom, type, -1, lpcszHeader );
			}
			break;
		}
	}
	catch(...) {
		ATLTRACE(_T("%s: 例外をキャッチしました。\n"), __FILE__LINE__ );
	}
}
#endif // ifdef _DEBUG

// 空間参照をトレースする
#ifdef _DEBUG
void AheTraceSpatialReference( ISpatialReference* ipSpRef, LPCTSTR lpcszHeader )
{
	USES_CONVERSION;

	if( ! ipSpRef )
		return;

	CComBSTR bstrAbbreviation, bstrAlias, bstrName, bstrRemarks;
	LONG lFactoryCode = -1;
	DOUBLE dXMin = -1.0f, dYMin = -1.0f, dXMax = -1.0f, dYMax = -1.0;
	DOUBLE dFalseX = -1.0f, dFalseY = -1.0f, dXYUnits = -1.0f;
	DOUBLE dOutMMin = -1.0f, dOutMMax = -1.0f;
	DOUBLE dFalseM = -1.0f, dMUnits = -1.0f;
	DOUBLE dOutZMin = -1.0f, dOutZMax = -1.0f;
	DOUBLE dFalseZ = -1.0f, dZUnits = -1.0f;
	//DOUBLE dMeatersPerUnit = -1.0f;
	VARIANT_BOOL vbHasMPrecision = VARIANT_FALSE, vbHasXYPrecision = VARIANT_FALSE, vbHasZPrecision = VARIANT_FALSE;

	ipSpRef->get_Abbreviation( &bstrAbbreviation );
	ipSpRef->get_Alias( &bstrAlias );
	ipSpRef->get_FactoryCode( &lFactoryCode );
	ipSpRef->get_Name( &bstrName );
	ipSpRef->get_Remarks( &bstrRemarks );
	ipSpRef->GetDomain( &dXMin, &dXMax, &dYMin, &dYMax );
	ipSpRef->GetFalseOriginAndUnits( &dFalseX, &dFalseY, &dXYUnits );
	ipSpRef->GetMDomain( &dOutMMin, &dOutMMax );
	ipSpRef->GetMFalseOriginAndUnits( &dFalseM, &dMUnits );
	ipSpRef->GetZDomain( &dOutZMin, &dOutZMax );
	ipSpRef->GetZFalseOriginAndUnits( &dFalseZ, &dZUnits );
	ipSpRef->HasMPrecision( &vbHasMPrecision );
	ipSpRef->HasXYPrecision( &vbHasXYPrecision );
	ipSpRef->HasZPrecision( &vbHasZPrecision );
	ATLTRACE(_T("%s Abbreviation:%s Alias:%s Name:%s Remarks:%s\n"), lpcszHeader, OLE2T(bstrAbbreviation), OLE2T(bstrAlias), OLE2T(bstrName), OLE2T(bstrRemarks) );
	ATLTRACE(_T("%s DOMAIN （%s） X=%lf-%lf Y=%lf-%lf FALSEORIGIN:X=%lf Y=%lf FALSEUNITS=%lf\n"), lpcszHeader, (vbHasXYPrecision==VARIANT_TRUE ? _T("有") : _T("無") ), dXMin, dXMax, dYMin, dYMax, dFalseX, dFalseY, dXYUnits );
	ATLTRACE(_T("%s MDOMAIN（%s） M=%lf-%lf MFALSEORIGIN=%lf MFALSEUNITS=%lf\n"), lpcszHeader, (vbHasMPrecision==VARIANT_TRUE ? _T("有") : _T("無") ), dOutMMin, dOutMMax, dFalseM, dMUnits );
	ATLTRACE(_T("%s ZDOMAIN（%s） Z=%lf-%lf ZFALSEORIGIN=%lf ZFALSEUNITS=%lf\n"), lpcszHeader, (vbHasZPrecision==VARIANT_TRUE ? _T("有") : _T("無") ), dOutZMin, dOutZMax, dFalseZ, dZUnits );
}
#endif // ifdef _DEBUG

bool AheIsHighPrecision(ISpatialReferencePtr ipSpRef)
{
	if ( ! ipSpRef )
		return false;

	IControlPrecision2Ptr ipControlPrecision( ipSpRef );
	bool bHighPrecision = false;

	if ( ipControlPrecision )
	{
		VARIANT_BOOL vbIsHighPrecision = VARIANT_FALSE;

		ipControlPrecision->get_IsHighPrecision( &vbIsHighPrecision );
		bHighPrecision = vbIsHighPrecision ? true : false;
	}

	return bHighPrecision;
}

// 設定ファイルでハイライト色を設定できるようにする(bug9404)
bool GetHighlightColor( const _bstr_t& strXPath, int& nRed, int& nGreen, int& nBlue )
{
	// レジストリ読み込み
	CRegKey aRegKey;
	TCHAR cPath[1024] = {0};
	if(aRegKey.Open(HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON")) == ERROR_SUCCESS)
	{
		ULONG uLen = 1024;
		if(aRegKey.QueryStringValue(_T("HighlightSettingPath"), cPath, &uLen) == ERROR_SUCCESS)
		{
			if( 0 > lstrlen( cPath ) )
				return false;
		}
		else
			return false;
	}

	// 設定ファイル読み込み
	MSXML2::IXMLDOMDocumentPtr pDoc;
	pDoc.CreateInstance(CLSID_DOMDocument);
	pDoc->put_async(VARIANT_FALSE);
	VARIANT_BOOL vbIsLoad =VARIANT_FALSE;
	vbIsLoad = pDoc->load( CComVariant( cPath ) );
	if( !vbIsLoad )
		return false;

	// ルートノードを取得
	MSXML2::IXMLDOMElementPtr pRootNode;
	pDoc->get_documentElement( &pRootNode );
	if( !pRootNode )
		return false;

	// ハイライト色設定があるノードを取得
	MSXML2::IXMLDOMNodePtr pNode;
	pNode = pDoc->selectSingleNode( strXPath );
	if( !pNode )
		return false;

	// エレメントにして属性(設定)値を取得
	MSXML2::IXMLDOMElementPtr pXMLElement;
	pXMLElement = pNode;
	if( !pXMLElement )
		return false;

	CComVariant vaRed, vaGreen, vaBlue;
	vaRed = pXMLElement->getAttribute( _T("red") );
	vaGreen = pXMLElement->getAttribute( _T("green") );
	vaBlue = pXMLElement->getAttribute( _T("blue") );

	if( vaRed.vt == VT_NULL || vaGreen.vt == VT_NULL || vaBlue.vt == VT_NULL )
		return false;

	nRed = _wtol(vaRed.bstrVal);
	nGreen = _wtol(vaGreen.bstrVal);
	nBlue = _wtol(vaBlue.bstrVal);

	return true;
}
