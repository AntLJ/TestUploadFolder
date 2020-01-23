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
#include "LinkRelationCheckFunction.h"
#include "value.h"
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <ArcHelperEx/AheBuddyGlobals.h>

extern crd_cnv g_cnv;

using namespace sindy::schema;
using namespace std;

void LinkRelationCheckFunction::checkLinkRelation(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadLink,
	const FeaturesPack& walkLink,
	const MultiRowsPack& linkRelationPack,
	bool bLinkRelation
	)
{
	using namespace link_relation;
	using namespace ipc_table;
	using namespace uh::str_util;

	m_output.SetLayer( walk_link::kTableName );
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, kTableName );

	map<long, IGeometryPtr> relLinks;
	long baseLinkID = 0;

	// ���s�\��ʂɃG���[�����铹�H�����N��ID��ێ����Ă���
	std::set<long> walkableErrRoadLinks;

	long relatedIndex = linkRelationPack.m_FieldMap.FindField( kRelatedLinkID );
	long baseIndex = linkRelationPack.m_FieldMap.FindField( kBaseLinkID );
	long walkClassIndex = walkLink.m_FieldMap.FindField( walk_link::kWalkClass );
	long autoWalkIndex = walkLink.m_FieldMap.FindField( walk_link::kAutoWalkFlag );
	long escalatorIndex = walkLink.m_FieldMap.FindField( walk_link::kEscalatorFlag );

	long operatorIndex = walkLink.m_FieldMap.FindField( kOperator );
	long dateIndex = walkLink.m_FieldMap.FindField( kModifyDate );
	long programIndex = walkLink.m_FieldMap.FindField( kModifyProgName );
	long programDateIndex = walkLink.m_FieldMap.FindField( kProgModifyDate );
	long roadClassIndex = roadLink.m_FieldMap.FindField( road_link::kRoadClass );
	long walkableIndex = roadLink.m_FieldMap.FindField( road_link::kWalkable );
	double maxDistance = -1;
	for( const auto& linkRelation: linkRelationPack.m_Rows)
	{
		auto& linkRelationObj = linkRelation.second;
		long walkLinkID = linkRelationObj->GetValue( baseIndex ).lVal;

		auto walkIte = walkLink.m_FeatureMap.find( walkLinkID );
		// �R�t�������N���`�F�b�N�Ώۃ��b�V���ɑ��݂��Ȃ��ꍇ�X�L�b�v
		if( walkIte == walkLink.m_FeatureMap.end() )
			continue;
		if( walkIte->second.IsDisjoint(ipMeshGeo))
			continue;

		long roadLinkID = linkRelationObj->GetValue( relatedIndex ).lVal;
		//linkRelarion.first�͂����Ŏg��ObjectID�ł͂Ȃ�
		long objectId = linkRelationObj->GetOID();
		auto& walkLinkObj = walkIte->second;

		m_output.SetModifyInfo( walkLinkID, walkLinkObj.GetStringValue( operatorIndex ),
			walkLinkObj.GetStringValue( dateIndex ), walkLinkObj.GetStringValue( programIndex ), walkLinkObj.GetStringValue( programDateIndex ) );
		long walkLinkClass = walkLinkObj.GetLongValue( walkClassIndex );
		// ���G���[ ������N�̕��s�Ҏ�ʂ��ΏۊO
		if ( !judge_value::IsOKBaseLinkWalkCode( walkLinkClass ) )
			m_output.OutputRegLog( err_level::error, err_code::NgLRWalkClass,
				ToString( objectId ), ToString( walkLinkClass ) );
		// ���G���[ �R�t�ΏۊO�ɂȂ�t���O��ON�ɂȂ��Ă���
		auto autoWalkFlag = walkLinkObj.GetLongValue( autoWalkIndex );
		if( autoWalkFlag )
			m_output.OutputRegLog( err_level::error, err_code::NgLRWalkFlag,
				ToString( objectId ), walk_link::kAutoWalkFlag );
		// ���G���[ �R�t�ΏۊO�ɂȂ�t���O��ON�ɂȂ��Ă���
		auto escalatorFlag = walkLinkObj.GetLongValue( escalatorIndex );
		if ( escalatorFlag == FLAG_ON )
			m_output.OutputRegLog( err_level::error, err_code::NgLRWalkFlag,
				ToString( objectId ), walk_link::kEscalatorFlag );

		// ���H�����N�̃`�F�b�N
		auto roadIte = roadLink.m_FeatureMap.find( roadLinkID );
		if ( roadIte == roadLink.m_FeatureMap.end() ) // ���G���[ �֘A�����NID�����H�����N�ɑ��݂��Ȃ�
		{
			m_output.OutputRegLog( err_level::error, err_code::NotExistRoadLink, 
				ToString( objectId ), ToString( roadLinkID ) );
			continue;
		}

		auto& roadLinkObj = roadIte->second;
		long roadLinkClass = roadLinkObj.GetLongValue( roadClassIndex );
		// bug 11461 - [u][SJ���s��]WC_1204�u�֘A�����N�̓��H�\����ʂ��ΏۊO�v�̎�ʂ�ύX���Ăق���
		// ���G���[ �֘A�����N�̓��H�\����ʂ��ΏۊO
		if ( !((0 < roadLinkClass && roadLinkClass < 200)
			|| roadLinkClass == road_link::road_class::kGarden) )

			// TODO: ID��modXX�������Ă��Ȃ����ǂ����̂��H
			m_output.OutputLog( road_link::kTableName, roadLinkID,
				err_level::error, err_code::NgLRRoadClass, meshcode,
				walkLinkObj.GetStringValue( operatorIndex ), walkLinkObj.GetStringValue( dateIndex ),
				walkLinkObj.GetStringValue( programIndex ), walkLinkObj.GetStringValue( programDateIndex ),
				ToString( objectId ), ToString( roadLinkClass ) );

		// ���G���[ �����N���m������Ă���
		checkLinkDistance(walkLinkObj, roadLinkID, roadLinkObj,objectId);

		// ���G���[ �����N���m�̊p�x���傫��
		checkLinkAngle( walkLinkObj, roadLinkID, roadLinkObj, objectId );

		// link_relation���[�h�ł͎��s���Ȃ�
		if(!bLinkRelation)
		{
			// ���G���[ Buddy�����̂ɕ��s�֎~�ȊO
			// ���H�����N�͕�����Buddy�������߁A������A���l�̃G���[�Ɉ����|����\��������
			// �G���[�Ƃ��ďo�͂���͈̂�x�ł����̂ŁA�R���e�i�ɋl�߂āA����܂Ƃ߂ďo�͂���
			if ( road_link::walkable_class::kNotWalk !=  roadLinkObj.GetLongValue( walkableIndex ) )
				walkableErrRoadLinks.insert(roadLinkID);
		}

		if(baseLinkID != walkLinkID)
		{
			if( baseLinkID != 0 )
				checkLinkRelationGroup( walkLink.m_FeatureMap.at( baseLinkID ), walkLink.m_FieldMap, relLinks, maxDistance, false );

			baseLinkID = walkLinkID;
			relLinks.clear();
		}
		relLinks[roadLinkID] = roadLinkObj.m_ipGeometry;
	}

	// link_relation���[�h�ł͎��s���Ȃ�
	if(!bLinkRelation)
	{
		outputWalkableErr( walkableErrRoadLinks, roadLink, meshcode );
	}

	if ( walkLink.m_FeatureMap.find( baseLinkID ) != walkLink.m_FeatureMap.end() )
		checkLinkRelationGroup( walkLink.m_FeatureMap.at( baseLinkID ), walkLink.m_FieldMap, relLinks, maxDistance, false );

	// WALK_LINK�̃`�F�b�N
	// WALK_LINK���Ń`�F�b�N�����LINK_RELATION�`�F�b�N���ȊO�ł��G���[�����o����Ă��܂�����
	auto linkRelationFlagindex = walkLink.m_FieldMap.FindField( walk_link::kLinkRelationRF );
	for ( const auto& walkLink : walkLink.m_FeatureMap )
	{
		// �`�F�b�N�Ώۃ��b�V���ȊO�̓X�L�b�v
		if( walkLink.second.IsDisjoint(ipMeshGeo))
			continue;

		if ( !walkLink.second.GetLongValue( linkRelationFlagindex ) )
			continue;

		auto range = linkRelationPack.m_Rows.equal_range( walkLink.first );
		// ���G���[ �����N�R�Â��e�[�u���t���O��ON�����A���s�ҁE���H�����N�R�t���e�[�u����񂪂Ȃ�
		if(range.first == range.second)
		{
			m_output.OutputLog( walk_link::kTableName, walkLink.first,
				err_level::error, err_code::NgLinkRelationF, meshcode,
				walkLink.second.GetStringValue( operatorIndex ), walkLink.second.GetStringValue( dateIndex ),
				walkLink.second.GetStringValue( programIndex ), walkLink.second.GetStringValue( programDateIndex ) );
		}
	}

	// Buddy�֌W�ň�ԋ����������ӏ����o��
	if( maxDistance >= 0 )
		m_output.OutputLog( err_level::info, err_code::MaxLinkDistance,
			uh::str_util::ToString( maxDistance ) );
}

