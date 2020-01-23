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

// ChgCRepAddrCode.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include <time.h>
#include "ChgCode.h"

#ifdef _DEBUG
#define DEF_ADR_CONNECT "sindy2004/sindy2004/CityRep/5151/topaz"
#else
#define DEF_ADR_CONNECT "sindy2004/sindy2004/CityRep/5151/topaz"
#endif
#define DEF_REP_POINT "City_Rep_Point"

static CString gAdrConnect = DEF_ADR_CONNECT;
static CString gOwner;

static CString gRepFeature = DEF_REP_POINT;
static CString gConfigFile;
static LPCTSTR gListFile = NULL;
static bool gChaneMode = false;

void PrintUsage(LPCTSTR App)
{
	LPCTSTR AppName;
	AppName = strrchr(App, '\\');
	if(AppName != NULL){
		AppName++;
	}else{
		AppName = App;
	}
	printf("�n�ԑ�\�_�̏Z���R�[�h�ύX�v���O����\n"
			"Usage:%s [Options] <�Ή����X�g�t�@�C��>\n"
			"  Options:\n"
			"\t-C <�ݒ�t�@�C��>\n"
			"\t-G :�ύX���[�h�w��\n"
			, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("�Ή��\�����ɁA�n�ԑ�\�_�̏Z���R�[�h�̕t���ւ����s���܂��B\n"
			"-G�I�v�V�������w�肳��Ă��Ȃ���΁A���������͍s���܂���B\n"
			"�Ή����X�g�́A�^�u��؂�ŁA�u���R�[�h�v�A�u�V�R�[�h�v�̏��ŋL�q���ĉ������B\n"
			"�R�}���h���C���I�v�V�������A�ݒ�t�@�C�����̎w�肪�D�悳��܂��B\n"
			"�ݒ�t�@�C���̏������́AChgRepAddrCode.txt�������������B\n"
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
				}else if(strncmp(Buf, "REPLAYER", 8) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gRepFeature = p;
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
		if(i == argc - 1){
			gListFile = argv[i];
		}else{
			PrintUsage(argv[0]);
			return false;
		}
		if(gConfigFile != ""){
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
	aEnv = getenv(_T("FC_CITY_REP_POINT"));
	if(aEnv != NULL){
		gRepFeature = aEnv;
	}
	return true;
}

void End( LPCTSTR lpszMessage )
{
	printf(_T("%s\n"), lpszMessage );

	// �I�����ɂ͕K���Ă�ŉ�����
	::CoUninitialize();

	exit(0);
}

IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr )
{
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_SdeWorkspaceFactory );	// 
	IPropertySetPtr ipPropertySet( CLSID_PropertySet );						// �ڑ��v���p�e�B�[�i�[�p
	IWorkspacePtr ipWorkspace;

//	USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v

	/// �ڑ��p�v���p�e�B�ݒ�
	LPTSTR lpszUserName, lpszPassword, lpszInstance, lpszServer, lpszVersion;
	char *aTmpStr = new char[strlen(lpszConnectStr)+1];
	strcpy(aTmpStr, lpszConnectStr);
	lpszUserName = aTmpStr;
	lpszPassword = strchr(aTmpStr, '/');
	if(lpszPassword != NULL){
		*lpszPassword = '\0';
		lpszPassword++;
	}else{
		fprintf(stderr, "�s���Ȑڑ�������B:%s\n", lpszConnectStr);
		return NULL;
	}
	lpszVersion = strchr(lpszPassword, '/');
	if(lpszVersion != NULL){
		*lpszVersion = '\0';
		lpszVersion++;
	}else{
		fprintf(stderr, "�s���Ȑڑ�������B:%s\n", lpszConnectStr);
		return NULL;
	}
	lpszInstance = strchr(lpszVersion, '/');
	if(lpszInstance != NULL){
		*lpszInstance = '\0';
		lpszInstance++;
	}else{
		fprintf(stderr, "�s���Ȑڑ�������B:%s\n", lpszConnectStr);
		return NULL;
	}
	lpszServer = strchr(lpszInstance, '/');
	if(lpszServer != NULL){
		*lpszServer = '\0';
		lpszServer++;
	}else{
		fprintf(stderr, "�s���Ȑڑ�������B:%s\n", lpszConnectStr);
		return NULL;
	}
	gOwner = lpszUserName;
	CComVariant vaUser(lpszUserName), vaPass(lpszPassword), vaInstance(lpszInstance), vaServer(lpszServer), vaVersion(lpszVersion);
	ipPropertySet->SetProperty( CComBSTR(_T("USER")), vaUser );
	ipPropertySet->SetProperty( CComBSTR(_T("INSTANCE")), vaInstance );
	ipPropertySet->SetProperty( CComBSTR(_T("SERVER")), vaServer );
	ipPropertySet->SetProperty( CComBSTR(_T("PASSWORD")), vaPass );
	ipPropertySet->SetProperty( CComBSTR(_T("VERSION")), vaVersion );

//	printf(_T("�ڑ��v���p�e�B�F\nUSER=%s \nINSTANCE=%s \nSERVER=%s \nPASSWORD=%s \nVERSION=%s\n"), 
//		OLE2T( vaUser.bstrVal ), OLE2T( vaInstance.bstrVal ), OLE2T( vaServer.bstrVal ), OLE2T( vaPass.bstrVal ), OLE2T( vaVersion.bstrVal ) );

	// �ڑ�
	printf(_T("%s@%s(%s)�ɐڑ���..."),lpszUserName, lpszServer, lpszVersion);
	delete []aTmpStr;
	if( ipWorkspaceFactory->Open( ipPropertySet, NULL, &ipWorkspace ) != S_OK ) 
	{
		End(_T("�ڑ��ł��Ȃ������x��"));
	}
	printf(_T("�ڑ����܂���\n"));

	return ipWorkspace;
}

void Execute()
{
	// �ڑ�
	IWorkspacePtr ipWorkspace = ::ConnectSDE( _T(gAdrConnect) );
	if( ipWorkspace != NULL ){
		IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
		ATLASSERT( ipFeatureWorkspace != NULL );

		IWorkspaceEditPtr ipWorkspaceEdit = ipWorkspace;
		if(gChaneMode){
			// �ҏW�J�n�iArcMap �̕ҏW�J�n�Ɠ����j
			ipWorkspaceEdit->StartEditing( VARIANT_FALSE );	// Undo Redo ������Ȃ��Ȃ� VARIANT_FALSE
			ipWorkspaceEdit->StartEditOperation();
		}

		IFeatureClassPtr ipRepClass;

		if(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(gRepFeature)), &ipRepClass) != S_OK){
			fprintf(stderr, "�t�B�[�`���[���J���Ȃ�,%s\n", _T(gRepFeature));
			return;
		}
		ChgCode aChgCode;
		if(aChgCode.init(ipRepClass, gListFile, gRepFeature)){
			aChgCode.execChgCRepAddrCode(gChaneMode);
		}
		if(gChaneMode){
			if(FAILED(ipWorkspaceEdit->StopEditOperation())){
				fprintf(stderr, "#�ύX���ۑ��ł��Ȃ��B,\n");
			}
			if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_TRUE ))){	// �ҏW��ۑ�����Ȃ� VARIANT_TRUE �ɂ��邱��
				fprintf(stderr, "#�ۑ������B\n");
			}else{
				fprintf(stderr, "#�ύX���ۑ��ł��Ȃ��B\n");
			}
		}
	}
	return ;
}

int main(int argc, char* argv[])
{
	time_t st_time;
	time(&st_time);
	GetEnv();
	if(AnalyseArg(argc, argv)){
		// COM ���g�p�ł���悤�ɂ��邽�߂̏�����
		::CoInitialize(0);
		{
			// Execute the command
			Execute();
		}

	time_t tm;
	time(&tm);
	printf( " %6.0f�b\n", difftime( tm, st_time));
#ifdef _DEBUG
	getchar();
#endif
		End(_T("�I�����܂�"));
	}
	return 0;
}
