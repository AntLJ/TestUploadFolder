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
	setProgressLogPrefix(_T("MSTADM変換"));
}

bool CMstAdm::init(void)
{
	if (!CAdrpBase::init())
		return false;

	// フィールドINDEX取得
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
	oss << CT2A(strAdminatt) << "\t"	// 行政区域種別コード
		<< CT2A(strName);				// 名称

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
	setProgressLogPrefix(_T("MSTPFX変換"));
}

bool CMstPfx::init(void)
{
	if (!CAdrpBase::init())
		return false;

	// フィールドINDEX取得
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
	oss << CT2A(strPrefix) << "\t"	// 接頭辞種別コード
		<< CT2A(strName) << "\t"	// 名称
		<< CT2A(strYomi);			// 読み

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
	setProgressLogPrefix(_T("MSTSFX変換"));
}

bool CMstSfx::init(void)
{
	if (!CAdrpBase::init())
		return false;

	// フィールドINDEX取得
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
	oss << CT2A(strSuffix) << "\t"	// 接尾辞種別コード
		<< CT2A(strName) << "\t"	// 名称
		<< CT2A(strYomi);			// 読み

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
	// 進捗率出力（標準出力）
	// 短時間のため進捗率を出す程ではないため最初と最後OKだけ出力
	CString strProgressLogPrefix(strOutFileName.Left(strOutFileName.Find(_T("."))) + _T("変換"));
	CString strLogMsg;
	strLogMsg.Format(_T("  %s\r"), strProgressLogPrefix);
	std::cout << CT2CA(strLogMsg);

	// コード値ドメイン取得
	std::map<long, CString> mapDomain;
	if (!get(strCodeDomainName, mapDomain)) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errGetDomain, _T("コード値ドメイン取得失敗 [%s]"), strCodeDomainName);
		return false;
	}

	// 出力
	CFileEUC cFile;
	if (!cFile.open(m_strOutDirName, strOutFileName)) {
		return false;
	}
	for (const auto& itr : mapDomain)
	{
		std::ostringstream oss;
		oss << itr.first << "\t"	// コード
			<< CT2A(itr.second);	// 名称

		cFile.write(oss.str());
	}

	// 結果出力（runログと標準出力）
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