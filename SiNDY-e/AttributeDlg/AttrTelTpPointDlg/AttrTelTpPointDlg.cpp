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

// AttrTelTpPointDlg.cpp : DLL エクスポートの実装です。


#include "stdafx.h"
#include "AttrTelTpPointDlg.h"

/// ウィンドウマネージャ配置マップ（都市地図家形用）
BEGIN_WINDOW_MAP(TELTPPOINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ADDRCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ADDRCODE)
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
				RCTOFIT(IDC_STATIC_KEISAINAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_KEISAINAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_POINTSTAT)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_POINTSTAT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ADDXY)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_ADDXY)
			ENDGROUP()
			RCSPACE(10)
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
 * @retval CAttrTelTpPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrTelTpPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrTelTpPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrTelTpPointDlg::CAttrTelTpPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrTelTpPointDlg::CAttrTelTpPointDlg() 
	: ATTR_BASE_CLASS(TELTPPOINT_MAP, IDD_ATTRTELTPPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_ADDRCODE ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_ADDRNAME ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_KATAGAKI ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_KEISAINAME ),
		m_wnd4( _T("COMBO"), this, IDC_COMBO_POINTSTAT ),
		m_wnd5( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd6( _T("COMBO"), this, IDC_COMBO_ADDXY )
{
	// フォント作成
	m_hFont = CreateFont(
		16, 0, 0, 0, 100,		// 高さ，幅，Ｘ角度，Ｙ角度，太さ
		FALSE, FALSE, FALSE,    // 斜体，下線，打消線
		SHIFTJIS_CHARSET,       // キャラクタ設定
		OUT_DEFAULT_PRECIS,     // 出力精度
		CLIP_DEFAULT_PRECIS,    // クリップ精度
		DEFAULT_QUALITY,        // 品質
		FIXED_PITCH | FF_DONTCARE,// ピッチ
		_T("ＭＳＰゴシック"));   // フォント種類
	// フォント作成
	m_hStaticFont = CreateFont(
		12, 0, 0, 0, FW_BOLD,		// 高さ，幅，Ｘ角度，Ｙ角度，太さ
		FALSE, TRUE, FALSE,    // 斜体，下線，打消線
		SHIFTJIS_CHARSET,       // キャラクタ設定
		OUT_DEFAULT_PRECIS,     // 出力精度
		CLIP_DEFAULT_PRECIS,    // クリップ精度
		DEFAULT_QUALITY,        // 品質
		FIXED_PITCH | FF_DONTCARE,// ピッチ
		_T("ＭＳＰゴシック"));   // フォント種類
}

/** 
 * @fn HWND CAttrTelTpPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrTelTpPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrTelTpPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrTelTpPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrTelTpPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * 原稿番号の設定もしておきます。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */

BOOL CAttrTelTpPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		if( CheckSamePoint() )
		{
			// 編集開始されているときについでに、SetFocusしておく
			::SetFocus( GetDlgItem(IDC_COMBO_POINTSTAT) );
		}
		else
		{
			AheMessageBox( AheLoadString( IDS_CANTEDIT ), AheLoadString( IDS_CAPTION ), MB_OK );
			::PostMessage( GetParent(), WM_CLOSE, 0, 0 ); 
			return false;
		}
	}

	// 編集不可にしておく
	SetDlgStatus(IDC_EDIT_ADDRCODE, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDRNAME, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_KATAGAKI, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_KEISAINAME, FALSE, TRUE );
	::EnableWindow( GetDlgItem( IDC_COMBO_ADDXY ), FALSE);

    return bRet;
}

