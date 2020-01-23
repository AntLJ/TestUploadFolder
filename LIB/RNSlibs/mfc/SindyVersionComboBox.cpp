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

// SindyVersionComboBox.cpp : インプリメンテーション ファイル
//
// $Id: SindyVersionComboBox.cpp,v 1.1.1.1 2003/09/12 07:26:04 f_kiku Exp $

#include "stdafx.h"
#include "SindyVersionComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSindyVersionComboBox

CSindyVersionComboBox::CSindyVersionComboBox() :
m_pConnectProperty(0)
{
}

CSindyVersionComboBox::~CSindyVersionComboBox()
{
}


BEGIN_MESSAGE_MAP(CSindyVersionComboBox, CComboBox)
	//{{AFX_MSG_MAP(CSindyVersionComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSindyVersionComboBox メッセージ ハンドラ

BOOL CSindyVersionComboBox::PreTranslateMessage(MSG* pMsg)
{
	// エスケープキーによるキャンセルを不可にする
    if(pMsg->message == WM_KEYDOWN)
    {
		switch(pMsg->wParam) {
		case VK_UP:
		case VK_DOWN:
			reloadList();
			SetFocus();
			return FALSE;
		}
    }
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CSindyVersionComboBox::OnDropdown() 
{
	reloadList();
}

/////////////////////////////////////////////////////////////////////////////
// 設定
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief ドロップダウンリストの項目を再設定する
 */
void CSindyVersionComboBox::reloadList()
{
	ATLASSERT(m_pConnectProperty);

	// 接続設定に変更があったらリストを更新
	if( ! ( m_pConnectProperty->e_strServer.IsEmpty() ||
			m_pConnectProperty->e_strInstance.IsEmpty() ||
		    m_pConnectProperty->e_strUser.IsEmpty() ||
		    m_pConnectProperty->e_strPassword.IsEmpty() ) &&
		( m_cConnectingProperty.e_strServer != m_pConnectProperty->e_strServer ||
		  m_cConnectingProperty.e_strInstance != m_pConnectProperty->e_strInstance ||
		  m_cConnectingProperty.e_strUser != m_pConnectProperty->e_strUser ||
		  m_cConnectingProperty.e_strPassword!= m_pConnectProperty->e_strPassword )
	)
	{
		CSindyConnectProperty aProperty = *m_pConnectProperty;
		aProperty.e_strVersion = _T("SDE.DEFAULT");

		// 予備初期化
		HRESULT hr = ::CoInitialize(0);

		// 初期化
		CString strText;
		GetWindowText(strText);
		ResetContent();
		m_cConnectingProperty.clear();
		SetWindowText(strText);

		// 接続処理
		CWaitCursor wait;

		EnableWindow(FALSE);
		IVersionedWorkspacePtr ipVerWorkspace = ScmWorkspace(aProperty.getPropertySet());
		EnableWindow(TRUE);

		if(ipVerWorkspace == 0) {
			MessageBox(_T("データベースへの接続に失敗しました。"), _T("接続失敗"), MB_OK | MB_ICONWARNING);
			return;
		}

		// バージョン列挙
		IEnumVersionInfoPtr ipEnumVerInfo;
		ipVerWorkspace->get_Versions(&ipEnumVerInfo);

		IVersionInfoPtr ipVerInfo;
		while(SUCCEEDED(ipEnumVerInfo->Next(&ipVerInfo)) && ipVerInfo != 0) {
			CComBSTR bstrName;
			ipVerInfo->get_VersionName(&bstrName);
			std::string aName = RNSSindyPlant::wcsToMbs(bstrName);
			AddString(aName.c_str());
		}
		
		// 列挙済み接続設定を更新
		m_cConnectingProperty = *m_pConnectProperty;

		// 解放
		ipEnumVerInfo = 0;
		ipVerInfo = 0;
		ipVerWorkspace = 0;

		// 予備解放
		if(SUCCEEDED(hr)) {
			::CoUninitialize();
		}
	}
}
