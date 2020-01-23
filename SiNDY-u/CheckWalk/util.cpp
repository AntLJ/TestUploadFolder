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
#include "util.h"
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/enumerate_elements.h>
#include <ArcHelperEx/define.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobals.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheGeometryConvert.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <TDC/sindylib_core/FieldMap.h>
#include "value.h"

using namespace std;
using namespace sindy::schema;

extern crd_cnv g_cnv;

namespace util
{

void createRTree( const map<long, GeoClass>& geoMap, BGRTree& rTree )
{
	for(const auto& geo : geoMap){
		long oID = geo.first;
		BGBox box = geo.second.GetBox();
		rTree.insert( make_pair( box, static_cast<unsigned int>(oID) ) );
	}
}

std::map<CString, IGeometryPtr> getBaseMesh( const std::set<std::string>& meshCodes, ITablePtr table )
{
	std::map<CString, IGeometryPtr> ret;
	// �����p��1000���P�ʂŕ���
	auto meshes = uh::enumerate_elements( meshCodes );

	CString subField;
	subField.Format(_T("%s,%s"), basemesh::kMeshCode, ipc_feature::kShape);
	for(auto& meshSet : meshes)
	{
		CString strWhere;
		strWhere.Format( _T( "%s IN (%s)" ), basemesh::kMeshCode, CString(meshSet.c_str()) );
		IFeatureCursorPtr ipCursor = ( (IFeatureClassPtr)table )->_Search( AheInitQueryFilter( nullptr, subField, strWhere ), VARIANT_FALSE );
		if( !ipCursor )
			return std::map<CString, IGeometryPtr>();

		long meshCodeIndex = ipCursor->_FindField(basemesh::kMeshCode);
		IFeaturePtr ipFeature;
		while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature )
		{
			// �������擾���Amap�Ɋi�[����
			long meshCode = (ipFeature->GetValue( meshCodeIndex )).lVal;
			IGeometryPtr meshGeometry = ipFeature->GetShape();
			ret.emplace( uh::str_util::ToString(meshCode), meshGeometry);
		}
	}
	return ret;
}

bool getLine2Points(const IGeometryPtr &ipLineGeometry, std::vector<IPointPtr>& seqPoint)
{
	// vector�̏�����
	seqPoint.clear();
	IPointCollectionPtr ipPointCollection(ipLineGeometry);
	long pointNum = -1;
	if (FAILED(ipPointCollection->get_PointCount(&pointNum))) {
		return false;
	}
	for( long i = 0; i < pointNum; ++i) {
		IPointPtr ipPoint;
		if (FAILED(ipPointCollection->get_Point(i, &ipPoint))) {
			return false;
		}
		seqPoint.push_back(ipPoint);
	}
	return true;
}

// R-Tree����A�^�������͈̔͂Ɉ���������ID�Q���擾
set<long> getIntersectsIDs( const BGBox& box, const BGRTree& rTree )
{
	set<long> retIDs;
	vector<pair<BGBox, unsigned int>> results;
	bgi::query( rTree, bgi::intersects( box ), std::back_inserter(results) );

	for( const auto& result : results )
		retIDs.insert( static_cast<long>( result.second ) );

	return retIDs;
}

std::set<long> getTargetIDs(
	const FeaturesPack& features
	, LPCTSTR fieldName
	, const std::set<long> fileldValues
	, const IGeometryPtr& meshGeometry )
{
	std::set<long> retIDs;
	long fieldIndex = features.m_FieldMap.FindField( fieldName );
	for ( const auto& feature : features.m_FeatureMap )
	{
		if ( 0 == fileldValues.count( feature.second.GetLongValue( fieldIndex ) ) )
			continue;

		// �����Ώۃ��b�V��������Ȃ����̂̓X�L�b�v
		// ���b�V�����E��̂��̂��擾
		// TODO: ����9���b�V���̃����N������Ă��Ă��邽�߂ɕK�v�ȏ����B
		// �����𒼂���������Ă����B
		if( AheIsDisjoint(meshGeometry, feature.second.m_ipGeometry) )
			continue;

		retIDs.insert(feature.first);
	}
	return retIDs;
}

