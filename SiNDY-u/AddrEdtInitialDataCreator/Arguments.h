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

#include "Utility.h"
#include <TDC/useful_headers/boost_program_options.h>

namespace po = boost::program_options;

/**
 * @brief オプション名
*/
namespace arg {
	const char * const k_imp_type    = "imp_type";
	const char * const k_edt_addr_db = "edt_addr_db";
	const char * const k_old_addr_db = "old_addr_db";
	const char * const k_gs_addr_db   = "gs_addr_db";
	const char * const k_run_log = "run_log";
	const char * const k_err_log = "err_log";
	const char * const k_citylist = "citylist";
	const char * const k_extcode_list = "extcode_list";
};

/**
 * @brief imp_typeオプション値
*/
namespace arg_imp_type {
	const char * const k_placename_rep = "placename_rep";  // 地名代表点ポイント生成
	const char * const k_addr_poly = "addr_poly";          // 住所ポリゴン生成
	const char * const k_addr_point = "addr_point";        // 住所ポイント生成
	const char * const k_addr_all = "addr_all";            // 住所ポリゴン＋住所ポイント生成
};

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments() : m_impType(Utility::ExecuteType::None) {}

	/**
	 * @brief デストラクタ
	 */
	virtual ~Arguments() {}

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  コマンドライン引数（imp_typeオプション用）の確認
	 * @param  vm       [in]  オプション解析結果
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckImpTypeParam
	(
		const po::variables_map& vm,
		const char* const optName,
		CString& optValue
	);

	/**
	 * @brief  コマンドライン引数の確認
	 * @param  vm       [in]  オプション解析結果
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @param  bFile    [in]  ファイルであるか（デフォルトfalse）
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckParam
	( 
		const po::variables_map& vm,
		const char* const optName,
		CString& optValue,
		bool bFile = false
	);

public:
	CString  m_impTypeStr;             //!< インポートタイプ（文字列）
	Utility::ExecuteType  m_impType;   //!< インポートタイプ（列挙型）
	CString  m_edtAddrDb;              //!< SiNDY（住所・編集）接続先
	CString  m_oldAddrDb;              //!< SiNDY（旧住所・編集）接続先
	CString  m_gsAddrDb;               //!< GS納品データ(住所）接続先
	CString  m_runLog;                 //!< 実行ログ
	CString  m_errLog;                 //!< エラーログ
	CString  m_cityList;               //!< 市区町村コードリスト
	CString  m_extcodeList;            //!< 拡張コードリスト
};
