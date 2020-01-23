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

// SindyConnectDlg.cpp : インプリメンテーション ファイル
//
// $Id: SindyConnectDlg.cpp,v 1.2 2004/01/14 06:00:14 f_kiku Exp $

#include "stdafx.h"
#include "SindyConnectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSindyConnectDlg ダイアログ


CSindyConnectDlg::CSindyConnectDlg(CSindyIniConnectProperty& rProperty, CWnd* pParent /*=NULL*/)
	: CDialog(CSindyConnectDlg::IDD, pParent)
	, m_rProperty(rProperty)
	, m_bIniProperty(TRUE)
{
	//{{AFX_DATA_INIT(CSindyConnectDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

CSindyConnectDlg::CSindyConnectDlg(CSindyConnectProperty& rProperty, CWnd* pParent /*=NULL*/)
	: CDialog(CSindyConnectDlg::IDD, pParent)
	, m_rProperty(rProperty)
	, m_bIniProperty(FALSE)
{
	//{{AFX_DATA_INIT(CSindyConnectDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CSindyConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSindyConnectDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSindyConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CSindyConnectDlg)
	ON_BN_CLICKED(IDC_SETDEFAULT_BUTTON, OnSetdefaultButton)
	ON_EN_CHANGE(IDC_INSTANCE_EDIT, OnChangeInstanceEdit)
	ON_EN_CHANGE(IDC_PASSWORD_EDIT, OnChangePasswordEdit)
	ON_EN_CHANGE(IDC_SERVER_EDIT, OnChangeServerEdit)
	ON_EN_CHANGE(IDC_USER_EDIT, OnChangeUserEdit)
	ON_CBN_EDITCHANGE(IDC_VERSION_COMBO, OnEditchangeVersionCombo)
	ON_CBN_SELCHANGE(IDC_VERSION_COMBO, OnSelchangeVersionCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSindyConnectDlg メッセージ ハンドラ

BOOL CSindyConnectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_wndVersionCombo.SubclassDlgItem(IDC_VERSION_COMBO, this);

	SetDlgItemText(IDC_SERVER_EDIT, m_rProperty.e_strServer);
	SetDlgItemText(IDC_INSTANCE_EDIT, m_rProperty.e_strInstance);
	SetDlgItemText(IDC_USER_EDIT, m_rProperty.e_strUser);
	SetDlgItemText(IDC_PASSWORD_EDIT, m_rProperty.e_strPassword);
	SetDlgItemText(IDC_VERSION_COMBO, m_rProperty.e_strVersion);

	if(m_bIniProperty) {
		GetDlgItem(IDC_SETDEFAULT_BUTTON)->EnableWindow(! static_cast<CSindyIniConnectProperty&>(m_rProperty).isDefault());
	}
	else {
		GetDlgItem(IDC_SETDEFAULT_BUTTON)->ShowWindow(SW_HIDE);
	}

	m_wndVersionCombo.setConnectProperty(&m_rProperty);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CSindyConnectDlg::OnOK() 
{
	updateMembers();
	CDialog::OnOK();
}

void CSindyConnectDlg::OnSetdefaultButton() 
{
	if(m_bIniProperty) {
		static_cast<CSindyIniConnectProperty&>(m_rProperty).writeToPrivateIni();
	}
	GetDlgItem(IDC_SETDEFAULT_BUTTON)->EnableWindow(FALSE);
}

// 接続設定を更新したら、「規定値に設定」ボタンも更新

void CSindyConnectDlg::OnChangeServerEdit() 
{
	GetDlgItemText(IDC_SERVER_EDIT, m_rProperty.e_strServer);
	if(m_bIniProperty) {
		GetDlgItem(IDC_SETDEFAULT_BUTTON)->EnableWindow(! static_cast<CSindyIniConnectProperty&>(m_rProperty).isDefault());
	}
}

void CSindyConnectDlg::OnChangeInstanceEdit() 
{
	GetDlgItemText(IDC_INSTANCE_EDIT, m_rProperty.e_strInstance);
	if(m_bIniProperty) {
		GetDlgItem(IDC_SETDEFAULT_BUTTON)->EnableWindow(! static_cast<CSindyIniConnectProperty&>(m_rProperty).isDefault());
	}
}

void CSindyConnectDlg::OnChangeUserEdit() 
{
	GetDlgItemText(IDC_USER_EDIT, m_rProperty.e_strUser);
	if(m_bIniProperty) {
		GetDlgItem(IDC_SETDEFAULT_BUTTON)->EnableWindow(! static_cast<CSindyIniConnectProperty&>(m_rProperty).isDefault());
	}
}

void CSindyConnectDlg::OnChangePasswordEdit() 
{
	GetDlgItemText(IDC_PASSWORD_EDIT, m_rProperty.e_strPassword);
	if(m_bIniProperty) {
		GetDlgItem(IDC_SETDEFAULT_BUTTON)->EnableWindow(! static_cast<CSindyIniConnectProperty&>(m_rProperty).isDefault());
	}
}

void CSindyConnectDlg::OnEditchangeVersionCombo() 
{
	GetDlgItemText(IDC_VERSION_COMBO, m_rProperty.e_strVersion);
	if(m_bIniProperty) {
		GetDlgItem(IDC_SETDEFAULT_BUTTON)->EnableWindow(! static_cast<CSindyIniConnectProperty&>(m_rProperty).isDefault());
	}
}

void CSindyConnectDlg::OnSelchangeVersionCombo() 
{
	CComboBox* pCombo = static_cast<CComboBox*>(GetDlgItem(IDC_VERSION_COMBO));
	pCombo->GetLBText(pCombo->GetCurSel(), m_rProperty.e_strVersion);
	if(m_bIniProperty) {
		GetDlgItem(IDC_SETDEFAULT_BUTTON)->EnableWindow(! static_cast<CSindyIniConnectProperty&>(m_rProperty).isDefault());
	}
}

/////////////////////////////////////////////////////////////////////////////
// 処理
/////////////////////////////////////////////////////////////////////////////

void CSindyConnectDlg::updateMembers()
{
	GetDlgItemText(IDC_SERVER_EDIT, m_rProperty.e_strServer);
	GetDlgItemText(IDC_INSTANCE_EDIT, m_rProperty.e_strInstance);
	GetDlgItemText(IDC_USER_EDIT, m_rProperty.e_strUser);
	GetDlgItemText(IDC_PASSWORD_EDIT, m_rProperty.e_strPassword);
	GetDlgItemText(IDC_VERSION_COMBO, m_rProperty.e_strVersion);
}

void CSindyConnectDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
