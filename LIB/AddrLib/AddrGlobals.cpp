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
#include "AddrGlobals.h"

namespace addr
{
	/**
	 * @brief	数字のみで構成された文字列？
	 * @param	lpcszData	[in]	チェック対象文字列
	 * @return	bool
	 */
	bool IsNumString( const _TCHAR* lpcszData )
	{
		CString str( lpcszData );
		int nLen = str.GetLength();
		for( int i = 0; i < nLen; ++i )
		{
			if( !_istdigit(str.GetAt(i)) )
				return false;
		}
		return true;
	}


} // addr