void LinkRelationCheckFunction::checkHumanCarOrWalkable(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadLink,
	const FeaturesPack& walkLink,
	const MultiRowsPack& linkRelationPack
	)
{
	using namespace link_relation;
	using namespace ipc_table;

	m_output.OutputStdErr( err_level::info, err_code::StrChecking, kTableName );

	// ���s�\��ʂɃG���[�����铹�H�����N��ID��ێ����Ă���
	std::set<long> walkableErrRoadLinks;

	long relatedIndex = linkRelationPack.m_FieldMap.FindField( kRelatedLinkID );
	long baseIndex = linkRelationPack.m_FieldMap.FindField( kBaseLinkID );
	long walkableaIndex = roadLink.m_FieldMap.FindField( road_link::kWalkable );
	for ( const auto& linkRelation : linkRelationPack.m_Rows )
	{
		auto& linkRelationObj = linkRelation.second;
		long walkLinkID = linkRelationObj->GetValue( baseIndex ).lVal;
		auto walkIte = walkLink.m_FeatureMap.find( walkLinkID );

		// �R�t�������N���`�F�b�N�Ώۃ��b�V���ɑ��݂��Ȃ��ꍇ�X�L�b�v
		if ( walkIte == walkLink.m_FeatureMap.end() )
			continue;
		if( walkIte->second.IsDisjoint(ipMeshGeo))
			continue;

		long roadLinkID = linkRelationObj->GetValue( relatedIndex ).lVal;
		// ���H�����N������
		auto roadIte = roadLink.m_FeatureMap.find( roadLinkID );
		if ( roadIte == roadLink.m_FeatureMap.end() ) continue;
		auto roadLinkObj = roadIte->second;

		// ���G���[ Buddy�����̂ɕ��s�֎~�ȊO
		// ���H�����N�͕�����Buddy�������߁A������A���l�̃G���[�Ɉ����|����\��������
		// �G���[�Ƃ��ďo�͂���͈̂�x�ł����̂ŁA�R���e�i�ɋl�߂āA����܂Ƃ߂ďo�͂���
		if ( road_link::walkable_class::kNotWalk != roadLinkObj.GetLongValue( walkableaIndex ) )
			walkableErrRoadLinks.insert(roadLinkID);
	}

	// buddy�������s�֎~�̓��H�����N�̃G���[�o��
	outputWalkableErr( walkableErrRoadLinks, roadLink, meshcode );
}

