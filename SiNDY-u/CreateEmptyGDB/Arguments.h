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
#include "DataDef.h"

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments();

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief outputタイプを取得
	 * @return outputタイプ
	 */
	OUTPUT_TYPE getOutputType() const { return m_outputType; }

protected:
	/**
	 * @brief  必須となるコマンドライン引数の確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @param  bFile    [in]  ファイルであるか（デフォルトfalse）
	 * @retval true    引数OK
	 * @retval false   引数NG
	 */
	bool checkParam( const char* const optName, const uh::tstring& optValue, bool bFile = false ) const;

	/**
	 * @brief 引数がファイルであることを確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @retval true  ファイルである
	 * @retval false ファイルではない
	 */
	bool checkFile( const char* const optName, const uh::tstring& optValue ) const;

	/**
	 * @brief ファイルパスがツールで対応している拡張子をもっているか確認
	 * @param path [in] 確認するファイルパス
	 */
	bool checkFormat( const uh::tstring& path ) const;

public:
	uh::tstring m_input;         //!< 入力情報
	uh::tstring m_output;        //!< 出力情報
	uh::tstring m_meshList;      //!< メッシュリスト
	uh::tstring m_meshDB;        //!< メッシュ参照先のDB
	uh::tstring m_meshLayerName; //!< メッシュレイヤ名
	uh::tstring m_dbFormat;      //!< 生成するDBのフォーマット
	uh::tstring m_config;        //!< 定義ファイルパス
	uh::tstring m_runLog;        //!< 実行ログファイルパス
	uh::tstring m_errLog;        //!< エラーログファイルパス

protected:
	OUTPUT_TYPE m_outputType;  //!< 解析した出力先タイプを格納
};
