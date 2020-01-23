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
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

namespace
{
/**
 * @brief  �K�{�I�v�V���������݂��邩�m�F
 * @param  optName [in] �I�v�V������
 * @param  optVal  [in] �I�v�V�����l
 * @retval true  �K�{�I�v�V�������ݒ肳��Ă���
 * @retval false �K�{�I�v�V�������ݒ肳��Ă��Ȃ�
 */
inline bool checkParam( const char* const optName, const tstring& optValue)
{
	if(optValue.empty()){
		cerr << optName << "�I�v�V�����͕K�{�ł�" << endl;
		return false;
	}
	return true;
}

} // namespace

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_db_road      = "db_road";
	const char * const k_db_road_past = "db_road_past";
	const char * const k_db_stop      = "db_stop";
	const char * const k_db_mesh      = "db_mesh";
	const char * const k_output       = "output";
	const char * const k_meshlist     = "meshlist";
	const char * const k_extract      = "extract";
	const char * const k_check        = "check";

	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_db_road,      tvalue<tstring>(&m_db_road),      "[�K�{]���HDB(�`�F�b�N���[�h�ł͌o�N�ω��O�̓��HDB)")
			(k_db_road_past, tvalue<tstring>(&m_db_road_past), "[�`�F�b�N���[�h�ŕK�{]�o�N�ω��O�̓��HDB")
			(k_db_stop,      tvalue<tstring>(&m_db_stop),      "[�K�{]�ꎞ��~DB")
			(k_db_mesh,      tvalue<tstring>(&m_db_mesh),      "[�K�{]���b�V��DB")
			(k_output,       tvalue<tstring>(&m_output),       "[�K�{]���O�t�@�C��")
			(k_meshlist,     tvalue<tstring>(&m_meshlist),     "[�K�{]���b�V�����X�g")
			(k_extract,      "���o���[�h")
			(k_check,        "�`�F�b�N���[�h")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;

		good &= checkParam(k_db_road,  m_db_road);
		good &= checkParam(k_db_stop,  m_db_stop);
		good &= checkParam(k_db_mesh,  m_db_mesh);
		good &= checkParam(k_output,   m_output);
		if( checkParam(k_meshlist, m_meshlist) )
		{
			if(! isFile(m_meshlist.c_str())){
				cerr << toStr(m_meshlist) << "�̓t�@�C���ł͂���܂���" << endl;
				good = false;
			}
		}
		else
			good = false;

		// �`�F�b�N���[�h�̏ꍇ�͌o�N�ω��O�̓��HDB���K�v
		if( m_check )
			good &= checkParam(k_db_road_past, m_db_road_past);

		// �C�Ӄp�����[�^
		if( vm.count(k_extract) != 0 ) { m_extract = true; }
		if( vm.count(k_check)   != 0 ) { m_check   = true; }

		if( !m_extract && !m_check ){
			cerr << k_extract << " or " << k_check << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		}

		if(! good)
		{
			cerr << desc << endl;
			return false;
		}

		// �g�p���Ȃ��I�v�V�������w�肳��Ă���ꍇ�͌x����\�����A
		// �I�v�V�������w�肳��Ă��Ȃ������Ƃ��āA�����𑱂���
		if( m_extract && !m_db_road_past.empty() )
		{
			cerr << k_db_road_past << "�I�u�V�����͒��o���[�h�Ŏw�肷��K�v�͂���܂���" << endl;
			cerr << k_db_road_past << "�I�v�V�����̎w��𖳌��ɂ��A�����𑱍s���܂�" << endl;
			m_db_road_past.clear();
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
