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

// AddColorPGDB.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "AdmColor.h"
#include <boost/format.hpp>
#include <arctl/coinitializer.h>

namespace {
LPCSTR DEF_BASE_DIR = ".\\";

LPCSTR DEF_CITY_ADMIN = "City_Admin";
CString gConnect = DEF_BASE_DIR;
CString gAdmLayer = DEF_CITY_ADMIN;
LPCTSTR gConfigFile = NULL;
LPCTSTR gListFile = NULL;
CString gTableFile = ".\\ColorTable.txt";

bool gChgMode = false;
}

void PrintUsage(LPCTSTR App)
{
	LPCTSTR AppName;
	AppName = strrchr(App, '\\');
	if(AppName != NULL){
		AppName++;
	}else{
		AppName = App;
	}
	printf("�X��F�p�s���E�F�t���v���O����(�e�[�u������t�^PGDB��)\n"
			"�f�B���N�g���̕ύX�́A���ϐ��uDB_ADDRESS�v���͐ݒ�t�@�C�����g�p���ĉ������B\n"
			"Usage:%s [�I�v�V����] <���b�V�����X�g>\n"
			" <�I�v�V����>\n"
			"   -S:�ύX���[�h�w��\n"
			"   -C <�ݒ���t�@�C��>\n"
			" �@-T <�J���[�e�[�u���t�@�C��>\n"
			"�@�f�t�H���g�ł́A�`�F�b�N�݂̂ł��B\n"
			, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("���b�V�����X�g�ƃJ���[�e�[�u������X��F�p�̐F���s���E�ɕt�^���܂��B\n"
			"�ݒ�t�@�C���̏������́AAddColorPGDB.txt�������������B\n");
}

bool AnalyseConfig()
{
	if(gConfigFile == NULL){
		return true;
	}
	FILE *fp;
	char Buf[256];
	bool aResult = true;
	char *p;
	if((fp = fopen(gConfigFile, "rt")) != NULL){
		while(fgets(Buf, 256, fp)){
			if(Buf[0] != '#' && Buf[0] != '\n'){
				int aLen = strlen(Buf)-1;
				if(Buf[aLen] == '\n' || Buf[aLen] == '\r'){
					Buf[aLen] = '\0';
				}
				if(strncmp(Buf, "DB_ADDRESS", 10) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_CITY_ADMIN", 13) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gAdmLayer = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "COLOR_TABLE", 11) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gTableFile = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else{
					fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
					aResult = false;
					break;
				}
			}
		}
		fclose(fp);
	}else{
		fprintf(stderr, "�ݒ�t�@�C�����J���܂���:%s\n", gConfigFile);
		aResult = false;
	}
	return aResult;
}

bool AnalyseArg(int argc, char* argv[])
{
	if(argc < 2){
		PrintUsage(argv[0]);
		return false;
	}else{
		int i = 1;
		while(i < argc){
			if(argv[i][0] == '-' || argv[i][0] == '/'){
				switch(argv[i][1] ){
					case 'c':
					case 'C':
						i++;
						gConfigFile = argv[i];
						break;
					case 't':
					case 'T':
						i++;
						gTableFile = argv[i];
						break;
					case 's':
					case 'S':
						gChgMode = true;
						break;
					case '?':
						PrintHelp(argv[0]);
						return false;
						break;
					default:
						fprintf(stderr, "�s���ȃI�v�V�������w�肳��܂����B\n");
						PrintUsage(argv[0]);
						return false;
						break;
				}
			}else{
				break;
			}
			i++;
		}
		if(i == argc -1){
			gListFile = argv[i];
		}else{
			PrintUsage(argv[0]);
			return false;
		}
	}
	return AnalyseConfig();
}

bool GetEnv(void)
{
	char *aEnv = getenv(_T("DB_ADDRESS"));
	if(aEnv != NULL){
		gConnect = aEnv;
	}
	aEnv = getenv(_T("FC_CITY_ADMIN"));
	if(aEnv != NULL){
		gAdmLayer = aEnv;
	}
	aEnv = getenv(_T("COLOR_TABLE"));
	if(aEnv != NULL){
		gTableFile = aEnv;
	}
	return true;
}


void Execute()
{
	if(gChgMode){
		printf("�ύX���[�h�œ��삵�܂��B\n");
	}else{
		printf("�`�F�b�N���[�h�œ��삵�܂��B\n");
	}
	AdmColor aAdm;
	if(aAdm.init(gConnect, gAdmLayer, gTableFile, gChgMode)){
		FILE *pFile;
		if((pFile = fopen(gListFile, "rt")) != NULL){
			char aBuf[16];
			while(fgets(aBuf, 16, pFile) != NULL){
				if(aBuf[0] != '#' && aBuf[0] != '\n'){
					aAdm.addColor(atol(aBuf));
				}
			}
			fclose(pFile);
		}else{
			throw std::runtime_error((boost::format("���b�V�����X�g���J���܂���B,%s") % CT2CA(gListFile).m_psz).str());
		}
	}
}

int main(int argc, char* argv[])
{
	const arctl::coinitializer coinit;
	try {
		time_t st_time;
		time(&st_time);
		GetEnv();
		if(AnalyseArg(argc, argv)){
			SYSTEMTIME  aTime;
			GetLocalTime(&aTime);
			fprintf(stderr, "#�J�n����: %04d/%02d/%02d %02d:%02d:%02d\n",
			aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
			aTime.wMinute, aTime.wSecond);

			Execute();

			time_t tm;
			time(&tm);
			GetLocalTime(&aTime);
			fprintf(stderr, "#�I������: %04d/%02d/%02d %02d:%02d:%02d\n",
				aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
				aTime.wMinute, aTime.wSecond);
			printf( " %6.0f�b\n", difftime( tm, st_time));
			fprintf(stderr, "#�g�[�^������%6.0f�b\n", difftime( tm, st_time));
			printf(_T("�I�����܂�"));
		}
	}
	catch(const std::exception& e) {
		printf("%s\n", e.what());
		fprintf(stderr, "#Error,%s\n", e.what());
	}
	return 0;
}
