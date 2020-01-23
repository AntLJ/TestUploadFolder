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
#include <sindy/libschema.h>
#include "Param.h"
#include "LogSys.h"
#include "DBUtils.h"
#include "Addr2TsvBase.h"
#include "FileEUC.h"
#include "Adrp.h"

//-----------
// CAdrpBase
//-----------
CAdrpBase::CAdrpBase(const CParam& cParam, const IWorkspacePtr& ipWS)
{
	setWorkspace(ipWS);
	setOwner(CString(cParam.getAddrOwner().c_str()));
	setOutDirName(CString(cParam.getOutDir().c_str()));
}

bool CAdrpBase::init(void)
{
	if (m_strOutDirName.IsEmpty() || m_strOutFileName.IsEmpty())
		return false;	// バグ以外ありえないルート

	if (!CAddr2TsvBase::init())
		return false;

	if (!m_cFile.open(m_strOutDirName, m_strOutFileName))
		return false;

	return true;
}

bool CAdrpBase::execute(void)
{
	if (!CAddr2TsvBase::execute())
		return false;

	m_cFile.close();
	return true;
}

//-----------
// CAdrp001
//-----------
CAdrp001::CAdrp001(const CParam& cParam, const IWorkspacePtr& ipWS)
	: CAdrpBase(cParam, ipWS), m_idxPrefCode(-1), m_idxName(-1), m_idxYomi(-1)
{
	setTableName(C_ADRP_TABLE_001);
	setSortField(C_ADRP_SORT_001);
	setOutFileName(C_ADRP_FILE_001);
	setProgressLogPrefix(_T("ADRP001変換"));
}

bool CAdrp001::init(void)
{
	if (!CAdrpBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::pref_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::pref_point::kName, m_idxName) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::pref_point::kYomi, m_idxYomi))
		return false;

	return true;
}

bool CAdrp001::convRecord(const _IRowPtr& ipRow)
{
	CString strPrefCode, strName, strYomi;
	if (!getValue(ipRow, sindy::schema::pref_point::kPrefCode, m_idxPrefCode, strPrefCode) ||
		!getValue(ipRow, sindy::schema::pref_point::kName, m_idxName, strName) ||
		!getValue(ipRow, sindy::schema::pref_point::kYomi, m_idxYomi, strYomi))
		return false;

	int mesh = 0, x = 0, y = 0;
	if (!getMeshXY((IFeaturePtr)ipRow, mesh, x, y))
		return false;

	std::ostringstream oss;
	oss << CT2A(strPrefCode) << "\t"	// 都道府県コード
		<< mesh << "\t"					// 2次メッシュコード
		<< x << "\t"					// X座標
		<< y << "\t"					// Y座標
		<< CT2A(strName) << "\t"		// 名称
		<< CT2A(strYomi);				// 読み

	m_cFile.write(oss.str());

	// 件数表用にコードと名称の対応リスト確保
	m_codeNameList.insert(std::pair<CString, CString>(strPrefCode, strName));

	return true;
}

//-----------
// CAdrp002
//-----------
CAdrp002::CAdrp002(const CParam& cParam, const IWorkspacePtr& ipWS)
	: CAdrpBase(cParam, ipWS), m_idxPrefCode(-1), m_idxCityCode(-1), m_idxName(-1), m_idxYomi(-1), m_idxAdminAtt(-1)
{
	setTableName(C_ADRP_TABLE_002);
	setSortField(C_ADRP_SORT_002);
	setOutFileName(C_ADRP_FILE_002);
	setProgressLogPrefix(_T("ADRP002変換"));
}

bool CAdrp002::init(void)
{
	if (!CAdrpBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::city_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::city_point::kCityCode, m_idxCityCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::city_point::kName, m_idxName) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::city_point::kYomi, m_idxYomi) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::city_point::kAdminAtt, m_idxAdminAtt))
		return false;

	return true;
}

