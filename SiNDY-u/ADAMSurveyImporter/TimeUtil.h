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

/**
 * 時間系ユーティリティクラス
 */
class TimeUtil
{
public:
	TimeUtil(void);
	~TimeUtil(void);

	/**
	 * 現在日時の文字列取得
	 *
	 * @return	現在日時(%4d%2d%2d_%2d%2d%2d (ex 20151215_182534))
	 */
	static uh::tstring GetTime();

	/**
	 * 現在日付の文字列取得
	 *
	 * @return	現在日時(%4d/%2d/%2d(ex 2015/12/15))
	 */
	static uh::tstring GetDate();
};

