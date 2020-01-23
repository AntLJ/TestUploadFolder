#include "stdafx.h"
#include <sindy/libschema.h>
#include "Param.h"
#include "LogSys.h"
#include "DBUtils.h"
#include "Addr2TsvBase.h"
#include "FileEUC.h"
#include "Adrp.h"
#include "Master.h"

//-----------
// CMstAdm
//-----------
CMstAdm::CMstAdm(const CParam& cParam, const IWorkspacePtr& ipWS)
	: CAdrpBase(cParam, ipWS), m_idxAdminatt(-1), m_idxName(-1)
{
	setTableName(C_MST_TABLE_ADM);
	setSortField(C_MST_SORT_ADM);
	setOutFileName(C_MST_FILE_ADM);
	setProgressLogPrefix(_T("MSTADM�ϊ�"));
}

bool CMstAdm::init(void)
{
	if (!CAdrpBase::init())
		return false;

	// �t�B�[���hINDEX�擾
	if (!getFieldIndex(m_fieldIndex, sindy::schema::admin_att_master::kAdminAtt, m_idxAdminatt) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::admin_att_master::kName, m_idxName))
		return false;

	return true;
}

bool CMstAdm::convRecord(const _IRowPtr& ipRow)
{
	CString strAdminatt, strName;
	if (!getValue(ipRow, sindy::schema::admin_att_master::kAdminAtt, m_idxAdminatt, strAdminatt) ||
		!getValue(ipRow, sindy::schema::admin_att_master::kName, m_idxName, strName))
		return false;

	std::ostringstream oss;
	oss << CT2A(strAdminatt) << "\t"	// �s������ʃR�[�h
		<< CT2A(strName);				// ����

	m_cFile.write(oss.str());

	return true;
}

//-----------
// CMstPfx
//-----------
CMstPfx::CMstPfx(const CParam& cParam, const IWorkspacePtr& ipWS)
	: CAdrpBase(cParam, ipWS), m_idxPrefix(-1), m_idxName(-1), m_idxYomi(-1)
{
	setTableName(C_MST_TABLE_PFX);
	setSortField(C_MST_SORT_PFX);
	setOutFileName(C_MST_FILE_PFX);
	setProgressLogPrefix(_T("MSTPFX�ϊ�"));
}

bool CMstPfx::init(void)
{
	if (!CAdrpBase::init())
		return false;

	// �t�B�[���hINDEX�擾
	if (!getFieldIndex(m_fieldIndex, sindy::schema::prefix_master::kPrefix, m_idxPrefix) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::prefix_master::kName, m_idxName) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::prefix_master::kYomi, m_idxYomi))
		return false;

	return true;
}

bool CMstPfx::convRecord(const _IRowPtr& ipRow)
{
	CString strPrefix, strName, strYomi;
	if (!getValue(ipRow, sindy::schema::prefix_master::kPrefix, m_idxPrefix, strPrefix) ||
		!getValue(ipRow, sindy::schema::prefix_master::kName, m_idxName, strName) ||
		!getValue(ipRow, sindy::schema::prefix_master::kYomi, m_idxYomi, strYomi))
		return false;

	std::ostringstream oss;
	oss << CT2A(strPrefix) << "\t"	// �ړ�����ʃR�[�h
		<< CT2A(strName) << "\t"	// ����
		<< CT2A(strYomi);			// �ǂ�

	m_cFile.write(oss.str());

	return true;
}

//-----------
// CMstSfx
//-----------
CMstSfx::CMstSfx(const CParam& cParam, const IWorkspacePtr& ipWS)
	: CAdrpBase(cParam, ipWS), m_idxSuffix(-1), m_idxName(-1), m_idxYomi(-1)
{
	setTableName(C_MST_TABLE_SFX);
	setSortField(C_MST_SORT_SFX);
	setOutFileName(C_MST_FILE_SFX);
	setProgressLogPrefix(_T("MSTSFX�ϊ�"));
}