bool CAdrp002::convRecord(const _IRowPtr& ipRow)
{
	CString strPrefCode, strCityCode, strName, strYomi, strAdminAtt;
	if (!getValue(ipRow, sindy::schema::city_point::kPrefCode, m_idxPrefCode, strPrefCode) ||
		!getValue(ipRow, sindy::schema::city_point::kCityCode, m_idxCityCode, strCityCode) ||
		!getValue(ipRow, sindy::schema::city_point::kName, m_idxName, strName) ||
		!getValue(ipRow, sindy::schema::city_point::kYomi, m_idxYomi, strYomi) ||
		!getValue(ipRow, sindy::schema::city_point::kAdminAtt, m_idxAdminAtt, strAdminAtt))
		return false;

	int mesh = 0, x = 0, y = 0;
	if (!getMeshXY((IFeaturePtr)ipRow, mesh, x, y))
		return false;

	std::ostringstream oss;
	oss << CT2A(strPrefCode) << "\t"	// 都道府県コード
		<< CT2A(strCityCode) << "\t"	// 市区町村コード
		<< mesh << "\t"					// 2次メッシュコード
		<< x << "\t"					// X座標
		<< y << "\t"					// Y座標
		<< CT2A(strName) << "\t"		// 名称
		<< CT2A(strYomi) << "\t"		// 読み
		<< CT2A(strAdminAtt);			// 行政区域種別コード

	m_cFile.write(oss.str());

	// 5桁コードリスト確保
	m_codeList.insert(std::pair<CString, CString>(strPrefCode, strCityCode));

	// 件数表用にコードと名称の対応リスト確保
	m_codeNameList.insert(std::make_pair(std::make_pair(strPrefCode, strCityCode), strName));

	return true;
}

//-----------
// CAdrp003
//-----------
CAdrp003::CAdrp003(const CParam& cParam, const IWorkspacePtr& ipWS)
	: CAdrpBase(cParam, ipWS), m_idxPrefCode(-1), m_idxCityCode(-1), m_idxAddrCode1(-1), m_idxName(-1), m_idxYomi(-1),
	m_idxPrefix(-1), m_idxSuffix(-1), m_idxNameCode(-1), m_idxAddrDisp(-1)
{
	setTableName(C_ADRP_TABLE_003);
	setSortField(C_ADRP_SORT_003);
	setOutFileName(C_ADRP_FILE_003);
	setProgressLogPrefix(_T("ADRP003変換"));
}

bool CAdrp003::init(void)
{
	if (!CAdrpBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::oaza_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::oaza_point::kCityCode, m_idxCityCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::oaza_point::kAddrCode1, m_idxAddrCode1) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::oaza_point::kName, m_idxName) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::oaza_point::kYomi, m_idxYomi) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::oaza_point::kPrefix, m_idxPrefix) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::oaza_point::kSuffix, m_idxSuffix) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::oaza_point::kNameCode, m_idxNameCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::oaza_point::kAddrDisp, m_idxAddrDisp))
		return false;

	return true;
}

