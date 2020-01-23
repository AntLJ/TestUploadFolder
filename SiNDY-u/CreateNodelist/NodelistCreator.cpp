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


#include "Defs.h"
#include "NodelistCreator.h"
#include "MeshcodeUtility.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/directory_util.h>

// ArcHelperEx,WinLib
#include <AheLayerFunctions.h>
#include <GlobalFunctions.h>

// boost
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include "boost/range/algorithm/find_if.hpp"
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>

using namespace sindy;
using namespace schema;
using namespace uh;


static crd_cnv gCrdCnv;

// ������
bool CNodelistCreator::init( const CString& strDBConnect, const CString& strOutputPath, const CString& strLatLon, CString& strErrMsg )
{
	return init( strDBConnect, strDBConnect, strOutputPath, strLatLon, strErrMsg );
}


// ������(���H�m�[�h�̐ڑ���DB���C���X�g�����N�Ƃ͕ʂɎw��)
bool CNodelistCreator::init( const CString& strDBConnect, const CString& strRoadDBConnect, const CString& strOutputPath, const CString& strLatLon, CString& strErrMsg )
{
	// �e�[�u���t�@�C���_�ݒ�
	//   ILLUST_LINK
	CWorkspace ipWorkspace;
	if( ! connectSeveralTimes( strDBConnect, 3, ipWorkspace )) {
		strErrMsg = str_util::format( _T("�ڑ���DB�����݂��Ȃ����A�J���܂��� : %s"), strDBConnect );
		return false;
	}

	ITablePtr ipIllustTable = ipWorkspace.OpenSameOwnersTable( illust_link::kTableName );
	if( ! ipIllustTable ) {
		strErrMsg = str_util::format( _T("�t�B�[�`���[�N���X�����݂��Ȃ����ߏ����𒆒f���܂�: %s"), illust_link::kTableName );
		return false;
	}
	m_ipTableFinder.InitCollection( ipIllustTable , CModel() );


	// �e�[�u���t�@�C���_�ݒ�
	//   ROAD_NODE
	if( ! connectSeveralTimes( strRoadDBConnect, 3, ipWorkspace ) )
	{
		strErrMsg = str_util::format( _T("�ڑ���DB�����݂��Ȃ����A�J���܂��� : %s"), strRoadDBConnect );
		return false;
	}

	ITablePtr ipRoadNodeTable = ipWorkspace.OpenSameOwnersTable( road_node::kTableName );
	if( ! ipRoadNodeTable )
	{
		strErrMsg = str_util::format( _T("�t�B�[�`���[�N���X�����݂��Ȃ����ߏ����𒆒f���܂�: %s"), road_node::kTableName );
		return false;
	}
	m_ipTableFinder.InitCollection( ipRoadNodeTable , CModel() );


	// �m�[�h���X�g�o�͐�t�H���_�p�X
	m_strOutputPath = strOutputPath;

	// �ܓx�o�x�o�͐ݒ�
	m_LatLon = strLatLon;

	return true;
}


