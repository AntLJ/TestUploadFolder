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
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <boost/optional.hpp>

/**
 * @brief エラーなどの報告用機能群
 * @note	ソースの見通しを良くするため、報告用機能はここに集約する。
 */
namespace notifier
{
	using namespace std;
	static string fd = "\t";
	static wstring wfd = L"\t";

	/// エラーレベル列挙型
	enum class error_level {
		fatal,
		error,
		warning,
		notice,
		none,
	};

	/**
	 * @brief 与えられたエラーレベルに対応するエラーレベル文字列を返す
	 * @param[in]	errorLevel	エラーレベル
	 * @return	エラーレベル文字列
	 */
	inline string getErrorLevelStr( error_level errorLevel )
	{
		string aErrorLevelStr;
		switch (errorLevel)
		{
			case error_level::fatal:
				aErrorLevelStr = "FATAL";
				break;
			case error_level::error:
				aErrorLevelStr = "ERROR";
				break;
			case error_level::warning:
				aErrorLevelStr = "WARNING";
				break;
			case error_level::notice:
				aErrorLevelStr = "NOTICE";
				break;
			default:
				assert(0);
				break;
		}
		return aErrorLevelStr;
	}

	/**
	 * @brief 与えられた文字列に対応するエラーレベルを返す
	 * @warning 大小文字は区別しない
	 * @param[in]	errorLevelStr	エラーレベル文字列
	 * @return	エラーレベル
	 */
	inline error_level getErrorLevel( const std::string& errorLevelStr )
	{
		if(0==stricmp(errorLevelStr.c_str(), "FATAL"))
			return error_level::fatal;
		if(0==stricmp(errorLevelStr.c_str(), "ERROR"))
			return error_level::error;
		if(0==stricmp(errorLevelStr.c_str(), "WARNING"))
			return error_level::warning;
		if(0==stricmp(errorLevelStr.c_str(), "NOTICE"))
			return error_level::notice;
		return error_level::none;
	}

	/**
	 * @brief 致命的なエラーが発生した時に呼ばれる関数群
	 * @exception std::runtime_error例外を発生させてプログラムを強制終了させる。
	 */
	namespace critical
	{
		/**
		 * @brief 汎用エラー処理関数
		 * @note 指定されたエラーメッセージを持つ例外を送出する。
		 * @param[in]	message	エラーメッセージ
		 */
		inline void versatile( const uh::tstring& message )
		{
			wcerr << L"# ERROR : " << message << endl;
			wcout << L"# 異常終了" << endl;

			throw std::runtime_error( uh::toStr(message) );
		}

		/**
		 * @param[in]	tableName	テーブル名
		 */
		inline void asCannotOpenTable( const uh::tstring& tableName )
		{
			versatile(
				_T("テーブルのオープンに失敗") + uh::toTStr(fd) + tableName );
		}

		/**
		 * @param[in]	path	ログファイルのパス
		 */
		inline void asCannotOpenLogFile(const uh::tstring& path)
		{
			versatile(
				_T("ログファイルのオープンに失敗") + uh::toTStr(fd) + path );
		}

		/**
		 * @param[in]      db    対象DB接続先文字列
		 */
		inline void asCannotConnectDB( const uh::tstring& db )
		{
			std::stringstream ss;
			ss << "DBへの接続失敗 : \"" << uh::toStr( db ) << "\"";
			versatile( uh::toTStr(ss.str()) );
		}
	} // namespace critical


	/**
	 * @brief 汎用通知処理関数
	 * @note 指定された通知メッセージを出力し、処理を続行する。
	 * @param[in]  message  通知メッセージ
	 * @param[in]  wos      出力ストリーム
	 */
/*	inline void versatile(
		const uh::tstring& message,
		std::wostream& wos = wcerr
	)
	{
		wos << L"# " << message << endl;
	}*/

	/**
	 * @brief 汎用通知処理関数
	 * @note 指定された通知メッセージを出力し、処理を続行する。
	 * @param[in]  message  通知メッセージ
	 * @param[in]  wos      出力ストリーム
	 */
	inline void versatile(
		const uh::tstring& message,
		std::wostream& wos = wcerr
	)
	{
		wos << L"# " << message.c_str() << endl;
	}

