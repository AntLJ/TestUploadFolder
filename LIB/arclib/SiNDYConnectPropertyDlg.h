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

// SiNDYConnectPropertyDlg.h : CSiNDYConnectPropertyDlg の宣言

#ifndef __SINDYCONNECTPROPERTYDLG_H_
#define __SINDYCONNECTPROPERTYDLG_H_

#ifndef _ROADCHECK
extern CComModule _Module;
#endif
#include "../arclib/Resource.h"       // メイン シンボル
#include <atlcom.h>
#include <atlwin.h>
#include <map>
#ifndef _AFXDLL
#include "WaitCursor.h"
#endif // _AFXDLL
#include "ATLControls.h"
#include "SiNDYConnectPropertySet.h"
#include <atlhost.h>
#include <arctl/coinitializer.h>

/////////////////////////////////////////////////////////////////////////////
// CSiNDYConnectPropertyDlg
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief ATL版SiNDY接続プロパティ設定ダイアログクラス
 *
 * @note rConnectPropertyの値は、キャンセル
 */
class CSiNDYConnectPropertyDlg : 
	public CAxDialogImpl<CSiNDYConnectPropertyDlg>
{
public:
	CSiNDYConnectPropertyDlg(SiNDYConnectPropertySet& rConnectPropertySet);
	~CSiNDYConnectPropertyDlg();

	enum { IDD = SG_IDD_SINDYCONNECTPROPERTYDLG };

BEGIN_MSG_MAP(CSiNDYConnectPropertyDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_HANDLER(SG_IDC_EDIT_SERVER, EN_CHANGE, OnChangeEditServer)
	COMMAND_HANDLER(SG_IDC_EDIT_INSTANCE, EN_CHANGE, OnChangeEditInstance)
	COMMAND_HANDLER(SG_IDC_EDIT_USER, EN_CHANGE, OnChangeEditUser)
	COMMAND_HANDLER(SG_IDC_EDIT_PASSWORD, EN_CHANGE, OnChangeEditPassword)
	COMMAND_HANDLER(SG_IDC_COMBO_VERSION, CBN_EDITUPDATE, OnEditupdateComboVersion)
	COMMAND_HANDLER(SG_IDC_COMBO_VERSION, CBN_DROPDOWN, OnDropdownComboVersion)
	COMMAND_HANDLER(SG_IDC_COMBO_VERSION, CBN_SELCHANGE, OnSelchangeComboVersion)
END_MSG_MAP()

// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeEditServer(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeEditInstance(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeEditUser(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeEditPassword(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditupdateComboVersion(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangeComboVersion(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDropdownComboVersion(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
// 設定/取得
	int GetDlgItemText(int nId, CString& rString);

// 処理
	bool updateVersionComboList();
	bool updateVersionComboList(IVersionedWorkspace* ipVerWorkspace);

// プロパティ
	SiNDYConnectPropertySet& m_rConnectPropertySet;
	SiNDYConnectPropertySet m_cConnectPropertySetBackup;
	HFONT m_hVersionDescriptionFont;

	SiNDYConnectPropertySet m_cConnectPropertySetVersionCombo;

	std::map<int, CString> m_cVersionDescriptionMap;
};

/////////////////////////////////////////////////////////////////////////////
// 実装
/////////////////////////////////////////////////////////////////////////////

// 構築/消滅
/////////////////////////////////////////////////////////////////////////////

inline CSiNDYConnectPropertyDlg::CSiNDYConnectPropertyDlg(SiNDYConnectPropertySet& rConnectPropertySet) :
m_rConnectPropertySet(rConnectPropertySet),
m_cConnectPropertySetBackup(rConnectPropertySet)
{

}

inline CSiNDYConnectPropertyDlg::~CSiNDYConnectPropertyDlg()
{

}

// メッセージ ハンドラ
/////////////////////////////////////////////////////////////////////////////

inline LRESULT CSiNDYConnectPropertyDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetDlgItemText(SG_IDC_EDIT_SERVER, m_rConnectPropertySet.m_strServer);
	SetDlgItemText(SG_IDC_EDIT_INSTANCE, m_rConnectPropertySet.m_strInstance);
	SetDlgItemText(SG_IDC_EDIT_USER, m_rConnectPropertySet.m_strUser);
	SetDlgItemText(SG_IDC_EDIT_PASSWORD, m_rConnectPropertySet.m_strPassword);
	SetDlgItemText(SG_IDC_COMBO_VERSION, m_rConnectPropertySet.m_strVersion);

	ATLControls::CEdit edit(GetDlgItem(SG_IDC_EDIT_VERSIONDESCRIPTION));
	HFONT hFont = edit.GetFont();
	LOGFONT font;
	::GetObject(hFont, sizeof(font), &font);
	font.lfHeight = 10;
	m_hVersionDescriptionFont = ::CreateFontIndirect(&font);
	edit.SetFont(m_hVersionDescriptionFont);

	return 1;  // システムにフォーカスを設定させます
}

inline LRESULT CSiNDYConnectPropertyDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	::DeleteObject(m_hVersionDescriptionFont);

	return 0;
}

inline LRESULT CSiNDYConnectPropertyDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_rConnectPropertySet.setPropertySet(m_cConnectPropertySetBackup);

	EndDialog(wID);
	::DeleteObject(m_hVersionDescriptionFont);

	return 0;
}

inline LRESULT CSiNDYConnectPropertyDlg::OnChangeEditServer(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(m_rConnectPropertySet.m_bConnect_AS)
		GetDlgItemText(wID, m_rConnectPropertySet.m_strServer);
	else
		GetDlgItemText(wID, m_rConnectPropertySet.m_strDBConnectionProperties);

	return 0;
}

inline LRESULT CSiNDYConnectPropertyDlg::OnChangeEditInstance(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// インスタンス文字列でDCかASかを判断する
	CString dlgText;
	GetDlgItemText(wID, dlgText);
	// DC接続の場合
	if(dlgText.MakeLower().Find(_T("sde:")) == 0)
	{
		// サーバ文字列を接続プロパティ文字列に移す
		if(!m_rConnectPropertySet.m_strServer.IsEmpty())
		{
			m_rConnectPropertySet.m_strDBConnectionProperties = m_rConnectPropertySet.m_strServer;
			m_rConnectPropertySet.m_strServer.Empty();
		}
		m_rConnectPropertySet.m_bConnect_AS = false; // 接続モード更新
		m_rConnectPropertySet.m_strInstance.Empty(); // インスタンス情報削除
	}
	// AS接続の場合
	else
	{
		// 接続プロパティ文字列をサーバ文字列へ移す
		if(!m_rConnectPropertySet.m_strDBConnectionProperties.IsEmpty())
		{
			m_rConnectPropertySet.m_strServer = m_rConnectPropertySet.m_strDBConnectionProperties;
			m_rConnectPropertySet.m_strDBConnectionProperties.Empty();
		}
		m_rConnectPropertySet.m_bConnect_AS = true;    // 接続モード更新
		m_rConnectPropertySet.m_strInstance = dlgText; // インスタンス更新
	}

	return 0;
}

inline LRESULT CSiNDYConnectPropertyDlg::OnChangeEditUser(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GetDlgItemText(wID, m_rConnectPropertySet.m_strUser);

	return 0;
}

inline LRESULT CSiNDYConnectPropertyDlg::OnChangeEditPassword(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GetDlgItemText(wID, m_rConnectPropertySet.m_strPassword);

	return 0;
}

inline LRESULT CSiNDYConnectPropertyDlg::OnEditupdateComboVersion(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GetDlgItemText(wID, m_rConnectPropertySet.m_strVersion);
	SetDlgItemText(SG_IDC_EDIT_VERSIONDESCRIPTION, _T(""));

	return 0;
}

inline LRESULT CSiNDYConnectPropertyDlg::OnSelchangeComboVersion(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ATLControls::CComboBox cb(GetDlgItem(SG_IDC_COMBO_VERSION));

	int nIndex = cb.GetCurSel();
	int nLen = cb.GetLBTextLen(nIndex);
	if(nLen != CB_ERR) {
		cb.GetLBText(nIndex, m_rConnectPropertySet.m_strVersion.GetBuffer(nLen + 1));
		m_rConnectPropertySet.m_strVersion.ReleaseBuffer();
	}

	SetDlgItemText(SG_IDC_EDIT_VERSIONDESCRIPTION, m_cVersionDescriptionMap[nIndex]);

	return 0;
}

inline LRESULT CSiNDYConnectPropertyDlg::OnDropdownComboVersion(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	updateVersionComboList();

	return 0;
}

// 処理
/////////////////////////////////////////////////////////////////////////////

