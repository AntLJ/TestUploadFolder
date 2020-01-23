#include "stdafx.h"
#include "CppUnitTest.h"

#include <boost/assign/list_of.hpp>

#include <arctl/coinitializer.h>
#include "../RoadLinkMatcher.h"

#include <SiNDYLib/FieldMap.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{		
	TEST_CLASS(LinkMatcherTest)
	{
	public:
		TEST_METHOD(getMatchLinkTest)
		{
			using namespace sindy;
			using namespace sindy::schema;
			using namespace boost::assign;

			const arctl::coinitializer aCoInitializer;

			//const TCHAR* adam_db = _T("..\\testdata\\ADAM2_車線NWテスト.gdb");
			const TCHAR* road_db = _T("..\\unittest\\unittest_data\\unittest_data.mdb");
			IWorkspacePtr ws = sindy::create_workspace( road_db );
			if( ! ws )
				Assert::Fail();

			ITablePtr laneTable;
			if(FAILED(IFeatureWorkspacePtr(ws)->OpenTable(CComBSTR(_T("LANE_LINK")), &laneTable)))
				Assert::Fail();

		
			ITablePtr roadTable;
			if(FAILED(IFeatureWorkspacePtr(ws)->OpenTable(CComBSTR(_T("ROAD_LINK")), &roadTable)))
				Assert::Fail();


			// 規制なし道路リンクと正しくマッチングされるか
			// マッチング元のフィーチャ取得
			IFeaturePtr srcFeature = laneTable->_GetRow(2);

			// マッチング先のフィーチャ群
			std::vector<IFeaturePtr> destFeatures;
			destFeatures.push_back( roadTable->_GetRow(1));
			destFeatures.push_back( roadTable->_GetRow(2));
			destFeatures.push_back( roadTable->_GetRow(4));

			RoadLinkMatcher linkMatcher(roadTable, 30);
			bool isReverse(false);


			IFeaturePtr matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse);
			Assert::IsTrue( matchFeature->GetOID() == 2 );
			Assert::IsFalse( isReverse );

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, true);
			Assert::IsTrue( matchFeature->GetOID() == 4 );
			Assert::IsTrue( isReverse );


			destFeatures.clear();
			srcFeature = laneTable->_GetRow(3);
			destFeatures.push_back( roadTable->_GetRow(5));
			destFeatures.push_back( roadTable->_GetRow(7));

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse);
			Assert::IsTrue( matchFeature->GetOID() == 5 );
			Assert::IsTrue( isReverse );

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, true);
			Assert::IsTrue( matchFeature->GetOID() == 7 );
			Assert::IsFalse( isReverse );


			destFeatures.clear();
			srcFeature = laneTable->_GetRow(4);
			destFeatures.push_back( roadTable->_GetRow(8));
			destFeatures.push_back( roadTable->_GetRow(9));

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, false);
			Assert::IsTrue( matchFeature->GetOID() == 8 );
			Assert::IsTrue( isReverse );

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, true);
			Assert::IsTrue( matchFeature->GetOID() == 9 );
			Assert::IsFalse( isReverse );


			destFeatures.clear();
			srcFeature = laneTable->_GetRow(5);
			destFeatures.push_back( roadTable->_GetRow(11));
			destFeatures.push_back( roadTable->_GetRow(12));

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, false);
			Assert::IsTrue( matchFeature->GetOID() == 11 );
			Assert::IsFalse( isReverse );

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, true);
			Assert::IsTrue( matchFeature->GetOID() == 12 );
			Assert::IsFalse( isReverse );


			destFeatures.clear();
			srcFeature = laneTable->_GetRow(6);
			destFeatures.push_back( roadTable->_GetRow(13));
			destFeatures.push_back( roadTable->_GetRow(15));

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, false);
			Assert::IsTrue( matchFeature->GetOID() == 13 );
			Assert::IsTrue( isReverse );

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, true);
			Assert::IsTrue( matchFeature->GetOID() == 15 );
			Assert::IsTrue( isReverse );


			destFeatures.clear();
			srcFeature = laneTable->_GetRow(8);
			destFeatures.push_back( roadTable->_GetRow(16));
			destFeatures.push_back( roadTable->_GetRow(17));

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, false);
			Assert::IsTrue( matchFeature->GetOID() == 17 );
			Assert::IsTrue( isReverse );

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, true);
			Assert::IsTrue( matchFeature->GetOID() == 16 );
			Assert::IsTrue( isReverse );


			destFeatures.clear();
			srcFeature = laneTable->_GetRow(9);
			destFeatures.push_back( roadTable->_GetRow(18));
			destFeatures.push_back( roadTable->_GetRow(19));

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, false);
			Assert::IsTrue( matchFeature->GetOID() == 18 );
			Assert::IsTrue( isReverse );

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, true);
			Assert::IsTrue( matchFeature->GetOID() == 19 );
			Assert::IsFalse( isReverse );


			destFeatures.clear();
			srcFeature = laneTable->_GetRow(10);
			destFeatures.push_back( roadTable->_GetRow(20));
			destFeatures.push_back( roadTable->_GetRow(21));

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, false);
			Assert::IsTrue( matchFeature->GetOID() == 21 );
			Assert::IsTrue( isReverse );

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, true);
			Assert::IsTrue( matchFeature->GetOID() == 20 );
			Assert::IsTrue( isReverse );


			destFeatures.clear();
			srcFeature = laneTable->_GetRow(12);
			destFeatures.push_back( roadTable->_GetRow(24));
			destFeatures.push_back( roadTable->_GetRow(25));

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, false);
			Assert::IsTrue( matchFeature->GetOID() == 24 );
			Assert::IsFalse( isReverse );

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, true);
			Assert::IsTrue( matchFeature == nullptr );


			destFeatures.clear();
			srcFeature = laneTable->_GetRow(13);
			destFeatures.push_back( roadTable->_GetRow(26));
			destFeatures.push_back( roadTable->_GetRow(27));

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, false);
			Assert::IsTrue( matchFeature->GetOID() == 26 );
			Assert::IsTrue( isReverse );

			matchFeature = linkMatcher.getMatchLink(srcFeature, destFeatures, isReverse, true);
			Assert::IsTrue( matchFeature->GetOID() == 26 );
			Assert::IsFalse( isReverse );
		}
	};
}