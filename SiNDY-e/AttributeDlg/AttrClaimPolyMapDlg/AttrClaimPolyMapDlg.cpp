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

// AttrClaimPolyMapDlg.cpp: CAttrClaimPolyMapDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrClaimPolyMapDlg.h"

/// ウィンドウマネージャ配置マップ（クレームポリゴン用）
BEGIN_WINDOW_MAP(CLAIMPOLY_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_TOFIT,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CLAIMTYPE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_CLAIMTYPE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CLAIMNO)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_CLAIMNO)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_COMFIRM)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_USELESS)
			ENDGROUP()
			RCSPACE(10)
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
 * @retval CAttrClaimPolyMapDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrClaimPolyMapDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrClaimPolyMapDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrClaimPolyMapDlg::CAttrClaimPolyMapDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrClaimPolyMapDlg::CAttrClaimPolyMapDlg() 
	: ATTR_BASE_CLASS(CLAIMPOLY_MAP, IDD_ATTRCLAIMPOLYMAPDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_CLAIMTYPE ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_CLAIMNO ),
		m_wnd2( _T("CHECKBOX"), this, IDC_CHECK_COMFIRM ),
		m_wnd3( _T("CHECKBOX"), this, IDC_CHECK_USELESS ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_REMARKS )
{
}

/** 
 * @fn HWND CAttrClaimPolyMapDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrClaimPolyMapDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrClaimPolyMapDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrClaimPolyMapDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrClaimPolyMapDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrClaimPolyMapDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_COMBO_CLAIMTYPE) );
	}

    return bRet;
}

/**
 * @fn void CAttrClaimPolyMapDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrClaimPolyMapDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// クレームポリゴン住所

		// クレームタイプ
		m_cControlRel.SetControl( IDC_COMBO_CLAIMTYPE,	TYPE_COMBO,		0,	0,	_T("CLAIMTYPE_C"),	NULL, CLAIMPOLY_MAP_TABLE_NAME );
		// クレームNO
		m_cControlRel.SetControl( IDC_EDIT_CLAIMNO,		TYPE_EDITSTRING,0,	0,	_T("CLAIM_NO"),		NULL, CLAIMPOLY_MAP_TABLE_NAME );
		// 常時確認フラグ
		m_cControlRel.SetControl( IDC_CHECK_COMFIRM,	TYPE_CHECKBOX,	0,	0,	_T("COMFIRM_F"),	NULL, CLAIMPOLY_MAP_TABLE_NAME );
		// 不用フラグ
		m_cControlRel.SetControl( IDC_CHECK_USELESS,	TYPE_CHECKBOX,	0,	0,	_T("USELESS_F"),	NULL, CLAIMPOLY_MAP_TABLE_NAME );
		// コメント
		m_cControlRel.SetControl( IDC_EDIT_REMARKS,		TYPE_EDITSTRING,0,	0,	_T("REMARKS"),		NULL, CLAIMPOLY_MAP_TABLE_NAME );
	}
}

/**
 * @fn BOOL CAttrClaimPolyMapDlg::ErrorCheck()
 * @brief ダイアログが閉じられるときに道路リンクのエラーチェックを行う
 *
 * @return エラーでなかったら TRUE、エラーだったら FALSE
 */
BOOL CAttrClaimPolyMapDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == CLAIMPOLY_ADDR_TABLE_NAME || strTableName == CLAIMPOLY_MAP_TABLE_NAME )
		{
			// 現在編集中の値を習得
			CFieldDef* pFieldDef;
			CComVariant vaClaimType;
			if( strTableName == CLAIMPOLY_ADDR_TABLE_NAME )
				pFieldDef = it->GetFieldDef( CLAIMPOLY_ADDR_TABLE_NAME, _T("CLAIMTYPE_C") );
			else
				pFieldDef = it->GetFieldDef( CLAIMPOLY_MAP_TABLE_NAME, _T("CLAIMTYPE_C") );

			vaClaimType = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID = 0;
			it->GetFeature()->get_OID( &lOID );

			if( vaClaimType.lVal == 0 )
			{
				strMessage.Format(AheLoadString( IDS_NOTCLAIMTYPE ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}
