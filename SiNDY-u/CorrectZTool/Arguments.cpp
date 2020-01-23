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
bool Arguments::parse( int argc, _TCHAR* argv[] )
{
	const char * const k_db = "db";
	const char * const k_mesh_db = "mesh_db";
	const char * const k_mesh_list = "mesh_list";
	const char * const k_run_log = "run_log";
	const char * const k_err_log = "err_log";
	const char * const k_slope_max = "slope_max";

	po::options_description desc( "How to use" );

	try
	{
		desc.add_options()
			( k_db,			tvalue<tstring>( &m_db ),			"[�K�{]�����Ώ�DB" )
			( k_mesh_db,	tvalue<tstring>( &m_mesh_db ),		"[�K�{]���b�V���Q�Ɛ�DB" )
			( k_mesh_list,	tvalue<tstring>( &m_mesh_list ),	"[�K�{]���b�V�����X�g" )
			( k_run_log,	tvalue<tstring>( &m_run_log ),		"[�K�{]���s���O" )
			( k_err_log,	tvalue<tstring>( &m_err_log ),		"[�K�{]�G���[���O" )
			( k_slope_max,	tvalue<double>( &m_slope_max ),		"[�K�{]���z�̍ő勖�e�l�i%�j" )
			;

		po::variables_map vm;
		po::store( po::parse_command_line( argc, argv, desc ), vm );
		po::notify( vm );

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		good &= checkParam( k_db, m_db );
		good &= checkParam( k_mesh_db, m_mesh_db );
		good &= checkParam( k_mesh_list, m_mesh_list, true );
		good &= checkParam( k_run_log, m_run_log, true );
		good &= checkParam( k_err_log, m_err_log, true );
		good &= checkParam( k_slope_max, m_slope_max );

		if( ! good )
		{
			cerr << desc << endl;
			return false;
		}
	}
	catch( const std::exception& e )
	{
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

// �R�}���h���C�������̊m�F
bool Arguments::checkParam( const char* const optName, const tstring& optValue, bool bFile /* = false */ )
{
	if( optValue.empty() ){	
		cerr << optName << "�I�v�V�����͕K�{�ł�" << endl;
		return false;
	}

	if( bFile ){
		if( ! isFile( optValue.c_str() ) ){
			cerr << toStr( optValue ) << "�̓t�@�C���ł͂���܂���" << endl;
			return false;
		}
	}

	return true;
}

bool Arguments::checkParam( const char* const optName, double optValue )
{
	if( optValue == -1.0 ) {
		cerr << optName << "�I�v�V�����͕K�{�ł�" << endl;
		return false;
	}

	return true;
}