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
 * @brief arctl::pgdb_creator �� @ref unit_test �N���X
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
//@{ @name �e�X�g�P�[�X
	/// �g���q�t���̃t�@�C�����ɑ΂���A�N���X�̓�����m�F����B
	void testClassWithExt()
	{
		test(arctl::pgdb_creator(), _T("_test.mdb"));
	}

	/// �g���q�Ȃ��̃t�@�C�����ɑ΂���A�N���X�̓�����m�F����B
	void testClassNoExt()
	{
		test(arctl::pgdb_creator(), _T("_test"));
	}

	/// �g���q�t���̃t�@�C�����ɑ΂���A�֐��̓�����m�F����B
	void testFunctionWithExt()
	{
		test(&arctl::create_pgdb, _T("_test.mdb"));
	}

	/// �g���q�Ȃ��̃t�@�C�����ɑ΂���A�֐��̓�����m�F����B
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

		// ���΃p�X�̌��؁B
		::_tcscpy(szPath, _T("data/"));
		::_tcscat(szPath, lpszFileName);
		testCreate(f, szPath);

		// ��΃p�X�̌��؁B
		::GetTempPath(_MAX_PATH, szPath);
		::_tcscat(szPath, lpszFileName);
		testCreate(f, szPath);

		// �f�B���N�g�������݂��Ȃ��ꏊ��PGDB���쐬���邱�Ƃ͏o���Ȃ��B
		::_tcscpy(szPath, _T("data/inexist_directory/"));
		::_tcscat(szPath, lpszFileName);
		CPPUNIT_ASSERT_THROW(f(szPath), _com_error);
	}

	template <typename TFunction>
	void testCreate(const TFunction& f, LPCTSTR lpszPath)
	{
		// �S�~���c���Ă�����폜����B
		::DeleteFile(lpszPath);

		// PGDB�̍쐬�ɐ�������B
		CPPUNIT_ASSERT(f(lpszPath) != 0);

		// �t�@�C���̑��݊m�F���s���B
		TCHAR szExistPath[_MAX_PATH] = { 0 };
		::_tcscpy(szExistPath, lpszPath);
		LPCTSTR p = ::_tcsrchr(lpszPath, _T('.'));
		if(! p || ::_tcsicmp(p + 1, _T("mdb")) != 0) {
			::_tcscat(szExistPath, _T(".mdb"));
		}
		CPPUNIT_ASSERT(::_taccess(szExistPath, 06) == 0);

		// ���ɑ��݂���PGDB�͍쐬�ł��Ȃ��B
		CPPUNIT_ASSERT_THROW(f(lpszPath), _com_error);

		// �쐬����PGDB���폜����B
		CPPUNIT_ASSERT(::DeleteFile(szExistPath));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(PGDBCreatorTest);
