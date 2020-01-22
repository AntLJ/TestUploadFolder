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

// AnnoICCheck.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "Anno.h"
#include <boost/format.hpp>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include <WinLib/VersionInfo.h>
#include <TDC/useful_headers/flush_scope_guard.h>

namespace {
	LPCSTR DEF_MAP_CONNECT = "SiNDY2008A/SiNDY2008A/SDE.Current08A/5151/ruby2";
	LPCSTR DEF_ANNO_CONNECT = "SiNDY2008A/SiNDY2008A/SDE.Current08A/5151/ruby2";
	LPCSTR DEF_ROD_CONNECT = "SiNDY2008/SiNDY2008/SDE.DEFAULT/5151/onyx";

	LPCSTR DEF_CITY_ANNO_CLASS = "City_Annotation";
	LPCSTR DEF_BASE_ANNO_CLASS = "Base_Annotation";
	LPCSTR DEF_MIDL_ANNO_CLASS = "Middle_Annotation";

	LPCSTR DEF_ROAD_CLASS = "Facil_Info_Point";

	LPCSTR DEF_MESH_CLASS = "Reference.CityMesh";
	LPCSTR DEF_SITE_CLASS = "City_Site";

	//#define DEF_RULE_FILE "Rule.txt"

	CString gMapConnect = DEF_MAP_CONNECT;
	CString gAnnoConnect = DEF_ANNO_CONNECT;
	CString gRodConnect = DEF_ROD_CONNECT;

	CString gMidlAnnoClass = DEF_MIDL_ANNO_CLASS;
	CString gBaseAnnoClass = DEF_BASE_ANNO_CLASS;
	CString gCityAnnoClass = DEF_CITY_ANNO_CLASS;
	CString gAnnoClass;
	CString gRoadClass = DEF_ROAD_CLASS;
	CString gMeshClass = DEF_MESH_CLASS;
	CString gSiteClass = DEF_SITE_CLASS;

	LPCTSTR gConfigFile = NULL;
	LPCTSTR gLogFile = NULL;
	//static CString gRuleFile = DEF_RULE_FILE;

	double gPower = 1.0;
	int gMode = -1;
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
	printf( "�������H�{��-���L�����`�F�b�N�v���O����\n"
		"Usage:%s [-C <�ݒ�t�@�C��>] [-P <臒l>] -{T|B|M} <���O�t�@�C��>\n"
			"\t-P <臒l>�́A�����͈͂����[�g���i�ߎ��l�j�Ŏw��B\n"
			"\t-T �s�s�n�}���[�h\n"
			"\t-B �x�[�X���[�h\n"
			"\t-M �~�h�����[�h\n"
			"\t�T�[�o�A�o�[�W��������ύX����ɂ́A���ϐ��uDB_MAP�v�uDB_ROAD�v��"
			"�g�p���ĉ������B\n"
			, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("�������H�{�݂ƒ��L�̐����`�F�b�N���s���܂��B\n"
		"�R�}���h���C���I�v�V�������A�ݒ�t�@�C�����̎w�肪�D�悳��܂��B\n"
		"�ݒ�t�@�C���̏������́AAnnoICChecktxt�������������B\n"
		);
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
				int aLen = strlen(Buf);
				if(Buf[aLen-1] == '\n' || Buf[aLen-1] == '\r'){
					Buf[aLen-1] = '\0';
				}
				if(strncmp(Buf, "DB_MAP", 6) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gMapConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "DB_ANNO", 7) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gAnnoConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "DB_ROAD", 7) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gRodConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_CITY_ANNOTATION", 18) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gCityAnnoClass = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_CITY_SITE", 12) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gSiteClass = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_MIDDLE_ANNOTATION", 20) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gMidlAnnoClass = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_BASE_ANNOTATION", 18) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gBaseAnnoClass = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_FACIL_INFO_POINT", 19) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gRoadClass = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_CITYMESH", 11) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gMeshClass = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "POWER", 5) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gPower = atof(p);
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
	if(argc < 3){
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
					case 'p':
					case 'P':
						i++;
						gPower = atof(argv[i]);
						break;
					case 't':
					case 'T':
						if(gMode == -1){
							gMode = 2;
							gAnnoClass = gCityAnnoClass;
						}else{
							fprintf(stderr, "�I�v�V����T,B,M�͓����Ɏw��͂ł��܂���B\n");
							PrintUsage(argv[0]);
							return false;
						}
						break;
					case 'b':
					case 'B':
						if(gMode == -1){
							gMode = 1;
							gAnnoClass = gBaseAnnoClass;
						}else{
							fprintf(stderr, "�I�v�V����T,B,M�͓����Ɏw��͂ł��܂���B\n");
							PrintUsage(argv[0]);
							return false;
						}
						break;
					case 'm':
					case 'M':
						if(gMode == -1){
							gMode = 0;
							gAnnoClass = gMidlAnnoClass;
						}else{
							fprintf(stderr, "�I�v�V����T,B,M�͓����Ɏw��͂ł��܂���B\n");
							PrintUsage(argv[0]);
							return false;
						}
						break;
					case '?':
						PrintHelp(argv[0]);
						return false;
						break;
					default:
						fprintf(stderr, "�s���ȃI�v�V�������w�肳��܂����B[%c]\n", argv[i][1]);
						PrintUsage(argv[0]);
						return false;
						break;
				}
			}else{
				break;
			}
			i++;
		}
		if(gMode == -1){
			fprintf(stderr, "�I�v�V����T,B,M�̂����ꂩ���w�肵�ĉ������B\n");
			PrintUsage(argv[0]);
			return false;
		}
		if(i == argc -1){
			gLogFile = argv[i];
		}else{
			PrintUsage(argv[0]);
			return false;
		}
	}
	return AnalyseConfig();
}

