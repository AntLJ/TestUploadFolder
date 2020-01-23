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

// DBConnect.cpp: CDBConnect クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBConnect.h"

#include "..\..\LIB\WinLib\CStringTokenizer.h"

/**
 * @class	CDBConnect
 */

// フィーチャクラス取得
IFeatureClassPtr CDBConnect::GetFeatureClassByName( LPCTSTR lpszName )
{
	IFeatureClassPtr ipFeatureClass;

	HRESULT hr = ((IFeatureWorkspacePtr)m_ipWorkspace)->OpenFeatureClass( CComBSTR(lpszName), &ipFeatureClass );

	return ( SUCCEEDED(hr)? ipFeatureClass : NULL );
}

// 参照テーブル取得
ITablePtr CDBConnect::GetTableByName( LPCTSTR lpszName )
{
	ITablePtr ipTable;

	HRESULT hr = ((IFeatureWorkspacePtr)m_ipWorkspace)->OpenTable(CComBSTR(lpszName), &ipTable);

	return ( SUCCEEDED(hr)? ipTable : NULL );
}

// ドメインマップ取得関数
DOMAINMAP CDBConnect::GetDomainMap( LPCTSTR lpszTableName, LPCTSTR lpszFieldName )
{
	int i;
	DOMAINMAP map;

	// テーブル取得
	ITablePtr ipTable;
	ipTable = GetTableByName( lpszTableName );
	if( NULL == ipTable )
		return map;

	// フィールド名からフィールドID取得
	LONG lFieldID;
	if( FAILED(ipTable->FindField( CComBSTR(lpszFieldName), &lFieldID )) )
		return map;

	// とりあえず何かオブジェクトを取得して、ドメインを取得する
	_IRowPtr ipRow;
	for( i=1;i<INT_MAX;i++ )
	{
		// 取得できたらループ抜ける
		ipTable->GetRow( i, &ipRow );
		if( ipRow != NULL )
			break;
	}

	if( NULL == ipRow )
		return map;

	// IField取得する
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );
	IFieldPtr ipField;
	ipFields->get_Field( lFieldID, &ipField );

	// IDomain取得して、mapを生成
	IDomainPtr ipDomain;
	if( FAILED(ipField->get_Domain( &ipDomain )) )
		return map;

	ICodedValueDomainPtr ipCVDomain(ipDomain);
	if( NULL == ipCVDomain )
		return map;

	LONG lDomainCount;
	ipCVDomain->get_CodeCount( &lDomainCount );

	USES_CONVERSION;
	for( i=0;i<lDomainCount;i++)
	{
		CComVariant vaValue;
		BSTR bstr;
		ipCVDomain->get_Value( i, &vaValue );
		ipCVDomain->get_Name( i, &bstr );


		map.insert( pair<LONG, std::string>(vaValue.lVal, OLE2A(bstr)) );
		::SysFreeString(bstr);
	}

	return map;
}

/**
 * @class	CSDEConnect
 */

// SDE接続
bool CSDEConnect::Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx /* = NULL */ )
{	
	CStringTokenizer cTokenizer( lpszDBProp, _T("/"), TOKEN_RET_EMPTY_ALL );

	if( 5 != cTokenizer.CountTokens() )
		return false;

	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_SdeWorkspaceFactory );
	IPropertySetPtr ipPropertySet( CLSID_PropertySet );

	m_vaUser     = cTokenizer.GetNextToken();
	m_vaPass     = cTokenizer.GetNextToken();
	m_vaVersion  = cTokenizer.GetNextToken();
	m_vaInstance = cTokenizer.GetNextToken();
	m_vaServer   = cTokenizer.GetNextToken();

	// プロパティセット
	USES_CONVERSION;
	ipPropertySet->SetProperty( T2BSTR(_T("USER")), m_vaUser );
	ipPropertySet->SetProperty( T2BSTR(_T("PASSWORD")), m_vaPass );
	ipPropertySet->SetProperty( T2BSTR(_T("INSTANCE")), m_vaInstance );
	ipPropertySet->SetProperty( T2BSTR(_T("SERVER")), m_vaServer );
	ipPropertySet->SetProperty( T2BSTR(_T("VERSION")), m_vaVersion );

	if( ipWorkspaceFactory->Open(ipPropertySet, NULL, &m_ipWorkspace) != S_OK )
		return false;

	return true;
}

/**
 * @class	CPGDBConnect
 */

// 接続
bool CPGDBConnect::Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEz /* = NULL */ )
{
	if( NULL == lpszDBProp )
		return false;

	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );

	if( FAILED(ipWorkspaceFactory->OpenFromFile(CComBSTR(lpszDBProp), 0, &m_ipWorkspace)) )
		return false;

	m_strFile = lpszDBProp;

	return true;
}

/**
 * @class	CShapeConnect
 */

