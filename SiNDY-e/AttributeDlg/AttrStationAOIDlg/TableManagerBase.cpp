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
#include "TableManagerBase.h"

#include <TDC/useful_headers/str_util.h>
#include <boost/assign/list_of.hpp>
#include <sindy/schema.h>

using namespace boost::assign;
using namespace sindy::schema;

void CTableManagerBase::Join(const CString& strMain, const CString& strSub, const CString& strMainKey, const CString& strSubKey)
{
	//TODO:安全のために環状が発生するようなら追加しないという処理がいる
	m_tableSearchedKey[strSub] = strSubKey;
	m_joinMap[strMain] = std::make_pair(strSub,strMainKey);

	auto checkList = list_of (strMain) (strSub);
	// 指定テーブルのフィールドマップが作成されているか判定し、作成されていないなら作成する
	for( auto& elem : checkList )
	{
		if( m_FieldMaps.find(elem)!=m_FieldMaps.end() )
			continue;
		//新出ならフィールドマップ生成
		sindy::CFieldMap fm;
		createFieldMap( elem, fm);
		m_FieldMaps[elem] = fm;
	}
}

AOI_MATRIX CTableManagerBase::GetValue( const CString& key, const CString& fieldName, const CString& strTable )
{
	ITablePtr ipTable = AheOpenSameOwnerTable( m_ipTable, strTable );
	if( !ipTable )
		return AOI_MATRIX();

	CString strWhere, strSubField;
	strWhere.Format(_T("%s=%s"), fieldName, key);
	setStrSubFields( strSubField, strTable, fieldName );

	_ICursorPtr ipCursor;
	if( FAILED(ipTable->Search( AheInitQueryFilter( nullptr, strSubField, strWhere ), VARIANT_TRUE, &ipCursor)) )
	{
		return AOI_MATRIX();
	}

	return getValue(ipCursor, strTable);
}

void CTableManagerBase::SetReturnData( const std::list<std::pair<CString,CString>>& dataList )
{
	m_dataList = dataList;
	// サブフィールドも設定
	m_subFields.clear();
	for( const auto& elem : m_dataList )
	{
		m_subFields[elem.first].push_back( elem.second );
	}
}

AOI_MATRIX CTableManagerBase::getValue(  _ICursorPtr& ipCursor, const CString& strTable )
{
	CSearchList tempList;
	loadValue( ipCursor, strTable, tempList);
	AOI_MATRIX reList;

	// 子が一つもなく、自身の要素もないなら、処理はそこで止める
	if( tempList.m_childList.empty() && tempList.m_attrList.empty() )
		return reList;

	searchList2ListList( tempList, reList );

	// リストの長さがm_dataListと一致しない==関連データが揃っていないデータなので、消す
	for( auto it = reList.begin(); it!=reList.end(); ++it )
	{
		if(it->size()!=m_dataList.size())
			it = reList.erase(it);
	}

	return reList;
}

void CTableManagerBase::loadValue( _ICursorPtr& ipCursor, const CString& strTable, CSearchList& parent )
{
	/// 次に検索するテーブルが存在するか否かのフラグ
	bool isNextExist = m_joinMap.find( strTable ) != m_joinMap.end();
	/// 検索結果となるデータが存在したか否かのフラグ
	bool isExistData = false;

	std::list<CString> keyValues;
	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow)==S_OK && ipRow )
	{
		// データを一度でも読み取ったなら、フラグを変更
		isExistData = true;

		parent.m_childList.push_back(CSearchList());
		// その行の属性取得
		getRowValue( ipRow, strTable, (*parent.m_childList.rbegin()).m_attrList );

		if( !isNextExist )
			continue;

		//次のテーブルを検索するのに必要なデータの取得
		CComVariant value = AheGetAttribute( ipRow, m_FieldMaps[strTable].FindField( m_joinMap[strTable].second ) );
		// データ取得に失敗したものは除く
		if( value.vt == VT_EMPTY )
			continue;
		//次を検索する際のキーとなる値を取得
		CString strValue = uh::str_util::ToString( value );
		keyValues.push_back(strValue);
	}
	if( !isNextExist || !isExistData )
		return ;

	const CString& strNextTable = m_joinMap[strTable].first;
	const CString& strKeyField = m_tableSearchedKey[strNextTable];

	ITablePtr ipTable = AheOpenSameOwnerTable( m_ipTable, strNextTable );
	if( !ipTable )
		return ;

	auto it = parent.m_childList.begin();
	for( auto& key : keyValues )
	{
		CString strWhere, strSubField;
		strWhere.Format(_T("%s=%s"), strKeyField, key);
		setStrSubFields( strSubField, strNextTable );

		_ICursorPtr ipRelCursor;
		if( FAILED(ipTable->Search( AheInitQueryFilter( nullptr, strSubField, strWhere ), VARIANT_TRUE, &ipRelCursor)) )
		{
			return ;
		}
		loadValue(ipRelCursor, strNextTable, *it );
		++it;
	}
}

