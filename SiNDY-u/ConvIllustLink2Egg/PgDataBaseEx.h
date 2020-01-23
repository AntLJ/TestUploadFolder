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
#include <libpq-fe.h>
#include<TDC/nsdb/PgDatabase.h>
#include <boost/format.hpp>

/**
 *	@class PgDatabaseEx
 *	@brief PgDatabaseの機能拡張
 *  コピー文を使用したデータインポート機能追加
 */
class PgDataBaseEx :
	public ns::PgDatabase
{
public:
	/**
	 *	@brief	コンストラクタ
	 *	@param	conninfo [in] 接続情報文字列
	 */
	PgDataBaseEx(const char *conninfo) : ns::PgDatabase(conninfo) {}
	/**
	 *	@brief	デストラクタ
	 */
	~PgDataBaseEx(void) {}

	/**
	 *	@brief	コピー開始
	 *	@note	コピー開始後はコピー文以外のSQLは実行してはいけない。コピープロセスが終了する。
	 *	@param	tablename [in] テーブル名
	 *	@param	fields [in] フィールド列(カンマ区切り)
	 *	@retval true 成功
	 *	@retval	false 失敗
	 */
	bool StartCopy(const char *tablename, const char *fields) {
		std::string sql = boost::str(boost::format("COPY %s (%s) FROM STDIN;") % tablename % fields);
		PGconn *conn = getConn();
		PGresult *result = PQexec(conn, sql.c_str());
		if (! result || (PQresultStatus(result) != PGRES_COPY_IN)) {
			setError(PQerrorMessage(conn));
			PQclear(result);
			return false;
		}
		PQclear(result);
		return true;
	}
	/**
	 *	@brief	コピー終了
	 *	@note	コピー完了時に呼び出す
	 *	@retval true 成功
	 *  @reval  false 成功
	 */
	bool EndCopy() {
		PGconn *conn = getConn();
		int result = PQputCopyEnd(conn, NULL);
		switch (result) {
		case 0:
			setError("Writing COPY data blocked.");
			return false;
		case -1:
			setError(PQerrorMessage(conn));
			return false;
		default:
			break;
		}
		PGresult *res = PQgetResult(conn);
		if (res && PQresultStatus(res) != PGRES_COMMAND_OK) {
			setError(boost::str(boost::format("COPY statement failed.\n%s") % PQerrorMessage(conn)).c_str());
			PQclear(res);
			return false;
		}
		PQclear(res);
		return true;
	}
	/**
	 *	@brief	データのコピー
	 *	@note	事前にstartCopyが呼ばれていることが前提
	 *	@param	data [in] コピーするデータ
	 *  @param	size [in] データサイズ(dataの長さ)
	 *  @retval true 成功
	 *  @retval false 失敗
	 */
	bool copyData(const char *data, unsigned long size) {
		PGconn *conn = getConn();
		int result = PQputCopyData(conn, data, size);
		switch (result) {
		case 0:
			setError("Writing COPY data blocked.");
			return false;
		case -1:
			setError(PQerrorMessage(conn));
			return false;
		default:
			break;
		}
		return true;
	}

	/**
	 *	@brief	データベース名の取得
	 *  @return 接続データベース名
	 */
	std::string getDBName() {
		std::string retval = PQdb(getConn());
		return retval;
	}
	/**
	 *	@brief	ホスト名の取得
	 *	@return	ホスト名
	 */
	std::string getHostName() {
		std::string retval = PQhost(getConn());
		return retval;
	}
	/**
	 *	@brief	ポート番号の取得</b>\n
	 *	@return	ポート番号
	 */
	std::string getPort() {
		std::string retval = PQport(getConn());
		return retval;
	}
	/**
	 *	@brief	ユーザ名を取得</b>\n
	 *	@return	ユーザ名
	 */
	std::string getUser() {
		std::string retval = PQuser(getConn());
		return retval;
	}
	/**
	 *  @brief	パスワードを取得する
	 *	@return	パスワード
	 */
	std::string getPassword() {
		std::string retval = PQpass(getConn());
		return retval;
	}
};

