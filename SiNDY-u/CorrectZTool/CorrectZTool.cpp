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
#include "CorrectZTool.h"
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/uh_float.h>
#include <TDC/useful_headers/scope_guard.h>
#include <WinLib/VersionInfo.h>

using namespace std;
using namespace sindy::schema;
using namespace sindy::schema::adam_v2;
using namespace uh;
using namespace uh::str_util;
using namespace adam;

// ������
bool CCorrectZTool::init()
{
	// ���O�I�[�v��
	if( !ERRLOGGER.Initialize( m_args.m_err_log.c_str() ) )
	{
		cerr << "�G���[���O�t�@�C���̃I�[�v���Ɏ��s" << endl;
		return false;
	}
	if( !RUNLOGGER.Initialize( m_args.m_run_log.c_str() ) )
	{
		cerr << "���s���O�t�@�C���̃I�[�v���Ɏ��s" << endl;
		return false;
	}
	RUNLOGGER.writeOption( m_args );
	RUNLOGGER.writeInfo( _T( "�J�n����" ) );

	// ���b�V�����X�g�擾
	if( !getMeshList( m_args.m_mesh_list.c_str(), m_meshList ) )
	{
		RUNLOGGER.writeInfo( _T( "���b�V�����X�g�擾���s" ) );
		return false;
	}

	// BASEMESH�̂�OK
	list<long>::iterator headItr = m_meshList.begin();
	list<long>::reverse_iterator tailItr = m_meshList.rbegin();
	long headLength = static_cast<long>( log10( *headItr ) + 1 );
	long tailLength = static_cast<long>( log10( *tailItr ) + 1 );
	if( headLength != 6 || tailLength != 6 )
	{
		RUNLOGGER.writeInfo( _T( "���b�V���������s��" ) );
		return false;
	}

	// ���[�N�X�y�[�X�擾
	if( !openWorkspace( m_args.m_db.c_str(), m_ipWorkspace ) ||
		!openWorkspace( m_args.m_mesh_db.c_str(), m_ipMeshWorkspace ) )
		return false;

	// DB�^�C�v���`�F�b�N
	switch( m_ipWorkspace->GetType() )
	{
		case esriRemoteDatabaseWorkspace:
		{
			// �f�t�H���g�o�[�W�����͏������~
			if( isDefaultVersion( m_ipWorkspace ) )
				return false;
			break;
		}
		case esriLocalDatabaseWorkspace:
			break;
		default:
			return false;
	}

	// �t�B�[�`���N���X�E�e�[�u���擾
	m_ipMesh = openTable( m_ipMeshWorkspace, basemesh::kTableName, m_cMeshFM );
	m_ipLaneLink = openTable( m_ipWorkspace, lane_link::kTableName, m_cLaneLinkFM );
	m_ipLaneNode = openTable( m_ipWorkspace, lane_node::kTableName, m_cLaneNodeFM );
	m_ipBorderLink = openTable( m_ipWorkspace, border_link::kTableName, m_cBorderLinkFM );
	m_ipRelLaneLinkBorderLink = openTable( m_ipWorkspace, rel_border_link_lane_link::kTableName, m_cRelLaneLinkBorderLinkFM );
	m_ipNqLaneNode = openTable( m_ipWorkspace, nq_lane_node::kTableName,  m_cNqLaneNodeFM );
	if( !m_ipMesh || !m_ipLaneLink || !m_ipLaneNode || !m_ipBorderLink || !m_ipRelLaneLinkBorderLink || !m_ipNqLaneNode )
		return false;

	// �c�[�����擾
	CVersion cVersion;
	m_toolName = cVersion.GetInternalName();

	return true;
}

// ���s
bool CCorrectZTool::execute()
{
	// ������
	if( !init() )
		return false;

	// �����̃X�R�[�v�𔲂����狭���I��abort
	uh::scope_guard scope( bind( abortEdit, m_ipWorkspace ) );

	// ���b�V�����X�g����`����擾
	if( !getMeshPolygon( m_meshList, m_meshGeoMap ) )
		return false;

	// ���b�V�������[�v
	long count = 0;
	for( const auto& mesh : m_meshGeoMap )
	{
		cout << "���b�V��������..." + to_string( mesh.first ) + " : " + to_string( ++count ) + " / " + to_string( m_meshGeoMap.size() ) << endl;

		// �ҏW�J�n
		if( !startEdit( m_ipWorkspace ) )
			return false;

		// �Ԑ������N����
		GidList ipcLaneLinkGids;
		if( !searchLaneLink( mesh.second, ipcLaneLinkGids ) )
			return false;

		// �m�[�h����������ĕR�t���Ԑ��m�[�h�𒲂ׂ�
		GidList laneNodeGids;
		Node2LinkMap node2link;
		Link2NodeMap dummy;
		if( !searchNqTable( ipcLaneLinkGids, laneNodeGids, node2link, dummy ) )
			return false;

		// �Ԑ��m�[�h����������AT�m�[�h�̈ꗗ���擾
		GidSet atLaneNodeGids;
		if( !searchLaneNode( laneNodeGids, atLaneNodeGids ) )
			return false;

		// �֘A�e�[�u�����������ĕR�t�����H���E�������N�𒲂ׂ�
		GidList borderLinkGids;
		LaneBorderRelMap lane2border;
		if( !searchRelTable( ipcLaneLinkGids, borderLinkGids, lane2border ) )
			return false;

		// ���H���E�������N���擾
		FeatureMap borderLinks;
		if( !searchBorderLink( borderLinkGids, borderLinks ) )
			return false;

		// �����N�X�V�iUpdate�J�[�\���ŁA�����N�\���_�ƍŋߖT���E���n�_�Ƃ�Z�l�X�V�j
		if( !updateLaneLink( ipcLaneLinkGids, atLaneNodeGids, lane2border, borderLinks, mesh.first ) )
			return false;

		// �m�[�h�X�V�iUpdate�J�[�\���ŁA�m�[�h�ƍŋߖT���E���n�_�Ƃ�Z�l�X�V�j
		if( !updateLaneNode( laneNodeGids, node2link, lane2border, borderLinks, mesh.first ) )
			return false;

		// �ҏW�I��
		if( !stopEdit( m_ipWorkspace ) )
			return false;
	}

	// �ȉ��͑S�����ꊇ����
	cout << "�����N�Ԓi����������...�J�n" << endl;

	// �ҏW�J�n
	if( !startEdit( m_ipWorkspace ) )
		return false;

	// �X�V�����m�[�h�ɕR�Â������N���m��Z�l����v���Ă��邩�m�F���A��v���Ă��Ȃ������N�̍\���_�̓m�[�h��Z�l�ŕ␳����
	if( !doConsistentZ() )
		return false;

	// �ҏW�I��
	if( !stopEdit( m_ipWorkspace ) )
		return false;

	cout << "�����N�Ԓi����������...����" << endl;

	cout << "�}���z�����N���`��ԏ���...�J�n" << endl;

	// �ҏW�J�n
	if( !startEdit( m_ipWorkspace ) )
		return false;

	// �}���z�����N�Ƃ�������FIX�����N�܂ł̂��ׂĂ̐ڑ������N����`��Ԃ���
	// ���킹�Ċ֘A����m�[�h�����`��Ԃ���
	if( !doLinerInterpolation() )
		return false;

	// �ҏW�I��
	if( !stopEdit( m_ipWorkspace ) )
		return false;

	cout << "�}���z�����N���`��ԏ���...����" << endl;

	return true;
}

