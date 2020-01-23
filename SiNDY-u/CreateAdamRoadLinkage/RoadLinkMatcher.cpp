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

// ArcHelper
#include "AheGeometryConvert.h"

#include "PointArrayUtil.h"
#include "RoadLinkMatcher.h"

extern crd_cnv g_cnv;
namespace PAU = PointArrayUtil;


RoadLinkMatcher::RoadLinkMatcher(const ITablePtr& roadlink_t, double matching_range)
	: m_MatchingRange(matching_range)
{	
	// 道路リンクのフィールドマップ
	IFieldsPtr fields = roadlink_t->GetFields();
	m_RoadLinkFM.CreateFieldMap(fields);
}

IFeaturePtr RoadLinkMatcher::getMatchLink(const IFeaturePtr& src_feature, const std::vector<IFeaturePtr>& dest_features, bool& is_reverse, bool as_src_is_reverse)
{
	WKSPoints srcPoints = featureToWKSPoints(src_feature, as_src_is_reverse);
	if( srcPoints.size() < 2 )
		return nullptr;

	IFeaturePtr matchFeature;
	double bestMatchingRate(DBL_MAX);
	for( const auto& feature : dest_features )
	{
		// 道路リンクが逆方向規制を持っているなら、ポイントへの分解時に逆向きにする
		Direction roadDirection = getDirection(feature);
		

		// マッチングレートを計算
		double matchingRate(DBL_MAX);
		bool isRoadReverse(false);
		if( roadDirection == kForword )
		{
			WKSPoints destPoints = featureToWKSPoints(feature, false);
			matchingRate = getMatchingRate(srcPoints, destPoints);
		}
		else if( roadDirection == kReverse )
		{
			WKSPoints destPoints = featureToWKSPoints(feature, true);
			matchingRate = getMatchingRate(srcPoints, destPoints);
			isRoadReverse = true;
		}
		else if( roadDirection == kBoth )
		{
			// 道路リンクを正方向としたときのマッチングレート
			WKSPoints destPointsFoword = featureToWKSPoints(feature, false);
			double forwordRate = getMatchingRate(srcPoints, destPointsFoword);
			// 道路リンクを逆方向としたときのマッチングレート
			WKSPoints destPointsReverse = featureToWKSPoints(feature, true);
			double reverseRate = getMatchingRate(srcPoints, destPointsReverse);

			matchingRate = min( forwordRate, reverseRate);
			isRoadReverse = reverseRate < forwordRate;
		}
		
		// 今まで見たものよりもマッチングレベルが高い(値は小さい)ならば、返すフィーチャを更新
		if( matchingRate < bestMatchingRate ){
			matchFeature = feature;
			bestMatchingRate = matchingRate;
			is_reverse = isRoadReverse;
		}
	}

	if( ! matchFeature )
		return nullptr;

	return matchFeature;
}

WKSPoints RoadLinkMatcher::featureToWKSPoints(const IFeaturePtr& feature, bool reverse)
{
	WKSPoints ret;

	IGeometryPtr srcGeo = feature->GetShape();
	// TODO: ポリラインでなければ終了

	// Polygon -> MultiPoint
	auto srcMultiPoint = ToMultipoint( srcGeo );

	// MultiPoint -> Point -> WKSZPoint
	IPointCollectionPtr srcPointCol(srcMultiPoint);
	long count = srcPointCol->GetPointCount();
	for( int i = 0; i < count; ++i )
	{
		IPointPtr point = srcPointCol->GetPoint(i);
		WKSPoint wksPt = {};
		point->QueryCoords( &wksPt.X, &wksPt.Y);

		if( reverse )
			ret.insert(ret.begin(), wksPt );
		else
			ret.push_back(wksPt);
	}

	return ret;
}

double RoadLinkMatcher::getMatchingRate(const WKSPoints& src_points, const WKSPoints& dest_points)
{
	if( src_points.size() < 2 || dest_points.size() < 2 )
		return DBL_MAX;

	// カバー率を計算
	double tmp = 0;
	double coverRatio = PAU::calcCoverRatio(src_points, dest_points, tmp, m_MatchingRange);
	if(coverRatio == 0)
		return DBL_MAX;

	WKSPoints cutPoints;
	bool fromMatch = false, toMatch = false;

	// SIPリンクを道路リンクの区間で切断する
	if(PAU::cutLineByLine(dest_points, src_points, cutPoints, fromMatch, toMatch, m_MatchingRange))
	{
		// 切断されたSIPリンクと道路リンクのうち、最も離れた点を計算し、カバー率で割ってマッチングレベル計算
		double dist = PAU::getFarthestDistance(cutPoints, src_points);

		return dist/coverRatio;
	}

	return DBL_MAX;
}

RoadLinkMatcher::Direction RoadLinkMatcher::getDirection( const IFeaturePtr& feature )
{
	using namespace sindy::schema::road_link;

	long indexOnewayC = m_RoadLinkFM.FindField( kOneway );
	long onewayC = feature->GetValue(indexOnewayC);

	// 条件なし一方通行（正方向）
	if( onewayC == oneway::kFore )
		return kForword;

	// 条件なし一方通行（逆方向）
	if( onewayC == oneway::kReverse )
		return kReverse;

	return kBoth;
}
