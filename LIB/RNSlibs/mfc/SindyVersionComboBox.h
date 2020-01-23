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

// $Id: SindyVersionComboBox.h,v 1.1.1.1 2003/09/12 07:26:04 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYVERSIONCOMBOBOX_H__41C981C4_4E56_4FEE_8D24_51DD97DC3E73__INCLUDED_)
#define AFX_SINDYVERSIONCOMBOBOX_H__41C981C4_4E56_4FEE_8D24_51DD97DC3E73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SindyVersionComboBox.h : �w�b�_�[ �t�@�C��
//

#include "SindyConnectProperty.h"

/////////////////////////////////////////////////////////////////////////////
// CSindyVersionComboBox �E�B���h�E

class CSindyVersionComboBox : public CComboBox
{
// �R���X�g���N�V����
public:
	CSindyVersionComboBox();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSindyVersionComboBox)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CSindyVersionComboBox();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CSindyVersionComboBox)
	afx_msg void OnDropdown();
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
// �ݒ�
	void setConnectProperty(const CSindyConnectProperty* pConnectProperty) { m_pConnectProperty = pConnectProperty; }
	void reloadList();

private:
// �v���p�e�B
	const CSindyConnectProperty* m_pConnectProperty; ///< �Q�Ɛڑ��v���p�e�B
	CSindyConnectProperty m_cConnectingProperty; ///< �񋓍ςݐڑ��v���p�e�B
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SINDYVERSIONCOMBOBOX_H__41C981C4_4E56_4FEE_8D24_51DD97DC3E73__INCLUDED_)
