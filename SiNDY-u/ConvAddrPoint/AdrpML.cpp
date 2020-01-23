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
#include "DBUtils.h"
#include "Addr2TsvBase.h"
#include "FileUTF8.h"
#include "AdrpML.h"
#include <rns/sstream.h>


//-------------
// CAdrpMLBase
//-------------
CAdrpMLBase::CAdrpMLBase(const CParam& cParam, const IWorkspacePtr& ipWS)
{
	setWorkspace(ipWS);
	setOwner(CString(cParam.getMlangOwner().c_str()));
	setOutDirName(CString(cParam.getOutDir().c_str()));
}

bool CAdrpMLBase::init(void)
{
	if (m_strOutDirName.IsEmpty() || m_strOutFileName.IsEmpty())
		return false;	// バグ以外ありえないルート

	if (!CAddr2TsvBase::init())
		return false;

	if (!m_cFile.open(m_strOutDirName, m_strOutFileName))
		return false;

	return true;
}

bool CAdrpMLBase::execute(void)
{
	if (!CAddr2TsvBase::execute())
		return false;

	m_cFile.close();
	return true;
}

//-------------
// CAdrp001ML
//-------------
CAdrp001ML::CAdrp001ML(const CParam& cParam, const IWorkspacePtr& ipWS)	: CAdrpMLBase(cParam, ipWS)
{
	setTableName(C_ADRPML_TABLE_001);
	setSortField(C_ADRPML_SORT_001);
	setOutFileName(C_ADRPML_FILE_001);
	setProgressLogPrefix(_T("ADRP001ML変換"));
}

CAdrp001ML::~CAdrp001ML(void)
{
}

bool CAdrp001ML::init(void)
{
	if (!CAdrpMLBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::pref_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::pref_point::kLangCode, m_idxLangCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::pref_point::kName, m_idxName))
		return false;

	return true;
}

bool CAdrp001ML::convRecord(const _IRowPtr& ipRow)
{
	CString strPrefCode, strLangCode, strName;
	if (!getValue(ipRow, sindy::schema::multi_language::pref_point::kPrefCode, m_idxPrefCode, strPrefCode) ||
		!getValue(ipRow, sindy::schema::multi_language::pref_point::kLangCode, m_idxLangCode, strLangCode) ||
		!getValue(ipRow, sindy::schema::multi_language::pref_point::kName, m_idxName, strName))
		return false;

	rns::tostringstream toss;
	toss << (LPCTSTR)strPrefCode << _T("\t")	// 都道府県コード
		<< (LPCTSTR)strLangCode << _T("\t")		// 言語コード
		<< (LPCTSTR)strName;					// 名称

	m_cFile.write(toss.str());
	return true;
}

//-------------
// CAdrp002ML
//-------------
CAdrp002ML::CAdrp002ML(const CParam& cParam, const IWorkspacePtr& ipWS) : CAdrpMLBase(cParam, ipWS)
{
	setTableName(C_ADRPML_TABLE_002);
	setSortField(C_ADRPML_SORT_002);
	setOutFileName(C_ADRPML_FILE_002);
	setProgressLogPrefix(_T("ADRP002ML変換"));
}

CAdrp002ML::~CAdrp002ML(void)
{
}

bool CAdrp002ML::init(void)
{
	if (!CAdrpMLBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::city_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::city_point::kCityCode, m_idxCityCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::city_point::kLangCode, m_idxLangCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::city_point::kName, m_idxName))
		return false;

	return true;
}

