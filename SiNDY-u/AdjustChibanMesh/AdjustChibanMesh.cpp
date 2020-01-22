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

// AjustChibanMesh.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <boost/format.hpp>
#include <sindy/workspace.h>
#include <arctl/coinitializer.h>
#include <WinLib/VersionInfo.h>
#include "meshindex.h"
#include "AChiban.h"

namespace{
#ifdef _DEBUG
LPCTSTR DEF_ADR_CONNECT = _T("sindy2012a/sindy2012a/SDE.EDT_SINDY2012A/5151/topaz2");
#else
LPCTSTR DEF_ADR_CONNECT = _T("sindy2006b/sindy2006b/SDE.DEFAULT/5151/topaz2");
#endif

LPCTSTR DEF_CHIBAN = _T("City_Rep_Point");
LPCTSTR DEF_MESH = _T("reference.CityMesh");

LPCTSTR DEF_MESHFIELD = _T("MeshCode");

CString gAdrConnect = DEF_ADR_CONNECT;	///< 住所系サーバ接続文字列

CString gMeshFeature = DEF_MESH;		///< 都市地図メッシュポリゴンフィーチャクラス名
CString gChibanFeature = DEF_CHIBAN;	///< 地番代表点フィーチャクラス名
LPCTSTR gConfigFile = NULL;				///< 設定ファイル名
LPCTSTR gListFile = NULL;				///< メッシュリストファイル名
CString gWhere;							///< メッシュ選択用の条件（SQL）
CString gMeshField = DEF_MESHFIELD;		///< メッシュコードフィールド名（昔は統一されていなかった名残）

LPCTSTR gMeshCode = NULL;				///< メッシュコード（単体指定用）
bool gAllFlag = false;					///< 全国処理フラグ
bool gChgMode = false;					///< 変更モード
}

/** 
 * @brief 使用法表示 
 * @param[in]	App	実行時コマンドラインの一つ目 
 */ 
void PrintUsage(LPCTSTR App)
{
	LPCTSTR AppName;
	AppName = _tcsrchr(App, _T('\\'));
	if(AppName != NULL){
		AppName++;
	}else{
		AppName = App;
	}
	_tprintf(_T("メッシュ境界上の地番注記調整（フラグオフ）プログラム\n")
			_T("Usage:%s [-C <設定ファイル>] [-G] <メッシュコード>\n")
			_T(" or   %s [-C <設定ファイル>] [-G] -M <メッシュコードリスト>\n")
			_T(" or   %s [-C <設定ファイル>] [-G] -A\n")
			_T(" -G:変更モード指定\n")
			, AppName, AppName, AppName);
}

/** 
 * @brief 簡易ヘルプ表示 
 * @param[in]	App	実行時コマンドラインの一つ目 
 */ 
void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("メッシュ境界上の地番注記のフラグをオフにします。\n"
			"サーバやバージョンを変更するには、設定ファイルか環境変数「DB_ADDRESS」\n"
			"を使用して下さい。\n"
			"設定ファイルの書き方は、AjustChibanMesh.txtをご覧下さい。\n"
			"「-A」オプションを使用し、かつ設定ファイル内で条件を指定しなければ、"
			"全ての都市地図エリアが対象になります。\n");
}

