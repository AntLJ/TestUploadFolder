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
 * @file FieldShapeTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <arctl/pgdb.h>
#include <arctl/field/shape.h>

using namespace arctl::field;

/**
 * @ingroup unit_test
 * @brief arctl::field::shape_tの@ref unit_testクラス。
 */
class FieldShapeTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(FieldShapeTest);
	CPPUNIT_TEST(testIndex);
	CPPUNIT_TEST(testShapeConstruct);
	CPPUNIT_TEST(testShapeCopyConstruct);
	CPPUNIT_TEST(testWriteToFeature);
	CPPUNIT_TEST(testWriteToBuffer);
	CPPUNIT_TEST(testPolylineEquals);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		m_ipFeatureWorkspace =
			IScratchWorkspaceFactoryPtr(__uuidof(ScratchWorkspaceFactory))->_CreateNewScratchWorkspace();

		// テスト用の属性フィールド（点形状）を作成する。
		IFieldsEditPtr ipFieldsEdit1(IObjectClassDescriptionPtr(CLSID_FeatureClassDescription)->GetRequiredFields());

		for(long i = 0; i < ipFieldsEdit1->GetFieldCount(); i++) {
			IFieldEditPtr ipFieldEdit(ipFieldsEdit1->GetField(i));
			if(ipFieldEdit->GetType() == esriFieldTypeGeometry) {
				IGeometryDefEditPtr(ipFieldEdit->GetGeometryDef())->put_GeometryType(esriGeometryPoint);
				break;
			}
		}

		// テスト用のフィーチャクラス（点形状）を作成する。
		CPPUNIT_ASSERT_EQUAL(
			S_OK,
			m_ipFeatureWorkspace->CreateFeatureClass(
				CComBSTR(OLESTR("test_class1")),
				ipFieldsEdit1,
				0,
				0,
				esriFTSimple,
				CComBSTR(kShape),
				0,
				&m_ipFeatureClass1
			)
		);

		// テスト用の属性フィールド（点列形状）を作成する。
		IFieldsEditPtr ipFieldsEdit2(IObjectClassDescriptionPtr(CLSID_FeatureClassDescription)->GetRequiredFields());

		for(long i = 0; i < ipFieldsEdit2->GetFieldCount(); i++) {
			IFieldEditPtr ipFieldEdit(ipFieldsEdit2->GetField(i));
			if(ipFieldEdit->GetType() == esriFieldTypeGeometry) {
				IGeometryDefEditPtr(ipFieldEdit->GetGeometryDef())->put_GeometryType(esriGeometryPolyline);
				break;
			}
		}

		// テスト用のフィーチャクラス（点列形状）を作成する。
		CPPUNIT_ASSERT_EQUAL(
			S_OK,
			m_ipFeatureWorkspace->CreateFeatureClass(
				CComBSTR(OLESTR("test_class2")),
				ipFieldsEdit2,
				0,
				0,
				esriFTSimple,
				CComBSTR(kShape),
				0,
				&m_ipFeatureClass2
			)
		);
	}

	virtual void tearDown()
	{
		m_ipFeatureClass2 = 0;
		m_ipFeatureClass1 = 0;
		m_ipFeatureWorkspace = 0;
	}

