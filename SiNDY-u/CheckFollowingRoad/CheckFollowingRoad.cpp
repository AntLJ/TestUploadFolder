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
#include "CheckFollowingRoad.h"
#include "ArcHelperEx/AheMeshUtilArcObjects.h"
#include "ArcHelperEx/AheGlobalsCoord.h"
#include "ArcHelperEx/GlobalFunctions.h"
#include "TDC/sindylib_base/ContainerBase.h"
#include "TDC/sindylib_base/RowContainer.h"
#include "TDC/sindylib_base/TableContainer.h"
#include "ArcHelperEx/AheMeshUtil.h"
#include "ErrTypeDef.h"
#include <sindy/libschema.h>

using namespace std;
using namespace uh;
using namespace uh::str_util;
using namespace uh::variant_util;
using namespace sindy;
using namespace sindy::schema;
using namespace err_type;

bool CCheckFollowingRoad::execute()
{
	// ���O����
	if( !preProcessing() )
		throw exception( "�������J�n�ł��܂���" );

	// ���b�V���P�ʂŏ��������{
	bool isSucceedAll = true;
	for( const long mesh : m_Meshes )
	{
		m_RunLogger.writeLogAndConsole( format( _T("meshcode: %d ...�����J�n"), mesh ) );
		bool isSucceed = doProcessing( mesh );
		m_RunLogger.writeLogAndConsole( 
			format( _T("meshcode: %d ...%s"), mesh, (isSucceed ? _T("����I��") : _T("�ُ�I��")) )
		);
		isSucceedAll &= isSucceed;
	}

	return isSucceedAll;
}

bool CCheckFollowingRoad::preProcessing()
{
	CWorkspace wsBaseDB, wsCompDB;
	
	// �eDB�ɐڑ��ł��邩�m�F
	bool isConnected = true;
	isConnected &= connectDB( m_Args.m_BaseDB, wsBaseDB );
	isConnected &= connectDB( m_Args.m_CompDB, wsCompDB );
	if( !isConnected )
		return false;

	// �m�[�h�ƃ����N���eDB�ɑ��݂��邩�m�F
	ITablePtr ipBaseNode, ipBaseLink, ipCompNode, ipCompLink;
	bool isExistTable = true;
	isExistTable &= openTargetTable( wsBaseDB, road_node::kTableName, ipBaseNode );
	isExistTable &= openTargetTable( wsCompDB, road_node::kTableName, ipCompNode );
	isExistTable &= openTargetTable( wsBaseDB, road_link::kTableName, ipBaseLink );
	isExistTable &= openTargetTable( wsCompDB, road_link::kTableName, ipCompLink );
	if( !isExistTable )
		return false;

	// ���f���ݒ�
	CModel model;
	model.Add( road_node::kTableName, road_node::kObjectID, nullptr, nullptr );
	model.Add( road_link::kTableName, road_link::kFromNodeID, road_node::kTableName, road_node::kObjectID );
	model.Add( road_link::kTableName, road_link::kToNodeID, road_node::kTableName, road_node::kObjectID );
	m_BaseFinder.InitCollection(ipBaseNode, model);
	m_CompFinder.InitCollection(ipCompNode, model);

	return true;
}

bool CCheckFollowingRoad::doProcessing( const long mesh )
{
	m_ErrLogger.write( format( _T("#%d"), mesh ) );

	auto& baseNodeT = *m_BaseFinder.FindTable( road_node::kTableName );
	baseNodeT.clear();
	auto& compNodeT = *m_CompFinder.FindTable( road_node::kTableName );
	compNodeT.clear();

	// ���Ȃ胊���N����`�F�b�N
	return chkDiffFollowingRoad( mesh, baseNodeT, compNodeT );
}

