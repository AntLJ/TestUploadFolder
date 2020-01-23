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
	const char * const k_road_db           = "road_db";
	const char * const k_adam_db           = "adam_db";
	const char * const k_diff_db           = "diff_db";
	const char * const k_target_road_where = "target_road_where";
	const char * const k_mesh_list         = "mesh_list";
	const char * const k_log_dir           = "log_dir";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_road_db,           tvalue<tstring>(&m_RoadDB),           "[�K�{]�ڑ���DB(���HDB)")
			(k_adam_db,           tvalue<tstring>(&m_AdamDB),           "[�K�{]�ڑ���DB(ADAM DB)")
			(k_diff_db,           tvalue<tstring>(&m_DiffDB),           "[�C��]���H�����N�܂��͎Ԑ������N�̌`�󍷕�FGDB")
			(k_target_road_where, tvalue<tstring>(&m_TargetRoadWhere),  "[�C��]�R�t���Ώۓ��H�����N��������")
			(k_mesh_list,         tvalue<tstring>(&m_MeshListStr),      "[�K�{]���b�V���R�[�h�܂��̓��b�V�����X�g�t�@�C���p�X")
			(k_log_dir,           tvalue<tstring>(&m_LogDir),           "[�K�{]���O�o�̓t�H���_�p�X")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		good &= CheckParam( k_road_db,   m_RoadDB );
		good &= CheckParam( k_adam_db,   m_AdamDB );
		good &= CheckParam( k_mesh_list, m_MeshListStr );
		good &= CheckParam( k_log_dir,   m_LogDir );

		// ���b�V���R�[�h�ǂݍ���
		bool isMeshErr(false);
		for( long meshcode : uh::text_util::ToLongList( m_MeshListStr ) ){
			if( meshcode < 100000  || 1000000 <= meshcode ) {
				std::cerr << "���b�V���R�[�h�̎w�肪�s���ł�(�x�[�X���b�V���̂ݎw���) : " << std::to_string(meshcode) << std::endl;
				isMeshErr = true;
				continue;
			}
			m_MeshList.push_back( meshcode );
		}
		if( isMeshErr )
			return false;

		if ( m_MeshList.empty() ){
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

bool Arguments::CheckParam( const char* const opt_name, const uh::tstring& opt_value )
{
	if( opt_value.empty() ){	
		cerr << opt_name << "�I�v�V�����͕K�{�ł�" << endl;
		return false;
	}
	return true;
}
