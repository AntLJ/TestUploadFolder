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

// LQAttrGuideDlg.cpp : CLQAttrGuideDlg のインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrGuideDlg.h"

/// ウィンドウマネージャ配置マップ（強制誘導用）
BEGIN_WINDOW_MAP(GUIDE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(20)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_GUIDECLASS)
			RCREST(IDC_COMBO_GUIDECLASS)
		ENDGROUP()
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
 * @retval CLQAttrGuideDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrGuideDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrGuideDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////
CLQAttrGuideDlg::CLQAttrGuideDlg() 
	: LQATTR_COMMON_BASE_CLASS(GUIDE_MAP, IDD_LQATTRGUIDEDLG),
	m_wndGuideClass( _T("COMBOBOX"), this, IDC_COMBO_GUIDECLASS ),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CLQAttrGuideDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CLQAttrGuideDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// ダイアログの作成はLQATTR_COMMON_BASE_CLASSに任せます
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrGuideDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CLQAttrGuideDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrGuideDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CLQAttrGuideDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
		::SetFocus( GetDlgItem(IDC_COMBO_GUIDECLASS) );
	else
		::SendMessage( GetDlgItem(IDC_COMBO_GUIDECLASS), WM_KILLFOCUS, NULL, NULL );

    return bRet;
}

/**
 * @fn void CLQAttrGuideDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CLQAttrGuideDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		CString strInfoTableName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueGuide, sindyeLoadQueueInfoInfoTableName );
		m_cControlRel.SetHWND( m_hWnd );
		// 強制誘導用
		m_cControlRel.SetControl( IDC_COMBO_GUIDECLASS,	TYPE_COMBO,			0, 0,	_T("GUIDETYPE_C"),	NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,	0, 0,	_T("SOURCE"),		NULL, strInfoTableName );
	}
}


BOOL CLQAttrGuideDlg::ErrorCheck()
{
	BOOL bRet = TRUE;
	CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueGuide, sindyeLoadQueueInfoInfoTableName );
	CString strMsg;
	for( std::list<CLQRowDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		if( strInfTableName.CompareNoCase( it->GetTableName() ) == 0 )
		{
			LONG lOID;
			CFieldDef* pOIDFieldDef = it->GetFieldDef( strInfTableName, _T("OBJECTID"));
			lOID = pOIDFieldDef->m_vaValue.lVal;

			CFieldDef* pGuideFieldDef = it->GetFieldDef( strInfTableName, _T("GUIDETYPE_C") );

			if( !pGuideFieldDef )
			{
				_ASSERTE( pGuideFieldDef != NULL );
				continue;
			}

			CString strMsg;
			// 案内種別コードに NULL が入っていないかどうか
			if( pGuideFieldDef->m_vaValue.lVal == 0 )
			{
				strMsg.Format( AheLoadString( IDS_ERRORGUIDE ), strInfTableName, lOID);
				AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

				bRet = FALSE;
				break;
			}
		}
	}
	
	return bRet;
}
