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
#include "UndergroundCheckFunction.h"
#include "value.h"

#include <sindy/schema/sj.h>
#include <sindy/schema/sindyk.h>

#include <ArcHelperEx/define.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobals.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheGeometryConvert.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#undef min
#undef max
using namespace sindy::schema;

void UndergroundCheckFunction::CheckUnderground( 
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo, 
	const FeaturesPack& underArea,
	const FeaturesPack& walkLink
	)
{
	using namespace walk_link;
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, underground_area::kTableName );
	std::set<long> underAreas = util::getTargetIDs( underArea.m_FeatureMap, ipMeshGeo );
	if(underAreas.empty()) return;

	// 地下通路だけ抜き出す
	// bug 12738 で「地下改札内」「地下プラットフォーム」追加
	static const std::set<long > underGround = boost::assign::list_of
		( walk_class::kUnderGround )
		( walk_class::kInternalTicketGateUG )
		( walk_class::kPlatFormUG );

	std::set<long> underLinks = util::getTargetIDs(	walkLink, kWalkClass, underGround, ipMeshGeo);
	if(underLinks.empty()) return;

	// 複数のポリゴンに跨って内包されているものをここで除外
	IPolygonPtr unionPolygon = util::getUnionGeometry( underArea.m_FeatureMap, underAreas );
	assert(unionPolygon);

	// いずれの地下街ポリゴンにも含まれない地下通路リンクのOIDを取得
	std::set<long> outLinks = util::excludeContainLink( underLinks, unionPolygon, walkLink.m_FeatureMap );

	// 地下街ポリゴンが一部はみ出ている部分をそれぞれエラー出力
	errLinkPartIncludedArea(underLinks, outLinks, unionPolygon, underAreas, walkLink, underArea, meshcode);

	// 地下街ポリゴンが完全に含まれていないリンク群をそれぞれエラー出力
	errOutLink(outLinks, walkLink, meshcode);

	// メッシュ内の地下通路リンクを繋げたとき、地下街ポリゴンに重ならないリンク群をエラー出力
	errNoneExist(underLinks, unionPolygon, walkLink, meshcode);
}

void UndergroundCheckFunction::errLinkPartIncludedArea(
	const std::set<long>& underLinkIdAll
	, const std::set<long>& outsideLinkId
	, const IGeometryPtr& underAreaUnion
	, const std::set<long>& underAreaId
	, const FeaturesPack& walkLink
	, const FeaturesPack& underArea
	, const CString& meshcode)
{
	std::vector<long> overlapLinks;
	std::set_difference(
		underLinkIdAll.begin(), underLinkIdAll.end(),
		outsideLinkId.begin(), outsideLinkId.end(),
		std::back_inserter(overlapLinks) );

	std::sort(overlapLinks.begin(), overlapLinks.end());

	long modOperatorIndex = walkLink.m_FieldMap.FindField( walk_link::kOperator );
	long modDateIndex = walkLink.m_FieldMap.FindField( walk_link::kModifyDate );
	long modProgramIndex = walkLink.m_FieldMap.FindField( walk_link::kModifyProgName );
	long modProgramDateIndex = walkLink.m_FieldMap.FindField( walk_link::kProgModifyDate );
	for(const long linkID : overlapLinks)
	{
		auto& linkInfo = walkLink.m_FeatureMap.at( linkID );
		// マージされた地下街ポリゴンに含まれるものはエラーではない
		if( AheIsContain4( underAreaUnion, linkInfo.m_ipGeometry ) )
			continue;

		auto points = util::getCrossPoint( underAreaUnion, linkInfo.m_ipGeometry );
		std::sort(points.begin(), points.end(),
			[&](const IPointPtr& ipoint1, const IPointPtr& ipoint2)
			{
				if( ipoint1->GetX() == ipoint2->GetX() )
					return ipoint1->GetY() < ipoint2->GetY();
				return ipoint1->GetX() < ipoint2->GetX();
			});

		if ( checkModifyTimeToPartIncludedErr( linkInfo, walkLink.m_FieldMap, underAreaId, underArea, points ) )
		{

			CString lonStr, latStr;
			for(auto& point : points)
			{
				lonStr.AppendFormat(_T("%.14lf,"), point->GetX());
				latStr.AppendFormat(_T("%.14lf,"), point->GetY());
			}
			if(!lonStr.IsEmpty())
				lonStr.TrimRight(_T(","));
			if(!latStr.IsEmpty())
				latStr.TrimRight(_T(","));

			// ★エラー 地下通路の一部分が地下街からはみ出ている
			m_output.OutputLog( walk_link::kTableName, linkID, err_level::warning, err_code::PartOfUnderLinkIncludedArea,
				meshcode, linkInfo.GetStringValue( modOperatorIndex ),
				linkInfo.GetStringValue( modDateIndex ), linkInfo.GetStringValue( modProgramIndex ),
				linkInfo.GetStringValue( modProgramDateIndex ),
				lonStr, latStr);
		}
	}
}

