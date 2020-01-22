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

/*! \file RowsControlRel.cpp
	\brief CRowsControlRel クラスのインターフェース
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2002-03-05
*/
#include "stdafx.h"
#include "../RowsControlRel.h"

#include "MapHelper.h"
#include "ArcHelper.h"

/**
 * コンストラクタ
 */
CRowsControlRel::CRowsControlRel()
{
	m_lFeatureDefIndex = -1;
	m_listFeatureDef.clear();
}
/**
 * デストラクタ
 */
CRowsControlRel::~CRowsControlRel()
{
	m_listFeatureDef.clear();
}
/**
 * コントロールを登録する（単一フィールド用）
 *
 * @see CAttrDlg::CreateControlRelation()
 *
 * @param nID			[in]			登録するコントロールのID
 * @param type			[in]			登録するコントロールのタイプ
 * @param nGroupFrom	[in]			表示するグループの開始インデックス
 * @param nGroupTo		[in]			表示するグループの終了インデックス
 * @param lpszFieldName	[in,optional]	コントロールが扱うフィールドの名前
 * @param uiCheckType	[in,optional]	フィールドの値チェックをする時のタイプ
 * @param lpszTableName	[in,optional]	コントロールが扱うフィーチャの所属するテーブル名
 * @param lpszOuterKey	[in,optional]	関連するフィーチャの外部キー用フィールド名（外部テーブルを扱う場合）
 * @param nFollowCtrlID	[in,optional]	追従するコントロールのID
 * @param nFollowBy		[in,optional]	追従する場合のタイプ
 * @param nRelType		[in,optional]	未使用
 */
void CRowsControlRel::SetControl(INT			nID, 
								  CONTROLTYPE	type, 
								  INT			nGroupFrom,
								  INT			nGroupTo,
								  LPCTSTR		lpszFieldName/* = NULL */, 
								  UINT			uiCheckType/* = NULL */,
								  LPCTSTR		lpszTableName/* = NULL */, 
								  LPCTSTR		lpszOuterKey/* = NULL */, 
								  INT			nFollowCtrlID/* = NULL */,
								  UINT			uiFollowBy/* = NULL */,
								  INT			nRelType )
{
	m_pControlRel.SetControl( nID, type, nGroupFrom, nGroupTo,
		lpszFieldName, uiCheckType, lpszTableName, lpszOuterKey, nFollowCtrlID, uiFollowBy, nRelType );
}
/**
 * コントロールを登録する（複数フィールド用）
 *
 * @see CAttrDlg::CreateControlRelation()
 *
 * @param nID			[in]			登録するコントロールのID
 * @param type			[in]			登録するコントロールのタイプ
 * @param nGroupFrom	[in]			表示するグループの開始インデックス
 * @param nGroupTo		[in]			表示するグループの終了インデックス
 * @param lpszFieldName	[in,optional]	コントロールが扱うフィールドの名前リスト
 * @param uiCheckType	[in,optional]	フィールドの値チェックをする時のタイプ
 * @param lpszTableName	[in,optional]	コントロールが扱うフィーチャの所属するテーブル名
 * @param lpszOuterKey	[in,optional]	関連するフィーチャの外部キー用フィールド名（外部テーブルを扱う場合）
 * @param nFollowCtrlID	[in,optional]	追従するコントロールのID
 * @param nFollowBy		[in,optional]	追従する場合のタイプ
 * @param nRelType		[in,optional]	未使用
 */
void CRowsControlRel::SetControl(INT				 nID, 
								  CONTROLTYPE		 type, 
								  INT			     nGroupFrom,
								  INT			     nGroupTo,
								  std::list<CString> arrayFieldNames, 
								  UINT				 uiCheckType/* = NULL */,
								  LPCTSTR			 lpszTableName/* = NULL */, 
								  LPCTSTR			 lpszOuterKey/* = NULL */, 
								  INT				 nFollowCtrlID/* = NULL */,
								  UINT				 uiFollowBy/* = NULL */,
								  INT				 nRelType )
{
	m_pControlRel.SetControl( nID, type, nGroupFrom, nGroupTo,
		arrayFieldNames, uiCheckType, lpszTableName, lpszOuterKey, nFollowCtrlID, uiFollowBy, nRelType );
}
/**
 * フィーチャを登録する（複数フィーチャ用）
 *
 * @param ipSet		[in]	フィーチャの入ったセット
 *
 * @return 表示されるフィーチャ
 */
