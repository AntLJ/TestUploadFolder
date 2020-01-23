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

// CopyLayer.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "Pgdb.h"
#include <string>
#include <arctl/coinitializer.h>
#include <WinLib/VersionInfo.h>

namespace {
#ifdef _DEBUG
LPCTSTR DEF_SRC_DIR = ".\\Src\\";
//#define DEF_TGT_DIR "C:\\SVN\\SiNDY-u\\CopyLayer\\Tgt"
LPCTSTR DEF_TGT_DIR = ".\\Tgt\\";
#else
LPCTSTR DEF_SRC_DIR = ".\\";
LPCTSTR DEF_TGT_DIR = ".\\";
#endif

LPCTSTR gSrcDir = DEF_SRC_DIR;
LPCTSTR gTgtDir = DEF_TGT_DIR;

LPCTSTR gMeshCode = NULL;
LPCTSTR gSuffix = NULL;
LPCTSTR gMeshList = NULL;

bool gForceMode = false;
}

void PrintUsage( LPCTSTR App)
{
	 LPCTSTR AppName;

	AppName = strrchr(App, '\\');
	if(AppName != NULL){
		AppName++;
	}else{
		AppName = App;
	}
	printf( "Usage:%s [オプション] <MeshCode> <Suffix>\n"
			"      %s [オプション] -L <ListFile> <Suffix>\n"
			"　[オプション]\n"
			"\t-S <SrcDir>:コピー元のPGDBディレクトリ (default)%s\n"
			"\t-T <TgtDir>:コピー先のPGDBディレクトリ (default)%s\n"
			"\t-F         :既にフィーチャクラスがある場合でも、上書きします。\n"
			, AppName, AppName, DEF_SRC_DIR, DEF_TGT_DIR);
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
					case 'l':
					case 'L':
						i++;
						gMeshList = argv[i];
						break;
					case 's':
					case 'S':
						i++;
						gSrcDir = argv[i];
						break;
					case 't':
					case 'T':
						i++;
						gTgtDir = argv[i];
						break;
					case 'f':
					case 'F':
						gForceMode = true;
						break;
					case '?':
						PrintUsage(argv[0]);
						return false;
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
		if(gMeshList == NULL){
			if(i == argc -2){
				gMeshCode = argv[i];
				gSuffix = argv[i+1];
			}else{
				PrintUsage(argv[0]);
				return false;
			}
		}else{
			if(i == argc -1 ){
				gSuffix = argv[i];
			}else{
				PrintUsage(argv[0]);
				return false;
			}
		}
	}
	return true;
}

void Execute()
{
	Pgdb aPgdb;
	if(aPgdb.init(gSrcDir, gTgtDir, gSuffix, gForceMode)){
		if(gMeshList == NULL){
			printf("%s.%s実行中・・・\n", gMeshCode, gSuffix);
			aPgdb.execCopyLayer(atol(gMeshCode));
		}else{
			FILE *fp;
			if((fp = fopen(gMeshList, "rt")) != NULL){
				char aBuf[16];
				while(fgets(aBuf, 16, fp) != NULL){
					aBuf[8] = '\0';
					printf("%s.%s実行中・・・\n", aBuf, gSuffix);
					aPgdb.execCopyLayer(atol(aBuf));
					fflush(stderr);
					printf("終了\n");
				}
				fclose(fp);
			}else{
				fprintf(stderr, "#リストファイルが開けません。,%s\n", gMeshList);
			}
		}
	}
	return ;
}

int main(int argc, char* argv[])
{
	const arctl::coinitializer coinit;
	try {
		time_t st_time;
		time(&st_time);
		if(AnalyseArg(argc, argv)){
			// ツールバージョン
			CVersion cVer;
			fprintf(stderr, "#%s FILEVERSION %s PRODUCTVERSION %s\n", static_cast<LPCSTR>(cVer.GetOriginalFilename()), static_cast<LPCSTR>(cVer.GetFileVersion()), static_cast<LPCSTR>(cVer.GetProductVersion()));
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
