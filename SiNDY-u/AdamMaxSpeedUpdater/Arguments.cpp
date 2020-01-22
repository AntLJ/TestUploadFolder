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
#include <TDC/useful_headers/text_util.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_road_db   = "road_db";
	const char * const k_adam_db   = "adam_db";
	const char * const k_mesh_list = "mesh_list";
	const char * const k_log_dir   = "log_dir";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_road_db   , tvalue<tstring>(&m_roadDB)     , "[�K�{]�ڑ���DB(���HDB)")
			(k_adam_db   , tvalue<tstring>(&m_adamDB)     , "[�K�{]�ڑ���DB(ADAM DB)")
			(k_mesh_list , tvalue<tstring>(&m_meshListStr), "[�K�{]�x�[�X���b�V���R�[�h�܂��̓x�[�X���b�V�����X�g�̃t�@�C���p�X")
			(k_log_dir   , tvalue<tstring>(&m_logDir)     , "[�K�{]���O�o�̓t�H���_�p�X")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);


		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		good &= CheckParam(k_road_db, m_roadDB);
		good &= CheckParam(k_adam_db, m_adamDB);
		good &= CheckParam(k_mesh_list, m_meshListStr);
		good &= CheckParam(k_log_dir, m_logDir);

		// ���b�V���R�[�h�ǂݍ���
		bool isMeshErr(false);
		for (long meshcode : uh::text_util::ToLongList(m_meshListStr)) {
			if (meshcode < 100000 || 1000000 <= meshcode) {
				std::cerr << "���b�V���R�[�h�̎w�肪�s���ł�(�x�[�X���b�V���̂ݎw���) : " << meshcode << std::endl;
				isMeshErr = true;
				continue;
			}
			m_meshList.push_back(meshcode);
		}
		if (isMeshErr)
			return false;

		if (m_meshList.empty()) {
			std::cerr << "mesh_list �I�v�V�����w�肪�s���ł��B" << std::endl;
			return false;
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

bool Arguments::CheckParam( const char* const optName, const tstring& optValue)
{
	if( optValue.empty() ){	
		cerr << optName << "�I�v�V�����͕K�{�ł�" << endl;
		return false;
	}

	return true;
}