_IRowPtr CRowsControlRel::SetFeatures( _ISetPtr ipSet )
{
	IUnknownPtr ipFeature;
	CMapHelper cMapHelper( m_cArcHelper );

	ipSet->Reset();
	while( ipSet->Next( &ipFeature ) == S_OK ) {
		if( !ipFeature ) break;
	
		CFeatureDef featDef;
		featDef.SetArcHelper( m_cArcHelper.GetApp() );
		featDef.SetFeature( ipFeature );
		m_listFeatureDef.push_back( featDef );
	}
	// フィーチャ定義クラスをコントロールに関連付け
	m_pControlRel.SetFeatureDefList( &m_listFeatureDef );
	// 現在のフィーチャをセット（一番最初のもの）
	SetView( 0 );

	if( m_listFeatureDef.size() == 0 )
		return NULL;
	else
		return m_listFeatureDef.begin()->GetFeature();	// 一番最初を返す
}
/**
 * フィーチャを登録する（単一フィーチャ用）
 *
 * @param ipFeature		[in]	セットしたいフィーチャ
 *
 * \note 複数登録する場合は続けて呼ぶことで OK ですが、まだちゃんと実装されていません。
 * \note その結果、フィーチャの切り替えを何度もすると複数登録されてしまうはずです。
 */
void CRowsControlRel::SetFeature( IFeaturePtr ipFeature )
{
	CFeatureDef featDef;
	
	featDef.SetArcHelper( m_cArcHelper.GetApp() );
	featDef.SetFeature( ipFeature );
	m_listFeatureDef.push_back( featDef );

	// フィーチャ定義クラスをコントロールに関連付け
	m_pControlRel.SetFeatureDefList( &m_listFeatureDef );
	// 現在のフィーチャをセット
	SetView( 0 );
}
/**
 * 与えられたインデックス番号で示されるフィーチャを取得する
 *
 * @param lIndex	[in]	フィーチャのインデックス番号
 *
 * @return 登録されている場合はフィーチャ（レコード）、登録されていなければ NULL を返す
 */
_IRowPtr CRowsControlRel::GetFeature( LONG lIndex )
{
	ATLASSERT( lIndex >= 0 );

	// 一つも登録されていない場合は NULL 
	if( m_listFeatureDef.size() == 0 ) return NULL;

	std::list<CFeatureDef>::iterator i = m_listFeatureDef.begin();
	advance( i, lIndex );

	return i->GetFeature();
}
/**
 * ArcObjects ヘルパークラスをセットする
 *
 * このクラスを使用する前に必ずセットしてください。
 *
 * @param pHelper	[in]	ArcObjects ヘルパークラスのポインタ
 */
void CRowsControlRel::SetArcHelper( IApplication* ipApp )
{
	ATLASSERT( ipApp != NULL );
	m_cArcHelper.Init( (IApplicationPtr)ipApp );
	m_pControlRel.SetArcHelper( m_cArcHelper.GetApp() );
}
/**
 * メンバを全て初期化する
 */
void CRowsControlRel::Clear()
{
	m_listFeatureDef.clear();
	m_pControlRel.Clear();
}
/**
 * 初期状態（編集前）に戻す
 */
void CRowsControlRel::Reset()
{
	for( std::list<CFeatureDef>::iterator i = m_listFeatureDef.begin(); i != m_listFeatureDef.end(); i++ ) {
		i->Reset();		
	}
	m_pControlRel.ResetFeatures();
}
/**
 * フィーチャを保存する
 *
 * @param bDoOperation		[in]	StartEditOperation を自分でするか、呼び出し側のツールでするかどうか
 */
void CRowsControlRel::Store( BOOL bDoOperation )
{
	for( std::list<CFeatureDef>::iterator i = m_listFeatureDef.begin(); i != m_listFeatureDef.end(); i++ ) {
		i->Store( bDoOperation );
	}
}
/**
 * 現在表示されているフィーチャのラッパクラスのポインタを返す
 *
 * @return 現在表示されているフィーチャのラッパクラスのポインタを返す
 */
CFeatureDef* CRowsControlRel::GetCurrentFeatureDef()
{
	std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin();
	advance( it, m_lFeatureDefIndex );

	return &(*it);
}
void CRowsControlRel::SetView( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex )
{
	m_pControlRel.SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex );

	m_lFeatureDefIndex = lFeatureIndex;
}
/**
 * 変更があるかどうかの判定
 *
 * return 変更がある場合は TRUE、なければ FALSE
 */
BOOL CRowsControlRel::Changed()
{
	for( std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin(); it != m_listFeatureDef.end(); it++ )
	{
		if( it->Changed() ) 
			return TRUE;
	}

	return FALSE;
}
