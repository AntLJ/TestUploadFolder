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
#include "NodelistCreator.h"
#include "Defs.h"

// useful_headers
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>


namespace po = boost::program_options;


// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_input_db      = "input_db";
	const char * const k_name_rule_ini = "name_rule_ini";
	const char * const k_output_ini    = "output_ini";
	const char * const k_output_dir    = "output_dir";
	const char * const k_runlog        = "runlog";
	const char * const k_errlog        = "errlog";
	const char * const k_latlon        = "latlon";
	const char * const k_road_db       = "road_db";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_input_db,       uh::tvalue<uh::tstring>(&m_input_db),        "[�K�{] �ڑ���DB")
			(k_name_rule_ini,  uh::tvalue<uh::tstring>(&m_name_rule_ini),   "[�K�{] �C���X�g�����[���ݒ�t�@�C���p�X")
			(k_output_ini,     uh::tvalue<uh::tstring>(&m_output_ini),      "[�K�{] �C���X�g�o�͏��ݒ�t�@�C���p�X")
			(k_output_dir,     uh::tvalue<uh::tstring>(&m_output_dir),      "[�K�{] �o�̓t�H���_�p�X")
			(k_runlog,         uh::tvalue<uh::tstring>(&m_runlog),          "[�K�{] �������O")
			(k_errlog,         uh::tvalue<uh::tstring>(&m_errlog),          "[�K�{] �G���[���O")
			(k_latlon,         uh::tvalue<uh::tstring>(&m_latlon),          "[�C��] �ܓx�o�x�o�͎w��")
			(k_road_db,        uh::tvalue<uh::tstring>(&m_road_db),         "[�C��] �ڑ���DB(���H)")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �p�����[�^�̃`�F�b�N
		bool good = true;
		good &= CheckParam( k_input_db,      m_input_db );
		good &= CheckParam( k_name_rule_ini, m_name_rule_ini, kIsFile );
		good &= CheckParam( k_output_ini,    m_output_ini   , kIsFile );
		good &= CheckParam( k_output_dir,    m_output_dir   , kIsFolder );
		good &= CheckParam( k_runlog,        m_runlog );
		good &= CheckParam( k_errlog,        m_errlog );
		good &= CheckLatLonParam( m_latlon );

		if( ! good ){
			std::cerr << desc << std::endl;
			return false;
		}
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return false;
	}
	return true;
}


// �R�}���h���C�������̔ėp�`�F�b�N
bool Arguments::CheckParam( const char* const optName, const uh::tstring& optValue, ChkMode eChkMode /*= kNone */ ) const
{
	if( optValue.empty() ){	
		std::cerr << optName << "�I�v�V�����͕K�{�ł�" << std::endl;
		return false;
	}

	if( eChkMode ==  kIsFile ){
		if( ! uh::isFile( optValue.c_str() )){
			std::cerr << uh::toStr( optValue ) << "�̓t�@�C���ł͂���܂���" << std::endl;
			return false;
		}
	}

	if( eChkMode == kIsFolder ){
		if( ! uh::isDirectory( optValue.c_str() )){
			std::cerr << uh::toStr( optValue ) << "�̓t�H���_�ł͂���܂���" << std::endl;
			return false;
		}
	}

	return true;
}


// latlon�I�v�V�����̃`�F�b�N
bool Arguments::CheckLatLonParam( const uh::tstring& strOptValue ) const
{
	if( strOptValue.empty() )
		return true;

	if( strOptValue != latlon::DMS && strOptValue != latlon::DEG )
	{
		std::cerr << "latlon�I�v�V�����̎w�肪�s���ł� : " << uh::toStr( strOptValue ) << std::endl;
		return false;
	}
	return true;
}
