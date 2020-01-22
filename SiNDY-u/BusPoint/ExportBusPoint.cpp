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
#include "ExportBusPoint.h"
#include "BusPointCommon/Time.h"
#include "BusPointCommon/CommonFunc.h"
#include <sindy/workspace.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace sindy::schema::bus;

// �e�폈�������s���邽�߂ɍŏ��ɌĂ΂��֐�
bool CExportBusPoint::execute()
{
	// �e�평���������{
	if( !Init() ){
		return false;
	}

	// �o�͑Ώێ��Ǝ҃��X�g�쐬
	if( m_allCompany ){
		// �S���Ǝҏo�̓��[�h�̏ꍇ
		if( !CreateCompanyList() ){
			cerr << "#ERROR �H���}�X�^����S���Ǝ҃��X�g�쐬���s" << endl;
			return false;
		}
	}
	else{
		// �o�͎��ƎҖ����X�g�w��̏ꍇ
		if( !LoadList() ){
			cerr << "#ERROR ���ƎҖ����X�g�̓ǂݍ��ݎ��s" << endl;
			return false;
		}
	}

	// �`�F�b�N��E�Q�Ɛ�DB���o��
	m_ofsExp << "#DB_BUS�F" << CT2CA(m_strBusDB) << endl;

	// �J�n����
	m_ofsExp << "#�J�n���ԁF" << CT2CA(GetCurTime()) << endl;

	for( auto& companyName : m_companySet ){

		m_ofsExp << "#�o�͎��ƎҖ��F" << CT2CA(companyName) << endl;

		// �o�X��o�H���o�̓t�@�C���I�[�v���i<�w��Dir>\�o�X��E�o�X�o�H���i���ƎЖ��jyyyymmdd.csv�j
		CString strDetailFile;
		strDetailFile.Format( _T("%s\\�o�X��E�o�X�o�H���i%s�j%s.csv"), m_strExpDir, companyName, GetCurDay() );
		m_ofsDetailFile.open( strDetailFile );
		if( !m_ofsDetailFile ){
			cerr << "#ERROR �o�X��o�H���o�̓t�@�C���I�[�v���Ɏ��s�F" << CT2CA(strDetailFile) << endl;
			return false;
		}

		// �o�X�◯�����ٕ\�L���o�̓t�@�C���i<�w��Dir>\�o�X�◯�����ٕ\�L���i���ƎЖ��jyyyymmdd.csv�j
		CString strChangedFile;
		strChangedFile.Format( _T("%s\\�o�X�◯�����ٕ\�L���i%s�j%s.csv"), m_strExpDir, companyName, GetCurDay() );
		m_ofsChangedFile.open( strChangedFile );
		if( !m_ofsChangedFile ){
			cerr << "#ERROR �o�X�◯�����ٕ\�L���o�̓t�@�C���I�[�v���Ɏ��s�F" << CT2CA(strChangedFile) << endl;
			return false;
		}

		// �o�X��o�H���o�̓t�@�C���w�b�_�o��
		m_ofsDetailFile << "�ԍ�,��ʔԍ�,�����ԍ�,�◯���R�[�h,�◯����,�◯������݂���,�W���ԍ�,�n���ԍ�,"
			<< "�n����,�o�H,���◯����,���◯����,�o�R,�o�H�j��,�o�H���ԑ�,�ܓx,�o�x,���l" << endl;

		// �o�X�◯�����ٕ\�L���o�̓t�@�C���w�b�_�o��
		m_ofsChangedFile << "�ԍ�,�◯���R�[�h,�◯����,�◯������݂���,�W���ԍ�,�ٕ\�L��,�ٕ\�L��݂���,���l" << endl;

		// �o��
		Export( companyName );

		// �o�X��o�H���o�̓t�@�C���N���[�Y
		m_ofsDetailFile.close();

		// �o�X�◯�����ٕ\�L���o�̓t�@�C���N���[�Y
		m_ofsChangedFile.close();
	}

	// �I������
	m_ofsExp << "#�I�����ԁF" << CT2CA(GetCurTime()) << endl;

	return true;
}

