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

#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments() : m_output_header(false)
	{
	}

public:
	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);
	
private:
	/**
	 * @brief コマンドライン引数の解析
	 * @param strRunLog [in] 実行ログパス
	 * @param strErrLog [in] エラーログパス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool initLog(const CString& strRunLog, const CString& strErrLog);

	/**
	 * @brief コマンドライン引数出力用文字列取得
	 * @return コマンドライン引数文字列
	 */
	CString GetOption() const;

	/**
	 * @brief オプション文字列取得
	 * @param strParam [in] オプション名
	 * @param strValue [in] オプション値
	 * @return オプション文字列
	 */
	CString GetOptionLine(const CString& strParam, const CString& strValue) const;

public:
	boost::program_options::variables_map m_vm;
	uh::tstring	m_db_info;				//!< DB接続情報
	uh::tstring	m_conv_table_path;		//!< 変換テーブルのパス
	uh::tstring	m_conv_field_path;		//!< 変換フィールド対応表のパス
	uh::tstring	m_cnv_param_path;		//!< 座標変換用パラメータファイル(東京測地系→JGD2000変換用)
	uh::tstring	m_output_dir;			//!< データ出力ディレクトリ
	uh::tstring	m_output_delimiter;		//!< 出力データのデリミタ（csv,tsv）
	uh::tstring	m_output_encode;		//!< 出力データの文字コード（utf-8,shift_jis）
	uh::tstring	m_output_newline;		//!< 出力データの改行コード（CR,LF,CRLF）
	uh::tstring	m_output_georef;		//!< 出力データの測地系（tky,jgd）
	uh::tstring	m_output_shapetype;		//!< 出力データの形状タイプ（normal,wkt）
	uh::tstring	m_output_polygontype;	//!< 出力データのポリゴンの向き（normal,clockwise,anticlockwise）
	bool m_output_header;

	uh::tstring	m_runlog_path;			//!< 実行ログ
	uh::tstring	m_errlog_path;			//!< エラーログ
};
