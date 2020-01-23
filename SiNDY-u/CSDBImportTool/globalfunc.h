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

#include "stdafx.h"
#include <map>
#include <vector>
#include <list>

namespace gf {
std::vector<CString> Tokenize2Vector( const CString & strSrc, LPCTSTR lpcszTorkens, LPCTSTR lpcszTrim = NULL );

CString GetIniFileValue( LPCTSTR lpcszFilepath, LPCTSTR lpcszSection, LPCTSTR lpcszKey );

int GetArraySize( tFieldSchema *tFields );

CString DummyStr( int n );

bool CheckFieldValue( IField* ipField, CComVariant& vaVal, CString& strErr );


CString Var2Str( const CComVariant& vaVal, CString strDefault = _T("") );
long Var2Long( const CComVariant& vaVal, long lDefault = -1 );
void IDs2Str( const std::list<CString>& listIDs, std::list<CString>& strIDs, LPCTSTR str_sep = NULL );

}
