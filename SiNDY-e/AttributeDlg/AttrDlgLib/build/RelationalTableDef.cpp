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

/*! \file RelationalTableDef.cpp
	\brief CRelationalTableDef クラスのインプリメンテーション
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2002-03-05
*/
#include "stdafx.h"
#include "../RelationalTableDef.h"
/**
 * コンストラクタ
 */
CRelationalTableDef::CRelationalTableDef() : m_bChanged(false)
{
	Clear();
}
/**
 * デストラクタ
 */
CRelationalTableDef::~CRelationalTableDef()
{
	Clear();
}
/**
 * メンバを初期状態に戻す
 */
void CRelationalTableDef::Clear()
{
	for( iterator it = begin(); it != end(); it++ ) 
		it->Clear();

	clear();

	m_ipTable = NULL;
}
/**
 * フィールド変更前に戻す
 */
void CRelationalTableDef::Reset()
{
	iterator it = begin();
	while( it != end() )	
	{
		// CreateFlag が立っているものは削除
		if( it->GetCreateFlag() ) {
			it = erase( it );
		} else {
			it->Reset();
			it++;
		}
	}
}
/**
 * 関連テーブル定義を作成する
 *
 * @param ipTable			[in]			定義するITableオブジェクト
 * @param lpcszTableName	[in]			定義するテーブル名
 * @patam lOID				[in]			関連する外部キー（フィーチャのオブジェクトID）
 * @param lpcszOuterKey		[in]			外部キーフィールド名
 * @param bForce			[in,optional]	TRUE の場合、空の仮想レコードを一つだけ強制的に作成します（CControlRel::ControlChanged()で使用するため）
 * @param bNoSearch			[in,optional]	TRUE の場合、検索を実行しません（CLQAttributeDlgObj::SetLQs() で使用するため）
 */
void CRelationalTableDef::SetTable( ITablePtr ipTable, LPCTSTR lpcszTableName, LONG lOID, LPCTSTR lpcszOuterKey, BOOL bForce/* = FALSE */, BOOL bNoSearch/* = FALSE*/ )
{
	ATLASSERT( ipTable != NULL );

	m_ipTable = ipTable;
	m_strTableName = lpcszTableName;
	m_lOID = lOID;

	ATLASSERT( lstrcmpi( lpcszOuterKey, _T("") ) != 0 );

	SetOuterKey( lpcszOuterKey, bForce, bNoSearch );
}
/**
 * 外部キーを元に、テーブルから検索してレコードを作成する
 *
 *   SetTable() した後に呼ばれます
 * 
 * @param lpcszOuterKey [in]			外部キー文字列
 * @param bForce		[in]			TRUE の場合、空の仮想レコードを一つだけ強制的に作成します（CControlRel::ControlChanged()で使用するため）
 * @param bNoSearch		[in,optional]	TRUE の場合、検索を実行しません（CLQAttributeDlgObj::SetLQs() で使用するため）
 *
 * @return 検索が正常に終了したら TRUE、検索が正常に終了しないか、テーブル自体が存在しない場合は FALSE
 */
BOOL CRelationalTableDef::SetOuterKey( LPCTSTR lpcszOuterKey, BOOL bForce, BOOL bNoSearch/* = FALSE*/ )
{
	ATLASSERT( lpcszOuterKey != NULL );

	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	_ICursorPtr ipCursor;
	_IRowPtr ipRow;
	IFieldsPtr ipFields;
	IFieldPtr ipField;

	LONG lFieldCount = -1;
	CString strSubFields, strWhereClause;
	
	m_strOuterKey = lpcszOuterKey;
	
	// フィールドを登録
	if( !m_ipTable ) {
		ATLTRACE(_T("CRelationalTableDef::SetOuterKey() : テーブルがセットされていません\n") );
		ATLASSERT(FALSE);
		return FALSE;
	}

	// 検索しない場合はこっち
	if( bNoSearch )
		return TRUE;

	USES_CONVERSION;
	
	// 強制フラグが立っていたら空の仮想レコードを作成
	if( bForce ) {
		return CreateNewRow();
	}

	// 強制フラグが立ってない場合はこっち
	strWhereClause.Format(_T("%s = %d"), m_strOuterKey, m_lOID );
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	m_ipTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	if( !ipCursor ) 
		return FALSE;

	BSTR bstrFieldName;
	while( ipCursor->NextRow( &ipRow ) == S_OK ) {
		if( !ipRow )
			break;

		CRowDef rowDef;
		
		rowDef.SetRow( ipRow );
		rowDef.SetTable( m_ipTable );

		ipRow->get_Fields( &ipFields );
		ipFields->get_FieldCount( &lFieldCount );
		for( LONG i = 0; i < lFieldCount; i++ ) {
			ipFields->get_Field( i, &ipField );
			ipField->get_Name( &bstrFieldName );
			CString strFieldName = bstrFieldName;
			::SysFreeString(bstrFieldName);

			CFieldDef fieldDef;
			fieldDef.SetArcHelper( m_cArcHelper.GetApp() );
			fieldDef.SetFieldDef( m_strTableName, strFieldName );
			fieldDef.SetRowBuffer( i, ipField, ipRow );
			rowDef.push_back( fieldDef );
		}
		push_back( rowDef );
	}

	return TRUE;
}
/**
 * 削除フラグまたは新規作成フラグを立てる
 *
 * @param lIndex	[in]	削除フラグまたは新規作成フラグを立てるレコードのインデックス番号
 * @param bDelete	[in]	削除する場合は TRUE、新規作成する場合は FALSE
 */
