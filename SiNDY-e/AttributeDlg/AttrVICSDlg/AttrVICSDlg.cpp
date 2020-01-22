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

// AttrVICSDlg.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrVICSDlg.h"

/// ウィンドウマネージャ配置マップ（VICS用）
BEGIN_WINDOW_MAP(VICS_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MESH)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_MESH)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_VICSCLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_VICSCLASS)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_VICSID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_VICSID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BUTTON_DRAW)
				RCTOFIT(-1)
				RCTOFIT(IDC_STATIC_SAMEVICS)
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
 * @retval CAttrVICSDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrVICSDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrVICSDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrVICSDlg::CAttrVICSDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrVICSDlg::CAttrVICSDlg()
	: ATTR_BASE_CLASS(VICS_MAP, IDD_ATTRVICSDLG),
		m_wnd0( _T("EDIT"),		this, IDC_EDIT_MESH ),
		m_wnd1( _T("COMBOBOX"), this, IDC_COMBO_VICSCLASS ),
		m_wnd2( _T("EDIT"),		this, IDC_EDIT_VICSID ),
		m_wnd3( _T("EDIT"),		this, IDC_EDIT_SOURCE ),
		m_wnd4( _T("BUTTON"),		this, IDC_BUTTON_DRAW )
{
}

/** 
 * @fn HWND CAttrVICSDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrVICSDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrVICSDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrVICSDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrVICSDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrVICSDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );
	CWindow staticName( GetDlgItem(IDC_STATIC_SAMEVICS) );
	staticName.SetWindowText( (LPCTSTR)(_T("")) );

    return bRet;
}

/**
 * @fn void CAttrVICSDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrVICSDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {

		m_cControlRel.SetHWND( m_hWnd );

		// VICS
		// メッシュコード
		m_cControlRel.SetControl( IDC_EDIT_MESH,		TYPE_EDIT,	0,	0,	_T("MESHCODE"),		NULL, m_strFeatureClassName);
		// VICS種別
		m_cControlRel.SetControl( IDC_COMBO_VICSCLASS,	TYPE_COMBO,	0,	0,	_T("VICS_CLASS"),	NULL, m_strFeatureClassName);
		// VICS_ID
		m_cControlRel.SetControl( IDC_EDIT_VICSID,		TYPE_EDIT,	0,	0,	_T("VICS_ID"),		NULL, m_strFeatureClassName);
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("VICSINFO"),	NULL, m_strFeatureClassName );
	}
}

/**
 * @fn void CAttrVICSDlg::DrawSameVICS(IRowPtr ipVICSRow)
 * @brief 同じVICSをハイライト表示します
 *
 * @param ipVICSRow	[in]	描画したいVICSフィーチャ
 */
void CAttrVICSDlg::DrawSameVICS(_IRowPtr ipVICSRow)
{
	if( !ipVICSRow )
		return;

	// 表示されているVICSのメッシュコード、種別、VICSIDを取得する
	IFieldsPtr ipFields;
	LONG	lIndex = 0;
	ipVICSRow->get_Fields( &ipFields );
	
	LONG lOID = 0;
	ipVICSRow->get_OID( &lOID );

	CComVariant vaValue;
	ipFields->FindField( CComBSTR(_T("MESHCODE")),&lIndex );
	ipVICSRow->get_Value( lIndex, &vaValue );
	long lMeshCode = vaValue.lVal;

	ipFields->FindField( CComBSTR(_T("VICS_CLASS")),&lIndex );
	ipVICSRow->get_Value( lIndex, &vaValue );
	long lVICSClass = vaValue.lVal;

	ipFields->FindField( CComBSTR(_T("VICS_ID")),&lIndex );
	ipVICSRow->get_Value( lIndex, &vaValue );
	long lVICSID = vaValue.lVal;

	ITablePtr		ipTable;
	_ICursorPtr		ipCursor;
	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;
        
	ipVICSRow->get_Table( &ipTable );

	// メッシュコードで検索
	strWhereClause.Format(_T("MESHCODE = %d AND VICS_CLASS = %d AND VICS_ID = %d"), lMeshCode, lVICSClass, lVICSID );
	strSubFields.Format(_T("OBJECTID,VICS_CLASS,VICS_ID,SHAPE"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	ipTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	_IRowPtr ipRow;
	LONG lCounter=0;
	if( ipCursor )
	{
		IGeometryPtr ipVICSGeometry;
		while( ipCursor->NextRow( &ipRow ) == S_OK )
		{
			IFeaturePtr ipFeature = ipRow;
			if( !ipVICSGeometry )
				ipFeature->get_ShapeCopy( &ipVICSGeometry );
			else
			{
				IGeometryPtr ipGeomMaster;
				IGeometryPtr ipGeomSlave;
				ipFeature->get_ShapeCopy( &ipGeomMaster );

				ITopologicalOperatorPtr ipTopo(ipVICSGeometry);
				if( SUCCEEDED( ipTopo->Union( ipGeomMaster, &ipGeomSlave)))
				{
					if(ipGeomSlave )
					{
						ipVICSGeometry = ipGeomSlave;
						lCounter++;
					}
				}		
			}
		}
		m_ipVICSGeom = ipVICSGeometry;
	}

	// VICSの周りだけ再描画
	IEnvelopePtr ipFeatEnv;
	if( m_ipVICSGeom )
	{
		m_ipVICSGeom->get_Envelope( &ipFeatEnv );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, ipFeatEnv );
	}
}

/**
 * @fn void CAttrVICSDlg::Refresh()
 * @brief 描画中のVICSの周りを再描画します
 */
void CAttrVICSDlg::Refresh()
{
	if( m_ipVICSGeom )
		AheDrawArrowline( m_cArcHelper.GetScreenDisplay(), m_ipVICSGeom, RGB( 0, 250, 0), 3, esriSLSSolid);
}
