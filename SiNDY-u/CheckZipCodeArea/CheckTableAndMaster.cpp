#include "stdafx.h"
#include "CheckTableAndMaster.h"

using namespace sindy::schema;

namespace ErrorCode
{
	// CheckTableAndMaster�̃G���[�R�[�h��002xxx�Ƃ���
	const CString DoesNotExistInMasterAdr11 = _T("002001");
	const CString DoesNotExistInMasterAdr5 = _T("002002");
}

/**
 * @brief �Z���R�[�h�̃Z�b�g�ɍ������邩�ǂ����𒲂ׂ�
 * @param masterRecords [in] �Z���}�X�^�̏Z���R�[�h
 * @param addrCodes [in] �����𒲂ׂ����Z���R�[�h
 * @param diffCodes [in,out] �Z���R�[�h�̍���
 * @param fieldName [in] �Z���}�X�^�̏Z���R�[�h�t�B�[���h��
 * @retval true �����Ȃ�
 * @retval false ��������
 */
bool HasAddrDiff(
	addr::CJusyoRecordSet& masterRecords,
	const std::set<CString>& addrCodes,
	std::set<CString>& diffCodes,
	const CString& fieldName)
{
	// �K�v�ȕ�����CJusyoRecord����std::set�֊i�[
	std::set<CString> masterAddrCodes;
	masterRecords.begin();
	while (!masterRecords.end())
	{
		masterAddrCodes.insert(masterRecords.GetStringValue(fieldName));
		masterRecords.next();
	}

	// addrCodes�ɂ�����master�ɂȂ����̂�diffCodes�ɓ����
	std::set_difference(
		addrCodes.begin(), addrCodes.end(),
		masterAddrCodes.begin(), masterAddrCodes.end(),
		std::inserter(diffCodes, diffCodes.end()));

	// diffCodes����(�����Ȃ�)�Ȃ�true
	return (diffCodes.empty());
}

CheckTableAndMaster::CheckTableAndMaster()
{
}

CheckTableAndMaster::~CheckTableAndMaster()
{
}

bool CheckTableAndMaster::Init()
{
	// �Z���}�X�^�擾
	if (!m_jusyoMasterHelper.Connect(m_args.m_jusyoMaster.c_str()))
	{
		RUNLOGGER.Error(_T("�Z���}�X�^�ڑ��Ɏ��s"));
		return false;
	}

	// �����}�X�^�擾
	if (!m_kajyoMasterHelper.Connect(m_args.m_kajyoMaster.c_str()))
	{
		RUNLOGGER.Error(_T("�����}�X�^�ڑ��Ɏ��s"));
		return false;
	}

	// SDE(�X�֔ԍ��|���S��)�ڑ�
	sindy::CWorkspace zipWorkspace;
	if (!zipWorkspace.Connect(m_args.m_zipDb.c_str()))
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S��DB�ڑ���ւ̐ڑ��Ɏ��s"));
		return false;
	}

	// �R�t���e�[�u���擾
	m_ipRelAddrTable = zipWorkspace.OpenTable(rel_addr::kTableName20);
	if (!m_ipRelAddrTable)
	{
		RUNLOGGER.Error(_T("�Z���R�t���e�[�u���擾���s"));
		return false;
	}

	// �t�B�[���h�}�b�v����
	IFieldsPtr ipFields;
	m_ipRelAddrTable->get_Fields(&ipFields);
	if (!ipFields)
	{
		RUNLOGGER.Error(_T("�Z���R�t���e�[�u���t�B�[���h�Ή��\�擾���s\n"));
		return false;
	}
	if (sindy::errorcode::sindyErr_NoErr != m_relAddrTableFieldMap.CreateFieldMap(ipFields))
	{
		RUNLOGGER.Error(_T("�Z���R�t���e�[�u���t�B�[���h�Ή��\�擾���s\n"));
		return false;
	}
	return true;
}

bool CheckTableAndMaster::Check()
{
	RUNLOGGER.Info(_T("�Z���R�t���e�[�u���ƏZ���}�X�^�̐����`�F�b�N���s�c\n"));
	std::cout << "�Z���R�t���e�[�u���ƏZ���}�X�^�̐����`�F�b�N���s�c" << std::endl;

	RUNLOGGER.Info(_T("11���`�F�b�N���c\n"));
	std::cout << "11���`�F�b�N���c" << std::endl;
	bool result = CheckExistAddrCode();

	RUNLOGGER.Info(_T("5���`�F�b�N���c\n"));
	std::cout << "5���`�F�b�N���c" << std::endl;
	result &= CheckExistCityCode();

	RUNLOGGER.Info(_T("�`�F�b�N�I��\n"));
	std::cout << "�`�F�b�N�I��" << std::endl;
	return result;
}

