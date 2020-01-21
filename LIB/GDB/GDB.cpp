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

/**
 * @file
 * @note	GDB.hで定義されたインライン関数を非インライン化するためにcppファイルを追加。
 *			インライン関数のままだとUnicodeビルドが通らず、Unicode構成からincludeして使用する事ができなかったので、
 *			実装をcppに移し、その部分がincludeされないようにして回避する。
 */

#include "GDB.h"

bool lessVariant::operator ()( const VARIANT& val1, const VARIANT& val2 ) const
{
	assert( val1.vt != VT_NULL );
	assert( val2.vt != VT_NULL );

	CComVariant vaVal1, vaVal2;
	vaVal1.ChangeType( VT_BSTR, &val1 );
	vaVal2.ChangeType( VT_BSTR, &val2 );
	USES_CONVERSION;
	std::string str1(OLE2T(vaVal1.bstrVal));
	std::string str2(OLE2T(vaVal2.bstrVal));
	return str1 < str2;
}

HRESULT GDBGetLocalTime(std::string *cTime)
{
	SYSTEMTIME aLocalTime;
	::GetLocalTime(&aLocalTime);
	char buff[BUFSIZ];
	(void)sprintf(buff, "%04d/%02d/%02d %02d:%02d:%02d", aLocalTime.wYear, aLocalTime.wMonth, aLocalTime.wDay, aLocalTime.wHour, aLocalTime.wMinute, aLocalTime.wSecond);
	*cTime = buff;
	return S_OK;
}
