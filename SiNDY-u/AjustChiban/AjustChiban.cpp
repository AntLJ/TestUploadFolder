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

// AjustChiban.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "AChiban.h"
#include "AdminTable.h"
#include <boost/format.hpp>
#include <sindy/workspace.h>
#include <arctl/coinitializer.h>
#include <WinLib/VersionInfo.h>

namespace {
#define __VERSION
#ifdef _DEBUG
LPCSTR DEF_ADR_CONNECT = "sindy2007a/sindy2007a/CityRep/5151/topaz2";
#else
LPCSTR DEF_ADR_CONNECT = "sindy/sindy/SDE.DEFAULT/5151/topaz";
#endif

LPCSTR DEF_GYOSEI = "City_Admin";
LPCSTR DEF_CHIBAN = "City_Rep_Point";

CString gAdrConnect = DEF_ADR_CONNECT;

CString gGyoseiFeature = DEF_GYOSEI;
CString gChibanFeature = DEF_CHIBAN;
LPCTSTR gConfigFile = NULL;
LPCTSTR gAddrFile = NULL;
CString gWhere = _T("");

LPCTSTR gAddrCode = NULL;
bool gAllFlag = false;
bool gChgMode = false;
}

void PrintUsage(LPCTSTR App)
{
	LPCTSTR AppName;
	AppName = _tcsrchr(App, _T('\\'));
	if(AppName != NULL){
		AppName++;
	}else{
		AppName = App;
	}
	_tprintf(_T("�n�Ԓ��L�����v���O�����i�����e�Łj\n")
			_T("Usage:%s [�I�v�V����] <�s�撬���R�[�h(5������11��)>\n")
			_T(" or   %s [�I�v�V����] -J <�Z���R�[�h���X�g>\n")
			_T(" or   %s [�I�v�V����] -A\n")
			_T(" �I�v�V�����F\n")
			_T("  -C <�ݒ�t�@�C��>\n")
			_T("  -G :�ύX���[�h�w��\n")
			, AppName, AppName, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	_tprintf(_T("�n�Ԓ��L�̍s���E���E�ł̏d�Ȃ���C�����܂��B\n")
			_T("�T�[�o��o�[�W������ύX����ɂ́A�ݒ�t�@�C�������ϐ��uDB_ADDRESS�v\n")
			_T("���g�p���ĉ������B\n")
			_T("�ݒ�t�@�C���̏������́AAjustChiban.txt�������������B\n")
			_T("�u-A�v�I�v�V�������g�p���A���ݒ�t�@�C�����ŏ������w�肵�Ȃ���΁A")
			_T("�s���E�f�[�^�Ɋ܂܂��S�Ă̎s�撬���R�[�h���ΏۂɂȂ�܂��B\n"));
}

bool AnalyseConfig()
{
	FILE *fp;
	_TCHAR Buf[256];
	bool aResult = true;
	LPTSTR p;
	if((fp = _tfopen(gConfigFile, _T("rt"))) != NULL){
		while(_fgetts(Buf, 256, fp)){
			if(Buf[0] != _T('#') && Buf[0] != _T('\n')){
				int aLen = _tcslen(Buf)-1;
				if(Buf[aLen] == _T('\n') || Buf[aLen] == _T('\r')){
					Buf[aLen] = '\0';
				}
				if(_tcsncmp(Buf, _T("DB_ADDRESS"), 10) == 0){
					if((p = _tcschr(Buf, _T('\t'))) != NULL){
						p++;
						gAdrConnect = p;
					}else{
						_ftprintf(stderr, _T("�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else if(_tcsncmp(Buf, _T("FC_CITY_ADMIN"), 13) == 0){
					if((p = _tcschr(Buf, '\t')) != NULL){
						p++;
						gGyoseiFeature = p;
					}else{
						fprintf(stderr, "#�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_tcsncmp(Buf, _T("FC_CITY_REP_POINT"), 17) == 0){
					if((p = _tcschr(Buf, '\t')) != NULL){
						p++;
						gChibanFeature = p;
					}else{
						_ftprintf(stderr, _T("#�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else if(_tcsncmp(Buf, _T("WHERE"), 5) == 0){
					if((p = _tcschr(Buf, '\t')) != NULL){
						p++;
						gWhere = p;
					}else{
						_ftprintf(stderr, _T("#�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else{
					_ftprintf(stderr, _T("#�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n"), Buf);
					aResult = false;
					break;
				}
			}
		}
		fclose(fp);
	}else{
		_ftprintf(stderr, _T("#�ݒ�t�@�C�����J���܂���:%s\n"), gConfigFile);
		aResult = false;
	}
	return aResult;
}

bool AnalyseArg(int argc, _TCHAR* argv[])
{
	if(argc < 2){
		PrintUsage(argv[0]);
		return false;
	}else{
		int i = 1;
		while(i < argc){
			if(argv[i][0] == '-' || argv[i][0] == '/'){
				switch(argv[i][1] ){
					case _T('?'):
						PrintHelp(argv[0]);
						return false;
						break;
					case _T('a'):
					case _T('A'):
						if(gAddrFile != NULL){
							PrintUsage(argv[0]);
							return false;
						}
						gAllFlag = true;
						break;
					case _T('c'):
					case _T('C'):
						i++;
						gConfigFile = argv[i];
						break;
					case _T('j'):
					case _T('J'):
						if(gAllFlag){
							PrintUsage(argv[0]);
							return false;
						}
						i++;
						gAddrFile = argv[i];
						break;
					case _T('g'):
					case _T('G'):
						gChgMode = true;
						break;
					default:
						_ftprintf(stderr, _T("#�s���ȃI�v�V�������w�肳��܂����B\n"));
						PrintUsage(argv[0]);
						return false;
						break;
				}
			}else{
				break;
			}
			i++;
		}
		if(i == argc-1){
			if(!gAllFlag && gAddrFile == NULL){
				gAddrCode = argv[i];
			}else{
				PrintUsage(argv[0]);
				return false;
			}
		}else if (i == argc){
			if(!gAllFlag && gAddrFile == NULL){
				PrintUsage(argv[0]);
				return false;
			}
		}else{
			PrintUsage(argv[0]);
			return false;
		}
		if(gConfigFile != NULL){
			return AnalyseConfig();
		}
	}
	return true;
}

bool GetEnv(void)
{
	LPCTSTR aEnv = _tgetenv(_T("DB_ADDRESS"));
	if(aEnv != NULL){
		gAdrConnect = aEnv;
	}
	aEnv = _tgetenv(_T("FC_CITY_ADMIN"));
	if(aEnv != NULL){
		gGyoseiFeature = aEnv;
	}
	aEnv = _tgetenv(_T("FC_CITY_REP_POINT"));
	if(aEnv != NULL){
		gChibanFeature = aEnv;
	}
	return true;
}

IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr )
{
	// �ڑ�
	_tprintf(_T("%s�ɐڑ���..."), lpszConnectStr);
	IWorkspacePtr ipWorkspace =  sindy::create_workspace(lpszConnectStr);
	if( NULL == ipWorkspace )
	{
//		_tprintf(_T("[%s]�ɐڑ��ł��Ȃ������x��\n") , lpszConnectStr);
		_ftprintf(stderr, _T("#[%s]�ɐڑ��ł��Ȃ������x��\n") , lpszConnectStr);
		throw std::runtime_error((boost::format("[%s]�ɐڑ��ł��Ȃ������x��") % CT2CA(lpszConnectStr).m_psz).str());
	}
	_tprintf(_T("�ڑ����܂���\n"));
	_ftprintf(stderr, _T("#%s�ɐڑ�\n"), sindy::workspace_address(ipWorkspace).c_str());
	return ipWorkspace;
}

bool MainteSingleChiban(AChiban &cChiban, CString& cAddrCode)
{
	_tprintf(_T("%s,���s���E�E�E\n"), cAddrCode);
	_ftprintf(stderr, _T("#%s�J�n\n"), cAddrCode);
	if(cChiban.execAjustChiban(cAddrCode)){
		if(gChgMode){
			_ftprintf(stderr, _T("#%s�ύX����\n"), cAddrCode);
		}else{
			_ftprintf(stderr, _T("#%s�ύX�̕K�v����\n"), cAddrCode);
		}
	}else{
		if(gChgMode){
			_ftprintf(stderr, _T("#%s�ύX�Ȃ�\n"), cAddrCode);
		}else{
			_ftprintf(stderr, _T("#%s�ύX�̕K�v�Ȃ�\n"), cAddrCode);
		}
	}
	cChiban.unInit();
	_ftprintf(stderr, _T("#%s�I��\n"), cAddrCode);
	_tprintf(_T("�I��\n"));
	fflush(stderr);
	return true;
}


void Execute()
{
	// �ڑ�
	IWorkspacePtr ipAdrWorkspace = ::ConnectSDE(gAdrConnect);
	ATLASSERT( ipAdrWorkspace != NULL );
	IFeatureWorkspacePtr ipAdrFeatureWorkspace(ipAdrWorkspace);
	ATLASSERT( ipAdrFeatureWorkspace != NULL );

	IFeatureClassPtr ipGyoseiClass, ipChibanClass;
	if(ipAdrFeatureWorkspace->OpenFeatureClass(CComBSTR(gGyoseiFeature), &ipGyoseiClass) != S_OK){
		_ftprintf(stderr, _T("#�t�B�[�`���[���J���Ȃ�,%s\n"), static_cast<LPCTSTR>(gGyoseiFeature));
		return;
	}
	if(ipAdrFeatureWorkspace->OpenFeatureClass(CComBSTR(gChibanFeature), &ipChibanClass) != S_OK){
		_ftprintf(stderr, _T("#�t�B�[�`���[���J���Ȃ�,%s\n"), static_cast<LPCTSTR>(gChibanFeature));
		return;
	}

	if(gChgMode){
		// �ҏW�J�n�iArcMap �̕ҏW�J�n�Ɠ����j
		IWorkspaceEditPtr ipWorkspaceEdit = ipAdrWorkspace;
		ipWorkspaceEdit->StartEditing( VARIANT_FALSE );	// Undo Redo ������Ȃ��Ȃ� VARIANT_FALSE
		ipWorkspaceEdit->StartEditOperation();
	}
	AChiban aChiban;
	if(aChiban.init(ipGyoseiClass, ipChibanClass, gChgMode)){
		if(gAllFlag){
			AdminTable aAdminTable;
			if(aAdminTable.init(ipAdrWorkspace, gGyoseiFeature, gWhere)){
				CString aAddrCode;
				while(aAdminTable.next(aAddrCode)){
					MainteSingleChiban(aChiban, aAddrCode);
					if(_kbhit()){
						int aKey = _getch();
						if(aKey == 'Q'){
							_tprintf(_T("�����͒��~����܂����B\n"));
							_ftprintf(stderr, _T("�����͒��~����܂����B\n"));
							break;
						}
					}
				}
			}
		}else if(gAddrFile != NULL){
			FILE *pFile;
			char aBuf[16];
			if((pFile = _tfopen(gAddrFile, _T("rt"))) != NULL){
				while(fgets(aBuf, 16, pFile) != NULL){
					if(aBuf[0] != '\n' && aBuf[0] != '#'){
						int aLen = strlen(aBuf) -1;
						if(aBuf[aLen] == '\n'){
							aBuf[aLen] = '\0';
						}
						MainteSingleChiban(aChiban, CString(aBuf));
					}
					if(_kbhit()){
						int aKey = _getch();
						if(aKey == 'Q'){
							_tprintf(_T("�����͒��~����܂����B\n"));
							_ftprintf(stderr, _T("�����͒��~����܂����B\n"));
							break;
						}
					}
				}
				fclose(pFile);
			}else{
				_ftprintf(stderr, _T("�t�@�C�����J���Ȃ��B:%s\n"), gAddrFile);
			}
		}else{
			MainteSingleChiban(aChiban, CString(gAddrCode));
		}
	}
	if(gChgMode){
		IWorkspaceEditPtr ipWorkspaceEdit = ipAdrWorkspace;
		ipWorkspaceEdit->StopEditOperation();
		if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_TRUE ))){	// �ҏW��ۑ�����Ȃ� VARIANT_TRUE �ɂ��邱��
			fprintf(stderr, "#�ۑ������B\n");	
		}else{
			fprintf(stderr, "#�ύX���ۑ��ł��Ȃ��B\n");	
		}
	}
	return ;
}

int _tmain(int argc, _TCHAR* argv[])
{
	::setlocale(LC_ALL, "Japanese");
	try {
		const arctl::coinitializer coinit;
		time_t st_time;
		time(&st_time);
		if(GetEnv()){
			if(AnalyseArg(argc, argv)){
				// �c�[���o�[�W����
				CVersion cVer;
				_ftprintf(stderr, _T("#%s FILEVERSION %s PRODUCTVERSION %s\n"), static_cast<LPCTSTR>(cVer.GetOriginalFilename()), static_cast<LPCTSTR>(cVer.GetFileVersion()), static_cast<LPCTSTR>(cVer.GetProductVersion()));
				SYSTEMTIME  aTime;
				GetLocalTime(&aTime);
				_ftprintf(stderr, _T("#�J�n����: %04d/%02d/%02d %02d:%02d:%02d\n"),
				aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
				aTime.wMinute, aTime.wSecond);

				Execute();

				time_t tm;
				time(&tm);
				GetLocalTime(&aTime);
				_ftprintf(stderr, _T("#�I������: %04d/%02d/%02d %02d:%02d:%02d\n"),
					aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
					aTime.wMinute, aTime.wSecond);
				int h,m,s;
				double t = difftime( tm, st_time);
				h = t/3600;
				m = (int(t)%3600)/60;
				s = int(t)%60;
				_tprintf(_T("�g�[�^������ %d����%d��%d�b\n"), h, m, s);
				_ftprintf(stderr, _T("#�g�[�^������ %d����%d��%d�b\n"), h, m, s);
				_tprintf(_T("�I�����܂�\n"));
			}
		}
	}
	catch(const std::exception& e) {
		printf("%s\n", e.what());
		fprintf(stderr, "#Error,%s\n", e.what());
	}
	return 0;
}