bool CheckTableAndMaster::CheckExistAddrCode()
{
	// [�`�F�b�N] �R�t���e�[�u���̏Z���R�[�h(11��)���Z���}�X�^�A�����}�X�^��ɑ��݂��Ă��邩�m�F
	// �R�t���e�[�u������11���擾

	CString whereClause;
	whereClause.Format(_T("%s <> %ld"),
		rel_addr::kModifyStatus,
		rel_addr::ModifyStatusCode::Used5digit);

	_ICursorPtr ipCursor;
	auto query = AheInitQueryFilter(nullptr, nullptr, whereClause);
	if (S_OK != m_ipRelAddrTable->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		RUNLOGGER.Error(_T("�J�[�\���擾���s"));
		return false;
	}

	_IRowPtr ipRow;
	std::set<CString> addrCodes;
	std::map<CString, std::vector<long>> addrCodeIds;
	long idxAddrCode = m_relAddrTableFieldMap.FindField(rel_addr::kAddrCode);
	while (S_OK == ipCursor->NextRow(&ipRow) && ipRow)
	{
		long oid = 0;
		ipRow->get_OID(&oid);
		CComVariant addrCode;
		ipRow->get_Value(idxAddrCode, &addrCode);
		addrCodes.emplace(CString(addrCode.bstrVal).Left(11));
		addrCodeIds[CString(addrCode.bstrVal).Left(11)].emplace_back(oid);
	}

	// �Z���}�X�^�ɂ�����̂͏��O
	CString queryString;
	queryString.Append(_T("SELECT (ken_code || shi_code || oaza_code || aza_code) as ADDRCODE "));
	queryString.Append(_T("FROM   areacode_master "));
	addr::CJusyoRecordSet masterRecords;
	if (!m_jusyoMasterHelper.Select(queryString, masterRecords))
	{
		RUNLOGGER.Error(_T("�Z���}�X�^�擾���s\n"));
		return false;
	}
	if (0 == masterRecords.GetRecordCount())
	{
		RUNLOGGER.Error(_T("�Z���}�X�^�擾���s\n"));
		return false;
	}

	std::set<CString> diffMaster;
	if (HasAddrDiff(masterRecords, addrCodes, diffMaster, _T("ADDRCODE")))
		return true;

	// �c����̂������}�X�^�ɂ��邱�Ƃ��m�F
	addrCodes = diffMaster;
	diffMaster.clear();
	if (!m_kajyoMasterHelper.Select(queryString, masterRecords))
	{
		RUNLOGGER.Error(_T("�����}�X�^�擾���s\n"));
		return false;
	}
	if (0 == masterRecords.GetRecordCount())
	{
		RUNLOGGER.Error(_T("�����}�X�^�擾���s\n"));
		return false;
	}

	if (HasAddrDiff(masterRecords, addrCodes, diffMaster, _T("ADDRCODE")))
		return true;

	// �Ō�܂Ŏc���Ă��̓G���[
	for (auto& addrcode : diffMaster)
	{
		for (auto& oid : addrCodeIds[addrcode])
		{
			ERRLOGGER.writeStdLog(
				rel_addr::kTableName20,
				oid,
				_T(""),
				_T(""),
				_T("ERROR"),
				ErrorCode::DoesNotExistInMasterAdr11,
				_T("�Z���R�[�h�i11/16/20���j���}�X�^�ɑ��݂��Ȃ�"),
				addrcode
			);
		}
	}
	return false;
}

bool CheckTableAndMaster::CheckExistCityCode()
{
	// [�`�F�b�N] �R�t���e�[�u���̏Z���R�[�h(5��)���Z���}�X�^��ɑ��݂��A���������Ă��邩�m�F
	// �R�t���e�[�u������5���擾

	CString whereClause;
	whereClause.Format(_T("%s = %ld"),
		rel_addr::kModifyStatus,
		rel_addr::ModifyStatusCode::Used5digit);

	_ICursorPtr ipCursor;
	auto query = AheInitQueryFilter(nullptr, nullptr, whereClause);
	if (S_OK != m_ipRelAddrTable->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		RUNLOGGER.Error(_T("�J�[�\���擾���s"));
		return false;
	}

	_IRowPtr ipRow;
	std::set<CString> cityCodes;
	std::map<CString, std::vector<long>> cityCodeIds;
	long idxAddrCode = m_relAddrTableFieldMap.FindField(rel_addr::kAddrCode);
	while (S_OK == ipCursor->NextRow(&ipRow) && ipRow)
	{
		long oid = 0;
		ipRow->get_OID(&oid);
		CComVariant addrCode;
		ipRow->get_Value(idxAddrCode, &addrCode);
		cityCodes.emplace(CString(addrCode.bstrVal));
		cityCodeIds[CString(addrCode.bstrVal)].emplace_back(oid);
	}

	// �Z���}�X�^�ɂ�����̂͏��O
	CString queryString;
	queryString.Append(_T("SELECT distinct (new_ken_code || new_shi_code) as ADDRCODE "));
	queryString.Append(_T("FROM   areacode_master "));
	queryString.Append(_T("WHERE  haishi_date = '000000' "));
	queryString.Append(_T("AND    new_oaza_code = '000' AND new_aza_code = '000' "));
	queryString.Append(_T("AND    new_shi_code != '000' "));
	addr::CJusyoRecordSet masterRecords;
	if (!m_jusyoMasterHelper.Select(queryString, masterRecords))
	{
		RUNLOGGER.Error(_T("�Z���}�X�^�擾���s\n"));
		return false;
	}
	if (0 == masterRecords.GetRecordCount())
	{
		RUNLOGGER.Error(_T("�Z���}�X�^�擾���s\n"));
		return false;
	}

	std::set<CString> diffMaster;
	if (HasAddrDiff(masterRecords, cityCodes, diffMaster, _T("ADDRCODE")))
		return true;

	// �Ō�܂Ŏc���Ă��̓G���[
	for (auto& citycode : diffMaster)
	{
		for (auto& oid : cityCodeIds[citycode])
		{
			ERRLOGGER.writeStdLog(
				rel_addr::kTableName20,
				oid,
				_T(""),
				_T(""),
				_T("ERROR"),
				ErrorCode::DoesNotExistInMasterAdr5,
				_T("�Z���R�[�h�i5���j���}�X�^�ɑ��݂��Ȃ�"),
				citycode
			);
		}
	}
	return false;
}