// �m�[�h���X�g�쐬
bool CNodelistCreator::create( const CIllustOutputInfo::OutputInfo info, const std::vector<CString>& rule, CString& strErrMsg )
{
	// ILLUST_LINK �֐ڑ�
	const auto& table = CAST_ILLUSTLINKTABLE( m_ipTableFinder.FindTable( illust_link::kTableName )); 
	if( ! table )
		return false;

	// �C���X�g��ʂƎd������Ō���
	CString strWhereClause = str_util::format( _T("%s=%d AND %s=%d"), 
												illust_link::kIllustClass,  info.mIllustClass,
												illust_link::kCustomerCode, info.mCustomerClass 
												);
	table->_Select( AheInitQueryFilter( nullptr, nullptr, strWhereClause ), true );
	table->CreateCache();

	// �o�̓t�@�C���X�g���[������
	std::ofstream ofs = getOutputFileStream( info.mOutputFileName.GetString() );
	if( ofs.fail() ){
		strErrMsg += _T("�o�̓t�@�C���쐬�Ɏ��s���܂���");
		return false;
	}


	// �m�[�h���X�g�o��
	std::map<long, CSPIllustLinkRow> records; // [bug11680]�o�̓��R�[�h��OID���Ƀ\�[�g

	// [bug11679]�g���l���T�t�B�b�N�X�t�^�t���O�������ɒǉ�
	CIllustNameCreator cCreator( info.mPrefix.GetString(), info.mSuffix.GetString(), rule, info.mTunnelSuffix );
	for( const auto& row : *table )
	{
		const auto& illust_row = CAST_ILLUSTLINKROW( row );

		// �m�[�h���X�g�o�͏��O�ΏۂȂ�Ώo�͂��Ȃ�
		if( CIllustOutputInfo::IsException( illust_row, info ))
			continue;

		records[ illust_row->GetOID() ] = illust_row;
	}

	for( const auto& rec : records )
	{
		// 1���R�[�h�o�͕��̃R���e�i
		std::vector<CString> OutRec;

		// �ݒ�t�@�C���̎w��t�B�[���h�̒l���o��
		if( ! getFieldValue( cCreator, rec.second, info.mOutputField, OutRec ) )
		{
			strErrMsg += uh::str_util::format( _T("�ݒ�t�@�C���ɑ��݂��Ȃ��t�B�[���h�����w�肳��Ă��܂�: ���: %d, �d������: %d ( Line %d )")
									, info.mIllustClass, info.mCustomerClass, info.mLineNum );
			ofs.close();
			return false;
		}
		
		// �ܓx�o�x�o��
		if( ! getLatLon( rec.second, OutRec ) ) {
			strErrMsg += uh::str_util::format( _T("�ܓx�o�x�̏o�͂Ɏ��s���܂���: ���: %d, �d������: %d ( Line %d )")
									, info.mIllustClass, info.mCustomerClass, info.mLineNum );
			ofs.close();
			return false;
		}

		// �����N����o��
		if( ! getLQ( rec.second, info.mOutputStartNode, info.mOutputEndNode, OutRec ) )
		{
			strErrMsg += uh::str_util::format( _T("�����N��̏o�͂Ɏ��s���܂���: ���: %d, �d������: %d ( Line %d )")
									, info.mIllustClass, info.mCustomerClass, info.mLineNum );
			ofs.close();
			return false;
		}

		// 1���R�[�h�o��
		ofs << CT2A( uh::str_util::join( OutRec, _T("\t") )) << std::endl;
	}

	table->DeleteCache();
	table->clear();

	ofs.close();
	return true;
}


// �w��񐔂���DB�ւ̐ڑ����g���C����
bool CNodelistCreator::connectSeveralTimes( const CString& strDBConnect, int nTryCount, sindy::CWorkspace& cWorkspace )
{
	for( int i = 0; i < nTryCount; ++i )
	{
		if(  cWorkspace.Connect( strDBConnect.GetString() ) )
			return true;
		// 3�b�ナ�g���C
		Sleep(3000);
	}
	return false;
}


// �ݒ�t�@�C���L�ڂ̎w��t�B�[���h�̒l���擾
bool CNodelistCreator::getFieldValue( sindy::CIllustNameCreator cNameCreator, sindy::CSPIllustLinkRow spRow , const std::vector<CString>& OutputFields, std::vector<CString>& OutString )
{
	for( const auto& field : OutputFields )
	{
		// �C���X�g���擾
		if( field == setting::ILLUSTNAME )
		{
			CString strIllustName = cNameCreator.GetIllustName( spRow );
			if( strIllustName.IsEmpty() )
				return false;

			OutString.push_back( strIllustName ); 
			continue;
		}

		// ���݂��Ȃ��t�B�[���h�����w�肳��Ă�����G���[
		if( spRow->FindField( field ) == -1 )
			return false;

		// �t�B�[���h�l���擾
		OutString.push_back( convFieldValue( spRow, field ));
	}

	return true;
}


