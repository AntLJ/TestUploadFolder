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

// SindyVersionComboBox.cpp : �C���v�������e�[�V���� �t�@�C��
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
// CSindyVersionComboBox ���b�Z�[�W �n���h��

BOOL CSindyVersionComboBox::PreTranslateMessage(MSG* pMsg)
{
	// �G�X�P�[�v�L�[�ɂ��L�����Z����s�ɂ���
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
// �ݒ�
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief �h���b�v�_�E�����X�g�̍��ڂ��Đݒ肷��
 */
void CSindyVersionComboBox::reloadList()
{
	ATLASSERT(m_pConnectProperty);

	// �ڑ��ݒ�ɕύX���������烊�X�g���X�V
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

		// �\��������
		HRESULT hr = ::CoInitialize(0);

		// ������
		CString strText;
		GetWindowText(strText);
		ResetContent();
		m_cConnectingProperty.clear();
		SetWindowText(strText);

		// �ڑ�����
		CWaitCursor wait;

		EnableWindow(FALSE);
		IVersionedWorkspacePtr ipVerWorkspace = ScmWorkspace(aProperty.getPropertySet());
		EnableWindow(TRUE);

		if(ipVerWorkspace == 0) {
			MessageBox(_T("�f�[�^�x�[�X�ւ̐ڑ��Ɏ��s���܂����B"), _T("�ڑ����s"), MB_OK | MB_ICONWARNING);
			return;
		}

		// �o�[�W������
		IEnumVersionInfoPtr ipEnumVerInfo;
		ipVerWorkspace->get_Versions(&ipEnumVerInfo);

		IVersionInfoPtr ipVerInfo;
		while(SUCCEEDED(ipEnumVerInfo->Next(&ipVerInfo)) && ipVerInfo != 0) {
			CComBSTR bstrName;
			ipVerInfo->get_VersionName(&bstrName);
			std::string aName = RNSSindyPlant::wcsToMbs(bstrName);
			AddString(aName.c_str());
		}
		
		// �񋓍ςݐڑ��ݒ���X�V
		m_cConnectingProperty = *m_pConnectProperty;

		// ���
		ipEnumVerInfo = 0;
		ipVerInfo = 0;
		ipVerWorkspace = 0;

		// �\�����
		if(SUCCEEDED(hr)) {
			::CoUninitialize();
		}
	}
}