bool CCorrectZTool::searchLaneLink( const IGeometryPtr& mesh, GidList& ipcLaneLinkGids )
{
	using namespace lane_link;

	IFeatureCursorPtr ipFeatureCur;
	if( FAILED( m_ipLaneLink->Search( AheInitSpatialFilter( nullptr, mesh ), VARIANT_FALSE, &ipFeatureCur ) ) || !ipFeatureCur )
	{
		RUNLOGGER.writeInfo( format( _T( "%s�������s" ), kTableName ) );
		return false;
	}

	IFeaturePtr ipFeature;
	while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
	{
		CString gid = ipFeature->GetValue( m_cLaneLinkFM.HasField( kGlobalID ) ).bstrVal;
		if( gid.IsEmpty() )
			continue;

		// SOURCEID = NULL �Ȃ�IPC�����N�ANOT NULL �Ȃ�AT�����N
		CString sourceId = ipFeature->GetValue( m_cLaneLinkFM.HasField( kSourceID ) ).bstrVal;
		if( sourceId.IsEmpty() )
			ipcLaneLinkGids.emplace_back( gid );
	}

	return true;
}

bool CCorrectZTool::searchRelTable( const GidList& laneLinkGids, GidList& borderLinkGids, LaneBorderRelMap& lane2border )
{
	using namespace rel_border_link_lane_link;

	CString subFieleds = format( _T( "%s,%s,%s" ), kLaneLinkGID, kBorderLinkGID, kLRC );

	// 1000�����Ɍ���
	for( const auto& str : getString( laneLinkGids ) )
	{
		_ICursorPtr ipCur;
		if( FAILED( m_ipRelLaneLinkBorderLink->Search(
			AheInitQueryFilter( nullptr, subFieleds, _T( "%s IN (%s)" ), kLaneLinkGID, str ),
			VARIANT_TRUE, &ipCur ) ) || !ipCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s�������s" ), kTableName ) );
			return false;
		}

		_IRowPtr ipRow;
		while( S_OK == ipCur->NextRow( &ipRow ) && ipRow )
		{
			CString laneGid = ipRow->GetValue( m_cRelLaneLinkBorderLinkFM.HasField( kLaneLinkGID ) ).bstrVal;
			CString borderGid = ipRow->GetValue( m_cRelLaneLinkBorderLinkFM.HasField( kBorderLinkGID ) ).bstrVal;
			long lrC = ipRow->GetValue( m_cRelLaneLinkBorderLinkFM.HasField( kLRC ) ).lVal;

			borderLinkGids.emplace_back( borderGid );
			// �Ԑ������N�ɑ΂��ē��H���E�������N�͍��E��1�{�ȏジ�R�t��
			lane2border[laneGid].emplace( lrC, borderGid );
		}
	}

	// �Ō�ɏd���폜
	borderLinkGids.sort();
	borderLinkGids.unique();

	return true;
}

