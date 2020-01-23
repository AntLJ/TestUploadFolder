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
	 * @brief  ヘッダー出力
	 */
	virtual void writeHeader() = 0;

private:

	std::ofstream m_Log;		//!< ログファイルストリーム

};


/**
 * @brief  実行ログクラス
 */
class CRunLogger : public CLogger, 
                   private boost::noncopyable
{
public:

	/**
	 * @brief  静的オブジェクト取得
	 */
	static CRunLogger& GetLogger()
	{
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
	CRunLogger( void ) :CLogger(){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CRunLogger(void){}

	/**
	 * @brief  ヘッダー出力
	 */
	virtual void writeHeader() override;

};


/**
 * @brief  エラーログクラス
 */
class CErrLogger : public CLogger, 
                   private boost::noncopyable
{
public:

	/**
	 * @brief  静的オブジェクト取得
	 */
	static CErrLogger& GetLogger()
	{
		static CErrLogger m_inst;
		return m_inst;
	}

	/**
	 * @brief  メッセージ出力（ERROR）
	 * @param layer		[in] レイヤ
	 * @param oid		[in] オブジェクトID
	 * @param gid		[in] グローバルID
	 * @param errMsg	[in] メッセージ
	 * @param meshCode	[in] メッシュコード
	 * @param source	[in] 道路境界線リンクのSOURCE
	 */
	void error( const CString& layer, const CString& oid, const CString& gid, const CString& errMsg, const CString& meshCode, const CString& source );

	/**
	 * @brief  メッセージ出力（WARNING）
	 * @param layer		[in] レイヤ
	 * @param oid		[in] オブジェクトID
	 * @param gid		[in] グローバルID
	 * @param errMsg	[in] メッセージ
	 * @param meshCode	[in] メッシュコード
	 * @param source	[in] 道路境界線リンクのSOURCE
	 */
	void warning( const CString& layer, const CString& oid, const CString& gid, const CString& errMsg, const CString& meshCode, const CString& source );

	/**
	 * @brief  メッセージ出力（INFO）
	 * @param layer		[in] レイヤ
	 * @param oid		[in] オブジェクトID
	 * @param gid		[in] グローバルID
	 * @param errMsg	[in] メッセージ
	 * @param meshCode	[in] メッシュコード
	 * @param source	[in] 道路境界線リンクのSOURCE
	 */
	void info( const CString& layer, const CString& oid, const CString& gid, const CString& errMsg, const CString& meshCode, const CString& source );

private:
	/**
	 * @brief  コンストラクタ
	 */
	CErrLogger(void) :CLogger(){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CErrLogger(void){}

	/**
	 * @brief  ヘッダー出力
	 */
	virtual void writeHeader() override;

};


namespace log_utility
{
	/**
	 * @brief  日時の情報を文字列で返す
	 * @return 日時情報 ( [YYYY-MM-DD HH:MM::SS] )
	 */
	CString GetDateTimeInfo();
}
