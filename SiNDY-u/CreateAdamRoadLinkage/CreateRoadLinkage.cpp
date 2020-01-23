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

#include <atltime.h>
#include <VersionInfo.h>

// boost
#include <boost/assign/list_of.hpp>

// ArcHelperEx
#include <GlobalFunctions.h>
#include <AheGlobalsMisc.h>
#include <AheLayerFunctions.h>
#include <AheGeometryOp.h>

// useful_headders
#include <TDC/useful_headers/str_util.h>

// sindylib_base
#include <TDC/sindylib_base/TableContainer.h>

#include "Logger.h"
#include "CreateRoadLinkage.h"
#include "FeatureReader.h"
#include "RoadLinkMatcher.h"
#include "Util.h"

namespace {
	// ���H�����N�̃}�b�`���O�͈�
	const double MATCHING_METER = 30.0;
	// ���H�����N�̓ǂݍ��ݎ��̌�������
	const double SEARCH_METER = MATCHING_METER + 10.0;
}

// ����FGDB�X�L�[�}
namespace diff_db {
	const CString kChangeType = _T( "CHANGE_TYPE" );	// �����^�C�v
	const CString kObjectIdOrg = _T( "OBJECTID_ORG" );	// ���f�[�^��OBJECTID

	namespace change_type_code {
		enum ECode {
			kNew = 1,		// �ǉ�
			kDelete = 2,	// �폜
			kUpdate = 3,	// �X�V
		};
	}
}

bool CCreateRoadLinkage::execute()
{
	if(! initLog()){
		finish(false);
		return false;
	}

	if(! openTable()){
		finish(false);
		return false;
	}

	if(! startEdit()){
		finish(false);
		return false;
	}

	if(! createLinkage()){
		abortEdit();
		finish(false);
		return false;
	}

	stopEdit();
	finish(true);

	return true;
}


bool CCreateRoadLinkage::initLog() const
{
	// �������O�t�@�C���I�[�v��
	if(! RUNLOGGER.Initialize( m_Args.m_LogDir ))
	{
		std::cerr <<  "�������O�t�@�C���̃I�[�v���Ɏ��s���܂��� : " <<  uh::toStr( m_Args.m_LogDir.c_str() ) << std::endl;
		return false;
	}

	// �c�[�����s���o��
	RUNLOGGER.Log( _T("�ڑ���DB(���H):\t%s"),               m_Args.m_RoadDB.c_str()          );
	RUNLOGGER.Log( _T("�ڑ���DB(ADAM):\t%s"),               m_Args.m_AdamDB.c_str()          );
	RUNLOGGER.Log( _T("�ڑ���DB(�`�󍷕�):\t%s"),            m_Args.m_DiffDB.c_str()          );
	RUNLOGGER.Log( _T("���b�V�����X�g:\t%s"),               m_Args.m_MeshListStr.c_str()     );
	RUNLOGGER.Log( _T("�R�t���Ώۓ��H�����N��������:\t%s"), m_Args.m_TargetRoadWhere.c_str() );
	RUNLOGGER.Log( _T("���O�o�̓t�H���_�p�X:\t%s"),         m_Args.m_LogDir.c_str()          );

	// �G���[���O�t�@�C���I�[�v��
	if(! ERRLOGGER.Initialize( m_Args.m_LogDir ))
	{
		RUNLOGGER.Error( _T("�G���[���O�t�@�C���̃I�[�v���Ɏ��s���܂��� : ") + m_Args.m_LogDir );
		return false;
	}

	RUNLOGGER.Info(_T("���s�J�n "));

	return true;
}

