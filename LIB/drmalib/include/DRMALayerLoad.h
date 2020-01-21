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

// DRMALayerLoad.h
//
//////////////////////////////////////////////////////////////////////

/**
 * @file DRMALayerLoad.h
 * @brief DRMAデータレイヤ単位読み込みクラス定義ヘッダファイル
 */
#ifndef __DRMA_LAYER_LOAD_H__
#define __DRMA_LAYER_LOAD_H__

#include "DRMALoadProtocol.h"
#include "DRMALayer.h"

/**
 * @brief レイヤ単位読み込みテンプレートクラス
 */
template<typename TLayer, typename TProtocol = DRMALoadProtocol>
class DRMALayerLoadTemplate : public TProtocol, public TLayer
{
protected:
// プロトコルの実装
	virtual bool append(int cRecordId, const char* cOriginalRecord)
	{
		if(cRecordId != Record::sRecordId)
			return true;

		TLayer::append(reinterpret_cast<const Record::Format&>(*cOriginalRecord));

		return true;
	}

	virtual void clear()
	{
		TLayer::clear();
	}
};

/**
 * @brief 管理レイヤ用読み込みテンプレートクラス
 *
 * 管理データは１レコードしか存在しないため、１つ見つかったらそこで終了
 */
template<typename TLayer, typename TProtocol = DRMALoadProtocol>
class DRMAAdminLayerLoadTemplate : public DRMALayerLoadTemplate<TLayer, TProtocol>
{
protected:
// プロトコルの実装
	virtual bool append(int cRecordId, const char* cOriginalRecord)
	{
		if(! DRMALayerLoadTemplate<TLayer, TProtocol>::append(cRecordId, cOriginalRecord))
			return false;

		setComplete();

		return true;
	}
};

// DRMAレイヤ読み込みクラス定義

typedef DRMAAdminLayerLoadTemplate<DRMAAdmin1Layer> DRMAAdmin1Load;
typedef DRMAAdminLayerLoadTemplate<DRMAAdmin2Layer> DRMAAdmin2Load;
typedef DRMALayerLoadTemplate<DRMABaseRoadNodeLayer> DRMABaseRoadNodeLoad;
typedef DRMALayerLoadTemplate<DRMABaseRoadLinkLayer> DRMABaseRoadLinkLoad;
typedef DRMALayerLoadTemplate<DRMABaseRoadLinkAttrLayer> DRMABaseRoadLinkAttrLoad;
typedef DRMALayerLoadTemplate<DRMARoadLinkRelationLayer> DRMARoadLinkRelationLoad;
typedef DRMALayerLoadTemplate<DRMABeaconLocationLayer> DRMABeaconLocationLoad;
typedef DRMALayerLoadTemplate<DRMAAllRoadNodeLayer> DRMAAllRoadNodeLoad;
typedef DRMALayerLoadTemplate<DRMAAllRoadLinkLayer> DRMAAllRoadLinkLoad;
typedef DRMALayerLoadTemplate<DRMASiteLayer> DRMASiteLoad;
typedef DRMALayerLoadTemplate<DRMAAdminLocationLayer> DRMAAdminLocationLoad;
typedef DRMALayerLoadTemplate<DRMARailwayLocationLayer> DRMARailwayLocationLoad;
typedef DRMALayerLoadTemplate<DRMAFacilLocationLayer> DRMAFacilLocationLoad;
typedef DRMALayerLoadTemplate<DRMAFacilShapeLayer> DRMAFacilShapeLoad;
typedef DRMALayerLoadTemplate<DRMAPlaceNameLocationLayer> DRMAPlaceNameLocationLoad;
typedef DRMALayerLoadTemplate<DRMAAllRoadLinkAttrLayer> DRMAAllRoadLinkAttrLoad;

/**
 * @example example2.cpp
 *
 * Dドライブのルートディレクトリ直下に存在する「DRMA」ディレクトリより533945の基本道路ノードデータを読み込み、
 * 交差点名称を持つノードの情報を標準出力に出力する。
 */

#endif
