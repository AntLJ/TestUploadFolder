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

#ifndef RNS_URL_STRING_ALGORITHM_H_
#define RNS_URL_STRING_ALGORITHM_H_

#include <ctype.h>
#include <iterator>
#include <string>

namespace rns {
namespace algorithm {

/**
 * @brief URL文字列をデコードする。
 *
 * 出力される文字数は std::distance(first, last) と同じか、それよりも少ない。<br>
 * 不正なパーセントエンコーディングは "?" に変換する。
 *
 * @tparam InputIterator URL文字列反復子
 * @tparam OutputIterator デコード済み文字列出力反復子
 * @param[in] first URL文字列先頭反復子
 * @param[in] last URL文字列終端反復子
 * @param[in] result デコード済み文字列出力反復子
 * @return デコード済み文字列出力反復子
 */
template <typename InputIterator, typename OutputIterator>
inline OutputIterator
decode_url_copy(InputIterator first, InputIterator last, OutputIterator result)
{
	typedef typename std::iterator_traits<InputIterator>::value_type char_type;

	for(; first != last; ++first)
	{
		char_type ch = *first;

		switch(ch)
		{
		case '%':
			ch = 0;
			for(int i = 0; i < 2; i++)
			{
				ch *= 16;

				if(++first == last)
				{
					*result = '?';
					return ++result;
				}

				const int x = ::tolower(std::char_traits<char_type>::to_int_type(*first));

				if(x >= '0' && x <= '9')
				{
					ch += x - '0';
					continue;
				}
				else if(x >= 'a' && x <= 'f')
				{
					ch += x - 'a' + 10;
					continue;
				}
				else
				{
					ch = '?';
					break;
				}
			}
			break;
		case '+':
			ch = ' ';
			break;
		}

		*result = ch;
		++result;
	}

	return result;
}

/**
 * @brief URL文字列をデコードする。
 *
 * この関数は引数を直接書き換える。<br>
 * デコード後の文字列長は、デコード前と同じか、それよりも短くなる。
 *
 * @tparam Sequence std::string や std::vector<char>
 * @param[in,out] input デコードする文字列
 *
 * @sa rns::algorithm::decode_url_copy()
 */
template <typename Sequence>
inline void decode_url(Sequence& input)
{
	input.erase(
		decode_url_copy(input.begin(), input.end(), input.begin()),
		input.end()
	);
}

} // namespace rns
} // namespace algorithm

#endif // RNS_URL_STRING_ALGORITHM_H_
