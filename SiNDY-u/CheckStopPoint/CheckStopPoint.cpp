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
#include "CheckStopPoint.h"
#include "util.h"
#include <boost/range/algorithm.hpp>
#include <sindy/workspace.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheLayerFunctions.h>
#include <TDC/useful_headers/str_util.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace uh::str_util;

namespace
{
	crd_cnv g_crd;

	/**
	 * @brief �n�_�E�I�_�Ԃ̋��������[�g���P�ʂŕԂ��֐�
	 * @param ipFrom [in] �n�_
	 * @param ipTo   [in] �I�_
	 * @retrun �n�_�E�I�_�Ԃ̋���[m]
	 */
	double GetLengthBetween2Point( const IPointPtr& ipFrom, const IPointPtr& ipTo )
	{
		double dX1=0,dY1=0,dX2=0,dY2=0;
		ipFrom->QueryCoords( &dX1, &dY1 );
		ipTo->QueryCoords( &dX2, &dY2 );
		return g_crd.GetDist( dX1, dY1, dX2, dY2 );
	}

	/**
	 * @brief �|�����C���̒��������[�g���ŕԂ��֐�
	 * @param ipLine [in] �|�����C��
	 * @return �|�����C���̒���[m]
	 */
	double GetLineLength( const IPolylinePtr& ipLine )
	{
		double dLength = 0;
		IPointCollectionPtr ipPointCol = ipLine;
		long lPointCol = ipPointCol->GetPointCount();
		for( int i = 0; i < lPointCol-1; ++i )
		{
			IPointPtr ipPoint1, ipPoint2;
			ipPointCol->get_Point( i, &ipPoint1 );
			ipPointCol->get_Point( i+1, &ipPoint2 );
			dLength += GetLengthBetween2Point( ipPoint1, ipPoint2 );
		}
		return dLength;
	}

	/**
	 * @brief �I�[�v���������[�N�X�y�[�X���擾
	 * @param  strWorkspaceName [in] �I�[�v�����郏�[�N�X�y�[�X��
	 * @retval IWorkspacePtr ���[�N�X�y�[�X�̃I�[�v���ɐ���
	 * @retval nullptr       ���[�N�X�y�[�X�̃I�[�v���Ɏ��s
	 */
	inline IWorkspacePtr openWorkspace(const CString& strWorkspaceName)
	{
		IWorkspacePtr ipWorkspace = create_workspace( strWorkspaceName );
		if( !ipWorkspace ){
			cerr << "#ERROR ���[�N�X�y�[�X�̃I�[�v���Ɏ��s�F" << CT2CA( strWorkspaceName ) << endl;
			return nullptr;
		}
		return ipWorkspace;
	}

	/**
	 * @brief �G���[���O���o�͂��邽�߂̊֐�
	 * @param logM         [in] ���O�o�͊Ǘ��C���X�^���X
	 * @param featureName  [in] ���[�U�[���t���t�B�[�`���N���X��
	 * @param stopPoint    [in] �G���[�`�F�b�N�Ώۈꎞ��~�|�C���g�̏������\����
	 * @param stopPointOID [in] �G���[�`�F�b�N�Ώۈꎞ��~�|�C���g�̃I�u�W�F�N�gID
	 * @param errorCode    [in] �G���[�R�[�h
	 * @param meshcode     [in] �`�F�b�N���̃��b�V���R�[�h
	 */
	inline void writeErrorLog(CLogManager& logM, const CStringA& featureName, const CheckStopPoint::STOP& stopPoint, long stopPointOID, long errorCode, long meshcode)
	{
		logM.WriteCheckLog( featureName, stopPointOID, errorCode,
			stopPoint.ipPoint->GetX(), stopPoint.ipPoint->GetY(),
			meshcode, stopPoint.linkID, stopPoint.nodeID );
	}

}

bool CheckStopPoint::init()
{
	m_logManager.Init( m_strOutput );

	if( !util::loadMeshList( m_strMeshlist, m_meshList ) ){
		cerr << "#ERROR ���b�V�����X�g���J���܂���F" << CT2CA( m_strMeshlist ) << endl;
		return false;
	}

	// ���[�N�X�y�[�X���I�[�v������
	if( !(m_ipRoadWorkspace     = openWorkspace( m_strDBRoad )) )      return false;
	if( !(m_ipStopWorkspace     = openWorkspace( m_strDBStop )) )      return false;
	if( !(m_ipMeshWorkspace     = openWorkspace( m_strDBMesh )) )      return false;

	// �t�B�[�`���N���X���I�[�v������
	if( !OpenFeatureClass( road_link::kTableName,    m_ipRoadWorkspace,     m_ipRoadLinkFC,       m_RoadLinkFM      ) ) return false;
	if( !OpenFeatureClass( road_node::kTableName,    m_ipRoadWorkspace,     m_ipRoadNodeFC,       m_RoadNodeFM      ) ) return false;
	if( !OpenFeatureClass( stop_point::kTableName,   m_ipStopWorkspace,     m_ipStopPointFC,      m_StopPointFM     ) ) return false;
	if( !OpenFeatureClass( basemesh::kTableName,     m_ipMeshWorkspace,     m_ipBaseMeshFC,       m_BaseMeshFM      ) ) return false;

	// �`�F�b�N���[�h�̏ꍇ�͌o�N�ω��O�̓��HDB���I�[�v������
	if( m_check )
	{
		if( !(m_ipRoadPastWorkspace = openWorkspace( m_strDBRoad_past )) ) return false;
		if( !OpenFeatureClass( road_link::kTableName,    m_ipRoadPastWorkspace, m_ipRoadLinkFC_past,  m_RoadLinkFM_past ) ) return false;
		if( !OpenFeatureClass( road_node::kTableName,    m_ipRoadPastWorkspace, m_ipRoadNodeFC_past,  m_RoadNodeFM_past ) ) return false;
	}

	return true;
}

