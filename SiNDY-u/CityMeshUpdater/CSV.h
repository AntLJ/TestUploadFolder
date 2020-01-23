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

#pragma once

#include<map>
#include<vector>

class CCSV
{
public:
	CCSV(void);
	CCSV( LPCTSTR lpcszFilepeth );
	~CCSV(void);

	std::map< long, std::vector<CString> > m_RowMap;		// < OID、属性値vector >
	std::vector<CString> m_vecField;						// フィールド名vector

	long GetRecords(){ return m_RowMap.size(); }

private:
	int m_MeshCodeIndex;
};
