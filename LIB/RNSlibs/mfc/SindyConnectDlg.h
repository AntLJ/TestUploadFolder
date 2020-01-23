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
// SindyConnectDlg.h : ヘッダー ファイル
//

#include "resource.h"
#include "SindyConnectProperty.h"
#include "SindyVersionComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSindyConnectDlg ダイアログ

/**
 * @brief SiNDY接続設定ダイアログ
 *
 * @note pPropertyの値は、キャンセルが押された場合でも書き換えられるので注意。
 */
class CSindyConnectDlg : public CDialog
{
// コンストラクション
public:
	CSindyConnectDlg(CSindyIniConnectProperty& rProperty, CWnd* pParent = NULL);
	CSindyConnectDlg(CSindyConnectProperty& rProperty, CWnd* pParent = NULL);

// ダイアログ データ
	//{{AFX_DATA(CSindyConnectDlg)
	enum { IDD = IDD_SINDYCONNECT };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSindyConnectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
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
// 処理
	void updateMembers();

private:
// プロパティ
	CSindyConnectProperty& m_rProperty;

	BOOL m_bIniProperty;

	CSindyVersionComboBox m_wndVersionCombo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SINDYCONNECTDLG_H__52046959_F13D_492D_BABD_C717A6C9DBFA__INCLUDED_)
