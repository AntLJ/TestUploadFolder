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
#include "ErrorDef.h"

#define RUNLOGGER RunLogger::getLogger()

/**
 * @brief  ログベースクラス
 */
class Logger
{
public:

	/**
	 * @brief  コンストラクタ
	 */
	Logger(void){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~Logger(void){ finalize(); }

public:

	/**
	 * @brief  ファイルオープン
	 */
	bool initialize( const CString& strDirPath );

	/**
	 * @brief  ファイルクローズ
	 */
	void finalize() { m_log.close(); }
	
	/**
	 * @brief  ファイルフラッシュ
	 */
	void flush() { m_log.flush(); }
	/**
	 * @brief  メッセージ出力
	 */
	void log( const CString& strMessage );

	/**
	 * @brief  メッセージ出力
	 */
	void log( const TCHAR* format, ... );

	/**
	 * @brief  << オペレータのオーバーロード
	 */
	Logger& operator<<( const CString& strMessage );

private:

	/**
	 * @brief  ヘッダー出力
	 */
	virtual void writeHeader() = 0;

	std::ofstream m_log;		//!< ログファイルストリーム

};


/**
 * @brief  実行ログクラス
 * @note   シングルトン
 */
class RunLogger : public Logger, 
                   private boost::noncopyable
{
public:

	/**
	 * @brief  静的オブジェクト取得
	 */
	static std::mutex s_mutex;
	static RunLogger& getLogger()
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		static RunLogger m_inst;
		return m_inst;
	}

private:
	
	/**
	 * @brief  コンストラクタ
	 */
	RunLogger( void ) :Logger(){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~RunLogger(void){}

	/**
	 * @brief  ヘッダー出力
	 */
	void writeHeader() override;

};


/**
 * @brief  エラーログクラス
 * @note   シングルトン
 */
class ErrLogger : public Logger, 
                   private boost::noncopyable
{
public:
		/**
	 * @brief  コンストラクタ
	 */
	ErrLogger(void) :Logger(){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~ErrLogger(void){}

	/**
	 * @brief 出力関数
	 * @param [in] emCode           エラーコード
 	 * @param [in] lpcszLayer       レイヤ名
	 * @param [in] oID              オブジェクトID
	 * @param [in] ipGeometry       ジオメトリ(エラー位置)
	 * @param [in] lpcszFormat      情報2-nの出力フォーマット(１つならここに値を入れる)
	 * @param [in] ...              情報2-n
	 */
	void write( emCheck_ErrCode emCode, LPCTSTR lpcszLayer, long oID, const IGeometryPtr& ipGeometry, LPCTSTR lpcszFormat = NULL, ...);

private:
	
	/**
	 * @brief  ヘッダー出力
	 */
	void writeHeader() override;

	/**
	 * @brief	出力関数実体
	 * @param	[in] cErrInfo        エラー情報
	 * @param	[in] lpcszFormat     情報2-nの出力フォーマット
	 * @param	[in] vaInfo          情報2-n
	 */
	void writeCommon( const ErrInfo& cErrInfo, LPCTSTR lpcszFormat = NULL, va_list vaInfo = NULL );

		/**
	 * @brief	エラーレベル取得
	 * @param	[in] emLevel		エラーレベル
	 * @return	エラーレベル
	 */
	LPCTSTR getErrorLevel( emErrLevel emLevel ) const;

	/**
	 * @brief	エラーメッセージ出力
	 * @param	[in] emCode		エラーコード
	 * @return	エラーレベル<tab>エラーコード<tab>エラーメッセージ
	 */
	CString getErrorString( emCheck_ErrCode emCode ) const;

	/**
	 * @brief	経度・緯度取得
	 * @param	[in]  ipGeometry    ジオメトリ
	 * @param	[out] point         WKSPoint
	 */
	void getLonLat( const IGeometryPtr& ipGeometry, WKSPoint& point )const;
};
