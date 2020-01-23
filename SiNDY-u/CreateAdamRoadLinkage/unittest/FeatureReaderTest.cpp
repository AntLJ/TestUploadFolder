#include "stdafx.h"
#include "CppUnitTest.h"

#include <arctl/coinitializer.h>
#include <boost/assign/list_of.hpp>
#include <TDC/useful_headers/str_util.h>

#include "../FeatureReader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{		
	TEST_CLASS(FeatureReaderTest)
	{
	public:
		
		TEST_METHOD(readTest1)
		{
			using namespace sindy;
			using namespace boost::assign;

			const arctl::coinitializer aCoInitializer;

			const TCHAR* test_db = _T("TRIAL@coral2(ETC_S_MURAKAMI_TEST)");
			const TCHAR* mesh_db = _T("REFERENCE@spinel2(SDE.DEFAULT)");

			IWorkspacePtr ws = sindy::create_workspace( test_db );
			if( ! ws )
				Assert::Fail();
			IWorkspacePtr mesh_ws = sindy::create_workspace( mesh_db );
			if( ! mesh_ws )
				Assert::Fail();

			ITablePtr roadTable;
			if(FAILED(IFeatureWorkspacePtr(ws)->OpenTable(CComBSTR(_T("ROAD_LINK")), &roadTable)))
				Assert::Fail();
			ITablePtr meshTable;
			if(FAILED(IFeatureWorkspacePtr(mesh_ws)->OpenTable(CComBSTR(_T("BASEMESH")), &meshTable)))
				Assert::Fail();
			
			FeatureReader featureReader(roadTable, meshTable);
			Assert::IsTrue( featureReader.read( 533935 ));
			Assert::IsFalse( featureReader.read( 53393533 ));

			std::vector<IFeaturePtr> features = featureReader.getFeatures();
			Assert::IsTrue( features.size() != 0 );

		}

		TEST_METHOD(readTest2)
		{
			using namespace sindy;
			using namespace boost::assign;

			const arctl::coinitializer aCoInitializer;

			const TCHAR* test_db = _T("TRIAL@coral2(ETC_S_MURAKAMI_TEST)");
			const TCHAR* mesh_db = _T("REFERENCE@spinel2(SDE.DEFAULT)");

			IWorkspacePtr ws = sindy::create_workspace( test_db );
			if( ! ws )
				Assert::Fail();
			IWorkspacePtr mesh_ws = sindy::create_workspace( mesh_db );
			if( ! mesh_ws )
				Assert::Fail();

			ITablePtr roadTable;
			if(FAILED(IFeatureWorkspacePtr(ws)->OpenTable(CComBSTR(_T("ROAD_LINK")), &roadTable)))
				Assert::Fail();
			ITablePtr meshTable;
			if(FAILED(IFeatureWorkspacePtr(mesh_ws)->OpenTable(CComBSTR(_T("BASEMESH")), &meshTable)))
				Assert::Fail();
			
			// 空間検索とクエリ検索の併用テスト
			FeatureReader featureReader(roadTable, meshTable);
			CString whereClause = uh::str_util::format(_T("%s=%ld"), _T("OBJECTID"), 4281646);
			Assert::IsTrue( featureReader.read( 533935, whereClause));

			std::vector<IFeaturePtr> features = featureReader.getFeatures();
			Assert::IsTrue( features.size() == 1 );

			// 空間検索とクエリ検索の併用テスト(検索結果なし)
			whereClause = uh::str_util::format(_T("%s=%ld"), _T("OBJECTID"), 4281646);
			Assert::IsTrue( featureReader.read( 533835, whereClause));

			features = featureReader.getFeatures();
			Assert::IsTrue( features.size() == 0 );
		}

		TEST_METHOD(readAroundTest)
		{
			using namespace sindy;
			using namespace boost::assign;

			const arctl::coinitializer aCoInitializer;

			const TCHAR* test_db = _T("TRIAL@coral2(ETC_S_MURAKAMI_TEST)");

			IWorkspacePtr ws = sindy::create_workspace( test_db );
			if( ! ws )
				Assert::Fail();

			ITablePtr roadTable;
			if(FAILED(IFeatureWorkspacePtr(ws)->OpenTable(CComBSTR(_T("ROAD_LINK")), &roadTable)))
				Assert::Fail();


			IFeaturePtr feature = roadTable->_GetRow(4281646);
			IGeometryPtr geo = feature->GetShape();

			FeatureReader featureReader(roadTable);
			featureReader.readAround( geo, 15.0 );
			auto features = featureReader.getFeatures();

			std::vector<long> oids;
			for( const auto& f : features )
				oids.push_back(f->GetOID());

			Assert::IsTrue( ! oids.empty() );
		}

	};
}