bool CCreateRoadLinkage::openTable()
{
	using namespace sindy::schema;
	using namespace sindy::schema::category;
	using namespace sindy::schema::adam_v2;
	using namespace boost::assign;

	// ���HDB�ڑ�
	IWorkspacePtr road_ws = sindy::create_workspace( m_Args.m_RoadDB.c_str() );
	if(! road_ws){
		RUNLOGGER.Error( _T("���HDB�ւ̐ڑ��Ɏ��s���܂��� : ") + m_Args.m_RoadDB );
		return false;
	}
	// ���H�֘A�e�[�u���I�[�v��
	if(FAILED(IFeatureWorkspacePtr(road_ws)->OpenTable(CComBSTR(road_link::kTableName), &m_RoadLinkTable))){
		RUNLOGGER.Error( _T("���H�����N�̃I�[�v���Ɏ��s���܂���"));
		return false;
	}
	if(FAILED(IFeatureWorkspacePtr(road_ws)->OpenTable(CComBSTR(basemesh::kTableName), &m_BaseMeshTable)))
	{
		m_BaseMeshTable = AheOpenTableByTableName( road_ws, basemesh::kTableName, TRUE );
		if( ! m_BaseMeshTable ){
			RUNLOGGER.Error( _T("�x�[�X���b�V���̃I�[�v���Ɏ��s���܂���"));
			return false;
		}
	}

	// ADAMDB�ڑ�
	if(! m_Workspace.open(m_Args.m_AdamDB.c_str())){
		RUNLOGGER.Error( _T("ADAM DB�ւ̐ڑ��Ɏ��s���܂��� : ") + m_Args.m_AdamDB );
		return false;
	}

	// SDE�Ȃ�Ύq�o�[�W�������쐬���ĕҏW���s�����[�h�ɂ���
	if(m_Workspace.isSDE())
	{
		const std::vector<CString> edit_tables = list_of( CString( rel_road_link_lane_link::kTableName ) );
		if(! m_Workspace.childCreateModeOn( getChildVersionName(), edit_tables ) )
		{
			RUNLOGGER.Error( _T("���H�����N�|�Ԑ������N�֘A�e�[�u���̓o�[�W�����Ή��ł͂���܂���") );
			return false;
		}
	}

	// ADAM�֘A�e�[�u���I�[�v��
	m_LaneLinkTable = m_Workspace.openTable(lane_link::kTableName);
	if(! m_LaneLinkTable){
		RUNLOGGER.Error( _T("�Ԑ������N�̃I�[�v���Ɏ��s���܂���"));
		return false;
	}
	m_RelRoadLinkLaneLinkTable = m_Workspace.openTable(rel_road_link_lane_link::kTableName);
	if(! m_RelRoadLinkLaneLinkTable){
		RUNLOGGER.Error( _T("���H�����N�|�Ԑ������N�֘A�e�[�u���̃I�[�v���Ɏ��s���܂���"));
		return false;
	}

	// ���H�����N�|�Ԑ������N�֘A�e�[�u���A�Ԑ������N�̃t�B�[���h�}�b�v�A�x�[�X���b�V���̃t�B�[���h�}�b�v���擾����
	auto getFieldMap = []( ITablePtr table ){
		sindy::CFieldMap fm;
		fm.CreateFieldMap( table->GetFields() );
		return fm;
	};
	m_RelRoadLinkLaneLinkFM = getFieldMap( m_RelRoadLinkLaneLinkTable );
	m_LaneLinkFM = getFieldMap( m_LaneLinkTable );
	m_BaseMeshFM = getFieldMap( m_BaseMeshTable );

	// �Ԑ������N�̋�ԎQ�Ƃ��擾����
	m_LaneSpRef = IGeoDatasetPtr( m_LaneLinkTable )->GetSpatialReference();

	// �`�󍷕�FGDB���w�肳��Ă��Ȃ���΂�����return
	if( m_Args.m_DiffDB.empty() )
		return true;


	// �`�󍷕�FGDB�ڑ�
	IWorkspacePtr ipDiffWS = sindy::create_workspace( m_Args.m_DiffDB.c_str() );
	if( !ipDiffWS ) {
		RUNLOGGER.Error( _T( "�`�󍷕�FGDB�ւ̐ڑ��Ɏ��s���܂��� : " ) + m_Args.m_DiffDB );
		return false;
	}

	// �`�󍷕��e�[�u���I�[�v��
	ITablePtr ipRoadTable, ipLaneTable;
	IFeatureWorkspacePtr( ipDiffWS )->OpenTable( CComBSTR( road_link::kTableName ), &ipRoadTable );
	IFeatureWorkspacePtr( ipDiffWS )->OpenTable( CComBSTR( lane_link::kTableName ), &ipLaneTable );
	// �ǂ��炩�Е��������Ă���󋵂łȂ���ΑS��NG
	if( !ipRoadTable && !ipLaneTable ) {
		RUNLOGGER.Error( _T( "�`�󍷕��e�[�u���̃I�[�v���Ɏ��s���܂���" ) );
		return false;
	}
	if( ipRoadTable && ipLaneTable ) {
		RUNLOGGER.Error( _T( "���H�����N�ƎԐ������N�̌`�󍷕��������Ƃ����݂��Ă��܂�" ) );
		return false;
	}
	m_DiffTable = ipRoadTable ? ipRoadTable : ipLaneTable;

	// �`�󍷕��e�[�u���̃t�B�[���h�}�b�v���擾����
	m_DiffFM = getFieldMap( m_DiffTable );

	// �`�󍷕��e�[�u���̋�ԎQ�Ƃ��擾����
	m_DiffSpRef = IGeoDatasetPtr( m_DiffTable )->GetSpatialReference();

	return true;
}

bool CCreateRoadLinkage::startEdit()
{
	if( ! m_Workspace.startEditing() ){
		RUNLOGGER.Error( _T("�ҏW�J�n�Ɏ��s���܂���") );
		return false;
	}
	if( ! m_Workspace.startEditOperation() ) {
		m_Workspace.stopEditing(false);
		RUNLOGGER.Error( _T("�ҏW�J�n�Ɏ��s���܂���") );
		return false;
	}
	return true;
}

void CCreateRoadLinkage::stopEdit()
{
	if (! m_Workspace.stopEditOperation()){
		m_Workspace.stopEditing(false);
		RUNLOGGER.Error( _T("�ҏW�I���Ɏ��s���܂���") );
		return;
	}
	if (! m_Workspace.stopEditing(true)){
		RUNLOGGER.Error( _T("�ҏW�I���Ɏ��s���܂���") );
	}
}

