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
 * @file
 * @brief テーブル名称文字列からユーザ名称前置詞のみ取り出したり、逆にユーザ名称前置詞のみ取り除いたり。
 *
 * - @ref algorithm_skip_owner_prefix
 * - @ref algorithm_copy_owner_prefix
 */
#ifndef ARCTL_TABLE_NAME_ALGORITHM_H_
#define ARCTL_TABLE_NAME_ALGORITHM_H_

#include <algorithm>
#include <boost/range.hpp>
#include <boost/concept_check.hpp>
#include <boost/tokenizer.hpp>
#include <../../TDC/useful_headers/text_util.h>

namespace arctl {

namespace algorithm {

namespace detail {

} // namespace detail

/**
 * @defgroup algorithm_skip_owner_prefix skip_owner_prefix
 */
//@{

/**
 * @brief ユーザ名称前置詞（<em>user</em>.）を除いたテーブル名称文字列を求める。
 *
 * @param[in] src ユーザー名称前置詞を含むかもしれないテーブル名称文字列。
 * @return ユーザ名称前置詞を除いたテーブル名称の文字列。
 */
inline LPCTSTR skip_owner_prefix(const _bstr_t& src)
{
	std::wstring strret;
	std::wstring str(static_cast<LPCWSTR>(src));
	unsigned int pos = str.find( L".", 0 );
	if( pos != std::string::npos )
	{
		typedef boost::tokenizer<boost::char_separator<wchar_t>, std::wstring::const_iterator, std::wstring> token;
		boost::char_separator<wchar_t> sep(L".");
		token tok(str, sep);
		// デクリメントができないからループして最後のが返るようにする
		for ( const auto& it : tok )
			strret = it;
	}
	else
		strret = str;
	static uh::tstring tStrret;
	tStrret = uh::toTStr(strret);
	return tStrret.c_str();
}

//@}

/**
 * @defgroup algorithm_copy_owner_prefix copy_owner_prefix
 */
//@{

/**
 * @brief ユーザ名称前置詞（<em>user</em>.）を抜き出す。
 *
 * @param[in] src ユーザー名称前置詞を含むかもしれないテーブル名称文字列。
 * @return ユーザ名称前置詞文字列。引数がユーザ名称文字列を含まない場合は「empty()」。
 *
 * @test
 * -# ユーザ名称前置詞を含む LPCTSTR が渡された時に正しく動作する
 *   - <i>[ CopyOwnerPrefixTest::testConstCharArrayHasPrefix() ]</i>
 * -# ユーザ名称前置詞を含まない LPCTSTR が渡された時に正しく動作する
 *   - <i>[ CopyOwnerPrefixTest::testConstCharArrayHasntPrefix() ]</i>
 */
template <typename Char>
inline std::basic_string<Char> copy_owner_prefix(const Char* src)
{
	std::basic_string<Char> srcStr(src, src+strlen(uh::toStr(src).c_str()));
	return	copy_owner_prefix(srcStr);
}

/**
 * @brief ユーザ名称前置詞（<em>user</em>.）を抜き出す。
 *
 * @param[in] src ユーザー名称前置詞を含むかもしれないテーブル名称文字列。
 * @return ユーザ名称前置詞文字列。引数がユーザ名称文字列を含まない場合は「empty()」。
 */
template <typename Char>
inline std::basic_string<Char> copy_owner_prefix(const std::basic_string<Char>& src)
{
	LPCTSTR skipOwnerStr = skip_owner_prefix(src.c_str());
	if(::_tcsicmp(src.c_str(),skipOwnerStr)==0)
		return std::basic_string<Char>();
	return std::basic_string<Char>(src.data(), src.find(skipOwnerStr));
}

/**
 * @brief ユーザ名称前置詞（<em>user</em>.）を抜き出す。
 *
 * @param[in] src ユーザー名称前置詞を含むかもしれないテーブル名称文字列（_bstr_t）。
 * @return ユーザ名称前置詞文字列（_bstr_t）。引数がユーザ名称文字列を含まない場合は「length()==0」。
 *
 * @test
 * -# ユーザ名称前置詞を含む文字列が渡された時に正しく動作する
 *   - <i>[ CopyOwnerPrefixTest::testBStringHasPrefix() ]</i>
 * -# ユーザ名称前置詞を含まない文字列が渡された時に正しく動作する
 *   - <i>[ CopyOwnerPrefixTest::testBStringHasntPrefix() ]</i>
 */
inline _bstr_t copy_owner_prefix(const _bstr_t& src)
{
	std::wstring strret;
	std::wstring str(static_cast<LPCWSTR>(src));
	unsigned int pos = str.find( L".", 0 );
    if( pos != std::string::npos )
	{
		strret = str.substr(0, pos+1);
	}

	return strret.c_str();
}

//@}

} // namespace algorithm

using algorithm::skip_owner_prefix;
using algorithm::copy_owner_prefix;

} // namespace arctl

#endif // ARCTL_TABLE_NAME_ALGORITHM_H_
