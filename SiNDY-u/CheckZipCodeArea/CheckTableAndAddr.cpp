#include "stdafx.h"
#include "CheckUtil.h"
#include "CheckTableAndAddr.h"

using namespace addr;
using namespace sindy;
using namespace sindy::schema;

namespace ErrorCode
{
	// CheckTableAndAddr�̃G���[�R�[�h��003xxx�Ƃ���
	const CString DoesNotExistInAddressData = _T("003001");
	const CString DoesNotExistInRelAddr = _T("003002");
}

CheckTableAndAddr::CheckTableAndAddr()
{
}

CheckTableAndAddr::~CheckTableAndAddr()
{
}

bool CheckTableAndAddr::Init()
{
	// SDE(�X�֔ԍ��|���S��)�ڑ�
	sindy::CWorkspace zipWorkspace;
	if (!zipWorkspace.Connect(m_args.m_zipDb.c_str()))
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S��DB�ڑ���ւ̐ڑ��Ɏ��s"));
		return false;
	}

	// �R�t���e�[�u���t�@�C���_������
	CModel zipModel;
	m_relAddrFinder.InitCollection(zipWorkspace.OpenTable(rel_addr::kTableName20), zipModel);

	// SDE(�s���E�A��)�ڑ�
	sindy::CWorkspace addrWorkspace;
	if (!addrWorkspace.Connect(m_args.m_addrDb.c_str()))
	{
		RUNLOGGER.Error(_T("�Z���f�[�^DB�ڑ���ւ̐ڑ��Ɏ��s"));
		return false;
	}

	// �Z���f�[�^(�s���E�A��)�t�@�C���_������
	CModel addrModel;
	m_addrFinder.InitCollection(addrWorkspace.OpenTable(city_admin::kTableName), addrModel);

	// �����t���Z���R�[�h�ϊ��e�[�u���t�@�C���ڑ�
	if (!m_addrRecHelper.connectCharConvFile(m_args.m_charConv.c_str()))
	{
		RUNLOGGER.Error(_T("�����t���ϊ��e�[�u���ւ̐ڑ��Ɏ��s"));
		return false;
	}

	// �����t���Z���R�[�h�ϊ����e�[�u���t�@�C���ڑ�
	if (!m_addrRecHelper.connectCharConvTmpFile(m_args.m_charConvTmp.c_str()))
	{
		RUNLOGGER.Error(_T("�����t���ϊ����e�[�u���ւ̐ڑ��Ɏ��s"));
		return false;
	}

	return true;
}


bool CheckTableAndAddr::Check()
{
	RUNLOGGER.Info(_T("�Z���R�t���e�[�u���ƏZ���f�[�^�̐����`�F�b�N���s�c\n"));
	std::cout << "�Z���R�t���e�[�u���ƏZ���f�[�^�̐����`�F�b�N���s�c" << std::endl;

	bool result = true;
	RUNLOGGER.Info(_T("�e�[�u������Z���f�[�^�̃`�F�b�N\n"));
	std::cout << "�e�[�u������Z���f�[�^�̃`�F�b�N" << std::endl;
	result &= CheckTableToAddr();

	RUNLOGGER.Info(_T("�s���E����e�[�u���̃`�F�b�N\n"));
	std::cout << "�s���E����e�[�u���̃`�F�b�N" << std::endl;
	result &= CheckCityAdminToTable();

	RUNLOGGER.Info(_T("�`�F�b�N�I��\n"));
	std::cout << "�`�F�b�N�I��" << std::endl;
	return result;
}

bool CheckTableAndAddr::CheckTableToAddr()
{
	CheckUtil util(m_addrRecHelper);

	// SiNDY�Z���f�[�^
	auto& cityAdminTbl = *m_addrFinder.FindTable(city_admin::kTableName);
	cityAdminTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto& gouPointTbl = *m_addrFinder.FindTable(gou_point::kTableName);
	gouPointTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto& csPointTbl = *m_addrFinder.FindTable(cs_addr_point::kTableName);
	csPointTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// �Z���R�t���e�[�u��
	auto& relAddrTbl = *m_relAddrFinder.FindTable(rel_addr::kTableName20);
	relAddrTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto query = AheInitQueryFilter(
		nullptr, nullptr,
		_T("%s NOT IN (%ld, %ld, %ld)"),
		rel_addr::kModifyStatus,
		rel_addr::ModifyStatusCode::UnUsed,
		rel_addr::ModifyStatusCode::Obsolete,
		rel_addr::ModifyStatusCode::Used5digit);
	relAddrTbl._Select(query, false);

	// 11���R�[�h+����(�n�ԁA��)�̈ꗗ���쐬
	// ���Ȃ����͎̂d���Ȃ��̂ŁA��ꂽ���̂����Ń`�F�b�N
	std::map<CString, AddrRecPairs> relAddrMap;
	if (!util.MakeAddrYomiList(relAddrTbl, relAddrMap))
	{
		RUNLOGGER.Error(_T("11���R�[�h�{���̂̎擾�ɃG���[������܂��B(�G���[���O�Q��)\n"));
	}

	// CS�͏��Ȃ��̂ň�U�S��������Ă��܂�
	std::map<CString, AddrRecs> csAddrRecsMap;
	m_addrRecHelper.makeAdrRecordsFromCs(csPointTbl, _T(""), csAddrRecsMap);
	csPointTbl.clear();

	// 11�����Ƃɏ���
	for (auto & relAddrRecPairs : relAddrMap)
	{
		std::cout << "�Z���R�[�h: " << CStringA(relAddrRecPairs.first).GetString() << " ������" << std::endl;

		// 11���R�[�h����s���E�擾
		AddrRecPairs adminAddrRecs;
		m_addrRecHelper.getCityAdminRecs(cityAdminTbl, relAddrRecPairs.first, adminAddrRecs);
		cityAdminTbl.clear();

		// 11���R�[�h����CS�|�C���g�擾
		AddrRecs csAddrRecs;
		if (0 != csAddrRecsMap.count(relAddrRecPairs.first.Left(11)))
		{
			csAddrRecs = csAddrRecsMap[relAddrRecPairs.first.Left(11)];
		}

		// ���ݔ���
		for (auto& relAddrRec : relAddrRecPairs.second)
		{
			bool exist = util.CheckAddressDataExistance(
				adminAddrRecs, csAddrRecs, gouPointTbl, relAddrRec.first);

			if (!exist)
			{
				ERRLOGGER.writeStdLog(
					rel_addr::kTableName20,
					relAddrRec.second.GetOID(),
					_T(""),
					_T(""),
					_T("ERROR"),
					ErrorCode::DoesNotExistInAddressData,
					_T("�Z���R�[�h�ɑΉ�����s���E�A���ACS�����݂��Ȃ�"),
					relAddrRec.first.getJusyocode()
				);
			}
		}
	}

	return true;
}