bool CCheckFollowingRoad::connectDB( const tstring& dbName, CWorkspace& ws )
{
	if( ws.Connect( dbName.c_str() ) )
		return true;

	const CString msg( format( _T("%s�ɐڑ��ł��܂���"), dbName.c_str() ) );
	m_RunLogger.writeErrMsg( msg );
	return false;
}

bool CCheckFollowingRoad::openTargetTable( const CWorkspace& ws, const tstring& tableName, ITablePtr& table )
{
	table = ws.OpenSameOwnersTable( tableName.c_str() );
	if( table )
		return true;

	const CString msg( format( _T("%s��%s�����݂��܂���"), ws.GetNameString()->GetPathName(), tableName.c_str() ) );
	m_RunLogger.writeErrMsg( msg );
	return false;
}

bool CCheckFollowingRoad::getFollowingRoad( const long nodeOID, const CTableContainer& linkT, FollowingRoad& followingRoad )
{
	// ���H�m�[�h���瓹�H�����N�̍\���_�܂ł̒������Ȃ��p�x�����ꂼ�ꋁ�߂�
	map<long, double> mapAngle; // (���H�����N��OID, ���H�m�[�h����\���_�܂ł̊p�x)
	for( auto& linkCon : linkT )
	{
		const auto& linkRow = *CAST_ROWC( linkCon );

		IPolylinePtr ipLinkLine( linkRow.CRowBase::GetShapeCopy() );
		if( !ipLinkLine )
		{
			const CString msg(
				format( _T("%s[%d]:�����N�`����擾�ł��܂���"), road_link::kTableName, linkRow.GetOID() ) 
				); 
			throw exception( toStr( msg.GetString() ).c_str() );
		}

		IPointCollectionPtr ipPointCol( ipLinkLine );
		long fromIdx = 0, toIdx = 1;
		// �������̓��H�m�[�h�����H�����N�̏I�_�ł���΁A�I�_������k��
		if( nodeOID == linkRow.GetValue(road_link::kToNodeID).lVal )
		{
			fromIdx = ipPointCol->GetPointCount() - 1;
			toIdx = ipPointCol->GetPointCount() - 2;
		}

		// ���H�m�[�h���̒[�_���擾
		IPointPtr ipFromPoint;
		if( FAILED( ipPointCol->get_Point( fromIdx, &ipFromPoint ) ) )
		{
			const CString msg(
				format( _T("%s[%d]:�m�[�h���̒[�_���擾�ł��܂���"), road_link::kTableName, linkRow.GetOID() )
				); 
			throw exception( toStr( msg.GetString() ).c_str() );
		}

		// 1�ׂ̍\���_���擾
		IPointPtr ipToPoint;
		if( FAILED( ipPointCol->get_Point( toIdx, &ipToPoint ) ) )
		{
			const CString msg(
				format( _T("%s[%d]:�\���_���擾�ł��܂���"), road_link::kTableName, linkRow.GetOID() )
				); 
			throw exception( toStr( msg.GetString() ).c_str() );
		}

		// FROM����TO�܂ł̊p�x[��]���擾
		double angle = getAngle( ipFromPoint, ipToPoint );

		mapAngle.emplace( make_pair( linkRow.GetOID(), angle ) );
	}

	// �y�A���ł��Ȃ��Ȃ�܂œ��Ȃ胊���N������
	const size_t followingPairNum = mapAngle.size() / 2;
	unsigned int getCnt = 0;
	while( getCnt < followingPairNum )
	{
		// �����N���m�̐ڑ��p�x�����߁A���Ȃ胊���N�����肷��
		multimap< double, pair<long, long> > mapDiffAngle;
		for( auto& it_angle1 = mapAngle.begin(); it_angle1 != mapAngle.end(); ++it_angle1 )
		{
			auto it_angle2 = it_angle1;
			++it_angle2;

			for( ; it_angle2 != mapAngle.end(); ++it_angle2 )
			{
				double diffAngle = abs( it_angle1->second - it_angle2->second );
				// 0�� �� diffAngle �� 180��
				if( 180 < diffAngle ) diffAngle = 360 - diffAngle;
				
				mapDiffAngle.emplace(make_pair(diffAngle, make_pair(it_angle1->first, it_angle2->first)));
			}
		}

		// �ő�ڑ��p�x�����y�A����������A
		// ���ꂪ(���݂���y�A�̐� - ����܂łɌ������y�A�̐�)���������Ȃ����ꍇ�́A
		// ���Ȃ胊���N������ł��Ȃ�
		double maxDiffAngle = mapDiffAngle.rbegin()->first;
		const size_t maxDiffCnt = mapDiffAngle.count( maxDiffAngle );
		if( (followingPairNum - getCnt) < maxDiffCnt )
			return false;

		auto& it_max = mapDiffAngle.find( maxDiffAngle );
		if( 1 < maxDiffCnt )
		{
			// �����̃y�A���������H�����N��ʂ�ꍇ�́A���Ȃ胊���N������ł��Ȃ�
			auto it = it_max;
			map<long, int> mapOidCnt;
			for( unsigned int i = 0; i < maxDiffCnt; ++i )
			{
				++mapOidCnt[it->second.first];
				++mapOidCnt[it->second.second];
				++it;
			}
			for( const auto& oid : mapOidCnt )
			{
				if( 1 < oid.second )
					return false;
			}
		}

		for( unsigned int i = 0; i < maxDiffCnt; ++i )
		{
			followingRoad.emplace( make_pair( it_max->second, it_max->first ) );
			// ���Ȃ胊���N��ƂȂ������H�����N�͏��O
			mapAngle.erase(it_max->second.first);
			mapAngle.erase(it_max->second.second);
			++it_max;
			++getCnt;
		}
	}

	return true;
}

