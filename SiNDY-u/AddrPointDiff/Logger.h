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
	 * @brief  メッセージ出力（標準出力）
	 * @param strMessage [in] 出力メッセージ
	 */
	virtual void StdOut( const uh::tstring& strMessage );

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
	 * @param strMessage  [in] メッセージ
	 * @param isOutStdout [in] 標準出力に出力するか(true:する、false:しない)
	 */
	void Fatal( const uh::tstring& strMessage, bool isOutStdout = false );
	void Error( const uh::tstring& strMessage, bool isOutStdout = false );
	void Warn( const uh::tstring& strMessage, bool isOutStdout = false );
	void Info( const uh::tstring& strMessage, bool isOutStdout = false );
	void Debug( const uh::tstring& strMessage, bool isOutStdout = false );

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
