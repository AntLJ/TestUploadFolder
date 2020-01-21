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
 * @brief ArcObjectsグローバルヘルパー関数実装
 *
 * このファイルには拡大縮小、スクロールなどに関する
 * グローバル関数が実装されています。
 * 
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @date 2007/01/26
 * $Id$
 */
#include "stdafx.h"
#include "AheZooMoveFunctions.h"
#include <math.h>
#include "WinLib/WaitCursor.h"


// ZooMoveToolのキー移動拡大縮小処理
bool AheKeyZooMove( IActiveView* ipActiveView, long lKeyCode, long lShift )
{
	CWaitCursor cursor;
	bool bRet = false; // 返り値

	switch( lKeyCode )
	{
	// スペースキーで、カーソルを減マップのセンターへ移動する
	case VK_SPACE:
		bRet = AheMoveCursorToCenter( ipActiveView );
		break;

	//　NumPAD側で地図の移動を処理。+CTRLで一画面移動、通常は半画面移動
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
	//　NumPADの5は現状カーソル位置を画面のセンターへ移動する
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

	//　NumPADの+-でZoomUP/DOWN
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
	//　NumPADの/*で最大最小のZoom
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
	// 1キーで再描画
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
// マウスポインタの位置が中心にくるようにスクロールする
bool AheMoveCenterToMousePointer( IActiveView* ipActiveView, bool bSuppressEvents )
{
	if( !ipActiveView )
		return false;

	// マウスポインタのデバイス座標取得
	IScreenDisplayPtr ipDisp;
	ipActiveView->get_ScreenDisplay( &ipDisp );
	if( !ipDisp )
		return false;

	POINT stPoint = { 0, 0 };
	HWND hWnd = 0;
	::GetCursorPos( &stPoint );
	ipDisp->get_hWnd( reinterpret_cast<OLE_HANDLE*>( &hWnd ) );
	::ScreenToClient( hWnd, &stPoint );

	// 地図座標に変換
	IPointPtr ipPoint;
	IDisplayTransformationPtr ipDispTrans;
	ipDisp->get_DisplayTransformation( &ipDispTrans );
	if( !ipDispTrans )
		return false;

	ipDispTrans->ToMapPoint( stPoint.x, stPoint.y, &ipPoint );

	// 中心へ移動
	IEnvelopePtr ipEnv;
	ipActiveView->get_Extent( &ipEnv );
	ipEnv->CenterAt( ipPoint );

	// [Bug 3772]拡大が効かなくなるバグへの対応
	// マップのサイズは変わらないのに、座標移動前のEnvelopeを座標移動後の
	// Envelopeに設定するとスケールが変わることがあるという問題
	// 一旦イベントをとめて、VisibleBoundsで領域を設定する
	// [Bug 5495]しかし、イベントをとめてしまうと描画の変更はあっても、内部的には前の座標位置であるため
	// MoveCenterToMousePointerの後にイベントを発行する処理がない場合には、イベントとめないことにする
	// 一応この処理であれば、[Bug 3772]と両立はできる
	// TODO:この処理だと、既にイベント通知が停止されていた場合に強制的に再開されることになるため、
	// あとで、ちゃんとイベント停止されているかどうか確認する。
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

// カーソルをビューのセンターに移動
bool AheMoveCursorToCenter( IActiveView* ipActiveView )
{
	if( !ipActiveView )
		return false;

	// マップの座標サイズ取得
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

	// マップの中心を取得
	POINT stPoint;
	stPoint.x = (rect.left + rect.right )/2;
	stPoint.y = (rect.bottom + rect.top)/2;

	// クライアント領域の座標からスクリーン座標に変換
	HWND hWnd;
	ipDisp->get_hWnd( reinterpret_cast<OLE_HANDLE*>( &hWnd ) );
	::ClientToScreen( hWnd, &stPoint );

	// マウスカーソル移動
	::SetCursorPos( stPoint.x, stPoint.y );
	return true;
}

// 指定ステップ数分の移動
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
		// [bug 5970] 拡大縮小ツールで拡大した時に座標がずれることがある
		// 拡大する場合はスケール1/2倍、縮小する場合はスケール2倍(現スケールから)
		ipMap->put_MapScale( dCurrent * ( bUpScale ? 0.5 : 2.0 ) );

		return true;
	}

	return false;
}

// MapFANのZM値に則した拡大率での拡大処理
bool AheZoomIn( IActiveView* ipActiveView )
{
	return AheZoom( ipActiveView, true );
}

// MapFANのZM値に則した拡大率での縮小処理
bool AheZoomOut( IActiveView* ipActiveView )
{
	return AheZoom( ipActiveView, false );
}

// 再描画モードを取得
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
				bRefreshMode = FALSE;	// [bug 8437]キーがない場合はちらつき抑制にしない（デフォルト）
		}
		if( -1 == bRefreshMode )
			bRefreshMode = TRUE;
	}
	return (bRefreshMode) ? true : false;
}

// ちらつき抑制再描画
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

					// 作業用HDCを白に
					::BitBlt( hdcCacheMemDC, rRect.left, rRect.top, nWidth, nHeight, NULL, 0, 0, WHITENESS );
					// 作業用HDCに描画
					ipActiveView->Output( (OLE_HANDLE)hdcCacheMemDC, 0, &rRect, ipEnvTmp, ipCancel );
					// メインに描画
					::BitBlt( hdcScreen, rRect.left, rRect.top, nWidth, nHeight, hdcCacheMemDC, rRect.left, rRect.top, SRCCOPY );
					// 無効領域を設定
					ipScreen->Invalidate( ipEnvTmp, VARIANT_TRUE, esriViewBackground );
					// リフレッシュ
					ipActiveView->PartialRefresh( esriViewForeground, NULL, ipEnvTmp );

					::ReleaseDC(hWnd, hdcScreen);
				}
			}
		}
		else
			ipActiveView->Refresh();
	}
}
