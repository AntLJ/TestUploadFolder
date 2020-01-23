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
 * @file Progressor.cpp
 * @brief CProgressor�����t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "Progressor.h"

#include "util.h"
#include "AheLicenseFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CProgressor::CProgressor() : m_ipTrackCancel(NULL)
{
}
CProgressor::~CProgressor()
{
	if( m_ipTrackCancel )
	{
		m_ipTrackCancel->Release();
		m_ipTrackCancel = NULL;
	}
}

CProgressor::CProgressor( ITrackCancel* lp )
{
	m_ipTrackCancel = lp;
	if( NULL != m_ipTrackCancel )
		m_ipTrackCancel->AddRef();
}

/**
 * @brief <b>�v���O���X�_�C�A���O��\�����邽�߂̐e�E�B���h�E�n���h�����Z�b�g����</b>\n
 * �v���O���X�_�C�A���O�g�p���ɂ͕K���Z�b�g����K�v������܂��B
 * @param hWnd	[in]	�e�E�B���h�E�n���h��
 */
void CProgressor::SetHWND( HWND hWnd ){ m_hWnd = hWnd; }

/**
 * @brief <b>�L�����Z���g���b�J�[���擾����</b>\n
 * ������Ύ����I�ɍ쐬����܂��B
 */
ITrackCancelPtr CProgressor::GetTrackCancel() const
{
	if( ! m_ipTrackCancel )
	{
		if( FAILED( CoCreateInstance( CLSID_CancelTracker, NULL, CLSCTX_ALL, IID_ITrackCancel, (void**)&m_ipTrackCancel ) ) )
			_ASSERTE(false);
	}
	return m_ipTrackCancel;
}
IProgressorPtr CProgressor::GetProgressor() const
{
	IProgressorPtr ipRet;
	ITrackCancelPtr ipTrackCancel( GetTrackCancel() );
	if( NULL != ipTrackCancel )
	{
		ipTrackCancel->get_Progressor( &ipRet );
		if( NULL == ipRet )
		{
			IProgressDialogFactoryPtr ipFact;
			CLSID clsid = ( CheckLicense( esriLicenseProductCodeStandard ) ) ? CLSID_ProgressDialogFactory : CLSID_SMWProgressDialogFactory;
			if( SUCCEEDED( ipFact.CreateInstance( clsid ) ) )
			{
				IStepProgressorPtr ipProgressor;
				if( SUCCEEDED( ipFact->Create( ipTrackCancel, (OLE_HANDLE)m_hWnd, &ipProgressor ) ) )
				{
					ipProgressor->put_MaxRange( 100 );
					ipProgressor->put_MinRange( 0 );

					ipRet = ipProgressor;
				}
				else _ASSERTE(false);
			}
			else _ASSERTE(false);
		}
	}
	return ipRet;
}
void CProgressor::ShowProgressBar( bool bShow/* = true*/ ) const
{
	IProgressorPtr ipProgressor( GetProgressor() );
	if( NULL != ipProgressor )
	{
		HRESULT hr = ( bShow ) ? ipProgressor->Show() : ipProgressor->Hide();
		if( FAILED(hr) ) _ASSERTE(false);
	}
}
void CProgressor::ShowProgressDialog( bool bShow/* = true*/ )
{
	_IProgressDialogPtr ipProgressor( GetProgressor() );
	if( NULL != ipProgressor )
	{
		if( bShow )
		{
			if( SUCCEEDED( ipProgressor->ShowDialog() ) )
			{
				// �f�o�b�K�Ńu���[�N�𗧂ĂĂ���Ƃ��Ȃǂ͗��ɉB��Ă��܂��̂�
				// ���b�Z�[�W�|���v����
				MSG msg;
				while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){ TranslateMessage( &msg ); DispatchMessage( &msg ); }
			}
			else
				_ASSERTE(false);
		}
		else {
			if( SUCCEEDED( ipProgressor->HideDialog() ) )
			{
			}
			else
				_ASSERTE(false);

			// �g���܂킵�ł��Ȃ��炵���c
			m_ipTrackCancel->Release();
			m_ipTrackCancel = NULL;
		}
	}
}
bool CProgressor::Continue() const
{
	bool bRet = true;
	ITrackCancelPtr ipTrackCancel( GetTrackCancel() );
	if( NULL != ipTrackCancel )
	{
		VARIANT_BOOL vbKeepGoing = VARIANT_FALSE;
		if( SUCCEEDED( ipTrackCancel->Continue( &vbKeepGoing ) ) )
			bRet = VB2bool(vbKeepGoing);
	}
	return bRet;
}
