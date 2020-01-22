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
	// TODO: ���������L��
	const char * const k_bl_name_db   = "bl_name_db";
	const char * const k_building_db  = "building_db";
	const char * const k_anno_db      = "anno_db";
	const char * const k_mesh_db      = "mesh_db";
	const char * const k_output       = "output";
	const char * const k_meshlist     = "meshlist";
	const char * const k_ng_build     = "ng_build";
	const char * const k_err_word     = "err_word";
	const char * const k_warn_word    = "warn_word";
	const char * const k_bldc_ok_word = "bldc_ok_word";
	const char * const k_bldc_ng_word = "bldc_ng_word";
	const char * const k_chk_fixclass = "chk_fixclass";
	const char * const k_replace_word = "replace_word";
	const char * const k_join_anno    = "join_anno";
	const char * const k_exclude_mark = "exclude_mark";
	const char * const k_buffer_range = "buffer_range";

	po::options_description desc("How to use");

	try{
		// TODO: �����̎󂯎��ϐ��Ɛ������L��
		desc.add_options()
			(k_bl_name_db,   tvalue<tstring>(&m_strBlNameDB),    "[�K�{]�r������DB�ڑ���")
			(k_building_db,  tvalue<tstring>(&m_strBuildingDB),  "[�K�{]�s�s����DB�ڑ���")
			(k_anno_db,      tvalue<tstring>(&m_strAnnoDB),      "[�K�{]���LDB�ڑ���")
			(k_mesh_db,      tvalue<tstring>(&m_strMeshDB),      "[�K�{]���b�V��DB�ڑ���")
			(k_output,       tvalue<tstring>(&m_strOutput),      "[�K�{]�A�E�g�v�b�g")
			(k_meshlist,     tvalue<tstring>(&m_strMeshList),    "[�K�{]�s�s�n�}���b�V�����X�g")
			(k_ng_build,     tvalue<tstring>(&m_strNgBuild),     "[�K�{]NG������ʃ��X�g")
			(k_err_word,     tvalue<tstring>(&m_strErrWord),     "[�K�{]NG�����񃊃X�g(error)")
			(k_warn_word,    tvalue<tstring>(&m_strWarnWord),    "[�K�{]NG�����񃊃X�g(warning)")
			(k_bldc_ok_word, tvalue<tstring>(&m_strBldcOKWord),  "[�K�{]������ʃR�[�h��OK���[�h���X�g")
			(k_bldc_ng_word, tvalue<tstring>(&m_strBldcNGWord),  "[�K�{]������ʃR�[�h��NG���[�h���X�g")
			(k_chk_fixclass, tvalue<bool>   (&m_chkFixclass),    "������ʃR�[�h�ʃ��[�h�`�F�b�N����ʊm��R�[�h�����m�F�i0�j�̃f�[�^�ɑ΂��Ă̂ݍs�����ǂ���:default true")
			(k_replace_word, tvalue<tstring>(&m_strReplaceWord), "[�K�{]�������r�p�u�����X�g")
			(k_join_anno,    tvalue<tstring>(&m_strJoinAnno),    "[�K�{]���L��r�p���������񃊃X�g")
			(k_exclude_mark, tvalue<tstring>(&m_strExclude_mark),"�������`�F�b�N�����O�L�����X�g")
			(k_buffer_range, tvalue<double>(&m_bufferRange),     "�o�b�t�@�ŕ߂܂���͈�(m):default 50m")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �K�{�p�����[�^�̃`�F�b�N
		// TODO: �����ɉ����ĕύX
		bool good = true;
		good &= CheckParam( k_bl_name_db,   m_strBlNameDB )    &&
			    CheckParam( k_building_db,  m_strBuildingDB )  &&
			    CheckParam( k_anno_db,      m_strAnnoDB )      &&
			    CheckParam( k_mesh_db,      m_strMeshDB )      &&
			    CheckParam( k_output,       m_strOutput )      &&
			    CheckParam( k_meshlist,     m_strMeshList )    &&
			    CheckParam( k_ng_build,     m_strNgBuild )     &&
			    CheckParam( k_err_word,     m_strErrWord )     &&
				CheckParam( k_warn_word,    m_strWarnWord )    &&
				CheckParam( k_bldc_ok_word, m_strBldcOKWord )  &&
			    CheckParam( k_bldc_ng_word, m_strBldcNGWord )  &&
			    CheckParam( k_replace_word, m_strReplaceWord ) &&
			    CheckParam( k_join_anno,    m_strJoinAnno );

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
