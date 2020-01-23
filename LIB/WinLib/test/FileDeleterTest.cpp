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
 * @brief FileDeleter�̃e�X�g�N���X
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

// �e�X�g�P�[�X

	/// ���݂���t�@�C���̍폜�ɐ�������
	void testExistFile()
	{
		// �t�@�C���̐���
		TCHAR szTempFilePath[MAX_PATH];
		createTempFile(m_szTempPath, szTempFilePath);
		CPPUNIT_ASSERT(::GetFileAttributes(szTempFilePath) != -1);

		// ���s
		CPPUNIT_ASSERT((*m_pFileDeleter)(szTempFilePath));

		// ��m�F
		CPPUNIT_ASSERT_EQUAL(static_cast<DWORD>(-1), ::GetFileAttributes(szTempFilePath));
	}

	/// ���݂��Ȃ��t�@�C���̍폜�Ɏ��s����
	void testNoExistFile()
	{
		// ���݂��Ȃ��t�@�C�����̐���
		TCHAR szTempFilePath[MAX_PATH];
		createTempFile(m_szTempPath, szTempFilePath);
		::DeleteFile(szTempFilePath);
		CPPUNIT_ASSERT_EQUAL(static_cast<DWORD>(-1), ::GetFileAttributes(szTempFilePath));

		// ���s
		CPPUNIT_ASSERT(! (*m_pFileDeleter)(szTempFilePath));
	}

	/// ��t�H���_�̍폜�ɐ�������
	void testEmptyFolder()
	{
		// ��t�H���_�̐���
		TCHAR szTempFolderPath[MAX_PATH];
		createTempFolder(m_szTempPath, szTempFolderPath);
		DWORD dw = ::GetFileAttributes(szTempFolderPath);
		CPPUNIT_ASSERT(dw != -1 && (dw & FILE_ATTRIBUTE_DIRECTORY));

		// ���s
		CPPUNIT_ASSERT((*m_pFileDeleter)(szTempFolderPath));

		// ��m�F
		CPPUNIT_ASSERT_EQUAL(static_cast<DWORD>(-1), ::GetFileAttributes(szTempFolderPath));
	}

	/// ���Ƀt�@�C�������݂���t�H���_�̍폜�ɐ�������
	void testFolder()
	{
		// ���ɂQ�̃t�@�C�������݂���t�H���_�̐���
		TCHAR szFolderPath[MAX_PATH], szBuffer[MAX_PATH];
		createTempFolder(m_szTempPath, szFolderPath);
		createTempFile(szFolderPath, szBuffer);
		createTempFile(szFolderPath, szBuffer);
		CPPUNIT_ASSERT(::GetFileAttributes(szBuffer) != -1);

		// ���s
		CPPUNIT_ASSERT((*m_pFileDeleter)(szFolderPath));

		// ��m�F
		CPPUNIT_ASSERT_EQUAL(static_cast<DWORD>(-1), ::GetFileAttributes(szFolderPath));
	}

	/// ���Ƀt�@�C�������݂���t�H���_�����݂���t�H���_�̍폜�ɐ�������
	void testReflexiveFolder()
	{
		// ���ɂQ�̃t�@�C�������݂���t�H���_�̐���
		TCHAR szFolderPath[MAX_PATH], szBuffer[MAX_PATH];
		createTempFolder(m_szTempPath, szFolderPath);
		createTempFile(szFolderPath, szBuffer);
		createTempFile(szFolderPath, szBuffer);

		// ���̒��ɂ���ɁA�Q�̃t�@�C�������݂���t�H���_�𐶐�
		TCHAR szFolderPath2[MAX_PATH];
		createTempFolder(szFolderPath, szFolderPath2);
		createTempFile(szFolderPath2, szBuffer);
		createTempFile(szFolderPath2, szBuffer);
		CPPUNIT_ASSERT(::GetFileAttributes(szBuffer) != -1);

		// ���s
		CPPUNIT_ASSERT((*m_pFileDeleter)(szFolderPath));

		// ��m�F
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

// �ϐ�
	FileDeleter* m_pFileDeleter;
	TCHAR m_szTempPath[MAX_PATH];
};

CPPUNIT_TEST_SUITE_REGISTRATION(FileDeleterTest);
