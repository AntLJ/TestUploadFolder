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
#ifndef _SYMBOLFUNCTIONS_H_
#define _SYMBOLFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief <b>色を作成する</b>\n
 * RGBから色を作成したい場合は、RGB()マクロ
 * を使用してCOLORREFに変換して渡してください。
 * @param color [in]	RGB
 * @return IColorPtr
 */
IColorPtr AheCreateColor( COLORREF color );

/**
 * @brief <b>ポイントシンボルを作成する</b>\n
 * @param ipColor	[in]			IColor*
 * @param size		[in,optional]	ポイントシンボル幅（デフォルト：1.0）
 * @param style		[in,optional]	シンボルスタイル（デフォルト：esriSMSCircle）
 * @param bDrawXOR	[in,optional]	XORで描画するかどうか（デフォルト：false）
 * @param bOutline	[in,optional]	アウトラインを描画するかどうか（デフォルト：false）
 * @return ISymbolPtr
 */
ISymbolPtr AheCreatePointSymbol( IColor* ipColor, double size = 1.0, esriSimpleMarkerStyle style = esriSMSCircle, bool bDrawXOR = false, bool bOutline = false );

/**
 * @brief <b>シンプルラインシンボルを作成する</b>\n
 * @param ipColor	[in]			IColor*
 * @param width		[in,optional]	シンボル幅（デフォルト：1.0）
 * @param style		[in,optional]	シンボルスタイル（デフォルト：esriSLSSolid）
 * @param bDrawXOR	[in,optional]	XORで描画するかどうか（デフォルト：false）
 * @return ISymbolPtr
 */
ISymbolPtr AheCreateLineSymbol( IColor* ipColor, double width = 1.0, esriSimpleLineStyle style = esriSLSSolid, bool bDrawXOR = false );

/**
 * @brief <b>シンプルポリゴンシンボルを作成する</b>\n
 * @param ipColor	[in]			IColor*
 * @param style		[in,optional]	シンボルスタイル（デフォルト：esriSLSSolid）
 * @param bDrawXOR	[in,optional]	XORで描画するかどうか（デフォルト：false）
 * @return ISymbolPtr
 */
ISymbolPtr AheCreatePolygonSymbol( IColor* ipColor, esriSimpleFillStyle style = esriSFSSolid, bool bDrawXOR = false );

#endif // _SYMBOLFUNCTIONS_H_