bool CCorrectZTool::searchLaneNode( const GidList& laneNodeGids, GidSet& atLaneNodeGids )
{
	using namespace lane_node;

	CString subFieleds = format( _T( "%s,%s" ), kSourceID, kGlobalID );

	// 1000�����Ɍ���
	for( const auto& str : getString( laneNodeGids ) )
	{
		IFeatureCursorPtr ipFeatureCur;
		if( FAILED( m_ipLaneNode->Search(
			AheInitQueryFilter( nullptr, subFieleds, _T( "%s IN (%s)" ), kGlobalID, str ),
			VARIANT_TRUE, &ipFeatureCur ) ) || !ipFeatureCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s�������s" ), kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
		{
			// SOURCEID != NULL �Ȃ�AT�m�[�h
			CString sourceId = ipFeature->GetValue( m_cLaneNodeFM.HasField( kSourceID ) ).bstrVal;
			if( !sourceId.IsEmpty() )
			{
				CString gid = ipFeature->GetValue( m_cLaneNodeFM.HasField( kGlobalID ) ).bstrVal;
				atLaneNodeGids.emplace( gid );
			}
		}
	}

	return true;
}

bool CCorrectZTool::searchBorderLink( const GidList& borderLinkGids, FeatureMap& borderLinks )
{
	using namespace border_link;

	CString subFieleds = format( _T( "%s,%s" ), kGlobalID, kShape );

	// 1000�����Ɍ���
	for( const auto& str : getString( borderLinkGids ) )
	{
		IFeatureCursorPtr ipFeatureCur;
		if( FAILED( m_ipBorderLink->Search(
			AheInitQueryFilter( nullptr, subFieleds, _T( "%s IN (%s)" ), kGlobalID, str ),
			VARIANT_FALSE, &ipFeatureCur ) ) || !ipFeatureCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s�������s" ), kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
		{
			CString gid = ipFeature->GetValue( m_cBorderLinkFM.HasField( kGlobalID ) ).bstrVal;
			if( gid.IsEmpty() )
				continue;
			borderLinks.emplace( gid, ipFeature );
		}
	}

	return true;
}

bool CCorrectZTool::updateLaneLink( const GidList& laneLinkGids, const GidSet& atLaneNodeGids, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long meshCode )
{
	// 1000�����Ɍ���
	for( const auto& str : getString( laneLinkGids ) )
	{
		IFeatureCursorPtr ipUpdateCur;
		if( FAILED( m_ipLaneLink->Update(
			AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), lane_link::kGlobalID, str ),
			VARIANT_FALSE, &ipUpdateCur ) ) || !ipUpdateCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s�������s" ), lane_link::kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipUpdateCur->NextFeature( &ipFeature ) && ipFeature )
		{
			CString gid = ipFeature->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
			if( gid.IsEmpty() )
				continue;

			// AT�����N�Ɛڑ�����ꍇ�́A�ڑ������̍\���_�����X�V���X�L�b�v
			set<long> skipPointNums;
			if( 1 == atLaneNodeGids.count( ipFeature->GetValue( m_cLaneLinkFM.HasField( lane_link::kFromNodeGID ) ).bstrVal ) )
				skipPointNums.emplace( 0 ); // FROM�m�[�h���������\���_
			if( 1 == atLaneNodeGids.count( ipFeature->GetValue( m_cLaneLinkFM.HasField( lane_link::kToNodeGID ) ).bstrVal ) )
				skipPointNums.emplace( IPointCollectionPtr( ipFeature->GetShape() )->GetPointCount() - 1 ); // TO�m�[�h���������\���_

			GidSet dummy;
			updateRun( ipFeature, ipUpdateCur, dummy, lane2border, borderLinks, skipPointNums, lane_link::kTableName, m_cLaneLinkFM, meshCode );
		}
	}

	return true;
}

bool CCorrectZTool::searchNqTable( const GidList& laneLinkGids, GidList& laneNodeGids, Node2LinkMap& node2link, Link2NodeMap& link2node )
{
	using namespace nq_lane_node;

	CString subFieleds = format( _T( "%s,%s,%s" ), kLaneLinkGID, kLaneNodeGID, kSequence );

	// 1000�����Ɍ���
	for( const auto& str : getString( laneLinkGids ) )
	{
		_ICursorPtr ipCur;
		if( FAILED( m_ipNqLaneNode->Search(
			AheInitQueryFilter( nullptr, subFieleds, _T( "%s IN (%s)" ), kLaneLinkGID, str ),
			VARIANT_TRUE, &ipCur ) ) || !ipCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s�������s" ), kTableName ) );
			return false;
		}

		_IRowPtr ipRow;
		while( S_OK == ipCur->NextRow( &ipRow ) && ipRow )
		{
			CString linkGid = ipRow->GetValue( m_cNqLaneNodeFM.HasField( kLaneLinkGID ) ).bstrVal;
			CString nodeGid = ipRow->GetValue( m_cNqLaneNodeFM.HasField( kLaneNodeGID ) ).bstrVal;
			long seq = ipRow->GetValue( m_cNqLaneNodeFM.HasField( kSequence ) ).lVal;

			laneNodeGids.emplace_back( nodeGid );
			node2link[nodeGid].emplace( linkGid );
			link2node[linkGid].emplace( nodeGid, seq );
		}
	}

	// �Ō�ɏd���폜
	laneNodeGids.sort();
	laneNodeGids.unique();

	return true;
}

bool CCorrectZTool::updateLaneNode( const GidList& laneNodeGids, const Node2LinkMap& node2link, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long meshCode )
{
	// 1000�����Ɍ���
	for( const auto& str : getString( laneNodeGids ) )
	{
		IFeatureCursorPtr ipUpdateCur;
		if( FAILED( m_ipLaneNode->Update(
			AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), lane_node::kGlobalID, str ),
			VARIANT_FALSE, &ipUpdateCur ) ) || !ipUpdateCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s�������s" ), lane_node::kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipUpdateCur->NextFeature( &ipFeature ) && ipFeature )
		{
			// SOURCEID != NULL �Ȃ�AT�m�[�h�Ȃ̂ŁA�X�V�ΏۊO
			CString sourceId = ipFeature->GetValue( m_cLaneNodeFM.HasField( lane_node::kSourceID ) ).bstrVal;
			if( !sourceId.IsEmpty() )
				continue;

			CString gid = ipFeature->GetValue( m_cLaneNodeFM.HasField( lane_node::kGlobalID ) ).bstrVal;
			if( gid.IsEmpty() )
				continue;

			set<long> dummy;
			updateRun( ipFeature, ipUpdateCur, node2link.at( gid ), lane2border, borderLinks, dummy, lane_node::kTableName, m_cLaneNodeFM, meshCode );
		}
	}

	return true;
}

