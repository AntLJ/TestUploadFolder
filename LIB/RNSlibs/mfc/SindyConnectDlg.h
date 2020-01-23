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

// $Id: SindyConnectDlg.h,v 1.2 2004/01/14 06:00:14 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYCONNECTDLG_H__52046959_F13D_492D_BABD_C717A6C9DBFA__INCLUDED_)
#define AFX_SINDYCONNECTDLG_H__52046959_F13D_492D_BABD_C717A6C9DBFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SindyConnectDlg.h : �w�b�_�[ �t�@�C��
//

#include "resource.h"
#include "SindyConnectProperty.h"
#include "SindyVersionComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSindyConnectDlg �_�C�A���O

/**
 * @brief SiNDY�ڑ��ݒ�_�C�A���O
 *
 * @note pProperty�̒l�́A�L�����Z���������ꂽ�ꍇ�ł�������������̂Œ��ӁB
 */
class CSindyConnectDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CSindyConnectDlg(CSindyIniConnectProperty& rProperty, CWnd* pParent = NULL);
	CSindyConnectDlg(CSindyConnectProperty& rProperty, CWnd* pParent = NULL);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSindyConnectDlg)
	enum { IDD = IDD_SINDYCONNECT };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSindyConnectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSindyConnectDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetdefaultButton();
	afx_msg void OnChangeInstanceEdit();
	afx_msg void OnChangePasswordEdit();
	afx_msg void OnChangeServerEdit();
	afx_msg void OnChangeUserEdit();
	afx_msg void OnEditchangeVersionCombo();
	afx_msg void OnSelchangeVersionCombo();
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
// ����
	void updateMembers();

private:
// �v���p�e�B
	CSindyConnectProperty& m_rProperty;

	BOOL m_bIniProperty;

	CSindyVersionComboBox m_wndVersionCombo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SINDYCONNECTDLG_H__52046959_F13D_492D_BABD_C717A6C9DBFA__INCLUDED_)
