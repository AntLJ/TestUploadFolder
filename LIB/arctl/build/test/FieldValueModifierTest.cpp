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
 * @file FieldValueModifierTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <arctl/pgdb.h>
#include <arctl/field/value.h>
#include <arctl/field/shape.h>
#include <arctl/field/modifier.h>

using namespace arctl::field;

/**
 * @ingroup unit_test
 * @brief arctl::field::modifier��@ref unit_test�N���X�B
 */
class FieldValueModifierTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(FieldValueModifierTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		// �e�X�g�p��PGDB���c���Ă�����폜����B
		::DeleteFile(kTestWorkspace);

		// �e�X�g�p��PGDB���쐬����B
		IFeatureWorkspacePtr ipFeatureWorkspace = arctl::create_pgdb(kTestWorkspace);
		CPPUNIT_ASSERT(ipFeatureWorkspace != 0);

		// �e�X�g�p�̑����t�B�[���h���쐬����B
		IFieldsEditPtr ipFieldsEdit(IObjectClassDescriptionPtr(CLSID_FeatureClassDescription)->GetRequiredFields());

		for(long i = 0; i < ipFieldsEdit->GetFieldCount(); i++) {
			IFieldEditPtr ipFieldEdit(ipFieldsEdit->GetField(i));
			if(ipFieldEdit->GetType() == esriFieldTypeGeometry) {
				IGeometryDefEditPtr(ipFieldEdit->GetGeometryDef())->put_GeometryType(esriGeometryPoint);
				break;
			}
		}

		// ����2:VALUE1:�����^�B
		IFieldEditPtr ipFieldEdit;
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(kValue1)));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeInteger));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		// �e�X�g�p�̃t�B�[�`���N���X���쐬����B
		CPPUNIT_ASSERT_EQUAL(
			S_OK,
			ipFeatureWorkspace->CreateFeatureClass(
				CComBSTR(OLESTR("test_class")),
				ipFieldsEdit,
				0, 0,
				esriFTSimple,
				CComBSTR(kShape),
				0,
				&m_ipFeatureClass
			)
		);
	}

	virtual void tearDown()
	{
		m_ipFeatureClass = 0;

		// �e�X�g�p��PGDB���폜����B
		CPPUNIT_ASSERT_EQUAL(TRUE, ::DeleteFile(kTestWorkspace));
	}

//@{ @name �e�X�g�P�[�X
	void test()
	{
		// �C���f�b�N�X�p�ӁB
		FieldValue::index_type aFieldIndex(m_ipFeatureClass);

		// �����l�p�ӊJ�n�B
		FieldValue aFieldValue;
		index_t<kValue1> aIndex1(aFieldIndex);

		// SHAPE�ݒ�B
		IPointPtr ipPoint(CLSID_Point);
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_X(10.0));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_Y(20.0));
		static_cast<shape_base_t<>&>(aFieldValue).value() = ipPoint;

		// VALUE1�ݒ�
		get<kValue1>(aFieldValue) = 1L;

		// IFeatureBuffer�����B
		IFeatureBufferPtr ipFeatureBuffer;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipFeatureClass->CreateFeatureBuffer(&ipFeatureBuffer));
		CPPUNIT_ASSERT(aFieldValue.write(ipFeatureBuffer, aFieldIndex));
		// SHAPE����ւ��B
		ipPoint.CreateInstance(CLSID_Point);
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_X(10.0));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_Y(20.0));
		static_cast<shape_base_t<>&>(aFieldValue).value() = ipPoint;

		// �����l��n���Ă��X�V�ł��Ȃ��B
		CPPUNIT_ASSERT_EQUAL(false, make_modifier(aFieldValue, aFieldIndex)(ipFeatureBuffer));

		IPointPtr ipFeaturePoint(ipFeatureBuffer->Shape);
		CPPUNIT_ASSERT_EQUAL(10.0, ipFeaturePoint->GetX());
		CPPUNIT_ASSERT_EQUAL(20.0, ipFeaturePoint->GetY());
		CPPUNIT_ASSERT_EQUAL(1L, static_cast<long>(ipFeatureBuffer->Value[aIndex1]));

		// �قȂ�l�̂Ƃ��̂ݍX�V�ł���B
		get<kValue1>(aFieldValue) = 2L;
		CPPUNIT_ASSERT_EQUAL(true, make_modifier(aFieldValue, aFieldIndex)(ipFeatureBuffer));
		CPPUNIT_ASSERT_EQUAL(2L, static_cast<long>(ipFeatureBuffer->Value[aIndex1]));

		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_X(20.0));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_Y(30.0));
		CPPUNIT_ASSERT_EQUAL(true, make_modifier(aFieldValue, aFieldIndex)(ipFeatureBuffer));
		ipFeaturePoint = ipFeatureBuffer->Shape;
		CPPUNIT_ASSERT_EQUAL(20.0, ipFeaturePoint->GetX());
		CPPUNIT_ASSERT_EQUAL(30.0, ipFeaturePoint->GetY());

		get<kValue1>(aFieldValue) = 3L;
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_X(30.0));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_Y(30.0));
		CPPUNIT_ASSERT_EQUAL(true, make_modifier(aFieldValue, aFieldIndex)(ipFeatureBuffer));
		ipFeaturePoint = ipFeatureBuffer->Shape;
		CPPUNIT_ASSERT_EQUAL(3L, static_cast<long>(ipFeatureBuffer->Value[aIndex1]));
		CPPUNIT_ASSERT_EQUAL(30.0, ipFeaturePoint->GetX());
		CPPUNIT_ASSERT_EQUAL(30.0, ipFeaturePoint->GetY());
	}
//@}

private:
// ��`
	ARCTL_FIELD_VALUE_CLASS(FieldValue, ((shape_t<>)) (2(kValue1, long)) );

// �ϐ�
	IFeatureClassPtr m_ipFeatureClass;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FieldValueModifierTest);
