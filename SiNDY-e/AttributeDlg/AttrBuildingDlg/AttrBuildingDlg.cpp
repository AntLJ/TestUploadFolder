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

// AttrBuildingDlg.cpp: CAttrBuildingDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrBuildingDlg.h"

using namespace sindy::schema;

/// ウィンドウマネージャ配置マップ（都市地図家形用）
BEGIN_WINDOW_MAP(BUILDING_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_BLDCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_BLDCLASS_C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENKOU)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENKOU)
				RCTOFIT(IDC_BUTTON_SEARCH)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_CORRIDOR_F)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_NOWALL_F)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GEOID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GEOID)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrBuildingDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrBuildingDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBuildingDlg::CAttrBuildingDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrBuildingDlg::CAttrBuildingDlg() 
	: ATTR_BASE_CLASS(BUILDING_MAP, IDD_ATTRBUILDINGDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_BLDCLASS_C ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_GENKOU ),
		m_wnd2( _T("BUTTON"), this, IDC_BUTTON_SEARCH ),
		m_wnd3( _T("CHECKBOX"), this, IDC_CHECK_CORRIDOR_F ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd5( _T("CHECKBOX"), this, IDC_CHECK_NOWALL_F ),
		m_wnd6( _T("EDIT"), this, IDC_EDIT_GEOID )
{
}

/** 
 * @fn HWND CAttrBuildingDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrBuildingDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrBuildingDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrBuildingDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrBuildingDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * 原稿番号の設定もしておきます。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */

BOOL CAttrBuildingDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	ShowGenkouByBackgroundClass();

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		SetIMEMode(FALSE);
		::SetFocus( GetDlgItem(IDC_EDIT_GENKOU) );
	}

	// GEOSPACE_IDは編集されないのでDISABLEに
	SetDlgStatus( IDC_EDIT_GEOID, FALSE, TRUE );

    return bRet;
}

/**
 * @fn void CAttrBaseRailwayDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrBuildingDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// 都市地図家形用

		// 道路重複許可フラグ(AK対応で渡り廊下から名称変更)
		m_cControlRel.SetControl( IDC_CHECK_CORRIDOR_F, TYPE_CHECKBOX,   0, 0, building::kCorridor,   NULL, BUILDING_TABLE_NAME );
		// 原稿番号チェック
		m_cControlRel.SetControl( IDC_EDIT_GENKOU,      TYPE_EDIT,       0, 0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH,    TYPE_BUTTON,     0, 0 );
		// 建物種別
		m_cControlRel.SetControl( IDC_COMBO_BLDCLASS_C,	TYPE_COMBO,      0, 0, building::kBldClass,   NULL, BUILDING_TABLE_NAME );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,      TYPE_EDITSTRING, 0, 0, building::kSource,     NULL, BUILDING_TABLE_NAME );
		// 無壁舎フラグ
		m_cControlRel.SetControl( IDC_CHECK_NOWALL_F,   TYPE_CHECKBOX,   0, 0, building::kNoWall,     NULL, BUILDING_TABLE_NAME );
		// GEOSPACEID
		m_cControlRel.SetControl( IDC_EDIT_GEOID,       TYPE_EDITSTRING, 0, 0, building::kGeospaceID, NULL, BUILDING_TABLE_NAME );
	}
}


/**
 * @fn BOOL CAttrBuildingDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
 * @brief 建物種別設定
 *
 * 建物種別EditBoxに入力された建物種別原稿番号から建物種別コードを取得し、建物種別ComboBoxに設定する
 *
 * @param nCodeID			[in]	建物種別原稿番号が入っているEditBoxのコードID
 * @param nClassID			[in]	建物種別ComboBoxのコードID
 *
 * @return 建物種別をセットすることができたらTRUE,できなかったらFALSE
 */
