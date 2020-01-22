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

/*! \file ControlRel.cpp
	\brief CControlRel クラスのインプリメンテーション
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2002-03-05
*/
#include "stdafx.h"
#include "../ControlRel.h"

#include "ArcHelper.h"

/**
 * コンストラクタ
 */
CControlRel::CControlRel() : m_hWnd(NULL)
{
	m_ipRule.CreateInstance( CLSID_SiNDYRuleObj );
	Clear();
}
/**
 * デストラクタ
 */
CControlRel::~CControlRel()
{
	Clear();
}
/**
 * 全てのメンバ変数を初期化する
 */
void CControlRel::Clear()
{
	iterator i;

	for( i = begin(); i != end(); i++ ) {
		i->Clear();
	}
	clear();

	m_pFeatureDefList = NULL;
}
void CControlRel::ResetHWND( HWND hWnd )
{
	for( iterator it = begin(); it != end(); it++ )
		it->SetHWND( ::GetDlgItem( hWnd, it->m_nID ) );
}
/**
 * 表示したいフィーチャを指定する
 *
 * @param lIndex	[in]	インデックス番号
 */
void CControlRel::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex )
{
	for( iterator it = begin(); it != end(); it++ )
		it->SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex );
}
/**
 * 全てのコントロール定義クラスにフィーチャ定義リストを関連付ける
 *
 * @param pFeatureDefList	[in]	フィーチャ定義クラスのリストポインタ
 */
void CControlRel::SetFeatureDefList( std::list<FEATUREDEF>* pFeatureDefList )
{
	ATLASSERT( pFeatureDefList != NULL );
	for( iterator it = begin(); it != end(); it++ ) it->SetFeatureDefList( pFeatureDefList );

	m_pFeatureDefList = pFeatureDefList;
}

/**
 * フィーチャの初期値でコントロールの表示を初期化する
 */
void CControlRel::ResetFeatures()
{
	for( iterator i = begin(); i != end(); i++ ) {
		i->Reset();
		SetValue( &(*i) );	// コントロール側に値を再セットする必要がある場合がある（グリッドなど）
	}

///	m_bChanged = FALSE;	// 編集フラグ初期化
}
/**
 * コントロールリストとフィーチャリストの関連をクリアする
 */
void CControlRel::ClearFeatureDefs()
{
	for( iterator i = begin(); i != end(); i++ ) 
		i->ClearFeatureDef();
}
/**
 * コントロールを登録する（単一フィールド用）
 *
 * @see CAttrDlg::CreateControlRelation()
 *
 * @param nID			[in]			登録するコントロールのID
 * @param type			[in,optional]	登録するコントロールのタイプ
 * @param nGroupFrom	[in,optional]	表示するグループの開始インデックス
 * @param nGroupTo		[in,optional]	表示するグループの終了インデックス
 * @param lpszFieldName	[in,optional]	コントロールが扱うフィールドの名前
 * @param uiCheckType	[in,optional]	フィールドの値チェックをする時のタイプ
 * @param lpszTableName	[in,optional]	コントロールが扱うフィーチャの所属するテーブル名
 * @param lpszOuterKey	[in,optional]	関連するフィーチャの外部キー用フィールド名（外部テーブルを扱う場合）
 * @param nFollowCtrlID	[in,optional]	追従するコントロールのID
 * @param nFollowBy		[in,optional]	追従する場合のタイプ
 * @param nRelType		[in,optional]	未使用
 */