std::set<long> getTargetIDs(
	const std::map<long, GeoClass>& featureMap
	, const IGeometryPtr& meshGeometry )
{
	std::set<long> retIDs;
	for( const auto& feature : featureMap )
	{
		// �����Ώۃ��b�V��������Ȃ����̂̓X�L�b�v
		// ���b�V�����E��̂��̂��擾
		// TODO: ����9���b�V���̃����N������Ă��Ă��邽�߂ɕK�v�ȏ����B
		// �����𒼂���������Ă����B
		if( AheIsDisjoint(meshGeometry, feature.second.m_ipGeometry ) )
			continue;

		retIDs.insert(feature.first);
	}
	return retIDs;
}


IGeometryPtr getUnionGeometry(
	const std::map<long, GeoClass>& featureMap
	, const std::set<long>& ids )
{
	if(featureMap.empty()) return nullptr;
	if(1==featureMap.size()) return featureMap.begin()->second.m_ipGeometry;

	// �����̃|���S���Ɍׂ��ē����Ă�����̂������ŏ��O
	IGeometryPtr retGeom; // �`���Union��������
	for(auto it = featureMap.begin(); it!=featureMap.end(); ++it)
	{
		if( ids.end()==ids.find(it->first) )
			continue;

		IGeometryPtr geometry = it->second.m_ipGeometry;
		if( !retGeom )
			retGeom = geometry;
		else
		{
			IGeometryPtr tmpGeometry = retGeom;
			((ITopologicalOperatorPtr)tmpGeometry)->Union( geometry, &retGeom );
		}
	}
	return retGeom;
}
IGeometryPtr getUnionGeometry(const std::map<long, const GeoClass*>& featureMap)
{
	if(featureMap.empty()) return nullptr;
	if(1==featureMap.size()) return featureMap.begin()->second->m_ipGeometry;

	// �����̃|���S���Ɍׂ��ē����Ă�����̂������ŏ��O
	IGeometryPtr retGeom; // �`���Union��������
	for(auto it = featureMap.begin(); it!=featureMap.end(); ++it)
	{
		IGeometryPtr geometry = it->second->m_ipGeometry;
		if( !retGeom )
			retGeom = geometry;
		else
		{
			IGeometryPtr tmpGeometry = retGeom;
			((ITopologicalOperatorPtr)tmpGeometry)->Union( geometry, &retGeom );
		}
	}
	return retGeom;
}
IGeometryPtr getUnionGeometry(const std::vector<IPointPtr>& pointList)
{
	if(pointList.empty()) return nullptr;
	if(1==pointList.size()) return *pointList.begin();

	// �����̃|���S���Ɍׂ��ē����Ă�����̂������ŏ��O
	IGeometryPtr retGeom; // �`���Union��������
	for(auto it = pointList.begin(); it!=pointList.end(); ++it)
	{
		IGeometryPtr geometry = *it;
		if( !retGeom )
			retGeom = geometry;
		else
		{
			IGeometryPtr tmpGeometry = retGeom;
			((ITopologicalOperatorPtr)tmpGeometry)->Union( geometry, &retGeom );
		}
	}
	return retGeom;
}


std::vector<IPointPtr> getCrossPoint(IGeometryPtr baseGeometry , IGeometryPtr testGeometry)
{
	std::vector<IPointPtr> ret;
	IPointCollectionPtr ipPointCol = AheGetCrossOrTouchPoints( baseGeometry, testGeometry, 0 );

	if (!ipPointCol)
		return ret;

	long lCount(0);
	ipPointCol->get_PointCount( &lCount );
	for( long i = 0; i < lCount; ++i )
	{
		IPointPtr ipPoint;
		ipPointCol->get_Point( i, &ipPoint );
		if( !ipPoint )
			continue;

		// AheGetCrossOrTouchPoints���������W��2�Ԃ����Ƃ�����̂�
		// ���܂��肽���Ȃ����A�����Ńt�B���^�����O����
		bool canInsert(true);
		for( const auto& r : ret )
		{
			if ( AheIsEqual( r, ipPoint) )
			{
				canInsert = false;
				break;
			}
		}
		if( !canInsert )
			continue;
		ret.push_back( ipPoint );
	}

	return ret;
}

