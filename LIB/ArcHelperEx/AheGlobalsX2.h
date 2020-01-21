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
* @file AheGlobalsX2.h
*
* @brief Arc Helper グローバル2関数定義ファイル
*/
#if	!defined( __AHEGLOBALSX2_H__)
#define __AHEGLOBALSX2_H__	//!< 重複インクルード防止

/**
 * @brief *.lyr ファイルから ILayer を取得する
 *
 * @param lpszLayerFile		[in]			*.lyr ファイルのパス
 * @param ipRetLayer		[out]			取得された ILayer オブジェクト
 *
 * @retval TURE ILayer を取得できた
 * @retval FALSE ILayer を取得できなかった
 */
BOOL AheGetLayerFromFile( LPCTSTR lpszLayerFile, ILayer** ipRetLayer );

/**
 * @brief レイヤファイルからレイヤを取得する
 *
 *   読み込んだレイヤは、接続されていないかもしれないので注意すること。
 *
 * @param lpcszFilePath				[in]			レイヤファイルのパス
 *
 * @retval ILayer
 */
ILayerPtr AheGetLayerFromLyrFile( LPCTSTR lpcszFilePath );

#endif // __AHEGLOBALSX2_H__
