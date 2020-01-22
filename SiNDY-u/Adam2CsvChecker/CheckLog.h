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

#include <fstream>

class CheckLog {
private:
	~CheckLog() {}
	CheckLog ();

	CheckLog(const CheckLog& );
	CheckLog& operator = (const CheckLog&);

public:
	static CheckLog* GetInstance() {
		static CheckLog kInstance;
		return &kInstance;
	}


public:
	void printError(
				long                code,       // エラーコードNo
				const std::string&  info,       // エラーコードに対応するメッセージ
				const std::string&  file,       // 読み込んでいるエイリアス名
				long                oid,        // OBJECTID
				const std::string&  field,      // エラー時のフィールド名
				const std::string&  msg         // エラー時の入力値/取りうる値、等
	);

	void printWarning(
				long                code,       // エラーコードNo
				const std::string&  info,       // エラーコードに対応するメッセージ
				const std::string&  file,       // 読み込んでいるエイリアス名
				long                oid,        // OBJECTID
				const std::string&  field,      // エラー時のフィールド名
				const std::string&  msg         // エラー時の入力値/取りうる値、等
	);

	void printInfo(
				long                code,       // エラーコードNo
				const std::string&  info,       // エラーコードに対応するメッセージ
				const std::string&  file,       // 読み込んでいるエイリアス名
				long                oid,        // OBJECTID
				const std::string&  field,      // エラー時のフィールド名
				const std::string&  msg         // エラー時の入力値/取りうる値、等
	);


protected:
	void print(
				long                level,      // エラーレベル (ERR/WAR/INF/DEL)
				long                code,       // エラーコードNo
				const std::string&  info,       // エラーコードに対応するメッセージ
				const std::string&  file,       // 読み込んでいるエイリアス名
				long                oid,        // OBJECTID
				const std::string&  field,      // エラー時のフィールド名
				const std::string&  msg         // エラー時の入力値/取りうる値、等
	);


private:
	std::ofstream   m_cStream;
};

#define LOGCODE_ERROR_COLUMNS           (3)
#define LOGCODE_ERROR_FORMAT            (1)
#define LOGCODE_ERROR_INVALID           (2)
#define LOGCODE_ERROR_OID_ASC           (4)


#define LOGMSG_ERROR_COLUMNS            "列数チェックエラー"
#define LOGMSG_ERROR_FORMAT             "書式チェックエラー"
#define LOGMSG_ERROR_INVALID            "無効値チェックエラー"
#define LOGMSG_ERROR_OID_ASC            "OID昇順チェックエラー"

