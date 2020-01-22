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
#include "CheckBusPoint.h"
#include "BusPointCommon/RegexChk.h"
#include "BusPointCommon/CommonFunc.h"
#include <ArcHelperEx/GlobalFunctions.h>
#include <sindy/workspace.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace sindy::schema::bus;

// �o�X��|�C���g�ƃ��[�g�T���p�|�C���g�̋���臒l�i�ȏゾ��ERROR�j
const long k_stop_to_route_dist = 20; //!< 20m

// �o�X��W���|�C���g���m�̋���臒l�i�ȉ�����WARNING�j
const long k_pole_to_pole_dist = 5;   //!< 5m

// ���[�g�T���p�|�C���g�Ɠ��H�����N�̋���臒l�i�ȏゾ��ERROR�j
const double k_route_to_nw_dist = 0.5;  //!< 50cm

// �e�폈�������s���邽�߂ɍŏ��ɌĂ΂��֐�
bool CCheckBusPoint::execute()
{
	// �e�평���������{
	if( !Init() ){
		return false;
	}

	// �`�F�b�N��E�Q�Ɛ�DB���o��
	m_ofsChk << "#DB_BUS�F" << CT2CA(m_strBusDB) << endl;
	m_ofsChk << "#DB_ROAD�F" << CT2CA(m_strRoadDB) << endl;

	// ���R�`���w�b�_�o��
	m_ofsChk << "#FREESTYLELOG" << endl;
	m_ofsChk << "LAYER\tOBJECTID\tERR_LEVEL\tERR_MESSAGE\tCOMPANY\tROUTE_NAME\tROUTE_DETAIL\tSTOP_SEQ\tFLAG\tCOMMENT" << endl;

	// �o�X��|�C���g�`�F�b�N
	CheckStopPoint();

	// �o�X���[�g�T���p�|�C���g�`�F�b�N
	CheckRoutePoint();

	// �o�X�H���}�X�^�`�F�b�N
	CheckLineMaster();
	
	// �o�X��ԏ��Ǘ��`�F�b�N
	CheckStopSeq();

	return true;
}

// �e�평����
bool CCheckBusPoint::Init()
{
	// DB�`�F�b�N���O�t�@�C���I�[�v��
	m_ofsChk.open( m_strChkLog );
	if( !m_ofsChk ){
		cerr << "#ERROR DB�`�F�b�N���O�t�@�C���I�[�v���Ɏ��s�F" << CT2CA(m_strChkLog) << endl;
		return false;
	}

	// �o�X�֘A���[�N�X�y�[�X�I�[�v��
	m_ipBusWorkspace = sindy::create_workspace( m_strBusDB );

	if( ! m_ipBusWorkspace ){
		cerr << "#ERROR �o�X�T�[�o�ւ̐ڑ��Ɏ��s : " << CT2CA(m_strBusDB) << endl;
		return false;
	}

	// �o�X��|�C���g�t�B�[�`���N���X�I�[�v��
	((IFeatureWorkspacePtr)m_ipBusWorkspace)->OpenFeatureClass( _bstr_t(bus_stop_point::kTableName), &m_ipBusStopPointFC );
	if( ! m_ipBusStopPointFC ){
		cerr << "#ERROR �t�B�[�`���N���X�̃I�[�v���Ɏ��s�F" << CT2CA(bus_stop_point::kTableName) << endl;
		return false;
	}
	// �o�X��|�C���g�t�B�[���h�}�b�v�쐬
	m_busStopPointFM.CreateFieldMap( (ITablePtr)m_ipBusStopPointFC );

	// �o�X�H���}�X�^�e�[�u���I�[�v��
	((IFeatureWorkspacePtr)m_ipBusWorkspace)->OpenTable( _bstr_t(bus_line_master::kTableName), &m_ipBusLineMasterTable );
	if( ! m_ipBusLineMasterTable ){
		cerr << "#ERROR �t�B�[�`���N���X�̃I�[�v���Ɏ��s�F" << CT2CA(bus_line_master::kTableName) << endl;
		return false;
	}
	// �o�X�H���}�X�^�e�[�u���t�B�[���h�}�b�v�쐬
	m_busLineMasterFM.CreateFieldMap( m_ipBusLineMasterTable );

	// �o�X��ԏ��Ǘ��e�[�u���I�[�v��
	((IFeatureWorkspacePtr)m_ipBusWorkspace)->OpenTable( _bstr_t(bus_stop_seq::kTableName), &m_ipBusStopSeqTable );
	if( ! m_ipBusStopSeqTable ){
		cerr << "#ERROR �t�B�[�`���N���X�̃I�[�v���Ɏ��s�F" << CT2CA(bus_stop_seq::kTableName) << endl;
		return false;
	}
	// �o�X��ԏ��Ǘ��e�[�u���t�B�[���h�}�b�v�쐬
	m_busStopSeqFM.CreateFieldMap( m_ipBusStopSeqTable );

	// �o�X���[�g�T���p�|�C���g�t�B�[�`���N���X�I�[�v��
	((IFeatureWorkspacePtr)m_ipBusWorkspace)->OpenFeatureClass( _bstr_t(bus_route_point::kTableName), &m_ipBusRoutePointFC );
	if( ! m_ipBusRoutePointFC ){
		cerr << "#ERROR �t�B�[�`���N���X�̃I�[�v���Ɏ��s�F" << CT2CA(bus_route_point::kTableName) << endl;
		return false;
	}
	// �o�X���[�g�T���p�|�C���g�t�B�[���h�}�b�v�쐬
	m_busRoutePointFM.CreateFieldMap( (ITablePtr)m_ipBusRoutePointFC );


	// ���HNW���[�N�X�y�[�X�I�[�v��
	m_ipRoadWorkspace = sindy::create_workspace( m_strRoadDB );

	if( ! m_ipRoadWorkspace ){
		cerr << "#ERROR �o�X�T�[�o�ւ̐ڑ��Ɏ��s : " << CT2CA(m_strRoadDB) << endl;
		return false;
	}

	// ���HNW(ROAD_LINK)�t�B�[�`���N���X�I�[�v��
	((IFeatureWorkspacePtr)m_ipRoadWorkspace)->OpenFeatureClass( _bstr_t(road_link::kTableName), &m_ipRoadLinkFC );
	if( ! m_ipRoadLinkFC ){
		cerr << "#ERROR �t�B�[�`���N���X�̃I�[�v���Ɏ��s�F" << CT2CA(road_link::kTableName) << endl;
		return false;
	}
	return true;
}

