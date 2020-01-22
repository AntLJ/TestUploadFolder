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

// AjustChiban.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "AChiban.h"
#include "AdminTable.h"
#include <boost/format.hpp>
#include <sindy/workspace.h>
#include <arctl/coinitializer.h>
#include <WinLib/VersionInfo.h>

namespace {
#define __VERSION
#ifdef _DEBUG
LPCSTR DEF_ADR_CONNECT = "sindy2007a/sindy2007a/CityRep/5151/topaz2";
#else
LPCSTR DEF_ADR_CONNECT = "sindy/sindy/SDE.DEFAULT/5151/topaz";
#endif

LPCSTR DEF_GYOSEI = "City_Admin";
LPCSTR DEF_CHIBAN = "City_Rep_Point";

CString gAdrConnect = DEF_ADR_CONNECT;

CString gGyoseiFeature = DEF_GYOSEI;
CString gChibanFeature = DEF_CHIBAN;
LPCTSTR gConfigFile = NULL;
LPCTSTR gAddrFile = NULL;
CString gWhere = _T("");

LPCTSTR gAddrCode = NULL;
bool gAllFlag = false;
bool gChgMode = false;
}

void PrintUsage(LPCTSTR App)
{
	LPCTSTR AppName;
	AppName = _tcsrchr(App, _T('\\'));
	if(AppName != NULL){
		AppName++;
	}else{
		AppName = App;
	}
	_tprintf(_T("地番注記調整プログラム（メンテ版）\n")
			_T("Usage:%s [オプション] <市区町村コード(5桁又は11桁)>\n")
			_T(" or   %s [オプション] -J <住所コードリスト>\n")
			_T(" or   %s [オプション] -A\n")
			_T(" オプション：\n")
			_T("  -C <設定ファイル>\n")
			_T("  -G :変更モード指定\n")
			, AppName, AppName, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	_tprintf(_T("地番注記の行政界境界での重なりを修正します。\n")
			_T("サーバやバージョンを変更するには、設定ファイルか環境変数「DB_ADDRESS」\n")
			_T("を使用して下さい。\n")
			_T("設定ファイルの書き方は、AjustChiban.txtをご覧下さい。\n")
			_T("「-A」オプションを使用し、かつ設定ファイル内で条件を指定しなければ、")
			_T("行政界データに含まれる全ての市区町村コードが対象になります。\n"));
}

bool AnalyseConfig()
{
	FILE *fp;
	_TCHAR Buf[256];
	bool aResult = true;
	LPTSTR p;
	if((fp = _tfopen(gConfigFile, _T("rt"))) != NULL){
		while(_fgetts(Buf, 256, fp)){
			if(Buf[0] != _T('#') && Buf[0] != _T('\n')){
				int aLen = _tcslen(Buf)-1;
				if(Buf[aLen] == _T('\n') || Buf[aLen] == _T('\r')){
					Buf[aLen] = '\0';
				}
				if(_tcsncmp(Buf, _T("DB_ADDRESS"), 10) == 0){
					if((p = _tcschr(Buf, _T('\t'))) != NULL){
						p++;
						gAdrConnect = p;
					}else{
						_ftprintf(stderr, _T("設定ファイルのフォーマットが違います:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else if(_tcsncmp(Buf, _T("FC_CITY_ADMIN"), 13) == 0){
					if((p = _tcschr(Buf, '\t')) != NULL){
						p++;
						gGyoseiFeature = p;
					}else{
						fprintf(stderr, "#設定ファイルのフォーマットが違います:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(_tcsncmp(Buf, _T("FC_CITY_REP_POINT"), 17) == 0){
					if((p = _tcschr(Buf, '\t')) != NULL){
						p++;
						gChibanFeature = p;
					}else{
						_ftprintf(stderr, _T("#設定ファイルのフォーマットが違います:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else if(_tcsncmp(Buf, _T("WHERE"), 5) == 0){
					if((p = _tcschr(Buf, '\t')) != NULL){
						p++;
						gWhere = p;
					}else{
						_ftprintf(stderr, _T("#設定ファイルのフォーマットが違います:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else{
					_ftprintf(stderr, _T("#設定ファイルのフォーマットが違います:%s\n"), Buf);
					aResult = false;
					break;
				}
			}
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
	if(argc < 2){
		PrintUsage(argv[0]);
		return false;
	}else{
		int i = 1;
		while(i < argc){
			if(argv[i][0] == '-' || argv[i][0] == '/'){
				switch(argv[i][1] ){
					case _T('?'):
						PrintHelp(argv[0]);
						return false;
						break;
					case _T('a'):
					case _T('A'):
						if(gAddrFile != NULL){
							PrintUsage(argv[0]);
							return false;
						}
						gAllFlag = true;
						break;
					case _T('c'):
					case _T('C'):
						i++;
						gConfigFile = argv[i];
						break;
					case _T('j'):
					case _T('J'):
						if(gAllFlag){
							PrintUsage(argv[0]);
							return false;
						}
						i++;
						gAddrFile = argv[i];
						break;
					case _T('g'):
					case _T('G'):
						gChgMode = true;
						break;
					default:
						_ftprintf(stderr, _T("#不正なオプションが指定されました。\n"));
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
			if(!gAllFlag && gAddrFile == NULL){
				gAddrCode = argv[i];
			}else{
				PrintUsage(argv[0]);
				return false;
			}
		}else if (i == argc){
			if(!gAllFlag && gAddrFile == NULL){
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
	LPCTSTR aEnv = _tgetenv(_T("DB_ADDRESS"));
	if(aEnv != NULL){
		gAdrConnect = aEnv;
	}
	aEnv = _tgetenv(_T("FC_CITY_ADMIN"));
	if(aEnv != NULL){
		gGyoseiFeature = aEnv;
	}
	aEnv = _tgetenv(_T("FC_CITY_REP_POINT"));
	if(aEnv != NULL){
		gChibanFeature = aEnv;
	}
	return true;
}

IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr )
{
	// 接続
	_tprintf(_T("%sに接続中..."), lpszConnectStr);
	IWorkspacePtr ipWorkspace =  sindy::create_workspace(lpszConnectStr);
	if( NULL == ipWorkspace )
	{
//		_tprintf(_T("[%s]に接続できなかったＹｏ\n") , lpszConnectStr);
		_ftprintf(stderr, _T("#[%s]に接続できなかったＹｏ\n") , lpszConnectStr);
		throw std::runtime_error((boost::format("[%s]に接続できなかったＹｏ") % CT2CA(lpszConnectStr).m_psz).str());
	}
	_tprintf(_T("接続しました\n"));
	_ftprintf(stderr, _T("#%sに接続\n"), sindy::workspace_address(ipWorkspace).c_str());
	return ipWorkspace;
}

bool MainteSingleChiban(AChiban &cChiban, CString& cAddrCode)
{
	_tprintf(_T("%s,実行中・・・\n"), cAddrCode);
	_ftprintf(stderr, _T("#%s開始\n"), cAddrCode);
	if(cChiban.execAjustChiban(cAddrCode)){
		if(gChgMode){
			_ftprintf(stderr, _T("#%s変更あり\n"), cAddrCode);
		}else{
			_ftprintf(stderr, _T("#%s変更の必要あり\n"), cAddrCode);
		}
	}else{
		if(gChgMode){
			_ftprintf(stderr, _T("#%s変更なし\n"), cAddrCode);
		}else{
			_ftprintf(stderr, _T("#%s変更の必要なし\n"), cAddrCode);
		}
	}
	cChiban.unInit();
	_ftprintf(stderr, _T("#%s終了\n"), cAddrCode);
	_tprintf(_T("終了\n"));
	fflush(stderr);
	return true;
}


void Execute()
{
	// 接続
	IWorkspacePtr ipAdrWorkspace = ::ConnectSDE(gAdrConnect);
	ATLASSERT( ipAdrWorkspace != NULL );
	IFeatureWorkspacePtr ipAdrFeatureWorkspace(ipAdrWorkspace);
	ATLASSERT( ipAdrFeatureWorkspace != NULL );

	IFeatureClassPtr ipGyoseiClass, ipChibanClass;
	if(ipAdrFeatureWorkspace->OpenFeatureClass(CComBSTR(gGyoseiFeature), &ipGyoseiClass) != S_OK){
		_ftprintf(stderr, _T("#フィーチャーが開けない,%s\n"), static_cast<LPCTSTR>(gGyoseiFeature));
		return;
	}
	if(ipAdrFeatureWorkspace->OpenFeatureClass(CComBSTR(gChibanFeature), &ipChibanClass) != S_OK){
		_ftprintf(stderr, _T("#フィーチャーが開けない,%s\n"), static_cast<LPCTSTR>(gChibanFeature));
		return;
	}

	if(gChgMode){
		// 編集開始（ArcMap の編集開始と同じ）
		IWorkspaceEditPtr ipWorkspaceEdit = ipAdrWorkspace;
		ipWorkspaceEdit->StartEditing( VARIANT_FALSE );	// Undo Redo がいらないなら VARIANT_FALSE
		ipWorkspaceEdit->StartEditOperation();
	}
	AChiban aChiban;
	if(aChiban.init(ipGyoseiClass, ipChibanClass, gChgMode)){
		if(gAllFlag){
			AdminTable aAdminTable;
			if(aAdminTable.init(ipAdrWorkspace, gGyoseiFeature, gWhere)){
				CString aAddrCode;
				while(aAdminTable.next(aAddrCode)){
					MainteSingleChiban(aChiban, aAddrCode);
					if(_kbhit()){
						int aKey = _getch();
						if(aKey == 'Q'){
							_tprintf(_T("処理は中止されました。\n"));
							_ftprintf(stderr, _T("処理は中止されました。\n"));
							break;
						}
					}
				}
			}
		}else if(gAddrFile != NULL){
			FILE *pFile;
			char aBuf[16];
			if((pFile = _tfopen(gAddrFile, _T("rt"))) != NULL){
				while(fgets(aBuf, 16, pFile) != NULL){
					if(aBuf[0] != '\n' && aBuf[0] != '#'){
						int aLen = strlen(aBuf) -1;
						if(aBuf[aLen] == '\n'){
							aBuf[aLen] = '\0';
						}
						MainteSingleChiban(aChiban, CString(aBuf));
					}
					if(_kbhit()){
						int aKey = _getch();
						if(aKey == 'Q'){
							_tprintf(_T("処理は中止されました。\n"));
							_ftprintf(stderr, _T("処理は中止されました。\n"));
							break;
						}
					}
				}
				fclose(pFile);
			}else{
				_ftprintf(stderr, _T("ファイルが開けない。:%s\n"), gAddrFile);
			}
		}else{
			MainteSingleChiban(aChiban, CString(gAddrCode));
		}
	}
	if(gChgMode){
		IWorkspaceEditPtr ipWorkspaceEdit = ipAdrWorkspace;
		ipWorkspaceEdit->StopEditOperation();
		if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_TRUE ))){	// 編集を保存するなら VARIANT_TRUE にすること
			fprintf(stderr, "#保存成功。\n");	
		}else{
			fprintf(stderr, "#変更が保存できない。\n");	
		}
	}
	return ;
}

int _tmain(int argc, _TCHAR* argv[])
{
	::setlocale(LC_ALL, "Japanese");
	try {
		const arctl::coinitializer coinit;
		time_t st_time;
		time(&st_time);
		if(GetEnv()){
			if(AnalyseArg(argc, argv)){
				// ツールバージョン
				CVersion cVer;
				_ftprintf(stderr, _T("#%s FILEVERSION %s PRODUCTVERSION %s\n"), static_cast<LPCTSTR>(cVer.GetOriginalFilename()), static_cast<LPCTSTR>(cVer.GetFileVersion()), static_cast<LPCTSTR>(cVer.GetProductVersion()));
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
				int h,m,s;
				double t = difftime( tm, st_time);
				h = t/3600;
				m = (int(t)%3600)/60;
				s = int(t)%60;
				_tprintf(_T("トータル時間 %d時間%d分%d秒\n"), h, m, s);
				_ftprintf(stderr, _T("#トータル時間 %d時間%d分%d秒\n"), h, m, s);
				_tprintf(_T("終了します\n"));
			}
		}
	}
	catch(const std::exception& e) {
		printf("%s\n", e.what());
		fprintf(stderr, "#Error,%s\n", e.what());
	}
	return 0;
}