bool CAdrp003::convRecord(const _IRowPtr& ipRow)
{
	CString strPrefCode, strCityCode, strAddrCode1, strName, strYomi, strPrefix, strSuffix, strNameCode, strAddrDisp;
	if (!getValue(ipRow, sindy::schema::oaza_point::kPrefCode, m_idxPrefCode, strPrefCode) ||
		!getValue(ipRow, sindy::schema::oaza_point::kCityCode, m_idxCityCode, strCityCode) ||
		!getValue(ipRow, sindy::schema::oaza_point::kAddrCode1, m_idxAddrCode1, strAddrCode1) ||
		!getValue(ipRow, sindy::schema::oaza_point::kName, m_idxName, strName) ||
		!getValue(ipRow, sindy::schema::oaza_point::kYomi, m_idxYomi, strYomi) ||
		!getValue(ipRow, sindy::schema::oaza_point::kPrefix, m_idxPrefix, strPrefix) ||
		!getValue(ipRow, sindy::schema::oaza_point::kSuffix, m_idxSuffix, strSuffix) ||
		!getValue(ipRow, sindy::schema::oaza_point::kNameCode, m_idxNameCode, strNameCode) ||
		!getValue(ipRow, sindy::schema::oaza_point::kAddrDisp, m_idxAddrDisp, strAddrDisp))
		return false;

	int mesh = 0, x = 0, y = 0;
	if (!getMeshXY((IFeaturePtr)ipRow, mesh, x, y))
		return false;

	std::ostringstream oss;
	oss << CT2A(strPrefCode) << "\t"	// 都道府県コード
		<< CT2A(strCityCode) << "\t"	// 市区町村コード
		<< CT2A(strAddrCode1) << "\t"	// 町・大字コード
		<< mesh << "\t"					// 2次メッシュコード
		<< x << "\t"					// X座標
		<< y << "\t"					// Y座標
		<< CT2A(strName) << "\t"		// 名称
		<< CT2A(strYomi) << "\t"		// 読み
		<< CT2A(strPrefix) << "\t"		// 接頭辞種別コード
		<< CT2A(strSuffix) << "\t"		// 接尾辞種別コード
		<< CT2A(strNameCode) << "\t"	// 公称通称種別コード
		<< CT2A(strAddrDisp);			// 住居表示区域コード

	m_cFile.write(oss.str());

	// 件数表用に5桁コード単位で件数カウント
	auto ret = m_codeCountList.insert(std::make_pair(std::make_pair(strPrefCode, strCityCode), 0));
	++((ret.first)->second);

	return true;
}

//-----------
// CAdrp004
//-----------
CAdrp004::CAdrp004(const CParam& cParam, const IWorkspacePtr& ipWS)
	: CAdrpBase(cParam, ipWS), m_idxPrefCode(-1), m_idxCityCode(-1), m_idxAddrCode1(-1), m_idxAddrCode2(-1), m_idxName(-1), m_idxYomi(-1),
	m_idxPrefix(-1), m_idxSuffix(-1), m_idxNameCode(-1), m_idxAddrDisp(-1)
{
	setTableName(C_ADRP_TABLE_004);
	setSortField(C_ADRP_SORT_004);
	setOutFileName(C_ADRP_FILE_004);
	setProgressLogPrefix(_T("ADRP004変換"));
}

bool CAdrp004::init(void)
{
	if (!CAdrpBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::aza_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::aza_point::kCityCode, m_idxCityCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::aza_point::kAddrCode1, m_idxAddrCode1) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::aza_point::kAddrCode2, m_idxAddrCode2) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::aza_point::kName, m_idxName) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::aza_point::kYomi, m_idxYomi) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::aza_point::kPrefix, m_idxPrefix) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::aza_point::kSuffix, m_idxSuffix) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::aza_point::kNameCode, m_idxNameCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::aza_point::kAddrDisp, m_idxAddrDisp))
		return false;

	return true;
}

