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

// NULL判定
// TODO：バスライブラリ取り込みまでの臨時対応のため、取り込み後削除
inline bool consider_as_null( const CComVariant& va )
{
	switch( va.vt )
	{
	case VT_NULL:
	case VT_EMPTY:
		return true;
		break;
	case VT_BSTR:
		if( 0 == _wcsicmp( va.bstrVal, L"" ) )
			return true;
		break;
	case VT_BOOL:
		if( VARIANT_FALSE == va.boolVal )
			return true;
	default:
		break;
	}
	return false;
}