void CCreateRoadLinkage::abortEdit()
{
	RUNLOGGER.Error( _T("�ҏW�Ɏ��s���܂���") );
	if (! m_Workspace.abortEditOperation() ) {
		m_Workspace.stopEditing(false);
		RUNLOGGER.Error( _T("�ҏW�I���Ɏ��s���܂���") );
		return;
	}
	if (! m_Workspace.stopEditing(false)) {
		RUNLOGGER.Error( _T("�ҏW�I���Ɏ��s���܂���") );
	}
}

bool CCreateRoadLinkage::createLinkage()
{
	// ���b�V���|���S���擾
	std::map<long, IGeometryPtr> meshGeoMap;
	if( !getMeshPolygon( m_Args.m_MeshList, meshGeoMap ) )
		return false;

	// ���b�V���P�ʂŎԐ������N->���H�����N�̃}�b�`���O������s��
	for( const auto& mesh : meshGeoMap )
	{
		std::vector<IFeaturePtr> lanelinkFeatures;	// �R�t���Ώۂ̎Ԑ������N
		std::multimap<CString, long> lane2Road;		// �폜�����֘A�iKey�F�Ԑ������N�AValue�F���H�����N�j

		// �`�󍷕�FGDB�w��Ȃ�
		if( !m_DiffTable )
		{
			if( !run4NotUseDiff( mesh, lanelinkFeatures ) )
				return false;
		}
		// �`�󍷕�FGDB�w�肠��
		else
		{
			if( !run4UseDiff( mesh, lanelinkFeatures, lane2Road ) )
				return false;
		}

		std::multimap<CString, long> existRel;		// �֘A�e�[�u���ɑ��݂���֘A�iKey�F�Ԑ������N�AValue�F���H�����N�j

		// �d���C���|�[�g�h�~�΍􏈗�
		preventDuplication( lanelinkFeatures, existRel );

		// ���H�����N - �Ԑ������N�̕R�t������
		if( !createRel( lanelinkFeatures, lane2Road, existRel, mesh.first ) )
			return false;
	}

	return true;
}

bool CCreateRoadLinkage::run4NotUseDiff( const std::pair<long, IGeometryPtr>& mesh, std::vector<IFeaturePtr>& lanelinkFeatures ) const
{
	// �����Ώۃ��b�V�����̎Ԑ������N�ǂݍ���
	FeatureReader laneLinkReader( m_LaneLinkTable, m_BaseMeshTable );
	if( !laneLinkReader.read( mesh.second ) )
	{
		RUNLOGGER.Error( _T( "�Ԑ������N�̓ǂݍ��݂Ɏ��s���܂��� : " ) + uh::toTStr( std::to_string( mesh.first ) ) );
		return false;
	}
	// 1���b�V�����̎Ԑ������N�擾
	lanelinkFeatures = laneLinkReader.getFeatures();

	return true;
}

bool CCreateRoadLinkage::run4UseDiff(
	const std::pair<long, IGeometryPtr>& mesh,
	std::vector<IFeaturePtr>& lanelinkFeatures,
	std::multimap<CString, long>& lane2Road
) const
{
	using namespace sindy::schema::adam_v2;
	using namespace Util;

	// 1���b�V�����̍������R�[�h�擾
	std::map<long, std::vector<IFeaturePtr>> diffMap;
	if( !getDiff( diffMap, mesh.second ) )
		return false;

	std::map<CString, Info4ErrLog> lane4DeleteRel;	// �Ԑ������N�Ə��i�֘A�폜�Ώہj

	// ���H�����N���Ԑ������N���ŏ����𕪂���
	if( isRoadDiff() )
	{
		// �`�󍷕��𕪐�
		std::vector<long> orgOids;
		analyzeRoadDiff( diffMap, orgOids );

		// �R�t���̂���֘A���R�[�h�̂ݍ폜
		if( !deleteRel( getWhereClauseINSearch( orgOids, rel_road_link_lane_link::kRoadLinkID ), lane2Road ) )
			return false;

		// �R�t���Ώۂ̎Ԑ������N�ƁA�폜���ꂽ�Ԑ������N�̏����擾����
		if( !getLinkInfo4CreateAndDeleted( lane2Road, mesh.first, lanelinkFeatures, lane4DeleteRel ) )
			return false;
	}
	else
	{
		// �����Ώۃ��b�V�����̎Ԑ������N�ǂݍ���
		FeatureReader laneLinkReader( m_LaneLinkTable, m_BaseMeshTable );
		if( !laneLinkReader.read( mesh.second ) )
		{
			RUNLOGGER.Error( _T( "�Ԑ������N�̓ǂݍ��݂Ɏ��s���܂��� : " ) + uh::toTStr( std::to_string( mesh.first ) ) );
			return false;
		}
		std::map<CString, IFeaturePtr> baseLane;
		getGid2Feature( laneLinkReader.getFeatures(), m_LaneLinkFM, baseLane );

		// �`�󍷕��𕪐�
		analyzeLaneDiff( diffMap, baseLane, lanelinkFeatures, lane4DeleteRel );

		std::set<CString> gids;
		for( const auto& pair : lane4DeleteRel )
			gids.emplace( pair.first );

		// GID ����v���铹�H�����N�|�Ԑ������N�֘A�e�[�u���̃��R�[�h���폜
		if( !deleteRel( getWhereClauseINSearch( gids, rel_road_link_lane_link::kLaneLinkGID ), lane2Road ) )
			return false;
	}

	// �폜�����֘A�ɂ��ăG���[���O�ɏo��
	writeDeletedRelInfo( lane4DeleteRel, lane2Road, mesh.first );

	return true;
}

