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
* @file AheDraw.cpp
*
* @brief Arc Helper 描画系グローバル関数実装ファイル
*/
#include "stdafx.h"

#include "AheGlobals.h"
#include "AheDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR FONT_DISP = _T("ＭＳ Ｐゴシック");
#else
	LPCTSTR FONT_DISP = _T("Georgia");
#endif
}



/////////////////////////////////////////////////////////////////////////////
//
// 色及びシンボルに関する関数
//
/////////////////////////////////////////////////////////////////////////////
IColorPtr AheGetColor( LONG red, LONG green, LONG blue )
{
	IRgbColorPtr ipRgbColor( CLSID_RgbColor );
	if( ipRgbColor != NULL )
	{
		ipRgbColor->put_Red( red );
		ipRgbColor->put_Blue( blue );
		ipRgbColor->put_Green( green );
	}
	return ipRgbColor;
}
IColorPtr AheGetColor( COLORREF color )
{
	return AheGetColor( GetRValue( color ), GetGValue( color ), GetBValue( color ) );
}
ISymbolPtr AheGetPointSymbol( double size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bDrawXOR, BOOL bOutLine )
{
	ISimpleMarkerSymbolPtr ipColorSym( CLSID_SimpleMarkerSymbol );

	if( ipColorSym != NULL )
	{
		ipColorSym->put_Color( AheGetColor( red, green, blue ) );
		ipColorSym->put_Size( size);
		ipColorSym->put_Style( style );
		if( bOutLine == TRUE ) 
		{
			ipColorSym->put_Outline( VARIANT_TRUE );
			ipColorSym->put_OutlineSize( 1 );
			ipColorSym->put_OutlineColor( AheGetColor( 255 - red, 255 - green, 255 - blue ) );
		}

		if( bDrawXOR == TRUE ) {
			// ドローモード
			((ISymbolPtr)ipColorSym)->put_ROP2( esriROPXOrPen );
		}
	}

	return (ISymbolPtr)ipColorSym;
}
ISymbolPtr AheGetPointSymbol( double size, COLORREF color, esriSimpleMarkerStyle style, BOOL bDrawXOR, BOOL bOutLine )
{
	return AheGetPointSymbol( size, GetRValue( color ), GetGValue( color ), GetBValue( color ), style, bDrawXOR, bOutLine );
}
ISymbolPtr AheGetLineSymbol( double width, LONG red, LONG green, LONG blue, esriSimpleLineStyle style, BOOL bDrawXOR )
{
	ISimpleLineSymbolPtr ipColorSym( CLSID_SimpleLineSymbol );

	if( ipColorSym != NULL )
	{
		ipColorSym->put_Color( AheGetColor( red, green, blue ) );
		ipColorSym->put_Width( width );
		ipColorSym->put_Style( style );

		if( bDrawXOR == TRUE )
			((ISymbolPtr)ipColorSym)->put_ROP2( esriROPNotXOrPen );
	}
	return (ISymbolPtr)ipColorSym;
}
ISymbolPtr AheGetLineSymbol( double width, COLORREF color, esriSimpleLineStyle style, BOOL bDrawXOR )
{
	return AheGetLineSymbol( width, GetRValue( color ), GetGValue( color ), GetBValue( color ), style, bDrawXOR );
}
ISymbolPtr AheGetPolygonSymbol( LONG red, LONG green, LONG blue, esriSimpleFillStyle style, BOOL bDrawXOR )
{
	ISimpleFillSymbolPtr	ipFillSymbol( CLSID_SimpleFillSymbol );
	ISymbolPtr				ipSymbol;
	ISymbolPtr				ipLSymbol;
	IColorPtr				ipColor( CLSID_RgbColor );

	if( ipFillSymbol != NULL )
	{
		ipFillSymbol->put_Color( AheGetColor( red, green, blue ) );
		ipFillSymbol->put_Style( style );
		
		if( bDrawXOR == TRUE )
			((ISymbolPtr)ipFillSymbol)->put_ROP2( esriROPNotXOrPen );

		// アウトライン
		ILineSymbolPtr ipLineSymbol;
		ipFillSymbol->get_Outline( &ipLineSymbol );
		ipLineSymbol->put_Color( AheGetColor( 255 - red, 255 - green, 255 - blue ) );
		ipLineSymbol->put_Width( 2 );

		if( bDrawXOR == TRUE )
			((ISymbolPtr)ipLineSymbol)->put_ROP2( esriROPNotXOrPen );

		ipFillSymbol->put_Outline( ipLineSymbol );
	}

	return ipFillSymbol;
}
ISymbolPtr AheGetPolygonSymbol( COLORREF color, esriSimpleFillStyle style, BOOL bDrawXOR )
{
	return AheGetPolygonSymbol( GetRValue( color ), GetGValue( color ), GetBValue( color ), style, bDrawXOR );
}
/////////////////////////////////////////////////////////////////////////////
//
// フィーチャ描画
//
/////////////////////////////////////////////////////////////////////////////
void AheDrawFeature( IDisplayPtr piScreenDisplay, IGeometryPtr ipGeom, BOOL bDoStartStop )
{
	IPolygonPtr ipPolygon( ipGeom );
	IPolylinePtr ipPolyline( ipGeom );
	IPointPtr ipPoint( ipGeom );

	if( ipPolygon != NULL )AheDrawPolygon( piScreenDisplay, ipPolygon, bDoStartStop );
	if( ipPolyline != NULL )AheDrawPolyline( piScreenDisplay, ipPolyline, bDoStartStop );
	if( ipPoint != NULL )AheDrawPoint( piScreenDisplay, ipPoint, bDoStartStop );
}

