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

// KKDLayerLoad.h
//
//////////////////////////////////////////////////////////////////////

/**
 * @file KKDLayerLoad.h
 * @brief KKDデータレイヤ単位読み込みクラス定義ヘッダファイル
 */
#ifndef __KKD_LAYER_LOAD_H__
#define __KKD_LAYER_LOAD_H__

#include <DRMALayerLoad.h>
#include "KKDLoadProtocol.h"
#include "KKDLayer.h"

/**
 * @brief KKDレイヤ単位読み込みテンプレートクラス
 */
template <typename TLayer>
class KKDLayerLoadTemplate : public DRMALayerLoadTemplate<TLayer, KKDLoadProtocol>
{
};

/**
 * @brief KKD管理レイヤ用読み込みテンプレートクラス
 */
template <typename TLayer>
class KKDAdminLayerLoadTemplate : public DRMAAdminLayerLoadTemplate<TLayer, KKDLoadProtocol>
{
};

// KKDレイヤ読み込みクラス定義

typedef KKDAdminLayerLoadTemplate<KKDAdminLayer> KKDAdminLoad;
typedef KKDLayerLoadTemplate<KKDRegulationRoadNodeLayer> KKDRegulationRoadNodeLoad;
typedef KKDLayerLoadTemplate<KKDRegulationRoadLinkLayer> KKDRegulationRoadLinkLoad;
typedef KKDLayerLoadTemplate<KKDLaneRegulationRoadNodeLayer> KKDLaneRegulationRoadNodeLoad;
typedef KKDLayerLoadTemplate<KKDLaneRegulationRoadLinkLayer> KKDLaneRegulationRoadLinkLoad;
typedef KKDLayerLoadTemplate<KKDLaneRegulationIntersectionNodeLayer> KKDLaneRegulationIntersectionNodeLoad;

/**
 * @example example2.cpp
 *
 * Dドライブのルートディレクトリ直下に存在する「kkd」ディレクトリより533945のKKD交通規制道路ノードデータを読み込み、
 * 各種情報を出力する。
 */

#endif
