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

// GetGouPointList.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

/**
 * @file	GetGouPointList.cpp
 * @brief	行政界を検索し、その下にある号ポイントを取得するサンプルコード
 */

#include "stdafx.h"
#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>
#include <AddrLib/GouPoints.h>

using namespace std;
using namespace sindy;
using namespace addr;


int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, L"japanese" );

	::CoInitialize(0);
	{
		// [SiNDYLib]
		// ワークスペース取得
		CWorkspace cWorkspace( _T("..\\..\\Test\\53392442.mdb") );

		// [SiNDYLib/AddrLib]
		// 都市地図行政界フィーチャクラス取得
		CGouPoints cGous( cWorkspace.OpenTable(schema::city_admin::kTableName) );

		// [AddrLib]
		// 指定住所コードを持つ行政界を検索
		cGous.SelectByAddrCode( _T("01101") );
		
		// [AddrLib]
		// 検索ヒット数
		wcout << L"住所コード[01101]の行政界ポリゴン数 : " << cGous.size(cGous.GetTableType()) << endl;
		wcout << endl;

		// [AddrLib]
		// 各行政界内にある号ポイント取得・住所コード出力
		for( CCityAdmin::const_rows_iterator itAdmin = cGous.begin(cGous.GetTableType()); itAdmin != cGous.end(cGous.GetTableType()); ++itAdmin )
		{
			// 行政界内にある号ポイント取得
			const CSPCityAdmin pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
			wcout << L"[行政界(" << pAdmin->GetOID() << L")以下の号ポイント " << pAdmin->size(sindyTableType::gou_point) << L"出力]" << endl;
			for( CGouPoint::const_rows_iterator itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou )
			{
				const CSPGouPoint pGou = boost::static_pointer_cast<CGouPoint>(*itGou);
				wcout << L"\t号ポイント(" << pGou->GetOID() << L") : " << LPCTSTR(pGou->GetAddrCode()) << endl;
			}
			wcout << endl;
		}
	}
	::CoUninitialize();

	return 0;
}

