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
#include "CheckInit.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace sindy;
using namespace sindy::schema;
using namespace addr;

namespace Test
{
	TEST_CLASS(JusyoHelperTest)
	{
	public:
		// �e�X�g�N���X����������
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
		}

		// �e�X�g�N���X�I������
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// �ڑ��e�X�g1(�ڑ��L�q�q)
		TEST_METHOD(JusyoHelper_Connect1)
		{
			const _TCHAR* lpcszConnectString = _T("release/release@proteus");
			CJusyoHelper jusyoHelper;
			Assert::IsTrue(jusyoHelper.Connect(lpcszConnectString));

			// �R���X�g���N�^����̐ڑ�
			CJusyoHelper cHelper_tmp( lpcszConnectString );
			Assert::IsTrue(cHelper_tmp.IsConnected());
		}
	
		// �ڑ��e�X�g2(���[�U�E�p�X�E�T�[�r�X�����o�[�W����)
		TEST_METHOD(JusyoHelper_Connect2)
		{
			const _TCHAR* lpcszUser = _T("release");
			const _TCHAR* lpcszPass = _T("release");
			const _TCHAR* lpcszService = _T("proteus");
			CJusyoHelper jusyoHelper;
			Assert::IsTrue(jusyoHelper.Connect(lpcszUser, lpcszPass, lpcszService));

			// �R���X�g���N�^����̐ڑ�
			CJusyoHelper cHelper_tmp(lpcszUser, lpcszPass, lpcszService);
			Assert::IsTrue(cHelper_tmp.IsConnected());
		}

		// �ڑ��e�X�g3(NG�P�[�X)
		TEST_METHOD(JusyoHelper_Connect3)
		{
			CJusyoHelper jusyoHelper;

			// ���NG
			Assert::IsFalse(jusyoHelper.Connect(_T("")));
			Assert::IsFalse(jusyoHelper.IsConnected());
			Assert::IsFalse(jusyoHelper.Connect(_T("/@")));
			Assert::IsFalse(jusyoHelper.IsConnected());

			// ���OK�Őڑ�NG
			Assert::IsFalse(jusyoHelper.Connect(_T("a/b@c")));
			Assert::IsFalse(jusyoHelper.IsConnected());
		}

		// �����e�X�g
		TEST_METHOD(JusyoHelper_Select)
		{
			// �̂̓e�X�g�p�̃��[�U������Ă������悤��(map@delltcp)
			// ���͂Ȃ��̂�release���[�U����f�[�^������Ă���邩�ǂ����̌������s��
			const _TCHAR* lpcszConnectString = _T("release/release@proteus");
			CJusyoHelper jusyoHelper;
			Assert::IsTrue(jusyoHelper.Connect(lpcszConnectString));

			// Areacode_Kana�e�[�u�����當����l������Ă݂�
			CJusyoRecordSet cRecords;
			CString whereClause;
			whereClause += _T("select * ");
			whereClause += _T("from areacode_kana ");
			whereClause += _T("where ken_code = '03' and shi_code = '201' and oaza_code = '000'");
			Assert::IsTrue(jusyoHelper.Select(whereClause, cRecords));

			// ���R�[�h�����͏�L�����Ȃ�1��
			Assert::AreEqual((long)1, cRecords.GetRecordCount());

			// �e�f�[�^�^�̃t�B�[���h����f�[�^������Ă݂�
			Assert::AreEqual(_T("03"), cRecords.GetStringValue((long)0)); // ken_code(char)
			Assert::AreEqual(_T("�C���e�P��"), cRecords.GetStringValue(4)); // ken_kana_d(varchar2)

			// �t�B�[���h���Ŏ���Ă݂�
			Assert::AreEqual(_T("03"), cRecords.GetStringValue(_T("ken_code")));
			Assert::AreEqual(_T("�C���e�P��"), cRecords.GetStringValue(_T("ken_kana_d")));

			// ���R�[�h�I�[�̔���
			cRecords.next();
			Assert::IsTrue(cRecords.end());

			// Address_Point���琔�l������Ă݂�
			whereClause.Empty();
			whereClause += _T("select * ");
			whereClause += _T("from address_point ");
			whereClause += _T("where ken_code = '03' and shi_code = '201' and oaza_code = '000'");
			Assert::IsTrue(jusyoHelper.Select(whereClause, cRecords));
			
			// ���R�[�h�����͏�L�����Ȃ�1��
			Assert::AreEqual((long)1, cRecords.GetRecordCount());

			// ������\�_�ʒu���ς������X�̏������K�X�ύX
			Assert::AreEqual((long)141, cRecords.GetLongValue(12)); // longitude(number)
			Assert::AreEqual((double)39.699249166667, cRecords.GetDoubleValue(13)); // latitude(number)

			Assert::AreEqual((long)141, cRecords.GetLongValue(_T("longitude")));
			Assert::AreEqual((double)39.699249166667, cRecords.GetDoubleValue(_T("latitude")));

			// ���R�[�h�I�[�̔���
			cRecords.next();
			Assert::IsTrue(cRecords.end());
		}
	};
}
