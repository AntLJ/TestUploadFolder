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

// AttrDRMALinkDlg.cpp: CAttrDRMALinkDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrDRMALinkDlg.h"

///  ウィンドウマネージャ配置マップ（基本属性タブ用）
BEGIN_WINDOW_MAP(DRMAKIHONMAP)
	BEGINROWS(WRCT_REST,IDC_TAB_DRMAATTR,RCMARGINS(2,2))
		RCSPACE(22)	// タブのつまみ分
		RCSPACE(30)	
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_DISPCLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_DISPCLASS)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MANAGER)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_MANAGER)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ROADWIDTH)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_ROADWIDTH)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LINKCLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LINKCLASS1)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// ウィンドウマネージャ配置マップ（追加属性タブ用）
BEGIN_WINDOW_MAP(DRMATUIKAMAP)
	BEGINROWS(WRCT_REST,IDC_TAB_DRMAATTR,RCMARGINS(2,2))
		RCSPACE(22)	// タブのつまみ分
		RCSPACE(30)	
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_BYPASS)
				RCTOFIT(IDC_CHECK_CARONLYF)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LANE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LANE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// ウィンドウマネージャ配置マップ（P_LINK用）
BEGIN_WINDOW_MAP(DRMAKOTUKISEIMAP)
	BEGINROWS(WRCT_REST,IDC_TAB_DRMAATTR,RCMARGINS(2,2))
		RCSPACE(22)	// タブのつまみ分
		RCSPACE(30)	
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_REGULER)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_REGULER)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LINKOK)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LINKOK)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// ウィンドウマネージャ配置マップ（路線番号タブ用）
BEGIN_WINDOW_MAP(DRMAROSENMAP)
	BEGINROWS(WRCT_REST,IDC_TAB_DRMAATTR,RCMARGINS(2,2))
		RCSPACE(22)	// タブのつまみ分
		RCSPACE(30)	
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(5,5))
				RCTOFIT(IDC_STATIC_ROADNUMBER)
				RCTOFIT(-1)
				RCTOFIT(IDC_EDIT_ROADNUMBER)
				RCTOFIT(IDC_STATIC_ROADNUMBER2)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,50)
			BEGINROWS(WRCT_REST, 0, RCMARGINS(5,5))
				RCREST(IDC_GRID_DUPLILINK)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// ウィンドウマネージャ配置マップ（DRMAタブ用）
BEGIN_WINDOW_MAP(DRMADRMAMAP)
	BEGINROWS(WRCT_REST,IDC_TAB_DRMAATTR,RCMARGINS(2,2))
		RCSPACE(22)	// タブのつまみ分
		RCSPACE(30)	
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KUBUNCODE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_KUBUNCODE)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LIMITSPEED)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LIMITSPEED)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_TRIPSPEED)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_TRIPSPEED)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrDRMALinkDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrDRMALinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDRMALinkDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrDRMALinkDlg::CAttrDRMALinkDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrDRMALinkDlg::CAttrDRMALinkDlg() 
	: ATTR_BASE_CLASS(DRMAKIHONMAP, IDD_ATTRDRMALINKDLG)
{
	// タブ位置の初期化
	m_iChangeType = 0;
	m_lDRMATabNum = 0;
}

/** 
 * @fn HWND CAttrDRMALinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrDRMALinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrDRMALinkDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrDRMALinkDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrDRMALinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrDRMALinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{
	CreateControlRelation();
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_lDRMATabNum, bForce, bEditable );

	return bRet;
}

/**
 * @fn LONG CAttrDRMALinkDlg::GetTabNum()
 * @brief 現在のタブ位置を渡します
 *
 * @retval 現在のタブ位置
 */
LONG CAttrDRMALinkDlg::GetTabNum()
{
	return m_lDRMATabNum;
}

/**
 * @fn void CAttrDRMALinkDlg::SetTabNum(LONG lTabNum)
 * @brief タブ位置をセットします
 *
 * @param lTabNum	[in]	タブ位置
 */
void CAttrDRMALinkDlg::SetTabNum(LONG lTabNum)
{
	m_lDRMATabNum = lTabNum;
	m_iChangeType = lTabNum;
}

/**
 * @fn void CAttrDRMALinkDlg::ChangeWindowMgrMap( INT iIndex )
 * ウィンドウマネージャのマップの切り替えを行う
 *
 * @param iIndex	[in]	マップのインデックス番号
 */
void CAttrDRMALinkDlg::ChangeWindowMgrMap( INT iIndex )
{
	switch( iIndex ) {
	case EDITVIEW_DRMA_KIHON:		// 基本属性タブ
		m_winMgr.ChangeMgrMap(DRMAKIHONMAP);
		break;
	case EDITVIEW_DRMA_TUIKA:		// 追加属性タブ
		m_winMgr.ChangeMgrMap(DRMATUIKAMAP);
		break;
	case EDITVIEW_DRMA_KOTUKISEI:	// 交通規制タブ
		m_winMgr.ChangeMgrMap(DRMAKOTUKISEIMAP);
		break;
	case EDITVIEW_DRMA_ROSEN:		// 路線番号タブ 
		m_winMgr.ChangeMgrMap(DRMAROSENMAP);
		break;
	case EDITVIEW_DRMA_DRMA:		// DRMAタブ 
		m_winMgr.ChangeMgrMap(DRMADRMAMAP);
		break;
	default:
		break;
	}
	// 再配置
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);	
}

