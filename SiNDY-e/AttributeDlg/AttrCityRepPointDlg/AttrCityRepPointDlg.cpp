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

// AttrCityRepPointDlg.cpp: CAttrCityRepPointDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrCityRepPointDlg.h"

/// ウィンドウマネージャ配置マップ（都市地図代表点用）
BEGIN_WINDOW_MAP(BASE_REP_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BASE_REP_POINT_STATIC1)
				RCTOFIT(-1)
				RCREST(IDC_BASE_REP_POINT_EDIT1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BASE_REP_POINT_STATIC2)
				RCTOFIT(-1)
				RCREST(IDC_BASE_REP_POINT_EDIT2)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCREST(IDC_CHECK_ANNO_F)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BASE_REP_POINT_STATIC3)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_POINTCLASS_C)
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
 * @retval CAttrCityRepPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrCityRepPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCityRepPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCityRepPointDlg::CAttrCityRepPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrCityRepPointDlg::CAttrCityRepPointDlg()
	: ATTR_BASE_CLASS(BASE_REP_POINT_MAP, IDD_ATTRCITYREPPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_BASE_REP_POINT_EDIT1 ),
		m_wnd1( _T("EDIT"),	this, IDC_BASE_REP_POINT_EDIT2 ), 
		m_wnd2( _T("CHECK"), this, IDC_CHECK_ANNO_F ),
		m_wnd3( _T("COMBO"), this, IDC_COMBO_POINTCLASS_C ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CAttrCityRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrCityRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCityRepPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrCityRepPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrCityRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCityRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_BASE_REP_POINT_EDIT1) );
	}

    return bRet;
}

/**
 * @fn void CAttrCityRepPointDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrCityRepPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// 都市地図代表点用

		// 11桁住所コード
		m_cControlRel.SetControl( IDC_BASE_REP_POINT_EDIT1,		TYPE_EDITSTRING,0,	0,	_T("ADDRCODE"),		NULL, CITY_REP_POINT_TABLE_NAME );
		// 地番
		m_cControlRel.SetControl( IDC_BASE_REP_POINT_EDIT2,		TYPE_EDIT,		0,	0,	_T("CHIBAN_NO"),	NULL, CITY_REP_POINT_TABLE_NAME );
		// 地番注記フラグ
		m_cControlRel.SetControl( IDC_CHECK_ANNO_F,				TYPE_CHECKBOX,	0,	0,	_T("ANNO_F"),		NULL, CITY_REP_POINT_TABLE_NAME );
		// 地番
		m_cControlRel.SetControl( IDC_COMBO_POINTCLASS_C,		TYPE_COMBO,		0,	0,	_T("POINTCLASS_C"),	NULL, CITY_REP_POINT_TABLE_NAME );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,				TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, CITY_REP_POINT_TABLE_NAME );
	}
}

/**
 * @fn BOOL CAttrCityRepPointDlg::ErrorCheck()
 * @brief エラーがないかどうか調べる
 *
 * @return	エラーがなかったらTRUE、エラーがあればFALSE
 */
BOOL CAttrCityRepPointDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == BASE_REP_POINT_TABLE_NAME )
		{

			// 現在編集中の値を習得
			CFieldDef* pFieldDef;
			CComVariant vaAddrCode, vaAdminName;
			pFieldDef = it->GetFieldDef( BASE_REP_POINT_TABLE_NAME, _T("ADDRCODE") );
			vaAddrCode = pFieldDef->m_vaValue;

			USES_CONVERSION;

			CString strMessage;
			LONG lOID;
			it->GetFeature()->get_OID( &lOID );

			CString strAddrCode;
			strAddrCode.Format( _T("%s"), OLE2T(vaAddrCode.bstrVal) );

			if( strAddrCode.GetLength() != 11 )
			{
				strMessage.Format( AheLoadString( IDS_ADDRCODE11 ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}

			if( strAddrCode.Find( _T(" ") ) != -1 )
			{
				strMessage.Format( AheLoadString( IDS_NOTNEEDSPACE ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}
