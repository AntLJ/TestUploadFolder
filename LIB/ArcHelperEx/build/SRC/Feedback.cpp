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

// Feedback.cpp: CFeedback クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AheGlobals.h"
#include "AheDraw.h"
#include "Feedback.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CFeedback::CFeedback() : m_piFeedback(NULL), m_nTolerance(8)
{
}

CFeedback::~CFeedback()
{

}

// フィードバックを開始
BOOL CFeedback::StartDrawSearchArea( IScreenDisplayPtr piScreen, IPointPtr piPoint, UINT unTolerance)
{
	// ポイントフィードバックインスタンス作成
	IMovePointFeedbackPtr piPointFeedback;
	if( SUCCEEDED( piPointFeedback.CreateInstance( CLSID_MovePointFeedback)))
	{
		// IScreenDisplayをもとに、表示円の半径算出
		double dbTwipe = unTolerance;
		OLE_HANDLE hWnd;
		if( SUCCEEDED( piScreen->get_hWnd( &hWnd)))
		{
			HDC hDC = ::GetDC( reinterpret_cast<HWND>( hWnd));
			if( NULL != hDC)
			{
				int nLogicX = GetDeviceCaps( hDC, LOGPIXELSX);
				int nLogicY = GetDeviceCaps( hDC, LOGPIXELSY);
				int nLogical = max( nLogicX, nLogicY);
				dbTwipe = nLogical;
				dbTwipe *= unTolerance;
				dbTwipe /= static_cast<double>( 72);

				::ReleaseDC( reinterpret_cast<HWND>( hWnd), hDC);
			}
		}

		// ポイントフィードバック初期化
		piPointFeedback->putref_Display( piScreen );
		piPointFeedback->putref_Symbol( AheGetPointSymbol( dbTwipe, 192, 192, 0, esriSMSCircle, TRUE, TRUE));
		piPointFeedback->Start( piPoint, piPoint );
		m_piFeedback = piPointFeedback;
		m_nTolerance = unTolerance;
		m_ipScreenDisplay = piScreen;

		return TRUE;
	}
	return FALSE;
}

// フィードバックの描画
BOOL CFeedback::DrawSearchArea( IPointPtr piPoint)
{
	if( NULL == m_piFeedback) return FALSE;

	m_piFeedback->MoveTo( piPoint);

	return TRUE;
}

// フィードバックの再描画
void CFeedback::ResetSearchArea( OLE_HANDLE hDC/* = NULL */)
{
	if( NULL == m_piFeedback) return;

	m_piFeedback->Refresh( hDC );
}