double CCheckFollowingRoad::getAngle( const IPointPtr& fromPoint, const IPointPtr& toPoint )
{
	double fromX = -1.0, fromY = -1.0, toX = -1.0, toY = -1.0;
	fromPoint->QueryCoords( &fromX, &fromY );
	toPoint->QueryCoords( &toX, &toY );

	double distX = -1.0, distY = -1.0;
	g_cnv.GetDistXY(fromX, fromY, toX, toY, &distX, &distY );

	double angle = atan2( distY, distX );

	// 0 �� angle �� 2��
	if( 0 > angle )	angle += 2 * M_PI;
	
	// [rad]����[��]�ɕϊ�
	return (angle * 180 / M_PI);
}

bool CCheckFollowingRoad::hasChangedConnectedLink( const CTableContainer& baseLinkT, const CTableContainer& compLinkT )
{
	for( const auto& compLinkCon : compLinkT )
	{
		const auto& compLinkRow = *CAST_ROWC(compLinkCon);
		const long compLinkOID = compLinkRow.GetOID();

		// ��r��o�[�W�����̓��H�����N��OID�ŁA��r���o�[�W�����̓��H�����N������
		// �q�b�g���Ȃ������ꍇ�̓o�[�W�����Ԃŕω�����
		if( baseLinkT.end() == baseLinkT.find_by_key( road_link::kObjectID, compLinkOID ) )
			return true;
	}
	return false;
}

