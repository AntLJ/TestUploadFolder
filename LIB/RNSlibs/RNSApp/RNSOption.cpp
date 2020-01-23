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

/* ************************************************
	初稿 :
	改訂 :  
	追記 : 
 
	制作 :  Fukuta.K
************************************************* */

/* ----- インクルードファイル ----- */

#include <iostream>

#include "RNSOption.h"

// ===== public メソッド ===== //

// --- 初期化 --- //

/// デフォルトコンストラクタ
RNSOption::RNSOption()
{
	init();
}

/// コンストラクタ
RNSOption::RNSOption(char iSimbol, bool iNeadValue)
{
	init();
	mSimbol = iSimbol;
	mNeadValue = iNeadValue;
}

/// 初期化
void RNSOption::init()
{
	mSimbol = 0;
	mNeadValue = false;
	mFlag = false;
}

// --- 設定　--- //

/// オプションが指定されました！
void RNSOption::setFlag(bool iFlag)
{
	mFlag = iFlag;
}

/// 値設定
void RNSOption::setValue(const char *iValue)
{
	mValue = new RNSString(iValue);
	setFlag();
}

// --- 取得 --- //

/// このオプションは指定されましたか？
bool RNSOption::flag() const 
{
	return mFlag;
}

/// 値を取得
const char* RNSOption::value() const 
{
	return mValue ? mValue->c_str() : 0;
}

// --- 判定 --- //
/// 値を必要とするオプションですか？
bool RNSOption::isNeadValue() const {return mNeadValue;}

// --- オペレータ --- //
/// オプション取得
RNSOption::operator char () const
{
	return mSimbol;
}

/// オプションがセットされたか判定
RNSOption::operator bool () const
{
	return mFlag;
}

/// オプションの大小判定
bool RNSOption::operator < (const RNSOption &iOption) const
{
	return (char)*this < (char)iOption;
}

