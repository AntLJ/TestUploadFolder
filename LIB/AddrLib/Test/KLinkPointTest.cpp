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
	TEST_CLASS(KLinkPointTest)
	{
	private:
		static std::shared_ptr<CFeatureClass> m_spFcKLinkPoint;

	public:
		// �e�X�g�N���X����������
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
			CWorkspace cWorkspace(verificationDatDir + _T("53392442.mdb"));
			m_spFcKLinkPoint.reset(new CFeatureClass(cWorkspace.OpenTable(klink_point::kTableName)));
		}

		// �e�X�g�N���X�I������
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// ����22���Z���R�[�h�擾�e�X�g
		TEST_METHOD(KLink_GetAddrCodeOrg)
		{
			Assert::AreEqual(_T("3820407700100001003501"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetAddrCodeOrg());
			Assert::AreEqual(_T("3820407700100001003701"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetAddrCodeOrg());
		}

		// �ŐV22���Z���R�[�h�擾�e�X�g
		TEST_METHOD(KLink_GetAddrCode)
		{
			Assert::AreEqual(_T("3820403100100001003501"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetAddrCode());
			Assert::AreEqual(_T("3820408300100001003701"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetAddrCode());
		}

		// �������̎擾�e�X�g
		TEST_METHOD(KLink_GetKatagakiName)
		{
			Assert::AreEqual(_T(""), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetKatagakiName());
			Assert::AreEqual(_T("�e�X�g"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetKatagakiName());
		}

		// �f�ږ��̎擾�e�X�g
		TEST_METHOD(KLink_GetKeisaiName)
		{
			Assert::AreEqual(_T("�t�W�p���@���Q�c�Ə�"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetKeisaiName());
			Assert::AreEqual(_T("�����l�_�C�n�c���[�^�[�X"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetKeisaiName());
		}

		// ���񋟌��R�[�h�擾�e�X�g
		TEST_METHOD(KLink_GetInfoSrc)
		{
			using namespace klink_point::info_src;
			Assert::IsTrue(kWallLinkData == CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetInfoSrc());
			Assert::IsTrue(kGouMaintenanceData == CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetInfoSrc());
		}

		// ���͗p���W���x�R�[�h�擾�e�X�g
		TEST_METHOD(KLink_GetInputSeido)
		{
			using namespace klink_point::input_seido;
			Assert::IsTrue(kNoVerification == CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetInputSeido());
			Assert::IsTrue(kMultipleWallCounterData == CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetInputSeido());
		}

		// �����[�X�p���W���x�R�[�h�擾�e�X�g
		TEST_METHOD(KLink_GetReleaseSeido)
		{
			using namespace klink_point::release_seido;
			Assert::IsTrue(kMunicipalityBorder == CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetReleaseSeido());
			Assert::IsTrue(kOazaBorder == CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetReleaseSeido());
		}

		// �r�����̎擾�e�X�g
		TEST_METHOD(KLink_GetBuildName)
		{
			Assert::AreEqual(_T("��؃r��"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetBuildName());
			Assert::AreEqual(_T(""), CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetBuildName());
		}

		// �r�����񋟌��R�[�h�擾�e�X�g
		TEST_METHOD(KLink_GetBuildInfoSrc)
		{
			using namespace klink_point::buildinfo_src;
			Assert::IsTrue(kAutoGeneration == CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetBuildInfoSrc());
			Assert::IsTrue(kPhoneSurvey == CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetBuildInfoSrc());
		}

		// �r���ŏ�K���擾�e�X�g
		TEST_METHOD(KLink_GetMaxFloors)
		{
			Assert::AreEqual((long)1, CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetMaxFloors());
			Assert::AreEqual((long)10, CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetMaxFloors());
		}

		// �e�i���g�K���擾�e�X�g
		TEST_METHOD(KLink_GetFloor)
		{
			Assert::AreEqual((long)1, CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetFloor());
			Assert::AreEqual((long)10, CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetFloor());
		}

		// �Z�����擾
		TEST_METHOD(KLink_GetAddrName)
		{
			Assert::AreEqual(_T("���Q�������l�s����P���ڂP�|�R�T"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetAddrName());
			Assert::AreEqual(_T("���Q�������l�s����T�T�X"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(5)).GetAddrName());
			Assert::AreEqual(_T("���Q�������l�s����"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(6)).GetAddrName());
		}

		// �v���b�g�ʒu�̓s�s�n�}�s���E�擾
		TEST_METHOD(KLink_GetCityAdmin)
		{
			Assert::IsNull(CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetCityAdmin());
			Assert::AreEqual((long)3, CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetCityAdmin()->GetOID());
		}
	};

	// static�����o�ϐ���`
	std::shared_ptr<CFeatureClass> KLinkPointTest::m_spFcKLinkPoint;
}
