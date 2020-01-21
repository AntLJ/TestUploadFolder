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
 * @file AheZooMoveFunctions.cpp
 * @brief ArcObjects�O���[�o���w���p�[�֐�����
 *
 * ���̃t�@�C���ɂ͊g��k���A�X�N���[���ȂǂɊւ���
 * �O���[�o���֐�����������Ă��܂��B
 * 
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @date 2007/01/26
 * $Id$
 */
#include "stdafx.h"
#include "AheZooMoveFunctions.h"
#include <math.h>
#include "WinLib/WaitCursor.h"


// ZooMoveTool�̃L�[�ړ��g��k������
bool AheKeyZooMove( IActiveView* ipActiveView, long lKeyCode, long lShift )
{
	CWaitCursor cursor;
	bool bRet = false; // �Ԃ�l

	switch( lKeyCode )
	{
	// �X�y�[�X�L�[�ŁA�J�[�\�������}�b�v�̃Z���^�[�ֈړ�����
	case VK_SPACE:
		bRet = AheMoveCursorToCenter( ipActiveView );
		break;

	//�@NumPAD���Œn�}�̈ړ��������B+CTRL�ň��ʈړ��A�ʏ�͔���ʈړ�
	case 'Z':
		if( 0 != lShift )
			break;
	case VK_NUMPAD1:
		bRet = AheScrollMap( ipActiveView, ( 2 == lShift ) ? -1 : -2, ( 2 == lShift ) ? -1 : -2 );
		break;
	case 'S':
		if( 0 != lShift )
			break;
	case VK_NUMPAD2:
		bRet = AheScrollMap( ipActiveView, 0, ( 2 == lShift ) ? -1 : -2 );
		break;
	case 'C':
		if( 0 != lShift )
			break;
	case VK_NUMPAD3:
		bRet = AheScrollMap( ipActiveView, ( 2 == lShift ) ? 1 : 2, ( 2 == lShift ) ? -1 : -2 );
		break;
	case 'A':
		if( 0 != lShift )
			break;
	case VK_NUMPAD4:
		bRet = AheScrollMap( ipActiveView, ( 2 == lShift ) ? -1 : -2, 0 );
		break;
	case 'D':
		if( 0 != lShift )
			break;
	case VK_NUMPAD6:
		bRet = AheScrollMap( ipActiveView, ( 2 == lShift ) ? 1 : 2, 0 );
		break;
	case 'Q':
		if( 0 != lShift )
			break;
	case VK_NUMPAD7:
		bRet = AheScrollMap( ipActiveView, ( 2 == lShift ) ? -1 : -2, ( 2 == lShift ) ? 1 : 2 );
		break;
	case 'W':
		if( 0 != lShift )
			break;
	case VK_NUMPAD8:
		bRet = AheScrollMap( ipActiveView, 0, ( 2 == lShift ) ? 1 : 2 );
		break;
	case 'E':
		if( 0 != lShift )
			break;
	case VK_NUMPAD9:
		bRet = AheScrollMap( ipActiveView, ( 2 == lShift ) ? 1 : 2, ( 2 == lShift ) ? 1 : 2 );
		break;
	//�@NumPAD��5�͌���J�[�\���ʒu����ʂ̃Z���^�[�ֈړ�����
	case 'X':
		if( 0 != lShift )
			break;
	case VK_NUMPAD5:
		{
			AheMoveCenterToMousePointer( ipActiveView, false );
			AheMoveCursorToCenter( ipActiveView );

			AheRefresh(ipActiveView);
			bRet = true;
		}
		break;

	//�@NumPAD��+-��ZoomUP/DOWN
	case '2':
		if( 0 != lShift )
			break;
	case VK_ADD:
		{
			AheMoveCenterToMousePointer( ipActiveView );
			AheMoveCursorToCenter( ipActiveView );
			AheZoomIn( ipActiveView );
			AheRefresh(ipActiveView);
			bRet = true;
		}
		break;
	case '3':
		if( 0 != lShift )
			break;
	case VK_SUBTRACT:
		bRet = AheZoomOut( ipActiveView );
		AheRefresh(ipActiveView);
		break;
	//�@NumPAD��/*�ōő�ŏ���Zoom
	case VK_MULTIPLY:
		{
			IMapPtr ipMap;

			ipActiveView->get_FocusMap( &ipMap );
			ipMap->put_MapScale( static_cast<double>( 1562 ));
			AheRefresh(ipActiveView);
			bRet = true;
		}
		break;
	case VK_DIVIDE:
		{
			IMapPtr ipMap;

			ipActiveView->get_FocusMap( &ipMap );
			ipMap->put_MapScale( static_cast<double>( 3200000 ));
			AheRefresh(ipActiveView);
			bRet = true;
		}
		break;
	// 1�L�[�ōĕ`��
	case '1':
		if( 0 != lShift )
			break;
		AheRefresh(ipActiveView);
		bRet = true;
		break;
	default:
		break;
	}

	return bRet;
}
// �}�E�X�|�C���^�̈ʒu�����S�ɂ���悤�ɃX�N���[������
bool AheMoveCenterToMousePointer( IActiveView* ipActiveView, bool bSuppressEvents )
{
	if( !ipActiveView )
		return false;

	// �}�E�X�|�C���^�̃f�o�C�X���W�擾
	IScreenDisplayPtr ipDisp;
	ipActiveView->get_ScreenDisplay( &ipDisp );
	if( !ipDisp )
		return false;

	POINT stPoint = { 0, 0 };
	HWND hWnd = 0;
	::GetCursorPos( &stPoint );
	ipDisp->get_hWnd( reinterpret_cast<OLE_HANDLE*>( &hWnd ) );
	::ScreenToClient( hWnd, &stPoint );

	// �n�}���W�ɕϊ�
	IPointPtr ipPoint;
	IDisplayTransformationPtr ipDispTrans;
	ipDisp->get_DisplayTransformation( &ipDispTrans );
	if( !ipDispTrans )
		return false;

	ipDispTrans->ToMapPoint( stPoint.x, stPoint.y, &ipPoint );

	// ���S�ֈړ�
	IEnvelopePtr ipEnv;
	ipActiveView->get_Extent( &ipEnv );
	ipEnv->CenterAt( ipPoint );

	// [Bug 3772]�g�傪�����Ȃ��Ȃ�o�O�ւ̑Ή�
	// �}�b�v�̃T�C�Y�͕ς��Ȃ��̂ɁA���W�ړ��O��Envelope�����W�ړ����
	// Envelope�ɐݒ肷��ƃX�P�[�����ς�邱�Ƃ�����Ƃ������
	// ��U�C�x���g���Ƃ߂āAVisibleBounds�ŗ̈��ݒ肷��
	// [Bug 5495]�������A�C�x���g���Ƃ߂Ă��܂��ƕ`��̕ύX�͂����Ă��A�����I�ɂ͑O�̍��W�ʒu�ł��邽��
	// MoveCenterToMousePointer�̌�ɃC�x���g�𔭍s���鏈�����Ȃ��ꍇ�ɂ́A�C�x���g�Ƃ߂Ȃ����Ƃɂ���
	// �ꉞ���̏����ł���΁A[Bug 3772]�Ɨ����͂ł���
	// TODO:���̏������ƁA���ɃC�x���g�ʒm����~����Ă����ꍇ�ɋ����I�ɍĊJ����邱�ƂɂȂ邽�߁A
	// ���ƂŁA�����ƃC�x���g��~����Ă��邩�ǂ����m�F����B
	IDisplayTransformationPtr ipTrans;
	ipDisp->get_DisplayTransformation( &ipTrans );
	if( !ipTrans )
		return false;
	if( bSuppressEvents )
		ipTrans->put_SuppressEvents( VARIANT_TRUE );
	ipTrans->put_VisibleBounds( ipEnv );
	if( bSuppressEvents )
		ipTrans->put_SuppressEvents( VARIANT_FALSE );
	return true;
}

// �J�[�\�����r���[�̃Z���^�[�Ɉړ�
bool AheMoveCursorToCenter( IActiveView* ipActiveView )
{
	if( !ipActiveView )
		return false;

	// �}�b�v�̍��W�T�C�Y�擾
	IScreenDisplayPtr ipDisp;
	ipActiveView->get_ScreenDisplay( &ipDisp );
	if( !ipDisp )
		return false;

	IDisplayTransformationPtr ipTrans;
	ipDisp->get_DisplayTransformation( &ipTrans );
	if( !ipTrans )
		return false;

	RECT rect;
	ipTrans->get_DeviceFrame( &rect );

	// �}�b�v�̒��S���擾
	POINT stPoint;
	stPoint.x = (rect.left + rect.right )/2;
	stPoint.y = (rect.bottom + rect.top)/2;

	// �N���C�A���g�̈�̍��W����X�N���[�����W�ɕϊ�
	HWND hWnd;
	ipDisp->get_hWnd( reinterpret_cast<OLE_HANDLE*>( &hWnd ) );
	::ClientToScreen( hWnd, &stPoint );

	// �}�E�X�J�[�\���ړ�
	::SetCursorPos( stPoint.x, stPoint.y );
	return true;
}

// �w��X�e�b�v�����̈ړ�
bool AheScrollMap( IActiveView* ipActiveView, int nXStep, int nYStep )
{
	IEnvelopePtr ipEnv;
	if( SUCCEEDED( ipActiveView->get_Extent( &ipEnv )))
	{
		double dbMin;
		double dbMax;
		double dbWork;

		if( 0 != nXStep )
		{
			ipEnv->get_Width( &dbWork );
			ipEnv->get_XMax( &dbMax );
			ipEnv->get_XMin( &dbMin );
			dbMax += ( dbWork / static_cast<double>( nXStep ));
			dbMin += ( dbWork / static_cast<double>( nXStep ));
			ipEnv->put_XMax( max( dbMax, dbMin ));
			ipEnv->put_XMin( min( dbMax, dbMin ));
		}

		if( 0 != nYStep )
		{
			ipEnv->get_Height( &dbWork );
			ipEnv->get_YMax( &dbMax );
			ipEnv->get_YMin( &dbMin );
			dbMax += ( dbWork / static_cast<double>( nYStep ));
			dbMin += ( dbWork / static_cast<double>( nYStep ));
			ipEnv->put_YMax( max( dbMax, dbMin ));
			ipEnv->put_YMin( min( dbMax, dbMin ));
		}

		if( SUCCEEDED( ipActiveView->put_Extent( ipEnv )))
		{
			AheRefresh(ipActiveView);

			return true;
		}
	}

	return false;
}

bool AheZoom( IActiveView* ipActiveView, bool bUpScale )
{
	double dCurrent = 0.0;

	IMapPtr ipMap;
	ipActiveView->get_FocusMap( &ipMap );
	if( SUCCEEDED( ipMap->get_MapScale( &dCurrent ) ) )
	{
		// [bug 5970] �g��k���c�[���Ŋg�債�����ɍ��W������邱�Ƃ�����
		// �g�傷��ꍇ�̓X�P�[��1/2�{�A�k������ꍇ�̓X�P�[��2�{(���X�P�[������)
		ipMap->put_MapScale( dCurrent * ( bUpScale ? 0.5 : 2.0 ) );

		return true;
	}

	return false;
}

// MapFAN��ZM�l�ɑ������g�嗦�ł̊g�又��
bool AheZoomIn( IActiveView* ipActiveView )
{
	return AheZoom( ipActiveView, true );
}

// MapFAN��ZM�l�ɑ������g�嗦�ł̏k������
bool AheZoomOut( IActiveView* ipActiveView )
{
	return AheZoom( ipActiveView, false );
}

// �ĕ`�惂�[�h���擾
bool AheIsRefreshMode()
{
	static BOOL bRefreshMode = -1;
	if( -1 == bRefreshMode )
	{
		CRegKey key;
		DWORD dwRefreshMode = 0;
		if( ERROR_SUCCESS == key.Open( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
		{
			if( ERROR_SUCCESS == key.QueryDWORDValue( _T("RefreshMode"), dwRefreshMode ) )
				bRefreshMode = ( 1 == dwRefreshMode ) ? TRUE : FALSE;
			else
				bRefreshMode = FALSE;	// [bug 8437]�L�[���Ȃ��ꍇ�͂�����}���ɂ��Ȃ��i�f�t�H���g�j
		}
		if( -1 == bRefreshMode )
			bRefreshMode = TRUE;
	}
	return (bRefreshMode) ? true : false;
}

// ������}���ĕ`��
void AheRefresh(IActiveView* ipActiveView, IEnvelope* ipEnv/*=NULL*/)
{
	if ( ipActiveView )
	{
		if ( AheIsRefreshMode() )
		{
			IEnvelopePtr ipEnvTmp( ipEnv );
			if ( ! ipEnvTmp )
				ipActiveView->get_Extent( &ipEnvTmp );

			IScreenDisplayPtr ipScreen;
			ipActiveView->get_ScreenDisplay( &ipScreen );

			if ( ipScreen )
			{
				IDisplayTransformationPtr ipTrans;
				ipScreen->get_DisplayTransformation( &ipTrans );

				if ( ipTrans )
				{
					ITrackCancelPtr ipCancel;
					ipScreen->get_CancelTracker( &ipCancel );

					HDC hdcCacheMemDC = NULL;
					ipScreen->get_CacheMemDC( 0, (OLE_HANDLE*)&hdcCacheMemDC );

					HWND hWnd = NULL;
					ipScreen->get_hWnd( (OLE_HANDLE*)&hWnd );
					HDC hdcScreen = ::GetDC(hWnd);

					RECT rRect={0};
					ipTrans->get_DeviceFrame( &rRect );
					const int nWidth = rRect.right - rRect.left;
					const int nHeight = rRect.bottom - rRect.top;

					// ��ƗpHDC�𔒂�
					::BitBlt( hdcCacheMemDC, rRect.left, rRect.top, nWidth, nHeight, NULL, 0, 0, WHITENESS );
					// ��ƗpHDC�ɕ`��
					ipActiveView->Output( (OLE_HANDLE)hdcCacheMemDC, 0, &rRect, ipEnvTmp, ipCancel );
					// ���C���ɕ`��
					::BitBlt( hdcScreen, rRect.left, rRect.top, nWidth, nHeight, hdcCacheMemDC, rRect.left, rRect.top, SRCCOPY );
					// �����̈��ݒ�
					ipScreen->Invalidate( ipEnvTmp, VARIANT_TRUE, esriViewBackground );
					// ���t���b�V��
					ipActiveView->PartialRefresh( esriViewForeground, NULL, ipEnvTmp );

					::ReleaseDC(hWnd, hdcScreen);
				}
			}
		}
		else
			ipActiveView->Refresh();
	}
}