void CCreateRoadLinkage::analyzeLaneDiff(
	const std::map<long, std::vector<IFeaturePtr>>& diffMap,
	const std::map<CString, IFeaturePtr>& baseLane,
	std::vector<IFeaturePtr>& lanelinkFeatures,
	std::map<CString, Info4ErrLog>& lane4DeleteRel
) const
{
	using namespace sindy::schema::adam_v2;
	using namespace diff_db::change_type_code;
	using namespace Util;

	// �����̎�ޖ��Ƀ��[�v
	for( const auto& pair : diffMap )
	{
		// �ǉ��E�X�V
		if( pair.first == kNew || pair.first == kUpdate )
		{
			for( const auto& feature : pair.second )
			{
				CString gid = feature->GetValue( m_DiffFM.HasField( lane_link::kGlobalID ) ).bstrVal;
				if( 0 == baseLane.count( gid ) )
					continue;
				lanelinkFeatures.emplace_back( baseLane.at( gid ) );
			}
		}

		// �폜�E�X�V
		if( pair.first == kDelete || pair.first == kUpdate )
		{
			for( const auto& feature : pair.second )
			{
				CString gid = feature->GetValue( m_DiffFM.HasField( lane_link::kGlobalID ) ).bstrVal;

				Info4ErrLog info;
				if( pair.first == kUpdate )
				{
					info.laneOid = baseLane.at( gid )->GetOID();
					info.msg = _T( "�ĕR�t���{" );
					getXY( baseLane.at( gid ), info.x, info.y );
				}
				else
				{
					info.laneOid = feature->GetValue( m_DiffFM.HasField( diff_db::kObjectIdOrg ) ).lVal; // ��OID
					info.msg = _T( "�֘A�폜�̂ݎ��{" );
					getXY( feature, info.x, info.y, m_LaneSpRef ); // ADAM���̋�ԎQ�Ƃ𓖂Ă�
				}

				lane4DeleteRel.emplace( gid, info );
			}
		}
	}
}

void CCreateRoadLinkage::analyzeRoadDiff(
	const std::map<long, std::vector<IFeaturePtr>>& diffMap,
	std::vector<long>& orgOids
) const
{
	using namespace sindy::schema::adam_v2;
	using namespace diff_db::change_type_code;
	
	// �����̎�ޖ��Ƀ��[�v
	for( const auto& pair : diffMap )
	{
		// �폜�E�X�V�łȂ���΃X�L�b�v�i�ǉ��͎g��Ȃ��j
		if( pair.first != kDelete && pair.first != kUpdate )
			continue;

		for( const auto& feature : pair.second )
		{
			long orgOid = feature->GetValue( m_DiffFM.HasField( diff_db::kObjectIdOrg ) ).lVal;
			orgOids.emplace_back( orgOid );
		}
	}
}

bool CCreateRoadLinkage::deleteRel( const std::vector<CString>& wheres, std::multimap<CString, long>& lane2Road ) const
{
	using namespace sindy::schema::adam_v2;

	for( const auto& where : wheres )
	{
		_ICursorPtr ipUpdateCursor;
		if( FAILED( m_RelRoadLinkLaneLinkTable->Update(
			AheInitQueryFilter( nullptr, nullptr, where ), VARIANT_FALSE, &ipUpdateCursor ) ) &&
			!ipUpdateCursor )
		{
			RUNLOGGER.Error( _T( "���H�����N�|�Ԑ������N�֘A�e�[�u���̓ǂݍ��݂Ɏ��s���܂���" ) );
			return false;
		}

		_IRowPtr ipRow;
		while( S_OK == ipUpdateCursor->NextRow( &ipRow ) && ipRow )
		{
			// �폜�����֘A������ŏo�͂��邽�ߕێ�
			CString lane = ipRow->GetValue( m_RelRoadLinkLaneLinkFM.HasField( rel_road_link_lane_link::kLaneLinkGID ) ).bstrVal;
			long road = ipRow->GetValue( m_RelRoadLinkLaneLinkFM.HasField( rel_road_link_lane_link::kRoadLinkID ) ).lVal;
			lane2Road.emplace( lane, road );

			if( FAILED( ipUpdateCursor->DeleteRow() ) )
			{
				RUNLOGGER.Error( _T( "���H�����N�|�Ԑ������N�֘A�e�[�u���̍폜�Ɏ��s���܂���" ) );
				return false;
			}
		}
	}

	return true;
}

