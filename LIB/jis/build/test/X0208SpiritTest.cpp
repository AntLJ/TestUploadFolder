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

#include "stdafx.h"
#include <string>
#include <boost/assign/std/vector.hpp>
#include <jis/x0208/spirit.h>

using namespace boost::spirit;
using namespace jis::x0208::spirit;

class SJISPrimitiveTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(SJISPrimitiveTest);
	CPPUNIT_TEST(test1B);
	CPPUNIT_TEST(test2B);
	CPPUNIT_TEST(testComposite);
	CPPUNIT_TEST(testEscape);
	CPPUNIT_TEST(testKatakana);
	CPPUNIT_TEST(testMix1);
	CPPUNIT_TEST(testMix2);
	CPPUNIT_TEST(testMix3);
	CPPUNIT_TEST(testUTF16Kana);
	CPPUNIT_TEST_SUITE_END();

public:
//@{ @name テストケース
	/**
	 * @brief 1バイト文字1文字の検査。
	 */
	void test1B()
	{
		std::string strAcutal;
		parse("1", sjis_p[assign(strAcutal)]);
		CPPUNIT_ASSERT_EQUAL(std::string("1"), strAcutal);
	}

	/**
	 * @brief 2バイト文字1文字の検査。
	 */
	void test2B()
	{
		std::string strAcutal;
		parse("あ", sjis_p[assign(strAcutal)]);
		CPPUNIT_ASSERT_EQUAL(std::string("あ"), strAcutal);
	}

	/**
	 * @brief 混合検査。
	 */
	void testComposite()
	{
		std::string strAcutal;
		parse("2バイト文字を含め13文字", (repeat_p(13)[sjis_p])[assign(strAcutal)]);
		CPPUNIT_ASSERT_EQUAL(std::string("2バイト文字を含め13文字"), strAcutal);
	}

	/**
	 * @brief 2バイト目がエスケープ文字となる2バイト文字を含む文字列の検査。
	 */
	void testEscape()
	{
		// anychar_p を使うと「能」の2バイト目で解析が止まる。
		parse_info<> aInfo = parse("機能概要", *(anychar_p - '\\'));
		CPPUNIT_ASSERT(! aInfo.full);
		CPPUNIT_ASSERT_EQUAL(std::string("\\概要"), std::string(aInfo.stop));

		// sjis_p なら問題なく解析可能。
		std::string strAcutal;
		CPPUNIT_ASSERT(parse("機能概要", (*(sjis_p - '\\'))[assign(strAcutal)]).full);
		CPPUNIT_ASSERT_EQUAL(std::string("機能概要"), strAcutal);
	}

	/**
	 * @brief カタカナ文字の検査。
	 */
	void testKatakana()
	{
		std::string strAcutal;
		CPPUNIT_ASSERT(parse("ハチモジとおまけ", (repeat_p(4)[sjis_katakana_p])[assign(strAcutal)] >> repeat_p(4)[sjis_p]).full);
		CPPUNIT_ASSERT_EQUAL(std::string("ハチモジ"), strAcutal);
	}

	/**
	 * @brief 色んな文字を混ぜ合わせて検査その１。
	 */
	void testMix1()
	{
		std::vector<std::string> strExpects[2], strActuals[2];

		using namespace boost::assign;

		strExpects[0] += "プ", "ギ", "ャ", "ー", "蛸", "（", "・", "ｏ", "・", "）", "た", "こ";
		strExpects[1] += "m", "9", "(", "^", "Д", "^", ")", "ﾌ", "ﾟ", "ｷ", "ﾞ", "ｬ", "ｰ";

		parse_info<> aInfo =
			parse(
				"プギャーm9(^Д^)蛸ﾌﾟｷﾞｬｰ（・ｏ・）たこ",
				*(
					(sjis_kanji_p | sjis_kana_p | sjis_alnum_p | sjis_chset("（）ー・"))[push_back_a(strActuals[0])] |
					sjis_p[push_back_a(strActuals[1])]
				)
			);

		CPPUNIT_ASSERT(aInfo.full);
		CPPUNIT_ASSERT(strExpects[0] == strActuals[0]);
		CPPUNIT_ASSERT(strExpects[1] == strActuals[1]);
	}

	/**
	 * @brief 色んな文字を混ぜ合わせて検査その２。
	 */
	void testMix2()
	{
		parse_info<> aInfo(parse("６а", *(sjis_kanji_p | sjis_kana_p | sjis_alnum_p | sjis_chset("（）ー・"))));

		CPPUNIT_ASSERT_EQUAL(size_t(2), aInfo.length);
	}

	/**
	 * @brief 数字とキリル。
	 */
	void testMix3()
	{
		CPPUNIT_ASSERT(parse("６а", sjis_digit_p >> sjis_cyrillic_p).full);
	}

	/**
	 * @brief UTF-16文字列中のJIS X 0208かな／カナ文字判定検査。
	 *
	 * @note
	 * 本気で検査するなら、JIS X 0208に存在存在しないカナ文字（濁点付きのワとか）も含めるべきではある。
	 */
	void testUTF16Kana()
	{
		std::vector<std::wstring> strExpects[2], strActuals[2];

		using namespace boost::assign;

		strExpects[0] += L"ひらがなと", L"を";
		strExpects[1] += L"カタカナ";

		parse_info<const wchar_t*> aInfo = 
			parse(
				L"ひらがなとカタカナを抽出",
				*(
					(+utf16_hiragana_p)[push_back_a(strActuals[0])] |
					(+utf16_katakana_p)[push_back_a(strActuals[1])] |
					+anychar_p
				)
			);

		CPPUNIT_ASSERT(aInfo.full);
		CPPUNIT_ASSERT(strExpects[0] == strActuals[0]);
		CPPUNIT_ASSERT(strExpects[1] == strActuals[1]);
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(SJISPrimitiveTest);
