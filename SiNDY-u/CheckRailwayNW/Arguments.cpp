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
	const char * const k_rail_nw_db         = "rail_nw_db";
	const char * const k_rail_poi_db        = "rail_poi_db";
	const char * const k_mesh_db            = "mesh_db";
	const char * const k_station_db         = "station_db";
	const char * const k_output             = "output";
//	const char * const k_output_mesh        = "output_mesh";
//	const char * const k_output_query       = "output_query";
	const char * const k_meshlist           = "meshlist";
	const char * const k_dist_threshold     = "dist_threshold";
	const char * const k_neighbor_threshold = "neighbor_threshold";
//	const char * const k_mesh_mode          = "mesh";
//	const char * const k_query_mode         = "query";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_rail_nw_db,         tvalue<tstring>(&m_strRailNwDB),        "[�K�{]�S��NW DB�ڑ���")
			(k_rail_poi_db,        tvalue<tstring>(&m_strRailPoiDB),       "[�K�{]�S��POI DB�ڑ���")
			(k_mesh_db,            tvalue<tstring>(&m_strMeshDB),          "[�K�{]���b�V��DB�ڑ���")
			(k_station_db,         tvalue<tstring>(&m_strStationDB),       "[�K�{]���k�w��DB�ڑ���")
			(k_output,             tvalue<tstring>(&m_strLog),             "[�K�{]���O�t�@�C��")
//			(k_output_mesh,        tvalue<tstring>(&m_strMeshLog),         "[���b�V�����[�h�K�{]���b�V�����[�h���O�t�@�C��")
			(k_meshlist,           tvalue<tstring>(&m_strMeshList),        "[�K�{]���b�V�����X�g")
			(k_dist_threshold,     tvalue<tstring>(&m_dist_threshold),     "[�K�{]����臒l(m)")
			(k_neighbor_threshold, tvalue<tstring>(&m_neighbor_threshold), "[�K�{]�ߖT臒l(m)")
//			(k_output_query,       tvalue<tstring>(&m_strQueryLog),        "[�N�G�����[�h�K�{]�N�G�����[�h���O�t�@�C��")
//			(k_mesh_mode,                                                  "[�C��]���b�V�����[�h")
//			(k_query_mode,                                                 "[�C��]�N�G�����[�h")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		bool good = true;
		
//		// ���[�h�Z�b�g
//		good != SetMode( vm, k_mesh_mode, k_query_mode);	

		// �p�����[�^�̃`�F�b�N
		good &= CheckParam( k_rail_nw_db,  m_strRailNwDB );
		good &= CheckParam( k_rail_poi_db, m_strRailPoiDB );
		good &= CheckParam( k_mesh_db,     m_strMeshDB );
		good &= CheckParam( k_station_db,  m_strStationDB );
		good &= CheckParam( k_output,      m_strLog );
		good &= CheckParam( k_meshlist,    m_strMeshList );
		good &= CheckParam( k_dist_threshold,     m_dist_threshold );
		good &= CheckParam( k_neighbor_threshold, m_neighbor_threshold );

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

//bool Arguments::SetMode( const po::variables_map& vm, const char* const meshMode, const char* const queryMode )
//{
//	if( vm.count( meshMode ) != 0 )
//		m_meshMode  = true;
//
//	if( vm.count( queryMode ) != 0 )
//		m_queryMode = true;
//
//	if( ! m_meshMode && ! m_queryMode ){
//		cerr << "[" << meshMode << "/" << queryMode << "]�I�v�V�����͂����ꂩ�K�{�ł�" << endl;
//		return false;
//	}
//	return true;
//}
