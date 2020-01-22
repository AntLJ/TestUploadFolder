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

// AttrRoadNodeDlg.cpp: CAttrRoadNodeDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrRoadNodeDlg.h"

/// ウィンドウマネージャ配置マップ（ノード用）
BEGIN_WINDOW_MAP(ROADNODE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_WORKYEAR_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_WORKYEAR_EDIT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_NODEKINDS_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_NODEKINDS_COMBO)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SIGNAL_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SIGNAL)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KANJI_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_KANJI_EDIT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KANA_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_KANA_EDIT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_HEIGHTFLAG_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_HEIGHTFLAG_COMBO)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_HEIGHT_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_HEIGHT_EDIT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_PARKINGID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_PARKINGID)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT, 0, 10)
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
 * @retval CAttrRoadNodeDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrRoadNodeDlg();
}
/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadNodeDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrRoadNodeDlg::CAttrRoadNodeDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化、フォント作成をしておきます。
 */
CAttrRoadNodeDlg::CAttrRoadNodeDlg()
	: ATTR_BASE_CLASS(ROADNODE_MAP, IDD_ATTRROADNODEDLG),
		m_wnd0( _T("EDIT"), this, IDC_WORKYEAR_EDIT ), 
		m_wnd1( _T("COMBOBOX"), this, IDC_NODEKINDS_COMBO ), 
		m_wnd2( _T("COMBOBOX"), this, IDC_COMBO_SIGNAL ), 
		m_wnd3( _T("EDIT"), this, IDC_KANJI_EDIT ), 
		m_wnd4( _T("EDIT"), this, IDC_KANA_EDIT ), 
		m_wnd5( _T("COMBOBOX"), this, IDC_HEIGHTFLAG_COMBO ), 
		m_wnd6( _T("EDIT"), this, IDC_HEIGHT_EDIT ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_PARKINGID ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
	// フォント作成
	m_hFont = CreateFont(
		18, 0, 0, 0, 100,		// 高さ，幅，Ｘ角度，Ｙ角度，太さ
		FALSE, FALSE, FALSE,    // 斜体，下線，打消線
		SHIFTJIS_CHARSET,       // キャラクタ設定
		OUT_DEFAULT_PRECIS,     // 出力精度
		CLIP_DEFAULT_PRECIS,    // クリップ精度
		DEFAULT_QUALITY,        // 品質
		FIXED_PITCH | FF_DONTCARE,// ピッチ
		AheLoadString(IDS_FONT));   // フォント種類
}

/** 
 * @fn HWND CAttrRoadNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrRoadNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrRoadNodeDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrRoadNodeDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrRoadNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrRoadNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_WORKYEAR_EDIT) );
	}

	SetToolTip(IDC_KANJI_EDIT);
	SetToolTip(IDC_KANA_EDIT);

	return bRet;
}

/**
 * @fn void CAttrRoadNodeDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrRoadNodeDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// 道路ノード用
		m_cControlRel.SetControl( IDC_WORKYEAR_EDIT,	TYPE_EDIT,		0,	0,	_T("WORK_YEAR"),	NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_NODEKINDS_COMBO,	TYPE_COMBO,		0,	0,	_T("NODECLASS_C"),	NULL, NODE_TABLE_NAME , NULL, -1, FOLLOW_READONLY );
		m_cControlRel.SetControl( IDC_COMBO_SIGNAL,		TYPE_COMBO,		0,	0,	_T("SIGNAL_C"),		NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KANJI_EDIT,		TYPE_EDITSTRING,0,	0,	_T("NAME_KANJI"),	NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KANA_EDIT,		TYPE_EDITSTRING,0,	0,	_T("NAME_YOMI"),	NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_HEIGHTFLAG_COMBO,	TYPE_COMBO,		0,	0,	_T("HEIGHT_C"),		NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_HEIGHT_EDIT,		TYPE_EDIT,		0,	0,	_T("HEIGHT"),		NULL, NODE_TABLE_NAME, _T("LINK_ID"), IDC_HEIGHTFLAG_COMBO, FOLLOW_VALUEIS1 );
		m_cControlRel.SetControl( IDC_EDIT_PARKINGID,	TYPE_EDIT,		0,	0,	_T("PARKING_ID"),	NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, NODE_TABLE_NAME );
	}
}

/**
 * @fn void CAttrRoadNodeDlg::SetToolTip(INT nTargetItem)
 * @brief ツールチップを作成します。
 *
 * @note 作成するツールチップは属性に変更がかからないため、作りっぱなしで問題なし
 *
 * @param nTargetItem	[in]	ツールチップを作成するコントロールアイテムのID
*/
void CAttrRoadNodeDlg::SetToolTip(INT nTargetItem)
{
	// エディットボックスにかかれている文字列を取得
	CEdit edit( GetDlgItem(nTargetItem) );
	INT nTextLen = edit.GetWindowTextLength() + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	edit.GetWindowText( lpszText, nTextLen );

	// まだツールチップ作成されていなかったら作成する（一番最初のみ入ってくる）
	HWND hToolTip;
	hToolTip =
		CreateWindowEx(
		0,				//拡張ウィンドウスタイル
		TOOLTIPS_CLASS,	//クラスネーム
		NULL,			//ウィンドウネーム
		TTS_ALWAYSTIP,	// ウィンドウスタイル
		CW_USEDEFAULT,	// Ｘ座標
		CW_USEDEFAULT,	// Ｙ座標
		CW_USEDEFAULT,	// 幅
		CW_USEDEFAULT,	// 高さ
		GetDlgItem(nTargetItem),// 親ウィンドウのハンドル
		NULL,			// メニューハンドル
		NULL,			// インスタンスハンドル
		NULL);			// WM_CREATEデータ

	// ToolInfo構造体 
	static TOOLINFO ti;
	ZeroMemory(&ti , sizeof(TOOLINFO)) ; 
	ti.cbSize = sizeof(TOOLINFO) ; 
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS ; 
	ti.hwnd = GetDlgItem(nTargetItem);
	ti.uId =  (UINT_PTR)GetDlgItem(nTargetItem).m_hWnd;

	ti.lpszText = lpszText;

	::SendMessage( hToolTip, TTM_ADDTOOL , 0 , (LPARAM)&ti);
	::SendMessage( hToolTip, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)300);
	::SendMessage( hToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);

	CWindow cWnd( hToolTip );
	cWnd.SetFont( m_hFont );
}

/**
 * @fn BOOL CAttrRoadNodeDlg::ErrorCheck()
 * @brief ダイアログが閉じられるときに道路ノードのエラーチェックを行う
 *
 * @return エラーでなかったら TRUE、エラーだったら FALSE
 */
BOOL CAttrRoadNodeDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == NODE_TABLE_NAME )
		{

			// 現在編集中の値を習得
			CFieldDef* pFieldDef;
			CComVariant vaName_Kanji, vaName_Yomi;
			pFieldDef = it->GetFieldDef( NODE_TABLE_NAME, _T("NAME_KANJI") );
			vaName_Kanji = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( NODE_TABLE_NAME, _T("NAME_YOMI") );
			vaName_Yomi = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID;
			it->GetFeature()->get_OID( &lOID );

			if( vaName_Kanji.vt != VT_NULL && vaName_Yomi.vt == VT_NULL )
			{
				strMessage.Format( AheLoadString( IDS_ERRORKANA ) ,lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else if( vaName_Kanji.vt == VT_NULL && vaName_Yomi.vt != VT_NULL )
			{
				strMessage.Format( AheLoadString( IDS_ERRORKANA ) ,lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}