void AheDrawVertex( IDisplayPtr piScreenDisplay, IFeaturePtr ipFeature, BOOL bXOR, LONG size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bOutLine, BOOL bDoStartStop )
{
	IGeometryPtr ipFeatGeom;

	if( ipFeature == NULL ) return;

	ipFeature->get_Shape( &ipFeatGeom );
	AheDrawGeomVertex( piScreenDisplay, ipFeatGeom, bXOR, size, red, green, blue, style, bOutLine, bDoStartStop );
}	

void AheDrawGeomVertex( IDisplayPtr piScreenDisplay, IGeometryPtr ipFeatGeom, BOOL bXOR, LONG size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bOutLine, BOOL bDoStartStop )
{
	IPolylinePtr ipPolyline;
	IPointCollectionPtr ipCol;
	IPolygonPtr ipPolygon;
	IPointPtr ipPoint, ipPoint2;
	IGeometryPtr ipVertexGeom;
	LONG lPointCount = 0;

	if( ipFeatGeom == NULL ) return;

	if(SUCCEEDED( ipFeatGeom->QueryInterface( IID_IPolyline, (void**)&ipPolyline ) ) )
		ipCol = ipPolyline;
	else if( SUCCEEDED( ipFeatGeom->QueryInterface( IID_IPolygon, (void**)&ipPolygon ) ) ) {
		ipCol = ipPolygon;
	}
	else if( SUCCEEDED( ipFeatGeom->QueryInterface( IID_IPoint, (void**)&ipPoint ) ) ) {
		// ポイントのみ別処理
		AheDrawPoint( piScreenDisplay, ipPoint, bXOR, size, red, green, blue, style, bOutLine, bDoStartStop );
		return;
	}
	// 上記3つに当てはまらない場合は、IPointCollectionがNULLのままきてい、下で落ちてしまうため、
	// マルチ系ジオメトリはとりあえず構成点ハイライトさせないでおく。
	// もし正式にマルチ系のジオメトリに正式対応する場合にはしっかりコード書くこと
	else if( SUCCEEDED( ipFeatGeom->QueryInterface( IID_IPointCollection, (void**)&ipCol ) ) )
		;
	else
		return;

	// 重複している点がある場合、重複を取り除く
	IPointCollectionPtr ipCol2( CLSID_Multipoint );
	((IGeometryPtr)ipCol2)->putref_SpatialReference( AheGetSpatialReference( ipFeatGeom ) ); // 空間参照付与
	ipCol->get_PointCount( &lPointCount );
	for( LONG i = 0; i < lPointCount; i++ ) {
		ipCol->get_Point( i, &ipPoint2 );
		ipCol2->AddPoint( ipPoint2 );
	}
	ITopologicalOperatorPtr ipTopo( ipCol2 );
	ipTopo->Simplify();
	ipCol2 = ipTopo;
	AheDrawPoints( piScreenDisplay, ipCol2, bXOR, size, red, green, blue, style, bOutLine, bDoStartStop );
}

/////////////////////////////////////////////////////////////////////////////
//
// ポイント系描画
//
/////////////////////////////////////////////////////////////////////////////

void AheDrawPoint( IDisplayPtr piScreenDisplay, IPointPtr ipPoint, BOOL bXOR, LONG size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bOutLine, BOOL bDoStartStop )
{
	if( piScreenDisplay == NULL || ipPoint == NULL ) return;

	IGeometryPtr ipGeom( ipPoint );

	// 描画
	DOUBLE dScale;
	IDisplayTransformationPtr ipTrans;
	piScreenDisplay->get_DisplayTransformation( &ipTrans );
	ipTrans->get_ReferenceScale( &dScale );
	ipTrans->put_ReferenceScale( 0.0 );
	if( bDoStartStop == TRUE )
	{
		UINT hDC;
		piScreenDisplay->get_hDC( &hDC);
		piScreenDisplay->StartDrawing( hDC, esriNoScreenCache );
	}
	piScreenDisplay->SetSymbol( AheGetPointSymbol( size, red, green, blue, style, bXOR, bOutLine ) );
	piScreenDisplay->DrawPoint( ipPoint );
	if( bDoStartStop == TRUE )
		piScreenDisplay->FinishDrawing();
	ipTrans->put_ReferenceScale( dScale );
}

void AheDrawPoints( IDisplayPtr piScreenDisplay, IPointCollectionPtr ipPointCol, BOOL bXOR, LONG size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bOutLine, BOOL bDoStartStop )
{
	if( piScreenDisplay == NULL || ipPointCol == NULL ) return;

	IPointPtr ipPoint;
	LONG lPointCount = 0;

	// ポイント数
	ipPointCol->get_PointCount( &lPointCount );
//	ATLTRACE("CDrawTool::DrawPoints : 描画点数：%d\n", lPointCount );
	// 描画
	DOUBLE dScale;
	IDisplayTransformationPtr ipTrans;
	piScreenDisplay->get_DisplayTransformation( &ipTrans );
	ipTrans->get_ReferenceScale( &dScale );
	ipTrans->put_ReferenceScale( 0.0 );
	if( bDoStartStop == TRUE )
	{
		UINT hDC;
		piScreenDisplay->get_hDC( &hDC );
		piScreenDisplay->StartDrawing( hDC, esriNoScreenCache );
	}
	piScreenDisplay->SetSymbol( AheGetPointSymbol( size, red, green, blue, style, bXOR, bOutLine ) );
	for( LONG i = 0; i < lPointCount; i++ )
	{
		ipPointCol->get_Point( i, &ipPoint );
			piScreenDisplay->DrawPoint( ipPoint );
	}
	if( bDoStartStop == TRUE )
		piScreenDisplay->FinishDrawing();
	ipTrans->put_ReferenceScale( dScale );
}

