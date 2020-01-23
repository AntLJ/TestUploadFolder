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

// ChgAddrCode.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include <time.h>
#include "ChgCode.h"
#include <boost/format.hpp>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include "..\..\LIB\WinLib\VersionInfo.h"	// [bug 8793]�Ή����ǉ�

namespace {
#ifdef _DEBUG
LPCSTR DEF_ADR_CONNECT = "sinup2006b/sinup2006b/SDE.Current06B/5151/garnet2";
#else
LPCSTR DEF_ADR_CONNECT = "ronly/ronly/SDE.DEFAULT/5151/topaz";
#endif

LPCSTR DEF_GYOSEI = "City_Admin";

CString gAdrConnect = DEF_ADR_CONNECT;

CString gGyoseiFeature = DEF_GYOSEI;
LPCSTR gConfigFile = NULL;
LPCSTR gListFile = NULL;
bool gChaneMode = false;
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
	printf("�s���E�R�[�h�ύX�v���O����\n"
			"Usage:%s [Options] <�Ή����X�g�t�@�C��>\n"
			"  Options:\n"
			"\t-C <�ݒ�t�@�C��>\n"
			"\t-G :�ύX���[�h�w��\n"
			, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("�Ή��\�����ɁA�Z���R�[�h�̕t���ւ����s���܂��B\n"
			"-G�I�v�V�������w�肳��Ă��Ȃ���΁A���������͍s���܂���B\n"
			"�Ή����X�g�́A�^�u��؂�ŁA�u���R�[�h�v�A�u�V�R�[�h�v�̏��ŋL�q���ĉ������B\n"
			"�R�}���h���C���I�v�V�������A�ݒ�t�@�C�����̎w�肪�D�悳��܂��B\n"
			"�ݒ�t�@�C���̏������́AChgAddrCode.txt�������������B\n"
			"�T�[�o����ύX����ꍇ�́A���ϐ��uDB_ADDRESS�v���͐ݒ�t�@�C�����g�p���ĉ������B\n"
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
					case 'g':
					case 'G':
						gChaneMode = true;
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

	IWorkspacePtr ipWorkspace;
	for(int i = 0; i < 5; i++){
		_tprintf(_T("%s�ɐڑ���(%d���)..."), lpszConnectStr, i+1);
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		if(ipUnknown != NULL){
			ipWorkspace = ipUnknown;
			break;
		}else{
			printf("���s\n");
			Sleep(2000);
		}
	}
	if( NULL == ipWorkspace )
	{
		throw std::runtime_error((boost::format("[%s]�ɐڑ��ł��Ȃ������x��") % CT2CA(lpszConnectStr).m_psz).str());
	}
	printf("�ڑ����܂���\n");
	fprintf(stderr, "#%s�ɐڑ�\n", static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz);
	if( gChaneMode == true ) {	// [bug 8793]�Ή����ɒǉ�
		fprintf(stderr, "#�X�V���[�h�Ŏ��s\n", static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz);	
	}
//	std::cout << "�ڑ��� : " << static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz << std::endl;
	return ipWorkspace;
}

bool InitAttemptWithoutExtension( esriLicenseProductCode product )
{
	IAoInitializePtr ipInit(CLSID_AoInitialize);

	esriLicenseStatus status = esriLicenseFailure;

	HRESULT hr = ipInit->IsProductCodeAvailable( product, &status);
	if (esriLicenseAvailable == status)
	{
		ipInit->Initialize(product, &status);
		if( status == esriLicenseNotLicensed ||
			status == esriLicenseUnavailable ||
			status == esriLicenseFailure ||
			status == esriLicenseNotInitialized )
			return false;
		else
			return true;
	}

	return false;
}

void Execute()
{
	//if( !InitAttemptWithoutExtension( esriLicenseProductCodeArcEditor ) ){
	//	if( !InitAttemptWithoutExtension( esriLicenseProductCodeEngineGeoDB ) ){
	//		fprintf(stderr, "#���C�Z���X�̔F�؂Ɏ��s\n");
	//		return;
	//	}else{
	//		fprintf(stderr, "#ArcGIS Engine + GeoDB�ŔF��\n");
	//	}
	//}else{
	//	fprintf(stderr, "#ArcEditor�ŔF��\n");
	//}
	// �ڑ�
	IWorkspacePtr ipWorkspace = ::ConnectSDE( _T(gAdrConnect) );
	IVersionPtr ipChildVersion;
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	CString aChild;

	if(gChaneMode){
		SYSTEMTIME stSystemTime;
		::GetLocalTime( &stSystemTime);

		aChild.Format("ChgAddrCode_%04d/%02d/%02d %02d:%02d:%02d", 
			stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
			stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
		IVersionedWorkspacePtr ipVerWork = ipWorkspace;
		if(ipVerWork == NULL){
			fprintf(stderr, "#�o�[�W�����Ή�����Ȃ��H\n");
			return;
		}
		IVersionPtr ipCurrentVersion = ipWorkspace;
		HRESULT hr = ipCurrentVersion->CreateVersion(CComBSTR(aChild),&ipChildVersion);
		if(ipChildVersion == NULL){
			fprintf(stderr, "#�q�o�[�W�����쐬���s,%s(hr=%d)\n", static_cast<LPCSTR>(aChild), hr);
			IErrorInfoPtr	ipErrorInfo;
			::GetErrorInfo(0, &ipErrorInfo);
			BSTR	hoge;
			ipErrorInfo->GetDescription(&hoge);
		//	ATLTRACE(hoge);
			fprintf(stderr, "#Error,%s\n", static_cast<LPCSTR>(CString(hoge)));
			return;
		}else { // [bug 8793]�Ή����ǉ�
			fprintf(stderr, "#�q�o�[�W�����쐬����,%s\n", static_cast<LPCSTR>(aChild));
		}
		ipFeatureWorkspace = ipChildVersion;
		// �ҏW�J�n�iArcMap �̕ҏW�J�n�Ɠ����j
		IWorkspaceEditPtr ipWorkspaceEdit = ipChildVersion;
		ipWorkspaceEdit->StartEditing( VARIANT_FALSE );	// Undo Redo ������Ȃ��Ȃ� VARIANT_FALSE
		ipWorkspaceEdit->StartEditOperation();
	}
	ATLASSERT( ipFeatureWorkspace != NULL );

	IFeatureClassPtr ipGyoseiClass;
	if(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(gGyoseiFeature)), &ipGyoseiClass) != S_OK){
		fprintf(stderr, "�t�B�[�`���[���J���Ȃ�,%s\n", static_cast<LPCSTR>(gGyoseiFeature));
		return;
	}
	ChgCode aChgCode;
	if(aChgCode.init(ipGyoseiClass, gListFile, gGyoseiFeature)){
		aChgCode.execChgAddrCode(gChaneMode);
	}
	if(gChaneMode){
		IWorkspaceEditPtr ipWorkspaceEdit = ipChildVersion;
		ipWorkspaceEdit->StopEditOperation();
		if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_TRUE ))){	// �ҏW��ۑ�����Ȃ� VARIANT_TRUE �ɂ��邱��
		}else{
			fprintf(stderr, "#�ύX���ۑ��ł��Ȃ��B,%s\n", static_cast<LPCSTR>(aChild));
		}
	}
	return ;
}

int main(int argc, char* argv[])
{
	//-----------------------------------------------------------------------
	// �c�[���o�[�W�����\��
	CVersion	a_cvTool_Var;
	CString		a_csVar	= a_cvTool_Var.GetFileVersion();
	//-----------------------------------------------------------------------
	try {
		const arctl::coinitializer coinit;	//ArcGIS Engine�������Ă���Ɠ����Ȃ��Ȃ邩��_�����ۂ�
		time_t st_time;
		time(&st_time);
		GetEnv();
		if(AnalyseArg(argc, argv)){
			SYSTEMTIME  aTime;
			GetLocalTime(&aTime);
			fprintf(stderr, "#�J�n����: %04d/%02d/%02d %02d:%02d:%02d\n",
			aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
			aTime.wMinute, aTime.wSecond);
			fprintf(stderr, "#�o�[�W����: %s\n", static_cast<LPCSTR>(a_csVar) );

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