// �e�평����
bool CExportBusPoint::Init()
{
	// DB�`�F�b�N���O�t�@�C���I�[�v��
	m_ofsExp.open( m_strExpLog );
	if( !m_ofsExp ){
		cerr << "#ERROR �G�N�X�|�[�g���O�t�@�C���I�[�v���Ɏ��s�F" << CT2CA(m_strExpLog) << endl;
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

	return true;
}

// �o�X�H���}�X�^����S���Ǝ҃��X�g���쐬����
bool CExportBusPoint::CreateCompanyList()
{
	// �o�X�H���}�X�^����S���擾
	_ICursorPtr ipCursor;
	m_ipBusLineMasterTable->Search( NULL, VARIANT_FALSE, &ipCursor );

	_IRowPtr ipLineRow;
	// �I�u�W�F�N�g���Ƃ̏���
	while( ipCursor && S_OK == ipCursor->NextRow(&ipLineRow) ){

		// �t�B�[�`����NULL�`�F�b�N
		if( !ipLineRow ){
			continue;
		}

		// ���ƎҖ����擾
		CComVariant vaValue;
		ipLineRow->get_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kCompany ), &vaValue );
		if( !consider_as_null( vaValue ) ){
			m_companySet.insert( vaValue.bstrVal );
		}
	}

	if( m_companySet.size() > 0 ){
		return true;
	}
	else{
		return false;
	}
}

// �o�͑Ώێ��Ǝ҃��X�g��ǂݍ��݁A���ƎҖ�1�s�������X�g�Ɋi�[
bool CExportBusPoint::LoadList()
{
	ifstream ifs( m_strCompanyList );
	if( !ifs ){
		return false;
	}

	string str;
	while( getline( ifs, str )){
		CString cstr = str.c_str();
	
		if( !cstr.IsEmpty() ){
			m_companySet.insert( cstr );
		}
	}

	ifs.close();

	return true;
}

// �o�̓��C���֐�
void CExportBusPoint::Export( const CString& companyName )
{
	// �o�X�H���}�X�^����A
	// �E�Y�����Ǝ҂̏����n���ԍ����L�[�ɁAObjectID�E�n�����E�o�H�E�o�R�n�����l���i�[�i�o�H���o�͗p�j
	// �EObjectID���Z�b�g�Ɋi�[�i�ٕ\�L���o�͗p�j
	LINEMASTERS lineMasters;
	OIDSET      lineOIDs;
	GetLineMaster( companyName, lineMasters, lineOIDs );

	// �o�X��o�H���o��
	ExportDetail( lineMasters );

	// �o�X�◯�����ٕ\�L���o��
	ExportChanged( lineOIDs );
}

// �o�X�H���}�X�^����Y�����Ǝ҂̏����擾
void CExportBusPoint::GetLineMaster( const CString& companyName, LINEMASTERS& lineMasters, OIDSET& lineOIDs )
{
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// ���ƎҖ�
	strWhere.Format( _T("%s = '%s'"), bus_line_master::kCompany, companyName );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// ����
	_IRowPtr    ipRow;
	_ICursorPtr ipCursor;
	m_ipBusLineMasterTable->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	
	// �I�u�W�F�N�g���Ƃ̏���
	while( ipCursor && S_OK == ipCursor->NextRow(&ipRow) ){
		
		// �t�B�[�`����NULL�`�F�b�N
		if( !ipRow ){
			m_ofsExp << "#ERROR �o�X�H���}�X�^����t�B�[�`���擾���s" << endl;
			continue;
		}

		// �������擾
		CComVariant vaValue;
		MAPATTR     mapAttr;
		for( auto& field : m_busLineMasterFM.GetFieldMap() )
		{
			ipRow->get_Value( field.second, &vaValue );
			mapAttr[field.first] = vaValue;
		}

		// ObjectID
		long oID = 0;
		ipRow->get_OID( &oID );

		// ��ŃL�[�ƂȂ�t�B�[���h��NULL�`�F�b�N(�O�̂���)
		if( consider_as_null( mapAttr[bus_line_master::kRouteNumber] ) 
			|| consider_as_null( mapAttr[bus_line_master::kRouteDetail] )
			|| 0 == oID )
		{
			m_ofsExp << "#ERROR �o�X�H���}�X�^�̌n���ԍ�or�o�HorObjectID����" << endl;
			continue;
		}

		// �n���ԍ����L�[�ɁAObjectID�E�n�����E�o�H�E�o�R�n�����l���i�[�i�o�H���o�͗p�j
		lineMasters[mapAttr[bus_line_master::kRouteNumber].lVal].push_back( make_pair(
			make_pair( oID, mapAttr[bus_line_master::kRouteName].bstrVal ),
			make_pair( mapAttr[bus_line_master::kRouteDetail].bstrVal, mapAttr[bus_line_master::kRouteRemarks].bstrVal )));

		// ObjectID���Z�b�g�Ɋi�[�i�ٕ\�L���o�͗p�j
		lineOIDs.insert( oID );
	}
}

