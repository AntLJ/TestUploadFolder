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

#include "StdAfx.h"
#include "MasterListCache.h"

using namespace std;

CMasterListCache::CMasterListCache(void)
{
}

CMasterListCache::~CMasterListCache(void)
{
}

// キャッシュ作成
bool CMasterListCache::Create( const _TCHAR* lpcszFile )
{
	wifstream ifs;
	ifs.imbue( std::locale("") );
	ifs.open( static_cast<CT2CW>(lpcszFile) );
	if( ifs )
	{
		while( !ifs.eof() )
		{
			wchar_t szBuff[256] = L"";
			ifs.getline( szBuff, sizeof(szBuff) );

			CString  strBuff(szBuff);
			if( !strBuff.IsEmpty() )
			{
				int nPos = 0;
				long lGrpCode	= _tstol(strBuff.Tokenize(L"\t\n", nPos));	// 分類コード
				strBuff.Tokenize( L"\t\n", nPos ); // 分類名読み捨て
				long lChainCode	= _tstol(strBuff.Tokenize(L"\t\n", nPos)); // 店舗コード
				strBuff.Tokenize( L"\t\n", nPos ); // 店舗名

				POIUSABLEINFO info;
				info.bQualityF	= (strBuff.Tokenize( L"\t\n", nPos ) == L"1");	// 位置品質向上フラグ
				info.bPPRateF	= (strBuff.Tokenize( L"\t\n", nPos ) == L"1");	// PP率向上フラグ
				info.bImportantF= (strBuff.Tokenize( L"\t\n", nPos ) == L"1");	// 重要コンテンツフラグ（Bug9215対応：フォーマット変更し、1列追加）

				multimap<long, map<long, POIUSABLEINFO> >::iterator it = m_mapCache.find( lGrpCode );
				if( it != m_mapCache.end() )
				{
					it->second.insert( pair<long, POIUSABLEINFO>(lChainCode, info) );
				}
				else
				{
					map<long, POIUSABLEINFO> rec;
					rec.insert( pair<long, POIUSABLEINFO>(lChainCode, info) );
					m_mapCache.insert( pair<long, map<long, POIUSABLEINFO> >(lGrpCode, rec) );
				}
			}
		}
		return true;
	}
	else
	{
		wcerr << L"#Error ファイルオープンに失敗 : " << lpcszFile << endl;
		return false;
	}
}
