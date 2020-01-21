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

// UseAreaCodeHelper.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <AddrLib/AreaCodeHelper.h>

/**
 * @file	UseAreaCodeHelper.cpp
 * @brief	AddrLibのAreaCodeHelperクラスを用いた住所マスタへのアクセスを助けるサンプルコード
 */

using namespace std;
using namespace addr;

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, _T("Japanese") );

	CAreaCodeHelper cAreaCodeHelper;

	// DBへ接続
	if( cAreaCodeHelper.Connect(_T("y0901e/y0901e@delltcp")) )
	{
		// 住所名称取得
		wcout << L"[住所名称取得]" << endl;
		wcout << L"01          : " << LPCWSTR(cAreaCodeHelper.GetAddrKanji(_T("01"))) << endl;
		wcout << L"01          : " << LPCWSTR(cAreaCodeHelper.GetAddrKana(_T("01"))) << endl;
		wcout << L"03201       : " << LPCWSTR(cAreaCodeHelper.GetAddrKanji(_T("03201"))) << endl;
		wcout << L"03201       : " << LPCWSTR(cAreaCodeHelper.GetAddrKana(_T("03201"))) << endl;
		wcout << L"14203013    : " << LPCWSTR(cAreaCodeHelper.GetAddrKanji(_T("14203013"))) << endl;
		wcout << L"14203013    : " << LPCWSTR(cAreaCodeHelper.GetAddrKana(_T("14203013"))) << endl;
		wcout << L"13104092004 : " << LPCWSTR(cAreaCodeHelper.GetAddrKanji(_T("13104092004"))) << endl;
		wcout << L"13104092004 : " << LPCWSTR(cAreaCodeHelper.GetAddrKana(_T("13104092004"))) << endl;
		wcout << endl;

		// 旧住所コードから新住所コード(合併後住所コード)取得
		wcout << L"[合併後住所コード取得]" << endl;
		wcout << L"03307001101(岩手県岩手郡玉山村大字芋田字芋田)→" 
			  << LPCWSTR(cAreaCodeHelper.GetNewestCode(_T("03307001101"))) << L"(" << LPCWSTR(cAreaCodeHelper.GetAddrKanji(cAreaCodeHelper.GetNewestCode(_T("03307001101")))) << L")" << endl;
		wcout << endl;

		// 新旧住所コードから旧住所コード(合併前住所コード)取得
		wcout << L"[合併前住所コード取得]" << endl;
		wcout << L"03201148101(岩手県盛岡市玉山区芋田字芋田)→" 
			  << LPCWSTR(cAreaCodeHelper.GetPrevCode(_T("03201148101"))) << L"(" << LPCWSTR(cAreaCodeHelper.GetAddrKanji(cAreaCodeHelper.GetPrevCode(_T("03201148101")))) << L")" << endl;
		wcout << endl;
	}

	return 0;
}