// �o�X��o�H���o��
void CExportBusPoint::ExportDetail( LINEMASTERS& lineMasters )
{
	// �o�͗p�s�ԍ�
	long lineIdx = 0;

	// �n���ԍ����Ƃ̏���
	for( auto& lineMaster : lineMasters ){
		// lineMaster��first:�n���ԍ�, second( first:( first:ObjectID, second:�n���� ), second:( first:�o�H, second:�o�R�n�����l ))
	
		long routeNum = lineMaster.first; //!< �n���ԍ�
		
		// �o�H�̏����Ń\�[�g����
		sort( lineMaster.second.begin(), lineMaster.second.end(), lessDetail() );

		// �o�H���Ƃ̏���
		for( auto& details : lineMaster.second ){
			// routeDetails��first:( first:ObjectID, second:�n���� ), second:( first:�o�H, second:�o�R�n�����l )

			CString routeName    = details.first.second;  //!< �n����
			CString routeDetail  = details.second.first;  //!< �o�H
			CString routeRemarks = details.second.second; //!< �o�R�n�����l
			long    lineOID      = details.first.first;   //!< �H���}�X�^��ObjectID

			long    count = 0;

			// �o�X�H���}�X�^��ObjectID����A�R�Â���ԏ��Ǘ����擾
			_ICursorPtr ipSeqCursor = GetStopSeqs( lineOID, bus_stop_seq::kBusLineMasterID, count );
			_IRowPtr    ipSeqRow;

			// ����o�H�̒�ԏ��i�[�p
			STOPSEQS stopSeqs;

			// ��ԏ����Ƃ̏���
			while( ipSeqCursor && S_OK == ipSeqCursor->NextRow(&ipSeqRow) ){
		
				// �t�B�[�`����NULL�`�F�b�N
				if( !ipSeqRow ){
					m_ofsExp << "#ERROR �o�X��ԏ�����t�B�[�`���擾���s" << endl;
					continue;
				}

				// ��ԏ��̑������擾
				CComVariant vaSeqValue;
				MAPATTR     mapSeqAttr;
				for( auto& field : m_busStopSeqFM.GetFieldMap() )
				{
					ipSeqRow->get_Value( field.second, &vaSeqValue );
					mapSeqAttr[field.first] = vaSeqValue;
				}

				// �����ԍ��Ń\�[�g���ďo�͂��邽�߁A����o�H�̒�ԏ��������ԍ����L�[�ɑ�����map�Ɋi�[
				// �i�����ԍ����L�[�ɂ��邪�A�����ԍ��͏d�����邽�߁Amultimap�j
				stopSeqs.insert( make_pair( mapSeqAttr[bus_stop_seq::kStopSeq].lVal, mapSeqAttr ) );
			}

			// �o�H���Œ�ԏ���������݂��Ȃ��ꍇ�̓X�L�b�v
			if( stopSeqs.size() < 1 ){
				continue;
			}

			// ���◯�����擾(�o�H���̒�ԏ������ԍ���1��(�ύX��)�◯����)
			CString startStopName = GetStopName( stopSeqs.begin()->second );

			// ���◯�����擾(�o�H���̒�ԏ������ԍ����ő��(�ύX��)�◯����)
			CString endStopName   = GetStopName( stopSeqs.rbegin()->second );

			for( auto& stopSeq : stopSeqs ){ 

				// �o�X��|�C���gID��NULL�`�F�b�N(�O�̂���)
				if( consider_as_null( stopSeq.second[bus_stop_seq::kBusStopPointID] )){
					m_ofsExp << "#ERROR �o�X��ԏ�����o�X��|�C���gID�擾���s" << endl;
					continue;
				}

				// ��ԏ��̃o�X��|�C���gID����A�o�X��|�C���g���擾
				IFeaturePtr ipStopFeature = GetStopPoint( stopSeq.second[bus_stop_seq::kBusStopPointID].lVal, count );

				// �擾�����o�X��|�C���g�̏���
				if( ipStopFeature ){

					// �o�X��|�C���g�̑������擾
					CComVariant vaStopValue;
					MAPATTR     mapStopAttr;
					for( auto& field : m_busStopPointFM.GetFieldMap() )
					{
						ipStopFeature->get_Value( field.second, &vaStopValue );
						mapStopAttr[field.first] = vaStopValue;
					}

					// �o�X��|�C���g��ObjectID�擾
					long stopID = 0;
					ipStopFeature->get_OID( &stopID );

					// �o�X��|�C���g��ObjectID����A���[�g�T���p�|�C���g���擾
					IFeaturePtr ipRouteFeature = GetRoutePoint( stopID, count );
					
					// �擾�������[�g�T���p�|�C���g�̏���
					if( ipRouteFeature ){

						// ���[�g�T���p�|�C���g�̌`�󂩂�A1/256�b�ܓx�E�o�x���擾
						int routeLat256 = 0, routeLon256 = 0;
						Get256LatLon( ipRouteFeature, routeLat256, routeLon256 );

						// ���[�g�T���p�|�C���g��ObjectID�擾
						long routeID = 0;
						ipRouteFeature->get_OID( &routeID );


						// // �o�X��o�H���o�̓t�@�C���Ƀ��[�g�T���p�|�C���g1�s�o��
						if( !ExportDetailLine( ++lineIdx, routeLat256, routeLon256, routeNum, routeName, routeDetail, routeRemarks, startStopName, endStopName, stopSeq.second, mapStopAttr, true )){
							m_ofsExp << "#ERROR ���[�g�T���p�|�C���g1�s�o�͎��s�i�ԍ��F" << lineIdx << "�AObjectID�F" << routeID << "�j" << endl;
						}
					}

					// �o�X��|�C���g�̌`�󂩂�A1/256�b�ܓx�E�o�x���擾
					int stopLat256 = 0, stopLon256 = 0; 
					Get256LatLon( ipStopFeature, stopLat256, stopLon256 );


					// �o�X��o�H���o�̓t�@�C���Ƀo�X��|�C���g1�s�o��
					if( !ExportDetailLine( ++lineIdx, stopLat256, stopLon256, routeNum, routeName, routeDetail, routeRemarks, startStopName, endStopName, stopSeq.second, mapStopAttr )){
						m_ofsExp << "#ERROR �o�X��|�C���g1�s�o�͎��s�i�ԍ��F" << lineIdx << "�AObjectID�F" << stopID << "�j" << endl;
					}
				}
				else{
					// �o�X��|�C���g�擾���s
					if( !ipStopFeature ){
						m_ofsExp << "#ERROR ��ԏ��̃o�X��|�C���gID����o�X��|�C���g�擾���s" << endl;
						continue;
					}
				}
			}
		}

	}
}