void LinkRelationCheckFunction::checkRelease(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadLink,
	const FeaturesPack& walkLink,
	const MultiRowsPack& linkRelationPack
	)
{
	using namespace link_relation;
	using namespace ipc_table;
	using namespace uh::str_util;

	m_output.SetLayer( walk_link::kTableName );
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, kTableName );

	map<long, IGeometryPtr> relLinks;
	long baseLinkID = 0;

	double maxDistance = -1;
	long relatedIndex = linkRelationPack.m_FieldMap.FindField( kRelatedLinkID );
	long baseIndex = linkRelationPack.m_FieldMap.FindField( kBaseLinkID );
	long operatorIndex = walkLink.m_FieldMap.FindField( kOperator );
	long dateIndex = walkLink.m_FieldMap.FindField( kModifyDate );
	long programIndex = walkLink.m_FieldMap.FindField( kModifyProgName );
	long programDateIndex = walkLink.m_FieldMap.FindField( kProgModifyDate );
	for ( const auto& linkRelation : linkRelationPack.m_Rows )
	{
		auto& linkRelationObj = linkRelation.second;
		long walkLinkID = linkRelationObj->GetValue( baseIndex ).lVal;
		auto walkIte = walkLink.m_FeatureMap.find( walkLinkID );

		// �R�t�������N���`�F�b�N�Ώۃ��b�V���ɑ��݂��Ȃ��ꍇ�X�L�b�v
		if ( walkIte == walkLink.m_FeatureMap.end() )
			continue;
		if( walkIte->second.IsDisjoint(ipMeshGeo))
			continue;

		//linkRelarion.first�͂����Ŏg��ObjectID�ł͂Ȃ�
		long objectId = linkRelationObj->GetOID();
		long roadLinkID = linkRelationObj->GetValue( relatedIndex ).lVal;

		auto& walkLinkObj = walkIte->second;
		m_output.SetModifyInfo( walkLinkID, walkLinkObj.GetStringValue( operatorIndex ), walkLinkObj.GetStringValue( dateIndex ),
			walkLinkObj.GetStringValue( programIndex ), walkLinkObj.GetStringValue( programDateIndex ) );
		// ���H�����N�̃`�F�b�N
		auto roadIte = roadLink.m_FeatureMap.find( roadLinkID );
		if ( roadIte == roadLink.m_FeatureMap.end() ) // ���G���[ �֘A�����NID�����H�����N�ɑ��݂��Ȃ�
		{
			m_output.OutputRegLog( err_level::error, err_code::NotExistRoadLink,
				ToString( objectId ), ToString( roadLinkID ) );
			continue;
		}

		auto& roadLinkObj = roadIte->second;
		// ���G���[ �����N���m������Ă���
		checkLinkDistance( walkLinkObj, roadLinkID, roadLinkObj, objectId );

		if(baseLinkID != walkLinkID)
		{
			if( baseLinkID != 0 )
				checkLinkRelationGroup( walkLink.m_FeatureMap.at( baseLinkID ), walkLink.m_FieldMap, relLinks, maxDistance, true );

			baseLinkID = walkLinkID;
			relLinks.clear();
		}
		relLinks[roadLinkID] = roadLinkObj.m_ipGeometry;
	}

	if ( walkLink.m_FeatureMap.find( baseLinkID ) != walkLink.m_FeatureMap.end() )
		checkLinkRelationGroup( walkLink.m_FeatureMap.at( baseLinkID ), walkLink.m_FieldMap, relLinks, maxDistance, true );

	// Buddy�֌W�ň�ԋ����������ӏ����o��
	if( maxDistance >= 0 )
		m_output.OutputLog( err_level::info, err_code::MaxLinkDistance, 
			uh::str_util::ToString( maxDistance ) );
}


