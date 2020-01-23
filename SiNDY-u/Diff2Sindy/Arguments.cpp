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
#include "PathFinder.h"
#include "PathUtility.h"
#include "Notifier.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/text_util.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_input					= "input";
	const char * const k_output					= "output";
	const char * const k_input_owner			= "input_owner";
	const char * const k_output_owner			= "output_owner";
	const char * const k_mesh                   = "mesh";
	const char * const k_mesh_db                = "mesh_db";
	const char * const k_mesh_table_owner       = "mesh_table_owner";
	const char * const k_mesh_layer_name        = "mesh_layer_name";
	const char * const k_merge                  = "merge";
	const char * const k_convert                = "convert";
	const char * const k_crdcnv                 = "crdcnv";
	const char * const k_clip                   = "clip";
	const char * const k_runlog					= "runlog";
	const char * const k_errlog					= "errlog";
	const char * const k_thread                 = "thread";
	const char * const k_value                  = "value";
	const char * const k_dissolve_multi         = "dissolve_multi";
	const char * const k_output_ondemand_search = "output_ondemand_search";
// TODO: split_by_mesh ���b�V���Ő؂�I�v�V����

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_input,					tvalue<tstring>(&m_input),				  "[�K�{]�C���v�b�gDB���")
			(k_output,					tvalue<tstring>(&m_output),				  "[�K�{]�A�E�g�v�b�gDB���")
			(k_input_owner,				tvalue<tstring>(&m_input_owner),		  "[�C��]�C���v�b�g���I�[�i�[��")
			(k_output_owner,			tvalue<tstring>(&m_output_owner),		  "[�C��]�A�E�g�v�b�g���I�[�i�[��")
			(k_mesh,					tvalue<tstring>(&m_mesh_list_str),		  "[�C��]���b�V��/���X�g/�t�@�C��")
			(k_mesh_db,	                tvalue<tstring>(&m_meshDB),               "[�K�{]���b�V��DB")
			(k_mesh_table_owner,	    tvalue<tstring>(&m_meshTableOwner),       "[�C��]���b�V��DB�I�[�i�[")                // TODO: �K�{�p�����[�^�̒������邱��
			(k_mesh_layer_name,	        tvalue<tstring>(&m_meshLayerName),        "[�K�{]���b�V��DB�Q�ƃ��C����")
			(k_merge,					tvalue<tstring>(&m_mapDefFile[s2sModeMerge]),   "[�K�{]�}�[�W������`�t�@�C���p�X")
			(k_convert,					tvalue<tstring>(&m_mapDefFile[s2sModeConvert]), "[�K�{]�R���o�[�g������`�t�@�C���p�X")
			(k_crdcnv,					tvalue<tstring>(&m_mapDefFile[s2sModeCrdcnv]),  "[�K�{]���n�n�ϊ�������`�t�@�C���p�X")
			(k_clip,					tvalue<tstring>(&m_mapDefFile[s2sModeClip]),    "[�K�{]�N���b�v������`�t�@�C���p�X")
			(k_runlog,					tvalue<tstring>(&m_runLog),				  "[�K�{]���s���O�t�@�C���p�X")
			(k_errlog,					tvalue<tstring>(&m_errLog),				  "[�K�{]�G���[���O�t�@�C���p�X")
			(k_thread,					tvalue<long>(&m_thread),				  "[�C��]���s�X���b�h��")
			(k_value,                   tvalue<std::vector<tstring>>(&m_values),  "[�C��]��`���Ŏg�p����l�̎w��(������)")
			(k_dissolve_multi,          tvalue<bool>(&m_dissolveMulti),           "[�C��]������Ƀ}���`�p�[�g�`����������邩�i�f�t�H���g�Ftrue�j")
			(k_output_ondemand_search,  tvalue<bool>(&m_output_ondemand_search),  "[�C��][�R���o�[�g����]�A�E�g�v�b�g���̍X�V�Ώۃt�B�[�`���͓s�x��������i�f�t�H���g�Ffalse�j")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		good &= CheckParam( k_input,  m_input);
		good &= CheckParam( k_output, m_output );

		if( ! good ){
			cerr << "�K�{�p�����[�^������܂���" << std::endl;
			cerr << desc << endl;
			return false;
		}

		// �ǂꂩ�͍Œ��ݒ肳��Ă���K�v������
		bool hasMode = ( [&]()
		{
			for( const auto& path : m_mapDefFile )
			{
				if( !path.second.empty() )
					return true;
			}
			return false;
		}() );
		if( ! hasMode ) {
			cerr << "���[�h�p�����[�^������w�肳��Ă��܂���" << std::endl;
			cerr << desc << endl;
			return false;
		}

		// ���͂�PG(FG)DB�̏ꍇ
		if( isPGDBorFGDB( m_input ) )
		{
			// PG(FG)DB�̃f�B���N�g���p�X�Q���擾
			m_paths = getPGFGDBPath(m_input);
		}

		// ���b�V���R�[�h���ݒ肳��Ă��邩
		if(!m_mesh_list_str.empty())
		{
			// ���b�V���R�[�h���p�[�X
			for( auto meshcode : text_util::ToLongList(m_mesh_list_str) )
			{
				std::stringstream ss;
				ss << meshcode;

				m_meshes.push_back( uh::toTStr(ss.str()) );
			}
		}
		else
		{
			// ���b�V���R�[�h���ȗ�����Ă��āA���͂�PG(FG)DB�̏ꍇ
			// PG(FG)DB�̃t�@�C�����̂��烁�b�V���R�[�h���擾
			if( isPGDBorFGDB(m_input) )
			{
				m_meshes = getMeshCodesFromPaths(m_paths);
			}
			else
			{
				// SDE�̏ꍇ�̓G���[??
			}
		}

		if( !m_values.empty() )
		{
			// �u:�v�ŕ�������
			uh::tstring::size_type pos = uh::tstring::npos;
			for( const auto& value : m_values )
			{
				uh::tstring::size_type pos = value.find(_T(":"));
				if( uh::tstring::npos == pos )
				{
					cerr << "value�p�����[�^�̋�؂蕶��������܂���F" << toStr(value) << std::endl;
					return false;
				}
				m_mapValue[value.substr( 0, pos )] = CComVariant(value.substr(pos+1).c_str());
			}
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
		cerr << optName<< "�I�v�V�����͕K�{�ł�" << endl;
		return false;
	}
	return true;
}