bool CAdrp002ML::convRecord(const _IRowPtr& ipRow)
{
	CString strPrefCode, strCityCode, strLangCode, strName;
	if (!getValue(ipRow, sindy::schema::multi_language::city_point::kPrefCode, m_idxPrefCode, strPrefCode) ||
		!getValue(ipRow, sindy::schema::multi_language::city_point::kCityCode, m_idxCityCode, strCityCode) ||
		!getValue(ipRow, sindy::schema::multi_language::city_point::kLangCode, m_idxLangCode, strLangCode) ||
		!getValue(ipRow, sindy::schema::multi_language::city_point::kName, m_idxName, strName))
		return false;

	rns::tostringstream toss;
	toss << (LPCTSTR)strPrefCode << _T("\t")	// 都道府県コード
		<< (LPCTSTR)strCityCode << _T("\t")		// 市区町村コード
		<< (LPCTSTR)strLangCode << _T("\t")		// 言語コード
		<< (LPCTSTR)strName;					// 名称

	m_cFile.write(toss.str());
	return true;
}

//-------------
// CAdrp003ML
//-------------
CAdrp003ML::CAdrp003ML(const CParam& cParam, const IWorkspacePtr& ipWS) : CAdrpMLBase(cParam, ipWS)
{
	setTableName(C_ADRPML_TABLE_003);
	setSortField(C_ADRPML_SORT_003);
	setOutFileName(C_ADRPML_FILE_003);
	setProgressLogPrefix(_T("ADRP003ML変換"));
}

CAdrp003ML::~CAdrp003ML(void)
{
}

bool CAdrp003ML::init(void)
{
	if (!CAdrpMLBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::oaza_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::oaza_point::kCityCode, m_idxCityCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::oaza_point::kAddrCode1, m_idxAddrCode1) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::oaza_point::kLangCode, m_idxLangCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::oaza_point::kName, m_idxName))
		return false;

	return true;
}

bool CAdrp003ML::convRecord(const _IRowPtr& ipRow)
{
	CString strPrefCode, strCityCode, strAddrCode1, strLangCode, strName;
	if (!getValue(ipRow, sindy::schema::multi_language::oaza_point::kPrefCode, m_idxPrefCode, strPrefCode) ||
		!getValue(ipRow, sindy::schema::multi_language::oaza_point::kCityCode, m_idxCityCode, strCityCode) ||
		!getValue(ipRow, sindy::schema::multi_language::oaza_point::kAddrCode1, m_idxAddrCode1, strAddrCode1) ||
		!getValue(ipRow, sindy::schema::multi_language::oaza_point::kLangCode, m_idxLangCode, strLangCode) ||
		!getValue(ipRow, sindy::schema::multi_language::oaza_point::kName, m_idxName, strName))
		return false;

	rns::tostringstream toss;
	toss << (LPCTSTR)strPrefCode << _T("\t")	// 都道府県コード
		<< (LPCTSTR)strCityCode << _T("\t")		// 市区町村コード
		<< (LPCTSTR)strAddrCode1 << _T("\t")	// 町・大字コード
		<< (LPCTSTR)strLangCode << _T("\t")		// 言語コード
		<< (LPCTSTR)strName;					// 名称

	m_cFile.write(toss.str());
	return true;
}

//-------------
// CAdrp004ML
//-------------
CAdrp004ML::CAdrp004ML(const CParam& cParam, const IWorkspacePtr& ipWS) : CAdrpMLBase(cParam, ipWS)
{
	setTableName(C_ADRPML_TABLE_004);
	setSortField(C_ADRPML_SORT_004);
	setOutFileName(C_ADRPML_FILE_004);
	setProgressLogPrefix(_T("ADRP004ML変換"));
}

CAdrp004ML::~CAdrp004ML(void)
{
}

bool CAdrp004ML::init(void)
{
	if (!CAdrpMLBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::aza_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::aza_point::kCityCode, m_idxCityCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::aza_point::kAddrCode1, m_idxAddrCode1) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::aza_point::kAddrCode2, m_idxAddrCode2) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::aza_point::kLangCode, m_idxLangCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::aza_point::kName, m_idxName))
		return false;

	return true;
}

