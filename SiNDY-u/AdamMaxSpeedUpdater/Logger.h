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
	bool Initialize( const uh::tstring& strDirPath );

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
	void Fatal(const uh::tstring& strMessage);
	void Error(const uh::tstring& strMessage);
	void Warn(const uh::tstring& strMessage);
	void Info(const uh::tstring& strMessage);
	void Debug(const uh::tstring& strMessage);

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
	*/
	void Fatal(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);
	void Error(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);
	void Warn(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);
	void Info(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);
	void Debug(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);

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

	/**
	* @brief  エラー書き込み
	*         制限速度コード(最大)は指定がないならば、ログへ出力しない
	* @param[in] err_level    書き込むエラーレベル
	* @param[in] lane_link    対象の車線リンク
	* @param[in] road_link    対象の道路リンク
	* @param[in] basemesh     ベースメッシュ番号
	* @param[in] msg          エラーメッセージ
	* @param[in] max_speed_c  車線リンクの更新された制限速度コード(最大)
	*/
	void writeErrRecord(CErrLogger::ErrLevel err_level, const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);
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
