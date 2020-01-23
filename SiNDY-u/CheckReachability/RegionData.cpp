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

#include "stdafx.h"
#include <atl2/algorithm.h>
#include <arctl/iterator.h>
#include <sindy/schema.h>
#include "RegionData.h"

namespace {

/**
 * @brief リージョン形状に重なる道路リンクの一覧を作成する。
 *
 * @param ipRoadLinkClass [in] 道路リンクフィーチャクラス。
 * @param ipOuterGeom [in] リージョン（オーバーラップ）形状。
 * @param lpszWhereClause [in] リージョン構成道路リンク識別Where句。
 */
std::auto_ptr<RoadLinks>
create_region_road_links(IFeatureClass* ipRoadLinkClass, IGeometry* ipOuterGeom, LPCTSTR lpszWhereClause)
{
	std::auto_ptr<RoadLinks> pRoadLinks(new RoadLinks);

	const ISpatialFilterPtr ipSpFilter(__uuidof(SpatialFilter));
	ipSpFilter->WhereClause = lpszWhereClause;
	if(ipOuterGeom != 0) {
		ipSpFilter->Geometry = ipOuterGeom;
		ipSpFilter->SpatialRel = esriSpatialRelIntersects;
	}
	ipSpFilter->SubFields = L"";
	RoadLink::index_type::copy_name(ipRoadLinkClass, arctl::adder(ipSpFilter, &ISpatialFilter::_AddField));

	const RoadLink::index_type aIndex(ipRoadLinkClass);
	atl2::transform(
		ipRoadLinkClass->_Search(ipSpFilter, VARIANT_TRUE),
		std::back_inserter(*pRoadLinks),
		arctl::field::interpreter<RoadLink>(&aIndex)
	);

	return pRoadLinks;
}

} // anonymous namespace

/**
 * @brief リージョンのオブジェクト情報を作成する。
 *
 * @param ipRoadLinkClass [in] 道路リンクフィーチャクラス。
 * @param ipRoadNodeClass [in] 道路ノードフィーチャクラス。
 * @param rRegionProp [in] リージョンオブジェクト識別情報。
 */
std::auto_ptr<RegionData>
create_region_data(IFeatureClass* ipRoadLinkClass, IFeatureClass* ipRoadNodeClass, const RegionProperty& rRegionProp)
{
	_ASSERT(! ((rRegionProp.m_ipInnerArea == 0) ^ (rRegionProp.m_ipOuterArea == 0)));

	std::auto_ptr<RoadLinks>
		pRoadLinks(
			create_region_road_links(
				ipRoadLinkClass,
				IGeometryPtr(rRegionProp.m_ipOuterArea),
				rRegionProp.m_strRoadLinkWhereClause.c_str()
			)
		);

	using namespace arctl::field;
	using namespace sindy::schema::road_link;

	RoadNodeIDSet aRoadNodeIDSet;
	for(RoadLinks::const_iterator it(pRoadLinks->begin()), it_end(pRoadLinks->end()); it != it_end; ++it) {
		const RoadLink& rRoadLink = *it;
		aRoadNodeIDSet.insert(get<kFromNodeID>(rRoadLink));
		aRoadNodeIDSet.insert(get<kToNodeID>(rRoadLink));
	}

	std::auto_ptr<RoadNodeIDSet> pInnerRoadNodeIDSet(new RoadNodeIDSet), pOuterRoadNodeIDSet(new RoadNodeIDSet);

	if(rRegionProp.m_ipOuterArea == 0) {
		pInnerRoadNodeIDSet->swap(aRoadNodeIDSet);
	}
	else {
		const IRelationalOperatorPtr ipInnerRelOp(rRegionProp.m_ipInnerArea);

		const ISpatialFilterPtr ipSpFilter(__uuidof(SpatialFilter));
		ipSpFilter->SpatialRel = esriSpatialRelIntersects;
		ipSpFilter->Geometry = IGeometryPtr(rRegionProp.m_ipOuterArea);
		ipSpFilter->SubFields = ipRoadNodeClass->OIDFieldName;
		ipSpFilter->_AddField(ipRoadNodeClass->ShapeFieldName);

		const IFeatureCursorPtr ipFeatureCursor(ipRoadNodeClass->_Search(ipSpFilter, VARIANT_TRUE));
		for(IFeaturePtr ipFeature; ipFeatureCursor->NextFeature(&ipFeature) == S_OK; ) {
			const long nOID = ipFeature->OID;
			if(aRoadNodeIDSet.count(nOID) == 0)
				continue;

			(ipInnerRelOp->_Disjoint(ipFeature->Shape) ? pOuterRoadNodeIDSet : pInnerRoadNodeIDSet)->insert(nOID);
		}
	}

	std::auto_ptr<RegionData> pRegionData(new RegionData);
	pRegionData->m_pRoadLinks = pRoadLinks;
	pRegionData->m_pInnerRoadNodeIDSet = pInnerRoadNodeIDSet;
	pRegionData->m_pOuterRoadNodeIDSet = pOuterRoadNodeIDSet;

	return pRegionData;
}
