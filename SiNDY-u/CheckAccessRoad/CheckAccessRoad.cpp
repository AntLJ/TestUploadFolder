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

// CheckAccessRoad.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "CheckAccessRoad.h"
#include <arctl/coinitializer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// 唯一のアプリケーション オブジェクト

CWinApp theApp;

using namespace std;

int _tmain(int cArgc, TCHAR* cArgv[], TCHAR* envp[])
{
	int nRetCode = 0;

	try
	{
		//ARC931対応。ライセンス認証（＋COMの初期化）
		const arctl::coinitializer aCoInitializer;

		//日本語ファイル名に対応する為
	    std::locale loc = std::locale();
	    loc = std::locale(loc, "japanese", std::locale::ctype);
	    std::locale::global(loc);

		// MFC の初期化および初期化失敗時のエラーの出力
		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)){
			// TODO: 必要に応じてエラー コードを変更してください。
			cerr << _T("Fatal Error: initialization failed") << endl;
			nRetCode = 1;
		}
		else{
			// 施設ポイントにアクセスする道路追跡処理エンジン起動
			CApp	*apApp = new CApp();
			if( apApp ){
				if( apApp->init(cArgc, cArgv)){
					//起動モード(m_JobNo)が入力されているか否かで挙動を変える。
					//起動モードが入力されていない場合対話モードで起動
					if(apApp->m_JobNo == 0)
					{
						do {
							if(apApp->showMenu() == 1)
								break;
							nRetCode = apApp->run();
							if(nRetCode == 1)
								return nRetCode;
							char	aCh;
							cout << "Enter to 'e' end. " << endl;
							cout << "Enter    'c' to restart. : ";
							cin >> aCh;
							if(( aCh == 'e' )||( aCh == 'q' )){
								break;
							}
						} while( 1 );
					}
					//起動モードが入力されている場合はそのまま実行
					else if(apApp->m_JobNo >= 1 && apApp->m_JobNo <= 4)
					{
						nRetCode = apApp->run();
						if (nRetCode == 1)
							return nRetCode;
					}
					//起動モードの入力がおかしい場合はエラー
					else
					{
						std::cerr << "Job_No　は　1～4 の間で設定してください" << std::endl;
						std::cerr << "異常終了" << std::endl;
						return 1;
					}
				}
				else
				{
					std::cerr << "config.txt の指定に誤りがあります" << std::endl;
					std::cerr << "異常終了" << std::endl;
					return 1;
				}
				delete apApp;
			}
			else
			{
				std::cerr << "道路追跡処理エンジンの起動に失敗しました" << std::endl;
				std::cerr << "異常終了" << std::endl;
				return 1;
			}
		}
	}
	catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "異常終了(例外)" << std::endl;
		return 2;
	}
	return nRetCode;
}


