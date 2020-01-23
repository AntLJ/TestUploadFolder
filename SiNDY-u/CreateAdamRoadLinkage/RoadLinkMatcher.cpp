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
	// ���H�����N�̃t�B�[���h�}�b�v
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
		// ���H�����N���t�����K���������Ă���Ȃ�A�|�C���g�ւ̕������ɋt�����ɂ���
		Direction roadDirection = getDirection(feature);
		

		// �}�b�`���O���[�g���v�Z
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
			// ���H�����N�𐳕����Ƃ����Ƃ��̃}�b�`���O���[�g
			WKSPoints destPointsFoword = featureToWKSPoints(feature, false);
			double forwordRate = getMatchingRate(srcPoints, destPointsFoword);
			// ���H�����N���t�����Ƃ����Ƃ��̃}�b�`���O���[�g
			WKSPoints destPointsReverse = featureToWKSPoints(feature, true);
			double reverseRate = getMatchingRate(srcPoints, destPointsReverse);

			matchingRate = min( forwordRate, reverseRate);
			isRoadReverse = reverseRate < forwordRate;
		}
		
		// ���܂Ō������̂����}�b�`���O���x��������(�l�͏�����)�Ȃ�΁A�Ԃ��t�B�[�`�����X�V
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
	// TODO: �|�����C���łȂ���ΏI��

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

	// �J�o�[�����v�Z
	double tmp = 0;
	double coverRatio = PAU::calcCoverRatio(src_points, dest_points, tmp, m_MatchingRange);
	if(coverRatio == 0)
		return DBL_MAX;

	WKSPoints cutPoints;
	bool fromMatch = false, toMatch = false;

	// SIP�����N�𓹘H�����N�̋�ԂŐؒf����
	if(PAU::cutLineByLine(dest_points, src_points, cutPoints, fromMatch, toMatch, m_MatchingRange))
	{
		// �ؒf���ꂽSIP�����N�Ɠ��H�����N�̂����A�ł����ꂽ�_���v�Z���A�J�o�[���Ŋ����ă}�b�`���O���x���v�Z
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

	// �����Ȃ�����ʍs�i�������j
	if( onewayC == oneway::kFore )
		return kForword;

	// �����Ȃ�����ʍs�i�t�����j
	if( onewayC == oneway::kReverse )
		return kReverse;

	return kBoth;
}
