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

#include "StdAfx.h"
#include "Arguments.h"
#include "Utility.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try{
		desc.add_options()
			(arg::k_imp_type,     tvalue<tstring>(),  "[�K�{]�C���|�[�g�^�C�v")
			(arg::k_edt_addr_db,  tvalue<tstring>(),  "[�K�{]SiNDY�i�Z���E�ҏW�j�ڑ���")
			(arg::k_run_log,      tvalue<tstring>(),  "[�K�{]���s���O")
			(arg::k_err_log,      tvalue<tstring>(),  "[�K�{]�G���[���O")
			(arg::k_old_addr_db,  tvalue<tstring>(),  "[�C��]SiNDY�i���Z���E�ҏW�j�ڑ���")
			(arg::k_gs_addr_db,   tvalue<tstring>(),  "[�C��]GS�[�i�f�[�^(�Z���j�ڑ���")
			(arg::k_citylist,     tvalue<tstring>(),  "[�C��]�s�撬���R�[�h���X�g")
			(arg::k_extcode_list, tvalue<tstring>(),  "[�C��]�g���R�[�h���X�g")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		CString impType;
		good &= CheckImpTypeParam( vm, arg::k_imp_type, impType );
		good &= CheckParam( vm, arg::k_edt_addr_db, m_edtAddrDb );
		good &= CheckParam( vm, arg::k_run_log, m_runLog );
		good &= CheckParam( vm, arg::k_err_log, m_errLog );

		// ���s�^�C�v�ʂ̕K�{�p�����[�^�̃`�F�b�N
		// imp_type�I�v�V�����̃`�F�b�N�ŃG���[�̏ꍇ�͎��{���Ȃ�
		switch(m_impType)
		{
		case Utility::ExecuteType::PlaceNameRep :
			good &= CheckParam( vm, arg::k_old_addr_db, m_oldAddrDb );
			break;
		case Utility::ExecuteType::AddrPoly :
		case Utility::ExecuteType::AddrPoint :
		case Utility::ExecuteType::AddrAll :
			good &= CheckParam( vm, arg::k_gs_addr_db, m_gsAddrDb );
			good &= CheckParam( vm, arg::k_citylist, m_cityList, true );
			good &= CheckParam( vm, arg::k_extcode_list, m_extcodeList, true );
			break;
		default:
			break;
		}

		if( ! good ){
			cerr << desc << endl;
			return false;
		}
	}
	catch(const std::exception& e){
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

bool Arguments::CheckImpTypeParam(
		const po::variables_map& vm,
		const char* const optName,
		CString& optValue
)
{
	// imp_type�I�v�V�����̃`�F�b�N
	if( !CheckParam(vm, optName, optValue) )
	{
		return false;
	}

	m_impTypeStr = optValue;
	
	// imp_type�I�v�V�����l�̃`�F�b�N
	if( optValue.CompareNoCase( CString(arg_imp_type::k_placename_rep) ) == 0 )
	{
		m_impType = Utility::ExecuteType::PlaceNameRep;
	}
	else if( optValue.CompareNoCase( CString(arg_imp_type::k_addr_poly) ) == 0 )
	{
		m_impType = Utility::ExecuteType::AddrPoly;
	}
	else if( optValue.CompareNoCase( CString(arg_imp_type::k_addr_point) ) == 0 )
	{
		m_impType = Utility::ExecuteType::AddrPoint;
	}
	else if( optValue.CompareNoCase( CString(arg_imp_type::k_addr_all) ) == 0 )
	{
		m_impType = Utility::ExecuteType::AddrAll;
	}
	else
	{
		m_impType = Utility::ExecuteType::None;
		cerr << uh::toStr(optName) << "�I�v�V�����ɋK��O�̃I�v�V�����l���w�肳��Ă��܂�(" << (CT2A)optValue << ")" << endl;
		return false;
	}

	return true;
}

bool Arguments::CheckParam( 
		const po::variables_map& vm,
		const char* const optName,
		CString& optValue,
		bool bFile /* = false */
)
{
	// �I�v�V�������w�肳��Ă��邩�̃`�F�b�N
	if( vm.count(optName) == 0 )
	{
		cerr << uh::toStr(optName) << "�I�v�V�����͕K�{�ł�" << endl;
		return false;
	}

	// �I�v�V�����l���w�肳��Ă��邩�̃`�F�b�N
	optValue = vm[optName].as<uh::tstring>().c_str();
	if( optValue.IsEmpty() )
	{	
		cerr << uh::toStr(optName) << "�I�v�V�����͕K�{�ł�" << endl;
		return false;
	}

	if( bFile )
	{
		if( !PathFileExists( optValue ))
		{
			cerr << (CT2A)optValue << "�����݂��܂���" << endl;
			return false;
		}
	}
	return true;
}