bool GetEnv(void)
{
	char *aEnv = getenv(_T("DB_MAP"));
	if(aEnv != NULL){
		gMapConnect = aEnv;
	}
	aEnv = getenv(_T("DB_ANNO"));
	if(aEnv != NULL){
		gAnnoConnect = aEnv;
	}
	aEnv = getenv(_T("DB_ROAD"));
	if(aEnv != NULL){
		gRodConnect = aEnv;
	}
	aEnv = getenv(_T("FC_CITY_ANNOTATION"));
	if(aEnv != NULL){
		gCityAnnoClass = aEnv;
	}
	aEnv = getenv(_T("FC_BASE_ANNOTATION"));
	if(aEnv != NULL){
		gBaseAnnoClass = aEnv;
	}
	aEnv = getenv(_T("FC_MIDDLE_ANNOTATION"));
	if(aEnv != NULL){
		gMidlAnnoClass = aEnv;
	}
	aEnv = getenv(_T("FC_FACIL_INFO_POINT"));
	if(aEnv != NULL){
		gRoadClass = aEnv;
	}
	aEnv = getenv(_T("FC_CITYMESH"));
	if(aEnv != NULL){
		gMeshClass = aEnv;
	}
	aEnv = getenv(_T("FC_CITY_SITE"));
	if(aEnv != NULL){
		gSiteClass = aEnv;
	}
	aEnv = getenv(_T("POWER"));
	if(aEnv != NULL){
		gPower = atof(aEnv);
	}
	return true;
}

IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr, FILE *pFile)
{
	// �ڑ�
	INamePtr ipName( sindy::create_workspace_name(lpszConnectStr) );
	if( !ipName )
	{
		throw std::runtime_error((boost::format("[%s]�̍\�����ς��x��") % CT2CA(lpszConnectStr).m_psz).str());
	}

	IWorkspacePtr ipWorkspace;
	for(int i = 0; i < 5; ++i){
		_tprintf(_T("%s�ɐڑ���(%d���)..."), lpszConnectStr, i+1);
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		if(ipUnknown){
			ipWorkspace = ipUnknown;
			break;
		}else{
			printf("���s\n");
			Sleep(2000);
		}
	}
	if( !ipWorkspace )
	{
		fprintf(pFile, "#Error,%s�ɐڑ��ł��Ȃ������x��\n", lpszConnectStr);
		fclose(pFile);
		throw std::runtime_error((boost::format("[%s]�ɐڑ��ł��Ȃ������x��") % CT2CA(lpszConnectStr).m_psz).str());
	}
	printf("�ڑ����܂���\n");
	fprintf(stderr, "#%s�ɐڑ�\n", static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz);
//	std::cout << "�ڑ��� : " << static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz << std::endl;
	return ipWorkspace;
}

