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
	// �Z���}�X�^���[�U�����Ȃ��Ȃ�����K�X�ύX���Ă�������
	const _TCHAR* lpcszConnectString = _T("y1005/y1005@delltcp");

	TEST_CLASS(AreaCodeHelperTest)
	{
	private:
		static std::shared_ptr<CAreaCodeHelper> m_spAreaCodeHelper;

	public:
		// �e�X�g�N���X����������
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
			m_spAreaCodeHelper.reset(new CAreaCodeHelper());
			m_spAreaCodeHelper->Connect(lpcszConnectString);
		}

		// �e�X�g�N���X�I������
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// GetNewestCode�e�X�g
		TEST_METHOD(ACHelper_GetNewestCode)
		{
			Assert::AreEqual(_T("03215199101"), m_spAreaCodeHelper->GetNewestCode(_T("03204001000")));
		}
	
		// GetPrevCode�e�X�g
		TEST_METHOD(ACHelper_GetPrevCode)
		{
			Assert::AreEqual(_T("03204001000"), m_spAreaCodeHelper->GetPrevCode(_T("03215199101")));
		}

		// GetCodeStatus�e�X�g
		TEST_METHOD(ACHelper_GetCodeStatus)
		{
			Assert::IsTrue(kShiko == m_spAreaCodeHelper->GetCodeStatus(_T("03215199101")));
			//Assert::IsTrue(kTmpShiko == m_spAreaCodeHelper->GetCodeStatus(_T("33121501000")));
			Assert::IsTrue(kHaishi == m_spAreaCodeHelper->GetCodeStatus(_T("03204001000")));
			//Assert::IsTrue(kTmpHaishi == m_spAreaCodeHelper->GetCodeStatus(_T("33201001000")));
			Assert::IsTrue(kInValid == m_spAreaCodeHelper->GetCodeStatus(_T("00201001000")));
		}

		// GetAddrKanji�e�X�g
		TEST_METHOD(ACHelper_GetAddrKanji)
		{
			Assert::AreEqual(_T("�����s"), m_spAreaCodeHelper->GetAddrKanji(_T("13")));
			Assert::AreEqual(_T("�����s�V�h��"), m_spAreaCodeHelper->GetAddrKanji(_T("131040")));
			Assert::AreEqual(_T("�����s�V�h��l�J"), m_spAreaCodeHelper->GetAddrKanji(_T("13104092")));
			Assert::AreEqual(_T("�����s�V�h��l�J�R����"), m_spAreaCodeHelper->GetAddrKanji(_T("13104092003")));
		}

		// GetAddrKana�e�X�g
		TEST_METHOD(ACHelper_GetAddrKana)
		{
			Assert::AreEqual(_T("�g�E�L���E�g"), m_spAreaCodeHelper->GetAddrKana(_T("13")));
			Assert::AreEqual(_T("�g�E�L���E�g�V���W���N�N"), m_spAreaCodeHelper->GetAddrKana(_T("131040")));
			Assert::AreEqual(_T("�g�E�L���E�g�V���W���N�N���c��"), m_spAreaCodeHelper->GetAddrKana(_T("13104092")));
			Assert::AreEqual(_T("�g�E�L���E�g�V���W���N�N���c���R"), m_spAreaCodeHelper->GetAddrKana(_T("13104092003")));
		}

		// GetZipCode�e�X�g
		TEST_METHOD(ACHelper_GetZipCode)
		{
			// TODO: AreaCodeHelper�Ɏ������ꂽ��e�X�g������
			Assert::IsTrue(true);
		}
	};

	// static�����o�ϐ���`
	std::shared_ptr<CAreaCodeHelper> AreaCodeHelperTest::m_spAreaCodeHelper;
}
