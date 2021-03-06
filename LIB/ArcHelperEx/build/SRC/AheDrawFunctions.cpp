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
 * @file DrawFunctions.cpp
 * @brief 描画系グローバルヘルパー関数実装
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "DrawFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 * @brief 描画用マクロ
 */
#define DRAWGEOMETRY(disp,sym,method,geom,refscale,noref,sf,rec) \
	IDisplayTransformationPtr ipTrans; \
	double dScale = -1.0; \
	if( noref ) \
	{ \
		disp->get_DisplayTransformation( &ipTrans ); \
		ipTrans->get_ReferenceScale( &dScale ); \
		ipTrans->put_ReferenceScale( 0.0 ); \
	} \
	else if( refscale > 0.0 ) \
	{ \
		disp->get_DisplayTransformation( &ipTrans ); \
		ipTrans->get_ReferenceScale( &dScale ); \
		ipTrans->put_ReferenceScale( refscale ); \
	} \
	IScreenDisplayPtr ipScDisp( disp ); \
	if( rec && ipScDisp != NULL ) \
		ipScDisp->StartRecording(); \
	if( sf ) \
	{ \
		OLE_HANDLE hDC = 0; \
		if( ipScDisp != NULL ) \
			ipScDisp->get_WindowDC( &hDC ); \
		disp->StartDrawing( hDC, esriNoScreenCache ); \
	} \
	disp->SetSymbol( sym ); \
	disp->method( geom ); \
	if( sf ) \
		disp->FinishDrawing(); \
	if( rec && ipScDisp != NULL ) \
		ipScDisp->StopRecording(); \
	if( noref || refscale > 0.0 ) \
		ipTrans->put_ReferenceScale( dScale );

#define DRAWTEXT(disp,sym,method,geom,text,refscale,noref,sf,rec) \
	IDisplayTransformationPtr ipTrans; \
	double dScale = -1.0; \
	if( noref ) \
	{ \
		disp->get_DisplayTransformation( &ipTrans ); \
		ipTrans->get_ReferenceScale( &dScale ); \
		ipTrans->put_ReferenceScale( 0.0 ); \
	} \
	else if( refscale > 0.0 ) \
	{ \
		disp->get_DisplayTransformation( &ipTrans ); \
		ipTrans->get_ReferenceScale( &dScale ); \
		ipTrans->put_ReferenceScale( refscale ); \
	} \
	IScreenDisplayPtr ipScDisp( disp ); \
	if( rec && ipScDisp != NULL ) \
		ipScDisp->StartRecording(); \
	if( sf ) \
	{ \
		OLE_HANDLE hDC = 0; \
		if( ipScDisp != NULL ) \
			ipScDisp->get_WindowDC( &hDC ); \
		disp->StartDrawing( hDC, esriNoScreenCache ); \
	} \
	disp->SetSymbol( sym ); \
	BSTR bstrText = T2BSTR(text); \
	disp->method( geom, bstrText ); \
	SysFreeString(bstrText); \
	if( sf ) \
		disp->FinishDrawing(); \
	if( rec && ipScDisp != NULL ) \
		ipScDisp->StopRecording(); \
	if( noref || refscale > 0.0 ) \
		ipTrans->put_ReferenceScale( dScale );

// ポリゴンを描画する
void AheDrawPolygon( IDisplay* ipDisplay, ISymbol* ipSymbol, IPolygon* ipPolygon, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWGEOMETRY( ipDisplay, ipSymbol, DrawPolygon, ipPolygon, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}

// 矩形を描画する
void AheDrawRectangle( IDisplay* ipDisplay, ISymbol* ipSymbol, IEnvelope* ipEnvelope, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWGEOMETRY( ipDisplay, ipSymbol, DrawRectangle, ipEnvelope, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}

// ポリラインを描画する
void AheDrawPolyline( IDisplay* ipDisplay, ISymbol* ipSymbol, IPolyline* ipPolyline, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWGEOMETRY( ipDisplay, ipSymbol, DrawPolyline, ipPolyline, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}

// ポイントを描画する
void AheDrawPoint( IDisplay* ipDisplay, ISymbol* ipSymbol, IPoint* ipPoint, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWGEOMETRY( ipDisplay, ipSymbol, DrawPoint, ipPoint, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}

// 構成点を描画する
void AheDrawVertex( IDisplay* ipDisplay, ISymbol* ipSymbol, IGeometry* ipGeometry, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	IPolylinePtr ipPolyline;
	IPointCollectionPtr ipCol;
	IPolygonPtr ipPolygon;
	IPointPtr ipPoint, ipPoint2;
	IGeometryPtr ipVertexGeom;
	LONG lPointCount = 0;

	if( ipGeometry == NULL ) 
		return;

	if(SUCCEEDED( ipGeometry->QueryInterface( IID_IPolyline, (void**)&ipPolyline ) ) )
		ipCol = ipPolyline;
	else if( SUCCEEDED( ipGeometry->QueryInterface( IID_IPolygon, (void**)&ipPolygon ) ) ) {
		ipCol = ipPolygon;
	}
	else if( SUCCEEDED( ipGeometry->QueryInterface( IID_IPoint, (void**)&ipPoint ) ) ) {
		// ポイントのみ別処理
		AheDrawPoint( ipDisplay, ipSymbol, ipPoint, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
		return;
	}

	// 重複している点がある場合、重複を取り除く
	IPointCollectionPtr ipCol2( CLSID_Multipoint );
	ipCol->get_PointCount( &lPointCount );
	for( LONG i = 0; i < lPointCount; i++ ) {
		ipCol->get_Point( i, &ipPoint2 );
		ipCol2->AddPoint( ipPoint2 );
	}
	ITopologicalOperatorPtr ipTopo( ipCol2 );
	ipTopo->Simplify();
	ipCol2 = ipTopo;

	if( ipDisplay == NULL || ipCol2 == NULL )
		return;

	// ポイント数
	ipCol2->get_PointCount( &lPointCount );
	// 描画
	DOUBLE dScale;
	IDisplayTransformationPtr ipTrans;
	ipDisplay->get_DisplayTransformation( &ipTrans );
	ipTrans->get_ReferenceScale( &dScale );
	ipTrans->put_ReferenceScale( 0.0 );
	if( bDoStartFinish == TRUE )
	{
		UINT hDC;
		ipDisplay->get_hDC( &hDC );
		ipDisplay->StartDrawing( hDC, esriNoScreenCache );
	}
	ipDisplay->SetSymbol( ipSymbol );
	for( LONG i = 0; i < lPointCount; i++ )
	{
		ipCol2->get_Point( i, &ipPoint );
		ipDisplay->DrawPoint( ipPoint );
	}
	if( bDoStartFinish == TRUE )
		ipDisplay->FinishDrawing();
	ipTrans->put_ReferenceScale( dScale );
}

// マルチポイントを描画する
void AheDrawMultipoint( IDisplay* ipDisplay, ISymbol* ipSymbol, IMultipoint* ipPoints, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWGEOMETRY( ipDisplay, ipSymbol, DrawMultipoint, ipPoints, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}

// 文字を描画する
void AheDrawText( IDisplay* ipDisplay, ISymbol* ipSymbol, IGeometry* ipGeom, LPCTSTR lpcszText, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWTEXT( ipDisplay, ipSymbol, DrawText, ipGeom, lpcszText, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}
