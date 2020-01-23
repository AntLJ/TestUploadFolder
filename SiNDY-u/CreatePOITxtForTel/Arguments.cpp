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

#include "stdafx.h"
#include "Arguments.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/text_util.h>
#include <TDC/useful_headers/boost_program_options.h>

namespace po = boost::program_options;
using namespace std;
using namespace uh;

CArguments::CArguments(void)
{
}


CArguments::~CArguments(void)
{
}

bool checkNecessaryOption( const tstring& argument, const char* option )
{
	if( argument.empty() )
	{
		cerr << option << "�I�v�V�����͕K�{�ł��B" << endl;
		return false;
	}
	return true;
}

bool CArguments::parse( int argc, _TCHAR* argv[] )
{
	const char * const poi_db =          "poi_db";
	const char * const addr_db =         "addr_db";
	const char * const map_db =          "map_db";
	const char * const poi_fc =          "poi_fc";
	const char * const poi_sub_fc =      "poi_sub_fc";
	const char * const admin_fc =        "admin_fc";
	const char * const building_fc =     "building_fc";
	const char * const contents_master = "contents_master";
	const char * const list =            "list";
	const char * const out =             "out";
	const char * const out_sub =         "out_sub";
	const char * const rsp_idn =         "rsp_idn";

	po::options_description desc("[�d�b���}�b�`���O�pPOI�f�[�^�o�̓c�[��]");
	try
	{
		desc.add_options()
			(poi_db,             tvalue<tstring>(&m_poiDB),        "[�K�{]POI�f�[�^SDE�ڑ���")
			(addr_db,            tvalue<tstring>(&m_addrDB),       "[�K�{]�Z���f�[�^SDE�ڑ���")
			(map_db,             tvalue<tstring>(&m_mapDB),        "[�K�{]�n�}�f�[�^SDE�ڑ���")
			(poi_fc,             tvalue<tstring>(&m_poiPoint),     "[�K�{]POI_POINT�t�B�[�`���N���X��")
			(poi_sub_fc,         tvalue<tstring>(&m_poiSubPoint),  "[�K�{]POI_SUB_POINT�t�B�[�`���N���X��")
			(admin_fc,           tvalue<tstring>(&m_cityAdmin),    "[�K�{]CITY_ADMIN�t�B�[�`���N���X��")
			(building_fc,        tvalue<tstring>(&m_building),     "[�K�{]BUILDING�t�B�[�`���N���X��")
			(contents_master,    tvalue<tstring>(&m_contents),     "[�K�{]POI_CONTENTS_MASTER�e�[�u����")
			(list,               tvalue<tstring>(&m_listPath),     "[�K�{]�o�͑Ώێ�ʕ\���X�g")
			(out,                tvalue<tstring>(&m_outFile),      "[�K�{]�o�̓t�@�C���iPOI�|�C���g�t�@�C���j��")
			(out_sub,            tvalue<tstring>(&m_outFileSub),   "[�K�{]�o�̓t�@�C���i���[�g�T���p�|�C���g�t�@�C���j��")
			(rsp_idn,            tvalue<tstring>(&m_idnCode),      "[�K�{]�Ώ�IDNCODE")
			;
	
		po::variables_map vm;
		po::store( po::parse_command_line(argc, argv, desc), vm );
		po::notify( vm );

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		good &= checkNecessaryOption( m_poiDB,     poi_db );
		good &= checkNecessaryOption( m_addrDB,    addr_db );
		good &= checkNecessaryOption( m_mapDB,     map_db );
		good &= checkNecessaryOption( m_poiPoint,  poi_fc );
		good &= checkNecessaryOption( m_poiSubPoint, poi_sub_fc );
		good &= checkNecessaryOption( m_cityAdmin, admin_fc );
		good &= checkNecessaryOption( m_building,  building_fc );
		good &= checkNecessaryOption( m_contents,  contents_master );
		good &= checkNecessaryOption( m_listPath,  list );
		good &= checkNecessaryOption( m_outFile,   out );
		good &= checkNecessaryOption( m_outFileSub,   out_sub );
		good &= checkNecessaryOption( m_idnCode,   rsp_idn );

		if( !good )
		{
			cerr << desc << endl;
			cerr << "���W���o�͂Ɏ��s���O���o�͂���܂��B" << endl;
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
