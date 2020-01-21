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
 * @brief MPQVer1 �� MPQView �̑g�ݍ��킹���e�X�g����N���X
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
// �\�z/����
	MPQViewTest1() :
	m_pMPQIO(0),
	m_pMPQ(0)
	{
	}

	virtual void setUp()
	{
		m_pMPQIO = new MPQVer1;
		m_pMPQ = new MPQView(m_pMPQIO);

		// �T���v���f�[�^���J���B
		CPPUNIT_ASSERT(m_pMPQ->read("sample\\ver1", 533916));
	}

	virtual void tearDown()
	{
		delete m_pMPQ;
		delete m_pMPQIO;
	}

// �e�X�g�P�[�X
	/**
	 * @brief �قȂ�o�[�W�����̃t�@�C���͊J���Ȃ�
	 */
	void testVer2()
	{
		// Ver2�f�[�^���J���B
		CPPUNIT_ASSERT(! m_pMPQ->read("sample\\ver2", 533916));
	}

	/**
	 * @brief �t�@�C���Ǘ����C���̐��퐫�m�F
	 */
	void testFileManageLayer()
	{
		// �t�H�[�}�b�g�o�[�W������"1"�B
		CPPUNIT_ASSERT_EQUAL(1, m_pMPQ->getVer());

		// ���b�V���R�[�h��"533916"�B
		CPPUNIT_ASSERT_EQUAL(533916, m_pMPQ->getMesh());

		// �t�@�C���쐬����"2004�N4��8��"�B
		CPPUNIT_ASSERT_EQUAL(20040408, m_pMPQ->getCreate());

		// �ŏI�ҏW����"2004�N4��8��"�B
		CPPUNIT_ASSERT_EQUAL(20040408, m_pMPQ->getEdit());

		// ��IN�쐬����"2004�N3��27��"�B
		CPPUNIT_ASSERT_EQUAL(20040327, m_pMPQ->getINVer());
	}

	/**
	 * @brief �|�����C���f�[�^���̐��퐫�m�F
	 */
	void testLinkCount()
	{
		// �|�����C���̐���"131"�B
		CPPUNIT_ASSERT_EQUAL(131, std::distance(m_pMPQ->begin(), m_pMPQ->end()));
	}

	/**
	 * @brief �擪�Ɉʒu����|�����C���f�[�^�̐��퐫�m�F
	 */
	void testFirstPolylineData()
	{
		// �擪�̃|�����C�������o���B
		MPQ::Link* pLink = valid(m_pMPQ->begin());

		// �_�̐���"2"�B
		CPPUNIT_ASSERT_EQUAL(2U, pLink->eShape.size());

		// �\����ʂ�"0x05002100"�B
		CPPUNIT_ASSERT_EQUAL(0x05002100, pLink->eClass);

		// �����N�񑮐���"1"�i����ʍs�j�B
		CPPUNIT_ASSERT_EQUAL(MPQ::Link::Ord, pLink->eWay);

		// �_1�̉񓚂�p�ӁB
		MPQ::Point aPoint1(0x142C, 0x0D5B80);
		aPoint1.eID = 0x2FD2E5;
		aPoint1.eIN = true;

		// �_1�����؁B
		checkEqual(aPoint1, pLink->eShape[0]);

		// �_2�̉񓚂�p�ӁB
		MPQ::Point aPoint2(0x0, 0x0D5526);
		aPoint2.eID = 0x2FB9F6;
		aPoint2.eIN = true;

		// �_2�����؁B
		checkEqual(aPoint2, pLink->eShape[1]);
	}

	/**
	 * @brief IN�ɑ��݂��Ȃ���ԓ_�ɑ΂��鐳�퐫�m�F
	 */
	void testOriginalVertex()
	{
		std::set<const MPQ::Point*> aOriginalVertexSet;

		for(MPQ::Iter it = m_pMPQ->begin(); it != m_pMPQ->end(); ++it) {
			MPQ::Link* pLink = valid(it);
			for(unsigned int i = 0; i < pLink->eShape.size(); i++) {
				const MPQ::Point& rPoint = pLink->eShape[i];
				if(rPoint.eIN == false) {
					// IN�ɑ��݂��Ȃ���ԓ_�́A�K�����������B
					CPPUNIT_ASSERT(rPoint.eAttr);

					// ���𐔂���B
					aOriginalVertexSet.insert(&rPoint);
				}
			}
		}

		// IN�ɑ��݂��Ȃ���ԓ_�̐���"28"�B
		CPPUNIT_ASSERT_EQUAL(28U, aOriginalVertexSet.size());
	}

private:
// ����
	/**
	 * @brief �����q�̗L�����m�F
	 *
	 * @param it [in] �L�������m�F���锽���q�B
	 * @return �����q���w�������Ă���|�����C���f�[�^�B
	 */
	MPQ::Link* valid(const MPQ::Iter& it)
	{
		CPPUNIT_ASSERT(it != m_pMPQ->end());
		
		MPQ::Link* pLink = *it;
		CPPUNIT_ASSERT(pLink);

		return pLink;
	}

	/**
	 * @brief �_�̓��ꐫ�m�F
	 *
	 * @param rExpected [in] �������l�����_�B
	 * @param rActual [in] �l�̌��؂��s���_�B
	 */
	void checkEqual(const MPQ::Point& rExpected, const MPQ::Point& rActual)
	{
		CPPUNIT_ASSERT_EQUAL(rExpected.eX, rActual.eX);
		CPPUNIT_ASSERT_EQUAL(rExpected.eY, rActual.eY);
		CPPUNIT_ASSERT_EQUAL(rExpected.eID, rActual.eID);
		CPPUNIT_ASSERT_EQUAL(rExpected.eIN, rActual.eIN);
	}

// �ϐ�
	MPQIO* m_pMPQIO;
	MPQ* m_pMPQ;
};

CPPUNIT_TEST_SUITE_REGISTRATION(MPQViewTest1);
