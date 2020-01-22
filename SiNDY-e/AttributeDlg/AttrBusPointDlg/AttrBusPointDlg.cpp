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

//////////////////////////////////////////////////////////////////////
//
// AttrBusPointDlg.cpp: CAttrBusPointDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrBusPointDlg.h"
#include "AheLayerFunctions.h"
#include "AheGlobals.h"
#include "AheDraw.h"
#include "DrawFunctions.h"

/// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(BUSPOINTMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_STOP_ID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_STOP_ID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_POINT_ATTR_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_ATTR)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_POLE_NO)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_POLE_NO)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_YOMI)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_YOMI)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_SOURCE)
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
 * @retval CAttrBusPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrBusPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBusPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBusPointDlg::CAttrBusPointDlg()
 * @brief コンストラクタ
 *
 */
CAttrBusPointDlg::CAttrBusPointDlg() 
	: ATTR_BASE_CLASS(BUSPOINTMAP, IDD_ATTRBUSPOINTDLG),
		m_wnd0( _T( "EDIT" ), this, IDC_EDIT_STOP_ID ),
		m_wnd1( _T( "COMBOBOX" ), this, IDC_COMBO_ATTR ),
		m_wnd2( _T( "EDIT" ), this, IDC_EDIT_POLE_NO ),
		m_wnd3( _T( "EDIT" ), this, IDC_EDIT_NAME ),
		m_wnd4( _T( "EDIT" ), this, IDC_EDIT_YOMI ),
		m_wnd5( _T( "EDIT" ), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CAttrBusPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrBusPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create( hWndParent, dwInitParam );
	return m_hChildWnd;
}

/**
 * @fn void CAttrBusPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrBusPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow( m_hChildWnd );
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrBusPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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

BOOL CAttrBusPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );
	
	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		SetIMEMode( FALSE );
		::SetFocus( GetDlgItem( IDC_EDIT_STOP_ID ) );
	}

    return bRet;
}

/**
 * @fn void CAttrBusPointDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrBusPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() );

	if( !m_cControlRel.empty() )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// 停留所コード
		m_cControlRel.SetControl( IDC_EDIT_STOP_ID, TYPE_EDIT, 0, 0, _T( "Stop_ID" ), NULL, _T( "BUS_POINT" )  );
		// 種別
		m_cControlRel.SetControl( IDC_COMBO_ATTR, TYPE_COMBO, 0, 0, _T( "Point_Attr_C" ), NULL, _T( "BUS_POINT" ) );
		// 標柱番号
		m_cControlRel.SetControl( IDC_EDIT_POLE_NO, TYPE_EDIT, 0, 0, _T( "Pole_No" ), NULL, _T( "BUS_POINT" ) );
		// 停留所名
		m_cControlRel.SetControl( IDC_EDIT_NAME, TYPE_EDITSTRING, 0, 0, _T( "Stop_Name_Kanji" ), NULL, _T( "BUS_POINT" ) );
		// 停留所よみ
		m_cControlRel.SetControl( IDC_EDIT_YOMI, TYPE_EDITSTRING, 0, 0, _T( "Stop_Name_Yomi" ), NULL, _T( "BUS_POINT" ) );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, _T( "SOURCE" ), NULL, _T( "BUS_POINT" ) );
	}
}