bool UndergroundCheckFunction::checkModifyTimeToPartIncludedErr(
	const GeoClass& linkInfo
	, const sindy::CFieldMap& fieldMap
	, const std::set<long>& underAreaId
	, const FeaturesPack& underArea
	, const std::vector<IPointPtr>& crossPoint)
{
	if( VT_DATE!=m_modifyFilter.vt ) return true;
	long linkProgModDateIndex = fieldMap.FindField( ipc_feature::kProgModifyDate );
	long linkModDateIndex = fieldMap.FindField( ipc_feature::kModifyDate );
	long areaProgModDateIndex = underArea.m_FieldMap.FindField( ipc_feature::kProgModifyDate );
	long areaModDateIndex = underArea.m_FieldMap.FindField( ipc_feature::kModifyDate );

	if ( util::checkModifyTime( m_modifyFilter, linkInfo.GetValue( linkModDateIndex ), linkInfo.GetValue( linkProgModDateIndex ) ) )
		return true;

	// 接触している地下街ポリゴンが、指定日時以降に更新されているなら、それもチェック対象
	// 期待される処理数が少ないからこそできる処理
	std::set<long> intersectAreaID;
	for(const auto underID : underAreaId)
	{
		bool ok = false;
		for(const auto& poi : crossPoint)
		{
			if ( AheIsDisjoint( poi, underArea.m_FeatureMap.at( underID ).m_ipGeometry) )
				continue;
			ok = true;
			break;
		}
		if(!ok) continue;
		intersectAreaID.insert(underID);
	}
	for(const long areaId : intersectAreaID)
	{
		const auto& areaMap = underArea.m_FeatureMap.at( areaId );
		if ( util::checkModifyTime( m_modifyFilter, areaMap.GetValue( areaModDateIndex ), areaMap.GetValue( areaProgModDateIndex ) ) )
			return true;
	}
	return false;
}

void UndergroundCheckFunction::errOutLink(
	const std::set<long>& outLinks
	, const FeaturesPack& walkLinkPack
	, const CString& meshcode)
{
	long modOperatorIndex = walkLinkPack.m_FieldMap.FindField( walk_link::kOperator );
	long modDateIndex = walkLinkPack.m_FieldMap.FindField( walk_link::kModifyDate );
	long modProgramIndex = walkLinkPack.m_FieldMap.FindField( walk_link::kModifyProgName );
	long modProgramDateIndex = walkLinkPack.m_FieldMap.FindField( walk_link::kProgModifyDate );
	auto group = util::toEveryPartLink(outLinks, walkLinkPack );
	for(const auto& elem : group)
	{
		auto& walkLink = walkLinkPack.m_FeatureMap.at( elem.first );
		long  objectId   = elem.first;
		// ★エラー 地下通路が完全に地下街からはみ出ている
		m_output.OutputLog( walk_link::kTableName, objectId, err_level::error, err_code::UndergroundAreaNoneOverLinks,
			meshcode, walkLink.GetStringValue( modOperatorIndex ), 
			walkLink.GetStringValue( modDateIndex ), walkLink.GetStringValue( modProgramIndex ),
			walkLink.GetStringValue( modProgramDateIndex ) );
	}
}

void UndergroundCheckFunction::errNoneExist(
		const std::set<long>& underLinks
		, const IGeometryPtr& unionArea
		, const FeaturesPack& walkLinkPack
		, const CString& meshcode)
{
	long modOperatorIndex = walkLinkPack.m_FieldMap.FindField( walk_link::kOperator );
	long modDateIndex = walkLinkPack.m_FieldMap.FindField( walk_link::kModifyDate );
	long modProgramIndex = walkLinkPack.m_FieldMap.FindField( walk_link::kModifyProgName );
	long modProgramDateIndex = walkLinkPack.m_FieldMap.FindField( walk_link::kProgModifyDate );
	auto group = util::toEveryPartLink(underLinks, walkLinkPack );
	for(auto& elem : group)
	{
		IGeometryPtr unionLink = util::getUnionGeometry( walkLinkPack.m_FeatureMap, elem.second );
		assert(unionLink);
		if( !AheIsDisjoint(unionArea, unionLink) )
			continue;

		IEnvelopePtr env;
		unionLink->get_Envelope(&env);
		IAreaPtr area(env);
		IPointPtr centerPoint;
		area->get_Centroid(&centerPoint);

		auto& walkLink = walkLinkPack.m_FeatureMap.at( elem.first );
		long  objectId   = elem.first;
		// ★エラー 地下通路リンク群に重なる地下街がない
		m_output.OutputLog( walk_link::kTableName, objectId, err_level::error, err_code::UndergroundAreaNotExist,
			meshcode, walkLink.GetStringValue( modOperatorIndex ),
			walkLink.GetStringValue( modDateIndex ), walkLink.GetStringValue( modProgramIndex ),
			walkLink.GetStringValue( modProgramDateIndex ),
			_T(""), _T(""), centerPoint->GetX(), centerPoint->GetY());
	}
}
