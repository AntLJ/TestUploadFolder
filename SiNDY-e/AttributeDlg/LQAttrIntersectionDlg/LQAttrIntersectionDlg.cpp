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

// LQAttrIntersectionDlg.cpp: CLQAttrIntersectionDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrIntersectionDlg.h"

/// ウィンドウマネージャ配置マップ（進入方向別交差点名称用）
BEGIN_WINDOW_MAP(INTERSECTION_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			BEGINROWS(WRCT_REST, IDC_STATIC_INTERSECTION, RCMARGINS(10,5))	// 進入方向別交差点グループ
				RCSPACE(15)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,5))		// 漢字名称
					RCTOFIT(IDC_STATIC_KANJI)
					RCREST(IDC_EDIT_KANJI)
				ENDGROUP()
				BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,5))		// カナ名称
					RCTOFIT(IDC_STATIC_YOMI)
					RCREST(IDC_EDIT_YOMI)
				ENDGROUP()
				RCSPACE(10)
			ENDGROUP()
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))		// コピーボタン
			RCTOFIT(IDC_BUTTON_BASE_COPY)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			BEGINROWS(WRCT_REST, IDC_STATIC_BASE_INTERSECTION, RCMARGINS(10,5))	// 代表交差点グループ
				RCSPACE(15)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,5))		// 漢字名称
					RCTOFIT(IDC_STATIC_BASE_KANJI)
					RCREST(IDC_EDIT_BASE_KANJI)
				ENDGROUP()
				BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,5))		// カナ名称
					RCTOFIT(IDC_STATIC_BASE_YOMI)
					RCREST(IDC_EDIT_BASE_YOMI)
				ENDGROUP()
				RCSPACE(10)
			ENDGROUP()
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
 * @retval CLQAttrIntersectionDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrIntersectionDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrIntersectionDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////
/** 
 * @fn CLQAttrIntersectionDlg::CLQAttrIntersectionDlg()
 * @brief コンストラクタ
 */
CLQAttrIntersectionDlg::CLQAttrIntersectionDlg() 
	: CLQAttrCommonBaseDlg(INTERSECTION_MAP, IDD_LQATTRINTERSECTIONDLG),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE ),
	m_wndSourceKanji( _T("EDIT"), this, IDC_EDIT_KANJI ),
	m_wndSourceYomi( _T("EDIT"), this, IDC_EDIT_YOMI ),
	m_wndSourceBaseKanji( _T("EDIT"), this, IDC_EDIT_BASE_KANJI ),
	m_wndSourceBaseYomi( _T("EDIT"), this, IDC_EDIT_BASE_YOMI ),
	m_wndSourceCopyBtn( _T("BUTTON"), this, IDC_BUTTON_BASE_COPY ) // タブオーダーがおかしかったので、XXX_BASE_XXXについても追加 (bug9458)
{
	m_hTipNameKanji = NULL;
	m_hTipNameYomi = NULL;
	m_hTipBaseNameKanji = NULL;
	m_hTipBaseNameYomi = NULL;
}
/** 
 * @fn HWND CLQAttrIntersectionDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CLQAttrIntersectionDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// ダイアログの作成はLQATTR_COMMON_BASE_CLASSに任せます
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrIntersectionDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CLQAttrIntersectionDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrIntersectionDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CLQAttrIntersectionDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );
	
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	else
		::SendMessage( GetDlgItem(IDC_EDIT_SOURCE), WM_KILLFOCUS, NULL, NULL );

	SetBaseIntersectionInfo();	// 代表交差点名称のセット

	SetToolTip( IDC_EDIT_KANJI );
	SetToolTip( IDC_EDIT_YOMI );
	SetToolTip( IDC_EDIT_BASE_KANJI );
	SetToolTip( IDC_EDIT_BASE_YOMI );
 
	return bRet;
}

/**
 * @fn void CLQAttrIntersectionDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CLQAttrIntersectionDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		CString strInfoTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueIntersection, sindyeLoadQueueInfoInfoTableName );
		m_cControlRel.SetHWND( m_hWnd );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,	0, 0,	_T("SOURCE"),		NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_EDIT_KANJI,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI"),	NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_EDIT_YOMI,	TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI"),	NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_BUTTON_BASE_COPY,		TYPE_BUTTON,	0,	0 );
	}
}
BOOL CLQAttrIntersectionDlg::SetCtrlColorChange(HWND hTargetWND)
{
	HWND hWnd1,hWnd2;

	hWnd1 = CWindow( GetDlgItem( IDC_EDIT_KANJI ) );
	hWnd2 = CWindow( GetDlgItem( IDC_EDIT_YOMI ) );
	if( hWnd1 == hTargetWND || hWnd2 == hTargetWND )
		return TRUE;

	return FALSE;
}
/**
 * @fn BOOL CLQAttrIntersectionDlg::ErrorCheck()
 * @brief ダイアログが閉じられるときにエラーチェックを行う
 *
 * @return エラーでなかったら TRUE、エラーだったら FALSE
 */
