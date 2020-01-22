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
#include <boost/program_options.hpp>

/**
 * @brief ツールベースクラス
 */
class CToolBase
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	CToolBase();

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CToolBase();

	/**
	 * @brief  初期化
	 * @param  argc [in]  コマンドライン引数個数
	 * @param  argv [in]  コマンドライン引数文字列
	 * @return bool 正常に初期化できたかどうか
	 */
	virtual bool Init( int argc, _TCHAR* argv[] ) = 0;

	/**
	 * @brief  処理実行仮想関数
	 * @return bool 正常に処理が終了したかどうか
	 */
	virtual bool Execute() = 0;

protected:
	/**
	 * @brief  ログファイルをオープンする関数
	 * @note   ファイルオープンした時に、ツール名と引数も出力するためにプログラムオプションも引数に必要
	 * @param  strLogFileName [in] ログファイル名
	 */
	bool OpenLogFile( const CString& strLogFileName );

	/**
	 * @brief  ログファイルにログを書き込む関数
	 * @note   標準出力する必要のないログの場合には、第2引数をfalseにすること
	 * @param  strLog  [in] 書き込むログ
	 * @param  bDisp   [in] 標準出力をするかどうか（デフォルト:true）
	 */
	void WriteLog( const CString& strLog, bool bDisp = true );

	/**
	 * @brief  ログヘッダをログに書き込む関数
	 * @note   ヘッダを書き込む場合はサブクラス側でオーバーライド
	 */
	virtual void WriteLogHeader();

	/**
	 * @brief  ログファイルをクローズする関数
	 */
	void CloseLogFile();

private:
	/**
	 * @brief  ログファイルにプログラム開始としてツール情報、オプション引数を出力する関数
	 */
	void StartLogFile();

	/**
	 * @brief  ログファイルにプログラム終了時刻と処理結果を出力する関数
	 */
	void EndLogFile();

	/**
	 * @brief  ログファイルにツール名とバージョンを出力する関数
	 */
	void WriteFileVersion();

	/**
	 * @brief  ログファイルにオプション引数を出力する関数
	 */
	void WriteOptionString();

protected:
	boost::program_options::variables_map	m_variable_map;	//!< プログラムオプション格納マップ

	CString			m_strLogFileName;	//!< ログ出力ファイル名
	bool			m_bIsOK;			//!< ツールが正常終了したかどうか（継承ツール側で正常に終了したらtrueをセットしてあげる必要がある）
	std::ofstream	m_fStream;			//!< ログ出力用ファイルストリーム
};