// �t�B�[���h�̒l��ϊ����ĕԂ�
CString CNodelistCreator::convFieldValue(sindy::CSPIllustLinkRow spRow, const CString& strFieldName)
{
	using namespace illust_link;
	using namespace illust_class;
	using namespace branch_dir_code;
	using namespace uh;

	long lIllustClass  = spRow->GetValue( kIllustClass ).lVal;			// �C���X�g���
	long lStraightCode = spRow->GetValue( kStraightCode ).lVal;			// ���i�����R�[�h

	if(  strFieldName.Compare( kBranchDirectionCode ) == 0 )
	{
		long lCode = spRow->GetValue( strFieldName ).lVal;

		switch ( lIllustClass )
		{
		case kGrade:
			if( lStraightCode == straight_code::kStraight )
			{
				if( lCode == kLeft ) return _T("SL");
				if( lCode == kRight) return _T("SR");

			}
			else
			{
				if( lCode == kLeft ) return _T("L");
				if( lCode == kRight) return _T("R");
			}
			break;

		default:
			break;
		}
	}

	return str_util::ToString( spRow->GetValue( strFieldName ));
}


// �ݒ�t�@�C���L�ڂ̎w��t�B�[���h�̒l���t�@�C���ɏo�͂���
bool CNodelistCreator::getLatLon(sindy::CSPIllustLinkRow spRow, std::vector<CString>& OutString)
{
	if( m_LatLon != latlon::DEG && m_LatLon != latlon::DMS )
		return true;

	// ILLUST_LINK �֐ڑ�
	const auto& table = m_ipTableFinder.FindTable( road_node::kTableName ); 
	if( ! table )
		return false;	

	// �m�[�h2��ID�擾
	long lNodeId = spRow->GetValue( illust_link::kNode2 ).lVal;

	// �m�[�h�̍��W�擾
	WKSPoint latlon = getLatLonByNodeId( lNodeId );
	if( latlon.X == -1 && latlon.Y != -1 )
		return false;
	
	// ���W�l���o�͌`���ɕϊ�
	CString strX, strY;
	if( m_LatLon == latlon::DMS) 
	{
		// �x���b�ɕϊ�
		const dms sX = *gCrdCnv.DECtoDMS( latlon.X );
		const dms sY = *gCrdCnv.DECtoDMS( latlon.Y );

		strX = uh::str_util::format( _T("%d.%d.%2.2f"), sX.deg, sX.min, sX.sec );
		strY = uh::str_util::format( _T("%d.%d.%2.2f"), sY.deg, sY.min, sY.sec );
	}
	else if( m_LatLon == latlon::DEG )
	{
		// �����Œ�
		strX = uh::str_util::format( _T("%3.7f"), latlon.X );
		strY = uh::str_util::format( _T("%3.7f"), latlon.Y );
	}
	
	// ���W�l���擾
	OutString.push_back( strY );
	OutString.push_back( strX );

	return true;
}