void Execute()
{
	time_t st_time;
	time(&st_time);
	SYSTEMTIME  aTime;
	GetLocalTime(&aTime);

	FILE *pFile;
	if((pFile = fopen(gLogFile, "wt")) == NULL){
		fprintf(stderr, "#Error,�o�̓t�@�C�����J���Ȃ��B,%s\n", gLogFile);
		return;
	}
	fprintf(pFile, "# SINDYSTDLOG\n");
	// �c�[���o�[�W����
	CVersion cVer;
	fprintf(stderr, "#%s FILEVERSION %s PRODUCTVERSION %s\n",
		static_cast<LPCSTR>(cVer.GetOriginalFilename()), static_cast<LPCSTR>(cVer.GetFileVersion()), static_cast<LPCSTR>(cVer.GetProductVersion()));
	fprintf(pFile, "#%s FILEVERSION %s PRODUCTVERSION %s\n",
		static_cast<LPCSTR>(cVer.GetOriginalFilename()), static_cast<LPCSTR>(cVer.GetFileVersion()), static_cast<LPCSTR>(cVer.GetProductVersion()));
	fprintf(stderr, "#�J�n����: %04d/%02d/%02d %02d:%02d:%02d\n",
	aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
	aTime.wMinute, aTime.wSecond);
	fprintf(pFile, "#�J�n����: %04d/%02d/%02d %02d:%02d:%02d\n",
	aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
	aTime.wMinute, aTime.wSecond);

	// �ڑ�
	//�n�}�n�T�[�o
	IWorkspacePtr ipMapWorkspace = ::ConnectSDE(gMapConnect, pFile);

	// ���L�n�T�[�o
	IWorkspacePtr ipAnnoWorkspace = ::ConnectSDE(gAnnoConnect, pFile);

	//���H�E���k�n�T�[�o
	IWorkspacePtr ipRodWorkspace = ::ConnectSDE(gRodConnect, pFile);


	//���ۂ̏���
	Anno aAnno(ipMapWorkspace, ipAnnoWorkspace, ipRodWorkspace);
	if(aAnno.init(pFile, gAnnoClass, gRoadClass, gPower, gMode)){
		if(gMode == 2){
			if(!aAnno.setCityArea(gMeshClass, gSiteClass)){
				return;
			}
		}
		aAnno.execCheck();
	}

	time_t tm;
	time(&tm);
	GetLocalTime(&aTime);
	fprintf(stderr, "#�I������: %04d/%02d/%02d %02d:%02d:%02d\n",
		aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
		aTime.wMinute, aTime.wSecond);
	fprintf(pFile, "#�I������: %04d/%02d/%02d %02d:%02d:%02d\n",
		aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
		aTime.wMinute, aTime.wSecond);
	printf( " %6.0f�b\n", difftime( tm, st_time));
	fprintf(stderr, "#�g�[�^������%6.0f�b\n", difftime( tm, st_time));
	fprintf(pFile, "#�g�[�^������%6.0f�b\n", difftime( tm, st_time));
//	printf( " %6.0f�b\nPush any key!\n", difftime( tm, st_time));
	fclose(pFile);
	return;
}

int main(int argc, char* argv[])
{
	// �X�R�[�v�𔲂���Ƃ��ɋ����I��flush����
	uh::flush_scope_guard fsg;
	try {
		const arctl::coinitializer coinit;
		GetEnv();
		if(AnalyseArg(argc, argv)){

			Execute();

		}
	}
	catch(const std::exception& e) {
		printf("%s\n", e.what());
		fprintf(stderr, "#Error,%s\n", e.what());
	}
	return 0;
}


