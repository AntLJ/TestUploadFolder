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
#include "CSV.h"
#include "grobalfunc.h"
#include "LocalDefine.h"
#include <fstream>
#include <string> 	

CCSV::CCSV(void)
{
	m_MeshCodeIndex = 0;
}

CCSV::~CCSV(void)
{
}

CCSV::CCSV( LPCTSTR lpcszFilepeth )
{
	std::ifstream ifs( lpcszFilepeth );

	// 1行目のフィールド名定義を拾う
	std::string strLine;
	std::getline( ifs, strLine );
	m_vecField = gf::Tokenize2Vector( CString( CA2CT( strLine.c_str() ) ) , _T(","), _T("\"") );

	// メッシュコードのインデックス番号を覚えとく
	int i=0;
	for( std::vector<CString>::const_iterator it = m_vecField.begin(); it != m_vecField.end(); ++it )
	{
		if( it->CompareNoCase( MESHCORD_FIELDNAME ) == 0 )
		{
			m_MeshCodeIndex = i;
			break;
		}
		i++;
	}

	while(ifs && std::getline(ifs, strLine) ) 
	{
		std::vector<CString> vecRecord = gf::Tokenize2Vector( CString( CA2CT( strLine.c_str() ) ) , _T(","), _T("\"") );
		m_RowMap[ _ttol(vecRecord[m_MeshCodeIndex]) ] = vecRecord;
	}
}
