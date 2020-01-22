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

// AttrKLinkPointDlg.cpp: CAttrKLinkPointDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrKLinkPointDlg.h"

/// ウィンドウマネージャ配置マップ（家形リンクポイント用）
BEGIN_WINDOW_MAP(KLINK_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(4)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KLINK_POINT_STATIC2)
				RCTOFIT(-1)
				RCREST(IDC_KLINK_POINT_EDIT2)
			ENDGROUP()
			RCSPACE(4)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KLINK_POINT_STATIC3)
				RCTOFIT(-1)
				RCREST(IDC_KLINK_POINT_EDIT3)
			ENDGROUP()
			RCSPACE(4)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KLINK_POINT_STATIC4)
				RCTOFIT(-1)
				RCREST(IDC_KLINK_POINT_EDIT4)
			ENDGROUP()
			RCSPACE(4)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KLINK_POINT_STATIC5)
				RCTOFIT(-1)
				RCREST(IDC_KLINK_POINT_EDIT5)
			ENDGROUP()
			RCSPACE(4)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KLINK_POINT_STATIC6)
				RCTOFIT(-1)
				RCREST(IDC_KLINK_POINT_COMBO1)
			ENDGROUP()
			RCSPACE(4)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KLINK_POINT_STATIC7)
				RCTOFIT(-1)
				RCREST(IDC_KLINK_POINT_COMBO2)
			ENDGROUP()
			RCSPACE(4)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KLINK_POINT_STATIC8)
				RCTOFIT(-1)
				RCREST(IDC_KLINK_POINT_COMBO3)
			ENDGROUP()
			RCSPACE(4)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KLINK_POINT_STATIC9)
				RCTOFIT(-1)
				RCREST(IDC_KLINK_POINT_EDIT6)
			ENDGROUP()
			RCSPACE(4)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KLINK_POINT_STATIC10)
				RCTOFIT(-1)
				RCREST(IDC_KLINK_POINT_COMBO4)
			ENDGROUP()
			RCSPACE(4)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KLINK_POINT_STATIC11)
				RCTOFIT(-1)
				RCTOFIT(IDC_KLINK_POINT_EDIT7)
				RCTOFIT(-1)
				RCTOFIT(IDC_KLINK_POINT_STATIC12)
				RCTOFIT(-1)
				RCTOFIT(IDC_KLINK_POINT_EDIT8)
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
 * @retval CAttrKLinkPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrKLinkPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrKLinkPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrKLinkPointDlg::CAttrKLinkPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrKLinkPointDlg::CAttrKLinkPointDlg()
	: ATTR_BASE_CLASS(KLINK_POINT_MAP, IDD_ATTRKLINKPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_KLINK_POINT_EDIT2 ), 
		m_wnd1( _T("EDIT"), this, IDC_KLINK_POINT_EDIT3 ), 
		m_wnd2( _T("EDIT"), this, IDC_KLINK_POINT_EDIT4 ), 
		m_wnd3( _T("EDIT"), this, IDC_KLINK_POINT_EDIT5 ), 
		m_wnd4( _T("COMBOBOX"), this, IDC_KLINK_POINT_COMBO1 ), 
		m_wnd5( _T("COMBOBOX"), this, IDC_KLINK_POINT_COMBO2 ), 
		m_wnd6( _T("COMBOBOX"), this, IDC_KLINK_POINT_COMBO3 ),
		m_wnd7( _T("EDIT"), this, IDC_KLINK_POINT_EDIT6 ),
		m_wnd8( _T("COMBOBOX"), this, IDC_KLINK_POINT_COMBO4 ),
		m_wnd9( _T("EDIT"), this, IDC_KLINK_POINT_EDIT7 ),
		m_wnd10( _T("EDIT"), this, IDC_KLINK_POINT_EDIT8 ),
		m_wnd11( _T("EDIT"), this, IDC_EDIT_SOURCE )

{
}

/** 
 * @fn HWND CAttrKLinkPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrKLinkPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrKLinkPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrKLinkPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrKLinkPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrKLinkPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_KLINK_POINT_EDIT2) );
	}

    return bRet;
}

/**
 * @fn void CAttrKLinkPointDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrKLinkPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// 家形リンクポイント用
		m_cControlRel.SetControl( IDC_KLINK_POINT_EDIT2,	TYPE_EDITSTRING,	0,	0,	_T("ADDRCODE_ORG"),		NULL, KLINK_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KLINK_POINT_EDIT3,	TYPE_EDITSTRING,	0,	0,	_T("ADDRCODE"),			NULL, KLINK_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KLINK_POINT_EDIT4,	TYPE_EDITSTRING, 	0,	0,	_T("KATAGAKINAME"),		NULL, KLINK_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KLINK_POINT_EDIT5,	TYPE_EDITSTRING, 	0,	0,	_T("KEISAINAME"),		NULL, KLINK_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KLINK_POINT_COMBO1,	TYPE_COMBO,		 	0,	0,	_T("INFOSRC_C"),		NULL, KLINK_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KLINK_POINT_COMBO2,	TYPE_COMBO, 		0,	0,	_T("INPUTSEIDO_C"),		NULL, KLINK_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KLINK_POINT_COMBO3,	TYPE_COMBO, 		0,	0,	_T("RELEASESEIDO_C"),	NULL, KLINK_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KLINK_POINT_EDIT6,	TYPE_EDITSTRING, 	0,	0,	_T("BUILDNAME"),		NULL, KLINK_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KLINK_POINT_COMBO4,	TYPE_COMBO,		 	0,	0,	_T("BUILDINFOSRC_C"),	NULL, KLINK_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KLINK_POINT_EDIT7,	TYPE_EDIT,		 	0,	0,	_T("MAX_FLOORS"),		NULL, KLINK_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KLINK_POINT_EDIT8,	TYPE_EDIT,			0,	0,	_T("FLOOR"),			NULL, KLINK_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,	0,	0,	_T("SOURCE"),			NULL, KLINK_POINT_TABLE_NAME );
	}
}
