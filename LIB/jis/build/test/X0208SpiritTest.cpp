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
//@{ @name �e�X�g�P�[�X
	/**
	 * @brief 1�o�C�g����1�����̌����B
	 */
	void test1B()
	{
		std::string strAcutal;
		parse("1", sjis_p[assign(strAcutal)]);
		CPPUNIT_ASSERT_EQUAL(std::string("1"), strAcutal);
	}

	/**
	 * @brief 2�o�C�g����1�����̌����B
	 */
	void test2B()
	{
		std::string strAcutal;
		parse("��", sjis_p[assign(strAcutal)]);
		CPPUNIT_ASSERT_EQUAL(std::string("��"), strAcutal);
	}

	/**
	 * @brief ���������B
	 */
	void testComposite()
	{
		std::string strAcutal;
		parse("2�o�C�g�������܂�13����", (repeat_p(13)[sjis_p])[assign(strAcutal)]);
		CPPUNIT_ASSERT_EQUAL(std::string("2�o�C�g�������܂�13����"), strAcutal);
	}

	/**
	 * @brief 2�o�C�g�ڂ��G�X�P�[�v�����ƂȂ�2�o�C�g�������܂ޕ�����̌����B
	 */
	void testEscape()
	{
		// anychar_p ���g���Ɓu�\�v��2�o�C�g�ڂŉ�͂��~�܂�B
		parse_info<> aInfo = parse("�@�\�T�v", *(anychar_p - '\\'));
		CPPUNIT_ASSERT(! aInfo.full);
		CPPUNIT_ASSERT_EQUAL(std::string("\\�T�v"), std::string(aInfo.stop));

		// sjis_p �Ȃ���Ȃ���͉\�B
		std::string strAcutal;
		CPPUNIT_ASSERT(parse("�@�\�T�v", (*(sjis_p - '\\'))[assign(strAcutal)]).full);
		CPPUNIT_ASSERT_EQUAL(std::string("�@�\�T�v"), strAcutal);
	}

	/**
	 * @brief �J�^�J�i�����̌����B
	 */
	void testKatakana()
	{
		std::string strAcutal;
		CPPUNIT_ASSERT(parse("�n�`���W�Ƃ��܂�", (repeat_p(4)[sjis_katakana_p])[assign(strAcutal)] >> repeat_p(4)[sjis_p]).full);
		CPPUNIT_ASSERT_EQUAL(std::string("�n�`���W"), strAcutal);
	}

	/**
	 * @brief �F��ȕ������������킹�Č������̂P�B
	 */
	void testMix1()
	{
		std::vector<std::string> strExpects[2], strActuals[2];

		using namespace boost::assign;

		strExpects[0] += "�v", "�M", "��", "�[", "��", "�i", "�E", "��", "�E", "�j", "��", "��";
		strExpects[1] += "m", "9", "(", "^", "�D", "^", ")", "�", "�", "�", "�", "�", "�";

		parse_info<> aInfo =
			parse(
				"�v�M���[m9(^�D^)���߷ެ��i�E���E�j����",
				*(
					(sjis_kanji_p | sjis_kana_p | sjis_alnum_p | sjis_chset("�i�j�[�E"))[push_back_a(strActuals[0])] |
					sjis_p[push_back_a(strActuals[1])]
				)
			);

		CPPUNIT_ASSERT(aInfo.full);
		CPPUNIT_ASSERT(strExpects[0] == strActuals[0]);
		CPPUNIT_ASSERT(strExpects[1] == strActuals[1]);
	}

	/**
	 * @brief �F��ȕ������������킹�Č������̂Q�B
	 */
	void testMix2()
	{
		parse_info<> aInfo(parse("�U�p", *(sjis_kanji_p | sjis_kana_p | sjis_alnum_p | sjis_chset("�i�j�[�E"))));

		CPPUNIT_ASSERT_EQUAL(size_t(2), aInfo.length);
	}

	/**
	 * @brief �����ƃL�����B
	 */
	void testMix3()
	{
		CPPUNIT_ASSERT(parse("�U�p", sjis_digit_p >> sjis_cyrillic_p).full);
	}

	/**
	 * @brief UTF-16�����񒆂�JIS X 0208���ȁ^�J�i�������茟���B
	 *
	 * @note
	 * �{�C�Ō�������Ȃ�AJIS X 0208�ɑ��ݑ��݂��Ȃ��J�i�����i���_�t���̃��Ƃ��j���܂߂�ׂ��ł͂���B
	 */
	void testUTF16Kana()
	{
		std::vector<std::wstring> strExpects[2], strActuals[2];

		using namespace boost::assign;

		strExpects[0] += L"�Ђ炪�Ȃ�", L"��";
		strExpects[1] += L"�J�^�J�i";

		parse_info<const wchar_t*> aInfo = 
			parse(
				L"�Ђ炪�ȂƃJ�^�J�i�𒊏o",
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
