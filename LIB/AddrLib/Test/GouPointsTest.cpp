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
	// FIX���[�U����������K�X�ύX���ĉ�����
	const CString strTableName = _T("FIX201605.City_Admin");

	TEST_CLASS(GouPointsTest)
	{
	private:
		// CGouPoints��static�����o�Ŏ��ƁA�ʂ��Ń`�F�b�N�����Ƃ��ɃG���[�ɂȂ�̂�
		// CWorkspace�������o�Ŏ����ACGouPoints�͓s�x��������悤�ɂ���
		static std::shared_ptr<CWorkspace> m_spWorkspacePgdb;
		static std::shared_ptr<CWorkspace> m_spWorkspaceSde;

	public:
		// �e�X�g�N���X����������
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
			m_spWorkspacePgdb.reset(new CWorkspace(verificationDatDir + _T("53392442.mdb")));

			// �T�[�o���ς������K�X�ύX���ĉ�����
			m_spWorkspaceSde.reset(new CWorkspace(_T("ronly/ronly/SDE.DEFAULT/5151/plum")));
		}

		// �e�X�g�N���X�I������
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// �w��s���E�ȉ��A���|�C���g�擾�e�X�g(�s�s�n�}�s���E�Ɠ��ꃆ�[�U�E���[�N�X�y�[�X�̍��|�C���g)
		TEST_METHOD(Gous_SelectByAddrCode1)
		{
			CGouPoints gouPoints(CFeatureClass(m_spWorkspacePgdb->OpenTable(city_admin::kTableName)));

			// �u01 101 002 001�v�����A�s�s�n�}�s���E�ȉ��̍��|�C���g������
			gouPoints.SelectByAddrCode(_T("01101002001"));
			auto tableType = gouPoints.GetTableType();
			for (auto itAdmin = gouPoints.begin(tableType); itAdmin != gouPoints.end(tableType); ++itAdmin)
			{
				const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
				Assert::AreEqual((long)6, pAdmin->GetOID());

				// ���|�C���g�́A2��
				Assert::AreEqual((size_t)2, pAdmin->size(sindyTableType::gou_point));

				// �s���E���ɂ��鍆�|�C���g�擾
				for (auto itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou)
				{
					const boost::shared_ptr<CGouPoint> p = boost::static_pointer_cast<CGouPoint>(*itGou);
					Assert::IsTrue(12 <= p->GetOID() && p->GetOID() <= 13);
				}
			}
		}

		// �w��s���E�ȉ��A���|�C���g�擾�e�X�g(���[�N�X�y�[�X�����L���[�U�łȂ��ꍇ)
		TEST_METHOD(Gous_SelectByAddrCode2)
		{
			CGouPoints gouPoints(CFeatureClass(m_spWorkspaceSde->OpenTable(strTableName)));

			// �u07 201 118 000�v�����A�s�s�n�}�s���E�ȉ��̍��|�C���g������
			gouPoints.SelectByAddrCode(_T("07201118000"));
			auto tableType = gouPoints.GetTableType();
			for (auto itAdmin = gouPoints.begin(tableType); itAdmin != gouPoints.end(tableType); ++itAdmin )
			{
				// �s���E�����Ă���
				const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
				Assert::IsTrue(0 < pAdmin->GetOID());

				// ���|�C���g�����邱�Ƃ��킩���Ă���
				Assert::IsTrue(0 < pAdmin->size(sindyTableType::gou_point));

				// �s���E���ɂ��鍆�|�C���g�擾
				for (auto itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou)
				{
					// ���|�C���g������
					const boost::shared_ptr<CGouPoint> p = boost::static_pointer_cast<CGouPoint>(*itGou);
					Assert::IsTrue(0 < p->GetOID());
				}
			}
		}

		// �w��s���E�ȉ��A���|�C���g�擾�e�X�g(���|�C���g�蓮�ݒ�Ver)
		TEST_METHOD(Gous_SelectByAddrCode3)
		{
			// �s�s�n�}�s���E�́APGDB
			// ���|�C���g�́ASDE
			CGouPoints gouPoints1(CFeatureClass(m_spWorkspacePgdb->OpenTable(city_admin::kTableName)));
			CGouPoints gouPoints2(CFeatureClass(m_spWorkspaceSde->OpenTable(strTableName)));

			// ��ɍ��|�C���g�蓮�ݒ�
			gouPoints1.SetGouPointTable(gouPoints2.OpenSameOwnerTable(gou_point::kTableName));

			// �u02 201 001 002�v�����A�s�s�n�}�s���E�ȉ��̍��|�C���g������
			gouPoints1.SelectByAddrCode(_T("02201001002"));
			auto tableType = gouPoints1.GetTableType();
			for (auto itAdmin = gouPoints1.begin(tableType); itAdmin != gouPoints1.end(tableType); ++itAdmin )
			{
				const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
				Assert::AreEqual((long)4, pAdmin->GetOID());

				// ���|�C���g�́A1��
				Assert::AreEqual((size_t)1, pAdmin->size(sindyTableType::gou_point));
			
				// �s���E���ɂ��鍆�|�C���g�擾
				for (auto itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou)
				{
					const boost::shared_ptr<CGouPoint> p = boost::static_pointer_cast<CGouPoint>(*itGou);
					Assert::IsTrue(21429856 == p->GetOID());
				}
			}
		}

		// 16���R�[�h�w��s���E�ȉ��A���|�C���g�擾�e�X�g
		TEST_METHOD(Gous_SelectByAddrCode4)
		{
			CGouPoints gouPoints(CFeatureClass(m_spWorkspacePgdb->OpenTable(city_admin::kTableName)));

			// �u01 101 001 001 00001�v�����A�s�s�n�}�s���E�ȉ��̍��|�C���g������
			gouPoints.SelectByAddrCode(_T("0110100100100001"));
			auto tableType = gouPoints.GetTableType();
			for (auto itAdmin = gouPoints.begin(tableType); itAdmin != gouPoints.end(tableType); ++itAdmin )
			{
				const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
				Assert::AreEqual((long)11, pAdmin->GetOID());

				// ���|�C���g�́A2��
				Assert::AreEqual((size_t)2, pAdmin->size(sindyTableType::gou_point));
			
				// �s���E���ɂ��鍆�|�C���g�擾
				for( auto itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou )
				{
					const boost::shared_ptr<CGouPoint> p = boost::static_pointer_cast<CGouPoint>(*itGou);
					Assert::IsTrue(1 == p->GetOID() || 3 == p->GetOID());
				}
			}
		}

		// Where��w��ɂ��s���E�ȉ��A���|�C���g�擾�e�X�g
		TEST_METHOD(Gous_SelectByWhereClause)
		{
			CGouPoints gouPoints(CFeatureClass(m_spWorkspacePgdb->OpenTable(city_admin::kTableName)));

			// �u01 101 001 001 00001�v�����A�s�s�n�}�s���E�ȉ��̍��|�C���g������
			CString strWhere;
			strWhere.Format(
				_T("%s='01101' and %s='001001' and %s=1"),
				city_admin::kCityCode, city_admin::kAddrCode, city_admin::kGaikuFugo);
			gouPoints.SelectByWhereClause(strWhere);
			auto tableType = gouPoints.GetTableType();
			for (auto itAdmin = gouPoints.begin(tableType); itAdmin != gouPoints.end(tableType); ++itAdmin)
			{
				const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
				Assert::AreEqual((long)11, pAdmin->GetOID());

				// ���|�C���g�́A2��
				Assert::AreEqual((size_t)2, pAdmin->size(sindyTableType::gou_point));
			
				// �s���E���ɂ��鍆�|�C���g�擾
				for (auto itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou)
				{
					const boost::shared_ptr<CGouPoint> p = boost::static_pointer_cast<CGouPoint>(*itGou);
					Assert::IsTrue(1 == p->GetOID() || 3 == p->GetOID());
				}
			}
		}
	};

	// static�����o�ϐ���`
	std::shared_ptr<CWorkspace> GouPointsTest::m_spWorkspacePgdb;
	std::shared_ptr<CWorkspace> GouPointsTest::m_spWorkspaceSde;
}
