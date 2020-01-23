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

namespace
{
	const char * const k_hyphen = "--";
	const char * const k_cityDb = "city_db";
	const char * const k_jusyoUser = "jusyo_user";
	const char * const k_jusyoDb = "jusyo_db";
	const char * const k_outDir = "out_dir";
	const char * const k_shpName = "shape_name";
	const char * const k_tblName = "table_name";
	const char * const k_runLog = "run_log";
	const char * const k_errLog = "err_log";

	const char * const k_shp = "shp";
	const char * const k_sde = "sde";
	const char * const k_outDb = "out_db";
};

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_shp, "[�I��]shpae�o��")
			(k_sde, "[�I��]sde�o��")
			(k_cityDb, tvalue<tstring>(&m_cityDb), "[�K�{]�s�s�n�}�s���E�f�[�^�ڑ���")
			(k_jusyoUser, tvalue<tstring>(&m_jusyoUser), "[�K�{]�Z���}�X�^���[�U��")
			(k_jusyoDb, tvalue<tstring>(&m_jusyoDb), "[�K�{]�Z��DB��")
			(k_outDir, tvalue<tstring>(&m_outputDir), "[SHP�K�{]�X�֔ԍ��|���S���o�͐�")
			(k_shpName, tvalue<tstring>(&m_shapeName), "[SHP�K�{]�X�֔ԍ��|���S���t�@�C����")
			(k_tblName, tvalue<tstring>(&m_tableName), "[SHP�K�{]�X�֔ԍ��f�[�^�e�[�u����")
			(k_outDb, tvalue<tstring>(&m_outputDb), "[SDE�K�{]�X�֔ԍ��|���S���o�͐�")
			(k_runLog, tvalue<tstring>(&m_runLog), "[�K�{]���s���O�o�͐�")
			(k_errLog, tvalue<tstring>(&m_errLog), "[�K�{]�G���[���O�o�͐�")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
		
		if (0 < vm.count(k_shp))
		{
			if (0 < vm.count(k_sde))
			{
				cerr << "SDE���[�h�ASHP���[�h�������w�肳��Ă��܂��B" << endl;
				cerr << desc << endl;
				return false;
			}
			m_executeMode = ExecuteMode::shp;
		}
		else if (0 < vm.count(k_sde))
		{
			m_executeMode = ExecuteMode::sde;
		}

		if (ExecuteMode::none == m_executeMode)
		{
			cerr << "SDE���[�h�ASHP���[�h�ǂ�����w�肳��Ă��܂���B" << endl;
			cerr << desc << endl;
			return false;
		}

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		good &= CheckParam(k_cityDb, m_cityDb);
		good &= CheckParam(k_jusyoUser, m_jusyoUser);
		good &= CheckParam(k_jusyoDb, m_jusyoDb);
		good &= CheckParam(k_runLog, m_runLog);
		good &= CheckParam(k_errLog, m_errLog);

		if( !good )
		{
			cerr << desc << endl;
			return false;
		}

		// shp�Ȃ�K�{
		if (ExecuteMode::shp == m_executeMode)
		{
			good &= CheckParam(k_shpName, m_shapeName);
			good &= CheckParam(k_tblName, m_tableName);
			good &= CheckParam(k_outDir, m_outputDir);
			if (!good)
			{
				cerr << desc << endl;
				return false;
			}

			// shp�o�͐�`�F�b�N
			if( !uh::isDirectory(m_outputDir.c_str()) )
			{
				cerr << m_outputDir << " : ���݂��Ȃ��f�B���N�g���ł�" << endl;
				return false;
			}

			tstring shpPath = m_outputDir + _T("\\") + m_shapeName + _T(".shp");
			if( uh::isFile(shpPath.c_str()) )
			{
				cerr << "���� " << m_shapeName << ".shp �����݂��܂�" << endl;
				return false;
			}

			tstring tblPath = m_outputDir + _T("\\") + m_tableName + _T(".dbf");
			if( uh::isFile(tblPath.c_str()) )
			{
				cerr << "���� " << m_tableName << ".dbf �����݂��܂�" << endl;
				return false;
			}
		}

		// sde�Ȃ�K�{
		if (ExecuteMode::sde == m_executeMode)
		{
			if (!CheckParam(k_outDb, m_outputDb))
			{
				cerr << desc << endl;
				return false;
			}
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

bool Arguments::CheckParam(const char* const optName, const tstring& optValue)
{
	if( optValue.empty() )
	{
		cerr << optName << "�I�v�V�����͕K�{�ł�" << endl;
		return false;
	}
	return true;
}


CString Arguments::GetAllOptionInfo()
{
	stringstream ss;
	if (ExecuteMode::shp == m_executeMode)
		ss << "���s���[�h : shp" << endl;
	else if (ExecuteMode::sde == m_executeMode)
		ss << "���s���[�h : sde" << endl;

	ss << k_hyphen << k_cityDb << " : " << m_cityDb << endl;
	ss << k_hyphen << k_jusyoUser << " : " << m_jusyoUser << endl;
	ss << k_hyphen << k_jusyoDb << " : " << m_jusyoDb << endl;
	ss << k_hyphen << k_outDir << " : " << m_outputDir << endl;
	ss << k_hyphen << k_outDb << " : " << m_outputDb << endl;
	ss << k_hyphen << k_shpName << " : " << m_shapeName << endl;
	ss << k_hyphen << k_tblName << " : " << m_tableName << endl;
	ss << k_hyphen << k_runLog << " : " << m_runLog << endl;
	ss << k_hyphen << k_errLog << " : " << m_errLog << endl;

	return ss.str().c_str();
}
