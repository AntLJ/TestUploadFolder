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

// ImportBusPoint.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "ImportBusPoint.h"
#include "BusPointCommon/Time.h"
#include <WinLib/VersionInfo.h>
#include <ArcHelperEx/GlobalFunctions.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace sindy::schema::bus;

namespace 
{
	// �X�L�[�}�őΉ��ł��Ȃ��t�B�[���h�̒�`
	const char * const k_line_idx         = "LINEIDX";
	const char * const k_name_remarks     = "NAMEREMARKS";
	const char * const k_day_time_remarks = "DAYTIMEREMARKS";
	const char * const k_dms_lat          = "DMSLAT";
	const char * const k_dms_lon          = "DMSLON";

	crd_cnv g_crd;
}

// �e�폈�������s���邽�߂ɍŏ��ɌĂ΂��֐�
bool CImportBusPoint::execute()
{
	// �e�평���������{
	if( !Init() ){
		return false;
	}

	// �C���|�[�g���ʃt���O
	bool impOK = true;

	if( m_import ){
	
		// �C���|�[�g�p�R���e�i
		BUSPOINTMAP  busStopPoints;
		BUSMASTERMAP busLineMasters;
		BUSSEQVEC    busStopSeqs; 

		// �C���|�[�g��DB���o��
		m_ofsImp << "#DB_BUS�F" << CT2CA(m_strBusDB) << endl;

		// �����̌n���ԍ���S�Ď擾����
		GetExistRouteNumber();

		// �����̒◯���R�[�h�ƕW���ԍ��̃y�A��S�Ď擾����
		GetExistStopAndPole();

		// �������ʃt�@�C���ǂݍ���
		if( !LoadExcelData( busStopPoints, busLineMasters, busStopSeqs )){
			m_ofsImp << "#ERROR �C���|�[�g�������s" << endl;
			return false;
		}

		// �C���|�[�g����
		if( ImportBusPoint( busStopPoints, busLineMasters, busStopSeqs ) ){
			m_ofsImp << "#�C���|�[�g��������" << endl;
		}
		else{
			impOK = false;
			m_ofsImp << "#ERROR �C���|�[�g�������s" << endl;
		}
	}

	if( m_autoRPoint ){

		if( impOK ){

			// �쐬��E�Q�Ɛ�DB���o��
			m_ofsARP << "#DB_BUS�F" << CT2CA(m_strBusDB) << endl;
			m_ofsARP << "#DB_ROAD�F" << CT2CA(m_strRoadDB) << endl;

			// ���R�`���w�b�_�o��
			m_ofsARP << "#FREESTYLELOG" << endl;
			m_ofsARP << "LAYER\tOBJECTID\tSTOP_ID\tPOLE_NO\tSTOP_NAME_KANJI\tSTOP_NAME_YOMI\tCOMMENT" << endl;

			// ���[�g�T���p�|�C���g�쐬
			CreateRPoints();
		}
		else{
			// �C���|�[�g������NG�������ꍇ�́A���O�ɂ��̎|���L�ڂ���݂̂Ƃ���
			m_ofsARP << "#�C���|�[�g���[�h�ŃG���[�����������߁A���[�g�T���p�|�C���g���[�h�͎��{���܂���" << endl;
		}

	}

	return true;
}

// �e�평����
bool CImportBusPoint::Init()
{
	// �������ʃt�@�C���p�X���X�g��ǂݍ��݁A�t�@�C���p�X��vector�Ɋi�[
	if( m_import && !LoadList()){
		cerr << "#ERROR �������ʃt�@�C�����X�g�̓ǂݍ��݂Ɏ��s�F" << CT2CA(m_strInFile) << endl;
		return false;
	}

	// �C���|�[�g���O�t�@�C���I�[�v��
	if( m_import ){
		m_ofsImp.open( m_strImpLog );
		if( !m_ofsImp ){
			cerr << "#ERROR �C���|�[�g���O�t�@�C���I�[�v���Ɏ��s�F" << CT2CA(m_strImpLog) << endl;
			return false;
		}
	}

	// ���[�g�T���pP���O�t�@�C���I�[�v��
	if( m_autoRPoint ){
		m_ofsARP.open( m_strARPLog );
		if( !m_ofsARP ){
			cerr << "#ERROR �C���|�[�g���O�t�@�C���I�[�v���Ɏ��s�F" << CT2CA(m_strARPLog) << endl;
			return false;
		}
	}

	// �C���|�[�g���� or ���[�g�T���pP����
	if( m_import || m_autoRPoint ){
	
		// �o�X�֘A���[�N�X�y�[�X�I�[�v��
		if( ! DB::OpenWorkspace( m_strBusDB, m_ipBusWorkspace ) )
			return false;

		// �o�X��|�C���g�t�B�[�`���N���X�I�[�v�����t�B�[���h�}�b�v�쐬
		if( ! DB::OpenFeatureClass( bus_stop_point::kTableName, m_ipBusWorkspace, m_ipBusStopPointFC, m_busStopPointFM ) )
			return false;

		// �C���|�[�g����
		if( m_import ){
			// �o�X�H���}�X�^�e�[�u���I�[�v�����t�B�[���h�}�b�v�쐬
			if( ! DB::OpenTable( bus_line_master::kTableName, (IWorkspacePtr)m_ipBusWorkspace, m_ipBusLineMasterTable, m_busLineMasterFM ) )
				return false;
			
			// �o�X��ԏ��Ǘ��e�[�u���I�[�v�����t�B�[���h�}�b�v�쐬
			if( ! DB::OpenTable( bus_stop_seq::kTableName, (IWorkspacePtr)m_ipBusWorkspace, m_ipBusStopSeqTable, m_busStopSeqFM ) )
				return false;

			// ���Ǝ҃t�@�C���񖼂����X�g�Ɋi�[
			SetColumnName();
		}

		// ���[�g�T���p�|�C���g����
		if( m_autoRPoint ){
			// �o�X���[�g�T���p�|�C���g�t�B�[�`���N���X�I�[�v�����t�B�[���h�}�b�v�쐬
			if( ! DB::OpenFeatureClass( bus_route_point::kTableName, m_ipBusWorkspace, m_ipBusRoutePointFC, m_busRoutePointFM ) )
				return false;
		}
	}

	if( m_autoRPoint ){
		// ���HNW���[�N�X�y�[�X�I�[�v��
		if( ! DB::OpenWorkspace( m_strRoadDB, m_ipRoadWorkspace ) )
			return false;

		// ���HNW(ROAD_LINK)�t�B�[�`���N���X�I�[�v��
		CFieldMap roadLinkFM;
		if( ! DB::OpenFeatureClass( road_link::kTableName, m_ipRoadWorkspace, m_ipRoadLinkFC, roadLinkFM ) )
			return false;
	}
	return true;
}

