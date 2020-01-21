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
 * @file SymbolFunctions.cpp
 * @brief 色・シンボル系グローバルヘルパー関数実装
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "AheSymbolFunctions.h"
#include "type_convert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 色を作成する
IColorPtr AheCreateColor( COLORREF color )
{
	IColorPtr ipColor( CLSID_RgbColor );
	ipColor->put_RGB( (OLE_COLOR)color );
	return ipColor;
}

// ポイントシンボルを作成する
ISymbolPtr AheCreatePointSymbol( IColor* ipColor, double size/* = 1.0*/, esriSimpleMarkerStyle style/* = esriSMSSolid*/, bool bDrawXOR/* = false*/, bool bOutLine/* = false*/ )
{
	ISimpleMarkerSymbolPtr ipSym;
	if( ipColor != NULL && size > 0.0 )
	{
		ipSym.CreateInstance( CLSID_SimpleMarkerSymbol );

		if( ipSym != NULL )
		{
			ipSym->put_Color( ipColor );
			ipSym->put_Size( size );
			ipSym->put_Style( style );

			// アウトライン
			if( bOutLine ) 
			{
				COLORREF rgb = 0;
				ipColor->get_RGB( (OLE_COLOR*)&rgb );

				ipSym->put_Outline( bool2VB(true) );
				ipSym->put_OutlineSize( 1 );
				ipSym->put_OutlineColor( AheCreateColor( RGB( 255 - GetRValue(rgb), 255 - GetGValue(rgb), 255 - GetBValue(rgb) ) ) );
			}

			// XOR
			if( bDrawXOR ) {
				((ISymbolPtr)ipSym)->put_ROP2( esriROPXOrPen );
			}
		}
	}
	else
		_ASSERTE( ipColor != NULL && size > 0.0 );

	return (ISymbolPtr)ipSym;
}

// シンプルラインシンボルを作成する
ISymbolPtr AheCreateLineSymbol( IColor* ipColor, double width/* = 1.0*/, esriSimpleLineStyle style/* = esriSLSSolid*/, bool bDrawXOR/* = false*/ )
{
	ISimpleLineSymbolPtr ipSym;
	if( ipColor != NULL && width > 0.0 )
	{
		ipSym.CreateInstance( CLSID_SimpleLineSymbol );

		if( ipSym != NULL )
		{
			ipSym->put_Color( ipColor );
			ipSym->put_Width( width );
			ipSym->put_Style( style );

			// XOR
			if( bDrawXOR )
				((ISymbolPtr)ipSym)->put_ROP2( esriROPNotXOrPen );
		}
	}
	else
		_ASSERTE( ipColor != NULL && width > 0.0 );

	return (ISymbolPtr)ipSym;
}

// シンプルポリゴンシンボルを作成する
ISymbolPtr AheCreatePolygonSymbol( IColor* ipColor, esriSimpleFillStyle style, bool bDrawXOR )
{
	ISimpleFillSymbolPtr	ipFillSymbol( CLSID_SimpleFillSymbol );
	ISymbolPtr				ipSymbol;
	ISymbolPtr				ipLSymbol;

	if( ipFillSymbol != NULL )
	{
		ipFillSymbol->put_Color( ipColor );
		ipFillSymbol->put_Style( style );
		
		if( bDrawXOR )
			((ISymbolPtr)ipFillSymbol)->put_ROP2( esriROPNotXOrPen );

		// アウトライン
		COLORREF rgb = 0;
		ILineSymbolPtr ipLineSymbol;

		ipColor->get_RGB( (OLE_COLOR*)&rgb );
		ipFillSymbol->get_Outline( &ipLineSymbol );
		ipLineSymbol->put_Color( AheCreateColor( RGB( 255 - GetRValue(rgb), 255 - GetGValue(rgb), 255 - GetBValue(rgb) ) ) );
		ipLineSymbol->put_Width( 1 );

		if( bDrawXOR == TRUE )
			((ISymbolPtr)ipLineSymbol)->put_ROP2( esriROPNotXOrPen );

		ipFillSymbol->put_Outline( ipLineSymbol );
	}

	return ipFillSymbol;
}