void LinkRelationCheckFunction::checkLinkDistance(
	const GeoClass& walkLinkObj,
	const long roadLinkID,
	const GeoClass& roadLinkObj,
	const long objectId)
{
	using namespace sindy;
	using namespace uh::str_util;

	// [bug12525] ���C�u�������g�p���Ĕ��肷��悤�ύX
	LinkRelationPosition relationInfo;
	if(AheJudgeLinkRelation( walkLinkObj.m_ipGeometry, roadLinkObj.m_ipGeometry, m_buddy_dist_threshold, relationInfo )
		&& relationInfo != kNotFound )
		return;

	m_output.OutputRegLog( err_level::warning, err_code::NgLinkDist,
		ToString( objectId ), ToString( roadLinkID ) );
}

void LinkRelationCheckFunction::checkLinkAngle(
		const GeoClass& walkLinkObj,
		const long roadLinkID,
		const GeoClass& roadLinkObj,
		const long objectId)
{
	if( ContainAngluarDiff(walkLinkObj, roadLinkObj, m_buddy_angle_threshold))
		return;

	m_output.OutputRegLog( err_level::warning, err_code::NgLinkAngle, 
		uh::str_util::ToString( objectId ), uh::str_util::ToString( roadLinkID ) );
}

void LinkRelationCheckFunction::checkLinkRelationGroup(
	const GeoClass& baseLinkObj,
	const sindy::CFieldMap& fieldMap,
	const std::map<long, IGeometryPtr>& relLinks,
	double& maxDistance,
	bool bRelease
	)
{
	using namespace link_relation;
	using namespace ipc_table;
	using namespace uh::str_util;

	m_output.SetModifyInfo( baseLinkObj.m_ObjectID , baseLinkObj.GetStringValue( fieldMap.FindField( kOperator ) ),
		baseLinkObj.GetStringValue( fieldMap.FindField( kModifyDate ) ) , baseLinkObj.GetStringValue( fieldMap.FindField( kModifyProgName ) ) ,
		baseLinkObj.GetStringValue( fieldMap.FindField( kProgModifyDate ) ) );

	// ���G���[ �֘A�����N�Q���ڑ����Ă��Ȃ�
	if( !bRelease && ! ConnectLinks( relLinks ) )
		m_output.OutputRegLog( err_level::warning, err_code::NgConnectRelLinks,
			JoinKeyValue( relLinks ) );

	// bug 11494 - [u][SJ���s��]Buddy���R�t���Ȃ��Ȃ�ꍇ�ɃG���[�Ƃ��Ăق���
	std::list<long> errorRelLinks;
	// ���G���[ �����[�X�ϊ��ŕR�t���Ȃ�Buddy�֌W
	if( ! judge_value::IsOKBuddyRelation( baseLinkObj.m_ipGeometry, relLinks, errorRelLinks, maxDistance ) )
	{
		for ( const auto& errorRelLink : errorRelLinks )
			m_output.OutputRegLog( err_level::error, err_code::NgBuddyRelation, ToString( errorRelLink ) );
	}
}

