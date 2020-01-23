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
#include "Param.h"
#include "LogSys.h"
#include "DBUtils.h"
#include "Addr2TsvBase.h"
#include "FileEUC.h"
#include "FileUTF8.h"
#include <sindy/libschema.h>
#include "Adrp.h"
#include "AdrpML.h"
#include "Zipp.h"
#include "Master.h"
#include "ConvAddrPoint.h"
#include "FileSJIS.h"

CConvAddrPoint::CConvAddrPoint(CParam& cParam) :m_cParam(cParam)
{
}


CConvAddrPoint::~CConvAddrPoint(void)
{
}

bool CConvAddrPoint::init(void)
{
	// 住所DB接続
	CString dbAddr(m_cParam.getAddrDB().c_str());
	if (!CDBUtils::initDB(dbAddr, m_ipWorkspaceAddr))
	{
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errConnectDB, _T("DB接続失敗 [%s]"), dbAddr);
		return false;
	}
	CLogSys::GetInstance().WriteRunLog(true, false, _T("住所DB接続 ... OK\n"));

	// 多言語DB接続
	CString dbMlang(m_cParam.getMlangDB().c_str());
	if (!dbMlang.IsEmpty()) {
		if (!CDBUtils::initDB(dbMlang, m_ipWorkspaceMlang))
		{
			CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errConnectDB, _T("DB接続失敗 [%s]"), dbMlang);
			return false;
		}
		CLogSys::GetInstance().WriteRunLog(true, false, _T("多言語DB接続 ... OK\n"));
	}

	return true;
}

bool CConvAddrPoint::execute(void)
{
	bool bResult = true;
	// 処理タイプで分岐
	switch (m_cParam.getEType())
	{
	case outtype::adr_authdb:
		bResult = adr_authdb();
		break;
	case outtype::pos_authdb:
		bResult = pos_authdb();
		break;
	case outtype::adrmst_authdb:
		bResult = adrmst_authdb();
		break;
	default:
		break;
	}

	return bResult;
}

bool CConvAddrPoint::adr_authdb(void)
{
	CLogSys::GetInstance().WriteRunLog(true, false, _T("編集用DB向け-住所ポイント変換\n"));

	CAdrp001 adrp001(m_cParam, m_ipWorkspaceAddr);
	CAdrp002 adrp002(m_cParam, m_ipWorkspaceAddr);
	CAdrp003 adrp003(m_cParam, m_ipWorkspaceAddr);
	CAdrp004 adrp004(m_cParam, m_ipWorkspaceAddr);
	if (!adrp001.init() || !adrp001.execute())
		return false;
	if (!adrp002.init() || !adrp002.execute())
		return false;
	if (!adrp003.init() || !adrp003.execute())
		return false;
	if (!adrp004.init() || !adrp004.execute())
		return false;

	// 5桁コードリスト取得
	std::set<std::pair<CString, CString>> codeList;
	codeList = adrp002.getCodeList();

	CAdrp005 adrp005(m_cParam, m_ipWorkspaceAddr, codeList);
	CAdrp006 adrp006(m_cParam, m_ipWorkspaceAddr, codeList);
	if (!adrp005.init() || !adrp005.execute())
		return false;
	if (!adrp006.init() || !adrp006.execute())
		return false;

	if (m_ipWorkspaceMlang)
	{
		CAdrp001ML adrp001ML(m_cParam, m_ipWorkspaceMlang);
		CAdrp002ML adrp002ML(m_cParam, m_ipWorkspaceMlang);
		CAdrp003ML adrp003ML(m_cParam, m_ipWorkspaceMlang);
		CAdrp004ML adrp004ML(m_cParam, m_ipWorkspaceMlang);
		CAdrp005ML adrp005ML(m_cParam, m_ipWorkspaceMlang);
		CAdrp006ML adrp006ML(m_cParam, m_ipWorkspaceMlang);
		if (!adrp001ML.init() || !adrp001ML.execute())
			return false;
		if (!adrp002ML.init() || !adrp002ML.execute())
			return false;
		if (!adrp003ML.init() || !adrp003ML.execute())
			return false;
		if (!adrp004ML.init() || !adrp004ML.execute())
			return false;
		if (!adrp005ML.init() || !adrp005ML.execute())
			return false;
		if (!adrp006ML.init() || !adrp006ML.execute())
			return false;
	}

	// 件数表出力
	if (!make_count_auth_adr(adrp001, adrp002, adrp003, adrp004, adrp005, adrp006))
		return false;

	return true;
}

