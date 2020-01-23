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

// DiffImportRoad.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <boost/format.hpp>
#include <sindy/workspace.h>
#include <arctl/coinitializer.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/boost_program_options.h>
#include "Reflect.h"
#include "TimeLog.h"

namespace {

	CString gConnect[2];	///< �T�[�o�ڑ�������

	CString gMeshList;		///< ���b�V�����X�g
	CString gOldPath;		///< �ҏW�OPGDB�x�[�X�p�X
	CString gNewPath;		///< �ҏW��PGDB�x�[�X
	CString gWorkVersion;	///< ��Ɨp�q�o�[�W����
	long gMode = 0;			///< ���샂�[�h�i�X�V�̗L���E�D��j
}

using namespace std;
namespace po = boost::program_options;

/** 
 * @brief �g�p�@�\�� 
 * @param[in]	App	���s���R�}���h���C���̈�� 
 */ 
void PrintUsage(LPCTSTR App, po::options_description& opt)
{
	_TCHAR AppName[64]= {0};
	_tsplitpath(App, NULL, NULL, AppName, NULL);

	cout << "���H�pPGDB������荞�݃v���O����\n"; 
	cout <<	"Usage:" << AppName << " [�I�v�V����]\n";
	cout << opt << endl;
}

/** 
 * @brief �g�p�@�\�� �i������Ƃ����ڂ����j
 * @param[in]	App	���s���R�}���h���C���̈�� 
 */ 
void PrintHelp(LPCTSTR App, po::options_description& opt)
{
	PrintUsage(App, opt);
	_tprintf(_T("�ҏW�O�E�ҏW���PGDB�̍�����SiNDY�ɔ��f������v���O�����ł��B\n")
			 _T("�ݒ�t�@�C���̏������́ADiffImportRoad.txt�������������B\n")
			);
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
		("old-path,o", uh::tvalue<uh::tstring>(),    "�ҏW�OPGDB�x�[�X�f�B���N�g���i�K�{�j")
		("new-path,n", uh::tvalue<uh::tstring>(),    "�ҏW��PGDB�x�[�X�f�B���N�g���i�K�{�j")
		("mesh-list,m", uh::tvalue<uh::tstring>(),   "���b�V�����X�g�i�K�{�j")
		("test-mode,t",                              "�e�X�g���[�h�i�ύX�Ȃ��j")
		("japan-mode,j",                             "������ƗD�惂�[�h")
		("help,?",                                   "�w���v��\��")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	//�w���v�\��
	if( vm.count("help")){
		PrintHelp(argv[0], desc);
		return false;
	}

	//�K�{�I�v�V��������
	if(!vm.count("old-path") || !vm.count("new-path") || !vm.count("mesh-list")){
		PrintUsage(argv[0], desc);
		return false;
	}

	gOldPath = vm["old-path"].as<uh::tstring>().c_str();
	gNewPath = vm["new-path"].as<uh::tstring>().c_str();
	gMeshList = vm["mesh-list"].as<uh::tstring>().c_str();
	if(vm.count("test-mode")){
		gMode |= Reflect::NO_CHG_MODE;
	}
	if(vm.count("japan-mode")){
		gMode |= Reflect::TDC_MODE;
	}
	return true;
}

/** 
 * @brief ���ϐ�����ڑ����̎擾�ƍ�Ɨp�q�o�[�W�����������߂�
 *
 */ 
