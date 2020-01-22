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

// AttrExTPPointDlg.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "AttrExTPPointDlg.h"


/// ウィンドウマネージャ配置マップ（2次メッシュ用）
BEGIN_WINDOW_MAP(EXTP_POINT_MAP)
	BEGINROWS(WRCT_TOFIT,0,0)
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_ADDRCODE)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_ADDRCODE)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_DUMMY1)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_ADDRNUM1)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_ADDR1)
			RCSPACE(10)
			RCTOFIT(IDC_STATIC_ADDRNUM2)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_ADDR2)
			RCSPACE(10)
			RCTOFIT(IDC_STATIC_ADDRNUM3)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_ADDR3)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_DUMMY2)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_ADDRNUM4)
			RCTOFIT(-1)
			RCREST(IDC_STATIC_ADDR4)
		ENDGROUP()
		RCSPACE(3)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_DUMMY3)
			RCTOFIT(-1)
			RCTOFIT(IDC_EDIT_ADDR1)
			RCSPACE(10)
			RCTOFIT(IDC_EDIT_ADDR2)
			RCSPACE(10)
			RCTOFIT(IDC_EDIT_ADDR3)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_DUMMY4)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_ADDR4)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_ADDRNAME)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_ADDRNAME)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_KATAGAKI)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_KATAGAKI)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_KEISAI)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_KEISAI)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CHECK_ADDRGP)
			RCSPACE(30)
			RCTOFIT(IDC_STATIC_INFOSRC)
			RCTOFIT(-1)
//			RCREST(IDC_COMBO_INFOSRC)
			RCREST(IDC_EDIT_INFOSRC)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_POINTSTAT)
			RCTOFIT(-1)
			RCREST(IDC_COMBO_POINTSTAT)
		ENDGROUP()
		RCSPACE(10)
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
 * @retval CAttrExTPPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlgByName( LPCTSTR lpcszTableName )
{
	CExportDlg* pDlg = NULL;
	pDlg = new CAttrExTPPointDlg();
	if( pDlg )
		pDlg->SetFeatureClassName( lpcszTableName );

	return pDlg;
}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrExTPPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrExTPPointDlg::CAttrExTPPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrExTPPointDlg::CAttrExTPPointDlg()
	: ATTR_BASE_CLASS(EXTP_POINT_MAP, IDD_ATTREXTPPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_ADDRCODE ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_ADDRNAME ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_KATAGAKI ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_KEISAI ),
		m_wnd4( _T("COMBOBOX"), this, IDC_COMBO_POINTSTAT ),
		m_wnd5( _T("CHECKBOX"), this, IDC_CHECK_ADDRGP ),
		m_wnd6( _T("EDIT"), this, IDC_EDIT_INFOSRC ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_ADDR1 ),
		m_wnd9( _T("EDIT"), this, IDC_EDIT_ADDR2 ),
		m_wnd10( _T("EDIT"), this, IDC_EDIT_ADDR3 ),
		m_wnd11( _T("EDIT"), this, IDC_EDIT_ADDR4 )
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

	m_hBrush2 = NULL;
}