void CCorrectZTool::updateRun(
	IFeaturePtr& ipFeature,
	IFeatureCursorPtr& ipUpdateCur,
	const GidSet& linkGids,
	const LaneBorderRelMap& lane2border,
	const FeatureMap& borderLinks,
	const set<long>& skipPointNums,
	const CString layer,
	const sindy::CFieldMap& fieldMap,
	long meshCode
)
{
	CString gid = ipFeature->GetValue( fieldMap.HasField( adam_v2_global_ipc_feature::kGlobalID ) ).bstrVal;

	// ��x�X�V�����t�B�[�`���[�͍X�V�X�L�b�v
	if( 1 == m_updatedList.count( gid ) )
	{
		ERRLOGGER.warning( layer, ToString( ipFeature->GetOID() ), gid, _T( "�X�L�b�v�F���̃��b�V���̏����ɂčX�V�ς�" ), ToString( meshCode ), CString() );
		return;
	}

	// ���E�̓��H���E�������N���擾
	FeatureList leftBorders, rightBorders;
	if( layer.CompareNoCase( lane_link::kTableName ) == 0 )
	{
		if( 0 == lane2border.count( gid ) )
			return; // SA�EPA�Ƃ��͌����狫�E�����Ȃ��̂ōX�V�ΏۊO

		getBorderLink( gid, lane2border, borderLinks, rel_border_link_lane_link::lr_code::kLeft, leftBorders );
		getBorderLink( gid, lane2border, borderLinks, rel_border_link_lane_link::lr_code::kRight, rightBorders );
	}
	else if( layer.CompareNoCase( lane_node::kTableName ) == 0 )
	{
		// �m�[�h�ɕR�Â������N�����[�v
		long notFoundCount = 0;
		for( const auto& linkGid : linkGids )
		{
			if( 0 == lane2border.count( linkGid ) )
			{
				++notFoundCount;
				continue; // SA�EPA�Ƃ��͌����狫�E�����Ȃ��̂ōX�V�ΏۊO
			}

			getBorderLink( linkGid, lane2border, borderLinks, rel_border_link_lane_link::lr_code::kLeft, leftBorders );
			getBorderLink( linkGid, lane2border, borderLinks, rel_border_link_lane_link::lr_code::kRight, rightBorders );

			if( leftBorders.empty() || rightBorders.empty() )
			{
				// �Е��ł��������烊�Z�b�g���Ď���
				// �G���[�͑S���̃����N�Ń_�����������ɏo��
				// TODO: �S�Ă̎Ԑ������N�ɕR�Â����E�̓��H���E���S�Ă���AZ�l�̒l�����߂������ۊǂ����Z�l�̐��x�͍����Ȃ�͂�
				leftBorders.clear(); rightBorders.clear();
				continue;
			}
			break; // ���������Ă��OK
		}

		// �S���̃����N�ŋ��E�����Ȃ�������return
		if( notFoundCount == linkGids.size() )
			return;
	}
	else
		assert( false ); // �����֗��Ă͂����Ȃ�

	if( leftBorders.empty() || rightBorders.empty() )
	{
		ERRLOGGER.error( layer, ToString( ipFeature->GetOID() ), gid, _T( "�X�L�b�v�F���H���E�������N�����E�����Ă��Ȃ�" ), ToString( meshCode ), CString() );
		return;
	}

	// Z�l���X�V����
	FeatureMap usedBorders;
	IGeometryPtr ipNewGeo( ipFeature->GetShape() );
	bool bIsSame = false;
	if( !updateZValue( ipFeature->GetShape(), leftBorders, rightBorders, skipPointNums, usedBorders, ipNewGeo, bIsSame ) )
	{
		ERRLOGGER.error( layer, ToString( ipFeature->GetOID() ), gid, _T( "�X�L�b�v�FZ�l�X�V�Ɏ��s" ), ToString( meshCode ), CString() );
		return;
	}

	// ���O�o�͗p�̓��H���E�������NSOURCE
	CString source;
	set<CString> sources;
	for( const auto& feature : usedBorders )
		sources.emplace( feature.second->GetValue( m_cBorderLinkFM.HasField( border_link::kSource ) ).bstrVal );
	source = join( sources, _T(",") );

	// Z�l�ɕω���������΃X�L�b�v
	if( bIsSame )
	{
		ERRLOGGER.warning( layer, ToString( ipFeature->GetOID() ), gid, _T( "�X�L�b�v�FZ�l�ɕω����Ȃ�" ), ToString( meshCode ), source );
		return;
	}

	// �t�B�[�`���[���X�V����
	if( !putValue( ipFeature, ipNewGeo, fieldMap ) || FAILED( ipUpdateCur->UpdateFeature( ipFeature ) ) )
	{
		ERRLOGGER.error( layer, ToString( ipFeature->GetOID() ), gid, _T( "�X�L�b�v�F�t�B�[�`���[�X�V�Ɏ��s" ), ToString( meshCode ), source );
		return;
	}

	ERRLOGGER.info( layer, ToString( ipFeature->GetOID() ), gid, _T( "�X�V����" ), ToString( meshCode ), source );
	m_updatedList.emplace( gid );

	// �m�[�h�Ȃ�A��X�̃`�F�b�N�p�ɏ���ێ�
	if( layer.CompareNoCase( lane_node::kTableName ) == 0 )
		m_updatedNode.emplace( gid, ipFeature );
}

bool CCorrectZTool::doConsistentZ()
{
	// �X�V���ꂽ�m�[�hGID���擾
	list<CString> nodeGids;
	for( const auto& pair : m_updatedNode )
		nodeGids.emplace_back( pair.first );

	// 1000�����Ɍ���
	map<CString, map<CString, vector<IFeaturePtr>>> node2Links; // Key:�m�[�hGID, Value:�}�b�v�iKey:�t�B�[���h, Value:�����N�j
	for( const auto& ids : getString( nodeGids ) )
	{
		if( !getNode2Links( lane_link::kFromNodeGID, ids, node2Links ) ||
			!getNode2Links( lane_link::kToNodeGID, ids, node2Links ) )
			return false;
	}

	// �R�Â������N���m��Z�l����v���Ă��邩�m�F���A�s��v�ȏꍇ�̓����N���m��ڑ�������
	map<CString, pair<IGeometryPtr, CString>> linkInfo; // Key:�����NGID, Value:�y�A�iFirst:�X�V�ナ���N�`��, Value:�X�V�ӏ��iFrom/To�j�j
	for( const auto& pair : node2Links )
	{
		if( !connectLink( m_updatedNode[pair.first], pair.second, linkInfo ) )
			return false;
	}

	// �����NGID���擾
	list<CString> linkGids;
	for( const auto& pair : linkInfo )
		linkGids.emplace_back( pair.first );

	// 1000�����ɍX�V
	for( const auto& ids : getString( linkGids ) )
	{
		if( !reUpdateLinkZ( ids, linkInfo ) )
			return false;
	}

	return true;
}

