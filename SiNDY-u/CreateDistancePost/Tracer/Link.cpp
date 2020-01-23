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

// Link.cpp: Link クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <sindy/schema/road.h>
#include "Link.h"

using namespace sindy::schema;

namespace sindy
{

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Link::Link() :
m_nObjectId(0),
m_nFromNodeId(0),
m_nToNodeId(0),
m_nRoadClass(0),
m_nNaviClass(0),
m_nRoadWidth(0),
m_nLinkClass(0)
{

}

Link::Link(const IFeaturePtr& ipFeature) :
m_nObjectId(0),
m_nFromNodeId(0),
m_nToNodeId(0),
m_nRoadClass(0),
m_nNaviClass(0),
m_nRoadWidth(0),
m_nLinkClass(0)
{
	setFeature(ipFeature);
}

Link::~Link()
{

}

//////////////////////////////////////////////////////////////////////
// 設定
//////////////////////////////////////////////////////////////////////

void Link::setFeature(const IFeaturePtr& ipFeature)
{
	using namespace road_link;

	ATLASSERT(ipFeature);
	CComVariant va;

	// オブジェクトID取得
	ipFeature->get_OID(&m_nObjectId);

	// FromノードID取得
	va = GetAttribute(ipFeature, kFromNodeID);
	va.ChangeType(VT_I4);
	m_nFromNodeId = va.lVal;

	// ToノードID取得
	va = GetAttribute(ipFeature, kToNodeID);
	va.ChangeType(VT_I4);
	m_nToNodeId = va.lVal;

	// 道路種別取得
	va = GetAttribute(ipFeature, kRoadClass);
	va.ChangeType(VT_I4);
	m_nRoadClass = va.lVal;

	// 経路種別取得
	va = GetAttribute(ipFeature, kNaviClass);
	va.ChangeType(VT_I4);
	m_nNaviClass = va.lVal;

	// 道路復員区分取得
	va = GetAttribute(ipFeature, kRoadWidth);
	va.ChangeType(VT_I4);
	m_nRoadWidth = va.lVal;

	// リンク種別取得
	va = GetAttribute(ipFeature, kMainLinkClass);
	va.ChangeType(VT_I4);
	m_nLinkClass = va.lVal;

	// 路線番号取得
	va = GetAttribute(ipFeature, kRoadNo);
	va.ChangeType(VT_I4);
	m_nRoadNo= va.lVal;

	// 路線コード取得
	va = GetAttribute(ipFeature, kRoadCode);
	va.ChangeType(VT_I4);
	m_nRoadCode = va.lVal;

	// 形状取得
	ipFeature->get_ShapeCopy(&m_ipGeometry);
}

CComVariant Link::GetAttribute( const _IRowPtr& ipRow, long lIndex )
{
	CComVariant vaValue;

	// 引数チェック
	if( ! ( ipRow && 0 <= lIndex ) )
	{
		//_ASSERTE( ipRow && 0 <= lIndex ); うざい
		return vaValue;
	}

	if( FAILED( ipRow->get_Value( lIndex, &vaValue ) ) )
		_ASSERTE( SUCCEEDED( ipRow->get_Value( lIndex, &vaValue ) ) );

	return vaValue;
}

CComVariant Link::GetAttribute( const _IRowPtr& ipRow, LPCTSTR lpcszFieldName )
{
	// 引数チェック
	if( ! lpcszFieldName )
	{
		_ASSERTE( lpcszFieldName );
		return CComVariant();
	}

	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );

	return GetAttribute( ipRow, FindField( ipFields, lpcszFieldName ) );
}

// フィールド名でフィールドインデックスを取得する
long Link::FindField( const IFieldsPtr& ipFields, LPCTSTR lpcszFieldName )
{
	if( ! ( ipFields && lpcszFieldName ) )
	{
		_ASSERTE( ipFields && lpcszFieldName );
		return -1;
	}

	// FindFieldだと大文字小文字を考慮する必要が出てくるため、
	// 無駄だがループをまわすことで回避
	long lFieldCount = -1;
	ipFields->get_FieldCount( &lFieldCount );
	for( long i = 0; i < lFieldCount; ++i )
	{
		IFieldPtr ipField;
		CComBSTR bstrName;

		ipFields->get_Field( i, &ipField );
		ipField->get_Name( &bstrName );

		// テーブル結合にも対応する
		CString strFieldName( bstrName );
		INT iPos = strFieldName.ReverseFind('.');
		if( iPos > 0 ) 
			strFieldName = strFieldName.Right( strFieldName.GetLength() - iPos - 1 );

		if( 0 == lstrcmpi( lpcszFieldName, strFieldName ) )
			return i;
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////
// 取得処理
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 判定処理
//////////////////////////////////////////////////////////////////////

}