/**
 * @fn void CAttrBaseRailwayDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrTelTpPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// 電話番号住所ポイント用

		// 住所コード
		m_cControlRel.SetControl( IDC_EDIT_ADDRCODE,	TYPE_EDITSTRING,0,	0,  _T("ADDRCODE"),	NULL, _T("TELTP_POINT") );
		// 住所名
		m_cControlRel.SetControl( IDC_EDIT_ADDRNAME,	TYPE_EDITSTRING,0,	0,  _T("ADDRNAME"),	NULL, _T("TELTP_POINT") );
		// 方書
		m_cControlRel.SetControl( IDC_EDIT_KATAGAKI,	TYPE_EDITSTRING,0,	0,  _T("KATAGAKINAME"),	NULL, _T("TELTP_POINT") );
		// 掲載名
		m_cControlRel.SetControl( IDC_EDIT_KEISAINAME,	TYPE_EDITSTRING,0,	0,  _T("KEISAINAME"),	NULL, _T("TELTP_POINT") );
		// 調査結果コード
		m_cControlRel.SetControl( IDC_COMBO_POINTSTAT,	TYPE_COMBO,		0,	0,  _T("POINTSTAT_C"),	NULL, _T("TELTP_POINT") );
		// 座標付与種別コード
		m_cControlRel.SetControl( IDC_COMBO_ADDXY,	TYPE_COMBO,		0,	0,  _T("ADDXY_C"),	NULL, _T("TELTP_POINT") );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,  _T("SOURCE"),		NULL, _T("TELTP_POINT") );
	}

}
bool CAttrTelTpPointDlg::CheckSamePoint()
{
	CFeatureDef *pCurrentFeatureDef = GetCurrentFeatureDef();
	IFeaturePtr ipCurrentFeature = pCurrentFeatureDef->GetFeature();
	if( !ipCurrentFeature )
		return false;

	long lCurrentOID = 0;
	ipCurrentFeature->get_OID( &lCurrentOID );
	ITablePtr ipCurrentTable;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IGeometryPtr ipOrgGeom;
	ipCurrentFeature->get_Shape( &ipOrgGeom );
	IRelationalOperatorPtr ipRel( ipOrgGeom );
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		IFeaturePtr temp = it->GetFeature();
		if( !temp )
			continue;
		LONG lOID = 0;
		temp->get_OID( &lOID );
		ITablePtr ipTable;
		temp->get_Table( &ipTable );
		if( ipTable != ipCurrentTable )
			continue;
		if(lOID == lCurrentOID)
			continue;

		// ここまできて初めてチェック対象
		IGeometryPtr ipTempGeom;
		temp->get_Shape( &ipTempGeom );
		VARIANT_BOOL vaEqual;
		ipRel->Equals( ipTempGeom, &vaEqual );
		// 同じ座標でなかったら終了
		if( vaEqual == VARIANT_FALSE )
			return false;
	}
	return true;
}

void CAttrTelTpPointDlg::CopyOtherFeature(CString strCopyFieldName)
{
	// とりあえず、現在保持しているリストすべてにおいてループを行う
	CFeatureDef *pCurrentFeatureDef = GetCurrentFeatureDef();
	IFeaturePtr ipCurrentFeature = pCurrentFeatureDef->GetFeature();
	if( !ipCurrentFeature )
		return;

	long lCurrentOID = 0;
	ipCurrentFeature->get_OID( &lCurrentOID );
	ITablePtr ipCurrentTable;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IGeometryPtr ipOrgGeom;
	ipCurrentFeature->get_Shape( &ipOrgGeom );
	IRelationalOperatorPtr ipRel( ipOrgGeom );

	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		IFeaturePtr temp = it->GetFeature();
		if( !temp )
			continue;
		LONG lOID;
		temp->get_OID( &lOID );
		ITablePtr ipTable;
		temp->get_Table( &ipTable );
		if( ipTable != ipCurrentTable )
			continue;
		if(lOID == lCurrentOID)
			continue;
		IGeometryPtr ipTempGeom;
		temp->get_Shape( &ipTempGeom );
		VARIANT_BOOL vaEqual;
		ipRel->Equals( ipTempGeom, &vaEqual );
		if( vaEqual == VARIANT_FALSE )
			continue;

		CFeatureDef *pFeatureDef = &(*it);
		if( pFeatureDef != NULL )
		{
			// すべてのフィールド値をチェック
			for( std::list<CFieldDef>::iterator itField = GetCurrentFeatureDef()->begin(); itField != GetCurrentFeatureDef()->end(); itField++ )
			{
				CString strFieldName = itField->m_strFieldName;

				// 以下if文の中身はコピーしてしまってはいけないもの
				if( strFieldName == strCopyFieldName)
				{
					// 上記のif文の条件以外はコピーする
					CFieldDef *pFieldDef;
					pFieldDef = pFeatureDef->GetFieldDef( _T("TELTP_POINT"), itField->m_strFieldName );
					VARIANT vaValue;
					pFeatureDef->GetFeature()->get_Value( itField->m_lFieldIndex, &vaValue );
					if( itField->m_vaValue != vaValue )
					{
						pFieldDef->m_vaValue = itField->m_vaValue;
						pFieldDef->m_bChanged = TRUE;
					}
					else{
						pFieldDef->m_vaValue = itField->m_vaValue;
						pFieldDef->m_bChanged = FALSE;
					}
				}
			}
		}
	}
}