/**
 * @fn void CAttrDRMALinkDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrDRMALinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		std::list<CString> array;
		
		// 道路の基本属性タブ
		m_cControlRel.SetControl( IDC_TAB_DRMAATTR,		TYPE_OTHER,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_DRMA );
		m_cControlRel.SetControl( IDC_STATIC_DISPCLASS,	TYPE_OTHER,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_DISPCLASS,	TYPE_COMBO,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON, _T("DISPCODE"),			NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_MANAGER,	TYPE_OTHER,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_MANAGER,	TYPE_COMBO,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON, _T("MANAGECODE"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_COMBO_ROADWIDTH,	TYPE_COMBO,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON, _T("ROADWIDTHCODE"),	NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_ROADWIDTH,	TYPE_OTHER,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_LINKCLASS1,	TYPE_COMBO,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON, _T("LINKKINDCODE"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_LINKCLASS,	TYPE_OTHER,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON );
		// 道路の追加属性タブ
		m_cControlRel.SetControl( IDC_CHECK_BYPASS,		TYPE_CHECKBOX,	EDITVIEW_DRMA_TUIKA, EDITVIEW_DRMA_TUIKA, _T("BYPASS_FLAG"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_CHECK_CARONLYF,	TYPE_CHECKBOX,	EDITVIEW_DRMA_TUIKA, EDITVIEW_DRMA_TUIKA, _T("CARONLY_FLAG"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_LANE,		TYPE_OTHER,		EDITVIEW_DRMA_TUIKA, EDITVIEW_DRMA_TUIKA );
		m_cControlRel.SetControl( IDC_COMBO_LANE,		TYPE_COMBO,		EDITVIEW_DRMA_TUIKA, EDITVIEW_DRMA_TUIKA, _T("ROADLANECODE"),		NULL, m_strFeatureClassName );
		// 道路の交通規制タブ
		m_cControlRel.SetControl( IDC_STATIC_REGULER,	TYPE_OTHER,		EDITVIEW_DRMA_KOTUKISEI, EDITVIEW_DRMA_KOTUKISEI );
		m_cControlRel.SetControl( IDC_COMBO_REGULER,	TYPE_COMBO,		EDITVIEW_DRMA_KOTUKISEI, EDITVIEW_DRMA_KOTUKISEI, _T("REGSCODE"),	NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_LINKOK,	TYPE_OTHER,		EDITVIEW_DRMA_KOTUKISEI, EDITVIEW_DRMA_KOTUKISEI );
		m_cControlRel.SetControl( IDC_COMBO_LINKOK,		TYPE_COMBO,		EDITVIEW_DRMA_KOTUKISEI, EDITVIEW_DRMA_KOTUKISEI, _T("LINKPASSCODE"),NULL, m_strFeatureClassName );
		// 道路の路線タブ
		m_cControlRel.SetControl( IDC_STATIC_ROADNUMBER,	TYPE_OTHER,		EDITVIEW_DRMA_ROSEN, EDITVIEW_DRMA_ROSEN );
		m_cControlRel.SetControl( IDC_EDIT_ROADNUMBER,		TYPE_EDIT,		EDITVIEW_DRMA_ROSEN, EDITVIEW_DRMA_ROSEN, _T("ROADNO"),			NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_ROADNUMBER2,	TYPE_OTHER,		EDITVIEW_DRMA_ROSEN, EDITVIEW_DRMA_ROSEN );
		array.clear();
		array.push_back( _T("ROAD_CODE") );
		array.push_back( _T("NAME_KANJI") );
		array.push_back( _T("NAME_YOMI") );
		m_cControlRel.SetControl( IDC_GRID_DUPLILINK, TYPE_OUTERROWS, EDITVIEW_DRMA_ROSEN, EDITVIEW_DRMA_ROSEN, array, NULL, DUPLILINK_TABLE_NAME, _T("LINK_ID"), IDC_CHECK_HASDUPLI, FOLLOW_VALUE );
		
		// 道路のDRMAタブ
		m_cControlRel.SetControl( IDC_STATIC_KUBUNCODE,		TYPE_OTHER,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA );
		m_cControlRel.SetControl( IDC_COMBO_KUBUNCODE,		TYPE_COMBO,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA, _T("ROADSECCODE"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_LIMITSPEED,	TYPE_OTHER,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA );
		m_cControlRel.SetControl( IDC_COMBO_LIMITSPEED,		TYPE_COMBO,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA, _T("REGSSPEEDCODE"),	NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_TRIPSPEED,		TYPE_OTHER,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA );
		m_cControlRel.SetControl( IDC_EDIT_TRIPSPEED,		TYPE_EDIT,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA, _T("TRAFFICSPEED"),		NULL, m_strFeatureClassName );
	}
}

HRESULT CAttrDRMALinkDlg::CreateActiveXControls(UINT nID)
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
