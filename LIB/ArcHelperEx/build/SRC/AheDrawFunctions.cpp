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
 * @brief •`‰æŒnƒOƒ[ƒoƒ‹ƒwƒ‹ƒp[ŠÖ”ŽÀ‘•
 * @author ’n}‚c‚a§ì•”ŠJ”­ƒOƒ‹[ƒv ŒÃì‹MG
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
 * @brief •`‰æ—pƒ}ƒNƒ
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

// ƒ|ƒŠƒSƒ“‚ð•`‰æ‚·‚é
void AheDrawPolygon( IDisplay* ipDisplay, ISymbol* ipSymbol, IPolygon* ipPolygon, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWGEOMETRY( ipDisplay, ipSymbol, DrawPolygon, ipPolygon, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}

// ‹éŒ`‚ð•`‰æ‚·‚é
void AheDrawRectangle( IDisplay* ipDisplay, ISymbol* ipSymbol, IEnvelope* ipEnvelope, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWGEOMETRY( ipDisplay, ipSymbol, DrawRectangle, ipEnvelope, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}

// ƒ|ƒŠƒ‰ƒCƒ“‚ð•`‰æ‚·‚é
void AheDrawPolyline( IDisplay* ipDisplay, ISymbol* ipSymbol, IPolyline* ipPolyline, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWGEOMETRY( ipDisplay, ipSymbol, DrawPolyline, ipPolyline, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}

// ƒ|ƒCƒ“ƒg‚ð•`‰æ‚·‚é
void AheDrawPoint( IDisplay* ipDisplay, ISymbol* ipSymbol, IPoint* ipPoint, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWGEOMETRY( ipDisplay, ipSymbol, DrawPoint, ipPoint, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}

// \¬“_‚ð•`‰æ‚·‚é
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
		// ƒ|ƒCƒ“ƒg‚Ì‚Ý•Êˆ—
		AheDrawPoint( ipDisplay, ipSymbol, ipPoint, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
		return;
	}

	// d•¡‚µ‚Ä‚¢‚é“_‚ª‚ ‚éê‡Ad•¡‚ðŽæ‚èœ‚­
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

	// ƒ|ƒCƒ“ƒg”
	ipCol2->get_PointCount( &lPointCount );
	// •`‰æ
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

// ƒ}ƒ‹ƒ`ƒ|ƒCƒ“ƒg‚ð•`‰æ‚·‚é
void AheDrawMultipoint( IDisplay* ipDisplay, ISymbol* ipSymbol, IMultipoint* ipPoints, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWGEOMETRY( ipDisplay, ipSymbol, DrawMultipoint, ipPoints, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}

// •¶Žš‚ð•`‰æ‚·‚é
void AheDrawText( IDisplay* ipDisplay, ISymbol* ipSymbol, IGeometry* ipGeom, LPCTSTR lpcszText, double dRefScale/* = 0.0*/, bool bIgnoreRefScale/* = true*/, bool bDoStartFinish/* = true*/, bool bDoRecording/* = false*/ )
{
	DRAWTEXT( ipDisplay, ipSymbol, DrawText, ipGeom, lpcszText, dRefScale, bIgnoreRefScale, bDoStartFinish, bDoRecording );
}
