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

#ifndef __RNS_OPTION__
#define	__RNS_OPTION__

/* ************************************************

 	初稿 :
 	改訂 :  
 	追記 : 
 
 	制作 :	Fukuta.K
 	
************************************************* */

/* ----- 定義 ----- */

#include <RNSObject.h>
#include <RNSString.h>

/* ////// クラス定義  ////// */

/**
 *	オプション記録クラス
**/
class RNSOption : public RNSObject {
	char			mSimbol;		// オプション識別子 ("-a"の a とか）
	RNSStringPtr	mLongName;		// 未使用
	bool			mNeadValue;		// -a 150 のように、値を必要とするか

	bool			mFlag;			// オプションが入っています
	RNSStringPtr	mValue;			// 値

	// 全部０に
	void init();

 public:
	RNSOption();
	RNSOption(char iSimbol, bool iNeadValue/* = false*/);

	/// オプションが指定されました！
	void		setFlag(bool iFlag = true);
	/// 値設定
	void		setValue(const char *iValue);

	/// このオプションは指定されましたか？
	bool		flag() const;
	/// 値を取得
	const char* value() const;

	/// 値を必要とするオプションですか？
	bool		isNeadValue() const;

	/// オペレータ
	bool		operator < (const RNSOption &iOption) const;
				operator char () const;
				operator bool () const;
};

_RNSOBJECT_TYPEDEF(RNSOption);

#endif /* __RNS_OPTION__ */
