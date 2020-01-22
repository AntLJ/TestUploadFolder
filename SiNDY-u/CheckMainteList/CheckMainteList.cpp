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

// CheckMainteList.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <WinLib/VersionInfo.h>
#include "Check.h"

namespace {
	CString gOldDir = _T(".\\");
	CString gNewDir = _T(".\\");
	CString gMMaster = _T("");
	LPCTSTR gFileName = NULL;
	LPCTSTR gListFile = NULL;
	LPCTSTR gLogFile = NULL;
	int gMode = 0;
}

void PrintUsage(LPCTSTR App)
{
	LPCTSTR AppName;
	AppName = _tcsrchr(App, '\\');
	if(AppName != NULL){
		AppName++;
	}else{
		AppName = App;
	}
	_tprintf(_T("�����e���X�g���l�`�F�b�N�v���O����\n")
			_T("Usage:%s [�I�v�V����] -F <��r�t�@�C����>\n")
			_T("      %s [�I�v�V����] -L <���X�g�t�@�C����>\n")
			_T("[�I�v�V����]\n")
			_T("    -O <�ҏW�O�f�[�^�f�B���N�g��>\n")
			_T("    -N <�ҏW��f�[�^�f�B���N�g��>\n")
			_T("    -E <�G���[���O�t�@�C��>\n")
			_T("    -G :GS�c�Ǝ��ԃ��[�h\n")
			_T("    -M <���o�}�X�^>\n")
			, AppName, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	_tprintf(_T("�����e���X�g�̌��l�`�F�b�N�����܂�\n")
			);
}

bool AnalyseArg(int argc, _TCHAR* argv[])
{
	if(argc < 3){
		PrintUsage(argv[0]);
		return false;
	}else{
		int i = 1;
		while(i < argc){
			if(argv[i][0] == '-' || argv[i][0] == '/'){
				switch(argv[i][1] ){
					case 'o':
					case 'O':
						i++;
						gOldDir = argv[i];
						break;
					case 'n':
					case 'N':
						i++;
						gNewDir = argv[i];
						break;
					case 'f':
					case 'F':
						i++;
						gFileName = argv[i];
						break;
					case 'l':
					case 'L':
						i++;
						gListFile = argv[i];
						break;
					case 'e':
					case 'E':
						i++;
						gLogFile = argv[i];
						break;
					case 'm':
					case 'M':
						i++;
						gMMaster = argv[i];
						break;
					case 'g':
					case 'G':
						gMode = 1;
						break;
					case '?':
						PrintHelp(argv[0]);
						return false;
						break;
					default:
						_ftprintf(stderr, _T("�s���ȃI�v�V�������w�肳��܂����B\n"));
						PrintUsage(argv[0]);
						return false;
						break;
				}
			}else{
				break;
			}
			i++;
		}
		if(i != argc || (gListFile == NULL && gFileName == NULL)){
			PrintUsage(argv[0]);
			return false;
		}
	}
	return true;
}

void GetEnv(void)
{
}

int _tmain(int argc, _TCHAR* argv[])
{
	::setlocale(LC_ALL, "Japanese");
	time_t st_time;
	time(&st_time);
	GetEnv();
	if(AnalyseArg(argc, argv)){
		// �c�[���o�[�W����
		CVersion cVer;
		_ftprintf(stderr, _T("#%s FILEVERSION %s PRODUCTVERSION %s\n"), static_cast<LPCTSTR>(cVer.GetOriginalFilename()), static_cast<LPCTSTR>(cVer.GetFileVersion()), static_cast<LPCTSTR>(cVer.GetProductVersion()));
		SYSTEMTIME  aTime;
		GetLocalTime(&aTime);
		_ftprintf(stderr, _T("#�J�n����: %04d/%02d/%02d %02d:%02d:%02d\n"),
		aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
		aTime.wMinute, aTime.wSecond);
		// COM�̏�����
//		::CoInitialize(NULL);
		CCheck aCheck;
		if(aCheck.init(gOldDir, gNewDir, gLogFile, gMode, gMMaster)){
			if(gListFile){
				FILE *pFile;
				if((pFile = _tfopen(gListFile, _T("rt"))) != NULL){
					_TCHAR aBuf[256];
					while(_fgetts(aBuf, 256, pFile) != NULL){
						if(aBuf[0] != _T('#') && aBuf[0] != _T('\n')){
							aBuf[_tcslen(aBuf) -1] = _T('\0');
							aCheck.execCheck(aBuf);
						}
					}
					fclose(pFile);
				}
			}else{
				aCheck.execCheck(gFileName);
			}
		}
//		::CoUninitialize();
		time_t tm;
		time(&tm);
		GetLocalTime(&aTime);
		_ftprintf(stderr, _T("#�I������: %04d/%02d/%02d %02d:%02d:%02d\n"),
		aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
		aTime.wMinute, aTime.wSecond);
		_tprintf(_T(" %6.0f�b\n"), difftime( tm, st_time));
		_ftprintf(stderr, _T("#�g�[�^������%6.0f�b\n"), difftime( tm, st_time));
		_tprintf(_T("�I�����܂�\n"));
	}
	return 0;

}
