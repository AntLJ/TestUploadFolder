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

// DialogMessageHook.h: interface for the CDialogMessageHook class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGMESSAGEHOOK_H__53812B4C_FBAD_4FD3_8238_85CD48CFE453__INCLUDED_)
#define AFX_DIALOGMESSAGEHOOK_H__53812B4C_FBAD_4FD3_8238_85CD48CFE453__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>

typedef std::set<HWND> THWNDCollection;

// CDialogMessageHook makes it easy to properly
// process tab and accelerator keys in
// ATL modeless dialogs
class CDialogMessageHook  
{
public:
    // set a dialog message hook for the specified modeless dialog
    static HRESULT InstallHook(HWND hWnd);
    static HRESULT UninstallHook(HWND hWnd);

private:
    // the hook function
    static LRESULT CALLBACK GetMessageProc(int nCode, 
                            WPARAM wParam, LPARAM lParam);

    // the hook handle
    static HHOOK m_hHook;

    // the set of HWNDs we are hooking
    static THWNDCollection m_aWindows;
};

#endif
// !defined(AFX_DIALOGMESSAGEHOOK_H__53812B4C_FBAD_4FD3_8238_85CD48CFE453__INCLUDED_)
