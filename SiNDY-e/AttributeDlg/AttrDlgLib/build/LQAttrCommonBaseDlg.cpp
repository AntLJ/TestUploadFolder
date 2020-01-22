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

#include "stdafx.h"
#include "../LQAttrCommonBaseDlg.h"

LRESULT CLQAttrCommonBaseDlg::OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hdc = (HDC) wParam;    // handle to display context 
	HWND hWnd = (HWND) lParam; // handle to static control 

	CControlDef* def = m_cControlRel.GetControlDef( hWnd );

	if( def && def->Errored() )
	{
		if ( !m_hErrorBrush )
			m_hErrorBrush = CreateSolidBrush( RGB(255,0,0) );

		// Get the DC provided and set the appropriate attributes

		SetBkMode(hdc, TRANSPARENT );
		SetBkColor(hdc, RGB(255,0,0));

		// Return a handle to the brush
		HBRUSH far* hbr = (HBRUSH far*) m_hErrorBrush;

		return ((DWORD) hbr);
	}
	else if( def && def->Changed() )
	{

		if( !m_hBrush )
			m_hBrush = CreateSolidBrush( RGB(255,168,126) );

		// Get the DC provided and set the appropriate attributes

		SetBkMode(hdc, TRANSPARENT );
		SetBkColor(hdc, RGB(255,168,126));

		// Return a handle to the brush
		HBRUSH far* hbr = (HBRUSH far*) m_hBrush;

		return ((DWORD) hbr);
	} 

	bHandled = FALSE;

	return 0;
}
/**
* WM_COMMAND 用イベントハンドラ
*
* 各コモンコントロールで変更があった場合、ここで処理します
* 現在はエディットボックス、チェックボックス、コンボボックスの処理が行われます
*/
LRESULT CLQAttrCommonBaseDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
	if( !m_bFinishInit ) return 0;

	INT msg = HIWORD(wParam);						// 操作メッセージ
	INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール
	// エディットボックスの時に全選択にする
	if( msg == EN_SETFOCUS )
		SelectDlgItem( nTargetControl );
	// コンボボックスのリストボックスの長さを調節
	if( msg == CBN_DROPDOWN )
		SetComboboxList( nTargetControl );

	// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
	if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
	{
		// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
		m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
		InvalidateRect( NULL, FALSE );
		// 変更された場合はボタンを Enable に
		SetButton( Changed() );
	}

	bHandled = FALSE;
	return 0;
}
