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

 AttrCSDBDlg.cpp : CAttrCSDBDlg のインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrCSDBDlg.h"

/// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(DEFAULTMAP)
	BEGINROWS(WRCT_REST,0,0)
		RCREST(IDC_GRIDCTRL)
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrCSDBDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrCSDBDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCSDBDlg::CAttrCSDBDlg()
 * @brief コンストラクタ
 *
 * なにもしません
 */
CAttrCSDBDlg::CAttrCSDBDlg() 
	: ATTR_BASE_CLASS(DEFAULTMAP, IDD_CSDBDLG)
{
	m_lPosition = -1;
}
/**
 * ダイアログのサイズ変更があったときのイベントハンドラ
 */
LRESULT CAttrCSDBDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RefreshGrid();
	bHandled = FALSE;

	return 0;
}

/** 
 * @fn HWND CAttrCSDBDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrCSDBDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// ダイアログの作成はATTR_BASE_CLASSに任せます
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCSDBDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrCSDBDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CAttrCSDBDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCSDBDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	m_spGrid->put_Redraw( flexRDNone );
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	SetHiddenRow();			// [bug 7714] [e][要望]nullの項目を非表示にして欲しい

	long lCols = -1, lRows = -1;
	m_spGrid->get_Cols( &lCols );
	m_spGrid->put_ExtendLastCol( TRUE );
	m_spGrid->put_AutoSizeMode( flexAutoSizeColWidth );
	m_spGrid->AutoSize( 0, lCols -1 );
	m_spGrid->put_AutoSizeMode( flexAutoSizeRowHeight );
	m_spGrid->AutoSize( 0, lCols -1 );

	m_spGrid->put_Ellipsis( flexEllipsisEnd );
	m_spGrid->put_Redraw( (RedrawSettings)VARIANT_TRUE );

	// GridEditModeじゃないとBaseDlgで弾かれるのでここで再設定
	m_spGrid->put_Editable(flexEDKbdMouse);

	return bRet;
}

/**
 * @fn void CAttrCSDBDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrCSDBDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// 標準グリッド
		m_cControlRel.SetControl( IDC_GRIDCTRL,	TYPE_FEATUREGRID, 0, 0 );
	}
}

void CAttrCSDBDlg::SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList )
{ 
	// セットされるFeatureDef（ジオメトリのみのフィーチャ）を横取りして
	// 対応する属性を別テーブルから引っ張ってきて置き換える
	m_lstDmyFeatureDef.clear();
	for( std::list<CFeatureDef>::iterator it=pFeatureDefList->begin(); it!=pFeatureDefList->end(); ++it )
	{
		IFeaturePtr ipFeature = it->GetFeature();
		IFieldsPtr ipFields;
		ipFeature->get_Fields( &ipFields );
		long lIndex = 0;

		ITablePtr ipTable;
		ipFeature->get_Table( &ipTable );
		if( !ipTable )
			continue;

		IDatasetPtr ipDataSet( ipTable );
		if( !ipDataSet )
			continue;

		CComBSTR cComBSTR;
		ipDataSet->get_Name( &cComBSTR );
		CString strFeatureName( cComBSTR );
		long ldot = strFeatureName.ReverseFind( '.' );
		if( ldot != -1 )
			strFeatureName = strFeatureName.Mid( ldot+1 );

		// CSDBとNAVIの場合で処理を分ける
		CString strTableName;
		if( strFeatureName.CompareNoCase( _T("CSDB") ) == 0 )
			strTableName.Format( _T("ATTR_CSDB") );
		else if( strFeatureName.CompareNoCase( _T("NAVI") ) == 0 )
			strTableName.Format( _T("ATTR_NAVI") );

		ipFields->FindField( CComBSTR("ATTRTBLID"), &lIndex );

		// CSDBのフィーチャクラスの時だけ属性テーブルを読みにいく
		if( lIndex > -1)
		{
			CComVariant vaVal;
			ipFeature->get_Value( lIndex, &vaVal );

			// 属性テーブルを開いてIDに対応する属性を取得
			ITablePtr ipTable = AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), strTableName );
			if( !ipTable )
			{
				CString msg;
				msg.Format( AheLoadString( IDS_NOTFINDTABLE ), strTableName );
				AttrMessageBox( msg, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				m_lstDmyFeatureDef = *pFeatureDefList;
				break;
			}
			_IRowPtr ipAttrRow;
			ipTable->GetRow( vaVal.lVal, &ipAttrRow );

			CFeatureDef featDef;
			featDef.clear();
			featDef.SetArcHelper( m_cArcHelper.GetApp() );
			featDef.SetFeature( ipAttrRow );

			// この関数を抜けても実体を保持していないといけないのでメンバ変数を使う
			m_lstDmyFeatureDef.push_back( featDef );
		}
	}

	ATTR_BASE_CLASS::SetFeatureDefList( &m_lstDmyFeatureDef ); 
}

void CAttrCSDBDlg::SetAliasOrField( BOOL bAliasOrField )
{
	for( std::list<CFeatureDef>::iterator it = m_lstDmyFeatureDef.begin(); it != m_lstDmyFeatureDef.end(); ++it )
	{
		// 常にエイリアス表示
		m_cControlRel.SetAliasOrField( FALSE );
	}
}

void CAttrCSDBDlg::CreateAliasMap( _IRowPtr ipRow )
{
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );
	long lFields = 0;
	ipFields->get_FieldCount( &lFields );
	for( int i=0; i<lFields; ++i)
	{
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		CComBSTR bstrFieldName, bstrAlias;
		ipField->get_Name( &bstrFieldName );
		ipField->get_AliasName( &bstrAlias );
		m_mapDB2Alias[ CString(bstrFieldName) ] = CString( bstrAlias );
		m_mapAlias2DB[ CString(bstrAlias) ] = CString( bstrFieldName );
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// グリッドコントロール用
//
/////////////////////////////////////////////////////////////////////////////

HRESULT CAttrCSDBDlg::CreateActiveXControls(UINT nID)
{
	// Load dialog template and InitData
	HRSRC hDlgInit = ::FindResource(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), (LPTSTR)_ATL_RT_DLGINIT);
	BYTE* pInitData = NULL;
	HGLOBAL hData = NULL;
	HRESULT hr = S_OK;
	if (hDlgInit != NULL)
	{
		hData = ::LoadResource(_AtlBaseModule.GetResourceInstance(), hDlgInit);
		if (hData != NULL)
			pInitData = (BYTE*) ::LockResource(hData);
	}

	HRSRC hDlg = ::FindResource(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), (LPTSTR)RT_DIALOG);
	if (hDlg != NULL)
	{
		HGLOBAL hResource = ::LoadResource(_AtlBaseModule.GetResourceInstance(), hDlg);
		DLGTEMPLATE* pDlg = NULL;
		if (hResource != NULL)
		{
			pDlg = (DLGTEMPLATE*) ::LockResource(hResource);
			if (pDlg != NULL)
			{
				// Get first control on the template
				BOOL bDialogEx = _DialogSplitHelper::IsDialogEx(pDlg);
				WORD nItems = _DialogSplitHelper::DlgTemplateItemCount(pDlg);

				// Get first control on the dialog
				DLGITEMTEMPLATE* pItem = _DialogSplitHelper::FindFirstDlgItem(pDlg);
				HWND hWndPrev = GetWindow(GW_CHILD);

				// Create all ActiveX cotnrols in the dialog template and place them in the correct tab order (z-order)
				for (WORD nItem = 0; nItem < nItems; nItem++)
				{
					DWORD wID = bDialogEx ? ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : pItem->id;
					if (_DialogSplitHelper::IsActiveXControl(pItem, bDialogEx))
					{
						BYTE* pData = NULL;
						DWORD dwLen = _DialogSplitHelper::FindCreateData(wID, pInitData, &pData);
						CComPtr<IStream> spStream;
						if (dwLen != 0)
						{
							HGLOBAL h = GlobalAlloc(GHND, dwLen);
							if (h != NULL)
							{
								BYTE* pBytes = (BYTE*) GlobalLock(h);
								BYTE* pSource = pData; 
								Checked::memcpy_s(pBytes, dwLen, pSource, dwLen);
								GlobalUnlock(h);
								CreateStreamOnHGlobal(h, TRUE, &spStream);
							}
							else
							{
								hr = E_OUTOFMEMORY;
								break;
							}
						}

						CComBSTR bstrLicKey;
						hr = _DialogSplitHelper::ParseInitData(spStream, &bstrLicKey.m_str);
						if (SUCCEEDED(hr))
						{
							CAxWindow2 wnd;
							// Get control caption.
							LPWSTR pszClassName = 
								bDialogEx ? 
									(LPWSTR)(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem) + 1) :
									(LPWSTR)(pItem + 1);
							// Get control rect.
							RECT rect;
							rect.left = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->x : 
									pItem->x;
							rect.top = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->y : 
									pItem->y;
							rect.right = rect.left + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cx : 
									pItem->cx);
							rect.bottom = rect.top + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cy : 
									pItem->cy);

							// Convert from dialog units to screen units
							MapDialogRect(&rect);

							// Create AxWindow with a NULL caption.
							wnd.Create(m_hWnd, 
								&rect, 
								NULL, 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->style : 
									pItem->style) | WS_TABSTOP, 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->exStyle : 
									0,
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : 
									pItem->id,
								NULL);

							if (wnd != NULL)
							{
								// Set the Help ID
								if (bDialogEx && ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID != 0)
									wnd.SetWindowContextHelpId(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID);
								// Try to create the ActiveX control.
								static WCHAR pwchLicenseKey[] =
								{
									0x004F,	0x0030,	0x0045,	0x0054,	0x0054,	0x0043,	
									0x0037,	0x0034,	0x0037
								};
								hr = wnd.CreateControlLic(pszClassName, spStream, NULL, /*bstrLicKey*/CComBSTR(pwchLicenseKey));
								if (FAILED(hr))
									break;
								// Set the correct tab position.
								if (nItem == 0)
									hWndPrev = HWND_TOP;
								wnd.SetWindowPos(hWndPrev, 0,0,0,0,SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
								hWndPrev = wnd;
							}
							else
							{
								hr = AtlHresultFromLastError();
							}
						}
					}
					else
					{
						if (nItem != 0)
							hWndPrev = ::GetWindow(hWndPrev, GW_HWNDNEXT);
					}
					pItem = _DialogSplitHelper::FindNextDlgItem(pItem, bDialogEx);
				}
			}
			else
				hr = AtlHresultFromLastError();
		}
		else
			hr = AtlHresultFromLastError();
	}
	return hr;
}