BOOL CLQAttrIntersectionDlg::ErrorCheck()
{
	BOOL bRet = TRUE;
	CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueIntersection, sindyeLoadQueueInfoInfoTableName );
	CString strMsg;
	for( std::list<CLQRowDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();
		if( strTableName == strInfTableName )
		{
			// 現在編集中の値を習得
			CFieldDef* pFieldDef;
			CComVariant vaName_Kanji, vaName_Yomi;
			pFieldDef = it->GetFieldDef( strTableName, _T("NAME_KANJI") );
			vaName_Kanji = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( strTableName, _T("NAME_YOMI") );
			vaName_Yomi = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID = 0;
			it->GetFeature()->get_OID( &lOID );

			if( vaName_Kanji.vt != VT_NULL && vaName_Yomi.vt == VT_NULL )
			{
				strMessage.Format( AheLoadString( IDS_KANAERROR ), lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else if( vaName_Kanji.vt == VT_NULL && vaName_Yomi.vt != VT_NULL )
			{
				strMessage.Format(AheLoadString( IDS_KANAERROR ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}

IFeatureClassPtr CLQAttrIntersectionDlg::GetRoadLinkWithLQ( CLQRowDef* pRowDef )
{
	IFeatureClassPtr ipRoadFeatureClass;
	if( !pRowDef )
		return NULL;

	_IRowPtr ipRow = pRowDef->GetFeature();
	ITablePtr ipTestTable;
	ipRow->get_Table( &ipTestTable );
	IDatasetPtr ipDataset = ipTestTable;
	CString strTestOwnerName = AheGetFeatureClassOwnerName( ipDataset );

	// TOCにいるすべてのジオフィーチャレイヤを取得してループ回しお目当ての道路リンク取得する
	IMapPtr ipMap( m_cArcHelper.GetMap() );
	IUIDPtr ipUID( CLSID_UID );
	ipUID->put_Value( CComVariant(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ); // IGeoFeatureLayer
	IEnumLayerPtr ipEnumLayer;
	if( SUCCEEDED( ipMap->get_Layers( ipUID, VARIANT_TRUE, &ipEnumLayer ) ) )
	{
		ipEnumLayer->Reset();
		ILayerPtr ipLayer;
		while( S_OK == ipEnumLayer->Next( &ipLayer ) )
		{
			if( !ipLayer )
				break;

			IFeatureClassPtr ipFeatClass;
			if( SUCCEEDED( ((IFeatureLayerPtr)ipLayer)->get_FeatureClass( &ipFeatClass ) ) )
			{
				// 道路リンクかつ、オーナ名が同じでないといけない
				if( AheGetFeatureClassName( ipLayer ).CompareNoCase( ROAD_TABLE_NAME ) == 0 && strTestOwnerName == AheGetFeatureClassOwnerName( ipFeatClass ))
				{
					ipRoadFeatureClass = ipFeatClass;
					break;
				}
			}
		}
	}
	return ipRoadFeatureClass;
}

IFeaturePtr CLQAttrIntersectionDlg::GetRoadFeature( CLQRowDef* pRowDef, LONG lOID )
{
	IFeaturePtr ipFeature;
	if( lOID > 0 )
	{
		// LQと同じワークスペースの道路リンクフィーチャクラスを取得
		IFeatureClassPtr ipRoadClass = GetRoadLinkWithLQ( pRowDef );
		// 流入リンクフィーチャ取得
		// 編集開始前は編集キャッシュがとれないので、全検索したほうがはやいので編集開始前開始後で場合わけ
		if( IsStartEditing() )
		{
			if( ipRoadClass )
				ipRoadClass->GetFeature( lOID, &ipFeature );
		}
		else{
			// 道路リンクテーブルからクエリ検索して探す
			IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, _T("TO_NODE_ID,FROM_NODE_ID,SHAPE"), _T("OBJECTID=%ld"), lOID );

			if( ipFilter )
			{
				IFeatureCursorPtr ipCursor;
				ipRoadClass->Search( ipFilter, VARIANT_FALSE, &ipCursor );
				ipCursor->NextFeature( &ipFeature );
			}
		}
	}

	return ipFeature;	
}
IFeatureClassPtr CLQAttrIntersectionDlg::GetRoadNodeFeatureClass( CLQRowDef* pRowDef )
{
	IFeatureClassPtr ipRoadNodeFeatureClass;
	if( !pRowDef )
		return NULL;

	_IRowPtr ipRow = pRowDef->GetFeature();
	ITablePtr ipTestTable;
	ipRow->get_Table( &ipTestTable );
	IDatasetPtr ipDataset = ipTestTable;
	CString strTestOwnerName = AheGetFeatureClassOwnerName( ipDataset );

	// TOCにいるすべてのジオフィーチャレイヤを取得してループ回しお目当ての道路ノード取得する
	IMapPtr ipMap( m_cArcHelper.GetMap() );
	IUIDPtr ipUID( CLSID_UID );
	ipUID->put_Value( CComVariant(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ); // IGeoFeatureLayer
	IEnumLayerPtr ipEnumLayer;
	if( SUCCEEDED( ipMap->get_Layers( ipUID, VARIANT_TRUE, &ipEnumLayer ) ) )
	{
		ipEnumLayer->Reset();
		ILayerPtr ipLayer;
		while( S_OK == ipEnumLayer->Next( &ipLayer ) )
		{
			if( !ipLayer )
				break;

			IFeatureClassPtr ipFeatClass;
			if( SUCCEEDED( ((IFeatureLayerPtr)ipLayer)->get_FeatureClass( &ipFeatClass ) ) )
			{
				// 道路ノードかつ、オーナ名が同じでないといけない
				if( AheGetFeatureClassName( ipLayer ).CompareNoCase( NODE_TABLE_NAME ) == 0 && strTestOwnerName == AheGetFeatureClassOwnerName( ipFeatClass ))
				{
					ipRoadNodeFeatureClass = ipFeatClass;
					break;
				}
			}
		}
	}
	return ipRoadNodeFeatureClass;
}
void CLQAttrIntersectionDlg::SetBaseIntersectionInfo()
{
	CLQRowDef* pLQRowDef = GetCurrentFeatureDef();
	IFeatureClassPtr ipFeatureClassNode = GetRoadNodeFeatureClass( pLQRowDef );
	if( pLQRowDef && ipFeatureClassNode )
	{
		LONG lDirC = 0;
		LONG lLinkID = 0;
		// リンク列の最後リンクのIDと向きを取得する
		if( GetTargetLinkAndDir( pLQRowDef, lDirC, lLinkID ) )
		{
			LONG lNodeID = 0;
			IFeaturePtr ipLinkFeature = GetRoadFeature( pLQRowDef, lLinkID );
			IFieldsPtr ipFields;
			long lidx = 0;
			ipLinkFeature->get_Fields( &ipFields );
			CComVariant cVal;
			if( lDirC == 1 )
			{
				ipFields->FindField( _T("TO_NODE_ID"), &lidx );
			}
			else
			{
				ipFields->FindField( _T("FROM_NODE_ID"), &lidx );
			}
			ipLinkFeature->get_Value( lidx, &cVal );
			lNodeID = cVal.lVal;

			IFeaturePtr ipNodeFeature;
			ipFeatureClassNode->GetFeature( lNodeID, &ipNodeFeature );
			ipFields = NULL;
			ipNodeFeature->get_Fields( &ipFields );
			CString strVal;
			// 道路ノードの代表交差点漢字名称
			ipFields->FindField( _T("NAME_KANJI"), &lidx );
			ipNodeFeature->get_Value( lidx, &cVal );
			strVal = cVal.bstrVal;
			CEdit cEdit_Kanji( GetDlgItem(IDC_EDIT_BASE_KANJI) );
			cEdit_Kanji.SetWindowTextW( strVal );

			// 道路ノードの代表交差点カナ名称
			ipFields->FindField( _T("NAME_YOMI"), &lidx );
			ipNodeFeature->get_Value( lidx, &cVal );
			strVal = cVal.bstrVal;
			CEdit cEdit_Yomi( GetDlgItem(IDC_EDIT_BASE_YOMI) );
			cEdit_Yomi.SetWindowTextW( strVal );
		}
	}
}
bool CLQAttrIntersectionDlg::GetTargetLinkAndDir(CLQRowDef* pRowDef, LONG &lDirC, LONG &lLinkID)
{
	LONG OID = pRowDef->GetRowOID();
	CRelationalTableDef* pRelTableDef = pRowDef->GetRelationalTableDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueIntersection, sindyeLoadQueueInfoLQTableName ) );
	for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
	{
		if( GetLongValue( &(*itLQRow), LASTLINK_F_FIELD ) == 1 )
		{
			lLinkID = GetLongValue( &(*itLQRow), LINK_ID_FIELD );
			lDirC = GetLongValue( &(*itLQRow), LINKDIR_C_FIELD );

			return true;
		}
	}
	return false;
}
LONG CLQAttrIntersectionDlg::GetLongValue( CRowDef* pRowDef, LPCTSTR lpcszFieldName )
{
	LONG lVal = -1;

	if( pRowDef )
	{
		const CFieldDef* pFieldDef = pRowDef->GetFieldDef( lpcszFieldName );
		if( pFieldDef != NULL )
		{
			if( pFieldDef->m_vaValue.vt == VT_I4 )
				lVal = pFieldDef->m_vaValue.lVal;
		}
	}
	return lVal;
}

LRESULT CLQAttrIntersectionDlg::OnBnClickedButtonBaseCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CLQRowDef* pLQRowDef = GetCurrentFeatureDef();
	CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueIntersection, sindyeLoadQueueInfoInfoTableName );

	TCHAR cBufKanji[MAX_PATH];
	CEdit cEdit_BaseKanji( GetDlgItem(IDC_EDIT_BASE_KANJI) );
	cEdit_BaseKanji.GetWindowTextW( cBufKanji, MAX_PATH );

	TCHAR cBufYomi[MAX_PATH];
	CEdit cEdit_BaseYomi( GetDlgItem(IDC_EDIT_BASE_YOMI) );
	cEdit_BaseYomi.GetWindowTextW( cBufYomi, MAX_PATH );

	CEdit cEdit_Kanji( GetDlgItem(IDC_EDIT_KANJI) );
	cEdit_Kanji.SetWindowTextW( cBufKanji );

	CEdit cEdit_Yomi( GetDlgItem(IDC_EDIT_YOMI) );
	cEdit_Yomi.SetWindowTextW( cBufYomi );

	_IRowPtr ipRow = pLQRowDef->GetFeature();
	if( ipRow )
	{
		IFieldsPtr ipFields;
		ipRow->get_Fields( &ipFields );
		long lidx = 0;
		ipFields->FindField( _T("NAME_KANJI"), &lidx );
		CComVariant cValKanji( cBufKanji );
		ipRow->put_Value( lidx, cValKanji );

		ipFields->FindField( _T("NAME_YOMI"), &lidx );
		CComVariant cValYomi( cBufYomi );
		ipRow->put_Value( lidx, cValYomi );
	}

	return 0;
}

void CLQAttrIntersectionDlg::SetToolTip(INT nTargetItem, BOOL bIsChange)
{
	// ToolInfo構造体 
	static TOOLINFO ti;
	if( !bIsChange )
	{
		// エディットボックスにかかれている文字列を取得
		CEdit edit( GetDlgItem(nTargetItem) );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
		edit.GetWindowText( lpszText, nTextLen );

		HWND hTmpWnd = NULL;
		switch( nTargetItem )
		{
			case IDC_EDIT_KANJI:
				hTmpWnd = m_hTipNameKanji;
				break;
			case IDC_EDIT_YOMI:
				hTmpWnd = m_hTipNameYomi;
				break;
			case IDC_EDIT_BASE_KANJI:
				hTmpWnd = m_hTipBaseNameKanji;
				break;
			case IDC_EDIT_BASE_YOMI:
				hTmpWnd = m_hTipBaseNameYomi;
				break;
			default:
				break;
		}
		if( hTmpWnd )
		{
			InitToolTip( nTargetItem, &ti );
			::SendMessage( hTmpWnd, TTM_GETTOOLINFO, 0, (LPARAM)&ti );
			ti.lpszText = lpszText;

			::SendMessage( hTmpWnd, TTM_SETTOOLINFO, 0, (LPARAM)&ti );
			return;
		}

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

		InitToolTip( nTargetItem, &ti );

		ti.lpszText = lpszText;
		switch( nTargetItem )
		{
			case IDC_EDIT_KANJI:
				m_hTipNameKanji = hToolTip;
				break;
			case IDC_EDIT_YOMI:
				m_hTipNameYomi = hToolTip;
				break;
			case IDC_EDIT_BASE_KANJI:
				m_hTipBaseNameKanji = hToolTip;
				break;
			case IDC_EDIT_BASE_YOMI:
				m_hTipBaseNameYomi = hToolTip;
				break;
			default:
				break;
		}
		hTmpWnd = hToolTip;
		::SendMessage( hTmpWnd, TTM_ADDTOOL , 0 , (LPARAM)&ti);
		::SendMessage( hTmpWnd, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);

		return;
	}
	else
	{
		// エディットボックスにかかれている文字列を取得
		CEdit edit( GetDlgItem(nTargetItem) );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
		edit.GetWindowText( lpszText, nTextLen );

		HWND hTmpWnd = NULL;
		switch( nTargetItem )
		{
			case IDC_EDIT_KANJI:
				hTmpWnd = m_hTipNameKanji;
				break;
			case IDC_EDIT_YOMI:
				hTmpWnd = m_hTipNameYomi;
				break;
			case IDC_EDIT_BASE_KANJI:
				hTmpWnd = m_hTipBaseNameKanji;
				break;
			case IDC_EDIT_BASE_YOMI:
				hTmpWnd = m_hTipBaseNameYomi;
				break;
			default:
				break;
		}
		if( hTmpWnd )
		{
			InitToolTip( nTargetItem, &ti );
			::SendMessage( hTmpWnd, TTM_GETTOOLINFO, 0, (LPARAM)&ti );
			ti.lpszText = lpszText;

			::SendMessage( hTmpWnd, TTM_SETTOOLINFO, 0, (LPARAM)&ti );
			return;
		}
	}
}

void CLQAttrIntersectionDlg::InitToolTip(INT nTargetItem, TOOLINFO *ti)
{
	ZeroMemory(&(*ti) , sizeof(TOOLINFO)) ; 
	(*ti).cbSize = sizeof(TOOLINFO) ; 
	(*ti).uFlags = TTF_IDISHWND | TTF_SUBCLASS; 
	(*ti).hwnd = GetDlgItem(nTargetItem);
	(*ti).uId = (UINT_PTR)GetDlgItem(nTargetItem).m_hWnd;
}
