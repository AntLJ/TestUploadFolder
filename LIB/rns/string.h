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
 * @file string.h
 * @brief 汎用テキストクラス tstring を定義するクラス。
 */
#ifndef RNS_STRING_H_
#define RNS_STRING_H_

#include <string>

namespace rns {

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif // _UNICODE

} // namespace rns

#endif // RNS_STRING_H_
