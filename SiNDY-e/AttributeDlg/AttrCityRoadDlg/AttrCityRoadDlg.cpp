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

// AttrCityRoadDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrCityRoadDlg.h"
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(CSITE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ROADCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_ROADCLASS_C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENKOU)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENKOU)
				RCTOFIT(IDC_BUTTON_SEARCH)
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
 * @retval CAttrCityRoadDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrCityRoadDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCityRoadDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrCityRoadDlg::CAttrCityRoadDlg() 
	: ATTR_BASE_CLASS(CSITE_MAP, IDD_CITYROADDLG)	
{
	// 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		( IDC_COMBO_ROADCLASS_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_ROADCLASS_C ) )
		( IDC_EDIT_GENKOU, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_GENKOU ) )
		( IDC_BUTTON_SEARCH, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCH ) );
		
}

/** 
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrCityRoadDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrCityRoadDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
    delete this; // 必ずnewで確保する前提
}

/**
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * また、原稿番号の値も初期化して、ルールから取得しておく
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrCityRoadDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	ShowGenkouByBackgroundClass();
	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 最初にフォーカスされるコントロール
		SetIMEMode(FALSE);
		::SetFocus( GetDlgItem(IDC_EDIT_GENKOU) );
	}

    return bRet;
}

/**
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrCityRoadDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// ControlRelにフィールド名とテーブル名を登録
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,       TYPE_EDITSTRING, 0, 0, city_road::kSource,    0, city_road::kTableName );
		// 道路ポリゴン種別
		m_cControlRel.SetControl( IDC_COMBO_ROADCLASS_C, TYPE_COMBO,      0, 0, city_road::kRoadClass, 0, city_road::kTableName );
		// 原稿番号入力
		m_cControlRel.SetControl( IDC_EDIT_GENKOU,		TYPE_EDIT,		0,	0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH,	TYPE_BUTTON,	0,	0 );
	}
}

/**
 * @brief 道路ポリゴンEditBoxに入力された道路ポリゴン原稿番号から道路ポリゴン種別コードを取得し、道路ポリゴン種別ComboBoxに設定
 *
 * @param nCodeID			[in]	道路ポリゴン原稿番号が入っているEditBoxのコードID
 * @param nClassID			[in]	道路ポリゴン種別ComboBoxのコードID
 *
 * @retval TRUE  道路ポリゴン種別をセット可
 * @retval FALSE 道路ポリゴン種別をセット不可
 */
BOOL CAttrCityRoadDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
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
		// 道路ポリゴン種別コンボボックス取得
		CComboBox	cWAnnoClass( GetDlgItem(nClassID) );
		LONG		lClassNum = cWAnnoClass.GetCount();
		LONG		lComboBoxItemNum = 0;
		DWORD		dwComboBoxValue = 0;
		LONG		lComboBoxValue = 0;
		for (; lComboBoxItemNum < lClassNum ; ++lComboBoxItemNum)
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
 * @brief 現在選択中の道路ポリゴン種別の原稿番号を、原稿番号フィールドに表示
 */
void CAttrCityRoadDlg::ShowGenkouByBackgroundClass()
{
	CWindow	cWGenkou(GetDlgItem(IDC_EDIT_GENKOU));			// 原稿番号テキストボックス
	long lBackgroundClass = -1;
	GetBackgroundClass(&lBackgroundClass);
	cWGenkou.SetWindowText( GetMSNOByClass( lBackgroundClass ) );
}

/**
 * @brief 現在選択中の道路ポリゴン種別IDを取得
 *
 * @param	lBackgroundClass	[out]		選択中の道路ポリゴン種別ID		
 */
void CAttrCityRoadDlg::GetBackgroundClass(LONG *lBackgroundClass)
{
	CComboBox	cWAnnoClass(GetDlgItem(IDC_COMBO_ROADCLASS_C));
	LONG		lSelectIndex = cWAnnoClass.GetCurSel();
	DWORD		dbComboBoxValue = cWAnnoClass.GetItemData(lSelectIndex);
	*lBackgroundClass = (LONG)dbComboBoxValue;
}

/**
 * @brief リターンキーが押された時の動作
 *
 * @param	nClassID	[in]		リターンキーが押された時のItemID
 *
 * @retval  TRUE  リターンキーとしての動作(適用処理)可
 * @retval  FALSE リターンキーとしての動作不可 
 */
BOOL CAttrCityRoadDlg::CheckReturnKeyItem(INT nClassID)
{
	return SetGetBackgroundClassToCombo(nClassID);
}

/**
 * @brief ESCキーが押された時の動作
 *
 * @param	ClassID	[i]		ESCキーが押された時のItemID
 *
 * @retval  TRUE  ESCキーとしての動作(適用処理)可
 * @retval  FALSE ESCキーとしての動作不可 
 */
BOOL CAttrCityRoadDlg::CheckEscKeyItem(INT nClassID)
{
	return SetGetBackgroundClassToCombo(nClassID);
}

/**
 * @brief 種別コードをコンボボックスにセットする
 *
 * @param	ClassID	[i]		ESCキーが押された時のItemID
 *
 * @retval  TRUE  キーとしての動作(適用処理)可
 * @retval  FALSE キーとしての動作不可 
 */
BOOL CAttrCityRoadDlg::SetGetBackgroundClassToCombo(INT nClassID)
{
	if ((nClassID == IDC_EDIT_GENKOU) || (nClassID == IDC_BUTTON_SEARCH)) {		// 原稿番号エディットボックス or 検索ボタン
		if( !SetBackgroundClassCode(IDC_EDIT_GENKOU, IDC_COMBO_ROADCLASS_C) )
			return FALSE;
	}
	return TRUE;
}
