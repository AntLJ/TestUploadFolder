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
#include <boost/noncopyable.hpp>
#include <mutex>

#include "ErrDef.h"

#define RUNLOGGER CRunLogger::GetLogger()
#define ERRLOGGER CErrLogger::GetLogger()

/**
 * @brief  ログベースクラス
 */
class CLogger
{
public:

	/**
	 * @brief  コンストラクタ
	 */
	CLogger(void){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CLogger(void){ Finalize(); }


public:

	/**
	 * @brief  ファイルオープン
	 */
	bool Initialize( const uh::tstring& dir_path );

	/**
	 * @brief  ファイルクローズ
	 */
	void Finalize() { m_Log.close(); }

	/**
	 * @brief  メッセージ出力
	 */
	virtual void Log( const uh::tstring& msg );

	/**
	 * @brief  メッセージ出力
	 */
	virtual void Log( const TCHAR* format, ... );

	/**
	 * @brief  << オペレータのオーバーロード
	 */
	virtual CLogger& operator<<( const uh::tstring& msg );

private:

	/**
	 * @brief  ファイル名返す
	 * @return ファイル名
	 */
	virtual uh::tstring getFileName() = 0;

	/**
	 * @brief  ヘッダー出力
	 */
	virtual void writeHeader() = 0;

private:

	std::ofstream m_Log;		//!< ログファイルストリーム

};


/**
 * @brief  実行ログクラス
 * @note   シングルトン
 */
class CRunLogger : public CLogger,
                   private boost::noncopyable
{
public:

	/**
	 * @brief  静的オブジェクト取得
	 */
	static std::mutex s_mutex;
	static CRunLogger& GetLogger()
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		static CRunLogger m_inst;
		return m_inst;
	}

	/**
	 * @brief  エラーレベル付きのメッセージ出力
	 * @param msg [in] メッセージ
	 */
	void Fatal( const uh::tstring& msg );
	void Error( const uh::tstring& msg );
	void Warn( const uh::tstring& msg );
	void Info( const uh::tstring& msg );
	void Debug( const uh::tstring& msg );

private:
	/**
	 * @brief  コンストラクタ
	 */
	CRunLogger(void){};

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CRunLogger(void){};

	/**
	 * @brief  ファイル名返す
	 * @return ファイル名
	 */
	virtual uh::tstring getFileName();

	/**
	 * @brief  ヘッダー出力
	 */
	void writeHeader();

};


/**
 * @brief  エラーログクラス
 * @note   シングルトン
 */
class CErrLogger : public CLogger,
                   private boost::noncopyable
{
public:

	/**
	 * @brief  静的オブジェクト取得
	 */
	static std::mutex s_mutex;
	static CErrLogger& GetLogger()
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		static CErrLogger m_inst;
		return m_inst;
	}

	/**
	 * @brief  エラーレベル付きのメッセージ出力
	 * @param layer [in] エラー対象レイヤ
	 * @param oid   [in] エラー対象オブジェクトのOID
	 * @param err   [in] エラー情報
	 */
	void Fatal( const uh::tstring& layer, long oid, const err_def::ErrPair& err );
	void Error( const uh::tstring& layer, long oid, const err_def::ErrPair& err );
	void Warn(  const uh::tstring& layer, long oid, const err_def::ErrPair& err );
	void Info(  const uh::tstring& layer, long oid, const err_def::ErrPair& err );
	void Debug( const uh::tstring& layer, long oid, const err_def::ErrPair& err );

private:
	/**
	 * @brief  コンストラクタ
	 */
	CErrLogger(void){};

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CErrLogger(void){};

	/**
	 * @brief  ファイル名返す
	 * @return ファイル名
	 */
	virtual uh::tstring getFileName();

	/**
	 * @brief  ヘッダー出力
	 */
	void writeHeader();

};


namespace log_utility
{
	/**
	 * @brief  ファイル名のプレフィックスを取得
	 * @return プレフィックス("ツール名_YYYYMMDDHHMMSS_")を返す
	 */
	static uh::tstring GetLogfileNamePrefix();

	/**
	 * @brief  日時の情報を文字列で返す
	 * @return 日時情報 ( [YYYY-MM-DD HH:MM::SS] )
	 */
	static uh::tstring GetDateTimeInfo();
}