bool CCorrectZTool::getNode2Links( const CString& field, const CString& ids, map<CString, map<CString, vector<IFeaturePtr>>>& node2Links ) const
{
	IFeatureCursorPtr ipFeatureCur;
	if( FAILED( m_ipLaneLink->Search(
		AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), field, ids ),
		VARIANT_FALSE, &ipFeatureCur ) ) || !ipFeatureCur )
	{
		RUNLOGGER.writeInfo( format( _T( "%s�������s" ), lane_link::kTableName ) );
		return false;
	}

	IFeaturePtr ipFeature;
	while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
	{
		CString nodeGid = ipFeature->GetValue( m_cLaneLinkFM.HasField( field ) ).bstrVal;
		node2Links[nodeGid][field].emplace_back( ipFeature );
	}

	return true;
}

bool CCorrectZTool::connectLink( const IFeaturePtr& ipFeature, const map<CString, vector<IFeaturePtr>>& field2Links, map<CString, pair<IGeometryPtr, CString>>& linkInfo )
{
	// �m�[�h��Z�l���擾
	double nodeZ = IPointPtr( ipFeature->GetShape() )->GetZ();

	// �����N�Ń��[�v
	for( const auto& pair : field2Links )
	{
		for( const auto& link : pair.second )
		{
			CString linkGid = link->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
			IGeometryPtr linkGeo = link->GetShapeCopy();

			// �������ɔ��Α��̃m�[�h�̏����ŃW�I���g�����X�V���Ă�����A���̃W�I���g�����܂��g��
			// �������Ȃ���1�{�̃����N�ŗ����Ƃ��Y���Ă���P�[�X�ɑΉ��ł��Ȃ�
			if( 1 == linkInfo.count( linkGid ) )
				linkGeo = linkInfo[linkGid].first;

			// From/To�ɉ����������N�̍\���_����AZ�l���擾
			IPointCollectionPtr ipPointCol( linkGeo );
			long targetNum = pair.first.CompareNoCase( lane_link::kFromNodeGID ) == 0 ? 0 : ipPointCol->GetPointCount() - 1;
			IPointPtr ipPoint = ipPointCol->GetPoint( targetNum );
			double linkZ = ipPoint->GetZ();

			// �m�[�h��Z�l�ƈ�v���Ă����OK
			if( nodeZ == linkZ )
				continue;

			// ���z���v�Z����
			long nextNum = pair.first.CompareNoCase( lane_link::kFromNodeGID ) == 0 ? 1 : ipPointCol->GetPointCount() - 2;
			IPointPtr nextPt = ipPointCol->GetPoint( nextNum );
			double nextLinkZ = nextPt->GetZ();

			double height = nextLinkZ - nodeZ;						// �����i�����j
			double distance = AheGetMeterLength( ipPoint, nextPt ); // �����i�����j
			double slope = height * 100.0 / distance;				// ���z�i���� * 100 / ����[%]�j

			// �}���z�Ȃ�A��Ő��`��Ԃ��邽�ߊm��
			if( m_args.m_slope_max < fabs( slope ) )
				m_steepLink.emplace( linkGid );

			// �����N�̍\���_������Z�l���X�V����
			if( FAILED( ipPoint->put_Z( nodeZ ) ) || FAILED( ipPointCol->UpdatePoint( targetNum, ipPoint ) ) )
				return false;

			CString msg = pair.first.CompareNoCase( lane_link::kFromNodeGID ) == 0 ?
				str_util::format( _T( "FROM�m�[�h���̍\���_������Z�l���čX�V�F���z(%lf)" ), slope ) :
				str_util::format( _T( "TO�m�[�h���̍\���_������Z�l���čX�V�F���z(%lf)" ), slope );

			// �������ɔ��Α��̃m�[�h�ŏ������Ă�����A���b�Z�[�W���ǉ�
			if( 1 == linkInfo.count( linkGid ) )
				msg += ( _T( ", " ) + linkInfo[linkGid].second );

			// �X�V���������N�̏����m��
			linkInfo[linkGid] = make_pair( IGeometryPtr( ipPointCol ), msg );

			// �X�V���������N�ɐڑ����郊���N��Z�l���܂��^�Ȃ̂ŁA���`��ԏ����Ōo�H�T���ΏۊO�ɂ��郊���N�Ƃ��Ċm��
			CString otherField = pair.first.CompareNoCase( lane_link::kFromNodeGID ) == 0 ? lane_link::kToNodeGID : lane_link::kFromNodeGID;
			if( 0 == field2Links.count( otherField ) )
				continue;
			for( const auto& fixLink : field2Links.at( otherField ) )
				m_fixLink.emplace( fixLink->GetOID() );
		}
	}

	return true;
}