void CControlRel::SetControl(INT			nID, 
								  CONTROLTYPE	type, 
								  INT			nGroupFrom/* = 0*/,
								  INT			nGroupTo/* = 0*/,
								  LPCTSTR		lpszFieldName/* = NULL */, 
								  UINT			uiCheckType/* = NULL */,
								  LPCTSTR		lpszTableName/* = NULL */, 
								  LPCTSTR		lpszOuterKey/* = NULL */, 
								  INT			nFollowCtrlID/* = NULL */,
								  UINT			uiFollowBy/* = NULL */,
								  INT			nRelType )
{
	CControlDef def;
	def.Clear();
	def.SetControlDef( m_cArcHelper.GetApp(), nID, m_hWnd, type, nGroupFrom, 
		nGroupTo, lpszFieldName, uiCheckType, lpszTableName, lpszOuterKey, nFollowCtrlID, uiFollowBy, nRelType );

	push_back(def);
}
/**
 * コントロールを登録する（複数フィールド用）
 *
 * @see CAttrDlg::CreateControlRelation()
 *
 * @param nID				[in]			登録するコントロールのID
 * @param type				[in]			登録するコントロールのタイプ
 * @param nGroupFrom		[in]			表示するグループの開始インデックス
 * @param nGroupTo			[in]			表示するグループの終了インデックス
 * @param arrayFieldNames	[in,optional]	コントロールが扱うフィールドの名前リスト
 * @param uiCheckType		[in,optional]	フィールドの値チェックをする時のタイプ
 * @param lpszTableName		[in,optional]	コントロールが扱うフィーチャの所属するテーブル名
 * @param lpszOuterKey		[in,optional]	関連するフィーチャの外部キー用フィールド名（外部テーブルを扱う場合）
 * @param nFollowCtrlID		[in,optional]	追従するコントロールのID
 * @param nFollowBy			[in,optional]	追従する場合のタイプ
 * @param nRelType			[in,optional]	未使用
 */
void CControlRel::SetControl(INT			nID, 
								  CONTROLTYPE	     type, 
								  INT			     nGroupFrom,
								  INT			     nGroupTo,
								  std::list<CString> arrayFieldNames, 
								  UINT			     uiCheckType/* = NULL */,
								  LPCTSTR		     lpszTableName/* = NULL */, 
								  LPCTSTR		     lpszOuterKey/* = NULL */, 
								  INT			     nFollowCtrlID/* = NULL */,
								  UINT			     uiFollowBy/* = NULL */,
								  INT			     nRelType)
{
	CControlDef def;
	def.SetControlDef( m_cArcHelper.GetApp(), nID, m_hWnd, type, nGroupFrom, 
		nGroupTo, arrayFieldNames, uiCheckType, lpszTableName, lpszOuterKey, nFollowCtrlID, uiFollowBy, nRelType );

	push_back(def);
}
/**
 * HWND からコントロールを割り出す
 *
 * @param hWnd	[in]	探し出したいコントロールのウィンドウハンドル
 *
 * @return コントロールが登録されていればこのコントロールの定義クラスのポインタを、なければ NULL を返す
 */
CControlDef* CControlRel::GetControlDef( HWND hWnd )
{
	HWND hCtrlWnd;

	for( iterator i = begin(); i != end(); i++ ) {
		hCtrlWnd = ::GetDlgItem( m_hWnd, i->m_nID );
		if( hWnd == hCtrlWnd )
			return &(*i);
	}
	
	return NULL;
}
/**
 * グループ単位でコントロールの表示・非表示を切り替える
 *
 * @param nGroup	[in]			表示するグループ（@see EditView ）
 * @param bForce	[in,optional]	強制的に制御するかどうか（編集開始、終了のハンドリング時のみに使用）
 * @param bEditable	[in,optional]	強制的に編集可・不可を設定する（編集開始、終了のハンドリング時のみに使用）
 */
void CControlRel::Visible( INT nGroup, BOOL bForce/* = FALSE*/, BOOL bEditable/* = TRUE */ )
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( i->m_nGroupFrom <= nGroup && i->m_nGroupTo >= nGroup ) {
			ChangeVisible( &(*i), TRUE );
		} else {
			ChangeVisible( &(*i), FALSE );
		}
		SetValue( &(*i), bForce, bEditable );
	}
}
/**
 * コントロールに変更があった場合に呼ばれる関数
 *
 * @param nID	[in]	変更のあったコントロールのID
 */
