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
 * @brief arctl::field::modifierの@ref unit_testクラス。
 */
class FieldValueModifierTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(FieldValueModifierTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		// テスト用のPGDBが残っていたら削除する。
		::DeleteFile(kTestWorkspace);

		// テスト用のPGDBを作成する。
		IFeatureWorkspacePtr ipFeatureWorkspace = arctl::create_pgdb(kTestWorkspace);
		CPPUNIT_ASSERT(ipFeatureWorkspace != 0);

		// テスト用の属性フィールドを作成する。
		IFieldsEditPtr ipFieldsEdit(IObjectClassDescriptionPtr(CLSID_FeatureClassDescription)->GetRequiredFields());

		for(long i = 0; i < ipFieldsEdit->GetFieldCount(); i++) {
			IFieldEditPtr ipFieldEdit(ipFieldsEdit->GetField(i));
			if(ipFieldEdit->GetType() == esriFieldTypeGeometry) {
				IGeometryDefEditPtr(ipFieldEdit->GetGeometryDef())->put_GeometryType(esriGeometryPoint);
				break;
			}
		}

		// 属性2:VALUE1:整数型。
		IFieldEditPtr ipFieldEdit;
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(kValue1)));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeInteger));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		// テスト用のフィーチャクラスを作成する。
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

		// テスト用のPGDBを削除する。
		CPPUNIT_ASSERT_EQUAL(TRUE, ::DeleteFile(kTestWorkspace));
	}

//@{ @name テストケース
	void test()
	{
		// インデックス用意。
		FieldValue::index_type aFieldIndex(m_ipFeatureClass);

		// 属性値用意開始。
		FieldValue aFieldValue;
		index_t<kValue1> aIndex1(aFieldIndex);

		// SHAPE設定。
		IPointPtr ipPoint(CLSID_Point);
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_X(10.0));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_Y(20.0));
		static_cast<shape_base_t<>&>(aFieldValue).value() = ipPoint;

		// VALUE1設定
		get<kValue1>(aFieldValue) = 1L;

		// IFeatureBuffer生成。
		IFeatureBufferPtr ipFeatureBuffer;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipFeatureClass->CreateFeatureBuffer(&ipFeatureBuffer));
		CPPUNIT_ASSERT(aFieldValue.write(ipFeatureBuffer, aFieldIndex));
		// SHAPE入れ替え。
		ipPoint.CreateInstance(CLSID_Point);
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_X(10.0));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_Y(20.0));
		static_cast<shape_base_t<>&>(aFieldValue).value() = ipPoint;

		// 同じ値を渡しても更新できない。
		CPPUNIT_ASSERT_EQUAL(false, make_modifier(aFieldValue, aFieldIndex)(ipFeatureBuffer));

		IPointPtr ipFeaturePoint(ipFeatureBuffer->Shape);
		CPPUNIT_ASSERT_EQUAL(10.0, ipFeaturePoint->GetX());
		CPPUNIT_ASSERT_EQUAL(20.0, ipFeaturePoint->GetY());
		CPPUNIT_ASSERT_EQUAL(1L, static_cast<long>(ipFeatureBuffer->Value[aIndex1]));

		// 異なる値のときのみ更新できる。
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
// 定義
	ARCTL_FIELD_VALUE_CLASS(FieldValue, ((shape_t<>)) (2(kValue1, long)) );

// 変数
	IFeatureClassPtr m_ipFeatureClass;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FieldValueModifierTest);
