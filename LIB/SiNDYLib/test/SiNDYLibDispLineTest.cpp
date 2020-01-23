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
 * @brief SiNDYLib�� CDispLine ���e�X�g����N���X�̒�`�t�@�C���ł��B
 * @author �Ð�M�G�i�R���e���c�{���n�}DB���암�V�X�e���J���O���[�v�j
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
 * @brief SiNDYLib�� CDispLine �̓�����e�X�g����N���X
 * <h3>�`�F�b�N���e�F<h3>
 * @li ��s���L�����ʂł��邩
 * @li ��s���L�����ʂł��邩
 * @li �c�������L�����ʂł��邩
 * @li ���󒍋L�����ʂł��邩
 * @li ��s���L�̓�s�ڊJ�n�C���f�b�N�X���������擾�ł��邩
 * @li [bug 6046] ��s���L�̈�s�ڂ��ꕶ���ł���������s���L�Ɣ��肳��邩
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
		// �`�F�b�N
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

	/// ��s��s�A�c���̔���̃`�F�b�N
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