// �o�X��|�C���g�`�F�b�N
void CCheckBusPoint::CheckStopPoint()
{
	// �o�X��|�C���g����S���擾
	IFeatureCursorPtr ipCursor;
	m_ipBusStopPointFC->Search( NULL, VARIANT_FALSE, &ipCursor );

	// �o�X��|�C���g���m�̑�����r�p
	BUSSTOPP busStopPoints;

	IFeaturePtr ipStopFeature;
	// �I�u�W�F�N�g���Ƃ̏���
	while( ipCursor && S_OK == ipCursor->NextFeature(&ipStopFeature) ){

		// �t�B�[�`����NULL�`�F�b�N
		if( !ipStopFeature ){
			m_ofsChk << "#ERROR �o�X��|�C���g�`�F�b�N���Ƀt�B�[�`���擾���s" << endl;
			continue;
		}

		// �������擾���Ă���
		CComVariant vaValue;
		MAPATTR     mapAttr;
		for( auto& field : m_busStopPointFM.GetFieldMap() )
		{
			ipStopFeature->get_Value( field.second, &vaValue );
			mapAttr[field.first] = vaValue;
		}

		// ObjectID
		long oID = 0;
		ipStopFeature->get_OID( &oID );

		// �|�C���g��ʂ��Ƃ̃`�F�b�N
		bus_stop_point::point_attr_code::ECode pAttrCode = (bus_stop_point::point_attr_code::ECode)(mapAttr[bus_stop_point::kPointAttrCode].lVal);

		switch( pAttrCode )
		{

		// �W���|�C���g�̂�
		case bus_stop_point::point_attr_code::kPole: 
			{
			
			// �o�X��|�C���g���́i�W���|�C���g�j
			if( consider_as_null( mapAttr[bus_stop_point::kPoleNo] )){
				// �y�◯���W���|�C���g�̕W���ԍ����󔒂ł���z
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t�◯���W���|�C���g�̕W���ԍ����󔒂ł���\t\t\t\t\t0\t" << endl;
			}

			// �Ή����郋�[�g�T���p�|�C���g�֘A
			long routePointCnt = 0;
			IFeaturePtr ipRouteFeature = GetRoutePoint( oID, routePointCnt );
			if( 0 == routePointCnt ){
				// �y�◯���W���|�C���g�ɑΉ�����◯�����[�g�T���p�|�C���g���Ȃ��z
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t�◯���W���|�C���g�ɑΉ�����◯�����[�g�T���p�|�C���g���Ȃ�\t\t\t\t\t0\t" << endl;
			}
			else if( 1 < routePointCnt ){
				// �y�◯���W���|�C���g�ɑΉ�����◯�����[�g�T���p�|�C���g2�ȏ゠��z
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t�◯���W���|�C���g�ɑΉ�����◯�����[�g�T���p�|�C���g2�ȏ゠��\t\t\t\t\t0\t" << endl;
			}
			else{
				// �◯���|�C���g�ƃ��[�g�T���p�|�C���g�̋����𑪂�
				double dist = GetDistFeature( ipStopFeature, ipRouteFeature );
				if( k_stop_to_route_dist < dist ){
					// �y�◯���W���|�C���g�ɑΉ����郋�[�g�T���p�|�C���g�����ꂷ���Ă���(臒l�F20m)(WARNING)�z
					m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tWARNING\t�◯���W���|�C���g�ɑΉ����郋�[�g�T���p�|�C���g�����ꂷ���Ă���(臒l�F" << k_stop_to_route_dist << "m)\t\t\t\t\t0\t" << endl;
				}
			}

			long stopPointCount = 0;
			IFeatureCursorPtr ipNearCursor = GetNearStopPoints( ipStopFeature, stopPointCount );
			if( 0 < stopPointCount ){
				// �o�X��|�C���g��5m�ȓ��ɑ��݂���ꍇ�́A�W���|�C���g�̐��𐔂���
				if( 0 < CountPolePoint( ipNearCursor ) ){
					// �y�ʂ̒◯���W���|�C���g��5m�ȓ��ɂ���(WARNING)�z
					m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tWARNING\t�ʂ̒◯���W���|�C���g��" << k_pole_to_pole_dist << "m�ȓ��ɂ���\t\t\t\t\t0\t" << endl;
				}
			}
		
			// �Ή������ԏ����擾
			long seqCnt = 0;
			set<CString> companys;
			_IRowPtr ipSeqRow;
			_ICursorPtr ipSeqCursor = GetStopSeqs( oID, bus_stop_seq::kBusStopPointID, seqCnt );
			while( ipSeqCursor && S_OK == ipSeqCursor->NextRow( &ipSeqRow )){

				// �t�B�[�`����NULL�`�F�b�N
				if( !ipSeqRow ){
					continue;
				}

				CComVariant vaSeqValue;
				ipSeqRow->get_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kBusLineMasterID ), &vaSeqValue );

				if( !consider_as_null( vaSeqValue )){
					// �Ή�����H�����Ǘ����擾	
					_IRowPtr ipLineRow = GetLineMaster( vaSeqValue.lVal );
					if( ipLineRow ){

						// �H�����Ǘ��e�[�u���̎��ƎҖ��擾
						CComVariant vaLineValue;
						ipLineRow->get_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kCompany ), &vaLineValue );
						if( !consider_as_null( vaLineValue ) && VT_BSTR == vaLineValue.vt){
							companys.insert( vaLineValue.bstrVal );
						}
					}
				}
			}

			// ���ƎҖ�����ȏ�擾���ꂽ�ꍇ
			if( 1 < companys.size() ){
				// �y����̒◯�����قȂ鎖�Ǝ҂̘H���Ŏg�p����Ă���z
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t����̒◯�����قȂ鎖�Ǝ҂̘H���Ŏg�p����Ă���\t\t\t\t\t0\t" << endl;
			}
			}
			break;

		// ���y���l���|�C���g�̂�
		case bus_stop_point::point_attr_code::kNationalLand: 
			{

			// �Ή������ԏ����擾
			long seqCnt = 0;
			_ICursorPtr ipSeqCursor = GetStopSeqs( oID, bus_stop_seq::kBusStopPointID, seqCnt );
			if( 0 < seqCnt ){
				// ���y���l���|�C���g�ɑΉ����Ă����ԏ�������
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t���y���l���|�C���g�ɑΉ����Ă����ԏ�������\t\t\t\t\t0\t" << endl;
			}
			}
			break;


		// �o�H���ԃ|�C���g�̂�
		case bus_stop_point::point_attr_code::kRoute: 
			{

			// �y�o�H���ԃ|�C���g�Ƀ|�C���g��ʈȊO�̑��������͂���Ă���z
			for( const auto& val : mapAttr )
			{
				if( val.first == bus_stop_point::kPointAttrCode ){
					continue;
				}
				if( !consider_as_null( val.second ) ){
					m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t�o�H���ԃ|�C���g�Ƀ|�C���g��ʈȊO�̑��������͂���Ă���\t\t\t\t\t0\t" << endl;
					break;
				}
			}
			}
			break;
		}

		// �W���|�C���g�ƍ��y���l���|�C���g
		if( bus_stop_point::point_attr_code::kPole == pAttrCode || bus_stop_point::point_attr_code::kNationalLand == pAttrCode ){
	
			// �|�C���g����
			if( consider_as_null( mapAttr[bus_stop_point::kName] )){
				// �y�◯���W���|�C���g�A���y���l���|�C���g�̒◯�������󔒂ł���z
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t�◯���W���|�C���g�A���y���l���|�C���g�̒◯�������󔒂ł���\t\t\t\t\t0\t" << endl;
			}

			if( consider_as_null( mapAttr[bus_stop_point::kYomi] )){
				// �y�◯���W���|�C���g�A���y���l���|�C���g�̒◯�������󔒂ł���z
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t�◯���W���|�C���g�A���y���l���|�C���g�̒◯������݂��Ȃ��󔒂ł���\t\t\t\t\t0\t" << endl;
			}
		}

		// �W���|�C���g�ƌo�H���ԃ|�C���g
		if( bus_stop_point::point_attr_code::kPole == pAttrCode || bus_stop_point::point_attr_code::kRoute == pAttrCode ){

			// �Ή������ԏ����擾
			long seqCnt = 0;
			_ICursorPtr ipSeqCursor = GetStopSeqs( oID, bus_stop_seq::kBusStopPointID, seqCnt );
			if( 0 == seqCnt ){
				// �y�◯���W���|�C���g�A�o�H���ԃ|�C���g�ɑΉ������ԏ����Ȃ�(WARNING)�z
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tWARNING\t�◯���W���|�C���g�A�o�H���ԃ|�C���g�ɑΉ������ԏ����Ȃ�\t\t\t\t\t0\t" << endl;
			}
		}


		// �ȉ��͑S�Ẵ|�C���g�i�W���|�C���g�E���y���l���|�C���g�E�o�H���ԃ|�C���g�j

		// �y(�ύX��◯����/�ύX��◯�������/���̕ύX���l�j�̂����A1�܂���2���󗓂ɂȂ��Ă���z
		if( consider_as_null( mapAttr[ bus_stop_point::kChangedName ] ) == 
			( consider_as_null( mapAttr[ bus_stop_point::kChangedYomi ] ) &&
			consider_as_null( mapAttr[ bus_stop_point::kRemarks ] ) ) ){
			// OK
		}
		else{
			// ERROR
			m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t(�ύX��◯����/�ύX��◯�������/���̕ύX���l�j�̂����A1�܂���2���󗓂ɂȂ��Ă���\t\t\t\t\t0\t" << endl;
		}

		// �◯���R�[�h�ƕW���ԍ�����������̃o�X��|�C���g�m�F�̏����ŁA����L�[�ɂ��āAvector��OID���߂�
		busStopPoints[make_pair( mapAttr[bus_stop_point::kStopID].lVal, mapAttr[bus_stop_point::kPoleNo].lVal )].push_back( oID );

	} // �I�u�W�F�N�g���Ƃ̏��������܂�


	// �o�X��|�C���g���m�̊m�F�i�����j
	for( auto& busStopPoint : busStopPoints ){
		if( busStopPoint.second.size() > 1 ){
			// �y�◯���R�[�h�ƕW���ԍ�����������̃o�X��|�C���g������z
			for( auto& errOID : busStopPoint.second ){
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << errOID << "\tERROR\t�◯���R�[�h�ƕW���ԍ�����������̃o�X��|�C���g������\t\t\t\t\t0\t" << endl;
			}
		}
	}
}

