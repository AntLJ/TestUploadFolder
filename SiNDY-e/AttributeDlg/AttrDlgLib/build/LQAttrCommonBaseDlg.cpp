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
* WM_COMMAND �p�C�x���g�n���h��
*
* �e�R�����R���g���[���ŕύX���������ꍇ�A�����ŏ������܂�
* ���݂̓G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�A�R���{�{�b�N�X�̏������s���܂�
*/
LRESULT CLQAttrCommonBaseDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
	if( !m_bFinishInit ) return 0;

	INT msg = HIWORD(wParam);						// ���상�b�Z�[�W
	INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��
	// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
	if( msg == EN_SETFOCUS )
		SelectDlgItem( nTargetControl );
	// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
	if( msg == CBN_DROPDOWN )
		SetComboboxList( nTargetControl );

	// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
	if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
	{
		// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
		m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
		InvalidateRect( NULL, FALSE );
		// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
		SetButton( Changed() );
	}

	bHandled = FALSE;
	return 0;
}