// �񖼂����X�g�Ɋi�[
void CImportBusPoint::SetColumnName()
{
	m_columnNameList.push_back( bus_stop_point::kPointAttrCode ); //!< ��ʔԍ�
	m_columnNameList.push_back( bus_stop_seq::kStopSeq );         //!< �����ԍ�
	m_columnNameList.push_back( bus_stop_point::kStopID );        //!< �◯���R�[�h
	m_columnNameList.push_back(  bus_stop_point::kName );         //!< �◯����
	m_columnNameList.push_back( bus_stop_point::kYomi );          //!< �◯������݂���
	m_columnNameList.push_back( bus_stop_point::kChangedName );   //!< �ύX��◯����
	m_columnNameList.push_back( bus_stop_point::kChangedYomi );   //!< �ύX��◯������݂���
	m_columnNameList.push_back( k_name_remarks );                 //!< ���̕ύX���l
	m_columnNameList.push_back( bus_stop_point::kPoleNo );        //!< �W���ԍ�
	m_columnNameList.push_back( bus_stop_point::kGettingOffF );   //!< �~�Ԑ�p
	m_columnNameList.push_back( bus_line_master::kRouteNumber );  //!< �n���ԍ�
	m_columnNameList.push_back( bus_line_master::kRouteName );    //!< �n����
	m_columnNameList.push_back( bus_line_master::kRouteDetail );  //!< �o�H
	m_columnNameList.push_back( bus_line_master::kRouteRemarks ); //!< �o�R�n�����l
	m_columnNameList.push_back( bus_stop_seq::kDayCodes );        //!< �o�H�j��
	m_columnNameList.push_back( bus_stop_seq::kTimeCodes );       //!< �o�H���ԑ�
	m_columnNameList.push_back( k_day_time_remarks );             //!< �j�����Ԕ��l
	m_columnNameList.push_back( k_dms_lat );                      //!< DMS�ܓx
	m_columnNameList.push_back( k_dms_lon );                      //!< DMS�o�x
}

// �������ʃt�@�C�����X�g��ǂݍ��݁A�t�@�C���p�X1�s�������X�g�Ɋi�[
bool CImportBusPoint::LoadList()
{
	ifstream ifs( m_strInFile );
	if( !ifs ){
		return false;
	}
	copy( std::istream_iterator<string>(ifs), 
				std::istream_iterator<string>(), 
				std::back_inserter(m_fileList) );

	return ifs.eof();
}

// BUS_LINE_MASTER��������̌n���ԍ��iROUTE_NUMBER�j��S�Ď擾����
void CImportBusPoint::GetExistRouteNumber()
{
	// SubFields�Z�b�g
	CString subStr;
	subStr.Format( _T("%s"), bus_line_master::kRouteNumber );

	// �T�[�`
	_ICursorPtr ipCursor = m_ipBusLineMasterTable->_Search( AheInitQueryFilter( nullptr, subStr, nullptr ), VARIANT_TRUE );
	_IRowPtr    ipRow;
	while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow ){
		
		// �n���ԍ����擾���A�i�[����
		m_existRouteNumbers.insert( ipRow->GetValue( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteNumber ) ).lVal );
	}
}

// BUS_STOP_POINT��������̒◯���R�[�h�ƕW���ԍ��̃y�A��S�Ď擾����
void CImportBusPoint::GetExistStopAndPole()
{
	// SubFields�Z�b�g
	CString subStr;
	subStr.Format( _T("%s,%s"), bus_stop_point::kStopID, bus_stop_point::kPoleNo );

	// �T�[�`
	IFeatureCursorPtr ipCursor = m_ipBusStopPointFC->_Search( AheInitQueryFilter( nullptr, subStr, nullptr ), VARIANT_TRUE );
	IFeaturePtr       ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ){
		
		// �◯���R�[�h�ƕW���ԍ��擾
		long stopID = ipFeature->GetValue( m_busStopPointFM.GetFieldIndex( bus_stop_point::kStopID ) ).lVal;
		long poleNo = ipFeature->GetValue( m_busStopPointFM.GetFieldIndex( bus_stop_point::kPoleNo ) ).lVal;

		// �y�A�ŃZ�b�g�Ɋi�[
		m_existStopPole.insert( make_pair( stopID, poleNo ) );
	}
}

