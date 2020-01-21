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
#include "CppUnitTest.h"

#include <arctl/coinitializer.h>

#include <atltime.h>

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

// sindy::schema
#include <schema.h>

// �e�X�g�Ώ�
#include "AheBuddyGlobals.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{		
	// �e�X�g�f�[�^(PGDB)
	const TCHAR* TESTDB = _T("..\\unittest\\TestData\\WalkNw.mdb");

	TEST_CLASS(AheBuddyGlobalsTest)
	{

	public:
		
		TEST_METHOD(JedgeLinkRelationTest)
		{
			const arctl::coinitializer aCoInitializer;

			using namespace sindy;
			using namespace schema;

			// Workspace �ɐڑ�
			CWorkspace cWS;
			if( ! cWS.Connect(TESTDB) )
				Assert::Fail();

			// �e�X�g����
			//   �e�[�u���t�@�C���_����
			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( _T("WALK_LINK")) , CModel() );
			//   �e�[�u���ڑ�
			CSPTableContainer walk_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("WALK_LINK") ));
			auto road_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("ROAD_LINK") ));
			walk_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );
			road_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// �ȉ��A�e�X�g���s
			sindy::LinkRelationPosition nResultCode = sindy::LinkRelationPosition::kNotFound;
			// �p�^�[��1
			IGeometryPtr ipBaseGeo   = getGeoByOID( walk_table, 12 );
			IGeometryPtr ipTargetGeo = getGeoByOID( road_table, 13 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��2
			ipBaseGeo   = getGeoByOID( walk_table, 13 );
			ipTargetGeo = getGeoByOID( road_table, 15 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��3
			ipBaseGeo   = getGeoByOID( walk_table, 15 );
			ipTargetGeo = getGeoByOID( road_table, 17 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 1);	// �E

			// �p�^�[��4
			ipBaseGeo   = getGeoByOID( walk_table, 16 );
			ipTargetGeo = getGeoByOID( road_table, 18 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 5.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 0);	// ������Ȃ�

			// �p�^�[��5
			ipBaseGeo   = getGeoByOID( walk_table, 19 );
			ipTargetGeo = getGeoByOID( road_table, 21 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 1);	// �E

			// �p�^�[��6
			ipBaseGeo   = getGeoByOID( walk_table, 21 );
			ipTargetGeo = getGeoByOID( road_table, 22 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 20.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��7
			ipBaseGeo   = getGeoByOID( walk_table, 23 );
			ipTargetGeo = getGeoByOID( road_table, 25 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��8
			ipBaseGeo   = getGeoByOID( walk_table, 24 );
			ipTargetGeo = getGeoByOID( road_table, 26 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��9
			ipBaseGeo   = getGeoByOID( walk_table, 26 );
			ipTargetGeo = getGeoByOID( road_table, 27 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 50.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 1);	// �E

			// �p�^�[��10
			ipBaseGeo   = getGeoByOID( walk_table, 28 );
			ipTargetGeo = getGeoByOID( road_table, 28 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 50.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��11
			ipBaseGeo   = getGeoByOID( walk_table, 29 );
			ipTargetGeo = getGeoByOID( road_table, 29 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��12
			ipBaseGeo   = getGeoByOID( walk_table, 30 );
			ipTargetGeo = getGeoByOID( road_table, 30 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			 // �p�^�[��13
			ipBaseGeo   = getGeoByOID( walk_table, 33 );
			ipTargetGeo = getGeoByOID( road_table, 33 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��14
			ipBaseGeo   = getGeoByOID( walk_table, 34 );
			ipTargetGeo = getGeoByOID( road_table, 34 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 1);	// �E

			// ���x(10,000��1���ȓ��Ƃ��Ă���)
			ipBaseGeo   = getGeoByOID( walk_table, 31 );
			ipTargetGeo = getGeoByOID( road_table, 31 );
			CFileTime cTimeStart = CFileTime::GetCurrentTime();
			for( int i = 0; i < 10000; ++i )
				AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode );
			CFileTime cTimeEnd = CFileTime::GetCurrentTime(); 
			CFileTimeSpan cTimeSpan = cTimeEnd - cTimeStart;
			Assert::IsTrue( cTimeSpan.GetTimeSpan()/10000 < 60000 );  //[ms]

		}

		TEST_METHOD(JudgeLinkRelation2Test)
		{
			const arctl::coinitializer aCoInitializer;

			using namespace sindy;
			using namespace schema;

			// Workspace �ɐڑ�
			CWorkspace cWS;
			if( ! cWS.Connect(TESTDB) )
				Assert::Fail();

			// �e�X�g����
			//   �e�[�u���t�@�C���_����
			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( _T("WALK_LINK")) , CModel() );
			//   �e�[�u���ڑ�
			CSPTableContainer walk_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("WALK_LINK") ));
			auto road_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("ROAD_LINK") ));
			walk_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );
			road_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// �ȉ��A�e�X�g���s
			unsigned int nResultCode = 99;
			// �p�^�[��1
			IGeometryPtr ipBaseGeo   = getGeoByOID( walk_table, 12 );
			IGeometryPtr ipTargetGeo = getGeoByOID( road_table, 13 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��2
			ipBaseGeo   = getGeoByOID( walk_table, 13 );
			ipTargetGeo = getGeoByOID( road_table, 15 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��3
			ipBaseGeo   = getGeoByOID( walk_table, 16 );
			ipTargetGeo = getGeoByOID( road_table, 18 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��4
			ipBaseGeo   = getGeoByOID( walk_table, 23 );
			ipTargetGeo = getGeoByOID( road_table, 25 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��5
			ipBaseGeo   = getGeoByOID( walk_table, 24 );
			ipTargetGeo = getGeoByOID( road_table, 26 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// �p�^�[��6
			ipBaseGeo   = getGeoByOID( walk_table, 26 );
			ipTargetGeo = getGeoByOID( road_table, 27 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 1);	// �E

			// �p�^�[��7
			ipBaseGeo   = getGeoByOID( walk_table, 28 );
			ipTargetGeo = getGeoByOID( road_table, 28 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			 // �p�^�[��8
			ipBaseGeo   = getGeoByOID( walk_table, 33 );
			ipTargetGeo = getGeoByOID( road_table, 33 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// ��

			// ���x(10,000��1���ȓ��Ƃ��Ă���)
			ipBaseGeo   = getGeoByOID( walk_table, 31 );
			ipTargetGeo = getGeoByOID( road_table, 31 );
			CFileTime cTimeStart = CFileTime::GetCurrentTime();
			for( int i = 0; i < 10000; ++i )
				AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			CFileTime cTimeEnd = CFileTime::GetCurrentTime(); 
			CFileTimeSpan cTimeSpan = cTimeEnd - cTimeStart;
			Assert::IsTrue( cTimeSpan.GetTimeSpan()/10000 < 60000 );  //[ms]

		}

		TEST_METHOD(JudgeLinkRelation3Test)
		{
			const arctl::coinitializer aCoInitializer;

			using namespace sindy;
			using namespace schema;

			// Workspace �ɐڑ�
			CWorkspace cWS;
			if( ! cWS.Connect(TESTDB) )
				Assert::Fail();

			// �e�X�g����
			//   �e�[�u���t�@�C���_����
			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( _T("WALK_LINK")) , CModel() );
			//   �e�[�u���ڑ�
			CSPTableContainer walk_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("WALK_LINK") ));
			auto road_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("ROAD_LINK") ));
			walk_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );
			road_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// �ȉ��A�e�X�g���s
			// �e�p�^�[����JedgeLinkRelationTest�Ńe�X�g���Ă��邽�߁A�����ł͔��茋�ʂŒl���Ó����̂݃`�F�b�N

			sindy::LinkRelationPosition nResultCode = sindy::LinkRelationPosition::kNotFound;
			double dFindDist = 0.0;
			// �p�^�[��1
			IGeometryPtr ipBaseGeo   = getGeoByOID( walk_table, 12 );
			IGeometryPtr ipTargetGeo = getGeoByOID( road_table, 13 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode, dFindDist );
			Assert::AreEqual((int)nResultCode, 2);	// ��
			Assert::IsTrue( 15.0 < dFindDist &&  dFindDist < 16.0 );	// ��15.8m

			// �p�^�[��2

			// �p�^�[��3
			ipBaseGeo   = getGeoByOID( walk_table, 15 );
			ipTargetGeo = getGeoByOID( road_table, 17 );
			dFindDist = 0.0;
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode, dFindDist );
			Assert::AreEqual((int)nResultCode, 1);	// �E
			Assert::IsTrue( 10.0 < dFindDist &&  dFindDist < 11.0 );	// ��10.4m

			// �p�^�[��4
			ipBaseGeo   = getGeoByOID( walk_table, 16 );
			ipTargetGeo = getGeoByOID( road_table, 18 );
			dFindDist = 0.0;
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 5.0, nResultCode, dFindDist );
			Assert::AreEqual((int)nResultCode, 0);	// ������Ȃ�
			Assert::AreEqual(dFindDist, -1.0);		// -1.0m

		}

	private:
		// TableFinder����OID��1���R�[�h�擾
		IGeometryPtr getGeoByOID( const sindy::CSPTableContainer& table, long lOid )
		{
			using namespace sindy;
			using namespace schema;

			IGeometryPtr ipRetGeo;
			_IRowPtr ipRow = table->GetRow( lOid );
			IFeaturePtr(ipRow)->get_Shape( &ipRetGeo );
			return ipRetGeo;
		}

	};
}