// �t�B�[�`���N���X�I�[�v�����t�B�[���h�}�b�v�쐬
bool CheckStopPoint::OpenFeatureClass(const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, CFieldMap& fieldMap )
{
	((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t( name ), &ipFeatureClass );
	if( !ipFeatureClass ){
		cerr << "#ERROR �t�B�[�`���N���X�̃I�[�v���Ɏ��s�F" << CT2CA( name ) << endl;
		return false;
	}
	fieldMap.CreateFieldMap( ipFeatureClass->GetFields() );

	return true;
}

// �e�[�u���I�[�v�����t�B�[���h�}�b�v�쐬
bool CheckStopPoint::OpenTable(const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, CFieldMap& fieldMap )
{
	((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( _bstr_t( name ), &ipTable );
	if( !ipTable ){
		cerr << "#ERROR �e�[�u���̃I�[�v���Ɏ��s�F" << CT2CA( name ) << endl;
		return false;
	}
	fieldMap.CreateFieldMap( ipTable->GetFields() );

	return true;
}

bool CheckStopPoint::execute()
{
	if( !init() )
		return false;

	if( m_extract )
	{
		m_logManager.WriteExtractLogHeader();
		m_logManager.WriteInfo( "#INFO ���o���[�h���s" );
	}
	if( m_check )
	{
		m_logManager.WriteCheckLogHeader();
		m_logManager.WriteInfo( "#INFO �`�F�b�N���[�h���s" );
	}
	m_logManager.WriteInfo( "#INFO ���H�nDB				�F%s", (CStringA)m_strDBRoad );
	if( !m_strDBRoad_past.IsEmpty() )
		m_logManager.WriteInfo( "#INFO ���H�nDB(�o�N�ω��O)	�F%s", (CStringA)m_strDBRoad_past );
	m_logManager.WriteInfo( "#INFO �ꎞ��~DB			�F%s", (CStringA)m_strDBStop );
	m_logManager.WriteInfo( "#INFO ���b�V��DB			�F%s", (CStringA)m_strDBMesh );
	m_logManager.WriteInfo( "#INFO �G���[���O�t�@�C����	�F%s", (CStringA)m_strOutput );
	m_logManager.WriteInfo( "#INFO ���b�V�����X�g		�F%s", (CStringA)m_strMeshlist );

	// ���b�V�����Ƃ̏���
	for( const auto& mesh : m_meshList ){
		m_logManager.WriteInfo( "#%d", mesh );
		cerr << "#" << mesh << endl;
		LINKMAP linkMap;
		NODEMAP nodeMap;

		// ���b�V���|���S���擾
		IGeometryPtr ipMeshGeo;
		if( !GetMeshPolygon( mesh, ipMeshGeo ) ){
			m_logManager.WriteInfo( "#FATAL ���b�V���|���S���擾���s" );
			return false;
		}

		// ���b�V�����̈ꎞ��~���擾
		STOPMAP stops;
		if( !GetStopInMesh( ipMeshGeo, stops ) ){
			m_logManager.WriteInfo( "#FATAL �����ꎞ��~�̎擾���s" );
			return false;
		}

		// ���b�V�����̓��H�����N���擾
		if( !GetLinkNodeInMesh( ipMeshGeo, linkMap, nodeMap, m_ipRoadLinkFC, m_RoadLinkFM, m_ipRoadNodeFC, m_RoadNodeFM ) ){
			m_logManager.WriteInfo( "#FATAL ���b�V�����̓��H�����N�擾���s" );
			return false;
		}

		// ���o����
		if( m_extract )
			Extract( linkMap, nodeMap, stops, mesh );
		// �`�F�b�N����
		if( m_check )
		{
			// �o�N�ω��O�̌`���ǂݍ���ł���
			LINKMAP linkMap_past;
			NODEMAP nodeMap_past;
			// ���b�V�����̌o�N�ω��O�̓��H�����N���擾
			if( !GetLinkNodeInMesh( ipMeshGeo, linkMap_past, nodeMap_past, m_ipRoadLinkFC_past, m_RoadLinkFM_past, m_ipRoadNodeFC_past, m_RoadNodeFM_past ) ){
				m_logManager.WriteInfo( "#FATAL ���b�V�����̌o�N�ω��O�̓��H�����N�擾���s" );
				return false;
			}
			ErrorCheck( linkMap, nodeMap, linkMap_past, nodeMap_past, stops, mesh );
		}
	}

	m_logManager.WriteInfo( "#INFO ����I��" );
	
	return true;
}

void CheckStopPoint::Extract( const LINKMAP& linkMap, const NODEMAP& nodeMap, const STOPMAP& stops, long mesh )
{
	// TODO: �m�F�p�Ȃ̂Ō�ō폜����OK
	auto& domainValue = m_RoadLinkFM.GetDomain( road_link::kRoadClass );
	map<long, CString> roadClassName;
	for( auto& value : domainValue ){
		// �Ȃ��ɃL�[��������Ȃ񂾂낤
		roadClassName[value.second] = value.first;
	}

	// �e�m�[�h���Ƃ̏���
	for( auto& node : nodeMap ){

		// ���b�V�����E�m�[�h�̏ꍇ�A�X�L�b�v
		if( IsMeshEdge(node.second) )
			continue;

		long nodeID = node.first;

		map<long, LINK> okLinks;
		list<long> mainUpDownLinks;
		
		// �R�Â��Ă��郊���N���̏���
		for( auto& linkID : node.second.linkIDs ){
			
			// �����N�����擾
			auto& link = linkMap.find( linkID );

			// �擾�ł��Ȃ����̂́A���b�V���O�̂��̂Ȃ̂ŃX�L�b�v
			if( link == linkMap.end() )
				continue;

			// ���b�V���O�̂���(�ڂ��Ă��邾���̂���)�̓X�L�b�v
			if( link->second.outOfMesh )
				continue;

			long roadClass = link->second.roadClass; //!< ���O�p

			// �����ꎞ��~�ɑ��݂��邩�H
			if( FindStopPoint( linkID, nodeID, stops ) ){
				CStringA strLog;
				strLog.Format( "%d\t%d(%s)" , linkID, roadClass, CStringA(roadClassName.at( roadClass )) );
				m_logManager.WriteExtractLog( road_node::kTableName, nodeID, mesh, "WARNING", "�����ꎞ��~�ɑ���", strLog );
				continue;
			}

			// �K���͖��Ȃ����H
			if( !IsRegOK( link->second, nodeID ) ){
				CStringA strLog;
				strLog.Format( "%d\t%d(%s)" , linkID, roadClass, CStringA(roadClassName.at( roadClass )) );
				m_logManager.WriteExtractLog( road_node::kTableName, nodeID, mesh, "WARNING", "����ʍs/�ʍs�֎~�ɖ�肠��", strLog );
				continue;
			}

			// ���o�Ώۂ��H
			if( !IsExtractTarget( link->second, nodeID, linkMap, nodeMap ) ){
				CStringA strLog;
				strLog.Format( "%d\t%d(%s)" , linkID, roadClass, CStringA(roadClassName.at( roadClass )) );
				m_logManager.WriteExtractLog( road_node::kTableName, nodeID, mesh, "WARNING", "���o�����𖞂����Ȃ�", strLog );
				continue;
			}

			// --�����܂łŒ��o�ΏۊO��e��

			// OK�Ȃ��̂͂ЂƂ܂��i�[���Ă���
			okLinks[linkID] = link->second;

			// 2�𓹘H���Ȃ��\�������郊���N�͂����ŕێ����Ă���
			// ���̂Ƃ���㉺���������N�̂�
			if( road_link::main_link_class::kMainUpDown == link->second.linkClass )
				mainUpDownLinks.push_back( linkID );
		}

		// 2�𓹘H���Ȃ����̂������Ă���ꍇ�́A������OK���珜�O
		if( !mainUpDownLinks.empty() ){
			// TODO: �p�x�⑼�̏������K�v�Ȃ炱���ɒǋL
			CString strLog;
			for( auto& mainUpDownID : mainUpDownLinks ){
				auto itr = okLinks.find( mainUpDownID );
				if( okLinks.end() != itr ){
					strLog.AppendFormat( _T("%d\t%d(%s)\t"), mainUpDownID, itr->second.roadClass, roadClassName.at( itr->second.roadClass ) );
					okLinks.erase( itr );
				}
			}
			m_logManager.WriteExtractLog( road_node::kTableName, nodeID, mesh, "WARNING", "2�𓹘H�̂��ߏ��O", CStringA(strLog) );
		}

		if( !( okLinks.empty() ) ){
			CString strLog;
			for( auto& ok : okLinks )
				strLog.AppendFormat( _T("%d\t%d(%s)\t"), ok.first, ok.second.roadClass, roadClassName.at( ok.second.roadClass ) );
			m_logManager.WriteExtractLog( road_node::kTableName, nodeID, mesh, "INFO", "���o", CStringA(strLog) );
		}
	}
}

// �s���Ȉꎞ��~�|�C���g�����o����
void CheckStopPoint::ErrorCheck( const LINKMAP& linkMap, const NODEMAP& nodeMap, const LINKMAP& linkMap_past, const NODEMAP& nodeMap_past, const STOPMAP& stops, long mesh )
{
	// ���O�ɏo�͂���I�[�i�[�t���t�B�[�`���N���X����p�ӂ��Ă���
	CStringA stopPointName( AheGetOwnerDotClassName(m_ipStopPointFC) );

	for( auto stopPairIt=stops.begin(); stopPairIt!=stops.end(); ++stopPairIt )
	{
		const STOP& targetPoint = stopPairIt->second;

		// ���������N�E���o�m�[�h���������̃��b�V�����ɑ��݂��邩�m�F
		// ���������N�ɂ��ẮA���b�V���ɐڂ��Ă��邾���̂��̂͑��݂��Ȃ������Ƃ���
		bool bLinkExist = (linkMap.find(targetPoint.linkID)!=linkMap.end() && !linkMap.at(targetPoint.linkID).outOfMesh);
		bool bNodeExist = nodeMap.find(targetPoint.nodeID)!=nodeMap.end();

		// �ȉ��AHC1000�ԑ�̃G���[�o��
		// ���ݎQ�ƒ��̃|�C���g�̎��̃|�C���g����Ō�̃|�C���g�܂Ŕ�r
		for(auto compPointIt = std::next(stopPairIt); compPointIt!=stops.end(); ++compPointIt)
		{
			// HC1001 �����̃|�C���g������n�_�Ɉʒu
			if( AheIsEqual(targetPoint.ipPoint, compPointIt->second.ipPoint) )
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 1001, mesh );
			// HC1002 �����̃|�C���g���ʒu����n�_�͓���ł͂Ȃ����A����(���������N�A���o�m�[�h)�̒l������
			else if( targetPoint==compPointIt->second )
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 1002, mesh );
		}

		// HC1003 ���������N���������̈�ʂ̋t���A�������ʋցE�ʍs�s���H
		if( bLinkExist && !IsRegOK( linkMap.at(targetPoint.linkID), targetPoint.nodeID ) )
			writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 1003, mesh );

		// HC1004 ���o�m�[�h���A�M���@��������_�m�[�h
		if( bNodeExist && IsSignalCross(nodeMap.at(targetPoint.nodeID)) )
			writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 1004, mesh );

		// HC1005 �ꎞ��~�|�C���g�Ɋ֘A�t�����������N�̌`�󂪌o�N�ω�or�����N��ɑ��݂��Ȃ��ꍇ�ɃG���[
		if( bLinkExist && bNodeExist )
		{
			const LINK& lLink = linkMap.at(targetPoint.linkID);
			const NODE& lNode = nodeMap.at(targetPoint.nodeID);
			bool bError = false;
			// ���������N��Ɉꎞ��~�|�C���g�����݂��Ȃ��ꍇ�A�G���[�Ƃ���
			IRelationalOperatorPtr ipRel( lLink.ipGeo );
			if( ipRel->_Disjoint( targetPoint.ipPoint ) != VARIANT_FALSE )
				bError = true;
			else
			{
				// �ꎞ��~�t�^�c�[���Ɠ����A���S���Y���Ń|�C���g�𐶐�
				IGeometryPtr ipCompGeo = CreateStopPointGeometry(targetPoint.linkID, targetPoint.nodeID, lLink, lNode, lNode.ipGeo->GetSpatialReference());
				// �t�^���ƈႤ�n�_�ɂł��Ă���Ȃ�A�G���[�Ƃ���
				if( !AheIsEqual(targetPoint.ipPoint,ipCompGeo) )
					bError = true;
			}
			// �����ꂩ��̃G���[�����𖞂����Ă����Ȃ�A1005�G���[���o�͂���
			if( bError )
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 1005, mesh );
		}

		if( bNodeExist && ! IsCross( nodeMap.at( targetPoint.nodeID ) ) )
		{
			// HC1006 ���o�m�[�h��2�����b�V�����E�m�[�h
			if( IsMeshEdge( nodeMap.at( targetPoint.nodeID ) ) )
				writeErrorLog( m_logManager, stopPointName, targetPoint, stopPairIt->first, 1006, mesh );
			// HC1010 ���o�m�[�h�������_�m�[�h�ȊO
			else
				writeErrorLog( m_logManager, stopPointName, targetPoint, stopPairIt->first, 1010, mesh );
		}
		
		// HC1007 ���������N��������_�����N
		if( bLinkExist && IsRoundAbout( linkMap.at(targetPoint.linkID) ) )
			writeErrorLog( m_logManager, stopPointName, targetPoint, stopPairIt->first, 1007, mesh );

		// HC1008 ���������N���{���i�㉺�����j�����N
		if( bLinkExist && IsMainUpDown( linkMap.at(targetPoint.linkID) ) )
			writeErrorLog( m_logManager, stopPointName, targetPoint, stopPairIt->first, 1008, mesh );

		// HC1009 ���������N��HWY�{��
		if( bLinkExist && IsHighwayMain( linkMap.at(targetPoint.linkID) ) )
			writeErrorLog( m_logManager, stopPointName, targetPoint, stopPairIt->first, 1009, mesh );

		//�ȉ� HC 2000�ԑ�̃G���[���o
		if( !bLinkExist )
		{
			// HC2001 �m�[�h�͑��݂��A�����N�͑��݂��Ȃ�
			if( bNodeExist )
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 2001, mesh );
			else // HC2003 �����N�E�m�[�h�����݂��Ȃ�
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 2003, mesh );
		}
		else if( !bNodeExist )  // HC2002 �����m�[�h�����݂��Ȃ�
			writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 2002, mesh );

		// HC2005 �w���ID�̗��������N�A����ї��o�m�[�h�����݂��邪�A�`�󂪈�v���Ȃ�
		if( bLinkExist && bNodeExist )
		{
			// ���������N��FromID�Ƃ�ToID�Ƃ����o�m�[�h����v���Ȃ�
			if( linkMap.at(targetPoint.linkID).fromID != targetPoint.nodeID &&
				linkMap.at(targetPoint.linkID).toID != targetPoint.nodeID )
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 2004, mesh );

			// �o�N�ω��O���NW�`�󂪈قȂ�ꍇ�̏o��(ERROR_CODE:2005)�͏��O���ꂽ�ibug 11276�j
		}
	}
}

