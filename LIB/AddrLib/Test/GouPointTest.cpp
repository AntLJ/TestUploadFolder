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
	TEST_CLASS(GouPointTest)
	{
	private:
		static std::shared_ptr<CFeatureClass> m_spFcGouPgdb; // ���ؗpPGDB��Gou
		static std::shared_ptr<CFeatureClass> m_spFcGouSde; // SDE��Gou

	public:
		// �e�X�g�N���X����������
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();

			CWorkspace cWorkspacePgdb(CString(verificationDatDir + _T("53392442.mdb")));
			m_spFcGouPgdb.reset(new CFeatureClass(cWorkspacePgdb.OpenTable(gou_point::kTableName)));

			CWorkspace cWorkspaceSde(_T("ronly/ronly/SDE.DEFAULT/5151/plum"));
			m_spFcGouSde.reset(new CFeatureClass(cWorkspaceSde.OpenTable(_T("FIX201605.gou_point"))));
		}

		// �e�X�g�N���X�I������
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// ���ԍ��擾�e�X�g
		TEST_METHOD(Gou_GetGouNo)
		{
			CGouPoint cGou1(m_spFcGouPgdb->GetFeature(1));
			CGouPoint cGou2(m_spFcGouPgdb->GetFeature(2));
			CGouPoint cGou3(m_spFcGouPgdb->GetFeature(3));

			// ���ԍ��擾
			Assert::AreEqual(_T("10"), cGou1.GetGouNo());
			Assert::AreEqual(_T("11-1"), cGou2.GetGouNo());
			Assert::AreEqual(_T("12-2-3"), cGou3.GetGouNo());

			// ���ԍ��g�[�N���؂�o��
			Assert::AreEqual(_T("11"), cGou2.GetGouNo(0, 1));
			Assert::AreEqual(_T("1"), cGou2.GetGouNo(1));
			Assert::AreEqual(_T("12-2"), cGou3.GetGouNo(0, 2));
			Assert::AreEqual(_T("2"), cGou3.GetGouNo(1, 2));
			Assert::AreEqual(_T("2-3"), cGou3.GetGouNo(1));
		}

		// ���ԍ��g�[�N�����e�X�g
		TEST_METHOD(Gou_GetTokenCount)
		{
			Assert::AreEqual((long)1, CGouPoint(m_spFcGouPgdb->GetFeature(1)).GetTokenCount(), _T("TestGetTokenCount1 : ���s"));
			Assert::AreEqual((long)2, CGouPoint(m_spFcGouPgdb->GetFeature(2)).GetTokenCount(), _T("TestGetTokenCount2 : ���s"));
			Assert::AreEqual((long)3, CGouPoint(m_spFcGouPgdb->GetFeature(3)).GetTokenCount(), _T("TestGetTokenCount3 : ���s"));
		}

		// ���^�C�v�擾�e�X�g
		TEST_METHOD(Gou_GetGouType)
		{
			Assert::AreEqual((long)1, CGouPoint(m_spFcGouPgdb->GetFeature(1)).GetGouType() );
			Assert::AreEqual((long)2, CGouPoint(m_spFcGouPgdb->GetFeature(2)).GetGouType() );
		}

		// �D��t���O�擾�e�X�g
		TEST_METHOD(Gou_IsPrior)
		{
			Assert::AreEqual(false, CGouPoint(m_spFcGouPgdb->GetFeature(1)).IsPrior());
			Assert::AreEqual(true, CGouPoint(m_spFcGouPgdb->GetFeature(2)).IsPrior());
		}

		// �g�����ԍ��t���O�擾�e�X�g
		TEST_METHOD(Gou_IsExGouNo)
		{
			Assert::AreEqual(false, CGouPoint(m_spFcGouPgdb->GetFeature(1)).IsExGouNo());
			Assert::AreEqual(true, CGouPoint(m_spFcGouPgdb->GetFeature(3)).IsExGouNo());
		}

		// 20���R�[�h�擾
		TEST_METHOD(Gou_GetAddrCode)
		{
			// 20���R�[�h(�����l������)
			Assert::AreEqual(_T("01101001001000010010"), CGouPoint(m_spFcGouPgdb->GetFeature(1)).GetAddrCode());
			Assert::AreEqual(_T("01101001001000110001"), CGouPoint(m_spFcGouPgdb->GetFeature(2)).GetAddrCode());
			Assert::AreEqual(_T("01101001001000120002"), CGouPoint(m_spFcGouPgdb->GetFeature(3)).GetAddrCode());
			// [�`1-1] -> 1-1�Ɖ���
			Assert::AreEqual(_T("01101002001000010001"), CGouPoint(m_spFcGouPgdb->GetFeature(12)).GetAddrCode());
			// [�`-1]  -> �ϊ�����Ȃ�
			Assert::IsTrue(CGouPoint(m_spFcGouPgdb->GetFeature(13)).GetAddrCode().IsEmpty());

			// 20���R�[�h(�����l��)
			Assert::AreEqual(_T("01101001001A00000001"), CGouPoint(m_spFcGouPgdb->GetFeature(6)).GetAddrCode(true));
			Assert::AreEqual(_T("01101001001000050005"), CGouPoint(m_spFcGouPgdb->GetFeature(7)).GetAddrCode(true));
			Assert::AreEqual(_T("0110100100100005A000"), CGouPoint(m_spFcGouPgdb->GetFeature(8)).GetAddrCode(true));
			Assert::AreEqual(_T("0110100100100005B001"), CGouPoint(m_spFcGouPgdb->GetFeature(9)).GetAddrCode(true));
			Assert::AreEqual(_T("0110100100100005A001"), CGouPoint(m_spFcGouPgdb->GetFeature(10)).GetAddrCode(true));
			Assert::AreEqual(_T("0110100100100012A002"), CGouPoint(m_spFcGouPgdb->GetFeature(3)).GetAddrCode(true));
		}

		// �Z�����̎擾
		TEST_METHOD(Gou_GetAddrName)
		{
			Assert::AreEqual(_T("�k�C���D�y�s�����戮�P�u�P���ڂP�P�|�P"), CGouPoint(m_spFcGouPgdb->GetFeature(2)).GetAddrName(false));
			Assert::AreEqual(_T("�k�C���D�y�s�����戮�P�u�P���ڂP"), CGouPoint(m_spFcGouPgdb->GetFeature(6)).GetAddrName(false));
			Assert::AreEqual(_T("�k�C���D�y�s�����戮�P�u�P���ڃe�X�g�|�P"), CGouPoint(m_spFcGouPgdb->GetFeature(6)).GetAddrName(true));
			Assert::AreEqual(_T("�k�C���D�y�s�����戮�P�u�P���ڂT"), CGouPoint(m_spFcGouPgdb->GetFeature(8)).GetAddrName(false));
			Assert::AreEqual(_T("�k�C���D�y�s�����戮�P�u�P���ڂT�|���"), CGouPoint(m_spFcGouPgdb->GetFeature(8)).GetAddrName(true));
		}

		// �s�s�����擾
		TEST_METHOD(Gou_GetBuilding)
		{
			CGouPoint cGou1(m_spFcGouPgdb->GetFeature(1)), cGou2(m_spFcGouPgdb->GetFeature(2));

			// �����ƌ����̋��E��ɂ���Ƃ��͎擾���Ȃ�
			CBuilding* pBld = cGou1.GetBuilding();
			Assert::IsNull(pBld);

			// �擾����
			Assert::AreEqual((long)1, cGou2.GetBuilding()->GetOID());
		}

		// �v���b�g�ʒu�̓s�s�n�}�s���E�擾(����I�[�i�[�E���[�N�X�y�[�X��)
		TEST_METHOD(Gou_GetCityAdmin1)
		{
			Assert::AreEqual((long)11, CGouPoint(m_spFcGouPgdb->GetFeature(1)).GetCityAdmin()->GetOID());
			Assert::IsNull(CGouPoint(m_spFcGouPgdb->GetFeature(14)).GetCityAdmin());
		}

		// �v���b�g�ʒu�̓s�s�n�}�s���E�擾(���[�N�X�y�[�X�����L���[�U�łȂ��ꍇ)
		TEST_METHOD(Gou_GetCityAdmin2)
		{
			Assert::AreEqual((long)5161071, CGouPoint(m_spFcGouSde->GetFeature(4130945)).GetCityAdmin()->GetOID());
		}
	};

	// staic�����o�ϐ���`
	std::shared_ptr<CFeatureClass> GouPointTest::m_spFcGouPgdb;
	std::shared_ptr<CFeatureClass> GouPointTest::m_spFcGouSde;
}
