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
	TEST_CLASS(AddrCodeListTest)
	{
	private:
		// CAddrCodeList��static�����o�Ŏ��ƁA�ʂ��Ń`�F�b�N�����Ƃ��ɃG���[�ɂȂ�̂�
		// CWorkspace�������o�Ŏ����ACAddrCodeList�͓s�x��������悤�ɂ���
		static std::shared_ptr<CWorkspace> m_spWorkspace;

	public:
		// �e�X�g�N���X����������
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
			m_spWorkspace.reset(new CWorkspace(verificationDatDir + _T("53392442.mdb")));
			// PGDB���[�h��ON�ɂ��Ă���
			_tputenv_s(_T("PGDB_MODE"), _T("ON"));
		}

		// �e�X�g�N���X�I������
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
			// �O�̂���PGDB���[�h��OFF�ɂ��Ă���
			_tputenv_s(_T("PGDB_MODE"), _T("OFF"));
		}
		
		// �S������Z���R�[�h���X�g�����e�X�g
		TEST_METHOD(CodeList_MakeCodeListAll)
		{
			CAddrCodeList addrCodeList(m_spWorkspace->OpenTable((city_admin::kTableName)));

			// �s���{���R�[�h�擾�ł��邩�m�F
			Assert::IsTrue(addrCodeList.MakeCodeListAll(2));
			Assert::AreEqual((size_t)3, addrCodeList.size());
		
			CAddrCodeList::const_iterator itr1 = addrCodeList.begin();
			Assert::AreEqual(_T("01"), *itr1); ++itr1;
			Assert::AreEqual(_T("02"), *itr1); ++itr1;
			Assert::AreEqual(_T("03"), *itr1);
			addrCodeList.clear();

			// �s�撬���R�[�h�擾�ł��邩�m�F
			Assert::IsTrue(addrCodeList.MakeCodeListAll(5));
			Assert::AreEqual((size_t)3, addrCodeList.size());
		
			CAddrCodeList::const_iterator itr2 = addrCodeList.begin();
			Assert::AreEqual(_T("01101"), *itr2); ++itr2;
			Assert::AreEqual(_T("02201"), *itr2); ++itr2;
			Assert::AreEqual(_T("03201"), *itr2);
			addrCodeList.clear();

			// �厚�R�[�h�擾�ł��邩�m�F
			Assert::IsTrue(addrCodeList.MakeCodeListAll(8));
			Assert::AreEqual((size_t)4, addrCodeList.size());
		
			CAddrCodeList::const_iterator itr3 = addrCodeList.begin();
			Assert::AreEqual(_T("01101001"), *itr3); ++itr3;
			Assert::AreEqual(_T("01101002"), *itr3); ++itr3;
			Assert::AreEqual(_T("02201001"), *itr3); ++itr3;
			Assert::AreEqual(_T("03201001"), *itr3);
			addrCodeList.clear();

			// ���R�[�h�擾�ł��邩�m�F
			Assert::IsTrue(addrCodeList.MakeCodeListAll(11));
			Assert::AreEqual( (size_t)4, addrCodeList.size() );
		
			CAddrCodeList::const_iterator itr4 = addrCodeList.begin();
			Assert::AreEqual(_T("01101001001"), *itr4); ++itr4;
			Assert::AreEqual(_T("01101002001"), *itr4); ++itr4;
			Assert::AreEqual(_T("02201001002"), *itr4); ++itr4;
			Assert::AreEqual(_T("03201001003"), *itr4);
			addrCodeList.clear();
		}

		// �R�[�h�͈͂���Z���R�[�h���X�g�����e�X�g
		TEST_METHOD(CodeList_MakeCodeListFromRange)
		{
			CAddrCodeList addrCodeList(m_spWorkspace->OpenTable((city_admin::kTableName)));

			// �s���{���R�[�h�擾�ł��邩�m�F
			Assert::IsTrue(addrCodeList.MakeCodeListFromRange(_T("01"), _T("03")));
			Assert::AreEqual((size_t)3, addrCodeList.size());

			CAddrCodeList::const_iterator itr1 = addrCodeList.begin();
			Assert::AreEqual(_T("01"), *itr1); ++itr1;
			Assert::AreEqual(_T("02"), *itr1); ++itr1;
			Assert::AreEqual(_T("03"), *itr1);
			addrCodeList.clear();

			// �s�撬���R�[�h�擾�ł��邩�m�F
			Assert::IsTrue(addrCodeList.MakeCodeListFromRange(_T("01000"), _T("03000")));
			Assert::AreEqual((size_t)2, addrCodeList.size());

			CAddrCodeList::const_iterator itr2 = addrCodeList.begin();
			Assert::AreEqual(_T("01101"), *itr2); ++itr2;
			Assert::AreEqual(_T("02201"), *itr2);
			addrCodeList.clear();

			// �厚�R�[�h�擾�ł��邩�m�F
			Assert::IsTrue(addrCodeList.MakeCodeListFromRange(_T("01101002"), _T("04000000")));
			Assert::AreEqual((size_t)3, addrCodeList.size());

			CAddrCodeList::const_iterator itr3 = addrCodeList.begin();
			Assert::AreEqual(_T("01101002"), *itr3); ++itr3;
			Assert::AreEqual(_T("02201001"), *itr3); ++itr3;
			Assert::AreEqual(_T("03201001"), *itr3);
			addrCodeList.clear();

			// ���R�[�h�擾�ł��邩�m�F
			Assert::IsTrue(addrCodeList.MakeCodeListFromRange(_T("01102001000"), _T("03000000000")));
			Assert::AreEqual((size_t)1, addrCodeList.size());

			CAddrCodeList::const_iterator itr4 = addrCodeList.begin();
			Assert::AreEqual(_T("02201001002"), *itr4);
			addrCodeList.clear();
		}

		// �R�[�h���X�g�t�@�C������R�[�h���X�g�����e�X�g
		TEST_METHOD(CodeList_AddFromFile)
		{
			CAddrCodeList addrCodeList(m_spWorkspace->OpenTable((city_admin::kTableName)));

			// �s���{���R�[�h�擾�ł��邩�m�F
			CString codeListPath = verificationDatDir + _T("codelist.txt");
			Assert::IsTrue(addrCodeList.AddFromFile(codeListPath, 2));
			Assert::AreEqual((size_t)4, addrCodeList.size());
		
			CAddrCodeList::const_iterator itr1 = addrCodeList.begin();
			Assert::AreEqual(_T("01"), *itr1); ++itr1;
			Assert::AreEqual(_T("02"), *itr1); ++itr1;
			Assert::AreEqual(_T("03"), *itr1); ++itr1;
			Assert::AreEqual(_T("11"), *itr1);
			addrCodeList.clear();

			// �s�撬���R�[�h�擾�ł��邩�m�F
			Assert::IsTrue(addrCodeList.AddFromFile(codeListPath, 5));
			Assert::AreEqual((size_t)4, addrCodeList.size());
		
			CAddrCodeList::const_iterator itr2 = addrCodeList.begin();
			Assert::AreEqual(_T("01101"), *itr2); ++itr2;
			Assert::AreEqual(_T("02101"), *itr2); ++itr2;
			Assert::AreEqual(_T("03301"), *itr2); ++itr2;
			Assert::AreEqual(_T("11111"), *itr2);
			addrCodeList.clear();

			// �厚�R�[�h�擾�ł��邩�m�F
			Assert::IsTrue(addrCodeList.AddFromFile(codeListPath, 8));
			Assert::AreEqual((size_t)5, addrCodeList.size());
		
			CAddrCodeList::const_iterator itr3 = addrCodeList.begin();
			Assert::AreEqual(_T("01101001"), *itr3); ++itr3;
			Assert::AreEqual(_T("01101003"), *itr3); ++itr3;
			Assert::AreEqual(_T("02101001"), *itr3); ++itr3;
			Assert::AreEqual(_T("03301001"), *itr3); ++itr3;
			Assert::AreEqual(_T("11111111"), *itr3);
			addrCodeList.clear();

			// ���R�[�h�擾�ł��邩�m�F
			Assert::IsTrue(addrCodeList.AddFromFile(codeListPath, 11));
			Assert::AreEqual((size_t)6, addrCodeList.size());
		
			CAddrCodeList::const_iterator itr4 = addrCodeList.begin();
			Assert::AreEqual(_T("01101001001"), *itr4); ++itr4;
			Assert::AreEqual(_T("01101003001"), *itr4); ++itr4;
			Assert::AreEqual(_T("02101001100"), *itr4); ++itr4;
			Assert::AreEqual(_T("03301001100"), *itr4); ++itr4;
			Assert::AreEqual(_T("11111111111"), *itr4); ++itr4;
			Assert::AreEqual(_T("11111111222"), *itr4);

			addrCodeList.clear();

			// �Ō�ɓǂݍ��ݎ��s�����������m���邩�m�F
			Assert::IsFalse(addrCodeList.AddFromFile(_T("unknown.txt")));
		}

		// �P�ƃR�[�h�ǉ��ɂ��R�[�h���X�g�����e�X�g
		TEST_METHOD(CodeList_Add)
		{
			CAddrCodeList addrCodeList(m_spWorkspace->OpenTable((city_admin::kTableName)));

			addrCodeList.Add( _T("02101") );
			addrCodeList.Add( _T("01101") );
			addrCodeList.Add( _T("03101") );
			addrCodeList.Add( _T("01101") );

			Assert::AreEqual((size_t)3, addrCodeList.size());
		
			CAddrCodeList::const_iterator itr1 = addrCodeList.begin();
			Assert::AreEqual(_T("01101"), *itr1); ++itr1;
			Assert::AreEqual(_T("02101"), *itr1); ++itr1;
			Assert::AreEqual(_T("03101"), *itr1);
		}
	};

	// static�����o�ϐ���`
	std::shared_ptr<CWorkspace> AddrCodeListTest::m_spWorkspace;
}