bool CCorrectZTool::reUpdateLinkZ( const CString& ids, map<CString, pair<IGeometryPtr, CString>> linkInfo )
{
	IFeatureCursorPtr ipUpdateCur;
	if( FAILED( m_ipLaneLink->Update(
		AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), lane_link::kGlobalID, ids ),
		VARIANT_FALSE, &ipUpdateCur ) ) || !ipUpdateCur )
	{
		RUNLOGGER.writeInfo( format( _T( "%s�������s" ), lane_link::kTableName ) );
		return false;
	}

	IFeaturePtr ipFeature;
	while( S_OK == ipUpdateCur->NextFeature( &ipFeature ) && ipFeature )
	{
		CString gid = ipFeature->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
		IGeometryPtr ipNewGeo = linkInfo.at( gid ).first;
		CString msg = linkInfo.at( gid ).second;

		if( !putValue( ipFeature, ipNewGeo, m_cLaneLinkFM ) || FAILED( ipUpdateCur->UpdateFeature( ipFeature ) ) )
		{
			ERRLOGGER.error( lane_link::kTableName, ToString( ipFeature->GetOID() ), gid, _T( "�X�L�b�v�F�t�B�[�`���[�X�V�Ɏ��s" ), CString(), CString() );
			continue;
		}

		ERRLOGGER.info( lane_link::kTableName, ToString( ipFeature->GetOID() ), gid, format( _T( "�X�V�����F%s" ), msg ), CString(), CString() );
	}

	return true;
}

bool CCorrectZTool::doLinerInterpolation()
{
	// �s�v�ȃ������̊J���{�g���񂵂̂��߁A�����I�Ƀ��Z�b�g
	m_updatedList.clear();

	// �}���z�����N���擾����
	map<long, IFeaturePtr> steepLinkMap;
	for( const auto& ids : getString( GidList( m_steepLink.begin(), m_steepLink.end() ) ) )
	{
		if( !GetFeatures( m_ipLaneLink, steepLinkMap, nullptr, format( _T( "%s IN (%s)" ), lane_link::kGlobalID, ids ) ) )
		{
			RUNLOGGER.writeInfo( format( _T( "%s�������s" ), lane_link::kTableName ) );
			return false;
		}
	}

	// �n�߂�O����}���z�̃����N���ΏۊO��������Ă����獢��̂őΏۊO����e���i�Ώۂɂ���j
	for( const auto& pair : steepLinkMap )
	{
		if( 1 == m_fixLink.count( pair.first ) )
			m_fixLink.erase( pair.first );
	}

	// �}���z�����N��1�{�����[�v����
	for( const auto& pair : steepLinkMap )
	{
		map<long, IFeaturePtr> laneLinkFeatureMap;
		laneLinkFeatureMap.emplace( pair.first, pair.second );

		// ���`�⊮�����Ԑ������N�Q
		map<long, IGeometryPtr> newGeomMap;

		// �}���z�����N�Ƃ�������FIX�����N�܂ł̂��ׂĂ̐ڑ������N����`��Ԃ���
		CRestrictableZValueAdjuster adjuster;
		adjuster.SetFixLink( m_fixLink );
		if( !adjuster.AdjustZValue( laneLinkFeatureMap, newGeomMap ) )
		{
			CString gid = pair.second->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
			ERRLOGGER.error( lane_link::kTableName, ToString( pair.first ), gid, _T( "�X�L�b�v�F���`��ԂɎ��s" ), CString(), CString() );
			continue;
		}

		// ���`��Ԃ��������N���m�ہi����m_fixLink��CRestrictableZValueAdjuster�ɓn����鎞�A���`��Ԃ��������N��FIX�����N�ɂȂ�悤�Ɂj
		// ���Ƃ��łɃL���X�g
		GidList linkGids;
		map<CString, IGeometryPtr> linkGeomMap;
		for( const auto& link : newGeomMap )
		{
			m_fixLink.emplace( link.first );

			CString gid = laneLinkFeatureMap[link.first]->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
			linkGids.emplace_back( gid );
			linkGeomMap.emplace( gid, link.second );
		}

		// �����N�X�V
		if( !reUpdateLinkZ( linkGids, linkGeomMap ) )
			return false;

		// �m�[�h����������ĕR�t���Ԑ��m�[�h�𒲂ׂ�
		GidList nodeGids;
		Node2LinkMap node2link;
		Link2NodeMap link2node;
		if( !searchNqTable( linkGids, nodeGids, node2link, link2node ) )
			return false;

		// �m�[�h�X�V
		if( !reUpdateNodeZ( nodeGids, node2link, link2node, linkGeomMap ) )
			return false;
	}

	return true;
}

bool CCorrectZTool::reUpdateLinkZ( const GidList& linkGids, const std::map<CString, IGeometryPtr>& newGeomMap )
{
	for( const auto& ids : getString( linkGids ) )
	{
		IFeatureCursorPtr ipUpdateCur;
		if( FAILED( m_ipLaneLink->Update(
			AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), lane_link::kGlobalID, ids ),
			VARIANT_FALSE, &ipUpdateCur ) ) || !ipUpdateCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s�������s" ), lane_link::kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipUpdateCur->NextFeature( &ipFeature ) && ipFeature )
		{
			CString gid = ipFeature->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
			IGeometryPtr ipNewGeo = newGeomMap.at( gid );

			if( !putValue( ipFeature, ipNewGeo, m_cLaneLinkFM ) || FAILED( ipUpdateCur->UpdateFeature( ipFeature ) ) )
			{
				ERRLOGGER.error( lane_link::kTableName, ToString( ipFeature->GetOID() ), gid, _T( "�X�L�b�v�F�t�B�[�`���[�X�V�Ɏ��s" ), CString(), CString() );
				continue;
			}

			ERRLOGGER.info( lane_link::kTableName, ToString( ipFeature->GetOID() ), gid, format( _T( "�X�V�����F���`���" ) ), CString(), CString() );
		}
	}

	return true;
}