// �o�X�◯�����ٕ\�L���o��
void CExportBusPoint::ExportChanged( const OIDSET& lineOIDs )
{
	// �o�͗p�s�ԍ�
	long lineIdx = 0;

	long count = 0;

	// �o�X��|�C���g�i�[�p
	STOPPOINTS stopPoints;

	// �Y�����ƎҖ������H���}�X�^��ObjectID���Ƃ̏���
	for( auto& lineOID : lineOIDs ){

		// �o�X�H���}�X�^��ObjectID����A�R�Â���ԏ��Ǘ����擾
		_ICursorPtr ipSeqCursor = GetStopSeqs( lineOID, bus_stop_seq::kBusLineMasterID, count );
		_IRowPtr    ipSeqRow;

		// ��ԏ����Ƃ̏���
		while( ipSeqCursor && S_OK == ipSeqCursor->NextRow(&ipSeqRow) ){
		
			// �t�B�[�`����NULL�`�F�b�N
			if( !ipSeqRow ){
				m_ofsExp << "#ERROR �o�X��ԏ�����t�B�[�`���擾���s" << endl;
				continue;
			}

			// ��ԏ��̃o�X��|�C���gID���擾
			CComVariant vaSeqValue;
			ipSeqRow->get_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kBusStopPointID ), &vaSeqValue );

			// ��ԏ��̃o�X��|�C���gID����o�X��|�C���g���擾
			IFeaturePtr ipStopFeature = GetStopPoint( vaSeqValue.lVal, count );

			// �擾�����o�X��|�C���g�̏���
			if( ipStopFeature ){

				// �o�X��|�C���g�̑������擾
				CComVariant vaStopValue;
				MAPATTR     mapStopAttr;
				for( auto& field : m_busStopPointFM.GetFieldMap() )
				{
					ipStopFeature->get_Value( field.second, &vaStopValue );
					mapStopAttr[field.first] = vaStopValue;
				}

				// �\�[�g�o�͂̂��߁A�◯���R�[�h����L�[�A�W���ԍ�����L�[�Ƃ��āA�o�X��|�C���g�̑�����map�Ɋi�[
				stopPoints[mapStopAttr[bus_stop_point::kStopID].lVal].insert( 
					make_pair( mapStopAttr[bus_stop_point::kPoleNo].lVal, mapStopAttr ));
			}
		}
	}

	// �◯���R�[�h���Ƃ̃o�X��|�C���g����
	for( auto& stopPoint : stopPoints ){

		// �W���ԍ����Ƃ̃o�X��|�C���g����
		for( auto& stopAttr : stopPoint.second ){

			// �ύX��◯�����̂�NULL�ȊO���o�͑Ώ�
			if( !consider_as_null( stopAttr.second[bus_stop_point::kChangedName] )){

				ExportChangedLine( ++lineIdx, stopAttr.second );
			}
		}
	}
}