// �o�X���[�g�T���p�|�C���g�`�F�b�N
void CCheckBusPoint::CheckRoutePoint()
{
	// �o�X���[�g�T���p�|�C���g����S���擾
	IFeatureCursorPtr ipCursor;
	m_ipBusRoutePointFC->Search( NULL, VARIANT_FALSE, &ipCursor );

	IFeaturePtr ipRouteFeature;
	// �I�u�W�F�N�g���Ƃ̏���
	while( ipCursor && S_OK == ipCursor->NextFeature(&ipRouteFeature) ){

		// �t�B�[�`����NULL�`�F�b�N
		if( !ipRouteFeature ){
			m_ofsChk << "#ERROR �o�X�⃋�[�g�T���p�|�C���g�`�F�b�N���Ƀt�B�[�`���擾���s" << endl;
			continue;
		}

		// ObjectID
		long oID = 0;
		ipRouteFeature->get_OID( &oID );

		// �o�X��|�C���gID
		CComVariant vaValue;
		ipRouteFeature->get_Value( m_busRoutePointFM.GetFieldIndex( bus_route_point::kBusStopPointID ), &vaValue );
		long stopID = vaValue.lVal;

		// �y���[�g�T���p�|�C���g�ɑΉ�����◯���|�C���g���Ȃ��z
		long count = 0;
		GetStopPoint( stopID, count );
		if( 0 == count ){
			m_ofsChk << CT2CA(bus_route_point::kTableName) << "\t" << oID << "\tERROR\t���[�g�T���p�|�C���g�ɑΉ�����◯���|�C���g���Ȃ�\t\t\t\t\t0\t" << endl;
		}
		
		// �y���[�g�T���p�|�C���g�����H�����N�̋ߖT�ɂȂ�(臒l�F50cm)�z
		if( !GetNearRoadLink( ipRouteFeature )){
			m_ofsChk << CT2CA(bus_route_point::kTableName) << "\t" << oID << "\tERROR\t���[�g�T���p�|�C���g�����H�����N�̋ߖT�ɂȂ�(臒l�F" << k_route_to_nw_dist << "m)\t\t\t\t\t0\t" << endl;
		}
	}
}