std::set<long> excludeContainLink(
	const std::set<long>& ids
	, const IGeometryPtr& geometry
	, const std::map<long, GeoClass>& featureMap)
{
	if(ids.empty()) return ids;
	std::set<long> retSet;
	for(long featureId : ids)
	{
		const auto& linkInfo = featureMap.at(featureId);
		if( AheIsDisjoint(geometry, linkInfo.m_ipGeometry ) )
		{
			retSet.insert(featureId);
			continue;
		}
		if( AheIsContain4(geometry, linkInfo.m_ipGeometry ) )
			continue;
		if( AheIsTouch(geometry, linkInfo.m_ipGeometry ) )
		{
			retSet.insert(featureId);
			continue;
		}
	}
	return retSet;
}

std::map<long, std::set<long>> toEveryPartLink(
	const std::set<long>& linkIds
	, const FeaturesPack& link)
{
	using namespace sindy::schema::category;

	std::map<long, std::set<long>> result;

	if( linkIds.empty() ) return result;
	if( linkIds.size()==1 )
	{
		result[*linkIds.begin()].insert(*linkIds.begin());
		return result;
	}

	long fromNodeIdIndex = link.m_FieldMap.FindField( from_to_node_relational_table::kFromNodeID );
	long toNodeIdIndex = link.m_FieldMap.FindField( from_to_node_relational_table::kToNodeID );
	// <�����NID, �����NID���瓞�B�ł���m�[�h��OBJECTID>
	typedef std::map<long, std::set<long>> LINK2CONNECTNODE;
	// �����NID��FROM/TO�m�[�hID�̑Ή��\���쐬
	LINK2CONNECTNODE linkId2NodeId;
	for(long id : linkIds)
	{
		auto& fromToId = linkId2NodeId[id];
		fromToId.insert( link.m_FeatureMap.at( id ).GetLongValue( fromNodeIdIndex ) );
		fromToId.insert( link.m_FeatureMap.at( id ).GetLongValue( toNodeIdIndex ) );
	}

	std::pair<long, std::set<long>> targetSet = *linkId2NodeId.begin();
	linkId2NodeId.erase(linkId2NodeId.begin());
	result[targetSet.first].insert(targetSet.first);
	while(!linkId2NodeId.empty())
	{
		// �m�F���̃����N�ɐڑ����Ă��郊���N������
		auto it = std::find_if(linkId2NodeId.begin(), linkId2NodeId.end(),
			[&](const std::pair<long, std::set<long>>& nodeIds)
			{
				for(auto& checkNodeId : targetSet.second)
				{
					if( nodeIds.second.end() != nodeIds.second.find(checkNodeId))
						return true;
				}
				return false;
			});
		// �ڑ����Ă��郊���N���������Ȃ�A���̃����N��OID��FROM/TO�m�[�hID��o�^
		if(linkId2NodeId.end()!=it)
		{
			// �ڑ����Ă��郊���N�Ƃ��ēo�^
			result[targetSet.first].insert(it->first);
			// �ڑ����Ă��郊���N������FROM/TO�m�[�hID���A
			// �m�F���̃����N���瓞�B�\�ȃm�[�h�Ƃ��ēo�^
			for(auto& nodeId : it->second)
				targetSet.second.insert(nodeId);
			// �o�^�ς݂̂��̂͏���
			linkId2NodeId.erase(it);
			continue;
		}
		// �ڑ����Ă��郊���N���Ȃ��Ȃ�A���̃����N���m�F
		targetSet = *linkId2NodeId.begin();
		linkId2NodeId.erase(linkId2NodeId.begin());
		result[targetSet.first].insert(targetSet.first);
	}
	return result;
}