bool CAdrp004ML::convRecord(const _IRowPtr& ipRow)
{
	CString strPrefCode, strCityCode, strAddrCode1, strAddrCode2, strLangCode, strName;
	if (!getValue(ipRow, sindy::schema::multi_language::aza_point::kPrefCode, m_idxPrefCode, strPrefCode) ||
		!getValue(ipRow, sindy::schema::multi_language::aza_point::kCityCode, m_idxCityCode, strCityCode) ||
		!getValue(ipRow, sindy::schema::multi_language::aza_point::kAddrCode1, m_idxAddrCode1, strAddrCode1) ||
		!getValue(ipRow, sindy::schema::multi_language::aza_point::kAddrCode2, m_idxAddrCode2, strAddrCode2) ||
		!getValue(ipRow, sindy::schema::multi_language::aza_point::kLangCode, m_idxLangCode, strLangCode) ||
		!getValue(ipRow, sindy::schema::multi_language::aza_point::kName, m_idxName, strName))
		return false;

	rns::tostringstream toss;
	toss << (LPCTSTR)strPrefCode << _T("\t")	// 都道府県コード
		<< (LPCTSTR)strCityCode << _T("\t")		// 市区町村コード
		<< (LPCTSTR)strAddrCode1 << _T("\t")	// 町・大字コード
		<< (LPCTSTR)strAddrCode2 << _T("\t")	// 丁目・字コード
		<< (LPCTSTR)strLangCode << _T("\t")		// 言語コード
		<< (LPCTSTR)strName;					// 名称

	m_cFile.write(toss.str());
	return true;
}

//-------------
// CAdrp005ML
//-------------
CAdrp005ML::CAdrp005ML(const CParam& cParam, const IWorkspacePtr& ipWS) : CAdrpMLBase(cParam, ipWS)
{
	setTableName(C_ADRPML_TABLE_005);
	setSortField(C_ADRPML_SORT_005);
	setOutFileName(C_ADRPML_FILE_005);
	setProgressLogPrefix(_T("ADRP005ML変換"));
}

CAdrp005ML::~CAdrp005ML(void)
{
}

bool CAdrp005ML::init(void)
{
	if (!CAdrpMLBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::main_no_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::main_no_point::kCityCode, m_idxCityCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::main_no_point::kAddrCode1, m_idxAddrCode1) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::main_no_point::kAddrCode2, m_idxAddrCode2) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::main_no_point::kChibanCode, m_idxChibanCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::main_no_point::kLangCode, m_idxLangCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::main_no_point::kName, m_idxName))
		return false;

	return true;
}

bool CAdrp005ML::convRecord(const _IRowPtr& ipRow)
{
	CString strPrefCode, strCityCode, strAddrCode1, strAddrCode2, strChibanCode, strLangCode, strName;
	if (!getValue(ipRow, sindy::schema::multi_language::main_no_point::kPrefCode, m_idxPrefCode, strPrefCode) ||
		!getValue(ipRow, sindy::schema::multi_language::main_no_point::kCityCode, m_idxCityCode, strCityCode) ||
		!getValue(ipRow, sindy::schema::multi_language::main_no_point::kAddrCode1, m_idxAddrCode1, strAddrCode1) ||
		!getValue(ipRow, sindy::schema::multi_language::main_no_point::kAddrCode2, m_idxAddrCode2, strAddrCode2) ||
		!getValue(ipRow, sindy::schema::multi_language::main_no_point::kChibanCode, m_idxChibanCode, strChibanCode) ||
		!getValue(ipRow, sindy::schema::multi_language::main_no_point::kLangCode, m_idxLangCode, strLangCode) ||
		!getValue(ipRow, sindy::schema::multi_language::main_no_point::kName, m_idxName, strName))
		return false;

	rns::tostringstream toss;
	toss << (LPCTSTR)strPrefCode << _T("\t")	// 都道府県コード
		<< (LPCTSTR)strCityCode << _T("\t")		// 市区町村コード
		<< (LPCTSTR)strAddrCode1 << _T("\t")	// 町・大字コード
		<< (LPCTSTR)strAddrCode2 << _T("\t")	// 丁目・字コード
		<< (LPCTSTR)strChibanCode << _T("\t")	// 街区符号・地番コード
		<< (LPCTSTR)strLangCode << _T("\t")		// 言語コード
		<< (LPCTSTR)strName;					// 名称

	m_cFile.write(toss.str());
	return true;
}