// �������ʃt�@�C������A�C���|�[�g�p���X�g�쐬
bool CImportBusPoint::LoadExcelData( BUSPOINTMAP& busStopPoints, BUSMASTERMAP& busLineMasters, BUSSEQVEC& busStopSeqs )
{
	try{
		using namespace Excel;

		// �������ʃt�@�C�����Ƃ̏���
		for( auto& strFile : m_fileList ){

			BUSSEQVEC fileInputs;

			cerr << "�������ʃt�@�C���I�[�v���� ... ";
			CExcelHelper cExcel;

			if( !cExcel.OpenReadOnly( (CString)(strFile.c_str()) )){

				cerr << "���s" << endl;
				m_ofsImp << "#ERROR �������ʃt�@�C���̃I�[�v���Ɏ��s�F" << strFile << endl;
				continue;
			}
			cerr << "����" << endl;

			// �t�@�C����
			CString companyName = cExcel.GetFileName().Left( cExcel.GetFileName().ReverseFind('.') );

			// �������ʃt�@�C�����̃V�[�g���Ƃ̏����i1���ڂ̃V�[�g��Num=1�j
			for( int sheetNum = 1 ; sheetNum <= cExcel.GetSheetCnt()  ; ++sheetNum ){
				_WorksheetPtr ipWorkSheet = cExcel.GetSheet( sheetNum );
				if( !ipWorkSheet ){
					continue;
				}
				CString sheetName = (LPCTSTR)(ipWorkSheet->Name);
				BUSSEQVEC sheetInputs; //!< �V�[�g�̈ꎟ�i�[�p
				set<long> routeNums;   //!< �n���ԍ��m�F�p
				
				// �������ʃt�@�C������1�s���Ƃ̏����i2�s�ڂ���f�[�^�s�FIndex=2�j
				for( int lineIdx = 2 ; ; ++lineIdx ){

					CString strStartKey;

					strStartKey.Format( _T("A%d"), lineIdx );

					// [A?�Z��]�ɉ��������Ă���΃��R�[�h������Ɣ��f(��Ȃ���͂����܂�)
					CComVariant vaData = ipWorkSheet->Range[CComVariant(strStartKey)]->Value2;
					if( VT_EMPTY == vaData.vt || vaData.vt == VT_NULL ){
						break;
					}

					// 1�s���V�[�g���̏����擾
					BUSCONTAINER busInput;
					if( !GetLineFromSheet( ipWorkSheet, lineIdx, busInput ) ){

						// �ǂݍ��ݎ��s�����s�́A�G���[�o�́��X�L�b�v
						m_ofsImp << "#ERROR " << lineIdx << " �s�ڂ̓ǂݍ��݂Ɏ��s�F" << strFile << "([" << CT2CA(sheetName) << "])" << endl;
						continue;
					}
					
					// �n���ԍ��m�F�p��set�Ɋi�[���Ă���
					routeNums.insert( _ttol( busInput.at( bus_line_master::kRouteNumber ) ) );

					// 1�s��1���R�[�h�Ƃ��āA�i�[
					sheetInputs.push_back( busInput );
				}
				
				// �n���ԍ��ɗh�ꂪ��������C���|�[�g���Ȃ��i�Ȃ��͂������ǁj
				if( routeNums.size() != 1 ){
					m_ofsImp << "#ERROR �V�[�g���Ōn���ԍ���2�ȏ㑶�݂��邽�߁A�C���|�[�g���Ȃ��F" << strFile << "([" << CT2CA(sheetName) << "])" << endl;
					continue;
				}

				// ����BUS_LINE_MASTER�ɑ��݂���n���ԍ����m�F����
				auto routeNum = *(routeNums.begin());
				if( m_existRouteNumbers.end() != m_existRouteNumbers.find( routeNum ) ){
					auto routeName =  sheetInputs.begin()->at( bus_line_master::kRouteName );
					m_ofsImp << "#INFO " << routeNum << "(" << CT2CA( routeName ) << ") �͊���BUS_LINE_MASTER�ɑ��݂��邽�߁A�C���|�[�g���Ȃ��F" << strFile << "([" << CT2CA(sheetName) << "])" << endl;
					continue;
				}

				// ���Ȃ���΃C���|�[�g�p�̃R���e�i�Ɋi�[����
				fileInputs.insert( fileInputs.end(), sheetInputs.begin(), sheetInputs.end() );


			}	// �V�[�g���Ƃ̏��������܂�

			// �t�@�C�����ƂɃC���|�[�g�p���X�g�Ɋi�[����iBUS_LINE_MASTER�Ńt�@�C����(���ƎҖ�)���K�v�Ȃ��߁j
			for( auto& rec : fileInputs ){

				// ���R�[�h���Ƃ̏���

				// �◯���R�[�h���W���ԍ��̃��j�[�N������āA�o�X��|�C���g(BUS_STOP_POINT)�ւ̃C���|�[�g���X�g�Ɋi�[
				BUSCONTAINER busStopPoint;
				if( SetBusStopPoint( rec, busStopPoint ) ){
					busStopPoints.insert( make_pair( make_pair( rec.at( bus_stop_point::kStopID ), rec.at( bus_stop_point::kPoleNo )), busStopPoint ));
				}

				// �n���ԍ����o�H�̃��j�[�N������āABUS_LINE_MASTER(�o�X�H���}�X�^)�ւ̃C���|�[�g���X�g�Ɋi�[
				BUSCONTAINER busLineMaster;
				if( SetBusLineMaster( rec, companyName, busLineMaster )){
					busLineMasters.insert( make_pair( make_pair( rec.at( bus_line_master::kRouteNumber ), rec.at( bus_line_master::kRouteDetail )), busLineMaster ));
				}
				// �S�Ă̒������ʂ�BUS_STOP_SEQ(�o�X��ԏ��Ǘ�)�ւ̃C���|�[�g���X�g�Ɋi�[
				BUSCONTAINER busStopSeq;
				if( SetBusStopSeq( rec, busStopSeq ) ){
					busStopSeqs.push_back( busStopSeq );
				}
			}
		
			cExcel.Close();

		}	// �t�@�C�����Ƃ̏��������܂�
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR �������ʃt�@�C������A�C���|�[�g�p���X�g�쐬���ɗ�O�������F" << e.what() << endl;
		return false;
	}
	return true;
}

// �������ʃt�@�C����1�s����A�R���e�i�ɓǂݍ���
bool CImportBusPoint::GetLineFromSheet( const Excel::_WorksheetPtr& ipWorkSheet, const long lineIdx, BUSCONTAINER& busInput )
{
	CString strStartKey, strEndKey;

	strStartKey.Format( _T("A%d"), lineIdx );
	strEndKey.Format( _T("S%d"), lineIdx );

	CComSafeArray<VARIANT> sa;
	long idx = 0;

	try
	{
		// 1�s�܂Ƃ߂ēǂݍ��񂾌�A��ŕ�������
		CComVariant vaRecord = ipWorkSheet->Range[CComVariant(strStartKey)][CComVariant(strEndKey)]->Value2;
		if( VT_NULL == vaRecord.vt ){
			return false;
		}
		sa.Attach(vaRecord.parray);

		for( auto& columnName : m_columnNameList ){

			// 1�Z������荞��(���W�l��DMS�̕�����̂��߁A���l��͑S�Đ����ŗǂ�
			busInput.insert( make_pair( columnName, GetCellStr( sa, ++idx )));
		}

		sa.Detach();
	}
	catch( ... )
	{
		sa.Detach();
		return false;
	}

	return true;
}

// �o�X��|�C���g�C���|�[�g�p���X�g�ւ̒l�Z�b�g
bool CImportBusPoint::SetBusStopPoint( const BUSCONTAINER& busInput, BUSCONTAINER& busStopPoint )
{
	try{
		busStopPoint.insert( make_pair( bus_stop_point::kStopID, busInput.at( bus_stop_point::kStopID )));

		// �C���v�b�g���ꂽ�������ʎ�ʂ��u2�v�̏ꍇ�F�C���|�[�g�̍ۂ́u1�v�ɕύX���邽�߁A�����Œl�ύX
		// �C���v�b�g���ꂽ�������ʎ�ʂ��u2�v�ȊO�@�F�C���v�b�g���ꂽ�������ʎ�ʂ̂܂�
		busStopPoint.insert( make_pair( bus_stop_point::kPointAttrCode, 
			busInput.at( bus_stop_point::kPointAttrCode ) == _T("2")? _T("1") : busInput.at( bus_stop_point::kPointAttrCode )));

		busStopPoint.insert( make_pair( bus_stop_point::kName, busInput.at( bus_stop_point::kName )));
		busStopPoint.insert( make_pair( bus_stop_point::kYomi, busInput.at( bus_stop_point::kYomi )));
		busStopPoint.insert( make_pair( bus_stop_point::kChangedName, busInput.at( bus_stop_point::kChangedName )));
		busStopPoint.insert( make_pair( bus_stop_point::kChangedYomi, busInput.at( bus_stop_point::kChangedYomi )));
		busStopPoint.insert( make_pair( bus_stop_point::kRemarks, busInput.at( k_name_remarks )));
		busStopPoint.insert( make_pair( bus_stop_point::kPoleNo, busInput.at( bus_stop_point::kPoleNo )));
		busStopPoint.insert( make_pair( bus_stop_point::kGettingOffF, busInput.at( bus_stop_point::kGettingOffF )));
		busStopPoint.insert( make_pair( k_dms_lat, busInput.at( k_dms_lat )));
		busStopPoint.insert( make_pair( k_dms_lon, busInput.at( k_dms_lon )));
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR �o�X��|�C���g�C���|�[�g�p���X�g�쐬���ɗ�O�������F" << e.what() << endl;
		return false;
	}
	return true;
}

// �o�X�H���}�X�^�C���|�[�g�p���X�g�ւ̒l�Z�b�g
bool CImportBusPoint::SetBusLineMaster( const BUSCONTAINER& busInput, const CString& companyName, BUSCONTAINER& busLineMaster )
{
	try{
		busLineMaster.insert( make_pair( bus_line_master::kCompany, companyName ));
		busLineMaster.insert( make_pair( bus_line_master::kRouteNumber, busInput.at( bus_line_master::kRouteNumber )));
		busLineMaster.insert( make_pair( bus_line_master::kRouteName, busInput.at( bus_line_master::kRouteName )));
		busLineMaster.insert( make_pair( bus_line_master::kRouteDetail, busInput.at( bus_line_master::kRouteDetail )));
		busLineMaster.insert( make_pair( bus_line_master::kRouteRemarks, busInput.at( bus_line_master::kRouteRemarks )));
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR �o�X�H���}�X�^�C���|�[�g�p���X�g�쐬���ɗ�O�������F" << e.what() << endl;
		return false;
	}
	return true;
}

// �o�X��ԏ��Ǘ��C���|�[�g�p���X�g�ւ̒l�Z�b�g
bool CImportBusPoint::SetBusStopSeq( const BUSCONTAINER& busInput, BUSCONTAINER& busStopSeq )
{
	try{
		busStopSeq.insert( make_pair( bus_stop_seq::kStopSeq, busInput.at( bus_stop_seq::kStopSeq )));

		// �C���v�b�g���ꂽ�������ʎ�ʂ��u2�v�̏ꍇ�F�C���|�[�g�̍ۂ́u1�v�ɕύX���邽�߁A�����Œl�ύX
		// �C���v�b�g���ꂽ�������ʎ�ʂ��u2�v�ȊO�@�F�C���|�[�g�̍ۂ́u0�v�ɕύX���邽�߁A�����Œl�ύX
		busStopSeq.insert( make_pair( bus_stop_seq::kPassF, 
			busInput.at( bus_stop_point::kPointAttrCode ) == _T("2")? _T("1") : _T("0") ));

		busStopSeq.insert( make_pair( bus_stop_seq::kDayCodes,  busInput.at( bus_stop_seq::kDayCodes )));
		busStopSeq.insert( make_pair( bus_stop_seq::kTimeCodes, busInput.at( bus_stop_seq::kTimeCodes )));
		busStopSeq.insert( make_pair( bus_stop_seq::kRemarks,   busInput.at( k_day_time_remarks )));

		// �ȉ��̓C���|�[�g���̑��e�[�u������OID�擾�p
		busStopSeq.insert( make_pair( bus_line_master::kRouteNumber, busInput.at( bus_line_master::kRouteNumber )));
		busStopSeq.insert( make_pair( bus_line_master::kRouteDetail, busInput.at( bus_line_master::kRouteDetail )));
		busStopSeq.insert( make_pair( bus_stop_point::kStopID, busInput.at( bus_stop_point::kStopID )));
		busStopSeq.insert( make_pair( bus_stop_point::kPoleNo, busInput.at( bus_stop_point::kPoleNo )));
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR �o�X��ԏ��Ǘ��C���|�[�g�p���X�g�쐬���ɗ�O�������F" << e.what() << endl;
		return false;
	}
	return true;
}

// �������ʂ���SDE�̃o�X�֘A�e�[�u���ɃC���|�[�g
bool CImportBusPoint::ImportBusPoint( const BUSPOINTMAP& busStopPoints, const BUSMASTERMAP& busLineMasters, const BUSSEQVEC& busStopSeqs )
{
	bool bError = false;

	// �ҏW�J�n
	if( ! DB::StartEdit( m_ipBusWorkspace ) )
		return false;

	// �o�X��|�C���g�t�B�[�`���N���X
	IFeatureCursorPtr ipBusStopPointCursor;
	m_ipBusStopPointFC->Insert( true, &ipBusStopPointCursor );

	for( auto& busStopPoint : busStopPoints ){

		long stopID = _ttol(busStopPoint.second.at( bus_stop_point::kStopID ));
		long poleNo = _ttol(busStopPoint.second.at( bus_stop_point::kPoleNo ));

		// ����BUS_STOP_POINT�ɑ��݂���◯���R�[�h�ƕW���ԍ��̃y�A�̏ꍇ�A�ǉ����Ȃ�
		if( m_existStopPole.end() != m_existStopPole.find( make_pair( stopID, poleNo ) ) ){
			m_ofsImp << "#INFO [�◯���R�[�h�F" << stopID << "/�W���ԍ��F" << poleNo << "] �͊���BUS_STOP_POINT�ɑ��݂��邽�߁A�C���|�[�g���Ȃ�(��ԏ��̂ݒǉ�)" << endl;
			continue;
		}
		IFeatureBufferPtr ipBusStopPointBuffer;
		m_ipBusStopPointFC->CreateFeatureBuffer( &ipBusStopPointBuffer );
		if( !SetWriteBusStopPoint( busStopPoint.second, ipBusStopPointBuffer ) ){
			bError = true;
			continue;
		}
		CComVariant vIDVariant;
		ipBusStopPointCursor->InsertFeature(ipBusStopPointBuffer, &vIDVariant);

		// ����������◯���R�[�h�ƕW���ԍ��̃y�A���X�g�ɒǉ����Ă���
		m_existStopPole.insert( make_pair( stopID, poleNo ) );

	}
	ipBusStopPointCursor->Flush();


	// �o�X�H���}�X�^�e�[�u��
	_ICursorPtr ipBusLineMasterCursor;
	m_ipBusLineMasterTable->Insert( true, &ipBusLineMasterCursor );

	for( auto& busLineMaster : busLineMasters ){

		IRowBufferPtr ipBusLineMasterBuffer;
		m_ipBusLineMasterTable->CreateRowBuffer( &ipBusLineMasterBuffer );
		if( !SetWriteBusLineMaster( busLineMaster.second, ipBusLineMasterBuffer ) ){
			bError = true;
			continue;
		}
		CComVariant vIDVariant;
		ipBusLineMasterCursor->InsertRow(ipBusLineMasterBuffer, &vIDVariant);

		// ����������n���ԍ����X�g�ɂ��ǉ����Ă���
		long routeNumber = _ttol(busLineMaster.second.at( bus_line_master::kRouteNumber ));
		m_existRouteNumbers.insert( routeNumber );
	}
	ipBusLineMasterCursor->Flush();


	// �o�X��ԏ��Ǘ��e�[�u��
	_ICursorPtr ipBusStopSeqCursor;
	m_ipBusStopSeqTable->Insert( true, &ipBusStopSeqCursor );

	for( auto& busStopSeq : busStopSeqs ){

		IRowBufferPtr ipBusStopSeqBuffer;
		m_ipBusStopSeqTable->CreateRowBuffer( &ipBusStopSeqBuffer );
		if( !SetWriteBusStopSeq( busStopSeq, ipBusStopSeqBuffer ) ){
			bError = true;
			continue;
		}
		CComVariant vIDVariant;
		ipBusStopSeqCursor->InsertRow(ipBusStopSeqBuffer, &vIDVariant);
	}
	ipBusStopSeqCursor->Flush();

	// �ҏW�ۑ����I��
	if( ! DB::StopEdit( m_ipBusWorkspace ) )
		return false;
	
	return (! bError);
}

// �o�X��|�C���g�C���|�[�g���X�g���R�[�h����A�`��E������FeatureBuffer�ɃZ�b�g
bool CImportBusPoint::SetWriteBusStopPoint( const BUSCONTAINER& busStopPoint, IFeatureBufferPtr& ipBusStopPointBuffer )
{
	try{
		// N35... , E139... �̌`��DMS�ܓx�o�x�������Ă��邽�߁Acrd_cnv��dms�^�ɕϊ����� 
		CString dmsStr = busStopPoint.at( k_dms_lon );
		int first = 0, second = 0;
		first  = dmsStr.Find( _T(".") );
		second = dmsStr.Find( _T("."), first + 1 );
		dms dmsLon;
		dmsLon.deg = _ttoi( dmsStr.Mid( 1, first - 1 ));
		dmsLon.min = _ttoi( dmsStr.Mid( first + 1, second - first - 1 ));
		dmsLon.sec = _ttof( dmsStr.Mid( second + 1 ));

		dmsStr = busStopPoint.at( k_dms_lat );
		first  = dmsStr.Find( _T(".") );
		second = dmsStr.Find( _T("."), first + 1 );
		dms dmsLat;
		dmsLat.deg = _ttoi( dmsStr.Mid( 1, first - 1 ));
		dmsLat.min = _ttoi( dmsStr.Mid( first + 1, second - first - 1 ));
		dmsLat.sec = _ttof( dmsStr.Mid( second + 1 ));


		// DMS�ܓx�o�x����10�i�ɕϊ���A�|�C���g�쐬
		WKSPoint point;

		point.X = g_crd.DMStoDEC( dmsLon );
		point.Y = g_crd.DMStoDEC( dmsLat );

		ISpatialReferencePtr ipSpRef;
		((IGeoDatasetPtr)m_ipBusStopPointFC)->get_SpatialReference( &ipSpRef );
		IPointPtr ipPoint( CLSID_Point );
		ipPoint->PutCoords( point.X, point.Y );

		// �`��Z�b�g
		if( S_OK != ((IFeatureBufferPtr)ipBusStopPointBuffer)->putref_Shape( ipPoint )){
			m_ofsImp << "#ERROR �o�X��|�C���g�փC���|�[�g�̍ہA�`��Z�b�g�Ɏ��s�i�◯���R�[�h�F�u" 
				<< CT2CA(busStopPoint.at( bus_stop_point::kStopID )) << "�v, �W���ԍ��u" << CT2CA(busStopPoint.at( bus_stop_point::kPoleNo )) << "�v�j" << endl;
			return false;
		}

		// �����Z�b�g
		if( S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( ipc_table::kOperator ), CComVariant(CComBSTR(_T("sindy"))) )
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( ipc_table::kPurpose ), CComVariant(0L) )
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( ipc_table::kModifyProgName ), CComVariant(CVersion().GetInternalName()) )
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( ipc_table::kProgModifyDate ), CComVariant(GetCurTime()) )
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kStopID ), CComVariant(_ttol(busStopPoint.at( bus_stop_point::kStopID ))))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kPointAttrCode ), CComVariant(_ttol(busStopPoint.at( bus_stop_point::kPointAttrCode ))))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kName ), CComVariant(busStopPoint.at( bus_stop_point::kName )))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kYomi ), CComVariant(busStopPoint.at( bus_stop_point::kYomi )))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kChangedName ), CComVariant(busStopPoint.at( bus_stop_point::kChangedName )))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kChangedYomi ), CComVariant(busStopPoint.at( bus_stop_point::kChangedYomi )))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kRemarks ), CComVariant(busStopPoint.at( bus_stop_point::kRemarks )))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kPoleNo ), CComVariant(_ttol(busStopPoint.at( bus_stop_point::kPoleNo ))))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kGettingOffF ), CComVariant(_ttol(busStopPoint.at( bus_stop_point::kGettingOffF ))))
			){
	
			m_ofsImp << "#ERROR �o�X��|�C���g�փC���|�[�g�̍ہA�����Z�b�g�Ɏ��s�i�◯���R�[�h�F�u" 
				<< CT2CA(busStopPoint.at( bus_stop_point::kStopID )) << "�v, �W���ԍ��u" << CT2CA(busStopPoint.at( bus_stop_point::kPoleNo )) << "�v�j" << endl;
			return false; 
		}
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR �o�X��|�C���g�̌`��E�����Z�b�g���ɗ�O�������F" << e.what() << endl;
		return false;
	}
	return true;
}

// �o�X�H���}�X�^�e�[�u���C���|�[�g���X�g���R�[�h����A������RowBuffer�ɃZ�b�g
bool CImportBusPoint::SetWriteBusLineMaster( const BUSCONTAINER& busLineMaster, IRowBufferPtr& ipBusLineMasterBuffer )
{
	try{
		// �����Z�b�g
		if( S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( ipc_table::kOperator ), CComVariant(CComBSTR(_T("sindy"))) )
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( ipc_table::kPurpose ), CComVariant(0L) )
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( ipc_table::kModifyProgName ), CComVariant(CVersion().GetInternalName()) )
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( ipc_table::kProgModifyDate ), CComVariant(GetCurTime()) )
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kCompany ), CComVariant(busLineMaster.at( bus_line_master::kCompany )))
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteNumber ), CComVariant(_ttol(busLineMaster.at( bus_line_master::kRouteNumber ))))
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteName ), CComVariant(busLineMaster.at( bus_line_master::kRouteName )))
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteDetail ), CComVariant(busLineMaster.at( bus_line_master::kRouteDetail )))
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteRemarks ), CComVariant(busLineMaster.at( bus_line_master::kRouteRemarks )))
			){
	
			m_ofsImp << "#ERROR �o�X�H���}�X�^�փC���|�[�g�̍ہA�����Z�b�g�Ɏ��s�i�n���ԍ��F�u" 
				<< CT2CA(busLineMaster.at( bus_line_master::kRouteNumber )) << "�v, �o�H�u" << CT2CA(busLineMaster.at( bus_line_master::kRouteDetail )) << "�v�j" << endl;
			return false; 
		}
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR �o�X�H���}�X�^�̑����Z�b�g���ɗ�O�������F" << e.what() << endl;
		return false;
	}
	return true;
}

// �o�X��ԏ��Ǘ��e�[�u���C���|�[�g���X�g���R�[�h����A������RowBuffer�ɃZ�b�g
bool CImportBusPoint::SetWriteBusStopSeq( const BUSCONTAINER& busStopSeq, IRowBufferPtr& ipBusStopSeqBuffer )
{
	try{
		// BUS_LINE_MASTER(�o�X�H���}�X�^)����A�n���ԍ��ƌo�H������̃��R�[�h��OID���擾
		long lineMasterOID = 0;

		IQueryFilterPtr ipFilter(CLSID_QueryFilter);
		CString strWhere;

		strWhere.Format( _T("%s = '%s' and %s = '%s'"), bus_line_master::kRouteNumber, busStopSeq.at( bus_line_master::kRouteNumber ), 
			bus_line_master::kRouteDetail, busStopSeq.at( bus_line_master::kRouteDetail ));
		ipFilter->put_WhereClause( _bstr_t(strWhere) );

		_ICursorPtr ipCursor;
		m_ipBusLineMasterTable->Search( ipFilter, false, &ipCursor );
		_IRowPtr	ipRow;
		if( ipCursor && ipCursor->NextRow(&ipRow) == S_OK ){
			ipRow->get_OID( &lineMasterOID );
		}
		else{
			m_ofsImp << "#ERROR �o�X��ԏ��Ǘ��փC���|�[�g�̍ہA�o�X�H���}�X�^����ObjectID�擾���s�i�n���ԍ��F�u" 
				<< CT2CA(busStopSeq.at( bus_line_master::kRouteNumber )) << "�v, �o�H�u" << CT2CA(busStopSeq.at( bus_line_master::kRouteDetail )) << "�v�j" << endl;
			return false; 
		}

		// BUS_STOP_POINT(�o�X��|�C���g)�A�◯���R�[�h�ƕW���ԍ�������̃��R�[�h��OID���擾
		long stopPointOID = 0;
		IQueryFilterPtr ipFilter2(CLSID_QueryFilter);
		strWhere.Format( _T("%s = '%s' and %s = '%s'"), bus_stop_point::kStopID, busStopSeq.at( bus_stop_point::kStopID ), 
			bus_stop_point::kPoleNo, busStopSeq.at( bus_stop_point::kPoleNo ));
		ipFilter2->put_WhereClause( _bstr_t(strWhere) );

		IFeatureCursorPtr ipCursor2;
		m_ipBusStopPointFC->Search( ipFilter2, false, &ipCursor2 );
		IFeaturePtr	ipFeature;
		if( ipCursor2 && ipCursor2->NextFeature(&ipFeature) == S_OK ){
			ipFeature->get_OID( &stopPointOID );
		}
		else{
			m_ofsImp << "#ERROR �o�X��ԏ��Ǘ��փC���|�[�g�̍ہA�o�X��|�C���g����ObjectID�擾���s�i�◯���R�[�h�F�u" 
				<< CT2CA(busStopSeq.at( bus_stop_point::kStopID )) << "�v, �W���ԍ��u" << CT2CA(busStopSeq.at( bus_stop_point::kPoleNo )) << "�v�j" << endl;
			return false; 
		}

		// �����Z�b�g
		if( S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( ipc_table::kOperator ), CComVariant(CComBSTR(_T("sindy"))) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( ipc_table::kPurpose ), CComVariant(0L) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( ipc_table::kModifyProgName ), CComVariant(CVersion().GetInternalName()) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( ipc_table::kProgModifyDate ), CComVariant(GetCurTime()) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kBusLineMasterID ), CComVariant(lineMasterOID) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kStopSeq ), CComVariant(_ttol(busStopSeq.at( bus_stop_seq::kStopSeq ))))
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kBusStopPointID ), CComVariant(stopPointOID) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kPassF ), CComVariant(_ttol(busStopSeq.at( bus_stop_seq::kPassF ))))
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kDayCodes ), CComVariant(busStopSeq.at( bus_stop_seq::kDayCodes )))
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kTimeCodes ), CComVariant(busStopSeq.at( bus_stop_seq::kTimeCodes )))
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kRemarks ), CComVariant(busStopSeq.at( bus_stop_seq::kRemarks )))
			){
	
			m_ofsImp << "#ERROR �o�X��ԏ��Ǘ��փC���|�[�g�̍ہA�����Z�b�g�Ɏ��s�i�n���ԍ��F�u" << CT2CA(busStopSeq.at( bus_line_master::kRouteNumber )) 
				<< "�v, �o�H�u" << CT2CA(busStopSeq.at( bus_line_master::kRouteDetail )) << "�v, �◯���R�[�h�u" << CT2CA(busStopSeq.at( bus_stop_point::kStopID )) 
				<< "�v, �W���ԍ��u" << CT2CA(busStopSeq.at( bus_stop_point::kPoleNo )) << "�v, �����ԍ��u" << CT2CA(busStopSeq.at( bus_stop_seq::kStopSeq )) << "�v�j" << endl;
			return false; 
		}
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR �o�X��ԏ��Ǘ��̑����Z�b�g���ɗ�O�������F" << e.what() << endl;
		return false;
	}
	return true;
}

// �o�X��|�C���g�̕W���|�C���g�ɑΉ����郋�[�g�T���p�|�C���g��ǉ�
bool CImportBusPoint::CreateRPoints()
{
	set<list<CString>> busPoints;

	// ���[�g�T���p�|�C���g�ǉ��Ώۂ̃o�X��|�C���gObjectID���X�g���쐬����
	if( !CreateOIDList( busPoints )){
		m_ofsARP << "#ERROR �o�X��|�C���g����A���[�g�T���p�|�C���g�ǉ��Ώۂ̎擾�Ɏ��s" << endl;
		return false;
	}

	// �ҏW�J�n
	if( ! DB::StartEdit( m_ipBusWorkspace ) )
		return false;

	// �o�X���[�g�T���p�|�C���g�쐬����
	IFeatureCursorPtr ipBusRoutePointCursor;
	m_ipBusRoutePointFC->Insert( true, &ipBusRoutePointCursor );
	
	for( auto& busPoint : busPoints ){

		IFeatureBufferPtr ipBusRoutePointBuffer;
		m_ipBusRoutePointFC->CreateFeatureBuffer( &ipBusRoutePointBuffer );

		if( !SetWriteBusRoutePoint( _ttol(*(busPoint.begin())), ipBusRoutePointBuffer ) ){
			continue;
		}
		// ���O�o��
		m_ofsARP << "BUS_STOP_POINT\t";
		for( auto& field : busPoint ){
			m_ofsARP << CT2CA(field) << "\t";
		}
		m_ofsARP << "�쐬����" << endl;

		CComVariant vIDVariant;
		ipBusRoutePointCursor->InsertFeature(ipBusRoutePointBuffer, &vIDVariant);
	}
	ipBusRoutePointCursor->Flush();


	// �ҏW�ۑ����I��
	if( ! DB::StopEdit( m_ipBusWorkspace ) )
		return false;
	
	return true;
}

// �쐬�Ώۂ̃o�X��|�C���gObjectID���X�g���쐬����
bool CImportBusPoint::CreateOIDList( set<list<CString>>& busPoints )
{
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// �o�X��|�C���g�̃o�X�|�C���g��ʃR�[�h = 1�i�◯���W���|�C���g�j
	strWhere.Format( _T("%s = '%d'"), bus_stop_point::kPointAttrCode, bus_stop_point::point_attr_code::kPole );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	IFeatureCursorPtr ipCursor;

	// �܂��̓o�X��|�C���g����쐬�Ώۂ�S�Ď擾����
	m_ipBusStopPointFC->Search(ipFilter, VARIANT_FALSE, &ipCursor);

	if( ipCursor ){
		IFeaturePtr ipFeature;

		while( ipCursor->NextFeature( &ipFeature ) == S_OK ){

			list<CString> busPoint;

			// �o�X��|�C���g�ɑΉ����郋�[�g�T���p�|�C���g���Ȃ�����Ɋm�F����
			long busPointOID;
			ipFeature->get_OID( &busPointOID );
			if( IsRPointTarget( busPointOID )){
				
				// ObjectID���i�[
				CString oID;
				oID.Format( _T("%d"), busPointOID );
				busPoint.push_back( oID );

				// ���O�o�͗p�ɍ쐬�Ώۂ̏����擾����
				if( !GetBusPointInfo( ipFeature, busPoint ) ){
					return false;
				}

				// ���[�g�T���p�|�C���g�����݂��Ȃ����̂������X�g�ɒǉ�
				busPoints.insert( busPoint );
			}
		}
	}
	else{
		return false;
	}
	return true;
}

//���[�g�T���p�|�C���g�̒ǉ��Ώۂ����ׂ�
bool CImportBusPoint::IsRPointTarget( const long busPointOID )
{
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// �o�X�⃋�[�g�T���p�|�C���g����A�o�X��|�C���gOID�������̂����݂��邩
	strWhere.Format( _T("%s = '%d'"), bus_route_point::kBusStopPointID, busPointOID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	IFeatureCursorPtr ipCursor;

	m_ipBusRoutePointFC->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	IFeaturePtr ipFeature;

	// ���������ꍇ�́A���ɑ΂ƂȂ郋�[�g�T���p�|�C���g�����݂��邽�߁A�ǉ��ΏۂƂ��Ȃ�
	if( ipCursor && ipCursor->NextFeature( &ipFeature ) == S_OK ){
		return false;
	}
	else{
		return true;
	}
}

// ���[�g�T���p�|�C���g�쐬�Ώۂ̃o�X��|�C���g�����擾����
bool CImportBusPoint::GetBusPointInfo( const IFeaturePtr& ipFeature, list<CString>& busPoint )
{
	CComVariant vaValue;
	CString     str;

	// STOP_ID
	if( S_OK != ipFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kStopID ), &vaValue ) ){
		return false;
	}
	str.Format( _T("%d"), vaValue.lVal );
	busPoint.push_back( str );

	// POLE_NO
	if( S_OK != ipFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kPoleNo ), &vaValue ) ){
		return false;
	}
	str.Format( _T("%d"), vaValue.lVal );
	busPoint.push_back( str );

	// STOP_NAME_KANJI
	if( S_OK != ipFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kName ), &vaValue ) ){
		return false;
	}
	str = (VT_BSTR == vaValue.vt)? vaValue.bstrVal : _T("");
	busPoint.push_back( str );

	// STOP_NAME_YOMI
	if( S_OK != ipFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kYomi ), &vaValue ) ){
		return false;
	}
	str = (VT_BSTR == vaValue.vt)? vaValue.bstrVal : _T("");
	busPoint.push_back( str );

	return true;
}

// �o�X��|�C���g��OID����A�o�X���[�g�T���p�|�C���g�ǉ��pFeatureBuffer�Ɍ`��E�������Z�b�g
bool CImportBusPoint::SetWriteBusRoutePoint( const long busStopPointOID, IFeatureBufferPtr& ipBusRoutePointBuffer )
{
	IFeaturePtr ipFeature;
	m_ipBusStopPointFC->GetFeature( busStopPointOID, &ipFeature );
	if(ipFeature){
		IPointPtr ipPoint = GetNearestPoint( ipFeature );
		if( ipPoint ){

			// �`��Z�b�g
			if( S_OK != ((IFeatureBufferPtr)ipBusRoutePointBuffer)->putref_Shape( ipPoint )){
				m_ofsARP << "#ERROR �o�X���[�g�T���p�|�C���g�փC���|�[�g�̍ہA�`��Z�b�g�Ɏ��s�i�o�X��|�C���gOID�F�u" << busStopPointOID << "�v�j" << endl;
				return false;
			}

			// �����Z�b�g
			if( S_OK != ipBusRoutePointBuffer->put_Value( m_busRoutePointFM.GetFieldIndex( ipc_table::kOperator ), CComVariant(CComBSTR(_T("sindy"))) )
				|| S_OK != ipBusRoutePointBuffer->put_Value( m_busRoutePointFM.GetFieldIndex( ipc_table::kPurpose ), CComVariant(0L) )
				|| S_OK != ipBusRoutePointBuffer->put_Value( m_busRoutePointFM.GetFieldIndex( ipc_table::kModifyProgName ), CComVariant(CVersion().GetInternalName()) )
				|| S_OK != ipBusRoutePointBuffer->put_Value( m_busRoutePointFM.GetFieldIndex( ipc_table::kProgModifyDate ), CComVariant(GetCurTime()) )
				|| S_OK != ipBusRoutePointBuffer->put_Value( m_busRoutePointFM.GetFieldIndex( bus_route_point::kBusStopPointID ), CComVariant(busStopPointOID) )
			){
	
				m_ofsARP << "#ERROR �o�X��|�C���g�փC���|�[�g�̍ہA�����Z�b�g�Ɏ��s�i�o�X��|�C���gOID�F�u" << busStopPointOID << "�v�j" << endl;
				return false;
			}
		}
		else{
			// �ŋߖT�����N���擾�ł��Ȃ�����
			m_ofsARP << "#ERROR �o�X��|�C���g����A�ŋߖT�����N��̍ŒZ�����̓_�̎擾���s�iOID�F�u" << busStopPointOID << "�v"  << endl;
			return false;
		}
	}
	else{
		// ��񌟍��ς݂�OID�̂��߁A���ʂ͏o�Ȃ��G���[
		m_ofsARP << "#ERROR �o�X��|�C���gOID����A���擾���s�iOID�F�u" << busStopPointOID << "�v"  << endl;
		return false;
	}
	return true;
}

