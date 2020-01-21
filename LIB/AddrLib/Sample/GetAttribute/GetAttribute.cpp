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

// GetAttribute.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

/**
 * @file	GetAttribute,cpp
 * @brief	AddrLibを用いた属性取得サンプルコード
 */

#include "stdafx.h"
#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>
#include <AddrLib/Initializer.h>
#include <AddrLib/GouPoint.h>
#include <AddrLib/CityAdmin.h>

using namespace std;
using namespace sindy;
using namespace addr;


int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, L"japanese");

	::CoInitialize(0);
	{
		// [AddrLib]
		// 初期化
		// ※ 文字付き考慮した住所コードを扱いたい時は、必ずInitializer.h の Init_CodeConverter関数を呼ぶ
		// ※ 文字付き考慮した住所ヨミを取得したい時は、必ずInitializer.h の Init_YomiConverter関数を呼ぶ
		// ※ 住所コードから名称取得したい時は、必ずInitializer.h の Init_YomiConverter関数を呼ぶ
		if( Init_AreaCodeHelper() && Init_CodeConverter(_T("..\\..\\Test\\CodeConvTable.mdb")) )
		{
			// [SiNDYLib]
			// ワークスペース取得
			CWorkspace cWorkspace( _T("..\\..\\Test\\53392442.mdb") );

			// [SiNDYLib]
			// 都市地図行政界フィーチャクラス取得
			// 号都市地図行政界フィーチャクラス取得
			CFeatureClass cAdminFC( cWorkspace.OpenTable(schema::city_admin::kTableName) );
			CFeatureClass cGouFC( cWorkspace.OpenTable(schema::gou_point::kTableName) );

			// [SiNDYLib/AddrLib]
			// 都市地図行政界フィーチャ取得
			// 号ポイントフィーチャ取得
			CCityAdmin cAdmin( cAdminFC.GetFeature(70) );
			CSPFieldMap cFields( cGouFC.GetFieldMap() );
			CSPTableNameString cNames( cGouFC.GetNameString() );
			CGouPoint cGou( cGouFC.GetFeature(8), sindyTableType::gou_point, false, cFields, cNames );

			cGou.GetShape();

			// [AddrLib]
			// 行政界属性取得
			wcout << L"[都市地図行政界属性取得]" << endl;
			wcout << L"市区町村コード           : " << LPCTSTR(cAdmin.GetCityCode()) << endl;
			wcout << L"大字・字コード           : " << LPCTSTR(cAdmin.GetOazaAzaCode()) << endl;
			wcout << L"街区符号                 : " << cAdmin.GetGaikuFugo() << endl;
			wcout << L"拡張街区符号             : " << LPCTSTR(cAdmin.GetExGaikuFugo()) << endl;
			wcout << L"住所コード               : " << LPCTSTR(cAdmin.GetAddrCode()) << endl;
			
			// ※住所コードは一度取得するとキャッシュされるので、文字非考慮で住所コードを取得後に
			// 文字付き考慮で住所コードを取得する場合は一度キャッシュを削除する必要がある
			cAdmin.DeleteCache();
			wcout << L"住所コード(文字付き考慮) : " << LPCTSTR(cAdmin.GetAddrCode(true)) << endl;

			wcout << endl;

			// [AddrLib]
			// 号ポイント属性取得
			wcout << L"[号ポイント属性取得]" << endl;
			wcout << L"号番号                   : " << LPCTSTR(cGou.GetGouNo()) << endl;
			wcout << L"号タイプ                 : " << cGou.GetGouType() << endl;
			wcout << L"住所コード               : " << LPCTSTR(cGou.GetAddrCode()) << endl;
			// ※住所コードは一度取得するとキャッシュされるので、文字非考慮で住所コードを取得後に
			// 文字付き考慮で住所コードを取得する場合は一度キャッシュを削除する必要がある
			cGou.DeleteCache();
			wcout << L"住所コード(文字付き考慮) : " << LPCTSTR(cGou.GetAddrCode(true)) << endl;
		}
	}
	::CoUninitialize();

	return 0;
}