// �����N����o�͂���
bool CNodelistCreator::getLQ(sindy::CSPIllustLinkRow spRow, int nStartNode, int nEndNode, std::vector<CString>& OutString)
{
	// �o�̓m�[�h�̖������擾
	int nEndNodeNum =  ( nEndNode == 0 ? getLastEnableNodeField( spRow ) : nEndNode );

	// �e�m�[�h�̃m�[�hID�ƃ��b�V���R�[�h�̑Ή����擾
	std::vector<NodeInfo> NodeInfos;
	for( int i = nStartNode; i <= nEndNodeNum; ++i )
	{
		NodeInfo info;
		// �m�[�hID
		long id = spRow->GetValue( illust_link::kNode[i] ).lVal;
		if( id == 0 )
			return false;
		info.id = id;
		// ���b�V���R�[�h
		std::vector<int> meshcodes = getMeshcodes( id );
		if( meshcodes.empty() )
			return false;
		for( const auto mesh : meshcodes )
			info.addMesh( mesh );

		NodeInfos.push_back(info);
	}

	// �擪�m�[�h��1�O�̃m�[�h�������郁�b�V���R�[�h���擾
	// (2�����b�V����̃m�[�h�̏o�͏��l���Ɏg�p)
	int nPreMesh = getPreRoadNodeMeshcode( NodeInfos, NodeInfos.begin() );
	if( nPreMesh == -1 )
		return false;


	// �m�[�h���X�g�ɏo�͂��郁�b�V��&�m�[�hID�����Ɋi�[
	for( auto iter = NodeInfos.begin(); iter != NodeInfos.end(); ++iter )
	{
		// 2�����b�V����ł͂Ȃ��ꍇ
		if( ! iter->isOn2ndMesh() )
		{
			OutString.push_back( str_util::ToString( *iter->meshes.begin() ));
			OutString.push_back( str_util::ToString( iter->id ));
			nPreMesh = *iter->meshes.begin();
			continue;
		}

		// 2�����b�V����̏ꍇ
		using boost::phoenix::arg_names::arg1;
		int nCurrentMesh = *std::find_if( iter->meshes.begin(), iter->meshes.end(), arg1 != nPreMesh );

		//  2�����b�V����̏ꍇ�A�ׂ��������̃��b�V��2���̃m�[�h�����o��(�o�͏��͍l������)
		//  �������A�擪/�����m�[�h�̏ꍇ��1�����o��
		if( iter != NodeInfos.begin() ){
			OutString.push_back( str_util::ToString( nPreMesh ));
			OutString.push_back( str_util::ToString( iter->id ));
		}
		if( iter+1 != NodeInfos.end() ){
			OutString.push_back( str_util::ToString( nCurrentMesh ));
			OutString.push_back( str_util::ToString( iter->id ));
		}

		nPreMesh = nCurrentMesh;
	}

	return true;
}


// ���b�V���R�[�h�Q���擾����
std::vector<int> CNodelistCreator::getMeshcodes( long lNodeId )
{
	WKSPoint lonlat = getLatLonByNodeId( lNodeId );
	if( lonlat.X == -1 || lonlat.Y == -1)
		return std::vector<int>(); 

	return CMeshcodeUtility::GetBelong2ndMeshcode( lonlat );
}


// �m�[�hID��������ܓx�o�x���擾����
WKSPoint CNodelistCreator::getLatLonByNodeId(long lNodeId)
{
	WKSPoint err_point = { -1, -1 };

	const auto& table = m_ipTableFinder.FindTable( road_node::kTableName );
	if( ! table )
		return err_point;

	// �m�[�h�擾
	table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );
	const auto& rows = table->finds_by_key( road_node::kObjectID, lNodeId );
	if( rows.empty() )
		return err_point;

	const auto& row = *CAST_ROWC( **rows.begin() );
	IPointPtr ipPoint = row.CRowBase::GetShapeCopy();
	if( ! ipPoint )
		return err_point;

	// ���W�擾
	double dX = 0.0, dY = 0.0;
	ipPoint->QueryCoords( &dX, &dY );

	WKSPoint lonlat = { dX, dY };
	return lonlat;
}


