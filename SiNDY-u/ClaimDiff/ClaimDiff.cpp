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

// ClaimDiff.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "Diff.h"
#include <boost/format.hpp>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include <WinLib/VersionInfo.h>
#include <TDC/useful_headers/str_util.h>
#include <boost/assign.hpp>

namespace {
	using namespace boost::assign;
#ifdef _DEBUG
	LPCSTR DEF_MAP_CONNECT = "trial/trial/TRIAL.ETC_ROADCHECK_matsumoto/5151/coral2";
	LPCSTR DEF_ANO_CONNECT = "trial/trial/TRIAL.ETC_ROADCHECK_matsumoto/5151/coral2";
	LPCSTR DEF_BLD_CONNECT = "trial/trial/TRIAL.ETC_ROADCHECK_matsumoto/5151/coral2";
	LPCSTR DEF_BAS_CONNECT = "trial/trial/TRIAL.ETC_ROADCHECK_matsumoto/5151/coral2";
	LPCSTR DEF_ROD_CONNECT = "trial/trial/TRIAL.ETC_ROADCHECK_matsumoto/5151/coral2";
	LPCSTR DEF_CLM_CONNECT = "claim/claim/SDE.DEFAULT/5151/spinel2";
	LPCSTR DEF_OLD_MAP_CONNECT = "trial/trial/TRIAL.ETC_ROADCHECK_matsumoto/5151/coral2";
	LPCSTR DEF_OLD_ANO_CONNECT = "SiNUP2014A/SiNUP2014A/SiNUP2014A.ETC_SINUP2014A_RoadNumNote_Chk/5151/arion";
	LPCSTR DEF_OLD_BLD_CONNECT = "trial/trial/TRIAL.ETC_ROADCHECK_matsumoto/5151/coral2";
	LPCSTR DEF_OLD_BAS_CONNECT = "trial/trial/TRIAL.ETC_ROADCHECK_matsumoto/5151/coral2";
	LPCSTR DEF_OLD_ROD_CONNECT = "trial/trial/TRIAL.ETC_ROADCHECK_matsumoto/5151/coral2";
	std::set<CString> DEF_CHECK_FEATURES = list_of(_T("CITY_ANNOTATION"))(_T("CITY_DISP_LINE"));
#else
	LPCSTR DEF_MAP_CONNECT;
	LPCSTR DEF_ANO_CONNECT;
	LPCSTR DEF_BLD_CONNECT;
	LPCSTR DEF_BAS_CONNECT;
	LPCSTR DEF_ROD_CONNECT;
	LPCSTR DEF_CLM_CONNECT;
	LPCSTR DEF_OLD_MAP_CONNECT;
	LPCSTR DEF_OLD_ANO_CONNECT;
	LPCSTR DEF_OLD_BLD_CONNECT;
	LPCSTR DEF_OLD_BAS_CONNECT;
	LPCSTR DEF_OLD_ROD_CONNECT;
	std::set<CString> DEF_CHECK_FEATURES;
#endif

	CString gMapConnect = DEF_MAP_CONNECT;
	CString gAnoConnect = DEF_ANO_CONNECT;
	CString gBldConnect = DEF_BLD_CONNECT;
	CString gBasConnect = DEF_BAS_CONNECT;
	CString gRodConnect = DEF_ROD_CONNECT;
	CString gClmConnect = DEF_CLM_CONNECT;
	CString gOldMapConnect = DEF_OLD_MAP_CONNECT;
	CString gOldAnoConnect = DEF_OLD_ANO_CONNECT;
	CString gOldBldConnect = DEF_OLD_BLD_CONNECT;
	CString gOldBasConnect = DEF_OLD_BAS_CONNECT;
	CString gOldRodConnect = DEF_OLD_ROD_CONNECT;
	std::set<CString> gCheckFeatures = DEF_CHECK_FEATURES;

