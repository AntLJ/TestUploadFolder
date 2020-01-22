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

#include <TDC/useful_headers/boost_program_options.h>

/**
 * 汎用ログクラス
 */
class LogUtil
{
public:
	LogUtil(void);
	~LogUtil(void);

	/**
	 * 標準出力する
	 * 
	 * @param[in] filename	出力内容
	 */
	static void print(const std::string& msg);
	static void print(const uh::tstring& msg);
	static void print(const std::ostream& msg);
	static void print(const boost::program_options::options_description& msg);
	static void print(long id, const std::string& errmsg);
	static void print(long id, const std::string& errmsg, const std::string& value,const std::string& target);

	static void flushLine(const std::string& msg);

	/**
	 * デバック内容を標準出力する
	 * プレフィックス [DEBUG] を付けて標準出力
	 * 
	 * @param[in] filename	出力内容
	 */
	static void printDebug(const std::string& msg);
	static void printDebug(const uh::tstring& msg);
	static void printDebug(const std::ostream& msg);
	static void printDebug(const boost::program_options::options_description& msg);
	static void printDebug(long id, const std::string& errmsg);
	static void printDebug(long id, const std::string& errmsg, const std::string& value,const std::string& target);

	/**
	 * エラー内容を標準出力する
	 * プレフィックス [WAR] を付けて標準出力
	 * 
	 * @param[in] filename	出力内容
	 */
	static void printWarning(const std::string& msg);
	static void printWarning(const uh::tstring& msg);
	static void printWarning(const std::ostream& msg);
	static void printWarning(const boost::program_options::options_description& msg);
	static void printWarning(long id, const std::string& errmsg);
	static void printWarning(long id, const std::string& errmsg, const std::string& value,const std::string& target);

	/**
	 * エラー内容を標準出力する
	 * プレフィックス [ERR] を付けて標準出力
	 * 
	 * @param[in] filename	出力内容
	 */
	static void printError(const std::string& msg);
	static void printError(const uh::tstring& msg);
	static void printError(const std::ostream& msg);
	static void printError(const boost::program_options::options_description& msg);
	static void printError(long id, const std::string& errmsg);
	static void printError(long id, const std::string& errmsg, const std::string& value,const std::string& target);
	
	/**
	 * ログ内容をファイル出力する
	 *
	 * @note 出力先は、実行ファイルと同Dir
	 * 「./ADAM_AuthoringLog_YYYYMMDDhhmmss.txt」
	 */
	static void write();
	
	/**
	 * ログ内容をファイル出力する
	 *
	 * @param[in] dir 出力ディレクトリ
	 */
	static void write(const uh::tstring& dir);

	/**
	 * ログ出力ディレクトリを設定する
	 *
	 * @param[in] dir 出力ディレクトリ
	 */
	static void setLogDirPath(const uh::tstring& dir);

	/**
	 * ERRログ件数を取得する
	 */
	static long getErrCount();

	/**
	 * WARログ件数を取得する
	 */
	static long getWarCount();

	/**
	 * 線(-----------------)を出力する
	 */
	static void printLine();

	/**
	 * 二重線(=====================)を出力する
	 */
	static void printDoubleLine();

	/**
	 * パラメータを出力する
	 *
	 * @param[in]	param	パラメータ値
	 * @param[in]	value	設定値
	 */
	static void printParam(const uh::tstring& param, const uh::tstring& value);
	static void printParam(const std::string& param, const std::string& value);

	/**
	 * フィーチャ名と接続状態を出力する
	 *
	 * @param[in]	name	フィーチャ名(和名)
	 * @param[in]	table	フィーチャ名
	 * @param[in]	state	接続状態(true:成功、false:失敗)
	 */
	static void printTableState(const uh::tstring& name, const uh::tstring& table, bool state);
	static void printTableState(const std::string& name, const std::string& table, bool state);
	static void printTableState(long id, const std::string& table, bool state);

	/**
	 * フィーチャ名と接続状態を出力する
	 *
	 * @param[in]	name	フィーチャ名(和名)
	 * @param[in]	table	フィーチャ名
	 * @param[in]	state	接続状態名
	 */
	static void printTableState(const uh::tstring& name, const uh::tstring& table, const uh::tstring& state);
	static void printTableState(const std::string& name, const std::string& table, const std::string& state);

private:
	/**
	 * ログ内容をファイル出力する
	 *
	 * @param filename [in]ファイルパス
	 */
	static void _writeLog(const uh::tstring& filename);
private:
	static std::stringstream _strLogMsg;
	static uh::tstring _dirPath;
	static long _errCount;
	static long _warCount;
};

