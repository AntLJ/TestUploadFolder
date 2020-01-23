#include "stdafx.h"
#include "CppUnitTest.h"

#include <boost/assign/list_of.hpp>

#include <arctl/coinitializer.h>
#include "../ChildVerEditWorkspace.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{		
	TEST_CLASS(SmartWorkspaceTest)
	{
	public:
		
		TEST_METHOD(startEditing01)
		{
			using namespace sindy;
			using namespace boost::assign;

			const arctl::coinitializer aCoInitializer;

			const TCHAR* test_db = _T("TRIAL@coral2(ETC_S_MURAKAMI_TEST)");

			ChildVerEditWorkspace sws;
			Assert::IsTrue(sws.open(test_db));

			// �ҏW�Ώۂ̃e�[�u����o�^����
			const std::vector<CString> edit_tables = list_of( _T("ROAD_LINK") );
			
			// �q�o�[�W�����쐬���[�h
			const CString childDbName = _T("ETC_S_MURAKAMI_TEST_2");
			Assert::IsTrue( sws.childCreateModeOn(childDbName, edit_tables));

			Assert::IsTrue( sws.startEditing() );
			Assert::IsTrue( sws.startEditOperation() );
			Assert::IsTrue( sws.stopEditOperation() );
			Assert::IsTrue( sws.stopEditing(true) );

			// �쐬�����q�o�[�W�����͍폜���Ă���
			const TCHAR* child_db = _T("TRIAL@coral2(") + childDbName + _T(")");
			IWorkspacePtr childWS = sindy::create_workspace(child_db);
			Assert::IsTrue( childWS != nullptr );

			IVersionPtr version(childWS);
			if(FAILED( version->Delete()))
				Assert::Fail();
		}

		TEST_METHOD(startEditing02)
		{
			using namespace sindy;
			using namespace boost::assign;

			const arctl::coinitializer aCoInitializer;

			const TCHAR* test_db = _T("TRIAL@coral2(ETC_S_MURAKAMI_TEST)");

			ChildVerEditWorkspace sws;
			Assert::IsTrue(sws.open(test_db));

			// �ҏW�Ώۂ̃e�[�u����o�^����
			const std::vector<CString> edit_tables = list_of( _T("ROAD_LINK") );
			
			// �q�o�[�W�����쐬���[�h
			const CString childDbName = _T("ETC_S_MURAKAMI_TEST_2");
			Assert::IsTrue( sws.childCreateModeOn(childDbName, edit_tables));

			Assert::IsTrue( sws.startEditing() );
			Assert::IsTrue( sws.startEditOperation() );
			Assert::IsTrue( sws.abortEditOperation() );
			Assert::IsTrue( sws.stopEditing(true) );

			// �쐬�����q�o�[�W�������폜����Ă��邩
			const TCHAR* child_db = _T("TRIAL@coral2(") + childDbName + _T(")");
			IWorkspacePtr childWS = sindy::create_workspace(child_db);
			Assert::IsTrue( childWS == nullptr );
		}

	};
}