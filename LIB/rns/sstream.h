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
 * @file sstream.h
 * @brief 汎用テキストストリームクラス tstringstream を定義するクラス。
 */
#ifndef RNS_SSTREAM_H_
#define RNS_SSTREAM_H_

#include <sstream>

namespace rns {

#ifdef _UNICODE
typedef std::wstringstream tstringstream;
typedef std::wostringstream tostringstream;
typedef std::wistringstream tistringstream;
#else
typedef std::stringstream tstringstream;
typedef std::ostringstream tostringstream;
typedef std::istringstream tistringstream;
#endif // _UNICODE

} // namespace rns

#endif // RNS_SSTREAM_H_
