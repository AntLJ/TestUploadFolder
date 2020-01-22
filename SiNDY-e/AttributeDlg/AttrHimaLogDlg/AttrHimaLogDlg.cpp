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

// AttrHimaLogDlg.cpp: CAttrHimaLogDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrHimaLogDlg.h"

/// ウィンドウマネージャ配置マップ（ひまわりログ用）
BEGIN_WINDOW_MAP(HIMALOG_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_TOFIT,0,0)
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LOG)
				RCTOFIT(IDC_EDIT_LOG)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_WORKER)
				RCTOFIT(IDC_EDIT_WORKER)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SHEETID)
				RCTOFIT(IDC_EDIT_SHEETID)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_KIGOU)
				RCTOFIT(IDC_EDIT_KIGOU)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENTYOU_C)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENTYOU_C)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENTYOU_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENTYOU_NAME)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KIZON_C)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_KIZON_C)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KIZON_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_KIZON_NAME)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_DISP_C)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_DISP_C)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_DISP_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_DISP_NAME)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_TDCSTATUS)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_TDCSTATUS)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_RESULT)
				RCTOFIT(IDC_EDIT_RESULT)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_MATCHING)
				RCTOFIT(IDC_EDIT_MATCHING)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_RESULT_SHA)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_RESULT_SHA)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_REST, 0, RCMARGINS(4,4))
			RCTOFIT(IDC_STATIC_REMARKS)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_REMARKS)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrHimaLogDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrHimaLogDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHimaLogDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrHimaLogDlg::CAttrHimaLogDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrHimaLogDlg::CAttrHimaLogDlg() 
	: ATTR_BASE_CLASS(HIMALOG_MAP, IDD_ATTRHIMALOGDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_LOG ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_WORKER ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_SHEETID ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_KIGOU ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_GENTYOU_C ),
		m_wnd5( _T("EDIT"), this, IDC_EDIT_GENTYOU_NAME ),
		m_wnd6( _T("EDIT"), this, IDC_EDIT_KIZON_C ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_KIZON_NAME ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_DISP_C ),
		m_wnd9( _T("EDIT"), this, IDC_EDIT_DISP_NAME ),
		m_wnd10( _T("EDIT"), this, IDC_EDIT_TDCSTATUS ),
		m_wnd11( _T("EDIT"), this, IDC_EDIT_RESULT ),
		m_wnd12( _T("EDIT"), this, IDC_EDIT_MATCHING ),
		m_wnd13( _T("EDIT"), this, IDC_EDIT_RESULT_SHA ),
		m_wnd14( _T("EDIT"), this, IDC_EDIT_REMARKS )
{
	// フォント作成
	m_hFont1 = CreateFont(
		12, 0, 0, 0, 100,                           // 高さ，幅，Ｘ角度，Ｙ角度，太さ
		FALSE, TRUE, FALSE,                        // 斜体，下線，打消線
		SHIFTJIS_CHARSET,                           // キャラクタ設定
		OUT_DEFAULT_PRECIS,                         // 出力精度
		CLIP_DEFAULT_PRECIS,                        // クリップ精度
		DEFAULT_QUALITY,                            // 品質
		FIXED_PITCH | FF_DONTCARE,                // ピッチ
		_T("ＭＳＰゴシック"));                       // フォント種類

	m_hFont2 = CreateFont(
		18, 0, 0, 0, 100,                           // 高さ，幅，Ｘ角度，Ｙ角度，太さ
		FALSE, FALSE, FALSE,                        // 斜体，下線，打消線
		SHIFTJIS_CHARSET,                           // キャラクタ設定
		OUT_DEFAULT_PRECIS,                         // 出力精度
		CLIP_DEFAULT_PRECIS,                        // クリップ精度
		DEFAULT_QUALITY,                            // 品質
		FIXED_PITCH | FF_DONTCARE,                // ピッチ
		_T("ＭＳＰゴシック"));                       // フォント種類

}

