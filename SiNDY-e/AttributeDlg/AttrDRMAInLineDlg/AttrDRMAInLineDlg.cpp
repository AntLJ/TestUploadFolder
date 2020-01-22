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

// AttrDRMAInLineDlg.cpp: CAttrDRMAInLineDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrDRMAInLineDlg.h"

/// ウィンドウマネージャ配置マップ（DRMAリンク内属性用）
BEGIN_WINDOW_MAP(DRMAINLINE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LINKINC)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LINKINC)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LINK_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LINK_C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KANJINAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_KANJINAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KANANAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_KANANAME)
			ENDGROUP()
			RCSPACE(10)
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrDRMAInLineDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrDRMAInLineDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDRMAInLineDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrDRMAInLineDlg::CAttrDRMAInLineDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrDRMAInLineDlg::CAttrDRMAInLineDlg()
	: ATTR_BASE_CLASS(DRMAINLINE_MAP, IDD_ATTRDRMAINLINEDLG)
{
}

/** 
 * @fn HWND CAttrDRMAInLineDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrDRMAInLineDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrDRMAInLineDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrDRMAInLineDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrDRMAInLineDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrDRMAInLineDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );
    return bRet;
}

/**
 * @fn void CAttrDRMAInLineDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrDRMAInLineDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {

		m_cControlRel.SetHWND( m_hWnd );

		// DRMAリンク内属性orリンク内ポイント道路

		// リンク内種別
		m_cControlRel.SetControl( IDC_COMBO_LINKINC,	TYPE_COMBO,		0,	0,	_T("LINKATTRCODE"),		NULL, m_strFeatureClassName);
		// リンクタイプ
		m_cControlRel.SetControl( IDC_COMBO_LINK_C,		TYPE_COMBO,		0,	0,	_T("LINKTYPE"),			NULL, m_strFeatureClassName);
		// 
		m_cControlRel.SetControl( IDC_EDIT_KANJINAME,	TYPE_EDITSTRING,0,	0,	_T("LV1_KANJI_NAME"),	NULL, m_strFeatureClassName);
		m_cControlRel.SetControl( IDC_EDIT_KANANAME,	TYPE_EDITSTRING,0,	0,	_T("LV1_KANA_NAME"),	NULL, m_strFeatureClassName);
	}
}
