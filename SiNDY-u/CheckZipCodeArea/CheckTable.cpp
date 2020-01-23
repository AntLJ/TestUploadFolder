#include "stdafx.h"
#include "CheckTable.h"

using namespace sindy::schema;

namespace ErrorCode
{
	// CheckTable�̃G���[�R�[�h��001xxx�Ƃ���
	const CString DuplicationOfAddrCode = _T("001001");
	const CString IllegalModifyStatus = _T("001002");
}

CheckTable::CheckTable()
{
}

CheckTable::~CheckTable()
{
}

bool CheckTable::Init()
{
	RUNLOGGER.Info(_T("�Z���R�t���e�[�u���`�F�b�N�������c\n"));

	// SDE(�X�֔ԍ��|���S��)�ڑ�
	sindy::CWorkspace zipWorkspace;
	if (!zipWorkspace.Connect(m_args.m_zipDb.c_str()))
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S��DB�ڑ���ւ̐ڑ��Ɏ��s\n"));
		return false;
	}

	// �R�t���e�[�u���擾
	m_ipRelAddrTable = zipWorkspace.OpenTable(rel_addr::kTableName20);
	if (!m_ipRelAddrTable)
	{
		RUNLOGGER.Error(_T("�Z���R�t���e�[�u���擾���s\n"));
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

bool CheckTable::Check()
{
	RUNLOGGER.Info(_T("�Z���R�t���e�[�u���`�F�b�N���s�c\n"));
	std::cout << "�Z���R�t���e�[�u���`�F�b�N���s�c" << std::endl;
	bool result = true;

	RUNLOGGER.Info(_T("�Z���R�t���e�[�u���d���`�F�b�N\n"));
	std::cout << "�Z���R�t���e�[�u���d���`�F�b�N" << std::endl;
	result &= CheckAddrCodeDuplication();

	RUNLOGGER.Info(_T("�Z���R�t���e�[�u�������󋵃R�[�h�`�F�b�N\n"));
	std::cout << "�Z���R�t���e�[�u�������󋵃R�[�h�`�F�b�N" << std::endl;
	result &= CheckModifyStatusCode();

	RUNLOGGER.Info(_T("�`�F�b�N�I��\n"));
	std::cout << "�`�F�b�N�I��" << std::endl;

	return result;
}


bool CheckTable::CheckAddrCodeDuplication()
{
	// [�`�F�b�N1] �e�[�u���̏Z���R�[�h�ɏd�����Ȃ����Ƃ��m�F����
	// �����󋵁u�p�~�v�͏��O
	auto whereClause = uh::str_util::format(
		_T("%s <> %ld"), rel_addr::kModifyStatus, rel_addr::ModifyStatusCode::Obsolete);
	auto query = AheInitQueryFilter(nullptr, nullptr, whereClause);
	_ICursorPtr ipCursor;
	if (S_OK != m_ipRelAddrTable->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		RUNLOGGER.Error(_T("�J�[�\���擾���s\n"));
		return false;
	}

	// �Ƃ肠�����S������Ă���
	_IRowPtr ipRow;
	long idxAddrCode = m_relAddrTableFieldMap.FindField(rel_addr::kAddrCode);
	std::map<CString, std::pair<long, std::vector<long>>> addrCodes; // key:�Z���R�[�h�Aval:�d���J�E���g�Aoid
	while (S_OK == ipCursor->NextRow(&ipRow) && ipRow)
	{
		CComVariant addrCode;
		ipRow->get_Value(idxAddrCode, &addrCode);
		CString strAddrCode(addrCode);
		if (strAddrCode.IsEmpty())
			continue;

		++addrCodes[strAddrCode].first;
		long oid = 0;
		ipRow->get_OID(&oid);
		addrCodes[strAddrCode].second.push_back(oid);
	}

	bool hasError = false;
	for (const auto& dat : addrCodes)
	{
		if (1 < dat.second.first)
		{
			// �G���[�o��
			for (auto oid : dat.second.second)
			{
				ERRLOGGER.writeStdLog(
					rel_addr::kTableName20,
					oid,
					_T(""),
					_T(""),
					_T("ERROR"),
					ErrorCode::DuplicationOfAddrCode,
					_T("�Z���R�[�h�d��"),
					dat.first
				);
			}
			hasError = true;
		}
	}
	return !hasError;
}

bool CheckTable::CheckModifyStatusCode()
{
	// [�`�F�b�N2] �����󋵃R�[�h���P�C�Q�C�R�C�S�ł��邱�Ƃ��m�F����
	// �P�C�Q�C�R�C�S�ȊO�Ō������Ă݂�
	CString whereClause;
	whereClause.Format(_T("%s not in (%d, %d, %d, %d)"),
		rel_addr::kModifyStatus,
		rel_addr::ModifyStatusCode::Obsolete,
		rel_addr::ModifyStatusCode::UnUsed,
		rel_addr::ModifyStatusCode::Used11digit,
		rel_addr::ModifyStatusCode::Used5digit);

	_ICursorPtr ipCursor;
	auto query = AheInitQueryFilter(nullptr, nullptr, whereClause);
	if (S_OK != m_ipRelAddrTable->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		RUNLOGGER.Error(_T("�J�[�\���擾���s\n"));
		return false;
	}

	// ����������͑S���G���[
	bool hasError = false;
	_IRowPtr ipRow;
	long idxAddrCode = m_relAddrTableFieldMap.FindField(rel_addr::kAddrCode);
	long idxModifyStatus = m_relAddrTableFieldMap.FindField(rel_addr::kModifyStatus);
	while (S_OK == ipCursor->NextRow(&ipRow) && ipRow)
	{
		long oid = 0;
		ipRow->get_OID(&oid);
		CComVariant addrcode, modifyStatusCode;
		ipRow->get_Value(idxAddrCode, &addrcode);
		ipRow->get_Value(idxModifyStatus, &modifyStatusCode);
		ERRLOGGER.writeStdLog(
			rel_addr::kTableName20,
			oid,
			_T(""),
			_T(""),
			_T("ERROR"),
			ErrorCode::IllegalModifyStatus,
			_T("�s���Ȑ����󋵃R�[�h"),
			uh::str_util::format(_T("%s\t%d"), addrcode.bstrVal, modifyStatusCode.lVal)
		);
		hasError = true;
	}
	return !hasError;
};