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
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "../FileDeleter.h"

/**
 * @brief FileDeleterのテストクラス
 */
class FileDeleterTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(FileDeleterTest);
	CPPUNIT_TEST(testExistFile);
	CPPUNIT_TEST(testNoExistFile);
	CPPUNIT_TEST(testEmptyFolder);
	CPPUNIT_TEST(testFolder);
	CPPUNIT_TEST(testReflexiveFolder);
	CPPUNIT_TEST_SUITE_END();

public:
	FileDeleterTest() :
	m_pFileDeleter(0)
	{
		::GetTempPath(MAX_PATH, m_szTempPath);
	}

	virtual void setUp()
	{ 
		m_pFileDeleter = new FileDeleter;
	}

	virtual void tearDown()
	{
		delete m_pFileDeleter;
		m_pFileDeleter = 0;
	}

// テストケース

	/// 存在するファイルの削除に成功する
	void testExistFile()
	{
		// ファイルの生成
		TCHAR szTempFilePath[MAX_PATH];
		createTempFile(m_szTempPath, szTempFilePath);
		CPPUNIT_ASSERT(::GetFileAttributes(szTempFilePath) != -1);

		// 試行
		CPPUNIT_ASSERT((*m_pFileDeleter)(szTempFilePath));

		// 後確認
		CPPUNIT_ASSERT_EQUAL(static_cast<DWORD>(-1), ::GetFileAttributes(szTempFilePath));
	}

	/// 存在しないファイルの削除に失敗する
	void testNoExistFile()
	{
		// 存在しないファイル名の生成
		TCHAR szTempFilePath[MAX_PATH];
		createTempFile(m_szTempPath, szTempFilePath);
		::DeleteFile(szTempFilePath);
		CPPUNIT_ASSERT_EQUAL(static_cast<DWORD>(-1), ::GetFileAttributes(szTempFilePath));

		// 試行
		CPPUNIT_ASSERT(! (*m_pFileDeleter)(szTempFilePath));
	}

	/// 空フォルダの削除に成功する
	void testEmptyFolder()
	{
		// 空フォルダの生成
		TCHAR szTempFolderPath[MAX_PATH];
		createTempFolder(m_szTempPath, szTempFolderPath);
		DWORD dw = ::GetFileAttributes(szTempFolderPath);
		CPPUNIT_ASSERT(dw != -1 && (dw & FILE_ATTRIBUTE_DIRECTORY));

		// 試行
		CPPUNIT_ASSERT((*m_pFileDeleter)(szTempFolderPath));

		// 後確認
		CPPUNIT_ASSERT_EQUAL(static_cast<DWORD>(-1), ::GetFileAttributes(szTempFolderPath));
	}

	/// 中にファイルが存在するフォルダの削除に成功する
	void testFolder()
	{
		// 中に２つのファイルが存在するフォルダの生成
		TCHAR szFolderPath[MAX_PATH], szBuffer[MAX_PATH];
		createTempFolder(m_szTempPath, szFolderPath);
		createTempFile(szFolderPath, szBuffer);
		createTempFile(szFolderPath, szBuffer);
		CPPUNIT_ASSERT(::GetFileAttributes(szBuffer) != -1);

		// 試行
		CPPUNIT_ASSERT((*m_pFileDeleter)(szFolderPath));

		// 後確認
		CPPUNIT_ASSERT_EQUAL(static_cast<DWORD>(-1), ::GetFileAttributes(szFolderPath));
	}

	/// 中にファイルが存在するフォルダが存在するフォルダの削除に成功する
	void testReflexiveFolder()
	{
		// 中に２つのファイルが存在するフォルダの生成
		TCHAR szFolderPath[MAX_PATH], szBuffer[MAX_PATH];
		createTempFolder(m_szTempPath, szFolderPath);
		createTempFile(szFolderPath, szBuffer);
		createTempFile(szFolderPath, szBuffer);

		// その中にさらに、２つのファイルが存在するフォルダを生成
		TCHAR szFolderPath2[MAX_PATH];
		createTempFolder(szFolderPath, szFolderPath2);
		createTempFile(szFolderPath2, szBuffer);
		createTempFile(szFolderPath2, szBuffer);
		CPPUNIT_ASSERT(::GetFileAttributes(szBuffer) != -1);

		// 試行
		CPPUNIT_ASSERT((*m_pFileDeleter)(szFolderPath));

		// 後確認
		CPPUNIT_ASSERT_EQUAL(static_cast<DWORD>(-1), ::GetFileAttributes(szFolderPath));
	}

private:
	void createTempFile(LPCTSTR lpszDirectoryPath, LPTSTR lpszTempFilePath)
	{
		::GetTempFileName(lpszDirectoryPath, _T("tst"), 0, lpszTempFilePath);
	}

	void createTempFolder(LPCTSTR lpszDirectoryPath, LPTSTR lpszTempFolderPath)
	{
		createTempFile(lpszDirectoryPath, lpszTempFolderPath);
		::DeleteFile(lpszTempFolderPath);
		::CreateDirectory(lpszTempFolderPath, 0);
	}

// 変数
	FileDeleter* m_pFileDeleter;
	TCHAR m_szTempPath[MAX_PATH];
};

CPPUNIT_TEST_SUITE_REGISTRATION(FileDeleterTest);