// �o�X�H���}�X�^�`�F�b�N
void CCheckBusPoint::CheckLineMaster()
{
	// �o�X�H���}�X�^����S���擾
	_ICursorPtr ipCursor;
	m_ipBusLineMasterTable->Search( NULL, VARIANT_FALSE, &ipCursor );

	SAMELINEMASTER sameLineMaster; //!< ���ƎҖ��A�n���ԍ��A�o�H�����ꂪ�Ȃ����m�F�pmap(���L3���L�[�ɁAOID�E�H�����E�o�H)
	SAMELINENUM    sameLineNum;	//!< ����n���ԍ��ŁA�n�������قȂ���̂̊m�F�pmap(�n���ԍ����L�[�ɁA
	
	_IRowPtr ipLineRow;
	// �I�u�W�F�N�g���Ƃ̏���
	while( ipCursor && S_OK == ipCursor->NextRow(&ipLineRow) ){

		// �t�B�[�`����NULL�`�F�b�N
		if( !ipLineRow ){
			m_ofsChk << "#ERROR �o�X�H���}�X�^�`�F�b�N���Ƀt�B�[�`���擾���s" << endl;
			continue;
		}

		// �������擾���Ă���
		CComVariant vaValue;
		MAPATTR     mapAttr;
		for( auto& field : m_busLineMasterFM.GetFieldMap() )
		{
			ipLineRow->get_Value( field.second, &vaValue );
			mapAttr[field.first] = vaValue;
		}

		// ObjectID
		long oID = 0;
		ipLineRow->get_OID( &oID );

		// �Ή������ԏ����擾
		long seqCnt = 0;
		_ICursorPtr ipSeqCursor = GetStopSeqs( oID, bus_stop_seq::kBusLineMasterID, seqCnt );
		if( 0 == seqCnt ){
			// �y�o�X�H���ɑΉ������ԏ����Ȃ��z
			m_ofsChk << CT2CA(bus_line_master::kTableName) << "\t" << oID << "\tERROR\t�o�X�H���ɑΉ������ԏ����Ȃ�\t" 
				<< CT2CA(mapAttr[bus_line_master::kCompany].bstrVal) << "\t" << CT2CA(mapAttr[bus_line_master::kRouteName].bstrVal)
				 << "\t" << CT2CA(mapAttr[bus_line_master::kRouteDetail].bstrVal) << "\t\t0\t" << endl;
		}

		// ���ƎҖ��A�n���ԍ��A�o�H������̘H�����m�F�̏����ŁA�O���L�[�ɂ��āAvector�Ɍn������OID���߂�
		sameLineMaster[make_pair( mapAttr[bus_line_master::kRouteNumber].lVal, 
			make_pair( mapAttr[bus_line_master::kCompany].bstrVal, mapAttr[bus_line_master::kRouteDetail].bstrVal ))].push_back(
			make_pair( mapAttr[bus_line_master::kRouteName].bstrVal, oID ));

		// �n���ԍ�������ŁA�n�������قȂ�H�����m�F�̏����ŁA�n���ԍ����L�[�ɂ��āAvector�Ɏ��ƎҖ��EOID�E�n�����E�o�H���߂�
		sameLineNum[mapAttr[bus_line_master::kRouteNumber].lVal].push_back( make_pair( make_pair( mapAttr[bus_line_master::kCompany].bstrVal, oID ), 
			make_pair( mapAttr[bus_line_master::kRouteName].bstrVal, mapAttr[bus_line_master::kRouteDetail].bstrVal )));
	}

	// �o�X�H���}�X�^���m�̊m�F
	CheckLineMasterAttrs( sameLineMaster, sameLineNum );
}