/** 
 * @brief 設定ファイル読み込み 
 * @retval true		成功
 * @retval false	失敗
 */ 
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
				}else if(_tcsncmp(Buf, _T("FC_CITY_MESH"), 12) == 0){
					if((p = _tcschr(Buf, _T('\t'))) != NULL){
						p++;
						gMeshFeature = p;
					}else{
						_ftprintf(stderr, _T("#設定ファイルのフォーマットが違います:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else if(_tcsncmp(Buf, _T("FC_CITY_REP_POINT"), 17) == 0){
					if((p = _tcschr(Buf, _T('\t'))) != NULL){
						p++;
						gChibanFeature = p;
					}else{
						_ftprintf(stderr, _T("#設定ファイルのフォーマットが違います:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else if(_tcsncmp(Buf, _T("WHERE"), 5) == 0){
					if((p = _tcschr(Buf, _T('\t'))) != NULL){
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

/**
 * @brief コマンドライン解析。
 *
 * @param argc	[in]	メイン関数の第一引数
 * @param argv	[in]	メイン関数の第二引数
 *
 * @retval true		成功
 * @retval false	失敗、エラー
 */
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
						if(gListFile != NULL){
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
					case _T('g'):
					case _T('G'):
						gChgMode = true;
						break;
					case _T('m'):
					case _T('M'):
						if(gAllFlag){
							PrintUsage(argv[0]);
							return false;
						}
						i++;
						gListFile = argv[i];
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
			if(!gAllFlag && gListFile == NULL){
				gMeshCode = argv[i];
			}else{
				PrintUsage(argv[0]);
				return false;
			}
		}else if (i == argc){
			if(!gAllFlag && gListFile == NULL){
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

/**
 * @brief 環境変数取得
 *
 * @retval true		成功（しか返さない）
 */
bool GetEnv(void)
{
	LPCTSTR aEnv = _tgetenv(_T("DB_ADDRESS"));
	if(aEnv != NULL){
		gAdrConnect = aEnv;
	}
	
	aEnv = _tgetenv(_T("FC_CITY_MESH"));
	if(aEnv != NULL){
		gMeshFeature = aEnv;
	}
	aEnv = _tgetenv(_T("FC_CITY_REP_POINT"));
	if(aEnv != NULL){
		gChibanFeature = aEnv;
	}
	return true;
}


/**
 * @brief ワークスペースのオープン（サーバへの接続）。
 *
 * @param argc	[in]	接続文字列
 *
 * @retval NULL以外	ワークスペースのポインタ
 * @retval NULL	失敗、エラー
 */
IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr )
{
	// 接続
	_tprintf(_T("%sに接続中..."), lpszConnectStr);
	IWorkspacePtr ipWorkspace =  sindy::create_workspace(lpszConnectStr);
	if( NULL == ipWorkspace )
	{
		_tprintf(_T("[%s]に接続できなかったＹｏ\n") , lpszConnectStr);
		_ftprintf(stderr, _T("[%s]に接続できなかったＹｏ\n") , lpszConnectStr);
		return NULL;
	}
	_tprintf(_T("接続しました\n"));
	_ftprintf(stderr, _T("#%sに接続\n"), sindy::workspace_address(ipWorkspace).c_str());
	return ipWorkspace;
}

/**
 * @brief 実行部本体。
 *
 */
void Execute()
{
	// 接続
	IWorkspacePtr ipAdrWorkspace = ::ConnectSDE(gAdrConnect);
	ATLASSERT( ipAdrWorkspace != NULL );
	if( ipAdrWorkspace == NULL ){
		return;
	}
	IFeatureWorkspacePtr ipAdrFeatureWorkspace(ipAdrWorkspace);
	ATLASSERT( ipAdrFeatureWorkspace != NULL );

	IFeatureClassPtr ipMeshClass;
	if(ipAdrFeatureWorkspace->OpenFeatureClass(CComBSTR(gMeshFeature), &ipMeshClass) != S_OK){
		_ftprintf(stderr, _T("#フィーチャーが開けない,%s\n"), gMeshFeature);
		return;
	}

	IFeatureClassPtr ipRepClass;
	if(ipAdrFeatureWorkspace->OpenFeatureClass(CComBSTR(gChibanFeature), &ipRepClass) != S_OK){
		_ftprintf(stderr, _T("#フィーチャーが開けない,%s\n"), gChibanFeature);
		return;
	}

	// 編集開始（ArcMap の編集開始と同じ）
	IWorkspaceEditPtr ipWorkspaceEdit = ipAdrWorkspace;
	if(gChgMode){
		ipWorkspaceEdit->StartEditing( VARIANT_FALSE );	// Undo Redo がいらないなら VARIANT_FALSE
		ipWorkspaceEdit->StartEditOperation();
	}else{
		_tprintf(_T("チェックモードで動作します。\n"));
		_ftprintf(stderr, _T("#チェックモードで動作します。\n"));
	}
	AChiban aChiban;
	MeshIndex aMeshIndex(ipMeshClass);
	if(aChiban.init(ipRepClass, gChgMode)){
		if(gAllFlag){
			if(aMeshIndex.init(gMeshField, gWhere)){
				LONG aMesh;
				IFeaturePtr ipFeature;
				while((ipFeature = aMeshIndex.nextFeature(&aMesh)) != NULL){
					aChiban.execAdjust(ipFeature, aMesh);
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
		}else if(gListFile != NULL){
			FILE *pFile;
			_TCHAR aBuf[16];
			if((pFile = _tfopen(gListFile, _T("rt"))) != NULL){
				while(_fgetts(aBuf, 16, pFile) != NULL){
					if(aBuf[0] != _T('\n') || aBuf[0] != _T('#')){
						int aLen = _tcsclen(aBuf) -1;
						if(aBuf[aLen] == '\n'){
							aBuf[aLen] = _T('\0');
						}
						if(aMeshIndex.init(aBuf, gMeshField)){
							IFeaturePtr ipFeature;
							LONG aMeshCode;
							if((ipFeature = aMeshIndex.nextFeature(&aMeshCode)) != NULL){
								aChiban.execAdjust(ipFeature, aMeshCode);
							}
						}
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
				_ftprintf(stderr, _T("ファイルが開けない。:%s\n"), gListFile);
			}
		}else{
			if(aMeshIndex.init(_ttol(gMeshCode), gMeshField)){
				IFeaturePtr ipFeature;
				LONG aMeshCode;
				if((ipFeature = aMeshIndex.nextFeature(&aMeshCode)) != NULL){
					aChiban.execAdjust(ipFeature, aMeshCode);
				}
			}
		}
	}
	if(gChgMode){
		ipWorkspaceEdit->StopEditOperation();
		if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_TRUE ))){	// 編集を保存するなら VARIANT_TRUE にすること
			_ftprintf(stderr, _T("#保存成功。\n"));	
		}else{
			_ftprintf(stderr, _T("#変更が保存できない。\n"));	
		}
	}
	return ;
}

/**
 * @brief メイン関数。
 *
 * @param argc	[in]	コマンドラインのトークン数（？）
 * @param argv	[in]	コマンドラインのトークン列（？）
 *
 * @retval 0		正常終了（実はきっちり守られてはいない）
 * @retval 0以外	異常終了
 */
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
		return 1;
	}
	return 0;
}
