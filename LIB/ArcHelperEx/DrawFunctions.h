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
#ifndef _DRAWFUNCTIONS_H_
#define _DRAWFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief <b>ポリゴンを描画する</b>\n
 * 通常基本スケールが設定されている場合にそのスケールでの表示に対して相対的に拡大・縮小されますが、
 * bIgnoreRefScaleをtrueに設定すると、基本スケールを一時的に無効化して描画するため、そのままの
 * 描画がなされます（デフォルトはtrueです）。\n
 * また、描画をする際には必ずStartDrawing()、FinishDrawing()でシンボル設定及び描画を囲まなければ
 * なりませんが、IActiveViewEvents::AfterDraw()内では自動的に囲まれた中になります。その場合には、
 * bDoStartFinishをfalseにして下さい（デフォルト：true）。\n
 * bDoRecordingをtrueにすると、メッセージボックスが消えた後やアプリケーションを切り替えた後などに
 * 描画が消えてしまうのを防ぐことができます。これにより、IActiveViewEventsをインプリメントしなくても
 * ITool::Refresh()で再描画すればよいということになります（デフォルト：false）。
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipPolygon			[in]			描画するポリゴン
 * @param dRefScale			[in,optional]	基本スケール（デフォルト：0.0）。bIgnoreRefScaleがfalseの時にこの値が0以上だとこの値が基本スケールとして使用される
 * @param bIgnoreRefScale	[in,optional]	基本スケールを無視する場合はtrue（デフォルト：true）
 * @param bDoStartFinish	[in,optional]	StartDrawing()、FinishDrawing()をこの関数内でして欲しい場合はtrue（デフォルト：true）
 * @param bDoRecording		[in,optional]	描画キャッシュに記録するならtrue（デフォルト：false）
 */
void AheDrawPolygon( IDisplay* ipDisplay, ISymbol* ipSymbol, IPolygon* ipPolygon, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>矩形を描画する</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipEnvelope		[in]			描画する矩形
 * @param dRefScale			[in,optional]	基本スケール（デフォルト：0.0）。bIgnoreRefScaleがfalseの時にこの値が0以上だとこの値が基本スケールとして使用される
 * @param bIgnoreRefScale	[in,optional]	基本スケールを無視する場合はtrue（デフォルト：true）
 * @param bDoStartFinish	[in,optional]	StartDrawing()、FinishDrawing()をこの関数内でして欲しい場合はtrue（デフォルト：true）
 * @param bDoRecording		[in,optional]	描画キャッシュに記録するならtrue（デフォルト：false）
 */
void AheDrawRectangle( IDisplay* ipDisplay, ISymbol* ipSymbol, IEnvelope* ipEnvelope, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>ポリラインを描画する</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipPolyline		[in]			描画するライン
 * @param dRefScale			[in,optional]	基本スケール（デフォルト：0.0）。bIgnoreRefScaleがfalseの時にこの値が0以上だとこの値が基本スケールとして使用される
 * @param bIgnoreRefScale	[in,optional]	基本スケールを無視する場合はtrue（デフォルト：true）
 * @param bDoStartFinish	[in,optional]	StartDrawing()、FinishDrawing()をこの関数内でして欲しい場合はtrue（デフォルト：true）
 * @param bDoRecording		[in,optional]	描画キャッシュに記録するならtrue（デフォルト：false）
 */
void AheDrawPolyline( IDisplay* ipDisplay, ISymbol* ipSymbol, IPolyline* ipPolyline, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>ポイントを描画する</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipPoint			[in]			描画する点
 * @param dRefScale			[in,optional]	基本スケール（デフォルト：0.0）。bIgnoreRefScaleがfalseの時にこの値が0以上だとこの値が基本スケールとして使用される
 * @param bIgnoreRefScale	[in,optional]	基本スケールを無視する場合はtrue（デフォルト：true）
 * @param bDoStartFinish	[in,optional]	StartDrawing()、FinishDrawing()をこの関数内でして欲しい場合はtrue（デフォルト：true）
 * @param bDoRecording		[in,optional]	描画キャッシュに記録するならtrue（デフォルト：false）
 */
void AheDrawPoint( IDisplay* ipDisplay, ISymbol* ipSymbol, IPoint* ipPoint, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>構成点を描画する</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipGeometry		[in]			描画する構成点のジオメトリ
 * @param dRefScale			[in,optional]	基本スケール（デフォルト：0.0）。bIgnoreRefScaleがfalseの時にこの値が0以上だとこの値が基本スケールとして使用される
 * @param bIgnoreRefScale	[in,optional]	基本スケールを無視する場合はtrue（デフォルト：true）
 * @param bDoStartFinish	[in,optional]	StartDrawing()、FinishDrawing()をこの関数内でして欲しい場合はtrue（デフォルト：true）
 * @param bDoRecording		[in,optional]	描画キャッシュに記録するならtrue（デフォルト：false）
 */
void AheDrawVertex( IDisplay* ipDisplay, ISymbol* ipSymbol, IGeometry* ipGeometry, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>マルチポイントを描画する</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipPoints			[in]			描画する点
 * @param dRefScale			[in,optional]	基本スケール（デフォルト：0.0）。bIgnoreRefScaleがfalseの時にこの値が0以上だとこの値が基本スケールとして使用される
 * @param bIgnoreRefScale	[in,optional]	基本スケールを無視する場合はtrue（デフォルト：true）
 * @param bDoStartFinish	[in,optional]	StartDrawing()、FinishDrawing()をこの関数内でして欲しい場合はtrue（デフォルト：true）
 * @param bDoRecording		[in,optional]	描画キャッシュに記録するならtrue（デフォルト：false）
 */
void AheDrawMultipoint( IDisplay* ipDisplay, ISymbol* ipSymbol, IMultipoint* ipPoints, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>文字を描画する</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipGeom			[in]			ジオメトリ
 * @param lpcszText			[in]			描画するテキスト
 * @param dRefScale			[in,optional]	基本スケール（デフォルト：0.0）。bIgnoreRefScaleがfalseの時にこの値が0以上だとこの値が基本スケールとして使用される
 * @param bIgnoreRefScale	[in,optional]	基本スケールを無視する場合はtrue（デフォルト：true）
 * @param bDoStartFinish	[in,optional]	StartDrawing()、FinishDrawing()をこの関数内でして欲しい場合はtrue（デフォルト：true）
 * @param bDoRecording		[in,optional]	描画キャッシュに記録するならtrue（デフォルト：false）
 */
void AheDrawText( IDisplay* ipDisplay, ISymbol* ipSymbol, IGeometry* ipGeom, LPCTSTR lpcszText, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

#endif	//_DRAWFUNCTIONS_H_
