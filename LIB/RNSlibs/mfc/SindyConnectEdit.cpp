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

// SindyConnectEdit.cpp : インプリメンテーション ファイル
//
// $Id: SindyConnectEdit.cpp,v 1.1.1.1 2003/09/12 07:26:04 f_kiku Exp $

#include "stdafx.h"
#define _SINDY_NO_CSTRING
#include <atlbase.h>
#include "SindyConnectEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSindyConnectEdit

CSindyConnectEdit::CSindyConnectEdit()
{
}

CSindyConnectEdit::~CSindyConnectEdit()
{
}


BEGIN_MESSAGE_MAP(CSindyConnectEdit, CEdit)
	//{{AFX_MSG_MAP(CSindyConnectEdit)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSindyConnectEdit メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
// 設定
/////////////////////////////////////////////////////////////////////////////

void CSindyConnectEdit::setProperties(const CSindyConnectProperty& rProperty)
{
	SetWindowText(rProperty.getPropertiesString());
}

void CSindyConnectEdit::getPropertiesTo(CSindyConnectProperty& rProperty)
{
	CString strProperties;
	GetWindowText(strProperties);

	rProperty.setPropertiesString(strProperties);
}