// [bug 7714] [e][要望]nullの項目を非表示にして欲しい
void CAttrCSDBDlg::SetHiddenRow()
{
	CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
	if( !pFeatureDef )
		return;

	_IRowPtr ipCurrent = pFeatureDef->GetFeature();
	if( !ipCurrent )
		return;

	ITablePtr ipCurrentTable;
	ipCurrent->get_Table( &ipCurrentTable );
	if( !ipCurrentTable )
		return;

	IDatasetPtr ipDataSet( ipCurrentTable );
	if( !ipDataSet )
		return;

	CComBSTR cComBSTR;
	ipDataSet->get_Name( &cComBSTR );
	CString strFeatureName( cComBSTR );
	long ldot = strFeatureName.ReverseFind( '.' );
	if( ldot != -1 )
		strFeatureName = strFeatureName.Mid( ldot+1 );

	// CSDBとNAVIの場合で処理を分ける
	long lChangeCase = 0;
	if( strFeatureName.CompareNoCase( _T("ATTR_CSDB") ) == 0 )
		lChangeCase = 0;		// CSDBの場合 0
	else if( strFeatureName.CompareNoCase( _T("ATTR_NAVI") ) == 0 )
		lChangeCase = 1;		// NAVIの場合 1
	else
		return;

	// 位置番号
	for( long i=1; i<=10; ++i )
	{
		std::list<CString> strList;
		CString strLOC;

		strLOC.Format( _T("LOC%02d_NAME"), i );
		strList.push_back( strLOC );
		strLOC.Format( _T("LOC%02d_LAT"), i );
		strList.push_back( strLOC );
		strLOC.Format( _T("LOC%02d_LONG"), i );
		strList.push_back( strLOC );

		_SetHiddenRow( ipCurrent, strList );

	}
	// 二次調査02 or 回答・調査
	for( long i=1; i<=10; ++i )
	{
		std::list<CString> strList;
		CString strINQ;

		switch( lChangeCase )
		{
			case 0:	// ATTR_CSDB
			default:
				strINQ.Format( _T("INQ2_%02dFCT"), i );
				strList.push_back( strINQ );
				strINQ.Format( _T("INQ2_%02dDTL"), i );
				strList.push_back( strINQ );
				strINQ.Format( _T("INQ2_%02dTGT"), i );
				strList.push_back( strINQ );
				break;
			case 1: // ATTR_NAVI
				strINQ.Format( _T("INQ_%02dFCT"), i );
				strList.push_back( strINQ );
				strINQ.Format( _T("INQ_%02dDTL"), i );
				strList.push_back( strINQ );
				strINQ.Format( _T("INQ_%02dTGT"), i );
				strList.push_back( strINQ );
				break;
		}

		_SetHiddenRow( ipCurrent, strList );

	}
}

void CAttrCSDBDlg::_SetHiddenRow(_IRowPtr ipRow, std::list<CString>& strList)
{
	if( !ipRow )	return;

	ITablePtr ipTable;
	ipRow->get_Table( &ipTable );
	if( !ipTable )	return;

	std::vector<long> vecHidden;
	// リスト内フィールド値がすべてnullの時のみ
	for( std::list<CString>::iterator it = strList.begin(); it != strList.end(); ++it )
	{
		long lidx = 0;
		if( FAILED( ipTable->FindField( CComBSTR( (*it) ), &lidx ) ) )
			return;

		CComVariant cVal;
		if( FAILED( ipRow->get_Value( lidx, &cVal ) ) )
			return;
		
		if( cVal.vt != VT_NULL )
			return;

		vecHidden.push_back( lidx+1 );
	}

	// グリッドの行を非表示にする
	for( std::vector<long>::iterator it = vecHidden.begin(); it != vecHidden.end(); ++it )
		m_spGrid->put_RowHidden( (*it), VARIANT_TRUE );
}