bool CAdrp004::convRecord(const _IRowPtr& ipRow)
{
	CString strPrefCode, strCityCode, strAddrCode1, strAddrCode2, strName, strYomi, strPrefix, strSuffix, strNameCode, strAddrDisp;
	if (!getValue(ipRow, sindy::schema::aza_point::kPrefCode, m_idxPrefCode, strPrefCode) ||
		!getValue(ipRow, sindy::schema::aza_point::kCityCode, m_idxCityCode, strCityCode) ||
		!getValue(ipRow, sindy::schema::aza_point::kAddrCode1, m_idxAddrCode1, strAddrCode1) ||
		!getValue(ipRow, sindy::schema::aza_point::kAddrCode2, m_idxAddrCode2, strAddrCode2) ||
		!getValue(ipRow, sindy::schema::aza_point::kName, m_idxName, strName) ||
		!getValue(ipRow, sindy::schema::aza_point::kYomi, m_idxYomi, strYomi) ||
		!getValue(ipRow, sindy::schema::aza_point::kPrefix, m_idxPrefix, strPrefix) ||
		!getValue(ipRow, sindy::schema::aza_point::kSuffix, m_idxSuffix, strSuffix) ||
		!getValue(ipRow, sindy::schema::aza_point::kNameCode, m_idxNameCode, strNameCode) ||
		!getValue(ipRow, sindy::schema::aza_point::kAddrDisp, m_idxAddrDisp, strAddrDisp))
		return false;

	int mesh = 0, x = 0, y = 0;
	if (!getMeshXY((IFeaturePtr)ipRow, mesh, x, y))
		return false;

	std::ostringstream oss;
	oss << CT2A(strPrefCode) << "\t"	// 都道府県コード
		<< CT2A(strCityCode) << "\t"	// 市区町村コード
		<< CT2A(strAddrCode1) << "\t"	// 町・大字コード
		<< CT2A(strAddrCode2) << "\t"	// 丁目・字コード
		<< mesh << "\t"					// 2次メッシュコード
		<< x << "\t"					// X座標
		<< y << "\t"					// Y座標
		<< CT2A(strName) << "\t"		// 名称
		<< CT2A(strYomi) << "\t"		// 読み
		<< CT2A(strPrefix) << "\t"		// 接頭辞種別コード
		<< CT2A(strSuffix) << "\t"		// 接尾辞種別コード
		<< CT2A(strNameCode) << "\t"	// 公称通称種別コード
		<< CT2A(strAddrDisp);			// 住居表示区域コード

	m_cFile.write(oss.str());

	// 件数表用に5桁コード単位で件数カウント
	auto ret = m_codeCountList.insert(std::make_pair(std::make_pair(strPrefCode, strCityCode), 0));
	++((ret.first)->second);

	return true;
}


//---------------
// CAdrpListBase
//---------------
CAdrpListBase::CAdrpListBase(const CParam& cParam, const IWorkspacePtr& ipWS, const std::set<std::pair<CString, CString>>& codeList)
	:m_codeList(codeList)
{
	setWorkspace(ipWS);
	setOwner(CString(cParam.getAddrOwner().c_str()));
	setOutDirName(CString(cParam.getOutDir().c_str()));
}

bool CAdrpListBase::init(void)
{
	if (m_strOutDirName.IsEmpty() || m_strOutFileName.IsEmpty())
		return false;	// バグ以外ありえないルート

	if (!CAddr2TsvBase::init())
		return false;

	if (!m_cFile.open(m_strOutDirName, m_strOutFileName))
		return false;

	return true;
}

bool CAdrpListBase::execute(void)
{
	// 進捗率出力（標準出力）
	long lCount = m_codeList.size();
	long lEndCount = 0;
	long lPrePercentage = 0;
	CString strLogMsg;
	strLogMsg.Format(_T("  %s 0%%\r"), m_strProgressLogPrefix);
	std::cout << CT2CA(strLogMsg);

	for (const auto& itr : m_codeList)
	{
		// 検索
		CString strWhere;
		strWhere.Format(m_strWhereFormat, itr.first, itr.second);
		if (!CAddr2TsvBase::execute(false, strWhere))
			return false;

		// 出力
		output();

		// 進捗率出力（標準出力）
		++lEndCount;
		long lPercentage = (long)floor((((double)lEndCount / (double)lCount) * 100.0));
		if (lPrePercentage != lPercentage) {
			strLogMsg.Format(_T("  %s %ld%%\r"), m_strProgressLogPrefix, lPercentage);
			std::cout << CT2CA(strLogMsg);
		}
		lPrePercentage = lPercentage;
	}
	// 結果出力（runログと標準出力）
	strLogMsg.Format(_T("  %s ... OK\n"), m_strProgressLogPrefix);
	CLogSys::GetInstance().WriteRunLog(true, false, strLogMsg);

	m_cFile.close();
	return true;
}

