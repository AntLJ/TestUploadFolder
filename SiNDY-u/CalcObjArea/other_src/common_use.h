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

#ifndef	__COMMON_USE_H__
#define	__COMMON_USE_H__

#include "StdAfx.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>

using namespace	std;

//-----------------------------------------
// 共通で使用できるものについてここにまとめる
//-----------------------------------------


class	common_use
{

public:
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

	//-----------------------------------------------------------
	// メッシュコードのリストを作成する
	// @return	成功	true
	// @return	失敗	false
	//-----------------------------------------------------------
	bool	MakeMeshList(
		char*		c_cpRFName,	///< メッシュリストファイル名[IN]
		set<int>*	c_spMList	///< メッシュリストへのポインタ[OUT]	
	);

//	string	Str_to_SJIS	(
//		CComBSTR*	c_CStr		///< 文字列(Unicode)
//	);

	enum	SRC_STR_MODE {
		UNICODE		= 0x01,	///< SiNDY上のデータの場合
		SJIS		= 0x02	///< PGDB上のデータの場合
	};

	string	Str_to_SJIS	(
		CComBSTR&	c_CStr,	///< 文字列(Unicode)
		int			c_iMode	///< 文字変換モード
	);

	unsigned	Rev_Int(void* c_vpBuffer)
	{
		char	a_cpBuffer[4];
		a_cpBuffer[0] = ((char*) c_vpBuffer)[3];
		a_cpBuffer[1] = ((char*) c_vpBuffer)[2];
		a_cpBuffer[2] = ((char*) c_vpBuffer)[1];
		a_cpBuffer[3] = ((char*) c_vpBuffer)[0];
		return *((unsigned*) a_cpBuffer);
	}

	/**
		半角文字列を全角に直す（数字、アルファベットのみ）
	*/
	string	Hankaku_to_Zenkaku	( string	c_sStr	)	// 半角文字列
	{
		string		ret_str;

		long		length	= c_sStr.size();
		
		int	i = 0;
		for( i = 0; i < length; i++ )
		{
			// ASCII文字列の数値
			if( (0x30 <= c_sStr[i] && c_sStr[i] <= 0x39) ||
				(0x41 <= c_sStr[i] && c_sStr[i] <= 0x5a) ||
				(0x61 <= c_sStr[i] && c_sStr[i] <= 0x7a) )
			{
				ret_str	+= 0x82;
				ret_str	+= c_sStr[i]+0x1f;
			}else {
				ret_str	+= c_sStr[i];	// そのまま返す（範囲外の文字がはいるとたぶんおかしくなる）
			}
		}
		return	(ret_str);
	}
};



#endif