void CTableManagerBase::getRowValue( _IRowPtr& ipRow, const CString& strTable, AOI_LINE& addList )
{
	FIELDNAMES& list = m_subFields[strTable];
	for( auto& elem : list )
	{
		CComVariant value = AheGetAttribute( ipRow, m_FieldMaps[strTable].FindField(elem) );
		if( value.vt != VT_EMPTY )
			addList.push_back( uh::str_util::ToString(value) );
	}
}

// サブテーブルのフィールドをセット
void CTableManagerBase::setStrSubFields( CString& strSubField, const CString& strTable, const CString& keyField )
{
	// 条件文生成用のset(重複を消すために使用)
	std::set<CString> whereSet;

	// 今のテーブルを検索される際に必要となるフィールドがある場合は、それを読み出す
	if( !m_tableSearchedKey[strTable].IsEmpty() )
		whereSet.insert(m_tableSearchedKey[strTable]);

	// 次のテーブルを検索する際に必要となるフィールドがある場合は、それを読み出す
	if( !m_joinMap[strTable].second.IsEmpty() )
		whereSet.insert(m_joinMap[strTable].second);

	// 検索する際のキーとなるフィールドを設定
	if( !keyField.IsEmpty() )
		whereSet.insert(keyField);

	// その他サブフィールドを設定
	for( auto& elem : m_subFields[strTable] )
		whereSet.insert( elem );

	// （ないはずだが）条件となるものがないなら処理はそこで終える
	if( whereSet.empty() )
		return;

	auto it = whereSet.begin();

	// 始めの一つだけ特別に処理
	strSubField.Format( _T("%s"), *it );

	for( ; it!= whereSet.end(); ++it )
	{
		strSubField.AppendFormat( _T(",%s"), *it );
	}
}

// SearchListをAOI_MATRIXの形式に変換(再帰)
void CTableManagerBase::searchList2ListList( CSearchList& ops, AOI_MATRIX& lList ) const 
{
	if( ops.m_childList.empty() )
	{
		lList.push_back( ops.m_attrList );
	}

	bool firstFlag = true;
	for( auto& child : ops.m_childList )
	{
		AOI_MATRIX childDataList;
		searchList2ListList( child, childDataList );
		firstFlag = false;
		for( auto& elem : childDataList )
		{
			lList.push_back( ops.m_attrList );
			//.merge呼ぶと順番入れ替わる... mergeは順番維持してくれない？
			//.spliceで末尾に連結しても駄目...仕方ないので一個一個足す
			for( auto& elem2 : elem )
				lList.rbegin()->push_back( elem2 );
		}
	}
}

// フィールドマップを作成
bool CTableManagerBase::createFieldMap( const CString& strTable, sindy::CFieldMap& fieldMap )
{
	ITablePtr ipTable =  AheOpenSameOwnerTable( m_ipTable, strTable );
	if( !ipTable )
		return false;

	IFieldsPtr ipFields;
	ipTable->get_Fields( &ipFields );
	if( !ipFields )
		return false;

	fieldMap.CreateFieldMap( ipFields );
	return true;
}
