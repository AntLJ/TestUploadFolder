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

#if !defined(AFX_MESHSELECTDLG_H__A3992011_4172_4BC9_89E1_03FCE0338AF3__INCLUDED_)
#define AFX_MESHSELECTDLG_H__A3992011_4172_4BC9_89E1_03FCE0338AF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshSelectDlg.h : �w�b�_�[ �t�@�C��
//

#include "resource.h"
#include "MeshDraw.h"

/////////////////////////////////////////////////////////////////////////////
// CMeshSelectDlg �_�C�A���O

class CMeshSelectDlg : public CDialog
{
// �R���X�g���N�V����
public:
	typedef CMeshDraw::MeshCodeSet MeshCodeSet;

	CMeshSelectDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CMeshSelectDlg)
	enum { IDD = IDD_MESHSELECT };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMeshSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMeshSelectDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnClearmeshButton();
	afx_msg void OnAddmeshlistfileButton();
	afx_msg void OnMeshlistoutputButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
// �擾
	CMeshDraw& GetMeshDraw() { return m_wndMeshDraw; }

// �`��
	void redrawMeshCount();
	void redrawSelectedMeshCount();

private:
// �T�u�N���X
	CMeshDraw m_wndMeshDraw;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MESHSELECTDLG_H__A3992011_4172_4BC9_89E1_03FCE0338AF3__INCLUDED_)