//-----------
// CAdrp005
//-----------
CAdrp005::CAdrp005(const CParam& cParam, const IWorkspacePtr& ipWS, const std::set<std::pair<CString, CString>>& codeList)
	: CAdrpListBase(cParam, ipWS, codeList),
	m_idxPrefCode(-1), m_idxCityCode(-1), m_idxAddrCode1(-1), m_idxAddrCode2(-1), m_idxChibanCode(-1), m_idxBldOID(-1), m_idxName(-1), m_idxYomi(-1),
	m_idxStatus(-1), m_idxPntType(-1), m_idxAddrDisp(-1), m_idxRef(-1), m_idxBranch(-1), m_idxMulti(-1), m_idxGnrtType(-1)
{
	setTableName(C_ADRP_TABLE_005);
	setOutFileName(C_ADRP_FILE_005);
	setProgressLogPrefix(_T("ADRP005変換"));
	CString strWhere;
	strWhere.Format(_T("%s = '%%s' AND %s = '%%s'"), sindy::schema::main_no_point::kPrefCode, sindy::schema::main_no_point::kCityCode);
	setWhereFormat(strWhere);
}

bool CAdrp005::init(void)
{
	if (!CAdrpListBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kCityCode, m_idxCityCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kAddrCode1, m_idxAddrCode1) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kAddrCode2, m_idxAddrCode2) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kChibanCode, m_idxChibanCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kBldOID, m_idxBldOID) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kName, m_idxName) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kYomi, m_idxYomi) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kStatus, m_idxStatus) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kPntType, m_idxPntType) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kAddrDisp, m_idxAddrDisp) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kRep, m_idxRef) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kBranch, m_idxBranch) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kMulti, m_idxMulti) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::main_no_point::kGnrtType, m_idxGnrtType))
		return false;

	return true;
}

bool CAdrp005::convRecord(const _IRowPtr& ipRow)
{
	struct CAdrpData005 stData;
	if (!getValue(ipRow, sindy::schema::main_no_point::kPrefCode, m_idxPrefCode, stData.strPrefCode) ||
		!getValue(ipRow, sindy::schema::main_no_point::kCityCode, m_idxCityCode, stData.strCityCode) ||
		!getValue(ipRow, sindy::schema::main_no_point::kAddrCode1, m_idxAddrCode1, stData.strAddrCode1) ||
		!getValue(ipRow, sindy::schema::main_no_point::kAddrCode2, m_idxAddrCode2, stData.strAddrCode2) ||
		!getValue(ipRow, sindy::schema::main_no_point::kChibanCode, m_idxChibanCode, stData.strChibanCode) ||
		!getValue(ipRow, sindy::schema::main_no_point::kBldOID, m_idxBldOID, stData.strBldOID) ||
		!getValue(ipRow, sindy::schema::main_no_point::kName, m_idxName, stData.strName) ||
		!getValue(ipRow, sindy::schema::main_no_point::kYomi, m_idxYomi, stData.strYomi) ||
		!getValue(ipRow, sindy::schema::main_no_point::kStatus, m_idxStatus, stData.strStatus) ||
		!getValue(ipRow, sindy::schema::main_no_point::kPntType, m_idxPntType, stData.strPntType) ||
		!getValue(ipRow, sindy::schema::main_no_point::kAddrDisp, m_idxAddrDisp, stData.strAddrDisp) ||
		!getValue(ipRow, sindy::schema::main_no_point::kRep, m_idxRef, stData.strRef) ||
		!getValue(ipRow, sindy::schema::main_no_point::kBranch, m_idxBranch, stData.strBranch) ||
		!getValue(ipRow, sindy::schema::main_no_point::kMulti, m_idxMulti, stData.strMulti) ||
		!getValue(ipRow, sindy::schema::main_no_point::kGnrtType, m_idxGnrtType, stData.strGnrtType))
		return false;

	// BLD_OIDはNULL許可のため文字列で保持するがソート時は数値で比較するため数値も保持（NULLの場合は初期値0xFFFFFFFFのまま）
	if (!stData.strBldOID.IsEmpty())
		stData.lBldOID = _ttol(stData.strBldOID);

	if (!getMeshXY((IFeaturePtr)ipRow, stData.mesh, stData.x, stData.y))
		return false;

	m_outData.push_back(stData);

	// 件数表用に5桁コード単位で件数カウント
	std::vector<long> vecValue(3, 0);
	auto ret = m_codeCountList.insert(std::make_pair(std::make_pair(stData.strPrefCode, stData.strCityCode), vecValue));
	// std::vectorの0番目：番地            ：街区符号・地番コードが数値だけのレコード件数
	// std::vectorの1番目：番地(文字付)    ：街区符号・地番コードが数値以外を含むレコード件数
	// std::vectorの2番目：番地のうち符号付：支号フラグが1のレコード件数
	if (stData.strChibanCode.SpanIncluding(_T("0123456789")) == stData.strChibanCode)
		++((ret.first)->second)[0];
	else
		++((ret.first)->second)[1];

	if (stData.strBranch == _T("1"))
		++((ret.first)->second)[2];

	return true;
}

