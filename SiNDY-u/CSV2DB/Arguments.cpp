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
#include <iostream>
#include <TDC/useful_headers/directory_util.h>

using namespace std;
using namespace uh;

namespace arg{
	const string kInput       = "input";
	const string kTargetDB    = "target_db";
	const string kTargetTbl   = "target_tbl";
	const string kRunLog      = "run_log";  
	const string kErrLog      = "err_log";
	const string kForceOutput = "force_output";
	const string kNoUpdateHistry = "no_update_sindy_history";
	const string kTestMode    = "test_mode";
} // namespace arg

CArgInfo::CArgInfo( const std::string& arg_, const std::string& desc_, bool required_/*= false*/, bool flag_/*= false*/, const std::string& default_ /*=""*/ ):
	arg( arg_ ),
	desc( desc_ ),
	required( required_ ),
	flag( flag_ ),
	def_val( default_)
{
}

void CArgInfos::add(const CArgInfo& arg) 
{
	m_argInfos.push_back(arg); 
}

bool CArgInfos::parse( int argc, _TCHAR* argv[] )
{
	namespace po = boost::program_options;

	po::options_description desc("How to use");
	try{
		for( const auto& arg : m_argInfos )
		{
			if( arg.flag )
			{
				desc.add_options()
					( arg.arg.c_str(), arg.desc.c_str());
			}
			else
			{
				if( arg.def_val.empty() )
				{
					desc.add_options()
						( arg.arg.c_str(), po::value<string>(), arg.desc.c_str());
				}
				else
				{
					desc.add_options()
						( arg.arg.c_str(), po::value<string>()->default_value( arg.def_val ), arg.desc.c_str());
				}
			}
		}

		po::store(po::parse_command_line(argc, argv, desc), m_args);
		po::notify(m_args);
	}
	catch(const std::exception& e)
	{
		// ��`����Ă��Ȃ��I�v�V�����i--xxx�j���w�肳�ꂽ
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}

	return true;
}

CString CArgInfos::getValue( const std::string& arg ) const
{
	CString value;

	if( !hasOption(arg) )
		return value;

	try{
		value = m_args[arg].as<string>().c_str();
	}
	catch(const std::exception& e){
		// m_args�Ɋi�[����Ă��Ȃ�
		cerr << "bad argument: " << arg << endl;
		throw;
	}

	return value;
}

bool CArgInfos::hasOption( const std::string& arg ) const
{
	return m_args.count( arg ) > 0;
}

bool CArgInfos::chekArg() const 
{
	bool chk = true;
	// [�K�{]�`�F�b�N
	for( const auto& arg : m_argInfos )
	{
		if( arg.required && m_args.count(arg.arg.c_str()) == 0 )
		{
			cerr << "required option: --" << arg.arg << endl;
			chk = false;
		}
	}
	return chk;
}

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	m_args.add( CArgInfo( arg::kInput, "[�K�{]���̓t�@�C���icsv�j", true, false, "C:\\tmp\\test4_1_.csv" ) );
	//m_args.add( CArgInfo( arg::kInput, "[�K�{]���̓t�@�C���icsv�j", true, false, "C:\\tmp\\test_create.csv" ) );
	//m_args.add( CArgInfo( arg::kTargetDB, "[�K�{]�X�V��DB", true, false, "C:\\PGDB\\BldName.gdb" ) );
	m_args.add( CArgInfo( arg::kTargetDB, "[�K�{]�X�V��DB", true, false, "PRODENG1/PRODENG1/SDE.EDT_ETCTEST/5151/coral2" ) );
	//m_args.add( CArgInfo( arg::kTargetDB, "[�K�{]�X�V��DB", true, false, "SINDYTEST/SINDYTEST/ETC_SINDYTEST_BLDAA_TEST2/5151/coral2" ) );
	//m_args.add( CArgInfo( arg::kTargetTbl, "[�K�{]�X�V�Ώۃe�[�u��", true, false, "BUILDINGNAME_SRC_POINT" ) );
	m_args.add( CArgInfo( arg::kTargetTbl, "[�K�{]�X�V�Ώۃe�[�u��", true, false, "BUILDINGNAME_SRC_POINT" ) );
	//m_args.add( CArgInfo( arg::kTargetTbl, "[�K�{]�X�V�Ώۃe�[�u��", true, false, "ROAD_CODE_LIST" ) );
	m_args.add( CArgInfo( arg::kRunLog, "[�K�{]���s���O�t�@�C��", true, false, "C:\\tmp\\run.log" ) );
	m_args.add( CArgInfo( arg::kErrLog, "[�K�{]�G���[���O�t�@�C��", true, false, "C:\\tmp\\err.log" ) );
	m_args.add( CArgInfo( arg::kForceOutput, "[�C��]�����o�̓t�B�[���h", false, false ) );
	// TODO: �f�o�b�O�Ńt���O�I�v�V�������w�肷��ꍇ�̓R�}���h�����Ŏw�肷�邱��
	m_args.add( CArgInfo( arg::kNoUpdateHistry, "[�C��]SiNDY�̗����t�B�[���h���X�V���Ȃ�", false, true ) );
	m_args.add( CArgInfo( arg::kTestMode, "[�C��]�e�X�g���[�h", false, true ) );
#else
	m_args.add( CArgInfo( arg::kInput, "[�K�{]���̓t�@�C���icsv�j", true, false ) );
	m_args.add( CArgInfo( arg::kTargetDB, "[�K�{]�X�V��DB", true, false ) );
	m_args.add( CArgInfo( arg::kTargetTbl, "[�K�{]�X�V�Ώۃe�[�u��", true, false ) );
	m_args.add( CArgInfo( arg::kRunLog, "[�K�{]���s���O�t�@�C��", true, false ) );
	m_args.add( CArgInfo( arg::kErrLog, "[�K�{]�G���[���O�t�@�C��", true, false ) );
	m_args.add( CArgInfo( arg::kForceOutput, "[�C��]�����o�̓t�B�[���h", false, false ) );
	m_args.add( CArgInfo( arg::kNoUpdateHistry, "[�C��]SiNDY�̗����t�B�[���h���X�V���Ȃ�", false, true ) );
	m_args.add( CArgInfo( arg::kTestMode, "[�C��]�e�X�g���[�h", false, true ) );
#endif

	if( !m_args.parse( argc, argv ) )
		return false;
	
	if( !m_args.chekArg() )
		return false;

	return true;
}


CString Arguments::description() const
{
	CString desc = _T("# Options:\n");
	for( const auto& arg : m_args )
	{
		// �w�肳��Ă��Ȃ���Ώo���Ȃ�
		if( !hasOption( arg.arg ) )
			continue;

		desc.AppendFormat( _T("#  --%s: %s\n"), CString(arg.arg.c_str()), getValue( arg.arg ) );
	}
	desc += _T("\n");

	return desc;
}
