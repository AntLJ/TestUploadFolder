#include "stdafx.h"
#include "CheckPolygonAndTable.h"

using namespace sindy::schema;

namespace ErrorCode
{
	// CheckPolygonAndTable�̃G���[�R�[�h��005xxx�Ƃ���
	const CString DoesNotExistInRelAddr = _T("005001");
	const CString DoesNotExistInZipArea = _T("005002");
}

CheckPolygonAndTable::CheckPolygonAndTable()
{
}

CheckPolygonAndTable::~CheckPolygonAndTable()
{
}

bool CheckPolygonAndTable::Init()
{
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

	// �X�֔ԍ��|���S���擾
	m_ipZipCodeArea = zipWorkspace.OpenTable(zipcode_area::kTableName);
	if (!m_ipZipCodeArea)
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S���t�B�[�`���N���X�擾���s\n"));
		return false;
	}

	m_ipZipCodeArea->get_Fields(&ipFields);
	if (!ipFields)
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S���t�B�[���h�Ή��\�擾���s\n"));
		return false;
	}
	if (sindy::errorcode::sindyErr_NoErr != m_zipCodeAreaFieldMap.CreateFieldMap(ipFields))
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S���t�B�[���h�Ή��\�擾���s\n"));
		return false;
	}

	return true;
}

bool CheckPolygonAndTable::Check()
{
	RUNLOGGER.Info(_T("�|���S���ƕR�t���e�[�u���̐����`�F�b�N���s�c\n"));
	std::cout << "�|���S���ƕR�t���e�[�u���̐����`�F�b�N���s�c" << std::endl;

	bool result = CheckZipCode();

	RUNLOGGER.Info(_T("�`�F�b�N�I��\n"));
	std::cout << "�`�F�b�N�I��" << std::endl;

	return result;
}

bool CheckPolygonAndTable::CheckZipCode()
{
	// [�`�F�b�N] �X�֔ԍ��|���S���̎��X�֔ԍ��ƕR�t���e�[�u���̎��X�֔ԍ��̐����`�F�b�N
	// �X�֔ԍ��|���S���̎��X�֔ԍ����擾
	IFeatureCursorPtr ipCursor;
	auto subFields = uh::str_util::format(
		_T("%s, %s, %s"),
		zipcode_area::kObjectID, zipcode_area::kZipCode1, zipcode_area::kZipCode2);
	auto query = AheInitQueryFilter(nullptr, subFields);
	if ( S_OK != m_ipZipCodeArea->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S���J�[�\���擾���s\n"));
		return false;
	}

	long idxZipCode1 = m_zipCodeAreaFieldMap.FindField(zipcode_area::kZipCode1);
	long idxZipCode2 = m_zipCodeAreaFieldMap.FindField(zipcode_area::kZipCode2);
	std::set<CString> polygonZipCodes;
	std::map<CString, std::vector<long>> polygonZipOids; // �X�֔ԍ���OID�Ή��\(���O�p)
	IFeaturePtr ipFeature;
	while (S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature)
	{
		long oid = 0;
		ipFeature->get_OID(&oid);

		CComVariant zipcode1, zipcode2;
		ipFeature->get_Value(idxZipCode1, &zipcode1);
		ipFeature->get_Value(idxZipCode2, &zipcode2);

		polygonZipCodes.insert(zipcode1.bstrVal);
		polygonZipOids[zipcode1.bstrVal].push_back(oid);
		if (!((CString)zipcode2.bstrVal).IsEmpty())
		{
			polygonZipCodes.insert(zipcode2.bstrVal);
			polygonZipOids[zipcode2.bstrVal].push_back(oid);
		}
	}

	// �R�t���e�[�u���̎��X�֔ԍ����擾
	subFields = uh::str_util::format(
		_T("%s, %s"),
		ipc_table::kObjectID, rel_addr::kZipCode);
	auto whereClause = uh::str_util::format(_T("%s = %ld"),
		rel_addr::kModifyStatus, rel_addr::ModifyStatusCode::Used11digit);
	query = AheInitQueryFilter(nullptr, subFields, whereClause);
	_ICursorPtr ipTbCursor;
	if (S_OK != m_ipRelAddrTable->Search(query, VARIANT_TRUE, &ipTbCursor) || !ipTbCursor)
	{
		RUNLOGGER.Error(_T("�Z���R�t���e�[�u���J�[�\���擾���s\n"));
		return false;
	}

	std::set<CString> tableZipCodes;
	std::map<CString, std::vector<long>> tableZipOids; // �X�֔ԍ���OID�Ή��\(���O�p)
	_IRowPtr ipRow;
	long idxAddrCode = m_relAddrTableFieldMap.FindField(rel_addr::kZipCode);
	while (S_OK == ipTbCursor->NextRow(&ipRow) && ipRow)
	{
		long oid = 0;
		ipRow->get_OID(&oid);

		CComVariant zipcode;
		ipRow->get_Value(idxAddrCode, &zipcode);
		tableZipCodes.insert(zipcode.bstrVal);
		tableZipOids[zipcode.bstrVal].push_back(oid);
	}

	// �ˍ�
	std::set<CString> polygonDiff;
	std::set<CString> tableDiff;
	std::set_difference(
		polygonZipCodes.begin(), polygonZipCodes.end(),
		tableZipCodes.begin(), tableZipCodes.end(),
		std::inserter(polygonDiff, polygonDiff.end()));
	std::set_difference(
		tableZipCodes.begin(), tableZipCodes.end(),
		polygonZipCodes.begin(), polygonZipCodes.end(),
		std::inserter(tableDiff, tableDiff.end()));

	// �G���[����
	bool hasError = false;
	if (!polygonDiff.empty() || !tableDiff.empty())
		hasError = true;

	// �G���[�o��
	for (auto& zipcode : polygonDiff)
	{
		for (auto& oid : polygonZipOids[zipcode])
		{
			ERRLOGGER.writeStdLog(
				zipcode_area::kTableName,
				oid,
				_T(""),
				_T(""),
				_T("ERROR"),
				ErrorCode::DoesNotExistInRelAddr,
				_T("�X�֔ԍ��|���S���ɂ��邪�R�t���e�[�u���ɑ��݂��Ȃ��X�֔ԍ�"),
				zipcode
			);
		}
	}

	for (auto& zipcode : tableDiff)
	{
		for (auto& oid : tableZipOids[zipcode])
		{
			ERRLOGGER.writeStdLog(
				rel_addr::kTableName20,
				oid,
				_T(""),
				_T(""),
				_T("ERROR"),
				ErrorCode::DoesNotExistInZipArea,
				_T("�R�t���e�[�u���ɂ��邪�X�֔ԍ��|���S���ɑ��݂��Ȃ��X�֔ԍ�"),
				zipcode
			);
		}
	}

	return !hasError;
};