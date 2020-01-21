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
 * @file AheZooMoveFunctions.h
 * @brief ArcObjectsグローバルヘルパー関数定義
 *
 * このファイルには拡大縮小、スクロールなどに関する
 * グローバル関数が定義されています。
 * 
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @date 2007/01/26
 * $Id$
 */
#ifndef SINDYLIB_AHE_ZOOMOVEFUNCTIONS_H_
#define SINDYLIB_AHE_ZOOMOVEFUNCTIONS_H_

/**
* @brief ZooMoveToolのキー移動拡大縮小処理
*
* @note ZooMoveToolで実装したキーボードでの処理部分を抽出
*
* @param ipActView [in] ビュー
* @param lKeyCode [in] 仮想キーコード
* @param lShift [in] 修飾キー？ステータス
*
* @retval true handled
* @retval false not handled
*/
bool AheKeyZooMove( IActiveView* ipActView, long lKeyCode, long lShift );

/**
* @brief カーソル位置に画面の中心を移動する
* @param ipActiveView [in] ビュー
*/
bool AheMoveCenterToMousePointer( IActiveView* ipActiveView, bool bSuppressEvents = true );

/**
* @brief カーソルをビューのセンターに移動
* @param ipActiveView [in] ビュー
*/
bool AheMoveCursorToCenter( IActiveView* ipActiveView );

/**
* @brief 指定ステップ数分の移動
*
* @param piActiveView [in] マップのビュー
* @param nXStep [in] 移動単位
* @param nYStep [in] 移動単位
*/
bool AheScrollMap( IActiveView* ipActView, int nXStep, int nYStep );

/// MapFANのZM値に則した拡大率での拡大・縮小処理
bool AheZoom( IActiveView* ipActiveView, bool bUpScale );

/// MapFANのZM値に則した拡大率での拡大処理
bool AheZoomIn( IActiveView* ipActiveView );

/// MapFANのZM値に則した拡大率での縮小処理
bool AheZoomOut( IActiveView* ipActiveView );

// 再描画モードを取得
bool AheIsRefreshMode();

/**
* @brief ちらつき抑制再描画
*
* @param piActiveView [in] マップのビュー
* @param ipEnv [in] 再描画矩形
*/
void AheRefresh(IActiveView* ipActiveView, IEnvelope* ipEnv=NULL);

#endif // ifndef SINDYLIB_AHE_ZOOMOVEFUNCTIONS_H_
