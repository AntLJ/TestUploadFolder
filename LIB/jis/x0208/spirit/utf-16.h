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

#ifndef JIS_X0208_SPIRIT_UTF_16_H_
#define JIS_X0208_SPIRIT_UTF_16_H_

#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
#ifndef BOOST_SPIRIT_NO_CHAR_TRAITS
#	define BOOST_SPIRIT_NO_CHAR_TRAITS 1
#endif // BOOST_SPIRIT_NO_CHAR_TRAITS
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)

#include <boost/version.hpp>
#if BOOST_VERSION <= 103400
#include <boost/spirit/core.hpp>
#else
#include <boost/spirit/include/classic_core.hpp>
#endif

namespace jis {

namespace x0208 {

namespace spirit {

#if BOOST_VERSION <= 103400
using namespace boost::spirit;
#else
using namespace boost::spirit::classic;
#endif

/**
 * @defgroup jis_x_0208_utf_16_parser JIS X 0208�Ή�UTF-16�����p�[�T
 *
 * @note
 * ��ꐅ�������p�p�[�T�Ƃ��͖����B�͈͂ŕ\����킯�ł��Ȃ��B<br>
 * �f����Shift JIS�ɕϊ�������������B
 */
//@{
const range<wchar_t> utf16_hiragana_p = range_p(L'��', L'��'); ///< JIS X 0208�Ή�UTF-16�����������i���`��j
const range<wchar_t> utf16_katakana_p = range_p(L'�@', L'��'); ///< JIS X 0208�Ή�UTF-16�Љ��������i�@�`���j
//@}

} // namespace spirit

} // namespace x0208

} // namespace jis

#endif // JIS_X0208_SPIRIT_UTF_16_H_