// �o�X�H���}�X�^���m�̊m�F
void CCheckBusPoint::CheckLineMasterAttrs( const SAMELINEMASTER& sameLineMaster, const SAMELINENUM& sameLineNum )
{
	// �o�X�H���}�X�^���m�̊m�F(���ƎҖ��A�n���ԍ��A�o�H������H)
	for( auto& lineMaster : sameLineMaster ){
		if( 1 < lineMaster.second.size() ){
			// �y���ƎҖ��A�n���ԍ��A�o�H������̘H����񂪂���z
			for( auto& errOID : lineMaster.second ){
				m_ofsChk << CT2CA(bus_line_master::kTableName) << "\t" << errOID.second << "\tERROR\t���ƎҖ��A�n���ԍ��A�o�H������̘H����񂪂���\t" 
					<< CT2CA(lineMaster.first.second.first) << "\t" << CT2CA(errOID.first) << "\t" << CT2CA(lineMaster.first.second.second) 
					<< "\t\t0\t" << endl;
			}
		}
	}

	// �o�X�H���}�X�^���m�̊m�F(�n���ԍ�������A�n�������قȂ�H)
	for( auto& lineNum : sameLineNum ){
		set<CString> routeNameSet;
		for( auto& routeName : lineNum.second ){
			// �n�������i�[
			routeNameSet.insert( routeName.second.first);
		}
		// ����n���ԍ��Ōn������2�ȏ�
		if( 1 < routeNameSet.size() ){
			for( auto& errOIDs : lineNum.second ){
				// �y�n���ԍ�������ŁA�n�������قȂ�H����񂪂���z
				m_ofsChk << CT2CA(bus_line_master::kTableName) << "\t" << errOIDs.first.second << "\tERROR\t�n���ԍ�������ŁA�n�������قȂ�H����񂪂���\t"
					<< CT2CA(errOIDs.first.first) << "\t" << CT2CA(errOIDs.second.first) << "\t" << CT2CA(errOIDs.second.second) << "\t\t0\t" << endl;
			}
		}
	}
}

