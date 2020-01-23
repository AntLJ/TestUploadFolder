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

// �L���b�V���쐬
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
				long lGrpCode	= _tstol(strBuff.Tokenize(L"\t\n", nPos));	// ���ރR�[�h
				strBuff.Tokenize( L"\t\n", nPos ); // ���ޖ��ǂݎ̂�
				long lChainCode	= _tstol(strBuff.Tokenize(L"\t\n", nPos)); // �X�܃R�[�h
				strBuff.Tokenize( L"\t\n", nPos ); // �X�ܖ�

				POIUSABLEINFO info;
				info.bQualityF	= (strBuff.Tokenize( L"\t\n", nPos ) == L"1");	// �ʒu�i������t���O
				info.bPPRateF	= (strBuff.Tokenize( L"\t\n", nPos ) == L"1");	// PP������t���O
				info.bImportantF= (strBuff.Tokenize( L"\t\n", nPos ) == L"1");	// �d�v�R���e���c�t���O�iBug9215�Ή��F�t�H�[�}�b�g�ύX���A1��ǉ��j

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
		wcerr << L"#Error �t�@�C���I�[�v���Ɏ��s : " << lpcszFile << endl;
		return false;
	}
}
