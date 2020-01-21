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

#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
#ifndef BOOST_SPIRIT_NO_CHAR_TRAITS
#	define BOOST_SPIRIT_NO_CHAR_TRAITS 1
#endif // BOOST_SPIRIT_NO_CHAR_TRAITS
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)

#include <boost/version.hpp>
#if BOOST_VERSION <= 103400
#include <boost/spirit/core.hpp>
#include <boost/spirit/utility.hpp>
#else
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_utility.hpp>
#endif

namespace jis {

namespace x0410 {

namespace spirit {

#if BOOST_VERSION <= 103400
using namespace boost::spirit;
#else
using namespace boost::spirit::classic;
#endif

typedef fixed_loop<digit_parser, int> area1_parser_type;
typedef fixed_loop<range<int>, int> area2_parser_type;
typedef fixed_loop<digit_parser, int> area3_parser_type;

/**
 * @defgroup jis_x0410_spirit_area_parser 地域区画文字列プリミティブ
 *
 * 第n次地域区画を表す文字列にマッチングするプリミティブ。
 */
//@{
const area1_parser_type area1_p = area1_parser_type(digit_p, 4);
const area2_parser_type area2_p = area2_parser_type(range<int>('0', '7'), 2);
const area3_parser_type area3_p = area3_parser_type(digit_p, 2);
//@}

typedef area1_parser_type meshcode1_parser_type;
typedef sequence<meshcode1_parser_type, area2_parser_type> meshcode2_parser_type;
typedef sequence<meshcode2_parser_type, area3_parser_type> meshcode3_parser_type;

/**
 * @defgroup jis_x0410_spirit_mesh_parser メッシュコード文字列プリミティブ
 *
 * n次メッシュコードを表す文字列にマッチングするプリミティブ。
 */
//@{
const meshcode1_parser_type meshcode1_p = area1_p;
const meshcode2_parser_type meshcode2_p = meshcode2_parser_type(meshcode1_p, area2_p);
const meshcode3_parser_type meshcode3_p = meshcode3_parser_type(meshcode2_p, area3_p);
//@}

} // namespace spirit

} // namespace x0410

} // namespace jis