// ���b�V���|���S���擾
bool CheckStopPoint::GetMeshPolygon( long mesh, IGeometryPtr& ipMeshGeo )
{
	// ���b�V���R�[�h���烁�b�V���|���S���擾
	CString strWhere;
	strWhere.Format( _T("%s = %d"), basemesh::kMeshCode, mesh );
	IFeatureCursorPtr ipMeshCursor = m_ipBaseMeshFC->_Search( AheInitQueryFilter( NULL, NULL, strWhere ), VARIANT_FALSE );
	if( !ipMeshCursor )
		return false;
	IFeaturePtr ipMeshFeature;
	if( S_OK == ipMeshCursor->NextFeature( &ipMeshFeature ) && ipMeshFeature ){

		ipMeshGeo = ipMeshFeature->GetShapeCopy();
	}

	return ( ipMeshGeo != nullptr );
}

// ���b�V�����̊����ꎞ��~���擾
bool CheckStopPoint::GetStopInMesh( const IGeometryPtr& ipMeshGeo, STOPMAP& stops )
{
	CString subField;
	subField.Format( _T("%s,%s,%s,SHAPE"), kObjectID, stop_point::kLinkID, stop_point::kNodeID );
	IFeatureCursorPtr ipCursor = m_ipStopPointFC->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_TRUE );
	if( !ipCursor )
		return false;
	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ){

		STOP stop;
		stop.ipPoint = IPointPtr(ipFeature->GetShapeCopy());
		if( !stop.ipPoint )
		{
			CStringA strError;
			strError.Format("#FATAL �ꎞ��~�|�C���g���s���Ȍ`�� : OBJECTID[%ld]", ipFeature->OID);
			m_logManager.WriteInfo( strError );
			return false;
		}
		stop.linkID = (ipFeature->GetValue( m_StopPointFM.FindField( stop_point::kLinkID ) )).lVal;
		stop.nodeID = (ipFeature->GetValue( m_StopPointFM.FindField( stop_point::kNodeID ) )).lVal;
		stops.insert( make_pair(ipFeature->OID, stop) );
	}
	
	return true;
}