//-------------
// CAdrp006ML
//-------------
CAdrp006ML::CAdrp006ML(const CParam& cParam, const IWorkspacePtr& ipWS) : CAdrpMLBase(cParam, ipWS)
{
	setTableName(C_ADRPML_TABLE_006);
	setSortField(C_ADRPML_SORT_006);
	setOutFileName(C_ADRPML_FILE_006);
	setProgressLogPrefix(_T("ADRP006ML変換"));
}

CAdrp006ML::~CAdrp006ML(void)
{
}

bool CAdrp006ML::init(void)
{
	if (!CAdrpMLBase::init())
		return false;

	// フィールドINDEX取得
	if (!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::sub_no_point::kPrefCode, m_idxPrefCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::sub_no_point::kCityCode, m_idxCityCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::sub_no_point::kAddrCode1, m_idxAddrCode1) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::sub_no_point::kAddrCode2, m_idxAddrCode2) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::sub_no_point::kChibanCode, m_idxChibanCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::sub_no_point::kJukyoCode, m_idxJukyoCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::sub_no_point::kLangCode, m_idxLangCode) ||
		!getFieldIndex(m_fieldIndex, sindy::schema::multi_language::sub_no_point::kName, m_idxName))
		return false;

	return true;
}

bool CAdrp006ML::convRecord(const _IRowPtr& ipRow)
{
	CString strPrefCode, strCityCode, strAddrCode1, strAddrCode2, strChibanCode, strJukyoCode, strLangCode, strName;
	if (!getValue(ipRow, sindy::schema::multi_language::sub_no_point::kPrefCode, m_idxPrefCode, strPrefCode) ||
		!getValue(ipRow, sindy::schema::multi_language::sub_no_point::kCityCode, m_idxCityCode, strCityCode) ||
		!getValue(ipRow, sindy::schema::multi_language::sub_no_point::kAddrCode1, m_idxAddrCode1, strAddrCode1) ||
		!getValue(ipRow, sindy::schema::multi_language::sub_no_point::kAddrCode2, m_idxAddrCode2, strAddrCode2) ||
		!getValue(ipRow, sindy::schema::multi_language::sub_no_point::kChibanCode, m_idxChibanCode, strChibanCode) ||
		!getValue(ipRow, sindy::schema::multi_language::sub_no_point::kJukyoCode, m_idxJukyoCode, strJukyoCode) ||
		!getValue(ipRow, sindy::schema::multi_language::sub_no_point::kLangCode, m_idxLangCode, strLangCode) ||
		!getValue(ipRow, sindy::schema::multi_language::sub_no_point::kName, m_idxName, strName))
		return false;

	rns::tostringstream toss;
	toss << (LPCTSTR)strPrefCode << _T("\t")	// 都道府県コード
		<< (LPCTSTR)strCityCode << _T("\t")		// 市区町村コード
		<< (LPCTSTR)strAddrCode1 << _T("\t")	// 町・大字コード
		<< (LPCTSTR)strAddrCode2 << _T("\t")	// 丁目・字コード
		<< (LPCTSTR)strChibanCode << _T("\t")	// 街区符号・地番コード
		<< (LPCTSTR)strJukyoCode << _T("\t")	// 住居番号・枝番コード
		<< (LPCTSTR)strLangCode << _T("\t")		// 言語コード
		<< (LPCTSTR)strName;					// 名称

	m_cFile.write(toss.str());
	return true;
}