bool existAroundWalkNodeSameName(
	const GeoClass& checkObj,
	const FeaturesPack& checkFeatures,
	const FeaturesPack& walkNode,
	const BGRTree& walkNodeRTree,
	const CString& nameField,
	const CString& yomiField,
	const CString& idField,
	double refer_id_threshold,
	const long checkWalkNodeClass/* =-1*/
	)
{
	using namespace sindyk;

	long nameFieldIndex = checkFeatures.m_FieldMap.FindField( nameField );
	long yomiFieldIndex = checkFeatures.m_FieldMap.FindField( yomiField );
	long idFieldIndex = walkNode.m_FieldMap.FindField( idField );
	long walkNodeFieldIndex = walkNode.m_FieldMap.FindField( walk_node::kNodeClass );
	long poiDeleteFieldIndex = -1;
	if ( walk_node::node_class::kUndergroundGate == checkWalkNodeClass ) {
		poiDeleteFieldIndex = checkFeatures.m_FieldMap.FindField( poi_point_org::kDelete );
	}
	auto name = checkObj.GetStringValue( nameFieldIndex );
	auto yomi = checkObj.GetStringValue( yomiFieldIndex );
	// ���̂�����ꍇ�A���ӂ̕��s�҃m�[�h���擾
	auto findWalkNodeIDs = getIntersectsIDs(checkObj.GetBox(refer_id_threshold), walkNodeRTree);
	for( const auto& walkNodeId : findWalkNodeIDs )
	{
		long checkObjID = -1;
		{ // ���s�҃m�[�h�ɕR�t���t�B�[�`����OBJECTID���擾
			auto walkNodeIt = walkNode.m_FeatureMap.find( walkNodeId );
			if ( walkNode.m_FeatureMap.end() == walkNodeIt )
				continue;

			// ���s�҃m�[�h��ʂ��w�肳�ꂽ���̂łȂ��ꍇ�X�L�b�v
			if ( checkWalkNodeClass != -1
				&& walkNodeIt->second.GetLongValue( walkNodeFieldIndex ) != checkWalkNodeClass )
				continue;
			checkObjID = walkNodeIt->second.GetLongValue( idFieldIndex );
		}
		// ���s�҃m�[�h�ɕR�t���I�u�W�F�N�g���Ȃ��Ȃ�X�L�b�v
		if( checkObjID <= 0 ) continue;

		// ���̂������R�[�h���`�F�b�N
		auto checkObjIt = checkFeatures.m_FeatureMap.find( checkObjID );
		if ( checkFeatures.m_FeatureMap.end() == checkObjIt )
			continue;

		// [bug 11619] �n���X�o�����̏ꍇ�A�폜�R�[�h���u�����v�̂��̂Ƃ�����r���Ȃ�
		if ( walk_node::node_class::kUndergroundGate == checkWalkNodeClass
			&& poi_point_org::delete_code::kAlive != checkObjIt->second.GetLongValue( poiDeleteFieldIndex ) ) 
			continue;

		const auto& cmpName = checkObjIt->second.GetStringValue( nameFieldIndex );
		const auto& cmpYomi = checkObjIt->second.GetStringValue( yomiFieldIndex );

		// ���ꖼ�́A���~�̏ꍇtrue
		if( name.CompareNoCase(cmpName) == 0 && yomi.CompareNoCase(cmpYomi) == 0 )
			return true;
	}
	//���ꖼ�́A���~�����m�[�h��������Ȃ������ꍇfalse
	return false;
}

CComVariant selectModifyTime(
	const CComVariant& modifyDate
	, const CComVariant& programModifyDate)
{
	if(VT_DATE!=modifyDate.vt)
	{
		if(VT_DATE!=programModifyDate.vt) return VT_NULL;
		return programModifyDate;
	}
	if(VT_DATE!=programModifyDate.vt)
		return modifyDate;
	if(modifyDate < programModifyDate)
		return programModifyDate;
	return modifyDate;
}

bool checkModifyTime(
	const CComVariant& modifyFilter
	, const CComVariant& modifyDate
	, const CComVariant& programModifyDate)
{
	if(VT_DATE!=modifyFilter.vt) return true;

	CComVariant date = selectModifyTime(modifyDate, programModifyDate);
	// �ǂ���̍X�V�����ɂ����t�������Ă��Ȃ��Ȃ�A����̓`�F�b�N�Ώ�
	if(VT_DATE!=date.vt) return true;
	// TODO: <= ���Z�q���g�p�ł���肪����Ȃ�C������
	return modifyFilter==date || modifyFilter < date;
}

bool isPointOnLink(IGeometryPtr link, IGeometryPtr point, double range)
{
	IProximityOperatorPtr ipProxy(point);
	double retDis = 0.0;
	ipProxy->ReturnDistance(link, &retDis);
	return retDis <= range;
}

