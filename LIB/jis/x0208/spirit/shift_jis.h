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

#ifndef JIS_X0208_SPIRIT_SHIFT_JIS_H_
#define JIS_X0208_SPIRIT_SHIFT_JIS_H_

#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
#ifndef BOOST_SPIRIT_NO_CHAR_TRAITS
#	define BOOST_SPIRIT_NO_CHAR_TRAITS 1
#endif // BOOST_SPIRIT_NO_CHAR_TRAITS
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)

#include <functional>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_utility.hpp>
#include <boost/spirit/include/classic_if.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits.hpp>
#include <boost/next_prior.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>

namespace jis {

namespace x0208 {

namespace spirit {

using namespace boost::spirit::classic;

#pragma warning( push )
#pragma warning( disable : 4305 4309 )
/**
 * @brief Shift JIS文字の上位1バイトにマッチするパーサ
 */
struct sjis_high : public char_parser<sjis_high>
{
	template <typename T>
	bool test(T ch_) const
	{
		BOOST_STATIC_ASSERT(sizeof(T) == 1);
		const unsigned char ch = *reinterpret_cast<unsigned char*>(&ch_);
		return (ch >= 0x81 && ch <= 0x9f) || (ch >= 0xe0 && ch <= 0xef);
	}
};

/**
 * @brief Shift JIS文字の下位1バイトにマッチするパーサ
 */
struct sjis_low : public char_parser<sjis_low>
{
	template <typename T>
	bool test(T ch_) const
	{
		BOOST_STATIC_ASSERT(sizeof(T) == 1);
		const unsigned char ch = *reinterpret_cast<unsigned char*>(&ch_);
		return ch != 0x7f && (ch >= 0x40 && ch <= 0xfc);
	}
};
#pragma warning( pop )

/// @brief sjis_high のインスタンス
const sjis_high sjis_high_p = sjis_high();

/**
 * @brief Shift JIS文字1文字（1～2バイト）にマッチするパーサ
 */
const impl::if_else_parser<sjis_low, anychar_parser, sjis_high>
	sjis_p = impl::if_else_parser<sjis_low, anychar_parser, sjis_high>(sjis_low(), anychar_p, sjis_high());

/**
 * @brief Shift JIS文字1文字にマッチし、その文字コード（unsigned char）を解析時アクションに渡すパーサ
 */
struct sjis_code : public parser<sjis_code>
{
    typedef sjis_code self_t;
   
	template <typename ScannerT>
	struct result 
	{
		typedef typename match_result<ScannerT, unsigned short>::type type;
	};

	template <typename ScannerT>
	typename parser_result<self_t, ScannerT>::type parse(ScannerT const& scan) const
	{
		typedef typename parser_result<self_t, ScannerT>::type result_t;
		typedef typename ScannerT::value_t value_t;
		typedef typename ScannerT::iterator_t iterator_t;

		BOOST_STATIC_ASSERT(sizeof(value_t) == 1);

		const iterator_t high(scan.first);
		if(result_t m = sjis_p.parse(scan)) {
			unsigned short code = static_cast<unsigned short>(*high) & 0xff;
			const iterator_t low(boost::next(high));
			if(low != scan.first) {
				(code <<= 8) |= static_cast<unsigned const>(*low) & 0xff;
			}
			return scan.create_match(m.length(), code, high, scan.first);
		}
		return scan.no_match();
	}
};

/// @brief sjis_code のインスタンス
const sjis_code sjis_code_p = sjis_code();

/**
 * @brief ch1以上ch2以下の範囲のShift JIS文字1文字にマッチするパーサ
 */
struct sjis_range : public parser<sjis_range>
{
    typedef sjis_range self_t;

	sjis_range(unsigned short ch1, unsigned short ch2) :
	first(ch1),
	last(ch2)
	{
	}
    
	template <typename ScannerT>
	typename parser_result<self_t, ScannerT>::type parse(ScannerT const& scan) const
	{
		const typename ScannerT::iterator_t save(scan.first);
		const typename parser_result<sjis_code, ScannerT>::type m(sjis_code_p.parse(scan));
		return m && test(m.value()) ? scan.create_match(m.length(), nil_t(), save, scan.first) : scan.no_match();
	}

	bool test(unsigned short code) const
	{
		return code >= first && code <= last;
	}

	unsigned short lower() const
	{
		return first;
	}

	unsigned short upper() const
	{
		return last;
	}

private:
	unsigned short first, last;
};

/**
 * @brief sjis_range のジェネレータ。
 */
inline sjis_range sjis_range_p(unsigned short ch1, unsigned short ch2)
{
	return sjis_range(ch1, ch2);
}

/**
 * @brief sjis_range のジェネレータ（１文字）。
 */
inline sjis_range sjis_ch_p(unsigned short ch)
{
	return sjis_range(ch, ch);
}

/**
 * @brief 複数の sjis_range にマッチするパーサ
 */
struct sjis_ranges : public parser<sjis_ranges>
{
    typedef sjis_ranges self_t;

	sjis_ranges& add(const sjis_range& rng)
	{
		ranges.push_back(rng);
		return *this;
	}