bool CConvAddrPoint::make_count_auth_adr(const CAdrp001& adrp001, const CAdrp002& adrp002, const CAdrp003& adrp003, const CAdrp004& adrp004, const CAdrp005& adrp005, const CAdrp006& adrp006)
{
	std::cout << "  住所件数表出力\r";

	// 出力ファイルオープン
	CFileSJIS cFile;
	std::ofstream ofs;
	if (! cFile.open(CString(m_cParam.getOutDir().c_str()), C_COUNT_AUTH_ADR_FILE, ofs))
		return false;

	// ヘッダ部出力
	ofs << "都道府県		市区町村		町・大字	丁目・字	番地			号		" << std::endl;
	ofs << "コード	名称	コード	名称			番地	文字付き番地	番地のうち支号付	号	文字付き号	号のうち枝番付" << std::endl;

	// 件数表用コード名称対応リスト取得
	std::map<CString, CString> codeNameList001 = adrp001.getCodeNameList();
	std::map<std::pair<CString, CString>, CString> codeNameList002 = adrp002.getCodeNameList();
	// 件数表用リスト取得
	std::map<std::pair<CString, CString>, long> codeCountList003 = adrp003.getCodeCountList();
	std::map<std::pair<CString, CString>, long> codeCountList004 = adrp004.getCodeCountList();
	std::map<std::pair<CString, CString>, std::vector<long>> codeCountList005 = adrp005.getCodeCountList();
	std::map<std::pair<CString, CString>, std::vector<long>> codeCountList006 = adrp006.getCodeCountList();

	// 都道府県コード(2桁)、市区町村コード(3桁)の5桁コードmapで繰り返し
	for (const auto& it002 : codeNameList002)
	{
		CString strPrefCode = (it002.first).first;
		CString strCityCode = (it002.first).second;
		auto it001 = codeNameList001.find(strPrefCode);
		auto it003 = codeCountList003.find(std::make_pair(strPrefCode, strCityCode));
		auto it004 = codeCountList004.find(std::make_pair(strPrefCode, strCityCode));
		auto it005 = codeCountList005.find(std::make_pair(strPrefCode, strCityCode));
		auto it006 = codeCountList006.find(std::make_pair(strPrefCode, strCityCode));

		ofs << CT2A(strPrefCode) << "\t"												// 都道府県コード
			<< (it001 != codeNameList001.end() ? CT2A(it001->second) : "") << "\t"		// 都道府県名称
			<< CT2A(strCityCode) << "\t"												// 市区町村コード
			<< CT2A(it002.second) << "\t"												// 市区町村名称
			<< (it003 != codeCountList003.end() ? it003->second : 0) << "\t"			// 町・大字のレコード件数
			<< (it004 != codeCountList004.end() ? it004->second : 0) << "\t"			// 丁目・字のレコード件数
			<< (it005 != codeCountList005.end() ? (it005->second)[0] : 0) << "\t"		// 番地のレコード件数
			<< (it005 != codeCountList005.end() ? (it005->second)[1] : 0) << "\t"		// 文字付き番地のレコード件数
			<< (it005 != codeCountList005.end() ? (it005->second)[2] : 0) << "\t"		// 番地のうち支号付のレコード件数
			<< (it006 != codeCountList006.end() ? (it006->second)[0] : 0) << "\t"		// 号のレコード件数
			<< (it006 != codeCountList006.end() ? (it006->second)[1] : 0) << "\t"		// 文字付き号のレコード件数
			<< (it006 != codeCountList006.end() ? (it006->second)[2] : 0) << std::endl;	// 号のうち枝番付のレコード件数
	}

	// データ部先頭行、データ部最終行、合計行
	long firstLine = 3;
	long lastLine = 2 + codeNameList002.size();
	long totalLine = lastLine + 1;

	// 合計行出力
	ofs << boost::str(boost::format("total				\"=SUM(E%1%:E%2%)\"	\"=SUM(F%1%:F%2%)\"	\"=SUM(G%1%:G%2%)\"	\"=SUM(H%1%:H%2%)\"	\"=SUM(I%1%:I%2%)\"	\"=SUM(J%1%:J%2%)\"	\"=SUM(K%1%:K%2%)\"	\"=SUM(L%1%:L%2%)\"") % firstLine % lastLine).c_str() << std::endl;
	ofs << std::endl;
	ofs << std::endl;

	// フッタ部出力
	ofs << boost::str(boost::format("									件数	通常住所	\"通常住所\n＋文字付き住所\"")).c_str() << std::endl;
	ofs << boost::str(boost::format("									都道府県	%1%	%1%") % codeNameList001.size()).c_str() << std::endl;
	ofs << boost::str(boost::format("									市区町村	%1%	%1%") % codeNameList002.size()).c_str() << std::endl;
	ofs << boost::str(boost::format("									町・大字	=E%1%	=E%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									丁目・字	=F%1%	=F%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									番地	=G%1%	=G%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									番地(文字付き)	-	=H%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									  番地のうち支号付	-	=I%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									号	=J%1%	=J%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									号(文字付き)	-	=K%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									  号のうち枝番付	-	=L%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									total	\"=SUM(K%1%:K%2%)\"	\"=SUM(L%1%:L%3%)+SUM(L%4%:L%5%)\"") % (lastLine + 5) % (lastLine + 14) % (lastLine + 10) % (lastLine + 12) % (lastLine + 13)).c_str() << std::endl;

	CLogSys::GetInstance().WriteRunLog(true, false, _T("  住所件数表出力 ... OK\n"));
	return true;
}