// ���b�V�����̓��H�����N�Ɠ��H�m�[�h���擾
bool CheckStopPoint::GetLinkNodeInMesh( const IGeometryPtr& ipMeshGeo, LINKMAP& linkMap, NODEMAP& nodeMap,
	IFeatureClassPtr& ipLinkFC, sindy::CFieldMap& linkFM, IFeatureClassPtr& ipNodeFC, sindy::CFieldMap& nodeFM )
{
	IRelationalOperatorPtr ipRelOpe( ipMeshGeo );
	CString subField;
	subField.Format( _T("%s,%s,%s,%s,%s,%s,%s,%s,%s,SHAPE"), kObjectID, road_link::kRoadClass, road_link::kMainLinkClass, road_link::kUpDownClass,
		road_link::kRoadWidth, road_link::kFromNodeID, road_link::kToNodeID, road_link::kOneway, road_link::kNoPassage );
	// �W�I���g����ێ����邽�߁AVARIANT_FALSE
	IFeatureCursorPtr ipLinkCursor = ipLinkFC->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_FALSE );
	if( !ipLinkCursor )
		return false;
	IFeaturePtr ipLinkFeature;

	// ���H�����N�̃��[�v
	while( S_OK == ipLinkCursor->NextFeature( &ipLinkFeature ) && ipLinkFeature ){
		long roadClass = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kRoadClass ) )).lVal;
			
		// roadclass_c >= 201 �̓��H�����N�́A���݂��Ȃ����̂Ƃ��Ă݂Ȃ��B
		if( IsNoTarget( roadClass ) )
			continue;

		// ObjectID��K�v���擾
		long linkID = ipLinkFeature->GetOID();
		long fromID = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kFromNodeID ) )).lVal;
		long toID   = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kToNodeID ) )).lVal;
		LINK link;
		link.fromID      = fromID;
		link.toID        = toID;
		link.roadClass   = roadClass;
		link.linkClass   = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kMainLinkClass ) )).lVal;
		link.upDownClass = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kUpDownClass ) )).lVal;
		link.roadWidth   = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kRoadWidth ) )).lVal;
		long onewayC     = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kOneway ) )).lVal;
		long nopassC     = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kNoPassage ) )).lVal;
		if( road_link::oneway::kFore == onewayC )
			link.foreOneway = true;
		else if( road_link::oneway::kReverse == onewayC )
			link.revOneway = true;
		if( road_link::no_passage::kNoPassage == nopassC || road_link::no_passage::kInpassable == nopassC )
			link.noPassage = true;

		link.ipGeo = ipLinkFeature->GetShapeCopy();

		// �m�[�hID���L�[�ɁA�ڑ������N�����i�[
		nodeMap[fromID].linkIDs.push_back( linkID );
		nodeMap[toID].linkIDs.push_back( linkID );

		VARIANT_BOOL vaContains;
		if( FAILED( ipRelOpe->Contains( ipLinkFeature->GetShape(), &vaContains ) )){
			m_logManager.WriteInfo( "#FATAL ���b�V���Ƃ�Contains����Ɏ��s�F�����NID(%d)", linkID );
			continue;
		}
		if( !vaContains )
			link.outOfMesh = true;

		linkMap[linkID] = move(link);
	}

	subField.Format( _T("%s,%s,%s,SHAPE"), kObjectID, road_node::kNodeClass, road_node::kSignal );
	IFeatureCursorPtr ipNodeCursor = ipNodeFC->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_TRUE );
	if( !ipNodeCursor )
		return false;
	IFeaturePtr ipNodeFeature;

	// ���H�m�[�h�̃��[�v
	while( S_OK == ipNodeCursor->NextFeature( &ipNodeFeature ) && ipNodeFeature ){
		long nodeID = ipNodeFeature->GetOID();

		// �m�[�hID���L�[�ɁA�`��E��ʁE�M���@�������i�[
		nodeMap[nodeID].ipGeo     = ipNodeFeature->GetShapeCopy();
		nodeMap[nodeID].nodeClass = (ipNodeFeature->GetValue( nodeFM.FindField( road_node::kNodeClass ) )).lVal;
		nodeMap[nodeID].signal    = (ipNodeFeature->GetValue( nodeFM.FindField( road_node::kSignal ) )).lVal;
	}

	return true;
}

