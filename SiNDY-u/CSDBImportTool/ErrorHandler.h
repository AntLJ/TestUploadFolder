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

#include <fstream>
#include <time.h>

enum ERR_CODE{
	ERR_FIELD,
	ERR_SHAPE,
	ERR_STORE,
	ERR_DELETE
};

class CErrorHandler
{
public:
	CErrorHandler(void);
	CErrorHandler(LPCTSTR lpszFilename){ Init(lpszFilename); }
	~CErrorHandler(void);

	bool CheckID( CString & strID );
	void CErrorHandler::Init( LPCTSTR lpszLogFile );
	void FieldErr( const CString& strID, const CString& strFieldNames );
	void GeomErr( const CString& strID );
	void SetResult( HRESULT hr, const CString &strFieldName, int nIndex, ERR_CODE emCode );

private:

	std::ofstream m_fout;				// ログファイル出力ストリーム

	clock_t m_time;
};
