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

// ChkGentyDB.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "GentyDB.h"
#include <WinLib/VersionInfo.h>
#define DEF_PGDB_DIR ".\\"
#define DEF_IMAGE_DIR ".\\"

namespace {
	LPCTSTR gPgdbDir = DEF_PGDB_DIR;
	LPCTSTR gImageDir = DEF_IMAGE_DIR;
	LPCTSTR gListFile = NULL;
	bool gNewRecordOnly = false;
	bool gPhotoF = false;
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
	printf( "現調用AccessDBのチェックプログラム\n"
		    "Usage:%s [Options] <メッシュリストファイル>\n"
			"  Options:\n"
			"\t-A <Accessのディレクトリ>:(default)%s\n"
			"\t-P <写真のディレクトリ>:(default)%s\n"
			"\t-N :更新されたレコードのみを対象とする\n"
			"\t-T :写真のチェックは更新された所と撮影必須ポイントのみを対象とする\n"
			, AppName, DEF_PGDB_DIR, DEF_IMAGE_DIR);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("現調用AccessDBの論理チェックを行います。\n");
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
					case 'a':
					case 'A':
						i++;
						gPgdbDir = argv[i];
						break;
					case 'n':
					case 'N':
						gNewRecordOnly = true;
						break;
					case 'p':
					case 'P':
						i++;
						gImageDir = argv[i];
						break;
					case 't':
					case 'T':
						gPhotoF = true;
						break;
					case '?':
						PrintHelp(argv[0]);
						return false;
						break;
					default:
						fprintf(stderr, "不正なオプションが指定されました。[%c]\n", argv[i][1]);
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
	}
	return true;
}

bool GetEnv(void)
{
	return true;
}

void Execute()
{
	if(gListFile != NULL){
		GentyDB aGentyDB;
		if(aGentyDB.init(gPgdbDir, gImageDir, gNewRecordOnly, gPhotoF)){
			FILE *pFile;
			if((pFile = fopen(gListFile, "rt")) != NULL){
				char aBuf[1024];
				while(fgets(aBuf, 1024, pFile) != NULL){
					if(aBuf[0] != '#' && aBuf[0] != '\n'){
						aGentyDB.execCheckGentyDB(atol(aBuf));
					}
				}
				fclose(pFile);
			}else{
				fprintf(stderr, "リストファイルが開けません。,%s\n", gListFile);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	time_t st_time;
	time(&st_time);
	GetEnv();
	if(AnalyseArg(argc, argv)){
		// ツールバージョン
		CVersion cVer;
		fprintf(stderr, "#%s FILEVERSION %s PRODUCTVERSION %s\n", static_cast<LPCSTR>(cVer.GetOriginalFilename()), static_cast<LPCSTR>(cVer.GetFileVersion()), static_cast<LPCSTR>(cVer.GetProductVersion()));
		SYSTEMTIME  aTime;
		GetLocalTime(&aTime);
		fprintf(stderr, "#開始時刻: %04d/%02d/%02d %02d:%02d:%02d\n",
		aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
		aTime.wMinute, aTime.wSecond);
		// COM を使用できるようにするための初期化
		::CoInitialize(0);
		{
			// Execute the command
			Execute();
		}

		time_t tm;
		time(&tm);
		GetLocalTime(&aTime);
		fprintf(stderr, "#終了時刻: %04d/%02d/%02d %02d:%02d:%02d\n",
			aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
			aTime.wMinute, aTime.wSecond);
		printf( " %6.0f秒\n", difftime( tm, st_time));
		fprintf(stderr, "#トータル時間%6.0f秒\n", difftime( tm, st_time));
//	printf( " %6.0f秒\nPush any key!\n", difftime( tm, st_time));
		printf("終了します\n");

		// 終了時には必ず呼んで下さい
		::CoUninitialize();
	}
	return 0;
}