// ���݂��Ȃ����̂ƌ��Ȃ����H��ʂ��H
bool CheckStopPoint::IsNoTarget( long roadClass )
{
	using namespace road_link::road_class;
	switch( roadClass ){

		// roadclass_c >= 201 �̓��H�����N�́A���݂��Ȃ����̂Ƃ��Ă݂Ȃ��B
		case kFerryNoDisp:
		case kFerryS2:
		case kFerryS3:
		case kFerryS4:
		case kVirtual:
		case kGarden:
		case kBridge:
		case kFacilityEntrance:
		case kParkingEntrance:
		case kParking:
		case kMatching:
		case kMatchingBT:
			return true;
		default:
			return false;
	}
	return false;
}

// �����NID�ƃm�[�hID�̑g�ݍ��킹�������ꎞ��~�ɑ��݂��邩�H
bool CheckStopPoint::FindStopPoint( long linkID, long nodeID, const STOPMAP& stops )
{
	STOP stop;
	stop.linkID = linkID;
	stop.nodeID = nodeID;
	return (stops.end() != boost::find_if(stops, [&](const std::pair<long, STOP>& st)->bool{ return (st.second==stop);} ) );
}

// �����N�ƃm�[�h�̊֌W����A���o�����𖞂������m�F����
bool CheckStopPoint::IsExtractTarget( const LINK& link, long nodeID, const LINKMAP& links, const NODEMAP& nodeMap )
{
	using namespace sindy::schema::road_link::road_class;
	using namespace sindy::schema::road_link::road_width;

	// �����N��3�{�ȏ�ڑ����Ă��Ȃ��ꍇ�͏����ɍ���Ȃ�
	auto itr = nodeMap.find( nodeID );
	if( itr == nodeMap.end() || itr->second.linkIDs.size() < 3 )
		return false;

	// main_lincclass_c <> 8 & updownclass_c in (0, 6) �𖞂����Ȃ����̂͏����ɍ���Ȃ�
	if( !IsNotBicycleNotHWY( link ) )
		return false;

	switch( link.roadClass ){

		// �׊X�HL2�Froadclass_c = 9
		// �׊X�HL1�Froadclass_c = 8
		// ���̑����H�i�Ԉ����Ώہj�Froadclass_c = 7
		// ���̑����H�Froadclass_c in (6, 108)
		case kNarrowL2:
		case kNarrowL1:
		case kOtherNoDisp:
		case kOther:
		case kTollOther:
		{
			/* 
			�ЂƂ܂����������͊O���Ă悢�Ƃ�������
			TODO: �����������K�v�ł���΁A��Lcase���K�v�ɉ����ĕ������邱��
			if( link.roadWidth == kBetween30And55 || link.roadWidth == kBetween55And130 )
				return true;
			*/
			return true;
		}

		// �w��s�̈�ʎs���Froadclass_c in (5, 107)
		case kCityNormal:
		case kTollCityNormal:
		{
			// �����_�m�[�h�ł���K�v����
			if( road_node::node_class::kCross != itr->second.nodeClass )
				return false;

			// �M���@�Ȃ��isignal_c = 0�j
			if( road_node::signal::kNoSignal == itr->second.signal ){
				// �����F3.0���ȏ�5.5������, 5.5m�ȏ�13.0m����
				if( link.roadWidth != kBetween30And55 && link.roadWidth != kBetween55And130 )
					return false;

				return true;
			}
			// �M���@����isignal_c = 1�j
			else if( road_node::signal::kSignal == itr->second.signal ){
				// �����F3.0���ȏ�5.5������
				if( link.roadWidth != kBetween30And55 )
					return false;

				// ��v�n�����ȏ�̃����N��2�{�ȏ�
				int OkLinkCount = 0;
				for( auto& linkID : itr->second.linkIDs ){
					if( IsPrefOrOver( links, linkID ) )
						++OkLinkCount;
				}
				if( OkLinkCount > 1 )
					return true;
			}

			return false;
		}

		// ��ʓs���{�����Froadclass_c in (4, 106)
		case kPrefectureNormal:
		case kTollPrefectureNormal:
		{
			// �����_�m�[�h�ł���K�v����
			if( road_node::node_class::kCross != itr->second.nodeClass )
				return false;

			// �M���@�Ȃ��isignal_c = 0�j
			if( road_node::signal::kNoSignal == itr->second.signal ){
				
				// �����F3.0���ȏ�5.5������, 5.5m�ȏ�13.0m����
				if( link.roadWidth != kBetween30And55 && link.roadWidth != kBetween55And130 )
					return false;

				return true;
			}
			// �M���@����isignal_c = 1�j
			else if( road_node::signal::kSignal == itr->second.signal ){

				int OkLinkCount = 0;
				for( auto& linkID : itr->second.linkIDs ){
					if( IsCountryOrOver( links, linkID ) )
						++OkLinkCount;
				}
				if( OkLinkCount > 1 )
					return true;
			}

			return false;
		}

		// ��v�n�����i�s�{���j/��v�n�����i�w��s�j�Froadclass_c in (2, 3, 104, 105)
		case kPrefectureMain:
		case kCityMain:
		case kTollPrefectureMain:
		case kTollCityMain:
		{
			// �����_�m�[�h�ł���K�v����
			if( road_node::node_class::kCross != itr->second.nodeClass )
				return false;

			// �����_�m�[�h�i�M���@�Ȃ��j�Fsignal_c = 0
			if( road_node::signal::kNoSignal == itr->second.signal ){

				// �����F3.0���ȏ�5.5������, 5.5m�ȏ�13.0m����
				if( link.roadWidth != kBetween30And55 && link.roadWidth != kBetween55And130 )
					return false;

				return true;
			}

			return false;
		}

		// �����Froadclass_c in (1, 103)
		case kCountry:
		case kTollCountry:
		{
			// �����_�m�[�h�ł���K�v����
			if( road_node::node_class::kCross != itr->second.nodeClass )
				return false;

			// �����_�m�[�h�i�M���@�Ȃ��j�Fsignal_c = 0
			if( road_node::signal::kNoSignal == itr->second.signal ){

				// �����F3.0���ȏ�5.5������, 5.5m�ȏ�13.0m����
				if( link.roadWidth != kBetween30And55 && link.roadWidth != kBetween55And130 )
					return false;

				return true;
			}
			return false;
		}
		default:
			break;
	}

	return false;
}

