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
#include "BusPointExcelChk.h"
#include <TDC/useful_headers/directory_util.h>
#include <WinLib/VersionInfo.h>

using namespace std;
using namespace sindy::schema::bus;

// �X�L�[�}�őΉ��ł��Ȃ��t�B�[���h�̒�`
const char * const k_line_idx = "LINEIDX";
const char * const k_name_remarks = "NAMEREMARKS";
const char * const k_day_time_remarks = "DAYTIMEREMARKS";
const char * const k_dms_lat = "DMSLAT";
const char * const k_dms_lon = "DMSLON";

// �e�폈�������s���邽�߂ɍŏ��ɌĂ΂��֐�
bool CBusPointExcelChk::execute()
{
	// �e�평���������{
	if( !Init() ){
		return false;
	}

	// �������ʃt�@�C���ǂݍ��ݐ�R���e�i�i�t�@�C�����E�V�[�g���ŊK�w�L�[�j
	BUSFILEMAP busInputs;

	// �������ʃt�@�C���ǂݍ���
	LoadExcelData( busInputs );

	// �������ʃt�@�C���̃`�F�b�N
	if( !InputCheck( busInputs ) ){
		m_ofsChk << "#ERROR �������ʃt�@�C���Ɉ�ȏ�G���[���܂܂�܂��B" << endl;
	}

	return true;
}

// �e�평����
bool CBusPointExcelChk::Init()
{
	// �������ʃt�@�C�����X�g��ǂݍ��݁Avector�Ɋi�[
	if( !LoadList() ){
		cerr << "#ERROR �������ʃt�@�C�����X�g�̓ǂݍ��݂Ɏ��s�F" << CT2CA(m_strInFile) << endl;
		return false;
	}

	// �`�F�b�N���O�t�@�C���I�[�v��
	m_ofsChk.open( m_strChkLog );
	if( !m_ofsChk ){
		cerr << "#ERROR �`�F�b�N���O�t�@�C���I�[�v���Ɏ��s�F" << CT2CA(m_strChkLog) << endl;
		return false;
	}


	// �񖼂����X�g�Ɋi�[
	SetColumnName();

	return true;
}

// �e�평����
void CBusPointExcelChk::SetColumnName()
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
	m_columnNameList.push_back( k_dms_lat );                      //!< 1/256�b�ܓx
	m_columnNameList.push_back( k_dms_lon );                      //!< 1/256�b�o�x
}

// �������ʃt�@�C�����X�g��ǂݍ��݁A�t�@�C���p�X1�s�������X�g�Ɋi�[
bool CBusPointExcelChk::LoadList()
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

// �������ʃt�@�C������A���R�[�h�����t�@�C�����E�V�[�g���ŊK�w�L�[�̃R���e�i�Ɋi�[
void CBusPointExcelChk::LoadExcelData( BUSFILEMAP& busInputs )
{
	using namespace Excel;

	// �������ʃt�@�C�����Ƃ̏���
	for( auto& strFile : m_fileList ){

		cerr << "�������ʃt�@�C���I�[�v���� ... ";
		CExcelHelper cExcel;

		if( !cExcel.OpenReadOnly( (CString)(strFile.c_str()) )){

			cerr << "���s" << endl;
			m_ofsChk << "#ERROR �������ʃt�@�C���̃I�[�v���Ɏ��s�F" << strFile << endl;
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

			// �V�[�g��
			CString sheetName = (LPCTSTR)(ipWorkSheet->GetName());
			BUSCONTVEC sheetBusInputs;

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
					m_ofsChk << "#ERROR " << lineIdx << " �s�ڂ̓ǂݍ��݂Ɏ��s�F" << strFile << endl;
					continue;
				}
				
				// ���O�o�͗p�ɏ璷�����ǁA���R�[�h�ɃV�[�g���̏����������Ă���
				busInput.insert( make_pair( _T("SHEET_NAME"), sheetName ));

				// 1�s��1���R�[�h�Ƃ��āA�i�[
				sheetBusInputs.push_back( busInput );
			}

			// �t�@�C�������L�[��1�V�[�g��1���R�[�h�Ƃ��āA�i�[
			busInputs[companyName].push_back( make_pair( sheetName, sheetBusInputs ));
	
		}	// �V�[�g���Ƃ̏��������܂�

		cExcel.Close();

	}	// �t�@�C�����Ƃ̏��������܂�
}



