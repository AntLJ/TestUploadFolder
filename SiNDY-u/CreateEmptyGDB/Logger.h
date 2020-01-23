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
	CLogger(){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CLogger(){ Close(); }

public:

	/**
	 * @brief  ファイルオープン
	 * @param[in] filepath ファイルパス
	 * @retval true  ファイルのオープンに成功
	 * @retval false ファイルのオープンに失敗
	 */
	bool Initialize( const uh::tstring& filepath );

	/**
	 * @brief  ファイルクローズ
	 */
	void Close() { m_Log.close(); }

	/**
	 * @brief  メッセージ出力
	 * @param[in] strMessage 出力するメッセージ
	 * @param[in] lineBreak 改行するか否か（デフォルト:true）
	 */
	virtual void Log( const uh::tstring& strMessage, bool lineBreak = true );

	/**
	 * @brief 1要素を1行としてメッセージ出力
	 * @param[in] strMessages 出力するメッセージ群
	 */
	virtual void Log( const std::vector<uh::tstring>& strMessages);

	/**
	 * @brief  メッセージ出力
	 * @detail 改行されない。改行する場合はフォーマット中に改行文字を入れる。
	 * @param[in] format 出力するメッセージのフォーマット
	 * (printfの書式と同じ)
	 */
	virtual void FormatLog( const TCHAR* format, ... );

	/**
	 * @brief  << オペレータのオーバーロード
	 */
	virtual CLogger& operator<<( const uh::tstring& strMessage );

private:
	/**
	 * @brief  ヘッダー出力
	 */
	virtual void writeHeader() = 0;

protected:

	std::wofstream m_Log;		//!< ログファイルストリーム

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
	 * @brief FATALエラー付きのメッセージ出力
	 * @param stringTableId [in] StringTableのID
	 * @param dbPath        [in] エラーがあったDBパス
	 * @param layerName     [in] エラーがあったレイヤ名
	 * @param columnName    [in] エラーがあったカラム名
	 * @param comment       [in] その他出力する文字列
	 */
	void fatal( long stringTableId
		, const uh::tstring& dbPath
		, const uh::tstring& layerName
		, const uh::tstring& columnName
		, const uh::tstring& comment = uh::tstring());

	/**
	 * @brief XML読込み用のエラー付きのメッセージ出力
	 * @param stringTableId [in] StringTableのID
	 * @param tag       [in] エラーのあるタグ名
	 * @param attrName  [in] エラーのある属性名
	 * @param attrValue [in] 属性に指定された値
	 */
	void fatalXML( long stringTableId
		, const uh::tstring& tag
		, const uh::tstring& attrName
		, const uh::tstring& attrValue);

	/**
	 * @brief ERRORエラー付きのメッセージ出力
	 * @param stringTableId [in] StringTableのID
	 * @param dbPath        [in] エラーがあったDBパス
	 * @param layerName     [in] エラーがあったレイヤ名
	 * @param columnName    [in] エラーがあったカラム名
	 * @param comment       [in] その他出力する文字列
	 */
	void error( long stringTableId
		, const uh::tstring& dbPath
		, const uh::tstring& layerName
		, const uh::tstring& columnName
		, const uh::tstring& comment = uh::tstring());

	/**
	 * @brief Warningエラー付きのメッセージ出力
	 * @param stringTableId [in] StringTableのID
	 * @param dbPath        [in] エラーがあったDBパス
	 * @param layerName     [in] エラーがあったレイヤ名
	 * @param columnName    [in] エラーがあったカラム名
	 * @param comment       [in] その他出力する文字列
	 */
	void warn( long stringTableId
		, const uh::tstring& dbPath
		, const uh::tstring& layerName
		, const uh::tstring& columnName
		, const uh::tstring& comment = uh::tstring());

	/**
	 * @brief FATALエラー付きのメッセージ出力
	 * @param[in] format 出力するメッセージのフォーマット
	 * (printfの書式と同じ)
	 */
	void fatal( const TCHAR* format, ... );
	/**
	 * @brief エラー付きのメッセージ出力
	 * @param[in] format 出力するメッセージのフォーマット
	 * (printfの書式と同じ)
	 */
	void error( const TCHAR* format, ... );
	/**
	 * @brief Warning付きのメッセージ出力
	 * @param[in] format 出力するメッセージのフォーマット
	 * (printfの書式と同じ)
	 */
	void warn( const TCHAR* format, ... );
	/**
	 * @brief Info付きのメッセージ出力
	 * @param[in] format 出力するメッセージのフォーマット
	 * (printfの書式と同じ)
	 */
	void info( const TCHAR* format, ... );
	/**
	 * @brief Debug付きのメッセージ出力
	 * @param[in] format 出力するメッセージのフォーマット
	 * (printfの書式と同じ)
	 */
	void debug( const TCHAR* format, ... );

	/**
	 * @brief 現在時刻を出力
	 * @param[in] comment 時刻の前に挿入するコメント<br>
	 * "開始時刻 : "など
	 */
	void writeTime( const uh::tstring& comment = uh::tstring() );

	/**
	 * @brief 終了時にログに異常終了と通知する
	 * @note ERROR以上の異常があった場合に呼び出す。
	 */
	void setFail();

protected:
	/**
	 * @brief ログと標準エラー出力に実行結果を出力し、終了時刻をログに出力
	 */
	void endLog();

	/**
	 * @brief エラーレベル付きのメッセージ出力の共通部を定義
	 * @param stringTableId [in] StringTableのID
	 * @param dbPath        [in] エラーがあったDBパス
	 * @param layerName     [in] エラーがあったレイヤ名
	 * @param columnName    [in] エラーがあったカラム名
	 * @param comment       [in] その他出力する文字列
	 */
	void writeError( const uh::tstring& strLevel, long stringTableId
		, const uh::tstring& dbPath
		, const uh::tstring& layerName
		, const uh::tstring& columnName
		, const uh::tstring& comment);

	/**
	 * @brief エラーレベル付きのメッセージ出力の共通部を定義
	 * @param[in] strLevel エラーレベル文字列
	 * @param[in] strMsg   エラーメッセージ
	 */
	void writeError( const uh::tstring& strLevel, const uh::tstring& strMsg );

private:
	/**
	 * @brief  コンストラクタ
	 */
	CRunLogger():m_bFail(false){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CRunLogger(){ endLog(); }

	/**
	 * @brief  ヘッダー出力
	 */
	virtual void writeHeader() override;

	bool m_bFail; //!< 終了時にログに異常終了と出力するか否か
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
	CErrLogger(){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CErrLogger(){}

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
	uh::tstring getDateTimeInfo();
}