// �����NID���烊���N���擾���A��v�n�����ȏォ�m�F����
bool CheckStopPoint::IsPrefOrOver( const LINKMAP& links, long linkID )
{
	//     HWY�Ώۓ��H(updownclass_c not in (0,6))
	//     �܂���
    //     HWY�Ώۓ��H�ł͂Ȃ��āA��v�n�����ȏ�(updownclass_c in (0,6) and roadclass_c in (1,2,3,103,104,105))
	using namespace sindy::schema::road_link::road_class;
	using namespace sindy::schema::road_link::up_down_class;

	try{
		auto& link = links.at( linkID );

		if( kNoHighway != link.upDownClass && kEtcAccess != link.upDownClass )
			return true;

		else{
			switch( link.roadClass ){

				case kCountry:
				case kTollCountry:
				case kPrefectureMain:
				case kTollPrefectureMain:
				case kCityMain:
				case kTollCityMain:
					return true;
				default:
					return false;
			}
		}
	}	
	catch( ... ){
		return false;
	}
	return false;

}

// �����NID���烊���N���擾���A�����ȏォ�m�F����
bool CheckStopPoint::IsCountryOrOver( const LINKMAP& links, long linkID )
{
	//     HWY�Ώۓ��H(updownclass_c not in (0,6))
	//     �܂���
    //     HWY�Ώۓ��H�ł͂Ȃ��āA�����ȏ�(updownclass_c in (0,6) and roadclass_c in (1,103))
	using namespace sindy::schema::road_link::road_class;
	using namespace sindy::schema::road_link::up_down_class;

	try{
		auto& link = links.at( linkID );

		if( kNoHighway != link.upDownClass && kEtcAccess != link.upDownClass )
			return true;

		if( kCountry == link.roadClass || kTollCountry == link.roadClass )
			return true;
	}	
	catch( ... ){
		return false;
	}
	return false;
}

