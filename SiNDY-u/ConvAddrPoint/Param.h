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

#pragma once

/**
 * @file	Param.h
 */

// 本クラスを使用するには以下のincludeが必要
// 可能な限り.hでのincludeは避けるべきだが、ほぼ全ての.cppに影響があるため本ヘッダでincludeしてしまう
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/boost_program_options.h>

#define D_TYPE				"type"
#define D_ADDR_DB			"addr_db"
#define D_ADDR_OWNER		"addr_owner"
#define D_MLANG_DB			"mlang_db"
#define D_MLANG_OWNER		"mlang_owner"
#define D_OUT_DIR			"out_dir"
#define D_RUN_LOG			"run_log"
#define D_ERR_LOG			"err_log"

#define D_TYPE_ADR_AUTHDB		"adr_authdb"	// 住所・編集用DB向け
#define D_TYPE_ADR_ESPODB		"adr_espoirdb"	// 住所・espoir-DB向け　※未実装
#define D_TYPE_POS_AUTHDB		"pos_authdb"	// 郵便番号・編集用DB向け
#define D_TYPE_POS_ESPODB		"pos_espoirdb"	// 郵便番号・espoir-DB向け　※未実装
#define D_TYPE_ADRMST_AUTHDB	"adrmst_authdb"	// 住所系マスタ・編集用-DB向け

namespace outtype {
	enum EType {
		unknown       = 0,
		adr_authdb    = 1,	// 住所・編集用DB向け
		adr_espoirdb,		// 住所・espoir-DB向け　※未実装
		pos_authdb,			// 郵便番号・編集用DB向け
		pos_espoirdb,		// 郵便番号・espoir-DB向け　※未実装
		adrmst_authdb		// 住所系マスタ・編集用-DB向け
	};
}

/**
* @class	CParam
* @brief	パラメータクラス
*/
class CParam
{
public:
	CParam(void);
	~CParam(void);

	/**
	* @brief	オプション解析
	* @param	argc	[in]	引数の数
	* @param	argv	[in]	引数
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool parse(int argc, _TCHAR* argv[]);

	/**
	* @brief	オプション取得
	* @return	値
	*/
	std::string	getType(void) const { return uh::toStr(m_tstrType); };
	std::string	getAddrDB(void) const { return uh::toStr(m_tstrAddrDB); };
	std::string	getAddrOwner(void) const { return uh::toStr(m_tstrAddrOwner); };
	std::string	getMlangDB(void) const { return uh::toStr(m_tstrMlangDB); };
	std::string	getMlangOwner(void) const { return uh::toStr(m_tstrMlangOwner); };
	std::string	getOutDir(void) const { return uh::toStr(m_tstrOutDir); };
	std::string	getRunLog(void) const { return uh::toStr(m_tstrRunLog); };
	std::string	getErrLog(void) const { return uh::toStr(m_tstrErrLog); };

	/**
	* @brief	オプション取得
	* @return	オプション文字列
	*/
	std::string	getOption(void) const;

	/**
	* @brief	出力タイプ取得
	* @return	値
	*/
	outtype::EType	getEType(void) const { return m_eType; };

private:
	/**
	* @brief	必須チェック
	* @param	tstrValue	[in]	値
	* @param	lpszDesc	[in]	表示名
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool check_required(const uh::tstring& tstrValue, LPCSTR lpszDesc);

	/**
	* @brief	オプション１行取得
	* @param	cParam	[in]	パラメータ
	* @param	strValue	[in]	値
	* @return	オプション１行文字列
	*/
	std::string	getOptionLine(const char* const cParam, const std::string& strValue) const;

private:
	// option
	uh::tstring	m_tstrType;			// 出力タイプ
	uh::tstring	m_tstrAddrDB;		// 住所DB接続情報
	uh::tstring	m_tstrAddrOwner;	// 住所DBのオーナー
	uh::tstring	m_tstrMlangDB;		// 多言語DB接続情報
	uh::tstring	m_tstrMlangOwner;	// 多言語DBのオーナー
	uh::tstring	m_tstrOutDir;		// 出力ディレクトリ
	uh::tstring	m_tstrRunLog;		// 実行ログ
	uh::tstring	m_tstrErrLog;		// エラーログ

	outtype::EType m_eType;			// 出力タイプ
};
