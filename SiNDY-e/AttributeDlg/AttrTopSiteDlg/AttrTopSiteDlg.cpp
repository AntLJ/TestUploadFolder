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

// AttrTopSiteDlg.cpp: CAttrTopSiteDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrTopSiteDlg.h"

/// ウィンドウマネージャ配置マップ（トップ背景用）
BEGIN_WINDOW_MAP(CSITE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SITE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SITE)
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
 * @retval CAttrTopSiteDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrTopSiteDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrTopSiteDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrTopSiteDlg::CAttrTopSiteDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrTopSiteDlg::CAttrTopSiteDlg() 
	: ATTR_BASE_CLASS(CSITE_MAP, IDD_ATTRTOPSITEDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_SITE ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_GENKOU ),
		m_wnd2( _T("BUTTON"), this, IDC_BUTTON_SEARCH ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CAttrTopSiteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrTopSiteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrTopSiteDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrTopSiteDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrTopSiteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrTopSiteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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

    return bRet;
}

/**
 * @fn void CAttrTopSiteDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrTopSiteDlg::CreateControlRelation()
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

		// 背景種別
		m_cControlRel.SetControl( IDC_COMBO_SITE,		TYPE_COMBO,		0,	0,  _T("BGCLASS_C"),	NULL, _T("TOP_SITE") );
		// 原稿番号入力
		m_cControlRel.SetControl( IDC_EDIT_GENKOU,		TYPE_EDIT,		0,	0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH,	TYPE_BUTTON,	0,	0 );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,  _T("SOURCE"),		NULL, _T("TOP_SITE") );
	}
}

/**
 * @fn BOOL CAttrTopSiteDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
 * @brief 背景種別をコンボボックスに設定する
 *
 * 背景種別EditBoxに入力された背景原稿番号から背景種別コードを取得し、背景種別ComboBoxに設定する
 *
 * @param nCodeID			[in]	背景原稿番号が入っているEditBoxのコードID
 * @param nClassID			[in]	背景種別ComboBoxのコードID
 *
 * @return 背景種別をセットすることができたらTRUE,できなかったらFALSE
 */
BOOL CAttrTopSiteDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
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
 * @fn void CAttrTopSiteDlg::ShowGenkouByBackgroundClass()
 * @brief 現在選択中の背景種別の原稿番号を、原稿番号フィールドに表示
 */
void CAttrTopSiteDlg::ShowGenkouByBackgroundClass()
{
	CWindow	cWGenkou(GetDlgItem(IDC_EDIT_GENKOU));			// 原稿番号テキストボックス
	long lBackgroundClass = -1;
	GetBackgroundClass(&lBackgroundClass);
	cWGenkou.SetWindowText( GetMSNOByClass( lBackgroundClass ) );
	return;
}

/**
 * @fn void CAttrTopSiteDlg::GetBackgroundClass(LONG *lBackgroundClass)
 * @brief 現在選択中の背景種別IDを取得
 *
 * @param	lBackgroundClass	[out]		選択中の背景種別ID		
 */
void CAttrTopSiteDlg::GetBackgroundClass(LONG *lBackgroundClass)
{
	CComboBox	cWAnnoClass(GetDlgItem(IDC_COMBO_SITE));
	LONG		lSelectIndex= cWAnnoClass.GetCurSel();
	DWORD		dbComboBoxValue = cWAnnoClass.GetItemData(lSelectIndex);
	*lBackgroundClass = (LONG)dbComboBoxValue;
}

/**
 * @fn BOOL CAttrTopSiteDlg::CheckReturnKeyItem(INT nClassID)
 * @brief リターンキーが押された時の動作
 *
 * @param	nClassID	[in]		リターンキーが押された時のItemID
 *
 * @return	TRUE : リターンキーとしての動作(適用処理)可 , FALSE : リターンキーとしての動作不可 
 */
BOOL CAttrTopSiteDlg::CheckReturnKeyItem(INT nClassID)
{
	if ((nClassID == IDC_EDIT_GENKOU) || (nClassID == IDC_BUTTON_SEARCH)) {		// 原稿番号エディットボックス or 検索ボタン
		if( !SetBackgroundClassCode(IDC_EDIT_GENKOU, IDC_COMBO_SITE) )
			return FALSE;
	}
	return TRUE;
}

/**
 * @fn BOOL CAttrTopSiteDlg::CheckEscKeyItem(INT nClassID)
 * @brief ESCキーが押された時の動作
 *
 * @param	ClassID	[i]		ESCキーが押された時のItemID
 *
 * @return	TRUE : ESCキーとしての動作(適用処理)可 , FALSE : ESCキーとしての動作不可 
 */
BOOL CAttrTopSiteDlg::CheckEscKeyItem(INT nClassID)
{
	if ((nClassID == IDC_EDIT_GENKOU) || (nClassID == IDC_BUTTON_SEARCH)) {		// 原稿番号エディットボックス or 検索ボタン
		if( !SetBackgroundClassCode(IDC_EDIT_GENKOU, IDC_COMBO_SITE) )
			return FALSE;
	}
	return TRUE;
}