// �o�X��ԏ��Ǘ��`�F�b�N
void CCheckBusPoint::CheckStopSeq()
{
	// �o�X��ԏ�����S���擾
	_ICursorPtr ipCursor;
	m_ipBusStopSeqTable->Search( NULL, VARIANT_FALSE, &ipCursor );

	// ����H���̒�ԏ��m�F�p
	SAMESEQ sameSeq;

	_IRowPtr ipSeqRow;

	// �I�u�W�F�N�g���Ƃ̏���
	while( ipCursor && S_OK == ipCursor->NextRow(&ipSeqRow) ){

		// �t�B�[�`����NULL�`�F�b�N
		if( !ipSeqRow ){
			m_ofsChk << "#ERROR �o�X��ԏ��Ǘ��`�F�b�N���Ƀt�B�[�`���擾���s" << endl;
			continue;
		}

		// �������擾���Ă���
		CComVariant vaValue;
		MAPATTR     mapAttr;
		for( auto& field : m_busStopSeqFM.GetFieldMap() )
		{
			ipSeqRow->get_Value( field.second, &vaValue );
			mapAttr[field.first] = vaValue;
		}

		// ObjectID
		long oID = 0;
		ipSeqRow->get_OID( &oID );

		// ���O�o�͗p�ɘH���}�X�^�̌n�����ƌo�H�Ǝ��ƎҖ��A���g�̃V�[�P���X���擾���Ă���
		_IRowPtr ipLineRow = GetLineMaster( mapAttr[bus_stop_seq::kBusLineMasterID].lVal );
		CComVariant vaLineValue;
		ipLineRow->get_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteName), &vaLineValue );
		CString routeName   = vaLineValue.bstrVal;
		ipLineRow->get_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteDetail), &vaLineValue );
		CString routeDetail = vaLineValue.bstrVal; 
		ipLineRow->get_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kCompany), &vaLineValue );
		CString company = vaLineValue.bstrVal;
		long stopSeq = mapAttr[bus_stop_seq::kStopSeq].lVal;

		// ����H���̒�ԏ��m�F�̏����ŁA�H��ID���L�[�ɁA�����ԍ��E�o�X��|�C���gID�EObjectID�E���ƎҖ��E�n�����E�o�H��Vector�Ɋi�[
		sameSeq[mapAttr[bus_stop_seq::kBusLineMasterID].lVal].push_back( make_pair( 
			make_pair( mapAttr[bus_stop_seq::kStopSeq].lVal, make_pair( mapAttr[bus_stop_seq::kBusStopPointID].lVal, oID )), 
			make_pair( company, make_pair( routeName, routeDetail ))));

		// �y�Ή�����o�X��|�C���g���Ȃ��z
		long count = 0;
		GetStopPoint( mapAttr[bus_stop_seq::kBusStopPointID].lVal, count );
		if( 0 == count ){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t�Ή�����o�X��|�C���g���Ȃ�\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}

		// �y�w�肳�ꂽ�����ƋL���ȊO�̒l�����͂���Ă���(�o�H�j��)�z
		boost::wregex	regOnlyNumScolon(L"^([1-6;]|99)+$");	// (�o�H�j��(DAY_CODES)�F1�`6�����99�̔��p�����ƁA���p�Z�~�R����(;)�݂̂̐��K�\��
		if( !consider_as_null( mapAttr[bus_stop_seq::kDayCodes] ) && !IsOKRegEx( mapAttr[bus_stop_seq::kDayCodes].bstrVal, regOnlyNumScolon )){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t�w�肳�ꂽ�����ƋL���ȊO�̒l�����͂���Ă���(�o�H�j��)\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}
		
		// �y�w�肳�ꂽ�����ƋL���ȊO�̒l�����͂���Ă���(�o�H���ԑ�)�z
		boost::wregex	regOnlyNumColonScolon(L"^[0-9;:\-]+$");	// ���p����+���p�Z�~�R����+���p�R����+���p�n�C�t���݂̂̐��K�\��
		if( !consider_as_null( mapAttr[bus_stop_seq::kTimeCodes] ) && !IsOKRegEx( mapAttr[bus_stop_seq::kTimeCodes].bstrVal, regOnlyNumColonScolon )){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t�w�肳�ꂽ�����ƋL���ȊO�̒l�����͂���Ă���(�o�H���ԑ�)\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}

		// �y�o�H���ԑтƂ��Ă��蓾�Ȃ����������͂���Ă���(4:00����27:59�̊ԈȊO)�z
		// [Bug 10475]�`�F�b�N���e(�o�H���ԑсj�̕ύX �ɂ��A[0:00�`23:59]����C��
		boost::wregex	regOKTime(L"^(( |;)*(([4-9]|[1][0-9]|[2][0-7]):[0-5][0-9]-([4-9]|[1][0-9]|[2][0-7]):[0-5][0-9])*( |;)*)*$");	// [4:00�`27:59]-[4:00�`27:59]�� ;�i������؂�j
		if( !consider_as_null( mapAttr[bus_stop_seq::kTimeCodes] ) && !IsOKRegEx( mapAttr[bus_stop_seq::kTimeCodes].bstrVal, regOKTime )){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t�o�H���ԑтƂ��Ă��蓾�Ȃ����������͂���Ă���(4:00����27:59�̊ԈȊO)\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}

		// �y�o�H�j���Ǝ��ԑт̏��������قȂ�z
		boost::wregex	regOnlyScolon(L";");
		if( GetRegExCnt( mapAttr[bus_stop_seq::kDayCodes].bstrVal, regOnlyScolon ) != GetRegExCnt( mapAttr[bus_stop_seq::kTimeCodes].bstrVal, regOnlyScolon )){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t�o�H�j���Ǝ��ԑт̏��������قȂ�\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}

		// �y�o�H�j����99(���̑�)�Ȃ̂ɁA�j�����Ԕ��l�ɋL�ڂ��Ȃ��z
		CString dayCodes = mapAttr[bus_stop_seq::kDayCodes].bstrVal;
		if( _T("99") == dayCodes && consider_as_null( mapAttr[bus_stop_seq::kRemarks] )){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t�o�H�j����99(���̑�)�Ȃ̂ɁA�j�����Ԕ��l�ɋL�ڂ��Ȃ�\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}
	}

	// ����H���̒�ԏ��m�F
	CheckSameSeqs( sameSeq );

}

