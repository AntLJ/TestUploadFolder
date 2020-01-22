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

#include <TDC/useful_headers/boost_program_options.h>
namespace po = boost::program_options;

/**
* @brief  引数格納用コンテナ
*/
struct Arguments
{
	uh::tstring convMode;       //!< オプション：city,addr,all
	uh::tstring transResult;    //!< 翻訳結果フォルダ
	uh::tstring out;            //!< 変換結果フォルダ
	uh::tstring runLog;         //!< 実行ログファイル
	uh::tstring errLog;         //!< エラーログファイル
};

/**
 * @brief  引数タイプ
 */
enum argumentType
{
	kConvMode       = 1,
	kTransResult    = 2,
	kOut            = 3,
	kRunLog         = 4,
	kErrLog         = 5,
};

/**
 * @class  Argument
 * @brief  コマンドライン引数の処理
 */
class Argument
{
public:
	/**
	* @brief  コンストラクタ
	*/
	Argument(void) {};
	
	/**
	* @brief  デストラクタ
	*/
	virtual ~Argument(void) {};

	/**
	* @brief  必要なオプション情報があるかをチェックする
	* @param  [in] vm           引数のmap
	* @param  [in] option       オプション名称
	* @param  [in] desctiption  オプションの記述
	* @retval true              必要なオプションあり
	* @retval false             必要なオプションなし
	*/
	bool checkParameter(const po::variables_map& vm, LPCSTR option, LPCSTR desctiption) const;

	/**
	* @brief  コマンドライン引数の解析
	* @param  [in] argc         コマンドライン引数の数
	* @param  [in] argv         コマンドライン引数の内容
	* @param  [in] runLogger    実行ログファイル
	* @param  [in] errLogger    エラーログファイル
	* @retval true              成功
	* @retval false             失敗
	*/
	bool setParameter(int argc, TCHAR** argv, FileController& runLogger, FileController& errLogger);

	/**
	* @brief  引数の取得
	* @param  [in] type         引数タイプ
	* @return 引数の内容
	*/
	const CStringW& getValue(argumentType type) const;

	CStringW   m_convMode;      //!< オプション：city,addr,all
	CStringW   m_transResult;   //!< 翻訳結果フォルダ
	CStringW   m_out;           //!< 変換結果フォルダ
	CStringW   m_runLog;        //!< 実行ログファイル
	CStringW   m_errLog;        //!< エラーログファイル
};