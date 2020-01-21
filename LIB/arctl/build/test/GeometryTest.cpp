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
 * @file GeometryTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <atl2/seal.h>
#include <arctl/geometry.h>

namespace {

/**
 * @brief �����}�`�B
 */
WKSPoint s_Points[][3] = {
	{ {0,0}, {10,0}, {20,10} },
	{ {20,10}, {30,10}, {30,0} },
	{ {30,0}, {20,0}, {20,-10} },
	{ {20,-10}, {10,-10}, {0,0} },
};

} // anonymous namespace

/**
 * @ingroup unit_test
 * @brief arctl::align_directed_pointcollection �� @ref unit_test �N���X
 */
class AlignDirectedPointCollectionTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(AlignDirectedPointCollectionTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testBreak);
	CPPUNIT_TEST_SUITE_END();

public:
	/**
	 * @brief �����}�`���\������4��Polyline�𐶐�����B
	 */
	virtual void setUp()
	{
		for(int i = 0; i < sizeof(s_Points) / sizeof(s_Points[0]); i++) {
			IPointCollectionPtr ipPointCol(CLSID_Polyline);
			ipPointCol->AddWKSPoints(sizeof(s_Points[i]) / sizeof(s_Points[i][0]), s_Points[i]);
			m_PolylineList.push_back(ipPointCol);
		}
	}

	/**
	 * @brief 4��Polyline��j������B
	 */
	virtual void tearDown()
	{
		m_PolylineList.clear();
	}

private:
/// @name �e�X�g�P�[�X
//@{
	/**
	 * @brief �����}�`�ƊJ�����}�`���e�X�g�B
	 */
	void test()
	{
		for(int i = m_PolylineList.size(); i > 0; i--) {
			std::vector<IPointCollection*> aList(i);
			std::copy(&m_PolylineList[0], &m_PolylineList[0] + i, aList.begin());

			// �������`���p�ӂ���B
			IPointCollectionPtr ipExpectedPointCol(CLSID_Polyline);
			std::for_each(
				aList.begin(),
				aList.end(),
				std::bind1st(std::mem_fun(&IPointCollection::_AddPointCollection), ipExpectedPointCol)
			);

			// next_permutation���[�v�̂��߃\�[�g
			std::sort(aList.begin(), aList.end());

			// �S�Ẵp�^�[�������؂���B
			do {
				IGeometryPtr ipActualGeom(arctl::align_directed_pointcollection(aList.begin(), aList.end()));
				CPPUNIT_ASSERT(ipActualGeom != 0);
				CPPUNIT_ASSERT_EQUAL(VARIANT_TRUE, IRelationalOperatorPtr(ipExpectedPointCol)->_Equals(ipActualGeom));
			} while(std::next_permutation(aList.begin(), aList.end()));
		}
	}

	/**
	 * @brief �q����Ȃ��ꍇ�͎��s�B
	 */
	void testBreak()
	{
		IPointCollection* aList[2] = { m_PolylineList[0], m_PolylineList[2] };
		CPPUNIT_ASSERT(arctl::align_directed_pointcollection(aList, aList + 2) == 0);
	}
//@}

private:
	std::vector<atl2::seal<IPointCollectionPtr> > m_PolylineList;
};

CPPUNIT_TEST_SUITE_REGISTRATION(AlignDirectedPointCollectionTest);
