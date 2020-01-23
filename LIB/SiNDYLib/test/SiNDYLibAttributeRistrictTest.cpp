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
 * @file SiNDYLibAttributeRistrict.cpp
 * @brief AttributeRistRict.[h|cpp]���e�X�g����
 * �N���X�̒�`�t�@�C���ł��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "AttributeRistrict.h"
#include <list>
#include "Workspace.h"
#include "Row.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

using namespace sindy;

/**
 * @class CAttributeRistrictTest
 * @brief SiNDYLib�̃O���[�o���֐��ł���sindy::AheCheckOrFixAnnotationString���e�X�g����
 * �N���X
 * <h3>�`�F�b�N���e�F<h3>
 * @li ���L������ƕ����񒷂̐������e�X�g
 * @li �S�p������g�p�\�����̃e�X�g
 * @li �W�����N�V�����A�C���^�[�`�F���W�A�t�F���[�^�[�~�i���̏ꍇ�̍ŏI������e�X�g
 * @li ��v�n�����ԍ��A�����ԍ��̏ꍇ�̃e�X�g
 * @li �R�x�W���̏ꍇ�̃e�X�g
 */
class CAttributeRistrictTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CAttributeRistrictTest);
  CPPUNIT_TEST(test_rule);
  CPPUNIT_TEST(test_rule_minmax_error);
  CPPUNIT_TEST(test_rule_minmax_non_error);
  CPPUNIT_TEST(test_rule_minmax_warning);
  CPPUNIT_TEST(test_rule_values_error);
  CPPUNIT_TEST(test_rule_values_warning);
  CPPUNIT_TEST(test_rule_and);
  CPPUNIT_TEST(test_rule_and_mix);
  CPPUNIT_TEST_SUITE_END();
public:
	CAttributeRistrictTest(){}
	virtual ~CAttributeRistrictTest(){}

	void setUp()
	{
		m_cWork.ConnectToPGDB( _T("Test.mdb") );

		ITablePtr ipTable( m_cWork.OpenTable( _T("ROAD_LINK") ) );
		ipTable->GetRow( 1, m_cRow.Release() );

		m_cRow.Trace();

		m_cRow.CreateCache( true );
	}

	void tearDown()
	{
		m_cWork = NULL;
	}
	/**
	 * @brief �V���v���ȃ��[���̃e�X�g
	 * @li �P�ꐧ���ŃG���[�ɂȂ邩�ǂ���
	 * @li �P�ꐧ���ŃG���[�ɂȂ�Ȃ����ǂ���
	 * @li �P�ꐧ���Ōx���ɂȂ邩�ǂ���
	 */
	void test_rule()
	{
		m_cRow.SetBoolValueByFieldName(_T("FIELDSURVEY_F"), true );

		// [test] �P�ꐧ���ŃG���[�ɂȂ邩�ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "simple error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �P�ꐧ���ŃG���[�ɂȂ�Ȃ����ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=0") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "simple non-error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �P�ꐧ���Ōx�����o�邩�ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "simple warning test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
 	 * @brief �ő�l�E�ŏ��l�ł̐������[���̃G���[�e�X�g
	 * @li �ő�ŏ����l�͔F�߂��Ȃ����ǂ����̃e�X�g
	 * @li �P���ȃ��[���ŃG���[�ɂȂ邩�ǂ���
	 * @li �ŏ��̋��E�l�ŃG���[�ɂȂ邩�ǂ����̃e�X�g
	 * @li �ő�̋��E�l�ŃG���[�ɂȂ邩�ǂ����̃e�X�g
	 * @li �ŏ������l�ŃG���[�ɂȂ邩�ǂ����̃e�X�g
	 * @li �ő傪���l�ŃG���[�ɂȂ邩�ǂ����̃e�X�g
	 */
	void test_rule_minmax_error()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );

		// [test] �ő�ŏ����l���F�߂��Ȃ����ǂ����̃e�X�g
		m_cRule.clear();
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax same value error test failed.", false, m_cRule.AddRistrict( _T("FIELDSURVEY_F=3-3") ) );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax same value error test failed.", false, m_cRule.AddRistrict( _T("FIELDSURVEY_F=3-3:W") ) );

		// [test] �G���[�ɂȂ邩�ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=2-10") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �ŏ��̋��E�l�ŃG���[�ɂȂ邩�ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3-10") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min threthold error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �ő�̋��E�l�ŃG���[�ɂȂ邩�ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1-3") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max threthold error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �ŏ������l�ŃG���[�ɂȂ邩�ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-5-10") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min is minus error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �ő傪���l�ŃG���[�ɂȂ邩�ǂ����̃e�X�g
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), -5 );
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-30--2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max is minus error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
 	 * @brief �ő�l�E�ŏ��l�ł̐������[���̔�G���[�e�X�g
	 * @li �P���ȃ��[���ŃG���[�ɂȂ�Ȃ����ǂ���
	 * @li �ŏ��̋��E�l�ŃG���[�ɂȂ�Ȃ����ǂ����̃e�X�g
	 * @li �ő�̋��E�l�ŃG���[�ɂȂ�Ȃ����ǂ����̃e�X�g
	 * @li �ŏ������l�ŃG���[�ɂȂ��Ȃ��ǂ����̃e�X�g
	 * @li �ő傪���l�ŃG���[�ɂȂ��Ȃ��ǂ����̃e�X�g
	 */
	void test_rule_minmax_non_error()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );

		// [test] �G���[�ɂȂ�Ȃ����ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=5-10") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �ŏ��̋��E�l�ŃG���[�ɂȂ�Ȃ����ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=4-10") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min threthold error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �ő�̋��E�l�ŃG���[�ɂȂ�Ȃ����ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1-2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max threthold error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �ŏ������l�ŃG���[�ɂȂ�Ȃ����ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-5-2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min is minus error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �ő傪���l�ŃG���[�ɂȂ�Ȃ����ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-30--2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max is minus error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
 	 * @brief �ő�l�E�ŏ��l�ł̐������[���̌x���e�X�g
	 * @li �P���ȃ��[���Ōx���ɂȂ邩�ǂ���
	 * @li �ŏ��̋��E�l�Ōx���ɂȂ邩�ǂ����̃e�X�g
	 * @li �ő�̋��E�l�Ōx���ɂȂ邩�ǂ����̃e�X�g
	 * @li �ŏ������l�Ōx���ɂȂ邩�ǂ����̃e�X�g
	 * @li �ő傪���l�Ōx���ɂȂ邩�ǂ����̃e�X�g
	 */
	void test_rule_minmax_warning()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );

		// [test] �x���ɂȂ邩�ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=2-10:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax error test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �ŏ��̋��E�l�Ōx���ɂȂ邩�ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3-10:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min threthold error test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �ő�̋��E�l�Ōx���ɂȂ邩�ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1-3:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max threthold error test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �ŏ������l�Ōx���ɂȂ邩�ǂ����̃e�X�g
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-5-10:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min is minus error test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

/*		// [test] �ő傪���l�Ōx���ɂȂ邩�ǂ����̃e�X�g
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F:W"), -5 );
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-30--2:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max is minus error test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );*/
	}
	/**
	 * @brief �����l�ł̐������[���̃G���[�e�X�g
	 */
	void test_rule_values_error()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );

		// [test] �P���ȃ��[���ŃG���[�ɂȂ邩�ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=2,3,4") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[1] error test of rule of values failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �P���ȃ��[���ŃG���[�ɂȂ�Ȃ����ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=4,5,6") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[2] error test of rule of values failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] ���l���E�ŃG���[�ɂȂ邩�ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3,4,5") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] error test of rule of values failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �E�l���E�ŃG���[�ɂȂ邩�ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1,2,3") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[4] error test of rule of values failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] ���l�������Ă���ꍇ�ɃG���[�ɂȂ邩�ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-2,3,4") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[5] error test of rule of values failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] ���l�������Ă���ꍇ�ɃG���[�ɂȂ�Ȃ����ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-2,4,5") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[6] error test of rule of values failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
	 * @brief �����l�ł̐������[���x���e�X�g
	 */
	void test_rule_values_warning()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );

		// [test] �P���ȃ��[���Ōx���ɂȂ邩�ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=2,3,4:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[1] warning test of rule of values failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �P���ȃ��[���Ōx���ɂȂ�Ȃ����ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=4,5,6:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[2] warning test of rule of values failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] ���l���E�ŃG���[�ɂȂ邩�ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3,4,5:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] warning test of rule of values failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �E�l���E�ŃG���[�ɂȂ邩�ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1,2,3:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[4] warning test of rule of values failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] ���l�������Ă���ꍇ�ɃG���[�ɂȂ邩�ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-2,3,4:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[5] warning test of rule of values failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] ���l�������Ă���ꍇ�ɃG���[�ɂȂ�Ȃ����ǂ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-2,4,5:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[6] warning test of rule of values failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
	 * @brief AND�����ł̃��[���e�X�g
	 */
#include <comutil.h>
	void test_rule_and()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );
		m_cRow.SetLongValueByFieldName(_T("ROADCLASS_C"), 1 );

		// [test] 2��AND�����̂�����Ƃ����v�̏ꍇ�ɃG���[�ƂȂ邩
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3|ROADCLASS_C=1") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[1] error test of rule of and failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] 2��AND�����̂�����̂ݍ��v�̏ꍇ�ɃG���[�ƂȂ�Ȃ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3|ROADCLASS_C=2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[2] error test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] 2��AND�����̂�����Ƃ����v���Ȃ��ꍇ�ɃG���[�ƂȂ�Ȃ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=5|ROADCLASS_C=2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] error test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 2��AND�����̂�����Ƃ����v�̏ꍇ�Ɍx���ƂȂ邩
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3|ROADCLASS_C=1:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[1] warning test of rule of and failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] 2��AND�����̂�����̂ݍ��v�̏ꍇ�Ɍx���ƂȂ�Ȃ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3|ROADCLASS_C=2:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[2] warning test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] 2��AND�����̂�����Ƃ����v���Ȃ��ꍇ�Ɍx���ƂȂ�Ȃ���
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=5|ROADCLASS_C=2:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] warning test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] �������G�ȏ����ŃG���[�����o�ł��邩�ibug 4521�j
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 1 );
		m_cRow.SetLongValueByFieldName(_T("ROADCLASS_C"), 1 );
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1|ROADCLASS_C!=0") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[bug 4521] error test of rule of and failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		m_cRow.SetLongValueByFieldName(_T("ROADCLASS_C"), 0 );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[bug 4521] non-error test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		m_cRow.SetStringValueByFieldName(_T("SOURCE"), _T("aaa") );
		m_cRow.SetLongValueByFieldName(_T("USERCLAIM_F"), 1 );
		m_cRow.SetLongValueByFieldName(_T("ROADCLASS_C"), 150 );
		m_cRule.clear();
		m_cRule.AddRistrict(_T("SOURCE!=NULL|USERCLAIM_F=1|ROADCLASS_C=100-200:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[bug 4521] warning test of rule of and failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		VARIANT vaNull;
		VariantInit(&vaNull);
		vaNull.vt = VT_NULL;
		m_cRow.SetValueByFieldName(_T("SOURCE"), vaNull );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[bug 4521] non-warning test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
	 * @brief AND�����Ƒ��̏�����MIX�e�X�g
	 */
	void test_rule_and_mix()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 1 );
		m_cRow.SetLongValueByFieldName(_T("ROADCLASS_C"), 15 );
		m_cRow.SetLongValueByFieldName(_T("DISPCLASS_C"), 100 );
		m_cRow.SetLongValueByFieldName(_T("TO_NODE_ID"), 3 );
		// [test] �Е��̏����̂݃G���[�ƂȂ�ꍇ�ɃG���[�ƂȂ邩
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1|ROADCLASS_C=15,16,17|DISPCLASS_C!=200-500") );
		m_cRule.AddRistrict( _T("TO_NODE_ID=4") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[1] error test of rule of and-mix failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] �Е��̏������G���[�A�����Е����x���ƂȂ�ꍇ�ɃG���[�ƂȂ邩
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1|ROADCLASS_C=15,16,17|DISPCLASS_C!=200-500") );
		m_cRule.AddRistrict( _T("TO_NODE_ID=3:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[2] error test of rule of and-mix failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] �����̏������x���ƂȂ�ꍇ�Ɍx���ƂȂ邩
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1|ROADCLASS_C=15,16,17|DISPCLASS_C!=200-500:W") );
		m_cRule.AddRistrict( _T("TO_NODE_ID=3:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] warning test of rule of and-mix failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] �ǂ̏����ɂ�����������Ȃ��ꍇ�ɕҏW�ƂȂ邩
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=2|ROADCLASS_C=15,16,17|DISPCLASS_C!=200-500") );
		m_cRule.AddRistrict( _T("TO_NODE_ID=4:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] warning test of rule of and-mix failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

	}
private:
	CWorkspace m_cWork;
	CRow m_cRow;
	CAttributeRistrictsOr m_cRule;
	TCHAR m_strMsg[1024];
};

CPPUNIT_TEST_SUITE_REGISTRATION(CAttributeRistrictTest);