bool CheckTableAndAddr::CheckCityAdminToTable()
{
	// SiNDY�Z���f�[�^
	auto& cityAdminTbl = *m_addrFinder.FindTable(city_admin::kTableName);
	cityAdminTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// �Z���R�t���e�[�u��
	auto& relAddrTbl = *m_relAddrFinder.FindTable(rel_addr::kTableName20);
	relAddrTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	bool hasError = false;
	for (int i = 1; i <= 47; ++i)
	{
		RUNLOGGER.Log(_T("�s���{���R�[�h: %02d �̃`�F�b�N��\n"), i);
		auto prefCode = uh::str_util::formatA(_T("%02d"), i).GetString();
		std::cout << "�s���{���R�[�h: " << prefCode << "�̃`�F�b�N��" << std::endl;

		CString strWhere = uh::str_util::format(_T("%s LIKE '%02d%%%%'"), city_admin::kCityCode, i);
		cityAdminTbl._Select(AheInitQueryFilter(nullptr, nullptr, strWhere), false);

		std::map<CString, std::vector<long>> addrCodeMap; // key:11���R�[�h val:OID�Q
		for (const auto& cityAdmin : cityAdminTbl)
		{
			auto& row = *CAST_ROWC(cityAdmin);
			CString citycode = row.GetValue(city_admin::kCityCode, CString());
			CString addrcode1 = row.GetValue(city_admin::kAddrCode, CString());
			CString addrcode2 = row.GetValue(city_admin::kAddrCode2, CString());

			// ADDRCODE��000000��999999�͏��O (�C�Ƃ����q���~�n�Ƃ��ŗX�֔ԍ��͎����Ȃ�)
			if (0 != addrcode1.Compare(_T("000000")) && 0 != addrcode1.Compare(_T("999999")))
			{
				addrCodeMap[citycode + addrcode1].push_back(row.GetOID());
			}

			// ADDRCODE2������(999999�͂��Ȃ��͂�����)
			if (0 != addrcode2.Compare(_T("000000")) && 0 != addrcode2.Compare(_T("999999")))
			{
				addrCodeMap[citycode + addrcode2].push_back(row.GetOID());
			}
		}
		cityAdminTbl.clear();

		// �R�t���e�[�u�����i�荞��
		strWhere = uh::str_util::format(_T("%s LIKE '%02d%%%%' AND %s <> %ld"),
			rel_addr::kAddrCode, i,
			rel_addr::kModifyStatus,
			rel_addr::ModifyStatusCode::Obsolete);
		relAddrTbl._Select(AheInitQueryFilter(nullptr, rel_addr::kAddrCode, strWhere), false);
		static const long idxAddrCode = relAddrTbl.FindField(rel_addr::kAddrCode); // index����Ă����Ƃ�����Ƒ����Ȃ�
		std::set<CString> relAddrCodeSet;
		for (auto rec : relAddrTbl)
		{
			auto relAddrCode = CAST_ROWC(rec)->GetValue(idxAddrCode, CString());
			relAddrCodeSet.emplace(relAddrCode.Left(11));
		}
		relAddrTbl.clear();

		// �O��11������v���郌�R�[�h�������OK
		for (const auto& addrCode : addrCodeMap)
		{
			const auto& result = relAddrCodeSet.find(addrCode.first);
			if (result != relAddrCodeSet.end())
				continue;

			hasError = true;
			for (auto oid : addrCode.second)
			{
				ERRLOGGER.writeStdLog(
					city_admin::kTableName,
					oid,
					_T(""),
					_T(""),
					_T("ERROR"),
					ErrorCode::DoesNotExistInRelAddr,
					_T("�s���E�ɂ��邪�R�t���e�[�u���ɑ��݂��Ȃ�"),
					addrCode.first
				);
			}
		}
	}
	return !hasError;
}
