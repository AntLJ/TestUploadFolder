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

/**
 * @file ActiveView.cpp
 * @brief CActiveView �N���X�����t�@�C��
 * @author �n�}DB���암�V�X�e���J���O���[�v �Ð� �M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "ActiveView.h"
#include "AheZooMoveFunctions.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IEnvelopePtr CActiveView::GetExtent() const
{
	IEnvelopePtr ipEnvelope;
	if( FAILED( p->get_Extent( &ipEnvelope ) ) )
		_ASSERTE(false);

	return ipEnvelope;
}

IScreenDisplayPtr CActiveView::GetScreenDisplay() const
{
	IScreenDisplayPtr ipScreenDisplay;
	if( FAILED( p->get_ScreenDisplay( &ipScreenDisplay ) ) )
		_ASSERTE(false);

	return ipScreenDisplay;
}

IMapPtr CActiveView::GetFocusMap() const
{
	IMapPtr ipMap;
	if( FAILED( p->get_FocusMap( &ipMap ) ) )
		_ASSERTE(false);

	return ipMap;
}

// �n���ꂽ�|�C���g�����S�ɂ���悤�ɃX�N���[������
HRESULT CActiveView::MoveCenterTo( IPoint* ipPoint, double dScale/*  = -1.0 */, bool async/* = false*/ ) const
{
	HRESULT hr = S_OK; // �߂�l

	IEnvelopePtr ipEnv( GetExtent() );
	
	if( ! ipEnv )
		return E_FAIL;

	if( SUCCEEDED( hr = ipEnv->CenterAt( ipPoint ) ) )
	{
		if( SUCCEEDED( hr = p->put_Extent( ipEnv ) ) )
		{
			if( dScale > 0.0 )
				GetFocusMap()->put_MapScale( dScale );

			// �ĕ`��
			p->Refresh();
			if( ! async )
				GetScreenDisplay()->UpdateWindow();
		}
	}
	return hr;
}

// �}�E�X�|�C���^�̈ʒu�����S�ɂ���悤�ɃX�N���[������
bool CActiveView::MoveCenterToMousePointer(bool bSuppressEvents) const
{
	return AheMoveCenterToMousePointer( p, bSuppressEvents );
}

// ZooMoveTool�̃L�[�ړ��g��k������
bool CActiveView::ZooMoveByKey( long lKeyCode, long lShift ) const
{
	return AheKeyZooMove( p, lKeyCode, lShift );
}

bool CActiveView::MoveCursorToCenter() const
{
	return AheMoveCursorToCenter(p);
}

bool CActiveView::Shift( int nXStep, int nYStep ) const
{
	return AheScrollMap( p, nXStep, nYStep );
}

bool CActiveView::Zoom( bool bUpScale ) const
{
	return AheZoom( p, bUpScale );
}
