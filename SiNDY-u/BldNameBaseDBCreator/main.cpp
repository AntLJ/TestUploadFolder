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

// main.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "LogSys.h"
#include "common.h"

#include <arctl/coinitializer.h>
#include <WinLib\/VersionInfo.h>
#include <WinLib/CStringTokenizer.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/japanese_locale.h>
#include <shlwapi.h>


using boost::program_options::options_description;
using boost::program_options::value;

namespace po = boost::program_options;

extern bool exec(const po::variables_map &vmContent, bool isUpdate, bool isSurvey);

void WriteOptionContent(LPCSTR option, LPCTSTR content)
{
	CString opAndCon;
	opAndCon.Format(_T("  %s:\t%s\n"), CString(option), content);
	LogSys::GetInstance().WriteProcLog(true, false, opAndCon);
}

void ProcInfoWrite(const po::variables_map &vmContent)
{
	// �c�[�����̕\��
	CVersion toolVersion;
	CString toolInfo;
	toolInfo.Format(_T("%s FILVERSION:%s PRODUCTVERSION:%s\n"), toolVersion.GetOriginalFilename(), toolVersion.GetFileVersion(), toolVersion.GetProductVersion());
	LogSys::GetInstance().WriteProcLog(true, false, toolInfo);
	// �ݒ���e�̕\��
	LogSys::GetInstance().WriteProcLog(true, false, _T("ini contents\n"));
	WriteOptionContent(SRC_DB, vmContent[SRC_DB].as<uh::tstring>().c_str());
	WriteOptionContent(MAP_DB, vmContent[MAP_DB].as<uh::tstring>().c_str());
	WriteOptionContent(ADDR_DB, vmContent[ADDR_DB].as<uh::tstring>().c_str());
	WriteOptionContent(TWN_DB, vmContent[TWN_DB].as<uh::tstring>().c_str());
	WriteOptionContent(POI_DB, vmContent[POI_DB].as<uh::tstring>().c_str());
	WriteOptionContent(BASE_DB, vmContent[BASE_DB].as<uh::tstring>().c_str());
	WriteOptionContent(MESH_DB, vmContent[MESH_DB].as<uh::tstring>().c_str());
	WriteOptionContent(AM_DB, vmContent[AM_DB].as<uh::tstring>().c_str());
	WriteOptionContent(SURVEY_LIST, vmContent[SURVEY_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(TGT_ADDR_LIST, vmContent[TGT_ADDR_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(SRC_PRI_LIST_PRIOR, vmContent[SRC_PRI_LIST_PRIOR].as<uh::tstring>().c_str());
	WriteOptionContent(SRC_PRI_LIST_MATCH, vmContent[SRC_PRI_LIST_MATCH].as<uh::tstring>().c_str());
	WriteOptionContent(POI_PRI_LIST, vmContent[POI_PRI_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(SUP_POI_PRI_LIST, vmContent[SUP_POI_PRI_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(BLD_EXC_LIST, vmContent[BLD_EXC_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(REPLACE_LIST, vmContent[REPLACE_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(COND_LIST, vmContent[COND_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(TWN_TBL_NAME, vmContent[TWN_TBL_NAME].as<uh::tstring>().c_str());
	WriteOptionContent(SOURCENAME, vmContent[SOURCENAME].as<uh::tstring>().c_str());
	WriteOptionContent(SOURCEDATE, vmContent[SOURCEDATE].as<uh::tstring>().c_str());
}

bool WriteHowToSetIni()
{
	std::cout << "HOW to set ini_file" << std::endl;
	std::cout << "  " << SRC_DB << " = <�f��DB>" << std::endl;
	std::cout << "  " << MAP_DB << " = <�n�}DB>" << std::endl;
	std::cout << "  " << ADDR_DB << " = <�s���EDB>" << std::endl;
	std::cout << "  " << TWN_DB << " = <TWNDB>" << std::endl;
	std::cout << "  " << POI_DB << " = <POIDB>" << std::endl;
	std::cout << "  " << BASE_DB << " = <�x�[�XDB(�o�͐�)>" << std::endl;
	std::cout << "  " << MESH_DB << " = <���b�V��DB>" << std::endl;
	std::cout << "  " << AM_DB << " = <�Z���}�X�^�i�[�Z��DB>" << std::endl;
	std::cout << "  " << SURVEY_LIST << " = <�����|�C���g���X�g(�o�͐�)>" << std::endl;
	std::cout << "  " << TGT_ADDR_LIST << " = <�����Ώۍs���R�[�h(5��)���X�g" << std::endl;
	std::cout << "  " << SRC_PRI_LIST_PRIOR << " = <�f�ޗD��x��ʃ��X�g(�D�旘�p)>" << std::endl;
	std::cout << "  " << SRC_PRI_LIST_MATCH << " = <�f�ޗD��x��ʃ��X�g(�}�b�`���O)>" << std::endl;
	std::cout << "  " << POI_PRI_LIST << " = <���LPOI�D��x��ʃ��X�g>" << std::endl;
	std::cout << "  " << SUP_POI_PRI_LIST << " = <�⊮�p���LPOI�D��x��ʃ��X�g>" << std::endl;
	std::cout << "  " << BLD_EXC_LIST << " = <�����ΏۊO������ʃ��X�g>" << std::endl;
	std::cout << "  " << REPLACE_LIST << " = <�u�������񃊃X�g>" << std::endl;
	std::cout << "  " << COND_LIST << " = <�̗p�����L�q���X�g>" << std::endl;
	std::cout << "  " << TWN_TBL_NAME << " = <townpage�e�[�u����>" << std::endl;
	std::cout << "  " << SOURCENAME << " = <SOURCENAME�o�͒l>" << std::endl;
	std::cout << "  " << SOURCEDATE << " = <SOURCEDATE�o�͒l>" << std::endl;
	return false;
}

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

bool yyyymmddd_chk(const po::variables_map &vmMap, LPCSTR option) 
{
	CComVariant yyyymmdd = vmMap[option].as<uh::tstring>().c_str();
	yyyymmdd.ChangeType(VT_DATE);

	if (yyyymmdd.vt != VT_DATE) {
		std::cerr << "ERROR\t" << option << "���s���ł�" << std::endl;
		return false;
	}
	return true;
}

bool ini_check(const po::variables_map &vmContent, bool isUpdate, bool isSurvey)
{
	bool ok = true;
	// DB�֘A�͒l�������Ă��邩�̃`�F�b�N�E���X�g�֘A�̓t�@�C���̑��݃`�F�b�N
	ok &= option_exist_chk(vmContent, SRC_DB);
	ok &= option_exist_chk(vmContent, MAP_DB);
	ok &= option_exist_chk(vmContent, ADDR_DB);
	ok &= option_exist_chk(vmContent, POI_DB);
	ok &= option_exist_chk(vmContent, BASE_DB);
	if (isSurvey) {
		ok &= option_exist_chk(vmContent, MESH_DB);
		ok &= option_exist_chk(vmContent, AM_DB);
		ok &= option_exist_chk(vmContent, TWN_DB);
		// TWN_TBL_NAME, SOURCENAME�ɂ��Ă��A�l�������Ă��邩�`�F�b�N
		ok &= option_exist_chk(vmContent, TWN_TBL_NAME);
		ok &= option_exist_chk(vmContent, SOURCENAME);
		// SOURCEDATE�ɂ��Ă�yyyy/mm/dd�œ��͂���Ă��邩�`�F�b�N
		ok &= yyyymmddd_chk(vmContent, SOURCEDATE);
	}
	ok &= file_exist_chk(vmContent, TGT_ADDR_LIST);
	ok &= file_exist_chk(vmContent, SRC_PRI_LIST_PRIOR);
	ok &= file_exist_chk(vmContent, SRC_PRI_LIST_MATCH);
	ok &= file_exist_chk(vmContent, POI_PRI_LIST);
	ok &= file_exist_chk(vmContent, SUP_POI_PRI_LIST);
	ok &= file_exist_chk(vmContent, BLD_EXC_LIST);
	ok &= file_exist_chk(vmContent, REPLACE_LIST);
	ok &= file_exist_chk(vmContent, COND_LIST);
	return ok;
}

bool arg_check(int argc, _TCHAR* argv[], po::variables_map *vmContent, bool *isUpdate, bool *isSurvey)
{
	// ������
	*isUpdate = false;
	*isSurvey = false;
	// ���P�[���֘A�̕s��h�~
	uh::setJapaneseCharacterTypeFacet();
	// �����ǂݍ���
	uh::tstring iniFile, procLog, errLog;
	po::options_description descPath("How to user");
	descPath.add_options()
		(INI, uh::tvalue<uh::tstring>(&iniFile), "�ݒ�t�@�C��(�K�{)")
		(UPDATE, "�x�[�XDB�X�V���[�h(�C��)")
		(SURVEY, "�����Ώۃ|�C���g�o�̓��[�h(�C��)")
		(PROCLOG, uh::tvalue<uh::tstring>(&procLog), "���s���O(�C��)")
		(ERRLOG, uh::tvalue<uh::tstring>(&errLog), "�G���[���O(�C��)");
	if (1 == argc) {
		std::cout << descPath << std::endl;
		return WriteHowToSetIni();
	}

	po::variables_map vmPath;
	po::store(po::parse_command_line(argc, argv, descPath), vmPath);
	po::notify(vmPath);

	// �ݒ�t�@�C���p�X�`�F�b�N
	if (! PathFileExists(iniFile.c_str())) {
		std::cerr << "ERROR\t" << INI << "�I�v�V�������s���ł�" << std::endl;
		return false;
	}
	// ���[�h�̃`�F�b�N
	if ((!vmPath.count(UPDATE)) && (!vmPath.count(SURVEY))) {
		std::cerr << "ERROR\t" << UPDATE << "�I�v�V���� or " << SURVEY << "�I�v�V�������w�肵�Ă�������" << std::endl;
		return false;
	}
	if (vmPath.count(UPDATE))
		*isUpdate = true;
	if (vmPath.count(SURVEY))
		*isSurvey = true;

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
		// SiNDYFreeStyelLog�p�̃w�b�_�L��
		LogSys::GetInstance().WriteErrLog(false, false, _T("# FREESTYLELOG\n"));
		LogSys::GetInstance().WriteErrLog(false, false, _T("FLAG\tLAYER\tOBJECTID\t�G���[���x��\t�G���[���b�Z�[�W\n"));
	}
	// �ݒ�t�@�C���ǂݍ���
	po::options_description descContent("How to set a ini file");
	descContent.add_options()
		(SRC_DB, uh::tvalue<uh::tstring>(), "�f��DB")
		(MAP_DB, uh::tvalue<uh::tstring>(), "�n�}DB")
		(ADDR_DB, uh::tvalue<uh::tstring>(), "�s���EDB")
		(TWN_DB, uh::tvalue<uh::tstring>(), "TWNDB")
		(POI_DB, uh::tvalue<uh::tstring>(), "POIDB")
		(BASE_DB, uh::tvalue<uh::tstring>(), "�x�[�XDB(�o�͐�)")
		(MESH_DB, uh::tvalue<uh::tstring>(), "���b�V��DB")
		(AM_DB, uh::tvalue<uh::tstring>(), "�Z���}�X�^�i�[�Z��DB")
		(SURVEY_LIST, uh::tvalue<uh::tstring>(), "�����|�C���g���X�g(�o�͐�)")
		(TGT_ADDR_LIST, uh::tvalue<uh::tstring>(), "�����Ώۍs���R�[�h(5��)���X�g")
		(SRC_PRI_LIST_PRIOR, uh::tvalue<uh::tstring>(), "�f�ޗD��x��ʃ��X�g(�D�旘�p)")
		(SRC_PRI_LIST_MATCH, uh::tvalue<uh::tstring>(), "�f�ޗD��x��ʃ��X�g(�}�b�`���O)")
		(POI_PRI_LIST, uh::tvalue<uh::tstring>(), "���LPOI�D��x��ʃ��X�g")
		(SUP_POI_PRI_LIST, uh::tvalue<uh::tstring>(), "�⊮�p���LPOI�D��x��ʃ��X�g")
		(BLD_EXC_LIST, uh::tvalue<uh::tstring>(), "�����ΏۊO������ʃ��X�g")
		(REPLACE_LIST, uh::tvalue<uh::tstring>(), "�u�������񃊃X�g")
		(COND_LIST, uh::tvalue<uh::tstring>(), "�̗p�����L�q���X�g")
		(TWN_TBL_NAME, uh::tvalue<uh::tstring>(), "townpage�e�[�u������")
		(SOURCENAME, uh::tvalue<uh::tstring>(), "SOURCENAME�o�͒l")
		(SOURCEDATE, uh::tvalue<uh::tstring>(), "SOURCEDATE�o�͒l");

	std::ifstream ifsIniFile(CT2CA(iniFile.c_str()));
	if (! ifsIniFile.is_open()) {
		std::cerr << "ERROR\t�ݒ�t�@�C���̃I�[�v���Ɏ��s���܂���" << std::endl;
		return false;
	}
	po::store(po::parse_config_file(ifsIniFile, descContent), *vmContent);
	po::notify(*vmContent);

	if (! ini_check(*vmContent, *isUpdate, *isSurvey))
		return WriteHowToSetIni();
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;
	try {
		arctl::coinitializer coinitializer;
		// �����擾&�`�F�b�N
		po::variables_map vmContent;
		bool isUpdate = false, isSurvey = false;
		if (! arg_check(argc, argv, &vmContent, &isUpdate, &isSurvey)) {
			retval = 1;
		} else {
			// �������s
			LogSys::GetInstance().WriteProcLog(true, true, _T("�����J�n\n"));
			if (! exec(vmContent, isUpdate, isSurvey))
				retval = 1;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		retval = 2;
	} catch (const _com_error &com_err) {
		std::cerr << CT2CA(com_err.ErrorMessage()) << std::endl;
		retval = 2;
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
