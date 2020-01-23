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
// SindyVersionComboBox.h : ヘッダー ファイル
//

#include "SindyConnectProperty.h"

/////////////////////////////////////////////////////////////////////////////
// CSindyVersionComboBox ウィンドウ

class CSindyVersionComboBox : public CComboBox
{
// コンストラクション
public:
	CSindyVersionComboBox();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSindyVersionComboBox)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CSindyVersionComboBox();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CSindyVersionComboBox)
	afx_msg void OnDropdown();
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
// 設定
	void setConnectProperty(const CSindyConnectProperty* pConnectProperty) { m_pConnectProperty = pConnectProperty; }
	void reloadList();

private:
// プロパティ
	const CSindyConnectProperty* m_pConnectProperty; ///< 参照接続プロパティ
	CSindyConnectProperty m_cConnectingProperty; ///< 列挙済み接続プロパティ
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SINDYVERSIONCOMBOBOX_H__41C981C4_4E56_4FEE_8D24_51DD97DC3E73__INCLUDED_)
