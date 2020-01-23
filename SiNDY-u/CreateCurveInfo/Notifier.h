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
#include <sindy/schema.h>

/**
 * @brief エラーなどの報告用機能群
 * @note	ソースの見通しを良くするため、報告用機能はここに集約する。
 */
namespace notifier
{
	using namespace std;
	static std::string fd = "\t";	// フィールドデリミタ

	// エラーレベル列挙型
	namespace error_level {
		enum ECode {
			fatal,
			error,
			warning,
			notice
		};
	}

	/**
	 * @brief 与えられたエラーレベルに対応するエラーレベル文字列を返す
	 * @param[in]	cErrorLevel	エラーレベル
	 * @return	エラーレベル文字列
	 */
	inline string getErrorLevelStr( error_level::ECode cErrorLevel )
	{
		string aErrorLevelStr;
		switch (cErrorLevel)
		{
			using namespace error_level;
			case fatal:
				aErrorLevelStr = "FATAL";
				break;
			case error:
				aErrorLevelStr = "ERROR";
				break;
			case warning:
				aErrorLevelStr = "WARNING";
				break;
			case notice:
				aErrorLevelStr = "NOTICE";
				break;
			default:
				ATLASSERT(0);
				break;
		}
		return aErrorLevelStr;
	}

	/**
	 * @brief 致命的なエラーが発生した時に呼ばれる関数群
	 * @note	std::runtime_error例外を発生させてプログラムを強制終了させる
	 */
	namespace critical
	{
		inline void asCannotGetWorkspace( std::string cDBStr )
		{
			throw std::runtime_error(
				std::string("ワークスペースの取得に失敗") + fd + cDBStr);
		}

		inline void asCannotGetTable( std::string cTableName )
		{
			throw std::runtime_error(
				std::string("テーブルの取得に失敗") + fd + cTableName);
		}

		inline void asCannotOpenMeshListFile(std::string cFilePath)
		{
			throw std::runtime_error(
				std::string("メッシュリストのオープンに失敗") + fd + cFilePath);
		}

		inline void asMeshListFileIsInvalid(long cLineNo)
		{
			throw std::runtime_error(
				std::string("メッシュリストが不正") + fd + "Line " + boost::lexical_cast<std::string>(cLineNo));
		}

		inline void asCannotOpenLogFile(std::string cPath)
		{
			throw std::runtime_error(
				std::string("ログファイルのオープンに失敗") + fd + cPath);
		}

		inline void asCannotCreateDirectory( std::string cPath )
		{
			throw std::runtime_error(
				std::string("出力ディレクトリの作成に失敗") + fd + cPath);
		}

		inline void asCannotSetSpatialReference()
		{
			throw std::runtime_error(
				std::string("空間参照のセットに失敗"));
		}
	}

	/**
	 * @brief 保険付きAssertion
	 * @note	Release版でも気付けるように、使用者向けのエラー出力をAssertion失敗時に行う。
	 * @note	チェックが堅牢になる代わりに、「パフォーマンスが犠牲になるのはDebug版のみ」という利点が損なわれるので、
	 *			使用の際は留意すること。
	 * @param[in]		cBool	Assertion内容を表す真偽値
	 * @param[in,out]	co		出力ストリーム
	 */
	inline void safeAssert( bool cBool, ostream& co )
	{
		// 引数が真なら何もしない
		if (cBool)
			{ return; }

		// 引数が偽なら、Assertionの失敗とエラー出力を行う
		ATLASSERT(0);
		co << "#" << fd << getErrorLevelStr( error_level::fatal )
			<< fd << "想定外のエラーです。これが出力されたら、開発者に連絡してください。" << endl;
	}

	/**
	 * @brief [Bug7560] メッシュ単位の処理中に例外が発生した場合のエラーメッセージ出力
	 * @param[in,out]	co			出力ストリーム
	 * @param[in]		cMeshCode	対象２次メッシュコード
	 */
	inline void asAbortedByUnexpectedData( ostream& co, long cMeshCode )
	{
		co << CT2A( sindy::schema::basemesh::kTableName ) << fd << cMeshCode << fd << getErrorLevelStr( error_level::fatal )
			<< fd << "ツールが予期していないデータの存在により、このメッシュの処理は中断されました。" << endl;
	}

	/**
	 * @brief [Bug9292] 指定されたメッシュについて、まとめて処理する際に例外が発生した場合のエラーメッセージ出力
	 * @param[in,out]	co			出力ストリーム
	 */
	inline void asAbortedByUnexpectedData( ostream& co )
	{
		co << CT2A( sindy::schema::basemesh::kTableName ) << fd << getErrorLevelStr( error_level::fatal )
			<< fd << "ツールが予期していないデータの存在により、処理は中断されました。" << endl;
	}

	/**
	 * @brief 区間が１つもないメッシュのShapeファイル作成をスキップした事を通知する
	 * @param[in,out]	co			出力ストリーム
	 * @param[in]		cMeshCode	当該メッシュコード
	 */
	inline void asMeshWithNoSection( ostream& co, long cMeshCode )
	{
		co << CT2A( sindy::schema::basemesh::kTableName ) << fd << cMeshCode << fd << getErrorLevelStr( error_level::notice )
			<< fd << "区間が１つも作成されなかったので、Shapeファイルの作成がスキップされました。" << endl;
	}

	/**
	 * @brief 対象リンクの数が0件であった事を通知する
	 * @param[in,out]	co			出力ストリーム
	 * @param[in]		cMeshCode	当該メッシュコード
	 */
	inline void asLinkNotFound( ostream& co, long cMeshCode )
	{
		co << CT2A( sindy::schema::basemesh::kTableName ) << fd << cMeshCode << fd << getErrorLevelStr( error_level::notice )
			<< fd << "対象リンク数が0件です。" << endl;
	}

} // namespace notifier