// �Y����ԏ��ƕR�Â��o�X��|�C���g��(�ύX��)�◯�����擾
CString CExportBusPoint::GetStopName( const MAPATTR& mapSeqAttr )
{
	try{
		// ��ԏ��̃o�X��|�C���gID����A�o�X��|�C���g���擾
		long stopCount = 0;
		IFeaturePtr ipStopFeature = GetStopPoint( (mapSeqAttr.at( bus_stop_seq::kBusStopPointID )).lVal, stopCount );
	
		if( ipStopFeature ){
			CComVariant vaValue;
	
			// BUS_STOP_SEQ�ƕR�Â��o�X��|�C���g��
			//   CHANGED_STOP_NAME_KANJI != NULL�FCHANGED_STOP_NAME_KANJI
			//   CHANGED_STOP_NAME_KANJI  = NULL�FSTOP_NAME_KANJI 
			ipStopFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kChangedName ), &vaValue );

			if( !consider_as_null( vaValue ) ){
				return vaValue.bstrVal;
			}
			else{
				ipStopFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kName ), &vaValue );
				if( !consider_as_null( vaValue ) ){
					return vaValue.bstrVal;
				}
			}
		}
	}
	catch( out_of_range& e ){
		m_ofsExp << "#ERROR �o�X��|�C���g�̒◯�����擾���ɗ�O�������F" << e.what() << endl;
		return _T("");
	}

	// �o�X��|�C���g��������Ȃ� or ���̂��ǂ����NULL�͋󕶎�
	return _T("");
}

// �t�B�[�`������A1/256�b�ܓx�E�o�x���擾
void CExportBusPoint::Get256LatLon( const IFeaturePtr& ipFeature, int& lat256, int& lon256 )
{
	static crd_cnv crdCnv;

	IGeometryPtr ipGeo;
	ipFeature->get_ShapeCopy( &ipGeo );
	IPointPtr ipPoint( ipGeo );

	double x = 0, y = 0;
	ipPoint->QueryCoords( &x, &y );

	crdCnv.LLtoLL256( x, y, &lon256, &lat256 );
}

