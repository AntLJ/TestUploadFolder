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

// MeshSelectDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "MeshSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshSelectDlg �_�C�A���O


CMeshSelectDlg::CMeshSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeshSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeshSelectDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CMeshSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeshSelectDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeshSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CMeshSelectDlg)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_CLEARMESH_BUTTON, OnClearmeshButton)
	ON_BN_CLICKED(IDC_ADDMESHLISTFILE_BUTTON, OnAddmeshlistfileButton)
	ON_BN_CLICKED(IDC_MESHLISTOUTPUT_BUTTON, OnMeshlistoutputButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshSelectDlg ���b�Z�[�W �n���h��

BOOL CMeshSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndMeshDraw.SubclassDlgItem(IDC_MESHDRAW_STATIC, this);
	
	redrawMeshCount();
	redrawSelectedMeshCount();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

BOOL CMeshSelectDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	if(zDelta > 0) {
		m_wndMeshDraw.zoomIn();
	}
	else {
		m_wndMeshDraw.zoomOut();
	}

	return TRUE;
}

void CMeshSelectDlg::OnClearmeshButton() 
{
	GetMeshDraw().clearSelect();
	redrawSelectedMeshCount();
	GetMeshDraw().Invalidate();
}

void CMeshSelectDlg::OnAddmeshlistfileButton() 
{
	CFileDialog dlg(
		TRUE,
		_T("txt"),
		NULL,
		0,
		"÷��̧�� (*.txt)|*.txt|���ׂĂ�̧�� (*.*)|*.*||",
		NULL
	);

	if(dlg.DoModal() != IDOK)
		return;

	CMeshDraw& rMeshDraw = GetMeshDraw();

#if _MSC_VER > 1200
	std::ifstream aStream(T2W(dlg.GetPathName()));
#else
	std::ifstream aStream(dlg.GetPathName());
#endif
	rMeshDraw.appendSelectBy(aStream);

	redrawSelectedMeshCount();
	rMeshDraw.Invalidate();
}

void CMeshSelectDlg::OnMeshlistoutputButton() 
{
	CFileDialog dlg(
		FALSE,
		_T("txt"),
		_T("mesh_list.txt"),
		OFN_OVERWRITEPROMPT,
		"÷��̧�� (*.txt)|*.txt|���ׂĂ�̧�� (*.*)|*.*||",
		NULL
	);


	if(dlg.DoModal() != IDOK)
		return;

	CMeshDraw& rMeshDraw = GetMeshDraw();

	std::ofstream aStream(dlg.GetPathName());

	CWaitCursor wait;
	const CMeshDraw::MeshCodeSet& rMeshCodeSet = rMeshDraw.selectedMeshCodeSet();
	for(CMeshDraw::MeshCodeSet::iterator itr = rMeshCodeSet.begin(); itr != rMeshCodeSet.end(); ++itr) {
		aStream << *itr << std::endl;
	}
}

/////////////////////////////////////////////////////////////////////////////
// �`��
/////////////////////////////////////////////////////////////////////////////

void CMeshSelectDlg::redrawMeshCount()
{
	CWnd *pWnd = GetDlgItem(IDC_MESHCOUNT_EDIT);

	CString strText;
	strText.Format(_T("%d"), GetMeshDraw().meshCodeSet().size());
	pWnd->SetWindowText(strText);
}

void CMeshSelectDlg::redrawSelectedMeshCount()
{
	CWnd *pWnd = GetDlgItem(IDC_SELECTEDMESHCOUNT_EDIT);

	CString strText;
	strText.Format(_T("%d"), GetMeshDraw().selectedMeshCodeSet().size());
	pWnd->SetWindowText(strText);
}