// ���]�ԓ��ł͂Ȃ�(�����N���(not 8))��HYW�ł͂Ȃ�(�n�C�E�F�C�H��(0,6))���m�F����
bool CheckStopPoint::IsNotBicycleNotHWY( const LINK& link )
{
	using namespace sindy::schema::road_link::main_link_class;
	using namespace sindy::schema::road_link::up_down_class;
	
	if( link.linkClass != kBicycle && ( link.upDownClass == kNoHighway || link.upDownClass == kEtcAccess ))
		return true;

	return false;
}

// �ʍs�֎~�����Ă��Ȃ����A�ꎞ��~�̌��������Ȃ����m�F����
bool CheckStopPoint::IsRegOK( const LINK& link, long nodeID )
{
	// �ʍs�֎~�����Ă���ꍇ
	if( link.noPassage )
		return false;

	// ��������� �� �����N���猩�ė��o�m�[�h��fromID�ƂȂ��Ă���ꍇ(��ʋt��)
	if( link.foreOneway && nodeID == link.fromID )
		return false;

	// �t������� �� �����N���猩�ė��o�m�[�h��toID�ƂȂ��Ă���ꍇ(��ʋt��)
	if( link.revOneway && nodeID == link.toID )
		return false;
	
	return true;
}

// ���H�����N��������_�����N�ł��邩�𔻒肷��
bool CheckStopPoint::IsRoundAbout(const LINK & link)
{
	return ( link.linkClass == sindy::schema::road_link::link_class::kRoundAbout );
}