void AheDrawMultipoint( IDisplayPtr piScreenDisplay, IGeometryPtr ipGeom, BOOL bXOR, LONG size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bOutLine, BOOL bDoStartStop )
{
	if( piScreenDisplay == NULL || ipGeom == NULL ) return;

	IPointPtr ipPoint;
	LONG lPointCount = 0;

	// 描画
	DOUBLE dScale;
	IDisplayTransformationPtr ipTrans;
	piScreenDisplay->get_DisplayTransformation( &ipTrans );
	ipTrans->get_ReferenceScale( &dScale );
	ipTrans->put_ReferenceScale( 0.0 );
	if( bDoStartStop == TRUE )
	{
		UINT hDC;
		piScreenDisplay->get_hDC( &hDC );
		piScreenDisplay->StartDrawing( hDC, esriNoScreenCache );
	}
	piScreenDisplay->SetSymbol( AheGetPointSymbol( size, red, green, blue, style, bXOR, bOutLine ) );

	esriGeometryType type;
	ipGeom->get_GeometryType( &type );
	if ( type == esriGeometryPoint )
		piScreenDisplay->DrawPoint( ipGeom );
	else
		piScreenDisplay->DrawMultipoint( ipGeom );

	if( bDoStartStop == TRUE )
		piScreenDisplay->FinishDrawing();
	ipTrans->put_ReferenceScale( dScale );
}

/////////////////////////////////////////////////////////////////////////////
//
// ライン系描画
//
/////////////////////////////////////////////////////////////////////////////
void AheDrawPolyline( IDisplayPtr piScreenDisplay, IPolylinePtr ipPolyline, BOOL bDoStartStop )
{
	AheDrawPolyline( piScreenDisplay, ipPolyline, RGB( 0, 200, 50), 1, esriSLSSolid, bDoStartStop );
}

void AheDrawPolyline( IDisplayPtr piScreenDisplay, IPolylinePtr piPolyline, COLORREF rgbColor, int nWidth, esriSimpleLineStyle emStyle, BOOL bDoStartStop/* = TRUE*/, BOOL bXOR/* = FALSE*/ )
{
	if( piScreenDisplay == NULL || piPolyline == NULL ) return;

	// 描画
	DOUBLE dScale;
	IDisplayTransformationPtr ipTrans;
	piScreenDisplay->get_DisplayTransformation( &ipTrans );
	ipTrans->get_ReferenceScale( &dScale );
	ipTrans->put_ReferenceScale( 0.0 );
	if( bDoStartStop == TRUE )
	{
		UINT hDC;
		piScreenDisplay->get_hDC( &hDC);
		piScreenDisplay->StartDrawing( hDC, esriNoScreenCache);
	}
	piScreenDisplay->SetSymbol( AheGetLineSymbol( nWidth, rgbColor, emStyle, bXOR ) );
	piScreenDisplay->DrawPolyline( piPolyline);
	if( bDoStartStop == TRUE )
		piScreenDisplay->FinishDrawing();
	ipTrans->put_ReferenceScale( dScale );
}
void AheDrawArrowline( IDisplayPtr ipScreenDisplay, IPolylinePtr ipPolyline, COLORREF rgbColor, BOOL bDoStartStop, BOOL bDrawArrow/* = TRUE*/, BOOL bOffset/* = FALSE*/, BOOL bOffsetReverse/* = FALSE*/ )
{
	if( ipScreenDisplay == NULL || ipPolyline == NULL ) return;

	ISymbolPtr ipSymbol;
	IColorPtr ipColor( AheGetColor( rgbColor ) );

	if( bDrawArrow == TRUE )
	{
		ILinePropertiesPtr ipLineProperties( CLSID_CartographicLineSymbol );
		ILineDecorationPtr ipLineDecoration( CLSID_LineDecoration );

		IArrowMarkerSymbolPtr ipArrowMarkerSymbol( CLSID_ArrowMarkerSymbol );
 		ipArrowMarkerSymbol->put_Style( esriAMSPlain );
		ipArrowMarkerSymbol->put_Color( ipColor );
		ISymbolPtr ipArrowSymbol( ipArrowMarkerSymbol );

		ISimpleLineDecorationElementPtr ipLineDecElement( CLSID_SimpleLineDecorationElement );

		ipLineDecElement->put_Rotate( VARIANT_TRUE );
		ipLineDecElement->put_MarkerSymbol( ipArrowMarkerSymbol );
		ipLineDecElement->AddPosition( 1 );

		ipLineDecoration->AddElement( ipLineDecElement );
				
		ipLineProperties->putref_LineDecoration( ipLineDecoration );
		ICartographicLineSymbolPtr ipCartographicLineSymbol( ipLineProperties );
				
		ipCartographicLineSymbol->put_Color( ipColor );
		ipCartographicLineSymbol->put_Width( 2 );
		ipCartographicLineSymbol->put_Cap( esriLCSSquare );
		ipCartographicLineSymbol->put_Join( esriLJSRound );

		ipSymbol = ipCartographicLineSymbol;
	}
	else {
		/// 指定された色、幅のシンプルラインシンボルを作成する
		ISimpleLineSymbolPtr ipLineSymbol( CLSID_SimpleLineSymbol );
		ipLineSymbol->put_Color( ipColor );
		ipLineSymbol->put_Width( 2 );
		ipLineSymbol->put_Style( esriSLSSolid );

		ipSymbol = ipLineSymbol;
	}

	IDisplayTransformationPtr ipTrans;
	ipScreenDisplay->get_DisplayTransformation( &ipTrans );
	if( bOffset == TRUE )
	{
		/// 矢印の描画単位を取得
		double dResult;
		ipTrans->FromPoints( 4.0, &dResult );

		/// スケールを取得
		double dScale=0;
		ipTrans->get_ScaleRatio(&dScale);

		if (dScale < 3500) {
			dResult *= 1.5;
		} else if (dScale < 5000 ) {
			dResult *= 1.2;
		}

		IConstructCurvePtr pIConstructCurve;
		pIConstructCurve.CreateInstance(CLSID_Polyline);
		((IGeometryPtr)pIConstructCurve)->putref_SpatialReference( AheGetSpatialReference( ipPolyline ) ); // 空間参照付与

		// 作成したラインをずらしてみる
		if( bOffsetReverse == TRUE ) dResult = -dResult;
		CComVariant vaOffsetHow( esriConstructOffsetMitered + esriConstructOffsetSimple );
		if ( SUCCEEDED( pIConstructCurve->ConstructOffset(ipPolyline, dResult, &vaOffsetHow ) ) )
			ipPolyline = pIConstructCurve;
	}
	// 描画
	DOUBLE dScale;
	ipTrans->get_ReferenceScale( &dScale );
	ipTrans->put_ReferenceScale( 0.0 );
	if( bDoStartStop == TRUE )
	{
		UINT hDC;
		ipScreenDisplay->get_hDC( &hDC );
		ipScreenDisplay->StartDrawing( hDC, esriNoScreenCache);
	}
	ipScreenDisplay->SetSymbol( ipSymbol );
	ipScreenDisplay->DrawPolyline( ipPolyline );
	if( bDoStartStop == TRUE )
		ipScreenDisplay->FinishDrawing();
	ipTrans->put_ReferenceScale( dScale );
}

