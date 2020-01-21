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

// DRMALayer.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRMA_LAYER_H__
#define __DRMA_LAYER_H__

#include <vector>

#include "DRMAAdmin1.h"
#include "DRMAAdmin2.h"
#include "DRMABaseRoadNode.h"
#include "DRMABaseRoadLink.h"
#include "DRMABaseRoadLinkAttr.h"
#include "DRMARoadLinkRelation.h"
#include "DRMABeaconLocation.h"
#include "DRMAAllRoadNode.h"
#include "DRMAAllRoadLink.h"
#include "DRMASite.h"
#include "DRMAAdminLocation.h"
#include "DRMARailwayLocation.h"
#include "DRMAFacilLocation.h"
#include "DRMAFacilShape.h"
#include "DRMAPlaceNameLocation.h"
#include "DRMAAllRoadLinkAttr.h"

/**
 * @brief レイヤの基本形
 */
template <typename TRecord>
class DRMALayerTemplate : public std::vector<TRecord>
{
public:
	typedef TRecord Record; ///< レコードクラス

	/**
	 * @brief レコードを追加する
	 *
	 * @param cRecord [in] 追加するレコード
	 */
	virtual void append(const typename TRecord::Format& cRecord)
	{
		push_back(Record(cRecord));
	}
};

/**
 * @brief 継続フラグ有りレイヤの基本形
 */
template <typename TRecord>
class DRMAContinualLayerTemplate : public DRMALayerTemplate<TRecord>
{
public:
	/**
	 * @brief レコードを追加する
	 *
	 * @note 直前のレコードで継続フラグが立っていた場合、そちらに値を追加する。
	 *
	 * @param cRecord [in] 追加するレコード
	 */
	virtual void append(const typename Record::Format& cRecord)
	{
		if(! empty()) {
			Record& rRecord = *(rbegin());
			if(rRecord.eContinue != 0) {
				rRecord.append(cRecord);
				return;
			}
		}

		DRMALayerTemplate<Record>::append(cRecord);
	}
};

// DRMAレイヤ一覧

#define DRMAAdmin1LayerT DRMALayerTemplate
#define DRMAAdmin2LayerT DRMALayerTemplate
#define DRMABaseRoadNodeLayerT DRMAContinualLayerTemplate
#define DRMABaseRoadLinkLayerT DRMAContinualLayerTemplate
#define DRMABaseRoadLinkAttrLayerT DRMAContinualLayerTemplate
#define DRMARoadLinkRelationLayerT DRMAContinualLayerTemplate
#define DRMABeaconLocationLayerT DRMALayerTemplate
#define DRMAAllRoadNodeLayerT DRMALayerTemplate
#define DRMAAllRoadLinkLayerT DRMAContinualLayerTemplate
#define DRMASiteLayerT DRMAContinualLayerTemplate
#define DRMAAdminLocationLayerT DRMAContinualLayerTemplate
#define DRMARailwayLocationLayerT DRMAContinualLayerTemplate
#define DRMAFacilLocationLayerT DRMALayerTemplate
#define DRMAFacilShapeLayerT DRMAContinualLayerTemplate
#define DRMAPlaceNameLocationLayerT DRMALayerTemplate
#define DRMAAllRoadLinkAttrLayerT DRMAContinualLayerTemplate

typedef DRMAAdmin1LayerT<DRMAAdmin1> DRMAAdmin1Layer;
typedef DRMAAdmin2LayerT<DRMAAdmin2> DRMAAdmin2Layer;
typedef DRMABaseRoadNodeLayerT<DRMABaseRoadNode> DRMABaseRoadNodeLayer;
typedef DRMABaseRoadLinkLayerT<DRMABaseRoadLink> DRMABaseRoadLinkLayer;
typedef DRMABaseRoadLinkAttrLayerT<DRMABaseRoadLinkAttr> DRMABaseRoadLinkAttrLayer;
typedef DRMARoadLinkRelationLayerT<DRMARoadLinkRelation> DRMARoadLinkRelationLayer;
typedef DRMABeaconLocationLayerT<DRMABeaconLocation> DRMABeaconLocationLayer;
typedef DRMAAllRoadNodeLayerT<DRMAAllRoadNode> DRMAAllRoadNodeLayer;
typedef DRMAAllRoadLinkLayerT<DRMAAllRoadLink> DRMAAllRoadLinkLayer;
typedef DRMASiteLayerT<DRMASite> DRMASiteLayer;
typedef DRMAAdminLocationLayerT<DRMAAdminLocation> DRMAAdminLocationLayer;
typedef DRMARailwayLocationLayerT<DRMARailwayLocation> DRMARailwayLocationLayer;
typedef DRMAFacilLocationLayerT<DRMAFacilLocation> DRMAFacilLocationLayer;
typedef DRMAFacilShapeLayerT<DRMAFacilShape> DRMAFacilShapeLayer;
typedef DRMAPlaceNameLocationLayerT<DRMAPlaceNameLocation> DRMAPlaceNameLocationLayer;
typedef DRMAAllRoadLinkAttrLayerT<DRMAAllRoadLinkAttr> DRMAAllRoadLinkAttrLayer;

#endif
