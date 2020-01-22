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

#ifndef RNS_STRING_CONCATENATE_ITERATOR_H_
#define RNS_STRING_CONCATENATE_ITERATOR_H_

#include <boost/shared_ptr.hpp>

namespace rns {

/**
 * @brief 文字列連結反復子
 *
 * std::ostream_iterator 似のクラス。<br>
 * 相違点は
 * - あらゆる型の値を受け取れる
 * - 文字列の実体は反復子自身が持ち、反復子の複製時にシャローコピーされる
 * - 区切り文字の付け方
 *   - std::ostream_iterator は出力ごとに後ろに区切り文字を付け加える（改行毎の出力に最適）
 *   - このクラスは最初の出力時は区切り文字をつけず、それ以降は出力の前に区切り文字を付け加える（カンマ区切りの出力に最適）
 *
 * @test StringConcatenateIteratorTest
 */
template <typename Char>
class basic_string_concatenate_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	typedef basic_string_concatenate_iterator self;
	typedef std::basic_ostringstream<Char> stream_type;
	typedef std::basic_string<Char> string_type;
public:
	class output_proxy
	{
		friend basic_string_concatenate_iterator;
	public:
		output_proxy(const Char* delim) :
		m_delim(delim),
		m_first(true)
		{
		}

		template <typename Value>
		output_proxy& operator=(const Value& rhs)
		{
			if(m_delim) {
				if(m_first) {
					m_first = false;
					m_oss << rhs;
				}
				else {
					m_oss << m_delim << rhs;
				}
			}
			else {
				m_oss << rhs;
			}

			return *this;
		}

	private:
		std::basic_ostringstream<Char> m_oss;
		const Char* m_delim;
		bool m_first;
	};

	/**
	 * @param[in] delim 区切り文字列。コピーはしないので寿命に注意。区切らない場合はnull。
	 */
	explicit basic_string_concatenate_iterator(const Char* delim = 0) :
	m_proxy(new output_proxy(delim))
	{
	}

/// @name 反復子的関数
//@{
	output_proxy& operator*() { return *m_proxy; }
	self& operator++() { return *this; }
	self& operator++(int) { return *this; }
//@}

	/**
	 * @brief 連結された文字列を返す。
	 */
	string_type str() const { return m_proxy->m_oss.str(); }

private:
	boost::shared_ptr<output_proxy> m_proxy;
};

typedef basic_string_concatenate_iterator<char> string_concatenate_iterator;
typedef basic_string_concatenate_iterator<wchar_t> wstring_concatenate_iterator;

#ifdef _TCHAR_DEFINED
typedef basic_string_concatenate_iterator<TCHAR> tstring_concatenate_iterator;
#endif // TCHAR_DEFINED

} // namespace rns

#endif // RNS_STRING_CONCATENATE_ITERATOR_H_
