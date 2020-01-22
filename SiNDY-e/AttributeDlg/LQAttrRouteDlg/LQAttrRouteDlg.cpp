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

// LQAttrRouteDlg.cpp: CLQAttrRouteDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrRouteDlg.h"

using namespace std;

/// ウィンドウマネージャ配置マップ（アクセス/乗り継ぎ道用）
BEGIN_WINDOW_MAP(ROUTE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_ROUTECLASS)
			RCREST(IDC_COMBO_ROUTECLASS)
		ENDGROUP()
		RCSPACE(7)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_SOURCENAME)
			RCREST(IDC_EDIT_SOURCENAME)
		ENDGROUP()
		RCSPACE(7)
		RCREST(-1)
		RCTOFIT(IDC_STATIC_SOURCESEP)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_SOURCE)
			RCREST(IDC_EDIT_SOURCE)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CLQAttrRouteDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrRouteDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrRouteDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CLQAttrRouteDlg::CLQAttrDefaultDlg()
 * @brief コンストラクタ
 *
 * なにもしません
 */
CLQAttrRouteDlg::CLQAttrRouteDlg() 
	: LQATTR_COMMON_BASE_CLASS(ROUTE_MAP, IDD_LQATTRROUTEDLG),
	m_wndRouteClass( _T("COMBOBOX"), this, IDC_COMBO_ROUTECLASS ),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE ),
	m_wndSourceName( _T("EDIT"), this, IDC_EDIT_SOURCENAME )
{
}

