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
	TEST_CLASS(CityAdminTest)
	{
	private:
		static std::shared_ptr<CFeatureClass> m_spFcCityAdmin;

	public:
		// �e�X�g�N���X����������
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
			CWorkspace cWorkSpace(verificationDatDir + _T("53392442.mdb"));
			m_spFcCityAdmin.reset(new CFeatureClass(cWorkSpace.OpenTable(city_admin::kTableName)));
		}

		// �e�X�g�N���X�I������
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// �s�撬���R�[�h�擾
		TEST_METHOD(Adm_GetCityCode)
		{
			Assert::AreEqual(_T("01101"), CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetCityCode());
			Assert::AreEqual(_T("01101"), CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetCityCode());
		}

		// �厚�E���R�[�h�擾
		TEST_METHOD(Adm_GetOazaAzaCode)
		{
			Assert::AreEqual(_T("001001"), CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetOazaAzaCode());
			Assert::AreEqual(_T("001001"), CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetOazaAzaCode());
		}

		// �X�敄���擾
		TEST_METHOD(Adm_GetGaikuFugo)
		{
			Assert::AreEqual((long)1, CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetGaikuFugo());
		}

		// �g���X�敄���擾
		TEST_METHOD(Adm_GetExGaikuFugo)
		{
			Assert::AreEqual(_T("�e�X�g"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetExGaikuFugo());
		}

		// �g���X�敄�����~
		TEST_METHOD(Adm_GetExGaikuFugoYomi)
		{
			Assert::AreEqual(_T("�X�Y�L"), CCityAdmin(m_spFcCityAdmin->GetFeature(7)).GetExGaikuFugoYomi());
			Assert::AreEqual(_T("�^�i�J"), CCityAdmin(m_spFcCityAdmin->GetFeature(8)).GetExGaikuFugoYomi());
			// �I�u�W�F�N�g���~�e�[�u�����D�悳��Ă��邩���m�F
			Assert::AreEqual(_T("�f���`����"), CCityAdmin(m_spFcCityAdmin->GetFeature(9)).GetExGaikuFugoYomi());
		}

		// �Z��������ʎ擾
		TEST_METHOD(Adm_GetAddrClass)
		{
			using namespace sindy::schema::city_admin::addr_class;
			Assert::IsTrue(kResidenceIndicationAndChibanUndeveloped == CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetAddrClass());
			Assert::IsTrue(kResidenceIndicationMaintenance == CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetAddrClass());
			Assert::IsTrue(kAzaUndeveloped == CCityAdmin(m_spFcCityAdmin->GetFeature(2)).GetAddrClass());
			Assert::IsTrue(kChibanMaintenance == CCityAdmin(m_spFcCityAdmin->GetFeature(3)).GetAddrClass());
		}

		// �G���A���ʃR�[�h�擾
		TEST_METHOD(Adm_GetAreaClass)
		{
			using namespace sindy::schema::city_admin::area_class;
			Assert::IsTrue(kOutArea == CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetAreaClass() );	
			Assert::IsTrue(kInArea == CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetAreaClass() );
			Assert::IsTrue(kSea == CCityAdmin(m_spFcCityAdmin->GetFeature(2)).GetAreaClass() );	
		}

		// �Z���ϊ����[�h�擾
		TEST_METHOD(Adm_GetConvType)
		{
			using namespace sindy::schema::city_admin;
			Assert::IsTrue(conv_type::kNormal == CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetConvType() );
			Assert::IsTrue(conv_type::kOazaGeneration == CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetConvType() );
			Assert::IsTrue(conv_type::kAzaGeneration == CCityAdmin(m_spFcCityAdmin->GetFeature(3)).GetConvType() );
		}

		// 20���R�[�h�擾
		TEST_METHOD(Adm_GetAddrCode)
		{
			// 20���R�[�h(�����l������)
			Assert::AreEqual(_T("01101001001"), CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetAddrCode());
			Assert::AreEqual(_T("0110100100100001"), CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetAddrCode());
			Assert::AreEqual(_T("01101001001"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrCode());

			// 20���R�[�h(�����l��)
			Assert::AreEqual(_T("01101001001A0000"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrCode(true));

			// 20���R�[�h(�ʏ�)
			Assert::AreEqual(_T("01101081000"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrCode2());
			Assert::AreEqual(_T("0110108200000001"), CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetAddrCode2());

			// 20���R�[�h(�ʏ́E�����l��)
			Assert::AreEqual(_T("01101081000A0000"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrCode2(true));
		}

		// �Z�����擾
		TEST_METHOD(Adm_GetAddrName)
		{
			// �Z�����擾
			Assert::AreEqual(_T("�k�C���D�y�s�����戮�P�u�P���ڂP"), CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetAddrName());
			Assert::AreEqual(_T("�k�C���D�y�s�����戮�P�u�P���ڃe�X�g"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrName());
			Assert::AreEqual(_T("�k�C���D�y�s������~�R�k���e�X�g"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrName2());
			Assert::AreEqual(_T("�X���X�s���Q����"), CCityAdmin(m_spFcCityAdmin->GetFeature(4)).GetAddrName());
		}
	};

	// static�����o�ϐ���`
	std::shared_ptr<CFeatureClass> CityAdminTest::m_spFcCityAdmin;
}
