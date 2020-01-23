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
#include <tchar.h>

/**
 * @note 列挙型の接頭辞の意味は下記の通り。
 *       ・「i」：Information。エラーではない情報。
 *       ・「e」：Error。エラー情報。
 */
namespace rns_message
{
	enum ECode
	{
		// MESSAGE_TABLEの第一引数だけを使用してenumを定義。
#define MESSAGE_TABLE( code, japanese, english ) code,
#include "RNSMessageTable.h"
#undef MESSAGE_TABLE
	};

	/**
	 * @brief 指定したコードに対応するメッセージを取得する
	 * @param[in]  code  コード（このファイルの上で定義されたenumのいずれか）
	 * @return メッセージ
	 */
	const char* getMsg( ECode code );

} // namespace message
