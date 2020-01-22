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

// CatGentyDB.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "GentyDB.h"
#define DEF_PGDB_DIR ".\\"
#define DEF_PHOTO_DIR ".\\"

static LPCTSTR gOutFile = NULL;
static LPCTSTR gPgdbDir = DEF_PGDB_DIR;
static LPCTSTR gPhotoDir = DEF_PHOTO_DIR;
static LPCTSTR gListFile = NULL;
static bool gTextModeFlag = false;
static bool gNewRecordOnly = false;

void PrintUsage(LPCTSTR App)
{
	LPCTSTR AppName;
	AppName = strrchr(App, '\\');
	if(AppName != NULL){
		AppName++;
	}else{
		AppName = App;
	}
	printf( "現調用AccessDBをマージorテキスト出力するプログラム\n"
		    "Usage:%s [Options] <メッシュリストファイル> <出力ファイル>\n"
			"  Options:\n"
			"\t-A <Accessのディレクトリ>:(default)%s\n"
			"\t-P <写真のディレクトリ>:(default)%s\n"
			"\t-T :テキスト出力モード(default)AccessDB\n"
			"\t-N :更新されたレコードのみを対象とする\n"
			, AppName, DEF_PGDB_DIR, DEF_PHOTO_DIR);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("メッシュごとの現調用AccessDBを1つのAccessDBもしくはテキストに出力します。\n");
}

bool AnalyseArg(int argc, char* argv[])
{
	if(argc < 3){
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
						gPhotoDir = argv[i];
						break;
					case 't':
					case 'T':
						gTextModeFlag = true;
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
		if(i == argc -2){
			gListFile = argv[i];
			gOutFile = argv[i+1];
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

void End( LPCTSTR lpszMessage )
{
	printf(_T("%s\n"), lpszMessage );

	// 終了時には必ず呼んで下さい
	::CoUninitialize();
}

void Execute()
{
	if(gListFile != NULL){
		GentyDB aGentyDB;
		if(aGentyDB.init(gPgdbDir, gOutFile, gPhotoDir, gTextModeFlag, gNewRecordOnly)){
			FILE *pFile;
			if((pFile = fopen(gListFile, "rt")) != NULL){
				char aBuf[1024];
				while(fgets(aBuf, 1024, pFile) != NULL){
					aGentyDB.execCatGentyDB(atol(aBuf));
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
		End(_T("終了します"));
	}
	return 0;
}