void AheDrawArrowline2( IDisplayPtr ipScreenDisplay, std::list<LQ> Data, std::map<COLORREF, SYMBOL> mapColor, LONG lDispDrawMode, DOUBLE dbArrowSize)
{
	if( ipScreenDisplay == NULL || Data.size() == 0) return;

	/// シンボルの作成
	ITopologicalOperator2Ptr	ipTopo;
	IGeometryCollectionPtr		ipGeomColl;
	IDisplayTransformationPtr	ipTrans;
	ISymbolPtr					pISymbol;
	ISimpleTextSymbolPtr		ipTextSymbol( CLSID_TextSymbol );
	IFontDispPtr				ipFontDisp( CLSID_StdFont );
	IPolylinePtr				piPoly, piPoly2;
	double						dResult, dResult2, dScale;
	CString						strIDString;
	CY							cy;
	LONG						lGCount;

	ipScreenDisplay->get_DisplayTransformation( &ipTrans );

	/// スケールを取得
	ipTrans->get_ScaleRatio( &dScale );

	/// 矢印の描画単位を取得
	ipTrans->FromPoints( 4.0, &dResult );
	if ( dScale < 1000 )
		dResult *= 1.5;
	else if (dScale < 2000 )
		dResult *= 1.4;
	else if (dScale < 3000 )
		dResult *= 1.3;
	else if (dScale < 4000 )
		dResult *= 1.2;
	else 
		dResult *= 1.1;

	// IFontDisp作成
	((IFontPtr)ipFontDisp)->put_Name( CComBSTR(FONT_DISP) );

	cy.Hi = 0;
	cy.Lo = 16 * 7500;
	((IFontPtr)ipFontDisp)->put_Size( cy );

	// ITextSymbol設定
	ipTextSymbol->put_Font( ipFontDisp );
	ipTextSymbol->put_Angle( 0 );
	ipTextSymbol->put_Size( 16 * 0.75f );
	ipTextSymbol->put_VerticalAlignment( esriTVABottom );
	ipTextSymbol->put_HorizontalAlignment( esriTHACenter );

	for ( std::map<COLORREF, SYMBOL>::iterator it = mapColor.begin(); it != mapColor.end(); it++) {
//		ATLTRACE(_T("COLOR : %x\n"), (*it).first);
		IColorPtr ipColor( AheGetColor((*it).first) );

		/// Arrow Symbol
		ILinePropertiesPtr ipLineProperties( CLSID_CartographicLineSymbol );
		ILineDecorationPtr ipLineDecoration( CLSID_LineDecoration );

		IArrowMarkerSymbolPtr ipArrowMarkerSymbol( CLSID_ArrowMarkerSymbol );
 		ipArrowMarkerSymbol->put_Style( esriAMSPlain );
		ipArrowMarkerSymbol->put_Color( ipColor );
		ipArrowMarkerSymbol->put_Size( dbArrowSize );
		ISymbolPtr ipArrowSymbol( ipArrowMarkerSymbol );

		ISimpleLineDecorationElementPtr ipLineDecElement( CLSID_SimpleLineDecorationElement );

		ipLineDecElement->put_Rotate( VARIANT_TRUE );
		ipLineDecElement->put_MarkerSymbol( ipArrowMarkerSymbol );
		ipLineDecElement->AddPosition( 1 );

		ipLineDecoration->AddElement( ipLineDecElement );

		ipLineProperties->putref_LineDecoration( ipLineDecoration );
		ICartographicLineSymbolPtr ipCartographicLineSymbol( ipLineProperties );
				
		ipCartographicLineSymbol->put_Color( ipColor );
		ipCartographicLineSymbol->put_Width( 2 );
		ipCartographicLineSymbol->put_Cap( esriLCSSquare );
		ipCartographicLineSymbol->put_Join( esriLJSRound );

		(*it).second.piArrowSymbol = ipCartographicLineSymbol;

		/// Arrow Symbol Narrow
		ILinePropertiesPtr ipLineProperties2( CLSID_CartographicLineSymbol );
		ILineDecorationPtr ipLineDecoration2( CLSID_LineDecoration );

		IArrowMarkerSymbolPtr ipArrowMarkerSymbol2( CLSID_ArrowMarkerSymbol );
 		ipArrowMarkerSymbol2->put_Style( esriAMSPlain );
		ipArrowMarkerSymbol2->put_Color( ipColor );
		ipArrowMarkerSymbol2->put_Size( dbArrowSize );
		ISymbolPtr ipArrowSymbol2( ipArrowMarkerSymbol2 );

		ISimpleLineDecorationElementPtr ipLineDecElement2( CLSID_SimpleLineDecorationElement );

		ipLineDecElement2->put_Rotate( VARIANT_TRUE );
		ipLineDecElement2->put_MarkerSymbol( ipArrowMarkerSymbol2 );
		ipLineDecElement2->AddPosition( 1 );

		ipLineDecoration2->AddElement( ipLineDecElement2 );

		ipLineProperties2->putref_LineDecoration( ipLineDecoration2 );
		ICartographicLineSymbolPtr ipCartographicLineSymbol2( ipLineProperties2 );

		ipCartographicLineSymbol2->put_Color( ipColor );
		ipCartographicLineSymbol2->put_Width( 1.2 );
		ipCartographicLineSymbol2->put_Cap( esriLCSSquare );
		ipCartographicLineSymbol2->put_Join( esriLJSRound );

		(*it).second.piArrowSymbolNarrow = ipCartographicLineSymbol2;	

		/// Normal Symbol
		ISimpleLineSymbolPtr ipLineSymbol( CLSID_SimpleLineSymbol );
		ipLineSymbol->put_Color( ipColor );
		ipLineSymbol->put_Width( 2 );
		ipLineSymbol->put_Style( esriSLSSolid );

		(*it).second.piNormalSymbol = ipLineSymbol;
		
		/// Normal Symbol Narrow
		ISimpleLineSymbolPtr ipLineSymbol2( CLSID_SimpleLineSymbol );
		ipLineSymbol2->put_Color( ipColor );
		ipLineSymbol2->put_Width( 1.2 );
		ipLineSymbol2->put_Style( esriSLSSolid );

		(*it).second.piNormalSymbolNarrow = ipLineSymbol2;
	}

	for ( std::list<LQ>::const_iterator it2 = Data.begin(); it2 != Data.end(); it2++ ) {
		IConstructCurvePtr			pIConstructCurve(CLSID_Polyline); // TODO: 空間参照何与えればいいの？？
		CComVariant					vaOffsetHow( esriConstructOffsetMitered + esriConstructOffsetSimple ), vaOffsetHow2( esriConstructOffsetMitered );
		BOOL						bIsTorF = FALSE;

		piPoly = AheGetClone( (*it2).piGeom );

		ipTopo = piPoly;
		if ( ipTopo != NULL ) {
			ipTopo->put_IsKnownSimple( VARIANT_FALSE );
			ipTopo->Simplify();
		}

		lGCount = 1;
		ipGeomColl = piPoly;
		if ( ipGeomColl != NULL )
			ipGeomColl->get_GeometryCount( &lGCount );
		if ( lGCount >= 2 )
			// 自己交差してる･･･はず
			bIsTorF = TRUE;

		if ( ((*it2).DrawMode == _MULTISYMBOL_VICS || (*it2).DrawMode == _MULTISYMBOL_INTERNAVI) )
			dResult2 = -dResult;
		else
			dResult2 = dResult;

		if ( lDispDrawMode & _MULTISYMBOL_LANE && lDispDrawMode & _MULTISYMBOL_NOLRTURN && lDispDrawMode & _MULTISYMBOL_DIRGUIDE && lDispDrawMode & _MULTISYMBOL_UTURN ) {
			if ( (*it2).DrawMode == _MULTISYMBOL_LANE ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 2.2f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 2.2f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else if ( (*it2).DrawMode == _MULTISYMBOL_NOLRTURN ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.8f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.8f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else if ( (*it2).DrawMode == _MULTISYMBOL_DIRGUIDE ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			}
		} else if ( ( lDispDrawMode & _MULTISYMBOL_LANE && lDispDrawMode & _MULTISYMBOL_NOLRTURN && lDispDrawMode & _MULTISYMBOL_DIRGUIDE ) ||
					( lDispDrawMode & _MULTISYMBOL_LANE && lDispDrawMode & _MULTISYMBOL_NOLRTURN && lDispDrawMode & _MULTISYMBOL_UTURN ) ) {
			if ( (*it2).DrawMode == _MULTISYMBOL_LANE ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.8f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.8f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else if ( (*it2).DrawMode == _MULTISYMBOL_NOLRTURN ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			}
		} else if ( lDispDrawMode & _MULTISYMBOL_LANE && lDispDrawMode & _MULTISYMBOL_DIRGUIDE && lDispDrawMode & _MULTISYMBOL_UTURN ) {
			if ( lDispDrawMode == _MULTISYMBOL_LANE ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.8f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.8f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else if ( (*it2).DrawMode == _MULTISYMBOL_DIRGUIDE ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			}
		} else if ( lDispDrawMode & _MULTISYMBOL_NOLRTURN && lDispDrawMode & _MULTISYMBOL_DIRGUIDE && lDispDrawMode & _MULTISYMBOL_UTURN ) {
			if ( lDispDrawMode == _MULTISYMBOL_NOLRTURN ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.8f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.8f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else if ( (*it2).DrawMode == _MULTISYMBOL_DIRGUIDE ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			}
		} else if ( ( lDispDrawMode & _MULTISYMBOL_LANE && lDispDrawMode & _MULTISYMBOL_NOLRTURN ) ||
					( lDispDrawMode & _MULTISYMBOL_LANE && lDispDrawMode & _MULTISYMBOL_DIRGUIDE ) ||
					( lDispDrawMode & _MULTISYMBOL_LANE && lDispDrawMode & _MULTISYMBOL_UTURN ) ) {
			if ( (*it2).DrawMode == _MULTISYMBOL_LANE ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			}
		} else if ( ( lDispDrawMode & _MULTISYMBOL_NOLRTURN && lDispDrawMode & _MULTISYMBOL_DIRGUIDE ) ||
					( lDispDrawMode & _MULTISYMBOL_NOLRTURN && lDispDrawMode & _MULTISYMBOL_UTURN ) ) {
			if ( (*it2).DrawMode == _MULTISYMBOL_NOLRTURN ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			}
		} else if ( lDispDrawMode & _MULTISYMBOL_DIRGUIDE && lDispDrawMode & _MULTISYMBOL_UTURN ) {
			if ( lDispDrawMode == _MULTISYMBOL_DIRGUIDE ) {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2 * 1.4f, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			} else {
				if ( !bIsTorF ) {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow ) ) )
						piPoly = pIConstructCurve;
				} else {
					if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow2 ) ) )
						piPoly = pIConstructCurve;
				}
			}
		} else {
			if ( !bIsTorF ) {
				if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow ) ) )
					piPoly = pIConstructCurve;
			} else {
				if ( SUCCEEDED( pIConstructCurve->ConstructOffset( (IPolylinePtr)AheGetClone( (*it2).piGeom ), dResult2, &vaOffsetHow2 ) ) )
					piPoly = pIConstructCurve;
			}
		}

		if ( (*it2).bDrawAllow ) {
			if ( (*it2).bIsNarrow ) {
				mapColor[(*it2).Color].piArrowSymbolNarrow->put_Width( (*it2).Size );
				pISymbol = mapColor[(*it2).Color].piArrowSymbolNarrow;
			} else {
				mapColor[(*it2).Color].piArrowSymbol->put_Width( (*it2).Size );
				pISymbol = mapColor[(*it2).Color].piArrowSymbol;
			}
		} else {
			if ((*it2).bIsNarrow ) {
				mapColor[(*it2).Color].piNormalSymbolNarrow->put_Width( (*it2).Size );
				pISymbol = mapColor[(*it2).Color].piNormalSymbolNarrow;
			} else {
				mapColor[(*it2).Color].piNormalSymbol->put_Width( (*it2).Size );
				pISymbol = mapColor[(*it2).Color].piNormalSymbol;
			}
		}

		ipScreenDisplay->SetSymbol( pISymbol );
		ipScreenDisplay->DrawPolyline( piPoly );

		if ( (*it2).bIsDispID ) {
			IColorPtr ipColor( AheGetColor((*it2).ID_Color) );

			strIDString.Format( _T("%d"), (*it2).ID );
			ipTextSymbol->put_Color( ipColor );
			ipTextSymbol->put_Text( CComBSTR( strIDString ) );

			ipScreenDisplay->SetSymbol( (ISymbolPtr)ipTextSymbol );
			ipScreenDisplay->DrawText( piPoly, CComBSTR( strIDString ) );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// ポリゴン系描画
//
/////////////////////////////////////////////////////////////////////////////

void AheDrawPolygon( IDisplayPtr piScreenDisplay, IPolygonPtr ipPolygon, BOOL bDoStartStop )
{
	AheDrawPolygon( piScreenDisplay, ipPolygon, RGB( 0, 200, 50), 1, esriSLSSolid, bDoStartStop );
}

void AheDrawPolygon( IDisplayPtr piScreenDisplay, IPolygonPtr piPolygon, COLORREF rgbColor, int nWidth, esriSimpleLineStyle emStyle, BOOL bDoStartStop/* = TRUE*/, BOOL bXOR/* = FALSE*/ )
{
	ISimpleLineSymbolPtr ipColorSym;
	if( SUCCEEDED( ipColorSym.CreateInstance( CLSID_SimpleLineSymbol)))
	{
		IRgbColorPtr ipRgb;
		if( SUCCEEDED( ipRgb.CreateInstance( CLSID_RgbColor)))
		{
			ipRgb->put_Red( GetRValue( rgbColor));
			ipRgb->put_Green( GetGValue( rgbColor));
			ipRgb->put_Blue( GetBValue( rgbColor));
			IColorPtr ipColor( ipRgb);
			ipColorSym->put_Color( ipColor);
			ipColorSym->put_Width( nWidth);
			ipColorSym->put_Style( emStyle);
			ISymbolPtr ipSymbol( ipColorSym);

			if( bXOR == TRUE )
				ipSymbol->put_ROP2( esriROPNotXOrPen );

			// 描画
			DOUBLE dScale;
			IDisplayTransformationPtr ipTrans;
			piScreenDisplay->get_DisplayTransformation( &ipTrans );
			ipTrans->get_ReferenceScale( &dScale );
			ipTrans->put_ReferenceScale( 0.0 );
			if( bDoStartStop == TRUE )
			{
				UINT hDC;
				piScreenDisplay->get_hDC( &hDC);
				piScreenDisplay->StartDrawing( hDC, esriNoScreenCache);
			}
			piScreenDisplay->SetSymbol( ipSymbol);
			piScreenDisplay->DrawPolyline( piPolygon);
			if( bDoStartStop == TRUE )
				piScreenDisplay->FinishDrawing();
			ipTrans->put_ReferenceScale( dScale );
		}
	}
}

void AheDrawPolygon2( IDisplayPtr piScreenDisplay, IPolygonPtr ipPolygon, BOOL bDoStartStop )
{
	IGeometryPtr ipGeom( ipPolygon );
	ISimpleLineSymbolPtr ipLineSym( CLSID_SimpleLineSymbol );
	ISimpleFillSymbolPtr ipColorSym( CLSID_SimpleFillSymbol );
	IRgbColorPtr ipRgb( CLSID_RgbColor );

	ipRgb->put_Red( 0 );
	ipRgb->put_Blue( 50 );
	ipRgb->put_Green( 200 );
	IColorPtr ipColor( ipRgb );
	ipLineSym->put_Color( ipColor );
	ipLineSym->put_Width( 1 );
	ipLineSym->put_Style( esriSLSSolid );
	ILineSymbolPtr ipSymbol( ipLineSym );
	ipColorSym->put_Color( ipColor );
	ipColorSym->put_Style( esriSFSForwardDiagonal );
	ipColorSym->put_Outline( ipSymbol );
	ISymbolPtr ipFillSymbol( ipColorSym );
	// 描画
	DOUBLE dScale;
	IDisplayTransformationPtr ipTrans;
	piScreenDisplay->get_DisplayTransformation( &ipTrans );
	ipTrans->get_ReferenceScale( &dScale );
	ipTrans->put_ReferenceScale( 0.0 );
	if( bDoStartStop == TRUE )
	{
		UINT hDC;
		piScreenDisplay->get_hDC( &hDC );
		piScreenDisplay->StartDrawing( hDC, esriNoScreenCache );
	}
	piScreenDisplay->SetSymbol( ipFillSymbol );
	piScreenDisplay->DrawPolyline( ipPolygon );
	if( bDoStartStop == TRUE )
		piScreenDisplay->FinishDrawing();
	ipTrans->put_ReferenceScale( dScale );
}

void AheDrawPolygonXOR( IDisplayPtr piScreenDisplay, IPolygonPtr ipPolygon, BOOL bDoStartStop )
{
	IGeometryPtr			ipGeom( ipPolygon );
	ISimpleFillSymbolPtr	ipFillSymbol;
	ILineSymbolPtr			ipLineSymbol;
	ISymbolPtr				ipSymbol;
	ISymbolPtr				ipLSymbol;
	IRgbColorPtr			ipRgb( CLSID_RgbColor );

	ipFillSymbol.CreateInstance( CLSID_SimpleFillSymbol );
	ipFillSymbol->QueryInterface( &ipSymbol );

	// ドローモード
	ipSymbol->put_ROP2( esriROPXOrPen );

	// 色
	ipRgb->put_UseWindowsDithering( VARIANT_FALSE );
	ipRgb->put_Red( 45 );
	ipRgb->put_Blue( 45 );
	ipRgb->put_Green( 45 );
	IColorPtr ipColor;
	ipRgb->QueryInterface( &ipColor );
	// シンボルにセット
	ipFillSymbol->put_Color( ipColor );
	ipFillSymbol->put_Style( esriSFSSolid );

	// アウトライン
	ipFillSymbol->get_Outline( &ipLineSymbol );
	ipLineSymbol->QueryInterface( &ipLSymbol );
	ipLSymbol->put_ROP2( esriROPXOrPen );
	// 色
	ipRgb->put_Red( 145 );
	ipRgb->put_Blue( 145 );
	ipRgb->put_Green( 145 );
	ipRgb->QueryInterface( &ipColor );
	ipLineSymbol->put_Color( ipColor );
	// スタイル
	ipLineSymbol->put_Width( 2 );
	
	// セット
	ipFillSymbol->put_Outline( ipLineSymbol );
	
	// 描画
	DOUBLE dScale;
	IDisplayTransformationPtr ipTrans;
	piScreenDisplay->get_DisplayTransformation( &ipTrans );
	ipTrans->get_ReferenceScale( &dScale );
	ipTrans->put_ReferenceScale( 0.0 );
	if( bDoStartStop == TRUE )
	{
		UINT hDC = 0;
		piScreenDisplay->get_hDC( &hDC );
		piScreenDisplay->StartDrawing( hDC, esriNoScreenCache );
	}
	piScreenDisplay->SetSymbol( ipSymbol );
	piScreenDisplay->DrawPolygon( ipPolygon );
	if( bDoStartStop == TRUE )
		piScreenDisplay->FinishDrawing();
	ipTrans->put_ReferenceScale( dScale );
}

void AheDrawFeatureXOR( IDisplayPtr piScreenDisplay, IFeaturePtr ipFeature, double dbBufferSize, BOOL bDrawVertex, BOOL bDoStartStop )
{
	IPolygonPtr ipPolygon = NULL;

	AheFeature2Polygon( ipFeature, dbBufferSize, &ipPolygon );
	AheDrawPolygonXOR( piScreenDisplay, ipPolygon, bDoStartStop );

	if( bDrawVertex == TRUE )
		AheDrawVertex( piScreenDisplay, ipFeature, bDoStartStop );
}
/////////////////////////////////////////////////////////////////////////////
//
// ポイント系エレメント追加
//
/////////////////////////////////////////////////////////////////////////////
_ISetPtr AheAddPointElement( IGraphicsContainerPtr ipContainer, IGeometryPtr ipGeometry, COLORREF rgbColor, int nSize, esriSimpleMarkerStyle emStyle )
{
	if( ipContainer == NULL )
	{
		IApplicationPtr ipApp( CLSID_AppRef );
		if( ipApp != NULL )
		{
			IDocumentPtr ipDoc;
			ipApp->get_Document( &ipDoc );
			IMxDocumentPtr ipMxDoc( ipDoc );
			if( ipMxDoc != NULL )
			{
				IActiveViewPtr ipView;
				ipMxDoc->get_ActiveView( &ipView );
				if( ipView != NULL )
					ipView->get_GraphicsContainer( &ipContainer );
			}
		}
	}

	if( ipContainer == NULL || ipGeometry == NULL || rgbColor == NULL ) return NULL;

	IPointCollectionPtr ipPointCol( ipGeometry );
	if( ipPointCol == NULL )
	{
		ipPointCol.CreateInstance( CLSID_Multipoint );
		((IGeometryPtr)ipPointCol)->putref_SpatialReference( AheGetSpatialReference( ipGeometry ) ); // 空間参照付与
		ipPointCol->AddPoint( (IPointPtr)ipGeometry );
	}

	_ISetPtr ipSet( CLSID_Set );
	LONG lPointCount = -1;
	ipPointCol->get_PointCount( &lPointCount );
	for( LONG i = 0; i < lPointCount; i++ )
	{
		IPointPtr ipPoint;

		ipPointCol->get_Point( i, &ipPoint );
		IElementPtr ipElement( CLSID_MarkerElement );
		if( ipElement != NULL )
		{
			ipElement->put_Geometry( ipPoint );
			((IMarkerElementPtr)ipElement)->put_Symbol( (IMarkerSymbolPtr)AheGetPointSymbol( nSize, rgbColor, emStyle, FALSE, FALSE ) );
			ipContainer->AddElement( ipElement, 1 );

			ipSet->Add( ipElement );
		}
	}

	return ipSet;
}
/////////////////////////////////////////////////////////////////////////////
//
// ライン系エレメント追加
//
/////////////////////////////////////////////////////////////////////////////
IElementPtr AheAddLineElement( IGraphicsContainerPtr ipContainer, IGeometryPtr ipGeometry, COLORREF rgbColor, int nWidth, esriSimpleLineStyle emStyle )
{
	if( ipContainer == NULL )
	{
		IApplicationPtr ipApp( CLSID_AppRef );
		if( ipApp != NULL )
		{
			IDocumentPtr ipDoc;
			ipApp->get_Document( &ipDoc );
			IMxDocumentPtr ipMxDoc( ipDoc );
			if( ipMxDoc != NULL )
			{
				IActiveViewPtr ipView;
				ipMxDoc->get_ActiveView( &ipView );
				if( ipView != NULL )
					ipView->get_GraphicsContainer( &ipContainer );
			}
		}
	}

	if( ipContainer == NULL || ipGeometry == NULL || rgbColor == NULL ) return NULL;

	IElementPtr ipElement( CLSID_LineElement );
	if( ipElement != NULL )
	{
		ipElement->put_Geometry( ipGeometry );
		((ILineElementPtr)ipElement)->put_Symbol( (ILineSymbolPtr)AheGetLineSymbol( nWidth, rgbColor, emStyle, FALSE ) );
		ipContainer->AddElement( ipElement, 1 );

		return ipElement;
	}

	return NULL;
}
/////////////////////////////////////////////////////////////////////////////
//
// ポリゴン系エレメント追加
//
/////////////////////////////////////////////////////////////////////////////
IElementPtr AheAddPolygonElement( IGraphicsContainerPtr ipContainer, IGeometryPtr ipGeometry, COLORREF rgbColor, esriSimpleFillStyle emStyle )
{
	if( ipContainer == NULL )
	{
		IApplicationPtr ipApp( CLSID_AppRef );
		if( ipApp != NULL )
		{
			IDocumentPtr ipDoc;
			ipApp->get_Document( &ipDoc );
			IMxDocumentPtr ipMxDoc( ipDoc );
			if( ipMxDoc != NULL )
			{
				IActiveViewPtr ipView;
				ipMxDoc->get_ActiveView( &ipView );
				if( ipView != NULL )
					ipView->get_GraphicsContainer( &ipContainer );
			}
		}
	}

	if( ipContainer == NULL || ipGeometry == NULL || rgbColor == NULL ) return NULL;

	IElementPtr ipElement( CLSID_PolygonElement );
	if( ipElement != NULL )
	{
		ipElement->put_Geometry( ipGeometry );
		((IFillShapeElementPtr)ipElement)->put_Symbol( (IFillSymbolPtr)AheGetPolygonSymbol( rgbColor, emStyle, FALSE ) );
		ipContainer->AddElement( ipElement, 1 );

		return ipElement;
	}

	return NULL;
}