bool CCorrectZTool::reUpdateNodeZ( const GidList& nodeGids, const Node2LinkMap& node2link, const Link2NodeMap& link2node, const map<CString, IGeometryPtr>& linkGeomMap )
{
	// Z�l�������_�ȉ��扽�ʂŎl�̌ܓ����邩
	// DB����Z�l���W���x�̓s����Aget_Z()�Ŏ擾����Z�l�ƌv�Z�ŋ��߂�Z�l�������Ɉ�v���Ȃ����߁A
	// �ۂߍ��݂��s�Ȃ��Ĉ�v������s��
	// TODO: ���蓾�Ȃ��Ǝv�����AZ�l�̍��W���x�ɕύX���������ꍇ�́A��������ύX���邱��
	static const long lRoundDigit = 5;

	for( const auto& ids : getString( nodeGids ) )
	{
		IFeatureCursorPtr ipUpdateCur;
		if( FAILED( m_ipLaneNode->Update(
			AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), lane_node::kGlobalID, ids ),
			VARIANT_FALSE, &ipUpdateCur ) ) || !ipUpdateCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s�������s" ), lane_node::kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipUpdateCur->NextFeature( &ipFeature ) && ipFeature )
		{
			// SOURCEID != NULL �Ȃ�AT�m�[�h�Ȃ̂ŁA�X�V�ΏۊO
			CString sourceId = ipFeature->GetValue( m_cLaneNodeFM.HasField( lane_node::kSourceID ) ).bstrVal;
			if( !sourceId.IsEmpty() )
				continue;

			CString nodeGid = ipFeature->GetValue( m_cLaneNodeFM.HasField( lane_node::kGlobalID ) ).bstrVal;
			if( 1 == m_updatedList.count( sourceId ) )
			{
				ERRLOGGER.warning( lane_node::kTableName, ToString( ipFeature->GetOID() ), sourceId, _T( "�X�L�b�v�F���ɐ��`��ԍς�" ), CString(), CString() );
				continue;
			}

			// �m�[�h������Ă��郊���N���擾
			// ��ԓ_�m�[�h�ɑ΂��ẮA�����N��1�{�������݂��Ȃ��͂�
			// FromTo�m�[�h�ɑ΂��ẮA���������N�����݂��邪�A
			// �ǂ̃����N�̍\���_��Z�l�͓����i�C������Đڑ����Ă���j�͂��Ȃ̂ŁA�ŏ���1�{�����
			set<CString>::const_iterator it = node2link.at( nodeGid ).begin();
			CString linkGid = *it;

			// �����N�̍\���_��Z�l���擾
			// �\���_�̔ԍ��̓m�[�h��SEQUENCE�ƈ�v���Ă���O��
			IPointCollectionPtr ipPointCol( linkGeomMap.at( linkGid ) );
			long targetNum = link2node.at( linkGid ).at( nodeGid );
			IPointPtr ipPoint = ipPointCol->GetPoint( targetNum );
			double linkZ = ipPoint->GetZ();

			// �m�[�h��Z�l���擾
			IGeometryPtr ipNewGeo = ipFeature->GetShapeCopy();
			double nodeZ = IPointPtr( ipNewGeo )->GetZ();

			// �����N�̍\���_�ƃm�[�h��Z�l���ꏏ�Ȃ�X�L�b�v
			if( roundingOff( linkZ, lRoundDigit ) == roundingOff( nodeZ, lRoundDigit ) )
				continue;

			if( FAILED( IPointPtr( ipNewGeo )->put_Z( linkZ ) ) ||
				!putValue( ipFeature, ipNewGeo, m_cLaneLinkFM ) ||
				FAILED( ipUpdateCur->UpdateFeature( ipFeature ) ) )
			{
				ERRLOGGER.error( lane_node::kTableName, ToString( ipFeature->GetOID() ), nodeGid, _T( "�X�L�b�v�F�t�B�[�`���[�X�V�Ɏ��s" ), CString(), CString() );
				continue;
			}

			ERRLOGGER.info( lane_node::kTableName, ToString( ipFeature->GetOID() ), nodeGid, format( _T( "�X�V�����F���`���" ) ), CString(), CString() );
			m_updatedList.emplace( nodeGid );
		}
	}

	return true;
}

void CCorrectZTool::getBorderLink( const CString& linkGid, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long lrCode, FeatureList& borders )
{
	auto range = lane2border.at( linkGid ).equal_range( lrCode );
	for( auto it = range.first; it != range.second; ++it )
	{
		if( 0 == borderLinks.count( it->second ) )
			continue;

		borders.emplace_back( borderLinks.at( it->second ) );
	}
}

bool CCorrectZTool::putValue( IFeaturePtr& ipFeature, const IGeometryPtr& ipGeo, const sindy::CFieldMap& fieldMap )
{
	// �����Z�b�g
	if( FAILED( ipFeature->put_Value( fieldMap.HasField( ipc_feature::kOperator ),			CComVariant( format( _T( "sindy" ) ) ) ) ) ||
		FAILED( ipFeature->put_Value( fieldMap.HasField( ipc_feature::kUpdateType ),		CComVariant( ipc_feature::update_type::kShapeUpdated ) ) ) || // 3�F�`��ύX
		FAILED( ipFeature->put_Value( fieldMap.HasField( ipc_feature::kProgModifyDate ),	CComVariant( log_utility::GetDateTimeInfo() ) ) ) ||
		FAILED( ipFeature->put_Value( fieldMap.HasField( ipc_feature::kModifyProgName ),	CComVariant( m_toolName ) ) ) ||
		FAILED( ipFeature->putref_Shape( ipGeo ) ) )
		return false;

	return true;
}

list<CString> CCorrectZTool::getString( const list<CString>& target, const CString& sep /* =_T("'") */ )
{
	list<CString> strings;
	IDs2Str( target, strings, sep );
	
	return strings;
}

list<CString> CCorrectZTool::getString( const list<long>& target )
{
	list<CString> strings;
	IDs2Str( target, strings );

	return strings;
}



bool CCorrectZTool::openWorkspace( const CString& db, IWorkspacePtr& ipWorkspace )
{
	ipWorkspace = sindy::create_workspace( _bstr_t( db ) );
	if( !ipWorkspace )
	{
		RUNLOGGER.writeInfo( format( _T( "%s�ڑ����s" ), db ) );
		return false;
	}

	return true;
}

