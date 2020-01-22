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
#include "Logger.h"
#include "Arguments.h"
#include "MeshType.h"

/**
 * @brief 実行ログ管理クラス
 */
class CRunLogger :
	public CLogger
{
public:
	explicit CRunLogger(const uh::tstring& path) : CLogger(path){}

	/**
	 * @brief ヘッダを出力する
	 * @note バイナリのバージョンや、実行時に指定されたオプションを出力する
	 * @param args[in] コマンドライン引数・メッシュコード管理クラス
	 */
	void header( const Arguments& args );

	/**
	 * @brief メッシュリストの解析結果を出力する
	 * @note 処理するメッシュ単位を出力し、あればメッシュリスト内の不正な文字列を出力
	 * @param type[in] 処理メッシュ単位
	 * @param mapInvalidRow[in] メッシュリスト内のすべての不正な文字列
	 */
	void writeParseMeshResult( const meshtype::MeshType& type, const std::map<int, CString>& mapInvalidRow );

	/**
	 * @brief 開始・終了時間を出力する
	 * @note isStartの値に応じて、開始時刻か終了時刻かを判定する
	 * @param isStart[in] trueなら開始時刻、falseなら終了時刻
	 */
	void writeExeTime( bool isStart );

	/**
	 * @brief エラーメッセージを出力する
	 * @note 処理中に発生したエラーの内容を出力する
	 * @param msg[in] エラーメッセージ
	 */
	void writeErrMsg( const CString& msg );

	/**
	 * @brief ログファイルとコンソールの両方にメッセージを出力する
	 * @param msg[in] メッセージ
	 */
	void writeLogAndConsole( const CString& msg );
};

