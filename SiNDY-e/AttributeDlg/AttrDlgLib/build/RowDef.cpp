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

/*! \file RowDef.cpp
	\brief CRowDef クラスのインプリメンテーション
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2002-03-05
*/
#include "stdafx.h"
#include "../RowDef.h"

#include "../FeatureDef.h"
/**
	コンストラクタ・デストラクタ
*/
/**
 * コンストラクタ
 */
CRowDef::CRowDef()
{
	Clear();
}
/**
 * デストラクタ
 */
CRowDef::~CRowDef()
{
	Clear();
}
/**
 * メンバ変数を全て初期状態に戻す
 */
void CRowDef::Clear()
{
	m_ipRow = NULL;
	m_ipTable = NULL;
	m_bDeleteFlag = FALSE;
	m_bCreateFlag = FALSE;
	m_bDeleted = FALSE;
	clear();
}
/**
 * メンバ変数をフィールド値の変更がかかる前に戻す
 */
void CRowDef::Reset()
{
	for( iterator i = begin(); i != end(); i++ ) {
		i->Reset();
	}
	/// 作成・削除フラグは初期化
	m_bDeleteFlag = FALSE;
	m_bCreateFlag = FALSE;
}
/**
 * フィールド値の変更をデータベースに書き込む
 *
 * 削除指定フラグ（m_bDeleteFlag）や新規作成フラグ（m_bCreateFlag）がたっているときは
 * それぞれテーブルから削除又はテーブルに新規作成します。
 *
 * @note 返り値はフィーチャのフィールドのみ判定するので、例えば外部テーブルに変更があったとしても
 * @note フィーチャのフィールドに変更がなければ FALSE が返ります。
 *
 * @param bDoOperation		[in]	StartEditOperation を自分でするか、呼び出し側のツールでするかどうか
 *
 * @return 変更があって、Store() した場合は TRUE 、Store() 出来なかった、あるいは変更がなかった場合は FALSE
 */
BOOL CRowDef::Store( BOOL bDoOperation )
{
	BOOL bStored = FALSE;

	if( m_bDeleteFlag && !m_bCreateFlag )
		Delete();
	else if( !m_bDeleteFlag && !m_bCreateFlag ) {
		for( iterator i = begin(); i != end(); i++ ) {
			if( i->Store() )
				bStored = TRUE;	// フィーチャのフィールドに変更があった
		}

		// m_ipRow が NULL の場合はテーブルではなくフィーチャなので、その場合は CFeature::Store() で履歴を書き込む
		// 新規作成の際にはbDoOperationがFALSEできてしまい、新規作成の際に、関連するリンク列を拾ってきて、
		// その関連するリンク列の属性を変更した場合に、履歴が入らなくなってしまうことがあるため、
		// 属性ダイアログでは変更があった場合には、必ず履歴をいれるようにする
		// ようにしたかったが、無理でした、、新規作成できた場合には新規作成にしなければならないため、属性変更とは区別がつかない
		if( bStored && bDoOperation && m_ipRow )
		{
			if( !AheSetModifyData( m_ipRow, sindyUpdateTypeProperty ) )
				ATLASSERT( FALSE );	// iPC Feature 以外を編集するとこうなる？
			m_ipRow->Store();	// 外部テーブルはここで Store() する
		}
	} else if( !m_bDeleteFlag && m_bCreateFlag ) {
		Create();
	}
	// リレーションフラグ ON -> フィールドを削除した場合は、下記の条件になるため特別処理
	else if( m_bDeleteFlag && m_bCreateFlag && !m_ipRow )
	{
		/// フラグ類をクリア
		m_bDeleteFlag = FALSE;
		m_bCreateFlag = FALSE;
		// 削除フラグを設定しないと、CFeatureDef::Store() の リレーションフラグ OFF の処理がうまくいかない
		m_bDeleted = TRUE;
	}

	return bStored;
}
/**
 * テーブル名を取得する
 *
 * @return m_strTableName を返す
 */
LPCTSTR CRowDef::GetTableName()
{
	return m_strTableName;
}
/**
 * 外部テーブルでレコードを削除する場合、このフラグを立てる
 *
 *  m_bDeleteFlag == TRUE,  m_bCreateFlag == FALSE -> 既存レコード削除
 *  m_bDeleteFlag == TRUE,  m_bCreateFlag == TRUE  -> 新規にレコードが追加されたがすぐ削除された（変更なし）
 *  m_bDeleteFlag == FALSE, m_bCreateFlag == FALSE -> 既存レコード
 *  m_bDeleteFlag == FALSE, m_bCreateFlag == TRUE  -> 新規にレコードが追加された
 */
void CRowDef::SetDeleteFlag( BOOL bDelete/* = TRUE */ )
{
	m_bDeleteFlag = bDelete;
}
/**
 * 外部テーブルにレコードを追加する場合、このフラグを立てる
 *
 *  一度セットされたものを削除するときは SetDeleteFlag(TRUE) をしてください
 *
 *  m_bDeleteFlag == TRUE,  m_bCreateFlag == FALSE -> 既存レコード削除
 *  m_bDeleteFlag == TRUE,  m_bCreateFlag == TRUE  -> 新規にレコードが追加されたがすぐ削除された（変更なし）
 *  m_bDeleteFlag == FALSE, m_bCreateFlag == FALSE -> 既存レコード
 *  m_bDeleteFlag == FALSE, m_bCreateFlag == TRUE  -> 新規にレコードが追加された
 */
void CRowDef::SetCreateFlag()
{
	m_bCreateFlag = TRUE;
}
/**
 * テーブルに新規にレコードを追加する
 *
 * m_ipTable が存在して、且つ m_ipRow が存在しない場合、新規にレコードを作成します。
 * 作成に成功したら、m_bDelteFlag と m_bCreateFlag をそれぞれ FALSE にセットします。
 */
