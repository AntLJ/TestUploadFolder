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

#include "stdafx.h"
#include "AutoValue.h"
#include "util.h"

using namespace sindy;

namespace {
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR RULE_IGNORE_FIELD_OR_VALUE_EMPTY  = _T("%s : Automatically added attribute rule:\"%s\" : A field name or a value is empty. This rule is ignored.\n");
#else
	LPCTSTR RULE_IGNORE_FIELD_OR_VALUE_EMPTY  = _T("%s : 自動付与属性ルール：\"%s\" : フィールド名又は値が空です。このルールは無視されます。\n");
#endif	// SINDY_FOR_ENGLISH
} // namespace


CAutoValue::CAutoValue(void)
{
}

CAutoValue::~CAutoValue(void)
{
}

void CAutoValue::Init( LPCTSTR lpcszAutoValue )
{
	// 解析
	int iStart = 0;
	CString strTokenize( lpcszAutoValue );
	CString strToken = strTokenize.Tokenize( _T("|"), iStart );
	while( ! strToken.IsEmpty() )
	{
		int iStart2 = 0;
		CString strFieldName = strToken.Tokenize(_T("="), iStart2 );
		CString strValue     = strToken.Tokenize(_T("="), iStart2 );

		if( ! ( strFieldName.IsEmpty() || strValue.IsEmpty() ) )
			m_listValue.push_back( std::pair<CString,CString>(strFieldName, strValue) );
		else {
			TRACEMESSAGE( RULE_IGNORE_FIELD_OR_VALUE_EMPTY, __func__, lpcszAutoValue );
			_ASSERTE(false);	// 変な値がセットされている
		}

		strToken = strTokenize.Tokenize(_T("|"), iStart );
	}
}
