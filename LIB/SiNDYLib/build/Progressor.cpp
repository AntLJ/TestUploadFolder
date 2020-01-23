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
 * @brief CProgressor実装ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
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
 * @brief <b>プログレスダイアログを表示するための親ウィンドウハンドルをセットする</b>\n
 * プログレスダイアログ使用時には必ずセットする必要があります。
 * @param hWnd	[in]	親ウィンドウハンドル
 */
void CProgressor::SetHWND( HWND hWnd ){ m_hWnd = hWnd; }

/**
 * @brief <b>キャンセルトラッカーを取得する</b>\n
 * 無ければ自動的に作成されます。
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
				// デバッガでブレークを立てているときなどは裏に隠れてしまうので
				// メッセージポンプを回す
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

			// 使いまわしできないらしい…
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
