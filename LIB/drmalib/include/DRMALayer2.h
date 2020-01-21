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

// DRMALayer2.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRMA_LAYER2_H__
#define __DRMA_LAYER2_H__

#include <map>
#include "DRMALayer.h"
#include "DRMABaseRoadNode2.h"
#include "DRMABaseRoadLink2.h"
#include "DRMABaseRoadLinkAttr2.h"
#include "DRMAAllRoadNode2.h"
#include "DRMAAllRoadLink2.h"
#include "DRMAAllRoadLinkAttr2.h"

/**
 * @brief DRMALayer拡張クラス
 *
 * @note DRMALayerにインデックス領域を追加する。
 */
template <typename T>
class DRMALayer2T : public T
{
public:
	typedef std::map<typename T::Record::Id, typename T::Record*> IndexedRecordMap;

// 設定/取得
	virtual void clearIndex()
	{
		m_cIndexedRecordMap.clear();
	}

	virtual void setIndex()
	{
		clearIndex();
		for(iterator it = begin(); it != end(); ++it) {
			indexRecord(&*it);
		}
	}

	virtual const typename T::Record* search(const typename T::Record::Id& cId) const
	{
		IndexedRecordMap::const_iterator it = m_cIndexedRecordMap.find(cId);
		return it != m_cIndexedRecordMap.end() ? it->second : 0;
	}

	virtual typename T::Record* search(const typename T::Record::Id& cId)
	{
		IndexedRecordMap::iterator it = m_cIndexedRecordMap.find(cId);
		return it != m_cIndexedRecordMap.end() ? it->second : 0;
	}

private:
// 設定/取得
	void indexRecord(typename T::Record* cRecord)
	{
		m_cIndexedRecordMap[cRecord->eId] = cRecord;
	}

// プロパティ
	IndexedRecordMap m_cIndexedRecordMap;
};

typedef DRMALayer2T<DRMABaseRoadNodeLayerT<DRMABaseRoadNode2> > DRMABaseRoadNodeLayer2;
typedef DRMALayer2T<DRMABaseRoadLinkLayerT<DRMABaseRoadLink2> > DRMABaseRoadLinkLayer2;
typedef DRMALayer2T<DRMABaseRoadLinkAttrLayerT<DRMABaseRoadLinkAttr2> > DRMABaseRoadLinkAttrLayer2;
typedef DRMALayer2T<DRMARoadLinkRelationLayerT<DRMARoadLinkRelation> > DRMARoadLinkRelationLayer2;
typedef DRMALayer2T<DRMABeaconLocationLayerT<DRMABeaconLocation> > DRMABeaconLocationLayer2;
typedef DRMALayer2T<DRMAAllRoadNodeLayerT<DRMAAllRoadNode2> > DRMAAllRoadNodeLayer2;
typedef DRMALayer2T<DRMAAllRoadLinkLayerT<DRMAAllRoadLink2> > DRMAAllRoadLinkLayer2;
typedef DRMALayer2T<DRMASiteLayerT<DRMASite> > DRMASiteLayer2;
typedef DRMALayer2T<DRMAAdminLocationLayerT<DRMAAdminLocation> > DRMAAdminLocationLayer2;
typedef DRMALayer2T<DRMARailwayLocationLayerT<DRMARailwayLocation> > DRMARailwayLocationLayer2;
typedef DRMALayer2T<DRMAFacilLocationLayerT<DRMAFacilLocation> > DRMAFacilLocationLayer2;
typedef DRMALayer2T<DRMAFacilShapeLayerT<DRMAFacilShape> > DRMAFacilShapeLayer2;
typedef DRMALayer2T<DRMAPlaceNameLocationLayerT<DRMAPlaceNameLocation> > DRMAPlaceNameLocationLayer2;
typedef DRMALayer2T<DRMAAllRoadLinkAttrLayerT<DRMAAllRoadLinkAttr2> > DRMAAllRoadLinkAttrLayer2;

#endif
