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

// DeleteMarkedKakei.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <boost/format.hpp>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include <WinLib/VersionInfo.h>
#include <tstring.h>
#include <boost_program_options.h>
#include "Pgdb.h"

using namespace std;
namespace po = boost::program_options;

namespace {
	CString gTargetPath;	///< �^�[�Q�b�g�f�[�^�t�H���_
	CString gMarkPath;		///< �}�[�L���O�f�[�^�t�H���_
	CString gDelWord;		///< �폜�������[�h
	CString gDate1;			///< �������t�P
	CString gDate2;			///< �������t�Q
	CString gLogFile;		///< ���O�t�@�C��
	CString gMeshList;		///< ���b�V�����X�g
}

/** 
 * @brief �g�p�@�\�� 
 * @param[in]	App	���s���R�}���h���C���̈�� 
 */ 
void PrintUsage( LPCTSTR App, po::options_description& opt)
{
	_TCHAR AppName[64]= {0};
	_tsplitpath(App, NULL, NULL, AppName, NULL);

	cout << "�}�[�N�ƌ`�폜�v���O����\n"; 
	cout <<	"Usage:" << AppName << " [�I�v�V����]\n";
	cout << opt << endl;
}

/**
 * @brief �R�}���h���C����́B
 *
 * @param argc	[in]	���C���֐��̑�����
 * @param argv	[in]	���C���֐��̑�����
 *
 * @retval true		����
 * @retval false	���s�A�G���[
 */
bool AnalyseArg(int argc, _TCHAR* argv[])
{
	// �I�v�V�����̐݌v
	po::options_description desc("�I�v�V����");
	desc.add_options()
		("target-path,t", uh::tvalue<uh::tstring>(),   "�^�[�Q�b�g�f�[�^�t�H���_")
		("marking-path,m", uh::tvalue<uh::tstring>(),  "�}�[�L���O�f�[�^�t�H���_")
		("keyword,w", uh::tvalue<uh::tstring>(),       "�폜�������[�h")
		("date1,1", uh::tvalue<uh::tstring>(),        "�������t�P")
		("date2,2", uh::tvalue<uh::tstring>(),        "�������t�Q")
		("out-path,o", uh::tvalue<uh::tstring>(),      "���s�����O�t�@�C��")
		("mesh-list,l", uh::tvalue<uh::tstring>(),     "���b�V�����X�g")
		("help,?",                                     "�w���v��\��")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	//�K�{�I�v�V��������
	if( vm.count("help") || !vm.count("target-path") || !vm.count("marking-path") || !vm.count("keyword") || !vm.count("out-path") || !vm.count("mesh-list")){
		PrintUsage(argv[0], desc);
		return false;
	}

	gTargetPath = vm["target-path"].as<uh::tstring>().c_str();
	gMarkPath = vm["marking-path"].as<uh::tstring>().c_str();
	gDelWord = vm["keyword"].as<uh::tstring>().c_str();
	gLogFile = vm["out-path"].as<uh::tstring>().c_str();
	gMeshList = vm["mesh-list"].as<uh::tstring>().c_str();
	if(vm.count("date1")){
		gDate1 = vm["date1"].as<uh::tstring>().c_str();
	}
	if(vm.count("date2")){
		gDate2 = vm["date2"].as<uh::tstring>().c_str();
	}
	return true;
}

/**
 * @brief ���s���{�́B
 *
 */
void Execute()
{
	FILE *pFile;
	if((pFile = _tfopen(gLogFile, _T("wt"))) == NULL){
		_ftprintf(stderr, _T("ERROR,���O�t�@�C�����J���܂���,%s\n"), static_cast<LPCTSTR>(gLogFile));
		return;
	}
	_ftprintf(pFile, _T("#MESHXYLOG\n"));
	// �c�[���o�[�W����
	time_t st_time;
	time(&st_time);
	CVersion cVer;
	_ftprintf(pFile, _T("#%s FILEVERSION %s PRODUCTVERSION %s\n"), static_cast<LPCTSTR>(cVer.GetOriginalFilename()), static_cast<LPCTSTR>(cVer.GetFileVersion()), static_cast<LPCTSTR>(cVer.GetProductVersion()));
	SYSTEMTIME  aTime;
	GetLocalTime(&aTime);
	_ftprintf(pFile, _T("#�J�n����: %04d/%02d/%02d %02d:%02d:%02d\n"),
		aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
		aTime.wMinute, aTime.wSecond);
	CPgdb aPgdb;
	if(aPgdb.init(gTargetPath, gMarkPath, gDelWord, gDate1, gDate2, pFile)){
		aPgdb.execDeleteKakei(gMeshList);
	}
	time_t tm;
	time(&tm);
	GetLocalTime(&aTime);
	_ftprintf(pFile, _T("#�I������: %04d/%02d/%02d %02d:%02d:%02d\n"),
		aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
		aTime.wMinute, aTime.wSecond);
	_tprintf(_T(" %6.0f�b\n"), difftime( tm, st_time));
	_ftprintf(pFile, _T("#�g�[�^������%6.0f�b\n"), difftime( tm, st_time));
	_tprintf(_T("�I�����܂�\n"));
	fclose(pFile);
	return;
}

/**
 * @brief ���C���֐��B
 *
 * @param argc	[in]	�R�}���h���C���̃g�[�N�����i�H�j
 * @param argv	[in]	�R�}���h���C���̃g�[�N����i�H�j
 *
 * @retval 0		����I���i���͂����������Ă͂��Ȃ��j
 * @retval 0�ȊO	�ُ�I��
 */
int _tmain(int argc, _TCHAR* argv[])
{
	::_tsetlocale(LC_ALL, _T("Japanese"));

	try {
		const arctl::coinitializer coinit;	//�F�؂Ɏ��s����Ɨ����邩��try�̓����Ɉړ�
		if(AnalyseArg(argc, argv)){
			Execute();
		}
	}
	catch(const std::exception& e) {
		printf("%s\n", e.what());
		fprintf(stderr, "#Error,%s\n", e.what());
		return 1;
	}
	return 0;
}
