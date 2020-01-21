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
 * @file PGDBCreatorTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <io.h>
#include <arctl/pgdb.h>

/**
 * @ingroup unit_test
 * @brief arctl::pgdb_creator の @ref unit_test クラス
 */
class PGDBCreatorTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(PGDBCreatorTest);
	CPPUNIT_TEST(testClassWithExt);
	CPPUNIT_TEST(testClassNoExt);
	CPPUNIT_TEST(testFunctionWithExt);
	CPPUNIT_TEST(testFunctionNoExt);
	CPPUNIT_TEST_SUITE_END();

public:
//@{ @name テストケース
	/// 拡張子付きのファイル名に対する、クラスの動作を確認する。
	void testClassWithExt()
	{
		test(arctl::pgdb_creator(), _T("_test.mdb"));
	}

	/// 拡張子なしのファイル名に対する、クラスの動作を確認する。
	void testClassNoExt()
	{
		test(arctl::pgdb_creator(), _T("_test"));
	}

	/// 拡張子付きのファイル名に対する、関数の動作を確認する。
	void testFunctionWithExt()
	{
		test(&arctl::create_pgdb, _T("_test.mdb"));
	}

	/// 拡張子なしのファイル名に対する、関数の動作を確認する。
	void testFunctionNoExt()
	{
		test(&arctl::create_pgdb, _T("_test"));
	}
//@}

private:
	template <typename TFunction>
	void test(TFunction f, LPCTSTR lpszFileName)
	{
		TCHAR szPath[_MAX_PATH];

		// 相対パスの検証。
		::_tcscpy(szPath, _T("data/"));
		::_tcscat(szPath, lpszFileName);
		testCreate(f, szPath);

		// 絶対パスの検証。
		::GetTempPath(_MAX_PATH, szPath);
		::_tcscat(szPath, lpszFileName);
		testCreate(f, szPath);

		// ディレクトリが存在しない場所にPGDBを作成することは出来ない。
		::_tcscpy(szPath, _T("data/inexist_directory/"));
		::_tcscat(szPath, lpszFileName);
		CPPUNIT_ASSERT_THROW(f(szPath), _com_error);
	}

	template <typename TFunction>
	void testCreate(const TFunction& f, LPCTSTR lpszPath)
	{
		// ゴミが残っていたら削除する。
		::DeleteFile(lpszPath);

		// PGDBの作成に成功する。
		CPPUNIT_ASSERT(f(lpszPath) != 0);

		// ファイルの存在確認を行う。
		TCHAR szExistPath[_MAX_PATH] = { 0 };
		::_tcscpy(szExistPath, lpszPath);
		LPCTSTR p = ::_tcsrchr(lpszPath, _T('.'));
		if(! p || ::_tcsicmp(p + 1, _T("mdb")) != 0) {
			::_tcscat(szExistPath, _T(".mdb"));
		}
		CPPUNIT_ASSERT(::_taccess(szExistPath, 06) == 0);

		// 既に存在するPGDBは作成できない。
		CPPUNIT_ASSERT_THROW(f(lpszPath), _com_error);

		// 作成したPGDBを削除する。
		CPPUNIT_ASSERT(::DeleteFile(szExistPath));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(PGDBCreatorTest);