bool GetConnectInfo(void)
{
#ifdef _DEBUG
	_putenv("DB_ROAD=SINDY2013/SINDY2013/SDE.ETC_IMPORTROAD_TEST/5151/onyx");
//	_putenv("DB_ROAD=TRIAL/TRIAL/TRIAL.ETC_IMPORTROAD_TEST/5151/coral2");
	_putenv("OPERATOR=test_130805");
	_putenv("WORK_VERSION=ETC_TEST_IMPORT_130829_test");
	_putenv("FIX_USER=FIX201307");
#endif

	LPCTSTR aEnv = _tgetenv(_T("DB_ROAD"));
	if(aEnv != NULL){
		gConnect[CLayerManage::DB_ROAD] = aEnv;
	}else{
		_ftprintf(stderr, _T("���ϐ��uDB_ROAD�v���ݒ肳��Ă��܂���\n"));
		return false;
	}
#ifdef STOP_POINT
	aEnv = _tgetenv(_T("DB_STOP"));
	if(aEnv != NULL){
		gConnect[CLayerManage::DB_STOP] = aEnv;
	}else{
		_ftprintf(stderr, _T("���ϐ��uDB_STOP�v���ݒ肳��Ă��܂���\n"));
		return false;
	}
#endif
	aEnv = _tgetenv(_T("WORK_VERSION"));
	if(aEnv != NULL){
		gWorkVersion = aEnv;
	}else{
		SYSTEMTIME stSystemTime;
		::GetLocalTime( &stSystemTime);
		//�����e�ŏ�����悤�ɂ��悤�Ƃ������A����ł͒��f�@�\�����ʂɂȂ�̂Łu_for_post�v�ǉ�
		gWorkVersion.Format(_T("DiffImportRoad_%04d/%02d/%02d %02d:%02d:%02d_for_post"), 
			stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
			stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
	}
	return true;
}

/** 
 * @brief ��Ɨp�q�o�[�W�����̍쐬���͎g���܂킵��A���ۂ̃o�[�W��������Ԃ�
 *
 * @retval ��Ɨp�̎q�o�[�W�������i���̂܂ܐڑ�������Ƃ��Ďg�p�\�j
 */ 
CString CreateWorkVersion(void)
{
	// �ڑ�
#ifdef STOP_POINT
	IVersionPtr ipCurrentVersion = CLayerManage::connectSDE( gConnect[CLayerManage::DB_STOP] );
#else
	IVersionPtr ipCurrentVersion = CLayerManage::connectSDE( gConnect[CLayerManage::DB_ROAD] );
#endif
	if(ipCurrentVersion){
		IVersionPtr ipChildVersion;
		((IVersionedWorkspacePtr)ipCurrentVersion)->FindVersion(CComBSTR(gWorkVersion), &ipChildVersion);
		if(ipChildVersion){
			_ftprintf(stderr, _T("#��Ɨp�q�o�[�W�����͊����̂��̂��g���܂�,%s\n"), gWorkVersion);
		}else{
			HRESULT hr = ipCurrentVersion->CreateVersion(CComBSTR(gWorkVersion),&ipChildVersion);
			if(ipChildVersion == NULL){
				_ftprintf(stderr, _T("#��Ɨp�q�o�[�W�����쐬���s,%s(hr=%d)\n"), gWorkVersion, hr);
				return _T("");
			}
		}
		return sindy::workspace_address((IWorkspacePtr)ipChildVersion).c_str();
	}
	return _T("");
}

/** 
 * @brief ���s���{��
 *
 */ 
void Execute()
{
	//�q�o�[�W�������쐬(�쐬�������U�ؒf)
	CString aWorkVersion[2];	//�z���0�Ԗڂ͓��H�f�[�^��ʗp�A1�͈ꎞ��~�p
	//�ŏI�I�ɂ͐ڑ�1�ŗǂ��Ȃ�\��������̂ŁA�Ƃ肠�������낢��n�[�h�R�[�f�B���O
#ifdef STOP_POINT
	aWorkVersion[CLayerManage::DB_ROAD] = gConnect[CLayerManage::DB_ROAD];
	aWorkVersion[1] = CreateWorkVersion();
#else
	aWorkVersion[CLayerManage::DB_ROAD] = CreateWorkVersion();
	aWorkVersion[CLayerManage::DB_STOP] = aWorkVersion[CLayerManage::DB_ROAD];	// �g��Ȃ��Ȃ����ǁA���̃G���[�`�F�b�N�����邵�������̂�˂�����
#endif
	if(!aWorkVersion[CLayerManage::DB_ROAD].IsEmpty() && !aWorkVersion[CLayerManage::DB_STOP].IsEmpty()){
#ifdef STOP_POINT
		_tprintf(_T("��ƃo�[�W������[%s]�ł�\n"), aWorkVersion[CLayerManage::DB_STOP]);
		_ftprintf(stderr, _T("#��ƃo�[�W������[%s]�ł�\n"), aWorkVersion[CLayerManage::DB_STOP]);
#else
		_tprintf(_T("��ƃo�[�W������[%s]�ł�\n"), aWorkVersion[CLayerManage::DB_ROAD]);
		_ftprintf(stderr, _T("#��ƃo�[�W������[%s]�ł�\n"), aWorkVersion[CLayerManage::DB_ROAD]);
#endif
		bool aContinue = true;
		CReflect aReflect;
		if(aReflect.init_once(aWorkVersion, gOldPath, gNewPath, gMeshList, gMode)){
			while(aContinue){
				//���f�ł���悤�Ƀ��[�v
				if(!aReflect.init()){
					return;
				}
				aContinue = aReflect.execReflect();
				while(aContinue){
					//�啶���́uQ�v���́uS�v�ȊO�͎󂯕t���Ȃ��iCTRL+C�͌����Ă��܂��j
					_tprintf(_T("�uS�v�L�[�ōĊJ���܂��B�uQ�v�L�[�ŏI�����܂��B\n"));
					int aKey = _getch();
					if(aKey == 'Q'){
						return; 
					}
					if(aKey == 'S'){
						break;
					}
				}
			}
		}
	}
}


/**
 * @brief ���C���֐�
 *
 * @param argc	[in]	�R�}���h���C���̃g�[�N�����i�H�j
 * @param argv	[in]	�R�}���h���C���̃g�[�N����i�H�j
 *
 * @retval 0		����I���i���͂����������Ă͂��Ȃ��j
 * @retval 0�ȊO	�ُ�I��
 */
int _tmain(int argc, _TCHAR* argv[])
{
	::setlocale(LC_ALL, "Japanese");
	const arctl::coinitializer coinit;

	//���s���o��
	CTimeLog aTimeLog;

	try {
		if(AnalyseArg(argc, argv) && GetConnectInfo()){	//���̌Ăԏ��Ԃ͏d�v�B�K�v�Ȋ��ϐ�����`����Ă��Ȃ���Usage���\���ł��Ȃ��Ȃ�̂�
			Execute();
		}
	}
	catch(const std::exception& e) {
		printf("ERROR,%s\n", e.what());
		fprintf(stderr, "#ERROR,%s\n", e.what());
		return 1;
	}
	return 0;
}