bool LinkRelationCheckFunction::IsDisjoint( const GeoClass& geo1, const GeoClass& geo2 , double distance)
{
	double dConvRange = 0.0;
	// TODO �C�O�Ŏg�p����ۂɂ͒���
	IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );// m��x�ɕϊ�
	ipUnitConv->ConvertUnits(distance,  esriMeters, esriDecimalDegrees, &dConvRange );

	IGeometryPtr ipGeometryBuf;
	if(FAILED(((ITopologicalOperatorPtr)geo1.m_ipGeometry)->Buffer(dConvRange, &ipGeometryBuf)))
		return false;

	return ( 0 != AheIsDisjoint(ipGeometryBuf,  geo2.m_ipGeometry ) );
}

CString LinkRelationCheckFunction::JoinKeyValue(const map<long, IGeometryPtr>& target)
{
	CString keys;
	for( const auto& rec: target)
	{
		keys += uh::str_util::ToString( rec.first ) + _T(",");
	}
	keys.TrimRight( _T(",") );

	return keys;
}


bool LinkRelationCheckFunction::ConnectLinks(const map<long, IGeometryPtr>& links)
{
	auto ConnectChk = []( const map<long, IGeometryPtr>& roads ){
		// �Ȃ��ꍇ��1�̏ꍇ��OK
		if( roads.size() < 2 )
			return true;

		IGeometryCollectionPtr col( CLSID_GeometryBag );
		ISpatialReferencePtr ipSptRef;
		(roads.begin()->second )->get_SpatialReference(&ipSptRef);
		((IGeometryPtr)col)->putref_SpatialReference( ipSptRef );
		for( const auto& road : roads )
			col->AddGeometry( road.second );

		ITopologicalOperatorPtr topo( CLSID_Polyline );
		((IGeometryPtr)topo)->putref_SpatialReference( ipSptRef );

		if( FAILED(topo->ConstructUnion( (IEnumGeometryPtr)col ) ) )
			return false;

		long num = 0;
		((IGeometryCollectionPtr)topo)->get_GeometryCount( &num );
		if( num > 1 )
			return false;

		return true;
	};

	return ConnectChk( links );

}

