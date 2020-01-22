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
#include "RelationTableManager.h"

#include <TDC/useful_headers/str_util.h>
#include <TDC/sindylib_base/TableContainer.h>

void CRelationTableManager::Initialize( const CString& strTable, const CString& strKey, const FIELDNAMES& subFields, _IRowPtr& ipRow )
{
	m_KeyFieldName = strKey;
	m_subFields = subFields;

	// 選択されているフィーチャーを取得
	if( !ipRow )
		return;

	// フィーチャーからテーブルを取得
	ITablePtr ipCurrentTable;
	ipRow->get_Table(&ipCurrentTable);
	if( !ipCurrentTable )
		return;

	m_ipTable = AheOpenSameOwnerTable( ipCurrentTable, strTable );
	if( !m_ipTable )
		return ;

	// フィールドマップの作成
	IFieldsPtr ipFields;
	m_ipTable->get_Fields( &ipFields );
	m_RelTableFM.CreateFieldMap( ipFields );
	return;
}

// 関連テーブルにデータを追加する処理
bool CRelationTableManager::AddData( long lOID, const AOI_MATRIX& lList ) const
{
	if( lList.empty() ) return true;

	_ICursorPtr ipCursor;
	if( FAILED( m_ipTable->Insert( VARIANT_TRUE, &ipCursor ) ) )
		return false;
	IRowBufferPtr ipRowBuffer;
	if( FAILED( m_ipTable->CreateRowBuffer( &ipRowBuffer ) ) )
		return false;

	for( auto& value : lList )
	{
		if( FAILED(ipRowBuffer->put_Value( m_RelTableFM.FindField( m_KeyFieldName ), CComVariant(lOID)) ) ||
		   !putValues(ipRowBuffer, value) )
			return false;

		CComVariant newRowData;
		if( FAILED( ipCursor->InsertRow(ipRowBuffer, &newRowData) ) )
			return false;
	}
	if( FAILED(ipCursor->Flush()) )
		return false;
	return true;
}

// 関連テーブルにデータを追加する処理
bool CRelationTableManager::AddData( long lOID, const AOI_LINE& lList ) const
{
	AOI_MATRIX tempList;
	for( auto& elem : lList )
	{
		AOI_LINE addList;
		addList.push_back( elem );
		tempList.push_back(addList);
	}
	return AddData( lOID, tempList );
}

// 関連テーブルからデータを削除する処理
bool CRelationTableManager::DeleteData( long lOID, const DELETELIST& lList ) const
{
	if( lList.empty() ) return true;

	DELETELISTS targetValues;
	// DeleteDataで使用できるように整形
	for( auto& elem : lList )
	{
		std::list<CString> addList;
		addList.push_back( elem );
		targetValues.push_back( addList );
	}

	return DeleteData( lOID, targetValues );
}

// 関連テーブルからデータを削除する処理
bool CRelationTableManager::DeleteData( long lOID, const DELETELISTS& lList ) const
{
	if( lList.empty() ) return true;

	/// 検索条件に渡す文字列
	CString strWhere = makeDeleteWhereClause( lOID, lList );

	// 条件に当て嵌まる行を削除
	if( FAILED(m_ipTable->DeleteSearchedRows( AheInitQueryFilter( nullptr, nullptr, strWhere ) )) )
		return false;

	return true;
}

// 関連テーブルからキーフィールドとOIDが一致する行の一要素を取得
AOI_COLUMN CRelationTableManager::GetRelationData( long lOID, const CString& strField ) const
{
	AOI_COLUMN retList;
	// (最終的には返り値として返すが)GetRelationDataに引き渡すために、要素にstrFieldを追加
	retList.push_back( strField );

	// m_KeyFieldNameがOIDである行のstrFieldの値を取得
	auto tempList = GetRelationData( lOID, retList );

	// 返り値として返すために整形
	// AOI_MATRIXをAOI_COLUMNに直す
	retList.clear();
	for( auto& columns : tempList )
	{
		// 要素数はどれも1なので、1つだけ取り出す
		retList.push_back( *columns.begin() );
	}
	return retList;
}

// 関連テーブルからOIDに関連する行のJOIN_LINE_INFO_IDを全て取得
AOI_MATRIX CRelationTableManager::GetRelationData( long lOID, const FIELDNAMES& strFields ) const
{
	AOI_MATRIX retList;

	_ICursorPtr ipCursor;
	CString strSubFields, strWhereClause;

	// サブフィールドを設定
	strSubFields.Format( _T("%s"), m_KeyFieldName );
	for( auto& elem : strFields )
		strSubFields.AppendFormat( _T(",%s"), elem );

	// 条件を設定
	strWhereClause.Format( _T("%s=%ld"), m_KeyFieldName, lOID );
	if( FAILED(m_ipTable->Search( AheInitQueryFilter( nullptr, strSubFields, strWhereClause ), VARIANT_TRUE, &ipCursor)) )
		return retList;

	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow)==S_OK && ipRow )
	{
		AOI_LINE addList;
		for( auto& elem : strFields )
		{
			CComVariant value = AheGetAttribute( ipRow, m_RelTableFM.FindField( elem ) );
			if( value.vt!=VT_INT && value.vt!=VT_I4 && value.vt!=VT_I2 )
				continue;
			addList.push_back( uh::str_util::ToString(value) );
		}
		retList.push_back( addList );
	}
	return retList;
}

// IRowBufferPtrに値を入れる
bool CRelationTableManager::putValues( IRowBufferPtr& ipRowBuffer, const AOI_LINE& columns ) const
{
	// 挿入するデータとデータ挿入先のサブフィールドの数が一致しない場合は、失敗とする
	if( m_subFields.size() != columns.size() )
		return false;

	auto it = columns.begin();
	for( auto& elem : m_subFields )
	{
		if( FAILED(ipRowBuffer->put_Value( m_RelTableFM.FindField(elem), CComVariant(*it))) )
		{
			return false;
		}
		++it;
	}
	return true;
}

//  DB上のデータを削除する際の条件式を生成
CString CRelationTableManager::makeDeleteWhereClause( long lOID, const DELETELISTS& lList ) const
{
	CString strWhereClause;
	strWhereClause.Format( _T("%s=%ld"), m_KeyFieldName, lOID );

	int index = 0;
	for( auto& strField : m_subFields )
	{
		//"%s in ({この部分})"を作成するためのリストを作成
		std::list<CString> targetValues;
		for( auto& elem : lList )
		{
			auto it = elem.begin();
			std::advance( it, index );
			if( it!=elem.end() )
				targetValues.push_back(*it);
		}
		// 空=引数の指定の仕方がおかしいので、条件文には足さない
		if( targetValues.empty() )
			continue;

		// 以下、"in (...)"の部分の整形
		strWhereClause.AppendFormat(_T(" and %s in ("), strField);

		// INで使用できる文字列を生成
		std::list<CString> inList;
		sindy::CTableBase::IDs2Str( targetValues, inList );

		// 千個以上要素はないものとして処理する
		// TODO:1000個以上の要素を処理する可能性がある場合は、全体を見直して改修のこと
		for( const auto& elem : inList )
		{
			strWhereClause.AppendFormat( _T("%s"), elem );
		}

		// 最後のかっこを追加 最終的に %s=%ld and %s IN ('%ld','%ld') のようになるはず。
		strWhereClause.Append(_T(")"));

		++index;
	}
	return strWhereClause;
}