bool CCreateRoadLinkage::getLinkInfo4CreateAndDeleted(
	const std::multimap<CString, long>& lane2Road,
	long meshCode,
	std::vector<IFeaturePtr>& lanelinkFeatures,
	std::map<CString, Info4ErrLog>& lane4DeleteRel
) const
{
	using namespace sindy::schema::adam_v2;
	using namespace Util;

	std::set<CString> gids;
	for( const auto& pair : lane2Road )
		gids.emplace( pair.first );

	// ���H�������R�Â��Ă����Ԑ������N�̃t�B�[�`���[���擾
	FeatureReader laneLinkReader( m_LaneLinkTable, m_BaseMeshTable );
	std::map<CString, IFeaturePtr> linkedLane;
	for( const auto& where : getWhereClauseINSearch( gids, lane_link::kGlobalID ) )
	{
		if( !laneLinkReader.read( where ) )
		{
			RUNLOGGER.Error( _T( "�Ԑ������N�̓ǂݍ��݂Ɏ��s���܂��� : " ) + uh::toTStr( std::to_string( meshCode ) ) );
			return false;
		}
		getGid2Feature( laneLinkReader.getFeatures(), m_LaneLinkFM, linkedLane );
	}

	for( const auto& gid : gids )
	{
		// �R�Â��Ԑ������N�����ɍ폜����Ă���ꍇ�͍ĕR�t���܂łł��Ȃ�
		// ����FGDB�͓��H�Ȃ̂ŁA�Ԑ������N�̋�OID��XY�����Ȃ�
		// �Ԑ�FGDB�����HFGDB�̏��Ɏ��s����΂��̖��͋N���Ȃ����A�^�p���@�����܂��Ă��Ȃ��Ƃ̂��ƂȂ̂ŁA
		// ��U���̑Ή��Ƃ��Ă����i�Ή����@�ɂ��Ă͐������ƍ��Ӎς݁j
		if( 0 == linkedLane.count( gid ) )
		{
			Info4ErrLog info( -1, 0.0, 0.0, _T( "�֘A�폜�̂ݎ��{" ) );
			lane4DeleteRel.emplace( gid, info );
			continue;
		}
		// �R�t���Ώۂ��m��
		lanelinkFeatures.emplace_back( linkedLane.at( gid ) );

		// ���O�o�͗p�Ɋ֘A�����m��
		double dX = 0.0, dY = 0.0;
		getXY( linkedLane.at( gid ), dX, dY );
		Info4ErrLog info( linkedLane.at( gid )->GetOID(), dX, dY, _T( "�ĕR�t���{" ) );
		lane4DeleteRel.emplace( gid, info );
	}

	return true;
}

void CCreateRoadLinkage::writeDeletedRelInfo( const std::map<CString, Info4ErrLog>& lane4DeleteRel, const std::multimap<CString, long>& lane2Road, long meshCode ) const
{
	using namespace uh;

	// �폜�����֘A���i�Ԑ������N�ɑ΂��ĕR�t���Ă������H�����N�j���G���[���O�ɏo��
	for( const auto& pair : lane4DeleteRel )
	{
		// ���H�����N���擾
		std::set<long> oids;
		auto range = lane2Road.equal_range( pair.first );
		for( auto it = range.first; it != range.second; ++it )
			oids.emplace( it->second );

		CString strOids = str_util::join( oids, _T( "," ) );
		if( strOids.IsEmpty() )
			continue; // ������΍폜�����Ă��Ȃ��̂ŃX�L�b�v

		ERRLOGGER.Info( pair.second.laneOid, pair.first,
			meshCode, pair.second.x, pair.second.y,
			str_util::format( _T( "%s_�����HOID:%s" ), pair.second.msg, strOids ) );
	}
}

void CCreateRoadLinkage::preventDuplication( std::vector<IFeaturePtr>& lanelinkFeatures, std::multimap<CString, long>& existRel ) const
{
	if( m_DiffTable && isRoadDiff() )
	{
		// �`�󍷕�FGDB�����H�̎��́A�R�t�����c���Ă��Ă��ĕR�t���������̂ōi�荞�݂͂��Ȃ�
		// ���̑���A�d�����R�[�h���C���|�[�g���Ȃ��悤�A���̎��_�Ŋ֘A�̃L���b�V�������
		std::set<CString> dummy; // �_�~�[
		getExistRel( lanelinkFeatures, dummy, existRel );
	}
	else
	{
		// ���H�����N�|�Ԑ������N�֘A�e�[�u���ɕR�t�������݂��Ȃ����R�[�h�݂̂ɍi�荞��
		extractNotLinkedFeature( lanelinkFeatures );
	}
}

