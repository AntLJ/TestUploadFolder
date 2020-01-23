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

CParam::CParam(void):m_eType(outtype::EType::unknown)
{
}


CParam::~CParam(void)
{
}


bool CParam::parse(int argc, _TCHAR* argv[])
{
	boost::program_options::options_description opt("オプション");
	opt.add_options()
		(D_TYPE, uh::tvalue<uh::tstring>(&m_tstrType), "[必須] 出力タイプ(adr_authdb,pos_authdb,adrmst_authdb)")
		(D_ADDR_DB, uh::tvalue<uh::tstring>(&m_tstrAddrDB), "[必須] 住所DB接続情報")
		(D_ADDR_OWNER, uh::tvalue<uh::tstring>(&m_tstrAddrOwner), "[選択] 住所DBのオーナー")
		(D_MLANG_DB, uh::tvalue<uh::tstring>(&m_tstrMlangDB), "[選択] 多言語DB接続情報")
		(D_MLANG_OWNER, uh::tvalue<uh::tstring>(&m_tstrMlangOwner), "[選択] 多言語DBのオーナー")
		(D_OUT_DIR, uh::tvalue<uh::tstring>(&m_tstrOutDir), "[必須] 出力ディレクトリ")
		(D_RUN_LOG, uh::tvalue<uh::tstring>(&m_tstrRunLog), "[必須] 実行ログ")
		(D_ERR_LOG, uh::tvalue<uh::tstring>(&m_tstrErrLog), "[必須] エラーログ");

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, opt), vm);
	boost::program_options::notify(vm);

	bool bAllow = true;
	bAllow &= check_required(m_tstrType, D_TYPE);
	bAllow &= check_required(m_tstrAddrDB, D_ADDR_DB);
	bAllow &= check_required(m_tstrOutDir, D_OUT_DIR);
	bAllow &= check_required(m_tstrRunLog, D_RUN_LOG);
	bAllow &= check_required(m_tstrErrLog, D_ERR_LOG);
	if (!bAllow) {
		std::cerr << opt << std::endl;
		return false;
	}

	if (uh::toStr(m_tstrType) == D_TYPE_ADR_AUTHDB)
		m_eType = outtype::EType::adr_authdb;
	else if (uh::toStr(m_tstrType) == D_TYPE_POS_AUTHDB)
		m_eType = outtype::EType::pos_authdb;
	else if (uh::toStr(m_tstrType) == D_TYPE_ADRMST_AUTHDB)
		m_eType = outtype::EType::adrmst_authdb;

	if (m_eType == outtype::EType::unknown) {
		std::cerr << "ERROR\t" << D_TYPE << "オプションが不正です" << std::endl;
		std::cerr << opt << std::endl;
		return false;
	}

	if (m_tstrMlangDB.size() != 0 || m_tstrMlangOwner.size() != 0) {
		if (m_eType != outtype::EType::adr_authdb) {
			std::cerr << "ERROR\t" << "出力タイプが" << uh::toStr(m_tstrType).c_str() << "の時は" << D_MLANG_DB << "と" << D_MLANG_OWNER << "オプションは設定できません" << std::endl;
			return false;
		}
	}

	return true;
}

bool CParam::check_required(const uh::tstring& tstrValue, LPCSTR lpszDesc)
{
	if (tstrValue.size() == 0) {
		std::cerr << "ERROR\t" << lpszDesc << "オプションが未設定です" << std::endl;
		return false;
	}
	return true;
}

std::string CParam::getOption(void) const
{
	std::string strOption = "  option\n";
	if (!m_tstrType.empty())
		strOption.append(getOptionLine(D_TYPE, uh::toStr(m_tstrType)));

	if (!m_tstrAddrDB.empty())
		strOption.append(getOptionLine(D_ADDR_DB, uh::toStr(m_tstrAddrDB)));

	if (!m_tstrAddrOwner.empty())
		strOption.append(getOptionLine(D_ADDR_OWNER, uh::toStr(m_tstrAddrOwner)));

	if (!m_tstrMlangDB.empty())
		strOption.append(getOptionLine(D_MLANG_DB, uh::toStr(m_tstrMlangDB)));

	if (!m_tstrMlangOwner.empty())
		strOption.append(getOptionLine(D_MLANG_OWNER, uh::toStr(m_tstrMlangOwner)));

	if (!m_tstrOutDir.empty())
		strOption.append(getOptionLine(D_OUT_DIR, uh::toStr(m_tstrOutDir)));

	if (!m_tstrRunLog.empty())
		strOption.append(getOptionLine(D_RUN_LOG, uh::toStr(m_tstrRunLog)));

	if (!m_tstrErrLog.empty())
		strOption.append(getOptionLine(D_ERR_LOG, uh::toStr(m_tstrErrLog)));

	return strOption;
}

std::string CParam::getOptionLine(const char* const cParam, const std::string& strValue) const
{
	std::string strOptionLine = "    --";
	strOptionLine.append(cParam);
	strOptionLine.append(" ");
	strOptionLine.append(strValue);
	strOptionLine.append("\n");

	return strOptionLine;
}