// �ŋߖT�����N��̍ŒZ�����̓_�����߂�i�d�l������̂��߁A���o�XPJ�̃��W�b�N�̂܂܁j
IPointPtr CImportBusPoint::GetNearestPoint(const IFeaturePtr& ipFeature)
{
	const double METER_PARAM = 0.00001; ///< �x�����[�g���ɕϊ�(���{�ł̃U�b�N���ϊ�)
	const double BASE_DIST = 10.0 * METER_PARAM;	///< �ŋߖT�����N�T���J�n�ŏ����a
	const double DELTA_DIST = 50.0 * METER_PARAM;	///< �ŋߖT�����N�T�����̑���

	// �o�X��|�C���g�t�B�[�`������A�W�I���g���擾
	IGeometryPtr ipGeo;
	ipFeature->get_Shape( &ipGeo );
	IPointPtr ipPoint( ipGeo );

	// ��ԎQ�Ǝ擾
	ISpatialReferencePtr ipSpRef;
	((IGeoDatasetPtr)m_ipBusStopPointFC)->get_SpatialReference( &ipSpRef );

	// �o�X��|�C���g�W�I���g������AX�EY�擾
	double srcX, srcY;
	ipPoint->QueryCoords( &srcX, &srcY );

	// ���HNW�����Ԍ����̏���
	CComBSTR	bstrFieldName;
	m_ipRoadLinkFC->get_ShapeFieldName( &bstrFieldName );
	ISpatialFilterPtr ipFilter;
	double minDist = DBL_MAX;
	double tgtX = 0, tgtY = 0;
	bool found = false;

	// ��10m����50m�����₵�āA�ő�110m(10,60,110)
	for( int i = 0 ; !found && i < 2 ; ++i ){

		// ���m�ȋ����Ŗ����Ă��ǂ����߁A�ȈՂɕϊ��������[�g���p�̒l��Buffer�擾
		// ���m�ɍs���ꍇ�́AProjectedCoordinateSystem�ő��n�n�w�蓙���s���ABuffer�擾���K�v
		IGeometryPtr ipBuf;
		double radius = BASE_DIST + ( i * DELTA_DIST );
		((ITopologicalOperatorPtr)ipGeo)->Buffer( radius, &ipBuf );
		ipBuf->putref_SpatialReference(ipSpRef);

		ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), ipBuf, bstrFieldName );
		IFeatureCursorPtr ipCursor;

		// ���H�����N�����Ԍ���
		m_ipRoadLinkFC->Search( ipFilter, VARIANT_FALSE, &ipCursor );
		if( ipCursor ){
			IFeaturePtr ipTgtFeature;
			while( ipCursor->NextFeature(&ipTgtFeature) == S_OK ){
				IGeometryPtr ipLine;
				ipTgtFeature->get_Shape( &ipLine );
				IPointPtr ipNearestPoint;
				IProximityOperatorPtr ipProximity( ipLine );
				ipProximity->ReturnNearestPoint( ipPoint, esriNoExtension, &ipNearestPoint );
				if( ipNearestPoint ){
					double nearX, nearY;
					ipNearestPoint->QueryCoords( &nearX, &nearY );
					double dist = g_crd.GetDist( srcX, srcY, nearX, nearY );

					// ���ꃋ�[�v���ň�ԋ߂����̂��̗p����
					if( minDist > dist ){
						found = true;
						minDist = dist;
						tgtX = nearX;
						tgtY = nearY;
					}
				}
			}
		}else{
			break;
		}
	}

	// ������Ȃ������ꍇ
	if( !found ){
		return NULL;
	}

	IPointPtr ipTgtGeo(CLSID_Point);
	ipTgtGeo->PutCoords( tgtX, tgtY );
	ipTgtGeo->putref_SpatialReference( ipSpRef );
	ipTgtGeo->SnapToSpatialReference();

	return ipTgtGeo;
}

