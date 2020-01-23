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

// CSCheck.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "Check.h"
#include <boost/format.hpp>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include <WinLib/VersionInfo.h>
#include <TDC/useful_headers/flush_scope_guard.h>

namespace {
	LPCTSTR DEF_ADR_CONNECT = _T("sindy2006b/sindy2006b/SDE.Current06B/5151/topaz2");
	LPCTSTR DEF_MAP_CONNECT = _T("sindy2006b/sindy2006b/SDE.Current06B/5151/ruby2");
	LPCTSTR DEF_BAS_CONNECT = _T("sindy2006/sindy2006/SDE.DEFAULT/5151/onyx");
	LPCTSTR DEF_REF_CONNECT = _T("reference/reference/SDE.DEFAULT/5151/spinel");

	LPCTSTR DEF_JDB_CONNECT = _T("y0611e/y0611e@delltcp");

	LPCTSTR DEF_GYOSEI = _T("City_Admin");
	LPCTSTR DEF_GOUPOINT = _T("Gou_Point");
	LPCTSTR DEF_CS_ADDR = _T("CS_Addr_Point");
	LPCTSTR DEF_CITY_SITE = _T("City_Site");
	LPCTSTR DEF_CITY_DIVIDER = _T("City_Divider");
	LPCTSTR DEF_BASE_SITE = _T("Base_Site");
	LPCTSTR DEF_RULE = _T("WORKPURPOSE");
	 
	CString gAdrConnect = DEF_ADR_CONNECT;
	CString gMapConnect = DEF_MAP_CONNECT;
	CString gBasConnect = DEF_BAS_CONNECT;
	CString gRefConnect = DEF_REF_CONNECT;
	CString gJdbConnect = DEF_JDB_CONNECT;
	CString gGyoseiFeature = DEF_GYOSEI;
	CString gGouFeature = DEF_GOUPOINT;
	CString gCsFeature = DEF_CS_ADDR;
	CString gCSiteFeature = DEF_CITY_SITE;
	CString gCDividerFeature = DEF_CITY_DIVIDER;
	CString gBSiteFeature = DEF_BASE_SITE;

	LPCTSTR gConfigFile = NULL;
	LPCTSTR gListFile = NULL;
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
	_tprintf(_T("CS_Addr_Pointチェックプログラム\n")
			_T("Usage:%s [-C <設定ファイル>] -G\n")
			_T("     :%s [-C <設定ファイル>] -L <8桁リストファイル>\n")
			_T("-G:全国実行\n")
			, AppName, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("CS_Addr_Pointの正当性をチェックします。\n"
			"サーバやバージョンを変更するには、設定ファイルか環境変数「DB_xx」\n"
			"を使用して下さい。\n"
			"設定ファイルの書き方は、CSCheck.txtをご覧下さい。\n"
			);
}

bool AnalyseConfig()
{
	FILE *fp;
	char Buf[256];
	bool aResult = true;
	char *p;
	if((fp = _tfopen(gConfigFile, _T("rt"))) != NULL){
		while(fgets(Buf, 256, fp)){
			if(Buf[0] != '#' && Buf[0] != '\n'){
				int aLen = strlen(Buf)-1;
				if(Buf[aLen] == '\n' || Buf[aLen] == '\r'){
					Buf[aLen] = '\0';
				}
				if(strncmp(Buf, "DB_MAP", 6) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gMapConnect = p;
					}else{
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "DB_BASE", 7) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gBasConnect = p;
					}else{
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "DB_ADDRESS", 10) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gAdrConnect = p;
					}else{
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "DB_RULE", 7) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gRefConnect = p;
					}else{
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "JDB_CONNECT", 11) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gJdbConnect = p;
					}else{
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_CITY_ADMIN", 13) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gGyoseiFeature = p;
					}else{
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_GOU_POINT", 12) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gGouFeature = p;
					}else{
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_CS_ADDR_POINT", 16) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gCsFeature = p;
					}else{
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_CITY_SITE", 12) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gCSiteFeature = p;
					}else{
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_CITY_DIVIDER", 15) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gCDividerFeature = p;
					}else{
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_BASE_SITE", 12) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						++p;
						gBSiteFeature = p;
					}else{
						aResult = false;
						break;
					}
				}else{
					aResult = false;
					break;
				}
			}
		}
		if(!aResult)
		{
			fprintf(stderr, "#設定ファイルのフォーマットが違います:%s\n", Buf);
		}
		fclose(fp);
	}else{
		_ftprintf(stderr, _T("#設定ファイルが開けません:%s\n"), gConfigFile);
		aResult = false;
	}
	return aResult;
}

