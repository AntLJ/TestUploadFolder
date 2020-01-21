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

#ifndef __SINDY_CONNECTION_DLG_I_H__
#define __SINDY_CONNECTION_DLG_I_H__

#include "exportimport.h"
#include "Commdlg.h"

namespace sindy {

class SINDYLIB_API ConnectionDlgI
{
public:
	class HookProcI
	{
	public:
		virtual void operator()(LPOFNOTIFY lpof) = 0;
	};

// ç\íz/è¡ñ≈
	ConnectionDlgI();
	virtual ~ConnectionDlgI();

// èàóù
	virtual BOOL DoModal(HWND hParentWnd) = 0;

// ïœêî
	OPENFILENAME m_OFN;

private:
	static UINT CALLBACK OFNHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);

// ïœêî
	TCHAR m_szFile[MAX_PATH];
	TCHAR m_szFileTitle[MAX_PATH];
};

} // namespace sindy

#endif // __SINDY_CONNECTION_DLG_I_H__
