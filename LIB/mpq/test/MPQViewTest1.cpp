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
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <set>
#include <MPQVer1.h>
#include <MPQView.h>

/**
 * @brief MPQVer1 と MPQView の組み合わせをテストするクラス
 */
class MPQViewTest1 : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(MPQViewTest1);
	CPPUNIT_TEST(testVer2);
	CPPUNIT_TEST(testFileManageLayer);
	CPPUNIT_TEST(testLinkCount);
	CPPUNIT_TEST(testFirstPolylineData);
	CPPUNIT_TEST(testOriginalVertex);
	CPPUNIT_TEST_SUITE_END();

public:
// 構築/消滅
	MPQViewTest1() :
	m_pMPQIO(0),
	m_pMPQ(0)
	{
	}

	virtual void setUp()
	{
		m_pMPQIO = new MPQVer1;
		m_pMPQ = new MPQView(m_pMPQIO);

		// サンプルデータを開く。
		CPPUNIT_ASSERT(m_pMPQ->read("sample\\ver1", 533916));
	}

	virtual void tearDown()
	{
		delete m_pMPQ;
		delete m_pMPQIO;
	}

// テストケース
	/**
	 * @brief 異なるバージョンのファイルは開けない
	 */
	void testVer2()
	{
		// Ver2データを開く。
		CPPUNIT_ASSERT(! m_pMPQ->read("sample\\ver2", 533916));
	}

	/**
	 * @brief ファイル管理レイヤの正常性確認
	 */
	void testFileManageLayer()
	{
		// フォーマットバージョンは"1"。
		CPPUNIT_ASSERT_EQUAL(1, m_pMPQ->getVer());

		// メッシュコードは"533916"。
		CPPUNIT_ASSERT_EQUAL(533916, m_pMPQ->getMesh());

		// ファイル作成日は"2004年4月8日"。
		CPPUNIT_ASSERT_EQUAL(20040408, m_pMPQ->getCreate());

		// 最終編集日も"2004年4月8日"。
		CPPUNIT_ASSERT_EQUAL(20040408, m_pMPQ->getEdit());

		// 元IN作成日は"2004年3月27日"。
		CPPUNIT_ASSERT_EQUAL(20040327, m_pMPQ->getINVer());
	}

	/**
	 * @brief ポリラインデータ数の正常性確認
	 */
	void testLinkCount()
	{
		// ポリラインの数は"131"。
		CPPUNIT_ASSERT_EQUAL(131, std::distance(m_pMPQ->begin(), m_pMPQ->end()));
	}

	/**
	 * @brief 先頭に位置するポリラインデータの正常性確認
	 */
	void testFirstPolylineData()
	{
		// 先頭のポリラインを取り出す。
		MPQ::Link* pLink = valid(m_pMPQ->begin());

		// 点の数は"2"。
		CPPUNIT_ASSERT_EQUAL(2U, pLink->eShape.size());

		// 表示種別は"0x05002100"。
		CPPUNIT_ASSERT_EQUAL(0x05002100, pLink->eClass);

		// リンク列属性は"1"（一方通行）。
		CPPUNIT_ASSERT_EQUAL(MPQ::Link::Ord, pLink->eWay);

		// 点1の回答を用意。
		MPQ::Point aPoint1(0x142C, 0x0D5B80);
		aPoint1.eID = 0x2FD2E5;
		aPoint1.eIN = true;

		// 点1を検証。
		checkEqual(aPoint1, pLink->eShape[0]);

		// 点2の回答を用意。
		MPQ::Point aPoint2(0x0, 0x0D5526);
		aPoint2.eID = 0x2FB9F6;
		aPoint2.eIN = true;

		// 点2を検証。
		checkEqual(aPoint2, pLink->eShape[1]);
	}

	/**
	 * @brief INに存在しない補間点に対する正常性確認
	 */
	void testOriginalVertex()
	{
		std::set<const MPQ::Point*> aOriginalVertexSet;

		for(MPQ::Iter it = m_pMPQ->begin(); it != m_pMPQ->end(); ++it) {
			MPQ::Link* pLink = valid(it);
			for(unsigned int i = 0; i < pLink->eShape.size(); i++) {
				const MPQ::Point& rPoint = pLink->eShape[i];
				if(rPoint.eIN == false) {
					// INに存在しない補間点は、必ず属性を持つ。
					CPPUNIT_ASSERT(rPoint.eAttr);

					// 数を数える。
					aOriginalVertexSet.insert(&rPoint);
				}
			}
		}

		// INに存在しない補間点の数は"28"。
		CPPUNIT_ASSERT_EQUAL(28U, aOriginalVertexSet.size());
	}

private:
// 処理
	/**
	 * @brief 反復子の有効性確認
	 *
	 * @param it [in] 有効性を確認する反復子。
	 * @return 反復子が指し示しているポリラインデータ。
	 */
	MPQ::Link* valid(const MPQ::Iter& it)
	{
		CPPUNIT_ASSERT(it != m_pMPQ->end());
		
		MPQ::Link* pLink = *it;
		CPPUNIT_ASSERT(pLink);

		return pLink;
	}

	/**
	 * @brief 点の同一性確認
	 *
	 * @param rExpected [in] 正しい値を持つ点。
	 * @param rActual [in] 値の検証を行う点。
	 */
	void checkEqual(const MPQ::Point& rExpected, const MPQ::Point& rActual)
	{
		CPPUNIT_ASSERT_EQUAL(rExpected.eX, rActual.eX);
		CPPUNIT_ASSERT_EQUAL(rExpected.eY, rActual.eY);
		CPPUNIT_ASSERT_EQUAL(rExpected.eID, rActual.eID);
		CPPUNIT_ASSERT_EQUAL(rExpected.eIN, rActual.eIN);
	}

// 変数
	MPQIO* m_pMPQIO;
	MPQ* m_pMPQ;
};

CPPUNIT_TEST_SUITE_REGISTRATION(MPQViewTest1);