bool CCheckFollowingRoad::isModifiedSinceDesignedDate( const CRowContainer& nodeRow )
{
	// �ŏI�X�V�����w�肳��Ă��Ȃ���΁A�X�V���̃`�F�b�N�͂��Ȃ�
	if( isNullOrEmpty( m_Args.m_ModDate ) )
		return true;

	// �n�߂ɓ��H�m�[�h���`�F�b�N

	// �ŏI�X�V��
	const auto& nodeModDate = nodeRow.GetValue(road_node::kModifyDate);
	if( !isNullOrEmpty(nodeModDate) && m_Args.m_ModDate < nodeModDate )
		return true;

	// �ŏI�v���O�����X�V����
	const auto& nodeProgModDate = nodeRow.GetValue(road_node::kProgModifyDate);
	if(!isNullOrEmpty(nodeProgModDate) && m_Args.m_ModDate < nodeProgModDate )
		return true;

	// ���ɓ��H�m�[�h�ɐڑ����铹�H�����N���`�F�b�N
	const auto& linkT = *nodeRow.GetItemTable( road_link::kTableName );
	for( const auto& linkCon : linkT )
	{
		const auto& linkRow = *CAST_ROWC(linkCon);

		// �ŏI�X�V��
		const auto& linkModDate = linkRow.GetValue(road_link::kModifyDate);
		if( !isNullOrEmpty(linkModDate) && m_Args.m_ModDate < linkModDate )
			return true;

		// �ŏI�v���O�����X�V����
		const auto& linkProgModDate = linkRow.GetValue(road_link::kProgModifyDate);
		if( !isNullOrEmpty(linkProgModDate) && m_Args.m_ModDate < linkProgModDate )
			return true;
	}
	
	return false;
}

bool CCheckFollowingRoad::getMapFollowingRoad( const CTableContainer& nodeT, map<long, FollowingRoad>& mapFollowingRoad )
{
	// ������̃G���[������������false
	bool isSucceed = true;

	for( const auto& nodeCon : nodeT )
	{
		const auto& baseNodeRow = *CAST_ROWC(nodeCon);
		const size_t baseNodeOID = baseNodeRow.GetOID();
		const auto& baseLinkT = *baseNodeRow.GetItemTable( road_link::kTableName );
		// �ڑ����铹�H�����N��3�{�����ł���ΑΏۊO
		if( 3 > baseLinkT.size() )
			continue;

		// ���Ȃ胊���N����擾����
		FollowingRoad baseFollowingRoad;
		try
		{
			if( !getFollowingRoad( baseNodeOID, baseLinkT, baseFollowingRoad ) )
			{
				// ���Ȃ胊���N������ł��Ȃ�������G���[(105)
				m_ErrLogger.writeErrInfo( baseNodeRow, kBaseUnspecified );
				continue;
			}
		}
		catch(const exception& e)
		{
			m_RunLogger.writeErrMsg( e.what() );
			isSucceed = false;
			continue;
		}

		mapFollowingRoad.emplace( make_pair( baseNodeOID, baseFollowingRoad ) );
	}

	return isSucceed;
}

