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
#include "BaseAdmin.h"
#include "AreaCodeHelper.h"

namespace addr
{
	// 市区町村名取得
	CString CBaseAdmin::GetCityName() const
	{
		if( m_strCityName.IsEmpty() )
			m_strCityName = g_cAreaCodeHelper.IsConnected()? g_cAreaCodeHelper.GetAddrKanji(GetCityCode()) : _T("");	// 住所DB接続していなかったら空文字返す
		return m_strCityName;
	}
}	// namespace addr