void CControlRel::ControlChanged( INT nID )
{
	// 前の値と比べて変更されていたら他のコントロールにも反映させる
	LONG lVal = 0;
	CControlDef* def = NULL;
	for( iterator i = begin(); i != end(); i++ ) {
		if( i->m_nID == nID ) {
			def = &(*i);
			break;
		}
	}

	if( !def )
		return;	// コントロールが登録されていない

	// コントロールから値をセット
	def->SetValueFromControl();

	for( iterator i = begin(); i != end(); i++ ) {
		if( ( i->m_nFollowCtrlID != NULL ) && ( def->m_nID == i->m_nFollowCtrlID ) ) {
			// 値をセット
			SetValue( &(*i) );
			ATLTRACE(_T("CControlRel::ControlChanged()：追従する値が変更されました %s\n"), i->GetFieldName() );
		}
	}
}
/**
 * コントロールの可視・不可視状態を変更する
 *
 * @param def		[in]	制御する CControlDef ポインタ
 * @param bEnable	[in]	可視にする場合は TRUE、不可視にする場合は FALSE（デフォルト：TRUE）
 */
void CControlRel::ChangeVisible( CControlDef* def, BOOL bVisible/* = TRUE*/ )
{
	INT nCmdShow = ( bVisible == TRUE ? SW_SHOW : SW_HIDE );
	::ShowWindow( ::GetDlgItem( m_hWnd, def->m_nID ), nCmdShow );
}
/**
 * 与えられたコントロール定義クラスに値をセットする
 *
 * @param def		[in]			コントロール定義クラス
 * @param bForce	[in,optional]	強制的に制御するかどうか（編集開始、終了のハンドリング時のみに使用）
 * @param bEditable	[in,optional]	強制的に編集可・不可を設定する（編集開始、終了のハンドリング時のみに使用）
 */
void CControlRel::SetValue( CControlDef* def, BOOL bForce/* = FALSE*/, BOOL bEditable/* = TRUE */ )
{

	// 表示状態用チェック(初期値としてとりあえず、-1（未定義）を
	sindyeDispOtherRelation nCheck = sindyeDispOtherNoDef;

	LONG lVal = 0;

	esriEditState state;
	m_cArcHelper.GetEditor()->get_EditState( &state );

	if( state == esriStateEditing )
	{
		// 表示状態の変更
		if( def->m_uiFollowBy & FOLLOW_VALUE ) {
			GetValue( def->m_nFollowCtrlID, lVal );
			if( lVal == 0 ) 
				def->ChangeEnable( FALSE, bForce, bEditable );
			else
				def->ChangeEnable( TRUE, bForce, bEditable );
		}
		else if( def->m_uiFollowBy & FOLLOW_VALUEIS1 ){
			GetValue(def->m_nFollowCtrlID, lVal);
			if(lVal == 1 )
				def->ChangeEnable( TRUE, bForce, bEditable );
			else
				def->ChangeEnable( FALSE, bForce, bEditable );
		}
		else if( def->m_uiFollowBy & FOLLOW_VALUEREVERSE ){
			GetValue(def->m_nFollowCtrlID, lVal);
			if(lVal == 0 )
				def->ChangeEnable( TRUE, bForce, bEditable );
			else
				def->ChangeEnable( FALSE, bForce, bEditable );
		}
		// 読み込み専用属性
		else if( def->m_uiFollowBy & FOLLOW_READONLY ) {
			// [Bug 5509]FOLLOW_READONLYが効かないバグで、[Bug 5409]を修正したときに出てきてしまったバグ
			// bForceやbEditableの意味がなくなっているし、リードオンリーといってるんだから強制的でいいんじゃね
			def->ChangeEnable( FALSE, TRUE, FALSE );	
		}
		else {
			def->ChangeEnable( TRUE, bForce, bEditable );
		}
	}
	else {
		def->ChangeEnable( FALSE, bForce, bEditable );	
	}

	switch( def->m_type ) {
	case TYPE_EDIT:
	case TYPE_EDITSTRING:
	case TYPE_EDITFLOAT:
	case TYPE_EDITDOUBLE:
		def->SetEdit();
		break;
	case TYPE_CHECKBOX:
		def->SetCheckBox();
		break;
	case TYPE_COMBO:
		def->SetComboBox( GetDispClass() );
		break;
	case TYPE_LIST:
		break;
	case TYPE_FEATURESGRID:
		def->SetFeaturesGrid( TRUE );
		break;
	case TYPE_FEATUREGRID:
		def->SetFeatureGrid( FALSE );
		break;
	case TYPE_OTHER:
	case TYPE_BUTTON:
		def->SetOtherControl();
		break;
	case TYPE_TIMEREG:
	case TYPE_OWTIMEREG:
		def->SetLinkTimeRegGrid();
		break;
	case TYPE_OUTERROWS:
		def->SetOuterRowsGrid();
		break;
	default:
		break;
	}
}
/**
 * 渡されたIDを持つコントロール定義クラスの値を LONG 値で返す
 *
 * 現在コントロールに表示されている値ではなく、CFieldDef::m_vaValue になります
 *
 * @param nID	[in]	値を取得したいコントロールのID
 * @param lVal	[out]	取得した値
 */