void CRelationalTableDef::SetDeleteFlag( LONG lIndex, BOOL bDelete/* = TRUE */ )
{
	iterator it = begin();
	advance( it, lIndex );
	it->SetDeleteFlag( bDelete );
}
/**
 * 編集をデータベースに適用する
 */
void CRelationalTableDef::Store()
{
	for( iterator it = begin(); it != end(); it++ )
		it->Store( TRUE	);	// 外部テーブルの更新履歴に関しては必ず入れる必要がある
}
/**
 * 登録されているレコードが存在するかどうかをチェックする
 *
 * 削除されたレコードに関してはカウント対象外です。
 *
 * @return 一つもレコードが存在しない場合は TRUE、そうでない場合は FALSE
 */
BOOL CRelationalTableDef::IsNoRows()
{
	LONG lCount = 0;
	for( iterator it = begin(); it != end(); it++ )
	{
		if( it->IsDeleted() )
			continue;
		else
			lCount++;
	}

	if( lCount > 0 ) 
		return FALSE;
	else 
		return TRUE;
}
/**
 * 新しいレコードを追加する
 */
BOOL CRelationalTableDef::CreateNewRow()
{
	CRowDef rowDef;
//  MOD 02.10.01 by hkakeno
	//CComVariant vaDefaultValue;
	BSTR bstrFieldName;
	IFieldsPtr ipFields;
	IFieldPtr ipField;
	LONG lFieldCount = -1;
	VARIANT_BOOL vaIsNullable = VARIANT_FALSE;

	USES_CONVERSION;

	rowDef.SetTable( m_ipTable );
	rowDef.SetCreateFlag();	// 新規作成フラグをつける

	m_ipTable->get_Fields( &ipFields );
	ipFields->get_FieldCount( &lFieldCount );
	for( LONG i = 0; i < lFieldCount; i++ ) {
		ipFields->get_Field( i, &ipField );

		ipField->get_Name( &bstrFieldName );
		CString strFieldName = bstrFieldName;
		::SysFreeString( bstrFieldName );

		CComVariant vaDefaultValue;
		ipField->get_DefaultValue( &vaDefaultValue );
		ipField->get_IsNullable( &vaIsNullable );

		CFieldDef fieldDef;
		fieldDef.SetArcHelper( m_cArcHelper.GetApp() );
		fieldDef.SetFieldDef( m_strTableName, strFieldName );
		fieldDef.m_ipField = ipField;												// フィールドセット
		fieldDef.m_lFieldIndex = i;													// フィールドインデックス番号
		
		LONG lVarType;
		ipField->get_VarType( &lVarType );
		if ((  vaDefaultValue.vt != VT_NULL || vaDefaultValue.vt != VT_EMPTY) 
			&& vaDefaultValue.vt != (unsigned short)lVarType)	
			vaDefaultValue.ChangeType((unsigned short)lVarType);
//} MOD 02.10.02 by hkakeno 		

		// 外部キー保持フィールドならフィーチャのOIDを入れ、そうでなければデフォルト値をセット
		fieldDef.m_vaValue = ( 0 == fieldDef.m_strFieldName.CompareNoCase( GetOuterKey() ) ) ? GetOID() : vaDefaultValue;

		// 変更されていれば、変更フラグをセット
		if( fieldDef.m_vaInitValue != fieldDef.m_vaValue )							
			fieldDef.m_bChanged = TRUE;

		// NULL OK かどうかのフラグ
		fieldDef.m_bIsNullable = ( vaIsNullable == VARIANT_TRUE ) ? TRUE : FALSE;	

		// 初期化完了フラグ
		fieldDef.m_bIsInited = TRUE;

		rowDef.push_back( fieldDef );
	}
	push_back( rowDef );

	return TRUE;
}
