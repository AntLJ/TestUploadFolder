#include "stdafx.h"
#include <sindy/libschema.h>
#include "Param.h"
#include "LogSys.h"
#include "DBUtils.h"
#include "Addr2TsvBase.h"
#include "FileEUC.h"
#include "Adrp.h"
#include "Zipp.h"


CZipp001::CZipp001(const CParam& cParam, const IWorkspacePtr& ipWS)
	: CAdrpBase(cParam, ipWS), m_idxZipCode(-1), m_idxZipAtt(-1), m_idxName(-1), m_idxYomi(-1),
	m_idxPrefCode(-1), m_idxCityCode(-1), m_idxAddrCode1(-1), m_idxAddrCode2(-1), m_idxRep(-1)
{
	setTableName(C_ZIPP_TABLE_001);
	setSortField(C_ZIPP_SORT_001);
	setOutFileName(C_ZIPP_FILE_001);
	setProgressLogPrefix(_T("ZIPP001�ϊ�"));
}

bool CZipp001::init(void)
{
	if (!CAdrpBase::init())
		return false;

	// �t�B�[���hINDEX�擾
	if (!getFieldIndex(m_fieldIndex, sindy::schema::zip_point::kZipCode, m_idxZipCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::zip_point::kZipAtt, m_idxZipAtt) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::zip_point::kName, m_idxName) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::zip_point::kYomi, m_idxYomi) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::zip_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::zip_point::kCityCode, m_idxCityCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::zip_point::kAddrCode1, m_idxAddrCode1) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::zip_point::kAddrCode2, m_idxAddrCode2) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::zip_point::kRep, m_idxRep))
		return false;

	return true;
}

bool CZipp001::convRecord(const _IRowPtr& ipRow)
{
	CString strZipCode, strZipAtt, strName, strYomi, strPrefCode, strCityCode, strAddrCode1, strAddrCode2, strRep;
	if (!getValue(ipRow, sindy::schema::zip_point::kZipCode, m_idxZipCode, strZipCode) ||
		!getValue(ipRow, sindy::schema::zip_point::kZipAtt, m_idxZipAtt, strZipAtt) ||
		!getValue(ipRow, sindy::schema::zip_point::kName, m_idxName, strName) ||
		!getValue(ipRow, sindy::schema::zip_point::kYomi, m_idxYomi, strYomi) ||
		!getValue(ipRow, sindy::schema::zip_point::kPrefCode, m_idxPrefCode, strPrefCode) ||
		!getValue(ipRow, sindy::schema::zip_point::kCityCode, m_idxCityCode, strCityCode) ||
		!getValue(ipRow, sindy::schema::zip_point::kAddrCode1, m_idxAddrCode1, strAddrCode1) ||
		!getValue(ipRow, sindy::schema::zip_point::kAddrCode2, m_idxAddrCode2, strAddrCode2) ||
		!getValue(ipRow, sindy::schema::zip_point::kRep, m_idxRep, strRep))
		return false;

	int mesh = 0, x = 0, y = 0;
	if (!getMeshXY((IFeaturePtr)ipRow, mesh, x, y))
		return false;

	std::ostringstream oss;
	oss << CT2A(strZipCode) << "\t"		// �X�֔ԍ��R�[�h
		<< CT2A(strZipAtt) << "\t"		// �X�֔ԍ���ʃR�[�h
		<< mesh << "\t"					// 2�����b�V���R�[�h
		<< x << "\t"					// X���W
		<< y << "\t"					// Y���W
		<< CT2A(strName) << "\t"		// ����
		<< CT2A(strYomi) << "\t"		// �ǂ�
		<< CT2A(strPrefCode) << "\t"	// �s���{���R�[�h
		<< CT2A(strCityCode) << "\t"	// �s�撬���R�[�h
		<< CT2A(strAddrCode1) << "\t"	// ���E�厚�R�[�h
		<< CT2A(strAddrCode2) << "\t"	// ���ځE���R�[�h
		<< CT2A(strRep);				// ��\�_�t���O

	m_cFile.write(oss.str());

	// �����\�p�ɗX�֔ԍ���ʃR�[�h�P�ʂŌ����J�E���g
	std::vector<long> vecValue(2, 0);
	std::pair<std::map<long, std::vector<long>>::iterator, bool> ret;
	ret = m_codeCountList.insert(std::make_pair(_ttol(strZipAtt), vecValue));
	// std::vector��0�ԖځF������
	// std::vector��1�ԖځF���j�[�N�����i�X�֔ԍ���ʃR�[�h���Z���̏ꍇ�́A��\�t���O��1�̃��R�[�h���B�X�֔ԍ���ʃR�[�h�����̑��̏ꍇ�͑������Ɠ��l�j
	++((ret.first)->second)[0];
	if (_ttol(strZipAtt) == sindy::schema::zip_point::zip_att::kAddress) {
		if (strRep == _T("1"))
			++((ret.first)->second)[1];
	}
	else
		++((ret.first)->second)[1];

	return true;
}
