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

// RNSDelimitStream.cpp: RNSDelimitStream クラスのインプリメンテーション
//
// $Id: RNSDelimitStream.cpp,v 1.2 2003/05/16 09:36:19 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#include <string>
#include "RNSDelimitStream.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

/**
 * @brief	コンストラクタ
 *
 * 出力するストリームとデリミタを指定。
 * 
 * @param	pStream		[in]	出力ストリーム
 * @param	cDelimiter	[in]	デリミタ
 */
RNSDelimitStream::RNSDelimitStream(std::ostream* pStream, const char* cDelimiter) :
	mDelimNum(0),
	mStream(0)
{
	setStream(pStream);
	setDelimiter(cDelimiter);
}

//////////////////////////////////////////////////////////////////////
// 設定
//////////////////////////////////////////////////////////////////////

/**
 * @brief	出力ストリームを設定
 *
 * @param	pStream	[in]	出力ストリーム
 * @exception	std::invalid_arugment	pStreamがNULLである。
 */
void RNSDelimitStream::setStream(std::ostream* pStream)
{
	if(! pStream)
		throw std::invalid_argument(std::string("null stream pointer"));

	mStream = pStream;
}

/**
 * @brief	デリミタ追加（前置）
 *
 * 後値型のオペレータは面倒なので対応していない。
 * 
 * @return	自己
 */
RNSDelimitStream& RNSDelimitStream::operator ++ ()
{
	mDelimNum++;
	return *this;
}