/** 
 * @fn HWND CAttrHimaLogDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrHimaLogDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrHimaLogDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrHimaLogDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrHimaLogDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrHimaLogDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_EDIT_LOG) );
	}

    return bRet;
}

/**
 * @fn void CAttrHimaLogDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrHimaLogDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// ひまわりログ

		// ログ
		m_cControlRel.SetControl( IDC_EDIT_LOG,			TYPE_EDITSTRING,0,	0,	_T("LOG"),			NULL, HIMA_LOG_TABLE_NAME );
		// 作業者名
		m_cControlRel.SetControl( IDC_EDIT_WORKER,		TYPE_EDITSTRING,0,	0,	_T("W_NAME"),		NULL, HIMA_LOG_TABLE_NAME );
		// シートID
		m_cControlRel.SetControl( IDC_EDIT_SHEETID,		TYPE_EDITSTRING,0,	0,	_T("SHEETID"),		NULL, HIMA_LOG_TABLE_NAME );
		// 記号
		m_cControlRel.SetControl( IDC_EDIT_KIGOU,		TYPE_EDITSTRING,0,	0,	_T("KIGOU"),		NULL, HIMA_LOG_TABLE_NAME );
		// 現調種別
		m_cControlRel.SetControl( IDC_EDIT_GENTYOU_C,	TYPE_EDITSTRING,0,	0,	_T("CHOUSA_CODE"),	NULL, HIMA_LOG_TABLE_NAME );
		// 現調名称
		m_cControlRel.SetControl( IDC_EDIT_GENTYOU_NAME,TYPE_EDITSTRING,0,	0,	_T("GENCHO_NAME"),	NULL, HIMA_LOG_TABLE_NAME );
		// 既存種別
		m_cControlRel.SetControl( IDC_EDIT_KIZON_C,		TYPE_EDITSTRING,0,	0,	_T("KIZON_CODE"),	NULL, HIMA_LOG_TABLE_NAME );
		// 既存名称
		m_cControlRel.SetControl( IDC_EDIT_KIZON_NAME,	TYPE_EDITSTRING,0,	0,	_T("KIZON_NAME"),	NULL, HIMA_LOG_TABLE_NAME );
		// 表記修正種別
		m_cControlRel.SetControl( IDC_EDIT_DISP_C,		TYPE_EDITSTRING,0,	0,	_T("MODIFY_COD"),	NULL, HIMA_LOG_TABLE_NAME );
		// 表記修正名称
		m_cControlRel.SetControl( IDC_EDIT_DISP_NAME,	TYPE_EDITSTRING,0,	0,	_T("MODIFY_NAME"),	NULL, HIMA_LOG_TABLE_NAME );
		// TDC_STATUS
		m_cControlRel.SetControl( IDC_EDIT_TDCSTATUS,	TYPE_EDITSTRING,0,	0,	_T("TDC_STATUS"),	NULL, HIMA_LOG_TABLE_NAME );
		// 対応結果
		m_cControlRel.SetControl( IDC_EDIT_RESULT,		TYPE_EDITSTRING,0,	0,	_T("RESULT"),		NULL, HIMA_LOG_TABLE_NAME );
		// 一致判定
		m_cControlRel.SetControl( IDC_EDIT_MATCHING,	TYPE_EDITSTRING,0,	0,	_T("MATCHING"),		NULL, HIMA_LOG_TABLE_NAME );
		// 平面対応結果
		m_cControlRel.SetControl( IDC_EDIT_RESULT_SHA,	TYPE_EDITSTRING,0,	0,	_T("RESULT_SHAPE"),	NULL, HIMA_LOG_TABLE_NAME );
		// 備考
		m_cControlRel.SetControl( IDC_EDIT_REMARKS,		TYPE_EDITSTRING,0,	0,	_T("MEMO_"),		NULL, HIMA_LOG_TABLE_NAME );
	}
}
