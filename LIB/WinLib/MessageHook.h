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

#pragma once

#include "WindowHook.h"

class CMessageHook : public CWindowHook
{
public:
	CMessageHook();
	~CMessageHook();

	struct Observer
	{
		// �������z�֐�
		virtual void OnHook( int id, ULONG hookType, int nCode, WPARAM wParam, LPARAM lParam, bool& bAccept ) = 0;
	};

	// �t�b�N�J�n
	void Initialize( int id, HWND hWnd, ULONG hookType, Observer* pObserver, DWORD tid = 0 );
	// �t�b�N�I��
	void Shutdown(int id);

private:
	LRESULT OnHook( int id, ULONG HookType, int nCode, WPARAM wParam, LPARAM lParam, bool bAccept );

private:
	Observer* m_pObserver;	//!< �Ăяo����
};