//@{ @name テストケース
	/// arctl::field::shape_index_t のテスト。
	void testIndex()
	{
		// コンストラクタのテスト。
		CPPUNIT_ASSERT_EQUAL(-1L, static_cast<long>(shape_index_t()));
		CPPUNIT_ASSERT_EQUAL(1L, static_cast<long>(shape_index_t(m_ipFeatureClass1)));

		// 否定演算子のテスト。
		CPPUNIT_ASSERT_EQUAL(true, ! shape_index_t());
		CPPUNIT_ASSERT_EQUAL(false, ! shape_index_t(m_ipFeatureClass1));
	}

	/// arctl::field::shape_t::shape_t()のテスト。
	void testShapeConstruct()
	{
		// 10個ほど生成する。
		for(int i = 0; i < 10; i++) {
			IFeaturePtr ipFeature;
			CPPUNIT_ASSERT_EQUAL(S_OK, m_ipFeatureClass1->CreateFeature(&ipFeature));

			IPointPtr ipPoint(CLSID_Point);
			CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_X(ipFeature->OID));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_Y(ipFeature->OID * 2.0));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipFeature->putref_Shape(ipPoint));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipFeature->Store());
		}

		CPPUNIT_ASSERT_EQUAL(static_cast<IGeometry*>(0), static_cast<IGeometry*>(shape_t<>().value()));

		IFeatureCursorPtr ipFeatureCursor;
		m_ipFeatureClass1->Search(0, VARIANT_FALSE, &ipFeatureCursor);

		IFeaturePtr ipFeature;
		while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK) {
			shape_t<> aShape(ipFeature); //< IGeometry型
			shape_t<IPoint> aPoint(ipFeature); //< IPoint型特定
			shape_t<IPolyline> aPolyline(ipFeature); //< IPolyline型（不可）

			CPPUNIT_ASSERT(aShape.value() != 0);
			CPPUNIT_ASSERT(aPoint.value() != 0);
			CPPUNIT_ASSERT(aPolyline.value() == 0);

			IPointPtr ipPoint(aPoint.value());
			CPPUNIT_ASSERT_EQUAL(static_cast<double>(ipFeature->OID), ipPoint->GetX());
			CPPUNIT_ASSERT_EQUAL(ipFeature->OID * 2.0 , ipPoint->GetY());

			CPPUNIT_ASSERT((ipPoint = aShape.value()) != 0);
			CPPUNIT_ASSERT_EQUAL(static_cast<double>(ipFeature->OID), ipPoint->GetX());
			CPPUNIT_ASSERT_EQUAL(ipFeature->OID * 2.0 , ipPoint->GetY());
		}
	}

	/// arctl::field::shape_copy_t::shape_copy_t()のテスト。
	void testShapeCopyConstruct()
	{
		// 10個ほど生成する。
		for(int i = 0; i < 10; i++) {
			IFeaturePtr ipFeature;
			CPPUNIT_ASSERT_EQUAL(S_OK, m_ipFeatureClass1->CreateFeature(&ipFeature));

			IPointPtr ipPoint(CLSID_Point);
			CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_X(ipFeature->OID));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_Y(ipFeature->OID * 2.0));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipFeature->putref_Shape(ipPoint));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipFeature->Store());
		}

		CPPUNIT_ASSERT_EQUAL(static_cast<IGeometry*>(0), static_cast<IGeometry*>(shape_copy_t<>().value()));

		IFeatureCursorPtr ipFeatureCursor;
		m_ipFeatureClass1->Search(0, VARIANT_FALSE, &ipFeatureCursor);

		IFeaturePtr ipFeature;
		while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK) {
			shape_copy_t<> aShape(ipFeature); //< IGeometry型
			shape_copy_t<IPoint> aPoint(ipFeature); //< IPoint型特定
			shape_copy_t<IPolyline> aPolyline(ipFeature); //< IPolyline型（不可）

			CPPUNIT_ASSERT(aShape.value() != 0);
			CPPUNIT_ASSERT(aPoint.value() != 0);
			CPPUNIT_ASSERT(aPolyline.value() == 0);

			IPointPtr ipPoint(aPoint.value());
			CPPUNIT_ASSERT_EQUAL(static_cast<double>(ipFeature->OID), ipPoint->GetX());
			CPPUNIT_ASSERT_EQUAL(ipFeature->OID * 2.0 , ipPoint->GetY());

			CPPUNIT_ASSERT((ipPoint = aShape.value()) != 0);
			CPPUNIT_ASSERT_EQUAL(static_cast<double>(ipFeature->OID), ipPoint->GetX());
			CPPUNIT_ASSERT_EQUAL(ipFeature->OID * 2.0 , ipPoint->GetY());
		}
	}

	/// arctl::field::shape_base_t::write(IFeature*, const index_type&) constのテスト。
	void testWriteToFeature()
	{
		IFeaturePtr ipFeature;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipFeatureClass1->CreateFeature(&ipFeature));
		testWrite(ipFeature.GetInterfacePtr());
	}

	/// arctl::field::shape_base_t::write(IFeatureBuffer*, const index_type&) constのテスト。
	void testWriteToBuffer()
	{
		IFeatureBufferPtr ipFeatureBuffer;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipFeatureClass1->CreateFeatureBuffer(&ipFeatureBuffer));
		testWrite(ipFeatureBuffer.GetInterfacePtr());
	}

	/// arctl::field::shape_base_t::equal_value(IFeatureBuffer*, const index_type&) constのテスト。
	void testPolylineEquals()
	{
		WKSPoint pts[2] = {
			{ 10, 10 },
			{ 20, 20 }
		};

		const IPointCollectionPtr ipPointCol(CLSID_Polyline);
		ipPointCol->AddWKSPoints(2, pts);

		IFeatureBufferPtr ipFeatureBuffer;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipFeatureClass2->CreateFeatureBuffer(&ipFeatureBuffer));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFeatureBuffer->putref_Shape(IGeometryPtr(ipPointCol)));

		const ICurvePtr ipCurve(IClonePtr(ipPointCol)->_Clone());
		CPPUNIT_ASSERT(shape_base_t<>(ipCurve).equal_value(ipFeatureBuffer));

		CPPUNIT_ASSERT_EQUAL(S_OK, ipCurve->ReverseOrientation());
		CPPUNIT_ASSERT(! shape_base_t<>(ipCurve).equal_value(ipFeatureBuffer));
	}
//@}

private:
// 関数
	template <typename TIFeature>
	void testWrite(TIFeature* ipFeature)
	{
		IPointPtr ipPoint(CLSID_Point);
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_X(10.0));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipPoint->put_X(20.0));

		// IGeometry型のwrite()。
		CPPUNIT_ASSERT(shape_base_t<>(ipPoint).write(ipFeature));

		// IPoint型のwrite()。
		CPPUNIT_ASSERT(shape_base_t<IPoint>(ipPoint).write(ipFeature));

		// nullポインタのwrite()。
		CPPUNIT_ASSERT(shape_base_t<>().write(ipFeature));
	}

// 変数
	IFeatureWorkspacePtr m_ipFeatureWorkspace;
	IFeatureClassPtr m_ipFeatureClass1;
	IFeatureClassPtr m_ipFeatureClass2;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FieldShapeTest);
