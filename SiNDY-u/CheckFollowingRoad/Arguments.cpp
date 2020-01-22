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
#include "TDC/useful_headers/text_util.h"
#include "TDC/sindylib_base/sindylib_base_global.h"
#include <regex>

using namespace std;
using namespace uh;
using namespace meshtype;
namespace po = boost::program_options;

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const string k_base = "base";
	const string k_comp = "comp";
	const string k_mesh = "mesh";
	const string k_date = "date";
	const string k_runlog = "runlog";
	const string k_errlog = "errlog";

	po::options_description desc("How to use");

	try
	{
		tstring strModDate;

		desc.add_options()
			(k_base.c_str(),		tvalue<tstring>(&m_BaseDB),		"[�K�{]��r���o�[�W������DB")
			(k_comp.c_str(),		tvalue<tstring>(&m_CompDB),		"[�K�{]��r��o�[�W������DB")
			(k_mesh.c_str(),		tvalue<tstring>(&m_MeshList),	"[�K�{]���b�V�����X�g")
			(k_date.c_str(),		tvalue<tstring>(&strModDate),	"[�C��]�ŏI�X�V��")
			(k_runlog.c_str(),		tvalue<tstring>(&m_Runlog),		"[�K�{]���s���O")
			(k_errlog.c_str(),		tvalue<tstring>(&m_Errlog),		"[�K�{]�G���[���O")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		if(m_BaseDB.empty())
		{
			wcerr << L"��r���o�[�W������DB���w�肵�Ă�������" << endl;
			good = false;
		}

		if(m_CompDB.empty())
		{
			wcerr << L"��r��o�[�W������DB���w�肵�Ă�������" << endl;
			good = false;
		}

		if(m_MeshList.empty())
		{
			wcerr << L"���b�V�����X�g���w�肵�Ă�������" << endl;
			good = false;
		}

		if(m_Runlog.empty())
		{
			wcerr << L"���s���O���w�肵�Ă�������" << endl;
			good = false;
		}

		if(m_Errlog.empty())
		{
			wcerr << L"�G���[���O���w�肵�Ă�������" << endl;
			good = false;
		}
		
		if(!good)
		{
			cerr << desc << endl;
			return false;
		}

		// �ŏI�X�V����臒l���w�肳��Ă���ΐݒ�
		if( !strModDate.empty() )
		{
			m_ModDate = strModDate.c_str();
			if( FAILED( m_ModDate.ChangeType(VT_DATE) ) )
			{
				wcerr << L"�w�肳�ꂽ�ŏI�X�V�����s���ł�" << endl;
				return false;
			}
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

bool Arguments::parseMeshes( vector<long>& meshes, map<int, CString>& mapInvalidRow )
{
	using namespace uh::str_util;
		
	// ���b�V�����X�g�̉��
	int rowCnt = 0;
	const auto& listStrMesh = text_util::ToTStrList(m_MeshList);
	for( const auto& strMesh : listStrMesh )
	{
		++rowCnt;

		try
		{
			// ���b�V���R�[�h�Ƃ��Đ��������`�F�b�N
			// �ŏ��̌���3�`6, 6���̏ꍇ�͉�2����0�`7, 8���̏ꍇ�͉�4����0�`7
			const tstring strRegEx( _T("^[3-6]\\d{3}([0-7]{2}|[0-7]{4})$") );
			bool isValidMeshCode = regex_match( toStr(strMesh), smatch(), regex( toStr(strRegEx) ) );
			if( !isValidMeshCode )
				throw std::exception();

			// long�^�ɃL���X�g
			long mesh = boost::lexical_cast<long>( strMesh );

			// 6����8���̃R�[�h�����i�[����Ȃ��͂�
			meshes.emplace_back( mesh );
		}
		catch( ... )
		{
			// �L���X�g�ł��Ȃ��ꍇ�A���b�V���R�[�h�Ƃ��ĕs���ȏꍇ�A
			// �����Ώۂ̃��b�V���łȂ��ꍇ�ɂ́A�s�ԍ��ƕR�t���ĕێ�
			mapInvalidRow[rowCnt] = strMesh.c_str();
		}
		
	}
	
	// �s���������������炱���ŏI��
	if( !mapInvalidRow.empty() )
	{
		m_MeshType = kInvalid;
		return false;
	}

	// ���b�V�����X�g��1�s�ڂ̌�������A�������b�V���P�ʂ𔻒�
	int frontMeshDigit = g_cnv.GetMeshDigit( meshes.front() );
	switch (frontMeshDigit)
	{
	case 6:
		m_MeshType = kBasemesh;
		break;
	case 8:
		m_MeshType = kCitymesh;
		break;
	default:
		assert(false);
		break;
	}

	int meshCnt = 0;
	for( const auto& mesh : meshes )
	{
		++meshCnt;

		// 1�s�ڂƌ����������ł����OK
		if( frontMeshDigit == g_cnv.GetMeshDigit( mesh ) )
			continue;

		// �������قȂ�ꍇ�́A�s�ԍ��ƕR�t���ĕێ�
		mapInvalidRow[meshCnt] = format( _T("%d"), mesh );
	}

	// �s���ȕ����񂪂Ȃ����true
	return mapInvalidRow.empty();
}
