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

#include <boost/program_options.hpp>

/**
 * @brief ログ管理クラス（シングルトン）
 */
class LogMgr
{
public:
	/**
	 * @brief エラーレベル
	 */
	enum EErrLevel
	{
		kNone	= 0,	//!< エラーレベルなし
		kInfo	= 1,	//!< 情報
		kWarn	= 2,	//!< 警告
		kError	= 3,	//!< エラー
		kFatal	= 4,	//!< 致命的なエラー
	};

private:
	/**
	 * @brief コンストラクタ
	 */
	LogMgr();

	/**
	 * @brief コピーコンストラクタ
	 * @param logMgr [in] コピー元オブジェクト
	 */
	LogMgr( const LogMgr& logMgr );

	/**
	 * @brief デストラクタ
	 */
	~LogMgr();

	/**
	 * @brief 代入演算子
	 * @param logMgr [in] コピー元オブジェクト
	 * @return クラスオブジェクト自身
	 */
	LogMgr& operator = ( const LogMgr& logMgr );

	/**
	 * @brief バージョン情報出力
	 */
	void writeVersion();

	/**
	 * @brief 開始時刻出力
	 */
	void writeStartTime();

	/**
	 * @brief 終了時刻出力
	 */
	void writeEndTime();

public:
	/**
	 * @brief 実行ログファイルを設定
	 * @param strFileName [in] 実行ログファイル名
	 * @param varMap [in] オプション格納マップ（オプション）
	 * @return 成功したかどうか
	 */
	bool setRunLog( const CString& strFileName, const boost::program_options::variables_map* varMap = nullptr );

	/**
	 * @brief エラーログファイルを設定
	 * @param strFileName [in] エラーログファイル名
	 * @return 成功したかどうか
	 */
	bool setErrLog( const CString& strFileName );

	/**
	 * @brief オプション情報の出力
	 * @param varMap [in] オプション格納マップ
	 */
	void writeOptionInfos( const boost::program_options::variables_map& varMap );

	/**
	 * @brief インスタンスの取得
	 * @return インスタンス
	 */
	static LogMgr& getInstance()
	{
		static LogMgr logMgr;
		return logMgr;
	}

	/**
	 * @brief 実行ログファイルへの出力
	 * @param strMessage [in] 出力メッセージ
	 */
	void writeRunLog( const CString& strMessage );

	/**
	 * @brief エラーログファイルへの出力
	 * @param strMessage [in] 出力メッセージ
	 */
	void writeErrLog( const CString& strMessage );

	/**
	 * @brief エラーログファイルへの出力
	 * @param errLv [in] エラーレベル
	 * @param strMessage [in] 出力メッセージ
	 */
	void writeErrLog( EErrLevel errLv, const CString& strMessage );

	/**
	 * @brief エラーログレベルを文字列に変換
	 * @param errLv [in] エラーレベル
	 * @return エラーレベルを表現する文字列
	 */
	static CString getErrLvString( EErrLevel errLv );

	/**
	 * @brief バージョン情報文字列を取得
	 * @note 「<実行ファイル名> F: x.x.x.x P: x.x.x.x」の形式で取得
	 * @return バージョン情報文字列
	 */
	static CString getVersionInfo();

private:
	std::ofstream	m_ofsRun;	//!< 実行ログ出力ファイルストリーム
	std::ofstream	m_ofsErr;	//!< エラーログ出力ファイルストリーム
};
