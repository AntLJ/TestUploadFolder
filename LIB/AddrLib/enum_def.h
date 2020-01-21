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

/**
 * @file	enum_def.h
 * @brief	AddrLibで使うenum定義値いろいろ
 */
#ifndef __ADDRLIB_ENUM_DEF__
#define __ADDRLIB_ENUM_DEF__

namespace addr
{

/**
 * @enum	addrPointClass
 * @brief	住所ポイント種別
 */
enum addrPointClass
{
	kGou,		//!< 号ポイント(GOU)
	kTpg,		//!< 号ポイント(TPG)
	kCS,		//!< CSポイント
	kKlink,		//!< 家形リンクポイント
	kUnknown,	//!< 不明
};

/**
 * @enum	addrStatus
 * @brief	住所ステータス
 */
enum addrStatus
{
	kNormal,			//!< 通常住所
	kExtGaiku,			//!< 拡張街区符号あり
	kMojiGou,			//!< 号番号に文字あり
	kMojiGou_ExtGaiku,	//!< 号番号に文字あり(拡張街区符号内)
	kSepOver,			//!< 号番号の区切りが多い
	kSepOver_ExtGaiku,	//!< 号番号の区切りが多い(拡張街区符号内)
	kNoOver,			//!< 地番・号で表現しきれない番号
	kNoOver_ExtGaiku,	//!< 地番・号で表現しきれない番号(拡張街区符号内)
};

/**
 * @enum	addrCodeStatus
 * @brief	住所コードステータス
 */
enum addrCodeStatus
{
	kShiko,		//!< 施行コード
	kTmpShiko,	//!< 仮施行コード
	kHaishi,	//!< 廃止コード
	kTmpHaishi,	//!< 仮廃止コード
	kInValid,	//!< 無効コード
};

}	// namespace addr

#endif	// __ADDRLIB_ENUM_DEF__