bool isLinkEdgeOnLink(IGeometryPtr checkTargetLink, IGeometryPtr overlapLink, double range)
{
	if(!isPointOnLink( overlapLink, AheGetFromPoint(checkTargetLink), range))
		return false;
	return isPointOnLink( overlapLink, AheGetToPoint(checkTargetLink), range);
}

std::vector<IPointPtr> getOverlapConfigurationPoints(IPolylinePtr checkedLine, IPolylinePtr overlapLine, double range)
{
	if(! (checkedLine && overlapLine) ) return std::vector<IPointPtr>();

	// ���H�����N�̍\���_���擾
	std::vector<IPointPtr> result;
	IPointCollectionPtr linePointColl( checkedLine );
	long linePointCnt = linePointColl->GetPointCount();
	for(long i=0; i<linePointCnt; ++i)
	{
		IPointPtr confPoint = linePointColl->GetPoint( i );
		if( isPointOnLink(overlapLine, confPoint, range) )
			result.push_back(confPoint);
	}
	return result;
}

double GetDistGeoClasses( const GeoClass& geo1, const GeoClass& geo2 )
{
	return GetDistGeometries(geo1.m_ipGeometry, geo2.m_ipGeometry);
}

double GetDistGeometries( const IGeometryPtr& geo1, const IGeometryPtr& geo2 )
{
	double x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;
	IPointPtr ipPoint1( geo1 );
	ipPoint1->QueryCoords( &x1, &y1 );
	IPointPtr ipPoint2( geo2 );
	ipPoint2->QueryCoords( &x2, &y2 );

	return g_cnv.GetDist( x1, y1, x2, y2 );
}


list<GeoClass> GetObjects( long oID, const set<long>& oIDs, const map<long, GeoClass>& geoMap )
{
	list<GeoClass> retList;

	for( const auto& id : oIDs ){

		// ���g��������geoMap����T���āAretList�Ɋi�[
		if( oID == id )
			continue;

		auto itr = geoMap.find( id );
		if( itr == geoMap.end() )
			continue;

		retList.push_back( itr->second );
	}

	return retList;
}
std::map<long, const GeoClass*> GetObjectMap(
	long oID,
	const std::set<long>& oIDs,
	const std::map<long, GeoClass>& geoMap
	)
{
	std::map<long, const GeoClass*> retMap;
	for( const auto& id : oIDs )
	{
		// ���g��������geoMap����T���āAretMap�Ɋi�[
		if( oID == id )
			continue;

		auto itr = geoMap.find( id );
		if( itr == geoMap.end() )
			continue;

		retMap.emplace(itr->first, &itr->second);
	}
	return retMap;
}

long getOtherEdgeNode( long linkId, const FeaturesPack& linkMap, long nodeId )
{
	using namespace category::from_to_node_relational_table;
	auto itTargetLink = linkMap.m_FeatureMap.find( linkId );
	if ( linkMap.m_FeatureMap.end() == itTargetLink )
		return 0;

	auto& targetLinkGeo = itTargetLink->second;

	// �n�_���ƈ�v
	long fromNodeId = targetLinkGeo.GetLongValue( linkMap.m_FieldMap.FindField( kFromNodeID ) );
	if ( fromNodeId == nodeId )
	{
		return targetLinkGeo.GetLongValue( linkMap.m_FieldMap.FindField( kToNodeID ) );
	}
	return fromNodeId;
}

bool isMeshEdgeNode( long nodeId, const FeaturesPack& nodeMap, bool isWalk )
{
	auto itNode = nodeMap.m_FeatureMap.find( nodeId );
	if ( nodeMap.m_FeatureMap.end() == itNode ) return false;

	if ( isWalk )
	{
		return walk_node::node_class::kOutline == itNode->second.GetLongValue( nodeMap.m_FieldMap.FindField( walk_node::kNodeClass ) );
	}
	else
	{
		long nodeIndex = nodeMap.m_FieldMap.FindField( road_node::kNodeClass );
		return road_node::node_class::kEdge == itNode->second.GetLongValue( nodeIndex )
			|| road_node::node_class::kPecEdge == itNode->second.GetLongValue( nodeIndex );
	}
}

