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

// UseAddrCodeList.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

/**
 * @file	UseAddrCodeList.cpp
 * @brief	AddrLibのAddrCodeListクラスを用いた住所コードリストをハンドルするサンプルコード
 */

#include "stdafx.h"
#include <SiNDYLib/Workspace.h>
#include <AddrLib/AddrCodeList.h>

using namespace std;
using namespace sindy;
using namespace addr;

// コードリスト出力用関数オブジェクト
struct CodePrint
{
	void operator()( const _TCHAR* lpcszCode ){ wcout << lpcszCode << endl; }
};

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, L"japanese" );

	::CoInitialize(0);
	{
		// [SiNDYLib]
		// ワークスペース取得
		CWorkspace cWorkspace( _T("..\\..\\Test\\53392442.mdb") );

		// [SiNDYLib/AddrLib]
		// 都市地図行政界フィーチャクラスでCAddrCodeListを初期化
		// ※(3)(4)でしか使用しない場合(＝行政界使用しない)は、デフォルトコンストラクタで呼び出すだけでよい)
		CAddrCodeList cList( cWorkspace.OpenTable(schema::city_admin::kTableName) );

		// [AddrLib]
		// (1) 都市行政界から全国分の市区町村コードリストを作成
		if( cList.MakeCodeListAll(5) )
		{
			wcout << L"[都市行政界内の全ポリゴンから市区町村コードリスト生成]" << endl;
			for_each( cList.begin(), cList.end(), CodePrint() );
		}
		wcout << endl;

		// (2) 都市行政界から指定した範囲内で11桁住所コードを作成
		cList.clear();
		if( cList.MakeCodeListFromRange( _T("01000000000"), _T("02000000000")) )
		{
			wcout << L"[都市行政界内の全ポリゴンから指定範囲内の住所コードリスト生成]" << endl;
			for_each( cList.begin(), cList.end(), CodePrint() );
		}
		wcout << endl;

		// (3) 外部ファイルをロード
		cList.clear();
		if( cList.AddFromFile(_T("testlist.txt"), 8) )
		{
			wcout << L"[外部ファイルから住所コードリスト生成]" << endl;
			for_each( cList.begin(), cList.end(), CodePrint() );
		}
		wcout << endl;

		// (4) 単純にコード追加
		cList.clear();
		const _TCHAR* szCodeList[] = { _T("01"), _T("02") };
		for( int i = 0; i < 2; ++i )
			cList.Add( szCodeList[i] );
		wcout << L"[単純にコードリスト生成]" << endl;
		for_each( cList.begin(), cList.end(), CodePrint() );
		wcout << endl;

	}
	::CoUninitialize();

	return 0;
}
