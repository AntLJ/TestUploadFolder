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

// ChgRepAddrCode.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <time.h>
#include "ChgCode.h"
#include <boost/format.hpp>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>

namespace {
#ifdef _DEBUG
LPCSTR DEF_ADR_CONNECT = "beta9/beta9/SDE.DEFAULT/5151/geo-as";
#else
LPCSTR DEF_ADR_CONNECT = "sindy/sindy/SDE.DEFAULT/5151/topaz";
#endif
LPCSTR DEF_REP_POINT = "Base_Rep_Point";

CString gAdrConnect = DEF_ADR_CONNECT;

CString gRepFeature = DEF_REP_POINT;
CString gConfigFile;
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
	printf("町丁目代表点（レイヤ変更でCSポイントも）の住所コード変更プログラム\n"
			"Usage:%s [Options] <対応リストファイル>\n"
			"  Options:\n"
			"\t-C <設定ファイル>\n"
			"\t-G :変更モード指定\n"
			, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("対応表を元に、町丁目代表点の住所コードの付け替えを行います。\n"
			"-Gオプションが指定されていなければ、書き換えは行いません。\n"
			"対応リストは、タブ区切りで、「旧コード」、「新コード」の順で記述して下さい。\n"
			"対象レイヤを変更するには、環境変数「FC_BASE_REP_POINT」又は設定ファイルで指定して下さい\n"
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
				}else if(strncmp(Buf, "FC_BASE_REP_POINT", 17) == 0){
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
	aEnv = getenv(_T("FC_BASE_REP_POINT"));
	if(aEnv != NULL){
		gRepFeature = aEnv;
	}
	return true;
}

IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr )
{
	// 接続
	INamePtr ipName( sindy::create_workspace_name(lpszConnectStr) );
	if( NULL == ipName )
	{
		throw std::runtime_error((boost::format("[%s]の構文が変だＹｏ") % CT2CA(lpszConnectStr).m_psz).str());
	}

	printf("%sに接続中...", lpszConnectStr);
	IUnknownPtr ipUnknown;
	ipName->Open( &ipUnknown );
	IWorkspacePtr ipWorkspace = ipUnknown;
	if( NULL == ipWorkspace )
	{
		throw std::runtime_error((boost::format("[%s]に接続できなかったＹｏ") % CT2CA(lpszConnectStr).m_psz).str());
	}
	printf("接続しました\n");
	fprintf(stderr, "#%sに接続\n", static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz);
//	std::cout << "接続先 : " << static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz << std::endl;
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
	//if( !InitAttemptWithoutExtension( esriLicenseProductCodeEngineGeoDB ) ){
	//	if( !InitAttemptWithoutExtension( esriLicenseProductCodeArcEditor ) ){
	//		fprintf(stderr, "#ライセンスの認証に失敗\n");
	//		return;
	//	}else{
	//		fprintf(stderr, "#ArcEditorで認証\n");
	//	}
	//}else{
	//	fprintf(stderr, "#ArcGIS Engine + GeoDBで認証\n");
	//}
	// 接続
	IWorkspacePtr ipWorkspace = ::ConnectSDE( _T(gAdrConnect) );
	IVersionPtr ipChildVersion;
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	CString aChild;

	if(gChaneMode){
		SYSTEMTIME stSystemTime;
		::GetLocalTime( &stSystemTime);

		aChild.Format("ChgRepAddrCode_%04d/%02d/%02d %02d:%02d:%02d", 
			stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
			stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
		IVersionedWorkspacePtr ipVerWork = ipWorkspace;
		if(ipVerWork == NULL){
			fprintf(stderr, "#バージョン対応じゃない？\n");
			return;
		}
		ipVerWork->FindVersion(CComBSTR(aChild), &ipChildVersion);
		if(ipChildVersion == NULL){
			IVersionPtr ipCurrentVersion = ipWorkspace;
			ipCurrentVersion->CreateVersion(CComBSTR(aChild),&ipChildVersion);
			if(ipChildVersion == NULL){
				fprintf(stderr, "#子バージョン作成失敗,%s\n", static_cast<LPCSTR>(aChild));
				return;
			}
		}
		ipFeatureWorkspace = ipChildVersion;
		// 編集開始（ArcMap の編集開始と同じ）
		IWorkspaceEditPtr ipWorkspaceEdit = ipChildVersion;
		ipWorkspaceEdit->StartEditing( VARIANT_FALSE );	// Undo Redo がいらないなら VARIANT_FALSE
		ipWorkspaceEdit->StartEditOperation();
	}

	ATLASSERT( ipFeatureWorkspace != NULL );
	IFeatureClassPtr ipRepClass;

	if(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(gRepFeature)), &ipRepClass) != S_OK){
		fprintf(stderr, "フィーチャーが開けない,%s\n", static_cast<LPCSTR>(gRepFeature));
		return;
	}
	ChgCode aChgCode;
	if(aChgCode.init(ipRepClass, gListFile, gRepFeature)){
		aChgCode.execChgRepAddrCode(gChaneMode);
	}
	if(gChaneMode){
		IWorkspaceEditPtr ipWorkspaceEdit = ipChildVersion;
		ipWorkspaceEdit->StopEditOperation();
		if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_TRUE ))){	// 編集を保存するなら VARIANT_TRUE にすること
		}else{
			fprintf(stderr, "#変更が保存できない。,%s\n", static_cast<LPCSTR>(aChild));
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
			fprintf(stderr, "#開始時刻: %04d/%02d/%02d %02d:%02d:%02d\n",
			aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
			aTime.wMinute, aTime.wSecond);

			Execute();

			time_t tm;
			time(&tm);
			GetLocalTime(&aTime);
			fprintf(stderr, "#終了時刻: %04d/%02d/%02d %02d:%02d:%02d\n",
				aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
				aTime.wMinute, aTime.wSecond);
			printf( " %6.0f秒\n", difftime( tm, st_time));
			fprintf(stderr, "#トータル時間%6.0f秒\n", difftime( tm, st_time));
			printf(_T("終了します"));
		}
	}
	catch(const std::exception& e) {
		printf("%s\n", e.what());
		fprintf(stderr, "#Error,%s\n", e.what());
	}
	return 0;
}
