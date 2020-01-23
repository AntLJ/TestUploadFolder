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
// MeshSelectDlg.h : ヘッダー ファイル
//

#include "resource.h"
#include "MeshDraw.h"

/////////////////////////////////////////////////////////////////////////////
// CMeshSelectDlg ダイアログ

class CMeshSelectDlg : public CDialog
{
// コンストラクション
public:
	typedef CMeshDraw::MeshCodeSet MeshCodeSet;

	CMeshSelectDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMeshSelectDlg)
	enum { IDD = IDD_MESHSELECT };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMeshSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMeshSelectDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnClearmeshButton();
	afx_msg void OnAddmeshlistfileButton();
	afx_msg void OnMeshlistoutputButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
// 取得
	CMeshDraw& GetMeshDraw() { return m_wndMeshDraw; }

// 描画
	void redrawMeshCount();
	void redrawSelectedMeshCount();

private:
// サブクラス
	CMeshDraw m_wndMeshDraw;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MESHSELECTDLG_H__A3992011_4172_4BC9_89E1_03FCE0338AF3__INCLUDED_)
