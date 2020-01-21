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
#include "BaseRepPoint.h"

#include "AreaCodeHelper.h"

namespace addr
{
// �Z�����̎擾
CString CBaseRepPoint::GetAddrName( bool bChar /* = false */ )
{
	return g_cAreaCodeHelper.IsConnected()? g_cAreaCodeHelper.GetAddrKanji(GetAddrCode()) : _T("");
}

}	// namespace addr
