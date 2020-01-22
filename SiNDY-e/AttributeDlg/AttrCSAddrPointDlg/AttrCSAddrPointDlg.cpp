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

// AttrCSAddrPointDlg.cpp: CAttrCSAddrPointDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrCSAddrPointDlg.h"

/// ウィンドウマネージャ配置マップ（ノード用）
BEGIN_WINDOW_MAP(CSADDRPOINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CSADDRPOINT)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_CSADDRPOINT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GOUNO)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GOUNO)
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
 * @retval CAttrCSAddrPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrCSAddrPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSAddrPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCSAddrPointDlg::CAttrCSAddrPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrCSAddrPointDlg::CAttrCSAddrPointDlg()
	: ATTR_BASE_CLASS(CSADDRPOINT_MAP, IDD_ATTRCSADDRPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_CSADDRPOINT ), 
		m_wnd1( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_GOUNO )
{
}

/** 
 * @fn HWND CAttrCSAddrPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrCSAddrPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCSAddrPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrCSAddrPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrCSAddrPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCSAddrPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	//[Bug 7485]拡充作業用ポイントRefS_Point用のダイアログが欲しいに対応
	// 一旦号番号は、DISALBEに
	SetDlgStatus( IDC_EDIT_GOUNO, FALSE, TRUE );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_EDIT_CSADDRPOINT) );
		// 編集開始かつREFS_POINTなら、号番号編集可能に
		if( m_strFeatureClassName == _T("REFS_POINT") )
			SetDlgStatus( IDC_EDIT_GOUNO, TRUE, TRUE );
	}

    return bRet;
}

/**
 * @fn void CAttrCSAddrPointDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrCSAddrPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// CS住所ポイント用

		//[Bug 7485]拡充作業用ポイントRefS_Point用のダイアログが欲しいに対応
		// 号番号
		if( m_strFeatureClassName == _T("REFS_POINT") )
			m_cControlRel.SetControl( IDC_EDIT_GOUNO,	TYPE_EDITSTRING,	0,	0,	_T("GOU_NO"),	NULL, m_strFeatureClassName );

		// 20桁コード
		m_cControlRel.SetControl( IDC_EDIT_CSADDRPOINT,	TYPE_EDITSTRING,	0,	0,	_T("ADDRCODE"),	NULL, m_strFeatureClassName );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,	0,	0,	_T("SOURCE"),	NULL, m_strFeatureClassName );
	}
}

/**
 * @fn BOOL CAttrCSAddrPointDlg::ErrorCheck()
 * @brief ダイアログが閉じられるときに道路リンクのエラーチェックを行う
 *
 * @return エラーでなかったら TRUE、エラーだったら FALSE
 */
BOOL CAttrCSAddrPointDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == CS_ADDRPOINT_TABLE_NAME )
		{
			// 現在編集中の値を習得
			CFieldDef* pFieldDef;
			CComVariant vaAddrCode;
			pFieldDef = it->GetFieldDef( CS_ADDRPOINT_TABLE_NAME, _T("ADDRCODE") );

			CString strMessage;
			LONG lOID = 0;
			it->GetFeature()->get_OID( &lOID );

			CString strAddrCode = pFieldDef->value2string(FALSE);
			LONG lAddrCodeLong = strAddrCode.GetLength();

			if( lAddrCodeLong != 20 )
			{
				strMessage.Format( AheLoadString( IDS_NOT20FIGURES ),lOID );
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}