// ���H�����N���{���i�㉺�����j�����N�ł��邩�𔻒肷��
bool CheckStopPoint::IsMainUpDown(const LINK & link)
{
	return ( link.linkClass == sindy::schema::road_link::link_class::kMainUpDown );
}

// ���H�����N��HWY�{���ł��邩�𔻒肷��
bool CheckStopPoint::IsHighwayMain(const LINK & link)
{
	return (link.upDownClass==sindy::schema::road_link::up_down_class::kUpInWest ||
			link.upDownClass==sindy::schema::road_link::up_down_class::kDownOutEast ||
			link.upDownClass==sindy::schema::road_link::up_down_class::kCommon );
}

// 2�����b�V�����E�m�[�h���ǂ����𔻒肷��
bool CheckStopPoint::IsMeshEdge( const NODE& node)
{
	return (road_node::node_class::kEdge == node.nodeClass ||
			road_node::node_class::kPecEdge == node.nodeClass);
}

// ���H�m�[�h�������_�m�[�h���ǂ����𔻒肷��
bool CheckStopPoint::IsCross(const NODE& node)
{
	return (road_node::node_class::kCross == node.nodeClass);
}

// ���H�m�[�h���M���@�t�������_�ł��邩�ǂ����𔻒肷��
bool CheckStopPoint::IsSignalCross( const NODE& node )
{
	return (road_node::node_class::kCross == node.nodeClass &&
			road_node::signal::kSignal == node.signal);
}

// 2�̃����N���Ȃ��p�x�����߂�
double CheckStopPoint::GetAngle( const LINK& link1, const LINK& link2 )
{
	return AheGetAngle( (ICurvePtr)link1.ipGeo, (ICurvePtr)link2.ipGeo );
}

// �ꎞ��~�|�C���g�𐶐�����֐�
IGeometryPtr CheckStopPoint::CreateStopPointGeometry(long linkID, long nodeID, const LINK& link,  const NODE& node, ISpatialReferencePtr ipSpRef)
{
	IGeometryPtr ipRoadGeom(AheCreateClone(link.ipGeo)), ipNodeGeom(AheCreateClone(node.ipGeo));
	IConstructPointPtr ipConstructPoint( ipNodeGeom );
	double dLineLength = GetLineLength( (IPolylinePtr)ipRoadGeom );

	if( dLineLength > 11.0 )
	{
		// From����5m�̒n�_�̃|�C���g�擾
		if( link.fromID == nodeID )
			ipConstructPoint->ConstructAlong( (ICurvePtr)(IPolylinePtr)ipRoadGeom, esriExtendAtFrom, 5.0 / dLineLength, VARIANT_TRUE );
		// To����5m�̃|�C���g�擾
		else
			ipConstructPoint->ConstructAlong( (ICurvePtr)(IPolylinePtr)ipRoadGeom, esriExtendAtFrom, ( dLineLength - 5.0 ) / dLineLength, VARIANT_TRUE );
	}
	else
	{
		// From����A�����N�̒�����1/4m�n�_�̃|�C���g�擾
		if( link.fromID == nodeID )
			ipConstructPoint->ConstructAlong( (ICurvePtr)(IPolylinePtr)ipRoadGeom, esriExtendAtFrom, 0.25 , VARIANT_TRUE );
		// To����A�����N�̒�����1/4m�n�_�̃|�C���g�擾
		else
			ipConstructPoint->ConstructAlong( (ICurvePtr)(IPolylinePtr)ipRoadGeom, esriExtendAtFrom, 0.75 , VARIANT_TRUE );

	}
	((IGeometryPtr)ipConstructPoint)->putref_SpatialReference( ipSpRef );

	return ipConstructPoint;
}