void CCreateRoadLinkage::extractNotLinkedFeature( std::vector<IFeaturePtr>& lane_features ) const
{
	using namespace sindy::schema::adam_v2;

	std::set<CString> existGids;
	std::multimap<CString, long> dummy; // �_�~�[
	getExistRel( lane_features, existGids, dummy );
	long indexLaneLinkGid = m_LaneLinkFM.FindField( lane_link::kGlobalID );

	// ���H�����N�|�Ԑ������N�֘A�e�[�u���ɕR�t����񂪑��݂���Ԑ������N���A�����Ώۃ��X�g(lane_features)����폜
	lane_features.erase( std::remove_if(
		std::begin( lane_features ), std::end( lane_features ), [&indexLaneLinkGid, &existGids]( const IFeaturePtr& feature )
	{
		CComVariant vaGid = feature->GetValue( indexLaneLinkGid );
		if( vaGid.vt == VT_NULL )
			return true;
		return existGids.count( vaGid.bstrVal ) != 0;
	} ),
		std::end( lane_features ) );
}

void CCreateRoadLinkage::getExistRel( const std::vector<IFeaturePtr>& lane_features, std::set<CString>& existGids, std::multimap<CString, long>& exsistRel ) const
{
	using namespace sindy::schema::adam_v2;

	// �Ԑ������N��GID���X�g���擾
	std::set<CString> gids;
	for( const auto& feature : lane_features ) {
		CComVariant vaGid = feature->GetValue( m_LaneLinkFM.FindField( lane_link::kGlobalID ) );
		if( vaGid.vt != VT_NULL )
			gids.emplace( vaGid.bstrVal );
	}

	// GID ����v���铹�H�����N�|�Ԑ������N�֘A�e�[�u���̃��R�[�h������
	std::vector<CString> whereClauses = getWhereClauseINSearch( gids, rel_road_link_lane_link::kLaneLinkGID );
	for( const auto& where_clause : whereClauses )
	{
		_ICursorPtr cursor;
		if( FAILED( m_RelRoadLinkLaneLinkTable->Search(
			AheInitQueryFilter( nullptr, nullptr, where_clause ),
			VARIANT_FALSE,
			&cursor ) ) )
		{
			return;
		}

		// �����������R�[�h��GID�Q���擾
		_IRowPtr row;
		while( SUCCEEDED( cursor->NextRow( &row ) ) && row ) {
			CComVariant vaGid = row->GetValue( m_RelRoadLinkLaneLinkFM.FindField( rel_road_link_lane_link::kLaneLinkGID ) );
			CComVariant vaLinkId = row->GetValue( m_RelRoadLinkLaneLinkFM.HasField( rel_road_link_lane_link::kRoadLinkID ) );
			if( vaGid.vt != VT_NULL )
			{
				existGids.emplace( vaGid.bstrVal );
				exsistRel.emplace( vaGid.bstrVal, vaLinkId.lVal );
			}
		}
	}
}

std::vector<CString> CCreateRoadLinkage::getWhereClauseINSearch( const std::set<CString>& vals, const CString& field_name  ) const
{
	// IN���p�̕����񐶐�
	std::list<CString> listId(vals.begin(), vals.end());
	std::list<CString> inClauses;
	sindy::CTableBase::IDs2Str(listId, inClauses, _T("'"));

	return getWhereClauseINSearch( inClauses, field_name );
}

std::vector<CString> CCreateRoadLinkage::getWhereClauseINSearch( const std::vector<long>& vals, const CString& field_name ) const
{
	// IN���p�̕����񐶐�
	std::list<CString> listId;
	for( const auto& val : vals )
		listId.emplace_back( uh::str_util::ToString( val ) );
	std::list<CString> inClauses;
	sindy::CTableBase::IDs2Str( listId, inClauses );

	return getWhereClauseINSearch( inClauses, field_name );
}

std::vector<CString> CCreateRoadLinkage::getWhereClauseINSearch( const std::list<CString>& vals, const CString& field_name ) const
{
	std::vector<CString> retWhereClauses;
	for( const auto& where : vals )
	{
		CString whereClause = uh::str_util::format( _T( "%s in ( %s )" ), field_name, where );
		retWhereClauses.push_back( whereClause );
	}

	return retWhereClauses;
}

bool CCreateRoadLinkage::createRel(
	const std::vector<IFeaturePtr>& lanelinkFeatures,
	const std::multimap<CString, long> lane2Road,
	const std::multimap<CString, long> existRel,
	long meshCode
) const
{
	std::vector<InsertRecord> insertRecords;
	for( const auto& laneFeature : lanelinkFeatures )
	{
		// �Ԑ������N���ӂ̓��H�����N�ǂݍ���
		// �I�v�V�����Ŏw�肵���������l��
		FeatureReader roadLinkReader( m_RoadLinkTable );
		if( !roadLinkReader.readAround( laneFeature->GetShapeCopy(), SEARCH_METER, m_Args.m_TargetRoadWhere.c_str() ) )
			continue;
		std::vector<IFeaturePtr> roadFeatures = roadLinkReader.getFeatures();

		// �}�b�`���O����
		matchLaneLinkToRoadLinks( laneFeature, roadFeatures, lane2Road, existRel, meshCode, insertRecords );
	}

	// ���H�����N�|�Ԑ������N�֘A�e�[�u���֒ǉ�
	if( !insertRecord( insertRecords ) ) {
		RUNLOGGER.Error( _T( "���H�����N�|�Ԑ������N�֘A�e�[�u���̒ǉ��Ɏ��s���܂���" ) );
		return false;
	}

	return true;
}

