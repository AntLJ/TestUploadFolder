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

// CalcAdminArea.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include <boost/format.hpp>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include "CalcArea.h"

#ifdef _DEBUG
#define DEF_ADR_CONNECT "sindy2007a/sindy2007a/SDE.Current07A/5151/topaz2"
//#define DEF_ADR_CONNECT "prodmap/prodmap/SDE.DEFAULT/5151/ruby"
//#define DEF_ADR_CONNECT "FIX200401/FIX200401/SDE.DEFAULT/5151/topaz"
#else
#define DEF_ADR_CONNECT "sindy/sindy/SDE.DEFAULT/5151/topaz"
#endif

#ifdef __FOR_BT__
#define DEF_EXT_CONNECT "techmap/techmap/SDE.DEFAULT/5151/spinel"
#define DEF_EXT_LAYER "BT_Ext_Area"
#endif

#define DEF_GYOSEI "City_Admin"
//#define DEF_GYOSEI "DinaOlthoArea050530"
//#define DEF_MESH_INDEX "Reference.CityMesh"
#define DEF_OUT_DIR "."

static CString gAdrConnect = DEF_ADR_CONNECT;

static CString gGyoseiFeature = DEF_GYOSEI;

static CString gOutDir = DEF_OUT_DIR;
static LPCTSTR gConfigFile = NULL;
static bool gSpecialFlag = false;
static bool gAllFlag = false;
static LPCTSTR gAddrCode = NULL;

void PrintUsage(LPCTSTR App)
{
	LPCTSTR AppName;
	AppName = strrchr(App, '\\');
	if(AppName != NULL){
		AppName++;
	}else{
		AppName = App;
	}
	printf("�s�撬���ʖʐόv�Z�v���O����\n"
			"Usage:%s [Options]\n"
			" or   %s [Options] <�s���{���R�[�h(2��)>\n"
			" Options:\n"
			"\t-C <�ݒ�t�@�C��>\n"
			"\t-O <�o�̓f�B���N�g��>:(�f�t�H���g�F%s)\n"
			"\t-S:�����s�ȊO�̐��ߎw��s�s�́u�s�v��1�Ƃ��Čv�Z���܂��B\n"
			"\t-A:�s�s�n�}�G���A�O�ɂ��Ă��v�Z���܂��B\n"
			, AppName, AppName, DEF_OUT_DIR);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("�s���E�|���S�������ɁA�ʐς��v�Z���܂��B\n"
			"�ݒ�t�@�C���̏������́ACalcAdminArea.txt�������������B\n"
			"�s���{���R�[�h���w�肵�Ȃ���΁A�S�Ă̓s���{�����v�Z���܂��B\n"
			);
}

bool AnalyseConfig()
{
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
				if(strncmp(Buf, "DB_ADDRESS", 10) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gAdrConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_CITY_ADMIN", 13) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gGyoseiFeature = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "OUTDIR", 6) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gOutDir = p;
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
	if(argc < 1){
		PrintUsage(argv[0]);
		return false;
	}else{
		int i = 1;
		while(i < argc){
			if(argv[i][0] == '-' || argv[i][0] == '/'){
				switch(argv[i][1] ){
					case '?':
						PrintHelp(argv[0]);
						return false;
						break;
					case 'a':
					case 'A':
						gAllFlag = true;
						break;
					case 's':
					case 'S':
						gSpecialFlag = true;
						break;
					case 'c':
					case 'C':
						i++;
						gConfigFile = argv[i];
						break;
					case 'o':
					case 'O':
						i++;
						gOutDir = argv[i];
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
		if(i == argc-1){
			gAddrCode = argv[i];
			if(strlen(gAddrCode) != 2){
				PrintUsage(argv[0]);
				return false;
			}
		}else if(i == argc){
			printf("�S���̌v�Z���s���܂��B�����܂����H(Y/N/?)\n");
			int aKey = _getch();
			aKey = toupper( aKey );
			while(aKey != 'Y' && aKey != 'N' && aKey != '?'){
				aKey = _getch();
				aKey = toupper( aKey );
			}
			if(aKey == 'N'){
				return false;
			}else if(aKey == '?'){
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
	char *aEnv = getenv(_T("DB_ADDRESS"));
	if(aEnv != NULL){
		gAdrConnect = aEnv;
	}
	aEnv = getenv(_T("FC_CITY_ADMIN"));
	if(aEnv != NULL){
		gGyoseiFeature = aEnv;
	}
	return true;
}


IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr )
{
	// �ڑ�
	INamePtr ipName( sindy::create_workspace_name(lpszConnectStr) );
	if( NULL == ipName )
	{
		throw std::runtime_error((boost::format("[%s]�̍\�����ς��x��") % CT2CA(lpszConnectStr).m_psz).str());
	}

	printf("%s�ɐڑ���...", lpszConnectStr);
	IUnknownPtr ipUnknown;
	ipName->Open( &ipUnknown );
	IWorkspacePtr ipWorkspace = ipUnknown;
	if( NULL == ipWorkspace )
	{
		throw std::runtime_error((boost::format("[%s]�ɐڑ��ł��Ȃ������x��") % CT2CA(lpszConnectStr).m_psz).str());
	}
	printf("�ڑ����܂���\n");
	fprintf(stderr, "#%s�ɐڑ�\n", static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz);
//	std::cout << "�ڑ��� : " << static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz << std::endl;
	return ipWorkspace;
}

void Execute()
{
	// �ڑ�
	IWorkspacePtr ipWorkspace = ::ConnectSDE( _T(gAdrConnect) );
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	ATLASSERT( ipFeatureWorkspace != NULL );
#ifdef __FOR_BT__
	IWorkspacePtr ipExtWorkspace = ::ConnectSDE( _T(DEF_EXT_CONNECT) );
	if( ipExtWorkspace == NULL ){
		return;
	}
	IFeatureWorkspacePtr ipExtFeatureWorkspace(ipExtWorkspace);
	ATLASSERT( ipExtFeatureWorkspace != NULL );
	IFeatureClassPtr ipExtClass;

	if(ipExtFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(DEF_EXT_LAYER)), &ipExtClass) != S_OK){
		fprintf(stderr, "�t�B�[�`���[���J���Ȃ�,%s\n", _T(DEF_EXT_LAYER));
		return;
	}
	
#endif
	IFeatureClassPtr ipGyoseiClass;

	if(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(gGyoseiFeature), &ipGyoseiClass) != S_OK){
		fprintf(stderr, "�t�B�[�`���[���J���Ȃ�,%s\n", static_cast<LPCTSTR>(gGyoseiFeature));
		return;
	}

	CalcArea aCalcArea;
#ifdef __FOR_BT__
	if(aCalcArea.init(ipGyoseiClass, ipExtClass, gOutDir, gAllFlag, gSpecialFlag)){
#else
	if(aCalcArea.init(ipGyoseiClass, gOutDir, gAllFlag, gSpecialFlag)){
#endif
		if(gAddrCode == NULL){
			int i;
			for(i = 1; i < 48; i++){
				aCalcArea.execCalcArea(i);
			}
		}else{
			aCalcArea.execCalcArea(atoi(gAddrCode));
		}
	}
	return ;
}

int main(int argc, char* argv[])
{
	try {
		const arctl::coinitializer coinit;
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
}