// �o�X��o�H���o�̓t�@�C����1�s�o��
bool CExportBusPoint::ExportDetailLine( const long lineIdx, const int lat256, const int lon256, const long routeNum, const CString& routeName, const CString& routeDetail, const CString& routeRemarks, const CString& startStopName, const CString& endStopName, const MAPATTR& mapSeqAttr, const MAPATTR& mapStopAttr, bool routeFlag /* = false */ )
{
	try{
		// �y��ʔԍ��z
		// ���[�g�T���p�|�C���g�F�u0�v�ŌŒ�
		// �o�X��|�C���g�iBUS_STOP_SEQ.PASS_F=1�̏ꍇ�j�F�u2�v(�ʉߒ◯���|�C���g)
		//               �i����ȊO�̏ꍇ�j             �FBUS_STOP_POINT.POINT_ATTR_CODE�̒l
		long attrCode = 0;
		if( !routeFlag ){
			attrCode = ((mapSeqAttr.at( bus_stop_seq::kPassF )).lVal == 1)?  2 : (mapStopAttr.at( bus_stop_point::kPointAttrCode )).lVal;
		}

		// �y�◯�����z
		// BUS_STOP_POINT.CHANGED_STOP_NAME_KANJI != NULL�FBUS_STOP_POINT.CHANGED_STOP_NAME_KANJI
		// BUS_STOP_POINT.CHANGED_STOP_NAME_KANJI  = NULL�FBUS_STOP_POINT.STOP_NAME_KANJI
		CString stopName = !consider_as_null(mapStopAttr.at( bus_stop_point::kChangedName ))? 
				(mapStopAttr.at( bus_stop_point::kChangedName )).bstrVal : (mapStopAttr.at( bus_stop_point::kName )).bstrVal;

		// �y�◯������݂��ȁz
		// BUS_STOP_POINT.CHANGED_STOP_NAME_YOMI != NULL�FBUS_STOP_POINT.CHANGED_STOP_NAME_YOMI
		// BUS_STOP_POINT.CHANGED_STOP_NAME_YOMI  = NULL�FBUS_STOP_POINT.STOP_NAME_YOMI
		CString stopYomi = !consider_as_null(mapStopAttr.at( bus_stop_point::kChangedYomi ))? 
				(mapStopAttr.at( bus_stop_point::kChangedYomi )).bstrVal : (mapStopAttr.at( bus_stop_point::kYomi )).bstrVal;

		// �y�o�H�j���z
		CString dayCodes = !consider_as_null( mapSeqAttr.at( bus_stop_seq::kDayCodes ))? 
				(mapSeqAttr.at( bus_stop_seq::kDayCodes )).bstrVal : _T("");

		// �y�o�H���ԑсz
		CString timeCodes = !consider_as_null( mapSeqAttr.at( bus_stop_seq::kTimeCodes ))?
				(mapSeqAttr.at( bus_stop_seq::kTimeCodes )).bstrVal : _T("");

		// �y���l�z
		// ����3�̓��e���o�͂���B�������鎞��";"(�Z�~�R����)�ŋ�؂�B
		// 1.BUS_STOP_SEQ.REMARKS�ɓ��͂���Ă���΂��̓��e�����̂܂܏o�́B
		// 2.�◯�����̂�BUS_STOP_POINT.CHANGED_STOP_NAME_KANJI����擾�����ꍇ�́u���̕ύX([STOP_NAME_KANJI]->[CHANGED_STOP_NAME_KANJI])�v�Əo��
		// 3.BUS_STOP_POINT.GETTING_OFF_ONLY=1�Ȃ�"�~�Ԑ�p"�Əo��
		CString etcRemarks;
		if( !consider_as_null( mapSeqAttr.at( bus_stop_seq::kRemarks ))){
			etcRemarks = mapSeqAttr.at( bus_stop_seq::kRemarks );
		}
		if( !consider_as_null( mapStopAttr.at( bus_stop_point::kChangedName ))){
			
			// 1�����͂���Ă���ꍇ�́A�u ; �v���͌�ɖ��̕ύX����ǉ�
			etcRemarks.AppendFormat( _T("%s���̕ύX(%s->%s)"), 
				etcRemarks.IsEmpty()? _T("") : _T(" ; "),
				(mapStopAttr.at( bus_stop_point::kName )).bstrVal,
				(mapStopAttr.at( bus_stop_point::kChangedName )).bstrVal );
		}
		if( 1 == (mapStopAttr.at( bus_stop_point::kGettingOffF )).lVal ){

			// 1��2�̂ǂ��炩�ł����͂���Ă���ꍇ�́A�u ; �v���͌�Ɂu�~�Ԑ�p�v��ǉ�
			etcRemarks.AppendFormat( _T("%s�~�Ԑ�p"), etcRemarks.IsEmpty()? _T("") : _T(" ; ") );
		}

		// 1�s�o��
		m_ofsDetailFile 
			<< lineIdx << ","                                         //!< �s�ԍ�
			<< attrCode << ","                                        //!< ��ʔԍ�
			<< (mapSeqAttr.at( bus_stop_seq::kStopSeq )).lVal << ","  //!< �����ԍ�
			<< (mapStopAttr.at( bus_stop_point::kStopID)).lVal << "," //!< �◯���R�[�h
			<< CT2CA(stopName) << ","                                 //!< �◯����
			<< CT2CA(stopYomi) << ","                                 //!< �◯������݂���
			<< (mapStopAttr.at( bus_stop_point::kPoleNo)).lVal << "," //!< �W���ԍ�
			<< routeNum << ","                                        //!< �n���ԍ�
			<< CT2CA(routeName) << ","                                //!< �n����
			<< CT2CA(routeDetail) << ","                              //!< �o�H
			<< CT2CA(startStopName) << ","                            //!< ���◯����
			<< CT2CA(endStopName) << ","                              //!< ���◯����
			<< CT2CA(routeRemarks) << ","                             //!< �o�R
			<< CT2CA(dayCodes) << ","                                 //!< �o�H�j��
			<< CT2CA(timeCodes)  << ","                               //!< �o�H���ԑ�
			<< lat256 << ","                                          //!< 1/256�b�ܓx
			<< lon256 << ","                                          //!< 1/256�b�o�x
			<< CT2CA(etcRemarks)                                      //!< ���l
			<< endl;
	}
	catch( out_of_range& e ){
		m_ofsExp << "#ERROR �o�X��o�H���o�̓t�@�C����1�s�o�͎��ɗ�O�������F" << e.what() << endl;
		return false;
	}
	return true;
}

