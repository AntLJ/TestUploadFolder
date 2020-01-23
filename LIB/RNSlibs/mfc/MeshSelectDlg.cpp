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

// MeshSelectDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MeshSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshSelectDlg ダイアログ


CMeshSelectDlg::CMeshSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeshSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeshSelectDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CMeshSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeshSelectDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
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
// CMeshSelectDlg メッセージ ハンドラ

BOOL CMeshSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndMeshDraw.SubclassDlgItem(IDC_MESHDRAW_STATIC, this);
	
	redrawMeshCount();
	redrawSelectedMeshCount();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CMeshSelectDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

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
		"ﾃｷｽﾄﾌｧｲﾙ (*.txt)|*.txt|すべてのﾌｧｲﾙ (*.*)|*.*||",
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
		"ﾃｷｽﾄﾌｧｲﾙ (*.txt)|*.txt|すべてのﾌｧｲﾙ (*.*)|*.*||",
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
// 描画
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