// フィードバックを停止
void CFeedback::StopDrawSearchArea()
{
	if( NULL == m_piFeedback) return;

	IPointPtr piPoint;
	m_piFeedback->Stop( &piPoint);
	m_piFeedback = NULL;
}
// 検索エリアを取得する（ArcObjectsバグ回避版）
#ifdef _DEBUG
void AheTraceSpatialReference( ISpatialReference* ipSpRef, LPCTSTR lpcszHeader ); // AheGlobalFunctions.h
#endif // ifdef _DEBUG
IGeometryPtr CFeedback::GetSearchArea( IPointPtr ipPoint, DOUBLE* dSearchRadius, ISpatialReferencePtr ipSpRef/* = NULL*/ ) const
{
	if( ipPoint == NULL || dSearchRadius == NULL || ! m_ipScreenDisplay ) return NULL;

	IGeometryPtr ipSearchArea;
	ITopologicalOperatorPtr ipTopo( ipPoint );
	
	*dSearchRadius = -1;	// 初期化
	if( ipTopo == NULL )
	{
		ATLASSERT( ipTopo != NULL );
		return NULL;
	}

	if( ipSpRef == NULL )
		ipPoint->get_SpatialReference( &ipSpRef );

	double dMinTolerance = -1.0;
	if( ipSpRef )
	{
#ifdef _DEBUG
		AheTraceSpatialReference( ipSpRef, _T("aaa") );
#endif // ifdef _DEBUG
		((ISpatialReferenceTolerancePtr)ipSpRef)->get_XYTolerance( &dMinTolerance );
		dMinTolerance *= 2.0; // XY許容値そのままではやはり失敗するようだ…
	}

	// 半径を計算（半径は +10 しない円で計算）
	IGeometryPtr ipSearchAreaTmp;
	double dSearchRadiusTmp = AheConvertPixelsToMapUnits( m_ipScreenDisplay, m_nTolerance );
	if( 0 < dMinTolerance )
		dSearchRadiusTmp = max( dMinTolerance, dSearchRadiusTmp ); // XY許容値x2より小さい場合は失敗するので、その場合はXY許容値x2とする
	ipTopo->Buffer( dSearchRadiusTmp, &ipSearchAreaTmp );
#ifdef _DEBUG
	esriGeometryType emType;
	ipSearchAreaTmp->get_GeometryType( &emType );
	_ASSERT_EXPR( esriGeometryPolygon == emType, _T("CFeedback::GetSearchArea() : ITopologicalOperator::Buffer()に失敗しました") );
#endif // ifdef _DEBUG
	ipSearchAreaTmp->putref_SpatialReference( ipSpRef );
	ipSearchAreaTmp->SnapToSpatialReference();
	IAreaPtr ipAreaTmp( ipSearchAreaTmp );

	if( ! ipAreaTmp )
		return AheGetClone(ipPoint); // バッファリングできなかった場合はしょうがないので渡されたポイントを返す

	{
		IPointPtr ipCenterPoint, ipStartPoint;
		((IAreaPtr)ipSearchAreaTmp)->get_Centroid( &ipCenterPoint );
		((IPointCollectionPtr)ipSearchAreaTmp)->get_Point( 0, &ipStartPoint );
		if( ((IProximityOperatorPtr)ipCenterPoint)->ReturnDistance( ipStartPoint, dSearchRadius ) != S_OK)
			*dSearchRadius = 0;
	}

	// 入力されたポイントを選択エリア半径でバッファし、選択エリアを作成
	//!< 円で検索すると僅かに入るフィーチャが検索できない場合あり！！
	//!< しょうがないのでドット数を10だけ増やしてやる（see Message-Id: <20021219.164152.16173513.furukawa@mr.ipc.pioneer.co.jp>）
	dSearchRadiusTmp = AheConvertPixelsToMapUnits( m_ipScreenDisplay, 10 );
	HRESULT hr = ((ITopologicalOperatorPtr)ipSearchAreaTmp)->Buffer( dSearchRadiusTmp, &ipSearchArea );
	// Bufferとれないほど拡大されているときには、無理やり四角形を作って対応
	if( !ipSearchArea )
	{
		IGeometryPtr ipGeom( CLSID_Envelope );
		double dX = 0.0, dY = 0.0;
		ipPoint->QueryCoords( &dX, &dY ); 
		((IEnvelopePtr)ipGeom)->PutCoords( dX - dMinTolerance, dY - dMinTolerance, dX + dMinTolerance, dY + dMinTolerance );
		ipSearchArea = ipGeom;
	}
#ifdef _DEBUG
	ipSearchArea->get_GeometryType( &emType );
	_ASSERT_EXPR( esriGeometryPolygon == emType, _T("CFeedback::GetSearchArea() : ITopologicalOperator::Buffer()に失敗しました") );
#endif // ifdef _DEBUG
	ipSearchArea->putref_SpatialReference( ipSpRef );
	ipSearchArea->SnapToSpatialReference();

	return ipSearchArea;
}

// 検索エリアを取得する
IGeometryPtr CFeedback::GetSearchArea( IPointPtr ipPoint, ISpatialReferencePtr ipSpRef ) const
{
	if( ! ipPoint || ! m_ipScreenDisplay )
	{
		_ASSERTE( ipPoint );
		return NULL;
	}

	IGeometryPtr ipRet; // 返り値
	ITopologicalOperatorPtr ipTopo( ipPoint );
	if( ! ipTopo )
	{
		_ASSERTE( ipTopo != NULL );
		return NULL;
	}

	if( ! ipSpRef )
		ipPoint->get_SpatialReference( &ipSpRef );

	double dSearchRadius = AheConvertPixelsToMapUnits( m_ipScreenDisplay, m_nTolerance );
	if( SUCCEEDED( ipTopo->Buffer( dSearchRadius, &ipRet ) ) )
	{
		// ipTopo::Bufferが成功して、ipRetがNULLということがあるかどうかはわからないが念のため
		if( !ipRet )
		{
			double dMinTolerance = -1.0;
			if( ipSpRef )
			{
				((ISpatialReferenceTolerancePtr)ipSpRef)->get_XYTolerance( &dMinTolerance );
				dMinTolerance *= 2.0; // XY許容値そのままではやはり失敗するようだ…
			}
			IGeometryPtr ipGeom( CLSID_Envelope );
			double dX = 0.0, dY = 0.0;
			ipPoint->QueryCoords( &dX, &dY ); 
			((IEnvelopePtr)ipGeom)->PutCoords( dX - dMinTolerance, dY - dMinTolerance, dX + dMinTolerance, dY + dMinTolerance );
			ipRet = ipGeom;
		}

		ipRet->putref_SpatialReference( ipSpRef );
		return ipRet;
	}

	// バッファリングに失敗した場合は全件件索回避の為に
	// 与えられたポイントの複製を返す
	_ASSERTE(false);
	return AheGetClone( ipPoint );
}