	/**
	 * @brief オブジェクトの情報を出力する
	 * @note 書式はSINDYSTDLOG。出力する必要のない列には何も出力しない。
	 * @param[in]      tableName  テーブル名
	 * @param[in]      oid        オブジェクトID（出力しない場合は無効値）
	 * @param[in]      x          経度（出力しない場合は無効値）
	 * @param[in]      y          緯度（出力しない場合は無効値）
	 * @param[in]      errorLevel エラーレベル
	 * @param[in]      errorCode  エラーコード
	 * @param[in]      errorMsg   エラーメッセージ
	 * @param[in]      meshcode   メッシュコード
	 * @param[in,out]  wos        出力先ストリーム
	 */
	inline void reportObjInfo(
		const uh::tstring& tableName,
		boost::optional< long > oid,
		boost::optional< double > x,
		boost::optional< double > y,
		boost::optional< error_level > errorLevel,
		boost::optional< const uh::tstring& > errorCode,
		const uh::tstring& errorMsg,
		boost::optional< const uh::tstring& > meshcode,
		std::wostream& wos
	)
	{
		// 座標の有効桁数指定
		wos << std::setprecision( 20 );

		// 対応状況
		wos << wfd;
		// レイヤ名
		wos << uh::toWStr( tableName ) << wfd;
		// オブジェクトID
		if ( oid )
			wos << *oid << wfd;
		else
			wos << wfd;
		// 経度
		if( x )
			wos << *x << wfd;
		else
			wos	<< wfd;
		// 緯度
		if( y )
			wos << *y << wfd;
		else
			wos << wfd;
		// エラーレベル
		if ( errorLevel )
			wos << uh::toWStr( getErrorLevelStr( *errorLevel ) );
		wos << wfd;
		// エラーコード
		if ( errorCode )
			wos << *errorCode;
		wos << wfd;
		// エラーメッセージ
		wos << uh::toWStr( errorMsg );
		// メッシュコード(情報1)
		if ( meshcode )
			wos << wfd << uh::toWStr(*meshcode);

		wos <<  endl;
		// マニピュレータを元に戻す
		wos << resetiosflags(ios_base::floatfield);
	}

	/**
	 * @brief オブジェクトの情報を出力する
	 * @note 書式はSINDYSTDLOG。出力する必要のない列には何も出力しない。
	 * @param[in]      tableName  テーブル名
	 * @param[in]      oid        オブジェクトID（出力しない場合は無効値）
	 * @param[in]      x          X座標（出力しない場合は無効値）
	 * @param[in]      y          Y座標（出力しない場合は無効値）
	 * @param[in]      errorLevel エラーレベル
	 * @param[in]      errorMsg   エラーメッセージ
	 * @param[in,out]  wos        出力先ストリーム
	 */
	inline void reportObjInfo(
		const uh::tstring& tableName,
		boost::optional< long > oid,
		boost::optional< double > x,
		boost::optional< double > y,
		boost::optional< error_level > errorLevel,
		const uh::tstring& errorMsg,
		std::wostream& wos)
	{
		reportObjInfo( tableName, oid, x, y, errorLevel, nullptr, errorMsg, nullptr, wos );
	}


	/**
	 * @brief オブジェクトの情報を出力する
	 * @note 書式はSINDYSTDLOG。出力する必要のない列には何も出力しない。
	 * @param[in]      tableName  テーブル名
	 * @param[in]      oid        オブジェクトID（出力しない場合は無効値）
	 * @param[in]      errorLevel エラーレベル
	 * @param[in]      errorMsg   エラーメッセージ
	 * @param[in,out]  wos        出力先ストリーム
	 */
	inline void reportObjInfo(
		const uh::tstring& tableName,
		boost::optional< long > oid,
		boost::optional< error_level > errorLevel,
		const uh::tstring& errorMsg,
		std::wostream& wos
	)
	{
		reportObjInfo( tableName, oid, nullptr, nullptr, errorLevel, errorMsg, wos );
	}

	/**
	 * @brief オブジェクトの情報を出力する（非エラーオブジェクト用）
	 * @param[in]      tableName  テーブル名
	 * @param[in]      oid        オブジェクトID（出力しない場合は無効値）
	 * @param[in]      msg        メッセージ（エラーではないが、SINDYSTDLOGの「エラーメッセージ」列に出力する）
	 * @param[in,out]  wos        出力先ストリーム
	 */
	inline void reportObjInfo(
		const uh::tstring& tableName,
		boost::optional< long > oid,
		const uh::tstring& msg,
		std::wostream& wos
	)
	{
		reportObjInfo(
			tableName,
			oid,
			boost::none,
			msg,
			wos
		);
	}

} // namespace notifier
