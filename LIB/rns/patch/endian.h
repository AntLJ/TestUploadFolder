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

#ifndef RNS_PATCH_ENDIAN_H_
#define RNS_PATCH_ENDIAN_H_

// https://svn.boost.org/trac/boost/ticket/7516 対策
// 2013/6/3時点最新（1.53.0）のboost/endian.hppは以下の理由によりバイトオーダーを認識できない環境がある。
// ・g++でendian.hを取込んでエンディアン環境を識別することができない
// ・__ARMEL__をリトルエンディアン環境と認識しない
// そこで、g++環境でエンディアン指定がない場合、endian.hを取込んでboost/endian.hppが認識できるエンディアン指定を行う。
#if defined(__GNUC__) && ! defined(__LITTLE_ENDIAN__) && ! defined(__BIG_ENDIAN__)
# include <endian.h>
# if defined(__BYTE_ORDER__)
#  if (__BYTE_ORDER__ == __LITTLE_ENDIAN)
#   define __LITTLE_ENDIAN__
#  elif (__BYTE_ORDER__ == __BIG_ENDIAN)
#   define __BIG_ENDIAN__
#  endif // __BYTE_ORDER__ == __LITTLE/BIG_ENDIAN
# endif // defined(__BYTE_ORDER__)
// ※注
// boost/endian.hppは「_LITTLE_ENDIANが定義されていて_BIG_ENDIANが定義されていない」状態もリトルエンディアン環境と認識する実装となっているが、
// Android NDKのendian.hは「_BIG_ENDIANと_LITTLE_ENDIANの両方を定義する（__BIG_ENDIANや__LITTLE_ENDIANと同じ値にする）」ため、そちらで判断させることはできない。
// そのため「__LITTLE_ENDIAN__が定義されていて__BIG_ENDIAN__が定義されていない」状態で判定させるようにしている。
#endif // defined(__GNUC__) && ! defined(__LITTLE_ENDIAN__) && ! defined(__BIG_ENDIAN__)

#endif // RNS_PATCH_ENDIAN_H_