void CRowDef::Create()
{
	if( !m_ipTable ) {
		ATLASSERT( FALSE );
		ATLTRACE(_T("CRowDef::Create() : テーブルがセットされていないためレコードを作成できません\n"));
		return;
	}
	if( m_ipRow ) {
		ATLASSERT( FALSE );
		ATLTRACE(_T("CRowDef::Create() : 既にレコードがメンバ変数として存在します\n"));
		return;
	}
	
	m_ipTable->CreateRow( &m_ipRow );
	AheSetModifyData( m_ipRow, sindyUpdateTypeCreate );	// 新規作成履歴
	
	if( !m_ipRow ) {
		ATLTRACE(_T("CRowDef::Create : レコードの作成に失敗しました。レコードは作成されません。\n"));
		return;
	}

	IFieldPtr ipField;

	ATLTRACE(_T("CRowDef::Create() : レコードを作成します...\n"));
	for( iterator it = begin(); it != end(); ++it )
	{
		if( it->IsEditable() )
		{
			m_ipRow->put_Value( it->m_lFieldIndex, it->m_vaValue );
			ATLTRACE(_T("CRowDef::Create() : フィールド＝%s 値＝%s\n"), it->m_strFieldName, it->value2string() );
		}
	}

	if( m_ipRow->Store() != S_OK )
		ATLTRACE(_T("CRowDef::Create() : 作成に失敗しました\n"));
	else
		ATLTRACE(_T("CRowDef::Create() : 作成完了\n"));

	/// フラグ類をクリア
	m_bDeleteFlag = FALSE;
	m_bCreateFlag = FALSE;
}
/**
 * テーブルからレコードを削除する
 *
 * m_ipRow が存在する場合にテーブルからレコードを削除します。
 * 削除が完了したら、m_bDeleteFlag と m_bCreateFlag を FALSE にセットします。
 * また、削除が完了したことを示す m_bDeleted フラグを TRUE にセットします。
 */
void CRowDef::Delete()
{
	if( !m_ipRow ) {
		// CFeatureDef::m_ipFeature が存在するかどうかチェック（ほんとにこんなんでいいのー？）
		m_ipRow = reinterpret_cast<CFeatureDef *>(this)->GetFeature();
		if( !m_ipRow )
		{
			ATLASSERT( FALSE );
			ATLTRACE(_T("CRowDef::Delete() : レコードがメンバ変数として存在していません\n"));
			return;
		}
	}
	
	ATLTRACE(_T("CRowDef::Delete : レコードを削除します...\n"));

	AheSetModifyData( m_ipRow, sindyUpdateTypeDelete );	// 削除履歴
	if( m_ipRow->Delete() != S_OK ) {
		ATLTRACE(_T("CRowDef::Delete() : 削除に失敗しました\n"));
		return;
	}

	/// フラグ類をクリア
	m_bDeleteFlag = FALSE;
	m_bCreateFlag = FALSE;

	if( m_ipRow->Store() != S_OK ) {
		ATLTRACE(_T("CRowDef::Delete() : 削除に失敗しました\n"));
	} else {
		ATLTRACE(_T("CRowDef::Delete() : 削除完了\n"));
	}

	/// 削除完了フラグ（これがないとCRelationTableDef::IsNoRows()でリレーションフラグのチェックが出来ない）
	m_bDeleted = TRUE;
}
/**
 * 元となるレコードをセットする
 *
 * @param ipRow	[in]	元となる _IRow インターフェースポインタ
 */
void CRowDef::SetRow( _IRowPtr ipRow )
{
	m_ipRow = ipRow;
}
/**
 * m_ipRow が存在するテーブルをセットする
 *
 * @param ipTable	[in]	ITable インターフェースポインタ
 */
void CRowDef::SetTable( ITablePtr ipTable )
{
	m_ipTable = ipTable;
}
/**
 * フィールド名から該当する CFieldDef ポインタを取得する
 *
 * @param lpszFieldName	[in]	取得したいフィールド名
 *
 * @return 既に登録されていれば該当する CFieldDef* を、存在しなければ NULL を返す
 */
CFieldDef* CRowDef::GetFieldDef( LPCTSTR lpszFieldName )
{
	for( iterator it = begin(); it != end(); ++it )
	{
		if( it->m_strFieldName.CompareNoCase( lpszFieldName ) == 0 )
			return &(*it);
	}

	return NULL;
}
/**
 * このクラスの大元である _IRow の OBJECTID を返す
 *
 * return OBJECTID
 */
LONG CRowDef::GetRowOID()
{
	if( !m_ipRow ) 
		return -1;

	LONG lOID = 0;
	m_ipRow->get_OID( &lOID );

	return lOID;
}
/**
 * フィールドに変更がないかどうか調べる
 *
 * @return 変更がある場合は TRUE、なければ FALSE
 */
BOOL CRowDef::Changed()
{
	// 作成・削除フラグのチェック
	if( GetDeleteFlag() || GetCreateFlag() )
	{
		ATLTRACE(_T("CRowDef::Changed()：作成・削除フラグが付いています。\n"));
		return TRUE;
	}
	// フィールド
	for( std::list<CFieldDef>::iterator itField = begin(); itField != end(); ++itField )
	{
		if( itField->m_bChanged )
		{
			ATLTRACE(_T("CRowDef::Changed()：値が変更されています：%s value=%s and init value=%s\n"), 
				itField->m_strFieldName, itField->value2string(), itField->value2string(TRUE) );

			return TRUE;
		}
	}

	return FALSE;
}
