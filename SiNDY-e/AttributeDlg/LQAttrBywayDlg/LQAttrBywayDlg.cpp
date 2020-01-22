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

// LQAttrBywayDlg.cpp : CLQAttrBywayDlg のインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrBywayDlg.h"

/// ウィンドウマネージャ配置マップ（抜け道用）
BEGIN_WINDOW_MAP(BYWAY_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_CHECK_NUKEMITI)
		ENDGROUP()
		RCSPACE(7)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_CHECK_URAMITI)
		ENDGROUP()
		RCSPACE(7)
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
 * @retval CLQAttrBywayDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrBywayDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrBywayDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CLQAttrDefaultDlg::CLQAttrDefaultDlg()
 * @brief コンストラクタ
 *
 * なにもしません
 */
CLQAttrBywayDlg::CLQAttrBywayDlg() 
	: LQATTR_COMMON_BASE_CLASS(BYWAY_MAP, IDD_LQATTRBYWAYDLG),
	m_wndNuke( _T("CHECKBOX"), this, IDC_CHECK_NUKEMITI ),
	m_wndUra( _T("CHECKBOX"), this, IDC_CHECK_URAMITI ),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE ),
	m_wndSourceName( _T("EDIT"), this, IDC_EDIT_SOURCENAME2 )
{
}

/** 
 * @fn HWND CLQAttrBywayDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CLQAttrBywayDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// ダイアログの作成はLQATTR_COMMON_BASE_CLASSに任せます
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrBywayDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CLQAttrBywayDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrBywayDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CLQAttrBywayDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
		::SetFocus( GetDlgItem(IDC_CHECK_NUKEMITI) );
	else
		::SendMessage( GetDlgItem(IDC_CHECK_NUKEMITI), WM_KILLFOCUS, NULL, NULL );

    return bRet;
}

/**
 * @fn void CLQAttrBywayDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CLQAttrBywayDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		CString strInfTableName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueByway, sindyeLoadQueueInfoInfoTableName );
		m_cControlRel.SetHWND( m_hWnd );
		// 抜け道用
		m_cControlRel.SetControl( IDC_CHECK_NUKEMITI,		TYPE_CHECKBOX,		0, 0,	BYWAY_F_FIELD,		NULL, _T("INF_BYWAY") );
		m_cControlRel.SetControl( IDC_CHECK_URAMITI,		TYPE_CHECKBOX,		0, 0,	BACKROAD_F_FIELD,	NULL, _T("INF_BYWAY") );
		m_cControlRel.SetControl( IDC_EDIT_SOURCENAME2,		TYPE_EDITSTRING,	0, 0,	_T("SOURCE_NAME"),	NULL, _T("INF_BYWAY") );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,	0, 0,	_T("SOURCE"),		NULL, _T("INF_BYWAY") );
	}
}

BOOL CLQAttrBywayDlg::ErrorCheck()
{
	BOOL bRet = TRUE;
	CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueByway, sindyeLoadQueueInfoInfoTableName );
	CString strMsg;
	for( std::list<CLQRowDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		if( strInfTableName.CompareNoCase( it->GetTableName() ) == 0 )
		{
			CFieldDef* pOIDFieldDef = it->GetFieldDef( strInfTableName, _T("OBJECTID"));
			CFieldDef* pBywayFieldDef = it->GetFieldDef( strInfTableName, _T("BYWAY_F"));
			CFieldDef* pBackroadFieldDef = it->GetFieldDef( strInfTableName, _T("BACKROAD_F"));
			long lOID = pOIDFieldDef->m_vaValue.lVal;
			long lByway = pBywayFieldDef->m_vaValue.lVal;
			long lBackroad = pBackroadFieldDef->m_vaValue.lVal;

			if( lByway == 0 && lBackroad == 0 )
			{
				strMsg.Format(AheLoadString( IDS_BYWAYERROR ), strInfTableName, lOID );
				AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

				bRet = FALSE;
				break;
			}
		}
	}
	return bRet;
}