// ILLUST_LINK�̃����N��(�m�[�hID�Q)��1�O�̃m�[�h�������郁�b�V���R�[�h���擾����
int CNodelistCreator::getPreRoadNodeMeshcode( std::vector<CNodelistCreator::NodeInfo>& infos, std::vector<CNodelistCreator::NodeInfo>::iterator iter )
{
	// ���b�V�����E��̃m�[�h�łȂ���΁A�擪�m�[�h�̃��b�V���R�[�h�����̂܂܎擾
	if( ! iter->isOn2ndMesh() )
		return *iter->meshes.begin();


	// 2�����b�V����̃m�[�h�Ȃ�΁A���̃m�[�h�����ɍs��
	auto next_iter = iter + 1;
	if( next_iter == infos.end() )
		return -1;

	// ���̃m�[�h��2�����b�V����̃m�[�h�łȂ��Ȃ�΁A
	// ���̃m�[�h�̃��b�V���R�[�h�ł͂Ȃ����̃��b�V���R�[�h��Ԃ�
	if( ! next_iter->isOn2ndMesh() )
	{
		using boost::phoenix::arg_names::arg1;
		auto mesh_iter = std::find_if( iter->meshes.begin(), iter->meshes.end(), arg1 != *next_iter->meshes.begin() );
		if( mesh_iter == iter->meshes.end() )
			return -1;

		return *mesh_iter;
	}

	// ���ꃁ�b�V�����E��ł͂Ȃ�
	if( ! iter->isEqualMesh( *next_iter ))
	{
		int mesh = iter->getCommonMesh( *next_iter );

		using boost::phoenix::arg_names::arg1;
		auto mesh_iter = std::find_if( iter->meshes.begin(), iter->meshes.end(), arg1 != mesh );
		if( mesh_iter == iter->meshes.end() )
			return -1;
	}

	// ���ꃁ�b�V�����E��̃m�[�h�������ꍇ���l��
	int result_mesh = getPreRoadNodeMeshcode( infos, next_iter );
	if( result_mesh == -1 )
		return - 1;

	using boost::phoenix::arg_names::arg1;
	auto result_mesh_iter = std::find_if( iter->meshes.begin(), iter->meshes.end(), arg1 != result_mesh );
	if( result_mesh_iter == iter->meshes.end() )
		return -1;

	return *result_mesh_iter;
}



// ILLUST_LINK�̃����N��(�m�[�hID�Q)����ID���i�[����Ă��閖���̃C���f�b�N�X���擾����
int CNodelistCreator::getLastEnableNodeField( sindy::CSPIllustLinkRow spRow )
{
	for( int i = 1; i <= illust_link::kMaxNodeNumber; ++i )
	{
		if( spRow->GetValue( illust_link::kNode[i] ).vt == VT_NULL )
			return i - 1;
	}

	return -1;
}


// �m�[�h��2�����b�V����ɂ��邩�𔻒肷�� 
bool CNodelistCreator::isOn2ndMesh(long lNodeId)
{
	const auto& table = m_ipTableFinder.FindTable( road_node::kTableName );
	if( ! table )
		return false;

	 // �m�[�h�擾
	table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );
	const auto& rows = table->finds_by_key( road_node::kObjectID, lNodeId );
	if( rows.empty() )
		return false;

	const auto& row = *CAST_ROWC( **rows.begin() );
	long lNodeClass = row.GetValue( road_node::kNodeClass ).lVal;

	// �m�[�h��ʂŔ��f����
	if( lNodeClass == road_node::node_class::kEdge || lNodeClass == road_node::node_class::kPecEdge )
		return true;

	return false;
}


// �m�[�h���X�g�o�̓X�g���[�����擾����
std::ofstream CNodelistCreator::getOutputFileStream(const CString& strFilePath)
{
	// �o�͐�t�@�C���p�X�쐬
	CString strOutputFilePath = m_strOutputPath + _T("\\") + strFilePath;

	// �o�̓t�@�C���I�[�v��( ���ɑ��݂���Ȃ�ǋL���[�h )
	std::ofstream ofs;
	if( uh::isFile( strOutputFilePath.GetString() )) {
		ofs.open( strOutputFilePath.GetString(), std::ios::out | std::ios::app );
	}else {
		ofs.open( strOutputFilePath.GetString(), std::ios::out );
	}

	return ofs;
}