	LPCTSTR gConfigFile = NULL;
	LPCTSTR gOutFile = NULL;
	LPCTSTR gExtFile = NULL;
	bool gOutputAll = false;	// ���������ׂďo�͂��邩�ǂ��� [Bug 10390]�����̂������S�ẴI�u�W�F�N�g�̍�����񂪏o�͂����I�v�V�������~����

}
void PrintUsage(LPCTSTR App)
{
	LPCTSTR AppName;
	AppName = strrchr(App, '\\');
	if(AppName != NULL){
		++AppName;
	}else{
		AppName = App;
	}
	printf( "�N���[���|���S���������`�F�b�N�v���O����\n"
		    "Usage:%s [-C <�ݒ�t�@�C��>] [-E <��OID���X�g�t�@�C��>] <�o�̓t�@�C��>\n"
			"\t�T�[�o�A�o�[�W��������ύX����ɂ́A���ϐ����͐ݒ�t�@�C����"
			"�g�p���ĉ������B\n"
			, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("�N���[���|���S�����̃I�u�W�F�N�g�ɕύX�����������ǂ����̃`�F�b�N���s���܂��B\n"
		"�R�}���h���C���I�v�V�������A�ݒ�t�@�C�����̎w�肪�D�悳��܂��B\n"
		"�ݒ�t�@�C���̏������́AClaimDiff.txt�������������B\n");
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
				if(_strnicmp(Buf, "DB_MAP", 6) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gMapConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_strnicmp(Buf, "DB_ANNO", 7) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gAnoConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_strnicmp(Buf, "DB_BLD", 6) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gBldConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_strnicmp(Buf, "DB_BASE", 7) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gBasConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_strnicmp(Buf, "DB_ROAD", 7) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gRodConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_strnicmp(Buf, "DB_CLAIM", 8) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gClmConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_strnicmp(Buf, "DB_OLD_MAP", 10) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gOldMapConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_strnicmp(Buf, "DB_OLD_ANNO", 11) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gOldAnoConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_strnicmp(Buf, "DB_OLD_BLD", 10) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gOldBldConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_strnicmp(Buf, "DB_OLD_BASE", 11) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gOldBasConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_strnicmp(Buf, "DB_OLD_ROAD", 11) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gOldRodConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_strnicmp(Buf, "CHECK_FEATURES", 14) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						std::vector<CString> vecCheckFeatures = uh::str_util::split(CString(p), _T(","));
						for(const auto& feature : vecCheckFeatures)
							gCheckFeatures.insert(CString(feature).MakeUpper());
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
						++i;
						gConfigFile = argv[i];
						break;
					case 'e':
					case 'E':
						++i;
						gExtFile = argv[i];
						break;
					// [Bug 10390]�����̂������S�ẴI�u�W�F�N�g�̍�����񂪏o�͂����I�v�V�������~����
					// ���������ׂďo�͂��邩�̃I�v�V����
					case 'o':
					case 'O':
						gOutputAll = true;
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
			++i;
		}
		if(i == argc -1){
				gOutFile = argv[i];
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
	if(aEnv){
		gMapConnect = aEnv;
	}
	aEnv = getenv(_T("DB_ANNO"));
	if(aEnv){
		gAnoConnect = aEnv;
	}
	aEnv = getenv(_T("DB_BLD"));
	if(aEnv){
		gBldConnect = aEnv;
	}
	aEnv = getenv(_T("DB_BASE"));
	if(aEnv){
		gBasConnect = aEnv;
	}
	aEnv = getenv(_T("DB_ROAD"));
	if(aEnv){
		gRodConnect = aEnv;
	}
	aEnv = getenv(_T("DB_CLAIM"));
	if(aEnv){
		gClmConnect = aEnv;
	}
	aEnv = getenv(_T("DB_OLD_MAP"));
	if(aEnv){
		gOldMapConnect = aEnv;
	}
	aEnv = getenv(_T("DB_OLD_ANNO"));
	if(aEnv){
		gOldAnoConnect = aEnv;
	}
	aEnv = getenv(_T("DB_OLD_BLD"));
	if(aEnv){
		gOldBldConnect = aEnv;
	}
	aEnv = getenv(_T("DB_OLD_BASE"));
	if(aEnv){
		gOldBasConnect = aEnv;
	}
	aEnv = getenv(_T("DB_OLD_ROAD"));
	if(aEnv){
		gOldRodConnect = aEnv;
	}
	aEnv = getenv(_T("CHECK_FEATURES"));
	if(aEnv){
		std::vector<CString> vecCheckFeatures = uh::str_util::split(CString(aEnv), _T(","));
		for(const auto& feature : vecCheckFeatures)
			gCheckFeatures.insert(CString(feature).MakeUpper());
	}
	return true;
}

IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr, FILE *pFile)
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
	fprintf(pFile, "#%s�ɐڑ�\n", static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz);
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
	if((pFile = fopen(gOutFile, "wt")) == NULL){
		fprintf(stderr, "#Error,�o�̓t�@�C�����J���Ȃ��B,%s\n", gOutFile);
		return;
	}
	// �c�[���o�[�W����
	CVersion cVer;
	fprintf(stderr, "#%s FILEVERSION %s PRODUCTVERSION %s\n", static_cast<LPCSTR>(cVer.GetOriginalFilename()), static_cast<LPCSTR>(cVer.GetFileVersion()), static_cast<LPCSTR>(cVer.GetProductVersion()));
	fprintf(pFile, "# SINDYSTDLOG\n");
	fprintf(pFile, "#%s FILEVERSION %s PRODUCTVERSION %s\n", static_cast<LPCSTR>(cVer.GetOriginalFilename()), static_cast<LPCSTR>(cVer.GetFileVersion()), static_cast<LPCSTR>(cVer.GetProductVersion()));
	fprintf(stderr, "#�J�n����: %04d/%02d/%02d %02d:%02d:%02d\n",
	aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
	aTime.wMinute, aTime.wSecond);
	fprintf(pFile, "#�J�n����: %04d/%02d/%02d %02d:%02d:%02d\n",
	aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
	aTime.wMinute, aTime.wSecond);
	// �ڑ�
	//���n�}�n�T�[�o
	IWorkspacePtr ipMapWorkspace = ::ConnectSDE(gMapConnect, pFile);

	//���n�}�n�T�[�o
	IWorkspacePtr ipOldMapWorkspace = ::ConnectSDE(gOldMapConnect, pFile);


	//�����L�n�T�[�o
	IWorkspacePtr ipAnoWorkspace = ::ConnectSDE(gAnoConnect, pFile);

	//�����L�n�T�[�o
	IWorkspacePtr ipOldAnoWorkspace = ::ConnectSDE(gOldAnoConnect, pFile);

	//�������T�[�o
	IWorkspacePtr ipBldWorkspace;
	if(gBldConnect != "" && gMapConnect != gBldConnect){
		ipBldWorkspace = ::ConnectSDE(gBldConnect, pFile);
	}else{
		ipBldWorkspace = ipMapWorkspace;
	}
	//�������T�[�o
	IWorkspacePtr ipOldBldWorkspace;
	if(gOldBldConnect != "" && gOldMapConnect != gOldBldConnect){
		ipOldBldWorkspace = ::ConnectSDE(gOldBldConnect, pFile);
	}else{
		ipOldBldWorkspace = ipOldMapWorkspace;
	}

	//�����k�T�[�o
	IWorkspacePtr ipBasWorkspace = ::ConnectSDE(gBasConnect, pFile);

	//�����k�T�[�o
	IWorkspacePtr ipOldBasWorkspace = ::ConnectSDE(gOldBasConnect, pFile);

	//�����H�T�[�o
	IWorkspacePtr ipRodWorkspace = ::ConnectSDE(gRodConnect, pFile);

	//�����H�T�[�o
	IWorkspacePtr ipOldRodWorkspace = ::ConnectSDE(gOldRodConnect, pFile);

	//�N���[���|���S��
	IWorkspacePtr ipClmWorkspace = ::ConnectSDE(gClmConnect, pFile);
	Diff aDiff;

	if(aDiff.init(pFile, ipMapWorkspace, ipOldMapWorkspace, ipAnoWorkspace, ipOldAnoWorkspace, ipBasWorkspace,
		ipOldBasWorkspace, ipRodWorkspace, ipOldRodWorkspace, ipBldWorkspace, ipOldBldWorkspace, ipClmWorkspace, gCheckFeatures, gOutputAll))
	{
		aDiff.readExtFile(gExtFile);
		aDiff.execCheckDiff();
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
	const arctl::coinitializer coinit;
	try {
		GetEnv();
		if(AnalyseArg(argc, argv)){
			Execute();
			printf("�I�����܂�");
		}
	}
	catch(const std::exception& e) {
		printf("%s\n", e.what());
		fprintf(stderr, "#Error,%s\n", e.what());
	}
	return 0;
}

