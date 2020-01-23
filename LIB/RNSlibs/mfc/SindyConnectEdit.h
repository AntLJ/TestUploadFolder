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

// $Id: SindyConnectEdit.h,v 1.1.1.1 2003/09/12 07:26:04 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYCONNECTEDIT_H__5DE197E3_2501_46F9_B36A_21FEB5972B61__INCLUDED_)
#define AFX_SINDYCONNECTEDIT_H__5DE197E3_2501_46F9_B36A_21FEB5972B61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SindyConnectEdit.h : ヘッダー ファイル
//

#include "SindyConnectProperty.h"

/////////////////////////////////////////////////////////////////////////////
// CSindyConnectEdit ウィンドウ

class CSindyConnectEdit : public CEdit
{
// コンストラクション
public:
	CSindyConnectEdit();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSindyConnectEdit)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CSindyConnectEdit();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CSindyConnectEdit)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
// 設定処理
	void setProperties(const CSindyConnectProperty& rProperty);

// 取得処理
	void getPropertiesTo(CSindyConnectProperty& rProperty);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SINDYCONNECTEDIT_H__5DE197E3_2501_46F9_B36A_21FEB5972B61__INCLUDED_)