/** 
 * @fn HWND CLQAttrRouteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CLQAttrRouteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// ダイアログの作成はLQATTR_COMMON_BASE_CLASSに任せます
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrRouteDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CLQAttrRouteDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrRouteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CLQAttrRouteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
		::SetFocus( GetDlgItem(IDC_COMBO_ROUTECLASS) );
	else
		::SendMessage( GetDlgItem(IDC_COMBO_ROUTECLASS), WM_KILLFOCUS, NULL, NULL );

    return bRet;
}

/**
 * @fn void CLQAttrRouteDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CLQAttrRouteDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		CString strInfoTableName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueRoute, sindyeLoadQueueInfoInfoTableName );
		m_cControlRel.SetHWND( m_hWnd );
		// VICS用
		m_cControlRel.SetControl( IDC_COMBO_ROUTECLASS,	TYPE_COMBO,		0, 0,	_T("ROUTECLASS_C"),	NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0, 0,	_T("SOURCE"),		NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_EDIT_SOURCENAME,	TYPE_EDITSTRING,0, 0,	_T("SOURCE_NAME"),	NULL, strInfoTableName );
	}
}

BOOL CLQAttrRouteDlg::IsAlreadyExistOverlapedRoute( LONG lCurrentRouteClass, BOOL bErrEnd/* = TRUE*/ )
{
	// 道路リンクIDを取得
	list<ROWDEF> listRoads;
	LONG lDrawMode = CLoadQueueInfo::GetLoadQueueDrawMode( sindyeRoadQueueRoute );
	CString strInfTableName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueRoute, sindyeLoadQueueInfoInfoTableName );
	CString strLQTableName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueRoute, sindyeLoadQueueInfoLQTableName );
	CLQRowDef* pInfoRow = GetCurrentFeatureDef();
	CRelationalTableDef* pRelTable = NULL;

	if( pInfoRow )
		pRelTable = pInfoRow->GetRelationalTableDef( strLQTableName );
	if( pRelTable )
	{
		for( CRelationalTableDef::iterator it = pRelTable->begin(); it != pRelTable->end(); it++ )
		{
			CFieldDef* pFieldDef = it->GetFieldDef( LINK_ID_FIELD );
			if( pFieldDef )
			{
				if( pFieldDef->m_vaValue.vt == VT_I4 )
				{
					ROWDEF def;
					def.flg = lDrawMode;
					def.oid = pFieldDef->m_vaValue.lVal;
					listRoads.push_back( def );
				}
			}
		}
	}
	_ASSERTE( listRoads.size() > 0 );
	
	// 道路リンクIDからLQを検索
	list<ROWDEF> listLQRows;
	list<LONG> listInfoIDs;
	CString strRoadRelFieldName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueRoute, sindyeLoadQueueInfoLQRFRoadFieldName );
	AheSelectLQRowByRoadIDs( listRoads, pRelTable->GetTable(), strRoadRelFieldName, lDrawMode, listLQRows, listInfoIDs );
	// InfoIDsからInfoを検索
	list< CAdapt<_IRowPtr> > listIRow;
	_IRowPtr ipRow = pInfoRow->GetFeature();
	ITablePtr ipInfoTable;
	LONG lOID = -1;
	if( ipRow )
	{
		ipRow->get_Table( &ipInfoTable );
		ipRow->get_OID( &lOID );
	}
	if( ipInfoTable )
	{
		for( list<LONG>::iterator it = listInfoIDs.begin(); it != listInfoIDs.end(); it++ )
		{
			_IRowPtr ipInfoRow;
			LONG lOIDTmp = -1;

			ipInfoTable->GetRow( *it, &ipInfoRow );
			// 現在のリンク列は飛ばして格納
			if( ipInfoRow != NULL ) ipInfoRow->get_OID( &lOIDTmp );	
			if( lOID != lOIDTmp ) 
			{
				// リンク列属性編集ツールに既に読み込まれていたらそっちから値を取得
				list<CLQRowDef>::iterator itFeatureDefList;
				for( itFeatureDefList = m_pFeatureDefList->begin(); itFeatureDefList != m_pFeatureDefList->end(); itFeatureDefList++ )
				{
					_IRowPtr ipListInfoRow = itFeatureDefList->GetFeature();
					LONG lListInfoID = -1;

					ipListInfoRow->get_OID( &lListInfoID );
					if( lOIDTmp == lListInfoID )
						break;
				}
				CComVariant vaRouteClass;
				if( itFeatureDefList != m_pFeatureDefList->end() )
				{
					CFieldDef* pRouteClassDef    = itFeatureDefList->GetFieldDef( strInfTableName, ROUTECLASS_FIELD );
					if( pRouteClassDef != NULL && pRouteClassDef->m_vaValue.vt == VT_I4 && lCurrentRouteClass == pRouteClassDef->m_vaValue.lVal )
						listIRow.push_back( ipInfoRow );
				}
				// リンク列属性編集ツールに既に読み込まれていなかったら検索したものから値を取得
				else {
					vaRouteClass = AheGetAttribute( ipInfoRow, ROUTECLASS_FIELD );
					if( vaRouteClass.vt == VT_I4 && lCurrentRouteClass == vaRouteClass.lVal )
						listIRow.push_back( ipInfoRow );
				}
			}
		}
	}
	// 同じ種別で方向を持つLQを探す
	listInfoIDs.clear();
	for( list<ROWDEF>::iterator it = listLQRows.begin(); it != listLQRows.end(); it++ )
	{
		LONG lOriID = it->oid;
		for( list< CAdapt<_IRowPtr> >::iterator itIRow = listIRow.begin(); itIRow != listIRow.end(); itIRow++ )
		{
			// 先に格納したIRowと同じIDをROUTE_IDにもつもののみチェック
			LONG lRowID = -1, lRouteID = -1;
			itIRow->m_T->get_OID( &lRowID );
			CComVariant vaInfoID = AheGetAttribute( it->obj, INF_ID_FIELD );
			if( vaInfoID.vt == VT_I4 ) lRouteID = vaInfoID.lVal;
			if( lRowID == lRouteID )
			{
				for( CRelationalTableDef::iterator itRelTable = pRelTable->begin(); itRelTable != pRelTable->end(); itRelTable++ )
				{
					CFieldDef* vaTempID = itRelTable->GetFieldDef( _T("OBJECTID") );
					LONG lTempID = vaTempID->m_vaValue.lVal;
					CComVariant vaInfoID = AheGetAttribute( it->obj, INF_ID_FIELD );
					if( vaInfoID.vt == VT_I4 )
						listInfoIDs.push_back( vaInfoID.lVal );
				}
			}	
		}
	}
	listInfoIDs.sort();
	listInfoIDs.unique();
	if( listInfoIDs.size() > 0 )
	{
		CString strMsg( AheLoadString( IDS_DUPLIROUTE ) );
		for( list<LONG>::iterator it = listInfoIDs.begin(); it != listInfoIDs.end(); it++ )
		{
			if( lOID == *it ) break;
			CString strMsgTmp;
			strMsgTmp.Format(_T(" OBJECTID=%d "), *it );
			strMsg += strMsgTmp;
		}
		if( bErrEnd )
			strMsg += AheLoadString( IDS_CONTINUE );

		if( bErrEnd )
		{
			if( AttrMessageBox( strMsg, AheLoadString(IDS_CAPTION), MB_YESNO|MB_ICONWARNING, WARNLEVEL_HIGH  ) == IDNO )
				return FALSE;
		}
		else {
			AttrMessageBox( strMsg, AheLoadString(IDS_CAPTION), MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH  );
			return FALSE;
		}
	}

	return TRUE;
}
BOOL CLQAttrRouteDlg::ErrorCheck()
{
	BOOL bRet = TRUE;
	CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueRoute, sindyeLoadQueueInfoInfoTableName );
	CString strMsg;
	for( list<CLQRowDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		if( strInfTableName.CompareNoCase( it->GetTableName() ) == 0 )
		{
			LONG lOID;
			CFieldDef* pOIDFieldDef = it->GetFieldDef( strInfTableName, _T("OBJECTID"));
			lOID = pOIDFieldDef->m_vaValue.lVal;

			CFieldDef* pRouteFieldDef = it->GetFieldDef( strInfTableName, _T("ROUTECLASS_C") );

			if( !pRouteFieldDef )
			{
				_ASSERTE( pRouteFieldDef != NULL );
				continue;
			}

			CString strMsg;
			// 案内種別コードに NULL が入っていないかどうか
			LONG ltest = pRouteFieldDef->m_vaValue.lVal;
			if( pRouteFieldDef->m_vaValue.lVal == 0 )
			{
				strMsg.Format(AheLoadString( IDS_NOROUTECLASS ), strInfTableName, lOID);
				AttrMessageBox( strMsg, AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH );

				bRet = FALSE;
				break;
			}
		}
	}
	
	return bRet;
}