void CCreateRoadLinkage::matchLaneLinkToRoadLinks(
	const IFeaturePtr& lane_link,
	const std::vector<IFeaturePtr> road_links,
	const std::multimap<CString, long>& lane2Road,
	const std::multimap<CString, long>& exsistRel,
	long base_mesh,
	std::vector<InsertRecord>& insert_records
) const
{
	using namespace uh;
	using namespace sindy::schema::adam_v2::lane_link;
	using namespace sindy::schema::category::adam_v2_link_code;
	using namespace sindy::schema::category::adam_v2_link_dir_code;
	using namespace Util;

	// �K�v�ȃt�B�[���h�̃C���f�b�N�X���擾
	long indexLaneLinkGid = m_LaneLinkFM.FindField( kGlobalID );
	long indexOnewayC = m_LaneLinkFM.FindField( kOneWayC );

	// �o�ܓx�i���O�o�͗p�j
	double x = 0.0, y = 0.0;
	getXY( lane_link, x, y );

	CComVariant vaGid = lane_link->GetValue(indexLaneLinkGid);
	CString laneGid;
	if( vaGid.vt != VT_NULL )
		laneGid = vaGid.bstrVal;

	// �Ԑ������N�Ɠ��H�����N�̕R�t���𔻒�
	// �R�t���Ȃ������ꍇ���m�F������FREESTYLE���O�֏o�͂���
	RoadLinkMatcher linkMatcher(m_RoadLinkTable, MATCHING_METER);
	bool isReverse(false);
	IFeaturePtr matchRoadFeature = linkMatcher.getMatchLink( lane_link, road_links, isReverse);

	if( !matchRoadFeature )
	{
		ERRLOGGER.Warn(lane_link->GetOID(), laneGid, base_mesh, x, y, str_util::format(_T("�R�Â����H�����N��������܂���ł���")));
		return;
	}

	// ���H�����̎������A���R�[�h�d�����Ȃ��悤�`�F�b�N
	if( m_DiffTable && isRoadDiff() )
	{
		std::set<long> delOids;
		auto range = lane2Road.equal_range( laneGid );
		for( auto itr = range.first; itr != range.second; ++itr )
			delOids.emplace( itr->second );

		// �֘A�e�[�u���ō폜�������H�����N��ID�ƈꏏ�Ȃ�X���[
		if( delOids.count( matchRoadFeature->GetOID() ) == 0 )
		{
			std::set<long> exOids;
			auto rg = exsistRel.equal_range( laneGid );
			for( auto it = rg.first; it != rg.second; ++it )
				exOids.emplace( it->second );

			// �֘A�e�[�u���̒��Ŋ����̕R�t��������Ȃ�X�L�b�v
			// �������A���ɍ폜���ꂽ�֘A�����邽�ߌx��
			if( exOids.count( matchRoadFeature->GetOID() ) == 1 )
			{
				ERRLOGGER.Warn(lane_link->GetOID(), laneGid, base_mesh, x, y, str_util::format(_T("�����̊֘A�̂����A�폜���ꂽ�֘A������܂�")));
				return;
			}
		}
	}

	ERRLOGGER.Info(lane_link->GetOID(), laneGid, base_mesh, x, y,
		str_util::format(_T("�R�Â����H�����N��������܂��� : ROAD_LINK_ID : %ld ( %s )"),
		matchRoadFeature->GetOID(),
		isReverse ? _T("�t����") : _T("������")));

	insert_records.emplace_back(matchRoadFeature->GetOID(),
		laneGid,
		isReverse ? roadlink_dir_code::kReverse : roadlink_dir_code::kForward);
}

bool CCreateRoadLinkage::insertRecord(const std::vector<InsertRecord>& insert_records ) const
{
	using namespace sindy::schema::adam_v2::rel_road_link_lane_link;

	_ICursorPtr cursor;
	if(FAILED(m_RelRoadLinkLaneLinkTable->Insert( VARIANT_TRUE, &cursor)) )
		return false;

	// �t�B�[���h�C���f�b�N�X�擾
	long indexOperator       = m_RelRoadLinkLaneLinkFM.FindField( kOperator );
	long indexProgModifyDate = m_RelRoadLinkLaneLinkFM.FindField( kProgModifyDate );
	long indexModifyProgName = m_RelRoadLinkLaneLinkFM.FindField( kModifyProgName );
	long indexUpdateType     = m_RelRoadLinkLaneLinkFM.FindField( kUpdateType );
	long indexRoadLinkId  = m_RelRoadLinkLaneLinkFM.FindField( kRoadLinkID );
	long indexLaneLinkGid = m_RelRoadLinkLaneLinkFM.FindField( kLaneLinkGID );
	long indexRoadLinkDC  = m_RelRoadLinkLaneLinkFM.FindField( kRoadLinkDirectionC );

	for( const auto& rec : insert_records )
	{
		IRowBufferPtr buffer;
		if( FAILED(m_RelRoadLinkLaneLinkTable->CreateRowBuffer( &buffer )) ) return false;

		if( FAILED(buffer->put_Value( indexOperator, CComVariant(_T("sindy")) ))) return false; 
		if( FAILED(buffer->put_Value( indexProgModifyDate, CComVariant( getCurrentTime())))) return false; 
		if( FAILED(buffer->put_Value( indexModifyProgName, CComVariant( CVersion().GetInternalName() )))) return false; 
		if( FAILED(buffer->put_Value( indexUpdateType, CComVariant( update_type::kCreated )))) return false; 

		if( FAILED(buffer->put_Value( indexRoadLinkId, CComVariant(rec.roadOid) ))) return false; 
		if( FAILED(buffer->put_Value( indexLaneLinkGid, CComVariant(rec.laneGid) ))) return false; 
		if( FAILED(buffer->put_Value( indexRoadLinkDC, CComVariant(rec.roadDC) ))) return false; 

		CComVariant oid;
		if( FAILED( cursor->InsertRow( buffer, &oid )) ) 
			return false;
	}

	return true;
}