BOOL CAttrBuildingDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
{
	CEdit edit( GetDlgItem( nCodeID ) );
	CString strGenkouCode;
	edit.GetWindowText( strGenkouCode );

	// 原稿番号を半角に変換
	CString	strGenkou = AheConvertToGenkouNoStr(strGenkouCode);	// 半角,英字OK,マイナスNO

	// BACKGROUND_CLASSからコード取得
	long lSearchClassID = -1;
	if (!strGenkou.IsEmpty())
		lSearchClassID = GetClassByMSNO( strGenkou );

	if (lSearchClassID >= 0)
	{
		// 背景種別コンボボックス取得
		CComboBox	cWAnnoClass( GetDlgItem(nClassID) );
		LONG		lClassNum = cWAnnoClass.GetCount();
		LONG		lComboBoxItemNum = 0;
		DWORD		dwComboBoxValue = 0;
		LONG		lComboBoxValue = 0;
		for (lComboBoxItemNum ; lComboBoxItemNum < lClassNum ; lComboBoxItemNum++)
		{
			dwComboBoxValue = cWAnnoClass.GetItemData(lComboBoxItemNum);
			lComboBoxValue = (INT)dwComboBoxValue;
			if (lComboBoxValue == lSearchClassID)
				break;
		}
		cWAnnoClass.SetCurSel(lComboBoxItemNum);

		SetDlgItemText(nCodeID, strGenkou);			// 原稿番号を設定

		m_cControlRel.ControlChanged( nClassID );
		InvalidateRect( NULL, FALSE );
		// 変更された場合はボタンを Enable に
		SetButton( Changed() );
	} else {
		AttrMessageBox( AheLoadString( IDS_NOMANUSCRIPT ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
		ShowGenkouByBackgroundClass();			// 原稿番号を設定
		return FALSE;
	}
	return TRUE;
}

/**
 * @fn void CAttrBuildingDlg::ShowGenkouByBackgroundClass()
 * @brief 現在選択中の建物種別の原稿番号を、原稿番号フィールドに表示
 */
void CAttrBuildingDlg::ShowGenkouByBackgroundClass()
{
	CWindow	cWGenkou(GetDlgItem(IDC_EDIT_GENKOU));			// 原稿番号テキストボックス
	long lBackgroundClass = -1;
	GetBackgroundClass(&lBackgroundClass);
	cWGenkou.SetWindowText( GetMSNOByClass( lBackgroundClass ) );
	return;
}

/**
 * @fn void CAttrBuildingDlg::GetBackgroundClass(LONG *lBackgroundClass)
 * @brief 現在選択中の建物種別IDを取得
 *
 * @param	lBackgroundClass		[o]		選択中の建物種別ID		
 */
void CAttrBuildingDlg::GetBackgroundClass(LONG *lBackgroundClass)
{
	CComboBox	cWAnnoClass(GetDlgItem(IDC_COMBO_BLDCLASS_C));
	LONG		lSelectIndex = cWAnnoClass.GetCurSel();
	DWORD		dbComboBoxValue = cWAnnoClass.GetItemData(lSelectIndex);
	*lBackgroundClass = (LONG)dbComboBoxValue;
}

/**
 * @fn BOOL CAttrBuildingDlg::CheckReturnKeyItem(INT nClassID)
 * @brief リターンキーが押された時の動作
 *
 * @param	nClassID	[in]		リターンキーが押された時のItemID
 *
 * @return	TRUE : リターンキーとしての動作(適用処理)可 , FALSE : リターンキーとしての動作不可 
 */
BOOL CAttrBuildingDlg::CheckReturnKeyItem(INT nClassID)
{
	if ((nClassID == IDC_EDIT_GENKOU) || (nClassID == IDC_BUTTON_SEARCH)) {		// 原稿番号エディットボックス or 検索ボタン
		if( !SetBackgroundClassCode(IDC_EDIT_GENKOU, IDC_COMBO_BLDCLASS_C) )
			return FALSE;
	}
	return TRUE;
}

/**
 * @fn BOOL CAttrBuildingDlg::CheckEscKeyItem(INT nClassID)
 * @brief ESCキーが押された時の動作
 *
 * @param	ClassID	[i]		ESCキーが押された時のItemID
 *
 * @return	TRUE : ESCキーとしての動作(適用処理)可 , FALSE : ESCキーとしての動作不可 
 */
BOOL CAttrBuildingDlg::CheckEscKeyItem(INT nClassID)
{
	if ((nClassID == IDC_EDIT_GENKOU) || (nClassID == IDC_BUTTON_SEARCH)) {		// 原稿番号エディットボックス or 検索ボタン
		if( !SetBackgroundClassCode(IDC_EDIT_GENKOU, IDC_COMBO_BLDCLASS_C) )
			return FALSE;
	}
	return TRUE;
}

/**
 * @brief GEOSPACE_IDが変更された時の動作
 *
 * @return なし
 */
void CAttrBuildingDlg::SetGeospceID()
{
	// GEOSPACE_IDをエディットボックスから取得
	CEdit cEdit( GetDlgItem( IDC_EDIT_GEOID ) );
	CString strEdit;
	cEdit.GetWindowText( strEdit );

	// GEOSPACE_IDを半角に変換(原稿番号の変換ロジック使用)
	// ひらがな等は取り除かれる ex. 123あいうabcDEF → 123ABCDEF
	CString strGeoID;
	strGeoID = AheConvertToGenkouNoStr( strEdit );	// 半角,英字OK,マイナスNO

	// 変換したGEOSPACE_IDをセット
	cEdit.SetWindowText( strEdit );
}
