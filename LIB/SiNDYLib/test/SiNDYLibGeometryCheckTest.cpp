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
 * @file SiNDYLibGeometryCheckTest.cpp
 * @brief SiNDYLib�� CGeometryCheck.cpp �̂��e�X�g����N���X�̒�`�t�@�C���ł��B
 * @author 
 * @version $Id$
 */
#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "Geometry.h"
//#include "Workspace.h"
//#include "Table.h"
#include "ErrorCode.h"
#include <list>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
/**
 * @class CSiNDYLibGeometryCheckTest
 * @brief SiNDYLib�� CGeometry�̃W�I���g���`�F�b�N���e�X�g����N���X
 * <h3>�`�F�b�N���e�F<h3>
 * @li 臒l�̓��̓`�F�b�N
 * @li ��O�l�iNULL�A�͈͊O�j
 */
using namespace sindy;
using namespace sindy;
using namespace sindy::errorcode;

class CSiNDYLibGeometryCheckTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CSiNDYLibGeometryCheckTest);
  CPPUNIT_TEST(testGeometryCheck);
  CPPUNIT_TEST_SUITE_END();
public:
	CSiNDYLibGeometryCheckTest(){}
	virtual ~CSiNDYLibGeometryCheckTest(){}

	void setUp ()
	{
	}


	void tearDown()
	{
	}


	/**
	 * @brief Set/Get Test
	 */
	void testGeometryCheck()
	{
		// ��`��`�F�b�N
		// �ҏW�\�G���A�`�F�b�N
		// ���b�N�G���A�`�F�b�N
		// �p�[�g���`�F�b�N�i�|�C���g�A�|�����C���j
		// �T�|�[�g�O�`��`�F�b�N
		// �G�N�X�e���A���`�F�b�N�i�|���S���j
		// �˂���`�F�b�N�i�|���S���j
		// ���[�v�`�F�b�N

		// ��Ȍ`��`�F�b�N
		// �p���X�E�s�p�^�[���`�F�b�N
		// �Z�O�����g���`�F�b�N
		// ���ȐڐG�`�F�b�N
		// �Œ�Z�O�����g���`�F�b�N
	}

	void testSegmentCheck()
	{
	}

	void testPointCheck()
	{
	}
	
private:
};

CPPUNIT_TEST_SUITE_REGISTRATION(CSiNDYLibGeometryCheckTest);