bool CCreateRoadLinkage::getDiff( std::map<long, std::vector<IFeaturePtr>>& diffMap, const IGeometryPtr& mesh ) const
{
	using namespace sindy::schema::adam_v2;
	using namespace diff_db::change_type_code;

	// �������Ȃ����return
	if( !m_DiffTable )
		return true;

	// �ǂݍ���
	FeatureReader diffReader( m_DiffTable );
	if( !diffReader.read( mesh, CString() ) ) {
		RUNLOGGER.Error( _T( "�`�󍷕��e�[�u���̓ǂݍ��݂Ɏ��s���܂���" ) );
		return false;
	}

	for( const auto& feature : diffReader.getFeatures() )
	{
		long changeType = feature->GetValue( m_DiffFM.HasField( diff_db::kChangeType ) ).lVal;

		if( isRoadDiff() )
		{
			// ���H�����N�́u�ǉ��v�͎g��Ȃ�
			if( changeType == kNew )
				continue;

			// ���H�����N�̓��b�V�����E�Ń����N���؂�Ă��邪�A
			// �����̒i�K�ł́A���E�ɑ΂��ē����̃����N���O���̃����N���擾���Ă���
			// �����ŊO���̃����N��e��
			if( !AheIsContain( mesh, feature->GetShapeCopy() ) )
				continue;
		}

		diffMap[changeType].emplace_back( feature );
	}

	return true;
}

bool CCreateRoadLinkage::isRoadDiff() const
{
	using namespace sindy::schema::adam_v2;

	// ����������t���O�Ǘ��������Ȃ̂ł��̕��@�����
	return ( -1 == m_DiffFM.HasField( lane_link::kGlobalID ) );
}

bool CCreateRoadLinkage::getMeshPolygon( const std::vector<long>& meshList, std::map<long, IGeometryPtr>& meshGeoMap ) const
{
	using namespace sindy::schema;
	using namespace uh::str_util;

	CString subFieleds = format( _T( "%s,%s" ), category::mesh_code_table::kMeshCode, feature::kShape );
	std::vector<CString> whereClauses = getWhereClauseINSearch( meshList, category::mesh_code_table::kMeshCode );

	// 1000���b�V�����Ɍ���
	for( const auto& where : whereClauses )
	{
		IFeatureCursorPtr ipFeatureCur;
		if( FAILED( IFeatureClassPtr( m_BaseMeshTable )->Search(
			AheInitQueryFilter( nullptr, subFieleds, where ),
			VARIANT_TRUE, &ipFeatureCur ) ) || !ipFeatureCur )
		{
			RUNLOGGER.Error( _T( "���b�V���e�[�u���̓ǂݍ��݂Ɏ��s���܂���" ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
		{
			CComVariant var = ipFeature->GetValue( m_BaseMeshFM.FindField( category::mesh_code_table::kMeshCode ) );
			var.ChangeType( VT_I4 );
			meshGeoMap.emplace( var.lVal, ipFeature->GetShapeCopy() );
		}
	}

	return true;
}

CString CCreateRoadLinkage::getCurrentTime() const
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	return uh::str_util::format( _T("%d/%02d/%02d %02d:%02d:%02d")
        ,st.wYear ,st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
}

void CCreateRoadLinkage::finish(bool isSucceeded) const
{
	// �I�����b�Z�[�W�o��
	uh::tstring result = isSucceeded ? _T("����I��") : _T("�ُ�I��");
	RUNLOGGER.Info( _T("��������: ") + result);

	// �t�@�C���̃N���[�Y
	RUNLOGGER.Finalize();
	ERRLOGGER.Finalize();
}

CString CCreateRoadLinkage::getChildVersionName() const
{
	const CString prefix = _T("ETC_ADAMEDIT_");

	// ���݂̓�����"YYYYMMDDHHDDSS" �Ŏ擾
	CTime cTime = CTime::GetCurrentTime();
	const CString timeString = cTime.Format( _T("%Y%m%d%H%M%S") );

	return prefix + timeString;
}