bool CConvAddrPoint::pos_authdb(void)
{
	CLogSys::GetInstance().WriteRunLog(true, false, _T("編集用DB向け-郵便番号ポイント変換\n"));

	CZipp001 zipp001(m_cParam, m_ipWorkspaceAddr);
	if (!zipp001.init() || !zipp001.execute())
		return false;

	// 件数表出力
	if (!make_count_auth_pos(zipp001))
		return false;

	return true;
}

bool CConvAddrPoint::make_count_auth_pos(const CZipp001& zipp001)
{
	std::cout << "  郵便番号件数表出力\r";

	// 出力ファイルオープン
	CFileSJIS cFile;
	std::ofstream ofs;
	if (! cFile.open(CString(m_cParam.getOutDir().c_str()), C_COUNT_AUTH_POS_FILE, ofs))
		return false;

	// ヘッダ部出力
	ofs << "郵便番号種別	総件数	ユニーク件数" << std::endl;

	// 件数表用リスト取得
	std::map<long, std::vector<long>> codeCountList = zipp001.getCodeCountList();

	// 郵便番号種別毎の出力
	std::map<long, std::vector<long>>::const_iterator it;
	it = codeCountList.find(sindy::schema::zip_point::zip_att::kAddress);
	ofs << "住所" << "\t"
		<< (it != codeCountList.end() ? it->second[0] : 0) << "\t"
		<< (it != codeCountList.end() ? it->second[1] : 0) << std::endl;

	it = codeCountList.find(sindy::schema::zip_point::zip_att::kBuilding);
	ofs << "ビル" << "\t"
		<< (it != codeCountList.end() ? it->second[0] : 0) << "\t"
		<< (it != codeCountList.end() ? it->second[1] : 0) << std::endl;

	it = codeCountList.find(sindy::schema::zip_point::zip_att::kOffice);
	ofs << "大口事業所" << "\t"
		<< (it != codeCountList.end() ? it->second[0] : 0) << "\t"
		<< (it != codeCountList.end() ? it->second[1] : 0) << std::endl;

	it = codeCountList.find(sindy::schema::zip_point::zip_att::kPostOfficeBox);
	ofs << "私書箱" << "\t"
		<< (it != codeCountList.end() ? it->second[0] : 0) << "\t"
		<< (it != codeCountList.end() ? it->second[1] : 0) << std::endl;

	// 合計行出力
	ofs << "合計" << "\t" << "\"=SUM(B2:B5)\"" << "\t" << "\"=SUM(C2:C5)\"" << std::endl;

	CLogSys::GetInstance().WriteRunLog(true, false, _T("  郵便番号件数表出力 ... OK\n"));
	return true;
}

bool CConvAddrPoint::adrmst_authdb(void)
{
	CLogSys::GetInstance().WriteRunLog(true, false, _T("編集用DB向け-住所系マスタ変換\n"));

	CMstAdm mstadm(m_cParam, m_ipWorkspaceAddr);
	CMstPfx mstpfx(m_cParam, m_ipWorkspaceAddr);
	CMstSfx mstsfx(m_cParam, m_ipWorkspaceAddr);
	if (!mstadm.init() || !mstadm.execute())
		return false;
	if (!mstpfx.init() || !mstpfx.execute())
		return false;
	if (!mstsfx.init() || !mstsfx.execute())
		return false;

	CCodeDomain2Tsv codeDomain2Tsv(m_cParam, m_ipWorkspaceAddr);
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_NMA, C_MST_FILE_NMA))
		return false;
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_RAA, C_MST_FILE_RAA))
		return false;
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_CBA, C_MST_FILE_CBA))
		return false;
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_GMA, C_MST_FILE_GMA))
		return false;
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_CST, C_MST_FILE_CST))
		return false;
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_ZPA, C_MST_FILE_ZPA))
		return false;

	return true;
}