inline bool CSiNDYConnectPropertyDlg::updateVersionComboList()
{
	// 接続設定に変更があったらリストを更新
	if(
		// AS接続設定条件
		(! (
			m_rConnectPropertySet.m_strServer.IsEmpty() ||
			m_rConnectPropertySet.m_strInstance.IsEmpty() ||
		    m_rConnectPropertySet.m_strUser.IsEmpty() ||
		    m_rConnectPropertySet.m_strPassword.IsEmpty()) &&
		! (
			m_rConnectPropertySet.m_strServer == m_cConnectPropertySetVersionCombo.m_strServer &&
			m_rConnectPropertySet.m_strInstance == m_cConnectPropertySetVersionCombo.m_strInstance &&
			m_rConnectPropertySet.m_strUser == m_cConnectPropertySetVersionCombo.m_strUser &&
			m_rConnectPropertySet.m_strPassword == m_cConnectPropertySetVersionCombo.m_strPassword))
		||
		// DC接続設定条件
		(! (
			m_rConnectPropertySet.m_strUser.IsEmpty() ||
		    m_rConnectPropertySet.m_strPassword.IsEmpty() ||
			m_rConnectPropertySet.m_strDBConnectionProperties.IsEmpty()) &&
		! (
			m_rConnectPropertySet.m_strUser == m_cConnectPropertySetVersionCombo.m_strUser &&
			m_rConnectPropertySet.m_strPassword == m_cConnectPropertySetVersionCombo.m_strPassword &&
			m_rConnectPropertySet.m_strDBConnectionProperties == m_cConnectPropertySetVersionCombo.m_strDBConnectionProperties))
		)
	{
		SiNDYConnectPropertySet cConnectPropertySet = m_rConnectPropertySet;
		cConnectPropertySet.m_strVersion = _T("SDE.DEFAULT");

		// 予備初期化
		HRESULT hr = ::CoInitialize(0);

#if 0	// TODO: arctl::coinitializerでOKなら、こちらは削除（下の方のip=NULLも）
IAoInitializePtr ip(CLSID_AoInitialize);
esriLicenseStatus e;
ip->Initialize( esriLicenseProductCodeStandard,&e);
#else
		const arctl::coinitializer aCoInitializer;
#endif

#ifndef _UNICODE
		if(SUCCEEDED(hr)) {
			// ArcGIS CQ00151553 に対応してみる
			IWorkspaceFactoryPtr pWorkspaceFact(CLSID_ShapefileWorkspaceFactory);
			IWorkspacePtr ipWorkspace;
			pWorkspaceFact->OpenFromFile(CComBSTR("C:\\"), 0, &ipWorkspace);
		}
#endif // _UNICODE

		{
			ATLControls::CComboBox cb(GetDlgItem(SG_IDC_COMBO_VERSION));
			
			// 初期化
			CString strCurrentVersion;
			GetDlgItemText(SG_IDC_COMBO_VERSION, strCurrentVersion);

			cb.ResetContent();
			m_cVersionDescriptionMap.clear();
			m_cConnectPropertySetVersionCombo.clear();

			cb.SetWindowText(strCurrentVersion);

			// 接続処理
			CWaitCursor wait;

			EnableWindow(FALSE);
			IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_SdeWorkspaceFactory);
			IWorkspacePtr ipWorkspace;
			ipWorkspaceFactory->Open(cConnectPropertySet.getPropertySet(), NULL, &ipWorkspace);
			IVersionedWorkspacePtr ipVerWorkspace = ipWorkspace;
			EnableWindow(TRUE);

			if(ipWorkspace == 0) {
				MessageBox(_T("データベースへの接続に失敗しました。"), _T("接続失敗"), MB_OK | MB_ICONWARNING);
				return false;
			}

			updateVersionComboList(IVersionedWorkspacePtr(ipWorkspace));

			cb.SelectString(-1, strCurrentVersion);

			// 列挙済み接続設定を更新
			m_cConnectPropertySetVersionCombo.setPropertySet(m_rConnectPropertySet);
		}
#if 0
ip=NULL;
#endif
		// 予備解放
		if(SUCCEEDED(hr)) {
			::CoUninitialize();
		}
	}

	return true;
}

inline bool CSiNDYConnectPropertyDlg::updateVersionComboList(IVersionedWorkspace* ipVerWorkspace)
{
	if(! ipVerWorkspace)
		return false;

	ATLControls::CComboBox cb(GetDlgItem(SG_IDC_COMBO_VERSION));
	
	// バージョン列挙
	IEnumVersionInfoPtr ipEnumVerInfo;
	ipVerWorkspace->get_Versions(&ipEnumVerInfo);

	IVersionInfoPtr ipVerInfo;
	for(int i = 0; SUCCEEDED(ipEnumVerInfo->Next(&ipVerInfo)) && ipVerInfo != 0; i++) {
		CComBSTR bstrName;
		ipVerInfo->get_VersionName(&bstrName);
		cb.AddString(CString(bstrName));

		CComBSTR bstrDescription;
		ipVerInfo->get_Description(&bstrDescription);
		m_cVersionDescriptionMap[i] = bstrDescription;
	}

	return true;
}

// 設定/取得
/////////////////////////////////////////////////////////////////////////////

inline int CSiNDYConnectPropertyDlg::GetDlgItemText(int nId, CString& rString)
{
	TCHAR sz[256];
	int nResult = CWindow::GetDlgItemText(nId, sz, 256);
	rString = sz;
	return nResult;
}

#endif //__SINDYCONNECTPROPERTYDLG_H_