bool CMstSfx::init(void)
{
	if (!CAdrpBase::init())
		return false;

	// �t�B�[���hINDEX�擾
	if (!getFieldIndex(m_fieldIndex, sindy::schema::suffix_master::kSuffix, m_idxSuffix) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::suffix_master::kName, m_idxName) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::suffix_master::kYomi, m_idxYomi))
		return false;

	return true;
}

bool CMstSfx::convRecord(const _IRowPtr& ipRow)
{
	CString strSuffix, strName, strYomi;
	if (!getValue(ipRow, sindy::schema::suffix_master::kSuffix, m_idxSuffix, strSuffix) ||
		!getValue(ipRow, sindy::schema::suffix_master::kName, m_idxName, strName) ||
		!getValue(ipRow, sindy::schema::suffix_master::kYomi, m_idxYomi, strYomi))
		return false;

	std::ostringstream oss;
	oss << CT2A(strSuffix) << "\t"	// �ڔ�����ʃR�[�h
		<< CT2A(strName) << "\t"	// ����
		<< CT2A(strYomi);			// �ǂ�

	m_cFile.write(oss.str());

	return true;
}


//----------------
// CCodeDomain2Tsv
//----------------
CCodeDomain2Tsv::CCodeDomain2Tsv(const CParam& cParam, const IWorkspacePtr& ipWS)
{
	setWorkspace(ipWS);
	setOutDirName(CString(cParam.getOutDir().c_str()));
}

bool CCodeDomain2Tsv::execute(const CString& strCodeDomainName, const CString& strOutFileName)
{
	// �i�����o�́i�W���o�́j
	// �Z���Ԃ̂��ߐi�������o�����ł͂Ȃ����ߍŏ��ƍŌ�OK�����o��
	CString strProgressLogPrefix(strOutFileName.Left(strOutFileName.Find(_T("."))) + _T("�ϊ�"));
	CString strLogMsg;
	strLogMsg.Format(_T("  %s\r"), strProgressLogPrefix);
	std::cout << CT2CA(strLogMsg);

	// �R�[�h�l�h���C���擾
	std::map<long, CString> mapDomain;
	if (!get(strCodeDomainName, mapDomain)) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errGetDomain, _T("�R�[�h�l�h���C���擾���s [%s]"), strCodeDomainName);
		return false;
	}

	// �o��
	CFileEUC cFile;
	if (!cFile.open(m_strOutDirName, strOutFileName)) {
		return false;
	}
	for (const auto& itr : mapDomain)
	{
		std::ostringstream oss;
		oss << itr.first << "\t"	// �R�[�h
			<< CT2A(itr.second);	// ����

		cFile.write(oss.str());
	}

	// ���ʏo�́irun���O�ƕW���o�́j
	strLogMsg.Format(_T("  %s ... OK\n"), strProgressLogPrefix);
	CLogSys::GetInstance().WriteRunLog(true, false, strLogMsg);

	return true;
}

bool CCodeDomain2Tsv::get(const CString& strCodeDomainName, std::map<long, CString>& mapDomain)
{
	IWorkspaceDomainsPtr ipWorkspaceDomain(m_ipWorkspace);
	IDomainPtr ipDomain;
	if (FAILED(ipWorkspaceDomain->get_DomainByName(CComBSTR(strCodeDomainName), &ipDomain)))
		return false;

	ICodedValueDomainPtr ipCodedValueDomain(ipDomain);
	if (!ipCodedValueDomain)
		return false;

	long lDomainCount = 0;
	if (FAILED(ipCodedValueDomain->get_CodeCount(&lDomainCount)))
		return false;

	for (long i = 0; i < lDomainCount; i++)
	{
		CComVariant vaValue;
		CComBSTR bstrName;
		if (FAILED(ipCodedValueDomain->get_Value(i, &vaValue)))
			return false;
		if (FAILED(ipCodedValueDomain->get_Name(i, &bstrName)))
			return false;

		mapDomain.insert(std::pair<long, CString>(vaValue.lVal, CString(bstrName)));
	}
	return true;
}