// �������ʃt�@�C����1�s����A�R���e�i�ɓǂݍ���
bool CBusPointExcelChk::GetLineFromSheet( const Excel::_WorksheetPtr& ipWorkSheet, long lineIdx, BUSCONTAINER& busInput )
{
	CString strStartKey, strEndKey;

	CString strLineIdx;
	strLineIdx.Format( _T("%d"), lineIdx );

	strStartKey.Format( _T("A%d"), lineIdx );
	strEndKey.Format( _T("S%d"), lineIdx );

	CComSafeArray<VARIANT> sa;
	long idx = 0;

	try
	{

		// ���O�ɏo�͗p�ɉ��s�ڂƂ��������R���e�i�Ɏ������Ă���
		busInput.insert( make_pair( k_line_idx, strLineIdx )); //!< �s��

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

// �������ʃt�@�C���`�F�b�N
bool CBusPointExcelChk::InputCheck( const BUSFILEMAP& busInputs )
{
	bool chkOK= true;

	try{
		// �t�@�C�����Ƃ̏���
		for( auto& file : busInputs ){
			// �t�@�C�����`�F�b�N�Ŏg�p���邽�߂̊i�[�p�R���e�i
			map<CCPAIR, BUSCONTVEC>	fileSameStop, fileSameCoord;

			CString fileName = file.first;
			m_ofsChk << "#�t�@�C���F" << CT2CA(fileName) << endl;

			// �V�[�g���Ƃ̏���
			for( auto& sheet : file.second ){

				// ��V�[�g�̏ꍇ�̓X�L�b�v
				if( sheet.second.empty() ){
					continue;
				}

				CString sheetName = sheet.first;
				m_ofsChk << " #�V�[�g[" << CT2CA(sheetName) << "]" << endl;

				// �P�ꃌ�R�[�h�`�F�b�N
				m_ofsChk << "  #�P�ꃌ�R�[�h�`�F�b�N" << endl;

				// ���R�[�h���Ƃ̏���
				for( auto& rec : sheet.second ){
				
					list<CString> lineErrMsgs;
					if( !CheckByLine( rec, lineErrMsgs ) ){
					
						// ���R�[�h�P�ʂ�1�ł��G���[���������ꍇ�́ANG�Ƃ���
						chkOK = false;

						// �G���[���b�Z�[�W�̕������G���[�o��
						for( auto& errMsg : lineErrMsgs ){
							m_ofsChk << "   #ERROR " << CT2CA(rec.at(k_line_idx)) << " �s�ځF" << CT2CA(errMsg) << endl;
						}
						m_ofsChk.flush();
					}

					// ����◯���ł̃t�B�[���h�s�����`�F�b�N�p�ɕʃR���e�i�Ɋi�[����i�◯���R�[�h�ƕW���ԍ����L�[�j
					fileSameStop[make_pair(rec.at(bus_stop_point::kStopID), rec.at(bus_stop_point::kPoleNo))].push_back( rec );
	
					// ����ܓx�o�x�Œ◯���R�[�hor�W���ԍ��p�ɕʃR���e�i�Ɋi�[����i�ܓx�ƌo�x���L�[�j
					fileSameCoord[make_pair(rec.at(k_dms_lat), rec.at(k_dms_lon))].push_back( rec );

				} // ���R�[�h���Ƃ̏��������܂�

				// �V�[�g���`�F�b�N
				m_ofsChk << "  #�V�[�g���`�F�b�N" << endl;

				list<CString> sheetErrMsgs;
				if( !CheckBySheet( sheet, sheetErrMsgs ) ){
	
					// �V�[�g�P�ʂ�1�ł��G���[���������ꍇ�́ANG�Ƃ���
					chkOK = false;

					// �G���[���b�Z�[�W�̕������G���[�o��
					for( auto& errMsg : sheetErrMsgs ){
						m_ofsChk << "   #ERROR " <<  CT2CA(errMsg) << endl;
					}
					m_ofsChk.flush();
				}
			} // �V�[�g���Ƃ̏��������܂�
			

			// �t�@�C�����`�F�b�N�i����◯���ł̃t�B�[���h�s�����`�F�b�N�j
			m_ofsChk << " #�t�@�C�����`�F�b�N" << endl;

			list<CString> fileErrMsgs;
			if( !CheckByFileSameStop( fileSameStop, fileErrMsgs ) ){
	
				// �t�@�C���P�ʂ�1�ł��G���[���������ꍇ�́ANG�Ƃ���
				chkOK = false;

				// �G���[���b�Z�[�W�̕������G���[�o��
				for( auto& errMsg : fileErrMsgs ){
					m_ofsChk << "   #ERROR " <<  CT2CA(errMsg) << endl;
				}
				m_ofsChk.flush();
			}

			// �t�@�C�����`�F�b�N�i����ܓx�o�x�Œ◯���R�[�hor�W���ԍ��`�F�b�N�p�j
			list<CString> fileErrMsgs2;
			if( !CheckByFileSameCoord( fileSameCoord, fileErrMsgs2 ) ){
	
				// �t�@�C���P�ʂ�1�ł��G���[���������ꍇ�́ANG�Ƃ���
				chkOK = false;

				// �G���[���b�Z�[�W�̕������G���[�o��
				for( auto& errMsg : fileErrMsgs2 ){
					m_ofsChk << "   #ERROR " <<  CT2CA(errMsg) << endl;
				}
				m_ofsChk.flush();
			}

		} // �t�@�C�����Ƃ̏��������܂�

		m_ofsChk.flush();
	}
	catch( out_of_range& e ){
		m_ofsChk << "#ERROR Excel�t�@�C���`�F�b�N���ɖ��o�^�L�[�̗�O�������F" << e.what() <<endl;
		return false;
	}

	return chkOK;
}

// �P�ꃌ�R�[�h���`�F�b�N
bool CBusPointExcelChk::CheckByLine( const BUSCONTAINER& busInput, list<CString>& errMsgs )
{
	try{
		CString errMsg;

		// -- �yNULL�s���t�B�[���h�z�������� --
		// �W���ԍ�
		if( busInput.at(bus_stop_point::kPoleNo).IsEmpty() ){
			errMsg.Format( _T("[�W���ԍ�]��NULL") );
			errMsgs.push_back( errMsg );
		}
		// �o�H
		if( busInput.at(bus_line_master::kRouteDetail).IsEmpty() ){
			errMsg.Format( _T("[�o�H]��NULL") );
			errMsgs.push_back( errMsg );
		}
		// �n���ԍ�
		if( busInput.at(bus_line_master::kRouteNumber).IsEmpty() ){
			errMsg.Format( _T("[�n���ԍ�]��NULL") );
			errMsgs.push_back( errMsg );
		}
		// �ܓx
		if( busInput.at(k_dms_lat).IsEmpty() || _T("N") != busInput.at(k_dms_lat).Left(1) ){
			errMsg.Format( _T("[�ܓx]��NULL or �t�H�[�}�b�g�ᔽ") );
			errMsgs.push_back( errMsg );
		}
		// �o�x
		if( busInput.at(k_dms_lon).IsEmpty() || _T("E") != busInput.at(k_dms_lon).Left(1) ){
			errMsg.Format( _T("[�o�x]��NULL or �t�H�[�}�b�g�ᔽ") );
			errMsgs.push_back( errMsg );
		}
		// -- �yNULL�s���t�B�[���h�z�����܂� --


		// -- �y�w��T�C�Y�I�[�o�[�z�������� --
		// �����ԍ��i5���j
		if( 5 < busInput.at(bus_stop_seq::kStopSeq).GetLength() ){
			errMsg.Format( _T("[�����ԍ�]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// �◯���R�[�h�i7���j
		if( 7 < busInput.at(bus_stop_point::kStopID).GetLength() ){
			errMsg.Format( _T("[�◯���R�[�h]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// �◯�����i200�o�C�g�j
		if( 200 < CStringA(busInput.at(bus_stop_point::kName)).GetLength() ){
			errMsg.Format( _T("[�◯����]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// �◯����݂��ȁi200�o�C�g�j
		if( 200 < CStringA(busInput.at(bus_stop_point::kYomi)).GetLength() ){
			errMsg.Format( _T("[�◯����݂���]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// �ύX��◯�����i200�o�C�g�j
		if( 200 < CStringA(busInput.at(bus_stop_point::kChangedName)).GetLength() ){
			errMsg.Format( _T("[�ύX��◯����]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// �ύX��◯����݂��ȁi200�o�C�g�j
		if( 200 < CStringA(busInput.at(bus_stop_point::kChangedYomi)).GetLength() ){
			errMsg.Format( _T("[�ύX��◯������݂���]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// ���̕ύX���l�i2000�o�C�g�j
		if( 2000 < CStringA(busInput.at(k_name_remarks)).GetLength() ){
			errMsg.Format( _T("[���̕ύX���l]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// �W���ԍ��i2���j
		if( 2 < busInput.at(bus_stop_point::kPoleNo).GetLength() ){
			errMsg.Format( _T("[�W���ԍ�]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// �n���ԍ��i7���j
		if( 7 < busInput.at(bus_line_master::kRouteNumber).GetLength() ){
			errMsg.Format( _T("[�n���ԍ�]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// �n�����i200�o�C�g�j
		if( 200 < CStringA(busInput.at(bus_line_master::kRouteName)).GetLength() ){
			errMsg.Format( _T("[�n����]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// �o�H�i200�o�C�g�j
		if( 200 < CStringA(busInput.at(bus_line_master::kRouteDetail)).GetLength() ){
			errMsg.Format( _T("[�o�H]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// �o�R�i�o�R�n�����l�j�i2000�o�C�g�j
		if( 2000 < CStringA(busInput.at(bus_line_master::kRouteRemarks)).GetLength() ){
			errMsg.Format( _T("[�o�R�i�o�R�n�����l�j]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// �j�����Ԕ��l�i2000�o�C�g�j
		if( 2000 < CStringA(busInput.at(k_day_time_remarks)).GetLength() ){
			errMsg.Format( _T("[�j�����Ԕ��l]�̃T�C�Y���I�[�o�[") );
			errMsgs.push_back( errMsg );
		}
		// -- �y�w��T�C�Y�I�[�o�[�z�����܂� --


		// -- �y���p�����ȊO�̕���������z�������� --
		boost::wregex	regOnlyNum(L"^[0-9]+$");	// ���p�����݂̂̐��K�\��
		// �����ԍ�
		if( !busInput.at(bus_stop_seq::kStopSeq).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_seq::kStopSeq), regOnlyNum )){
			errMsg.Format( _T("[�����ԍ�]�ɔ��p�����ȊO�̕���������") );
			errMsgs.push_back( errMsg );
		}
		// �◯���R�[�h
		if( !busInput.at(bus_stop_point::kStopID).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_point::kStopID), regOnlyNum )){
			errMsg.Format( _T("[�◯���R�[�h]�ɔ��p�����ȊO�̕���������") );
			errMsgs.push_back( errMsg );
		}
		// �W���ԍ�
		if( !busInput.at(bus_stop_point::kPoleNo).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_point::kPoleNo), regOnlyNum )){
			errMsg.Format( _T("[�W���ԍ�]�ɔ��p�����ȊO�̕���������") );
			errMsgs.push_back( errMsg );
		}
		// �n���ԍ�
		if( !busInput.at(bus_line_master::kRouteNumber).IsEmpty() && !IsOKRegEx( busInput.at(bus_line_master::kRouteNumber), regOnlyNum )){
			errMsg.Format( _T("[�n���ԍ�]�ɔ��p�����ȊO�̕���������") );
			errMsgs.push_back( errMsg );
		}
		// -- �y���p�����ȊO�̕���������z�����܂� --


		// -- �y�w�肳�ꂽ�����ƋL���ȊO�̕���������z�������� --
		// ��ʔԍ��i0 or 1�j
		if( !busInput.at(bus_stop_point::kPointAttrCode).IsEmpty() && ( _T("0") != busInput.at(bus_stop_point::kPointAttrCode) && _T("1") != busInput.at(bus_stop_point::kPointAttrCode) )){
			errMsg.Format( _T("[��ʔԍ�]�� 0 or 1 �ȊO�̕���������") );
			errMsgs.push_back( errMsg );
		}
		// �~�Ԑ�p�i1�j
		if( !busInput.at(bus_stop_point::kGettingOffF).IsEmpty() && _T("1") != busInput.at(bus_stop_point::kGettingOffF) ){
			errMsg.Format( _T("[�~�Ԑ�p]�� 1 �ȊO�̕���������") );
			errMsgs.push_back( errMsg );
		}
		// �o�H�j���i���p���� or ���p�Z�~�R�����j
		boost::wregex	regOnlyNumScolon(L"^[0-9;]+$");	// ���p����+���p�Z�~�R�����݂̂̐��K�\��
		if( !busInput.at(bus_stop_seq::kDayCodes).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_seq::kDayCodes), regOnlyNumScolon )){
			errMsg.Format( _T("[�o�H�j��]�ɔ��p�����Ɣ��p�Z�~�R�����ȊO�̕���������") );
			errMsgs.push_back( errMsg );
		}
		// �o�H���ԑсi���p���� or ���p�R���� or ���p�Z�~�R���� or ���p�n�C�t���j
		boost::wregex	regOnlyNumColonScolon(L"^[0-9;:\-]+$");	// ���p����+���p�Z�~�R����+���p�R����+���p�n�C�t���݂̂̐��K�\��
		if( !busInput.at(bus_stop_seq::kTimeCodes).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_seq::kTimeCodes), regOnlyNumColonScolon )){
			errMsg.Format( _T("[�o�H���ԑ�]�ɔ��p����,���p�R����,���p�Z�~�R����,���p�X�y�[�X�ȊO�̕���������") );
			errMsgs.push_back( errMsg );
		}
		// -- �y�w�肳�ꂽ�����ƋL���ȊO�̕���������z�����܂� --
	

		// -- �y���̑��P�ꃌ�R�[�h�`�F�b�N�z�������� --
		// �o�H�j���ƌo�H���ԑт̏��������قȂ�
		boost::wregex	regOnlyScolon(L";");
		if( GetRegExCnt( busInput.at(bus_stop_seq::kDayCodes), regOnlyScolon ) != GetRegExCnt( busInput.at(bus_stop_seq::kTimeCodes), regOnlyScolon )){
			errMsg.Format( _T("[�o�H�j��]��[�o�H���ԑ�]�̏��������قȂ�") );
			errMsgs.push_back( errMsg );
		}
		// �o�H���ԑтƂ��āA���蓾�Ȃ�����������i4:00�`27:59�j
		// [bug 10474] ���e�l��4:00�`27:59 �ɕύX
		boost::wregex	regOKTime(L"^(( |;)*(([4-9]|1[0-9]|2[0-7]):[0-5][0-9]-([4-9]|1[0-9]|2[0-7]):[0-5][0-9])*( |;)*)*$");	// [4:00�`27:59]-[4:00�`27:59]�� ;�i������؂�j
		if( !busInput.at(bus_stop_seq::kTimeCodes).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_seq::kTimeCodes), regOKTime )){
			errMsg.Format( _T("[�o�H���ԑ�]�Ƃ��āA���蓾�Ȃ������ƂȂ��Ă���") );
			errMsgs.push_back( errMsg );
		}
		// �o�H�j����99(���̑�)�Ȃ̂ɁA�o�H�j�����l�ɋL�ڂ��Ȃ�
		if( _T("99") == busInput.at(bus_stop_seq::kDayCodes) && busInput.at(k_day_time_remarks).IsEmpty() ){
			errMsg.Format( _T("[�o�H�j��]��99(���̑�)�Ȃ̂ɁA[�j�����Ԕ��l]�ɋL�ڂ��Ȃ�") );
			errMsgs.push_back( errMsg );
		}
		// (�ύX��◯����/�ύX��◯������݂���/���̕ύX���l�j�̂ǂ���1��������2����
		if( !(( busInput.at(bus_stop_point::kChangedName).IsEmpty() == busInput.at(bus_stop_point::kChangedYomi).IsEmpty() ) && ( busInput.at(bus_stop_point::kChangedYomi).IsEmpty() == busInput.at(k_name_remarks).IsEmpty() ))){
			errMsg.Format( _T("[�ύX��◯����],[�ύX��◯������݂���],[���̕ύX���l]�ǂ���1��������2����") );
			errMsgs.push_back( errMsg );
		}
		// -- �y���̑��P�ꃌ�R�[�h�`�F�b�N�z�����܂� --


		if( errMsgs.empty() ){
			return true;
		}
		else{
			return false;
		}
	}
	catch( out_of_range& e ){
		m_ofsChk << "#ERROR �P�̃��R�[�h�`�F�b�N���ɖ��o�^�L�[�̗�O�������F" << e.what() << endl;
		return false;
	}
}

// �V�[�g���`�F�b�N
bool CBusPointExcelChk::CheckBySheet( const BUSSHEETPAIR& sBusInputs, list<CString>& errMsgs )
{
	try{
		map<pair<CCPAIR, CString>, set<long>> seqNumMap; //!< �u����̌n���A�o�H�ŁA�����ԍ��Ɍ���(���)������v�m�F�p
		map<CCPAIRPAIR, set<CString>>         stopIDMap; //!< �u����̌n���A�o�H�A�����ԍ��ŁA�◯���R�[�h���قȂ�v�m�F�p
		map<CString, set<CString>>            routeMap;  //!< �u�n���ԍ�������Ōn�������قȂ�v�m�F�p
	
		CString errMsg;

		for( auto& chk : sBusInputs.second ){
			// �y�A(�y�A(�n���ԍ�, �n����), �o�H)���L�[�ɁA�����ԍ����i�[
			seqNumMap[make_pair( make_pair( chk.at(bus_line_master::kRouteNumber), chk.at(bus_line_master::kRouteName) ), chk.at(bus_line_master::kRouteDetail))].insert( _ttol( chk.at(bus_stop_seq::kStopSeq) ));

			// �y�A(�y�A(�n���ԍ�, �n����), �y�A(�o�H, �����ԍ�))���L�[�ɁA�◯���R�[�h���i�[
			stopIDMap[make_pair( make_pair( chk.at(bus_line_master::kRouteNumber), chk.at(bus_line_master::kRouteName) ), make_pair( chk.at(bus_line_master::kRouteDetail), chk.at(bus_stop_seq::kStopSeq) ))].insert( chk.at(bus_stop_point::kStopID) );

			// �n���ԍ����L�[�ɁA�n�������i�[
			routeMap[chk.at(bus_line_master::kRouteNumber)].insert( chk.at(bus_line_master::kRouteName) );
		}

		// ����̌n���A�o�H�ŁA�����ԍ��Ɍ���(���)���Ȃ����`�F�b�N
		for( auto& seqNums : seqNumMap ){

			// ��̏ꍇ�X�L�b�v
			if( seqNums.second.empty() ){
				continue;
			}

			for( int i = 1 ; i < *(seqNums.second.rbegin()) ; ++i ){

				// 1�`�ő�l�܂ŁA�V�[�P���X�����݂��邩�m�F����
				if( seqNums.second.end() == seqNums.second.find( i ) ){

					// ����̌n���A�o�H�ŁA�����ԍ��Ɍ���(���)������
					errMsg.Format( _T("[�n���ԍ�(%s)],[�n����(%s)],[�o�H(%s)]�ŁA[�����ԍ�]�Ɍ���(���)������i�u%d�v�j"), 
						seqNums.first.first.first, seqNums.first.first.second, seqNums.first.second, i );
					errMsgs.push_back( errMsg );
				}
			}
		}

		// ����̌n���A�o�H�A�����ԍ��ŁA�◯���R�[�h���قȂ�Ȃ����`�F�b�N
		for( auto& stopIDs : stopIDMap ){

			if( 1 < stopIDs.second.size() ){

				// 2�ȏ�i�[����Ă���ꍇ�́A�G���[
				for( auto& stopID : stopIDs.second ){

					// ����̌n���A�o�H�A�����ԍ��ŁA�◯���R�[�h���قȂ�
					errMsg.Format( _T("[�n���ԍ�(%s)],[�n����(%s)],[�o�H(%s)],[�����ԍ�(%s)]������ŁA[�◯���R�[�h]���قȂ�i�u%s�v�j"), 
						stopIDs.first.first.first, stopIDs.first.first.second, stopIDs.first.second.first, stopIDs.first.second.second, stopID );
					errMsgs.push_back( errMsg );
				}
			}
		}

		// �n���ԍ�������Ōn�������قȂ�Ȃ����`�F�b�N
		for( auto& routes : routeMap ){
	
			if( 1 < routes.second.size() ){

				// 2�ȏ�i�[����Ă���ꍇ�́A�G���[
				for( auto& route : routes.second ){

					// �n���ԍ�������Ōn�������قȂ�
					errMsg.Format( _T("[�n���ԍ�(%s)]������ŁA[�n����]���قȂ�i�u%s�v�j"), 
						routes.first, route );
					errMsgs.push_back( errMsg );
				}
			}
		}
	}
	catch( out_of_range& e ){
		m_ofsChk << "#ERROR �V�[�g���`�F�b�N���ɖ��o�^�L�[�̗�O�������F" << e.what() << endl;
		return false;
	}

	if( errMsgs.empty() ){
		return true;
	}
	else{
		return false;
	}
}

// �t�@�C�����`�F�b�N�i����◯���ł̃t�B�[���h�s�����j
bool CBusPointExcelChk::CheckByFileSameStop( const map<CCPAIR, BUSCONTVEC>& sBusInputs, list<CString>& errMsgs )
{
	try{
		CString errMsg;
	
		for( auto& sameStop : sBusInputs ){
		
			CString strStopKanji    = sameStop.second.begin()->at(bus_stop_point::kName);
			CString strStopYomi     = sameStop.second.begin()->at(bus_stop_point::kYomi);
			CString strChgStopKanji = sameStop.second.begin()->at(bus_stop_point::kChangedName);
			CString strChgStopYomi  = sameStop.second.begin()->at(bus_stop_point::kChangedYomi);
			CString strNameRemarks  = sameStop.second.begin()->at(k_name_remarks);
			CString strGetOffOnly   = sameStop.second.begin()->at(bus_stop_point::kGettingOffF);
			CString strLat          = sameStop.second.begin()->at(k_dms_lat);
			CString strLon          = sameStop.second.begin()->at(k_dms_lon);
			CString strSheetName    = sameStop.second.begin()->at(_T("SHEET_NAME"));

			for( auto& chk : sameStop.second ){

				// �◯���R�[�h�ƕW���ԍ��������◯���ŁA
				// �◯����/�◯������݂���/�ύX��◯����/�ύX��◯������݂���/���̕ύX���l/�~�Ԑ�p/�ܓx/�o�x
				// �̂����ꂩ���قȂ�

				// �◯����
				if( strStopKanji != chk.at(bus_stop_point::kName) ){
					errMsg.Format( _T("[�◯���R�[�h(%s)],[�W���ԍ�(%s)]������ŁA[�◯����]���قȂ�i�u%s�v([%s]�V�[�g) �Ɓu%s�v([%s]�V�[�g) �j"), 
						sameStop.first.first, sameStop.first.second, strStopKanji, strSheetName,
						chk.at(bus_stop_point::kName), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// �◯������݂���
				if( strStopYomi != chk.at(bus_stop_point::kYomi) ){
					errMsg.Format( _T("[�◯���R�[�h(%s)],[�W���ԍ�(%s)]������ŁA[�◯������݂���]���قȂ�i�u%s�v([%s]�V�[�g) �Ɓu%s�v([%s]�V�[�g) �j"), 
						sameStop.first.first, sameStop.first.second, strStopYomi, strSheetName,
						chk.at(bus_stop_point::kYomi), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// �ύX��◯����
				if( strChgStopKanji != chk.at(bus_stop_point::kChangedName) ){
					errMsg.Format( _T("[�◯���R�[�h(%s)],[�W���ԍ�(%s)]������ŁA[�ύX��◯����]���قȂ�i�u%s�v([%s]�V�[�g) �Ɓu%s�v([%s]�V�[�g) �j"), 
						sameStop.first.first, sameStop.first.second, strChgStopKanji, strSheetName,
						chk.at(bus_stop_point::kChangedName), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// �ύX��◯������݂���
				if( strChgStopYomi != chk.at(bus_stop_point::kChangedYomi) ){
					errMsg.Format( _T("[�◯���R�[�h(%s)],[�W���ԍ�(%s)]������ŁA[�ύX��◯������݂���]���قȂ�i�u%s�v([%s]�V�[�g) �Ɓu%s�v([%s]�V�[�g) �j"), 
						sameStop.first.first, sameStop.first.second, strChgStopYomi, strSheetName,
						chk.at(bus_stop_point::kChangedYomi), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// ���̕ύX���l
				if( strNameRemarks != chk.at(k_name_remarks) ){
					errMsg.Format( _T("[�◯���R�[�h(%s)],[�W���ԍ�(%s)]������ŁA[���̕ύX���l]���قȂ�i�u%s�v([%s]�V�[�g) �Ɓu%s�v([%s]�V�[�g) �j"), 
						sameStop.first.first, sameStop.first.second, strNameRemarks, strSheetName,
						chk.at(k_name_remarks), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// �~�Ԑ�p
				if( strGetOffOnly != chk.at(bus_stop_point::kGettingOffF) ){
					errMsg.Format( _T("[�◯���R�[�h(%s)],[�W���ԍ�(%s)]������ŁA[�~�Ԑ�p]���قȂ�i�u%s�v([%s]�V�[�g) �Ɓu%s�v([%s]�V�[�g) �j"), 
						sameStop.first.first, sameStop.first.second, strGetOffOnly, strSheetName,
						chk.at(bus_stop_point::kGettingOffF), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// �ܓx
				if( strLat != chk.at(k_dms_lat) ){
					errMsg.Format( _T("[�◯���R�[�h(%s)],[�W���ԍ�(%s)]������ŁA[�ܓx]���قȂ�i�u%s�v([%s]�V�[�g) �Ɓu%s�v([%s]�V�[�g) �j"), 
						sameStop.first.first, sameStop.first.second, strLat, strSheetName,
						chk.at(k_dms_lat), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// �o�x
				if( strLon != chk.at(k_dms_lon) ){
					errMsg.Format( _T("[�◯���R�[�h(%s)],[�W���ԍ�(%s)]������ŁA[�o�x]���قȂ�i�u%s�v([%s]�V�[�g) �Ɓu%s�v([%s]�V�[�g) �j"), 
						sameStop.first.first, sameStop.first.second, strLon, strSheetName,
						chk.at(k_dms_lon), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}

				strStopKanji    = chk.at(bus_stop_point::kName);
				strStopYomi     = chk.at(bus_stop_point::kYomi);
				strChgStopKanji = chk.at(bus_stop_point::kChangedName);
				strChgStopYomi  = chk.at(bus_stop_point::kChangedYomi);
				strNameRemarks  = chk.at(k_name_remarks);
				strGetOffOnly   = chk.at(bus_stop_point::kGettingOffF);
				strLat          = chk.at(k_dms_lat);
				strLon          = chk.at(k_dms_lon);
				strSheetName    = chk.at(_T("SHEET_NAME"));
			}
		}
	}
	catch( out_of_range& e ){
		m_ofsChk << "#ERROR �t�@�C�����`�F�b�N���ɖ��o�^�L�[�̗�O�������F" << e.what() << endl;
		return false;
	}

	if( errMsgs.empty() ){
		return true;
	}
	else{
		return false;
	}
}

// �t�@�C�����`�F�b�N�i����ܓx�o�x�Œ◯���R�[�hor�W���ԍ��m�F�j
bool CBusPointExcelChk::CheckByFileSameCoord( const map<CCPAIR, BUSCONTVEC>& sBusInputs, list<CString>& errMsgs )
{
	try{
		CString errMsg;
	
		for( auto& sameLL : sBusInputs ){

			CString strStopID    = sameLL.second.begin()->at(bus_stop_point::kStopID);
			CString strPoleNum   = sameLL.second.begin()->at(bus_stop_point::kPoleNo);
			CString strSheetName = sameLL.second.begin()->at(_T("SHEET_NAME"));

			for( auto& chk : sameLL.second ){

				// �ܓx�o�x�������◯���ŁA(�◯���R�[�h/�W���ԍ�)�̂����ꂩ���قȂ�

				// �◯���R�[�h
				if( strStopID != chk.at(bus_stop_point::kStopID) ){
					errMsg.Format( _T("[�ܓx(%s)],[�o�x(%s)]������ŁA[�◯���R�[�h]���قȂ�i�u%s�v([%s]�V�[�g) �Ɓu%s�v([%s]�V�[�g) �j"), 
						sameLL.first.first, sameLL.first.second, strStopID, strSheetName,
						chk.at(bus_stop_point::kStopID), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// �W���ԍ�
				if( strPoleNum != chk.at(bus_stop_point::kPoleNo) ){
					errMsg.Format( _T("[�ܓx(%s)],[�o�x(%s)]������ŁA[�W���ԍ�]���قȂ�i�u%s�v([%s]�V�[�g) �Ɓu%s�v([%s]�V�[�g) �j"), 
						sameLL.first.first, sameLL.first.second, strPoleNum, strSheetName,
						chk.at(bus_stop_point::kPoleNo), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}

				strStopID    = chk.at(bus_stop_point::kStopID);
				strPoleNum   = chk.at(bus_stop_point::kPoleNo);
				strSheetName = chk.at(_T("SHEET_NAME"));
			}
		}
	}
	catch( out_of_range& e ){
		m_ofsChk << "#ERROR �t�@�C�����`�F�b�N���ɖ��o�^�L�[�̗�O�������F" << e.what() << endl;
		return false;
	}

	if( errMsgs.empty() ){
		return true;
	}
	else{
		return false;
	}
}
