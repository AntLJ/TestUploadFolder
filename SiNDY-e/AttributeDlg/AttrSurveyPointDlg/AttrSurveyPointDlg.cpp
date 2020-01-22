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

// AttrSurveyPointDlg.cpp : CAttrSurveyPointDlg のインプリメンテーション

#include "stdafx.h"
#include "AttrSurveyPointDlg.h"

/// ウィンドウマネージャ配置マップ（現地調査ポイント用）
BEGIN_WINDOW_MAP(SURVEYPOINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NODEID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NODEID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MASTERID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_MASTERID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MESHCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_MESHCODE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LOTNO)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_LOTNO)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_WORKYEAR)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_WORKYEAR)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_WORKSEASON)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_WORKSEASON)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SURVEYLV)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SURVEYLV)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_PRIMID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_PRIMID)
			ENDGROUP()
			RCSPACE(20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_DUMMY)
				RCTOFIT(-1)
				RCREST(IDC_BUTTON_DISPLAYPICTURE)
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
 * @retval CAttrSurveyPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrSurveyPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSurveyPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrSurveyPointDlg::CAttrSurveyPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrSurveyPointDlg::CAttrSurveyPointDlg() 
	: ATTR_BASE_CLASS(SURVEYPOINT_MAP, IDD_ATTRSURVEYPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_NODEID ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_MASTERID ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_MESHCODE ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_LOTNO ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_WORKYEAR ),
		m_wnd5( _T("COMBO"), this, IDC_COMBO_WORKSEASON ),
		m_wnd6( _T("COMBO"), this, IDC_COMBO_SURVEYLV ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_PRIMID ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd9( _T("BUTTON"), this, IDC_BUTTON_DISPLAYPICTURE )
{
}

/** 
 * @fn HWND CAttrSurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrSurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrSurveyPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrSurveyPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrSurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrSurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_EDIT_NODEID) );
	}

    return bRet;
}

/**
 * @fn void CAttrBaseAdminDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrSurveyPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// 現地調査ポイント用

		// ノードID
		m_cControlRel.SetControl( IDC_EDIT_NODEID,		TYPE_EDIT,		0,	0,  _T("NODEID"),		NULL, SURVEY_POINT_TABLE_NAME );
		// 交差原票ID
		m_cControlRel.SetControl( IDC_EDIT_MASTERID,	TYPE_EDIT,		0,	0,  _T("MASTERID"),	NULL, SURVEY_POINT_TABLE_NAME );
		// メッシュコード
		m_cControlRel.SetControl( IDC_EDIT_MESHCODE,	TYPE_EDIT,		0,	0,  _T("MESHCODE"),	NULL, SURVEY_POINT_TABLE_NAME );
		// ロットNO
		m_cControlRel.SetControl( IDC_EDIT_LOTNO,		TYPE_EDIT,		0,	0,  _T("LOTNO"),		NULL, SURVEY_POINT_TABLE_NAME );
		// 作業年度
		m_cControlRel.SetControl( IDC_EDIT_WORKYEAR,	TYPE_EDIT,		0,	0,  _T("WORKYEAR"),	NULL, SURVEY_POINT_TABLE_NAME );
		// 作業時期
		m_cControlRel.SetControl( IDC_COMBO_WORKSEASON,	TYPE_COMBO,		0,	0,  _T("WORKSEASON"),	NULL, SURVEY_POINT_TABLE_NAME );
		// 現調レベル
		m_cControlRel.SetControl( IDC_COMBO_SURVEYLV,	TYPE_COMBO,		0,	0,  _T("SURVEYLV"),	NULL, SURVEY_POINT_TABLE_NAME );
		// プリミティブID
		m_cControlRel.SetControl( IDC_EDIT_PRIMID,		TYPE_EDIT,		0,	0,  _T("TMP_PRIMID"),	NULL, SURVEY_POINT_TABLE_NAME );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,  _T("SOURCE"),		NULL, SURVEY_POINT_TABLE_NAME );
	}
}

void CAttrSurveyPointDlg::GetRegCode()
{
 	CString strRegOpenKey( _T("SOFTWARE\\INCREMENT P CORPORATION\\SiNDY-e\\TOOLS\\ATTRIBUTETOOL" ) );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		TCHAR strToolPath[255];

		if( EmxGetRegString( _T("SurveyToolPath"), strToolPath, 255, hOpenKey ) )
			m_strToolPath = strToolPath;

		RegCloseKey( hOpenKey );
	}
}
