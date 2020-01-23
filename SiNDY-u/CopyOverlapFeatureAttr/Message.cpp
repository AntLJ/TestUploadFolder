/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved. 
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
#include "Message.h"

// 指定したコードに対応するメッセージを取得する
const TCHAR* message::getMsg( ECode code )
{
#define MESSAGE_TABLE( code, message ) case code: return message;
	switch( code )
	{
#include "MessageTable.h"
	default: return _T("");		
	}
#undef MESSAGE_TABLE
}