// ����H���̒�ԏ��m�F
void CCheckBusPoint::CheckSameSeqs( const SAMESEQ& sameSeq )
{
	// lineMaster��first:�H��ID, second( first:(first:stopSeq, second:(first:BusStopPointID, second:oID)), second:(first:company, second:(first:routeName, second:routeDetail)))
	for( auto& lineMaster : sameSeq ){

		SAMELINESEQ sameLineMap;

		for( auto& sameLine : lineMaster.second ){

			// ��ԏ����L�[�ɁA�o�X��ID��ObjectID�Ǝ��Ǝ҂ƌn�����ƌo�H��vector�Ɋi�[
			sameLineMap[sameLine.first.first].push_back( make_pair( make_pair( sameLine.first.second.first, sameLine.first.second.second ), 
				make_pair( sameLine.second.first, make_pair( sameLine.second.second.first, sameLine.second.second.second ))));
		}

		// ����H���̃V�[�P���X�m�F
		for( int i = 1 ; i < sameLineMap.rbegin()->first ; ++i ){

			// 1�`�ő�l�܂ŁA�V�[�P���X�����݂��邩�m�F����
			if( sameLineMap.end() == sameLineMap.find( i ) ){

				// 1���Ȃ�
				if( i == 1 ){
					// �y����H���̒�ԏ���1����n�܂�Ȃ��z
					m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << (sameLineMap.begin()->second).begin()->first.second
						<< "\tERROR\t����H���̒�ԏ���1����n�܂�Ȃ�\t" << CT2CA((sameLineMap.begin()->second).begin()->second.first)
						<< "\t" << CT2CA((sameLineMap.begin()->second).begin()->second.second.first) 
						<< "\t" << CT2CA((sameLineMap.begin()->second).begin()->second.second.second) << "\t" << i << "\t0\t" << endl;
				}
				// ����ȊO�̌���
				else{
					// �y����H���̒�ԏ��Ɍ��Ԃ�����z
					m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << (sameLineMap.begin()->second).begin()->first.second 
						<< "\tERROR\t����H���̒�ԏ��Ɍ��Ԃ�����\t" << CT2CA((sameLineMap.begin()->second).begin()->second.first)
						<< "\t" << CT2CA((sameLineMap.begin()->second).begin()->second.second.first) 
						<< "\t" << CT2CA((sameLineMap.begin()->second).begin()->second.second.second) << "\t" << i << "\t0\t" << endl;
				}
			}
		}

		// seqChk��first:��ԏ�, second( first( first:�o�X��ID, second:ObjectID ), second( first:���ƎҖ�, second:(first:�n����, second:�o�H )))) 
		// �d���V�[�P���X�̏ꍇ�A�o�X��|�C���g�̒◯���R�[�h(STOP_ID)���m�F
		for( auto& seqChk : sameLineMap ){

			// �V�[�P���X���d�����Ă���ꍇ
			if( 1 < seqChk.second.size() ){

				set<long> stopIDs;

				// �o�X��|�C���g�̒◯���R�[�h(STOP_ID)���m�F����
				for( auto& stopPointID : seqChk.second ){

					// �o�X��ԏ��̃o�X��|�C���gID(BUS_STOP_POINT_ID)����A�o�X��|�C���g���擾���A�◯���R�[�h(STOP_ID)���i�[����
					long count = 0;
					IFeaturePtr ipStopFeature = GetStopPoint( stopPointID.first.first, count );
					if( ipStopFeature ){
						CComVariant vaValue;
						ipStopFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kStopID ), &vaValue );
						stopIDs.insert( vaValue.lVal );
					}
				}
				
				// ����H���A����V�[�P���X�ŁA�o�X��|�C���gID(STOP_ID)���قȂ�ꍇ
				if( 1 < stopIDs.size() ){
							
					for( auto& errOIDs : seqChk.second ){

						// �y����H���̒�ԏ��ŁA����◯���R�[�h�ȊO�̃o�X��ŏ����ԍ����d�����Ă���z
						m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << errOIDs.first.second 
							<< "\tERROR\t����H���̒�ԏ��ŁA����◯���R�[�h�ȊO�̃o�X��ŏ����ԍ����d�����Ă���\t" 
							<< CT2CA(errOIDs.second.first) << "\t" << CT2CA(errOIDs.second.second.first) << "\t" 
							<< CT2CA(errOIDs.second.second.second) << "\t" << seqChk.first << "\t0\t" << endl;
					}
				}
			}
		}
	}
}