std::set<long> getMeshOverLink(
	long linkId, const FeaturesPack& linkMap
	, long& nodeId, const FeaturesPack& nodeMap
	, const NODEIDMAP& nodeLinkIDs, bool isWalk )
{
	std::set<long> preNode; // �v�Z��Œʉ߂����m�[�h
	while(preNode.end()==preNode.find(nodeId))
	{
		long otherId = getOtherEdgeNode( linkId, linkMap, nodeId );
		if ( isMeshEdgeNode( otherId, nodeMap, isWalk ) )
		{
			auto otherConnectIds = nodeLinkIDs.find(otherId);
			// �񍷘H�łȂ��Ȃ������_�ŒT���͐؂�
			// (�񎟃��b�V�����E�ɑ΂��ē񍷘H�ŕ��򂵂Ă��邱�ƂɂȂ�̂ŁA�ʏ�f�[�^�ł͂��蓾�Ȃ�)
			if(otherConnectIds->second.size()!=2)
				return std::set<long>();

			auto otherLinks = GetObjectMap( linkId, otherConnectIds->second, linkMap.m_FeatureMap );
			linkId = otherLinks.begin()->first;

			// �T���ς݃m�[�h��o�^���A���̃����N�̒[�_���m�F����
			preNode.insert(nodeId);
			nodeId = otherId;
			continue;
		}
		auto otherConnectIds = nodeLinkIDs.find(otherId);
		auto otherLinks = GetObjectMap( linkId, otherConnectIds->second, linkMap.m_FeatureMap );
		std::set<long> retSet;
		for(auto& elem : otherLinks)
			retSet.insert(elem.first);

		// �T���̏I�_�m�[�h�Œ[�_�m�[�h�����X�V����
		nodeId = otherId;
		return retSet;
	}
	return std::set<long>();
}

bool isFromNode( long linkId, const FeaturesPack& linkMap, long nodeId )
{
	using namespace category::from_to_node_relational_table;
	auto it = linkMap.m_FeatureMap.find( linkId );
	if ( linkMap.m_FeatureMap.end() == it ) return false;
	return nodeId == it->second.GetLongValue( linkMap.m_FieldMap.FindField( kFromNodeID ) );
}

std::set<long> getAdjacentLink(
	long& nodeId
	, const std::map<long, const GeoClass*>& adjacentLink
	, const FeaturesPack& link
	, const FeaturesPack& node
	, const NODEIDMAP& nodeLinkIDs
	, bool isWalk
)
{
	std::set<long> result;
	if ( isMeshEdgeNode( nodeId, node, isWalk ) )
	{
		// 2�����b�V���ڑ��m�[�h�ł���Ƃ������Ƃ́A���ʁAadjacentLink�͈�_�B
		// �����A�s���f�[�^�������ꍇ���������肦��̂ŁA�����O��ł����
		for(auto& linkInfo : adjacentLink)
		{
			// nodeId���T���̏I�_�m�[�h�ɍX�V�����̂ŁA�v����
			std::set<long> meshOverId = getMeshOverLink( linkInfo.first, link, nodeId, node, nodeLinkIDs, isWalk );
			for(long linkId : meshOverId)
				result.insert(linkId);
		}
	}
	else
	{
		for(auto& linkInfo : adjacentLink)
			result.insert(linkInfo.first);
	}
	return result;
}

long GetExistMeshcode(
	const GeoClass& geoClass,
	const std::map<long, std::pair<IGeometryPtr,IGeometryPtr>>& cityMeshMap )
{
	for( const auto& cityMesh : cityMeshMap )
	{
		if( ! geoClass.IsDisjoint( cityMesh.second.first ) )
			return cityMesh.first;
	}
	return 0;
}

bool isOnlyTargetWalkLink( const std::map<long, GeoClass>::iterator& nodeMapIt, long linkId, const NODEIDMAP& nodeLinkIDs, long nodeClassIndex )
{
	// �w��m�[�h�ɕR�t�������N��1���ł͂Ȃ��ꍇ�Afalse
	auto linkIDSet = nodeLinkIDs.find( nodeMapIt->first )->second;
	if ( 1 != linkIDSet.size() )
		return false;
	


	// �w��m�[�h���[�_�m�[�h�̏ꍇ�́A��������1���݂̂ƂȂ�̂ŁAfalse
	if ( walk_node::node_class::kTerminal == nodeMapIt->second.GetLongValue( nodeClassIndex ) )
		return false;

	// �w��m�[�h�ɕR�t�������N��1���݂̂̏ꍇ�Atrue
	if ( linkId == (*linkIDSet.begin()) )
		return true;

	return false;
}