void CAdrp005::output()
{
	// 出力用コンテナをソート
	std::sort(m_outData.begin(), m_outData.end());

	// 出力ファイルに出力
	for (const auto& itrData : m_outData) {
		std::ostringstream oss;
		oss << CT2A(itrData.strPrefCode) << "\t"	// 都道府県コード
			<< CT2A(itrData.strCityCode) << "\t"	// 市区町村コード
			<< CT2A(itrData.strAddrCode1) << "\t"	// 町・大字コード
			<< CT2A(itrData.strAddrCode2) << "\t"	// 丁目・字コード
			<< CT2A(itrData.strChibanCode) << "\t"	// 街区符号・地番コード
			<< CT2A(itrData.strBldOID) << "\t"		// 建物ID
			<< itrData.mesh << "\t"					// 2次メッシュコード
			<< itrData.x << "\t"					// X座標
			<< itrData.y << "\t"					// Y座標
			<< CT2A(itrData.strName) << "\t"		// 名称
			<< CT2A(itrData.strYomi) << "\t"		// 読み
			<< CT2A(itrData.strStatus) << "\t"		// 地番状態コード
			<< CT2A(itrData.strPntType) << "\t"		// 街区・地番識別コード
			<< CT2A(itrData.strAddrDisp) << "\t"	// 住居表示区域コード
			<< CT2A(itrData.strRef) << "\t"			// 代表点フラグ
			<< CT2A(itrData.strBranch) << "\t"		// 支号フラグ
			<< CT2A(itrData.strMulti) << "\t"		// 複数住所フラグ
			<< CT2A(itrData.strGnrtType);			// 生成データ種別
		m_cFile.write(oss.str());
	}

	// 出力用コンテナをクリア
	m_outData.clear();
}

//-----------
// CAdrp006
//-----------
CAdrp006::CAdrp006(const CParam& cParam, const IWorkspacePtr& ipWS, const std::set<std::pair<CString, CString>>& codeList)
	: CAdrpListBase(cParam, ipWS, codeList),
	m_idxPrefCode(-1), m_idxCityCode(-1), m_idxAddrCode1(-1), m_idxAddrCode2(-1), m_idxChibanCode(-1), m_idxJukyoCode(-1), m_idxBldOID(-1),
	m_idxName(-1), m_idxYomi(-1), m_idxRef(-1), m_idxBranch(-1), m_idxMulti(-1), m_idxGnrtType(-1)
{
	setTableName(C_ADRP_TABLE_006);
	setOutFileName(C_ADRP_FILE_006);
	setProgressLogPrefix(_T("ADRP006変換"));
	CString strWhere;
	strWhere.Format(_T("%s = '%%s' AND %s = '%%s'"), sindy::schema::sub_no_point::kPrefCode, sindy::schema::sub_no_point::kCityCode);
	setWhereFormat(strWhere);
}

bool CAdrp006::init(void)
{
	if (!CAdrpListBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kCityCode, m_idxCityCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kAddrCode1, m_idxAddrCode1) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kAddrCode2, m_idxAddrCode2) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kChibanCode, m_idxChibanCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kJukyoCode, m_idxJukyoCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kBldOID, m_idxBldOID) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kName, m_idxName) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kYomi, m_idxYomi) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kRep, m_idxRef) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kBranch, m_idxBranch) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kMulti, m_idxMulti) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::sub_no_point::kGnrtType, m_idxGnrtType))
		return false;

	return true;
}

