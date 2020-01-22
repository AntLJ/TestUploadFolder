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

// LQAttrCommonBaseDlg.h : CLQAttrCommonBaseDlg の宣言

#ifndef __LQATTRCOMMONBASEDLG_H_
#define __LQATTRCOMMONBASEDLG_H_

#include <atlhost.h>

#include "LQAttrBaseDlg.h"

typedef CLQAttrBaseDlg LQATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CLQAttrCommonBaseDlg
class CLQAttrCommonBaseDlg : public LQATTR_BASE_CLASS
{
public:
	CLQAttrCommonBaseDlg( WINRECT* pWinMap, WORD resourceID ) : LQATTR_BASE_CLASS( pWinMap, resourceID )
	{
		m_hErrorBrush = m_hBrush = NULL;	// エディットボックスなどの色変更のためのブラシ
	}

	~CLQAttrCommonBaseDlg()
	{
	}

BEGIN_MSG_MAP(CLQAttrCommonBaseDlg)
	CHAIN_MSG_MAP(LQATTR_BASE_CLASS)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
END_MSG_MAP()

	LRESULT OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	HBRUSH					m_hBrush, m_hErrorBrush;	//!< コモンコントロールの色変更用ブラシ
};

#endif //__LQATTRCOMMONBASEDLG_H_