// �o�X�◯�����ٕ\�L���o�̓t�@�C����1�s�o��
bool CExportBusPoint::ExportChangedLine( const long lineIdx, const MAPATTR& mapStopAttr )
{
	try{
		CString name        = !consider_as_null( mapStopAttr.at( bus_stop_point::kName ))?        (mapStopAttr.at( bus_stop_point::kName )).bstrVal        : _T("");
		CString yomi        = !consider_as_null( mapStopAttr.at( bus_stop_point::kYomi ))?        (mapStopAttr.at( bus_stop_point::kYomi )).bstrVal        : _T("");
		CString changedName = !consider_as_null( mapStopAttr.at( bus_stop_point::kChangedName ))? (mapStopAttr.at( bus_stop_point::kChangedName )).bstrVal : _T("");
		CString changedYomi = !consider_as_null( mapStopAttr.at( bus_stop_point::kChangedYomi ))? (mapStopAttr.at( bus_stop_point::kChangedYomi )).bstrVal : _T("");
		CString remarks     = !consider_as_null( mapStopAttr.at( bus_stop_point::kRemarks ))?     (mapStopAttr.at( bus_stop_point::kRemarks )).bstrVal     : _T("");

		// 1�s�o��
		m_ofsChangedFile 
			<< lineIdx << ","                                          //!< �s�ԍ�
			<< (mapStopAttr.at( bus_stop_point::kStopID)).lVal << ","  //!< �◯���R�[�h
			<< CT2CA(name) << ","                                      //!< �◯����
			<< CT2CA(yomi) << ","                                      //!< �◯������݂���
			<< (mapStopAttr.at( bus_stop_point::kPoleNo )).lVal << "," //!< �W���ԍ�
			<< CT2CA(changedName) << ","                               //!< �ٕ\�L��
			<< CT2CA(changedYomi) << ","                               //!< �ٕ\�L����݂���
			<< CT2CA(remarks)                                          //!< ���l
			<< endl;
	}
	catch( out_of_range& e ){
		m_ofsExp << "#ERROR �o�X�◯�����ٕ\�L���o�̓t�@�C����1�s�o�͎��ɗ�O�������F" << e.what() << endl;
		return false;
	}
	return true;
}

// �w��t�B�[���h��ObjectID�ɑΉ������ԏ��Ǘ��e�[�u�����擾
_ICursorPtr CExportBusPoint::GetStopSeqs( const long oID, const CString& field, long& count )
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

// ���[�g�T���p�|�C���gor��ԏ��ɑΉ�����o�X��|�C���g���擾
IFeaturePtr CExportBusPoint::GetStopPoint( const long stopID, long& count )
{
	count = 0;

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// (�o�X��|�C���g)OBJECTID = (���[�g�T���p�|�C���gor��ԏ�)�o�X��|�C���gID
	strWhere.Format( _T("OBJECTID = %d"), stopID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// ���J�E���g
	m_ipBusStopPointFC->FeatureCount( ipFilter, &count );

	// 1�ȊO��NULL��Ԃ�
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

// �o�X��|�C���g�ɑΉ����郋�[�g�T���p�|�C���g���擾
IFeaturePtr CExportBusPoint::GetRoutePoint( const long oID, long& count )
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
