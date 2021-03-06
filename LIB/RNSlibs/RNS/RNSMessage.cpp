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

#include "RNSMessage.h"

const char* rns_message::getMsg( ECode code )
{
	switch ( code )
	{
		// 日本語か英語かによって返す文字列を切り替える。
#ifdef _FOR_ENGLISH
#define MESSAGE_TABLE( code, japanese, english ) case code: return english;
#else
#define MESSAGE_TABLE( code, japanese, english ) case code: return japanese;
#endif
#include "RNSMessageTable.h"
		default:                                            return "";
#undef MESSAGE_TABLE
	}
}