bool isOnlyTargetRoadLink( const std::map<long, GeoClass>::iterator& nodeMapIt, long linkId, const NODEIDMAP& nodeLinkIDs, long nodeClassIndex )
{
	// �w��m�[�h�ɕR�t�������N��1���ł͂Ȃ��ꍇ�Afalse
	auto linkIDSet = nodeLinkIDs.find( nodeMapIt->first )->second;
	if ( 1 != linkIDSet.size() )
		return false;
	
	// �w��m�[�h���[�_�m�[�h�̏ꍇ�́A��������1���݂̂ƂȂ�̂ŁAfalse
	if ( road_node::node_class::kTerminal == nodeMapIt->second.GetLongValue( nodeClassIndex ) )
		return false;

	// �w��m�[�h�ɕR�t�������N��1���݂̂̏ꍇ�Atrue
	if ( linkId == (*linkIDSet.begin()) )
		return true;

	return false;
}

bool GetLinkFeaturesWithNodeIdInGeometry(
	const IGeometryPtr& ipGeometry,
	const FIELD_LIST& fieldList,
	const CString& strWhere,
	FeaturesPack& features,
	NODEIDMAP& nodeNodeIDs,
	NODEIDMAP& nodeLinkIDs )
{
	if ( !features.m_ipTable )
		return false;

	CString subField = uh::str_util::join( fieldList, _T( "," ) );
	IFeatureCursorPtr ipCursor = features.m_ipTable->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipGeometry ), subField.IsEmpty() ? NULL : (LPCTSTR)subField, strWhere ), VARIANT_FALSE );
	if ( !ipCursor )
		return false;
	long fromIdIndex = features.m_FieldMap.FindField(  category::from_to_node_relational_table::kFromNodeID );

	long toIdIndex = features.m_FieldMap.FindField( category::from_to_node_relational_table::kToNodeID );

	if ( fromIdIndex < 0 || toIdIndex < 0 ) {
		return false;
	}
	IFeaturePtr ipFeature;

	while ( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ) {
		// �������擾���Amap�Ɋi�[����
		long keyValue = ipFeature->GetOID();

		GeoClass geoClass(ipFeature);

		long fromNodeID = ipFeature->GetValue( fromIdIndex ).lVal;
		long toNodeID = ipFeature->GetValue( toIdIndex ).lVal;

		nodeNodeIDs[fromNodeID].insert( toNodeID );
		nodeNodeIDs[toNodeID].insert( fromNodeID );

		nodeLinkIDs[fromNodeID].insert( keyValue );
		nodeLinkIDs[toNodeID].insert( keyValue );

		features.m_FeatureMap[keyValue] = move( geoClass );
	}

	return true;
}
bool GetLinkFeaturesInGeometry(
	const IGeometryPtr& ipGeometry,
	const FIELD_LIST& fieldList,
	const CString& strWhere,
	FeaturesPack& features,
	NODEIDMAP& nodeLinkIDs )
{
	if ( !features.m_ipTable )
		return false;

	CString subField = uh::str_util::join( fieldList, _T( "," ) );
	IFeatureCursorPtr ipCursor = features.m_ipTable->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipGeometry ), subField.IsEmpty() ? NULL : (LPCTSTR)subField, strWhere ), VARIANT_FALSE );
	if ( !ipCursor )
		return false;
	long fromIdIndex = features.m_FieldMap.FindField( category::from_to_node_relational_table::kFromNodeID );

	long toIdIndex = features.m_FieldMap.FindField( category::from_to_node_relational_table::kToNodeID );

	if ( fromIdIndex < 0 || toIdIndex < 0 ) {
		return false;
	}
	IFeaturePtr ipFeature;

	while ( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ) {
		// �������擾���Amap�Ɋi�[����
		long keyValue = ipFeature->GetOID();

		GeoClass geoClass( ipFeature );

		long fromNodeID = ipFeature->GetValue( fromIdIndex ).lVal;
		long toNodeID = ipFeature->GetValue( toIdIndex ).lVal;

		nodeLinkIDs[fromNodeID].insert( keyValue );
		nodeLinkIDs[toNodeID].insert( keyValue );

		features.m_FeatureMap[keyValue] = move( geoClass );
	}

	return true;
}