// �o�X��|�C���g�ɑΉ����郋�[�g�T���p�|�C���g���擾
IFeaturePtr CCheckBusPoint::GetRoutePoint( const long& oID, long& count )
{
	count = 0;

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// �o�X��|�C���gID
	strWhere.Format( _T("%s = %d"), bus_route_point::kBusStopPointID, oID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// ���J�E���g
	m_ipBusRoutePointFC->FeatureCount( ipFilter, &count );

	// 1�ȊO��NULL��Ԃ�
	if( count != 1 ){
		return nullptr;
	}

	// ����
	IFeaturePtr ipFeature;
	IFeatureCursorPtr ipCursor;
	m_ipBusRoutePointFC->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	if( ipCursor ){
		ipCursor->NextFeature( &ipFeature );
		return ipFeature;
	}
	else{
		return nullptr;
	}
}

// �t�B�[�`���Ԃ̋���(m)�𑪒�
double CCheckBusPoint::GetDistFeature( const IFeaturePtr& ipSrcFeature, const IFeaturePtr& ipDestFeature )
{
	static crd_cnv crdCnv;

	double srcX = 0, srcY = 0, destX = 0, destY = 0;
	IGeometryPtr ipSrcGeo, ipDestGeo;
	ipSrcFeature->get_ShapeCopy( &ipSrcGeo );
	ipDestFeature->get_ShapeCopy( &ipDestGeo );
	IPointPtr ipSrcPoint( ipSrcGeo ), ipDestPoint( ipDestGeo );
	ipSrcPoint->QueryCoords( &srcX, &srcY );
	ipDestPoint->QueryCoords( &destX, &destY );

	// crd_cnv�̋������茋�ʂ�Ԃ�(m)
	return crdCnv.GetDist( srcX, srcY, destX, destY );
}

// �w��t�B�[���h��ObjectID�ɑΉ������ԏ��Ǘ��e�[�u�����擾
_ICursorPtr CCheckBusPoint::GetStopSeqs( const long& oID, const CString& field, long& count )
{
	count = 0;

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// �o�X��|�C���gID or �o�X�H���}�X�^ID
	strWhere.Format( _T("%s = %d"), field, oID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// ���J�E���g
	m_ipBusStopSeqTable->RowCount( ipFilter, &count );

	// 0��NULL��Ԃ�
	if( count == 0 ){
		return nullptr;
	}

	// ����
	_IRowPtr    ipRow;
	_ICursorPtr ipCursor;
	m_ipBusStopSeqTable->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	
	return ipCursor;
}

// ��ԏ��Ǘ��e�[�u���̘H�����ID����H�����Ǘ��e�[�u�����擾
_IRowPtr CCheckBusPoint::GetLineMaster( const long& lineID )
{
	long count = 0;

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// (�H�����Ǘ��e�[�u��)OBJECTID = (��ԏ��Ǘ��e�[�u��)�H�����ID
	strWhere.Format( _T("OBJECTID = %d"), lineID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// ���J�E���g
	m_ipBusLineMasterTable->RowCount( ipFilter, &count );

	// 0��NULL��Ԃ�
	if( count == 0 ){
		return nullptr;
	}

	// ����
	_IRowPtr    ipRow;
	_ICursorPtr ipCursor;
	m_ipBusLineMasterTable->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	if( ipCursor ){
		ipCursor->NextRow( &ipRow );
		return ipRow;
	}
	else{
		return nullptr;
	}
}

// �o�X��|�C���g�̎w��ߖT�̃o�X��|�C���g���擾
IFeatureCursorPtr CCheckBusPoint::GetNearStopPoints( const IFeaturePtr& ipFeature, long& count )
{
	count = 0;

	// �w��ߖT��Buffer���쐬
	IGeometryPtr ipGeo, ipBuf;
	ipFeature->get_ShapeCopy( &ipGeo );

	ISpatialReferenceFactory2Ptr ipSpRefFct(CLSID_SpatialReferenceEnvironment);
	IProjectedCoordinateSystemPtr ipProjectedCoordinateSystem;

	// ����
	ipSpRefFct->CreateProjectedCoordinateSystem(esriSRProjCS_TokyoJapan9, &ipProjectedCoordinateSystem);
	ipGeo->Project(ipProjectedCoordinateSystem);

	((ITopologicalOperatorPtr)ipGeo)->Buffer( k_pole_to_pole_dist, &ipBuf );
	ISpatialFilterPtr ipFilter( AheInitSpatialFilter( NULL, ipBuf ));

	// �������g�����O����Where��
	long oID = 0;
	ipFeature->get_OID( &oID );
	CString strWhere;
	strWhere.Format( _T("OBJECTID <> %d"), oID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	m_ipBusStopPointFC->FeatureCount( ipFilter, &count );

	// 0��NULL��Ԃ�
	if( count == 0 ){
		return nullptr;
	}

	// �w��ߖT�͈͂̃o�X��|�C���g������
	IFeatureCursorPtr ipCursor;
	m_ipBusStopPointFC->Search( ipFilter, VARIANT_FALSE, &ipCursor );

	return ipCursor;
}

// �o�X��|�C���g�̃J�[�\������A�W���|�C���g�����J�E���g
long CCheckBusPoint::CountPolePoint( const IFeatureCursorPtr& ipCursor )
{
	long count = 0;
	IFeaturePtr ipFeature;
	while( ipCursor && S_OK == ipCursor->NextFeature( &ipFeature )){

		// �t�B�[�`����NULL�`�F�b�N
		if( !ipFeature ){
			continue;
		}

		// �W���|�C���g���ǂ����m�F
		CComVariant vaValue;
		ipFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kPointAttrCode ), &vaValue );

		if( bus_stop_point::point_attr_code::kPole == vaValue.lVal ){
			
			// �������������J�E���g
			++count;
		}
	}
	return count;
}

// ���[�g�T���p�|�C���gor��ԏ��ɑΉ�����o�X��|�C���g���擾
IFeaturePtr CCheckBusPoint::GetStopPoint( const long& stopID, long& count )
{
	count = 0;

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// (�o�X��|�C���g)OBJECTID = (���[�g�T���p�|�C���gor��ԏ�)�o�X��|�C���gID
	strWhere.Format( _T("OBJECTID = %d"), stopID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// ���J�E���g
	m_ipBusStopPointFC->FeatureCount( ipFilter, &count );

	// 0��NULL��Ԃ�
	if( count != 1 ){
		return nullptr;
	}

	// ����
	IFeatureCursorPtr ipCursor;
	m_ipBusStopPointFC->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	if( ipCursor ){
		IFeaturePtr ipFeature;
		ipCursor->NextFeature( &ipFeature ); 
		return ipFeature;
	}
	else{
		return nullptr;
	}
}

// ���[�g�T���p�|�C���g�̎w��ߖT�̓��H�����N���擾
IFeaturePtr CCheckBusPoint::GetNearRoadLink( const IFeaturePtr& ipFeature )
{
	long count = 0;

	// �w��ߖT��Buffer���쐬
	IGeometryPtr ipGeo, ipBuf;
	ipFeature->get_ShapeCopy( &ipGeo );

	ISpatialReferenceFactory2Ptr ipSpRefFct(CLSID_SpatialReferenceEnvironment);
	IProjectedCoordinateSystemPtr ipProjectedCoordinateSystem;

	// ����
	ipSpRefFct->CreateProjectedCoordinateSystem(esriSRProjCS_TokyoJapan9, &ipProjectedCoordinateSystem);
	ipGeo->Project(ipProjectedCoordinateSystem);

	((ITopologicalOperatorPtr)ipGeo)->Buffer( k_route_to_nw_dist, &ipBuf );
	ISpatialFilterPtr ipFilter( AheInitSpatialFilter( NULL, ipBuf ));

	// �w��ߖT�͈͂̓��H�����N���J�E���g
	m_ipRoadLinkFC->FeatureCount( ipFilter, &count );

	// 0��NULL��Ԃ�
	if( count == 0 ){
		return nullptr;
	}

	// �w��ߖT�͈͂̓��H�����N������
	IFeatureCursorPtr ipCursor;
	m_ipRoadLinkFC->Search( ipFilter, VARIANT_FALSE, &ipCursor );

	if( ipCursor ){
		IFeaturePtr ipRetFeature;
		ipCursor->NextFeature( &ipRetFeature );
		return ipRetFeature;
	}
	else{
		return nullptr;
	}
}


