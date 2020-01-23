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
#include <arctl/pgdb.h>
#include <sindy/workspace.h>

inline std::ostream& operator<<(std::ostream& os, const std::wstring& rhs)
{
	return os << CW2A(rhs.c_str()).m_psz;
}

namespace {

const _TCHAR s_szAtStyleTestServer[] = _T("sindytest@galatea(SDE.DEFAULT)");
const _TCHAR s_szTestServerSDEFilePath[] = _T("data/testserver.sde");
const _TCHAR s_szTempPGDBPath[] = _T("data/temp.mdb");
const _TCHAR s_szPostGISProperties[] = _T("host=sinup03-k34690;port=5432;user=sinup2007a;password=sinup2007a;dbname=base2007a");
const _TCHAR s_szTestServerPGSFilePath[] = _T("data/testserver.pgs");

#pragma warning(push)
#pragma warning(disable : 6011)
/**
 * @brief 開けることを確認する。
 *
 * @param ipName [in] ワークスペース名。
 */
IWorkspacePtr check_open(IName* ipName)
{
	CPPUNIT_ASSERT(ipName);

	IUnknownPtr ipUnk;
	CPPUNIT_ASSERT_EQUAL(S_OK, ipName->Open(&ipUnk));

	const IFeatureWorkspacePtr ipFeatureWorkspace(ipUnk);
	CPPUNIT_ASSERT(ipFeatureWorkspace);

	return ipFeatureWorkspace;
}
#pragma warning(pop)


} // anonymous namespace

class CreateWorskpaceNameTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(CreateWorskpaceNameTest);
	CPPUNIT_TEST(testAtStyle);
	CPPUNIT_TEST(testAtStyleFail1);
	CPPUNIT_TEST(testAtStyleFail2);
	CPPUNIT_TEST(testSlashStyle);
	CPPUNIT_TEST(testSlashStyleFail);
	CPPUNIT_TEST(testSlashStyleFail2);
	CPPUNIT_TEST(testSDEFile);
	CPPUNIT_TEST(testShapeFileFolder);
	CPPUNIT_TEST(testPGDB);
	CPPUNIT_TEST(testPostGISByString);
	CPPUNIT_TEST(testPGSFile);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name テストケース
//@{
	void testAtStyle()
	{
		using namespace sindy;

		CPPUNIT_ASSERT_EQUAL(
			std::basic_string<TCHAR>(_T("sindytest@galatea(SDE.DEFAULT)")),
			workspace_address(check_open(INamePtr(create_workspace_name(s_szAtStyleTestServer))))
		);
	}

	void testAtStyleFail1()
	{
		const INamePtr ipName(sindy::create_workspace_name(_T("sindytest@galatea")));
		CPPUNIT_ASSERT(ipName == 0);
	}

	void testAtStyleFail2()
	{
		const INamePtr ipName(sindy::create_workspace_name(_T("sindytest@galatea()")));
		CPPUNIT_ASSERT(ipName == 0);
	}

	void testSlashStyle()
	{
		using namespace sindy;

		CPPUNIT_ASSERT_EQUAL(
			std::basic_string<TCHAR>(_T("sindytest@galatea(SDE.DEFAULT)")),
			workspace_address(check_open(INamePtr(create_workspace_name(_T("sindytest/sindytest/SDE.DEFAULT/5151/galatea")))))
		);
	}

	void testSlashStyleFail()
	{
		const INamePtr ipName(sindy::create_workspace_name(_T("sindytest/sindytest/user2007/06/28 13:12:00/5151/galatea")));
		CPPUNIT_ASSERT(ipName == 0);
	}

	void testSlashStyleFail2()
	{
		const IWorkspaceNamePtr ipName(sindy::create_workspace_name(_T("../test/../test/data")));
		CPPUNIT_ASSERT(ipName != 0);
		CPPUNIT_ASSERT_EQUAL(
			std::string("esriDataSourcesFile.ShapefileWorkspaceFactory.1"),
			std::string(ipName->WorkspaceFactoryProgID)
		);
	}

	void testSDEFile()
	{
		using namespace sindy;

		CPPUNIT_ASSERT_EQUAL(
			std::basic_string<TCHAR>(_T("sindytest@galatea(SDE.DEFAULT)")),
			workspace_address(check_open(INamePtr(create_workspace_name(s_szTestServerSDEFilePath))))
		);
	}

	void testShapeFileFolder()
	{
		using namespace sindy;

		TCHAR szBuff[_MAX_PATH];
		CPPUNIT_ASSERT(::_tfullpath(szBuff, _T("data"), _MAX_PATH));

		CPPUNIT_ASSERT_EQUAL(
			std::basic_string<TCHAR>(szBuff),
			workspace_address(check_open(INamePtr(create_workspace_name(_T("data")))))
		);
	}

	void testPGDB()
	{
		using namespace sindy;

		TCHAR szBuff[_MAX_PATH];
		CPPUNIT_ASSERT(::_tfullpath(szBuff, s_szTempPGDBPath, _MAX_PATH));

		::DeleteFile(s_szTempPGDBPath);
		CPPUNIT_ASSERT(arctl::create_pgdb(s_szTempPGDBPath));
		CPPUNIT_ASSERT_EQUAL(
			std::basic_string<TCHAR>(szBuff),
			workspace_address(check_open(INamePtr(create_workspace_name(s_szTempPGDBPath))))
		);
		CPPUNIT_ASSERT(::DeleteFile(s_szTempPGDBPath));
	}

	void testPostGISByString()
	{
		using namespace sindy;

		CPPUNIT_ASSERT_EQUAL(
			std::basic_string<TCHAR>(_T("sinup2007a@sinup03-k34690(PostGIS.base2007a)")),
			workspace_address(check_open(INamePtr(create_workspace_name(s_szPostGISProperties))))
		);
	}

	void testPGSFile()
	{
		using namespace sindy;

		CPPUNIT_ASSERT_EQUAL(
			std::basic_string<TCHAR>(_T("sinup2007a@sinup03-k34690(PostGIS.base2007a)")),
			workspace_address(check_open(INamePtr(create_workspace_name(s_szTestServerPGSFilePath))))
		);
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(CreateWorskpaceNameTest);
