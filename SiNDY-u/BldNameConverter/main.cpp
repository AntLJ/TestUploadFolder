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

// BldNameConverter.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//
#include <arctl/coinitializer.h>
#include <WinLib/VersionInfo.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/japanese_locale.h>
#include <comdef.h>
#include <shlwapi.h>

#include "stdafx.h"
#include "LogSys.h"
#include "common.h"

using boost::program_options::options_description;
using boost::program_options::value;

namespace po = boost::program_options;

extern bool exec(const po::variables_map &vmContent, bool isTest);

bool option_exist_chk(const po::variables_map &vmMap, LPCSTR option)
{
	if (vmMap.count(option) == 0) {
		std::cerr << "ERROR\t" << option << "���s���ł�" << std::endl;
		return false;
	}
	return true;
}

bool file_exist_chk(const po::variables_map &vmMap, LPCSTR option)
{
	if (! PathFileExists(vmMap[option].as<uh::tstring>().c_str())) {
		std::cerr << "ERROR\t" << option << "���s���ł�" << std::endl;
		return false;
	}
	return true;
}

bool dir_exist_chk(const po::variables_map &vmMap, LPCSTR option)
{
	if (! PathIsDirectory(vmMap[option].as<uh::tstring>().c_str())) {
		std::cerr << "ERROR\t" << option << "���s���ł�" << std::endl;
		return false;
	}
	return true;
}

bool ini_chk(const po::variables_map &vmContent)
{
	bool ok = true;
	// DB�֘A�͒l�������Ă��邩�̃`�F�b�N�E���X�g�E�f�B���N�g���֘A�̓t�@�C���̑��݃`�F�b�N�E
	ok &= option_exist_chk(vmContent, BASE_DB);
	ok &= option_exist_chk(vmContent, MAP_DB);
	ok &= option_exist_chk(vmContent, MESH_DB);
	ok &= file_exist_chk(vmContent, BLD_EXC_LIST);
	ok &= file_exist_chk(vmContent, MESH_LIST);
	ok &= dir_exist_chk(vmContent, OUTPUT_DIR);
	return ok;
}

bool write_usage()
{
	std::cout << "How to set ini_file" << std::endl;
	std::cout << "  " << BASE_DB << " = <�x�[�XDB>" << std::endl;
	std::cout << "  " << MAP_DB << " = <�n�}DB>" << std::endl;
	std::cout << "  " << MESH_DB << " = <���b�V��DB>" << std::endl;
	std::cout << "  " << BLD_EXC_LIST << " = <�����ΏۊO������ʃ��X�g>" << std::endl;
	std::cout << "  " << MESH_LIST << " = <�ϊ��Ώۃ��b�V�����X�g>" << std::endl;
	std::cout << "  " << OUTPUT_DIR << " = <�o�̓f�B���N�g��>" << std::endl;
	return false;
}

bool arg_check(int argc, _TCHAR* argv[], po::variables_map *vmContent, bool *isTest)
{
	// ���P�[���֘A�̕s��h�~	
	uh::setJapaneseCharacterTypeFacet();
	// �����ǂݍ���
	uh::tstring iniFile, procLog, errLog;
	po::options_description descPath("How to user");
	descPath.add_options()
		(INI, uh::tvalue<uh::tstring>(&iniFile), "�ݒ�t�@�C��(�K�{)")
		(PROCLOG, uh::tvalue<uh::tstring>(&procLog), "���s���O(�C��)")
		(ERRLOG, uh::tvalue<uh::tstring>(&errLog), "�G���[���O(�C��)")
		(TEST, "���ؗp���[�h(�`��t���ł̏o��)");
	if (1 == argc) {
		std::cout << descPath << std::endl;
		return write_usage();
	}

	po::variables_map vmPath;
	po::store(po::parse_command_line(argc, argv, descPath), vmPath);
	po::notify(vmPath);

	// �ݒ�t�@�C���p�X�`�F�b�N
	if (! PathFileExists(iniFile.c_str())) {
		std::cerr << "ERROR\t" << INI << "�I�v�V�������s���ł�" << std::endl;
		return false;
	}
	// ���s���O�E�G���[���O�̃`�F�b�N
	if (vmPath.count(PROCLOG) != 0) {
		if (! LogSys::GetInstance().SetProcLog(procLog.c_str())) {
			std::cerr << "ERROR\t" << PROCLOG << "�I�v�V�������s���ł�" << std::endl;
			return false;
		}
	}
	if (vmPath.count(ERRLOG) != 0) {
		if (! LogSys::GetInstance().SetErrLog(errLog.c_str())) {
			std::cerr << "ERROR\t" << ERRLOG << "�I�v�V�������s���ł�" << std::endl;
			return false;
		}
		// SiNDYFreeStyleLog�p�̃w�b�_�L��
		LogSys::GetInstance().WriteErrLog(false, false, _T("# FREESTYLELOG\n"));
		LogSys::GetInstance().WriteErrLog(false, false, _T("FLAG\tLAYER\tOBJECTID\t�G���[���x��\t�G���[���b�Z�[�W\n"));
	}
	// ���[�h�̃`�F�b�N
	if (vmPath.count(TEST) != 0)
		*isTest = true;

	// �ݒ�t�@�C���ǂݍ���
	po::options_description descContent("How to set a ini file");
	descContent.add_options()
		(BASE_DB, uh::tvalue<uh::tstring>(), "�x�[�XDB")
		(MAP_DB, uh::tvalue<uh::tstring>(), "�n�}DB")
		(MESH_DB, uh::tvalue<uh::tstring>(), "���b�V��DB")
		(BLD_EXC_LIST, uh::tvalue<uh::tstring>(), "�����ΏۊO������ʃ��X�g")
		(MESH_LIST, uh::tvalue<uh::tstring>(), "�ϊ��Ώۃ��b�V�����X�g")
		(OUTPUT_DIR, uh::tvalue<uh::tstring>(), "�o�̓f�B���N�g��");

	std::ifstream ifsIniFile(CT2CA(iniFile.c_str()));
	if (! ifsIniFile.is_open()) {
		std::cerr << "ERROR\t�ݒ�t�@�C���̃I�[�v���Ɏ��s���܂���" << std::endl;
		return false;
	}
	po::store(po::parse_config_file(ifsIniFile, descContent), *vmContent);
	po::notify(*vmContent);

	if (! ini_chk(*vmContent))
		return write_usage();
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;
	try {
		arctl::coinitializer coinitializer;
		// �����擾&�`�F�b�N
		po::variables_map vmContent;
		bool isTest = false;
		if (! arg_check(argc, argv, &vmContent, &isTest)) {
			retval = 1;
		} else {
			// �������s
			LogSys::GetInstance().WriteProcLog(true, true, _T("�����J�n\n"));
			if (! exec(vmContent, isTest))
				retval = 1;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		retval = 2;
	} catch (const _com_error &com_err) {
		std::cerr << CT2CA(com_err.ErrorMessage()) << std::endl;
	}
	if (retval == 0) {
		LogSys::GetInstance().WriteProcLog(true, true, _T("����I��\n"));
	} else {
		LogSys::GetInstance().WriteProcLog(true, true, _T("�ُ�I��\n"));
	}

	std::cout << std::flush;
	std::cerr << std::flush;

	return retval;
}