bool CCheckFollowingRoad::chkDiffFollowingRoad( const long mesh, CTableContainer& baseNodeT, CTableContainer& compNodeT )
{
	// ������̃G���[������������false
	bool isSucceed = true;

	// ���b�V����`�擾
	IEnvelopePtr env( AheMESH2ENV( mesh, AheGetSpatialReference( (ITable*)baseNodeT ) ) );
	if( !env )
	{
		m_RunLogger.writeErrMsg( _T("���b�V���|���S���̎擾�Ɏ��s") );
		return false;
	}

	// ��r�����A���b�V����`���̓��H�m�[�h�ƁA����ɐڑ����铹�H�����N���擾
	map<CString, IQueryFilterPtr> mapQuery;
	const IQueryFilterPtr ipSpFilter( AheInitSpatialFilter( nullptr, env ) );
	mapQuery.emplace( make_pair( road_node::kTableName, ipSpFilter ) );
	baseNodeT.Select( mapQuery );

	// ��r���̓��H�m�[�h�Ɠ��Ȃ胊���N��̑Ή��}�b�v���擾
	map<long, FollowingRoad> mapBaseFollowingRoad;
	isSucceed &= getMapFollowingRoad( baseNodeT, mapBaseFollowingRoad );
	
	// ��r���̓��H�m�[�h��OID���L�[�ɁA��r�悩�瓯��OID�̓��H�m�[�h���Z���N�g����
	list<long> baseNodeList;
	for( const auto& followingRoad : mapBaseFollowingRoad )
	{
		baseNodeList.emplace_back( followingRoad.first );
	}
	compNodeT.Select( baseNodeList );

	// ��r���Ɣ�r��œ��Ȃ胊���N��̕ω����`�F�b�N
	for( auto& compNodeCon : compNodeT )
	{
		const auto& compNodeRow = *CAST_ROWC(compNodeCon);
		const auto& compLinkT = *compNodeRow.GetItemTable( road_link::kTableName );
		const size_t compLinkNum = compLinkT.size();
		// �ڑ����铹�H�����N�̖{����3�{�����Ȃ�X�L�b�v
		if( 3 > compLinkNum )
			continue;

		// �ŏI�X�V�����w�肳��Ă���ꍇ�A���H�m�[�h�Ƃ���ɐڑ����铹�H�����N��
		// ��������w�肳�ꂽ���t�ȍ~�ɍX�V����Ă��Ȃ���΁A�`�F�b�N�ΏۂƂ��Ȃ�
		if( !isModifiedSinceDesignedDate( compNodeRow ) )
			continue;

		const long compNodeOID = compNodeRow.GetOID();

		const auto& it_Base = baseNodeT.find_by_key( road_node::kObjectID, compNodeOID );
		const auto& baseNodeRow = *CAST_ROWC(*it_Base);
		const auto& baseLinkT = *baseNodeRow.GetItemTable( road_link::kTableName );
		const size_t baseLinkNum = baseLinkT.size();

		// ���H�����N�̖{���ω����`�F�b�N
		if( compLinkNum > baseLinkNum )
		{
			// ���H�����N�̖{���������Ă�����G���[(103)
			m_ErrLogger.writeErrInfo( compNodeRow, kIncreaseLink );
			continue;
		}
		else if( compLinkNum < baseLinkNum )
		{
			// ���H�����N�̖{���������Ă�����G���[(104)
			m_ErrLogger.writeErrInfo( compNodeRow, kDecreaseLink );
			continue;
		}

		// �o�[�W�����Ԃœ��H�m�[�h�ɐڑ����铹�H�����N���ω����Ă�����G���[(102)
		if( hasChangedConnectedLink( baseLinkT, compLinkT ) )
		{
			m_ErrLogger.writeErrInfo( compNodeRow, kChangeLink );
			continue;
		}

		FollowingRoad compFollowingRoad;
		try
		{
			if( !getFollowingRoad( compNodeOID, compLinkT, compFollowingRoad ) )
			{
				// ���Ȃ胊���N������ł��Ȃ�������G���[(106)
				m_ErrLogger.writeErrInfo( compNodeRow, kCompUnspecified );
				continue;
			}
		}
		catch(const exception& e)
		{
			m_RunLogger.writeErrMsg( e.what() );
			isSucceed = false;
			continue;
		}

		const auto& baseFollowingRoad = mapBaseFollowingRoad[compNodeOID];
		for( auto& follow : compFollowingRoad )
		{
			if( baseFollowingRoad.end() != baseFollowingRoad.find(follow.first) )
			{
#ifdef _DEBUG
				// ���Ȃ胊���N�񂪕ω����Ă��Ȃ��Ă��A20���ȏ�ω����Ă�����(�ꉞ)���O�Ɏc��
				static double angleThreshold = 20.0; // �p�x�ω���臒l
				const auto& it = baseFollowingRoad.find(follow.first);
				double diff = abs( it->second - follow.second );
				if( 180 < diff ) diff = 360 - diff;
				if( angleThreshold < diff )
					m_ErrLogger.writeErrInfo( compNodeRow, kDiffAngle );
#endif //DEBUG
				continue;
			}

			// ���Ȃ胊���N�񂪕ω����Ă�����G���[(101)
			m_ErrLogger.writeErrInfo( compNodeRow, kChangeFollow );
			break;
		}
	}

	return isSucceed;
}