bool Arguments::isPGDBorFGDB(const uh::tstring& str)
{
	return uh::isFile(str.c_str()) || uh::isDirectory(str.c_str());
}

// �C���v�b�g/�A�E�g�v�b�g���̃I�[�i�[���⊮�e�[�u������Ԃ�
uh::tstring Arguments::getInputTableName( const uh::tstring& name ) const
{
	if( !m_input_owner.empty() )
		return m_input_owner + _T(".") + name;

	return name;
}

uh::tstring Arguments::getOutputTableName( const uh::tstring& name ) const
{
	if( !m_output_owner.empty() )
		return m_output_owner + _T(".") + name;

	return name;
}

// �I�v�V�����Ŏw�肳�ꂽ�l���擾����
CComVariant Arguments::GetValue( LPCTSTR name ) const
{
	// TODO: --value name1:value1 --value name2:value2 ... �݂����Ȋ����Ŏw��ł���悤�ɂ���
	const auto& it = m_mapValue.find(name);
	if( it != m_mapValue.end() )
		return it->second;
	else {
		std::wstringstream ss;
		ss << _T("��`�t�@�C���Ŏw�肳��Ă���l���I�v�V�����Ŏw�肳��Ă��܂���F") << name;
		notifier::critical::versatile(ss.str());
	}
	return CComVariant();
}
// ���ʎq�ɑΉ�����V�X�e���ϐ����擾
uh::tstring Arguments::GetSystemValue( SYSTEM_VALUE identifier, const uh::tstring& nullValue /*= uh::tstring()*/ ) const
{
	auto it = m_systemValue.find(identifier);
	if(m_systemValue.end()==it) return nullValue;
	return it->second;
}

std::vector<uh::tstring> Arguments::getPGFGDBPath(const uh::tstring& directoryPath)
{
	// input��PG(FG)DB�̃p�X���擾
	PathFinder finder;
	std::vector<uh::tstring> filter;
	filter.push_back(_T("gdb"));
	filter.push_back(_T("mdb"));
	std::vector<uh::tstring> vPaths( finder.getFilePathByExtension(directoryPath, filter ));
	return vPaths;
}

std::vector<uh::tstring> Arguments::getMeshCodesFromPaths(const std::vector<uh::tstring>& vPaths)
{
	std::vector<uh::tstring> ret;
	for (const auto& p : vPaths )
	{
		ret.push_back(PathUtillity::getStem(p));
	}
	return ret;
}
