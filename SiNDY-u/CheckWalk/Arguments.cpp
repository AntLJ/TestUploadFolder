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
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_ini_file		= "ini_file";
	const char * const k_log_dir		= "log_dir";
	const char * const k_data_type		= "data_type";
	
	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_ini_file,	tvalue<tstring>(&ini_file),	"[�K�{]�ݒ�t�@�C��")
			(k_log_dir,		tvalue<tstring>(&log_dir),	"[�K�{]���O�o�̓f�B���N�g��")
			(k_data_type,	tvalue<tstring>(&data_type),"�f�[�^�^�C�v")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		good &= CheckParam( k_ini_file, ini_file, true);
		good &= CheckParam( k_log_dir, log_dir, false);

		if( ! good ){
			cerr << desc << endl;
			return false;
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

// �R�}���h���C�������̊m�F
bool Arguments::CheckParam( const char* const optName, const tstring& optValue, bool bFile /* = false */ )
{
	if( optValue.empty() ){	
		cerr << optName << "�I�v�V�����͕K�{�ł�" << endl;
		return false;
	}

	if( bFile ){
		if( ! isFile( optValue.c_str() )){
			cerr << toStr( optValue ) << "�̓t�@�C���ł͂���܂���" << endl;
			return false;
		}
	}

	return true;
}