bool AnalyseArg(int argc, _TCHAR* argv[])
{
	if(argc < 2  || argc > 5){
		PrintUsage(argv[0]);
		return false;
	}else{
		int i = 1;
		bool aAllFlag = false;
		while(i < argc){
			if(argv[i][0] == '-' || argv[i][0] == '/'){
				switch(argv[i][1] ){
					case '?':
						PrintHelp(argv[0]);
						return false;
						break;
					case 'g':
					case 'G':
						aAllFlag = true;
						break;
					case 'c':
					case 'C':
						i++;
						gConfigFile = argv[i];
						break;
					case 'l':
					case 'L':
						i++;
						gListFile = argv[i];
						break;
					default:
						fprintf(stderr, "#不正なオプションが指定されました。\n");
						PrintUsage(argv[0]);
						return false;
						break;
				}
			}else{
				break;
			}
			i++;
		}
		if(i != argc){
			PrintUsage(argv[0]);
			return false;
		}
		if(!(aAllFlag ^ (gListFile != NULL))){
			fprintf(stderr, "#「-G」と「-L」は同時には指定できません。\n");
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
	LPCTSTR aEnv = _tgetenv(_T("DB_MAP"));
	if(aEnv){
		gMapConnect = aEnv;
	}	
	aEnv = _tgetenv(_T("DB_BASE"));
	if(aEnv){
		gBasConnect = aEnv;
	}	
	aEnv = _tgetenv(_T("DB_ADDRESS"));
	if(aEnv){
		gAdrConnect = aEnv;
	}	
	aEnv = _tgetenv(_T("DB_RULE"));
	if(aEnv){
		gRefConnect = aEnv;
	}
	aEnv = _tgetenv(_T("FC_CITY_ADMIN"));
	if(aEnv){
		gGyoseiFeature = aEnv;
	}
	aEnv = _tgetenv(_T("FC_GOU_POINT"));
	if(aEnv){
		gGouFeature = aEnv;
	}
	aEnv = _tgetenv(_T("FC_CS_ADDR_POINT"));
	if(aEnv){
		gCsFeature = aEnv;
	}
	aEnv = _tgetenv(_T("FC_CITY_SITE"));
	if(aEnv){
		gCSiteFeature = aEnv;
	}
	aEnv = _tgetenv(_T("FC_CITY_DIVIDER"));
	if(aEnv){
		gCDividerFeature = aEnv;
	}
	aEnv = _tgetenv(_T("FC_BASE_SITE"));
	if(aEnv){
		gBSiteFeature = aEnv;
	}
	aEnv = _tgetenv(_T("JDB_CONNECT"));
	if(aEnv){
		gJdbConnect = aEnv;
	}

	return true;
}

IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr )
{
	// 接続
	INamePtr ipName( sindy::create_workspace_name(lpszConnectStr) );
	if( !ipName )
	{
		throw std::runtime_error((boost::format("[%s]の構文が変だＹｏ") % CT2CA(lpszConnectStr).m_psz).str());
	}

	IWorkspacePtr ipWorkspace;
	for(int i = 0; i < 5; ++i){
		_tprintf(_T("%sに接続中(%d回目)..."), lpszConnectStr, i+1);
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		if(ipUnknown != NULL){
			ipWorkspace = ipUnknown;
			break;
		}else{
			_tprintf(_T("\n"));
			Sleep(2000);
		}
	}
	if( !ipWorkspace )
	{
		throw std::runtime_error((boost::format("[%s]に接続できなかったＹｏ") % CT2CA(lpszConnectStr).m_psz).str());
	}
	_tprintf(_T("接続しました\n"));
	fprintf(stderr, "#%sに接続\n", static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz);
//	std::cout << "接続先 : " << static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz << std::endl;
	return ipWorkspace;
}


void Execute()
{
	// 接続
	//地図系サーバ
	IWorkspacePtr ipMapWorkspace = ::ConnectSDE( gMapConnect );
	IFeatureWorkspacePtr ipMapFeatureWorkspace(ipMapWorkspace);
	ATLASSERT( ipMapFeatureWorkspace != NULL );

	//住所系サーバ
	IWorkspacePtr ipAdrWorkspace = ::ConnectSDE( gAdrConnect );
	IFeatureWorkspacePtr ipAdrFeatureWorkspace(ipAdrWorkspace);
	ATLASSERT( ipAdrFeatureWorkspace != NULL );

	//中縮系サーバ
	IWorkspacePtr ipBasWorkspace = ::ConnectSDE( gBasConnect );
	IFeatureWorkspacePtr ipBasFeatureWorkspace(ipBasWorkspace);
	ATLASSERT( ipBasFeatureWorkspace != NULL );

	//ルールサーバ
	IWorkspacePtr ipRefWorkspace = ::ConnectSDE( gRefConnect );
	IFeatureWorkspacePtr ipRefFeatureWorkspace(ipRefWorkspace);
	ATLASSERT( ipRefFeatureWorkspace != NULL );

	IFeatureClassPtr ipGyoseiClass, ipGouClass, ipCSClass, ipCityClass, ipCityDividerClass, ipBaseClass;
	if(ipAdrFeatureWorkspace->OpenFeatureClass(CComBSTR(gGyoseiFeature), &ipGyoseiClass) != S_OK){
		_ftprintf(stderr, _T("#フィーチャーが開けない,%s\n"), static_cast<LPCTSTR>(gGyoseiFeature));
		return;
	}
	if(ipAdrFeatureWorkspace->OpenFeatureClass(CComBSTR(gGouFeature), &ipGouClass) != S_OK){
		_ftprintf(stderr, _T("#フィーチャーが開けない,%s\n"), static_cast<LPCTSTR>(gGouFeature));
		return;
	}
	if(ipAdrFeatureWorkspace->OpenFeatureClass(CComBSTR(gCsFeature), &ipCSClass) != S_OK){
		_ftprintf(stderr, _T("#フィーチャーが開けない,%s\n"), static_cast<LPCTSTR>(gCsFeature));
		return;
	}
	if(ipMapFeatureWorkspace->OpenFeatureClass(CComBSTR(gCSiteFeature), &ipCityClass) != S_OK){
		_ftprintf(stderr, _T("#フィーチャーが開けない,%s\n"), static_cast<LPCTSTR>(gCSiteFeature));
		return;
	}
	if(ipMapFeatureWorkspace->OpenFeatureClass(CComBSTR(gCDividerFeature), &ipCityDividerClass) != S_OK){
		_ftprintf(stderr, _T("#フィーチャーが開けない,%s\n"), static_cast<LPCTSTR>(gCDividerFeature));
		return;
	}
	if(ipBasFeatureWorkspace->OpenFeatureClass(CComBSTR(gBSiteFeature), &ipBaseClass) != S_OK){
		_ftprintf(stderr, _T("#フィーチャーが開けない,%s\n"), static_cast<LPCTSTR>(gBSiteFeature));
		return;
	}

	
	ITablePtr ipRule;
	if(ipRefFeatureWorkspace->OpenTable(CComBSTR(DEF_RULE), &ipRule) != S_OK){
		_ftprintf(stderr, _T("#テーブルが開けない,%s\n"), DEF_RULE);
		return;
	}

	_ftprintf(stderr, _T("FLAG\tLAYER\tOBJECTID\t1:LAYER\t1:OBJECTID\tエラーレベル\tエラーメッセージ\t作業目的\t住所コード\t情報ソース\tCOMMENT\n"));
	CCheck aCheck;
	if(aCheck.init(ipGyoseiClass, ipGouClass, ipCSClass, ipCityClass, ipCityDividerClass, ipBaseClass, ipRule, gJdbConnect)){
		aCheck.execCheck(gListFile);
	}
	return ;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// スコープを抜ける時に強制的にflushする
	uh::flush_scope_guard fsg;

	::setlocale(LC_ALL, "Japanese");
	try {
		const arctl::coinitializer coinit;
		time_t st_time;
		time(&st_time);
		GetEnv();
		if(AnalyseArg(argc, argv)){
			// ツールバージョン
			CVersion cVer;
			_ftprintf(stderr, _T("#FREESTYLELOG\n#%s FILEVERSION %s PRODUCTVERSION %s\n"), static_cast<LPCTSTR>(cVer.GetOriginalFilename()), static_cast<LPCTSTR>(cVer.GetFileVersion()), static_cast<LPCTSTR>(cVer.GetProductVersion()));
			SYSTEMTIME  aTime;
			GetLocalTime(&aTime);
			_ftprintf(stderr, _T("#開始時刻: %04d/%02d/%02d %02d:%02d:%02d\n"),
			aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
			aTime.wMinute, aTime.wSecond);

			Execute();

			time_t tm;
			time(&tm);
			GetLocalTime(&aTime);
			_ftprintf(stderr, _T("#終了時刻: %04d/%02d/%02d %02d:%02d:%02d\n"),
				aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
				aTime.wMinute, aTime.wSecond);
			_tprintf(_T(" %6.0f秒\n"), difftime( tm, st_time));
			_ftprintf(stderr, _T("#トータル時間%6.0f秒\n"), difftime( tm, st_time));
			_tprintf(_T("終了します\n"));
		}
	}
	catch(const std::exception& e) {
		printf("%s\n", e.what());
		fprintf(stderr, "#Error,%s\n", e.what());
	}
	return 0;
}