	sjis_ranges& add(const sjis_ranges& rs)
	{
		std::copy(rs.ranges.begin(), rs.ranges.end(), std::back_inserter(ranges));
		return *this;
	}

	template <typename ScannerT>
	typename parser_result<self_t, ScannerT>::type parse(ScannerT const& scan) const
	{
		const typename ScannerT::iterator_t save(scan.first);
		if(const typename parser_result<sjis_code, ScannerT>::type m = sjis_code_p.parse(scan)) {
			if(std::find_if(ranges.begin(), ranges.end(), std::bind2nd(std::mem_fun_ref(&sjis_range::test), m.value())) != ranges.end())
				return scan.create_match(m.length(), nil_t(), save, scan.first);
		}
		return scan.no_match();
	}

private:
	std::vector<sjis_range> ranges;
};

/**
 * @brief 渡されたShift JIS文字列に含まれる全ての文字にマッチする sjis_range のジェネレータ。
 *
 * @param[in] str Shift JIS文字列。
 * @return sjis_range のインスタンス。
 */
template <typename CharT>
inline sjis_ranges sjis_chset(const CharT* str)
{
	std::vector<unsigned short> chs;
	parse(str, *(sjis_code_p[push_back_a(chs)]));

	sjis_ranges rs;
	for(std::vector<unsigned short>::const_iterator it(chs.begin()), it_end(chs.end()); it != it_end; ++it) {
		rs.add(sjis_ch_p(*it));
	}
	return rs;
};

namespace detail {

template <typename T>
struct is_sjis_ranges_addable :
	public boost::mpl::or_<
		boost::is_same<T, sjis_range>,
		boost::is_same<T, sjis_ranges>
	>
{
};

} // namespace detail

template <typename T, typename U>
inline typename boost::enable_if<
	boost::mpl::and_<
		detail::is_sjis_ranges_addable<T>,
		detail::is_sjis_ranges_addable<U>
	>,
	sjis_ranges
>::type
operator|(const T& lhs, const U& rhs)
{
	return sjis_ranges().add(lhs).add(rhs);
}

/**
 * @defgroup jis_x_0208_shift_jis_parser JIS X 0208対応Shift JIS文字パーサ
 */
//@{
const sjis_range sjis_digit_p = sjis_range(0x824f, 0x8258); ///< Shift JIS数値文字（０～９）
const sjis_range sjis_upper_p = sjis_range(0x8260, 0x8279); ///< Shift JIS英大文字（Ａ～Ｚ）
const sjis_range sjis_lower_p = sjis_range(0x8281, 0x829a); ///< Shift JIS英小文字（ａ～ｚ）

const sjis_range sjis_hiragana_p = sjis_range(0x829f, 0x82f1); ///< Shift JIS平仮名文字（ぁ～ん）
const sjis_range sjis_katakana_p = sjis_range(0x8340, 0x8396); ///< Shift JIS片仮名文字（ァ～ヶ）
const sjis_range sjis_upper_greek_p = sjis_range(0x839f, 0x83b6); ///< Shift JISギリシャ大文字（Α～Ω）
const sjis_range sjis_lower_greek_p = sjis_range(0x83bf, 0x83d6); ///< Shift JISギリシャ大文字（α～ω）
const sjis_range sjis_upper_cyrillic_p = sjis_range(0x8440, 0x8460); ///< Shift JISキリル大文字（А～Я）
const sjis_range sjis_lower_cyrillic_p = sjis_range(0x8470, 0x8491); ///< Shift JISキリル大文字（а～я）

const sjis_range sjis_kanji1_p = sjis_range(0x889f, 0x9872); ///< Shift JIS第一水準漢字文字（亜～腕）
const sjis_ranges sjis_kanji2_p = sjis_ranges().add(sjis_range(0x989f, 0x9ffc)).add(sjis_range(0xe040, 0xeaa4)); ///< Shift JIS第二水準漢字文字（弌～滌、漾～熙）

const sjis_ranges sjis_alpha_p = sjis_ranges().add(sjis_upper_p).add(sjis_lower_p); ///< Shift JIS英文字
const sjis_ranges sjis_alnum_p = sjis_ranges().add(sjis_alpha_p).add(sjis_digit_p); ///< Shift JIS英数字
const sjis_ranges sjis_greek_p = sjis_ranges().add(sjis_upper_greek_p).add(sjis_lower_greek_p); ///< Shift JISギリシャ文字
const sjis_ranges sjis_cyrillic_p = sjis_ranges().add(sjis_upper_cyrillic_p).add(sjis_lower_cyrillic_p); ///< Shift JISキリル文字
const sjis_ranges sjis_kana_p = sjis_ranges().add(sjis_hiragana_p).add(sjis_katakana_p); ///< Shift JIS平仮名片仮名
const sjis_ranges sjis_kanji_p = sjis_ranges().add(sjis_kanji1_p).add(sjis_kanji2_p); ///< Shift JIS第一、第二水準漢字
//@}

} // namespace spirit

} // namespace x0208

} // namespace jis

#endif // JIS_X0208_SPIRIT_SHIFT_JIS_H_