bool CAdrp006::convRecord(const _IRowPtr& ipRow)
{
	struct CAdrpData006 stData;
	if (!getValue(ipRow, sindy::schema::sub_no_point::kPrefCode, m_idxPrefCode, stData.strPrefCode) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kCityCode, m_idxCityCode, stData.strCityCode) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kAddrCode1, m_idxAddrCode1, stData.strAddrCode1) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kAddrCode2, m_idxAddrCode2, stData.strAddrCode2) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kChibanCode, m_idxChibanCode, stData.strChibanCode) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kJukyoCode, m_idxJukyoCode, stData.strJukyoCode) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kBldOID, m_idxBldOID, stData.strBldOID) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kName, m_idxName, stData.strName) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kYomi, m_idxYomi, stData.strYomi) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kRep, m_idxRef, stData.strRef) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kBranch, m_idxBranch, stData.strBranch) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kMulti, m_idxMulti, stData.strMulti) ||
		!getValue(ipRow, sindy::schema::sub_no_point::kGnrtType, m_idxGnrtType, stData.strGnrtType))
		return false;

	// BLD_OIDはNULL許可のため文字列で保持するがソート時は数値で比較するため数値も保持（NULLの場合は初期値0xFFFFFFFFのまま）
	if (!stData.strBldOID.IsEmpty())
		stData.lBldOID = _ttol(stData.strBldOID);

	if (!getMeshXY((IFeaturePtr)ipRow, stData.mesh, stData.x, stData.y))
		return false;

	m_outData.push_back(stData);

	// 件数表用に5桁コード単位で件数カウント
	std::vector<long> vecValue(3, 0);
	auto ret = m_codeCountList.insert(std::make_pair(std::make_pair(stData.strPrefCode, stData.strCityCode), vecValue));
	// std::vectorの0番目：号            ：街区符号・地番コードと住居番号・枝番コードが数値だけのレコード件数
	// std::vectorの1番目：号(文字付)    ：街区符号・地番コードと住居番号・枝番コードのどちらかに数値以外を含むレコード件数
	// std::vectorの2番目：号のうち枝番付：枝番フラグが1のレコード件数
	if (stData.strChibanCode.SpanIncluding(_T("0123456789")) == stData.strChibanCode &&
		stData.strJukyoCode.SpanIncluding(_T("0123456789")) == stData.strJukyoCode)
		++((ret.first)->second)[0];
	else
		++((ret.first)->second)[1];

	if (stData.strBranch == _T("1"))
		++((ret.first)->second)[2];

	return true;
}

void CAdrp006::output()
{
	// 出力用コンテナをソート
	std::sort(m_outData.begin(), m_outData.end());

	// 出力ファイルに出力
	for (const auto& itrData : m_outData) {
		std::ostringstream oss;
		oss << CT2A(itrData.strPrefCode) << "\t"	// 都道府県コード
			<< CT2A(itrData.strCityCode) << "\t"	// 市区町村コード
			<< CT2A(itrData.strAddrCode1) << "\t"	// 町・大字コード
			<< CT2A(itrData.strAddrCode2) << "\t"	// 丁目・字コード
			<< CT2A(itrData.strChibanCode) << "\t"	// 街区符号・地番コード
			<< CT2A(itrData.strJukyoCode) << "\t"	// 住居番号・枝番コード
			<< CT2A(itrData.strBldOID) << "\t"		// 建物ID
			<< itrData.mesh << "\t"					// 2次メッシュコード
			<< itrData.x << "\t"					// X座標
			<< itrData.y << "\t"					// Y座標
			<< CT2A(itrData.strName) << "\t"		// 名称
			<< CT2A(itrData.strYomi) << "\t"		// 読み
			<< CT2A(itrData.strRef) << "\t"			// 代表点フラグ
			<< CT2A(itrData.strBranch) << "\t"		// 支号フラグ
			<< CT2A(itrData.strMulti) << "\t"		// 複数住所フラグ
			<< CT2A(itrData.strGnrtType);			// 生成データ種別
		m_cFile.write(oss.str());
	}

	// 出力用コンテナをクリア
	m_outData.clear();
}