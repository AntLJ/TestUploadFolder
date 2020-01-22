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

namespace sindy
{
/**
 * @class CArguments
 * @brief コマンドライン引数クラス
 * @file  Arguments.h
 * @author ビットエイジ　秦
 * $Id$
*/
class CArguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CArguments()
	{
	}

	/**
	 * @brief デストラクタ
	 */
	~CArguments(){}

	/**
	 * @brief コマンドライン引数の解析
	 * @param argc [in] コマンドライン引数の数
	 * @param argv [in] コマンドライン引数の内容
	 * @return 成功したかどうか
	*/
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief オプション引数一覧作成
	*/
	void GetOptionParam();

public:
	uh::tstring m_strDb;                    ///< 入力パラメータ 処理DB
	uh::tstring	m_strList;                  ///< 入力パラメータ 対象路線一覧ファイルパス
	uh::tstring	m_strRoadCost;              ///< 入力パラメータ 対象道路種別コスト定義ファイルパス
	uh::tstring	m_strLinkCost;              ///< 入力パラメータ 対象リンク種別コスト定義ファイルパス
	uh::tstring m_strPitch;                 ///< 入力パラメータ ピッチ
	uh::tstring	m_strLogPath;               ///< 入力パラメータ 作業ログ出力パス
	uh::tstring m_strBuf;                   ///< 入力パラメータ リンク特定バッファ
	uh::tstring m_strRange;                 ///< 入力パラメータ 許容範囲
	uh::tstring m_strSeparate;              ///< 入力パラメータ 2条分離許範囲

	CString	m_strOpParam;                   ///< オプション引数一覧文字列
};

}
