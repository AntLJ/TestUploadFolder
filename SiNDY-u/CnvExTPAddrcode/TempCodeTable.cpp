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

// TempCodeTable.cpp: CTempCodeTable クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TempCodeTable.h"

// 仮コード対応表ファイルロード
bool CTempCodeTable::load( LPCTSTR lpcszFile )
{
	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( !ifs )
		return false;

	while( !ifs.eof() )
	{
		std::string str;
		std::getline( ifs, str );

		if( str.empty() )
			continue;

		m_mapTmpTable.insert( std::pair<CString, CString>(
								str.substr(0, str.find('\t')).c_str(),
								str.substr(str.find('\t')+1).c_str()
								)
							);
	}

	m_bIsSet = true;

	return true;
}
