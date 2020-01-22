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
	bool Initialize( const uh::tstring& strFilePath );

	/**
	 * @brief  ファイルクローズ
	 */
	void Finalize() { m_Log.close(); }

	/**
	 * @brief  メッセージ出力
	 */
	virtual void Log( const uh::tstring& strMessage );

	/**
	 * @brief  メッセージ出力
	 */
	virtual void Log( const TCHAR* format, ... );

	/**
	 * @brief  << オペレータのオーバーロード
	 */
	virtual CLogger& operator<<( const uh::tstring& strMessage );

private:
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
	 * @param strMessage [in] メッセージ
	 */
	void Fatal( const uh::tstring& strMessage );
	void Error( const uh::tstring& strMessage );
	void Warn( const uh::tstring& strMessage );
	void Info( const uh::tstring& strMessage );
	void Debug( const uh::tstring& strMessage );

	/**
	 * @brief  終了ログ出力
	 * @param ret [in] 処理成功/失敗
	 */
	void writeExitLog(bool ret);

private:
	/**
	 * @brief  コンストラクタ
	 */
	CRunLogger(void){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CRunLogger(void){}

	/**
	 * @brief  ヘッダー出力
	 */
	void writeHeader() override;

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
	 * @brief  エラーログを書きこむ
	 * @param  layer   [in] レイヤ名
	 * @param  oid     [in] オブジェクトID
	 * @param  message [in] エラーメッセージ
	 */
	void writeError(const uh::tstring& layer, const long oid, const uh::tstring& message);

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
	 * @brief  ヘッダー出力
	 */
	void writeHeader() override;
};


namespace log_utility
{
	/**
	 * @brief  日時の情報を文字列で返す
	 * @return 日時情報 ( [YYYY-MM-DD HH:MM::SS] )
	 */
	static uh::tstring GetDateTimeInfo();
}
