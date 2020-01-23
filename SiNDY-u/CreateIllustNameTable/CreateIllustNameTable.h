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

#include "IllustSettings.h"
#include "LogMgr.h"
#include "IllustNameTable.h"
#include <boost/program_options.hpp>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/illustlib/IllustLinkRow.h>

/**
 * @brief イラストファイル名対応表作成クラス
 */
class CreateIllustNameTable
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CreateIllustNameTable();

	/**
	 * @brief デストラクタ
	 */
	~CreateIllustNameTable();

	/**
	 * @brief 初期化
	 * @param argc [in] 引数の数
	 * @param argv [in] 引数リスト
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool init( int argc, _TCHAR* argv[] );

	/**
	 * @brief 処理の実行
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool execute();

private:
	/**
	 * @brief 引数のチェック
	 * @param argc [in] 引数の数
	 * @param argv [in] 引数リスト
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool checkArg( int argc, _TCHAR* argv[] );

	/**
	 * @brief 引数パラメータの取得
	 * @param strName [in] 引数名
	 * @param strParam [out] 取得したパラメータ
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getArgParam( const std::string& strName, CString& strParam );

	/**
	 * @brief ファイルの存在を確認
	 * @param strFileName [in] 対象ファイル名
	 * @retval true  ファイルが存在する
	 * @retval false ファイルが存在しない
	 */
	bool checkFileExistence( const CString& strFileName );

	/**
	 * @brief エラーログを標準ログフォーマットで出力
	 * @param illustLinkRow [in] 対象のイラストリンク
	 * @param errLv [in] エラーレベル
	 * @param strErrCode [in] エラーコード
	 * @param strErrMsg [in] エラーメッセージ
	 * @param strOption [in] 追加情報
	 */
	void outputErrLog( const sindy::CSPIllustLinkRow& illustLinkRow, LogMgr::EErrLevel errLv, const CString& strErrCode, const CString& strErrMsg, const CString& strOption = _T("") );

private:
	boost::program_options::variables_map	m_varMap;	//!< オプション格納マップ

	CString		m_strRunLog;	//!< 実行ログファイル名
	CString		m_strErrLog;	//!< エラーログファイル名

	CString		m_strDBConnect;	//!< DB接続文字列
	CString		m_strOutFile;	//!< イラストファイル名対応表ファイル名
	CString		m_strIniFile;	//!< 処理情報設定ファイル名

	int			m_nErrCounter;	//!< エラー数のカウンタ

	IllustSettings		m_cIllustSettings;	//!< 処理情報設定クラス

	sindy::CTableFinder	m_ipTableFinder;	//!< テーブルファインダ

	IllustNameTable		m_cIllustNameTable;	//!< イラストファイル名対応表クラス
};
