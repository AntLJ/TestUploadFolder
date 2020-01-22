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

// LQAttrUturnDlg.cpp: CLQAttrUturnDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrUturnDlg.h"

/// ウィンドウマネージャ配置マップ（Uターン用）
BEGIN_WINDOW_MAP(UTURN_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_SOURCENAME2)
			RCREST(IDC_EDIT_SOURCENAME2)
		ENDGROUP()
		RCSPACE(7)
		RCREST(-1)
		RCTOFIT(IDC_STATIC_SOURCESEP)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_SOURCE)
			RCREST(IDC_EDIT_SOURCE)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CLQAttrUturnDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrUturnDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrUturnDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////
/** 
 * @fn CLQAttrUturnDlg::CLQAttrUturnDlg()
 * @brief コンストラクタ
 *
 * なにもしません
 */
CLQAttrUturnDlg::CLQAttrUturnDlg() 
	: LQATTR_COMMON_BASE_CLASS(UTURN_MAP, IDD_LQATTRUTURNDLG),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE ),
	m_wndSourceName( _T("EDIT"), this, IDC_EDIT_SOURCENAME2 )
{
}
/** 
 * @fn HWND CLQAttrUturnDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CLQAttrUturnDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// ダイアログの作成はLQATTR_COMMON_BASE_CLASSに任せます
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrUturnDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CLQAttrUturnDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrUturnDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CLQAttrUturnDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	else
		::SendMessage( GetDlgItem(IDC_EDIT_SOURCE), WM_KILLFOCUS, NULL, NULL );

    return bRet;
}

/**
 * @fn void CLQAttrUturnDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CLQAttrUturnDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		CString strInfoTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueUTurn, sindyeLoadQueueInfoInfoTableName );
		m_cControlRel.SetHWND( m_hWnd );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,	0, 0,	_T("SOURCE"),		NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_EDIT_SOURCENAME2,	TYPE_EDITSTRING,	0, 0,	_T("SOURCE_NAME"),	NULL, strInfoTableName );
	}
}