/** 
 * @fn HWND CAttrExTPPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrExTPPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrExTPPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrExTPPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrExTPPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrExTPPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_COMBO_POINTSTAT) );
	}

	// 編集不可に設定しておく
	SetDlgStatus(IDC_EDIT_ADDR1, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDR2, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDR3, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDR4, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDRCODE, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDRNAME, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_KATAGAKI, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_KEISAI, FALSE, TRUE );
	SetDlgStatus(IDC_CHECK_ADDRGP, FALSE);
	SetDlgStatus(IDC_EDIT_INFOSRC, FALSE,TRUE );

	DevideAddrCode();

	SetToolTip(IDC_EDIT_ADDRNAME);
	SetToolTip(IDC_EDIT_KATAGAKI);
	SetToolTip(IDC_EDIT_KEISAI);

    return bRet;
}

/**
 * @fn void CAttrExTPPointDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrExTPPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else
	{
		m_cControlRel.SetHWND( m_hWnd );

		// 20桁住所コード
		m_cControlRel.SetControl( IDC_EDIT_ADDRCODE,	TYPE_EDITSTRING,0,	0,	_T("ADDRCODE"),		NULL, m_strFeatureClassName );
		// 住所名
		m_cControlRel.SetControl( IDC_EDIT_ADDRNAME,	TYPE_EDITSTRING,0,	0,	_T("ADDRNAME"),		NULL, m_strFeatureClassName );
		// 方書名称
		m_cControlRel.SetControl( IDC_EDIT_KATAGAKI,	TYPE_EDITSTRING,0,	0,	_T("KATAGAKINAME"),	NULL, m_strFeatureClassName );
		// 掲載名称
		m_cControlRel.SetControl( IDC_EDIT_KEISAI,		TYPE_EDITSTRING,0,	0,	_T("KEISAINAME"),	NULL, m_strFeatureClassName );
		// 調査結果コード
		m_cControlRel.SetControl( IDC_COMBO_POINTSTAT,	TYPE_COMBO,		0,	0,	_T("POINTSTAT_C"),	NULL, m_strFeatureClassName );
		// 号ポイント化フラグ
		m_cControlRel.SetControl( IDC_CHECK_ADDRGP,		TYPE_CHECKBOX,	0,	0,	_T("ADDPOINT_F"),	NULL, m_strFeatureClassName );
		// 提供元
		// ドメインが設定されているけれども、数値で出してほしいとのことなので、あえてEDITBOXで
		m_cControlRel.SetControl( IDC_EDIT_INFOSRC,		TYPE_EDIT,		0,	0,	_T("INFOSRC_C"),	NULL, m_strFeatureClassName );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, m_strFeatureClassName );
	}
}


/**
 * @fn void CAttrExTPPointDlg::DevideAddrCode()
 * @brief 20桁住所コードを分解し、見やすいように変換します。
 */
void CAttrExTPPointDlg::DevideAddrCode()
{
	CEdit cAddrEdit( GetDlgItem( IDC_EDIT_ADDRCODE ) );
	CEdit cAddrEdit1( GetDlgItem( IDC_EDIT_ADDR1 ) );
	CEdit cAddrEdit2( GetDlgItem( IDC_EDIT_ADDR2 ) );
	CEdit cAddrEdit3( GetDlgItem( IDC_EDIT_ADDR3 ) );
	CEdit cAddrEdit4( GetDlgItem( IDC_EDIT_ADDR4 ) );

	// 絶対数字で20桁のようなので、固定で取得
	LPTSTR lpszAddrCode = (LPTSTR)_alloca(21*sizeof(TCHAR));
	cAddrEdit.GetWindowText( lpszAddrCode, 21 );
	CString strAddrCode = (LPCTSTR)lpszAddrCode;

	CString strAddr1,strAddr2,strAddr3,strAddr4;
	strAddr1 = strAddrCode.Mid(0,5);
	strAddr2 = strAddrCode.Mid(5,6);
	strAddr3 = strAddrCode.Mid(11,5);
	strAddr4 = strAddrCode.Mid(16,4);

	// ③街区符号と④住居番号は数字のように扱い、00500とかであったら
	// 500などと表示されるようにする
	long lAddr3 = _ttol( (LPTSTR)(LPCTSTR)strAddr3 );
	long lAddr4 = _ttol( (LPTSTR)(LPCTSTR)strAddr4 );
	strAddr3.Format(_T("%d"),lAddr3 );
	strAddr4.Format(_T("%d"),lAddr4 );

	cAddrEdit1.SetWindowText( strAddr1 );
	cAddrEdit2.SetWindowText( strAddr2 );
	cAddrEdit3.SetWindowText( strAddr3 );
	cAddrEdit4.SetWindowText( strAddr4 );
}

/**
 * @fn void CAttrExTPPointDlg::SetToolTip(INT nTargetItem)
 * @brief ツールチップを作成します。
 *
 * @note 作成するツールチップは属性に変更がかからないため、作りっぱなしで問題なし
 *
 * @param nTargetItem	[in]	ツールチップを作成するコントロールアイテムのID
*/
void CAttrExTPPointDlg::SetToolTip(INT nTargetItem)
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
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS; 
	ti.hwnd = GetDlgItem(nTargetItem);
	ti.uId =  (UINT_PTR)GetDlgItem(nTargetItem).m_hWnd;

	ti.lpszText = lpszText;

	::SendMessage( hToolTip, TTM_ADDTOOL , 0 , (LPARAM)&ti);
	::SendMessage( hToolTip, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)300);
	::SendMessage( hToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);

	CWindow cWnd( hToolTip );
	cWnd.SetFont( m_hFont );
}
