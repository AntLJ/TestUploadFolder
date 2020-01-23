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
	bool Initialize( const uh::tstring& dir_path );

	/**
	 * @brief  ファイルクローズ
	 */
	void Finalize() { m_Log.close(); }

	/**
	 * @brief  メッセージ出力
	 */
	virtual void Log( const uh::tstring& message );

	/**
	 * @brief  メッセージ出力
	 */
	virtual void Log( const TCHAR* format, ... );

	/**
	 * @brief  << オペレータのオーバーロード
	 */
	virtual CLogger& operator<<( const uh::tstring& message );

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

protected:
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
	void Fatal( const uh::tstring& message );
	void Error( const uh::tstring& message );
	void Warn( const uh::tstring& message );
	void Info( const uh::tstring& message );
	void Debug( const uh::tstring& message );

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
private:
	/**
	 * @brief  エラーログ出力時のエラーレベル指定
	 */
	enum ErrLevel
	{
		kFatal,
		kErr,
		kWarn,
		kInfo,
		kDebug
	};

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
	 * @brief  エラーレベル指定のメッセージ出力
	 * @param[in] oid        対象の車線リンクのオブジェクトID
	 * @param[in] gid        車線リンクのグローバルID
	 * @param[in] mesh_code  ベースメッシュ番号
	 * @param[in] x          経度
	 * @param[in] y          緯度
	 * @param[in] strMsg     エラーメッセージ
	 */
	void Fatal( long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg );
	void Error( long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg );
	void Warn ( long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg );
	void Info ( long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg );
	void Debug( long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg );

private:
	/**
	 * @brief  コンストラクタ
	 */
	CErrLogger(void){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CErrLogger(void){}

	/**
	 * @brief  ファイル名返す
	 * @return ファイル名
	 */
	virtual uh::tstring getFileName();

	/**
	 * @brief  ヘッダー出力
	 */
	void writeHeader();

	/**
	 * @brief  エラー書き込み
	 * @param[in] err_level  書き込むエラーレベル
	 * @param[in] oid        対象の車線リンクのオブジェクトID
	 * @param[in] gid        車線リンクのグローバルID
	 * @param[in] mesh_code  ベースメッシュ番号
	 * @param[in] x          経度
	 * @param[in] y          緯度
	 * @param[in] strMsg     エラーメッセージ
	 */
	void writeErrRecord( CErrLogger::ErrLevel err_level, long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg );
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
