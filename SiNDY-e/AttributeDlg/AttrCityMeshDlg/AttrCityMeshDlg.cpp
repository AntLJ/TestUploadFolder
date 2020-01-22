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

// AttrCitySiteDlg.cpp: CAttrCitySiteDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrCityMeshDlg.h"

/// ウィンドウマネージャ配置マップ（都市地図メッシュ用）
BEGIN_WINDOW_MAP(CITYMESH_MAP)
	BEGINROWS(WRCT_TOFIT,0,0)
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CITYMESH_STATIC1)
			RCTOFIT(-1)
			RCREST(IDC_CITYMESH_EDIT1)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CITYMESH_STATIC2)
			RCTOFIT(-1)
			RCREST(IDC_CITYMESH_EDIT2)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CITYMESH_STATIC3)
			RCTOFIT(-1)
			RCREST(IDC_CITYMESH_EDIT3)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CITYMESH_STATIC4)
			RCTOFIT(-1)
			RCREST(IDC_CITYMESH_EDIT4)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(-1)
			RCTOFIT(IDC_CITYMESH_CHECK1)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CITYMESH_STATIC5)
			RCTOFIT(-1)
			RCREST(IDC_CITYMESH_EDIT5)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()


/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrCityMeshDlgを返します。
 */
 extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrCityMeshDlg();
}

/** 
 * @fn CAttrCityMeshDlg::CAttrCityMeshDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrCityMeshDlg::CAttrCityMeshDlg()
	: ATTR_BASE_CLASS(CITYMESH_MAP, IDD_ATTRCITYMESHDLG),
		m_wnd0( _T("EDIT"), this, IDC_CITYMESH_EDIT1 ),
		m_wnd1( _T("EDIT"), this, IDC_CITYMESH_EDIT2 ), 
		m_wnd2( _T("EDIT"), this, IDC_CITYMESH_EDIT3 ), 
		m_wnd3( _T("EDIT"), this, IDC_CITYMESH_EDIT4 ), 
		m_wnd4( _T("CHECKBOX"), this, IDC_CITYMESH_CHECK1 ), 
		m_wnd5( _T("EDIT"), this, IDC_CITYMESH_EDIT5 )
{
}

/** 
 * @fn HWND CAttrCityMeshDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrCityMeshDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCityMeshDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrCityMeshDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrCityMeshDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCityMeshDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

    return bRet;
}

/**
 * @fn void CAttrCityMeshDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。各コントロールに値を設定したりします。
 */
void CAttrCityMeshDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// 都市地図メッシュ用

		// 都市地図メッシュコード
		m_cControlRel.SetControl( IDC_CITYMESH_EDIT1,	TYPE_EDIT,			0,	0,	_T("MESHCODE"),				NULL, CITYMESH_TABLE_NAME );
		// 新規作成年度
		m_cControlRel.SetControl( IDC_CITYMESH_EDIT2,	TYPE_EDITSTRING,	0,	0,	_T("CREATE_YEAR"),				NULL, CITYMESH_TABLE_NAME );
		// 現況修正年月
		m_cControlRel.SetControl( IDC_CITYMESH_EDIT3,	TYPE_EDITSTRING,	0,	0,	_T("MODIFY_YEAR"),			NULL, CITYMESH_TABLE_NAME );
		// 注記現調年月
		m_cControlRel.SetControl( IDC_CITYMESH_EDIT4,	TYPE_EDITSTRING,	0,	0,	_T("ANNO_SURVEY_YEAR"),			NULL, CITYMESH_TABLE_NAME );
		// 完全メッシュフラグ
		m_cControlRel.SetControl( IDC_CITYMESH_CHECK1,	TYPE_CHECKBOX,		0,	0,	_T("COMPLETE_F"),			NULL, CITYMESH_TABLE_NAME );
		// 規制現地調査年度
		m_cControlRel.SetControl( IDC_CITYMESH_EDIT5,	TYPE_EDITSTRING,	0,	0,	_T("TR_SURVEY_YEAR"),	NULL, CITYMESH_TABLE_NAME );
	}
}
