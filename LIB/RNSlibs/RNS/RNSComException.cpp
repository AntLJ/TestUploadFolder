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

// RNSComException.cpp: RNSComException クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#if(0)

#include <afx.h>
#include <atlbase.h>

#include "RNSComException.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RNSComException::RNSComException(HRESULT cHr, const char *iMessage, const char *iLocation)
	: RNSException(iMessage, iLocation)
{
	mHr = cHr;
}

RNSComException::~RNSComException()
{

}

//////////////////////////////////////////////////////////////////////
// 取得
//////////////////////////////////////////////////////////////////////

HRESULT RNSComException::hr() const
{
	return mHr;
}

#endif
