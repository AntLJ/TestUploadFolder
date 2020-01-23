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
#include <regex>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_input = "input";

	po::options_description desc("�g����");

	try
	{
		desc.add_options()
			("help,h", "�w���v��\��")
			(g_strBeforeShpPath,         uh::tvalue<uh::tstring>(&BeforeShpPath),     "[�K�{]�ϊ��O�V�F�[�v�t�@�C���f�B���N�g��")
			(g_strBeforeDBPath,          uh::tvalue<uh::tstring>(&BeforeDBPath),      "[�I��]�ϊ��ODB (�����m�[�h�����������ɕK�{)")
			(g_strAfterShpPath,          uh::tvalue<uh::tstring>(&AfterShpPath),      "[�K�{]�ϊ���V�F�[�v�t�@�C���f�B���N�g��")
			(g_strMeshDBPath,            uh::tvalue<uh::tstring>(&MeshDBPath),        "[�I��]���b�V���`��p��DB�i�����m�[�h�������͕K�{�j")
			(g_strMeshSchema,            uh::tvalue<uh::tstring>(&MeshSchema),        "[�I��]���b�V���`��p��DB�X�L�[�}��")
			(g_strMeshlistPath,          uh::tvalue<uh::tstring>(&MeshlistPath),      "[�K�{]���b�V�����X�g")
			(g_strCheckMode,             uh::tvalue<uh::tstring>(&CheckMode),         "[�K�{]�`�F�b�N���[�h (1=�����N�����������N������, 2=���s�҃m�[�h�����E���s�҃����N�}�[�W����, 3=�����m�[�h��������)")
			(g_strTky2JgdPath,           uh::tvalue<uh::tstring>(&Tky2JgdPath),       "[�I��]�n��p�����[�^�p�X (�����m�[�h�����������ɑI����)")
			(g_strMinSegment,            uh::tvalue<uh::tstring>(&MinSegment),        "[�K�{]�����Z�O�����g�� (�P��[m])")
			(g_strRunlog,                uh::tvalue<uh::tstring>(&RunLogPath),        "[�K�{]���s���O�o�͐�")
			(g_strErrlog,                uh::tvalue<uh::tstring>(&ErrLogPath),        "[�K�{]�G���[���O�o�͐�")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if(1 == argc) {
			std::cout << desc << std::endl;
			return false;
		}

		// �K�{�p�����[�^�̃`�F�b�N
		bool bAllow = true;
		if(0 == vm.count(g_strBeforeShpPath) ||
			0 == vm.count(g_strAfterShpPath) ||
			0 == vm.count(g_strMeshlistPath) ||
			0 == vm.count(g_strCheckMode) ||
			0 == vm.count(g_strMinSegment) ||
			0 == vm.count(g_strRunlog) ||
			0 == vm.count(g_strErrlog)){
				std::cout << "�K�{�p�����[�^���w�肵�Ă�������" << std::endl;
				std::cout << desc << std::endl;
				return false;
		}
		// �����m�[�h�������́umesh_db�v�I�v�V�����͕K�{
		if(0 == CheckMode.compare(_T("3")) && 0 == vm.count(g_strMeshDBPath)){
			std::cout << "�����m�[�h�������������Ɂumesh_db�v�I�v�V�����͕K�{" << std::endl;
			std::cout << desc << std::endl;
			return false;
		}
		// �����m�[�h�������́ubefore_db�v�I�v�V�����͕K�{
		if(0 == CheckMode.compare(_T("3")) && 0 == vm.count(g_strBeforeDBPath)){
			std::cout << "�����m�[�h������������[before_db]�I�v�V�����͕K�{" << std::endl;
			std::cout << desc << std::endl;
			return false;
		}
		if(0 != CheckMode.compare(_T("3")) && 0 < vm.count(g_strBeforeDBPath)){
			std::cout << "�����m�[�h�����ȊO��[before_db]�I�v�V�����̎w��͕s�v" << std::endl;
			std::cout << desc << std::endl;
			return false;
		}

		// �����m�[�h�������́utky2jgd_path�v�I�v�V�����͑I�𐧁A����ȊO��NG
		if(0 != CheckMode.compare(_T("3")) && 0 < vm.count(g_strTky2JgdPath)){
			std::cout << "�����m�[�h�����ȊO��[tky2jgd_path]�I�v�V�����̎w��͕s�v" << std::endl;
			std::cout << desc << std::endl;
			return false;
		}

		// �`�F�b�N���[�h�̒l�́u1,2,3�v�̂ݗL��
		if(0 != CheckMode.compare(_T("1")) &&
			0 != CheckMode.compare(_T("2")) &&
			0 != CheckMode.compare(_T("3")) &&
			0 != CheckMode.compare(_T("99"))){
				std::cout << "�`�F�b�N���[�h��[1=�����N�����������N������, 2=���s�҃m�[�h�����E���s�҃����N�}�[�W����, 3=�����m�[�h��������]�̂ݎw��\" << std::endl;
				std::cout << desc << std::endl;
				return false;
		}

		// �����Z�O�����g���͐��l�̂݋��e
		std::wregex reg(L"\\d+.\\d+");
		if(!std::regex_match(MinSegment.c_str(), reg)){
			std::cout << "�����Z�O�����g���͐��l�̂ݎw��\" << std::endl;
			std::cout << desc << std::endl;
			return false;
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

CString Arguments::GetOption()
{
	CString strOption = _T("[option]\n");
	if(!BeforeShpPath.empty() )
		strOption += GetOptionLine(g_strBeforeShpPath, BeforeShpPath.c_str());

	if(!BeforeDBPath.empty() )
		strOption += GetOptionLine(g_strBeforeDBPath, BeforeDBPath.c_str());

	if(!AfterShpPath.empty() )
		strOption += GetOptionLine(g_strAfterShpPath, AfterShpPath.c_str());

	if(!MeshDBPath.empty() )
		strOption += GetOptionLine(g_strMeshDBPath, MeshDBPath.c_str());

	if(!MeshSchema.empty() )
		strOption += GetOptionLine(g_strMeshSchema, MeshSchema.c_str());

	if(!MeshlistPath.empty() )
		strOption += GetOptionLine(g_strMeshlistPath, MeshlistPath.c_str());

	if(!CheckMode.empty())
		strOption += GetOptionLine(g_strCheckMode, CheckMode.c_str());

	if(!Tky2JgdPath.empty())
		strOption += GetOptionLine(g_strTky2JgdPath, Tky2JgdPath.c_str());

	if(!MinSegment.empty())
		strOption += GetOptionLine(g_strMinSegment, MinSegment.c_str());

	if(!RunLogPath.empty())
		strOption += GetOptionLine(g_strRunlog, ErrLogPath.c_str());

	if(!ErrLogPath.empty())
		strOption += GetOptionLine(g_strErrlog, RunLogPath.c_str());

	return strOption;
}

CString Arguments::GetOptionLine(const CString& strParam, const CString& strValue)
{
	CString strOptionLine = _T("   --");
	strOptionLine.Format(_T("   --%s %s\n"), strParam, strValue);

	return strOptionLine;
}