bool GetFeaturesInGeometry(
	const IGeometryPtr& ipGeometry,
	const FIELD_LIST& fieldList,
	const CString& strWhere,
	FeaturesPack& features )
{
	CString subField = uh::str_util::join( fieldList, _T( "," ) );
	IFeatureCursorPtr ipCursor = features.m_ipTable->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipGeometry ), subField.IsEmpty() ? NULL : (LPCTSTR)subField, strWhere ), VARIANT_FALSE );
	if ( !ipCursor )
		return false;

	IFeaturePtr ipFeature;
	while ( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ) {
		GeoClass geoClass(ipFeature);
		features.m_FeatureMap[geoClass.m_ObjectID] = move( geoClass );
	}
	return true;
}
// �w��ID���L���b�V�����猟�����A�Ȃ���΃e�[�u���S�̂��猟������
// WalkLinkCheckFunction�́usearchEdgeNode�v�O���ėp����������
GeoClass SearchObj( const FeaturesPack& features, long id )
{

	auto objIterator = features.m_FeatureMap.find( id );
	if ( objIterator == features.m_FeatureMap.end() )
	{
		// [bug11701]���s�҃����N��2�̓s�s�n�}���b�V�����ׂ��ꍇ�A
		// �n�_�E�I�_���אڃ��b�V���O�ɑ��݂���\��������̂ŁA�S�̂ɑ���������������
		_IRowPtr row;
		if (FAILED( features.m_ipTable->GetRow( id, &row )) || !row )
		{
			return GeoClass();
		}
		return GeoClass( row );
	}
	else
	{
		return objIterator->second;
	}
}

long getDifferenceRoadLinkAndWalkLinkCount( std::set<long>checkedLinkIds, std::vector<long>walkLinkIds, const FeaturesPack&roadLink, const FeaturesPack&walkLink)
{
	long sameLinkCount(checkedLinkIds.size());
	for(const auto& link: checkedLinkIds)
	{
		auto linkItr = roadLink.m_FeatureMap.find( link );
		if( linkItr == roadLink.m_FeatureMap.end() )
			continue;

		for(const auto& walk:walkLinkIds)
		{
			if(walkLink.m_FeatureMap.find(walk) == walkLink.m_FeatureMap.end())
				continue;
			auto fromGeo = walkLink.m_FeatureMap.at(walk);
			if( !AheIsExactEqual(linkItr->second.m_ipGeometry, fromGeo.m_ipGeometry) )
				continue;
			--sameLinkCount;
			break;
		}
	}
	return sameLinkCount;
}

// �`�F�b�N�ΏۂɌ����Ȃ�|���S��������
long findOverlapGeometry(
	const GeoClass& geoObject,
	const FeaturesPack& pack,
	const BGRTree& rtree,
	std::vector<GeoClass>& crossObjects,
	double range
	)
{
	// R-Tree����A�^�������͈̔͂Ɉ���������ID�Q���擾
	auto findIDs = util::getIntersectsIDs( geoObject.GetBox( range ), rtree );

	// �����������[�v
	for( const auto& findID : findIDs )
	{
		auto findItr = pack.m_FeatureMap.find( findID );
		if ( findItr == pack.m_FeatureMap.end() )
			continue;

		VARIANT_BOOL disjoint(VARIANT_FALSE);
		if (SUCCEEDED(((IRelationalOperatorPtr)geoObject.m_ipGeometry)->Disjoint(findItr->second.m_ipGeometry, &disjoint)) && disjoint == VARIANT_FALSE) 
		{
			GeoClass crossObject;
			crossObject.m_ipGeometry = findItr->second.m_ipGeometry;
			crossObject.m_ObjectID = findItr->second.m_ObjectID;
			crossObjects.push_back( crossObject );
		}
	}
	return crossObjects.size();
}

} // util