BOOL CControlRel::GetValue( INT nID, LONG& lVal )
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( nID == i->m_nID ) {
			CComVariant vaValue;
			i->GetOldValue( vaValue );
			lVal = vaValue.lVal;	// LONG値限定
	
			return TRUE;
		}
	}

	return FALSE;
}
/**
 * ROADCLASS_C の値を取得する
 *
 * ROADCLASS_C フィールドが定義されたメンバが存在すれば、それの値を返します
 *
 * @return ROADCLASS_C の値、登録されていなければ -1 を返す
 */
LONG CControlRel::GetDispClass()
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( lstrcmpi( i->GetFieldName(), _T("ROADCLASS_C") ) == 0 ) {
			CComVariant vaValue;
			i->GetOldValue( vaValue );
			
			if( vaValue.vt == VT_NULL )
				return -1;
			else
				return vaValue.lVal;		// LONG値限定
		}
	}
	return -1;
}
/**
 * 通常の処理では処理できないコントロールはここで処理
 *
 * @param l1			[in]	付加パラメータ
 * @param l2			[in]	付加パラメータ
 *
 * @return 編集可能な場合は TRUE、編集不可能な場合は FALSE
 */
BOOL CControlRel::ClickOtherControl( INT nID, LONG l1, LONG l2, BOOL bIsGridEditMode )
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( i->m_nID == nID ) {
			return i->Click( l1, l2, bIsGridEditMode );
		}
	}

	return FALSE;
}
BOOL CControlRel::IsClickCelEditable( INT nID, LONG l1, LONG l2 )
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( i->m_nID == nID ) {
			return i->IsClickCelEditable( l1, l2 );
		}
	}

	return FALSE;
}
/**
 * 通常の処理では処理できないコントロールはここで処理
 *
 * @param l1			[in]	付加パラメータ
 * @param l2			[in]	付加パラメータ
 */
void CControlRel::EditedOtherControl( INT nID, LONG l1, LONG l2 )
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( i->m_nID == nID ) {
			i->Edited( l1, l2 );
			break;
		}
	}
}
/**
 * メンバのダンプをする
 */
void CControlRel::dump()
{
	for( iterator i = begin(); i != end(); i++ ) {
		i->dump( _T("CControlRel::SetValue()") );
	}
}

void CControlRel::SetAliasOrField(BOOL bAliasOrField)
{
	for( iterator def = begin(); def != end(); def++ ) {
		def->m_bAliasOrField = bAliasOrField;
	}
}

BOOL CControlRel::CheckError()
{
	BOOL		bIsError = FALSE;
	for( iterator def = begin(); def != end(); def++) {
		if (def->Errored()) {
			bIsError = TRUE;
		}
	}
	return bIsError;
}
