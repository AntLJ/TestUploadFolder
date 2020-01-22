#pragma once

#include <fstream>
#include <boost/noncopyable.hpp>
#include <mutex>
#include "Arguments.h"

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
	 * @param   strDirPath [in] ディレクトリパス
	 */
	bool Initialize( const uh::tstring& strDirPath );

	/**
	 * @brief  ファイルクローズ
	 */
	void Finalize() { m_Log.close(); }

	/**
	 * @brief  メッセージ出力
	 * @param   strMessage [in] メッセージ
	 */
	virtual void Log( const uh::tstring& strMessage );

	/**
	 * @brief  メッセージ出力
	 * @param   strMessage [in] メッセージ
	 */
	virtual void Log( const TCHAR* format, ... );

	/**
	 * @brief  << オペレータのオーバーロード
	 * @param   strMessage [in] メッセージ
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
	 * @brief  ログ情報＆現在時刻出力
	 */
	void writeInfo( const CString& info );

	/**
	 * @brief  コマンドライン引数の内容出力
	 * @param arg [in] コマンドライン引数
	 */
	void writeOption( const Arguments& arg );

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
	 * @brief  メッセージ出力（ERROR）
	 * @param layer		[in] レイヤ
	 * @param oid		[in] オブジェクトID
	 * @param errMsg	[in] メッセージ
	 * @param meshCode	[in] メッシュコード
	 */
	void error( const CString& layer, const CString& oid, const CString& errMsg, const CString& meshCode );

	/**
	 * @brief  メッセージ出力（WARNING）
	 * @param layer		[in] レイヤ
	 * @param oid		[in] オブジェクトID
	 * @param errMsg	[in] メッセージ
	 * @param meshCode	[in] メッシュコード
	 */
	void warning( const CString& layer, const CString& oid, const CString& errMsg, const CString& meshCode );

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
	void writeHeader();

};


namespace log_utility
{
	/**
	 * @brief  日時の情報を文字列で返す
	 * @return 日時情報 ( [YYYY/MM/DD HH:MM::SS] )
	 */
	static CString GetDateTimeInfo();
}
