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
 * @file SiNDYLibDispLineTest.cpp
 * @brief SiNDYLibの CDispLine をテストするクラスの定義ファイルです。
 * @author 古川貴宏（コンテンツ本部地図DB制作部システム開発グループ）
 * @version $Id$
 */
#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "Workspace.h"
#include "DispLine.h"
#include "ErrorObject.h"
#include "ErrorCode.h"
#include "Annotations.h"
#include "GlobalRules.h"
#include "Rule.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "GeometryRule.h"

using namespace sindy;
using namespace sindy::errorcode;

/**
 * @class CSiNDYDispLineTest
 * @brief SiNDYLibの CDispLine の動作をテストするクラス
 * <h3>チェック内容：<h3>
 * @li 一行注記が判別できるか
 * @li 二行注記が判別できるか
 * @li 縦書き注記が判別できるか
 * @li 線状注記が判別できるか
 * @li 二行注記の二行目開始インデックスが正しく取得できるか
 * @li [bug 6046] 二行注記の一行目が一文字でも正しく二行注記と判定されるか
 */
using namespace sindy;

class CSiNDYDispLineTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CSiNDYDispLineTest);
  CPPUNIT_TEST(testCalcParameter_1);
  CPPUNIT_TEST_SUITE_END();
public:
	CSiNDYDispLineTest(){}
	virtual ~CSiNDYDispLineTest(){}

	void setUp ()
	{
		g_cRule.CreateRuleCache( true, false );
		CWorkspace cWork(_T("Test.mdb"));
		m_cAnno.SetObject( sindyTableType::city_annotation, (IFeatureClassPtr)cWork.OpenTable(_T("CITY_ANNOTATION")) );
		m_cAnno.SelectByMesh( 59414131, mesh_type::kCity, true, true );
		// チェック
		CLogicRule cRule;
		cRule.SetRelThresholdPx( schema::city_annotation::kTableName, schema::city_disp_line::kTableName, std::pair<double,double>(-1.4,16.0) );
		cRule.SetRelThresholdPx( schema::base_annotation::kTableName, schema::b_sc4disp_line::kTableName, std::pair<double,double>(-1.3,16.0) );
		cRule.SetRelThresholdPx( schema::base_annotation::kTableName, schema::b_sc3disp_line::kTableName, std::pair<double,double>(-1.3,16.0) );
		cRule.SetRelThresholdPx( schema::base_annotation::kTableName, schema::b_sc2disp_line::kTableName, std::pair<double,double>(-1.3,16.0) );
		cRule.SetRelThresholdPx( schema::base_annotation::kTableName, schema::b_sc1disp_line::kTableName, std::pair<double,double>(-1.3,16.0) );
		cRule.SetRelThresholdPx( schema::middle_annotation::kTableName, schema::m_sc4disp_line::kTableName, std::pair<double,double>(-0.2,15.0) );
		cRule.SetRelThresholdPx( schema::middle_annotation::kTableName, schema::m_sc3disp_line::kTableName, std::pair<double,double>(-0.2,15.0) );
		cRule.SetRelThresholdPx( schema::middle_annotation::kTableName, schema::m_sc2disp_line::kTableName, std::pair<double,double>(-0.2,15.0) );
		cRule.SetRelThresholdPx( schema::middle_annotation::kTableName, schema::m_sc1disp_line::kTableName, std::pair<double,double>(-0.2,15.0) );
		cRule.SetRelThresholdPx( schema::top_annotation::kTableName, schema::t_sc4disp_line::kTableName, std::pair<double,double>(0.0,10.0) );
		cRule.SetRelThresholdPx( schema::top_annotation::kTableName, schema::t_sc3disp_line::kTableName, std::pair<double,double>(0.0,10.0) );
		cRule.SetRelThresholdPx( schema::top_annotation::kTableName, schema::t_sc2disp_line::kTableName, std::pair<double,double>(0.0,10.0) );
		cRule.SetRelThresholdPx( schema::top_annotation::kTableName, schema::t_sc1disp_line::kTableName, std::pair<double,double>(0.0,10.0) );
		m_cAnno.CheckLogic( cRule, m_cErrs );
	}


	void tearDown()
	{
	}

	/// 一行二行、縦横の判定のチェック
	void testCalcParameter_1()
	{
		for( CAnnotations::iterator it = m_cAnno.begin(); it != m_cAnno.end(); ++it )
		{
			for( CAnnotations::rows_iterator itAnno = it->second.begin(); itAnno != it->second.end(); ++itAnno )
			{
				CRelAnnotationParameter& cAnno = static_cast<CRelAnnotationParameter&>(**itAnno);
				for( CRelAnnotationParameter::iterator it2 = cAnno.begin(); it2 != cAnno.end(); ++it2 )
				{
					for( CRelAnnotationParameter::rows_iterator itDisp = it2->second.begin(); itDisp != it2->second.end(); ++itDisp )
					{
						CDispLine& cDisp = static_cast<CDispLine&>( **itDisp );
						switch( cAnno.GetOID() )
						{
						case 3:
						case 4:
						case 5:
						case 6:
						case 7:
						case 8:
						case 9:
							CPPUNIT_ASSERT_EQUAL( cDisp.CalcParameter( false ), (sindyAnnotationLineType)(sindyAnnoLine_Horizonal|sindyAnnoLine_Single) );
							break;
						case 1:
						case 10:
						case 13:
						case 14:
							CPPUNIT_ASSERT_EQUAL( cDisp.CalcParameter( false ), (sindyAnnotationLineType)(sindyAnnoLine_Horizonal|sindyAnnoLine_Double) ); // [bug 6046]
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
	/// [bug 6537]
	void test6537()
	{
		const CAnnotation& cAnnoPoint = static_cast<const CAnnotation&>(**m_cAnno.find( sindyTableType::city_annotation, 16 ));
		const CDispLine& cAnnoDisp = static_cast<const CDispLine&>(**cAnnoPoint.begin(sindyTableType::city_disp_line));
	}
private:
	CAnnotations m_cAnno;
	CErrorObjects m_cErrs;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CSiNDYDispLineTest);
