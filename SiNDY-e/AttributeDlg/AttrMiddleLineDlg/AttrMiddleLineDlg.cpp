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

// AttrMiddleLineDlg.cpp: CAttrMiddleLineDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrMiddleLineDlg.h"

/// ウィンドウマネージャ配置マップ（ミドルライン用）
BEGIN_WINDOW_MAP(MIDDLE_LINE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_SC1)
				RCTOFIT(-1)
				RCTOFIT(IDC_CHECK_SC2)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_SC3)
				RCTOFIT(-1)
				RCTOFIT(IDC_CHECK_SC4)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(-1)
				RCTOFIT(IDC_CHECK_UG_F)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LINETYPE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LINETYPE)
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
 * @retval CAttrMiddleLineDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrMiddleLineDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrMiddleLineDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrMiddleLineDlg::CAttrMiddleLineDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrMiddleLineDlg::CAttrMiddleLineDlg()
	: ATTR_BASE_CLASS(MIDDLE_LINE_MAP, IDD_ATTRMIDDLELINEDLG),
		m_wnd0( _T("CHECKBOX"), this, IDC_CHECK_SC1 ), 
		m_wnd1( _T("CHECKBOX"), this, IDC_CHECK_SC2 ), 
		m_wnd2( _T("CHECKBOX"), this, IDC_CHECK_SC3 ), 
		m_wnd3( _T("CHECKBOX"), this, IDC_CHECK_SC4 ), 
		m_wnd4( _T("CHECKBOX"), this, IDC_CHECK_UG_F ), 
		m_wnd5( _T("COMBOBOX"), this, IDC_COMBO_LINETYPE ),
		m_wnd6( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CAttrMiddleLineDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrMiddleLineDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrMiddleLineDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrMiddleLineDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrMiddleLineDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrMiddleLineDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_COMBO_LINETYPE) );
	}

    return bRet;
}

/**
 * @fn void CAttrMiddleLineDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrMiddleLineDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// ミドルライン用

		// スケール１表示フラグ
		m_cControlRel.SetControl( IDC_CHECK_SC1,	TYPE_CHECKBOX,	0,	0,	_T("DISPSCALE1_F"),		NULL, MIDDLE_LINE_TABLE_NAME );
		// スケール２表示フラグ
		m_cControlRel.SetControl( IDC_CHECK_SC2,	TYPE_CHECKBOX,	0,	0,	_T("DISPSCALE2_F"),		NULL, MIDDLE_LINE_TABLE_NAME );
		// スケール３表示フラグ
		m_cControlRel.SetControl( IDC_CHECK_SC3,	TYPE_CHECKBOX,	0,	0,	_T("DISPSCALE3_F"),		NULL, MIDDLE_LINE_TABLE_NAME );
		// スケール４表示フラグ
		m_cControlRel.SetControl( IDC_CHECK_SC4,	TYPE_CHECKBOX,	0,	0,	_T("DISPSCALE4_F"),		NULL, MIDDLE_LINE_TABLE_NAME );
		// 地下フラグ
		m_cControlRel.SetControl( IDC_CHECK_UG_F,	TYPE_CHECKBOX,	0,	0,	_T("UNDERGROUND_F"),	NULL, MIDDLE_LINE_TABLE_NAME );
		// 背景ライン種別フラグ
		m_cControlRel.SetControl( IDC_COMBO_LINETYPE,	TYPE_COMBO,		0,	0,	_T("BGCLASS_C"),		NULL, MIDDLE_LINE_TABLE_NAME );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("SOURCE"),			NULL, MIDDLE_LINE_TABLE_NAME );
	}
}
