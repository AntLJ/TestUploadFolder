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

// LQAttrDefaultDlg.cpp : CLQAttrDefaultDlg のインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrDefaultDlg.h"

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
 * @retval CLQAttrDefaultDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrDefaultDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDefaultDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CLQAttrDefaultDlg::CLQAttrDefaultDlg()
 * @brief コンストラクタ
 *
 * なにもしません
 */
CLQAttrDefaultDlg::CLQAttrDefaultDlg() 
	: CLQAttrBaseDlg(DEFAULTMAP, IDD_LQATTRDEFAULTDLG)
{
}

/** 
 * @fn HWND CLQAttrDefaultDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * CLQAttrBaseDlg::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CLQAttrDefaultDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// ダイアログの作成はCLQAttrBaseDlgに任せます
	m_hChildWnd = CLQAttrBaseDlg::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrDefaultDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CLQAttrDefaultDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrDefaultDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CLQAttrDefaultDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = CLQAttrBaseDlg::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

    return bRet;
}

/**
 * @fn void CLQAttrDefaultDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CLQAttrDefaultDlg::CreateControlRelation()
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

/////////////////////////////////////////////////////////////////////////////
//
// グリッドコントロール用
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @fn BOOL CLQAttrDefaultDlg::StartEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col )
 * グリッドのセル編集開始時に呼ばれる
 *
 * @param nID		[in]	グリッドのコントロールID
 * @param spGrid	[in]	グリッドコントロール
 * @param Row		[in]	編集対象セルの行
 * @param Col		[in]	編集対象セルの列
 *
 * @return 編集できるセルなら TRUE、出来ないセルなら FALSE
 */
BOOL CLQAttrDefaultDlg::StartEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col )
{
	BOOL bRet = m_cControlRel.ClickOtherControl( nID, Row, Col, m_bIsGridEditMode );

	return bRet;
}
/**
 * @fn void CLQAttrDefaultDlg::AfterEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col )
 * グリッドのセル編集終了時に呼ばれる
 * 
 * @param nID		[in]	グリッドのコントロールID
 * @param spGrid	[in]	グリッドコントロール
 * @param Row		[in]	編集対象セルの行
 * @param Col		[in]	編集対象セルの列
 */
void CLQAttrDefaultDlg::AfterEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col )
{
	USES_CONVERSION;

	// 作成したコンボボックスはその都度消去しないとダメ
	spGrid->put_ComboList( CComBSTR(_T("")));

	m_cControlRel.EditedOtherControl( nID, Row, Col );	// 編集を CFieldDef に適用
	m_cControlRel.ControlChanged( nID );					// 追従するコントロールがあるならそれも適用
	// 変更された場合はボタンを Enable に
	SetButton( Changed() );
}

// FlexGridのライセンスウィンドウを出さないようにするための処理
HRESULT CLQAttrDefaultDlg::CreateActiveXControls(UINT nID)
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