bool CCorrectZTool::isDefaultVersion( const IWorkspacePtr& ipWorkspace )
{
	IVersionedWorkspacePtr ipVerWorkspace( ipWorkspace );
	if( !ipVerWorkspace ) return false;

	IVersionPtr ipVersion( ipWorkspace );
	CComBSTR bstr;
	ipVersion->get_VersionName( &bstr );
	CString strVerName = COLE2T( bstr );
	if( strVerName.Find( _T( "DEFAULT" ) ) != -1 )
	{
		RUNLOGGER.writeInfo( _T( "�ڑ��悪DEFAULT�o�[�W����" ) );
		return true;
	}

	return false;
}

bool CCorrectZTool::getMeshList( const CString& fileName, MeshList& meshList )
{
	if( fileName.IsEmpty() )
		return false;

	ifstream ifs( fileName );
	if( !ifs )
		return false;

	string line;
	while( getline( ifs, line ) )
	{
		// ��̍s��s���u#�v�͖���
		if( line.size() == 0 || line.at( 0 ) == '#' )
			continue;

		meshList.emplace_back( stoul( line ) );
	}
	if( ifs.fail() && !ifs.eof() )
		return false;

	meshList.sort();

	return true;
}

bool CCorrectZTool::getMeshPolygon( const MeshList& meshList, MeshGeoMap& meshGeoMap )
{
	long count = 0; // �����������b�V����
	CString subFieleds = format( _T( "%s,%s" ), category::mesh_code_table::kMeshCode, feature::kShape );

	// 1000���b�V�����Ɍ���
	for( const auto& str : getString( meshList ) )
	{
		IFeatureCursorPtr ipFeatureCur;
		if( FAILED( m_ipMesh->Search(
			AheInitQueryFilter( nullptr, subFieleds, _T( "%s IN (%s)" ), category::mesh_code_table::kMeshCode, str ),
			VARIANT_FALSE, &ipFeatureCur ) ) || !ipFeatureCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s�������s" ), basemesh::kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
		{
			CComVariant var = ipFeature->GetValue( m_cMeshFM.FindField( category::mesh_code_table::kMeshCode ) );
			var.ChangeType( VT_I4 );
			meshGeoMap.emplace( var.lVal, ipFeature->GetShape() );
			++count;
		}
		system( "cls" );
		cout << "���b�V����������... " + to_string( count ) + " / " + to_string( meshList.size() ) << endl;
	}

	return true;
}

ITablePtr CCorrectZTool::openTable( const IWorkspacePtr& ipWorkspace, const CString& tableName, sindy::CFieldMap& fieldMap )
{
	ITablePtr ipTable;
	if( FAILED( IFeatureWorkspacePtr( ipWorkspace )->OpenTable( _bstr_t( tableName ), &ipTable ) ) || !ipTable )
	{
		RUNLOGGER.writeInfo( format( _T( "%s�擾���s" ), tableName ) );
		return nullptr;
	}
	fieldMap.CreateFieldMap( ipTable->GetFields() );

	return ipTable;
}

bool CCorrectZTool::isVersioned( const IFeatureClassPtr& ipFeatureClass )
{
	VARIANT_BOOL vbIsVersioned = VARIANT_FALSE;
	IVersionedObjectPtr( ipFeatureClass )->get_IsRegisteredAsVersioned( &vbIsVersioned );

	return vbIsVersioned ? true : false;
}

bool CCorrectZTool::startEdit( IWorkspacePtr& ipWorkspace )
{
	IMultiuserWorkspaceEditPtr ipMultiuserWorkspace( ipWorkspace );
	if( ipMultiuserWorkspace )
	{
		// �Ԑ������N�̃o�[�W�����Ή����\�Ƃ��Č��Ă���
		esriMultiuserEditSessionMode mode = isVersioned( m_ipLaneLink ) ? esriMESMVersioned : esriMESMNonVersioned;
		if( FAILED( ipMultiuserWorkspace->StartMultiuserEditing( mode ) ) )
		{
			RUNLOGGER.writeInfo( _T( "�ҏW�J�n���s" ) );
			return false;
		}

		if( mode == esriMESMVersioned )
		{
			if( FAILED( IWorkspaceEditPtr( ipWorkspace )->StartEditOperation() ) )
			{
				RUNLOGGER.writeInfo( _T( "�ҏW�J�n���s" ) );
				return false;
			}
		}
	}
	else
	{
		if( FAILED( IWorkspaceEditPtr( ipWorkspace )->StartEditing( VARIANT_FALSE ) ) )
		{
			RUNLOGGER.writeInfo( _T( "�ҏW�J�n���s" ) );
			return false;
		}
	}

	return true;
}

bool CCorrectZTool::stopEdit( IWorkspacePtr& ipWorkspace )
{
	IMultiuserWorkspaceEditPtr ipMultiuserWorkspace( ipWorkspace );
	if( ipMultiuserWorkspace && isVersioned( m_ipLaneLink ) )
	{
		if( FAILED( IWorkspaceEditPtr( ipWorkspace )->StopEditOperation() ) )
		{
			RUNLOGGER.writeInfo( _T( "�ҏW�I�����s" ) );
			abortEdit( ipWorkspace );
			return false;
		}
	}

	if( FAILED( IWorkspaceEditPtr( ipWorkspace )->StopEditing( VARIANT_TRUE ) ) )
	{
		RUNLOGGER.writeInfo( _T( "�ҏW�I�����s" ) );
		abortEdit( ipWorkspace );
		return false;
	}

	return true;
}

void CCorrectZTool::abortEdit( IWorkspacePtr& ipWorkspace )
{
	VARIANT_BOOL vb = VARIANT_FALSE;
	IWorkspaceEditPtr( ipWorkspace )->IsBeingEdited( &vb );
	if( !vb )
		return;

	if( FAILED( IWorkspaceEditPtr( ipWorkspace )->AbortEditOperation() ) ||
		FAILED( IWorkspaceEditPtr( ipWorkspace )->StopEditing( VARIANT_FALSE ) ) )
		RUNLOGGER.writeInfo( _T( "�ҏW�j�����s" ) );
}