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

// �e�X�g�Ώ�
#include "../StationChecker.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Arc
#include <arctl/coinitializer.h>

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

// sindy::schema
#include <sindy/schema.h>

namespace unittest
{	
	// �e�X�g�f�[�^ (PGDB)
	const TCHAR* TEST_DB = _T("..\\unittest\\TestData\\TestData.mdb");
	const TCHAR* TEST_DB_ERR = _T("..\\unittest\\TestData\\TestData_Err.mdb");


	TEST_CLASS(StationCheckerTest)
	{
	public:

		// �������֐��̃`�F�b�N
		TEST_METHOD(init_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;

			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CWorkspace cWS_Err;
			if( ! cWS_Err.Connect(TEST_DB_ERR) )
				Assert::Fail();

			
			// ����
			// �������ɐ������邩
			{
				StationChecker checker;
				if( ! checker.init( cWS ) )
					Assert::Fail();
			}

			// �G���[
			// STASIOTN_DETAIL�����݂��Ȃ�Workspace��
			// �n�����Ƃ��������Ɏ��s���邩
			{
				StationChecker checker;
				if( checker.init( cWS_Err ) )
					Assert::Fail();
			}
		}

		// �`�F�b�N�֐��̃e�X�g
		TEST_METHOD(checkPolygonC_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;
			using namespace sindy::schema::sj;

			// �e�X�g�f�[�^�擾����
			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( station_detail::kTableName ) , CModel() );

			const auto& table = cFinder.FindTable( station_detail::kTableName );
			table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );

			// ����
			StationChecker checker;
			if( ! checker.init( cWS ) )
				Assert::Fail();;;

			// �e�X�g -----------------------------
			// ����
			{
				const auto& row = getRowCByOID( table, 5);
				Assert::IsTrue( checker.checkPolygonC( *row ) );
			}
			// �G���[
			{
				const auto& row = getRowCByOID( table, 10);
				Assert::IsFalse( checker.checkPolygonC( *row ) );
			}
			// ------------------------------------

		}

		// �w���R�t���`�F�b�N
		TEST_METHOD(checkLinkedStation_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;
			using namespace sindy::schema::sj;

			// �e�X�g�f�[�^�擾����
			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( station_detail::kTableName ) , CModel() );

			const auto& table = cFinder.FindTable( station_detail::kTableName );
			table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// ����
			StationChecker checker;
			if( ! checker.init( cWS ) )
				Assert::Fail();;

			// �e�X�g -----------------------------
			// ����
			{
				const auto& row = getRowCByOID( table, 5);
				Assert::IsTrue( checker.checkLinkedStation( *row, station_detail::kTableName ) );
			}

			// �G���[
			{
				const auto& row = getRowCByOID( table, 6);
				Assert::IsFalse( checker.checkLinkedStation( *row, station_detail::kTableName ) );
			}
			// ------------------------------------
		}

		// �w���R�t���`�F�b�N
		TEST_METHOD(checkContain_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;
			using namespace sindy::schema::sj;

			// �e�X�g�f�[�^�擾����
			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( station_detail::kTableName ) , CModel() );

			const auto& table = cFinder.FindTable( station_detail::kTableName );
			table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );

			const auto& table2 = cFinder.FindTable( highzoom_station::kTableName );
			table2->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );

			// ����
			StationChecker checker;
			if( ! checker.init( cWS ) )
				Assert::Fail();;

			// �e�X�g -----------------------------
			// ����
			// ���S�ɓ���Ă���p�^�[��
			{
				const auto& row = getRowCByOID( table, 5);
				Assert::IsTrue( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}
			// �ꕔ�̕ӂ��ڂ���`�œ���Ă���p�^�[��
			{
				const auto& row = getRowCByOID( table, 8);
				Assert::IsTrue( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}

			// �G���[
			// �|���S���S�̂��͂ݏo���Ă���p�^�[��
			{
				const auto& row = getRowCByOID( table, 7);
				Assert::IsFalse( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}
			// �ꕔ�����͂ݏo���Ă���p�^�[��
			{
				const auto& row = getRowCByOID( table, 9);
				Assert::IsFalse( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}
			// ����Ă��邪�wID���قȂ�p�^�[��
			{
				const auto& row = getRowCByOID( table, 12);
				Assert::IsTrue( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsFalse( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}
			// ���̃��C���Ƀt�B�[�`����1�����Ȃ��P�[�X
			{
				const auto& row = getRowCByOID( table2, 8);
				Assert::IsFalse( checker.checkContain( *row, midzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, midzoom_station::kTableName ) );
			}
			// ���g�ɉwID���R�t�����Ă��Ȃ��A�������|���S�������݂���P�[�X
			// �ʂ̃`�F�b�N�Ō��o���ׂ����e�̂��߁A�G���[�Ƃ��Ȃ�
			{
				const auto& row = getRowCByOID( table, 15);
				Assert::IsTrue( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}

			// ���̃|���S���ɉwID���R�t�����Ă��Ȃ��P�[�X
			{
				const auto& row = getRowCByOID( table, 14);
				Assert::IsTrue( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsFalse( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}
			// ------------------------------------
		}

		//  �w�|���S���Ƃ̕R�t���������`�F�b�N
		TEST_METHOD(checkLinkedNotExistPolygon_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;
			using namespace sindy::schema::sj;

			// �e�X�g�f�[�^�擾����
			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( rel_station_site::kTableName ) , CModel() );

			const auto& table = cFinder.FindTable( rel_station_site::kTableName );
			table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// ����
			StationChecker checker;
			if( ! checker.init( cWS ) )
				Assert::Fail();;

			// �e�X�g -----------------------------
			// ����
			{
				const auto& row = getRowCByOID( table, 15);
				Assert::IsTrue( checker.checkLinkedNotExistPolygon( *row ) );
			}
			// �G���[
			{
				const auto& row = getRowCByOID( table, 16);
				Assert::IsFalse( checker.checkLinkedNotExistPolygon( *row ) );
			}
			// ------------------------------------

		}

		//  �w�Ƃ̕R�t���������`�F�b�N
		TEST_METHOD(checkLinkedNotExistStation_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;
			using namespace sindy::schema::sj;

			// �e�X�g�f�[�^�擾����
			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( rel_station_site::kTableName ) , CModel() );

			const auto& table = cFinder.FindTable( rel_station_site::kTableName );
			table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// ����
			StationChecker checker;
			if( ! checker.init( cWS ) )
				Assert::Fail();;

			// �e�X�g -----------------------------
			// ����
			{
				const auto& row = getRowCByOID( table, 15);
				Assert::IsTrue( checker.checkLinkedNotExistStation( *row ) );
			}
			// �G���[
			{
				const auto& row = getRowCByOID( table, 17);
				Assert::IsFalse( checker.checkLinkedNotExistStation( *row ) );
			}
			// ------------------------------------
		}


	// �e�X�g�p�֐�

	private:

		// TableContainer����OID��1���R�[�h�擾
		sindy::CSPRowContainer getRowCByOID( const sindy::CSPTableContainer& table, long lOid )
		{
			using namespace sindy;
			using namespace schema;

			const auto& row = *table->find_by_key( sindy::schema::ipcfeature::kObjectID, lOid );
			const auto& row_c = CAST_ROWC(row);

			return row_c;
		}

	};	// TEST_CLASS


}
