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
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>

#ifndef	_UTIL_CLASS_H_
#define	_UTIL_CLASS_H_

using	namespace	std;

class	util_class
{
public:
	enum	SRC_STR_MODE {
		UNICODE		= 0x01,	///< SiNDY上のデータの場合
		SJIS		= 0x02	///< PGDB上のデータの場合
	};

	string	Str_to_SJIS	(
		CComBSTR&	c_CStr,		///< 文字列(Unicode)
		int			c_iMode		///< 文字変換モード
	);

	//-----------------------------------------------------------
	// メッシュコードのリストを作成する
	// @return	成功	true
	// @return	失敗	false
	//-----------------------------------------------------------
	bool	MakeMeshList(
		char*		c_cpRFName,	///< メッシュリストファイル名[IN]
		set<int>*	c_spMList	///< メッシュリストへのポインタ[OUT]
	);

	//------------------------------------------------------
	// 字句で文字列を分割する（元の文字列を分割しない）
	// @return  取り出した文字列の個数
	//------------------------------------------------------
	int		DivbyToken(
		char		*c_cpString,	///< 検索対象文字列
		char		*c_cpToken,		///< 区切り文字列
		int			c_iStrNum,		///< 先頭から何番目の文字列から取り出すか
		int			c_iN,			///< 何個の文字列を取り出すか
		string*		c_cpStrPtr		///< 取り出した文字列のポインタ
	);

};

#endif

