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

// AttrBaseStationDlg.cpp: CAttrBaseStationDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrBaseStationDlg.h"

/// ウィンドウマネージャ配置マップ（中縮ライン用）
BEGIN_WINDOW_MAP(BASE_STATION_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_UNDERGROUND_F)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_STATIONCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_STATIONCLASS_C)
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
 * @retval CAttrBaseStationDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrBaseStationDlg();
}
/////////////////////////////////////////////////////////////////////////////
//
// CAttrBaseStationDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBaseStationDlg::CAttrBaseStationDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrBaseStationDlg::CAttrBaseStationDlg()
	: ATTR_BASE_CLASS(BASE_STATION_MAP, IDD_ATTRBASESTATIONDLG),
		m_wnd0( _T("CHECKBOX"), this, IDC_CHECK_UNDERGROUND_F ), 
		m_wnd1( _T("COMBOBOX"), this, IDC_COMBO_STATIONCLASS_C ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_GENKOU ),
		m_wnd3( _T("BUTTON"), this, IDC_BUTTON_SEARCH ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_SOURCE)
{
}

/** 
 * @fn HWND CAttrBaseStationDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrBaseStationDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrBaseStationDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrBaseStationDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrBaseStationDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * 原稿番号もセットしておきます。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrBaseStationDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
 * @fn void CAttrBaseStationDlg::CreateControlRelation()
 * @brief 各コントロールに値をセットします
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrBaseStationDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {

		m_cControlRel.SetHWND( m_hWnd );
		// 中縮鉄道用

		// 地下フラグ
		m_cControlRel.SetControl( IDC_CHECK_UNDERGROUND_F,		TYPE_CHECKBOX,	0,	0,	_T("UNDERGROUND_F"),	NULL, BASESTATION_TABLE_NAME );
		// 鉄道種別
		m_cControlRel.SetControl( IDC_COMBO_STATIONCLASS_C,		TYPE_COMBO,		0,	0,	_T("STATIONCLASS_C"),	NULL, BASESTATION_TABLE_NAME );
		// 原稿番号入力用
		m_cControlRel.SetControl( IDC_EDIT_GENKOU,				TYPE_EDIT,		0,	0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH,			TYPE_BUTTON,	0,	0 );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,				TYPE_EDITSTRING,0,	0,	_T("SOURCE"),			NULL, BASESTATION_TABLE_NAME );
	}
}

/**
 * @fn BOOL CAttrBaseStationDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
 * @brief 中縮鉄道駅舎種別設定
 *
 * 中縮鉄道駅舎種別EditBoxに入力された鉄道種別駅舎原稿番号から鉄道種別コードを取得し、鉄道種別駅舎ComboBoxに設定する
 *
 * @param nCodeID			[in]	鉄道種別原稿番号が入っているEditBoxのコードID
 * @param nClassID			[in]	鉄道種別ComboBoxのコードID
 *
 * @return 鉄道種別をセットすることができたらTRUE,できなかったらFALSE
 */
BOOL CAttrBaseStationDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
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
		// 鉄道駅舎種別コンボボックス取得
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
			{
				break;
			}
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
 * @fn void CAttrBaseStationDlg::ShowGenkouByBackgroundClass()
 * @brief 現在選択中の鉄道種別の原稿番号を、原稿番号フィールドに表示
 */
void CAttrBaseStationDlg::ShowGenkouByBackgroundClass()
{
	CWindow	cWGenkou(GetDlgItem(IDC_EDIT_GENKOU));			// 原稿番号テキストボックス
	long lBackgroundClass = -1;
	GetBackgroundClass(&lBackgroundClass);
	cWGenkou.SetWindowText( GetMSNOByClass( lBackgroundClass ) );
	return;
}

/**
 * @fn void CAttrBaseStationDlg::GetBackgroundClass(LONG *lBackgroundClass)
 * @brief 現在選択中の鉄道種別IDを取得
 *
 * @param	lBackgroundClass		[o]		選択中の鉄道種別ID		
 */
void CAttrBaseStationDlg::GetBackgroundClass(LONG *lBackgroundClass)
{
	CComboBox	cWAnnoClass(GetDlgItem(IDC_COMBO_STATIONCLASS_C));
	LONG		lSelectIndex = cWAnnoClass.GetCurSel();
	DWORD		dbComboBoxValue = cWAnnoClass.GetItemData(lSelectIndex);
	*lBackgroundClass = (LONG)dbComboBoxValue;
}

/**
 * @fn BOOL CAttrBaseStationDlg::CheckReturnKeyItem(INT nClassID)
 * @brief リターンキーが押された時の動作
 *
 * @param	nClassID	[in]		リターンキーが押された時のItemID
 *
 * @return	TRUE : リターンキーとしての動作(適用処理)可 , FALSE : リターンキーとしての動作不可 
 */
BOOL CAttrBaseStationDlg::CheckReturnKeyItem(INT nClassID)
{
	if ((nClassID == IDC_EDIT_GENKOU) || (nClassID == IDC_BUTTON_SEARCH)) {		// 原稿番号エディットボックス or 検索ボタン
		if( !SetBackgroundClassCode(IDC_EDIT_GENKOU, IDC_COMBO_STATIONCLASS_C) )
			return FALSE;
	}
	return TRUE;
}

/**
 * @fn BOOL CAttrBaseStationDlg::CheckEscKeyItem(INT nClassID)
 * @brief ESCキーが押された時の動作
 *
 * @param	ClassID	[i]		ESCキーが押された時のItemID
 *
 * @return	TRUE : ESCキーとしての動作(適用処理)可 , FALSE : ESCキーとしての動作不可 
 */
BOOL CAttrBaseStationDlg::CheckEscKeyItem(INT nClassID)
{
	if ((nClassID == IDC_EDIT_GENKOU) || (nClassID == IDC_BUTTON_SEARCH)) {		// 原稿番号エディットボックス or 検索ボタン
		if( !SetBackgroundClassCode(IDC_EDIT_GENKOU, IDC_COMBO_STATIONCLASS_C) )
			return FALSE;
	}
	return TRUE;
}

