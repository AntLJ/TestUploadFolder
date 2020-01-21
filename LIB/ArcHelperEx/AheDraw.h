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
* @file AheDraw.h
*
* @brief Arc Helper 描画系グローバル関数定義ファイル
*/
#if	!defined( __AHEDRAW_H__)
#define __AHEDRAW_H__	//!< 重複インクルード防止

/////////////////////////////////////////////////////////////////////////////
//
// 色及びシンボルに関する関数
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief IColorPtr を取得する
 *
 * @param red				[in]			色(赤）
 * @param green				[in]			色(緑）
 * @param blue				[in]			色(青）
 *
 * @return IColorPtr
 */
IColorPtr AheGetColor( LONG red, LONG green, LONG blue );

/**
 * @brief IColorPtr を取得する
 *
 * @param color				[in]			RGB マクロで作成した COLORREF
 *
 * @return IColorPtr
 */
IColorPtr AheGetColor( COLORREF color );

/**
 * @brief 指定した形状を持つポイントシンボルを作成する
 *
 * @param size,				[in]			ポイントシンボルのサイズ
 * @param red				[in]			ポイントシンボルの色(赤）
 * @param green				[in]			ポイントシンボルの色(緑）
 * @param blue				[in]			ポイントシンボルの色(青）
 * @param style				[in]			ポイントのスタイル
 * @param bDrawXOR			[in]			XOR で描画するかどうか
 * @param bOutLine			[in]			アウトラインを描画するかどうか
 *	
 * @return ISymbolPtr
 */
ISymbolPtr AheGetPointSymbol( double size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bDrawXOR, BOOL bOutLine);

/**
 * @brief 指定した形状を持つポイントシンボルを作成する
 *
 * @param size,				[in]			ポイントシンボルのサイズ
 * @param color				[in]			RGB マクロで作成した COLORREF
 * @param style				[in]			ポイントのスタイル
 * @param bDrawXOR			[in]			XOR で描画するかどうか
 * @param bOutLine			[in]			アウトラインを描画するかどうか
 *	
 * @return ISymbolPtr
 */
ISymbolPtr AheGetPointSymbol( double size, COLORREF color, esriSimpleMarkerStyle style, BOOL bDrawXOR, BOOL bOutLine );

/**
 * @brief 指定した形状を持つポイントシンボルを作成する
 *
 * @param width,			[in]			ラインシンボルの幅
 * @param red				[in]			ラインシンボルの色(赤）
 * @param green				[in]			ラインシンボルの色(緑）
 * @param blue				[in]			ラインシンボルの色(青）
 * @param style				[in]			ラインのスタイル
 * @param bDrawXOR			[in]			XOR で描画するかどうか
 *	
 * @return ISymbolPtr
 */
ISymbolPtr AheGetLineSymbol( double width, LONG red, LONG green, LONG blue, esriSimpleLineStyle style, BOOL bDrawXOR );

/**
 * @brief 指定した形状を持つポイントシンボルを作成する
 *
 * @param width,			[in]			ラインシンボルの幅
 * @param color				[in]			RGB マクロで作成した COLORREF
 * @param style				[in]			ラインのスタイル
 * @param bDrawXOR			[in]			XOR で描画するかどうか
 *	
 * @return ISymbolPtr
 */
ISymbolPtr AheGetLineSymbol( double width, COLORREF color, esriSimpleLineStyle style, BOOL bDrawXOR );

/**
 * @brief ポリゴンシンボルを作成する
 *
 * @param red				[in]			ポリゴンシンボルの色(赤）
 * @param green				[in]			ポリゴンシンボルの色(緑）
 * @param blue				[in]			ポリゴンシンボルの色(青）
 * @param style				[in]			ポリゴンのスタイル
 * @param bDrawXOR			[in]			XOR で描画するかどうか
 *
 * @return ISymbolPtr
 */
ISymbolPtr AheGetPolygonSymbol( LONG red, LONG green, LONG blue, esriSimpleFillStyle style, BOOL bDrawXOR );

/**
 * @brief ポリゴンシンボルを作成する
 *
 * @param color				[in]			RGB マクロで作成した COLORREF
 * @param style				[in]			ポリゴンのスタイル
 * @param bDrawXOR			[in]			XOR で描画するかどうか
 *
 * @return ISymbolPtr
 */
ISymbolPtr AheGetPolygonSymbol( COLORREF color, esriSimpleFillStyle style, BOOL bDrawXOR );
/////////////////////////////////////////////////////////////////////////////
//
// フィーチャ描画
//
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief 指定した形状を持つフィーチャを描画する
 *
 * @param pIDisplay			[in]			IDisplay のインスタンス
 * @param ipGeom			[in]			描画したい形状
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 */
void AheDrawFeature( IDisplayPtr pIDisplay, IGeometryPtr ipGeom, BOOL bDoStartStop = TRUE );

/**
 * @brief ポイント、ライン、ポリゴンの点列を表示
 *
 * @param pIDisplay	[in]			IDisplay のインスタンス
 * @param ipFeature			[in]			点列を表示する対象フィーチャ
 * @param bDrawXOR			[in]			XOR で描画するかどうか
 * @param size				[in]			サイズ
 * @param red				[in]			点の色(赤）
 * @param green				[in]			点の色(緑）
 * @param blue				[in]			点の色(青）
 * @param style				[in]			点のスタイル
 * @param bOutLine			[in]			アウトラインを描画するかどうか
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 */
void AheDrawVertex( IDisplayPtr pIDisplay, IFeaturePtr ipFeature, BOOL bXOR = FALSE, LONG size = 3, LONG red = 200, LONG green = 200, LONG blue = 200, esriSimpleMarkerStyle style = esriSMSSquare, BOOL bOutLine = FALSE, BOOL bDoStartStop = TRUE );

/**
 * @brief ジオメトリの頂点を表示
 *
 * @param pIDisplay	[in]			IDisplay のインスタンス
 * @param ipFeatGeom		[in]			点列を表示する対象ジオメトリ
 * @param bDrawXOR			[in]			XOR で描画するかどうか
 * @param size				[in]			サイズ
 * @param red				[in]			点の色(赤）
 * @param green				[in]			点の色(緑）
 * @param blue				[in]			点の色(青）
 * @param style				[in]			点のスタイル
 * @param bOutLine			[in]			アウトラインを描画するかどうか
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 */
void AheDrawGeomVertex( IDisplayPtr pIDisplay, IGeometryPtr ipFeatGeom, BOOL bXOR, LONG size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bOutLine, BOOL bDoStartStop = TRUE );

/**
 * @brief ポイントを表示
 *
 * @param pIDisplay	[in]			IDisplay のインスタンス
 * @param ipPoint			[in]			点列を表示する対象ジオメトリ
 * @param bDrawXOR			[in]			XOR で描画するかどうか
 * @param size				[in]			サイズ
 * @param red				[in]			点の色(赤）
 * @param green				[in]			点の色(緑）
 * @param blue				[in]			点の色(青）
 * @param style				[in]			点のスタイル
 * @param bOutLine			[in]			アウトラインを描画するかどうか
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 */
void AheDrawPoint( IDisplayPtr ipScreenDisplay, IPointPtr ipPoint, BOOL bXOR = FALSE, LONG size = 3, LONG red = 200, LONG green = 200, LONG blue = 200, esriSimpleMarkerStyle style = esriSMSSquare, BOOL bOutLine = FALSE, BOOL bDoStartStop = TRUE );	

/**
 * @brief ポイントコレクションを表示
 *
 * @param pIDisplay	[in]			IDisplay のインスタンス
 * @param ipPointCol		[in]			表示するポイントコレクション
 * @param bDrawXOR			[in]			XOR で描画するかどうか
 * @param size				[in]			サイズ
 * @param red				[in]			点の色(赤）
 * @param green				[in]			点の色(緑）
 * @param blue				[in]			点の色(青）
 * @param style				[in]			点のスタイル
 * @param bOutLine			[in]			アウトラインを描画するかどうか
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 */
void AheDrawPoints( IDisplayPtr pIDisplay, IPointCollectionPtr ipPointCol, BOOL bXOR, LONG size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bOutLine, BOOL bDoStartStop = TRUE );

void AheDrawMultipoint( IDisplayPtr pIDisplay, IGeometryPtr ipGeom, BOOL bXOR, LONG size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bOutLine, BOOL bDoStartStop = TRUE );

/////////////////////////////////////////////////////////////////////////////
//
// ライン系描画
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief ポリラインを表示
 *
 * @param pIDisplay			[in]			IDisplay のインスタンス
 * @param ipPolyline		[in]			表示するポリライン
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 */
void AheDrawPolyline( IDisplayPtr pIDisplay, IPolylinePtr ipPolyline, BOOL bDoStartStop = TRUE );


/**
 * @brief ポリラインを表示
 *
 * @param pIDisplay			[in]			IDisplay のインスタンス
 * @param ipPolyline		[in]			表示するポリライン
 * @param rgbColor			[in]			表示色
 * @param nWidth			[in]			幅
 * @param emStyle			[in]			表示スタイル
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 * @param bXOR				[in,optional]	XOR で描画するかどうか（デフォルト FALSE）
 */
void AheDrawPolyline( IDisplayPtr pIDisplay, IPolylinePtr piPolyline, COLORREF rgbColor, int nWidth, esriSimpleLineStyle enStyle, BOOL bDoStartStop = TRUE, BOOL bXOR = FALSE );

/**
 * @brief 矢印付きポリラインを表示
 *
 * @param ipScreenDisplay	[in]			IDisplayPtr
 * @param ipPolyline		[in]			表示するポリライン
 * @param rgbColor			[in]			表示色
 * @param bDoStartStop		[in]			StartDrawing, StopDrawing するかどうか
 * @param bDrawArrow		[in,optional]	矢印を描画するかどうか（デフォルト：TRUE）
 * @param bOffset			[in,optional]	オフセット表示するかどうか（デフォルト：FALSE）
 * @param bOffsetReverse	[in,optional]	オフセットを逆に取るかどうか（デフォルト：FALSE）
 */
void AheDrawArrowline( IDisplayPtr ipScreenDisplay, IPolylinePtr ipPolyline, COLORREF rgbColor, BOOL bDoStartStop, BOOL bDrawArrow = TRUE, BOOL bOffset = FALSE, BOOL bOffsetReverse = FALSE );
void AheDrawArrowline2( IDisplayPtr ipScreenDisplay, std::list<LQ> Data, std::map<COLORREF, SYMBOL> mapColor, LONG lDispDrawMode, DOUBLE dbArrowSize);
/////////////////////////////////////////////////////////////////////////////
//
// ポリゴン系描画
//
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief ポリゴンを表示
 *
 * @param pIDisplay			[in]			IDisplay のインスタンス
 * @param ipPolygon			[in]			表示するポリゴン
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 */
void AheDrawPolygon( IDisplayPtr pIDisplay, IPolygonPtr ipPolygon, BOOL bDoStartStop = TRUE );

/**
 * @brief ポリゴンを表示
 *
 * @param pIDisplay			[in]			IDisplay のインスタンス
 * @param ipPolygon			[in]			表示するポリゴン
 * @param rgbColor			[in]			表示色
 * @param nWidth			[in]			幅
 * @param emStyle			[in]			表示スタイル
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 * @param bXOR				[in,optional]	XOR で描画するかどうか（デフォルト FALSE）
 */
void AheDrawPolygon( IDisplayPtr pIDisplay, IPolygonPtr piPolygon, COLORREF rgbColor, int nWidth, esriSimpleLineStyle enStyle, BOOL bDoStartStop = TRUE, BOOL bXOR = FALSE );

/**
 * @brief ポリゴンを表示２
 *
 * @param pIDisplay			[in]			IDisplay のインスタンス
 * @param ipPolygon			[in]			表示するポリゴン
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 */
void AheDrawPolygon2( IDisplayPtr pIDisplay, IPolygonPtr ipPolygon, BOOL bDoStartStop = TRUE );

/**
 * @brief ポリゴンを XOR で表示
 *
 * @param pIDisplay			[in]			IDisplay のインスタンス
 * @param ipPolygon			[in]			表示するポリゴン
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 */
void AheDrawPolygonXOR( IDisplayPtr pIDisplay, IPolygonPtr ipPolygon, BOOL bDoStartStop = TRUE );

/**
 * @brief フィーチャを XOR で描画する
 *
 * @param pIDisplay			[in]			IDisplay のインスタンス
 * @param ipFeature			[in]			表示するフィーチャ
 * @param dbBufferSize		[in]			バッファするサイズ
 * @param bDrawVertex		[in]			頂点を描画するかどうか
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
 */
void AheDrawFeatureXOR( IDisplayPtr pIDisplay, IFeaturePtr ipFeature, double dbBufferSize, BOOL bDrawVertex, BOOL bDoStartStop = TRUE );
/////////////////////////////////////////////////////////////////////////////
//
// ライン系エレメント追加
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief ポリラインのエレメント追加
 *
 * @param ipContainer		[in]			IGraphicsContainerPtr
 * @param ipGeometry		[in]			IGeometryPtr
 * @param rgbColor			[in]			RGB マクロで作成した COLORREF
 * @param nWidth			[in]			幅
 * @param emStyle			[in]			スタイル
 *
 * @return IElementPtr
 */
IElementPtr AheAddLineElement( IGraphicsContainerPtr ipContainer, IGeometryPtr ipGeometry, COLORREF rgbColor, int nWidth, esriSimpleLineStyle emStyle );
/////////////////////////////////////////////////////////////////////////////
//
// ポイント系エレメント追加
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief ポイントのエレメント追加
 *
 * @param ipContainer		[in]			IGraphicsContainerPtr
 * @param ipGeometry		[in]			IGeometryPtr
 * @param rgbColor			[in]			RGB マクロで作成した COLORREF
 * @param nSize				[in]			サイズ
 * @param emStyle			[in]			スタイル
 *
 * @return IElementPtr
 */
_ISetPtr AheAddPointElement( IGraphicsContainerPtr ipContainer, IGeometryPtr ipGeometry, COLORREF rgbColor, int nSize, esriSimpleMarkerStyle emStyle );
/////////////////////////////////////////////////////////////////////////////
//
// ポリゴン系エレメント追加
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief ポリゴンのエレメント追加
 *
 * @param ipContainer		[in]			IGraphicsContainerPtr
 * @param ipGeometry		[in]			IGeometryPtr
 * @param rgbColor			[in]			RGB マクロで作成した COLORREF
 * @param emStyle			[in]			スタイル
 *
 * @return IElementPtr
 */
IElementPtr AheAddPolygonElement( IGraphicsContainerPtr ipContainer, IGeometryPtr ipGeometry, COLORREF rgbColor, esriSimpleFillStyle emStyle );

#endif	//__AHEDRAW_H__
