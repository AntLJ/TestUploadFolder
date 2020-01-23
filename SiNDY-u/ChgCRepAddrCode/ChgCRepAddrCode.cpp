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

// ChgCRepAddrCode.cpp : コンソール アプリケーション用のエントリ ポイントの定義
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
	printf("地番代表点の住所コード変更プログラム\n"
			"Usage:%s [Options] <対応リストファイル>\n"
			"  Options:\n"
			"\t-C <設定ファイル>\n"
			"\t-G :変更モード指定\n"
			, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("対応表を元に、地番代表点の住所コードの付け替えを行います。\n"
			"-Gオプションが指定されていなければ、書き換えは行いません。\n"
			"対応リストは、タブ区切りで、「旧コード」、「新コード」の順で記述して下さい。\n"
			"コマンドラインオプションより、設定ファイル内の指定が優先されます。\n"
			"設定ファイルの書き方は、ChgRepAddrCode.txtをご覧下さい。\n"
			"サーバ等を変更する場合は、環境変数「DB_ADDRESS」又は設定ファイルを使用して下さい。\n"
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
						fprintf(stderr, "設定ファイルのフォーマットが違います:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "REPLAYER", 8) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gRepFeature = p;
					}else{
						fprintf(stderr, "設定ファイルのフォーマットが違います:%s\n", Buf);
						aResult = false;
						break;
					}
				}else{
					fprintf(stderr, "設定ファイルのフォーマットが違います:%s\n", Buf);
					aResult = false;
					break;
				}
			}
		}
		fclose(fp);
	}else{
		fprintf(stderr, "設定ファイルが開けません:%s\n", gConfigFile);
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
						fprintf(stderr, "不正なオプションが指定されました。\n");
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

	// 終了時には必ず呼んで下さい
	::CoUninitialize();

	exit(0);
}

IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr )
{
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_SdeWorkspaceFactory );	// 
	IPropertySetPtr ipPropertySet( CLSID_PropertySet );						// 接続プロパティー格納用
	IWorkspacePtr ipWorkspace;

//	USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要

	/// 接続用プロパティ設定
	LPTSTR lpszUserName, lpszPassword, lpszInstance, lpszServer, lpszVersion;
	char *aTmpStr = new char[strlen(lpszConnectStr)+1];
	strcpy(aTmpStr, lpszConnectStr);
	lpszUserName = aTmpStr;
	lpszPassword = strchr(aTmpStr, '/');
	if(lpszPassword != NULL){
		*lpszPassword = '\0';
		lpszPassword++;
	}else{
		fprintf(stderr, "不正な接続文字列。:%s\n", lpszConnectStr);
		return NULL;
	}
	lpszVersion = strchr(lpszPassword, '/');
	if(lpszVersion != NULL){
		*lpszVersion = '\0';
		lpszVersion++;
	}else{
		fprintf(stderr, "不正な接続文字列。:%s\n", lpszConnectStr);
		return NULL;
	}
	lpszInstance = strchr(lpszVersion, '/');
	if(lpszInstance != NULL){
		*lpszInstance = '\0';
		lpszInstance++;
	}else{
		fprintf(stderr, "不正な接続文字列。:%s\n", lpszConnectStr);
		return NULL;
	}
	lpszServer = strchr(lpszInstance, '/');
	if(lpszServer != NULL){
		*lpszServer = '\0';
		lpszServer++;
	}else{
		fprintf(stderr, "不正な接続文字列。:%s\n", lpszConnectStr);
		return NULL;
	}
	gOwner = lpszUserName;
	CComVariant vaUser(lpszUserName), vaPass(lpszPassword), vaInstance(lpszInstance), vaServer(lpszServer), vaVersion(lpszVersion);
	ipPropertySet->SetProperty( CComBSTR(_T("USER")), vaUser );
	ipPropertySet->SetProperty( CComBSTR(_T("INSTANCE")), vaInstance );
	ipPropertySet->SetProperty( CComBSTR(_T("SERVER")), vaServer );
	ipPropertySet->SetProperty( CComBSTR(_T("PASSWORD")), vaPass );
	ipPropertySet->SetProperty( CComBSTR(_T("VERSION")), vaVersion );

//	printf(_T("接続プロパティ：\nUSER=%s \nINSTANCE=%s \nSERVER=%s \nPASSWORD=%s \nVERSION=%s\n"), 
//		OLE2T( vaUser.bstrVal ), OLE2T( vaInstance.bstrVal ), OLE2T( vaServer.bstrVal ), OLE2T( vaPass.bstrVal ), OLE2T( vaVersion.bstrVal ) );

	// 接続
	printf(_T("%s@%s(%s)に接続中..."),lpszUserName, lpszServer, lpszVersion);
	delete []aTmpStr;
	if( ipWorkspaceFactory->Open( ipPropertySet, NULL, &ipWorkspace ) != S_OK ) 
	{
		End(_T("接続できなかったＹｏ"));
	}
	printf(_T("接続しました\n"));

	return ipWorkspace;
}

void Execute()
{
	// 接続
	IWorkspacePtr ipWorkspace = ::ConnectSDE( _T(gAdrConnect) );
	if( ipWorkspace != NULL ){
		IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
		ATLASSERT( ipFeatureWorkspace != NULL );

		IWorkspaceEditPtr ipWorkspaceEdit = ipWorkspace;
		if(gChaneMode){
			// 編集開始（ArcMap の編集開始と同じ）
			ipWorkspaceEdit->StartEditing( VARIANT_FALSE );	// Undo Redo がいらないなら VARIANT_FALSE
			ipWorkspaceEdit->StartEditOperation();
		}

		IFeatureClassPtr ipRepClass;

		if(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(gRepFeature)), &ipRepClass) != S_OK){
			fprintf(stderr, "フィーチャーが開けない,%s\n", _T(gRepFeature));
			return;
		}
		ChgCode aChgCode;
		if(aChgCode.init(ipRepClass, gListFile, gRepFeature)){
			aChgCode.execChgCRepAddrCode(gChaneMode);
		}
		if(gChaneMode){
			if(FAILED(ipWorkspaceEdit->StopEditOperation())){
				fprintf(stderr, "#変更が保存できない。,\n");
			}
			if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_TRUE ))){	// 編集を保存するなら VARIANT_TRUE にすること
				fprintf(stderr, "#保存成功。\n");
			}else{
				fprintf(stderr, "#変更が保存できない。\n");
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
		// COM を使用できるようにするための初期化
		::CoInitialize(0);
		{
			// Execute the command
			Execute();
		}

	time_t tm;
	time(&tm);
	printf( " %6.0f秒\n", difftime( tm, st_time));
#ifdef _DEBUG
	getchar();
#endif
		End(_T("終了します"));
	}
	return 0;
}