bool LinkRelationCheckFunction::ContainAngluarDiff( const GeoClass& geo1, const GeoClass& geo2 , double angle )
{
	double lin1Angle=GetAngle(geo1.m_ipGeometry );
	double lin2Angle=GetAngle(geo2.m_ipGeometry );

	double result = fabs(lin1Angle - lin2Angle);

	// �O�p�̏ꍇ�A���p���m�F�Ώۂɂ���B
	if( result > 180 )
		result = 360 - result;

	return ( result <= angle || 180 - angle <= result );
}

double LinkRelationCheckFunction::GetAngle(const IGeometryPtr& ipGeometry)
{
	IPointPtr ipFromPoint, ipToPoint;

	if( FAILED( ((IPolylinePtr)ipGeometry)->get_FromPoint(&ipFromPoint) ) )
		return -1;
	if( FAILED( ((IPolylinePtr)ipGeometry)->get_ToPoint(&ipToPoint) ) )
		return -1;

	double distX = 0, distY =0;
	g_cnv.GetDistXY(ipFromPoint->GetX(),ipFromPoint->GetY(),ipToPoint->GetX(),ipToPoint->GetY(), &distX, &distY);

	if(distY == 0.0)
		if(distX >= 0.0)
			return 0.0;
		else
			return 180.0;
	if(distX == 0.0)
		if(distY >= 0.0)
			return 90.0;
		else
			return -90.0;

	return ( atan2(distY, distX) * 180.0 / M_PI );
}

// buddy�������s�֎~�̓��H�����N�̃G���[�o��
void LinkRelationCheckFunction::outputWalkableErr(
	const std::set<long>& walkableErrRoadLinks,
	const FeaturesPack& roadLink,
	const CString& meshcode)
{
	using namespace road_link;
	long operatorIndex = roadLink.m_FieldMap.FindField( kOperator );
	long dateIndex = roadLink.m_FieldMap.FindField( kModifyDate );
	long programIndex = roadLink.m_FieldMap.FindField( kModifyProgName );
	long programDateIndex = roadLink.m_FieldMap.FindField( kProgModifyDate );
	for ( long errRoadLinkId : walkableErrRoadLinks )
	{
		auto roadLinkObj = roadLink.m_FeatureMap.at( errRoadLinkId );
		// ���G���[ Buddy�����̂ɕ��s�֎~�ȊO
		m_output.OutputLog( road_link::kTableName, errRoadLinkId,
			err_level::warning, err_code::NgLRRoadLinkWalkable, meshcode,
			roadLinkObj.GetStringValue( operatorIndex ), roadLinkObj.GetStringValue( dateIndex ), roadLinkObj.GetStringValue( programIndex ), roadLinkObj.GetStringValue( programDateIndex ) );
	}
}