// 接続
bool CShapeConnect::Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx /* = NULL */ )
{
	if( NULL == lpszDBProp )
		return false;

	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_ShapefileWorkspaceFactory );
	IPropertySetPtr ipPropertySet( CLSID_PropertySet );
	ipPropertySet->SetProperty( CComBSTR(_T("DATABASE")), CComVariant(lpszDBProp) );

	if( FAILED(ipWorkspaceFactory->Open(ipPropertySet, 0, &m_ipWorkspace)) )
		return false;

	m_strDir = lpszDBProp;

	return true;
}

//////////////////////////////////////////////////////////
//	共通関数
//////////////////////////////////////////////////////////

// PGDBモード?
bool isPGDBMode()
{
	return (_tgetenv(_T("PGDB_MODE")) != NULL)? true : false;
}

// 空間・属性検索
bool SelectByShape( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursor** ppFeatureCursor, esriSpatialRelEnum spatialRel, 
				    LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
{
	/// クエリフィルタ作成
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR bstrFieldName;	
	ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( ipGeo );
	ipFilter->put_SpatialRel( spatialRel );
	
	// Where句指定されていたら、セットする
	if( lpszWhereClause != NULL ){
		ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where句セット
		ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// 検索優先順位も指定する
	}

	// SubFields指定されていたら、セットする
	if( lpszSubFields != NULL )
		ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

	/// 検索
	ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ppFeatureCursor );
	return ( (ppFeatureCursor != NULL) ? true : false );
}

// 空間・属性検索(件数のみ版)
LONG SelectByShapeOnlyCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */ )
{
	/// クエリフィルタ作成
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	ipFilter->putref_Geometry( ipGeo );
	ipFilter->put_SpatialRel( spatialRel );
	
	// Where句指定されていたら、セットする
	if( lpszWhereClause != NULL ){
		ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where句セット
		ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// 検索優先順位も指定する
	}

	/// 検索結果が 0 ならリターン
	LONG lHitCount = 0;
	return (SUCCEEDED(ipFeatureClass->FeatureCount(ipFilter, &lHitCount)) ? lHitCount : 0 );
}


// 空間・属性検索(件数も取得版)
LONG SelectByShapeWithCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursor** ppFeatureCursor, 
					esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
{
	/// クエリフィルタ作成
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR bstrFieldName;	
	ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( ipGeo );
	ipFilter->put_SpatialRel( spatialRel );
	
	// Where句指定されていたら、セットする
	if( lpszWhereClause != NULL ){
		ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where句セット
		ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// 検索優先順位も指定する
	}

	// SubFields指定されていたら、セットする
	if( lpszSubFields != NULL )
		ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

	/// 検索結果が 0 ならリターン
	LONG lHitCount = 0;
	ipFeatureClass->FeatureCount( ipFilter, &lHitCount );
	if( lHitCount < 1 ) {
		return 0;
	}

	/// 検索
	ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ppFeatureCursor );
	if( *ppFeatureCursor == NULL )
		return 0;
			
	return lHitCount;
}

// 属性検索
bool SelectByAttr( ITablePtr ipTable, _ICursor** ppCursor, LPCSTR lpszWhereClause, LPCTSTR lpszSubFields /* = NULL */ )
{
	/// クエリフィルタ作成
	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	
	// Where句指定されていたら、セットする
	if( lpszWhereClause == NULL )
		return false;
	ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );

	// SubFields指定されていたら、セットする
	if( lpszSubFields != NULL )
		ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

	/// 検索
	return SUCCEEDED(ipTable->Search(ipFilter, VARIANT_FALSE, ppCursor));
}

// 属性検索(件数のみ)
LONG SelectByAttrOnlyCount( ITablePtr ipTable, LPCSTR lpszWhereClause )
{
	/// クエリフィルタ作成
	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	
	// Where句指定されていたら、セットする
	if( lpszWhereClause == NULL )
		return false;
	ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where句セット

	/// 検索
	LONG lCount = 0;
	return ( SUCCEEDED(ipTable->RowCount(ipFilter, &lCount)) ? lCount : 0 );
}

// 属性検索(件数も一緒に)
LONG SelectByAttrWithCount( ITablePtr ipTable, _ICursor** ppCursor, LPCSTR lpszWhereClause, LPCTSTR lpszSubFields /* = NULL */ )
{
	/// クエリフィルタ作成
	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	
	// Where句指定されていたら、セットする
	if( lpszWhereClause == NULL )
		return false;
	ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );

	// SubFields指定されていたら、セットする
	if( lpszSubFields != NULL )
		ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

	// 件数取得
	LONG lHitCount = 0;
	ipTable->RowCount( ipFilter, &lHitCount );
	if( lHitCount < 1 )
		return 0;

	/// 検索
	return ( SUCCEEDED(ipTable->Search(ipFilter, VARIANT_FALSE, ppCursor)) ? lHitCount : 0 );
}
