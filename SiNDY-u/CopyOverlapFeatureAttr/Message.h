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
#pragma once
#include <tchar.h>

namespace message 
{
	enum ECode
	{
		// MESSAGE_TABLEの第一引数だけを使用してenumを定義
#define MESSAGE_TABLE( code, message ) code,
#include "MessageTable.h"
#undef MESSAGE_TABLE
	};

	/**
	 * @brief 指定したコードに対応するメッセージを取得する
	 * @param [in] code  コード
	 * @return メッセージ
	 */
	const TCHAR* getMsg( ECode code );

} // namespace